#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "timer0.h"
#include "timer2.h"

#include "preferences.h"
#include "watchdog.h"
#include <ADuC845.h>
//----------------------------------------------------
struct Frequency
{
	unsigned int time_counter;//������
	struct //������������� �����
	{
		unsigned int event_counter;//������� �������
		unsigned int timestamp;	   //����� �������
		unsigned int time_copy;	   //���������� ����� �� �������� �����
		unsigned int event_copy;   //���������� ������� �� �������� �����
		
	}frame[4];
};
//----------------------------------------------------
void Frequency_Init(void);//������������� ��������� �������
unsigned char Frequency_Measure_Process(void);//����������� ������� ��������� �������
unsigned char Sec_Task(void);//��������� ������ ��� ����������� ������� �������
void INT0_ISR(void);//���������� �������� ���������� 0
void INT1_ISR(void);//���������� �������� ���������� 1
void INT2_ISR(void);//���������� �������� ���������� 2-���������� ������� ���� ������� T0
void Timer2_ISR(void);//���������� ���������� �������� ������� �������
//----------------------------------------------------
//extern struct Channel xdata channels[CHANNEL_NUMBER];
//----------------------------------------------------
#endif