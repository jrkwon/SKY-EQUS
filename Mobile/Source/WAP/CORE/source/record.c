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
 * record.c
 *
 * Created by Anders Edenbrandt, Fri May 21 08:41:32 1999.
 *
 * Revision history:
 *   001012, AED:  Changed how records are logged.
 *
 *
 * This file implements the Record layer in WTLS.
 *
 */
#include "wtlsdef.h"

#define RECORD_HEADER_SIZE(rec) (1 + \
  (((rec)->rec_type & RECTYPE_SEQNUM) ? 2 : 0) + \
  (((rec)->rec_type & RECTYPE_LENGTH_FIELD) ? 2 : 0))


/*
 * Initialize the record layer at startup.
 * Presently, there is nothing in particular we have to do...??
 */
void
wtls_rec_init (void)
{
}


/*
 * Install the pending state as the current write state.
 */
INT16
wtls_rec_install_pending_write (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  conn->h_state->abortable = 0;
  wtls_pending_state_install (&(conn->write), &(conn->h_state->pending));
  conn->write.last_refresh = 0;

  return RET_OK;
}

/*
 * Install the pending state as the current read state.
 */
INT16
wtls_rec_install_pending_read (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  conn->h_state->abortable = 0;
  wtls_pending_state_install (&(conn->read), &(conn->h_state->pending));
  conn->mask = 0;
  conn->read.last_refresh = 0;

  return RET_OK;
}

/*
 * Compress, compute MAC and encrypt a record.
 * On input, the message part to encrypt is held in rec->fragment.
 * On output, rec->fragment is replaced by the encrypted version,
 * and the old fragment is deallocated.
 */
INT16
wtls_rec_encrypt_record (void *connptr, void *recptr)
{
  wtls_connection_t     *conn = (wtls_connection_t *)connptr;
  wtls_record_t         *rec = (wtls_record_t *)recptr;
  wtls_connection_state *cstate = &(conn->write);
  BYTE                  md[MAX_HASH_MAC_SIZE];
  BYTE                  *buf;
  UINT16                buflen;

  /* Compress */
  if (cstate->cobj.compression_alg != COMPRESS_NULL) {
    /* Currently we have no compression algorithms. */
  }

  /* Compute MAC. */
  if (cstate->cobj.mac_size > 0) {
    if (wtls_crypto_MAC (&(cstate->cobj), cstate->mac_secret,
                         rec->fragment, rec->length,
                         rec->seqnum, rec->rec_type, md) < 0) {
      wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                    0, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
  }

  /* Encrypt: */
  if (cstate->cobj.type == CIPHER_TYPE_BLOCK) {
    BYTE   iv1[MAX_IV_SIZE];
    UINT16 i, len;
    UINT8  paddinglen;
    BYTE   padding[8];

    /* Fix the per-record IV (described in section 11.2) */
    for (i = 0; i < cstate->cobj.iv_size; i += 2) {
      iv1[i] = cstate->iv[i] ^ (rec->seqnum >> 8);
      iv1[i + 1] = cstate->iv[i + 1] ^ (rec->seqnum & 0xff);
    }

    /* Add padding, to make the length a multiple of the block length. */
    /* The padding algorithm is described in section 9.2.3.3. */
    len = rec->length + cstate->cobj.mac_size + 1;
    paddinglen = cstate->cobj.block_size - (len % cstate->cobj.block_size);
    if (paddinglen == cstate->cobj.block_size) {
      paddinglen = 0;
    }
    for (i = 0; i <= paddinglen; i++) {
      padding[i] = paddinglen;
    }

    /* Now, the encryption. The result is placed in a new buffer,
     * that will replace the current fragment. */
    buflen = rec->length + cstate->cobj.mac_size + 1 + paddinglen;
    buf = NEWARRAY (BYTE, buflen);
    if (buf == NULL) {
      wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                    0, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    B_COPYSTRINGN (buf, rec->fragment, rec->length);
    B_COPYSTRINGN (buf + rec->length, md, cstate->cobj.mac_size);
    B_COPYSTRINGN (buf + rec->length + cstate->cobj.mac_size,
                   padding, paddinglen + 1);

    if (wtls_crypto_encrypt (&(cstate->cobj),
                             cstate->encryption_key, iv1,
                             buf, buflen,
                             buf) < 0) {
      DEALLOC (&buf);
      wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                    0, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    DEALLOC (&(rec->fragment));
    rec->fragment = buf;
    rec->length = buflen;
  }
  else {
    /* Currently we have no stream ciphers except the NULL alg. */
    if (cstate->cobj.mac_size > 0) {
      buflen = rec->length + cstate->cobj.mac_size;
      buf = NEWARRAY (BYTE, buflen);
      if (buf == NULL) {
        wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                      0, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
        return -1;
      }
      B_COPYSTRINGN (buf, rec->fragment, rec->length);
      B_COPYSTRINGN (buf + rec->length, md, cstate->cobj.mac_size);
      DEALLOC (&(rec->fragment));
      rec->fragment = buf;
      rec->length = buflen;
    }
  }

  return RET_OK;
}

/*
 * With each connection we have a list of records where we store
 * handshake messages to be sent at a later time.
 * The records in this list are transmitted by calling the routine
 * CreateBufferSDU. They can be resent, by calling CreateBufferSDU
 * several times. In addition, an outgoing data record may be sent
 * with the records in the table prepended to the data record,
 * and all of the records sent in one SDU. To do this, one calls
 * the routine CreateDataAndBufferSDU.
 */


void
wtls_rec_append_record (void *connptr, void *recptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec = (wtls_record_t *)recptr;

  wtls_rec_list_append (rec, &(conn->out_records));
}

/*
 * Create a handshake message record, to be appended to the buffer.
 */
SDL_Integer
wtls_rec_create_handshake_record (void *connptr,
                                  void *buf, SDL_Natural buflen,
                                  SDL_Boolean is_last,
                                  void **pprec)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec;

  rec = NEWARRAY (wtls_record_t, 1);
  if (rec == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  rec->rec_type = CONTENT_TYPE_HANDSHAKE;
  rec->seqnum = conn->write.seqnum++;
  rec->fragment = buf;
  rec->length = (UINT16)buflen;

  if (is_last == SDL_False) {
    rec->rec_type |= RECTYPE_LENGTH_FIELD;
  }
  if (conn->write.use_cipher_spec) {
    rec->rec_type |= RECTYPE_USE_CS;
  }
  /* Sequence numbers MUST be used in handshake messages.
   * (WTLS spec. 9.2.3.1) */
  rec->rec_type |= RECTYPE_SEQNUM;

  *pprec = rec;

#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------CREATING OUTBOUND RECORD--------\n");
  wtls_log_record (rec, LOG_OUTBOUND);
#endif

  if (rec->rec_type & RECTYPE_USE_CS) {
    UINT16 s;

    /* Time for key refresh? */
    s = (~0 << conn->write.key_refresh_rate) & (rec->seqnum);
    if (s != conn->write.last_refresh) {
      conn->write.last_refresh = s;

      return RET_KEY_REFRESH;
    }

    return RET_ENCRYPT_RECORD;
  }

  return RET_OK;
}

/*
 * Create a CCS record, for immediate transmission.
 */
INT16
wtls_rec_create_CCS_record (void *connptr, void **pprec)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec;

  rec = NEWARRAY (wtls_record_t, 1);
  if (rec == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
  }

  /* We use a length field, since this record is always part of a
   * larger SDU. */
  rec->rec_type = CONTENT_TYPE_CCS | RECTYPE_LENGTH_FIELD;
  rec->seqnum = conn->write.seqnum++;
  rec->length = 1;
  rec->fragment = NEWARRAY (BYTE, 1);
  rec->fragment[0] = 1;

  if (conn->write.use_cipher_spec) {
    rec->rec_type |= RECTYPE_USE_CS;
  }
  /* Sequence numbers MUST be used in handshake messages,
   * (WTLS spec. 9.2.3.1), but is CCS a 'handshake message' in
   * this context? Let's hope so... */
  rec->rec_type |= RECTYPE_SEQNUM;

  *pprec = rec;

#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------CREATING OUTBOUND RECORD--------\n");
  wtls_log_record (rec, LOG_OUTBOUND);
#endif

  if (rec->rec_type & RECTYPE_USE_CS) {
    UINT16 s;

    /* Time for key refresh? */
    s = (~0 << conn->write.key_refresh_rate) & (rec->seqnum);
    if (s != conn->write.last_refresh) {
      conn->write.last_refresh = s;

      return RET_KEY_REFRESH;
    }

    return RET_ENCRYPT_RECORD;
  }

  return RET_OK;
}

/*
 * Create an Alert record, for immediate transmission.
 */
INT16
wtls_rec_create_alert_record (void *connptr,
                              SDL_Integer alert_level, SDL_Integer alert_desc,
                              void **pprec)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec;
  wtls_alert_t      alert;
  wtls_cvt_t        cvt_obj;

  rec = NEWARRAY (wtls_record_t, 1);
  if (rec == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  0, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  /* We will not use a length field, since this record will be
   * sent in a SDU by itself. */
  rec->rec_type = CONTENT_TYPE_ALERT;
  rec->seqnum = conn->write.seqnum++;
  rec->length = 6;
  rec->fragment = NEWARRAY (BYTE, 6);

  if (conn->write.seqnum_mode == SEQNUMMODE_EXPLICIT) {
    rec->rec_type |= RECTYPE_SEQNUM;
  }

  alert.level = (BYTE)alert_level;
  alert.description = (BYTE)alert_desc;
  alert.checksum = conn->read_cksum;
  if (conn->write.use_cipher_spec &&
      (alert.description != ALERT_DESC_NO_CONNECTION) &&
      (alert.description != ALERT_DESC_BAD_RECORD_MAC) &&
      (alert.description != ALERT_DESC_DECRYPTION_FAILED) &&
      (alert.description != ALERT_DESC_RECORD_OVERFLOW) &&
      (alert.description != ALERT_DESC_DECOMPRESSION_FAILURE)) {
    rec->rec_type |= RECTYPE_USE_CS;
  }

  wtls_cvt_init (&cvt_obj, CVT_ENCODE, rec->fragment, rec->length);
  if (wtls_cvt_alert (&cvt_obj, &alert)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING,
                  0, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
  }

  *pprec = rec;

#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------CREATING OUTBOUND RECORD--------\n");
  wtls_log_record (rec, LOG_OUTBOUND);
#endif

  if (rec->rec_type & RECTYPE_USE_CS) {
    UINT16 s;

    /* Time for key refresh? */
    s = (~0 << conn->write.key_refresh_rate) & (rec->seqnum);
    if (s != conn->write.last_refresh) {
      conn->write.last_refresh = s;

      return RET_KEY_REFRESH;
    }

    return RET_ENCRYPT_RECORD;
  }

  return RET_OK;
}

/*
 * Create an application data record, for immediate transmission.
 */
SDL_Integer
wtls_rec_prepare_data_record (void *connptr, void *recptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec = (wtls_record_t *)recptr;;

  /* We will not use a length field, since this record will be
   * sent in a SDU by itself. */
  rec->seqnum = conn->write.seqnum++;

  if (conn->write.use_cipher_spec) {
    rec->rec_type |= RECTYPE_USE_CS;
  }
  if (conn->write.seqnum_mode == SEQNUMMODE_EXPLICIT) {
    rec->rec_type |= RECTYPE_SEQNUM;
  }

#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------CREATING OUTBOUND RECORD--------\n");
  wtls_log_record (rec, LOG_OUTBOUND);
#endif

  if (rec->rec_type & RECTYPE_USE_CS) {
    UINT16 s;

    /* Time for key refresh? */
    s = (~0 << conn->write.key_refresh_rate) & (rec->seqnum);
    if (s != conn->write.last_refresh) {
      conn->write.last_refresh = s;

      return RET_KEY_REFRESH;
    }

    return RET_ENCRYPT_RECORD;
  }

  return RET_OK;
}

void
wtls_rec_clear_length_field (void *recptr)
{
  wtls_record_t *rec = (wtls_record_t *)recptr;

  rec->rec_type &= ~RECTYPE_LENGTH_FIELD;
}

/*
 * Encode a record in a SDU.
 */
INT16
wtls_rec_create_SDU (void *connptr, void *recptr, TDUnitdataReqType *data_req)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec = (wtls_record_t *)recptr;
  wtls_cvt_t        cvt_obj;
  BYTE              *buf;
  UINT16            buflen;

#ifdef LOG_WTLS
  wtls_log_msg (0, "\n-------SENDING SDU--------\n");
  wtls_log_msg (0, "SDU[ =>\n");
  wtls_log_record_brief (rec);
  wtls_log_msg (0, "]\n");
#endif
  wtls_cvt_init (&cvt_obj, CVT_ENCODE_SIZE, NULL, 0);
  if (wtls_cvt_record (&cvt_obj, rec)) {
    wtls_rec_delete_record (rec);
    return -1;
  }
  buflen = cvt_obj.pos;
  buf = NEWARRAY (BYTE, buflen);
  if (buf == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  0, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
  }

  wtls_cvt_init (&cvt_obj, CVT_ENCODE, buf, buflen);
  if (wtls_cvt_record (&cvt_obj, rec)) {
    wtls_rec_delete_record (rec);
    DEALLOC (&buf);
    return -1;
  }

  /* Allocate a PDU buffer. */
  data_req->UserData = pdubuf_newFromData (buflen, buf);
  if (data_req->UserData == NULL) {
    wtls_rec_delete_record (rec);
    DEALLOC (&buf);
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  0, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  pdubuf_setLength (data_req->UserData, buflen);

  /* Copy addresses from the connection state. */
  yAssF_AddressType (data_req->SourceAddress, conn->client_addr, XASS);
  yAssF_AddressType (data_req->DestinationAddress, conn->server_addr, XASS);

  conn->write_cksum [conn->num_cksums++ % 4] =
    wtls_alert_compute_checksum (buf, buflen);

  wtls_rec_delete_record (rec);

  return RET_OK;
}

/*
 * Create a SDU holding the stored outbound handshake records,
 * plus an application data record, for immediate sending.
 */
SDL_Integer
wtls_rec_create_data_and_buffer_SDU (void *connptr,
                                     void *recptr,
                                     TDUnitdataReqType *data_req)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec = (wtls_record_t *)recptr;
  wtls_record_t     *prec;
  wtls_cvt_t        cvt_obj;
  UINT16            totlen = 0;
  UINT16            lastlen;
  BYTE              *lastpos;

  /* Compute length of all saved outbound records. */
  totlen = rec->length + RECORD_HEADER_SIZE (rec);
  for (prec = conn->out_records; prec != NULL; prec = prec->next) {
    totlen += prec->length + RECORD_HEADER_SIZE (prec);
  }

  /* Allocate new PDU buffer. */
  data_req->UserData = pdubuf_new (totlen);
  if (data_req->UserData == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  0, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  pdubuf_setLength (data_req->UserData, totlen);

  /* Copy the outgoing data to the PDU buffer. */
#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------SENDING SDU--------\n");
  wtls_log_msg (0, "SDU[ =>\n");
#endif
  wtls_cvt_init (&cvt_obj, CVT_ENCODE,
                 pdubuf_getStart (data_req->UserData), totlen);
  for (prec = conn->out_records; prec != NULL; prec = prec->next) {
    lastpos = cvt_obj.data + cvt_obj.pos;
    if (wtls_cvt_record (&cvt_obj, prec)) {
      return -1;
    }
    lastlen = (cvt_obj.data + cvt_obj.pos) - lastpos;
#ifdef LOG_WTLS
    wtls_log_record_brief (prec);
#endif
  }
  if (wtls_cvt_record (&cvt_obj, rec)) {
    return -1;
  }
#ifdef LOG_WTLS
  wtls_log_record_brief (rec);
  wtls_log_msg (0, "]\n");
#endif
  wtls_rec_delete_record (rec);

  /* Copy addresses from the connection state. */
  yAssF_AddressType (data_req->SourceAddress, conn->client_addr, XASS);
  yAssF_AddressType (data_req->DestinationAddress, conn->server_addr, XASS);

  conn->write_cksum [conn->num_cksums++ % 4] =
    wtls_alert_compute_checksum (lastpos, lastlen);

  return RET_OK;
}

/*
 * Create a SDU holding the stored outbound handshake records,
 * for immediate sending.
 */
INT16
wtls_rec_create_buffer_SDU (void *connptr, TDUnitdataReqType *data_req)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec;
  wtls_cvt_t        cvt_obj;
  UINT16            totlen;
  UINT16            lastlen;
  BYTE              *lastpos;

  /* Should we allow this, and just return? Then, what should be sent? */
  if (conn->out_records == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_MISSING_RECORD_BUFFER,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  /* Compute length of all saved outbound records. */
  totlen = 0;
  for (rec = conn->out_records; rec != NULL; rec = rec->next) {
    totlen += rec->length + RECORD_HEADER_SIZE (rec);
  }

  /* Allocate a PDU buffer. */
  data_req->UserData = pdubuf_new (totlen);
  if (data_req->UserData == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  pdubuf_setLength (data_req->UserData, totlen);

  /* Copy the outgoing data to the PDU buffer. */
#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------SENDING SDU--------\n");
  wtls_log_msg (0, "SDU[ =>\n");
#endif

  wtls_cvt_init (&cvt_obj, CVT_ENCODE,
                 pdubuf_getStart (data_req->UserData), totlen);
  for (rec = conn->out_records; rec != NULL; rec = rec->next) {
    lastpos = cvt_obj.data + cvt_obj.pos;
    if (wtls_cvt_record (&cvt_obj, rec)) {
      return -1;
    }
    lastlen = (cvt_obj.data + cvt_obj.pos) - lastpos;
#ifdef LOG_WTLS
    wtls_log_record_brief (rec);
#endif
  }
#ifdef LOG_WTLS
  wtls_log_msg (0, "]\n");
#endif

  /* Copy addresses from the connection state. */
  yAssF_AddressType (data_req->SourceAddress, conn->client_addr, XASS);
  yAssF_AddressType (data_req->DestinationAddress, conn->server_addr, XASS);

  /* Compute Alert checksum. */
  conn->write_cksum [conn->num_cksums++ % 4] =
    wtls_alert_compute_checksum (lastpos, lastlen);

  return RET_OK;
}

/*
 * Remove the stored outbound handshake records.
 */
void
wtls_rec_clear_buffer (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  wtls_rec_list_delete (&(conn->out_records));
}

void
wtls_rec_list_delete (wtls_record_t **rlist)
{
  wtls_record_t *rec;

  while (*rlist != NULL) {
    rec = *rlist;
    *rlist = rec->next;
    DEALLOC (&rec->fragment);
    DEALLOC (&rec);
  }
}


/**********************************************************
 *
 * ROUTINES FOR INCOMING RECORDS
 *
 **********************************************************/


/*
 * The WTLS protocol states that several records may be sent in
 * one SDU. This packaging of records ought to be transparent to
 * the layers above the record layer. However, since Alert messages
 * need to include a checksum computed on the last record sent
 * by the other end, we have to identify and process each record
 * in the SDU we receive before we start passing the records to
 * the layers above. Hence the routine SplitIncomingSDU below.
 *
 * During the initial processing of the records in a SDU we perform
 * a number of checks, to decide whether to accept the record.
 * Only records that pass those checks will be passed to higher
 * layers, and also will affect the sequence numbers and Alert
 * checksums. However, all the records will be extracted and
 * stored in a list of incoming, undelivered records.
 *
 * In the next pass, when the records are passed to the layers above,
 * further checks may yield new errors. For example, decryption
 * is carried out at this stage and may give erroneous results.
 */

/*
 * Handling of sequence numbers.
 * The current value of the read state's seqNum value is one
 * greater than the largest incoming sequence number we have seen.
 * Hence, initially, the value is 0.
 *
 * The current value of the write state's seqNum value, is the
 * sequence number we should use in the next Record we send.
 * Hence, here too, the value is initally 0.
 *
 * We use a sliding window of size 32, and keep track of which
 * sequence numbers we have seen with the help of a bit mask
 * of 32 bits. The rightmost bit in the bit mask represents
 * the largest sequence number we have seen. Each bit in the mask
 * that is set, means that the sequence number has been seen.
 * For example, if the read state's seqNum value is 118, then
 * the bits in the mask represent the sequence numbers 86..117.
 */

#define WINDOW_SIZE       32

/*
 * Check if a given sequence number is acceptable, i.e.,
 * is either larger than the largest seen so far,
 * or is smaller but within the window limits and is not marked
 * as having been seen. Does NOT update the window or mask.
 */
static INT16
wtls_rec_check_seqnum (UINT16 seqnum, wtls_connection_t *conn)
{
  UINT16 diff;

  if (seqnum >= conn->read.seqnum) { /* Larger than largest so far is OK */
    return RET_OK;
  }
  diff = conn->read.seqnum - seqnum;

  if (diff > WINDOW_SIZE) {         /* Too old, beyond the window's */
    return -1;                      /* left edge. */
  }

  if (conn->mask & ((UINT32)1 << (diff - 1))) { /* Inside window, but */
    return -1;                              /* marked as having been seen. */
  }

  return RET_OK;  /* Passed all tests. */
}

/*
 * Update the read sequence number.
 * Assumes that seqNum has previously been checked and found valid.
 */
static void
wtls_rec_update_seqnum (UINT16 seqnum, wtls_connection_t *conn)
{
  UINT16 diff;

  if (seqnum >= conn->read.seqnum) {
    /* A new larger sequence number, we have to update the window. */
    diff = seqnum - conn->read.seqnum;
    if (diff < WINDOW_SIZE) { /* Move the window */
      conn->mask <<= (diff + 1);
      conn->mask |= 1;
    }
    else {
      conn->mask = 1;
    }
    conn->read.seqnum = seqnum + 1;
  }
  else {
    /* Set one of the bits in the window. */
    diff = conn->read.seqnum - seqnum;
    conn->mask |= ((UINT32)1 << (diff - 1));
  }
}

/*
 * Decrypt, check padding and MAC, and decompress a record.
 */
INT16
wtls_rec_decrypt_record (wtls_connection_state *cstate, wtls_record_t *rec)
{
  INT16  i;
  UINT16 len;
  UINT8  paddinglen;
  BYTE   md[MAX_HASH_MAC_SIZE];

  /* Decrypt: */
  if (cstate->cobj.type == CIPHER_TYPE_BLOCK) {
    BYTE iv1[MAX_IV_SIZE];

    /* Fix the per-record IV */
    for (i = 0; i < cstate->cobj.iv_size; i += 2) {
      iv1[i] = cstate->iv[i] ^ (rec->seqnum >> 8);
      iv1[i + 1] = cstate->iv[i + 1] ^ (rec->seqnum & 0xff);
    }
    len = rec->length;
    if (wtls_crypto_decrypt (&(cstate->cobj), cstate->encryption_key, iv1,
                             rec->fragment, rec->length,
                             rec->fragment) < 0) {
      return -1;
    }

    /* Check padding */
    paddinglen = rec->fragment[len - 1];
    if (len < paddinglen + 1) {
      wtls_err_set (ERR_GENERAL, ERR_BAD_PADDING,
                    1, ALERT_LEVEL_WARNING, ALERT_DESC_DECRYPTION_FAILED);
      return -1;
    }
    rec->length = len - (paddinglen + 1);
    for (i = 0; i < paddinglen; i++) {
      if (rec->fragment[len - i - 2] != paddinglen) {
        wtls_err_set (ERR_GENERAL, ERR_BAD_PADDING,
                      1, ALERT_LEVEL_WARNING, ALERT_DESC_DECRYPTION_FAILED);
        return -1;
      }
    }
  }
  else if (cstate->cobj.bulk_cipher_alg != CIPHER_NULL) {
    /* Currently we have no stream ciphers except the NULL alg. */
  }

  /* Check MAC: */
  if (cstate->cobj.mac_size > 0) {
    if (rec->length < cstate->cobj.mac_size) {
      wtls_err_set (ERR_GENERAL, ERR_BAD_MAC,
                    1, ALERT_LEVEL_WARNING, ALERT_DESC_BAD_RECORD_MAC);
      return -1;
    }
    rec->length -= cstate->cobj.mac_size;
    if (wtls_crypto_MAC (&(cstate->cobj), cstate->mac_secret,
                         rec->fragment, rec->length,
                         rec->seqnum, rec->rec_type, md) < 0) {
      return -1;
    }

    /* Compare with MAC in decrypted record. */
    for (i = 0; i < cstate->cobj.mac_size; i++) {
      if (rec->fragment[rec->length + i] != md[i]) {
        wtls_err_set (ERR_GENERAL, ERR_BAD_MAC,
                      1, ALERT_LEVEL_WARNING, ALERT_DESC_BAD_RECORD_MAC);
        return -1;
      }
    }
  }

  /* Decompress: */
  if (cstate->cobj.compression_alg != COMPRESS_NULL) {
    /* Currently we have no compression algorithms. */
  }

  return RET_OK;
}

/*
 * Get the next incoming record from this connection's list of
 * incoming records, and delete it from that list.
 */
INT16
wtls_rec_next_msg (void *connptr, void **pprec, SDL_Boolean same_SDU)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec;
  UINT16            msg_type;

  rec = wtls_main_peek_incoming_message (conn->cm_proc);
  if ((rec == NULL) ||
      (rec->is_first && (same_SDU == SDL_True))) {
    return RET_QUEUE_EMPTY;
  }
  rec = wtls_main_pop_incoming_message (conn->cm_proc);
  msg_type = rec->rec_type & RECTYPE_CONTENT_TYPE;

  /* STEP 1: does the record have a sequence number? */
  if (!(rec->rec_type & RECTYPE_SEQNUM)) {
    /* Is it allowed to leave out the sequence number? */
    if ((msg_type == CONTENT_TYPE_CCS) ||
        (msg_type == CONTENT_TYPE_HANDSHAKE) ||
        (conn->read.seqnum_mode == SEQNUMMODE_EXPLICIT)) {
      wtls_rec_delete_record (rec);
      return RET_NEXT_MESSAGE;
    }
  }

  /* STEP 2: does the use cipher spec bit agree with our current state? */
  if ((rec->rec_type & RECTYPE_USE_CS) && !conn->read.use_cipher_spec) {
    wtls_rec_delete_record (rec);
    return RET_NEXT_MESSAGE;
  }
  if (!(rec->rec_type & RECTYPE_USE_CS) && conn->read.use_cipher_spec) {
    /* Certain alerts are allowed to be transmitted in the clear. */
    if (msg_type == CONTENT_TYPE_ALERT) {
      UINT8 alertdesc = rec->fragment[1];
      if ((alertdesc == ALERT_DESC_NO_CONNECTION) ||
          (alertdesc == ALERT_DESC_BAD_RECORD_MAC) ||
          (alertdesc == ALERT_DESC_DECRYPTION_FAILED) ||
          (alertdesc == ALERT_DESC_RECORD_OVERFLOW) ||
          (alertdesc == ALERT_DESC_DECOMPRESSION_FAILURE)) {
        goto rec_ok;
      }
    }
    wtls_rec_delete_record (rec);
    return RET_NEXT_MESSAGE;
  }
  rec_ok:
  *pprec = rec;

  /* As a preparation to decryption, check if we need to
   * refresh the keys. */
  if (rec->rec_type & RECTYPE_USE_CS) {
    UINT16 s;

    /* Time for key refresh? */
    s = (~0 << conn->read.key_refresh_rate) & (rec->seqnum);
    if (s != conn->read.last_refresh) {
      conn->read.last_refresh = s;

      return RET_KEY_REFRESH;
    }
  }

  return RET_OK;
}

INT16
wtls_rec_continue_next_msg (void *connptr, void *recptr,
                            SDL_Natural *pmsg_type,
                            void **pbuf, SDL_Natural *pbuflen,
                            SDL_Natural *alert_level,
                            SDL_Natural *alert_desc)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_record_t     *rec = (wtls_record_t *)recptr;
  wtls_alert_t      alert;
  UINT16            msg_type = rec->rec_type & RECTYPE_CONTENT_TYPE;

  *pbuf = NULL;
  *pbuflen = 0;

  /* STEP 3: If this record is encrypted, try to decrypt it.
   * If decryption fails, send a warning Alert and get next message. */
  if (rec->rec_type & RECTYPE_USE_CS) {
    if (wtls_rec_decrypt_record (&(conn->read), rec) < 0) {
      SDL_Integer errtype, errcode, sendalert;

      wtls_rec_delete_record (rec);
      wtls_err_get (&errtype, &errcode, &sendalert,
                    (SDL_Integer *)alert_level,
                    (SDL_Integer *)alert_desc);
      if (*alert_level == ALERT_LEVEL_WARNING) {
        return RET_DECRYPTION_FAILED;
      }
      return -1;
    }
  }

  /* STEP 4: If it is an alert, verify the checksum.
   * We accept checksums of any of the last 4 messages we sent. */ 
  if (msg_type == CONTENT_TYPE_ALERT) {
    INT16      i, limit;
    wtls_cvt_t cvt_obj;

    wtls_cvt_init (&cvt_obj, CVT_DECODE, rec->fragment, rec->length);
    if (wtls_cvt_alert (&cvt_obj, &alert)) {
      /* Error in message format, ignore record silently. */
      wtls_rec_delete_record (rec);
      return RET_NEXT_MESSAGE;
    }
    limit = MIN (4, conn->num_cksums);
    for (i = 0; i < limit; i++) {
      if (alert.checksum == conn->write_cksum[i]) {
        /* We found a match. */
        break;
      }
    }
#ifndef TEST_WTLS
    if (i == limit) {
      /* No match, ignore record silently. */
      wtls_rec_delete_record (rec);
      return RET_NEXT_MESSAGE;
    }
#endif
    /* Fatal Alert sent in cleartext should be treated as critical. */
    if ((alert.level == ALERT_LEVEL_FATAL) &&
        (!(rec->rec_type & RECTYPE_USE_CS))) {
      alert.level = ALERT_LEVEL_CRITICAL;
    }
    *alert_level = (SDL_Natural)alert.level;
    *alert_desc = (SDL_Natural)alert.description;
  }

  /* STEP 5: If it is an alert of type "no connection", go to step 6.
   * Otherwise, check the sequence number. */
  if ((msg_type != CONTENT_TYPE_ALERT) ||
      (alert.description != ALERT_DESC_NO_CONNECTION)) {
    if (wtls_rec_check_seqnum (rec->seqnum, conn) < 0) {
      /* Invalid sequence number, ignore silently. */
      wtls_rec_delete_record (rec);
      return RET_NEXT_MESSAGE;
    }
  }
  /* Handshake messages must increment sequence numbers by 1. */
  /* How can we check this?  */
  /* (Consider, e.g., if this is the FIRST handshake message?) */

  /* STEP 6: Increment the sequence number. */
  wtls_rec_update_seqnum (rec->seqnum, conn);

#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------PROCESSING RECEIVED RECORD--------\n");
  wtls_log_record (rec, LOG_INCOMING);
#endif

  /* STEP 7: Retrieve buffer and delete record. */
  if ((msg_type == CONTENT_TYPE_HANDSHAKE) &&
      (rec->fragment != NULL) &&
      (rec->fragment[0] == HANDSHK_HELLO_REQUEST)) {
    msg_type = CONTENT_TYPE_HELLO_REQUEST;
  }
  *pbuflen = rec->length;
  *pbuf = rec->fragment;
  if ((msg_type == CONTENT_TYPE_CCS) ||
      (msg_type == CONTENT_TYPE_ALERT) ||
      (msg_type == CONTENT_TYPE_HELLO_REQUEST)) {
    DEALLOC (&(rec->fragment));
    *pbuf = NULL;
    *pbuflen = 0;
  }
  rec->fragment = NULL;

  *pmsg_type = msg_type;
  wtls_rec_delete_record (rec);

  if (msg_type == CONTENT_TYPE_ALERT) {
    if (alert.description == ALERT_DESC_TIME_REQUIRED) {
      return RET_TIME_REQUIRED;
    }
    else {
      return RET_ALERT;
    }
  }

  return RET_OK;
}

/*
 * Routines to handle the lists of records.
 */

/*
 * Add a record at the end of a list of records.
 */
void
wtls_rec_list_append (wtls_record_t *rec, wtls_record_t **rec_list)
{
  rec->next = NULL;
  if (*rec_list == NULL) {
    *rec_list = rec;
  }
  else {
    wtls_record_t *p = *rec_list;
    while (p->next != NULL) {
      p = p->next;
    }
    p->next = rec;
  }
}

/*
 * Remove, and return, the first record in a list of records.
 */
wtls_record_t *
wtls_rec_list_pop (wtls_record_t **rec_list)
{
  wtls_record_t *rec = *rec_list;

  if (rec != NULL) {
    *rec_list = rec->next;
    rec->next = NULL;
  }

  return rec;
}

void
wtls_rec_delete_record (wtls_record_t *rec)
{
  if (rec) {
    DEALLOC (&(rec->fragment));
    OSConnectorFree (rec);
  }
}
