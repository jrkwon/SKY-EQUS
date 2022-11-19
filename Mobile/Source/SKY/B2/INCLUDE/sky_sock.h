/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_sock.c
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


#ifndef __dssock_h__
#define  __dssock_h__

//socket.c
#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "dssocket.h"
#include "msg.h"
#include "err.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct hostent {
    char*   h_name;
    char**	h_aliases;
    short   h_addrtype;
    short   h_length;
    char**	h_addr_list;
};

typedef unsigned int	SOCKET;

#define INVALID_SOCKET	        (unsigned int)(~0)
#define SOCKET_ERROR				          (-1)
#define INADDR_NONE				0xffffffff


typedef enum 
{
	NET_NULL_S = 0,
	NET_PPPOPENING_S,
	NET_PPPOPEN_S,
	NET_CLOSING_S,
	NET_CLOSED_S
} NETSTATE;

//comm.c
#define MAX_REPLY_LENGTH	256
#define CRLF	"\r\n"

typedef struct tagReplyStack
{
	char reply[MAX_REPLY_LENGTH + 1];
	int  npos;
} ReplyStack;

extern ReplyStack reply_stack;

typedef enum {
	NOSVC = 0,
	WAP ,
	JAVA,
	GVM ,
	WAVELET,
	VAS,
	MAX
} service_name_type;

typedef struct  
{
	service_name_type    KindOfService;  //service kind
} VASSTATE_T;

extern VASSTATE_T gVasState;


#define GET_SERVICE_STATE               (  gVasState.KindOfService           )
#define SET_SERVICE_STATE( fValue )     (  gVasState.KindOfService = fValue  )

#define HI_BYTE(word)	((unsigned char)(((word)>>8) & (0x00ff)))
#define LO_BYTE(word)	((unsigned char)((word) & (0x00ff)))
#define MAKE_HALF(h, l) ((unsigned short)((((h)&0x00ff)<<8) | ((l)&0x00ff)))
#define MAKE_LONG(uh,ul,lh,ll) ((unsigned long)( ((uh)<<24) | ((ul)<<16) | \
												 ((lh)<< 8) | ((ll)) ) )

#define MUPPER_BYTE(exp)         ((unsigned char)(((exp)>>24) & (0x000000ff)))
#define MLOWER_BYTE(exp)		 ((unsigned char)(((exp)>>16) & (0x000000ff)))
#define LUPPER_BYTE(exp)		 ((unsigned char)(((exp)>> 8) & (0x000000ff)))
#define LLOWER_BYTE(exp)		 ((unsigned char)(((exp)    ) & (0x000000ff)))

#ifdef WIN32
	#define DPRINTF(msg)	printf msg
#else
	#define DPRINTF(msg)	
#endif//win32


extern void SetKindOfService(int Service);
extern int GetKindOfService(void);

extern int connect(
	SOCKET         s,  
	const struct sockaddr *name, 
	int            namelen
);

extern sint15 disconnect(SOCKET s);

extern SOCKET socket (
  int af,       
  int type,     
  int protocol  
);

extern int closesocket (
  SOCKET s  
);

extern int send (
  SOCKET s,              
  const char * buf,  
  int len,               
  int flags              
);

extern int recv (
  SOCKET s,       
  char*  buf,  
  int len,        
  int flags       
);

extern int sendto (
  SOCKET s,                        
  const char* buf,            
  int len,                         
  int flags,                       
  struct sockaddr* to,  
  int tolen                        
);

extern int recvfrom (
  SOCKET s,                   
  char* buf,              
  int len,                    
  int flags,                  
  struct sockaddr * from,  
  int* fromlen            
);

extern int bind (
  SOCKET s,                          
  struct sockaddr *  name,  
  int namelen                        
);

extern struct hostent * gethostbyname (
  const char * name  
);

extern unsigned long inet_addr (
  const char * cp  
);

extern const char* getserver(void);

extern sint15 PPPOPEN (void);
extern sint15 PPPCLOSE(void);
extern void NetworkSetState(NETSTATE netState);
extern NETSTATE NetworkGetState(void);
extern int CheckSocketState(SOCKET s);

extern void	EnableWriteEvent(SOCKET	socketID);
extern void	DisableWriteEvent(SOCKET socketID);
extern void	EnableReadEvent(SOCKET	socketID);
extern void	DisableReadEvent(SOCKET	socketID);
extern void	EnableCloseEvent(SOCKET	socketID);
extern void	DisableCloseEvent(SOCKET socketID);

extern void WAP_SocketStateChanged(void);

#ifdef FEATURE_WAVELET
extern void WAV_SocketStateChanged(void);
#endif // FEATURE_WAVELET

#ifdef FEATURE_VAS
extern void VAS_SocketStateChanged(void);
#endif//feature_vas

extern sint15 PPPCLOSE_VM(void);
extern sint15 CLOSE_NETLIB(void);

#endif //__dssock_h__