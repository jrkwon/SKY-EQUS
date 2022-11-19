#ifdef FEATURE_OTASP
#ifndef OTASPI_H
#define OTASPI_H
/*===========================================================================

         O T A S P  I N T E R N A L  D A T A   S T R U C T U R E S

DESCRIPTION
  This file contains data structures and constants which represent
  the internal OTASP messages and information used within the Main
  Control and Authentication tasks.

Copyright (c) 1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/otaspi.h_v   1.1   28 Nov 2000 18:17:48   ckrishna  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/15/00   ck      Added support for IS-683A using R-UIM.
03/14/00   ry      Added OTAPA support
10/07/99   ck      Changed FEATURE_JSTD008 to FEATURE_CDMA_1900.
04/16/99   ck      OTASPI_SID_NID_MAX is conditionally compiled to be equal to
                   NV_MAX_HOME_SID_NID or NV_MAX_SID_NID depending on the 
                   target and changed T_AUTH to FEATURE_AUTH
02/15/99   ck      Deleted the structure otaspi_rtc_prl_size_type as it was
                   redundant with otaspi_roam_siz_type
01/21/99   ck      Added support for IS-683A
07/20/98   pms     Added prototypes for otasp_aho_handoff() and
                   otasp_set_aho_handoff().
04/18/97   dna     Added structure definition for ftc Cellular Indicator Data.
09/13/96   dna     Added otasp_report_status().
08/08/96   dna     Added defines for Sprint subsidy lock and pr list dload
06/25/96   dna     Checked in initial version.
04/15/96   day     Created file. 

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "qw.h"
#include "nv.h"
#ifdef FEATURE_DH
#include "dh.h"
#endif /* FEATURE_DH */
#ifdef FEATURE_AUTH
#include "auth.h"
#endif /* FEATURE_AUTH */


/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                      OTASP PARAMETERS                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* ----------------------------------------------------------------
** Maximum number of SID/NID pairs
** ---------------------------------------------------------------- */
#ifdef FEATURE_SSPR_ENHANCEMENTS
#define OTASPI_SID_NID_MAX NV_MAX_HOME_SID_NID
#else
#define OTASPI_SID_NID_MAX NV_MAX_SID_NID
#endif

/* -------------------------------------------------------
** Maximum number of digits in the mobile directory number
** ------------------------------------------------------- */
#define OTASPI_N_DIGITS_MAX 15

/* -------------------------------------------------------
** Maximum number of NAM parameter blocks
** (arbitrarily chosen -- only 6 block ID types available)
** ------------------------------------------------------- */
#define OTASPI_PARAM_BLOCK_MAX 6

/* -------------------------------------------------------
** Maximum number of Validation parameter blocks
** (arbitrarily chosen -- only 3 block ID types available)
** ------------------------------------------------------- */
#define OTASPI_VALIDN_BLOCK_MAX 3

/* -------------------------------------------------------------
** Number of octets of key exchange parameter P (see IS-683 5.2.1)
** Also used as the maximum number of bytes of MS_RESULT and
** BS_RESULT as the result cannot exceed the number of bits in prime.
** ------------------------------------------------------------- */

#define OTASPI_PARAM_P     64

/* -------------------------------------------------------------
** Number of octets of key exchange parameter G (see IS-683 5.2.1)
** ------------------------------------------------------------- */
#define OTASPI_PARAM_G_MAX 20

/* ------------------------------------
** Maximum number of supported features
** ------------------------------------ */ 
#define OTASPI_FEATURE_MAX 5

/* ------------------------------------
** Maximum number of additional fields
** in the protocol capability rsp msg
** ------------------------------------ */
#define OTASPI_MAX_ADDNL_FIELDS  5

#define A_KEY_SIZE               8
/* Number of octets in A-Key */

/* OTASP Feature Protocol Version - See IS-683, Table 3.5.1.7-1 */
#define OTASP_FEATURE_P_REV  1

#ifdef FEATURE_RUIM

/* Number of blocks in a Request message */
#define OTASP_RUIM_MAX_BLOCKS    4

/* This is the maximum number of messages that we can track in the OTASP 
   module */
#define OTASP_RUIM_MAX_MSG_BUFS  5

#endif /* FEATURE_RUIM */

#if ((defined (FEATURE_NSOTASP) && defined(FEATURE_CDMA_1900)) || \
     (!defined (FEATURE_NSOTASP)))
   #define OTASP_CDMA_NAM_RULE
#endif


#ifdef FEATURE_AUTH
extern boolean otasp_reauth_in_progress;
#endif /*FEATURE_AUTH*/

extern boolean akey_commit;
  /* Flag to indicate if the akey has been committed */

#ifdef FEATURE_DH
/* Internal OTASP processing states for Diffie-Helman exponentiation */
typedef enum
{
   OTASP_WAIT_FOR_DH,         /* Awaiting Diffie-Helman initiation           */
   OTASP_WAIT_FOR_MS_RESULT,  /* Awaiting the result of first exponentiation */
                              /*  which is the MS_RESULT                     */
   OTASP_WAIT_FOR_KEY_GEN_REQ,    /* Awaiting Key Generation Request Msg in order*/
                              /*  to start the second exponentiation         */
   OTASP_WAIT_FOR_A_KEY       /* Awaiting result of the second exponentiation*/
                              /*  which is the A-Key                         */
}  otasp_dh_state_type;
#endif /* FEATURE_DH */

#ifdef FEATURE_DH
typedef struct
{
   dword                param_g[OTASPI_PARAM_G_MAX/sizeof(dword)];
   dword                private_key[DH_RAND/sizeof(dword)];
   dword                param_p[OTASPI_PARAM_P/sizeof(dword)];
   dword                ms_result[OTASPI_PARAM_P/sizeof(dword)];
   qword                a_key;
   otasp_dh_state_type  state;
}  otasp_dh_data;
#endif /* FEATURE_DH */

/* Flag to indicate the result of Akey commit operation by NV */
extern boolean otasp_akey_commit_result;

typedef enum
{
  NO_SRV_PROV
  , USER_INIT_SRV_PROV
#ifdef FEATURE_OTASP_OTAPA
  , NETWORK_INIT_SRV_PROV
#endif /* FEATURE_OTASP_OTAPA */
} otasp_call_type;

#ifdef FEATURE_RUIM
typedef struct {
  byte num_features;
  byte feature_id[OTASPI_FEATURE_MAX];
  byte feature_p_rev[OTASPI_FEATURE_MAX];
} ruim_otasp_features_type;
#endif /* FEATURE_RUIM */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                 MACRO DEFINITIONS                                       */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define ACTIVATE_CODE_TO_BAND_CLASS(x) \
    (((x)==ABAND || (x) == BBAND)?CAI_BAND_CELL:CAI_BAND_PCS)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*               INTERNAL TYPES USED FOR OTASP MESSAGES                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Internal message formats are used to represented OTASP message data on  */
/* byte-aligned boundaries.  Bit-packing and bit-unpacking routines        */
/* utilize these definitions to translate OTASP message data to and from   */
/* bit-packed formats.                                                     */

/* -----------------------------------------------
** Generic type to isolate OTASP Data Message type
** ----------------------------------------------- */
typedef struct
{
  byte msg_type;         /* OTASP Data Message type */
} otaspi_gen_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*            OTASP REVERSE LINK PARAMETER BLOCK TYPES                     */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CDMA/Analog NAM Parameter Block (see IS-683 section 3.5.2.1)    */

typedef struct
{
  word  firstchp;         /* First paging channel used in home system */
  word  home_sid;         /* Home system identification */
  byte  ex;               /* Extended address indicator */
  byte  scm;              /* Station class mark */
  byte  mob_p_rev;        /* Mobile station protocol revision number */
  byte  imsi_class;       /* IMSI Class assignment of the mobile station */
  byte  imsi_addr_num;    /* Number of IMSI address digits */
  word  mcc;              /* Mobile country code */
  byte  imsi_11_12;       /* 11th and 12th digits of IMSI */
  qword imsi_s;           /* The least significant ten digits of the IMSI */
  byte  accolc;           /* Access overload class */
  byte  local_control;    /* Local control status */
  byte  mob_term_home;    /* Termination indicator for the home system */
  byte  mob_term_for_sid; /* Termination indicator for SID roaming */
  byte  mob_term_for_nid; /* Termination indicator for NID roaming */
  byte  max_sid_nid;      /* Maximum stored SID/NID pairs */
  byte  stored_sid_nid;   /* Number of stored SID/NID pairs */
  struct
  {
    word sid;             /* System identification  */
    word nid;             /* Network identification */
  } var[OTASPI_SID_NID_MAX];
} otaspi_rtc_cell_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Mobile Directory Number (see IS-683 section 3.5.2.2)                    */

typedef struct
{
  byte n_digits;                     /* Number of digits */
  byte digitn[OTASPI_N_DIGITS_MAX];  /* Digits (4 bit DTMF in 8 bit field */
} otaspi_mob_dir_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* IMSI_T Parameter Block  (see IS-683 section 3.5.2.4)                    */
typedef struct
{
   byte  imsi_t_class;
   byte  imsi_t_addr_num;
   word  imsi_t_mcc;
   byte  imsi_t_11_12;
   qword imsi_t_s;
} otaspi_imsi_t_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CDMA NAM Parameter Block (see IS-683 section 3.5.2.3)         */

typedef struct
{
  byte  slotted_mode;     /* Slotted mode */
  byte  mob_p_rev;        /* Mobile station protocol revision number */
  byte  imsi_class;       /* IMSI Class assignment of the mobile station */
  byte  imsi_addr_num;    /* Number of IMSI address digits */
  word  mcc;              /* Mobile country code */
  byte  imsi_11_12;       /* 11th and 12th digits of IMSI */
  qword imsi_s;           /* The least significant ten digits of the IMSI */
  byte  accolc;           /* Access overload class */
  byte  local_control;    /* Local control status */
  byte  mob_term_home;    /* Termination indicator for the home system */
  byte  mob_term_for_sid; /* Termination indicator for SID roaming */
  byte  mob_term_for_nid; /* Termination indicator for NID roaming */
  byte  max_sid_nid;      /* Maximum stored SID/NID pairs */
  byte  stored_sid_nid;   /* Number of stored SID/NID pairs */
  struct
  {
    word sid;             /* System identification  */
    word nid;             /* Network identification */
  } var[OTASPI_SID_NID_MAX];
} otaspi_rtc_pcs_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Preferred Roaming List Dimensions                                       */
#ifdef FEATURE_NSOTASP
typedef struct
{
  word  max_pr_list_size; /* Max Preferred Roaming List Size */
  word  cur_pr_list_size; /* Current Preferred Roaming List Size */
  byte  num_acq_recs;     /* Number of Acquisition Records in current RL */
  byte  num_sys_recs;     /* Number of System Records in current RL */
} otaspi_roam_siz_type;
#else
/* Preferred Roaming List Dimensions    (see IS-683A section 3.5.3.1)      */
typedef struct
{
  word  max_pr_list_size; /* Max Preferred Roaming List Size */
  word  cur_pr_list_size; /* Current Preferred Roaming List Size */
  word  pr_list_id;       /* PRL Identification        */
  byte  reserved;         /* Reserved                  */
  word  num_acq_recs;     /* Number of Acquisition Records in current RL */
  word  num_sys_recs;     /* Number of System Records in current RL */
} otaspi_roam_siz_type;
#endif /* FEATURE_NSOTASP */

#ifdef FEATURE_RUIM
/* Data blocks for the R-UIM */
typedef byte otaspi_ruim_data_type[255];
#endif /* FEATURE_RUIM */

/* Parameter block */
typedef struct
{
  byte block_id;   /* Parameter block identifier */
#ifndef FEATURE_NSOTASP
  byte block_len;
#endif
  /* block_len intentionally ommitted - will be calculated in otaspx.c */
  union
  {
    otaspi_rtc_cell_data_type rtc_cell;   /* RTC CDMA/Analog NAM Parameter  */
    otaspi_mob_dir_data_type  mob_dir;    /* Mobile Directory Number     */
    otaspi_rtc_pcs_data_type  rtc_pcs;    /* RTC CDMA NAM Parameter         */
#ifndef FEATURE_NSOTASP
    otaspi_imsi_t_data_type   imsi_t;     /* IMSI_T Parameter block         */
#endif
    otaspi_roam_siz_type      roam_siz;   /* Preferred Roaming List Size */
#ifdef FEATURE_RUIM
    otaspi_ruim_data_type     ruim_rsp_data; /* Response from R-UIM */
#endif /* FEATURE_RUIM */
  } blk;
} otaspi_rtc_param_blk_type;

/* Preferred Roaming List                 (see IS-683 section 3.5.3.2)     */
typedef struct
{
  byte last_segment;     /* Last segment indicator bit */
  word segment_offset;   /* Segment offset */
  byte segment_size;     /* Segment size */
  byte *bits_ptr;        /* Pointer to the Segment data */
} otaspi_rtc_sspr_prl_type;

/* SSPR Parameter blocks */
typedef struct {
   union
   {
      otaspi_roam_siz_type     prl_size;
      otaspi_rtc_sspr_prl_type sspr_prl;   /* Preferred Roaming List */
#ifdef FEATURE_RUIM
      otaspi_ruim_data_type    ruim_rsp_data; /* Response from R-UIM */
#endif /* FEATURE_RUIM */
   } blk;
} otaspi_rtc_sspr_param_blk_type;


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                REVERSE LINK OTASP MESSAGES                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CONFIGURATION RESPONSE MESSAGE (see IS-683 section 3.5.1.1)             */

typedef struct
{
  byte msg_type;       /* OTASP Data Message type    */
  byte num_blocks;     /* Number of parameter blocks */
  otaspi_rtc_param_blk_type block[OTASPI_PARAM_BLOCK_MAX]; /* Param data */
#ifndef FEATURE_NSOTASP
  byte result_code[OTASPI_PARAM_BLOCK_MAX]; /* Result code */
#endif /* FEATURE_NSOTASP */
} otaspi_config_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DOWNLOAD RESPONSE MESSAGE (see IS-683 section 3.5.1.2)                  */

typedef struct
{
  byte msg_type;                 /* OTASP Data Message type    */
  byte num_blocks;               /* Number of parameter blocks */
  struct
  {
    byte block_id;               /* Parameter block identifier */
    byte result_code;            /* Download result code       */
  } block[OTASPI_PARAM_BLOCK_MAX]; /* Parameter block results    */
} otaspi_download_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* MS KEY RESPONSE MESSAGE (see IS-683 section 3.5.1.3)                    */

typedef struct
{
  byte msg_type;                     /* OTASP Data Message type   */
  byte result_code;                  /* Key exchange result code  */
} otaspi_ms_key_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* KEY GENERATION RESPONSE MESSAGE (see IS-683 section 3.5.1.4)            */

typedef struct
{
  byte msg_type;       /* OTASP Data Message type   */
  byte result_code;    /* Key exchange result code  */
  byte ms_result_len;  /* Length of MS_RESULT field */
  dword ms_result[OTASPI_PARAM_P/sizeof(dword)]; /* Mobile station calculation result */
} otaspi_key_gen_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* RE-AUTHENTICATE RESPONSE MESSAGE (see IS-683 section 3.5.1.5)           */

typedef struct
{
  byte  msg_type;       /* OTASP Data Message type             */
  dword authr;          /* Authentication signature data       */
  byte  randc;          /* Random challenge value              */
  byte  count;          /* Call history parameter              */
  dword auth_data;      /* Authentication Data input parameter */
} otaspi_reauth_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* COMMIT RESPONSE MESSAGE (see IS-683 section 3.5.1.6)                    */

typedef struct
{
  byte msg_type;       /* OTASP Data Message type */
  byte result_code;    /* Data commit result code */
} otaspi_commit_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PROTOCOL CAPABILITY RESPONSE MESSAGE (see IS-683 section 3.5.1.7)       */

typedef struct
{
  byte msg_type;        /* OTASP Data Message type                    */
  word mob_firm_rev;    /* Mobile station firmware revision number    */
  byte mob_model;       /* Mobile station manufacturer's model number */
  byte num_features;    /* Number of features                         */
  struct
  {
    byte feature_id;    /* Feature identifier       */
    byte feature_p_rev; /* Feature protocol version */
  } var[OTASPI_FEATURE_MAX];
#ifndef FEATURE_NSOTASP
  byte add_length;
  byte addnl_fields[OTASPI_MAX_ADDNL_FIELDS];
#endif
} otaspi_prot_cap_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SSPR CONFIGURATION RESPONSE MESSAGE (see IS-683A section 3.5.1.8)       */
typedef struct
{
   byte msg_type;        /* OTASP Data Message type                    */
   byte block_id;        /* Parameter Block Identifier                 */
   byte result_code;     /* SSPR Configuration Result Code             */
   byte block_len;       /* Parameter block length                     */
   otaspi_rtc_sspr_param_blk_type block;
                         /* SSPR parameter block type                  */
} otaspi_sspr_cfg_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SSPR DOWNLOAD RESPONSE MESSAGE (see IS-683A section 3.5.1.9)       */
typedef struct
{
   byte msg_type;        /* OTASP Data Message type      */
   byte block_id;        /* Parameter Block Identifier   */
   byte result_code;     /* SSPR Download Result Code    */
   word segment_offset;  /* Segment offset               */
   byte segment_size;    /* Segment size                 */
} otaspi_sspr_dload_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* VALIDATION RESPONSE MESSAGE         (see IS-683A section 3.5.1.10)      */
typedef struct
{
   byte msg_type;        /* OTASP Data Message type      */
   byte num_blocks;      /* Number of parameter blocks   */
   struct
   {
    byte block_id;        /* Parameter block identifier  */
    byte result_code;     /* Download result code        */
   } block[OTASPI_VALIDN_BLOCK_MAX]; /* Parameter block results    */
} otaspi_validation_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* OTAPA RESPONSE MESSAGE              (see IS-683A section 3.5.1.11)      */
typedef struct
{
   byte     msg_type;        /* OTASP Data Message type      */
   byte     result_code;     /* SSPR Download Result Code    */
   byte     nam_lock_ind;    /* NAM LOCK Indicator           */
   dword    rand_otapa;      /* SPASM Random challenge       */
}  otaspi_otapa_rsp_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*            OTASP FORWARD LINK PARAMETER BLOCK TYPES                     */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CDMA/Analog NAM Parameter Block (see IS-683 section 4.5.2.1)    */

typedef struct
{
  word  firstchp;         /* First paging channel used in home system */
  word  home_sid;         /* Home system identification */
  byte  ex;               /* Extended address indicator */
  byte  imsi_class;       /* IMSI Class assignment of the mobile station */
  byte  imsi_addr_num;    /* Number of IMSI address digits */
  word  mcc;              /* Mobile country code */
  byte  imsi_11_12;       /* 11th and 12th digits of IMSI */
  qword imsi_s;           /* The least significant ten digits of the IMSI */
  byte  accolc;           /* Access overload class */
  byte  local_control;    /* Local control status */
  byte  mob_term_home;    /* Termination indicator for the home system */
  byte  mob_term_for_sid; /* Termination indicator for SID roaming */
  byte  mob_term_for_nid; /* Termination indicator for NID roaming */
  byte  n_sid_nid;        /* Number of SID/NID pairs */
  struct
  {
    word sid;             /* System identification  */
    word nid;             /* Network identification */
  } var[OTASPI_SID_NID_MAX];
} otaspi_ftc_cell_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Mobile Directory Number (see IS-683 section 4.5.2.2)                    */

/* Note: Use otaspi_mob_dir_data_type for Mobile Directory Number data */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CDMA NAM Parameter Block (see IS-683 section 4.5.2.3)         */

typedef struct
{
  byte  imsi_class;       /* IMSI Class assignment of the mobile station */
  byte  imsi_addr_num;    /* Number of IMSI address digits */
  word  mcc;              /* Mobile country code */ 
  byte  imsi_11_12;       /* 11th and 12th digits of IMSI */
  qword imsi_s;           /* The least significant ten digits of the IMSI */
  byte  accolc;           /* Access overload class */
  byte  local_control;    /* Local control status */
  byte  mob_term_home;    /* Termination indicator for the home system */
  byte  mob_term_for_sid; /* Termination indicator for SID roaming */
  byte  mob_term_for_nid; /* Termination indicator for NID roaming */
  byte  n_sid_nid;        /* Number of SID/NID pairs */
  struct
  {
    word sid;             /* System identification  */
    word nid;             /* Network identification */
  } var[OTASPI_SID_NID_MAX];
} otaspi_ftc_pcs_data_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Preferred Roaming List                                                  */

typedef struct
{
  byte  seq_num;         /* Sequence number for this piece of Roam List */
  word  num_bits;        /* Number of bits in this piece of Roam List */
  byte *bits_ptr;        /* Pointer into caii_rx_msg_type where BITS begins */
} otaspi_ftc_roam_lst_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Service Programming Code                                                */

typedef byte otaspi_ftc_spc_type[NV_SEC_CODE_SIZE]; /* Holds SPC digits */

/* NAM Parameter blocks */
typedef struct
{
  byte block_id;   /* Parameter block identifier */
  byte block_len;  /* Parameter block length     */
  union
  {
    otaspi_mob_dir_data_type  mob_dir;   /* Mobile Directory Number    */
    otaspi_ftc_pcs_data_type  ftc_pcs;   /* FTC PCS Download Data      */
    otaspi_ftc_cell_data_type ftc_cell;  /* FTC Cellular Download Data */
#ifndef FEATURE_NSOTASP
    otaspi_imsi_t_data_type   imsi_t;    /* IMSI_T Parameter block     */
#endif /* FEATURE_NSOTASP */
    otaspi_ftc_roam_lst_type  ftc_roam;  /* Preferred Roaming List     */
    otaspi_ftc_spc_type       ftc_spc;   /* Service Programming Code   */
#ifdef FEATURE_RUIM
    otaspi_ruim_data_type     ruim_data; /* Data for the R-UIM */
#endif /* FEATURE_RUIM */
  } blk;
} otaspi_ftc_param_blk_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Preferred Roaming List as per IS-683A  (see IS-683 section 4.5.3.1)     */
typedef struct
{
  byte last_segment;  /* Last segment indicator bit */
  word segment_offset;   /* Segment offset */
  byte segment_size;     /* Segment size */
  byte *bits_ptr;        /* Pointer to the Segment data */
} otaspi_ftc_sspr_prl_type;

/* SSPR Parameter blocks */
typedef struct {
   union
   {
      otaspi_ftc_sspr_prl_type sspr_prl;   /* Preferred Roaming List */
#ifdef FEATURE_RUIM
      byte prl_data[255];                  /* Unpacked PRL Data for R-UIM */
#endif /* FEATURE_RUIM */
   } blk;
} otaspi_ftc_sspr_param_blk_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Verify SPC Block  (see IS-683 section 4.5.4.1)                          */
typedef struct
{
  byte spc[NV_SEC_CODE_SIZE];              /* 6 digit SPC */
} otaspi_ftc_verify_spc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Change SPC Block  (see IS-683 section 4.5.4.2)                          */
typedef struct
{
  byte spc[NV_SEC_CODE_SIZE];              /* 6 digit SPC */
} otaspi_ftc_change_spc_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Verify SPASM Block  (see IS-683 section 4.5.4.3)                        */
typedef struct
{
  byte auth_otapa_incl;    /* OTAPA SPASM validation signature included */
                           /* indicator                                 */
  dword auth_otapa;     /* OTAPA SPASM validation signature          */
  byte nam_lock;           /* SPASM Protection indicator for active NAM */
} otaspi_ftc_validate_spasm_type;

/* Validation Parameter blocks  */
typedef struct
{
   byte block_id;        /* Parameter block identifier */
   byte block_len;       /* Parameter block length     */
   union
   {
     otaspi_ftc_verify_spc_type     verify_spc;     /* Verify SPC     */
     otaspi_ftc_change_spc_type     change_spc;     /* Change SPC     */
     otaspi_ftc_validate_spasm_type validate_spasm; /* Validate SPASM */
#ifdef FEATURE_RUIM
     otaspi_ruim_data_type          validn_data;
#endif /* FEATURE_RUIM */
   }blk;
} otaspi_ftc_valid_param_blk_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                FORWARD LINK OTASP MESSAGES                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CONFIGURATION REQUEST MESSAGE (see IS-683 section 4.5.1.1)              */

#define OTASPI_NUM_BLOCKS_MAX 255
  /* Max number of blocks is constrained by size of the num_blocks field */

typedef struct
{
  byte msg_type;       /* OTASP Data Message type              */
  byte num_blocks;     /* Number of parameter blocks requested */
  byte block_id[OTASPI_NUM_BLOCKS_MAX]; /* Parameter block identifier */
} otaspi_config_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* DOWNLOAD REQUEST MESSAGE (see IS-683 section 4.5.1.2)                   */

typedef struct
{
  byte msg_type;       /* OTASP Data Message type    */
  byte num_blocks;     /* Number of parameter blocks */
  otaspi_ftc_param_blk_type block[OTASPI_PARAM_BLOCK_MAX]; /* Param data */
} otaspi_download_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* COMMIT REQUEST MESSAGE (see IS-683 section 4.5.1.3)                     */

typedef struct
{
  byte msg_type;       /* OTASP Data Message type */
} otaspi_commit_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* RE-AUTHENTICATE REQUEST MESSAGE (see IS-683 section 4.5.1.4)            */

typedef struct
{
  byte  msg_type;      /* OTASP Data Message type */
  dword rand;          /* Random Challenge value  */
} otaspi_reauth_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* MS KEY REQUEST MESSAGE (see IS-683 section 4.5.1.5)                     */

typedef struct
{
  byte msg_type;                    /* OTASP Data Message type       */
  byte a_key_p_rev;                 /* Key Exchange protocol version */
  byte param_p_len;                 /* Length of PARAM_P field       */
  dword param_p[OTASPI_PARAM_P/(sizeof(dword))];
                                    /* Key exchange parameter P in dwords*/
  byte param_g_len;                 /* Length of PARAM_G field       */
  dword param_g[OTASPI_PARAM_G_MAX/(sizeof(dword))];
                                    /* Key exchange parameter G in dwords*/
} otaspi_ms_key_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* KEY GENERATION REQUEST MESSAGE (see IS-683 section 4.5.1.6)             */

typedef struct
{
  byte msg_type;                     /* OTASP Data Message type         */
  byte bs_result_len;                /* Length of BS_RESULT field       */
  dword bs_result[OTASPI_PARAM_P/(sizeof(dword))]; /* Base station calculation result */
} otaspi_key_gen_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* PROTOCOL CAPABILITY REQUEST MESSAGE (see IS-683 section 4.5.1.7)        */

typedef struct
{
  byte msg_type;       /* OTASP Data Message type   */
} otaspi_prot_cap_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SSPR CONFIGURATION REQUEST MESSAGE (see IS-683 section 4.5.1.8)         */
typedef struct
{
   byte msg_type;             /* OTASP Data message type */
   byte block_id;             /* Parameter block identifier */
   word req_offset;           /* Segment offset */
   byte req_max_size;         /* Maximum segment size */
} otaspi_sspr_cfg_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SSPR DOWNLOAD REQUEST MESSAGE (see IS-683 section 4.5.1.9)              */
typedef struct
{
   byte msg_type;             /* OTASP Data message type    */
   byte block_id;             /* Parameter block identifier */
   byte block_len;            /* Parameter block length     */
   otaspi_ftc_sspr_param_blk_type block;
                              /* SSPR Parameter block type  */
} otaspi_sspr_dload_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* VALIDATION REQUEST MESSAGE (see IS-683 section 4.5.1.10)                */
typedef struct
{
   byte msg_type;             /* OTASP Data message type    */
   byte num_blocks;           /* Number of parameter blocks */
   otaspi_ftc_valid_param_blk_type block[OTASPI_VALIDN_BLOCK_MAX];
                              /* Validation Param data      */
} otaspi_validation_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* OTAPA REQUEST MESSAGE      (see IS-683 section 4.5.1.11)                */
typedef struct
{
  byte msg_type;              /* OTASP Data message type            */
  byte start_stop;         /* OTAPA session start-stop indicator */
} otaspi_otapa_req_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*   Union of Reverse Traffic Channel OTASP Data Messages                  */

typedef union
{
  otaspi_gen_msg_type          gen;
  otaspi_config_rsp_msg_type   config_rsp;  /* Configuration Response       */
  otaspi_download_rsp_msg_type dload_rsp;   /* Download Response            */
  otaspi_ms_key_rsp_msg_type   ms_key_rsp;  /* MS Key Response              */
  otaspi_key_gen_rsp_msg_type  key_gen_rsp; /* Key Generation Response      */
  otaspi_reauth_rsp_msg_type   reauth_rsp;  /* Re-Authentication Response   */
  otaspi_commit_rsp_msg_type   commit_rsp;  /* Commit Response              */
  otaspi_prot_cap_rsp_msg_type cap_rsp;     /* Protocol Capability Response */
  otaspi_sspr_cfg_rsp_msg_type   sspr_cfg_rsp;/* SSPR Config Response      */
  otaspi_sspr_dload_rsp_msg_type sspr_dload_rsp;  /* SSPR Download Response */
  otaspi_validation_rsp_msg_type validn_rsp;  /* Validation Resposne    */
  otaspi_otapa_rsp_msg_type      otapa_rsp;   /* OTAPA Respomse            */
} otaspi_rtc_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*   Union of Forward Traffic Channel OTASP Data Messages                  */

typedef union
{
  otaspi_gen_msg_type          gen;
  otaspi_config_req_msg_type   config_req;   /* Configuration Request       */
  otaspi_download_req_msg_type dload_req;    /* Download Request            */
  otaspi_commit_req_msg_type   commit_req;   /* Commit Request              */
  otaspi_reauth_req_msg_type   reauth_req;   /* Re-Authenticate Request     */
  otaspi_ms_key_req_msg_type   ms_key_req;   /* MS Key Request              */
  otaspi_key_gen_req_msg_type  key_gen_req;  /* Key Generation Request      */
  otaspi_prot_cap_req_msg_type prot_cap_req; /* Protocol Capability Request */
  otaspi_sspr_cfg_req_msg_type   sspr_cfg_req; /* SSPR Config Request         */
  otaspi_sspr_dload_req_msg_type sspr_dload_req;  /* SSPR Download Request    */
  otaspi_validation_req_msg_type validn_req;   /* Validation Request       */
  otaspi_otapa_req_msg_type      otapa_req;    /* OTAPA Request               */
} otaspi_ftc_msg_type;

#ifdef FEATURE_RUIM
typedef struct
{
  byte msg_type;             /* OTASP Msg Type */
  byte num_blocks_req_msg;   /* Number of blocks in the request message */
  byte num_blocks_req_card;  /* Number of block requests sent to the card */
  byte num_blocks_rsp_card;  /* Number of blocks responded to by the card */
  byte rej_block_id[OTASP_RUIM_MAX_BLOCKS]; 
                             /* ID of blocks not sent to the card */
} otasp_ruim_msg_info_type;

typedef struct
{
  otasp_ruim_msg_info_type  msg[OTASP_RUIM_MAX_MSG_BUFS];
  byte req_index;
  byte rsp_index;
} otasp_ruim_req_rsp_table_type;
#endif /* FEATURE_RUIM */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_PROCESS_MSG

DESCRIPTION
  This function processes an OTASP message.

DEPENDENCIES
  None.

RETURN VALUE
  Next state to be processed.  CDMA_CONV or CDMA_EXIT for powerdown.

SIDE EFFECTS
  None.

===========================================================================*/

extern word otasp_process_msg 
(
  otaspi_ftc_msg_type *otasp_msg_ptr
    /* OTASP message to be processed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_PROCESS_COMMIT

DESCRIPTION
  This function processes an OTASP Commit, after NV has finished processing
  the NV_OTASP_COMMIT_F command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void otasp_process_commit (void);

/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_INIT

DESCRIPTION
  This function initializes the OTASP function.  Should be called when
  entering CDMA_CONV substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void otasp_init (void);

/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_RELOAD_NAM_PARAMS

DESCRIPTION
   This function updates the cdma structure with newly cmmitted info

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void otasp_reload_nam_params (void) ;

#ifdef FEATURE_DH_EXP
/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_PROCESS_DH_RPT

DESCRIPTION
  This function processes a report from the Authentication task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void otasp_process_dh_report ( dh_rpt_type *dh_rpt_ptr); /* Pointer to dh report */
#endif /* FEATURE_DH */

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_MS_KEY_REQ

DESCRIPTION
  This function processes an OTASP MS Key Request Message,
  formats the appropriate MS Key Response Message, then
  calls the routine to send it.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void process_ms_key_req
(
  otaspi_ms_key_req_msg_type* ms_key_req_ptr
    /* Pointer to MS Key Request Message */
);

#ifdef FEATURE_AUTH
/* <EJECT> */
/*===========================================================================
FUNCTION OTASP_PROCESS_AUTH_REPORT

DESCRIPTION
   This function processes the report received from Auth task while
   reauthentication is in progress or auth_otapa is in progress.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void otasp_process_auth_report
(
   auth_rpt_type* otasp_auth_rpt
   /* Pointer to auth report type */
);
#endif

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_MS_KEY_RESP_MSG

DESCRIPTION
   This function send the MS Key Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_ms_key_resp_msg
(
   void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_KEY_GEN_RESP_MSG

DESCRIPTION
   This function sends the Key Genration Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_key_gen_resp_msg
(
  void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_REAUTH_RESP_MSG

DESCRIPTION
   This function sends the Re-Authentication Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_reauth_resp_msg
(
void
);

/* <EJECT> */
/*===========================================================================

FUNCTION BUILD_REAUTH_RESP_MSG

DESCRIPTION
   This function builds the Re-Authentication Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void build_reauth_resp_msg 
(
void 
);

#ifdef FEATURE_RUIM 

/* <EJECT> */
/*===========================================================================
FUNCTION OTASP_PROCESS_RUIM_REPORT

DESCRIPTION
   This function processes the report received from Auth/RUIM task for OTASP
   related messages.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
word otasp_process_ruim_report
(
   auth_rpt_type* otasp_ruim_rpt,
   /* Pointer to auth report type */
   word next_state
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_COMMIT_RESP_MSG

DESCRIPTION
   This function send the Commit Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_commit_resp_msg
(
   void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_OTAPA_RESP_MSG

DESCRIPTION
   This function sends the OTAPA Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_otapa_resp_msg
(
   void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_CFG_RESP_MSG

DESCRIPTION
   This function sends the Configuration Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_cfg_resp_msg
(
   void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_SSPR_CFG_RESP_MSG

DESCRIPTION
   This function sends the SSPR Configuration Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_sspr_cfg_resp_msg
(
    void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_DLOAD_RESP_MSG

DESCRIPTION
   This function sends the Download Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_dload_resp_msg
(
    void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_SSPR_DLOAD_RESP_MSG

DESCRIPTION
   This function sends the SSPR Download Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_sspr_dload_resp_msg
(
    void
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_VALIDN_RESP_MSG

DESCRIPTION
   This function sends the Validation Response Message

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void send_validn_resp_msg
(
    void
);

#endif /* FEATURE_RUIM */

/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_AHO_HANDOFF

DESCRIPTION
  This function returns whether or not this OTASP call has been handed off
  to an AMPS system.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if current OTASP call has been handed off to an AMPS system,
  FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean otasp_aho_handoff (void);

/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_SET_AHO_HANDOFF

DESCRIPTION
  This function allows write access to the OTASP AMPS handoff flag.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if current OTASP call has been handed off to an AMPS system,
  FALSE otherwise.

SIDE EFFECTS
  Sets the value of otasp_call_ho_from_cdma.

===========================================================================*/

extern void otasp_set_aho_handoff (boolean value);

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_OTAPA_REQ

DESCRIPTION
  This function processes an OTAPA Request Message, formats the
  appropriate Configuration Response Message, then calls the
  routine to send it.

DEPENDENCIES
  None.

RETURN VALUE
  Next state to return. CDMA_CONV or CDMA_EXIT for power down.

SIDE EFFECTS
  None.

===========================================================================*/
word process_otapa_req
(
  otaspi_otapa_req_msg_type* otapa_req_ptr
    /* Pointer to OTAPA Request Message */
);

#ifdef FEATURE_OTASP_OTAPA
/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_GET_OTAPA_SESSION

DESCRIPTION
  This function returns whether or not an OTAPA session has taken place

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if an OTAPA session has taken place
  FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean otasp_get_otapa_session
(
  void
);


/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_OTAPA_INIT

DESCRIPTION
  This function initializes the OTAPA function.  Should be called when
  an OTAPA session starts.

  It does not initialize the following flags as otasp_init does:
  otasp_successful_commit
  otasp_otapa_session_occurred

  It sets nam_locked_state to be cdma.nam_lock


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void otasp_otapa_init
(
  void
);


#endif /* FEATURE_OTASP_OTAPA */



/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_REPORT_COMMIT_STATUS_TO_CM

DESCRIPTION
  The function sends a commit status command to CM

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void otasp_report_commit_status_to_cm
(
  boolean commit_in_progress
    /* Flag if an OTAPA session is in progress */
);


/*===========================================================================

FUNCTION OTASP_IS_SUCCESSFUL_COMMIT

DESCRIPTION
  The function determines if an OTASP commit is successful or not.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if an OTASP commit is successful
  FALSE, else.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean otasp_is_successful_commit
(
  void
);

#ifdef FEATURE_RUIM
/* <EJECT> */
/*===========================================================================

FUNCTION OTASP_SET_RUIM_NAM_LOCK_STATE

DESCRIPTION
  This function indicates the nature of the otasp call and sets
  the nam lock state in the R-UIM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void otasp_set_ruim_nam_lock_state
(
   otasp_call_type ota_call_type
);

#endif /* FEATURE_RUIM */
#endif /* OTASPI_H */
#endif /* FEATURE_OTASP */
