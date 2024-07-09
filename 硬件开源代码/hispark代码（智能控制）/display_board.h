

#ifndef __DISPLAY_BOARD_H__
#define __DISPLAY_BOARD_H__

#include "car_config.h"

/* ---- ---- ---- ---- ---- ---- ---- ---- */

//  写入 1条 指令
static hi_void Ssd1306_I2c_Write(hi_u8 addr, hi_u8 data);

// OLED  初始化
hi_void OLED_Init(hi_void);

// OLED 清缓存
hi_void OLED_Clear(hi_void);

// OLED 显示信息
hi_void OLED_Show(hi_void);

// 输入1行字符 必须是英数输入的字符, 否者…… 
// 1个字符 8*16 像素， 全屏 16*4 字符
// x : 0-127 行首距离边框的距离 屏幕一行最多16个字符
// y : 0-7 内存行 1个字符 在内存占2行，屏幕最多4行
// 这样取值 是为了显示居中 比如只有3行 或者行间加行距，但是 容易输入误解 
hi_void OLED_String(hi_u8 x, hi_u8 y, hi_u8 *str);

/*

  居中例子

  // x = ( 128 - 字符数 * 8 ) / 2 - 1;

  OLED_String(31, 0, "********");
  OLED_String(43, 3, "12345");
  OLED_String(31, 6, "********");

*/

/* ---- ---- ---- ---- ---- ---- ---- ---- */

// 按键 ADC 初始化 稳定性太差 弃用
// hi_void Switch_Adc_Init(hi_void);

/* ---- ---- ---- ---- ---- ---- ---- ---- */

#endif