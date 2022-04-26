/*==================================================
ע�⣺�˴���4��SPIʵ����ֻ��3��SPI�ߣ�SCL,MOSI,SS 
�������ΪDC:���������л���
������Ϊ��oledҺ��������Ҫ�������������ݣ�����MISO
==================================================*/



#ifndef __OLED_H__
#define __OLED_H__ 

#include	"config.h"
//typedef unsigned char u8;
//typedef unsigned int u16;

//��������
//sbit OLED_SCLK=P1^4;	// SCL
//sbit OLED_SDIN=P1^3;	// SDA
extern const unsigned char code F6x8[][6];
extern const unsigned char code F8X16[];
extern unsigned char code Hzk[][16];
//����-�����л�
#define OLED_CMD  0		//д����
#define OLED_DATA 1		//д����

//��������
#define OLED_SCLK_Clr() OLED_SCLK=0	//ʱ���õ�
#define OLED_SCLK_Set() OLED_SCLK=1	//ʱ���ø�

#define OLED_SDIN_Clr() OLED_SDIN=0	//���ݵ�
#define OLED_SDIN_Set() OLED_SDIN=1	//���ݸ�


//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    	

//OLED�����ú���
void OLED_Init();						//OLED��ʼ��

void fill_picture(unsigned char fill_Data);//ͼƬ����
   
void OLED_Display_On(void);//����OLED��ʾ 
     
void OLED_Display_Off(void);//�ر�OLED��ʾ

void OLED_Clear(void);//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!

void OLED_On(void);//��������,����������Ļ!!!

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);//��ָ��λ����ʾһ���ַ�,���������ַ�

void OLED_ShowNum(u8 x,u8 y,u16 num,u8 len,u8 size2);//��ʾ2������

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);//��ʾһ���ַ��Ŵ�

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);//��ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7

void OLED_ShowCHinese(u8 x,u8 y,u8 no);	//��ʾ����(x:�У�y:�У�no:����)


#endif
