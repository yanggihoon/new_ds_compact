
#include "SOAP_Handler.h"

#define TIME_CONNECT 5
#define TIME_SEND	 5
#define TIME_RECV	 5

void soapTimeout(struct soap* soap, int connect, int send, int recv);
static char * event_end_point = "http://127.0.0.1:29701";


SOAP_Handler* SOAP_Handler::_instance = 0;


SOAP_Handler* SOAP_Handler::Instance()
{

	if( _instance == 0) {

		_instance = new SOAP_Handler();

	}

	return _instance;
}

void SOAP_Handler::Instance_Close()
{	
	if( _instance != NULL )
	{
		delete _instance;
		Log(LOG::HND, "SOAP_Handler _instance delete\n");	
		_instance = NULL;		
	}

}

SOAP_Handler::SOAP_Handler()
{
	soap_handler_runningFlag = TRUE;
	g_subscriberList.push_back(event_end_point);
	pthread_mutex_init(&control_mutex, NULL);
	pthread_mutex_init(&event_mutex, NULL);
	pthread_mutex_init(&subscribe_mutex, NULL);	
}


SOAP_Handler::~SOAP_Handler()
{

}

void SOAP_Handler::Close()
{
	soap_handler_runningFlag = FALSE;
	usleep(200000);

	pthread_mutex_destroy(&event_mutex);
	pthread_mutex_destroy(&control_mutex);		
	pthread_mutex_destroy(&subscribe_mutex);


	Instance_Close();
}

void soapTimeout(struct soap* soap, int connect, int send, int recv)
{
	
	soap->connect_timeout = connect;
	soap->send_timeout = send; 
	soap->recv_timeout = recv; 
	
}

void SOAP_Handler::soap_print(unsigned char cmd_mode, enum DEVICE_NAME device_name, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	char str[10];
	if(cmd_mode == CONTROL)
		sprintf(str,"CONTROL");
	else if(cmd_mode == EVENT)
		sprintf(str, "EVENT");

	switch(device_name)
	{
		case LIGHT:
		{
			switch(function1)
			{
				case LIGHT_MODE_BINARY:
				{
					if(function2 == LIGHT_POWER_EVENT)
					{
						if(function3 == LIGHT_POWER_ON)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_OFF)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLON)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLOFF)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);							
						}
						else
							Log(LOG::ERR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, WRONG_FUNCTION\n", str, order);
					}
					else
						Log(LOG::ERR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, WRONG_EVENT\n", str, order);

					break;
				}
				
				case LIGHT_MODE_DIMMABLE:
				{
					if(function2 == LIGHT_POWER_EVENT)
					{
						if(function3 == LIGHT_POWER_ON)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_OFF)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLON)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLOFF)
						{
							Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);							
						}								
						else
							Log(LOG::ERR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, WRONG_FUNCTION\n", str, order);
					}
					else if(function2 == LIGHT_DIMMING_EVENT)
					{
						Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, LIGHT_DIMMING_LEVEL %d\n", str, order, function3);
						//Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, LIGHT_DIMMING_LEVEL %d\n", str, order, function3);
						//Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, LIGHT_DIMMING_LEVEL %d\n", str, order, function3);
						//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, LIGHT_DIMMING_LEVEL %d\n", str, order, function3);						
					}
					else
						Log(LOG::ERR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, WRON_EVENT\n", str, order);

					break;	
				}

				default:
					Log(LOG::ERR, "%s : LIGHT [%d], WRONG_EVENT\n", str, order);
					break;
			}
			break;
		}
		
		case GAS:
		{
			switch(function1)
			{
				case GAS_CLOSE:
					{
						Log(LOG::HND, "%s : GAS [%d], GAS_CLOSE\n", str, order);
						//Log(LOG::MGR, "%s : GAS [%d], GAS_CLOSE\n", str, order);
						//Log(LOG::INFO, "%s : GAS [%d], GAS_CLOSE\n", str, order);
						//Log(LOG::PRTCL, "%s : GAS [%d], GAS_CLOSE\n", str, order);						
					}
					break;

				case GAS_OPEN:
					{
						Log(LOG::HND, "%s : GAS [%d], GAS_OPEN\n", str, order);
						//Log(LOG::MGR, "%s : GAS [%d], GAS_OPEN\n", str, order);
						//Log(LOG::INFO, "%s : GAS [%d], GAS_OPEN\n", str, order);
						//Log(LOG::PRTCL, "%s : GAS [%d], GAS_OPEN\n", str, order);						
					}
					break;	

				case GAS_LEAK:
					{
						Log(LOG::HND, "%s : GAS [%d], GAS_LEAK\n", str, order);
						//Log(LOG::MGR, "%s : GAS [%d], GAS_LEAK\n", str, order);
						//Log(LOG::INFO, "%s : GAS [%d], GAS_LEAK\n", str, order);
						//Log(LOG::PRTCL, "%s : GAS [%d], GAS_LEAK\n", str, order);						
					}
					break;

				default:
					Log(LOG::ERR, "%s : GAS [%d], WRONG_ACTION\n", str, order);
					break;
			}
			break;
		}

		case BOILER:
		{
			switch(function1)
			{
				case BOILER_POWER_EVENT:
				{
					if(function2 == BOILER_POWER_ON)
					{
						Log(LOG::HND, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::MGR, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::INFO, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
					else if(function2 == BOILER_POWER_OFF)
					{
						Log(LOG::HND, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::MGR, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::INFO, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
				}
				break;

				case BOILER_OUT_EVENT:
				{
					if(function2 == BOILER_OUT_SET)
					{
						Log(LOG::HND, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::MGR, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::INFO, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
					else if(function2 == BOILER_OUT_RELEASE)
					{
						Log(LOG::HND, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::MGR, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::INFO, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
				}
				break;	

				case BOILER_RTEMP_EVENT:
				{
					Log(LOG::HND, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::MGR, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::INFO, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
				}
				break;

				case BOILER_CTEMP_EVENT:
				{
					Log(LOG::HND, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::MGR, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::INFO, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
				}
				break;

				default:
					Log(LOG::ERR, "%s : BOILER [%d], WRONG_ACTION\n", str, order);
					break;
			}
		}
		break;

		case BUNDLELIGHT:
		{
			switch(function1)
			{
				case BUNDLELIGHT_POWER_EVENT:
				{
					if(function2 == BUNDLELIGHT_POWER_ON)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_POWER_OFF)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_POWER_ALLON)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_POWER_ALLOFF)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLOFF \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLOFF \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLOFF \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLOFF \n", str, order);						
					}
					else
						Log(LOG::ERR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, WRONG_FUNCTION \n", str, order);
					break;
				}

				case BUNDLELIGHT_READYPOWER_EVENT:
				{
					if(function2 == BUNDLELIGHT_READYPOWER_ON)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_OFF)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_ALLON)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_ALLOFF)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLOFF \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLOFF \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLOFF \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLOFF \n", str, order);						
					}
					else
						Log(LOG::ERR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, WRONG_FUNCTION \n", str, order);
					break;
				}	

				case BUNDLELIGHT_OUT_EVENT:
				{
					if(function2 == BUNDLELIGHT_OUT_REQUEST)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_REQUEST_SUCCESS)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_REQUEST_FAIL)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_SET)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_CANCEL)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_CANCEL \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_CANCEL \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_CANCEL \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_CANCEL \n", str, order);						
					}
					else
						Log(LOG::ERR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, WRONG_FUNCTION \n", str, order);
					break;
				}

				case BUNDLELIGHT_GASCLOSE_EVENT:
				{
					if(function2 == BUNDLELIGHT_GASCLOSE_REQUEST)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);						
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_GASCLOSE_REQUEST_FAIL)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL \n", str, order);						
					}
					else
						Log(LOG::ERR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, WRONG_FUNCTION \n", str, order);						
					break;
				}

				case BUNDLELIGHT_ELEVATORCALL_EVENT:
				{
					if(function2 == BUNDLELIGHT_ELEVATORCALL_UP_REQUEST)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);						
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);						
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);						
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);
					}
					else if(function2 == BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);						
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL)
					{
						Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL \n", str, order);
						//Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL \n", str, order);
						//Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL \n", str, order);						
					}
					else
						Log(LOG::ERR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, WRONG_FUNCTION \n", str, order);
					break;
				}

				default:
					Log(LOG::ERR, "%s : BUNDLELIGHT [%d], WRONG EVENT \n", str, order);
					break;
			}
		}
		break;

		case CURTAIN:
		{
			switch(function1)
			{
				case CURTAIN_ACTION_EVENT:
				{
					if(function2 == CURTAIN_OPEN)
					{
						Log(LOG::HND, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
						//Log(LOG::MGR, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
						//Log(LOG::INFO, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
						//Log(LOG::PRTCL, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
					}
					else if(function2 == CURTAIN_CLOSE)
					{
						Log(LOG::HND, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
						//Log(LOG::MGR, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
						//Log(LOG::INFO, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
						//Log(LOG::PRTCL, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
					}
					else if(function2 == CURTAIN_STOP)
					{
						Log(LOG::HND, "%s : CURTAIN [%d], CURTAIN_STOP\n", str, order);
						//Log(LOG::MGR, "%s : CURTAIN [%d], CURTAIN_STOP\n", str, order);
						//Log(LOG::INFO, "%s : CURTAIN [%d], CURTAIN_STOP\n", str, order);
						//Log(LOG::PRTCL, "%s : CURTAIN [%d], CURTAIN_STOP\n", str, order);
					}

				}
				break;

				default:
					Log(LOG::ERR, "%s : CURTAIN [%d], WRONG_ACTION\n", str, order);
					break;
			}
		}
		break;

		
		default:
			Log(LOG::ERR, "%s : WRONG DEVICE \n", str);
			break;
	}
}

/***************************************************************************************/
// Soap Control
/*************************************************************************************/
int SOAP_Handler::Add_Control(Control control)
{
	pthread_mutex_lock(&control_mutex);							

	if(g_controlList.size() >= MAX_CONTROL_LIST)
	{
		pthread_mutex_unlock(&control_mutex);
		Log(LOG::ERR, "### SOAP_HANDLER:: Over Max Control List\n");	
		return -1;
	}

	g_controlList.push_back(control);

	pthread_mutex_unlock(&control_mutex);

	return 1;
}

void SOAP_Handler::Control_End_Proc(list<Control>::iterator it)
{
	int result;

	result = NotifyControlToDevice(it->device_name, it->order, it->function1, it->function2, it->function3, it->function4);
}

void * SOAP_Handler::control_run(void * arg)
{
	Log(LOG::HND, "### SOAP_HANDLER:: CONTROL THREAD STARTING [ INTERFACE THREAD ]\n");

	int ret = 0, result;
	SOAP_Handler * s_handler = ( SOAP_Handler*)arg;
	list<Control>::iterator it;
	
	while( s_handler->soap_handler_runningFlag ) {

		if( s_handler->g_controlList.size() <= 0 ) {

			usleep(CONTROL_TIME);		
			
			continue;
		}

		Log(LOG::HND, "### SOAP_HANDLER:: g_controlList.size = %d\n", s_handler->g_controlList.size());
		it = s_handler->g_controlList.begin();

		s_handler->soap_print(CONTROL, it->device_name, it->order, it->function1, it->function2, it->function3, it->function4);
	
		//send control to
		s_handler->Control_End_Proc(it);

		switch(it->device_name)
		{
			case LIGHT:
				Log(LOG::HND, "### SOAP_HANDLER:: soap control sent 'LIGHT', order = %d\n", it->order);
				break;

			case GAS:
				Log(LOG::HND, "### SOAP_HANDLER:: soap control sent 'GAS', order = %d\n", it->order);
				break;

			case BOILER:
				Log(LOG::HND, "### SOAP_HANDLER:: soap control sent 'BOILER', order = %d\n", it->order);
				break;

			case BUNDLELIGHT:
				Log(LOG::HND, "### SOAP_HANDLER:: soap control sent 'BUNDLELIGHT', order = %d\n", it->order);
				break;				

			case CURTAIN:
				Log(LOG::HND, "### SOAP_HANDLER:: soap control sent 'CURTAIN', order = %d\n", it->order);
				break;				
	
		}

		usleep(CONTROL_TIME);	

		pthread_mutex_lock(&s_handler->control_mutex);

		s_handler->g_controlList.pop_front();

		pthread_mutex_unlock(&s_handler->control_mutex);
		Log(LOG::HND, "### SOAP_HANDLER:: Delete Control List\n");
		Log(LOG::SEP, "");
	}	

	pthread_exit(NULL);
	Log(LOG::HND, "### SOAP_HANDLER:: CONTROL THREAD ENDING [ INTERFACE THREAD ]\n");
	return NULL;
}


void SOAP_Handler::Control_Start()
{
	int ret;	

	if( (ret = pthread_attr_init( &control_buf_thread_t) ) != 0 ) {

			Log(LOG::HND, "CONTROL CAN'T  CREATE THREAD ATTRIBUTE\n");
	}
	
	if( (ret = pthread_attr_setdetachstate( &control_buf_thread_t, PTHREAD_CREATE_DETACHED) ) != 0 ) {
			
			Log(LOG::HND, " CONTROLCAN'T SET THREAD SET DETACH\n");
	}

	if( (ret = pthread_create( &control_buf_thread, &control_buf_thread_t, SOAP_Handler::control_run, (void*) this) ) != 0 )
	{
		Log(LOG::HND,  "CONTROL CAN'T FAIL THREAD CREATE\n");
	}

	//Log(LOG::HND,  "CONTROL SUCCESSFULLY THREAD CREATE \n");

	(void)pthread_attr_destroy(&control_buf_thread_t);		

 }

/***************************************************************************************/
// Soap Event
/**************************************************************************************/
int SOAP_Handler::Add_Event(Event event)
{
	pthread_mutex_lock(&event_mutex);
	
	if(g_eventList.size() >= MAX_EVENT_LIST)
	{
		pthread_mutex_unlock(&event_mutex);
		Log(LOG::ERR, "### SOAP_HANDLER:: Over Max Event List\n");	
		return -1;
	}

	g_eventList.push_back(event);

	pthread_mutex_unlock(&event_mutex);

	return 1;
}

int SOAP_Handler::Soap_Event_Parser(list<Event>::iterator it)
{
	unsigned int current_device_cnt;
	int parsing_success = TRUE;
	vector<string> member;
	vector<string>::iterator member_it;
	Subscribe_Copy(&member);
	cmxDeviceService::ns__rootDevice *rootDevice;
	D_Item d_item;
	member_it = member.begin();
	current_device_cnt = Get_Count(it->device_name);
	enum DEVICE_PROTOCOL dProtocol;

	if(it->order > current_device_cnt)
	{
		Log(LOG::ERR, "### SOAP_HANDLER:: Over Current Device Cnt\n");	
		return FALSE;
	}

	soap_print(EVENT, it->device_name, it->order, it->function1, it->function2, it->function3, it->function4);

	switch(it->device_name)
	{
		case LIGHT:
		{
			cmxDeviceService::ns__light *lightEvent = new cmxDeviceService::ns__light;

			lightEvent->order = it->order;
			lightEvent->dev = cmxDeviceService::_light;

			dProtocol = get_protocol(LIGHT);	
			if(dProtocol == COMMAX)
			{
				lightEvent->proto = cmxDeviceService::_protoCommax;		
				lightEvent->model = cmxDeviceService::_model_Light_JoongAngControl;		
			}
			lightEvent->intf = cmxDeviceService::_intfRS485;			


			switch(it->function1)
			{
				case LIGHT_MODE_BINARY:
				{
					if(it->function2 == LIGHT_POWER_EVENT)
					{
						lightEvent->func = cmxDeviceService::f_lightPower;
						lightEvent->lightSwitchMode = cmxDeviceService::_lightSwitch_Binary;		

						if(it->function3 == LIGHT_POWER_ON)
						{
							lightEvent->lightPower = cmxDeviceService::_lightPower_On;
						}
						else if(it->function3 == LIGHT_POWER_OFF)
						{
							lightEvent->lightPower = cmxDeviceService::_lightPower_Off;
						}
						else
							parsing_success = FALSE;
					}

					break;
				}
				
				case LIGHT_MODE_DIMMABLE:
				{
					if(it->function2 == LIGHT_POWER_EVENT)
					{
						lightEvent->func = cmxDeviceService::f_lightPower;
						lightEvent->lightSwitchMode = cmxDeviceService::_lightSwitch_Dimmable;		

						if(it->function3 == LIGHT_POWER_ON)
						{
							lightEvent->lightPower = cmxDeviceService::_lightPower_On;
							lightEvent->dimmableLevel = it->function4;
						}
						else if(it->function3 == LIGHT_POWER_OFF)
						{
							lightEvent->lightPower = cmxDeviceService::_lightPower_Off;
						}
						else
							parsing_success = FALSE;
					}
					else if(it->function2 == LIGHT_DIMMING_EVENT)
					{
						//lightEvent->func = cmxDeviceService::f_dimmableLevel;
						lightEvent->func = cmxDeviceService::f_lightPower;						
						lightEvent->lightSwitchMode = cmxDeviceService::_lightSwitch_Dimmable;		
						lightEvent->lightPower = cmxDeviceService::_lightPower_On;
						lightEvent->dimmableLevel = it->function3;
					}
					else 
						parsing_success = FALSE;

					break;
				}
			}

			rootDevice = (cmxDeviceService::ns__rootDevice*)lightEvent;
			
			break;
		}
		
		case GAS:
		{
			cmxDeviceService::ns__gasValve *gasEvent = new cmxDeviceService::ns__gasValve;

			gasEvent->dev = cmxDeviceService::_gasValve;

			dProtocol = get_protocol(GAS);	
			if(dProtocol == COMMAX)
			{
				gasEvent->proto = cmxDeviceService::_protoCommax;		
				gasEvent->model = cmxDeviceService::_model_GasValve_Shinwoo;
			}
			gasEvent->intf = cmxDeviceService::_intfRS485;			
			gasEvent->order = it->order;

			switch(it->function1)
			{
				case GAS_CLOSE:
					gasEvent->func = cmxDeviceService::f_gvDo;
					gasEvent->gvDo = cmxDeviceService::_gvDo_Close;
					break;

				case GAS_OPEN:
					gasEvent->func = cmxDeviceService::f_gvDo;
					gasEvent->gvDo = cmxDeviceService::_gvDo_Open;
					break;	

				case GAS_LEAK:
					gasEvent->func = cmxDeviceService::f_gvAlarm;
					gasEvent->gvDo = cmxDeviceService::_gvAlarm_On;
					break;

				default:
					parsing_success = FALSE;
					break;
			}

			rootDevice = (cmxDeviceService::ns__rootDevice*)gasEvent;

			break;
		}

		case BOILER:
		{
			cmxDeviceService::ns__boiler *boilerEvent = new cmxDeviceService::ns__boiler;

			boilerEvent->dev = cmxDeviceService::_boiler;

			dProtocol = get_protocol(BOILER);	
			if(dProtocol == COMMAX)
			{
				boilerEvent->proto = cmxDeviceService::_protoCommax;		
				boilerEvent->model = cmxDeviceService::_model_boiler_HansungCisco;			
			}
			boilerEvent->intf = cmxDeviceService::_intfRS485;			

			
			boilerEvent->order = it->order;		

			switch(it->function1)
			{
				case BOILER_POWER_EVENT:
				{
					boilerEvent->func = cmxDeviceService::f_boilerPower;
					boilerEvent->boilerRequestedTemperature = it->function3;
					boilerEvent->boilerCurrentTemperature = it->function4;					
					if(it->function2 == BOILER_POWER_ON)
						boilerEvent->boilerPower = cmxDeviceService::_boilerPower_On; 
					else if(it->function2 == BOILER_POWER_OFF)
						boilerEvent->boilerPower = cmxDeviceService::_boilerPower_Off; 
					else
						parsing_success = FALSE;

					break;
				}

				case BOILER_CTEMP_EVENT:
				{
					boilerEvent->func = cmxDeviceService::f_boilerCurrentTemperature;
					boilerEvent->boilerRequestedTemperature = it->function2;					
					boilerEvent->boilerCurrentTemperature = it->function3;					
					break;
				}

				case BOILER_RTEMP_EVENT:
				{
					boilerEvent->func = cmxDeviceService::f_boilerRequestedTemperature;
					boilerEvent->boilerRequestedTemperature = it->function2;					
					boilerEvent->boilerCurrentTemperature = it->function3;					
					break;
				}

				case BOILER_OUT_EVENT:
				{
					boilerEvent->func = cmxDeviceService::f_boilerOutMode;
					boilerEvent->boilerRequestedTemperature = it->function3;					
					boilerEvent->boilerCurrentTemperature = it->function4;					

					if(it->function2 == BOILER_OUT_SET)
						boilerEvent->boilerOutMode = cmxDeviceService::_boilerOutMode_Set; 
					else if(it->function2 == BOILER_OUT_RELEASE)
						boilerEvent->boilerOutMode = cmxDeviceService::_boilerOutMode_Release; 
					else
						parsing_success = FALSE;

					break;
				}

				default:
					parsing_success = FALSE;
					break;

			}
			
			rootDevice = (cmxDeviceService::ns__rootDevice*)boilerEvent;
			
			break;
		}

		case BUNDLELIGHT:
		{
			cmxDeviceService::ns__bundleLight *bundleLightEvent = new cmxDeviceService::ns__bundleLight;

			bundleLightEvent->dev = cmxDeviceService::_bundleLight;

			dProtocol = get_protocol(BUNDLELIGHT);	
			if(dProtocol == COMMAX)
			{
				bundleLightEvent->proto = cmxDeviceService::_protoCommax;		
				bundleLightEvent->model = cmxDeviceService::_model_BundleLight_Clio;
			}
			bundleLightEvent->intf = cmxDeviceService::_intfRS485;			

			bundleLightEvent->order = it->order;

			switch(it->function1)
			{
				case BUNDLELIGHT_POWER_EVENT:
				{
					bundleLightEvent->func = cmxDeviceService::f_bundleLightPower;
					if(it->function2 == BUNDLELIGHT_POWER_ON)
							bundleLightEvent->bundleLightPower = cmxDeviceService::_bundleLightPower_On;
					else if(it->function2 == BUNDLELIGHT_POWER_OFF)
							bundleLightEvent->bundleLightPower = cmxDeviceService::_bundleLightPower_Off;
					else
						parsing_success = FALSE;

					break;
				}

				case BUNDLELIGHT_READYPOWER_EVENT:
				{
					bundleLightEvent->func = cmxDeviceService::f_readyEnergyPower;
					if(it->function2 == BUNDLELIGHT_READYPOWER_ON)
							bundleLightEvent->readyEnergyPower = cmxDeviceService::_readyEnergyPower_On;
					else if(it->function2 == BUNDLELIGHT_READYPOWER_OFF)
							bundleLightEvent->readyEnergyPower = cmxDeviceService::_readyEnergyPower_Off;
					else
						parsing_success = FALSE;

					break;
				}

				case BUNDLELIGHT_ELEVATORCALL_EVENT:
				{
					bundleLightEvent->func = cmxDeviceService::f_bundleLightElevator;
					if(it->function2 == BUNDLELIGHT_ELEVATORCALL_UP_REQUEST)
							bundleLightEvent->bundleLightElevator = cmxDeviceService::_bundleLightElevator_Up;
					else if(it->function2 == BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST)
							bundleLightEvent->bundleLightElevator = cmxDeviceService::_bundleLightElevator_Down;
					else
						parsing_success = FALSE;

					break;
				}

				case BUNDLELIGHT_OUT_EVENT:
				{
					bundleLightEvent->func = cmxDeviceService::f_bundleLightExit;
					if(it->function2 == BUNDLELIGHT_OUT_REQUEST)
						bundleLightEvent->bundleLightExit = cmxDeviceService::_bundleLightExit_Notify;
					else
						parsing_success = FALSE;

					break;
				}

				case BUNDLELIGHT_GASCLOSE_EVENT:
				{
					if(it->function2 == BUNDLELIGHT_GASCLOSE_REQUEST)
					{
						//일괄소등 가스 닫기 명령 처리 (end_proc으로 보내지는 않는다.)
						for(int i = 0; i < Get_Count(GAS); i++)
						{
							get_device_item(GAS, i + 1, &d_item);
							if(d_item.gasItem.action != GAS_CLOSE)
							{
								control.device_name = GAS;
								control.order = i + 1;
								control.function1 = GAS_CLOSE;
								control.function2 = 0;
								control.function3 = 0;
								control.function4 = 0;

								Add_Control(control);
							}
						}
						parsing_success = FALSE;
					}
					break;
				}

				default:
					parsing_success = FALSE;
					break;
			}

			rootDevice = (cmxDeviceService::ns__rootDevice*)bundleLightEvent;
			
			break;	
		}

		case CURTAIN:
		{
			cmxDeviceService::ns__curtain *curtainEvent = new cmxDeviceService::ns__curtain;

			curtainEvent->order = it->order;
			curtainEvent->dev = cmxDeviceService::_curtain;

			dProtocol = get_protocol(CURTAIN);	
			if(dProtocol == COMMAX)
			{
				curtainEvent->proto = cmxDeviceService::_protoCommax;
				curtainEvent->model = cmxDeviceService::_model_curtain_CLIO;
			}
			else if(dProtocol == HAMUN)
			{
				curtainEvent->proto = cmxDeviceService::_protoHaMun;		
				curtainEvent->model = cmxDeviceService::_model_curtain_HaMun;				
			}
			curtainEvent->intf = cmxDeviceService::_intfRS485;			

			switch(it->function1)
			{
				case CURTAIN_ACTION_EVENT:
				{
					curtainEvent->func = cmxDeviceService::f_curtainDo;
					switch(it->function2)
					{
						case CURTAIN_CLOSE:
							curtainEvent->curtainDo = cmxDeviceService::_curtainDo_Close;
							break;

						case CURTAIN_OPEN:
							curtainEvent->curtainDo = cmxDeviceService::_curtainDo_Open;
							break;	

						case CURTAIN_STOP:
							curtainEvent->curtainDo = cmxDeviceService::_curtainDo_Stop;
							break;

						default:
							parsing_success = FALSE;
							break;
					}
				}

				default:
					break;
			}

			rootDevice = (cmxDeviceService::ns__rootDevice*)curtainEvent;

			break;
		}
		
		default:
			break;
	}


	if(parsing_success == TRUE)
	{
		for(member_it = member.begin( ); member_it != member.end( ); member_it++ )
		{
			
			string strd = *member_it;
			char url[256];
			strcpy(url, strd.c_str());
			Log(LOG::HND, "End Point Url : %s \n" , url);
			
			Event_End_Proc(url, rootDevice);
		}
	}
	else
		return -1;
	
	return 1;
}

void SOAP_Handler::Event_End_Proc(char* url, 	cmxDeviceService::ns__rootDevice *rootDevice)
{
	cmxDeviceService::cds ds;
	ds.endpoint = url;

	soapTimeout(ds.soap, TIME_CONNECT, TIME_SEND, TIME_RECV);

	int r=~SOAP_OK;

	int out = 0; 

	switch(rootDevice->dev)
	{
		case cmxDeviceService::_boiler:
		{
			//Log(LOG::HND, "EVENT_END_PROC ->_BOILER[%d]\n", rootDevice->order);

			cmxDeviceService::ns__boiler value;
			memcpy(&value, rootDevice, sizeof(cmxDeviceService::ns__boiler));
			r = ds.ns__boilerEvent(value, &out);
			break;			
		}


		case cmxDeviceService::_light:
		{
			//Log(LOG::HND, "EVENT_END_PROC ->_LIGHT[%d]\n", rootDevice->order);

			cmxDeviceService::ns__light value;
			memcpy(&value, rootDevice, sizeof(cmxDeviceService::ns__light));

			r = ds.ns__lightEvent(value, &out);

			break;			
		}


		case cmxDeviceService::_gasValve:
		{
			//Log(LOG::HND, "EVENT_END_PROC ->_GAS[%d]\n", rootDevice->order);

			cmxDeviceService::ns__gasValve value;
			memcpy(&value, rootDevice, sizeof(cmxDeviceService::ns__gasValve));
			r = ds.ns__gasValveEvent(value, &out);
			break;				
		}


		case cmxDeviceService::_bundleLight:
		{
			//Log(LOG::HND, "EVENT_END_PROC ->_BUNDLE_LIGHT[%d]\n", rootDevice->order);

			cmxDeviceService::ns__bundleLight  value;
			memcpy(&value, rootDevice, sizeof(cmxDeviceService::ns__bundleLight));
			r = ds.ns__BundleLightEvent(value, &out);
			break;				
		}

		case cmxDeviceService::_curtain:
		{
			//Log(LOG::HND, "EVENT_END_PROC ->_CURTAIN[%d]\n", rootDevice->order);

			cmxDeviceService::ns__curtain value;
			memcpy(&value, rootDevice, sizeof(cmxDeviceService::ns__curtain));
			r = ds.ns__curtainEvent(value, &out);
			break;				
		}


		default:
		{
			Log(LOG::ERR, "ERROR: EVENT_END_PROC->DEFAULT[%d]\n", rootDevice->order);
			r = SOAP_OK;

			break;
		}

	}

	if( r == SOAP_OK)  {

		//Log(LOG::HND, " SUCCESSFULLY SOAP EVENT TO CLIENT\n");

	}else {
		Log(LOG::ERR, " SOAP EVENT TO CLIENT EEROR \n");
		soap_print_fault(ds.soap, stderr);
	}
	
	if(r)  // if failed	
	{
		if( strcmp(event_end_point, url) != 0) {			
			Log(LOG::ERR, "EVENT_END_PROC :  FAILED => REMOVE URL : %s\n", url);
			Subscribe_Remove(url);
		}
	}
	else
	{
		//Log(LOG::HND, "EVENT_END_PROC : SUCCESS => URL :  %s\n", url);
	}
}

void * SOAP_Handler::event_run(void * arg)
{
	
	Log(LOG::HND, "### SOAP_HANDLER:: EVENT THREAD STARTING [ INTERFACE THREAD ]\n");

	int ret = 0, result;
	SOAP_Handler * s_handler = ( SOAP_Handler*)arg;
	list<Event>::iterator it;
	
	while( s_handler->soap_handler_runningFlag ) {

		if( s_handler->g_eventList.size() <= 0 ) {

			usleep(EVENT_TIME);		
			
			continue;
		}

		Log(LOG::HND, "### SOAP_HANDLER:: g_eventList.size = %d\n", s_handler->g_eventList.size());
		it = s_handler->g_eventList.begin();
		
		//send soap service
		result = s_handler->Soap_Event_Parser(it);

		switch(it->device_name)
		{
			case LIGHT:
				Log(LOG::HND, "### SOAP_HANDLER:: soap event sent 'LIGHT', order = %d\n", it->order);
				break;

			case GAS:
				Log(LOG::HND, "### SOAP_HANDLER:: soap event sent 'GAS', order = %d\n", it->order);
				break;

			case BOILER:
				Log(LOG::HND, "### SOAP_HANDLER:: soap event sent 'BOILER', order = %d\n", it->order);
				break;

			case BUNDLELIGHT:
				Log(LOG::HND, "### SOAP_HANDLER:: soap event sent 'BUNDLELIGHT', order = %d\n", it->order);
				break;	
				
			case CURTAIN:
				Log(LOG::HND, "### SOAP_HANDLER:: soap event sent 'CURTAIN', order = %d\n", it->order);
				break;				
				
		}

		usleep(EVENT_TIME);

		pthread_mutex_lock(&s_handler->event_mutex);

		s_handler->g_eventList.pop_front();

		pthread_mutex_unlock(&s_handler->event_mutex);
		Log(LOG::HND, "### SOAP_HANDLER:: Delete Event List\n");
		Log(LOG::SEP, "");
	}	

	pthread_exit(NULL);
	Log(LOG::HND, "### SOAP_HANDLER:: EVENT  THREAD ENDING [ INTERFACE THREAD ]\n");
	return NULL;

}

void SOAP_Handler::Event_Start()
 {

	int ret;	

	if( (ret = pthread_attr_init( &event_buf_thread_t) ) != 0 ) {

			Log(LOG::HND, "EVENT CAN'T  CREATE THREAD ATTRIBUTE\n");
	}
	
	if( (ret = pthread_attr_setdetachstate( &event_buf_thread_t, PTHREAD_CREATE_DETACHED) ) != 0 ) {
			
			Log(LOG::HND, "EVENT CAN'T SET THREAD SET DETACH\n");
	}

	if( (ret = pthread_create( &event_buf_thread, &event_buf_thread_t, SOAP_Handler::event_run, (void*) this) ) != 0 )
	{
		Log(LOG::HND,  "EVENT CAN'T FAIL THREAD CREATE\n");
	}

	//Log(LOG::HND,  "EVENT SUCCESSFULLY THREAD CREATE \n");

	(void)pthread_attr_destroy(&event_buf_thread_t);		


 }

/***************************************************************************************/
// Soap Subscriber
/**************************************************************************************/
int SOAP_Handler::Add_Subscriber(char* subscriber)
{
	pthread_mutex_lock(&subscribe_mutex);

	if(g_subscriberList.size() >= MAX_SUBSCRIBER)
	{
		pthread_mutex_unlock(&subscribe_mutex);
		Log(LOG::ERR, "### SOAP_HANDLER::  Subscriber Max OVer\n");	
		return -1;
	}

	vector<string>::iterator it;
	for(it = g_subscriberList.begin( ); it != g_subscriberList.end( ); it++ )
	{
		string strd = *it;
		
		// check duplicate URLs
		if(!strcmp(subscriber, strd.c_str()))
		{
			Log(LOG::HND, "### SOAP_HANDLER::  Duplicate End Point Url : %s\n", strd.c_str() );
			pthread_mutex_unlock(&subscribe_mutex);
			return -1;
		}
	}

	// add URL to array
	g_subscriberList.push_back(subscriber);

	pthread_mutex_unlock(&subscribe_mutex);

	Log(LOG::HND, "### SOAP_HANDLER::  %s Subscriber Add\n", subscriber);	

	return 1;
}


void SOAP_Handler::Subscribe_Copy(vector<string> *copy_subscribe)
{
	pthread_mutex_lock(&subscribe_mutex);
	
	copy_subscribe->clear();

	vector<string>::iterator it;
	for(it = g_subscriberList.begin( ); it != g_subscriberList.end( ); it++ )
		copy_subscribe->push_back(*it);

	pthread_mutex_unlock(&subscribe_mutex);

}

void SOAP_Handler::Subscribe_Remove(char* url)
{
	pthread_mutex_lock(&subscribe_mutex);

	vector<string>::iterator it;

	int i=0;
	int found = 0;
	for(it = g_subscriberList.begin( ); it != g_subscriberList.end( ); it++ )
	{	
		string strd = *it;
		if(!strcmp(url, strd.c_str()))
		{
			found = 1;
			//cout << "remove url " << strd.c_str() << endl;
			Log(LOG::HND, "Remove Registered End Point Url : %s", strd.c_str() ) ;
			break;
		}
		i++;
	}
	
	// remove URL if found matching URL
	if(found)
		g_subscriberList.erase(g_subscriberList.begin()+i,g_subscriberList.begin()+i+1);

	pthread_mutex_unlock(&subscribe_mutex);
}


/***************************************************************************************/
//SOAP Command
/**************************************************************************************/

int SOAP_Handler::Get_Count(enum DEVICE_NAME device_name)
{
	int count;
	count = get_current_supported_cnt(device_name);

	return count;
}


int SOAP_Handler::Get_DeviceCategory(cmxDeviceService::ns__deviceCategory * device)
{
	int categoryIndex, error_check; 
	unsigned int g_device_cnt, result;

	g_device_cnt = get_devicecategory_cnt();
	
	for(categoryIndex = 0; categoryIndex < g_device_cnt; categoryIndex++)
	{
		switch(get_name(categoryIndex))
		{
			case LIGHT:
				result = Get_Count(LIGHT);
				if(result > 0)
				{
					device->enable_device_category._light = cmxDeviceService::_device_support;
					Log(LOG::HND, "LIGHT _device_support\n");
				}
				else
				{
					device->enable_device_category._light = cmxDeviceService::_not_supported_device;
					Log(LOG::HND, "LIGHT _not_supported_device\n");
				}

				error_check = check_device_port_error();
				if(error_check == FALSE)		//port error
				{
					device->lightDeviceError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
					Log(LOG::HND, "check_device_port = FALSE\n");					
				}
				else
				{
					error_check = check_device_disconnection(LIGHT);
					if(error_check == FALSE)	//light disconnection
					{
						device->deviceConnectionCheck = cmxDeviceService::_not_device_disconnect;
						Log(LOG::HND, "check_device_ack = FALSE\n");
					}
				}
				break;

			case GAS:
				result = Get_Count(GAS);
				if(result > 0)
				{
					device->enable_device_category._gasValve= cmxDeviceService::_device_support;
					Log(LOG::HND, "GAS _device_support\n");						
				}
				else
				{
					device->enable_device_category._gasValve = cmxDeviceService::_not_supported_device;
					Log(LOG::HND, "GAS _not_supported_device\n");															
				}

				error_check = check_device_port_error();
				if(error_check == FALSE)	//port error
				{
					device->gasDeviceError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
					Log(LOG::HND, "check_device_port = FALSE\n");					
				}
				else
				{
					error_check = check_device_disconnection(GAS);
					if(error_check == FALSE)	//gas disconnection
					{
						device->deviceConnectionCheck = cmxDeviceService::_not_device_disconnect;
						Log(LOG::HND, "check_device_ack = FALSE\n");					
					}
				}
		
				break;
				
			case BOILER:
				result = Get_Count(BOILER);
				if(result > 0)
				{
					device->enable_device_category._boiler = cmxDeviceService::_device_support;
					Log(LOG::HND, "BOILER _device_support\n");					
				}
				else
				{
					device->enable_device_category._boiler = cmxDeviceService::_not_supported_device;
					Log(LOG::HND, "BOILER _not_supported_device\n");					
				}
				
				result = check_device_port_error();
				if(result == FALSE)
				{
					device->boilerDeviceError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
					Log(LOG::HND, "check_device_port = FALSE\n");					
				}
				else
				{
					result = check_device_disconnection(BOILER);
					if(result == FALSE)
					{
						device->deviceConnectionCheck = cmxDeviceService::_not_device_disconnect;
						Log(LOG::HND, "check_device_ack = FALSE\n");					
					}
				}

				break;
			
			case BUNDLELIGHT:
				result = Get_Count(BUNDLELIGHT);
				if(result > 0)
				{
					device->enable_device_category._bundleLight = cmxDeviceService::_device_support;
					Log(LOG::HND, "BUNDLELIGHT _device_support\n");										
				}
				else
				{
					device->enable_device_category._bundleLight = cmxDeviceService::_not_supported_device;
					Log(LOG::HND, "BUNDLELIGHT _not_supported_device\n");										
				}

				error_check = check_device_port_error();
				if(error_check == FALSE)
				{
					device->bundleLightDeviceError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
					Log(LOG::HND, "check_device_port = FALSE\n");					
				}
				else
				{
					error_check = check_device_disconnection(BUNDLELIGHT);
					if(error_check == FALSE)
					{
						device->deviceConnectionCheck = cmxDeviceService::_not_device_disconnect;
						Log(LOG::HND, "check_device_ack = FALSE\n");					
					}
				}
				
				break;

			case CURTAIN:
				result = Get_Count(CURTAIN);
				if(result > 0)
				{
					device->enable_device_category._curtain = cmxDeviceService::_device_support;
					Log(LOG::HND, "CURTAIN _device_support\n");						
				}
				else
				{
					device->enable_device_category._curtain = cmxDeviceService::_not_supported_device;
					Log(LOG::HND, "CURTAIN _not_supported_device\n");															
				}

				error_check = check_device_port_error();
				if(error_check == FALSE)	//port error
				{
					device->curtainDeviceError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
					Log(LOG::HND, "check_device_port = FALSE\n");					
				}
				else
				{
					error_check = check_device_disconnection(CURTAIN);
					if(error_check == FALSE)	//gas disconnection
					{
						device->deviceConnectionCheck = cmxDeviceService::_not_device_disconnect;
						Log(LOG::HND, "check_device_ack = FALSE\n");					
					}
				}
		
				break;

			default:
				break;
		}
	}


	return SOAP_OK;
}

int SOAP_Handler::Get_Property(cmxDeviceService::ns__rootDevice* device, int* order)
{
	int res = SOAP_OK;
	D_Property d_property;

	switch(device->dev)
	{
		case cmxDeviceService::_boiler:
		{
			if(Get_Count(BOILER) < *order || *order < 1) 
				*order = 1;

			get_device_property(BOILER, *order, &d_property);
			cmxDeviceService::ns__boiler* pObject = (cmxDeviceService::ns__boiler*)device;

			// 개별 보일러
			if( d_property.boilerProperty.boilerMode == INDIVIDUAL_BOILER)
			{
 				pObject->devBoilerProperty._indivisualBoiler = cmxDeviceService::_supported_function;
				pObject->devBoilerProperty._eachBoiler = cmxDeviceService::_not_supported_function;

				pObject->devBoilerProperty._boilerPower_Off	= cmxDeviceService::_supported_function;
				pObject->devBoilerProperty._boilerPower_On  = cmxDeviceService::_supported_function;

				pObject->devBoilerProperty._boilerHeatingPower_Off	= cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerHeatingPower_On = cmxDeviceService::_not_supported_function;

				pObject->devBoilerProperty._boilerOutMode_Set = cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerOutMode_Release = cmxDeviceService::_not_supported_function;
				Log(LOG::HND, "BOILER PROPERTY = INDIVIDUAL_BOILER\n");					
			}
			else if(d_property.boilerProperty.boilerMode == EACH_BOILER)
			{
 				pObject->devBoilerProperty._indivisualBoiler = cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._eachBoiler = cmxDeviceService::_supported_function;

				pObject->devBoilerProperty._boilerPower_Off	= cmxDeviceService::_supported_function;
				pObject->devBoilerProperty._boilerPower_On  = cmxDeviceService::_supported_function;

				pObject->devBoilerProperty._boilerHeatingPower_Off	= cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerHeatingPower_On = cmxDeviceService::_not_supported_function;

				pObject->devBoilerProperty._boilerOutMode_Set = cmxDeviceService::_supported_function;
				pObject->devBoilerProperty._boilerOutMode_Release = cmxDeviceService::_supported_function;
				Log(LOG::HND, "BOILER PROPERTY = EACH_BOILER\n");									
			}
			else
			{
 				pObject->devBoilerProperty._indivisualBoiler = cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._eachBoiler = cmxDeviceService::_not_supported_function;

				pObject->devBoilerProperty._boilerPower_Off	= cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerPower_On  = cmxDeviceService::_not_supported_function;

				pObject->devBoilerProperty._boilerHeatingPower_Off	= cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerHeatingPower_On = cmxDeviceService::_not_supported_function;

				pObject->devBoilerProperty._boilerOutMode_Set = cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerOutMode_Release = cmxDeviceService::_not_supported_function;
				Log(LOG::HND, "BOILER PROPERTY = NOT SUPPORTED\n");									
			}
			
			// 온돌(난방 수)
			if(  d_property.boilerProperty.dipSwitchMode == HYPOCAUST_BOILER)
			{
				pObject->devBoilerProperty._boilerDipSwitchMode_Hypocaust =  cmxDeviceService::_supported_function;
				pObject->devBoilerProperty._boilerDipSwitchMode_InnerTemperature =  cmxDeviceService::_not_supported_function;					
				Log(LOG::HND, "BOILER PROPERTY = HYPOCAUST_BOILER\n");	
			}
			else if(  d_property.boilerProperty.dipSwitchMode == INNERTEMPERATURE_BOILER)
			{
				pObject->devBoilerProperty._boilerDipSwitchMode_Hypocaust =  cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerDipSwitchMode_InnerTemperature =  cmxDeviceService::_supported_function;					
				Log(LOG::HND, "BOILER PROPERTY = INNERTEMPERATURE_BOILER\n");					
			}
			else
			{
				pObject->devBoilerProperty._boilerDipSwitchMode_Hypocaust =  cmxDeviceService::_not_supported_function;
				pObject->devBoilerProperty._boilerDipSwitchMode_InnerTemperature =  cmxDeviceService::_not_supported_function;					
				Log(LOG::HND, "BOILER PROPERTY = NOT SUPPORTED\n");				
			}

			pObject->devBoilerProperty.boilerRequestedTemperature =  cmxDeviceService::_supported_function;
			pObject->devBoilerProperty.boilerRequestedTemperatureFrom = d_property.boilerProperty.requestTemperatureMin;
			pObject->devBoilerProperty.boilerRequestedTemperatureTo = d_property.boilerProperty.requestTemperatureMax;

			pObject->devBoilerProperty.boilerCurrentTemperature =   cmxDeviceService::_supported_function;
			pObject->devBoilerProperty.boilerCurrentTemperatureFrom =  d_property.boilerProperty.currentTemperatureMin;
			pObject->devBoilerProperty.boilerCurrentTemperatureTo =  d_property.boilerProperty.currentTemperatureMax;

			pObject->devBoilerProperty.boilerHeatingWaterTemperature = cmxDeviceService::_not_supported_function;
			pObject->devBoilerProperty.boilerHeatingWaterTemperatureFrom = cmxDeviceService::_not_supported_function;
			pObject->devBoilerProperty.boilerHeatingWaterTemperatureTo = cmxDeviceService::_not_supported_function;
			Log(LOG::HND, "TEMPERATURE From %d, To %d\n", d_property.boilerProperty.requestTemperatureMin, d_property.boilerProperty.requestTemperatureMax);
 		}
		break;
		
		case cmxDeviceService::_bundleLight:
		{
			if(Get_Count(BUNDLELIGHT) < *order || *order < 1) 
				*order = 1;

			get_device_property(BUNDLELIGHT, *order, &d_property);
			cmxDeviceService::ns__bundleLight* pObject = (cmxDeviceService::ns__bundleLight*)device;	

			if( d_property.bundleLightProperty.power == SUPPORTED)
			{			
				pObject->devBundleLightProperty._bundleLightPower_Off = cmxDeviceService::_supported_function;
				pObject->devBundleLightProperty._bundleLightPower_On  = cmxDeviceService::_supported_function;
				pObject->devBundleLightProperty._bundleLightPower_AllOff  = cmxDeviceService::_not_supported_function;
				pObject->devBundleLightProperty._bundleLightPower_AllOn  = cmxDeviceService::_not_supported_function;	
				Log(LOG::HND, "BUNDLELIGHT POWER PROPERTY = SUPPORTED\n");	
			}
			else
			{
				pObject->devBundleLightProperty._bundleLightPower_Off = cmxDeviceService::_not_supported_function;
				pObject->devBundleLightProperty._bundleLightPower_On  = cmxDeviceService::_not_supported_function;
				pObject->devBundleLightProperty._bundleLightPower_AllOff  = cmxDeviceService::_not_supported_function;
				pObject->devBundleLightProperty._bundleLightPower_AllOn  = cmxDeviceService::_not_supported_function;	
				Log(LOG::HND, "BUNDLELIGHT POWER PROPERTY = NOT SUPPORTED\n");				
			}

			if( d_property.bundleLightProperty.readyPower == SUPPORTED)
			{
				pObject->devBundleLightProperty._readyEnergyPower_Off = cmxDeviceService::_supported_function;			 // 대기 전력 스위치_꺼짐
				pObject->devBundleLightProperty._readyEnergyPower_On = cmxDeviceService::_supported_function;			 // 대기 전력 스위치_켜짐
				pObject->devBundleLightProperty._readyEnergyPower_AllOff = cmxDeviceService::_not_supported_function;		 // 대기 전력 스위치_전체꺼짐
				pObject->devBundleLightProperty._readyEnergyPower_AllOn = cmxDeviceService::_not_supported_function;		 // 대기 전력 스위치_전체켜짐
				Log(LOG::HND, "BUNDLELIGHT READYPOWER PROPERTY = SUPPORTED\n");					
			}
			else
			{
				pObject->devBundleLightProperty._readyEnergyPower_Off = cmxDeviceService::_not_supported_function;			 // 대기 전력 스위치_꺼짐
				pObject->devBundleLightProperty._readyEnergyPower_On = cmxDeviceService::_not_supported_function;			 // 대기 전력 스위치_켜짐
				pObject->devBundleLightProperty._readyEnergyPower_AllOff = cmxDeviceService::_not_supported_function;		 // 대기 전력 스위치_전체꺼짐
				pObject->devBundleLightProperty._readyEnergyPower_AllOn = cmxDeviceService::_not_supported_function;		 // 대기 전력 스위치_전체켜짐
				Log(LOG::HND, "BUNDLELIGHT READYPOWER PROPERTY = NOT SUPPORTED\n");									
			}

			if( d_property.bundleLightProperty.elevatorCall == SUPPORTED)
			{
				pObject->devBundleLightProperty._bundleLightElevator_Up  = cmxDeviceService::_supported_function;
				pObject->devBundleLightProperty._bundleLightElevator_Down  = cmxDeviceService::_supported_function;				
				Log(LOG::HND, "BUNDLELIGHT ELEVATORCALL PROPERTY = SUPPORTED\n");									
			}
			else
			{
				pObject->devBundleLightProperty._bundleLightElevator_Up  = cmxDeviceService::_not_supported_function;
				pObject->devBundleLightProperty._bundleLightElevator_Down  = cmxDeviceService::_not_supported_function;				
				Log(LOG::HND, "BUNDLELIGHT ELEVATORCALL PROPERTY = NOT SUPPORTED\n");													
			}

			pObject->devBundleLightProperty._bundleLightExit_Notify  = cmxDeviceService::_not_supported_function;
			pObject->devBundleLightProperty._bundleLightExit_Entry_Accept = cmxDeviceService::_not_supported_function;	 // 외출 진입 허가 [ DS로 부터 외출 설정 통보시 월패드에서 외출 허락/거부를 DS로 알려 줘야함
			pObject->devBundleLightProperty._bundleLightExit_Entry_Deny = cmxDeviceService::_not_supported_function;     	// 외출 진입 불가 
			pObject->devBundleLightProperty._bundleLightExit_Setting = cmxDeviceService::_not_supported_function;     		// 외출 설정	
			pObject->devBundleLightProperty._bundleLightExit_Release = cmxDeviceService::_not_supported_function;    		// 외출 해제
			pObject->devBundleLightProperty._gasValve_Entry_Accept = cmxDeviceService::_not_supported_function;
			pObject->devBundleLightProperty._gasValve_Entry_Deny = cmxDeviceService::_not_supported_function;
		}
		break;

		case cmxDeviceService::_gasValve:
		{
			if(Get_Count(GAS) < *order || *order < 1) 
				*order = 1;

			get_device_property(GAS, *order, &d_property);
			cmxDeviceService::ns__gasValve* pObject = (cmxDeviceService::ns__gasValve*)device;		

			if( d_property.gasProperty.action ==SUPPORTED) 
			{

				pObject->devGasValveProperty._gvDo_Close  = cmxDeviceService::_supported_function;
				pObject->devGasValveProperty._gvDo_Open   = cmxDeviceService::_supported_function;
				pObject->devGasValveProperty._gvAlarm_On  = cmxDeviceService::_supported_function;
				pObject->devGasValveProperty._gvAlarm_Off = cmxDeviceService::_not_supported_function;

				Log(LOG::HND, "GAS [%d] PROPERTY = SUPPORTED\n", *order);					

			}
			else 
			{
			
				pObject->devGasValveProperty._gvDo_Close  = cmxDeviceService::_not_supported_function;
				pObject->devGasValveProperty._gvDo_Open   = cmxDeviceService::_not_supported_function;
				pObject->devGasValveProperty._gvAlarm_On  = cmxDeviceService::_not_supported_function;
				pObject->devGasValveProperty._gvAlarm_Off = cmxDeviceService::_not_supported_function;

				Log(LOG::HND, "GAS [%d] PROPERTY = NOT SUPPORTED\n", *order);					
			}
		}	
		break;

		case cmxDeviceService::_light:
		{
			if(Get_Count(LIGHT) < *order || *order < 1) 
				*order = 1;

			get_device_property(LIGHT, *order, &d_property);
			cmxDeviceService::ns__light* pObject = (cmxDeviceService::ns__light*)device;				

			if(d_property.lightProperty.mode == LIGHT_MODE_BINARY)
			{
				pObject->devLightProperty._lightBinary = cmxDeviceService::_supported_function;
				pObject->devLightProperty._lightDimmable = cmxDeviceService::_not_supported_function;

				pObject->devLightProperty._lightPower_Off = cmxDeviceService::_supported_function;
				pObject->devLightProperty._lightPower_On  = cmxDeviceService::_supported_function;	

				pObject->devLightProperty._lightPower_AllOff = cmxDeviceService::_supported_function;		
				pObject->devLightProperty._lightPower_AllOn = cmxDeviceService::_supported_function;	

				pObject->devLightProperty.dimmerLevelFrom	= 0;
				pObject->devLightProperty.dimmerLevelTo	= 0;	
				Log(LOG::HND, "LIGHT [%d] PROPERTY = LIGHT_MODE_BINARY\n", *order);					
			}
			else if(d_property.lightProperty.mode == LIGHT_MODE_DIMMABLE)
			{
				pObject->devLightProperty._lightBinary = cmxDeviceService::_not_supported_function;
				pObject->devLightProperty._lightDimmable = cmxDeviceService::_supported_function;

				pObject->devLightProperty._lightPower_Off = cmxDeviceService::_supported_function;
				pObject->devLightProperty._lightPower_On  = cmxDeviceService::_supported_function;	

				pObject->devLightProperty._lightPower_AllOff = cmxDeviceService::_supported_function;		
				pObject->devLightProperty._lightPower_AllOn = cmxDeviceService::_supported_function;	

				pObject->devLightProperty.dimmerLevelFrom	= 1;
				pObject->devLightProperty.dimmerLevelTo	= 8;	
				Log(LOG::HND, "LIGHT [%d] PROPERTY = LIGHT_MODE_DIMMABLE\n", *order);					
			}
			else
			{
				pObject->devLightProperty._lightBinary = cmxDeviceService::_not_supported_function;
				pObject->devLightProperty._lightDimmable = cmxDeviceService::_not_supported_function;

				pObject->devLightProperty._lightPower_Off = cmxDeviceService::_not_supported_function;
				pObject->devLightProperty._lightPower_On  = cmxDeviceService::_not_supported_function;	

				pObject->devLightProperty._lightPower_AllOff = cmxDeviceService::_not_supported_function;		
				pObject->devLightProperty._lightPower_AllOn = cmxDeviceService::_not_supported_function;	

				pObject->devLightProperty.dimmerLevelFrom	= 0;
				pObject->devLightProperty.dimmerLevelTo	= 0;	
				Log(LOG::HND, "LIGHT [%d] PROPERTY = NOT SUPPORTED\n", *order);	
			}
		}
		break;

		case cmxDeviceService::_curtain:
		{
			if(Get_Count(CURTAIN) < *order || *order < 1) 
				*order = 1;

			get_device_property(CURTAIN, *order, &d_property);
			cmxDeviceService::ns__curtain* pObject = (cmxDeviceService::ns__curtain*)device;		

			if( d_property.curtainProperty.action ==SUPPORTED) 
			{

				pObject->devCurtainProperty._curtainDo_Close = cmxDeviceService::_supported_function;
				pObject->devCurtainProperty._curtainDo_Open  = cmxDeviceService::_supported_function;
				pObject->devCurtainProperty._curtainDo_Stop  = cmxDeviceService::_supported_function;
				pObject->devCurtainProperty._curtainAngular  = cmxDeviceService::_not_supported_function;

				Log(LOG::HND, "CURTAIN [%d] PROPERTY = SUPPORTED\n", *order);					

			}
			else 
			{
			
				pObject->devCurtainProperty._curtainDo_Close = cmxDeviceService::_not_supported_function;
				pObject->devCurtainProperty._curtainDo_Open  = cmxDeviceService::_not_supported_function;
				pObject->devCurtainProperty._curtainDo_Stop  = cmxDeviceService::_not_supported_function;
				pObject->devCurtainProperty._curtainAngular  = cmxDeviceService::_not_supported_function;

				Log(LOG::HND, "CURTAIN [%d] PROPERTY = SUPPORTED\n", *order);					
			}
		}	
		break;

	}

	return res;
}

int SOAP_Handler::Get_Item(struct soap *pSoap, cmxDeviceService::ns__rootDevice* device)
{
	int res = SOAP_OK;
	D_Item d_item;
	D_Property d_property;
	enum DEVICE_PROTOCOL dProtocol;

	switch(device->dev)
	{
		case cmxDeviceService::_light:
		{
			cmxDeviceService::ns__light* pObject = (cmxDeviceService::ns__light*)device;					
			get_device_property(LIGHT, device->order, &d_property);
			get_device_item(LIGHT, device->order, &d_item);						

			// 디바이스 아이디 범위 초과 에러	
			if(Get_Count(LIGHT) < device->order || device->order < 1) 
				return SOAP_IOB;

			dProtocol = get_protocol(LIGHT);
			if(dProtocol == COMMAX)
			{
				pObject->proto = cmxDeviceService::_protoCommax;				
				pObject->intf =  cmxDeviceService::_intfRS485;					
				pObject->model = cmxDeviceService::_model_Light_JoongAngControl;	

				if(d_property.lightProperty.mode == LIGHT_MODE_DIMMABLE)
					pObject->lightSwitchMode = cmxDeviceService::_lightSwitch_Dimmable;
				else
					pObject->lightSwitchMode = cmxDeviceService::_lightSwitch_Binary;
			}

			if(d_item.lightItem.error == PORT_ERROR)
			{
				pObject->func = cmxDeviceService::f_lightDevError;
				pObject->lightDevError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
			
				Log(LOG::ERR, "LIGHT [%d] PORT_ERROR\n", device->order);
				return SOAP_OK;	
			}
			else if(d_item.lightItem.error == LIGHT_DISCONNECTION)
			{
				pObject->func = cmxDeviceService::f_lightDevError;
				pObject->lightDevError = cmxDeviceService::devError_light_DisConnect;
			
				Log(LOG::ERR, "LIGHT [%d] LIGHT_DISCONNECTION\n", device->order);
				return SOAP_OK;	
			}

			int ret = 0;

			cmxDeviceService::cds service;
			service.endpoint = event_end_point;
			
			if(d_item.lightItem.power == LIGHT_POWER_OFF)
			{
				pObject->lightPower = cmxDeviceService::_lightPower_Off;		
				Log(LOG::HND,"LIGHT [%d] LIGHT_POWER_OFF\n",device->order);					
			}
			else if(d_item.lightItem.power == LIGHT_POWER_ON)
			{
				pObject->lightPower = cmxDeviceService::_lightPower_On;		
				Log(LOG::HND,"LIGHT [%d] LIGHT_POWER_OFF\n",device->order);					
			}

			if(d_property.lightProperty.mode == LIGHT_MODE_BINARY)
			{
				pObject->dimmableLevel = 0;
				Log(LOG::HND,"LIGHT [%d] dimmableLevel = %d\n", device->order, pObject->dimmableLevel);				
			}
			else if(d_property.lightProperty.mode == LIGHT_MODE_DIMMABLE)
			{
				pObject->dimmableLevel = d_item.lightItem.dimmingLevel;
				Log(LOG::HND,"LIGHT [%d] dimmableLevel = %d\n", device->order, pObject->dimmableLevel);								
			}

			break;
		}
		
		case cmxDeviceService::_gasValve:
		{
			cmxDeviceService::ns__gasValve* pObject = (cmxDeviceService::ns__gasValve*)device;					
			get_device_property(GAS, device->order, &d_property);
			get_device_item(GAS, device->order, &d_item);	
						
			// 디바이스 아이디 범위 초과 에러	
			if(Get_Count(GAS) < device->order  || device->order < 1) 
				return SOAP_IOB;

			dProtocol = get_protocol(GAS);
			if(dProtocol == COMMAX)
			{
				pObject->proto = cmxDeviceService::_protoCommax;				
				pObject->intf =  cmxDeviceService::_intfRS485;					
				pObject->model = cmxDeviceService::_model_GasValve_Shinwoo;	
			}

			if(d_item.gasItem.error == PORT_ERROR)
			{
				pObject->func = cmxDeviceService::f_gvDevError;
				pObject->gvDevError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
			
				Log(LOG::ERR, "GAS [%] PORT_ERROR\n", device->order);
				return SOAP_OK;	
			}
			else if(d_item.gasItem.error == GAS_DISCONNECTION)
			{
				pObject->func = cmxDeviceService::f_gvDevError;
				pObject->gvDevError = cmxDeviceService::devError_gas_DisConnect;
			
				Log(LOG::ERR, "GAS [%] GAS_DISCONNECTION\n", device->order);
				return SOAP_OK;	
			}

			int ret = 0;

			cmxDeviceService::cds service;
			service.endpoint = event_end_point;

			if(d_item.gasItem.action == GAS_CLOSE)
			{
				pObject->gvDo = cmxDeviceService::_gvDo_Close;
				Log(LOG::HND,"GAS [%d] GAS_CLOSE\n", device->order);
			}
			else if(d_item.gasItem.action == GAS_OPEN)
			{
				pObject->gvDo = cmxDeviceService::_gvDo_Open;
				Log(LOG::HND,"GAS [%d] GAS_OPEN\n", device->order);
			}

			break;
		}			


		case cmxDeviceService::_bundleLight:
		{
			cmxDeviceService::ns__bundleLight* pObject = (cmxDeviceService::ns__bundleLight*)device;					
			get_device_property(BUNDLELIGHT, device->order, &d_property);
			get_device_item(BUNDLELIGHT, device->order, &d_item);	

			// 디바이스 아이디 범위 초과 에러	
			if(Get_Count(BUNDLELIGHT) < device->order || device->order < 1) 
				return SOAP_IOB;

			dProtocol = get_protocol(BUNDLELIGHT);
			if(dProtocol == COMMAX)
			{
				pObject->proto = cmxDeviceService::_protoCommax;				
				pObject->intf =  cmxDeviceService::_intfRS485;					
				pObject->model = cmxDeviceService::_model_BundleLight_Speel;	
			}

			if(d_item.bundleLightItem.error == PORT_ERROR)
			{
				pObject->func = cmxDeviceService::f_bundleLightDevError;
				pObject->bundleLightDevError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
			
				Log(LOG::ERR, "BUNDLELIGHT [%d] PORT_ERROR\n", device->order);
				return SOAP_OK;	
			}
			else if(d_item.bundleLightItem.error == BUNDLELIGHT_DISCONNECTION)
			{
				pObject->func = cmxDeviceService::f_bundleLightDevError;
				pObject->bundleLightDevError = cmxDeviceService::devError_bundleLight_DisConnect;
			
				Log(LOG::ERR, "BUNDLELIGHT [%d] BUNDLELIGHT_DISCONNECTION\n", device->order);
				return SOAP_OK;	
			}

			int ret = 0;

			cmxDeviceService::cds service;
			service.endpoint = event_end_point;

			if(d_item.bundleLightItem.power == BUNDLELIGHT_POWER_OFF)
			{
				pObject->bundleLightPower = cmxDeviceService::_bundleLightPower_Off;

				Log(LOG::HND,"BUNDLELIGHT [%d] BUNDLELIGHT_POWER_OFF\n", device->order);
			}
			else if(d_item.bundleLightItem.power == BUNDLELIGHT_POWER_ON)
			{
				pObject->bundleLightPower = cmxDeviceService::_bundleLightPower_On;

				Log(LOG::HND,"BUNDLELIGHT [%d]  BUNDLELIGHT_POWER_ON\n", device->order);
			}

			if(d_item.bundleLightItem.readyPower == BUNDLELIGHT_READYPOWER_OFF)
			{
				pObject->readyEnergyPower = cmxDeviceService::_readyEnergyPower_Off;

				Log(LOG::HND,"BUNDLELIGHT [%d]  BUNDLELIGHT_READYPOWER_OFF\n", device->order);
			}
			else if(d_item.bundleLightItem.readyPower == BUNDLELIGHT_READYPOWER_ON)
			{
				pObject->readyEnergyPower = cmxDeviceService::_readyEnergyPower_On;

				Log(LOG::HND,"BUNDLELIGHT [%d]  BUNDLELIGHT_READYPOWER_ON\n", device->order);
			}

			if(d_item.bundleLightItem.elevatorCall== BUNDLELIGHT_ELEVATORCALL_UP_REQUEST)
			{
				pObject->bundleLightElevator = cmxDeviceService::_bundleLightElevator_Up;

				Log(LOG::HND,"BUNDLELIGHT [%d] BUNDLELIGHT_ELEVATORCALL_UP_REQUEST\n", device->order);
			}
			else if(d_item.bundleLightItem.elevatorCall == BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST)
			{
				pObject->bundleLightElevator = cmxDeviceService::_bundleLightElevator_Down;

				Log(LOG::HND,"BUNDLELIGHT [%d] BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST\n", device->order);
			}

			break;
		}			

		case cmxDeviceService::_boiler:
		{
			cmxDeviceService::ns__boiler* pObject = (cmxDeviceService::ns__boiler*)device;					
			get_device_property(BOILER, device->order, &d_property);
			get_device_item(BOILER, device->order, &d_item);	
		
			// 디바이스 아이디 범위 초과 에러	
			if(Get_Count(BOILER) < device->order || device->order < 1) 
				return SOAP_IOB;

			dProtocol = get_protocol(BOILER);
			if(dProtocol == COMMAX)
			{
				pObject->proto = cmxDeviceService::_protoCommax;				
				pObject->intf =  cmxDeviceService::_intfRS485;					
				pObject->model = cmxDeviceService::_model_boiler_Rinnai;	
			}

			if(d_item.boilerItem.error == PORT_ERROR)
			{
				pObject->func = cmxDeviceService::f_boilerDevError;
				pObject->boilerDevError = cmxDeviceService::devError_485_Serial_Port_Open_Error;

				Log(LOG::ERR, "BOILER [%d] PORT_ERROR\n", device->order);
				return SOAP_OK;	
			}
			else if(d_item.boilerItem.error == BOILER_DISCONNECTION)
			{
				pObject->func = cmxDeviceService::f_boilerDevError;
				pObject->boilerDevError = cmxDeviceService::devError_boiler_DisConnect;
			
				Log(LOG::ERR, "BOILER [%d] BOILER_DISCONNECTION\n", device->order);
				return SOAP_OK;	

			}

			int ret = 0;

			cmxDeviceService::cds service;
			service.endpoint = event_end_point;
						
			pObject->boilerCurrentTemperature = d_item.boilerItem.currentTemperature;
			pObject->boilerRequestedTemperature = d_item.boilerItem.requestTemperature;

			if(d_item.boilerItem.power == BOILER_POWER_OFF)
			{
				pObject->boilerPower = cmxDeviceService::_boilerPower_Off;
				Log(LOG::HND,"BOILER [%d] BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", device->order, d_item.boilerItem.requestTemperature, d_item.boilerItem.currentTemperature);
			}
			else if(d_item.boilerItem.power == BOILER_POWER_ON)
			{
				pObject->boilerPower = cmxDeviceService::_boilerPower_On;
				Log(LOG::HND,"BOILER [%d] BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", device->order, d_item.boilerItem.requestTemperature, d_item.boilerItem.currentTemperature);
			}

			if(d_property.boilerProperty.dipSwitchMode == HYPOCAUST_BOILER)
				pObject->boilerDipSwitchMode = cmxDeviceService::_boilerDipSwitchMode_Hypocaust;
			else
				pObject->boilerDipSwitchMode = cmxDeviceService::_boilerDipSwitchMode_InnerTemperature;
			
			if(d_property.boilerProperty.boilerMode == EACH_BOILER)
			{
				pObject->boilerDipSwitchMode = cmxDeviceService::_boilerDipSwitchMode_InnerTemperature;	

				if(d_item.boilerItem.outMode == BOILER_OUT_RELEASE)
				{
					pObject->boilerOutMode = cmxDeviceService::_boilerOutMode_Release;
					Log(LOG::HND,"BOILER [%] BOILER_OUT_RELEASE\n", device->order);				
				}
				else if(d_item.boilerItem.outMode == BOILER_OUT_SET)
				{
					pObject->boilerOutMode = cmxDeviceService::_boilerOutMode_Set;
					Log(LOG::HND,"BOILER [%] BOILER_OUT_SET\n", device->order);				
				}
			}
		}
		break;

		case cmxDeviceService::_curtain:
		{
			cmxDeviceService::ns__curtain * pObject = (cmxDeviceService::ns__curtain*)device;					
			get_device_property(CURTAIN, device->order, &d_property);
			get_device_item(CURTAIN, device->order, &d_item);	
						
			// 디바이스 아이디 범위 초과 에러	
			if(Get_Count(CURTAIN) < device->order  || device->order < 1) 
				return SOAP_IOB;

			dProtocol = get_protocol(CURTAIN);
			if(dProtocol == COMMAX)
			{
				pObject->proto = cmxDeviceService::_protoCommax;		
				pObject->intf =  cmxDeviceService::_intfRS485;			
				pObject->model = cmxDeviceService::_model_curtain_CLIO;
			}
			else if(dProtocol == HAMUN)
			{
				pObject->proto = cmxDeviceService::_protoCommax;		
				pObject->intf =  cmxDeviceService::_intfRS485;			
				pObject->model = cmxDeviceService::_model_curtain_HaMun;
			}

			if(d_item.curtainItem.error == PORT_ERROR)
			{
				pObject->func = cmxDeviceService::f_curtainDevError;
				pObject->curtainDevError = cmxDeviceService::devError_485_Serial_Port_Open_Error;
			
				Log(LOG::ERR, "CURTAIN [%] PORT_ERROR\n", device->order);
				return SOAP_OK;	
			}
			else if(d_item.curtainItem.error == CURTAIN_DISCONNECTION)
			{
				pObject->func = cmxDeviceService::f_curtainDevError;
				pObject->curtainDevError = cmxDeviceService::devError_curtain_DisConnect;
			
				Log(LOG::ERR, "CURTAIN [%] CURTAIN_DISCONNECTION\n", device->order);
				return SOAP_OK;	
			}

			int ret = 0;

			cmxDeviceService::cds service;
			service.endpoint = event_end_point;

			if(d_item.curtainItem.action == CURTAIN_OPEN || d_item.curtainItem.action == CURTAIN_STOP)
			{
				pObject->curtainDo = cmxDeviceService::_curtainDo_Open;
				Log(LOG::HND,"CURTAIN [%d] CURTAIN_OPEN\n", device->order);
			}
			else if(d_item.curtainItem.action == CURTAIN_CLOSE)
			{
				pObject->curtainDo = cmxDeviceService::_curtainDo_Close;
				Log(LOG::HND,"GAS [%d] CURTAIN_CLOSE\n", device->order);
			}

			break;
		}	
	}

	return res;

}

int SOAP_Handler::Set_Dev(cmxDeviceService::ns__rootDevice* device)
{
	int ret = SOAP_OK, i;
	unsigned int count;
	D_Property d_property;

	if(g_controlList.size() > MAX_CONTROL_LIST)
	{
		Log(LOG::HND, "CONTROL buffer overflow, current size = %d\n", g_controlList.size());					
		ret = SOAP_E_TEMP_UNAVAIL;
	}

	switch(device->dev)
	{
		case cmxDeviceService::_boiler:
		{
			cmxDeviceService::ns__boiler* pObject = new cmxDeviceService::ns__boiler;
			pObject->func =  ((cmxDeviceService::ns__boiler*)device)->func;

			control.device_name = BOILER;
			control.order = device->order;
			
			switch( pObject->func ) 
			{
				case cmxDeviceService::f_boilerPower :
					control.function1 = BOILER_POWER_EVENT;
					if(((cmxDeviceService::ns__boiler*)device)->boilerPower == cmxDeviceService::_boilerPower_On)
						control.function2 = BOILER_POWER_ON;
					else if(((cmxDeviceService::ns__boiler*)device)->boilerPower == cmxDeviceService::_boilerPower_Off)
						control.function2 = BOILER_POWER_OFF;

					break;								

				case cmxDeviceService::f_boilerRequestedTemperature :
					control.function1 = BOILER_RTEMP_EVENT;
					control.function2 = ((cmxDeviceService::ns__boiler*)device)->boilerRequestedTemperature;
					break;					

				case cmxDeviceService::f_boilerOutMode :
					control.function1 = BOILER_OUT_EVENT;
					if(((cmxDeviceService::ns__boiler*)device)->boilerOutMode == cmxDeviceService::_boilerOutMode_Set)
						control.function2 = BOILER_OUT_SET;
					else if(((cmxDeviceService::ns__boiler*)device)->boilerOutMode == cmxDeviceService::_boilerOutMode_Release)
						control.function2 = BOILER_OUT_RELEASE;
					break;

				default:
					Log(LOG::ERR, "BOILER NOT SUPPORTED FUNC\n");					
					break;
			}
		}
		break;

		case cmxDeviceService::_light:
		{
			cmxDeviceService::ns__light* pObject = new cmxDeviceService::ns__light;
			get_device_property(LIGHT, device->order, &d_property);
			//get_device_item(LIGHT, device->order, &d_item);						

			pObject->func =  ((cmxDeviceService::ns__light*)device)->func;

			control.device_name = LIGHT;
			control.order = device->order;

			if(d_property.lightProperty.mode == LIGHT_MODE_BINARY)
				control.function1 = LIGHT_MODE_BINARY;
			else
				control.function1 = LIGHT_MODE_DIMMABLE;
			
			switch( pObject->func ) 
			{
				case cmxDeviceService::f_lightPower :
					control.function2 = LIGHT_POWER_EVENT;
					if(((cmxDeviceService::ns__light*)device)->lightPower == cmxDeviceService::_lightPower_On)
						control.function3 = LIGHT_POWER_ON;
					else if(((cmxDeviceService::ns__light*)device)->lightPower == cmxDeviceService::_lightPower_Off)
						control.function3 = LIGHT_POWER_OFF;
					else if(((cmxDeviceService::ns__light*)device)->lightPower == cmxDeviceService::_lightPower_AllOff)
						control.function3 = LIGHT_POWER_ALLOFF;
					else if(((cmxDeviceService::ns__light*)device)->lightPower == cmxDeviceService::_lightPower_AllOn)
						control.function3 = LIGHT_POWER_ALLON;
					break;			

				case cmxDeviceService::f_dimmableLevel :
					control.function2 = LIGHT_DIMMING_EVENT;
					control.function3 = ((cmxDeviceService::ns__light*)device)->dimmableLevel;

					break;

				default:
					Log(LOG::ERR, "LIGHT NOT SUPPORTED FUNC\n");					
					break;
			}
		}
		break;

		case cmxDeviceService::_gasValve:
		{
			cmxDeviceService::ns__gasValve* pObject = new cmxDeviceService::ns__gasValve;
			pObject->func =  ((cmxDeviceService::ns__gasValve*)device)->func;
			pObject->gvDo =  ((cmxDeviceService::ns__gasValve*)device)->gvDo;

			control.device_name = GAS;
			control.order = device->order;

			if(pObject->func == cmxDeviceService::f_gvDo)
			{
				if(pObject->gvDo == cmxDeviceService::_gvDo_Close)
					control.function1 = GAS_CLOSE;
				else
					Log(LOG::ERR, "GAS NOT SUPPORTED FUNC\n");					
			}
		}
		break;

		case cmxDeviceService::_bundleLight:
		{

			cmxDeviceService::ns__bundleLight* pObject= new cmxDeviceService::ns__bundleLight;
			pObject->dev = device->dev;
			pObject->order = device->order;	

			pObject->func =  ((cmxDeviceService::ns__bundleLight*)device)->func;

			control.device_name = BUNDLELIGHT;
			control.order = device->order;
	
			switch( pObject->func ) {

				case cmxDeviceService::f_bundleLightPower :
					control.function1 = BUNDLELIGHT_POWER_EVENT;
					if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightPower == cmxDeviceService::_bundleLightPower_Off)
						control.function2 = BUNDLELIGHT_POWER_OFF;
					else if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightPower == cmxDeviceService::_bundleLightPower_On)
						control.function2 = BUNDLELIGHT_POWER_ON;
					else if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightPower == cmxDeviceService::_bundleLightPower_AllOff)
						control.function2 = BUNDLELIGHT_POWER_ALLOFF;
					else if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightPower == cmxDeviceService::_bundleLightPower_AllOn)
						control.function2 = BUNDLELIGHT_POWER_ALLON;
					
				break;

				case cmxDeviceService::f_bundleLightElevator :
					control.function1 = BUNDLELIGHT_ELEVATORCALL_EVENT;
					if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightElevator == cmxDeviceService::_bundleLightElevator_Success)
						control.function2 = BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS;
					else if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightElevator == cmxDeviceService::_bundleLightElevator_Fail)
						control.function2 = BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL;

				break;

				case cmxDeviceService::f_readyEnergyPower :
					control.function1 = BUNDLELIGHT_READYPOWER_EVENT;
					if(((cmxDeviceService::ns__bundleLight*)device)->readyEnergyPower == cmxDeviceService::_readyEnergyPower_Off)
						control.function2 = BUNDLELIGHT_READYPOWER_OFF;
					else if(((cmxDeviceService::ns__bundleLight*)device)->readyEnergyPower == cmxDeviceService::_readyEnergyPower_On)
						control.function2 = BUNDLELIGHT_READYPOWER_ON;
					else if(((cmxDeviceService::ns__bundleLight*)device)->readyEnergyPower == cmxDeviceService::_readyEnergyPower_AllOff)
						control.function2 = BUNDLELIGHT_READYPOWER_ALLOFF;
					else if(((cmxDeviceService::ns__bundleLight*)device)->readyEnergyPower == cmxDeviceService::_readyEnergyPower_AllOn)
						control.function2 = BUNDLELIGHT_READYPOWER_ALLON;
					
				break;

				case cmxDeviceService::f_bundleLightExit :
					control.function1 = BUNDLELIGHT_OUT_EVENT;
					if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightExit == cmxDeviceService::_bundleLightExit_Entry_Accept)
						control.function2 = BUNDLELIGHT_OUT_REQUEST_SUCCESS;
					else if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightExit == cmxDeviceService::_bundleLightExit_Entry_Deny)
						control.function2 = BUNDLELIGHT_OUT_REQUEST_FAIL;
					else if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightExit == cmxDeviceService::_bundleLightExit_Setting)
						control.function2 = BUNDLELIGHT_OUT_SET;
					else if(((cmxDeviceService::ns__bundleLight*)device)->bundleLightExit == cmxDeviceService::_bundleLightExit_Release)
						control.function2 = BUNDLELIGHT_OUT_CANCEL;

				break;

				case cmxDeviceService::f_gasValve :
					control.function1 = BUNDLELIGHT_GASCLOSE_EVENT;
					if(((cmxDeviceService::ns__bundleLight*)device)->gasValve == cmxDeviceService::_gasValve_Entry_Accept)
						control.function2 = BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS;
					else if(((cmxDeviceService::ns__bundleLight*)device)->gasValve == cmxDeviceService::_gasValve_Entry_Deny)
						control.function2 = BUNDLELIGHT_GASCLOSE_REQUEST_FAIL;

				break;

				default:
					Log(LOG::ERR, "BUNDLELIGHT NOT SUPPORTED FUNC\n");
				break;
			}
		}
		break;

		case cmxDeviceService::_curtain:
		{
			cmxDeviceService::ns__curtain* pObject = new cmxDeviceService::ns__curtain;
			pObject->func =  ((cmxDeviceService::ns__curtain*)device)->func;
			pObject->curtainDo =  ((cmxDeviceService::ns__curtain*)device)->curtainDo;

			control.device_name = CURTAIN;
			control.order = device->order;

			if(pObject->func == cmxDeviceService::f_curtainDo)
			{
				control.function1 = CURTAIN_ACTION_EVENT;
				if(pObject->curtainDo == cmxDeviceService::_curtainDo_Close)
					control.function2 = CURTAIN_CLOSE;
				else if(pObject->curtainDo == cmxDeviceService::_curtainDo_Open)
					control.function2 = CURTAIN_OPEN;
				else if(pObject->curtainDo == cmxDeviceService::_curtainDo_Stop)
					control.function2 = CURTAIN_STOP;
				else
					Log(LOG::ERR, "CURTAIN NOT SUPPORTED FUNC\n");	
			}
		}
		break;

		default:
			Log(LOG::HND, "DEVICE NOT SUPPORTED\n");					
			break;
	}

	ret = Add_Control(control);

	return ret;
}

