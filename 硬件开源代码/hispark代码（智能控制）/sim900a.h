#ifndef SIM900A_H
#define SIM900A_H

#define UART_BUFF_SIZE 150
#define U_SLEEP_TIME 1000000
uint32_t len = 0;
char uartReadBuff[UART_BUFF_SIZE] = {0};
uint8_t ef[2] = {0x10,0x0D};
const char *data = "ATD15637272476;\r\n";
//拨打电话
const char* tel ="ATD15637272476;\r\n";//拨打电话
//发送短信
const char* Mode ="AT+CMGF=1\r\n";//短信模式
const char* GSM ="AT+CSCS=\"GSM\"\r\n";//GSM模式
const char* Gettel ="AT+CSCA?\r\n";//获取短信中心号码
const char* Modex ="AT+CSMP=17,167,0,240\r\n";//配置模式 240：显示在终端 241：正常短信
const char* Pho ="AT+CMGS=\"15637272476\"\r\n";//目标号码
const char* Mess ="HELP ME!!!\r\n";//短信内容
uint8_t ctrl_z_command[] = {0x1A};

char sim_call(void);
char sim_send_message(void);
void Uart1GpioInit(void);
void Uart1Config(void);

#endif