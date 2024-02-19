#ifndef AUDIO_H
#define AUDIO_H


#ifdef __cplusplus
extern "C"
{
#endif

#include <libavformat/avformat.h>

#ifdef __cplusplus
}
#endif

int AudioInit(AVCodecContext	*pAudioCodecCtx);
void AudioPacketPush(AVPacket *packet);
void AudioPlay( const uint8_t *data, const uint32_t size,  int stop );
void AudioDecodeThread( void *userdata );
void AudioSetTimeBase(double timeBase);
void AudioClose(void);
int AudioPacketSize();
#endif // AUDIO_H
