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
#define AVC_MASTER

/* Define Connections*/
#define AVC_I2C

const uint16_t  ADDR_ME = ADDR_DISPLAY_MASTER;	//	See avclan-registers.h for list

#define MAXSENDATTEMP	3

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

#endif

