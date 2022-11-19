#ifndef __dssock_h__
#define  __dssock_h__

#ifndef WIN32

//	Added by khekim 00/1/7
#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "dssocket.h"

struct hostent {
    char*   h_name;
    char**	h_aliases;
    short   h_addrtype;
    short   h_length;
    char**	h_addr_list;
};

typedef unsigned int	SOCKET;

#define INVALID_SOCKET	(unsigned int)(~0)
#define SOCKET_ERROR				  (-1)
#define INADDR_NONE				0xffffffff

#define	DSS_NORMAL						0
#define	DSS_BEFORE_SOCKET_CONNECTION	-2
#define	DSS_BEFORE_CALL_CONNECTION		-3

// dss_htons is defined at dssocket.h
/*
#ifdef	FEATURE_DS_SOCKETS_not	//	khekim 00/1/7
#define htons(x)		dss_htons(x)
#define	ntohs(x)		dss_ntohs(x)
#define htonl(x)		dss_htonl(x)
#else
#define htons(x) \
        ((uint16)((((uint16)(x) & 0x00ff) << 8) | \
        (((uint16)(x) & 0xff00) >> 8)))
#define ntohs(x) \
        ((uint16)((((uint16)(x) & 0x00ff) << 8) | \
        (((uint16)(x) & 0xff00) >> 8)))
#define htonl(x) \
        ((uint32)((((uint32)(x) & 0x000000ffU) << 24) | \
        (((uint32)(x) & 0x0000ff00U) <<  8) | \
        (((uint32)(x) & 0x00ff0000U) >>  8) | \
        (((uint32)(x) & 0xff000000U) >> 24)))
#endif
*/
unsigned long inet_addr(char* cp);

SOCKET socket(char af, char type, char protocol); /* NKHA */
SOCKET get_socket(	struct hostent *hostentry, short port, char* check);
int closesocket(SOCKET s);
int sendto(SOCKET s, char* buf, int len, int flags, struct sockaddr* to, int tolen);
int recvfrom (SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen, int waittime);
struct hostent* gethostbyname(const char* name);
struct hostent* fb_gethostbyaddr(const char* addr, int len, int type);
int sktt_connect(SOCKET sockfd, struct sockaddr* servaddr, int addrlen);

int ConnectInternet(void);
int DisconnectInternet(void);
 
#endif //win32

#endif // __dssock_h__
