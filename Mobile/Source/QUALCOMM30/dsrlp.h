#ifndef _DSRLP_H
#define _DSRLP_H

/*===========================================================================

  D A T A   S E R V I C E S   R L P   E X T E R N A L   H E A D E R  F I L E
                   
DESCRIPTION
   This file contains external variable and function declarations for the 
   Radio Link Protocol.

   This should be included by files which interface with RLP, like the
   MuxLayer and Call Processing.
   
   Note, order of fields in structs is optimized to save memory in 32bit
   ARM environment. This should be kept in mind when changing the ordering
   of fields.

Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/dsrlp.h_v   1.2   28 Feb 2001 00:41:34   akhare  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/22/01   ak      Extern for array which holds RLP Logging IDs.
11/29/00   ak      Added DSRLP_RATE_ERASURE_SCH, which indicates an erasure
                   on the SCH. Due to LTU CRC checks in MUX.
09/05/00   ttl     Added DSRLP_RATE_BLANK_MASK and DSRLP_CAT_BLANK_MASK for
                   Enabling the data TX on SCH when the FCH is muted for data.
07/08/00   ak      Added function to return if RLP has data to send.
06/22/00   igt     Added "state" field in dsrlp_stats_info_type and also
                   added dsrlp_stats_enum to support AT$QCRL3D, AT$QCRL3R
06/07/00   ak      Removed prototype for dsrlp_setup_watermarks, since that
                   was obsoleted by dsrlp_reg_srvc.
06/05/00   ak      Extern for dsrlp_reg_srvc(), which is how dsmgr registers
                   with RLP.
06/02/00   ak      Added void to new functions to remove compiler warnings.
05/30/00   igt     added dsrlp_get_max_ms_nak_rounds_fwd() and _rev()
05/10/00   igt     added reset_timestamp to statistics structure.
05/09/00   igt     added RLP_BLOB_used, curr_tx_nak_rounds to stat structure.
04/28/00   igt     added fields to dsrlp_stats_info_type
03/21/00   igt     add dsrlp_get_stats(), dsrlp_clear_stats() and stat struct
02/15/00   igt     Added signatures for BLOB process and creation functions
07/28/99   na/hcg  Created module.

===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#if defined( FEATURE_DS_RLP1) || defined( FEATURE_DS_RLP2) ||  \
    defined( FEATURE_DS_RLP3)
#include "dsm.h"
#include "log.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Until the code can handle multiple sr_id's, a constant fixed value will
  be used for sr_id.
---------------------------------------------------------------------------*/
#define DSRLP_FIXED_SR_ID         1

/*---------------------------------------------------------------------------
  Enum of the different RLP versions that this code supports.
---------------------------------------------------------------------------*/
typedef enum
{
  DSRLP_REV_1=0,                        /* RLP Type I                      */
  DSRLP_REV_2,                          /* RLP Type II                     */
  DSRLP_REV_3,                          /* RLP Type III                    */
  DSRLP_REV_HDR,                        /* RLP for HDR                     */
  DSRLP_NUM_REVS                        /* Max num of RLP Versions- unused */
} dsrlp_ver_enum_type;

/*---------------------------------------------------------------------------
  Enum definition for different MuxPDU types. 
  MuxPDU_1 - Rateset1 for FCH/DCCH/SCCH
  MuxPDU_2 - Rateset2 for FCH/DCCH/SCCH
  MuxPDU_3 - For SCH.
---------------------------------------------------------------------------*/
typedef enum
{
  DSRLP_MUX_PDU_1 = 0,
  DSRLP_MUX_PDU_2,
  DSRLP_MUX_PDU_3,
  DSRLP_MUX_PDU_MAX
} dsrlp_mux_pdu_enum_type;

/*---------------------------------------------------------------------------
  Enum definition for the service type (primary/secondary/ dont care).
---------------------------------------------------------------------------*/
typedef enum
{
  DSRLP_PRIMARY_SRVC = 0,
  DSRLP_SECONDARY_SRVC,
  DSRLP_GENERIC_SRVC
} dsrlp_srvc_enum_type;

/*---------------------------------------------------------------------------
  Enum for the different frame rates supported by RLP.
---------------------------------------------------------------------------*/
typedef enum
{
  DSRLP_RATE_1 =0,
  DSRLP_RATE_7_8,
  DSRLP_RATE_3_4,
  DSRLP_RATE_1_2,
  DSRLP_RATE_7_16,
  DSRLP_RATE_3_8,
  DSRLP_RATE_1_4,
  DSRLP_RATE_3_16,
  DSRLP_RATE_1_8,
  DSRLP_RATE_1_16,
  DSRLP_RATE_ERASURE,                                       /* implies FCH */
  DSRLP_RATE_BLANK,                                         /* implies FCH */
  DSRLP_RATE_ERASURE_SCH                                    /* on the SCH  */
} dsrlp_rate_enum_type;

/*---------------------------------------------------------------------------
  Structure of RLP frames received on the forward link - each frame is put
  into this structure by the Mux and this is passed onto RLP.
---------------------------------------------------------------------------*/
typedef struct 
{
  boolean valid_frame;                  /* TRUE-good frame; FALSE- erasure */
  dsrlp_srvc_enum_type srvc_type;       /* Primary/Sec./NA                 */
  dsrlp_mux_pdu_enum_type mux_pdu_type; /* MuxPDU type                     */
  dsrlp_rate_enum_type    fr_rate;      /* Received frame rate             */
//  uint16 frame_len;                     /* Frame len( #bytes in frame_item)*/
  dsm_item_type *fr_item_ptr;           /* Ptr to frame payload            */
} dsrlp_fwd_frame_type;

/*---------------------------------------------------------------------------
  Type definition for all the frames passed to RLP by the mux layer.
---------------------------------------------------------------------------*/
#define RLP_MAX_FWD_PDU   9    /* Max # of RLP frames passed by Mux to RLP */

typedef struct
{
  byte sr_id;           /* Service ID - identifies service                 */
  byte num_rlp_frames;  /* # of RLP frames in frame_array; 0 means erasure */
  uint64 fr_time;       /* MUX: system time - used for logging             */
  dsrlp_fwd_frame_type fwd_arr[ RLP_MAX_FWD_PDU];  
                                              /* array of rx'ed RLP frames */
} dsrlp_fwd_frame_block_type;

/*---------------------------------------------------------------------------
  Enum for the return value dsrlp_rx_process_frames.
---------------------------------------------------------------------------*/
typedef enum
{
  DSRLP_RX_SYNC_TIMEOUT,                /* RLP timed out trying to SYNC    */
  DSRLP_RX_IDLE,                        /* Idle frame(s) rx'ed             */
  DSRLP_RX_NON_IDLE,                    /* Ctl/Data/Retx frame rx'ed       */
  DSRLP_RX_ERROR                        /* Errors in rx'ed frame(s)        */
} dsrlp_rx_retval_enum_type;

/*---------------------------------------------------------------------------
  Masks for different RLP rates. These masks are used in the fr_rate_mask
  field in dsrlp_rev_frame_struct. These masks are ored together by the Mux
  layer to tell RLP, which rates are available.
---------------------------------------------------------------------------*/
#define DSRLP_RATE_1_16_MASK  0x0001
#define DSRLP_RATE_1_8_MASK   0x0002
#define DSRLP_RATE_3_16_MASK  0x0004
#define DSRLP_RATE_1_4_MASK   0x0008
#define DSRLP_RATE_3_8_MASK   0x0010
#define DSRLP_RATE_7_16_MASK  0x0020
#define DSRLP_RATE_1_2_MASK   0x0040
#define DSRLP_RATE_3_4_MASK   0x0080
#define DSRLP_RATE_7_8_MASK   0x0100
#define DSRLP_RATE_1_MASK     0x0200
#define DSRLP_RATE_BLANK_MASK 0x0400

/*---------------------------------------------------------------------------
  Masks for the different frame categories that RLP can use to make a frame.
  The MUX layer uses these masks depending on the physical channel in use.
---------------------------------------------------------------------------*/
#define DSRLP_CAT_DATA_MASK   0x01      /* RLP can make new/rexmit data fr */
#define DSRLP_CAT_CTL_MASK    0x02      /* RLP can make init/NAK ctl frames*/
#define DSRLP_CAT_IDLE_MASK   0x04      /* RLP can make idle/fill frames   */
#define DSRLP_CAT_BLANK_MASK  0x08      /* RLP can make BLANK frames       */

/*---------------------------------------------------------------------------
  Structure of RLP frames to be transmitted on the reverse link. The fields
  with "MUX:" are set by the Mux layer, and the fields with "RLP:" are set
  by RLP.
---------------------------------------------------------------------------*/
typedef struct 
{
  uint16 max_frame_size;                /* MUX: Max. # of bytes in frame   */
  uint16 fr_rate_mask;                  /* MUX: Avail. rates for this frame*/
  dsrlp_srvc_enum_type srvc_type;       /* MUX: Primary/Sec./NA            */
  dsrlp_mux_pdu_enum_type mux_pdu_type; /* MUX: MuxPDU type                */
  byte fr_cat_mask;                     /* MUX: Can send New/Ctl/Idle/Init */
  boolean idle_frame;                   /* RLP: idle fr- can DTX if TRUE   */
  dsrlp_rate_enum_type fr_rate;         /* RLP: Current frame rate         */
  boolean free_dsm_pkt;                 /* RLP: Free the dsm pkt after tx  */
  dsm_item_type *fr_item_ptr;           /* RLP: Ptr to frame payload       */
} dsrlp_rev_frame_type;                

/*---------------------------------------------------------------------------
  Type definition for all the frames the MUX layer gets from RLP. This is 
  passed once every frame time to RLP.
---------------------------------------------------------------------------*/
#define RLP_MAX_REV_PDU   9 /* Max # of RLP frames passed by Mux to RLP    */

typedef struct 
{
  byte sr_id;               /* MUX: Service Identifier                     */
  byte max_frames;          /* MUX: Max. # of RLP frames allowed in fr_arr */
  byte num_frames;          /* RLP: # of RLP frames in frame_array         */
  uint64 fr_time;           /* MUX: system time - used for logging         */
  dsrlp_rev_frame_type rev_arr[ RLP_MAX_REV_PDU];  
                            /* RLP: Actual frame payload                   */
} dsrlp_rev_frame_block_type;

/*---------------------------------------------------------------------------
  Array that holds the log id's for rlp logging.
---------------------------------------------------------------------------*/
extern const 
           log_rlp_phy_channel_enum_type dsrlp_log_id_table[RLP_MAX_REV_PDU];

/*---------------------------------------------------------------------------
  Array that holds the len in bytes for every rate on FCH/DCCH.
---------------------------------------------------------------------------*/
extern const byte dsrlp_len_table[DSRLP_RATE_ERASURE][DSRLP_GENERIC_SRVC]
                          [DSRLP_MUX_PDU_3];

/*---------------------------------------------------------------------------
  Array that holds the len in bytes for every rate on FCH/DCCH.  This table
  however, is used in setting the "used" field in dsm items.  Needed because
  some rlp frames are not evenly divisible by 8.
---------------------------------------------------------------------------*/
extern const byte dsrlp_used_len_table[DSRLP_RATE_ERASURE]
                                      [DSRLP_GENERIC_SRVC]
                                      [DSRLP_MUX_PDU_3];



#define DSRLP_MAX_NAK_ROUNDS    3

#define DSRLP_BLOB_NOT_USED     0 /* RLP BLOB was not received             */
#define DSRLP_BLOB_NAK_SCHEME   1 /* RLP BLOB specified NAK scheme         */
#define DSRLP_BLOB_RTT          2 /* RLP BLOB specified RTT                */ 
#define DSRLP_BLOB_RTT_PLUS_NAK 3 /* RLP BLOB specified RTT & NAK scheme   */

/*---------------------------------------------------------------------------
  RLP statistics information definition. This structure contains all of the 
  RLP statistical information for the currently active RLP session.
---------------------------------------------------------------------------*/
typedef struct
{
  
  uint32 rx_rexmit_fr_cnt;         /* # of received re-transmitted frames   */
  uint32 rx_idle_fr_cnt;           /* # of received idle frames             */
  uint32 rx_fill_fr_cnt;           /* # of received fill frames             */
  uint32 rx_blank_fr_cnt;          /* # of blank frames received            */
  uint32 rx_null_fr_cnt;           /* # of null frames received             */
  uint32 rx_new_data_fr_cnt;       /* total # of received new data frames   */ 
  uint32 rx_20ms_fr_cnt;           /* # of received Fundamental data frames */
  
  uint32 tx_rexmit_fr_cnt;         /* # of transmitted re-transmitted frames*/
  uint32 tx_idle_fr_cnt;           /* # of transmitted idle frames          */
  uint32 tx_new_data_fr_cnt;       /* total # of new transmitted data frames*/ 
  uint32 tx_20ms_fr_cnt;           /* # of txed Fundamental data frames     */

  uint32 naks_received ;           /* total # of nak frames received        */
  
  uint32 re_xmits_not_found;       /* # of Rexmits not found                */

  uint32 rx_total_bytes;           /* total # of bytes received             */
  uint32 tx_total_bytes;           /* total # of bytes transmitted          */


  uint32 rx_rlp_erasures;          /* # of rlp erasures received            */
  uint32 rx_mux_erasures;          /* # of mux erasures received            */
  uint16 largest_contig_erasure;   /* largest block of consecutive erasures */

  uint8  resets;                   /* # of resets                           */
  uint8  aborts;                   /* # of aborts    (nak aborts)           */

  uint32 last_rtt;                 /* round trip time                       */
  
  uint64 reset_timestamp;          /* Timestamp when these stats were reset */
  
  uint8  RLP_BLOB_used;            /* keep track of how RLP_BLOB is used    */
  uint8  curr_tx_nak_rounds;       /* # tx nak rounds used at this time     */
  
  uint32 tx_naks[DSRLP_MAX_NAK_ROUNDS];
                                   /* single, double etc NAKs sent array    */
  
  uint16 state;                    /* current state of the rlp              */

} dsrlp_stats_info_type;


#ifdef FEATURE_DS_PSTATS
#ifdef FEATURE_DS_RLP3
/*---------------------------------------------------------------------------
  Type to index the array of RLP stats.
  These stats apply to the current or most recent call.
---------------------------------------------------------------------------*/
typedef enum
{
  DSRLP_STATS_RX_DATA_CNT,         /* # of data frames received            */
  DSRLP_STATS_TX_DATA_CNT,         /* # of data frames Transmitted         */
  DSRLP_STATS_NAKS,                /* count of NAKs sent                   */
  DSRLP_STATS_DOUBLE_NAKS,         /* count of 2-in-a-row NAKs sent        */
  DSRLP_STATS_TRIPLE_NAKS,         /* count of 3-in-a-row NAKs sent        */
  DSRLP_STATS_RX_NAKS,             /* total # of NAKs received             */

/*DSRLP_STATS_TX_REXMITS,             count of REXMITs sent                */
/*DSRLP_STATS_RX_REXMITS,             count of REXMITs received            */
/*DSRLP_STATS_TX_IDLE_CNT,            # of transmitted IDLE frames         */
/*DSRLP_STATS_RX_IDLE_CNT,            # of received    IDLE frames         */
/*DSRLP_STATS_RX_BLANK_CNT,           # of received BLANK frames           */
/*DSRLP_STATS_RX_NULL_CNT,            # of received NULL frames            */
/*DSRLP_STATS_TX_FUNDAMENTAL,         # of txed  Fundamental Data frames   */
/*DSRLP_STATS_RX_FUNDAMENTAL,         # of received Fundamental Data frs   */
  
  DSRLP_STATS_TX_TOTAL_BYTES,      /* # of total bytes transmitted         */
  DSRLP_STATS_RX_TOTAL_BYTES,      /* # of total bytes received            */


  DSRLP_STATS_RE_XMITS_NOT_FOUND,  /* count of REXMITs not found           */
  DSRLP_STATS_FILL_FRAMES_RXED,    /* count of fill frames received        */

  DSRLP_STATS_RLP_ERASURES,        /* count of rlp level erasures          */
  DSRLP_STATS_MUX_ERASURES,        /* count of mux level erasures          */

  DSRLP_STATS_LARGEST_CONTIG_ERASE,/* largest block of contig. erasures    */
  
  DSRLP_STATS_LAST_RTT,            /* last round-trip time                 */
  
  DSRLP_STATS_RESETS,              /* count of RLP resets                  */
  DSRLP_STATS_ABORTS,              /* # of NAK aborts                      */

/*DSRLP_STATS_BLOB_USED,              shows if BLOB has been used and how  */

/*DSRLP_STATS_CURR_TX_NAK_RNDS,       # of tx nak rounds currently used    */

  DSRLP_STATS_RLP_STATE,           /* current state of the rlp             */

  DSRLP_STATS_COUNT                /* number of RLP stats                  */

} dsrlp_stats_enum;

/*---------------------------------------------------------------------------
  Array of pointers to stats names.
---------------------------------------------------------------------------*/
extern const byte * const dsrlp_stats_name_ptr[DSRLP_STATS_COUNT];

/*---------------------------------------------------------------------------
  Array of pointers to names of the last call ending state.
---------------------------------------------------------------------------*/
extern const byte * const dsrlp_last_call_synced[3];

/*---------------------------------------------------------------------------
  Pointer (into dsrlp_last_call_synced) to name of last call ending state.
---------------------------------------------------------------------------*/
extern byte * dsrlp_last_call_synced_ptr;

/*---------------------------------------------------------------------------
  Copy of the last round trip time.
---------------------------------------------------------------------------*/
extern word dsrlp_last_rtt;
#endif /* FEATURE_DS_RLP3   */
#endif /* FEATURE_DS_PSTATS */




/*===========================================================================

  
                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION       DSRLP_STARTUP

DESCRIPTION    This function is to be called only once per power-up
               of the phone.  This function initializes any structs
               which may be global across all RLP versions or instances.
               
               DSRLP_INIT is called per RLP instance, and so can be 
               called many times, but this function must be called
               only once, and before any other RLP functions are called.


DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None
===========================================================================*/
void dsrlp_startup(void);

/*===========================================================================

FUNCTION DSRLP_INIT

DESCRIPTION
  This function initializes the RLP session corresponding to the passed
  sr_id for the given version of RLP. All data structures are setup and 
  RLP is reset to start new operation. This function should be called at the
  beginning of each call for each service.
  
  Input Parameters:
    sr_id: The Service ID for the particular service. Primary is usually 1
           and secondary is usually 2.
    ver  : The RLP version - 1,2,3... 
    octet_sequenced: This RLP will run in byte sequenced mode. This means
           that all new frames and retransmissions will have a single
           byte sequence space. 
           Mode of operation cannot be changed in the middle of a RLP
           session.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsrlp_init
(
  byte sr_id,                                /* Service ID                 */
  dsrlp_ver_enum_type ver,                   /* RLP version                */
  boolean octet_sequenced                    /* Byte sequenced operation?  */
);
/*===========================================================================

FUNCTION DSRLP_CLEANUP

DESCRIPTION
  This function cleansup the memory and resets the RLP state for a given
  sr_id. This should be called at the end of a RLP session.
  
  Input Parameters:
    sr_id: The Service ID for the particular service. Primary is usually 1
           and secondary is usually 2.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsrlp_cleanup
(
  byte sr_id                                 /* Service ID                 */
);
/*===========================================================================

FUNCTION DSRLP_PROCESS_RX_FRAMES

DESCRIPTION
  This function processes the RLP frame(s) received over the air. This is the
  interface between RLP receive and the multiplex sublayer. The multiplex 
  sublayer can send all the frames it received in one frame time to RLP or
  split it up into multiple calls of this function.
  
  Input Parameters:

  The parameter to this function is a pointer to a structure which contains
  an array of frames that the mux sublayer wants RLP to process. Each element
  of this array includes all the information that RLP needs to process the 
  frame independently. The information passed with each frame includes a 
  pointer to the dsm item containing the frame, the muxPDU type, service type
  (primary or secondary).

  The fr_item_ptr can be a pointer to dsm packet chain (with multiple
  dsm items).

  This per frame data structure is passed to a number of functions within 
  the RLP code.

DEPENDENCIES
  dsrlp_init() should have been previously invoked. rlp_establish_srvc_type()
  should have been previously invoked to set up the traffic type, rate set
  and RLP type in use.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsrlp_process_rx_frames
(
  dsrlp_fwd_frame_block_type *fwd_frame_block/* Ptr to rcvd RLP frames     */
);
/*===========================================================================

FUNCTION DSRLP_BUILD_TX_FRAMES

DESCRIPTION
  This function builds RLP frame(s) to be transmitted over the air. 
  This is the interface between RLP transmit and the multiplex sublayer. 
  The multiplex sublayer can either call this function once, to build all
  the frames or call this several times to build a few frames at a time.
  
  Input Parameters:

  The parameter to this function is a pointer to a structure which contains
  an array of frames that the mux sublayer wants RLP to process. Each element
  of this array includes all the information that RLP needs to process the 
  frame independently.   This per frame data structure is passed to a number
  of functions within the RLP frame building code.The information passed
  with each frame includes fields which are set by the mux layer, and fields
  which are set by RLP, when RLP builds the frame. 

  Fields set by Mux layer for entire se of frames asked.
  
    sr_id         : service id for this frame. For SCH frames, sr_id is in 
                    the MuxPDU header. For FCH/DCCH frames, this is set by 
                    the Mux layer after looking up the appropriate sr_id for
                    primary or secondary. MUX layer must ask for frames of 
                    one sr_id in one invocation of this function.
  
  The fields set by the mux layer per frame:
                    
    srvc_type     : For FCH/DCCH frames, the mux layer sets the srvc_type 
                    to DSRLP_PRIMARY_SRVC or DSRLP_SECONDARY. For SCH/SCCH,
                    Mux layer sets the srvc_type to DSRLP_GENERIC_SRVC.
                    
    mux_pdu_type  : Set to DSRLP_MUX_PDU_1 for rateset1 FCH/DCCH; set to
                    DSRLP_MUX_PDU_2 for rateset2 FCH/DCCH; set to 
                    DSRLP_MUX_PDU_3 for SCH operation.
                    
    fr_cat_mask   : For FCH/DCCH, Mux layer sets this mask to 
                    ( DSRLP_CAT_DATA_MASK | DSRLP_CAT_CTL_MASK | 
                      DSRLP_CAT_IDLE_MASK), because data, ctl/idle frames
                    can be sent on FCH/DCCH.
                    For SCH/SCCH, Mux layer sets this mask to 
                    DSRLP_CAT_DATA_MASK, because only data frames can be
                    sent on SCH/SCCH.
                    
    max_frame_size: This specifies the max. number of bytes avail. for RLP to 
                    put in the frame. For MuxPDU type1 and 2, when multiple 
                    frame rates are available, the Mux layer should set 
                    this field to the size of the highest frame rate.
                    The Mux layer sets this field to the number of bytes 
                    available for RLP to put itsheader and payload excluding
                    the type bits.
                    Basically, take the number of bits of RLP payload as
                    specified in the standard, subtract 8 from it, divide it 
                    by 8 and round up to the next byte. 
                    e.g for secondary rate1 frames on FCH rateset1, 
                    max_frame_size = (168-8)/8 = 20 bytes.
                    
                    For MuxPDU3, this will be set to MuxPDU size-1. e.g. for
                    SCH double size LTUs in Service Option 33, this will be
                    43 bytes.
                    
    fr_rate_mask  : The mux layer ors in all the different frame rates 
                    allowed for this frame in this mask. For FCH, the mask
                    can take on a whole range of values depending on primary
                    or secondary service. For DCCH/SCH/SCCH, the mask is 
                    usually DSRLP_RATE_1_MASK.
  
  
  The fields set by the RLP:
  
    idle_frame    : RLP sets this to TRUE, if this frame can be DTXed by
                    the Mux layer (for DCCH/secondary operation). When this
                    is FALSE, the Mux layer has to send out this frame.
                    
    fr_rate       : RLP sets this to the frame rate this frame should be 
                    used by the Mux layer to send out this frame.
                                    
    fr_item_ptr   : Pointer to dsm packet chain (may have multiple dsm items)
                    RLP should ensure that the used fields are properly set
                    in the item, to the number of bits used by RLP rounded
                    up to the next byte.
                    The mux layer shall transmit the number of bytes in the
                    item chain. If there are any left over bits, the Mux
                    layer shall pad the bits with zeros.
                    RLP shall set the unused part of the item chain to zeros
        
DEPENDENCIES
  dsrlp_init() should have been called. 
  
  Mux layer must ask for frames for only one sr_id in one invocation of this
  funciton.
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsrlp_build_tx_frames
(
  dsrlp_rev_frame_block_type *rev_fr_blk     /* Ptr to frames to trasmit   */
);

/*===========================================================================

FUNCTION DSRLP_BUILD_RLP_BLOB

DESCRIPTION

  This function builds an RLP BLOB (Block of Bytes)
    
  Input Parameters:

  *blob_ptr:   is a pointer pointing to free space for new blob be built.  
  

DEPENDENCIES
  None

RETURN VALUE
  *blob_ptr: is a pointer pointing to the blob that was build.
   
   blob_len: is returned by the function and contains the length of the new
             blob. 

SIDE EFFECTS
  None
===========================================================================*/
byte dsrlp_build_rlp_blob
(
  byte  *blob_ptr 
);

/*===========================================================================

FUNCTION DSRLP_PROCESS_RLP_BLOB

DESCRIPTION
  This function processes the received RLP BLOB (Block of Bytes)
    
  Input Parameters:
  
  sr_id: used to find the appropriate session control block pointer
  
  *blob_ptr: points to the BLOB to be processed
  
  blob_len: specifies the length of the BLOB to be processed
  
DEPENDENCIES
  A valid sr_id value must be passed into this function.
  
  Also the session control block pointed to by the rscb_ptr should have been 
  initialized and have valid values as this function uses that structure.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean dsrlp_process_rlp_blob
(
  byte   sr_id, 
  byte  *blob_ptr, 
  byte   blob_len
);

/*===========================================================================

FUNCTION DSRLP_GET_STATS

DESCRIPTION
  This function gets an sr_id and then returns a pointer to the appropriate
  structure containing the related statistics.
    
  Input Parameters:
  
  sr_id:         used to find the appropriate session control block pointer
  
  stats_ptr_ptr: is a pointer to a pointer to the stats structure 
  
DEPENDENCIES
  A valid sr_id value must be passed into this function.
  
  Also the session control block pointed to by the rscb_ptr should have been 
  initialized and have valid values as this function uses that structure.

RETURN VALUE
  stats_ptr_ptr:  is the pointer to the pointer to the stats structure

SIDE EFFECTS
  None
===========================================================================*/
void dsrlp_get_stats
(
  byte                      sr_id,  
  dsrlp_stats_info_type   **stats_ptr_ptr
);


/*===========================================================================

FUNCTION DSRLP_CLEAR_STATS

DESCRIPTION
  This function takes an sr_id and uses that to get the appropriate session
  control block pointer and from there it will initialize the related 
  structure with the statistics.
    
  Input Parameters:
  
  sr_id:         used to find the appropriate session control block pointer
  
   
DEPENDENCIES
  A valid sr_id value must be passed into this function.
  
  Also the session control block pointed to by the rscb_ptr should have been 
  initialized and have valid values as this function uses that structure.

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void dsrlp_clear_stats
(
    byte sr_id
);


#ifdef FEATURE_DS_PSTATS
#ifdef FEATURE_DS_RLP3
/*===========================================================================

FUNCTION DSRLP_GET_STATS_AT

DESCRIPTION
  This function copies the RLP stats into the given array.
  This particular function is for use with the AT commands.

DEPENDENCIES
  The given array must be of size DSRLP_STATS_COUNT or greater.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsrlp_get_stats_at
(
  uint32 *stats_ptr,  /* pointer to array to load with stats */
  byte    sr_id
);
#endif /* FEATURE_DS_RLP3   */
#endif /* FEATURE_DS_PSTATS */

/*===========================================================================

FUNCTION DSRLP_GET_MAX_MS_NAK_ROUNDS_FWD

DESCRIPTION
  This function the MAX number of nak rounds the mobile will support
  on the forward channel
    
  Input Parameters: None

DEPENDENCIES
  None

RETURN VALUE
  The max number of nak rounds
  
SIDE EFFECTS
  None
===========================================================================*/
byte dsrlp_get_max_ms_nak_rounds_fwd(void);


/*===========================================================================

FUNCTION DSRLP_GET_MAX_MS_NAK_ROUNDS_REV

DESCRIPTION
  This function the MAX number of nak rounds the mobile will support
  on the reverse channel
    
  Input Parameters: None

DEPENDENCIES
  None

RETURN VALUE
  The max number of nak rounds
  
SIDE EFFECTS
  None
===========================================================================*/
byte dsrlp_get_max_ms_nak_rounds_rev(void);


/*===========================================================================
FUNCTION      DSRLP_REG_SRVC

DESCRIPTION   Registers the watermarks and queues/callbacks for an RLP 
              session.
   
DEPENDENCIES  A valid sr_id value must be passed into this function.
  
RETURN VALUE  None
  
SIDE EFFECTS  None
===========================================================================*/
void dsrlp_reg_srvc
(
  byte                sr_id,                 /* session id                 */
  dsm_watermark_type  *tx_watermark_ptr,     /* ptr to Tx wm for txmit frs */
  void                (*post_rx_func_ptr)(), /* rx callback for rx frames  */
  q_type              *post_rx_q_ptr         /* queue for rx'ed frames     */
);

/*===========================================================================
FUNCTION      DSRLP_HAS_DATA_TO_SEND

DESCRIPTION   Returns TRUE or FALSE if there is data (new data or rexmits)
              to send.
   
DEPENDENCIES  The passed in SR_ID is good.
  
RETURN VALUE  TRUE = there is data to send.  FALSE = no data to send.
  
SIDE EFFECTS  None
===========================================================================*/
boolean dsrlp_has_data_to_send
(
   byte sr_id                            /* passed in SR_ID                */
);
#endif  /* FEATURE_DS */
#endif  /* _DSRLP_H */


