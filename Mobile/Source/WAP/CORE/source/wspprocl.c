/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * All rights reserved.
 */

/*
 * WSPproCL.c
 *
 * Procedures for WSP Connectionless
 *
 * Created by Anders Edenbrandt, March 1999.
 *
 * Revision history:
 *     990423, AED:  Changed to pass data as void* between WAE and WSP,
 *                    instead of using MBM buffers.
 *     990909, AED:  Changed to use PDU buffers instead of MBM buffers.
 *	   990928, FKN:  Added ExtractPushPDU_CL
 *
 */

#include "cmmnrsrc.h"

#include "wspprocl.h"
#if defined LOG_INTERNAL
#include "aapiclnt.h"
#endif


/*
 * Check address fields.
 * Returns FALSE if the addresses have incorrect syntax.
 */
extern SDL_Boolean
ValidAddressFieldsFromWAE (SUnitMethodInvokeReqType *SUnitMethodInvokeReqData)
{
#if defined LOG_INTERNAL
  CLNTa_log (0, 0, "WSP: Procedure ValidAddressFields in C!\n");
#endif
  if ((SUnitMethodInvokeReqData->ServerAddress.Address.length != 0) &&
      (SUnitMethodInvokeReqData->ClientAddress.Address.length != 0)) {

#if defined LOG_INTERNAL
    CLNTa_log (0, 0, "WSP Addressfield from WAE OK  \n");
#endif
    return SDL_True;
  }
#if defined LOG_INTERNAL
  CLNTa_log (0, 0, "WSP UnValidAddressFields from WAE \n");
#endif
  return SDL_False;
}


/*
 * Create a PDU for a GET request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL if memory allocation fails.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateGetPDU_CL (SUnitMethodInvokeReqType *SUnitMethodInvokeReqData)
{
  UINT32 headlen;       /* Length of header */
  UINT32 URIlen;        /* Length of URI */
  UINT16 PDUlen;        /* Calculated length of PDU */
  pdubuf *PDU;          /* PDU buffer */
  BYTE   *pb;

  headlen = SUnitMethodInvokeReqData->HeadersLen;
  URIlen = SUnitMethodInvokeReqData->URILen;
  PDUlen = (UINT16)(headlen + UintvarLen (URIlen) + URIlen + 2);

  /* Get new PDU buffer, of size PDUlen */
  if ((PDU = pdubuf_new (PDUlen)) == NULL) {
    /* Error, no buffers available. */
#if defined LOG_INTERNAL
    CLNTa_log (0, 0, "WSP CreateGetPDU: unable to allocate new PDU buffer.\n");
#endif
    return NULL;
  }
  pdubuf_setLength (PDU, PDUlen);
  pb = pdubuf_getStart (PDU);

  /* Place Transaction ID, PDU type, URIlen, RequestURI,
     and RequestHeaders into PDU */
  *pb++ = (SDL_Octet)SUnitMethodInvokeReqData->TransactionId;
  *pb++ = (SDL_Octet)64;
  pb += IntToUintvar (URIlen, pb);
  /* Removed in 1.1: pb += IntToUintvar (headlen, pb); */
  B_COPYSTRINGN (pb, SUnitMethodInvokeReqData->RequestURI, URIlen);
  pb += URIlen;
  B_COPYSTRINGN (pb, SUnitMethodInvokeReqData->RequestHeaders, headlen);

  /* Release buffers given as in-parameters. */
  DEALLOC (&SUnitMethodInvokeReqData->RequestURI);
  DEALLOC (&SUnitMethodInvokeReqData->RequestHeaders);

#if defined LOG_INTERNAL
  CLNTa_log (0, 0, "WSP Get PDU created successfully in C!\n");
#endif

  return PDU;
}


/*
 * Create a PDU for a POST request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL if memory allocation fails.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreatePostPDU_CL (SUnitMethodInvokeReqType *SUnitMethodInvokeReqData)
{
  SDL_Integer URIlen;        /* Length of URI */
  SDL_Integer headlen;       /* Length of header */
  SDL_Integer bodylen;       /* Length of body */
  UINT16      PDUlen;        /* Calculated length of PDU */
  pdubuf      *PDU;          /* PDU buffer */
  BYTE        *pb;

  URIlen = SUnitMethodInvokeReqData->URILen;
  headlen = SUnitMethodInvokeReqData->HeadersLen;
  bodylen = SUnitMethodInvokeReqData->BodyLen;

  PDUlen = (UINT16)(UintvarLen (URIlen) + URIlen + UintvarLen (headlen)
                    + headlen + bodylen + 2);

  /* Get new PDU buffer, of size PDUlen */
  if ((PDU = pdubuf_new (PDUlen)) == NULL) {
    /* Error, no buffers available. */
#if defined LOG_INTERNAL
    CLNTa_log (0, 0, "WSP CreatePostPDU: unable to allocate new PDU buffer.\n");
#endif
    return NULL;
  }
  pdubuf_setLength (PDU, PDUlen);
  pb = pdubuf_getStart (PDU);

  /* Place Transaction ID, PDU type, URIlen, headlen, RequestURI,
     RequestHeaders, and RequestBody into PDU */
  *pb++ = (SDL_Octet)SUnitMethodInvokeReqData->TransactionId;
  *pb++ = (SDL_Octet)96;
  pb += IntToUintvar (URIlen, pb);
  pb += IntToUintvar (headlen, pb);
  B_COPYSTRINGN (pb, SUnitMethodInvokeReqData->RequestURI, URIlen);
  pb += URIlen;
  B_COPYSTRINGN (pb, SUnitMethodInvokeReqData->RequestHeaders, headlen);
  pb += headlen;
  B_COPYSTRINGN (pb, SUnitMethodInvokeReqData->RequestBody, bodylen);

  /* Release buffers given as in-parameters. */
  DEALLOC (&SUnitMethodInvokeReqData->RequestURI);
  DEALLOC (&SUnitMethodInvokeReqData->RequestHeaders);
  DEALLOC (&SUnitMethodInvokeReqData->RequestBody);

#if defined LOG_INTERNAL
  CLNTa_log (0, 0, "WSP Post PDU created successfully in C!\n");
#endif

  return PDU;
}


/*
 * Extract the fields from a reply PDU.
 * Copies extracted data to newly allocated buffers in the parameter
 * "SUnitMethodResultIndData", and deallocates the given PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer is not
 * released.
 * NOTE: it is the caller's responsibility to deallocate the buffers
 * in the result structure.
 */
extern SDL_Boolean
ExtractReplyPDU_CL (pdubuf *PDU,
                    SUnitMethodResultIndType *SUnitMethodResultIndData)
{
  BYTE      *pb = pdubuf_getStart (PDU);
  SDL_Octet PDU_type;
  UINT16    SDU_size = pdubuf_getLength (PDU);
  UINT32    headlen, bodylen;
  UINT32    hn;

  if ((pb == NULL) || (SDU_size <= 3)) { return FALSE; }

  SUnitMethodResultIndData->TransactionId = *pb++;
  PDU_type = *pb++;
  SUnitMethodResultIndData->Status = *pb++;

  if (PDU_type != 4) { return FALSE; }
  if ((hn = UintvarToInt (pb, &headlen, 5)) == 0) { return FALSE; }
  pb += hn;
  if ((int)(headlen + 3 + hn) > SDU_size) { return FALSE; }
  bodylen = SDU_size - 3 - hn - headlen;

  /* Get new buffers and copy data into them. */
  if (headlen > 0) {
    SUnitMethodResultIndData->ResponseHeaders = NEWARRAY (BYTE, headlen);

    if (SUnitMethodResultIndData->ResponseHeaders == NULL) {
      /* Error, no memory available. */
#if defined LOG_INTERNAL
      CLNTa_log (0, 0, "WSP ExtractReplyPDU: unable to allocate memory.\n");
#endif
      return FALSE;
    }
    B_COPYSTRINGN (SUnitMethodResultIndData->ResponseHeaders, pb, headlen);
    pb += headlen;
  }
  else
    SUnitMethodResultIndData->ResponseHeaders = NULL;

  if (bodylen > 0) {
    SUnitMethodResultIndData->ResponseBody = NEWARRAY (BYTE, bodylen);

    if (SUnitMethodResultIndData->ResponseBody == NULL) {
      /* Error, no memory available. */
#if defined LOG_INTERNAL
      CLNTa_log (0, 0, "WSP ExtractReplyPDU: unable to allocate memory.\n");
#endif
      DEALLOC (&SUnitMethodResultIndData->ResponseHeaders);
      return FALSE;
    }
    B_COPYSTRINGN (SUnitMethodResultIndData->ResponseBody, pb, bodylen);
  }
  else
    SUnitMethodResultIndData->ResponseBody = NULL;

  SUnitMethodResultIndData->HeadersLen = headlen;
  SUnitMethodResultIndData->BodyLen = bodylen;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (PDU);

#if defined LOG_INTERNAL
  CLNTa_log (0, 0, "WSP Reply PDU extracted successfully in C!\n");
#endif

  return TRUE;
}

/*
 * Extract the fields from a push PDU.
 * Copies extracted data to newly allocated buffers in the parameter
 * "SUnitPushIndData", and deallocates the given PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer is not
 * released.
 * NOTE: it is the caller's responsibility to deallocate the buffers
 * in the result structure.
 */
extern SDL_Boolean
ExtractPushPDU_CL (pdubuf *PDU,
                    SUnitPushIndType *SUnitPushIndData)
{
  BYTE      *pb = pdubuf_getStart (PDU);
  SDL_Octet PDU_type;
  UINT16    SDU_size = pdubuf_getLength (PDU);
  UINT32    headlen, bodylen;
  UINT32    hn;

  if ((pb == NULL) || (SDU_size <= 2)) { return FALSE; }

  SUnitPushIndData->PushId = *pb++;
  PDU_type = *pb++;

  if (PDU_type != 6) { return FALSE; }
  if ((hn = UintvarToInt (pb, &headlen, 4)) == 0) { return FALSE; }
  pb += hn;
  if ((int)(headlen + 2 + hn) > SDU_size) { return FALSE; }
  bodylen = SDU_size - 2 - hn - headlen;

  /* Get new buffers and copy data into them. */
  if (headlen > 0) {
    SUnitPushIndData->PushHeaders = NEWARRAY (BYTE, headlen);

    if (SUnitPushIndData->PushHeaders == NULL) {
      /* Error, no memory available. */
#if defined LOG_INTERNAL
      CLNTa_log (0, 0, "WSP ExtractPushPDU: unable to allocate memory.\n");
#endif
      return FALSE;
    }
    B_COPYSTRINGN (SUnitPushIndData->PushHeaders, pb, headlen);
    pb += headlen;
  }
  else
    SUnitPushIndData->PushHeaders = NULL;

  if (bodylen > 0) {
    SUnitPushIndData->PushBody = NEWARRAY (BYTE, bodylen);

    if (SUnitPushIndData->PushBody == NULL) {
      /* Error, no memory available. */
#if defined LOG_INTERNAL
      CLNTa_log (0, 0, "WSP ExtractPushPDU: unable to allocate memory.\n");
#endif
      DEALLOC (&SUnitPushIndData->PushHeaders);
      return FALSE;
    }
    B_COPYSTRINGN (SUnitPushIndData->PushBody, pb, bodylen);
  }
  else
    SUnitPushIndData->PushBody = NULL;

  SUnitPushIndData->HeadersLen = headlen;
  SUnitPushIndData->BodyLen = bodylen;

  /* Release PDU buffer given as in-parameter. */
  pdubuf_release (PDU);

#if defined LOG_INTERNAL
  CLNTa_log (0, 0, "WSP Reply PDU extracted successfully in C!\n");
#endif

  return TRUE;
}