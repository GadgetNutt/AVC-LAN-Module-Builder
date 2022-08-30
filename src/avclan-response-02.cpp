/*
avclan-response-02.cpp - AVCLan Serial library for avclan interface for Atmega328
Created by Greg Nutt 2021-11-12
Version 0.0.1

The only likely use for this module would be for testing purposes or to build a custom replacement GW Module
which would also have to interface with the other communication buses of the car.

Not for commercial use.
*/

#include "avclan-responses.h"


void avclan_response::respond02(avclan_frame_t* msg_frame) {
	switch (messages.getDestDevice(msg_frame)) {	 //	Get logical device being addressed and respond accordingly
	case 0x01:
		/*  My device assignments from the master network device
			Master device's response to my cmd12
			Map assigned network devices to logical devices I'm listening for
		*/
		for (int i = 1; i < msg_frame->size + 1; i = i + 2) {
			device.my_listening_device_map[msg_frame->data[i] & 0xF].network_device = msg_frame->data[i - 1] << 4 & msg_frame->data[i] >> 4;
#ifdef local_debug
			avcSerial.print("Network Device: ");
			avcSerial.printHex8(device.my_listening_device_map[msg_frame->data[i] & 0xF].network_device >> 8);
			avcSerial.printHex8(device.my_listening_device_map[msg_frame->data[i] & 0xF].network_device);
			avcSerial.print(" mapped to logical device: ");
			avcSerial.printHex8(device.my_listening_device_map[msg_frame->data[i] & 0xF].logic_device);
			avcSerial.println();
#endif
		}
		break;
		// Define messages to be sent here
		// < d 1D4 190 04 000174E0
		// < d 1D4 190 04 000160E0
		// < d 1D4 1C6 04 005DE0E0
		// < d 1D4 1C6 04 005DE0E6
		// < d 1D4 1C6 04 005DE0E8

	default:

		break;

	}

}