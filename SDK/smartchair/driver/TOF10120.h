#ifndef TOF10120_H_
#define TOF10120_H_
void TOF_Init(void);
void TOF_Write_Byte(unsigned char addr,unsigned char dat);
unsigned char TOF_Read_Byte(unsigned char addr);
unsigned int GetDistance(unsigned char ch);
unsigned int DistanDiffer(unsigned int x,unsigned int y);


#endif






