/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     sky_sock.c
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
#include "dsm.h"
#include "dsi.h"
#include "dssocket.h"
#include "dsatcop.h"
#include "sky_ui.h"
#include "sky_sock.h"

#include "msg.h"
#include "err.h"

#ifdef FEATURE_VAS

extern ip4a primaryIP;
extern ip4a secondaryIP;

#define PERIOD_3SEC   3000
#define PERIOD_5SEC   5000
#define PERIOD_10SEC 10000
#define PERIOD_20SEC 20000
#define PERIOD_30SEC 30000

#define wait_sig_mask_network ( UI_PS_TIMER_SIG | UI_NETWORK_CB_SIG )
                        
#define wait_sig_mask_socket  ( UI_PS_TIMER_SIG | UI_NETWORK_CB_SIG | \
                                UI_SOCKET_CB_SIG )

#define wait_sig_mask_dns     ( UI_NETWORK_CB_SIG | UI_SOCKET_CB_SIG | \
								UI_DNS_CB_SIG  )

static NETSTATE NetState = NET_NULL_S;

static struct hostent g_hostent;
static char *g_addrlist[4];
static char g_addr[10];

static signed short  APPID = DSS_ERROR;

/*===========================================================================

LOCAL FUNCTION GETSERVER

DESCRIPTION
	Without calling gethostbyname, call getserver.  Mainly used for test

RETURN VALUE
    destionation IP for test.

SIDE EFFECTS
  None

===========================================================================*/

const char* getserver(void)
{
	const static char dnsserver[] = "203.236.1.20";  //sktelecom
//	const static char dnsserver[] = "203.236.1.14";  //connect fail test
	return dnsserver;
}

/*===========================================================================

LOCAL FUNCTION GET/SETKINDOFSERVICE

DESCRIPTION
  To differentiate its own service from other applications
  , set his service name.

DEPENDENCIES
  
RETURN VALUE
  Service kind/None

SIDE EFFECTS
  None

===========================================================================*/

void SetKindOfService(int Service)
{
	SET_SERVICE_STATE((service_name_type)Service);
}


int GetKindOfService(void)
{
	return (GET_SERVICE_STATE);
}

void NetworkSetState(NETSTATE netState)
{
	NetState = netState; 

	switch (NetState)
	{
		case	NET_NULL_S:
			MSG_MED("NET_NULL_S set",0,0,0);
//			APPID = DSS_ERROR;
//			SetKindOfService(NOSVC);
			break;

		case	NET_PPPOPENING_S:
			MSG_MED("NET_PPPOPENING_S set",0,0,0);
			break;		
		case	NET_PPPOPEN_S:
			MSG_MED("NET_PPPOPEN_S set",0,0,0);
			send_ui_cmd(UI_NET_CONNECTED_F);
			break;
			
		case	NET_CLOSING_S:
			MSG_MED("NET_CLOSING_S set",0,0,0);
			break;	
		case    NET_CLOSED_S:
			send_ui_cmd(UI_NET_DISCONNECTED_F);
			NetworkSetState(NET_NULL_S);
			break;
	}
}

NETSTATE NetworkGetState(void)
{
	switch (NetState)
	{
		case	NET_NULL_S:
			MSG_MED("NET_NULL_S",0,0,0);
			break;

		case	NET_PPPOPENING_S:
			MSG_MED("NET_PPPOPENING_S",0,0,0);
			break;		
		case	NET_PPPOPEN_S:
			MSG_MED("NET_PPPOPEN_S",0,0,0);
			break;
			
		case	NET_CLOSING_S:
			MSG_MED("NET_CLOSING_S",0,0,0);
			break;		
	}
	return NetState;
}


/*===========================================================================

  FUNCTION ProcessSocketCallback

  DESCRIPTION
  The function is to notify a given application that there is data to get.
  1. Socket Callback issued
  2. Notify Application that data is arrived by calling SocketStateChanged().
  3. Application will check which event is occured by using CheckSocketState().
  4. If read event occured, Application call a recv() or recvfrom().
  5. Enable or Disable the event selection.
  *  We don't use write event.

  DEPENDENCIES
  Accoding to service application.
  
  RETURN VALUE
  none

  SIDE EFFECTS
  None
===========================================================================*/
void	ProcessSocketCallback
(
	void
)
{
	int KindOfService = GetKindOfService();

	if (NetworkGetState() != NET_PPPOPEN_S)		
	{
		MSG_ERROR("Invalid State [%d]",NetworkGetState(),0,0);
		return;
	}

	 switch(KindOfService)
	 {
	 case WAP:
#ifdef FEATURE_WAP
		 WAP_SocketStateChanged();
#endif
		 break;
	 case JAVA:
		 //Do not use this function because application will poll to check data is.
		 break;
     /* undersea 2001-07-25 --> @why, what */
#ifdef FEATURE_SKVM
     case GVM:
         GVM_SocketStateChanged();
         break;
#endif
#ifdef FEATURE_WAVELET
     case WAVELET:
         WAV_SocketStateChanged();
         break;
#endif //FEATURE_WAVELET
#ifdef FEATURE_VAS
	 case VAS:
		 VAS_SocketStateChanged();
		 break;
#endif
		 /* <-- */
	 default:
		 break;
	}
}


/*===========================================================================

  FUNCTION CheckSocketState

  DESCRIPTION

  Check which event is occured.  Only read event is used.

  DEPENDENCIES
  Accoding to service application.
  
  RETURN VALUE
  event mask value

  SIDE EFFECTS
  None
===========================================================================*/

int CheckSocketState(SOCKET s)
{
	sint15 mask;
	sint15 errno;

	mask = dss_getnextevent(APPID, (short*)&s, &errno);
	if (mask & DS_READ_EVENT)
	{
		return 1;
	}
	else if (mask & DS_WRITE_EVENT)
	{
		return 2;
	}
	else if (mask & DS_CLOSE_EVENT)
	{
		return 3;
	}else if (mask == 0)
	{
		return 0;  //no event
	}
	return DSS_ERROR;
}

/*===========================================================================

  FUNCTION ProcessNetworkCallback

  DESCRIPTION
  The function is to notify a given application that network state is chaged.
  1. Network Callback issued
  2. Notify Application that network state is chaned
  3. Application check which event is occured. --> CheckNetworkState()
  4. If network is opend or closed, Application go on or stop.

  DEPENDENCIES
  Accoding to service application.
  
  RETURN VALUE
  result

  SIDE EFFECTS
  None
===========================================================================*/
void ProcessNetworkCallback(void)
{
	sint15 errno, state;

	switch(NetworkGetState())
	{
	case NET_PPPOPENING_S:
		state = dss_netstatus(APPID, &errno); 
		switch(errno)
		{
			case DS_ENETISCONN :
				MSG_HIGH("ConnectInternet: net_wait", 0, 0, 0);
				NetworkSetState(NET_PPPOPEN_S);
				break;
			case DS_EBADAPP :
				MSG_HIGH("BAD Connect: from net_wait", 0, 0, 0);
				break;
			case DS_ENETNONET :
				MSG_HIGH("BAD Connect: from net_wait", 0, 0, 0);
				break;
			case DS_ENETINPROGRESS :
				MSG_HIGH("network is establishing: from net_wait", 0, 0, 0);
				break;
			case DS_ENETCLOSEINPROGRESS :
				MSG_HIGH("net closing: from net_wait", 0, 0, 0);
				break;
			default:
				MSG_HIGH("Unknown errno [%d]", errno, 0, 0);
				break;
		}

		if(errno != DS_ENETISCONN)
		{
			if( GetKindOfService() == JAVA)
			{
				PPPCLOSE_VM();
			}
			else
			{
					PPPCLOSE();
			}
			MSG_ERROR("Network Not Available [%d]",errno,0,0);
		}
		break;

	case NET_CLOSING_S:
		if( GetKindOfService() == JAVA)
		{
			PPPCLOSE_VM();
		}
		else
		{
			PPPCLOSE();
		}
		break;

	case NET_NULL_S:
		MSG_HIGH("Already network is down",0,0,0);
		break;

	default:
		MSG_ERROR("Invalid networkstate [%d]",NetworkGetState(),0,0);
		break;
	}
}


sint15 PPPOPEN(void)
{
	sint15 errno;
	NETSTATE state = NetworkGetState();

	if(state == NET_PPPOPEN_S)
	{
		MSG_ERROR("Already network opened",0,0,0);
		return DSS_SUCCESS;
	}

	if(APPID == DSS_ERROR)
	{
	APPID = dss_open_netlib(&app_network_callback, &app_socket_callback, &errno); 
	}

	if( APPID == DSS_ERROR )
	{
		MSG_ERROR("openlib error",0,0,0);
		return DSS_ERROR;
	}

	if (dss_pppopen(APPID, &errno) == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADAPP:			// invalid application ID specified
				MSG_ERROR("PPPOPEN:err! invalid app. ID specified", 0, 0, 0);
				break;
		 	case DS_EWOULDBLOCK:
				MSG_HIGH("PPPOPEN: the operation would block", 0, 0, 0);
				break;
			case DS_ENETCLOSEINPROGRESS:
				MSG_ERROR("PPPOPEN:err! network close in progress", 0, 0, 0);
				break;
		}
		if (errno == DS_EWOULDBLOCK) 
		{
			MSG_HIGH("PPP OPENING....",0,0,0);
			NetworkSetState(NET_PPPOPENING_S);
			return (DSS_ERROR);
		}else
		{
			MSG_ERROR("PPP OPEN fail...",0,0,0);
			//Application should call PPPCLOSE
			return (DSS_ERROR);
		}
	}
	NetworkSetState(NET_PPPOPEN_S);
	MSG_HIGH("PPPOPEN: success!! %d", errno, 0, 0);		
	return (DSS_SUCCESS);
}	

sint15 PPPCLOSE(void)
{
	sint15 errno,result;
	NETSTATE state = NetworkGetState();

	if(state == NET_NULL_S)
	{
		return DSS_SUCCESS;
	}

	if (dss_pppclose(APPID, &errno) == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADAPP:	// invalid application ID specified
				MSG_ERROR("PPPCLOSE:err! invalid application ID specified", 0, 0, 0);
				break;
			case DS_ENETCLOSEINPROGRESS:
//				NetworkSetState(NET_CLOSED_S);
				MSG_ERROR("PPPCLOSE:err! network close in progress", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_HIGH("PPPCLOSE: operation would block", 0, 0, 0);
				NetworkSetState(NET_CLOSING_S);
				break;
		}
		return (DSS_ERROR);
	} 
	//pppclose success
	result = dss_close_netlib(APPID, &errno);
	if(result == DSS_ERROR)
	{
		switch(errno)
		{
			case DS_EBADAPP:
				MSG_HIGH("Close net lib : Bad app [%d]", APPID, 0, 0);
				break;
			case DS_SOCKEXIST:
				MSG_HIGH("Close net lib : sock exist", 0, 0, 0);
				break;
			case DS_ENETEXIST:
				MSG_HIGH("Close net lib : Net exist", 0, 0, 0);
				break;
		}
		return (DSS_ERROR);
	}
	APPID = DSS_ERROR;
	NetworkSetState(NET_CLOSED_S);
	MSG_HIGH("PPPCLOSE success",0,0,0);
	return	(DSS_SUCCESS);
}

/*===========================================================================

  FUNCTION SOCKET

  DESCRIPTION
  The Windows Sockets connect function establishes a connection to a specifed
  socket.

  DEPENDENCIES
	af   [in] An address family specification. 
	type [in] A type specification for the new socket. 
	The following are the only two type specifications supported for Windows Sockets 1.1: 
	Type Explanation 
	SOCK_STREAM Provides sequenced, reliable, two-way, connection-based byte streams with an out-of-band data transmission mechanism. Uses TCP for the Internet address family. 
	SOCK_DGRAM Supports datagrams, which are connectionless, unreliable buffers of a fixed (typically small) maximum length. Uses UDP for the Internet address family. 

  RETURN VALUE
  socket or DSS_ERROR

  SIDE EFFECTS
  None
===========================================================================*/

SOCKET socket
(
	int	af, 
	int	type, 
	int	protocol
)
{
	sint15	errno;
	SOCKET	s;

	s = dss_socket(APPID, af, type, protocol, &errno);
	if (s == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EAFNOSUPPORT:
				MSG_ERROR("socket: address family not supported", 0, 0, 0);
				break;
			case DS_EBADAPP:
				MSG_ERROR("socket: invalid application ID", 0, 0, 0);
				break;
			case DS_EPROTOTYPE:
				MSG_ERROR("socket: specified protocol invalid for socket type", 0, 0, 0);
				break;
			case DS_ESOCKNOSUPPORT:
				MSG_ERROR("socket: invalid or unsupported socket parameter specified", 0, 0, 0);
				break;
			case DS_EPROTONOSUPPORT:
				MSG_ERROR("socket: specified protocol not supported", 0, 0, 0);
				break;
			case DS_EMFILE:
				MSG_ERROR("socket: too many descriptors open.  A socket is already open or has not closed compeletely.", 0, 0, 0);
				break;
		}
	}
	MSG_HIGH("socket creation is completed !!", 0, 0, 0);		
	return s; 
}


/*===========================================================================

  FUNCTION CLOSESOCKET

  DESCRIPTION
  The Windows Sockets closesocket function closes an existing socket.

  DEPENDENCIES
  s [in] A descriptor identifying a socket to close.
  
  RETURN VALUE
  DSS_SUCCESS or DSS_ERROR

  SIDE EFFECTS
  None
===========================================================================*/
int closesocket 
(
  SOCKET s  
)
{
	sint15 result, errno;

	MSG_HIGH("closesocket(%s) called",s,0,0);
	
	if(s == DSS_ERROR)	return (DSS_SUCCESS);

	DisableReadEvent(s);
	DisableWriteEvent(s);

	result = dss_close((sint15)s, &errno);
	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
		case DS_EBADF:
			MSG_ERROR("closesocket err!: Invalid socket descriptor is specified", 0, 0, 0);
			break;
		case DS_EWOULDBLOCK:
			MSG_ERROR("closesocket err!: The operation would block", 0, 0, 0);
			EnableCloseEvent(s);
			if( ui_wait_sig((long int)PERIOD_10SEC, wait_sig_mask_socket) == FALSE)
			{
				return (DSS_ERROR);
			}

			result = dss_close((sint15)s, &errno);
			if(result == DSS_ERROR)
			{
				MSG_ERROR("closesocket : dss_close Error",0,0,0);
				return (DSS_ERROR);
			}
			break;
		}
	}
	return (int)result;
}

/*===========================================================================

  FUNCTION CONNECT

  DESCRIPTION
  The Windows Sockets connect function establishes a connection to a specifed socket.

  DEPENDENCIES
	Parameters
	s 
	[in] A descriptor identifying an unconnected socket. 
	name 
	[in] The name of the socket to connect to. 
	namelen 
	[in] The length of the name parameter. 

	Remarks
	The connect function is used to create a connection to the specified destination. 
	If the socket, s, is unbound, unique values are assigned to the local association 
	by the system, and the socket is marked as bound.
  
  RETURN VALUE
  success or error

  SIDE EFFECTS
  None
===========================================================================*/
int connect(
	SOCKET         s,  
	const struct sockaddr *name, 
	int            namelen
)
{
	sint15				errno, result;
	sint31				mask;

	result = dss_connect(s, (struct sockaddr *)name, 
						 (uint16)sizeof(struct sockaddr), &errno);

	if (errno == DS_EWOULDBLOCK) 
	{
		EnableWriteEvent(s);

		MSG_ERROR("CONNECT WAIT FOR CONNECT",0,0,0);
		if( ui_wait_sig((long int)PERIOD_30SEC, wait_sig_mask_socket) == FALSE)
		{
			MSG_ERROR("FAIL TCP CONNECT",0,0,0);
			return (DSS_ERROR);
		}
		MSG_ERROR("SUCCESS TCP CONNECT",0,0,0);

		mask = dss_getnextevent(APPID, (short*)&s, &errno);

		if (mask & DS_WRITE_EVENT) 
		{
			MSG_HIGH("connect: DS_WRITE_EVENT get write mask!!", 0, 0, 0);
			result = dss_connect(s, (struct sockaddr *)name, 
								 (uint16)sizeof(struct sockaddr), &errno);

			if (result==DSS_SUCCESS || errno==DS_EISCONN)
			{
				MSG_HIGH("connect: success!!", 0, 0, 0);		
				return DSS_SUCCESS;	
			}
			else
			{
				switch (errno) 
				{
					case DS_EWOULDBLOCK:
						MSG_ERROR("connect: operation would block", 0, 0, 0);
						break;
					case DS_EBADF:
						MSG_ERROR("connect: invalid socket descriptor is specfied", 0, 0, 0);
						break;
					case DS_ECONNREFUSED:
						MSG_ERROR("connect: connection attempt refused", 0, 0, 0);
						break;
					case DS_ETIMEDOUT:
						MSG_ERROR("connect: connection attempt timed out", 0, 0, 0);
						break;
					case DS_EFAULT:
						MSG_ERROR("connect: addrlen parameter is invalid", 0, 0, 0);
						break;
					case DS_EIPADDRCHANGED:
						MSG_ERROR("connect: IP address changed due to PPP resync", 0, 0, 0);
						break;
					case DS_EINPROGRESS:
						MSG_ERROR("connect: connection establishment in progress", 0, 0, 0);
						break;
					case DS_EISCONN:
						MSG_HIGH("connect: a socket descriptor is specified that is already connected", 0, 0, 0);
						return (sint15)s;
					case DS_ENETDOWN:
						MSG_ERROR("connect:: network subsystem unavailable", 0, 0, 0);
						break;
					case DS_EOPNOTSUPP:
						MSG_ERROR("connect: invalid server address specified", 0, 0, 0);
						break;
					case DS_EADDRREQ:
						MSG_ERROR("connect: destination address is required.", 0, 0, 0);
						break;
				}
				return DSS_ERROR;
			}
		}
	}
	
	if (result==DSS_SUCCESS || errno==DS_EISCONN)
	{
		return	DSS_SUCCESS;
	}
	else
	{
		return	DSS_ERROR;
	}
}

/*===========================================================================

  FUNCTION SENDTO

  DESCRIPTION

  The Windows Sockets send function sends data on a connected socket.

 
	Parameters
	s 
	[in] A descriptor identifying a connected socket. 
	buf 
	[in] A buffer containing the data to be transmitted. 
	len 
	[in] The length of the data in buf. 
	flags 
	[in] An indicator specifying the way in which the call is made. 

  RETURN VALUE
  n - the number of bytes written, which could be less than the number of 
      bytes specified.
  On error, return DSS_ERROR .

  SIDE EFFECTS
  None
===========================================================================*/
int send 
(
  SOCKET s,              
  const char * buf,  
  int len,               
  int flags              
)
{
	sint15 result, errno;

	result = dss_write((sint15)s, (byte*)buf, (sint15)len, &errno);
	EnableReadEvent(s);
	MSG_HIGH("send: len %d %d", len, result, 0);

	if (result == DSS_ERROR) 
	{
		switch (errno) {
		case DS_EBADF:
			MSG_ERROR("send: invalid socket descriptor is specfied", 0, 0, 0);
			break;
		case DS_ENOTCONN:
			MSG_ERROR("send: socket not connected", 0, 0, 0);
			break;
		case DS_ECONNRESET:
			MSG_ERROR("send: TCP connection reset by server", 0, 0, 0);
			break;
		case DS_ECONNABORTED:
			MSG_ERROR("send: TCP connection aborted due to timeout or other failure", 0, 0, 0);
			break;
		case DS_EIPADDRCHANGED:
			MSG_ERROR("send: IP address changed, causing TCP connection reset", 0, 0, 0);
			break;
		case DS_EPIPE:
			MSG_ERROR("send: broken pipe", 0, 0, 0);
			break;
		case DS_EADDRREQ:
			MSG_ERROR("send: destination address required", 0, 0, 0);
			break;
		case DS_ENETDOWN:
			MSG_ERROR("send: network subsystem unavailable", 0, 0, 0);
			break;
		case DS_EFAULT:
			MSG_ERROR("send: application buffer no valid part of address space", 0, 0, 0);
			break;
		case DS_EWOULDBLOCK:
			MSG_ERROR("send: operation would block", 0, 0, 0);
			break;
		default:
			MSG_ERROR("send: default", 0, 0, 0);
			break;
		}
	}
	return result;
}

/*===========================================================================

  FUNCTION RECV

  DESCRIPTION

  The Windows Sockets recv function receives data from a connected socket.
 
	Parameters
	s 
	[in] A descriptor identifying a connected socket. 
	buf 
	[out] A buffer for the incoming data. 
	len 
	[in] The length of buf. 
	flags 
	[in] A flag specifying the way in which the call is made. 


  RETURN VALUE
  n - the number of bytes to be read, which could be less than the number
      of bytes specified.
  On error, return DSS_ERROR .

  SIDE EFFECTS
  None
===========================================================================*/
int recv 
(
  SOCKET s,       
  char * buf,  
  int len,        
  int flags       
)
{
	sint15 result, errno;
	result = dss_read((sint15)s, (byte*)buf, (uint16)len, &errno);

	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("recv: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_ENOTCONN:
				MSG_ERROR("recv: socket not connected", 0, 0, 0);
				break;
			case DS_ECONNRESET:
				MSG_ERROR("recv: TCP connection reset by server", 0, 0, 0);
				break;
			case DS_ECONNABORTED:
				MSG_ERROR("recv: TCP connection aborted due to timeout or other failure", 0, 0, 0);
				break;
			case DS_EIPADDRCHANGED:
				MSG_ERROR("recv: IP address changed, causing TCP connection reset", 0, 0, 0);
				break;
			case DS_EPIPE:
				MSG_ERROR("recv: broken pipe", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("recv: destination address required", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("recv: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("recv: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				//EnableReadEvent(s);
				MSG_ERROR("recv: operation would block", 0, 0, 0);
				break;
			default:
				MSG_ERROR("sockApi_recvTCPDataOnce: default", 0, 0, 0);
				break;
		}
	}
	return result;
}


/*===========================================================================

  FUNCTION SENDTO

  DESCRIPTION
  The Windows Sockets recv function receives data from a connected socket.

  DEPENDENCIES
	s     [in] A descriptor identifying a (possibly connected) socket. 
	buf	  [in] A buffer containing the data to be transmitted. 
	len   [in] The length of the data in buf. 
	flags [in] An indicator specifying the way in which the call is made. 
	to    [in] An optional pointer to the address of the target socket. 
	tolen [in] The size of the address in to.  
  
  RETURN VALUE
  n - the number of bytes written, which could be less than the number of 
      bytes specified.
  On error, return DSS_ERROR .

  SIDE EFFECTS
  None
===========================================================================*/
int sendto
(
  SOCKET s,                        
  const char* buf,            
  int len,                         
  int flags,                       
  struct sockaddr* to,  
  int tolen                        
)
{
	sint15		result, errno;

	result = dss_sendto((sint15)s, 
						(byte*)buf, 
						(uint16)len,
						(uint16)flags, 
						to, 
						(uint16)tolen, 
						&errno);
    EnableReadEvent(s);

	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
		case DS_EBADF:
			MSG_ERROR("sendto err!: invalid socket descriptor is specfied", 0, 0, 0);
			break;
		case DS_EAFNOSUPPORT:
			MSG_ERROR("sendto err!: address family not supported", 0, 0, 0);
			break;
		case DS_EWOULDBLOCK:
			MSG_ERROR("sendto err!: operation would block", 0, 0, 0);
			break;
		case DS_EADDRREQ:
			MSG_ERROR("sendto err!: destination address required ", 0, 0, 0);
			break;
		case DS_ENETDOWN:
			MSG_ERROR("sendto err!: network subsystem unavailable", 0, 0, 0);
			break;
		case DS_EFAULT:
			MSG_ERROR("sendto err!: application buffer no valid part of address space", 0, 0, 0);
			break;
		case DS_EOPNOTSUPP:
			MSG_ERROR("sendto err!: option not supported", 0, 0, 0);
			break;
		default:
			MSG_ERROR("sendto err!: unknown error", 0, 0, 0);
			break;
		}
		return DSS_ERROR;
	}
	
	return result;
}
 

/*===========================================================================

  FUNCTION RECVFROM

  DESCRIPTION
  The Windows Sockets recv function receives data from a connected socket.

  DEPENDENCIES
	s       [in] A descriptor identifying a bound socket. 
	buf     [out] A buffer for the incoming data. 
	len     [in] The length of buf. 
	flags   [in] An indicator specifying the way in which the call is made. 
	from    [out] An optional pointer to a buffer that will hold the source address upon return. 
	fromlen [in/out] An optional pointer to the size of the from buffer. 

  
  RETURN VALUE
  n - the number of bytes to be read, which could be less than the number
      of bytes specified.
  On error, return DSS_ERROR .

  SIDE EFFECTS
  None
===========================================================================*/
int recvfrom
(
  SOCKET s,                   
  char* buf,              
  int len,                    
  int flags,                  
  struct sockaddr * from,  
  int* fromlen            
)
{
	sint15		result = DSS_ERROR;
	sint15      errno;

	result = dss_recvfrom((SOCKET)s, 
						(byte*)buf, 
						(uint16)len, 
						(uint16)flags, 
						from, 
						(uint16*)fromlen, 
						&errno);	

	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
		case DS_EBADF:
			MSG_ERROR("recvfrom: invalid socket descriptor is specfied", 0, 0, 0);
			break;
		case DS_EAFNOSUPPORT:
			MSG_ERROR("recvfrom: address family not supported", 0, 0, 0);
			break;
		case DS_EWOULDBLOCK:
			EnableReadEvent(s);	
			MSG_ERROR("recvfrom: operation would block", 0, 0, 0);
			break;
		case DS_EADDRREQ:
			MSG_ERROR("recvfrom: destination address required ", 0, 0, 0);
			break;
		case DS_ENETDOWN:
			MSG_ERROR("recvfrom: network subsystem unavailable", 0, 0, 0);
			break;
		case DS_EFAULT:
			MSG_ERROR("recvfrom: application buffer no valid part of address space", 0, 0, 0);
			break;
		case DS_EOPNOTSUPP:
			MSG_ERROR("recvfrom: option not supported", 0, 0, 0);
			break;
		}
	}		
	return result;
}

/*===========================================================================
  FUNCTION BIND
	The Windows Sockets bind function associates a local address with a socket.
 
	Parameters
	s 
	[in] A descriptor identifying an unbound socket. 
	name 
	[in] The address to assign to the socket from the SOCKADDR structure. 
	namelen 
	[in] The length of the name. 
	Remarks
	The bind function is used on an unconnected socket before subsequent calls to the connect or listen functions. It is used to bind to either connection-oriented (stream) or connectionless (datagram) sockets. When a socket is created with a call to the socket function, it exists in a name space (address family), but it has no name assigned to it. Use bind to establish the local association of the socket by assigning a local name to an unnamed socket.

  RESULT
  Returns DSS_SUCCESS on success.

  On error, return DSS_ERROR.
===========================================================================*/

int bind (
  SOCKET s,                          
  struct sockaddr *  name,  
  int namelen                        
)
{

	sint15		result = DSS_ERROR;
	sint15      errno;

	result = dss_bind( s,
		      name,
			  namelen,
			  &errno
			  );

	if(result == DSS_ERROR)
	{
		switch(errno)
		{
		case DS_EBADF:
			MSG_ERROR("bind: invalid socket descriptor is specfied", 0, 0, 0);
			break;
		case DS_EOPNOTSUPP:
			MSG_ERROR("bind: option not supported", 0, 0, 0);
			break;
		case DS_EADDRINUSE:
			MSG_ERROR("bind: address values are already specified", 0, 0, 0);
			break;
		case DS_EINVAL:
			MSG_ERROR("bind: socket is already attatched to a local name", 0, 0, 0);
			break;
		case DS_EFAULT:
			MSG_ERROR("bind: invalid address parameter", 0, 0, 0);
			break;
		default:
			break;
		}
	}//if

	return result;
}

/*===========================================================================

  FUNCTION INET_ADDR
  The Windows Sockets inet_addr function converts a string containing an (Ipv4)
  Internet Protocol dotted address into a proper address for the IN_ADDR structure.

  Parameters
  cp 
  [in] A null-terminated character string representing a number expressed in the Internet standard ".'' (dotted) notation. 
  Remarks
  The inet_addr function interprets the character string specified by the cp parameter. This string represents a numeric Internet address expressed in the Internet standard ".'' notation. The value returned is a number suitable for use as an Internet address. All Internet addresses are returned in IP's network order (bytes ordered from left to right).

  RESULT
  If no error occurs, inet_addr returns an unsigned long value 
  containing a suitable binary representation of the Internet address given. 
  If the string in the cp parameter does not contain a legitimate Internet address, 
  for example if a portion of an "a.b.c.d" address exceeds 255, 
  inet_addr returns the value INADDR_NONE.

===========================================================================*/
unsigned long inet_addr (
  const char * cp  
)
{
	int base, n;
	char c;
	unsigned int parts[4], val;
	unsigned int *pp = parts;

	c = *cp;
	for (;;) 
	{ 
		/* 0x=hex, 0=octal, isdigit=decimal */
//		if (!isdigit((int)c))
		if (!(c>='0' && c<='9'))
			return INADDR_NONE;
		val = 0; base = 10;
		if (c == '0') 
		{
			c = *++cp;
			if (c == 'x' || c == 'X') 
			{
				base = 16; 
				c = *++cp;
			} else
				base = 8;
		}
		for (;;) 
		{
//			if ((c < 0x7f) && isdigit((int)c)) 
			if ((c < 0x7f) && (c>='0' && c<='9'))
			{
				val = (val * base) + (c - '0');
			} 
//			else if (base == 16 && (c < 0x7f) && isxdigit((int)c)) 
			else if (base == 16 && (c < 0x7f) && 
				((c>='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F')))
			{
				val = (val << 4) |
//					(10 + c - (islower((int)c) ? 'a' : 'A'));
					(10 + c - ((c>='a' && c<='f') ? 'a' : 'A'));
			} 
			else
			{
				break;
			}
			c = *++cp;
		}
		if (c == '.') 
		{
			if (pp >= parts + 3)
				return 0;
			*pp++ = val;
			c = *++cp;
		} else
		{
			break;
		}
	}
//	if (c != '\0' && (!(c < 0x7f) || !isspace((int)c)))
	if (c != '\0' && (!(c < 0x7f) || c!=' '))
		return INADDR_NONE;

	n = pp - parts + 1;
	switch (n) {
		case 1:				/* a -- 32 bits */
			break;

		case 2:				/* a.b -- 8.24 bits */
			if (val > 0xffffff)
				return INADDR_NONE;
			val |= parts[0] << 24;
			break;

		case 3:				/* a.b.c -- 8.8.16 bits */
			if (val > 0xffff)
				return INADDR_NONE;
			val |= (parts[0] << 24) | (parts[1] << 16);
			break;

		case 4:				/* a.b.c.d -- 8.8.8.8 bits */
			if (val > 0xff)
				return INADDR_NONE;
			val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
			break;
	}
	return (val);
}

#ifdef  FEATURE_WAP

#define FEATURE_CANONICAL

PACKED struct dnsheader {
	unsigned short 	id;
	
	byte	rd		: 1;
	byte	tc		: 1;
	byte	aa		: 1;
	byte	opcode	: 4;
	byte	qr		: 1;
	
	byte	rcode	: 4;
	byte	z		: 3;
	byte	ra		: 1;

	unsigned short 	qdcount;
	unsigned short 	ancount;
	unsigned short 	nscount;
	unsigned short 	arcount;
};

/*===========================================================================

  FUNCTION GETHOSTBYNAME

  DESCRIPTION
	The Windows Sockets gethostbyname function retrieves host information corresponding to a host name from a host database.
 
 DEPENDENCIES

	Parameters
	name 
	[out] A pointer to the null-terminated name of the host to resolve. 
	Remarks
	The gethostbyname function returns a pointer to a HOSTENT structure ? a structure allocated by Windows Sockets. The HOSTENT structure contains the results of a successful search for the host specified in the name parameter.

	The application must never attempt to modify this structure or to free any of its components. Furthermore, only one copy of this structure is allocated per thread, so the application should copy any information it needs before issuing any other Windows Sockets function calls.

	The gethostbyname function cannot resolve IP address strings passed to it. Such a request is treated exactly as if an unknown host name were passed. Use inet_addr to convert an IP address string the string to an actual IP address, then use another function, gethostbyaddr, to obtain the contents of the HOSTENT structure.

	The gethostbyname function resolves the string returned by a successful call to gethostname.

	Return Values
	If no error occurs, gethostbyaddr returns a pointer to the HOSTENT structure. Otherwise, 
	it returns a NULL pointer, and a specific error code can be retrieved by calling WSAGetLastError.

  SIDE EFFECTS
  None
===========================================================================*/
LOCAL char g_hostname[80];
static char array_dnsname[32*3]; // 96


const char* next_delimeter(const char* str, char ch)
{
	while (*str && *str != ch)
		str++;
	return str;
}

struct hostent* gethostbyname(const char* name)
{
	int g_buf[500/4];
	SOCKET s;
	struct dnsheader header;
	char* data_buf;
	char* data_ptr;
	const char* next;

	int buf_size;
	int namelen;
	int len = 0;
	int pos = 0;
	int addrsize;
	struct sockaddr_in addr;	

	int	dns_access_cnt = 0;

	const char* g_dnsname;
	int CanonicalFormat = 0;

	g_dnsname = array_dnsname;
	memset((char*) g_dnsname, 0, sizeof(array_dnsname) );
	strncpy((char*)g_dnsname , (const char*)name, strlen(name) );

	if (*name>='0' && *name<='9')
	{
		unsigned long	iaddr = 0;

		iaddr = dss_htonl(inet_addr(name));
		if(iaddr != INADDR_NONE) //0xffffffff 
		{
			memset(g_addr, 0, sizeof(g_addr) );
			memcpy(g_addr, (char *)&iaddr, 4);
			g_addrlist[0] = g_addr;

			g_hostent.h_name = NULL;
			g_hostent.h_addrtype = AF_INET;
			g_hostent.h_aliases = NULL;	
			g_hostent.h_addr_list = g_addrlist;
			return (&g_hostent);
		}

		MSG_ERROR("gethostbyname Error ", 0, 0, 0);
		return (struct hostent *)(NULL);
	}

#ifdef FEATURE_CANONICAL
	CanonicalFormat = 0;

redirection:

    pos = 0;
    dns_access_cnt = 0;
#endif //#ifdef FEATURE_CANONICAL

#ifdef VASTEST
   {
		#include "clnt.h"

		CLNTa_log(1,0, "dns name [%s]", g_dnsname);
   }
#endif//test

	memset((void*)&header, 0, sizeof(struct dnsheader));
	memset(g_addrlist, 0, sizeof(char*) * 4);

	header.id = htons(0x01);
	header.rd = 1;
	header.qdcount = htons(0x01);

	namelen = strlen(g_dnsname);

	buf_size = sizeof(g_buf) ;
	memset(g_buf, 0, sizeof(g_buf) );
	data_buf = (char*)g_buf;

	memcpy(data_buf, (void*)&header, sizeof(struct dnsheader));
	pos = sizeof(struct dnsheader);
	next = next_delimeter(g_dnsname, '.');

	while(1)
	{
		len = next - g_dnsname;
		data_buf[pos++] = len;
		strncpy(data_buf + pos, g_dnsname, len);
		pos += len;
		if (*next == 0x00)
			break;		
		g_dnsname = next + 1;
		next = next_delimeter(g_dnsname, '.');		
	}

	data_buf[pos++] = 0x00;

	data_buf[pos]   = 0x00;
	data_buf[pos+1] = 0x01;	// Q TYPE: IP Address
	data_buf[pos+2] = 0x00;
	data_buf[pos+3] = 0x01; // Q CLASS : Internet Address
	pos += 4;


DNS_ACCESS_AGAIN:
	if(dns_access_cnt==0)
	{
		addr.sin_addr.s_addr = dss_htonl(primaryIP);

		MSG_HIGH("Primary IP %x %x ", 
			(int)((primaryIP>>16)&0x0000ffff),
			(int)((primaryIP)&0x0000ffff), 0);
		dns_access_cnt = 1;
	}
	else 
	{
		addr.sin_addr.s_addr = dss_htonl(secondaryIP);
		MSG_HIGH("Secondary IP %x %x ", 
			(int)((secondaryIP>>16)&0x0000ffff),
			(int)((secondaryIP)&0x0000ffff), 0);
		dns_access_cnt = 2;
	}

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(53);	

//#ifdef FEATURE_EMAIL_TEST //jmpark for without help dnsserver
//	addr.sin_addr.s_addr = inet_addr((char*)getdnsserver());
//#endif
	addrsize = sizeof(struct sockaddr);

	s = socket((byte)AF_INET, (byte)SOCK_DGRAM, (byte)IPPROTO_UDP); 
	if (s == INVALID_SOCKET) {
		MSG_HIGH("Invalid Socket in gethostbyname", 0, 0, 0);
		return NULL;
	}

	MSG_HIGH("SENDTO",0,0,0);
	len = sendto(s, data_buf, pos, 0, (struct sockaddr *)&addr, addrsize);
	if (len < 0) {
		MSG_HIGH("len 0 from sentto in gethostbyname", 0, 0, 0);
		closesocket(s);
		return NULL;
	}
	
	/* end of using data_buf. feel free to use it */
	if( ui_wait_sig((long int)PERIOD_3SEC, wait_sig_mask_socket ) == FALSE)
	{
		MSG_ERROR("FAIL WHILE WAIT",0,0,0);
		
		closesocket(s);
		if(dns_access_cnt==1 )
		{
			goto DNS_ACCESS_AGAIN;
		}
		return (struct hostent *)(NULL);
	}

	MSG_HIGH("RECVFROM",0,0,0);
	len = recvfrom(s, data_buf, buf_size, 0, (struct sockaddr *)&addr, &addrsize);	

	if (len < 0) 
	{
		MSG_HIGH("len 0 from recvfrom in gethostbyname", 0, 0, 0);

		closesocket(s);
		return (struct hostent *)(NULL);
	}

	if (closesocket(s) < 0) {
		MSG_ERROR("Invalid Socket in gethostbyname", 0, 0, 0);
		return (struct hostent *)(NULL);
	}

	/* need id check for MT */
	if ( ((struct dnsheader*) data_buf)->rcode != 0x00 )
	{
		MSG_HIGH("dns resultcode %d",((struct dnsheader*) data_buf)->rcode,0,0);
        if( ((struct dnsheader*) data_buf)->rcode == 3 )
		{
	         MSG_ERROR("서버이름을 확인",0,0,0);
		}
		return NULL;
	}
	pos = sizeof(struct dnsheader);
	
	/* skip my question */
	while( data_buf[pos] )
		pos += data_buf[pos] + 1;
	pos++;
	pos += 2;	/* QTYPE */
	pos += 2;	/* QCLASS */

//answer #1(RR) start
	/* now The Anser */
	data_ptr = g_hostname;

	if ( (data_buf[pos] & 0xc0) == 0xc0 )
	{
		/* Message Compress */
		int offset = ((data_buf[pos] & 0x3f) << 8) + data_buf[pos + 1];    

		pos += 2;
		while( data_buf[offset] )
		{
			strncpy(data_ptr, data_buf + offset + 1, data_buf[offset]);		
			data_ptr += data_buf[offset];
			offset += data_buf[offset] + 1;
			if (data_buf[offset])
				*data_ptr++ = '.';
		}
		*data_ptr = 0x00;
	}
	else
	{
		while( data_buf[pos] )
		{
			strncpy(data_ptr, data_buf + pos + 1, data_buf[pos]);			
			data_ptr += data_buf[pos];
			pos += data_buf[pos] + 1;
			if (data_buf[pos])
				*data_ptr++ = '.';
	
			if ( (data_buf[pos] & 0xc0) == 0xc0 )
			{
				/* Message Compress */
				int offset = ((data_buf[pos] & 0x3f) << 8) + data_buf[pos + 1];
				pos += 2;
				while( data_buf[offset] )
				{
					strncpy(data_ptr, data_buf + offset + 1, data_buf[offset]);			
					data_ptr += data_buf[offset];
					offset += data_buf[offset] + 1;
					if (data_buf[offset])
						*data_ptr++ = '.';
					break;
				}
				*data_ptr = 0x00;
			}
		}
		*data_ptr = 0x00;
	}
#ifdef FEATURE_CANONICAL
    if( ((data_buf[pos]<<8) & 0xff00 | data_buf[pos+1] & 0x00ff) == 0x05 )
	{
		MSG_HIGH("CANONICAL TYPE",0,0,0);
		if(++CanonicalFormat > 3) CanonicalFormat = 0;
	}
	else CanonicalFormat = 0;
#endif 
	pos += 2 + 2 + 4;	/* Type / Class / TTL */

    g_hostent.h_length =  (((*(data_buf + pos))<<8)&0xff00)|
                               ((*(data_buf + pos+1))&0x00ff);
	pos += 2;	/* Length  */

#ifdef FEATURE_CANONICAL
	MSG_HIGH("ip address length is %d", g_hostent.h_length,0,0);
	if(g_hostent.h_length > sizeof(g_addr)) 
		  g_hostent.h_length = sizeof(g_addr); //for safe 
	memcpy(g_addr, data_buf + pos, g_hostent.h_length);	
	g_addr[g_hostent.h_length] = 0; //for safe

	if(CanonicalFormat ) 
	{
		memset(g_addr, 0, sizeof(g_addr) );
		data_ptr = g_addr;
		if ( (data_buf[pos] & 0xc0) == 0xc0 )
		{
			/* Message Compress */
			int offset = ((data_buf[pos] & 0x3f) << 8) + data_buf[pos + 1];    
			MSG_HIGH("Message Compress offset %d",offset,0,0);
			pos += 2;
			while( data_buf[offset] )
			{
				strncpy(data_ptr, data_buf + offset + 1, data_buf[offset]);		
				data_ptr += data_buf[offset];
				offset += data_buf[offset] + 1;
				if (data_buf[offset])
					*data_ptr++ = '.';
			}
			*data_ptr = 0x00;
		}
		else
		{
			MSG_HIGH("No Message Compress",0,0,0);
			while( data_buf[pos] )
			{
				strncpy(data_ptr, data_buf + pos + 1, data_buf[pos]);
				data_ptr += data_buf[pos];
				pos += data_buf[pos] + 1;
				if (data_buf[pos])
					*data_ptr++ = '.';
		
				if ( (data_buf[pos] & 0xc0) == 0xc0 )
				{
					/* Message Compress */
					int offset = ((data_buf[pos] & 0x3f) << 8) + data_buf[pos + 1];
					pos += 2;
					while( data_buf[offset] )
					{
						strncpy(data_ptr, data_buf + offset + 1, data_buf[offset]);			
						data_ptr += data_buf[offset];
						offset += data_buf[offset] + 1;
						if (data_buf[offset])
							*data_ptr++ = '.';
					}
					*data_ptr = 0x00;
					break;
				} //if ( (data_buf[pos] & 0xc0) == 0xc0 )
			}//while( data_buf[pos] )
		*data_ptr = 0x00;
		}//if else
		g_dnsname = array_dnsname;
		memset((char*) g_dnsname, 0, sizeof(array_dnsname) );
		strcpy((char*)g_dnsname, (const char*)g_addr);
		MSG_HIGH("redirection",0,0,0);
		goto redirection;
	}//	if(CanonicalFormat )
#else
	if(g_hostent.h_length != 4 )
	{
	   MSG_ERROR("ip address length is %d", g_hostent.h_length,0,0);
	   g_hostent.h_length = 4; /*ntohs(*((u_short*) (data_buf + pos)));*/
	}
	memcpy(g_addr, data_buf + pos, 4 /*g_hostent.h_length*/);
#endif //FEATURE_CANONICAL
// answer #1(RR) end

	g_addrlist[0] = g_addr;

	g_hostent.h_name = g_hostname;
	g_hostent.h_addrtype = AF_INET;
	g_hostent.h_aliases = NULL;	
	g_hostent.h_addr_list = g_addrlist;

	return &g_hostent;
}

#else ///////////////////////////////////////////////////////////////////////
struct hostent * gethostbyname (
  const char * name  
)
{
	extern void dss_init_dns_cash_tbl(void);

	sint31				errno;
	uint32				ipAddr = 0;

	if (*name>='0' && *name<='9')
	{

		ipAddr = inet_addr(name);
		if(ipAddr != INADDR_NONE) //0xffffffff 
		{
			memset(g_addr, 0, sizeof(g_addr) );
			memcpy(g_addr, (char *)&ipAddr, 4);
			g_addrlist[0] = g_addr;

			g_hostent.h_name = NULL;
			g_hostent.h_addrtype = AF_INET;
			g_hostent.h_aliases = NULL;	
			g_hostent.h_addr_list = g_addrlist;
			return (&g_hostent);
		}

		MSG_ERROR("gethostbyname Error ", 0, 0, 0);
		return (struct hostent *)(NULL);
	}

	ds_atcop_qcdns_val[0] = primaryIP;
	ds_atcop_qcdns_val[1] = secondaryIP;	//	dss_gethostaddrs use this.
	
	errno = dss_gethostaddrs((char*)name, &ipAddr, &app_dns_callback);
	if (errno == DSS_SUCCESS)
	{
		MSG_HIGH("HIT CACHE ipAddr 0x%x%x", (int)((ipAddr)>>16), 
			                                (int)(ipAddr&0xFFFF), 0);
		if(GetKindOfService() == WAP)
		{
			dss_init_dns_cash_tbl();
		}

		if(ipAddr == 0)
		{
			return (struct hostent *)(NULL);
		}
		memset(g_addr, 0, sizeof(g_addr) );
		memcpy(g_addr, (char *)&ipAddr, 4);
		g_addrlist[0] = g_addr;

		g_hostent.h_name = NULL;
		g_hostent.h_addrtype = AF_INET;
		g_hostent.h_aliases = NULL;	
		g_hostent.h_addr_list = g_addrlist;
		return (&g_hostent);

	}else if (errno == DS_EWOULDBLOCK) 
	{
		if( ui_wait_sig((long int)0, wait_sig_mask_dns) == FALSE)
		{
			return (struct hostent *)(NULL);
		}
		errno = dss_gethostaddrs((char*)name, &ipAddr, &app_dns_callback);
		if (errno==DSS_SUCCESS)
		{
			MSG_HIGH("ipAddr 0x%x%x", (int)((ipAddr)>>16), (int)(ipAddr&0xFFFF), 0);

			if(GetKindOfService() == WAP)
			{
				dss_init_dns_cash_tbl();
			}

			if(ipAddr == 0)
			{
				return (struct hostent *)(NULL);
			}
			memset(g_addr, 0, sizeof(g_addr) );
			memcpy(g_addr, (char *)&ipAddr, 4);
			g_addrlist[0] = g_addr;

			g_hostent.h_name = NULL;
			g_hostent.h_addrtype = AF_INET;
			g_hostent.h_aliases = NULL;	
			g_hostent.h_addr_list = g_addrlist;
			return (&g_hostent);

		}else
		{
			switch(errno)
			{
			case DS_NAMEERR:
				MSG_ERROR("gethostbyname()>> DS_NAMEERR", 0, 0, 0);
				break;
			case DS_NAMEMISS:
				MSG_ERROR("gethostbyname()>> DS_AMEMISS", 0, 0, 0);
				break;
			case DS_NETERR:
				MSG_ERROR("gethostbyname()>> DS_NETERR", 0, 0, 0);
				break;
			case DS_NOMEMORY:
				MSG_ERROR("gethostbyname()>> DS_NOFMEMORY", 0, 0, 0);
				break;
			case DS_INUSE:
				MSG_ERROR("gethostbyname()>> DS_INUSE", 0, 0, 0);
				break;
			default:
				MSG_ERROR("gethostbyname()>> %d", errno, 0, 0);
			}
		}
	}//if(errno == DS_EWOULDBLOCK) 
	return (struct hostent *)(NULL);
}
#endif//gethostbyname

/*------------------------------------------------------------------------------------
						ALL KINDS OF API
--------------------------------------------------------------------------------------*/

void	EnableWriteEvent
(
	SOCKET	socketID
)
{
	sint15	errno;

	if (dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("EnableWriteEvent Err %d", errno, 0, 0);
	}
}

void	DisableWriteEvent
(
	SOCKET	socketID
)
{
	sint15	errno;

	if (dss_async_deselect((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("DisableWriteEvent Err %d", errno, 0, 0);
	}
}

void	EnableReadEvent
(
	SOCKET	socketID
)
{
	sint15	errno;

	if (dss_async_select((sint15)socketID, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("EnableReadEvent Err %d", errno, 0, 0);
	}
}

void	DisableReadEvent
(
	SOCKET	socketID
)
{
	sint15	errno;

	if (dss_async_deselect((sint15)socketID, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("DisableReadEvent Err %d", errno, 0, 0);
	}
}

void	EnableCloseEvent
(
	SOCKET	socketID
)
{
	sint15	errno;

	if (dss_async_select((sint15)socketID, (sint31)DS_CLOSE_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("EnableCloseEvent Err %d", errno, 0, 0);
	}
}

void	DisableCloseEvent
(
	SOCKET	socketID
)
{
	sint15	errno;

	if (dss_async_deselect((sint15)socketID, (sint31)DS_CLOSE_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("DisableCloseEvent Err %d", errno, 0, 0);
	}
}

/*--------------------------------------------------------------------------------*/
// for JAVA
sint15 PPPCLOSE_VM(void)
{
	sint15 errno;
	NETSTATE state = NetworkGetState();

	if(state == NET_NULL_S)
	{
		return DSS_SUCCESS;
	}

	if (dss_pppclose(APPID, &errno) == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADAPP:	// invalid application ID specified
				MSG_ERROR("PPPCLOSE:err! invalid application ID specified", 0, 0, 0);
				break;
			case DS_ENETCLOSEINPROGRESS:
//				NetworkSetState(NET_CLOSED_S);
				MSG_ERROR("PPPCLOSE:err! network close in progress", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_HIGH("PPPCLOSE: operation would block", 0, 0, 0);
				NetworkSetState(NET_CLOSING_S);
				break;
		}
		return (DSS_ERROR);
	}
	NetworkSetState(NET_CLOSED_S);
	return (DSS_SUCCESS);
}

sint15 CLOSE_NETLIB(void)
{
	sint15 result,errno;

	// 2001.09.27 by frogkim
	// netlib close된 상태에서 다시 CLOSE_NETLIB가 불릴 경우를 대비 하여.
	if (APPID == DSS_ERROR)
		return (DSS_SUCCESS);

	//pppclose success
	result = dss_close_netlib(APPID, &errno);
	if(result == DSS_ERROR)
	{
		switch(errno)
		{
			case DS_EBADAPP:
				MSG_HIGH("Close net lib : Bad app [%d]", APPID, 0, 0);
				break;
			case DS_SOCKEXIST:
				MSG_HIGH("Close net lib : sock exist", 0, 0, 0);
				break;
			case DS_ENETEXIST:
				MSG_HIGH("Close net lib : Net exist", 0, 0, 0);
				break;
		}
		return (DSS_ERROR);
	}
	APPID = DSS_ERROR;
//	NetworkSetState(NET_CLOSED_S);
	MSG_HIGH("CLOSE_NETLIB success",0,0,0);
	return	(DSS_SUCCESS);
}
//end of JAVA
/*--------------------------------------------------------------------------------*/

#endif//FEATURE_VAS