
#ifndef __CONCRETEDEVICECREATOR_H__
#define __CONCRETEDEVICECREATOR_H__

// User Define Header
#include "DeviceCreator.h"

// System Header
#include <stdio.h>

class ConcreteDeviceCreator : public DeviceCreator
{

	private:
			static ConcreteDeviceCreator* uniqueInstance;	

	public:
			static ConcreteDeviceCreator* GetInstance();

			ConcreteDeviceCreator();
			~ConcreteDeviceCreator();

			DeviceProtocol* DeviceFactoryMethod(enum DEVICE_NAME deviceNAME, enum DEVICE_PROTOCOL deviceProtocol);
};

#endif
