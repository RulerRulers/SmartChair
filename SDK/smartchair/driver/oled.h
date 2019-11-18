#ifndef OLED_H
#define OLED_H

void delay(unsigned int z); // ��ʱ����,����
void IIC_Start(); // ����I2C����
void IIC_Stop(); // �ر�I2C����
void Write_IIC_Byte(unsigned char IIC_Byte); // ͨ��I2C����дһ��byte������
void OLED_WrDat(unsigned char dat); // ��OLED��д����
void OLED_WrCmd(unsigned char cmd); // ��OLED��д����
void OLED_Set_Pos(unsigned char x, unsigned char y); // ������ʾ����
void OLED_Fill(unsigned char bmp_dat); // ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
void OLED_CLS();// ��λ/����
void OLED_Init();// OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
void OLED_P6x8Str(unsigned char x, y,unsigned char dat); // 6x8������������ʾASCII�����С���У���̫����

void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]); // 8x16������������ʾASCII�룬�ǳ�����
void OLED_P8x16Str_t(unsigned char x, y,unsigned char ch[]);//������ʾ

void OLED_P16x16Ch(unsigned char x, y, N); // 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]); //��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���

void oled_show_num(unsigned char x, unsigned char y, unsigned int num,unsigned char mode);

void OLED_ClearLine(unsigned char line);//���ĳһ��


#endif


