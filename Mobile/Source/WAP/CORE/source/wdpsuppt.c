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
 * WDPsuppt.c
 *
 * Created by Anders Edenbrandt, Wed Aug 16 12:43:53 2000.
 *
 * Revision history:
 *
 */
#include "wdpsuppt.h"
#include "cmmnrsrc.h"

/************************************************************
 * Routines that handle the port table, i.e.,
 * a table holding information about current connections.
 ************************************************************/
typedef struct st_port_table_node {
  struct st_port_table_node *next;
  SDL_Integer                portnum;
  SDL_PId                    pid;
  SDL_Natural                config;
  SDL_Octet                  bearer;
  SDL_Octet                  view_id;
  SDL_Octet                  channel_id;
} port_table_t;

static port_table_t *port_table = 0;

/*
 * Initialize the port table.
 */
void
wdp_porttbl_init (void)
{
  port_table = 0;
}

/*
 * Remove all elements from the port table.
 */
void
wdp_porttbl_clear (void)
{
  while (port_table) {
    port_table_t *pn = port_table;

    port_table = port_table->next;
    OSConnectorFree (pn);
  }
}

/*
 * Locate an entry in the port table.
 */
static port_table_t *
wdp_porttbl_find (SDL_Integer portnum)
{
  port_table_t *pn;

  for (pn = port_table; pn; pn = pn->next) {
    if (pn->portnum == portnum) {
      break;
    }
  }

  return pn;
}

/*
 * Add or update an element in the port table.
 */
void
wdp_porttbl_update (SDL_Integer portnum, SDL_PId pid,
                    SDL_Natural config, SDL_Octet bearer,
                    SDL_Octet view_id, SDL_Octet channel_id)
{
  port_table_t *pn;

  pn = wdp_porttbl_find (portnum);
  if (!pn) {
    pn = OSConnectorAlloc (sizeof (port_table_t));
    pn->next = port_table;
    port_table = pn;
    pn->portnum = portnum;
  }
  pn->pid = pid;
  pn->config = config;
  pn->bearer = bearer;
  pn->view_id = view_id;
  pn->channel_id = channel_id;
}

/*
 * Delete the element with the specified port number
 * from the port table.
 */
void
wdp_porttbl_delete (SDL_Integer portnum)
{
  port_table_t *pn, *qn;

  for (qn = 0, pn = port_table; pn; qn = pn, pn = pn->next) {
    if (pn->portnum == portnum) {
      break;
    }
  }
  if (!pn) {
    return;
  }
  if (qn) {
    qn->next = pn->next;
  }
  else {
    port_table = pn->next;
  }
  OSConnectorFree (pn);
}

/*
 * Retrieve values from the port table, stored under
 * the specified port number.
 * Returns SDL_True if the port number was found,
 * and SDL_False otherwise.
 */
SDL_Boolean
wdp_porttbl_lookup (SDL_Integer portnum, SDL_PId *pid,
                    SDL_Natural *config, SDL_Octet *bearer,
                    SDL_Octet *view_id, SDL_Octet *channel_id)
{
  port_table_t *pn;

  pn = wdp_porttbl_find (portnum);

  if (!pn) {
    return SDL_False;
  }
  *pid = pn->pid;
  *config = pn->config;
  *bearer = pn->bearer;
  *view_id = pn->view_id;
  *channel_id = pn->channel_id;

  return SDL_True;
}


/************************************************************
 * Message building and parsing.
 ************************************************************/

#define SEGMENTED_HEADER_SIZE      12
#define NONSEGMENTED_HEADER_SIZE    7

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
                   void** segment, SDL_Natural *seglen)
{
  BYTE *ptr;

  if (total_num_segments > 1) {
    *seglen = length + SEGMENTED_HEADER_SIZE;
    *segment = OSConnectorAlloc (*seglen);
    ptr = (BYTE *)*segment;
    ptr[0] = 11;
    ptr[1] = IE_ID_PORT_NUMBER;
    ptr[2] = 4;
    ptr[3] = (UINT8)(dst_port >> 8);
    ptr[4] = (UINT8)(dst_port & 0xff);
    ptr[5] = (UINT8)(src_port >> 8);
    ptr[6] = (UINT8)(src_port & 0xff);
    ptr[7] = IE_ID_SAR_SHORT;
    ptr[8] = 3;
    ptr[9] = (BYTE)refnum;
    ptr[10] = (BYTE)total_num_segments;
    ptr[11] = (BYTE)segnum;

    memcpy (ptr + SEGMENTED_HEADER_SIZE, pdubuf_getStart (pb), length);
  }
  else {
    *seglen = length + NONSEGMENTED_HEADER_SIZE;
    *segment = OSConnectorAlloc (*seglen);
    ptr = (BYTE *)*segment;
    ptr[0] = 6;
    ptr[1] = IE_ID_PORT_NUMBER;
    ptr[2] = 4;
    ptr[3] = (UINT8)(dst_port >> 8);
    ptr[4] = (UINT8)(dst_port & 0xff);
    ptr[5] = (UINT8)(src_port >> 8);
    ptr[6] = (UINT8)(src_port & 0xff);

    memcpy (ptr + NONSEGMENTED_HEADER_SIZE, pdubuf_getStart (pb), length);
  }
}

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
                 void **WCMP_ptr, SDL_Natural *WCMP_length)
{
  BYTE  *p = segment;
  INT16 i;

  if (length <= 0) {
    return SDL_False;
  }
  *header_length = (UINT16)p[0] + 1;
  if (length < *header_length) {
    return SDL_False;
  }
  *is_segmented = SDL_False;
  *has_WCMP = SDL_False;

  for (i = 1; i < *header_length;) {
    switch (p[i]) {
    case IE_ID_PORT_NUMBER:
      if (i + 5 >= *header_length) {
        return SDL_False;
      }
      if (p[i + 1] != 4) {
        return SDL_False;
      }
      *dst_port = (p[i + 2] << 8) | (p[i + 3] & 0xff);
      *src_port = (p[i + 4] << 8) | (p[i + 5] & 0xff);
      i += 6;
      break;

    case IE_ID_SAR_SHORT:
      if (i + 4 >= *header_length) {
        return SDL_False;
      }
      if (p[i + 1] != 3) {
        return SDL_False;
      }
      *is_segmented = SDL_True;
      *refnum = (p[i + 2] & 0xff);
      *total_num_segments = (p[i + 3] & 0xff);
      *segnum = (p[i + 4] & 0xff);
      i += 5;
      break;

    case IE_ID_SAR_LONG:
      if (i + 5 >= *header_length) {
        return SDL_False;
      }
      if (p[i + 1] != 4) {
        return SDL_False;
      }
      *is_segmented = SDL_True;
      *refnum = (p[i + 2] << 8) | (p[i + 3] & 0xff);
      *total_num_segments = (p[i + 4] & 0xff);
      *segnum = (p[i + 5] & 0xff);
      i += 6;
      break;

    case IE_ID_WCMP:
      if (i + 1 >= *header_length) {
        return SDL_False;
      }
      *has_WCMP = SDL_True;
      *WCMP_ptr = &(p[i + 2]);
      *WCMP_length = p[i + 1];
      i += p[i + 1] + 2;
      break;

    default:
      /* Unknown IE Identifier, skip past it. */
      if (i + 1 >= *header_length) {
        return SDL_False;
      }
      i += p[i] + 2;
      break;
    }
  }

  return SDL_True;
}

/*
 * Construct a WCMP message.
 */
void
wdp_wcmp_build_msg (SDL_Octet type, SDL_Octet code,
                    SDL_Integer dst_port, SDL_Integer src_port,
                    DeviceAddress *addr,
                    void *old_msg, SDL_Natural old_msg_length,
                    void **msg, SDL_Natural *msg_length)
{
  BYTE  *p;
  UINT8 IE_length;

  if (type == WCMP_MESSAGE_TOO_BIG) {
    IE_length = (UINT8)(6 + 2 + addr->length + 2);
    *msg_length = 2 + IE_length;
    p = *msg = OSConnectorAlloc (*msg_length);
    p[addr->length + 11] = (MaxPDUsize >> 8);
    p[addr->length + 12] = (MaxPDUsize & 0xff);
  }
  else if (type == WCMP_DESTINATION_UNREACHABLE) {
    IE_length = (UINT8)(6 + 2 + addr->length);
    *msg_length = 2 + IE_length;
    p = *msg = OSConnectorAlloc (*msg_length);
  }
  else if (type == WCMP_ECHO_REPLY) {
    IE_length = (UINT8)(old_msg_length + 1);
    *msg_length = 2 + IE_length;
    p = *msg = OSConnectorAlloc (*msg_length);
    memcpy (p + 3, old_msg, old_msg_length);
    p[0] = (UINT8)*msg_length;
    p[1] = 9;
    p[2] = IE_length;
    p[3] = WCMP_ECHO_REPLY;
    return;
  }
  else {
    *msg = NULL;
    *msg_length = 0;
    return;
  }
  p[0] = (UINT8)*msg_length;
  p[1] = 9;
  p[2] = (UINT8)IE_length;
  p[3] = (UINT8)type;
  p[4] = (UINT8)code;
  p[5] = (UINT8)(dst_port >> 8);
  p[6] = (UINT8)(dst_port & 0xff);
  p[7] = (UINT8)(src_port >> 8);
  p[8] = (UINT8)(src_port & 0xff);
  p[9] = 3; /* Addresstype GSM SMS */
  p[10] = (UINT8)(addr->length);
  memcpy (p + 11, addr->data, addr->length);
}

/************************************************************
 * Tables of ongoing reassemblies, and rejected reassemblies.
 ************************************************************/

#define EMPTY_REFNUM      1000
#define MAX_REASSEMBLIES  10
#define MAX_REJECTIONS    20

typedef struct wdp_seg_node_st {
  struct wdp_seg_node_st *next;
  SDL_Natural segnum;
  BYTE        *segment;
  SDL_Natural length;
  SDL_Natural header_length;
} wdp_seg_node;

typedef struct {
  SDL_Natural   refnum;
  DeviceAddress server_addr;
  SDL_Integer   portnum;
  SDL_Natural   total_num_segments;
  wdp_seg_node  *segments;
} wdp_segtbl_entry;

typedef struct {
  SDL_Natural   refnum;
  DeviceAddress server_addr;
} wdp_rejtbl_entry;

static wdp_segtbl_entry wdp_segtbl[MAX_REASSEMBLIES];
static wdp_rejtbl_entry wdp_rejtbl[MAX_REJECTIONS];
static unsigned int wdp_rejtbl_insert_next = 0;

/*
 * Initialize the segment table.
 */
void
wdp_segtbl_init (void)
{
  int i;

  for (i = 0; i < MAX_REASSEMBLIES; i++) {
    wdp_segtbl[i].refnum = EMPTY_REFNUM;
    wdp_segtbl[i].total_num_segments = 0;
    wdp_segtbl[i].segments = NULL;
  }
}

/*
 * Delete all entries from the segment table.
 */
void
wdp_segtbl_clear (void)
{
  int i;

  for (i = 0; i < MAX_REASSEMBLIES; i++) {
    wdp_segtbl_delete_entry (i);
  }
}

/*
 * Initialize the table of rejects, i.e., reassemblies
 * that have failed. Segments belonging to such a failed
 * reassembly will be discarded upon arrival.
 */
void
wdp_rejtbl_init (void)
{
  int i;

  for (i = 0; i < MAX_REJECTIONS; i++) {
    wdp_rejtbl[i].refnum = EMPTY_REFNUM;
    wdp_rejtbl[i].server_addr.length = 0;
  }

  wdp_rejtbl_insert_next = 0;
}

/*
 * Add an element to the table of rejects.
 */
void
wdp_rejtbl_add (SDL_Natural refnum, DeviceAddress server_addr)
{
  if (wdp_rejtbl_lookup (refnum, server_addr) == SDL_True) {
    return;
  }
  if (wdp_rejtbl_insert_next == MAX_REJECTIONS) {
    wdp_rejtbl_insert_next = 0;
  }
  wdp_rejtbl[wdp_rejtbl_insert_next].refnum = refnum;
  yDef_DeviceAddress (&(wdp_rejtbl[wdp_rejtbl_insert_next].server_addr));
  yAssF_DeviceAddress (wdp_rejtbl[wdp_rejtbl_insert_next].server_addr,
                       server_addr, XASS);

  wdp_rejtbl_insert_next++;
}

/*
 * Check if a <refnum, server_addr> pair is in the table of rejects.
 */
SDL_Boolean
wdp_rejtbl_lookup (SDL_Natural refnum, DeviceAddress server_addr)
{
  int i;

  for (i = 0; i < MAX_REJECTIONS; i++) {
    if ((wdp_rejtbl[i].refnum == refnum) &&
        yEqF_DeviceAddress (wdp_rejtbl[i].server_addr, server_addr)) {
      return SDL_True;
    }
  }

  return SDL_False;
}

/*
 * Check if a reassembly in the segment table has all its
 * segments.
 */
SDL_Boolean
wdp_segtbl_is_complete (SDL_Integer index)
{
  int n = 0;
  wdp_seg_node *p = wdp_segtbl[index].segments;

  if ((wdp_segtbl[index].refnum == EMPTY_REFNUM) ||
      (wdp_segtbl[index].total_num_segments == 0)) {
    return SDL_False;
  }
  while (p) {
    n++;
    if (n != p->segnum) {
      return SDL_False;
    }
    p = p->next;
  }

  return (n == wdp_segtbl[index].total_num_segments) ? SDL_True : SDL_False;
}

/*
 * Terminate the reassembly at position "index", and insert
 * it into the table of rejects.
 */
void
wdp_segtbl_reject (SDL_Integer index)
{
  if (wdp_segtbl[index].refnum == EMPTY_REFNUM) {
    return;
  }
  wdp_rejtbl_add (wdp_segtbl[index].refnum, wdp_segtbl[index].server_addr);
  wdp_segtbl_delete_entry (index);
}

/*
 * Delete the reassembly using client port "portnum".
 * Returns the index in the table where it was found,
 * or -1 if it's not in the table.
 */
SDL_Integer
wdp_segtbl_delete_port (SDL_Integer portnum)
{
  int i;

  for (i = 0; i < MAX_REASSEMBLIES; i++) {
    if ((wdp_segtbl[i].refnum != EMPTY_REFNUM) &&
        (wdp_segtbl[i].portnum == portnum)) {
      wdp_segtbl_delete_entry (i);
      return i;
    }
  }

  return -1;
}

/*
 * Delete the reassembly at position "index" in the table.
 */
void
wdp_segtbl_delete_entry (SDL_Integer index)
{
  wdp_seg_node *p;

  if (wdp_segtbl[index].refnum == EMPTY_REFNUM) {
    return;
  }
  while ((p = wdp_segtbl[index].segments) != 0) {
    wdp_segtbl[index].segments = p->next;
    OSConnectorFree (p->segment);
    OSConnectorFree (p);
  }
  wdp_segtbl[index].refnum = EMPTY_REFNUM;
  wdp_segtbl[index].portnum = 0;
  wdp_segtbl[index].total_num_segments = 0;
  wdp_segtbl[index].segments = NULL;
}

/*
 * Check if a <refnum, server_addr> pair is in the table
 * of reassemblies. Returns the index in the table if
 * it was found, and -1 otherwise.
 */
SDL_Integer
wdp_segtbl_lookup (SDL_Natural refnum, DeviceAddress addr,
                   SDL_Integer *empty_pos)
{
  SDL_Integer i, e = -1;

  for (i = 0; i < MAX_REASSEMBLIES; i++) {
    if ((wdp_segtbl[i].refnum == refnum) &&
        yEqF_DeviceAddress (wdp_segtbl[i].server_addr, addr)) {
      return i;
    }
    else if ((e < 0) && (wdp_segtbl[i].refnum == EMPTY_REFNUM)) {
      e = i;
    }
  }

  *empty_pos = e;
  return -1;
}

/*
 * Assemble all the segments of the reassembly at position "index"
 * in the table, and turn into a PDU buffer. Also deletes the entry
 * from the table.
 */
void
wdp_segtbl_assemble (SDL_Integer index, pdubuf **pb_ptr)
{
  wdp_seg_node *p;
  UINT16       length = 0;
  pdubuf       *pb;
  BYTE         *q;

  if (wdp_segtbl[index].refnum == EMPTY_REFNUM) {
    *pb_ptr = NULL;
    return;
  }

  for (p = wdp_segtbl[index].segments; p; p = p->next) {
    length += p->length - p->header_length;;
  }

  pb = pdubuf_new (length);
  pdubuf_setLength (pb, length);

  q = pdubuf_getStart (pb);
  for (p = wdp_segtbl[index].segments; p; p = p->next) {
    memcpy (q, p->segment + p->header_length, p->length - p->header_length);
    q += p->length - p->header_length;
  }

  wdp_segtbl_delete_entry (index);

  *pb_ptr = pb;
}

/*
 * Add a segment to the reassembly table.
 */
void
wdp_segtbl_add (SDL_Integer index,
                SDL_Natural refnum, DeviceAddress server_addr,
                SDL_Integer portnum, SDL_Natural total_num_segments,
                SDL_Natural segnum,
                void *segment, SDL_Natural length, SDL_Natural header_length)
{
  wdp_seg_node *p, *q, *r;

  if (wdp_segtbl[index].refnum == EMPTY_REFNUM) {
    wdp_segtbl[index].refnum = refnum;
    wdp_segtbl[index].server_addr = server_addr;
    wdp_segtbl[index].portnum = (UINT16)portnum;
    wdp_segtbl[index].total_num_segments = (UINT16)total_num_segments;
    wdp_segtbl[index].segments = NULL;
  }
  for (q = 0, p = wdp_segtbl[index].segments; p; q = p, p = p->next) {
    if (p->segnum == segnum) {
      /* This segment is a duplicate; deallocate and return. */
      OSConnectorFree (segment);
      return;
    }
    if (p->segnum > segnum) {
      break;
    }
  }
  /* We have found the correct position */
  r = OSConnectorAlloc (sizeof (wdp_seg_node));
  r->segment = segment;
  r->length = (UINT16)length;
  r->segnum = (UINT16)segnum;
  r->header_length = (UINT16)header_length;
  if (q) {
    q->next = r;
  }
  else {
    wdp_segtbl[index].segments = r;
  }
  r->next = p;
}


/************************************************************
 * Saved addresses.
 ************************************************************/

static DeviceAddress    saved_client_addr;
static AuxiliaryAddress saved_smsc_addr;
static AuxiliaryAddress saved_ussdc_addr;

/*
 * Initialize the saved addresses.
 */
void
wdp_address_init (void)
{
  saved_client_addr.length = 0;
  saved_smsc_addr.length = 0;
  saved_ussdc_addr.length = 0;
}

/*
 * Save an address. The first parameter is a bit mask
 * that indicates which address(es) should be saved.
 */
void
wdp_address_save (SDL_Integer type, DeviceAddress *addr,
                  AuxiliaryAddress *addr2)
{
  if (type & WDP_CLIENT_ADDRESS) {
    yAssF_DeviceAddress (saved_client_addr, *addr, XASS);
  }
  if (type & WDP_SMSC_ADDRESS) {
    yAssF_AuxiliaryAddress (saved_smsc_addr, *addr2, XASS);
  }
  if (type & WDP_USSDC_ADDRESS) {
    yAssF_AuxiliaryAddress (saved_ussdc_addr, *addr2, XASS);
  }
}

/*
 * Retrieve a saved address. The first parameter is a bit mask
 * that indicates which address(es) should be retrieved.
 */
void
wdp_address_get (SDL_Integer type, DeviceAddress *addr,
                 AuxiliaryAddress *addr2)
{
  if (type & WDP_CLIENT_ADDRESS) {
    yAssF_DeviceAddress (*addr, saved_client_addr, XASS);
  }
  if (type & WDP_SMSC_ADDRESS) {
    yAssF_AuxiliaryAddress (*addr2, saved_smsc_addr, XASS);
  }
  if (type & WDP_USSDC_ADDRESS) {
    yAssF_AuxiliaryAddress (*addr2, saved_ussdc_addr, XASS);
  }
}

