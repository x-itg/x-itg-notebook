#include <iostream>       // std::cout
#include <mutex>          // std::mutex
#include <stdarg.h>
#include <string>
#include <chrono>
#include <cinttypes>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <thread>

#include "video.h"
#include "av_clock.h"
#include "avpackets_queue.h"
#include "log.h"

typedef struct video_param
{
    /* 视频输出 */
    SDL_Texture		*pFrameTexture	= NULL;
    SDL_Window		*pWindow = NULL;
    SDL_Renderer 	*pRenderer = NULL;
    PacketQueue videoQueue;
    SDL_Rect	rect;
    struct SwsContext	*pSwsCtx = NULL;
    AVFrame* pFrameYUV = NULL;
    enum AVPixelFormat sPixFmt = AV_PIX_FMT_YUV420P;
    int quit = 0;
    int getFirstFrame = 0;
    double videoBaseTime  = 0.0;
    int packetEof = 0;
}T_VideoParam;

static T_VideoParam sVideoParam;

static int64_t getNowTime()
{
    auto time_now = std::chrono::system_clock::now();
    auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
    return duration_in_ms.count();
}

int VideoInit(int width, int height, enum AVPixelFormat pix_fmt)
{
    memset(&sVideoParam, 0 ,sizeof(T_VideoParam));
    PacketQueueInit(&sVideoParam.videoQueue);

    return 0;
}

int _VideoSDL2Init(int width, int height, enum AVPixelFormat pix_fmt)
{
    sVideoParam.pWindow = SDL_CreateWindow( "Video Window",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            width, height,
                                            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
    if ( !sVideoParam.pWindow )
    {
        LogError( "SDL: could not set video mode - exiting\n" );
        return -1;
    }

    SDL_RendererInfo	info;
    sVideoParam.pRenderer = SDL_CreateRenderer( sVideoParam.pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( !sVideoParam.pRenderer )
    {
        av_log( NULL, AV_LOG_WARNING, "Failed to initialize a hardware accelerated renderer: %s\n", SDL_GetError() );
        sVideoParam.pRenderer = SDL_CreateRenderer( sVideoParam.pWindow, -1, 0 );
    }
    if ( sVideoParam.pRenderer )
    {
        if ( !SDL_GetRendererInfo( sVideoParam.pRenderer, &info ) )
            av_log( NULL, AV_LOG_VERBOSE, "Initialized %s renderer.\n", info.name );
    }

    sVideoParam.pFrameTexture = SDL_CreateTexture( sVideoParam.pRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(!sVideoParam.pFrameTexture)
    {
        LogError( "SDL: SDL_CreateTexture failed\n" );
        return -1;
    }
    LogInfo("pix_fmt = %d, %d\n", pix_fmt, AV_PIX_FMT_YUV420P);

    // ffmepg decode output format为YUV420
    sVideoParam.pSwsCtx = sws_getContext
            (
                width, height, pix_fmt,
                width, height, AV_PIX_FMT_YUV420P,
                SWS_BICUBIC,
                NULL,
                NULL,
                NULL
                );
    if(!sVideoParam.pSwsCtx)
    {
        LogError( "SDL: sws_getContext failed\n" );
        return -1;
    }
    sVideoParam.rect.x	= 0;
    sVideoParam.rect.y	= 0;
    sVideoParam.rect.w	= width;
    sVideoParam.rect.h	= height;

    // 分配用于保存转换后的视频帧
    sVideoParam.pFrameYUV = av_frame_alloc();
    if (!sVideoParam.pFrameYUV)
    {
        LogError( "av_frame_alloc sVideoParam.pFrameYUV  failed!\n" );
        return -1;
    }

    int numBytes = avpicture_get_size( AV_PIX_FMT_YUV420P, width,  height );
    uint8_t* buffer = (uint8_t *) av_malloc( numBytes * sizeof(uint8_t) );

    avpicture_fill( (AVPicture *) sVideoParam.pFrameYUV, buffer, AV_PIX_FMT_YUV420P, width, height );

    return 0;
}
int VideoDecodeThread( void *userdata )
{
    FunEntry();
    int64_t pts;

    AVCodecContext	*pVideoCodecCtx = (AVCodecContext *) userdata;

    AVPacket	packet;
    int			frameFinished; // 是否获取帧

    // SDL2的初始化和显示必须在同一线程进行
    _VideoSDL2Init(pVideoCodecCtx->width, pVideoCodecCtx->height, pVideoCodecCtx->pix_fmt);
    // 控制事件
    SDL_Event event;
    int64_t th = 0;
    // 分配视频帧
    AVFrame		*pVideoFrame	= NULL;
    pVideoFrame = av_frame_alloc();
    if(!pVideoFrame)
    {
        LogError( "av_frame_alloc pVideoFrame  failed!\n" );
        sVideoParam.quit = 1;
    }
    static int64_t sPrePts = 0;
    sVideoParam.getFirstFrame = 0;
    sVideoParam.packetEof = 0;
    int first = 0;
    int64_t curTime3 = 0;
    while (sVideoParam.quit != 1)
    {
        if(sVideoParam.packetEof && sVideoParam.videoQueue.size == 0)
            sVideoParam.quit = 1;

        int ret = PacketQueueTake( &sVideoParam.videoQueue, &packet, 0);

        if(ret == 0)
        {
            sVideoParam.packetEof = packet.size ? 0 : 1;
            // 解视频帧
            avcodec_decode_video2( pVideoCodecCtx, pVideoFrame, &frameFinished, &packet );
            // 确定已经获取到视频帧
            if ( frameFinished )
            {
                pts =   sVideoParam.videoBaseTime * pVideoFrame->pts * 1000;        // 转为毫秒计算
                // LogInfo("video1 pts = %lld", pVideoFrame->pts);
                th = pVideoFrame->pkt_duration * sVideoParam.videoBaseTime * 1000;
                if(pts < 0)
                {
                    pts = th + sPrePts;
                }
                // LogInfo("video2 pts = %lld, th = %lld", pVideoFrame->pts, th);
                while(!sVideoParam.quit)
                {
                    int64_t diff = (pts - AVClockGetCurTime());
                    if(0==first)
                    {
                        LogInfo("\n    video pts = %lld, cur = %lld, diff = %lld", pts, AVClockGetCurTime(), diff);
                        LogInfo("video base = %lf", sVideoParam.videoBaseTime);
                    }
                    if(th = 0)
                    {
                        if(sPrePts != 0)
                        {
                            pts - sPrePts;
                        }
                        else
                            th = 30;
                    }
                    if(diff > th  && diff < 10000)      // 阈值为30毫秒, 超过一定的阈值则直接播放
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(2));
                    }
                    else
                    {
                        break;
                    }
                    if(!AVClockIsEnable())
                    {
                        if(sVideoParam.getFirstFrame)
                            std::this_thread::sleep_for(std::chrono::milliseconds(2));
                        else
                        {
                            break;
                        }
                    }
                }
                sVideoParam.getFirstFrame  = 1;
                int64_t curTime = getNowTime();
                static int64_t sPreTime = curTime;

                if(0  == first)
                    LogInfo("cur = %lld, dif pts = %lld, time = %lld ", curTime, pts - sPrePts, curTime - sPreTime);
                sPreTime = curTime;
                sPrePts = pts;

                curTime3 = getNowTime();
                VideoDisplay(pVideoFrame, pVideoCodecCtx->height);
                //LogInfo("VideoDisplay diff = %lld", getNowTime() - curTime3);
                first = 1;
                if(pVideoCodecCtx->refcounted_frames)
                {
                    av_frame_unref( pVideoFrame);
                }
            }
            // 释放packet占用的内存
            av_packet_unref( &packet );
        }
        else
        {
            //LogInfo("sleep_for wait data , time = %lld", getNowTime());
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        //LogInfo("SDL_PollEvent, time = %lld", getNowTime() - curTime3);
        SDL_PollEvent( &event );
        switch ( event.type )
        {
        case SDL_QUIT:
            SDL_Quit();
            exit( 0 );
            break;
        default:
            break;
        }
    }

    /* Free the YUV frame */
    av_free( pVideoFrame );


    FunExit();
}

void VideoDisplay(AVFrame *pVideoFrame, int height)
{
    static int64_t sPreTime = 0;
    static int64_t sPrePts = 0;
    int64_t curTime = getNowTime();
    AVFrame *pDisplayFrame;

    if(!pVideoFrame)
    {
        LogError( "pVideoFrame is null\n");
        return;
    }
    sPreTime = curTime;
    sPrePts = pVideoFrame->pts;
    if(sVideoParam.pFrameYUV)
    {
        sws_scale			// 经过scale，pts信息被清除？
                (
                    sVideoParam.pSwsCtx,
                    (uint8_t const * const *) pVideoFrame->data,
                    pVideoFrame->linesize,
                    0,
                    height,
                    sVideoParam.pFrameYUV->data,
                    sVideoParam.pFrameYUV->linesize
                    );
        pDisplayFrame = sVideoParam.pFrameYUV;
    }
    else
    {
        pDisplayFrame = pVideoFrame;
    }


    /*
     * 视频帧直接显示
     * //iPitch 计算yuv一行数据占的字节数
     */
    SDL_UpdateTexture( sVideoParam.pFrameTexture, &sVideoParam.rect, pDisplayFrame->data[0], pDisplayFrame->linesize[0] );
    SDL_RenderClear( sVideoParam.pRenderer );
    SDL_RenderCopy( sVideoParam.pRenderer, sVideoParam.pFrameTexture, &sVideoParam.rect, &sVideoParam.rect );
    SDL_RenderPresent( sVideoParam.pRenderer );
}

int VideoPacketPush(AVPacket *packet)
{
    PacketQueuePut( &sVideoParam.videoQueue, packet);
}

int VideoPacketSize()
{
    return PacketQueueGetSize(&sVideoParam.videoQueue);
}

void VideoClose()
{
    FunEntry();
    if(sVideoParam.pSwsCtx)
        sws_freeContext(sVideoParam.pSwsCtx);
    if(sVideoParam.pFrameTexture)
        SDL_DestroyTexture( sVideoParam.pFrameTexture );
    if(sVideoParam.pRenderer)
        SDL_DestroyRenderer(sVideoParam.pRenderer);
    if(sVideoParam.pWindow)
        SDL_DestroyWindow(sVideoParam.pWindow);
    if(sVideoParam.pFrameYUV)
        av_free( sVideoParam.pFrameYUV );
    FunExit();
}

int VideoGetFirstFrame()
{
    return sVideoParam.getFirstFrame;
}

void VideoSetTimeBase(double timeBase)
{
    sVideoParam.videoBaseTime = timeBase;
}


