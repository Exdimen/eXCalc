#include "SystemExtra.h"
#include <chrono>

using namespace std;
/** 
 * @brief 截止时间检测
 * @param deadline 截止时间戳
 * @retval bool 是否到时间了
 * */
bool TimeUpCheck(uint32_t deadline) {
	return millis()>=deadline;
}

void PeriodTriggerInit(PeriodTrigger* pt, uint32_t period) {
	pt->period = period;
	pt->last_timestamp = 0;
}
bool PeriodTriggerCheck(PeriodTrigger* pt) {
	return TimeUpCheck(pt->last_timestamp + pt->period);
}

void CntDetectorInit(CntDetector* cd) {
    cd->cnt = 0;
    cd->last_cnt = 0;
}
void CntDetectorUpdate(CntDetector* cd) {
    cd->cnt ++;
}
bool CntDetectorCheck(CntDetector* cd) {
    return cd->cnt != cd->last_cnt;
}
/**
 * @brief 获取系统时钟，单位us
 * 
 * @return uint32_t 时钟
 */
uint32_t micros(void)
{
    uint32_t ret = chrono::duration_cast<chrono::microseconds>(chrono::system_clock().now().time_since_epoch()).count();
    return ret;
}
/**
 * @brief 获取系统时钟，单位ms
 * 
 * @return uint32_t 时钟
 */
uint32_t millis(void) {
    uint32_t ret = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock().now().time_since_epoch()).count();
    return ret;
	// return HAL_GetTick();
}