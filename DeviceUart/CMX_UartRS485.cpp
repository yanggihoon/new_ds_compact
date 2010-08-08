
#include "CMX_UartRS485.h"
#include "../app.h"


#define CMX_CHECKSUM (recvBuffer[0] + recvBuffer[1] + recvBuffer[2] + recvBuffer[3] + recvBuffer[4] + recvBuffer[5] + recvBuffer[6])

static unsigned char current_command = 0x00;
CMX_UartRS485* CMX_UartRS485::_instance = 0;

CMX_UartRS485* CMX_UartRS485::Instance()
{

	if( _instance == 0) {

		_instance = new CMX_UartRS485();

	}

	return _instance;
}

void CMX_UartRS485::Instance_Close()
{	
	if( _instance != NULL )
	{
		delete _instance;
		Log(LOG::UART, "CMX_UartRS485 _instance delete\n");	
		_instance = NULL;		
	}

}

void	CMX_UartRS485::Close()
{
	if(isOpen() == TRUE)
		UartClose();

	usleep(200000);
	Instance_Close();
}

CMX_UartRS485::CMX_UartRS485()
{
	b_open = FALSE;
	b_portSet = FALSE;
	run_flag = TRUE;
	thread_start = FALSE;
}

CMX_UartRS485::~CMX_UartRS485() 
{
	//UartClose();
}

int CMX_UartRS485::UartOpen(unsigned int COMMPORT, unsigned int BAUDRATE)
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

/*
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
*/
	/*if (!b_portSet) {
	    tcgetattr(i_SLfd,&oldtio); 
		b_portSet = true;
	}*/

	/*
	bzero(&newtio, sizeof(newtio)); 
	
	newtio.c_cflag =  BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;   //문자 사이의 timer를 disable 	
	newtio.c_cc[VMIN]  = 0;   //최소 1 문자 받을 때까진 blocking 		
	*/

	/*
	now clean the modem line and activate the settings for the port
	*/

	/*
	tcflush(i_SLfd, TCIFLUSH);
	tcsetattr(i_SLfd,TCSANOW,&newtio);
	*/

	u_info.baud = 9600;
	u_info.bits = 8;
	u_info.parity = 0; //COMM_PARITY_NONE
	u_info.stops = 1;

	ioctl(i_SLfd, IOCTL_UART_SET, &u_info );

	//Log(LOG::UART, "CMX RS485 SERIAL Successfully Serial Open:\n");
	

	return SUCCESS;
}


 /** shows if the serial link is opened */
 bool CMX_UartRS485::isOpen() const
 {
	return b_open;
 }

bool CMX_UartRS485::isRunning() const
{
	return thread_start;
}

void CMX_UartRS485::isSleep(unsigned long sec)
{
	sleep(sec);
}

/** closing the serial port */
int CMX_UartRS485::UartClose()
{
	 /* restore the old port settings */  
	
	/*if (b_portSet)  tcsetattr(i_SLfd,TCSANOW,&oldtio);
		b_portSet = false;*/

	b_portSet = FALSE;

	if (b_open) 
		::close(i_SLfd);

	b_open = FALSE;
	run_flag = FALSE;

	Log(LOG::UART, "RS485:: UART CLOSE\n");
	return SUCCESS;
}


#define WRITE 1
#define SEND_PACKET_SIZE	8
unsigned int  CMX_UartRS485::WriteFrame(unsigned char * frame, unsigned int frameLength)
{
	
	unsigned int sent = 0;
	int res = 0;
	int selectStatus = 0;
	unsigned int index = 0;
	unsigned char temp[SEND_PACKET_SIZE] = {0xff,};

	if (!b_open) {

		Log(LOG::ERR, "UARTRS485 INTERFACE [ RS485 ] OPEN :: FAIL \n");	
		return FAIL;
	}
	
	Log(LOG::UART, "Write FRAME : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7]);
	
	while( sent < frameLength) {	
		
		FD_ZERO(&fdsw);
		FD_SET(i_SLfd,&fdsw);
		
		selectStatus = (unsigned)select(i_SLfd+1,NULL,&fdsw,NULL, NULL);

		if(selectStatus == -1) {

			Log(LOG::ERR, "SERIAL -> CMX485::  UART.C : uart_data_send function -> Error Select\n");

			return FAIL;
		}
		
		if(FD_ISSET(i_SLfd,&fdsw)) {
			current_command = frame[0];
			res = write(i_SLfd, &frame[index], WRITE);
			if (res == -1) {

				Log(LOG::ERR, "SERIAL -> CMX485:: UARTSERIAL.C :: uart_data_send function : Error Writing to Serial Port\n");
				return FAIL;
			}   
			else if(res == 0 )

				Log(LOG::ERR, "SERIAL -> CMX485:: UARTSERIAL.C :: uart_data_send function : write res value => 0\n");
				
			else {	
				
				sent += res;				
				index++;				
			}	
					
		}else{
			if( sent > 0 ) {		

				Log(LOG::ERR, "PACKET ROUTER -> INTERFACE MEDIA [RS 485]:: SENDING TIME OUT :%d \n", sent);
				return FAIL;
			}
		}

		
	} // end of while

	return SUCCESS;
}


int CMX_UartRS485::FrameSendToDevice(unsigned char buf[])
{
	int index =0;
	int result;
	DeviceProtocol* dCategory;

	switch(buf[0])
	{
		case 0x8f:
		case 0x82:
		case 0x84:
		case 0x81:
		case 0x85:
			dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BOILER, COMMAX);			
			result = dCategory->FrameRecv(buf);
			break;

		case 0x90:
		case 0x91:
			dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(GAS, COMMAX);			
			result = dCategory->FrameRecv(buf);
			break;

		case 0xA0:
		case 0xA1:
		case 0xA2:
			dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(BUNDLELIGHT, COMMAX);			
			result = dCategory->FrameRecv(buf);
			break;

		case 0xB0:
		case 0xB1:
			dCategory = (ConcreteDeviceCreator::GetInstance())->DeviceFactoryMethod(LIGHT, COMMAX);			
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
#define PACK_SIZE 	8

void * CMX_UartRS485::run(void *arg)
{
	fd_set fdsr;	
	int recvSize = 0;	
	int selectStatus = 0;
	int result = 0;

	unsigned int rcvCnt = 0;
	unsigned char readBuffer = 0x00;
	unsigned char recvBuffer[PACK_SIZE] = {0x00,};

	Log(LOG::UART, "CMX_UartRS485:: THREAD STARTING [ INTERFACE THREAD ]\n");

	
	CMX_UartRS485 * rs485 = (CMX_UartRS485*)arg;

	while(rs485->run_flag) { 

		FD_ZERO(&fdsr);
		FD_SET(rs485->i_SLfd,&fdsr);		

		selectStatus = select(rs485->i_SLfd+1, &fdsr, NULL, NULL,NULL);
		if(selectStatus == -1)
		{			
			Log(LOG::ERR, "RS485:: UARTRS485.CPP :: ERROR SELECT : RETURN  SELECT : -1\n");
			//continue;
			rs485->UartClose();			
		}

		if(FD_ISSET(rs485->i_SLfd,&fdsr)) {

			recvSize = read(rs485->i_SLfd, &readBuffer, PACK_SIZE_ONE);

			//Log(LOG::UART, "***** RECV FRAME : %02x *****\n",readBuffer);
			if (recvSize == -1) {				
				Log(LOG::ERR, "RS485 <- COMMAX PROTOCOL:: ERROR READ FROM SERIAL PORT : -1\n");
				
				rs485->UartClose();						

				break;

			} else if(recvSize == 0 ) {
				Log(LOG::ERR, "RS485 <- COMMAX PROTOCOL:: READ VALUE FROM SERIAL : 0\n");

			} else {						
				memcpy( (recvBuffer+rcvCnt), &readBuffer, recvSize);

				//if( (recvBuffer[0] == 0x8f) || (recvBuffer[0] == 0x82) || (recvBuffer[0] == 0x84) || (recvBuffer[0] == 0x81) || (recvBuffer[0] == 0x83) || (recvBuffer[0] == 0x85) || (recvBuffer[0] == 0x90) || (recvBuffer[0] == 0x91) || (recvBuffer[0] == 0xb0) || (recvBuffer[0] == 0xb1) || (recvBuffer[0] == 0xa0) ||(recvBuffer[0] == 0xa1) ||(recvBuffer[0] == 0xa2) || (recvBuffer[0] == 0xf2) ||(recvBuffer[0] == 0xf3) || (recvBuffer[0] == 0xf6) ||(recvBuffer[0] == 0xf7) ||(recvBuffer[0] == 0xf8)) { 
				if(recvBuffer[0] == current_command + 0x80) {
					rcvCnt += recvSize;

				}else {
					
					//Log(Wall::LOG_RS485, "################## SERIAL <- CMX485:: TYPE MISMATCH [ %02x, %02x]\n ################",recvBuffer[0], readBuffer );							

					rcvCnt = 0;
					readBuffer = 0x00;
					memset((void*)recvBuffer, 0x00, PACK_SIZE);	
				}

				
				if( rcvCnt == PACK_SIZE) {

					Log(LOG::UART, "READ FRAME : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", recvBuffer[0], recvBuffer[1], recvBuffer[2], recvBuffer[3], recvBuffer[4], recvBuffer[5], recvBuffer[6], recvBuffer[7]);

					if((unsigned char)CMX_CHECKSUM == recvBuffer[7])
					{
						result = rs485->FrameSendToDevice(recvBuffer);
					}
					else
					{
						Log(LOG::ERR, "CHECKSUM FAIL : %02x`%02x`%02x`%02x %02x`%02x`%02x`%02x\n", recvBuffer[0], recvBuffer[1], recvBuffer[2], recvBuffer[3], recvBuffer[4], recvBuffer[5], recvBuffer[6], recvBuffer[7]);
					}
					

					memset((void*)recvBuffer, 0x00, PACK_SIZE);
					
					readBuffer = 0x00;
					rcvCnt = 0;

				}else if( rcvCnt > PACK_SIZE) {
				
					Log(LOG::ERR, "SERIAL <- CMX485:: RECV DATA OVERFLOW : %d", recvSize);				
					rcvCnt = 0;
					memset((void*)recvBuffer, 0x00,  PACK_SIZE);
			   	}
			}
		} // end of case

    	} // end of switch statement

	Log(LOG::INFO, "CMX_UartRS485:: THREAD ENDING [ INTERFACE THREAD ]\n");

	rs485->thread_start = FALSE;
	pthread_exit(NULL);
	
	return NULL;
}

void CMX_UartRS485::Start()
 {

	int ret;
	thread_start = TRUE;

	if( (ret = pthread_attr_init( &uart_thread_t) ) != 0 ) {

			Log(LOG::ERR, " CAN'T  CREATE THREAD ATTRIBUTE\n");
	}
	
	if( (ret = pthread_attr_setdetachstate( &uart_thread_t, PTHREAD_CREATE_DETACHED) ) != 0 ) {
			
			Log(LOG::ERR, " CAN'T SET THREAD SET DETACH\n");
	}

	if( (ret = pthread_create( &uart_thread, &uart_thread_t, CMX_UartRS485::run, (void*) this) ) != 0 )
	{
		
		
		Log(LOG::ERR, " CAN'T FAIL THREAD CREATE\n");

	}


	//Log(LOG::UART, " SUCCESSFULLY THREAD CREATE \n");

	(void)pthread_attr_destroy(&uart_thread_t);		


 }
