/*
  config.h - AVCLan Head Unit library for for Atmega328
  Created by Greg Nutt 2020-12-04
  Version 0.0.1

  Not for commercial use.
*/

#ifndef config_h
#define config_h

/* Define MCU */
#define AVR

/* Device configuration section */

/*  Only define one of the following: */
//#define AVC_MASTER
//#define AVC_DEVICE
//#define AVC_SNIFFER

#ifdef AVC_MASTER
#define AVC_DEVICE // Enabling AVC_MASTER automatically enables AVC_DEVICE
#endif
#ifdef AVC_SNIFFER // Enabling AVC_SNIFFER automatically disables AVC_MASTER and AVC_DEVICE
#undef AVC_MASTER
#undef AVC_DEVICE
#endif


#include "avclan-registers.h"
const uint16_t  ADDR_ME = 0x0178;	// See avclan-registers.h for list


/* Protocol configuration */
#define MAXSENDATTEMPT		3	// Used in avclan-drv.cpp
#define AVC_MAXMSGLEN		32	// Used in avclan-drv.cpp
#define AVC_CONTROL_FLAGS	0xF	// Used in avclan-drv.cpp
#define DEFAULT_MSG_DELAY 	15	// Used in avclan.ino

/* Define Connections*/
//#define AVC_I2C


/* Hardware configuration section */

// DATAOUT (TX-) connected to Pin 7 of PORTD (i.e. Arduino terminal D7)
#define DATAOUT			7
#define DATAOUT_DDR		DDRD
#define DATAOUT_PORT	PORTD
#define	DATAOUT_PIN		PIND

// DATAIN (TX+) connected to Pin 6 of PORTD (i.e. Arduino terminal D6)
#define DATAIN			6
#define DATAIN_DDR		DDRD
#define DATAIN_PORT		PORTD
#define	DATAIN_PIN		PIND

// TX_LED connected to Pin 2 of PORTC (i.e. Arduino terminal A2)
#define TX_LED_OUT		2
#define TX_LED_DDR		DDRC
#define TX_LED_PORT		PORTC
#define	TX_LED_PIN		PINC

// RX_LED connected to Pin 2 of PORTC (i.e. Arduino terminal A4)
#define RX_LED_OUT		4
#define RX_LED_DDR		DDRC
#define RX_LED_PORT		PORTC
#define	RX_LED_PIN		PINC


/* Define crystal frequency
* 1	-	8 MHz
* 2 -	14.7456 MHz
* 3 -	16 MHz
*/
#define CRYSTAL		3

/* Crystal timing specifications
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
*/

#if CRYSTAL == 1
#define AVC_NORMAL_BIT_LENGTH           	0x25
#define AVC_BIT_1_HOLD_ON_LENGTH			0x14
#define AVC_BIT_0_HOLD_ON_LENGTH			0x20
#define AVC_BIT_0_HOLD_ON_MIN_LENGTH		0x16
#define AVC_START_BIT_LENGTH				0x2F
#define AVC_START_BIT_HOLD_ON_LENGTH		0x2A
#define AVC_START_BIT_HOLD_ON_MIN_LENGTH	0x0B
#define AVC_1U_LENGTH	                    0x01
#endif
#if CRYSTAL == 2
#define AVC_NORMAL_BIT_LENGTH           	0x44
#define AVC_BIT_1_HOLD_ON_LENGTH			0x25
#define AVC_BIT_0_HOLD_ON_LENGTH			0x3B
#define AVC_BIT_0_HOLD_ON_MIN_LENGTH		0x2D
#define AVC_START_BIT_LENGTH				0x55
#define AVC_START_BIT_HOLD_ON_LENGTH		0x4D
#define AVC_START_BIT_HOLD_ON_MIN_LENGTH	0x13
#define AVC_1U_LENGTH	                    0x02
#endif
#if CRYSTAL == 3
#define AVC_NORMAL_BIT_LENGTH           	0x4A
#define AVC_BIT_1_HOLD_ON_LENGTH			0x28
#define AVC_BIT_0_HOLD_ON_LENGTH			0x40
#define AVC_BIT_0_HOLD_ON_MIN_LENGTH		0x32
#define AVC_START_BIT_LENGTH				0x5D
#define AVC_START_BIT_HOLD_ON_LENGTH		0x54
#define AVC_START_BIT_HOLD_ON_MIN_LENGTH	0x16
#define AVC_1U_LENGTH	                    0x02
#endif

#endif