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

#include "CmmnRsrc.h"

#include "WSPproCL.h"
#if defined LOG_INTERNAL
#include "aapiclnt.h"
#endif

#include "ui.h"
#include "wip_mem.h"

//#include "custA7.h"
#if !(defined WAP_B1 || defined WAP_B2)
#include "custB0.h"
#endif //(MODEL_ID == MODEL_B0)

static ui_cmd_type  *ui_cmd_ptr;

#ifdef FOR_MS
extern BOOL wtls_enc_start;
extern BOOL Wtlshello;

#ifdef DYNAMIC
extern	BYTE   *Postdata = NULL;
#else
extern	BYTE   Postdata[1500];
#endif//#ifdef DYNAMIC

extern int postdatalen ;
extern void   wsp_send(unsigned char *ucpSendData, unsigned  int nSendLen);
#endif//#ifdef FOR_MS


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
      (SUnitMethodInvokeReqData->ClientAddress.Address.length != 0) &&
      ((SUnitMethodInvokeReqData->ServerAddress.Bearer < 4) ||
	   (SUnitMethodInvokeReqData->ServerAddress.Bearer == 0xFF))) {

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
extern BOOL   bCreateReq;
extern BOOL   initialaccess;
extern VOID   CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);
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

   
/*==========================================================
  일정 시간동안 몇개의 event를 동작하지 않도록 한다.(2/9)
 ==========================================================*/

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
/*==========================================================*/

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
#ifdef FOR_MS
static BYTE wcpRec[1500];   //Enc data..
extern int  ency_write(int *ipPrim,unsigned char *ucpSend, int nSendLen,int *npErr, char* Rec);
#endif//FOR_MS

extern pdubuf *
CreatePostPDU_CL (SUnitMethodInvokeReqType *SUnitMethodInvokeReqData)
{
  SDL_Integer URIlen;        /* Length of URI */
  SDL_Integer headlen;       /* Length of header */
  SDL_Integer bodylen;       /* Length of body */
  UINT16      PDUlen;        /* Calculated length of PDU */
  pdubuf      *PDU;          /* PDU buffer */
  BYTE        *pb;

#ifdef FOR_MS
  int   enc_length;
  int   err;
#ifndef WAP_USE_SECURE	
  if(Wtlshello)
  {
	SUnitMethodInvokeReqData->BodyLen = postdatalen;
  }
#endif//#ifndef WAP_USE_SECURE
#endif//#ifdef FOR_MS

  URIlen = SUnitMethodInvokeReqData->URILen;
  headlen = SUnitMethodInvokeReqData->HeadersLen;
  bodylen = SUnitMethodInvokeReqData->BodyLen;

#ifdef FOR_MS
  if(wtls_enc_start)
  {
	extern boolean bStopKey;

//jjkyg 2000/03/10 for stopkey
	bStopKey = TRUE;
	enc_length = 0;	
    
	memset(wcpRec,0x00,1500);//After ENC procedure data

	if(bodylen >1500 ) 
	{
		bStopKey = FALSE;return NULL;
	}
	//error발생시 처리  
	enc_length = ency_write(NULL,(unsigned char*)SUnitMethodInvokeReqData->RequestBody,bodylen,&err,(char*)wcpRec);
#ifdef WAP_USE_LEAK
	if(enc_length == 0)
	{
		return NULL;
	}
#endif//#ifdef WAP_USE_LEAK
	
	SUnitMethodInvokeReqData->BodyLen = enc_length;
	bodylen = enc_length;
	
  }   
#endif//#ifdef FOR_MS 


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

#ifdef FOR_MS
  if(wtls_enc_start)
  {
	B_COPYSTRINGN (pb, wcpRec, bodylen);
  }
#ifndef WAP_USE_SECURE
  else if(Wtlshello)
  {
	  CLNTa_log(1,0,"HERE IS WSPPROC_CL.C bodylen %d",bodylen);
	  B_COPYSTRINGN (pb,Postdata,bodylen);
  }
#endif//#ifndef WAP_USE_SECURE
  else
  {
	B_COPYSTRINGN (pb, SUnitMethodInvokeReqData->RequestBody,bodylen);
  }
#else
  B_COPYSTRINGN (pb, SUnitMethodInvokeReqData->RequestBody, bodylen);
#endif


  /* Release buffers given as in-parameters. */
  DEALLOC (&SUnitMethodInvokeReqData->RequestURI);
  DEALLOC (&SUnitMethodInvokeReqData->RequestHeaders);

#ifdef FOR_MS
#ifndef WAP_USE_SECURE
  if(!Wtlshello)
#endif//#ifndef WAP_USE_SECURE
  {
	DEALLOC(&SUnitMethodInvokeReqData->RequestBody);
  }

#else
  DEALLOC(&SUnitMethodInvokeReqData->RequestBody);
#endif//#ifdef FOR_MS


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

#ifdef FOR_CO
//========================================================================================================
//						 ST_NULL,ST_WAIT만 사용함 
//========================================================================================================
void wsp_send(unsigned char *ucpSendData, unsigned  int nSendLen)
{
#ifdef DYNAMIC
	if(Postdata!=NULL)
	{
		wip_free(Postdata);Postdata = NULL;
	}
	Postdata = (BYTE*)wip_malloc(nSendLen);//NEWARRAY(BYTE,nSendLen);
	memset(Postdata,0,nSendLen);
#else
	memset(Postdata,0,1500);
#endif
   	memcpy(Postdata, ucpSendData, nSendLen);
	postdatalen = nSendLen ;//Postdata에는 NULL terminated되어야 하는 거 같음..
	
	CLNTa_log(1,0,"Pass here POSTDATA nSendlen %d",nSendLen);	
}
#endif//#ifdef FOR_CO
