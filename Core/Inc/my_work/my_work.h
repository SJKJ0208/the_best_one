//
// Created by o.o on 2022/5/20.
//

#ifndef TEST_280_MY_WORK_H
#define TEST_280_MY_WORK_H
#include "retarget.h"


///初始化
void The_sum_init();


///test各种命令///
///test各种lcd的命令///
void test_lcd();
///test_open代码///
void test_command_open();
///test_6050
void test_6050();
void use_6050();


void danger_reply();
void Lcd_num();

///与上位机交流的代码
void The_comcupter_comunicate();

extern float R_of_circle;


#endif //TEST_280_MY_WORK_H
