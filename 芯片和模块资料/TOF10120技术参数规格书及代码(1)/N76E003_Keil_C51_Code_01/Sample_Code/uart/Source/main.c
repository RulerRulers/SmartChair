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
//  File Function: N76E003 UART-0 Mode3 demo code
//***********************************************************************************************************

#include <stdio.h>
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Common.h"
#include "Delay.h"
#include "Function_Define.h"

#define BUFFER_SIZE		16

unsigned char rxbuf[16],rxempty, rxcnt,rxflag,waitflag;
unsigned short int length_val;
unsigned int timeout;
/**
 * FUNCTION_PURPOSE: serial interrupt, echo received data.
 * FUNCTION_INPUTS: P3.0(RXD) serial input
 * FUNCTION_OUTPUTS: P3.1(TXD) serial output
 */
void serial_IT(void) interrupt 4 
{
    if (RI) 
    {                                       /* if reception occur */
        clr_RI;                             /* clear reception flag for next reception */
			
				if(rxflag==0)
				{
					rxbuf[rxcnt++] = SBUF;
					if(rxcnt>2)
					{
						if(waitflag==1)
						{
							if((rxbuf[rxcnt-2]=='o')&&(rxbuf[rxcnt-1]=='k'))//串口接收距离数据格式ASCII码0mm~2000mm
							{
								waitflag=2;
							}
						}
						else
						{
							if((rxbuf[rxcnt-1]=='m')&&(rxbuf[rxcnt-2]=='m'))//串口接收距离数据格式ASCII码0mm~2000mm
							{
								rxflag=1;
							}
						}
					}
					else if(rxcnt>=16)
					{
						rxcnt=0;
					}
				}
				else
				{
					rxempty = SBUF;
				}
				
    }
    if(TI)
    {
        clr_TI;                             /* if emission occur */
    }
}
/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void main (void)
{
	unsigned char i;
   
	P06_Quasi_Mode;               				//Set UART GPIO are Quasi Mode
	P07_Quasi_Mode;
	InitialUART0_Timer3(9600);            //9600 Baud Rate from timer1
	
	set_ES;                               //enable UART interrupt
	set_EA;                               //enable global interrupt
	
	Send_Data_To_UART0('s');
	Send_Data_To_UART0('5');
	Send_Data_To_UART0('-');
	Send_Data_To_UART0('1');
	Send_Data_To_UART0('#'); //设为被动读取模式
	Timer0_Delay1ms(10);
	waitflag=1;
	while(waitflag==1);
	waitflag=0; 
	Timer0_Delay1ms(1000);
	rxcnt=0;
	while(1)
	{
		Send_Data_To_UART0('r');
		Send_Data_To_UART0('6');
		Send_Data_To_UART0('#');//主动读取距离命令
		timeout=10000;
		while((rxflag==0)&&(timeout--));
		
		if(rxflag)//接收到1组有效数据
		{
			for(i=0; i<rxcnt; i++)
			{
				if(rxbuf[i]=='m')
				{
					if(rxbuf[i+1]=='m')	//ASCII码转换为16进制数据，单位mm
					{
						if((i>0)&&(rxbuf[i-1]>='0')&&(rxbuf[i-1]<='9'))
							length_val=rxbuf[i-1]-'0';
						if((i>1)&&(rxbuf[i-2]>='0')&&(rxbuf[i-2]<='9'))
							length_val+=(rxbuf[i-2]-'0')*10;
						if((i>2)&&(rxbuf[i-3]>='0')&&(rxbuf[i-3]<='9'))
							length_val+=(rxbuf[i-3]-'0')*100;
						if((i>3)&&(rxbuf[i-4]>='0')&&(rxbuf[i-4]<='9'))
							length_val+=(rxbuf[i-4]-'0')*1000;
						break;
					}
				}
			}
			rxflag = 0;
			rxcnt = 0;
		}
	}	
}

