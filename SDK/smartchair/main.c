#include <STC12C5A60S2.H>
#include "oled.h"
#include "adc.h"
#include "uart.h"
#include "e2prom.h"
#include "key.h"
#include "TOF10120.h"
#include "timer.h"

#include "menu.h"
#include "menufun.h"

#include "stdio.h"//printf����
#include "intrins.h"//nop ����


void (*KeyFuncPtr)(void);
unsigned char KeyFuncIndex=0,LastIndex =1; 


unsigned int CurrrntDistan = 5;//ʵ�ʾ����ֵ

/**********��������************/
void MenuTask(void);

void main()
{
	UartInit();//9600bps@12MHz
	OLED_Init();//OLED��ʼ��
	InitADC();//ADC��ʼ��
	TOF_Init();//���ģ���ʼ��
	Timer0Init();//50����@12.000MHz
	TOF_Write_Byte(0x09,1);//�رմ����Զ�����
	
	MyParamInit(&myparam);//��ʼ������ȡ����
		
	while(1)
	{
		MenuTask();
  }
}


void MenuTask(void)
{

  unsigned char Key_Value=0;
  while(1)
	{
    Key_Value = GetKey();//������ȡ
	  switch (Key_Value)
		{
      case  LEFT : //������� 
				KeyFuncIndex = Key[KeyFuncIndex].KeyLeftState;break;
			case  RIGHT://�Ҽ�����
				KeyFuncIndex = Key[KeyFuncIndex].KeyRightState;break;
      case  KEY_RETURN : //���ؼ������� 
				KeyFuncIndex = Key[KeyFuncIndex].KeyBackState;break;
			case  KEY_ENTER://ȷ�ϼ�����
				KeyFuncIndex = Key[KeyFuncIndex].KeyEntertState;break; 
			default:break; 
    }
		if(KeyFuncIndex != LastIndex)
		{
			KeyFuncPtr = Key[KeyFuncIndex].CurrentOperate;
			LastIndex  = KeyFuncIndex;
			(*KeyFuncPtr)();//����app
		}
		else if(KeyFuncIndex ==0 &&(KeyFuncIndex==LastIndex))
		{
      KeyFuncPtr = Key[KeyFuncIndex].CurrentOperate;
			LastIndex  = KeyFuncIndex;
			(*KeyFuncPtr)();//����app
      
		}		
  }
}


