#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"



#define USE_HORIZONTAL 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif

//-----------------LCD�˿ڶ���----------------
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




void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);//ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��
#endif




