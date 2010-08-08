#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "logutil.h"
#include "lognamespace.h" 

unsigned int g_logType = 0;
/*
	attribute
	00 - ���� 
	01 - ����
	04 - ����
	05 - �����Ÿ�
	07 - ����
	08 - ����
	
	colors
	30 - ������
	31 - ������
	32 - ���
	33 - �����
	34 - �Ķ���
	35 - ��ȫ��
	36 - û�ϻ�
	37 - �Ͼ��
	40 - ������
	41 - ������
	42 - ���
	43 - �����
	44 - �Ķ���
	45 - ��ȫ��
	46 - û���
	47 - �Ͼ��
*/

void Log(unsigned int nType, const char * s, ...) 
{
	char tmp1[1024];
	struct timeval tp;
	struct tm* t1;
	time_t t;

	if((g_logType & nType) != nType)
		return;

	if(nType == LOG::SEP)
	{
		fprintf(stdout, "\n");
		return;
	}


	t = time(NULL);
	t1 = localtime(&t);
	gettimeofday(&tp, NULL);

	sprintf(tmp1, "[%02d/%02d %02d:%02d:%02d:%03d] ",

	t1->tm_mon+1, t1->tm_mday,
	t1->tm_hour, t1->tm_min, t1->tm_sec, tp.tv_usec / 1000);
	
	va_list args;

	va_start(args, s);

	switch(nType)
	{
		case LOG::ERR:  
			fprintf(stderr, "\033[01;31m"); 
			fprintf(stderr, "%s ", tmp1);
			fprintf(stderr, "[ERROR]: "); 

			vfprintf(stderr, s, args);
			va_end(args);

		break;
		
		case LOG::MGR: 
			fprintf(stdout, "\033[01;32m"); 
			fprintf(stdout, "%s ", tmp1);
			fprintf(stdout, "[MGR]: "); 

			vfprintf(stdout, s, args);
			va_end(args);

		break;

		case LOG::UART: 
			fprintf(stdout, "\033[01;37m"); 
			fprintf(stdout, "%s ", tmp1);
			fprintf(stdout, "[UART]: "); 

			vfprintf(stdout, s, args);
			va_end(args);

		break;

		case LOG::PRTCL: 
			fprintf(stdout, "\033[01;36m"); 
			fprintf(stdout, "%s ", tmp1);
			fprintf(stdout, "[PRTCL]: "); 

			vfprintf(stdout, s, args);
			va_end(args);

		break;

		case LOG::HND: 
			fprintf(stdout, "\033[01;34m"); 
			fprintf(stdout, "%s ", tmp1);
			fprintf(stdout, "[HND]: "); 

			vfprintf(stdout, s, args);
			va_end(args);

		break;


		case LOG::SERV: 
			fprintf(stdout, "\033[01;33m"); 
			fprintf(stdout, "%s ", tmp1);
			fprintf(stdout, "[SERV]: "); 

			vfprintf(stdout, s, args);
			va_end(args);

		break;

		case LOG::INFO: 
			fprintf(stdout, "\033[01;36m"); 
			//fprintf(stdout, "%s ", tmp1);
			//fprintf(stdout, "[INFO]: "); 

			vfprintf(stdout, s, args);
			va_end(args);

		break;

	}

	// ���� ���·� ����
	fprintf(stderr, "\033[0m");
}

