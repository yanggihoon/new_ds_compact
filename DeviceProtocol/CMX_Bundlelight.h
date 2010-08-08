

#ifndef __CMX_BUNDLELIGHT_H__
#define __CMX_BUNDLELIGHT_H__

// User Define Header
#include "DeviceProtocol.h"

// System Header
#include <app.h>

//CMD
#define BUNDLELIGHT_STATUS_COMMAND						0x20
#define OLD_BUNDLELIGHT_CTRL_COMMAND					0x21
#define NEW_BUNDLELIGHT_CTRL_COMMAND					0x22
#define BUNDLELIGHT_OUTMODE_COMMAND						0x2E
#define BUNDLELIGHT_GROUP_POWER_COMMAND				0x2F
#define BUNDLELIGHT_GROUP_READYPOWER_COMMAND			0x2D

//ACK
#define BUNDLELIGHT_STATUS_ACK						0xA0
#define OLD_BUNDLELIGHT_CTRL_ACK						0xA1
#define NEW_BUNDLELIGHT_CTRL_ACK						0xA2
#define BUNDLELIGHT_OUTMODE_ACK						0x00	//no ack
#define BUNDLELIGHT_LIGHTPOWER_ACK					0x00	//no ack
#define BUNDLELIGHT_READYENERGY_ACK					0x00	//no ack

#define MAX_SUPPORTED_BUNDLELIGHT_CNT				2

struct BundlelightStatus
{
	unsigned int order;	
	unsigned int isAck;
	char deviceCharName[MAX_DEVICENAME_STRING_LENGTH];
	enum BUNDLELIGHT_POWER 				power;
	enum BUNDLELIGHT_READYPOWER			readyPower;
	enum BUNDLELIGHT_ELEVATORCALL 			elevatorCall;
	enum BUNDLELIGHT_OUT					out;
	enum BUNDLELIGHT_GASCLOSE				gasClose;
	struct BUNDLELIGHT_SUPPORT_FUNCTION 	supported_function;
};


class  CMX_Bundlelight : public DeviceProtocol
{

	private :
			static CMX_Bundlelight* uniqueInstance;	
		 	unsigned char buf[8];
		
	public  :
			static CMX_Bundlelight* GetInstance();
			CMX_Bundlelight();
			~CMX_Bundlelight();

			void DeviceInit();
			int FrameSend(unsigned char wBuf[]);
			int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
			int FrameRecv(unsigned char rBuf[]);
			int FarmeParser(unsigned char buf[]);
			unsigned int getCurrentSupportedCount();
			int checkEachAck(int order);
			int checkDisconnected();

			BundlelightStatus	bundlelightStatus[MAX_SUPPORTED_BUNDLELIGHT_CNT];
			
};
#endif
