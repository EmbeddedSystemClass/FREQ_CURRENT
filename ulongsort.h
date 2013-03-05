#ifndef ULONGSORT_H
#define ULONGSORT_H
#include "adc.h"
#include "proto_uso/channels.h"
#include "calibrate/calibrate.h"
#include <string.h>
#include <math.h>

#define COPY_STATE	0x1//��������� ����������� �������
#define SORT_STATE	0x2//��������� ���������� �������
#define MID_STATE	0x3//������� �������� �������� 
#define WAIT_STATE	0x4//��������� ��������
#define FIND_NEW_MEASURE	0x5//������ ������ � ������ �����������
void ulongsort_process(void);//��������� ������ � ������ ��������� �������� ��� ����������
void Get_Mid(void);//������ �� ����� ���� ���������� �������

extern struct ADC_Channels xdata adc_channels[];

#endif