#ifndef _LOG_H_
#define _LOG_H_

class	LOG
{
public:
	// type of log
	enum LogType
	{	
		ERR			= 0x0001,
		MGR			= 0x0002,
		UART		= 0x0004,
		PRTCL		= 0x0008,
		HND			= 0x0010,
		SERV		= 0x0020,
		INFO 		= 0x0040,
		SEP			= 0x0100
	};	
};
#endif
