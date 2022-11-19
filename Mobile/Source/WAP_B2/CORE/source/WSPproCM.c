/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */

/*
 * WSPproCM.c
 *
 * Procedures for WSP Connection oriented.
 *
 * Created by Anders Edenbrandt, May 1999.
 *
 * Revision history:
 *   990915, AED: Changed from using MBM buffers to PDU buffers.
 *
 */

#include "WSPproCM.h"
#include "pdubuf.h"


//jjkyg defined header
#include "ui.h"
#include "wip_mem.h"
//#include "custA7.h"
#if !(defined WAP_B1 || defined WAP_B2)
#include "custB0.h"
#endif //not WAP_B1
#include "waptask.h"
#include "clnt.h"

/*==============================================================
						jjkyg defined region
 ==============================================================*/
#ifdef BROWSER_UPDATE
	#define REDIRECT
#endif

#ifdef REDIRECT
	BOOL   isPost = FALSE;
	UINT32 s_postlen = 0;
	BYTE   s_postdata[1500];
	extern redirect_config redirect;
	
#endif

#ifndef WAP_USE_SECURE
	extern BOOL Wtlshello;
	BYTE   Postdata[1500];
	int    postdatalen =0;
	void   wsp_send(unsigned char *ucpSendData, unsigned  int nSendLen);
#endif//#ifndef WAP_USE_SECURE

static ui_cmd_type *ui_cmd_ptr;
extern BOOL wtls_enc_start;
extern VOID   CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);

#ifdef DEBUG_WSPPROC
#include <stdio.h>
#include <stdarg.h>
static FILE *logfile = NULL;
static int log_init = 0;

static void
log_msg (char *fmt, ...)
{
  va_list args;

  if (!log_init) {
    log_init = 1;
    logfile = fopen ("debug_wsp.log", "w");
  }

  va_start (args, fmt);
  vfprintf (logfile, fmt, args);
  fflush (logfile);
  va_end (args);
}

#endif

#ifdef STANDALONETEST
SDL_Integer CurrentMOM = MOM;
SDL_Integer CurrentMOP = MOP;
SDL_Integer CurrentMRU = MRU;
SDL_Integer CurrentSMRU = SMRU;
SDL_Octet CurrentProtoOpt = PROTOOPT;
SDL_Boolean ConfirmedPushEnabled = SDL_True;
SDL_Boolean PushEnabled = SDL_True;
SDL_Boolean SessionResumeEnabled = SDL_True;
SDL_Octet MdfByClient = 0;
SDL_Octet MdfByServer = 0;
#endif

#ifndef MIN
#define MIN(x, y)     (((x) < (y)) ? (x) : (y))
#endif


#define WTP_MAX_HEADER_SIZE   5


/*
 * Extract the fields from a reply PDU.
 * Sets a status value, two VOIDSTAR out-parameters and two integer
 * out-parameters (containing headers and data from the PDU as well as
 * their lengths), and then releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffers
 * (unless the routine returns FALSE).
 */


extern SDL_Boolean
ExtractReplyPDU (pdubuf *PDU,
                 SDL_Octet *Status, VOIDSTAR *Headers, SDL_Natural *HeadLen,
                 VOIDSTAR *Data, SDL_Natural *DataLen)
{
/*
 * The contents of a Reply PDU is the following:
 *     UINT8    PDU type
 *     UINT8    Status
 *     UintVar  Length of content-type and headers fields combined
 *     Octets   Content type
 *     Octets   Headers
 *     Octets   Data
 */
  BYTE      *pd = pdubuf_getStart (PDU);
  SDL_Octet  PDU_type;
  UINT32     remsize = pdubuf_getLength (PDU);
  UINT32     n;

  CLNTa_log(1,0,"ExtractReplyPDU] is called]");

  if (remsize <= 2) { return FALSE; }

  PDU_type = *pd++;
  if (PDU_type != REPLY_PDU_TYPE) { return FALSE; }

  *Status = *pd++;

  remsize -= 2;
  if ((n = UintvarToInt (pd, HeadLen, remsize)) == 0) { return FALSE; }
  pd += n;
  remsize -= n;

  if (*HeadLen > remsize) { return FALSE; }
  *DataLen = remsize - *HeadLen;

  /* Get new buffers */
  if (*HeadLen > 0) {
    *Headers = NEWARRAY (BYTE, *HeadLen);

    if (*Headers == NULL) {
      /* Error, no memory available. */
      return FALSE;
    }
    B_COPYSTRINGN (*Headers, pd, *HeadLen);
    pd += *HeadLen;
  }
  else
    *Headers = NULL;

  if (*DataLen > 0) {
    *Data = NEWARRAY (BYTE, *DataLen);

    if (*Data == NULL) {
      /* Error, no memory available. */
      DEALLOC (Headers);
      return FALSE;
    }
    B_COPYSTRINGN (*Data, pd, *DataLen);
  }
  else
    *Data = NULL;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (PDU);

  CLNTa_log(1,0,"ExtractReplyPDU] OK OK OK]");
  
  return TRUE;
}


/*
 * Extract the fields from a Push PDU.
 * Sets two VOIDSTAR out-parameters and two integer
 * out-parameters (containing headers and data from the PDU as well as
 * their lengths), and then releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffers
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractPushPDU (pdubuf *PDU,
                VOIDSTAR *Headers, SDL_Natural *HeadLen,
                VOIDSTAR *Data, SDL_Natural *DataLen)
{
/*
 * The content of a Push PDU is as follows:
 *     UINT8    PDU type
 *     UintVar  Length of content-type and headers fields combined
 *     Octets   Content type
 *     Octets   Headers
 *     Octets   Data
 */
  BYTE      *pd = pdubuf_getStart (PDU);
  SDL_Octet  PDU_type;
  UINT32     remsize = pdubuf_getLength (PDU);
  UINT32     n;

  CLNTa_log(1,0,"ExtractPushPDU] is called]");

  if (remsize <= 2) { return FALSE; }

  PDU_type = *pd++;
  remsize -= 1;
  if ((PDU_type != PUSH_PDU_TYPE) && (PDU_type != CONFIRMED_PUSH_PDU_TYPE)) {
    return FALSE;
  }

  if ((n = UintvarToInt (pd, HeadLen, remsize)) == 0) { return FALSE; }
  pd += n;
  remsize -= n;

  if (*HeadLen > remsize) { return FALSE; }
  *DataLen = remsize - *HeadLen;

  /* Get new buffers */
  if (*HeadLen > 0) {
    *Headers = NEWARRAY (BYTE, *HeadLen);

    if (*Headers == NULL) {
      /* Error, no memory available. */
      return FALSE;
    }
    B_COPYSTRINGN (*Headers, pd, *HeadLen);
    pd += *HeadLen;
  }
  else
    *Headers = NULL;

  if (*DataLen > 0) {
    *Data = NEWARRAY (BYTE, *DataLen);

    if (*Data == NULL) {
      /* Error, no memory available. */
      DEALLOC (Headers);
      return FALSE;
    }
    B_COPYSTRINGN (*Data, pd, *DataLen);
  }
  else
    *Data = NULL;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (PDU);

  CLNTa_log(1,0,"ExtractPushPDU] OK OK OK");
  
  return TRUE;
}


/*
 * Create a PDU for a Connect request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateConnectPDU (SConnectReqType *SConnectReqData)
{
/*
 * A Connect PDU has the following contents:
 *    UINT8    PDU type
 *    UINT8    WSP Protocol Version
 *    UintVar  Length of the Capabilities field
 *    UintVar  Length of the Headers field
 *    Octets   Capabilities
 *    Octets   Headers
 */
  UINT32  caplen;     /* Length of capabilities */
  UINT32  headlen;    /* Length of header */
  INT32   PDUlen;     /* Calculated length of PDU */
  pdubuf  *PDU;       /* PDU buffer */
  BYTE    *pd;

  CLNTa_log(1,0,"CreateConnectPDU] is called");

  caplen = SConnectReqData->CapabilitiesLen;
  headlen = SConnectReqData->HeadersLen;
  PDUlen = headlen + caplen + UintvarLen (headlen) + UintvarLen (caplen) + 2;

  /* Get new PDU buffer, of size PDUlen */
  if ((PDU = pdubuf_new ((UINT16)(PDUlen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (PDU, (UINT16)PDUlen);

  /* Place PDU type, protocol version, capabilities length, headers length,
     capabilities and headers in the PDU. */
  pd = pdubuf_getStart (PDU);
  *pd++ = (SDL_Octet)CONNECT_PDU_TYPE;
  *pd++ = (SDL_Octet)WSP_PROTOCOL_VERSION;
  pd += IntToUintvar (caplen, pd);
  pd += IntToUintvar (headlen, pd);
  B_COPYSTRINGN (pd, SConnectReqData->RequestedCapabilities, caplen);
  pd += caplen;
  B_COPYSTRINGN (pd, SConnectReqData->ClientHeaders, headlen);

  /* Release buffers given as in-parameters. */
  DEALLOC (&SConnectReqData->ClientHeaders);
  DEALLOC (&SConnectReqData->RequestedCapabilities);

  CLNTa_log(1,0,"CreateConnectPDU] OK OK OK");

  return PDU;
}

/*
 * Extract the fields from a Connect Reply PDU.
 * Sets a session ID value and fields in the SConnectCnfType structure
 * representing capabilities and headers as well as their lengths,
 * and then releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffers
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractConnectReplyPDU (pdubuf *PDU,
                        SDL_Natural *ServerSessionId,
                        SConnectCnfType *SConnectCnfData)
{
/*
 * A Connect Reply PDU has the following content:
 *     UINT8     PDU type
 *     UintVar   Server session ID
 *     UintVar   Length of capabilities field
 *     UintVar   Length of headers field
 *     Octets    Capabilities
 *     Octets    Headers
 */
  BYTE      *pd = pdubuf_getStart (PDU);
  SDL_Octet  PDU_type;
  UINT32     remsize = pdubuf_getLength (PDU);
  UINT32     n;

  CLNTa_log(1,0,"ExtractConnectReplyPDU] is called");

  if (remsize < 4) { return FALSE; }

  PDU_type = *pd++;
  remsize--;
  if (PDU_type != CONNECTREPLY_PDU_TYPE) { return FALSE; }

  if ((n = UintvarToInt (pd, ServerSessionId, remsize)) == 0) {
    return FALSE;
  }
  pd += n;
  remsize -= n;
  if ((n = UintvarToInt (pd, &SConnectCnfData->CapabilitiesLen,
                         remsize)) == 0) {
    return FALSE;
  }
  pd += n;
  remsize -= n;

  if ((n = UintvarToInt (pd, &SConnectCnfData->HeadersLen, remsize)) == 0) {
    return FALSE;
  }
  pd += n;
  remsize -= n;

  if ((SConnectCnfData->CapabilitiesLen  + SConnectCnfData->HeadersLen) >
      remsize) { return FALSE; }

  /* Get new buffers */
  if (SConnectCnfData->CapabilitiesLen > 0) {
    SConnectCnfData->NegotiatedCapabilities =
      NEWARRAY (BYTE, SConnectCnfData->CapabilitiesLen);

    if (SConnectCnfData->NegotiatedCapabilities == NULL) {
      /* Error, no memory available. */
      return FALSE;
    }
    B_COPYSTRINGN (SConnectCnfData->NegotiatedCapabilities, pd,
                   SConnectCnfData->CapabilitiesLen);
    pd += SConnectCnfData->CapabilitiesLen;
  }
  else
    SConnectCnfData->NegotiatedCapabilities = NULL;

  if (SConnectCnfData->HeadersLen > 0) {
    SConnectCnfData->ServerHeaders =
      NEWARRAY (BYTE, SConnectCnfData->HeadersLen);

    if (SConnectCnfData->ServerHeaders == NULL) {
      /* Error, no memory available. */
      DEALLOC (&SConnectCnfData->NegotiatedCapabilities);
      return FALSE;
    }
    B_COPYSTRINGN (SConnectCnfData->ServerHeaders, pd,
                   SConnectCnfData->HeadersLen);
  }
  else
    SConnectCnfData->ServerHeaders = NULL;


  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (PDU);

  CLNTa_log(1,0,"ExtractConnectReplyPDU] OK OK OK");
  
  return TRUE;
}

/*
 * Create a Disconnect PDU.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateDisconnectPDU (SDL_Integer SessionID)
{
/*
 * A Disconnect PDU has the following content:
 *     UINT8    PDU type
 *     UintVar  Server Session ID
 */
  pdubuf  *PDU;
  INT32   PDUlen;        /* Calculated length of PDU */
  BYTE    *pd;

  CLNTa_log(1,0,"CreateDisconnectPDU] is called");

  PDUlen = UintvarLen (SessionID) + 1;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers */
  if ((PDU = pdubuf_new ((UINT16)(PDUlen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (PDU, (UINT16)PDUlen);

  pd = pdubuf_getStart (PDU);
  *pd++ = DISCONNECT_PDU_TYPE;
  (void)IntToUintvar (SessionID, pd);

  CLNTa_log(1,0,"CreateDisconnectPDU] OK OK OK");

  return PDU;
}

/*
 * Extract the fields from a Disconnect PDU.
 * Sets the session ID value, and then releases the PDU buffer.
 * In case of error, the PDU buffer is NOT released.
 * Returns FALSE on error, TRUE otherwise.
 */
extern SDL_Boolean
ExtractDisconnectPDU (pdubuf *PDU,
                      SDL_Natural *ServerSessionId)
{
/*
 * A Disconnect PDU has the following content:
 *     UINT8    PDU type (always 5)
 *     UintVar  Server Session ID
 */
  BYTE *pd = pdubuf_getStart (PDU);
  UINT32     SDU_size = pdubuf_getLength (PDU);
/*  Removed by GBU,000314,  not used
  UINT32     n;
*/

  CLNTa_log(1,0,"ExtractDisconnectPDU] OK OK OK");

  pd++; /* Skip over PDU type byte. */
/*  Modified by GBU,000314,  n not used
  if ((n = UintvarToInt (pd, ServerSessionId, SDU_size - 1)) == 0) {
*/
  if (UintvarToInt (pd, ServerSessionId, SDU_size - 1) == 0) {
    return SDL_False;
  }

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (PDU);

  CLNTa_log(1,0,"ExtractDisconnectPDU] OK OK OK");

  return SDL_True;
}

/*
 * Create a PDU for a Get or similar request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern BOOL   bCreateReq;
extern BOOL   initialaccess;
extern pdubuf *
CreateMethodPDU (SMethodInvokeReqType *SMethodInvokeReqData)
{
/*
 * A Get PDU has the following content:
 *     UINT8    PDU Type
 *     UintVar  Length of URI
 *     Octets   URI
 *     Octets   Headers
 */
  UINT32 urilen;
  UINT32 headlen;  /* Length of header */
  INT32  PDUlen;   /* Calculated length of PDU */
  BYTE   *pd;
  pdubuf *PDU;     /* PDU buffer */

#ifdef DEBUG_WSPPROC
  log_msg ("Entering CreateMethodPDU\n");
#endif


  CLNTa_log(1,0,"CreateMethodPDU] is called");

  urilen = SMethodInvokeReqData->URILen;
  headlen = SMethodInvokeReqData->HeadersLen;
  PDUlen = urilen + headlen + UintvarLen (urilen) + 1;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers. */
  if ((PDU = pdubuf_new ((UINT16)(PDUlen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
#ifdef DEBUG_WSPPROC
  log_msg ("pdubuf_new returned NULL.\n");
#endif
    return NULL;
  }
#ifdef DEBUG_WSPPROC
  log_msg ("pdubuf_new returned new buffer.\n");
#endif
  pdubuf_setLength (PDU, (UINT16)(PDUlen));

  /* Place PDU type, URI length, Headers length, Request URI,
     Request headers, and Request body in the PDU */
  pd = pdubuf_getStart (PDU);
  *pd++ = SMethodInvokeReqData->Method;     /* PDU type */
  pd += IntToUintvar (urilen, pd);
  B_COPYSTRINGN (pd, SMethodInvokeReqData->RequestURI, urilen);
  pd += urilen;
  B_COPYSTRINGN (pd, SMethodInvokeReqData->RequestHeaders, headlen);
#ifdef DEBUG_WSPPROC
  log_msg ("Copied all data to new PDU buffer.\n");
#endif

  /* Release buffers given as in-parameters. */
  DEALLOC (&SMethodInvokeReqData->RequestURI);
  DEALLOC (&SMethodInvokeReqData->RequestHeaders);
  DEALLOC (&SMethodInvokeReqData->RequestBody);
#ifdef DEBUG_WSPPROC
  log_msg ("Released old VOIDSTAR buffers.\n");
#endif

  if(bCreateReq && initialaccess)
  {
	if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q))!= NULL)
	{
#ifdef PORTING_B0_WAP
		ui_cmd_ptr->hdr.cmd        = UI_WAP_REQ_F;
		ui_cmd_ptr->hdr.task_ptr   = NULL;
		ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		ui_cmd( ui_cmd_ptr);
#endif //PORTING_B0_WAP
	}	
  }	
  
  CLNTa_log(1,0,"CreateMethodPDU] OK OK OK");

  return PDU;
}

/*
 * Create a PDU for a Post request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
#ifdef FOR_MS
static BYTE wcpRec[1500];   //Enc data..
extern int  ency_write(int *ipPrim,unsigned char *ucpSend, int nSendLen,int *npErr, char* Rec);
#endif//FOR_MS


extern pdubuf *
CreatePostPDU (SMethodInvokeReqType *SMethodInvokeReqData)
{
/*
 * A Post PDU has the following content:
 *     UINT8   PDU type (always 0x60)
 *     UintVar Length of URI
 *     UintVar Length of content type and headers fields combined
 *     Octets  URI
 *     Octets  Content type
 *     Octets  Headers
 *     Octets  Data
 */
  UINT32 urilen;
  UINT32 headlen;  /* Length of header */
  UINT32 bodylen;
  INT32  PDUlen;   /* Calculated length of PDU */
  BYTE   *pd;
  pdubuf *PDU;     /* PDU buffer */

#ifdef FOR_MS
  int   enc_length;
  int   err;


  CLNTa_log(1,0,"CreatePostPDU] is called");

#ifndef WAP_USE_SECURE
  if(Wtlshello)
  {
	  //jjkyg 07/05
	  extern boolean bStopKey;
	  bStopKey = TRUE;
	  SMethodInvokeReqData->BodyLen = postdatalen;
  }
#endif//#ifndef WAP_USE_SECURE
#endif//#ifdef FOR_MS

#ifdef REDIRECT
  if(isPost && redirect.bCorrect)
  {
	SMethodInvokeReqData->BodyLen = (int)s_postlen;
	if(s_postlen > 1500) 
	{
		isPost = FALSE;	
		memset(s_postdata,0x00,1500);s_postlen  = 0;		
		return NULL;
	}
	CLNTa_log(1,0,"Redirect length body len %d ,slen =%d",SMethodInvokeReqData->BodyLen,s_postlen);
  }
#endif

  urilen  = SMethodInvokeReqData->URILen;
  headlen = SMethodInvokeReqData->HeadersLen;
  bodylen = SMethodInvokeReqData->BodyLen;

#ifdef FOR_MS
  if(wtls_enc_start)
  {
	extern boolean bStopKey;

	bStopKey = TRUE;
	enc_length = 0;	
    
	memset(wcpRec,0x00,1500);//After ENC procedure data
	if(bodylen > 1500 ) 
	{
		bStopKey = FALSE;return NULL;
	}

	//error발생시 처리  
	enc_length = ency_write(NULL,(unsigned char*)SMethodInvokeReqData->RequestBody,bodylen,&err,(char*)wcpRec);
#ifdef WAP_USE_LEAK
	if(enc_length == 0)
	{
		return NULL;
	}
#endif//#ifdef WAP_USE_LEAK
	
	SMethodInvokeReqData->BodyLen = enc_length;
	bodylen = enc_length;	
  }   
#endif//#ifdef FOR_MS 


  PDUlen = urilen + headlen + bodylen + UintvarLen (urilen) + UintvarLen (headlen) + 1;
 
  /* Get new PDU buffer, of size PDUlen plus romm for WTP headers. */
  if ((PDU = pdubuf_new ((UINT16)(PDUlen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (PDU, (UINT16)PDUlen);

  /* Place PDU type, URI length, Headers length, Request URI,
     Request headers, and Request body in the PDU */
  pd = pdubuf_getStart (PDU);
  *pd++ = (SDL_Octet)POST_PDU_TYPE;
  pd += IntToUintvar (urilen, pd);
  pd += IntToUintvar (headlen, pd);
  B_COPYSTRINGN (pd, SMethodInvokeReqData->RequestURI, urilen);
  pd += urilen;
  B_COPYSTRINGN (pd, SMethodInvokeReqData->RequestHeaders, headlen);
  pd += headlen;

#ifdef FOR_MS
  if(wtls_enc_start)
  {
	B_COPYSTRINGN (pd, wcpRec, bodylen);
  }

#ifndef WAP_USE_SECURE
  else if(Wtlshello)
  {
	  CLNTa_log(1,0,"HERE IS WSPPROC_CM.C bodylen %d",bodylen);
	  B_COPYSTRINGN (pd,Postdata,bodylen);
  }
#endif//#ifndef WAP_USE_SECURE

#ifdef REDIRECT
  else if(isPost && redirect.bCorrect)
  {
	CLNTa_log(1,0,"REDIRECT WSPPROC_CM.C bodylen %d",bodylen);
	B_COPYSTRINGN (pd,s_postdata,bodylen);
  }
#endif//#ifdef REDIRECT
  else
  {
	B_COPYSTRINGN (pd, SMethodInvokeReqData->RequestBody,bodylen);
  }
#else//#ifdef FOR_MS
  B_COPYSTRINGN (pb, SMethodInvokeReqData->RequestBody, bodylen);
#endif//#ifdef FOR_MS

#ifdef REDIRECT
//================================================
//				Post data Clear
//================================================
	if(isPost)
	{
		isPost = FALSE;	
		memset(s_postdata,0x00,1500);s_postlen  = 0;
	}
//=================================================
#endif//

  /* Release buffers given as in-parameters. */
  DEALLOC (&SMethodInvokeReqData->RequestURI);
  DEALLOC (&SMethodInvokeReqData->RequestHeaders);
#ifdef FOR_MS 
#ifndef WAP_USE_SECURE
  if(Wtlshello || redirect.bCorrect)
#else
  if(redirect.bCorrect)
#endif//#ifndef WAP_USE_SECURE
  {
	  CLNTa_log(1,0,"only return");
	  return PDU;
  }
  else
  {
	DEALLOC(&SMethodInvokeReqData->RequestBody);
  }  
#else
  DEALLOC(&SMethodInvokeReqData->RequestBody);
#endif//#ifdef FOR_MS

  CLNTa_log(1,0,"CreatePostPDU] OK OK OK");

  return PDU;
}

/*
 * Create a PDU for a Suspend request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateSuspendPDU (SDL_Natural sessionId)
{
/*
 * A Suspend PDU has the following content:
 *     UINT8   PDU type
 *     UintVar Session ID
 */
  INT32  PDUlen;    /* Calculated length of PDU */
  BYTE   *pd;
  pdubuf *PDU;      /* PDU buffer */

  CLNTa_log(1,0,"CreateSuspendPDU] is called");

  PDUlen = UintvarLen (sessionId) + 1;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers. */
  if ((PDU = pdubuf_new ((UINT16)(PDUlen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (PDU, (UINT16)PDUlen);

  /* Place PDU type and session Id in the PDU */
  pd = pdubuf_getStart (PDU);
  *pd++ = (SDL_Octet)SUSPEND_PDU_TYPE;
  pd += IntToUintvar (sessionId, pd);

  CLNTa_log(1,0,"CreateSuspendPDU] OK OK OK");

  return PDU;
}

/*
 * Create a PDU for a Resume request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateResumePDU (SDL_Natural sessionId, SResumeReqType *SResumeReqData)
{
/*
 * A Resume PDU has the following content:
 *     UINT8   PDU type
 *     UintVar Session ID
 *     UintVar Length of capabilities field (Currently always 0)
 *     Octets  Capabilities                 (Currently always empty)
 *     Octets  Headers
 */
  UINT32 caplen = 0;
  UINT32 headlen;
  INT32  PDUlen;        /* Calculated length of PDU */
  BYTE   *pd;
  pdubuf *PDU;          /* PDU buffer */


  CLNTa_log(1,0,"CreateResumePDU is called]");

  /* caplen = SResumeReqData->CapabilitiesLen; */
  headlen = SResumeReqData->HeadersLen;
  PDUlen = 1 + UintvarLen (sessionId) + UintvarLen (caplen) + caplen + headlen;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers. */
  if ((PDU = pdubuf_new ((UINT16)(PDUlen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (PDU, (UINT16)PDUlen);

  /* Place PDU type, session Id, capabilites length, capabilites and
   * headers in the PDU. */
  pd = pdubuf_getStart (PDU);
  *pd++ = (SDL_Octet)RESUME_PDU_TYPE;
  pd += IntToUintvar (sessionId, pd);
  pd += IntToUintvar (caplen, pd);
  /*
  B_COPYSTRINGN (pd, SResumeReqData->RequestedCapabilities, caplen);
  pd += caplen;
  */
  B_COPYSTRINGN (pd, SResumeReqData->ClientHeaders, headlen);

  /* Release buffers given as in-parameters. */
  /* DEALLOC (&SResumeReqData->RequestedCapabilities); */
  DEALLOC (&SResumeReqData->ClientHeaders);

  CLNTa_log(1,0,"CreateResumePDU] OK OK OK");

  return PDU;
}


/*
 * Extract the fields from a redirect PDU.
 * Sets a reason code, a redirect security indication,
 * and places the redirect addresses in a newly allocated buffer,
 * and finally releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffer
 * (unless the routine returns FALSE).
 */
/*====================================================================================
	SKT에서 제안한 redirect는 일단 ip address가 아니 string이 내려온다는 
	점에서 spec.과는 차이가 있다. 일단 다음과 같은 조건을 둔다.

	1. 실제 address필드에 숫자가 아닌 코드가 있을 경우에는 이것을 이용하여
	   DNS query를 다시 시도한다.
	2. URL이 string일 경우에는 SDL_False를 return한다.
	3. SDL_False를 core로 보낼 경우 Error가 return되므로 이것을 이용하여
	   wapstate.c에서 nv저장하거나 일시적으로 사용하여 DNS query를 시도한다.
	4. FLAG = 0x40(secure session reuse)는 사용하지 않는다.
	5. Redirect type의 Address는 wapstate.c의 proxy_url에 copy한후 지운다.(free한다.)
	6. bCorrect = TRUE는 wapstate.c에서 Error발생시, Redirect action을 수행하는 뜻임
	7. 6번을 사용하여 IP로 오는 redirect에도 사용가능하다. 이럴경우 bCorrect를 int형으로
       바꾸어서 0은 Error, 1은 IP, 2는 URL string으로 구별하면 된다.
	8. Redirect address가 string일 경우에만, ifdef REDIRECT를 사용하며, 아닐 경우에는 
	   기존의 routine을 사용하며, Header.c의 ParseRedirectAddress()를 이용한다.
	   →이럴 경우, Setstrconfig가 제대로 동작하여야만 한다.
 =====================================================================================*/
#ifdef REDIRECT
#define Bearer     0x80
#define PortNum    0x40
#define Addlength  0x3f

BOOL isfirstdirect = FALSE;

extern void  wip_free(void* mem);
extern void *wip_malloc (UINT32 size);
extern byte get_length( byte *chk_str, byte buff_length); 

void init_buffer(redirect_config* buf)
{

	buf->bCorrect        = FALSE;
	buf->bPerm_or_not    = FALSE;
	
	CLNTa_log(1,0,"buf->address size %d",strlen(buf->Address));

	if(isfirstdirect)
	{
		buf->Address  = NULL;isfirstdirect = FALSE;
		CLNTa_log(1,0,"isfirst..........");
	}
	else
	{
		CLNTa_log(1,0,"Not isfirst..........");
		//================================================
		//				Post data Clear
		//================================================
			if(isPost) isPost = FALSE;
			memset(s_postdata,0x00,1500);
			s_postlen  = 0;	
		//=================================================

		if(buf->Address!=NULL)
		{
			wip_free(buf->Address);	buf->Address = NULL;
		}
	}
	
	buf->Addresslen =0;

	buf->bNetinclude     = FALSE;
	buf->bPortNuminclude = FALSE;

	buf->Portnumber = 0;
	buf->BearerType = 0;
}

BOOL ExtractPdu(VOIDSTAR* Cordbuf)
{
	UINT8 *ptemp;
	ptemp =(UINT8*)*Cordbuf;

	//network include
	if((*ptemp & Bearer) == Bearer)
	{
		redirect.bNetinclude  = TRUE;		
	}

	if((*ptemp & PortNum) == PortNum)
	{
		redirect.bPortNuminclude = TRUE;
	}

	if((*ptemp & Addlength)!=0)
	{
		redirect.Addresslen = (*ptemp & Addlength);
	}
	ptemp++;

	if(	redirect.bNetinclude)
	{
		redirect.BearerType = *ptemp++;
	}

	if(redirect.bPortNuminclude)
	{
		memcpy((BYTE*)&redirect.Portnumber,ptemp,2);
		ptemp+=2;
	}

	//address가 string인지 아닌지 먼저 살핀다.
	if((*ptemp >= 0x30)&&(*ptemp <0x7B))
	{
		int length =0;
		CHAR* pptemp = NULL;

		pptemp = (CHAR*)ptemp;
		length = get_length((unsigned char*)pptemp,(unsigned char)redirect.Addresslen ); 
		CLNTa_log(1,0,"receive length %d",length);

		redirect.Addresslen = length;
#ifdef WAP_USE_LEAK
		redirect.Address =(CHAR*)WIPAlloc(redirect.Addresslen+1);
		if(!redirect.Address)
		{
			redirect.bCorrect = FALSE;
			return redirect.bCorrect;
		}
#else //#ifdef WAP_USE_LEAK
		redirect.Address =(CHAR*)wip_malloc(redirect.Addresslen+1);
#endif //#ifdef WAP_USE_LEAK
		memset(redirect.Address,0x00,redirect.Addresslen+1);
		strncpy(redirect.Address,(CHAR*)ptemp,redirect.Addresslen);
		redirect.bCorrect = TRUE;
	}
	else
	{
		redirect.bCorrect = FALSE;
		if(redirect.Address!=NULL)
		{	
			CLNTa_log(1,0,"Here is not enter when normal state");
			wip_free(redirect.Address);
			redirect.Address = NULL;
		}
	}
	return redirect.bCorrect;
}
#endif//#ifdef REDIRECT

extern SDL_Boolean
ExtractRedirectPDU (pdubuf *PDU, SDL_Octet *ReasonCode,
                    SDL_Boolean *RedirectSecurity,
                    VOIDSTAR *RedirectedAddresses,
                    SDL_Natural *AddressLen)
{
/*
 * A Redirect PDU has the following content:
 *     UINT8     PDU type
 *     UINT8     Flags
 *     Octets    Redirect addresses
 */
  BYTE       *pd = pdubuf_getStart (PDU);
  static     UINT32 remsize; 
  SDL_Octet  PDU_type;  
  BYTE       flags;

#ifdef REDIRECT
  isfirstdirect = TRUE;
  init_buffer(&redirect);
#endif//#ifdef REDIRECT

  CLNTa_log(1,0,"Receive Redirect Message");	
  remsize = pdubuf_getLength (PDU);

  if (remsize < 2) 
  { 
#ifdef REDIRECT	  
	  //jjkyg 00/06/20
	  redirect.bCorrect = FALSE;
#endif//#ifdef REDIRECT	  
	  return SDL_False; 
  }

  PDU_type = *pd++;
  remsize--;
  if (PDU_type != REDIRECT_PDU_TYPE) 
  {
#ifdef REDIRECT	  	  
	  //jjkyg 00/06/20
	  redirect.bCorrect = FALSE;
#endif//#ifdef REDIRECT	  
	  return SDL_False; 
  }

  flags = *pd++;
  remsize--;

  if (flags & REDIRECT_FLAGS_PERMANENT) 
  {
	CLNTa_log(1,0,"Permanent FLAG bit"); *ReasonCode = 0x31;
#ifdef REDIRECT	  
	//jjkyg 00/06/20
	redirect.bPerm_or_not = TRUE;
#endif//#ifdef REDIRECT	  
  }
  else if(flags == 0x00)
  {
	CLNTa_log(1,0,"temp FLAG bit"); *ReasonCode = 0x32;	
#ifdef REDIRECT	  
	//jjkyg 00/06/20
	redirect.bPerm_or_not = FALSE;
#endif//#ifdef REDIRECT	  
  }

/*============================================================================
				Secure reuse는 사용하지 않는다.
 ============================================================================*/
  if (flags & REDIRECT_FLAGS_REUSE_SEC) {
    *RedirectSecurity = SDL_True;
  } 
  else
  {
#ifdef REDIRECT	  
	  //jjkyg 00/06/20
	  redirect.bCorrect = FALSE;
#endif//#ifdef REDIRECT	  
	  *RedirectSecurity = SDL_False;
  }
/*==============================================================================*/
  
  if (remsize > 0) 
  {
    CLNTa_log(1,0,"receive Redirect size %d",remsize);

	*RedirectedAddresses = NEWARRAY (BYTE, remsize);	

	if (*RedirectedAddresses == NULL)
	{
#ifdef REDIRECT	        	
		redirect.bCorrect = FALSE;
#endif//#ifdef REDIRECT	  
		return SDL_False;
    }
    B_COPYSTRINGN (*RedirectedAddresses, pd, remsize);

#ifdef REDIRECT	  
	//jjkyg 00/06/20
	if(ExtractPdu(RedirectedAddresses)==TRUE)
	{
		return SDL_False;
	}	
#endif//#ifdef REDIRECT	  
  }
  else
  {
    *RedirectedAddresses = NULL;
  }
  *AddressLen = remsize;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (PDU);

  return SDL_True;
  
}

/*
 * Examine a list of capabilities and possibly modify some of them.
 * The (possibly modified) values found are stored in the out-parameters
 * CurrentMOM, CurrentMOP, etc. In the parameter MdfBy a bit is set
 * to show which capabilities were found. If a capability of the type
 * Protocol Options is present, the out-parameters ConfirmedPushEnabled,
 * PushEnabled, and SessionResumeEnabled are set to show which of these
 * options are enabled.
 * The parameter ReqCapLen holds the length of the buffer on input,
 * and on output holds the length of the modified buffer.
 * Returns FALSE on error, TRUE otherwise.
 */
extern SDL_Boolean
GetAndMdfReqCap (void *RequestedCapabilities,
                 SDL_Natural *ReqCapLen,
                 SDL_Natural *CurrentMOM, SDL_Natural *CurrentMOP,
                 SDL_Natural *CurrentMRU, SDL_Natural *CurrentSMRU,
                 SDL_Octet *CurrentProtoOpt, SDL_Boolean *ConfirmedPushEnabled,
                 SDL_Boolean *PushEnabled, SDL_Boolean *SessionResumeEnabled,
                 SDL_Octet *MdfBy)
{
/*  Modified by GBU,000103  added cast */
/*
  BYTE *src = RequestedCapabilities;
*/
  BYTE *src = (BYTE*)RequestedCapabilities;
  BYTE *dst = src;
  UINT32 remsize = *ReqCapLen;  /* Number of bytes that remain to look at. */
  BYTE   capid;                 /* Current capability identifier. */
  UINT32 caplen;                /* Length of current capability. */
  UINT32 value;        
  UINT32 n;

#ifdef DEBUG_WSPPROC
  {
    unsigned int i;
    log_msg ("\nCapabilities: ");
    for (i = 0; i < remsize; i++) {
      log_msg ("0x%x ", src[i]);
    }
    log_msg ("\n");
    log_msg ("Length of capability string: %d\n\n", remsize);
  }
#endif  

  while (remsize > 0) {
#ifdef DEBUG_WSPPROC
    log_msg ("Remaining size: %d\n", remsize);
#endif
    if ((n = UintvarToInt (src, &caplen, remsize)) == 0) {
#ifdef DEBUG_WSPPROC
      log_msg ("Incorrect Uintvar as capability length.\n");
#endif
      return SDL_False;
    }
    src += n;
    remsize -= n;

#ifdef DEBUG_WSPPROC
    log_msg ("Caplen: %d\n", caplen);
#endif

    if (caplen == 0) { return SDL_False; }      /* Too small length. */
    if (remsize < caplen) { return SDL_False; } /* Too large length. */

    /* We assume that the capability identifier is a single byte. */
    /* This might change in the future? */
    capid = *src++;
    remsize--;

#ifdef DEBUG_WSPPROC
    log_msg ("Capid:  %d\n", capid);
#endif

    if (capid < 0x80) {
      /* An unknown capability identifier. */
      /* Simply copy the whole thing. */
      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      B_COPYSTRINGN (dst, src, caplen - 1);
      src += caplen - 1;
      remsize -= caplen - 1;
      dst += caplen - 1;
      continue;
    }

    switch (capid & 0x7f) {
    case CAP_CLIENT_SDU_SIZE:
      /* Client SDU-size is a Uintvar. */
      *MdfBy |= MDF_CLIENT_SDU;
      if ((n = UintvarToInt (src, &value, remsize)) == 0) {
#ifdef DEBUG_WSPPROC
        log_msg ("Incorrect Uintvar in CLIENT_SDU_SIZE\n");
#endif
        return SDL_False;
      }
      src += n;
      remsize -= n;
      value = (value == 0) ? MRU : value;
      *CurrentMRU = MIN (value, MRU);

      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      dst += IntToUintvar (*CurrentMRU, dst);
      break;

    case CAP_SERVER_SDU_SIZE:
      /* Server SDU-size is a Uintvar. */
      *MdfBy |= MDF_SERVER_SDU;
      if ((n = UintvarToInt (src, &value, remsize)) == 0) {
#ifdef DEBUG_WSPPROC
        log_msg ("Incorrect Uintvar in SERVER_SDU_SIZE\n");
#endif
        return SDL_False;
      }
      src += n;
      remsize -= n;
      value = (value == 0) ? SMRU : value;
      *CurrentSMRU = MIN (value, SMRU);

      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      dst += IntToUintvar (*CurrentSMRU, dst);
      break;

    case CAP_PROTOCOL_OPTIONS:
      /* Protocol options are multiple octets, although presently
       * it is really only one byte. At least, we only care about
       * the first byte, the rest are simply copied through. */
      if (caplen < 2) {
#ifdef DEBUG_WSPPROC
        log_msg ("Too small remaining size in PROTOCOL_OPTIONS.\n");
#endif
        return SDL_False;
      }
      *MdfBy |= MDF_PROTOCOL_OPTIONS;
      value = *src++;
      remsize--;
      *CurrentProtoOpt &= value;
      *ConfirmedPushEnabled = (*CurrentProtoOpt & OPT_CONFIRMED_PUSH) ?
        SDL_True : SDL_False;
      *PushEnabled = (*CurrentProtoOpt & OPT_PUSH) ? SDL_True : SDL_False;
      *SessionResumeEnabled = (*CurrentProtoOpt & OPT_SESSION_RESUME) ?
        SDL_True : SDL_False;

      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      *dst++ = *CurrentProtoOpt;
      B_COPYSTRINGN (dst, src, caplen - 2);
      src += caplen - 2;
      remsize -= caplen - 2;
      dst += caplen - 2;
      break;

    case CAP_METHOD_MOR:
      /* Method-MOR is a UINT8. */
      if (caplen < 2) {
#ifdef DEBUG_WSPPROC
        log_msg ("Too small remaining size in METHOD_MOR\n");
#endif
        return SDL_False;
      }
      *MdfBy |= MDF_METHOD_MOR;
      value = *src++;
      remsize--;
      *CurrentMOM = MIN (value, MOM);

      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      *dst++ = (BYTE)*CurrentMOM;
      break;

    case CAP_PUSH_MOR:
      /* Push-MOR is a UINT8. */
      if (caplen < 2) {
#ifdef DEBUG_WSPPROC
        log_msg ("Too small remaining size in PUSH_MOR\n");
#endif
        return SDL_False;
      }
      *MdfBy |= MDF_PUSH_MOR;
      value = *src++;
      remsize--;
      *CurrentMOP = MIN (value, MOP);

      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      *dst++ = (BYTE)*CurrentMOP;
      break;

    case CAP_ALIASES:
      /* Aliases are multiple octets, but we just copy it as it stands. */
      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      B_COPYSTRINGN (dst, src, caplen - 1);
      src += caplen - 1;
      remsize -= caplen - 1;
      dst += caplen - 1;
      break;

    default:
      /* An unknown capability identifier. */
      /* Simply copy the whole thing. */
      dst += IntToUintvar (caplen, dst);
      *dst++ = capid;
      B_COPYSTRINGN (dst, src, caplen - 1);
      src += caplen - 1;
      remsize -= caplen - 1;
      dst += caplen - 1;
    }
  }

  *ReqCapLen -= (src - dst);
  return SDL_True;
}


/*
 * Builds and returns a buffer of capabilities.
 * The out-parameter RequstedCapabilities is set to point to
 * the newly allocated buffer, and ReqCapLen is set to the length
 * of the list of capabilities.
 * Which capabilities are included is governed by the bit maps
 * MdfByClient and MdfByServer, such that a capability is included
 * if and only if the corresponding bit is set in MdfByClient and
 * is not set in MdfByServer.
 * Returns FALSE on error, and TRUE otherwise.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned buffer, unless this routine returns FALSE.
 */
extern SDL_Boolean
ModifiedCap (SDL_Octet MdfByClient, SDL_Octet MdfByServer,
             SDL_Natural CurrentMOM, SDL_Natural CurrentMOP,
             SDL_Natural CurrentMRU, SDL_Natural CurrentSMRU,
             SDL_Octet CurrentProtoOpt,
             void **RequestedCapabilities, SDL_Natural *ReqCapLen)
{
  BYTE *dst;
  UINT32 n = 0;
  UINT32 i;

  /* We simply allocate a buffer of a size that we know is large enough,
   * although it might be too large of course. */
  if ((dst = NEWARRAY (BYTE, *ReqCapLen + 20)) == NULL) {
    return SDL_False;
  }
  B_COPYSTRINGN (dst, *RequestedCapabilities, *ReqCapLen);
  DEALLOC (RequestedCapabilities);
  *RequestedCapabilities = dst;
  dst += *ReqCapLen;

  if ((MdfByClient & MDF_METHOD_MOR) && !(MdfByServer & MDF_METHOD_MOR)) {
    *dst++ = 2;
    *dst++ = 0x80 | CAP_METHOD_MOR;
    *dst++ = (BYTE)CurrentMOM;
    n += 3;
  }

  if ((MdfByClient & MDF_PUSH_MOR) && !(MdfByServer & MDF_PUSH_MOR)) {
    *dst++ = 2;
    *dst++ = 0x80 | CAP_PUSH_MOR;
    *dst++ = (BYTE)CurrentMOP;
    n += 3;
  }

  if ((MdfByClient & MDF_CLIENT_SDU) && !(MdfByServer & MDF_CLIENT_SDU)) {
    i = 1 + UintvarLen (CurrentMRU);
    *dst++ = (BYTE)i;
    *dst++ = 0x80 | CAP_CLIENT_SDU_SIZE;
    dst += IntToUintvar (CurrentMRU, dst);
    n += (i + 1);
  }

  if ((MdfByClient & MDF_SERVER_SDU) && !(MdfByServer & MDF_SERVER_SDU)) {
    i = 1 + UintvarLen (CurrentSMRU);
    *dst++ = (BYTE)i;
    *dst++ = 0x80 | CAP_SERVER_SDU_SIZE;
    dst += IntToUintvar (CurrentSMRU, dst);
    n += (i + 1);
  }

  if ((MdfByClient & MDF_PROTOCOL_OPTIONS) &&
      !(MdfByServer & MDF_PROTOCOL_OPTIONS)) {
    *dst++ = 2;
    *dst++ = 0x80 | CAP_PROTOCOL_OPTIONS;
    *dst++ = CurrentProtoOpt;
    n += 3;
  }

  *ReqCapLen += n;

  return SDL_True;
}

#ifdef FOR_MS
#ifndef WAP_USE_SECURE
//========================================================================================================
//						 ST_NULL,ST_WAIT만 사용함 
//========================================================================================================
void wsp_send(unsigned char *ucpSendData, unsigned  int nSendLen)
{
	memset(Postdata,0,1500);
   	memcpy(Postdata, ucpSendData, nSendLen);
	postdatalen = nSendLen ;//Postdata에는 NULL terminated되어야 하는 거 같음..
	
	CLNTa_log(1,0,"Pass here POSTDATA nSendlen %d",nSendLen);	
}
#endif//#ifndef WAP_USE_SECURE
#endif//#ifdef FOR_MS
