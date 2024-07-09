


/**
 * 
 * I2C
 * 
 * :\device\hisilicon\hispark_pegasus\sdk_liteos\build\config\usr_config.mk
 * 
 * # CONFIG_I2C_SUPPORT is not set
 * CONFIG_I2C_SUPPORT=y
 * 
 * 
 * qmc5883 三轴磁传感器 (上海矽睿科技有限公司)
 * 
 * qmc5883l的供电范围是2.16~3.6V
 * 
 * qmc5883默认的7-bit IIC地址为0X0D：0001101
 * qmc5883l有一个S1引脚，
 * 如果我们将S1接VDD，那么写地址为0X1A，读地址为0X1B（通常接VDD）；
 * 如果我们将S1接GND，那么写地址为0X18，读地址为0X19；
 * 
 * IIC 地址 : 0x0D 0001 101
 * 
 * 寄存器地址
 * 00H : x out [7:0]
 * 01H : x out [15:8]
 * 
 * 02H : y out [7:0]
 * 03H : y out [15:8]
 * 
 * 04H : z out [7:0]
 * 05H : z out [15:8]
 * 
 * 2个寄存器完成存储1个数据 低8位在前 高8位在后 -32768 ~ 32768
 * 
 * 06H : [2] DOR , [1] OVL , [0] DRDY ,
 * 
 * 状态位 测量完毕 DRDY = 1 被读取 DRDY = 0
 *       测量超范围 OVL = 1 没超范围 OVL = 0
 *       连续测量模式下 DOR = 1 当数据被读取后 DOR = 0
 * 
 * 07H : out [7:0]
 * 08H : out [15:8]
 * 
 * 温度值
 * 
 * 09H : [76] OSR , [54] RNG , [32] ODR , [10] MODE
 * 
 * 设置芯片
 * 
 * MODE : 工作模式 00 待机模式 , 01 连续测量 , 10 null 11 null
 * ODR : 数据输出速率 00 10Hz , 01 50Hz , 10 100Hz , 11 200Hz
 * RNG : 00 2G , 01 8G , 10 null , 11 null
 * OSR : 采样率 00 512 , 01 256 , 10 128 , 11 64
 * 
 * 0AH :    7 SOFT_RST = 1 软复位 设置为默认值
 *          6 ROL_PNT = 0 I2C 不翻转 = 1 自动翻转 
 *          0 INT_ENB = 0 DRDY中断使能 = 1 中断被禁止
 *  
 * OBH : 复位时间 推荐 0x01
 * 
 * 0CH : null
 * 
 * 0DH : ID
 * 
 * 通过读取ID 检验能否连到芯片 返回值 0xff
 * 
 * 
 * 正常读取数据
 * 
 * 1. 读取 如果 DRDY = 1 读取数据 如果 DRDY = 0 等待
 * 
 * 2. 读取完毕 DOR = 1 如果读取中断 DOR = 0
 * 
 * 
 **/ 


#include "car_qmc5883.h"

// 直采 字节数据
// hi_u8 qmc_ds[6] = {0};

// 3861接入端口设置 i2c设置
// hi_void Hi3861_Gpio_Init(hi_void)
// {
//     hi_u32 ret;

//     hi_io_set_func(HI_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_I2C0_SDA);
//     hi_io_set_func(HI_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_I2C0_SCL);

//     ret = hi_i2c_init(HI_I2C_IDX_0, 400000);

//     if (ret != HI_ERR_SUCCESS)
//     {
//         printf(" [hi_i2c_init] Error! \n");
//         return;
//     }
// }


// qmc5883 写入 指令
static hi_void Qmc5883_I2c_Write(hi_u8 addr, hi_u8 data)
{
    hi_u8 data_send_buf[] = {addr, data};

    hi_u32 status = 0;

    hi_i2c_data qmc5883_i2c_data = {0};

    qmc5883_i2c_data.send_buf = data_send_buf;
    qmc5883_i2c_data.send_len = 2;

    status = hi_i2c_write(HI_I2C_IDX_0, 0x1A, &qmc5883_i2c_data);

    if(status != HI_ERR_SUCCESS)
    {
        printf(" [Qmc5883_I2c_Write] Error! \n");
        return;
    }
}

// qmc5883 读取 状态
static hi_void Qmc5883_I2c_Read(hi_u8 addr, hi_u8* data_receive_buf, hi_u32 data_receive_len)
{
    hi_u8 data_send_buf[] = { addr };

    hi_u32 status = 0;

    hi_i2c_data qmc5883_i2c_data = {0};

    qmc5883_i2c_data.send_buf = data_send_buf;
    qmc5883_i2c_data.send_len = 1;

    status = hi_i2c_write(HI_I2C_IDX_0, 0x1A, &qmc5883_i2c_data);

    if(status != HI_ERR_SUCCESS)
    {
        printf(" [Qmc5883_I2c_Read] Write Error! \n");
        return;
    }

    qmc5883_i2c_data.receive_buf = data_receive_buf;
    qmc5883_i2c_data.receive_len = data_receive_len;

    status = hi_i2c_read(HI_I2C_IDX_0, 0x1B, &qmc5883_i2c_data);

    if(status != HI_ERR_SUCCESS)
    {
        printf(" [Qmc5883_I2c_Read] Read Error! \n");
        return;
    }
}

// Qmc5883初始设置
hi_void Qmc5883_Init(hi_void)
{
    Qmc5883_I2c_Write(0x0A, 0x80);  // 恢复默认值 1000 000

    hi_udelay(50000);   // 等待50ms

    Qmc5883_I2c_Write(0x0A, 0x01);  // 不反转 中断禁止 0000 0001

    Qmc5883_I2c_Write(0x0B, 0x01);  // 设置复位时间 推荐值 0x01

    Qmc5883_I2c_Write(0x09, 0x01);  // 设置测量参数 00 00 00 01  512 2G 10Hz 连续测量 

    hi_udelay(50000);   // 等待50ms

    printf("\n car_qmc5883 [Qmc5883_Init] \n");
}

// 返回值结果
hi_bool Qmc5883_Measure(hi_u8 *datas)
{

    hi_bool boo = HI_FALSE;

    // 读取数据
    Qmc5883_I2c_Read(0x06, datas, 1);

    if(datas[0] && 0x01)    // 可测
    {
        // 将数值转成 字符
        memset(datas, 0, sizeof(datas));    // 缓存清0
        Qmc5883_I2c_Read(0x00, datas, 6);
        boo = HI_TRUE;
    }

    printf("\n car_qmc5883 [Qmc5883_Measure] \n");

    return boo;

}

// 输出 XY 角度
hi_s32 Qmc5883_Angle(hi_u8 *dats)
{

    if(dats == NULL)
    {
        return;
    }

    hi_s32 ang1 = 0;
    double ang2 = 0.0;

    short x1 = (short)dats[1] << 8 | dats[0];
    short y1 = (short)dats[3] << 8 | dats[2];

    printf("\n car_qmc5883 [Qmc5883_Angle] x1 = %d, y1 = %d \n", x1, y1);
    
    int x2 = abs(x1);
    int y2 = abs(y1);

    printf("\n car_qmc5883 [Qmc5883_Angle] x2 = %d, y2 = %d \n", x2, y2);

    // 反正切函数 求弧度
    //ang2 = atan( (double)y2 / (double)x2);
    
    // 可以忽略除0异常 但是 跟正反切结果 是相反的
    double a1 = acos( x2 / (sqrt( x2 * x2 + y2 * y2)) );

    // 弧度转角度
    ang2 = a1 * 180 / 3.14159;

    printf("\n car_qmc5883 [Qmc5883_Angle] ang2 = %lf \n", ang2);

    // 第一象限
    // if (x1 >= 0 && y1 >= 0)
    // {
    //     // ang2 = ang2;
    // }

    // 第二象限
    if (x1 < 0 && y1 >= 0)
    {
        ang2 = 180 - ang2;
    }

    // 第三象限
    if (x1 < 0 && y1 < 0)
    {
        ang2 = 180 + ang2;
    }

    // 第四象限
    if (x1 >= 0 && y1 < 0)
    {
        ang2 = 360 - ang2;
    }

    ang1 = (hi_s32)ang2;

    printf("\n car_qmc5883 [Qmc5883_Angle] ang1 = %lf \n", ang2);

    return ang1;
    
}
