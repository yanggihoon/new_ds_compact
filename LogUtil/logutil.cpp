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
	00 - æ¯¿Ω 
	01 - ±Ω∞‘
	04 - πÿ¡Ÿ
	05 - ±Ù∫˝∞≈∏≤
	07 - π›¿¸
	08 - º˚±Ë
	
	colors
	30 - ∞À¿∫ªˆ
	31 - ª°∞£ªˆ
	32 - ≥Ïªˆ
	33 - ≥Î∂˚ªˆ
	34 - ∆ƒ∂˚ªˆ
	35 - ¿⁄»´ªˆ
	36 - √ª∑œªˆ
	37 - «œæ·ªˆ
	40 - ∞À¡§ªˆ
	41 - ª°∞£ªˆ
	42 - ≥Ïªˆ
	43 - ≥Î∂˚ªˆ
	44 - ∆ƒ∂˚ªˆ
	45 - ¿⁄»´ªˆ
	46 - √ª≥Ïªˆ
	47 - «œæ·ªˆ
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

	// ¿Ã¿¸ ªÛ≈¬∑Œ ∫π±∏
	fprintf(stderr, "\033[0m");
}

