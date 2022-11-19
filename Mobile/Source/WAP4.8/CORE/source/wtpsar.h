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
 * wtpsar.h
 *
 * Created by Anders Edenbrandt, Tue Oct 17 15:55:28 2000.
 *
 * Revision history:
 *
 */
#ifndef _wtpsar_h
#define _wtpsar_h

#include "wtpsuppt.h"

/*
 * Initialize a transaction structure.
 * Returns a pointer that is used in subsequent calls to
 * refer to this transaction.
 */
void *
wtp_transaction_init (SDL_Natural tid,
                      SDL_Boolean tid_new,
                      TRInvokeReqType *req);

/*
 * Delete a transaction structure.
 */
void
wtp_delete_transaction (void *trp);

/*
 * Create an Invoke PDU for a class 0 transaction.
 */
pdubuf *
wtp_create_invoke_pdu0 (void *trp);

/************************************************************
 * First come the routines used for segmented invoke.
 ************************************************************/

/*
 * Routine called when a segmented invoke request arrives.
 */
void
wtp_segmented_invoke (void *trp, TRInvokeReqType *inv_req);

/*
 * Return a copy of the last invoke segment that was sent.
 */
pdubuf *
wtp_get_last_segment (void *trp);

/*
 * Create next group.
 */
void
wtp_create_group (void *trp);

/*
 * Delete this transaction's current group of invoke segments.
 */
void
wtp_delete_group (void *trp);

/*
 * Return the next segment to be sent in the current group.
 * Returns NULL in case there are no more segments waiting
 * to be transmitted.
 */
pdubuf *
wtp_get_next_segment (void *trp,
                      SDL_Boolean *group_done,
                      SDL_Boolean *msg_done);

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
                    SDL_Duration *delay_time);

/*
 * Unpack a NACK PDU and all TPIs. Deletes the PDU
 * afterwards. Creates a list of missing segments
 * and stores with this transaction.
 */
void
wtp_unpack_nack_pdu (void *trp, pdubuf *pdu);

/*
 * Return a copy of the first segment on the list of
 * missing segments. Deletes this segment from the list.
 * Returns NULL if the list is empty.
 */
pdubuf *
wtp_next_missing_segment (void *trp);


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
wtp_get_last_recv_seqnum (void *trp);

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
                        SDL_Natural *abort_code);

#endif

