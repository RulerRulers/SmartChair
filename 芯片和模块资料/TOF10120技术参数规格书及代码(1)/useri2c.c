/**
  ******************************************************************************
  * @file    useri2c.c
  * @author  kevin_guo
  * @version V1.0.0
  * @date    12-15-2018
  * @brief   This file provides functions to useri2c
  ******************************************************************************
  * @attention
	* 硬件连接：  
	*           PB8  - USERI2C_SCL      
  *           PB9  - USERI2C_SDA   
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "useri2c.h"	
#include "led.h"
#include "delay.h"
#include "oled.h"
#include "lcd.h"
#include "display.h"
#include "devctrl.h"

/* extern variables ----------------------------------------------------------*/
extern unsigned char dispbuf[16];
extern unsigned char useruart_rxcmd_flag;

extern unsigned char dirt_send_flag;
extern unsigned char dirt_detection_flag;
extern unsigned short int length_val;
extern unsigned short int length_aveval;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned int i2cread_interval;
/* Private function prototypes -----------------------------------------------*/
void UserI2c_DelayuS(unsigned int nCount);
/* Private function ----------------------------------------------------------*/
/*******************************************************************************
* Function Name  : UserI2c_DelayuS
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_DelayuS(unsigned int nCount)
{
  for(; nCount != 0; nCount--)
	{//100kHz
			__nop();__nop();__nop();__nop();__nop();
			__nop();__nop();__nop();__nop();__nop();
			__nop();__nop();__nop();__nop();__nop();
			__nop();__nop();__nop();__nop();__nop();
			__nop();__nop();
	}
}
/*******************************************************************************
* Function Name  : UserI2c_Start
* Description    : 时钟脚为高，数据脚高变低
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_Start(void)
{
	USERI2C_SDA=1;	  	  
	USERI2C_SCL=1;
	UserI2c_DelayuS(30);
 	USERI2C_SDA=0;			
	UserI2c_DelayuS(30);
	USERI2C_SCL=0;			//钳住I2C总线，准备发送或接收数据 
}	  
/*******************************************************************************
* Function Name  : UserI2c_Stop
* Description    : 时钟脚为高，数据脚低变高
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_Stop(void)
{
	USERI2C_SCL=0;
	USERI2C_SDA=0;
 	UserI2c_DelayuS(30);
	USERI2C_SCL=1;
 	UserI2c_DelayuS(30);
	USERI2C_SDA=1;		//发送I2C总线结束信号
	
}
/*******************************************************************************
* Function Name  : UserI2c_Wait_Ack
* Description    : 第9个时钟上升沿读取ack信号
* Input          : None
* Output         : None
* Return         : =0有ack
*								 : =1无ack
*******************************************************************************/
unsigned char UserI2c_Wait_Ack(void)
{
	unsigned int ucErrTime=0;
  unsigned char RetValue;
	
	USERI2C_SDA_IN();      //SDA设置为输入   
	USERI2C_SDA=1;  
	USERI2C_SCL=0;
 	UserI2c_DelayuS(10);
	USERI2C_SCL=1; 
  ucErrTime = 10000;
  while( ucErrTime-- > 0 )
  {
    if(USERI2C_READ_SDA )
    {
      RetValue = 1;     			
    }
    else
    {
      RetValue = 0;
			break;
    }
  }
	USERI2C_SCL=0;//时钟输出0 
 	UserI2c_DelayuS(10);	
	USERI2C_SDA_OUT();//sda线输出 
	USERI2C_SDA=1; 	
	return RetValue;  
} 
/*******************************************************************************
* Function Name  : useri2c_ack
* Description    : 第9个时钟上升沿发送低电平ack
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void useri2c_ack(void)
{
	USERI2C_SCL=0;
	USERI2C_SDA=0;
	UserI2c_DelayuS(10);
	USERI2C_SCL=1;
	UserI2c_DelayuS(10);
	USERI2C_SCL=0;
}

/*******************************************************************************
* Function Name  : useri2c_nack
* Description    : 第9个时钟上升沿发送高电平nack
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/   
void useri2c_nack(void)
{
	USERI2C_SCL=0;
	USERI2C_SDA=1;
	UserI2c_DelayuS(10);
	USERI2C_SCL=1;
	UserI2c_DelayuS(10);
	USERI2C_SCL=0;
}					
 
/*******************************************************************************
* Function Name  : UserI2c_Send_Byte
* Description    : 写1字节数据到i2c总线
* Input          : dat-要发送的数据
* Output         : None
* Return         : None
*******************************************************************************/	  
void UserI2c_Send_Byte(unsigned char dat)
{                        
	unsigned char i; 
  	    
	USERI2C_SCL=0; 	 
	UserI2c_DelayuS(10); 
	for(i=0;i<8;i++)
	{         
		if((dat&0x80)>>0)
			USERI2C_SDA=1;
		else
			USERI2C_SDA=0;
		dat<<=1; 	   
		USERI2C_SCL=1;
		UserI2c_DelayuS(10);
		USERI2C_SCL=0; 	 
		UserI2c_DelayuS(10); 
	}	 
} 	 
/*******************************************************************************
* Function Name  : UserI2c_Read_Byte
* Description    : 从i2c总线读1字节数据
* Input          : None
* Output         : None
* Return         : 读到的1字节数据
*******************************************************************************/ 
unsigned char UserI2c_Read_Byte(void)
{
	unsigned char i,receive=0;
	
	USERI2C_SDA_IN();//SDA设置为输入
	USERI2C_SDA=1;
	UserI2c_DelayuS(10);
	for(i=0;i<8;i++ )
	{ 
		USERI2C_SCL=0;
		UserI2c_DelayuS(10);
		USERI2C_SCL=1; 
		UserI2c_DelayuS(10);
		receive<<=1; 
		if(USERI2C_READ_SDA)
			receive++;  
	}				
	USERI2C_SCL=0;   
	USERI2C_SDA_OUT();//sda线输出 
	return receive;
}

/*******************************************************************************
* Function Name  : SensorWritenByte
* Description    : Sensor read api.
* Input          : *txbuff-the buffer which to write.
*                : regaddr-the address write to
*                : size-write data size
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SensorWritenByte(unsigned char *txbuff, unsigned char regaddr, unsigned char size)
{
	unsigned char i = 0;
  
	UserI2c_Start();

  UserI2c_Send_Byte( I2C_DEVID|0x00 );
  if( 1 == UserI2c_Wait_Ack() )
  {
		UserI2c_Stop();
    return 1;
  }
  UserI2c_Send_Byte( regaddr&0xff );
  if( 1 == UserI2c_Wait_Ack() )
  {
		UserI2c_Stop();
    return 1;
  }
	for ( i = 0; i < size; i++)
	{
		UserI2c_Send_Byte( txbuff[size-i-1] );
		if( 1 == UserI2c_Wait_Ack() )
		{
			UserI2c_Stop();
			return 1;
		}
	}
  UserI2c_Stop();
	
  return 0;
}
/*******************************************************************************
* Function Name  : SensorReadnByte
* Description    : Sensor read api.
* Input          : *rxbuff-the buffer which stores read content.
*                : regaddr-the address read from
*                : size-read data size
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SensorReadnByte(unsigned char *rxbuff, unsigned char regaddr, unsigned char size)
{
  unsigned char i = 0;
	
  UserI2c_Start();

  UserI2c_Send_Byte( I2C_DEVID|0x00 );
  if( 1 == UserI2c_Wait_Ack() )
  {
		UserI2c_Stop();
    return 1;
  }
  UserI2c_Send_Byte( regaddr&0xff );
  if( 1 == UserI2c_Wait_Ack() )
  {
		UserI2c_Stop();
    return 1;
  }
	UserI2c_Stop();
  UserI2c_Start();
  UserI2c_Send_Byte( I2C_DEVID|0x01 );
  if( 1 == UserI2c_Wait_Ack() )
  {
		UserI2c_Stop();
    return 1;
  }
	UserI2c_DelayuS(30);//30uS
	for ( i = 0; i < size; i++)
	{
		rxbuff[size-i-1] = UserI2c_Read_Byte();	
		if((i+1)==size)			
			useri2c_nack();
		else
			useri2c_ack();
		UserI2c_DelayuS(30);//30uS
	}
  UserI2c_Stop();
	
  return 0;
}
/*******************************************************************************
* Function Name  : UserI2c_Init
* Description    : config i2c driver gpio
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UserI2c_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9); 	//PB8,PB9 输出高
}
/*******************************************************************************
* Function Name  : Sensor_I2C_Test
* Description    : I2C 读写测试
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Sensor_I2C_Test(void)
{			
	dirt_send_flag=1;
	SensorWritenByte((unsigned char *)&dirt_send_flag, 0x09, 1);
	Delay_mS(100);
	dirt_detection_flag=0;
	SensorWritenByte((unsigned char *)&dirt_detection_flag, 0x08, 1);
	Delay_mS(100);
 	while(1)
	{			
    IWDG_ReloadCounter();  					//喂狗	
		if(useruart_rxcmd_flag==1)
		{	
			if(dirt_send_flag==0)
			{
				CalDisp_Length();
			}
			useruart_rxcmd_flag=0;
		}
		else if((dirt_send_flag==1)&&(i2cread_interval>=100))//100mS读1次
		{
			i2cread_interval=0;
			SensorReadnByte((unsigned char *)&length_aveval, 0x00,2);//(unsigned char *)&length_val		
			if(length_aveval>0)			
				Display_i2cLength(length_aveval);
		}
		Delay_mS(1);
	}  // while		
}	
/************************END OF FILE*************************/
