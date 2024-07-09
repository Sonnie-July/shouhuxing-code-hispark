

#ifndef __CAR_HCSR04_H__
#define __CAR_HCSR04_H__

#include "car_config.h"

// 读取 系统时间 微秒 这个系统时间独立的 不被其他进程影响
static time_t sys_time_us(void);

// 初始设置 管脚设置
hi_void Hcsr04_Init(hi_void);

// 测量距离 返回单位 毫米 测距时间间隔大于60ms
hi_s32 Hcsr04_Measure(hi_void);


#endif