#ifndef PREFERENCES_H
#define	PREFERENCES_H

//#pragma NOAREGS

//#define F_OSC_12  12582912UL//частота ядра-12MHz
#define F_OSC_6 6291456


//#define BAUDRATE_9600
#define BAUDRATE_57600
//#define BAUDRATE_38400



//----------------------------------------------------------------------
#ifdef BAUDRATE_9600
	#ifdef F_OSC_12
	//	#define TH1_VAL 	0xEB//0xD7
	//	#define TL1_VAL		0x85//0x0A;
		#define T3CON_VAL	0x86
		#define T3FD_VAL	0x12
	#endif
	
	#ifdef F_OSC_6
		//#define TH1_VAL 	0xF5
		//#define TL1_VAL		0xC3;
		#define T3CON_VAL	0x85
		#define T3FD_VAL	0x12
	#endif
#endif

#ifdef BAUDRATE_57600 //
	#ifdef F_OSC_12
		//#define TH1_VAL 	0xFC
	//	#define TL1_VAL		0x96;
		#define T3CON_VAL	0x83
		#define T3FD_VAL	0x2D
	#endif
	
	#ifdef F_OSC_6
	//	#define TH1_VAL 	0xFE
	//	#define TL1_VAL		0x4B;
		#define T3CON_VAL	0x82
		#define T3FD_VAL	0x2D
	#endif
#endif

#ifdef 	BAUDRATE_38400 //
	#ifdef F_OSC_12
	//	#define TH1_VAL 	0xFA
	//	#define TL1_VAL		0xE3//0xE1
		#define T3CON_VAL	0x84
		#define T3FD_VAL	0x12
	#endif
#endif
//----------------------------------------------------------------------
#ifdef 	F_OSC_12
	#define PLLCON_VAL 0xF8
	
	#define TH0_VAL 	0xFF//0xCE //200Hz
	#define TL0_VAL 	0xFF//0xD9

	#define TH1_VAL 	0x15
	#define TL1_VAL		0xA0//0xE1

	#define F_OSC F_OSC_12
#endif

#ifdef F_OSC_6
	#define PLLCON_VAL 0xF9

	#define TH0_VAL 0xFF //1000Hz
	#define TL0_VAL 0xFF

	#define TH1_VAL 	0xF3//0x1A//0x8A  //частота 2048 Hz-опорная для генератора импульсов
	#define TL1_VAL		0xFF//0x0//0xD0//0xE1

	#define F_OSC F_OSC_6
#endif
//----------------------------------------------------------------------


//----------------------------------------------------------------------

#define DEVICE_ADDR_MAX	0xF //максимальный адрес устройства
//----------------------------адреса настроек в ППЗУ--------------------

#define I04_mA_ADDR		0x1
#define I20_mA_ADDR		0x2
#define FREQ_MAX_ADDR	0x3
#define	FREQ_FRAME_ADDR	0x4
//----------------------------interfaces--------------------------------
#define RS_232
//#define RS_485
//----------------------------------------------------------------------
//----------тайминги циклических процессов-----------------------------
#ifdef 	F_OSC_12
	#define GET_MID_TIM 		1	
	#define FRQ_MEAS_PROC_TIM	50
	#define SEC_TSK_TIM			200
	#define SK_TSK_TIM			200

#endif

#ifdef F_OSC_6
	#define GET_MID_TIM 		1	
	#define FRQ_MEAS_PROC_TIM	50
	#define SEC_TSK_TIM			100
	#define SK_TSK_TIM			30
#endif
//---------------------------------------------------------------------
#endif
