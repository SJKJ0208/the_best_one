#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//LED��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/9/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#define LED_R PAout(1)   	//LED0
#define LED_G PAout(2)   	//LED1
#define LED_B PAout(3)   	//LED1

///�˴�����Ӳ��������иĶ�
#define DOWN_LED 1
#define   UP_LED 0

void LED_Init(void);
#endif
