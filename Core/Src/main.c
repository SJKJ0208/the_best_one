/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void send_Instruction(void)
{
    uint8_t send_data[4]={0};
    send_data[0]=0xa5;
    send_data[1]=0x55;
    send_data[2]=0x50;
    send_data[3]=0x4a;
    USART_Send_bytes(send_data,4);//发送温度/欧拉角输出指令

    HAL_Delay(200);

    send_data[0]=0xa5;
    send_data[1]=0x56;
    send_data[2]=0x02;
    send_data[3]=0xfd;
    USART_Send_bytes(send_data,4);//发送自动输出指令
    HAL_Delay(200);
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

float pressure, temperature, humidity;

uint16_t size;
uint8_t Data[256];
uint8_t is_error = 0;
uint32_t re_draw = 1;
uint32_t pose = 0;
uint32_t state_sys = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    uint8_t  len;
    uint16_t  times=0;
    uint8_t data_buf[50]={0},count=0;
    int16_t ROLL=0,PITCH=0,YAW=0;
    int16_t rpy[3]={0},Acc[3]={0},Gyr[3]={0},Mag[3]={0},Q[4]={0},Temp=0,Altitude=0;
    uint32_t pressure=0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  delay_init(64);
  //MX_GPIO_Init();


  MX_SPI1_Init();
 // uart_init_2(115200);
  uart_init_1(115200);
  /* USER CODE BEGIN 2 */
  ///自锁
  RetargetInit(&UART1_Handler);
  LED_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1)
    {
//
//        if(USART_RX_STA&0x8000)
//        {
//            len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//            printf("\r\n您发送的消息为:\r\n");
//            HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
//            while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
//            printf("\r\n\r\n");//插入换行
//            USART_RX_STA=0;
//        }else
//        {
//            times++;
//            if(times%5000==0)
//            {
//                printf("\r\nALIENTEK 精英STM32开发板 串口实验\r\n");
//                printf("正点原子@ALIENTEK\r\n\r\n\r\n");
//            }
//            if(times%200==0)
//                printf("请输入数据,以回车键结束\r\n");
//            if(times%30==0)
//                HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);//闪烁LED,提示系统正在运行.
//            delay_ms(20);
//
//        }
            ///test the led
            LED_B = 1;
        delay_ms(300);
        LED_B = 0;
        delay_ms(300);

    }
  while (1)
  {
//    /* USER CODE END WHILE */
//
//    /* USER CODE BEGIN 3 */
//      if (re_draw == 1 && is_error == 0)
//      {
//          re_draw = 0;
//      }
//      if(!stata)
//          continue;
//      stata=0;
//      if(CHeck(data_buf))
//      {
//          count=0;
//          if(data_buf[2]&0x01) //ACC
//          {
//              Acc[0]=(data_buf[4]<<8)|data_buf[5];
//              Acc[1]=(data_buf[6]<<8)|data_buf[7];
//              Acc[2]=(data_buf[8]<<8)|data_buf[9];
//              count=6;
//          }
//          if(data_buf[2]&0x02) //GYRO
//          {
//              Gyr[0]=(data_buf[4+count]<<8)|data_buf[5+count];
//              Gyr[1]=(data_buf[6+count]<<8)|data_buf[7+count];
//              Gyr[2]=(data_buf[8+count]<<8)|data_buf[9+count];
//              count+=6;
//          }
//          if(data_buf[2]&0x04) //MAG
//          {
//              Mag[0]=(data_buf[4+count]<<8)|data_buf[5+count];
//              Mag[1]=(data_buf[6+count]<<8)|data_buf[7+count];
//              Mag[2]=(data_buf[8+count]<<8)|data_buf[9+count];
//              count+=6;
//          }
//
//          if(data_buf[2]&0x10) //欧拉角
//          {
//              rpy[0]=(data_buf[4+count]<<8)|data_buf[5+count];
//              rpy[1]=(data_buf[6+count]<<8)|data_buf[7+count];
//              rpy[2]=(data_buf[8+count]<<8)|data_buf[9+count];
//
//              HAL_Delay(20);
//              if((abs((float) rpy[0]/100)>50)||(abs((float) rpy[1]/100)>50))
//              {
//                  printf("RPY: %.2f,%.2f ,%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
//                  is_error = 1;
//              }
//              else{
//                  is_error = 0;
//                  printf("RPY: %.2f,%.2f ,%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
//              }
//              count+=6;
//              if((abs((float) rpy[0]/100)>10))
//              {
//                  if (((float) rpy[0]/100) <0)
//                  {
//
//                      ///ST7789_WriteString(ST7789_WIDTH/2+38, ST7789_HEIGHT/2-75, "UP", Font_16x26, BLACK, WHITE);
//                  }
//                  else {
//                      ///ST7789_WriteString(ST7789_WIDTH / 2 + 38, ST7789_HEIGHT / 2 - 75, "DOWN", Font_16x26, BLACK,WHITE);
//                  }
//              }
//          }
//          if(data_buf[2]&0x40) //温度
//          {
//              Temp=(data_buf[4+count]<<8)|data_buf[5+count];
//              printf(" ,Temp: %.2f ℃ \r\n",(float) Temp/100);
//              if ((float) Temp/100 > 34 && is_error == 0)
//              {
//                 /// ST7789_WriteString(ST7789_WIDTH/2-90, ST7789_HEIGHT/2-75, "HIGHT", Font_16x26, BLACK, WHITE);
//              }
//              else if ((float) Temp/100 <= 34 && is_error == 0)
//              {
//                 /// ST7789_WriteString(ST7789_WIDTH/2-90, ST7789_HEIGHT/2-75, "NOR", Font_16x26, BLACK, WHITE);
//
//              }
//              count+=2;
//          }
//      }
//      //warinning_task();
        }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void warinning_task()
//{
//    if (is_error!= 0)
//        re_draw = 1;
//    if (is_error == 1)
//    {
//        ST7789_Fill_Color(RED);
//        HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
//    }
//    else if (is_error == 2)
//    {
//        ///heating
//        ST7789_Fill_Color(RED);
//        HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
//    }
//    else
//    {
//        HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
//    }
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
