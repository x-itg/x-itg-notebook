#include "avpackets_queue.h"
#include "log.h"


void PacketQueueInit(PacketQueue *queue)
{
    queue->first_pkt    = NULL;
    queue->last_pkt     = NULL;
    queue->size 		= 0;
    queue->nb_packets = 0;
    queue->seq = 0;
}


int PacketQueuePut(PacketQueue *queue, AVPacket *packet)
{
    AVPacketList   *pkt1;

    pkt1 = (AVPacketList *)av_malloc(sizeof(AVPacketList));
    if (pkt1 == NULL)
    {
        return -1;
    }

    pkt1->pkt   = *packet;
    pkt1->next  = NULL;

    //上锁
    queue->seq++;

    std::lock_guard<std::mutex> lock(queue->mutex);
    if (!queue->last_pkt)    //空队
    {
        queue->first_pkt = pkt1;
    }
    else
    {
        queue->last_pkt->next = pkt1;
    }

    queue->last_pkt = pkt1;
    queue->nb_packets++;
    queue->size += packet->size;

    return 0;
}


int PacketQueueTake(PacketQueue *queue, AVPacket *pkt, int block)
{
    AVPacketList   *pkt1 = NULL;
    int            ret = 0;

    //上锁
    std::lock_guard<std::mutex> lock(queue->mutex);

    pkt1 = queue->first_pkt;
    if (pkt1)
    {
        queue->first_pkt = pkt1->next;
        if (!queue->first_pkt)
        {
            queue->last_pkt = NULL;
        }

        queue->nb_packets--;
        queue->size -= pkt1->pkt.size;
        *pkt = pkt1->pkt;
        av_free(pkt1);
        ret = 0;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

int PacketQueueGetSize(PacketQueue *queue)
{
    // std::lock_guard<std::mutex> lock(queue->mutex);
    return queue->size;
}

