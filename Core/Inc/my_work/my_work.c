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
///״̬��
uint8_t speed = 12;
float R_of_circle = 0;

///���ڴ��ʼ�����һ�εĳ�ʼ��̬
uint8_t the_flag_of_first = 1;
int16_t rpy_fi[3]={0},Acc_fi[3]={0},Gyr_fi[3]={0},Mag_fi[3]={0},Q_fi[4]={0},Temp_fi=0,Altitude_fi=0;


///6050ѭ�������õ���
uint8_t data_buf[50]={0},count=0;
int16_t ROLL=0,PITCH=0,YAW=0;
int16_t rpy[3]={0},Acc[3]={0},Gyr[3]={0},Mag[3]={0},Q[4]={0},Temp=0,Altitude=0;
uint32_t pressure=0;

void send_Instruction(void)
{
    ///���͵�����Ӧ���� a5 55 50 4a a5 56 02 fd
    uint8_t send_data[4]={0};
    send_data[0]=0xa5;
    send_data[1]=0x55;
    send_data[2]=0x50;
    send_data[3]=0x4a;
    USART_Send_bytes(send_data,4);//�����¶�/ŷ�������ָ��

    delay_ms(100);

    send_data[0]=0xa5;
    send_data[1]=0x56;
    send_data[2]=0x02;
    send_data[3]=0xfd;
    USART_Send_bytes(send_data,4);//�����Զ����ָ��
    delay_ms(100);
}
void The_main()
{
    LCD_DrawLine(0,70,240,70,BLACK);
    LCD_DrawLine(0,140,240,140,BLACK);
    LCD_DrawLine(0,210,240,210,BLACK);
    LCD_ShowChinese(20,70/2-32/2,"�¶�",BLACK,WHITE,32,0);
    LCD_ShowString(20+32*5,70/2-32/2,"C",BLACK,WHITE,32,0);
    LCD_ShowChinese(20,70/2+70-32/2,"�ٶ�",BLACK,WHITE,32,0);
    LCD_ShowString(20+32*5,70/2+70-32/2,"km/h",BLACK,WHITE,32,0);
    LCD_ShowChinese(20,70/2+140-32/2,"��ʾ",BLACK,WHITE,32,0);
    LCD_ShowChinese(40,70/2+210-32/2,"���������",RED,WHITE,32,0);
    ///��ʾ��ʼ����ʾΪ
    LCD_ShowChinese(20+32+32+10,70/2+140-32/2,"һ·ƽ��",BLACK,WHITE,32,0);
}
///��ϵͳ��ʼ�����ȫ����ʼ������  /* Initialize all configured peripherals */
Set_R_Of_Circle()
{
    LCD_ShowChinese(20,70/2-32/2,"������",BLACK,WHITE,32,0);
    LCD_ShowChinese(LCD_W/2-64-16,70/2+70-32/2,"���ӵ�ֱ��",BLACK,WHITE,32,0);
    LCD_ShowString(LCD_W/2+50,70/2+140-32/2,"cm",BLACK,WHITE,32,0);
    LCD_ShowString(LCD_W/2+50-3*32,70/2+140-32/2,"000",BLACK,WHITE,32,0);
    while(R_of_circle == 0);///�ȴ��û�����,���жϴ���λ
    ///����
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
}

void The_sum_init()
{
    delay_init(64);
    MX_SPI1_Init();
    uart_init_1(115200);
    uart_init_2(115200);
    RetargetInit(&UART1_Handler);
    //LED_Init();//LED��ʼ������Ŵ���2
    LCD_Init();//LCD��ʼ��
    ///6050��ʼ��
    send_Instruction();
    ///�ȶ���Ļ�׻�
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    ///��ӡУ��///�ڴ治����ʱ����ע��
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
        ///�¶�
        LCD_ShowIntNum(20+32+32+32,70/2-32/2,(int)Temp,2,RED,WHITE,32);
        ///�ٶ�
        LCD_ShowIntNum(20+32+32+32,70/2+70-32/2,speed,2,RED,WHITE,32);
    }
}


///���ڲ��Ե�Ԫģ��
void test_command_open()
{
    LCD_ShowPicture((LCD_W/2)-(180/2),(LCD_H/2)-(174/2)-15,180,174,gImage_180_174);
    delay_ms(500);
    LCD_ShowChinese((LCD_W/2)-(3*32),(LCD_H/2)+(174/2),"�㶫��ҵ��ѧ",RED,WHITE,32,0);
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
///����Σ��Ԥ�� ���������޷����е�����»Ỻ���������ʱ���齵�ͳ��ٻ�����ʻ
///1  :���³���15�Ⱦ���ʾ������ʾ   8��-20��֮��***
///2  :���³���25�Ⱦ���ʾ������ʾ   ����ǰ���ɲ��ʧ��***
///3  :������������30������ʾע�����°�ȫ�����׶�������
///4  :�г̹�Զ��һ���õ�������ʵ���������Ϊ��׼
///5  :�����������㵹�ķ���***
///6  :���ο����Ѿ��㵹***
///�˴��и��Ӽ��Ĵ���д��
uint8_t test_danger()
{
    ///�¶�Ԥ��
    ///ǰ����������һ��
    ///����״̬�� �� -90(����-��������-��С) 0(ǰ��Ϊ-������Ϊ+) ����λ��ת�򣬲��ù�
    if (abs((int) rpy[1]/100) >  (0+25) && rpy[1] < 0 && abs((int) rpy[1]/100) <  (0+70))///ǰ��
    {
        return 2;
    }
    if (abs((int) rpy[1]/100) >  (0+15) && rpy[1] > 0 && abs((int) rpy[1]/100) <  (0+70))///����
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
///����Σ�պ������Ľ���
void danger_call()
{
    ///����������

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
            printf("�������㵹�ķ���");
            ///lcdͬ����ʾ
            break;
        case 6:
            printf("���ο����Ѿ�����Σ��");
            break;
        case 1:
            printf("��ע�������¶�");
            break;
        case 2:
            printf("��ע�������¶�");
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

        if(data_buf[2]&0x10) //ŷ����
        {
            rpy[0]=(data_buf[4+count]<<8)|data_buf[5+count];
            rpy[1]=(data_buf[6+count]<<8)|data_buf[7+count];
            rpy[2]=(data_buf[8+count]<<8)|data_buf[9+count];
            printf("����: %.2f,ǰ��: %.2f ,����:%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
            count+=6;
        }

        if(data_buf[2]&0x40) //�¶�
        {
            Temp=(data_buf[4+count]<<8)|data_buf[5+count];
            printf(" ,Temp: %.2f �� \r\n",(float) Temp/100);
            count+=2;
        }
        if (the_flag_of_first <= 5)
            the_flag_of_first++;
        if (the_flag_of_first == 4)
            the_first_get();
    }
}
///test6050��Ԫ
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

        if(data_buf[2]&0x10) //ŷ����
        {
            rpy[0]=(data_buf[4+count]<<8)|data_buf[5+count];
            rpy[1]=(data_buf[6+count]<<8)|data_buf[7+count];
            rpy[2]=(data_buf[8+count]<<8)|data_buf[9+count];
            printf("RPY: %.2f,%.2f ,%.2f ",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
            count+=6;
        }

        if(data_buf[2]&0x40) //�¶�
        {
            Temp=(data_buf[4+count]<<8)|data_buf[5+count];
            printf(" ,Temp: %.2f �� \r\n",(float) Temp/100);
            count+=2;
        }


    }
}

///���ڲ���lcd��Ļ
void test_lcd()
{
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    ///��˸��������
    LED_R!=LED_R;
    LCD_ShowChinese(50,0,"�㶫��ҵ��ѧ",RED,WHITE,16,0);
    LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
    LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
    LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
    LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
    LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
    LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
    LCD_ShowFloatNum1(128,70,30,4,RED,WHITE,16);
    delay_ms(1000);
}

///��������λ����ϵ
void The_comcupter_comunicate()
{
        if(USART_RX_STA&0x8000)
    {
        len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
        printf("\r\n�����͵���ϢΪ:\r\n");
        HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
        while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
        printf("\r\n\r\n");//���뻻��
        USART_RX_STA=0;
    }else
    {
        times++;
        if(times%5000==0)
        {
            printf("\r\nALIENTEK ��ӢSTM32������ ����ʵ��\r\n");
            printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
        }
        if(times%200==0)
            printf("����������,�Իس�������\r\n");
        if(times%30==0)
            HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);//��˸LED,��ʾϵͳ��������.
        delay_ms(20);
    }
}

