/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               UI S O C K E T  A P I

GENERAL DESCRIPTION
  This Software unit contains all the control functions for the Subscriber
  email application Task. This includes all REX Interfacing functions, the task
  entry point and main processing loop, and the email app. task-specific Wait 
  function.

EXTERNALIZED FUNCTIONS
	
INITIALIZATION AND SEQUENCING REQUIREMENTS

   Copyright (c) 2000 by SK Teletech Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "queue.h"
#include "ds.h"
#include "ps.h"
#include "dsm.h"
#include "mc.h"
#include "nv.h"
#include "msg.h"
#include "err.h"
#include "task.h"

#if MMI_TYPE == SKY_UI
#include "sky.h"
#else
#include "ui.h"
#endif
#include "dog.h"
#include <memory.h>
#include <stdio.h>

#include "uisockApi.h"
#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/04
#if MMI_TYPE == SKY_UI
#include "sky.h"
#else
#include "uistate.h"
#endif
#include "sockctl.h"
#include "dssocket.h"
#include "sockApi.h"
#include "dsskt.h"

q_type	sockSendDataQ, sockRecvDataQ;


///////////////////////////////////////////////////////////////////////////////////
void	SvrLinkPPPConnect
(
	void
)
{
	sock_cmd_type   sock_cmd_item;                        /* to hold the UI cmd. */

	MSG_HIGH("UI=>SOCK: SOCKET_SVR_PPPCONNECT_CMD", 0, 0, 0);
	sock_cmd_item.cmd_name = SOCKET_SVR_PPPCONNECT_CMD;

	sock_cmd(&sock_cmd_item);
}


void	SvrLinkUDPConnect
(
	char*		serverName,
	uint16		portNo
)
{
	sock_cmd_type   sock_cmd_item;                        /* to hold the UI cmd. */

	MSG_HIGH("UI=>SOCK: SOCKET_SVR_UDPCONNECT_CMD", 0, 0, 0);
	sock_cmd_item.cmd_name = SOCKET_SVR_UDPCONNECT_CMD;

	sock_cmd_item.num = portNo;	//	port Number
	strcpy(sock_cmd_item.parms.serverName, serverName);	//	server Name

	sock_cmd(&sock_cmd_item);
}

void	SvrLinkConnect
(
	char*		serverName,
	uint16		portNo
)
{
	sock_cmd_type   sock_cmd_item;                        /* to hold the UI cmd. */

	MSG_HIGH("UI=>SOCK: SOCKET_SVR_TCPCONNECT_CMD", 0, 0, 0);
	sock_cmd_item.cmd_name = SOCKET_SVR_TCPCONNECT_CMD;

	sock_cmd_item.num = portNo;	//	port Number
	strcpy(sock_cmd_item.parms.serverName, serverName);	//	server Name

	sock_cmd(&sock_cmd_item);
}

void	SvrLinkReconnect
(
	char*		serverName,
	uint16		portNo
)
{
	sock_cmd_type   sock_cmd_item;                        /* to hold the UI cmd. */

	MSG_HIGH("UI=>SOCK: SOCKET_SVR_TCPRECONNECT_CMD", 0, 0, 0);
	sock_cmd_item.cmd_name = SOCKET_SVR_TCPRECONNECT_CMD;

	sock_cmd_item.num = portNo;	//	port Number
	strcpy(sock_cmd_item.parms.serverName, serverName);	//	server Name

	sock_cmd(&sock_cmd_item);
}


void	SvrLinkSend
(
	byte*	data,
	uint16	length
)
{
	dsm_item_type	*item_ptr;
	uint16			in=0;

	MSG_HIGH("SvrLinkSend>> [%d]",length, 0, 0);

	while (length>0)
	{
		item_ptr = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL);

		if ( item_ptr == NULL )
		{
			MSG_ERROR("PtpLinkSend()>> dsm_new_buffer failed", 0, 0, 0);
		}
		if (length>=DSM_DS_SMALL_ITEM_SIZ)
		{
			memcpy(item_ptr->data_ptr, &data[in], DSM_DS_SMALL_ITEM_SIZ);
			item_ptr->kind = DSM_APP_PAYLOAD;
			item_ptr->used = DSM_DS_SMALL_ITEM_SIZ;
			in += DSM_DS_SMALL_ITEM_SIZ;
			length -= DSM_DS_SMALL_ITEM_SIZ;
		}
		else
		{
			memcpy(item_ptr->data_ptr, &data[in], length);
		    item_ptr->kind = DSM_APP_PAYLOAD;
			item_ptr->used = length;
			in += length;
			length = 0;
		}
		q_put( &sockSendDataQ, &item_ptr->link);
	}

	if (q_cnt(&sockSendDataQ)>0)
	{
		rex_set_sigs(&sock_tcb, SOCK_SENDDATA_Q_SIG);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// SOCKET->UI
byte	SvrLinkRcv
(
	byte*	data
)
{
	dsm_item_type *item_ptr;
	byte		length = 0;

	if ((item_ptr = (dsm_item_type *)q_get(&sockRecvDataQ)) != NULL)
	{
		memcpy(data, item_ptr->data_ptr, item_ptr->used);
		length = item_ptr->used;
		dsm_free_buffer(item_ptr);
	}

	MSG_HIGH("SvrLinkRcv>> [%d]",length, 0, 0);

	return length;
}


void	SvrLinkRcvDataToUi
(
	byte*	data,
	byte	length
)
{
	dsm_item_type	*item_ptr;
	byte			in=0;

	while (length>0)
	{
		item_ptr = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL);

		if ( item_ptr == NULL )
		{
			MSG_ERROR("PtpLinkSend()>> dsm_new_buffer failed", 0, 0, 0);
		}
		if (length>=DSM_DS_SMALL_ITEM_SIZ)
		{
			memcpy(item_ptr->data_ptr, &data[in], DSM_DS_SMALL_ITEM_SIZ);
			item_ptr->used = DSM_DS_SMALL_ITEM_SIZ;
			in += DSM_DS_SMALL_ITEM_SIZ;
			length -= DSM_DS_SMALL_ITEM_SIZ;
		}
		else
		{
			memcpy(item_ptr->data_ptr, &data[in], length);
			item_ptr->used = length;
			//in += length;
			length = 0;
		}
		q_put( &sockRecvDataQ, &item_ptr->link);
	}

	if (q_cnt(&sockRecvDataQ)>0)	
	{
		MSG_HIGH("SOCK=>UI: UI_RECVDATA_Q_SIG", 0, 0, 0);
		rex_set_sigs(&ui_tcb, UI_RECVDATA_Q_SIG);
	}
}


void	SvrLinkRelease
(
	void
)
{
	sock_cmd_type   sock_cmd_item;                        /* to hold the UI cmd. */

	MSG_HIGH("UI=>SOCK: SOCKET_RELEASE_CMD", 0, 0, 0);
	sock_cmd_item.cmd_name = SOCKET_RELEASE_CMD;
	sock_cmd(&sock_cmd_item);
}


void	SvrLinkReleasebyUi
(
	void
)
{
	SvrLinkRelease();
}

void	cleanSockDataQueue
(
	void
)
{
	dsm_item_type	*item_ptr;

	while(( item_ptr = (dsm_item_type *)q_get(&sockSendDataQ)) != NULL)
	{
		dsm_free_buffer(item_ptr);
	}

	while(( item_ptr = (dsm_item_type *)q_get(&sockRecvDataQ)) != NULL)
	{
		dsm_free_buffer(item_ptr);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// SOCKET->UI TASK: TCP/PPP Connect result
void	SvrLinkResult
(
	sint15	connectStatus
)
{
#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) 
	ui_cmd_type        *ui_cmd_ptr;  /* pointer to UI command item           */

	if ((ui_cmd_ptr = (ui_cmd_type *)q_get( &ui_cmd_free_q ))== NULL)
	{
		MSG_ERROR( "No UI cmd item available", 0, 0, 0);
	}
	else
	{
		if (connectStatus==DSS_SUCCESS)
		{
			MSG_HIGH( "SOCK=>UI: CONNECT_SUCCESS_F", 0, 0, 0);
			ui_cmd_ptr->hdr.cmd         = UI_SOCK_TCPCONNECT_SUCCESS_F;
		}
		else
		{
			MSG_HIGH( "SOCK=>UI: CONNECT_FAIL_F", 0, 0, 0);
			ui_cmd_ptr->hdr.cmd         = UI_SOCK_TCPCONNECT_FAIL_F;
		}
		ui_cmd_ptr->hdr.task_ptr    = NULL;
		ui_cmd_ptr->hdr.done_q_ptr  = &ui_cmd_free_q;
		ui_cmd (ui_cmd_ptr);
	}
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) 
}

/////////////////////////////////////////////////////////////////////////////////
// SOCKET->UI TASK: TCP/PPP Connect result
void	SvrLinkReleased
(
	void
)
{
#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) 
	ui_cmd_type        *ui_cmd_ptr;  /* pointer to UI command item           */

	if ((ui_cmd_ptr = (ui_cmd_type *)q_get( &ui_cmd_free_q ))== NULL)
	{
		MSG_ERROR( "No UI cmd item available", 0, 0, 0);
	}
	else
	{
		MSG_HIGH( "SOCK=>UI: UI_SOCK_RELEASED_F", 0, 0, 0);
		ui_cmd_ptr->hdr.cmd         = UI_SOCK_RELEASED_F;
		ui_cmd_ptr->hdr.task_ptr    = NULL;
		ui_cmd_ptr->hdr.done_q_ptr  = &ui_cmd_free_q;
		ui_cmd (ui_cmd_ptr);
	}
#endif	//	#if defined(FEATURE_GVM) || defined(FEATURE_IP_AIRDATA) 
}
#endif	//	#ifdef	FEATURE_SKT_SOCKAPI


#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/04/09

q_type	ptpRecvDataQ;

#ifdef	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/08/28
PtpLinkSendFuncPtrType	PtpLinkSend;
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI


#ifdef	FEATURE_SKT_PIWFPTPAPI
void	EnterPtpLinkMode
(
	DS_SRVC_TYPE	srvcType
)
{
	ds_cmd_type ds_cmd_item;                    /* item to hold cmd to dsmgr */
                        
	cleanPtpDataQueue();

	MSG_HIGH( "UI=>DS: DS_ENTER_AIRDATA_MODE_CMD", 0, 0, 0);
	ds_cmd_item.cmd_name = DS_ENTER_AIRDATA_MODE_CMD;
	ds_cmd_item.parms.srv_opt = (word)srvcType;
	if (srvcType==DS_PIWF_SRVC)	//	PIWF
	{
		PtpLinkSend = PtpLinkSendbyPIWF;
	}
	else
	{
		PtpLinkSend = PtpLinkSendbyAsync;
	}
	ds_cmd(&ds_cmd_item);
}
#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
void	EnterPtpLinkMode
(
	void
)
{
	ds_cmd_type ds_cmd_item;                    /* item to hold cmd to dsmgr */
                        
	cleanPtpDataQueue();

	MSG_HIGH( "UI=>DS: DS_ENTER_AIRDATA_MODE_CMD", 0, 0, 0);
	ds_cmd_item.cmd_name = DS_ENTER_AIRDATA_MODE_CMD;
	ds_cmd(&ds_cmd_item);
}
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

void	EnterPtpLinkOrig
(
	char*	dialStr
)
{
	ds_cmd_type ds_cmd_item;                    /* item to hold cmd to dsmgr */

	MSG_HIGH( "UI=>DS: DS_ORIG_AIRDATA_CALL_CMD", 0, 0, 0);
	ds_cmd_item.cmd_name = DS_ORIG_AIRDATA_CALL_CMD;
	memcpy(ds_cmd_item.parms.dialStr, (byte *)dialStr, strlen(dialStr));
	ds_cmd_item.parms.dialStr[strlen(dialStr)] = '\0';	//	End of String	

	ds_cmd(&ds_cmd_item);
}


#ifdef	FEATURE_SKT_PIWFPTPAPI
void	PtpLinkConnect
(
	DS_SRVC_TYPE	srvcType,
	char*			dialStr
)
{
	EnterPtpLinkMode(srvcType);
	EnterPtpLinkOrig(dialStr);                          
}
#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
void	PtpLinkConnect
(
	char*	dialStr
)
{
	EnterPtpLinkMode();
	EnterPtpLinkOrig(dialStr);                          
}
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI


#ifdef	FEATURE_SKT_PIWFPTPAPI
void	PtpLinkWait
(
	DS_SRVC_TYPE	srvcType
)
{
	EnterPtpLinkMode(srvcType);	// Any value is OK
}
#else   //  #ifdef	FEATURE_SKT_PIWFPTPAPI
void	PtpLinkWait
(
    void
)
{
	EnterPtpLinkMode();
}
#endif   //  #ifdef	FEATURE_SKT_PIWFPTPAPI

void	PtpLinkRelease
(
	void	
)
{
	ds_cmd_type ds_cmd_item;                    /* item to hold cmd to dsmgr */
                            
	MSG_HIGH( "UI=>DS: DS_EXIT_AIRDATA_MODE_CMD", 0, 0, 0);
	ds_cmd_item.cmd_name = DS_EXIT_AIRDATA_MODE_CMD;
	ds_cmd(&ds_cmd_item);

	cleanPtpDataQueue();
}

void	PtpLinkAnswer
(
	void
)
{
// in dsmgr.c, dsi_answer_is99_call() is called (Auto Answer)
//	dsi_answer_is99_call();
}


void	PtpLinkResult
(
	boolean	connectStatus
)
{
	ui_cmd_type        *ui_cmd_ptr;  /* pointer to UI command item           */

	if ((ui_cmd_ptr = (ui_cmd_type *)q_get( &ui_cmd_free_q ))== NULL)
	{
		MSG_ERROR( "No UI cmd item available", 0, 0, 0);
	}
	else
	{
		if (connectStatus==TRUE)
		{
			MSG_HIGH( "DS=>UI: CONNECT_SUCCESS_F", 0, 0, 0);
			ui_cmd_ptr->hdr.cmd         = UI_SOCK_TCPCONNECT_SUCCESS_F;
		}
		else
		{
			MSG_HIGH( "DS=>UI: CONNECT_FAIL_F", 0, 0, 0);
			ui_cmd_ptr->hdr.cmd         = UI_SOCK_TCPCONNECT_FAIL_F;
		}
		ui_cmd_ptr->hdr.task_ptr    = NULL;
		ui_cmd_ptr->hdr.done_q_ptr  = &ui_cmd_free_q;
		ui_cmd (ui_cmd_ptr);
	}
}

#ifdef	FEATURE_SKT_PIWFPTPAPI
void	PtpLinkSendbyAsync
#else	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
void	PtpLinkSend
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI
(
	byte*	data,
	uint16	length
)
{
	extern	boolean psi_tcp_send_q_flow_enable;
	dsm_item_type	*item_ptr;
	uint16			in = 0;

	MSG_HIGH("PtpLinkSend>> [%d]",length,0,0);

	while (length>0)
	{
		if (psi_tcp_send_q_flow_enable==TRUE)
		{
			item_ptr = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL);

			if ( item_ptr == NULL )
			{
				MSG_ERROR("PtpLinkSend()>> dsm_new_buffer failed", 0, 0, 0);
			}
			if (length>=DSM_DS_SMALL_ITEM_SIZ)
			{
				memcpy(item_ptr->data_ptr, &data[in], DSM_DS_SMALL_ITEM_SIZ);
				item_ptr->kind = DSM_RAW_APP_PAYLOAD;
				item_ptr->used = DSM_DS_SMALL_ITEM_SIZ;
				in += DSM_DS_SMALL_ITEM_SIZ;
				length -= DSM_DS_SMALL_ITEM_SIZ;
			}
			else
			{
				memcpy(item_ptr->data_ptr, &data[in], length);
				item_ptr->kind = DSM_RAW_APP_PAYLOAD;
				item_ptr->used = length;
				in += length;
				length = 0;
			}
			ps_tcp_enqueue(&item_ptr);
		}
	}
}

#ifdef	FEATURE_SKT_PIWFPTPAPI	//	khekim 00/08/28
void	PtpLinkSendbyPIWF
(
	byte*	data,
	uint16	length
)
{
	extern	boolean psi_tcp_send_q_flow_enable;
	dsm_item_type	*item_ptr;
	uint16			in = 0;

	MSG_HIGH("PtpLinkSendbyPIWF>> [%d]",length,0,0);

	while (length>0)
	{
		if (psi_tcp_send_q_flow_enable==TRUE)
		{
			item_ptr = dsm_new_buffer( DSM_DS_SMALL_ITEM_POOL);

			if ( item_ptr == NULL )
			{
				MSG_ERROR("PtpLinkSend()>> dsm_new_buffer failed", 0, 0, 0);
			}
			if (length>=DSM_DS_SMALL_ITEM_SIZ)
			{
				memcpy(item_ptr->data_ptr, &data[in], DSM_DS_SMALL_ITEM_SIZ);
				item_ptr->kind = DSM_APP_PAYLOAD;
				item_ptr->used = DSM_DS_SMALL_ITEM_SIZ;
				in += DSM_DS_SMALL_ITEM_SIZ;
				length -= DSM_DS_SMALL_ITEM_SIZ;
			}
			else
			{
				memcpy(item_ptr->data_ptr, &data[in], length);
				item_ptr->kind = DSM_APP_PAYLOAD;
				item_ptr->used = length;
				in += length;
				length = 0;
			}
			ps_tcp_enqueue(&item_ptr);
		}
	}
}
#endif	//	#ifdef	FEATURE_SKT_PIWFPTPAPI

void	PtpLinkRcvDataToUi
(
	dsm_item_type	*item_ptr
)
{

	MSG_HIGH("PtpLinkRcvDataToUI>> [%d]",item_ptr->used, 0, 0);

	q_put( &ptpRecvDataQ, &item_ptr->link);

	if (q_cnt(&ptpRecvDataQ)>0)
	{
		MSG_HIGH( "PS=>UI: UI_RECVDATA_Q_SIG", 0, 0, 0);
		rex_set_sigs(&ui_tcb, UI_RECVDATA_Q_SIG);
	}
}

uint16	PtpLinkRcv
(
	byte*	data
)
{
	dsm_item_type *item_ptr;
	uint16 length = 0;

	if (( item_ptr = (dsm_item_type *)q_get(&ptpRecvDataQ)) != NULL)
	{
		memcpy(data, item_ptr->data_ptr, item_ptr->used);
		length = item_ptr->used;
		dsm_free_buffer(item_ptr);
	}

	MSG_HIGH("PtpLinkRcv>> [%d]",length, 0, 0);

	return length;
}

void	cleanPtpDataQueue
(
	void
)
{
	dsm_item_type	*item_ptr;

	while(( item_ptr = (dsm_item_type *)q_get(&ptpRecvDataQ)) != NULL)
	{
		dsm_free_buffer(item_ptr);
	}
}


#endif	//	#ifdef	FEATURE_SKT_PTPAPI				