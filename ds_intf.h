

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
	_bundleLight		=12,			// �ϰ� �ҵ� ����ġ
	_systemAircon		=13,			// �Ｚ �ý��� ������ : �ص� �Ǽ�
	_fanSystem			=14,			// ȯ�� �ý���   : ������
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
	
	// ����̽��� �����Ǿ� ���� ����
	_sensorEmer,
	_exTerminal,

	//
	 // Controlled Device Protocol
	_protoCommax,
	_protoNokSung, //�߱� �켺 �ε��� [��� ����]	
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

	// ���� : LnCP
	devError_no,
	devError_232_Serial_Port_Open_Error,  
	devError_lncp_Common_Micom_Error,				        // �����Ŀ���
	devError_lncp_Common_Running_Door_Open,                 // ���� �� �� ����
	devError_lncp_Common_Hardware_Error,                    // �ϵ���� ����
	devError_lncp_Common_Sensor_Error,                      // ���� ����
	devError_lncp_Common_Motor_Error,                       // ���� ����
	devError_lncp_Common_Power_Fail,                        // ���� ����
	devError_lncp_Common_Temperature_Range_Error,           // ���� �µ� ���� �ʰ�
	devError_lncp_Common_Device_Preempted_Error,            // ����̽� ����
	devError_lncp_Common_Device_control_timeout,            // ó�� �ð� ���
	devError_lncp_Common_Device_Not_Exist_DD,               // �������� �ʴ� ����̽� ��ũ����
	devError_lncp_Common_Device_Nak_Error,			        // ����̽����� NAK�� ����
	devError_lncp_Common_Incorrect_Option_Setting,          // �߸��� Option�� ���� �� ���
	devError_lncp_Common_Argument_Error,                    // Input Parameter ����
	
	// ������� ��� ����

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
	
	// ���Ϸ�
	devError_Boiler_control,  // ����� ����
	devError_Boiler_drive,    // ������ ����
	devError_Boiler_boiler,   // ���Ϸ� ����
	devError_Boiler_eeprom,   // EEPROM ����

	// ���� 
	devGas_notFound_sensor,
 
	// �Ʒθ�
	devError_aroma_catridge1_drive, // 1�� īƮ���� ���� ����
	devError_aroma_catridge2_drive, // 2�� īƮ���� ���� ����
	devError_aroma_catridge3_drive, // 3�� īƮ���� ���� ����
	
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
	_not_founded_file            = 1,          // ���� ������ ����
	_not_founded_configuration_info   =2,      // ���� ������ ����
	_not_founded_mismatch_configuration = 3,   // ���� ������ Ʋ��
	_not_supported_function      = 4,          // ��� ������ ���� ����
	_supported_function          = 5,          // ��� ������ ���� ����
    _not_supported_device        = 6,          // ���� ����̹����� ����̽��� ������ ���� ����
	_device_support              = 7,          // ���� ����̹����� ����̽��� ����
	_not_response_control        = 8,          // ��� ���� ������ ����	
	_not_device_use              = 9,         // ����̽��� ����� �� ���� :: ����̽� ��ü�� �������� ���� �� ������ ���
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

		unsigned int			order;					// ����
};


//-------------------------------------------------------------
//	Device Service :: ���� ��� ������ ����̽� Category
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

		// ����̹����� �����ϴ� ����̽� ��
		struct xsd_devCategory enable_device_category;  
		
		enum ns__enum_config_Common_Error commonError;	
		
		enum ns__enum_devError boilerDeviceError;           // RS 485 ��� ����
		enum ns__enum_devError lightDeviceError;			// RS 485 ��� ����
		enum ns__enum_devError gasDeviceError;				// RS 485 ��� ����		
		enum ns__enum_devError doorLockDeviceError;         // RS 485 ��� ����
		enum ns__enum_devError acConsentDeviceError;        // RS 485 ��� ����
		enum ns__enum_devError curtainDeviceError;			// RS 485 ��� ����
		enum ns__enum_devError bundleLightDeviceError;		// RS 485 ��� ����
		enum ns__enum_devError fanSystemDeviceError;        // RS 485 ��� ����
		enum ns__enum_devError securitySensorDeviceError;        // RS 485 ��� ����		

		enum ns__enum_devError airconDeviceError;           // RS 232 ��� ����

		enum ns__enum_devError rfDeviceError;               // TTL ��� ����
		enum ns__enum_devError systemAirconDeviceError;     // RS 485 ��� ����

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
//	boiler, ���Ϸ�, FunctionCode : 1
//-------------------------------------------------------------
enum ns__enum_boiler
{
	 // ���� �� ���� ����
	_boilerPower_Off,						// ��������_Off
	_boilerPower_On,						// ��������_On

	_boilerHeating_Off,                     // ���漳��_Off
	_boilerHeating_On,                      // ���漳��_On

     // ���� ��� ����
	_boilerOutMode_Set,						// ������_����
	_boilerOutMode_Release,					// ������_����

	// ���Ϸ� ��� ����
	_boilerDipSwitchMode_Hypocaust,         // ��� : ����µ�����_�µ�
	_boilerDipSwitchMode_InnerTemperature,  // ��� : ����µ�����_�ǳ��µ�
	
	// ȸ�� ��
	
	_model_boiler_Rinnai,				    // ������
	_model_boiler_Kiturami,					// �ͶѶ�� ���Ϸ�
    _model_boiler_KyungDong,				// �浿  
	
	_model_boiler_HansungCisco,				// �Ѽ� �ý���
	_model_boiler_HauWell,				    // �ϴ���
	_model_boiler_HanEnergy,                // �� ������
	_model_boiler_Valcon,                   // �ѱ� ����
	_model_boiler_Kotech                    // ����
};


struct xsd_BoilerProperty
{
	unsigned   _indivisualBoiler			 : 1;  //�������Ϸ�
	unsigned   _eachBoiler					 : 1;  //���� �µ� ������

	unsigned   _boilerPower_Off				 : 1;  //���Ϸ� ����
	unsigned   _boilerPower_On				 : 1;  //���Ϸ� ����

	unsigned   _boilerHeatingPower_Off		 : 1;  //���� ����		
	unsigned   _boilerHeatingPower_On		 : 1;  //���� ����

	unsigned   _boilerOutMode_Set			 : 1;
	unsigned   _boilerOutMode_Release		 : 1;

	//���Ϸ� ���
	unsigned _boilerDipSwitchMode_Hypocaust  : 1;
	unsigned _boilerDipSwitchMode_InnerTemperature : 1;	
	
	//���Ϸ� �µ�
	unsigned boilerRequestedTemperature      : 1;  // ���� �µ� �������� ���� �Ǵ�
	unsigned boilerRequestedTemperatureFrom  : 7;  // �µ� ���� ���� 
	unsigned boilerRequestedTemperatureTo    : 7;  // �µ� �� ����

	unsigned boilerCurrentTemperature        : 1;  // ���� �µ� �������� ���� �Ǵ�
    unsigned boilerCurrentTemperatureFrom    : 7;  // �µ� ���� ���� 
	unsigned boilerCurrentTemperatureTo      : 7;  // �µ� �� ����

	unsigned boilerHeatingWaterTemperature   : 1;   // �¼� �µ� �������� ���� �Ǵ�
	unsigned boilerHeatingWaterTemperatureFrom : 7; // �µ� ���� ���� 
	unsigned boilerHeatingWaterTemperatureTo   : 7; // �µ� �� ����

};

class ns__boiler : public ns__rootDevice
{
public:

	struct xsd_BoilerProperty  devBoilerProperty;
	char *						boilerZoneName;                   // ���Ϸ�(���� �µ� ������)��ġ

	int							boilerRequestedTemperature;	      // ���� �µ�����
	int							boilerCurrentTemperature;	      // ���� ����µ�
	int							boilerHeatingWaterTemperature;	  // �¼� ����µ�

	enum ns__enum_boiler		model;						      // �𵨸�

	enum ns__enum_boiler		boilerPower;				      // ��������
	enum ns__enum_boiler		boilerHeatingPower;				  // ���漳��
	enum ns__enum_boiler		boilerOutMode;				      // ������
	enum ns__enum_boiler        boilerDipSwitchMode;		      // ���� �µ����� ���
	

	enum ns__enum_devError		boilerDevError;				      // ����̽� ����	
	
	enum ns__boilerFunc
	{
		f_boilerPower,                     // ���� 
		f_boilerHeatingPower,              // ����
		f_boilerOutMode,                   // ���� ���
		f_boilerRequestedTemperature,      // ���� �µ� ����
		f_boilerCurrentTemperature,        // ���� ���� �µ�
		f_boilerHeatingWaterTemperature,   // �¼� ���� �µ�
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
//	light, ����,  FunctionCode : 2
//-------------------------------------------------------------


enum ns__enum_light
{
	_lightPower_Off = 0,				// �����Ŀ�_����
	_lightPower_On = 1,					// �����Ŀ�_����
	_lightPower_AllOff =3,				// �����Ŀ�_��ü����
	_lightPower_AllOn = 4,				// �����Ŀ�_��ü����

	_lightSwitch_Binary,				//  ON/OFF ����ġ
	_lightSwitch_Dimmable,				//  ��� ����ġ

	_model_Light_JoongAngControl = 5,	// �߾�����
	_model_Light_Clio = 6,		        // Ŭ����
	_model_Light_Anam = 7,              // �Ƴ�
	_model_Light_GoIM = 8,				// GoIM
	_model_Light_PlanetInt = 9,			// PlanetInt
	_model_UnKnown = 10

};

struct xsd_LightProperty
{
	unsigned _lightBinary       : 1;    // On/Off ����ġ
	unsigned _lightDimmable     : 1;    // ��� ����ġ

	unsigned _lightPower_Off    : 1;
	unsigned _lightPower_On	    : 1;	

	unsigned _lightPower_AllOff	: 1;		
	unsigned _lightPower_AllOn	: 1;	

	unsigned dimmerLevelFrom	: 4;	// ���� ���� ���� ����
	unsigned dimmerLevelTo		: 4;	// ���� ���� �� ����

	
};

class ns__light : public ns__rootDevice
{
	public:
		
		struct xsd_LightProperty    devLightProperty;	
		char*						lightZoneMainName;
		char*						lightZoneSubName;
		int							dimmableLevel;   // ��� ����ġ �������� :: ����( 1 - 10 )		

		enum ns__enum_light			model;			// �𵨸�
		enum ns__enum_light			lightPower;		// �����Ŀ�
		enum ns__enum_light         lightSwitchMode; // Binary Or Dimming Switch
		enum ns__enum_devError		lightDevError;	// ����̽� ����	

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
//	gasValve, �������,  FunctionCode : 3
//-------------------------------------------------------------
enum ns__enum_gasValve
{
	_gvDo_Close,				// ���ۼ���_����
	_gvDo_Open,					// ���ۼ���_����

	_gvAlarm_On,				// ��������溸_�︲
	_gvAlarm_Off,				// ��������溸_����

	_model_GasValve_Shinwoo,	// �ſ� ����
	_model_GasValve_GoIM,		// ����̿�
	_model_GasValve_Mat,        // Mat
	_model_GasValve_koreaFire   // �ѱ� �ҹ�
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

	enum ns__enum_gasValve		model;		// �𵨸�
	enum ns__enum_gasValve		gvDo;		// ���ۼ���
	enum ns__enum_gasValve		gvAlarm;	// ��������溸
	enum ns__enum_devError		gvDevError;	// ����̽� ����

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
//	curtain, Ŀư, FunctionCode : 6
//-------------------------------------------------------------
enum ns__enum_curtain
{
	_curtainDo_Close,			// Ŀư��_����
	_curtainDo_Open,			// Ŀư��_����
	_curtainDo_Stop,			// Ŀư��_����

	_model_curtain_PlanetInt,   // �÷���
	_model_curtain_CLIO,			// Ŭ���� 
	_model_curtain_HaMun		// �Ϲ�
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
	enum ns__enum_curtain		model;				// �𵨸�
	enum ns__enum_curtain		curtainDo;			// Ŀư��
	enum ns__enum_devError		curtainDevError;	// ����̽� ����

	int curtainAngular;							    // Ŀư ����

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
//	�ϰ��ҵ� ����ġ[Function Code] Function Code : 12
//-------------------------------------------------------------
enum ns__enum_bundleLight
{
	_bundleLightPower_Off,		     // �ϰ� ���� ����ġ_����
	_bundleLightPower_On,			 // �ϰ� ���� ����ġ_����
	_bundleLightPower_AllOff,		 // �ϰ� ���� ����ġ_��ü����
	_bundleLightPower_AllOn,		 // �ϰ� ���� ����ġ_��ü����

	_readyEnergyPower_Off,			 // ��� ���� ����ġ_����
	_readyEnergyPower_On,			 // ��� ���� ����ġ_����
	_readyEnergyPower_AllOff,		 // ��� ���� ����ġ_��ü����
	_readyEnergyPower_AllOn,		 // ��� ���� ����ġ_��ü����

	_bundleLightExit_Notify,		 // ���� �뺸
	_bundleLightExit_Entry_Accept,	 // ���� ���� �㰡 [ DS�� ���� ���� ���� �뺸�� ���е忡�� ���� ���/�źθ� DS�� �˷� �����
	_bundleLightExit_Entry_Deny,     // ���� ���� �Ұ� 
	_bundleLightExit_Setting,        // ���� ����
	_bundleLightExit_Release,        // ���� ���� 

	_bundleLightElevator_Up,         // ���������� ȣ�� : Up
	_bundleLightElevator_Down,       // ���������� ȣ�� : Down
	_bundleLightElevator_Fail,       // ���������� ȣ�� : Fail    (���������� ȣ�� ���и� �ϰų�, L/S�� �����̵��� ���� ��� ȣ�� ]
	_bundleLightElevator_Success,    // ���������� ȣ�� : Success (���������� ȣ�� ���� )
	_gasValve_Entry_Accept,
	_gasValve_Entry_Deny,

	_model_BundleLight_icom,	     //  �Ƽ� 
	_model_BundleLight_Clio,	     //  Ŭ����
	_model_BundleLight_Anam,	     //  �Ƴ�	
	_model_BundleLight_Speel		//  ����


};	

struct xsd_bundleLightProperty
{
	unsigned   _bundleLightPower_Off			: 1;	
	unsigned   _bundleLightPower_On				: 1;	
	unsigned   _bundleLightPower_AllOff			: 1;	
	unsigned   _bundleLightPower_AllOn			: 1;	

	unsigned  _readyEnergyPower_Off				: 1;  	 // ��� ���� ����ġ_����
	unsigned  _readyEnergyPower_On				: 1;		 // ��� ���� ����ġ_����
	unsigned  _readyEnergyPower_AllOff			: 1;		 // ��� ���� ����ġ_��ü����
	unsigned  _readyEnergyPower_AllOn			: 1;		 // ��� ���� ����ġ_��ü����

	unsigned   _bundleLightExit_Notify			: 1;		
	unsigned   _bundleLightExit_Entry_Accept    : 1;	 // ���� ���� �㰡 [ DS�� ���� ���� ���� �뺸�� ���е忡�� ���� ���/�źθ� DS�� �˷� �����
	unsigned   _bundleLightExit_Entry_Deny      : 1;     // ���� ���� �Ұ� 
	unsigned   _bundleLightExit_Setting         : 1;     // ���� ����
	unsigned   _bundleLightExit_Release         : 1;     // ���� ���� 

	unsigned   _bundleLightElevator_Up          : 1;
	unsigned   _bundleLightElevator_Down        : 1;

	unsigned   _gasValve_Entry_Accept			: 1;
	unsigned   _gasValve_Entry_Deny				: 1;

};


class ns__bundleLight : public ns__rootDevice
{
public:

	struct xsd_bundleLightProperty devBundleLightProperty;

	enum ns__enum_bundleLight			model;					// �𵨸�
	enum ns__enum_bundleLight			bundleLightPower;		// �ϰ��ҵ� ����ġ ����
	enum ns__enum_bundleLight			bundleLightExit;		// �ϰ��ҵ� ����
	enum ns__enum_bundleLight			readyEnergyPower;		// ��� ���� ����
	enum ns__enum_bundleLight			gasValve;				// ���� ���� �Ұ�/ �㰡
	enum ns__enum_bundleLight			bundleLightElevator;	// ���������� Up/Down

	enum ns__enum_devError			    bundleLightDevError;	// ����̽� ����		

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
//	�ý��� ������[Function Code] Function Code : 13
//-------------------------------------------------------------
enum ns__enum_systemAircon
{
	_systemAirconDo_Power_Off,							// �ý��� ������ ����_����
	_systemAirconDo_Power_On,							// �ý��� ������ ����_����
	_systemAirconDo_Power_AllOff,						// �ý��� ������ ����_��ü����
	_systemAirconDo_Power_AllOn,						// �ý��� ������ ����_��ü����
	
	_systemAirconDo_WindDirection_UpDown,				// �ý��� ������ ǳ�� ����
	_systemAirconDo_WindDirection_LeftRight,			// �ý��� ������ ǳ�� �¿�
	_systemAirconDo_WindDirection_UpDownLeftRight,			// �ý��� ������ ǳ�� �����¿�
	_systemAirconDo_WindDirection_Stop,				// �ý��� ������ ǳ�� ����

	_systemAirconDo_Drive_Auto,							// �ý��� ������  ���� �ڵ�				
	_systemAirconDo_Drive_Cooling,						// �ý��� ������  ���� �ù�
	_systemAirconDo_Drive_Humidity,						// �ý��� ������  ���� ����
	_systemAirconDo_Drive_Ventilation,					// �ý��� ������  ���� ��ǳ
	_systemAirconDo_Drive_Heating,						// �ý��� ������  ���� ����

	_systemAirconDo_WindPower_Auto,					    // �ý��� ������  �ڵ�
	_systemAirconDo_WindPower_Breeze,					// �ý��� ������  ��ǳ
	_systemAirconDo_WindPower_Middle,					// �ý��� ������  ��ǳ
	_systemAirconDo_WindPower_Super	,				    // �ý��� ������  ��ǳ
	_systemAirconDo_WindPower_Natural,				    // �ý��� ������  �ڿ�ǳ

	_systemAirconDo_StatusQuery_Start,					// ���� ��û ����
	_systemAirconDo_StatusQuery_Stop,                   // ���� ��û ����

	_systemAirconSt_Power_Start,						    // �ý��� ������ ����_����
	_systemAirconSt_Power_Stop,						    // �ý��� ������ ����_����
	
	_systemAirconSt_WindDirection_UpDown,				// �ý��� ������ ǳ�� ����
	_systemAirconSt_WindDirection_LeftRight,			    // �ý��� ������ ǳ�� �¿�
	_systemAirconSt_WindDirection_UpDownLeftRight,      // �ý��� ������ ǳ�� �����¿�
	_systemAirconSt_WindDirection_Stop,				    // �ý��� ������ ǳ�� ����
	
	_systemAirconSt_WindPower_Auto,					    // �ý��� ������  �ڵ�
	_systemAirconSt_WindPower_Breeze,					// �ý��� ������  ��ǳ
	_systemAirconSt_WindPower_Middle,					// �ý��� ������  ��ǳ
	_systemAirconSt_WindPower_Super,					    // �ý��� ������  ��ǳ
	_systemAirconSt_WindPower_Natural,				    // �ý��� ������  �ڿ�ǳ

	_systemAirconSt_Drive_Driving,					    // �ý��� ������  �ǳ����� : ����
	_systemAirconSt_Drive_EventRelease,				    // �ý��� ������  �ǳ����� : Event ����
	_systemAirconSt_Drive_Auto,						    // �ý��� ������  �ǳ����� : �ڵ�
	_systemAirconSt_Drive_JaeSang,					    // �ý��� ������  �ǳ����� : ����
	_systemAirconSt_Drive_Ventilation,				    // �ý��� ������  �ǳ����� : ��ǳ
	_systemAirconSt_Drive_Humidity,					    // �ý��� ������  �ǳ����� : ����
	_systemAirconSt_Drive_Cooling,					    // �ý��� ������  �ǳ����� : �ù�
	_systemAirconSt_Drive_Heating,					    // �ý��� ������  �ǳ����� : ����

	_systemAirconSt_Tracking_Success,                   // �ý��� ������  Ʈ��ŷ : ����
	_systemAirconSt_Tracking_Fail,	                    // �ý��� ������  Ʈ��ŷ : ����
	_systemAirconSt_Tracking_Searching,                 // �ý��� ������  Ʈ��ŷ : �˻�
	_systemAirconSt_Tracking_End,		                // �ý��� ������  Ʈ��ŷ : �Ϸ�
	_systemAirconSt_Installation_Not_Exist,             // �ý��� ������  �ǳ��� : �̼�ġ

	_systemAirconSt_InnerRoom_Error_Normal,				// �ǳ��� ����
	_systemAirconSt_InnerRoom_Error_InnerTemperature,	// �ý��� ������  �ǳ��� ���� : �ǳ� �µ� ���� ����
	_systemAirconSt_InnerRoom_Error_EVA_IN,       	    // �ý��� ������  �ǳ��� ���� : �ǳ��� EVA_IN ����
	_systemAirconSt_InnerRoom_Error_FAN,				    // �ý��� ������  �ǳ��� ���� : �ǳ��� FAN ����
	_systemAirconSt_InnerRoom_Error_Communication,	    // �ý��� ������  �ǳ��� ���� : �ǳ��� ��� ����
	_systemAirconSt_InnerRoom_Error_Floating_Switch,    // �ý��� ������  �ǳ��� ���� : Floating ����ġ ����
	_systemAirconSt_InnerRoom_Error_Open_Two,			// �ý��� ������  �ǳ��� ���� : ���� �� ���� ���� ����
	_systemAirconSt_InnerRoom_Error_EVA_OUT,	        // �ý��� ������  �ǳ��� ���� : �ǳ��� EVA_OUT ����

	_systemAirconSt_OutRoom_Error_Normal,				// �ǿܱ� ����
	_systemAirconSt_OutRoom_Error_Temp,				    // �ý��� ������  �ǳ��� ���� :  �ǿܱ� �µ� ���� ����
	_systemAirconSt_OutRoom_Error_Sensor,				// �ý��� ������  �ǳ��� ���� :  ���� ���� ����
	_systemAirconSt_OutRoom_Error_Cond,				    // �ý��� ������  �ǳ��� ���� :  COND ������� ����
	_systemAirconSt_OutRoom_Error_Commnuication,	    // �ý��� ������  �ǳ��� ���� :  ��� ����
	_systemAirconSt_OutRoom_Error_Error,				// �ý��� ������  �ǳ��� ���� :  ���� �߻�
	_systemAirconSt_OutRoom_Error_lowPower,			    // �ý��� ������  �ǳ��� ���� :  ���м��� ����
	_systemAirconSt_OutRoom_Error_MagicalPower,		    // �ý��� ������  �ǳ��� ���� :  ���¼��� ����
	_systemAirconSt_OutRoom_Error_InitCommunication,	// �ý��� ������  �ǳ��� ���� :  �ʱ� ��� ����	

	_model_systemAircon_SamSung,						//  �Ｚ �ý��� ������
	

};	

struct xsd_systemAirconProperty
{
	unsigned   _systemAirconDo_Power_Start						: 1;	 // �ý��� ������ ����_����
	unsigned   _systemAirconDo_Power_Stop						: 1;	 // �ý��� ������ ����_����
	unsigned   _systemAirconDo_Power_AllRunning				    : 1;	 // �ý��� ������ ����_��ü����
	unsigned   _systemAirconDo_Power_AllStop					: 1;	 // �ý��� ������ ����_��ü����

	unsigned   _systemAirconDo_WindDirection_UpDown				: 1;     // �ý��� ������ ǳ�� ����
	unsigned   _systemAirconDo_WindDirection_LeftRight			: 1;     // �ý��� ������ ǳ�� �¿�
	unsigned   _systemAirconDo_WindDirection_UpDownLeftRight    : 1;     // �ý��� ������ ǳ�� �����¿�
	unsigned   _systemAirconDo_WindDirection_Stop				: 1;     // �ý��� ������ ǳ�� ����
	
	unsigned   _systemAirconDo_WindPower_Auto					: 1;     // �ý��� ������  �ڵ�
	unsigned   _systemAirconDo_WindPower_Breeze					: 1;     // �ý��� ������  ��ǳ
	unsigned   _systemAirconDo_WindPower_Middle					: 1;     // �ý��� ������  ��ǳ
	unsigned   _systemAirconDo_WindPower_Super					: 1;     // �ý��� ������  ��ǳ
	unsigned   _systemAirconDo_WindPower_Natural				: 1;     // �ý��� ������  �ڿ�ǳ

	unsigned   _systemAirconDo_Drive_Auto						: 1;     // �ý��� ������   �ڵ�
	unsigned   _systemAirconDo_Drive_Cool						: 1;     // �ý��� ������   �ù�
	unsigned   _systemAirconDo_Drive_Humidity					: 1;     // �ý��� ������   ����
	unsigned   _systemAirconDo_Drive_Ventilation				: 1;     // �ý��� ������   ��ǳ
	unsigned   _systemAirconDo_Drive_Heating					: 1;     // �ý��� ������   ����

	unsigned   _systemAirconSt_Power_Start						: 1;	 // �ý��� ������ ����_����
	unsigned   _systemAirconSt_Power_Stop						: 1;	 // �ý��� ������ ����_����
	
	unsigned   _systemAirconSt_WindDirection_UpDown				: 1;     // �ý��� ������ ǳ�� ����
	unsigned   _systemAirconSt_WindDirection_LeftRight			: 1;     // �ý��� ������ ǳ�� �¿�
	unsigned   _systemAirconSt_WindDirection_UpDownLeftRight    : 1;     // �ý��� ������ ǳ�� �����¿�
	unsigned   _systemAirconSt_WindDirection_Stop				: 1;     // �ý��� ������ ǳ�� ����
	
	unsigned   _systemAirconSt_WindPower_Auto					: 1;     // �ý��� ������  �ڵ�
	unsigned   _systemAirconSt_WindPower_Breeze					: 1;     // �ý��� ������  ��ǳ
	unsigned   _systemAirconSt_WindPower_Middle					: 1;     // �ý��� ������  ��ǳ
	unsigned   _systemAirconSt_WindPower_Super					: 1;     // �ý��� ������  ��ǳ
	unsigned   _systemAirconSt_WindPower_Natural				: 1;     // �ý��� ������  �ڿ�ǳ

	unsigned   _systemAirconSt_Drive_Driving					: 1;     // �ý��� ������  �ǳ����� : ����
	unsigned   _systemAirconSt_Drive_EventRelease				: 1;     // �ý��� ������  �ǳ����� : Event ����
	unsigned   _systemAirconSt_Drive_Auto						: 1;     // �ý��� ������  �ǳ����� : �ڵ�
	unsigned   _systemAirconSt_Drive_JaeSang					: 1;     // �ý��� ������  �ǳ����� : ����
	unsigned   _systemAirconSt_Drive_Ventilation				: 1;     // �ý��� ������  �ǳ����� : ��ǳ
	unsigned   _systemAirconSt_Drive_Humidity					: 1;     // �ý��� ������  �ǳ����� : ����
	unsigned   _systemAirconSt_Drive_Cooling					: 1;     // �ý��� ������  �ǳ����� : �ù�
	unsigned   _systemAirconSt_Drive_Heating					: 1;     // �ý��� ������  �ǳ����� : ����

	unsigned   _systemAirconSt_Tracking_Success                 : 1;     // �ý��� ������  Ʈ��ŷ : ����
	unsigned   _systemAirconSt_Tracking_Fail	                : 1;     // �ý��� ������  Ʈ��ŷ : ����
	unsigned   _systemAirconSt_Tracking_Searching               : 1;     // �ý��� ������  Ʈ��ŷ : �˻�
	unsigned   _systemAirconSt_Tracking_End		                : 1;     // �ý��� ������  Ʈ��ŷ : �Ϸ�
 
	unsigned   _systemAirconSt_Installation_Not_Exist           : 1;	 // �ý��� ������  �ǳ��� : �̼�ġ
 
	unsigned   _systemAirconSt_InnerRoom_Error_Normal           : 1;
	unsigned   _systemAirconSt_InnerRoom_Error_InnerTemperature	: 1;     // �ý��� ������  �ǳ��� ���� : �ǳ� �µ� ���� ����
	unsigned   _systemAirconSt_InnerRoom_Error_EVA_IN       	: 1;     // �ý��� ������  �ǳ��� ���� : �ǳ��� EVA_IN ����
	unsigned   _systemAirconSt_InnerRoom_Error_FAN				: 1;     // �ý��� ������  �ǳ��� ���� : �ǳ��� FAN ����
	unsigned   _systemAirconSt_InnerRoom_Error_Communication	: 1;     // �ý��� ������  �ǳ��� ���� : �ǳ��� ��� ����
	unsigned   _systemAirconSt_InnerRoom_Error_Floating_Switch  : 1;     // �ý��� ������  �ǳ��� ���� : Floating ����ġ ����
	unsigned   _systemAirconSt_InnerRoom_Error_Open_Two			: 1;     // �ý��� ������  �ǳ��� ���� : ���� �� ���� ���� ����
	unsigned   _systemAirconSt_InnerRoom_Error_EVA_OUT	        : 1;     // �ý��� ������  �ǳ��� ���� : �ǳ��� EVA_OUT ����

	unsigned   _systemAirconSt_OutRoom_Error_Normal             : 1;
	unsigned   _systemAirconSt_OutRoom_Error_Temp				: 1;     // �ý��� ������  �ǿܱ� ���� :  �ǿܱ� �µ� ���� ����
	unsigned   _systemAirconSt_OutRoom_Error_Sensor				: 1;     // �ý��� ������  �ǿܱ� ���� :  ���� ���� ����
	unsigned   _systemAirconSt_OutRoom_Error_Cond				: 1;     // �ý��� ������  �ǿܱ� ���� :  COND ������� ����
	unsigned   _systemAirconSt_OutRoom_Error_Commnuication		: 1;     // �ý��� ������  �ǿܱ� ���� :  ��� ����
	unsigned   _systemAirconSt_OutRoom_Error_Error				: 1;     // �ý��� ������  �ǿܱ� ���� :  ���� �߻�
	unsigned   _systemAirconSt_OutRoom_Error_lowPower			: 1;     // �ý��� ������  �ǿܱ� ���� :  ���м��� ����
	unsigned   _systemAirconSt_OutRoom_Error_MagicalPower		: 1;     // �ý��� ������  �ǿܱ� ���� :  ���¼��� ����
	unsigned   _systemAirconSt_OutRoom_Error_InitCommunication	: 1;     // �ý��� ������  �ǿܱ� ���� :  �ʱ� ��� ����

};


class ns__systemAircon : public ns__rootDevice
{
public:

	struct xsd_systemAirconProperty devsystemAirconProperty;

	enum ns__enum_systemAircon			model;								// �𵨸�
	enum ns__enum_systemAircon			systemAirconDo_Power;				// �ý��� ������ ���� / ����
	enum ns__enum_systemAircon          systemAirconDo_WindDirection;		// ǳ��	
	enum ns__enum_systemAircon          systemAirconDo_WindPower;			// ǳ��
	enum ns__enum_systemAircon          systemAirconDo_Drive;				// ���� ����
	enum ns__enum_systemAircon			systemAirconDo_StatusQuery;			// ����� ��׷����� ���� �߻� : ���ÿ��� ���� ��û�� ���� ����

	enum ns__enum_systemAircon          systemAirconSt_Power;				 // �ý��� ������ ���� / ����
	enum ns__enum_systemAircon          systemAirconSt_WindDirection;		 // ǳ��	
	enum ns__enum_systemAircon          systemAirconSt_WindPower;			 // ǳ��	
	enum ns__enum_systemAircon          systemAirconSt_Drive;				 // �ǳ� ����	
	enum ns__enum_systemAircon          systemAirconSt_Tracking;             // Ʈ��ŷ
	enum ns__enum_systemAircon          systemAirconSt_Installation;         // �ǳ��� ��ġ ���� Ȯ��
	enum ns__enum_systemAircon          systemAirconSt_InnerRoom_Error;		 // �ǳ��� ����
	enum ns__enum_systemAircon          systemAirconSt_OutRoom_Error;		 // �ǿܱ� ����
	int									systemAirconDo_Temperature_Set;      // ���� �µ�
	int									systemAirconSt_Temperature_Set;      // ���� �µ�
	int									systemAirconSt_Temperature_Current;  // ���� �µ�
	enum ns__enum_devError			    systemAirconDevError;	             // ����̽� ����		

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
//	ȯ�� �ý���[Function Code] Function Code : 14 
//-------------------------------------------------------------
enum ns__enum_fanSystem
{
	_fanSystemDo_Power_Off,							 // ȯ�� �ý��� : ���� ����
	_fanSystemDo_Power_On,							 // ȯ�� �ý��� : ���� ����
	_fanSystemDo_AutoFan_ElectricHeat,				 // �ڵ�ȯ�� ��� -> ���� ��ȯ
	_fanSystemDo_AutoFan_GeneralFan,                 // �ڵ�ȯ�� ��� -> �Ϲ� ȯ��

    _fanSystemDo_FanMode_ElectricHeat,               // ȯ�� ���     -> ���� ��ȯ
	_fanSystemDo_FanMode_GeneralFan,                 // ȯ�� ���     -> �Ϲ� ȯ��
	_fanSystemDo_FanMode_Schedule,                   // ȯ�� ���     -> ������
	_fanSystemDo_FanMode_SleepingMode,               // ȯ�� ���     -> ��ħ���

	_fanSystemDo_Wind_Off,						     // ǳ�� ����
	_fanSystemDo_Wind_Low,					         // ǳ�� ��
	_fanSystemDo_Wind_Middle,					     // ǳ�� ��							
	_fanSystemDo_Wind_High,						     // ǳ�� ��

	_fanSystemDo_ReservationSet_Set,				 // ���� ���� ����
	_fanSystemDo_ReservationSet_Release,			 // ���� ���� ����
	_fanSystemDo_ReservationSet_Running,			 // ���� ���� ������
	_fanSystemDo_ReservationSet_NotRunning,			 // ���� ���� ���� ����

	_fanSystemSt_Power_Off,				             // ȯ�� �ý��� : ���� ����
	_fanSystemSt_Power_On,					         // ȯ�� �ý��� : ���� ����

	_fanSystemSt_AutoFan_ElectricHeat,               // �ڵ�ȯ�� ��� -> ���� ��ȯ
	_fanSystemSt_AutoFan_GeneralFan,                 // �ڵ�ȯ�� ��� -> �Ϲ� ȯ��
	_fanSystemSt_CurrentFanMode_AutoFan,			 // ȯ�� ��� (�ڵ�)

    _fanSystemSt_FanMode_ElectricHeat,               // ȯ�� ���     -> ���� ��ȯ
	_fanSystemSt_FanMode_GeneralFan,                 // ȯ�� ���     -> �Ϲ� ȯ��
	_fanSystemSt_FanMode_Schedule,                   // ȯ�� ���     -> ������
	_fanSystemSt_FanMode_SleepingMode,               // ȯ�� ���     -> ��ħ���
	_fanSystemSt_CurrentFanMode_Manual,		         // ȯ�� ���(����)

	_fanSystemSt_Wind_Off,						     // ǳ�� ����
	_fanSystemSt_Wind_Low,					         // ǳ�� ��
	_fanSystemSt_Wind_Middle,					     // ǳ�� ��							
	_fanSystemSt_Wind_High,						     // ǳ�� ��

	
	_fanSystemSt_FanReservationSetMode_Hour,		 // ���� ������ ���� �� �� �� �� �� �ĺ� �ϱ� ����	
	_fanSystemSt_FanReservationSetMode_Minute,       // ���� ������ ���� �� �� �� �� �ĺ� �ϱ� ����

	_fanSystemSt_ReservationSet_Set,				  // ���� ���� ����
	_fanSystemSt_ReservationSet_Release,			  // ���� ���� ����
	_fanSystemSt_ReservationSet_Running,			  // ���� ���� ������
	_fanSystemSt_ReservationSet_NotRunning,			  // ���� ���� ���� ����

	_fanSystemSt_SensorMode_NotExistSenosr,           // ���� ��� : ���� ����
	_fanSystemSt_SensorMode_GasSenosr,				  // ���� ��� : ���� ����
	_fanSystemSt_SensorMode_CO2Senosr,			      // ���� ��� : CO2����

    _fanSystemSt_DamperMode_NotExist,		  	      // ���� ��� : ����
	_fanSystemSt_DamperMode_Exist,		              // ���� ��� : ����

	_fanSystemSt_FilterEmpressMode_Normal,    	      // ����/���� : ����
    _fanSystemSt_FilterEmpressMode_Clean,    	      // ����/���� : û��
	_fanSystemSt_FilterEmpressMode_Change,            // ����/���� : ��ȯ
	_fanSystemSt_FilterEmpressMode_FEChange,          // ����/���� : ���� ��ȯ

	_fanSystemSt_AlarmMode_Normal, 
	_fanSystemSt_AlarmMode_HighSpeedMotor,            // �˶� ���� : �ޱ� ����
	_fanSystemSt_AlarmMode_VentilationMotor,          // �˶� ���� : ��� ����
	_fanSystemSt_AlarmMode_DamperMotor,               // �˶� ���� : ���� ����
	_fanSystemSt_AlarmMode_CO2,                       // �˶� ���� : CO2
	_fanSystemSt_AlarmMode_TemperatureMotor,          // �˶� ���� : �µ� ����
	_fanSystemSt_AlarmMode_OverHeating,               // �˶� ���� : ���� ����
	_fanSystemSt_AlarmMode_DeviceProtect,             // �˶� ���� : ��ġ ��ȣ

	_fanSystemSt_CO2_DensityMode_CO2High,             // CO2�� ��  
	_fanSystemSt_CO2_DensityMode_CO2Low,              // CO2�� ��


	_fanSystemSt_GAS_Density_Low,                     // ���� �� : ��
	_fanSystemSt_GAS_Density_Middle,				  // ���� �� : ��
	_fanSystemSt_GAS_Density_High,					  // ���� �� : ��
	
	_model_fanSystem_KoKomTech,	     // ������	

};	

struct xsd_fanSystemProperty
{
	unsigned   _fanSystemDo_Power_Off                  : 1;	
	unsigned   _fanSystemDo_Power_On                   : 1;	
	unsigned   _fanSystemDo_AutoFan_ElectricHeat       : 1;		// �ڵ�ȯ�� ��� -> ���� ��ȯ
	unsigned   _fanSystemDo_AutoFan_GeneralFan         : 1;		// �ڵ�ȯ�� ��� -> �Ϲ� ȯ��

    unsigned   _fanSystemDo_FanMode_ElectricHeat       : 1;		// ȯ�� ���     -> ���� ��ȯ
	unsigned   _fanSystemDo_FanMode_GeneralFan         : 1;		// ȯ�� ���     -> �Ϲ� ȯ��
	unsigned   _fanSystemDo_FanMode_Schedule           : 1;		// ȯ�� ���     -> ������
	unsigned   _fanSystemDo_FanMode_SleepingMode       : 1;		// ȯ�� ���     -> ��ħ���

	unsigned   _fanSystemDo_Wind_Off				   : 1;		// ǳ�� ����
	unsigned   _fanSystemDo_Wind_Low				   : 1;		// ǳ�� ��
	unsigned   _fanSystemDo_Wind_Middle				   : 1;		// ǳ�� ��							
	unsigned   _fanSystemDo_Wind_High				   : 1;		// ǳ�� ��

	unsigned   _fanSystemDo_ReservationSet			   : 1;		// ���� ���� ���� ���
	unsigned   _fanSystemDo_ReservationSet_Release	   : 1;		// ���� ���� ����
	unsigned   _fanSystemDo_ReservationSet_Running	   : 1;		// ���� ���� ������
	unsigned   _fanSystemDo_ReservationSet_NotRunning  : 1;		// ���� ���� ���� ����

	unsigned   _fanSystemSt_Power_Off                  : 1;	
	unsigned   _fanSystemSt_Power_On                   : 1;	
	unsigned   _fanSystemSt_AutoFan_ElectricHeat       : 1;		// �ڵ�ȯ�� ��� -> ���� ��ȯ
	unsigned   _fanSystemSt_AutoFan_GeneralFan         : 1;		// �ڵ�ȯ�� ��� -> �Ϲ� ȯ��
	unsigned   _fanSystemSt_CurrentFanMode_AutoFan	   : 1;     // ȯ�� ���(�ڵ�)

    unsigned   _fanSystemSt_FanMode_ElectricHeat       : 1;		// ȯ�� ���     -> ���� ��ȯ
	unsigned   _fanSystemSt_FanMode_GeneralFan         : 1;		// ȯ�� ���     -> �Ϲ� ȯ��
	unsigned   _fanSystemSt_FanMode_Schedule           : 1;		// ȯ�� ���     -> ������
	unsigned   _fanSystemSt_FanMode_SleepingMode       : 1;		// ȯ�� ���     -> ��ħ���

	unsigned   _fanSystemSt_CurrentFanMode_Manual      : 1;     // ȯ�� ���(����)

	unsigned   _fanSystemSt_Wind_Off				   : 1;		// ǳ�� ����
	unsigned   _fanSystemSt_Wind_Low				   : 1;		// ǳ�� ��
	unsigned   _fanSystemSt_Wind_Middle				   : 1;		// ǳ�� ��							
	unsigned   _fanSystemSt_Wind_High				   : 1;		// ǳ�� ��

	unsigned   _fanSystemSt_ReservationSet			   : 1;		// ���� ���� ���� ���
	unsigned   _fanSystemSt_ReservationSet_Release	   : 1;		// ���� ���� ����
	unsigned   _fanSystemSt_ReservationSet_Running	   : 1;		// ���� ���� ������
	unsigned   _fanSystemSt_ReservationSet_NotRunning  : 1;		// ���� ���� ���� ����

	unsigned   _fanSystemSt_FanReservationSetMode_Hour : 1;
	unsigned   _fanSystemSt_FanReservationSetMode_Minute : 1;

	unsigned   _fanSystemSt_FanReservation_Hour		   : 7;		// ���� ���� �ð�
	unsigned   _fanSystemSt_FanReservation_Minute	   : 7;		// ���� ���� ��

	unsigned   _fanSystemSt_SensorMode_NotExistSenosr  : 1;     // ���� ��� : ���� ����
	unsigned   _fanSystemSt_SensorMode_GasSenosr	   : 1;     // ���� ��� : ���� ����
	unsigned   _fanSystemSt_SensorMode_CO2Senosr	   : 1;     // ���� ��� : CO2����
	unsigned   _fanSystemSt_DamperMode_NotExist		   : 1;	    // ���� ��� : ����
	unsigned   _fanSystemSt_DamperMode_Exist		   : 1;     // ���� ��� : ����
	unsigned   _fanSystemSt_FilterEmpressMode_Clean    : 1;		// ����/���� : û��
	unsigned   _fanSystemSt_FilterEmpressMode_Change   : 1;     // ����/���� : ��ȯ
	unsigned   _fanSystemSt_FilterEmpressMode_FEChange : 1;     // ����/���� : ���� ��ȯ
	unsigned   _fanSystemSt_FilterEmpressMode_Normal   : 1;     // ����      : ����

	unsigned   _fanSystemSt_AlarmMode_Normal           : 1;     // �˶� ���� : ����
	unsigned   _fanSystemSt_AlarmMode_HighSpeedMotor   : 1;     // �˶� ���� : �ޱ� ����
	unsigned   _fanSystemSt_AlarmMode_VentilationMotor : 1;     // �˶� ���� : ��� ����
	unsigned   _fanSystemSt_AlarmMode_DamperMotor      : 1;     // �˶� ���� : ���� ����
	unsigned   _fanSystemSt_AlarmMode_CO2              : 1;     // �˶� ���� : CO2
	unsigned   _fanSystemSt_AlarmMode_TemperatureMotor : 1;     // �˶� ���� : �µ� ����
	unsigned   _fanSystemSt_AlarmMode_OverHeating      : 1;     // �˶� ���� : ���� ����
	unsigned   _fanSystemSt_AlarmMode_DeviceProtect    : 1;     // �˶� ���� : ��ġ ��ȣ

	unsigned   _fanSystemSt_CO2_DensityMode_CO2High    : 1;     // CO2�� �� ���
	unsigned   _fanSystemSt_CO2_DensityMode_CO2Low     : 1;     // CO2�� �� ���

	unsigned   _fanSystemSt_CO2_Density_High		   : 7;	    // CO2�� ���� ��
	unsigned   _fanSystemSt_CO2_Density_Low            : 7;     // CO2�� ���� ��

	unsigned   _fanSystemSt_GAS_Density_Low			   : 1;     // ���� �� : ��
	unsigned   _fanSystemSt_GAS_Density_Middle		   : 1;     // ���� �� : ��
	unsigned   _fanSystemSt_GAS_Density_High           : 1;     // ���� �� : ��
};


class ns__fanSystem : public ns__rootDevice
{
public:

	struct xsd_fanSystemProperty devFanSystemProperty;

	enum ns__enum_fanSystem			model;								// �𵨸�

	enum ns__enum_fanSystem			fanSystemDo_Power;					// ���� / ����
	enum ns__enum_fanSystem	        fanSystemDo_AutoFan;				// �ڵ�ȯ��
	enum ns__enum_fanSystem     	fanSystemDo_FanMode;				// ȯ����
	enum ns__enum_fanSystem         fanSystemDo_FanWind;				//ǳ�� ��� : ǳ�� ���� -> �� -> �� -> ��
	enum ns__enum_fanSystem         fanSystemDo_FanReservationSet;	    //���� ���� ���� 	

	enum ns__enum_fanSystem			fanSystemSt_Power;					// ���� / ����
	enum ns__enum_fanSystem	        fanSystemSt_AutoFan;				// �ڵ�ȯ��
	enum ns__enum_fanSystem     	fanSystemSt_FanMode;				// ȯ����

	enum ns__enum_fanSystem     	fanSystemSt_CurrentFanMode;			// ȯ�� ���(�ڵ�, ����)����

	enum ns__enum_fanSystem         fanSystemSt_FanWind;				//ǳ�� ��� : ǳ�� ���� -> �� -> �� -> ��
	enum ns__enum_fanSystem         fanSystemSt_FanReservationSet;	    //���� ���� ���� 	

	enum ns__enum_fanSystem         fanSystemSt_FanReservationSetMode;  //���� ���� �ð� / �� üũ
	
	int fanReservation_Hour;											//���� ���� ���� : �ð� 
	int	fanReservation_Minute;											//���� ���� ���� : ��

	enum ns__enum_fanSystem         fanSystemSt_SensorMode;				//���� ��� : ���� ����, ���� ����, CO2 ����
	enum ns__enum_fanSystem         fanSystemSt_DamperMode;				//���� ��� : ���� ����, ���� ����

	enum ns__enum_fanSystem         fanSystemSt_FilterEmpressMode;		// ����/���� ��� : ���� û��, ���� ��ȯ, ���� ��ȯ

	enum ns__enum_fanSystem         fanSystemSt_AlarmMode;              // �˶� ���� : �ޱ� ����,������, ���۸���,CO2,�µ�����, ���� ����, ��ġ ��ȣ

	int  fanSystemSt_Temperature;										// ���� ������ �µ�(�ޱ� �µ�)

	enum ns__enum_fanSystem         fanSystemSt_CO2_DensityMode;        // CO2�� �� / �� üũ

	int  fanSystemSt_CO2_Density_High;								    // CO2�� ����
	int  fanSystemSt_CO2_Density_Low;                                   // CO2�� ����

	enum ns__enum_fanSystem         fanSystemSt_GAS_Density;			// GAS ��
	
	enum ns__enum_devError			fanSystemDevError;					//����̽� ����		

	enum ns__fanSystemFunc
	{
		f_fanSystemDo_Power,											//�������
		f_fanSystemDo_AutoFan,											//�ڵ�ȯ��
		f_fanSystemDo_FanMode,											//ȯ����
		f_fanSystemDo_FanWind,											//ǳ��
		f_fanSystemDo_FanReservationSet,								//���� ���� 

		f_fanSystemSt_Power,											//�������
		f_fanSystemSt_AutoFan,											//�ڵ�ȯ��
		f_fanSystemSt_FanMode,											//ȯ����
		f_fanSystemSt_FanWind,											//ǳ��
		f_fanSystemSt_FanReservationSet,								//���� ���� 

		f_fanSystemSt_SensorMode,									    // ����
		f_fanSystemSt_DamperMode,									    // ����
		f_fanSystemSt_FilterEmpress,									// ����/����
		f_fanSystemSt_AlarmMode,										// �˶� ����
		f_fanSystemSt_Temperature,										// �µ�
		f_fanSystemSt_CO2_Density,									    // CO2�� : ����, ����
		f_fanSystemSt_GAS_Density,									    // GAS ��
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
//	��� ����: NokSung Construction [�߱� �켺 �ε���] :  
//-------------------------------------------------------------
enum ns__enum_securitySensor
{
	
	_securitySensorSt_Detected,
	_securitySensorSt_UnDetected,
	
	_model_NokSung_XP830RS8I	        // �켺 �ε���
	
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

	enum ns__enum_securitySensor		model;								// �𵨸�
	enum ns__enum_securitySensor        securitySensorStSubChannel_One;		// ���� ���� / ���� ���� : Sub Id 1
	enum ns__enum_securitySensor        securitySensorStSubChannel_Two;		// ���� ���� / ���� ���� : Sub Id 2
	enum ns__enum_securitySensor        securitySensorStSubChannel_Three;	// ���� ���� / ���� ���� : Sub Id 3	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Four;	// ���� ���� / ���� ���� : Sub Id 4	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Five;	// ���� ���� / ���� ���� : Sub Id 5	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Six;		// ���� ���� / ���� ���� : Sub Id 6	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Seven;	// ���� ���� / ���� ���� : Sub Id 7	
	enum ns__enum_securitySensor        securitySensorStSubChannel_Eight;	// ���� ���� / ���� ���� : Sub Id 8	
	
	enum ns__enum_devError	securitySensorDevError;							// ����̽� ����	

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
//	�������
//-------------------------------------------------------------

// �̺�Ʈ�� �����ϱ����� ���
int ns__subcribeEvent(int port, int *out );

}
