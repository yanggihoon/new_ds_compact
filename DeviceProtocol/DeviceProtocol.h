#ifndef _DEVICE_PROTOCOL_H_
#define _DEVICE_PROTOCOL_H_

#define MAX_DEVICENAME_STRING_LENGTH	15
#define MAX_RETRY_SEND_CNT				3

#define USED			1
#define NOT_USED	0

#define SUPPORTED		1
#define NOT_SUPPORTED	0

#define ACK		1
#define NO_ACK	0

#define POLLING_CMD		1
#define CONTROL_CMD	2


/*********************************Property*********************************/

typedef struct 
{
	unsigned char mode;
	unsigned char power;
	unsigned char allPower;		
	unsigned char dimmerLevelFrom;	// 조도 레벨 시작 범위
	unsigned char dimmerLevelTo;	// 조도 레벨 끝 범위
}_LightProperty;

typedef struct 
{
	unsigned char action;
	unsigned char alarm;
}_GasProperty;

typedef struct 
{
	unsigned char power;
	unsigned char readyPower;
	unsigned char out;
	unsigned char gasClose;
	unsigned char elevatorCall;
}_BundleLightProperty;

typedef struct 
{
	unsigned char boilerMode;
	unsigned char dipSwitchMode;
	unsigned char power;
	unsigned char heatingPower;
	unsigned char outMode;
	unsigned char requestTemperature;
	unsigned char requestTemperatureMax;
	unsigned char requestTemperatureMin;
	unsigned char currentTemperature;
	unsigned char currentTemperatureMax;
	unsigned char currentTemperatureMin;
	unsigned char heatingWaterTemperature;
	unsigned char heatingWaterTemperatureMax;
	unsigned char heatingWaterTemperatureMin;
}_BoilerProperty;

typedef struct 
{
	unsigned char action;
	unsigned char angular;
}_CurtainProperty;

typedef struct 
{
	unsigned char sensorDetected;
	unsigned char sensorUndetected;
}_SensorProperty;

typedef struct
{
	unsigned char doPower;
	unsigned char doWindDirection;
	unsigned char doWindPower;
	unsigned char doDrive;
	unsigned char stPower;	
	unsigned char stWindDirection;
	unsigned char stWindPower;
	unsigned char stDrive;
	unsigned char stTracking;
	unsigned char stInstallation;
	unsigned char stInnerRoomError;
	unsigned char stOutRoomError;
}_SystemAirconProperty;

typedef union
{
	_LightProperty lightProperty;
	_GasProperty gasProperty;
	_BundleLightProperty bundleLightProperty;
	_BoilerProperty boilerProperty;
	_CurtainProperty curtainProperty;
	_SensorProperty sensorProperty;
	_SystemAirconProperty systemAirconProperty;
}D_Property;



/*********************************Item*********************************/

typedef struct 
{
	unsigned char power;
	unsigned char dimmingLevel;	
	unsigned char error;
}_LightItem;

typedef struct 
{
	unsigned char action;
	unsigned char error;
}_GasItem;

typedef struct 
{
	unsigned char power;
	unsigned char readyPower;
	unsigned char out;
	unsigned char elevatorCall;
	unsigned char gasClose;
	unsigned char error;	
}_BundleLightItem;

typedef struct 
{
	unsigned char power;
	unsigned char outMode;
	unsigned char requestTemperature;
	unsigned char currentTemperature;
	unsigned char error;
}_BoilerItem;

typedef struct 
{
	unsigned char action;
	unsigned char	angular;
	unsigned char error;
}_CurtainItem;

typedef struct 
{
	unsigned char subAction[8];
	unsigned char error;
}_SensorItem;

typedef struct
{
	unsigned char power;
	unsigned char windDirection;
	unsigned char windPower;
	unsigned char drive;
	unsigned char rtemp;
	unsigned char ctemp;
	unsigned char trackingMode;
	unsigned char error;
}_SystemAirconItem;

typedef union
{
	_LightItem lightItem;
	_GasItem gasItem;
	_BundleLightItem bundleLightItem;
	_BoilerItem boilerItem;
	_CurtainItem curtainItem;
	_SensorItem sensorItem;
}D_Item;


/*********************************BOILER Enum*********************************/
enum BOILER_MODE
{
	INDIVIDUAL_BOILER			=	1,
	EACH_BOILER				=	2
};

enum BOILER_DETAILMODE
{
	HYPOCAUST_BOILER			=	1,
	INNERTEMPERATURE_BOILER	=	2
};

enum BOILER_EVENT
{
	BOILER_POWER_EVENT	=	1,
	BOILER_RTEMP_EVENT	=	2,
	BOILER_CTEMP_EVENT	=	3,
	BOILER_OUT_EVENT		=	4
};

enum BOILER_POWER
{
	BOILER_POWER_NONE	=	0,
	BOILER_POWER_ON		=	1,
	BOILER_POWER_OFF		=	2
};

enum BOILER_OUT
{
	BOILER_OUT_NONE		=	0,
	BOILER_OUT_RELEASE	=	1,
	BOILER_OUT_SET		=	2
};

/*********************************LIGHT Enum*********************************/
enum LIGHT_EVENT
{
	LIGHT_POWER_EVENT 	= 1,
	LIGHT_DIMMING_EVENT 	= 2
};

enum LIGHT_POWER
{
	LIGHT_POWER_NONE		=	0,
	LIGHT_POWER_OFF 		= 	1,
	LIGHT_POWER_ON		= 	2,
	LIGHT_POWER_ALLOFF 	= 	3,
	LIGHT_POWER_ALLON 	= 	4
};

enum LIGHT_MODE
{
	LIGHT_MODE_BINARY		=	1,
	LIGHT_MODE_DIMMABLE	=	2
};

/*********************************GAS Enum*********************************/

enum GAS_ACTION
{
	GAS_NONE		=	0,
	GAS_CLOSE		=	1,
	GAS_OPEN		=	2,
	GAS_LEAK		=	3
};


/*********************************BUNDLELIGHT Enum*********************************/

enum BUNDLELIGHT_EVENT
{
	BUNDLELIGHT_POWER_EVENT 					= 	1,
	BUNDLELIGHT_READYPOWER_EVENT			=	2,
	BUNDLELIGHT_ELEVATORCALL_EVENT			=	3,
	BUNDLELIGHT_OUT_EVENT					=	4,
	BUNDLELIGHT_GASCLOSE_EVENT				=	5,
};

enum BUNDLELIGHT_POWER
{
	BUNDLELIGHT_POWER_NONE		=	0,
	BUNDLELIGHT_POWER_ON		=	1,
	BUNDLELIGHT_POWER_OFF		=	2,
	BUNDLELIGHT_POWER_ALLON		=	3,
	BUNDLELIGHT_POWER_ALLOFF	=	4
};

enum BUNDLELIGHT_READYPOWER
{
	BUNDLELIGHT_READYPOWER_NONE		=	0,
	BUNDLELIGHT_READYPOWER_ON			=	1,
	BUNDLELIGHT_READYPOWER_OFF			=	2,
	BUNDLELIGHT_READYPOWER_ALLON		=	3,
	BUNDLELIGHT_READYPOWER_ALLOFF		=	4
};

enum BUNDLELIGHT_ELEVATORCALL
{
	BUNDLELIGHT_ELEVATORCALL_NONE				=	0,
	BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST		=	1,
	BUNDLELIGHT_ELEVATORCALL_UP_REQUEST		=	2,
	BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS	=	3,
	BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL		=	4
};

enum BUNDLELIGHT_OUT
{
	BUNDLELIGHT_OUT_NONE					=	0,
	BUNDLELIGHT_OUT_REQUEST				=	1,
	BUNDLELIGHT_OUT_REQUEST_SUCCESS	=	2,
	BUNDLELIGHT_OUT_REQUEST_FAIL		=	3,
	BUNDLELIGHT_OUT_SET					=	4,
	BUNDLELIGHT_OUT_CANCEL				=	5
};

enum BUNDLELIGHT_GASCLOSE
{
	BUNDLELIGHT_GASCLOSE_NONE					=	0,
	BUNDLELIGHT_GASCLOSE_REQUEST				=	1,
	BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS		=	2,
	BUNDLELIGHT_GASCLOSE_REQUEST_FAIL			=	3
};

struct BUNDLELIGHT_SUPPORT_FUNCTION
{
	unsigned _none			:1;
	unsigned _gasclose		:1;
	unsigned _out			:1;
	unsigned _power			:1;
	unsigned _readypower	:1;
	unsigned _elevatorcall	:1;
};

/*********************************Curtain Enum*********************************/
enum CURTAIN_EVENT
{
	CURTAIN_ACTION_EVENT		=	1,
};

enum CURTAIN_ACTION
{
	CURTAIN_NONE		=	0,
	CURTAIN_CLOSE		=	1,
	CURTAIN_OPEN		=	2,
	CURTAIN_STOP		=	3
};

/*********************************SensorEmer Enum*********************************/
enum SENSOR_EVENT
{
	SENSOR_ACTION_EVENT		=	1,
};

enum SENSOR_ACTION
{
	SENSOR_NONE			=	0,
	SENSOR_DETECTED		=	1,
	SENSOR_UNDETECTED	=	2
};

/*********************************SystemAircon Enum*********************************/
enum SYSTEMAIRCON_EVENT
{

};

/*
enum SENSOR_ACTION
{

};
*/

/*********************************Category*********************************/
enum DEVICE_PROTOCOL
{
	PROTOCOL_DUMMY 	= 0,
	COMMAX				= 1,
	HAMUN 				= 2,
	NOKSUNG			= 3
};

enum DEVICE_NAME
{
	DEVICE_DUMMY		= 0,
	LIGHT				= 1,	
	GAS					= 2,
	BOILER				= 3,
	BUNDLELIGHT		= 4,
	CURTAIN			= 5,
	SENSOR				= 6,
	SYSTEMAIRCON 		= 7,
	FANSYSTEM			= 8,
	DOORLOCK			= 9
};

enum DEVICE_INTERFACE
{
	NONE			= 0,
	EXT1			= 1,
	EXT2			= 2,
	EXT3			= 3
};

enum DEVICE_ERROR
{
	NO_ERROR						= 0,
	PORT_ERROR						= 1,
	LIGHT_DISCONNECTION			= 2,
	GAS_DISCONNECTION			= 3,
	BOILER_DISCONNECTION			= 4,
	BUNDLELIGHT_DISCONNECTION	= 5,
	CURTAIN_DISCONNECTION		= 6,
	SENSOR_DISCONNECTION			= 7
};

class DeviceProtocol
{
public:
	DeviceProtocol(){};
	virtual ~DeviceProtocol(){};

	virtual void DeviceInit() = 0;

	virtual int FrameSend(unsigned char wBuf[]) = 0;
	virtual int FrameMake(unsigned char cmd_flag, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4) = 0;
	virtual int FrameRecv(unsigned char rBuf[]) = 0;
	virtual int FarmeParser(unsigned char buf[]) = 0;

	virtual unsigned int getCurrentSupportedCount() = 0;
	virtual int checkEachAck(int order) = 0;
	virtual int checkDisconnected() = 0;

	unsigned int supportedPollingCount;	//최초 device.conf로 부터 읽어 오는 count
	unsigned int isAck;

	unsigned int isRecv;			//제어 command 보낸 후 제어 ack 확인 용 flag
	unsigned int retrySendCnt;		//제어 command 실패시 제 전송 count
	
	enum DEVICE_NAME dName;
	enum DEVICE_PROTOCOL pName;
	enum DEVICE_INTERFACE iName;
	enum DEVICE_ERROR	eName;

};
#endif

