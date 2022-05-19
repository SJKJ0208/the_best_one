#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//LED驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/9/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#define LED_R PAout(1)   	//LED0
#define LED_G PAout(2)   	//LED1
#define LED_B PAout(3)   	//LED1

///此处根据硬件需求进行改动
#define DOWN_LED 1
#define   UP_LED 0

void LED_Init(void);
#endif
