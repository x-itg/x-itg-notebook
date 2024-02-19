#ifndef AVPACKETS_QUEUE_H
#define AVPACKETS_QUEUE_H
#include <mutex>
#include <condition_variable>

#ifdef __cplusplus
extern "C"{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus
}
#endif

typedef struct PacketQueue
{
    AVPacketList    *first_pkt;     //队头的一个packet, 注意类型不是AVPacket
    AVPacketList    *last_pkt;      //队尾packet
    int             nb_packets;     // paket个数
    int             size;           //
    int             seq;
    std::mutex       mutex;         //
}PacketQueue;

void PacketQueueInit(PacketQueue *queue);
int PacketQueuePut(PacketQueue *queue, AVPacket *packet);
int PacketQueueTake(PacketQueue *queue, AVPacket *pakcet, int block);
int PacketQueueGetSize(PacketQueue *queue);



#endif // AVPACKETS_QUEUE_H
