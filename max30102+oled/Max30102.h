#ifndef __MAX30102_H__
#define __MAX30102_H__ //防止重定义

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#define max30102_WR_address 0xAE
#define I2C_WRITE_ADDR 			0xAE
#define I2C_READ_ADDR  			0xAF

typedef  			long 		int32_t;
typedef 			char 		int8_t;
typedef unsigned long uint32_t;
typedef unsigned int  uint16_t;	  //对数据类型进行声明定义
typedef unsigned char uint8_t;

//register addresses
#define REG_INTR_STATUS_1 	0x00
#define REG_INTR_STATUS_2 	0x01
#define REG_INTR_ENABLE_1 	0x02
#define REG_INTR_ENABLE_2 	0x03
#define REG_FIFO_WR_PTR   	0x04
#define REG_OVF_COUNTER     0x05
#define REG_FIFO_RD_PTR  	  0x06
#define REG_FIFO_DATA     	0x07
#define REG_FIFO_CONFIG  	  0x08
#define REG_MODE_CONFIG   	0x09
#define REG_SPO2_CONFIG   	0x0A
#define REG_LED1_PA       	0x0C
#define REG_LED2_PA 				0x0D
#define REG_PILOT_PA 				0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR       0x1F
#define REG_TEMP_FRAC       0x20
#define REG_TEMP_CONFIG     0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 					0xFE
#define REG_PART_ID				  0xFF

void max30102_Bus_Write(unsigned char Register_Address,unsigned char Word_Data);//MAX30102写寄存器函数
unsigned char max30102_Bus_Read(unsigned char Register_Address);//MAX30102读寄存器函数
void max30102_FIFO_Read(unsigned char Register_Address,unsigned int Word_Data[][2],unsigned char count);//MAX30102读缓冲器FIFO
void max30102_init();//MAX30102初始化（可根据自己的实际情况更改相应的配置）

//MAX30102读缓冲器FIFO Bytes
void max30102_FIFO_ReadBytes(unsigned char Register_Address,unsigned char *Data);
//MAX30102读缓冲器FIFO Data
void MAX30102_FIFO_ReadData(unsigned long *Red,unsigned long *Ir);

void maxim_max30102_read_fifo(unsigned long *pun_red_led,unsigned long *pun_ir_led);

#endif