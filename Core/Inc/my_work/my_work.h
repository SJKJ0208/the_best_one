//
// Created by o.o on 2022/5/20.
//

#ifndef TEST_280_MY_WORK_H
#define TEST_280_MY_WORK_H
#include "retarget.h"
#include "led.h"
#include "usart.h"

///��ʼ��
void The_sum_init();


///test��������///
///test����lcd������///
void test_lcd();
///test_open����///
void test_command_open();
///test_6050
void test_6050();
void use_6050();


void danger_reply();
void Lcd_num();

///����λ�������Ĵ���
void The_comcupter_comunicate();

extern float R_of_circle;


#endif //TEST_280_MY_WORK_H
