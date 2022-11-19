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
 * Connectn.c
 *
 * Created by Anders Edenbrandt, Tue Jun 15 14:20:59 1999.
 *
 * Revision history:
 *
 */
#include "wtlsdef.h"

/*
 * Free the connection structure pointed to by "connptr",
 * and recursively any other dynamically allocated data
 * that belongs to this connection.
 */
void
wtls_connection_free (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  wtls_handshake_state_free (conn->h_state);

  wtls_rec_list_delete (&(conn->out_records));

  DEALLOC (&conn);
}

/*
 * Initialize a connection state.
 */
INT16
wtls_connection_state_init (wtls_connection_state *cstate)
{
  memset (cstate, 0, sizeof (wtls_connection_state));

  return RET_OK;
}

/*
 * Initialize a pending state.
 */
INT16
wtls_pending_state_init (wtls_pending_state *pstate)
{
  memset (pstate, 0, sizeof (wtls_pending_state));

  return RET_OK;
}

/*
 * Copy a pending state to a connection state.
 */
INT16
wtls_pending_state_install (wtls_connection_state *cstate,
                            wtls_pending_state *pstate)
{
  INT16 i;

  cstate->session_id = pstate->session_id;

  wtls_crypto_init (&(cstate->cobj), pstate->bulk_cipher_alg,
                    pstate->mac_alg, pstate->compression_alg);

  for (i = 0; i < 16; i++) {
    cstate->client_hello_random[i] = pstate->client_hello_random[i];
    cstate->server_hello_random[i] = pstate->server_hello_random[i];
  }
  cstate->key_refresh_rate = pstate->key_refresh_rate;
  cstate->seqnum_mode = pstate->seqnum_mode;
  cstate->master_secret_id = pstate->master_secret_id;

  cstate->use_cipher_spec = TRUE;
  cstate->seqnum = 0;

  return RET_OK;
}
