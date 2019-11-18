#include <STC12C5A60S2.H>

#include "uart.h"
#include "stdio.h"

void UartInit(void)		//9600bps@12MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	BRT = 0xD9;		//�趨���������ʷ�������װֵ
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	
	//ES = 1;			 //�����ж�ʹ��
	//TI = 1;			 //�����жϱ��λ���������� ��������Ϊ��ʹ��printf���� �����ʹ��printf���� �����ȥ��
	
}

/****************���п�1����****************/  
// void UART1_SendOneByte(unsigned char c)  
// {  
//     SBUF = c;  
//     while(!TI);    //��TI=0���ڴ˵ȴ�  
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

