#ifndef AUTH_H
#define AUTH_H
/*===========================================================================

               E X T E R N A L   A U T H E N T I C A T I O N
                      T A S K   D E F I N I T I O N S

DESCRIPTION
  This contains all the declarations for the Authentication Task.

Copyright (c) 1995, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/auth.h_v   1.5   18 Dec 2000 20:01:58   ckrishna  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/18/00   ck      Changed the request offset in the SSPR Configuration Request
                   Interface to a word from byte.
11/28/00   ck      Added definitions to support IS-683A with the R-UIM.
11/14/00   ts      Added access to the MSISDN in the CDMA directory.
10/11/00   ts      Moved the RUIM status outside of the rpt union field into its 
                   own field.
07/16/00   ck      Added CDMA DF definition.
06/12/00   ck      Added definitions for checks related to PPS and access of
                   records from RUIM.
05/30/00   ck      Changed RUIM_UI_CHV_SIG to RUIM_CHV_SIG and also removed
                   RUIM_MAX_CHV_DIGITS as it was defined in ruim.h
05/26/00   ck      Added a comma that caused the compile to break
05/01/00   ck      Added support for RUIM software. Included the new interfaces
                   for RUIM software and featurised it under FEATURE_RUIM.
                   AUTH only interface is featurised under FEATURE_AUTH to give
                   the flexibility of defining only FEATURE_AUTH or 
                   FEATURE_RUIM.
04/06/00   va      Merged the following from MSM3100_CP.03.00.17
           ry      Added OTAPA support
05/06/99   kmp     Changed T_ACP to FEATURE_ACP
03/18/99   ck      Merged the support for A-Key exchange and Re-Authentication
09/04/98   pms     Neared the variables to save ROM space
07/28/98   dna     Retain message encryption on CDMA->AMPS handoff by
                   separating auth_reset from auth_resync.
11/17/95   dna     Report back when keys are generated.
08/03/95   dna     Added support for message encryption
06/07/95   dna     Created Module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "queue.h"
#include "qw.h"
#include "cmd.h"
#include "cai.h"
#include "caii.h"
#include "ulpn.h"
#include "nv.h"
#ifdef FEATURE_ACP
#include "acp553.h"
#endif /* FEATURE_ACP */
#ifdef FEATURE_RUIM
#include "ruim.h"
#endif /* FEATURE_RUIM */
/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_AUTH
#define AUTH_A_KEY_DIGITS         26
  /* Number of digits in the A-key */
#endif /* FEATURE_AUTH */
  
#ifdef FEATURE_RUIM

#define RUIM_CMD_RSP_TIME         2000
#define RUIM_COMMIT_RSP_TIME      30000
#define RUIM_POLL_TIME            30000
#define RUIM_INSTRUCTION_CLASS    0xA0
  /* Class of instruction (ISO/IEC 7816-3) */ 

#define RUIM_MAX_NUM_ATTEMPTS     3

#ifdef FEATURE_AUTH
#define RUIM_RUN_CAVE_DIGITS      3
#define RUIM_GLOBAL_RAND_CHAL     0x00
#define RUIM_UNIQUE_RAND_CHAL     0x01

  /* Process control bits */     
#define RUIM_DEFLT_PROC_CNTRL     0x00
#define RUIM_SAVE_REGS_ON         0x10
#define RUIM_COMMIT_NEW_SSD       0x04

#define RUIM_FIRST_OCTET_OF_VPM   1
#define RUIM_LAST_OCTET_OF_VPM    65    /* 520 bits of VPM */   
#endif /* FEATURE_AUTH */

#define RUIM_MAX_ATR_CHARS        33
#define RUIM_CHV_PAD_CHAR         0xFF
#define RUIM_MAX_PPS_CHARS        6
#define RUIM_PPS_DEFAULT_PPSS     0xFF
#define RUIM_PPS_DEFAULT_PPS0     0
#define RUIM_PPS_DEFAULT_PCK      0xFF

/* Definitions to check if we need to send a PPS based on the ATR */                                         
#define RUIM_ATR_T0_BYTE          1      
#define RUIM_ATR_TA1_PRESENT      0X10   
#define RUIM_ATR_TA1_BYTE         2      
#define RUIM_NO_PPS_VALUE1        0X11   
#define RUIM_NO_PPS_VALUE2        0X01   

#define RUIM_P2_NEXT_RECORD_MODE  2
#define RUIM_P2_PREV_RECORD_MODE  3
#define RUIM_P2_ABS_RECORD_MODE   4

#define RUIM_CDMA_DF              0x7F25

#ifdef FEATURE_OTASP
#define RUIM_NAM_LOCK_STATE_MASK  0x1
#define RUIM_NAM_LOCK_OTA_MASK    0x4
#define RUIM_NL_STATE_LOCK        0x1
#define RUIM_NL_STATE_UNLOCK      0x0  
#define RUIM_OTA_OTAPA            0x4
#define RUIM_OTA_OTASP            0x0
#define RUIM_OTA_NAM_LOCK         0x2
#define RUIM_NAM_LOCK_BITS        0x7
#endif /* FEATURE_OTASP */

#define RUIM_CC_MASK              0x3F
#endif /* FEATURE_RUIM */

/*--------------------------------------------------------------------------
                             Signal Masks
--------------------------------------------------------------------------*/
#define  AUTH_RPT_TIMER_SIG      0x0001
  /* This signal is set when the auth_rpt_timer expires.
     This timer is used to enable AUTH to kick the watchdog on time */

#define  AUTH_CMD_Q_SIG          0x0002
  /* This signal is set when an item is put on the auth_cmd_q. */
#ifdef FEATURE_AUTH
#define  AUTH_SSD_TIMER_SIG      0x0004
  /* This signal is set when the SSD timer expires */
#endif /* FEATURE_AUTH */

#define  AUTH_NV_CMD_SIG         0x0008
  /* This signal is set when NV completes a command */
  
#ifdef FEATURE_RUIM
#define  RUIM_CMD_RSP_SIG        0x0010
  /* This signal is set when RUIM is done processing a command */
  
#define  RUIM_CMD_RSP_TIMEOUT_SIG  0x0020
  /* This signal is set when the RUIM Command Response Timer expires */

#define  RUIM_POLL_TIMER_SIG     0x0040
  /* This signal is set when the RUIM Poll Timer expires */
  
#define  RUIM_UNSOLICITED_RSP_SIG  0x0080
  /* This signal is set when unexpected data is recd from the RUIM */
  
#define  RUIM_STATE_TOGGLE_SIG   0x0100
  /* This signal is set when some task votes on powerdown of RUIM */
                               
#define  RUIM_CHV_SIG            0x0200
  /* This signal is set when AUTH receives the CHV values */
#endif /* FEATURE_RUIM */


/*--------------------------------------------------------------------------
                         AUTHENTICATION REPORT

--------------------------------------------------------------------------*/
/* -------------------------------------------------------------------
** The Authentication Task optionally sends a report when it completes
** a command which produces a result which would be useful to the
** task which issued the command.
** ------------------------------------------------------------------- */
typedef enum
{
#ifdef FEATURE_AUTH
  AUTH_SIGNATURE_R,     /* Authentication Signature report */
  AUTH_A_KEY_R,         /* Whether A key was updated OK */
  AUTH_FINISH_SSD_R,    /* Whether SSD update completed OK */
  AUTH_GENERATE_KEY_R,  /* Indicates the key generation has completed */
  AUTH_RESYNC_R,         /* Verify AUTH has reset itself */
  AUTH_UPDATE_A_KEY_TEMP_R,/* Verfify that A_KEY_TEMP has been updated */
  AUTH_COMMIT_KEYS_R,
#ifdef FEATURE_OTASP_OTAPA
  AUTH_SIGNATURE_SPASM_R, /* Verify auth_otapa for SPASM has completed */
#endif /* FEATURE_OTASP_OTAPA */
#endif /* FEATURE_AUTH */
#ifdef FEATURE_RUIM
  AUTH_ACCESS_RUIM_R,   /* Access to the R-UIM                         */
  AUTH_UNBLOCK_CHV_R,   /* Un-block CHV on the R-UIM                   */
  AUTH_NEW_CHV_R,       /* Update the CHV */
  AUTH_RUIM_STATUS_R,   /* Status of the card */
#ifdef FEATURE_OTASP
  AUTH_OTASP_COMMIT_R,              /* Commit Request */
  AUTH_OTASP_MS_KEY_REQ_R,          /* MS Key Request */
  AUTH_OTASP_KEY_GEN_REQ_R,         /* Key Generation Request */
  AUTH_OTASP_VALIDATE_R,            /* Validation Request */
  AUTH_OTASP_CFG_REQ_R,             /* Configuration Request */
  AUTH_OTASP_DLOAD_REQ_R,           /* Download Request */
  AUTH_OTASP_SSPR_CFG_REQ_R,        /* SSPR Configuration Request */
  AUTH_OTASP_SSPR_DLOAD_REQ_R,      /* SSPR Download Reqeust */
  AUTH_OTASP_OTAPA_REQ_R,           /* OTAPA Request */
#endif /* FEATURE_OTASP */
  AUTH_STORE_ESN_ME_R,              /* Store ESN_ME  */ 
#endif /* FEATURE_RUIM */
  AUTH_MAX_R
} auth_report_code_type;

typedef enum {
  RUIM_FAIL = 0,
  RUIM_PASS = 1
} ruim_rpt_status;

#ifdef FEATURE_RUIM
#ifdef FEATURE_OTASP
typedef struct {
  byte result;
} auth_ms_key_rpt_type;

typedef struct {
  byte result;
  byte ms_result_len;
  byte ms_result[RUIM_PARAM_P];
} auth_key_gen_rpt_type;

typedef struct {
  byte result;
} auth_commit_rpt_type;

typedef struct {
  byte block_id;
  byte result;
} auth_validate_rpt_type;

typedef struct {
  byte block_id;
  byte block_length;
  byte result;
  byte data[RUIM_MAX_CHARS];
} auth_cfg_rpt_type;

typedef struct {
  byte block_id;
  byte result;
} auth_dload_rpt_type;

typedef struct {
  byte block_id;
  byte block_length;
  byte result;
  byte data[RUIM_MAX_CHARS];
} auth_sspr_cfg_rpt_type;

typedef struct {
  byte block_id;
  byte result;
  word segment_offset;
  byte segment_size;
} auth_sspr_dload_rpt_type;

typedef struct {
  byte result;
  byte nam_lock_ind;
  dword rand_otapa;
} auth_otapa_rpt_type;
#endif /* FEATURE_OTASP */

typedef struct {
  byte usage_ind_conf;
} auth_store_esn_rpt_type;

#endif /* FEATURE_RUIM */

typedef struct
{
  cmd_hdr_type         rpt_hdr;
    /* Includes the queue links,  pointer to requesting task TCB,
       signal to set on completion,  and done queue. */
  auth_report_code_type rpt_type; /* What type of report this is */
#ifdef FEATURE_RUIM
  ruim_rpt_status    rpt_status;      /* whether the access was OK */
#endif /* FEATURE_RUIM */
  union
  {
#ifdef FEATURE_AUTH
    dword              auth;           /* auth_signature (18 bits) */
    boolean            ssd_update_ok;  /* Whether SSD update completed OK */
    boolean            a_key_update_ok;/* Whether A key was updated OK */
    boolean            commit_keys_ok; /* Whether the keys committed were OK */
#endif /* FEATURE_AUTH */
#ifdef FEATURE_RUIM
#ifdef FEATURE_OTASP
    auth_ms_key_rpt_type     ms_key;
    auth_key_gen_rpt_type    key_gen;
    auth_commit_rpt_type     commit;
    auth_validate_rpt_type   validn;
    auth_cfg_rpt_type        cfg;
    auth_dload_rpt_type      dload;
    auth_sspr_cfg_rpt_type   sspr_cfg;
    auth_sspr_dload_rpt_type sspr_dload; 
    auth_otapa_rpt_type      otapa;
#endif /* FEATURE_OTASP */
    auth_store_esn_rpt_type  store_esn;
#endif /* FEATURE_RUIM */
  } rpt;
}
auth_rpt_type;

/*--------------------------------------------------------------------------
                    AUTHENTICATION COMMANDS

--------------------------------------------------------------------------*/

#ifndef FEATURE_RUIM
/* Status values for a command */
typedef enum {
  AUTH_RECEIVED_S,                  /* Command received */
  AUTH_INVALID_S,                   /* Invalid command */
  AUTH_ERR_S,                       /* An error occured */
  AUTH_UNINITIALIZED_S,             /* Authentication is not initialized */
  AUTH_MAXIMUM_S                    /* always last, this counts statuses */
} auth_status_type;
#endif /* FEATURE_RUIM */

/* Command types */
typedef enum {
#ifdef FEATURE_AUTH
  AUTH_INIT_F,                      /* Initialize on powerup or NAM change */
  AUTH_SSD_UPDATE_F,                /* Begin process to update SSD */
  AUTH_FINISH_SSD_UPDATE_F,         /* Finish process to update SSD */
  AUTH_SIGNATURE_F,                 /* Perform auth signature */
  AUTH_GENERATE_KEY_F,              /* Generate CMEA key and VPM */
  AUTH_UPDATE_A_KEY_F,              /* Update the A-key */
  AUTH_INC_COUNT_F,                 /* Increment count value */
  AUTH_START_TIMER_F,               /* Starts SSD Timer */
  AUTH_RESYNC_F,                    /* Re-synchronizes the command queue */
  AUTH_RESET_F,                     /* Reset Auth Task state */
  AUTH_UPDATE_A_KEY_TEMP_F,         /* Update the A-key */
  AUTH_COMMIT_KEYS_F,               /* Commit the key */
#ifdef FEATURE_OTASP_OTAPA
  AUTH_SIGNATURE_SPASM_F,           /* Perform auth signature for SPASM */
#endif /* FEATURE_OTASP_OTAPA */
#ifdef FEATURE_RUIM
  AUTH_BS_CHAL_F,                   /* Do a Base station Challenge */
#endif /* FEATURE_RUIM */
#endif /* FEATURE_AUTH */
#ifdef FEATURE_RUIM
  AUTH_ACCESS_RUIM_F,               /* Read or Write from RUIM */
  AUTH_RUIM_INTERNAL_F,             /* POWER UP RUIM */
  AUTH_UNBLOCK_CHV_F,               /* Unblock CHV */
  AUTH_NEW_CHV_F,                   /* New CHV has been written to NV */
  AUTH_RUIM_STATUS_F,               /* Status of the Card */
#ifdef FEATURE_OTASP
  AUTH_OTASP_COMMIT_F,              /* Commit Request */
  AUTH_OTASP_MS_KEY_REQ_F,          /* MS Key Request */
  AUTH_OTASP_KEY_GEN_REQ_F,         /* Key Generation Request */
  AUTH_OTASP_VALIDATE_F,            /* Validation Request */
  AUTH_OTASP_CFG_REQ_F,             /* Configuration Request */
  AUTH_OTASP_DLOAD_REQ_F,           /* Download Request */
  AUTH_OTASP_SSPR_CFG_REQ_F,        /* SSPR Configuration Request */
  AUTH_OTASP_SSPR_DLOAD_REQ_F,      /* SSPR Download Reqeust */
  AUTH_OTASP_OTAPA_REQ_F,           /* Otapa Request */
#endif /* FEATURE_OTASP */
#endif /* FEATURE_RUIM */
  AUTH_STORE_ESN_ME_F,              /* Store ESN_ME command */
  AUTH_MAX_F
} auth_cmd_name_type;

#ifdef FEATURE_RUIM 
/* RUIM Access types */
typedef enum {
  RUIM_READ_F  = 0,                    /* Read item from RUIM */
  RUIM_WRITE_F = 1,                    /* Write item to RUIM */
  RUIM_INC_F   = 2                     /* Increment item in RUIM */
} ruim_access_type;

typedef enum {
  AUTH_CHV_NONE = 0,
  AUTH_CHV1     = 1,                   /* CHV1 */
  AUTH_CHV2     = 2                    /* CHV2 */
} auth_chv_type;

typedef enum {
  RUIM_MSISDN            = 0,      /* Mobile Directory Number */
  RUIM_CC                = 1,      /* Call Count */
  RUIM_IMSI_M            = 2,      /* IMSI_M */
  RUIM_IMSI_T            = 3,      /* IMSI_T */
  RUIM_TMSI              = 4,      /* TMSI */
  RUIM_ANALOG_HOME_SID   = 5,      /* Analog Home SID */
  RUIM_ANALOG_OP_PARAMS  = 6,      /* Analog Operational Parameters */
  RUIM_ANALOG_LOCN_AND_REGN_IND = 7,/* Analog Locn and Regn Indicators */
  RUIM_CDMA_HOME_SID_NID = 8,      /* CDMA Home SID and NID */
  RUIM_CDMA_ZONE_BASED_REGN_IND = 9,/* CDMA Zone Based Regn Indicators */

  RUIM_CDMA_SYS_REGN_IND = 10,     /* CDMA System/Network Regn Indicators */
  RUIM_CDMA_DIST_BASED_REGN_IND = 11,/* CDMA Distance Based Regn Indicators */
  RUIM_ACCOLC            = 12,     /* Access Overload Class */
  RUIM_CALL_TERM_MODE_PREF = 13,   /* Call Termination Mode Preferences */
  RUIM_SCI               = 14,     /* Suggested Slot Cycle Index */
  RUIM_ANALOG_CHAN_PREF  = 15,     /* Analog Channel Preferences */
  RUIM_PRL               = 16,     /* Preferred Roaming List */
  RUIM_RUIM_ID           = 17,     /* Removable UIM ID */ 
  RUIM_CDMA_SVC_TABLE    = 18,     /* CDMA Service Table */
  RUIM_SPC               = 19,     /* Service Programming Code */

  RUIM_OTAPA_SPC_ENABLE  = 20,     /* OTAPA/SPC-Enable */
  RUIM_NAM_LOCK          = 21,     /* NAM-Lock */ 
  RUIM_OTASP_OTAPA_FEATURES = 22,  /* OTASP-OTAPA Parameters */
  RUIM_SERVICE_PREF      = 23,     /* Service Preferences */ 
  RUIM_ESN_ME            = 24,     /* ESN_ME */
  RUIM_RUIM_PHASE        = 25,     /* R-UIM Phase */
  RUIM_PREF_LANG         = 26,     /* Preferred Languages */
  RUIM_UNASSIGNED_1      = 27,     /* Not assigned yet */
  RUIM_SMS               = 28,     /* Short Messages */
  RUIM_SMS_PARAMS        = 29,     /* Short Message Service Parameters */

  RUIM_SMS_STATUS        = 30,     /* SMS Status */
  RUIM_SUP_SVCS_FETURE_CODE_TABLE = 31,/* Sup Svcs Feature Code Table */
  RUIM_UNASSIGNED_2      = 32,     /* Not assigned yet */
  RUIM_CDMA_HOME_SVC_PVDR_NAME = 33,   /* CDMA Home Service Provider Name */
  RUIM_UIM_ID_USAGE_IND  = 34,     /* R-UIM ID usage indicator */
  RUIM_UNASSIGNED_3      = 35,     /* Not assigned yet */
  RUIM_MDN               = 36,     /* Mobile Directory Number */
  RUIM_ADN               = 37,     /* Phonebook from Telecom directory */
  RUIM_ADM_DATA          = 38      /* Administrative Data */

} ruim_items_enum_type;

typedef enum {
  RUIM_POWER_UP_ST,                     /* Power Up state */                        
  RUIM_RESET_ST,                        /* Reset State */                           
  RUIM_PPS_ST,                          /* PPS State */                                
  RUIM_UPDATE_OP_PARAMS_ST,             /* Update Operational Parameters State */    
  RUIM_SELECT_ST,                       /* Select State */                          
  RUIM_STATUS_ST,                       /* Status State */                          
  RUIM_READ_BINARY_ST,                  /* Read Binary State */                     
  RUIM_UPDATE_BINARY_ST,                /* Update Binary State */                   
  RUIM_READ_RECORD_ST,                  /* Read Record State */                     
  RUIM_UPDATE_RECORD_ST,                /* Update Record State */                   
  RUIM_INCREMENT_ST,                    /* Increment State */                       
  RUIM_VERIFY_CHV1_ST,                  /* Verify CHV 1 State */                    
  RUIM_VERIFY_CHV2_ST,                  /* Verify CHV 2 State */                    
  RUIM_UNBLOCK_CHV1_ST,                 /* Unblock CHV 1 State */                       
  RUIM_UNBLOCK_CHV2_ST,                 /* Unblock CHV 2 State */                       
#ifdef FEATURE_AUTH                                         
  RUIM_UPDATE_SSD_ST,                   /* Update SSD State */                      
  RUIM_CONFIRM_SSD_ST,                  /* Confirm SSD State */                     
  RUIM_GENERATE_KEY_ST,                 /* Generate KEY State */                        
  RUIM_RUN_CAVE_ST,                     /* Run CAVE State */                        
  RUIM_BS_CHALLENGE_ST,                 /* BS Challenge State */                    
#endif /* FEATURE_AUTH */                       
#ifdef FEATURE_OTASP
  RUIM_MS_KEY_REQ_ST,                   /* MS Key Request State */
  RUIM_KEY_GEN_REQ_ST,                  /* Key Generation Request State */
  RUIM_CFG_REQ_ST,                      /* Configuration Request State */
  RUIM_DLOAD_REQ_ST,                    /* Download Request State */
  RUIM_SSPR_CFG_REQ_ST,                 /* SSPR Config Request State */
  RUIM_SSPR_DLOAD_REQ_ST,               /* SSPR Download Request State */
  RUIM_OTAPA_REQ_ST,                    /* Commit State */
  RUIM_COMMIT_ST,                       /* Validation State */
  RUIM_VALIDATE_ST,
#endif /* FEATURE_OTASP */
  RUIM_STORE_ESN_ME_ST,                 /* Store ESN_ME state */
  RUIM_DONE_ST                          /* Done State */    
} ruim_state_type;

typedef enum {
  RUIM_INVALID_S,                 /* Recd the command in an invalid State */ 
  AUTH_RECEIVED_S,                /* Recd the command */ 
  RUIM_UNINITIALIZED_S,           /* RUIM is not initialized yet */               
  RUIM_POWERED_DOWN_S,            /* RUIM is powered down */                           
  RUIM_INITIALIZED_S,             /* RUIM is initialized */                   
  RUIM_ERR_S,                     /* RUIM is faulty */                
  RUIM_MAXIMUM_S
} auth_ruim_status_type;

typedef enum {
  ONLY,                  /* Flush only this command from the Q */
  ALL                    /* Flush all the commands in the q */   
} auth_flush_type;

/* Indicate command handling options */
typedef enum {
  AUTH_RUIM_NONE = 0x00,    /* Disabled */
  AUTH_RUIM_AUTH = 0x01,    /* ruim power-down bit for Auth task */
  AUTH_RUIM_MC   = 0x02     /* ruim power-down bit for MC task */
} ruim_voter_type;

/* Indicate command handling options */
typedef enum {
  RUIM_OPTION_NONE       = 0,        /* No options used */
  RUIM_OPTION_ALWAYS_RPT = 1         /* Always report   */
} auth_ruim_option_type;

typedef struct {
  boolean         use_chv;           /* indicate if CHV should be used */
  byte            chv[RUIM_MAX_CHV_DIGITS];      /* CHV digits */
} auth_ruim_chv_type;

typedef struct {
  byte num_bytes;              /* number of bytes in ATR */
  byte data[RUIM_MAX_ATR_CHARS];/* buffer to hold the ATR */
} ruim_atr_buf_type;

#endif /* FEATURE_RUIM */

/* The common Header for all the commands */
typedef struct {
  cmd_hdr_type          cmd_hdr;       /* command header */
  auth_cmd_name_type    command;       /* The command */
#ifndef FEATURE_RUIM
  auth_status_type      status;        /* Space for message status */
#else
  auth_ruim_status_type status;        /* Status of the R-UIM */
  auth_ruim_option_type options;       /* Indicate command handling options */
  byte                  cmd_count;     /* Number of Attempts  */
#endif /* FEATURE_RUIM */
} auth_hdr_type;

#ifdef FEATURE_AUTH
/* Initialize on powerup or NAM change */
typedef struct {                  /* AUTH_INIT_F */
  auth_hdr_type   hdr;            /* Command header */
  byte            nam;            /* Which NAM info to initialize with */
} auth_init_type;

/* Begin process to update SSD */
typedef struct {                  /* AUTH_SSD_UPDATE_F */
#ifdef FEATURE_RUIM
  auth_hdr_type   hdr;            /* Command header */
  qword           randssd;        /* 56 bits from BS for SSD Generation */
  byte            process_control;/* process control bits */
  dword           esn;            /* ESN ( 7 bytes per standard ) */
#else
  auth_hdr_type   hdr;            /* Command header */
  qword           randssd;        /* 56 bits from BS for SSD Generation */
  dword           randbs;         /* 32 random bits for Auth Signature */
  dword           auth_data;      /* 24 data bits for Auth Signature */
#endif /* FEATURE_RUIM */
} auth_ssd_update_type;

/* Finish process to update SSD */
typedef struct {                  /* AUTH_FINISH_SSD_UPDATE_F */
  auth_hdr_type   hdr;            /* Command header */
  dword           authbs;         /* Auth Signature from Base Station */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_finish_ssd_update_type;

/* Perform auth signature */
typedef struct {                  /* AUTH_SIGNATURE_F */
#ifdef FEATURE_RUIM
  auth_hdr_type   hdr;            /* Command header */
  byte            rand_type;      /* Rand/Randu     */
  dword           rand_chal;      /* 32 bits from BS for Auth Signature */
  byte            dig_len;        /* digit length expressed in bits */
  byte            digits[RUIM_RUN_CAVE_DIGITS];      
                                  /* maximum of 6 BCD coded digits */
  byte            process_control;/* process control bits */
  dword           esn;            /* ESN ( 7 bytes per standard ) */
  void            (*rpt_function)( auth_rpt_type * );
#else
  auth_hdr_type   hdr;            /* Command header */
  dword           rand_chal;      /* 32 bits from BS for Auth Signature */
  dword           auth_data;      /* 24 data bits for Auth Signature */
  boolean         save_reg;       /* TRUE if registers are to be saved */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
#endif /* FEATURE_RUIM */
} auth_signature_type;

/* Generate CMEA key and VPM */
typedef struct {                  /* AUTH_GENERATE_KEY_F */
#ifdef FEATURE_RUIM
  auth_hdr_type   hdr;            /* Command header */
  byte            vpm_first_octet;/* First octet of VPM to be output */
  byte            vpm_last_octet; /* Last  octet of VPM to be output */
  void            (*rpt_function)( auth_rpt_type * );
#else
  auth_hdr_type   hdr;            /* Command header */
  boolean         do_vpm;         /* TRUE if Voice Privacy Mask is needed */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
#endif /* FEATURE_RUIM */
} auth_generate_key_type;

/* Update the A-key */
typedef struct {                  /* AUTH_UPDATE_A_KEY_F */
  auth_hdr_type   hdr;            /* Command header */
  byte            a_key[AUTH_A_KEY_DIGITS]; /* New A-key to be used */
  byte            nam;            /* NAM for which the A-key is intended */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_update_a_key_type;

/* Increment count value */
typedef struct {                  /* AUTH_START_TIMER_F */
  auth_hdr_type   hdr;            /* Command header */
} auth_inc_count_type;

/* Starts SSD Timer */
typedef struct {                  /* AUTH_START_TIMER_F */
  auth_hdr_type   hdr;            /* Command header */
} auth_start_timer_type;

typedef struct {                  /* AUTH_RESYNC_F */
  auth_hdr_type   hdr;            /* Command header */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_resync_type;

typedef struct {                  /* AUTH_RESET_F */
  auth_hdr_type   hdr;            /* Command header */
} auth_reset_type;

typedef struct {                  /* AUTH_COMMIT_KEYS_F */
  auth_hdr_type   hdr;            /* Command header */
  byte            nam_index;       /* the nam to commit the a_key into */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_commit_keys_type;

typedef struct {                  /* AUTH_UPDATE_A_KEY_TEMP_F */
  auth_hdr_type   hdr;            /* Command header */
  qword           a_key;/* a_key_temp */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_update_a_key_temp_type;

typedef struct {                  /* AUTH_BS_CHAL_F */
  auth_hdr_type   hdr;            /* Command header */
} auth_bs_chal_type;
#endif /* FEATURE_AUTH */

#ifdef FEATURE_RUIM

typedef struct {
  auth_hdr_type   hdr;            /* Command header */
  ruim_items_enum_type item;      /* Item to be accessed */
  byte            nam;            /* NAM info */
  ruim_access_type access;        /* Type of access, Read or Write */ 
  word            num_bytes;      /* Number of bytes to be read or written */
  word            num_bytes_rsp;  /* Number of bytes returned */
  word            num_records_rsp;/* Number of records in the response */
  word            offset;         /* Offset into the EF */
  void            *data_ptr;      /* Pointer for data */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_access_ruim_type;

typedef struct {
  auth_hdr_type   hdr;            /* Command header */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_ruim_int_type;

typedef struct {
  auth_hdr_type   hdr;            /* Command header */
  auth_chv_type   chv;
  byte            chv_digits[RUIM_MAX_CHV_DIGITS];      /* CHV digits */
  byte            new_chv_digits[RUIM_MAX_CHV_DIGITS];  /* New CHV digits */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_unblock_chv_type;

typedef struct {
  auth_hdr_type   hdr;            /* Command header */
  auth_chv_type   chv;            /* CHV1 or CHV 2 */
  byte            chv_digits[RUIM_MAX_CHV_DIGITS];  /* CHV digits */
  void            (*rpt_function)( auth_rpt_type * );
} auth_new_chv_type;

typedef struct {
  auth_hdr_type   hdr;            /* Command header */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_ruim_status_cmd_type;

#ifdef FEATURE_OTASP
typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      randseed[RUIM_RAND_SEED];/* Seed used to generate TRUE random no */
  byte      a_key_p_rev;             /* A-KEY-P_REV supported by BS */
  byte      param_p_len;             /* Length of Parameter P */
  byte      param_g_len;             /* Length of Parameter G */
  byte      param_p[RUIM_PARAM_P];   /* Param P */
  byte      param_g[RUIM_PARAM_G];   /* Param G */
  void      (*rpt_function)( auth_rpt_type * );
                                     /* Pointer to Function to send report */
} auth_ms_key_req_type;

typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      bs_result_len;           /* BS Result length */
  byte      bs_result[RUIM_PARAM_P]; /* BS Result */
  void      (*rpt_function)( auth_rpt_type * );
                                     /* Pointer to Function to send report */
} auth_key_gen_req_type;

typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      block_id;                /* Block ID of the block */
  byte      block_length;            /* Length of the block */
  byte      data[RUIM_MAX_CHARS];    /* data in the block */
  void      (*rpt_function)( auth_rpt_type * );
                                     /* Pointer to Function to send report */
} auth_validation_req_type;

typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      block_id;                /* Block ID of the block */
  void      (*rpt_function)( auth_rpt_type * );
                                     /* Pointer to Function to send report */
} auth_cfg_req_type;

typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      block_id;                /* Block ID of the block */
  byte      block_len;               /* Length of the block */
  byte      data[RUIM_MAX_CHARS];    /* data in the block */
  void      (*rpt_function)( auth_rpt_type * );
                                     /* Pointer to Function to send report */
} auth_dload_req_type;

typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      block_id;                /* Block ID of the block */
  word      req_offset;              /* Requested offset in the PRL */
  byte      req_max_size;            /* Requested Max size in the response */
  void      (*rpt_function)( auth_rpt_type * );
                                     /* Pointer to Function to send report */
} auth_sspr_cfg_req_type;

typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      block_id;                /* Block ID of the block */
  byte      block_len;               /* Length of the block */  
  byte      data[RUIM_MAX_CHARS];    /* data in the block */
  void      (*rpt_function)( auth_rpt_type * );
                                     /* Pointer to Function to send report */
} auth_sspr_dload_req_type;

typedef struct {
  auth_hdr_type   hdr;               /* Command header */
  byte      start_stop;              /* OTAPA Start Stop */
  dword     randseed;                /* Random seed */
  void      (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_otapa_req_type;

typedef struct {
  auth_hdr_type   hdr;            /* Command header */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_commit_req_type;

#endif /* FEATURE_OTASP */

typedef struct {
  auth_hdr_type   hdr;            /* Command header */
  byte            len_and_usage;  /* Length and usage of ESN */
  dword           esn;            /* ESN */
  void            (*rpt_function)( auth_rpt_type * );
                                  /* Pointer to Function to send report */
} auth_store_esn_type;

#endif /* FEATURE_RUIM */

typedef union {
  auth_hdr_type                hdr;     /* Generic header */
#ifdef FEATURE_AUTH
  auth_init_type               init;    /* Init auth variables */
  auth_ssd_update_type         ssd;     /* SSD Update*/
  auth_finish_ssd_update_type  f_ssd;   /* Finish SSD Update*/
  auth_signature_type          sig;     /* Authentication Signature */
  auth_generate_key_type       key;     /* Generate CMEA key and VPM */
  auth_update_a_key_type       a_key;   /* Update the A-key */
  auth_start_timer_type        start;   /* Start SSD timer */
  auth_inc_count_type          inc;     /* Increment COUNTsp */
  auth_resync_type             resync;  /* Re-syncronize command queue */
  auth_reset_type              reset;   /* Reset AUTH task state */
  auth_commit_keys_type        commit_keys;  /* commit the keys  */
  auth_update_a_key_temp_type  temp;    /* Update the a_key_temp */
  auth_bs_chal_type            chal;    /* Base station challenge type */
#endif /* FEATURE_AUTH */
#ifdef FEATURE_RUIM
  auth_access_ruim_type        access_ruim; /* Read from or Write to RUIM */
  auth_ruim_int_type           ruim_int ;   /* Init RUIM */
  auth_unblock_chv_type        unblk_chv;   /* Unblock CHV */
  auth_new_chv_type            new_chv;     /* New CHV */
  auth_ruim_status_cmd_type    status;      /* Status of the Card */
#ifdef FEATURE_OTASP
  auth_ms_key_req_type         key_req;     /* MS Key Request */
  auth_key_gen_req_type        key_gen;     /* Key Generation Request */
  auth_validation_req_type     validn;      /* Validation Request */
  auth_cfg_req_type            cfg;         /* Configuration Request */
  auth_dload_req_type          dload;       /* Download Request */
  auth_sspr_cfg_req_type       sspr_cfg;    /* SSPR Cfg Request */
  auth_sspr_dload_req_type     sspr_dload;  /* SSPR Download Request */
  auth_otapa_req_type          otapa;       /* OTAPA Request */
  auth_commit_req_type         commit;      /* Commit Request */
#endif /* FEATURE_OTASP */
  auth_store_esn_type          store_esn;   /* Store ESN */
#endif /* FEATURE_RUIM */
} auth_cmd_type;

/* Queue for other tasks to get auth command buffers from */
extern q_type auth_free_q;

#ifdef FEATURE_AUTH
/* Storage for A_KEY_TEMP */
extern qword a_key_temp;

/* to indicate if new A-Key has been generated */
extern boolean a_key_temp_valid;

/* to inidcate if the new ssd is valid */
extern boolean new_ssd_valid;

#endif /* FEATURE_AUTH */

#if defined (FEATURE_AUTH) && !defined(FEATURE_RUIM)

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_VALIDATE_A_KEY

DESCRIPTION
  This procedure tests if the given A key is valid.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the A key is valid
  FALSE if the A key is invalid

SIDE EFFECTS
  None

===========================================================================*/
extern boolean auth_validate_a_key
(
  byte a_key[AUTH_A_KEY_DIGITS]
    /* The A key to be validated */
);

#endif /* FEATURE_AUTH && !FEATURE_RUIM */

#ifdef FEATURE_AUTH

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_PRIVATE_LCM_AVAILABLE

DESCRIPTION
  This procedure checks the private lcm to make sure it can be activated.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the new lcm is available, FALSE othewise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean auth_private_lcm_available( void );

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_SWITCH_TO_PRIVATE_LCM

DESCRIPTION
  This procedure sets the private long code mask into encoder and
  demodulator.

DEPENDENCIES
  The function auth_check_private_lcm needs to be called first to ensure
  a valid long code mask is available.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void auth_switch_to_private_lcm ( void );

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_GET_COUNT

DESCRIPTION
  This procedure retrieves the current value of COUNTsp.

DEPENDENCIES
  None

RETURN VALUE
  Returns COUNTsp.

SIDE EFFECTS
  None

===========================================================================*/
extern byte auth_get_count( void );

#endif /* FEATURE_AUTH */

#ifdef FEATURE_RUIM
#ifdef FEATURE_AUTH
/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_GET_RANDBS

DESCRIPTION
  This procedure retrieves the current value of randbs.

DEPENDENCIES
  None

RETURN VALUE
  Returns randbs.

SIDE EFFECTS
  None

===========================================================================*/
extern dword auth_get_randbs( void );
#endif /* FEATURE_AUTH */

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_CHV_VALID

DESCRIPTION
  Report whether or not CHV is valid

DEPENDENCIES
  None

RETURN VALUE
  Validity of the CHV
    TRUE   CHV is valid
    FALSE  CHV is not valid

SIDE EFFECTS
  None

===========================================================================*/
extern boolean auth_chv_valid( void );

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_GET_RUIM_STATUS

DESCRIPTION
  This procedure retrieves the state of RUIM.

DEPENDENCIES
  None

RETURN VALUE
  Returns TRUE if RUIM is alive.

SIDE EFFECTS
  None

===========================================================================*/
extern auth_ruim_status_type auth_get_ruim_status( void );

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_RUIM_ON

DESCRIPTION
  This procedure changes the state of RUIM.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Internally sets the RUIM_TOGLLE_STATE_SIG

===========================================================================*/
extern void auth_ruim_on ( ruim_voter_type mask, boolean ruim_reqd);

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_CHV

DESCRIPTION
  This procedure updates or gets the value of CHVs from UI.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Internally sets the AUTH_CHV_SIG

===========================================================================*/
extern void auth_chv ( auth_ruim_chv_type *ui_chv1, auth_ruim_chv_type *ui_chv2);

#if defined (FEATURE_AUTH) && defined (FEATURE_AUTH_DIGITS)
#error code not present
#endif /* defined (FEATURE_AUTH) && defined (FEATURE_AUTH_DIGITS) */

#endif /* FEATURE_RUIM */

#ifdef FEATURE_AUTH
#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION AUTH_GET_PRIVATE_LCM

DESCRIPTION
  This procedure returns the private long code mask. 
 
DEPENDENCIES
  The function auth_check_private_lcm needs to be called first to ensure
  a valid long code mask is available.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void auth_get_private_lcm ( qword qplcm );
#endif //FEATURE_IS2000

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_ENCRYPTION_POSSIBLE

DESCRIPTION
  This procedure determines if message encryption is possible.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if auth could successfully encrypt messages.
  FALSE if auth is not initialized properly to do encryption.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean auth_encryption_possible( void );

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_ENCRYPT_CDMA

DESCRIPTION
  This procedure encrypts CDMA reverse traffic channel messages.
  If any fields require encryption they are encrypted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The num_digits, num_fields, record_len, etc. fields may not accurately
  represent the size of the field(s) if bits are added for encryption.

===========================================================================*/
extern void auth_encrypt_cdma
(
  word *len_ptr,
    /* Length of message in bits */
  byte *buf_ptr,
    /* Pointer to body of already packed traffic channel message */
  caii_tx_msg_type *int_ptr
    /* Pointer to internal message to be transmitted */
);

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_DECRYPT_CDMA

DESCRIPTION
  This procedure decrypts CDMA reverse traffic channel messages.
  If any fields require decryption they are decrypted.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if there are no problems encountered.
  FALSE if fields of the given message that should be decrypted are not.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean auth_decrypt_cdma
(
  word *len_ptr,
    /* Length of message in bits */
  byte *buf_ptr
    /* Pointer to received message to be translated */
);

#ifdef FEATURE_ACP
/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_ENCRYPT_ANALOG

DESCRIPTION
  This procedure encrypts analog voice channel messages.
  If any fields require encryption they are encrypted.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if there are no problems encountered.
  FALSE if fields of the given message that should be encrypted are not.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean auth_encrypt_analog
(
  acp553_txmsg_type *buf_ptr
    /* Pointer to received message to be translated */
);

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_DECRYPT_ANALOG

DESCRIPTION
  This procedure decrypts analog voice channel messages.
  If any fields require decryption they are decrypted.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if there are no problems encountered.
  FALSE if fields of the given message that should be decrypted are not.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean auth_decrypt_analog
(
  acp553_rxmsg_type *buf_ptr
    /* Pointer to received message to be translated */
);
#endif /* FEATURE_ACP */
#endif /* FEATURE_AUTH */

#if defined(FEATURE_RUIM) || defined (FEATURE_AUTH)
/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_TASK

DESCRIPTION
  This procedure is the entrance procedure for the Authentication task.

DEPENDENCIES
  None

RETURN VALUE
  Does not return.

SIDE EFFECTS
  None

===========================================================================*/

extern void auth_task (
  dword dummy
    /* Parameter required for REX.  Tell lint to ignore it. */
    /*lint -esym(715,dummy) */
);

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_CMD

DESCRIPTION
  The auth_cmd pointed to by the parameter passed in is queued up for AUTH.

DEPENDENCIES
  None.
RETURN VALUE
  None.
SIDE EFFECTS
  None.
===========================================================================*/
extern void auth_cmd (
  auth_cmd_type *cmd_ptr                    /* the command to queue up */
);
#endif /* FEATURE_RUIM || FEATURE_AUTH */

#endif /* AUTH_H */

