/*
    Name:       avclan.ino
    Created:	20-12-04 8:46:15 PM
    Author:     Greg Nutt
*/

#include "src/config.h"
#include "src/avclan-drv.h"
#include "src/avclan-serial.h"
#include "src/avclan-registers.h"

#if defined AVC_MASTER
#include "src/avclan-master.h"
#endif

#ifdef AVC_DEVICE
#include "src/avclan-device.h"
#include "src/avclan-messages.h"
#endif

#include "src/avclan-router.h"

//#define local_debug

uint8_t readSeq = 0;
uint8_t s_len = 0;
uint8_t s_dig = 0;
uint8_t s_c[2];
uint8_t data_tmp[32];
uint8_t i;


// Define Functions below here or use other .ino or cpp files

bool time_started;
unsigned long StartTime;
unsigned long CurrentTime; 
bool message_timeout;
unsigned long last_message_time;
//unsigned long CurrentTime2;

void(*resetFunc) (void) = 0;//declare reset function at address 0
void CheckForMessage(int WaitPeriod) {
    StartTime = millis();
    while (CurrentTime - StartTime < WaitPeriod) {
        CurrentTime = millis();
        CheckForMessage();
    }
}

void CheckForMessage() {
    avclan_frame_t msg_frame;
    if (INPUT_IS_SET) {
        RX_LED_ON;
        uint8_t res = avclan.readMessage(&msg_frame);
        RX_LED_OFF;
        if (!res) {
            last_message_time = millis();
            switch (msg_frame.slave) {
#ifndef AVC_SNIFFER
            case ADDR_ME:        //  Define global_master in config.h              
#ifdef local_debug
                avcSerial.println("Received message for ADDR_ME");
#endif
#ifdef AVC_MASTER
                master.processMessage(&msg_frame);
#endif
#ifdef AVC_DEVICE
                device.processMessage(&msg_frame);
#endif
                break;
#endif

                /*  If broadcast messages*/
            case ADDR_BROADCAST_1FF:
            case ADDR_BROADCAST_FFF:
#ifdef local_debug
                avcSerial.println("Received broadcast message");
#endif
#ifdef AVC_MASTER
                master.processMessage(&msg_frame);
                if (msg_frame.master != ADDR_ME) {

                }
#endif
#ifdef AVC_DEVICE
                device.processMessage(&msg_frame);
                if (msg_frame.master != ADDR_ME) {

                }
#endif
                break;
            }
        }
    } else {
        CurrentTime = millis();
        if (CurrentTime - last_message_time > 120000) {
#ifdef local_debug
            avcSerial.println();
            avcSerial.println("No communication received in 2 mins. Reset");
            avcSerial.println();
            delay(100);
#endif
            resetFunc();     //  Didn't receive any communications for 2 minutes!  Restart AVCLAN
            message_timeout = true;
        }
    }
}

#ifdef AVC_MASTER
void PingTimer() {
    //  Basic timer to send heart beat once every minute
    if (!time_started) {
        StartTime = millis();
        time_started = true;
    }
    else {
        CurrentTime = millis();
        if (CurrentTime - StartTime > 60000) {
            master.ping();     // Time's up!  Send ping!
            time_started = false;
        }
    }
}
#endif

void avclan_startup() {
    
    //  AVC-LAN Start up
#ifdef AVC_MASTER
    delay(3000);
    master.lan_start(0x00);                     // Master inviting devices to advertise their logical ID's
    CheckForMessage(1000);
    master.lan_start(0x00);                     // Master inviting devices to advertise their logical ID's
    CheckForMessage(DEFAULT_MSG_DELAY);
    master.lan_start(0x01);                     // Master inviting devices to advertise their logical ID's

    CheckForMessage(20);
    if (master.msg10_count > 0) {
#ifdef local_debug
        avcSerial.println("cmd10s received, sending cmd01");
#endif
        master.lan_start(0x01);
    }else{
        avcSerial.println("AVCLAN Startup Failed!");
        resetFunc();
    }
#endif
    //master.ping_address(ADDR_BROADCAST_FFF);
    //CheckForMessage(DEFAULT_MSG_DELAY);

#ifdef AVC_MASTER
    master.lan_start(0x58);
    CheckForMessage(DEFAULT_MSG_DELAY);


    //  What is this?  It is seems to be sent with 'b 190 FFF 09 A401DB000000000000' following it.
    master.lan_start(0x46);
    CheckForMessage(DEFAULT_MSG_DELAY);
 #endif

}

void setup() {
    avcSerial.begin( 115200 );
    avcSerial.println( "AVC-Lan Start" );
#if defined AVC_MASTER
    avcSerial.println( "Mode: MASTER");
#elif defined AVC_DEVICE
    avcSerial.println( "Mode: DEVICE");
#elif defined AVC_SNIFFER
    avcSerial.println( "Mode: SNIFFER");
#endif
#if ( CRYSTAL == 3 )
    avcSerial.print( "Crystal spec: ");
    avcSerial.println( "16MHz");
#elif ( CRYSTAL == 1 )
    avcSerial.print( "Crystal spec: ");
    avcSerial.println( "8MHz");
#endif
    avcSerial.print( "TX+ : ");
    avcSerial.printDec( DATAOUT );
    avcSerial.println();
    avcSerial.print( "TX- : ");
    avcSerial.printDec( DATAIN );
    avcSerial.println();
    avcSerial.print( "AVC_NORMAL_BIT_LENGTH: " );           avcSerial.printDec( AVC_NORMAL_BIT_LENGTH );
    avcSerial.println();
    avcSerial.print( "AVC_BIT_1_HOLD_ON_LENGTH: " );        avcSerial.printDec( AVC_BIT_1_HOLD_ON_LENGTH );
    avcSerial.println();
    avcSerial.print( "AVC_BIT_0_HOLD_ON_LENGTH: " );        avcSerial.printDec( AVC_BIT_0_HOLD_ON_LENGTH );
    avcSerial.println();
    avcSerial.print( "AVC_BIT_0_HOLD_ON_MIN_LENGTH: " );    avcSerial.printDec( AVC_BIT_0_HOLD_ON_MIN_LENGTH );
    avcSerial.println();
    avcSerial.print( "AVC_START_BIT_HOLD_ON_LENGTH: " );    avcSerial.printDec( AVC_START_BIT_HOLD_ON_LENGTH );
    avcSerial.println();
    avcSerial.print( "AVC_START_BIT_HOLD_ON_MIN_LENGTH: " );avcSerial.printDec( AVC_START_BIT_HOLD_ON_MIN_LENGTH );
    avcSerial.println();
    avcSerial.print( "AVC_1U_LENGTH: " );                   avcSerial.printDec( AVC_1U_LENGTH );
    avcSerial.println();

    delay(3000);
    // Setup LED
    sbi(TX_LED_DDR, TX_LED_OUT);
    cbi(TX_LED_PORT, TX_LED_OUT);
    sbi(RX_LED_DDR, RX_LED_OUT);
    cbi(RX_LED_PORT, RX_LED_OUT);

    ENABLE_TIMER1_INT;

    avclan.begin();
    //audio_hu.begin();
    //display.begin();

    avclan_startup();

}

void loop()
{
    avclan_frame_t msg_frame;

    CheckForMessage();

#ifdef AVC_MASTER
    master.update();
#endif

#ifdef AVC_DEVICE
   device.update();    
#endif
    
#ifdef AVC_MASTER
   PingTimer();
#endif


    if (avcSerial.available()) {
        uint8_t readkey = avcSerial.read();

        switch (readkey) {
        case 'R':
            resetFunc();                //call reset
            break;
#ifdef AVC_MASTER
        case 'L':
            master.printAddress2DeviceList();
            break;
#endif
        case 'S':	                   // start command
            readSeq = 1;
            s_len = 0;
            s_dig = 0;
            s_c[0] = s_c[1] = 0;
            break;
        case 'P':
        case 'Y':                      // end of remote command
            readSeq = 1;

            // Get Message Type
            uint8_t broadcast;
            broadcast = data_tmp[0];
            if (broadcast == 0x2D)		//  If broadcast received a "d", hex value 2D then set broadcast bit to 1
                msg_frame.direct = 1;
            else if (broadcast == 0x2B)	//  If broadcast received a "b", hex value 2B then set broadcast bit to 0
                msg_frame.direct = 0;

            // Get Master Address bits
            msg_frame.master = ((data_tmp[1] << 4) + (data_tmp[2] >> 4)) & 0x0FFF;
#ifdef local_debug
            avcSerial.print("S0 msg Master: ");
            avcSerial.printHex8((data_tmp[1] << 4) + (data_tmp[2] >> 4)>>4);
            avcSerial.printHex8((data_tmp[1] << 4) + (data_tmp[2] >> 4));
            avcSerial.println();
#endif
            // Get Slave Address bits
            msg_frame.slave = ((data_tmp[2] << 8) + (data_tmp[3])) & 0x0FFF;
#ifdef local_debug
            avcSerial.print("S0 msg Slave: ");
            avcSerial.printHex8((data_tmp[2] << 8) + (data_tmp[3])>>4);
            avcSerial.printHex8((data_tmp[2] << 8) + (data_tmp[3]));
            avcSerial.println();
#endif

            //  Get DataSize bits
            msg_frame.size = (data_tmp[4]);

            // Get the rest of the message after the data size byte
            for (i = 5; i < s_len; i++)
                msg_frame.data[i - 5] = data_tmp[i];

            if (readkey == 'Y') {
                router.sendMessage(&msg_frame);
            } 
#ifdef AVC_MASTER
            else {
                master.processMessage(&msg_frame);
            }
#endif
            break;
        default:
            if (readSeq == 1) {
                if (readkey != ' ') {					//  Key was a character
                    s_c[s_dig] = readkey;				//  Read the key entered
                    s_dig++;						//  Increase the data counter
                    if (s_dig == 2) {					//	If the data counter = 2 then
                        if (s_c[0] < ':') {			//  if the first key is < ':' then subtract 48, else subtract 55
                            s_c[0] -= 48;
                        }
                        else {
                            s_c[0] -= 55;
                        }
                        data_tmp[s_len] = 16 * s_c[0];	//	???
                        if (s_c[1] < ':') {
                            s_c[1] -= 48;
                        }
                        else {
                            s_c[1] -= 55;
                        }
                        data_tmp[s_len] += s_c[1];
                        s_len++;
                        s_dig = 0;
                        s_c[0] = s_c[1] = 0;
                    }
                }
            }
        }
    }
}

ISR(TIMER1_OVF_vect) {
    TCNT1H = TI1_H; // Load counter value hi
    TCNT1L = TI1_L;	// Load counter value lo
    /*
    avclanDevice.cd_sec = avclanDevice.hexInc(avclanDevice.cd_sec);
    if (avclanDevice.cd_sec == 0x60) {
        avclanDevice.cd_sec = 0;
        avclanDevice.cd_min = avclanDevice.hexInc(avclanDevice.cd_min);
        if (avclanDevice.cd_min == 0xA0) {
            avclanDevice.cd_min = 0x0;
        }
    }
    avclan.event = EV_STATUS;
    */


    /*  Trigger broadcast of timed interval status request messages for all devices
    */

}