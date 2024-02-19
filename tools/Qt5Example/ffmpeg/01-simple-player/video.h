#ifndef VIDEO_H
#define VIDEO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#include <sys/time.h>
#include <SDL2/include/SDL.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif
int VideoInit(int width, int height, enum AVPixelFormat pix_fmt);
void VideoDisplay(AVFrame *pVideoFrame, int height);
int VideoPacketPush(AVPacket *packet);
int VideoPacketSize();
void VideoClose();
int VideoDecodeThread( void *userdata );
int VideoGetFirstFrame();
void VideoSetTimeBase(double timeBase);
#endif // VIDEO_H
