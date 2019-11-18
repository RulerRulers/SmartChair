#include <STC12C5A60S2.H>
#include "menufun.h"
#include "oled.h"
#include "e2prom.h"
#include "key.h"
#include "TOF10120.h"
#include "intrins.h"//nop 函数
#include "adc.h"
#include "timer.h"


extern unsigned char KeyFuncIndex ,LastIndex; 
extern unsigned int CurrentPress ;//实际压力差值
extern unsigned int CurrrntDistan ;//实际距离差值

sbit voice = P3^0;//语音控制  低电平触发
unsigned int jiuzuo_count =0;//久坐持续时间计数器
unsigned char  qishen_count= 0;//起身次数 计数器





static void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}
  static void Delay1s()		//@12.000MHz
{
	unsigned int i, j;
	i = 50;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}
static void delay(unsigned int z)
{
	while(z--)
	{
    Delay1ms();
  }
}

unsigned char key_scan_task()
{
	   unsigned char Key_Value=0;
     Key_Value = GetKey();//按键读取
	  if((Key_Value == KEY_ENTER))//进入设置
    { 
				 KeyFuncIndex = 1;
				 LastIndex = 0;//进入设置界面
				 return  1; 	 
    }
    return 0;		
}



void MainWin()//主界面
{
	
	 unsigned int Distan1,Distan2,Distan_difference;
	
   OLED_Fill(0x00); //初始清屏
	   
	 OLED_P8x16Str(0,0,"Press:");  //压力 阈值  实际插值
	 OLED_P8x16Str(0,2,"Distan:"); //距离 阈值  实际差值
   OLED_P8x16Str(0,4,"I2C:");  // 
	 OLED_P8x16Str(0,6,"ADC:"); 
	
	 GetParam(&myparam);//读取阈值参数
	 oled_show_num(40,0,myparam.PressureValue,1);//显示压力阈值
	 oled_show_num(40,2,myparam.DistanceValue,0);//显示距离阈值
   		
	 while(1)
	 {
     if(key_scan_task()) return;// 退出当前页面 进入设置 
		  
		 Distan1=GetDistance(0);
		 Distan2=GetDistance(1);//获取 测距值
		 Distan_difference = DistanDiffer(Distan1,Distan2);//计算距离差值
		 if(Distan_difference> myparam.DistanceValue&&(GetADCResult(0)>10) && (GetADCResult(1)>10)) //距离超过阈值 驼背	且有人坐
		 {
			 tuobei_switch = 1;//打开驼背持续时间计数开关 开始累计驼背时间
			 if(T_500ms > 10)//驼背时间超过 5s 语音提醒
			 {
		        voice = 0;
		        Delay1s();
			    voice = 1;//语音提示
				tuobei_switch = 0;
				T_500ms = 0;
			 }
     }
     else//没有持续5s以上超过驼背阈值
		 {
			 
	         tuobei_switch = 0;
				 T_500ms = 0;
     }     
		 
		 if(T_500ms_flag == 1)//ADC 采集完成   500ms 执行一次
		 { 
			  jiuzuo_count++; //久坐时间计数器
			  adc_result = GetADCResult(0);
        adc_result1 = GetADCResult(1); 
			  oled_show_num(30,6, adc_result,0);	//ADC1	
			  oled_show_num(80,6,adc_result1,0);	//ADC2	 		 
			  CurrentPress = DistanDiffer(adc_result,adc_result1);//计算压力差值
			  //CurrentPress =(CurrentPress*100)/(adc_result+adc_result1);
			  oled_show_num(80,0,CurrentPress,1);//显示当前压力差值
        
			  if((adc_result < 10) && (adc_result1 < 10))//起身标志
				{
			            qishen_count++;//起身时间计数  500ms一次
			            if(qishen_count > 6) 	//起身超过 3s
						{
							jiuzuo_count = 0;//人起身  久坐计数器清0  				
							qishen_count = 0; 
						}
        }
				else
				{
            qishen_count = 0;//不满足 起身条件  起身计数器清0  
        }
        if(CurrentPress> myparam.PressureValue)//超过 压力阈值 开启坐歪计数器
				{
            zuowai_switch = 1;//打开坐歪计数开关
					  if(zuowai_count > 3)//坐歪超过7s
						{
			         		voice = 0;//语音提示
							Delay1s();
			    			voice = 1;//语音提示
							 zuowai_count = 0;
							 zuowai_switch = 0;
            }
        }
				else//没有坐歪  关闭坐歪计数开关
				{
				//	voice = 1;
          			 zuowai_count = 0;
					 zuowai_switch = 0;
        		}
			  if(jiuzuo_count > 4800)//久坐 超过 40min 
				{
//             voice = 1;
//             Delay1ms();
			       voice = 0;//语音提示
				   Delay1s();
			    voice = 1;//语音提示
					   jiuzuo_count = 0;//久坐计数器清0  				
						 qishen_count = 0; 
        		}			
				T_500ms_flag = 0;//开启下一次计数
     }	 
	   oled_show_num(80,2,Distan_difference,0);//显示当前距离差值	 		
     oled_show_num(30,4,Distan1,0);	//测距1	 实际值
		 oled_show_num(80,4,Distan2,0);	//测距2	 实际值 
   }
}

void pressureMenu()
{
	 unsigned char Key_Value=0;
	 unsigned char line = 1;// 初始化选择第一行反白
	
   OLED_Fill(0x00); //初始清屏  
	
	 OLED_P8x16Str_t(0,0,"1.SetPressLimit"); //反白第一行 
	 OLED_P8x16Str(0,2,"2.SetDistanLimit"); 
	 
	 OLED_P8x16Str(0,6,"enter"); 
	 OLED_P8x16Str(100,6,"esc"); 
	 
	 while(1)
	 {
		  Key_Value = GetKey();//获取按键
		  if(Key_Value == LEFT)
			{
				 OLED_ClearLine(1);
				 OLED_P8x16Str(0,0,"1.SetPressLimit"); 
				 OLED_P8x16Str_t(0,2,"2.SetDistanLimit"); 
				 
				 OLED_P8x16Str(0,6,"enter"); 
				 OLED_P8x16Str(100,6,"esc"); 
				 line = 2;
      }	
      else if(Key_Value == RIGHT)	
      {
         OLED_ClearLine(2);
				 OLED_P8x16Str_t(0,0,"1.SetPressLimit"); 
				 OLED_P8x16Str(0,2,"2.SetDistanLimit"); 
				 
				 OLED_P8x16Str(0,6,"enter"); 
				 OLED_P8x16Str(100,6,"esc"); 
          
         line = 1; 				
      }	
      else if(Key_Value == KEY_RETURN)//返回主菜单	
      {
           KeyFuncIndex = 0;
					 LastIndex = 0;
					 return;	
      }				
		  
			else if(Key_Value == KEY_ENTER) 
			{
         if(line == 1)//调整压力阈值
				 {
           KeyFuncIndex = 3;
					 LastIndex = 0;
					 return;	

         }
         else 
				 {
           KeyFuncIndex = 4;
					 LastIndex = 0;
					 return;	
         }
      }
		 
   }
}

void DistanceMenu()
{

	
}
void ChangePressure()//设置 压力阈值
{
   unsigned char Key_Value=0;
	
   OLED_Fill(0x00); //初始清屏  
	
	 OLED_P8x16Str(40,0,"Setting");//设置  
	
	 OLED_P8x16Str(0,2,"Pressure:");  
	 GetParam(&myparam);
	 oled_show_num(80,2,myparam.PressureValue,1);
	 
	 OLED_P8x16Str(0,6,"ok"); 
	 OLED_P8x16Str(100,6,"esc"); 
	
	 while(1)
	 {
		  Key_Value = GetKey();//获取按键
		  if(Key_Value == LEFT)
			{
				 myparam.PressureValue+=5;
				 if(myparam.PressureValue > 70) myparam.PressureValue = 10;			
				 OLED_ClearLine(2);//清掉旧值
				 OLED_P8x16Str(0,2,"Pressure:");  
				 oled_show_num(80,2,myparam.PressureValue,1);//显示新值
						
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 
      }	
      else if(Key_Value == RIGHT)	
      {
				 myparam.PressureValue-=5;
				 if(myparam.PressureValue < 10) myparam.PressureValue = 70;			
				 OLED_ClearLine(2);//清掉旧值
				 OLED_P8x16Str(0,2,"Pressure:");  
				 oled_show_num(80,2,myparam.PressureValue,1);//显示新值
				 
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 		
      }	
      else if(Key_Value == KEY_RETURN)//返回设置选择界面
      {
           KeyFuncIndex = 1;
					 LastIndex = 0;
					 return;	
      }				
		  
			else if(Key_Value == KEY_ENTER) 
			{
 
				   SetParam(&myparam);//保存参数
				
				   OLED_Fill(0x00); //初始清屏  
	
	         OLED_P8x16Str(40,0,"Save OK!");//保存成功  
					   
           delay(500);//延时500ms 
           				
           KeyFuncIndex = 0;
					 LastIndex = 0;
					 return;//返回主菜单	       
      }
	
		}
}
void ChangeDistance()//改变距离阈值
{
   unsigned char Key_Value=0;
	
   OLED_Fill(0x00); //初始清屏  
	
	 OLED_P8x16Str(40,0,"Setting");//设置  
	
	 OLED_P8x16Str(0,2,"Distance:");  
	 GetParam(&myparam);
	 oled_show_num(80,2,myparam.DistanceValue,0);//显示距离阈值
	 
	 OLED_P8x16Str(0,6,"ok"); 
	 OLED_P8x16Str(100,6,"esc"); 
	
	 while(1)
	 {
		  Key_Value = GetKey();//获取按键
		  if(Key_Value == LEFT)
			{
				 myparam.DistanceValue++;
				 if(myparam.DistanceValue > 12) myparam.DistanceValue = 3;			
				 OLED_ClearLine(2);//清掉旧值
				 OLED_P8x16Str(0,2,"Distance:");  
				 oled_show_num(80,2,myparam.DistanceValue,0);//显示新值 不带%号
						
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 
      }	
      else if(Key_Value == RIGHT)	
      {
				 myparam.DistanceValue--;
				 if(myparam.DistanceValue < 1) myparam.DistanceValue = 12;			
				 OLED_ClearLine(2);//清掉旧值
				 OLED_P8x16Str(0,2,"Distance:");  
				 oled_show_num(80,2,myparam.DistanceValue,0);//显示新值
				 
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 		
      }	
      else if(Key_Value == KEY_RETURN)//返回设置选择界面
      {
           KeyFuncIndex = 1;
					 LastIndex = 0;
					 return;	
      }				
		  
			else if(Key_Value == KEY_ENTER) 
			{
 
				   SetParam(&myparam);//保存参数
				
				   OLED_Fill(0x00); //初始清屏  
	
	         OLED_P8x16Str(40,0,"Save OK!");//保存成功  
					   
           delay(500);//延时500ms 
           				
           KeyFuncIndex = 0;
					 LastIndex = 0;
					 return;//返回主菜单	       
      }
	
		}
}
