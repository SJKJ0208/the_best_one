#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define USART_REC_LEN  	200  	//�����������ֽ��� 200
#define EN_USART1_RX    1
#define RXBUFFERSIZE    1
extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler;

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���

extern u8  USART_RX_BUF2[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA2;         		//����״̬���
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);

///��ʼ�����ڵĺ�������ӿ�
void uart_init_1(u32 bound);
void uart_init_2(u32 bound);




#ifdef __cplusplus
}
#endif
#endif


