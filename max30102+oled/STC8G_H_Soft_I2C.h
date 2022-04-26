/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef	__SOFT_I2C_H
#define	__SOFT_I2C_H

#include	"config.h"

void	SI2C_WriteNbyte(u8 addr, u8 *p, u8 number);
void	SI2C_ReadNbyte( u8 addr, u8 *p, u8 number);
void I2C_Delay(void);
void I2C_Start(void);
void I2C_Stop(void);
void S_ACK(void);
void S_NoACK(void);
void I2C_Check_ACK(void) ;
void I2C_WriteAbyte(u8 dat);
u8 I2C_ReadAbyte(void);

#endif

