#include <STC12C5A60S2.H>
#include "intrins.h"
#include "adc.h"


unsigned int xdata adcResult[6];
unsigned int xdata adcResult1[6];

unsigned int CurrentPress;//实际压力值
unsigned int CurrentPress1;//实际压力值

/*----------------------------
Get ADC result  10bit
----------------------------*/
unsigned int GetADCResult(unsigned char ch)//ch=0~7  对应 P1.0~P1.7
{
	   unsigned char result;
     ADC_CONTR = ADC_POWER |	ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
	  _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC
    //return (ADC_RES<<2+ADC_RESL); //Return ADC result  10bit
	  result = ADC_RES;
	  if(result == 0) result =1;//避免出现0
	  return result; //Return ADC result  8bit
}


/*----------------------------
Initial ADC sfr
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //Open 8 channels ADC function
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                       //ADC power-on and delay
}


/*----------------------------
Software delay function
----------------------------*/
void Delay(unsigned int n)
{
    unsigned int x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}


unsigned int average(unsigned int a[], unsigned int n)
{
	float max=a[0];
	float min=a[0];
	float sum=0;
  unsigned char i;
	for(i=0;i<n;i++)
	{
		sum+=a[i];
 
		max=max<a[i]? a[i]:max;
		min=min>a[i]? a[i]:min;
	}
 
	return (unsigned int)((sum-max-min)/(n-2));
}
