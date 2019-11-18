/**
  ******************************************************************************
  * @file    useri2c.h
  * @author  kevin_guo
  * @version V1.0.0
  * @date    12-15-2018
  * @brief   This file contains definitions for useri2c.c
  ******************************************************************************
  * @attention
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USERI2C_H
#define __USERI2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
//IO方向设置
#define USERI2C_SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;GPIOB->BSRR |=1<<9; }
#define USERI2C_SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

//IO操作函数	 
#define USERI2C_SCL    			PBout(8) //SCL
#define USERI2C_SDA    			PBout(9) //SDA	 
#define USERI2C_READ_SDA   	PBin(9)  //输入SDA 

#define I2C_DEVID    				0XA4
/* Exported functions ------------------------------------------------------- */
//IIC所有操作函数
void UserI2c_Init(void);           
void UserI2c_Send_Byte(unsigned char dat);
unsigned char UserI2c_Read_Byte(void);

unsigned char SensorWritenByte(unsigned char *txbuff, unsigned char regaddr, unsigned char size);
unsigned char SensorReadnByte(unsigned char *rtxbuff, unsigned char regaddr, unsigned char size);

void Sensor_I2C_Test(void);
#endif /* __USERI2C_H */

/************************END OF FILE*************************/

