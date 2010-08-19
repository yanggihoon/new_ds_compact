#ifndef __CMX_UART_H__
#define __CMX_UART_H__

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

// Extend Uart Start
#define WALLPAD4RD

//#define MX2ADS_ECS_BASE		CS0 //cs0 for ext cs devices
#define MX2ADS_ECS_BASE		0 //cs0 for ext cs devices

// ecs0 ~ ecs4 : gpio output port
#define ECS0	0
#define ECS1	1
#define ECS2	2
#define ECS3	3
#define ECS4	4

// ecs8 : ML7204 cs | ecs9,ecs10 : ST16C2550 cs
#define ECS8	8
#define ECS9	9
#define ECS10	10


#ifdef WALLPAD3RD // 3rd board
// ecs0 ~ ecs4 : (addr 22 : 1) (addr 23,21,20) -> 138 decoder
#define MX2ADS_ECS0_BASE			0x00400000 
#define MX2ADS_ECS1_BASE			0x00500000
#define MX2ADS_ECS2_BASE			0x00600000
#define MX2ADS_ECS3_BASE			0x00700000
#endif


#ifdef WALLPAD3RD // 3rd board
// ecs0 ~ ecs4 : (addr 23 : 1) (addr 22,21,20) -> 138 decoder
#define MX2ADS_ECS8_BASE			0x00800000
#define MX2ADS_ECS9_BASE			0x00900000
#define MX2ADS_ECS10_BASE			0x00a00000
#endif

#ifdef WALLPAD4RD//4rd board added by Seo Jong Beom
// ecs0 ~ ecs4 : (addr 22 , 23, 24 : 1, 0, 0) (addr 2, 3, 4) -> 138 decoder
#define MX2ADS_ECS0_BASE			0x00400000 
#define MX2ADS_ECS1_BASE			0x00400004
#define MX2ADS_ECS2_BASE			0x00400008
#define MX2ADS_ECS3_BASE			0x0040000C

// ecs8 ~ ecs10 : (addr 22, 23, 24 :
#define MX2ADS_ECS8_BASE			0x00800000
#define MX2ADS_ECS9_BASE			0x00C00000
#define MX2ADS_ECS10_BASE			0x01000000
#endif


#define UART0_BASE	(CS_BASE_ADDR(MX2ADS_ECS_BASE) + MX2ADS_ECS9_BASE)
#define UART1_BASE	(CS_BASE_ADDR(MX2ADS_ECS_BASE) + MX2ADS_ECS10_BASE)

#define IOCTL_UART_MAGIC    'O'  
  
typedef struct 
{ 
	unsigned long addr;
	unsigned long data;
	unsigned long baud;
	unsigned long bits;
	unsigned long parity;
	unsigned long stops;
	
} __attribute__ ((packed)) ioctl_uart_info; 
  
  
#define IOCTL_UART_RD			_IOR(IOCTL_UART_MAGIC, 0 , ioctl_uart_info )  
#define IOCTL_UART_WR			_IOW(IOCTL_UART_MAGIC, 1 , ioctl_uart_info )  
#define IOCTL_UART_SET			_IOW(IOCTL_UART_MAGIC, 2 , ioctl_uart_info )  

#define UART_IRQ 					8
//#define UART_INTR_MODE				SA_INTERRUPT|SA_SHIRQ
#define UART_INTR_MODE				SA_SHIRQ


#define UART_INT_EN		0x01

#define  COMM_RX_BUF_SIZE     512                /* Number of characters in Rx ring buffer             */
#define  COMM_TX_BUF_SIZE     512                /* Number of characters in Tx ring buffer             */

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#ifndef  NUL
#define  NUL                 0x00
#endif

#define  COMM1                  1
#define  COMM2                  2

#define  COMM_MAX_RX            1                /* NS16550A has 14 byte buffer                        */

                                                 /* ERROR CODES                                        */
#define  COMM_NO_ERR            0                /* Function call was successful                       */
#define  COMM_BAD_CH            1                /* Invalid communications port channel                */
#define  COMM_RX_EMPTY          2                /* Rx buffer is empty, no character available         */
#define  COMM_TX_FULL           3                /* Tx buffer is full, could not deposit character     */
#define  COMM_TX_EMPTY          4                /* If the Tx buffer is empty.                         */

#define  BIT0                       0x01
#define  BIT1                       0x02
#define  BIT2                       0x04
#define  BIT3                       0x08
#define  BIT4                       0x10
#define  BIT5                       0x20
#define  BIT6                       0x40
#define  BIT7                       0x80

#define  PIC_INT_REG_PORT         0x0020
#define  PIC_MSK_REG_PORT         0x0021

#define  COMM_UART_RBR                 0
#define  COMM_UART_THR                 0
#define  COMM_UART_DIV_LO              0
#define  COMM_UART_DIV_HI              1
#define  COMM_UART_IER                 1
#define  COMM_UART_FCR                 2
#define  COMM_UART_IIR                 2
#define  COMM_UART_LCR                 3
#define  COMM_UART_MCR                 4
#define  COMM_UART_LSR                 5
#define  COMM_UART_MSR                 6
#define  COMM_UART_SCR                 7

#define  COMM_PARITY_NONE       0                /* 패리티 지정 상수                                   */
#define  COMM_PARITY_ODD        1
#define  COMM_PARITY_EVEN       2

#define OS_ENTER_CRITICAL() pbuf->Flag &= ~UART_INT_EN 
#define OS_EXIT_CRITICAL() pbuf->Flag |= UART_INT_EN


typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* 무부호형 8비트 값                                  */
typedef signed   char  INT8S;                    /* 부호형 8비트 값                                    */
typedef unsigned int   INT16U;                   /* 무부호형 16비트 값                                 */
typedef signed   int   INT16S;                   /* 부호형 16비트 값                                   */
typedef unsigned long  INT32U;                   /* 무부호형 32비트 값                                 */
typedef signed   long  INT32S;                   /* 부호형 32비트 값                                   */
typedef float          FP32;                     /* 단정도 부동 소수                                   */
typedef double         FP64;                     /* 배정도 부동 소수                                   */


using namespace std;

#define SUCCESS	1
#define FAIL	0

#define CMX_PROTOCOL_LENGTH 						8

class CMX_UartRS485
{

private:
	static CMX_UartRS485* _instance;


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
		CMX_UartRS485();

		/** default destructor */
		 ~CMX_UartRS485();

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
		static CMX_UartRS485* Instance();
		
		// Thread Atrribute
		pthread_t uart_thread;

		pthread_attr_t uart_thread_t;

};

#endif

