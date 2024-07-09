#include "car_config.h"
#include "car_motor.h"
#define LOG_I(fmt, args...)   printf("<%8ld> - [TIMER]:"fmt"\r\n",osKernelGetTickCount(),##args);
#define LOG_E(fmt, args...)   printf("<%8ld>-[TIMER_ERR]>>>>>>>>>>>>:"fmt"\r\n",osKernelGetTickCount(), ##args);
 
osTimerId_t g_timer1_id;
osTimerId_t g_timer2_id;

extern int bzw;

/***** 定时器1 回调函数 *****/
void Timer1_Callback(void *arg)
{
    if(bzw==1)
    {
        static uint8_t cnt = 0;
        cnt++;
        LOG_I("timer1 callback,cnt:%d,param:%s",cnt,arg);
        if(cnt%2==0)
        {
            printf("%c/n",cnt);
            printf("000000000000000000000000000");
            Motor_Forward();
    
        }
        else
        {
            printf("%c/n",cnt);
            printf("000000000000000000000000000");
            Motor_Backward();
          
        }
    }
}
 
/***** 定时器2 回调函数 *****/
void Timer2_Callback(void *arg)
{
    LOG_I("timer2 callback,param:%d",*(uint32_t *)arg);
 
    osTimerStart(g_timer1_id, 100);
    LOG_I("start timer1");
}
 
char timer1_param[] = "timer1 param";
uint32_t timer2_param = 1024;
 
void Hello_World(void)
{
    LOG_I("Test software Timer");
 
    uint32_t timerDelay;
    osStatus_t status;
 
/*timer 1*/
    g_timer1_id = osTimerNew(Timer1_Callback, osTimerPeriodic, timer1_param, NULL);
    if (g_timer1_id != NULL)
    {
        // Hi3861 1U=10ms,100U=1S
        timerDelay = 100U;
 
        status = osTimerStart(g_timer1_id, timerDelay);
        if (status != osOK)
        {
            LOG_E("timer1 start error");        
        }
        else
        {
            LOG_I("timer1 start success,cycle:%dms",timerDelay * 10);
        }
    }
    else
    {
        LOG_E("timer1 create fail!!!");
    }
 
/*timer 2*/
    g_timer2_id = osTimerNew(Timer2_Callback, osTimerOnce, (void *)&timer2_param, NULL);
    if (g_timer2_id != NULL)
    {
        // Hi3861 1U=10ms,100U=1S
        timerDelay = 500U;
 
        status = osTimerStart(g_timer2_id, timerDelay);
        if (status != osOK)
        {
            LOG_E("timer2 start error");
        }
        else
        {
            LOG_I("timer2 start success,cycle:%dms",timerDelay * 10);
        }
    }
    else
    {
        LOG_E("timer2 create fail!!!");
    }
}