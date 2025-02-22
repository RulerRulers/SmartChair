#ifndef OLED_H
#define OLED_H

void delay(unsigned int z); // 延时函数,毫秒
void IIC_Start(); // 开启I2C总线
void IIC_Stop(); // 关闭I2C总线
void Write_IIC_Byte(unsigned char IIC_Byte); // 通过I2C总线写一个byte的数据
void OLED_WrDat(unsigned char dat); // 向OLED屏写数据
void OLED_WrCmd(unsigned char cmd); // 向OLED屏写命令
void OLED_Set_Pos(unsigned char x, unsigned char y); // 设置显示坐标
void OLED_Fill(unsigned char bmp_dat); // 全屏显示(显示BMP图片时才会用到此功能)
void OLED_CLS();// 复位/清屏
void OLED_Init();// OLED屏初始化程序，此函数应在操作屏幕之前最先调用
void OLED_P6x8Str(unsigned char x, y,unsigned char dat); // 6x8点整，用于显示ASCII码的最小阵列，不太清晰

void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]); // 8x16点整，用于显示ASCII码，非常清晰
void OLED_P8x16Str_t(unsigned char x, y,unsigned char ch[]);//反白显示

void OLED_P16x16Ch(unsigned char x, y, N); // 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]); //将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可

void oled_show_num(unsigned char x, unsigned char y, unsigned int num,unsigned char mode);

void OLED_ClearLine(unsigned char line);//清除某一行


#endif


