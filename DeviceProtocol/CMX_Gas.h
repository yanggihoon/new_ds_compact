

#ifndef __CMX_GAS_H__
#define __CMX_GAS_H__

// User Define Header
#include "DeviceProtocol.h"

// System Header
#include <app.h>

//CMD
#define GAS_STATUS_COMMAND					0x10
#define GAS_CTRL_COMMAND						0x11

//ACK
#define GAS_STATUS_ACK							0x90
#define GAS_CTRL_ACK							0x91

#define MAX_SUPPORTED_GAS_CNT						2

struct GasStatus
{
	unsigned int order;	
	unsigned int isAck;
	char deviceCharName[MAX_DEVICENAME_STRING_LENGTH];
	enum GAS_ACTION action;
};


class  CMX_Gas : public DeviceProtocol
{

	private :
			static CMX_Gas* uniqueInstance;	
		 	unsigned char buf[8];
		
	public  :
			static CMX_Gas* GetInstance();
			CMX_Gas();
			~CMX_Gas();

			void DeviceInit();
			int FrameSend(unsigned char wBuf[]);
			int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
			int FrameRecv(unsigned char rBuf[]);
			int FarmeParser(unsigned char buf[]);
			unsigned int getCurrentSupportedCount();
			int checkEachAck(int order);
			int checkDisconnected();


			GasStatus	gasStatus[MAX_SUPPORTED_GAS_CNT];
};
#endif
