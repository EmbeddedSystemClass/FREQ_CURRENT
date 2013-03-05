#include "ulongsort.h"

volatile unsigned char sort_auto=WAIT_STATE;//счетчик автомата сортировки(кооперативная многозадачность)
volatile unsigned long mas_copy[ADC_BUF_SIZE];
volatile unsigned char xdata mid_cycle_counter=0, sort_cycle_counter=0;
volatile unsigned long  temp;
volatile unsigned char cycle_count=0;
volatile unsigned char contin=1;

 sbit LED=P2^6;
//sbit LED2=P3^6;
//------------------------------------------------
void Get_Mid(void)//запуск на новый цикл нахождения медианы
{
 	if(sort_auto==WAIT_STATE) //если процесс свободен
	{
		sort_auto=FIND_NEW_MEASURE;
		contin=0;//
		cycle_count=1;
	}
//	LED1=~LED1;
 	//setTimer(ADC_MEAN_TASK, 0, 1);
	return;
}
//------------------------------------------------
void ulongsort_process(void)//сортирует массив и выдает медианное значение для усреднения
{

switch(sort_auto)
{
	case FIND_NEW_MEASURE: //поиск нового измерения
	{
	
		unsigned char counter=0;
	//LED1=1;
		if(!adc_channels[mid_cycle_counter].new_measuring)
		{
			counter=0;
			while(!adc_channels[mid_cycle_counter].new_measuring && counter!=ADC_CHANNELS_NUM)
			{
				mid_cycle_counter= (mid_cycle_counter+1)&0x7;
				counter++;
			}
			
			if(counter==ADC_CHANNELS_NUM)
			{
			    sort_auto=WAIT_STATE;
			}
			else
			{
				sort_auto=COPY_STATE;
			}		
		}
		else 
		{
			sort_auto=COPY_STATE;
		}
		 	
	}
	break;

	case COPY_STATE:
	{
	//	LED2=1;
		
		memcpy(mas_copy,adc_channels[mid_cycle_counter].ADC_BUF_UN,ADC_BUF_SIZE*sizeof(unsigned long)); 
		sort_auto=SORT_STATE;//переходим к сортировке
		cycle_count=1;//???
		contin=0;
	
		//LED2=0;	
	}
	break;

	case SORT_STATE:   //сортировка методом пузырька
	{
	//	LED1=1;
			
			if(cycle_count<ADC_BUF_SIZE)
			{	
				if(mas_copy[cycle_count-1]>mas_copy[cycle_count])
				{
				//	LED=1;	
					temp=mas_copy[cycle_count-1];
					mas_copy[cycle_count-1]=mas_copy[cycle_count];
					mas_copy[cycle_count]=temp;
					contin=1;
				//	LED=0;
				}
				cycle_count++;
			}
			else 
			{
				if(!contin)
				{
					sort_auto=MID_STATE;//переходим к нахождению среднего
				}
				contin=0;
				cycle_count=1;
			}  		
	}
	break;

	case MID_STATE:
	{
		channels[mid_cycle_counter].channel_data=(mas_copy[(ADC_BUF_SIZE>>1)-1]+mas_copy[ADC_BUF_SIZE>>1])>>1;

//		LED=1;
		if(channels[mid_cycle_counter].calibrate.cal.calibrate)
		{
 			channels[mid_cycle_counter].channel_data_calibrate=GetCalibrateVal(mid_cycle_counter,channels[mid_cycle_counter].channel_data);	//калиброванная величина
  		}
//		LED=0;
		adc_channels[mid_cycle_counter].new_measuring=0;

	  	mid_cycle_counter= (mid_cycle_counter+1)&0x7;
		sort_auto=WAIT_STATE;	
	}
	break;

	case WAIT_STATE:
	{
		//пусто
	}
	break;

	default:
	{
		contin=0;//
		cycle_count=1;
		sort_auto=WAIT_STATE;
	//	LED1=0;
	}
	break;
}
return;
}
//------------------------------------------------