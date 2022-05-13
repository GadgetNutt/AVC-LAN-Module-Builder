/*
  avclan-responses.h - AVCLan Messages library for avclan interface for Atmega328
  Created by Greg Nutt 2020-12-26
  Version 0.0.1
*/

#pragma once

#include <stdint.h>
#include "avclan-includes.h"

#ifndef avclan_responses_h
#define avclan_responses_h


class avclan_response {
public:
	void			respond00(avclan_frame_t* msg_frame);
	void			respond01(avclan_frame_t* msg_frame);
	void			respond02(avclan_frame_t* msg_frame);
	void			respond03(avclan_frame_t* msg_frame);
	void			respond20(avclan_frame_t* msg_frame);

};

extern avclan_response respond;

#endif