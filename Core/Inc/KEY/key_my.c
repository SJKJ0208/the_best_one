//
// Created by o.o on 2022/5/21.
//

#include "key_my.h"
#include "delay.h"
#include "retarget.h"

//������ʼ������
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��

    GPIO_Initure.Pin=KEY_UP_Pin|KEY_DOWN_Pin|KEY_Enter_Pin;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(KEY_UP==0||KEY_DOWN==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY_UP==0)       return KEY_UP_PRES;
        else if(KEY_DOWN==0)  return KEY_DOWN_PRES;
    }else if(KEY_UP==1&&KEY_DOWN==1)key_up=1;
    return 0;   //�ް�������
}

u8 KEY_Scan_enter(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(KEY_ENTER==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY_ENTER==0) return KEY_ENTER_PRES;
    }else if(KEY_ENTER==0)key_up=1;
    return 0;   //�ް�������
}