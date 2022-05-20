//
// Created by o.o on 2022/5/21.
//

#ifndef TEST_280_KEY_MY_H
#define TEST_280_KEY_MY_H
#include "sys.h"

#endif //TEST_280_KEY_MY_H

#define KEY_Enter_Pin GPIO_PIN_7
#define KEY_Enter_GPIO_Port GPIOB
#define KEY_UP_Pin GPIO_PIN_13
#define KEY_UP_GPIO_Port GPIOB
#define KEY_DOWN_Pin GPIO_PIN_14
#define KEY_DOWN_GPIO_Port GPIOB

//下面的方式是通过直接操作HAL库函数方式读取IO
#define KEY_UP          HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)  //KEY0按键PE4
#define KEY_DOWN        HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin)  //KEY1按键PE3
#define KEY_ENTER       HAL_GPIO_ReadPin(KEY_Enter_GPIO_Port,KEY_Enter_Pin)  //WKUP按键PA0

#define KEY_UP_PRES 	1
#define KEY_DOWN_PRES	2
#define KEY_ENTER_PRES   3

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
u8 KEY_Scan_enter(u8 mode);
