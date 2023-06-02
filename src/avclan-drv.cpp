/*
  avclan-drv.cpp - AVCLan Head Unit library for for Atmega328
  Created by Greg Nutt 2020-12-07
  Derived from AVCLanDrv.cpp by Kochetkov Aleksey, 04.08.2010
  Version 0.0.1

  Not for commercial use.
*/

#include "avclan-drv.h"
#include "avclan-serial.h"
#ifdef AVC_I2C
#include "avclan-i2c.h"
#endif
//#include <Arduino.h>						          
#ifdef AVR
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#endif
#ifdef AVC_I2C
#include <Wire.h>
#endif

void avclan_drv::begin() {
	// AVCLan TX+/TX- 	read line INPUT														                                                                                                                           
	cbi(DATAIN_DDR, DATAIN);
	cbi(DATAIN_PORT, DATAIN);

	// AVCLan TX+/TX-		write line OUTPUT	
	cbi(DATAOUT_DDR, DATAOUT);
	cbi(DATAOUT_PORT, DATAOUT);
	cbi(ADCSRB, ACME);	// Analog Comparator Multiplexer Enable - NO
	cbi(ACSR, ACIS1);	// Analog Comparator Interrupt Mode Select
	cbi(ACSR, ACIS0);   // Comparator Interrupt on Output Toggle
	cbi(ACSR, ACD); 	// Analog Comparator Disable - NO


	// timer2 setup, prescaler factor - 8
	TCCR2B = 0x02;

	// timer1 setup, prescaler factor - 1024
	TCCR1A = 0;     // normal mode
	TCCR1B = 5;     // Prescaler /1024
	TCNT1H = TI1_H; // Load counter value hi
	TCNT1L = TI1_L;	// Load counter value lo
	DISABLE_TIMER1_INT;
	//AZFM_INIT;

}

// Reads specified number of bits from the AVCLan.
// numBits (uint8_t) -> Number of bits to read.
// Return (uint16_t) -> Data value read.
uint16_t avclan_drv::readBits(uint8_t numBits) {
	uint16_t data = 0;
	_parityBit = 0;

	while (numBits-- > 0) {
		// Insert new bit
		data <<= 1;

		// Wait until rising edge of new bit.
		while (INPUT_IS_CLEAR) {}

		// Reset timer to measure bit length.
		TCNT2 = 0;

		// Wait until falling edge.
		while (INPUT_IS_SET);

		// Compare half way between a '1' (20 us) and a '0' (32 us ): 32 - (32 - 20) /2 = 26 us
		if (TCNT2 < AVC_BIT_0_HOLD_ON_MIN_LENGTH) {
			// Set new bit.
			data |= 0x0001;

			// Adjust parity.
			_parityBit = !_parityBit;
		}
	}
	while (INPUT_IS_CLEAR && TCNT2 < AVC_NORMAL_BIT_LENGTH);

	return data;
}

// Read incoming messages on the AVCLan.
// Return true if success.
uint8_t avclan_drv::_readMessage(avclan_frame_t *msg_frame) {

	uint8_t t = 0;
	uint8_t oldSREG = SREG;
	cli();             // disable interrupts

	// Start bit.
	while (INPUT_IS_CLEAR);
	TCCR2B = 0x03;      // prescaler 32
	TCNT2 = 0;
	// Wait until falling edge.
	while (INPUT_IS_SET) {
		t = TCNT2;
		if (t > 0xFF) {
			TCCR2B = 0x02;      // prescaler 8
			SREG = oldSREG;
			return 1;
		}
	}
	TCCR2B = 0x02;      // prescaler 8

	if (t < AVC_START_BIT_HOLD_ON_MIN_LENGTH) {
		//if (t < 0x16){
		SREG = oldSREG;
		return 2;
	}

	msg_frame->direct = readBits(1);

	msg_frame->master = readBits(12);
	bool p = _parityBit;
	if (p != readBits(1)) {
		SREG = oldSREG;
		return 3;
	}

	msg_frame->slave = readBits(12);
	p = _parityBit;
	if (p != readBits(1)) {
		SREG = oldSREG;
		return 4;
	}

	bool forMe = (msg_frame->slave == ADDR_ME);

	if (forMe) {
		// Send ACK.
		AVC_OUT_EN;
		send1BitWord(0);
		AVC_OUT_DIS;
	}
	else {
		readBits(1);
	}

	// Control 
	readBits(4);
	p = _parityBit;
	if (p != readBits(1)) {
		SREG = oldSREG;
		return 5;
	}

	if (forMe) {
		// Send ACK.
		AVC_OUT_EN;
		send1BitWord(0);
		AVC_OUT_DIS;
	}
	else {
		readBits(1);
	}

	msg_frame->size = readBits(8);
	p = _parityBit;
	if (p != readBits(1)) {
		SREG = oldSREG;
		return 6;
	}

	if (forMe) {
		// Send ACK.
		AVC_OUT_EN;
		send1BitWord(0);
		AVC_OUT_DIS;
	}
	else {
		readBits(1);
	}
	if (msg_frame->size > AVC_MAXMSGLEN) {
		SREG = oldSREG;
		return 7;
	}
	uint8_t i;
	for (i = 0; i < msg_frame->size; i++) {
		msg_frame->data[i] = readBits(8);
		p = _parityBit;
		if (p != readBits(1)) {
			SREG = oldSREG;
			return 8;
		}

		if (forMe) {
			// Send ACK.
			AVC_OUT_EN;
			send1BitWord(0);
			AVC_OUT_DIS;
		}
		else {
			readBits(1);
		}
	}
	SREG = oldSREG;
	return 0;
}

// Read incoming messages on the AVCLan, log message through serial port
// Return true if success.
uint8_t avclan_drv::readMessage(avclan_frame_t *msg_frame) {

	uint8_t res = _readMessage(msg_frame);
	if (!res) {
		avclan.printMessage(msg_frame, 1);
	}
	else {
		avcSerial.print("R");
		avcSerial.printHex4(res);
		avcSerial.println();
		while (!avclan.isAvcBusFree());
	}
	return res;
}

// Send a start bit to the AVCLan
void avclan_drv::sendStartBit() {
	// Reset timer to measure bit length.
	TCCR2B = 0x03;      // prescaler 32
	TCNT2 = 0;
	OUTPUT_SET_1;

	// Pulse level high duration.
	while (TCNT2 < AVC_START_BIT_HOLD_ON_LENGTH);
	OUTPUT_SET_0;

	// Pulse level low duration until ~185 us.
	while (TCNT2 < AVC_START_BIT_LENGTH);
	TCCR2B = 0x02;      // prescaler 8

}

// Send a 1 bit uint16_t to the AVCLan
void avclan_drv::send1BitWord(bool data) {
	// Reset timer to measure bit length.
	TCNT2 = 0;
	OUTPUT_SET_1;

	if (data) {
		while (TCNT2 < AVC_BIT_1_HOLD_ON_LENGTH);
	}
	else {
		while (TCNT2 < AVC_BIT_0_HOLD_ON_LENGTH);
	}

	OUTPUT_SET_0;
	while (TCNT2 < AVC_NORMAL_BIT_LENGTH);
}

// Send a 4 bit uint16_t to the AVCLan
void avclan_drv::send4BitWord(uint8_t data) {
	_parityBit = 0;

	// Most significant bit out first.   
	for (char nbBits = 0; nbBits < 4; nbBits++) {
		// Reset timer to measure bit length.
		TCNT2 = 2;
		OUTPUT_SET_1;

		if (data & 0x8) {
			// Adjust parity.
			_parityBit = !_parityBit;
			while (TCNT2 < AVC_BIT_1_HOLD_ON_LENGTH);
		}
		else {
			while (TCNT2 < AVC_BIT_0_HOLD_ON_LENGTH);
		}

		OUTPUT_SET_0;
		// Hold output low until end of bit.
		while (TCNT2 < AVC_NORMAL_BIT_LENGTH);

		// Fetch next bit.
		data <<= 1;
	}
}

// Send a 8 bit uint16_t to the AVCLan
void avclan_drv::send8BitWord(uint8_t data) {
	_parityBit = 0;

	// Most significant bit out first.   
	for (char nbBits = 0; nbBits < 8; nbBits++) {
		// Reset timer to measure bit length.
		TCNT2 = 2;
		OUTPUT_SET_1;

		if (data & 0x80) {
			// Adjust parity.
			_parityBit = !_parityBit;
			while (TCNT2 < AVC_BIT_1_HOLD_ON_LENGTH);
		}
		else {
			while (TCNT2 < AVC_BIT_0_HOLD_ON_LENGTH);
		}

		OUTPUT_SET_0;
		// Hold output low until end of bit.
		while (TCNT2 < AVC_NORMAL_BIT_LENGTH);

		// Fetch next bit.
		data <<= 1;
	}
}

// Send a 12 bit uint16_t to the AVCLan
void avclan_drv::send12BitWord(uint16_t data) {
	_parityBit = 0;

	// Most significant bit out first.   
	for (char nbBits = 0; nbBits < 12; nbBits++) {
		// Reset timer to measure bit length.
		TCNT2 = 2;
		OUTPUT_SET_1;

		if (data & 0x0800) {
			// Adjust parity.
			_parityBit = !_parityBit;
			while (TCNT2 < AVC_BIT_1_HOLD_ON_LENGTH);
		}
		else {
			while (TCNT2 < AVC_BIT_0_HOLD_ON_LENGTH);
		}

		OUTPUT_SET_0;
		// Hold output low until end of bit.
		while (TCNT2 < AVC_NORMAL_BIT_LENGTH);

		// Fetch next bit.
		data <<= 1;
	}
}

//  determine whether the bus is free (no tx/rx).
//  return TRUE is bus is free.
bool avclan_drv::isAvcBusFree(void) {
	// Reset timer.
	TCNT2 = 0;

	while (INPUT_IS_CLEAR) {
		// We assume the bus is free if anything happens for the length of 1 bit.
		if (TCNT2 > AVC_NORMAL_BIT_LENGTH) {
			return true;
		}
	}

	return false;
}

// reads the acknowledge bit the AVCLan
// return TRUE if ack detected else FALSE.
bool avclan_drv::readAcknowledge(void) {
	// The acknowledge pattern is very tricky: the sender shall drive the bus for the equivalent
	// of a bit '1' (20 us) then release the bus and listen. At this point the target shall have
	// taken over the bus maintaining the pulse until the equivalent of a bit '0' (32 us) is formed.

	// Reset timer to measure bit length.
	TCNT2 = 0;
	OUTPUT_SET_1;

	// Generate bit '0'.
	while (TCNT2 < AVC_BIT_1_HOLD_ON_LENGTH);
	OUTPUT_SET_0;

	AVC_OUT_DIS;

	while (TCNT2 < AVC_BIT_1_HOLD_ON_LENGTH + AVC_1U_LENGTH);
	// Measure final resulting bit.
	while (INPUT_IS_SET);

	// Sample half-way through bit '0' (26 us) to detect whether the target is acknowledging.
	if (TCNT2 > AVC_BIT_0_HOLD_ON_MIN_LENGTH) {
		// Slave is acknowledging (ack = 0). Wait until end of ack bit.
		while (INPUT_IS_SET);
		AVC_OUT_EN;
		return true;
	}

	// No sign of life on the bus.
	return false;
}

// sends ack bit if I am broadcasting otherwise wait and return received ack bit.
// return FALSE if ack bit not detected.
bool avclan_drv::handleAcknowledge(bool direct) {
	if (direct) {
		// Acknowledge.    
		send1BitWord(0);
		return true;
	}

	// Return acknowledge bit.
	return readAcknowledge();
}

// sends the message in global registers on the AVC LAN bus.
// return 0 if successful else error code
uint8_t avclan_drv::_sendMessage(avclan_frame_t *msg_frame) {
	uint8_t oldSREG = SREG;
	cli();             // disable interrupts
	while (!isAvcBusFree());

	AVC_OUT_EN;

	// Send start bit.
	sendStartBit();

	// Broadcast bit.
	send1BitWord(msg_frame->direct);

	// Master address
	send12BitWord(msg_frame->master);
	send1BitWord(_parityBit);

	// Slave address
	send12BitWord(msg_frame->slave);
	send1BitWord(_parityBit);
	if (!handleAcknowledge(msg_frame->direct)) {
		AVC_OUT_DIS;
		SREG = oldSREG;
		return 1;
	}

	// Control flag + parity.
	send4BitWord(AVC_CONTROL_FLAGS);
	send1BitWord(_parityBit);
	if (!handleAcknowledge(msg_frame->direct)) {
		AVC_OUT_DIS;
		SREG = oldSREG;
		return 2;
	}

	// Data length + parity.
	send8BitWord(msg_frame->size);
	send1BitWord(_parityBit);
	if (!handleAcknowledge(msg_frame->direct)) {
		AVC_OUT_DIS;
		SREG = oldSREG;
		return 3;
	}

	for (uint8_t i = 0; i < msg_frame->size; i++) {
		send8BitWord(msg_frame->data[i]);
		send1BitWord(_parityBit);
		if (!handleAcknowledge(msg_frame->direct)) {
			AVC_OUT_DIS;
			SREG = oldSREG;
			return false;
		}
	}
	AVC_OUT_DIS;
	SREG = oldSREG;
	return 0;
}

// sends the message in global registers on the AVC LAN bus, log message through serial port
// return 0 if successful else error code
uint8_t avclan_drv::sendMessage(avclan_frame_t* msg_frame, bool print) {
	uint8_t sc = MAXSENDATTEMPT;
	uint8_t res;

	do {
		//TX_LED_ON;
		sbi(TX_LED_DDR, TX_LED_OUT);
		res = avclan._sendMessage(msg_frame);
		//TX_LED_OFF;
		cbi(TX_LED_PORT, TX_LED_OUT);
		if (!res) {
			if (print) {
				avclan.printMessage(msg_frame, 0);
			}
		}
		else {
			avcSerial.print("W");
			avcSerial.printHex4(res);
			avcSerial.print(" - ");
			avclan.printMessage(msg_frame, 0);
			avcSerial.println();
			while (!avclan.isAvcBusFree());
		}
		sc--;
	} while (sc && res);
	return res;
}

// sends the message for given mesage ID on the AVC LAN bus, log message through serial port
// return 0 if successful else error code
uint8_t avclan_drv::sendMessage(const avclan_frame_t msg, bool print) {
	//loadMessage(msg);	//  Loaded pre-defined message from list in memory

	avclan_frame_t msg_frame;

	msg_frame.direct = msg.direct;
	msg_frame.master = msg.master;
	msg_frame.slave = msg.slave;
	msg_frame.size = msg.size;
	for (uint8_t i = 0; i < msg.size; ++i) {
		msg_frame.data[i] = msg.data[i];
	}

	return sendMessage(&msg_frame, print);
}

// print message to serial port
void avclan_drv::printMessage(avclan_frame_t* msg_frame, uint8_t incoming) {
#ifdef AVC_I2C
		sendMessageToI2C(msg_frame);
#endif
	switch (incoming) {
	case 0:
		avcSerial.print("> ");
		break;

	case 1:
		avcSerial.print("< ");
		break;

	case 2:
		avcSerial.print("~ ");
		break;
	}

	if (msg_frame->direct == AVCLAN_BROADCAST_MSG) {
		avcSerial.print("b ");
	}
	else {
		avcSerial.print("d ");
	}
	avcSerial.printHex4(msg_frame->master >> 8);
	avcSerial.printHex8(msg_frame->master);
	avcSerial.print(" ");

	avcSerial.printHex4(msg_frame->slave >> 8);
	avcSerial.printHex8(msg_frame->slave);
	avcSerial.print(" ");
	avcSerial.printHex8(msg_frame->size);
	avcSerial.print(" ");

	for (uint8_t i = 0; i < msg_frame->size; i++) {
		avcSerial.printHex8(msg_frame->data[i]);
	}
	avcSerial.println();

}

#ifdef AVC_I2C
void avclan_drv::sendMessageToI2C(avclan_frame_t* msg_frame) {

	//  Send Master / Slave addresses
	Wire.beginTransmission(0xD2);
	uint8_t address[5];
	address[0] = msg_frame->master >> 8;
	address[1] = msg_frame->master;
	address[2] = msg_frame->slave >> 8;
	address[3] = msg_frame->slave;
	address[4] = msg_frame->size;
	Wire.write(address, 5);
	Wire.write(msg_frame->data, msg_frame->size);
	Wire.endTransmission();

}
#endif

avclan_drv avclan;
