
#ifndef __DS_IMPL__H_
#define __DS_IMPL__H_

#include <time.h>

int setEvent(void* object);
time_t makeTime(int hour, int min, int sec);


void event_init();

void Close();

/* gSOAP String malloc */
int soapString_malloc(struct soap * pSoap, char ** ppDst, char * pSrc);


#endif //__DS_IMPL__

