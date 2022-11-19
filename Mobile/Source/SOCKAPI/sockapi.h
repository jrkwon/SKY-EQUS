#ifndef	SOCKAPI_H
#define	SOCKAPI_H

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "mc.h"
#include "ds.h"

#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/05

#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "dssocket.h"

#define	HOSTNAME_LEN			60
#define	DATA_LEN				128

extern	boolean	g_bReleased;

extern	void	sockApi_openPPPNetwork
(
	void
);

extern	void	sockApi_openTCPNetwork
(
	char*	serverName,
	uint16	portNo
);

extern	void	sockApi_reopenTCPNetwork
(
	char*	serverName,
	uint16	portNo
);

extern	void	sockApi_openUDPNetwork
(
	char*	serverName,
	uint16	portNo
);

extern	void	sockApi_closeNetworkbyReleaseCmd
(
	void
);

extern	void	sockApi_sendData
(
	void
);

extern	void	sockApi_recvData
(
	void
);

extern	void	sockApi_processSockCallBackFn
(
	void
);

extern	void	sockApi_processNetCallBackFn
(
	void
);

#ifdef	FEATURE_SOCKET_DNS_API	//	khekim 00/11/14
extern	void	sockApi_processDnsCallBackFn
(
	void
);
#endif	//	#ifdef	FEATURE_SOCKET_DNS_API

#endif	//	#ifdef	FEATURE_SKT_SOCKAPI
#endif	//	#define	SOCKAPI_H