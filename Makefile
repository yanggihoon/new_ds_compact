CPP		= arm-linux-g++
CC		= arm-linux-gcc
LD		= arm-linux-ld
AR		= arm-linux-ar
RANLIB		= arm-linux-ranlib
STRIP		= arm-linux-strip
#CPP		= g++
#CC		= gcc
#LD		= ld
#AR		= ar
#RANLIB		= ranlib
#STRIP		= strip
###############
SOAP_DIR	= soap
SOAP_LIB_DIR	= soap_lib

CPPFLAGS	= -DWITH_NONAMESPACES
CFLAGS		= 

INCPATH		= -I. -I$(SOAP_LIB_DIR) -I$(SOAP_DIR)
APP_LIBS	= -lpthread
####### Files

HEADERS = 	app.h \
		DeviceFactory/DeviceCreator.h \
		DeviceFactory/ConcreteDeviceCreator.h \
		DeviceProtocol/DeviceProtocol.h \
		DeviceProtocol/CMX_Boiler.h \
		DeviceProtocol/CMX_Bundlelight.h \
		DeviceProtocol/CMX_Gas.h \
		DeviceProtocol/CMX_Light.h \
		DeviceProtocol/HAMUN_Curtain.h \
		DeviceUart/CMX_UartRS485.h \
		DeviceUart/HAMUN_UartRS485.h \
		InterfaceHandler/SOAP_Handler.h \
		InterfaceService/SOAP_Service.h \
		LogUtil/lognamespace.h \
		LogUtil/logutil.h 

SOURCES =	main.cpp	\
		DeviceFactory/ConcreteDeviceCreator.cpp \
		DeviceProtocol/CMX_Boiler.cpp \
		DeviceProtocol/CMX_Bundlelight.cpp \
		DeviceProtocol/CMX_Gas.cpp \
		DeviceProtocol/CMX_Light.cpp \
		DeviceProtocol/HAMUN_Curtain.cpp \
		DeviceUart/CMX_UartRS485.cpp \
		DeviceUart/HAMUN_UartRS485.cpp \
		InterfaceHandler/SOAP_Handler.cpp \
		InterfaceService/SOAP_Service.cpp \
		LogUtil/logutil.cpp 


APP_OBJECTS =	main.o \
		LogUtil/logutil.o	\
		DeviceFactory/ConcreteDeviceCreator.o \
		DeviceProtocol/CMX_Boiler.o \
		DeviceProtocol/CMX_Bundlelight.o \
		DeviceProtocol/CMX_Gas.o \
		DeviceProtocol/CMX_Light.o \
		DeviceProtocol/HAMUN_Curtain.o \
		DeviceUart/CMX_UartRS485.o \
		DeviceUart/HAMUN_UartRS485.o \
		InterfaceHandler/SOAP_Handler.o \
		InterfaceService/SOAP_Service.o \
		$(SOAP_LIB_DIR)/stdsoap2.o \
		$(SOAP_DIR)/cmxDeviceServiceC.o \
		$(SOAP_DIR)/cmxDeviceServiceClient.o \
		$(SOAP_LIB_DIR)/envC.o 	\
		$(SOAP_DIR)/cmxDeviceServiceServer.o


APP_TARGET	=	new_ds_compact

####### Implicit rules

.SUFFIXES: .cpp .c

.cpp.o:
	$(CPP) -c $(CPPFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules
all: app

app : $(APP_OBJECTS)
	$(CPP) -o $(APP_TARGET) $(APP_OBJECTS) $(APP_LIBS)
	$(STRIP) $(APP_TARGET)

clean:
	-rm -f $(LIB_OBJECTS) $(APP_OBJECTS) $(APP_TARGET)

soap:
	mkdir -p $(SOAP_DIR)
	mkdir -p $(SOAP_LIB_DIR)
	touch $(SOAP_LIB_DIR)/env.h
	soapcpp2 -d $(SOAP_LIB_DIR) -penv $(SOAP_LIB_DIR)/env.h
	soapcpp2 -n -x -d $(SOAP_DIR) ds_intf.h
	

