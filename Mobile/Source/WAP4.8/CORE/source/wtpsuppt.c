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
 * wtpsuppt.c
 *
 * Created by Anders Edenbrandt, Fri Mar 17 11:10:10 2000.
 *
 * Revision history:
 *   000512, AED: Added comments (!)
 *   000512, AED: Removed the checking of the address for incoming
 *                messages when the client is Initiator.
 *   000517, AED: Routines for handling the port number table.
 *                Removed ttbl_findByPort (no longer needed).
 *   001027, AED: Removed ttbl_insertByTime and ttbl_checkExpired.
 *                Added new routines for creating and parsing PDUs.
 *
 */
#include "wtpsuppt.h"
#include "aapiclnt.h"


/************************************************************
 * Routines that handle transaction tables, i.e.,
 * tables holding information about current transactions.
 ************************************************************/

/*
 * A transaction table is realized as a singly linked list,
 * where each node in the list is of the following type.
 */
typedef struct st_ttbl_node {
  struct st_ttbl_node *next;
  TransactionType      tr;
} ttbl_node;

/*
 * Initialize and return a pointer to an empty table.
 */
void *
ttbl_init (void)
{
  return 0;
}

/*
 * Remove all elements in the table.
 */
void
ttbl_clear (void **tbl)
{
  ttbl_node *tn;

  if (tbl == NULL) {
    return;
  }

  tn = (ttbl_node *)*tbl;
  while (tn) {
    ttbl_node *tmp = tn;

    tn = tn->next;
    DEALLOC (&tmp);
  }

  *tbl = 0;
}

/*
 * Add an element to the table.
 */
void
ttbl_insert (void **tbl, TransactionType *tr)
{
  ttbl_node *tmp;

  if (tbl == NULL) {
    return;
  }
  tmp = NEWARRAY (ttbl_node, 1);
  if (tmp == NULL) {
    return;
  }
  tmp->next = (ttbl_node *)*tbl;
  tmp->tr = *tr;

  *tbl = tmp;
}

/*
 * Delete the element with specified handle from the table.
 */
void
ttbl_delete (void **tbl, SDL_Integer handle)
{
  ttbl_node *tn, *qn;

  if (tbl == NULL) {
    return;
  }

  qn = NULL;
  tn = (ttbl_node *)*tbl;

  while (tn) {
    if (tn->tr.handle == handle) {
      if (qn == NULL) {
        *tbl = tn->next;
      }
      else {
        qn->next = tn->next;
      }
      DEALLOC (&tn);
      break;
    }
    qn = tn;
    tn = tn->next;
  }
}

/*
 * Find the element with specified handle, store the information
 * in the location pointed to by "t", and return the SDL process ID
 * of the process handling this transaction.
 * Returns SDL_NULL if the element is not in the table.
 */
SDL_PId
ttbl_find (void *tbl, SDL_Integer handle, TransactionType *t)
{
  ttbl_node *tb = (ttbl_node *)tbl;

  for (; tb; tb = tb->next) {
    if (tb->tr.handle == handle) {
      *t = tb->tr;
      return tb->tr.pid;
    }
  }

  return SDL_NULL;
}

/*
 * Find the element with specified address and transaction ID,
 * and store the information in the location pointed to by "t".
 * The parameter "dir" indicates whether this is a transaction for
 * which the client is INITIATOR or RESPONDER.
 * Returns the SDL process ID of the process handling this transaction,
 * or SDL_NULL if the element is not in the table.
 */
SDL_PId
ttbl_findByTID (void *tbl, AddressType src, SDL_Natural tid,
                SDL_Natural dir, TransactionType *t)
{
  ttbl_node *tb = (ttbl_node *)tbl;

  for (; tb; tb = tb->next) {
    if (dir == WTP_INITIATOR) {
      if ((tb->tr.dir == dir) &&
          (tb->tr.tid == tid)) {
        *t = tb->tr;
        return tb->tr.pid;
      }
    }
    else if ((tb->tr.dir == dir) &&
             (tb->tr.tid == tid) &&
             (tb->tr.serverAddress.PortNumber == src.PortNumber) &&
             (yEqF_DeviceAddress (tb->tr.serverAddress.Address, src.Address))) {
      *t = tb->tr;
      return tb->tr.pid;
    }
  }

  return SDL_NULL;
}

/*
 * Remove the first transaction in the table, and return
 * the SDL process ID of the process handling that transaction.
 * Returns SDL_NULL if the table is empty.
 */
SDL_PId
ttbl_pop (void **tbl)
{
  ttbl_node *tn;
  SDL_PId   tmpPid;

  if ((tbl == NULL) || (*tbl == NULL)) {
    return SDL_NULL;
  }

  tn = (ttbl_node *)*tbl;
  *tbl = tn->next;
  tmpPid = tn->tr.pid;
  DEALLOC (&tn);

  return tmpPid;
}


/************************************************************
 * Routines for the TID caches.
 * A TID cache stores the last Transaction ID used with
 * a certain address.
 ************************************************************/

/*
 * A TID cache is implemented as a singly linked list,
 * with each node in the list having the following type.
 */
typedef struct st_tidcache_node {
  struct st_tidcache_node *next;
  SDL_Natural             tid;
  DeviceAddress           addr;
} tidcache_node;

/*
 * Initialize and return a pointer to an empty TID cache.
 */
void *
tidcache_init (void)
{
  return 0;
}

/*
 * Remove all elements from the TID cache.
 */
void
tidcache_clear (void **tbl)
{
  tidcache_node *tn;

  if (tbl == NULL) {
    return;
  }

  tn = (tidcache_node *)*tbl;
  while (tn) {
    tidcache_node *tmp = tn;

    tn = tn->next;
    DEALLOC (&tmp);
  }

  *tbl = 0;
}

/*
 * Find and return the TID associated with the address "addr".
 * Returns EMPTY_TID (a number outside the range of valid TIDs)
 * if the address is not in the table.
 */
SDL_Natural
tidcache_get (void *tbl, DeviceAddress addr)
{
  tidcache_node *tn = (tidcache_node *)tbl;
  
  for (; tn; tn = tn->next) {
    if (yEqF_DeviceAddress (tn->addr, addr)) {
      return tn->tid;
    }
  }

  return 32768;
}

/*
 * Store "val" as the TID assocated with the address "addr".
 */
void
tidcache_set (void **tbl, DeviceAddress addr, SDL_Natural val)
{
  tidcache_node **tmp = (tidcache_node **)tbl;
  tidcache_node *tn;
  
  if (tmp == NULL) {
    return;
  }
  
  for (tn = *tmp; tn; tn = tn->next) {
    if (yEqF_DeviceAddress (tn->addr, addr)) {
      tn->tid = val;
      return;
    }
  }

  tn = NEWARRAY (tidcache_node, 1);
  tn->next = *tmp;
  tn->addr = addr;
  tn->tid = val;

  *tbl = tn;
}


/************************************************************
 * Routines for the port table.
 * This is a table that stores information about how many
 * transactions are currently using a certain port number.
 ************************************************************/

/*
 * The port table is implemented as a singly linked list
 * with each node in the list having the following type.
 */
typedef struct st_port_table_node {
  struct st_port_table_node *next;
  SDL_Natural                portnum;
  SDL_Natural                path;
  UINT16                     numActive;
  UINT8                      closeOnDone;
} port_table_node;

static port_table_node *port_table = 0;

/*
 * Initialize the port number table.
 */
void
port_table_init (void)
{
  port_table = 0;
}

/*
 * Delete all elements in the port number table.
 */
void
port_table_clear (void)
{
  while (port_table) {
    port_table_node *pn = port_table;

    port_table = port_table->next;
    DEALLOC (&pn);
  }
}

/*
 * A ClosePort signal has arrived for the port "portnum".
 * Check if we can send the signal ClosePort right away.
 * Returns True if the signal can be sent now, False otherwise.
 */
SDL_Boolean
port_table_closeport (SDL_Natural portnum, SDL_Natural path)
{
  port_table_node *pn;

  for (pn = port_table; pn; pn = pn->next) {
    if (pn->portnum == portnum) {
      if (pn->numActive > 0) {
        pn->path = path;
        pn->closeOnDone = 1;
        return SDL_False;
      }
      else {
        pn->closeOnDone = 0;
        return SDL_True;
      }
    }
  }

  return SDL_True;
}

/*
 * A new transaction has been created using the port "portnum".
 * Update the port number table accordingly.
 */
void
port_table_newtrans (SDL_Natural portnum)
{
  port_table_node *pn;

  for (pn = port_table; pn; pn = pn->next) {
    if (pn->portnum == portnum) {
      pn->numActive++;
      pn->closeOnDone = 0;
      return;
    }
  }

  pn = NEWARRAY (port_table_node, 1);
  pn->portnum = portnum;
  pn->numActive = 1;
  pn->closeOnDone = 0;
  pn->next = port_table;
  port_table = pn;
}

/*
 * A transaction using the port "portnum" has terminated.
 * Update the port number table, and check if we should
 * send a ClosePort signal. Returns True if ClosePort should
 * be sent right away, False otherwise.
 */
SDL_Boolean
port_table_done (SDL_Natural portnum, SDL_Natural *path)
{
  port_table_node *pn, *qn;
  SDL_Boolean     retval = SDL_False;

  for (qn = 0, pn = port_table; pn; qn = pn, pn = pn->next) {
    if (pn->portnum == portnum) {
      if (pn->numActive > 0) {
        pn->numActive--;
      }
      if (pn->numActive == 0) {
        if (pn->closeOnDone) {
          *path = pn->path;
          retval = SDL_True;
        }
        /* Delete node */
        if (qn == 0) {
          port_table = pn->next;
        }
        else {
          qn->next = pn->next;
        }
        DEALLOC (&pn);
      }
      break;
    }
  }

  return retval;
}


/************************************************************
 * Routines for generating TIDs and handles:
 ************************************************************/

static unsigned int current_tid = 0;
static int current_handle = 0x10000;

/*
 * Initialize the TID counter to a random number.
 */
void
tid_init (void)
{
  srand (CLNTa_currentTime ());

  current_tid = rand () % 32768;
}

/*
 * Return a new TID. Values are increased by one each time,
 * and wraps around to 0 after 32767.
 */
SDL_Natural
tid_new (void)
{
  unsigned int tmp = current_tid;

  current_tid++;
  if (current_tid >= 32768) {
    current_tid = 0;
  }

  return tmp;
}

/*
 * Initialize the handle counter to a number larger than
 * the largest valid TID.
 */
void
handle_init (void)
{
  current_handle = 0x10000;
}

/*
 * Return a new handle. Values are increased by one each time.
 */
SDL_Natural
handle_new (void)
{
  current_handle++;

  return current_handle;
}


/************************************************************
 * Create and unpack common PDUs.
 ************************************************************/

/*
 * Retrieve some basic information from a PDU header:
 * the TID, the transaction class and the direction.
 */
void
wtp_get_pdu_info (pdubuf *pdu,
                  SDL_Natural *tidp,
                  SDL_Natural *pdu_type,
                  SDL_Natural *direction)
{
  BYTE   *pb;
  UINT16 tid;

  pb = pdubuf_getStart (pdu);
  if (pb == NULL)
    return;
  tid = (pb[1] << 8) | pb[2];
  if (pb[1] & 0x80)
    *direction = WTP_INITIATOR;
  else
    *direction = WTP_RESPONDER;
  *tidp = tid & 0x7fff;
  *pdu_type = (pb[0] >> 3) & 0xf;
}

/*
 * Unpack the header of an Invoke PDU, and delete the header
 * and all TPIs from the PDU.
 * Returns -1 in case of error, and 0 otherwise.
 */
SDL_Integer
wtp_unpack_invoke_pdu (pdubuf *pdu,
                       SDL_Boolean *gtr,
                       SDL_Boolean *ttr,
                       SDL_Natural *version,
                       SDL_Boolean *tidnew,
                       SDL_Natural *userAck,
                       SDL_Natural *transaction_class)
{
  BYTE   *pb = pdubuf_getStart (pdu);
  UINT16 remaining_length = pdubuf_getLength (pdu);
  UINT8  more_tpis, long_tpi;
  UINT16 tpi_length;

  if (remaining_length < 4)
    goto err_return;

  *gtr = (pb[0] & 0x4) ? SDL_True : SDL_False;
  *ttr = (pb[0] & 0x2) ? SDL_True : SDL_False;
  *version = (pb[3] >> 6);
  *tidnew = (pb[3] & 0x20) ? SDL_True : SDL_False;
  *userAck = (pb[3] >> 4) & 0x1;
  *transaction_class = (pb[3] & 0x3);

  more_tpis = (pb[0] >> 7) & 0x1;

  pb += 4;
  remaining_length -= 4;

  while (more_tpis) {
    if (remaining_length < 1)
      goto err_return;
    long_tpi = (pb[0] >> 2) & 0x1;
    if (long_tpi) {
      if (remaining_length < 2)
        goto err_return;
      tpi_length = pb[1];
      if (remaining_length < tpi_length + 2)
        goto err_return;
    }
    else {
      tpi_length = pb[0] & 0x3;
      if (remaining_length < tpi_length + 1)
        goto err_return;
    }
    more_tpis = (pb[0] >> 7) & 0x1;

    if (long_tpi) {
      pb += tpi_length + 2;
      remaining_length -= tpi_length + 2;
    }
    else {
      pb += tpi_length + 1;
      remaining_length -= tpi_length + 1;
    }
  }
  pdubuf_setLength (pdu, remaining_length);
  return 0;

 err_return:
  pdubuf_release (pdu);
  return -1;
}

/*
 * Create an Abort PDU.
 */
pdubuf *
wtp_create_abort_pdu (SDL_Natural tid,
                      SDL_Natural abort_code,
                      SDL_Natural abort_type)
{
  pdubuf            *pdu;
  BYTE              *pb;

  pdu = pdubuf_new (WTP_PDU_ABORT_HEADERLEN);
  if (pdu == NULL)
    return NULL;
  pdubuf_setLength (pdu, WTP_PDU_ABORT_HEADERLEN);
  pb = pdubuf_getStart (pdu);

  pb[0] = (UINT8)((WTP_PDU_TYPE_ABORT << 3) | (abort_type & 0x7));
  pb[1] = (UINT8)(tid >> 8);
  pb[2] = (UINT8)(tid & 0xff);
  pb[3] = (UINT8)abort_code;

  return pdu;
}

/*
 * Create an ACK PDU.
 */
pdubuf *
wtp_create_ack_pdu (SDL_Natural tid,
                    SDL_Natural last_recv_seqnum,
                    SDL_Boolean tidve,
                    SDL_Boolean resend)
{
  pdubuf *pdu;
  BYTE   *pb;
  UINT16  length;

  length = WTP_PDU_ACK_HEADERLEN;
  if (last_recv_seqnum > 0)
    length += 2;

  pdu = pdubuf_new (length);
  if (pdu == NULL)
    return NULL;
  pdubuf_setLength (pdu, length);
  pb = pdubuf_getStart (pdu);

  pb[0] = (WTP_PDU_TYPE_ACK << 3);
  if (tidve == SDL_True)
    pb[0] |= 0x04;
  if (resend == SDL_True)
    pb[0] |= 0x01;

  pb[1] = (UINT8)(tid >> 8);
  pb[2] = (UINT8)(tid & 0xff);

  if (last_recv_seqnum > 0) {
    pb[0] |= 0x80;
    pb[3] = (WTP_TPI_PSN << 3) | 0x01;
    pb[4] = last_recv_seqnum;
  }

  return pdu;
}
