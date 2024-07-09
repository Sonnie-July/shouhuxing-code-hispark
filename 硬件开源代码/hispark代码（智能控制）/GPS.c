#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_uart.h"
#include "hi_uart.h"
#include "iot_watchdog.h"
#include "iot_errno.h"

//定义接收来自GPS数据的数组长度1024
#define UART_BUFF_SIZE 1024
#define U_SLEEP_TIME   700000


/**********************************************************************************************************************/
void Uart2GpioInit(void)
{
    IoTGpioInit(IOT_IO_NAME_GPIO_11);

    // 设置GPIO0的管脚复用关系为UART1_TX Set the pin reuse relationship of GPIO0 to UART1_ TX
    IoSetFunc(IOT_IO_NAME_GPIO_11, IOT_IO_FUNC_GPIO_11_UART2_TXD);
    IoTGpioInit(IOT_IO_NAME_GPIO_12);

    // 设置GPIO1的管脚复用关系为UART1_RX Set the pin reuse relationship of GPIO1 to UART1_ RX
    IoSetFunc(IOT_IO_NAME_GPIO_12, IOT_IO_FUNC_GPIO_12_UART2_RXD);
}

/**********************************************************************************************************************/

void Uart2Config(void)
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

    ret = IoTUartInit(HI_UART_IDX_2, &uart_attr);

    if (ret != IOT_SUCCESS) {
        printf("Init Uart2 Falied Error No : %d\n", ret);
        return;
    }

}

/**********************************************************************************************************************/

//将GPS $GNGGA ddmm.mmmm（度分）转换成常用坐标dd.ddddd （WGS-84国际标准下）
double dm_to_dd(double dm) {
    // 获取度部分
    double degree = (int)(dm / 100);
    // 获取分部分
    double minute = dm - degree * 100;
    // 将度分转换为小数形式
    double decimal = degree + minute / 60;
    
    return decimal;
}

/**********************************************************************************************************************/

static void UartTask(void)
{
    const char *data = "Hello OpenHarmony !!!\n";
    uint32_t count = 0;
    uint32_t len = 0;
    unsigned char uartReadBuff[UART_BUFF_SIZE] = {0};

    // 对UART1的一些初始化 Some initialization of UART1
    Uart2GpioInit();
    // 对UART1参数的一些配置 Some configurations of UART1 parameters
    Uart2Config();

    while (1) {
        // 通过UART1 发送数据 Send data through UART1,即3861向GPS模块发送数据的部分。
        //IoTUartWrite(HI_UART_IDX_1, (unsigned char*)data, strlen(data));

        // 通过UART1 接收数据 Receive data through UART1
        usleep(U_SLEEP_TIME);

        len = IoTUartRead(HI_UART_IDX_2, uartReadBuff, UART_BUFF_SIZE);
        if (len > 0) {
            // 把接收到来自GPS模块的数据打印出来 Print the received data

            printf("\r\n--------------------------------------------------------------\r\n");
            printf("[ %d ]\r\n", count);
            printf("The serial port information output by the GPS module is : \r\n");
            printf("%s \r\n",uartReadBuff);

            /***********************************解析GPS模块发送的 NMEA 0183协议的数据帧**************************************/

            /********************获取$GNGGA的经纬度信息（WGS-84国际标准下）***************************************/
            // 查找$GNGGA所在的行
            char *nmea_gngga = strstr(uartReadBuff, "$GNGGA");
            if (nmea_gngga == NULL) {
                 printf("[$GNGGA,WGS-84]The information on the $GNGGA line not found\n");
            }else{
                 printf("[$GNGGA,WGS-84]The information on the $GNGGA line is found\n");
            }

            // 使用strtok函数分割 $GNGGA所在的行
            char *token;
            token = strtok(nmea_gngga, ",");
            char *nmea_fields[15]; //$GNGGA所在的行有15个数
            int i = 0;
            while (token != NULL) {
                 nmea_fields[i++] = token;
                 token = strtok(NULL, ",");
                 if (i >= 15) break;
            }
        
            // 获取第3,4,5,6个字段的值(即GPS模块接收的经纬度坐标)
            char ddmm_output[128];

            if (i >= 6) {  

                   //将第3,4,5,6个字段的值合并在一起，即ddmm_output的值为GPS模块接收的经纬度坐标(ddmm.mmmmm格式),例如3209.22695 N,11841.95567 E
                   //当GPS没有连接信号时，这个数值打印出来是错误的。
                   //nmea_fields[2]表示纬度
                   //nmea_fields[3]来判断是北纬还是南纬，
                   //nmea_fields[4]表示经度，
                   //nmea_fields[5]来判断是东经还是西经
                   sprintf(ddmm_output, "%s %s,%s %s", nmea_fields[2], nmea_fields[3], nmea_fields[4], nmea_fields[5]);
                   printf("[$GNGGA,WGS-84]Latitude and longitude data (ddmm.mmmmm) is : %s \n", ddmm_output);//ddmm_output的值为GPS模块接收的经纬度坐标(ddmm.mmmmm格式)

                   /******将GPS $GNGGA ddmm.mmmm（度分）转换成常用坐标dd.ddddd******/
                   //将字符数组nmea_fields[2]转换为double类型
                   double num0= 0.0;
                   num0 = atof(nmea_fields[2]);
                   //将字符数组nmea_fields[2]由ddmm.mmmm（度分）转换成常用坐标dd.ddddd （WGS-84国际标准下）
                   double dd0 = dm_to_dd(num0);

                   //将字符数组nmea_fields[4]转换为double类型
                   double num1 = 0.0;
                   num1 = atof(nmea_fields[4]);
                   //将字符数组nmea_fields[4]由ddmm.mmmm（度分）转换成常用坐标dd.ddddd （WGS-84国际标准下）
                   double dd1 = dm_to_dd(num1);
                   printf("[$GNGGA,WGS-84]Latitude and longitude data (dd.ddddd) is : %0.5lf\n", dd0);//%0.5lf是格式化输出实数（double类型）时的占位符，其中0表示数值不足指定位数时用0来填充，5表示小数点后保留5位有效数字，l表示该数值为double类型，f表示浮点数。因此，%0.5lf用于输出一个占位符宽度为5（浮点数的整数部分位数+小数点后5位）、小数点后保留5位的实数值。
                   printf("[$GNGGA,WGS-84]Latitude and longitude data (dd.ddddd) is : %0.5lf\n", dd1);
                   /**************************************************************/


            } else { printf("[$GNGGA]Latitude and longitude data no found\n"); }
            /********************************************************************************/

            /****************************************************************************************************************/

        }
        count++;

    }

}

/**********************************************************************************************************************/

void UartExampleEntry(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();

    attr.name = "UartTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 25 *1024; // 任务栈大小*1024 
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)UartTask, NULL, &attr) == NULL) {
        printf("[UartTask] Failed to create UartTask!\n");
    }
}

APP_FEATURE_INIT(UartExampleEntry);