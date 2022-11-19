#ifdef FEATURE_NV_HANDOVER_BLOCK
#ifndef NVI_HNDVR_H
#define NVI_HNDVR_H
/*==========================================================================

           N V   H A N D O V E R   B L O C K   H E A D E R

DESCRIPTION

  This header describes the RF handover block.

Copyright (c) 1999 by QUALCOMM Incorporated.  All Rights Reserved.
==========================================================================*/

/*==========================================================================

                        EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/nvi_hand.h_v   1.0   09 Oct 2000 13:38:12   pingguan  $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
10/04/99   rp      Initial ASW checkin to support Digby. 
05/28/99   dlb     Changed NVI_HANDOVER_VERNO_MIN to match factory.
05/24/99   dlb     Added Handover 2.0.
03/09/99   dlb     Fixed ifdef on pragma.
09/16/98   dlb     First implementation from ICD.

==========================================================================*/

/*==========================================================================

                            INCLUDE FILES

==========================================================================*/

#include "comdef.h"

/*==========================================================================

                          DATA DECLARATIONS

==========================================================================*/

/* Handover block 2.0 header. */

/**********************************************************************/
/* Information below is the handover 2.0 header.                      */
/**********************************************************************/

/* A 2.0 handover block consists of a chained sequence of
   "chain_header"s. */

typedef struct {
  dword  chain_header;          /* Identifies this as a 2.0 chain
                                   header. */
  dword  block_type;            /* What kind of block is this. */
  dword  next;                  /* Offset of next chain block, or
                                   NULL. */
  word   length;                /* Number of bytes of data following
                                   this header (depends on the
                                   block_type). */
  word   crc;                   /* An MSB CCITT-16 CRC-16 of the data
                                   and this header. */
} nvi_handover_chain_header_type;

/* Values for the chain_header field. */

#define NVI_CHAIN_HEADER        0x38646E68
/* The only type defined by this file.  Describes a Handover block
   2.0.  Appears as "hnd2" in a dump. */

/* Values for the block_type field. */

#define NVI_BLOCK_TYPE_HAND_1_0 0x302E3168
/* Immediately following the nvi_handover_chain_header_type is a 1.0
   header (nvi_handover_header_type). (appears as "h1.0" in a dump) */

#define NVI_BLOCK_TYPE_FILE     0x656C6966
/* Immediately following the nvi_handover_chain_header_type is a
   (nvi_handover_file_type). (appears as "file" in a dump). */

/* With a chain header type of NVI_BLOCK_TYPE_FILE, the chain header
   is immediately followed by one of these. */

typedef struct {
  word  flags;                  /* File system flags. */
  word  n_segments;             /* How man segments are there. */
  byte  name_length;            /* How many bytes are in the name. */
  byte  name[3];                /* The name itself. */
} nvi_handover_file_type;

/* Given a particular name length, determine how many bytes after the
   nvi_handover_file_type the segment table begins. */
#define NVI_HANDOVER_NAME_SKIP(len)  ((((len)+3)/4)-3)

/* After the header and the name, there are n_segments, of these. */
typedef struct {
  dword  offset;                /* Address of data. */
  dword  length;                /* Length of data. */
} nvi_handover_segment_type;

/* Given a pointer to a nvi_handover_file_type, return a pointer to
   the offset table. */
#define NVI_HANDOVER_OFFSET_TABLE(file)                                 \
  ((nvi_handover_segment_type *)                                        \
   ((byte *) file + NVI_HANDOVER_NAME_SKIP ((file)->name_length)))

/**********************************************************************/
/* Handover 1.0 header.                                               */
/**********************************************************************/

/* Constants for this version of the handover header. */

#define NVI_HANDOVER_VERNO_MAJ  0xFE /* Corresponds with an allocated
                                        major version number in NV. */
#define NVI_HANDOVER_VERNO_MIN  0x01 /* Allocated by NV. */
#define NVI_FORMAT_VER          0x0001 /* Describes this format of the
                                          handover block. */
#define NVI_HANDOVER_OFFSET     8 /* Beginning address of the handover
                                     block in NV. */

#if !defined (T_ARM)
# pragma pack(1)
#endif

/* The handover block header.  The block begins at offset 8 in the NV
   memory. */
typedef PACKED struct {
  byte  verno_maj;              /* Should be NVI_HANDOVER_VERNO_MAJ. */
  byte  verno_min;              /* Should be NVI_HANDOVER_VERNO_MIN. */
  word  nv_format_ver;          /* Should be NVI_FORMAT_VER. */
  word  size;                   /* Size, in bytes, of all of the items
                                   following this header (does not include the
                                   size of the header). */
  word  offset;                 /* Offset from the beginning of NV to the
                                   first data item.  Must be immediately
                                   following this header. */
  word  crc;                    /* A MSB CCITT-16 CRC-16 of the data and this
                                   header. */
} nvi_handover_header_type;

/* Each item is then preceeded by this header.  The data bytes immediately
   follow this header (there is no padding). */
typedef PACKED struct {
  word  nv_item_number;         /* Corresponds to an entry from
                                   nv_item_type. */
  byte  item_size;              /* Number of bytes of data following this
                                   header. */
} nvi_handover_item_header_type;

#if !defined (T_ARM)
# pragma pack()
#endif

#endif /* not NVI_HNDVR_H */
#endif /* #ifdef FEATURE_NV_HANDOVER_BLOCK */
