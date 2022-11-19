/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               S O C K E T  A P I

GENERAL DESCRIPTION
  This Software unit contains all the control functions for the Subscriber
  SOCK application Task. This includes all REX Interfacing functions, the task
  entry point and main processing loop, and the SOCK app. task-specific Wait 
  function.

LOCAL FUNCTIONS

	void	sockApi_setNetState(SOCK_NETSTATE	netState)
	sint15	sockApi_pppOpen(sint15	appID)
	sint15	sockApi_pppClose(sint15	appID)
	sint15	sockApi_tcpClose(sint15	socketID)
	sint15	sockApi_udpClose(sint15	socketID)
	sint15	sockApi_socketOpen(sint15 appID, byte family, byte type, byte protocol)
	sint15	sockApi_tcpConnectSub(sint15 appID,	sint15 socketID, struct hostentTag*, uint16	portNo)
	sint15	sockApi_tcpConnect(sint15 appID, struct hostentTag*, uint16)
	sint15	sockApi_sendTCPDataOnce(sint15 appID, sint15 socketID, byte*	buf, uint16	len)
	sint15	sockApi_recvTCPDataOnce(sint15 appID, sint15 socketID, byte*	buf, uint16	len)
	sint15	sockApi_sendTCPDatabyEvent(sint15 appID, sint15 socketID, byte*	buf, uint16	len)
	sint15	sockApi_recvTCPDatabyEvent(sint15 appID, sint15 socketID, byte*	buf, uint16	len)
	sint15	sockApi_sendUDPDataOnce(sint15 appID, sint15 socketID, byte* buf, uint16 len, 
			uint16 flags, struct sockaddr* to, uint16	tolen)
	sint15	sockApi_recvUDPDataOnce(sint15 appID, sint15 socketID, byte* buf, uint16 len, 
			uint16 flags, struct sockaddr* from, uint16*	tolen)
	sint15	sockApi_sendUDPDatabyEvent(sint15 appID, sint15 socketID, byte* buf, uint16 len, 
			uint16 flags, struct sockaddr* to, uint16	tolen)
	sint15	sockApi_recvUDPDatabyEvent(sint15 appID, sint15 socketID, byte* buf, uint16 len, 
			uint16 flags, struct sockaddr* from, uint16*	tolen)
	void	sockApi_closeNetwork(SOCK_NETSTATE	closedSockNetState)

EXTERNAL FUNCTIONS
    sockApi_openPPPNetwork(void)
    sockApi_openTCPNetwork(char* serverName, uint16	portNo)
    sockApi_openUDPNetwork(char* serverName, uint16	portNo)
	sockApi_closeNetworkbyReleaseCmd(void)
	sockApi_sendUDPData(void)
	sockApi_recvDUPData(void)
	sockApi_sendTCPData(void)
	sockApi_recvTCPData(void)
	
INITIALIZATION AND SEQUENCING REQUIREMENTS

===========================================================================
00/10/05	khekim	TCP connecting도중 UI Task에 의한 SOCKET_RELEASE_CMD 처리 구현
00/10/06	khekim	DNS server 접속중 UI Task에 의한 SOCKET_RELEASEE_CMD 수신시
					DNS server 재접속 하지 않도록 수정

   Copyright (c) 2000 by SK Teletech Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "queue.h"
#include "ds.h"
#include "dsm.h"
#include "mc.h"
#include "nv.h"
#include "msg.h"
#include "err.h"
#include "task.h"
#include "ui.h"
#include "dog.h"
#include <memory.h>
#include <stdio.h>
#include <ctype.h>

#ifdef	FEATURE_SKT_SOCKAPI
#include "sockctl.h"
#include "dssocket.h"
#include "uisockApi.h"

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
//#include "dssdns.h"
#include	"dsatcop.h"
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	FEATURE_CANONICAL
#define	FEATURE_SKT_DNS

#define	INADDR_NONE			0xffffffff
#define	PERIOD_5SEC			5000	//	5 seconds
#define	PERIOD_10SEC		10000	//	10 seconds
#define	PERIOD_20SEC		20000	//	20 seconds

typedef	enum	
{
	SOCK_NULL_S = 0,
	SOCK_PPPOPENING_S,
	SOCK_PPPOPEN_S,
	SOCK_UDPSOCK_OPENING_S,
	SOCK_UDPSOCK_OPEN_S,
	SOCK_TCPSOCK_OPENING_S,
	SOCK_TCPSOCK_OPEN_S,
	SOCK_TCPSOCK_REOPENING_S,
	SOCK_CLOSING_S
}	SOCK_NETSTATE;

struct hostentTag 
{
    char*   h_name;
    char**	h_aliases;
    short   h_addrtype;
    short   h_length;
    char**	h_addr_list;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VARIABLES
//////////////////////////////////////////////////////////////////////////////////////////////////////////
static	SOCK_NETSTATE	sockNetState = SOCK_NULL_S;
static	SOCK_NETSTATE	oldSockNetState = SOCK_NULL_S;

static	sint15					g_socketID = DSS_ERROR;
static	sint15					g_appID = DSS_ERROR;
static	struct hostentTag*		g_hostEntry = NULL;
static	struct sockaddr_in		sUdpHostAddr;
static	struct sockaddr_in*		g_udpHostAddr = NULL;	
static	uint16					g_sizeofudpHostAddr = 0;

static	struct hostentTag		l_hostent;

#ifdef	FEATURE_SKT_DNS	//	khekim 00/04/05
char				*dns_addrList[4];
char				dns_addr[33*3];
char				dns_serverNameSpace[33*3]; // 96
#endif	//	#ifdef	FEATURE_SKT_DNS	
boolean							g_bReleased = FALSE;

void	sockApi_closeNetwork(SOCK_NETSTATE	closedSockNetState);


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void	sockApi_setNetState
(
	SOCK_NETSTATE	netState
)
{	
	oldSockNetState = sockNetState;
	sockNetState = netState;
	switch (sockNetState)
	{
		case	SOCK_NULL_S:
			MSG_HIGH("setNetState>> SOCK_NULL_S", 0, 0, 0);
			sock_requested_sig_mask = SOCK_CMD_Q_SIG;
			sock_init();		//	cancel timer, clean send/recv Queue
			g_socketID = DSS_ERROR;
			g_appID = DSS_ERROR;
			g_bReleased = FALSE;	//	Released by UI
			break;
		case	SOCK_PPPOPENING_S:
			MSG_HIGH("setNetState>> SOCK_PPPOPENING_S", 0, 0, 0);
			sock_requested_sig_mask |= SOCK_NET_CB_SIG;
			break;
		case	SOCK_PPPOPEN_S:
			MSG_HIGH("setNetState>> SOCK_PPPOPEN_S", 0, 0, 0);
			sock_requested_sig_mask &= ~SOCK_NET_CB_SIG;
			break;
		case	SOCK_UDPSOCK_OPENING_S:
		case	SOCK_TCPSOCK_OPENING_S:
			// no operation
			if (sockNetState==SOCK_UDPSOCK_OPENING_S)
			{
				MSG_HIGH("setNetState>> SOCK_UDPSOCK_OPENING_S", 0, 0, 0);
			}
			else
			{
				MSG_HIGH("setNetState>> SOCK_TCPSOCK_OPENING_S", 0, 0, 0);
			}
			break;
		case	SOCK_UDPSOCK_OPEN_S:
		case	SOCK_TCPSOCK_OPEN_S:
			if (sockNetState==SOCK_UDPSOCK_OPEN_S)
			{
				MSG_HIGH("setNetState>> SOCK_UDPSOCK_OPEN_S", 0, 0, 0);
			}
			else
			{
				MSG_HIGH("setNetState>> SOCK_TCPSOCK_OPEN_S", 0, 0, 0);
			}
			sock_requested_sig_mask |= (SOCK_SENDDATA_Q_SIG | SOCK_SOCKET_CB_SIG);
#ifdef	FEATURE_SKT_SIMPLEIP	//	khekim 00/12/01
			sock_requested_sig_mask |= SOCK_NET_CB_SIG;	//	in case PPP resynced
#endif	//	#ifdef	FEATURE_SKT_SIMPLEIP
			break;
		case	SOCK_CLOSING_S:
			MSG_HIGH("setNetState>> SOCK_CLOSING_S", 0, 0, 0);
			sock_requested_sig_mask &= ~(SOCK_SENDDATA_Q_SIG | SOCK_SOCKET_CB_SIG);
			break;
		case	SOCK_TCPSOCK_REOPENING_S:
			MSG_HIGH("setNetState>> SOCK_TCPSOCK_REOPENING_S", 0, 0, 0);
			sock_requested_sig_mask &= ~(SOCK_SENDDATA_Q_SIG | SOCK_SOCKET_CB_SIG);
			break;
	}
}


sint15	sockApi_pppOpen
(
	sint15	appID
)
{
	sint15 errno, state;

	if (dss_pppopen(appID, &errno) == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADAPP:			// invalid application ID specified
				MSG_ERROR("sockApi_pppOpen:err! invalid app. ID specified", 0, 0, 0);
				break;
		 	case DS_EWOULDBLOCK:
				MSG_HIGH("sockApi_pppOpen: the operation would block", 0, 0, 0);
				break;
			case DS_ENETCLOSEINPROGRESS:
				MSG_ERROR("sockApi_pppOpen:err! network close in progress", 0, 0, 0);
				break;
		}
		if (errno == DS_EWOULDBLOCK) 
		{
			if(sock_wait_sig((long int)PERIOD_10SEC, 
							(long int)(SOCK_CMD_Q_SIG | SOCK_NET_CB_SIG  | SOCK_SOCKET_CB_SIG | SOCK_PS_TIMER_SIG),
									 (long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_NET_CB_SIG)) == DSS_ERROR)
			{
				return (DSS_ERROR);
			}
			state = dss_netstatus(appID, &errno); 
			MSG_HIGH("dss_netstatus : %d %d", errno, appID, 0);
			switch(errno)
			{
				case DS_ENETISCONN :
					MSG_HIGH("sockApi_pppOpen: %d %d", errno, state, 0);
					break;
				case DS_EBADAPP :
					MSG_HIGH("sockApi_pppOpen: Invalid appID", 0, 0, 0);
					break;
				case DS_ENETNONET :
					MSG_HIGH("sockApi_pppOpen: net unavailable %d %d", 0, 0, 0);
					break;
				case DS_ENETINPROGRESS :
					MSG_HIGH("sockApi_pppOpen: net establishing", 0, 0, 0);
					break;
				case DS_ENETCLOSEINPROGRESS :
					MSG_HIGH("sockApi_pppOpen: net closing", 0, 0, 0);
					break;
				default:
					break;
			}
		}
		if (errno != DS_ENETISCONN) 
		{
			MSG_HIGH("sockApi_pppOpen: failure!! %d", errno, 0, 0);		
			return DSS_ERROR;
		}
	}
	MSG_HIGH("sockApi_pppOpen: success!! %d", errno, 0, 0);		
	return DSS_SUCCESS;
}	

sint15	sockApi_pppClose
(
	sint15	appID
)
{
	sint15 errno, state;

	if (dss_pppclose(appID, &errno) == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADAPP:	// invalid application ID specified
				MSG_ERROR("sockApi_pppClose:err! invalid application ID specified", 0, 0, 0);
				break;
			case DS_ENETCLOSEINPROGRESS:
				MSG_ERROR("sockApi_pppClose:err! network close in progress", 0, 0, 0);
			case DS_EWOULDBLOCK:
				MSG_HIGH("sockApi_pppClose: operation would block", 0, 0, 0);
				if(sock_wait_sig((long int)PERIOD_5SEC, 
								(long int)(SOCK_CMD_Q_SIG | SOCK_NET_CB_SIG | SOCK_PS_TIMER_SIG),
								(long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_NET_CB_SIG))==DSS_ERROR) 
				{
					return (DSS_ERROR);
				}
				state = dss_netstatus(appID, &errno);
				switch(errno)
				{
					case DS_ENETISCONN :
						MSG_HIGH("sockApi_pppClose: conn%d %d", errno, state, 0);
						break;
					case DS_EBADAPP :
						MSG_HIGH("sockApi_pppClose: Invalid appID", 0, 0, 0);
						break;
					case DS_ENETNONET :
						MSG_HIGH("sockApi_pppOpen: network unavailable", 0, 0, 0);
						break;
					case DS_ENETINPROGRESS :
						MSG_HIGH("sockApi_pppOpen: net establishing", 0, 0, 0);
						break;
					case DS_ENETCLOSEINPROGRESS :
						MSG_HIGH("sockApi_pppOpen: net closing", 0, 0, 0);
						break;
					default:
						break;
				}
				if (errno != DS_ENETNONET)
				{
					MSG_HIGH("sockApi_pppClose: failure!! %d", errno, 0, 0);		
					return	DSS_ERROR;
				}
				break;
		}
	} 
	return	DSS_SUCCESS;
}

sint15	sockApi_tcpClose
(
	sint15	socketID
)
{
	sint15 result, errno;

	result = dss_close((sint15)socketID, &errno);
	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_tcpClose: Invalid socket descriptor ", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_tcpClose: The operation would block", 0, 0, 0);
				dss_async_deselect(socketID, (sint31)DS_WRITE_EVENT | DS_READ_EVENT, &errno);
				if(dss_async_select(socketID, (sint31)DS_CLOSE_EVENT, &errno)==DSS_ERROR)
				{
					MSG_HIGH("sockApi_tcpClose %d %d", result, errno, 0);
					return(DSS_ERROR);
				}
				if(sock_wait_sig((long int)PERIOD_5SEC, 
								(long int)(SOCK_CMD_Q_SIG | SOCK_SOCKET_CB_SIG | SOCK_NET_CB_SIG | SOCK_PS_TIMER_SIG),
								(long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_SOCKET_CB_SIG))==DSS_ERROR)
				{
					ERR("sockApi_tcpClose: SOCKET_CB_SIG not occurred",0,0,0);
					return (DSS_ERROR);
				}
				if ((result=dss_close((sint15)socketID, &errno))==DSS_ERROR)
				{
					ERR("sockApi_tcpClose: dss_close Error",0,0,0);
					return (DSS_ERROR);
				}
				break;
		}
	}
	return (result);
}


sint15 sockApi_udpClose
(
	sint15	socketID
)
{
	sint15 result, errno;
	result = dss_close((sint15)socketID, &errno);

	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_udpClose: Invalid socket descriptor is specified", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_udpClose: The operation would block", 0, 0, 0);
				break;
		}
	}
	return (int)result;
}


sint15 sockApi_socketOpen
(
	sint15	appID,
	byte	family, 
	byte	type, 
	byte	protocol
)
{
	sint15	errno;
	sint15	socketID;

	socketID = dss_socket(appID, family, type, protocol, &errno);
	if (socketID == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EAFNOSUPPORT:
				MSG_ERROR("sockApi_socketOpen: address family not supported", 0, 0, 0);
				break;
			case DS_EBADAPP:
				MSG_ERROR("sockApi_socketOpen: invalid application ID", 0, 0, 0);
				break;
			case DS_EPROTOTYPE:
				MSG_ERROR("sockApi_socketOpen: specified protocol invalid for socket type", 0, 0, 0);
				break;
			case DS_ESOCKNOSUPPORT:
				MSG_ERROR("sockApi_socketOpen: invalid or unsupported socket parameter specified", 0, 0, 0);
				break;
			case DS_EPROTONOSUPPORT:
				MSG_ERROR("sockApi_socketOpen: specified protocol not supported", 0, 0, 0);
				break;
			case DS_EMFILE:
				MSG_ERROR("sockApi_socketOpen: too many descriptors open.  A socket is already open or has not closed compeletely.", 0, 0, 0);
				break;
		}
	}
	MSG_HIGH("sockApi_socketOpen: success!!", 0, 0, 0);		

	return socketID; 
}


sint15 sockApi_sendTCPDataOnce
(
	sint15	appID,
	sint15	socketID,
	byte*	buf,
	uint16	len
)
{
	sint15 result = DSS_ERROR, errno;
	sint31 mask;

	result = dss_write((sint15)socketID, buf, (uint16)len, &errno);
	MSG_HIGH("sockApi_sendTCPDataOnce: len %d %d", len, result, 0);

	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK) ) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("sockApi_sendTCPDataOnce err: (0) %d %d", result, errno, 0);
			return(DSS_ERROR);
		}
	}
	else
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("sockApi_sendTCPDataOnce err: %d %d", result, errno, 0);
			return(DSS_ERROR);
		}

		if(sock_wait_sig((long int)PERIOD_5SEC, 
						(long int)(SOCK_CMD_Q_SIG | SOCK_SOCKET_CB_SIG |  SOCK_NET_CB_SIG | SOCK_PS_TIMER_SIG),
						(long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_SOCKET_CB_SIG)) == DSS_ERROR)
		{
			return (DSS_ERROR);
		}

		mask = dss_getnextevent(appID, &socketID, &errno);
		if (mask & DS_WRITE_EVENT) 
		{
			MSG_HIGH("sockApi_sendTCPDataOnce: get write mask!!", 0, 0, 0);
		} 
		else 
		{
			MSG_HIGH("sockApi_sendTCPDataOnce: failure writng", 0, 0, 0);
		}
	}
	
	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_sendTCPDataOnce: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_ENOTCONN:
				MSG_ERROR("sockApi_sendTCPDataOnce: socket not connected", 0, 0, 0);
				break;
			case DS_ECONNRESET:
				MSG_ERROR("sockApi_sendTCPDataOnce: TCP connection reset by server", 0, 0, 0);
				break;
			case DS_ECONNABORTED:
				MSG_ERROR("sockApi_sendTCPDataOnce: TCP connection aborted due to timeout or other failure", 0, 0, 0);
				break;
			case DS_EIPADDRCHANGED:
				MSG_ERROR("sockApi_sendTCPDataOnce: IP address changed, causing TCP connection reset", 0, 0, 0);
				break;
			case DS_EPIPE:
				MSG_ERROR("sockApi_sendTCPDataOnce: broken pipe", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_sendTCPDataOnce: destination address required", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_sendTCPDataOnce: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_sendTCPDataOnce: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_sendTCPDataOnce: operation would block", 0, 0, 0);
				break;
			default:
				MSG_ERROR("sockApi_sendTCPDataOnce: default", 0, 0, 0);
				break;
		}
	}
	return result;
}

sint15 sockApi_recvTCPDataOnce
(
	sint15	appID,
	sint15	socketID,
	byte*	buf,
	uint16	len
)
{
	sint15 result, errno;
	sint31 mask;

	result = dss_read((sint15)socketID, (byte*)buf, (uint16)len, &errno);

	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK) ) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("sockApi_recvTCPDataOnce err: %d %d", result, errno, 0);
			return(DSS_ERROR);
		}

		if(sock_wait_sig((long int)PERIOD_5SEC, 
						(long int)(SOCK_CMD_Q_SIG | SOCK_SOCKET_CB_SIG | SOCK_NET_CB_SIG | SOCK_PS_TIMER_SIG),
						(long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_SOCKET_CB_SIG)) == DSS_ERROR)
		{
			return (DSS_ERROR);
		}

		mask = dss_getnextevent(appID, &socketID, &errno);
		if (mask & DS_READ_EVENT) 
		{
			result = dss_read((sint15)socketID, (byte*)buf, (sint15)len, &errno);
			MSG_HIGH("sockApi_recvTCPDataOnce: len %d, errno %", result, errno, 0);
		} 
		else 
		{
			MSG_ERROR("sockApi_recvTCPDataOnce error!! len %d, errno %", result, errno, 0);
		}
	}

	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_recvTCPDataOnce: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_ENOTCONN:
				MSG_ERROR("sockApi_recvTCPDataOnce: socket not connected", 0, 0, 0);
				break;
			case DS_ECONNRESET:
				MSG_ERROR("sockApi_recvTCPDataOnce: TCP connection reset by server", 0, 0, 0);
				break;
			case DS_ECONNABORTED:
				MSG_ERROR("sockApi_recvTCPDataOnce: TCP connection aborted due to timeout or other failure", 0, 0, 0);
				break;
			case DS_EIPADDRCHANGED:
				MSG_ERROR("sockApi_recvTCPDataOnce: IP address changed, causing TCP connection reset", 0, 0, 0);
				break;
			case DS_EPIPE:
				MSG_ERROR("sockApi_recvTCPDataOnce: broken pipe", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_recvTCPDataOnce: destination address required", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_recvTCPDataOnce: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_recvTCPDataOnce: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_recvTCPDataOnce: operation would block", 0, 0, 0);
				break;
			default:
				MSG_ERROR("sockApi_recvTCPDataOnce: default", 0, 0, 0);
				break;
		}
	}
	return result;
}


sint15 sockApi_sendTCPDatabyEvent
(
	sint15	appID,
	sint15	socketID,
	byte*	buf,
	uint16	len
)
{
	sint15	result, errno;

	result = dss_write((sint15)socketID, (byte*)buf, (uint16)len, &errno);
	MSG_HIGH("sockApi_sendTCPDatabyEvent: len %d %d", len, result, 0);

	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK) ) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
		{
			MSG_ERROR("sockApi_sendTCPDatabyEvent err %d %d", result, errno, 0);
		}
	}
	
	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_ENOTCONN:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: socket not connected", 0, 0, 0);
				break;
			case DS_ECONNRESET:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: TCP connection reset by server", 0, 0, 0);
				break;
			case DS_ECONNABORTED:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: TCP connection aborted due to timeout or other failure", 0, 0, 0);
				break;
			case DS_EIPADDRCHANGED:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: IP address changed, causing TCP connection reset", 0, 0, 0);
				break;
			case DS_EPIPE:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: broken pipe", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: destination address required", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_sendTCPDataOnce: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: operation would block", 0, 0, 0);
				break;
			default:
				MSG_ERROR("sockApi_sendTCPDatabyEvent: default", 0, 0, 0);
				break;
		}
	}
	return result;
}

sint15 sockApi_recvTCPDatabyEvent
(
	sint15	appID,
	sint15	socketID,
	byte*	buf,
	uint16	len
)
{
	sint15	result, errno;

	result = dss_read((sint15)socketID, (byte*)buf, (uint16)len, &errno);

	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK) ) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("sockApi_recvTCPDatabyEvent err: %d %d", result, errno, 0);
		}
	}
	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_ENOTCONN:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: socket not connected", 0, 0, 0);
				break;
			case DS_ECONNRESET:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: TCP connection reset by server", 0, 0, 0);
				break;
			case DS_ECONNABORTED:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: TCP connection aborted due to timeout or other failure", 0, 0, 0);
				break;
			case DS_EIPADDRCHANGED:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: IP address changed, causing TCP connection reset", 0, 0, 0);
				break;
			case DS_EPIPE:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: broken pipe", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: destination address required", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: operation would block", 0, 0, 0);
				break;
			default:
				MSG_ERROR("sockApi_recvTCPDatabyEvent: default", 0, 0, 0);
				break;
		}
	}
	return result;
}


sint15	sockApi_sendUDPDataOnce
(
	sint15			appID,
	sint15			socketID, 
	byte*			buf, 
	uint16			len, 
	uint16			flags, 
	struct sockaddr*	to, 
	uint16			tolen
)
{
	sint15		result, errno;
	sint31		mask;

#if MSG_LEVEL != MSG_LVL_NONE 
	byte* p = (byte*)to->sa_data + 2;

	MSG_HIGH("sockApi_sendUDPDataOnce: len %d, addr %x %x", len, (p[0] << 8 | p[1]), (p[2] << 8 | p[3]));
#endif	//	#if MSG_LEVEL != MSG_LVL_NONE 

	result = dss_sendto((sint15)socketID, (byte*)buf, (uint16)len, (uint16)flags, 
													to, (uint16)tolen, &errno);
	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK)) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("sockApi_sendUDPDataOnce err: (0) %d %d", result, errno, 0);
			return	DSS_ERROR;
		}
		if(sock_wait_sig((long int)PERIOD_5SEC, 
						(long int)(SOCK_CMD_Q_SIG | SOCK_SOCKET_CB_SIG | SOCK_NET_CB_SIG | SOCK_PS_TIMER_SIG),
						(long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_SOCKET_CB_SIG))==DSS_ERROR)
		{
			return	DSS_ERROR;
		}

		mask = dss_getnextevent(appID, &socketID, &errno);
		if (mask & DS_WRITE_EVENT) 
		{
			MSG_HIGH("sockApi_sendUDPDataOnce:(1) get write mask!!", 0, 0, 0);
			result = dss_sendto((sint15)socketID, (byte*)buf, (uint16)len, (uint16)flags, 
															to, (uint16)tolen, &errno);
		} 
		else 
		{
			MSG_HIGH("sockApi_sendUDPDataOnce:(1) failure writng", 0, 0, 0);
		}
	}
	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_sendUDPDataOnce: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_EAFNOSUPPORT:
				MSG_ERROR("sockApi_sendUDPDataOnce: address family not supported", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_sendUDPDataOnce: operation would block", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_sendUDPDataOnce: destination address required ", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_sendUDPDataOnce: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_sendUDPDataOnce: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EOPNOTSUPP:
				MSG_ERROR("sockApi_sendUDPDataOnce: option not supported", 0, 0, 0);
				break;
		}
		return	DSS_ERROR;
	}
	MSG_HIGH("sockApi_sendUDPDataOnce: success!!", 0, 0, 0);		
	return	result;
}
 
sint15	sockApi_recvUDPDataOnce
(
	sint15	appID,
	sint15	socketID, 
	byte*	buf, 
	uint16	len, 
	uint16	flags, 
	struct sockaddr* from,
	uint16*	fromlen,
	uint16	waittime
)
{
	sint15		result=DSS_ERROR, errno;
	sint31		mask;

	result = dss_recvfrom((sint15)socketID, (byte*)buf, (uint16)len, (uint16)flags, 
						from, (uint16*)fromlen, &errno);	

	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK)) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_READ_EVENT, &errno) == DSS_ERROR)
		{
			MSG_HIGH("sockApi_recvUDPDataOnce: err %d %d", result, errno, 0);
			return	DSS_ERROR;
		}

		if(sock_wait_sig((long int)waittime, 
						(long int)(SOCK_CMD_Q_SIG | SOCK_SOCKET_CB_SIG |  SOCK_NET_CB_SIG | SOCK_PS_TIMER_SIG),
						(long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_SOCKET_CB_SIG)) == DSS_ERROR)
		{
			return	DSS_ERROR;
		}

		mask = dss_getnextevent(appID, &socketID, &errno);
		if (mask & DS_READ_EVENT) 
		{
			result = dss_recvfrom((sint15)socketID, (byte*)buf, (uint16)len, (uint16)flags, from, 
				(uint16*)fromlen, &errno);	
			MSG_HIGH("sockApi_recvUDPDataOnce: len %d, errno %d", result, errno, 0);
		}
		else 
		{
			MSG_HIGH("sockApi_recvUDPDataOnce: failure reading", 0, 0, 0);
		}

	} 

	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_recvUDPDataOnce: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_EAFNOSUPPORT:
				MSG_ERROR("sockApi_recvUDPDataOnce: address family not supported", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_recvUDPDataOnce: operation would block", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_recvUDPDataOnce: destination address required ", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_recvUDPDataOnce: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_recvUDPDataOnce: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EOPNOTSUPP:
				MSG_ERROR("sockApi_recvUDPDataOnce: option not supported", 0, 0, 0);
				break;
		}
		return DSS_ERROR;
	}
	MSG_HIGH("sockApi_recvUDPDataOnce: success!!", 0, 0, 0);		
	return result;
}


sint15	sockApi_sendUDPDatabyEvent
(
	sint15			appID,
	sint15			socketID, 
	byte*			buf, 
	uint16			len, 
	uint16			flags, 
	struct sockaddr*	to, 
	uint16			tolen
)
{
	sint15		result, errno;

#if MSG_LEVEL != MSG_LVL_NONE 
	byte* p = (byte*)to->sa_data + 2;

	MSG_HIGH("sockApi_sendUDPDatabyEvent: len %d, addr %x %x", len, (p[0] << 8 | p[1]), (p[2] << 8 | p[3]));
#endif	//	#if MSG_LEVEL != MSG_LVL_NONE 

	result = dss_sendto((sint15)socketID, (byte*)buf, (uint16)len, (uint16)flags, 
													to, (uint16)tolen, &errno);
	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK)) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("sockApi_sendUDPDatabyEvent err: (0) %d %d", result, errno, 0);
			return	DSS_ERROR;
		}
	}
	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_sendUDPDatabyEvent: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_EAFNOSUPPORT:
				MSG_ERROR("sockApi_sendUDPDatabyEvent: address family not supported", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_sendUDPDatabyEvent: operation would block", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_sendUDPDatabyEvent: destination address required ", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_sendUDPDatabyEvent: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_sendUDPDatabyEvent: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EOPNOTSUPP:
				MSG_ERROR("sockApi_sendUDPDatabyEvent: option not supported", 0, 0, 0);
				break;
		}
		return	DSS_ERROR;
	}
	MSG_HIGH("sockApi_sendUDPDatabyEvent: success!!", 0, 0, 0);		
	return	result;
}
 
sint15	sockApi_recvUDPDatabyEvent
(
	sint15	appID,
	sint15	socketID, 
	byte*	buf, 
	uint16	len, 
	uint16	flags, 
	struct sockaddr* from,
	uint16*	fromlen
)
{
	sint15		result=DSS_ERROR, errno;

	result = dss_recvfrom((sint15)socketID, (byte*)buf, (uint16)len, (uint16)flags, 
						from, (uint16*)fromlen, &errno);	

	if ((result == DSS_ERROR) && (errno == DS_EWOULDBLOCK)) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_READ_EVENT, &errno) == DSS_ERROR)
		{
			MSG_HIGH("sockApi_recvUDPDatabyEvent: err %d %d", result, errno, 0);
			return	DSS_ERROR;
		}
	} 

	if (result == DSS_ERROR) 
	{
		switch (errno) 
		{
			case DS_EBADF:
				MSG_ERROR("sockApi_recvUDPDatabyEvent: invalid socket descriptor is specfied", 0, 0, 0);
				break;
			case DS_EAFNOSUPPORT:
				MSG_ERROR("sockApi_recvUDPDatabyEvent: address family not supported", 0, 0, 0);
				break;
			case DS_EWOULDBLOCK:
				MSG_ERROR("sockApi_recvUDPDatabyEvent: operation would block", 0, 0, 0);
				break;
			case DS_EADDRREQ:
				MSG_ERROR("sockApi_recvUDPDatabyEvent: destination address required ", 0, 0, 0);
				break;
			case DS_ENETDOWN:
				MSG_ERROR("sockApi_recvUDPDatabyEvent: network subsystem unavailable", 0, 0, 0);
				break;
			case DS_EFAULT:
				MSG_ERROR("sockApi_recvUDPDatabyEvent: application buffer no valid part of address space", 0, 0, 0);
				break;
			case DS_EOPNOTSUPP:
				MSG_ERROR("sockApi_recvUDPDatabyEvent: option not supported", 0, 0, 0);
				break;
		}
		return DSS_ERROR;
	}
	MSG_HIGH("sockApi_recvUDPDatabyEvent: success!!", 0, 0, 0);		
	return result;
}
	

#ifdef	FEATURE_SKT_DNS	//	khekim 00/04/05

PACKED struct dnsheader {
	uint16 	id;
	
	unsigned int	rd		: 1;
	unsigned int	tc		: 1;
	unsigned int	aa		: 1;
	unsigned int	opcode	: 4;
	unsigned int	qr		: 1;
	
	unsigned int	rcode	: 4;
	unsigned int	z		: 3;
	unsigned int	ra		: 1;

	uint16 	qdcount;
	uint16	ancount;
	uint16 	nscount;
	uint16 	arcount;
};

char* getHostServer
(
	void
)
{
	static char hostServer[] = "203.236.1.12";  //sk
	return hostServer;
}

char* getNextDelimeter
(
	char* str, 
	char ch
)
{
	while (*str && *str != ch)
		str++;
	return str;
}

uint32 inetAddr
(
	char* cp
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
	return dss_htonl(val);
}


struct hostentTag* getHostByAddr
(
	char* addr
)
{
//	struct hostentTag	l_hostent;

	memcpy(dns_addr, addr, 4);
	dns_addrList[0] = dns_addr;

	l_hostent.h_name = NULL;
	l_hostent.h_addrtype = AF_INET;
	l_hostent.h_aliases = NULL;	
	l_hostent.h_addr_list = dns_addrList;
	return &l_hostent;
}

struct hostentTag* getHostByName
(
	sint15		appID,
	char* serverName
)
{

	sint15				socketID;
	struct dnsheader	header;
	char				data_buf[256];
	char*				data_ptr;
	char*				next;

//	struct hostentTag	l_hostent;
	uint16				namelen;
	sint15				len;
	uint16				pos = 0, posFirst;
	uint16				addrsize;
	struct sockaddr_in	addr;	

	uint16				dns_access_cnt = 0;
	uint16				waittime;

	char*				l_serverName;
#ifdef FEATURE_CANONICAL
	boolean				getCanonicalName=FALSE;	// alias of server name
#endif	//	#ifdef FEATURE_CANONICAL
	boolean				getDns=FALSE, getIPAddress=FALSE;
	uint16				in=0, ansCount, canonicalCount=0;

	if (*serverName>='0' && *serverName<='9')
	{
		uint32	ipAddr;

		ipAddr = inetAddr(serverName);
		MSG_HIGH("inetAddr 0x%x%x", (int)((ipAddr)>>16), (int)(ipAddr&0xFFFF), 0);
		if(ipAddr != INADDR_NONE) //0xffffffff 
		{
			memcpy(dns_addr, (char *)&ipAddr, 4);
			dns_addrList[0] = dns_addr;
			l_hostent.h_name = NULL;
			l_hostent.h_addrtype = AF_INET;
			l_hostent.h_aliases = NULL;	
			l_hostent.h_addr_list = dns_addrList;
			return &l_hostent;
		}
		return (struct hostentTag* )NULL;
	}
	else
	{
		l_serverName = dns_serverNameSpace;
		memset((char*) l_serverName, 0, sizeof(dns_serverNameSpace) );
		strncpy((char*)l_serverName , (char*)serverName, strlen(serverName) );

	do
	{
#ifdef FEATURE_CANONICAL
	getCanonicalName = FALSE;
    pos = 0;
    dns_access_cnt = 0;
#endif //#ifdef FEATURE_CANONICAL

	memset((byte *)&header, 0, sizeof(struct dnsheader));
	memset((byte *)dns_addrList, 0, sizeof(char*) * 4);

	header.id = dss_htons(0x01);
	header.rd = 1;
	header.qdcount = dss_htons(0x01);

	namelen = strlen(l_serverName);

	memset(data_buf, 0, sizeof(data_buf) );

	memcpy((byte *)data_buf, (byte *)&header, sizeof(struct dnsheader));
	pos = sizeof(struct dnsheader);
	next = getNextDelimeter(l_serverName, '.');

	while(1)
	{
		len = next - l_serverName;
		data_buf[pos++] = len;
		strncpy(data_buf + pos, l_serverName, len);
		pos += len;
		if (*next == 0x00)
			break;		
		l_serverName = next + 1;
		next = getNextDelimeter(l_serverName, '.');		
	}

	data_buf[pos++] = 0x00;

	data_buf[pos] = 0x00;
	data_buf[pos+1] = 0x01;	// Q TYPE: IP Address
	data_buf[pos+2] = 0x00;
	data_buf[pos+3] = 0x01; // Q CLASS : Internet Address
	pos += 4;

	while (getDns==FALSE && dns_access_cnt<2)
	{
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
	//	addr.sin_addr.s_addr = inetAddr((char*)getHostServer());
		addr.sin_family = AF_INET;
		addr.sin_port   = dss_htons(53);	
		addrsize = sizeof(struct sockaddr);

	// g_socketID should be set to close UDP socket successfully when receiving SOCKET_RELEASE_CMD by UI task
		if ((socketID=g_socketID=sockApi_socketOpen(appID, (byte)AF_INET, (byte)SOCK_DGRAM, (byte)IPPROTO_UDP))
				== DSS_ERROR) 
		{
			MSG_HIGH("Invalid Socket in gethostbyname", 0, 0, 0);
			return NULL;
		}

		if ((len=sockApi_sendUDPDataOnce(appID, socketID, (byte *)data_buf, pos, 0, (struct sockaddr *)&addr, addrsize))
			==DSS_ERROR) 
		{
			MSG_ERROR("getHostByName: sendUDPData err", 0, 0, 0);
			sockApi_udpClose(socketID);
			g_socketID = DSS_ERROR;
			return NULL;
		}
		/* end of using data_buf. feel free to use it */

		if(dns_access_cnt == 1)
			waittime = PERIOD_10SEC;
		else if(dns_access_cnt == 2)
			waittime = PERIOD_20SEC;
		else
			waittime = PERIOD_20SEC;

		len=sockApi_recvUDPDataOnce(appID, socketID, (byte *)data_buf, sizeof(data_buf), 0, (struct sockaddr *)&addr, &addrsize, waittime);	
		if (len==DSS_ERROR) 
		{
			MSG_HIGH("getHostByName: recvUDPData err", 0, 0, 0);
			if (g_bReleased==TRUE)	//	DSN server에 IP address를 받는 도중 SOCK_RELEASE_CMD수신시 다시 DNS Server에 접속을 하지 않도록 하기 위함
			{
				sockApi_udpClose(socketID);
				g_socketID = DSS_ERROR;
				return NULL;
			}
			else
			{
				if(dns_access_cnt==1)
				{
					sockApi_udpClose(socketID);
					g_socketID = DSS_ERROR;
				}
				else 
				{
					sockApi_udpClose(socketID);
					g_socketID = DSS_ERROR;
					return NULL;
				}
			}
		}
		else
		{
			getDns = TRUE;
			if (sockApi_udpClose(socketID)==DSS_ERROR) 
			{
				MSG_HIGH("getHostByName: UDP Invalid Socket", 0, 0, 0);
				g_socketID = DSS_ERROR;
				return NULL;
			}
			g_socketID = DSS_ERROR;
		}
	}

	/* need id check for MT */
	if ( ((struct dnsheader*) data_buf)->rcode != 0x00 )
	{
		MSG_HIGH("dns resultcode %d",((struct dnsheader*) data_buf)->rcode,0,0);
        if( ((struct dnsheader*) data_buf)->rcode == 3 )
		{
	         MSG_HIGH("getHostByName: Please check host name",0,0,0);
		}
		return NULL;
	}
	pos = sizeof(struct dnsheader);

	/* skip my question */
	while( data_buf[pos] )
		pos += data_buf[pos] + 1;
	pos++;	//	End of HostName
	pos += 2;	/* QTYPE */
	pos += 2;	/* QCLASS */

//answer #1(RR) start
	/* now The Answer */
	getIPAddress = FALSE;
	in = 0;
	ansCount = dss_htons(((struct dnsheader*) data_buf)->ancount);	//ansCount

	while (in++ < ansCount && getIPAddress==FALSE)	//	Number of Answer
	{
		data_ptr = dns_addr;

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
			MSG_HIGH("GET CANONICAL NAME",0,0,0);
			getCanonicalName = TRUE;
		}
		else
		{
			getCanonicalName = FALSE;
			if( ((data_buf[pos]<<8) & 0xff00 | data_buf[pos+1] & 0x00ff) == 0x01 )	//	get IP Address
			{
				MSG_HIGH("GET IP ADDRESS",0,0,0);
				getIPAddress = TRUE;
			}
		}
#endif 
		pos += 2 + 2 + 4;	/* Type / Class / TTL */

		l_hostent.h_length =  (((*(data_buf + pos))<<8)&0xff00)|
								   ((*(data_buf + pos+1))&0x00ff);
		pos += 2;	/* Length  */

#ifdef FEATURE_CANONICAL
		MSG_HIGH("ans%d ip address length is %d", ((struct dnsheader*) data_buf)->ancount, l_hostent.h_length,0);
		if(l_hostent.h_length > sizeof(dns_addr)) 
			  l_hostent.h_length = sizeof(dns_addr); //for safe 000313
		memcpy(dns_addr, data_buf + pos, l_hostent.h_length);	
		dns_addr[l_hostent.h_length] = 0; //for safe
////////////////////////////////////// DUMP
		{
			int i;
			for(i=0; i<l_hostent.h_length; i+=3)
			{
				MSG_HIGH("dns_addr %x %x %x",dns_addr[i],dns_addr[i+1],dns_addr[i+2] );
			}
		}
////////////////////////////////////// DUMP
		if(getCanonicalName ) 
		{
			memset(dns_addr, 0, sizeof(dns_addr) );
			data_ptr = dns_addr;
			posFirst = pos;
			while (pos< (posFirst+l_hostent.h_length))
			{
				if( data_buf[pos] )
				{
					strncpy(data_ptr, data_buf + pos + 1, data_buf[pos]);
					data_ptr += data_buf[pos];
					pos += data_buf[pos] + 1;
					if (data_buf[pos])
						*data_ptr++ = '.';
					else
						*data_ptr++ = 0x00;
				}
				else
				{
					pos++;
					*data_ptr++ = 0x00;
				}
			}
			l_serverName = dns_serverNameSpace;
			memset((char*) l_serverName, 0, sizeof(dns_serverNameSpace) );
			strcpy((char*)l_serverName, (char*)dns_addr);
		}//	if(getCanonicalName )
#else
		if(l_hostent.h_length != 4 )
		{
		   ERR("ip address length is %d", l_hostent.h_length,0,0);
		   l_hostent.h_length = 4; /*ntohs(*((u_short*) (data_buf + pos)));*/
		}
		memcpy(dns_addr, data_buf + pos, l_hostent.h_length);
#endif //FEATURE_CANONICAL
// answer #1(RR) end
	}
	} while (canonicalCount++<3 && getCanonicalName==TRUE && getIPAddress==FALSE); // DNS Query using Canonical Name
	if (getIPAddress)
	{
		dns_addrList[0] = dns_addr;

		l_hostent.h_name = dns_serverNameSpace;
		l_hostent.h_addrtype = AF_INET;
		l_hostent.h_aliases = NULL;	
		l_hostent.h_addr_list = dns_addrList;

		return &l_hostent;
	}
	else
	{
		return (struct hostentTag* )NULL;
	}
	}
}

#endif	//	#ifdef	FEATURE_SKT_DNS

//////////////////////////////////////////////////////////////////////////////////////////////////
// calls to dss_pppopen() while connection establishment is in progress or is already connected
// results in the application callback function being invoked.
void	sockApi_netCallback
(
	void	*arg1	// dummy arg: we don't use it
)
{
    (void) rex_set_sigs(&sock_tcb, (sint31)SOCK_NET_CB_SIG);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
void	sockApi_socketCallback
(
	void	*arg1	// dummy arg: we don't use it
)
{
    (void) rex_set_sigs(&sock_tcb, (sint31)SOCK_SOCKET_CB_SIG);
}

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
void	sockApi_dnsCallback
(
	void
)
{
	(void) rex_set_sigs(&sock_tcb, (sint31)SOCK_DNS_CB_SIG);
}

sint15	sockApi_getHostEntryByDnsApi
(
	char* serverName
)
{
//	struct hostentTag	l_hostent;	 
	uint32				ipAddr;
	sint31				errno;


	MSG_HIGH("sockApi_getHostEntryByDnsApi()", 0, 0, 0);

	if (*serverName>='0' && *serverName<='9')
	{

		ipAddr = inetAddr(serverName);
		MSG_HIGH("inetAddr 0x%x%x", (int)((ipAddr)>>16), (int)(ipAddr&0xFFFF), 0);
		if(ipAddr != INADDR_NONE) //0xffffffff 
		{
			memcpy(dns_addr, (char *)&ipAddr, 4);
			dns_addrList[0] = dns_addr;
			l_hostent.h_name = NULL;
			l_hostent.h_addrtype = AF_INET;
			l_hostent.h_aliases = NULL;	
			l_hostent.h_addr_list = dns_addrList;
			g_hostEntry = &l_hostent;
			MSG_HIGH("sockApi_getHostEntryByDnsApi() Success ", 0, 0, 0);
			return	(DSS_SUCCESS);
		}
		else
		{
			MSG_HIGH("sockApi_getHostEntryByDnsApi() Error ", 0, 0, 0);
		}
	}
	else
	{
		ds_atcop_qcdns_val[0] = primaryIP;
		ds_atcop_qcdns_val[1] = secondaryIP;	//	dss_gethostaddrs use this.

		errno = dss_gethostaddrs(serverName, &ipAddr, sockApi_dnsCallback);

		if((errno==DS_EWOULDBLOCK) && (sock_wait_sig((long int)PERIOD_10SEC, 
													(long int)(SOCK_CMD_Q_SIG | SOCK_DNS_CB_SIG | SOCK_PS_TIMER_SIG),
													(long int)(SOCK_CMD_Q_SIG | SOCK_DNS_CB_SIG | SOCK_PS_TIMER_SIG)) == DSS_SUCCESS) )
		{
			errno = dss_gethostaddrs(serverName, &ipAddr, sockApi_dnsCallback);
		}

		if (errno==DSS_SUCCESS)
		{
			ipAddr = dss_htonl(ipAddr);

			MSG_HIGH("ipAddr 0x%x%x", (int)((ipAddr)>>16), (int)(ipAddr&0xFFFF), 0);
			if(ipAddr != 0)
			{
				memcpy(dns_addr, (char *)&ipAddr, 4);
				dns_addrList[0] = dns_addr;
				l_hostent.h_name = NULL;
				l_hostent.h_addrtype = AF_INET;
				l_hostent.h_aliases = NULL;	
				l_hostent.h_addr_list = dns_addrList;
				g_hostEntry = &l_hostent;
				MSG_HIGH("sockApi_getHostEntryByDnsApi() Success ", 0, 0, 0);
				return	(DSS_SUCCESS);
			}
			else	//	ipAddr==0
			{
				switch(errno)
				{
				case DS_NAMEERR:
					MSG_ERROR("getHostEntryByDnsApi()>> DS_NAMEERR", 0, 0, 0);
					break;
				case DS_NAMEMISS:
					MSG_ERROR("getHostEntryByDnsApi()>> DS_AMEMISS", 0, 0, 0);
					break;
				case DS_NETERR:
					MSG_ERROR("getHostEntryByDnsApi()>> DS_NETERR", 0, 0, 0);
					break;
				case DS_NOMEMORY:
					MSG_ERROR("getHostEntryByDnsApi()>> DS_NOFMEMORY", 0, 0, 0);
					break;
				case DS_INUSE:
					MSG_ERROR("getHostEntryByDnsApi()>> DS_INUSE", 0, 0, 0);
					break;
				default:
					MSG_ERROR("getHostEntryByDnsApi()>> %d", errno, 0, 0);
				}
			}
		}
	}
	return (DSS_ERROR);
}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

sint15	sockApi_getHostEntry
(
	sint15		appID,
	char*		serverName
)
{
	g_hostEntry = getHostByName(appID, (char*)serverName);
	if (g_hostEntry==NULL)
	{
		return	DSS_ERROR;
	}
	else
	{
		return	DSS_SUCCESS;
	}

}

sint15	sockApi_tcpConnectSub
(
	sint15				appID,
	sint15				socketID,  
	struct hostentTag*	hostentry, 
	uint16				portNo
)
{
	sint15				errno, result;
	sint31				mask;
	struct sockaddr_in	addr;


	memset((char *)&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;

	addr.sin_port   = dss_htons(portNo);

	
	addr.sin_addr.s_addr = (((*hostentry->h_addr_list[0])<<24) & 0xff000000 ) |
						   (((*(hostentry->h_addr_list[0]+1))<<16) & 0x00ff0000 ) |
						   (((*(hostentry->h_addr_list[0]+2))<<8) & 0x0000ff00 ) |
						   (((*(hostentry->h_addr_list[0]+3))) & 0x000000ff );

	MSG_HIGH("sockApi_tcpConnectSub: sin_addr 0x%x%x", (int)((addr.sin_addr.s_addr)>>16), 
									  (int)(addr.sin_addr.s_addr&0xFFFF), 0);

// TEST SKTELECOM.COM
//addr.sin_addr.s_addr = 0xcbec0114;
// user.chollian.net
//addr.sin_addr.s_addr = 0xcbfc0519;

	addr.sin_addr.s_addr = dss_htonl(addr.sin_addr.s_addr); 


	result = dss_connect(socketID, (struct sockaddr *)&addr, 
						 (uint16)sizeof(struct sockaddr), &errno);

	if (errno == DS_EWOULDBLOCK) 
	{
		if(dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
//		if(dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT | DS_READ_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("sockApi_tcpConnectSub: DSS CONNECT ERR %d", errno, 0, 0);
			return(DSS_ERROR);
		}

		if(sock_wait_sig((long int)PERIOD_10SEC, 
						(long int)(SOCK_CMD_Q_SIG | SOCK_NET_CB_SIG  | SOCK_SOCKET_CB_SIG | SOCK_PS_TIMER_SIG),
								 (long int)(SOCK_CMD_Q_SIG | SOCK_PS_TIMER_SIG | SOCK_SOCKET_CB_SIG)) == DSS_ERROR)
		{
			return (DSS_ERROR);
		}

		mask = dss_getnextevent(appID, &socketID, &errno);

		if (mask & DS_WRITE_EVENT) 
		{
			MSG_HIGH("sockApi_tcpConnectSub: DS_WRITE_EVENT get write mask!!", 0, 0, 0);
			result = dss_connect(socketID, (struct sockaddr *)&addr, 
								 (uint16)sizeof(struct sockaddr), &errno);

			if (result==DSS_SUCCESS || errno==DS_EISCONN)
			{
//				char	cmd[100];  
//				if (mask & DS_READ_EVENT) 
//				{
//					MSG_HIGH("sockApi_tcpConnectSub: DS_READ_EVENT get write mask!!", 0, 0, 0);
//					result = 0;
//					while( (result=sockApi_recvTCPDataOnce(appID, socketID, cmd, 100) ) > 0) continue ;
////					if(result < 0) return(result) ;
//				}
				MSG_HIGH("sockApi_tcpConnectSub: success!!", 0, 0, 0);		

				return DSS_SUCCESS;	
			}
			else
			{
				switch (errno) 
				{
					case DS_EWOULDBLOCK:
						MSG_ERROR("sockApi_tcpConnectSub: operation would block", 0, 0, 0);
						break;
					case DS_EBADF:
						MSG_ERROR("sockApi_tcpConnectSub: invalid socket descriptor is specfied", 0, 0, 0);
						break;
					case DS_ECONNREFUSED:
						MSG_ERROR("sockApi_tcpConnectSub: connection attempt refused", 0, 0, 0);
						break;
					case DS_ETIMEDOUT:
						MSG_ERROR("sockApi_tcpConnectSub: connection attempt timed out", 0, 0, 0);
						break;
					case DS_EFAULT:
						MSG_ERROR("sockApi_tcpConnectSub: addrlen parameter is invalid", 0, 0, 0);
						break;
					case DS_EIPADDRCHANGED:
						MSG_ERROR("sockApi_tcpConnectSub: IP address changed due to PPP resync", 0, 0, 0);
						break;
					case DS_EINPROGRESS:
						MSG_ERROR("sockApi_tcpConnectSub: connection establishment in progress", 0, 0, 0);
						break;
					case DS_EISCONN:
						MSG_HIGH("a socket descriptor is specified that is already connected", 0, 0, 0);
						return (sint15)socketID;
					case DS_ENETDOWN:
						MSG_ERROR("sockApi_tcpConnectSub:: network subsystem unavailable", 0, 0, 0);
						break;
					case DS_EOPNOTSUPP:
						MSG_ERROR("sockApi_tcpConnectSub: invalid server address specified", 0, 0, 0);
						break;
					case DS_EADDRREQ:
						MSG_ERROR("sockApi_tcpConnectSub: destination address is required.", 0, 0, 0);
						break;
				}
				return DSS_ERROR;
			}
		}
	}
	MSG_HIGH("sockApi_tcpConnectSub: failed!!", 0, 0, 0);		
	if (result==DSS_SUCCESS || errno==DS_EISCONN)
	{
		return	DSS_SUCCESS;
	}
	else
	{
		return	DSS_ERROR;
	}
}


sint15	sockApi_tcpConnect
(
	sint15				appID,
	struct hostentTag*	hostEntry,
	uint16				portNo
)
{
	sint15	socketID;
	
	if ((socketID = sockApi_socketOpen(appID, (byte)AF_INET, (byte)SOCK_STREAM, (byte)IPPROTO_TCP)) == 
															DSS_ERROR) 
	{
		MSG_HIGH("sockApi_tcpConnect: Socket -1", 0, 0, 0);
		return DSS_ERROR;
	}
	if (socketID<0x1000 || socketID>0x1FFF) 
	{
		MSG_HIGH("sockApi_tcpConnect: Socket Err %d", socketID, 0, 0);
		sockApi_tcpClose(socketID);
		return DSS_ERROR;
	}

	if(sockApi_tcpConnectSub(appID, socketID, hostEntry, portNo)==DSS_ERROR)
	{	
		MSG_HIGH("sockApi_tcpConnect: connection error", 0, 0, 0);
		sockApi_tcpClose(socketID);
		MSG_HIGH("sockApi_tcpConnect: disconnect", 0, 0, 0);
		return DSS_ERROR;
	}

	MSG_HIGH("sockApi_tcpConnect success 0x%x", socketID, 0, 0);
	return socketID;
}	


void	sockApi_enableSendEvent
(
	sint15	socketID
)
{
	sint15	errno;

	if (dss_async_select((sint15)socketID, (sint31)DS_WRITE_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("sockApi_enableSendEvent Err %d", errno, 0, 0);
	}
}

void	sockApi_enableRecvEvent
(
	sint15	socketID
)
{
	sint15	errno;

	if (dss_async_select((sint15)socketID, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
	{
		MSG_ERROR("sockApi_enableRecvEvent Err %d", errno, 0, 0);
	}
}

void	sockApi_sendTCPData
(
	sint15	appID,
	sint15	socketID
)
{
	dsm_item_type	*item_ptr, *next_item_ptr;
	byte			data[DSM_DS_LARGE_ITEM_SIZ];
	sint15			result;

	while(( item_ptr = (dsm_item_type *)q_get(&sockSendDataQ)) != NULL)
	{
		result = sockApi_sendTCPDatabyEvent(appID, socketID, (byte *)item_ptr->data_ptr, (int)item_ptr->used);

		if (result<item_ptr->used)
		{
			if (result>0)
			{	
				dsm_pullup(&item_ptr, data, result);
			}
			next_item_ptr = (dsm_item_type *)q_check( &sockSendDataQ);
			if (next_item_ptr==NULL)
			{
				q_put( &sockSendDataQ, &item_ptr->link);
			}
			else
			{
				#ifdef FEATURE_Q_NO_SELF_QPTR
					q_insert( &sockSendDataQ, &item_ptr->link, &next_item_ptr->link);
				#else
					q_insert(  &item_ptr->link, &next_item_ptr->link);
				#endif
			}
			sockApi_enableSendEvent(socketID);
			break;
		}
		else
		{
			dsm_free_packet(&item_ptr);
		}
	}
}

void	sockApi_recvTCPData
(
	sint15	appID,
	sint15	socketID
)
{
	byte			data[5*DSM_DS_SMALL_ITEM_SIZ];
	sint15			result;

	result = sockApi_recvTCPDatabyEvent(appID, socketID, data, 5*DSM_DS_SMALL_ITEM_SIZ);
	if (result>0)
	{
		SvrLinkRcvDataToUi((byte *)data, (byte)result);
	}
	else
	{
		MSG_ERROR("sockApi_recvTCPData]ABNORMAL Release",0,0,0);
		sockApi_setNetState(SOCK_CLOSING_S);
		sockApi_closeNetwork(oldSockNetState);
	}
}


void	sockApi_sendUDPData
(
	sint15	appID,
	sint15	socketID,
	uint16	flags,
	struct sockaddr*	to,
	uint16	tolen
)
{
	dsm_item_type	*item_ptr, *next_item_ptr;
	byte			data[DSM_DS_LARGE_ITEM_SIZ];
	sint15			result;

	while(( item_ptr = (dsm_item_type *)q_get(&sockSendDataQ)) != NULL)
	{
		result = sockApi_sendUDPDatabyEvent(appID, socketID, (byte *)item_ptr->data_ptr, (uint16)item_ptr->used, flags, to, tolen);

		if (result<item_ptr->used)
		{
			if (result>0)
			{	
				dsm_pullup(&item_ptr, data, result);
			}
			next_item_ptr = (dsm_item_type *)q_check( &sockSendDataQ);
			if (next_item_ptr==NULL)
			{
				q_put( &sockSendDataQ, &item_ptr->link);
			}
			else
			{
				#ifdef FEATURE_Q_NO_SELF_QPTR
					q_insert( &sockSendDataQ, &item_ptr->link, &next_item_ptr->link);
				#else
					q_insert(  &item_ptr->link, &next_item_ptr->link);
				#endif
			}
			sockApi_enableSendEvent(socketID);
			break;
		}
		else
		{
			dsm_free_packet(&item_ptr);
		}
	}
}


void	sockApi_recvUDPData
(
	sint15	appID,
	sint15	socketID, 
	uint16	flags, 
	struct sockaddr* from,
	uint16*	fromlen
)
{
	byte			data[5*DSM_DS_SMALL_ITEM_SIZ];
	sint15			result;

	result = sockApi_recvUDPDatabyEvent(appID, socketID, data, 5*DSM_DS_SMALL_ITEM_SIZ, flags, from, fromlen);
	if (result>0)
	{
		SvrLinkRcvDataToUi((byte *)data, (byte)result);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERNAL FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void	sockApi_openPPPNetwork
(
	void
)
{
	sint15 errno;

	if (sockNetState!=SOCK_NULL_S)
	{
		MSG_ERROR("sockApi_openNetwork: sockNetState not SOCK_NULL_S", 0, 0, 0);
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_NULL_S
	sockApi_setNetState(SOCK_NULL_S);	

	// dss_open_netlib
	g_appID = dss_open_netlib(&sockApi_netCallback, &sockApi_socketCallback, &errno);

	if (errno!=DSS_SUCCESS)	
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult((sint15)DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_PPPOPENING_S
	sockApi_setNetState(SOCK_PPPOPENING_S);	
	
	// dss_pppopen
	if (sockApi_pppOpen(g_appID)==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult((sint15)DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_PPPOPEN_S
	sockApi_setNetState(SOCK_PPPOPEN_S);	

	// SOCET=>UI PPP CONNECT SUCCESS
	SvrLinkResult((sint15)DSS_SUCCESS);
}


void	sockApi_openTCPNetwork
(
	char*	serverName,
	uint16	portNo
)
{
	sint15 errno;

	if (sockNetState!=SOCK_NULL_S)
	{
		MSG_ERROR("sockApi_openNetworkTcp: sockNetState not SOCK_NULL_S", 0, 0, 0);
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_NULL_S
	sockApi_setNetState(SOCK_NULL_S);	

	// dss_open_netlib
	g_appID = dss_open_netlib(&sockApi_netCallback, &sockApi_socketCallback, &errno);

	if (errno!=DSS_SUCCESS)	
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_PPPOPENING_S
	sockApi_setNetState(SOCK_PPPOPENING_S);	
	
	// dss_pppopen
	if (sockApi_pppOpen(g_appID)==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_PPPOPEN_S
	sockApi_setNetState(SOCK_PPPOPEN_S);	

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
	if (sockApi_getHostEntryByDnsApi(serverName)==DSS_ERROR)
	{
		// sockApi_closeNetwork(sockNetState) is not processed
		SvrLinkResult(DSS_ERROR);
		return;
	}
#else	//	#ifdef	FEATURE_SOCKET_DNS_API
	//	sockNetState==SOCK_UDPOPENING_S
	sockApi_setNetState(SOCK_UDPSOCK_OPENING_S);	

	if (sockApi_getHostEntry(g_appID, serverName)==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

	g_socketID = DSS_ERROR;

	//	sockNetState==SOCK_TCPSOCK_OPENING_S
	sockApi_setNetState(SOCK_TCPSOCK_OPENING_S);

	if ((g_socketID=sockApi_tcpConnect(g_appID, g_hostEntry, portNo))==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_TCPSOCK_OPEN_S
	sockApi_setNetState(SOCK_TCPSOCK_OPEN_S);

	//	enable DS_READ_EVENT
	sockApi_enableRecvEvent(g_socketID);	

	// SOCET=>UI CONNECT SUCCESS
	SvrLinkResult(DSS_SUCCESS);
}


void	sockApi_reopenTCPNetwork
(
	char*	serverName,
	uint16	portNo
)
{
	// sockNetState should be SOCK_TCPSOCK_OPEN_S
	if (sockNetState!=SOCK_TCPSOCK_OPEN_S)
	{
		MSG_ERROR("sockApi_openNetworkTcp: sockNetState not SOCK_NULL_S", 0, 0, 0);
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_TCPSOCK_REOPEN_S
	sockApi_setNetState(SOCK_TCPSOCK_REOPENING_S);

	// close existing tcp socket
	sockApi_closeNetwork(sockNetState);

	// While closing one TCP for reconnecting the other TCP, process SOCK_RELEASE_CMD from UI
	if (g_bReleased==TRUE)
	{
		sockApi_setNetState(SOCK_TCPSOCK_OPEN_S);
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_PPPOPEN_S
	sockApi_setNetState(SOCK_PPPOPEN_S);

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
	if (sockApi_getHostEntryByDnsApi(serverName)==DSS_ERROR)
	{
		// sockApi_closeNetwork(sockNetState) is not processed
		SvrLinkResult(DSS_ERROR);
		return;
	}
#else	//	#ifdef	FEATURE_SOCKET_DNS_API
	//	sockNetState==SOCK_UDPOPEN_S
	sockApi_setNetState(SOCK_UDPSOCK_OPENING_S);	
	// get HostEntry
	if (sockApi_getHostEntry(g_appID, serverName)==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

	g_socketID = DSS_ERROR;

	//	sockNetState==SOCK_TCPSOCK_OPEN_S
	sockApi_setNetState(SOCK_TCPSOCK_OPENING_S);
	// connect tcp
	if ((g_socketID=sockApi_tcpConnect(g_appID, g_hostEntry, portNo))==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_TCPSOCK_OPEN_S
	sockApi_setNetState(SOCK_TCPSOCK_OPEN_S);

	//	enable DS_READ_EVENT
	sockApi_enableRecvEvent(g_socketID);	

	// SOCET=>UI CONNECT SUCCESS
	SvrLinkResult(DSS_SUCCESS);
}


void	sockApi_openUDPNetwork
(
	char*	serverName,
	uint16	portNo
)
{
	sint15 errno;

	if (sockNetState!=SOCK_NULL_S)
	{
		MSG_ERROR("sockApi_openNetworkTcp: sockNetState not SOCK_NULL_S", 0, 0, 0);
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_NULL_S
	sockApi_setNetState(SOCK_NULL_S);	

	// dss_open_netlib
	g_appID = dss_open_netlib(&sockApi_netCallback, &sockApi_socketCallback, &errno);

	if (errno!=DSS_SUCCESS)	
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_PPPOPENING_S
	sockApi_setNetState(SOCK_PPPOPENING_S);	
	
	// dss_pppopen
	if (sockApi_pppOpen(g_appID)==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	//	sockNetState==SOCK_PPPOPEN_S
	sockApi_setNetState(SOCK_PPPOPEN_S);	

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
	if (sockApi_getHostEntryByDnsApi(serverName)==DSS_ERROR)
	{
		// sockApi_closeNetwork(sockNetState) is not processed
		SvrLinkResult(DSS_ERROR);
		return;
	}
#else	//	#ifdef	FEATURE_SOCKET_DNS_API
	//	sockNetState==SOCK_UDPOPENING_S
	sockApi_setNetState(SOCK_UDPSOCK_OPENING_S);	
	if (sockApi_getHostEntry(g_appID, serverName)==DSS_ERROR)
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}
#endif	//	#ifndef	FEATURE_SOCKET_DNS_API

	g_socketID = DSS_ERROR;

	//	sockNetState==SOCK_UDPSOCK_OPENING_S
	sockApi_setNetState(SOCK_UDPSOCK_OPENING_S);

	if ((g_socketID=sockApi_socketOpen(g_appID, (byte)AF_INET, (byte)SOCK_DGRAM, (byte)IPPROTO_UDP))
			== DSS_ERROR) 
	{
		sockApi_closeNetwork(sockNetState);
		SvrLinkResult(DSS_ERROR);
		return;
	}

	{
		sUdpHostAddr.sin_family = AF_INET;
		sUdpHostAddr.sin_port   = dss_htons(portNo);	
		sUdpHostAddr.sin_addr.s_addr = (((*g_hostEntry->h_addr_list[0])<<24) & 0xff000000 ) |
									   (((*(g_hostEntry->h_addr_list[0]+1))<<16) & 0x00ff0000 ) |
									   (((*(g_hostEntry->h_addr_list[0]+2))<<8) & 0x0000ff00 ) |
									   (((*(g_hostEntry->h_addr_list[0]+3))) & 0x000000ff );

		sUdpHostAddr.sin_addr.s_addr = dss_htonl(sUdpHostAddr.sin_addr.s_addr); 
		g_udpHostAddr = &sUdpHostAddr;
		g_sizeofudpHostAddr = sizeof(struct sockaddr);
	}


	//	sockNetState==SOCK_UDPSOCK_OPEN_S
	sockApi_setNetState(SOCK_UDPSOCK_OPEN_S);

	//	enable DS_READ_EVENT
	sockApi_enableRecvEvent(g_socketID);	

	// SOCET=>UI CONNECT SUCCESS
	SvrLinkResult(DSS_SUCCESS);
}


void	sockApi_closeNetwork
(
	SOCK_NETSTATE	closedSockNetState
)
{
	sint15 state = DSS_SUCCESS, errno;

	MSG_HIGH("sockApi_closeNetwork: state%d cur%d", closedSockNetState, sockNetState, 0);

	switch (closedSockNetState)
	{
		case	SOCK_NULL_S:
			if (g_appID!=DSS_ERROR)
			{
				state = dss_close_netlib(g_appID, &errno);
			}
			break;
		case	SOCK_PPPOPENING_S:
// sockApi_pppOpen(g_appIP) may be processing.
//			state = dss_close_netlib(g_appID, &errno);
//			break;
		case	SOCK_PPPOPEN_S:
			sockApi_pppClose(g_appID); 
			state = dss_close_netlib(g_appID, &errno);
			break;
		case	SOCK_UDPSOCK_OPENING_S:
		case	SOCK_UDPSOCK_OPEN_S:
			if (g_socketID != DSS_ERROR)
			{
				sockApi_udpClose(g_socketID); 
			}
			sockApi_pppClose(g_appID); 
			state = dss_close_netlib(g_appID, &errno);
			break;
		case	SOCK_TCPSOCK_OPENING_S:
		case	SOCK_TCPSOCK_OPEN_S:
			if (g_socketID != DSS_ERROR)
			{
				sockApi_tcpClose(g_socketID); 
			}
			sockApi_pppClose(g_appID); 
			state = dss_close_netlib(g_appID, &errno);
			break;
		case	SOCK_TCPSOCK_REOPENING_S:
			if (g_socketID != DSS_ERROR)
			{
				sockApi_tcpClose(g_socketID); 
			}
			return;	//	return immediately
		default:
			MSG_ERROR("sockApi_closeNetwork: Invalid closedSockNetState%d", closedSockNetState, 0, 0);
	}
	
	if (state==DSS_ERROR)
	{
		switch(errno)
		{
			case DS_EBADAPP:
				MSG_HIGH("sockApi_closeNetwork : Bad App", 0, 0, 0);
				state = dss_close_netlib(g_appID, &errno);
				break;
			case DS_SOCKEXIST:
				MSG_HIGH("sockApi_closeNetwork : sock exist", 0, 0, 0);
				sockApi_tcpClose(g_socketID);
				sockApi_pppClose(g_appID);
				state = dss_close_netlib(g_appID, &errno);
				break;
			case DS_ENETEXIST:
				MSG_HIGH("sockApi_closeNetwork : Net exist", 0, 0, 0);
				sockApi_pppClose(g_appID);
				state = dss_close_netlib(g_appID, &errno);
				break;
		}
	}

	sockApi_setNetState(SOCK_NULL_S);
}	

void	sockApi_closeNetworkbyReleaseCmd
(
	void
)
{
	MSG_HIGH("sockApi_closeNetworkbyReleaseCmd old%d cur%d", oldSockNetState, SOCK_CLOSING_S, 0);

	g_bReleased = TRUE;		//	Released by UI
	sockApi_setNetState(SOCK_CLOSING_S);
	sockApi_closeNetwork(oldSockNetState);
}

void	sockApi_sendData
(
	void
)
{
	if (sockNetState==SOCK_UDPSOCK_OPEN_S)
	{
		sockApi_sendUDPData(g_appID, g_socketID, 0, (struct sockaddr *)g_udpHostAddr, g_sizeofudpHostAddr);
	}
	else if (sockNetState==SOCK_TCPSOCK_OPEN_S)
	{
		sockApi_sendTCPData(g_appID, g_socketID);
	}
}

void	sockApi_recvData
(
	void
)
{
	if (sockNetState==SOCK_UDPSOCK_OPEN_S)
	{
		sockApi_recvUDPData(g_appID, g_socketID, 0, (struct sockaddr *)g_udpHostAddr, &g_sizeofudpHostAddr);
	}
	else if (sockNetState==SOCK_TCPSOCK_OPEN_S)
	{
		sockApi_recvTCPData(g_appID, g_socketID);
	}
}


void	sockApi_processSockCallBackFn
(
	void
)
{
	sint15		errno;
	sint31		mask;

	if (sockNetState==SOCK_UDPSOCK_OPEN_S || sockNetState==SOCK_TCPSOCK_OPEN_S)
	{
		mask = dss_getnextevent(g_appID, &g_socketID, &errno);
		if (mask & DS_WRITE_EVENT)
		{
			MSG_HIGH("SOCKET_CB_SIG: DS_WRITE_EVENT", 0, 0, 0);
			sockApi_sendData();
		}
		if (mask & DS_READ_EVENT)
		{
			MSG_HIGH("SOCKET_CB_SIG: DS_READ_EVENT", 0, 0, 0);
			sockApi_recvData();
			sockApi_enableRecvEvent(g_socketID);	
		}
	}
}


void	sockApi_processNetCallBackFn
(
	void
)
{
	// NOT DEFINED 
}

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14

void	sockApi_processDnsCallBackFn
(
	void
)
{
}
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API


#endif	//	#ifdef	FEATURE_SKT_SOCKAPI