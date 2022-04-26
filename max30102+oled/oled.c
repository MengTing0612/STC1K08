#include "oled.h"
#include "I2C.h"
#include "intrins.h"
#include "oledfont.h"

////IIC��ʼ
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

////IIC����
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

////IIC����Ӧ��
//void IIC_Wait_Ack()
//{
//	OLED_SCLK_Set();//ʱ�����øߣ���������
//	_nop_();_nop_();_nop_();   
//	OLED_SCLK_Clr();
//	_nop_();_nop_();_nop_();   
//}

////IICд��һ���ֽ�
//void Write_IIC_Byte(unsigned char IIC_Byte)
//{
//	unsigned char i;
//	unsigned char m,da;
//	da=IIC_Byte;
//	_nop_();_nop_();_nop_();   
//	OLED_SCLK_Clr();	//ʱ�����õ�
//	_nop_();_nop_();_nop_();   
//	for(i=0;i<8;i++)		
//	{
//		m=da;
//		m=m&0x80;//���ݴӸ�λ��ʼ���ͺͽ���
//		if(m==0x80)
//			OLED_SDIN_Set();
//		else 
//			OLED_SDIN_Clr();
//		_nop_();_nop_();_nop_();   
//		da=da<<1;
//		OLED_SCLK_Set();//ʱ�����øߣ���ʼ��������
//		_nop_();_nop_();_nop_();   
//		OLED_SCLK_Clr();
//		_nop_();_nop_();_nop_();   
//	}
//}

//oledд����
void Write_IIC_Command(unsigned char IIC_Command)
{
	EAXSFR();
	Start();
	SendData(0x78);           //Slave address,SA0=0(D/C#=0; R/W#=0)
	RecvACK();	
	SendData(0x00);			//Contrl byte   Co("0")+D/C#("0")+6��0  	������һ��Data Byte������
	RecvACK();	
	SendData(IIC_Command); 
	RecvACK();	
	Stop();
	EAXRAM();
}

//oledд����
void Write_IIC_Data(unsigned char IIC_Data)
{
	EAXSFR();
	Start();
	SendData(0x78);			//Slave address,SA0=0(D/C#=0; R/W#=0)
	RecvACK();	
	SendData(0x40);			//Contrl byte	Co("0")+D/C#("1")+6��0  	������һ��Data Byte������
	RecvACK();	
	SendData(IIC_Data);
	RecvACK();	
	Stop();
	EAXRAM();
}

//oledдָ��
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)	//cmdΪ1ʱд����		
	Write_IIC_Data(dat);
	else	//����д����
	Write_IIC_Command(dat);
}

//ͼƬ����
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xB0+m,0);		//page0-page1(����)-----B0~B7
		OLED_WR_Byte(0x00,0);		//low column start address(����)
		OLED_WR_Byte(0x10,0);		//high column start address(����)-----�ߵ��ۺϴ�00��ʼ
		for(n=0;n<128;n++)
			OLED_WR_Byte(fill_Data,1);//(����)
	}
}

//��������(x:0~127  y:0~7)
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xB0+y,OLED_CMD);				//����������
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);	//���ø�������
	OLED_WR_Byte((x&0x0f),OLED_CMD);			//���õ������� 
}   

//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON  (��ʾ��)
}

//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF (��ʾ��)
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xB0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ��---�е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ��---�иߵ�ַ   
		for(n=0;n<128;n++)
			OLED_WR_Byte(0,OLED_DATA);	   //д0����
	} //������ʾ
}

//��������,����������Ļ!!!
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~7
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 8*16---16  8*6---6 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ			
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

//m^n����
u16 oled_pow(u8 m,u8 n)
{
	u16 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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

//��ʾһ���ַ��Ŵ�
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

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
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

//��ʾ����(x:�У�y:�У�no:����)
//(x:0~127  y:0~7)
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);
	//��8��
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	//��8��
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
}

//OLED��ʼ��
void OLED_Init()
{
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off                       	�ر�oled�����ʾ
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address			  	������ʾλ��---�еͿ�ʼ��ַ ������ҳѰַģʽ��Ч��
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address			������ʾλ��---�и߿�ʼ��ַ ������ҳѰַģʽ��Ч��
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address					������ʾλ��---�п�ʼ��ַ   ������ҳѰַģʽ��Ч��
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  			������ʾ��ʼ��--RAM row 0 is mapped to COM0
	
	OLED_WR_Byte(0x81,OLED_CMD); // contract control					���öԱȶ�(˫�ֽ�)
	OLED_WR_Byte(0xFF,OLED_CMD);//--256   								�Աȶ�Ϊ256
	
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 					���ö���ӳ�䣺0xa0���ҷ��� 0xa1����
		
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse					��������/������ʾΪ����(a6-����/a7-����)
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)		*���ø�����
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/64 duty							*������ֵΪ63+1
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction					����ɨ�跽��(�븴��������ϵ)---0xc0���·��� 0xc8����
	
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset					������ʾƫ��
	OLED_WR_Byte(0x00,OLED_CMD);//										ֵΪ0
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division						������ʾʱ�ӷ�Ƶ��				
	OLED_WR_Byte(0x80,OLED_CMD);//										֡Ƶ
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off				δ֪
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period					����Ԥ�����
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion				���� COM ����Ӳ������
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh								���� VCOMH ȡ��ѡ���ƽ
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable				δ֪
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel					��oled�����ʾ
	
}