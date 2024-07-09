

/**
 * 
 * 
 * 
    MPU6050

    AD0 - 接地 = 设备地址 [ 0xD0 ]

    电源管理寄存器1 0x6B
    [7] = 1 复位 数据清零
    [6] = 1 休眠模式 = 0 正常模式
    [3] = 0 使能 温度传感器
    [210] 选择系统时钟 
    000 内部8M RC晶振
    001 PLL，使用X轴陀螺作为参考
    010 PLL，使用Y轴陀螺作为参考
    011 PLL，使用Z轴陀螺作为参考
    100 PLL，使用外部32.768Khz作为参考
    101 PLL，使用外部19.2Mhz作为参考
    110 保留
    111 关闭时钟，保持时序产生电路复位状态

    陀螺仪配置寄存器 0x1B
    [43] 
    00 250‘/s
    01 500’/s
    10 1000‘/s
    11 2000’/s

    2000‘ = 0001 1000 = 0x18
 
    加速度传感器配置寄存器 0x1C
    [43]
    00 2g
    01 4g
    10 8g
    11 16g

    2g = 0000 0000 = 0x00


    FIFO 使能寄存器 0x23
    00000000 禁用 
    11111111 使能

    陀螺仪采样率分频寄存器 0x19

    设置MPU6050的陀螺仪采样频率

    采样频率 = 陀螺仪输出频率 / （1+SMPLRT—DIV） ???

    陀螺仪输出频率 1KHz/8KHz 低通滤波器设置

    输出频率 = 1K SMPLRT_DIV = 50 

    1K /（1+50） = 19

    配置寄存器 0x1A

    低通滤波器 设置

    [210]

    DLPF_CFG[2:0]	加速度传感器Fs=1Khz		|	角速度传感器（陀螺仪）

				    带宽(Hz)	延迟（ms）	|   带宽(Hz)    延迟（ms）  Fs(Khz)

    000				260			0               256			0.98		8
    001				184			2.0 			188			1.9 		1
    010				94			3.0 			98			2.8 		1
    011				44			4.9 			42			4.8 		1
    100				21 			8.5 			20 			8.3 		1
    101				10 			13.8 			10 			13.4 		1
    110				5 			19.0 			5 			18.6 		1
    111				保留						保留                     8

    电源管理寄存器2 0x6C

    0x00

    加速度传感器 数据输出寄存器 0x3B ~ 0x40 

    高字节在前

    0x3B 0x3C accel_xout
    0x3D 0x3E accel_yout
    0x3F 0x40 accel_zout

    温度传感器 数据输出 寄存器 0x41 ~ 0x42

    0x41 0x42 temp_out = regval

    Temperature = 36.53 + regval / 340

    陀螺仪数据输出寄存器 0x43 ~ 0x48

    0x43 0x44 gyro_xout
    0x45 0x46 gyro_yout
    0x47 0x48 gyro_zout

    short	2 字节	-32,768 到 32,767

    unsigned short	2 字节	0 到 65,535

    每个数据2个字节，值是16位有符号整数，正值正向，负值负向。

    加速度计算： 
    
    若取值2g 重力加速度取值9.8 AX = 2 * 9.8 * ( dx / 32768 ) ; // 为什么取32768，因为-1到0也是一个跨度值

    角速度计算：

    正值顺时针方向，负值逆时针方向

    若取值2000度/秒 Gx = 2000 * ( dx / 32768 );

 * 
 * 
 * 
 * 
 **/


#include "car_mpu6050.h"


static hi_u8 mpu6050_data[14] = {0};   // 直采数据
static hi_u8 data_len = 14;            // 直采数组长度

static hi_s16 value[7] = {0};          // 字节数组转整形结果值


//  写入 1条 指令
static hi_void Mpu6050_I2c_Write(hi_u8 addr, hi_u8 data)
{
    hi_u8 data_send_buf[] = {addr, data};

    hi_u32 status = 0;  // 状态

    hi_i2c_data mpu6050_i2c_data = {0};

    mpu6050_i2c_data.send_buf = data_send_buf;
    mpu6050_i2c_data.send_len = 2;

    status = hi_i2c_write(HI_I2C_IDX_0, 0xD0, &mpu6050_i2c_data);   // 0xD0 MPU6050地址

    if(status != HI_ERR_SUCCESS)
    {
        printf("\n [Mpu6050_I2c_Write] Failed \n");
    }
}


//  读取 n条 数据
static hi_void Mpu6050_I2c_Read(hi_u8 addr, hi_u8* data_receive_buf, hi_u32 data_receive_len)
{
    hi_u8 data_send_buf[] = {addr};

    hi_u32 status = 0;  // 状态

    hi_i2c_data mpu6050_i2c_data = {0};

    mpu6050_i2c_data.send_buf = data_send_buf;
    mpu6050_i2c_data.send_len = 1;

    status = hi_i2c_write(HI_I2C_IDX_0, 0xD0, &mpu6050_i2c_data);   // 0xD0 MPU6050地址

    if(status != HI_ERR_SUCCESS)
    {
        printf("\n [Mpu6050_I2c_Read] Write Failed \n");
    }

    mpu6050_i2c_data.receive_buf = data_receive_buf;
    mpu6050_i2c_data.receive_len = data_receive_len;

    status = hi_i2c_read(HI_I2C_IDX_0, 0xD1, &mpu6050_i2c_data);

    if(status != HI_ERR_SUCCESS)
    {
        printf("\n [Mpu6050_I2c_Read] Read Failed \n");
    }
}


//  0x6B    电源管理寄存器1
//  0x6C    电源管理寄存器2 

//  0x1B    陀螺仪配置寄存器
//  0x1C    加速度计配置寄存器
//  0x19    采样频率分频器
//  0x1A    配置寄存器

//  0x38    中断使能寄存器
//  0x6A    用户控制寄存器
//  0x23    FIFO使能寄存器
//  0x37    中断/旁路设置寄存器
//  0x75    器件ID寄存器

// MPU6050 初始设置  这个参数 最后根据实际情况 不断修正
hi_void Mpu6050_Init(hi_void)
{
    Mpu6050_I2c_Read(0x75, mpu6050_data, 1);    // 0x75 MPU6050的设备标识地址

    if(mpu6050_data[0] == 0x68) // 读取标识位ID==0x68表明设备在线正常
    {

        Mpu6050_I2c_Write(0x6b, 0x80);  // [7] = 1 复位 所有数据自动清零
        hi_udelay(100 * 1000);          // 等待100ms

        Mpu6050_I2c_Write(0x6b, 0x00);  // 唤醒 正常模式 温度计使能 内部8MHz晶振

        Mpu6050_I2c_Write(0x1b, 0x18);  // 角速度 不自检 测量范围 2000dps
        Mpu6050_I2c_Write(0x1c, 0x00);  // 加速度 不自检 测量范围 2g

        Mpu6050_I2c_Write(0x19, 0x07);  // 采样率 125Hz
        Mpu6050_I2c_Write(0x1a, 0x06);  // 低通滤波频率 5Hz   

        Mpu6050_I2c_Write(0x38, 0x00);  // 关闭所有中断
        Mpu6050_I2c_Write(0x6a, 0x00);  // i2c 主模式
        Mpu6050_I2c_Write(0x23, 0x00);  // fifo 关闭
        
        Mpu6050_I2c_Write(0x37, 0x80);  // int引脚低电平有效
        Mpu6050_I2c_Write(0x6c, 0x00);  // 加速器、陀螺仪工作

    }

    printf("\n car_mpu6050 [Mpu6050_Init] \n");
}

// // 3861接入端口设置 i2c设置
// hi_void Hi3861_Gpio_Init(hi_void)
// {
//     hi_u32 ret;

//     hi_io_set_func(HI_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_I2C0_SDA);
//     hi_io_set_func(HI_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_I2C0_SCL);

//     ret = hi_i2c_init(HI_I2C_IDX_0, 400000);

//     if (ret != HI_ERR_SUCCESS)
//     {
//         printf(" [hi_i2c_init] Failed \n");
//     }
// }


// 返回 字节型数据

// 返回整数数据 hi_u8 mpu6050_data[14] = {0} hi_u8 data_len = 14;
hi_void Mpu6050_Measure_By(hi_u8 *datas, hi_u8 len)
{
    Mpu6050_I2c_Read(0x3b, datas, len);

    printf("\n car_mpu6050 [Mpu6050_Measure_By] \n");
}


// 返回整数数据 hi_s16 datas[7] = {0};
hi_void Mpu6050_Measure_Sh(hi_s16 *datas)
{
    Mpu6050_Measure_By(mpu6050_data, data_len); 

    datas[0] = (hi_s16)mpu6050_data[0] << 8 | mpu6050_data[1];      // accel x
    datas[1] = (hi_s16)mpu6050_data[2] << 8 | mpu6050_data[3];      // accel y
    datas[2] = (hi_s16)mpu6050_data[4] << 8 | mpu6050_data[5];      // accel z
    datas[3] = (hi_s16)mpu6050_data[6] << 8 | mpu6050_data[7];      // temperature
    datas[4] = (hi_s16)mpu6050_data[8] << 8 | mpu6050_data[9];      // gyro x
    datas[5] = (hi_s16)mpu6050_data[10] << 8 | mpu6050_data[11];    // gyro y
    datas[6] = (hi_s16)mpu6050_data[12] << 8 | mpu6050_data[13];    // gyro z

    printf("\n car_mpu6050 [Mpu6050_Measure_Sh] \n");
}

// 返回浮点结果 hi_double *datas[7] = {0.0}; 
hi_void Mpu6050_Measure_Do(hi_double *datas)
{
    Mpu6050_Measure_Sh(value);

    datas[0] = (hi_double)value[0]/32768*2*980; // 单位 厘米 
    datas[1] = (hi_double)value[1]/32768*2*980; 
    datas[2] = (hi_double)value[2]/32768*2*980; 

    datas[3] = (hi_double)value[3]/340+36.53;

    datas[4] = (hi_double)value[4]/32768*2000; 
    datas[5] = (hi_double)value[5]/32768*2000; 
    datas[6] = (hi_double)value[6]/32768*2000;  

    printf("\n car_mpu6050 [Mpu6050_Measure_Do] \n");
}


// 加速度 在XY 平面的方向 和 大小 把字节数据
hi_void Mpu6050_ACC(hi_u8 *ds, hi_s32 *ac)
{
    if(ds == NULL)
    {
        return;
    }

    short ax = (short)ds[0] << 8 | ds[1];        // accel x
    short ay = (short)ds[2] << 8 | ds[3];        // accel y

    // ax 与 ay （atan）反正切 求方向 ， sqrt(ax * ax + ay * ay) = 加速度大小

    int ax1 = abs(ax);
    int ay1 = abs(ay);

    double ax2 = (double)ax1/32768*2*9800;    // 1g = 9800毫米
    double ay2 = (double)ay1/32768*2*9800; 

    double av = sqrt(ax2 * ax2 + ay2 * ay2);

    // 反正切函数 求弧度
    double an = atan( (double)ay1 / (double)ax1 );

    // 弧度转角度
    an = an * 180 / 3.14159;

    // 第一象限 没变化

    // 第二象限
    if (ax < 0 && ay >= 0)
    {
        an = 180 - an;
    }

    // 第三象限
    if (ax < 0 && ay < 0)
    {
        an = 180 + an;
    }

    // 第四象限
    if (ax >= 0 && ay < 0)
    {
        an = 360 - an;
    }    

    ac[0] = (hi_s32)an;
    ac[1] = (hi_s32)av;

    printf("\n car_mpu6050 [Mpu6050_ACC] aa = %lf, av = %lf \n", an, av);
}

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


