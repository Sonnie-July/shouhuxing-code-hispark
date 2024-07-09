/*
 * Copyright (C) 2023 HiHope Open Source Organization .
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "iot_uart.h"
#include "hi_uart.h"
#include "iot_gpio_ex.h"

#define LED_TASK_GPIO    9
#define IOT_GPIO_KEY     5
#define DELAY_TICKS_30  (30)
#define LED_TASK_STACK_SIZE 512
#define LED_TASK_PRIO   (25)
#define GPIO_HIGH       (1)
#define GPIO_LOW        (0)

enum LedState {
    LED_ON = 0,
    LED_OFF,
    LED_SPARK,
};

#define UART_BUFF_SIZE 150
#define U_SLEEP_TIME 1000000
#define IOT_SUCCESS    0
uint32_t len = 0;
enum LedState g_ledState = LED_SPARK;
int g_count = 100;
const char* tel ="ATD15637272476;\r\n";//拨打电话
char uartReadBuff[UART_BUFF_SIZE] = {0};

void Uart1GpioInit(void)
{
    IoTGpioInit(IOT_IO_NAME_GPIO_0);
    // 设置GPIO0的管脚复用关系为UART1_TX Set the pin reuse relationship of GPIO0 to UART1_ TX
    IoSetFunc(IOT_IO_NAME_GPIO_0, IOT_IO_FUNC_GPIO_0_UART1_TXD);
    IoTGpioInit(IOT_IO_NAME_GPIO_1);
    // 设置GPIO1的管脚复用关系为UART1_RX Set the pin reuse relationship of GPIO1 to UART1_ RX
    IoSetFunc(IOT_IO_NAME_GPIO_1, IOT_IO_FUNC_GPIO_1_UART1_RXD );
}

void Uart1Config(void)
{
    uint32_t ret;
    /* 初始化UART配置，波特率 9600，数据bit为8,停止位1，奇偶校验为NONE */
    /* Initialize UART configuration, baud rate is 9600, data bit is 8, stop bit is 1, parity is NONE */
    IotUartAttribute uart_attr = {
        .baudRate = 9600,
        .dataBits = 8,
        .stopBits = 1,
        .parity = 0,
    };
    ret = IoTUartInit(HI_UART_IDX_1, &uart_attr);
    if (ret != IOT_SUCCESS)
    {
        printf("Init Uart1 Falied Error No : %d\n", ret);
        return;
    }
}

char sim_call(void)
{
    memset(uartReadBuff, 0, sizeof(uartReadBuff));
    IoTUartWrite(HI_UART_IDX_1, (unsigned char *)tel, strlen(tel));
    //hi_udelay(1000000);
    if (strcmp(uartReadBuff, "OK"))
        return 1;
}
void *LedTask(void)
{
   
    //对UART1的一些初始化 Some initialization of UART1
    Uart1GpioInit();
    // 对UART1参数的一些配置 Some configurations of UART1 parameters
    Uart1Config();
    // if(g_ledState==LED_ON)
    // sim_call();
    while (g_count) {
        switch (g_ledState) {
            case LED_ON:
                IoTGpioSetOutputVal(LED_TASK_GPIO, GPIO_LOW);
                osDelay(DELAY_TICKS_30);
                sim_call();
                break;
            case LED_OFF:
                IoTGpioSetOutputVal(LED_TASK_GPIO, GPIO_HIGH);
                osDelay(DELAY_TICKS_30);
                break;
            case LED_SPARK:
                IoTGpioSetOutputVal(LED_TASK_GPIO, GPIO_LOW);
                osDelay(DELAY_TICKS_30);
                IoTGpioSetOutputVal(LED_TASK_GPIO, GPIO_HIGH);
                osDelay(DELAY_TICKS_30);
                break;
            default:
                osDelay(DELAY_TICKS_30);
                break;
        }
    }

    return NULL;
}

void OnButtonPressed(void)
{
    enum LedState nextState = LED_SPARK;
    switch (g_ledState) {
        case LED_ON:
            nextState = LED_OFF;
            break;
        case LED_OFF:
            nextState = LED_ON;
            break;
        case LED_SPARK:
            nextState = LED_OFF;
            break;
        default:
            break;
    }

    g_ledState = nextState;
    g_count--;
}

void LedExampleEntry(void)
{
    osThreadAttr_t attr;

    IoTGpioInit(LED_TASK_GPIO);
    IoTGpioSetDir(LED_TASK_GPIO, IOT_GPIO_DIR_OUT);
 
    IoTGpioInit(IOT_GPIO_KEY);
    hi_io_set_func(IOT_GPIO_KEY, GPIO_LOW);
    IoTGpioSetDir(IOT_GPIO_KEY, IOT_GPIO_DIR_IN);
    hi_io_set_pull(IOT_GPIO_KEY, GPIO_HIGH);

    IoTGpioRegisterIsrFunc(IOT_GPIO_KEY, IOT_INT_TYPE_EDGE,
        IOT_GPIO_EDGE_FALL_LEVEL_LOW,
        OnButtonPressed, NULL);

    attr.name = "LedTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TASK_STACK_SIZE;
    attr.priority = LED_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)LedTask, NULL, &attr) == NULL) {
        printf("[LedExample] Falied to create LedTask!\n");
    }
}
SYS_RUN(LedExampleEntry);