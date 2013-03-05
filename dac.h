#ifndef DAC_H
#define DAC_H
#include <ADuC841.h>
#include "preferences.h"
//#include "eeprom/eeprom.h"
//------------------------------------------------------------
#define REF_VOLTAGE 2.5
//#define DAC_MAX_CURRENT 15 //максимальный ток на уровне 15 мА
//------------------------------------------------------------
void DAC_Init(void);//инициализация ЦАП
void DAC_Set_Value(unsigned int); //установить напряжение на ЦАП

//------------------------------------------------------------


#endif