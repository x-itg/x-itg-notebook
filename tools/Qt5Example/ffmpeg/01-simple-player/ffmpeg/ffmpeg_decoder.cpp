#include "ffmpeg_decoder.h"
#include <algorithm>
#include <QDebug>
FfmpegDecoder::FfmpegDecoder():
    threads(0), lowres(0),
    thread_type_slice(false),
    lastFrameW(-1), lastFrameH(-1),
    sws_ctx(nullptr),
    desiredPixFmt(-1),
    Decoder()
{

}

FfmpegDecoder::~FfmpegDecoder()
{
    if(codec_ctx)
    {
        avcodec_close(codec_ctx);
        av_free(codec_ctx);
    }
    if(frame)
    {
        //av_frame_free(&frame);
    }
    qDebug() << "~FfmpegDecoder()";
}

/**
 * @brief 根据码流信息查找解码器
 * @param streamInfo
 * @return 返回解码器
 */
AVCodec *FfmpegDecoder::init(StreamInfo &streamInfo)
{
    // 根据解码器名字查找解码器
    AVCodec *codec = avcodec_find_decoder_by_name(streamInfo.codec_name.c_str());
    if (codec)
    {
        codec_ctx = avcodec_alloc_context3(codec);

        codec_ctx->codec_id = codec->id;
  #if 1
        codec_ctx->codec_tag = streamInfo.codec_tag;
        codec_ctx->bit_rate = streamInfo.bitrate;
        codec_ctx->channels = streamInfo.channels;
        codec_ctx->sample_rate = streamInfo.sample_rate;
        codec_ctx->block_align = streamInfo.block_align;
        codec_ctx->bits_per_coded_sample = streamInfo.bpcs;
        codec_ctx->pix_fmt = av_get_pix_fmt(streamInfo.format.c_str());
        codec_ctx->coded_width = codec_ctx->width = streamInfo.W;
        codec_ctx->coded_height = codec_ctx->height = streamInfo.H;
//		codec_ctx->debug_mv = FF_DEBUG_VIS_MV_P_FOR | FF_DEBUG_VIS_MV_B_FOR | FF_DEBUG_VIS_MV_B_BACK;
        if (!streamInfo.data.empty())
        {
            codec_ctx->extradata = (uint8_t *)streamInfo.data.data();
            codec_ctx->extradata_size = streamInfo.data.size();
        }
#endif
    }
    qDebug() << "FfmpegDecoder::init codec = " << codec;
    return codec;
}

/**
 * @brief 打开解码器
 * @param codec
 * @return
 */
bool FfmpegDecoder::openCodec(AVCodec *codec)
{
    if (avcodec_open2(codec_ctx, codec, nullptr))
    {
        return false;
    }
    switch (codec_ctx->codec_type)
    {
        case AVMEDIA_TYPE_VIDEO:
        case AVMEDIA_TYPE_AUDIO:
            //frame = av_frame_alloc();
            break;
        default:
            break;
    }
    return true;
}

void FfmpegDecoder::decodeFirstStep(const PacketBasePtr &encodedPacket, bool flush)
{
    if (encodedPacket->ts.hasDts())
        packet->dts = round(encodedPacket->ts.dts() / time_base);
    if (encodedPacket->ts.hasPts())
        packet->pts = round(encodedPacket->ts.pts() / time_base);
    if (flush)
        avcodec_flush_buffers(codec_ctx);
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
        memcpy(&codec_ctx->reordered_opaque, &encodedPacket->sampleAspectRatio_, 8);
}

void FfmpegDecoder::decodeLastStep(PacketBasePtr &encodedPacket, AVFrame *frame)
{
    const int64_t ts = av_frame_get_best_effort_timestamp(frame);
   // qDebug("ts = %lld, base:%lf", ts, time_base);
    if (ts != AV_NOPTS_VALUE)
    {
        encodedPacket->ts = ts * time_base;
        //qDebug("->ts = %lld", (int64_t)(encodedPacket->ts.pts() *1000));
    }
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        frame->flags;
        double sampleAspectRatio;
        memcpy(&sampleAspectRatio, &frame->reordered_opaque, 8);
        if (qFuzzyIsNull(sampleAspectRatio) && frame->sample_aspect_ratio.num)
            encodedPacket->sampleAspectRatio_ = av_q2d(frame->sample_aspect_ratio);
    }

}

void FfmpegDecoder::setSupportedPixelFormats(const PlayerPixelFormats &pixelFormats)
{
    supportedPixelFormats = pixelFormats;
    setPixelFormat();
}

void FfmpegDecoder::setPixelFormat()
{
    qDebug() << "setPixelFormat = ";
    return;
    const AVPixFmtDescriptor *pixDesc = av_pix_fmt_desc_get(codec_ctx->pix_fmt);
    if (!pixDesc) //Invalid pixel format
        return;
    // 如果不支持则需要做转换
    auto result = find(supportedPixelFormats.begin( ), supportedPixelFormats.end( ),
                       (PlayerPixelFormat)codec_ctx->pix_fmt );
    if(result == supportedPixelFormats.end( ))
    {
        dontConvert = false;
    }
    else
    {
        dontConvert = true;
    }
    if (dontConvert)
    {
        chromaShiftW = pixDesc->log2_chroma_w;
        chromaShiftH = pixDesc->log2_chroma_h;
        desiredPixFmt = codec_ctx->pix_fmt;     // 期望的目标格式
    }
    else for (int i = 0; i < supportedPixelFormats.size(); ++i)
    {
        const AVPixFmtDescriptor *supportedPixDesc = av_pix_fmt_desc_get((AVPixelFormat)supportedPixelFormats.at(i));
        if (i == 0 || (supportedPixDesc->log2_chroma_w == pixDesc->log2_chroma_w && supportedPixDesc->log2_chroma_h == pixDesc->log2_chroma_h))
        {
            //Use first format as default (mostly QMPlay2PixelFormat::YUV420P) and look at next formats,
            //otherwise break the loop if found proper format.
            chromaShiftW = supportedPixDesc->log2_chroma_w;
            chromaShiftH = supportedPixDesc->log2_chroma_h;
            desiredPixFmt = (int)supportedPixelFormats.at(i);
            if (i != 0)
                break;
        }
    }
}

/**
 * @brief decoder不做像素的格式转换
 * @param encodedPacket
 * @param decoded
 * @param newPixFmt
 * @param flush
 * @param hurry_up
 * @return
 */
int FfmpegDecoder::decodeVideo(PacketBasePtr &encodedPacket, PacketBasePtr &decoded,
                               QByteArray &newPixFmt, bool flush, unsigned hurry_up)
{
    int bytes_consumed = 0, frameFinished = 0;

    packet = (AVPacket *)encodedPacket->getRealPacket();
    frame = (AVFrame *)decoded->getRealPacket();
    decodeFirstStep(encodedPacket, flush);
    int retCode = -1;
    if  (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        retCode = avcodec_send_packet(codec_ctx, packet);
        if(!retCode)
        {
            retCode = avcodec_receive_frame(codec_ctx, frame);
            switch (retCode)
            {
                case 0:
                   // qDebug("video decode a frame success\n");
                    static int64_t pts = 0;
                    //qDebug("decode duration = %lld", frame->pts - pts);
                    pts = frame->pts;
                    decodeLastStep(decoded, frame);
                    //qDebug("video ->ts = %lld", (int64_t)(encodedPacket->ts.pts() *1000));
                    break;
                case AVERROR_EOF:
                    qDebug("video the decoder has been fully flushed,\
                           and there will be no more output frames.\n");
                    break;

                case AVERROR(EAGAIN):
                    //qDebug("video Resource temporarily unavailable\n");
                    break;

                case AVERROR(EINVAL):
                    qDebug("video Invalid argument\n");
                    break;
                default:
                    break;
            }
        }

    }

#if 0
    if (frameFinished)
    {
        static int64_t pts = 0;
        qDebug("decode duration = %lld", frame->pts - pts);
        pts = frame->pts;
        decodeLastStep(encodedPacket, frame);
        encodedPacket->setRealPacket(frame);
        if(codec_ctx->refcounted_frames)
        {
           // av_frame_unref(frame);
        }
    }
    else
        encodedPacket->ts.setInvalid();
#endif
    return bytes_consumed < 0 ? -1 : bytes_consumed;
}

int FfmpegDecoder::putPacket(PacketBasePtr &encodedPacket, bool flush)
{
    int bytes_consumed = 0, frameFinished = 0;

    packet = (AVPacket *)encodedPacket->getRealPacket();

    decodeFirstStep(encodedPacket, flush);
    int retCode = -1;
    if  (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        retCode = avcodec_send_packet(codec_ctx, packet);
    }
    return retCode;
}
int FfmpegDecoder::takeFrame(PacketBasePtr &decoded)
{
    frame = (AVFrame *)decoded->getRealPacket();
    int retCode = avcodec_receive_frame(codec_ctx, frame);
    switch (retCode)
    {
        case 0:
           // qDebug("video decode a frame success\n");
            static int64_t pts = 0;
            //qDebug("decode duration = %lld", frame->pts - pts);
            pts = frame->pts;
            decodeLastStep(decoded, frame);
            //qDebug("video ->ts = %lld", (int64_t)(encodedPacket->ts.pts() *1000));
            break;
        case AVERROR_EOF:
            qDebug("video the decoder has been fully flushed,\
                   and there will be no more output frames.\n");
            break;

        case AVERROR(EAGAIN):
            //qDebug("video Resource temporarily unavailable\n");
            break;

        case AVERROR(EINVAL):
            qDebug("video Invalid argument\n");
            break;
        default:
            break;
    }
    return retCode;
}

int FfmpegDecoder::decodeAudio(PacketBasePtr &encodedPacket, PacketBasePtr &decoded,
                               uint8_t &channels, uint32_t &sampleRate, bool flush)
{
    int bytes_consumed = 0, frameFinished = 0;

    packet = (AVPacket *)encodedPacket->getRealPacket();
    frame = (AVFrame *)decoded->getRealPacket();

    decodeFirstStep(encodedPacket, flush);

    if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        int retCode = avcodec_send_packet(codec_ctx, packet);
        if(!retCode) {
            retCode = avcodec_receive_frame(codec_ctx, frame);
            switch (retCode) {
                case 0:
                    //qDebug("audio decode a frame success\n");
                    static int64_t pts = 0;
                    //qDebug("decode duration = %lld", frame->pts - pts);
                    pts = frame->pts;
                    decodeLastStep(decoded, frame);
                    //qDebug("audio ->ts = %lld", (int64_t)(encodedPacket->ts.pts() *1000));
                    channels = codec_ctx->channels;
                    sampleRate = codec_ctx->sample_rate;
                    break;
                case AVERROR_EOF:
                    qDebug("audio the decoder has been fully flushed,\
                           and there will be no more output frames.\n");
                    break;

                case AVERROR(EAGAIN):
                    qDebug("audio Resource temporarily unavailable\n");
                    break;

                case AVERROR(EINVAL):
                    qDebug("audio Invalid argument\n");
                    break;
                default:
                    break;
            }
        }
    }


    return bytes_consumed;
}
void FfmpegDecoder::flushBuffers()
{
    avcodec_flush_buffers(codec_ctx);
}

bool FfmpegDecoder::open(StreamInfo &streamInfo, VideoOut *writer)
{
    AVCodec *codec = init(streamInfo);
    if (!codec)
    {
        qDebug() << "FfmpegDecoder::open codec = " << codec;
        return false;
    }
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
#if 1
        if ((codec_ctx->thread_count = threads) != 1)
        {
            if (!thread_type_slice)
                codec_ctx->thread_type = FF_THREAD_FRAME;
            else
                codec_ctx->thread_type = FF_THREAD_SLICE;
        }
        av_codec_set_lowres(codec_ctx, qMin(av_codec_get_max_lowres(codec), lowres));
        codec_ctx->refcounted_frames = true;            // 这些参数的 设置
#endif
        codec_ctx->refcounted_frames = 0;
        lastPixFmt = codec_ctx->pix_fmt;
    }
    if (!openCodec(codec))
    {
        qDebug() << "FfmpegDecoder::open openCodec failed";
        return false;
    }
    time_base = streamInfo.getTimeBase();
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO && codec_ctx->lowres)
    {
        streamInfo.W = codec_ctx->width;
        streamInfo.H = codec_ctx->height;
    }
    return true;
}
