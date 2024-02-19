#include "ffmpeg_common.h"

extern "C"
{
    #include <libavformat/version.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/dict.h>
}

FfmpegCommon::FfmpegCommon()
{

}


AVPacket *FfmpegCommon::createAVPacket()
{
    AVPacket *packet;
#if LIBAVCODEC_VERSION_MAJOR >= 57
    packet = av_packet_alloc();
#else
    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
#endif
    return packet;
}
void FfmpegCommon::freeAVPacket(AVPacket *&packet)
{
#if LIBAVCODEC_VERSION_MAJOR >= 57
    av_packet_free(&packet);
#else
    if (packet)
        av_packet_unref(packet);
    av_freep(&packet);
#endif
}
