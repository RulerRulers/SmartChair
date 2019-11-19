#include <STC12C5A60S2.H>
#include "intrins.h"
#include "e2prom.h"
#include "stdio.h"

//STC12C5A60S2 EEPROM的地址范围为：0x0000---0x03ff(0-1023)

parameterTypedef myparam;//参数结构体

/*----------------------------
Software delay function
----------------------------*/
// static void Delay(unsigned char n)
// {
//     unsigned int x;

//     while (n--)
//     {
//         x = 0;
//         while (++x);
//     }
// }

/*----------------------------
Disable ISP/IAP/EEPROM function
Make MCU in a safe state
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //Close IAP function
    IAP_CMD = 0;                    //Clear command to standby
    IAP_TRIG = 0;                   //Clear trigger register
    IAP_ADDRH = 0x80;               //Data ptr point to non-EEPROM area
    IAP_ADDRL = 0;                  //Clear IAP address to prevent misuse
}

/*----------------------------
Read one unsigned char from ISP/IAP/EEPROM area
Input: addr (ISP/IAP/EEPROM address)
Output:Flash data
----------------------------*/
unsigned char IapReadByte(unsigned int addr)
{
    unsigned char dat;                       //Data buffer

	  EA = 0;
    IAP_CONTR = ENABLE_IAP;         //Open IAP function, and set wait time
    IAP_CMD = CMD_READ;             //Set ISP/IAP/EEPROM READ command
    IAP_ADDRL = addr;               //Set ISP/IAP/EEPROM address low
    IAP_ADDRH = addr >> 8;          //Set ISP/IAP/EEPROM address high
    IAP_TRIG = 0x5a;                //Send trigger command1 (0x5a)
    IAP_TRIG = 0xa5;                //Send trigger command2 (0xa5)
    _nop_();                        //MCU will hold here until ISP/IAP/EEPROM operation complete
    dat = IAP_DATA;                 //Read ISP/IAP/EEPROM data
    IapIdle();                      //Close ISP/IAP/EEPROM function
    EA = 1;
    return dat;                     //Return Flash data
}

/*----------------------------
Program one unsigned char to ISP/IAP/EEPROM area
Input: addr (ISP/IAP/EEPROM address)
       dat (ISP/IAP/EEPROM data)
Output:-
----------------------------*/
void IapWriteByte(unsigned int addr, unsigned char dat)
{
	  EA = 0;
    IAP_CONTR = ENABLE_IAP;         //Open IAP function, and set wait time
    IAP_CMD = CMD_PROGRAM;          //Set ISP/IAP/EEPROM PROGRAM command
    IAP_ADDRL = addr;               //Set ISP/IAP/EEPROM address low
    IAP_ADDRH = addr >> 8;          //Set ISP/IAP/EEPROM address high
    IAP_DATA = dat;                 //Write ISP/IAP/EEPROM data
    IAP_TRIG = 0x5a;                //Send trigger command1 (0x5a)
    IAP_TRIG = 0xa5;                //Send trigger command2 (0xa5)
    _nop_();                        //MCU will hold here until ISP/IAP/EEPROM operation complete
	  EA = 1;
    IapIdle();
}

/*----------------------------
Erase one sector area
Input: addr (ISP/IAP/EEPROM address)
Output:-
----------------------------*/
void IapEraseSector(unsigned int addr)
{
    IAP_CONTR = ENABLE_IAP;         //Open IAP function, and set wait time
    IAP_CMD = CMD_ERASE;            //Set ISP/IAP/EEPROM ERASE command
    IAP_ADDRL = addr;               //Set ISP/IAP/EEPROM address low
    IAP_ADDRH = addr >> 8;          //Set ISP/IAP/EEPROM address high
    IAP_TRIG = 0x5a;                //Send trigger command1 (0x5a)
    IAP_TRIG = 0xa5;                //Send trigger command2 (0xa5)
    _nop_();                        //MCU will hold here until ISP/IAP/EEPROM operation complete
    IapIdle();
}
//Function:任意地址写入一串数据
//Description：向AT24C02写入一串数据
//Input:写地址,写数据,数据长度
//Output:无
//Return:无
//Others:无
// void TOF_Serial_Write(unsigned char Writeaddr,unsigned char *Str,unsigned char Len)
// {
// 	while(Len--)
// 	{
// 		IapWriteByte(Writeaddr,*Str);
// 		Writeaddr++;
// 		Str++;
// 	}
// }


//Function:指定地址读出一串数据
//Description：向AT24C02读出一串数据
//Input:读地址,读数据,数据长度
//Output:无
//Return:无
//Others:无
// void TOF_Serial_Read(unsigned char Readaddr,unsigned char *Str,unsigned char Len)
// {
// 	while(Len)
// 	{
// 		*Str++ = IapReadByte(Readaddr++);
// 		Len--;
// 	}
// }

// void TestE2PROM()
// {
//     unsigned int i;                   //Delay
//     IapEraseSector(IAP_ADDRESS);    //Erase current secto
// 	  printf("1.Read:\r\n");
//     for (i=0; i<255; i++)           //Check whether all sector data is FF
//     {
// 				printf("%bd ",IapReadByte(IAP_ADDRESS+i));	
//     }
// 		printf("\r\n");
// 		
//     Delay(10);  		//Delay
// 		printf("2.Write:\r\n");
//     for(i=0; i<255; i++)           //Program 512 bytes data into data flash
//     {
//         IapWriteByte(IAP_ADDRESS+i, (unsigned char)i);
// 			  printf("%bd ",(unsigned char)i);
//     }
// 		printf("\r\n");
//     Delay(10);		//Delay	
// 		printf("3.Read:\r\n");
//     for(i=0; i<255; i++)           //Verify 512 bytes data
//     {
//        printf("%bd ",IapReadByte(IAP_ADDRESS+i));	      
//     }
// 		printf("\r\n");
// 		printf("End\r\n");
// }

void GetParam(parameterTypedef *param)
{
   param->PressureValue = IapReadByte(paramADDRESS);//压力阈值10%
	 param->DistanceValue =IapReadByte(paramADDRESS+1);//距离阈值 3cm	   
   param->DistanceDiffLimit =IapReadByte(paramADDRESS+2);//距离差 阈值	 	
}

void SetParam(parameterTypedef *param)
{
	IapEraseSector(10);    //Erase current secto 
  IapWriteByte(paramADDRESS, param->PressureValue);
	IapWriteByte(paramADDRESS+1, param->DistanceValue);
	IapWriteByte(paramADDRESS+2, param->DistanceDiffLimit);
}


void MyParamInit(parameterTypedef *param)
{
   if(IapReadByte(600) != 0x5A)//没有参数  需要先设置默认值
	 {
		 IapEraseSector(10);    //Erase current secto
		 IapEraseSector(600); //Erase current secto
     IapWriteByte(600,0x5A);
     param->PressureValue = 10;//压力阈值10%
		 param->DistanceValue =3;//距离阈值 3cm	   
		 param->DistanceDiffLimit = 50;
		 SetParam(param);//设置参数
		 //TOF_Serial_Write(100,(unsigned char*)param,sizeof(param));
	 }
	 GetParam(param);//获取参数+
   //TOF_Serial_Read(100,(unsigned char*)param,sizeof(param));    
}






