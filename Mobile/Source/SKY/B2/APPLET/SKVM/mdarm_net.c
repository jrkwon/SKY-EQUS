/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
		MDARM_NET.C
GENERAL DESCRIPTION
  This Software unit contains all networking functions for XVM engine.
  This includes all REX Interfacing functions and XVM engine 
  Interfacing functions.
  
EXTERNALIZED FUNCTIONS
	
INITIALIZATION AND SEQUENCING REQUIREMENTS

   Copyright (c) 2001 by SK Teletech Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/******************************************************************/
/*	   include Header Files				                          */
/******************************************************************/

#include "extern.h" 
#include "sky.h"
#include "ska.h" 
#include "mdarm_def.h"
#include "sky_sock.h"
#include "dsi.h"

/******************************************************************/
/*	   define constans and macro		                          */
/******************************************************************/

#define	USE_SKTSOCK_O
//#define	USE_MDSOCK_O

#ifdef	USE_SKTSOCK_O
/******************************************************************/
/*	   grobal variables					                          */
/******************************************************************/
unsigned long	g_lpreservetime = 0;
int				g_WouldBlock = DSS_ERROR;
BOOL			g_bsetppptimer = FALSE;
BOOL			g_bWouldBlockTimer = FALSE;
int				g_iSocketID = NULL_ID;

/******************************************************************/
/*	   extern functions					                          */
/******************************************************************/
extern void xvm_set_md_ppp_preserve_time(unsigned long t);
extern void xvm_cancel_md_ppp_preserve_time();

#ifdef WAPDORMANT
extern sint15 Dormant_Reorigination(sint15 *errno);
#endif

int md_pppopen(void)
{
	int ret = 0;
	short  err;

	MSG_ERROR("md_pppopen", 0,0,0); 

	ret = g_WouldBlock = PPPOPEN();
	

	if(ret == DSS_ERROR)
	{
		if(NetworkGetState() == NET_PPPOPENING_S){
			MSG_ERROR("md_socket => ppp_open wouldblock! ", 0,0,0);	
			g_WouldBlock = DS_EWOULDBLOCK;
#ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK
			md_sleep(50);
#endif // #ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK
			g_bWouldBlockTimer = TRUE;
			return 0;
		}
		else{
			MSG_ERROR("md_socket => ppp_open error occurred! ",0,0,0);	
			return -1;
		}
	}


	if (ret == DSS_SUCCESS)
	{
#ifdef SUPPORT_DORMANT_IN_SKVM
		if (SKVM_IS_DORMANT())
		{
			ret = g_WouldBlock = Dormant_Reorigination(&err);
			if(ret == DSS_ERROR && err == DS_EWOULDBLOCK)
			{
				MSG_ERROR("reorigination progressing",0,0,0);
				g_WouldBlock = DS_EWOULDBLOCK;
#ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK
				md_sleep(50);
#endif // #ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK
				g_bWouldBlockTimer = TRUE;
				ret = 0;
			}
			else if(ret == DSS_SUCCESS)
			{
				SKVM_SET_DORMANT(FALSE);
				MSG_HIGH("reorigination done",0,0,0);
				ret = 1;
			}
			else
			{
//				SKVM_SET_DORMANT(FALSE);
				MSG_ERROR("Dormant Reorigination fail",0,0,0);
				ret = -1;
			}
		}
		else 
#endif // SUPPORT_DORMANT_IN_SKVM
			ret = 1;
	}
	else ret = -1;

	return ret;

}

int md_pppclose(void)
{
	int ret = 0;
	MSG_ERROR("md_pppclose", 0,0,0); 

	ret = g_WouldBlock = PPPCLOSE_VM();

	if(ret == DSS_ERROR)
	{
		if(NetworkGetState() == NET_CLOSING_S){
			MSG_ERROR("md_pppclose => ppp_close wouldblock! ", 0,0,0);	
			g_WouldBlock = DS_EWOULDBLOCK;
#ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK
			md_sleep(50);
#endif //#ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK
			g_bWouldBlockTimer = TRUE;
			return 0;
		}
		else{
			MSG_ERROR("md_pppclose => ppp_close error occurred! ",0,0,0);	
			g_WouldBlock = DSS_ERROR;
			return -1;
		}
	}

	if (ret == DSS_SUCCESS)
		ret = 1;
	else ret = -1;

	return ret;

}

void md_set_ppp_preserve_time(unsigned long t)
{

	MSG_ERROR("md_set_ppp_preserve_time", 0,0,0); 
	// ppp open 이후 t milliseconds 동안 socket open이 없으면 
	// 임의로 ppp close를 시행하기 위함.
	// 열려진 socket없이 ppp를 유지시켜줘야 하는 timeout 시간임. 
	// 따라서 socket close시점 부터 countdown을 시작하여 socket open(creat)시 초기화함.
	// 만약 countdown을 해서 0가 되면 자동적으로 ppp close시킴.
	// 입력값 t가 0인 경우는 무한대를 의미함.
	g_lpreservetime = t;
	g_bsetppptimer = FALSE;
}

void md_initsocket(void)
{
	g_WouldBlock = DSS_ERROR;
	g_lpreservetime = 0;
	g_bsetppptimer = FALSE;
}

void md_finalizesocket(void)
{
	MSG_ERROR("md_finalizesocket", 0,0,0); 
    if (SKVM_GET_SOCKETID() != NULL_ID)
		md_closesocket(SKVM_GET_SOCKETID());
	SKVM_SET_SOCKETID(NULL_ID);
	g_WouldBlock = DSS_ERROR;
	g_lpreservetime = 0;
	g_bsetppptimer = FALSE;
	g_bWouldBlockTimer = FALSE;
}

int md_accept(int fd)
{
	MSG_ERROR("md_accept", 0,0,0); 
    return -1;
}

int md_bind(int fd, int port)
{
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;
	int		 ret;


	g_WouldBlock = DSS_ERROR;
	

	memset((char *)&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family      = AF_INET;        
	addr.sin_port        = dss_htons(port);
	addr.sin_addr.s_addr = 0; 

	MSG_ERROR("md_bind %d %d", fd,port,0); 

	ret = bind(fd, (struct sockaddr *) &addr, sizeof(addr) );
	
	if (ret == DSS_SUCCESS)
		ret = 1;
	else ret = -1;


	return ret;

}

int md_closesocket (int fd)
{
	int ret; 
		
	g_WouldBlock = DSS_ERROR;

	ret = closesocket(fd);
	
	if (ret == DSS_SUCCESS)
	{
		SKVM_SET_SOCKETID(NULL_ID);
		ret = 1;
	}
	else ret = -1;

	MSG_ERROR("md_closesocket %d %d", fd,ret,0); 	

	if (NetworkGetState() == NET_PPPOPEN_S)
	{
		if (g_lpreservetime)
		{
			g_bsetppptimer = TRUE;
			xvm_set_md_ppp_preserve_time(g_lpreservetime);			
		}
	}
	return ret;
	
}

int md_connect(int fd, char *hostname, int port)
{
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;
	int		ret;
	g_WouldBlock = DSS_ERROR;


	
	hostentry = gethostbyname(hostname);
	if(hostentry == NULL)
	{
		MSG_ERROR("md_connect => hostentry error!!!", 0,0,0); 
		return -1;
	}

	memset((char *)&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family      = AF_INET;        
	addr.sin_port        = dss_htons(port);

#if 1
    addr.sin_addr.s_addr = get32( (unsigned char *)(hostentry->h_addr_list[0]) );
#else
	memcpy( &addr.sin_addr, hostentry->h_addr_list[0], sizeof(struct in_addr) );
#endif

	MSG_ERROR("REMOTE ADDRESS [0x%x%x] %d", (int)((addr.sin_addr.s_addr)>>16), 
									    (int)addr.sin_addr.s_addr, ret);

	addr.sin_addr.s_addr = dss_htonl(addr.sin_addr.s_addr); 
	
	ret = connect(fd,(const struct sockaddr *)&addr, sizeof(struct sockaddr));
	
	if (ret == DSS_SUCCESS)
		ret = 1;
	else ret = -1;

	MSG_ERROR("md_connect %d %d %d", fd,port,ret); 
	return ret;
}

int md_listen(int fd)
{
	MSG_ERROR("md_listen", 0,0,0); 
    return -1;
}

int md_socketread (int fd, void *rbuf, int len)
{
	int ret;

	g_WouldBlock = DSS_ERROR;

	ret = recv(fd,(char*) rbuf,(int) len,(int) 0);
	if (ret < 0 ) 
		ret = -1;

	MSG_ERROR("md_socketread %d %d %d", fd,len,ret); 

	return ret;
}

int md_socket(int stream)
{
	int ret = DSS_SUCCESS;

	if (g_bsetppptimer)
	{
		g_bsetppptimer = FALSE;
		xvm_cancel_md_ppp_preserve_time();
	}

	g_WouldBlock = DSS_ERROR;

	ret = md_pppopen();

	if (ret == 1){
		if (stream == 1)  // 1인경우
			ret = socket((byte)AF_INET,(byte)SOCK_STREAM,(byte)IPPROTO_TCP);
		else ret = socket((byte)AF_INET,(byte)SOCK_DGRAM,(byte)IPPROTO_UDP);
		SKVM_SET_SOCKETID(ret);
	}
	else if (ret == 0)
	{
		g_WouldBlock	= DS_EWOULDBLOCK;
		SKVM_SET_SOCKETID(NULL_ID);
	}

	return ret;
}

int md_socketwrite(int fd, void *wbuf, int len)
{
	int ret;

	g_WouldBlock = DSS_ERROR;

	ret = send(fd,(const char*) wbuf,len,0);
	
	if (ret < 0 ) 
		ret = -1;

	MSG_ERROR("md_socketwrite %d %d %d", fd,len,ret); 
	
	return ret;
}

int md_socketavailable(int fd)
{
	int ret = 0;

	g_WouldBlock = DSS_ERROR;
	
	if (dsi_get_callstate() == DS_CALL_ACTIVE_CSTATE)
	{
		ret = CheckSocketState(fd);
			
		if (ret == 1)
		{
			EnableReadEvent(fd);
			MSG_ERROR("read avail %d",fd,0,0);
		}
		else if(ret ==2)
		{
			MSG_ERROR("write avail %d",fd,0,0);
			ret = 0;
		}else if(ret ==3)
		{
			MSG_ERROR("close avail %d",fd,0,0);
			ret = -1;
		}else ret = 0;
	}
	else ret = -1;

	if (ret == 0){
		g_WouldBlock = DS_EWOULDBLOCK;
#ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK
		md_sleep(50);
#else 
		g_bWouldBlockTimer = TRUE;
#endif //#ifdef SKVM_USE_SLEEP_TIMER_IN_NETWORK

	}
	else g_WouldBlock = DSS_SUCCESS;
	return ret;

}

int md_recvfrom(int fd, char *from, int fromlen, void *data, int len)
{
	MSG_ERROR("md_recvfrom", 0,0,0); 
	g_WouldBlock = DSS_ERROR;

	return recvfrom(fd,(char *)data,len,0,(struct sockaddr *)&from,(int*)&fromlen);                
}

int md_sendto(int fd, char *hostname, int port, void *buf, int len)
{
	struct   hostent *hostentry = NULL ;
	struct   sockaddr_in addr  ;

	g_WouldBlock = DSS_ERROR;
	MSG_ERROR("md_sendto", 0,0,0); 
   	hostentry = gethostbyname(hostname);
	if(hostentry == NULL)
	{
		return -1;
	}

	memset((char *)&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family      = AF_INET;        
	addr.sin_port        = dss_htons(port);
	memcpy( &addr.sin_addr, hostentry->h_addr_list[0], sizeof(struct in_addr) );
	addr.sin_addr.s_addr = dss_htonl(addr.sin_addr.s_addr); 
	MSG_ERROR("REMOTE ADDRESS [0x%x%x]", (int)((addr.sin_addr.s_addr)>>16), 
									    (int)addr.sin_addr.s_addr, 0);

	return sendto( fd,(const char*)buf,len,0,(struct sockaddr*) &addr,sizeof(struct sockaddr));
}
#endif