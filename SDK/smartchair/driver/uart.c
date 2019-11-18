#include <STC12C5A60S2.H>

#include "uart.h"
#include "stdio.h"

void UartInit(void)		//9600bps@12MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0xD9;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	
	//ES = 1;			 //串口中断使能
	//TI = 1;			 //发送中断标记位，必须设置 这两句是为了使用printf函数 如果不使用printf函数 则可以去掉
	
}

/****************串行口1发送****************/  
// void UART1_SendOneByte(unsigned char c)  
// {  
//     SBUF = c;  
//     while(!TI);    //若TI=0，在此等待  
//     TI = 0;    
// }  

/*
 * putchar (mini version): outputs charcter only
 */
// char putchar (char c) 
// {
//   while (!TI);
//   TI = 0;
//   return (SBUF = c);
// }

