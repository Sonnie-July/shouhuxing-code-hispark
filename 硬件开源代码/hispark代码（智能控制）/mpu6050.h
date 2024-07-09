/***
 * MPU6050 接入Hi3861
 * iic通信，使用Hi3861的 I2C0 (I2C1 给oled)
 * 
 */
#ifndef MPU6050_H
#define MPU6050_H

#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_errno.h" //==IOT_SUCCESS =0 

#include "hi_io.h"   //上拉、复用
#include "hi_gpio.h" //hi_gpio_set_dir()、hi_gpio_set(get)_output(input)_val()
#include "hi_time.h"
#include "hi_i2c.h"

#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG			0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG			0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG			0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG			0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
 
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU_ADDR				0X68

#define MPU_SDA_IO13  13
#define MPU_SCL_IO14  14  
#define MPU_SDA_IO10  10
#define MPU_SCL_IO9  9  
#define MPU_I2C_IDX   0  //hi3861 iic0
#define MPU_I2C_BAUDRATE 50 //iic work frequency (400*1000) 

uint8_t MPU_Init(void); 
uint8_t MPU_Cmd(uint8_t regAddr);	
uint32_t MPU_Write_Data(uint8_t regAddr, uint8_t *data, unsigned int dataLen);	//dataLen NO use					
uint32_t MPU_Read_Data(uint8_t regAddr, uint8_t *data, unsigned int dataLen);					

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);
uint8_t MPU_Set_LPF( uint16_t lpf);
uint8_t MPU_Set_Rate( uint16_t rate);



short MPU_Get_Temperature(void);
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az);
void Print_Original_MPU_Data(void);



#endif






























/* **Soft iic code 2021.09.04 By HelloKun 软串口 获取不到数据，编译没问题。

#ifndef __MPU6050_H
#define __MPU6050_H

#include "mpu_i2c.h" 
#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"

#include "iot_errno.h" //==IOT_SUCCESS =0 

#define MPU_SELF_TESTX_REG		0X0D	// Լ Ĵ   X
#define MPU_SELF_TESTY_REG		0X0E	// Լ Ĵ   Y
#define MPU_SELF_TESTZ_REG		0X0F	// Լ Ĵ   Z
#define MPU_SELF_TESTA_REG		0X10	// Լ Ĵ   A
#define MPU_SAMPLE_RATE_REG		0X19	//    Ƶ ʷ Ƶ  
#define MPU_CFG_REG				0X1A	//   üĴ   
#define MPU_GYRO_CFG_REG		0X1B	//         üĴ   
#define MPU_ACCEL_CFG_REG		0X1C	//   ٶȼ    üĴ   
#define MPU_MOTION_DET_REG		0X1F	// ˶   ֵⷧ   üĴ   
#define MPU_FIFO_EN_REG			0X23	//FIFOʹ ܼĴ   
#define MPU_I2CMST_CTRL_REG		0X24	//IIC       ƼĴ   
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC ӻ 0      ַ Ĵ   
#define MPU_I2CSLV0_REG			0X26	//IIC ӻ 0   ݵ ַ Ĵ   
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC ӻ 0   ƼĴ   
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC ӻ 1      ַ Ĵ   
#define MPU_I2CSLV1_REG			0X29	//IIC ӻ 1   ݵ ַ Ĵ   
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC ӻ 1   ƼĴ   
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC ӻ 2      ַ Ĵ   
#define MPU_I2CSLV2_REG			0X2C	//IIC ӻ 2   ݵ ַ Ĵ   
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC ӻ 2   ƼĴ   
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC ӻ 3      ַ Ĵ   
#define MPU_I2CSLV3_REG			0X2F	//IIC ӻ 3   ݵ ַ Ĵ   
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC ӻ 3   ƼĴ   
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC ӻ 4      ַ Ĵ   
#define MPU_I2CSLV4_REG			0X32	//IIC ӻ 4   ݵ ַ Ĵ   
#define MPU_I2CSLV4_DO_REG		0X33	//IIC ӻ 4д   ݼĴ   
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC ӻ 4   ƼĴ   
#define MPU_I2CSLV4_DI_REG		0X35	//IIC ӻ 4     ݼĴ   

#define MPU_I2CMST_STA_REG		0X36	//IIC    ״̬ Ĵ   
#define MPU_INTBP_CFG_REG		0X37	// ж /  ·   üĴ   
#define MPU_INT_EN_REG			0X38	// ж ʹ ܼĴ   
#define MPU_INT_STA_REG			0X3A	// ж ״̬ Ĵ   

#define MPU_ACCEL_XOUTH_REG		0X3B	//   ٶ ֵ,X   8λ Ĵ   
#define MPU_ACCEL_XOUTL_REG		0X3C	//   ٶ ֵ,X   8λ Ĵ   
#define MPU_ACCEL_YOUTH_REG		0X3D	//   ٶ ֵ,Y   8λ Ĵ   
#define MPU_ACCEL_YOUTL_REG		0X3E	//   ٶ ֵ,Y   8λ Ĵ   
#define MPU_ACCEL_ZOUTH_REG		0X3F	//   ٶ ֵ,Z   8λ Ĵ   
#define MPU_ACCEL_ZOUTL_REG		0X40	//   ٶ ֵ,Z   8λ Ĵ   

#define MPU_TEMP_OUTH_REG		0X41	// ¶ ֵ ߰ λ Ĵ   
#define MPU_TEMP_OUTL_REG		0X42	// ¶ ֵ  8λ Ĵ   

#define MPU_GYRO_XOUTH_REG		0X43	//      ֵ,X   8λ Ĵ   
#define MPU_GYRO_XOUTL_REG		0X44	//      ֵ,X   8λ Ĵ   
#define MPU_GYRO_YOUTH_REG		0X45	//      ֵ,Y   8λ Ĵ   
#define MPU_GYRO_YOUTL_REG		0X46	//      ֵ,Y   8λ Ĵ   
#define MPU_GYRO_ZOUTH_REG		0X47	//      ֵ,Z   8λ Ĵ   
#define MPU_GYRO_ZOUTL_REG		0X48	//      ֵ,Z   8λ Ĵ   

#define MPU_I2CSLV0_DO_REG		0X63	//IIC ӻ 0   ݼĴ   
#define MPU_I2CSLV1_DO_REG		0X64	//IIC ӻ 1   ݼĴ   
#define MPU_I2CSLV2_DO_REG		0X65	//IIC ӻ 2   ݼĴ   
#define MPU_I2CSLV3_DO_REG		0X66	//IIC ӻ 3   ݼĴ   

#define MPU_I2CMST_DELAY_REG	0X67	//IIC      ʱ     Ĵ   
#define MPU_SIGPATH_RST_REG		0X68	// ź ͨ    λ Ĵ   
#define MPU_MDETECT_CTRL_REG	0X69	// ˶      ƼĴ   
#define MPU_USER_CTRL_REG		0X6A	// û    ƼĴ   
#define MPU_PWR_MGMT1_REG		0X6B	//  Դ     Ĵ   1
#define MPU_PWR_MGMT2_REG		0X6C	//  Դ     Ĵ   2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO     Ĵ    ߰ λ
#define MPU_FIFO_CNTL_REG		0X73	//FIFO     Ĵ    Ͱ λ
#define MPU_FIFO_RW_REG			0X74	//FIFO  д Ĵ   
#define MPU_DEVICE_ID_REG		0X75	//    ID Ĵ   
 
//   AD0  (9  ) ӵ ,IIC  ַΪ0X68(         λ).
//     V3.3,  IIC  ַΪ0X69(         λ).
#define MPU_ADDR				0X68


uint8_t MPU_Init(void); 								//  ʼ  MPU6050
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);//IIC    д
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf); //IIC       
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data);				//IICдһ   ֽ 
uint8_t MPU_Read_Byte(uint8_t reg);						//IIC  һ   ֽ 

uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr);
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr);
uint8_t MPU_Set_LPF( uint16_t lpf);
uint8_t MPU_Set_Rate( uint16_t rate);
uint8_t MPU_Set_Fifo(uint8_t sens);


short MPU_Get_Temperature(void);
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az);
void Print_Original_MPU_Data(void);

#endif


*/

































