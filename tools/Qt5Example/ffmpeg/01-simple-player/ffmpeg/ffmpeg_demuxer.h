#ifndef FFMPEG_DEMUXER_H
#define FFMPEG_DEMUXER_H

#include "demuxer.h"
#include "format_context.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/replaygain.h>
#include <libavutil/pixdesc.h>
}

class FfmpegDemuxer : public Demuxer
{
public:
    FfmpegDemuxer();
    ~FfmpegDemuxer();
    double length() const override;
    bool seek(double pos, bool backward) override;
    bool read(PacketBasePtr &, int &) override;      // 所有的传递都使用智能指针去处理

    bool open(const std::string &url) override;
private:
    FormatContextPtr formatContext_;
};

#endif // FFMPEG_DEMUXER_H
