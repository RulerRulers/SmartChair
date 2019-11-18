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

#include "stdio.h"//printf函数
#include "intrins.h"//nop 函数


void (*KeyFuncPtr)(void);
unsigned char KeyFuncIndex=0,LastIndex =1; 


unsigned int CurrrntDistan = 5;//实际距离差值

/**********函数定义************/
void MenuTask(void);

void main()
{
	UartInit();//9600bps@12MHz
	OLED_Init();//OLED初始化
	InitADC();//ADC初始化
	TOF_Init();//测距模块初始化
	Timer0Init();//50毫秒@12.000MHz
	TOF_Write_Byte(0x09,1);//关闭串口自动发送
	
	MyParamInit(&myparam);//初始化并读取参数
		
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
    Key_Value = GetKey();//按键读取
	  switch (Key_Value)
		{
      case  LEFT : //左键按下 
				KeyFuncIndex = Key[KeyFuncIndex].KeyLeftState;break;
			case  RIGHT://右键按下
				KeyFuncIndex = Key[KeyFuncIndex].KeyRightState;break;
      case  KEY_RETURN : //返回键键按下 
				KeyFuncIndex = Key[KeyFuncIndex].KeyBackState;break;
			case  KEY_ENTER://确认键按下
				KeyFuncIndex = Key[KeyFuncIndex].KeyEntertState;break; 
			default:break; 
    }
		if(KeyFuncIndex != LastIndex)
		{
			KeyFuncPtr = Key[KeyFuncIndex].CurrentOperate;
			LastIndex  = KeyFuncIndex;
			(*KeyFuncPtr)();//运行app
		}
		else if(KeyFuncIndex ==0 &&(KeyFuncIndex==LastIndex))
		{
      KeyFuncPtr = Key[KeyFuncIndex].CurrentOperate;
			LastIndex  = KeyFuncIndex;
			(*KeyFuncPtr)();//运行app
      
		}		
  }
}


