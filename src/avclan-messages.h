/*
  avclan-messages.h - AVCLan Messages library for avclan interface for Atmega328
  Created by Greg Nutt 2020-12-26
  Version 0.0.1

  Not for commercial use.
*/

#pragma once

#include <stdint.h>

#ifndef avclan_messages_h
#define avclan_messages_h

	const uint8_t	AVCLAN_DIRECT_MSG = 0x1;
	const uint8_t	AVCLAN_BROADCAST_MSG = 0x0;

	// Bit Commands
	const uint8_t	CMD_BIT_INIT		= 0x01;			//	0 0 0 0 0 0 0 1
	const uint8_t	CMD_BIT_DEVICE		= 0x02;			//	0 0 0 0 0 0 1 0
	const uint8_t	CMD_BIT_BIT3		= 0x04;			//	0 0 0 0 0 1 0 0
	const uint8_t	CMD_BIT_BIT4		= 0x08;			//	0 0 0 0 1 0 0 0
	const uint8_t	CMD_BIT_REGISTER	= 0x10;			//	0 0 0 1 0 0 0 0
	const uint8_t	CMD_BIT_PING		= 0x20;			//	0 0 1 0 0 0 0 0
	const uint8_t	CMD_BIT_MASTER		= 0x40;			//	0 1 0 0 0 0 0 0
	const uint8_t	CMD_BIT_STATUS		= 0x80;			//	1 0 0 0 0 0 0 0

typedef struct {
	uint16_t network_device;
	uint8_t logic_device;
} avclan_logical_device_map_t;

typedef struct {
	bool direct;
	uint16_t master;
	uint16_t slave;
	uint8_t size;
	uint8_t data[32];
} avclan_frame_t;

/*
avclan_frame frame_table[] = PROGMEM{

};
*/

class avclan_messages {
public:
	void				cmdMessage(uint8_t cmd, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev);
	void				cmd10(uint8_t device_list[], uint8_t num_devices, uint16_t lan_master, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev);					// Report sending logical devices
	void				cmd12(avclan_logical_device_map_t device_map[], uint8_t num_devices, uint16_t lan_master, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev);	// Report listening logical devices
	void				cmd13(uint8_t device_list[], uint8_t num_devices, uint16_t lan_master, uint8_t src_dev, uint8_t dest_dev);	// Broadcast logical devices
	void				cmd30(uint16_t lan_master, uint16_t lan_slave, uint8_t ping);	// Pong!
	void				cmdE(uint16_t lan_master, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev, uint8_t request_type);		// Status request
	uint8_t				getFunction(avclan_frame_t* msg_frame);
	uint8_t				getSrcDevice(avclan_frame_t* msg_frame);
	uint8_t				getDestDevice(avclan_frame_t* msg_frame);
};

extern avclan_messages messages;

#endif