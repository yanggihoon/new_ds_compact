

#ifndef __CMX_LIGHT_H__
#define __CMX_LIGHT_H__

// User Define Header
#include "DeviceProtocol.h"

// System Header
#include <app.h>

//CMD
#define LIGHT_STATUS_COMMAND					0x30
#define LIGHT_CTRL_COMMAND					0x31
#define LIGHT_GROUP_CTRL_COMMAND				0x3F

//ACK
#define LIGHT_STATUS_ACK						0xB0
#define LIGHT_CTRL_ACK							0xB1
#define LIGHT_GROUP_CTRL_ACK					0x00	//no ack

#define MAX_SUPPORTED_LIGHT_CNT	40
#define MAX_DIMMING_LEVEL			8

struct LightStatus
{
	unsigned int order;	
	unsigned int dimmingLevel;
	unsigned int maxDimmingLevel;
	unsigned int isAck;
	char deviceCharName[MAX_DEVICENAME_STRING_LENGTH];
	enum LIGHT_POWER power;
	enum LIGHT_MODE mode;
};


class  CMX_Light : public DeviceProtocol
{

	private :
			static CMX_Light* uniqueInstance;	
		 	unsigned char buf[8];

	protected:
			CMX_Light();
			~CMX_Light();
		
	public  :
			static CMX_Light* GetInstance();

			void DeviceInit();
			int FrameSend(unsigned char wBuf[]);
			int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
			int FrameRecv(unsigned char rBuf[]);
			int FarmeParser(unsigned char buf[]);
			unsigned int getCurrentSupportedCount();
			int checkEachAck(int order);
			int checkDisconnected();


			LightStatus	lightStatus[MAX_SUPPORTED_LIGHT_CNT];
};
#endif
