/*
avclan-i2c.cpp - AVCLan I2C library for avclan interface for Atmega328
Created by Greg Nutt 2021-12-18
Version 0.0.1

Not for commercial use.
*/

#include "avclan-i2c.h"
#include <stdio.h>
#include "config.h"
#include "Arduino.h"
#include <Wire.h>

void avclan_i2c::begin(int address) {
	Wire.begin(address);
}


void avclan_i2c::print(const char* pBuf) {
	Wire.write(pBuf);
}

void avclan_i2c::print(const char pBuf) {
	Wire.write(pBuf);
}


void avclan_i2c::println(const char* pBuf) {
	Wire.write(pBuf);
	Wire.write(0x0A);
}

void avclan_i2c::println(const char pBuf) {
	Wire.write(pBuf);
	Wire.write(0x0A);
}

void avclan_i2c::println(void) {
	Wire.write(0x0A);
}

void avclan_i2c::printHex4(uint8_t data) {
	Serial.print(data, HEX);

	/*
	uint8_t c = data & 0x0f;
	c += c < 10 ? '0' : 'A' - 10 ;
	sendByte(c);
	*/
}

void avclan_i2c::printHex8(uint8_t data) {
	unsigned long mask = 0x000F;
	mask = mask << 4;

	for (unsigned int i = 2; i > 0; --i) {
		Serial.print(((data & mask) >> (i - 1) * 4), HEX);
		mask = mask >> 4;
	}
}

void avclan_i2c::printDec(uint8_t data) {
	Serial.print(data, DEC);
}

// check rx buffer not empty
bool avclan_i2c::available(void) {

	return Serial.available();
}

uint8_t avclan_i2c::read(void) {
	return Serial.read();
}


void avclan_i2c::printHex8(uint8_t* data, uint8_t length) // prints 8-bit data in hex
{
	char tmp[length * 2 + 1];
	uint8_t first;
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


avclan_i2c avcI2C;