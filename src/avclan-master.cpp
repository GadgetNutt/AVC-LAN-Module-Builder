/*
  avclan-master.cpp - AVCLAN Master Device library for Atmega328
  Created by Greg Nutt 2020-12-04
  Version 0.0.1

  Not for commercial use.
*/

//#include "../AVCLanDrv/AVCLanDrv.h"
#include "avclan-master.h"
#include "avclan-device.h"
#include "avclan-router.h"
#include "avclan-drv.h"
#include "config.h"


#ifdef STM32
#include <stm32/interrupt.h>
#endif



#include "avclan-serial.h"

//#define local_debug


extern device2address       dev2addrVector[40];
extern int                  device2addressCount = 0;
extern passthroughdevices  pass_devices[10];
extern int                  pass_device_count = 0;
uint8_t ping_count;
#define DEFAULT_MSG_DELAY 15


// AVCLan Head Unit  & timer1 init, 
void avclan_master::begin() {
    lan_start(0x00);
}

void avclan_master::update() {
    // Set up necessary timed events here


}


//> b 190 FFF 04 110120D4
void avclan_master::ping_address(uint16_t address) {
    avclan_frame_t msg_frame;

    int count = 0;
    msg_frame.master = ADDR_ME;
    msg_frame.slave = address;
    msg_frame.direct = 0;

    if (address != ADDR_BROADCAST_FFF && address != ADDR_BROADCAST_1FF) {
        msg_frame.direct = 1;
        msg_frame.data[count] = 0x00;
        ++count;
    }
    msg_frame.size = 0x04 + count;
    msg_frame.data[count] = 0x11;
    ++count;
    msg_frame.data[count] = 0x01;
    ++count;
    msg_frame.data[count] = 0x20;
    ++count;
    msg_frame.data[count] = 0xD4;
    ++count;
    msg_frame.data[count] = '\0';

    // Send message
    router.sendMessage(&msg_frame);
}



void avclan_master::processMessage(avclan_frame_t *msg_frame) {
    uint8_t devices[20];
    uint8_t message_function = messages.getFunction(msg_frame);

#ifdef local_debug
    avcSerial.print("Master processing message function: ");// Enable for debug if necessary
    avcSerial.printHex8(message_function);
    avcSerial.print(" from :");
    avcSerial.printHex8(msg_frame->master >> 8);
    avcSerial.printHex8(msg_frame->master);
    avcSerial.print(" and data size: ");
    avcSerial.printHex8(msg_frame->size);
    avcSerial.println();
#endif
    switch (message_function) {
     case MSG_DEVICES_REGISTER:
         register_devices_message_10(msg_frame);
         lan_start(0x01, msg_frame->master);

         break;

     case MSG_DEVICES_REQUEST:
         if (device2addressCount < 2) {
             avcSerial.print("Not enough devices!  Only ");
             avcSerial.printDec(device2addressCount);
             avcSerial.println();
             //  Maybe should add a lan_restart?  
             break;
         }
         /*
         for (int i = 0; i < address2deviceCount; ++i) {
             if (addr2devVector[i].address != frame.master) {
                 // Rewrite...add only when devices don't belong to master in question
             }
         }*/

         device_map_message_02(msg_frame);
        break;

     case MSG_DEVICES_BROADCAST: //  Network device broadcasting its logical device list.  Identify master 
         
         lan_start(0x03, msg_frame->master);
         //lan_start(0x00, msg_frame->master);
         lan_start(0x00);

         // TODO: Determine if AVCLAN restart needed or if a directed 00 can be sent to specific device

         avclan_frame_t frame;
         //b 190 FFF 04 01015B40
         frame.direct = 0;
         frame.master = ADDR_ME;
         frame.slave = ADDR_BROADCAST_FFF;
         frame.size = 0x04;
         frame.data[0] = 0x01;
         frame.data[1] = 0x01;
         frame.data[2] = 0x5B;
         frame.data[3] = 0x40;
         frame.data[4] = '\0';

         router.sendMessage(&frame);
         
         break;
     case PING: // Received Pong (cmd30).  Process here if necessary
         break;
     case 0x95:
     case 0x97:
         break;

     case 0xE0:
         // When 1D4 asks for E0 as below, I should answer with:
         //     ex.  < d 1D4 190 04 00 0174 E0
         //     ex.  < d 190 1D4 0D 00 6001 F0166000066000000102    This would be a radio status
         break;
    }


};

void avclan_master::lan_start(uint8_t action, uint16_t address) {
    // < b 190 FFF 03 110101
#ifdef local_debug
    avcSerial.print("lan_start sent action: ");
    avcSerial.printHex8(action);
    avcSerial.print(" to: ");
    avcSerial.printHex8(address >> 8);
    avcSerial.printHex8(address);
    avcSerial.println();
#endif
    avclan_frame_t msg_frame;

    int count = 0;
    if (action == 0x00) {
        msg10_count = 0;
    }

    msg_frame.direct = 0;
    msg_frame.master = ADDR_ME;
    msg_frame.slave = address;

    if ((address != ADDR_BROADCAST_FFF) && (address != ADDR_BROADCAST_1FF)) {
        msg_frame.direct = 1;
        msg_frame.data[count] = 0x00;
        ++count;
    }
    msg_frame.size = 0x03 + count;
    msg_frame.data[count] = 0x11;
    ++count;
    msg_frame.data[count] = 0x01;
    ++count;
    msg_frame.data[count] = action;
    ++count;
    msg_frame.data[count] = '\0';

    // Send message
    router.sendMessage(&msg_frame);
};

void avclan_master::master_heart_beat() {    //  Some kind of heart beat the master unit should send every second or so?
    //  TODO:  Dec 26, the purpose of this message is still unknown.  It doesn't appear to be needed for activating the GW and LCD modules
    //< b 190 FFF 09 A401DB 00 00 00 00 00 00
    avclan_frame_t msg_frame;

    msg_frame.direct = AVCLAN_BROADCAST_MSG;
    msg_frame.master = ADDR_ME;
    msg_frame.slave = ADDR_BROADCAST_FFF;
    msg_frame.size = 0x09;
    msg_frame.data[0] = 0xA4;
    msg_frame.data[1] = 0x01;
    msg_frame.data[2] = 0xDB;
    msg_frame.data[3] = 0x00;
    msg_frame.data[4] = 0x00;
    msg_frame.data[5] = 0x00;
    msg_frame.data[6] = 0x00;
    msg_frame.data[7] = 0x00;
    msg_frame.data[8] = 0x00;
    msg_frame.data[9] = '\0';

    router.sendMessage(&msg_frame);
}

void avclan_master::ping() {
   // b 190 FFF 04 110120D4
    avclan_frame_t msg_frame;

    msg_frame.direct = 0;
    msg_frame.master = ADDR_ME;
    msg_frame.slave = ADDR_BROADCAST_FFF;
    msg_frame.size = 0x04;
    msg_frame.data[0] = 0x11;
    msg_frame.data[1] = 0x01;
    msg_frame.data[2] = 0x20;
    if (ping_count == 0xFF) ping_count = 0;
    ++ping_count;
    msg_frame.data[3] = ping_count;
    msg_frame.data[4] = '\0';

    router.sendMessage(&msg_frame);
}

void avclan_master::printAddress2DeviceList() {
    for (int i = 0; i < device2addressCount; ++i) {
        avcSerial.print("Device: ");
        avcSerial.printHex8(dev2addrVector[i].device);
        avcSerial.print("  Address: ");
        avcSerial.printHex8(dev2addrVector[i].address >> 8);
        avcSerial.printHex8(dev2addrVector[i].address);
        avcSerial.println();
    }
    for (int i = 0; i < pass_device_count; ++i) {
        avcSerial.print("Pass Device: ");
        avcSerial.printHex8(pass_devices[i].device);
        avcSerial.print("  Address: ");
        avcSerial.printHex8(pass_devices[i].address >> 8);
        avcSerial.printHex8(pass_devices[i].address);
        avcSerial.println();
    }

}

/*  register_devices_message_10 - Used to register message type 10 in memory  */
void avclan_master::register_devices_message_10(avclan_frame_t* msg_frame) {
    bool deviceExists = false;
    bool addressExists = false;
    int count = 0;
    //int isDirect = 0;

    msg10_count++;  // Need to change this.  If it is called again after initial registration it will still increment

    //if (!msg_frame->direct) isDirect = 1;

#ifdef local_debug
    avcSerial.print("Register devices for: ");
    avcSerial.printHex8(msg_frame->master >> 8);
    avcSerial.printHex8(msg_frame->master);
    avcSerial.println();
#endif
    device2address addDeviceList[20];

    
    //for (int i = 4 - isDirect; i < msg_frame->size; ++i) {
    for (int i = 4; i < msg_frame->size; ++i) {
            addDeviceList[count].address = msg_frame->master;
            addDeviceList[count].device = msg_frame->data[i];
        ++count;
    }
    int tmpcount = device2addressCount;
    for (int j = 0; j < count; ++j) {
        addressExists = false;
        for (int i = 0; i < tmpcount; ++i) {

            if (dev2addrVector[i].device == addDeviceList[j].device) {
                addressExists = true;
#ifdef local_debug
                avcSerial.print("Found device:");
                avcSerial.printHex8(dev2addrVector[i].device);
                avcSerial.print("  Handled by address:");
                avcSerial.printHex8(dev2addrVector[i].address >> 8);
                avcSerial.printHex8(dev2addrVector[i].address);
                avcSerial.println();
#endif
                break;
            }                                                                                         \
            //if (addressExists) { break; }
        }
        if (!addressExists) {                               //  This should only happen if the new address isn't already registered.
            dev2addrVector[device2addressCount].address = addDeviceList[j].address;
            dev2addrVector[device2addressCount].device = addDeviceList[j].device;
#ifdef local_debug
            avcSerial.print("Added device:");
            avcSerial.printHex8(dev2addrVector[device2addressCount].device);
            avcSerial.print("  Handled by address:");
            avcSerial.printHex8(dev2addrVector[device2addressCount].address >> 8);
            avcSerial.printHex8(dev2addrVector[device2addressCount].address);
            avcSerial.println();
#endif
            ++device2addressCount;
        }
    }

    // Send message
    //router.sendMessage();
}

/*  Create device map from dev2addrVector in response to message type 12  */
void avclan_master::device_map_message_02(avclan_frame_t* msg_frame) {
    avclan_frame_t send_frame;
    uint8_t sourcelist[30];
    int l = 0;
    bool passcheck = false;

#ifdef local_debug
    avcSerial.println("Sending device map");
#endif
    for (int i = 0; i < device2addressCount; ++i) {     //  Cycle through address list
        if (dev2addrVector[i].address != msg_frame->master) {       //  Only get devices that don't match the incoming master
            for (int j = 4; j < msg_frame->size; ++j) {      //  Number of devices from message type 12
                passcheck = checkForPassthrough(msg_frame->data[j], msg_frame->master);
                if (msg_frame->data[j] == dev2addrVector[i].device) {
                    if(passcheck) {
                        sourcelist[l] = ADDR_ME >> 4;
                        ++l;
                        sourcelist[l] = (ADDR_ME << 4) | (j - 4);
                        ++l;
                    }
                    else {
                        sourcelist[l] = dev2addrVector[i].address >> 4;
                        ++l;
                        sourcelist[l] = (dev2addrVector[i].address << 4) | (j - 4);
                        ++l;
                    }
//                                sourcelist[l] = j;
//                                ++l;
                }
            }
        }
    }
    send_frame.direct = AVCLAN_DIRECT_MSG;
    send_frame.master = ADDR_ME;
    send_frame.slave = msg_frame->master;

    /*  Add the message header (including command bit, source device, destination device and message type)  */
    send_frame.data[0] = 0x00;
    send_frame.data[1] = DEV_COM_MASTER;
    send_frame.data[2] = DEV_COM_CTRL;
    send_frame.data[3] = 0x02;
    //avcSerial.print("Device map:");
    //avcSerial.println(send_frame.data);

    /*  Add the rest of the data */
    for (uint8_t i = 0; i < l; ++i) {
        send_frame.data[i + 4] = sourcelist[i];
    }
    send_frame.data[l + 4] = '\0';

    send_frame.size = l + 4;

    // Send message
    avclan.sendMessage(send_frame);
}

void avclan_master::status_response(uint8_t request_type) {

    switch (request_type) {
    case 0xE0:
        /*      HU Audio, CD, 
        < d 1D4 190 04 00 0160  E0
        < d 1D4 190 04 00 0163  E0
        < d 190 1D4 0D 00 6001  F0   16 60 00 06 60 00 00 01 02         Radio
        < b 190 1FF 05 3231  F1   0000                                  Info Display, Front Monitor
        < b 190 1FF 10 6331  F3   00000000000000000000000000            CD-CH
        < b 190 1FF 0D 6331  F1   00F8FFFFFFFF00800000                  CD-CH
        < b 190 1FF 05 6F31  F1   0000                                  ?
        < b 190 1FF 13 7431  F1   10281010101010000F00000308000000      Amplifier
        < b 190 1FF 0D 6031  F1   00000000000000000000                  Radio
        */

        break;

    }
}

bool avclan_master::checkForPassthrough(uint8_t check_device, uint16_t check_address) {
    /*
    for (int k = 0; k < pass_device_count; ++k) {
        if (pass_devices[k].device == check_device && pass_devices[k].address == check_address) {
            return true;
            break;
        }
    }
    */
    bool passthroughexists = false;

    for (int i = 0; i < device.listening_device_map_size; ++i) {
        if (device.my_listening_device_map[i].logic_device == check_device) {
            for (int j = 0; j < pass_device_count; ++j) {
                if (pass_devices[j].device == check_device) {
                    passthroughexists = true;
                }
            }
            if (!passthroughexists && check_device != DEV_COM_CTRL0) {
                pass_devices[pass_device_count].device = check_device;
                pass_devices[pass_device_count].address = check_address;
                ++pass_device_count;
#ifdef local_debug
                avcSerial.print("Add Passthrough Device:");
                avcSerial.printHex8(check_device);
                avcSerial.print(" for address:");
                avcSerial.printHex8(check_address >> 8);
                avcSerial.printHex8(check_address);
                avcSerial.println();
#endif
            }


            return true;
            break;
        }
    }

    return false;

}
avclan_master master;