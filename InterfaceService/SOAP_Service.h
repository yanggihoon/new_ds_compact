#ifndef SOAP_SERVICE_H_
#define SOAP_SERVICE_H_

#include "../app.h"

using namespace std;

// sub error code
#define SOAP_E_NOT_SUPPORTED		0
#define SOAP_E_OUT_OF_RANGE		1
#define SOAP_E_TEMP_UNAVAIL		2
#define SOAP_E_PARAM_MISMATCH	3
#define SOAP_E_NO_RESPONSE		4
#define SOAP_E_INTERNAL_ERROR		5
#define SOAP_E_NOT_SPECIFIED		6


// port definition
#define PORT_BASE	29700
#define DS_PORT		(PORT_BASE + 0)
#define PS_PORT		(PORT_BASE + 3)
#define LS_PORT		(PORT_BASE + 6)
#define US_PORT		(PORT_BASE + 9)
#define FS_PORT		(PORT_BASE + 12)

// client-side event port helper
// ex) EVENT_PORT(DS_PORT)
#define EVENT_PORT(x) (x + 1)

class SOAP_Service
{
private:
	static SOAP_Service* _instance;

protected:
	SOAP_Service();
	~SOAP_Service();

public:
	int soap_service_runningFlag;
	
	void Start();
	void	Close();
	static void * soap_proc(void *);	

	void Instance_Close();
	static SOAP_Service* Instance();
	
	 // Thread Atrribute
	 pthread_t soap_service_buf_thread;
	 pthread_attr_t soap_service_buf_thread_t;

	// pthread_mutex_t	m_mutex;
};

#endif






