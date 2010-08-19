#ifndef __HAMUN_UART_H__
#define __HAMUN_UART_H__

#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <asm/mman.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <termios.h>


using namespace std;

#define SUCCESS	1
#define FAIL	0

#define HAMUN_PROTOCOL_LENGTH 						7

class HAMUN_UartRS485
{

private:
	static HAMUN_UartRS485* _instance;


	private: // Private attributes
		/** File Descriptor for the serial port */
		int i_SLfd;

		/** flag if the serial link is opened */
		bool b_open;

		/** stores old port settings */
		//struct termios oldtio;

		/** if port settings are changed and old port settings are stored. */
		bool b_portSet;

		/** stop running thread */
		bool run_flag;
		bool thread_start;
		
		fd_set fdsw;

		  struct termios oldtio;

	protected:
		/** default constructor */
		HAMUN_UartRS485();

		/** default destructor */
		 ~HAMUN_UartRS485();


	public:		  	 
		/** opening the serial port */  
		int UartOpen(unsigned int COMMPORT, unsigned int BAUDRATE);

		/** shows if the serial link is opened */
		bool isOpen() const;
		bool isRunning() const;

		/** closing the serial port */
		int UartClose();				  

		void isSleep(unsigned long sec);
		unsigned int  WriteFrame(unsigned char *, unsigned int);

		/* Find a device where send to */
		int FrameSendToDevice(unsigned char buf[]);

		// Only C Thread
		void Start();
		void	Close();
		static void * run(void *);	

		void Instance_Close();
		static HAMUN_UartRS485* Instance();
		
		// Thread Atrribute
		pthread_t uart_thread;

		pthread_attr_t uart_thread_t;

};

#endif

