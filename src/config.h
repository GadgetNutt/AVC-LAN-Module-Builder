/*
  config.h - AVCLan Head Unit library for for Atmega328
  Created by Greg Nutt 2020-12-04
  Version 0.0.1

  Not for commercial use.
*/

#include "avclan-registers.h"
#include <stdint.h>

#ifndef config_h
#define config_h

/* Define MCU */
#define AVR
//#define STM32 - NOT IMPLEMENTED

/* Define devices */
/* ONLY define one of the following
* 
*	AVC_MASTER
* 	AVC_DEVICE
* 	AVC_SNIFFER	 
*   
*   Enabling AVC_MASTER will automatically also enable AVC_DEVICE
*   Enabling AVC_SNIFFER will automatically disable AVC_MASTER and AVC_DEVICE
*/
//#define AVC_MASTER
#define AVC_DEVICE
//#define AVC_SNIFFER
 
/* Define Crystal
* 1	-	8 MHz
* 2 -	14.7456 MHz
* 3 -	16 MHz
*/
#define CRYSTAL		2


#ifdef AVC_MASTER
#define AVC_DEVICE		// AVC_DEVICE MUST BE DEFINED IF AVC_MASTER IS DEFINED
#endif
#ifdef AVC_SNIFFER
#undef AVC_MASTER
#undef AVC_DEVICE
#endif



/* Define Connections*/
//#define AVC_I2C

const uint16_t  ADDR_ME = ADDR_DISPLAY_MASTER;	//	See avclan-registers.h for list

#define MAXSENDATTEMPT	3

// define out pin (pin 6 arduino) for AVCLAN_RESISTOR
#define DATAOUT_DDR		DDRD
#define DATAOUT_PORT	PORTD
#define	DATAOUT_PIN		PIND
#define DATAOUT			6

// define in pin (pin 7 arduino) for AVCLAN_RESISTOR
#define DATAIN_DDR		DDRD
#define DATAIN_PORT		PORTD
#define	DATAIN_PIN		PIND
#define DATAIN			7

// TX LED connected to Pin1  of  PORTC
#define TX_LED_DDR		DDRC
#define TX_LED_PORT		PORTC
#define	TX_LED_PIN		PINC
#define TX_LED_OUT		2

// RX LED connected to Pin4  of  PORTC
#define RX_LED_DDR		DDRC
#define RX_LED_PORT		PORTC
#define	RX_LED_PIN		PINC
#define RX_LED_OUT		4

// AZFM board activate
#define AZFM_INIT
#define AZFM_ON  
#define AZFM_OFF 


/* Define Crystal
* 1	-	8 MHz
* 2 -	14.7456 MHz
* 3 -	16 MHz
* 
* #define AVC_NORMAL_BIT_LENGTH           	Frequency * 37 * (F_CPU / 1000000L / 8) 
* #define AVC_BIT_1_HOLD_ON_LENGTH			Frequency * 20 uS * (F_CPU / 1000000L / 8)
* #define AVC_BIT_0_HOLD_ON_LENGTH			Frequency * 32 uS * (F_CPU / 1000000L / 8)
* #define AVC_BIT_0_HOLD_ON_MIN_LENGTH		Frequency * 30 uS * (F_CPU / 1000000L / 8)    Compare half way between a '1' (20 us) and a '0' (32 us ): 32 - (32 - 20) /2 = 26 us
* #define AVC_START_BIT_LENGTH				Frequency * 186 uS  * (F_CPU / 1000000L / 32) ,  prescaler 32
* #define AVC_START_BIT_HOLD_ON_LENGTH		Frequency * 168 uS * (F_CPU / 1000000L / 32)    prescaler 32
* #define AVC_START_BIT_HOLD_ON_MIN_LENGTH	Frequency * 44 uS * (F_CPU / 1000000L / 32)      greater that AVC_NORMAL_BIT_LENGTH,  prescaler 32
* #define AVC_1U_LENGTH	                    Frequency * 1 uS * (F_CPU / 1000000L / 8)
* 
* The new values for above will need to be rounded up or down.  You may need to try both values.
* 
*/

#if CRYSTAL == 1
#define AVC_NORMAL_BIT_LENGTH           	0x25  // 37 * (F_CPU / 1000000L / 8) 
#define AVC_BIT_1_HOLD_ON_LENGTH			0x14  // 20 uS * (F_CPU / 1000000L / 8) 
#define AVC_BIT_0_HOLD_ON_LENGTH			0x20 // 32 uS * (F_CPU / 1000000L / 8)
#define AVC_BIT_0_HOLD_ON_MIN_LENGTH		0x16  // 30 uS * (F_CPU / 1000000L / 8)    Compare half way between a '1' (20 us) and a '0' (32 us ): 32 - (32 - 20) /2 = 26 us
#define AVC_START_BIT_LENGTH				0x2F  // 186 uS  * (F_CPU / 1000000L / 32) ,  prescaler 32
#define AVC_START_BIT_HOLD_ON_LENGTH		0x2A  // 168 uS * (F_CPU / 1000000L / 32)    prescaler 32
#define AVC_START_BIT_HOLD_ON_MIN_LENGTH	0x0B  // 44 uS * (F_CPU / 1000000L / 32)      greater that AVC_NORMAL_BIT_LENGTH,  prescaler 32
#define AVC_1U_LENGTH	                    0x01  // 1 uS * (F_CPU / 1000000L / 8)      
#endif
#if CRYSTAL == 2
#define AVC_NORMAL_BIT_LENGTH           	0x44  // 37 * (F_CPU / 1000000L / 8) 
#define AVC_BIT_1_HOLD_ON_LENGTH			0x25  // 20 uS * (F_CPU / 1000000L / 8) 
#define AVC_BIT_0_HOLD_ON_LENGTH			0x3B // 32 uS * (F_CPU / 1000000L / 8)
#define AVC_BIT_0_HOLD_ON_MIN_LENGTH		0x2D  // 30 uS * (F_CPU / 1000000L / 8)    Compare half way between a '1' (20 us) and a '0' (32 us ): 32 - (32 - 20) /2 = 26 us
#define AVC_START_BIT_LENGTH				0x55  // 186 uS  * (F_CPU / 1000000L / 32) ,  prescaler 32
#define AVC_START_BIT_HOLD_ON_LENGTH		0x4D  // 168 uS * (F_CPU / 1000000L / 32)    prescaler 32
#define AVC_START_BIT_HOLD_ON_MIN_LENGTH	0x13  // 44 uS * (F_CPU / 1000000L / 32)      greater that AVC_NORMAL_BIT_LENGTH,  prescaler 32
#define AVC_1U_LENGTH	                    0x02  // 1 uS * (F_CPU / 1000000L / 8)      
#endif
#if CRYSTAL == 3
#define AVC_NORMAL_BIT_LENGTH           	0x4A  // 37 * (F_CPU / 1000000L / 8) 
#define AVC_BIT_1_HOLD_ON_LENGTH			0x28  // 20 uS * (F_CPU / 1000000L / 8) 
#define AVC_BIT_0_HOLD_ON_LENGTH			0x40 // 32 uS * (F_CPU / 1000000L / 8)
#define AVC_BIT_0_HOLD_ON_MIN_LENGTH		0x32  // 30 uS * (F_CPU / 1000000L / 8)    Compare half way between a '1' (20 us) and a '0' (32 us ): 32 - (32 - 20) /2 = 26 us
#define AVC_START_BIT_LENGTH				0x5D  // 186 uS  * (F_CPU / 1000000L / 32) ,  prescaler 32
#define AVC_START_BIT_HOLD_ON_LENGTH		0x54  // 168 uS * (F_CPU / 1000000L / 32)    prescaler 32
#define AVC_START_BIT_HOLD_ON_MIN_LENGTH	0x16  // 44 uS * (F_CPU / 1000000L / 32)      greater that AVC_NORMAL_BIT_LENGTH,  prescaler 32
#define AVC_1U_LENGTH	                    0x02  // 1 uS * (F_CPU / 1000000L / 8)      
#endif

#define AVC_MAXMSGLEN		32
#define AVC_CONTROL_FLAGS	0xF



#endif

