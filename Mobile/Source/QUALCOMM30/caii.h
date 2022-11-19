#ifndef CAII_H
#define CAII_H
/*===========================================================================

         C D M A   I N T E R N A L   D A T A   S T R U C T U R E S

DESCRIPTION
  This file contains data structures and constants which represent
  the messages transmitted between the mobile and base stations while
  operating in the CDMA mode. This format is only used internally to
  the Main Control task.

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/caii.h_v   1.4   16 Feb 2001 10:24:14   bcalder  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/14/01   bgc     Moved unnecessary PACKED directives into PLT code.
01/24/01   ych     Merged T53 and JCDMA features.
01/02/01   lcc     Changed member rev_sch_rc_map from word to byte in
                   caii_rev_sch_spec_fields_type.
09/06/00   jq      Implemented FEATURE_FACTORY_TESTMODE based on FEATURE_PLT
07/06/00   jq      Added definititions for ECCLM and EGSRM. Modified CFS_RSP
                   and GSRM for addendum compliance.
06/20/00   fc      Renamed MAX_SCRM_BLOB_SIZ to CAI_MAX_SCRM_BLOB_SIZ,
                   MAX_SCRM_ACT_PN to CAI_MAX_ACT_PN and MAX_SCRM_NGHBR_PN to
                   CAI_MAX_SCRM_NGHBR_PN.
06/19/00   jrw     Corrected a problem with power control message where the
                   init_setpt shouldn't have been included with the SCH set points.
06/12/00   ks      Added support for align_timing field in CFSREQ message.
06/02/00   jrw     Changes to support IS2000 Rev 0 published addendum.
05/26/00   jq      Added CAII_BC_HDR_LEN
05/25/00   ks      Added support for Neighbor Search Window offset.
04/24/00   lcc/jrw Changed usage of SRCH_NSET_MAX and SRCH_ASET_MAX to be CAI_N8M
                   and CAI_N6M instead.  This avoid inclusion of srch.h in this
                   file, which can be problematic.
                   Added ESCAM, outer loop report message, and changes to PMRM and
                   CFSRM.
04/06/00   va      Merged with MSM3100_CP.03.00.17
           jq      Added broadcast SMS support.
03/21/00   va      Removed IS2000 #ifndef around srch.h inclusion
03/10/00   ry      Added IS2000 featurization
           ry      Removed FEATURE_IS2000_REV0_PREADDENDUM around
                   caii_feat_rec_type
           ry      Replaced FEATURE_SILK with FEATURE_MSG_PACK_UNIT_TEST
           ry      Merged the following from PP (MSM3100_CP.03.00.10):
           dna     CR12461 CFS Report Message must hold up to 46 entries.
03/06/00   lcc     Corrected some booleans to be bytes instead.
02/29/00   jrw     Removed Phase2 stuff (saved for next release)
02/11/00   jrw     Added support for IS2000 Addendum
02/02/00   hrk     Added PACKED directive around structs for RPC.
12/17/99   jrw     Added support for IS2000 Phase 1.
10/07/99   lh      Fixed caii_redir_rec_fix_type.
08/09/99   ry      Added pre_proc_done to caii_cfs_req_fix_type and
                   caii_cfs_ctrl_type for action time processing
07/09/99   ry      Removed an extraneous '\' from caii_cfs_rpt_mod0_type
06/30/99   kmp     In the caii_ext_disp_var_type struct, made the chari field
                   array size to be one, to appease the ARM 2.50 compiler.
                   See comment in the code for more detail.
06/02/99   kmp     Merge in the following IS-95B changes
           lh      Updated for Published version of IS-95B.
           jq      Added parenthes to some macros
05/19/99   doj     Consolidate structures for HDM, EHDM and GHDM into one
                   generic handoff structure.
02/13/99   doj     Added support for General Handoff Direction Message (GHDM).
                   Also includes several misc. updates by many people to
                   support FEATURE_SPECIAL_MDR.
01/13/99   lcc     Merged in MDR support from IS95B branch.  Only messages
                   needed for MDR are merged in.
01/06/99   lh      Extended Status Response and Status Response now share the
                   same internal formats.
01/06/99   lh      FEATURE_JSTD008 replaced with run-time P_REV and Band
                   Class checking.
12/23/98   lh      Modified ECAM. Now parser will parse the ECAM and pass the
                   caller a packed internal format.
12/16/98   lh      caii_pilot_rec_type defined.
                   ECAM implementation changed.
11/24/98   lh      Added comments.
11/19/98   lh      Added comments.
10/29/98   jcw     Changed Extended Display structs for 95B.
10/12/98   lh      Change soft_slop to soft_slope in ESPM processing.
09/15/98   lh      IS-95B support.
08/12/98   lh      IS95B WLL support: Line Control, Parametric Alerting and
                   Meter Pulses.
01/15/98   lh      Added IS-95B Line Control info record.
01/14/98   dna     Add support for Korean PCS in Band Class Status Response
07/16/97   jca     Deleted #include "ui.h".
12/13/96   jca     Added new fields to caii_chnasn_am2_type.
04/09/96   day     Added TMSI addressing modifications.  Using T_TMSI ifdef.
03/13/96   day     Added J-STD-008 PCS message modifications.  Using FEATURE_JSTD008
                   ifdef.
08/22/95   dna     New sync channel message field
08/18/95   dna     Added CAI_EXT_PAGING_CHAN_ASSIGN assign mode '101'
08/10/95   rdb     Added internal types to support Status Request and Status
                   Response Messages.
08/09/95   jca     Added General Page Message.  Added new Access and Paging
                   Channel address types include support for IMSI addresses.
07/20/95   gb      Added Extended Handoff Direction Message.
07/20/95   jca     Added Extended Sys Parameters and Service Redirection
                   messages.  Modified other messages as per IS95A.
07/13/95   rdb     Added types to support service negotiation.
12/22/93   jca     Changed message formats to IS-95 compatible.
07/23/92   jai     Modified file for lint and linking.
07/03/92   jai     Modified file for CAI 2.0
10/19/90   jai     Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "comdef.h"               /* Basic definitions */
#include "customer.h"
#include "qw.h"                   /* Quadword definitions */
#include "cai.h"                  /* CAI definitions */

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                         IMSI ADDRESS TYPES                              */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* IMSI Class 0 Type specific subfields (Class Type = 0) */
typedef struct
{
  qword imsi_s;             /* Last ten digits of the IMSI */
} caii_imsi_class0_type0_type;

/* IMSI Class 0 Type specific subfields (Class Type = 1) */
typedef struct
{
  byte  imsi_11_12;         /* The 11th and 12th digits of IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
} caii_imsi_class0_type1_type;

/* IMSI Class 0 Type specific subfields (Class Type = 2) */
typedef struct
{
  word  mcc;                /* Mobile country code */
  qword imsi_s;             /* Last ten digits of the IMSI */
} caii_imsi_class0_type2_type;

/* IMSI Class 0 Type specific subfields (Class Type = 3) */
typedef struct
{
  word  mcc;                /* Mobile country code */
  byte  imsi_11_12;         /* The 11th and 12th digits of IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
} caii_imsi_class0_type3_type;

/* Union of structures for IMSI Class 0 type specific subfields */
typedef struct
{
  byte imsi_class_0_type;  /* IMSI Class 0 type */
  union
  {
    caii_imsi_class0_type0_type zero;
    caii_imsi_class0_type1_type one;
    caii_imsi_class0_type2_type two;
    caii_imsi_class0_type3_type three;
  } type;
} caii_imsi_class_0_type;

/* IMSI Class 1 Type specific subfields (Class Type = 0) */
typedef struct
{
  byte  imsi_addr_num;      /* Number of IMSI address digits */
  byte  imsi_11_12;         /* The 11th and 12th digits of IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
} caii_imsi_class1_type0_type;

/* IMSI Class 1 Type specific subfields (Class Type = 1) */
typedef struct
{
  byte  imsi_addr_num;      /* Number of IMSI address digits */
  word  mcc;                /* Mobile country code */
  byte  imsi_11_12;         /* The 11th and 12th digits of IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
} caii_imsi_class1_type1_type;

/* Union of structures for IMSI Class 1 type specific subfields */
typedef struct
{
  byte imsi_class_1_type;  /* IMSI Class 0 type */
  union
  {
    caii_imsi_class1_type0_type zero;
    caii_imsi_class1_type1_type one;
  } type;
} caii_imsi_class_1_type;

/* Union of structures for IMSI type specific subfields */
typedef struct
{
  byte imsi_class;       /* IMSI Class */
  union
  {
    caii_imsi_class_0_type zero;
    caii_imsi_class_1_type one;
  } class;
} caii_imsi_addr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                        GENERIC MESSAGE TYPES                            */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to isolate msg_type */
typedef struct
{
  byte msg_type;         /* Message type */
} caii_gen_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Paging Channel common address fields (see IS-95A Section 7.7.2.3.1) */
typedef struct
{
  dword min1;   /* First part of the mobile identification number */
  word min2;    /* Second part of the mobile identification number */
} caii_addr0_type;

typedef struct
{
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  qword esn;    /* Mobile station's electronic serial number */
} caii_addr1_type;

typedef struct
{
  caii_imsi_addr_type imsi;  /* IMSI address type specific subfields */
} caii_addr2_type;

typedef struct
{
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  qword tmsi_zone;       /* TMSI zone */
  dword tmsi_code;       /* TMSI code */
} caii_tmsi_type;

#define CAII_BC_ADDR_MAX 15
  /* Limited by number of bits in ADDR_LEN field of a Paging Channel Msg */

#define CAII_BC_ADDR_LEN 5
  /* the broadcast address length is 5 bytes */

#define CAII_BC_HDR_LEN (sizeof(cai_pc_hdr_type) + sizeof(cai_addr5_type) * CAII_BC_ADDR_LEN)
  /* the length of data burst header that contains a BC SMS */

typedef struct
{
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte bc_addr[CAII_BC_ADDR_MAX];  /* Broadcast address */
} caii_addr5_type;

typedef union
{
  caii_addr0_type type0;  /* Addr_type '000' subfields */
  caii_addr1_type type1;  /* Addr_type '001' subfields */
  caii_addr2_type type2;  /* Addr_type '010' subfields */
  caii_tmsi_type  type3;  /* Addr_type '011' subfields */
  caii_addr5_type type5;  /* Addr_type '101' subfields */
} caii_pc_address_type;

/* Paging Channel common header fields */
typedef struct
{
  byte ack_seq;               /* Acknowledgement Sequence Number */
  byte msg_seq;               /* Message Sequence Number */
  boolean ack_req;            /* Acknowledgement Required */
  boolean valid_ack;          /* Valid Acknowledgement */
  byte addr_type;             /* Address field type */
  byte addr_len;              /* Address field length */
  caii_pc_address_type addr;  /* Mobile station address */
} caii_pc_hdr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '000') */
typedef struct
{
  dword min1;        /* First part of the mobile identification number */
  word  min2;        /* Second part of the mobile identification number */
  dword esn;         /* Mobile station's electronic serial number */
} caii_msid0_type;

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '001')  */
/* (NOTE: For now the mobile station's ESN field is limited to 32 bits) */
typedef struct
{
  dword esn;         /* Mobile station's electronic serial number */
} caii_msid1_type;

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '010')      */
/* (NOTE: For now the mobile station only supports Class 0 IMSI addresses)  */
typedef struct
{
  caii_imsi_addr_type imsi;  /* IMSI address type specific subfields */
} caii_msid2_type;

/* Access Channel Mobile Station Identifier Fields (MSID_TYPE = '011')      */
/* (NOTE: For now the mobile station only supports Class 0 IMSI addresses)  */
typedef struct
{
  dword esn;                 /* Mobile's electronic serial number */
  caii_imsi_addr_type imsi;  /* IMSI address type specific subfields */
} caii_msid3_type;

/* Union of structures for the Mobile station identifier (MSID) field */
typedef union
{
  caii_msid0_type type0;
  caii_msid1_type type1;
  caii_msid2_type type2;
  caii_msid3_type type3;
  caii_tmsi_type  type5;
} caii_msid_type;

/* Generic Access Channel Message Header */
typedef struct
{
  byte ack_seq;         /* Acknowledgement sequence number */
  byte msg_seq;         /* Message sequence number */
  byte ack_req;         /* Acknowledgement required indicator */
  byte valid_ack;       /* Valid acknowledgement indicator */
  byte ack_type;        /* Acknowledgement address type */
  byte msid_type;       /* Mobile station identifier field type */
  byte msid_len;        /* Mobile station identifier field length */
  caii_msid_type msid;  /* Mobile station identifier */
} caii_ac_hdr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Traffic Channel Layer 2 Header */

typedef struct
{
  byte ack_seq;        /* Acknowledgement sequence number */
  byte msg_seq;        /* Message sequence number */
  boolean ack_req;     /* Acknowledgement Required */
  byte encryption;     /* Message encryption indicator */
} caii_tc_hdr_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*     PAGING AND FORWARD TRAFFIC CHANNEL INFORMATION RECORD TYPES         */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Note - On the Paging Channel, information records may be included in
   the Feature Notification Message.  On the Forward Traffic Channel,
   information records may be included in the Alert With Information
   and the Flash with Information messages.                                */

#ifdef FEATURE_IS95B
#define CAII_MAX_INFO_REC_BYTES 498
  /* Maximum number of bytes allowed to accomodate internal form of variable
     length information record (based on worst case which is the Calling
     Party Subaddress information record received on the Forward Traffic
     Channel):
           2016 bits (maximum forward traffic channel message body size)
         -   24 bits (msg_type + common ack fields + reserved bits)
         -----------
           1992 bits (remaining for information records)
 divided by  24 bits (min size Calling Party Number info record)
         -----------
             83 (max possible Calling Party Number records per msg)
          *   6 (6 bytes per internal formatted record,  to make it even)
         -----------
            498 bytes */

#else
#define CAII_MAX_INFO_REC_BYTES 373
  /* Maximum number of bytes allowed to accomodate internal form of variable
     length information record (based on worst case which is the Calling
     Party Number information record received on the Forward Traffic Channel):
           2016 bits (maximum forward traffic channel message body size)
         -   24 bits (msg_type + common ack fields + reserved bits)
         -----------
           1992 bits (remaining for information records)
 divided by  32 bits (min size Calling Party Number info record)
         -----------
             62 (max possible Calling Party Number records per msg)
          *   6 (bytes per internal formatted record)
         -----------
           372 bytes
         +   1 byte remainder (1992 bits - (32 bits/record * 62 records))
         -----------
           373 bytes */
#endif /* FEATURE_IS95B */

/* Record header type */
typedef struct
{
  byte record_type;    /* Information record type */
} caii_rec_hdr_type;

/* Display record type (see IS-95A Section 7.7.5.1) */
typedef struct
{
  caii_rec_hdr_type hdr;       /* Header information */
  byte              num_char;  /* Number of characters in chari array */
} caii_display_rec_fix_type;

typedef byte caii_display_rec_var_type;

#define CAII_DISPLAY_REC_MAX  247
/*  2016 bits (max forward traffic channel message body size)
  -   24 bits (msg_type + common acknowledgement fields + reserved bits)
  -   16 bits (info record type and length fields)
  -----------
    1976 bits == 247 bytes (remaining for variable chari field) */

typedef struct
{
  caii_display_rec_fix_type fix;
  caii_display_rec_var_type var[CAII_DISPLAY_REC_MAX];
} caii_display_rec_type;

/* Called Party Number record type (see IS-95A Section 7.7.5.2) */
typedef struct
{
  caii_rec_hdr_type hdr;          /* Header information */
  byte              number_type;  /* Type of number */
  byte              number_plan;  /* Numbering plan */
  byte              num_char;     /* Number of characters in chari array */
} caii_called_rec_fix_type;

typedef byte caii_called_rec_var_type;

#define CAII_CALLED_REC_MAX  246
/*  2016 bits (max forward traffic channel message body size)
  -   24 bits (msg_type + common acknowledgement fields + reserved bits)
  -   16 bits (info record type and length fields)
  -    8 bits (fixed type-specific fields including reserved bit)
  -----------
    1968 bits == 246 bytes (remaining for variable chari field) */

typedef struct
{
  caii_called_rec_fix_type fix;
  caii_called_rec_var_type var[CAII_CALLED_REC_MAX];
} caii_called_rec_type;

/* Calling Party Number record type (see IS-95A Section 7.7.5.3) */
typedef struct
{
  caii_rec_hdr_type hdr;          /* Header information */
  byte              number_type;  /* Type of number */
  byte              number_plan;  /* Numbering plan */
  byte              pi;           /* Presentation indicator */
  byte              si;           /* Screening indicator */
  byte              num_char;     /* Number of characters in variable type */
} caii_calling_rec_fix_type;

typedef byte caii_calling_rec_var_type;

#define CAII_CALLING_REC_MAX  245
/*  2016 bits (max forward traffic channel message body size)
  -   24 bits (msg_type + common acknowledgement fields + reserved bits)
  -   16 bits (info record type and length fields)
  -   16 bits (fixed type-specific fields including reserved bits)
  -----------
    1960 bits == 245 bytes (remaining for variable chari field) */

typedef struct
{
  caii_calling_rec_fix_type fix;
  caii_calling_rec_var_type var[CAII_CALLING_REC_MAX];
} caii_calling_rec_type;

/* Use Calling Party Number record type for Connected Number information
   record type (see IS-95A Section 7.7.5.4 Connected Number) */

/* Signal record type (see IS-95A Section 7.7.5.5 Signal) */
typedef struct
{
  caii_rec_hdr_type hdr;          /* Header information */
  byte              signal_type;  /* Signal type */
  byte              alert_pitch;  /* Pitch of the alerting signal */
  byte              signal;       /* Signal code */
} caii_signal_rec_type;

/* Message Waiting record type (see IS-95A Section 7.7.5.6) */
typedef struct
{
  caii_rec_hdr_type hdr;        /* Header information */
  byte              msg_count;  /* Number of waiting messages */
} caii_msg_waiting_rec_type;

/* Service Configuration Record (see IS-95-A TSB). This information
   record is used both on the forward and reverse traffic channels. */

#define  CAII_SRV_CON_MAX  4
  /* Specifies the maximum number of service option connection records
     that the mobile station can process per message. This number is
     somewhat arbitrary, but is well below the number of connection
     records which could actually fit in either a forward or reverse link
     message. */

#ifdef FEATURE_IS2000
typedef struct
{
  byte sch_rc;
  byte coding;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean frame_40_used;
  byte frame_40_max_rate;
  boolean frame_80_used;
  byte frame_80_max_rate;
#endif
} caii_sch_chn_cfg_type;

typedef struct
{
  byte for_sch_id;
  word for_sch_mux;
  caii_sch_chn_cfg_type sch_chn_cfg;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte for_multi_frame_offset;
#endif
} caii_for_sch_type;

typedef struct
{
  byte rev_sch_id;
  word rev_sch_mux;
  caii_sch_chn_cfg_type sch_chn_cfg;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte rev_multi_frame_offset;
#endif
} caii_rev_sch_type;
#endif

typedef struct
{
  caii_rec_hdr_type hdr;  /* Header information */
  word  fwd_mux;          /* Forward link multiplex option */
  word  rev_mux;          /* Reverse link multiplex option */
  byte  fwd_rates;        /* Forward link transmission rates */
  byte  rev_rates;        /* Reverse link transmission rates */
  byte  num_con;          /* Number of service option connections */

  struct
  {
    byte  con_ref;             /* Connection reference */
    word  so;                  /* Service option number */
    byte  fwd_traf;            /* Traffic type used for forward link */
    byte  rev_traf;            /* Traffic type used for reverse link */
#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
    byte ui_encrypt_mode;
    byte sr_id;
    boolean rlp_info_incl;
    byte rlp_blob_len;
    byte rlp_blob[15];
#endif
#endif
  }  con[ CAII_SRV_CON_MAX ];  /* Array of service option connections */
#ifdef FEATURE_IS2000
  boolean fch_cc_incl;
  boolean fch_frame_size;
  byte for_fch_rc;
  byte rev_fch_rc;
  boolean dcch_cc_incl;
  byte dcch_frame_size;
  byte for_dcch_rc;
  byte rev_dcch_rc;
  boolean for_sch_cc_incl;
  byte num_for_sch;
  caii_for_sch_type for_sch[CAI_MAX_NUM_FOR_SCH];
  boolean rev_sch_cc_incl;
  byte num_rev_sch;
  caii_rev_sch_type rev_sch[CAI_MAX_NUM_REV_SCH];
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte rlp_info_len;
  byte rlp_blob[CAI_MAX_RLP_BLOB_SIZE];
#endif
#endif
} caii_srv_cfg_type;

#ifdef FEATURE_IS2000
typedef struct
{
  byte sr_id;
  byte logical_resource;
  byte physical_resource;
  boolean forward_flag;
  boolean reverse_flag;
  byte priority;
} caii_lpm_entry_type;

typedef struct
{
  boolean fpc_incl;
  byte fpc_pri_chan;         // 0 - Power control on on f-fch, 1 - power control on f-dcch
  byte fpc_mode;
  boolean fpc_olpc_fch_incl;
  byte fpc_fch_fer;
  byte fpc_fch_min_setpt;
  byte fpc_fch_max_setpt;
  boolean fpc_olpc_dcch_incl;
  byte fpc_dcch_fer;
  byte fpc_dcch_min_setpt;
  byte fpc_dcch_max_setpt;
  boolean gating_rate_incl;
  byte pilot_gate_rate;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean lpm_ind;
#else
  byte lpm_ind;
#endif
  byte num_lpm_entries;
  caii_lpm_entry_type lpm_entries[CAI_MAX_NUM_LPM_ENTRIES];
} caii_non_neg_srv_cfg_type;
#endif

typedef struct
{
   caii_srv_cfg_type neg_scr; //Negotiable part of the SCR, Same as 95A/B SCR
#ifdef FEATURE_IS2000
   caii_non_neg_srv_cfg_type non_neg_scr; //Non-negotiable part of the SCR
#endif
} caii_complete_srv_cfg_type;

#ifdef FEATURE_IS95B

/* Called Party Subaddress record type ( see IS-95B Section 7.7.5.8) */
/* Calling Party Subaddress info record IS-95B 7.7.5.9 */
/* Connected Subaddress info record IS-95B 7.7.5.10 */
/* Redirecting Subaddress info record IS-95B 7.7.5.12 */
typedef struct
{
  caii_rec_hdr_type hdr;       /* Header information */
  boolean extension_bit;       /* The extension bit */
  byte    subadd_type;         /* Type of subaddress */
  boolean odd_even_ind;        /* The indicator of even/odd bits */
  byte    num_char;            /* Number of characters in chari array */
} caii_subaddr_rec_fix_type;

typedef byte caii_subaddr_rec_var_type;

#define CAII_SUBADDR_REC_MAX  246
/*  2016 bits (max forward traffic channel message body size)
  -   24 bits (msg_type + common acknowledgement fields + reserved bits)
  -   16 bits (info record type and length fields)
  -    8 bits (type specific field)
  -----------
    1968 bits == 246 bytes (remaining for variable chari field) */

typedef struct
{
  caii_subaddr_rec_fix_type fix;
  caii_subaddr_rec_var_type var[CAII_SUBADDR_REC_MAX];
} caii_subaddr_rec_type;

/* Redirecting Number info record IS-95B 7.7.5.11 */
typedef struct
{
  caii_rec_hdr_type hdr;  /* Header information */
  boolean extension_bit1; /* The extension bit */
  byte number_type;       /* Type of number */
  byte number_plan;       /* Numbering plan */
  boolean extension_bit2; /* The extension bit */
  byte pi;                /* Presentation indicator */
  byte si;                /* Screening indicator */
  boolean extension_bit3; /* The extension bit */
  byte redir_reason;      /* The reason for redirection */
  byte num_char;          /* Number of characters in chari array */
} caii_redir_rec_fix_type;

typedef byte caii_redir_rec_var_type;

#define CAII_REDIR_REC_MAX  245
/*  2016 bits (max forward traffic channel message body size)
  -   24 bits (msg_type + common acknowledgement fields + reserved bits)
  -   16 bits (info record type and length fields)
  -   16 bits (type specific fields)
  -----------
    1960 bits == 245 bytes (remaining for variable chari field) */

typedef struct
{
  caii_redir_rec_fix_type fix;
  caii_redir_rec_var_type var[CAII_REDIR_REC_MAX];
} caii_redir_rec_type;

#endif /* FEATURE_IS95B */

#if defined( FEATURE_IS95B ) || defined( FEATURE_IS95B_WLL_ONLY )
/* Meter Pulses record type (see IS-95B Section 7.7.5.13) */
typedef struct
{
  caii_rec_hdr_type hdr;        /* Header information */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte on_time;                 /* Pulse on time */
  byte off_time;                /* Pulse off time */
  byte pulse_cnt;               /* Pulse count */
#ifndef FEATURE_MSG_PACK_UNIT_TEST
  word pulse_freq;              /* Pulse frequency, put here to save 2 bytes*/
#endif
} caii_meter_pulses_rec_type;

/* Parametric Alerting record type (see IS-95B Section 7.7.5.14) */
typedef struct
{
  caii_rec_hdr_type hdr;          /* Header information */
  byte cadence_cnt;               /* Cadence count */
  byte num_groups;                /* Number of groups in variable part*/
} caii_para_alert_fix_type;

typedef struct
{
  byte amplitude;                 /* Amplitude */
  word freq_1;                    /* Frequency 1 */
  word freq_2;                    /* Frequency 2 */
  byte on_time;                   /* On time */
  byte off_time;                  /* Off time */
  byte repeat;                    /* Repeat */
  byte delay;                     /* Delay */
} caii_para_alert_var_type;

#define CAII_PARA_ALERT_MAX   16
/* This is the maximum possible value the base station could send in
   the NUM_GROUP field. */

typedef struct
{
  caii_para_alert_fix_type fix;
  caii_para_alert_var_type var[CAII_PARA_ALERT_MAX];
} caii_para_alert_rec_type;

/* Line Control Record ( see IS-95B Section 7.7.5.15 ) */
typedef struct
{
  caii_rec_hdr_type hdr;  /* Header information */
  boolean pol_inc;        /* Polarity included */
  boolean tgl_mod;        /* Toggle mode */
  boolean rev_pol;        /* Reverse polarity */
  byte    pow_dtm;        /* Power denial time */
} caii_line_ctrl_rec_type;
#endif

#ifdef FEATURE_IS95B
/* Extended Display record ( see IS-95B Section 7.7.5.16 ) */
typedef struct
{
  caii_rec_hdr_type hdr;    /* Header information */
  boolean ext_display_ind;  /* The indicator if Extended Display record */
  byte display_type;        /* Display type */
  byte rec_len;             /* number of bytes in the variable part */
} caii_ext_disp_fix_type;


typedef struct  /* used to access the variable part of Extended Display */
{
  byte display_tag;  /* Indicator of the display information */
  byte display_len;  /* The display length */
                     /*lint -e43 */
  byte chari[1];     /* The character string (not null termd) */
                     /* This structure is only used for packing only. It
                     ** is not instantiated in the parser. The size
                     ** of the array is set to one to appease the ARM
                     ** 2.50 compiler. This is a variable length array
                     ** of length display_len and is determined when
                     ** the record is recieved.
                     */
                     /*lint +e43 */
} caii_ext_disp_var_type;


#define CAII_EXT_DISP_MAX  123
/*  2016 bits (max forward traffic channel message body size)
  -   24 bits (msg_type + common acknowledgement fields + reserved bits)
  -   16 bits (info record type and length fields)
  -    8 bits (type specific field)
  -----------
    1968 bits
  /    8 bits (how many bytes left )
  -----------
     246 byte( for variable variable field) */

typedef struct
{
  caii_ext_disp_fix_type fix;
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte var[CAII_EXT_DISP_MAX];
} caii_ext_disp_type;

#endif /*FEATURE_IS95B */

#ifdef FEATURE_IS2000
/* User Zone Update Record ( see IS-2000 Section 7.7.5.18 ) */
typedef struct
{
  caii_rec_hdr_type hdr;    /* Header information */
  word uzid;                /* User Zone Identifier */
} caii_user_zone_update_type;

/* User Zone Reject Record ( see IS-2000 Section 7.7.5.19 ) */
typedef struct
{
  caii_rec_hdr_type hdr;      /* Header information */
  word reject_uzid;           /* rejected user zone id */
  byte reject_action_indi;    /* rejection action ind */
  boolean uzid_assign_incl;   /* user zone id assignment incl */
  word assign_uzid;           /* assigned user zone id */
                              /*  (if user zone id assignment incl == 1) */
} caii_user_zone_reject_type;
#endif

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*            REVERSE TRAFFIC CHANNEL INFORMATION RECORD TYPES             */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Note - these records are used in the Flash With Information and the
   Status messages (see IS-95A Sections 6.7.2.3.2.3 Flash With Information
   Message and 6.7.2.3.2.8 Status Message)                                 */

/* Note that Reverse Traffic Channel Info records use the same record
   header (caii_rec_hdr_type) as Forward Traffic Channel Info records */

/* Feature Indicator record type (see IS-95A Section 6.7.4.1) */
typedef struct
{
  caii_rec_hdr_type hdr;      /* Header information */
  byte              feature;  /* Feature identifier */
} caii_feat_rec_type;

/* Keypad Facility record type (see IS-95A Section 6.7.4.2) */
typedef struct
{
  caii_rec_hdr_type hdr;       /* Header information */
  byte              num_char;  /* Number of characters in chari array */
} caii_key_rec_fix_type;

typedef byte caii_key_rec_var_type;

#define CAII_KEY_REC_MAX  ((CAII_MAX_INFO_REC_BYTES -            \
                             sizeof( caii_key_rec_fix_type )) /  \
                               sizeof( caii_key_rec_var_type ))
typedef struct
{
  caii_key_rec_fix_type fix;
  caii_key_rec_var_type var[CAII_KEY_REC_MAX];
} caii_key_rec_type;

/* Use Forward Traffic Channel information record types from above for
   Called Party Number and Calling Party Number infomation records (see
   IS-95A Sections 6.7.4.3 Called Party Number and 6.7.4.4 Calling
   Party Number) */

/* Call Mode record type (see IS-95A Section 6.7.4.6) */
typedef struct
{
  caii_rec_hdr_type hdr;          /* Header information */
  boolean           orig_mode;    /* Origination mode indicator */
  word              pri_service;  /* Primary service option */
  word              sec_service;  /* Secondary service option */
} caii_call_mode_rec_type;

/* Terminal Information record type (see IS-95A Section 6.7.4.7) */
typedef struct
{
  caii_rec_hdr_type hdr;  /* Header information */
  byte mob_p_rev;         /* Protocol revision of mobile station */
  byte mob_mfg_code;      /* Manufacturer code */
  byte mob_model;         /* Model number */
  word mob_firm_rev;      /* Firmware revision number */
  byte scm;               /* Station class mark */
  boolean local_ctrl;     /* Local control indicator */
  byte slot_cycle_index;  /* Slot cycle mode */
  byte num_so;            /* Number of entries in service option array */
} caii_term_rec_fix_type;

#define CAII_TERM_REC_MAX  ((CAII_MAX_INFO_REC_BYTES -           \
                              sizeof( caii_term_rec_fix_type )) /  \
                                sizeof( caii_term_rec_var_type ))

typedef struct
{
  word service_option;    /* Supported service option */
} caii_term_rec_var_type;

typedef struct
{
  caii_term_rec_fix_type fix;
  caii_term_rec_var_type var[CAII_TERM_REC_MAX];
} caii_term_rec_type;

/* Roaming Information record type (see IS-95A Section 6.7.4.8) */
typedef struct
{
  caii_rec_hdr_type hdr;     /* Header information */
  byte accolc;               /* Overload class */
  boolean mob_term_home;     /* Home registration enable indicator */
  boolean mob_term_for_sid;  /* Foreign SID roaming registration enable
                                indicator */
  boolean mob_term_for_nid;  /* Foreign NID roaming registration enable
                                indicator */
  word num_vars;             /* Number of sid,nid pairs included in record */
} caii_roam_rec_fix_type;

typedef struct
{
  word sid;                  /* System identification */
  word nid;                  /* Network identification */
} caii_roam_rec_var_type;

#define CAII_ROAM_REC_MAX  ((CAII_MAX_INFO_REC_BYTES -            \
                             sizeof( caii_roam_rec_fix_type )) /  \
                               sizeof( caii_roam_rec_var_type ))

typedef struct
{
  caii_roam_rec_fix_type fix;
  caii_roam_rec_var_type var[CAII_ROAM_REC_MAX];
} caii_roam_rec_type;

/* Security Status record type (see IS-95A Section 6.7.4.9) */
typedef struct
{
  caii_rec_hdr_type hdr;     /* Header information */
  byte auth_mode;            /* Authentication mode */
  byte encrypt_mode;         /* Message encryption mode */
  boolean private_lcm;       /* Private long code mask indicator */
} caii_sec_stat_rec_type;

/* Use Forward Traffic Channel Connected Number information record type for
   Connected Number type (see IS-95A Sections 6.7.4.10 and 7.7.5.4) */

/* IMSI record type (see IS-95A Section 6.7.4.11) */
typedef struct
{
  caii_rec_hdr_type hdr;     /* Header information */
  byte imsi_class;           /* IMSI Class number */
  byte imsi_addr_num;        /* Number of IMSI address digits */
  word mcc;                  /* Mobile country code */
  byte imsi_11_12;           /* The 11th and 12th digits of IMSI */
  qword imsi_s;              /* Last ten digits of the IMSI */
} caii_imsi_rec_type;

/* ESN record type (see IS-95A Section 6.7.4.12) */
typedef struct
{
  caii_rec_hdr_type hdr;     /* Header information */
  dword esn;                 /* Mobile station electronic serial number */
} caii_esn_rec_type;

/* Band class record type (see IS-95-A TSB) */
typedef struct
{
  caii_rec_hdr_type hdr; /* Header information */
  boolean band0;         /* For band class 0 (800 MHz cellular) */
  boolean band1;         /* For band class 1 (1.8-2.0 GHz PCS) */
  boolean band3;         /* For band class 3 (Japanese Cellular) */
  boolean band4;         /* For band class 4 (Korean PCS) */
#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  boolean band5;
  boolean band6;
#endif
#endif
} caii_band_rec_type;

/* Power class record type (see IS-95-A TSB) */
typedef struct
{
  caii_rec_hdr_type hdr; /* Header information */
  byte max_eirp;         /* Max EIRP (Effective Isotropic Radiated Power) */
} caii_pwr_rec_type;

/* Operating mode record type (see IS-95-A TSB) */
typedef struct
{
  caii_rec_hdr_type hdr; /* Header information */
  boolean op_mode0;      /* ANSI J-STD-008 */
  boolean op_mode1;      /* TIA/EIA/IS-95-A CDMA mode */
  boolean op_mode2;      /* TIA/EIA/IS-95-A analog mode */
  boolean op_mode3;      /* TIA/EIA/IS-91 wide analog mode */
  boolean op_mode4;      /* TIA/EIA/IS-91 narrow analog mode */
} caii_op_mode_rec_type;

/* Service option record type (see IS-95-A TSB) */

typedef struct
{
  caii_rec_hdr_type hdr; /* Header information */
  byte  num_so;          /* Number of so recs in variable part of msg */
} caii_so_rec_fix_type;

typedef struct
{
  boolean fwd;           /* Forward link support indicator */
  boolean rev;           /* Reverse link support indicator */
  word so;               /* Service option number */
} caii_so_rec_var_type;

typedef struct
{
  caii_so_rec_fix_type  fix;       /* Fixed portion of record */
  caii_so_rec_var_type  var[ 1 ];  /* Variable portion. Although array is
                                      dimensioned for 1, there may be an
                                      arbitrary number of array elements */
} caii_so_rec_type;

/* Multiplex option record type (see IS-95-A TSB) */
typedef struct
{
  caii_rec_hdr_type hdr; /* Header information */
  byte num_mux;          /* Number of mux recs in variable part of msg */
} caii_mux_rec_fix_type;

typedef struct
{
  word mux;              /* Multiplex option number */
  byte fwd_rates;        /* Forward link rates supported */
  byte rev_rates;        /* Reverse link rates supported */
} caii_mux_rec_var_type;

typedef struct
{
  caii_mux_rec_fix_type  fix;       /* Fixed part of record */
  caii_mux_rec_var_type  var[ 1 ];  /* Variable part. There may be an
                                       arbitrary number of these */
} caii_mux_rec_type;

/* Service configuration record type */
/* Same as defined for forward traffic channel (see above) */

#ifdef FEATURE_IS95B
/* Called Party Subaddress, Calling Party Subaddress, and Connected
   Subaddress (see IS-95B 6.7.4.19-21 ) use subaddress type defined
   for Forward Link */

/* Power Control Information (see IS-95B Section 6.7.4.22) */
typedef struct
{
  caii_rec_hdr_type hdr;    /* Header information */
  byte min_pwr_cntl_step;   /* Minimum power control step */
} caii_pwr_ctrl_rec_type;

/* IMSI_M record type (see IS-95A Section 6.7.4.23) */
/* IMSI_T record type (see IS-95A Section 6.7.4.24) */
/* Use type defined for IMSI or IMSI_O */

/* Capability info record (see IS-95B Section 6.7.4.25) */
typedef struct
{
  caii_rec_hdr_type hdr; /* Header information */
  boolean acc_entry_ho;    /* Access entry handoff supported */
  boolean acc_probe_ho;    /* Access probe handoff supported */
  boolean analog_search;   /* Analog search supported */
  boolean hop_beacon;      /* Hopping Beacon */
  boolean mahho;           /* Mobile Assisted HHO */
  boolean puf;             /* PUF */
  boolean analog_553;      /* TIA/EIA553 supported */
#ifdef FEATURE_IS2000
  boolean qpch;
  boolean slotted_timer;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean mac_cf_supported;
#endif
  boolean chs_supported;
  byte gating_rate_set;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean suspended_supported;
#endif
  boolean ext_cap_included;
  boolean mabo;
  boolean sdb;
  byte rlp_cap_blob_len;
  byte rlp_cap_blob[CAI_MAX_RLP_INFO_LEN];
#endif
} caii_capa_rec_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* FCH Fields ( see IS-2000 Section 2.7.4.27.1 )                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  boolean fch_frame_size;
  byte for_fch_len;
  word for_fch_rc_map;
  byte rev_fch_len;
  byte rev_fch_rc_map;
} caii_fch_spec_fields_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DCCH Fields ( see IS-2000 Section 2.7.4.27.2 )                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  byte dcch_frame_size;
  byte for_dcch_len;
  word for_dcch_rc_map;
  byte rev_dcch_len;
  byte rev_dcch_rc_map;
} caii_dcch_spec_fields_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* FOR SCH Fields ( see IS-2000 Section 2.7.4.27.3 )                       */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  byte for_sch_len;
  word for_sch_rc_map;
  byte for_sch_num;
  struct
  {
    boolean for_turbo_supported;
    byte for_max_turbo_block_size;
    boolean for_conv_supported;
    byte for_max_conv_block_size;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
    boolean for_frame_40_supported;
    byte for_frame_40_rate;
    boolean for_frame_80_supported;
    byte for_frame_80_rate;
#endif
  } for_sch_recs[CAI_MAX_NUM_FOR_SCH];
} caii_for_sch_spec_fields_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* REV SCH Fields ( see IS-2000 Section 2.7.4.27.4 )                       */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  byte rev_sch_len;
  byte rev_sch_rc_map;
  byte rev_sch_num;
  struct
  {
    boolean rev_turbo_supported;
    byte rev_max_turbo_block_size;
    boolean rev_conv_supported;
    byte rev_max_conv_block_size;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
    boolean rev_frame_40_supported;
    byte rev_frame_40_rate;
    boolean rev_frame_80_supported;
    byte rev_frame_80_rate;
#endif
  } rev_sch_recs[CAI_MAX_NUM_REV_SCH];
} caii_rev_sch_spec_fields_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Channel Config Capability Info (See IS2000 2.7.4.27)                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  caii_rec_hdr_type hdr;
  boolean otd_supported;
  boolean fch_supported;
  caii_fch_spec_fields_type fch_fields;
  boolean dcch_supported;
  caii_dcch_spec_fields_type dcch_fields;
  boolean for_sch_supported;
  caii_for_sch_spec_fields_type for_sch_fields;
  boolean rev_sch_supported;
  caii_rev_sch_spec_fields_type rev_sch_fields;
  boolean nonoctet_aligned_data;
  boolean octet_aligned_data;
} caii_chan_cfg_capa_info_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Extended Multiplex Option Info (See IS2000 2.7.4.28)                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  caii_rec_hdr_type hdr;
  byte num_mo_for_fch;
  struct
  {
    word mo_for_fch;
    byte for_rates_fch;
  } mo_for_fch_recs[CAI_MAX_MULT_OPT_FCH_DCCH];
  byte num_mo_rev_fch;
  struct
  {
    word mo_rev_fch;
    byte rev_rates_fch;
  } mo_rev_fch_recs[CAI_MAX_MULT_OPT_FCH_DCCH];
  byte num_mo_for_dcch;
  word mo_for_dcch[CAI_MAX_MULT_OPT_FCH_DCCH];
  byte num_mo_rev_dcch;
  word mo_rev_dcch[CAI_MAX_MULT_OPT_FCH_DCCH];
  byte num_mo_for_sch;
  struct
  {
    byte for_sch_id;      // Only 0 and 1 valid
    word mo_for_sch;
  } mo_for_sch_recs[CAI_MAX_MULT_OPT_SCH];
  byte num_mo_rev_sch;
  struct
  {
    byte rev_sch_id;      // Only 0 and 1 valid
    word mo_rev_sch;
  } mo_rev_sch_recs[CAI_MAX_MULT_OPT_SCH];
} caii_ext_mult_opt_info_type;

/* User Zone Update Request Record ( see IS-2000 Section 2.7.4.29 ) */
typedef struct
{
  caii_rec_hdr_type hdr;    /* Header information */
  word uzid;                /* User Zone Identifier */
} caii_user_zone_update_req_type;

#ifndef FEATURE_IS2000_2_1_ADDENDUM
/* Geo-location information ( IS-2000 addumdent section 2.7.4.29 ) */
typedef struct
{
  caii_rec_hdr_type hdr;
  byte geo_loc; /* geo location */
} caii_geo_loc_info_type;
#endif

#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                        SYNC CHANNEL MESSAGE                             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
{
  byte msg_type;        /* Message type */
  byte p_rev;           /* Protocol revision level */
  byte min_p_rev;       /* Minimum protocol revision level */
  word sid;             /* System identification */
  word nid;             /* Network identification */
  word pilot_pn;        /* Pilot PN sequence offset index */
  qword lc_state;       /* Long code state */
  qword sys_time;       /* System time */
  byte lp_sec;          /* Number of leap seconds that have occurred since
                           the start of System Time */
  byte ltm_off;         /* Offset of local time from System Time */
  boolean daylt;        /* Daylight savings time indicator */
  byte prat;            /* Paging channel data rate */
  word cdma_freq;       /* Frequency assignment */
#ifdef FEATURE_IS2000
  word ext_cdma_freq;   /* Extended Frequency Assignment */
#endif
} caii_sync_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                        PAGING CHANNEL MESSAGES                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GENERIC PAGING CHANNEL MESSAGE                                          */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_pc_hdr_type hdr;  /* Layer 2 header */
} caii_gen_pc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SYSTEM PARAMETERS MESSAGE (see IS-95A Section 7.7.2.3.2.1)             */

typedef struct
{
  byte msg_type;              /* Message type */
  word pilot_pn;              /* Pilot PN sequence offset index */
  byte config_msg_seq;        /* Configuration message sequence number */
  word sid;                   /* System identification */
  word nid;                   /* Network identification */
  word reg_zone;              /* Registration zone */
  byte total_zones;           /* Number of registration zones
                                 to be retained */
  byte zone_timer;            /* Zone timer length */
  boolean mult_sids;          /* Multiple SID storage indicator */
  boolean mult_nids;          /* Multiple NID storage indicator */
  word base_id;               /* Base station identification */
  byte base_class;            /* Base station class */
  byte page_chan;             /* Number of Paging Channels */
  byte max_slot_cycle_index;  /* Maximum slot cycle index */
  boolean home_reg;           /* Home registration indicator */
  boolean for_sid_reg;        /* SID roamer registration indicator */
  boolean for_nid_reg;        /* NID roamer registration indicator */
  boolean power_up_reg;       /* Power-up registration indicator */
  boolean power_down_reg;     /* Power-down registration indicator */
  boolean parameter_reg;      /* Parameter-change registration indicator */
  byte reg_prd;               /* Registration period */
  dword base_lat;             /* Base station latitude */
  dword base_long;            /* Base station longitude */
  word reg_dist;              /* Registration distance */
  byte srch_win_a;            /* Search window size for the Active Set and
                                 Candidate Set */
  byte srch_win_n;            /* Search window size for the Neighbor Set */
  byte srch_win_r;            /* Search window size for the Remaining Set */
  byte nghbr_max_age;         /* Neighbor Set maximum AGE */
  byte pwr_rep_thresh;        /* Power control reporting threshold */
  byte pwr_rep_frames;        /* Power control reporting frame count */
  boolean pwr_thresh_enable;  /* Threshold report mode indicator */
  boolean pwr_period_enable;  /* Threshold report mode indicator */
  byte pwr_rep_delay;         /* Power report delay */
  boolean rescan;             /* Rescan indicator */
  byte t_add;                 /* Pilot detection threshold */
  byte t_drop;                /* Pilot drop threshold */
  byte t_comp;                /* Active Set versus Candidate Set
                                 comparison threshold */
  byte t_tdrop;               /* Drop timer value */
  boolean ext_sys_parameter;  /* Extended System Parameters Msg indicator */
  boolean ext_nghbr_list;     /* Extended Neighbor List Message indicator */
/* FEATURE_IS95B */
  boolean gen_nghbr_list;     /* General Neighbor List Message indicator */
/* end of FEATURE_IS95B */
  boolean global_redirect;    /* Global Service Redirection Msg indicator */
#ifdef FEATURE_IS2000
  boolean is2000_incl;        /* is2000 fields included indicator */
  boolean pri_nghbr_list;     /* Private Neighbor List Message indicator */
  boolean user_zone_id;       /* User Zone Identification Message indicator */
  boolean ext_global_redirect; /* Extended Global Service Redirection
                                  Message indicator */
  boolean ext_chan_list;       /* Extended CDMA Channel List Message
                                  indicator */
#endif
} caii_sysparm_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ACCESS PARAMETERS MESSAGE (see IS-95A Section 7.7.2.3.2.2)             */

typedef struct
{
  byte msg_type;       /* Message type */
  word pilot_pn;       /* Pilot PN sequence offset index */
  byte acc_msg_seq;    /* Access parameters message sequence number */
  byte acc_chan;       /* Number of Access Channels */
  byte nom_pwr;        /* Nominal transmit power offset */
  byte init_pwr;       /* Initial power offset for access */
  byte pwr_step;       /* Power increment */
  byte num_step;       /* Number of access probes */
  byte max_cap_sz;     /* Maximum Access Channel message capsule size */
  byte pam_sz;         /* Access Channel preamble length */
  byte psist_0_9;      /* Persistence value for access overload classes
                          0 through 9 */
  byte psist_10;       /* Persistence value for Access Overload Class 10 */
  byte psist_11;       /* Persistence value for Access Overload Class 11 */
  byte psist_12;       /* Persistence value for Access Overload Class 12 */
  byte psist_13;       /* Persistence value for Access Overload Class 13 */
  byte psist_14;       /* Persistence value for Access Overload Class 14 */
  byte psist_15;       /* Persistence value for Access Overload Class 15 */
  byte msg_psist;      /* Persistence modifier for Access Channel attempts
                          for message transmissions */
  byte reg_psist;      /* Persistence modifier for Access Channel attempts
                          for registrations */
  byte probe_pn_ran;   /* Time randomization for Access Channel probes */
  byte acc_tmo;        /* Acknowledgement timeout */
  byte probe_bkoff;    /* Access Channel probe backoff range */
  byte bkoff;          /* Access Channel probe sequence backoff range */
  byte max_req_seq;    /* Maximum number of acces probe sequences for an
                          Access Channel request */
  byte max_rsp_seq;    /* Maximum number of access probe sequences for an
                          Access Channel response */
  byte auth;           /* Authentication mode */
  dword rand;          /* Random challenge value - only valid if auth
                          field is set to 1 */
  boolean nom_pwr_ext; /* Extended nominal transmit power */
} caii_accparm_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* EXTENDED NEIGHBOR LIST MESSAGE (see J-STD-008 Section 3.7.2.3.2.14)     */

typedef struct
{
  byte msg_type;         /* Message type */
  word pilot_pn;         /* Pilot PN sequence offset index */
  byte config_msg_seq;   /* Configuration message sequence number */
  byte pilot_inc;        /* Pilot PN sequence offset index increment */
  word num_nghbrs;       /* Number of neighbors listed in the nghbr array */
  struct
  {
    byte nghbr_config;        /* Neighbor configuration */
    word nghbr_pn;            /* Neighbor pilot PN sequence offset index */
    byte search_pri;          /* Pilot Channel search priority */
    boolean freq_incl;        /* Frequency included indicator */
    byte nghbr_band;          /* Neighbor band class */
    word nghbr_freq;          /* Neighbor frequency assignment */
  } nghbr[CAI_EXT_NGHBR_MAX];
} caii_ext_nghbr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* NEIGHBOR LIST MESSAGE (see IS-95A Section 7.7.2.3.2.3)                 */

typedef struct
{
  byte msg_type;         /* Message type */
  word pilot_pn;         /* Pilot PN sequence offset index */
  byte config_msg_seq;   /* Configuration message sequence number */
  byte pilot_inc;        /* Pilot PN sequence offset index increment */
  word num_nghbrs;       /* Number of neighbors listed in the nghbr array */
  struct
  {
    byte nghbr_config;        /* Neighbor configuration */
    word nghbr_pn;            /* Neighbor pilot PN sequence offset index */
  } nghbr[CAI_NGHBR_MAX];
} caii_nghbr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CDMA CHANNEL LIST MESSAGE (see IS-95A Section 7.7.2.3.2.4)             */

typedef struct
{
  byte msg_type;         /* Message type */
  word pilot_pn;         /* Pilot PN sequence offset index */
  byte config_msg_seq;   /* Configuration message sequence number */
  word num_freq;         /* Number of entries in the cdma_freq array */
  word cdma_freq[CAI_CHNLIST_MAX]; /* CDMA Channel frequency assignments */
} caii_chnlist_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Generic Page Body for Slotted Page Message and Page Message   */

typedef struct
{
  byte msg_seq;            /* Message sequence number */
  boolean ext_addr;        /* Extra address indicator */
  dword min1;              /* First part of mobile identification number */
  word min2;               /* Second part of mobile identification number */
  boolean special_service; /* Special service option indicator */
  word service_option;     /* Service option */
} caii_page_body_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SLOTTED PAGE MESSAGE (see IS-95A Section 7.7.2.3.2.5)                  */

typedef struct
{
  byte msg_type;          /* Message type */
  byte config_msg_seq;    /* Configuration message sequence number */
  byte acc_msg_seq;       /* Access parameters message sequence number */
  boolean more_pages;     /* More slotted pages to follow indicator */
  word num_pages;         /* Number of pages in the pages array */
  caii_page_body_type pages[CAI_SLT_PAGE_MAX];
} caii_slt_page_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PAGE MESSAGE (see IS-95A Section 7.7.2.3.2.6)                          */

typedef struct
{
  byte msg_type;          /* Message type */
  byte config_msg_seq;    /* Configuration message sequence number */
  byte acc_msg_seq;       /* Access parameters message sequence number */
  word num_pages;         /* Number of pages in the pages array */
  caii_page_body_type pages[CAI_PAGE_MAX];
} caii_page_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PAGING CHANNEL ORDER MESSAGE (IS-95A Sections 7.7.2.3.2.7 and 7.7.4)   */

/*  Order without order qualifier */
typedef struct
{
  caii_pc_hdr_type hdr;     /* Paging Channel common header fields */
  byte order;               /* Order code */
} caii_pc_gen_ord_type;

/*  Order With Order Qualifier type                                        */
typedef struct
{
  caii_pc_hdr_type hdr;     /* Paging Channel common header fields */
  byte order;               /* Order code */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;                /* Order qualification code */
} caii_pc_ordq_type;

/* Base Station Challenge Confirmation Order */
typedef struct
{
  caii_pc_hdr_type hdr;     /* Paging Channel common header fields */
  byte order;               /* Order code */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;                /* Order qualification code */
  dword authbs;             /* Challenge response */
} caii_pc_bs_chal_type;

#ifdef FEATURE_IS95B
/* Registration Accepted Order */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte order;            /* Order field */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;             /* Order qualification code */
  byte roam_indi;        /* Roaming Display Indication */
} caii_pc_reg_acc_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte order;            /* Order field */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;             /* Order qualification code */
  byte retry_type;
  byte retry_delay;
} caii_pc_retry_type;
#endif
#endif

/* Union of structures for the Paging Channel Order Message */
typedef union
{
  caii_pc_gen_ord_type  gen;
  caii_pc_ordq_type     ordq;
  caii_pc_bs_chal_type  chal;
#ifdef FEATURE_IS95B
  caii_pc_reg_acc_type  regacc;
#endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  caii_pc_retry_type    retry;
#endif
#endif
} caii_pc_ord_var_type;

typedef struct
{
  byte msg_type;         /* Message type */
  word num_ords;         /* Number of orders in the message */
  caii_pc_ord_var_type ords[CAI_PC_ORDER_MAX];  /* Array of orders */
} caii_pc_ord_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CHANNEL ASSIGNMENT MESSAGE (see IS-95A Section 7.7.2.3.2.8)             */

/* Base of Channel Assignment Message */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
} caii_chnasn_gen_type;

/* Channel Assignment Message with Assign mode = 000 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  boolean freq_incl;     /* Frequency included indicator */
  byte code_chan;        /* Code channel */
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
  byte frame_offset;     /* Frame offset */
  byte encrypt_mode;     /* Message encryption mode */
} caii_chnasn_am0_type;

/* Channel Assignment Message with Assign mode = 001 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  boolean respond;       /* Respond on new Access Channel indicator */
  boolean freq_incl;     /* Frequency included indicator */
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
  word num_pilots;       /* Number of entries in pilot_pn array */
  word pilot_pn[CAI_CHNASN_AM1_MAX]; /* Pilot PN sequence offset index */
} caii_chnasn_am1_type;

/* Channel Assignment Message with Assign mode = 010 */
typedef struct
{
  caii_pc_hdr_type hdr;   /* Paging Channel common header fields */
  byte assign_mode;       /* Assignment mode */
  boolean respond;        /* Respond on analog control channel indicator */
  byte analog_sys;        /* System indicator */
  boolean use_analog_sys; /* Use analog system indicator */
  byte band_class;        /* Band class */
} caii_chnasn_am2_type;

/* Channel Assignment Message with Assign mode = 011 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  word sid;              /* System identification of the analog system */
  byte vmac;             /* Voice mobile station attenuation code */
  word analog_chan;      /* Voice channel number */
  byte scc;              /* SAT color code */
  boolean mem;           /* Message encryption mode indiator */
  byte an_chan_type;     /* Analog voice channel type */
  byte dscc_msb;         /* DSCC most significant bit */
  byte band_class;       /* Band class */
} caii_chnasn_am3_type;

/* Channel Assignment Message with Assign mode = 100 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  boolean freq_incl;     /* Frequency included indicator */
  boolean bypass_alert_answer;  /* Bypass Indicator */
  byte default_config;   /* Default configuration */
  byte grant_mode;       /* Granted mode */
  byte code_chan;        /* Code channel */
  byte frame_offset;     /* Frame offset */
  byte encrypt_mode;     /* Message encryption mode */
  byte band;             /* Band class (only present if freq_incl
                            is set to 1) */
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
} caii_chnasn_am4_type;

/* Channel Assignment Message with Assign mode = 101 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  boolean respond;       /* Respond on new Access Channel indicator */
  boolean freq_incl;     /* Frequency included indicator */
  byte band;             /* Band class (only present if freq_incl
                            is set to 1) */
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
  word num_pilots;       /* Number of entries in pilot_pn array */
  word pilot_pn[CAI_CHNASN_AM5_MAX]; /* Pilot PN sequence offset index */
} caii_chnasn_am5_type;

/* Union of Channel Assignment messages */
typedef union
{
  caii_chnasn_gen_type gen;
  caii_chnasn_am0_type am0;
  caii_chnasn_am1_type am1;
  caii_chnasn_am2_type am2;
  caii_chnasn_am3_type am3;
  caii_chnasn_am4_type am4;
  caii_chnasn_am5_type am5;
} caii_ca_ord_type;

/* Channel Assignment message type */

typedef struct
{
  byte msg_type;     /* Message type */
  word num_chns;     /* Number of channel assignments in the chns array */
  caii_ca_ord_type chns[CAI_CHN_ASN_MAX];
} caii_chnasn_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT MESSAGE (see J-STD-008 Section 3.7.2.3.2.19)            */

typedef struct  /* TMSI Assignment Message data */
{
  byte zone_len;        /* TMSI zone length      */
  caii_tmsi_type info;  /* TMSI zone & TMSI code */
  dword exp_time;       /* TMSI expiration time  */
} caii_tmsi_data_type;

typedef struct  /* TMSI Assignment Message - Paging Channel */
{
  byte msg_type;             /* Message type */
  caii_pc_hdr_type hdr;      /* Paging Channel common header fields */
  caii_tmsi_data_type data;  /* TMSI Assignment Message data */
} caii_pc_tmsi_asgn_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DATA BURST MESSAGE (see IS-95A Section 7.7.2.3.2.9)                    */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte msg_number;       /* Message number */
  byte burst_type;       /* Data burst type */
  byte num_msgs;         /* Number of messages in the data burst stream */
  byte num_fields;       /* Number of characters in this message */
  byte chari[CAI_PC_BURST_MAX]; /* Characters */
} caii_pc_burst_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* AUTHENTICATION CHALLENGE MESSAGE (see IS-95A Section 7.7.2.3.2.10)     */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  dword randu;           /* Random challenge data */
} caii_pc_auth_ch_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SSD UPDATE MESAGE (see IS-95A Section 7.7.2.3.2.11)                    */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  qword randssd;         /* Random data for the computation of SSD */
} caii_pc_ssd_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* FEATURE NOTIFICATION MESSAGE (see IS-95A Section 7.7.2.3.2.12)         */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  boolean release;       /* Origination completion indicator */
  byte num_recs;         /* Number of information records in message */
  byte recs[CAII_MAX_INFO_REC_BYTES];
    /* Records - consecutive Paging Channel Information Records -
       use caii_rec_hdr type to parse different records */
} caii_pc_feature_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* EXTENDED SYSTEM PARAMETERS MESSAGE (see IS-95A Section 7.7.2.3.2.13)    */

typedef struct
{
  byte msg_type;             /* Message type */
  word pilot_pn;             /* Pilot PN sequence offset index */
  byte config_msg_seq;       /* Configuration message sequence number */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte pref_msid_type;       /* Preferred Access Channel MSID type */
  word mcc;                  /* Mobile country code */
  byte imsi_11_12;           /* 11th and 12th digits of the IMSI */
  byte tmsi_zone_len;        /* TMSI zone length */
  qword tmsi_zone;           /* TMSI zone */
  byte bcast_index;          /* Broadcast slot cycle index */
#ifndef FEATURE_MSG_PACK_UNIT_TEST
  boolean delete_for_tmsi;   /* Delete foreign TMSI */
#endif
#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  boolean imsi_t_supported;       /* IMSI_T_SUPPORTED flag */
  byte p_rev;                     /* P_REV supported by BS */
  byte min_p_rev;                 /* Minimum P_REV required for access */
  byte soft_slope;                /* Soft slop for soft handoff */
  byte add_intercept;             /* For IS-95B soft handoff */
  byte drop_intercept;            /* For IS-95B soft handoff */
  byte packet_zone_id;            /* Packet Data service zone identifier */
  byte max_num_alt_so;            /* Max. Num. of alternative service options */
  boolean reselect_included;      /* System Reselection parameters included */
  byte ec_thresh;                 /* Pilot Power Threshold */
  byte ec_io_thresh;              /* Pilot Ec/Io Threshold */
  boolean pilot_report;           /* Pilot reporting indicator */
  boolean nghbr_set_entry_info;   /* Access entry handoff info included */
  boolean acc_ent_ho_order;       /* Entry handoff permitted indicator */
  boolean nghbr_set_access_info;  /* Neighbor set access handoff included */
  boolean access_ho;              /* Access handoff permitted indicator */
  boolean access_ho_msg_rsp;      /* AHO permitted for msg response */
  boolean access_probe_ho;        /* APHO permitted indicator */
  boolean acc_ho_list_upd;        /* APHO List Update permitted indicator */
  boolean acc_probe_ho_other_msg; /* APHO for msg other than ORIG/PR */
  byte max_num_probe_ho;          /* Times limit for access probe handoff */
  byte nghbr_set_size;            /* Size of neighbor set */
  boolean access_entry_ho[MAX_NGHBR_SET_SIZE];   /* Entry handoff permitted */
  boolean access_ho_allowed[MAX_NGHBR_SET_SIZE]; /* AHO and APHO permitted */
#endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000
  boolean is2000_incl;            /* is2000 fields included indicator */
  boolean broadcast_gps_asst;     /* Broadcast GPS Assist indicator */
  boolean qpch_supported;         /* Quick Paging Channel Supported indicator */
  byte num_qpch;                  /* Number of Quick Paging Channels */
  boolean qpch_rate;              /* Quick Paging Channel Indicator rate */
  byte qpch_power_level_page;     /* Quick Paging Channel paging indicator
                                     transmit power level */
  boolean qpch_cci_supported;     /* Quick Paging Channel configuration change
                                     indicator supported */
  byte qpch_power_level_config;   /* Quick Paging Channel configuration change
                                      indicator transmit power level */
  boolean sdb_supported;          /* Short Data Burst supported indicator */
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean mac_cf_supported;       /* MAC control function supported indicator */
#endif
  byte rlgain_traffic_pilot;      /* Gain Adjustment of the Reverse Traffic
                                     Channel relative to the Reverse Pilot
                                     Channel for Radio Configurations greater
                                     than 2 */
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  boolean rev_pwr_cntl_delay_incl;
  byte rev_pwr_cntl_delay;
#endif
#endif
} caii_ext_sysparm_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* STATUS REQUEST MESSAGE (see IS-95-A TSB)                                */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte qual;             /* Qualification information */
  byte band;             /* Band class qualifier */
  byte op_mode;          /* Operating mode qualifier */
  byte num_fields;       /* Number of active elements in rec_typ */
#define         CAI_REC_TYP_MAX 15
  byte rec_typ[ CAI_REC_TYP_MAX ]; /* List of requested rec types */
} caii_pc_streq_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SERVICE REDIRECTION MESSAGE (see IS-95A Section 7.7.2.3.2.16)           */

/* Service Redirection Record Type = '00000001' */
/* Historically, we share the following structures in our code. However,
   in IS-95B, cai_redirect_rec1_type for Service Redirection and Global
   Service Redirection is slightly different. In SRM, 5 bits are reserved.
   In GSRM, the previously reserved 5 bits are defined as max_redirect_delay.
   To minimize impact to the Call Processing code, we will still share these
   structures, but the parser will take the responsibility of clearing
   the max_redirect_delay field for the SRM. */

typedef struct
{
  word expected_sid;       /* Expected SID */
  boolean ignore_cdma;     /* Ignore CDMA Available indicator */
  byte sys_ordering;       /* System ordering */
  byte max_redirect_delay; /* Max delay upon redirection. This field is
                              only for Global Service Redir, not for SRM */
} caii_redirect_rec1_type;

/* Service Redirection Record Type = '00000002' */

#define CAII_MAX_CDMA_CHANS 15
  /* Max number of CDMA chans is constrained by size of the num_chans field */

typedef struct
{
  byte band_class;         /* Band class */
  word expected_sid;       /* Expected SID */
  word expected_nid;       /* Expected NID */
  byte num_chans;          /* Number of CDMA channels */
  word cdma_chan[CAII_MAX_CDMA_CHANS];
} caii_redirect_rec2_type;

typedef struct
{
  word expected_aid;       /* Expected AID */
  boolean ignore_cdma;     /* Ignore CDMA available indicator */
} caii_redirect_rec4_type;

typedef struct
{
  byte msg_type;             /* Message type */
  caii_pc_hdr_type hdr;      /* Paging Channel common header fields */
  boolean return_if_fail;    /* Return if fail indicator */
  boolean delete_tmsi;       /* Delete TMSI indicator */
  /* FEATURE_IS95B */
  boolean redirect_type;     /* Redirect indicator for IS-95B */
  /* end of FEATURE_IS95B */
  byte record_type;          /* Redirection record type */
  byte record_len;           /* Redirection record length */
  union
  {
    caii_redirect_rec1_type rec1;
    caii_redirect_rec2_type rec2;
    caii_redirect_rec4_type rec4;
  } redir;
} caii_serv_redirect_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GENERAL PAGE MESSAGE (see IS-95A Section 7.7.2.3.2.17)                  */

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '00' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  qword imsi_s;             /* Last ten digits of the IMSI */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_0_sc_0_type;

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '01' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  byte imsi_11_12;          /* The 11th and 12th digits of the IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_0_sc_1_type;

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '10' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  word mcc;                 /* Mobile country code */
  qword imsi_s;             /* Last ten digits of the IMSI */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_0_sc_2_type;

/* PAGE_CLASS = '00' and PAGE_SUBCLASS = '11' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  word mcc;                 /* Mobile country code */
  byte imsi_11_12;          /* The 11th and 12th digits of the IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_0_sc_3_type;

/* PAGE_CLASS = '01' and PAGE_SUBCLASS = '00' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  byte imsi_addr_num;       /* Number of IMSI address digits */
  byte imsi_11_12;          /* The 11th and 12th digits of the IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_1_sc_0_type;

/* PAGE_CLASS = '01' and PAGE_SUBCLASS = '01' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  byte imsi_addr_num;       /* Number of IMSI address digits */
  word mcc;                 /* Mobile country code */
  byte imsi_11_12;          /* The 11th and 12th digits of the IMSI */
  qword imsi_s;             /* Last ten digits of the IMSI */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_1_sc_1_type;

/* PAGE_CLASS = '10' and PAGE_SUBCLASS = '00', '01', '10' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  dword tmsi_code_addr;     /* TMSI code */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_2_sc_012_type;

/* PAGE_CLASS = '10' and PAGE_SUBCLASS = '11' */
typedef struct
{
  byte msg_seq;             /* Message sequence number */
  byte tmsi_zone_len;       /* TMSI zone length      */
  caii_tmsi_type info;      /* TMSI zone & TMSI code */
  boolean special_service;  /* Special service option indicator */
  word service_option;      /* Service option (only present if
                               special_service field is set to 1) */
} caii_pc_2_sc_3_type;

/* PAGE_CLASS = '11' and PAGE_SUBCLASS = '01' */
typedef struct
{
  byte burst_type;                 /* Data burst type */
  byte addr_len;                   /* Address field length */
  byte bc_addr[CAII_BC_ADDR_MAX];  /* Broadcast address */
} caii_pc_3_sc_0_type;

typedef struct
{
  byte page_class;
  byte page_subclass;
  union
  {
    caii_pc_0_sc_0_type format0;
    caii_pc_0_sc_1_type format1;
    caii_pc_0_sc_2_type format2;
    caii_pc_0_sc_3_type format3;
    caii_pc_1_sc_0_type format4;
    caii_pc_1_sc_1_type format5;
    caii_pc_2_sc_012_type format8;
    caii_pc_2_sc_012_type format9;
    caii_pc_2_sc_012_type format10;
    caii_pc_2_sc_3_type   format11;
    caii_pc_3_sc_0_type format12;
  } rec;
} caii_gen_page_rec_type;

typedef struct
{
  byte msg_type;          /* Message type */
  byte config_msg_seq;    /* Configuration message sequence number */
  byte acc_msg_seq;       /* Access parameters message sequence number */
  boolean class_0_done;   /* Class 0 pages are done */
  boolean class_1_done;   /* Class 1 pages are done */
  boolean tmsi_done;      /* TMSI pages are done */
  boolean ordered_tmsis;  /* TMSIs sent in numerical order */
  boolean broadcast_done; /* Broadcast pages are done */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  word num_pages;         /* Number of pages in the pages array */
  caii_gen_page_rec_type gen_page[CAI_GEN_PAGE_MAX];
} caii_gen_page_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* GLOBAL SERVICE REDIRECTION MESSAGE (see IS-95A Section 7.7.2.3.2.18)    */

typedef struct
{
  byte msg_type;             /* Message type */
  word pilot_pn;             /* Pilot PN sequence offset index */
  byte config_msg_seq;       /* Configuration message sequence number */
  word redirect_accolc;      /* Redirected access overload classes */
  boolean return_if_fail;    /* Return if fail indicator */
  boolean delete_tmsi;       /* Delete TMSI indicator */
#ifdef FEATURE_IS2000
  boolean excl_p_rev_ms;     /* Exclude redirection indicator */
#endif /* FEATURE_IS2000 */
  byte record_type;          /* Redirection record type */
  byte record_len;           /* Redirection record length */
  union
  {
    caii_redirect_rec1_type rec1;
    caii_redirect_rec2_type rec2;
    caii_redirect_rec4_type rec4;
  } redir;
} caii_glob_redirect_type;

/* Pilot Record Type Definition */
typedef struct
{
  word pilot_pn;       /* Pilot PN offset index */
  boolean pwr_comb_ind;/* Power control symbol combining indicator */
  byte code_chan;      /* Code channel */
} caii_pilot_rec_type;

#ifdef FEATURE_IS2000
/* Additional Pilot Record Type Definition */
typedef struct
{
  byte otd_power_level;
} caii_add_pilot_rec0_type;

typedef union
{
  caii_add_pilot_rec0_type rec0;
} caii_add_pilot_rec_type;
#endif

#ifdef FEATURE_IS95B
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* PACA MESSAGE (see IS-95B Section 7.7.2.3.2.20)    */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte purpose;          /* Purpose of the PACA message */
  byte q_pos;            /* PACA Q position */
  byte paca_timeout;     /* PACA state timer duration */
} caii_paca_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* EXTENDED CHANNEL ASSIGNMENT MESSAGE (see IS-95B Section 7.7.2.3.2.21)             */

/* Base of Extended Channel Assignment Message */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
} caii_ext_chnasn_gen_type;

/* Extended Channel Assignment Message with Assign mode = 000 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  boolean freq_incl;     /* Frequency included indicator */
  byte default_config;   /* Default configuration */
  boolean bypass_alert_answer; /* Bypass indicator */
  byte num_pilots;       /* Number of pilot records following */
  byte granted_mode;     /* Granted mode */
  byte frame_offset;     /* Frame offset */
  byte encrypt_mode;     /* Message encryption mode */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
#ifndef FEATURE_MSG_PACK_UNIT_TEST
  byte band_class;       /* Band class */
#endif
#ifdef FEATURE_IS2000
  boolean is2000_incl;      /* is2000 fields included indicator */
  byte for_fch_rc;          /* Forward Fundamental Channel Radio Config */
  byte rev_fch_rc;          /* Reverse Fundamental Channel Radio Config */
  byte fpc_fch_init_setpt;  /* Initial Fundamental Channel Outer Loop
                               Eb/Nt setpoint */
  byte fpc_subchan_gain;    /* Forward Power Control Subchannel Rel Gain */
  byte rl_gain_adj;         /* Reverse Traffic Chan Power Relative to
                               access power */
  byte fpc_fch_fer;         /* Fundamental Channel Target Frame Err Rate */
  byte fpc_fch_min_setpt;   /* Min Fund Chan Outer Loop Eb/Nt setpoint */
  byte fpc_fch_max_setpt;   /* Max Fund Chan Outer Loop Eb/Nt setpoint */
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_fch_gating_mode;
  byte rev_pwr_cntl_delay_incl;
  byte rev_pwr_cntl_delay;
#endif
#endif
  caii_pilot_rec_type pilot_rec[CAI_EXT_CHNASN_AM0_MAX];
                         /* Active set pilot record */
} caii_ext_chnasn_am0_type;

/* Extended Channel Assignment Message with Assign mode = 001 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  boolean respond;       /* Respond on new Access Channel indicator */
  boolean freq_incl;     /* Frequency included indicator */
  byte band_class;       /* Band class */
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
  word num_pilots;       /* Number of entries in pilot_pn array */
  word pilot_pn[CAI_EXT_CHNASN_AM1_MAX]; /* Pilot PN sequence offset index */
} caii_ext_chnasn_am1_type;

/* Extended Channel Assignment Message with Assign mode = 010 */
typedef struct
{
  caii_pc_hdr_type hdr;   /* Paging Channel common header fields */
  byte assign_mode;       /* Assignment mode */
  boolean respond;        /* Respond on analog control channel indicator */
  byte analog_sys;        /* System indicator */
  boolean use_analog_sys; /* Use analog system indicator */
  byte band_class;        /* Band class */
} caii_ext_chnasn_am2_type;

/* Extended Channel Assignment Message with Assign mode = 011 */
typedef struct
{
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  word sid;              /* System identification of the analog system */
  byte vmac;             /* Voice mobile station attenuation code */
  word analog_chan;      /* Voice channel number */
  byte scc;              /* SAT color code */
  boolean mem;           /* Message encryption mode indiator */
  byte an_chan_type;     /* Analog voice channel type */
  byte dscc_msb;         /* DSCC most significant bit */
  byte band_class;       /* Band class */
} caii_ext_chnasn_am3_type;

#ifdef FEATURE_IS2000
typedef struct
{
  word pilot_pn;
  boolean add_pilot_rec_incl;
  byte pilot_rec_type;
  caii_add_pilot_rec_type add_pilot_rec;
  boolean pwr_comb_ind;
  word code_chan_fch;
  byte qof_mask_id_fch;
} caii_ext_ch_ind1_pilot_rec_type;

typedef struct
{
  byte fpc_fch_init_setpt;
  byte fpc_fch_fer;
  byte fpc_fch_min_setpt;
  byte fpc_fch_max_setpt;
  caii_ext_ch_ind1_pilot_rec_type pilot_rec[CAI_EXT_CHNASN_AM4_MAX];
} caii_ext_ch_ind1_type;

typedef struct
{
  word pilot_pn;
  boolean add_pilot_rec_incl;
  byte pilot_rec_type;
  caii_add_pilot_rec_type add_pilot_rec;
  boolean pwr_comb_ind;
  word code_chan_dcch;
  byte qof_mask_id_dcch;
} caii_ext_ch_ind2_pilot_rec_type;

typedef struct
{
  byte fpc_dcch_init_setpt;
  byte fpc_dcch_fer;
  byte fpc_dcch_min_setpt;
  byte fpc_dcch_max_setpt;
  caii_ext_ch_ind2_pilot_rec_type pilot_rec[CAI_EXT_CHNASN_AM4_MAX];
} caii_ext_ch_ind2_type;

typedef struct
{
  word pilot_pn;
  boolean add_pilot_rec_incl;
  byte pilot_rec_type;
  caii_add_pilot_rec_type add_pilot_rec;
  boolean pwr_comb_ind;
  word code_chan_fch;
  byte qof_mask_id_fch;
  word code_chan_dcch;
  byte qof_mask_id_dcch;
} caii_ext_ch_ind3_pilot_rec_type;

typedef struct
{
  byte fpc_fch_init_setpt;
  byte fpc_dcch_init_setpt;
  byte fpc_pri_chan;          // 0 - f-fch, 1 - f-dcch
  byte fpc_fch_fer;
  byte fpc_fch_min_setpt;
  byte fpc_fch_max_setpt;
  byte fpc_dcch_fer;
  byte fpc_dcch_min_setpt;
  byte fpc_dcch_max_setpt;
  caii_ext_ch_ind3_pilot_rec_type pilot_rec[CAI_EXT_CHNASN_AM4_MAX];
} caii_ext_ch_ind3_type;

/* Union of Extended Channel Assignment records */
typedef union
{
  caii_ext_ch_ind1_type ind1;
  caii_ext_ch_ind2_type ind2;
  caii_ext_ch_ind3_type ind3;
} caii_ext_ch_ind_type;

/* Extended Channel Assignment Message with Assign mode = 100 */
typedef struct
{
  caii_pc_hdr_type hdr;   /* Paging Channel common header fields */
  byte assign_mode;       /* Assignment mode */
  boolean freq_incl;
  byte band_class;
  word cdma_freq;
  boolean bypass_alert_answer;
  byte granted_mode;
  byte default_config;
  byte for_rc;
  byte rev_rc;
  byte frame_offset;
  byte encrypt_mode;
  byte fpc_subchan_gain;
  byte rl_gain_adj;
  byte num_pilots;
  byte ch_ind;
  caii_ext_ch_ind_type ch_rec;
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  byte rev_fch_gating_mode;
  byte rev_pwr_cntl_delay_incl;
  byte rev_pwr_cntl_delay;
#endif
} caii_ext_chnasn_am4_type;
#endif

/* Union of Extended Channel Assignment messages */
typedef union
{
  caii_ext_chnasn_gen_type gen;
  caii_ext_chnasn_am0_type am0;
  caii_ext_chnasn_am1_type am1;
  caii_ext_chnasn_am2_type am2;
  caii_ext_chnasn_am3_type am3;
#ifdef FEATURE_IS2000
  caii_ext_chnasn_am4_type am4;
#endif
} caii_ext_ca_ord_type;

#ifdef FEATURE_IS2000
#define CAII_MAX_INT_ECAM_SIZ   sizeof(caii_ext_chnasn_am4_type)
#else
#define CAII_MAX_INT_ECAM_SIZ   sizeof(caii_ext_chnasn_am1_type)
#endif

/* This is the maximum size possible for the internal format of a channel
   assignment record */

#define CAII_MAX_EXT_CHNASN_MSG_BUF_SIZ  ( CAI_EXT_CHN_ASN_MAX* \
                                         sizeof(caii_ext_chnasn_am2_type) \
                                         + CAII_MAX_INT_ECAM_SIZ )
/* Buffer for the packed internal format */


/* Extended Channel Assignment message type */
typedef struct
{
  byte msg_type;     /* Message type */
  word num_chns;     /* Number of channel assignments in the chns array */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  caii_ext_ca_ord_type* chns[CAI_EXT_CHN_ASN_MAX];
                     /* Pointer to the channel assignment record in the
                        following buffer */
#ifndef FEATURE_MSG_PACK_UNIT_TEST
  byte buf[CAII_MAX_EXT_CHNASN_MSG_BUF_SIZ];
                     /* Buffer to hold the packed internal format of
                        channel assignment records */
#endif
} caii_ext_chnasn_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* GENERAL NEIGHBOR LIST MESSAGE (see IS95B Section 3.7.2.3.2.22)          */

typedef struct
{
  byte band_class;
  byte sys_a_b;
} caii_analog_nghbr;

typedef struct
{
  byte msg_type;         /* Message type */
  word pilot_pn;         /* Pilot PN sequence offset index */
  byte config_msg_seq;   /* Configuration message sequence number */
  byte pilot_inc;        /* Pilot PN sequence offset index increment */
  byte nghbr_srch_mode;  /* Neighbor search mode */
  boolean nghbr_config_pn_incl; /* Neighbor configuration and PN offset
                                   included. */
  boolean freq_fields_incl;   /* Frequency fields included */
  boolean use_timing;         /* Use timing indicator */
  boolean global_timing_incl; /* Global timing included.  (Included if
                                 use_timing is set to 1) */
  byte global_tx_duration;    /* Global neighbor tranmit time duration
                                 (Included if global_timing_incl is included
                                  and set to 1). */
  byte global_tx_period;      /* Global neighbor transmit time period.
                                 (Included if global_timing_incl is included
                                  and set to 1). */
  byte num_nghbr;
  struct
  {
    byte nghbr_config;        /* Neighbor configuration (Included if
                                 nghbr_config_pn_incl is set to 1) */
    word nghbr_pn;            /* Neighbor pilot PN sequence offset index
                                 (Included if nghbr_config_pn_incl is set to
                                 1)*/
    byte search_pri;          /* Pilot Channel search priority */
    byte srch_win_nghbr;      /* Neighbor search window size */
    boolean freq_incl;        /* Frequency included indicator (Included if
                                 freq_fields_incl is set to 1.) */
    byte nghbr_band;          /* Neighbor band class */
    word nghbr_freq;          /* Neighbor frequency assignment */
    boolean timing_incl;      /* Timing included indicator (Included if
                                 use_timing is set to 1). */
    byte nghbr_tx_offset;     /* Neighbor transmit time offset. (Included
                                 if timing_incl is included and set to 1).*/
    byte nghbr_tx_duration;   /* Neighbor transit time duration.  (Included
                                 if timing_incl is included and set to 1,
                                 and global_timing_incl is set to 0) */
    byte nghbr_tx_period;     /* Neighbor transit time period.  (Included
                                 if timing_incl is included and set to 1,
                                 and global_timing_incl is set to 0). */
  } nghbr[CAI_GEN_NGHBR_MAX];
  byte num_analog_nghbr;      /* Number of neighboring analog systems
                                 (number listed in analog_nghbr array) */
  caii_analog_nghbr analog_nghbr[CAI_GEN_ANALOG_NGHBR_MAX];
#ifdef FEATURE_IS2000
  boolean is2000_incl;
  boolean srch_offset_incl;
  struct {
    boolean add_pilot_rec_incl;
    byte nghbr_pilot_rec_type;
    caii_add_pilot_rec_type add_pilot_rec;
    byte srch_offset_nghbr;
  } add_nghbr[CAI_GEN_NGHBR_MAX];
#endif
} caii_gen_nghbr_type;

typedef struct
{
  byte num_analog_nghbr;
  caii_analog_nghbr anl_nghbr[CAI_GEN_ANALOG_NGHBR_MAX];
} caii_analog_nghbr_type;


#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
/* Extended Global Service Redirection Message (EGSRM), IS2000, 3.7.2.3.2.27 */
/* Fixed Part of message */
typedef struct
{
  byte msg_type;        /* Message type */
  word pilot_pn;        /* Pilot PN sequence offset index */
  byte config_msg_seq;  /* Configuration message sequence number */
  word redirect_accolc; /* redirected access overload classes */
  boolean return_if_fail;  /* return if fail indicator */
  boolean delete_tmsi;     /* delete TMSI indicator */
  boolean redirect_p_rev_incl; /* redirection mobile protocol revision included */
  boolean excl_p_rev_ind;  /* excluding mobile protocol revision indicator */
  byte redirect_p_min;  /* minimum redirection protocol revision */
  byte redirect_p_max;       /* maximum direction protocol revision */
  byte record_type;     /* redirection record type */
  union
  {
    caii_redirect_rec1_type rec1;
    caii_redirect_rec2_type rec2;
  } redir;
} caii_ext_glb_redir_type;

/* Extended CDMA Channel List Message (ECCLM), IS2000, 3.7.2.3.2.28  */
/* Fixed part of message */
typedef struct
{
  byte msg_type;         /* Message type */
  word pilot_pn;         /* Pilot PN sequence offset index */
  byte config_msg_seq;   /* Configuration message sequence number */
  byte num_freq;         /* Number of entries in the cdma_freq array */
  word cdma_freq[CAI_EXT_CHNLIST_MAX];/* CDMA Channel frequency assignments */
  boolean rc_qpch_sel_incl; /* RC and QPCH selection included indicator */
  boolean rc_qpch_hash_ind[CAI_EXT_CHNLIST_MAX];/* RC_QPCH channel hashing indicator */
} caii_ext_chnlist_type;

#endif /* FEATURE_IS2000 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                      ACCESS CHANNEL MESSAGES                            */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PILOT REPORT ( see IS-95B Section 6.7.1.3.2 )                           */

typedef struct
{
  byte act_plt_str;       /* Active Pilot strength */
  boolean first_is_act;   /* Active Pilot is the first pilot on which an
                             access probe was sent */
  boolean first_is_pta;   /* The first pilo tis the previous to the active
                             pilot on which an access probe was sent */
  byte num_add_plt;       /* Number of additional reported pilots */
} caii_plt_rpt_fix_type;

typedef struct
{
  word plt_pn_phase;      /* Pilot measured phase */
  byte plt_strength;      /* Pilot strength */
  boolean acc_ho_en;      /* Access Handoff enable */
  boolean acc_attempted;  /* Access attempted flag */
} caii_plt_rpt_var_type;

typedef struct
{
  caii_plt_rpt_fix_type fix;
  caii_plt_rpt_var_type var[CAI_MAX_ADD_PILOTS];
} caii_plt_rpt_type;

#endif /* if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ACCESS CHANNEL MESSAGE HEADER                                           */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Layer 2 header */
} caii_gen_ac_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* REGISTRATION MESSAGE (see IS-95A Section 6.7.1.3.2.1)                  */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
  byte auth_mode;        /* Authentication mode */
  dword authr;           /* Authentication data - only present if
                            auth_mode == CAI_AUTH */
  byte randc;            /* Random challenge value - only present if
                            auth_mode == CAI_AUTH */
  byte count;            /* Call history parameter - only present if
                            auth_mode == CAI_AUTH */
  byte reg_type;         /* Registration type */
  byte slot_cycle_index; /* Slot cycle mode */
  byte mob_p_rev;        /* Protocol revision of the mobile station */
  byte scm;              /* Station class mark */
  boolean mob_term;      /* Mobile terminated calls accepted indicator */
  /* FEATURE_IS95B */
  byte return_cause;     /* Return Cause for IS-95B */
#ifndef FEATURE_IS2000
  byte p_rev_in_use;     /* According to which P_REV to pack the msg */
#else
  caii_plt_rpt_type pilot_rpt;
  boolean qpch_supported;
  boolean enhanced_rc;
  boolean uzid_incl;
  word uzid;
#endif
  /* end of FEATURE_IS95B */
} caii_reg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ACCESS CHANNEL ORDER MESSSAGE (see IS-95A Section 6.7.1.3.2.2)         */

/* Generic Order type */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
  byte order;            /* Order code */
} caii_ac_gen_ord_type;

/* Order Message with Order Qualifier */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
  byte order;            /* Order code */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;             /* Order qualification code */
} caii_ac_ordq_type;

/* Base Station Challenge Order */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
  byte order;            /* Order code */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;             /* Order qualification code */
  dword randbs;          /* Random challenge data */
} caii_ac_bs_chal_type;

/* Mobile Station Reject Orders */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
  byte order;            /* Order code */
#ifdef FEATURE_IS2000
  byte rejected_pdu_type;
#endif
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;             /* Order qualification code */
  byte rej_msg_type;     /* Message type of rejected message */
} caii_ac_rej_type;

typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
  byte order;            /* Order code */
#ifdef FEATURE_IS2000
  byte rejected_pdu_type;
#endif
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;             /* Order qualification code */
  byte rej_msg_type;     /* Message type of rejected message */
  byte rej_order;        /* Order type of rejected message */
  byte rej_ordq;         /* Order qualification code of rejected message */
} caii_ac_rej_ord_type;

typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
  byte order;            /* Order code */
#ifdef FEATURE_IS2000
  byte rejected_pdu_type;
#endif
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;             /* Order qualification code */
  byte rej_msg_type;     /* Message type of rejected message */
  byte rej_record;       /* Record type of the rejected info record */
} caii_ac_rej_rec_type;

/* Order Message */
typedef union
{
  caii_ac_gen_ord_type gen;
  caii_ac_ordq_type    ordq;
  caii_ac_bs_chal_type chal;
  caii_ac_rej_type     rej;
  caii_ac_rej_ord_type rej_ord;
  caii_ac_rej_rec_type rej_rec;
} caii_ac_ord_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT C0MPLETION MESSAGE (see J-STD-008 Section 2.7.1.3.2.8)  */

typedef struct  /* TMSI Assignment Completion Message - Access Channel */
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
  byte auth_mode;        /* Authentication mode */
  dword authr;           /* Authentication data - only present if
                            auth_mode == CAI_AUTH */
  byte randc;            /* Random challenge value - only present if
                            auth_mode == CAI_AUTH */
  byte count;            /* Call history parameter - only present if
                            auth_mode == CAI_AUTH */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
} caii_ac_tmsi_cmp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DATA BURST MESSAGE (see IS-95A Section 6.7.1.3.2.3)                    */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;  /* Access Channel common header fields */
  byte auth_mode;        /* Authentication mode */
  dword authr;           /* Authentication data - only present if
                            auth_mode == CAI_AUTH */
  byte randc;            /* Random challenge value - only present if
                            auth_mode == CAI_AUTH */
  byte count;            /* Call history parameter - only present if
                            auth_mode == CAI_AUTH */
  byte msg_number;       /* Message number within the data burst stream */
  byte burst_type;       /* Data burst type */
  byte num_msgs;         /* Number of messages in the data burst stream */
  byte num_fields;       /* Number of characters in this message */
  byte chari[CAI_AC_BURST_MAX];  /* Characters */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
} caii_ac_burst_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ORIGINATION MESSAGE (see IS-95A Section 6.7.1.3.2.4)                   */

typedef struct
{
  word alt_so;              /* Alternative service option */
} caii_alt_so_type;

typedef struct
{
  byte msg_type;            /* Message type */
  caii_ac_hdr_type hdr;     /* Access Channel common header fields */
  byte auth_mode;           /* Authentication mode */
  dword authr;              /* Authentication data - only present if
                               auth_mode == CAI_AUTH */
  byte randc;               /* Random challenge data - only present if
                               auth_mode == CAI_AUTH */
  byte count;               /* Call history parameter - only present if
                               auth_mode == CAI_AUTH */
  boolean mob_term;         /* Mobile terminated calls accepted indicator */
  byte slot_cycle_index;    /* Slot cycle index */
  byte mob_p_rev;           /* Protocol revision of the mobile station */
  byte scm;                 /* Station class mark */
  byte request_mode;        /* Requested mode code */
  boolean special_service;  /* Speical service option indicator */
  word service_option;      /* Requested service option for this origination
                               (only present if special_service field is
                                set to 1) */
  boolean pm;               /* Privacy mode indicator */
  boolean digit_mode;       /* Digit mode indicator */
  byte number_type;         /* Type of number (only present if digit_mode
                               field is set to 1) */
  byte number_plan;         /* Numbering plan (only present if digit_mode
                               field is set to 1) */
  boolean more_fields;      /* More dialed digits indicator */
  byte num_fields;          /* Number of dialed digits in the message */
  byte chari[CAI_AC_ORIG_MAX];  /* Dialed digits */
  boolean nar_an_cap;       /* Narrow analog capability */
  boolean paca_reorig;      /* PACA reorigination for IS-95B */
  byte return_cause;        /* Return cause for IS-95B */
  boolean more_records;     /* More records indicator */
  byte encry_sup;           /* Encryption Supported */
  boolean paca_sup;         /* PACA supported field */
  byte num_alt_so;          /* Number of alternative SO */
  word alt_so[CAI_AC_ALT_SO_MAX];
                            /* Alternative service options */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
  boolean drs;
  boolean uzid_incl;
  word uzid;
  byte ch_ind;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean blob_incl;
  byte blob;
  boolean new_so_ind;
#else
  byte sr_id;
#endif
  boolean otd_supported;
  boolean qpch_supported;
  boolean enhanced_rc;
  byte for_rc_pref;
  byte rev_rc_pref;
  boolean fch_supported;
  caii_fch_spec_fields_type fch_fields;
  boolean dcch_supported;
  caii_dcch_spec_fields_type dcch_fields;
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  boolean rev_fch_gating_req;
#endif
#else
  byte p_rev_in_use;        /* P_REV of this message, IMPORTANT! */
#endif
} caii_orig_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PAGE RESPONSE MESSAGE  (see IS-95A Section 6.7.1.3.2.5)                */

typedef struct
{
  byte msg_type;            /* Message type */
  caii_ac_hdr_type hdr;     /* Access Channel common header fields */
  byte auth_mode;           /* Authentication mode */
  dword authr;              /* Authentication data - only present if
                               auth_mode == CAI_AUTH */
  byte randc;               /* Random challenge data - only present if
                               auth_mode == CAI_AUTH */
  byte count;               /* Call history parameter - only present if
                               auth_mode == CAI_AUTH */
  boolean mob_term;         /* Mobile terminated calls accepted indicator */
  byte slot_cycle_index;    /* Slot cycle index */
  byte mob_p_rev;           /* Protocol revision of the mobile station */
  byte scm;                 /* Station class mark */
  byte request_mode;        /* Requested mode code */
  word service_option;      /* Service option */
  boolean pm;               /* Privacy mode indicator */
  byte nar_an_cap;          /* Narrow analog capability */
  byte encry_sup;           /* Encryption Supported for IS-95B */
  byte num_alt_so;          /* Number of alternative SO */
  word alt_so[CAI_AC_ALT_SO_MAX];
                            /* Alternative service options */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
  boolean uzid_incl;
  word uzid;
  byte ch_ind;
  boolean otd_supported;
  boolean qpch_supported;
  boolean enhanced_rc;
  byte for_rc_pref;
  byte rev_rc_pref;
  boolean fch_supported;
  caii_fch_spec_fields_type fch_fields;
  boolean dcch_supported;
  caii_dcch_spec_fields_type dcch_fields;
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  boolean rev_fch_gating_req;
#endif
#else
  byte p_rev_in_use;        /* P_REV of this message, IMPORTANT! */
#endif
} caii_page_resp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* AUTHENTICATION CHALLENGE RESPONSE MSG (see IS-95A Section 6.7.1.3.2.6) */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_ac_hdr_type hdr;     /* Access Channel common header fields */
  dword authu;           /* Authentication challenge response */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
} caii_auth_ch_resp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* STATUS RESPONSE MESSAGE (see IS-95-A TSB)                               */

typedef struct
{
  byte msg_type;            /* Message type */
  caii_ac_hdr_type hdr;     /* Access Channel common header fields */
#ifndef FEATURE_MSG_PACK_UNIT_TEST
  caii_msid0_type msid;     /* Mobile station identifier fields */
#endif
  byte qual;                /* Qualification information type */
  byte band;                /* Band class qualifier */
  byte op_mode;             /* Operating mode qualifier */
  byte num_recs;            /* Number of information records */
  byte recs[CAII_MAX_INFO_REC_BYTES];
    /* Records - consecutive Information Records -
       use caii_rec_hdr type to parse different records */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#ifndef FEATURE_MSG_PACK_UNIT_TEST
  word total_rec_len; /* IS2000 Addition */
  /* This field is used for holding the total record length of all records
     and this is computed as and when records are added to this data struct, This
     is used to compute how many additional pilots can be included with this */
#endif
#endif
} caii_ac_strsp_type;

#ifdef FEATURE_IS95B
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PACA CANCEL MESSAGE (see IS-95B Section 6.7.1.3.2.9)                    */

typedef struct
{
  byte msg_type;            /* Message type */
  caii_ac_hdr_type hdr;     /* Access Channel common header fields */
  byte auth_mode;           /* Authentication mode */
  dword authr;              /* Authentication data - only present if
                               auth_mode == CAI_AUTH */
  byte randc;               /* Random challenge data - only present if
                               auth_mode == CAI_AUTH */
  byte count;               /* Call history parameter - only present if
                               auth_mode == CAI_AUTH */
#ifdef FEATURE_IS2000
  caii_plt_rpt_type pilot_rpt;
#endif
} caii_ac_paca_can_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* EXTENDED STATUS RESPONSE MESSAGE (see IS-95B Section 6.7.1.3.2.10)      */

typedef caii_ac_strsp_type caii_ac_ext_strsp_type;

#endif /* FEATURE_IS95B */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                  FORWARD TRAFFIC CHANNEL MESSAGES                       */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* FORWARD TRAFFIC CHANNEL MESSAGE HEADER                                  */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
} caii_gen_tc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* FORWARD TRAFFIC CHANNEL ORDER MESSAGE (see IS-95A Section 7.7.3.3.2.1) */

/* Generic Order Message */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  boolean use_time;     /* Indicator of whether the action_time field should
                           be used */
  byte action_time;     /* Time at which order should be executed */
  byte order;           /* Order field */
} caii_ftc_gen_ord_type;

/* Order Message with Order Qualifier */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  boolean use_time;     /* Use action time indicator */
  byte action_time;     /* Action time */
  byte order;           /* Order field */
  byte ordq;            /* Order qualification field */
} caii_ftc_ordq_type;

/* Base Station Challenge Confirmation Order (see IS-95A Section 7.7.4.1) */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  boolean use_time;     /* Use action time indicator */
  byte action_time;     /* Action time */
  byte order;           /* Order field */
  byte ordq;            /* Order qualification field */
  dword authbs;         /* Challenge response */
} caii_ftc_chal_type;

/* Service Option Request Order (see IS-95A Section 7.7.4.2) */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  boolean use_time;     /* Use action time indicator */
  byte action_time;     /* Action time */
  byte order;           /* Order field */
  byte ordq;            /* Order qualification field */
  word service_option;  /* Service Option */
} caii_ftc_so_req_type;

/* Service Option Response Order (see IS-95A Section 7.7.4.3) */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  boolean use_time;     /* Use action time indicator */
  byte action_time;     /* Action time */
  byte order;           /* Order field */
  byte ordq;            /* Order qualification field */
  word service_option;  /* Service Option */
} caii_ftc_so_res_type;

/* Status Request Order (see IS-95A Section 7.7.4.4) */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  boolean use_time;     /* Use action time indicator */
  byte action_time;     /* Action time */
  byte order;           /* Order field */
  byte ordq;            /* Order qualification field */
  word sid;             /* System identification number - only present if
                           ordq == CAI_ID_REQ */
  word nid;             /* Network identification number - only present if
                           ordq == CAI_ID_REQ */
} caii_ftc_stat_req_type;

#ifdef FEATURE_IS95B
/* Periodic Pilot Measurement Order (see IS-95B Section 7.7.4.6) */
typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  byte use_time;               /* Use action time indicator */
  byte action_time;            /* Action time */
  byte order;                  /* Order field */
  byte ordq;                   /* Order qualification code */
  byte min_pilot_pwr_thresh;   /* Threshold of Ec of pilots
                                     in Active set         */
  byte min_pilot_ec_io_thresh; /* Pilot Strength Threshold of
                                     Serving Frequency     */
} caii_ftc_ppm_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  byte use_time;               /* Use action time indicator */
  byte action_time;            /* Action time */
  byte order;            /* Order field */
  byte ordq;             /* Order qualification code */
  byte retry_type;
  byte retry_delay;
} caii_ftc_retry_type;
#endif
#endif

/* Order Message */
typedef union
{
  caii_ftc_gen_ord_type  gen;
  caii_ftc_ordq_type     ordq;
  caii_ftc_chal_type     chal;
  caii_ftc_so_req_type   so_req;
  caii_ftc_so_res_type   so_res;
  caii_ftc_stat_req_type stat_req;
#ifdef FEATURE_IS95B
  caii_ftc_ppm_type      ppm;
#endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000
#ifndef FEATURE_IS2000_REV0_PREADDENDUM
  caii_ftc_retry_type    retry;
#endif
#endif
} caii_ftc_ord_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* AUTHENTICATION CHALLENGE MESSAGE (see IS-95A Section 7.7.3.3.2.2)      */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  dword randu;           /* Random challenge data */
} caii_auth_ch_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ALERT WITH INFORMATION MESSAGE (see IS-95A Section 7.7.3.3.2.3)        */

typedef struct
{
  byte msg_type;              /* Message type */
  caii_tc_hdr_type hdr;       /* Layer 2 header */
  byte num_recs;              /* Number of records in message */
  byte recs[CAII_MAX_INFO_REC_BYTES];
    /* Records - consecutive Forward Traffic Channel Information Records -
       use caii_rec_hdr type to parse different records */
} caii_alert_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT MESSAGE (see J-STD-008 Section 3.7.3.3.2.22)            */

typedef struct  /* TMSI Assignment Message - Forward Traffic Channel */
{
  byte msg_type;              /* Message type */
  caii_tc_hdr_type hdr;       /* Layer 2 header */
  caii_tmsi_data_type data;   /* TMSI Assignment Message data */
} caii_ftc_tmsi_asgn_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DATA BURST MESSAGE (see IS-95A Section 7.7.3.3.2.4)                    */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte msg_number;       /* Number of message in the data stream burst */
  byte burst_type;       /* Type of data burst */
  byte num_msgs;         /* Number of messages in the data stream burst */
  byte num_fields;       /* Number of characters in chari field */
  byte chari[CAI_FWD_TC_BURST_MAX];
} caii_fwd_tc_burst_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ANALOG HANDOFF DIRECTION MESSAGE (see IS-95A Section 7.7.3.3.2.6)      */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  boolean use_time;      /* Indicator of whether the action_time field should
                            be used */
  byte action_time;      /* Time at which order should be executed */
  word sid;              /* System ID of analog system */
  byte vmac;             /* Voice mobile station attenuation code */
  word analog_chan;      /* Analog voice channel number */
  byte scc;              /* SAT color code */
  boolean mem;           /* Message encryption mode indicator */
  byte an_chan_type;     /* Analog voice channel type */
  byte dscc_msb;         /* DSCC most significant bit */
  byte band_class;       /* Band class for IS-95B */
#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */
} caii_fm_ho_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* IN-TRAFFIC SYSTEM PARAMETER MESSAGE (see IS-95A Section 7.7.3.3.2.7)   */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  word sid;              /* System identification */
  word nid;              /* Network identification */
  byte srch_win_a;       /* Characteristic field for Active Pilot Set */
  byte srch_win_n;       /* Characteristic field for Neighbor Pilot Set */
  byte srch_win_r;       /* Characteristic field for the Remaining Pilot set */
  byte t_add;            /* Pilot detection threshold */
  byte t_drop;           /* Pilot drop threshold */
  byte t_comp;           /* Active Set vs Candidate set comparsion
                            threshold */
  byte t_tdrop;          /* Drop timer value */
  byte nghbr_max_age;    /* Neighbor set member maximum age for retention */
#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte p_rev;            /* Protocol Revision Level */
  byte soft_slope;       /* Slope in criterion to add/drop active set pilots*/
  byte add_intercept;    /* Intercept in criterion to add to active set */
  byte drop_intercept;   /* Intercept in criterion for dropping pilot */
  byte packet_zone_id;   /* Packet data services zone identifier */
  boolean extension;     /* Indicator that extension fields are present */
  byte t_mulchan;        /* Supplemental Channel Request Message pilot
                          * strength reporting offset. */
  byte begin_preamble;   /* Number of preamble frames at beginning */
  byte resume_preamble;  /* Number of preamble frames at resumption */
#endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000
  boolean t_slotted_incl;
  byte t_slotted;
#endif
} caii_tc_sysparm_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* NEIGHBOR LIST UPDATE MESSAGE (see IS-95A Section 7.7.3.3.2.8)          */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte pilot_inc;        /* Pilot Index Increment value */
  word num_nghbr;        /* Number of entries in the nghbr array */
  struct
  {
    word nghbr_pn;         /* Offset of neighbor's PN pilot offset relative
                              to system time */
  } nghbr[CAI_NLU_MAX];
} caii_nlu_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SEND BURST DTMF MESSAGE (see IS-95A Section 7.7.3.3.2.9)               */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte num_digits;       /* Number of DTMF digits in message */
  byte dtmf_on_length;   /* DTMF pulse width code */
  byte dtmf_off_length;  /* DTMF interdigit interval code */
  byte digit[CAI_SBDTMF_MAX];  /* Array of 4 bit representation of digits */
} caii_sbdtmf_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* POWER CONTROL PARAMETERS MESSAGE (see IS-95A Section 7.7.3.3.2.10)     */

typedef struct
{
  byte msg_type;              /* Message type */
  caii_tc_hdr_type hdr;       /* Layer 2 header */
  byte pwr_rep_thresh;        /* Power control reporting threshold */
  byte pwr_rep_frames;        /* Power control reporting frame count */
  boolean pwr_thresh_enable;  /* Threshold report mode indicator */
  boolean pwr_period_enable;  /* Threshold report mode indicator */
  byte pwr_rep_delay;         /* Power Measurement report delay */
} caii_pwr_ctl_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* RETRIEVE PARAMETERS MESSAGE (see IS-95A Section 7.7.3.3.2.11)          */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  word num_parms;        /* Number of parameters in parameter_id array */
  word parameter_id[CAI_RET_PARMS_MAX];  /* Array of Parameter ids */
} caii_ret_parms_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SET PARAMETERS MESSAGE (see IS-95A Section 7.7.3.3.2.12)              */

typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  word num_parms;              /* Number of parameters in parms array */
  struct
  {
    word parameter_id;   /* Parameter identification */
    word parameter_len;  /* length of parameter */
    union
    {
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#else
      byte parm_b;       /* byte length parameter */
      word parm_w;       /* word length parameter */
      dword parm_d;      /* dword length parameter */
      qword parm_q;      /* qword length parameter */
#endif
    } parameter;
  } parms[CAI_SET_PARM_MAX];  /* Array of parameters */
} caii_set_parm_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SSD UPDATE MESSAGE (see IS-95A Section 7.7.3.3.2.13)                   */

typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  qword randssd;               /* Random data */
} caii_ssd_up_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* FLASH WITH INFORMATION MESSAGE (See IS-95A Section 7.7.3.3.2.14)       */

/* Use caii_alert_type from Alert With Information record type
   definition above */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  MOBILE STATION REGISTERED MESSAGE (see IS-95A Section 7.7.3.3.2.15)   */

typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  word sid;                    /* System identification */
  word nid;                    /* Netword identification */
  word reg_zone;               /* Registration zone */
  byte total_zones;            /* Number of registration zones to be
                                  retained */
  byte zone_timer;             /* Zone timer length */
  boolean mult_sids;           /* Multiple SID storage indicator */
  boolean mult_nids;           /* Multiple NID storage indicator */
  dword base_lat;              /* Base station latitude */
  dword base_long;             /* Base Station longitude */
  word reg_dist;               /* Registration distance */
} caii_registered_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  STATUS REQUEST MESSAGE (see IS-95-A TSB)                               */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte qual;             /* Qualification information */
  byte band;             /* Band class qualifier */
  byte op_mode;          /* Operating mode qualifier */
  byte num_fields;       /* Number of active elements in rec_typ */
  byte rec_typ[ CAI_REC_TYP_MAX ]; /* List of requested rec types */
} caii_tc_streq_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE REQUEST MESSAGE (See IS-95-A TSB)                              */

typedef struct
{
  byte  msg_type;          /* Message type */
  caii_tc_hdr_type  hdr;   /* Layer 2 header */
  byte  req_seq;           /* Request sequence number */
  byte  purp;              /* Purpose code */
  caii_srv_cfg_type  cfg;  /* Service configuration record */
} caii_ftc_srv_req_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE RESPONSE MESSAGE (See IS-95-A TSB)                             */

typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  byte req_seq;
  byte purp;
  caii_srv_cfg_type cfg;
} caii_ftc_srv_rsp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE CONNECT MESSAGE (See IS-95-A TSB)                              */

typedef struct
{
  byte  msg_type;          /* Message type */
  caii_tc_hdr_type  hdr;   /* Layer 2 header */
  boolean  use_time;       /* Use action time indicator */
  byte  action_time;       /* Action time */
  byte  con_seq;           /* Connect sequence number */
  caii_srv_cfg_type  cfg;  /* Service configuration record */
#ifdef FEATURE_IS2000
  caii_non_neg_srv_cfg_type  non_neg_cfg;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean rc_sync_id_incl;
  byte rc_sync_id;
  boolean blob_incl;
  byte blob;
#endif
#endif
} caii_srv_con_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE OPTION CONTROL MESSAGE (See IS-95-A TSB)                       */

typedef struct
{
  byte  msg_type;                    /* Message type */
  caii_tc_hdr_type  hdr;             /* Layer 2 header */
  boolean  use_time;                 /* Use action time indicator */
  byte  action_time;                 /* Action time */
  byte  con_ref;                     /* Service option connection ref */
  word  so;                          /* Service option number */
  byte  tsf_len;                     /* Type specific fields length */
  byte  tsf[ CAI_FTC_SRV_CTL_MAX ];  /* Type specific fields */
} caii_ftc_srv_ctl_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SUPPLEMENTAL CHANNEL ASSIGNMENT MESSAGE(see IS-95A Section 7.7.3.3.2.24)*/
#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
/* This message is not used in special MDR but the following definition is */
/* used, so included it as part of special MDR.                            */
typedef struct
{
  word plt_pn;                 /* Pilot PN */
  boolean expl_code_chan;      /* Explicit Code Channel indicator */
  union
  {
    byte sup_code_chan[CAI_MAX_NUM_SUP];
                                  /* Supplemental conde channel index */
    byte base_code_chan;          /* Base code channel index */
  } chan_lst;
} caii_scam_var_type;
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

#ifdef FEATURE_IS95B
typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  boolean use_retry_delay;     /* Assign or retry indicator */
  byte retry_delay;            /* SCRM retry delay */
  boolean rev_included;        /* Reverse Supplemental Code Channel config */
  byte rev_dtx_duration;       /* Reverse Discontinuous Transmission Duration*/
  boolean expl_rev_start_tm;   /* Explicit reverse supp code chan assign
                                  start time */
  byte rev_start_tm;           /* Explicit start time */
  boolean use_rev_dur;         /* Use reverse duration indicator */
  byte rev_dur;                /* Reverse duration */
  boolean use_rev_hdm_seq;     /* Use reverse GHDM sequence # indicator */
  byte rev_hdm_seq;            /* Sequence # of the reverse linked GHDM */
  byte num_rev_codes;          /* Number of reverse sup code channels */
  boolean use_t_add_abt;       /* Reverse use T_ADD abort indicator */
  boolean use_scrm_seq;        /* Use SCRM sequence # indicator */
  byte scrm_seq_num;           /* SCRM sequence number */
  boolean rev_parms_inc;       /* Reverse additional parameter included */
  byte t_mulchan;              /* SCRM pilot strength reporting offset */
  byte begin_pre;              /* Number of preamble frames */
  byte resume_pre;             /* Number of preamble at resumption of xmit */
  boolean for_inc;             /* FWD supp channel config included indicator */
  byte for_sup_config;         /* FWD supp channel config indicator */
  boolean expl_for_start_tm;   /* Explicit forward start time indicator */
  byte for_start_tm;           /* Forward start time */
  boolean use_for_dur;         /* Use forward duration indicator */
  byte for_dur;                /* Forward duration */
  boolean use_for_hdm_seq;     /* User forward GHDM sequence # indicator */
  byte for_hdm_seq;            /* Sequence # of GHDM */
  byte num_sup_plt;            /* Number of pilots in the active set which
                                  xmitting the sup chan */
  byte num_sup;                /* Number of FWD supp code channel */

} caii_scam_fix_type;

typedef struct
{
  caii_scam_fix_type fix;
  caii_scam_var_type var[CAI_MAX_SUP_PILOTS];
} caii_scam_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  SERVICE REDIRECTION MESSAGE (see IS-95A Section 7.7.3.3.2.23)          */
typedef struct
{
  byte msg_type;             /* Message type */
  caii_tc_hdr_type hdr;      /* Layer 2 header */
  boolean return_if_fail;    /* Return if fail indicator */
  boolean delete_tmsi;       /* Delete TMSI indicator */
  byte redirect_type;        /* Redirect indicator for IS-95B */
  byte record_type;          /* Redirection record type */
  byte record_len;           /* Redirection record length */
  union
  {
    caii_redirect_rec1_type rec1;
    caii_redirect_rec2_type rec2;
    caii_redirect_rec4_type rec4;
  } redir;
} caii_ftc_srv_redir_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  POWER CONTROL MESSAGE (see IS-95A Section 7.7.3.3.2.25)                */

#ifdef FEATURE_IS2000
typedef struct
{
  byte sch_id;            // SCH can be 0 or 1
  byte fpc_sch_fer;
  /*   byte fpc_sch_init_setpt; */
  byte fpc_sch_min_setpt;
  byte fpc_sch_max_setpt;
} caii_fpc_sup_chn_type;

#ifdef FEATURE_PLT
#error code not present
#else
typedef struct
#endif
{
  boolean fch_incl;
  byte fch_chan_adj_gain;
  boolean dcch_incl;
  byte dcch_chan_adj_gain;
  boolean sch0_incl;
  byte sch0_chan_adj_gain;
  boolean sch1_incl;
  byte sch1_chan_adj_gain;
} caii_rpc_adj0_type;

#ifdef FEATURE_PLT
#error code not present
#else
typedef struct
#endif
{
  boolean rl_att_adj_gain_type;
  boolean rs1_20ms_incl;
  byte rl_att_adj_gain_1500;
  byte rl_att_adj_gain_2700;
  byte rl_att_adj_gain_4800;
  byte rl_att_adj_gain_9600;
  boolean rs2_20ms_incl;
  byte rl_att_adj_gain_1800;
  byte rl_att_adj_gain_3600;
  byte rl_att_adj_gain_7200;
  byte rl_att_adj_gain_14400;
  boolean five_ms_incl;
  byte norm_att_gain_9600_5ms;
} caii_rpc_adj1_type;

#ifdef FEATURE_PLT
#error code not present
#else
typedef struct
#endif
{
  byte code_type;               // 0 - convolutional, 1 - turbo
  byte rl_att_adj_gain_type;    // 0 - nominal gain attribute adjustment, 1 - pilot reference
  boolean rs1_20ms_incl;
  byte rl_att_adj_gain_19200;
  byte rl_att_adj_gain_38400;
  byte rl_att_adj_gain_76800;
  byte rl_att_adj_gain_153600;
  byte rl_att_adj_gain_307200;
  byte rl_att_adj_gain_614400;
  boolean rs2_20ms_incl;
  byte rl_att_adj_gain_28800;
  byte rl_att_adj_gain_57600;
  byte rl_att_adj_gain_115200;
  byte rl_att_adj_gain_230400;
  byte rl_att_adj_gain_460800;
  byte rl_att_adj_gain_1036800;
} caii_rpc_adj2_type;

#ifdef FEATURE_PLT
#error code not present
#else
typedef union
#endif
{
  caii_rpc_adj0_type adj0;
  caii_rpc_adj1_type adj1;
  caii_rpc_adj2_type adj2;
} caii_rpc_adj_type;

#ifdef FEATURE_PLT
#error code not present
#else
typedef struct
#endif
{
  byte rpc_adj_rec_type;
  caii_rpc_adj_type rpc_adj;
} caii_rpc_type;
#endif

typedef struct
{
  byte msg_type;               /* Message type */
  caii_tc_hdr_type hdr;        /* Layer 2 header */
  byte pwr_cntl_step;          /* Assign or retry indicator */
#ifdef FEATURE_IS2000
  boolean use_time;
  byte action_time;
  boolean fpc_incl;
  byte fpc_mode;
  byte fpc_pri_chan;          // 0 - f-fch, 1 - f-dcch
  boolean fpc_olpc_fch_incl;
  byte fpc_fch_fer;
  byte fpc_fch_min_setpt;
  byte fpc_fch_max_setpt;
  boolean fpc_olpc_dcch_incl;
  byte fpc_dcch_fer;
  byte fpc_dcch_min_setpt;
  byte fpc_dcch_max_setpt;
#ifdef FEATURE_IS2000_2_1_ADDENDUM
  boolean fpc_olpc_sch_m_incl;
#endif
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte fpc_sec_chan;
#else
  boolean fpc_sec_chan;
#endif
#ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte fpc_sch_m_fer;
  byte fpc_min_sch_m_setpt;
  byte fpc_max_sch_m_setpt;
#endif
  byte num_sup;
  caii_fpc_sup_chn_type sup_chns[CAI_MAX_NUM_FOR_SCH];
  boolean fpc_thresh_incl;
  byte fpc_setpt_thresh;
  boolean fpc_thresh_sch_incl;
  byte fpc_setpt_thresh_sch;
  boolean rpc_incl;
  byte rpc_num_rec;
  caii_rpc_type rpc_recs[CAI_MAX_NUM_RPC_RECS];
#endif
} caii_pwr_ctrl_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* EXTENDED NEIGHBOR LIST UPDATE MESSAGE (see IS-95A Section 7.7.3.3.2.26) */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte pilot_inc;        /* Pilot Index Increment value */
  byte nghbr_srch_mode;  /* Neighbor Search Mode */
  byte srch_win_n;       /* Default window size for neighbor set */
  boolean use_timing;    /* Use timing indicator */
  boolean global_timing_incl; /* Global Timing Included */
  byte global_tx_duration;  /* Global neighbor transmit time duration */
  byte global_tx_period;  /* Global neighbor transmit time period */
  byte num_nghbr;        /* Number of entries in the nghbr array */
  struct
  {
    word nghbr_pn;         /* Offset of neighbor's PN pilot offset relative
                              to system time */
    byte search_pri;         /* Search Piority */
    byte srch_win_nghbr;   /* Per neighbor search window */
    boolean timing_incl;   /* Timing Inlcuded Indicator */
    byte nghbr_tx_offset; /* Neighbor transmit time offset */
    byte nghbr_tx_duration; /* Global neighbor transmit time duration */
    byte nghbr_tx_period; /* Global neighbor transmit time period */
  } nghbr[CAI_EXT_NLU_MAX];
#ifdef FEATURE_IS2000
  boolean srch_offset_incl;
  struct
  {
    boolean add_pilot_rec_incl;
    byte nghbr_pilot_rec_type;
    caii_add_pilot_rec_type add_pilot_rec;
    byte srch_offset_nghbr;
  } add_nghbr[CAI_EXT_NLU_MAX];
#endif
} caii_ext_nlu_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  CANDIDATE FREQUENCY SEARCH REQUEST MESSAGE                             */
/*  (see IS-95B Section 7.7.3.3.2.27)                                      */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  boolean pre_proc_done; /* Is pre-processing done? */
  boolean use_time;      /* Indicator of whether the action_time field should
                            be used */
  byte action_time;      /* Action time */
  byte cfsrm_seq;        /* CFSR message sequence number */
  byte srch_type;        /* Search Type Code */
  byte srch_period;      /* Search Period    */
  byte srch_mode;        /* Search Mode Type */
  byte mod_sp_len;       /* Mode specific length */
} caii_cfs_req_fix_type;

typedef struct
{
  byte band_class;           /* Band Class     */
  word cdma_freq;            /* CDMA Frequence Assignment */
  byte sf_total_ec_thresh;   /* Serving Frequency total pilot Ec threshold */
  byte sf_total_ec_io_thresh;/* Serving Frequency total pilot Ec/Io thresh.*/
  byte diff_rx_pwr_thresh;   /* Minimum Difference in received power */
  byte min_total_pilot_ecio; /* Minimum Total Pilot Ec/Io */
  byte cf_t_add;             /* Pilot Detection Threshold for the CDMA
                              * Candidate Frequency                      */
  byte tf_wait_time;         /* CDMA Candidate Frequence Total Wait Time */
  byte cf_pilot_inc;         /* Pilot PN sequence offset index increment
                              * to be used on the CDMA Candidate Frequency
                              * after handoff                             */
  byte cf_srch_win_n;        /* Default Search Window Size for the
                              * Candidate Frequency Search Set            */
  byte cf_srch_win_r;        /* Search Window Size for the Remaining Set  */
  boolean pilot_update;      /* Pilot Search Parameter update Indicator   */
  byte num_pilot;            /* Number of Pilots included in the Message  */
  byte cf_nghbr_srch_mode;   /* Search mode for candidate frequency search
                              * set                                       */
  struct
  {
    word    nghbr_pn;        /* Offset of neighbor pilot pn */
    boolean search_set;      /* Pilot Search Indicator      */
    byte    search_priority; /* Pilot Channel Search priority */
    byte    srch_win_nghbr;  /* Beighbor pilot channel search window size */
  } pilot_update_rec[CAI_CFSREQ_REC_MAX];

#ifdef FEATURE_IS2000
  boolean cf_srch_offset_incl;
  struct {
    boolean add_pilot_rec_incl;
    byte nghbr_pilot_rec_type;
    caii_add_pilot_rec_type add_pilot_rec;
    byte srch_offset_nghbr;
  } add_nghbr[CAI_CFSREQ_REC_MAX];
#endif

} caii_cfs_req_mod0_type;

typedef struct
{
  byte band_class;           /* Band Class     */
  byte sf_total_ec_thresh;   /* Serving Frequency total pilot Ec threshold */
  byte sf_total_ec_io_thresh;/* Serving Frequency total pilot Ec/Io thresh.*/
  byte num_analog_freqs;     /* Number of analog freq */
  word analog_freq[CAI_CFS_ANALOG_MAX]; /* Analog frequency */
} caii_cfs_req_mod1_type;

typedef struct
{
  caii_cfs_req_fix_type fix;
  union
  {
    caii_cfs_req_mod0_type mod0;
    caii_cfs_req_mod1_type mod1;
  } var;
#ifdef FEATURE_IS2000
  byte align_timing;     /* Align timing for CF Search */
  byte search_offset;    /* Srch offset time depends on align_timing */
#endif
} caii_cfs_req_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CANDIDATE FREQUENCY SEARCH CONTROL MESSAGE(see IS-95A Sec 7.7.3.3.2.28) */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  boolean pre_proc_done; /* Is pre-processing done? */
  boolean use_time;      /* Indicator of whether the action_time field should
                            be used */
  byte action_time;      /* Action time */
  byte cfscm_seq;        /* CFSR message sequence number */
  byte srch_type;        /* Search Type Code */
} caii_cfs_ctrl_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* POWER UP FUNCTION MESSAGE (see IS-95A Section 7.7.3.3.2.29)     */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  boolean use_time;      /* Indicator of whether the action_time field should
                            be used */
  byte action_time;      /* Action time */
  byte act_tm_frm;       /* Action time frame */
  byte puf_setup_sz;     /* Puf setup size */
  byte puf_pulse_sz;     /* Puf pulse size */
  word puf_interval;     /* Puf interval */
  byte puf_init_pwr;     /* Power increase of initial pulse */
  byte puf_pwr_step;     /* PUF power step */
  byte tot_puf_prob;     /* Total number of PUF probes */
  byte max_pwr_puf;      /* Max # of probe at full power */
  boolean puf_freq_incl; /* Frequency included indicator */
  byte band_class;       /* PUF band class */
  word cdma_freq;        /* PUF CDMA frequency */
} caii_puf_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* POWER UP FUNCTION COMPLETION MESSAGE (see IS-95A Section 7.7.3.3.2.30)  */
typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  boolean loc_ind;       /* Location indicator */
  dword ms_lat;          /* Mobile station latitude */
  dword ms_long;         /* Mobile station longitude */
  dword tm_stamp;        /* Time stamp */
} caii_puf_comp_type;
#endif /* FEATURE_IS95B */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This section defines a generic handoff direction message.               */
/* HANDOFF DIRECTION MESSAGE (see IS-95A Section 7.7.3.3.2.5)              */
/* EXTENDED HANDOFF DIRECTION MESSAGE (see IS-95B Section 7.7.3.3.2.17)    */
/* GENERAL HANDOFF DIRECTION MESSAGE (see IS-95B Section 7.7.3.3.2.31)     */
/* UNIVERSAL HANDOFF DIRECTION MESSAGE (see IS-95B Section 7.7.3.3.2.36)   */

typedef struct
{
  byte srch_win_a;       /* Active and Candidate Set search window size */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte srch_win_n;       /* Search window size for the Neighbor Set */
  byte srch_win_r;       /* Search window size for the Remaining Set */
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

  byte t_add;            /* Pilot detection threshold */
  byte t_drop;           /* Pilot drop threshold */
  byte t_comp;           /* Active vs Candidate set comparison threshold */
  byte t_tdrop;          /* Drop timer value */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte soft_slope;       /* Soft Slope for soft HO inequality */
  byte add_intercept;    /* Add Intercept for soft HO inequality */
  byte drop_intercept;   /* Drop Intercept for soft HO inequality */
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

} caii_ho_srch_type;

typedef struct
{
#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  byte p_rev;            /* Protocol Revision for new base station */
  byte packet_zone_id;   /* Packet Data Zone ID for new base station */
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

  byte frame_offset;     /* Frame offset */
  boolean private_lcm;   /* Private long code mask indicator */
  boolean reset_l2;      /* Reset acknowledgement procedures command */
  boolean reset_fpc;     /* Reset forward power control procedures command */
  byte serv_neg_type;    /* Service Negotiation type */
  byte encrypt_mode;     /* Message encryption mode */
  boolean nom_pwr_ext;   /* Extended nominal transmit power indicator */
  byte nom_pwr;          /* Nominal transmit power */
  byte num_preamble;     /* Number of preamble frames to transmit */
  byte band_class;       /* Which band the CDMA channel frequency is in */
  byte freq_incl;        /* Only in HDM.  TRUE if CDMA_FREQ is included */
  word cdma_freq;        /* Frequency assignment of CDMA channel */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  boolean return_if_ho_fail; /* Return to old config if handoff fails */
  boolean complete_srch; /* Complete candidate frequency search flag */
  boolean periodic_srch; /* Periodic search of candidate freq flag */
  boolean scr_included;  /* Service configuration included flag */
  byte serv_con_seq;     /* Connect sequence number */
  caii_srv_cfg_type cfg; /* Service configuration record */
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
#ifdef FEATURE_IS2000
  byte rlgain_traffic_pilot;
  boolean default_rlag;
  boolean nnscr_included;
  caii_non_neg_srv_cfg_type non_neg_cfg;
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean rc_sync_id_incl;
  byte rc_sync_id;
#endif
#endif
} caii_ho_extra_type;

typedef struct
{
  byte add_length;
  byte ext_ho_p_rev;
} caii_ho_add_type;

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

typedef struct
{
  boolean for_included;      /* FWD supp channel config included indicator */
  byte for_sup_config;       /* FWD supp channel config indicator */
  byte num_for_sup;          /* Number of FWD supp code channel */
  boolean use_for_dur;       /* Use forward duration indicator */
  byte for_dur;              /* Forward duration */
  boolean rev_included;      /* Reverse Supplemental Code Channel config */
  byte rev_dtx_duration;     /* Reverse Discontinuous Transmission Duration*/
  boolean clear_retry_delay; /* Clear SCRM retry delay indicator */
  boolean use_rev_dur;       /* Use reverse duration indicator */
  byte rev_dur;              /* Reverse duration */
  byte num_rev_codes;        /* Number of reverse sup code channels */
  boolean use_t_add_abt;     /* Reverse use T_ADD abort indicator */
  boolean rev_parms_inc;     /* Reverse additional parameter included */
  byte t_mulchan;            /* SCRM pilot strength reporting offset */
  byte begin_pre;            /* Number of preamble frames */
  byte resume_pre;           /* Number of preamble at resumption of xmit */
} caii_ho_sup_chan_parms_type;

#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

#ifdef FEATURE_IS2000
typedef struct
{
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  byte for_sch_id; /* this is probably a typo in the standard */
#else
  byte for_sch_id;
#endif
  byte sccl_index;
  boolean pilot_incl;
  word code_chan_sch;
  byte qof_mask_id_sch;
} caii_sch_chan_rec_type;

typedef struct
{
  byte num_for_sch;
  struct {
   byte for_sch_id;   // Only 0 or 1 here
   byte sccl_index;
   byte for_sch_rate;
  } for_sch_cfg[CAI_MAX_NUM_FOR_SCH_CFG];

  byte num_rev_sch;
  struct {
   byte rev_sch_id;   // Only 0 or 1 here
   byte rev_walsh_id; // See IS2000.5 table 3.7.3.3.2.37-2
   byte rev_sch_rate;
  } rev_sch_cfg[CAI_MAX_NUM_REV_SCH_CFG];
} caii_sch_cfg_type;
#endif

typedef struct
{
  word    pilot_pn;          /* Pilot PN sequence offset index */
  boolean pwr_comb_ind;      /* Power control symbol combining indicator */
#if ((!defined(FEATURE_IS2000)) || defined(FEATURE_MSG_PACK_UNIT_TEST))
  byte    code_chan;         /* Fundamental Code channel index */
#endif
#ifdef FEATURE_IS2000
  boolean add_pilot_rec_incl;
  byte pilot_rec_type;
  caii_add_pilot_rec_type add_pilot_rec;
  word code_chan_fch;
  byte qof_mask_id_fch;
  word code_chan_dcch;
  byte qof_mask_id_dcch;
  byte num_sch;
#ifdef FEATURE_IS2000
  byte srch_offset;
#endif
#endif

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  boolean for_sup_inc;       /* Forward sup code chan included indicator */
  boolean expl_code_chan;    /* Explicit Code Channel indicator */
  union
  {
    byte sup_code_chan[CAI_MAX_NUM_SUP];
                             /* Forward supplemental code channels */
    byte base_code_chan;     /* Base code channel index */
#ifdef FEATURE_IS2000
    caii_sch_chan_rec_type sch_chan_rec[CAI_MAX_NUM_SCH_CHAN];
#endif
  } chan_rec;
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

} caii_ho_pilot_type;

typedef struct
{
  byte msg_type;                /* Message type */
  caii_tc_hdr_type hdr;         /* Layer 2 header */
  boolean pre_proc_done;        /* Is pre-processing done? */
  boolean use_time;             /* Use action_time indicator */
  byte action_time;             /* Action time */
  byte hdm_seq;                 /* Handoff direction message sequence # */
#ifdef FEATURE_IS2000
  boolean parms_incl;
#endif
  boolean srch_inc;             /* Pilot search parameter included */
  caii_ho_srch_type srch;       /* Search parameters */
  boolean extra_parms;          /* Hard Handoff plus other parms included */
  caii_ho_extra_type extra;     /* Extra parameters (mostly hard HO) */
  caii_ho_add_type add;         /* Additional fields for EHDM */

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
  boolean sup_chan_parms_inc;   /* MDR supplemental channel parms included */
  caii_ho_sup_chan_parms_type sup; /* MDR supplemental channel parameters */
  boolean use_pwr_cntl_step;    /* Use Power Control Step indicator */
  byte pwr_cntl_step;           /* Power Control Step */
#endif // defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)

  byte num_pilots;              /* Num pilot records included */
  caii_ho_pilot_type pilot[CAI_HO_MSG_MAX_PILOTS]; /* Pilot records */
  /* reserved field is not included, since we don't care what it is */
#ifdef FEATURE_IS2000
  byte fpc_subchan_gain;
  boolean use_pc_time;
  byte pc_action_time;
#ifndef FEATURE_IS2000_2_1_ADDENDUM
  boolean rev_fch_gating_mode;
  boolean rev_pwr_cntl_delay_incl;
  byte rev_pwr_cntl_delay;
#endif
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean blob_incl;
  byte blob;
#else
  boolean clear_retry_delay;
  boolean srch_offset_incl;
#endif
  boolean sch_incl;
  byte num_for_assign;
  struct {
    byte for_sch_id;
    byte for_sch_duration;
    boolean for_sch_start_time_incl;
    byte for_sch_start_time;
    byte sccl_index;
  } for_assign[CAI_MAX_NUM_FOR_SCH];
  byte num_rev_assign;
  struct {
    byte rev_sch_id;
    byte rev_sch_duration;
    boolean rev_sch_start_time_incl;
    byte rev_sch_start_time;
    byte rev_sch_rate;
  } rev_assign[CAI_MAX_NUM_REV_SCH];
  byte ch_ind;
  /* active_set_rec_len is used only for parsing */
  caii_sch_cfg_type sch_cfg;
#endif
} caii_ho_msg_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  Extended Supplemental Channel Assignment                               */
/*  (see IS-2000 Section 3.7.3.3.2.37)                                     */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#ifdef FEATURE_IS2000

typedef struct
{
  byte msg_type;                /* Message type */
  caii_tc_hdr_type hdr;         /* Layer 2 header */
  byte    start_time_unit;
  byte    rev_sch_dtx_duration;
  boolean use_t_add_abort;
  boolean use_scrm_seq_num;
  byte    scrm_seq_num;
  boolean add_info_incl;
  byte    fpc_pri_chan;         // 0 - Power control on on f-fch, 1 - power control on f-dcch
  boolean rev_cfg_included;
  byte    num_rev_cfg_recs;
  struct
  {
    byte    rev_sch_id;      // Only 0 and 1 valid
    boolean rev_walsh_id;
    byte    rev_sch_rate;
  } rev_cfg[CAI_ESCAM_MAX_REV_CFG];
  byte num_rev_sch;
  struct
  {
    byte    rev_sch_id;      // Only 0 and 1 valid
    byte    rev_sch_duration;
    boolean rev_sch_start_time_incl;
    byte    rev_sch_start_time;
    byte    rev_sch_rate;
  } rev_sch[CAI_ESCAM_MAX_REV_SCH];
  #ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte num_for_sch_cfg;
  struct
  {
    byte for_sch_id;      // Only 0 and 1 valid
    byte num_rec;
    struct
    {
      byte sccl_index;
      byte for_sch_rate;
      byte num_sup_sho;
      struct
      {
        word pilot_pn;
        byte add_pilot_rec_incl;
        byte active_pilot_rec_type;
        caii_add_pilot_rec_type add_pilot_rec;
        word for_sch_cc_index;
        byte qof_mask_id_sch;
      } sup_sho[CAI_ESCAM_MAX_SUP_SHO];
    } rec[CAI_ESCAM_MAX_RECS];
  } for_sch_cfg[CAI_ESCAM_MAX_FOR_CFG];
  #else
  boolean for_cfg_included;
  boolean for_sch_fer_rep;
  byte num_for_cfg_recs;
  struct
  {
    byte for_sch_id; // Only 0 and 1 valid
    byte sccl_index;
    byte for_sch_rate;
    byte num_sup_sho;
    struct
    {
      word pilot_pn;
      byte add_pilot_rec_incl;
      byte active_pilot_rec_type;
      caii_add_pilot_rec_type add_pilot_rec;
      word for_sch_cc_index;
      byte qof_mask_id_sch;
    } sup_sho[CAI_ESCAM_MAX_SUP_SHO];
  } for_cfg[CAI_ESCAM_MAX_FOR_CFG];
  #endif  // FEATURE_IS2000_2_1_ADDENDUM
  byte num_for_sch;
  struct
  {
    byte    for_sch_id;      // Only 0 and 1 valid
    byte    for_sch_duration;
    boolean for_sch_start_time_incl;
    byte    for_sch_start_time;
    #ifdef FEATURE_IS2000_2_1_ADDENDUM
    boolean for_sch_fer_rep;
    #endif
    byte    sccl_index;
  } for_sch[CAI_ESCAM_MAX_FOR_SCH];
  boolean fpc_incl;
  byte    fpc_mode_sch;
  boolean fpc_sch_init_setpt_op;
  #ifdef FEATURE_IS2000_2_1_ADDENDUM
  boolean fpc_olpc_sch_m_incl;
  #endif
  byte    fpc_sec_chan;   // 0 = SCH 0, 1 = SCH 1
  #ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte fpc_sch_m_fer;
  byte fpc_sch_init_m_setpt;
  byte fpc_sch_min_m_setpt;
  byte fpc_sch_max_m_setpt;
  #endif  // FEATURE_IS2000_2_1_ADDENDUM
  byte num_sup;
  struct
  {
    byte sch_id;
    byte fpc_sch_fer;
    byte fpc_sch_init_setpt;
    byte fpc_sch_min_setpt;
    byte fpc_sch_max_setpt;
  } sup[CAI_ESCAM_MAX_SUP];
  boolean fpc_thresh_sch_incl;
  byte    fpc_setpt_thresh_sch;
  boolean rpc_incl;
  #ifdef FEATURE_IS2000_2_1_ADDENDUM
  byte    rlgain_sch_pilot;
  #else
  boolean rpc_num_sup;
  struct
  {
    boolean sch_id;
    byte rlgain_sch_pilot;
  } rpc_sup[CAI_ESCAM_MAX_SUP];
  #endif  // FEATURE_IS2000_2_1_ADDENDUM
} caii_escam_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                REVERSE TRAFFIC CHANNEL MESSAGES                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* ORDER MESSAGE (see IS-95A Section 6.7.2.3.2.1                           */

/* Generic Order type */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  byte order;           /* Order field */
} caii_rtc_gen_ord_type;

/* Order with order qualifier */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  byte order;           /* Order field */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;            /* Order qualifier field */
} caii_rtc_ordq_type;

/* Base Station Challenge Order (see IS-95A Section 6.7.3.1) */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  byte order;           /* Order field */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;            /* Order qualifier field */
  dword randbs;         /* Random challenge data */
} caii_rtc_bs_chal_type;

/* Service option request or Service Option response type (see IS-95A
   Sections 6.7.3.2 and 6.7.3.3) */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  byte order;           /* Order field */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;            /* Order qualifier field */
  word service_option;  /* Service option */
} caii_rtc_so_type;

/* Mobile Station Reject Order (see IS-95A Section 6.7.3.4) */

/* Fixed part of Mobile Station Reject Order */
typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  byte order;           /* Order field */
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif
  byte ordq;            /* Order qualifier field */
  byte rej_msg_type;    /* Message type for the message being rejected */
} caii_rtc_rej_fixed_type;

/* Only present if the rejected message was an Order Message */
typedef struct
{
  byte rej_order;  /* Order type of rejected message */
  byte rej_ordq;   /* Order qualification code of rejected message */
} caii_rtc_rej_order_type;

/* Only present if the rejected message was a Retrieve Parameters Message
   or a Set Parameters Message */
typedef struct
{
  word rej_param_id;  /* Parameter id of the rejected parameter */
} caii_rtc_rej_param_type;

/* Only present if the rejected message was an Alert with Information
   Message or a Flash with Information Message */
typedef struct
{
  byte rej_record; /* Record type of the rejected information record */
} caii_rtc_rej_record_type;

typedef struct
{
  caii_rtc_rej_fixed_type fix;
  union {
    caii_rtc_rej_order_type  order;
    caii_rtc_rej_param_type  param;
    caii_rtc_rej_record_type rec;
  } var;
#ifdef FEATURE_IS2000
  byte rejected_pdu_type;
#endif
} caii_rtc_rej_type;

/* Order Message */
typedef union
{
  caii_rtc_gen_ord_type gen;
  caii_rtc_ordq_type    ordq;
  caii_rtc_bs_chal_type chal;
  caii_rtc_so_type      so;
  caii_rtc_rej_type     rej;
} caii_rtc_ord_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* AUTHENTICATION CHALLENGE RESPONSE MSG (see IS-95A Section 6.7.2.3.2.2) */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  dword authu;           /* Authentication challenge response */
} caii_auth_rsp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* FLASH WITH INFORMATION MESSAGE (see IS-95A Section 6.7.2.3.2.3)        */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte num_recs;         /* Number of information records in message */
  byte recs[CAII_MAX_INFO_REC_BYTES];
    /* Records - consecutive Reverse Traffic Channel Information Records -
       use caii_rec_hdr type to parse different records */
} caii_flash_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* TMSI ASSIGNMENT C0MPLETION MESSAGE (see J-STD-008 Section 2.7.2.3.2.17) */

typedef struct /* TMSI Assignment Completion Message - Reverse Traffic Chn */
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
} caii_rtc_tmsi_cmp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DATA BURST MESSAGE (see IS-95A Section 6.7.2.3.2.4)                    */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte msg_number;       /* Number of message in the data stream burst */
  byte burst_type;       /* Type of data burst */
  byte num_msgs;         /* Number of messages in the data stream burst */
  byte num_fields;       /* Number of characters in chari field */
  byte chari[CAI_REV_TC_BURST_MAX];
} caii_rev_tc_burst_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PILOT STRENGTH MEASUREMENT MESSAGE (see IS-95A Section 6.7.2.3.2.5)    */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  word ref_pn;           /* Time reference PN sequence offset */
  byte pilot_strength;   /* Pilot strength */
  boolean keep;          /* Keep pilot indicator */
  word num_msr;          /* Number of entries in the meas array */
  struct
  {
    word pilot_pn_phase;  /* Pilot measured phase */
    byte pilot_strength;  /* Pilot strength */
    boolean keep;         /* Keep pilot indicator */
  } meas[CAI_PIL_STR_MAX];
} caii_pil_str_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* POWER MEASUREMENT REPORT MESSAGE (see IS-95A Section 6.7.2.3.2.6)      */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte errors_detected;  /* Number of frame errors detected */
  word pwr_meas_frames;  /* Number of Forward Traffic Channel frames
                            in the measurement period */
  byte last_hdm_seq;     /* Handoff Direction Message sequence number */
  byte num_pilots;       /* Number of pilots */
  byte pilot_strength[CAI_PWR_MAX];  /* Active set pilots */
#ifdef FEATURE_IS2000
  boolean dcch_pwr_meas_incl;
  word dcch_pwr_meas_frames;
  byte dcch_errors_detected;
  boolean sch_pwr_meas_incl;
  boolean sch_id;
  word sch_pwr_meas_frames;
  word sch_errors_detected;
#endif
} caii_pwr_rpt_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SEND BURST DTMF MESSAGE (see IS-95A Section 6.7.2.3.2.7)               */

/* Use caii_sbdtmf_type from Forward Traffic Channel Message record type
   definitions above */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* STATUS MESSAGE (see IS-95A Section 6.7.2.3.2.8)                        */

typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  byte recs[CAII_MAX_INFO_REC_BYTES];
    /* Records - consecutive Reverse Traffic Channel Information Records -
       use caii_rec_hdr type to parse different records */
} caii_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ORIGINATION CONTINUATION MESSAGE (see IS-95A Section 6.7.2.3.2.9)      */

typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  boolean digit_mode;   /* Digit mode indicator */
  byte num_fields;      /* Number of dialed digits in the message */
  byte chari[CAI_ORIG_C_MAX];  /* Dialed digits */
  /* Following fields are for IS-95B only. Higher level should set num_fields
     to 0 for lower P_REV */
  byte num_recs;              /* Number of records in message */
  byte recs[CAII_MAX_INFO_REC_BYTES];
    /* Records - consecutive Forward Traffic Channel Information Records -
       use caii_rec_hdr type to parse different records */
} caii_orig_c_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* HANDOFF COMPLETION MESSAGE (see IS-95A Section 6.7.2.3.2.10)           */

typedef struct
{
  byte msg_type;         /* Message type */
  caii_tc_hdr_type hdr;  /* Layer 2 header */
  byte last_hdm_seq;     /* Handoff Direction Message sequence number */
  byte num_pilots;       /* Number of pilots in pilot_pn array */
  word pilot_pn[CAI_MAX_COMP];  /* Pilot PN sequence offsets */
} caii_comp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PARAMETERS RESPONSE MESSAGE (see IS-95A Section 6.7.2.3.2.11)          */

typedef struct
{
  byte msg_type;        /* Message type */
  caii_tc_hdr_type hdr; /* Layer 2 header */
  word num_parms;       /* Number of parameters in parameter array */
  struct
  {
    word parameter_id;  /* Parameter identification */
    word parameter_len; /* Length of parameter */
    union
    {
#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#else
      byte parm_b;      /* byte length parameter */
      word parm_w;      /* word length parameter */
      dword parm_d;     /* dword length parameter */
      qword parm_q;     /* qword length parameter */
#endif
    } parameter;
  } parms[CAI_PARM_RSP_MAX ];  /* Array of parameters */
} caii_parm_rsp_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE REQUEST MESSAGE (See IS-95-A TSB)                              */

typedef struct
{
  byte  msg_type;          /* Message type */
  caii_tc_hdr_type  hdr;   /* Layer 2 header */
  byte  req_seq;           /* Request sequence number */
  byte  purp;              /* Purpose code */
  caii_srv_cfg_type  cfg;  /* Service configuration record */
#ifdef FEATURE_IS2000
#ifdef FEATURE_IS2000_REV0_PREADDENDUM
  boolean blob_incl;
  byte blob;
#endif
#endif
} caii_rtc_srv_req_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE RESPONSE MESSAGE (See IS-95-A TSB)                             */

typedef struct
{
  byte  msg_type;          /* Message type */
  caii_tc_hdr_type  hdr;   /* Layer 2 header */
  byte  req_seq;           /* Request sequence number */
  byte  purp;              /* Purpose code */
  caii_srv_cfg_type  cfg;  /* Service configuration record */
} caii_rtc_srv_rsp_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE CONNECT COMPLETION MESSAGE (See IS-95-A TSB)                   */

typedef struct
{
  byte  msg_type;         /* Message type */
  caii_tc_hdr_type  hdr;  /* Layer 2 header */
  byte  con_seq;          /* Connect sequence number */
} caii_srv_cmp_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*  SERVICE OPTION CONTROL MESSAGE (See IS-95-A TSB)                       */

typedef struct
{
  byte  msg_type;                    /* Message type */
  caii_tc_hdr_type  hdr;             /* Layer 2 header */
  byte  con_ref;                     /* Service option connection ref */
  word  so;                          /* Service option number */
  byte  tsf_len;                     /* Type specific fields length */
  byte  tsf[ CAI_RTC_SRV_CTL_MAX ];  /* Type specific fields */
} caii_rtc_srv_ctl_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* STATUS RESPONSE MESSAGE (see IS-95-A TSB)                               */

typedef struct
{
  byte  msg_type;         /* Message type */
  caii_tc_hdr_type  hdr;  /* Layer 2 header */
  byte qual;              /* Qualification information type */
  byte band;              /* Band class qualifier */
  byte op_mode;           /* Operating mode qualifier */
  byte num_recs;          /* Number of information records */
  byte recs[CAII_MAX_INFO_REC_BYTES];
    /* Records - consecutive Information Records -
       use caii_rec_hdr type to parse different records */
} caii_tc_strsp_type;

#ifdef FEATURE_IS95B
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SUPPLEMENTAL CHANNEL REQUEST MESSAGE (see IS-95B Section 6.7.2.3.2.18)  */
typedef struct
{
  byte msg_type;              /* Message type */
  caii_tc_hdr_type hdr;       /* Layer 2 header */
  byte sz_req_blob;           /* Size of the request info block of bytes */
  byte req_blob[CAI_MAX_SCRM_BLOB_SIZE];
                              /* request info block */
  boolean use_scrm_seq_num;   /* Use SCRM sequence number indicator */
  byte scrm_seq_num;          /* SCRM sequence number */
  word ref_pn;                /* Time reference PN offset */
  byte pilot_str;             /* Pilot strength */
  byte num_act_pn;            /* Number of pn in the active set */
#ifndef FEATURE_MSG_PACK_UNIT_TEST
  byte num_nghbr_pn;          /* Number of neigbor set pn */
#endif
  struct
  {
    word act_pn_phase;        /* PN phase */
    byte act_pn_str;          /* Strength of the PN */
  } act_pn[CAI_MAX_SCRM_ACT_PN];

#ifdef FEATURE_MSG_PACK_UNIT_TEST
#error code not present
#endif

  struct
  {
    word nghbr_pn_phase;        /* PN phase */
    byte nghbr_pn_str;          /* Strength of the PN */
  } nghbr_pn[CAI_MAX_SCRM_NGHBR_PN];
} caii_scrm_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CANDIDATE FREQUENCY SEARCH RESPONSE MESSAGE(see IS-95B Sec 6.7.2.3.2.19)*/
typedef struct
{
  byte msg_type;              /* Message type */
  caii_tc_hdr_type hdr;       /* Layer 2 header */
  byte last_cfsrm_seq;        /* CFSRM sequence number */
  byte tot_off_tm_fwd;        /* Total time the mobile is off the FTC */
  byte max_off_tm_fwd;        /* Max time the mobile is off the FTC */
  byte tot_off_tm_rev;        /* Total time the mobile is off the RTC */
  byte max_off_tm_rev;        /* Max time the mobile is off the RTC */
#ifdef FEATURE_IS2000
  boolean pcg_off_times;
  boolean align_timing_used;
  byte num_visits;
  byte inter_visit_time;
#endif
} caii_cfs_rsp_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* CANDIDATE FREQUENCY SEARCH REPORT MESSAGE (see IS-95B Sec 6.7.2.3.2.20) */
typedef struct
{
  byte msg_type;              /* Message type */
  caii_tc_hdr_type hdr;       /* Layer 2 header */
  boolean last_srch_msg;      /* Type of msg started the search */
  byte last_srch_seq;         /* Seq # in the msg started the search */
  byte srch_mode;             /* Search mode */
} caii_cfs_rpt_fix_type;

typedef struct
{
  byte band_class;            /* Band class */
  word cdma_freq;             /* CDMA frequency */
  byte sf_tot_rx_pwr;         /* Total received pwr on the serving freq */
  byte cf_tot_rx_pwr;         /* Total received pwr on the candidate freq*/
  byte num_pilots;            /* Number of pilots */
  struct
  {
    word plt_pn_phase;         /* Pilot PN phase */
    byte plt_str;              /* Pilot strength */
  } pilot[CAI_N8M + CAI_N6M];  /* CF srch set same size as NSET. */
        /* Report gives results from CF srch and/or ASET from failed handoff */
} caii_cfs_rpt_mod0_type;

typedef struct
{
  byte band_class;            /* Band Class */
  byte sf_tot_rx_pwr;         /* Total receiving power on the serving freq */
  byte num_analog_freqs;      /* Number of analog frequencies */
  struct
  {
    word analog_freq;         /* Analog frequency */
    byte sig_str;             /* Signal strength */
  } analog[CAI_MAX_ANALOG_FREQS];
} caii_cfs_rpt_mod1_type;

typedef struct
{
  caii_cfs_rpt_fix_type fix;
  union
  {
    caii_cfs_rpt_mod0_type mod0;
    caii_cfs_rpt_mod1_type mod1;
  } var;
} caii_cfs_rpt_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* PERIODIC PILOT STRENGTH MEASUREMENT MESSAGE(see IS-95B Sec 6.7.2.3.2.21)*/
typedef struct
{
  byte msg_type;                 /* Message type */
  caii_tc_hdr_type hdr;          /* Layer 2 header */
  word ref_pn;                   /* Time reference PN seq offset */
  byte pilot_str;                /* Pilot strength */
  boolean keep;                  /* Keep pilot indicator */
  byte sf_rx_pwr;                /* Serving Freq received power density */
  byte num_pilot;                /* Number of pilots */
} caii_plt_str_meas_fix_type;

typedef struct
{
  word    plt_pn_phase;          /* Pilot PN phase */
  byte    plt_str;               /* Pilot strength */
  boolean keep;                  /* keep pilot indicator */
} caii_plt_str_meas_var_type;

typedef struct
{
  caii_plt_str_meas_fix_type  fix;
  caii_plt_str_meas_var_type  var[CAI_PLT_STR_MEAS_MAX];
} caii_plt_str_meas_type;

#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* OUTER LOOP REPORT MESSAGE (see IS-2000 Sec 2.7.2.3.2.22)                */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
typedef struct
{
  byte msg_type;                 /* Message type */
  caii_tc_hdr_type hdr;          /* Layer 2 header */
  boolean fch_incl;
  byte fpc_fch_curr_setpt;
  boolean dcch_incl;
  byte fpc_dcch_curr_setpt;
  byte num_sup;
  struct
  {
    boolean sch_id;
    byte fpc_sch_curr_setpt;
  } sup[CAI_OUTER_RPT_MAX_SUP];
} caii_outer_rpt_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*   Union of Received Messages                                            */

typedef union
{
  caii_gen_type           gen;
  caii_gen_pc_type        gen_pc;
  caii_gen_tc_type        gen_tc;
  caii_sync_type          sync;
  caii_sysparm_type       sysparm;
  caii_accparm_type       accparm;
  caii_ext_nghbr_type     ext_nghbr;
  caii_nghbr_type         nghbr;
  caii_chnlist_type       chnlist;
  caii_slt_page_type      slt_page;
  caii_page_type          page;
  caii_pc_ord_type        pc_ord;
  caii_chnasn_type        chnasn;
  caii_pc_burst_type      pc_burst;
  caii_pc_auth_ch_type    pc_auth;
  caii_pc_ssd_type        pc_ssd;
  caii_pc_feature_type    pc_feat;
  caii_ext_sysparm_type   ext_sysparm;
  caii_serv_redirect_type serv_redir;
  caii_pc_streq_type      pc_streq;
  caii_gen_page_type      gen_page;
  caii_glob_redirect_type glob_redir;
  caii_ftc_ord_type       tc_ord;
  caii_auth_ch_type       auth_ch;
  caii_alert_type         alert;
  caii_fwd_tc_burst_type  tc_burst;
  caii_ho_msg_type        ho;       /* Generic TC handoff msg. */
  caii_fm_ho_type         fm_ho;
  caii_tc_sysparm_type    tc_sys;
  caii_nlu_type           nlu;
  caii_sbdtmf_type        sbdtmf;
  caii_pwr_ctl_type       pwr_ctl;
  caii_ret_parms_type     ret;
  caii_set_parm_type      set;
  caii_ssd_up_type        ssd_up;
  caii_registered_type    reg;
  caii_tc_streq_type      tc_streq;
  caii_ftc_srv_req_type   srv_req;
  caii_ftc_srv_rsp_type   srv_rsp;
  caii_srv_con_type       srv_con;
  caii_ftc_srv_ctl_type   srv_ctl;
  caii_pc_tmsi_asgn_type  pc_tmsi;
  caii_ftc_tmsi_asgn_type tc_tmsi;

#ifdef FEATURE_IS95B
  caii_paca_type          paca;
  caii_ext_chnasn_type    ext_chnasn;
  caii_gen_nghbr_type     gen_nghbr;
  caii_ftc_srv_redir_type tc_srv_redir;
  caii_scam_type          scam;
  caii_pwr_ctrl_type      pwr_ctrl;
  caii_ext_nlu_type       ext_nlu;
  caii_cfs_req_type       cfs_req;
  caii_cfs_ctrl_type      cfs_ctrl;
  caii_puf_type           puf;
  caii_puf_comp_type      puf_comp;
#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
  caii_escam_type         escam;
  caii_ext_glb_redir_type egsrm;
  caii_ext_chnlist_type   ecclm;
#endif

} caii_rx_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*   Union of Transmitted Messages                                         */

typedef union
{
  caii_gen_type          gen;
  caii_gen_ac_type       gen_ac;
  caii_gen_tc_type       gen_tc;
  caii_reg_type          reg;
  caii_ac_ord_type       ac_ord;
  caii_ac_burst_type     ac_burst;
  caii_orig_type         orig;
  caii_page_resp_type    page_resp;
  caii_auth_ch_resp_type auth_ch;
  caii_auth_rsp_type     auth_resp;
  caii_ac_strsp_type     ac_strsp;
  caii_rtc_ord_type      tc_ord;
  caii_orig_c_type       orig_c;
  caii_flash_type        flash;
  caii_status_type       status;
  caii_rev_tc_burst_type tc_burst;
  caii_pil_str_type      pil_str;
  caii_pwr_rpt_type      pwr_rpt;
  caii_sbdtmf_type       sbdtmf;
  caii_parm_rsp_type     parm_rsp;
  caii_comp_type         comp;
  caii_rtc_srv_req_type  srv_req;
  caii_rtc_srv_rsp_type  srv_rsp;
  caii_srv_cmp_type      srv_cmp;
  caii_rtc_srv_ctl_type  srv_ctl;
  caii_tc_strsp_type     tc_strsp;
  caii_ac_tmsi_cmp_type  ac_tmsi_cmp;
  caii_rtc_tmsi_cmp_type tc_tmsi_cmp;
#ifdef FEATURE_IS95B
  caii_ac_paca_can_type  ac_paca_can;
  caii_ac_ext_strsp_type ac_ext_strsp;
  caii_scrm_type         scrm;
  caii_cfs_rsp_type      cfs_rsp;
  caii_cfs_rpt_type      cfs_rpt;
  caii_plt_str_meas_type per_plt;
#endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000
  caii_outer_rpt_type    outer_rpt;
#endif
} caii_tx_msg_type;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#endif /* CAII_H */

