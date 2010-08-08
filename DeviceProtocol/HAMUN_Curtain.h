

#ifndef __HAMUN_Curtain_H__
#define __HAMUN_Curtain_H__

// User Define Header
#include "DeviceProtocol.h"

// System Header
#include <app.h>

//CMD
#define CURTAIN_CTRL_COMMAND						0x7E

//ACK
#define CURTAIN_CTRL_ACK							0x7D

#define MAX_SUPPORTED_CURTAIN_CNT				8

struct CurtainStatus
{
	unsigned int order;	
	unsigned int isAck;
	char deviceCharName[MAX_DEVICENAME_STRING_LENGTH];
	unsigned int power;
	enum CURTAIN_ACTION action;
};


class  HAMUN_Curtain : public DeviceProtocol
{

	private :
			static HAMUN_Curtain* uniqueInstance;	
		 	unsigned char buf[7];
		
	public  :
			static HAMUN_Curtain* GetInstance();
			HAMUN_Curtain();
			~HAMUN_Curtain();

			void DeviceInit();
			int FrameSend(unsigned char wBuf[]);
			int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
			int FrameRecv(unsigned char rBuf[]);
			int FarmeParser(unsigned char buf[]);
			unsigned int getCurrentSupportedCount();
			int checkEachAck(int order);
			int checkDisconnected();

			unsigned int isStartPolled;
			CurtainStatus	curtainStatus[MAX_SUPPORTED_CURTAIN_CNT];
};
#endif
