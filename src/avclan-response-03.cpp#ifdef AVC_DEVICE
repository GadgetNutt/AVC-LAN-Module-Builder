/*
avclan-response-03.cpp - AVCLan Serial library for avclan interface for Atmega328
Created by Greg Nutt 2021-11-12
Version 0.0.1

The only likely use for this module would be for testing purposes or to build a custom replacement GW Module
which would also have to interface with the other communication buses of the car.

Not for commercial use.
*/

#include "avclan-responses.h"
#ifdef AVC_DEVICE


void avclan_response::respond03(avclan_frame_t* msg_frame) {
	switch (messages.getDestDevice(msg_frame)) {	 //	Get logical device being addressed and respond accordingly
	case 0x01:
		/*  Response to my cmd13, master device identifying itself.
			Save to my master_device class variable
		*/
		device.master_device = msg_frame->master;
#ifdef local_debug
		avcSerial.print("INFO_DISPLAY records master as: ");
		avcSerial.printHex8(msg_frame->master >> 8);
		avcSerial.printHex8(msg_frame->master);
		avcSerial.println();
#endif
		break;

	default:
		break;
	}


}

#endif
