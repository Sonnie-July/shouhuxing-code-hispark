
/**
 * 
 * car_motor.c
 * 
 * MX1919双电机驱动模块
 * 
 *   
 * ● 2-6 节 AA/AAA 干电池供电的玩具马达驱动
 * ● 2-6 节镍-氢/镍-镉充电电池供电的玩具马达驱动
 * ● 1-2 节锂电池供电的马达驱动
 * 
 * In1:  gpio1 pwm4
 * In2:  gpio2 pwm2
 * 
 * In3:  gpio6 pwm3
 * In4:  gpio8 pwm1
 * 
 * motor-a 正转（调速） in1 = 1/pwm in2 = 0
 *         反转（调速） in1 = 0     in2 = 1/pwm
 *         待机         in1 = 0     in2 = 0
 *         刹车         in1 = 1     in2 = 1
 * 
 * motor-b 正转（调速） in3 = 1/pwm in4 = 0
 *         反转（调速） in3 = 0     in4 = 1/pwm
 *         待机         in3 = 0     in4 = 0 
 *         刹车         in3 = 1     in4 = 1
 * 
 * 
 **/

#include "car_motor.h"

// pwm 周期
hi_u16 frequency = 0;

// pwm 脉宽 越大 转速越快
hi_u16 left_velocity = 0;   // 左电机
hi_u16 right_velocity = 0;  // 右电机

// 电机控制板初始化
hi_void Motor_Init(hi_void)
{
    // 配置某个IO的复用功能

    // Motor-A 左
    hi_io_set_func(HI_IO_NAME_GPIO_1, HI_IO_FUNC_GPIO_1_PWM4_OUT);
    hi_io_set_func(HI_IO_NAME_GPIO_2, HI_IO_FUNC_GPIO_2_PWM2_OUT);

    // Motor-B 右
    hi_io_set_func(HI_IO_NAME_GPIO_6, HI_IO_FUNC_GPIO_6_PWM3_OUT);
    hi_io_set_func(HI_IO_NAME_GPIO_8, HI_IO_FUNC_GPIO_8_PWM1_OUT);

    // PWM初始化

    // ma 左
    hi_pwm_init(HI_PWM_PORT_PWM4);
    hi_pwm_init(HI_PWM_PORT_PWM2);

    // mb 右
    hi_pwm_init(HI_PWM_PORT_PWM3);
    hi_pwm_init(HI_PWM_PORT_PWM1);   

    // 设置PWM模块时钟类型
    hi_pwm_set_clock(PWM_CLK_XTAL); // 可能是 40MHz 

    printf("\n car_motor [Motor_Init] \n"); 
}


// 运行
hi_void Motor_Run(hi_u16 in1_duty, hi_u16 in2_duty, hi_u16 in3_duty, hi_u16 in4_duty)
{
    hi_pwm_stop(HI_PWM_PORT_PWM4);
    hi_pwm_stop(HI_PWM_PORT_PWM2);

    hi_pwm_stop(HI_PWM_PORT_PWM3);
    hi_pwm_stop(HI_PWM_PORT_PWM1);

    hi_pwm_start(HI_PWM_PORT_PWM4, in1_duty, frequency);
    hi_pwm_start(HI_PWM_PORT_PWM2, in2_duty, frequency);

    hi_pwm_start(HI_PWM_PORT_PWM3, in3_duty, frequency);
    hi_pwm_start(HI_PWM_PORT_PWM1, in4_duty, frequency);

    printf("\n car_motor [Motor_Run] \n"); 
}

// 撤销
hi_void Motor_Deinit(hi_void)
{
    hi_pwm_stop(HI_PWM_PORT_PWM4);
    hi_pwm_stop(HI_PWM_PORT_PWM2);

    hi_pwm_stop(HI_PWM_PORT_PWM3);
    hi_pwm_stop(HI_PWM_PORT_PWM1);

    hi_pwm_deinit(HI_PWM_PORT_PWM4);
    hi_pwm_deinit(HI_PWM_PORT_PWM2);

    hi_pwm_deinit(HI_PWM_PORT_PWM3);
    hi_pwm_deinit(HI_PWM_PORT_PWM1);

    printf("\n car_motor [Motor_Deinit] \n"); 
}


// 前进
hi_void Motor_Forward(hi_void)
{    
    Motor_Run(left_velocity, 0, right_velocity, 0);
    printf("\n car_motor [Motor_Forward] \n"); 
}

// 后退
hi_void Motor_Backward(hi_void)
{
    Motor_Run(0, left_velocity, 0, right_velocity);
    printf("\n car_motor [Motor_Backward] \n"); 
}

// 原地 左转
hi_void Motor_Left(hi_void)
{
    Motor_Run(0, left_velocity, right_velocity, 0);
    printf("\n car_motor [Motor_Left] \n");
}

// 原地 右转
hi_void Motor_Right(hi_void)
{
    Motor_Run(left_velocity, 0, 0, right_velocity);
    printf("\n car_motor [Motor_Right] \n");
}

// 刹车 制动抱死
hi_void Motor_Stop(hi_void)
{
    Motor_Run(frequency, frequency, frequency, frequency);
    printf("\n car_motor [Motor_Stop] \n");
}

// 等待 空档
hi_void Motor_Wait(hi_void)
{
    Motor_Run(0, 0, 0, 0);
    printf("\n car_motor [Motor_Run] \n");
}

// 设置 左马达 速度
hi_void Motor_Set_Left_Velocity(hi_u16 vel)
{
    left_velocity = vel;
    printf("\n car_motor [Motor_Set_Left_Velocity] \n");
}

// 设置 右马达 速度
hi_void Motor_Set_Right_Velocity(hi_u16 vel)
{
    right_velocity = vel;
    printf("\n car_motor [Motor_Set_Right_Velocity] \n");
}

// 设置倍频
hi_void Motor_Set_Frequency(hi_u16 fre)
{
    frequency = fre;
    printf("\n car_motor [Motor_Set_Frequency] \n");
}






