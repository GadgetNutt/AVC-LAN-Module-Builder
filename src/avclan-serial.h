/*
  avclan-serial.h - AVCLan Serial library for avclan interface for Atmega328
  Created by Greg Nutt 2020-12-15
  Version 0.0.1

  Not for commercial use.
*/

#include <stdio.h>

#pragma once

#ifndef avclan_serial_h
#define avclan_serial_h
//#define local_debug

class avclan_serial {
public:
	void		begin(long);
	void		print(const char*);
	void		print(const char);
	void		println(const char*);
	void		println(const char);
	void		println(void);
	void		printHex4(uint8_t);
	void		printHex8(uint8_t);
	void		printHex8(uint8_t* data, uint8_t length);	 // prints 8-bit data in hex
	void		printHex16(uint16_t);
	void		printDec(uint8_t);
	bool		available(void);
	uint8_t		read(void);
};

extern avclan_serial avcSerial;

#endif
