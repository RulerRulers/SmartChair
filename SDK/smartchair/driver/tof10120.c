#include <string.h>
#include <STC12C5A60S2.H>
#include <intrins.h>
#include "TOF10120.h"
#include "stdio.h"


sbit IIC_SCL = P2^0;
sbit IIC_SDA = P2^1;

sbit IIC_SCL1 = P2^2;
sbit IIC_SDA1 = P2^3;

unsigned char channel = 1;

static void Delay1us()//@12.000MHz
{
	_nop_();
	_nop_();
}

static void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}

static void delay_us(unsigned int nus)
{
  while(nus--)
	{
    Delay1us();
  }
}



static void delay_ms(unsigned int nms)
{
  while(nms--)
	{
    Delay1ms();
  }

}
//Function:��ʼ��IO��
//Description��GPIO��ʼ��Ϊ��,֪ͨ�������߿���
//Input:��
//Output:��
//Return:��
//Others:��
void IO_Init()
{
	if(channel == 1)
	{
		IIC_SCL = 1;
		delay_us(5);
		IIC_SDA = 1;
		delay_us(5);
	}
	else
	{
    IIC_SCL1 = 1;
		delay_us(5);
		IIC_SDA1 = 1;
		delay_us(5);
  }
	
}

//Function:����IIC����
//Description��IIC_SCLΪ��ʱ,IIC_SDA�ɸߵ���
//Input:��
//Output:��
//Return:��
//Others:��
static void IIC_Start()
{
	if(channel == 1)
	{
		IIC_SDA = 1;
		IIC_SCL = 1;
		delay_us(5);
		IIC_SDA = 0;
		delay_us(5);
		IIC_SCL = 0;
	}
	else
  {
   	IIC_SDA1 = 1;
	  IIC_SCL1 = 1;
	  delay_us(5);
	  IIC_SDA1 = 0;
	  delay_us(5);
	  IIC_SCL1 = 0;
  }  		
	
}

//Function:ֹͣIIC����
//Description��IIC_SCLΪ��ʱ,IIC_SDA�ɵ͵���
//Input:��
//Output:��
//Return:��
//Others:��
static void IIC_Stop()
{
	if(channel == 1)
	{
		IIC_SCL = 0;
		IIC_SDA = 0;
		delay_us(5);
		IIC_SCL = 1;
		IIC_SDA = 1;
		delay_us(5);
	}
	else
	{
    IIC_SCL1 = 0;
		IIC_SDA1 = 0;
		delay_us(5);
		IIC_SCL1 = 1;
		IIC_SDA1 = 1;
		delay_us(5);
  }
}

//Function:����Ӧ���ź�
//Description��IIC_SCLΪ��ʱ,IIC_SDA�����Ͳ�����һ��ʱ��ĵ͵�ƽ
//Input:��
//Output:��
//Return:��
//Others:��
void IIC_Ack()
{
	unsigned char i;
	
	if(channel == 1)
	{	
		IIC_SCL = 1;//��sclΪ�ߵ�ƽ�ڼ�ȴ�Ӧ��
		delay_ms(2);
		while((IIC_SDA == 1) && i < 250)//��ΪӦ��0���˳����ӻ�����������Ӧ���ź�
			i++;//ͨ��i�����ȴ�һ��ʱ��	
		IIC_SCL = 0;//�õ�Ӧ��,����ʱ��
		delay_ms(2);
  }
	else
	{
    IIC_SCL1 = 1;//��sclΪ�ߵ�ƽ�ڼ�ȴ�Ӧ��
		delay_ms(2);
		while((IIC_SDA1 == 1) && i < 250)//��ΪӦ��0���˳����ӻ�����������Ӧ���ź�
			i++;//ͨ��i�����ȴ�һ��ʱ��	
		IIC_SCL1 = 0;//�õ�Ӧ��,����ʱ��
		delay_ms(2);
  }
}

//Function:������Ӧ���ź�
//Description��IIC_SCLΪ��ʱ,IIC_SDAΪ��
//Input:��
//Output:��
//Return:��
//Others:��
// void IIC_NAck()
// {
// 	IIC_SCL = 1;
// 	delay_ms(2);
// 	IIC_SDA = 1;
// 	IIC_SCL = 0;
// 	delay_ms(2);
// }

//Function:I2Cдһ���ֽ����ݣ�����ACK����NACK
//Description���Ӹ�λ��ʼ���δ���
//Input:д���ֽ�
//Output:��
//Return:��
//Others:��
void IIC_Write_Byte(unsigned char txd)
{
	unsigned char len;
	
	if(channel == 1)
	{
		for (len=0;len<8;len++)
		{
			IIC_SCL = 0;//ֻ����IIC_SCLΪ�͵�ƽʱ,������IIC_SDA�ϵ����ݱ仯
			delay_us(5); 
			if(txd & 0x80)
				IIC_SDA = 1;
			else
				IIC_SDA = 0;  
			delay_us(5);   
			IIC_SCL = 1;//��IIC_SCLΪ�ߵ�ƽʱ,������IIC_SDA�ϵ����ݱ仯��ʹ�����ȶ�
			txd <<= 1;
			delay_us(10); 
			IIC_SCL = 0;	
			delay_us(5);
			}
		IIC_SDA = 1;//�ͷ�����
		IIC_SCL = 1;
		delay_us(5);
	}
	else
	{
    for (len=0;len<8;len++)
		{
			IIC_SCL1 = 0;//ֻ����IIC_SCLΪ�͵�ƽʱ,������IIC_SDA�ϵ����ݱ仯
			delay_us(5); 
			if(txd & 0x80)
				IIC_SDA1 = 1;
			else
				IIC_SDA1 = 0;  
			delay_us(5);   
			IIC_SCL1 = 1;//��IIC_SCLΪ�ߵ�ƽʱ,������IIC_SDA�ϵ����ݱ仯��ʹ�����ȶ�
			txd <<= 1;
			delay_us(10); 
			IIC_SCL1 = 0;	
			delay_us(5);
		}
		IIC_SDA1 = 1;//�ͷ�����
		IIC_SCL1 = 1;
		delay_us(5);
  }
}

//Function:I2C��һ���ֽ����ݣ�����ACK����NACK
//Description���Ӹߵ���,���ν���
//Input:�Ƿ�Ӧ��
//Output:��
//Return:���յ�����
//Others:��
unsigned char IIC_Read_Byte(/*unsigned char ack*/)
{
	unsigned char len,dat = 0;
		
  if(channel == 1)
	{
		IIC_SDA = 1;//׼����ȡ����
		for(len=0;len<8;len++) 
		{
			
			IIC_SCL = 0; 
					delay_us(5);
			IIC_SCL = 1;//������IIC_SDA�仯
			delay_us(5); 
					dat <<= 1;
					dat |= IIC_SDA; //������ 
			delay_us(5); 
			IIC_SCL = 0;//����IIC_SDA�仯�ȴ���һλ���ݵĵ���
			delay_us(5); 
		}  
  }	
	else
	{
   	IIC_SDA1 = 1;//׼����ȡ����
		for(len=0;len<8;len++) 
		{
			
			IIC_SCL1= 0; 
			delay_us(5);
			IIC_SCL1 = 1;//������IIC_SDA�仯
			delay_us(5); 
					dat <<= 1;
					dat |= IIC_SDA1; //������ 
			delay_us(5); 
			IIC_SCL1 = 0;//����IIC_SDA�仯�ȴ���һλ���ݵĵ���
			delay_us(5); 
		}  
  }
	  
	return dat;
}

/*****************************************������TOF10120оƬ�Ķ�д����*************************************************/


//Function:��ʼ��AT24C02
//Description��IO�����ڿ���,׼����ʼͨ��
//Input:��
//Output:��
//Return:��
//Others:��
void TOF_Init(void)
{
	IO_Init();
}

//Function:дһ���ֽ�
//Description����AT24C02�������ַд��һ���ֽ�����
//Input:д��ĵ�ַ������
//Output:��
//Return:��
//Others:��
void TOF_Write_Byte(unsigned char addr,unsigned char dat)
{
	IIC_Start();	//����
	IIC_Write_Byte(0xA4);	//���ʹ��豸��ַ
	IIC_Ack();	//�ȴ����豸����Ӧ
	IIC_Write_Byte(addr);	//����оƬ�ڵ�ַ
	IIC_Ack();	//�ȴ����豸����Ӧ
	
	delay_us(30); 
	
	IIC_Write_Byte(dat);	//��������
	IIC_Ack();	//�ȴ����豸����Ӧ
	IIC_Stop();	//ֹͣ
	delay_ms(10);	 // д������
}

//Function:ָ����ַ��һ���ֽ�
//Description����AT24C02��ָ����ַ����һ���ֽ�����
//Input:����ַ
//Output:��
//Return:����������
//Others:��
unsigned char TOF_Read_Byte(unsigned char addr)
{
	unsigned char dat;
	
	IIC_Start();
	IIC_Write_Byte(0xA4);//���ͷ��ʹ��豸��ַ д����
	IIC_Ack();
	IIC_Write_Byte(addr);//����оƬ�ڵ�ַ
	IIC_Ack();
	delay_us(30); 
	IIC_Start();
	IIC_Write_Byte(0xA5);//���ͷ��ʹ��豸��ַ ������
	IIC_Ack();
	dat = IIC_Read_Byte();//��ȡ����
	IIC_Stop();
	
	return dat;
}

unsigned int GetDistance(unsigned char ch)
{
	unsigned int Distance = 0;
  if(ch == 0)
	{
		channel = 1;
    Distance = TOF_Read_Byte(0x04);
	  Distance *=256;	
	  Distance +=TOF_Read_Byte(0x05);	
	
  }
	else if(ch == 1)
	{  
		channel = 2;
		Distance = TOF_Read_Byte(0x04);
	  Distance *=256;	
	  Distance +=TOF_Read_Byte(0x05);	
  }
	return  Distance;
}


unsigned int DistanDiffer(unsigned int x,unsigned int y)
{
	unsigned int max,min,result;
	if (x > y)
	{
		max = x;
		min=y;
	}
	else
	{		
		max = y;
		min = x;
	}
	result = (max - min);
	if(result == 0) result = 1;
	return result;
}

