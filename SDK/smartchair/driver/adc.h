#ifndef ADC_H
#define ADC_H


/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks


extern unsigned int xdata adcResult[6];
extern unsigned int xdata adcResult1[6];

extern unsigned int CurrentPress;//实际压力值
extern unsigned int CurrentPress1;//实际压力值

void InitADC();
unsigned int GetADCResult(unsigned char ch);
void Delay(unsigned int n);
unsigned int average(unsigned int a[], unsigned int n);


#endif


