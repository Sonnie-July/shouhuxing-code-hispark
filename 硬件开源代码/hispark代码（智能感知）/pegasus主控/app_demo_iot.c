/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#include <hi_types_base.h>
#include <hi_early_debug.h>
#include <hi_stdlib.h>
#include <hi_uart.h>
#include <app_demo_uart.h>
#include <iot_uart.h>
#include "iot_gpio_ex.h"
#include "hi_io.h"
#include "iot_gpio.h"
#include "hisignalling_protocol.h"






#include <hi_task.h>
#include <string.h>
#include "iot_config.h"
#include "iot_log.h"
#include "iot_main.h"
#include "iot_profile.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "app_demo_multi_sample.h"
#include "app_demo_config.h"
#include "wifi_connecter.h"
#include "app_demo_aht20.h"
#include "app_demo_i2c_oled.h"
#include <app_demo_uart.h>



#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "iot_i2c.h"
#include "iot_gpio.h"
#include "iot_errno.h"
#include "iot_adc.h"//adc函数
#include "beep.h"

#include "app_demo_uart.h"
#include "hisignalling_protocol.h"




/* report traffic light count */
unsigned char ocBeepStatus = BEEP_OFF;
static int g_iState = 0;

/* attribute initiative to report */
#define TAKE_THE_INITIATIVE_TO_REPORT
/* oc request id */
#define CN_COMMADN_INDEX                    "commands/request_id="
/* oc report HiSpark attribute */
#define IO_FUNC_GPIO_OUT 0
#define IOT_GPIO_INDEX_10 10
#define IOT_GPIO_INDEX_11 16
#define IOT_GPIO_INDEX_12 12
#define TRAFFIC_LIGHT_CMD_PAYLOAD           "led_value"
#define TRAFFIC_LIGHT_CMD_CONTROL_MODE      "ControlModule"
#define TRAFFIC_LIGHT_CMD_AUTO_MODE         "AutoModule"
#define TRAFFIC_LIGHT_CMD_HUMAN_MODE        "HumanModule"
#define TRAFFIC_LIGHT_YELLOW_ON_PAYLOAD     "YELLOW_LED_ON"
#define TRAFFIC_LIGHT_RED_ON_PAYLOAD        "RED_LED_ON"
#define TRAFFIC_LIGHT_GREEN_ON_PAYLOAD      "GREEN_LED_ON"
#define TRAFFIC_LIGHT_SERVICE_ID_PAYLOAD    "TrafficLight"
#define TRAFFIC_LIGHT_BEEP_CONTROL          "BeepControl"
#define TRAFFIC_LIGHT_BEEP_ON               "BEEP_ON"
#define TRAFFIC_LIGHT_BEEP_OFF              "BEEP_OFF"
#define TRAFFIC_LIGHT_HUMAN_INTERVENTION_ON     "HUMAN_MODULE_ON"
#define TRAFFIC_LIGHT_HUMAN_INTERVENTION_OFF    "HUMAN_MODULE_OFF"

#define TASK_SLEEP_1000MS (1000)


/*******************************************************************************/
#define ADC_GPIO_OUTPORT 9
#define ADC_TASK_STACK_SIZE 1024
#define ADC_TASK_PRIO 24

float GetVoltage(void)
{
    unsigned int ret;
    unsigned short data;
    //ret相当于返回数值，通过ret的数值来判断是否初始化成功
    ret=AdcRead(IOT_ADC_CHANNEL_4,&data,IOT_ADC_EQU_MODEL_8,IOT_ADC_CUR_BAIS_DEFAULT,0xff);//使用通道ADC4
    if(ret!=IOT_SUCCESS)
    {
        printf("ADC READ FAIL!");
    }
    return (float)data*1.8*4/4096.0;
}
float GetVoltage2(void)//11、、雨滴
{
    unsigned int ret;
    unsigned short data;
    //ret相当于返回数值，通过ret的数值来判断是否初始化成功
    ret=AdcRead(IOT_ADC_CHANNEL_5,&data,IOT_ADC_EQU_MODEL_8,IOT_ADC_CUR_BAIS_DEFAULT,0xff);//使用通道ADC4
    if(ret!=IOT_SUCCESS)
    {
        printf("ADC READ FAIL!");
    }
    return (float)data*1.8*4/4096.0;
}
float GetVoltage3(void)//7人体
{
    unsigned int ret;
    unsigned short data;
    //ret相当于返回数值，通过ret的数值来判断是否初始化成功
    ret=AdcRead(IOT_ADC_CHANNEL_3,&data,IOT_ADC_EQU_MODEL_8,IOT_ADC_CUR_BAIS_DEFAULT,0xff);//使用通道ADC4
    if(ret!=IOT_SUCCESS)
    {
        printf("ADC READ FAIL!");
    }
    return (float)data*1.8*4/4096.0;
}








void TrafficLightAppOption(HiTrafficLightMode appOptionMode, HiControlModeType appOptionType)
{
    unsigned char currentMode = 0;

    currentMode = SetKeyStatus(appOptionMode);
    switch (GetKeyStatus(CURRENT_MODE)) {
        case TRAFFIC_CONTROL_MODE:
            TrafficLightStatusReport(TRAFFIC_CONTROL_MODE, SetupTrflControlModule);
            break;
        case TRAFFIC_AUTO_MODE:
            TrafficLightStatusReport(TRAFFIC_AUTO_MODE, SetupTrflAutoModule);
            break;
        case TRAFFIC_HUMAN_MODE:
            TrafficLightStatusReport(TRAFFIC_HUMAN_MODE, SetupTrflHumanModule);
            break;
        default:
            break;
    }
}

static void TrafficLightMsgRcvCallBack(char *payload)
{
    unsigned char currentMode = 0;
    unsigned char currentType = 0;
    IOT_LOG_DEBUG("PAYLOAD:%s\r\n", payload);
    if (strstr(payload, TRAFFIC_LIGHT_CMD_CONTROL_MODE) != NULL) {
        currentMode = SetKeyStatus(TRAFFIC_CONTROL_MODE);
        if (strstr(payload, TRAFFIC_LIGHT_YELLOW_ON_PAYLOAD) != NULL) { // YELLOW LED
            OledShowStr(OLED_X_POSITION_0, OLED_Y_POSITION_7, "2.Yellow On     ",
                        OLED_DISPLAY_STRING_TYPE_1); /* 0, 7, xx, 1 */
            GpioControl(IOT_GPIO_INDEX_10, IOT_GPIO_INDEX_10,
                        IOT_GPIO_DIR_OUT, IOT_GPIO_VALUE0, IO_FUNC_GPIO_OUT);
            GpioControl(IOT_GPIO_INDEX_12, IOT_GPIO_INDEX_12,
                        IOT_GPIO_DIR_OUT, IOT_GPIO_VALUE1, IO_FUNC_GPIO_OUT);
        } else if (strstr(payload, TRAFFIC_LIGHT_RED_ON_PAYLOAD) != NULL) { // RED LED
            OledShowStr(OLED_X_POSITION_0, OLED_Y_POSITION_7, "1.Red On       ",
                        OLED_DISPLAY_STRING_TYPE_1);  /* 0, 7, xx, 1 */
            GpioControl(IOT_GPIO_INDEX_10, IOT_GPIO_INDEX_10,
                        IOT_GPIO_DIR_OUT, IOT_GPIO_VALUE1, IO_FUNC_GPIO_OUT);
            GpioControl(IOT_GPIO_INDEX_11, IOT_GPIO_INDEX_11,
                        IOT_GPIO_DIR_OUT, IOT_GPIO_VALUE0, IO_FUNC_GPIO_OUT);
        } else if (strstr(payload, TRAFFIC_LIGHT_GREEN_ON_PAYLOAD) != NULL) { // GREEN LED
            OledShowStr(OLED_X_POSITION_0, OLED_Y_POSITION_7, "3.Green On      ",
                        OLED_DISPLAY_STRING_TYPE_1); /* 0, 7, xx, 1 */
            GpioControl(IOT_GPIO_INDEX_12, IOT_GPIO_INDEX_12,
                        IOT_GPIO_DIR_OUT, IOT_GPIO_VALUE0, IO_FUNC_GPIO_OUT);
            GpioControl(IOT_GPIO_INDEX_11, IOT_GPIO_INDEX_11,
                        IOT_GPIO_DIR_OUT, IOT_GPIO_VALUE1, IO_FUNC_GPIO_OUT);
        }
        TrafficLightAppOption(currentMode, currentType);
    } else if (strstr(payload, TRAFFIC_LIGHT_CMD_AUTO_MODE) != NULL) { // Auto module
        currentMode = SetKeyStatus(TRAFFIC_AUTO_MODE);
        TrafficLightAppOption(currentMode, currentType);
    } else if (strstr(payload, TRAFFIC_LIGHT_CMD_HUMAN_MODE) != NULL) { // Human module
        currentMode = SetKeyStatus(TRAFFIC_HUMAN_MODE);
        if (strstr(payload, TRAFFIC_LIGHT_HUMAN_INTERVENTION_ON) != NULL) {
            currentType = SetKeyType(TRAFFIC_HUMAN_TYPE);
        } else if (strstr(payload, TRAFFIC_LIGHT_HUMAN_INTERVENTION_OFF)) {
            currentType = SetKeyType(TRAFFIC_NORMAL_TYPE);
        }
        TrafficLightAppOption(currentMode, currentType);
    } else if (strstr(payload, TRAFFIC_LIGHT_BEEP_CONTROL) != NULL) { // BEEP option
        if (strstr(payload, TRAFFIC_LIGHT_BEEP_ON) != NULL) { // BEEP ON
            ocBeepStatus = BEEP_ON;
        } else if (strstr(payload, TRAFFIC_LIGHT_BEEP_OFF) != NULL) { // BEEP OFF
            ocBeepStatus = BEEP_OFF;
        }
    }
}

// /< this is the callback function, set to the mqtt, and if any messages come, it will be called
// /< The payload here is the json string
static void DemoMsgRcvCallBack(int qos, const char *topic, char *payload)
{
    const char *requesID;
    char *tmp;
    IoTCmdResp resp;
    IOT_LOG_DEBUG("RCVMSG:QOS:%d TOPIC:%s PAYLOAD:%s\r\n", qos, topic, payload);
    /* app 下发的操作 */
    TrafficLightMsgRcvCallBack(payload);
    tmp = strstr(topic, CN_COMMADN_INDEX);
    if (tmp != NULL) {
        // /< now you could deal your own works here --THE COMMAND FROM THE PLATFORM
        // /< now er roport the command execute result to the platform
        requesID = tmp + strlen(CN_COMMADN_INDEX);
        resp.requestID = requesID;
        resp.respName = NULL;
        resp.retCode = 0;   ////< which means 0 success and others failed
        resp.paras = NULL;
        (void)IoTProfileCmdResp(CONFIG_DEVICE_PWD, &resp);
    }
    return;
}

void SetupCleanTrflStatus(HiTrafficLightMode earlyMode)
{
    IoTProfileService service;
    IoTProfileKV property;
    if (earlyMode == TRAFFIC_CONTROL_MODE) {
        memset_s(&property, sizeof(property), 0, sizeof(property));
        property.type = EN_IOT_DATATYPE_STRING;
        property.key = "HumanModule";
        property.value = "OFF";
        memset_s(&service, sizeof(service), 0, sizeof(service));
        service.serviceID = "TrafficLight";
        service.serviceProperty = &property;
        IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    } else if (earlyMode == TRAFFIC_AUTO_MODE) {
        memset_s(&property, sizeof(property), 0, sizeof(property));
        property.type = EN_IOT_DATATYPE_STRING;
        property.key = "温度";
        property.value = "0";
        memset_s(&service, sizeof(service), 0, sizeof(service));
        service.serviceID = "TrafficLight";
        service.serviceProperty = &property;
        IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    } else if (earlyMode == TRAFFIC_HUMAN_MODE) {
        memset_s(&property, sizeof(property), 0, sizeof(property));
        property.type = EN_IOT_DATATYPE_STRING;
        property.key = "AutoModule";
        property.value = "OFF";
        memset_s(&service, sizeof(service), 0, sizeof(service));
        service.serviceID = "TrafficLight";
        service.serviceProperty = &property;
        IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    }
}
/* traffic light:1.control module */
void SetupTrflControlModule(HiTrafficLightMode currentMode, HiControlModeType currentType)
{
     IoTProfileService service;
    IoTProfileKV property;
    unsigned char status = 0;
    //IoTGpioSetOutputVal(ADC_GPIO_OUTPORT,1);
    //IoTGpioSetOutputVal(11,1);
    IoTGpioSetOutputVal(7,0);

    char str1[10];
    char str2[10];
    char str3[10];
    char str4[10];
    char str5[10];
    char str6[10];
    char str7[10];

    int zt = uartBuff[5];
    int wd = GetAhtSensorValue(1);
    int sd = GetAhtSensorValue(2);
    float gz = GetVoltage();
    gz = (1024/(1.1*gz))-3500;
    float nc = GetVoltage2();
    float hw = GetVoltage3();

    Beep_Init();
    if(nc>=3.0)
    {
        Beep_Alarm();
    }
    else if(hw >1.6)
    {
        Beep_Alarm();
    }
    

    




    snprintf(str1, sizeof(str1), "%d", wd);
    snprintf(str2, sizeof(str2), "%d", sd);
    snprintf(str3, sizeof(str3), "%.2lf", gz);
    snprintf(str4, sizeof(str4), "%.2lf", nc);
    snprintf(str5, sizeof(str5), "%.2lf", hw);
    snprintf(str7, sizeof(str7), "%d", zt);


    OledShowStr(OLED_X_POSITION_32, OLED_Y_POSITION_1, \
                str1, OLED_DISPLAY_STRING_TYPE_1); /* 0, 2, xx, 1 */
    OledShowStr(OLED_X_POSITION_32, OLED_Y_POSITION_2, \
                str2, OLED_DISPLAY_STRING_TYPE_1); 
    OledShowStr(OLED_X_POSITION_32, OLED_Y_POSITION_3, \
                str3, OLED_DISPLAY_STRING_TYPE_1);
    OledShowStr(OLED_X_POSITION_32, OLED_Y_POSITION_4, \
                str4, OLED_DISPLAY_STRING_TYPE_1);
    OledShowStr(OLED_X_POSITION_32, OLED_Y_POSITION_5, \
                str5, OLED_DISPLAY_STRING_TYPE_1);
    OledShowStr(OLED_X_POSITION_32, OLED_Y_POSITION_6, \
                str7, OLED_DISPLAY_STRING_TYPE_1);
    printf("traffic light:control module\r\n");
    if (currentMode != TRAFFIC_CONTROL_MODE && currentType != RED_ON) {
        printf("select current module is not the TRAFFIC_CONTROL_MODE\r\n");
        return HI_NULL;
    }
    status = SetKeyStatus(TRAFFIC_CONTROL_MODE);

    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Temperature";
    property.value = str1;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);

    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Humidity";
    property.value = str2;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);

    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Illumination";
    property.value = str3;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);

    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Bed-Wetting";
    property.value = str4;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);

    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "Human-infrared";
    property.value = str5;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);

    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "emojistatus";
    property.value = str7;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);







    /* report beep status */
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "AutoModule";
    if (ocBeepStatus == BEEP_ON) {
        property.value = "BEEP_ON";
    } else {
        property.value = "BEEP_OFF";
    }
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
}
/* report light time count */
void ReportLedLightTimeCount(void)
{
    IoTProfileService service;
    IoTProfileKV property;
    /* report red led light time count */
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_INT;
    property.key = "AutoModuleRLedTC";
    property.iValue = GetLedStatus(RED_LED_AUTOMODE_TIMECOUNT);
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    /* report yellow led light time count */
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_INT;
    property.key = "AutoModuleYLedTC";
    property.iValue = GetLedStatus(YELLOW_LED_AUTOMODE_TIMECOUNT) ;
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    /* report green led light time count */
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_INT;
    property.key = "AutoModuleGLedTC";
    property.iValue = GetLedStatus(GREEN_LED_AUTOMODE_TIMECOUNT);
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
}
/* traffic light:2.auto module */
void SetupTrflAutoModule(HiTrafficLightMode currentMode, HiControlModeType currentType)
{
    IoTProfileService service;
    IoTProfileKV property;
    unsigned char status = 0;

    printf("traffic light:auto module\r\n");
    if (currentMode != TRAFFIC_AUTO_MODE) {
        printf("select current module is not the CONTROL_MODE\r\n");
        return HI_NULL;
    }
    /* report beep status */
    status = SetKeyStatus(TRAFFIC_AUTO_MODE);
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "AutoModule";
    if (ocBeepStatus == BEEP_ON) {
        property.value = "BEEP_ON";
    } else {
        property.value = "BEEP_OFF";
    }
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    /* report light time count */
    ReportLedLightTimeCount();
}

/* traffic light:3.human module */
void SetupTrflHumanModule(HiTrafficLightMode currentMode, HiControlModeType currentType)
{
    IoTProfileService service;
    IoTProfileKV property;
    unsigned char status = 0;

    printf("traffic light:human module\r\n");
    if (currentMode != TRAFFIC_HUMAN_MODE) {
        printf("select current module is not the CONTROL_MODE\r\n");
        return HI_NULL;
    }
    status = GetKeyStatus(TRAFFIC_HUMAN_MODE);
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_STRING;
    property.key = "HumanModule";
    if (ocBeepStatus == BEEP_ON) {
        property.value = "BEEP_ON";
    } else {
        property.value = "BEEP_OFF";
    }
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);

    /* red led light time count */
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_INT;
    property.key = "HumanModuleRledTC";
    property.iValue = GetLedStatus(RED_LED_HUMANMODE_TIMECOUNT);
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    /* yellow led light time count */
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_INT;
    property.key = "HumanModuleYledTC";
    property.iValue = GetLedStatus(YELLOW_LED_HUMANMODE_TIMECOUNT);
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
    /* green led light time count */
    memset_s(&property, sizeof(property), 0, sizeof(property));
    property.type = EN_IOT_DATATYPE_INT;
    property.key = "HumanModuleGledTC";
    property.iValue = GetLedStatus(GREEN_LED_HUMANMODE_TIMECOUNT);
    memset_s(&service, sizeof(service), 0, sizeof(service));
    service.serviceID = "TrafficLight";
    service.serviceProperty = &property;
    IoTProfilePropertyReport(CONFIG_DEVICE_ID, &service);
}

void TrafficLightStatusReport(HiTrafficLightMode currentMode, const TrflCallBackFunc msgReport)
{
    printf("tarffic light: reporting status...\r\n");
    switch (currentMode) {
        case TRAFFIC_CONTROL_MODE:
            msgReport(TRAFFIC_CONTROL_MODE, NULL);
            break;
        case TRAFFIC_AUTO_MODE:
            msgReport(TRAFFIC_AUTO_MODE, NULL);
            break;
        case TRAFFIC_HUMAN_MODE:
            msgReport(TRAFFIC_HUMAN_MODE, NULL);
            break;
        default:
            break;
    }
    return HI_NULL;
}

static void ReportTrafficLightMsg(void)
{
    switch (GetKeyStatus(CURRENT_MODE)) {
        case TRAFFIC_CONTROL_MODE:
            TrafficLightStatusReport(TRAFFIC_CONTROL_MODE, SetupTrflControlModule);
            break;
        case TRAFFIC_AUTO_MODE:
            TrafficLightStatusReport(TRAFFIC_AUTO_MODE, SetupTrflAutoModule);
            break;
        case TRAFFIC_HUMAN_MODE:
            TrafficLightStatusReport(TRAFFIC_HUMAN_MODE, SetupTrflHumanModule);
            break;
        default:
            break;
    }
}
///< this is the demo main task entry,here we will set the wifi/cjson/mqtt ready ,and
///< wait if any work to do in the while
static void *DemoEntry(const char *arg)
{
    hi_watchdog_disable();
    WifiStaReadyWait();
    CJsonInit();
    printf("cJsonInit init \r\n");
    IoTMain();
    IoTSetMsgCallback(DemoMsgRcvCallBack);
/* 主动上报 */
#ifdef TAKE_THE_INITIATIVE_TO_REPORT
    while (1) {
        // /< here you could add your own works here--we report the data to the IoTplatform
        hi_sleep(TASK_SLEEP_1000MS);
        // /< now we report the data to the iot platform
        ReportTrafficLightMsg();
        if (g_iState == 0xffff) {
            g_iState = 0;
            break;
        }
    }
#endif
}

///< This is the demo entry, we create a task here, and all the works has been done in the demo_entry
#define CN_IOT_TASK_STACKSIZE  0x1000
#define CN_IOT_TASK_PRIOR 28
#define CN_IOT_TASK_NAME "IOTDEMO"
static void AppDemoIot(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();

    attr.name = "IOTDEMO";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = CN_IOT_TASK_STACKSIZE;
    attr.priority = CN_IOT_TASK_PRIOR;

    if (osThreadNew((osThreadFunc_t)DemoEntry, NULL, &attr) == NULL) {
        printf("[TrafficLight] Falied to create IOTDEMO!\n");
    }
}

SYS_RUN(AppDemoIot);