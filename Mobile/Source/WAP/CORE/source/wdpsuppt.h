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
 * WDPsuppt.h
 *
 * Created by Anders Edenbrandt, Wed Aug 16 11:16:18 2000.
 *
 * Revision history:
 *
 */
#ifndef _WDPsuppt_h
#define _WDPsuppt_h

#include "wiptrgt.h"
#if defined XSCT_CMICRO
#include "ml_typw.h"
#else
#include "scttypes.h"
#endif
#include "ctypes.h"
#include "glbldecl.h"

#include "pdubuf.h"

/************************************************************
 * Routines that handle the port table, i.e.,
 * a table holding information about current connections.
 ************************************************************/

/*
 * Initialize the port table.
 */
void
wdp_porttbl_init (void);

/*
 * Remove all elements from the port table.
 */
void
wdp_porttbl_clear (void);

/*
 * Add or update an element in the port table.
 */
void
wdp_porttbl_update (SDL_Integer portnum, SDL_PId pid,
                    SDL_Natural config, SDL_Octet bearer,
                    SDL_Octet view_id, SDL_Octet channel_id);

/*
 * Delete the element with the specified port number
 * from the port table.
 */
void
wdp_porttbl_delete (SDL_Integer portnum);

/*
 * Retrieve values from the port table, stored under
 * the specified port number.
 * Returns SDL_True if the port number was found,
 * and SDL_False otherwise.
 */
SDL_Boolean
wdp_porttbl_lookup (SDL_Integer portnum, SDL_PId *pid,
                    SDL_Natural *config, SDL_Octet *bearer,
                    SDL_Octet *view_id, SDL_Octet *channel_id);


/************************************************************
 * Message building and parsing.
 ************************************************************/

#define WCMP_DESTINATION_UNREACHABLE  51
#define WCMP_MESSAGE_TOO_BIG          60
#define WCMP_PORT_UNREACHABLE          4
#define WCMP_ECHO_REQUEST            178
#define WCMP_ECHO_REPLY              179

#define IE_ID_PORT_NUMBER         5
#define IE_ID_SAR_SHORT           0
#define IE_ID_SAR_LONG            8
#define IE_ID_WCMP                9

/*
 * Construct an SMS segment holding the first "length" bytes
 * in the given PDU buffer.
 * The result is stored in "segment" and "seglen".
 */
void
wdp_segment_build (pdubuf *pb, SDL_Integer length,
                   SDL_Integer dst_port, SDL_Integer src_port,
                   SDL_Natural refnum, SDL_Natural total_num_segments,
                   SDL_Natural segnum,
                   void** segment, SDL_Natural *seglen);

/*
 * Extract values from a User Data Header.
 * Returns SDL_True if the extraction succeeded, and
 * SDL_False otherwise.
 */
SDL_Boolean
wdp_UDH_extract (void *segment, SDL_Natural length,
                 SDL_Boolean *is_segmented,
                 SDL_Natural *header_length,
                 SDL_Integer *dst_port, SDL_Integer *src_port,
                 SDL_Natural *refnum, SDL_Natural *total_num_segments,
                 SDL_Natural *segnum,
                 SDL_Boolean *has_WCMP,
                 void **WCMP_ptr, SDL_Natural *WCMP_length);

/*
 * Construct a WCMP message.
 */
void
wdp_wcmp_build_msg (SDL_Octet type, SDL_Octet code,
                    SDL_Integer dst_port, SDL_Integer src_port,
                    DeviceAddress *addr,
                    void *old_msg, SDL_Natural old_msg_length,
                    void **msg, SDL_Natural *msg_length);


/************************************************************
 * Tables of ongoing reassemblies, and rejected reassemblies.
 ************************************************************/

/*
 * Initialize the segment table.
 */
void
wdp_segtbl_init (void);

/*
 * Delete all entries from the segment table.
 */
void
wdp_segtbl_clear (void);

/*
 * Initialize the table of rejects, i.e., reassemblies
 * that have failed. Segments belonging to such a failed
 * reassembly will be discarded upon arrival.
 */
void
wdp_rejtbl_init (void);

/*
 * Add an element to the table of rejects.
 */
void
wdp_rejtbl_add (SDL_Natural refnum, DeviceAddress server_addr);

/*
 * Check if a <refnum, server_addr> pair is in the table of rejects.
 */
SDL_Boolean
wdp_rejtbl_lookup (SDL_Natural refnum, DeviceAddress server_addr);

/*
 * Check if a reassembly in the segment table has all its
 * segments.
 */
SDL_Boolean
wdp_segtbl_is_complete (SDL_Integer index);

/*
 * Terminate the reassembly at position "index", and insert
 * it into the table of rejects.
 */
void
wdp_segtbl_reject (SDL_Integer index);

/*
 * Delete the reassembly using client port "portnum".
 * Returns the index in the table where it was found,
 * or -1 if it's not in the table.
 */
SDL_Integer
wdp_segtbl_delete_port (SDL_Integer portnum);

/*
 * Delete the reassembly at position "index" in the table.
 */
void
wdp_segtbl_delete_entry (SDL_Integer index);

/*
 * Check if a <refnum, server_addr> pair is in the table
 * of reassemblies. Returns the index in the table if
 * it was found, and -1 otherwise.
 */
SDL_Integer
wdp_segtbl_lookup (SDL_Natural refnum, DeviceAddress addr,
                   SDL_Integer *empty_pos);

/*
 * Assemble all the segments of the reassembly at position "index"
 * in the table, and turn into a PDU buffer. Also deletes the entry
 * from the table.
 */
void
wdp_segtbl_assemble (SDL_Integer index, pdubuf **pb_ptr);

/*
 * Add a segment to the reassembly table.
 */
void
wdp_segtbl_add (SDL_Integer index,
                SDL_Natural refnum, DeviceAddress server_addr,
                SDL_Integer portnum, SDL_Natural total_num_segments,
                SDL_Natural segnum,
                void *segment, SDL_Natural length, SDL_Natural header_length);



/************************************************************
 * Saved addresses.
 ************************************************************/

#define WDP_CLIENT_ADDRESS  0x01
#define WDP_SMSC_ADDRESS    0x02
#define WDP_USSDC_ADDRESS   0x04

/*
 * Initialize the saved addresses.
 */
void
wdp_address_init (void);

/*
 * Save an address. The first parameter is a bit mask
 * that indicates which address(es) should be saved.
 */
void
wdp_address_save (SDL_Integer type, DeviceAddress *addr,
                  AuxiliaryAddress *addr2);

/*
 * Retrieve a saved address. The first parameter is a bit mask
 * that indicates which address(es) should be retrieved.
 */
void
wdp_address_get (SDL_Integer type, DeviceAddress *addr,
                 AuxiliaryAddress *addr2);

#endif
