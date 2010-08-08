#ifndef SOAP_HANDLER_H_
#define SOAP_HANDLER_H_

#include "../app.h"

#include <list>
#include <string>
#include <vector>

#include "soap/cmxDeviceServiceH.h"
#include "soap/cmxDeviceServicecdsProxy.h"

#define EVENT_TIME			100000
#define CONTROL_TIME 		100000
#define MAX_EVENT_LIST		10
#define MAX_CONTROL_LIST	10
#define MAX_SUBSCRIBER		10

using namespace std;

class SOAP_Handler
{
	private:
		static SOAP_Handler* _instance;

	public:
		SOAP_Handler();
		~SOAP_Handler();

		typedef struct{
			enum DEVICE_NAME device_name;	
			unsigned char order;
			unsigned char function1;
			unsigned char function2;
			unsigned char function3;
			unsigned char function4;
		}Event;
		
		typedef struct{
			enum DEVICE_NAME device_name;	
			unsigned char order;
			unsigned char function1;
			unsigned char function2;
			unsigned char function3;
			unsigned char function4;
		}Control;

		int soap_handler_runningFlag;
		
		Event event;
		Control	control;
		list<Event> g_eventList;
		list<Control> g_controlList;
		vector<string> g_subscriberList;

		void soap_print(unsigned char cmd_mode, enum DEVICE_NAME device_name, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);

		//Control
		int Add_Control(Control control);
		void Control_End_Proc(list<Control>::iterator it);
		static void * control_run(void *);
		void Control_Start();

		//Event
	  	int Add_Event(Event event);
		int Soap_Event_Parser(list<Event>::iterator it);
		void Event_End_Proc(char* url, cmxDeviceService::ns__rootDevice *rootDevice);
		static void * event_run(void *);
		void Event_Start();

		//Subscriber
		int Add_Subscriber(char* subscriber);
		void Subscribe_Copy(vector<string> *copy_subscribe);
		void Subscribe_Remove(char* url);

		void	Close();
		
		//soap service command
		int Get_Count(enum DEVICE_NAME device_name);
		int Get_DeviceCategory(cmxDeviceService::ns__deviceCategory * device);
		int Get_Property(cmxDeviceService::ns__rootDevice* device, int* order);
		int Get_Item(struct soap *pSoap, cmxDeviceService::ns__rootDevice* device);
		int Set_Dev(cmxDeviceService::ns__rootDevice* device);
		
		void Instance_Close();
		static SOAP_Handler* Instance();

		 // Thread Atrribute
		 pthread_t event_buf_thread;
		 pthread_attr_t event_buf_thread_t;

		 pthread_t control_buf_thread;
		 pthread_attr_t control_buf_thread_t;

		 pthread_mutex_t	event_mutex;
		 pthread_mutex_t	control_mutex;	 
		 pthread_mutex_t	subscribe_mutex;	 

};

#endif

