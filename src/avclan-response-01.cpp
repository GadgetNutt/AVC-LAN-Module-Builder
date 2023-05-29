/*
avclan-response-01.cpp - AVCLan Serial library for avclan interface for Atmega328
Created by Greg Nutt 2021-11-12
Version 0.0.1

The only likely use for this module would be for testing purposes or to build a custom replacement GW Module
which would also have to interface with the other communication buses of the car.

Not for commercial use.
*/

#include "avclan-responses.h"
#ifdef AVC_DEVICE


void avclan_response::respond01(avclan_frame_t* msg_frame) {
	switch (messages.getDestDevice(msg_frame)) {	 //	Get logical device being addressed and respond accordingly
	case 0x01:
		/*  Logical device lists received by master.
			Time to send the logical device list I want to listen to
		*/
		messages.cmd12(device.my_listening_device_map, sizeof(device.my_listening_device_map) / sizeof(device.my_listening_device_map[0]), ADDR_ME, device.master_device, 0x01, 0x11);
		break;

	default:

		break;

	}

}

#endif
