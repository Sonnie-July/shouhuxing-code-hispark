#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "hi_types.h"
#include "hi_time.h"
#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_uart.h"
#include "hi_uart.h"
#include "iot_watchdog.h"
#include "iot_errno.h"
#include "sim900a.h"
//#include "app_demo_iot.h"
//#include "app_demo_aht20.h"

int aaa=30;
char sim_call(void)
{
    memset(uartReadBuff, 0, sizeof(uartReadBuff));
    IoTUartWrite(HI_UART_IDX_1, (unsigned char *)tel, strlen(tel));
    //hi_udelay(1000000);
    if (strcmp(uartReadBuff, "OK"))
        return 1;
}
// char sim_send_message(void)
// {
//     memset(uartReadBuff, 0, sizeof(uartReadBuff));
//     IoTUartWrite(HI_UART_IDX_1, (unsigned char *)Mode, strlen(Mode));
//     hi_udelay(1000000);
//     // if (strcmp(uartReadBuff, "OK"))
//     //     return 1;

//     memset(uartReadBuff, 0, sizeof(uartReadBuff));
//     IoTUartWrite(HI_UART_IDX_1, (unsigned char *)GSM, strlen(GSM));
//     hi_udelay(1000000);
//     // if (strcmp(uartReadBuff, "OK"))
//     //     return 2;

//     memset(uartReadBuff, 0, sizeof(uartReadBuff));
//     IoTUartWrite(HI_UART_IDX_1, (unsigned char *)Gettel, strlen(Gettel));
//     hi_udelay(1000000);
//     // if (strcmp(uartReadBuff, "OK"))
//     //     return 3;

//     memset(uartReadBuff, 0, sizeof(uartReadBuff));
//     IoTUartWrite(HI_UART_IDX_1, (unsigned char *)Modex, strlen(Modex));
//     hi_udelay(1000000);
//     // if (strcmp(uartReadBuff, "OK"))
//     //     return 4;

//     memset(uartReadBuff, 0, sizeof(uartReadBuff));
//     IoTUartWrite(HI_UART_IDX_1, (unsigned char *)Pho, strlen(Pho));
//     hi_udelay(1000000);
//     // if (strcmp(uartReadBuff, "OK"))
//     //     return 5;

//     memset(uartReadBuff, 0, sizeof(uartReadBuff));
//     IoTUartWrite(HI_UART_IDX_1, (unsigned char *)Mess, strlen(Mess));
//     hi_udelay(1000000);
//     // if (strcmp(uartReadBuff, "OK"))
//     //     return 5;

//     // 发送Ctrl+Z命令,完成短信发送
//     IoTUartWrite(HI_UART_IDX_1, ctrl_z_command, strlen(ctrl_z_command));
//     hi_udelay(1000000);
//     hi_udelay(1000000);
//     hi_udelay(1000000);
//     hi_udelay(1000000);
// }
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
// static void UartTask(void)
// {

//     // 对UART1的一些初始化 Some initialization of UART1
//     Uart1GpioInit();
//     // 对UART1参数的一些配置 Some configurations of UART1 parameters
//     Uart1Config();
//     uint32_t count = 0;
//     while (1)
//     {
//         if (aaa>28)
//         {
//             //sim_call();
//             printf("222222");
//             //sim_send_message();
//             // len = IoTUartRead(HI_UART_IDX_1, uartReadBuff, UART_BUFF_SIZE);
//             // if (len > 0)
//             // {
//             //     // 把接收到的数据打印出来 Print the received data
//             //     printf("%s\r\n", uartReadBuff);
//             // }
//             memset(uartReadBuff, 0, sizeof(uartReadBuff));
//             count =0;
//         }
//         // 通过UART1 发送数据 Send data through UART1
//         usleep(U_SLEEP_TIME);
//     }
// }
// void UartExampleEntry(void)
// {
//     osThreadAttr_t attr;
//     IoTWatchDogDisable();

//     attr.name = "UartTask";
//     attr.attr_bits = 0U;
//     attr.cb_mem = NULL;
//     attr.cb_size = 0U;
//     attr.stack_mem = NULL;
//     attr.stack_size = 5 * 1024; // 任务栈大小*1024 stack size 5*1024
//     attr.priority = osPriorityNormal;

//     if (osThreadNew((osThreadFunc_t)UartTask, NULL, &attr) == NULL)
//     {
//         printf("[UartTask] Failed to create UartTask!\n");
//     }
// }

// APP_FEATURE_INIT(UartExampleEntry);
