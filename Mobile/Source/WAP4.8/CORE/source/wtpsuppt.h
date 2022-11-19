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
 * wtpsuppt.h
 *
 * Created by Anders Edenbrandt, Wed Mar 15 15:44:14 2000.
 *
 * Revision history:
 *   000512, AED: Added comments (!)
 *   000517, AED: Routines for handling the port number table.
 *                Removed ttbl_findByPort (no longer needed).
 *   001027, AED: Removed ttbl_insertByTime and ttbl_checkExpired.
 *                Added new routines for creating and parsing PDUs.
 *
 *
 */
#ifndef _wtpsuppt_h
#define _wtpsuppt_h

#include "wiptrgt.h"
#if defined XSCT_CMICRO
#include "ml_typw.h"
#else
#include "scttypes.h"
#endif
#include "ctypes.h"
#include "glbldecl.h"
#include "buffmgt.h"
#include "wspcmwtp.h"
#include "wdp_ul.h"
#include "wtppckg.h"
#include "pdubuf.h"

#define WTP_INITIATOR                      1
#define WTP_RESPONDER                      2

#define WTP_PROVIDER_ABORT              0x00
#define WTP_USER_ABORT                  0x01

#define WTP_PDU_TYPE_INVOKE             0x01
#define WTP_PDU_TYPE_RESULT             0x02
#define WTP_PDU_TYPE_ACK                0x03
#define WTP_PDU_TYPE_ABORT              0x04
#define WTP_PDU_TYPE_SEGMENTED_INVOKE   0x05
#define WTP_PDU_TYPE_SEGMENTED_RESULT   0x06
#define WTP_PDU_TYPE_NACK               0x07

#define WTP_PDU_INVOKE_HEADERLEN           4
#define WTP_PDU_RESULT_HEADERLEN           3
#define WTP_PDU_ACK_HEADERLEN              3
#define WTP_PDU_ABORT_HEADERLEN            4
#define WTP_PDU_SEGMENTED_INVOKE_HEADERLEN 4
#define WTP_PDU_SEGMENTED_RESULT_HEADERLEN 4
#define WTP_PDU_NACK_HEADERLEN             4
#define WTP_PDU_TPI_HEADERLEN              1

#define WTP_TPI_ERROR                   0x00
#define WTP_TPI_INFO                    0x01
#define WTP_TPI_OPTION                  0x02
#define WTP_TPI_PSN                     0x03

#define WTP_TPI_OPTION_MAX_RECV_UNIT       0x01
#define WTP_TPI_OPTION_TOTAL_MESSAGE_SIZE  0x02
#define WTP_TPI_OPTION_DELAY_TRANS_TIMER   0x03
#define WTP_TPI_OPTION_MAX_GROUP_SIZE      0x04
#define WTP_TPI_OPTION_CURRENT_TID         0x05
#define WTP_TPI_OPTION_NO_CACHED_TID       0x06

/* Return codes */
#define WTP_RET_OK                  0
#define WTP_RET_WAIT_RESULT         1
#define WTP_RET_ERROR               2
#define WTP_RET_MESSAGE_COMPLETED   3
#define WTP_RET_GROUP_COMPLETED     4
#define WTP_RET_SEND_ACK            5

/* Error codes */
#define WTP_ERR_UNKNOWN             0
#define WTP_ERR_PROTOERR            1
#define WTP_ERR_INVALIDTID          2
#define WTP_ERR_NOTIMPLEMENTEDCL2   3
#define WTP_ERR_NOTIMPLEMENTEDSAR   4
#define WTP_ERR_NOTIMPLEMENTEDUACK  5
#define WTP_ERR_WTPVERSIONONE       6
#define WTP_ERR_CAPTEMPEXCEEDED     7
#define WTP_ERR_NORESPONSE          8
#define WTP_ERR_MESSAGETOOLARGE     9


/************************************************************
 * Routines that handle transaction tables, i.e.,
 * tables holding information about current transactions.
 ************************************************************/

/*
 * Initialize and return a pointer to an empty table.
 */
void *
ttbl_init (void);

/*
 * Remove all elements in the table.
 */
void
ttbl_clear (void **ttbl);

/*
 * Add an element to the table.
 */
void
ttbl_insert (void **ttbl, TransactionType *t);

/*
 * Delete the element with specified handle from the table.
 */
void
ttbl_delete (void **ttbl, SDL_Integer handle);

/*
 * Find the element with specified handle, store the information
 * in the location pointed to by "t", and return the SDL process ID
 * of the process handling this transaction.
 * Returns SDL_NULL if the element is not in the table.
 */
SDL_PId
ttbl_find (void *ttbl, SDL_Integer handle, TransactionType *t);

/*
 * Find the element with specified address and transaction ID,
 * and store the information in the location pointed to by "t".
 * The parameter "dir" indicates whether this is a transaction for
 * which the client is INITIATOR or RESPONDER.
 * Returns the SDL process ID of the process handling this transaction,
 * or SDL_NULL if the element is not in the table.
 */
SDL_PId
ttbl_findByTID (void *ttbl, AddressType src, SDL_Natural tid,
                SDL_Natural dir, TransactionType *t);

/*
 * Remove the first transaction in the table, and return
 * the SDL process ID of the process handling that transaction.
 * Returns SDL_NULL if the table is empty.
 */
SDL_PId
ttbl_pop (void **ttbl);


/************************************************************
 * Routines for the TID caches.
 * A TID cache stores the last Transaction ID used with
 * a certain address.
 ************************************************************/

/*
 * Initialize and return a pointer to an empty TID cache.
 */
void *
tidcache_init (void);

/*
 * Remove all elements from the TID cache.
 */
void
tidcache_clear (void **tbl);

/*
 * Find and return the TID associated with the address "addr".
 * Returns EMPTY_TID (a number outside the range of valid TIDs)
 * if the address is not in the table.
 */
SDL_Natural
tidcache_get (void *tbl, DeviceAddress addr);

/*
 * Store "val" as the TID assocated with the address "addr".
 */
void
tidcache_set (void **tbl, DeviceAddress addr, SDL_Natural val);


/************************************************************
 * Routines for the port table.
 * This is a table that stores information about how many
 * transactions are currently using a certain port number.
 ************************************************************/

/*
 * Initialize the port number table.
 */
void
port_table_init (void);

/*
 * Delete all elements in the port number table.
 */
void
port_table_clear (void);

/*
 * A ClosePort signal has arrived for the port "portnum".
 * Check if we can send the signal ClosePort right away.
 * Returns True if the signal can be sent now, False otherwise.
 */
SDL_Boolean
port_table_closeport (SDL_Natural portnum, SDL_Natural path);

/*
 * A new transaction has been created using the port "portnum".
 * Update the port number table accordingly.
 */
void
port_table_newtrans (SDL_Natural portnum);

/*
 * A transaction using the port "portnum" has terminated.
 * Update the port number table, and check if we should
 * send a ClosePort signal. Returns True if ClosePort should
 * be sent right away, False otherwise.
 */
SDL_Boolean
port_table_done (SDL_Natural portnum, SDL_Natural *path);


/************************************************************
 * Routines for generating TIDs and handles:
 ************************************************************/

/*
 * Initialize the TID counter to a random number.
 */
void
tid_init (void);

/*
 * Return a new TID. Values are increased by one each time,
 * and wraps around to 0 after 32767.
 */
SDL_Natural
tid_new (void);

/*
 * Initialize the handle counter to a number larger than
 * the largest valid TID.
 */
void
handle_init (void);

/*
 * Return a new handle. Values are increased by one each time.
 */
SDL_Natural
handle_new (void);


/************************************************************
 * Create and unpack common PDUs.
 ************************************************************/

/*
 * Retrieve some basic information from a PDU header:
 * the TID, the PDU type and the direction.
 */
void
wtp_get_pdu_info (pdubuf *pdu, SDL_Natural *tidp,
                  SDL_Natural *pdu_type, SDL_Natural *direction);

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
                       SDL_Natural *transaction_class);

/*
 * Create an Abort PDU.
 */
pdubuf *
wtp_create_abort_pdu (SDL_Natural tid,
                      SDL_Natural abort_code,
                      SDL_Natural abort_type);

/*
 * Create an ACK PDU with an ExitInfo TPI.
 */
pdubuf *
wtp_create_ack_with_info (SDL_Natural tid, pdubuf *exit_info);

/*
 * Create an ACK PDU.
 */
pdubuf *
wtp_create_ack_pdu (SDL_Natural tid,
                    SDL_Natural last_recv_seqnum,
                    SDL_Boolean tidve,
                    SDL_Boolean resend);

#endif
