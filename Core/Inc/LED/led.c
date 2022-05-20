#include "led.h"
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

//��ʼ��PB1Ϊ���.��ʹ��ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//����GPIOAʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();           	//����GPIOAʱ��

    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3; 				//PA1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_6; 				//PA2
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);


    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,DOWN_LED);	//PA1��1��Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,DOWN_LED);	//PA2��1��Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,DOWN_LED);	//PA3��1��Ĭ�ϳ�ʼ�������

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,BEEP_DOWN);	//PB8���㣬Ĭ�ϳ�ʼ������
}
