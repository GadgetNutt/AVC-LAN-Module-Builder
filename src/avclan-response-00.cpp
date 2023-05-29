/*
avclan-response-00.cpp - AVCLan library for avclan interface for Atmega328
Created by Greg Nutt 2021-11-12
Version 0.0.1

The only likely use for this module would be for testing purposes or to build a custom replacement GW Module
which would also have to interface with the other communication buses of the car.,

Not for commercial use.
*/

#include "avclan-includes.h"
#ifdef AVC_DEVICE

avclan_response respond;

//	Network device advertising its own logical devices
void avclan_response::respond00(avclan_frame_t* msg_frame) {
	switch (messages.getDestDevice(msg_frame)) {	 //	Get logical device being addressed and respond accordingly
	case 0x01:
		//  AVCLAN Init
		messages.cmd10(device.my_devices, sizeof(device.my_devices) / sizeof(device.my_devices[0]), ADDR_ME, device.master_device, 0x01, 0x11);

	default:

			break;

	}
}
#endif
