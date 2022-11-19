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
 * wtpsar.c
 *
 * Created by Anders Edenbrandt, Tue Oct 17 08:45:52 2000.
 *
 * Revision history:
 *
 * 010517, MALU, Compiler-warnings removed.
 *
 */
// 2001/10/05 SAR support시 define해야 한다.
//#define SKT_SAR_TEST

#include <string.h>

#include "wtpsuppt.h"
#include "wtpsar.h"

#ifdef WTP_TEST_SAR
#define WTP_SAR_GROUP_SIZE          300
#define WTP_SAR_SEGMENT_SIZE         12
#endif

#define WTP_FLAG_RID           0x01
#define WTP_FLAG_TTR           0x02
#define WTP_FLAG_GTR           0x04

#define WTP_FLAG_MORE_DATA     0x08
#define WTP_FLAG_TID_NEW       0x10
#define WTP_FLAG_USER_ACK      0x20
#define WTP_FLAG_SPLIT_RESULT  0x40
#define WTP_FLAG_SENT_NACK     0x80

/*
 * Each invoke segment has a node of this type:
 */
typedef struct iseg_st {
  struct iseg_st *next;
  BYTE           *data;
  UINT16         length;
  UINT8          segnum;
  UINT8          flags;
} wtp_invoke_segment_t;

/*
 * A missing segment is simply a pointer to an invoke segment.
 */
typedef struct mseg_st {
  struct mseg_st       *next;
  wtp_invoke_segment_t *iseg;
} wtp_missing_segment_t;

/*
 * Each result segment has a node of this type:
 */
typedef struct rseg_st {
  struct rseg_st *next;
  pdubuf         *pdu;
  UINT8          segnum;
  UINT8          flags;
} wtp_result_segment_t;

/*
 * Each transaction has information stored in this structure:
 */
typedef struct {
  /* General info */
  UINT16                tid;
  UINT8                 transaction_class;
  UINT8                 flags;
  SDL_Integer           handle;

  /* Relevant to segmented invoke */
  pdubuf                *invoke_pdu;
  wtp_invoke_segment_t  *invoke_group;
  wtp_invoke_segment_t  *last_segment;
  wtp_missing_segment_t *missing_segments;
  UINT32                total_send_msg_size;
  UINT32                max_send_group_size;
  UINT32                delay_time;
  UINT8                 segnum;
  UINT8                 ack_psn;

  /* Relevant to segmented result */
  wtp_result_segment_t  *result_list;
  UINT32                total_recv_msg_size;
  UINT32                max_recv_group_size;
  UINT32                recv_msg_size;
  UINT32                recv_group_size;
  UINT8                 num_recv_segments;
  UINT8                 first_segnum_in_current_group;
  UINT8                 first_segnum_in_previous_group;
} wtp_transaction_t;


/*
 * Read an unsigned integer stored in "len" bytes
 * in big-endian order. Return the value read.
 */
static UINT32
wtp_get_uintn (UINT8 *p, UINT8 len)
{
  UINT32 n = 0;

  while (len-- > 0) {
    n <<= 8;
    n |= *p++;
  }

  return n;
}

/*
 * Store an unsigned integer, "n", in big-endian order
 * in "len" bytes, at the location indicated by "p".
 */
static void
wtp_put_uintn (UINT32 n, UINT8 len,  UINT8 *p)
{
  p += len - 1;
  while (len-- > 0) {
    *p-- = (UINT8)(n & 0xff);
    n >>= 8;
  }
}

/*
 * Lengths of PDU headers. Used by TPI-reader below.
 */
static UINT8 wtp_pdu_header_lengths[8] = {
  0,
  WTP_PDU_INVOKE_HEADERLEN,
  WTP_PDU_RESULT_HEADERLEN,
  WTP_PDU_ACK_HEADERLEN,
  WTP_PDU_ABORT_HEADERLEN,
  WTP_PDU_SEGMENTED_INVOKE_HEADERLEN,
  WTP_PDU_SEGMENTED_RESULT_HEADERLEN,
  WTP_PDU_NACK_HEADERLEN};

/*
 * Read eventual TPIs in a PDU, update the transaction state
 * accordingly, and delete PDU header and TPIs from the PDU.
 * Returns -1 in case of error, and 0 otherwise.
 */
static int
wtp_read_tpis (wtp_transaction_t *tr, pdubuf *pdu)
{
  UINT8  more_tpis = 0, long_tpi;
  BYTE   *pb = pdubuf_getStart (pdu), *ip;
  UINT16 remaining_length = pdubuf_getLength (pdu);
  UINT8  pdu_type, tpi_id, tpi_length, option_id;
  UINT16 len;

  more_tpis = (pb[0] >> 7) & 0x1;
  pdu_type = (UINT8)((pb[0] >> 3) & 0x7);
  len = wtp_pdu_header_lengths[pdu_type];
  if (pdu_type == WTP_PDU_TYPE_NACK)
    len += pb[3];

  pb += len;
  remaining_length -= len;

  while (more_tpis) {
    if (remaining_length < 1)
      return -1;
    tpi_id = (pb[0] >> 3) & 0xf;
    long_tpi = (pb[0] >> 2) & 0x1;
    if (long_tpi) {
      if (remaining_length < 2)
        return -1;
      tpi_length = pb[1];
      if (remaining_length < tpi_length + 2)
        return -1;
    }
    else {
      tpi_length = pb[0] & 0x3;
      if (remaining_length < tpi_length + 1)
        return -1;
    }
    more_tpis = (pb[0] >> 7) & 0x1;

    switch (tpi_id) {
    case WTP_TPI_OPTION:
      if (long_tpi) {
        option_id = pb[2];
        ip = pb + 3;
      }
      else {
        option_id = pb[1];
        ip = pb + 2;
      }
      switch (option_id) {
      case WTP_TPI_OPTION_TOTAL_MESSAGE_SIZE:
        if (tpi_length > 4)
          return -1;
        tr->total_recv_msg_size = wtp_get_uintn (ip, (UINT8)tpi_length);
        break;

      case WTP_TPI_OPTION_DELAY_TRANS_TIMER:
        if (tpi_length > 4)
          return -1;
        tr->delay_time = wtp_get_uintn (ip, (UINT8)tpi_length);
        break;
        
      case WTP_TPI_OPTION_MAX_GROUP_SIZE:
        if (tpi_length > 4)
          return -1;
        tr->max_send_group_size = wtp_get_uintn (ip, (UINT8)tpi_length);
        break;

      default:
        /* Ignore other Option TPIs */
        break;
      }
      break;

    case WTP_TPI_ERROR:
      /* Ignore */
      break;

    case WTP_TPI_INFO:
      /* Ignore */
      break;

    case WTP_TPI_PSN:
      if (long_tpi) {
        tr->ack_psn = (UINT8)wtp_get_uintn (pb + 2, (UINT8)(tpi_length - 1));
      }
      else if (tpi_length == 1) {
        tr->ack_psn = pb[1];
      }
      else {
        tr->ack_psn = (UINT8)wtp_get_uintn (pb + 1, (UINT8)(tpi_length - 1));
      }
      break;

    default:
      return -1;
    }
    if (long_tpi) {
      pb += tpi_length + 2;
      remaining_length -= tpi_length + 2;
    }
    else {
      pb += tpi_length + 1;
      remaining_length -= tpi_length + 1;
    }
  }
  pdubuf_setLength (pdu, (UINT16)remaining_length);

  return 0;
}

/*
 * Initialize a transaction structure.
 * Returns a pointer that is used in subsequent calls to
 * refer to this transaction.
 */
void *
wtp_transaction_init (SDL_Natural tid,
                      SDL_Boolean tid_new,
                      TRInvokeReqType *req)
{
  wtp_transaction_t *tr;

  tr = (wtp_transaction_t*)OSConnectorAlloc (sizeof (wtp_transaction_t));
  tr->tid = (UINT16)tid;
  tr->transaction_class = (UINT8)req->ClassType;
  tr->handle = req->Handle;

  tr->flags = (tid_new == SDL_True) ? WTP_FLAG_TID_NEW : 0;
  tr->flags |= (req->AckType == 1) ? WTP_FLAG_USER_ACK : 0;

#ifdef SKT_SAR_TEST
  tr->flags |= (req->AllowSegmentedResult == SDL_True)? WTP_FLAG_SPLIT_RESULT : 0;
  tr->flags |= (req->MoreData == SDL_True) ? WTP_FLAG_MORE_DATA : 0;
#endif  //SKT_SAR_TEST
  tr->invoke_pdu = req->UserData;
  tr->invoke_group = NULL;
  tr->last_segment = NULL;
  tr->missing_segments = NULL;
  tr->total_send_msg_size = req->TotalSize;
  tr->max_send_group_size = WTP_SAR_GROUP_SIZE;
  tr->delay_time = 0;
  tr->segnum = 0;

  tr->result_list = NULL;
  tr->total_recv_msg_size = 0;
  tr->max_recv_group_size = WTP_SAR_GROUP_SIZE;
  tr->recv_msg_size = 0;
  tr->recv_group_size = 0;
  tr->num_recv_segments = 0;
  tr->first_segnum_in_current_group = 0;
  tr->first_segnum_in_previous_group = 0;

  return (void *)tr;
}

/*
 * Delete an invoke segment.
 */
static void
wtp_delete_invoke_segment (wtp_invoke_segment_t *seg)
{
  if (seg) {
    OSConnectorFree (seg);
  }
}

/*
 * Delete a result segment.
 */
static void
wtp_delete_result_segment (wtp_result_segment_t *seg)
{
  if (seg) {
    if (seg->pdu)
      pdubuf_release (seg->pdu);
    OSConnectorFree (seg);
  }
}

/*
 * Delete a transaction structure.
 */
void
wtp_delete_transaction (void *trp)
{
  wtp_transaction_t    *tr = (wtp_transaction_t *)trp;
  wtp_invoke_segment_t *iseg;
  wtp_result_segment_t *rseg;

  while (tr->invoke_group != NULL) {
    iseg = tr->invoke_group;
    tr->invoke_group = iseg->next;
    wtp_delete_invoke_segment (iseg);
  }    

  while (tr->result_list != NULL) {
    rseg = tr->result_list;
    tr->result_list = rseg->next;
    wtp_delete_result_segment (rseg);
  }    
  if (tr->invoke_pdu)
    pdubuf_release (tr->invoke_pdu);

  OSConnectorFree (tr);
}

/*
 * Create an Invoke PDU for a class 0 transaction.
 */
pdubuf *
wtp_create_invoke_pdu0 (void *trp)
{
  wtp_transaction_t *tr = (wtp_transaction_t *)trp;
  pdubuf            *pdu = tr->invoke_pdu;
  BYTE              *pb;

  /* We re-use the original PDU buffer in case it can be extended
   * the required amount. */
  tr->invoke_pdu = NULL;
  if (pdubuf_changeLength (pdu, WTP_PDU_INVOKE_HEADERLEN) == NULL) {
    UINT16 length = pdubuf_getLength (pdu);
    pdubuf *tmp_pdu = pdubuf_new ((UINT16)(length + WTP_PDU_INVOKE_HEADERLEN));
    if (tmp_pdu == NULL) {
      pdubuf_release (pdu);
      return NULL;
    }
    pdubuf_setLength (tmp_pdu, (UINT16)(length + WTP_PDU_INVOKE_HEADERLEN));
    pb = pdubuf_getStart (tmp_pdu);
    memcpy (pb + WTP_PDU_INVOKE_HEADERLEN, pdubuf_getStart (pdu), length);
    pdubuf_release (pdu);
    pdu = tmp_pdu;
  }
  pb = pdubuf_getStart (pdu);

  pb[0] = (WTP_PDU_TYPE_INVOKE << 3) | 0x02;
  pb[1] = (tr->tid >> 8);
  pb[2] = tr->tid & 0xff;
  pb[3] = 0;
  if (tr->flags & WTP_FLAG_TID_NEW)
    pb[3] |= 0x20;
  if (tr->flags & WTP_FLAG_USER_ACK)
    pb[3] |= 0x10;

  return pdu;
}


/************************************************************
 * First come the routines used for segmented invoke.
 ************************************************************/

/*
 * Routine called when a segmented invoke request arrives.
 */
void
wtp_segmented_invoke (void *trp, TRInvokeReqType *inv_req)
{
  wtp_transaction_t *tr = (wtp_transaction_t *)trp;

  if (inv_req->MoreData == SDL_False)
    tr->flags &= ~WTP_FLAG_MORE_DATA;
  
  if (tr->invoke_pdu != NULL)
    pdubuf_release (tr->invoke_pdu);
  tr->invoke_pdu = inv_req->UserData;
  tr->invoke_group = NULL;
  tr->last_segment = NULL;
}

/*
 * Create an invoke PDU from the indicated segment.
 */
static pdubuf *
wtp_create_invoke_pdu (wtp_transaction_t *tr, wtp_invoke_segment_t *iseg)
{
  pdubuf *pdu;
  BYTE   *pb;
  UINT16 length;
  UINT16 add_tpis;

  add_tpis = ((iseg->segnum == 0) && (tr->flags & WTP_FLAG_SPLIT_RESULT));
  length = (UINT16)(iseg->length + WTP_PDU_INVOKE_HEADERLEN);
  if (add_tpis) {
    /* This is the first segment in an Invoke.
     * We should add TPIs. */
    if (tr->total_send_msg_size > 0) {
      length += 4;
      if (tr->total_send_msg_size > 0xffff)
        length += 3;
    }
    length += 4;
    if (tr->max_recv_group_size > 0xffff)
      length += 3;
  }
  pdu = pdubuf_new (length);
  pdubuf_setLength (pdu, length);
  pb = pdubuf_getStart (pdu);

  if (iseg->segnum > 0) {
    pb[0] = (WTP_PDU_TYPE_SEGMENTED_INVOKE << 3);
    pb[3] = iseg->segnum;
  }
  else {
    pb[0] = (WTP_PDU_TYPE_INVOKE << 3);
    pb[3] = tr->transaction_class;
    if (tr->flags & WTP_FLAG_TID_NEW)
      pb[3] |= 0x20;
    if (tr->flags & WTP_FLAG_USER_ACK)
      pb[3] |= 0x10;
  }
  pb[1] = (tr->tid >> 8);
  pb[2] = tr->tid & 0xff;

#ifdef SKT_SAR_TEST
  if (iseg->flags & WTP_FLAG_GTR)
#endif //#ifdef SKT_SAR_TEST
    pb[0] |= 0x04;

#ifdef SKT_SAR_TEST
  if (iseg->flags & WTP_FLAG_TTR)
#endif //#ifdef SKT_SAR_TEST
    pb[0] |= 0x02;

  if (add_tpis) {
    pb[0] |= 0x80;
    pb += WTP_PDU_INVOKE_HEADERLEN;
    if (tr->total_send_msg_size > 0) {
      if (tr->total_send_msg_size > 0xffff) {
        pb[0] = (WTP_TPI_OPTION << 3) | 0x04 | 0x80;
        pb[1] = 5;
        pb[2] = WTP_TPI_OPTION_TOTAL_MESSAGE_SIZE;
        wtp_put_uintn (tr->total_send_msg_size, 4, pb + 3);
        pb += 7;
      }
      else {
        pb[0] = (WTP_TPI_OPTION << 3) | 0x03 | 0x80;
        pb[1] = WTP_TPI_OPTION_TOTAL_MESSAGE_SIZE;
        wtp_put_uintn (tr->total_send_msg_size, 2, pb + 2);
        pb += 4;
      }
    }

    if (tr->max_recv_group_size > 0xffff) {
      pb[0] = (WTP_TPI_OPTION << 3) | 0x04;
      pb[1] = 5;
      pb[2] = WTP_TPI_OPTION_MAX_GROUP_SIZE;
      wtp_put_uintn (tr->max_recv_group_size, 4, pb + 3);
      pb += 7;
    }
    else {
      pb[0] = (WTP_TPI_OPTION << 3) | 0x03;
      pb[1] = WTP_TPI_OPTION_MAX_GROUP_SIZE;
      wtp_put_uintn (tr->max_recv_group_size, 2, pb + 2);
      pb += 4;
    }
  }
  else
    pb += WTP_PDU_INVOKE_HEADERLEN;

  memcpy (pb, iseg->data, iseg->length);

  return pdu;
}

/*
 * Return a copy of the last invoke segment that was sent.
 */
pdubuf *
wtp_get_last_segment (void *trp)
{
  wtp_transaction_t *tr = (wtp_transaction_t *)trp;

  if (tr->last_segment == NULL)
    return NULL;

  return wtp_create_invoke_pdu (tr, tr->last_segment);
}

/*
 * Create next group.
 */
void
wtp_create_group (void *trp)
{
  wtp_transaction_t    *tr = (wtp_transaction_t *)trp;
  wtp_invoke_segment_t *iseg, *q = NULL;
  pdubuf               *pdu = tr->invoke_pdu;
  UINT16               length = pdubuf_getLength (pdu);
  UINT32               remaining_group_size;

  if (length == 0) {
    tr->invoke_group = NULL;
    tr->last_segment = NULL;
    return;
  }

  if ((tr->flags & WTP_FLAG_MORE_DATA) ||
      (length > tr->max_send_group_size) ||
      (tr->segnum > 0)) {
    /* We must use segmented invoke */

    /* Let first group have only one segment, to make sure
     * that responder can handle it. */
    if (tr->segnum == 0) {
      length = MIN (WTP_SAR_SEGMENT_SIZE, length);
    }
    remaining_group_size = tr->max_send_group_size;
    while (length > 0) {
      iseg = (wtp_invoke_segment_t*)OSConnectorAlloc (sizeof (wtp_invoke_segment_t));
      iseg->next = NULL;
      iseg->data = pdubuf_getStart (pdu);;
      iseg->flags = 0;
      iseg->length = MIN (WTP_SAR_SEGMENT_SIZE, length);
      iseg->length = (UINT16)MIN(iseg->length, remaining_group_size);
      iseg->segnum = tr->segnum++;
      length -= iseg->length;
      pdubuf_changeLength (pdu, (INT16)-((INT16)(iseg->length)));
      if (q == NULL)
        tr->invoke_group = iseg;
      else
        q->next = iseg;
      q = iseg;
      remaining_group_size -= iseg->length;
      if (remaining_group_size  == 0) {
        break;
      }
    }
    if ((tr->flags & WTP_FLAG_MORE_DATA) ||
        (pdubuf_getLength (tr->invoke_pdu) > 0)) {
      q->flags = WTP_FLAG_GTR;
    }
    else {
      q->flags = WTP_FLAG_TTR;
    }
  }
  else {
    /* Send entire invoke in one PDU */
    iseg = (wtp_invoke_segment_t*)OSConnectorAlloc (sizeof (wtp_invoke_segment_t));
    iseg->next = NULL;
    iseg->data = pdubuf_getStart (pdu);
    iseg->flags = WTP_FLAG_TTR;
    iseg->length = length;
    pdubuf_changeLength (pdu, (INT16)(-(INT16)length));
    iseg->segnum = tr->segnum++;
    tr->invoke_group = iseg;
  }
}

/*
 * Delete this transaction's current group of invoke segments.
 */
void
wtp_delete_group (void *trp)
{
  wtp_transaction_t    *tr = (wtp_transaction_t *)trp;
  wtp_invoke_segment_t *p;

  if (tr->last_segment == NULL)
    return;
  
  while (tr->invoke_group != NULL) {
    p = tr->invoke_group;
    tr->invoke_group = p->next;
    wtp_delete_invoke_segment (p);
  }
  tr->last_segment = NULL;
}

/*
 * Return the next segment to be sent in the current group.
 * Returns NULL in case there are no more segments waiting
 * to be transmitted.
 */
pdubuf *
wtp_get_next_segment (void *trp,
                      SDL_Boolean *group_done,
                      SDL_Boolean *msg_done)
{
  wtp_transaction_t    *tr = (wtp_transaction_t *)trp;
  wtp_invoke_segment_t *iseg;

  if (tr->last_segment == NULL)
    iseg = tr->invoke_group;
  else
    iseg = tr->last_segment->next;
  if (iseg == NULL)
    return NULL;
  tr->last_segment = iseg;

  *group_done = (iseg->flags & (WTP_FLAG_GTR | WTP_FLAG_TTR))
    ? SDL_True : SDL_False;
  *msg_done = (iseg->flags & WTP_FLAG_TTR) ? SDL_True : SDL_False;

  return wtp_create_invoke_pdu (tr, iseg);
}


/*
 * Unpack an ACK PDU and possible TPIs. Delete the PDU
 * afterwards. In case this ACK has a PSN TPI, check that
 * the sequence numbers agree.
 * Returns -1 on error, and 0 otherwise.
 */
SDL_Integer
wtp_unpack_ack_pdu (void *trp,
                    pdubuf *pdu,
                    SDL_Boolean *tidve,
                    SDL_Duration *delay_time)
{
  wtp_transaction_t *tr = (wtp_transaction_t *)trp;
  BYTE              *pb;

  if ((pdu == NULL) || (pdubuf_getLength (pdu) < WTP_PDU_ACK_HEADERLEN)) {
    pdubuf_release (pdu);
    return WTP_RET_ERROR;
  }
  pb = pdubuf_getStart (pdu);
  *tidve = (pb[0] & 0x04) ? SDL_True : SDL_False;

  tr->delay_time = 0;
  tr->ack_psn = 0;
  if (wtp_read_tpis (tr, pdu) < 0) {
    pdubuf_release (pdu);
    return WTP_RET_ERROR;
  }
  pdubuf_release (pdu);

  if ((tr->ack_psn > 0) && (tr->last_segment != NULL) &&
      (tr->ack_psn != tr->last_segment->segnum)) {
    return WTP_RET_ERROR;
  }
  if (tr->delay_time > 0) {
#ifdef XSCT_CMICRO
    *delay_time = SDL_DURATION_LIT (0, tr->delay_time, 0);
#else
    *delay_time = SDL_DURATION_LIT (0, tr->delay_time / 10,
                                    (tr->delay_time % 10) * 100000000);
#endif
  }
  else
    *delay_time = SDL_DURATION_LIT (0, 0, 0);

  return WTP_RET_OK;
}

/*
 * Delete the list of missing segments.
 */
static void
wtp_delete_missing_segments (wtp_transaction_t *tr)
{
  wtp_missing_segment_t *mseg;

  while (tr->missing_segments != NULL) {
    mseg = tr->missing_segments;
    tr->missing_segments = mseg->next;
    OSConnectorFree (mseg);
  }
}

/*
 * Unpack a NACK PDU and all TPIs. Deletes the PDU
 * afterwards. Creates a list of missing segments
 * and stores with this transaction.
 */
void
wtp_unpack_nack_pdu (void *trp, pdubuf *pdu)
{
  wtp_transaction_t     *tr = (wtp_transaction_t *)trp;
  wtp_invoke_segment_t  *iseg;
  wtp_missing_segment_t *mseg, *q;
  BYTE                  *pb;
  UINT8                 n;
  UINT8                 segnum;
  int                   i;

  pb = pdubuf_getStart (pdu);
  n = pb[3];
  tr->missing_segments = NULL;
  if (n + 3 > pdubuf_getLength (pdu)) {
    return;
  }

  wtp_delete_missing_segments (tr);
  q = NULL;
  iseg = tr->invoke_group;
  for (i = 4; i < n + 4; i++) {
    segnum = pb[i];
    for (; iseg != NULL; iseg = iseg->next) {
      if (iseg->segnum == segnum) {
        mseg = (wtp_missing_segment_t*)OSConnectorAlloc (sizeof (wtp_missing_segment_t));
        mseg->iseg = iseg;
        mseg->next = NULL;
        if (q == NULL)
          tr->missing_segments = mseg;
        else
          q->next = mseg;
        q = mseg;
        iseg = iseg->next;
        break;
      }
    }
  }
  wtp_read_tpis (tr, pdu);

  pdubuf_release (pdu);
}

/*
 * Return a copy of the first segment on the list of
 * missing segments. Deletes this segment from the list.
 * Returns NULL if the list is empty.
 */
pdubuf *
wtp_next_missing_segment (void *trp)
{
  wtp_transaction_t     *tr = (wtp_transaction_t *)trp;
  wtp_invoke_segment_t  *iseg;
  wtp_missing_segment_t *mseg;

  if (tr->missing_segments == NULL)
    return NULL;

  mseg = tr->missing_segments;
  iseg = mseg->iseg;
  tr->missing_segments = mseg->next;
  OSConnectorFree (mseg);

  return wtp_create_invoke_pdu (tr, iseg);
}


/*************************************************************
 * The routines below are used when receiving a
 * segmented result.
 ************************************************************/

/*
 * Return the sequence number of the last segment
 * in a completeley received segmented result.
 * Returns 0 if the result was not segmented.
 */
SDL_Natural
wtp_get_last_recv_seqnum (void *trp)
{
  wtp_transaction_t *tr = (wtp_transaction_t *)trp;

  return tr->first_segnum_in_current_group + tr->num_recv_segments - 1;
}


/*
 * Update parameters to make transaction ready to receive the next group.
 */
static void
wtp_group_done (wtp_transaction_t *tr, UINT16 segnum)
{
  tr->recv_group_size = 0;
  tr->num_recv_segments = 0;
  tr->first_segnum_in_previous_group = tr->first_segnum_in_current_group;
  tr->first_segnum_in_current_group = segnum + 1;
  tr->flags &= ~WTP_FLAG_SENT_NACK;
}

/*
 * Unpack the header of a result PDU.
 * Returns NULL in case of error,
 * otherwise returns a segment representing the original PDU.
 */
static wtp_result_segment_t *
wtp_unpack_result_pdu (pdubuf *pdu)
{
  wtp_result_segment_t *rseg = NULL;
  BYTE                 *pb = pdubuf_getStart (pdu);
  UINT16               length = pdubuf_getLength (pdu);
  UINT8                pdu_type;

  if (length < 3)
    goto err_return;

  rseg = (wtp_result_segment_t*)OSConnectorAlloc (sizeof (wtp_result_segment_t));
  rseg->pdu = pdu;

  pdu_type = (pb[0] >> 3) & 0xf;
  rseg->flags = pb[0] & 0x7;

  if (pdu_type == WTP_PDU_TYPE_SEGMENTED_RESULT) {
    if (length < 4)
      goto err_return;
    rseg->segnum = pb[3];
  }
  else if (pdu_type == WTP_PDU_TYPE_RESULT) {
    rseg->segnum = 0;
  }
  else {
    goto err_return;
  }

  return rseg;

 err_return:
  pdubuf_release (pdu);
  if (rseg)
    OSConnectorFree (rseg);
  return NULL;
}

/*
 * Add a segment to a transaction's list of saved segments.
 * Returns 0 if the segment could be added,
 *         1 if the segment belongs to an old grop already completed,
 *     and 2 if the segment is a duplicate in the current group.
 */
static int
wtp_add_result_segment (wtp_transaction_t *tr,
                        wtp_result_segment_t *rseg)
{
  wtp_result_segment_t *p, *q;
  UINT16               size;
  BYTE                 *pb;

  if (rseg->segnum < tr->first_segnum_in_current_group) {
    /* Old segment belonging to a group already completed. */
    return 1;
  }
  for (q = NULL, p = tr->result_list; p != NULL; q = p, p = p->next) {
    if (p->segnum == rseg->segnum) {
      /* Duplicate segment. */
      return 2;
    }
    if (p->segnum > rseg->segnum) {
      break;
    }
  }

  /* Insert before p */
  rseg->next = p;
  if (q)
    q->next = rseg;
  else
    tr->result_list = rseg;

  pb = pdubuf_getStart (rseg->pdu);
  size = pdubuf_getLength (rseg->pdu) - WTP_PDU_RESULT_HEADERLEN;
  if (((pb[0] >> 3) & 0xf) == WTP_PDU_TYPE_SEGMENTED_RESULT)
    size--;
  tr->recv_msg_size += size;
  tr->recv_group_size += size;
  tr->num_recv_segments += 1;
  tr->flags &= ~WTP_FLAG_SENT_NACK;

  return 0;
}

/*
 * Check if we have all segments belonging to the current group.
 * Returns NULL if there are segments missing, and
 * a pointer to the last segment in the group otherwise.
 */
static wtp_result_segment_t *
wtp_check_group_complete (wtp_transaction_t *tr)
{
  wtp_result_segment_t *rseg;

  for (rseg = tr->result_list; rseg->next != NULL; rseg = rseg->next);

  if ((rseg->flags & (WTP_FLAG_GTR | WTP_FLAG_TTR)) &&
      (rseg->segnum - tr->first_segnum_in_current_group + 1 ==
       tr->num_recv_segments)) {
    return rseg;
  }

  return NULL;
}

/*
 * Create a NACK PDU, holding a list of all missing segments
 * in the current group.
 * This routine is allowed to assume that the last segment
 * in the list of segments has the GTR or TTR flag set; that is,
 * the last segment in the list is also the last segment of
 * the current group.
 */
static void
wtp_create_nack_pdu (wtp_transaction_t *tr, UINT8 resend,
                     pdubuf **nack_pdu_ptr)
{
  pdubuf        *pdu;
  BYTE          *pb;
  UINT8         num_missing_segments;
  wtp_result_segment_t *first_seg = NULL, *last_seg, *seg;
  UINT8         i, exp_segnum;

  /* Find first and last received segments in current group */
  for (seg = tr->result_list; seg != NULL; seg = seg->next) {
    if ((seg->segnum == tr->first_segnum_in_current_group) ||
        ((first_seg == NULL) &&
         (seg->segnum > tr->first_segnum_in_current_group)))
      first_seg = seg;
    if (seg->next == NULL)
      last_seg = seg;
  }

  num_missing_segments = last_seg->segnum
    - tr->first_segnum_in_current_group + 1 - tr->num_recv_segments;

  pdu = pdubuf_new ((UINT16)(4 + num_missing_segments));
  pdubuf_setLength (pdu, (UINT16)(4 + num_missing_segments));
  pb = pdubuf_getStart (pdu);

  pb[0] = (WTP_PDU_TYPE_NACK << 3);
  if (resend)
    pb[0] |= 0x01;
  pb[1] = (tr->tid >> 8);
  pb[2] = (tr->tid & 0xff);
  pb[3] = num_missing_segments;

  i = 4;
  exp_segnum = tr->first_segnum_in_current_group;
  seg = first_seg;
  while (seg) {
    if (exp_segnum < seg->segnum) {
      pb[i++] = exp_segnum;
    }
    else {
      seg = seg->next;
    }
    exp_segnum += 1;
  }

  tr->flags |= WTP_FLAG_SENT_NACK;
  *nack_pdu_ptr = pdu;
}

/*
 * Create a result PDU-buffer, holding the contents of
 * either all segments in the current group, or all segments
 * from the very beginning.
 */
static pdubuf *
wtp_create_result_sdu (wtp_transaction_t *tr)
{
  pdubuf        *pdu;
  UINT16        result_size, n, sn;
  BYTE          *pb;
  wtp_result_segment_t *seg;

  if (tr->result_list->segnum > 0)
    result_size = (UINT16)tr->recv_group_size;
  else
    result_size = (UINT16)tr->recv_msg_size;

  if (tr->result_list->next == NULL) {
    /* Only one segment, re-use the PDU-buffer. */
    seg = tr->result_list;
    tr->result_list = NULL;
    pdu = seg->pdu;
    sn = seg->segnum;
    OSConnectorFree (seg);
  }
  else {
    pdu = pdubuf_new (result_size);
    pdubuf_setLength (pdu, result_size);
    pb = pdubuf_getStart (pdu);
  
    while (tr->result_list != NULL) {
      seg = tr->result_list;
      tr->result_list = seg->next;
      sn = seg->segnum;
      n = pdubuf_getLength (seg->pdu);
      memcpy (pb, pdubuf_getStart (seg->pdu), n);
      pb += n;
      pdubuf_release (seg->pdu);
      OSConnectorFree (seg);
    }
  }
  wtp_group_done (tr, sn);

  return pdu;
}

/*
 * Create an ACK PDU that serves as acknowledgement
 * of segment number segnum.
 */
static void
wtp_sar_create_ack_pdu (wtp_transaction_t *tr,
                        UINT8 segnum, UINT8 resend,
                        pdubuf **ack_pdu_ptr)
{
  pdubuf *pdu;
  BYTE   *pb;
  UINT16 length = 5;
  UINT16 add_tpis;

  add_tpis = (tr->first_segnum_in_current_group == 0) ||
    (resend && (tr->first_segnum_in_previous_group == 0));

  if (add_tpis) {
    /* This is the first ACK to a segmented result.
     * We should add TPIs. */
    length += 4;
    if (tr->max_recv_group_size > 0xffff)
      length += 3;
  }
  pdu = pdubuf_new (length);
  pdubuf_setLength (pdu, length);
  pb = pdubuf_getStart (pdu);
  
  pb[0] = 0x80 | (WTP_PDU_TYPE_ACK << 3);
  if (resend)
    pb[0] |= 0x01;
  pb[1] = (tr->tid >> 8);
  pb[2] = (tr->tid & 0xff);
  pb[3] = (WTP_TPI_PSN << 3) | 0x01;
  pb[4] = segnum;

  if (add_tpis) {
    pb[3] |= 0x80;
    pb += 5;
    if (tr->max_recv_group_size > 0xffff) {
      pb[0] = (WTP_TPI_OPTION << 3) | 0x04;
      pb[1] = 5;
      pb[2] = WTP_TPI_OPTION_MAX_GROUP_SIZE;
      wtp_put_uintn (tr->max_recv_group_size, 4, pb + 3);
    }
    else {
      pb[0] = (WTP_TPI_OPTION << 3) | 0x3;
      pb[1] = WTP_TPI_OPTION_MAX_GROUP_SIZE;
      wtp_put_uintn (tr->max_recv_group_size, 2, pb + 2);
    }
  }

  *ack_pdu_ptr = pdu;
}

/*
 * This routine is called when a result (segment) has arrived.
 * The following actions are performed:
 *
 *   Unpack the PDU header.
 *
 *   Try to add this segment to the list of previously saved segments.
 *   If it is a duplicate,
 *     discard the segment,
 *     return WTP_RET_WAIT_RESULT, unless it is a resend of the last
 *     message with the GTR or TTR flag set, in which case return
 *     WTP_RET_SEND_ACK.
 *
 *   If the total message size now exceeds our limit, OR
 *   if the total group size exceeds our limit, OR if the packet
 *   has the GTR or TTR flag set even though we have already received
 *   packets with higher sequence numbers, then
 *     return WTP_RET_ERROR.
 *
 *   Update local parameters according to the TPIs.
 *
 *   Check if all segments of the current group are now present.
 *
 *   If all segments in the current group are NOT present, then
 *     if it is the last segment of a group, or last in message, then
 *       create a NACK pdu and return WTP_RET_SEND_ACK
 *     else
 *       return WTP_RET_WAIT_RESULT.
 *
 *   If this is the last segment in the message, create a result pdu
 *   with the present segments, and return WTP_RET_MESSAGE_COMPLETED.
 *
 *   If this is the end of a group (but not the last one), then
 *     if we are doing segmented result-delivery,
 *       create a result pdu and return WTP_RET_GROUP_COMPLETED.
 *     else
 *       create an ACK pdu
 *       return WTP_RET_SEND_ACK.
 *
 */
SDL_Integer
wtp_process_result_pdu (void *trptr,
                        pdubuf *pdu,
                        pdubuf **ack_pdu_ptr,
                        TRResultIndType *result_ind,
                        SDL_Natural *abort_code)
{
  wtp_transaction_t    *tr = (wtp_transaction_t *)trptr;
  wtp_result_segment_t *rseg, *last_seg;
  int                  r, retval = WTP_RET_WAIT_RESULT;

  *ack_pdu_ptr = NULL;
  result_ind->UserData = NULL;

  rseg = wtp_unpack_result_pdu (pdu);
  if (rseg == NULL) {
    wtp_delete_transaction (tr);
    *abort_code = WTP_ERR_PROTOERR;
    return WTP_RET_ERROR;
  }
  r = wtp_add_result_segment (tr, rseg);
  if (r) {
    if ((rseg->flags & WTP_FLAG_RID) &&
        (rseg->flags & (WTP_FLAG_GTR | WTP_FLAG_TTR))) {
      /* This packet is a duplicate, but since it has the GTR or TTR
       * flag set, we may have to acknowledge it anyway. */
      if ((r == 1)
          && (rseg->segnum + 1 == tr->first_segnum_in_current_group)
          && (tr->num_recv_segments == 0)) {
        wtp_sar_create_ack_pdu (tr, rseg->segnum, 1, ack_pdu_ptr);
        retval = WTP_RET_SEND_ACK;
      }
      else if (r == 2) {
        wtp_create_nack_pdu (tr, (UINT8)(tr->flags & WTP_FLAG_SENT_NACK),
                             ack_pdu_ptr);
        retval = WTP_RET_SEND_ACK;
      }
    }
    wtp_delete_result_segment (rseg);
    return retval;
  }

  if (tr->recv_group_size > tr->max_recv_group_size) {
    wtp_delete_transaction (tr);
    *abort_code = WTP_ERR_MESSAGETOOLARGE;
    return WTP_RET_ERROR;
  }

  if ((rseg->flags & (WTP_FLAG_GTR | WTP_FLAG_TTR)) && (rseg->next != NULL)) {
    wtp_delete_transaction (tr);
    *abort_code = WTP_ERR_PROTOERR;
    return WTP_RET_ERROR;
  }

  if (wtp_read_tpis (tr, rseg->pdu) < 0) {
    wtp_delete_transaction (tr);
    *abort_code = WTP_ERR_PROTOERR;
    return WTP_RET_ERROR;
  }

  last_seg = wtp_check_group_complete (tr);

  if (!last_seg) {
    if (rseg->flags & (WTP_FLAG_GTR | WTP_FLAG_TTR)) {
      wtp_create_nack_pdu (tr, 0, ack_pdu_ptr);
      return WTP_RET_SEND_ACK;
    }
    return WTP_RET_WAIT_RESULT;
  }

  if (last_seg->flags & WTP_FLAG_TTR) {
    result_ind->UserData = wtp_create_result_sdu (tr);
    result_ind->Handle = tr->handle;
    result_ind->MoreData = SDL_False;
    result_ind->TotalSize = tr->total_recv_msg_size;
    return WTP_RET_MESSAGE_COMPLETED;
  }

  if (tr->flags & WTP_FLAG_SPLIT_RESULT) {
    wtp_sar_create_ack_pdu (tr, last_seg->segnum, 0, ack_pdu_ptr);
    result_ind->UserData = wtp_create_result_sdu (tr);
    result_ind->Handle = tr->handle;
    result_ind->MoreData = SDL_True;
    result_ind->TotalSize = tr->total_recv_msg_size;
    return WTP_RET_GROUP_COMPLETED;
  }

  wtp_sar_create_ack_pdu (tr, last_seg->segnum, 0, ack_pdu_ptr);
  wtp_group_done (tr, last_seg->segnum);

  return WTP_RET_SEND_ACK;
}
