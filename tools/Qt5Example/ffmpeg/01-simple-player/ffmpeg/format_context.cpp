#include "format_context.h"
#include "ffmpeg_common.h"
#include <QDebug>
class AVPacketRAII
{
public:
    inline AVPacketRAII(AVPacket *packet) :
        packet(packet)
    {}
    inline ~AVPacketRAII()
    {
        av_packet_unref(packet);
    }

private:
    AVPacket *packet;
};

static inline AVCodecParameters *codecParams(AVStream *stream)
{
    return stream->codecpar;
}
static inline const char *getPixelFormat(AVStream *stream)
{
    return av_get_pix_fmt_name((AVPixelFormat)stream->codecpar->format);
}
static inline const char *getSampleFormat(AVStream *stream)
{
    return av_get_sample_fmt_name((AVSampleFormat)stream->codecpar->format);
}

static bool streamNotValid(AVStream *stream)
{
    return
    (
        (stream->disposition & AV_DISPOSITION_ATTACHED_PIC)    ||
        (codecParams(stream)->codec_type == AVMEDIA_TYPE_DATA) ||
        (codecParams(stream)->codec_type == AVMEDIA_TYPE_ATTACHMENT && codecParams(stream)->codec_id != AV_CODEC_ID_TTF && codecParams(stream)->codec_id != AV_CODEC_ID_OTF)
    );
}

static std::string getTag(AVDictionary *metadata, const char *key, const bool deduplicate = true)
{
    AVDictionaryEntry *avTag = av_dict_get(metadata, key, nullptr, AV_DICT_IGNORE_SUFFIX);
    if (avTag && avTag->value)
    {
        const QByteArray tag = QByteArray(avTag->value);
        if (deduplicate)
        {
            // Workaround for duplicated tags separated by ';'.
            // Check only when both tag has the same length and use only letters and numbers for
            // comparision (sometimes it differs in apostrophe or different/incorrect encoding).
            // Return the second tag (mostly better).
            const QList<QByteArray> tags = tag.split(';');
            if (tags.count() == 2)
            {
                const QByteArray first  = tags[0].trimmed();
                const QByteArray second = tags[1].trimmed();
                if (first.length() == second.length())
                {
                    bool ok = true;
                    for (int i = 0; i < second.length(); ++i)
                    {
                        const char c1 = first[i];
                        const char c2 = second[i];
                        if
                        (
                            (c2 >= '0' && c2 <= '9' && c1 != c2) ||
                            (
                                ((c2 >= 'a' && c2 <= 'z') || (c2 >= 'A' && c2 <= 'Z')) &&
                                ((c1 | 0x20) != (c2 | 0x20))
                            )
                        )
                        {
                            ok = false;
                            break;
                        }
                    }
                    if (ok)
                        return std::string(second.data());
                }
            }
        }
        return std::string(tag.trimmed().data());
    }
    return std::string();
}

StreamInfo *FormatContext::getStreamInfo(AVStream *stream) const
{
    // 判断stream是否有效
    if (streamNotValid(stream))
        return nullptr;

    StreamInfo *streamInfo = new StreamInfo;

    // 根据codec_id查找 codec name
    if (const AVCodec *codec = avcodec_find_decoder(codecParams(stream)->codec_id))
        streamInfo->codec_name = codec->name;

    streamInfo->must_decode = true;
    // 二次查找decoder
    if (const AVCodecDescriptor *codecDescr = avcodec_descriptor_get(codecParams(stream)->codec_id))
    {
        if (codecDescr->props & AV_CODEC_PROP_TEXT_SUB)
            streamInfo->must_decode = false;

        if (streamInfo->codec_name.empty())
            streamInfo->codec_name = codecDescr->name;
    }

    streamInfo->bitrate = codecParams(stream)->bit_rate;
    streamInfo->bpcs = codecParams(stream)->bits_per_coded_sample;
    streamInfo->codec_tag = codecParams(stream)->codec_tag;
    streamInfo->is_default = stream->disposition & AV_DISPOSITION_DEFAULT;
    streamInfo->time_base.num = stream->time_base.num;
    streamInfo->time_base.den = stream->time_base.den;
    streamInfo->type = (PlayerMediaType)codecParams(stream)->codec_type; //Enumy są takie same

    if (codecParams(stream)->extradata_size)
    {
        streamInfo->data.reserve(codecParams(stream)->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
        streamInfo->data.resize(codecParams(stream)->extradata_size);
        memcpy(streamInfo->data.data(), codecParams(stream)->extradata, streamInfo->data.capacity());
       // streamInfo->data.assign(streamInfo->data.capacity(),  codecParams(stream)->extradata);
        av_free(codecParams(stream)->extradata);
        codecParams(stream)->extradata = (quint8 *)streamInfo->data.data();
    }



    switch (streamInfo->type)
    {
        case PLAYER_TYPE_AUDIO:
            streamInfo->format = getSampleFormat(stream);
            streamInfo->channels = codecParams(stream)->channels;
            streamInfo->sample_rate = codecParams(stream)->sample_rate;
            streamInfo->block_align = codecParams(stream)->block_align;
            break;
        case PLAYER_TYPE_VIDEO:
            streamInfo->format = getPixelFormat(stream);
            if (stream->sample_aspect_ratio.num)
                streamInfo->sample_aspect_ratio = av_q2d(stream->sample_aspect_ratio);
            else if (codecParams(stream)->sample_aspect_ratio.num)
                streamInfo->sample_aspect_ratio = av_q2d(codecParams(stream)->sample_aspect_ratio);
            streamInfo->W = codecParams(stream)->width;
            streamInfo->H = codecParams(stream)->height;
            if (!stillImage)
                streamInfo->FPS = av_q2d(stream->r_frame_rate);
            break;
        case AVMEDIA_TYPE_ATTACHMENT:
            streamInfo->title = getTag(stream->metadata, "filename", false);
            switch (codecParams(stream)->codec_id)
            {
                case AV_CODEC_ID_TTF:
                    streamInfo->codec_name = "TTF";
                    break;
                case AV_CODEC_ID_OTF:
                    streamInfo->codec_name = "OTF";
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return streamInfo;
}

FormatContext::FormatContext(StreamsInfoPtr &streams_info, bool reconnectStreamed):
    isError(false),
    currPos(0.0),
    reconnectStreamed(reconnectStreamed),
    maybeHasFrame(true),
    streams_info_(streams_info),
    formatCtx(nullptr),
    stillImage(false),
    isLocal(false),      // 是否为本地文件
    isStreamed(false)
{

}

FormatContext::~FormatContext()
{
    if (formatCtx)
    {
        for (AVStream *stream : streams)
        {
            if (codecParams(stream))
            {
                //Data is allocated in QByteArray, so FFmpeg mustn't free it!
                codecParams(stream)->extradata = nullptr;
                codecParams(stream)->extradata_size = 0;
            }
        }
        qDebug() << "~FormatContext() avformat_close_input";
        avformat_close_input(&formatCtx);
        avformat_free_context(formatCtx);
    }
    qDebug() << "~FormatContext()";
    index_map.clear();
    streamsTS.clear();
    streamsOffset.clear();
    nextDts.clear();
}

int FormatContext::bitrate() const
{
    return formatCtx->bit_rate / 1000;
}

/**
 * @brief FormatContext::length
 * @return 播放总时长
**/
double FormatContext::length() const
{
    if (!isStreamed && !stillImage && formatCtx->duration != PLAYER_NOPTS_VALUE)
    {
        qDebug() << "FormatContext::length()  = " << formatCtx->duration / (double)AV_TIME_BASE;
        return formatCtx->duration / (double)AV_TIME_BASE;
    }
    else
    {
        qDebug() << "FormatContext::length() isStreamed" << isStreamed;
        qDebug() << "FormatContext::length() stillImage" << stillImage;
    }
    return -1.0;
}

bool FormatContext::seek(double pos, bool backward)
{
    bool isOk = false;
    qDebug() << "FormatContext::isStreamed = " << isStreamed;
    qDebug() << "FormatContext::backward = " << backward;
    qDebug() << "FormatContext::seek seekTo= " << pos;
    if (!isStreamed)
    {
        const double len = length();
        if (pos < 0.0)
            pos = 0.0;
        else if (len > 0.0 && pos > len)
            pos = len;

        const double posToSeek = pos + startTime;
        const qint64 timestamp = ((streams_info_->size() == 1) ? posToSeek : (backward ? floor(posToSeek) : ceil(posToSeek))) * AV_TIME_BASE;
        qDebug() << "FormatContext::seek seekTo = " << timestamp;
        qDebug() << "FormatContext::backward = " << (backward ? AVSEEK_FLAG_BACKWARD : 0);

        //isOk = av_seek_frame(formatCtx, -1, timestamp, backward ? AVSEEK_FLAG_BACKWARD : 0) >= 0;
        isOk = av_seek_frame(formatCtx, -1, timestamp, AVSEEK_FLAG_BACKWARD) >= 0;
        if (isOk)
        {
//            for (int i = 0; i < streamsTS.count(); ++i)
//                streamsTS[i] = pos;
//            currPos = pos;
//            nextDts.fill(pos);
//            isError = false;
        }
    }
    return isOk;
}

bool FormatContext::read(PacketBasePtr &encoded, int &idx)
{
   // qDebug() << "FormatContext::read into";
    AVPacket *packet = (AVPacket *)(encoded.get()->getRealPacket());

    int ret;

    ret = av_read_frame(formatCtx, packet);
    if (ret == AVERROR_INVALIDDATA)
    {
        if (invalErrCount < 1000)
        {
            ++invalErrCount;
            return true;
        }
        isError = true;
        return false;
    }
    else
        invalErrCount = 0;
    if (ret == AVERROR(EAGAIN))
        return true;
    else if (ret)
    {
        isError = true;
        return false;
    }
#if 0
    qDebug() << "FormatContext stream_index = " << packet->stream_index;
    qDebug() << "FormatContext pts = " << packet->pts;
    qDebug() << "FormatContext duration = " << packet->duration;
#endif
    const int ff_idx = packet->stream_index;
    if (ff_idx >= streams.size())
    {
        qDebug() << "Stream index out of range: " << QString::number(ff_idx);
        return true;
    }
    const double time_base = av_q2d(streams.at(ff_idx)->time_base);

    {
        encoded->ts.setInvalid();
        if (packet->dts != AV_NOPTS_VALUE)
            encoded->ts.setDts(packet->dts * time_base, startTime);
        if (packet->pts != AV_NOPTS_VALUE)
            encoded->ts.setPts(packet->pts * time_base, startTime);
    }

    if (packet->duration > 0)
        encoded->setDuration((double) packet->duration * time_base);
    else //if ((encoded->getDuration() = encoded->ts - streamsTS.at(ff_idx)) < 0.0 )
        encoded->setDuration(0.0);
    streamsTS[ff_idx] = encoded->ts;

    if (isStreamed)
    {

       // encoded->ts = lastTime;
        //lastTime += encoded.duration;

    }

    encoded->setSize(packet->size);
    //currPos = encoded.ts;

    encoded->hasKeyFrame_ = packet->flags & AV_PKT_FLAG_KEY;
    if (streams.at(ff_idx)->sample_aspect_ratio.num)
        encoded->sampleAspectRatio_ = av_q2d(streams.at(ff_idx)->sample_aspect_ratio);

    // Generate DTS for key frames if DTS doesn't exist (workaround for some M3U8 seekable streams)
    if (encoded->hasKeyFrame_ && !encoded->ts.hasDts())
        encoded->ts.setDts(nextDts.at(ff_idx));
    nextDts[ff_idx] = encoded->ts + encoded->getDuration();

    idx = index_map.at(ff_idx);

    return true;
}

bool FormatContext::open(const std::string &_url)
{
    qDebug() << "FormatContext::open into ";
    int ret;
    // 打开demuxer
    qDebug( "avformat_alloc_context %s",   _url.c_str());
    //formatCtx = avformat_alloc_context();
    // 打开多媒体文件
    qDebug( "avformat_open_input %s",   _url.c_str());
    //formatCtx = NULL;
    if ( (ret = avformat_open_input(&formatCtx, _url.c_str(), NULL, NULL)) !=0)
    //if ( (ret = avformat_open_input(&formatCtx, "E:\\码流\\heart_jiazao.ts", NULL, NULL)) !=0)
    {
        qDebug() << "avformat_open_input open failed, ret = " + ret;
        return false;
    }
    // 获取多媒体流信息
    qDebug() << "avformat_find_stream_info, ret = " + ret;
    if ((ret = avformat_find_stream_info(formatCtx, nullptr)) < 0)
    {
        qDebug() << "avformat_find_stream_info failed, ret = " + ret;
        return false;
    }

    // 打印有关输入或输出格式的详细信息, 该函数主要用于debug
    qDebug() << "av_dump_format, ret = " + ret;
    av_dump_format( formatCtx, 0, _url.c_str(), 0 );

    // 设置起始时间
    if ((startTime = formatCtx->start_time / (double)AV_TIME_BASE) < 0.0)
        startTime = 0.0;

    // 根据stream成分重新初始化参数
    qDebug() << "formatCtx->nb_streams = " << formatCtx->nb_streams;
    index_map.resize(formatCtx->nb_streams);
    streamsTS.resize(formatCtx->nb_streams);
    streamsOffset.resize(formatCtx->nb_streams);
    nextDts.resize(formatCtx->nb_streams);

    for (unsigned i = 0; i < formatCtx->nb_streams; ++i)
    {
        StreamInfo *streamInfo = getStreamInfo(formatCtx->streams[i]);
        if (!streamInfo)
            index_map[i] = -1;
        else
        {
            qDebug() << "name = " << streamInfo->codec_name.c_str() << ", size = " << streams_info_->size();
            index_map[i] = streams_info_->size();
            streams_info_->push(streamInfo);
        }
        if (!fixMkvAss && codecParams(formatCtx->streams[i])->codec_id == AV_CODEC_ID_ASS && !strncasecmp(formatCtx->iformat->name, "matroska", 8))
            fixMkvAss = true;
        formatCtx->streams[i]->event_flags = 0;
        streams.push_back(formatCtx->streams[i]);

        streamsTS[i] = 0.0;
    }

    isStreamed = !isLocal && formatCtx->duration <= 0;

    qDebug() << "FormatContext::open ok, duration = " << formatCtx->duration;   // 单位为微妙
    return true;
}
