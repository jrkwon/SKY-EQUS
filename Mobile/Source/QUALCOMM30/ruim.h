#ifndef RUIM_H
#define RUIM_H
/*===========================================================================

              R - U I M   D E V I C E   L A Y E R   H E A D E R 

DESCRIPTION
   This is the header file for the interface layer of the RUIM device driver.  
   Therefore, this header file provides the complete interface to the low level
   RUIM device driver.

   Copyright (c) 1998 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/ruim.h_v   1.4   07 Feb 2001 15:39:46   wli  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/07/01   wli     Fixed the bug in ruim_file_structure_type.
12/18/00   ck      Changed the request offset in SSPR Configuration request
                   structure to an array of 2 bytes from a word.
12/04/00   ts      Added a half word (16 bit) access macro for the UART2 
                   configuration register for MSM5105.
12/01/00   ck      Added comments and definitions to support IS-683A.
03/09/00   ts      Created file.
===========================================================================*/

/* This defines the status received from the UIM as a result of an APDU      */

/*
  This is a conversion table that returns the number of bytes that follow the
  format byte.  Only the "presents of interface bytes" field is used in the 
  index.  The table contains the number of bits set in the index.
 */

extern const byte ruim_y_format_table[16];

/* These define the UIM specific interface bytes */

#define RUIM_K_FIELD          0x0F      /* In T0; Number of historical bytes */
#define RUIM_Y_FIELD_SHIFT       4      /* Number of bits to shift to get the
                                           Y field in the lsb                */
#define RUIM_TD_PRESENT       0x80      /* TD byte is present in following 
                                           bytes                             */
#define RUIM_T_FIELD          0x0F      /* In TD; transmission protocol      */

#define RUIM_DEFLT_GUARDTIME_BITS 0     /* This is the default number of extra 
                                           bits used for the guardtime.      */

#define RUIM_MAX_CHV_DIGITS      8      /* This is the maximum number of digits
                                           that make up a CHV code           */

#ifdef FEATURE_OTASP
#define RUIM_PARAM_P            64      /* 64 bytes of param p in key exchange */
#define RUIM_PARAM_G            20      /* 20 bytes of param g in key exchange */
#define RUIM_RAND_SEED          20      /* 20 bytes of seed used in key exchange */
#endif /* FEATURE_OTASP */

#define RUIM_MAX_CHARS         255      /* Maximum number of chars in one transaction */

typedef enum {
  RUIM_DONE,
  RUIM_DONE_EXTRA,
  RUIM_TOOLKIT_BUSY,              /* The R-UIM toolkit is unable to perform the
                                     operation because it is currently busy 
                                     working on another command              */
  RUIM_SUCCESS_AFTER_MULTI_TRIES, /* The command is successful; but after 
                                     using the internal update retry routine.*/
  RUIM_MEMORY_PROBLEM,            /* Memory problem.                         */
  RUIM_NO_EF_SELECTED,            /* No EF was selected                      */
  RUIM_OUT_OF_RANGE,              /* Indicates an invalid address            */
  RUIM_NOT_FOUND,                 /* Indicates file ID or pattern is not
                                     found.                                  */
  RUIM_INCONSISTENT_WITH_COMMAND, /* Indicates the file is inconsistent with
                                     the command.                            */
  RUIM_NO_CHV_INITIALIZED,        /* No CHV was initialized to allow this
                                     command                                 */
  RUIM_ACCESS_NOT_FULFILLED,      /* Indicates the access condition for the 
                                     command has not been fulfilled.
                                     Unsuccessful CHV verification, at least 
                                     one attempt left
                                     Unsuccessful UNBLOCK CHV verification, 
                                     at least one attempt left authentication 
                                     failed                                  */
  RUIM_CONTRADICTION_W_CHV_STATUS,/* Command is in contradiction with the CHV
                                     status                                  */
  RUIM_CONTRADICTION_W_INVALID_STATUS,/* Command is in contradiction with the 
                                     invalidation status                     */
  RUIM_SSD_UPDATE_SEQ_PROBLEM,    /* Problem in the sequence of the command 
                                     with respect to base station challange  */
  RUIM_CHV_BLOCKED,               /* Unsuccessful CHV verification, no attempt 
                                     left
                                     Unsuccessful UNBLOCK CHV verification, no
                                     attempt left
                                     CHV blocked
                                     UNBLOCK CHV blocked                     */
  RUIM_MAX_VALUE_REACHED,         /* Indicates the increase operation cannot be
                                     performed because the maximum value has 
                                     been reached.                           */
  RUIM_INCORRECT_PARAM_P3,        /* P3 is an incorrect parameter            */
  RUIM_INCORRECT_PARAM_P1_2,      /* P1 or P2 is an incorrect parameter      */
  RUIM_UNKNOWN_INS,               /* Unknown instruction code given in the 
                                     command (APDU)                          */
  RUIM_WRONG_CLASS,               /* Wrong instruction class given in the 
                                     command                                 */
  RUIM_PROBLEM,                   /* Technical problem with no diagnostic 
                                     given                                   */
  RUIM_PARITY                     /* Parity Error */

} ruim_apdu_status_type;

/* This type gives the allowable clock frequencies.                          */
typedef enum {
  UIM_CLK_2_46MHZ,
  UIM_CLK_4_92MHZ
} ruim_clk_freq_type;

/* The UIM clock rate conversion factor is the index value received from the 
   UIM card in TA(1) of the ATR or in the PPS.  This index refers to the clock rate
   conversion factor and the MAX clock rate for the UIM.  The clock rate conversion
   factor helps determine the baudrate of the UIM interface                  */
typedef enum {
  UIM_CRCF_372_0,         /* Conversion factor = 372                         */
  UIM_CRCF_372_1,         /* Conversion factor = 372                         */
  UIM_CRCF_558,           /* Conversion factor = 568                         */
  UIM_CRCF_744,           /* Conversion factor = 744                         */
  UIM_CRCF_1116,          /* Conversion factor = 1116                        */
  UIM_CRCF_1488,          /* Conversion factor = 1488                        */
  UIM_CRCF_1860,          /* Conversion factor = 1860                        */
  UIM_CRCF_RFU1,          /* Reserved for future use                         */
  UIM_CRCF_RFU2,          /* Reserved for future use                         */
  UIM_CRCF_512,           /* Conversion factor = 512                         */
  UIM_CRCF_768,           /* Conversion factor = 768                         */
  UIM_CRCF_1024,          /* Conversion factor = 1024                        */
  UIM_CRCF_1536,          /* Conversion factor = 1536                        */
  UIM_CRCF_2048,          /* Conversion factor = 2048                        */
  UIM_CRCF_RFU3,          /* Reserved for future use                         */
  UIM_CRCF_RFU4,          /* Reserved for future use                         */
  UIM_CRCF_SIZE           /* Number of constants of this type                */
} ruim_clock_rate_conversion_factor_type;

/* The UIM baud rate conversion factor is the index value received from the 
   UIM card in TA(1) of the ATR or in the PPS.  This index refers to the baud 
   rate adjustment factor for the UIM.  The baud rate adjustment factor helps 
   determine the baudrate of the UIM interface                               */
typedef enum {
  UIM_BRAF_RFU1,          /* Reserved for future use                         */
  UIM_BRAF_1,             /* Adjustment factor = 1                           */
  UIM_BRAF_2,             /* Adjustment factor = 2                           */
  UIM_BRAF_4,             /* Adjustment factor = 4                           */
  UIM_BRAF_8,             /* Adjustment factor = 8                           */
  UIM_BRAF_16,            /* Adjustment factor = 16                          */
  UIM_BRAF_32,            /* Adjustment factor = 32                          */
  UIM_BRAF_RFU2,          /* Reserved for future use                         */
  UIM_BRAF_12,            /* Adjustment factor = 12                          */
  UIM_BRAF_20,            /* Adjustment factor = 20                          */
  UIM_BRAF_RFU3,          /* Reserved for future use                         */
  UIM_BRAF_RFU4,          /* Reserved for future use                         */
  UIM_BRAF_RFU5,          /* Reserved for future use                         */
  UIM_BRAF_RFU6,          /* Reserved for future use                         */
  UIM_BRAF_RFU7,          /* Reserved for future use                         */
  UIM_BRAF_RFU8,          /* Reserved for future use                         */
  UIM_BRAF_SIZE           /* Number of constants of this type                */
} ruim_baud_rate_adjustment_factor_type;

typedef enum {

  RUIM_WAITING_FOR_T0,    /* Expecting the T0 character 
                             after skipping the num_bytes_to_skip */
  RUIM_WAITING_FOR_TD,    /* Expecting a TD character 
                             after skipping the num_bytes_to_skip */
  RUIM_WAITING_FOR_TCK,   /* Expecting the TCK character
                             after skipping the num_bytes_to_skip */
  RUIM_WAITING_FOR_END    /* Expecting the last of the ATR with NO
                             TCK character expected.              */
} ruim_atr_rx_mode_type;

/* These types defined the callback functions used by the RUIM driver.
                                                                             */
typedef void (*ruim_unsolict_resp_callback_type)( void );
typedef void (*ruim_resp_callback_type)( void );

typedef byte ruim_instrns_enum_type;

#define SELECT                 ((ruim_instrns_enum_type) 0xA4)
#define STATUS                 ((ruim_instrns_enum_type) 0xF2)
#define READ_BINARY            ((ruim_instrns_enum_type) 0xB0)
#define UPDATE_BINARY          ((ruim_instrns_enum_type) 0xD6)
#define READ_RECORD            ((ruim_instrns_enum_type) 0xB2)
#define UPDATE_RECORD          ((ruim_instrns_enum_type) 0xDC)
#define SEEK                   ((ruim_instrns_enum_type) 0xA2)
#define INCREASE               ((ruim_instrns_enum_type) 0x32)
#define VERIFY_CHV             ((ruim_instrns_enum_type) 0x20)
#define CHANGE_CHV             ((ruim_instrns_enum_type) 0x24)
#define DISABLE_CHV            ((ruim_instrns_enum_type) 0x26)
#define ENABLE_CHV             ((ruim_instrns_enum_type) 0x28)
#define UNBLOCK_CHV            ((ruim_instrns_enum_type) 0x2C)
#define INVALIDATE             ((ruim_instrns_enum_type) 0x04)
#define REHABILITATE           ((ruim_instrns_enum_type) 0x44)
#define RUN_GSM_ALGORITHM      ((ruim_instrns_enum_type) 0x88)
#define SLEEP                  ((ruim_instrns_enum_type) 0xFA)
#define GET_RESPONSE           ((ruim_instrns_enum_type) 0xC0)
#define TERMINAL_PROFILE       ((ruim_instrns_enum_type) 0x10)
#define ENVELOPE               ((ruim_instrns_enum_type) 0xC2)
#define FETCH                  ((ruim_instrns_enum_type) 0x12)
#define TERMINAL_RESPONSE      ((ruim_instrns_enum_type) 0x14)
#ifdef FEATURE_AUTH
#define UPDATE_SSD             ((ruim_instrns_enum_type) 0x84)
#define BS_CHALLENGE           ((ruim_instrns_enum_type) 0x8A)
#define CONFIRM_SSD            ((ruim_instrns_enum_type) 0x82) 
#define RUN_CAVE               ((ruim_instrns_enum_type) 0x88)
#define GEN_KEY_VPM            ((ruim_instrns_enum_type) 0x8E)
#endif /* FEATURE_AUTH */
#ifdef FEATURE_OTASP
#define MS_KEY_REQUEST         ((ruim_instrns_enum_type) 0xE0)
#define KEY_GEN_REQUEST        ((ruim_instrns_enum_type) 0xE2)
#define CONFIG_REQUEST         ((ruim_instrns_enum_type) 0xE6)
#define DOWNLOAD_REQUEST       ((ruim_instrns_enum_type) 0xE8)
#define SSPR_CONFIG_REQUEST    ((ruim_instrns_enum_type) 0xEA)
#define SSPR_DOWNLOAD_REQUEST  ((ruim_instrns_enum_type) 0xEC)
#define OTAPA_REQUEST          ((ruim_instrns_enum_type) 0xEE)
#define COMMIT                 ((ruim_instrns_enum_type) 0xCC)
#define VALIDATE               ((ruim_instrns_enum_type) 0xCE)
#endif /* FEATURE_OTASP */
#define STORE_ESN_ME           ((ruim_instrns_enum_type) 0xDE)

typedef enum {             
  TRANSPARENT = 0,            /* Transparent structure  */
  LINEAR_FIXED= 1,            /* Linear Fixed structure */ 
  CYCLIC      = 3             /* Cyclic structure       */
} ruim_file_structure_type;

typedef enum {
  RFU = 0,               /* Reserved for Future Use */
  MF = 1,                /* Master File             */
  DF = 2,                /* Dedicated File          */
  EF = 4                 /* Elementary File         */
} ruim_file_type;

typedef PACKED struct {
  byte           rfu1[2];         /* Reserved for Future Use (RFU) */
  byte           mem[2];          /* Amt of memory of selected directory which is
                                    not allocated to any of the DFs or EFs under
                                    the selected directory */
  byte           file_id[2];      /* File ID */
  ruim_file_type file_type;       /* File Type */
  byte           rfu2[5];         /* RFU */
  byte           len_data;        /* Length of the following GSM Specific data */
} ruim_sel_mf_df_rsp_type;

typedef PACKED struct {
  byte                  rfu[2];        /* RFU */
  byte                  file_size[2];  /* File Size */
  byte                  file_id[2];    /* File ID */
  ruim_file_type        file_type;     /* File Type */
  byte                  inc_allowed;   /* RFU/Inc Access for Cyclic File*/
  byte                  access[3];     /* Access Conditions */
  byte                  file_status;   /* File Status */
  byte                  len_data;      /* Length of following data */
  ruim_file_structure_type file_struct;/* Structure of EF */
  byte                  rec_len;       /* Length of record */
} ruim_sel_ef_rsp_type;

#ifdef FEATURE_OTASP

typedef PACKED struct {
  byte result;                         /* Result of Operation */
} ruim_ms_key_rsp_type;

typedef PACKED struct {
  byte result;                         /* Result of Operation */
  byte ms_result_len;                  /* Length of MS Result */
  byte ms_result[RUIM_PARAM_P];        /* MS Result */
} ruim_key_gen_rsp_type;

typedef PACKED struct {
  byte result;                         /* Result of Operation */
} ruim_commit_rsp_type;

typedef PACKED struct {
  byte result;                         /* Result of Operation */
  byte nam_lock_ind;                   /* NAM Lock Indication */
  byte rand_otapa[4];                  /* Rand OTAPA */
} ruim_otapa_rsp_type;

typedef PACKED struct {
  byte block_id;                       /* Block id */
  byte block_length;                   /* Block length */
  byte result_code;                    /* Result Code */
  byte data[RUIM_MAX_CHARS - 3];       /* Data from the card */
} ruim_cfg_rsp_type;

typedef PACKED struct {
  byte block_id;                       /* Block id */
  byte result_code;                    /* Result Code */
  byte block_length;                   /* Block length */
  byte data[RUIM_MAX_CHARS - 3];       /* Data from the card */
} ruim_sspr_cfg_rsp_type;

typedef PACKED struct {
  byte block_id;                       /* Block id */
  byte result_code;                    /* Result Code */
} ruim_dload_rsp_type;

typedef PACKED struct {
  byte block_id;                       /* Block id */
  byte result_code;                    /* Result Code */
  byte segment_offset[2];              /* Segment offset */
  byte segment_size;                   /* Segment size */
} ruim_sspr_dload_rsp_type;

typedef PACKED struct {
  byte block_id;                       /* Block id */
  byte result_code;                    /* Result Code */
} ruim_validn_rsp_type;

#endif /* FEATURE_OTASP */

typedef PACKED struct {
  byte usage_ind_conf;                 /* Usage Indicator Confirmation */
} ruim_store_esn_rsp_type;

typedef union {
  ruim_sel_mf_df_rsp_type mf_df;       /* Response to Select MF/DF */
  ruim_sel_ef_rsp_type    ef;          /* Response to Select EF */
#ifdef FEATURE_OTASP
  ruim_ms_key_rsp_type    ms_key;      /* Response to MS Key Request */
  ruim_key_gen_rsp_type   key_gen;     /* Response to Key Gen Request */
  ruim_commit_rsp_type    commit;      /* Response to Commit */
  ruim_otapa_rsp_type     otapa;       /* Response to OTAPA Request */
  ruim_cfg_rsp_type       cfg;         /* Response to CFG Rsp Type */
  ruim_sspr_cfg_rsp_type  sspr_cfg;    /* Response to SSPR CFG Rsp Type */
  ruim_dload_rsp_type     dload;       /* Response to Download Request */
  ruim_sspr_dload_rsp_type sspr_dload; /* Response to SSPR Download Request */
  ruim_validn_rsp_type    validn;      /* Response to Validation Request */
#endif /* FEATURE_OTASP */
  ruim_store_esn_rsp_type store_esn;   /* Response to Store ESN */
  byte                    data[RUIM_MAX_CHARS]; /* Response Data */
} ruim_rsp_type;

typedef struct {
  byte                  cmd_rsp_size;  /* Command Response Size */
  ruim_rsp_type         rsp;           /* Response Type */
  ruim_apdu_status_type cmd_status;    /* Command Status */
} ruim_rsp_buf_type;

typedef PACKED struct {

  byte  high_byte;                   /* High byte of the File to be selected */
  byte  low_byte;                    /* Low byte of the File to be selected */
} ruim_sel_mf_df_ef_req_type;

typedef PACKED struct {
  byte  chv[RUIM_MAX_CHV_DIGITS];    /* CHV to be verified*/
} ruim_verify_chv_type;

typedef PACKED struct {
  byte  chv[RUIM_MAX_CHV_DIGITS];    /* Unblock CHV */
  byte  new_chv[RUIM_MAX_CHV_DIGITS];/* New CHV to replace the old one */
} ruim_unblock_chv_type;

typedef PACKED struct {
  dword  rands;                      /* Random Seed */
} ruim_bs_chal_type;

typedef PACKED struct {
  qword  randssd;                    /* RandSSD from over the air */
  byte   proc_control;               /* Process Control Byte */
} ruim_update_ssd_type;

typedef PACKED struct {
  dword  authbs;                     /* AUTHBS */ 
} ruim_confirm_ssd_type;

#ifdef FEATURE_OTASP

typedef PACKED struct {
  byte randseed[RUIM_RAND_SEED];     /* Random Seed for Key Generation */
  byte a_key_p_rev;                  /* A_KEY_P_REV */
  byte param_p_len;                  /* Param P Length */
  byte param_g_len;                  /* Param G Length */
  byte param_p[RUIM_PARAM_P];        /* Param P */
  byte param_g[RUIM_PARAM_G];        /* Param G */
} ruim_ms_key_req_type;

typedef PACKED struct {
  byte bs_result_len;                /* Length of BS Result */
  byte bs_result[RUIM_PARAM_P];      /* BS Result */
} ruim_key_gen_req_type;

typedef PACKED struct {
  byte start_stop;                   /* Length of BS Result */
  dword rand_seed;                   /* Random Seed */
} ruim_otapa_req_type;

typedef PACKED struct {
  byte block_id;                     /* Block ID of the Config Request */
} ruim_cfg_req_type;

typedef PACKED struct {
  byte block_id;                     /* Block ID of the Config Request */
  byte req_offset[2];                /* Request Offset */
  byte req_max_size;                 /* Request Max Size */
} ruim_sspr_cfg_req_type;

typedef PACKED struct {
  byte block_id;                     /* Block ID of the Download Request */
  byte block_len;                    /* BLock length */
  byte data[RUIM_MAX_CHARS - 2];     /* Data buffer */
} ruim_dload_req_type;

typedef PACKED struct {
  byte block_id;                     /* Block ID of the Download Request */
  byte block_len;                    /* BLock length */
  byte data[RUIM_MAX_CHARS - 2];     /* Data buffer */
} ruim_sspr_dload_req_type;

typedef PACKED struct {
  byte block_id;                     /* Block ID of the Download Request */
  byte block_len;                    /* BLock length */
  byte data[RUIM_MAX_CHARS - 2];     /* Data buffer */
} ruim_validn_req_type;

#endif /* FEATURE_OTASP */

typedef PACKED struct {
  byte len_and_usage;                /* Length of usage and Usage */
  byte esn[7];                       /* ESN */
} ruim_store_esn_type;

typedef union {
  ruim_sel_mf_df_ef_req_type  mf_df_ef;          /* Select file */
  ruim_verify_chv_type        verify_chv;        /* Verify CHV */
  ruim_unblock_chv_type       unblk_chv;         /* Unblock CHV */
  ruim_bs_chal_type           bs_chal;           /* BS Challenge */
  ruim_update_ssd_type        update_ssd;        /* Update SSD */
  ruim_confirm_ssd_type       confirm_ssd;       /* Confirm SSD */
#ifdef FEATURE_OTASP
  ruim_ms_key_req_type        ms_key;            /* MS Key Request */
  ruim_key_gen_req_type       key_gen;           /* Key Gen Request */
  ruim_otapa_req_type         otapa_req;         /* OTAPA Request */
  ruim_cfg_req_type           cfg_req;           /* Cfg Request */
  ruim_sspr_cfg_req_type      sspr_cfg_req;      /* SSPR Cfg Request */
  ruim_dload_req_type         dload_req;         /* Download Request */
  ruim_sspr_dload_req_type    sspr_dload_req;    /* SSPR Download Request */
  ruim_validn_req_type        validn;            /* Validation Request */
#endif /* FEATURE_OTASP */
  ruim_store_esn_type         store_esn;         /* Store ESN Type */
  byte    data[RUIM_MAX_CHARS];
} ruim_req_type;

typedef PACKED struct {
  byte                    ruim_class; /* RUIM class of instruction           */
  ruim_instrns_enum_type  instrn;     /* Instruction type                    */
  byte                    p1;         /* input parameter 1                   */
  byte                    p2;         /* input parameter 2                   */
  byte                    p3;         /* input parameter 3                   */
} ruim_apdu_type;

typedef enum {
  ME_TO_RUIM,     /* Indicates data is sent from the ME to the RUIM          */
  RUIM_TO_ME,     /* Indicates data is sent from the RUIM to the ME          */
  NO_TRANSFER     /* Indicates no data is transfered.                        */
} ruim_data_dir_type;

typedef struct {
  ruim_apdu_type     apdu_hdr;          /* APDU header */
  ruim_data_dir_type data_dir;          /* data direction */
  ruim_req_type      req;               /* Request */
  ruim_rsp_buf_type  *rsp_ptr;          /* Response pointer */
  ruim_resp_callback_type rpt_function; /* call back function */
} ruim_req_buf_type;

/* 
  This type defines how to change the operations parameters.
 */
typedef struct {
  boolean change_clk_freq;           /* This indicates whether to change the UIM 
                                        clock frequency                       */
  ruim_clk_freq_type clock_frequency;/* This holds the selected clock 
                                        frequency                             */
  boolean change_baud_rate;          /* This indicates whether to change the UIM 
                                        interface baudrate                    */

  /* These parameters determine the baudrate of the UIM interface.            */
  ruim_clock_rate_conversion_factor_type FI;
                                    /* Holds the UIM clock rate conversion    */
  ruim_baud_rate_adjustment_factor_type  DI;
                                    /* Holds the baudrate adjustment factor   */
  boolean change_guardtime;         /* This indicates whether to change the UIM 
                                       guardtime                              */
  byte guardtime_bits;              /* Number of ETUs to increase the 
                                       guardtime                              */
} ruim_op_params_type;

/*
  This type defines the PPS request parameters.  The parameters contain the 
  PPS request as well as where to put the PPS response.
 */
typedef struct {
  byte size_of_pps_request;   /* This is the size of the PPS request          */
  byte *request_buf;          /* This points to the PPS request.              */
  ruim_rsp_buf_type  *rsp_ptr;/* This points to where to place the PPS        
                                 response                                     */
  ruim_resp_callback_type pps_resp_callback;/* Callback for response          */
} ruim_pps_req_buf_type;


/*---------------------------------------------------------------------------
 Macro to write to MSM UART registers. 
---------------------------------------------------------------------------*/

#ifdef T_MSM3
#define SIO_NOP 
#endif

#define MSMU_OUT( reg, val)   \
   (void)outp( (reg), (val)); \
   SIO_NOP
   
#define MSMUW_OUT( reg, val)   \
   (void)outpw( (reg), (val)); \
   SIO_NOP


/*===========================================================================

FUNCTION RUIM_DEV_INIT                             EXTERNALIZED FUNCTION

DESCRIPTION
  Initialize the clock regime, configures the GPIOs, MSM UART2 serial 
  interface.  It places the UIM power line low, the UIM reset line low,
  the I/O line low and the clock line low.

DEPENDENCIES
  The routine MUST be called before any other RUIM driver function.  It must
  only be called once on power up of the phone.

RETURN VALUE
  None

SIDE EFFECTS
  This function leaves ALL the UIM lines low after it executes.
  
===========================================================================*/
void ruim_dev_init
(
  void
);


/*===========================================================================

FUNCTION RUIM_POWER_UP                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function turns on the power for the UIM.  It puts the I/O line in 
  reception mode (stops sending break on the I/O line) and turns on the 
  UIM clock.

DEPENDENCIES
  This function must be executed either after ruim_dex_init or after 
  ruim_power_down.

RETURN VALUE
  None

SIDE EFFECTS
  After this function is executed, the UIM is powered up and has a clock.
  
===========================================================================*/
void ruim_power_up
(
  ruim_unsolict_resp_callback_type unsolict_resp_callback
);


/*===========================================================================

FUNCTION RUIM_RESET                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function asserts the reset line on the UIM.  Any data received by the
  phone will be flushed.  Any transfer from the phone will be terminated and 
  the buffers flushed.

DEPENDENCIES
  This function must be executed after ruim_dex_init.  The UIM should be powered
  up (via ruim_power_up) for this function to have any effect.

RETURN VALUE
  None

SIDE EFFECTS
  The RUIM driver is expecting an ATR after this reset.  
  
===========================================================================*/
void ruim_reset
(
  ruim_rsp_buf_type  *rsp_ptr,    /* Defines where to put the ATR           */
  ruim_resp_callback_type atr_rx_callback /* Callback for ATR               */
);


/*===========================================================================

FUNCTION RUIM_SEND_PPS                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function sets up the transfer of a PPS request to the UIM.  This 
  function starts the transfer, which, if all goes well, is finished by the
  ruim_rx_isr.

DEPENDENCIES
  This function starts a PPS exchange.  This exchange can only be done 
  immediately after an ATR is received.

RETURN VALUE
  None

SIDE EFFECTS
  The RUIM driver will continue to send out the PPS request.  Then the 
  driver will expect the PPS response.
  
===========================================================================*/
void ruim_send_pps
(
  ruim_pps_req_buf_type *pps_request  /* This points to the PPS request and 
                                         the PPS response buffer.           */
);


/*===========================================================================

FUNCTION RUIM_UPDATE_OP_PARAMS                         EXTERNALIZED FUNCTION

DESCRIPTION
  This function sets up the operational parameters of the RUIM interface.  It
  can change the baud rate of the RUIM interface, change the UIM clock 
  frequency or change the guardtime.

DEPENDENCIES
  This function changes the operational parameters of the RUIM interface.
  Whatever changes are requested in this function shall apply as soon as
  possible.  The interface will reflect the changes.

RETURN VALUE
  None

SIDE EFFECTS
  All transfers over the UIM interface will occur at the frequency, baudrate,
  or guardtime as specified in this function call.
  
===========================================================================*/
void ruim_update_op_params
(
  ruim_op_params_type *op_params       /* Proposed operational parameters */
);


/*===========================================================================

FUNCTION RUIM_SEND_COMMAND                          EXTERNALIZED FUNCTION

DESCRIPTION
  This function sets up the transfer of an APDU to the UIM.  This function 
  starts the transfer, which, if all goes well, is finished by the
  ruim_rx_isr.

DEPENDENCIES
  This function starts an APDU exchange.  The exchange can only happen after
  the ATR has been received and, optionally, after a PPS exchange.

RETURN VALUE
  None

SIDE EFFECTS
  The RUIM driver will continue to send out the APDU.  All the stages of 
  sending the APDU are handled by the RUIM driver.  Then the driver expects
  the APDU response.  Some commands solicit a GET_RESPONSE or a FETCH command
  to be sent after the APDU.
  
===========================================================================*/
void ruim_send_command
(
  ruim_req_buf_type *ruim_req /* This points to the APDU command buffer, the  
                                 command data buffer, the response buffer, the 
                                 APDU status and the callback function.       */
);


/*===========================================================================

FUNCTION RUIM_POWER_DOWN                          EXTERNALIZED FUNCTION

DESCRIPTION
  This function deactivates the UIM so it does becomes inactive and does not
  use power.  The electrical lines to the UIM are all set low in the 
  appropriate order.

DEPENDENCIES
  This function inactivates the UIM.  The ruim_power_up and ruim_reset 
  functions must be called after this function to bring the UIM back up
  to use.

RETURN VALUE
  None

SIDE EFFECTS
  After this function is executed, the UIM is power down and inactive.
  
===========================================================================*/
void ruim_power_down
(
  void
);

#endif /* RUIM_H */
