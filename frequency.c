#include "frequency.h"
#include "dac.h"

#define FREQ_CHANNELS	1


#define FRAME_TIME_1	511  //������� ������ �������
#define FRAME_TIME_2	1023
#define FRAME_TIME_3	1535
#define FRAME_TIME_4	2047

#define SHEAR			9	  //�������������� �� ��������� ����� �������, ��� N���-2

# define CLI_EXT  EX1=0; EX0=0; ET0=0;
# define STI_EXT  EX1=1; EX0=1; ET0=1;



unsigned long temp_freq=0;
unsigned char volatile idata measure_state=0;//������� ��������� ��� ����������� �������� ����������(���������� �������� �� ���������)
volatile unsigned char freq_ready=0;//��������� ������
//------------------------------------------
volatile struct Frequency xdata frequency[FREQ_CHANNELS] ; //��������� ��������� �������
//------------------------------------------
#define FRQ_CHNL_1 0
//#define FRQ_CHNL_2 1
//#define FRQ_CHNL_3 2
//--------------------------------------------------------
void Frequency_Init(void) //������������� ��������� �������
{
//	Timer0_Initialize();
	Timer2_Initialize();
//	EX1=1;//�������� ������� ����������
	EX0=1;
	IT0=1;
//	IT1=1;
	frequency[FRQ_CHNL_1].time_counter=0;
	return;
}
//--------------------------------------------------------
void INT0_ISR(void) interrupt 0 //using 3//���������� �������� ���������� 0
{
	unsigned char cnt;
	cnt=(frequency[FRQ_CHNL_1].time_counter>>SHEAR)&0x3;
	frequency[FRQ_CHNL_1].frame[cnt].event_counter++;
	frequency[FRQ_CHNL_1].frame[cnt].timestamp=frequency[FRQ_CHNL_1].time_counter;
	return;
}
//--------------------------------------------------------
/*void INT1_ISR(void) interrupt 2 //using 3//���������� �������� ���������� 1
{
 	unsigned char cnt;
	cnt=(frequency[FRQ_CHNL_1].time_counter>>SHEAR)&0x3;
	frequency[FRQ_CHNL_2].frame[cnt].event_counter++;
	frequency[FRQ_CHNL_2].frame[cnt].timestamp=frequency[FRQ_CHNL_1].time_counter;
	return;
}
//--------------------------------------------------------
void INT2_ISR(void) interrupt 1 //using 3//���������� �������� ���������� 2-���������� ������� ���� ������� T0
{
	unsigned char cnt;
		
	TH0=0xFF;
	TL0=0xFF;

	cnt=(frequency[FRQ_CHNL_1].time_counter>>SHEAR)&0x3;

	frequency[FRQ_CHNL_3].frame[cnt].event_counter++;
	frequency[FRQ_CHNL_3].frame[cnt].timestamp=frequency[FRQ_CHNL_1].time_counter;
	return;
} */
//--------------------------------------------------------
void Timer2_ISR(void) interrupt 5 //using 3//���������� ���������� �������� ������� �������
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
//			frequency[FRQ_CHNL_2].frame[0].time_copy=frequency[FRQ_CHNL_2].frame[0].timestamp+FRAME_TIME_4-frequency[FRQ_CHNL_2].frame[3].timestamp;
//			frequency[FRQ_CHNL_2].frame[0].event_copy=frequency[FRQ_CHNL_2].frame[0].event_counter;	
//
//
//			frequency[FRQ_CHNL_2].frame[3].event_counter=0;
//
//			//---------------------------------------
//			frequency[FRQ_CHNL_3].frame[0].time_copy=frequency[FRQ_CHNL_3].frame[0].timestamp+FRAME_TIME_4-frequency[FRQ_CHNL_3].frame[3].timestamp;
//			frequency[FRQ_CHNL_3].frame[0].event_copy=frequency[FRQ_CHNL_3].frame[0].event_counter;	
//
//
//			frequency[FRQ_CHNL_3].frame[3].event_counter=0;

		}
		break;

		case FRAME_TIME_2:
		{
			frequency[FRQ_CHNL_1].frame[1].time_copy=frequency[FRQ_CHNL_1].frame[1].timestamp-frequency[FRQ_CHNL_1].frame[0].timestamp;
			frequency[FRQ_CHNL_1].frame[1].event_copy=frequency[FRQ_CHNL_1].frame[1].event_counter;	

			frequency[FRQ_CHNL_1].frame[0].event_counter=0;
			freq_ready=1;
			//----------------------------------------------
//			frequency[FRQ_CHNL_2].frame[1].time_copy=frequency[FRQ_CHNL_2].frame[1].timestamp-frequency[FRQ_CHNL_2].frame[0].timestamp;
//			frequency[FRQ_CHNL_2].frame[1].event_copy=frequency[FRQ_CHNL_2].frame[1].event_counter;	
//
//			frequency[FRQ_CHNL_2].frame[0].event_counter=0;
//			//----------------------------------------------
//			frequency[FRQ_CHNL_3].frame[1].time_copy=frequency[FRQ_CHNL_3].frame[1].timestamp-frequency[FRQ_CHNL_3].frame[0].timestamp;
//			frequency[FRQ_CHNL_3].frame[1].event_copy=frequency[FRQ_CHNL_3].frame[1].event_counter;	
//
//			frequency[FRQ_CHNL_3].frame[0].event_counter=0;
		}
		break;

		case FRAME_TIME_3:
		{
			frequency[FRQ_CHNL_1].frame[2].time_copy=frequency[FRQ_CHNL_1].frame[2].timestamp-frequency[FRQ_CHNL_1].frame[1].timestamp;
			frequency[FRQ_CHNL_1].frame[2].event_copy=frequency[FRQ_CHNL_1].frame[2].event_counter;	

			frequency[FRQ_CHNL_1].frame[1].event_counter=0;
			freq_ready=1;
			//----------------------------------------------------
//			frequency[FRQ_CHNL_2].frame[2].time_copy=frequency[FRQ_CHNL_2].frame[2].timestamp-frequency[FRQ_CHNL_2].frame[1].timestamp;
//			frequency[FRQ_CHNL_2].frame[2].event_copy=frequency[FRQ_CHNL_2].frame[2].event_counter;	
//
//			frequency[FRQ_CHNL_2].frame[1].event_counter=0;
//			//---------------------------------------------------
//			frequency[FRQ_CHNL_3].frame[2].time_copy=frequency[FRQ_CHNL_3].frame[2].timestamp-frequency[FRQ_CHNL_3].frame[1].timestamp;
//			frequency[FRQ_CHNL_3].frame[2].event_copy=frequency[FRQ_CHNL_3].frame[2].event_counter;	
//
//			frequency[FRQ_CHNL_3].frame[1].event_counter=0;
		}
		break;

		case FRAME_TIME_4:
		{
			frequency[FRQ_CHNL_1].frame[3].time_copy=frequency[FRQ_CHNL_1].frame[3].timestamp-frequency[FRQ_CHNL_1].frame[2].timestamp;
			frequency[FRQ_CHNL_1].frame[3].event_copy=frequency[FRQ_CHNL_1].frame[3].event_counter;	

			frequency[FRQ_CHNL_1].frame[2].event_counter=0;	
			freq_ready=1;
			//-----------------------------------------------------
//			frequency[FRQ_CHNL_2].frame[3].time_copy=frequency[FRQ_CHNL_2].frame[3].timestamp-frequency[FRQ_CHNL_2].frame[2].timestamp;
//			frequency[FRQ_CHNL_2].frame[3].event_copy=frequency[FRQ_CHNL_2].frame[3].event_counter;	
//
//			frequency[FRQ_CHNL_2].frame[2].event_counter=0;
//			//----------------------------------------------------
//			frequency[FRQ_CHNL_3].frame[3].time_copy=frequency[FRQ_CHNL_3].frame[3].timestamp-frequency[FRQ_CHNL_3].frame[2].timestamp;
//			frequency[FRQ_CHNL_3].frame[3].event_copy=frequency[FRQ_CHNL_3].frame[3].event_counter;	
//
//			frequency[FRQ_CHNL_3].frame[2].event_counter=0;
//			
//			frequency[FRQ_CHNL_1].time_counter=0;
		}
		break;
	}

	frequency[FRQ_CHNL_1].time_counter++;
	frequency[FRQ_CHNL_1].time_counter&=FRAME_TIME_4;

	return;
}
//------------------------------------------------------------
unsigned char Frequency_Measure_Process(void)//����������� ������� ��������� �������
{	
   if(freq_ready==0)
   {
   		return 0;
   }
	
	temp_freq=(((unsigned long)frequency[FRQ_CHNL_1].frame[0].event_copy+(unsigned long)frequency[FRQ_CHNL_1].frame[1].event_copy+(unsigned long)frequency[FRQ_CHNL_1].frame[2].event_copy+(unsigned long)frequency[FRQ_CHNL_1].frame[3].event_copy)<<20/*18*/)/((frequency[FRQ_CHNL_1].frame[0].time_copy+frequency[FRQ_CHNL_1].frame[1].time_copy+frequency[FRQ_CHNL_1].frame[2].time_copy+frequency[FRQ_CHNL_1].frame[3].time_copy));

	if(temp_freq>=0xFFFF)
	{
		temp_freq=0xFFFF;
	}

	DAC_Set_Value((temp_freq>>4));

    freq_ready=0;
	return 0;
}
//------------------------------------------------------------
//void Timer1_ISR(void) interrupt 3 //using 3//���������� ���������� �������� ������� �������
//{
//	//static unsigned int dac_val;
//	//dac_val++;
//	//Frequency_Measure_Process();
//
////	dac_val++;	
//
//	TF1 = 0;
//	TH1=TH1_VAL;
//	TL1=TL1_VAL;
//}