
#ifndef __NOKSUNG_Sensor_H__
#define __NOKSUNG_Sensor_H__

// User Define Header
#include "DeviceProtocol.h"

// System Header
#include <app.h>

//CMD
#define SENSOR_STATUS_COMMAND				0x00
#define SENSOR_CTRL_COMMAND					0x00

//ACK
#define SENSOR_STATUS_COMMAND_ACK			0x00
#define SENSOR_CTRL_COMMAND_ACK				0x00

#define MAX_SUPPORTED_SENSOR_CNT				8
#define MAX_SUBSENSOR_CNT						8

struct SubSensorStatus
{
	unsigned char subID;
	enum SENSOR_ACTION action;	
};

struct SensorStatus
{
	unsigned int order;	
	unsigned int isAck;
	char deviceCharName[MAX_DEVICENAME_STRING_LENGTH];
	SubSensorStatus subSensor[MAX_SUBSENSOR_CNT];
};

class  NOKSUNG_Sensor : public DeviceProtocol
{

	private :
			static NOKSUNG_Sensor* uniqueInstance;	
		 	unsigned char buf[7];

	protected:
			NOKSUNG_Sensor();
			~NOKSUNG_Sensor();
		
	public  :
			static NOKSUNG_Sensor* GetInstance();

			void DeviceInit();
			int FrameSend(unsigned char wBuf[]);
			int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
			int FrameRecv(unsigned char rBuf[]);
			int FarmeParser(unsigned char buf[]);
			unsigned int getCurrentSupportedCount();
			int checkEachAck(int order);
			int checkDisconnected();

			unsigned int isStartPolled;
			SensorStatus	sensorStatus[MAX_SUPPORTED_SENSOR_CNT];
};
#endif
