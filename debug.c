#include <ADuC845.h>
#include "timer3.h"
#include "preferences.h"
#include "watchdog.h"
#include "frequency.h"
#include "dac.h"
sbit LED1=P3^4;
sbit LED2=P3^5;
sbit LED3=P3^6;
 //---------------------------------------

void main(void) //using 0
{			   
	EA = 0;
	
	//CFG845=0x1;//enable xram
	
	PLLCON&=PLLCON_VAL;//��������� ������� ����������

	
//	Timer1_Initialize(); //������ �������� 200��

	Frequency_Init();
	DAC_Init();
//	WDT_Init(WDT_2000);//�������� ���������� ������

	EA=1;

	while(1)
	{	
	  Frequency_Measure_Process();
	}
}


 //---------------------------------
