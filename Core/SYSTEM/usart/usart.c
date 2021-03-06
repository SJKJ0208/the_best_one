#include "sys.h"
#include "usart.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2019/9/17
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0 ///在retarget中实现
#pragma import(__use_no_semihosting)
//标准库需要的支持函数                 
struct __FILE
{
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#endif
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART_RX_BUF2[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
u16 USART_RX_STA2=0;       //接收状态标记

u8 bRxBuffer[RXBUFFERSIZE];///用于hal库串口接受缓冲///第二个串口所用的
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART句柄
UART_HandleTypeDef UART2_Handler;
//初始化IO 串口1 
//bound:波特率
void uart_init_1(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式

    if (HAL_UART_Init(&UART1_Handler) != HAL_OK) //HAL_UART_Init()会使能UART1
    {
        Error_Handler();
    }
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}
void uart_init_2(u32 bound)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    UART2_Handler.Instance = USART2;
    UART2_Handler.Init.BaudRate = bound;
    UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;
    UART2_Handler.Init.StopBits = UART_STOPBITS_1;
    UART2_Handler.Init.Parity = UART_PARITY_NONE;
    UART2_Handler.Init.Mode = UART_MODE_TX_RX;
    UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&UART2_Handler) != HAL_OK)      //HAL_UART_Init()会使能UART1
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */
    HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)bRxBuffer, RXBUFFERSIZE);
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收
    /* USER CODE END USART2_Init 2 */

}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();

		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10

        ///假若使用了接受，还需要设置中断
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,0,0);			//抢占优先级3，子优先级3
#endif
	}
    else if(huart->Instance==USART2)
    {
        GPIO_InitTypeDef GPIO_Initure2;
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_Initure2.Pin = GPIO_PIN_2;
        GPIO_Initure2.Mode = GPIO_MODE_AF_PP;
        GPIO_Initure2.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_Initure2);

        GPIO_Initure2.Pin = GPIO_PIN_3;
        GPIO_Initure2.Mode = GPIO_MODE_INPUT;
        GPIO_Initure2.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_Initure2);

        /* USART2 interrupt Init */
        ///设置接收中断
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

    if(uartHandle->Instance==USART1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    else if(uartHandle->Instance==USART2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

        /* USART2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}

void USART_send_byte(uint8_t byte)
{
    while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);//等待发送完成
    USART2->DR=byte;
}
//发送多字节数据
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        USART_send_byte(Buffer[i++]);
    }
}
//发送多字节数据+校验和
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        if(i<(Length-1))
            Buffer[Length-1]+=Buffer[i];//累加Length-1前的数据
        USART_send_byte(Buffer[i++]);
    }
}
//发送一帧数据
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
    uint8_t TX_DATA[30],i=0,k=0;
    memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
    TX_DATA[i++]=0X5A;//帧头
    TX_DATA[i++]=0X5A;//帧头
    TX_DATA[i++]=send;//功能字节
    TX_DATA[i++]=2*length;//数据长度
    for(k=0;k<length;k++)//存入数据到缓存TX_DATA
    {
        TX_DATA[i++]=(uint16_t)data[k]>>8;
        TX_DATA[i++]=(uint16_t)data[k];
    }
    USART_Send(TX_DATA,i);
}
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send)
{
    uint8_t TX_DATA[50],i=0,k=0;
    memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
    TX_DATA[i++]=0X5A;//帧头
    TX_DATA[i++]=0X5A;//帧头
    TX_DATA[i++]=send;//功能字节
    TX_DATA[i++]=length;//数据长度
    for(k=0;k<length;k++)//存入数据到缓存TX_DATA
    {
        TX_DATA[i++]=(uint16_t)data[k];
    }
    USART_Send(TX_DATA,i);
}

uint8_t RX_BUF_6050[50]={0},stata_6050=0;

//校验和检查
uint8_t CHeck(uint8_t *data)
{
    uint8_t sum=0,number=0,i=0;
    number=RX_BUF_6050[3]+5;
    if(number>42)//超过上传数据
        return 0;
    for(i=0;i<number-1;i++)
        sum+=RX_BUF_6050[i];
    if(sum==RX_BUF_6050[number-1])
    {
        memcpy(data,RX_BUF_6050,number);
        return 1;
    }
    else
        return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t RX_BUF[50]={0},stata=0;
    static uint8_t rebuf[13]={0},i=0;
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了
			}
			else //还没收到0X0D
			{
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收
				}
			}
		}
	}
    else if(huart->Instance==USART2)//如果是串口2
    {
        rebuf[i++]=bRxBuffer[0];
        if(rebuf[0]!=0x5a)//判断帧头
            i=0;
        if((i==2)&&(rebuf[1]!=0x5a))//判断帧头
            i=0;
        if(i>4)//当i计数值=5时，功能字节接受完毕，数据长度字节接收完毕
        {
            if(i==rebuf[3]+5)
            {
                memcpy(RX_BUF_6050,rebuf,i);
                stata_6050=1;
                i=0;
            }
        }
    }
}


//串口1中断服务程序
void USART1_IRQHandler(void)
{
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();
#endif
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;

	}
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
        //一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();
#endif
}


void USART2_IRQHandler(void)
{
    uint32_t  timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
    OSIntEnter();
#endif
    HAL_UART_IRQHandler(&UART2_Handler);
    timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)
        //等待就绪
    {
        timeout++;////超时处理
        if(timeout>HAL_MAX_DELAY) break;

    }
    timeout=0;
    while(HAL_UART_Receive_IT(&UART2_Handler, (uint8_t *)bRxBuffer, RXBUFFERSIZE) != HAL_OK)
        //一次处理完成之后，重新开启中断并设置RxXferCount为1
    {
        timeout++; //超时处理
        if(timeout>HAL_MAX_DELAY) break;
    }
#if SYSTEM_SUPPORT_OS	 	//使用OS
    OSIntExit();
#endif
}

/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。*/

//串口1中断服务程序
//void USART1_IRQHandler(void)
//{
//	u8 Res;
//	HAL_StatusTypeDef err;
//#if SYSTEM_SUPPORT_OS	 	//使用OS
//	OSIntEnter();
//#endif
//	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res=USART1->DR;
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了
//			}
//			else //还没收到0X0D
//			{
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收
//				}
//			}
//		}
//	}
//	HAL_UART_IRQHandler(&UART1_Handler);
//#if SYSTEM_SUPPORT_OS	 	//使用OS
//	OSIntExit();
//#endif
//}
//#endif


