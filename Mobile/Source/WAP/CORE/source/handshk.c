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
 * handshk.c
 *
 * Created by Anders Edenbrandt, Tue Jun 29 16:51:01 1999.
 *
 *
 * Revision history:
 *
 */
#include "wtlsdef.h"

/************************************************************
 * General functions
 ************************************************************/

/*
 * Create and initialize a new handshake structure.
 */
wtls_handshake_state *
wtls_handshake_state_new (void)
{
  wtls_handshake_state *hs = NEWARRAY (wtls_handshake_state, 1);

  if (hs == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return NULL;
  }

  wtls_pending_state_init (&(hs->pending));

  hs->key_exch.key_exchange_suite = 0;
  hs->need_client_key_exchange = 0;
  hs->key_id = NULL;
  hs->key_idlen = 0;
  hs->abortable = 1;
  hs->num_handshake_msgs = 0;
  hs->msgs = 0;

  return hs;
}

/*
 * Free a handshake structure, and recursively all data
 * allocated to this structure.
 */
INT16
wtls_handshake_state_free (wtls_handshake_state *hs)
{
  wtls_cvt_t cvt_obj;
  INT16      i;

  if (hs == NULL) {
    return RET_OK;
  }
                  
  if (hs->key_id != NULL) {
    DEALLOC (&(hs->key_id));
  }

  wtls_cvt_init (&cvt_obj, CVT_FREE, NULL, 0);
  if (hs->msgs & BIT_CLIENT_HELLO) {
    wtls_cvt_client_hello (&cvt_obj, &(hs->client_hello));
  }
  if (hs->msgs & BIT_CLIENT_CERTIFICATE) {
    wtls_cvt_client_certificate (&cvt_obj, &(hs->client_certificate));
  }
  if (hs->msgs & BIT_CLIENT_KEY_EXCHANGE) {
    wtls_cvt_client_key_exchange (&cvt_obj,
                                  hs->key_exch.key_exchange_suite,
                                  &(hs->client_key_exchange));
  }
  if (hs->msgs & BIT_CLIENT_CERTIFICATE_VERIFY) {
    wtls_cvt_certificate_verify (&cvt_obj, &(hs->certificate_verify));
  }
  if (hs->msgs & BIT_CLIENT_FINISHED) {
    wtls_cvt_finished (&cvt_obj, &(hs->client_finished));
  }

  for (i = 0; i < hs->num_handshake_msgs; i++) {
    DEALLOC (&(hs->hm[i].buf));
  }
  DEALLOC (&hs);

  return RET_OK;
}
