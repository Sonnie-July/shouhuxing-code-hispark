#include "car_config.h"    

#include "car_motor.h"      
#include "car_wifi.h"       
#include "car_qmc5883.h"    
#include "car_mpu6050.h"   
//#include "car_hcsr04.h"    
#include "display_board.h"  


#define WifiName    "3246904448ab"  
#define WifiPass    "12345678"      

int a=0;
int bzw=0;
unsigned short port_num = 6666;    

hi_u8 addr_str[15] = {0};          
hi_u8 port_str[15] = {0};          
int ser_sock = -1;                 
int cli_sock = -1;                  

unsigned char send_buff[32] = {0};  
unsigned char recv_buff[32] = {0}; 
unsigned char sock_stat = 0;       

ssize_t sock_ret;                 



hi_u8 car_stat = 0;                 


hi_s32 hcsr1 = 0;
hi_s32 hcsr2 = 0;
hi_s32 hcsr3 = 0;

hi_u8 hcs_b = 0;


hi_u8 qmc_ds[6] = {0};
hi_s32 qmc_ang = 0;    

hi_u8 qmc_b = 0;


hi_u8 mpu_ds[14] = {0}; 
hi_u8 mpu_ln = 14;

hi_s32 mpu_acc[2] = {0}; 

hi_u8 mpu_b = 0;


hi_u8 l_v = 20;
hi_u8 r_v = 20;
hi_u16 fre = 4000;


hi_u8 sg_a = 2;    


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

    filter->Q_angle = 0.001;
    filter->Q_gyro = 0.003;
    filter->R_angle = 0.03;
    filter->angle = 0;
    filter->bias = 0;
    filter->P[0][0] = 0;
    filter->P[0][1] = 0;
    filter->P[1][0] = 0;
    filter->P[1][1] = 0;
	}

float update_KalmanFilter(KalmanFilter *filter, float newAngle, float newRate, float dt) {
    float K[2];
	  K[0] = filter->P[0][0] / (filter->P[0][0] + filter->R_angle)；
    K[1] = filter->P[1][0] / (filter->P[0][0] + filter->R_angle);

    
    float y = newAngle - filter->angle;
    filter->angle += K[0] * y;

   
    filter->bias += K[1] * y;

    float P00_temp = filter->P[0][0];
    float P01_temp = filter->P[0][1];

    filter->P[0][0] -= K[0] * P00_temp;
    filter->P[0][1] -= K[0] * P01_temp;
    filter->P[1][0] -= K[1] * P00_temp;
    filter->P[1][1] -= K[1] * P01_temp;
		
    
    float newAnglePredicted = filter->angle + dt * newRate;

   
    filter->P[0][0] += dt * (dt * filter->P[1][1] - filter->P[0][1] - filter->P[1][0] + filter->Q_angle);
    filter->P[0][1] -= dt * filter->P[1][1];
    filter->P[1][0] -= dt * filter->P[1][1];
    filter->P[1][1] += filter->Q_gyro * dt;

 
    return newAnglePredicted;
}


hi_void Hi3861_Gpio_Init(hi_void)
{
    hi_u32 ret;

    hi_io_set_func(HI_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_I2C0_SDA);
    hi_io_set_func(HI_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_I2C0_SCL);

    ret = hi_i2c_init(HI_I2C_IDX_0, 400000);

    if (ret != HI_ERR_SUCCESS)
    {
        printf(" [hi_i2c_init] Failed \n");
    }

    printf("\n [Hi3861_Gpio_Init] \n");
}




void Sock_Server(void)
{
    ser_sock = -1;                              
    struct sockaddr_in ser_addr = {0};          
    unsigned short ser_port = port_num;         

 
    ser_sock = lwip_socket(AF_INET, SOCK_STREAM, 0);

   
    ser_addr.sin_family = AF_INET;                     
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);   
    ser_addr.sin_port = htons(ser_port);                
    
    lwip_bind(ser_sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr));

   
    lwip_listen(ser_sock, 1);    
    printf("\n [Sock_Server] Server Socket = %d \n", ser_sock);
}


void Sock_Client(void)
{
    cli_sock = -1;                              
    struct sockaddr_in cli_addr = {0};          
    socklen_t cli_addr_len = sizeof(cli_addr);  

   
    cli_sock = lwip_accept(ser_sock, (struct sockaddr *)&cli_addr, &cli_addr_len);

    printf("\n [Sock_Client] Click Socker = %d \n", cli_sock);
}


void Sock_Close(void)
{
  
    if( cli_sock != -1 )
    {
        lwip_close(cli_sock);
        osDelay(20);     
        cli_sock = -1;
    }

   
    if( ser_sock != -1 )
    {
        lwip_close(ser_sock);
        osDelay(20);
        ser_sock = -1;
    }

    printf("\n [Sock_Close] Server Socket = %d, Click Socker = %d \n", ser_sock, cli_sock);
}


hi_void Sg90_Init(hi_void)
{
    hi_io_set_func(HI_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_GPIO);
    hi_gpio_set_dir(HI_GPIO_IDX_10, HI_GPIO_DIR_OUT);
    hi_gpio_set_ouput_val(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
}


hi_void Sg90_Ang(hi_u8 a)
{
    hi_u32 n1 = 0;
    hi_u32 n2 = 0;
    hi_s8 i = 20;

    if(sg_a == a || a > 3)
    {
        return;
    }

    switch(a)
    {
    case 1 :
        n1 = 1000;
        break;
    case 2 :
       n1 = 1500;
       break;
    case 3:
       n1 = 2000;
       break;
    default :
       n1 = 1500;
    }

    n2 = 20000 - n1;

    while(i > 0)
    {
        hi_gpio_set_ouput_val(HI_GPIO_IDX_10, HI_GPIO_VALUE1);
        hi_udelay(n1);
        hi_gpio_set_ouput_val(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
        hi_udelay(n2);
        i--;
    }

    hi_udelay(200000);  //200ms  

    sg_a = a;    

    printf("\n [Sg90_Ang] sg_a = %d, n1 = %d, n2 = %d \n", sg_a, n1, n2);
}


hi_void Car_Task_Init(hi_void)
{
   
    hi_watchdog_disable();

    Hi3861_Gpio_Init();

    OLED_Init();

   
    OLED_Clear();
   
    OLED_String(31, 1, "Car Init");
    OLED_String(31, 5, "Wait ...");
   
    OLED_Show();

 

   
    Qmc5883_Init();
    qmc_b = 0;

  

   
    Mpu6050_Init();
    mpu_b = 0;

    hi_gpio_init();

    Motor_Init();

    l_v = 20;
    r_v = 20;
    fre = 4000;

    Motor_Set_Frequency(fre);

    Motor_Set_Left_Velocity(3000 + l_v * 10);

    Motor_Set_Right_Velocity(3000 + r_v * 10);

    Motor_Stop(); 


    //Hcsr04_Init();

    //hcs_b = 0;

    // ----

    // 舵机
    Sg90_Init();

    // 方向 置中
    Sg90_Ang(2);

    // ----

    printf("\n [Car_Task_Init] \n"); 
}

float MPUTask()
{
    
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
    // while (MPU_Init())
    // {
    //     printf("MPU Initialize Faild \n");
    // }
    
    // while(1) //连接成功 led 一直闪烁
    // {   
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
        printf("%.1f\n",roll);
        printf("sksksk\n");
        return roll;
    // }
}
// ---------- ---------- ----------

static void *Car_Task(const char *arg)
{
    (void)arg;
  Hello_World();
    // 初始设置
    Car_Task_Init();

    // ----  网络 设置

    // Wifi 连接开启
    WiFi_Conn_Start(WifiName, WifiPass, addr_str);
 
    // 服务端server socket 套接字
    Sock_Server();  
    
    // 端口号 转 字符串
    snprintf(port_str, sizeof(port_str), "%d", port_num);

    // OLED 清缓存
    OLED_Clear();
    // 显示 IP 和 端口
    OLED_String(0, 0, addr_str);
    OLED_String(0, 2, port_str);
    OLED_String(23, 5, "Accept ...");
    // OLED 显示信息
    OLED_Show();

    // 客户端 client socket 套接字
    Sock_Client();

    // OLED 清缓存
    OLED_Clear();
    // 显示 状态
    OLED_String(35, 3, "Car Run");
    // OLED 显示信息
    OLED_Show();   

    Hi3861_Gpio_Init();
    while(1)
    {
        if(MPUTask()<=-0.4)
        {
            Motor_Forward();
            car_stat = 1;
            a=1;
        }
        // else if(MPUTask()>=-0.2&&MPUTask()<=0.2)
        // Motor_Stop();
        if(MPUTask()>=0.4)
        {
            Motor_Backward();
            car_stat = 2;
            a=2;
        }
        if(MPUTask()>=-0.4&&MPUTask()<=0.4&&a!=0)
        {
            Motor_Stop();
            car_stat = 0;
            a=0;
        }
        // else
        // Motor_Stop();

        // 客户端 重新连接
        if(sock_stat == 2)
        {
            // OLED 清缓存
            OLED_Clear();
            // 显示 IP 和 端口
            OLED_String(0, 0, addr_str);
            OLED_String(0, 2, port_str);
            OLED_String(23, 5, "Accept ...");
            // OLED 显示信息
            OLED_Show();

            // 客户端 client socket 套接字
            Sock_Client();

            // OLED 清缓存
            OLED_Clear();
            // 显示 状态
            OLED_String(35, 3, "Car Run");
            // OLED 显示信息
            OLED_Show(); 

            // 状态 值 恢复
            sock_stat = 0;
        }

        // ----------

        // WiFi接收数据
        sock_ret = lwip_recv(cli_sock, recv_buff, sizeof(recv_buff), MSG_DONTWAIT);

        printf("\n Recv Ret = %d \n", sock_ret);

        // 接收到信息
        if(sock_ret > 0)
        {
            printf("\n Recv : %s \n", recv_buff);

            // 比较信息

            if(strlen(recv_buff) == 4)  // 长度一样
            {
                // 左 加速
                if(memcmp(recv_buff, "ladd", 4) == HISI_OK)
                {
                    Motor_Stop(); 
                    car_stat = 0;
                    l_v = l_v + 1;
                    Motor_Set_Left_Velocity(3000 + l_v * 10); 
                    printf("\n [Motor_Set_Left_Velocity] LV : %d \n", l_v);        
                }   

                // 左 减速
                if(memcmp(recv_buff, "lsub", 4) == HISI_OK)
                {
                    Motor_Stop(); 
                    car_stat = 0;
                    l_v = l_v - 1;
                    Motor_Set_Left_Velocity(3000 + l_v * 10);  
                    printf("\n [Motor_Set_Left_Velocity] LV : %d \n", l_v);       
                }

                // 右 加速
                if(memcmp(recv_buff, "radd", 4) == HISI_OK)
                {
                    Motor_Stop(); 
                    car_stat = 0;
                    r_v = r_v + 1;
                    Motor_Set_Right_Velocity(3000 + r_v * 10);   
                    printf("\n [Motor_Set_Right_Velocity] RV : %d \n", r_v);       
                }

                // 右 减速
                if(memcmp(recv_buff, "rsub", 4) == HISI_OK)
                {
                    Motor_Stop(); 
                    car_stat = 0;
                    r_v = r_v - 1;
                    Motor_Set_Right_Velocity(3000 + r_v * 10);
                    printf("\n [Motor_Set_Right_Velocity] RV : %d \n", r_v);       
                }

                // 前进
                if(memcmp(recv_buff, "forw", 4) == HISI_OK)
                {
                    Motor_Forward();   
                    car_stat = 1;    
                    printf("\n [Motor_Forward] \n");           
                }

                // 后退
                if(memcmp(recv_buff, "back", 4) == HISI_OK)  // 
                {
                    Motor_Backward(); 
                    car_stat = 2; 
                    printf("\n [Motor_Backward] \n");                
                }

                // 左转
                if(memcmp(recv_buff, "left", 4) == HISI_OK)  // 
                {
                    Motor_Left();   
                    car_stat = 3;   
                    printf("\n [Motor_Left] \n");          
                }

                // 右转
                if(memcmp(recv_buff, "righ", 4) == HISI_OK)  // 
                {
                    Motor_Right();     
                    car_stat = 4;  
                    printf("\n [Motor_Right] \n");           
                }

                // 停止
                if(memcmp(recv_buff, "stop", 4) == HISI_OK)  // 
                {
                    Motor_Stop(); 
                    car_stat = 0;
                    bzw=0;
                    printf("\n [Motor_Stop] \n");               
                }

                // 超声波
                // if(memcmp(recv_buff, "hcsr", 4) == HISI_OK)  // 
                // {
                //     hcs_b = ~hcs_b;
                //     printf("\n [hcsr04] \n");               
                // }

                // 磁力计
                if(memcmp(recv_buff, "qmcb", 4) == HISI_OK)  // 
                {
                    bzw=1;
                }

                // 陀螺仪
                if(memcmp(recv_buff, "mpub", 4) == HISI_OK)  // 
                {
                    mpu_b = ~mpu_b;
                    printf("\n [mpu6050] \n");               
                }

                // 退出
                if(memcmp(recv_buff, "quit", 4) == HISI_OK)  // 
                {
                    sock_stat = 1; 
                    printf("\n Quit [While] \n");              
                }

            }

        }

        // 接收缓存清0 
        memset(recv_buff, 0, sizeof(recv_buff));

        // 状态 == 1 退出
        if(sock_stat == 1)
        {            
            sock_stat = 0;
            printf("\n Exit While \n");
            break;
        } 

        // 正常状态
        if(sock_stat == 0)
        {
            hi_udelay(200000);  //200ms  
        }

        // ***** ***** ***** ***** ***** 
        
        // 发送缓存清0 
        memset(send_buff, 0, sizeof(send_buff)); 

        // ----------

        // // 超声波

        // send_buff[0] = hcs_b;

        // if( hcs_b )
        // {           

        //     Sg90_Ang(1);

        //     hcsr1 = Hcsr04_Measure();            

        //     printf("\n [Hcsr04_Measure] hcsr1 = %d \n", hcsr1);

        //     send_buff[1] = hcsr1 & 0xff;
        //     hcsr1 = hcsr1 >> 8;
        //     send_buff[2] = hcsr1 & 0xff;

        //     Sg90_Ang(2);

        //     hcsr2 = Hcsr04_Measure();

        //     printf("\n [Hcsr04_Measure] hcsr2 = %d \n", hcsr2);

        //     send_buff[3] = hcsr2 & 0xff;
        //     hcsr2 = hcsr2 >> 8;
        //     send_buff[4] = hcsr2 & 0xff;

        //     Sg90_Ang(3);

        //     hcsr3 = Hcsr04_Measure();

        //     printf("\n [Hcsr04_Measure] hcsr3 = %d \n", hcsr3);

        //     send_buff[5] = hcsr3 & 0xff;
        //     hcsr3 = hcsr3 >> 8;
        //     send_buff[6] = hcsr3 & 0xff;

        //     Sg90_Ang(2);

        //     // if(hcsr1 < 200 || hcsr2 < 200 || hcsr3 < 200)
        //     // {
        //     //     Motor_Stop();
        //     //     car_stat = 0;
        //     // }

        // }

        // ----------

        // 磁力计

        send_buff[7] = qmc_b;

        if( qmc_b )
        {
            // 返回值结果
            if ( Qmc5883_Measure(qmc_ds) == HI_TRUE)
            {
                // qmc_ang = Qmc5883_Angle(qmc_ds);

                // printf("\n QMC Angle = %d \n", qmc_ang);

                send_buff[8] = qmc_ds[0];
                send_buff[9] = qmc_ds[1];
                send_buff[10] = qmc_ds[2];
                send_buff[11] = qmc_ds[3];
            }

            // if(car_stat == 1 && (qmc_ang > 0 || qmc_ang < 0))
            // {
            //     Motor_Stop();
            //     car_stat = 0;
            // }

        }


        // ---------

        // 陀螺仪

        send_buff[12] = mpu_b;

        if( mpu_b )
        {

            Mpu6050_Measure_By(mpu_ds, mpu_ln);

            // Mpu6050_ACC(mpu_ds, mpu_acc);

            // printf("\n mpu_aa = %d, mpu_av = %d \n", mpu_acc[0], mpu_acc[1]);

            send_buff[13] = mpu_ds[0];
            send_buff[14] = mpu_ds[1];
            send_buff[15] = mpu_ds[2];
            send_buff[16] = mpu_ds[3];

            // if(car_stat == 1 && mpu_av < 2)
            // {
            //     Motor_Stop();
            //     car_stat = 0;
            // }

        }

        // --------

        // 小车速度

        send_buff[17] = l_v;
        send_buff[18] = r_v;

        // 小车 控制状态
        send_buff[19] = car_stat;

        // --------

        // char sen_str[] = "Hello!";        
        // memcpy_s(send_buff, sizeof(send_buff), sen_str, strlen(sen_str));    // 缓存写入返回信息

        // WIFI发送数据
        sock_ret = lwip_send(cli_sock, send_buff, sizeof(send_buff), 0);  

        printf("\n Send Ret = %d \n", sock_ret);   

        // 发送未被接收
        if(sock_ret < 0)
        {
            printf("\n Client : Failed \n"); 
            if( cli_sock != -1 )
            {
                lwip_close(cli_sock);
                osDelay(20);  // 等待      
                cli_sock = -1;
            }
            sock_stat = 2;            
            continue;
        }

    }

    // 退出 断网

    // 关闭 sock
    Sock_Close();

    // 断开连接
    Wifi_Conn_Stop();

    // OLED 清缓存
    OLED_Clear();
    // 显示 状态
    OLED_String(35, 3, "Car End");
    // OLED 显示信息
    OLED_Show();  

    return NULL;
}


// 应用入口
static void Car_Entry(void)
{
    osThreadAttr_t attr;

    attr.name = "Car_Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024 * 10;
    attr.priority = 25;

    if(osThreadNew((osThreadFunc_t)Car_Task, NULL, &attr) == NULL)
    {
        printf(" [Car_Entry] Falied to create Car_Task! \n");
    }
}


SYS_RUN(Car_Entry);




