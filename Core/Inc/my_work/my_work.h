//
// Created by o.o on 2022/5/20.
//

#ifndef TEST_280_MY_WORK_H
#define TEST_280_MY_WORK_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "stdlib.h"
#include "test_os/test_os.h"
#include "retarget.h"
#include "stdio.h"
#include "math.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "lcd_init.h"

///��ʼ��
void The_sum_init();


///test��������///
///test����lcd������///
void test_lcd();
///test_open����///
void test_command_open();



///����λ�������Ĵ���
void The_comcupter_comunicate();

#endif //TEST_280_MY_WORK_H
