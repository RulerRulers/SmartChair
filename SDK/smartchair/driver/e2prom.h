#ifndef E2PROM_H
#define E2PROM_H

/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE    0               //Stand-By
#define CMD_READ    1               //Byte-Read
#define CMD_PROGRAM 2               //Byte-Program
#define CMD_ERASE   3               //Sector-Erase

#define ENABLE_IAP  0x81            //if SYSCLK<20MHz
#define IAP_ADDRESS 0x0 
#define paramADDRESS 100


typedef struct parameter
{
  unsigned char PressureValue;//Ñ¹Á¦ãÐÖµ
  unsigned char DistanceValue;//¾àÀëãÐÖµ
  unsigned char DistanceDiffLimit;//¾àÀë²îµÄ ãÐÖµ  
}parameterTypedef;
extern parameterTypedef myparam;

unsigned char IapReadByte(unsigned int addr);
void IapWriteByte(unsigned int addr, unsigned char dat);
void IapEraseSector(unsigned int addr);
void TestE2PROM();

void GetParam(parameterTypedef *param);
void SetParam(parameterTypedef *param);
void MyParamInit(parameterTypedef *param);




#endif


