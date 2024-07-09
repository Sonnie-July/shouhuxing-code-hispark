/**
 *  MPU6050 初始化
 * 
 * iic接口、发送1byte数据、发送nbyte数据，读取1byte数据
 * Code By: HelloKun 2021.09.05
 * 
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_errno.h" //==IOT_SUCCESS =0 
#include <stddef.h>

#include "hi_io.h"   //上拉、复用
#include "hi_gpio.h" //hi_gpio_set_dir()、hi_gpio_set(get)_output(input)_val()
#include "hi_time.h"
#include "hi_i2c.h" 
#include "mpu6050.h"

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Init(void)
{
    uint8_t res; 
	
	MPU_Write_Data(MPU_PWR_MGMT1_REG,0X80,1);	//复位MPU6050
    hi_udelay(10000);
	MPU_Write_Data(MPU_PWR_MGMT1_REG,0X00,1);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(1);					//加速度传感器,±2g
	MPU_Set_Rate(50);						//设置采样率50Hz
	MPU_Write_Data(MPU_INT_EN_REG,0X00,1);	//关闭所有中断
	MPU_Write_Data(MPU_USER_CTRL_REG,0X00,1);	//I2C主模式关闭
	MPU_Write_Data(MPU_FIFO_EN_REG,0X00,1);	//关闭FIFO
	MPU_Write_Data(MPU_INTBP_CFG_REG,0X80,1);	//INT引脚低电平有效
	MPU_Read_Data(MPU_DEVICE_ID_REG,&res,1);  //确认器件地址 WHO AM I 地址0x75
	if(res==MPU_ADDR)//器件ID正确
	{
		printf("MPU6050 ID = %x\r\n",res); //0x68 十进制104
		MPU_Write_Data(MPU_PWR_MGMT1_REG,0X01,1);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Data(MPU_PWR_MGMT2_REG,0X00,1);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}else 
	{
		printf("MPU6050 Faild connected EEROR！Check Out MPU6050\r\n");
		return 1;
	}
	return 0;

}

/**************
 * Part 1 define mpu relay Fuction 
 * 
 */

/**
 * @brief Send Write command to MPU device.
 * @param regAddr the register address to Read or Writen.
 * @return Returns{@link IOT_SUCCESS} if the operation is successful;
 *  returns an error code defined in {@link iot_errno.h} otherwise.
 * */ 
uint8_t MPU_Cmd(uint8_t regAddr)
{ 
	hi_i2c_idx id = MPU_I2C_IDX;
    uint8_t buffer[] = {regAddr};
    hi_i2c_data i2cData;
	i2cData.receive_buf = NULL;
    i2cData.receive_len = 0;
    i2cData.send_buf = buffer;
    i2cData.send_len = sizeof(buffer)/sizeof(buffer[0]);
    return hi_i2c_write((hi_i2c_idx)id, ((MPU_ADDR<<1)|0), &i2cData);  //==发送器件地址+写命令 + 寄存器regAddr 
}

/**
 * @brief Write a  byte data to MPU device.
 * @param regAddr the register address.
 * 		  data the data to writen
 * @return Returns{@link IOT_SUCCESS} if the operation is successful;
 *  returns an error code defined in {@link iot_errno.h} otherwise.
 * */ 
uint32_t MPU_Write_Data(uint8_t regAddr, uint8_t *data, unsigned int dataLen)
{
    hi_i2c_idx id = MPU_I2C_IDX;
    uint8_t buffer[] = {regAddr, data};
    hi_i2c_data i2cData;
	i2cData.receive_buf = NULL;
    i2cData.receive_len = 0;
    i2cData.send_buf = buffer;
    i2cData.send_len = sizeof(buffer)/sizeof(buffer[0]);
    return hi_i2c_write((hi_i2c_idx)id, ((MPU_ADDR<<1)|0), &i2cData);  //==发送器件地址+写命令+ reg + data
}

/**
 * @brief Read a data byte from  MPU device.
 * @param  regAddr the register address.  8bit data
 * @return *data
 * */
uint32_t MPU_Read_Data(uint8_t regAddr, uint8_t *data, unsigned int dataLen)
{
    hi_i2c_idx id = MPU_I2C_IDX;
    hi_i2c_data i2cData;
    i2cData.send_buf = NULL;
    i2cData.send_len = 0;
    i2cData.receive_buf = data;
    i2cData.receive_len = dataLen;
	MPU_Cmd(regAddr);
    return hi_i2c_read((hi_i2c_idx)id,((MPU_ADDR<<1)|1), &i2cData);
}

/******************
 * Part 2 Initialize MPU data
 * */

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
  return MPU_Write_Data(MPU_GYRO_CFG_REG,fsr<<3,1); //设置陀螺仪满量程范围
}

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
  return MPU_Write_Data(MPU_ACCEL_CFG_REG,fsr<<3,1);//设置加速度传感器满量程范围
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_LPF( uint16_t lpf)
{   
	uint8_t data;
	if(lpf>=188)data=1; //带宽先查数据手册
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
   return MPU_Write_Data(MPU_CFG_REG,data,1);
}

//设置MPU6050的采样率(假定Fs=1KHz) 
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Rate( uint16_t rate)
{
   uint8_t data;
   if(rate>1000) rate =1000;
   if(rate<4) rate = 4;
   data =1000/rate-1;
   data =MPU_Write_Data(MPU_SAMPLE_RATE_REG,data,1);//设置数字低通滤波器 0X19采样频率分频器
   return MPU_Set_LPF(rate/2);	//自动设置DLPF为采样率的一半
}



/********************
 * Part 3 Get Mpu Data ！
 * 
 */	 

//01.得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    uint8_t buf[2]; 
    short raw;
	  float temp;
	  MPU_Read_Data(MPU_TEMP_OUTH_REG,&buf,2); 
	  //printf("MPU_Read_Temperature: %d\r\n",buf[0]);
    raw=((uint16_t)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp;
}

//02.得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Data(MPU_GYRO_XOUTH_REG,&buf,6); //加速度、角速度为H+L 一共6字节
	if(res==0)
	{
		//printf("0000000000");
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


//03.得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Data(MPU_ACCEL_XOUTH_REG,&buf,6);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


//04. 从Uart1输出所有原始数据
/*
	short Accel[3] 3轴加速度
	short Gyro[3]  3轴角速度
	float Temp;     温度
*/
void Print_Original_MPU_Data(void)
{
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度	    

	temp = MPU_Get_Temperature();
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	
	printf("Temperature: %d\r\n",temp);
	printf("Accelerates: X %d Y %d Z %d\r\n",aacx,aacy,aacz);
	printf("Gyroscope: Gx %d Gy %d Gz %d\r\n",gyrox,gyroy,gyroz);	
}









/*
***Soft iic code 2021.09.04 By HelloKun 软串口 获取不到数据，编译没问题。


#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_errno.h" //==IOT_SUCCESS =0 

#include "mpu6050.h"
#include "mpu_i2c.h" 
#include "hi_time.h"



    
uint8_t MPU_Init(void)
{ 
	
	uint8_t res; 
	MPU_IIC_Init();//   
	MPU_Write_Data(MPU_PWR_MGMT1_REG,0X80);	// 
    hi_udelay(10000);
	MPU_Write_Data(MPU_PWR_MGMT1_REG,0X00);	//   
	MPU_Set_Gyro_Fsr(3);					//   
	MPU_Set_Accel_Fsr(1);					// 
	MPU_Set_Rate(50);						//  
	MPU_Write_Data(MPU_INT_EN_REG,0X00);	// 
	MPU_Write_Data(MPU_USER_CTRL_REG,0X00);	//
	MPU_Write_Data(MPU_FIFO_EN_REG,0X00);	// 
	MPU_Write_Data(MPU_INTBP_CFG_REG,0X80);	//
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);  //
	if(res==MPU_ADDR)//   
	{
		printf("MPU6050 ID = %x \n",res); //
		MPU_Write_Data(MPU_PWR_MGMT1_REG,0X01);	//    
		MPU_Write_Data(MPU_PWR_MGMT2_REG,0X00);	//     
		MPU_Set_Rate(50);						//  
 	}else 
	{
		printf("MPU6050 Faild connected EEROR     MPU6050 Ľ   \n");
		return 1;
	}
	return 0;
}


uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Data(MPU_GYRO_CFG_REG,fsr<<3);//     
}



uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Data(MPU_ACCEL_CFG_REG,fsr<<3);//   
}



uint8_t MPU_Set_LPF( uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1; //    
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Data(MPU_CFG_REG,data);//     
}



uint8_t MPU_Set_Rate( uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Data(MPU_SAMPLE_RATE_REG,data);	//  
 	return MPU_Set_LPF(rate/2);	//
}


   
uint8_t MPU_Write_Data(uint8_t reg,uint8_t data) 				 
{ 
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//    	
	if(MPU_IIC_Wait_Ack())	// 
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//
    MPU_IIC_Wait_Ack();		// 
	MPU_IIC_Send_Byte(data);//        
	if(MPU_IIC_Wait_Ack())	//
	{
		MPU_IIC_Stop();	 
		return 1;		 
	}		 
    MPU_IIC_Stop();	 
	return 0;
}

   
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//          	
	if(MPU_IIC_Wait_Ack())	//  
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	
    MPU_IIC_Wait_Ack();		
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//        
		if(MPU_IIC_Wait_Ack())		//
		{
			MPU_IIC_Stop();	 
			return 1;		 
		}		
	}    
    MPU_IIC_Stop();	 
	return 0;	
} 

     
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res;
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//   
	MPU_IIC_Wait_Ack();		//  
    MPU_IIC_Send_Byte(reg);	// 
    MPU_IIC_Wait_Ack();		// 
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//  
    MPU_IIC_Wait_Ack();		// 
	res=MPU_IIC_Read_Byte(0);// 
    MPU_IIC_Stop();			//        
	return res;		
}

   
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//       	
	if(MPU_IIC_Wait_Ack())	// 
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//
    MPU_IIC_Wait_Ack();		// 
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);//        	
    MPU_IIC_Wait_Ack();		//   
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//    
		else *buf=MPU_IIC_Read_Byte(1);		//     
		len--;
		buf++; 
	}    
    MPU_IIC_Stop();	//  
	return 0;	
}


short MPU_Get_Temperature(void)
{
    uint8_t buf[2]; 
    short raw;
	  float temp;
	  MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
	  //printf("MPU_Read_Temperature: %d\r\n",buf[0]);
    raw=(( uint16_t)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}


      
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf); //   ٶȡ    ٶ ΪH+L һ  6 ֽ 
	if(res==0)
	{
		*gx=(( uint16_t)buf[0]<<8)|buf[1];  
		*gy=(( uint16_t)buf[2]<<8)|buf[3];  
		*gz=(( uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


    
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=(( uint16_t)buf[0]<<8)|buf[1];  
		*ay=(( uint16_t)buf[2]<<8)|buf[3];  
		*az=(( uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


void Print_Original_MPU_Data(void)
{
	short aacx,aacy,aacz;		//      
	short gyrox,gyroy,gyroz;	//     
	short temp;					//	    
	
	temp = MPU_Get_Temperature();
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	
	printf("   %8d",temp);
	printf("\r\n   %d %d %d ",aacx,aacy,aacz);
	printf("     %8d%8d%8d    ",gyrox,gyroy,gyroz);
	
		
}*/
