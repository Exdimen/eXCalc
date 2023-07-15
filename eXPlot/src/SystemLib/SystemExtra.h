#pragma once
#include <stdbool.h>
#include <stdint.h>


// 触发器
typedef struct {
    uint32_t period;            // 触发周期
    uint32_t last_timestamp;    // 上次触发时间戳
} PeriodTrigger;
void PeriodTriggerInit(PeriodTrigger* pt, uint32_t period);
bool PeriodTriggerCheck(PeriodTrigger* pt);

// 计数检测器
typedef struct {
    uint32_t cnt;               // 当前计数
    uint32_t last_cnt;          // 上一次检测时计数   
} CntDetector;
void CntDetectorInit(CntDetector* cd);
void CntDetectorUpdate(CntDetector* cd);
bool CntDetectorCheck(CntDetector* cd);

// 模块状态
enum EdmStatus {
    status_null = 0x00,
    status_init,
    status_init_failed,
    status_normal,
    status_communicate_warning,
    status_communicate_error,
	status_data_warning,
    status_data_error,
    status_thread_error
};

bool TimeUpCheck(uint32_t deadline);
uint32_t micros(void);
uint32_t millis(void);