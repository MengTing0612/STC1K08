#include "oled.h"
#include "I2C.h"
#include "intrins.h"
#include "oledfont.h"

////IIC开始
//void IIC_Start()
//{
//	OLED_SCLK_Set();
//	_nop_();_nop_();_nop_();   
//	OLED_SDIN_Set();
//	_nop_();_nop_();_nop_();   
//	OLED_SDIN_Clr();
//	_nop_();_nop_();_nop_();   
//	OLED_SCLK_Clr();
//	_nop_();_nop_();_nop_();   
//}

////IIC结束
//void IIC_Stop()
//{
//	OLED_SCLK_Set() ;
//	_nop_();_nop_();_nop_();   
//	OLED_SDIN_Clr();
//	_nop_();_nop_();_nop_();   
//	OLED_SDIN_Set();
//	_nop_();_nop_();_nop_();   
//	
//}

////IIC接收应答
//void IIC_Wait_Ack()
//{
//	OLED_SCLK_Set();//时钟线置高，接收数据
//	_nop_();_nop_();_nop_();   
//	OLED_SCLK_Clr();
//	_nop_();_nop_();_nop_();   
//}

////IIC写入一个字节
//void Write_IIC_Byte(unsigned char IIC_Byte)
//{
//	unsigned char i;
//	unsigned char m,da;
//	da=IIC_Byte;
//	_nop_();_nop_();_nop_();   
//	OLED_SCLK_Clr();	//时钟线置低
//	_nop_();_nop_();_nop_();   
//	for(i=0;i<8;i++)		
//	{
//		m=da;
//		m=m&0x80;//数据从高位开始发送和接收
//		if(m==0x80)
//			OLED_SDIN_Set();
//		else 
//			OLED_SDIN_Clr();
//		_nop_();_nop_();_nop_();   
//		da=da<<1;
//		OLED_SCLK_Set();//时钟线置高，开始接收数据
//		_nop_();_nop_();_nop_();   
//		OLED_SCLK_Clr();
//		_nop_();_nop_();_nop_();   
//	}
//}

//oled写命令
void Write_IIC_Command(unsigned char IIC_Command)
{
	EAXSFR();
	Start();
	SendData(0x78);           //Slave address,SA0=0(D/C#=0; R/W#=0)
	RecvACK();	
	SendData(0x00);			//Contrl byte   Co("0")+D/C#("0")+6个0  	控制下一个Data Byte是命令
	RecvACK();	
	SendData(IIC_Command); 
	RecvACK();	
	Stop();
	EAXRAM();
}

//oled写数据
void Write_IIC_Data(unsigned char IIC_Data)
{
	EAXSFR();
	Start();
	SendData(0x78);			//Slave address,SA0=0(D/C#=0; R/W#=0)
	RecvACK();	
	SendData(0x40);			//Contrl byte	Co("0")+D/C#("1")+6个0  	控制下一个Data Byte是数据
	RecvACK();	
	SendData(IIC_Data);
	RecvACK();	
	Stop();
	EAXRAM();
}

//oled写指令
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)	//cmd为1时写数据		
	Write_IIC_Data(dat);
	else	//否者写命令
	Write_IIC_Command(dat);
}

//图片填入
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xB0+m,0);		//page0-page1(命令)-----B0~B7
		OLED_WR_Byte(0x00,0);		//low column start address(命令)
		OLED_WR_Byte(0x10,0);		//high column start address(命令)-----高低综合从00开始
		for(n=0;n<128;n++)
			OLED_WR_Byte(fill_Data,1);//(数据)
	}
}

//坐标设置(x:0~127  y:0~7)
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xB0+y,OLED_CMD);				//设置行坐标
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);	//设置高行坐标
	OLED_WR_Byte((x&0x0f),OLED_CMD);			//设置低行坐标 
}   

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON  (显示开)
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF (显示关)
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xB0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置---列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置---列高地址   
		for(n=0;n<128;n++)
			OLED_WR_Byte(0,OLED_DATA);	   //写0清屏
	} //更新显示
}

//点亮函数,点亮整个屏幕!!!
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~7
//mode:0,反白显示;1,正常显示				 
//size:选择字体 8*16---16  8*6---6 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
//	if(x>Max_Column-1)
//		x=0;y=y+2;
	
	//8*16
	if(Char_Size==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	
	//8*6
	else 
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
	}
}

//m^n函数
u16 oled_pow(u8 m,u8 n)
{
	u16 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u16 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120){x=0;y+=2;}
		j++;
	}
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;
  
	if(y1%8==0) 
		y=y1/8;      
	else 
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//显示汉字(x:列，y:行，no:汉字)
//(x:0~127  y:0~7)
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);
	//上8个
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	//下8个
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
}

//OLED初始化
void OLED_Init()
{
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off                       	关闭oled面板显示
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address			  	设置显示位置---列低开始地址 （仅在页寻址模式有效）
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address			设置显示位置---列高开始地址 （仅在页寻址模式有效）
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address					设置显示位置---行开始地址   （仅在页寻址模式有效）
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  			设置显示起始行--RAM row 0 is mapped to COM0
	
	OLED_WR_Byte(0x81,OLED_CMD); // contract control					设置对比度(双字节)
	OLED_WR_Byte(0xFF,OLED_CMD);//--256   								对比度为256
	
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 					设置段重映射：0xa0左右反置 0xa1正常
		
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse					设置正常/反向显示为正常(a6-正常/a7-反向)
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)		*设置复用率
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/64 duty							*复用率值为63+1
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction					设置扫描方向(与复用率相联系)---0xc0上下反置 0xc8正常
	
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset					设置显示偏移
	OLED_WR_Byte(0x00,OLED_CMD);//										值为0
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division						设置显示时钟分频比				
	OLED_WR_Byte(0x80,OLED_CMD);//										帧频
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off				未知
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period					设置预充电期
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion				设置 COM 引脚硬件配置
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh								设置 VCOMH 取消选择电平
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable				未知
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel					打开oled面板显示
	
}