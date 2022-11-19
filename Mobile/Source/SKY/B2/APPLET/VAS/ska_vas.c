/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_vas.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-06-12 jmpark     Created.
**  
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include <stdlib.h>
#include "ska.h"
#include "uisockapi.h"
#include "dsm.h"
#include "ska_vas.h"
#include "sky_sock.h"
#include "msg.h"
#include "err.h"
#include "idevnet.h"

#include "psmisc.h"

#ifdef FEATURE_VAS
/* 
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
//#define VAS_TEST

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
enum {
	max_udp_num = 1,
	max_tcp_num = 2,
	max_socket_num = max_tcp_num + max_udp_num
};
/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
VASSTATE_T gVasState;
SOCKET fd[3] = { -1, -1, -1 };
sint15 gFD = -1 ;

BOOL test_flag1 = FALSE;
BOOL test_flag2 = FALSE;

LOCAL unsigned long destinationIP = 0x0UL;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
#define RXTEST_BYTES 1000
char TempBuf[RXTEST_BYTES + 100] ;
int  dataLen = RXTEST_BYTES;

static const char *HOST_NAME[] = {"sktelecom.com", "netsgo.com", "hitel.com" };

#define HOST_PORT 25 //SMTP SERVER PORT

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
#define GET_FD		( gFD )
#define SET_FD(s)   ( gFD = s )
#define MAX_SOCKET  ( max_socket_num )


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


boolean DevRecvTCP
(
   int bearer,			  // CDMA or SMS
   unsigned short destPort, // DevGetPullPort() 8502 or 4445, also reported by NetDataArrived()
   WDPAddress* srcAddress,  // fromaddess 
   void* data, 
   long* plength,
   SOCKET s
)
{
	int             length    = 0;
	unsigned short    size    = 0;
	struct sockaddr_in addr      ; 

	//You should check bearer and destPort
	MSG_MED("_",0,0,0);
	MSG_MED("DevRecvTCP((((((((((((",0,0,0);
	MSG_MED("destPort [%d] socket[%d]",destPort,s,0);

	memset((struct sockaddr*)&addr, '\0', sizeof(struct sockaddr));

	size = sizeof(struct sockaddr);

	length = recv(   s , 
					 (char*) data, 
					 (int) *plength, 
					 (int) 0);                //unsuppot flag  

	MSG_MED("Now receive datagrams [%d][%d] \n",s, length,0);

	if( length <= 0 )
	{
	  *plength = 0;
	  return FALSE;
	}
	
	*plength = length;
	return TRUE;
}

void NetDataArrived(signed int bearer, unsigned short destPort, SOCKET ns)
{
  long			TempBufLen;
  boolean		result;
  WDPAddress    dataArrivedAddr;

  /* Call DevRecvPDU to pick up the packet. */
//  recvMsgErrorType = DevRecvPDU(0, destPort, &dataArrivedAddr, TempBuf, &dataLen);
  MSG_ERROR("NetDataArrived [%d]",ns,0,0);

  TempBufLen = RXTEST_BYTES ;
  memset(TempBuf,0,sizeof(TempBuf));
  result = DevRecvTCP(0, destPort, &dataArrivedAddr, TempBuf, &TempBufLen, ns);  
  if(result == TRUE)
  {
		MSG_HIGH("NetDataArrived Success",0,0,0);
		EnableReadEvent(ns);
		{	static char tmp_buf[20];
			memcpy(tmp_buf,TempBuf,20);
			MSG_ERROR(tmp_buf,0,0,0);
		}
  }
  else
  {
		MSG_ERROR("NetDataArrived Fail",0,0,0);
		DisableReadEvent(ns);
		send_ui_cmd(UI_FAIL_F);
  }
}


void InitSocket(void)
{
	int i;

	for( i = 0 ; i < MAX_SOCKET; i++) 
	{
		fd[i] = DSS_ERROR; 
	}
}

void VAS_SocketStateChanged(void)
{
	int event;
	int i;

	MSG_MED("VAS_SocketStateChanged",0,0,0);

//  Should be changed for Application to call CheckSocketState fn.
	for(i = 0; i < MAX_SOCKET; i++)
	{
		if(fd[i] == DSS_ERROR)
			continue;
		event = CheckSocketState(fd[i]);
		if (event == 1)
		{
			//send_ui_cmd(VAS_RXDATA_CMD);
			NetDataArrived(0, 1904, fd[i]);
		}
		else if (event == 3)
		{
			MSG_MED("Socket closed",0,0,0)	;
			send_ui_cmd(VAS_RELEASE_CMD);
		}else
		{
			MSG_ERROR("No event",0,0,0);
		}

	}
}



///////////////////////////////////////////////////////////
void vas_dnsquerry_cmd(void)
{
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;

	MSG_HIGH("vas_dnsquerry_cmd",0,0,0);

	hostentry = gethostbyname("www.yahoo.com");   
 
	if(hostentry == NULL)
	{
		MSG_ERROR("DNS query fail",0,0,0);
	}else
	{
//		destinationIP = get32((byte *)(hostentry->h_addr_list[0]));        
//		MSG_HIGH("IP address [%x]", destinationIP, 0, 0);

//		memcpy( &addr.sin_addr, hostentry->h_addr_list[0], sizeof(struct in_addr) );
		addr.sin_addr.s_addr = get32( (byte *)(hostentry->h_addr_list[0]) );

		MSG_HIGH("REMOTE ADDRESS [0x%x%x]", (int)((addr.sin_addr.s_addr)>>16), 
									    (int)addr.sin_addr.s_addr, 0);
	}

}

void wap_key_cmd(signed char key_value)
{
    	MSG_ERROR("key value[%d]",key_value,0,0);
        SKY_TRACE(TL_HIGH, "key value");

}

void wap_gotourl_cmd(unsigned char *gotourl)
{
	unsigned char tmp_url[100];

	MSG_ERROR("wap_gotourl_cmd",0,0,0);
	memset(tmp_url, 0, sizeof(tmp_url));
	strcpy(tmp_url, gotourl);
}


void vas_pppopen_test_cmd(void)
{
	sint15 result = DSS_ERROR ;

	MSG_ERROR("vas_pppopen_test_cmd",0,0,0);

    SKY_SET_IN_WAP(TRUE); //ui.wap = TRUE;
    SetKindOfService(VAS); //for socket

	result = PPPOPEN();
	if(result == DSS_ERROR)
	{
		MSG_ERROR("PPP OPEN fail",0,0,0);
		send_ui_cmd(UI_FAIL_F);
		return;
	}

	MSG_ERROR(">>>>>> pppopen 완료",0,0,0);
}

void vas_pppclose_test_cmd(void)
{
}

void vas_release_cmd(void)
{
	sint15 result = DSS_ERROR ;
	int i;

	MSG_ERROR("vas_release_cmd",0,0,0);

	for( i = 0; i < MAX_SOCKET ; i++ )
	{
		if(fd[i] == DSS_ERROR)
			continue;
		result = closesocket(fd[i]);
		fd[i] = DSS_ERROR;
		if(result == DSS_ERROR)
		{
			MSG_ERROR("close socket fail",0,0,0);
			//return; //Do not use
		}
	}
	InitSocket();

	result = PPPCLOSE();
	if(result == DSS_ERROR)
	{
		MSG_HIGH("PPP CLOSE fail",0,0,0);
		send_ui_cmd(UI_FAIL_F);
		return;
	}

	MSG_ERROR(">>>>>> pppclose 완료",0,0,0);
}

void vas_udptxrx_cmd(void)
{
	sint15 result = DSS_ERROR ;

	WDPAddress DestAddress;
	int      i;
	unsigned long  destIP = 0x0UL;
	unsigned short destPort = 0;
	int      length;

	MSG_HIGH("vas_txrx_udp_cmd",0,0,0);

	InitSocket();
	SetKindOfService(VAS);
/*
	result = PPPOPEN();
	if(result == DSS_ERROR)
	{
		MSG_ERROR("PPP OPEN fail",0,0,0);
		send_ui_cmd(UI_FAIL_F);
		return;
	}
*/
	fd[0] = socket(     (byte)AF_INET, 
		                (byte)SOCK_DGRAM, 
						(byte)IPPROTO_UDP) ;
	if( fd[0] == DSS_ERROR ) 
	{
		MSG_ERROR("fail to open socket [%x]", fd[0], 0, 0);		
		send_ui_cmd(UI_FAIL_F);
		return;
	}	
	MSG_HIGH(">>>>>> pppopen 완료",0,0,0);

  memset(TempBuf, 0, sizeof(TempBuf));
  /* Generate the packet header. */
  TempBuf[0] = 'e';
  TempBuf[1] = 'r';

  /*
   * Use the following algorithm to generate the body:
   *    TempBuf[base] = 0xaa
   *    TempBuf[n] = xor(value[n-1], 0xff)
   */
  TempBuf[2] = 'c';
  TempBuf[3] = 'c';
  TempBuf[4] = 'c';
  TempBuf[5] = 'c'; /* We want to tell dtserve that 
			we are sending to port 1904 */
  TempBuf[6] = 0xaa;  /* seed value */

  /* We are going to test two PDU sizes. One an arbitrary length and one for
     the maximum size of 1992 bytes
  */
    for (i = 7; i < dataLen; i++)
    {
      TempBuf[i] = (TempBuf[i-1] ^ 0xff);
    }
   /*
     * Send the packet.
     */
	length   = dataLen;

	destPort = 1904;
	destIP   = PROXYADDR_DAGOBAH ;

//
	memset((struct sockaddr_in*)&DestAddress, '\0', sizeof(struct sockaddr_in));

	DestAddress.addrLen    = 4;
	DestAddress.bearerTypes = CBearer_IPv4 ;

	DestAddress.portNum[0] = HI_BYTE(destPort);
	DestAddress.portNum[1] = LO_BYTE(destPort);

	DestAddress.bearerAddress[0] = MUPPER_BYTE(destIP) ;
	DestAddress.bearerAddress[1] = MLOWER_BYTE(destIP) ;
	DestAddress.bearerAddress[2] = LUPPER_BYTE(destIP) ;
	DestAddress.bearerAddress[3] = LLOWER_BYTE(destIP) ;

//
	DevSendPDU(0, 0, &DestAddress, TempBuf, length, 0);

}

boolean sendflag = FALSE;

void vas_tcptxrx_cmd(void)
{
	int      i;
	sint15 result = DSS_ERROR ;
	int      length;
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;
	char     *hostname = NULL;

    unsigned short      payload_len; 


	MSG_ERROR("vas_tcptxrx_cmd",0,0,0);


	InitSocket();
	SetKindOfService(VAS);

/*
	result = PPPOPEN();
	if(result == DSS_ERROR)
	{
		MSG_ERROR("PPP OPEN fail",0,0,0);
		PPPCLOSE(); //to deregister callback function
		send_ui_cmd(UI_FAIL_F);
		return;
	}
*/
	MSG_ERROR(">>>>>> pppopen 완료",0,0,0);

	fd[0] = socket(
		                (byte)AF_INET, 
		                (byte)SOCK_STREAM, 
						(byte)IPPROTO_TCP) ;
	if( fd[0] == DSS_ERROR ) 
	{
		MSG_ERROR("fail to open socket [%x]", fd[0], 0, 0);	
		PPPCLOSE();
		send_ui_cmd(UI_FAIL_F);
		return;
	}	

	hostname = HOST_NAME[0];
	
	hostentry = gethostbyname(hostname);
	if(hostentry == NULL)
	{
			MSG_ERROR("gethostbyname error",0,0,0);
			result = closesocket(fd[0]);
			fd[0] = DSS_ERROR;
			result = PPPCLOSE();
			send_ui_cmd(UI_FAIL_F);
			return;
	}
	destinationIP = get32((byte *)(hostentry->h_addr_list[0]));     
	
	memset((struct sockaddr_in*)&addr, '\0', sizeof(struct sockaddr_in));

	addr.sin_family      = AF_INET;                   //0
	addr.sin_addr.s_addr = dss_htonl(destinationIP);  //0xcbec2b5d or 0x460cca60
	addr.sin_port        = dss_htons(HOST_PORT);

	/* Connect */
	if(connect(fd[0], &addr, sizeof(struct sockaddr)) == DSS_ERROR)
	{
		MSG_ERROR("TCP connection fail",0,0,0);
		closesocket(fd[0]);
		fd[0] =DSS_ERROR;
		PPPCLOSE();
		send_ui_cmd(UI_FAIL_F);
		return;
	}
	MSG_ERROR("TCP connection success",0,0,0);

	memset(TempBuf, 0, sizeof(TempBuf));
	//sprintf(TempBuf,"GET http://%s/ HTTP/1.0\r\n\r\n", "sktelecom.com");
	sprintf(TempBuf, "HELO %s\r\n", HOST_NAME[0]);
	length = strlen(TempBuf);

  /*-------------------------------------------------------------------------
    The incoming Browser packet needs to be split up into one or more large 
    items.Check the length of the packet to make sure and then take care of 
    the splitting in the while loop.
   ------------------------------------------------------------------------*/
	payload_len = MIN( length, 2000);

    /*-----------------------------------------------------------------------
       TCP packet is now ready - process it according to psi_state 
    -----------------------------------------------------------------------*/
    MSG_MED("Getting ready to send data [%d]", payload_len,0,0 );

	if( payload_len != 0)
	{
/*
		int send 
(
  SOCKET s,              
  const char * buf,  
  int len,               
  int flags              
)
*/
		result = send(    fd[0], 
						 (const char*) TempBuf, 						 
						 payload_len, 
						 0);

	}
	if(result == DSS_ERROR)
	{
		MSG_ERROR("DevSendTCP fail",0,0,0);
		closesocket(fd[0]);
		fd[0] = DSS_ERROR;
		PPPCLOSE();
		send_ui_cmd(UI_FAIL_F);
		return ;
	}
	MSG_ERROR(">>>>>> DevSendTCP 완료",0,0,0);
}

void vas_reorigination_cmd(void)
{
	sint15 result = DSS_ERROR ;
	extern sint15 Dormant_Reorigination(sint15 *errno);
	short    err;
	
	result = Dormant_Reorigination(&err);
	if(result == DSS_ERROR && err == DS_EWOULDBLOCK)
	{
		MSG_HIGH("reorigination progressing",0,0,0);
	}else if(result == DSS_SUCCESS)
	{
		MSG_HIGH("reorigination done",0,0,0);
	}else
	{
		MSG_ERROR("Dormant Reorigination fail",0,0,0);
	}
}

//
void vas_dormant2active_cmd(void)
{
	sint15 result = DSS_ERROR ;
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;
	char     *hostname = NULL;

		MSG_HIGH("vas_dormant2active_cmd",0,0,0);

		{
			{
				fd[0] = socket(
									(byte)AF_INET, 
									(byte)SOCK_STREAM, 
									(byte)IPPROTO_TCP) ;
				if( fd[0] == DSS_ERROR ) 
				{
					MSG_ERROR("fail to open socket [%x]", fd[0], 0, 0);	
					PPPCLOSE();
					send_ui_cmd(UI_FAIL_F);
					return;
				}	
				
			}

			memset((struct sockaddr_in*)&addr, '\0', sizeof(struct sockaddr_in));

			addr.sin_family      = AF_INET;                   //0
			addr.sin_addr.s_addr = dss_htonl(destinationIP);  //0xcbec2b5d or 0x460cca60
			addr.sin_port        = dss_htons(HOST_PORT);
		}

		/* Connect */
		if(connect(fd[0], &addr, sizeof(struct sockaddr)) == DSS_ERROR)
		{
			MSG_ERROR("TCP connection fail",0,0,0);
//			if(connect(fd[0], &addr, sizeof(struct sockaddr)) == DSS_ERROR)
			{
				closesocket(fd[0]);
				fd[0] = DSS_ERROR;
				PPPCLOSE();
				send_ui_cmd(UI_FAIL_F);
				return;
			}
		}
		MSG_ERROR("TCP connection success",0,0,0);

////////////////////////////////////////////////////////////////////
}


void vas_txsend_cmd(void)
{
	int length;
    unsigned short  payload_len; 
	sint15      result = DSS_ERROR;

	MSG_ERROR("vas_txsend_cmd",0,0,0);

	memset(TempBuf, 0, sizeof(TempBuf));
//	sprintf(TempBuf,"GET http://%s/ HTTP/1.0\r\n\r\n", "sktelecom.com");
	sprintf(TempBuf, "HELO %s\r\n", HOST_NAME[0]);
	length = strlen(TempBuf);

  /*-------------------------------------------------------------------------
    The incoming Browser packet needs to be split up into one or more large 
    items.Check the length of the packet to make sure and then take care of 
    the splitting in the while loop.
   ------------------------------------------------------------------------*/
	payload_len = MIN( length, 2000);

    /*-----------------------------------------------------------------------
       TCP packet is now ready - process it according to psi_state 
    -----------------------------------------------------------------------*/
    MSG_MED("Getting ready to send data [%d]", payload_len,0,0 );

	if( payload_len != 0)
	{
		result = send(    fd[0], 
						 (const char*) TempBuf, 						 
						 payload_len, 
						 0);
		sendflag = TRUE;
	}
	if(result == DSS_ERROR)
	{
		MSG_ERROR("DevSendTCP fail",0,0,0);
/*		closesocket(fd[0]);
		PPPCLOSE();
		send_ui_cmd(UI_FAIL_F);
*/
		return ;
	}
	MSG_ERROR(">>>>>> DevSendTCP 완료",0,0,0);
}


void vas_rxdata_cmd(void)
{
	MSG_HIGH("vas_rx_cmd",0,0,0);

	NetDataArrived(0,0,0);
	MSG_HIGH("vas_rxdata_cmd 완료",0,0,0);
}

void vas_multisocket_cmd(void)
{
	int      i;
	sint15 result = DSS_ERROR ;
	int      length;
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;

    unsigned short      payload_len; 


	MSG_HIGH("vas_tcptxrx_cmd",0,0,0);

	InitSocket();
	SetKindOfService(VAS);
/*
	result = PPPOPEN();
	if(result == DSS_ERROR)
	{
		MSG_ERROR("PPP OPEN fail",0,0,0);
		send_ui_cmd(UI_FAIL_F);
		return;
	}
	send_ui_cmd(UI_SUCCESS_F);
	MSG_HIGH(">>>>>> pppopen 완료",0,0,0);
*/
	for(i = 0; i < MAX_SOCKET; i++)
	{
		fd[i] = socket( 
							(byte)AF_INET, 
							(byte)SOCK_STREAM, 
							(byte)IPPROTO_TCP) ;
		if( fd[i] == DSS_ERROR ) 
		{
			MSG_ERROR("fail to open socket [%x]", fd[i], 0, 0);		
			send_ui_cmd(UI_FAIL_F);
			return;
		}	
	}
	send_ui_cmd(UI_SUCCESS_F);
/*
	for(; i < MAX_UDPSOCKET; i++)
	{
		fd[i] = socket( 
							(byte)AF_INET, 
							(byte)SOCK_DGRAM, 
							(byte)IPPROTO_UDP) ;
		if( fd[i] == DSS_ERROR ) 
		{
			MSG_ERROR("fail to open socket [%x]", fd[i], 0, 0);		
			//send_ui_cmd(UI_FAIL_F);
			return;
		}	
	}
*/
	//////////////////////////////////////////////////////////////////////////
	// Start Connection except UDP socket
	//MSG_ERROR("Start TCP connection success",0,0,0);
	for(i = 0; i < MAX_SOCKET; i++)
	{

	hostentry = gethostbyname(HOST_NAME[i]);
	if(hostentry == NULL)
	{
		MSG_ERROR("gethostbyname error",0,0,0);
		send_ui_cmd(UI_FAIL_F);
		return;
	}
	memset((char *)&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family      = AF_INET;           //0
	addr.sin_port        = dss_htons(HOST_PORT);//9203 or alternative
#if 1
	addr.sin_addr.s_addr = get32( (byte *)(hostentry->h_addr_list[0]) );

	MSG_HIGH("REMOTE ADDRESS [0x%x%x]", (int)((addr.sin_addr.s_addr)>>16), 
									    (int)addr.sin_addr.s_addr, 0);
	addr.sin_addr.s_addr = dss_htonl(addr.sin_addr.s_addr); 
#else
//	addr.sin_addr.s_addr = dss_htonl(destIP); //0xcca3a722UL
	addr.sin_addr.s_addr = dss_htonl(0xd27470c8UL); //0xcca3a722UL
#endif

	if(connect( fd[i], &addr, sizeof(struct sockaddr)) == DSS_ERROR)
	{
		MSG_ERROR("TCP connection fail[%d]",fd[i],0,0);
		send_ui_cmd(UI_FAIL_F);
		return;
	}
	send_ui_cmd(UI_SUCCESS_F);
	}//for
	// End of Connection
	//////////////////////////////////////////////////////////////////////////

	///Start Data Send
	for(i = 0; i < MAX_SOCKET; i++)
	{
		memset(TempBuf, 0, sizeof(TempBuf));
		//sprintf(TempBuf,"GET http://%s/ HTTP/1.0\r\n\r\n", "sktelecom.com");
		sprintf(TempBuf, "HELO %s\r\n", HOST_NAME[i]);
		length = strlen(TempBuf);

	  /*-------------------------------------------------------------------------
		The incoming Browser packet needs to be split up into one or more large 
		items.Check the length of the packet to make sure and then take care of 
		the splitting in the while loop.
	   ------------------------------------------------------------------------*/
		payload_len = MIN( length, 2000);

		/*-----------------------------------------------------------------------
		   TCP packet is now ready - process it according to psi_state 
		-----------------------------------------------------------------------*/
		MSG_MED("Getting ready to send data [%d]", payload_len,0,0 );

		if( payload_len != 0)
		{
			result = send(    fd[i], 
							 (const char*) TempBuf, 						 
							 payload_len, 
							 0);

		}
		if(result == DSS_ERROR)
		{
			MSG_ERROR("DevSendTCP fail",0,0,0);
			send_ui_cmd(UI_FAIL_F);
			return ;
		}
	}//for
	///End Data Send
	//////////////////////////////////////////////////////////////////////
	MSG_HIGH(">>>>>> DevSendTCP 완료",0,0,0);

}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainWAP()
**
** Description: 
**     idle상태에서 WAP key event시 처음invoke되는 함수
**     1. wap에서 쓰는 변수들 초기화
**     2. n.Top 초기화면/ 접속바 진행
**     3. DS로 origination cmd / ppp setup cmd
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

VOID SKY_API SKA_MainWAP0(SKY_EVENT_T *pEvent)
{	
	extern void SKY_API test_cmd(void);

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(TRUE);
		EQS_GetDC(TRUE);
		SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());	
		
		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators. 
            WA_SHOW_HEADER);
	    break;

	case SE_KEY_1:
		MSG_HIGH("SE_KEY_1 pressed",0,0,0);
		send_ui_cmd(VAS_PPPOPEN_TEST_CMD);
		break;
	case SE_KEY_2:
		MSG_HIGH("SE_KEY_2 pressed",0,0,0);
		send_ui_cmd(VAS_RELEASE_CMD);
		break;

	case SE_KEY_3:
		MSG_HIGH("SE_KEY_3 pressed",0,0,0);
		send_ui_cmd(VAS_UDPTXRX_CMD);
		break;

	case SE_KEY_4:
		MSG_HIGH("SE_KEY_4 pressed",0,0,0);
		send_ui_cmd(VAS_TCPTXRX_CMD);
		break;

	case SE_KEY_5:
		MSG_HIGH("SE_KEY_5 pressed",0,0,0);
		send_ui_cmd(VAS_TXSEND_CMD);
		break;
	case SE_KEY_6:
		MSG_HIGH("SE_KEY_6 pressed",0,0,0);
		vas_dnsquerry_cmd();
		break;
	case SE_KEY_7:
		MSG_HIGH("SE_KEY_7 pressed",0,0,0);
		send_ui_cmd(VAS_MULTISOCKET_CMD);
		break;
	case SE_KEY_8:
		MSG_HIGH("SE_KEY_8 pressed",0,0,0);
		vas_reorigination_cmd();
		break;
	case SE_KEY_9:
		if(test_flag1 == 1)
			test_flag1 = 0;
		else if(test_flag1 == 0)
			test_flag1 = 1;
		MSG_HIGH("SE_KEY_9 pressed test_flag1[%d]",test_flag1,0,0);
		break;
	case SE_KEY_0:
		if(test_flag2 == 1)
			test_flag2 = 0;
		else if(test_flag2 == 0)
			test_flag2 = 1;
		MSG_ERROR("SE_KEY_0 pressed test_flag2[%d]",test_flag2,0,0);
		break;		
	
	case SE_KEY_END ://HS_END_K:
		MSG_ERROR("SE_KEY_END",0,0,0);
		send_ui_cmd(VAS_RELEASE_CMD);
		break;

	case SE_NET_CONNECTED :
		MSG_ERROR("----> SE_NET_CONNECTED",0,0,0);
		break;

	case SE_NET_DISCONNECTED :
		MSG_ERROR("----> SE_NET_DISCONNECTED",0,0,0);
		break;

	case SE_APP_DRAW:
		break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;

    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_END :   /* The very last event */
		SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());		
		EQS_ReleaseDC();
		break;

	case SE_SUCCESS_F:
		MSG_HIGH("SE_SUCCESS_F",0,0,0);
		break;

	case SE_FAIL_F:
		MSG_HIGH("SE_FAIL_F",0,0,0);
		break;

	case SE_CALL_CONNECT:
		MSG_HIGH("SE_CALL_CONNECT",0,0,0);
		break;

	case SE_CALL_ENDED:
		MSG_HIGH("SE_CALL_ENDED",0,0,0);

		closesocket(fd[0]);
		fd[0] = DSS_ERROR;

		break;
	}
}


void SKY_API test_cmd(void)
{
}



#ifdef VAS_TEST

VOID SKY_API SKA_TEST1(SKY_EVENT_T* pEvent)
{
		
    switch(pEvent->EventID)
    {
    case SE_APP_START :
		if(test_flag1)
		{
			EQC_MessageBox(SKY_GET_TOKEN(TKN_NTOP_CHANGE_FLAG_F), 2, MBT_NONE, MBT_SUCCESS);//(BYTE*)"");
			test_flag1 = FALSE;
		}
		else
		{
			EQC_MessageBox(SKY_GET_TOKEN(TKN_NTOP_CHANGE_FLAG_T), 2, MBT_NONE, MBT_SUCCESS);//(BYTE*)"");
			test_flag1 = TRUE;
		}

		
      break;

    case SE_APP_END :     /* The very last event */
		break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */		
        break; 
 
	case SE_IO_MESGBOX_END:        
            EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }	
}

VOID SKY_API SKA_TEST2(SKY_EVENT_T* pEvent)
{
		
    switch(pEvent->EventID)
    {
    case SE_APP_START :
		if(test_flag2)
		{
			EQC_MessageBox(SKY_GET_TOKEN(TKN_NTOP_CHANGE_FLAG_F), 2, MBT_NONE, MBT_SUCCESS);//(BYTE*)"");
			test_flag2 = FALSE;
		}
		else
		{
			EQC_MessageBox(SKY_GET_TOKEN(TKN_NTOP_CHANGE_FLAG_T), 2, MBT_NONE, MBT_SUCCESS);//(BYTE*)"");
			test_flag2 = TRUE;
		}		
      break;

    case SE_APP_END :     /* The very last event */
		break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */		
        break; 
 
	case SE_IO_MESGBOX_END:
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }	
}
#endif //VAS_TEST

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

#endif