/**
 * 
 *    采集MPU 6050 原始数据 
 *处理通过串口发出，使用python脚本解析后执行相应动作
 * Code By： HelloKun  2021.09.06
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include <math.h>
#include "cmsis_os2.h"
#include "iot_errno.h" //==IOT_SUCCESS =0  

#include "hi_io.h"   //上拉、复用
#include "hi_gpio.h" //hi_gpio_set_dir()、hi_gpio_set(get)_output(input)_val()
#include "iot_gpio.h"//gpioInit
#include "hi_time.h"
#include "hi_i2c.h"
#include "mpu6050.h"

//#include "uart_control.h"

//#include "ssd1306_oled.h"
#define PI 3.1415926535


typedef struct {
    float Q_angle, Q_gyro; 
    float R_angle;         
    float angle;
    float bias;
    float P[2][2];         
} KalmanFilter;

double  atan2_custom(double  y,  double  x)  {
     double  result;
     if  (x  ==  0)  {
         if  (y  >  0)  {
             result  =  PI  /  2;
         }  else  if  (y  <  0)  {
             result  =  -PI  /  2;
         }  else  {
             result  =  0;
         }
     }  else  if  (y  ==  0)  {
         if  (x  >  0)  {
             result  =  0;
         }  else  if  (x  <  0)  {
             result  =  PI;
         }  else  {
             result  =  -PI;
         }
     }  else  {
         double  denominator  =  sqrt(x  *  x  +  y  *  y);
         if  (x  *  x  >=  y  *  y)  {
             result  =  atan(y  /  x);
         }  else  {
             result  =  PI  -  atan(y  /  x);
         }
     }
     return  result;
}


void init_KalmanFilter(KalmanFilter *filter) {

    filter->Q_angle = 0.001;//过程噪声协方差
    filter->Q_gyro = 0.003;//陀螺仪测量的过程噪声协方差
    filter->R_angle = 0.03;//加速度计测量的过程噪声协方差
    filter->angle = 0;//卡尔曼滤波器的状态变量，姿态估计的初始角度的估计值
    filter->bias = 0;//卡尔曼滤波器的状态变量，估计系统状态的偏差
    filter->P[0][0] = 0;
    filter->P[0][1] = 0;
    filter->P[1][0] = 0;
    filter->P[1][1] = 0;//误差协方差矩阵
	////处理噪声协方差//测量噪声协方差//角度估计//估计误差偏差误差协方差矩阵
}

float update_KalmanFilter(KalmanFilter *filter, float newAngle, float newRate, float dt) {
    float K[2];
	  K[0] = filter->P[0][0] / (filter->P[0][0] + filter->R_angle);//更新卡尔曼滤波器
    K[1] = filter->P[1][0] / (filter->P[0][0] + filter->R_angle);//卡尔曼增益系数

    // 更新角度估计
    float y = newAngle - filter->angle;
    filter->angle += K[0] * y;

    // 更新偏差估计
    filter->bias += K[1] * y;

    // 更新误差协方差矩阵
    float P00_temp = filter->P[0][0];
    float P01_temp = filter->P[0][1];

    filter->P[0][0] -= K[0] * P00_temp;
    filter->P[0][1] -= K[0] * P01_temp;
    filter->P[1][0] -= K[1] * P00_temp;
    filter->P[1][1] -= K[1] * P01_temp;
		
    // 计算新的传感器读数预测
    float newAnglePredicted = filter->angle + dt * newRate;

    //更新误差协方差矩阵预测
    filter->P[0][0] += dt * (dt * filter->P[1][1] - filter->P[0][1] - filter->P[1][0] + filter->Q_angle);
    filter->P[0][1] -= dt * filter->P[1][1];
    filter->P[1][0] -= dt * filter->P[1][1];
    filter->P[1][1] += filter->Q_gyro * dt;

    //返回更新后的角度估计
    return newAnglePredicted;
}





static void MPUTask(void *arg)
{
    (void) arg;
    short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	uint8_t temp;					//温度	    

    // IoTGpioInit(0);
    // IoTGpioInit(1);
    // OledInit();

    // IoTGpioInit(MPU_SDA_IO10);
    // IoTGpioInit(MPU_SCL_IO9);
    // hi_io_set_func(MPU_SDA_IO10, HI_IO_FUNC_GPIO_10_I2C0_SDA);
    // hi_io_set_func(MPU_SCL_IO9, HI_IO_FUNC_GPIO_9_I2C0_SCL);
    // hi_i2c_init(0, MPU_I2C_BAUDRATE);

    // IoTGpioInit(9);
    // IoTGpioSetDir(9,IOT_GPIO_DIR_OUT);         //==载板led初始化

    //IoTGpioSetOutputVal(9,0);// 只要一直初始化不成功，则灯不闪
    MPU_Init(); //返回0 成功
    KalmanFilter filter;
    init_KalmanFilter(&filter);
     typedef struct 
	{
    float Q_angle, Q_gyro; 
    float R_angle;         
    float angle;           
    float bias;             
    float P[2][2];         
  
	} KalmanFilter;
    while (MPU_Init())
    {
        printf("MPU Initialize Faild \n");
    }
    
    while(1) //连接成功 led 一直闪烁
    {   
        static char text[128] = {0};
       // temp = MPU_Get_Temperature();
        MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
        MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
        
        //printf("temp: %d\n",temp);
        // printf("gyrox: %d\n",gyrox);
        // printf("gyroy: %d\n",gyroy);
        // printf("gyroz: %d\n",gyroz); 
        //printf("%d:%d:%d\n",aacz,aacy,aacz);
        aacz=aacz-1300;
        aacy=aacy+2210;
        gyrox=gyrox-190;
       // float roll =atan2_custom(aacz, aacy)*180/PI;
        double roll =atan2_custom(aacy,aacz);
        float dt = 0.06;
        float filteredAngle = update_KalmanFilter(&filter, roll, gyrox, dt);
        printf("%.1f\n",filteredAngle);
        printf("sksksk\n");
        //printf("aacz: %d\n",aacz);

         snprintf(text, sizeof(text), "%f", roll);
         OledShowStr(32, 8, \
                 text, 1); /* 0, 2, xx, 1 */
        //OledShowString(0,1,(char)(temp),1);

        // IoTGpioSetOutputVal(9,0); 
        //car_backward();
        // usleep(30000);  
        // IoTGpioSetOutputVal(9,1); -1557750047.000000
        usleep(30000); 
        //Print_Original_MPU_Data();
        usleep(10000);  
    }
}

static void MPUDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "MPUTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;

    if (osThreadNew(MPUTask, NULL, &attr) == NULL) {
        printf("[MPUDemo] Falied to create MPUTask!\n");
    }
}
SYS_RUN(MPUDemo);


