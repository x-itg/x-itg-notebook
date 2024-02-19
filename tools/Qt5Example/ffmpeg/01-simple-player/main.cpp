#include <QCoreApplication>
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "log.h"
#include <QDebug>
#define __STDC_CONSTANT_MACROS      //ffmpeg要求

/* 必须记住，使用C++编译器时一定要使用extern "C"，否则找不到链接文件 */
#ifdef __cplusplus
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <SDL2/include/SDL.h>
//#include <SDL2/include/SDL_thread.h>
#ifdef __cplusplus
}
#endif

#include "audio.h"
#include "video.h"

const int kMaxPacketSize = 15*1024*1024;    // 音视频加起来最大缓存为15M


#undef main         // 避免和SDL2的main重定义
int main(int argc, char *argv[])
{
    LogInit();

    char fileName[128] = {"source.200kbps.768x320.flv"};
    //char fileName[256] = {"G:\\迅雷下载\\绣春刀II：修罗战场.HD.720p.国语中字.mkv"};
    // char fileName[256] = {"D:\\迅雷下载\\[阳光电影www.ygdy8.com]行尸之惧第二季第01集[中英双字].rmvb"};
    // char fileName[256] = {"E:\\stream\\si19_21c_2k_64qam_cr23_gi32_7.50.ts"};  // 音频解码错误
    //  char fileName[256] = {"E:\\OS\\cctv2-4.ts"};
    // char fileName[256] = {"E:\\stream\\Dual_Strm_1PID_321_210_AsscRouting_ddp_DVB_h264_25fps.ts"};  // 音频解码错误
    // char fileName[256] = {"E:\\stream\\再见陌生人.mp4"};
    AVFormatContext *pFormatCtx = NULL;
    int		i;
    int		audioStream;
    int		videoStream;

    // 视频解码
    AVCodecContext	*pVideoCodecCtx = NULL;         // 视频解码器上下文
    AVCodec		*pVideoCodec	= NULL;         	// 视频解码器

    // 音频解码
    AVCodecContext	*pAudioCodecCtx		= NULL; 	// 视频解码器上下文
    AVCodec		*pAudioCodec		= NULL; 		// 视频解码器


    AVPacket	packet;
    AVDictionary *optionsDict	= NULL;

    // 初始化SDL模块
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) )
    {
        LogError( "Could not initialize SDL - %s\n", SDL_GetError() );
        exit( 1 );
    }

    // 注册复用器,编码器等
    av_register_all();


    // 打开多媒体文件
    if ( avformat_open_input( &pFormatCtx, fileName, NULL, NULL ) != 0 )
    {
        LogError( "avformat_open_input failed\n" );
        return(-1);
    }
    // 获取多媒体流信息
    if ( avformat_find_stream_info( pFormatCtx, NULL ) < 0 )
    {
        LogError( "avformat_find_stream_info failed\n" );
        return(-1);
    }

    // 打印有关输入或输出格式的详细信息, 该函数主要用于debug
    av_dump_format( pFormatCtx, 0, fileName, 0 );

    // 查找音频/视频流
    audioStream	= -1;
    videoStream	= -1;
    for ( i = 0; i < pFormatCtx->nb_streams; i++ )
    {
        if ( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO )
        {
            audioStream = i;
        }
        else if ( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO )
        {
            videoStream = i;
        }
    }

    if ( audioStream == -1 )
    {
        LogError( "Can't find the audio stream\n" );
        return(-1);
    }
    if ( videoStream == -1 )
    {
        LogError( "Can't find the video stream\n" );
        return(-1);
    }

    // ----------------------处理音频 ----------------------------------
    // 获取音频解码器上下文
    pAudioCodecCtx = pFormatCtx->streams[audioStream]->codec;

    // 通过解码器ID创建一个解码器
    pAudioCodec = avcodec_find_decoder( pAudioCodecCtx->codec_id );
    if ( pAudioCodec == NULL )
    {
        LogError( "Unsupported audio codec!\n" );
        return(-1);
    }
    // 使用 AVCodec初始化AVCodecContext
    if ( avcodec_open2( pAudioCodecCtx, pAudioCodec, NULL ) < 0 )
    {
        LogError( "avcodec_open2 audio  failed!\n" );
        return(-1);
    }

    LogInfo("avcodec_open2 audio into");
    // ----------------------处理视频 ----------------------------------
    // 获取视频解码器上下文
    pVideoCodecCtx = pFormatCtx->streams[videoStream]->codec;

    // 通过解码器ID创建一个解码器
    pVideoCodec = avcodec_find_decoder( pVideoCodecCtx->codec_id );
    if ( pVideoCodec == NULL )
    {
        LogError( "Unsupported video codec!\n" );
        return(-1);
    }

    int refcount = 1;
    // 使用 AVCodec初始化AVCodecContext
    /* Init the decoders, with or without reference counting */
    av_dict_set(&optionsDict, "refcounted_frames", refcount ? "1" : "0", 0);
    if ( avcodec_open2( pVideoCodecCtx, pVideoCodec, &optionsDict ) < 0 )
    {
        LogError( "avcodec_open2 video  failed!\n" );
        return(-1);
    }
    LogInfo("avcodec_open2 video into");

    LogInfo("av_frame_alloc into");

    //初始化audio 输出
    AudioInit(pAudioCodecCtx);       // 初始化音频相关的设置
    VideoInit(pVideoCodecCtx->width, pVideoCodecCtx->height, pVideoCodecCtx->pix_fmt);
    AudioSetTimeBase(av_q2d(pFormatCtx->streams[audioStream]->time_base));
    VideoSetTimeBase(av_q2d(pFormatCtx->streams[videoStream]->time_base));

    std::thread audioThread(AudioDecodeThread, pAudioCodecCtx);
    std::thread videoThread(VideoDecodeThread, pVideoCodecCtx);


    LogInfo("av_read_frame into");
    int eof = 0;
    int audio = 0;
    int video = 0;
    // 一定要等线程进入才去做读取的动作
    while (eof != 1)
    {
        if(( (AudioPacketSize() + VideoPacketSize() >=  kMaxPacketSize)))
        {
            // 休眠
            //LogDebug("buffer = %d", AudioPacketSize() + VideoPacketSize());
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            continue;
        }
        if(av_read_frame( pFormatCtx, &packet ) >= 0)
        {

            if ( packet.stream_index == videoStream )           // 视频包
            {
                if(video == 0)
                {
                    video = 1;
                    LogDebug("video pts = %lld", av_q2d(pFormatCtx->streams[videoStream]->time_base) * packet.pts);
                }
                VideoPacketPush(&packet);
            }
            else if ( packet.stream_index == audioStream )      // 音频包
            {
                if(audio == 0)
                {
                    audio = 1;
                    LogDebug("audio pts = %lld", av_q2d(pFormatCtx->streams[audioStream]->time_base) * packet.pts);
                }
                AudioPacketPush( &packet);
            }
            else
            {
                av_packet_unref( &packet );
            }
        }
        else
        {
            eof = 1;
        }
    }
    LogDebug("read frame finish");
    // 插入空包，音频、视频解码线程在检测到数据为0的数据包时则在播放完缓存后退出线程
    packet.size = 0;
    AudioPacketPush(&packet);
    packet.size = 0;
    VideoPacketPush(&packet);

    // 一定要回收资源
    while(true)
    {
        if(audioThread.joinable())
        {
            audioThread.join();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    LogDebug("audioThread finish");
    while(true)
    {
        if(videoThread.joinable())
        {
            videoThread.join();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    LogDebug("videoThread finish");

    // 释放资源
    AudioClose();           // 清理和关闭音频相关的资源
    VideoClose();


    /* Close the codec */
    avcodec_close( pVideoCodecCtx );
    LogDebug("avcodec_close finish");
    /* Close the video file */
    avformat_close_input( &pFormatCtx );
    LogDebug("avformat_close_input finish");
    SDL_Quit();
    LogDebug("SDL_Quit finish");

    return(0);
}

