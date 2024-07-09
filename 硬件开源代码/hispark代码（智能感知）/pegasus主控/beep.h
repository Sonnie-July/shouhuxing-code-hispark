#ifndef __BEEP__H
#define __BEEP__H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"

#define Beep_GPIO 5
static void Beep_Init()
{
    IoTGpioInit(Beep_GPIO);
    //将GPIO10设置普通IO
    IoTGpioSetDir(Beep_GPIO,IOT_GPIO_DIR_OUT);//将GPIO端口设置为输出模式
    IoTGpioSetOutputVal(Beep_GPIO,IOT_GPIO_VALUE0);//将总线拉低
}
static void Beep_Alarm()
{
    IoTGpioSetOutputVal(Beep_GPIO,IOT_GPIO_VALUE1);//将总线拉高
    usleep(200000);//延时2000ms
    IoTGpioSetOutputVal(Beep_GPIO,IOT_GPIO_VALUE0);//将总线拉高
    usleep(200000);//延时2000ms
    IoTGpioSetOutputVal(Beep_GPIO,IOT_GPIO_VALUE1);//将总线拉高
    usleep(200000);//延时2000ms
    IoTGpioSetOutputVal(Beep_GPIO,IOT_GPIO_VALUE0);//将总线拉高
    usleep(200000);//延时2000ms
}
#endif