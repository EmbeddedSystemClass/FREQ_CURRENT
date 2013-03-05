#include "dac.h"
//-------------------------------------------------------------------------------
void DAC_Init(void)//инициализация ЦАП
{
//	float current=0;
	DACCON=0x3;//dac enable  Vref (AINCOM)
//	current= Restore_DAC_Settings();
//	if(current<=0 || current>=DAC_MAX_CURRENT)
//	{
//		DAC_Set_Current(0);//восстанвление настроек из ППЗУ
//	}
//	else
//	{
//		DAC_Set_Current(current);//восстанвление настроек из ППЗУ
//	}
	return;
}
//-------------------------------------------------------------------------------
void DAC_Start(void)
{
}
//-------------------------------------------------------------------------------
void DAC_Set_Value(unsigned int val) //?????????? ?????????? ?? ???
{
	DACH=(val>>8)&0xF;
	DACL=val&0xFF;
	return;
}
////------------------------------------------------------------------------------
//void DAC_Set_Current(float current)	//установка тока
//{
// 	 DAC_Set_Voltage(current*RESISTOR/1000);
//}
////-------------------------------------------------------------------------------
//void Store_DAC_Settings(float voltage)//сохранение напряжения ЦАП в ППЗУ
//{ 	
//	EEPROM_Write(&voltage,1,DAC_SETTINGS_ADDR);
//	return;
//}
////-------------------------------------------------------------------------------
//float Restore_DAC_Settings(void) //восстановить значение напряжения на ЦАП из ППЗУ
//{
//	float dac_voltage=0;
//	EEPROM_Read(&dac_voltage,1,DAC_SETTINGS_ADDR);
//	return dac_voltage;	
//}