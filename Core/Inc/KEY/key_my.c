//
// Created by o.o on 2022/5/21.
//

#include "key_my.h"
#include "delay.h"
#include "retarget.h"

//按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟

    GPIO_Initure.Pin=KEY_UP_Pin|KEY_DOWN_Pin|KEY_Enter_Pin;
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY_UP==0||KEY_DOWN==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY_UP==0)       return KEY_UP_PRES;
        else if(KEY_DOWN==0)  return KEY_DOWN_PRES;
    }else if(KEY_UP==1&&KEY_DOWN==1)key_up=1;
    return 0;   //无按键按下
}

u8 KEY_Scan_enter(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY_ENTER==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY_ENTER==0) return KEY_ENTER_PRES;
    }else if(KEY_ENTER==0)key_up=1;
    return 0;   //无按键按下
}