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
///状态量
uint8_t speed = 12;
float R_of_circle = 0;

///用于存初始化后第一次的初始姿态
uint8_t the_flag_of_first = 1;
int16_t rpy_fi[3]={0},Acc_fi[3]={0},Gyr_fi[3]={0},Mag_fi[3]={0},Q_fi[4]={0},Temp_fi=0,Altitude_fi=0;


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
void The_main()
{
    LCD_DrawLine(0,70,240,70,BLACK);
    LCD_DrawLine(0,140,240,140,BLACK);
    LCD_DrawLine(0,210,240,210,BLACK);
    LCD_ShowChinese(20,70/2-32/2,"温度",BLACK,WHITE,32,0);
    LCD_ShowString(20+32*5,70/2-32/2,"C",BLACK,WHITE,32,0);
    LCD_ShowChinese(20,70/2+70-32/2,"速度",BLACK,WHITE,32,0);
    LCD_ShowString(20+32*5,70/2+70-32/2,"km/h",BLACK,WHITE,32,0);
    LCD_ShowChinese(20,70/2+140-32/2,"提示",BLACK,WHITE,32,0);
    LCD_ShowChinese(40,70/2+210-32/2,"嘉怡是天队",RED,WHITE,32,0);
    ///提示初始化提示为
    LCD_ShowChinese(20+32+32+10,70/2+140-32/2,"一路平安",BLACK,WHITE,32,0);
}
///除系统初始化外的全部初始化集合  /* Initialize all configured peripherals */
Set_R_Of_Circle()
{
    LCD_ShowChinese(20,70/2-32/2,"请输入",BLACK,WHITE,32,0);
    LCD_ShowChinese(LCD_W/2-64-16,70/2+70-32/2,"轮子的直径",BLACK,WHITE,32,0);
    LCD_ShowString(LCD_W/2+50,70/2+140-32/2,"cm",BLACK,WHITE,32,0);
    LCD_ShowString(LCD_W/2+50-3*32,70/2+140-32/2,"000",BLACK,WHITE,32,0);
    while(R_of_circle == 0);///等待用户设置,在中断处置位
    ///留坑
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
}

void The_sum_init()
{
    delay_init(64);
    MX_SPI1_Init();
    uart_init_1(115200);
    uart_init_2(115200);
    RetargetInit(&UART1_Handler);
    //LED_Init();//LED初始化会干扰串口2
    LCD_Init();//LCD初始化
    ///6050初始化
    send_Instruction();
    ///先对屏幕白化
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    ///打印校徽///内存不够的时候先注释
    ///test_command_open();
    if (R_of_circle == 0)
        Set_R_Of_Circle();
    if (R_of_circle)
        The_main();
}
void Lcd_num()
{
    static times_update = 0;
    if (times_update == 1000)
        times_update = 0;
    if (times_update % 10 == 0)
    {
        //update_data
        ///温度
        LCD_ShowIntNum(20+32+32+32,70/2-32/2,(int)Temp,2,RED,WHITE,32);
        ///速度
        LCD_ShowIntNum(20+32+32+32,70/2+70-32/2,speed,2,RED,WHITE,32);
    }
}


///用于测试单元模块
void test_command_open()
{
    LCD_ShowPicture((LCD_W/2)-(180/2),(LCD_H/2)-(174/2)-15,180,174,gImage_180_174);
    delay_ms(500);
    LCD_ShowChinese((LCD_W/2)-(3*32),(LCD_H/2)+(174/2),"广东工业大学",RED,WHITE,32,0);
    delay_ms(500);
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
}
void the_first_get()
{
    the_flag_of_first = 0;
    for (int i = 0; i < 3; ++i) {
        rpy_fi[i] = rpy[i];
        Acc_fi[i] = Acc[i];
        Gyr_fi[i] = Gyr[i];
        Mag_fi[i] = Mag_fi[i];
        Q_fi[i] = Q[i];
    }
    Q_fi[4] =  Q[4];
    Temp_fi = Temp;
    Altitude_fi = Altitude;
}
///多种危险预报 电量不足无法上行的情况下会缓慢倒溜。下坡时建议降低车速缓慢行驶
///1  :上坡超过15度就显示文字提示   8度-20度之间***
///2  :下坡超过25度就显示文字提示   容易前倾和刹车失灵***
///3  :检测电量，低于30，则提示注意上坡安全，容易动力不足
///4  :行程过远，一般用电量或者实际里程数作为标准
///5  :轮椅有左右倾倒的风险***
///6  :轮椅可能已经倾倒***
///此处有更加简洁的代码写法
uint8_t test_danger()
{
    ///坡度预警
    ///前倾是左右那一栏
    ///理想状态下 是 -90(右倾-更大，左倾-更小) 0(前倾为-，上仰为+) 第三位是转向，不用管
    if (abs((int) rpy[1]/100) >  (0+25) && rpy[1] < 0 && abs((int) rpy[1]/100) <  (0+70))///前倾
    {
        return 2;
    }
    if (abs((int) rpy[1]/100) >  (0+15) && rpy[1] > 0 && abs((int) rpy[1]/100) <  (0+70))///上仰
    {
        return 1;
    }
    if ((abs((int) rpy[0]/100) > (90+15) && abs((int) rpy[0]/100) < (90+70)) || (abs((int) rpy[0]/100) < (90-15) && abs((int) rpy[0]/100) > (15))  )
    {
        return 5;
    }
    if (abs((int) rpy[0]/100) > (90+70)|| abs((int) rpy[0]/100) < (15) || abs((int) rpy[1]/100) >  (0+70))
    {
        return 6;
    }
}
///发生危险后，与外界的交流
void danger_call()
{
    ///蜂鸣器常响

}
void tip_lcd(int tip)
{
//    switch (tip) {
//        case 5:
//
//    }
}

void danger_reply()
{
    switch (test_danger()) {
        case 5:
            printf("轮椅有倾倒的风险");
            ///lcd同步显示
            break;
        case 6:
            printf("轮椅可能已经发生危险");
            break;
        case 1:
            printf("请注意上坡坡度");
            break;
        case 2:
            printf("请注意下坡坡度");
    }
}
void use_6050()
{
    if(!stata_6050)
        return;
    stata_6050=0;
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
            printf("左右: %.2f,前后: %.2f ,上下:%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
            count+=6;
        }

        if(data_buf[2]&0x40) //温度
        {
            Temp=(data_buf[4+count]<<8)|data_buf[5+count];
            printf(" ,Temp: %.2f ℃ \r\n",(float) Temp/100);
            count+=2;
        }
        if (the_flag_of_first <= 5)
            the_flag_of_first++;
        if (the_flag_of_first == 4)
            the_first_get();
    }
}
///test6050单元
void test_6050()
{
    if(!stata_6050)
        return;
    stata_6050=0;
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
            printf("RPY: %.2f,%.2f ,%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
            count+=6;
        }

        if(data_buf[2]&0x40) //温度
        {
            Temp=(data_buf[4+count]<<8)|data_buf[5+count];
            printf(" ,Temp: %.2f ℃ \r\n",(float) Temp/100);
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

