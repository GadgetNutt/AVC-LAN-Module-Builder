#pragma once

#ifndef avclan_includes_h
#define avclan_includes_h

#include "avclan-messages.h"
#include "avclan-responses.h"

#include "avclan-drv.h"
#ifdef AVC_MASTER
#include "avclan-master.h"
#endif
#ifdef AVC_DEVICE
#include "avclan-device.h"
#endif

#endif
