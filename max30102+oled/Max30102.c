//#include "IIC.h"
#include  "STC8G_H_Soft_I2C.h"
#include "Max30102.h"
#include "delay.h"


//���Ӻ���
void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char Data)
{				   	  	    																 
  I2C_Start();  
	I2C_WriteAbyte(daddr);	    //����д����
	I2C_Check_ACK();
	I2C_WriteAbyte(addr);//���͵�ַ
	I2C_Check_ACK();							  		   
	I2C_WriteAbyte(Data);     //�����ֽ�							   
	I2C_Check_ACK();  		    	   
  I2C_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
void IIC_Read_One_Byte(unsigned char daddr,unsigned char addr,unsigned char *Data)
{
	I2C_Start();  
	I2C_WriteAbyte(daddr);	    //����д����
	I2C_Check_ACK();
	I2C_WriteAbyte(addr);//���͵�ַ
	I2C_Check_ACK();		
  I2C_Start();  
  I2C_WriteAbyte(daddr|0x01);
	I2C_Check_ACK();
  *Data=I2C_ReadAbyte();
	S_NoACK();
	I2C_Stop();//����һ��ֹͣ���� 
	
}	
void IIC_ReadBytes(unsigned char deviceAddr,unsigned char writeAddr,unsigned char *Data,unsigned char dataLength)
{		
	unsigned char i;	
  I2C_Start();  
	I2C_WriteAbyte(deviceAddr);	    //����д����
	I2C_Check_ACK();
	I2C_WriteAbyte(writeAddr);
	I2C_Check_ACK();
	I2C_WriteAbyte(deviceAddr|0X01);//�������ģʽ			   
	I2C_Check_ACK();
	for(i=0;i<dataLength-1;i++)
	{
		Data[i] = I2C_ReadAbyte();
		S_ACK();
	}		
	Data[dataLength-1] = I2C_ReadAbyte();
  S_NoACK();
  I2C_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	
}


//MAX30102д�Ĵ�������
void max30102_Bus_Write(unsigned char Register_Address,unsigned char Word_Data)
{
	/* ��1��������I2C���������ź� */
	I2C_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
  I2C_Check_ACK();//IIC����Ӧ��
	/* ��4���������ֽڵ�ַ */
	I2C_WriteAbyte(Register_Address);
	/* ��5��������ACK */
	I2C_Check_ACK();//IIC����Ӧ��
	/* ��6������ʼд������ */
	I2C_WriteAbyte(Word_Data);
  /* ��7��������ACK */
	I2C_Check_ACK();//IIC����Ӧ��
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();//IIC����
}

//MAX30102���Ĵ�������
unsigned char max30102_Bus_Read(unsigned char Register_Address)
{
	unsigned char Data;
	/* ��1��������I2C���������ź� */
	I2C_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
  I2C_Check_ACK();
	/* ��4���������ֽڵ�ַ�� */
	I2C_WriteAbyte(Register_Address);
	/* ��5��������ACK */
  I2C_Check_ACK();
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	I2C_Start();
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	/* ��8��������ACK */
   I2C_Check_ACK();
	/* ��9������ȡ���� */
	Data = I2C_ReadAbyte();	/* ��1���ֽ� */
	/* ��10��������ACK */
  S_NoACK();
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();//IIC����
	return Data;	/* ִ�гɹ� ����dataֵ */
}

//MAX30102��������FIFO Words
void max30102_FIFO_ReadWords(unsigned char Register_Address,unsigned int Word_Data[][2],unsigned char count)
{
	unsigned char i=0;
	unsigned char no = count;
	unsigned char data1, data2;
	/* ��1��������I2C���������ź� */
	I2C_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
  I2C_Check_ACK();
	/* ��4���������ֽڵ�ַ�� */
	I2C_WriteAbyte(Register_Address);
	/* ��5��������ACK */
   I2C_Check_ACK();
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	I2C_Start();
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	/* ��8��������ACK */
  I2C_Check_ACK();
	/* ��9������ȡ���� */
	while (no)
	{
		data1 = I2C_ReadAbyte();
		S_ACK();
		data2 = I2C_ReadAbyte();
		S_ACK();
		Word_Data[i][0] = (((unsigned int)data1 << 8) | data2);  //
		data1 = I2C_ReadAbyte();
		S_ACK();
		data2 = I2C_ReadAbyte();
		if(1==no)
			S_NoACK();/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		else
			S_ACK();
		Word_Data[i][1] = (((unsigned int)data1 << 8) | data2); 
		no--;	
		i++;
	}
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();//IIC����
}

//MAX30102��������FIFO Bytes
void max30102_FIFO_ReadBytes(unsigned char Register_Address,unsigned char *Data)
{
	max30102_Bus_Read(REG_INTR_STATUS_1);//�������Ѫ���ж�
	max30102_Bus_Read(REG_INTR_STATUS_2);//����¶��ж�
	/* ��1��������I2C���������ź� */
	I2C_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
  I2C_Check_ACK();
	/* ��4���������ֽڵ�ַ�� */
	I2C_WriteAbyte(Register_Address);
	/* ��5��������ACK */
  I2C_Check_ACK();
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	I2C_Start();
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	/* ��8��������ACK */
   I2C_Check_ACK();
	/* ��9������ȡ���� */
	Data[0]=I2C_ReadAbyte();
	S_ACK();
	Data[1]=I2C_ReadAbyte();
	S_ACK();
	Data[2]=I2C_ReadAbyte();
	S_ACK();
	Data[3]=I2C_ReadAbyte();
	S_ACK();
	Data[4]=I2C_ReadAbyte();
	S_ACK();
	Data[5]=I2C_ReadAbyte();
	S_NoACK();
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();//IIC����
	
}

//MAX30102��������FIFO Data
void MAX30102_FIFO_ReadData(unsigned long *Red,unsigned long *Ir)
{
	unsigned char tem[6];
	unsigned long un_temp=0;
	*Red=0;
	*Ir=0;
	max30102_Bus_Read(REG_INTR_STATUS_1);//�������Ѫ���ж�
	max30102_Bus_Read(REG_INTR_STATUS_2);//����¶��ж�
	/* ��1��������I2C���������ź� */
	I2C_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
	/* ��3��������ACK */
  I2C_Check_ACK();
	/* ��4���������ֽڵ�ַ�� */
	I2C_WriteAbyte(REG_FIFO_DATA);
	/* ��5��������ACK */
  I2C_Check_ACK();
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	I2C_Start();
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	/* ��8��������ACK */
  I2C_Check_ACK();
	/* ��9������ȡ���� */
	tem[0]=I2C_ReadAbyte();
	S_ACK();
	tem[1]=I2C_ReadAbyte();
  S_ACK();
	tem[2]=I2C_ReadAbyte();
	S_ACK();
	tem[3]=I2C_ReadAbyte();
	S_ACK();
	tem[4]=I2C_ReadAbyte();
	S_ACK();
	tem[5]=I2C_ReadAbyte();
	S_NoACK();
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();//IIC����
	
	un_temp=tem[0];
  un_temp<<=16;
  *Red+=un_temp;	
  un_temp=tem[1];
  un_temp<<=8;
  *Red+=un_temp;	
  un_temp=tem[2];
  *Red+=un_temp;
  
  un_temp=tem[3];
  un_temp<<=16;
  *Ir+=un_temp;	
  un_temp=tem[4];
  un_temp<<=8;
  *Ir+=un_temp;	
  un_temp=tem[5];
  *Ir+=un_temp;
	
  *Red&=0x03FFFF;  //Mask MSB [23:18]
  *Ir&=0x03FFFF;  //Mask MSB [23:18]
}

void max30102_reset()
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}

//MAX30102��ʼ�����ɸ����Լ���ʵ�����������Ӧ�����ã�
void max30102_init()
{
	
	max30102_reset();//max30102��λ
	
	max30102_Bus_Write(REG_INTR_ENABLE_1,0xC0);	//  Interrupt Enable  02->C0
	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);	//  Interrupt Enable  03->00
	
	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//              FIFO  04->00       FIFO_WR_PTR[4:0] FIFOдָ��ָ����һ��Ҫд���ڴ��ַ
	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//              FIFO  05->00       OVF_COUNTER[4:0] FIFO�����ʧ���ݼ���	
	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//              FIFO  06->00       FIFO_RD_PTR[4:0] FIFO��ָ��ָ����һ��Ҫ�����ڴ��ַ
	
	max30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//FIFO Configuration  08->0f       sample avg = 1, fifo rollover=false, fifo almost full = 17
	max30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//Mode Configuration  09->03       0x02 for Red only, 0x03 for SpO2 mode, 0x07 multimode LED
	max30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	//SpO2 Configuration  0A->27       SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  0x0A
	max30102_Bus_Write(REG_LED1_PA,0x24);   	  //LED Pulse Amplitude 0C->7F       Choose value for ~ 7.0mA for LED1
	max30102_Bus_Write(REG_LED2_PA,0x24);   	  //LED Pulse Amplitude 0D->3F       Choose value for ~ 7.0mA for LED2
	max30102_Bus_Write(REG_PILOT_PA,0x7F);   	  //LED Pulse Amplitude 10->7F       Choose value for ~ 25mA for Pilot LED					
}

void maxim_max30102_write_reg(unsigned char uch_addr,unsigned char uch_data)
{
	IIC_Write_One_Byte(I2C_WRITE_ADDR, uch_addr, uch_data);
}

void maxim_max30102_read_reg(unsigned char uch_addr, unsigned char *puch_data)
{
	IIC_Read_One_Byte(I2C_WRITE_ADDR,uch_addr,puch_data);
}

void maxim_max30102_read_fifo(unsigned long *pun_red_led,unsigned long *pun_ir_led)
{
	unsigned long un_temp=0;
	unsigned char uch_temp=0;
	unsigned char ach_i2c_data[6];
	*pun_red_led=0;
	*pun_ir_led=0;
	
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);
	
	IIC_ReadBytes(I2C_WRITE_ADDR,REG_FIFO_DATA,ach_i2c_data,6);
	
	un_temp=ach_i2c_data[0];
  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=ach_i2c_data[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=ach_i2c_data[2];
  *pun_red_led+=un_temp;
  
  un_temp=ach_i2c_data[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=ach_i2c_data[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=ach_i2c_data[5];
  *pun_ir_led+=un_temp;
  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
}