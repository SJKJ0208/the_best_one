#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"



#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif

//-----------------LCD端口定义----------------
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) ;
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) ;


#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOA,I2C_CLK_TEM_Pin)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOA,I2C_CLK_TEM_Pin)

#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOA,I2C_SDA_TEM_Pin)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(GPIOA,I2C_SDA_TEM_Pin)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOB,ST7789_RST_Pin)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOB,ST7789_RST_Pin)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOA,ST7789_DC_Pin)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOA,ST7789_DC_Pin)
 		     
#define LCD_CS_Clr()   GPIO_ResetBits(GPIOB,ST7789_CS_Pin)//CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOB,ST7789_CS_Pin)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOB,LCD_BL_Pin)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOB,LCD_BL_Pin)




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




