

#ifndef __DUMMY_DEVICE_H__
#define __DUMMY_DEVICE_H__

// User Define Header
#include "DeviceProtocol.h"

// System Header
#include <app.h>


class  DUMMY_Device : public DeviceProtocol
{

	private :
			static DUMMY_Device* uniqueInstance;	
			unsigned char buf[8];

	protected:
			DUMMY_Device();
			~DUMMY_Device();
		
	public  :
			static DUMMY_Device* GetInstance();
			void Instance_Close();
				
			void DeviceInit();
			int FrameSend(unsigned char wBuf[]);
			int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
			int FrameRecv(unsigned char rBuf[]);
			int FarmeParser(unsigned char buf[]);
			unsigned int getCurrentSupportedCount();
			int checkEachAck(int order);
			int checkDisconnected();

};
#endif
