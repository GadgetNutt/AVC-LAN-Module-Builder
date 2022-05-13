#pragma once

#ifndef avclan_router_h
#define avclan_router_h

#include "avclan-messages.h"

class avclan_router
{
public:
	void		sendMessage(avclan_frame_t* msg_frame);
private:
	void		sendMessageDebugPrint(char* source, uint16_t master, avclan_frame_t* msg_frame);
};

extern avclan_router router;

#endif