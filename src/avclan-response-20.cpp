/*
avclan-response-20.cpp - AVCLan Serial library for avclan interface for Atmega328
Created by Greg Nutt 2021-11-12
Version 0.0.1

The only likely use for this module would be for testing purposes or to build a custom replacement GW Module
which would also have to interface with the other communication buses of the car.

Not for commercial use.
*/

#include "avclan-responses.h"
#ifdef AVC_DEVICE


void avclan_response::respond20(avclan_frame_t* msg_frame) {
	switch (messages.getDestDevice(msg_frame)) {	 //	Get logical device being addressed and respond accordingly
	case 0x01:
		/*  Received Ping from network device
			Respond to ping with cmd30
		*/
		messages.cmd30(ADDR_ME, msg_frame->master, msg_frame->data[3]);
		break;

	default:

		break;

	}

}

#endif
