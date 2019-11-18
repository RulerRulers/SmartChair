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
//Function:初始化IO口
//Description：GPIO初始化为高,通知器件总线空闲
//Input:无
//Output:无
//Return:无
//Others:无
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

//Function:启动IIC传输
//Description：IIC_SCL为高时,IIC_SDA由高到低
//Input:无
//Output:无
//Return:无
//Others:无
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

//Function:停止IIC传输
//Description：IIC_SCL为高时,IIC_SDA由低到高
//Input:无
//Output:无
//Return:无
//Others:无
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

//Function:产生应答信号
//Description：IIC_SCL为高时,IIC_SDA被拉低并保持一定时间的低电平
//Input:无
//Output:无
//Return:无
//Others:无
void IIC_Ack()
{
	unsigned char i;
	
	if(channel == 1)
	{	
		IIC_SCL = 1;//在scl为高电平期间等待应答
		delay_ms(2);
		while((IIC_SDA == 1) && i < 250)//若为应答0即退出，从机向主机发送应答信号
			i++;//通过i自增等待一段时间	
		IIC_SCL = 0;//得到应答,拉低时钟
		delay_ms(2);
  }
	else
	{
    IIC_SCL1 = 1;//在scl为高电平期间等待应答
		delay_ms(2);
		while((IIC_SDA1 == 1) && i < 250)//若为应答0即退出，从机向主机发送应答信号
			i++;//通过i自增等待一段时间	
		IIC_SCL1 = 0;//得到应答,拉低时钟
		delay_ms(2);
  }
}

//Function:产生非应答信号
//Description：IIC_SCL为高时,IIC_SDA为高
//Input:无
//Output:无
//Return:无
//Others:无
// void IIC_NAck()
// {
// 	IIC_SCL = 1;
// 	delay_ms(2);
// 	IIC_SDA = 1;
// 	IIC_SCL = 0;
// 	delay_ms(2);
// }

//Function:I2C写一个字节数据，返回ACK或者NACK
//Description：从高位开始依次传输
//Input:写的字节
//Output:无
//Return:无
//Others:无
void IIC_Write_Byte(unsigned char txd)
{
	unsigned char len;
	
	if(channel == 1)
	{
		for (len=0;len<8;len++)
		{
			IIC_SCL = 0;//只有在IIC_SCL为低电平时,才允许IIC_SDA上的数据变化
			delay_us(5); 
			if(txd & 0x80)
				IIC_SDA = 1;
			else
				IIC_SDA = 0;  
			delay_us(5);   
			IIC_SCL = 1;//在IIC_SCL为高电平时,不允许IIC_SDA上的数据变化，使数据稳定
			txd <<= 1;
			delay_us(10); 
			IIC_SCL = 0;	
			delay_us(5);
			}
		IIC_SDA = 1;//释放总线
		IIC_SCL = 1;
		delay_us(5);
	}
	else
	{
    for (len=0;len<8;len++)
		{
			IIC_SCL1 = 0;//只有在IIC_SCL为低电平时,才允许IIC_SDA上的数据变化
			delay_us(5); 
			if(txd & 0x80)
				IIC_SDA1 = 1;
			else
				IIC_SDA1 = 0;  
			delay_us(5);   
			IIC_SCL1 = 1;//在IIC_SCL为高电平时,不允许IIC_SDA上的数据变化，使数据稳定
			txd <<= 1;
			delay_us(10); 
			IIC_SCL1 = 0;	
			delay_us(5);
		}
		IIC_SDA1 = 1;//释放总线
		IIC_SCL1 = 1;
		delay_us(5);
  }
}

//Function:I2C读一个字节数据，返回ACK或者NACK
//Description：从高到低,依次接收
//Input:是否应答
//Output:无
//Return:接收的数据
//Others:无
unsigned char IIC_Read_Byte(/*unsigned char ack*/)
{
	unsigned char len,dat = 0;
		
  if(channel == 1)
	{
		IIC_SDA = 1;//准备读取数据
		for(len=0;len<8;len++) 
		{
			
			IIC_SCL = 0; 
					delay_us(5);
			IIC_SCL = 1;//不允许IIC_SDA变化
			delay_us(5); 
					dat <<= 1;
					dat |= IIC_SDA; //读数据 
			delay_us(5); 
			IIC_SCL = 0;//允许IIC_SDA变化等待下一位数据的到来
			delay_us(5); 
		}  
  }	
	else
	{
   	IIC_SDA1 = 1;//准备读取数据
		for(len=0;len<8;len++) 
		{
			
			IIC_SCL1= 0; 
			delay_us(5);
			IIC_SCL1 = 1;//不允许IIC_SDA变化
			delay_us(5); 
					dat <<= 1;
					dat |= IIC_SDA1; //读数据 
			delay_us(5); 
			IIC_SCL1 = 0;//允许IIC_SDA变化等待下一位数据的到来
			delay_us(5); 
		}  
  }
	  
	return dat;
}

/*****************************************以下是TOF10120芯片的读写函数*************************************************/


//Function:初始化AT24C02
//Description：IO口属于空闲,准备开始通信
//Input:无
//Output:无
//Return:无
//Others:无
void TOF_Init(void)
{
	IO_Init();
}

//Function:写一个字节
//Description：向AT24C02的任意地址写入一个字节数据
//Input:写入的地址和数据
//Output:无
//Return:无
//Others:无
void TOF_Write_Byte(unsigned char addr,unsigned char dat)
{
	IIC_Start();	//启动
	IIC_Write_Byte(0xA4);	//发送从设备地址
	IIC_Ack();	//等待从设备的响应
	IIC_Write_Byte(addr);	//发出芯片内地址
	IIC_Ack();	//等待从设备的响应
	
	delay_us(30); 
	
	IIC_Write_Byte(dat);	//发送数据
	IIC_Ack();	//等待从设备的响应
	IIC_Stop();	//停止
	delay_ms(10);	 // 写入周期
}

//Function:指定地址读一个字节
//Description：向AT24C02的指定地址读出一个字节数据
//Input:读地址
//Output:无
//Return:读到的数据
//Others:无
unsigned char TOF_Read_Byte(unsigned char addr)
{
	unsigned char dat;
	
	IIC_Start();
	IIC_Write_Byte(0xA4);//发送发送从设备地址 写操作
	IIC_Ack();
	IIC_Write_Byte(addr);//发送芯片内地址
	IIC_Ack();
	delay_us(30); 
	IIC_Start();
	IIC_Write_Byte(0xA5);//发送发送从设备地址 读操作
	IIC_Ack();
	dat = IIC_Read_Byte();//获取数据
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

