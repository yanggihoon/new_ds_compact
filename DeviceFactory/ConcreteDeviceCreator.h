
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

	protected:
			ConcreteDeviceCreator();
			~ConcreteDeviceCreator();
		
	public:
			static ConcreteDeviceCreator* GetInstance();


			DeviceProtocol* DeviceFactoryMethod(enum DEVICE_NAME deviceNAME, enum DEVICE_PROTOCOL deviceProtocol);
};

#endif
