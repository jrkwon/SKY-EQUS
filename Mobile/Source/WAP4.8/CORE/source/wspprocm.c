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
 * wspprocm.c
 *
 * Procedures for WSP Connection oriented.
 *
 * Created by Anders Edenbrandt, May 1999.
 *
 * Revision history:
 *   990915, AED: Changed from using MBM buffers to PDU buffers.
 *   001027, HEAD: Added two functions for SAR.
 *   001208, AED: Simplified by using the new common conversion routines.
 *                Removed debugging code.
 *   001211, AED: New way of handling capabilities.
 *
 */
#include "confvars.h"
#include "ansilibs.h"

#include "wspprocm.h"
#include "pdubuf.h"
#include "wapcvt.h"
#include "wapcommon.h"

#ifndef MIN
#define MIN(x, y)     (((x) < (y)) ? (x) : (y))
#endif

#ifdef FEATURE_WAP_REDIRECT
	BOOL   isPost = FALSE;
	UINT32 s_postlen = 0;
	BYTE   s_postdata[1500];
	extern redirect_config redirect;	
#endif //FEATURE_WAP_REDIRECT


/*
 * The intention was that WSP should allocate a large enough
 * PDU buffer so that the added WTP headers should fit,
 * withouth any new memory allocation. However, with the
 * introduction of SAR, this is only used for class 0 transactions.
 */
#define WTP_MAX_HEADER_SIZE  5


static INT16
wsp_create_capabilities_list (SDL_Natural client_sdu_size,
                              SDL_Natural server_sdu_size,
                              SDL_Octet protocol_options,
                              SDL_Octet method_mor,
                              SDL_Octet push_mor,
                              void **caplist, SDL_Natural *caplen);

static INT16
wsp_check_capabilities_list (wap_cvt_t *obj, SDL_Natural caplen,
                             SDL_Natural *client_sdu_size,
                             SDL_Natural *server_sdu_size,
                             SDL_Octet *protocol_options,
                             SDL_Octet *method_mor,
                             SDL_Octet *push_mor);

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
ExtractReplyPDU (pdubuf *pdu,
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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type;

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));

  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      (pdu_type != REPLY_PDU_TYPE) ||
      !wap_cvt_uint8 (&cvt_obj, Status) ||
      !wap_cvt_uintvar (&cvt_obj, HeadLen))
    return SDL_False;

  if (*HeadLen + cvt_obj.pos > cvt_obj.length)
    return SDL_False;
  *DataLen = cvt_obj.length - cvt_obj.pos - *HeadLen;

  if (!wap_cvt_bytevector (&cvt_obj, *HeadLen, (BYTE **)Headers) ||
      !wap_cvt_bytevector (&cvt_obj, *DataLen, (BYTE **)Data))
    return SDL_False;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (pdu);
  
  return SDL_True;
}

/*
 * Extract the fields from a Data PDU.
 * Sets a VOIDSTAR out-parameter and one integer out-parameter
 * (containing data from the PDU as well as the length) and then
 * releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffer
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractDataPDU (pdubuf *pdu, VOIDSTAR *Data, SDL_Natural *DataLen)
{
/*
 * The contents of a Data PDU is the following:
 *     Octets   Data
 */
  UINT32 pdulen = pdubuf_getLength (pdu);

  *DataLen = pdulen;

  if (pdulen > 0) {
    *Data = OSConnectorAlloc (pdulen);

    if (*Data == NULL) {
      /* Error, no memory available. */
      return SDL_False;
    }
    B_COPYSTRINGN (*Data, pdubuf_getStart (pdu), pdulen);
  }
  else
    *Data = NULL;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (pdu);
  
  return SDL_True;
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
ExtractPushPDU (pdubuf *pdu,
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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type;

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));

  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      ((pdu_type != PUSH_PDU_TYPE) &&
       (pdu_type != CONFIRMED_PUSH_PDU_TYPE)) ||
      !wap_cvt_uintvar (&cvt_obj, HeadLen))
    return SDL_False;

  if (*HeadLen + cvt_obj.pos > cvt_obj.length)
    return SDL_False;
  *DataLen = cvt_obj.length - cvt_obj.pos - *HeadLen;

  if (!wap_cvt_bytevector (&cvt_obj, *HeadLen, (BYTE **)Headers) ||
      !wap_cvt_bytevector (&cvt_obj, *DataLen, (BYTE **)Data))
    return SDL_False;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (pdu);
  
  return SDL_True;
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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type = CONNECT_PDU_TYPE;
  UINT8     protocol_version = WSP_PROTOCOL_VERSION;
  BYTE     *caplist = NULL;
  UINT32    caplen;     /* Length of capabilities */
  UINT32    headlen;    /* Length of header */
  UINT32    pdulen;     /* Calculated length of PDU */
  pdubuf   *pdu;        /* PDU buffer */

#ifdef FEATURE_WAP
  if (!wsp_create_capabilities_list (SConnectReqData->ClientSDUSize,
                                     SConnectReqData->ServerSDUSize,
                                     SConnectReqData->ProtocolOptions,
                                     SConnectReqData->MethodMOR,
                                     SConnectReqData->PushMOR,
                                     (void **)&caplist, &caplen)) {
#else
  if (!wsp_create_capabilities_list (SConnectReqData->ClientSDUSize,
                                     SConnectReqData->ServerSDUSize,
                                     SConnectReqData->ProtocolOptions,
                                     SConnectReqData->MethodMOR,
                                     SConnectReqData->PushMOR,
                                     &caplist, &caplen)) {
#endif //#ifdef FEATURE_WAP

    return NULL;
  }
                                    
  headlen = SConnectReqData->HeadersLen;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uint8 (&cvt_obj, &protocol_version) ||
      !wap_cvt_uintvar (&cvt_obj, &caplen) ||
      !wap_cvt_uintvar (&cvt_obj, &headlen) ||
      !wap_cvt_bytevector (&cvt_obj, caplen, NULL) ||
      !wap_cvt_bytevector (&cvt_obj, headlen, NULL))
    return NULL;
      
  pdulen = cvt_obj.pos;

  /* Get new PDU buffer, of size PDUlen */
  if ((pdu = pdubuf_new ((UINT16)(pdulen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (pdu, (UINT16)pdulen);

  /* Place PDU type, protocol version, capabilities length, headers length,
     capabilities and headers in the PDU. */
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uint8 (&cvt_obj, &protocol_version) ||
      !wap_cvt_uintvar (&cvt_obj, &caplen) ||
      !wap_cvt_uintvar (&cvt_obj, &headlen) ||
      !wap_cvt_bytevector (&cvt_obj, caplen, &caplist) ||
      !wap_cvt_bytevector (&cvt_obj, headlen,
                           (BYTE **)&SConnectReqData->ClientHeaders))
    return NULL;

  /* Release buffer given as in-parameter. */
  DEALLOC (&caplist);
  DEALLOC (&SConnectReqData->ClientHeaders);

  return pdu;
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
ExtractConnectReplyPDU (pdubuf *pdu,
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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type;
  UINT32    caplen;

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));

  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      (pdu_type != CONNECTREPLY_PDU_TYPE) ||
      !wap_cvt_uintvar (&cvt_obj, ServerSessionId) ||
      !wap_cvt_uintvar (&cvt_obj, &caplen) ||
      !wap_cvt_uintvar (&cvt_obj, &SConnectCnfData->HeadersLen))
    return SDL_False;

  if ((caplen + SConnectCnfData->HeadersLen + cvt_obj.pos) > cvt_obj.length)
    return SDL_False;

  if (!wsp_check_capabilities_list (&cvt_obj, caplen,
                                    &SConnectCnfData->ClientSDUSize,
                                    &SConnectCnfData->ServerSDUSize,
                                    &SConnectCnfData->ProtocolOptions,
                                    &SConnectCnfData->MethodMOR,
                                    &SConnectCnfData->PushMOR))
    return SDL_False;

  if (!wap_cvt_bytevector (&cvt_obj,
                           SConnectCnfData->HeadersLen,
                           (BYTE **)&SConnectCnfData->ServerHeaders))
    return SDL_False;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (pdu);
  
  return SDL_True;
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
  wap_cvt_t cvt_obj;
  UINT32    sid = (UINT32)SessionID;
  UINT8     pdu_type = DISCONNECT_PDU_TYPE;
  pdubuf   *pdu;
  UINT32    pdulen;        /* Calculated length of PDU */

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &sid))
    return NULL;

  pdulen = cvt_obj.pos;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers */
  if ((pdu = pdubuf_new ((UINT16)(pdulen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (pdu, (UINT16)pdulen);

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, pdubuf_getStart (pdu), pdulen);
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &sid))
    return NULL;

  return pdu;
}

/*
 * Extract the fields from a Disconnect PDU.
 * Sets the session ID value, and then releases the PDU buffer.
 * In case of error, the PDU buffer is NOT released.
 * Returns FALSE on error, TRUE otherwise.
 */
extern SDL_Boolean
ExtractDisconnectPDU (pdubuf *pdu,
                      SDL_Natural *ServerSessionId)
{
/*
 * A Disconnect PDU has the following content:
 *     UINT8    PDU type (always 5)
 *     UintVar  Server Session ID
 */
  wap_cvt_t cvt_obj;
  UINT8     pdu_type;

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      (pdu_type != DISCONNECT_PDU_TYPE) ||
      !wap_cvt_uintvar (&cvt_obj, ServerSessionId))
    return SDL_False;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (pdu);

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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type = SMethodInvokeReqData->Method;
  UINT32    urilen;
  UINT32    headlen;  /* Length of header */
  UINT32    pdulen;   /* Calculated length of PDU */
  pdubuf   *pdu;      /* PDU buffer */

  urilen = SMethodInvokeReqData->URILen;
  headlen = SMethodInvokeReqData->HeadersLen;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &urilen) ||
      !wap_cvt_bytevector (&cvt_obj, urilen, NULL) ||
      !wap_cvt_bytevector (&cvt_obj, headlen, NULL))
    return NULL;

  pdulen = cvt_obj.pos;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers. */
  if ((pdu = pdubuf_new ((UINT16)(pdulen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (pdu, (UINT16)pdulen);

  /* Place PDU type, URI length, Headers length, Request URI,
     Request headers, and Request body in the PDU */
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &urilen) ||
      !wap_cvt_bytevector (&cvt_obj, urilen,
                           (BYTE **)&SMethodInvokeReqData->RequestURI) ||
      !wap_cvt_bytevector (&cvt_obj, headlen,
                           (BYTE **)&SMethodInvokeReqData->RequestHeaders))
    return NULL;

  /* Release buffers given as in-parameters. */
  DEALLOC (&SMethodInvokeReqData->RequestURI);
  DEALLOC (&SMethodInvokeReqData->RequestHeaders);
  DEALLOC (&SMethodInvokeReqData->RequestBody);

  return pdu;
}

/*
 * Create a PDU for a Post request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
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
  wap_cvt_t cvt_obj;
  pdubuf   *pdu;      /* PDU buffer */
  UINT8     pdu_type = POST_PDU_TYPE;
  UINT32    pdulen;   /* Calculated length of PDU */
  UINT32    urilen;
  UINT32    headlen;  /* Length of header */
  UINT32    bodylen;

#ifdef FEATURE_WAP_REDIRECT
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
#endif //FEATURE_WAP_REDIRECT

  urilen = SMethodInvokeReqData->URILen;
  headlen = SMethodInvokeReqData->HeadersLen;
  bodylen = SMethodInvokeReqData->BodyLen;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &urilen) ||
      !wap_cvt_uintvar (&cvt_obj, &headlen) ||
      !wap_cvt_bytevector (&cvt_obj, urilen, NULL) ||
      !wap_cvt_bytevector (&cvt_obj, headlen, NULL) ||
      !wap_cvt_bytevector (&cvt_obj, bodylen, NULL))
    return NULL;
  pdulen = cvt_obj.pos;

  /* Get new PDU buffer, of size PDUlen plus romm for WTP headers. */
  if ((pdu = pdubuf_new ((UINT16)(pdulen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (pdu, (UINT16)pdulen);

  /* Place PDU type, URI length, Headers length, Request URI,
     Request headers, and Request body in the PDU */
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, pdubuf_getStart (pdu), pdulen);

#ifdef FEATURE_WAP_REDIRECT
  if(isPost && redirect.bCorrect)
  {
	CLNTa_log(1,0,"REDIRECT WSPPROC_CM.C bodylen %d",bodylen);
	B_COPYSTRINGN (SMethodInvokeReqData->RequestBody,s_postdata,bodylen);
  }
#endif//#ifdef FEATURE_WAP_REDIRECT
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &urilen) ||
      !wap_cvt_uintvar (&cvt_obj, &headlen) ||
      !wap_cvt_bytevector (&cvt_obj, urilen,
                           (BYTE **)&SMethodInvokeReqData->RequestURI) ||
      !wap_cvt_bytevector (&cvt_obj, headlen,
                           (BYTE **)&SMethodInvokeReqData->RequestHeaders) ||
      !wap_cvt_bytevector (&cvt_obj, bodylen,
                           (BYTE **)&SMethodInvokeReqData->RequestBody))
    return NULL;

#ifdef FEATURE_WAP_REDIRECT
  if(isPost)
  {
	isPost = FALSE;	
	memset(s_postdata,0x00,1500);
    s_postlen  = 0;
  }
#endif//FEATURE_WAP_REDIRECT

  /* Release buffers given as in-parameters. */
  DEALLOC (&SMethodInvokeReqData->RequestURI);
  DEALLOC (&SMethodInvokeReqData->RequestHeaders);
  DEALLOC (&SMethodInvokeReqData->RequestBody);

  return pdu;
}

/*
 * Create a PDU for more Data.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateMoreDataPDU (SMethodInvokeReqType *SMethodInvokeReqData)
{
/*
 * A MoreData PDU has the following content:
 *     Octets  Data
 */
  pdubuf *pdu;     /* PDU buffer */
  UINT32  datalen;

  datalen = SMethodInvokeReqData->BodyLen;

  /* Get new PDU buffer, of size datalen plus room for WTP headers. */
  if ((pdu = pdubuf_new ((UINT16)(datalen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (pdu, (UINT16)datalen);

  /* Place data in the PDU. */
  memcpy (pdubuf_getStart (pdu), SMethodInvokeReqData->RequestBody, datalen);

  /* Release buffers given as in-parameters. */
  DEALLOC (&SMethodInvokeReqData->RequestBody);

  return pdu;
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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type = SUSPEND_PDU_TYPE;
  UINT32    pdulen;    /* Calculated length of PDU */
  pdubuf   *pdu;       /* PDU buffer */

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &sessionId))
    return NULL;

  pdulen = cvt_obj.pos;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers. */
  if ((pdu = pdubuf_new ((UINT16)(pdulen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (pdu, (UINT16)pdulen);

  /* Place PDU type and session Id in the PDU */
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &sessionId))
    return NULL;

  return pdu;
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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type = RESUME_PDU_TYPE;
  UINT32    caplen = 0;
  UINT32    headlen;
  UINT32    pdulen;        /* Calculated length of PDU */
  pdubuf   *pdu;           /* PDU buffer */

  /* caplen = SResumeReqData->CapabilitiesLen; */
  headlen = SResumeReqData->HeadersLen;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &sessionId) ||
      !wap_cvt_uintvar (&cvt_obj, &caplen) ||
      !wap_cvt_bytevector (&cvt_obj, caplen, NULL) ||
      !wap_cvt_bytevector (&cvt_obj, headlen, NULL))
    return NULL;

  pdulen = cvt_obj.pos;

  /* Get new PDU buffer, of size PDUlen plus room for WTP headers. */
  if ((pdu = pdubuf_new ((UINT16)(pdulen + WTP_MAX_HEADER_SIZE))) == NULL) {
    /* Error, no buffers available. */
    return NULL;
  }
  pdubuf_setLength (pdu, (UINT16)pdulen);

  /* Place PDU type, session Id, capabilites length, capabilites and
   * headers in the PDU. */
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));
  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      !wap_cvt_uintvar (&cvt_obj, &sessionId) ||
      !wap_cvt_uintvar (&cvt_obj, &caplen) ||
      !wap_cvt_bytevector (&cvt_obj, caplen, NULL) ||
      /* !wap_cvt_bytevector (&cvt_obj, caplen,
         &SResumeReqData->RequestedCapabilities) || */
      !wap_cvt_bytevector (&cvt_obj, headlen,
                           (BYTE **)&SResumeReqData->ClientHeaders))
    return NULL;


  /* Release buffers given as in-parameters. */
  /* DEALLOC (&SResumeReqData->RequestedCapabilities); */
  DEALLOC (&SResumeReqData->ClientHeaders);

  return pdu;
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

#ifdef FEATURE_WAP_REDIRECT
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
		redirect.Address =(CHAR*)wip_malloc(redirect.Addresslen+1);
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
#endif//#ifdef FEATURE_WAP_REDIRECT

extern SDL_Boolean
ExtractRedirectPDU (pdubuf *pdu, SDL_Octet *ReasonCode,
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
  wap_cvt_t cvt_obj;
  UINT8     pdu_type;
  UINT8     flags;

#ifdef FEATURE_WAP_REDIRECT
  isfirstdirect = TRUE;
  init_buffer(&redirect);
#endif//#ifdef FEATURE_WAP_REDIRECT

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE,
                pdubuf_getStart (pdu), pdubuf_getLength (pdu));

  if (!wap_cvt_uint8 (&cvt_obj, &pdu_type) ||
      (pdu_type != REDIRECT_PDU_TYPE) ||
      !wap_cvt_uint8 (&cvt_obj, &flags))
    return SDL_False;

#ifdef FEATURE_WAP_REDIRECT
  if (flags & REDIRECT_FLAGS_PERMANENT) 
  {
	CLNTa_log(1,0,"Permanent FLAG bit");
    *ReasonCode = 0x31;
	redirect.bPerm_or_not = TRUE;
  }
  else
  {
	CLNTa_log(1,0,"temp FLAG bit");
    *ReasonCode = 0x32;	
	redirect.bPerm_or_not = FALSE;
  }
#else //FEATURE_WAP_REDIRECT
  *ReasonCode = (flags & REDIRECT_FLAGS_PERMANENT) ? 0x31 : 0x32;
#endif //FEATURE_WAP_REDIRECT
  *RedirectSecurity =
    (flags & REDIRECT_FLAGS_REUSE_SEC) ? SDL_True : SDL_False;

  *AddressLen = cvt_obj.length - cvt_obj.pos;
  if (!wap_cvt_bytevector (&cvt_obj, *AddressLen,
                           (BYTE **)RedirectedAddresses))
    return SDL_False;

#ifdef FEATURE_WAP_REDIRECT	  	
  if(ExtractPdu(RedirectedAddresses)==TRUE)
  	return SDL_False;  	
#endif//#ifdef FEATURE_WAP_REDIRECT

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (pdu);

  return SDL_True;
}

typedef struct {
  UINT8  id;
  union {
    UINT32 value32;
    UINT8  value8;
  } _u;
} wsp_capability_t;

static INT16
wsp_cvt_capability (wap_cvt_t *obj, wsp_capability_t *p)
{
  UINT32 length, oldlen;

  switch (obj->operation) {
  case WAP_CVT_DECODE:
  case WAP_CVT_DECODE_STATIC:
    if (!wap_cvt_uintvar (obj, &length) ||
        !wap_cvt_uint8 (obj, &p->id) ||
        (length + obj->pos > obj->length + 1))
      return FALSE;

    oldlen = obj->length;
    obj->length = obj->pos + length - 1;
    switch (p->id) {
    case CAP_CLIENT_SDU_SIZE:
    case CAP_SERVER_SDU_SIZE:
      if (!wap_cvt_uintvar (obj, &p->_u.value32))
        return FALSE;
      break;

    case CAP_METHOD_MOR:
    case CAP_PUSH_MOR:
      if (!wap_cvt_uint8 (obj, &p->_u.value8))
        return FALSE;
      break;

    case CAP_PROTOCOL_OPTIONS:
      if (!wap_cvt_uint8 (obj, &p->_u.value8))
        return FALSE;
      break;
    }
    obj->pos = obj->length;
    obj->length = oldlen;
    break;

  case WAP_CVT_ENCODE:
  case WAP_CVT_ENCODE_SIZE:
    switch (p->id) {
    case CAP_CLIENT_SDU_SIZE:
    case CAP_SERVER_SDU_SIZE:
      length = 1 + wap_cvt_uintvar_len (p->_u.value32);
      if (!wap_cvt_uintvar (obj, &length) ||
          !wap_cvt_uint8 (obj, &p->id) ||
          !wap_cvt_uintvar (obj, &p->_u.value32))
        return FALSE;
      break;

    default:
      length = 2;
      if (!wap_cvt_uintvar (obj, &length) ||
          !wap_cvt_uint8 (obj, &p->id) ||
          !wap_cvt_uint8 (obj, &p->_u.value8))
        return FALSE;
      break;
    }
    break;

  case WAP_CVT_FREE:
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Returns FALSE on error, TRUE otherwise.
 */
static INT16
wsp_create_capabilities_list (SDL_Natural client_sdu_size,
                              SDL_Natural server_sdu_size,
                              SDL_Octet protocol_options,
                              SDL_Octet method_mor,
                              SDL_Octet push_mor,
                              void **caplist, SDL_Natural *caplen)
{
  wap_cvt_t        cvt_obj;
  wsp_capability_t cap_client_sdu_size;
  wsp_capability_t cap_server_sdu_size;
#ifdef CONFIG_PUSH
  wsp_capability_t cap_protocol_options;
#endif
  wsp_capability_t cap_method_mor;
#ifdef CONFIG_PUSH
  wsp_capability_t cap_push_mor;
#endif


	push_mor = push_mor;					/*Added by integration, MALU*/
	protocol_options = protocol_options;	/*Added by integration, MALU*/

  cap_client_sdu_size.id = CAP_CLIENT_SDU_SIZE;
  cap_client_sdu_size._u.value32 = client_sdu_size;

  cap_server_sdu_size.id = CAP_SERVER_SDU_SIZE;
  cap_server_sdu_size._u.value32 = server_sdu_size;

#ifdef CONFIG_PUSH
  cap_protocol_options.id = CAP_PROTOCOL_OPTIONS;
  cap_protocol_options._u.value8 = protocol_options;
#endif

  cap_method_mor.id = CAP_METHOD_MOR;
  cap_method_mor._u.value8 = method_mor;

#ifdef CONFIG_PUSH
  cap_push_mor.id = CAP_PUSH_MOR;
  cap_push_mor._u.value8 = push_mor;
#endif

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wsp_cvt_capability (&cvt_obj, &cap_client_sdu_size)
      || !wsp_cvt_capability (&cvt_obj, &cap_server_sdu_size)
#ifdef CONFIG_PUSH
      || !wsp_cvt_capability (&cvt_obj, &cap_protocol_options)
#endif
      || !wsp_cvt_capability (&cvt_obj, &cap_method_mor)
#ifdef CONFIG_PUSH
      || !wsp_cvt_capability (&cvt_obj, &cap_push_mor)
#endif
      )
    return FALSE;

  *caplen = cvt_obj.pos;
  *caplist = OSConnectorAlloc (*caplen);

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, *caplist, *caplen);
  if (!wsp_cvt_capability (&cvt_obj, &cap_client_sdu_size)
      || !wsp_cvt_capability (&cvt_obj, &cap_server_sdu_size)
#ifdef CONFIG_PUSH
      || !wsp_cvt_capability (&cvt_obj, &cap_protocol_options)
#endif
      || !wsp_cvt_capability (&cvt_obj, &cap_method_mor)
#ifdef CONFIG_PUSH
      || !wsp_cvt_capability (&cvt_obj, &cap_push_mor)
#endif
      )
    return FALSE;

  return TRUE;
}

/*
 * Returns FALSE on error, and TRUE otherwise.
 */
static INT16
wsp_check_capabilities_list (wap_cvt_t *obj, SDL_Natural caplen,
                             SDL_Natural *client_sdu_size,
                             SDL_Natural *server_sdu_size,
                             SDL_Octet *protocol_options,
                             SDL_Octet *method_mor,
                             SDL_Octet *push_mor)
{
  wsp_capability_t cap;
  UINT32           savedlen;

  savedlen = obj->length;
  obj->length = obj->pos + caplen;
  while (obj->pos < obj->length) {
    if (!wsp_cvt_capability (obj, &cap))
      return FALSE;

    switch (cap.id) {
    case CAP_CLIENT_SDU_SIZE:
      if (cap._u.value32 != 0) {
        if (*client_sdu_size == 0)
          *client_sdu_size = cap._u.value32;
        else
          *client_sdu_size = MIN (*client_sdu_size, cap._u.value32);
      }
      break;

    case CAP_SERVER_SDU_SIZE:
      if (cap._u.value32 != 0) {
        if (*server_sdu_size == 0)
          *server_sdu_size = cap._u.value32;
        else
          *server_sdu_size = MIN (*server_sdu_size, cap._u.value32);
      }
      break;

    case CAP_PROTOCOL_OPTIONS:
      *protocol_options = cap._u.value8;
      break;

    case CAP_METHOD_MOR:
      if (cap._u.value8 != 0) {
        if (*method_mor == 0)
          *method_mor = cap._u.value8;
        else
          *method_mor = MIN (*method_mor, cap._u.value8);
      }
      break;
      

    case CAP_PUSH_MOR:
      if (cap._u.value8 != 0) {
        if (*push_mor == 0)
          *push_mor = cap._u.value8;
        else
          *push_mor = MIN (*push_mor, cap._u.value8);
      }
      break;

    default:
      /* Ignore */
      break;
    }
  }
  obj->length = savedlen;

  return TRUE;
}
