/*
  avclan-serial.cpp - AVCLan Serial library for avclan interface for Atmega328
  Created by Greg Nutt 2020-12-15
  Version 0.0.1

  Not for commercial use.
*/

#include "avclan-serial.h"
#include <stdio.h>
#include "config.h"
#include "Arduino.h"

void avclan_serial::begin(long speed) {
	Serial.begin(speed);
}


void avclan_serial::print(const char* pBuf) {
	Serial.print(pBuf);
	//digitalWrite(SS, LOW);
	//Mastereceive = SPI.transfer(pBuf,sizeof(pBuf)/sizeof(char));
}

void avclan_serial::print(const char pBuf) {
	Serial.print(pBuf);
}


void avclan_serial::println(const char* pBuf) {
	Serial.print(pBuf);
	Serial.println();
}

void avclan_serial::println(const char pBuf) {
	Serial.print(pBuf);
	Serial.println();
}

void avclan_serial::println(void) {
	Serial.println();
}

void avclan_serial::printHex4(uint8_t data) {
	Serial.print(data, HEX);

	/*
	uint8_t c = data & 0x0f;
	c += c < 10 ? '0' : 'A' - 10 ;
	sendByte(c);
	*/
}

void avclan_serial::printHex8(uint8_t data) {
	unsigned long mask = 0x000F;
	mask = mask << 4;

	for (unsigned int i = 2; i > 0; --i) {
		Serial.print(((data & mask) >> (i - 1) * 4), HEX);
		mask = mask >> 4;
	}
}

void avclan_serial::printDec(uint8_t data) {
	Serial.print(data, DEC);
}

// check rx buffer not empty
bool avclan_serial::available(void) {

	return Serial.available();
}

uint8_t avclan_serial::read(void) {
	return Serial.read();
}


void avclan_serial::printHex8(uint8_t* data, uint8_t length) // prints 8-bit data in hex
{
	char tmp[length * 2 + 1];
	byte first;
	int j = 0;
	for (uint8_t i = 0; i < length; i++)
	{
		first = (data[i] >> 4) | 48;
		if (first > 57) tmp[j] = first + (byte)39;
		else tmp[j] = first;
		j++;

		first = (data[i] & 0x0F) | 48;
		if (first > 57) tmp[j] = first + (byte)39;
		else tmp[j] = first;
		j++;
	}
	tmp[length * 2] = 0;
	
	Serial.println(tmp);
}


avclan_serial avcSerial;