//#include "IIC.h"
#include  "STC8G_H_Soft_I2C.h"
#include "Max30102.h"
#include "delay.h"


//附加函数
void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char Data)
{				   	  	    																 
  I2C_Start();  
	I2C_WriteAbyte(daddr);	    //发送写命令
	I2C_Check_ACK();
	I2C_WriteAbyte(addr);//发送地址
	I2C_Check_ACK();							  		   
	I2C_WriteAbyte(Data);     //发送字节							   
	I2C_Check_ACK();  		    	   
  I2C_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
void IIC_Read_One_Byte(unsigned char daddr,unsigned char addr,unsigned char *Data)
{
	I2C_Start();  
	I2C_WriteAbyte(daddr);	    //发送写命令
	I2C_Check_ACK();
	I2C_WriteAbyte(addr);//发送地址
	I2C_Check_ACK();		
  I2C_Start();  
  I2C_WriteAbyte(daddr|0x01);
	I2C_Check_ACK();
  *Data=I2C_ReadAbyte();
	S_NoACK();
	I2C_Stop();//产生一个停止条件 
	
}	
void IIC_ReadBytes(unsigned char deviceAddr,unsigned char writeAddr,unsigned char *Data,unsigned char dataLength)
{		
	unsigned char i;	
  I2C_Start();  
	I2C_WriteAbyte(deviceAddr);	    //发送写命令
	I2C_Check_ACK();
	I2C_WriteAbyte(writeAddr);
	I2C_Check_ACK();
	I2C_WriteAbyte(deviceAddr|0X01);//进入接收模式			   
	I2C_Check_ACK();
	for(i=0;i<dataLength-1;i++)
	{
		Data[i] = I2C_ReadAbyte();
		S_ACK();
	}		
	Data[dataLength-1] = I2C_ReadAbyte();
  S_NoACK();
  I2C_Stop();//产生一个停止条件 
	delay_ms(10);	
}


//MAX30102写寄存器函数
void max30102_Bus_Write(unsigned char Register_Address,unsigned char Word_Data)
{
	/* 第1步：发起I2C总线启动信号 */
	I2C_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：接收ACK */
  I2C_Check_ACK();//IIC接收应答
	/* 第4步：发送字节地址 */
	I2C_WriteAbyte(Register_Address);
	/* 第5步：接收ACK */
	I2C_Check_ACK();//IIC接收应答
	/* 第6步：开始写入数据 */
	I2C_WriteAbyte(Word_Data);
  /* 第7步：接收ACK */
	I2C_Check_ACK();//IIC接收应答
	/* 发送I2C总线停止信号 */
	I2C_Stop();//IIC结束
}

//MAX30102读寄存器函数
unsigned char max30102_Bus_Read(unsigned char Register_Address)
{
	unsigned char Data;
	/* 第1步：发起I2C总线启动信号 */
	I2C_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：接收ACK */
  I2C_Check_ACK();
	/* 第4步：发送字节地址， */
	I2C_WriteAbyte(Register_Address);
	/* 第5步：接收ACK */
  I2C_Check_ACK();
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	I2C_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */
	/* 第8步：接收ACK */
   I2C_Check_ACK();
	/* 第9步：读取数据 */
	Data = I2C_ReadAbyte();	/* 读1个字节 */
	/* 第10步：发送ACK */
  S_NoACK();
	/* 发送I2C总线停止信号 */
	I2C_Stop();//IIC结束
	return Data;	/* 执行成功 返回data值 */
}

//MAX30102读缓冲器FIFO Words
void max30102_FIFO_ReadWords(unsigned char Register_Address,unsigned int Word_Data[][2],unsigned char count)
{
	unsigned char i=0;
	unsigned char no = count;
	unsigned char data1, data2;
	/* 第1步：发起I2C总线启动信号 */
	I2C_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：接收ACK */
  I2C_Check_ACK();
	/* 第4步：发送字节地址， */
	I2C_WriteAbyte(Register_Address);
	/* 第5步：接收ACK */
   I2C_Check_ACK();
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	I2C_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */
	/* 第8步：接收ACK */
  I2C_Check_ACK();
	/* 第9步：读取数据 */
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
			S_NoACK();/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		else
			S_ACK();
		Word_Data[i][1] = (((unsigned int)data1 << 8) | data2); 
		no--;	
		i++;
	}
	/* 发送I2C总线停止信号 */
	I2C_Stop();//IIC结束
}

//MAX30102读缓冲器FIFO Bytes
void max30102_FIFO_ReadBytes(unsigned char Register_Address,unsigned char *Data)
{
	max30102_Bus_Read(REG_INTR_STATUS_1);//清除心率血氧中断
	max30102_Bus_Read(REG_INTR_STATUS_2);//清除温度中断
	/* 第1步：发起I2C总线启动信号 */
	I2C_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：接收ACK */
  I2C_Check_ACK();
	/* 第4步：发送字节地址， */
	I2C_WriteAbyte(Register_Address);
	/* 第5步：接收ACK */
  I2C_Check_ACK();
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	I2C_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */
	/* 第8步：接收ACK */
   I2C_Check_ACK();
	/* 第9步：读取数据 */
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
	/* 发送I2C总线停止信号 */
	I2C_Stop();//IIC结束
	
}

//MAX30102读缓冲器FIFO Data
void MAX30102_FIFO_ReadData(unsigned long *Red,unsigned long *Ir)
{
	unsigned char tem[6];
	unsigned long un_temp=0;
	*Red=0;
	*Ir=0;
	max30102_Bus_Read(REG_INTR_STATUS_1);//清除心率血氧中断
	max30102_Bus_Read(REG_INTR_STATUS_2);//清除温度中断
	/* 第1步：发起I2C总线启动信号 */
	I2C_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_WR);	/* 此处是写指令 */
	/* 第3步：接收ACK */
  I2C_Check_ACK();
	/* 第4步：发送字节地址， */
	I2C_WriteAbyte(REG_FIFO_DATA);
	/* 第5步：接收ACK */
  I2C_Check_ACK();
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	I2C_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2C_WriteAbyte(max30102_WR_address | I2C_RD);	/* 此处是读指令 */
	/* 第8步：接收ACK */
  I2C_Check_ACK();
	/* 第9步：读取数据 */
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
	/* 发送I2C总线停止信号 */
	I2C_Stop();//IIC结束
	
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

//MAX30102初始化（可根据自己的实际情况更改相应的配置）
void max30102_init()
{
	
	max30102_reset();//max30102复位
	
	max30102_Bus_Write(REG_INTR_ENABLE_1,0xC0);	//  Interrupt Enable  02->C0
	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);	//  Interrupt Enable  03->00
	
	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//              FIFO  04->00       FIFO_WR_PTR[4:0] FIFO写指针指向下一个要写的内存地址
	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//              FIFO  05->00       OVF_COUNTER[4:0] FIFO溢出丢失数据计数	
	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//              FIFO  06->00       FIFO_RD_PTR[4:0] FIFO读指针指向下一个要读的内存地址
	
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