

#ifndef __DEVICECREATOR_H__
#define __DEVICECREATOR_H__

#include "../DeviceProtocol/DeviceProtocol.h"
#include "../DeviceProtocol/DUMMY_Device.h"
#include "../DeviceProtocol/CMX_Bundlelight.h"
#include "../DeviceProtocol/CMX_Light.h"
#include "../DeviceProtocol/CMX_Gas.h"
#include "../DeviceProtocol/CMX_Boiler.h"
#include "../DeviceProtocol/HAMUN_Curtain.h"

class DeviceCreator
{

	public:
			 ~DeviceCreator() {};
			 virtual DeviceProtocol* DeviceFactoryMethod(enum DEVICE_NAME deviceNAME, enum DEVICE_PROTOCOL deviceProtocol) = 0;
};

#endif
