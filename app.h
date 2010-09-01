#ifndef APP_H_
#define APP_H_

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#include <unistd.h>
#include <sys/signal.h>

#include "LogUtil/logutil.h"
#include "LogUtil/lognamespace.h"

#include "DeviceFactory/ConcreteDeviceCreator.h"
#include "DeviceUart/CMX_UartRS485.h"
#include "DeviceUart/HAMUN_UartRS485.h"
#include "DeviceUart/NOKSUNG_UartRS485.h"
#include "InterfaceHandler/SOAP_Handler.h"
#include "InterfaceService/SOAP_Service.h"


#define TRUE			1
#define FALSE		0

#define SUCCESS		1
#define FAIL			0

#define EVENT  		1
#define CONTROL		2

#define ALL_DEVICE_COUNT					15

int NotifyEventToService(enum DEVICE_NAME dName, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
int NotifyControlToDevice(enum DEVICE_NAME dName, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);
void notify_print(unsigned char cmd_mode, enum DEVICE_NAME dName, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4);

int get_device_index(enum DEVICE_NAME device_name);	
enum DEVICE_NAME get_name(int categoryIndex);
enum DEVICE_PROTOCOL get_protocol(enum DEVICE_NAME device_name);
unsigned int get_devicecategory_cnt();
int check_device_supported(enum DEVICE_NAME device_name);
int get_current_supported_cnt(enum DEVICE_NAME dName);

int check_each_ack(enum DEVICE_NAME device_name, int order);
int check_device_disconnection(enum DEVICE_NAME device_name);
int check_device_port_error();

void  get_device_property(enum DEVICE_NAME device_name, int order, D_Property* d_property);
void get_device_item(enum DEVICE_NAME device_name, int order, D_Item* d_item);

void device_status_print(enum DEVICE_NAME device_name);

#endif
