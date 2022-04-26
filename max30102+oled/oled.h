/*==================================================
注意：此处的4线SPI实际上只有3个SPI线：SCL,MOSI,SS 
多出的线为DC:数据命令切换线
并且因为该oled液晶屏不需要向主机发送数据，故无MISO
==================================================*/



#ifndef __OLED_H__
#define __OLED_H__ 

#include	"config.h"
//typedef unsigned char u8;
//typedef unsigned int u16;

//引脚命名
//sbit OLED_SCLK=P1^4;	// SCL
//sbit OLED_SDIN=P1^3;	// SDA
extern const unsigned char code F6x8[][6];
extern const unsigned char code F8X16[];
extern unsigned char code Hzk[][16];
//数据-命令切换
#define OLED_CMD  0		//写命令
#define OLED_DATA 1		//写数据

//控制命令
#define OLED_SCLK_Clr() OLED_SCLK=0	//时钟置低
#define OLED_SCLK_Set() OLED_SCLK=1	//时钟置高

#define OLED_SDIN_Clr() OLED_SDIN=0	//数据低
#define OLED_SDIN_Set() OLED_SDIN=1	//数据高


//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    	

//OLED控制用函数
void OLED_Init();						//OLED初始化

void fill_picture(unsigned char fill_Data);//图片填入
   
void OLED_Display_On(void);//开启OLED显示 
     
void OLED_Display_Off(void);//关闭OLED显示

void OLED_Clear(void);//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!

void OLED_On(void);//点亮函数,点亮整个屏幕!!!

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);//在指定位置显示一个字符,包括部分字符

void OLED_ShowNum(u8 x,u8 y,u16 num,u8 len,u8 size2);//显示2个数字

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);//显示一个字符号串

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);//显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7

void OLED_ShowCHinese(u8 x,u8 y,u8 no);	//显示汉字(x:列，y:行，no:数字)


#endif
