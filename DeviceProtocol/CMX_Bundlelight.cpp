

#include "CMX_Bundlelight.h"
#include "../DeviceUart/CMX_UartRS485.h"


CMX_Bundlelight* CMX_Bundlelight::uniqueInstance = NULL;

CMX_Bundlelight* CMX_Bundlelight::GetInstance()
{
	
	if( uniqueInstance == NULL) {
		uniqueInstance = new CMX_Bundlelight();

	}

	return uniqueInstance;
}

CMX_Bundlelight::CMX_Bundlelight()
{
	Log(LOG::PRTCL, "BUNDLELIGHT Device Constructed\n");
}

CMX_Bundlelight::~CMX_Bundlelight()
{
	Log(LOG::PRTCL, "BUNDLELIGHT Device Deconstructed\n");
}

void CMX_Bundlelight::DeviceInit()
{
	int i;
	Log(LOG::PRTCL, "BUNDLELIGHT Device Init\n");
	for(i = 0; i < MAX_SUPPORTED_BUNDLELIGHT_CNT; i++)
	{
		bundlelightStatus[i].order = 0xFF;
		bundlelightStatus[i].isAck = FALSE;
		bundlelightStatus[i].power = BUNDLELIGHT_POWER_ON;
		bundlelightStatus[i].readyPower = BUNDLELIGHT_READYPOWER_NONE;
		bundlelightStatus[i].elevatorCall = BUNDLELIGHT_ELEVATORCALL_NONE;
		bundlelightStatus[i].out = BUNDLELIGHT_OUT_NONE;
		bundlelightStatus[i].gasClose = BUNDLELIGHT_GASCLOSE_NONE;
		bundlelightStatus[i].supported_function._none = NOT_SUPPORTED;
		bundlelightStatus[i].supported_function._elevatorcall = NOT_SUPPORTED;
		bundlelightStatus[i].supported_function._gasclose = NOT_SUPPORTED;
		bundlelightStatus[i].supported_function._out = NOT_SUPPORTED;
		bundlelightStatus[i].supported_function._power = NOT_SUPPORTED;
		bundlelightStatus[i].supported_function._readypower = NOT_SUPPORTED;
	}

	for(i = 0; i < supportedPollingCount; i++)
	{
		bundlelightStatus[i].order = i + 1;
		sprintf(bundlelightStatus[i].deviceCharName, "BUNDLELIGHT%d", i + 1);
	}
}

int CMX_Bundlelight::FrameSend(unsigned char wBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "BUNDLELIGHT SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6], wBuf[7]);
	result = (CMX_UartRS485::Instance())->WriteFrame(wBuf, CMX_PROTOCOL_LENGTH);
	
	return result;
}

int CMX_Bundlelight::FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result = 0;
	int gas_open = FALSE;
	D_Item gas1_item;
	D_Item gas2_item;	
	enum DEVICE_PROTOCOL dProtocol;
	
	if(cmd_flag == POLLING_CMD)
	{
		//일괄소등 가스 밸브 상태 확인은 가스 밸브 1번 아이디만 확인 한다.
		buf[0] = BUNDLELIGHT_STATUS_COMMAND;
		buf[1] = order;

		if(get_device_index(GAS) != -1)
		{
			get_device_item(GAS, 1, &gas1_item);	
			if(gas1_item.gasItem.action == GAS_OPEN)
				gas_open = TRUE;

			if(get_current_supported_cnt(GAS) > 1)
			{
				get_device_item(GAS, 2, &gas2_item);
				if(gas2_item.gasItem.action == GAS_OPEN)
					gas_open = TRUE;
			}

			if(gas_open == TRUE)
				buf[2] = 0x01;
			else
				buf[2] = 0x00;
		}
		else
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
		//이전 현장에 설치된 제품과 제어 command 구분 필요 (function 기능이 없으면 이전 현장)
		if(bundlelightStatus[order - 1].supported_function._none == SUPPORTED)
		{
			buf[0] = OLD_BUNDLELIGHT_CTRL_COMMAND;
			buf[1] = order;

			switch(function1)
			{
				case BUNDLELIGHT_POWER_EVENT:
				{
					if(function2 == BUNDLELIGHT_POWER_ON)
						buf[2] = 0x01;
					else if(function2 == BUNDLELIGHT_POWER_OFF)
						buf[2] = 0x00;
	 			}
				break;

				default:
					break;
			}

			buf[3] = 0x00;
			buf[4] = 0x00;
			buf[5] = 0x00;
			buf[6] = 0x00;
			buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
		}
		else
		{
			switch(function1)
			{
				case BUNDLELIGHT_POWER_EVENT:
				{
					if(function2 == BUNDLELIGHT_POWER_ON)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x01;
						buf[3] = 0x01;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 						
					}
					else if(function2 == BUNDLELIGHT_POWER_OFF)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;

						if(bundlelightStatus[order - 1].readyPower == BUNDLELIGHT_READYPOWER_ON)
							buf[2] = 0x02;
						else
							buf[2] = 0x00;
						
						buf[3] = 0x01;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 						
					}
					else if(function2 == BUNDLELIGHT_POWER_ALLON)
					{
						buf[0] = BUNDLELIGHT_GROUP_POWER_COMMAND;
						buf[1] = 0xFF;
						buf[2] = 0x00;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 
					}
					else if(function2 == BUNDLELIGHT_POWER_ALLOFF)
					{
						buf[0] = BUNDLELIGHT_GROUP_POWER_COMMAND;
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

				case BUNDLELIGHT_READYPOWER_EVENT:
				{
					if(function2 == BUNDLELIGHT_READYPOWER_ON)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x02;
						buf[3] = 0x02;						
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 						
						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_OFF)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;

						if(bundlelightStatus[order - 1].power == BUNDLELIGHT_POWER_ON)
							buf[2] = 0x01;
						else
							buf[2] = 0x00;

						buf[3] = 0x02;						
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_ALLON)
					{
						buf[0] = BUNDLELIGHT_GROUP_READYPOWER_COMMAND;
						buf[1] = 0xFF;
						buf[2] = 0x00;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_ALLOFF)
					{
						buf[0] = BUNDLELIGHT_GROUP_READYPOWER_COMMAND;
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

				case BUNDLELIGHT_OUT_EVENT:
				{
					if(function2 == BUNDLELIGHT_OUT_REQUEST_SUCCESS)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x04;
						buf[3] = 0x03;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 						

						bundlelightStatus[order -1].out = BUNDLELIGHT_OUT_NONE;
					}
					else if(function2 == BUNDLELIGHT_OUT_REQUEST_FAIL)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x08;
						buf[3] = 0x04;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 	

						bundlelightStatus[order -1].out = BUNDLELIGHT_OUT_REQUEST_FAIL;
					}
					else if(function2 == BUNDLELIGHT_OUT_SET)
					{
						buf[0] = BUNDLELIGHT_OUTMODE_COMMAND;
						buf[1] = 0x00;
						buf[2] = 0x00;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 

						bundlelightStatus[order -1].out = BUNDLELIGHT_OUT_SET;
					}
					else if(function2 == BUNDLELIGHT_OUT_CANCEL)
					{
						buf[0] = BUNDLELIGHT_OUTMODE_COMMAND;
						buf[1] = 0x01;
						buf[2] = 0x00;
						buf[3] = 0x00;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 	

						bundlelightStatus[order -1].out = BUNDLELIGHT_OUT_CANCEL;
					}
	 			}
				break;

				case BUNDLELIGHT_GASCLOSE_EVENT:
				{
					if(function2 == BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x10;
						buf[3] = 0x05;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 

						bundlelightStatus[order -1].gasClose = BUNDLELIGHT_GASCLOSE_NONE;
					}
					else if(function2 == BUNDLELIGHT_GASCLOSE_REQUEST_FAIL)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x20;
						buf[3] = 0x06;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 						

						bundlelightStatus[order -1].gasClose = BUNDLELIGHT_GASCLOSE_REQUEST_FAIL;
					}
	 			}
				break;

				case BUNDLELIGHT_ELEVATORCALL_EVENT:
				{
					if(function2 == BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x40;
						buf[3] = 0x07;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 	

						bundlelightStatus[order -1].elevatorCall = BUNDLELIGHT_ELEVATORCALL_NONE;
					}
					else if(function2 == BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL)
					{
						buf[0] = NEW_BUNDLELIGHT_CTRL_COMMAND;
						buf[1] = order;
						buf[2] = 0x80;
						buf[3] = 0x08;
						buf[4] = 0x00;
						buf[5] = 0x00;
						buf[6] = 0x00;
						buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]; 

						bundlelightStatus[order -1].elevatorCall = BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL;
					}
	 			}
				break;

				default:
					break;
			}
		}
			
		result = FrameSend(buf);
	}

	return result;
}

int CMX_Bundlelight::FrameRecv(unsigned char rBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "BUNDLELIGHT RecvFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", rBuf[0], rBuf[1], rBuf[2], rBuf[3], rBuf[4], rBuf[5], rBuf[6], rBuf[7]);
	result = FarmeParser(rBuf);

	return result;
}

int CMX_Bundlelight::FarmeParser(unsigned char buf[])
{
	unsigned char order;
	int result = 0;

	if(buf[5] == 0x00)		//기존 현장과 구분
	{
		bundlelightStatus[order - 1].isAck = TRUE;
		
		order = buf[2];

		if(buf[1] == BUNDLELIGHT_POWER_OFF)
		{
			if(bundlelightStatus[order - 1].power != BUNDLELIGHT_POWER_OFF)
			{
				bundlelightStatus[order - 1].power = BUNDLELIGHT_POWER_OFF;	
				result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF, 0, 0);
			}
		}
		else if(buf[1] == BUNDLELIGHT_POWER_ON)
		{
			if(bundlelightStatus[order - 1].power != BUNDLELIGHT_POWER_ON)
			{
				bundlelightStatus[order - 1].power = BUNDLELIGHT_POWER_ON;	
				result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON, 0, 0);
			}
		}	

		bundlelightStatus[order - 1].supported_function._power = SUPPORTED;
		bundlelightStatus[order - 1].supported_function._none = NOT_SUPPORTED;
		bundlelightStatus[order - 1].supported_function._gasclose = NOT_SUPPORTED;
		bundlelightStatus[order - 1].supported_function._out = NOT_SUPPORTED;
		bundlelightStatus[order - 1].supported_function._readypower = NOT_SUPPORTED;
		bundlelightStatus[order - 1].supported_function._elevatorcall = NOT_SUPPORTED;

	}
	else
	{
		order = buf[2];

		bundlelightStatus[order - 1].isAck = TRUE;

		bundlelightStatus[order - 1].supported_function._none = NOT_SUPPORTED;
		if((buf[5] & 0x01) == 0x01)	
			bundlelightStatus[order - 1].supported_function._gasclose = SUPPORTED;

		if((buf[5] & 0x02) == 0x02)
			bundlelightStatus[order - 1].supported_function._out = SUPPORTED;

		if((buf[5] & 0x04) == 0x04)
			bundlelightStatus[order - 1].supported_function._power = SUPPORTED;

		if((buf[5] & 0x08) == 0x08)
			bundlelightStatus[order - 1].supported_function._readypower = SUPPORTED;

		if((buf[5] & 0x10) == 0x10)
			bundlelightStatus[order - 1].supported_function._elevatorcall = SUPPORTED;

		if(buf[1] == 0x00)
		{
			if(bundlelightStatus[order - 1].power != BUNDLELIGHT_POWER_OFF)
			{
				bundlelightStatus[order - 1].power = BUNDLELIGHT_POWER_OFF;	
				result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF, 0, 0);
			}
		}
		else if(buf[1] == 0x01)
		{
			if(bundlelightStatus[order - 1].power != BUNDLELIGHT_POWER_ON)
			{
				bundlelightStatus[order - 1].power = BUNDLELIGHT_POWER_ON;	
				result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON, 0, 0);
			}
		}		

		if(bundlelightStatus[order - 1].supported_function._readypower == SUPPORTED)
		{
			if(buf[3] == 0x00)
			{
				if(bundlelightStatus[order - 1].readyPower != BUNDLELIGHT_READYPOWER_OFF)
				{
					bundlelightStatus[order - 1].readyPower = BUNDLELIGHT_READYPOWER_OFF;	
					result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF, 0, 0);
				}
			}
			else if(buf[3] == 0x01)
			{
				if(bundlelightStatus[order - 1].readyPower != BUNDLELIGHT_READYPOWER_ON)
				{
					bundlelightStatus[order - 1].readyPower = BUNDLELIGHT_READYPOWER_ON;	
					result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON, 0, 0);
				}
			}		
		}

		if(bundlelightStatus[order - 1].supported_function._elevatorcall == SUPPORTED)
		{
			if((buf[4] & 0x08) == 0x08)		//BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST
			{
				if(bundlelightStatus[order - 1].elevatorCall != BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST)
				{
					bundlelightStatus[order - 1].elevatorCall = BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST;	
					result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST, 0, 0);
				}
			}
			else if((buf[4] & 0x04) == 0x04)		//BUNDLELIGHT_ELEVATORCALL_UP_REQUEST
			{
				if(bundlelightStatus[order - 1].elevatorCall != BUNDLELIGHT_ELEVATORCALL_UP_REQUEST)
				{
					bundlelightStatus[order - 1].elevatorCall = BUNDLELIGHT_ELEVATORCALL_UP_REQUEST;	
					result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST, 0, 0);
				}
			}
			else
			{
				bundlelightStatus[order - 1].elevatorCall = BUNDLELIGHT_ELEVATORCALL_NONE;
			}
		}

		if(bundlelightStatus[order - 1].supported_function._out == SUPPORTED)
		{
			if((buf[4] & 0x02) == 0x02)		//BUNDLELIGHT_OUT_REQUEST
			{
				if(bundlelightStatus[order - 1].out != BUNDLELIGHT_OUT_REQUEST)
				{
					bundlelightStatus[order - 1].out = BUNDLELIGHT_OUT_REQUEST;	
					result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST, 0, 0);
				}
			}
			else
			{
				bundlelightStatus[order - 1].out != BUNDLELIGHT_OUT_NONE;
			}
		}

		if(bundlelightStatus[order - 1].supported_function._gasclose == SUPPORTED)
		{
			if((buf[4] & 0x01) == 0x01)		//BUNDLELIGHT_GASCLOSE_REQUEST
			{
				if(bundlelightStatus[order - 1].gasClose != BUNDLELIGHT_GASCLOSE_REQUEST)
				{
					bundlelightStatus[order - 1].gasClose = BUNDLELIGHT_GASCLOSE_REQUEST;	
					result = NotifyEventToService(BUNDLELIGHT, order, BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST, 0, 0);
				}
			}
			else
			{
				bundlelightStatus[order - 1].gasClose = BUNDLELIGHT_GASCLOSE_NONE;
			}
		}
	}

	return result;
}

int CMX_Bundlelight::checkEachAck(int order)
{
	if(bundlelightStatus[order - 1].isAck == TRUE)
		return TRUE;
	else
		return FALSE;
}

int CMX_Bundlelight::checkDisconnected()
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

unsigned int CMX_Bundlelight::getCurrentSupportedCount()
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
