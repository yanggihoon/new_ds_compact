

#include "HAMUN_Curtain.h"
#include "../DeviceUart/HAMUN_UartRS485.h"


HAMUN_Curtain* HAMUN_Curtain::uniqueInstance = NULL;

HAMUN_Curtain* HAMUN_Curtain::GetInstance()
{
	
	if( uniqueInstance == NULL) {
		uniqueInstance = new HAMUN_Curtain();

	}

	return uniqueInstance;
}

HAMUN_Curtain::HAMUN_Curtain()
{
	Log(LOG::PRTCL, "Curtain Device Constructed\n");
}

HAMUN_Curtain::~HAMUN_Curtain()
{
	Log(LOG::PRTCL, "Curtain Device Deconstructed\n");
}

void HAMUN_Curtain::DeviceInit()
{
	int i;
	Log(LOG::PRTCL, "Curtain Device Init\n");

	for(i = 0; i < MAX_SUPPORTED_CURTAIN_CNT; i++)
	{
		curtainStatus[i].order = 0xFF;
		curtainStatus[i].isAck = FALSE;
		curtainStatus[i].action = CURTAIN_NONE;
	}

	for(i = 0; i < supportedPollingCount; i++)
	{
		curtainStatus[i].order = i + 1;
		sprintf(curtainStatus[i].deviceCharName, "CURTAIN%d", i + 1);
	}

	isStartPolled = FALSE;
}

int HAMUN_Curtain::FrameSend(unsigned char wBuf[])
{
	int result = 0;

	isRecv = FALSE;
	retrySendCnt = 0;

	Log(LOG::PRTCL, "CURTAIN SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6]);
	result = (HAMUN_UartRS485::Instance())->WriteFrame(wBuf, HAMUN_PROTOCOL_LENGTH);

	usleep(100000);

	if(wBuf[0] == CURTAIN_CTRL_COMMAND && isRecv == FALSE)
	{
		while(isRecv == FALSE)
		{
			Log(LOG::ERR, "Retry CURTAIN SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6]);
			result = (HAMUN_UartRS485::Instance())->WriteFrame(wBuf, HAMUN_PROTOCOL_LENGTH);
			retrySendCnt++;

			if(retrySendCnt >= MAX_RETRY_SEND_CNT)
			{
				Log(LOG::ERR, "Retry CURTAIN Count Over\n");
				break;
			}

			usleep(200000);
		}
	}



	return result;
}

int HAMUN_Curtain::FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result = 0;

	if(cmd_flag == POLLING_CMD)
	{

	}

	if(cmd_flag == CONTROL_CMD)
	{
		switch(function1)
		{
			case CURTAIN_ACTION_EVENT:
			{
				buf[0] = CURTAIN_CTRL_COMMAND;
				buf[1] = 0x32;
				buf[2] = order + 0x30;

				if(function2 == CURTAIN_CLOSE)
					buf[3] = 0x01;
				else if(function2 == CURTAIN_OPEN)
					buf[3] = 0x02;
				else if(function2 == CURTAIN_STOP)
					buf[3] = 0x10;

				buf[4] = 0x00;
				buf[5] = 0x00;
				buf[6] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5];
			}
			
			default:
				break;
		}
		result = FrameSend(buf);
	}

	return result;
}

int HAMUN_Curtain::FrameRecv(unsigned char rBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "CURTAIN RecvFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x\n", rBuf[0], rBuf[1], rBuf[2], rBuf[3], rBuf[4], rBuf[5], rBuf[6]);
	if(rBuf[0] == CURTAIN_CTRL_ACK)
		isRecv = TRUE;

	result = FarmeParser(rBuf);

	return result;
}

int HAMUN_Curtain::FarmeParser(unsigned char buf[])
{
	unsigned char order;
	int result = 0;
	
	if(buf[0] == CURTAIN_CTRL_ACK)
	{
		if(buf[1] != 0x32)
			return result;

		order = buf[2] - 0x30;
		curtainStatus[order -1].isAck = TRUE;


		switch(buf[3])
		{
			case 0x01:
				curtainStatus[order -1].action = CURTAIN_CLOSE;
				result = NotifyEventToService(CURTAIN, order, CURTAIN_ACTION_EVENT, CURTAIN_CLOSE, 0, 0);				
				break;

			case 0x02:
				curtainStatus[order -1].action = CURTAIN_OPEN;
				result = NotifyEventToService(CURTAIN, order, CURTAIN_ACTION_EVENT, CURTAIN_OPEN, 0, 0);				
				break;

			case 0x10:
				curtainStatus[order -1].action = CURTAIN_STOP;
				result = NotifyEventToService(CURTAIN, order, CURTAIN_ACTION_EVENT, CURTAIN_STOP, 0, 0);				
				break;

			default:
				break;
		}

		curtainStatus[order - 1].power = (buf[5] & order);
	}

	return result;
}

int HAMUN_Curtain::checkEachAck(int order)
{
	if(curtainStatus[order - 1].isAck == TRUE)
		return TRUE;
	else
		return FALSE;
}

int HAMUN_Curtain::checkDisconnected()
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

unsigned int HAMUN_Curtain::getCurrentSupportedCount()
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
