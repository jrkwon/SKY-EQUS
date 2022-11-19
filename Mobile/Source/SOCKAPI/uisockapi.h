#ifndef	UISOCKAPI_H
#define	UISOCKAPI_H

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "mc.h"
#include "ds.h"
#include "queue.h"


#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/06

extern	q_type	sockSendDataQ, sockRecvDataQ;

extern	void	SvrLinkPPPConnect
(
	void
);

extern	void	SvrLinkUDPConnect
(
	char*		serverName,
	uint16		portNo
);

extern	void	SvrLinkConnect
(
	char*		serverName,
	uint16		portNo
);

extern	void	SvrLinkReconnect
(
	char*		serverName,
	uint16		portNo
);

extern	void	SvrLinkSend
(
	byte*	data,
	uint16	length
);

extern	byte	SvrLinkRcv
(
	byte*	data
);

extern	void	SvrLinkRcvDataToUi
(
	byte*	data,
	byte	length
);

extern	void	SvrLinkRelease
(
	void
);

extern void	SvrLinkReleasebyUi
(
	void
);

extern	void	SvrLinkResult
(
	sint15		connectStatus
);

extern  void	SvrLinkReleased
(
	void
);

extern	void	cleanSockDataQueue
(
	void
);

#endif	//	#ifdef	FEATURE_SKT_SOCKAPI


#if defined(FEATURE_SKT_PTPAPI)		//	khekim 00/04/07

#ifdef	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/11/15
typedef	enum
{
	DS_ASYNC_SRVC,
	DS_PIWF_SRVC
}	DS_SRVC_TYPE;
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

extern	q_type	ptpRecvDataQ;


#ifdef	FEATURE_SKT_PIWFPTPAPI
extern	void	PtpLinkConnect
(
	DS_SRVC_TYPE	srvcType,
	char*	dialStr
);
#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
extern	void	PtpLinkConnect
(
	char*	dialStr
);
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

#ifdef	FEATURE_SKT_PIWFPTPAPI
extern	void	PtpLinkWait
(
	DS_SRVC_TYPE	srvcType	
);
#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
extern	void	PtpLinkWait
(
	void	
);
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

extern	void	PtpLinkAnswer
(
	void	
);

extern	void	PtpLinkResult
(
	boolean	connectStatus
);

#ifdef	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/08/28
typedef	void	(*PtpLinkSendFuncPtrType)(byte* data, uint16 length);
extern	PtpLinkSendFuncPtrType	PtpLinkSend;

extern	void	PtpLinkSendbyAsync
(
	byte*	data,
	uint16	length
);

extern	void	PtpLinkSendbyPIWF
(
	byte*	data,
	uint16	length
);

#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
extern	void	PtpLinkSend
(
	byte*	data,
	uint16	length
);
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

extern	void	PtpLinkRcvDataToUi
(
	dsm_item_type	*item_ptr
);

extern	uint16	PtpLinkRcv
(
	byte* data
);

extern	void	PtpLinkRelease
(
	void
);

#ifdef	FEATURE_SKT_PIWFPTPAPI
extern	void	EnterPtpLinkMode
(
	DS_SRVC_TYPE	srvcType
);
#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
extern	void	EnterPtpLinkMode
(
	void
);
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

extern	void	cleanPtpDataQueue
(
	void
);


#endif	//	#ifdef	FEATURE_SKT_PTPAPI

#endif	//	#ifndef	UISOCKAPI_H