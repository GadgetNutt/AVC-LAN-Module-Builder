/*
  avclan-device.h - AVCLan Multi-display library for avclan interface for Atmega328
  Created by Greg Nutt 2020-12-15
  Version 0.0.1
  
  Not for commercial use.
*/
#include "avclan-messages.h"
#include "avclan-registers.h"
#include <avr/pgmspace.h>

#pragma once

#ifndef avclan_device_h
#define avclan_device_h

//uint8_t data[] = { 0x00, 0x01, 0x11, 0x30, 0x00, 0xFF };


class avclan_device {
public:
	void				begin();
	void				update();
	void				processMessage(avclan_frame_t *msg_frame);
	uint16_t			master_device;

	/*	Add your list of logical devices that you want to provide.
	* 
	*	You can combine logical devices that may have originally been on different network devices.
	* 
	*	Devices are defined in avclan-registers.h under "Device Register Values"
	*
	*	REMEMBER TO SET THE CORRECT ARRAY COUNT!!
	*
	*	ie:
	* 
	* 	uint8_t my_devices[5] = { 
	*	DEV_INFO_DISPLAY2,
	*	DEV_3F, 
	*	DEV_BLUETOOTH,
	*	DEV_CLIMATE_CTRL_DRAW,
	*	DEV_TRIP_INFO_DRAW };
	*
	*/
	//	{ 0x32, 0x3F, 0x55, 0x5D, 0x5F }
	uint8_t my_devices[6] = { 
		DEV_COM_CTRL0,
		DEV_INFO_DISPLAY2,
		DEV_3F, 
		DEV_BLUETOOTH,
		DEV_CLIMATE_CTRL_DRAW,
		DEV_TRIP_INFO_DRAW };

	/*	Add your list of logical devices that you want to receive data from.
	*	Devices are defined in avclan-registers.h under "Device Register Values"
	* 
	*	You can combine logical devices that may have originally been on different network devices.
	*
	*	REMEMBER TO SET THE CORRECT ARRAY COUNT!!
	*
	*	ie:
	*
	*	avclan_logical_device_map_t my_listening_device_map[13]  = {
	*		{0,DEV_COM_CTRL0},
	*		{0,DEV_BEEP_SPEAK},
	*		{0,DEV_CD},
	*		{0,DEV_RADIO},
	*		{0,DEV_CASSETTE},
	*		{0,DEV_CD_P},
	*		{0,DEV_CD_CH},
	*		{0,DEV_6D},
	*		{0,DEV_6F},
	*		{0,DEV_AUDIO_AMP},
	*		{0,DEV_XM_TUNER},
	*		{0,DEV_CLIMATE_CTRL_DEV},
	*		{0,DEV_TRIP_INFO}
	*	};
	*
	*/
	//	{ 0x32, 0x3F, 0x55, 0x5D, 0x5F }
	//			{0,0x00},{0,0x29},{0,0x43},{0,0x60},{0,0x61},{0,0x62},{0,0x63},{0,0x6D},{0,0x6F},{0,0x74},{0,0xC0},{0,0xE0},{0,0xE5}};
	static const int listening_device_map_size = 13;
	avclan_logical_device_map_t my_listening_device_map[listening_device_map_size]  = {
		{0,DEV_COM_CTRL0},
		{0,DEV_BEEP_SPEAK},
		{0,DEV_CD},
		{0,DEV_RADIO},
		{0,DEV_CASSETTE},
		{0,DEV_CD_P},
		{0,DEV_CD_CH},
		{0,DEV_6D},
		{0,DEV_6F},
		{0,DEV_AUDIO_AMP},
		{0,DEV_XM_TUNER},
		{0,DEV_CLIMATE_CTRL_DEV},
		{0,DEV_TRIP_INFO}
	};

private:
	void				cmdMessage(uint8_t cmd, uint16_t slave, uint8_t src_dev, uint8_t dest_dev);
#ifdef AVC_MASTER
	void				checkForPassthrough(uint8_t check_device, avclan_frame_t* msg_frame);
#endif
};

extern avclan_device device;
#endif
