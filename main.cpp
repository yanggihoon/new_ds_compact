
#include <app.h>

#define BUNDLELIGHT_GAS_CLOSE_MAX_WAIT_COUNT	100	//한개 polling은 대략 200ms 1초 polling은 5개 = 10초 polling은 50개 
#define MAX_CONF_BUF				100
#define MAX_DEVICE_CATEGORY		10
#define POLLING_TIME				100000

void SignalHandler(int sig);
void Close();

int mainRunningFlag = TRUE;
int controlFlag = FALSE;
int pollingDeviceIndex = 0;
int pollingDeviceOrder = 1;
int deviceCategoryCnt = 0;
DeviceProtocol* deviceCategory[MAX_DEVICE_CATEGORY] = {NULL, };
clock_t t_begin, t_end, t_elap;


int ParsingLine(char str[][MAX_CONF_BUF], char *line)
{
	int i=0;
	char *token;
	char seps[]   = " \t\n";

	i = 0;
	token = strtok( line, seps );
	while( token != NULL )
	{
		strcpy(str[i],token);
		i++;
        token = strtok( NULL, seps );
	}

	return i-1;
}

int GetHex(char s)
{
	int ret = 0;
	switch (s)
	{
	case 'A':
	case 'a':
		ret = 0xA;
		break;

	case 'B':
	case 'b':
		ret = 0xB;
		break;

	case 'C':
	case 'c':
		ret = 0xC;
		break;

	case 'D':
	case 'd':
		ret = 0xD;
		break;

	case 'E':
	case 'e':
		ret = 0xE;
		break;

	case 'F':
	case 'f':
		ret = 0xF;
		break;
	}
	return ret;
}

void Char2Hex(int *hex, char *str)
{
	
	int len = strlen(str);
	int *tmp = (int *) malloc(sizeof(int *) * len);
	char buf[2];
	*hex = 0;
	for (int i = 0; i < len; i++)
	{
		if ((('a' <= str[i]) && ('f' >= str[i])) || (('A' <= str[i]) && ('F' >= str[i])))
		{
			tmp[i] = GetHex(str[i]);
		}
		else if ((48 <= str[i]) && (57 >= str[i]))
		{
			buf[0] = str[i];
			buf[1] = '\0';
			tmp[i] = atoi(buf);
		}
	}
	
	for (int i = 0; i < len; i++)
	{
		tmp[i] *= (int)pow(16.0, (double) (len - (1+i)));
		*hex += tmp[i];
	}
	free(tmp);
}

char * ReadLine(char *prompt)
{
	char line[1000];
	int i = 0;

	printf("%s", prompt);
	if (fgets(line, 1000, stdin) == NULL) {
		return NULL;
	}
	for (i = strlen(line); i > 0; --i) {
		if (line[i] == '\n') {
			line[i] = '\0';
			break;
		}
	}

	return strdup(line);
}

void CmdCtrl(char* ctrl)
{
	char arg1[10] = {0}, arg2[10] = {0}, arg3[10] = {0}, arg4[10], arg5[10];
	int i, result = 0;
	int order;
	char confBuf[MAX_CONF_BUF][MAX_CONF_BUF] = {0};
	int tokenCnt = 0;
	DeviceProtocol* dCategory;
	D_Item	d_item;
	enum DEVICE_PROTOCOL dProtocol;
	
	int args = sscanf(ctrl, "%s %s %s %s %s", arg1, arg2, arg3, arg4, arg5);

	if(!strcmp(arg1, "elist"))
	{
		if(args != 1)
		{
			Log(LOG::ERR, "파라미터 수가 잘 못 되었습니다.\n");
			return;
		}

		list<SOAP_Handler::Event>::iterator it;
		for(it = 	(SOAP_Handler::Instance())->g_eventList.begin(); it != (SOAP_Handler::Instance())->g_eventList.end(); it++)

		switch(it->device_name)
		{
			case LIGHT:
				Log(LOG::INFO, "  LIGHT |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);
				break;
			case GAS:
				Log(LOG::INFO, "  GAS |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);
				break;
			case BOILER:
				Log(LOG::INFO, "  BOILER |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);
				break;
			case BUNDLELIGHT:
				Log(LOG::INFO, "  BUNDLELIGHT |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);				
				break;
			case CURTAIN:
				Log(LOG::INFO, "  CURTAIN |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);				
				break;
		}
	}
	else if(!strcmp(arg1, "clist"))
	{
		if(args != 1)
		{
			Log(LOG::ERR, "파라미터 수가 잘 못 되었습니다.\n");
			return;
		}

		list<SOAP_Handler::Control>::iterator it;
		for(it = 	(SOAP_Handler::Instance())->g_controlList.begin(); it != (SOAP_Handler::Instance())->g_controlList.end(); it++)

		switch(it->device_name)
		{
			case LIGHT:
				Log(LOG::INFO, "  LIGHT |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);
				break;
			case GAS:
				Log(LOG::INFO, "  GAS |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);
				break;
			case BOILER:
				Log(LOG::INFO, "  BOILER |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);
				break;
			case BUNDLELIGHT:
				Log(LOG::INFO, "  BUNDLELIGHT |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);				
				break;
			case CURTAIN:
				Log(LOG::INFO, "  CURTAIN |order = %d | function1 = %d | function2 = %d | function3 = %d | function4 = %d\n", it->order, it->function1, it->function2, it->function3, it->function4);				
				break;
		}

	}
	else if(!strcmp(arg1, "slist"))
	{
		if(args != 1)
		{
			Log(LOG::ERR, "파라미터 수가 잘 못 되었습니다.\n");
			return;
		}

		vector<string>::iterator subscriber_it;
		for(subscriber_it = (SOAP_Handler::Instance())->g_subscriberList.begin(); subscriber_it != (SOAP_Handler::Instance())->g_subscriberList.end(); subscriber_it++)
		{
			cout << "Registered End Point Url : " << *subscriber_it << endl;
		}
	}
	else if(!strcmp(arg1, "category"))
	{
		for(i = 0; i < deviceCategoryCnt; i++)
		{
			switch(deviceCategory[i]->dName)
			{
				case LIGHT:
						Log(LOG::INFO, "%d : LIGHT Supported\n", i);				
					break;

				case GAS:
						Log(LOG::INFO, "%d : GAS Supported\n", i);				
					break;

				case BUNDLELIGHT:
						Log(LOG::INFO, "%d : BUNDLELIGHT Supported\n", i);				
					break;

				case BOILER:
						Log(LOG::INFO, "%d : BOILER Supported\n", i);	
					break;
				case CURTAIN:
						Log(LOG::INFO, "%d : CURTAIN Supported\n", i);	
					break;
			}
		}
	}
	else if(!strcmp(arg1, "protocol"))
	{
		for(i = 0; i < deviceCategoryCnt; i++)
		{
			switch(deviceCategory[i]->dName)
			{
				case LIGHT:
						dProtocol = get_protocol(LIGHT);
						if(dProtocol == COMMAX)
							Log(LOG::INFO, "LIGHT Protocol is COMMAX\n");
						else
							Log(LOG::INFO, "LIGHT Protocol is Unknown\n");
					break;

				case GAS:
						dProtocol = get_protocol(GAS);
						if(dProtocol == COMMAX)
							Log(LOG::INFO, "GAS Protocol is COMMAX\n");
						else
							Log(LOG::INFO, "GAS Protocol is Unknown\n");
					break;

				case BUNDLELIGHT:
						dProtocol = get_protocol(BUNDLELIGHT);
						if(dProtocol == COMMAX)
							Log(LOG::INFO, "BUNDLELIGHT Protocol is COMMAX\n");
						else
							Log(LOG::INFO, "BUNDLELIGHT Protocol is Unknown\n");
					break;

				case BOILER:
						dProtocol = get_protocol(BOILER);
						if(dProtocol == COMMAX)
							Log(LOG::INFO, "BOILER Protocol is COMMAX\n");
						else
							Log(LOG::INFO, "BOILER Protocol is Unknown\n");
					break;
					
				case CURTAIN:
						dProtocol = get_protocol(CURTAIN);
						if(dProtocol == COMMAX)
							Log(LOG::INFO, "CURTAIN Protocol is COMMAX\n");
						else if(dProtocol == HAMUN)
							Log(LOG::INFO, "CURTAIN Protocol is HAMUN\n");							
						else
							Log(LOG::INFO, "CURTAIN Protocol is Unknown\n");
					break;
					
			}
		}
	}
	else if(!strcmp(arg1, "ack"))
	{
		for(i = 0; i < deviceCategoryCnt; i++)
		{
			dProtocol = get_protocol(deviceCategory[i]->dName);
			dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(deviceCategory[i]->dName, dProtocol);
			result = dCategory->checkDisconnected();

			switch(deviceCategory[i]->dName)
			{
				case LIGHT:
						if(result == TRUE)
							Log(LOG::INFO, "LIGHT connection is TRUE\n");
						else
							Log(LOG::INFO, "LIGHT is disconnected\n");
					break;

				case GAS:
						if(result == TRUE)
							Log(LOG::INFO, "GAS connection is TRUE\n");
						else
							Log(LOG::INFO, "GAS is disconnected\n");
					break;

				case BUNDLELIGHT:
						if(result == TRUE)
							Log(LOG::INFO, "BUNDLELIGHT connection is TRUE\n");
						else
							Log(LOG::INFO, "BUNDLELIGHT is disconnected\n");

					break;

				case BOILER:
						if(result == TRUE)
							Log(LOG::INFO, "BOILER connection is TRUE\n");
						else
							Log(LOG::INFO, "BOILER connection is disconnected\n");

					break;

				case CURTAIN:
						if(result == TRUE)
							Log(LOG::INFO, "CURTAIN connection is TRUE\n");
						else
							Log(LOG::INFO, "CURTAIN connection is disconnected\n");

					break;
			}
		}
	}
	else if(!strcmp(arg1, "port"))
	{
		result = check_device_port_error();
		if(result == TRUE)
			Log(LOG::INFO, "PORT is OK\n");
		else			
			Log(LOG::INFO, "PORT is error\n");
	}
	else if(!strcmp(arg1, "st"))
	{
		if(args != 1)
		{
			Log(LOG::ERR, "파라미터 수가 잘 못 되었습니다.\n");
			return;
		}

		for(i = 0; i < deviceCategoryCnt; i++)
			device_status_print(deviceCategory[i]->dName);

	}
	else if(!strcmp(arg1, "bundlelight"))
	{
		dProtocol = get_protocol(BUNDLELIGHT);
		dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BUNDLELIGHT, dProtocol);

		if(args != 2)
		{
			Log(LOG::ERR, "파라미터 수가 잘 못 되었습니다.\n");
			return;
		}

		if(!strcmp(arg2, "function"))
		{
			if(dProtocol == COMMAX)
			{
				if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[0].supported_function._gasclose == SUPPORTED)
					Log(LOG::INFO, "BUNDLELIGHT GAS_CLOSE SUPPORTED\n");
				else
					Log(LOG::INFO, "BUNDLELIGHT GAS_CLOSE NOT SUPPORTED\n");

				if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[0].supported_function._out == SUPPORTED)
					Log(LOG::INFO, "BUNDLELIGHT OUT SUPPORTED\n");
				else
					Log(LOG::INFO, "BUNDLELIGHT OUT NOT SUPPORTED\n");

				if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[0].supported_function._power == SUPPORTED)
					Log(LOG::INFO, "BUNDLELIGHT POWER SUPPORTED\n");
				else
					Log(LOG::INFO, "BUNDLELIGHT POWER NOT SUPPORTED\n");

				if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[0].supported_function._readypower == SUPPORTED)
					Log(LOG::INFO, "BUNDLELIGHT READYPOWER SUPPORTED\n");
				else
					Log(LOG::INFO, "BUNDLELIGHT READYPOWER NOT SUPPORTED\n");

				if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[0].supported_function._elevatorcall == SUPPORTED)
					Log(LOG::INFO, "BUNDLELIGHT ELEVATORCALL SUPPORTED\n");
				else
					Log(LOG::INFO, "BUNDLELIGHT ELEVATORCALL NOT SUPPORTED\n");
				
			}
		}
	}
	else if(!strcmp(arg1, "boiler"))
	{
		dProtocol = get_protocol(BOILER);
		dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BOILER, dProtocol);

		if(args != 2)
		{
			Log(LOG::ERR, "파라미터 수가 잘 못 되었습니다.\n");
			return;
		}

		if(!strcmp(arg2, "method"))
		{
			if(dProtocol == COMMAX)
			{
				if(((CMX_Boiler*)dCategory)->boilerMode == INDIVIDUAL_BOILER)
					Log(LOG::INFO, "BOILER MODE = INDIVIDUAL_BOILER\n");
				else if(((CMX_Boiler*)dCategory)->boilerMode == EACH_BOILER)
					Log(LOG::INFO, "BOILER MODE = EACH_BOILER\n");
				else
					Log(LOG::INFO, "BOILER MODE = NOT SUPPORTED\n");

				if(((CMX_Boiler*)dCategory)->boilerDetailMode == HYPOCAUST_BOILER)
					Log(LOG::INFO, "BOILER DETAIL MODE = HYPOCAUST_BOILER\n");
				else if(((CMX_Boiler*)dCategory)->boilerDetailMode == INNERTEMPERATURE_BOILER)
					Log(LOG::INFO, "BOILER DETAILMODE = INNERTEMPERATURE_BOILER\n");
				else
					Log(LOG::INFO, "BOILER DETAIL MODE = NOT SUPPORTED\n");

				Log(LOG::INFO, "BOILER COUNT = %d\n", ((CMX_Boiler*)dCategory)->boilerCnt);
				Log(LOG::INFO, "BOILER TemperatureMin = %d\n", ((CMX_Boiler*)dCategory)->boilerTemperatureMin);
				Log(LOG::INFO, "BOILER TemperatureMax = %d\n", ((CMX_Boiler*)dCategory)->boilerTemperatureMax);				
			}
		}
	}
	else
	{
		Log(LOG::ERR, "명령어가 잘 못 되었습니다.\n");
		return;
	}
}

//void* getCmd(void* arg)
void GetCmd()
{
	char *s = NULL, *s2 = NULL, *cmd = NULL;
	int line = 0;
	char prompt[100];

	printf("enter a command to execute\n");
	
	while(mainRunningFlag == TRUE)
	{
		sprintf(prompt, "%d>",  ++line);

		if (s)
			free(s);
		
		s = ReadLine(prompt);
		if (s != NULL) 
		{
			if (s2)
				free(s2);

			s2 = strdup(s);	

			//cmd = strtok(s2, " \t");
			cmd = s2;
		}

		if (!cmd)
			continue;

		if (!strcmp(cmd, "quit")) 
		{
			Close();
			exit(0);
		}
		else if(!strcmp(cmd, "help")) 
		{
			printf("****** Help *******\n");
			printf("elist : show current event list\n");
			printf("clist : show current control list\n");
			printf("slist : show current subscriber list\n");
			printf("category : show device category whether supported or not supported\n");
			printf("protocol : show protocol of supported device\n");
			printf("ack : show device connection status whether connected or disconnected\n");
			printf("port : show port error\n");
			printf("st : show current all device status\n");
			printf("bundlelight function : show supported function of bundlelight\n");
			printf("boiler method : show boiler method\n");
			printf("please Help Me (-_-);;;\n");	
		}
		else
		{
			if(strlen(cmd) > 1)
				CmdCtrl(cmd);
		}
	}
}

int SetLogType(int cnt, char* param)
{
	int log_type = 0;
	
	if(cnt > 1)
	{
		if(strcasecmp(param, "-debug") == 0)
		{
			g_logType = LOG::ERR | LOG::SEP | LOG::INFO;
			log_type = 2;
		}

		if(strcasecmp(param, "-mgr") == 0)
		{
			g_logType = LOG::ERR | LOG::SEP | LOG::MGR;
			log_type = 2;
		}

		if(strcasecmp(param, "-dev") == 0)
		{
			g_logType = LOG::ERR | LOG::SEP | LOG::UART | LOG::PRTCL;
			log_type = 1;
		}

		if(strcasecmp(param, "-intf") == 0)
		{
			g_logType = LOG::ERR | LOG::SEP | LOG::HND | LOG::SERV;
			log_type = 1;
		}

		if(strcasecmp(param, "-all") == 0)
		{
			g_logType = LOG::ERR | LOG::SEP | LOG::MGR | LOG::UART | LOG::PRTCL | LOG::HND | LOG::SERV;
			log_type = 1;
		}

	}

	return log_type;
}

int SetDeviceConfigure()
{
	FILE *fconf;
	char confBuf[MAX_CONF_BUF][MAX_CONF_BUF];
	char confTemp[MAX_CONF_BUF];
	int tokenCnt;
	int result;
	
	if((fconf = fopen("device.conf","r")) == NULL)	
	{
		perror("device.conf file open error\n");
		return -1;
	}


	while (fgets(confTemp, MAX_CONF_BUF, fconf))
	{
		memset(confBuf, 0x00, MAX_CONF_BUF * MAX_CONF_BUF);

		tokenCnt = ParsingLine(confBuf, confTemp);

		if(!strcmp(confBuf[0], "LIGHT"))
		{
			if(!strcmp(confBuf[1], "COMMAX"))
			{
				deviceCategory[deviceCategoryCnt] =  (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(LIGHT, COMMAX);
				deviceCategory[deviceCategoryCnt]->supportedPollingCount = atoi(confBuf[2]);
				deviceCategory[deviceCategoryCnt]->dName = LIGHT;
				deviceCategory[deviceCategoryCnt]->pName = COMMAX;
				deviceCategory[deviceCategoryCnt]->iName = EXT2;
				deviceCategory[deviceCategoryCnt]->DeviceInit();
				deviceCategoryCnt++;				
			}
			Log(LOG::MGR, "%s %s Device Setting up, supportedPollingCount = %d\n", confBuf[0], confBuf[1], atoi(confBuf[2]));
		}

		if(!strcmp(confBuf[0], "BOILER"))
		{
			if(!strcmp(confBuf[1], "COMMAX"))
			{
				deviceCategory[deviceCategoryCnt] =  (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BOILER, COMMAX);
				deviceCategory[deviceCategoryCnt]->supportedPollingCount = atoi(confBuf[2]);
				deviceCategory[deviceCategoryCnt]->dName = BOILER;
				deviceCategory[deviceCategoryCnt]->pName = COMMAX;
				deviceCategory[deviceCategoryCnt]->iName = EXT2;
				deviceCategory[deviceCategoryCnt]->DeviceInit();
				deviceCategoryCnt++;				
			}
			Log(LOG::MGR, "%s %s Device Setting up, supportedPollingCount = %d\n", confBuf[0], confBuf[1], atoi(confBuf[2]));
		}

		if(!strcmp(confBuf[0], "BUNDLELIGHT"))
		{
			if(!strcmp(confBuf[1], "COMMAX"))
			{
				deviceCategory[deviceCategoryCnt] =  (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BUNDLELIGHT, COMMAX);
				deviceCategory[deviceCategoryCnt]->supportedPollingCount = atoi(confBuf[2]);
				deviceCategory[deviceCategoryCnt]->dName = BUNDLELIGHT;
				deviceCategory[deviceCategoryCnt]->pName = COMMAX;
				deviceCategory[deviceCategoryCnt]->iName = EXT2;
				deviceCategory[deviceCategoryCnt]->DeviceInit();
				deviceCategoryCnt++;				
			}
			Log(LOG::MGR, "%s %s Device Setting up, supportedPollingCount = %d\n", confBuf[0], confBuf[1], atoi(confBuf[2]));
		}

		if(!strcmp(confBuf[0], "GAS"))
		{
			if(!strcmp(confBuf[1], "COMMAX"))
			{
				deviceCategory[deviceCategoryCnt] =  (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(GAS, COMMAX);
				deviceCategory[deviceCategoryCnt]->supportedPollingCount = atoi(confBuf[2]);
				deviceCategory[deviceCategoryCnt]->dName = GAS;
				deviceCategory[deviceCategoryCnt]->pName = COMMAX;
				deviceCategory[deviceCategoryCnt]->iName = EXT2;
				deviceCategory[deviceCategoryCnt]->DeviceInit();
				deviceCategoryCnt++;
			}
			Log(LOG::MGR, "%s %s Device Setting up, supportedPollingCount = %d\n", confBuf[0], confBuf[1], atoi(confBuf[2]));
		}

		if(!strcmp(confBuf[0], "CURTAIN"))
		{
			if(!strcmp(confBuf[1], "HAMUN"))
			{
				deviceCategory[deviceCategoryCnt] =  (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(CURTAIN, HAMUN);
				deviceCategory[deviceCategoryCnt]->supportedPollingCount = atoi(confBuf[2]);
				deviceCategory[deviceCategoryCnt]->dName = CURTAIN;
				deviceCategory[deviceCategoryCnt]->pName = HAMUN;
				deviceCategory[deviceCategoryCnt]->iName = EXT1;
				deviceCategory[deviceCategoryCnt]->DeviceInit();
				deviceCategoryCnt++;
			}
			Log(LOG::MGR, "%s %s Device Setting up, supportedPollingCount = %d\n", confBuf[0], confBuf[1], atoi(confBuf[2]));
		}

	}

	fclose(fconf);

	return 1;

}

void* DevicePollingManger(void * arg)
{
	int i, index;
	int result;
	int polling_cnt = -1;
	int polling_cnt_start = FALSE;
	
	DeviceProtocol* dCategory;
	D_Item d_item;
	enum DEVICE_PROTOCOL dProtocol;

	if(deviceCategory[pollingDeviceIndex] == NULL)
	{
		Log(LOG::ERR, "device.conf file error\n");
		mainRunningFlag = FALSE;
	}
	
	usleep(POLLING_TIME);

	while(mainRunningFlag == TRUE)
	{
		usleep(POLLING_TIME);

		if(polling_cnt_start == TRUE)
			polling_cnt++;
		
		if(controlFlag == FALSE)
		{
			switch(deviceCategory[pollingDeviceIndex]->dName)
			{
				case LIGHT:
					if(deviceCategory[pollingDeviceIndex]->pName == COMMAX)
					{
						Log(LOG::MGR, "%s Polling\n", ((CMX_Light*)deviceCategory[pollingDeviceIndex])->lightStatus[pollingDeviceOrder -1].deviceCharName);
						result = ((CMX_Light*)deviceCategory[pollingDeviceIndex])->FrameMake(POLLING_CMD, pollingDeviceOrder, 0, 0, 0, 0);
						pollingDeviceOrder++;	
						
						if(result == FAIL)
							Log(LOG::ERR, "%s Polling Error\n", ((CMX_Light*)deviceCategory[pollingDeviceIndex])->lightStatus[pollingDeviceOrder - 1].deviceCharName);					

						if(pollingDeviceOrder > deviceCategory[pollingDeviceIndex]->supportedPollingCount)
						{
							pollingDeviceOrder = 1;
							pollingDeviceIndex++;
						}
					}
					break;
					
				case GAS:
					if(deviceCategory[pollingDeviceIndex]->pName == COMMAX)
					{
						Log(LOG::MGR, "%s Polling\n", ((CMX_Gas*)deviceCategory[pollingDeviceIndex])->gasStatus[pollingDeviceOrder -1].deviceCharName);
						result = ((CMX_Gas*)deviceCategory[pollingDeviceIndex])->FrameMake(POLLING_CMD, pollingDeviceOrder, 0, 0, 0, 0);
						pollingDeviceOrder++;	

						if(result == FAIL)
							Log(LOG::ERR, "%s Polling Error\n", ((CMX_Gas*)deviceCategory[pollingDeviceIndex])->gasStatus[pollingDeviceOrder - 1].deviceCharName);					

						if(pollingDeviceOrder > deviceCategory[pollingDeviceIndex]->supportedPollingCount)
						{
							pollingDeviceOrder = 1;
							pollingDeviceIndex++;
						}
					}
					break;

				case BOILER:
					if(deviceCategory[pollingDeviceIndex]->pName == COMMAX)
					{
						if(((CMX_Boiler*)deviceCategory[pollingDeviceIndex])->isMethodPolled == FALSE)
						{
							Log(LOG::MGR, "Boiler Method Polling\n");
							result = ((CMX_Boiler*)deviceCategory[pollingDeviceIndex])->FrameMake(POLLING_CMD, 0, 0, 0, 0, 0);							
						}
						else
						{
							Log(LOG::MGR, "%s Polling\n", ((CMX_Boiler*)deviceCategory[pollingDeviceIndex])->boilerStatus[pollingDeviceOrder -1].deviceCharName);
							result = ((CMX_Boiler*)deviceCategory[pollingDeviceIndex])->FrameMake(POLLING_CMD, pollingDeviceOrder, 0, 0, 0, 0);
							pollingDeviceOrder++;
						}
						
						if(result == FAIL)
							Log(LOG::ERR, "%s Polling Error\n", ((CMX_Boiler*)deviceCategory[pollingDeviceIndex])->boilerStatus[pollingDeviceOrder - 1].deviceCharName);					

						if(pollingDeviceOrder > deviceCategory[pollingDeviceIndex]->supportedPollingCount)
						{
							((CMX_Boiler*)deviceCategory[pollingDeviceIndex])->isMethodPolled = FALSE;
							pollingDeviceOrder = 1;
							pollingDeviceIndex++;
						}

					}
					break;
					
				case BUNDLELIGHT:
					if(deviceCategory[pollingDeviceIndex]->pName == COMMAX)
					{				
						if(((CMX_Bundlelight*)deviceCategory[pollingDeviceIndex])->bundlelightStatus[pollingDeviceOrder - 1].gasClose == BUNDLELIGHT_GASCLOSE_REQUEST)
						{
							polling_cnt_start = TRUE;

							for(index = 0; index < get_current_supported_cnt(GAS); index++)
							{
								get_device_item(GAS, index + 1, &d_item);
								if(d_item.gasItem.action == GAS_CLOSE)
								{
									polling_cnt = 0;
									polling_cnt_start = FALSE;
	
									notify_print(EVENT, BUNDLELIGHT, pollingDeviceOrder, BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS, 0, 0);
									(SOAP_Handler::Instance())->control.device_name = BUNDLELIGHT;
									(SOAP_Handler::Instance())->control.order = pollingDeviceOrder;
									(SOAP_Handler::Instance())->control.function1 = BUNDLELIGHT_GASCLOSE_EVENT;
									(SOAP_Handler::Instance())->control.function2 = BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS;
									(SOAP_Handler::Instance())->control.function3 = 0x00;
									(SOAP_Handler::Instance())->control.function4 = 0x00;

									result = (SOAP_Handler::Instance())->Add_Control((SOAP_Handler::Instance())->control);
									
								}
								else 
								{
									if(polling_cnt > BUNDLELIGHT_GAS_CLOSE_MAX_WAIT_COUNT)
									{
										polling_cnt = 0;
										polling_cnt_start = FALSE;
	
										notify_print(EVENT, BUNDLELIGHT, pollingDeviceOrder, BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL, 0, 0);
										(SOAP_Handler::Instance())->control.device_name = BUNDLELIGHT;
										(SOAP_Handler::Instance())->control.order =pollingDeviceOrder;
										(SOAP_Handler::Instance())->control.function1 = BUNDLELIGHT_GASCLOSE_EVENT;
										(SOAP_Handler::Instance())->control.function2 = BUNDLELIGHT_GASCLOSE_REQUEST_FAIL;
										(SOAP_Handler::Instance())->control.function3 = 0x00;
										(SOAP_Handler::Instance())->control.function4 = 0x00;

										result = (SOAP_Handler::Instance())->Add_Control((SOAP_Handler::Instance())->control);
									}
									else
									{
										pollingDeviceIndex = get_device_index(GAS);	
										if(pollingDeviceIndex == -1) pollingDeviceIndex = 0;
										pollingDeviceOrder = 1;
									}
								}
							}
						}
						else
						{
							Log(LOG::MGR, "%s Polling\n", ((CMX_Bundlelight*)deviceCategory[pollingDeviceIndex])->bundlelightStatus[pollingDeviceOrder -1].deviceCharName);
							result = ((CMX_Bundlelight*)deviceCategory[pollingDeviceIndex])->FrameMake(POLLING_CMD, pollingDeviceOrder, 0, 0, 0, 0);
							pollingDeviceOrder++;	
							
							if(result == FAIL)
								Log(LOG::ERR, "%s Polling Error\n", ((CMX_Bundlelight*)deviceCategory[pollingDeviceIndex])->bundlelightStatus[pollingDeviceOrder - 1].deviceCharName);					
							
							if(pollingDeviceOrder > deviceCategory[pollingDeviceIndex]->supportedPollingCount)
							{
								pollingDeviceOrder = 1;
								pollingDeviceIndex++;
							}
						}
					}
					break;

				case CURTAIN:
					if(deviceCategory[pollingDeviceIndex]->pName == HAMUN)
					{
						//하문용 커튼은 폴링 방식이 아니므로 디바이스 상태 값을 확인 하기 위해 처음 한 번만 폴링(STOP control으로 폴링) 하고 따로 폴링을 하지 않는다.
						if(((HAMUN_Curtain*)deviceCategory[pollingDeviceIndex])->isStartPolled == FALSE)
						{
							for(i = 1; i <= deviceCategory[pollingDeviceIndex]->supportedPollingCount; i++)
							{
								Log(LOG::MGR, "%s Polling\n", ((HAMUN_Curtain*)deviceCategory[pollingDeviceIndex])->curtainStatus[i].deviceCharName);
								result = ((HAMUN_Curtain*)deviceCategory[pollingDeviceIndex])->FrameMake(CONTROL_CMD, i, CURTAIN_ACTION_EVENT, CURTAIN_STOP, 0, 0);
								usleep(100000);
							}
							((HAMUN_Curtain*)deviceCategory[pollingDeviceIndex])->isStartPolled = TRUE;
						}
						else
						{
							pollingDeviceOrder = 1;
							pollingDeviceIndex++;
						}

					}
					break;
					
				default:
					break;
			}

			if(pollingDeviceIndex > deviceCategoryCnt - 1)
				pollingDeviceIndex = 0;

		}
	}
	
	Log(LOG::MGR, "Device Polling Thread End\n");
	pthread_exit(NULL);

	return NULL;
}

int NotifyControlToDevice(enum DEVICE_NAME dName, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result;
	D_Property d_property;
	DeviceProtocol* notifyDevice;
	enum DEVICE_PROTOCOL dProtocol;

	dProtocol = get_protocol(dName);

	notify_print(CONTROL, dName, order, function1, function2, function3, function4);
		
	switch(dName)
	{
		case LIGHT:
			{
				get_device_property(LIGHT, order, &d_property);
				if(d_property.lightProperty.mode == LIGHT_MODE_BINARY && function2 == LIGHT_DIMMING_EVENT)
					break;
					
				notifyDevice = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(LIGHT, dProtocol);			

				controlFlag = TRUE;

				usleep(POLLING_TIME);
				result = notifyDevice->FrameMake(CONTROL_CMD, order, function1, function2, function3, function4);

				if(d_property.lightProperty.mode == LIGHT_MODE_DIMMABLE)
					usleep(POLLING_TIME);
				
				pollingDeviceIndex = get_device_index(LIGHT);
				if(pollingDeviceIndex == -1) pollingDeviceIndex = 0;
				pollingDeviceOrder = order;

				controlFlag = FALSE;
			}
			break;
				
		case GAS:
			{
				get_device_property(GAS, order, &d_property);
					
				notifyDevice = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(GAS, dProtocol);			

				controlFlag = TRUE;

				usleep(POLLING_TIME);
				result = notifyDevice->FrameMake(CONTROL_CMD, order, function1, function2, function3, function4);

				pollingDeviceIndex = get_device_index(GAS);
				if(pollingDeviceIndex == -1) pollingDeviceIndex = 0;
				pollingDeviceOrder = order;

				controlFlag = FALSE;
			}
			break;

		case BUNDLELIGHT:
			{
				get_device_property(BUNDLELIGHT, order, &d_property);
					
				notifyDevice = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BUNDLELIGHT, dProtocol);			

				controlFlag = TRUE;

				usleep(POLLING_TIME);
				result = notifyDevice->FrameMake(CONTROL_CMD, order, function1, function2, function3, function4);
				
				if(function1 == BUNDLELIGHT_GASCLOSE_EVENT)
				{
					pollingDeviceIndex = get_device_index(GAS);	
					if(pollingDeviceIndex == -1) pollingDeviceIndex = 0;
					pollingDeviceOrder = 1;
				}
				else
				{
					pollingDeviceIndex = get_device_index(BUNDLELIGHT);
					if(pollingDeviceIndex == -1) pollingDeviceIndex = 0;
					pollingDeviceOrder = order;
				}

				controlFlag = FALSE;
			}			
			break;

		case BOILER:
			{
				get_device_property(BOILER, order, &d_property);
					
				notifyDevice = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BOILER, dProtocol);			

				controlFlag = TRUE;

				usleep(POLLING_TIME);
				result = notifyDevice->FrameMake(CONTROL_CMD, order, function1, function2, function3, function4);
				
				pollingDeviceIndex = get_device_index(BOILER);
				if(pollingDeviceIndex == -1) pollingDeviceIndex = 0;
				pollingDeviceOrder = order;

				controlFlag = FALSE;
			}
			break;

		case CURTAIN:
			{
				get_device_property(CURTAIN, order, &d_property);
					
				notifyDevice = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(CURTAIN, dProtocol);			

				controlFlag = TRUE;

				usleep(POLLING_TIME);
				result = notifyDevice->FrameMake(CONTROL_CMD, order, function1, function2, function3, function4);
				
				pollingDeviceIndex = get_device_index(CURTAIN);
				if(pollingDeviceIndex == -1) pollingDeviceIndex = 0;
				pollingDeviceOrder = order;

				controlFlag = FALSE;
			}
			break;


		default:

			break;
	}

	return result;
}

int NotifyEventToService(enum DEVICE_NAME dName, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
{
	int result = 0, i;
	int request = FALSE;
	D_Item d_item;

	switch(dName)
	{
		case LIGHT:
			(SOAP_Handler::Instance())->event.device_name = LIGHT;
			break;
				
		case GAS:
			(SOAP_Handler::Instance())->event.device_name = GAS;
			break;

		case BOILER:
			(SOAP_Handler::Instance())->event.device_name = BOILER;
			break;

		case BUNDLELIGHT:
			(SOAP_Handler::Instance())->event.device_name = BUNDLELIGHT;
			break;

		case CURTAIN:
			(SOAP_Handler::Instance())->event.device_name = CURTAIN;
			break;

		default:
			break;
	}

	notify_print(EVENT, dName, order, function1, function2, function3, function4);

	(SOAP_Handler::Instance())->event.order = order;
	(SOAP_Handler::Instance())->event.function1 = function1;
	(SOAP_Handler::Instance())->event.function2 = function2;
	(SOAP_Handler::Instance())->event.function3 = function3;
	(SOAP_Handler::Instance())->event.function4 = function4;

	result = (SOAP_Handler::Instance())->Add_Event((SOAP_Handler::Instance())->event);

	return result;
}


int main(int argc, char** argv)
{
	int result = 0, i;
	int log_type;

	signal(SIGINT, SignalHandler);
	signal(SIGTERM, SignalHandler);

/************************************************************************************/
/* Set Log Type																		*/
/************************************************************************************/
	log_type = SetLogType(argc, argv[1]);

	Log(LOG::MGR, "App version : "__DATE__" "__TIME__"\n");
/************************************************************************************/
/* Device Setting																		*/
/************************************************************************************/
	result = SetDeviceConfigure();
	if(result < 0)
		return -1;

	Log(LOG::MGR, "Device Setting Success\n");

/***************************************************************************************/
// UART  Open & Start
/**************************************************************************************/
	enum DEVICE_PROTOCOL dProtocol;
	
	for( i = 0; i < deviceCategoryCnt; i++)
	{
		switch(deviceCategory[i]->iName)
		{
			case EXT1:
				if(deviceCategory[i]->pName == COMMAX)
				{
					if((CMX_UartRS485::Instance())->isOpen() == FALSE)
					{
						result = (CMX_UartRS485::Instance())->UartOpen(6, 3);

						if(result == FAIL)
						{
							Log(LOG::ERR, "/dev/uart1 UartOpen error\n");
							(CMX_UartRS485::Instance())->UartClose();
						}
					}

					if((CMX_UartRS485::Instance())->isRunning() == FALSE)
						(CMX_UartRS485::Instance())->Start();

				}
				else if(deviceCategory[i]->pName == HAMUN)
				{
					if((HAMUN_UartRS485::Instance())->isOpen() == FALSE)
					{
						result = (HAMUN_UartRS485::Instance())->UartOpen(3, 1);
						if(result == FAIL)
						{
							Log(LOG::ERR, "/dev/ttyS2 UartOpen error\n");
							(HAMUN_UartRS485::Instance())->UartClose();
						}
					}

					if((HAMUN_UartRS485::Instance())->isRunning() == FALSE)
						(HAMUN_UartRS485::Instance())->Start();

				}
				break;
				
			case EXT2:
				if(deviceCategory[i]->pName == COMMAX)
				{
					if((CMX_UartRS485::Instance())->isOpen() == FALSE)
					{
						result = (CMX_UartRS485::Instance())->UartOpen(6, 3);

						if(result == FAIL)
						{
							Log(LOG::ERR, "/dev/uart1 UartOpen error\n");
							(CMX_UartRS485::Instance())->UartClose();
						}
					}

					if((CMX_UartRS485::Instance())->isRunning() == FALSE)
						(CMX_UartRS485::Instance())->Start();

				}
				break;
				
			default:
				break;
		}
	}

/***************************************************************************************/
// Soap Handler Start
/**************************************************************************************/
	(SOAP_Handler::Instance())->Event_Start();
	(SOAP_Handler::Instance())->Control_Start();

/***************************************************************************************/
// Soap Service Start
/**************************************************************************************/
	(SOAP_Service::Instance())->Start();

/***************************************************************************************/
// Polling Thread Start
/**************************************************************************************/
	pthread_attr_t	ta;
	pthread_t		t;
	pthread_attr_init(&ta);

	pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);	
	pthread_create(&t, &ta, DevicePollingManger, (void*)0);
	(void)pthread_attr_destroy(&ta);

	Log(LOG::MGR, "Polling Thread Start\n");

/***************************************************************************************/
// START
/**************************************************************************************/
	sleep(2);
	
	Log(LOG::MGR, "***************MAIN START***************\n");
	Log(LOG::SEP, "\n");

	if(log_type == 2)	//mgr mode
	{
		GetCmd();
	}
	else
	{
		while(mainRunningFlag == TRUE)
		{
			sleep(2);
		}

	}

	printf(" ### MAIN PROGRAM : ENDING ###\n");

	//exit(-1);
	return 0;
} 

/***************************************************************************************/
// EXIT
/**************************************************************************************/
void SignalHandler(int sig)
{
	printf("\n");
	printf(" ### SIGNAL HANDLER : WATIING ###\n");

	mainRunningFlag = FALSE;
	sleep(2);
	Close();
	
	printf(" ### SIGNAL HANDLER : ENDING ###\n");
}

void Close()
{
	int i;

	(CMX_UartRS485::Instance())->Close();
	(HAMUN_UartRS485::Instance())->Close();	
	(SOAP_Handler::Instance())->Close();
	(SOAP_Service::Instance())->Close();

	(DUMMY_Device::GetInstance())->Instance_Close();
	
	for(i = 0; i < deviceCategoryCnt; i++)
	{
		delete deviceCategory[i];
	}

}

/***************************************************************************************/
//SOAP Handler Helper Command
/**************************************************************************************/
int get_device_index(enum DEVICE_NAME device_name)
{
	int i;
	for(i = 0; i < deviceCategoryCnt; i++)
	{
		if(deviceCategory[i]->dName == device_name)
			return i;
	}

	return -1;
}

enum DEVICE_NAME get_name(int categoryIndex)
{
	return deviceCategory[categoryIndex]->dName;
}

enum DEVICE_PROTOCOL get_protocol(enum DEVICE_NAME device_name)
{
	int i;

	for(i = 0; i < deviceCategoryCnt; i++)
		if(deviceCategory[i]->dName  == device_name)
			return deviceCategory[i]->pName;

	return PROTOCOL_DUMMY;
}

unsigned int get_devicecategory_cnt()
{
	return deviceCategoryCnt;
}

int get_current_supported_cnt(enum DEVICE_NAME device_name)
{
	int count;
	DeviceProtocol* dCategory;
	enum DEVICE_PROTOCOL dProtocol;

	dProtocol = get_protocol(device_name);

	dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(device_name, dProtocol);			

	count = dCategory->getCurrentSupportedCount();

	return count;
}

int check_device_disconnection(enum DEVICE_NAME device_name)
{

	int flag = FALSE;

	DeviceProtocol* dCategory;
	enum DEVICE_PROTOCOL dProtocol;
	
	dProtocol = get_protocol(device_name);

	dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(device_name, dProtocol);

	flag = dCategory->checkDisconnected();

	return flag;
}

int check_device_port_error()
{
	int i, flag = FALSE;
	for(i = 0; i < deviceCategoryCnt; i++)
	{
		flag = check_device_disconnection(deviceCategory[i]->dName);
		if(flag == TRUE)
			return TRUE;
	}

	return FALSE;
}

void  get_device_property(enum DEVICE_NAME device_name, int order, D_Property* d_property)
{
	DeviceProtocol *dCategory;
	enum DEVICE_PROTOCOL dProtocol;
	
	dProtocol = get_protocol(device_name);
	dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(device_name, dProtocol);

	if(order <= 0 || order > 0xFF) order = 1;

	switch(dCategory->dName)
	{
		case LIGHT:
			if(dCategory->pName == COMMAX)
			{
				if(((CMX_Light*)dCategory)->lightStatus[order - 1].mode == LIGHT_MODE_BINARY)
				{
					d_property->lightProperty.mode = LIGHT_MODE_BINARY;
					d_property->lightProperty.power = SUPPORTED;
					d_property->lightProperty.allPower = SUPPORTED;
					d_property->lightProperty.dimmerLevelFrom	= 0;
					d_property->lightProperty.dimmerLevelTo	= 0;	
				}
				else if(((CMX_Light*)dCategory)->lightStatus[order - 1].mode == LIGHT_MODE_DIMMABLE)
				{
					d_property->lightProperty.mode = LIGHT_MODE_DIMMABLE;
					d_property->lightProperty.power = SUPPORTED;
					d_property->lightProperty.allPower = SUPPORTED;
					d_property->lightProperty.dimmerLevelFrom	= 1;
					d_property->lightProperty.dimmerLevelTo	= 8;	
				}
				else
				{
					d_property->lightProperty.mode = NOT_SUPPORTED;
					d_property->lightProperty.power = NOT_SUPPORTED;
					d_property->lightProperty.allPower = NOT_SUPPORTED;
					d_property->lightProperty.dimmerLevelFrom	= 0;
					d_property->lightProperty.dimmerLevelTo	= 0;	
				}
			}
			break;

		case GAS:
			if(dCategory->pName == COMMAX)
			{
				if(((CMX_Gas*)dCategory)->gasStatus[order - 1].order != 0xFF)
				{
					d_property->gasProperty.action = SUPPORTED;
					d_property->gasProperty.alarm = SUPPORTED;
				}
				else
				{
					d_property->gasProperty.action = NOT_SUPPORTED;
					d_property->gasProperty.alarm = NOT_SUPPORTED;
				}
			}
			break;

		case BUNDLELIGHT:
			if(dCategory->pName == COMMAX)
			{
				if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].order != 0xFF)
				{
					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].supported_function._power == SUPPORTED)
						d_property->bundleLightProperty.power = SUPPORTED;
					else
						d_property->bundleLightProperty.power = NOT_SUPPORTED;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].supported_function._readypower == SUPPORTED)
						d_property->bundleLightProperty.readyPower = SUPPORTED;					
					else
						d_property->bundleLightProperty.readyPower = NOT_SUPPORTED;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].supported_function._out == SUPPORTED)
						d_property->bundleLightProperty.out = SUPPORTED;					
					else
						d_property->bundleLightProperty.out = NOT_SUPPORTED;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].supported_function._gasclose == SUPPORTED)
						d_property->bundleLightProperty.gasClose = SUPPORTED;					
					else
						d_property->bundleLightProperty.gasClose = NOT_SUPPORTED;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].supported_function._elevatorcall == SUPPORTED)
						d_property->bundleLightProperty.elevatorCall = SUPPORTED;					
					else
						d_property->bundleLightProperty.elevatorCall = NOT_SUPPORTED;
				}
				else
				{
					d_property->bundleLightProperty.power = NOT_SUPPORTED;
					d_property->bundleLightProperty.readyPower= NOT_SUPPORTED;
					d_property->bundleLightProperty.out = NOT_SUPPORTED;
					d_property->bundleLightProperty.gasClose = NOT_SUPPORTED;
					d_property->bundleLightProperty.elevatorCall = NOT_SUPPORTED;					
				}
			}
			break;

		case BOILER:
			if(dCategory->pName == COMMAX)
			{
				if(((CMX_Boiler*)dCategory)->boilerMode == INDIVIDUAL_BOILER)
				{
					d_property->boilerProperty.boilerMode = INDIVIDUAL_BOILER;
					d_property->boilerProperty.outMode = NOT_SUPPORTED;
					d_property->boilerProperty.power = SUPPORTED;
					d_property->boilerProperty.requestTemperature = SUPPORTED;
					d_property->boilerProperty.currentTemperature = SUPPORTED;
				}
				else if(((CMX_Boiler*)dCategory)->boilerMode == EACH_BOILER)
				{
					d_property->boilerProperty.boilerMode = EACH_BOILER;
					d_property->boilerProperty.outMode = SUPPORTED;
					d_property->boilerProperty.power = SUPPORTED;
					d_property->boilerProperty.requestTemperature = SUPPORTED;
					d_property->boilerProperty.currentTemperature = SUPPORTED;
				}
				else
				{
					d_property->boilerProperty.boilerMode = NOT_SUPPORTED;
					d_property->boilerProperty.outMode = NOT_SUPPORTED;	
					d_property->boilerProperty.power = NOT_SUPPORTED;						
					d_property->boilerProperty.requestTemperature = NOT_SUPPORTED;
					d_property->boilerProperty.currentTemperature = NOT_SUPPORTED;
				}
				
				if(((CMX_Boiler*)dCategory)->boilerDetailMode == HYPOCAUST_BOILER)
				{
					d_property->boilerProperty.dipSwitchMode = HYPOCAUST_BOILER;
					d_property->boilerProperty.requestTemperatureMax = 85;
					d_property->boilerProperty.requestTemperatureMin = 40;
					d_property->boilerProperty.currentTemperatureMax = 85;
					d_property->boilerProperty.currentTemperatureMin = 40;
				}
				else if(((CMX_Boiler*)dCategory)->boilerDetailMode == INNERTEMPERATURE_BOILER)
				{
					d_property->boilerProperty.dipSwitchMode = INNERTEMPERATURE_BOILER;
					d_property->boilerProperty.requestTemperatureMax = ((CMX_Boiler*)dCategory)->boilerTemperatureMax;
					d_property->boilerProperty.requestTemperatureMin = ((CMX_Boiler*)dCategory)->boilerTemperatureMin;
					d_property->boilerProperty.currentTemperatureMax = ((CMX_Boiler*)dCategory)->boilerTemperatureMax;
					d_property->boilerProperty.currentTemperatureMin = ((CMX_Boiler*)dCategory)->boilerTemperatureMin; 
				}
				else 
				{
					d_property->boilerProperty.dipSwitchMode = NOT_SUPPORTED;
					d_property->boilerProperty.requestTemperatureMax = 0;
					d_property->boilerProperty.requestTemperatureMin = 0;
					d_property->boilerProperty.currentTemperatureMax = 0;
					d_property->boilerProperty.currentTemperatureMin = 0;
				}
			
				d_property->boilerProperty.heatingPower = NOT_SUPPORTED;
				d_property->boilerProperty.heatingWaterTemperature = NOT_SUPPORTED;
				d_property->boilerProperty.heatingWaterTemperatureMax = NOT_SUPPORTED;
				d_property->boilerProperty.heatingWaterTemperatureMin = NOT_SUPPORTED;

			}
			break;

		case CURTAIN:
			if(dCategory->pName == HAMUN)
			{
				if(((HAMUN_Curtain*)dCategory)->curtainStatus[order - 1].order != 0xFF)
				{
					d_property->curtainProperty.action = SUPPORTED;
					d_property->curtainProperty.angular = NOT_SUPPORTED;
				}
				else
				{
					d_property->curtainProperty.action = NOT_SUPPORTED;
					d_property->curtainProperty.angular = NOT_SUPPORTED;
				}
			}
			break;
							
		default:
			break;
	}
}

void get_device_item(enum DEVICE_NAME device_name, int order, D_Item* d_item)
{
	int index, i;
	int ack;
	DeviceProtocol *dCategory;
	enum DEVICE_PROTOCOL dProtocol;

	ack = check_device_port_error();	
	dProtocol = get_protocol(device_name);
	dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(device_name, dProtocol);

	switch(dCategory->dName)
	{
		case LIGHT:
			if(dCategory->pName == COMMAX)
			{
				if(ack == FALSE)
					d_item->lightItem.error = PORT_ERROR;
				else if(check_device_disconnection(LIGHT) == FALSE)
					d_item->lightItem.error = LIGHT_DISCONNECTION;
				else
				{
					if(order > get_current_supported_cnt(LIGHT))
					{
						Log(LOG::ERR, "LIGHT order %d over Error\n", order);					
						break;							
					}

					d_item->lightItem.error = NO_ERROR;

					if(((CMX_Light*)dCategory)->lightStatus[order - 1].power == LIGHT_POWER_OFF)
						d_item->lightItem.power = LIGHT_POWER_OFF;
					else if(((CMX_Light*)dCategory)->lightStatus[order - 1].power == LIGHT_POWER_ON)
						d_item->lightItem.power = LIGHT_POWER_ON;
					else
						d_item->lightItem.power = LIGHT_POWER_NONE;

					if(((CMX_Light*)dCategory)->lightStatus[order - 1].mode == LIGHT_MODE_DIMMABLE)
						d_item->lightItem.dimmingLevel = ((CMX_Light*)dCategory)->lightStatus[order - 1].dimmingLevel;
					else
						d_item->lightItem.dimmingLevel = 0;
				}
			}
	
			break;

		case GAS:
			if(dCategory->pName == COMMAX)
			{
				if(ack == FALSE)
					d_item->gasItem.error = PORT_ERROR;
				else if(check_device_disconnection(GAS) == FALSE)
					d_item->gasItem.error = GAS_DISCONNECTION;
				else
				{
					if(order > get_current_supported_cnt(GAS))
					{
						Log(LOG::ERR, "GAS order %d over Error\n", order);					
						break;							
					}

					d_item->gasItem.error = NO_ERROR;

					if(((CMX_Gas*)dCategory)->gasStatus[order - 1].action == GAS_OPEN)
						d_item->gasItem.action = GAS_OPEN;
					else if(((CMX_Gas*)dCategory)->gasStatus[order - 1].action == GAS_CLOSE)
						d_item->gasItem.action = GAS_CLOSE;
					else if(((CMX_Gas*)dCategory)->gasStatus[order - 1].action == GAS_LEAK)
						d_item->gasItem.action = GAS_LEAK;
					else
						d_item->gasItem.action = GAS_NONE;
				}
			}
	
			break;

		case BUNDLELIGHT:
			if(dCategory->pName == COMMAX)
			{
				if(ack == FALSE)
					d_item->bundleLightItem.error = PORT_ERROR;
				else if(check_device_disconnection(BUNDLELIGHT) == FALSE)
					d_item->bundleLightItem.error = BUNDLELIGHT_DISCONNECTION;
				else
				{
					if(order > get_current_supported_cnt(BUNDLELIGHT))
					{
						Log(LOG::ERR, "BUNDLELIGHT order %d over Error\n", order);					
						break;							
					}

					d_item->bundleLightItem.error = NO_ERROR;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].power == BUNDLELIGHT_POWER_ON)
						d_item->bundleLightItem.power = BUNDLELIGHT_POWER_ON;
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].power == BUNDLELIGHT_POWER_OFF)
						d_item->bundleLightItem.power = BUNDLELIGHT_POWER_OFF;
					else
						d_item->bundleLightItem.power = BUNDLELIGHT_POWER_NONE;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].readyPower == BUNDLELIGHT_READYPOWER_ON)
						d_item->bundleLightItem.readyPower = BUNDLELIGHT_READYPOWER_ON;
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].readyPower == BUNDLELIGHT_READYPOWER_OFF)
						d_item->bundleLightItem.readyPower = BUNDLELIGHT_READYPOWER_OFF;
					else
						d_item->bundleLightItem.readyPower = BUNDLELIGHT_READYPOWER_NONE;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].elevatorCall == BUNDLELIGHT_ELEVATORCALL_UP_REQUEST)
						d_item->bundleLightItem.elevatorCall = BUNDLELIGHT_ELEVATORCALL_UP_REQUEST;
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].elevatorCall == BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST)
						d_item->bundleLightItem.elevatorCall = BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST;
					else
						d_item->bundleLightItem.elevatorCall = BUNDLELIGHT_ELEVATORCALL_NONE;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].out == BUNDLELIGHT_OUT_REQUEST)
						d_item->bundleLightItem.out = BUNDLELIGHT_OUT_REQUEST;
					else
						d_item->bundleLightItem.out = BUNDLELIGHT_OUT_NONE;

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[order - 1].gasClose == BUNDLELIGHT_GASCLOSE_REQUEST)
						d_item->bundleLightItem.gasClose = BUNDLELIGHT_GASCLOSE_REQUEST;
					else
						d_item->bundleLightItem.gasClose = BUNDLELIGHT_GASCLOSE_NONE;
					
				}
			}

		case BOILER:
			if(dCategory->pName == COMMAX)
			{
				if(ack == FALSE)
					d_item->boilerItem.error = PORT_ERROR;
				else if(check_device_disconnection(BOILER) == FALSE)
					d_item->boilerItem.error = BOILER_DISCONNECTION;
				else
				{
					if(order > get_current_supported_cnt(BOILER))
					{
						Log(LOG::ERR, "BOILER order %d over Error\n", order);					
						break;							
					}

					d_item->boilerItem.error = NO_ERROR;

					if(((CMX_Boiler*)dCategory)->boilerStatus[order - 1].power == BOILER_POWER_ON)
						d_item->boilerItem.power = BOILER_POWER_ON;
					else if(((CMX_Boiler*)dCategory)->boilerStatus[order - 1].power == BOILER_POWER_OFF)
						d_item->boilerItem.power = BOILER_POWER_OFF;

					d_item->boilerItem.requestTemperature = ((CMX_Boiler*)dCategory)->boilerStatus[order - 1].rtemp;	//설정온도
					d_item->boilerItem.currentTemperature = ((CMX_Boiler*)dCategory)->boilerStatus[order - 1].ctemp;	//현재온도

					if(((CMX_Boiler*)dCategory)->boilerStatus[order -1].out == BOILER_OUT_SET)
						d_item->boilerItem.outMode = BOILER_OUT_SET;
					else if(((CMX_Boiler*)dCategory)->boilerStatus[order -1].out == BOILER_OUT_RELEASE)
						d_item->boilerItem.outMode = BOILER_OUT_RELEASE;
				}
			}
	
			break;

		case CURTAIN:
			if(dCategory->pName == HAMUN)
			{
				if(ack == FALSE)
					d_item->curtainItem.error = PORT_ERROR;
				else if(check_device_disconnection(CURTAIN) == FALSE)
					d_item->curtainItem.error = CURTAIN_DISCONNECTION;
				else
				{
					if(order > get_current_supported_cnt(CURTAIN))
					{
						Log(LOG::ERR, "CURTAIN order %d over Error\n", order);					
						break;							
					}

					d_item->curtainItem.error = NO_ERROR;

					if(((HAMUN_Curtain*)dCategory)->curtainStatus[order - 1].action == CURTAIN_OPEN)
						d_item->curtainItem.action = CURTAIN_OPEN;
					else if(((HAMUN_Curtain*)dCategory)->curtainStatus[order - 1].action == CURTAIN_CLOSE)
						d_item->curtainItem.action = CURTAIN_CLOSE;
					else if(((HAMUN_Curtain*)dCategory)->curtainStatus[order - 1].action == CURTAIN_STOP)
						d_item->curtainItem.action = CURTAIN_STOP;
					else
						d_item->curtainItem.action = CURTAIN_NONE;
				}
			}
	
			break;			

		default:
			break;
	}

}


void notify_print(unsigned char cmd_mode, enum DEVICE_NAME device_name, unsigned char order, unsigned char function1, unsigned char function2, unsigned char function3, unsigned char function4)
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
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_OFF)
						{
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLON)
						{
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLOFF)
						{
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_BINARY, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
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
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_OFF)
						{
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_OFF\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLON)
						{
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_ON\n", str, order);							
						}
						else if(function3 == LIGHT_POWER_ALLOFF)
						{
							//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);
							//Log(LOG::PRTCL, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, LIGHT_POWER_ALL_OFF\n", str, order);							
						}								
						else
							Log(LOG::ERR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_POWER_EVENT, WRONG_FUNCTION\n", str, order);
					}
					else if(function2 == LIGHT_DIMMING_EVENT)
					{
						//Log(LOG::HND, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, LIGHT_DIMMING_LEVEL %d\n", str, order, function3);
						Log(LOG::MGR, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, LIGHT_DIMMING_LEVEL %d\n", str, order, function3);
						Log(LOG::INFO, "%s : LIGHT [%d], LIGHT_MODE_DIMMABLE, LIGHT_DIMMING_EVENT, LIGHT_DIMMING_LEVEL %d\n", str, order, function3);
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

		}
		break;
			
		case GAS:
		{
			switch(function1)
			{
				case GAS_CLOSE:
					{
						//Log(LOG::HND, "%s : GAS [%d], GAS_CLOSE\n", str, order);
						Log(LOG::MGR, "%s : GAS [%d], GAS_CLOSE\n", str, order);
						Log(LOG::INFO, "%s : GAS [%d], GAS_CLOSE\n", str, order);
						//Log(LOG::PRTCL, "%s : GAS [%d], GAS_CLOSE\n", str, order);						
					}
				break;

				case GAS_OPEN:
					{
						//Log(LOG::HND, "%s : GAS [%d], GAS_OPEN\n", str, order);
						Log(LOG::MGR, "%s : GAS [%d], GAS_OPEN\n", str, order);
						Log(LOG::INFO, "%s : GAS [%d], GAS_OPEN\n", str, order);
						//Log(LOG::PRTCL, "%s : GAS [%d], GAS_OPEN\n", str, order);						
					}
				break;	

				case GAS_LEAK:
					{
						//Log(LOG::HND, "%s : GAS [%d], GAS_LEAK\n", str, order);
						Log(LOG::MGR, "%s : GAS [%d], GAS_LEAK\n", str, order);
						Log(LOG::INFO, "%s : GAS [%d], GAS_LEAK\n", str, order);
						//Log(LOG::PRTCL, "%s : GAS [%d], GAS_LEAK\n", str, order);						
					}
				break;

				default:
					Log(LOG::ERR, "%s : GAS [%d], WRONG_ACTION\n", str, order);
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
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_POWER_OFF)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_OFF \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_POWER_ALLON)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_POWER_ALLOFF)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLOFF \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLOFF \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_POWER_EVENT, BUNDLELIGHT_POWER_ALLOFF \n", str, order);
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
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_OFF)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_OFF \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_ALLON)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLON \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_READYPOWER_ALLOFF)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLOFF \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLOFF \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_READYPOWER_EVENT, BUNDLELIGHT_READYPOWER_ALLOFF \n", str, order);
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
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_REQUEST_SUCCESS)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_SUCCESS \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_REQUEST_FAIL)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_REQUEST_FAIL \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_SET)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_SET \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_OUT_CANCEL)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_CANCEL \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_CANCEL \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_OUT_EVENT, BUNDLELIGHT_OUT_CANCEL \n", str, order);
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
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);						
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_GASCLOSE_REQUEST_FAIL)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_GASCLOSE_EVENT, BUNDLELIGHT_GASCLOSE_REQUEST_FAIL \n", str, order);
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
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);						
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);						
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_UP_REQUEST \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);						
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST \n", str, order);
					}
					else if(function2 == BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);						
						//Log(LOG::PRTCL, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS \n", str, order);						
					}
					else if(function2 == BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL)
					{
						//Log(LOG::HND, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL \n", str, order);
						Log(LOG::MGR, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL \n", str, order);
						Log(LOG::INFO, "%s : BUNDLELIGHT [%d], BUNDLELIGHT_ELEVATORCALL_EVENT, BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL \n", str, order);
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

		case BOILER:
		{
			switch(function1)
			{
				case BOILER_POWER_EVENT:
				{
					if(function2 == BOILER_POWER_ON)
					{
						//Log(LOG::HND, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::MGR, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::INFO, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_POWER_ON, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
					else if(function2 == BOILER_POWER_OFF)
					{
						//Log(LOG::HND, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::MGR, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::INFO, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_POWER_OFF, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
				}
				break;

				case BOILER_OUT_EVENT:
				{
					if(function2 == BOILER_OUT_SET)
					{
						//Log(LOG::HND, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::MGR, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::INFO, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_OUT_SET, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
					else if(function2 == BOILER_OUT_RELEASE)
					{
						//Log(LOG::HND, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::MGR, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						Log(LOG::INFO, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
						//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_OUT_RELEASE, RTEMP = %d, CTEMP = %d\n", str, order, function3, function4);
					}
				}
				break;	

				case BOILER_RTEMP_EVENT:
				{
					//Log(LOG::HND, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					Log(LOG::MGR, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					Log(LOG::INFO, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_RTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
				}
				break;

				case BOILER_CTEMP_EVENT:
				{
					//Log(LOG::HND, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					Log(LOG::MGR, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					Log(LOG::INFO, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
					//Log(LOG::PRTCL, "%s : BOILER [%d], BOILER_CTEMP_EVENT, RTEMP = %d, CTEMP = %d\n", str, order, function2, function3);
				}
				break;

				default:
					Log(LOG::ERR, "%s : BOILER [%d], WRONG_ACTION\n", str, order);
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
						//Log(LOG::HND, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
						Log(LOG::MGR, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
						Log(LOG::INFO, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
						//Log(LOG::PRTCL, "%s : CURTAIN [%d], CURTAIN_OPEN\n", str, order);
					}
					else if(function2 == CURTAIN_CLOSE)
					{
						//Log(LOG::HND, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
						Log(LOG::MGR, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
						Log(LOG::INFO, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
						//Log(LOG::PRTCL, "%s : CURTAIN [%d], CURTAIN_CLOSE\n", str, order);
					}
					else if(function2 == CURTAIN_STOP)
					{
						//Log(LOG::HND, "%s : CURTAIN [%d], CURTAIN_STOP\n", str, order);
						Log(LOG::MGR, "%s : CURTAIN [%d], CURTAIN_STOP\n", str, order);
						Log(LOG::INFO, "%s : CURTAIN [%d], CURTAIN_STOP\n", str, order);
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
			Log(LOG::ERR, "%s : device_name %d is WRONG DEVICE \n", str, device_name);
			break;
	}
}

void device_status_print(enum DEVICE_NAME device_name)
{
	int index, current_device_cnt;
	enum DEVICE_PROTOCOL protocol;
	DeviceProtocol *dCategory;
	protocol = get_protocol(device_name);
	dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(device_name, protocol);

	switch(device_name)
	{
		case LIGHT:
			current_device_cnt = get_current_supported_cnt(LIGHT);
			Log(LOG::INFO, "LIGHT current_device_cnt = %d\n", current_device_cnt);			

			if(protocol == COMMAX)
			{
				Log(LOG::INFO, "**************************************************************\n");
				Log(LOG::INFO, "| ID |   상태  |  타입   | Dimming레벨 | Dimming 레벨 최대치 |\n");
				for(index = 0; index < current_device_cnt; index++)
				{
					Log(LOG::INFO, "| %02d |", ((CMX_Light*)dCategory)->lightStatus[index].order);				

					if(((CMX_Light*)dCategory)->lightStatus[index].power == LIGHT_POWER_OFF)
						Log(LOG::INFO, " 전원Off |");
					else if(((CMX_Light*)dCategory)->lightStatus[index].power == LIGHT_POWER_ON)
						Log(LOG::INFO, " 전원On  |");
					else
						Log(LOG::INFO, "  %03d  |", ((CMX_Light*)dCategory)->lightStatus[index].power);

					if(((CMX_Light*)dCategory)->lightStatus[index].mode == LIGHT_MODE_BINARY)
						Log(LOG::INFO, "  BINARY |");
					else
						Log(LOG::INFO, " DIMMING |");

					Log(LOG::INFO, "      %02d     |", ((CMX_Light*)dCategory)->lightStatus[index].dimmingLevel);
					Log(LOG::INFO, "           %02d        |", ((CMX_Light*)dCategory)->lightStatus[index].maxDimmingLevel);

					Log(LOG::INFO, "\n");
				}
				Log(LOG::INFO, "**************************************************************\n");
			}
			break;

		case GAS:
			current_device_cnt = get_current_supported_cnt(GAS);
			Log(LOG::INFO, "GAS current_device_cnt = %d\n", current_device_cnt);

			if(protocol == COMMAX)
			{
				Log(LOG::INFO, "***************\n");
				Log(LOG::INFO, "| ID |  상태  |\n");
				for(index = 0; index < current_device_cnt; index++)
				{
					Log(LOG::INFO, "| %02d |", ((CMX_Gas*)dCategory)->gasStatus[index].order);				

					if(((CMX_Gas*)dCategory)->gasStatus[index].action == GAS_OPEN)
						Log(LOG::INFO, "  OPEN  |");
					else if(((CMX_Gas*)dCategory)->gasStatus[index].action == GAS_CLOSE)
						Log(LOG::INFO, " CLOSE  |");
					else if(((CMX_Gas*)dCategory)->gasStatus[index].action == GAS_LEAK)
						Log(LOG::INFO, "  LEAK  |");

					Log(LOG::INFO, "\n");
				}
				Log(LOG::INFO, "***************\n");
			}
			break;

		case BUNDLELIGHT:
			current_device_cnt = get_current_supported_cnt(BUNDLELIGHT);
			Log(LOG::INFO, "BUNDLELIGHT current_device_cnt = %d\n", current_device_cnt);

			if(protocol == COMMAX)
			{
				Log(LOG::INFO, "****************************************************************************************************\n");
				Log(LOG::INFO, "| ID | 일괄소등 상태 |   대기전력 상태   |     엘리베이터 콜    |   외출 설정  |     가스 닫기     |\n");
				for(index = 0; index < current_device_cnt; index++)
				{
					Log(LOG::INFO, "| %02d |", ((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].order);				

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].power == BUNDLELIGHT_POWER_ON)
						Log(LOG::INFO, "    POWER ON   |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].power == BUNDLELIGHT_POWER_OFF)
						Log(LOG::INFO, "   POWER OFF   |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].power == BUNDLELIGHT_POWER_ALLON)
						Log(LOG::INFO, "  POWER ALLON  |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].power == BUNDLELIGHT_POWER_ALLOFF)
						Log(LOG::INFO, "  POWER ALLOFF |");
					else
						Log(LOG::INFO, "   POWER NONE  |");
					
					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].readyPower == BUNDLELIGHT_READYPOWER_ON)
						Log(LOG::INFO, "   READYPOWER ON   |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].readyPower == BUNDLELIGHT_READYPOWER_OFF)
						Log(LOG::INFO, "   READYPOWER OFF  |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].readyPower == BUNDLELIGHT_READYPOWER_ALLON)
						Log(LOG::INFO, "   READYPOWER ALLON |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].readyPower == BUNDLELIGHT_READYPOWER_ALLOFF)
						Log(LOG::INFO, " READYPOWER ALLOFF |");
					else
						Log(LOG::INFO, "  READYPOWER NONE  |");

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].elevatorCall == BUNDLELIGHT_ELEVATORCALL_DOWN_REQUEST)
						Log(LOG::INFO, "   ELEVATORCALL DOWN  |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].elevatorCall == BUNDLELIGHT_ELEVATORCALL_UP_REQUEST)
						Log(LOG::INFO, "    ELEVATORCALL UP   |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].elevatorCall == BUNDLELIGHT_ELEVATORCALL_REQUEST_SUCCESS)
						Log(LOG::INFO, " ELEVATORCALL SUCCESS |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].elevatorCall == BUNDLELIGHT_ELEVATORCALL_REQUEST_FAIL)
						Log(LOG::INFO, "   ELEVATORCALL FAIL  |");
					else
						Log(LOG::INFO, "   ELEVATORCALL NONE  |");	

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].out == BUNDLELIGHT_OUT_REQUEST)
						Log(LOG::INFO, " OUT REQUEST |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].out == BUNDLELIGHT_OUT_REQUEST_SUCCESS)
						Log(LOG::INFO, " OUT SUCCESS |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].out == BUNDLELIGHT_OUT_REQUEST_FAIL)
						Log(LOG::INFO, "    OUT FAIL  |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].out == BUNDLELIGHT_OUT_SET)
						Log(LOG::INFO, "    OUT SET   |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].out == BUNDLELIGHT_OUT_CANCEL)
						Log(LOG::INFO, "   OUT CANCEL |");
					else
						Log(LOG::INFO, "    OUT NONE  |");	

					if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].gasClose == BUNDLELIGHT_GASCLOSE_REQUEST)
						Log(LOG::INFO, " GAS CLOSE REQUEST |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].gasClose == BUNDLELIGHT_GASCLOSE_REQUEST_SUCCESS)
						Log(LOG::INFO, " GAS CLOSE SUCCESS |");
					else if(((CMX_Bundlelight*)dCategory)->bundlelightStatus[index].gasClose == BUNDLELIGHT_GASCLOSE_REQUEST_FAIL)
						Log(LOG::INFO, "   GAS CLOSE FAIL  |");
					else
						Log(LOG::INFO, "   GAS CLOSE NONE  |");	
	

					Log(LOG::INFO, "\n");
				}
				Log(LOG::INFO, "****************************************************************************************************\n");
			}
			break;

		case BOILER:
			current_device_cnt = get_current_supported_cnt(BOILER);
			Log(LOG::INFO, "BOILER current_device_cnt = %d\n", current_device_cnt);
			
			if(protocol == COMMAX)
			{
				Log(LOG::INFO, "***********************************************\n");
				Log(LOG::INFO, "| ID | POWER | 설정온도 | 현재온도 | 외출모드 |\n");
				for(index = 0; index < current_device_cnt; index++)
				{
					Log(LOG::INFO, "| %02d |", ((CMX_Boiler*)dCategory)->boilerStatus[index].order);				

					if(((CMX_Boiler*)dCategory)->boilerStatus[index].power == BOILER_POWER_ON)
						Log(LOG::INFO, "   ON  |");
					else if(((CMX_Boiler*)dCategory)->boilerStatus[index].power == BOILER_POWER_OFF)
						Log(LOG::INFO, "  OFF  |");
					else
						Log(LOG::INFO, "  N/A  |");

					Log(LOG::INFO, "    %02d    |", ((CMX_Boiler*)dCategory)->boilerStatus[index].rtemp);
					Log(LOG::INFO, "    %02d    |", ((CMX_Boiler*)dCategory)->boilerStatus[index].ctemp);				

					if(((CMX_Boiler*)dCategory)->boilerStatus[index].out == BOILER_OUT_SET)
						Log(LOG::INFO, " 외출설정 |");
					else if(((CMX_Boiler*)dCategory)->boilerStatus[index].out == BOILER_OUT_RELEASE)
						Log(LOG::INFO, " 외출해제 |");
					else
						Log(LOG::INFO, "    N/A   |");

					Log(LOG::INFO, "\n");
				}
				Log(LOG::INFO, "***********************************************\n");
			}
			break;

		case CURTAIN:
			current_device_cnt = get_current_supported_cnt(CURTAIN);
			Log(LOG::INFO, "CURTAIN current_device_cnt = %d\n", current_device_cnt);

			if(protocol == HAMUN)
			{
				Log(LOG::INFO, "***************\n");
				Log(LOG::INFO, "| ID |  상태  |\n");
				for(index = 0; index < current_device_cnt; index++)
				{
					Log(LOG::INFO, "| %02d |", ((HAMUN_Curtain*)dCategory)->curtainStatus[index].order);				

					if(((HAMUN_Curtain*)dCategory)->curtainStatus[index].action == CURTAIN_OPEN)
						Log(LOG::INFO, "  OPEN  |");
					else if(((HAMUN_Curtain*)dCategory)->curtainStatus[index].action == CURTAIN_CLOSE)
						Log(LOG::INFO, " CLOSE  |");
					else if(((HAMUN_Curtain*)dCategory)->curtainStatus[index].action == CURTAIN_STOP)
						Log(LOG::INFO, "  STOP  |");
					else
						Log(LOG::INFO, "  NONE  |");

					Log(LOG::INFO, "\n");
				}
				Log(LOG::INFO, "***************\n");
			}
			break;			
			
		default:
			break;
	}
}


