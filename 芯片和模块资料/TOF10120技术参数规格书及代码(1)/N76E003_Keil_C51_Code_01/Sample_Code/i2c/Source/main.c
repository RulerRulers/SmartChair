/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2016
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 I2C demo code, Slave Address of sensor = 0xA4=164
//
//   ____________           ______________ 
//  |            |   SDA    |             |
//  |            |<-------->|             |
//  |            |          |             |
//  | N76E003(M) |          |   sensor    |
//  |            |   SCL    |             |
//  |            |--------->|             |
//  |____________|          |_____________|
//
//***********************************************************************************************************

#include <stdio.h>
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_Define.h"


#define SYS_CLK_EN              0
#define SYS_SEL                 2
#define SYS_DIV_EN              0                   //0: Fsys=Fosc=16mHz, 1: Fsys = Fosc/(2*CKDIV)=8mHz
#define SYS_DIV                 1
#define I2C_CLOCK               39									//Fi2c = Fsys/(4*(I2C_CLOCK+1))=16mHz/(4*(I2C_CLOCK+1))=100kHz

#define SENSOR_SLA              0xA4
#define SENSOR_WR               0
#define SENSOR_RD               1

#define ERROR_CODE              0x01

unsigned char rxbuf[16],rxempty, rxcnt,rxflag,errid;
unsigned short int length_val;
//========================================================================================================
void Init_I2C(void)
{
    /* Set I2C clock rate */
    I2CLK = I2C_CLOCK; 

    /* Enable I2C */
    set_I2CEN;                                   
}
//========================================================================================================
void ReadLenth(unsigned char addr)
{
	/* Step1 */
	set_STA;                                /* Send Start bit to I2C SENSOR */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x08)                     /* 0x08:  A START condition has been transmitted*/
	{
		errid = ERROR_CODE;
	}

	/* Step2 */
	I2DAT = (SENSOR_SLA | SENSOR_WR);       /* Send (SLA+W) to SENSOR */
	clr_STA;                                /* Clear STA and Keep SI value in I2CON */    
	clr_SI;
	while (!SI);
	if (I2STAT != 0x18)                     /* 0x18: SLA+W has been transmitted; ACK has been received */              
	{
		errid = ERROR_CODE;
	}

	/* Step3 */
	I2DAT = addr;            			 					/* Send I2C SENSOR's Address */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x28)                     /* 0x28:  Data byte in S1DAT has been transmitted; ACK has been received */              
	{
		errid = ERROR_CODE;
	}
//		Delay10us(3);												//根据需要加30uS延时
	/* Step4 */
	set_STA;                                /* Repeated START */
	clr_SI; 
	while (!SI);
	if (I2STAT != 0x10)                     /* 0x10: A repeated START condition has been transmitted */
	{
		errid = ERROR_CODE;
	}

	/* Step5 */
	clr_STA;                                /* Clear STA and Keep SI value in I2CON */
	I2DAT = (SENSOR_SLA | SENSOR_RD);       /* Send (SLA+R) to SENSOR */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x40)                     /* 0x40:  SLA+R has been transmitted; ACK has been received */              
	{
		errid = ERROR_CODE;
	}

	/* Step6 */                             /* Read I2C SENSOR data */
	set_AA;                             		/* Set Assert Acknowledge Control Bit */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x50)                 		/* 0x50:Data byte has been received; NOT ACK has been returned */              
	{
		errid = ERROR_CODE;
	}
	length_val=I2DAT;
	length_val<<=8;	
	
	set_AA;                             		/* Set Assert Acknowledge Control Bit */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x50)                 		/* 0x50:Data byte has been received; NOT ACK has been returned */              
	{
		errid = ERROR_CODE;
	}
	length_val+=I2DAT;

	/* Step7 */
	clr_AA;                                 /* Send a NACK to disconnect SENSOR */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x58)                     /* 0x58:Data byte has been received; ACK has been returned */
	{
		errid = ERROR_CODE;
	}
	
	/* Step8 */    
	clr_SI;
	set_STO;
	while (STO);                            /* Check STOP signal */ 
}
//========================================================================================================
void WritePara(unsigned char addr,unsigned char dat)
{
	/* Step1 */
	set_STA;                                /* Send Start bit to I2C SENSOR */         
	clr_SI;
	while (!SI);
	if (I2STAT != 0x08)                     /* 0x08:  A START condition has been transmitted*/
	{
    errid = ERROR_CODE;
	}

	/* Step2 */
	clr_STA;                                /* Clear STA and Keep SI value in I2CON */
	I2DAT = SENSOR_SLA | SENSOR_WR;         /* Send (SLA+W) to SENSOR */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x18)                     /* 0x18: SLA+W has been transmitted; ACK has been received */             
	{
    errid = ERROR_CODE;
	}

	/* Step3 */
	I2DAT = addr;             							/* Send SENSOR's Address */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x28)                     /* 0x28:  Data byte in S1DAT has been transmitted; ACK has been received */
	{
    errid = ERROR_CODE;
	}

	/* Step4 */															/* Write data to I2C SENSOR */
	I2DAT = dat;                      			/* Send data to SENSOR */
	clr_SI;
	while (!SI);
	if (I2STAT != 0x28)                 		/* 0x28:  Data byte in S1DAT has been transmitted; ACK has been received */
	{
		errid = ERROR_CODE;
	}     
																					//After STOP condition, a inner SENSOR timed-write-cycle 
																					//will occure and SENSOR will not response to outside command
																					/* 0x18: SLA+W has been transmitted; ACK has been received */
	/* Step6 */
	do
	{
		set_STO;                            	/* Set I2C STOP Control Bit */
		clr_SI;
		while (STO);                        	/* Check STOP signal */
		
		set_STA;                            	/* Check if no ACK is returned by SENSOR, it is under timed-write cycle */
		clr_SI;
		while (!SI);
		if (I2STAT != 0x08)                 	/* 0x08:  A START condition has been transmitted*/
		{
			errid = ERROR_CODE;
		}

		clr_STA;                            	/* Clear STA and Keep SI value in I2CON */
		I2DAT = (SENSOR_SLA | SENSOR_WR);   	/* Send (SLA+W) to SENSOR */
		clr_SI;
		while (!SI);
	}while(I2STAT != 0x18);

	/* Step7 */
	set_STO;                                /* Set STOP Bit to I2C SENSOR */
	clr_SI;
	while (STO);                            /* Check STOP signal */
}
//========================================================================================================
void main(void)
{
    Set_All_GPIO_Quasi_Mode;
		InitialUART0_Timer3(115200);
  
    /* Initial I2C function */
    Init_I2C();   //100kHz                            

		WritePara(0x09,0x00);//设为被动读取模式
		Timer0_Delay1ms(1000); 
    while (1)
		{		
			ReadLenth(0x00); //读取实时距离
			if((length_val>0)&&(errid==0))
			{
				_nop_();
				length_val=0;
			}
			ReadLenth(0x04); //读取滤波距离
			if((length_val>0)&&(errid==0))
			{
				_nop_();
				length_val=0;
			}
			Timer0_Delay1ms(100);
		}
/* =================== */
}

