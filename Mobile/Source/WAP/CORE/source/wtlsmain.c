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
 * wtlsmain.c
 *
 * Created by Anders Edenbrandt, Wed Jun 02 07:57:08 1999.
 *
 * Revision history:
 *   001012, AED:  Changed how records are logged.
 *
 */

#include "wtlsdef.h"

typedef struct ln_st {
  struct ln_st  *next;
  SDL_PId       cm_proc;
  SDL_PId       rec_proc;
  AddressType   src_addr;
  AddressType   dst_addr;
  void          *connptr;
  wtls_record_t *msgs_out;
  wtls_record_t *msgs_in;
  UINT8         close_on_done;
  SDL_Natural   portnum;
  SDL_Integer   path;
} list_node;

static list_node *connection_list;

/************************************************************
 * Forward declaration of local functions
 ************************************************************/

/*
 * Return a pointer to the list node with given PId.
 * Returns NULL in case the given PId is not in the list.
 */
static list_node *
wtls_main_find_node (SDL_PId pid);


/************************************************************
 * External routines called from SDL.
 ************************************************************/

/*
 * Initialize the WTLS_Main SDL process.
 */
void
wtls_main_init (void)
{
  connection_list = NULL;
}

/*
 * Terminate the WTLS_Main SDL process.
 */
void
wtls_main_terminate (void)
{
  while (connection_list != NULL) {
    (void)wtls_main_pop_connection ();
  }
}

/*
 * Add a new element to the list of known connection quadruples.
 */
SDL_Integer
wtls_main_new_connection (SDL_PId cm_proc, SDL_PId rec_proc,
                          AddressType *src_addr, AddressType *dst_addr)
{
  list_node *n = NEWARRAY (list_node, 1);

  if (n == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  0, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  n->cm_proc = cm_proc;
  n->rec_proc = rec_proc;
  yDef_AddressType (&(n->src_addr));
  yDef_AddressType (&(n->dst_addr));
  yAssF_AddressType (n->src_addr, *src_addr, XASS);
  yAssF_AddressType (n->dst_addr, *dst_addr, XASS);
  n->msgs_in = NULL;
  n->msgs_out = NULL;
  n->close_on_done = 0;
  n->next = connection_list;
  connection_list = n;

  return RET_OK;
}

/*
 * Delete an element from our list of known connection
 * quadruples, and also free its message queue.
 */
SDL_Integer
wtls_main_delete_connection (SDL_PId cm_proc)
{
  list_node *n, *p;

  p = NULL;
  for (n = connection_list; n; p = n, n = n->next) {
    if (n->cm_proc == cm_proc) {
      if (p != NULL)
        p->next = n->next;
      else
        connection_list = n->next;
      wtls_rec_list_delete (&(n->msgs_in));
      wtls_rec_list_delete (&(n->msgs_out));
      DEALLOC (&n);
      break;
    }
  }

  return RET_OK;
}

/*
 * Remove the first element in our list of known
 * connection quadruples, and free its message queues.
 * Returns the SDL_PId of the removed connection,
 * or xNULLPID if the list is empty.
 */
SDL_PId
wtls_main_pop_connection (void)
{
  list_node *n = connection_list;
  SDL_PId  p;

  if (n == NULL) {
    return xNULLPID;
  }
  p = n->cm_proc;
  connection_list = n->next;
  wtls_rec_list_delete (&(n->msgs_in));
  wtls_rec_list_delete (&(n->msgs_out));
  DEALLOC (&n);

  return p;
}

/*
 * Return the SDL_PId of the connection with the given
 * address quadruple.
 * Returns xNULLPID if the connection is not in our list
 * of known connections.
 */
SDL_PId
wtls_main_find_connection (AddressType *src_addr, AddressType *dst_addr,
                           SDL_PId *rec_proc)
{
  list_node *n;

  for (n = connection_list; n != NULL; n = n->next) {
    if ((n->src_addr.PortNumber == src_addr->PortNumber) &&
        (n->dst_addr.PortNumber == dst_addr->PortNumber) &&
        yEqF_DeviceAddress (n->src_addr.Address, src_addr->Address) &&
        yEqF_DeviceAddress (n->dst_addr.Address, dst_addr->Address)) {
      *rec_proc = n->rec_proc;
      return n->cm_proc;
    }
  }

  return xNULLPID;
}

/*
 * Return the SDL_PId of the connection with the given
 * source port number.
 * Returns xNULLPID if the connection is not in our list
 * of known connections.
 */
SDL_PId
wtls_main_find_connection_by_port (SDL_Natural portnum)
{
  list_node *n;

  for (n = connection_list; n != NULL; n = n->next) {
    if ((SDL_Natural)n->src_addr.PortNumber == portnum) {
      return n->cm_proc;
    }
  }

  return xNULLPID;
}

void
wtls_main_set_close_on_done (SDL_PId cm_proc,
                             SDL_Natural portnum,
                             SDL_Integer path)
{
  list_node *n = wtls_main_find_node (cm_proc);

  if (n == NULL) {
    return;
  }

  n->close_on_done = 1;
  n->portnum = portnum;
  n->path = path;
}

SDL_Boolean
wtls_main_get_close_on_done (SDL_PId cm_proc,
                             SDL_Natural *portnum,
                             SDL_Integer *path)
{
  list_node *n = wtls_main_find_node (cm_proc);

  if (n == NULL) {
    return SDL_False;
  }

  if (n->close_on_done) {
    *portnum = n->portnum;
    *path = n->path;

    return SDL_True;
  }
  else {
    return SDL_False;
  }
}

/*
 * Add a new outbound message to the message queue of the
 * indicated Connection Manager process.
 */
SDL_Integer
wtls_main_append_message (SDL_PId cm_proc, pdubuf *msg)
{
  list_node     *n = wtls_main_find_node (cm_proc);
  wtls_record_t *rec;

  if (n == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_MISSING_CONNECTION_NODE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  rec = NEWARRAY (wtls_record_t, 1);
  if (rec == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  rec->rec_type = CONTENT_TYPE_DATA;
  rec->length = pdubuf_getLength (msg);
  if (msg->currpos == 0) {
    rec->fragment = pdubuf_getStart (msg);
    DEALLOC (&msg);
  }
  else {
    rec->fragment = NEWARRAY (BYTE, rec->length);
    if (rec->fragment == NULL) {
      wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    B_COPYSTRINGN (rec->fragment, pdubuf_getStart (msg), rec->length);
    pdubuf_release (msg);
  }
  wtls_rec_list_append (rec, &(n->msgs_out));

  return RET_OK;
}


/*
 * Check if we have an active Connection Manager process
 * with the given PId. Returns 0 if we the process exists
 * and we have not sent it a TerminateReq signal,
 * and -1 otherwise.
 */
SDL_Integer
wtls_main_check_pid (SDL_PId pid)
{
  list_node *n;

  for (n = connection_list; n != NULL; n = n->next) {
    if ((n->cm_proc == pid) || (n->rec_proc == pid))
      break;
  }

  if (n == NULL) {
    return -1;
  }
  else {
    return 0;
  }
}

/*
 * Check if a received SDU is a clear text Alert message.
 */
INT16
wtls_main_is_cleartext_alert (pdubuf *data)
{
  wtls_cvt_t    cvt_obj;
  wtls_record_t rec;

  wtls_cvt_init (&cvt_obj, CVT_DECODE_STATIC,
                 pdubuf_getStart (data), pdubuf_getLength (data));
  if (wtls_cvt_record (&cvt_obj, &rec)) {
    return -1;
  }
  if (((rec.rec_type & RECTYPE_CONTENT_TYPE) == CONTENT_TYPE_ALERT) &&
      ((rec.rec_type & RECTYPE_USE_CS) == 0)) {
    return 0;
  }

  return -1;
}

/*
 * Create a SDU holding a No-Connection Alert, for immediate sending.
 * "dataInd" is the message we just received, for which we have no
 * connection.
 */
SDL_Integer
wtls_main_create_no_connection_alert (TDUnitdataIndType *dataInd,
                                      TDUnitdataReqType *dataReq)
{
  BYTE          *p = pdubuf_getStart (dataInd->UserData);
  UINT16        len = pdubuf_getLength (dataInd->UserData);
  wtls_alert_t  alert;
  wtls_cvt_t    cvt_obj;
  wtls_record_t rec;
  BYTE          *buf;
  UINT16        buflen;

  alert.level = ALERT_LEVEL_CRITICAL;
  alert.description = ALERT_DESC_NO_CONNECTION;
  alert.checksum = wtls_alert_compute_checksum (p, len);
  buflen = 6;
  buf = NEWARRAY (BYTE, buflen);
  if (buf == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }    

  wtls_cvt_init (&cvt_obj, CVT_ENCODE, buf, buflen);
  if (wtls_cvt_alert (&cvt_obj, &alert)) {
    return -1;
  }

  /* We will not use a length field, since this record will be
   * sent in a SDU by itself. */
  rec.rec_type = CONTENT_TYPE_ALERT | RECTYPE_SEQNUM;
  rec.seqnum = MAX_SEQNUM;
  rec.length = buflen;
  rec.fragment = buf;
#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------SENDING SDU--------\n");
  wtls_log_msg (0, "SDU[ =>\n");
  wtls_log_record_brief (&rec);
  wtls_log_msg (0, "]\n");
#endif

  wtls_cvt_init (&cvt_obj, CVT_ENCODE_SIZE, NULL, 0);
  if (wtls_cvt_record (&cvt_obj, &rec)) {
    return -1;
  }
  buflen = cvt_obj.pos;
  buf = NEWARRAY (BYTE, buflen);
  if (buf == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }    

  wtls_cvt_init (&cvt_obj, CVT_ENCODE, buf, buflen);
  if (wtls_cvt_record (&cvt_obj, &rec)) {
    DEALLOC (&buf);
    return -1;
  }
  DEALLOC (&(rec.fragment));

  /* Allocate a PDU buffer. */
  dataReq->UserData = pdubuf_newFromData (buflen, buf);
  pdubuf_setLength (dataReq->UserData, buflen);

  /* Copy addresses from the connection state. */
  yAssF_AddressType (dataReq->SourceAddress, dataInd->DestinationAddress,
                     XASS);
  yAssF_AddressType (dataReq->DestinationAddress, dataInd->SourceAddress,
                     XASS);

  return RET_OK;
}

/*
 * Split an incoming SDU into records, and store them in the connection's
 * list of incoming records.
 */
SDL_Integer
wtls_main_split_SDU (SDL_PId cm_proc, pdubuf *SDU)
{
  list_node     *n = wtls_main_find_node (cm_proc);
  wtls_record_t *rec = NULL;
  wtls_cvt_t    cvt_obj;
  BYTE          *p;
  UINT16        start;
  UINT8         content_type;
  UINT8         is_first = 1;

  if ((n == NULL) || (SDU == NULL)) {
    return -1;
  }

#ifdef LOG_WTLS
  wtls_log_msg (0, "\n--------RECEIVED SDU--------\n");
  wtls_log_msg (0, "SDU[ <=\n");
#endif
  wtls_cvt_init (&cvt_obj, CVT_DECODE,
                 pdubuf_getStart (SDU), pdubuf_getLength (SDU));

  while (cvt_obj.pos < cvt_obj.length) {
    p = cvt_obj.data + cvt_obj.pos;
    start = cvt_obj.pos;

    rec = NEWARRAY (wtls_record_t, 1);
    if (rec == NULL) {
      wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    if (wtls_cvt_record (&cvt_obj, rec)) {
      wtls_rec_delete_record (rec);
      return -1;
    }
    content_type = rec->rec_type & RECTYPE_CONTENT_TYPE;

    /* Check that we have a content type we recognize. */
    if ((content_type != CONTENT_TYPE_CCS) &&
        (content_type != CONTENT_TYPE_ALERT) &&
        (content_type != CONTENT_TYPE_HANDSHAKE) &&
        (content_type != CONTENT_TYPE_DATA)) {
      /* Unknown content type; ignore this record.
       * We cannot trust any information in this record,
       * so we have to assume that the record occupies the entire SDU. */
      wtls_rec_delete_record (rec);
      return -1;
    }
#ifdef LOG_WTLS
    wtls_log_record_brief (rec);
#endif
    rec->is_first = is_first;
    is_first = 0;
    wtls_rec_list_append (rec, &(n->msgs_in));

    /* Compute the Alert checksum. We tentatively accept this record,
     * and hence will use its checksum when we send an Alert. */
    ((wtls_connection_t *)(n->connptr))->read_cksum =
      wtls_alert_compute_checksum (p, (UINT16)(cvt_obj.pos - start));
  }

#ifdef LOG_WTLS
  wtls_log_msg (0, "]\n");
#endif

  return RET_OK;
}

/************************************************************
 * External routines called from other C functions.
 ************************************************************/

/*
 * Store a pointer to the connection record.
 */
void
wtls_main_set_connection_pointer (SDL_PId pid, void *connptr)
{
  list_node *n = wtls_main_find_node (pid);

  if (n == NULL) {
    return;
  }
  n->connptr = connptr;
}

/*
 * Get a pointer to the connection record for the specified SDL process.
 */
void *
wtls_main_get_connection_pointer (SDL_PId pid)
{
  list_node *n = wtls_main_find_node (pid);

  if (n == NULL) {
    return NULL;
  }

  return n->connptr;
}

/*
 * Remove, and return, the first message in the message queue
 * for the indicated Connection Manager process.
 * Returns NULL, in case the queue is empty.
 */
wtls_record_t *
wtls_main_pop_incoming_message (SDL_PId pid)
{
  list_node *n = wtls_main_find_node (pid);

  if (n == NULL) {
    return NULL;
  }
  return wtls_rec_list_pop (&(n->msgs_in));
}

wtls_record_t *
wtls_main_peek_incoming_message (SDL_PId pid)
{
  list_node *n = wtls_main_find_node (pid);

  if (n == NULL) {
    return NULL;
  }
  return n->msgs_in;
}

wtls_record_t *
wtls_main_pop_outbound_message (SDL_PId pid)
{
  list_node *n = wtls_main_find_node (pid);

  if (n == NULL) {
    return NULL;
  }
  return wtls_rec_list_pop (&(n->msgs_out));
}

/*
 * Check if a received PDU is a Hello Request message in clear text,
 * from a source that we can accept.
 * Returns 0 if we should act on this request, -1 otherwise.
 */
SDL_Integer
wtls_main_check_hello_request (pdubuf *pdu)
{
  wtls_cvt_t    cvt_obj;
  wtls_record_t rec;
  SDL_Integer   retval = -1;

  wtls_cvt_init (&cvt_obj, CVT_DECODE_STATIC,
                 pdubuf_getStart (pdu), pdubuf_getLength (pdu));
  if (wtls_cvt_record (&cvt_obj, &rec)) {
    return -1;
  }
  if (((rec.rec_type & RECTYPE_CONTENT_TYPE) == CONTENT_TYPE_HANDSHAKE) &&
      ((rec.rec_type & RECTYPE_USE_CS) == 0) &&
      (rec.length == 3) &&
      (rec.fragment[0] == 0) &&
      (rec.fragment[1] == 0) &&
      (rec.fragment[2] == 0)) {
    retval = 0;
  }
  
  return retval;
}

/************************************************************
 * Local functions
 ************************************************************/

/*
 * Return a pointer to the list node with given PId.
 * Returns NULL in case the given PId is not in the list.
 */
static list_node *
wtls_main_find_node (SDL_PId cm_proc)
{
  list_node *n;

  for (n = connection_list; n != NULL; n = n->next) {
    if (n->cm_proc == cm_proc)
      return n;
  }
  
  return NULL;
}
