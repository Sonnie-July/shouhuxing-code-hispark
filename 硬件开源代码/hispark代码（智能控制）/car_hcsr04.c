
/**
 * 
 * car_hcsr04.c
 * 
 * HC-SR04 超声波 测距模块
 * 
 * Trig GPIO12  控制端
 * Echo GPIO7   接收端
 * 
 * 主要的问题就是时间采样，采的时间不对，就会差的很大。
 * 
 * 
 * 
 * 
 **/ 

#include "car_hcsr04.h"


// 读取 系统时间 微秒 这个系统时间独立的 不被其他进程影响
static time_t sys_time_us(void) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    time_t time_us = current_time.tv_sec * 1000000 + current_time.tv_usec;
    return time_us;
}


// 初始设置 管脚设置
hi_void Hcsr04_Init(hi_void)
{
    // GPIO模块初始化
    // hi_gpio_init();

    // 配置某个IO的复用功能
    hi_io_set_func(HI_IO_NAME_GPIO_12, HI_IO_FUNC_GPIO_12_GPIO);    // Trig 控制端
    hi_io_set_func(HI_IO_NAME_GPIO_7, HI_IO_FUNC_GPIO_7_GPIO);      // Echo 接收端

    // 设置某个GPIO管脚方向
    hi_gpio_set_dir(HI_GPIO_IDX_12, HI_GPIO_DIR_OUT);   // Trig 输出 触发信号
    hi_gpio_set_dir(HI_GPIO_IDX_7, HI_GPIO_DIR_IN);     // Echo 输入 接收反馈

    // 设置单个GPIO管脚输出电平状态
    hi_gpio_set_ouput_val(HI_GPIO_IDX_12, HI_GPIO_VALUE0);      // 低电平
    // hi_gpio_set_ouput_val(HI_GPIO_IDX_7, HI_GPIO_VALUE0);    // 直接设置为低电平就可以的，就是觉得，如果有个拉力高电平状态返回会更有效一些，也许是想多了
    hi_io_set_pull(HI_IO_NAME_GPIO_7, HI_IO_PULL_DOWN);         // 下拉

    printf("\n car_hcsr04 [Hcsr04_Init] \n");
}


// 测量距离 返回单位 毫米
hi_s32 Hcsr04_Measure(hi_void)
{   
    hi_s32 distance = 0;    // 距离

    time_t start_time = 0;  // 开始时间
    time_t end_time = 0;    // 结束时间

    // 触发 测量
    hi_gpio_set_ouput_val(HI_GPIO_IDX_12, HI_GPIO_VALUE1);
    hi_udelay(10);   // 等待10us
    hi_gpio_set_ouput_val(HI_GPIO_IDX_12, HI_GPIO_VALUE0);

    hi_gpio_value echo_value = HI_GPIO_VALUE0;     // Echo 端口状态

    // 等待 接收端 高电平
    while(echo_value == HI_GPIO_VALUE0)
    {
        hi_gpio_get_input_val(HI_GPIO_IDX_7, &echo_value);
    }

    // 如果不是低电平了 开始计时
    start_time = sys_time_us(); // 调取系统时间 

    // 等待 接收端 高电平 结束
    while(echo_value == HI_GPIO_VALUE1)
    {
        hi_gpio_get_input_val(HI_GPIO_IDX_7, &echo_value);
    }

    // 记录结束后的系统时间
    end_time = sys_time_us();   // 结束时间

    // 声速在空气中大概为340米／秒 = 340 * 1000 mm / 1000 * 1000 us

    // 计算距离
    distance = ( end_time - start_time ) * 340 / 1000 / 2;    // 距离 毫米  

    // printf(" [ Hcsr04_Measure ] distance = %d \n", distance);

    printf("\n car_hcsr04 [Hcsr04_Measure] distance = %d \n", distance);

    return distance;
}