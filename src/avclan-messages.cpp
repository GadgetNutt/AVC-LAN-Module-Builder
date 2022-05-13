/*
  avclan-messages.cpp - AVCLAN base messages library
  Created by Greg Nutt 2021-04-04
  Version 0.0.1
  
  Not for commercial use.  
*/

#include "avclan-messages.h"
#include "avclan-drv.h"
#include "avclan-master.h"
#include "avclan-router.h"
#include "avclan-serial.h"


void avclan_messages::cmdMessage(uint8_t cmd, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev) {
	switch (cmd) {
		case 0x12:
			break;
	}

}

uint8_t avclan_messages::getFunction(avclan_frame_t* msg_frame) {

	if (msg_frame->direct) {
		return msg_frame->data[3];   //  Return from Direct message
	}
	else {
		return msg_frame->data[2];   //  Return from Broadcast message
	}
}

uint8_t avclan_messages::getSrcDevice(avclan_frame_t* msg_frame) {
	if (msg_frame->direct) {
		return msg_frame->data[1];   //  Return from Direct message
	}
	else {
		return msg_frame->data[0];   //  Return from Broadcast message
	}
}

uint8_t avclan_messages::getDestDevice(avclan_frame_t* msg_frame) {
	if (msg_frame->direct) {
		return msg_frame->data[2];   //  Return from Direct message
	}
	else {
		return msg_frame->data[1];   //  Return from Broadcast message
	}
}


//	Network device advertising its own logical devices
void avclan_messages::cmd10(uint8_t device_list[], uint8_t num_devices, uint16_t lan_master, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev) {
	//	I have these device ID's
//	ei. < d 1D4 190 09 00 0111  10   32 3F 55 5D 5F
	avclan_frame_t msg_frame;

	msg_frame.direct = 1;
	msg_frame.master = lan_master;
	msg_frame.slave = lan_slave;
	msg_frame.size = 0x04 + num_devices;
	msg_frame.data[0] = 0x00;
	msg_frame.data[1] = 0x01;
	msg_frame.data[2] = 0x11;
	msg_frame.data[3] = 0x10;
	for (int i = 0; i < num_devices; i++) {
		msg_frame.data[4 + i] = device_list[i];
	}
	msg_frame.data[4 + num_devices] = '\0';

	if (msg_frame.master == ADDR_ME) {
		//	If the message is for the defined master device, we only process it to the master's memory.
//#ifdef local_debug
		avcSerial.print("Sending cmd10 to local master: ");
		avcSerial.printHex8(lan_slave >> 8);
		avcSerial.printHex8(lan_slave);
		avcSerial.print(" from: ");
		avcSerial.printHex8(lan_master >> 8);
		avcSerial.printHex8(lan_master);
		avcSerial.println();
//#endif
		avclan.printMessage(&msg_frame, 2);
		master.register_devices_message_10(&msg_frame);
	}
	else {
		router.sendMessage(&msg_frame);
	}
}

// Network device expressing that it wants to hear from these devices
void avclan_messages::cmd12(avclan_logical_device_map_t device_map[], uint8_t num_devices, uint16_t lan_master, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev) {
	//	I want to hear these device ID's
	//	ei. < d 1D4 190 11 00 0111  12   00 29 43 60 61 62 63 6D 6F 74 C0 E0 E5
	avclan_frame_t msg_frame;

	if (lan_slave == 0x0) return;	// Means master device trying to register which isn't necessary

	msg_frame.direct = 1;
	msg_frame.master = lan_master;
	msg_frame.slave = lan_slave;
	msg_frame.size = 0x04 + num_devices;
	msg_frame.data[0] = 0x00;
	msg_frame.data[1] = 0x01;
	msg_frame.data[2] = 0x11;
	msg_frame.data[3] = 0x12;
	for (int i = 0; i < num_devices; i++) {
		msg_frame.data[4 + i] = device_map[i].logic_device;
	}
	msg_frame.data[4 + num_devices] = '\0';

	router.sendMessage(&msg_frame);
}

//	Network device advertising its own logical devices
void avclan_messages::cmd13(uint8_t device_list[], uint8_t num_devices, uint16_t lan_master, uint8_t src_dev, uint8_t dest_dev) {
	avclan_frame_t msg_frame;

#ifdef local_debug
	avcSerial.print("Preparing to send cmd13 for: ");
	avcSerial.printHex8(lan_master >> 8);
	avcSerial.printHex8(lan_master);
	avcSerial.println();
#endif
	//	I have these device ID's
	//	ei. < b 1D4 1FF 08 0111 13	32 3F 55 5D 5F
	msg_frame.direct = 0;
	msg_frame.master = lan_master;
	msg_frame.slave = 0x01FF;
	msg_frame.size = 0x03 + num_devices;
	msg_frame.data[0] = 0x01;
	msg_frame.data[1] = 0x11;
	msg_frame.data[2] = 0x13;
	for (int i = 0; i < num_devices; i++) {
		msg_frame.data[3 + i] = device_list[i];
	}
	msg_frame.data[3 + num_devices] = '\0';

	router.sendMessage(&msg_frame);
}

void avclan_messages::cmd30(uint16_t lan_master, uint16_t lan_slave, uint8_t ping) {
	avclan_frame_t msg_frame;
	//  Pong!  (Ping (cmd20) response)
	//  190 FFF 04 110120D4
	//  1D4 190 06 00011130D4FF

	msg_frame.direct = 1;
	msg_frame.master = lan_master;
	msg_frame.slave = lan_slave;
	msg_frame.size = 0x06;
	msg_frame.data[0] = 0x00;
	msg_frame.data[1] = 0x01;
	msg_frame.data[2] = 0x11;
	msg_frame.data[3] = 0x30;
	msg_frame.data[4] = ping;
	msg_frame.data[5] = 0xFF;
	msg_frame.data[6] = '\0';

	router.sendMessage(&msg_frame);
}

void avclan_messages::cmdE(uint16_t lan_master, uint16_t lan_slave, uint8_t src_dev, uint8_t dest_dev, uint8_t request_type) {
	avclan_frame_t msg_frame;
	// Example request and response
	// < d 1D4 190 04 00 0174  E0
	// < d 190 1D4 11 00 7401  F0   40 77 50 50 18 A8 0E 01 00 00 00 00 60

	msg_frame.direct = 1;
	msg_frame.master = lan_master;
	msg_frame.slave = lan_slave;
	msg_frame.size = 0x04;
	msg_frame.data[0] = 0x00;
	msg_frame.data[1] = src_dev;
	msg_frame.data[2] = dest_dev;
	msg_frame.data[5] = request_type;
	msg_frame.data[6] = '\0';

	router.sendMessage(&msg_frame);
}

avclan_messages messages;
