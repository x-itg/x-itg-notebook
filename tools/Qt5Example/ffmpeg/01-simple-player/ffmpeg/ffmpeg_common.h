#ifndef FFMPEG_COMMON_H
#define FFMPEG_COMMON_H

#include <cstdint>
#define PLAYER_NOPTS_VALUE ((int64_t)AV_NOPTS_VALUE)

struct AVDictionary;
struct AVPacket;

class FfmpegCommon
{
public:
    FfmpegCommon();
    static AVPacket *createAVPacket();
    static void freeAVPacket(AVPacket *&packet);
};

#endif // FFMPEG_COMMON_H
