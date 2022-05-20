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

///初始化
void The_sum_init();


///test各种命令///
///test各种lcd的命令///
void test_lcd();
///test_open代码///
void test_command_open();



///与上位机交流的代码
void The_comcupter_comunicate();

#endif //TEST_280_MY_WORK_H
