#ifndef FFMPEG_DECODER_H
#define FFMPEG_DECODER_H

#include "decoder.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/replaygain.h>
#include <libavutil/pixdesc.h>
}
class FfmpegDecoder : public Decoder
{
public:
    explicit FfmpegDecoder();
    virtual ~FfmpegDecoder();
    std::string name() const {return std::string("FfmpegDecoder");}

    AVCodec *init(StreamInfo &streamInfo);
    bool openCodec(AVCodec *codec);

    void decodeFirstStep(const PacketBasePtr &encodedPacket, bool flush);
    void decodeLastStep(PacketBasePtr &encodedPacket, AVFrame *frame);

    virtual void setSupportedPixelFormats(const PlayerPixelFormats &pixelFormats);
    void setPixelFormat();
    /*
     * hurry_up ==  0 -> no frame skipping, normal quality
     * hurry_up >=  1 -> faster decoding, lower image quality, frame skipping during decode
     * hurry_up == ~0 -> much faster decoding, no frame copying
    */
    virtual int decodeVideo(PacketBasePtr &encodedPacket, PacketBasePtr &decoded, QByteArray &newPixFmt, bool flush, unsigned hurry_up);
    int putPacket(PacketBasePtr &encodedPacket, bool flush);
    int takeFrame(PacketBasePtr &decoded);
    virtual int decodeAudio(PacketBasePtr &encodedPacket, PacketBasePtr &decoded, uint8_t &channels, uint32_t &sampleRate, bool flush = false);
    //virtual bool decodeSubtitle(const PacketBasePtr &encodedPacket, double pos, QMPlay2OSD *&osd, int w, int h);
    virtual void flushBuffers();
private:
    virtual bool open(StreamInfo &streamInfo, VideoOut *writer = nullptr);
    AVCodecContext *codec_ctx = nullptr;
    AVPacket *packet;           // 不由内部分配?
    AVFrame *frame = nullptr;             // 不由内部分配?

   PlayerPixelFormats supportedPixelFormats;
   uint8_t chromaShiftW, chromaShiftH;
   int desiredPixFmt;               // 输出支持的格式，其实解码器只需要解出数据即可，另外的模块做格式转换
   bool dontConvert;
   double time_base;
   int threads, lowres;
   bool respectHurryUP, skipFrames, forceSkipFrames, thread_type_slice;
   SwsContext *sws_ctx;
   int lastFrameW, lastFrameH, lastPixFmt;
};

#endif // FFMPEG_DECODER_H
