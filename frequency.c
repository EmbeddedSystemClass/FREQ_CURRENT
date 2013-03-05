#include "frequency.h"
#include "dac.h"
#include "preferences.h"
#define FREQ_CHANNELS	1


#define FRAME_TIME_1	511  //границы кадров захвата
#define FRAME_TIME_2	1023
#define FRAME_TIME_3	1535
#define FRAME_TIME_4	2047

#define SHEAR			9	  //рассчитывается из максимума кадра захвата, как Nбит-2

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

unsigned long data temp_freq=0;
unsigned char volatile data measure_state=0;//автомат состояний для оптимизации процесса вычисления(уменьшение нагрузки на процессор)
volatile unsigned char data freq_ready=0;//измерение готово
//------------------------------------------
volatile struct Frequency data frequency[FREQ_CHANNELS] ; //структура частотных каналов
//------------------------------------------
#define FRQ_CHNL_1 0
//#define FRQ_CHNL_2 1
//#define FRQ_CHNL_3 2
//--------------------------------------------------------
void Frequency_Init(void) //инициализация частотных каналов
{
//	Timer0_Initialize();
	Timer2_Initialize();
//	EX1=1;//включаем внешние прерывания
	EX0=1;
	IT0=1;
//	IT1=1;
	INT_1=1;
	T_0=1;
	LED=0;

	frequency[FRQ_CHNL_1].time_counter=0;
	return;
}
//--------------------------------------------------------
void INT0_ISR(void) interrupt 0 //using 3//обработчик внешнего прерывания 0
{
	unsigned char cnt;
	cnt=(frequency[FRQ_CHNL_1].time_counter>>SHEAR)&0x3;
	frequency[FRQ_CHNL_1].frame[cnt].event_counter++;
	frequency[FRQ_CHNL_1].frame[cnt].timestamp=frequency[FRQ_CHNL_1].time_counter;
	IMP=0;
	TR1=1;
//	LED=1;
	return;
}
//--------------------------------------------------------
void Timer2_ISR(void) interrupt 5 //using 3//обработчик прерывания счетного таймера частоты
{
	TF2 = 0;

 	switch(frequency[FRQ_CHNL_1].time_counter)
	{
		case FRAME_TIME_1:
		{
			frequency[FRQ_CHNL_1].frame[0].time_copy=frequency[FRQ_CHNL_1].frame[0].timestamp+FRAME_TIME_4-frequency[FRQ_CHNL_1].frame[3].timestamp;
			frequency[FRQ_CHNL_1].frame[0].event_copy=frequency[FRQ_CHNL_1].frame[0].event_counter;	


			frequency[FRQ_CHNL_1].frame[3].event_counter=0;
			freq_ready=1;
			//---------------------------------------
		}
		break;

		case FRAME_TIME_2:
		{
			frequency[FRQ_CHNL_1].frame[1].time_copy=frequency[FRQ_CHNL_1].frame[1].timestamp-frequency[FRQ_CHNL_1].frame[0].timestamp;
			frequency[FRQ_CHNL_1].frame[1].event_copy=frequency[FRQ_CHNL_1].frame[1].event_counter;	

			frequency[FRQ_CHNL_1].frame[0].event_counter=0;
			freq_ready=1;
			//----------------------------------------------
		}
		break;

		case FRAME_TIME_3:
		{
			frequency[FRQ_CHNL_1].frame[2].time_copy=frequency[FRQ_CHNL_1].frame[2].timestamp-frequency[FRQ_CHNL_1].frame[1].timestamp;
			frequency[FRQ_CHNL_1].frame[2].event_copy=frequency[FRQ_CHNL_1].frame[2].event_counter;	

			frequency[FRQ_CHNL_1].frame[1].event_counter=0;
			freq_ready=1;
			//----------------------------------------------------
		}
		break;

		case FRAME_TIME_4:
		{
			frequency[FRQ_CHNL_1].frame[3].time_copy=frequency[FRQ_CHNL_1].frame[3].timestamp-frequency[FRQ_CHNL_1].frame[2].timestamp;
			frequency[FRQ_CHNL_1].frame[3].event_copy=frequency[FRQ_CHNL_1].frame[3].event_counter;	

			frequency[FRQ_CHNL_1].frame[2].event_counter=0;	
			freq_ready=1;
			//-----------------------------------------------------
		}
		break;
	}

	frequency[FRQ_CHNL_1].time_counter++;
	frequency[FRQ_CHNL_1].time_counter&=FRAME_TIME_4;

	return;
}
//------------------------------------------------------------
unsigned char Frequency_Measure_Process(void)//циклический процесс измерения частоты
{	
   if(freq_ready==0)
   {
   		return 0;
   }
	
	temp_freq=(((unsigned long)frequency[FRQ_CHNL_1].frame[0].event_copy+(unsigned long)frequency[FRQ_CHNL_1].frame[1].event_copy+(unsigned long)frequency[FRQ_CHNL_1].frame[2].event_copy+(unsigned long)frequency[FRQ_CHNL_1].frame[3].event_copy)<<18/*18*/)/((frequency[FRQ_CHNL_1].frame[0].time_copy+frequency[FRQ_CHNL_1].frame[1].time_copy+frequency[FRQ_CHNL_1].frame[2].time_copy+frequency[FRQ_CHNL_1].frame[3].time_copy));
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
void Timer1_ISR(void) interrupt 3 //using 3//обработчик прерывания счетного таймера частоты
{
	IMP=1;	//передний фронт

	TR1=0;
	TF1 = 0;
	TH1=TH1_VAL;
	TL1=TL1_VAL;
}