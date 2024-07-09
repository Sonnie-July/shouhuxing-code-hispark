

#include "car_wifi.h"

static int wifi_eve;        // WIFI设备状态值

// -------------

static char sta_ifname[WIFI_IFNAME_MAX_SIZE + 1] = {0};     // STA接口名

static hi_bool conn_ap = HI_FALSE;       // 热点连接状态 多重用 是否检索到热点 是否连接上

// ---------------

static struct netif *wlan_netif = NULL;     // 网口信息


// WiFi 设备初始化
static void WiFi_Device_Init(void)
{   
    if(hi_wifi_get_init_status() != 1)   // 如果返回值1 驱动已经初始化
    {
        hi_wifi_init(1, 1);
    }
}


// WiFi 事件
static void Wifi_Event_CD(const hi_wifi_event *wifi_event)
{
    wifi_eve = -1;  // WiFi 事件 状态 值

    if(wifi_event == NULL)
    {
        return NULL;
    }

    wifi_eve = wifi_event -> event;
}


// wifi事件返回 设置
static void Wifi_Event_Init(void)
{
    // 配置事件回调的回调方式 : 1 (wpa线程直接调用)
    hi_wifi_config_callback(1, NULL, NULL);

    // 注册回调函数接口
    hi_wifi_register_event_callback(Wifi_Event_CD);    
}


// WiFi STA 设置 连接
static void WiFi_STA_Start(char *wifi_name, char *wifi_pass)
{
    // 创建 STA 接口 
    int sta_namlen = sizeof(sta_ifname);             // STA接口名长度
    
    // 开启STA
    hi_wifi_sta_start(sta_ifname, &sta_namlen);

    // --------------------

    // STA扫描（不带参数扫描）
    hi_wifi_sta_scan();

    while(wifi_eve != 1)
    {
        osDelay(10);
    }   

    // 返回 扫描到的热点信息 先申请1个空间 存放扫描结果
    unsigned int ap_info_len = sizeof(hi_wifi_ap_info) * WIFI_SCAN_AP_LIMIT;    // 支持扫描ap的最多数目  WIFI_SCAN_AP_LIMIT  32

    hi_wifi_ap_info *ap_list = malloc(ap_info_len); // 分配数据存放空间

    memset_s(ap_list, ap_info_len, 0, ap_info_len); // 空间格式化0

    unsigned int ap_num = WIFI_SCAN_AP_LIMIT;       // 最大 AP热点扫描数

    hi_wifi_sta_scan_results(ap_list, &ap_num);     // 传送1个最大扫描数 返回1个实搜到的数  

    int idx;    // 序号 多重复用    

    // 检索热点信息 在扫描结果中 一一比对有没有符合指定WiFi名的热点
    for(idx = 0; idx < ap_num; idx ++)
    {
        if(strlen(wifi_name) == strlen(ap_list[idx].ssid))  // 长度一样
        {
            if(memcmp(wifi_name, ap_list[idx].ssid, strlen(wifi_name)) == HISI_OK)  // 再逐一字符比对 如果一样 打印出来
            {
                conn_ap = HI_TRUE;  // 找到匹配的热点啦
                break;
            }
        }
    }

    if(conn_ap == HI_FALSE) // 没找到
    {
        free(ap_list);
        return;
    } 

    // 接入网络
    hi_wifi_assoc_request conn_req = {0};   // WiFi接入信息

    // SSID 名
    memcpy_s(conn_req.ssid, (HI_WIFI_MAX_SSID_LEN + 1), wifi_name, strlen(wifi_name));
    // 认证类型
    conn_req.auth = ap_list[idx].auth;
    // 密钥
    memcpy_s(conn_req.key, (HI_WIFI_MAX_KEY_LEN + 1), wifi_pass, strlen(wifi_pass));
    // BSSID
    memcpy_s(conn_req.bssid, HI_WIFI_MAC_LEN, ap_list[idx].bssid, strlen(ap_list[idx].bssid));
    // 加密方式
    conn_req.pairwise = 0;

    hi_wifi_sta_connect(&conn_req);  // STA 站点 接入

    while(wifi_eve != 2)
    {
        osDelay(10);
    }

    free(ap_list);  // 用完了 还回申请的空间
}


// DHCP 服务部分
static void WiFi_DHCP_Start(void)
{ 
    // 获取 接口的 描述信息
    wlan_netif = netifapi_netif_find(sta_ifname);

    if (wlan_netif == NULL )
    {
        return;     
    }
    
    // 给指定接口 分配IP
    netifapi_dhcp_start(wlan_netif);

    osDelay(200); // 等待 
}

// WiFi连接开启 wifi名 wifi密码 返回ip地址
void WiFi_Conn_Start(char *wifi_name, char *wifi_pass, char *ip_str)
{
    // WiFi 设备初始化
    WiFi_Device_Init();

    // WiFi 事件 注册
    Wifi_Event_Init();

    // WiFi STA 设置 连接
    WiFi_STA_Start(wifi_name, wifi_pass);

    // DHCP 服务部分
    WiFi_DHCP_Start();

    // ----

    ip4_addr_t wlan_ipaddr;     // ip地址
    ip4_addr_t wlan_netmask;    // 掩码
    ip4_addr_t wlan_gw;         // 网关

    IP4_ADDR(&wlan_ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&wlan_netmask, 0, 0, 0, 0);
    IP4_ADDR(&wlan_gw, 0, 0, 0, 0);

    // 查看分配的IP地址
    netifapi_netif_get_addr(wlan_netif, &wlan_ipaddr, &wlan_netmask, &wlan_gw); 
    char *ipaddr = NULL;
    ipaddr = ipaddr_ntoa(&wlan_ipaddr);
    strcpy(ip_str, ipaddr);

    printf("\n car_wifi [WiFi_Conn_Start] \n");
}


// WiFi 关闭 STA 连接
void Wifi_Conn_Stop(void)
{
    // // 查询Wi-Fi连接状态
    // hi_wifi_status conn_sta = {0};
    // memset_s(&conn_sta, sizeof(hi_wifi_status), 0, sizeof(hi_wifi_status));

    // // 获取station连接的网络状态
    // hi_wifi_sta_get_connect_info(&conn_sta);   

    // // 输出网络连接状态
    // if(conn_sta.status == HI_WIFI_CONNECTED)  // == 1
    // {
    //     // 断开网络连接
    //     hi_wifi_sta_disconnect();        
    // }
    
    // 链接状态为真
    if(conn_ap)
    {
        // 断开网络连接
        hi_wifi_sta_disconnect(); 
        conn_ap = HI_FALSE;
    }
    
    // wlan_netif 是否不为空
    if(wlan_netif != NULL)
    {
        // 撤销IP配置
        netifapi_dhcp_release(wlan_netif); 

        // 停止DHCP客户端
        netifapi_dhcp_stop(wlan_netif); 

        wlan_netif = NULL;
    }

    // 查看 ifname 是否为空
    if(strlen(sta_ifname) > 1)
    {
        // 关闭STA
        hi_wifi_sta_stop();

        // 清空 sta_ifname
        memset(sta_ifname, 0,  sizeof(sta_ifname));
    }

    printf("\n car_wifi [Wifi_Conn_Stop] \n");
}










