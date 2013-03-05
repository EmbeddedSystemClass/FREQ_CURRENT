#include "frequency.h"
#include "dac.h"
#include "preferences.h"
#include <stdio.h>

#define FREQ_CHANNELS	1


#define FRAME_TIME_1	511  //границы кадров захвата
#define FRAME_TIME_2	1023
//#define FRAME_TIME_3	1535
//#define FRAME_TIME_4	2047
#define FRAME_TIME_MAX	((FREQ_FRAME<<9)-1)//10239 //10 секунд

#define SHEAR			9	  //рассчитываетс€ из максимума кадра захвата, как Nбит FRAME_TIME_1

//#define LED_FLASH_TIME	1000

# define CLI_EXT  EX1=0; EX0=0; ET0=0;
# define STI_EXT  EX1=1; EX0=1; ET0=1;

//----------------основные настройки-------------------------
#define I04_mA 	0x333
#define I20_mA	0xFFF

#define	FREQ_MIN	0x0
#define FREQ_MAX	100//Hz
//-----------------------------------------------------------

 sbit INT_1=P3^2;
 sbit T_0=P3^4;
 sbit LED=P2^6;
 sbit IMP=P2^7;

 //sbit FLAG=P2^5;


volatile unsigned char data freq_ready=0;//измерение готово
//------------------------------------------
volatile struct Frequency xdata frequency[FREQ_CHANNELS] ; //структура частотных каналов


//------------------------------------------
#define FRQ_CHNL_1 0
//#define FRQ_CHNL_2 1
//#define FRQ_CHNL_3 2
//--------------------------------------------------------
void Frequency_Init(void) //инициализаци€ частотных каналов
{
//	Timer0_Initialize();
	unsigned char data i=0;
	Timer2_Initialize();
//	EX1=1;//включаем внешние прерывани€
	EX0=1;
	IT0=1;
//	IT1=1;
	INT_1=1;
	T_0=1;
	LED=0;

	frequency[FRQ_CHNL_1].time_counter=0;

	for(i=0;i<FREQ_FRAME;i++)	//суммируем значени€ всех кадров
	{
		frequency[FRQ_CHNL_1].frame[i].event_copy=0;
		frequency[FRQ_CHNL_1].frame[i].time_copy=0;
		frequency[FRQ_CHNL_1].frame[i].timestamp=0;	
		frequency[FRQ_CHNL_1].frame[i].event_counter=0;	
	}


	return;
}
//--------------------------------------------------------
void INT0_ISR(void) interrupt 0 //using 3//обработчик внешнего прерывани€ 0
{

	volatile unsigned char data cnt;
EA=0;
	cnt=((frequency[FRQ_CHNL_1].time_counter>>SHEAR)&0x1F);
	frequency[FRQ_CHNL_1].frame[cnt].event_counter++;
	frequency[FRQ_CHNL_1].frame[cnt].timestamp=frequency[FRQ_CHNL_1].time_counter;
	IMP=0;
	TR1=1;
EA=1;
	return;
}
//--------------------------------------------------------
void Timer2_ISR(void) interrupt 5 //using 3//обработчик прерывани€ счетного таймера частоты
{

	volatile unsigned char data temp_index;
//	EA=0;
	TF2 = 0;

	if(((frequency[FRQ_CHNL_1].time_counter)&FRAME_TIME_1)==FRAME_TIME_1)//через каждые полсекунды
	{	
		switch(frequency[FRQ_CHNL_1].time_counter)
		{
			case FRAME_TIME_1:
			{
				frequency[FRQ_CHNL_1].frame[0].time_copy=frequency[FRQ_CHNL_1].frame[0].timestamp+FRAME_TIME_MAX-frequency[FRQ_CHNL_1].frame[FREQ_FRAME-1].timestamp;
				frequency[FRQ_CHNL_1].frame[0].event_copy=frequency[FRQ_CHNL_1].frame[0].event_counter;	
	
				frequency[FRQ_CHNL_1].frame[FREQ_FRAME-1].event_counter=0;
			}
			break;
			
//			case FRAME_TIME_1:
//			{
//				frequency[FRQ_CHNL_1].frame[FREQ_FRAME-1].time_copy=frequency[FRQ_CHNL_1].frame[FREQ_FRAME-1].timestamp-frequency[FRQ_CHNL_1].frame[FREQ_FRAME-2].timestamp;
//				frequency[FRQ_CHNL_1].frame[FREQ_FRAME-1].event_copy=frequency[FRQ_CHNL_1].frame[FREQ_FRAME-1].event_counter;	
//	
//				frequency[FRQ_CHNL_1].frame[FREQ_FRAME-2].event_counter=0;		
//			}
//			break;
			
			default:
			{
				temp_index=(unsigned char)(frequency[FRQ_CHNL_1].time_counter>>SHEAR);
				frequency[FRQ_CHNL_1].frame[temp_index].time_copy=frequency[FRQ_CHNL_1].frame[temp_index].timestamp-frequency[FRQ_CHNL_1].frame[temp_index-1].timestamp;
				frequency[FRQ_CHNL_1].frame[temp_index].event_copy=frequency[FRQ_CHNL_1].frame[temp_index].event_counter;	
	
				frequency[FRQ_CHNL_1].frame[temp_index-1].event_counter=0;
			}
			break;	
		}
		freq_ready=1;//готово
	}
		
	frequency[FRQ_CHNL_1].time_counter++;
	if(frequency[FRQ_CHNL_1].time_counter>=(FRAME_TIME_MAX+1))
	{
		frequency[FRQ_CHNL_1].time_counter=0;
	}
//EA=1;
	return;
}
//------------------------------------------------------------
unsigned char Frequency_Measure_Process(void)//циклический процесс измерени€ частоты
{	
  
    volatile unsigned long data  temp_time=0;
	volatile unsigned int data  temp_freq=0;
    volatile unsigned char data i=0;

	union 
	{ 
		unsigned long long_num; 
		unsigned int  int_num[2];
	}  volatile data temp_event;

   
    if(freq_ready==0)
    {
		return 0;		
    }

    temp_event.long_num=0;
	temp_time=0;

   	for(i=0;i<FREQ_FRAME;i++)	//суммируем значени€ всех кадров
	{
		temp_event.int_num[0]+= frequency[FRQ_CHNL_1].frame[i].event_copy<<2;
		temp_time+=  frequency[FRQ_CHNL_1].frame[i].time_copy;		
	}



	if((temp_time!=0)&&(temp_event.long_num!=(unsigned long)0))
	{
		temp_freq=temp_event.long_num/(unsigned int)temp_time;
	}
	else
	{
		  temp_freq=0;
	}

	printf("%u %u %u\n",(unsigned int)temp_freq,temp_event.int_num[0]>>2,(unsigned int)temp_time);

	
	if(temp_freq>=0xFFFF)
	{
		temp_freq=0xFFFF;
	}

	if(temp_freq==0) //мигаем светодиодом при наличии частоты
	{
		LED=0;
	}
	else
	{
		LED=!LED;
	}


	temp_freq=((temp_freq-((unsigned long)FREQ_MIN<<8))*((unsigned long)I20_mA-(unsigned long)I04_mA))/(((unsigned long)FREQ_MAX-(unsigned long)FREQ_MIN)<<8)+(unsigned long)I04_mA;

   	if(temp_freq>=I20_mA)
	{
		temp_freq=I20_mA;
	}

//	if(temp_freq<=I04_mA)
//	{
//		temp_freq=I04_mA;	
//	}
	
	DAC_Set_Value((unsigned int)temp_freq);

    freq_ready=0;
	return 0;
}
//------------------------------------------------------------
void Timer1_ISR(void) interrupt 3 //using 3//обработчик прерывани€ счетного таймера частоты
{
	IMP=1;	//передний фронт

	TR1=0;
	TF1 = 0;
	TH1=TH1_VAL;
	TL1=TL1_VAL;
}