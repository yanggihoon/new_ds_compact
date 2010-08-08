#ifndef CMX_BOILER_H_
#define CMX_BOILER_H_


// User Define Header
#include "DeviceProtocol.h"

// System Header
#include <app.h>

//CMD
#define INDIVIDUAL_BOILER_STATUS_COMMAND			0x01
#define EACH_BOILER_STATUS_COMMAND					0x02
#define INDIVIDUAL_BOILER_CTRL_COMMAND				0x03
#define EACH_BOILER_CTRL_COMMAND						0x04
#define BOILER_OUT_CTRL_COMMAND						0x05
#define BOILER_CTRL_METHOD_COMMAND					0x0F


//ACK
#define INDIVIDUAL_BOILER_STATUS_ACK					0x81
#define EACH_BOILER_STATUS_ACK						0x82
#define INDIVIDUAL_BOILER_CTRL_ACK					0x83
#define EACH_BOILER_CTRL_ACK							0x84
#define BOILER_OUT_CTRL_ACK							0x85
#define BOILER_CTRL_METHOD_ACK						0x8F

#define MAX_SUPPORTED_BOILER_CNT					8

struct BoilerStatus
{
	unsigned int order;	
	unsigned int isAck;
	unsigned int rtemp;
	unsigned int ctemp;
	char deviceCharName[MAX_DEVICENAME_STRING_LENGTH];
	enum BOILER_POWER		power;
	enum BOILER_OUT		out;
};

class CMX_Boiler : public DeviceProtocol
{
	private:
		static CMX_Boiler* uniqueInstance;	
	 	unsigned char buf[8];	

	public:
		static CMX_Boiler* GetInstance();
		CMX_Boiler();
		 ~CMX_Boiler();
		
		void DeviceInit();
		int FrameSend(unsigned char wBuf[]);
		int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
		int FrameRecv(unsigned char rBuf[]);
		int FarmeParser(unsigned char buf[]);
		unsigned int getCurrentSupportedCount();
		int checkEachAck(int order);
		int checkDisconnected();

		unsigned int isMethodPolled;
		unsigned int boilerCnt;
		unsigned char boilerTemperatureMin;
		unsigned char boilerTemperatureMax;
		enum BOILER_MODE	boilerMode;
		enum BOILER_DETAILMODE boilerDetailMode;
		BoilerStatus	boilerStatus[MAX_SUPPORTED_BOILER_CNT];
};

#endif

