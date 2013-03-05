#include "dac.h"
#define DAC_STAND	0x09
#define DAC_SYNC	0x0D
//-------------------------------------------------------------------------------
static float val_current;//хранит значение тока
//-------------------------------------------------------------------------------
void DAC_Init(void)//инициализация ЦАП
{
	DACCON=DAC_STAND;
	DAC0H=0;
	DAC0L=0;
	DACCON=DAC_SYNC;
	return;
}
//-------------------------------------------------------------------------------
void DAC_Set_Value(unsigned int val) //установить напряжение на ЦАП
{
	DACCON=DAC_STAND;
	DAC0H=(val>>8)&0xF;
	DAC0L=val&0xFF;
	DACCON=DAC_SYNC;
    return;
}
//------------------------------------------------------------------------------
/*void DAC_Set_Current(float current)	//установка тока
{
 	 val_current=current; 
	 DAC_Set_Voltage(current*RESISTOR/1000);
} */
//-------------------------------------------------------------------------------
/*void Store_DAC_Settings(float voltage)//сохранение напряжения ЦАП в ППЗУ
{ 	
	EEPROM_Write(&voltage,1,DAC_SETTINGS_ADDR);
	return;
}
//-------------------------------------------------------------------------------
float Restore_DAC_Settings(void) //восстановить значение напряжения на ЦАП из ППЗУ
{
	float dac_voltage=0;
	EEPROM_Read(&dac_voltage,1,DAC_SETTINGS_ADDR);
	return dac_voltage;	
}
//-------------------------------------------------------------------------------
unsigned long DAC_Get_Settings(void)//получить настройки цап
{
	union
	{
		float float_val;
		unsigned long long_val;
	}float_to_long;

	float_to_long.float_val=val_current;
	return float_to_long.long_val;	
}*/	
//-------------------------------------------------------------------------------