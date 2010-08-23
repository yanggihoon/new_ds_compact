

#include "CMX_Gas.h"
#include "../DeviceUart/CMX_UartRS485.h"

static unsigned char current_order = 0x00;

CMX_Gas* CMX_Gas::uniqueInstance = NULL;

CMX_Gas* CMX_Gas::GetInstance()
{
	
	if( uniqueInstance == NULL) {
		uniqueInstance = new CMX_Gas();

	}

	return uniqueInstance;
}

CMX_Gas::CMX_Gas()
{
	Log(LOG::PRTCL, "GAS Device Constructed\n");
}

CMX_Gas::~CMX_Gas()
{
	Log(LOG::PRTCL, "GAS Device Deconstructed\n");
}

void CMX_Gas::DeviceInit()
{
	int i;
	Log(LOG::PRTCL, "GAS Device Init\n");
	for(i = 0; i < MAX_SUPPORTED_GAS_CNT; i++)
	{
		gasStatus[i].order = 0xFF;
		gasStatus[i].isAck = FALSE;
		gasStatus[i].action = GAS_NONE;
	}

	for(i = 0; i < supportedPollingCount; i++)
	{
		gasStatus[i].order = i + 1;
		sprintf(gasStatus[i].deviceCharName, "GAS%d", i + 1);
	}
}

int CMX_Gas::FrameSend(unsigned char wBuf[])
{
	int result = 0;

	isRecv = FALSE;
	retrySendCnt = 0;

	Log(LOG::PRTCL, "GAS SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6], wBuf[7]);
	result = (CMX_UartRS485::Instance())->WriteFrame(wBuf, CMX_PROTOCOL_LENGTH);

	if(wBuf[0] == GAS_CTRL_COMMAND && isRecv == FALSE)
	{
		usleep(100000);
		//가스 디바이스는 ack 응답이 느림 
		sleep(1);
		while(isRecv == FALSE)
		{
			Log(LOG::ERR, "Retry GAS SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6], wBuf[7]);
			result = (CMX_UartRS485::Instance())->WriteFrame(wBuf, CMX_PROTOCOL_LENGTH);
			retrySendCnt++;

			if(retrySendCnt >= MAX_RETRY_SEND_CNT)
			{
				Log(LOG::ERR, "Retry GAS Count Over\n");
				break;
			}

			usleep(200000);
		}
	}
	
	return result;
}

int CMX_Gas::FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result = 0;
	current_order = order;
	
	if(cmd_flag == POLLING_CMD)
	{
		buf[0] = GAS_STATUS_COMMAND;
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
			case GAS_CLOSE:
			{
				buf[0] = GAS_CTRL_COMMAND;
				buf[1] = order;
				buf[2] = 0x80;
				buf[3] = 0x00;
				buf[4] = 0x00;
				buf[5] = 0x00;
				buf[6] = 0x00;
				buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
			}
			
			default:
				break;
		}

		result = FrameSend(buf);
	}

	return result;
}

int CMX_Gas::FrameRecv(unsigned char rBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "GAS RecvFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", rBuf[0], rBuf[1], rBuf[2], rBuf[3], rBuf[4], rBuf[5], rBuf[6], rBuf[7]);
	if(rBuf[0] == GAS_CTRL_ACK)
		isRecv = TRUE;

	result = FarmeParser(rBuf);

	return result;
}

int CMX_Gas::FarmeParser(unsigned char buf[])
{
	unsigned char order;
	int result = 0;

	if(buf[0] == GAS_STATUS_ACK)  //|| buf[0] == GAS_CTRL_ACK) 제어 command ack에 대해서는 처리를 하지 않는다.(status ack로 상태 확인)
	{
		order = current_order;
		gasStatus[order -1].isAck = TRUE;


		if((buf[1] & 0x04) == 0x04)		
		{
			gasStatus[order -1 ].action = GAS_LEAK;
			result = NotifyEventToService(GAS, order, GAS_LEAK, 0, 0, 0);
		}

		if((buf[1] & 0x80) == 0x80)	
		{
			if(gasStatus[order - 1].action != GAS_OPEN)
			{
				gasStatus[order -1].action = GAS_OPEN;
				result = NotifyEventToService(GAS, order, GAS_OPEN, 0, 0, 0);				
			}
		}
		else if((buf[1] & 0x40) == 0x40)
		{
			if(gasStatus[order -1].action != GAS_CLOSE)
			{
				gasStatus[order - 1].action = GAS_CLOSE;
				result = NotifyEventToService(GAS, order, GAS_CLOSE, 0, 0, 0);					
			}
		}

		/*if(supportedPollingCount > 1)
		{
			order = 2;
			gasStatus[order -1].isAck = TRUE;
			
			if((buf[2] & 0x04) == 0x04)		
			{
				gasStatus[order -1 ].action = GAS_LEAK;
				result = NotifyEventToService(GAS, order, GAS_LEAK, 0, 0, 0);
			}

			if((buf[2] & 0x80) == 0x80)	
			{
				if(gasStatus[order - 1].action != GAS_OPEN)
				{
					gasStatus[order -1].action = GAS_OPEN;
					result = NotifyEventToService(GAS, order, GAS_OPEN, 0, 0, 0);				
				}
			}
			else if((buf[2] & 0x40) == 0x40)
			{
				if(gasStatus[order -1].action != GAS_CLOSE)
				{
					gasStatus[order - 1].action = GAS_CLOSE;
					result = NotifyEventToService(GAS, order, GAS_CLOSE, 0, 0, 0);					
				}
			}
		}*/
	}

	return result;
}

int CMX_Gas::checkEachAck(int order)
{
	if(gasStatus[order - 1].isAck == TRUE)
		return TRUE;
	else
		return FALSE;
}

int CMX_Gas::checkDisconnected()
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

unsigned int CMX_Gas::getCurrentSupportedCount()
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
