
#include "SOAP_Service.h"

#include "soap/cmxDeviceServiceH.h"
#include "soap/cmxDeviceService.nsmap"
#include "soap/cmxDeviceServicecdsProxy.h"


struct 
{
	char*	string;
}

soap_sub_error[] =
{
	"Not supported",
	"Out of range",
	"Resource temporarily unavailable",
	"Parameter(s) mismatch",
	"No response", 
	"System internal error", 
	"Not specified"
};

SOAP_Service* SOAP_Service::_instance = 0;

SOAP_Service* SOAP_Service::Instance()
{

	if( _instance == 0) {

		_instance = new SOAP_Service();

	}

	return _instance;
}

void SOAP_Service::Instance_Close()
{	
	if( _instance != NULL )
	{
		Log(LOG::SERV, "SOAP_Service _instance delete\n");	
		delete _instance;
		_instance = NULL;		
	}

}

SOAP_Service::SOAP_Service()
{
	soap_service_runningFlag = TRUE;
}


SOAP_Service::~SOAP_Service()
{

}

void SOAP_Service::Close()
{
	soap_service_runningFlag = FALSE;
	usleep(200000);

	Instance_Close();
}

void *process_request(void *soap)
{ 
	pthread_detach(pthread_self());


	cmxDeviceService::cmxDeviceService_serve((struct soap*)soap);

	soap_destroy((struct soap*)soap); // dealloc C++ data
	soap_end((struct soap*)soap); // dealloc data and clean up
	soap_done((struct soap*)soap); // detach soap struct
	free(soap);

	//Log(LOG::SERV,"thread terminating....\n");
	return NULL;
}


#define TIME_CONNECT 5
#define TIME_SEND	 5
#define TIME_RECV	 5
#define BACKLOG (100) // Max. request backlog

void * SOAP_Service::soap_proc(void * arg)
{
	Log(LOG::SERV, "### SOAP_SERVICE:: THREAD STARTING [ INTERFACE THREAD ]\n");

	struct soap soap;
	soap_init(&soap);
	soap_set_namespaces(&soap, cmxDeviceService_namespaces);
	soap.bind_flags |= SO_REUSEADDR;

#ifdef DEBUG
	soap_set_recv_logfile(&soap, 	"recv.xml");
	soap_set_sent_logfile(&soap, 	"sent.xml");
//	soap_set_test_logfile(&soap, NULL);
#endif

	soap.connect_timeout = TIME_CONNECT;
	soap.send_timeout = TIME_SEND; // 60 seconds
	soap.recv_timeout = TIME_RECV; // 60 seconds
	//soap.accept_timeout = 3600; // server stops after 1 hour of inactivity
	//soap.max_keep_alive = 100; // max keep-alive sequence
	void *process_request(void*);

	struct soap *tsoap;
	pthread_t tid;

	int port = 9300;

	SOAP_SOCKET m, s;
	m = soap_bind(&soap, NULL, DS_PORT, BACKLOG);
		
	if (!soap_valid_socket(m))
	{
		Log(LOG::ERR, "!soap_valid_socket\n");
		exit(1);
	}
		
	//Log(LOG::SERV,  " SOCKET CONNECTION SUCCESSFULLY :: %d\n", m);

	for (;;)
	{ 

		s = soap_accept(&soap);
		if(!soap_valid_socket(s))
		{ 
			if (soap.errnum)
			{ 
				soap_print_fault(&soap, stderr);
				exit(1);
			}
				
			Log(LOG::ERR, "server timed out\n");
			break;
		}
			
		Log(LOG::SERV, "Thread accepts socket [%d] connection from IP %d.%d.%d.%d\n", s, (soap.ip>>24)&0xFF, (soap.ip>>16)&0xFF, (soap.ip>>8)&0xFF, soap.ip&0xFF);

		tsoap = soap_copy(&soap); // make a safe copy
	
		if (!tsoap)
			break;
	
		pthread_create(&tid, NULL, (void*(*)(void*))process_request, (void*)tsoap);
	}

	
	soap_done(&soap); // detach soap struct

	pthread_exit(0);

	return NULL;

}

void SOAP_Service::Start()
 {

	int ret;	

	if( (ret = pthread_attr_init( &soap_service_buf_thread_t) ) != 0 ) {

			Log(LOG::SERV, " CAN'T  CREATE THREAD ATTRIBUTE\n");
	}
	
	if( (ret = pthread_attr_setdetachstate( &soap_service_buf_thread_t, PTHREAD_CREATE_DETACHED) ) != 0 ) {
			
			Log(LOG::SERV, " CAN'T SET THREAD SET DETACH\n");
	}

	if( (ret = pthread_create( &soap_service_buf_thread, &soap_service_buf_thread_t, SOAP_Service::soap_proc, (void*) this) ) != 0 )
	{
		Log(LOG::SERV,  " CAN'T FAIL THREAD CREATE\n");
	}

	//Log(LOG::SERV,  " SUCCESSFULLY THREAD CREATE \n");

	(void)pthread_attr_destroy(&soap_service_buf_thread_t);		


 }

#if 1
namespace cmxDeviceService
{

void soapTimeout(struct soap* soap, int connect, int send, int recv)
{
	
	soap->connect_timeout = connect;
	soap->send_timeout = send; 
	soap->recv_timeout = recv; 
	
}

#define TIME_CONNECT 5
#define TIME_SEND	 5
#define TIME_RECV	 5

//-------------------------------------------------------------
//	실패리턴
//-------------------------------------------------------------
int send_fault(struct soap* soap, int err, char* detail)
{
	char code[10];
	sprintf(code,"%d", err);
	soap_sender_fault_subcode(soap, code, soap_sub_error[err].string, detail);

	return SOAP_FATAL_ERROR;

}
//-------------------------------------------------------------
//	binary data
//-------------------------------------------------------------
int base64Binary_malloc(struct soap* soap, struct xsd__base64Binary* bin, int size)
{
	bin->__ptr = (unsigned char*)soap_malloc(soap, size);
	if(!bin->__ptr)
		return 0;
	bin->__size = size ;
	return 1;
}

//-------------------------------------------------------------
//	string copy
//-------------------------------------------------------------
int soapString_malloc(struct soap * pSoap, char ** ppDst, char * pSrc)
{

	if(strlen(pSrc) <= 0)
		return 0;

	*ppDst = (char *)soap_malloc(pSoap, strlen(pSrc) + 1);

	if(*ppDst == 0)
		return 0;

	strcpy(*ppDst, pSrc);

	return 1;
}

//-------------------------------------------------------------
//	devicecategory, 디바이스 드라이버에서 지원 가능한 디바이스 목록 조회
//-------------------------------------------------------------


int ns__getDeviceCategoryCount(struct soap* soap, void* _, int* out)
{
	
	Log(LOG::SERV, "ns__getDeviceCategoryCount\n");

	*out = 1;
	
	return SOAP_OK;
}

int ns__getDeviceCategoryItem(struct soap* soap, int order, ns__getDeviceCategoryItemResponse* out)
{

	int ret = 0;
	Log(LOG::SERV, "ns__getDeviceCategoryItem\n");

	ret = (SOAP_Handler::Instance())->Get_DeviceCategory(&(out->_return));	

	if( ret != SOAP_OK) return send_fault(soap, ret, "");
	
	return ret;
}

int ns__getDeviceCategory(struct soap* soap, ns__deviceCategory in, ns__getDeviceCategoryResponse* out)
{
	int ret = 0;
	Log(LOG::SERV, "ns__getDeviceCategory\n");

	ret = (SOAP_Handler::Instance())->Get_DeviceCategory(&(out->_return));

	if( ret != SOAP_OK) return send_fault(soap, ret, "");

	return ret;
}

int ns__setDeviceCategory(struct soap* soap, ns__deviceCategory in, ns__setDeviceCategoryResponse* out)
{
	return SOAP_OK;
}
int ns__deviceCategoryEvent(struct soap* soap, ns__deviceCategory in, int* out)
{

	return SOAP_OK;
}

//-------------------------------------------------------------
//	boiler, 보일러
//-------------------------------------------------------------
int ns__getBoilerCount(struct soap* soap, void* _, int* out)
{
	Log(LOG::SERV, "ns__getBoilerCount\n");

	*out = (SOAP_Handler::Instance())->Get_Count(BOILER);

	return SOAP_OK;

}

int ns__getBoilerPropertyItem(struct soap* soap, int order, ns__getBoilerPropertyItemResponse* out)
{
	Log(LOG::SERV, "ns__getBoilerPropertyItem\n");
	int ret = 0;
	
	out->_return.dev = _boiler;
	out->_return.order = order;

	ret =(SOAP_Handler::Instance())->Get_Property( &(out->_return), &order );

	if( ret != SOAP_OK ) return send_fault(soap, ret, "");
	
	return SOAP_OK;
}

int ns__getBoilerItem(struct soap* soap, int order, ns__getBoilerItemResponse* out)
{
	int ret = 0;
	int dev_order = order;
	
	Log(LOG::SERV, "ns__getBoilerItem order = %d\n", order);	
	
	out->_return.dev = _boiler;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));		
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}
	return ret;

}

int ns__getBoiler(struct soap* soap, ns__boiler in, ns__getBoilerResponse* out)
{
	int ret = 0;
	int dev_order = in.order;
	
	Log(LOG::SERV, "ns__getBoiler\n");	
	
	out->_return.dev = _boiler;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));	
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}
	
	return ret;
	
}

int ns__setBoiler(struct soap* soap, ns__boiler in, ns__setBoilerResponse* out)
{
  	int ret = 0;
	int order = in.order;	

	Log(LOG::SERV, "ns__setBoiler\n");

	switch(in.func)
	{
	case f_boilerPower:		
		
		//Log(LOG::SERV, "NS__SETBOILER->F_BOILERPOWER : [%d]\n", order);
		
		in.dev = cmxDeviceService::_boiler;

		ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

		break;

	case f_boilerOutMode:
		
		//Log(LOG::SERV, "NS__SETBOILER->F_BOILEROUTMMODE : [%d]\n", order);

		in.dev = cmxDeviceService::_boiler;

		ret =  (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );		

		break;

	case f_boilerRequestedTemperature:
		
		//Log(LOG::SERV, "NS__SETBOILER->F_BOILERREQUESTEDTEMPERATURE : [%d]\n", in.boilerRequestedTemperature);

		in.dev = cmxDeviceService::_boiler;

		ret =  (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );	

		break;
	case f_boilerCurrentTemperature:
		
		//Log(LOG::SERV, "NS__SETBOILER->F_BOILERCURRENTTEMPERATURE : [%d]\n", order);

		return send_fault(soap, SOAP_E_NOT_SUPPORTED, " boiler : f_boilerCurrentTemperature not supported");	
		break;
	default:

		Log(LOG::ERR, "NS__GETBOILER->DEFAULT\n");

		// func의 잘못된 지정
		return send_fault(soap, SOAP_E_OUT_OF_RANGE	, " boiler : ns__setBoiler : out of range");	
	}	
	
	if( ret != SOAP_OK) 
	{
		return send_fault(soap,SOAP_E_TEMP_UNAVAIL, "ns__setGasValve Control Buffer Overflow");
	}

	return SOAP_OK;
}

int ns__boilerEvent(struct soap* soap, ns__boiler in, int* out)
{
	return SOAP_NO_METHOD;
}

//-------------------------------------------------------------
//	light, 전등
//-------------------------------------------------------------
int ns__getLightCount(struct soap* soap, void* _, int* out)
{
	Log(LOG::SERV, "ns__getLightCount\n");

	*out = (SOAP_Handler::Instance())->Get_Count(LIGHT);
		
	return SOAP_OK;
}

int ns__getLightPropertyItem(struct soap* soap, int order, ns__getLightPropertyItemResponse* out)
{
	Log(LOG::SERV, "ns__getLightPropertyItem\n");
	int ret = SOAP_OK;
	
	out->_return.dev = _light;
	out->_return.order = order;

	ret =(SOAP_Handler::Instance())->Get_Property( &(out->_return), &order );

	if( ret != SOAP_OK ) return send_fault(soap, ret, "");
	
	return SOAP_OK;
}

int ns__getLightItem(struct soap* soap, int order, ns__getLightItemResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = order;
	
	Log(LOG::SERV, "ns__getLightItem order = %d\n", order);	
	
	out->_return.dev = _light;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));		
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}
	
	return ret;
	
}

int ns__getLight(struct soap* soap, ns__light in, ns__getLightResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = in.order;
	
	Log(LOG::SERV, "ns__getLight\n");	
	
	out->_return.dev = _light;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));	
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}
	return ret;
	
}

int ns__setLight(struct soap* soap, ns__light in, ns__setLightResponse* out)
{
	int ret = SOAP_OK;
	int order = in.order;	

	Log(LOG::SERV, "ns__setLight\n");

	switch(in.func)
	{
		case f_lightPower:	
			//Log(LOG::SERV, "CLIENT -> DEVICE SERVICE :: NS__SETLIGHT->F_LIGHTPOWER, ID -> %d\n", order);
		
			in.dev = cmxDeviceService::_light;

			ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );
		
		break;	

		case f_dimmableLevel:
			//Log(LOG::SERV, "CLIENT -> DEVICE SERVICE :: NS__SETLIGHT->F_DIMMABLELEVEL, ID -> : %d\n", order);
		
			in.dev = cmxDeviceService::_light;

			ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );
		
		break;	

		default:
			Log(LOG::ERR,"NS__SETLIGHT->DEFAULT");

			// func의 잘못된 지정
			return send_fault(soap, SOAP_E_PARAM_MISMATCH, " ns__setLight -> miss func");
		break;
	}
	
	if( ret != SOAP_OK) 
	{
		return send_fault(soap,SOAP_E_TEMP_UNAVAIL, "ns__setGasValve Control Buffer Overflow");
	}

	return SOAP_OK;
}

int ns__lightEvent(struct soap* soap, ns__light in,  int* out)
{
	return SOAP_NO_METHOD;
}

//-------------------------------------------------------------
//	gasValve, 가스밸브
//-------------------------------------------------------------
int ns__getGasValveCount(struct soap* soap, void* _, int* out)
{
	Log(LOG::SERV, "ns__getGasValveCount\n");

	*out = (SOAP_Handler::Instance())->Get_Count(GAS);

	return SOAP_OK;
}

int ns__getGasValvePropertyItem(struct soap* soap, int order, ns__getGasValvePropertyItemResponse * out)
{
	Log(LOG::SERV, "ns__getGasValvePropertyItem\n");
	int ret = SOAP_OK;
	
	out->_return.dev = _gasValve;
	out->_return.order = order;

	ret =(SOAP_Handler::Instance())->Get_Property( &(out->_return), &order );

	if( ret != SOAP_OK ) return send_fault(soap, ret, "");
	
	return SOAP_OK;
}

int ns__getGasValveItem(struct soap* soap, int order, ns__getGasValveItemResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = order;
	
	Log(LOG::SERV, "ns__getGasValveItem order = %d\n", order);	
	
	out->_return.dev = _gasValve;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));		
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}

	return ret;
	
}

int ns__getGasValve(struct soap* soap, ns__gasValve in, ns__getGasValveResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = in.order;
	
	Log(LOG::SERV, "ns__getGasValve\n" );	
	
	out->_return.dev = _gasValve;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));	
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}

	return ret;	
}

int ns__setGasValve(struct soap* soap, ns__gasValve in, ns__setGasValveResponse* out)
{
	
	int ret = SOAP_OK;
	int order = in.order;	

	Log(LOG::SERV, "ns__setGasValve\n" );	

	switch(in.func)
	{
		case f_gvDo:		

			//Log(LOG::SERV, "NS__SETLIGHT->F_GADO : [%d]\n", order);
		
			in.dev = cmxDeviceService::_gasValve;

			ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

		break;
		
		default:
			Log(LOG::ERR, "NS__SETGASVALVE->DEFAULT\n");

			// func의 잘못된 지정
			return send_fault(soap, SOAP_E_PARAM_MISMATCH, " ns__setGasValve -> Miss Func");
		break;
	}

	if( ret != SOAP_OK) 
	{
		if( ret == SOAP_E_TEMP_UNAVAIL)
			return send_fault(soap,SOAP_E_TEMP_UNAVAIL, "ns__setGasValve Control Buffer Overflow");
 	}

	return SOAP_OK;
}

int ns__gasValveEvent(struct soap* soap, ns__gasValve in, int* out)
{
	return SOAP_NO_METHOD;
}

//-------------------------------------------------------------
//	curtain, 커튼
//-------------------------------------------------------------
int ns__getCurtainCount(struct soap* soap, void* _, int* out)
{
	Log(LOG::SERV, "ns__getCurtainCount\n");

	*out = (SOAP_Handler::Instance())->Get_Count(CURTAIN);

	return SOAP_OK;
}

int ns__getCurtainPropertyItem(struct soap* soap, int order, ns__getCurtainPropertyItemResponse* out)
{
	Log(LOG::SERV, "ns__getCurtainPropertyItem\n");
	int ret = SOAP_OK;
	
	out->_return.dev = _curtain;
	out->_return.order = order;

	ret =(SOAP_Handler::Instance())->Get_Property( &(out->_return), &order );

	if( ret != SOAP_OK ) return send_fault(soap, ret, "");
	
	return SOAP_OK;
}

int ns__getCurtainItem(struct soap* soap, int order, ns__getCurtainItemResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = order;
	
	Log(LOG::SERV, "ns__getCurtainItem order = %d\n", order);	
	
	out->_return.dev = _curtain;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));		
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}

	return ret;
}

int ns__getCurtain(struct soap* soap, ns__curtain in, ns__getCurtainResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = in.order;
	
	Log(LOG::SERV, "ns__getCurtain\n" );	
	
	out->_return.dev = _curtain;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));	
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}

	return ret;	
}

int ns__setCurtain(struct soap* soap, ns__curtain in, ns__setCurtainResponse* out)
{
	
	int ret = SOAP_OK;
	int order = in.order;	

	Log(LOG::SERV, "ns__setCurtain\n" );	

	switch(in.func)
	{
		case f_gvDo:		

			//Log(LOG::SERV, "NS__SETLIGHT->F_GADO : [%d]\n", order);
		
			in.dev = cmxDeviceService::_curtain;

			ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

		break;
		
		default:
			Log(LOG::ERR, "NS__SETGASVALVE->DEFAULT\n");

			// func의 잘못된 지정
			return send_fault(soap, SOAP_E_PARAM_MISMATCH, " ns__setGasValve -> Miss Func");
		break;
	}

	if( ret != SOAP_OK) 
	{
		if( ret == SOAP_E_TEMP_UNAVAIL)
			return send_fault(soap,SOAP_E_TEMP_UNAVAIL, "ns__setGasValve Control Buffer Overflow");
 	}

	return SOAP_OK;
}

int ns__curtainEvent(struct soap* soap, ns__curtain in, int* out)
{
	return SOAP_OK;
}
//-------------------------------------------------------------
//	bundleLight, 일괄소등
//-------------------------------------------------------------
int ns__getBundleLightCount(struct soap* soap, void* _, int* out)
{
	Log(LOG::SERV, "ns__getBundleLightCount\n");

	*out = (SOAP_Handler::Instance())->Get_Count(BUNDLELIGHT);

	return SOAP_OK;
}

int ns__getBundleLightPropertyItem(struct soap* soap, int order, ns__getBundleLightPropertyItemResponse* out)
{
	Log(LOG::SERV, "ns__getBundleLightPropertyItem\n");
	int ret = SOAP_OK;
	
	out->_return.dev = _bundleLight;
	out->_return.order = order;

	ret =(SOAP_Handler::Instance())->Get_Property( &(out->_return), &order );

	if( ret != SOAP_OK ) return send_fault(soap, ret, "");
	
	return SOAP_OK;
}

int ns__getBundleLightItem(struct soap* soap, int order, ns__getBundleLightItemResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = order;
	
	Log(LOG::SERV, "ns__getBundleLightItem order = %d\n", order);	

	out->_return.dev = _bundleLight;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));		

	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}

	return ret;
 }

int ns__getBundleLight(struct soap* soap, ns__bundleLight in, ns__getBundleLightResponse* out)
{
	int ret = SOAP_OK;
	int dev_order = in.order;
	
	Log(LOG::SERV, "ns__getBundleLight\n");	
	
	out->_return.dev = _bundleLight;
	out->_return.order = dev_order;
	
	ret = (SOAP_Handler::Instance())->Get_Item(soap, &(out->_return));	
		
	if( ret != SOAP_OK) {
			
		if( ret == SOAP_IOB ) {
			return send_fault(soap, ret, "Order : Out of Range ");

		} else if( ret == SOAP_FATAL_ERROR) {
			
			return send_fault(soap, ret, "Order : UnKnown Interface Type ");
		}
	}
	return ret;
}

int ns__setBundleLight(struct soap* soap, ns__bundleLight in, class ns__setBundleLightResponse* out)
{
	int order = in.order;	
	int ret = SOAP_OK;

	 Log(LOG::SERV, "ns__setBundleLight\n");	

	switch(in.func)
	{
		case f_bundleLightPower :				

			in.dev = cmxDeviceService::_bundleLight;

			 ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

			break;
		
		case f_bundleLightExit :

			in.dev = cmxDeviceService::_bundleLight;

			 ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

			break;

		case f_readyEnergyPower :

			in.dev = cmxDeviceService::_bundleLight;

			 ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

		break;

		case f_gasValve :

			in.dev = cmxDeviceService::_bundleLight;

			 ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

		break;

		case f_bundleLightElevator :

			in.dev = cmxDeviceService::_bundleLight;

			 ret = (SOAP_Handler::Instance())->Set_Dev( (cmxDeviceService::ns__rootDevice*)&in );

		break;

		default:
			Log(LOG::ERR,"NS__SETBUNDLELIGHT->DEFAULT");

			// func의 잘못된 지정
			return send_fault(soap, SOAP_E_PARAM_MISMATCH, " ns__setBundleLight -> miss func");

			break;
	}
 
	if( ret != SOAP_OK) 
	{
		if( ret == SOAP_E_TEMP_UNAVAIL)
			return send_fault(soap,SOAP_E_TEMP_UNAVAIL, "ns__setBundleLight Control Buffer Overflow");
	}

 	return SOAP_OK;  
}

int ns__BundleLightEvent(struct soap* soap, ns__bundleLight in, int* out)
{
	return SOAP_OK;
}

//-------------------------------------------------------------
//	fancontroler, 환기 시스템
//-------------------------------------------------------------

int ns__getFanSystemCount(struct soap* soap, void* _, int* out)
{
	return SOAP_OK;
}

int ns__getFanSystemPropertyItem(struct soap* soap, int order, ns__getFanSystemPropertyItemResponse* out)
{
	return SOAP_OK;
}

int ns__getFanSystemItem(struct soap* soap, int order, ns__getFanSystemItemResponse* out)
{
	return SOAP_OK;		
}

int ns__getFanSystem(struct soap* soap, ns__fanSystem in, ns__getFanSystemResponse* out)
{
	return SOAP_OK;	
}
int ns__setFanSystem(struct soap* soap, ns__fanSystem in, class ns__setFanSystemResponse* out)
{
	return SOAP_OK;  
}

int ns__FanSystemEvent(struct soap* soap, ns__fanSystem in, int* out)
{
	return SOAP_OK;
}

//-------------------------------------------------------------
//	systemaircon, 시스템에어컨
//-------------------------------------------------------------

int ns__getSystemAirconCount(struct soap* soap, void* _, int* out)
{
	return SOAP_OK;
	
}

int ns__getSystemAirconPropertyItem(struct soap* soap, int order, ns__getSystemAirconPropertyItemResponse* out)
{
	return SOAP_OK;
	 
}

int ns__getSystemAirconItem(struct soap* soap, int order, ns__getSystemAirconItemResponse* out)
{
	return SOAP_OK;		
}

int ns__getSystemAircon(struct soap* soap, ns__systemAircon in, ns__getSystemAirconResponse* out)
{
	return SOAP_OK;	 
}

int ns__setSystemAircon(struct soap* soap, ns__systemAircon in, class ns__setSystemAirconResponse* out)
{
	return SOAP_OK;  
}

int ns__systemAirconEvent(struct soap* soap, ns__systemAircon in, int* out)
{
	return SOAP_OK;  
}

//-------------------------------------------------------------
//	sensorEmer, 방범센서
//-------------------------------------------------------------
int ns__getSensorEmerCount(struct soap* soap, void* _, int* out)
{
	return SOAP_OK;

}

int ns__getSensorEmerItem(struct soap* soap, int order, ns__getSensorEmerItemResponse* out)
{
	return SOAP_OK;

}

int ns__getSensorEmer(struct soap* soap, ns__sensorEmer in, ns__getSensorEmerResponse* out)
{
	return SOAP_OK;

}

int ns__setSensorEmer(struct soap* soap, ns__sensorEmer in, ns__setSensorEmerResponse* out)
{
	return SOAP_OK;

}

int ns__sensorEmerEvent(struct soap* soap, ns__sensorEmer in, int* out)
{
	return SOAP_NO_METHOD;
}


//-------------------------------------------------------------
//	subcriber
//-------------------------------------------------------------
int ns__subcribeEvent(struct soap * soap, int port, int * out)
{
	char tmp[30];
	Log(LOG::SERV, "ns__subcribeEvent\n");
	
	sprintf(tmp, "http://%d.%d.%d.%d:%d", 
				(soap->ip>>24)&0xFF, (soap->ip>>16)&0xFF, (soap->ip>>8)&0xFF, soap->ip&0xFF, port);

	(SOAP_Handler::Instance())->Add_Subscriber(tmp);

	return SOAP_OK;
}

}

#endif

