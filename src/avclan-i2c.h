/*
  avclan-i2c.h - AVCLan I2C library for avclan interface for Atmega328
  Created by Greg Nutt 2021-11-18
  Version 0.0.1

  Not for commercial use.
*/

#include <stdio.h>

#pragma once

#ifndef avclan_i2c_h
#define avclan_i2c_h
//#define local_debug

class avclan_i2c {
public:
	void		begin(int);
	void		print(const char*);
	void		print(const char);
	void		println(const char*);
	void		println(const char);
	void		println(void);
	void		printHex4(uint8_t);
	void		printHex8(uint8_t);
	void		printHex8(uint8_t* data, uint8_t length);	 // prints 8-bit data in hex
	void		printDec(uint8_t);
	bool		available(void);
	uint8_t		read(void);
};

extern avclan_i2c avcI2C;

#endif
