
#include "CMX_Boiler.h"
#include "../DeviceUart/CMX_UartRS485.h"

CMX_Boiler* CMX_Boiler::uniqueInstance = NULL;

CMX_Boiler* CMX_Boiler::GetInstance()
{

	if( uniqueInstance == 0) {

		uniqueInstance = new CMX_Boiler();

	}

	return uniqueInstance;
}

CMX_Boiler::CMX_Boiler()
{
	Log(LOG::PRTCL, "BOILER Device Constructed\n");
}

CMX_Boiler::~CMX_Boiler()
{
	Log(LOG::PRTCL, "BOILER Device Deconstructed\n");
}

void CMX_Boiler::DeviceInit()
{
	int i;
	Log(LOG::PRTCL, "BOILER Device Init\n");
	//boiler는 전체 count를 다른 방법으로 가져 오므로 처음 부터 값을 입력 한다.
	for(i = 0; i < MAX_SUPPORTED_BOILER_CNT; i++)
	{
		boilerStatus[i].order = i + 1;		
		boilerStatus[i].isAck = FALSE;
		boilerStatus[i].power = BOILER_POWER_NONE;
		boilerStatus[i].out = BOILER_OUT_NONE;
		sprintf(boilerStatus[i].deviceCharName, "BOILER%d", i + 1);
	}

	/*
	for(i = 0; i < supportedPollingCount; i++)
	{
		boilerStatus[i].order = i + 1;

	}
	*/

	isMethodPolled = FALSE;
}

int CMX_Boiler::FrameSend(unsigned char wBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "BOILER SendFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", wBuf[0], wBuf[1], wBuf[2], wBuf[3], wBuf[4], wBuf[5], wBuf[6], wBuf[7]);
	result = (CMX_UartRS485::Instance())->WriteFrame(wBuf, CMX_PROTOCOL_LENGTH);
	
	return result;
}

int CMX_Boiler::FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result = 0;

	if(cmd_flag == POLLING_CMD)
	{
		if(order == 0)
		{
			buf[0] = BOILER_CTRL_METHOD_COMMAND;
			buf[1] = 0x00;
			buf[2] = 0x00;
			buf[3] = 0x00;
			buf[4] = 0x00;
			buf[5] = 0x00;
			buf[6] = 0x00;		
			buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];

			isMethodPolled = TRUE;
		}
		else
		{
			if(boilerMode == INDIVIDUAL_BOILER)
			{
				buf[0] = INDIVIDUAL_BOILER_STATUS_COMMAND;
				buf[1] = 0x00;
			}

			if(boilerMode == EACH_BOILER)
			{
				buf[0] = EACH_BOILER_STATUS_COMMAND;
				buf[1] = order;
			}
			
			buf[2] = 0x00;
			buf[3] = 0x00;
			buf[4] = 0x00;
			buf[5] = 0x00;
			buf[6] = 0x00;		
			buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
		}

		result = FrameSend(buf);
	}

	if(cmd_flag == CONTROL_CMD)
	{
		switch(function1)
		{
			case BOILER_POWER_EVENT:
			{
				if(boilerMode == INDIVIDUAL_BOILER)
				{
					buf[0] = INDIVIDUAL_BOILER_CTRL_COMMAND;
					buf[1] = 0x01;

					if(function2 == BOILER_POWER_ON)
						buf[2] = 0xFF;
					else 
						buf[2] = 0x00;
						

					buf[3] = 0x00;
					buf[4] = 0x00;
					buf[5] = 0x00;
					buf[6] = 0x00;
					buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					printf("INDIVIDUAL_BOILER_CTRL_COMMAND boilerMode = %d\n", boilerMode);
				}
				else
				{
					buf[0] = EACH_BOILER_CTRL_COMMAND;
					buf[1] = order;
					buf[2] = 0x04;
					
					if(function2 == BOILER_POWER_ON)
						buf[3] = 0x81;
					else 
						buf[3] = 0x80;

					buf[4] = 0x00;
					buf[5] = 0x00;
					buf[6] = 0x00;
					buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
					printf("EACH_BOILER_CTRL_COMMAND boilerMode = %d\n", boilerMode);					
				}
			}
			break;

			case BOILER_RTEMP_EVENT:
			{
				if(boilerMode == INDIVIDUAL_BOILER)
				{
					buf[0] = INDIVIDUAL_BOILER_CTRL_COMMAND;
					buf[1] = 0x02;
					//translate Decimal To BCD Code		
					buf[2] = (((function2 / 10) & 0x0F) << 4) + (function2 % 10);
					buf[3] = 0x00;
					buf[4] = 0x00;
					buf[5] = 0x00;
					buf[6] = 0x00;
					buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
				}
				else
				{
					buf[0] = EACH_BOILER_CTRL_COMMAND;
					buf[1] = order;
					buf[2] = 0x03;
					//translate Decimal To BCD Code		
					buf[3] = (((function2 / 10) & 0x0F) << 4) + (function2 % 10);
					buf[4] = 0x00;
					buf[5] = 0x00;
					buf[6] = 0x00;
					buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
				}
			}
			break;
			
			case BOILER_OUT_EVENT:
			{
				buf[0] = BOILER_OUT_CTRL_COMMAND;
				buf[1] = 0x01;

				if(function2 == BOILER_OUT_SET)
					buf[2] = 0x01;
				else 
					buf[2] = 0x02;

				buf[3] = 0x00;
				buf[4] = 0x00;
				buf[5] = 0x00;
				buf[6] = 0x00;
				buf[7] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6];
			}
			break;

			default:
				break;
		}

		result = FrameSend(buf);
	}

	return result;
}

int CMX_Boiler::FrameRecv(unsigned char rBuf[])
{
	int result = 0;
	Log(LOG::PRTCL, "BOILER RecvFrame : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", rBuf[0], rBuf[1], rBuf[2], rBuf[3], rBuf[4], rBuf[5], rBuf[6], rBuf[7]);
	result = FarmeParser(rBuf);

	return result;
}

int CMX_Boiler::FarmeParser(unsigned char buf[])
{
	unsigned char order;
	unsigned char ctemp, rtemp;
	int result = 0;

	if(buf[0]  == BOILER_CTRL_METHOD_ACK)
	{
		if(buf[2] == 0x01)
		{
			boilerMode = INDIVIDUAL_BOILER;
			boilerCnt = 1;
			supportedPollingCount = 1;
		}
		else
		{
			boilerMode = EACH_BOILER;
			boilerCnt = buf[5];
			supportedPollingCount = buf[5];
		}
		
		//설정치 하한
		boilerTemperatureMin =  ( ((buf[3] & 0xF0) >> 4) * 10);
		boilerTemperatureMin +=  ( buf[3] & 0x0F) ;						

		//설정치 상한
		boilerTemperatureMax =  ( ((buf[4] & 0xF0) >> 4) * 10);
		boilerTemperatureMax +=  ( buf[4] & 0x0F) ;

		if(boilerTemperatureMin >= 40)
			boilerDetailMode =HYPOCAUST_BOILER;
		else
			boilerDetailMode =INNERTEMPERATURE_BOILER;
	}

	else if(buf[0] == INDIVIDUAL_BOILER_STATUS_ACK)			//개별 보일러 제어 command에 대한 ACK는 무시 함(INDIVIDUAL_BOILER_CTRL_ACK)
	{
		boilerStatus[0].isAck = TRUE;

		rtemp =  ( ((buf[2] & 0xF0) >> 4) * 10);
		rtemp +=  ( buf[2] & 0x0F);						
		ctemp =  ( ((buf[3] & 0xF0) >> 4) * 10);
		ctemp +=  ( buf[3] & 0x0F) ;					

		//power on/off
		if ( (buf[1] == 0x80) || ( (buf[1] & 128) == 128) ) 
		{
			if( boilerStatus[0].power != BOILER_POWER_ON) 
			{
				boilerStatus[0].power = BOILER_POWER_ON;
				boilerStatus[0].rtemp = rtemp;
				boilerStatus[0].ctemp = ctemp;
				result = NotifyEventToService(BOILER, 1, BOILER_POWER_EVENT, BOILER_POWER_ON, boilerStatus[0].rtemp, boilerStatus[0].ctemp);				 
			} 
		}
		else if( (buf[1] == 0x00) || ( (buf[1] & 128) == 0) )
		{
			if( boilerStatus[0].power != BOILER_POWER_OFF) 
			{
				boilerStatus[0].power = BOILER_POWER_OFF;
				boilerStatus[0].rtemp = rtemp;
				boilerStatus[0].ctemp = ctemp;
				result = NotifyEventToService(BOILER, 1, BOILER_POWER_EVENT, BOILER_POWER_OFF, boilerStatus[0].rtemp, boilerStatus[0].ctemp);				 
			} 
		}

		//설정 온도
		if( boilerStatus[0].rtemp != rtemp)
		{
			boilerStatus[0].rtemp = rtemp;
			boilerStatus[0].ctemp = ctemp;
			result = NotifyEventToService(BOILER, 1, BOILER_RTEMP_EVENT, boilerStatus[0].rtemp, boilerStatus[0].ctemp , 0);
		}

		//현재 온도
		if( boilerStatus[0].ctemp != ctemp)
		{
			boilerStatus[0].rtemp = rtemp;
			boilerStatus[0].ctemp = ctemp;
			result = NotifyEventToService(BOILER, 1, BOILER_CTEMP_EVENT, boilerStatus[0].rtemp, boilerStatus[0].ctemp , 0);
		}
	}

	else if(buf[0] == EACH_BOILER_STATUS_ACK || buf[0] == EACH_BOILER_CTRL_ACK)
	{
		order = buf[2];
		rtemp =  ( ((buf[4] & 0xF0) >> 4) * 10);
		rtemp +=  ( buf[4] & 0x0F);						
		ctemp =  ( ((buf[3] & 0xF0) >> 4) * 10);
		ctemp +=  ( buf[3] & 0x0F) ;						

		boilerStatus[order - 1].isAck = TRUE;

		//power on/off
		if((buf[1] & 0x01)  == 1) 
		{
			if(boilerStatus[order - 1].power != BOILER_POWER_ON)
			{
				boilerStatus[order - 1].power = BOILER_POWER_ON;
				boilerStatus[order - 1].rtemp = rtemp;
				boilerStatus[order - 1].ctemp = ctemp;
				result = NotifyEventToService(BOILER, order, BOILER_POWER_EVENT, BOILER_POWER_ON, boilerStatus[order - 1].rtemp, boilerStatus[order - 1].ctemp);
			}
		}
		else if((buf[1] & 0x01)  == 0) 
		{
			if(boilerStatus[order - 1].power != BOILER_POWER_OFF)
			{
				boilerStatus[order - 1].power = BOILER_POWER_OFF;
				boilerStatus[order - 1].rtemp = rtemp;
				boilerStatus[order - 1].ctemp = ctemp;
				result = NotifyEventToService(BOILER, order, BOILER_POWER_EVENT, BOILER_POWER_OFF, boilerStatus[order - 1].rtemp, boilerStatus[order - 1].ctemp);
			}
		}

		//외출설정/해제
		if(((buf[1] & 0x0F) >> 2) == 1)
		{
			if(boilerStatus[order - 1].out !=  BOILER_OUT_SET)
			{
				boilerStatus[order - 1].out =  BOILER_OUT_SET;
				boilerStatus[order - 1].rtemp = rtemp;
				boilerStatus[order - 1].ctemp = ctemp;

				result = NotifyEventToService(BOILER, order, BOILER_OUT_EVENT, BOILER_OUT_SET, boilerStatus[order - 1].rtemp, boilerStatus[order - 1].ctemp);
			}
		}
		else if(((buf[1] & 0x0F) >> 2) == 0)
		{
			if(boilerStatus[order - 1].out !=  BOILER_OUT_RELEASE)
			{
				boilerStatus[order - 1].out =  BOILER_OUT_RELEASE;
				boilerStatus[order - 1].rtemp = rtemp;
				boilerStatus[order - 1].ctemp = ctemp;

				result = NotifyEventToService(BOILER, order, BOILER_OUT_EVENT, BOILER_OUT_RELEASE, boilerStatus[order - 1].rtemp, boilerStatus[order - 1].ctemp);
			}
		}

		//설저 온도
		//power on/off  외출설정 /해제 이벤트가 발생하면 boiler_status[order - 1][2] = rtemp 됨 그래서 설정 온도 이벤트는 발생 안 함
		if( boilerStatus[order - 1].rtemp != rtemp)
		{
			boilerStatus[order - 1].rtemp = rtemp;
			boilerStatus[order - 1].ctemp = ctemp;
			result = NotifyEventToService(BOILER, order, BOILER_RTEMP_EVENT, boilerStatus[order - 1].rtemp, boilerStatus[order - 1].ctemp, 0);
		}

		//현재 온도
		//power on/off  외출설정 /해제 이벤트가 발생하면 boiler_status[order - 1][3] = ctemp 됨 그래서 설정 온도 이벤트는 발생 안 함
		if( boilerStatus[order - 1].ctemp != ctemp)
		{
			boilerStatus[order - 1].rtemp = rtemp;
			boilerStatus[order - 1].ctemp = ctemp;
			result = NotifyEventToService(BOILER, order, BOILER_CTEMP_EVENT, boilerStatus[order - 1].rtemp, boilerStatus[order - 1].ctemp, 0);
		}
	}

	//Main 온도 제어기 제어 요구 ACK command에 대한 처리는 무시 한다.(BOILER_OUT_CTRL_ACK)

	return result;
}

int CMX_Boiler::checkEachAck(int order)
{
	if(boilerStatus[order - 1].isAck == TRUE)
		return TRUE;
	else
		return FALSE;
}

int CMX_Boiler::checkDisconnected()
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

unsigned int CMX_Boiler::getCurrentSupportedCount()
{
	/*
	int order;
	int result;
	for(order = 1; order <= supportedPollingCount; order++)
	{
		result = checkEachAck(order);
		if(result == FALSE)	
			return order - 1;
	}

	return supportedPollingCount;
	*/

	return boilerCnt;
}

