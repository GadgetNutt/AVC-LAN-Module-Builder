/*
  avclan-device.cpp - AVCLan Serial library for avclan interface for Atmega328
  Created by Greg Nutt 2020-12-15
  Version 0.0.1

  Not for commercial use.
*/

#include <stdio.h>
#include <string.h>

#ifdef AVC_MASTER
#include "avclan-messages.h"
#endif

#include "avclan-includes.h"
#include "avclan-drv.h"
#include "avclan-router.h"
#include "avclan-serial.h"


void avclan_device::begin() {

	// I don't know who the master is.  Broadcast my devices (CMD13)
	//  TODO: This needs to be repeated at an interval until the master responds
	messages.cmd13(my_devices, sizeof(my_devices) / sizeof(my_devices[0]), ADDR_ME, 0x01, 0x11);

}

void avclan_device::update() {
	// Set up necessary timed events here
	if ((ADDR_ME != ADDR_ME) && (device.master_device == 0x00)) {
#ifdef local_debug
		avcSerial.println("INFO_DISPLAY Sending cmd13");
#endif
		messages.cmd13(device.my_devices, sizeof(device.my_devices) / sizeof(device.my_devices[0]), ADDR_ME, 0x01, 0x11);
	}
}

// TODO:	Intended to replace older version:
//			void avclan_device::processMessage(avclan_frame_t *msg_frame)
void avclan_device::processMessage(avclan_frame_t* msg_frame, bool bitswitch)
{
	//avclan_message_command_bit
	uint8_t devices[20];
	uint8_t src_dev = messages.getSrcDevice(msg_frame);
	uint8_t dest_dev = messages.getDestDevice(msg_frame);	// TODO: start by device, then by function?
	uint8_t message_function = messages.getFunction(msg_frame);
#ifdef local_debug
	avcSerial.print("Device bit processing message function: ");// Enable for debug if necessary
	avcSerial.printHex8(message_function);
	avcSerial.print(" from :");
	avcSerial.printHex8(msg_frame->master >> 8);
	avcSerial.printHex8(msg_frame->master);
	avcSerial.print(" dest device: ");
	avcSerial.printHex8(dest_dev);
	avcSerial.println();
#endif

	if ((message_function && CMD_BIT_STATUS))
	{

	}else{

		if ((message_function && CMD_BIT_MASTER))
		{
			//  TODO:  Message came from AVCLAN master device.  Get master address while we are at it
		}
		if ((message_function && CMD_BIT_PING))
		{
			if (message_function && CMD_BIT_REGISTER)
			{
			}  
			else
			{
				/*  Received Ping from network device
					Respond to ping with cmd30
				*/
				respond.respond20(msg_frame);
#ifdef AVC_MASTER
				checkForPassthrough(dest_dev, msg_frame);
#endif
			}
		}
		if (message_function && CMD_BIT_REGISTER)
		{
		}
		else
		{
			if (message_function && CMD_BIT_INIT)
			{
				/*  Logical device lists received by master.
					Time to send the logical device list I want to listen to
				*/
				respond.respond01(msg_frame);
			}
			else
			{
				//  AVCLAN Init
				respond.respond00(msg_frame);
			}

			if (message_function && CMD_BIT_DEVICE)
			{
				/*  My device assignments from the master network device
					Master device's response to my cmd12
					Map assigned network devices to logical devices I'm listening for
				*/
				respond.respond02(msg_frame);
			}

			if ((message_function && CMD_BIT_INIT) && (message_function && CMD_BIT_DEVICE))
			{
				/*  Response to my cmd13, master device identifying itself.
					Save to my master_device class variable
				*/
				respond.respond03(msg_frame);
			}
		}
	}

}

//  TODO:   Soon to be deprecated... being replaced by: 
//			void avclan_device::processMessage(avclan_frame_t* msg_frame, bool bitswitch)
void avclan_device::processMessage(avclan_frame_t *msg_frame)
{
	uint8_t devices[20];
	uint8_t src_dev = messages.getSrcDevice(msg_frame);
	uint8_t dest_dev = messages.getDestDevice(msg_frame);	// TODO: start by device, then by function?
	uint8_t message_function = messages.getFunction(msg_frame);
#ifdef local_debug
	avcSerial.print("Device processing message function: ");// Enable for debug if necessary
	avcSerial.printHex8(message_function);
	avcSerial.print(" from :");
	avcSerial.printHex8(msg_frame->master >> 8);
	avcSerial.printHex8(msg_frame->master);
	avcSerial.print(" dest device: ");
	avcSerial.printHex8(dest_dev);
	avcSerial.println();
#endif
	switch (message_function) {
	case 0x00:		
		//  AVCLAN Init
		respond.respond00(msg_frame);
		break;

	case 0x01:		
		/*  Logical device lists received by master.  
			Time to send the logical device list I want to listen to
		*/
		respond.respond01(msg_frame);
		break;

	case 0x02:		
		/*  My device assignments from the master network device
			Master device's response to my cmd12
		    Map assigned network devices to logical devices I'm listening for
		*/
		respond.respond02(msg_frame);
		break;

	case 0x03:		
		/*  Response to my cmd13, master device identifying itself.
			Save to my master_device class variable
		*/
		respond.respond03(msg_frame);
		break;

	case 0x20:		
		/*  Received Ping from network device
		    Respond to ping with cmd30
		*/
		respond.respond20(msg_frame);
#ifdef AVC_MASTER
		checkForPassthrough(dest_dev, msg_frame);
#endif
		break;

	case 0x45:
		respond.respond02(msg_frame);
		break;

	case 0x46:
		respond.respond02(msg_frame);
		break;


	case 0xF0:
		//  Additional logic can be added when needed
#ifdef AVC_MASTER
		//checkForPassthrough(dest_dev, msg_frame);
#endif
		break;

	default:
#ifdef AVC_MASTER
		//checkForPassthrough(dest_dev, msg_frame);
#endif
		break;
	
	}
}

#ifdef AVC_MASTER
//  Check to see if the message needs to be resent to another network device that shares one of my logical devices
void avclan_device::checkForPassthrough(uint8_t check_device, avclan_frame_t* msg_frame) {
	avclan_frame_t tmp_frame;

	for (int i = 0; i < master.pass_device_count; ++i) {
		//if (master.pass_devices[i].device == check_device && master.pass_devices[i].address != msg_frame->slave) {
			memcpy(&tmp_frame, msg_frame, sizeof(msg_frame));
#ifdef local_debug
			avcSerial.print("Checking memcpy :");
			avcSerial.printHex8(tmp_frame.master >> 8);
			avcSerial.printHex8(tmp_frame.master);
			avcSerial.println();
#endif

			tmp_frame.master = ADDR_ME;
			tmp_frame.slave = master.pass_devices[i].address;
			router.sendMessage(&tmp_frame);
		//}
	}
}
#endif

void avclan_device::cmdMessage(uint8_t cmd, uint16_t slave, uint8_t src_dev, uint8_t dest_dev) {
	int count = 0;

	switch (cmd) {

	case 0xE0:
		//	I want a status update from this device
		// Status < d 1D4 1C6 04 00 5DE0 E0
		// TODO: Lookup owner of logical device DEV_CLIMATE_CTRL_DEV and place in SLAVE position below instead of ADDR_GW_ECU)
		messages.cmdE(
			ADDR_ME,
			ADDR_GW_ECU,
			DEV_CLIMATE_CTRL_DRAW,
			DEV_CLIMATE_CTRL_DEV,
			0xE0);


		//	Status < d 1D4 190 04 000174E0
		for (int i = 0; i < (sizeof(my_listening_device_map) / sizeof(avclan_logical_device_map_t)); i++) {
			if (my_listening_device_map[i].logic_device == DEV_AUDIO_AMP) {
				messages.cmdE(
					ADDR_ME,
					my_listening_device_map[i].network_device,
					DEV_COM_CTRL,
					DEV_AUDIO_AMP,
					0xE0);
			}
			else {
#ifdef local_debug
				avcSerial.println("E0 Network device not found in map.");
#endif
			}
		}

		//	Status < d 1D4 190 04 000160E0
		// TODO: Lookup owner of logical device DEV_RADIO and place in SLAVE position below instead of master_device)
		messages.cmdE(
			ADDR_ME,
			master_device,
			DEV_COM_CTRL,
			DEV_RADIO,
			0xE0);


		//	Status < d 1D4 190 04 000163E0
		// TODO: Lookup owner of logical device DEV_CD_CH and place in SLAVE position below instead of master_device)
		messages.cmdE(
			ADDR_ME,
			master_device,
			DEV_COM_CTRL,
			DEV_CD_CH,
			0xE0);

		break;

	case 0xE2:
		//	I want a status update from this device
		// Status 1 < d 1D4 190 04 003174E2
		// TODO: Lookup owner of logical device DEV_AUDIO_AMP and place in SLAVE position below instead of ADDR_GW_ECU)
		messages.cmdE(
			ADDR_ME,
			ADDR_GW_ECU,
			DEV_INFO_DISPLAY1,
			DEV_AUDIO_AMP,
			0xE2);
		break;

	case 0xE4:
		//	I want a status updates from this device
		//	GW won't start sending data until this asks for it (?)

		// Status 1 < d 1D4 1C6 04 00 5DE0 E4
		// TODO: Lookup owner of logical device DEV_CLIMATE_CTRL_DEV and place in SLAVE position below instead of ADDR_GW_ECU)
		messages.cmdE(
			ADDR_ME,
			ADDR_GW_ECU,
			DEV_CLIMATE_CTRL_DRAW,
			DEV_CLIMATE_CTRL_DEV,
			0xE4);

		// Status 2 < d 1D4 190 04 003163E4
		// TODO: Lookup owner of logical device DEV_CD_CH and place in SLAVE position below instead of ADDR_GW_ECU)
		messages.cmdE(
			ADDR_ME,
			ADDR_GW_ECU,
			DEV_INFO_DISPLAY1,
			DEV_CD_CH,
			0xE4);
		break;


	case 0xE6:
		//	I want a status updates from this device
		//	GW won't start sending data until this asks for it (?)

		// Status 1 < d 1D4 1C6 04 00 5DE0 E6
		// TODO: Lookup owner of logical device DEV_CLIMATE_CTRL_DEV and place in SLAVE position below instead of ADDR_GW_ECU)
		messages.cmdE(
			ADDR_ME,
			ADDR_GW_ECU,
			DEV_CLIMATE_CTRL_DRAW,
			DEV_CLIMATE_CTRL_DEV,
			0xE6);
		break;

	case 0xE8:
		//	I want a status updates from this device
		//	GW won't start sending data until this asks for it (?)

		// Status 1 < d 1D4 1C6 04 00 5DE0 E8
		// TODO: Lookup owner of logical device DEV_CLIMATE_CTRL_DEV and place in SLAVE position below instead of ADDR_GW_ECU)
		messages.cmdE(
			ADDR_ME,
			ADDR_GW_ECU,
			DEV_CLIMATE_CTRL_DRAW,
			DEV_CLIMATE_CTRL_DEV,
			0xE8);
		break;
	}
}

avclan_device device;