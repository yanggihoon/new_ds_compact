
#include "NOKSUNG_UartRS485.h"
#include "../app.h"


static unsigned char auchCRCHi[] =

{

        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,

        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,

        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,

        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,

        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,

        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,

        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,

        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,

        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

        0x40

};

 

static char auchCRCLo[] =

{

        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,

        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,

        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,

        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,

        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,

        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,

        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,

        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,

        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,

        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,

        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,

        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,

        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,

        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,

        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,

        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,

        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,

        0x40

};
///////////////////////////////////////////// CRC 16 ERROR Table /////////////////////////////////////////////////////


static unsigned char current_command = 0x00;
NOKSUNG_UartRS485* NOKSUNG_UartRS485::_instance = 0;

NOKSUNG_UartRS485* NOKSUNG_UartRS485::Instance()
{

	if( _instance == 0) {

		_instance = new NOKSUNG_UartRS485();

	}

	return _instance;
}

void NOKSUNG_UartRS485::Instance_Close()
{	
	if( _instance != NULL )
	{
		Log(LOG::UART, "NOKSUNG_UartRS485 _instance delete\n");	
		delete _instance;
		_instance = NULL;		
	}

}

void	NOKSUNG_UartRS485::Close()
{
	if(isOpen() == TRUE)
		UartClose();

	usleep(200000);
	Instance_Close();
}

NOKSUNG_UartRS485::NOKSUNG_UartRS485()
{
	b_open = FALSE;
	b_portSet = FALSE;
	run_flag = TRUE;
	thread_start = FALSE;
}

NOKSUNG_UartRS485::~NOKSUNG_UartRS485() 
{
	//UartClose();
}

int NOKSUNG_UartRS485::UartOpen(unsigned int COMMPORT, unsigned int BAUDRATE)
{

	struct termios newtio;

	ioctl_uart_info u_info;

	if (b_open)  UartClose();	 

	switch( COMMPORT) {

		case 1:
			i_SLfd = ::open("/dev/ttyS0", O_RDWR | O_NOCTTY);
			Log(LOG::UART, "RS-485 PORT OPEN [ ttyS0 ] :: SUCCESS\n");	
		break;

		case 2:
			i_SLfd = ::open("/dev/ttyS1", O_RDWR | O_NOCTTY);
			Log(LOG::UART, "RS-485 PORT OPEN [ ttyS1 ] :: SUCCESS\n");	
		break;

		case 3:
			i_SLfd = ::open("/dev/ttyS2", O_RDWR | O_NOCTTY);
			Log(LOG::UART, "RS-485 PORT OPEN [ ttyS2 ] :: SUCCESS\n");	
		break;

		case 4:
			i_SLfd = ::open("/dev/ttyS3", O_RDWR | O_NOCTTY);
			Log(LOG::UART, "RS-485 PORT OPEN [ ttyS3 ] :: SUCCESS\n");	
		break;

		case 5:
			i_SLfd = ::open("/dev/uart0", O_RDWR | O_NDELAY);
			Log(LOG::UART, "RS-485 PORT OPEN [ /dev/uart0  ] :: SUCCESS\n");	
		break;

		case 6:
			i_SLfd = ::open("/dev/uart1", O_RDWR | O_NDELAY);
			Log(LOG::UART, "RS-485 PORT OPEN [ /dev/uart1 ] :: SUCCESS\n");	
		break;

		default:
			Log(LOG::UART, "RS-485 PORT OPEN [ ttyS0 - ttyS4 ] :: NOT FOUNDED\n");	
		break;

	}	

	if (i_SLfd < 0) {
		Log(LOG::UART, "RS-485 PORT OPEN [ COMMPROT : /dev/uart1] :: ERROR\n");	
		return FAIL;
	}
	else  
		b_open = TRUE;	

	if (!b_portSet) {
		b_portSet = TRUE;
	}

	switch( BAUDRATE ) {

		case 1: // B2400
			BAUDRATE = B2400;
			break;

		case 2:  // B4800
			BAUDRATE = B4800;
			break;

		case 3:  // B9600
			BAUDRATE = B9600;
			break;

		case 4:  // B19200
			BAUDRATE = B19200;
			break;
		
		case 5:  // B38400
			BAUDRATE = B38400;
			break;
		
		case 6:  // B57600
			BAUDRATE = B57600;
			break;

		case 7:  // B115200
			BAUDRATE = B115200;
			break;

		default:
			BAUDRATE = B4800;
			break;

	}
	
	if (!b_portSet) {
	    tcgetattr(i_SLfd,&oldtio); 
		b_portSet = true;
	}

	
	bzero(&newtio, sizeof(newtio)); 
	
	newtio.c_cflag =  BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;   //문자 사이의 timer를 disable 	
	newtio.c_cc[VMIN]  = 0;   //최소 1 문자 받을 때까진 blocking 		
	

	/*
	now clean the modem line and activate the settings for the port
	*/


	tcflush(i_SLfd, TCIFLUSH);
	tcsetattr(i_SLfd,TCSANOW,&newtio);
	

	/*
	u_info.baud = 9600;
	u_info.bits = 8;
	u_info.parity = 0; //COMM_PARITY_NONE
	u_info.stops = 1;
	*/
	
	//ioctl(i_SLfd, IOCTL_UART_SET, &u_info );

	//Log(LOG::UART, "CMX RS485 SERIAL Successfully Serial Open:\n");
	

	return SUCCESS;
}


 /** shows if the serial link is opened */
 bool NOKSUNG_UartRS485::isOpen() const
 {
	return b_open;
 }

bool NOKSUNG_UartRS485::isRunning() const
{
	return thread_start;
}

void NOKSUNG_UartRS485::isSleep(unsigned long sec)
{
	sleep(sec);
}

/** closing the serial port */
int NOKSUNG_UartRS485::UartClose()
{
	 /* restore the old port settings */  
	
	if (b_portSet)  tcsetattr(i_SLfd,TCSANOW,&oldtio);

	b_portSet = FALSE;

	if (b_open) 
		::close(i_SLfd);

	b_open = FALSE;
	run_flag = FALSE;

	Log(LOG::UART, "RS485:: NOKSUNG UART CLOSE\n");
	return SUCCESS;
}


#define WRITE 1
#define NOKSUNG_SEND_PACKET_SIZE	10
unsigned int  NOKSUNG_UartRS485::WriteFrame(unsigned char * frame, unsigned int frameLength)
{
	
	unsigned int sent = 0;
	int res = 0;
	int selectStatus = 0;
	unsigned int index = 0;
	unsigned char temp[NOKSUNG_SEND_PACKET_SIZE] = {0xff,};

	if (!b_open) {

		Log(LOG::ERR, "NOKSUNG UARTRS485 INTERFACE [ RS485 ] OPEN :: FAIL \n");	
		return FAIL;
	}
	
	Log(LOG::UART, "NOKSUNG Write FRAME : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x`%02x`%02x\n", frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7], frame[8], frame[9]);
	
	while( sent < frameLength) {	
		
		FD_ZERO(&fdsw);
		FD_SET(i_SLfd,&fdsw);
		
		selectStatus = (unsigned)select(i_SLfd+1,NULL,&fdsw,NULL, NULL);

		if(selectStatus == -1) {

			Log(LOG::ERR, "SERIAL -> NOKSUNG::  UART.C : uart_data_send function -> Error Select\n");

			return FAIL;
		}
		
		if(FD_ISSET(i_SLfd,&fdsw)) {
			current_command = frame[0];
			res = write(i_SLfd, &frame[index], WRITE);
			if (res == -1) {

				Log(LOG::ERR, "SERIAL -> NOKSUNG:: UARTSERIAL.C :: uart_data_send function : Error Writing to Serial Port\n");
				return FAIL;
			}   
			else if(res == 0 )

				Log(LOG::ERR, "SERIAL -> NOKSUNG:: UARTSERIAL.C :: uart_data_send function : write res value => 0\n");
				
			else {	
				
				sent += res;				
				index++;				
			}	
					
		}else{
			if( sent > 0 ) {		

				Log(LOG::ERR, "PACKET ROUTER -> NOKSUNG INTERFACE MEDIA [RS 485]:: SENDING TIME OUT :%d \n", sent);
				return FAIL;
			}
		}

		
	} // end of while

	return SUCCESS;
}

unsigned short NOKSUNG_UartRS485::CRC_Make(unsigned char *puchMsg, unsigned short usDataLen) 
{
	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;
	unsigned int uIndex ;

	while (usDataLen--)
	{
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}

	return (uchCRCHi << 8 | uchCRCLo);
}

int NOKSUNG_UartRS485::FrameSendToDevice(unsigned char buf[])
{
	int index =0;
	int result;
	DeviceProtocol* dCategory;

	switch(buf[0])
	{
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
			dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(SENSOR, NOKSUNG);			
			result = dCategory->FrameRecv(buf);
			break;

		default:
			index = -1;
			break;
	}

	return index;
}

// Thread of receive data
#define PACK_SIZE_ONE	1
#define NOKSUNG_PACK_SIZE 	8

void * NOKSUNG_UartRS485::run(void *arg)
{
	fd_set fdsr;	
	int recvSize = 0;	
	int selectStatus = 0;
	int result = 0;
	unsigned short crc16 = 0;

	unsigned int rcvCnt = 0;
	unsigned char readBuffer = 0x00;
	unsigned char recvBuffer[NOKSUNG_PACK_SIZE] = {0x00,};

	Log(LOG::UART, "NOKSUNG_UartRS485:: THREAD STARTING [ INTERFACE THREAD ]\n");

	
	NOKSUNG_UartRS485 * rs485 = (NOKSUNG_UartRS485*)arg;

	while(rs485->run_flag) { 

		FD_ZERO(&fdsr);
		FD_SET(rs485->i_SLfd,&fdsr);		

		selectStatus = select(rs485->i_SLfd+1, &fdsr, NULL, NULL,NULL);
		if(selectStatus == -1)
		{			
			Log(LOG::ERR, "RS485:: NOKSUNG UARTRS485.CPP :: ERROR SELECT : RETURN  SELECT : -1\n");
			//continue;
			rs485->UartClose();			
		}

		if(FD_ISSET(rs485->i_SLfd,&fdsr)) {

			recvSize = read(rs485->i_SLfd, &readBuffer, PACK_SIZE_ONE);

			Log(LOG::UART, "***** RECV FRAME : %02x *****\n",readBuffer);
			if (recvSize == -1) {				
				Log(LOG::ERR, "RS485 <- NOKSUNG PROTOCOL:: ERROR READ FROM SERIAL PORT : -1\n");
				
				rs485->UartClose();						

				break;

			} else if(recvSize == 0 ) {
				Log(LOG::ERR, "RS485 <- NOKSUNG PROTOCOL:: READ VALUE FROM SERIAL : 0\n");

			} else {						
				memcpy( (recvBuffer+rcvCnt), &readBuffer, recvSize);

				//if( (recvBuffer[0] == 0x8f) || (recvBuffer[0] == 0x82) || (recvBuffer[0] == 0x84) || (recvBuffer[0] == 0x81) || (recvBuffer[0] == 0x83) || (recvBuffer[0] == 0x85) || (recvBuffer[0] == 0x90) || (recvBuffer[0] == 0x91) || (recvBuffer[0] == 0xb0) || (recvBuffer[0] == 0xb1) || (recvBuffer[0] == 0xa0) ||(recvBuffer[0] == 0xa1) ||(recvBuffer[0] == 0xa2) || (recvBuffer[0] == 0xf2) ||(recvBuffer[0] == 0xf3) || (recvBuffer[0] == 0xf6) ||(recvBuffer[0] == 0xf7) ||(recvBuffer[0] == 0xf8)) { 
				rcvCnt += recvSize;

				if(rcvCnt >= 3)
				{
					if((recvBuffer[2] == 0x02) && (recvBuffer[3] == 0x80))
					{
						if( rcvCnt ==  NOKSUNG_SEND_PACKET_SIZE) 
						{		
							Log(LOG::UART, "### NOKSUNG RS485 <- NOKSUNG PROTOCOL:: ECHO DATA\n");
								
							memset((void*)recvBuffer, 0x00,  NOKSUNG_SEND_PACKET_SIZE);				
							rcvCnt = 0;
						}
					} 
					else 
					{
					
						if( rcvCnt == NOKSUNG_PACK_SIZE)
						{
							Log(LOG::UART, "NOKSUNG READ FRAME : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", recvBuffer[0], recvBuffer[1], recvBuffer[2], recvBuffer[3], recvBuffer[4], recvBuffer[5], recvBuffer[6], recvBuffer[7]);

							crc16 = rs485->CRC_Make(recvBuffer, NOKSUNG_PACK_SIZE - 2);
							if( ((crc16 & 0xFF00) >> 8 == recvBuffer[6]) && ((char)(crc16 & 0x00FF) == recvBuffer[7]))
							{
								result = rs485->FrameSendToDevice(recvBuffer);
							}
							else
							{
								Log(LOG::ERR, "NOKSUNG CHECKSUM FAIL : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", recvBuffer[0], recvBuffer[1], recvBuffer[2], recvBuffer[3], recvBuffer[4], recvBuffer[5], recvBuffer[6], recvBuffer[7]);
							}
						
							memset((void*)recvBuffer, 0x00, NOKSUNG_PACK_SIZE);
						
							readBuffer = 0x00;
							rcvCnt = 0;

						}
						else if( rcvCnt > NOKSUNG_PACK_SIZE) 
						{
							Log(LOG::ERR, "SERIAL <- NOKSUNG485:: RECV DATA OVERFLOW : %d", recvSize);				
							rcvCnt = 0;
							memset((void*)recvBuffer, 0x00,  NOKSUNG_PACK_SIZE);
						}
				   	}
				}
			}
		} // end of case

    	} // end of switch statement

	Log(LOG::INFO, "NOKSUNG_UartRS485:: THREAD ENDING [ INTERFACE THREAD ]\n");

	rs485->thread_start = FALSE;
	pthread_exit(NULL);
	
	return NULL;
}

void NOKSUNG_UartRS485::Start()
 {

	int ret;
	thread_start = TRUE;

	if( (ret = pthread_attr_init( &uart_thread_t) ) != 0 ) {

			Log(LOG::ERR, "NOKSUNG CAN'T  CREATE THREAD ATTRIBUTE\n");
	}
	
	if( (ret = pthread_attr_setdetachstate( &uart_thread_t, PTHREAD_CREATE_DETACHED) ) != 0 ) {
			
			Log(LOG::ERR, "NOKSUNG CAN'T SET THREAD SET DETACH\n");
	}

	if( (ret = pthread_create( &uart_thread, &uart_thread_t, NOKSUNG_UartRS485::run, (void*) this) ) != 0 )
	{
		
		
		Log(LOG::ERR, "NOKSUNG CAN'T FAIL THREAD CREATE\n");

	}


	//Log(LOG::UART, " SUCCESSFULLY THREAD CREATE \n");

	(void)pthread_attr_destroy(&uart_thread_t);		


 }
