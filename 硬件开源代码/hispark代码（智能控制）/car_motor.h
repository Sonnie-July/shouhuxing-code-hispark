

#ifndef __CAR_Motor_H__
#define __CAR_Motor_H__

#include "car_config.h"

// 电机控制板初始化
hi_void Motor_Init(hi_void);

// 运行
hi_void Motor_Run(hi_u16 in1_duty, hi_u16 in2_duty, hi_u16 in3_duty, hi_u16 in4_duty);

// 撤销
hi_void Motor_Deinit(hi_void);

// 前进
hi_void Motor_Forward(hi_void);

// 后退
hi_void Motor_Backward(hi_void);

// 原地 左转
hi_void Motor_Left(hi_void);

// 原地 右转
hi_void Motor_Right(hi_void);

// 刹车 制动抱死
hi_void Motor_Stop(hi_void);

// 等待 空档
hi_void Motor_Wait(hi_void);

// 设置 左马达 速度
hi_void Motor_Set_Left_Velocity(hi_u16 vel);

// 设置 右马达 速度
hi_void Motor_Set_Right_Velocity(hi_u16 vel);

// 设置倍频
hi_void Motor_Set_Frequency(hi_u16 fre);


#endif