#include <STC12C5A60S2.H>
#include "menufun.h"
#include "oled.h"
#include "e2prom.h"
#include "key.h"
#include "TOF10120.h"
#include "intrins.h"//nop ����
#include "adc.h"
#include "timer.h"


extern unsigned char KeyFuncIndex ,LastIndex; 
extern unsigned int CurrentPress ;//ʵ��ѹ����ֵ
extern unsigned int CurrrntDistan ;//ʵ�ʾ����ֵ

sbit voice = P3^0;//��������  �͵�ƽ����
unsigned int jiuzuo_count =0;//��������ʱ�������
unsigned char  qishen_count= 0;//������� ������



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

void Beep(unsigned char T)
{
  unsigned char n = 10;
  for(;n>0;n--)
	{
		 voice = 0;
     delay(T);
     voice = 1;
		 delay(T);
  }
}

unsigned char key_scan_task()
{
	   unsigned char Key_Value=0;
     Key_Value = GetKey();//������ȡ
	  if((Key_Value == KEY_ENTER))//��������
    { 
				 KeyFuncIndex = 1;
				 LastIndex = 0;//�������ý���
				 return  1; 	 
    }
    return 0;		
}

void MainWin()//������
{
	
	 unsigned int Distan1,Distan2,Distan_difference;
	
   OLED_Fill(0x00); //��ʼ����
	   
	 OLED_P8x16Str(0,0,"Press:");  //ѹ�� ��ֵ  ʵ�ʲ�ֵ
	 OLED_P8x16Str(0,2,"Distan:"); //���� ��ֵ  ʵ�ʲ�ֵ
   OLED_P8x16Str(0,4,"I2C:");  // 
	 OLED_P8x16Str(0,6,"ADC:"); 
	
	 GetParam(&myparam);//��ȡ��ֵ����
	 oled_show_num(40,0,myparam.PressureValue,1);//��ʾѹ����ֵ
	 oled_show_num(40,2,myparam.DistanceValue,0);//��ʾ������ֵ
   		
	 while(1)
	 {
     if(key_scan_task()) return;// �˳���ǰҳ�� �������ý���
		  
		 Distan1=GetDistance(0);
		 Distan2=GetDistance(1);//��ȡ ���ֵ
		 Distan_difference = DistanDiffer(Distan1,Distan2);//��������ֵ
		 
		 if(Distan_difference> myparam.DistanceValue&&((GetADCResult(0)+GetADCResult(1))>10)) //���볬����ֵ �ձ�	��������  (GetADCResult(0)>10) && (GetADCResult(1)>10)
		 {
			 tuobei_switch = 1;//���ձ�����ʱ��������� ��ʼ�ۼ��ձ�ʱ��
			 if(T_500ms > 10)//�ձ�ʱ�䳬�� 5s ��������
			 {
// 		    voice = 0;
// 		    Delay1s();
// 			  voice = 1;//������ʾ
				Beep(20);//������Ƶ�� 25HZ
				tuobei_switch = 0;
				T_500ms = 0;
			 }
     }
		 else if((Distan2 > Distan1) && ((Distan2 - Distan1) > myparam.DistanceDiffLimit)&& ((GetADCResult(0)+GetADCResult(1))>10) )//���洫����-���洫���� > ��ֵ ����
		 {
        Beep(20);//������Ƶ�� 25HZ
     }
     else//û�г���5s���ϳ����ձ���ֵ
		 {	 
	       tuobei_switch = 0;
				 T_500ms = 0;
     }     
		 
		 if(T_500ms_flag == 1)//ADC �ɼ����   500ms ִ��һ��
		 { 
			  jiuzuo_count++; //����ʱ�������
			  adc_result = GetADCResult(0);
        adc_result1 = GetADCResult(1); 
			  oled_show_num(30,6, adc_result,0);	//ADC1	
			  oled_show_num(80,6,adc_result1,0);	//ADC2	 		 
			  CurrentPress = DistanDiffer(adc_result,adc_result1);//����ѹ����ֵ
			  //CurrentPress =(CurrentPress*100)/(adc_result+adc_result1);
			  oled_show_num(80,0,CurrentPress,1);//��ʾ��ǰѹ����ֵ
        
			  if((adc_result < 10) && (adc_result1 < 10))//�����־
				{
			      qishen_count++;//����ʱ�����  500msһ��
			      if(qishen_count > 40) 	//������ 20s ��������
						{
							jiuzuo_count = 0;//������  ������������0  				
							qishen_count = 0; 
						}
        }
				else
				{
            qishen_count = 0;//������ ��������  �����������0  
        }
        if(CurrentPress> myparam.PressureValue)//���� ѹ����ֵ �������������
				{
            zuowai_switch = 1;//�������������
					  if(zuowai_count > 3)//���ᳬ��7s
						{
// 			         voice = 0;//������ʾ
// 							 Delay1s();
// 			    		 voice = 1;//������ʾ
							 Beep(100);//������Ƶ�� 5HZ
							 zuowai_count = 0;
							 zuowai_switch = 0;
            }
        }
				else//û������  �ر������������
				{
						//	voice = 1;
							 zuowai_count = 0;
							 zuowai_switch = 0;
        }
			  if(jiuzuo_count > 120)//���� ���� 1min 
				{
//             voice = 1;
//             Delay1ms();
			       voice = 0;//������ʾ
				     Delay1s();
			       voice = 1;//������ʾ
					   jiuzuo_count = 0;//������������0  				
						 qishen_count = 0; 
        }			
				T_500ms_flag = 0;//������һ�μ���
     }	 
	   oled_show_num(80,2,Distan_difference,0);//��ʾ��ǰ�����ֵ	 		
     oled_show_num(30,4,Distan1,0);	//���1	 ʵ��ֵ
		 oled_show_num(80,4,Distan2,0);	//���2	 ʵ��ֵ 
   }
}

void pressureMenu()
{
	 unsigned char Key_Value=0;
	 unsigned char line = 1;// ��ʼ��ѡ���һ�з���
	
   OLED_Fill(0x00); //��ʼ����  
	
	 OLED_P8x16Str_t(0,0,"1.SetPressLimit"); //���׵�һ�� 
	 OLED_P8x16Str(0,2,"2.SetDistanLimit"); 
	 OLED_P8x16Str(0,4,"3.SetDisdifLimit"); 
	 
	 OLED_P8x16Str(0,6,"enter"); 
	 OLED_P8x16Str(100,6,"esc"); 
	 
	 while(1)
	 {
		  Key_Value = GetKey();//��ȡ����
		  if(Key_Value == LEFT)
			{
				if(line == 1)//��ǰ�ǵ�һ��  ��Ҫˢ�µ�һ�� ���׵ڶ���
				{
					 OLED_ClearLine(1);
					 OLED_P8x16Str(0,0,"1.SetPressLimit"); 
					 OLED_P8x16Str_t(0,2,"2.SetDistanLimit"); 
					 OLED_P8x16Str(0,4,"3.SetDisdifLimit"); 
					
					 OLED_P8x16Str(0,6,"enter"); 
					 OLED_P8x16Str(100,6,"esc"); 
					 line = 2;
				}
				else if(line == 2)//��ǰ�ǵ�һ��  ��Ҫˢ�µڶ��� ���׵�����
				{
					 OLED_ClearLine(2);
					 OLED_P8x16Str(0,0,"1.SetPressLimit"); 
					 OLED_P8x16Str(0,2,"2.SetDistanLimit"); 
					 OLED_P8x16Str_t(0,4,"3.SetDisdifLimit"); 
					
					 OLED_P8x16Str(0,6,"enter"); 
					 OLED_P8x16Str(100,6,"esc"); 
					 line = 3;
				}
      }	
      else if(Key_Value == RIGHT)	
      {
				if(line == 3)
				{
         OLED_ClearLine(3);
				 OLED_P8x16Str(0,0,"1.SetPressLimit"); 
				 OLED_P8x16Str_t(0,2,"2.SetDistanLimit"); 
				 OLED_P8x16Str(0,4,"3.SetDisdifLimit"); 
				
				 OLED_P8x16Str(0,6,"enter"); 
				 OLED_P8x16Str(100,6,"esc"); 
          
         line = 2; 				
				}
				if(line == 2)//ˢ�µڶ��� ���׵�һ��
				{
          OLED_ClearLine(3);
				  OLED_P8x16Str_t(0,0,"1.SetPressLimit"); 
				  OLED_P8x16Str(0,2,"2.SetDistanLimit"); 
				  OLED_P8x16Str(0,4,"3.SetDisdifLimit"); 
				
				  OLED_P8x16Str(0,6,"enter"); 
				  OLED_P8x16Str(100,6,"esc"); 
					line = 1; 	 
        }
      }	
      else if(Key_Value == KEY_RETURN)//�������˵�	
      {
           KeyFuncIndex = 0;
					 LastIndex = 0;
					 return;	
      }				
		  
			else if(Key_Value == KEY_ENTER) 
			{
         if(line == 1)//����ѹ����ֵ
				 {
           KeyFuncIndex = 4;
					 LastIndex = 0;
					 return;	
         }
				 else if(line == 2)//����������ֵ
				 {
           KeyFuncIndex = 5;
					 LastIndex = 0;
					 return;	
         }
         else//������������ֵ
				 {
           KeyFuncIndex = 6;
					 LastIndex = 0;
					 return;	
         }
      }
		 
   }
}

void DistanceMenu()
{

	
}

void DisDiffMenu()
{


}
void ChangePressure()//���� ѹ����ֵ
{
   unsigned char Key_Value=0;
	
   OLED_Fill(0x00); //��ʼ����  
	
	 OLED_P8x16Str(40,0,"Setting");//����  
	
	 OLED_P8x16Str(0,2,"Pressure:");  
	 GetParam(&myparam);
	 oled_show_num(80,2,myparam.PressureValue,1);
	 
	 OLED_P8x16Str(0,6,"ok"); 
	 OLED_P8x16Str(100,6,"esc"); 
	
	 while(1)
	 {
		  Key_Value = GetKey();//��ȡ����
		  if(Key_Value == LEFT)
			{
				 myparam.PressureValue+=5;
				 if(myparam.PressureValue > 70) myparam.PressureValue = 10;			
				 OLED_ClearLine(2);//�����ֵ
				 OLED_P8x16Str(0,2,"Pressure:");  
				 oled_show_num(80,2,myparam.PressureValue,1);//��ʾ��ֵ
						
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 
      }	
      else if(Key_Value == RIGHT)	
      {
				 myparam.PressureValue-=5;
				 if(myparam.PressureValue < 10) myparam.PressureValue = 70;			
				 OLED_ClearLine(2);//�����ֵ
				 OLED_P8x16Str(0,2,"Pressure:");  
				 oled_show_num(80,2,myparam.PressureValue,1);//��ʾ��ֵ
				 
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 		
      }	
      else if(Key_Value == KEY_RETURN)//��������ѡ�����
      {
           KeyFuncIndex = 1;
					 LastIndex = 0;
					 return;	
      }				
		  
			else if(Key_Value == KEY_ENTER) 
			{
 
				   SetParam(&myparam);//�������
				
				   OLED_Fill(0x00); //��ʼ����  
	
	         OLED_P8x16Str(40,0,"Save OK!");//����ɹ�  
					   
           delay(500);//��ʱ500ms 
           				
           KeyFuncIndex = 0;
					 LastIndex = 0;
					 return;//�������˵�	       
      }
	
		}
}
void ChangeDistance()//�ı������ֵ
{
   unsigned char Key_Value=0;
	
   OLED_Fill(0x00); //��ʼ����  
	
	 OLED_P8x16Str(40,0,"Setting");//����  
	
	 OLED_P8x16Str(0,2,"Distance:");  
	 GetParam(&myparam);
	 oled_show_num(80,2,myparam.DistanceValue,0);//��ʾ������ֵ
	 
	 OLED_P8x16Str(0,6,"ok"); 
	 OLED_P8x16Str(100,6,"esc"); 
	
	 while(1)
	 {
		  Key_Value = GetKey();//��ȡ����
		  if(Key_Value == LEFT)
			{
				 myparam.DistanceValue++;
				 if(myparam.DistanceValue > 12) myparam.DistanceValue = 3;			
				 OLED_ClearLine(2);//�����ֵ
				 OLED_P8x16Str(0,2,"Distance:");  
				 oled_show_num(80,2,myparam.DistanceValue,0);//��ʾ��ֵ ����%��
						
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 
      }	
      else if(Key_Value == RIGHT)	
      {
				 myparam.DistanceValue--;
				 if(myparam.DistanceValue < 1) myparam.DistanceValue = 12;			
				 OLED_ClearLine(2);//�����ֵ
				 OLED_P8x16Str(0,2,"Distance:");  
				 oled_show_num(80,2,myparam.DistanceValue,0);//��ʾ��ֵ
				 
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 		
      }	
      else if(Key_Value == KEY_RETURN)//��������ѡ�����
      {
           KeyFuncIndex = 1;
					 LastIndex = 0;
					 return;	
      }				
		  
			else if(Key_Value == KEY_ENTER) 
			{
 
				   SetParam(&myparam);//�������
				
				   OLED_Fill(0x00); //��ʼ����  
	
	         OLED_P8x16Str(40,0,"Save OK!");//����ɹ�  
					   
           delay(500);//��ʱ500ms 
           				
           KeyFuncIndex = 0;
					 LastIndex = 0;
					 return;//�������˵�	       
      }
	
		}
}

void changeDisDiff()//���þ�������ֵ
{
   unsigned char Key_Value=0;
	
   OLED_Fill(0x00); //��ʼ����  
	
	 OLED_P8x16Str(40,0,"Setting");//����  
	 OLED_P8x16Str(0,2,"DistDiff:");  
	 GetParam(&myparam);
	 oled_show_num(80,2,myparam.DistanceDiffLimit,0);//��ʾ��������ֵ
	 
	 OLED_P8x16Str(0,6,"ok"); 
	 OLED_P8x16Str(100,6,"esc"); 
	
	 while(1)
	 {
		  Key_Value = GetKey();//��ȡ����
		  if(Key_Value == LEFT)
			{
				 myparam.DistanceDiffLimit+=10;
				 if(myparam.DistanceDiffLimit > 240) myparam.DistanceDiffLimit = 0;			
				 OLED_ClearLine(2);//�����ֵ
				 OLED_P8x16Str(0,2,"Differ:");  
				 oled_show_num(80,2,myparam.DistanceDiffLimit,0);//��ʾ��ֵ ����%��
						
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 
      }	
      else if(Key_Value == RIGHT)	
      {
				 myparam.DistanceDiffLimit-=10;
				 if(myparam.DistanceDiffLimit < 10) myparam.DistanceDiffLimit = 240;			
				 OLED_ClearLine(2);//�����ֵ
				 OLED_P8x16Str(0,2,"Differ:");  
				 oled_show_num(80,2,myparam.DistanceDiffLimit,0);//��ʾ��ֵ
				 
				 OLED_P8x16Str(0,6,"ok"); 
				 OLED_P8x16Str(100,6,"esc"); 		
      }	
      else if(Key_Value == KEY_RETURN)//��������ѡ�����
      {
           KeyFuncIndex = 1;
					 LastIndex = 0;
					 return;	
      }				
		  
			else if(Key_Value == KEY_ENTER) 
			{
 
				   SetParam(&myparam);//�������
				
				   OLED_Fill(0x00); //��ʼ����  
	
	         OLED_P8x16Str(40,0,"Save OK!");//����ɹ�  
					   
           delay(500);//��ʱ500ms 
           				
           KeyFuncIndex = 0;
					 LastIndex = 0;
					 return;//�������˵�	       
      }
	
		}
}

