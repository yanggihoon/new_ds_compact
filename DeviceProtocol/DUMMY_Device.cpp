

#include "DUMMY_Device.h"
#include "../DeviceUart/CMX_UartRS485.h"

DUMMY_Device* DUMMY_Device::uniqueInstance = NULL;

DUMMY_Device* DUMMY_Device::GetInstance()
{
	
	if( uniqueInstance == NULL) {
		uniqueInstance = new DUMMY_Device();

	}

	return uniqueInstance;
}

void DUMMY_Device::Instance_Close()
{	
	if( uniqueInstance != NULL )
	{
		Log(LOG::PRTCL, "DUMMY_Device uniqueInstance delete\n");	
		delete uniqueInstance;
		uniqueInstance = NULL;		
	}

}

DUMMY_Device::DUMMY_Device()
{
	Log(LOG::PRTCL, "DUMMY Device Constructed\n");
	supportedPollingCount = 0;
	isAck = 0;
	isRecv = 0;
	retrySendCnt = 0;
}

DUMMY_Device::~DUMMY_Device()
{
	Log(LOG::PRTCL, "DUMMY Device Deconstructed\n");
}

void DUMMY_Device::DeviceInit()
{

}

int DUMMY_Device::FrameSend(unsigned char wBuf[])
{
	return -1;
}

int DUMMY_Device::FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	return -1;
}

int DUMMY_Device::FrameRecv(unsigned char rBuf[])
{
	return -1;
}

int DUMMY_Device::FarmeParser(unsigned char buf[])
{
	return -1;
}

int DUMMY_Device::checkEachAck(int order)
{
	return -1;
}

int DUMMY_Device::checkDisconnected()
{
	return -1;
}

unsigned int DUMMY_Device::getCurrentSupportedCount()
{
	return -1;
}
