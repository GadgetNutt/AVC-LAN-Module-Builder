

#include "avclan-router.h"
#include "avclan-includes.h"
#include "avclan-serial.h"


void avclan_router::sendMessage(avclan_frame_t* msg_frame) {
	bool processed = 0;

#ifdef AVC_MASTER
	if (msg_frame->slave == ADDR_BROADCAST_1FF || msg_frame->slave == ADDR_BROADCAST_FFF || msg_frame->slave == ADDR_ME)
		{
#ifdef local_debug
		sendMessageDebugPrint("MASTER", ADDR_ME, msg_frame);
#endif
		master.processMessage(msg_frame);
		processed = 1;
	}
#endif

	if (msg_frame->slave == ADDR_BROADCAST_1FF || msg_frame->slave == ADDR_BROADCAST_FFF || msg_frame->slave == ADDR_ME)
		{
#ifdef local_debug
		sendMessageDebugPrint("DEVICE", ADDR_ME, msg_frame);
#endif
		device.processMessage(msg_frame);
		processed = 1;
	}

	//	IF a broadcast message or was not processed above, then we continue to send the message to the AVCLAN
	if (!processed || msg_frame->slave == ADDR_BROADCAST_1FF || msg_frame->slave == ADDR_BROADCAST_FFF) {
		avclan.sendMessage(msg_frame);
	}
	else {
		// TODO: Should this be enabled?
		avclan.printMessage(msg_frame, 2);
	}
}

void avclan_router::sendMessageDebugPrint(char* source, uint16_t master, avclan_frame_t* msg_frame) {
	avcSerial.print(source);
	avcSerial.print(" process local msg: ");
	if (msg_frame->direct) {
		avcSerial.printHex8(msg_frame->data[3]);
	}
	else {
		avcSerial.printHex8(msg_frame->data[2]);
	}
	avcSerial.print(" from: ");
	avcSerial.printHex8(msg_frame->master >> 8);
	avcSerial.printHex8(msg_frame->master);
	avcSerial.print(" by: ");
	avcSerial.printHex8(ADDR_ME >> 8);
	avcSerial.printHex8(ADDR_ME);
	avcSerial.print(" to: ");
	avcSerial.printHex8(msg_frame->slave >> 8);
	avcSerial.printHex8(msg_frame->slave);
	avcSerial.println();
}
avclan_router router;