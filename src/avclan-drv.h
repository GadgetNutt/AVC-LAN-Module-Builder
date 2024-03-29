/*
  avclan-drv.h - AVCLan driver library for Atmega328
  Created by Greg Nutt, 012.07.2020
  with some code derived from AVCLanDrv.h created by Kochetkov Aleksey, 04.08.2010
  Version 0.0.1

  Not for commercial use.
*/

#include "avclan-messages.h"
#include "avclan-registers.h"
#include "config.h"
//#include <avr/pgmspace.h>
#include <stdint.h>

#ifndef avclan_drv_h
#define avclan_drv_h

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
//#define cbi(sfr, bit) (sfr &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
//#define sbi (sfr,bit) (sfr |= _BV(bit))
#endif

#define TX_LED_ON	sbi(TX_LED_PORT, TX_LED_OUT);
#define TX_LED_OFF	cbi(TX_LED_PORT, TX_LED_OUT);
#define RX_LED_ON	sbi(RX_LED_PORT, RX_LED_OUT);
#define RX_LED_OFF	cbi(RX_LED_PORT, RX_LED_OUT);


#define INPUT_IS_SET   (ACSR & _BV(ACO))
#define INPUT_IS_CLEAR (!(ACSR & _BV(ACO)))
#define OUTPUT_SET_1   sbi(PORTD, DATAOUT);
#define OUTPUT_SET_0   cbi(PORTD, DATAOUT);
#define AVC_OUT_EN     sbi(PORTD, DATAOUT); sbi(DDRD, DATAOUT);  sbi(DDRD, DATAIN); sbi(ACSR, ACD); 
#define AVC_OUT_DIS    cbi(PORTD, DATAOUT); cbi(DDRD, DATAOUT);  cbi(DDRD, DATAIN); cbi(ACSR, ACD);

// timer1 overflow every 1 sec 
#define TI1_H	(((uint16_t)-(F_CPU / 1024)) >> 8)
#define TI1_L	(((uint16_t)-(F_CPU / 1024)) & 0xff )

#define ENABLE_TIMER1_INT  (sbi(TIMSK1, TOIE1))
#define DISABLE_TIMER1_INT (cbi(TIMSK1, TOIE1))

#include "avclan-messages.h"

class avclan_drv {
public:
	void			begin();
	uint16_t		readBits(uint8_t numBits);
	uint8_t			readMessage(avclan_frame_t* msg_frame);
	uint8_t			sendMessage(avclan_frame_t* msg_frame, bool print = true);
	uint8_t			sendMessage(const avclan_frame_t msg, bool print = true);
	void			printMessage(avclan_frame_t* msg_frame, uint8_t incoming);
private:
	bool			_parityBit;
	uint8_t			_readMessage(avclan_frame_t *msg_frame);
	void			sendStartBit();
	void			send1BitWord(bool data);
	void			send4BitWord(uint8_t data);
	void			send8BitWord(uint8_t data);
	void			send12BitWord(uint16_t data);
	bool			isAvcBusFree(void);
	bool			readAcknowledge(void);
	bool			handleAcknowledge(bool direct);
	uint8_t			_sendMessage(avclan_frame_t *msg_frame);
	void			sendMessageToI2C(avclan_frame_t* msg_frame);
};


extern avclan_drv avclan;
#endif