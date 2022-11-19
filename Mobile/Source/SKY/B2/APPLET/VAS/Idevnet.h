/*************************************************************************
*   Copyright (c) 1998-1999 Phone.com, Inc. All rights reserved.         *
*                                                                        *
*    THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Phone.com, Inc.      *
*    The copyright notice above does not evidence any                    *
*    actual or intended publication of such source code.                 *
**************************************************************************/

#ifndef	__IDEVNET_INCLUDED__
#define	__IDEVNET_INCLUDED__

/*
 *
 * DEVICE NETWORK APIs
 *
 */

#include "wdptypes.h"
/*
 * Multiple network bearer APIs
 *
 */
enum {
	NULL_BEARER,
// CDMA
	CDMA_SMS   ,
	CDMA_Packet,
	CDMA_CSD   ,
	CDMA_CSD2  ,
// CDMA
	MAX_BEARER_NAME   // must be last
};

typedef enum	{
	kBearerPacket,		/* Packet data, e.g. IP, GPRS */
	kBearerCircuit,		/* CSD */
	kBearerSMS,			/* 2-way SMS e.g. GSM SMS, GUTS/RDATA */
	kBearerSMS1
} BearerTypes;


typedef struct {
	const char* bearerName;
	BearerTypes	bearerType;
	TWDPBearerType	wdpBearerType;
} BearerInfo;

#define PROXYADDR_TERRA   0xcca3a722UL
#define PROXYADDR_DAGOBAH 0xcca3a716UL
#define PROXYADDR_MONGO   0xcca3a726UL
//Terra   - 204.163.167.34(0xCCA3A722)
//Dagobah - 204.163.167.22(0xCCA3A716)
//Mongo   - 204.163.167.36(0xCCA3A726)

#define PROXYADDR_STI_PRI 0xCBE2FC85UL  //203.226.252.133
#define PROXYADDR_STI_SEC 0xCBE2FCD3UL  //203.226.252.211
#define PROXYADDR_STI_DEM 0xCBE2FCD7UL  //203.226.252.215 demo server
//UP.Link Server Sites

///////////////////////////////////////////////////////////////////////////////
// zeus(yjchoi) 00.09.28
#define	INVALID_PROXY		(-1)
    /* Used to determine the validity of netCurProxy. */

#define	INVALID_PROXYADDR	(-1)
    /* Used to determine the validity of netCurProxyAddr. */

#define	INVALID_BEARER		(-1)
    /* Used to determine the validity of netCurBearer. */

#define CVTEST

#ifdef CVTEST
	#define PROXYADDR0_PRI PROXYADDR_STI_PRI
	#define PROXYADDR0_SEC PROXYADDR_STI_SEC
	#define PROXYADDR1_PRI PROXYADDR_DAGOBAH
	#define PROXYADDR1_SEC PROXYADDR_DAGOBAH
	#define PROXYADDR2_PRI PROXYADDR_TERRA
	#define PROXYADDR2_SEC PROXYADDR_TERRA
#else
	#define PROXYADDR0_PRI PROXYADDR_STI_PRI
	#define PROXYADDR0_SEC PROXYADDR_STI_SEC
	#define PROXYADDR1_PRI PROXYADDR_STI_PRI
	#define PROXYADDR1_SEC PROXYADDR_STI_SEC
	#define PROXYADDR2_PRI PROXYADDR_STI_PRI
	#define PROXYADDR2_SEC PROXYADDR_STI_SEC
#endif//cvtest
///////////////////////////////////////////////////////////////////////////////



/* Under WAP, the default ports for the UP.Link are now:
 *
 *   9200 - WAP Browser Connectionless Proxy Server (IANA assigned?)
 *   9201 - WAP Browser Proxy Server (IANA assigned)
 *   9203 - secure WAP Browser Proxy Server (IANA assigned)
 *   4445 - default port for UP push
 */
#define kWAPConnectionlessProxyPort 9200
#define kWAPProxyPort		9201
#define kWAPSecureProxyPort	9203
#define kWAPUPPushPort		4445

#define DS_DEFAULT_PORT	    (1904)
    /* The port on which Network Test communicates with DTServe,
     * a DevTest echo server. */
	  
/* 
	These two ports( 1902 & 1903) have been added for devtest's purpose
	for test multi-port transmission  
*/
#define PORT_1902			(1902)
#define PORT_1903			(1903)

#endif
