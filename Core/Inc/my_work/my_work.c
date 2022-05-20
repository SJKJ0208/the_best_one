//
// Created by o.o on 2022/5/20.
//

#include "my_work.h"
#include "pic.h"
#include "usart.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "stdlib.h"
#include "test_os/test_os.h"
#include "stdio.h"
#include "math.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "lcd_init.h"

uint16_t size;
uint8_t Data[256];
uint8_t is_error = 0;
uint32_t re_draw = 1;
uint32_t pose = 0;
uint32_t state_sys = 0;

uint8_t  len;
uint16_t  times=0;

///6050循环里面用到的
uint8_t data_buf[50]={0},count=0;
int16_t ROLL=0,PITCH=0,YAW=0;
int16_t rpy[3]={0},Acc[3]={0},Gyr[3]={0},Mag[3]={0},Q[4]={0},Temp=0,Altitude=0;
uint32_t pressure=0;

void send_Instruction(void)
{
    ///发送的内容应该是 a5 55 50 4a a5 56 02 fd
    uint8_t send_data[4]={0};
    send_data[0]=0xa5;
    send_data[1]=0x55;
    send_data[2]=0x50;
    send_data[3]=0x4a;
    USART_Send_bytes(send_data,4);//发送温度/欧拉角输出指令

    delay_ms(100);

    send_data[0]=0xa5;
    send_data[1]=0x56;
    send_data[2]=0x02;
    send_data[3]=0xfd;
    USART_Send_bytes(send_data,4);//发送自动输出指令
    delay_ms(100);
}
///除系统初始化外的全部初始化集合  /* Initialize all configured peripherals */
void The_sum_init()
{
    delay_init(64);
    MX_SPI1_Init();
    // uart_init_2(115200);
    uart_init_1(115200);
    uart_init_2(115200);
    RetargetInit(&UART1_Handler);
    LED_Init();//LED初始化
    LCD_Init();//LCD初始化
    ///6050初始化
    send_Instruction();

    ///先对屏幕白化
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
}

///用于测试单元模块
void test_command_open()
{
    BEEP = BEEP_DOWN;
    LCD_ShowChinese((LCD_W/2)-(3*32),(LCD_H/2)+(174/2),"广东工业大学",RED,WHITE,32,0);
    delay_ms(1000);
    BEEP = BEEP_UP;
    LCD_ShowPicture((LCD_W/2)-(180/2),(LCD_H/2)-(174/2)-15,180,174,gImage_180_174);

    ///test the led
    LED_B = 1;
    delay_ms(300);
    LED_B = 0;
    delay_ms(300);

}
///test6050单元
void test_6050()
{
    if(!stata)
        return;
    stata=0;
    if(CHeck(data_buf))
    {
        count=0;
        if(data_buf[2]&0x01) //ACC
        {
            Acc[0]=(data_buf[4]<<8)|data_buf[5];
            Acc[1]=(data_buf[6]<<8)|data_buf[7];
            Acc[2]=(data_buf[8]<<8)|data_buf[9];
            count=6;
        }
        if(data_buf[2]&0x02) //GYRO
        {
            Gyr[0]=(data_buf[4+count]<<8)|data_buf[5+count];
            Gyr[1]=(data_buf[6+count]<<8)|data_buf[7+count];
            Gyr[2]=(data_buf[8+count]<<8)|data_buf[9+count];
            count+=6;
        }
        if(data_buf[2]&0x04) //MAG
        {
            Mag[0]=(data_buf[4+count]<<8)|data_buf[5+count];
            Mag[1]=(data_buf[6+count]<<8)|data_buf[7+count];
            Mag[2]=(data_buf[8+count]<<8)|data_buf[9+count];
            count+=6;
        }

        if(data_buf[2]&0x10) //欧拉角
        {
            rpy[0]=(data_buf[4+count]<<8)|data_buf[5+count];
            rpy[1]=(data_buf[6+count]<<8)|data_buf[7+count];
            rpy[2]=(data_buf[8+count]<<8)|data_buf[9+count];

            HAL_Delay(20);
            if((abs((float) rpy[0]/100)>50)||(abs((float) rpy[1]/100)>50))
            {
                printf("RPY: %.2f,%.2f ,%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
                is_error = 1;
            }
            else{
                is_error = 0;
                printf("RPY: %.2f,%.2f ,%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
            }
            count+=6;
            if((abs((float) rpy[0]/100)>10))
            {
                if (((float) rpy[0]/100) <0)
                {

                    ///ST7789_WriteString(ST7789_WIDTH/2+38, ST7789_HEIGHT/2-75, "UP", Font_16x26, BLACK, WHITE);
                }
                else {
                    ///ST7789_WriteString(ST7789_WIDTH / 2 + 38, ST7789_HEIGHT / 2 - 75, "DOWN", Font_16x26, BLACK,WHITE);
                }
            }
        }
        if(data_buf[2]&0x40) //温度
        {
            Temp=(data_buf[4+count]<<8)|data_buf[5+count];
            printf(" ,Temp: %.2f ℃ \r\n",(float) Temp/100);
            if ((float) Temp/100 > 34 && is_error == 0)
            {
                /// ST7789_WriteString(ST7789_WIDTH/2-90, ST7789_HEIGHT/2-75, "HIGHT", Font_16x26, BLACK, WHITE);
            }
            else if ((float) Temp/100 <= 34 && is_error == 0)
            {
                /// ST7789_WriteString(ST7789_WIDTH/2-90, ST7789_HEIGHT/2-75, "NOR", Font_16x26, BLACK, WHITE);

            }
            count+=2;
        }
    }
}

///用于测试lcd屏幕
void test_lcd()
{
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    ///闪烁代表运行
    LED_R!=LED_R;
    LCD_ShowChinese(50,0,"广东工业大学",RED,WHITE,16,0);
    LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
    LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
    LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
    LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
    LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
    LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
    LCD_ShowFloatNum1(128,70,30,4,RED,WHITE,16);
    delay_ms(1000);
}

///用于与上位机联系
void The_comcupter_comunicate()
{
        if(USART_RX_STA&0x8000)
    {
        len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
        printf("\r\n您发送的消息为:\r\n");
        HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
        while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
        printf("\r\n\r\n");//插入换行
        USART_RX_STA=0;
    }else
    {
        times++;
        if(times%5000==0)
        {
            printf("\r\nALIENTEK 精英STM32开发板 串口实验\r\n");
            printf("正点原子@ALIENTEK\r\n\r\n\r\n");
        }
        if(times%200==0)
            printf("请输入数据,以回车键结束\r\n");
        if(times%30==0)
            HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);//闪烁LED,提示系统正在运行.
        delay_ms(20);
    }
}

