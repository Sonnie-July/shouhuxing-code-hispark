

#ifndef __CAR_QMC5883_H__
#define __CAR_QMC5883_H__

#include "car_config.h"

// 3861接入端口设置 i2c设置
// hi_void Hi3861_Gpio_Init(hi_void);

// qmc5883 写入 指令
static hi_void Qmc5883_I2c_Write(hi_u8 addr, hi_u8 data);

// qmc5883 读取 状态
static hi_void Qmc5883_I2c_Read(hi_u8 addr, hi_u8* data_receive_buf, hi_u32 data_receive_len);

// Qmc5883初始设置
hi_void Qmc5883_Init(hi_void);

// 返回值结果 hi_u8 datas[6]
hi_bool Qmc5883_Measure(hi_u8 *datas);

// 输出角度 hi_u8 dats[6] hi_s32 ang XY平面的角度
hi_s32 Qmc5883_Angle(hi_u8 *dats);



#endif