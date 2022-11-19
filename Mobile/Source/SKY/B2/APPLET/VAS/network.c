/*
 * SKTeletech, Inc. 2001. Confidential 
 * External Disclosure Requires NDA 
 * All intellectual property rights in this work belong to 
 * Phone.com, Inc. The information contained in this 
 * work is confidential and must not be reproduced, disclosed 
 * to others without the prior written permission of 
 * Phone.com, Inc., or used except as expressly authorized 
 * in writing by Phone.com, Inc. 
 *
 * Copyright 2001 SKTeletech, Inc.
 *
 * Sample device layer.
 *
 * File: network.c
 * (Device API) handle communications
 */
#include <string.h>
#include "idevnet.h"
//#include "iclient.h"
//#include "idev.h"
//#include "dt.h"
//#include "ires.h"

/*---------------------------------------------------------*/
#ifdef WIN32  

#include <windows.h>
#include <Winsock.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define sendudp  sendto
#define recvudp  recvfrom
#define _socket  socket

int gSOCKET = -1;

int init_socket(void)
{
	WORD    wVersion = MAKEWORD( 1, 1 );
	WSADATA wsaData;
	WSAStartup(wVersion, &wsaData);
	return 1;
}

#endif//WIN32
/*---------------------------------------------------------*/

#ifndef WIN32

#include "queue.h"
#include "dsm.h"
#include "psi.h"
#include "ska_sock.h"
#include "ska_vas.h"

/*------------------------------------------------------------------------*/
extern NETSTATE NetworkGetState(void);
/*--------------------------------------------------------------------------*/


#endif//ifndef WIN32


/*	Function: DevSendPDU
 *	Send a PDU to the network.
 *
 *	Args:
 *	bearer - bearer over which to transmit
 *	srcPort - port to be used to send data
 *	destAddress - address to send to
 *	data - data to send
 *	length of data
 *
 *	Returns: TRUE successful send, FALSE otherwise
 *
 */

#define DT_RXTEST //for devtest

#ifdef DT_RXTEST
	#define RXTEST_BYTES 1000 //2000
	extern char TempBuf[RXTEST_BYTES + 100];
	extern int  dataLen;
#endif//1

boolean DevSendPDU
(
	int bearer,              // CDMA or SMS
	unsigned short srcPort,  // DevGetPullPort() 8502 or 4445(SMS)
	const WDPAddress* destAddress, // can be changed by proxy load balancing. Reflected in ProxyTable ?
	void* data, 
	long length,
	SOCKET ss
)
{
	struct   sockaddr_in addr  ;
    unsigned short      payload_len; 
	int      status = -1;
	int      i;
	unsigned long  destIP = 0x0UL;
	unsigned short destPort = 0;

#ifdef WIN32

	if(gSOCKET == INVALID_SOCKET)
	{
		gSOCKET = _socket((byte)AF_INET, (byte)SOCK_DGRAM, (byte)IPPROTO_UDP); 
		if (gSOCKET == INVALID_SOCKET) 
		{
			DPRINTF(("Invalid Socket in DevSendPDU"));
			return FALSE;
		}
	}
#endif//WIN32
	MSG_MED("_",0,0,0);
	MSG_MED(")))))))))))))))DevSendPDU",0,0,0);
	
//	DPRINTF(("-->srcPort[%d] ??8502 bearer[%d]",srcPort, bearer));

	memset((struct sockaddr_in*)&addr, '\0', sizeof(struct sockaddr_in));
	destIP = MAKE_LONG(destAddress->bearerAddress[0],
			           destAddress->bearerAddress[1],
			           destAddress->bearerAddress[2],
			           destAddress->bearerAddress[3]);

	destPort = MAKE_HALF(destAddress->portNum[0],destAddress->portNum[1]);


	MSG_MED("destIP [%x] destPort [%d]",destIP, destPort,0);

	addr.sin_family      = AF_INET;           //0
#ifndef WIN32
	addr.sin_addr.s_addr = dss_htonl(destIP); //0xcca3a722UL
	addr.sin_port        = dss_htons(destPort);//9203 or alternative
#else
	addr.sin_addr.s_addr = htonl(destIP); //0xcca3a722UL
	addr.sin_port        = htons(destPort);//9203 or alternative

#endif//WIN32

//	DPRINTF(("Now send datagrams using sendto()"));
  /*-------------------------------------------------------------------------
    The incoming Browser packet needs to be split up into one or more large 
    items.Check the length of the packet to make sure and then take care of 
    the splitting in the while loop.
   ------------------------------------------------------------------------*/
	payload_len = MIN( length, 2000);

    /*-----------------------------------------------------------------------
       UDP packet is now ready - process it according to psi_state 
    -----------------------------------------------------------------------*/
    MSG_MED("Getting ready to send data [%d]", payload_len,0,0 );

	if( payload_len != 0)
	{
		status = sendto( ss, 
						 (const char*) data, 						 
						 payload_len, 
						 0,                         //unsuppot flag 
						 (struct sockaddr*) &addr, 
						 sizeof(struct sockaddr) );
	}
	if(status == -1)
	{
		return FALSE;
	}

	return TRUE;
}

/***************************************************************************/
#ifdef DT_RXTEST
boolean ntdtVerifyTest1PDU(unsigned char *data, long len)
{
  int		i = 0;
  boolean	isValid = FALSE;

	for(i = dataLen; i < dataLen+8; i++)
	{
		if(TempBuf[i] != (char)0xbb)
		{
		  DPRINTF(("Buf Overflow"));
		  MSG_MED("Buf Overflow",0,0,0);
		  return FALSE;
		}
	}	

  /* Verify received packet. */
  if ((TempBuf[0] == 'e') &&
      (TempBuf[1] == 'r') &&
      (TempBuf[2] == 'c') &&
      (TempBuf[3] == 'c') &&
      (TempBuf[4] == 'c') &&
      (TempBuf[5] == 'c') &&
      (TempBuf[6] == 0xaa) &&
      (len == dataLen))
    {
		for (i = 7; i < dataLen; i++)
		{
		  if (TempBuf[i] != (TempBuf[i-1] ^ 0xff))
			{
			  break;
			}
		}
		if (i == dataLen)
		{
		   isValid = TRUE;
		}
    }

  if (isValid == FALSE)
    {
      MSG_MED("Invalid received PDU! [%d]",i,0,0);
    }

  return isValid;

}	/* End of ntdtVerifyTest1PDU() */
#endif//RXTEST

/*	Function: DevRecvPDU
 *	Receive a PDU from the network.
 *	FILL IN ALL THE MEMBERS OF THE WDPAddress STRUCT OR YOUR BROWSER WILL NOT WORK.
 *
 *	Args:
 *	bearer - bearer to receive data from
 *	destPort - port as reported by NetDataArrived
 *	srcAddress - address of host sending data
 *	data - buffer for received data
 *	plength - count of received data bytes
 *
 *	Returns: TRUE receive OK, else FALSE
 *
 */

boolean DevRecvPDU
(
   int bearer,			  // CDMA or SMS
   unsigned short destPort, // DevGetPullPort() 8502 or 4445, also reported by NetDataArrived()
   WDPAddress* srcAddress,  // fromaddess 
   void* data, 
   long* plength,
   SOCKET rs
)
{
	int             length    = 0;
	unsigned short    size    = 0;
	unsigned long    srcIP    = 0x0;
	unsigned short   srcPort  = 0;
	struct sockaddr_in addr      ; 

	//You should check bearer and destPort
	MSG_MED("_",0,0,0);
	MSG_MED("DevRecvPDU((((((((((((",0,0,0);
	MSG_MED("-->destPort?8502 [%d] bearer[%d] plength[%d]",destPort,bearer,*plength);

#ifdef DT_RXTEST
	memset(TempBuf, 0, sizeof(TempBuf));
    /* Clear out any previous attempts  */
    memset(TempBuf,0xbb,dataLen+8);
#endif//1

	memset((struct sockaddr*)&addr, '\0', sizeof(struct sockaddr));


	size = sizeof(struct sockaddr);

	MSG_MED("destPort [%d]",destPort,0,0);

	{
	length = recvfrom(   rs, 
#ifdef DT_RXTEST
						 (char*) TempBuf,
#else
						 (char*) data, 
#endif//
						 (int) *plength, 
						 (int) 0,                //unsuppot flag 
						 (struct sockaddr*) &addr, 
						 (int*)&size); 
	}
	MSG_MED("Now receive datagrams [%d] \n",length,0,0);

#ifdef DT_RXTEST
	if( ntdtVerifyTest1PDU((unsigned char *)TempBuf, length) == FALSE)
	{
		MSG_ERROR("DevRecvPDU fail",0,0,0);
	}else
	{
		MSG_HIGH("DevRecvPDU success",0,0,0);
	}
#endif//rxtest
	srcAddress->addrLen    = 4;
	srcAddress->bearerTypes = CBearer_IPv4 ;
	
	srcPort = htons(addr.sin_port);

	srcAddress->portNum[0] = HI_BYTE(srcPort);
	srcAddress->portNum[1] = LO_BYTE(srcPort);

	srcIP = htonl(addr.sin_addr.s_addr);
	srcAddress->bearerAddress[0] = MUPPER_BYTE(srcIP) ;
	srcAddress->bearerAddress[1] = MLOWER_BYTE(srcIP) ;
	srcAddress->bearerAddress[2] = LUPPER_BYTE(srcIP) ;
	srcAddress->bearerAddress[3] = LLOWER_BYTE(srcIP) ;

	if( length <= 0 )
	{
	  *plength = 0;
	  return FALSE;
	}
	
	*plength = length;
	return TRUE;
}




