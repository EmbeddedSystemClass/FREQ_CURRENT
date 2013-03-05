#include <ADuC845.h>
#include "timer3.h"
#include "preferences.h"
#include "watchdog.h"
#include "frequency.h"
#include "dac.h"
#include "timer1.h"


 //---------------------------------------

void main(void) //using 0
{			   
	EA = 0;
	
	//CFG845=0x1;//enable xram
	
	PLLCON&=PLLCON_VAL;//��������� ������� ����������
//-------printf--------
//    T3CON = T3CON_VAL;
//
//    T3FD = T3FD_VAL;
//    SCON =0x52; //0x53;
//---------------------
	
	Timer1_Initialize(); //������ ��������� �������

	Frequency_Init();
	DAC_Init();
	WDT_Init(WDT_2000);//�������� ���������� ������

	EA=1;

	while(1)
	{	
	  Frequency_Measure_Process();
	  WDT_Clear();
	}
}


 //---------------------------------
