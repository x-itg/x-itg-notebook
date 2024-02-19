#ifndef AV_CLOCK_H
#define AV_CLOCK_H
#include <cstdint>
void AVClockResetTime(int64_t baseTime);       // 由用户去设置
int64_t AVClockGetCurTime();
void AVClockSetTime(int64_t time);
void AVClockEnable();
void AVClockDisable();
bool AVClockIsEnable();

#endif // AV_CLOCK_H
