

// User Define Header
#include "ConcreteDeviceCreator.h"

ConcreteDeviceCreator* ConcreteDeviceCreator::uniqueInstance = NULL;

ConcreteDeviceCreator* ConcreteDeviceCreator::GetInstance()
{
	
	if( uniqueInstance == NULL) {
		uniqueInstance = new ConcreteDeviceCreator();
	}

	return uniqueInstance;
}

ConcreteDeviceCreator::ConcreteDeviceCreator()
{

}

ConcreteDeviceCreator::~ConcreteDeviceCreator()
{

}

DeviceProtocol* ConcreteDeviceCreator::DeviceFactoryMethod(enum DEVICE_NAME deviceName, enum DEVICE_PROTOCOL deviceProtocol)
{
	switch( deviceName)  
	{
	
		case LIGHT:
			if(deviceProtocol == COMMAX)
				return (CMX_Light::GetInstance());
			
			break;

		case GAS:
			if(deviceProtocol == COMMAX)
				return (CMX_Gas::GetInstance());

			break;

		case BOILER:
			if(deviceProtocol == COMMAX)
				return (CMX_Boiler::GetInstance());

			break;

		case BUNDLELIGHT:
			if(deviceProtocol == COMMAX)
				return (CMX_Bundlelight::GetInstance());

			break;

		case CURTAIN:
			if(deviceProtocol == HAMUN)
				return (HAMUN_Curtain::GetInstance());

			break;

		default:
		break;
	}

}

