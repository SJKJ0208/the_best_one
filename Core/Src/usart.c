/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "string.h"

uint8_t  USART_RX_BUF[50];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t  USART_RX_STA=0;       //����״̬���

uint8_t  aRxBuffer[10];//HAL��ʹ�õĴ��ڽ��ջ���
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
    HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, 1);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ������
  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint8_t  USART_RX_BUF[50];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

uint8_t  aRxBuffer[10];//HAL��ʹ�õĴ��ڽ��ջ���


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;


void USART_send_byte(uint8_t byte)
{
    while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)!=SET);//�ȴ��������
    USART2->DR=byte;
}
//���Ͷ��ֽ�����
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        USART_send_byte(Buffer[i++]);
    }
}
//���Ͷ��ֽ�����+У���
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        if(i<(Length-1))
            Buffer[Length-1]+=Buffer[i];//�ۼ�Length-1ǰ������
        USART_send_byte(Buffer[i++]);
    }
}
//����һ֡����
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
    uint8_t TX_DATA[30],i=0,k=0;
    memset(TX_DATA,0,(2*length+5));//���㻺��TX_DATA
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=send;//�����ֽ�
    TX_DATA[i++]=2*length;//���ݳ���
    for(k=0;k<length;k++)//�������ݵ�����TX_DATA
    {
        TX_DATA[i++]=(uint16_t)data[k]>>8;
        TX_DATA[i++]=(uint16_t)data[k];
    }
    USART_Send(TX_DATA,i);
}
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send)
{
    uint8_t TX_DATA[50],i=0,k=0;
    memset(TX_DATA,0,(2*length+5));//���㻺��TX_DATA
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=0X5A;//֡ͷ
    TX_DATA[i++]=send;//�����ֽ�
    TX_DATA[i++]=length;//���ݳ���
    for(k=0;k<length;k++)//�������ݵ�����TX_DATA
    {
        TX_DATA[i++]=(uint16_t)data[k];
    }
    USART_Send(TX_DATA,i);
}
uint8_t RX_BUF[50]={0},stata=0;

//У��ͼ��
uint8_t CHeck(uint8_t *data)
{
    uint8_t sum=0,number=0,i=0;
    number=RX_BUF[3]+5;
    if(number>42)//�����ϴ�����
        return 0;
    for(i=0;i<number-1;i++)
        sum+=RX_BUF[i];
    if(sum==RX_BUF[number-1])
    {
        memcpy(data,RX_BUF,number);
        return 1;
    }
    else
        return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t rebuf[13]={0},i=0;
    if(huart->Instance==USART2)//����Ǵ���2
    {
        rebuf[i++]=aRxBuffer[0];
        if(rebuf[0]!=0x5a)//�ж�֡ͷ
            i=0;
        if((i==2)&&(rebuf[1]!=0x5a))//�ж�֡ͷ
            i=0;
        if(i>4)//��i����ֵ=5ʱ�������ֽڽ�����ϣ����ݳ����ֽڽ������
        {
            if(i==rebuf[3]+5)
            {
                memcpy(RX_BUF,rebuf,i);
                stata=1;
                i=0;
            }
        }
    }
}

void USART2_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    uint32_t  timeout=0;
    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart2);
    /* USER CODE BEGIN USART1_IRQn 1 */

    timeout=0;
    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)//�ȴ�����
    {
        timeout++;////��ʱ����
        if(timeout>HAL_MAX_DELAY) break;

    }

    timeout=0;
    while(HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, 1) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
    {
        timeout++; //��ʱ����
        if(timeout>HAL_MAX_DELAY) break;
    }
    /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
