/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include	"config.h"
#include	"GPIO.h"
#include	"delay.h"
#include  "STC8G_H_Soft_I2C.h"
#include  "UART.h"
#include  "Max30102.h"
#include  "oled.h"
#include  "I2C.h"
#include  "adc.h"
#include  "stdio.h"
#include  "string.h"
/***************	����˵��	****************

�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

����ʹ��P6������ʾ����ƣ������������

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/
u8 code ledNum[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //ֱ�Ӵ����ڳ���洢����


/*************	���ر�������	**************/
unsigned long Red=0;
unsigned long Ir=0;
u8 packet0[7];
u8 packet1[7];
unsigned char Heart=88;
unsigned char Spo2=97;
u16 over_load_count = 0; //�������
u16 adc_catch_count = 0; //ADC�ɼ�����
float avgADC0 = 0 ; //ADC��ֵ
float avgADC1 = 0 ; //ADC��ֵ
float avgADC2 = 0 ; //ADC��ֵ
float avgADC3 = 0 ; //ADC��ֵ
char strTemp0[8];
char strTemp1[8];
char strTemp2[8];
char strTemp3[8];
u8 TX_data[7]=0;
u8 timeIsOver = 0;

/*************	���غ�������	**************/
void cratepacket0()
{
	 packet0[0]=(unsigned char)(48+(Red/1000000)%10);
	 packet0[1]=(unsigned char)(48+(Red/100000)%10);
	 packet0[2]=(unsigned char)(48+(Red/10000)%10);
	 packet0[3]=(unsigned char)(48+(Red/1000)%10);
	 packet0[4]=(unsigned char)(48+(Red/100)%10);
	 packet0[5]=(unsigned char)(48+(Red/10)%10);
	 packet0[6]=(unsigned char)(48+(Red%10));
}
void cratepacket1()
{
   packet1[0]=(unsigned char)(48+(Ir/1000000)%10);
	 packet1[1]=(unsigned char)(48+(Ir/100000)%10);
	 packet1[2]=(unsigned char)(48+(Ir/10000)%10);
	 packet1[3]=(unsigned char)(48+(Ir/1000)%10);
	 packet1[4]=(unsigned char)(48+(Ir/100)%10);
	 packet1[5]=(unsigned char)(48+(Ir/10)%10);
	 packet1[6]=(unsigned char)(48+(Ir%10));
}
 
void GetADC()
{
  u8 k;
	for(k=0;k<5;k++)
	{
	avgADC0 += Get_ADCResult(2);
	avgADC1 += Get_ADCResult(3);
	avgADC2 += Get_ADCResult(6);
	avgADC3 += Get_ADCResult(7);
	}
	avgADC0 /= 5;
	avgADC1 /= 5;
	avgADC2 /= 5;
	avgADC3 /= 5;
	
	memset(strTemp0, 0, sizeof(strTemp0));
	memset(strTemp1, 0, sizeof(strTemp1));
	memset(strTemp2, 0, sizeof(strTemp2));
	memset(strTemp3, 0, sizeof(strTemp3));

	sprintf(strTemp0,"%.04f", avgADC0);//��������ת���ַ���
	sprintf(strTemp1,"%.04f", avgADC1);//��������ת���ַ���
	sprintf(strTemp2,"%.04f", avgADC2);//��������ת���ַ���
	sprintf(strTemp3,"%.04f", avgADC3);//��������ת���ַ���
	

}

void SendADC()
{
	u8 j;
  //Aͨ��	
	TX_data[0]=strTemp0[0];
	TX_data[1]=strTemp0[1];
	TX_data[2]=strTemp0[2];
	TX_data[3]=strTemp0[3];
	TX_data[4]=strTemp0[4];
	TX_data[5]=strTemp0[5];
	TX_data[6]=strTemp0[6];
	TX_data[7]=strTemp0[7];
	
	PrintString1("A:");
	for(j=0;j<8;j++)
	{
		TX1_write2buff(TX_data[j]);       //ͨ�����ڷ���PC
	}
		PrintString1("\r\n");
//Bͨ��	
	TX_data[0]=strTemp1[0];
	TX_data[1]=strTemp1[1];
	TX_data[2]=strTemp1[2];
	TX_data[3]=strTemp1[3];
	TX_data[4]=strTemp1[4];
	TX_data[5]=strTemp1[5];
	TX_data[6]=strTemp1[6];
	TX_data[7]=strTemp1[7];
	
	PrintString1("B:");
	for(j=0;j<8;j++)
	{
		TX1_write2buff(TX_data[j]);       //ͨ�����ڷ���PC
	}
		PrintString1("\r\n");
//Cͨ��
  TX_data[0]=strTemp2[0];
	TX_data[1]=strTemp2[1];
	TX_data[2]=strTemp2[2];
	TX_data[3]=strTemp2[3];
	TX_data[4]=strTemp2[4];
	TX_data[5]=strTemp2[5];
	TX_data[6]=strTemp2[6];
	TX_data[7]=strTemp2[7];
	
	PrintString1("C:");
	for(j=0;j<8;j++)
	{
		TX1_write2buff(TX_data[j]);       //ͨ�����ڷ���PC
	}
		PrintString1("\r\n");
//D
	TX_data[0]=strTemp3[0];
	TX_data[1]=strTemp3[1];
	TX_data[2]=strTemp3[2];
	TX_data[3]=strTemp3[3];
	TX_data[4]=strTemp3[4];
	TX_data[5]=strTemp3[5];
	TX_data[6]=strTemp3[6];
	TX_data[7]=strTemp3[7];
	
	PrintString1("D:");
	for(j=0;j<8;j++)
	{
		TX1_write2buff(TX_data[j]);       //ͨ�����ڷ���PC
	}
		PrintString1("\r\n");
	
}	
/*************  �ⲿ�����ͱ������� *****************/


/******************** IO���ú��� **************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ��
	
	GPIO_InitStructure.Pin  = GPIO_Pin_7| GPIO_Pin_6|GPIO_Pin_2 | GPIO_Pin_3;		//ָ��Ҫ��ʼ����IO,
	GPIO_InitStructure.Mode = GPIO_HighZ;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_6 | GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
	GPIO_InitStructure.Mode = GPIO_PullUp;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{

	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = Polity_1;			//ָ���ж����ȼ�(�͵���) Polity_0,Polity_1,Polity_2,Polity_3
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = Polity_0;			//ָ���ж����ȼ�(�͵���) Polity_0,Polity_1,Polity_2,Polity_3
	COMx_InitStructure.UART_P_SW      = UART2_SW_P10_P11;	//�л��˿�,   UART2_SW_P10_P11,UART2_SW_P46_P47
	UART_Configuration(UART2, &COMx_InitStructure);		//��ʼ������2 UART1,UART2,UART3,UART4


	PrintString1("STC8 UART1 Test Programme!\r\n");	//UART1����һ���ַ���	
}

void  IIC_config(void)//���ṩ�ο�
{
   I2C_InitTypeDef  IIC_InitStructure;
	 IIC_InitStructure.I2C_Speed=54;        //�����ٶ�=Fosc/2/(Speed*2+4),      0~63
	 IIC_InitStructure.I2C_Enable=ENABLE;        //I2C����ʹ��,   ENABLE, DISABLE
   IIC_InitStructure.I2C_Mode=I2C_Mode_Master; //����ģʽѡ��,  I2C_Mode_Master,I2C_Mode_Slave
   IIC_InitStructure.I2C_MS_Interrupt= DISABLE;	//ʹ������ģʽ�ж�,  ENABLE, DISABLE
	 IIC_InitStructure.I2C_MS_WDTA=DISABLE;  //����ʹ���Զ�����,  ENABLE, DISABLE
   
   IIC_InitStructure.I2C_SL_ESTAI=DISABLE;			//�ӻ�����START�ź��ж�ʹ��,  ENABLE, DISABLE
	 IIC_InitStructure.I2C_SL_ERXI=DISABLE;			//�ӻ�����1�ֽ������ж�ʹ��,  ENABLE, DISABLE
	 IIC_InitStructure.I2C_SL_ETXI=DISABLE;			//�ӻ�����1�ֽ������ж�ʹ��,  ENABLE, DISABLE
	 IIC_InitStructure.I2C_SL_ESTOI=DISABLE;			//�ӻ�����STOP�ź��ж�ʹ��,  ENABLE, DISABLE
	
	 IIC_InitStructure.I2C_SL_ADR=0; //�ӻ��豸��ַ,  0~127
	 IIC_InitStructure.I2C_SL_MA=DISABLE;				//�ӻ��豸��ַ�Ƚ�ʹ��,  ENABLE, DISABLE

   IIC_InitStructure.I2C_IoUse=I2C_P14_P15;
	
	 I2C_Init(&IIC_InitStructure);
}

void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X1T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_Power     = ENABLE;				//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
  ADC_InitStructure.ADC_Polity = Polity_1;			//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����	ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE

}

void WIFIconfig()
{

	PrintString2("AT+CIPSTART=\"TCP\",\"192.168.43.217\",8086\r\n");   
	delay_ms(5);  
	PrintString2("AT+CIPMODE=1");	   
	delay_ms(5);
	PrintString2("AT+CIPSEND");	
}


/******************** ������ **************************/
void main(void)
{ 
	u8 i;
  GPIO_config();
	UART_config();
	IIC_config();
//	max30102_init();
	OLED_Init();
//	WIFIconfig();
	ADC_config();
	EA = 1;
	OLED_Clear();//oled���� 
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(18,0,1);
	OLED_ShowCHinese(36,0,2);
	OLED_ShowCHinese(54,0,3);
	OLED_ShowCHinese(72,0,4);
	OLED_ShowCHinese(90,0,5);
	OLED_ShowCHinese(108,0,6);
	while(1)
	{
//		MAX30102_FIFO_ReadData(&Red,&Ir);
//	  PrintString1("The Red_Number is:");
//	  cratepacket0();
//		for(i=0;i<7;i++)
//		    TX1_write2buff(packet0[i]);
//		
//		PrintString1("     ");
//		cratepacket1();
//		for(i=0;i<7;i++)
//		    TX1_write2buff(packet1[i]);
//		PrintString1("\r\n");
		GetADC();
		SendADC();
		delay_ms(10);
		
		
		
		OLED_Clear();//oled����
		OLED_ShowCHinese(0,0,0);
		OLED_ShowCHinese(18,0,1);
		OLED_ShowCHinese(36,0,2);
		OLED_ShowCHinese(54,0,3);
		OLED_ShowCHinese(72,0,4);
		OLED_ShowCHinese(90,0,5);
		OLED_ShowCHinese(108,0,6);
		
		OLED_ShowCHinese(0,3,0);//��
		OLED_ShowCHinese(18,3,1);//��
		OLED_ShowCHinese(36,3,6);//��
		
		OLED_ShowCHinese(0,6,2);//Ѫ
		OLED_ShowCHinese(18,6,3);//��
		OLED_ShowCHinese(36,6,6);//��
		
		OLED_ShowNum(40,3,Heart,3,16);
		OLED_ShowCHinese(65,3,7);
		OLED_ShowChar(82,3,'/',16);
		OLED_ShowCHinese(91,3,8);
		OLED_ShowCHinese(110,3,9);
		
		OLED_ShowNum(40,6,Spo2,3,16);
		OLED_ShowChar(65,6,'%',16);
		delay_ms(50);
	}
}

