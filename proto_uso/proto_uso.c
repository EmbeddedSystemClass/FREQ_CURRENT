#include "proto_uso.h"
//#include "calibrate\calibrate.h"
#include "eeprom\eeprom.h"
#include <intrins.h>
#include "frequency.h"
//-----------------------------------------------------------------------------------
  unsigned char code Crc8Table[256]={0x00, 0xBC, 0x01, 0xBD, 0x02, 0xBE, 0x03, 0xBF, 
 									 0x04, 0xB8, 0x05, 0xB9, 0x06, 0xBA, 0x07, 0xBB, 
									 0x08, 0xB4, 0x09, 0xB5, 0x0A, 0xB6, 0x0B, 0xB7, 
									 0x0C, 0xB0, 0x0D, 0xB1, 0x0E, 0xB2, 0x0F, 0xB3, 
									 0x10, 0xAC, 0x11, 0xAD, 0x12, 0xAE, 0x13, 0xAF, 
									 0x14, 0xA8, 0x15, 0xA9, 0x16, 0xAA, 0x17, 0xAB, 
									 0x18, 0xA4, 0x19, 0xA5, 0x1A, 0xA6, 0x1B, 0xA7, 
									 0x1C, 0xA0, 0x1D, 0xA1, 0x1E, 0xA2, 0x1F, 0xA3, 
									 0x20, 0x9C, 0x21, 0x9D, 0x22, 0x9E, 0x23, 0x9F, 
									 0x24, 0x98, 0x25, 0x99, 0x26, 0x9A, 0x27, 0x9B, 
									 0x28, 0x94, 0x29, 0x95, 0x2A, 0x96, 0x2B, 0x97, 
									 0x2C, 0x90, 0x2D, 0x91, 0x2E, 0x92, 0x2F, 0x93, 
									 0x30, 0x8C, 0x31, 0x8D, 0x32, 0x8E, 0x33, 0x8F, 
									 0x34, 0x88, 0x35, 0x89, 0x36, 0x8A, 0x37, 0x8B, 
									 0x38, 0x84, 0x39, 0x85, 0x3A, 0x86, 0x3B, 0x87, 
									 0x3C, 0x80, 0x3D, 0x81, 0x3E, 0x82, 0x3F, 0x83, 
									 0x40, 0xFC, 0x41, 0xFD, 0x42, 0xFE, 0x43, 0xFF, 
									 0x44, 0xF8, 0x45, 0xF9, 0x46, 0xFA, 0x47, 0xFB, 
									 0x48, 0xF4, 0x49, 0xF5, 0x4A, 0xF6, 0x4B, 0xF7, 
									 0x4C, 0xF0, 0x4D, 0xF1, 0x4E, 0xF2, 0x4F, 0xF3, 
									 0x50, 0xEC, 0x51, 0xED, 0x52, 0xEE, 0x53, 0xEF, 
									 0x54, 0xE8, 0x55, 0xE9, 0x56, 0xEA, 0x57, 0xEB, 
									 0x58, 0xE4, 0x59, 0xE5, 0x5A, 0xE6, 0x5B, 0xE7, 
									 0x5C, 0xE0, 0x5D, 0xE1, 0x5E, 0xE2, 0x5F, 0xE3, 
									 0x60, 0xDC, 0x61, 0xDD, 0x62, 0xDE, 0x63, 0xDF, 
									 0x64, 0xD8, 0x65, 0xD9, 0x66, 0xDA, 0x67, 0xDB, 
									 0x68, 0xD4, 0x69, 0xD5, 0x6A, 0xD6, 0x6B, 0xD7, 
									 0x6C, 0xD0, 0x6D, 0xD1, 0x6E, 0xD2, 0x6F, 0xD3, 
									 0x70, 0xCC, 0x71, 0xCD, 0x72, 0xCE, 0x73, 0xCF, 
									 0x74, 0xC8, 0x75, 0xC9, 0x76, 0xCA, 0x77, 0xCB, 
									 0x78, 0xC4, 0x79, 0xC5, 0x7A, 0xC6, 0x7B, 0xC7, 
									 0x7C, 0xC0, 0x7D, 0xC1, 0x7E, 0xC2, 0x7F, 0xC3};
sbit DE_RE=P3^5;
// sbit LED=P2^6;
//-----------------------------------------------------------------------------------
//volatile unsigned char xdata DEV_NAME[DEVICE_NAME_LENGTH_SYM] ="<<uUSO_2>>"; //имя устройства
//volatile unsigned char xdata NOTICE[DEVICE_DESC_MAX_LENGTH_SYM]="<-- GEOSPHERA_2012 -->";//примечание 	
//volatile unsigned char xdata VERSION[DEVICE_VER_LENGTH_SYM] ="\x30\x30\x30\x30\x31";	// версия программы ПЗУ	не больше 5 байт

volatile unsigned char xdata ADRESS_DEV=0x1;

volatile unsigned char xdata dev_desc_len=20;//длина описания устройства
//--------------------------------global variable------------------------------------
volatile unsigned char xdata	PROTO_STATE;//счетчик состояний
volatile unsigned char xdata    recieve_count;//счетчик приемного буфера
volatile unsigned char xdata	transf_count;//счетчик передаваемых байтов	   
volatile unsigned char xdata	buf_len;//длина передаваемого буфера

//------------------------флаги ошибок--------------------------------
volatile unsigned char xdata CRC_ERR;	//ошибка сrc
volatile unsigned char xdata COMMAND_ERR;//неподдерживаемая команда

volatile unsigned char xdata TIMEOUT;//таймаут 

volatile unsigned char idata  CUT_OUT_NULL;//флаг-вырезаем 0 после 0xD7
volatile unsigned char xdata frame_len=0;//длина кадра, которую вытаскиваем из шестого байта кадра
//--------------------------------------------------------------------
volatile unsigned char xdata  RecieveBuf[MAX_LENGTH_REC_BUF]={0} ; //буфер принимаемых данных
volatile unsigned char xdata 			*TransferBuf;
//static unsigned char /*data*/ volatile  TransferBuf[MAX_LENGTH_TR_BUF] ; //буфер передаваемых данных
//--------------------------------------------------------------------
volatile unsigned char xdata  STATE_BYTE=0xC0;//байт состояния устройства
volatile unsigned char idata symbol=0xFF;//принятый символ
//-----------------------------------------------------------------------------------
union //объединение для конвертирования char->long
{
	float result_float;
	unsigned char result_char[4];
}
sym_8_to_float;
//-----------------------------------------------------------------------------------

void UART_ISR(void) interrupt 4 //using 1
{	
	EA=0;	//запрет прерывания
	
	if(RI)
	{
		RI=0; 
//----------------------обрабатываем возможные ошибки длины кадра-------------
		if(recieve_count>MAX_LENGTH_REC_BUF)	//если посылка слишком длинная
		{
			PROTO_STATE=PROTO_RESTART;//делаем рестарт протокола
			return;
		} 


		symbol=SBUF;
		switch(symbol)
		{
			case (char)(0xD7):
			{
				RecieveBuf[recieve_count]=symbol;
				recieve_count++;
				CUT_OUT_NULL=1;		 
			}
			break;

			case (char)(0x29):
			{
				if(CUT_OUT_NULL==1)
				{
					RecieveBuf[0]=0x0;
					RecieveBuf[1]=0xD7;
					RecieveBuf[2]=0x29;
					recieve_count=0x3;		 	
				}
				else
				{
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;	
				}
				CUT_OUT_NULL=0;
			}
			break;

			case (char)(0x0):
			{
 				if(CUT_OUT_NULL==1)	  //если после 0xD7-пропускаем
				{
					CUT_OUT_NULL=0;		
				}
				else
				{
					RecieveBuf[recieve_count]=symbol;
					recieve_count++;	
				}
			}
			break;

			default :
			{
				RecieveBuf[recieve_count]=symbol;
				recieve_count++;
				CUT_OUT_NULL=0;
				
			}
		}

	   if(recieve_count>6)
	   {
	   		  if(recieve_count==6+frame_len)	  // принимаем указанное в frame_len число байт данные, 6 значит что обмен идет с компом, надо ставаить 5 чтобы обмениваться с устройствами
   			  {
					PROTO_STATE=PROTO_ADDR_CHECK;
			  		ES=0;
			  		REN=0;  //recieve disable -запрещаем принимать в буфер	
				   	CUT_OUT_NULL=0;  			  			
			  }	  
	   }
	   else
	   {
			   if(recieve_count==6)
			   {     
		        	frame_len=RecieveBuf[recieve_count-1];  // получаем длину данных после заголовка					 
			   }	   		
	   }
//--------------------------начало кадра...проверка до длины кадра--------

	}
//----------------------------передача----------------------------------------------------------------
	if(TI)
	{
		TI=0;
		 
		if(transf_count<buf_len)
		{
			if(transf_count<3)//передаем заголовок
			{
				SBUF=TransferBuf[transf_count];			
				transf_count++;
			}
			else   //тело...   подставляем 0 после 0xD7
			{
					if(CUT_OUT_NULL==0)
					{
						if(TransferBuf[transf_count]==(unsigned char)0xD7)//проверим, это  ,0xD7 или другое
						{			
							CUT_OUT_NULL=0x1;	
						}
						SBUF=TransferBuf[transf_count];			
						transf_count++;
					}
					else
					{
						SBUF=(unsigned char)0x0;
						CUT_OUT_NULL=0;		
					}	
			}	
		}
		else
		{
			transf_count=0;		//обнуляем счетчик
			PROTO_STATE=PROTO_RESTART;
			CUT_OUT_NULL=0;
			DE_RE=0;//линия на прием
			
		}					   
	}			
	EA=1;
	return;
}
//------------------------------------------------------------------------------
void Protocol_Init(void) //using 0
{
	TI=0;
	RI=0;
	//------------------------флаги ошибок--------------------------------
	
	CRC_ERR=0x0;	//ошибка crc
	COMMAND_ERR=0x0;//неподдерживаемая команда
	
	TransferBuf=&RecieveBuf[0];	 //буфер ответа =буфер запроса

//	Restore_Dev_Address_Desc();

	PROTO_STATE=PROTO_RESTART;//счетчик состояний
	recieve_count=0x0;//счетчик буфера приема
	transf_count=0x0;//счетчик передаваемых байтов
	buf_len=0x0;//длина передаваемого буфера
	DE_RE=0;//линия на прием
	CUT_OUT_NULL=0;
	return;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
unsigned char  Channel_Set_Parameters(void) //using 0 //Установить параметры по каналам, согласно абсолютной нумерации;
{
	volatile unsigned char data cur_4_20,up_down,value;
	cur_4_20=RecieveBuf[6]&0x80;//последний бит-(0-4 или 1-20 мА)
	up_down=RecieveBuf[6]&0x40;//7 бит-0 увеличить 1-уменьшить
	value= RecieveBuf[6]&0x3F; //на какое значение увеличить/уменьшить

	if(cur_4_20==0)
	{
		if(up_down==0)
		{
			I04_mA+=value;
			if(I04_mA>=0xFF0)
			{
				I04_mA=0xFF0;
			}
		}
		else
		{
			I04_mA-=value;
			if(I04_mA<=0xA)
			{
				I04_mA=0xA;
			}
		}
	}
	else
	{
		if(up_down==0)
		{
			I20_mA+=value;
			if(I20_mA>=0xFFF)
			{
				I20_mA=0xFFF;
			}
		}
		else
		{
			I20_mA-=value;
			if(I20_mA<=I04_mA)
			{
				I20_mA=I04_mA+1;
			}
		}
	}
	  
	   return Request_Error(FR_SUCCESFUL);
}
////-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
unsigned char Channel_Set_Calibrate(void)//установить верхнюю или нижнюю точку калибровки
{

	unsigned long data temp;
	FREQ_MAX=RecieveBuf[6];
	FREQ_FRAME=RecieveBuf[7];
	if(FREQ_FRAME>FREQ_FRAME_MAX)
	{
		FREQ_FRAME=FREQ_FRAME_MAX;
	}
	temp=I04_mA;
	EEPROM_Write(&temp,0x1,I04_mA_ADDR);
	temp=I20_mA;
	EEPROM_Write(&temp,0x1,I20_mA_ADDR);
	temp=FREQ_MAX;
	EEPROM_Write(&temp,0x1,FREQ_MAX_ADDR);
	temp=FREQ_FRAME;
	EEPROM_Write(&temp,0x1,FREQ_FRAME_ADDR);


	return	Request_Error(FR_SUCCESFUL);//ошибки нет, подтверждение	
}
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
unsigned char Request_Error(unsigned char error_code) //using 0 //	Ошибочный запрос/ответ;
{
	TransferBuf[0]=0x00;TransferBuf[1]=0xD7;TransferBuf[2]=0x29;
    TransferBuf[3]=ADRESS_DEV;  // адрес узла
    TransferBuf[7]=RecieveBuf[4]; // код сбойной команды
    TransferBuf[4]=0xFF;  // код операции

	TransferBuf[6]=STATE_BYTE; // байт статуса узла
    TransferBuf[8]=error_code;	 
    TransferBuf[5]=0x04;	  // длина данных
    TransferBuf[9]=CRC_Check(TransferBuf,9);
	return 10;
}
//-----------------------------------------------------------------------------
void ProtoBufHandling(void) //using 0 //процесс обработки принятого запроса
{
  switch(RecieveBuf[4])
  {
////---------------------------------------

	//-----------------------------------
	case CHANNEL_SET_PARAMETERS_REQ:
	{
		buf_len=Channel_Set_Parameters();
	}
	break;
//	//-----------------------------------

//------------------------------------------
	case CHANNEL_SET_CALIBRATE:
	{
		 buf_len=Channel_Set_Calibrate();
	}
	break;
//------------------------------------------
    default:
	{
       COMMAND_ERR=0x1;//несуществующая команда
	   buf_len=Request_Error(FR_COMMAND_NOT_EXIST);
	   PROTO_STATE=PROTO_ERR_HANDLING;//на обработчик ошибки
    }								   
  }

  return;
}
//-----------------------------------------------------------------------------------
void ProtoProcess(void) //главный процесс 
{
	switch(PROTO_STATE)
	{
//------------------------------------------
		case(PROTO_RESTART):
		{
			recieve_count=0x0;//??
		//	transf_count=0x0;
			//TIC_Stop();	 //остановить секундный таймаут


			PROTO_STATE=PROTO_WAIT;//переходим в состояние ожидания 

			REN=1;//recieve enqble
			DE_RE=0;//линия на прием
			ES=1;	//прерывание uart	
		}
		break;
//------------------------------------------
		case(PROTO_WAIT)://ожидание приема
		{		
			//пусто
			_nop_();
		}
		break;
//------------------------------------------
		case(PROTO_ADDR_CHECK):// проверка адреса	   Устарело
		{						
				if(RecieveBuf[3]==ADRESS_DEV)//если адрес совпал	  
				{
					PROTO_STATE=PROTO_CRC_CHECK;	//проверяем crc			
				}
				else
				{
					PROTO_STATE=PROTO_RESTART;		//иначе ждем новой посылки
				}						
		}
		break; 
//------------------------------------------
		case(PROTO_CRC_CHECK):// проверка CRC
		{		 	
			  unsigned char xdata CRC=0x0;
					
			CRC=RecieveBuf[recieve_count-1];
					
			if(CRC_Check(&RecieveBuf,(recieve_count-CRC_LEN))==CRC)
			{
				PROTO_STATE=PROTO_BUF_HANDLING;	 
			}
			else
			{		
				CRC_ERR=0x1;//несовпадение CRC
	   			PROTO_STATE=PROTO_ERR_HANDLING;//на обработчик ошибки								
			} 	
		}
		break;
//------------------------------------------
		case(PROTO_BUF_HANDLING):// обработка принятого буфера
		{			
				ProtoBufHandling();//процедура обработки сообщения	
				if(buf_len)//если в буфере что то есть
					PROTO_STATE=PROTO_BUF_TRANSFER;//после обработки передаем сообщение	
				else
					PROTO_STATE=PROTO_RESTART; //иначе рестарт протокола		
		}
		break;
//------------------------------------------
		case(PROTO_BUF_TRANSFER):// передача буфера
		{
				DE_RE=1; //переключаем RS485 в режим передачи
								
				REN=0;	//запрет приема-только передача
				transf_count=0;
				CUT_OUT_NULL=0;
				SBUF=TransferBuf[transf_count];//передача байта, остальным займется автомат
				transf_count++;//инкрементируем счетчик переданных
				PROTO_STATE=PROTO_WAIT;//забросили данные в автомат и ждем
				
				ES=1; //включим прерывание уарт	
		}
		break;
//------------------------------------------
		case(PROTO_ERR_HANDLING):// обработка ошибок // можно не реализовывать
		{
				//---обработка ошибок----
				if(CRC_ERR)
				{
					//написать обработку ошибок

					CRC_ERR=0x0;	//ошибка lrc
				}

				if(COMMAND_ERR)
				{

					COMMAND_ERR=0x0;//неподдерживаемая команда	
				}
				//-----------------------
					
				PROTO_STATE=PROTO_RESTART;		//иначе рестарт и ждем новой посылки			
		}
		break;
//------------------------------------------
		default:
		{
			PROTO_STATE=PROTO_RESTART;
		}
		break;
	}

	return;
}
//-----------------------CRC------------------------------------------------------------
  unsigned char CRC_Check( unsigned char xdata *Spool_pr,unsigned char Count_pr ) 
 {

     unsigned char crc = 0x0;

     while (Count_pr--)
         crc = Crc8Table[crc ^ *Spool_pr++];

     return crc;

 }
////-----------------------------------------------------------------------------------------------
