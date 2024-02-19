#ifndef FORMAT_CONTEXT_H
#define FORMAT_CONTEXT_H
#include <vector>
#include "time_stamp.h"
#include "stream_info.h"
#include "packetbase.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/replaygain.h>
#include <libavutil/pixdesc.h>
}

/**
 * @brief The FormatContext class
 *  和FFmpegDemuxer之间做一个桥接，主要这里都用FFmpeg的API，FFmpegDemuxer则重新封装
 */
class FormatContext
{
public:
    FormatContext(StreamsInfoPtr &streams_info, bool reconnectStreamed = false);
    ~FormatContext();
    int bitrate() const;
    double length() const;
    bool seek(double pos, bool backward);
    bool read(PacketBasePtr &encoded, int &idx);
    bool open(const std::string &_url);
    StreamInfo *getStreamInfo(AVStream *stream) const;

    bool isLocal, isStreamed, isError;
    StreamsInfoPtr &streams_info_;

    double currPos;

    std::vector<int> index_map;
    std::vector<AVStream *> streams;
    std::vector<TimeStamp> streamsTS;
    std::vector<double> streamsOffset;
    std::vector<double> nextDts;
    AVFormatContext *formatCtx;


    bool reconnectStreamed;
    bool isPaused, fixMkvAss;
    mutable bool isMetadataChanged; //被mutable修饰的变量，将永远处于可变的状态，即使在一个const函数中。
    double lastTime, startTime;

    int invalErrCount, errFromSeek;
    bool maybeHasFrame;
    bool stillImage ;
};

typedef std::shared_ptr<FormatContext> FormatContextPtr;


#endif // FORMAT_CONTEXT_H
