#include "ffmpeg_demuxer.h"

#include <QDebug>
FfmpegDemuxer::FfmpegDemuxer()
    : Demuxer()
{
    formatContext_ = std::make_shared<FormatContext>(streams_info_, true);
}

double FfmpegDemuxer::length() const
{
    double length = -1.0;

    length = qMax(length, formatContext_->length());

    return length;
}

bool FfmpegDemuxer::seek(double pos, bool backward)
{
    bool seeked = false;
    qDebug() << "FfmpegDemuxer::seek seekTo= " << pos;
    if (formatContext_->seek(pos, backward))
        seeked |= true;
    else if (formatContext_->isStreamed  )
    {
        //formatContext_->setStreamOffset(pos);
        seeked |= true;
    }

    return seeked;
}

bool FfmpegDemuxer::read(PacketBasePtr &encoded, int &idx)     // 所有的传递都使用智能指针去处理
{

    return formatContext_->read(encoded, idx);
}

FfmpegDemuxer::~FfmpegDemuxer()
{
    formatContext_.reset();     //
    streams_info_.reset();
    qDebug("~FfmpegDemuxer");
}

bool FfmpegDemuxer::open(const std::string &url)
{
    if( formatContext_->open(url))
    {
        qDebug("FfmpegDemuxer::open ok, size = %d,",streams_info_->size());
        qDebug("streams_info_ size = %d, code %s, %s",
               streams_info_->size(),
               streams_info_->at(0)->codec_name.c_str(),
               streams_info_->at(1)->codec_name.c_str());
    }
    return true;
}
