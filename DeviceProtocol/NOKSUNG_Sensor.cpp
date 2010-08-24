

#include "NOKSUNG_Sensor.h"
#include "../DeviceUart/NOKSUNG_UartRS485.h"


NOKSUNG_Sensor* NOKSUNG_Sensor::uniqueInstance = NULL;

NOKSUNG_Sensor* NOKSUNG_Sensor::GetInstance()
{
	
	if( uniqueInstance == NULL) {
		uniqueInstance = new NOKSUNG_Sensor();

	}

	return uniqueInstance;
}

NOKSUNG_Sensor::NOKSUNG_Sensor()
{
	Log(LOG::PRTCL, "Sensor Device Constructed\n");
}

NOKSUNG_Sensor::~NOKSUNG_Sensor()
{
	Log(LOG::PRTCL, "Sensor Device Deconstructed\n");
}

	char deviceCharName[MAX_DEVICENAME_STRING_LENGTH];
	SubSensorStatus subSensor[8];

void NOKSUNG_Sensor::DeviceInit()
{
	int i, j;
	Log(LOG::PRTCL, "Sensor Device Init\n");

	for(i = 0; i < MAX_SUPPORTED_SENSOR_CNT; i++)
	{
		sensorStatus[i].order = 0xFF;
		sensorStatus[i].isAck = FALSE;
	}

	for(i = 0; i < supportedPollingCount; i++)
	{
		sensorStatus[i].order = i + 1;
		
		for(j = 0; j < MAX_SUBSENSOR_CNT; j++)
		{
			sensorStatus[i].subSensor[j].subID = j + 1;
			sensorStatus[i].subSensor[j].action = SENSOR_NONE;
		}

		sprintf(sensorStatus[i].deviceCharName, "SENSOR%d", i + 1);
	}
}

int NOKSUNG_Sensor::FrameSend(unsigned char wBuf[])
{
	int result = 0;

	Log(LOG::PRTCL, "SENSOR SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6], wBuf[7], wBuf[8], wBuf[9]);
	result = (NOKSUNG_UartRS485::Instance())->WriteFrame(wBuf, NOKSUNG_PROTOCOL_SEND_LENGTH);

	//녹성 센서는 제어 command가 없기 대문에 제어 실패시 retry하는 부분이 필요 없다
	//usleep(100000);
	
	return result;
}

int NOKSUNG_Sensor::FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result = 0;
	unsigned short crc = 0;
	
	if(cmd_flag == POLLING_CMD)
	{
		buf[0] = order;
		buf[1] = 0x03;
		buf[2] = 0x02;
		buf[3] = 0x80;
		buf[4] = 0x00;
		buf[5] = 0x01;
		buf[6] = 0x00;		
		buf[7] = 0x00;

		crc = (NOKSUNG_UartRS485::Instance())->CRC_Make(buf, NOKSUNG_PROTOCOL_SEND_LENGTH - 2);

		buf[8] =	((unsigned char *)(&crc))[1];
		buf[9] = ((unsigned char *)(&crc))[0];
		
		result = FrameSend(buf);
	}

	//녹성 센서는 제어 command가 없다.
	if(cmd_flag == CONTROL_CMD)
	{

	}

	return result;
}

int NOKSUNG_Sensor::FrameRecv(unsigned char rBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "SENSOR RecvFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", rBuf[0], rBuf[1], rBuf[2], rBuf[3], rBuf[4], rBuf[5], rBuf[6], rBuf[7]);

	//녹성 센서는 제어 command가 없다
	
	result = FarmeParser(rBuf);

	return result;
}

int NOKSUNG_Sensor::FarmeParser(unsigned char buf[])
{
	unsigned char order;
	unsigned char subID_bit;
	unsigned char subID;
	int result = 0, i;

	order = buf[0];

	sensorStatus[order -1].isAck = TRUE;

	subID = 1;
	if((buf[5] & 1) == 1)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 1) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}

	subID = 2;
	if((buf[5] & 2) == 2)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 2) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}	

	subID = 3;
	if((buf[5] & 4) == 4)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 4) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}	

	subID = 4;
	if((buf[5] & 8) == 8)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 8) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}	

	subID = 5;
	if((buf[5] & 16) == 16)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 16) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}

	subID = 6;
	if((buf[5] & 32) == 32)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 32) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}

	subID = 7;
	if((buf[5] & 64) == 64)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 64) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}

	subID = 8;
	if((buf[5] & 128) == 128)				//비상 해제
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_UNDETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_UNDETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_UNDETECTED, 0);
		}
	}
	else if((buf[5] & 128) == 0)			//비상
	{
		if(sensorStatus[order -1].subSensor[subID - 1].action != SENSOR_DETECTED)
		{
			sensorStatus[order -1].subSensor[subID - 1].action = SENSOR_DETECTED;
			result = NotifyEventToService(SENSOR, order, SENSOR_ACTION_EVENT, subID, SENSOR_DETECTED, 0);
		}
	}

	return result;
}

int NOKSUNG_Sensor::checkEachAck(int order)
{
	if(sensorStatus[order - 1].isAck == TRUE)
		return TRUE;
	else
		return FALSE;
}

int NOKSUNG_Sensor::checkDisconnected()
{
	int order;
	int result;

	for(order = 1; order <= getCurrentSupportedCount(); order++)		
	{
		result = checkEachAck(order);

		if(result == TRUE)		//No Disconnected
			return TRUE;
	}

	return FALSE;	//Yes Disconnected
}

unsigned int NOKSUNG_Sensor::getCurrentSupportedCount()
{
	int order;
	int result;

	if(supportedPollingCount > MAX_SUPPORTED_SENSOR_CNT)
		supportedPollingCount = MAX_SUPPORTED_SENSOR_CNT;

	for(order = supportedPollingCount; order > 0; order--)
	{
		result = checkEachAck(order);
		if(result == TRUE)
			return order;
	}
	
	return 0;
}
