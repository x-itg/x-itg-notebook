#include <thread>
#include <portaudio.h>
#include "audio.h"


#include "avpackets_queue.h"
#include "av_clock.h"
#include "video.h"
#include "log.h"

#define AVCODE_MAX_AUDIO_FRAME_SIZE	192000  /* 1 second of 48khz 32bit audio */

const double kPortAudioLatency  = 0.3;  // 0.3秒，也就是说PortAudio库需要累积0.3秒的数据后才真正播放

#define ERR_STREAM	stderr


typedef struct audio_param
{
    AVFrame wantFrame;			// 指定PCM输出格式

    PaStreamParameters *outputParameters;
    PaStream *stream;
    int sampleRate;
    int format;
    PacketQueue audioQueue;     // 音频队列
    int packetEof;              // 数据包已经读取到最后
    int quit;                   // 是否退出线程
    double audioBaseTime;       // 音频base time
    SwrContext	*swrCtx;        // 音频PCM格式转换
}T_AudioParam;

static T_AudioParam sAudioParam;


static int _AudioDecodeFrame( AVCodecContext *pAudioCodecCtx, uint8_t *audioBuf, int bufSize , int *packeEof);


void AudioDecodeThread( void *userdata )
{
    FunEntry();

    AVCodecContext	*pAudioCodecCtx = (AVCodecContext *) userdata;
    int		len1		= 0;
    int		audio_size	= 0;
    uint8_t audioBuf[AVCODE_MAX_AUDIO_FRAME_SIZE];

    AVClockDisable();       // 禁止时钟
    while ( sAudioParam.quit != 1 )
    {
        if(sAudioParam.packetEof && sAudioParam.audioQueue.size == 0)
        {
            sAudioParam.quit = 1;
        }
        audio_size = 0;
        if(VideoGetFirstFrame())            // 等图像出来后再出声音
        {
            audio_size = _AudioDecodeFrame( pAudioCodecCtx, audioBuf, sizeof(audioBuf), &sAudioParam.packetEof);
        }
        if ( audio_size > 0 )
        {
            AudioPlay(audioBuf, audio_size, 0);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));     // 没有数据时先休眠20毫秒
        }
    }


    FunExit();
}


// 对于音频来说，一个packet里面，可能含有多帧(frame)数据
/**
 * @brief _AudioDecodeFrame
 * @param pAudioCodecCtx
 * @param audioBuf
 * @param bufSize
 * @param packeEof
 * @return
 */
int _AudioDecodeFrame( AVCodecContext *pAudioCodecCtx,
                       uint8_t *audioBuf, int bufSize, int *packeEof)
{
    AVPacket	packet;
    AVFrame		*pFrame = NULL;
    int		gotFrame = 0;
    int		decodeLen  = 0;
    long 	audioBufIndex = 0;
    int		convertLength	= 0;
    int		convertAll	= 0;

    if ( PacketQueueTake( &sAudioParam.audioQueue, &packet, 1 ) < 0 )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));     // 没有取到数据休眠
        return(-1);
    }

    pFrame = av_frame_alloc();
    *packeEof = packet.size ? 0: 1;       // 约定使用数据长度为0时来标记packet已经读取完毕

    while ( packet.size > 0 )
    {
        /*
         * pAudioCodecCtx:解码器信息
         * pFrame:输出，存数据到frame
         * gotFrame:输出。0代表有frame取了，不意味发生了错误。
         * packet:输入，取数据解码。
         */
        decodeLen = avcodec_decode_audio4( pAudioCodecCtx, pFrame, &gotFrame, &packet );
        if(decodeLen < 0)
        {
            LogError("avcodec_decode_audio4 failed");
            break;
        }
        if ( gotFrame )
        {
            if(!AVClockIsEnable())
            {
                int64_t pts = sAudioParam.audioBaseTime * pFrame->pts * 1000;
                // 设置同步时钟，因为PortAudio Latency设置为了kPortAudioLatency秒，所以这里需要进去这个时间，避免视频提前kPortAudioLatency秒
                AVClockResetTime(pts - (int64_t)(kPortAudioLatency*1000));    // 单位为毫秒
                LogInfo("pts = %lld, clk = %lld", pts, AVClockGetCurTime());
                LogInfo("audio base = %lf", sAudioParam.audioBaseTime);
                AVClockEnable();
            }

            // 针对特殊情况推算channel_layout或者channels的参数值
            if ( pFrame->channels > 0 && pFrame->channel_layout == 0 )
            {
                pFrame->channel_layout = av_get_default_channel_layout( pFrame->channels );
            }
            else if ( pFrame->channels == 0 && pFrame->channel_layout > 0 )
            {
                pFrame->channels = av_get_channel_layout_nb_channels( pFrame->channel_layout );
            }

            if ( sAudioParam.swrCtx != NULL )
            {
                swr_free( &sAudioParam.swrCtx );
                sAudioParam.swrCtx = NULL;
            }

            // 配置转换器
            sAudioParam.swrCtx = swr_alloc_set_opts( NULL,
                                                     sAudioParam.wantFrame.channel_layout,
                                                     (enum AVSampleFormat) (sAudioParam.wantFrame.format),
                                                     sAudioParam.wantFrame.sample_rate,
                                                     pFrame->channel_layout,
                                                     (enum AVSampleFormat) (pFrame->format),
                                                     pFrame->sample_rate,
                                                     0, NULL );
            if ( sAudioParam.swrCtx == NULL || swr_init( sAudioParam.swrCtx ) < 0 )
            {
                LogError( "swr_init error" );
                break;
            }
            // 转换为PortAudio输出需要的数据格式
            convertLength = swr_convert( sAudioParam.swrCtx,
                                         &audioBuf + audioBufIndex,
                                         AVCODE_MAX_AUDIO_FRAME_SIZE,
                                         (const uint8_t * *) pFrame->data,
                                         pFrame->nb_samples );


            // 转换后的有效数据存储到哪里
            audioBufIndex += convertLength;
            /* 返回所有转换后的有效数据的长度 */
            convertAll += convertLength;    // 该packet解出来并做转换后的数据总长度
            av_frame_unref(pFrame);
            if ( sAudioParam.swrCtx != NULL )
            {
                swr_free( &sAudioParam.swrCtx );       // 不释放导致的内存泄漏
                sAudioParam.swrCtx = NULL;
            }
        }
        if(decodeLen > 0)
        {
            packet.size  -= decodeLen ;  // 计算当前解码剩余的数据
            if(packet.size > 0)
                LogInfo("packet.size = %d,  orig size = %d", packet.size, packet.size + decodeLen);
            packet.data += decodeLen;   // 改变数据起始位置
        }
    }
exit_:
    av_packet_unref(&packet);
    av_frame_free(&pFrame);

    return (sAudioParam.wantFrame.channels * convertAll * av_get_bytes_per_sample( (enum AVSampleFormat) (sAudioParam.wantFrame.format) ) );
}


int AudioInit(AVCodecContext	*pAudioCodecCtx)
{
    FunEntry();
    int ret;

    //1 初始化音频输出设备
    //memset(&sAudioParam, 0, sizeof(sAudioParam));
    // 初始化音频输出设备，调用PortAudio库
    Pa_Initialize();
    // 分配PaStreamParameters
    sAudioParam.outputParameters = (PaStreamParameters *)malloc(sizeof(PaStreamParameters));
    sAudioParam.outputParameters->suggestedLatency = kPortAudioLatency;       // 设置latency为0.3秒
    sAudioParam.outputParameters->sampleFormat = paFloat32;     // PCM格式
    sAudioParam.outputParameters->hostApiSpecificStreamInfo = NULL;
    // 设置音频信息, 用来打开音频设备
    sAudioParam.outputParameters->channelCount = pAudioCodecCtx->channels;      // 通道数量
    if(sAudioParam.outputParameters->sampleFormat == paFloat32)                 // 目前PortAudio我们只处理paFloat32和paInt16两种格式
    {
        sAudioParam.format = AV_SAMPLE_FMT_FLT;
    }
    else if(sAudioParam.outputParameters->sampleFormat == paInt16)
    {
        sAudioParam.format = AV_SAMPLE_FMT_S16;
    }
    else
    {
        sAudioParam.format = AV_SAMPLE_FMT_S16;
    }
    sAudioParam.sampleRate = pAudioCodecCtx->sample_rate;

    // 获取音频输出设备
    sAudioParam.outputParameters->device = Pa_GetDefaultOutputDevice();
    if(sAudioParam.outputParameters->device < 0)
    {
        LogError("Pa_GetDefaultOutputDevice failed, index = %d", sAudioParam.outputParameters->device);
    }
    // 打开一个输出流
    if((ret = Pa_OpenStream( &(sAudioParam.stream), NULL, sAudioParam.outputParameters, pAudioCodecCtx->sample_rate, 0, 0, NULL, NULL )) != paNoError)
    {
        LogError("Pa_OpenStream open failed, ret = %d", ret);
    }

    // 设置PortAudio需要的格式
    sAudioParam.wantFrame.format		= sAudioParam.format;
    sAudioParam.wantFrame.sample_rate	=  sAudioParam.sampleRate;
    sAudioParam.wantFrame.channel_layout	= av_get_default_channel_layout(sAudioParam.outputParameters->channelCount);
    sAudioParam.wantFrame.channels		= sAudioParam.outputParameters->channelCount;

    // 初始化音频队列
    PacketQueueInit( &sAudioParam.audioQueue );
    sAudioParam.packetEof = 0;

    // 初始化音频PCM格式转换器
    sAudioParam.swrCtx = NULL;

    FunExit();
    return 0;
}

void AudioPacketPush(AVPacket *packet)
{
    if(PacketQueuePut( &sAudioParam.audioQueue, packet ) != 0)
    {
        LogError("PacketQueuePut failed");
    }
}

int AudioPacketSize()
{
    return PacketQueueGetSize(&sAudioParam.audioQueue);
}

void AudioPlay( const uint8_t *data, const uint32_t size,  int stop )
{
    int chunkSize;
    int chunk = ( int )( sAudioParam.sampleRate * 0.02 ) * sAudioParam.outputParameters->channelCount * sizeof( short );
    int offset = 0;
    if ( !Pa_IsStreamActive( sAudioParam.stream ) )
        Pa_StartStream( sAudioParam.stream );
    while ( !stop && size > offset )
    {
        if( chunk < size - offset )
        {
            chunkSize = chunk;
        }
        else
        {
            chunkSize = size - offset;
        }

        if ( Pa_WriteStream( sAudioParam.stream, data + offset, chunkSize / sAudioParam.outputParameters->channelCount / sizeof( float ) ) == paUnanticipatedHostError )
            break;
        offset += chunkSize;
    }
}

void AudioClose(void)
{
    FunEntry();
    if(sAudioParam.outputParameters)
    {
        if ( sAudioParam.stream )
        {
            Pa_StopStream( sAudioParam.stream );
            Pa_CloseStream( sAudioParam.stream );
            sAudioParam.stream = NULL;
        }
        Pa_Terminate();
        free(sAudioParam.outputParameters);
    }
    if(sAudioParam.swrCtx)
    {
        swr_free( &sAudioParam.swrCtx );
    }
    FunExit();
}

void AudioSetTimeBase(double timeBase)
{
    sAudioParam.audioBaseTime = timeBase;
}


