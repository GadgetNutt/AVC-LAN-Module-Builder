/*
  avclan-registers.h - AVCLan Head Unit library for for Atmega328
  Created by Greg Nutt, 010.30.2020
  Version 0.0.1
  
  Not for commercial use.  
*/


#ifndef avclan_registers_h
#define avclan_registers_h

#define FRAME_MAXLEN			32;

// Address Register Values
const uint16_t ADDR_AUDIO_ECU			=	0x0180;
const uint16_t ADDR_AUDIO_HU			=	0x0190;
const uint16_t ADDR_DVD_P				=	0x01A0;
const uint16_t ADDR_REAR_TV				=	0x01B0;
const uint16_t ADDR_SINGLE_DIN_NAV		=	0x01B4;
const uint16_t ADDR_DISPLAY_SW			=	0x01B8;
const uint16_t ADDR_REAR_CTRL_SW		=	0x01C0;
const uint16_t ADDR_EURO_GW_ECU			=	0x01C2;
const uint16_t ADDR_RUSSIA_GW_ECU		=	0X01C4;
const uint16_t ADDR_GW_ECU				=	0x01C6;
const uint16_t ADDR_FM_MULTI_DISPLAY	=	0x01C8;
const uint16_t ADDR_STEERING_SW			=	0x01CC;
const uint16_t ADDR_MULTI_CD_DECODER	=	0x01D0;
const uint16_t ADDR_DISPLAY_MASTER		=   0x01D2;			// Entry for custom device
const uint16_t ADDR_DISPLAY				=	0x01D4;
const uint16_t ADDR_CLOCK				=	0x01D6;
const uint16_t ADDR_FR_CONTROLLED_SW	=	0x01D8;
const uint16_t ADDR_NAV_REM_CTRL		=	0x01DC;
const uint16_t ADDR_CD_CH_COMMANDER		=	0x01E0;
const uint16_t ADDR_CONSOLIDATED_SW		=	0x01E4;
const uint16_t ADDR_MD_CH_COMMANDER		=	0x01E8;
const uint16_t ADDR_BODY_COMPUTER		=	0x01EC;
const uint16_t ADDR_AMP_RADIO_TUNER		=	0x01F0;
const uint16_t ADDR_XM_RADIO_TUNER		=	0x01F2;
const uint16_t ADDR_RSA					=	0x01F4;
const uint16_t ADDR_RSE_M				=	0x01F6;
const uint16_t ADDR_BROADCAST_1FF		=	0x01FF;
const uint16_t ADDR_NAV_ECU				=	0x0200;
const uint16_t ADDR_ATIS				=	0x0210;
const uint16_t ADDR_VICS				=	0x0220;
const uint16_t ADDR_TV_TUNER			=	0x0230;
const uint16_t ADDR_HW_CD_CH			=	0x0240;
const uint16_t ADDR_HW_DVD_CH			=	0x0250;
const uint16_t ADDR_TEL_INFO_ECU		=	0x0260;
const uint16_t ADDR_CAMERA_CTRLR		=	0x0280;
const uint16_t ADDR_RADIO				=	0x0300;
const uint16_t ADDR_CASSETTE			=	0x0320;
const uint16_t ADDR_CASSETTE_NO_CH		=	0x0330;
const uint16_t ADDR_CD_P				=	0x0340;
const uint16_t ADDR_1DIN_CD_CH			=	0x0360;
const uint16_t ADDR_MD_P				=	0x0380;
const uint16_t ADDR_MD_CH				=	0x03A0;
const uint16_t ADDR_DAT					=	0x03C0;
const uint16_t ADDR_DCC					=	0x03E0;
const uint16_t ADDR_TEL_ECU				=	0x03F8;
const uint16_t ADDR_EQUALIZER			=	0x0400;
const uint16_t ADDR_DSP					=	0x0440;
const uint16_t ADDR_HW_AMP				=	0x0480;
const uint16_t ADDR_GPS_RECEIVER		=	0x0500;
const uint16_t ADDR_ATIS_DECODER		=	0x0510;
const uint16_t ADDR_FM_MULTI_DECODER	=	0x0520;
const uint16_t ADDR_RADIO_WAVE_BEACON	=	0x0528;
const uint16_t ADDR_OPTICAL_BEACON		=	0x052C;
const uint16_t ADDR_CD_CH				=	0x0540;
const uint16_t ADDR_MD_CH_2				=	0x0560;
const uint16_t ADDR_CDROM_CH			=	0x0580;
const uint16_t ADDR_MDROM_CH			=	0x05A0;
const uint16_t ADDR_TEL_INFO			=	0x05C0;
const uint16_t ADDR_MAYDAY				=	0x05C8;
const uint16_t ADDR_AC_ECU				=	0x0600;
const uint16_t ADDR_BODY_ECU			=	0x0680;
const uint16_t ADDR_BROADCAST_FFF		=	0x0FFF;

// Device Register Values
const uint16_t DEV_COM_CTRL0			=	0x00;
const uint16_t DEV_COM_CTRL				=	0x01;
const uint16_t DEV_COM_EXT				=	0x02;
const uint16_t DEV_COM_MASTER			=	0x11;
const uint16_t DEV_SW_AUDIO				=	0x21;
const uint16_t DEV_SW_SHIFT				=	0x23;
const uint16_t DEV_SW					=	0x25;
const uint16_t DEV_HU_BEEP				=	0x28;
const uint16_t DEV_BEEP_SPEAK			=	0x29;
const uint16_t DEV_INFO_DISPLAY1		=	0x31;
const uint16_t DEV_INFO_DISPLAY2		=	0x32;
const uint16_t DEV_INFO_DISPLAY3		=	0x34;
const uint16_t DEV_3F					=	0x3F;
const uint16_t DEV_TV_TUNER				=	0x40;
const uint16_t DEV_CD					=	0x43;
const uint16_t DEV_BLUETOOTH			=	0x55;
const uint16_t DEV_INFO_DRAW			=	0x56;
const uint16_t DEV_NAV_GPS				=	0x58;
const uint16_t DEV_FM_MULTIPLEX_VICS	=	0x5A;
const uint16_t DEV_BEACON				=	0x5B;
const uint16_t DEV_CAMERA				=	0x5C;
const uint16_t DEV_CLIMATE_CTRL_DRAW	=	0x5D;
const uint16_t DEV_AUDIO_DRAW			=	0x5E;
const uint16_t DEV_TRIP_INFO_DRAW		=	0x5F;
const uint16_t DEV_RADIO				=	0x60;
const uint16_t DEV_CASSETTE				=	0x61;
const uint16_t DEV_CD_P					=	0x62;
const uint16_t DEV_CD_CH				=	0x63;
const uint16_t DEV_MD					=	0x64;
const uint16_t DEV_MD_CH				=	0x65;
const uint16_t DEV_6D					=	0x6D;
const uint16_t DEV_6F					=	0x6F;
const uint16_t DEV_AUDIO_AMP			=	0x74;
const uint16_t DEV_NAV_GPS_2			=	0x80;
const uint16_t DEV_81					=	0x81;
const uint16_t DEV_FM_MULTIPLEX_DATA	=	0x82;
const uint16_t DEV_OPTICAL_BEACON		=	0x83;
const uint16_t DEV_RADIO_WAVE_BEACON	=	0x84;
const uint16_t DEV_VOICE_CONTROL		=	0x85;
const uint16_t DEV_FM_MULTIPLEX_TUNER	=	0x9A;
const uint16_t DEV_A4					=	0xA4;
const uint16_t DEV_XM_TUNER				=	0xC0;
const uint16_t DEV_CLIMATE_CTRL_DEV		=	0xE0;
const uint16_t DEV_E4					=	0xE4;
const uint16_t DEV_TRIP_INFO			=	0xE5;
const uint16_t DEV_EF					=	0xEF;

// Function Register Values
const uint16_t FUNC_DIRECT_COMMAND		=	0x00;
const uint16_t FUNC_LAN_INIT			=	0x01;
const uint16_t MSG_DEVICES_RESPONSE		=	0x02;
const uint16_t MSG_DEVICES_REGISTER		=	0x10;
const uint16_t FUNC_REGISTER			=	0x11;		//  TODO:  Confirm
const uint16_t MSG_DEVICES_REQUEST		=	0x12;
const uint16_t MSG_DEVICES_BROADCAST	=	0x13;
const uint16_t PING						=	0x20;
const uint16_t PONG						=	0x30;
const uint16_t FUNC_REGISTER_58			=	0x58;
const uint16_t FUNC_REGISTER_5B			=	0x5B;
const uint16_t FUNC_REGISTER_5F			=	0x5F;
const uint16_t FUNC_STATUS_B7			=	0xB7;
const uint16_t FUNC_UNKNOWN_INFO_D9		=	0xD9;		// TODO: Related to Info Display, try to identify
const uint16_t FUNC_AVG_KMH_INFO		=	0xDB;
const uint16_t FUNC_UNKNOWN_INFO_DC		=	0xDC;
const uint16_t FUNC_FUEL_RANGE_INFO		=	0xDD;
const uint16_t FUNC_TRIP_TIME_INFO		=	0xDE;
const uint16_t FUNC_STATUS_REQUEST_E0	=	0xE0;
const uint16_t FUNC_STATUS_REQUEST_E2	=	0xE2;
const uint16_t FUNC_STATUS_REQUEST_E4	=	0xE4;
const uint16_t FUNC_STATUS_RESPOND_F0	=	0xF0;
const uint16_t FUNC_STATUS_RESPOND_F1	=	0xF1;
const uint16_t FUNC_STATUS_RESPOND_F2	=	0xF2;
const uint16_t FUNC_STATUS_RESPOND_F3	=	0xF3;
const uint16_t FUNC_STATUS_RESPOND_F9	=	0xF9;

#endif
