

#ifndef __CAR_WIFI_H__
#define __CAR_WIFI_H__

#include "car_config.h"

// WiFi 设备初始化
static void WiFi_Device_Init(void);

// WiFi 事件
static void Wifi_Event_CD(const hi_wifi_event *wifi_event);

// wifi事件返回 设置
static void Wifi_Event_Init(void);

// WiFi STA 设置 连接
static void WiFi_STA_Start(char *wifi_name, char *wifi_pass);

// DHCP 服务部分
static void WiFi_DHCP_Start(void);

// ----

// WiFi连接开启 wifi名 wifi密码 返回ip地址
void WiFi_Conn_Start(char *wifi_name, char *wifi_pass, char *ip_str);

// WiFi 关闭 STA 连接
void Wifi_Conn_Stop(void);

//  ----



#endif