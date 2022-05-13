/*
  avclan-master.h - AVCLan Master Device library for Atmega328
  Created by Greg Nutt 2020-12-04
  Version 0.0.1
  
  Not for commercial use.
*/

#include <avr/pgmspace.h>
#include "avclan-drv.h"

#ifndef avclan_master_h
#define avclan_master_h

//const AVC_MAXMSGLEN 32;
#define AVCLANDEVICE_NAME    " Master Unit"
#define AVCLANDEVICE_VERSION "0.0.1"

typedef struct {
	uint8_t		device;
	uint16_t	address;
} device2address;

typedef struct {
	uint8_t		device;
	uint16_t	address;
} passthroughdevices;

class avclan_master {

public:
	void				begin();
	void				update();
	void				lan_start(uint8_t action, uint16_t address = ADDR_BROADCAST_FFF);
	void				register_devices_message_10(avclan_frame_t* msg_frame);
	void				device_map_message_02(avclan_frame_t* msg_frame);
	void				processMessage(avclan_frame_t *msg_frame);
	void				master_heart_beat();
	void				printAddress2DeviceList();
	void				ping_address(uint16_t address);
	void				ping();
	uint8_t				msg10_count;
	bool				checkForPassthrough(uint8_t device, uint16_t address);
	passthroughdevices	pass_devices[10];
	int                 pass_device_count;
private:
	void				status_response(uint8_t request_type);
	device2address		dev2addrVector[40];
	int					device2addressCount;
};

extern avclan_master master;
#endif
