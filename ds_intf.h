

namespace cmxDeviceService
{
//gsoap ns service name:	cds
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	http://220.120.109.1/project/soap/kim/wsdl/cds.wsdl
//gsoap ns service location:	http://220.120.109.37:29700
//gsoap ns schema  namespace:	urn:cds

//-------------------------------------------------------------
//	Device, Protocol, Interface
//-------------------------------------------------------------
enum ns__enum_devInfo
{
	 // Controlled Device	
	_dummy			=0,
	_boiler				=1,
	_light				=2,
	_gasValve			=3,	
	_doorLock			=4,
	_outlet				=5,		
	_curtain				=6,
	_airCleaner			=7,
	_bide               		=8,
	_aroma				=9,
	_rf				    	=10,
	_zlplc              		=11,
	_bundleLight		=12,			// 일괄 소등 스위치
	_systemAircon		=13,			// 삼성 시스템 에어컨 : 극동 건설
	_fanSystem			=14,			// 환기 시스템   : 코콤텍
	_aircon				=21,
	_washMachine		=22,
	_dishWashMachine	=23,
	_gasOven			=24,
	_microwave			=25,
	_television			=26,
	_audio            		=27,
	_vtr              			=28,
	_dvd                		=29,
	_refrigerator		=30,
	_kRefrigeator		=31,
	_securitySensor		=32,
	
	// 디바이스가 설정되어 있지 않음
	_sensorEmer,
	_exTerminal,

	//
	 // Controlled Device Protocol
	_protoCommax,
	_protoNokSung, //중국 녹성 부동산 [방범 센서]	
	_protoLnCP,
	_protoSCUBE,
	_protoZ256,
	_protoICROSS,
	_protoSamsung,
	_protoHaMun,

	 // Controlled Device Interface
	_intfPLC,
	_intfRF,
	_intfRS485,
	_intfLAN

};

//-------------------------------------------------------------
//	Device Error
//-------------------------------------------------------------

enum ns__enum_devError
{
	/*
	devError_485_Serial_Port_Open_Error,
	devError_boiler_DisConnect,
	devError_light_DisConnect,
	devError_gas_DisConnect,
	devError_bundleLight_DisConnect,
	devError_systemAircon_DisConnect,
	devError_fanSystem_DisConnect,
	*/

	// 가전 : LnCP
	devError_no,
	devError_232_Serial_Port_Open_Error,  
	devError_lncp_Common_Micom_Error,				        // 마이컴에러
	devError_lncp_Common_Running_Door_Open,                 // 수행 중 문 열림
	devError_lncp_Common_Hardware_Error,                    // 하드웨어 에러
	devError_lncp_Common_Sensor_Error,                      // 센서 에러
	devError_lncp_Common_Motor_Error,                       // 모터 에러
	devError_lncp_Common_Power_Fail,                        // 전원 실패
	devError_lncp_Common_Temperature_Range_Error,           // 설정 온도 범위 초과
	devError_lncp_Common_Device_Preempted_Error,            // 디바이스 선점
	devError_lncp_Common_Device_control_timeout,            // 처리 시간 경과
	devError_lncp_Common_Device_Not_Exist_DD,               // 존재하지 않는 디바이스 디스크립터
	devError_lncp_Common_Device_Nak_Error,			        // 디바이스에서 NAK를 보냄
	devError_lncp_Common_Incorrect_Option_Setting,          // 잘못된 Option을 설정 한 경우
	devError_lncp_Common_Argument_Error,                    // Input Parameter 오류
	
	// 백색가전 통신 에러

	devError_485_Serial_Port_Open_Error,
	devError_TTL_Serial_Port_Open_Error,
	devError_boiler_DisConnect,
	devError_light_DisConnect,
	devError_gas_DisConnect,
	devError_door_DisConnect,
	devError_outlet_DisConnect,
	devError_curtain_DisConnect,
	devError_airCleaner_DisConnect,
	devError_bide_DisConnect,
	devError_aroma_DisConnect,
	devError_rf_DisConnect,
	devError_plc_DisConnect,
	devError_bundleLight_DisConnect,
	devError_systemAircon_DisConnect,
	devError_fanSystem_DisConnect,
	devError_nokSungSensor_DisConnect,	

	devError_aircon_DisConnect,
	devError_washMachine_DisConnect,
	devError_dishWashMachine_DisConnect,
	devError_gasOven_DisConnect,
	devError_microwave_DisConnect,
	devError_television_DisConnect,
	devError_audio_DisConnect,
	devError_vtr_DisConnect,
	devError_refrigerator_DisConnect,
	devError_kRefrigeator_DisConnect,	
	
	// 보일러
	devError_Boiler_control,  // 제어기 에러
	devError_Boiler_drive,    // 조절기 에러
	devError_Boiler_boiler,   // 보일러 에러
	devError_Boiler_eeprom,   // EEPROM 에러

	// 가스 
	devGas_notFound_sensor,
 
	// 아로마
	devError_aroma_catridge1_drive, // 1번 카트리지 구동 에러
	devError_aroma_catridge2_drive, // 2번 카트리지 구동 에러
	devError_aroma_catridge3_drive, // 3번 카트리지 구동 에러
	
	devStatus_no,
};

//-------------------------------------------------------------
//	check Device Connection
//-------------------------------------------------------------
enum ns__enum_device_connect_check
{
	_device_connect = 1,
	_not_device_disconnect = 2,
};

//-------------------------------------------------------------
//	Config & Common Error
//-------------------------------------------------------------
enum ns__enum_config_Common_Error
{
	_not_error				     = 0,
	_not_founded_file            = 1,          // 설정 파일이 없음
	_not_founded_configuration_info   =2,      // 설정 정보가 없음
	_not_founded_mismatch_configuration = 3,   // 설정 정보가 틀림
	_not_supported_function      = 4,          // 기능 지원을 하지 않음
	_supported_function          = 5,          // 기능 지원을 하지 않음
    _not_supported_device        = 6,          // 현재 드라이버에서 디바이스를 지원을 하지 않음
	_device_support              = 7,          // 현재 드라이버에서 디바이스를 지원
	_not_response_control        = 8,          // 제어에 대한 응답이 없음	
	_not_device_use              = 9,         // 디바이스를 사용할 수 없음 :: 디바이스 자체에 물리적인 결함 이 생겼을 경우
};


struct xsd__base64Binary
{
	unsigned char*	__ptr;
	int				__size;
};

//-------------------------------------------------------------
//	root device ( base class )
//-------------------------------------------------------------
class ns__rootDevice
{
	public:	

		enum ns__enum_devInfo	dev;					// Device
		enum ns__enum_devInfo	proto;					// protocol
		enum ns__enum_devInfo	intf;					// physical interface

		unsigned int			order;					// 순번
};


//-------------------------------------------------------------
//	Device Service :: 현재 사용 가능한 디바이스 Category
//-------------------------------------------------------------

struct xsd_devCategory
{
	unsigned _boiler				:1;
	unsigned _light				:1;
	unsigned _gasValve			:1;
	unsigned _doorLock			:1;
	unsigned _outlet				:1;	
	unsigned _curtain				:1;
	unsigned _aroma				:1;
	unsigned _airCleaner			:1;	
	unsigned _bide				:1;		
	unsigned _rf					:1;
	unsigned _plc					:1;
	unsigned _bundleLight			:1;
	unsigned _systemAircon		:1;
	unsigned _fanSystem			:1;
	unsigned _aircon				:1;
	unsigned _washMachine		:1;
	unsigned _dishWashMachine	:1;
	unsigned _gasOven			:1;
	unsigned _microwave			:1;
	unsigned _television			:1;
   	 unsigned _audio				:1;
	unsigned _vtr			   		:1;
	unsigned _dvd				:1;
	unsigned _refrigerator		:1;
	unsigned _kRefrigeator		:1;
	unsigned _securitySensor    	:1;		
};


class ns__deviceCategory 
{
	public:

		// 드라이버에서 지원하는 디바이스 군
		struct xsd_devCategory enable_device_category;  
		
		enum ns__enum_config_Common_Error commonError;	
		
		enum ns__enum_devError boilerDeviceError;           // RS 485 통신 에러
		enum ns__enum_devError lightDeviceError;			// RS 485 통신 에러
		enum ns__enum_devError gasDeviceError;				// RS 485 통신 에러		
		enum ns__enum_devError doorLockDeviceError;         // RS 485 통신 에러
		enum ns__enum_devError acConsentDeviceError;        // RS 485 통신 에러
		enum ns__enum_devError curtainDeviceError;			// RS 485 통신 에러
		enum ns__enum_devError bundleLightDeviceError;		// RS 485 통신 에러
		enum ns__enum_devError fanSystemDeviceError;        // RS 485 통신 에러
		enum ns__enum_devError securitySensorDeviceError;        // RS 485 통신 에러		

		enum ns__enum_devError airconDeviceError;           // RS 232 통신 에러

		enum ns__enum_devError rfDeviceError;               // TTL 통신 에러
		enum ns__enum_devError systemAirconDeviceError;     // RS 485 통신 에러

		enum ns__enum_device_connect_check deviceConnectionCheck;

	enum ns__deviceCategoryFunc
	{

		f_deviceCategory,
		f_deviceCategoryDevError
	}
	func;
};


class ns__getDeviceCategoryItemResponse {ns__deviceCategory _return;};
class ns__getDeviceCategoryResponse {ns__deviceCategory _return;};


int ns__getDeviceCategoryCount(void* _, int* out);
int ns__getDeviceCategoryItem(int order, ns__getDeviceCategoryItemResponse* out);
int ns__getDeviceCategory(ns__deviceCategory in, ns__getDeviceCategoryResponse* out);
int ns__setDeviceCategory(ns__deviceCategory in, class ns__setDeviceCategoryResponse{}* out);

int ns__deviceCategoryEvent(ns__deviceCategory in, int* out);


//-------------------------------------------------------------
//	boiler, 보일러, FunctionCode : 1
//-------------------------------------------------------------
enum ns__enum_boiler
{
	 // 전원 및 난방 설정
	_boilerPower_Off,						// 전원설정_Off
	_boilerPower_On,						// 전원설정_On

	_boilerHeating_Off,                     // 난방설정_Off
	_boilerHeating_On,                      // 난방설정_On

     // 외출 기능 설정
	_boilerOutMode_Set,						// 외출모드_설정
	_boilerOutMode_Release,					// 외출모드_해제

	// 보일러 모드 설정
	_boilerDipSwitchMode_Hypocaust,         // 모드 : 난방온도제어_온돌
	_boilerDipSwitchMode_InnerTemperature,  // 모드 : 난방온도설정_실내온도
	
	// 회사 모델
	
	_model_boiler_Rinnai,				    // 린나이
	_model_boiler_Kiturami,					// 귀뚜라미 보일러
    _model_boiler_KyungDong,				// 경동  
	
	_model_boiler_HansungCisco,				// 한성 시스코
	_model_boiler_HauWell,				    // 하니웰
	_model_boiler_HanEnergy,                // 한 에너지
	_model_boiler_Valcon,                   // 한국 밸콘
	_model_boiler_Kotech                    // 코텍
};


struct xsd_BoilerProperty
{
	unsigned   _indivisualBoiler			 : 1;  //개별보일러
	unsigned   _eachBoiler					 : 1;  //각방 온도 조절기

	unsigned   _boilerPower_Off				 : 1;  //보일러 전원
	unsigned   _boilerPower_On				 : 1;  //보일러 전원

	unsigned   _boilerHeatingPower_Off		 : 1;  //난방 전원		
	unsigned   _boilerHeatingPower_On		 : 1;  //난방 전원

	unsigned   _boilerOutMode_Set			 : 1;
	unsigned   _boilerOutMode_Release		 : 1;

	//보일러 모드
	unsigned _boilerDipSwitchMode_Hypocaust  : 1;
	unsigned _boilerDipSwitchMode_InnerTemperature : 1;	
	
	//보일러 온도
	unsigned boilerRequestedTemperature      : 1;  // 설정 온도 지원가능 여부 판단
	unsigned boilerRequestedTemperatureFrom  : 7;  // 온도 시작 범위 
	unsigned boilerRequestedTemperatureTo    : 7;  // 온도 끝 범위

	unsigned boilerCurrentTemperature        : 1;  // 현재 온도 지원가능 여부 판단
    unsigned boilerCurrentTemperatureFrom    : 7;  // 온도 시작 범위 
	unsigned boilerCurrentTemperatureTo      : 7;  // 온도 끝 범위

	unsigned boilerHeatingWaterTemperature   : 1;   // 온수 온도 지원가능 여부 판단
	unsigned boilerHeatingWaterTemperatureFrom : 7; // 온도 시작 범위 
	unsigned boilerHeatingWaterTemperatureTo   : 7; // 온도 끝 범위

};

class ns__boiler : public ns__rootDevice
{
public:

	struct xsd_BoilerProperty  devBoilerProperty;
	char *						boilerZoneName;                   // 보일러(각방 온도 조절기)위치

	int							boilerRequestedTemperature;	      // 난방 온도설정
	int							boilerCurrentTemperature;	      // 난방 현재온도
	int							boilerHeatingWaterTemperature;	  // 온수 현재온도

	enum ns__enum_boiler		model;						      // 모델명

	enum ns__enum_boiler		boilerPower;				      // 전원설정
	enum ns__enum_boiler		boilerHeatingPower;				  // 난방설정
	enum ns__enum_boiler		boilerOutMode;				      // 외출모드
	enum ns__enum_boiler        boilerDipSwitchMode;		      // 난방 온도제어 모드
	

	enum ns__enum_devError		boilerDevError;				      // 디바이스 에러	
	
	enum ns__boilerFunc
	{
		f_boilerPower,                     // 전원 
		f_boilerHeatingPower,              // 난방
		f_boilerOutMode,                   // 외출 모드
		f_boilerRequestedTemperature,      // 난방 온도 설정
		f_boilerCurrentTemperature,        // 난방 현재 온도
		f_boilerHeatingWaterTemperature,   // 온수 현재 온도
		f_boilerDevError
	}
	func;
};

class ns__getBoilerPropertyItemResponse {ns__boiler _return;};
class ns__getBoilerItemResponse {ns__boiler _return;};
class ns__getBoilerResponse {ns__boiler _return;};

int ns__getBoilerCount(void* _, int* out);
int ns__getBoilerPropertyItem(int order, ns__getBoilerPropertyItemResponse* out);

int ns__getBoilerItem(int order, ns__getBoilerItemResponse* out);
int ns__getBoiler(ns__boiler in, ns__getBoilerResponse* out);
int ns__setBoiler(ns__boiler in, class ns__setBoilerResponse{}* out);

int ns__boilerEvent(ns__boiler in, int* out);


//-------------------------------------------------------------
//	light, 전등,  FunctionCode : 2
//-------------------------------------------------------------


enum ns__enum_light
{
	_lightPower_Off = 0,				// 전등파워_꺼짐
	_lightPower_On = 1,					// 전등파워_켜짐
	_lightPower_AllOff =3,				// 전등파워_전체꺼짐
	_lightPower_AllOn = 4,				// 전등파워_전체켜짐

	_lightSwitch_Binary,				//  ON/OFF 스위치
	_lightSwitch_Dimmable,				//  디밍 스위치

	_model_Light_JoongAngControl = 5,	// 중앙제어
	_model_Light_Clio = 6,		        // 클리오
	_model_Light_Anam = 7,              // 아남
	_model_Light_GoIM = 8,				// GoIM
	_model_Light_PlanetInt = 9,			// PlanetInt
	_model_UnKnown = 10

};

struct xsd_LightProperty
{
	unsigned _lightBinary       : 1;    // On/Off 스위치
	unsigned _lightDimmable     : 1;    // 디밍 스위치

	unsigned _lightPower_Off    : 1;
	unsigned _lightPower_On	    : 1;	

	unsigned _lightPower_AllOff	: 1;		
	unsigned _lightPower_AllOn	: 1;	

	unsigned dimmerLevelFrom	: 4;	// 조도 레벨 시작 범위
	unsigned dimmerLevelTo		: 4;	// 조도 레벨 끝 범위

	
};

class ns__light : public ns__rootDevice
{
	public:
		
		struct xsd_LightProperty    devLightProperty;	
		char*						lightZoneMainName;
		char*						lightZoneSubName;
		int							dimmableLevel;   // 디밍 스위치 조도레벨 :: 범위( 1 - 10 )		

		enum ns__enum_light			model;			// 모델명
		enum ns__enum_light			lightPower;		// 전등파워
		enum ns__enum_light         lightSwitchMode; // Binary Or Dimming Switch
		enum ns__enum_devError		lightDevError;	// 디바이스 에러	

		enum ns__lightFunc
		{
			f_lightPower,	
			f_dimmableLevel,
			f_lightDevError
		}
		func;

		
		
};

class ns__getLightPropertyItemResponse {ns__light _return;};
class ns__getLightItemResponse {ns__light _return;};
class ns__getLightResponse {ns__light _return;};

int ns__getLightCount(void* _, int* out);

int ns__getLightPropertyItem(int order, ns__getLightPropertyItemResponse* out);

int ns__getLightItem(int order, ns__getLightItemResponse* out);
int ns__getLight(ns__light in, ns__getLightResponse* out);
int ns__setLight(ns__light in, class ns__setLightResponse{}* out);

int ns__lightEvent(ns__light in, int* out);

//-------------------------------------------------------------
//	gasValve, 가스밸브,  FunctionCode : 3
//-------------------------------------------------------------
enum ns__enum_gasValve
{
	_gvDo_Close,				// 동작설정_닫힘
	_gvDo_Open,					// 동작설정_열림

	_gvAlarm_On,				// 가스누출경보_울림
	_gvAlarm_Off,				// 가스누출경보_꺼짐

	_model_GasValve_Shinwoo,	// 신우 가스
	_model_GasValve_GoIM,		// 고아이엠
	_model_GasValve_Mat,        // Mat
	_model_GasValve_koreaFire   // 한구 소방
};


struct xsd_gasValveProperty
{
	unsigned   _gvDo_Close  : 1;
	unsigned   _gvDo_Open   : 1;

	unsigned   _gvAlarm_On  : 1;
	unsigned   _gvAlarm_Off : 1;
	
};

class ns__gasValve : public ns__rootDevice
{
public:

	struct xsd_gasValveProperty devGasValveProperty;

	enum ns__enum_gasValve		model;		// 모델명
	enum ns__enum_gasValve		gvDo;		// 동작설정
	enum ns__enum_gasValve		gvAlarm;	// 가스누출경보
	enum ns__enum_devError		gvDevError;	// 디바이스 에러

	enum ns__gasValveFunc
	{
		f_gvDo,
		f_gvAlarm,
		f_gvDevError
	}
	func;
};

class ns__getGasValvePropertyItemResponse {ns__gasValve _return;};
class ns__getGasValveItemResponse {ns__gasValve _return;};
class ns__getGasValveResponse {ns__gasValve _return;};

int ns__getGasValveCount(void* _, int* out);

int ns__getGasValvePropertyItem(int order, ns__getGasValvePropertyItemResponse * out);

int ns__getGasValveItem(int order, ns__getGasValveItemResponse * out);
int ns__getGasValve(ns__gasValve in, ns__getGasValveResponse* out);
int ns__setGasValve(ns__gasValve in, class ns__setGasValveResponse{}* out);

int ns__gasValveEvent(ns__gasValve in, int* out);


//-------------------------------------------------------------
//	curtain, 커튼, FunctionCode : 6
//-------------------------------------------------------------
enum ns__enum_curtain
{
	_curtainDo_Close,			// 커튼값_닫힘
	_curtainDo_Open,			// 커튼값_열림
	_curtainDo_Stop,			// 커튼값_정지

	_model_curtain_PlanetInt,   // 플레넷
	_model_curtain_CLIO,			// 클리오 
	_model_curtain_HaMun		// 하문
};

struct xsd_curtainProperty
{
	unsigned   _curtainDo_Close  : 1;
	unsigned   _curtainDo_Open   : 1;	
	unsigned   _curtainDo_Stop   : 1;
	unsigned   _curtainAngular   : 9;
	
};

class ns__curtain : public ns__rootDevice
{
public:

	struct xsd_curtainProperty devCurtainProperty;
	enum ns__enum_curtain		model;				// 모델명
	enum ns__enum_curtain		curtainDo;			// 커튼값
	enum ns__enum_devError		curtainDevError;	// 디바이스 에러

	int curtainAngular;							    // 커튼 각도

	enum ns__curtainFunc
	{
		f_curtainDo,
	    f_curtainAngular,
		f_curtainDevError
	}
	func;
};

class ns__getCurtainPropertyItemResponse {ns__curtain _return;};
class ns__getCurtainItemResponse {ns__curtain _return;};
class ns__getCurtainResponse {ns__curtain _return;};

int ns__getCurtainCount(void* _, int* out);

int ns__getCurtainPropertyItem(int order, ns__getCurtainPropertyItemResponse* out);
int ns__getCurtainItem(int order, ns__getCurtainItemResponse* out);
int ns__getCurtain(ns__curtain in, ns__getCurtainResponse* out);
int ns__setCurtain(ns__curtain in, class ns__setCurtainResponse{}* out);

int ns__curtainEvent(ns__curtain in, int* out);



//-------------------------------------------------------------
//	일괄소등 스위치[Function Code] Function Code : 12
//-------------------------------------------------------------
enum ns__enum_bundleLight
{
	_bundleLightPower_Off,		     // 일괄 전등 스위치_꺼짐
	_bundleLightPower_On,			 // 일괄 전등 스위치_켜짐
	_bundleLightPower_AllOff,		 // 일괄 전등 스위치_전체꺼짐
	_bundleLightPower_AllOn,		 // 일괄 전등 스위치_전체켜짐

	_readyEnergyPower_Off,			 // 대기 전력 스위치_꺼짐
	_readyEnergyPower_On,			 // 대기 전력 스위치_켜짐
	_readyEnergyPower_AllOff,		 // 대기 전력 스위치_전체꺼짐
	_readyEnergyPower_AllOn,		 // 대기 전력 스위치_전체켜짐

	_bundleLightExit_Notify,		 // 외출 통보
	_bundleLightExit_Entry_Accept,	 // 외출 진입 허가 [ DS로 부터 외출 설정 통보시 월패드에서 외출 허락/거부를 DS로 알려 줘야함
	_bundleLightExit_Entry_Deny,     // 외출 진입 불가 
	_bundleLightExit_Setting,        // 외출 설정
	_bundleLightExit_Release,        // 외출 해제 

	_bundleLightElevator_Up,         // 엘리베이터 호출 : Up
	_bundleLightElevator_Down,       // 엘리베이터 호출 : Down
	_bundleLightElevator_Fail,       // 엘리베이터 호출 : Fail    (엘리베이터 호출 실패를 하거나, L/S와 연결이되지 않을 경우 호출 ]
	_bundleLightElevator_Success,    // 엘리베이터 호출 : Success (엘리베이터 호출 성공 )
	_gasValve_Entry_Accept,
	_gasValve_Entry_Deny,

	_model_BundleLight_icom,	     //  아성 
	_model_BundleLight_Clio,	     //  클리오
	_model_BundleLight_Anam,	     //  아남	
	_model_BundleLight_Speel		//  스필


};	

struct xsd_bundleLightProperty
{
	unsigned   _bundleLightPower_Off			: 1;	
	unsigned   _bundleLightPower_On				: 1;	
	unsigned   _bundleLightPower_AllOff			: 1;	
	unsigned   _bundleLightPower_AllOn			: 1;	

	unsigned  _readyEnergyPower_Off				: 1;  	 // 대기 전력 스위치_꺼짐
	unsigned  _readyEnergyPower_On				: 1;		 // 대기 전력 스위치_켜짐
	unsigned  _readyEnergyPower_AllOff			: 1;		 // 대기 전력 스위치_전체꺼짐
	unsigned  _readyEnergyPower_AllOn			: 1;		 // 대기 전력 스위치_전체켜짐

	unsigned   _bundleLightExit_Notify			: 1;		
	unsigned   _bundleLightExit_Entry_Accept    : 1;	 // 외출 진입 허가 [ DS로 부터 외출 설정 통보시 월패드에서 외출 허락/거부를 DS로 알려 줘야함
	unsigned   _bundleLightExit_Entry_Deny      : 1;     // 외출 진입 불가 
	unsigned   _bundleLightExit_Setting         : 1;     // 외출 설정
	unsigned   _bundleLightExit_Release         : 1;     // 외출 해제 

	unsigned   _bundleLightElevator_Up          : 1;
	unsigned   _bundleLightElevator_Down        : 1;

	unsigned   _gasValve_Entry_Accept			: 1;
	unsigned   _gasValve_Entry_Deny				: 1;

};


class ns__bundleLight : public ns__rootDevice
{
public:

	struct xsd_bundleLightProperty devBundleLightProperty;

	enum ns__enum_bundleLight			model;					// 모델명
	enum ns__enum_bundleLight			bundleLightPower;		// 일괄소등 스위치 전원
	enum ns__enum_bundleLight			bundleLightExit;		// 일괄소등 외출
	enum ns__enum_bundleLight			readyEnergyPower;		// 대기 전력 전원
	enum ns__enum_bundleLight			gasValve;				// 가스 진입 불가/ 허가
	enum ns__enum_bundleLight			bundleLightElevator;	// 엘리베이터 Up/Down

	enum ns__enum_devError			    bundleLightDevError;	// 디바이스 에러		

	enum ns__bundleLightFunc
	{
		f_bundleLightPower,		
		f_bundleLightExit,
		f_readyEnergyPower,
		f_gasValve,
		f_bundleLightElevator,
		f_bundleLightDevError
	}
	func;
};

class ns__getBundleLightPropertyItemResponse {ns__bundleLight _return;};
class ns__getBundleLightItemResponse {ns__bundleLight _return;};
class ns__getBundleLightResponse {ns__bundleLight _return;};

int ns__getBundleLightCount(void* _, int* out);

int ns__getBundleLightPropertyItem(int order, ns__getBundleLightPropertyItemResponse* out);
int ns__getBundleLightItem(int order, ns__getBundleLightItemResponse* out);
int ns__getBundleLight(ns__bundleLight in, ns__getBundleLightResponse* out);
int ns__setBundleLight(ns__bundleLight in, class ns__setBundleLightResponse{}* out);

int ns__BundleLightEvent(ns__bundleLight in, int* out);

//-------------------------------------------------------------
//	시스템 에어컨[Function Code] Function Code : 13
//-------------------------------------------------------------
enum ns__enum_systemAircon
{
	_systemAirconDo_Power_Off,							// 시스템 에어컨 전원_꺼짐
	_systemAirconDo_Power_On,							// 시스템 에어컨 전원_켜짐
	_systemAirconDo_Power_AllOff,						// 시스템 에어컨 전원_전체꺼짐
	_systemAirconDo_Power_AllOn,						// 시스템 에어컨 전원_전체켜짐
	
	_systemAirconDo_WindDirection_UpDown,				// 시스템 에어컨 풍향 상하
	_systemAirconDo_WindDirection_LeftRight,			// 시스템 에어컨 풍향 좌우
	_systemAirconDo_WindDirection_UpDownLeftRight,			// 시스템 에어컨 풍향 상하좌우
	_systemAirconDo_WindDirection_Stop,				// 시스템 에어컨 풍향 멈춤

	_systemAirconDo_Drive_Auto,							// 시스템 에어컨  운전 자동				
	_systemAirconDo_Drive_Cooling,						// 시스템 에어컨  운전 냉방
	_systemAirconDo_Drive_Humidity,						// 시스템 에어컨  운전 제습
	_systemAirconDo_Drive_Ventilation,					// 시스템 에어컨  운전 송풍
	_systemAirconDo_Drive_Heating,						// 시스템 에어컨  운전 난방

	_systemAirconDo_WindPower_Auto,					    // 시스템 에어컨  자동
	_systemAirconDo_WindPower_Breeze,					// 시스템 에어컨  미풍
	_systemAirconDo_WindPower_Middle,					// 시스템 에어컨  약풍
	_systemAirconDo_WindPower_Super	,				    // 시스템 에어컨  강풍
	_systemAirconDo_WindPower_Natural,				    // 시스템 에어컨  자연풍

	_systemAirconDo_StatusQuery_Start,					// 상태 요청 시작
	_systemAirconDo_StatusQuery_Stop,                   // 상태 요청 중지

	_systemAirconSt_Power_Start,						    // 시스템 에어컨 전원_꺼짐
	_systemAirconSt_Power_Stop,						    // 시스템 에어컨 전원_켜짐
	
	_systemAirconSt_WindDirection_UpDown,				// 시스템 에어컨 풍향 상하
	_systemAirconSt_WindDirection_LeftRight,			    // 시스템 에어컨 풍향 좌우
	_systemAirconSt_WindDirection_UpDownLeftRight,      // 시스템 에어컨 풍향 상하좌우
	_systemAirconSt_WindDirection_Stop,				    // 시스템 에어컨 풍향 멈춤
	
	_systemAirconSt_WindPower_Auto,					    // 시스템 에어컨  자동
	_systemAirconSt_WindPower_Breeze,					// 시스템 에어컨  미풍
	_systemAirconSt_WindPower_Middle,					// 시스템 에어컨  약풍
	_systemAirconSt_WindPower_Super,					    // 시스템 에어컨  강풍
	_systemAirconSt_WindPower_Natural,				    // 시스템 에어컨  자연풍

	_systemAirconSt_Drive_Driving,					    // 시스템 에어컨  실내운전 : 운전
	_systemAirconSt_Drive_EventRelease,				    // 시스템 에어컨  실내운전 : Event 해제
	_systemAirconSt_Drive_Auto,						    // 시스템 에어컨  실내운전 : 자동
	_systemAirconSt_Drive_JaeSang,					    // 시스템 에어컨  실내운전 : 제상
	_systemAirconSt_Drive_Ventilation,				    // 시스템 에어컨  실내운전 : 송풍
	_systemAirconSt_Drive_Humidity,					    // 시스템 에어컨  실내운전 : 제습
	_systemAirconSt_Drive_Cooling,					    // 시스템 에어컨  실내운전 : 냉방
	_systemAirconSt_Drive_Heating,					    // 시스템 에어컨  실내운전 : 난방

	_systemAirconSt_Tracking_Success,                   // 시스템 에어컨  트랙킹 : 성공
	_systemAirconSt_Tracking_Fail,	                    // 시스템 에어컨  트랙킹 : 실패
	_systemAirconSt_Tracking_Searching,                 // 시스템 에어컨  트랙킹 : 검색
	_systemAirconSt_Tracking_End,		                // 시스템 에어컨  트랙킹 : 완료
	_systemAirconSt_Installation_Not_Exist,             // 시스템 에어컨  실내기 : 미설치

	_systemAirconSt_InnerRoom_Error_Normal,				// 실내기 정상
	_systemAirconSt_InnerRoom_Error_InnerTemperature,	// 시스템 에어컨  실내기 에러 : 실내 온도 센서 에러
	_systemAirconSt_InnerRoom_Error_EVA_IN,       	    // 시스템 에어컨  실내기 에러 : 실내기 EVA_IN 에러
	_systemAirconSt_InnerRoom_Error_FAN,				    // 시스템 에어컨  실내기 에러 : 실내기 FAN 에러
	_systemAirconSt_InnerRoom_Error_Communication,	    // 시스템 에어컨  실내기 에러 : 실내기 통싱 에러
	_systemAirconSt_InnerRoom_Error_Floating_Switch,    // 시스템 에어컨  실내기 에러 : Floating 스위치 에러
	_systemAirconSt_InnerRoom_Error_Open_Two,			// 시스템 에어컨  실내기 에러 : 전동 변 열리 검지 에러
	_systemAirconSt_InnerRoom_Error_EVA_OUT,	        // 시스템 에어컨  실내기 에러 : 실내기 EVA_OUT 에러

	_systemAirconSt_OutRoom_Error_Normal,				// 실외기 정상
	_systemAirconSt_OutRoom_Error_Temp,				    // 시스템 에어컨  실내기 에러 :  실외기 온도 센서 에러
	_systemAirconSt_OutRoom_Error_Sensor,				// 시스템 에어컨  실내기 에러 :  토출 센서 에러
	_systemAirconSt_OutRoom_Error_Cond,				    // 시스템 에어컨  실내기 에러 :  COND 배관센서 에러
	_systemAirconSt_OutRoom_Error_Commnuication,	    // 시스템 에어컨  실내기 에러 :  통신 에러
	_systemAirconSt_OutRoom_Error_Error,				// 시스템 에어컨  실내기 에러 :  에러 발생
	_systemAirconSt_OutRoom_Error_lowPower,			    // 시스템 에어컨  실내기 에러 :  저압센서 에러
	_systemAirconSt_OutRoom_Error_MagicalPower,		    // 시스템 에어컨  실내기 에러 :  마력설정 에러
	_systemAirconSt_OutRoom_Error_InitCommunication,	// 시스템 에어컨  실내기 에러 :  초기 통신 에러	

	_model_systemAircon_SamSung,						//  삼성 시스템 에어컨
	

};	

struct xsd_systemAirconProperty
{
	unsigned   _systemAirconDo_Power_Start						: 1;	 // 시스템 에어컨 전원_꺼짐
	unsigned   _systemAirconDo_Power_Stop						: 1;	 // 시스템 에어컨 전원_켜짐
	unsigned   _systemAirconDo_Power_AllRunning				    : 1;	 // 시스템 에어컨 전원_전체꺼짐
	unsigned   _systemAirconDo_Power_AllStop					: 1;	 // 시스템 에어컨 전원_전체켜짐

	unsigned   _systemAirconDo_WindDirection_UpDown				: 1;     // 시스템 에어컨 풍향 상하
	unsigned   _systemAirconDo_WindDirection_LeftRight			: 1;     // 시스템 에어컨 풍향 좌우
	unsigned   _systemAirconDo_WindDirection_UpDownLeftRight    : 1;     // 시스템 에어컨 풍향 상하좌우
	unsigned   _systemAirconDo_WindDirection_Stop				: 1;     // 시스템 에어컨 풍향 멈춤
	
	unsigned   _systemAirconDo_WindPower_Auto					: 1;     // 시스템 에어컨  자동
	unsigned   _systemAirconDo_WindPower_Breeze					: 1;     // 시스템 에어컨  미풍
	unsigned   _systemAirconDo_WindPower_Middle					: 1;     // 시스템 에어컨  약풍
	unsigned   _systemAirconDo_WindPower_Super					: 1;     // 시스템 에어컨  강풍
	unsigned   _systemAirconDo_WindPower_Natural				: 1;     // 시스템 에어컨  자연풍

	unsigned   _systemAirconDo_Drive_Auto						: 1;     // 시스템 에어컨   자동
	unsigned   _systemAirconDo_Drive_Cool						: 1;     // 시스템 에어컨   냉방
	unsigned   _systemAirconDo_Drive_Humidity					: 1;     // 시스템 에어컨   제습
	unsigned   _systemAirconDo_Drive_Ventilation				: 1;     // 시스템 에어컨   송풍
	unsigned   _systemAirconDo_Drive_Heating					: 1;     // 시스템 에어컨   난방

	unsigned   _systemAirconSt_Power_Start						: 1;	 // 시스템 에어컨 전원_꺼짐
	unsigned   _systemAirconSt_Power_Stop						: 1;	 // 시스템 에어컨 전원_켜짐
	
	unsigned   _systemAirconSt_WindDirection_UpDown				: 1;     // 시스템 에어컨 풍향 상하
	unsigned   _systemAirconSt_WindDirection_LeftRight			: 1;     // 시스템 에어컨 풍향 좌우
	unsigned   _systemAirconSt_WindDirection_UpDownLeftRight    : 1;     // 시스템 에어컨 풍향 상하좌우
	unsigned   _systemAirconSt_WindDirection_Stop				: 1;     // 시스템 에어컨 풍향 멈춤
	
	unsigned   _systemAirconSt_WindPower_Auto					: 1;     // 시스템 에어컨  자동
	unsigned   _systemAirconSt_WindPower_Breeze					: 1;     // 시스템 에어컨  미풍
	unsigned   _systemAirconSt_WindPower_Middle					: 1;     // 시스템 에어컨  약풍
	unsigned   _systemAirconSt_WindPower_Super					: 1;     // 시스템 에어컨  강풍
	unsigned   _systemAirconSt_WindPower_Natural				: 1;     // 시스템 에어컨  자연풍

	unsigned   _systemAirconSt_Drive_Driving					: 1;     // 시스템 에어컨  실내운전 : 운전
	unsigned   _systemAirconSt_Drive_EventRelease				: 1;     // 시스템 에어컨  실내운전 : Event 해제
	unsigned   _systemAirconSt_Drive_Auto						: 1;     // 시스템 에어컨  실내운전 : 자동
	unsigned   _systemAirconSt_Drive_JaeSang					: 1;     // 시스템 에어컨  실내운전 : 제상
	unsigned   _systemAirconSt_Drive_Ventilation				: 1;     // 시스템 에어컨  실내운전 : 송풍
	unsigned   _systemAirconSt_Drive_Humidity					: 1;     // 시스템 에어컨  실내운전 : 제습
	unsigned   _systemAirconSt_Drive_Cooling					: 1;     // 시스템 에어컨  실내운전 : 냉방
	unsigned   _systemAirconSt_Drive_Heating					: 1;     // 시스템 에어컨  실내운전 : 난방

	unsigned   _systemAirconSt_Tracking_Success                 : 1;     // 시스템 에어컨  트랙킹 : 성공
	unsigned   _systemAirconSt_Tracking_Fail	                : 1;     // 시스템 에어컨  트랙킹 : 실패
	unsigned   _systemAirconSt_Tracking_Searching               : 1;     // 시스템 에어컨  트랙킹 : 검색
	unsigned   _systemAirconSt_Tracking_End		                : 1;     // 시스템 에어컨  트랙킹 : 완료
 
	unsigned   _systemAirconSt_Installation_Not_Exist           : 1;	 // 시스템 에어컨  실내기 : 미설치
 
	unsigned   _systemAirconSt_InnerRoom_Error_Normal           : 1;
	unsigned   _systemAirconSt_InnerRoom_Error_InnerTemperature	: 1;     // 시스템 에어컨  실내기 에러 : 실내 온도 센서 에러
	unsigned   _systemAirconSt_InnerRoom_Error_EVA_IN       	: 1;     // 시스템 에어컨  실내기 에러 : 실내기 EVA_IN 에러
	unsigned   _systemAirconSt_InnerRoom_Error_FAN				: 1;     // 시스템 에어컨  실내기 에러 : 실내기 FAN 에러
	unsigned   _systemAirconSt_InnerRoom_Error_Communication	: 1;     // 시스템 에어컨  실내기 에러 : 실내기 통신 에러
	unsigned   _systemAirconSt_InnerRoom_Error_Floating_Switch  : 1;     // 시스템 에어컨  실내기 에러 : Floating 스위치 에러
	unsigned   _systemAirconSt_InnerRoom_Error_Open_Two			: 1;     // 시스템 에어컨  실내기 에러 : 전동 변 열리 검지 에러
	unsigned   _systemAirconSt_InnerRoom_Error_EVA_OUT	        : 1;     // 시스템 에어컨  실내기 에러 : 실내기 EVA_OUT 에러

	unsigned   _systemAirconSt_OutRoom_Error_Normal             : 1;
	unsigned   _systemAirconSt_OutRoom_Error_Temp				: 1;     // 시스템 에어컨  실외기 에러 :  실외기 온도 센서 에러
	unsigned   _systemAirconSt_OutRoom_Error_Sensor				: 1;     // 시스템 에어컨  실외기 에러 :  토출 센서 에러
	unsigned   _systemAirconSt_OutRoom_Error_Cond				: 1;     // 시스템 에어컨  실외기 에러 :  COND 배관센서 에러
	unsigned   _systemAirconSt_OutRoom_Error_Commnuication		: 1;     // 시스템 에어컨  실외기 에러 :  통신 에러
	unsigned   _systemAirconSt_OutRoom_Error_Error				: 1;     // 시스템 에어컨  실외기 에러 :  에러 발생
	unsigned   _systemAirconSt_OutRoom_Error_lowPower			: 1;     // 시스템 에어컨  실외기 에러 :  저압센서 에러
	unsigned   _systemAirconSt_OutRoom_Error_MagicalPower		: 1;     // 시스템 에어컨  실외기 에러 :  마력설정 에러
	unsigned   _systemAirconSt_OutRoom_Error_InitCommunication	: 1;     // 시스템 에어컨  실외기 에러 :  초기 통신 에러

};


class ns__systemAircon : public ns__rootDevice
{
public:

	struct xsd_systemAirconProperty devsystemAirconProperty;

	enum ns__enum_systemAircon			model;								// 모델명
	enum ns__enum_systemAircon			systemAirconDo_Power;				// 시스템 에어컨 운전 / 정지
	enum ns__enum_systemAircon          systemAirconDo_WindDirection;		// 풍향	
	enum ns__enum_systemAircon          systemAirconDo_WindPower;			// 풍량
	enum ns__enum_systemAircon          systemAirconDo_Drive;				// 운전 선택
	enum ns__enum_systemAircon			systemAirconDo_StatusQuery;			// 사운드까 찌그러지는 현상 발생 : 평상시에는 상태 요청을 하지 않음

	enum ns__enum_systemAircon          systemAirconSt_Power;				 // 시스템 에어컨 운전 / 정지
	enum ns__enum_systemAircon          systemAirconSt_WindDirection;		 // 풍향	
	enum ns__enum_systemAircon          systemAirconSt_WindPower;			 // 풍량	
	enum ns__enum_systemAircon          systemAirconSt_Drive;				 // 실내 운전	
	enum ns__enum_systemAircon          systemAirconSt_Tracking;             // 트래킹
	enum ns__enum_systemAircon          systemAirconSt_Installation;         // 실내기 설치 여부 확인
	enum ns__enum_systemAircon          systemAirconSt_InnerRoom_Error;		 // 실내기 에러
	enum ns__enum_systemAircon          systemAirconSt_OutRoom_Error;		 // 실외기 에러
	int									systemAirconDo_Temperature_Set;      // 설정 온도
	int									systemAirconSt_Temperature_Set;      // 설정 온도
	int									systemAirconSt_Temperature_Current;  // 현재 온도
	enum ns__enum_devError			    systemAirconDevError;	             // 디바이스 에러		

	enum ns__systemAirconFunc
	{
		f_systemAirconDo_Power,
		f_systemAirconDo_WindDirection,
		f_systemAirconDo_WindPower,
		f_systemAirconDo_Temperature,
		f_systemAirconDo_Drive,

		f_systemAirconDo_StatusQuery,

		f_systemAirconSt_Power,
		f_systemAirconSt_WindDirection,
		f_systemAirconSt_WindPower,
		f_systemAirconSt_Temperature_Set,
		f_systemAirconSt_Temperature_Current,
		f_systemAirconSt_Drive,
		f_systemAirconSt_Tracking,
		f_systemAirconSt_InnerRoom_Error,
		f_systemAirconSt_OutRoom_Error,
		f_systemAirconSt_Installation,

		f_systemAirconDevError
	}
	func;
};

class ns__getSystemAirconPropertyItemResponse {ns__systemAircon _return;};
class ns__getSystemAirconItemResponse {ns__systemAircon _return;};
class ns__getSystemAirconResponse {ns__systemAircon _return;};

int ns__getSystemAirconCount(void* _, int* out);

int ns__getSystemAirconPropertyItem(int order, ns__getSystemAirconPropertyItemResponse* out);
int ns__getSystemAirconItem(int order, ns__getSystemAirconItemResponse* out);
int ns__getSystemAircon(ns__systemAircon in, ns__getSystemAirconResponse* out);
int ns__setSystemAircon(ns__systemAircon in, class ns__setSystemAirconResponse{}* out);

int ns__systemAirconEvent(ns__systemAircon in, int* out);


//-------------------------------------------------------------
//	환기 시스템[Function Code] Function Code : 14 
//-------------------------------------------------------------
enum ns__enum_fanSystem
{
	_fanSystemDo_Power_Off,							 // 환기 시스템 : 운전 정지
	_fanSystemDo_Power_On,							 // 환기 시스템 : 운전 정지
	_fanSystemDo_AutoFan_ElectricHeat,				 // 자동환기 모드 -> 전열 교환
	_fanSystemDo_AutoFan_GeneralFan,                 // 자동환기 모드 -> 일반 환기

    _fanSystemDo_FanMode_ElectricHeat,               // 환기 모드     -> 전열 교환
	_fanSystemDo_FanMode_GeneralFan,                 // 환기 모드     -> 일반 환기
	_fanSystemDo_FanMode_Schedule,                   // 환기 모드     -> 스케쥴
	_fanSystemDo_FanMode_SleepingMode,               // 환기 모드     -> 취침모드

	_fanSystemDo_Wind_Off,						     // 풍량 꺼짐
	_fanSystemDo_Wind_Low,					         // 풍량 약
	_fanSystemDo_Wind_Middle,					     // 풍량 중							
	_fanSystemDo_Wind_High,						     // 풍얄 강

	_fanSystemDo_ReservationSet_Set,				 // 꺼짐 예약 설정
	_fanSystemDo_ReservationSet_Release,			 // 꺼짐 예약 해제
	_fanSystemDo_ReservationSet_Running,			 // 꺼짐 예약 동작중
	_fanSystemDo_ReservationSet_NotRunning,			 // 꺼짐 예약 동작 안함

	_fanSystemSt_Power_Off,				             // 환기 시스템 : 운전 정지
	_fanSystemSt_Power_On,					         // 환기 시스템 : 운전 정지

	_fanSystemSt_AutoFan_ElectricHeat,               // 자동환기 모드 -> 전열 교환
	_fanSystemSt_AutoFan_GeneralFan,                 // 자동환기 모드 -> 일반 환기
	_fanSystemSt_CurrentFanMode_AutoFan,			 // 환기 모드 (자동)

    _fanSystemSt_FanMode_ElectricHeat,               // 환기 모드     -> 전열 교환
	_fanSystemSt_FanMode_GeneralFan,                 // 환기 모드     -> 일반 환기
	_fanSystemSt_FanMode_Schedule,                   // 환기 모드     -> 스케쥴
	_fanSystemSt_FanMode_SleepingMode,               // 환기 모드     -> 취침모드
	_fanSystemSt_CurrentFanMode_Manual,		         // 환기 모드(수동)

	_fanSystemSt_Wind_Off,						     // 풍량 꺼짐
	_fanSystemSt_Wind_Low,					         // 풍량 약
	_fanSystemSt_Wind_Middle,					     // 풍량 중							
	_fanSystemSt_Wind_High,						     // 풍얄 강

	
	_fanSystemSt_FanReservationSetMode_Hour,		 // 꺼짐 예약이 동작 중 일 때 시 을 식별 하기 위함	
	_fanSystemSt_FanReservationSetMode_Minute,       // 꺼짐 예약이 동작 중 일 때 분 식별 하기 위함

	_fanSystemSt_ReservationSet_Set,				  // 꺼짐 예약 설정
	_fanSystemSt_ReservationSet_Release,			  // 꺼짐 예약 해제
	_fanSystemSt_ReservationSet_Running,			  // 꺼짐 예약 동작중
	_fanSystemSt_ReservationSet_NotRunning,			  // 꺼짐 예약 동작 안함

	_fanSystemSt_SensorMode_NotExistSenosr,           // 센서 모드 : 센서 없음
	_fanSystemSt_SensorMode_GasSenosr,				  // 센서 모드 : 가스 센서
	_fanSystemSt_SensorMode_CO2Senosr,			      // 센서 모드 : CO2센서

    _fanSystemSt_DamperMode_NotExist,		  	      // 댐퍼 모드 : 없음
	_fanSystemSt_DamperMode_Exist,		              // 댐퍼 모드 : 있음

	_fanSystemSt_FilterEmpressMode_Normal,    	      // 필터/여제 : 정상
    _fanSystemSt_FilterEmpressMode_Clean,    	      // 필터/여제 : 청소
	_fanSystemSt_FilterEmpressMode_Change,            // 필터/여제 : 교환
	_fanSystemSt_FilterEmpressMode_FEChange,          // 필터/여제 : 여제 교환

	_fanSystemSt_AlarmMode_Normal, 
	_fanSystemSt_AlarmMode_HighSpeedMotor,            // 알람 상태 : 급기 모터
	_fanSystemSt_AlarmMode_VentilationMotor,          // 알람 상태 : 배기 모터
	_fanSystemSt_AlarmMode_DamperMotor,               // 알람 상태 : 댐퍼 모터
	_fanSystemSt_AlarmMode_CO2,                       // 알람 상태 : CO2
	_fanSystemSt_AlarmMode_TemperatureMotor,          // 알람 상태 : 온도 센서
	_fanSystemSt_AlarmMode_OverHeating,               // 알람 상태 : 과열 방지
	_fanSystemSt_AlarmMode_DeviceProtect,             // 알람 상태 : 장치 보호

	_fanSystemSt_CO2_DensityMode_CO2High,             // CO2농도 상  
	_fanSystemSt_CO2_DensityMode_CO2Low,              // CO2농도 하


	_fanSystemSt_GAS_Density_Low,                     // 가스 농도 : 약
	_fanSystemSt_GAS_Density_Middle,				  // 가스 농도 : 중
	_fanSystemSt_GAS_Density_High,					  // 가스 농도 : 강
	
	_model_fanSystem_KoKomTech,	     // 코콤텍	

};	

struct xsd_fanSystemProperty
{
	unsigned   _fanSystemDo_Power_Off                  : 1;	
	unsigned   _fanSystemDo_Power_On                   : 1;	
	unsigned   _fanSystemDo_AutoFan_ElectricHeat       : 1;		// 자동환기 모드 -> 전열 교환
	unsigned   _fanSystemDo_AutoFan_GeneralFan         : 1;		// 자동환기 모드 -> 일반 환기

    unsigned   _fanSystemDo_FanMode_ElectricHeat       : 1;		// 환기 모드     -> 전열 교환
	unsigned   _fanSystemDo_FanMode_GeneralFan         : 1;		// 환기 모드     -> 일반 환기
	unsigned   _fanSystemDo_FanMode_Schedule           : 1;		// 환기 모드     -> 스케쥴
	unsigned   _fanSystemDo_FanMode_SleepingMode       : 1;		// 환기 모드     -> 취침모드

	unsigned   _fanSystemDo_Wind_Off				   : 1;		// 풍량 꺼짐
	unsigned   _fanSystemDo_Wind_Low				   : 1;		// 풍량 약
	unsigned   _fanSystemDo_Wind_Middle				   : 1;		// 풍량 중							
	unsigned   _fanSystemDo_Wind_High				   : 1;		// 풍얄 강

	unsigned   _fanSystemDo_ReservationSet			   : 1;		// 꺼짐 예약 설정 모드
	unsigned   _fanSystemDo_ReservationSet_Release	   : 1;		// 꺼짐 예약 해제
	unsigned   _fanSystemDo_ReservationSet_Running	   : 1;		// 꺼짐 예약 동작중
	unsigned   _fanSystemDo_ReservationSet_NotRunning  : 1;		// 꺼짐 예약 동작 안함

	unsigned   _fanSystemSt_Power_Off                  : 1;	
	unsigned   _fanSystemSt_Power_On                   : 1;	
	unsigned   _fanSystemSt_AutoFan_ElectricHeat       : 1;		// 자동환기 모드 -> 전열 교환
	unsigned   _fanSystemSt_AutoFan_GeneralFan         : 1;		// 자동환기 모드 -> 일반 환기
	unsigned   _fanSystemSt_CurrentFanMode_AutoFan	   : 1;     // 환기 모드(자동)

    unsigned   _fanSystemSt_FanMode_ElectricHeat       : 1;		// 환기 모드     -> 전열 교환
	unsigned   _fanSystemSt_FanMode_GeneralFan         : 1;		// 환기 모드     -> 일반 환기
	unsigned   _fanSystemSt_FanMode_Schedule           : 1;		// 환기 모드     -> 스케쥴
	unsigned   _fanSystemSt_FanMode_SleepingMode       : 1;		// 환기 모드     -> 취침모드

	unsigned   _fanSystemSt_CurrentFanMode_Manual      : 1;     // 환기 모드(수동)

	unsigned   _fanSystemSt_Wind_Off				   : 1;		// 풍량 꺼짐
	unsigned   _fanSystemSt_Wind_Low				   : 1;		// 풍량 약
	unsigned   _fanSystemSt_Wind_Middle				   : 1;		// 풍량 중							
	unsigned   _fanSystemSt_Wind_High				   : 1;		// 풍량 강

	unsigned   _fanSystemSt_ReservationSet			   : 1;		// 꺼짐 예약 설정 모드
	unsigned   _fanSystemSt_ReservationSet_Release	   : 1;		// 꺼짐 예약 해제
	unsigned   _fanSystemSt_ReservationSet_Running	   : 1;		// 꺼짐 예약 동작중
	unsigned   _fanSystemSt_ReservationSet_NotRunning  : 1;		// 꺼짐 예약 동작 안함

	unsigned   _fanSystemSt_FanReservationSetMode_Hour : 1;
	unsigned   _fanSystemSt_FanReservationSetMode_Minute : 1;

	unsigned   _fanSystemSt_FanReservation_Hour		   : 7;		// 꺼짐 예약 시간
	unsigned   _fanSystemSt_FanReservation_Minute	   : 7;		// 꺼짐 예약 분

	unsigned   _fanSystemSt_SensorMode_NotExistSenosr  : 1;     // 센서 모드 : 센서 없음
	unsigned   _fanSystemSt_SensorMode_GasSenosr	   : 1;     // 센서 모드 : 가스 센서
	unsigned   _fanSystemSt_SensorMode_CO2Senosr	   : 1;     // 센서 모드 : CO2센서
	unsigned   _fanSystemSt_DamperMode_NotExist		   : 1;	    // 댐퍼 모드 : 없음
	unsigned   _fanSystemSt_DamperMode_Exist		   : 1;     // 댐퍼 모드 : 있음
	unsigned   _fanSystemSt_FilterEmpressMode_Clean    : 1;		// 필터/여제 : 청소
	unsigned   _fanSystemSt_FilterEmpressMode_Change   : 1;     // 필터/여제 : 교환
	unsigned   _fanSystemSt_FilterEmpressMode_FEChange : 1;     // 필터/여제 : 여제 교환
	unsigned   _fanSystemSt_FilterEmpressMode_Normal   : 1;     // 필터      : 정상

	unsigned   _fanSystemSt_AlarmMode_Normal           : 1;     // 알람 상태 : 정상
	unsigned   _fanSystemSt_AlarmMode_HighSpeedMotor   : 1;     // 알람 상태 : 급기 모터
	unsigned   _fanSystemSt_AlarmMode_VentilationMotor : 1;     // 알람 상태 : 배기 모터
	unsigned   _fanSystemSt_AlarmMode_DamperMotor      : 1;     // 알람 상태 : 댐퍼 모터
	unsigned   _fanSystemSt_AlarmMode_CO2              : 1;     // 알람 상태 : CO2
	unsigned   _fanSystemSt_AlarmMode_TemperatureMotor : 1;     // 알람 상태 : 온도 센서
	unsigned   _fanSystemSt_AlarmMode_OverHeating      : 1;     // 알람 상태 : 과열 방지
	unsigned   _fanSystemSt_AlarmMode_DeviceProtect    : 1;     // 알람 상태 : 장치 보호

	unsigned   _fanSystemSt_CO2_DensityMode_CO2High    : 1;     // CO2농도 상 모드
	unsigned   _fanSystemSt_CO2_DensityMode_CO2Low     : 1;     // CO2농도 하 모드

	unsigned   _fanSystemSt_CO2_Density_High		   : 7;	    // CO2농도 상위 값
	unsigned   _fanSystemSt_CO2_Density_Low            : 7;     // CO2농도 하위 값

	unsigned   _fanSystemSt_GAS_Density_Low			   : 1;     // 가스 농도 : 약
	unsigned   _fanSystemSt_GAS_Density_Middle		   : 1;     // 가스 농도 : 중
	unsigned   _fanSystemSt_GAS_Density_High           : 1;     // 가스 농도 : 강
};


class ns__fanSystem : public ns__rootDevice
{
public:

	struct xsd_fanSystemProperty devFanSystemProperty;

	enum ns__enum_fanSystem			model;								// 모델명

	enum ns__enum_fanSystem			fanSystemDo_Power;					// 운전 / 정지
	enum ns__enum_fanSystem	        fanSystemDo_AutoFan;				// 자동환기
	enum ns__enum_fanSystem     	fanSystemDo_FanMode;				// 환기모드
	enum ns__enum_fanSystem         fanSystemDo_FanWind;				//풍량 모드 : 풍량 꺼짐 -> 약 -> 중 -> 강
	enum ns__enum_fanSystem         fanSystemDo_FanReservationSet;	    //꺼짐 예약 설정 	

	enum ns__enum_fanSystem			fanSystemSt_Power;					// 운전 / 정지
	enum ns__enum_fanSystem	        fanSystemSt_AutoFan;				// 자동환기
	enum ns__enum_fanSystem     	fanSystemSt_FanMode;				// 환기모드

	enum ns__enum_fanSystem     	fanSystemSt_CurrentFanMode;			// 환기 모드(자동, 수동)구별

	enum ns__enum_fanSystem         fanSystemSt_FanWind;				//풍량 모드 : 풍량 꺼짐 -> 약 -> 중 -> 강
	enum ns__enum_fanSystem         fanSystemSt_FanReservationSet;	    //꺼짐 예약 설정 	

	enum ns__enum_fanSystem         fanSystemSt_FanReservationSetMode;  //꺼짐 예약 시간 / 분 체크
	
	int fanReservation_Hour;											//꺼짐 예약 설정 : 시간 
	int	fanReservation_Minute;											//꺼짐 예약 설정 : 분

	enum ns__enum_fanSystem         fanSystemSt_SensorMode;				//센서 모드 : 센서 없음, 가스 센서, CO2 센서
	enum ns__enum_fanSystem         fanSystemSt_DamperMode;				//댐퍼 모드 : 댐퍼 있음, 댐퍼 없음

	enum ns__enum_fanSystem         fanSystemSt_FilterEmpressMode;		// 필터/여제 모드 : 필터 청소, 필터 교환, 여제 교환

	enum ns__enum_fanSystem         fanSystemSt_AlarmMode;              // 알람 상태 : 급기 모터,배기모터, 댐퍼모터,CO2,온도센서, 과열 방지, 장치 보호

	int  fanSystemSt_Temperature;										// 유압 공기의 온도(급기 온도)

	enum ns__enum_fanSystem         fanSystemSt_CO2_DensityMode;        // CO2농도 상 / 하 체크

	int  fanSystemSt_CO2_Density_High;								    // CO2농도 상위
	int  fanSystemSt_CO2_Density_Low;                                   // CO2농도 하위

	enum ns__enum_fanSystem         fanSystemSt_GAS_Density;			// GAS 농도
	
	enum ns__enum_devError			fanSystemDevError;					//디바이스 에러		

	enum ns__fanSystemFunc
	{
		f_fanSystemDo_Power,											//운전모드
		f_fanSystemDo_AutoFan,											//자동환기
		f_fanSystemDo_FanMode,											//환기모드
		f_fanSystemDo_FanWind,											//풍량
		f_fanSystemDo_FanReservationSet,								//예약 설정 

		f_fanSystemSt_Power,											//운전모드
		f_fanSystemSt_AutoFan,											//자동환기
		f_fanSystemSt_FanMode,											//환기모드
		f_fanSystemSt_FanWind,											//풍량
		f_fanSystemSt_FanReservationSet,								//예약 설정 

		f_fanSystemSt_SensorMode,									    // 센서
		f_fanSystemSt_DamperMode,									    // 댐퍼
		f_fanSystemSt_FilterEmpress,									// 필터/여제
		f_fanSystemSt_AlarmMode,										// 알람 상태
		f_fanSystemSt_Temperature,										// 온도
		f_fanSystemSt_CO2_Density,									    // CO2농도 : 상위, 하위
		f_fanSystemSt_GAS_Density,									    // GAS 농도
		f_fanSystemDevError,
	 }
	func;
};

class ns__getFanSystemPropertyItemResponse {ns__fanSystem _return;};
class ns__getFanSystemItemResponse {ns__fanSystem _return;};
class ns__getFanSystemResponse {ns__fanSystem _return;};

int ns__getFanSystemCount(void* _, int* out);

int ns__getFanSystemPropertyItem(int order, ns__getFanSystemPropertyItemResponse* out);
int ns__getFanSystemItem(int order, ns__getFanSystemItemResponse* out);
int ns__getFanSystem(ns__fanSystem in, ns__getFanSystemResponse* out);
int ns__setFanSystem(ns__fanSystem in, class ns__setFanSystemResponse{}* out);

int ns__FanSystemEvent(ns__fanSystem in, int* out);


//-------------------------------------------------------------
//	방범 센서: NokSung Construction [중국 녹성 부동산] :  
//-------------------------------------------------------------
enum ns__enum_securitySensor
{
	
	_securitySensorSt_Detected,
	_securitySensorSt_UnDetected,
	
	_model_NokSung_XP830RS8I	        // 녹성 부동산
	
};

struct xsd_SecuritySensorProperty 
{
	unsigned   _securitySensorSt_Detected   : 1;	
	unsigned   _securitySensorSt_UnDetected : 1;	

};

class ns__securitySensor : public ns__rootDevice
{
public:

	struct xsd_SecuritySensorProperty devSecuritySensorProperty;

	int		securitySensorStMainChannel;									//  Main ID : 1 - 8

	enum ns__enum_securitySensor		model;								// 모델명
	enum ns__enum_securitySensor        securitySensorStSubChannel_One;		// 센서 감지 / 감지 해제 : Sub Id 1
	enum ns__enum_securitySensor        securitySensorStSubChannel_Two;		// 센서 감지 / 감지 해제 : Sub Id 2
	enum ns__enum_securitySensor        securitySensorStSubChannel_Three;	// 센서 감지 / 감지 해제 : Sub Id 3	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Four;	// 센서 감지 / 감지 해제 : Sub Id 4	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Five;	// 센서 감지 / 감지 해제 : Sub Id 5	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Six;		// 센서 감지 / 감지 해제 : Sub Id 6	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Seven;	// 센서 감지 / 감지 해제 : Sub Id 7	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Eight;	// 센서 감지 / 감지 해제 : Sub Id 8	
	
	enum ns__enum_devError	securitySensorDevError;							// 디바이스 에러	

	enum ns__securitySensorFunc
	{		
		//f_securitySensorStSubChannel,
		f_securitySensorStSubChannelOne,
		f_securitySensorStSubChannelTwo,
		f_securitySensorStSubChannelThree,
		f_securitySensorStSubChannelFour,
		f_securitySensorStSubChannelFive,
		f_securitySensorStSubChannelSix,
		f_securitySensorStSubChannelSeven,
		f_securitySensorStSubChannelEight,

		f_securitySensorDevError
	}
	func;
};

class ns__getSecuritySensorPropertyItemResponse {ns__securitySensor _return;};
class ns__getSecuritySensorItemResponse {ns__securitySensor _return;};
class ns__getSecuritySensorResponse {ns__securitySensor _return;};

int ns__getSecuritySensorCount(void* _, int* out);

int ns__getSecuritySensorPropertyItem(int order, ns__getSecuritySensorPropertyItemResponse* out);
int ns__getSecuritySensorItem(int order, ns__getSecuritySensorItemResponse* out);
int ns__getSecuritySensor(ns__securitySensor in, ns__getSecuritySensorResponse* out);
int ns__setSecuritySensor(ns__securitySensor in, class ns__setSecuritySensorResponse{}* out);

int ns__securitySensorEvent(ns__securitySensor  in, int* out);


//-------------------------------------------------------------
//	독립기능
//-------------------------------------------------------------

// 이벤트를 수신하기위한 등록
int ns__subcribeEvent(int port, int *out );

}
