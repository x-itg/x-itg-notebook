#include "av_clock.h"
#include <chrono>
#include <cinttypes>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "log.h"
extern "C"
{
#include <libavutil/time.h>
}

int64_t baseTime_ = 0;      // 基础时间,单位毫秒
int64_t startTime_ = 0;
int64_t offsetTime_ = 0;
bool enable_ = false;


static int64_t getNowTime()
{
    auto time_now = std::chrono::system_clock::now();
    auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
    return duration_in_ms.count();
}
void AVClockResetTime(int64_t baseTime)       // 由用户去设置
{
    startTime_ =  getNowTime();

    baseTime_ = baseTime;
    LogDebug("AVClock::startTime_ = %lld, baseTime = %lld", startTime_, baseTime_);
}

int64_t AVClockGetCurTime()
{
//    LogDebug("AVClock::getCurTime av_gettime = %lld, startTime = %lld, baseTime = %lld, offsetTime = %lld",
//           av_gettime()/1000, startTime_, baseTime_, offsetTime_);
    return (getNowTime() - startTime_ + baseTime_ + offsetTime_);
}

void AVClockSetTime(int64_t time)
{
    // 重新设置时间时，则主要是改变
    offsetTime_ = time - AVClockGetCurTime();      // >0 即是时间提前， 小于0则是时间退后
}

void AVClockEnable()
{
    enable_ = true;
}

void AVClockDisable()
{
    enable_ = false;
}

bool AVClockIsEnable()
{
    return enable_;
}
