

#include "CMX_Light.h"
#include "../DeviceUart/CMX_UartRS485.h"


CMX_Light* CMX_Light::uniqueInstance = NULL;

CMX_Light* CMX_Light::GetInstance()
{
	
	if( uniqueInstance == NULL) {
		uniqueInstance = new CMX_Light();

	}

	return uniqueInstance;
}

CMX_Light::CMX_Light()
{
	Log(LOG::PRTCL, "LIGHT Device Constructed\n");
}

CMX_Light::~CMX_Light()
{
	Log(LOG::PRTCL, "LIGHT Device Deconstructed\n");
}

void CMX_Light::DeviceInit()
{
	int i;
	Log(LOG::PRTCL, "LIGHT Device Init\n");
	for(i = 0; i < MAX_SUPPORTED_LIGHT_CNT; i++)
	{
		lightStatus[i].order = 0xFF;
		lightStatus[i].isAck = FALSE;
		lightStatus[i].power = LIGHT_POWER_NONE;
		lightStatus[i].dimmingLevel = 0xFF;
	}

	for(i = 0; i < supportedPollingCount; i++)
	{
		lightStatus[i].order = i + 1;
		sprintf(lightStatus[i].deviceCharName, "Light%d", i + 1);
	}
}

int CMX_Light::FrameSend(unsigned char wBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "LIGHT SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6], wBuf[7]);
	result = (CMX_UartRS485::Instance())->WriteFrame(wBuf, CMX_PROTOCOL_LENGTH);
	
	return result;
}

int CMX_Light::FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result = 0;

	if(cmd_flag == POLLING_CMD)
	{
		buf[0] = LIGHT_STATUS_COMMAND;
		buf[1] = order;
		buf[2] = 0x00;
		buf[3] = 0x00;
		buf[4] = 0x00;
		buf[5] = 0x00;
		buf[6] = 0x00;		
		buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];

		result = FrameSend(buf);
	}

	if(cmd_flag == CONTROL_CMD)
	{
		switch(function1)
		{
			case LIGHT_MODE_BINARY:
			{
				if(function2 == LIGHT_POWER_EVENT)
				{
					if(function3 == LIGHT_POWER_ON)
					{
						buf[0] = LIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x01;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
					else if(function3 == LIGHT_POWER_OFF)
					{
						buf[0] = LIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x00;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
					else if(function3 == LIGHT_POWER_ALLON)
					{
						buf[0] = LIGHT_GROUP_CTRL_COMMAND;
						buf[1] = 0xFF;
						buf[2] = 0xFF;
						buf[3] = 0xFF;
						buf[4] = 0xFF;
						buf[5] = 0xFF;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
					else if(function3 == LIGHT_POWER_ALLOFF)
					{
						buf[0] = LIGHT_GROUP_CTRL_COMMAND;
						buf[1] = 0x00;
						buf[2] = 0x00;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
				}
				break;
			}
			
			case LIGHT_MODE_DIMMABLE:
			{
				if(function2 == LIGHT_POWER_EVENT)
				{
					if(function3 == LIGHT_POWER_ON)
					{
						buf[0] = LIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x01;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x08;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
					else if(function3 == LIGHT_POWER_OFF)
					{
						buf[0] = LIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x01;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
					else if(function3 == LIGHT_POWER_ALLON)
					{
						buf[0] = LIGHT_GROUP_CTRL_COMMAND;
						buf[1] = 0xFF;
						buf[2] = 0xFF;
						buf[3] = 0xFF;
						buf[4] = 0xFF;
						buf[5] = 0xFF;
						buf[6] = 0x08;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
					else if(function3 == LIGHT_POWER_ALLOFF)
					{
						buf[0] = LIGHT_GROUP_CTRL_COMMAND;
						buf[1] = 0x00;
						buf[2] = 0x00;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					}
				}
				else if(function2 == LIGHT_DIMMING_EVENT)
				{
					buf[0] = LIGHT_CTRL_COMMAND;
					buf[1] = order;
					buf[2] = 0x01;
					buf[3] = 0x00;
					buf[4] = 0x00;
					buf[5] = 0x00;
					buf[6] = function3;
					buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
				}
				break;	
			}

			default:
				break;
		}

		result = FrameSend(buf);
	}

	return result;
}

int CMX_Light::FrameRecv(unsigned char rBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "LIGHT RecvFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", rBuf[0], rBuf[1], rBuf[2], rBuf[3], rBuf[4], rBuf[5], rBuf[6], rBuf[7]);
	result = FarmeParser(rBuf);

	return result;
}

int CMX_Light::FarmeParser(unsigned char buf[])
{
	unsigned char order;
	int result = 0;

	if(buf[0] == LIGHT_STATUS_ACK  || buf[0] == LIGHT_CTRL_ACK)
	{
		order = buf[2];

		lightStatus[order -1].isAck = TRUE;

		if(buf[1] == 0x00)
		{
			if(lightStatus[order -1].power != LIGHT_POWER_OFF)
			{
				lightStatus[order -1].power = LIGHT_POWER_OFF;	

				if(buf[6] == 0x00)
					result = NotifyEventToService(LIGHT, order, LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF, 0);
				else
					result = NotifyEventToService(LIGHT, order, LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF, 0);

				lightStatus[order - 1].dimmingLevel = 0x00;
			}
		}
		else if(buf[1] == 0x01)
		{
			if(lightStatus[order -1].power != LIGHT_POWER_ON)	//power off
			{
				lightStatus[order -1].power = LIGHT_POWER_ON;	//power on

				if(buf[6] == 0x00)
					result = NotifyEventToService(LIGHT, order, LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON, 0);
				else
					result = NotifyEventToService(LIGHT, order, LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON, buf[5]);

				lightStatus[order - 1].dimmingLevel = 	lightStatus[order - 1].maxDimmingLevel;
			}
		}
		
		if(buf[6] == 0x00)
		{
			lightStatus[order -1].mode = LIGHT_MODE_BINARY;
			lightStatus[order -1].dimmingLevel = 0;
			lightStatus[order -1].maxDimmingLevel = 0;
		}
		else
		{
			lightStatus[order -1].mode = LIGHT_MODE_DIMMABLE;	//dimming
			if(lightStatus[order - 1].dimmingLevel != buf[5])
			{
				lightStatus[order - 1].dimmingLevel = buf[5];
				result = NotifyEventToService(LIGHT, order, LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, lightStatus[order - 1].dimmingLevel, 0);								
			}
			lightStatus[order -1].maxDimmingLevel = buf[6];
		}
	}

	return result;
}

int CMX_Light::checkEachAck(int order)
{
	if(lightStatus[order - 1].isAck == TRUE)
		return TRUE;
	else
		return FALSE;
}

int CMX_Light::checkDisconnected()
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

unsigned int CMX_Light::getCurrentSupportedCount()
{
	int order;
	int result;
	for(order = 1; order <= supportedPollingCount; order++)
	{
		result = checkEachAck(order);
		if(result == FALSE)	
			return order - 1;
	}

	return supportedPollingCount;
}
