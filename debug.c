#include <ADuC841.h>
#include "timer3.h"
#include "preferences.h"
#include "watchdog.h"
#include "frequency.h"
sbit LED1=P3^4;
sbit LED2=P3^5;
sbit LED3=P3^6;
 //---------------------------------------

void main(void) //using 0
{			   
	EA = 0;
	
	//CFG845=0x1;//enable xram
	
	PLLCON&=PLLCON_VAL;//настройка частоты процессора

	
//	Timer1_Initialize(); //таймер шедулера 200√ц

	Frequency_Init();

//	WDT_Init(WDT_2000);//включить сторожевой таймер
	P2=0x0;
	LED1=0;
	LED2=0;
	LED3=0;
	EA=1;

	while(1)
	{	
	  Frequency_Measure_Process();
	}
}


 //---------------------------------
