#ifndef NVA_H
#define NVA_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            N V   A L L O C A T I O N  H E A D E R  F I L E

GENERAL DESCRIPTION
  This header file contains all the definitions necessary to interface
  with the Non Volatile Memory Dynamic Allocation services.

Copyright (c) 1994,1995,1996,1997,1998,1999,2000 by QUALCOMM Incorporated.
All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/nva.h_v   1.15   29 Aug 2000 18:05:48   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/29/00   pg      Added support for NV_BAND_PREF_I and NV_ROAM_PREF_I 
                   based on FEATURE_SD20.
08/28/00   pg      Added support for NV_DIAG_RESTART_CONFIG_I based on 
                   FEATURE_DIAG_RESTART_CONFIG.
08/04/00   pg      Added support for NV_DATA_SCRM_ENABLED_I based on 
                   FEATURE_IS2000_R_SCH.
07/10/00   pg      Added support for R-UIM items.
06/13/00   pg      Added support for NV_PRIMARY_DNS_I, and NV_SECONDARY_DNS_I.
06/09/00   rp      Replaced FEATURE_PA_RANGE_DIG_COMP with existing
                   FEATURE_PA_RANGE_TEMP_FREQ_COMP.
05/19/00   pg      Added conditional compile features around the struct 
                   nvi_fixed_upper8_type so that whenever none of the flags are
                   defined in the structure the compiler doesn't complain 
                   about having no members.
                   Changed NAM_LOCK to be a per-NAM item.
                   Made NAM_LOCK and OTAPA_ENABLED support conditional
                   based on FEATURE_OTASP_OTAPA.  
                   Added support for new RFR3100 items based on 
                   FEATURE_RFR3100.
                   Added support for NV_TTY_I based on FEATURE_TTY.
                   Added Broadcast SMS items support based on 
                   FEATURE_BROADCAST_SMS; these items are in the upper 8K.
                   Changed #ifdef FEATURE_DS_PAP to 
                   #if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) ).
05/15/00   pg      Changed NV_GPS1_PDE_IP_TANSPORT_I to NV_GPS1_PDE_TRANSPORT_I.
                   Added support for NV_PA_RANGE_OFFSETS_I, NV_TX_COMP0_I, 
                   NV_TX_COMP1_I, NV_TX_COMP2_I, NV_TX_COMP3_I.
03/29/00   rp      Added limited support for nvi_verno_ext_type to track new 
                   feature definitions so that a rebuild will occur when
                   feature selection changes.
03/29/00   pg      Added support for NV_PREF_FOR_RC, NV_DS_DEFAULT_BAUD,
                   NV_DIAG_DEFAULT_BAUD, NV_SIO_DEV_MAP_MENU_ITEM.
                   Added support for COMET testing.
01/10/00   rp      Modified support for NV_CDMA_TX_LIN_MASTER_I to four
                   items since storage for original item exceeded 128 bytes.
12/02/99   rp      Removed all unnecessary MSM5000 RF calibration items.
10/18/99   rp      Added support for MSM5000 RF calibration items associated
                   with FEATURE_PA_RANGE_DIG_COMP and 
                   FEATURE_4PAGE_TX_LINEARIZER.
08/16/99   rp      Split NV_PAP_DATA_I into two items, NV_PAP_USER_ID_I and
                   NV_PAP_PASSWORD_I.  Put pap_password in the invariant area.
08/13/99   rp      Added support for NV_PAP_DATA_I.
06/30/99   kmp     Changed FEATURE_NV_JSTD008 to FEATURE_NV_TMSI and removed
                   PACKED from the enums for the ARM 2.50 compiler.
03/31/99   snn     Deleted all references to 80186 pragma packs.
03/31/99   snn     ls made following changes and I merged them in this file.
                   Made NV_OTKSL_I universal.
                   Added NV_DATA_AUTO_PACKET_DETECTION_I item.
                   Made UP link access key a separate item.
03/11/99   snn     ck did following changes and I added them in this file:
                   Ifdefed EVRC Service Negotiation NV item PREF_VOICE_SO_I
                   under FEATURE_EVRC_SO_MGMT
03/11/99   snn     ny did following changes and I added them in this file:
                   Cleaned up the compilation to remove warnings/error messges.
                   Applied merge guidelines 3.0 to clean up the code
02/11/99   ls      Merged OTASP 3.0.
02/05/99   ls      Changed featurization of NV_PREF_MODE_I.
02/03/99   ls      Introduced enums-as-words option.
02/02/99   ls      Added stack checker items.
01/14/99   ls      Added PDM tables support for ISS2 targets.
11/21/98   ls      Added nv_dp_capacity function and NV_OUTPUT_UI_KEYS_I.
10/28/98   dlb     Added support for ARM.
10/26/98   ls      Added NV_SRDA_ENABLED_I.
10/19/98   ls      Merged SSPR Enhancements.
10/01/98   ls      Changed EVRC item subtypes and added NV_SMS_AUTO_DELETE_I.
09/29/98   ls      Added EVRC, VRHFK, and new RF Cal items.
09/10/98   ls      Revived AUTO_ANSWER_SHADOW, added NV_SIO_PWRDWN_I.
08/27/98   ls      Added NV_AFAX_I, plus other configuration changes.
08/25/98   ls      Added CARRIER_INFO for module.
08/19/98   ls      Added BORSCHT items.
08/11/98   ls      Added NV_FULL_SYSTEM_PREF_I.
08/04/98   ls      Added rental timer enabled item support.
08/02/98   ls      Added rental counter item support, moved some items to 
                   unused areas of the invariant pool.
07/27/98   ls      Added rental timer items, removed fixed pointers from tps.
07/14/98   ls      Merged remainder of SSPR-800  support under internal 
                   feature control.
07/06/98   ls      Partial merge of SSPR-800: added IS-683A PRL support.
06/22/98   ls      Removed Unwired Planet cache.
06/12/98   ls      Added Unwired Planet alert items.
05/14/98   ls      Added NV_PDM1_VS_TEMP_I and NV_PDM2_VS_TEMP_I.
03/19/98   ls      Added PDM1, BELL_MODEM, and ALERTS_LVL_SHADOW.
01/20/98   ls      Added DTACO items, new RF Cal items, and version field in
                   roaming list.
12/16/97   ls      Added RF Cal pointer, elapsed_time item, and disconnect
                   timer.
11/26/97   ls      Removed several obsolete items.
11/05/97   ls      Added Unwired Planet support, version 2.
10/31/97   ls      Made OTKSL available to 2-NAM builds.
10/22/97   ls      ISS2 builds always have a 1K roaming list.
10/21/97   ls      Added data services items.
09/09/97   ls      Added constant for allocation map segment size.
08/01/97   ls      Added ifdef around ip address declare macro.
07/23/97   ls      Added initial support for Unwired Planet.
07/01/97   ls      Added 8K NV support for ISS2.
06/06/97   ls      Added voice_privacy_alert item for TGP.
06/05/97   ls      Added another phone book entry for ISS2.
06/03/97   ls      Implemented auto-detection of EEPROM configuration.
05/15/97   ls      Added support for OTASP-800.
04/30/97   ls      Added ISS2 dialing plan state table item.
04/02/97   ls      Added Clam-Shell Call Answer item, removed old MRU
                   table format.
02/13/97   ls      Added vibrator item, defined both MRU table formats.
01/03/97   ls      Featurized NV.
11/04/96   ls      Added invariant area to fixed pool to preserve A_KEY, 
                   SSD_A, and SSD_B across upgrades.
10/08/96   ls      Improved bad-powerdown recovery.
10/03/96   ls      Added NV_TIMED_PREF_MODE_I.
09/17/96   ls      Moved call stack and phone book to the fixed pool.
08/29/96   ls      Mass migration of items to the fixed pool.
08/06/96   ls      Added NV_OTKSL_I.
08/01/96   ls      Deleted home_block and curr_nam from NAM structure.
06/26/96   ls      Added Roaming List and MRU Table support.
06/17/96   ls      Added support for OTASP "commit" operation.
05/30/96   ls      Added initial subset of J-STD-008 NAM parameters and
                   extensions.
02/28/96   ls      Changed RF Cal items LNA_RANGE_RISE and LNA_RANGE_FALL
                   to int1 type.
02/27/96   ls      Added N1m nv item.
02/15/96   ls      Added RF Calibration items.
01/29/96   ras     Pre-code review cleanup
12/21/95   ras     split klt into klt as substructure in fixed pool
                   moved sec_code,lock_code,err_log to fixed pool. 
12/05/95   ras     moved fm_rssi and vbatt to the fixed pool.
12/04/95   ras     modified nvi_msl_type for nva_store_msl() change.
11/22/95   ras     added NV_HW_CONFIG. Cleanup "pack()" usage
10/02/95   ras     added fm_agc,fsc2,fsc2_chksum,wdc, moved lcd to fixed item.
09/11/95   ras     made a_key a fixed item bumped rev
09/10/95   ras     re-fixed NAM_DECLARE for iss2.
09/05/95   ras     fixed items now specify # nams, by target. Misc reduction
                   in RAM usage,  bad power down code
08/23/95   ras     added fixed items to klt
08/08/95   ras     lint cleanup, IMSI, bad pwr down structures
07/17/95   ras     added full dead cell recovery and sms support
06/11/95   ras     ifdef'd in some dead cell recover code
05/22/95   ras     pool sizes changes. small = 8 long = 32
05/09/95   ras     Initial next generation nv checkin
07/14/94   ras     Initial revision                                      

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

#include "target.h"
#include "nvi.h"
#include "nvd.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* uninitialized data value */
#define NVIUNINIT         ((word)0)  

/* Maximum offset into NVM */
#define NVIHIGHADDR       ((word)(NVD_MIN_OFFSET + NVD_MAX_OFFSET)) 

/* Minimum offset into NVM */
#define NVILOWADDR        ((word)(NVD_MIN_OFFSET))          

/* Total number of bytes in NVM */
#define NVIMEMSIZE        ((word)(NVIHIGHADDR - NVILOWADDR)) 

/* Where klt (and fixed pool) begin */
#define NVIKLTADDR        ((word)NVILOWADDR)         

/* Special value to indicate nvm has been built */
#define NVIKLTMAGIC       (((word)NVI_KLT_REV<<8)+NVI_VERNO_MIN)

/* number of block bits in an allocation map byte */
#define NVIBLOCKSPERBYTE  ((word)8)            

/* Numbers of bytes represented by a bit in the allocation map */
#define NVIBLOCKSIZE       ((word)8)            

/* Number of bytes examined at a time while allocating space */
#define NVIBYTESPERENTRY   ((word)(NVIBLOCKSPERBYTE*NVIBLOCKSIZE))

/* Number of blocks in a long allocation block */
#define NVIBLOCKSPERLONGALLOC ((word)4)

/* Number of bytes in a short allocation block */
#define NVISHORTBLOCK     ((word)NVIBLOCKSIZE)   

/* Number of bytes in a long allocation block */
#define NVILONGBLOCK      ((word)(NVIBLOCKSIZE*NVIBLOCKSPERLONGALLOC)) 

/* Number of allocation blocks possible in NVM */
#define NVINUMBLOCKS      ((word)(NVIMEMSIZE/NVIBLOCKSIZE)) 

/* Number of bytes in allocation map */
#define NVIMAPSIZE        ((word)(NVINUMBLOCKS / NVIBLOCKSPERBYTE))

/* Mask for 'no blocks allocated' in a byte of the allocation map */
#define NVIALLBLOCKSFREE  ((word)0) 

/* Offset within long block of the chain link to the next block */
#define NVICHAINOFFSET    ((word)0)

/* Offset of data in a long block */
#define NVIDATAOFFSET    ((word)1)  

/* Number of data bytes in a long block */
#define NVILONGDATASIZE  ((word)(NVILONGBLOCK - NVIDATAOFFSET)) 

/* Mask for 'all blocks allocated' in a byte of the allocation map */
#define NVINOFREEBLOCKS   0xff         

/* Depth of dead cell recovery frames */
#define NVI_ERR_STACK_DEPTH         12


/* The following constants are used to verify that the invariant */
/* items (A-Keys, SSDs) are properly located.  Invarient items   */
/* always have the same address in every build, to facilitate    */
/* preserving these items even across full NV rebuilds.          */
#define NV_A_KEY_NAM1_ADDR  0x0025
#define NV_A_KEY_NAM2_ADDR  0x002E
#define NV_A_KEY_NAM3_ADDR  0x0037
#define NV_A_KEY_NAM4_ADDR  0x0040
#define NV_UP_KEY1_ADDR     0x0049
#define NV_UP_KEY2_ADDR     0x005A
#define NV_VERNO_EXT_ADDR   0x006B
#define NV_SSD_A_NAM1_ADDR  0x0085
#define NV_SSD_A_NAM2_ADDR  0x008E
#define NV_SSD_A_NAM3_ADDR  0x0097
#define NV_SSD_A_NAM4_ADDR  0x00A0
#define NV_SSD_B_NAM1_ADDR  0x00E5
#define NV_SSD_B_NAM2_ADDR  0x00EE
#define NV_SSD_B_NAM3_ADDR  0x00F7
#define NV_SSD_B_NAM4_ADDR  0x0100
#define NV_PAP_PASSWORD_ADDR  0x00A9


/* begin packing of data structures */
#ifdef FEATURE_UTF_WIN32
#error code not present
#endif
/*-------------------------------------------------------------------------*/
/* Stuff for recovering from dead nv cells.                                */

/* Enum for what class of write was in progress           */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NVI_SOL_W,            /* Misc. write type not handled by recovery code */
  NVI_MSL_WRITE,        /* write to the memory status list structure */
  NVI_TPS_WRITE,        /* write ontp the tps structure */
  NVI_KLT_WRITE,        /* write into the klt (can't be recovered from) */
  NVI_APPL_WRITE,       /* application writing to an allocated block */
  NVI_EXTPHNUM_WRITE,   /* write to a chained portion of an extended phone # */
  NVI_WRITE_ENUM_PAD = 0x7FFF /* Pad to 16 bits on ARM */
} nvi_write_enum_type;
#endif

/* Structure for error recovery from dead nv cells */
typedef PACKED struct {
  byte old_data[NVILONGBLOCK];   /* data in progress of writing to block */
  nvi_write_enum_type   wtype;   /* type of write in progress */
  word blocksize;                /* long pool or short pool */
  byte rev_ptr;                  /* offset of ptr to update */
  word tps_ptr;                  /* is the revlink in the tps? (head of list) */
  PACKED word *base_offset;      /* caller's offset variable */
  word current_offset;           /* Where the block started or moved to */
  boolean recovery_in_progress;  /* semaphore for recursion avoidance */
} nvi_write_err_type;

/* Enum for magic numbers for in progress activities during power down   */
#ifdef FEATURE_NV_ENUMS_ARE_WORDS
#error code not present
#else
typedef enum {
  NVI_NOTHING_IN_PROGRESS_V = 0x367c,
  NVI_ALLOC_IN_PROGRESS_V   = 0x1a1a,
  NVI_FREE_IN_PROGRESS_V    = 0x25e5,
  NVI_INPROGRESS_ENUM_PAD   = 0x7FFF /* Pad to 16 bits on ARM */
} nvi_inprogress_enum_type;
#endif

/*-------------------------------------------------------------------------*/
/* Memory Status List (MSL)                                                */
/* This contains a bit map of memory status, one bit per short block       */

typedef PACKED struct {
  byte   map_ptr[NVIMAPSIZE];   /* the bit map of memory */ 
  word   nvdiv;                 /* the division between short and long pools */
  dword  rsvd_chksum_00;        /* rsvd integrity check */
  dword  rsvd_chksum_01;        /* rsvd integrity check */
  dword  rsvd_chksum_02;        /* rsvd integrity check */
  dword  rsvd_chksum_03;        /* rsvd integrity check */
  word   rsvd1_XXX;             /* rsvd integrity check */
/*------------------------------------------------------------------------*/
/* The next six items are used for consistency checking of the dynamic   */
/* memory pools. If we powerdown while in a call to nva_alloc() or        */
/* nva_free(), we can not recover, and at power up we must rebuild the    */
/* dynamic pools. This is way the alloc_in_progress and free_in_progress  */
/* fields are used for. We create 2 fields for this so that we get 1/2 of */
/* the nv cell degredation as most nva_alloc() calls are associated with  */
/* and nva_free() call. The field newest_allocation contains the offset   */
/* of an allocation that has just been allocated and may be in the        */
/* process of being written to, but has not yet been linked in. If we     */
/* power down before this allocation is linked in, we must free it on     */
/* power up or suffer some memory loss. ptr_to_alloc contains the offset  */
/* that newest_allocation will be linked in to. In case that the item has */
/* been linked in, but newest_allocation has not been reset to zero,      */
/* reading the value ot the offset contained in ptr_to_alloc, will tell   */
/* us that nothing needs to be free'd, if the value at offset ptr_to_alloc*/
/* is equal to newest_allocation.                                         */


  PACKED struct {
    word   newest_allocation;    /* alloc, in process of being written to. */
    word   ptr_to_alloc_offset;  /* were alloc offset written to when updated */
    word   ptr_to_alloc_index;   /* were alloc offset written to when updated */
    word   tps_index;            /* item's tps table index */ 
    word   alloc_in_progress;    /* nva_alloc not completed? */
    word   free_in_progress;     /* nva_free not completed? */
  } bad_pwr_dwn;
} nvi_msl_type;

/* Table of Pointers to Structures (TPS) structure.                        */
typedef PACKED struct {
  word  used_bytes;              /* # of bytes allocated in nv */
  word  tps_size;                /* # of bytes in tps. When minor version
                                  * number rolls this is an easy way
                                  * to determine if the tps needs to be
                                  * reallocated to expand for a larger
                                  * items_ptr; voiding an nv build.
                                  */
  word  msl;                     /* memory status list (free/used/dead) */
  word  item_ptrs[NVI_TPS_MAX_I]; /* Array of internal item pointers */
} nvi_tps_type;


/*********************************************************************/
/*********Fixed read/write items are in the tail end of the KLT*******/
/*********************************************************************/

typedef PACKED struct {
    dword esn;                      /* ESN */
    dword esn_chksum;               /* ESN checksum */
    byte  verno_maj;                /* NV major version number */
    byte  verno_min;                /* NV Minor version number */
    word  dynamic_pool_capacity;    /* Max size of dynamic pool */
    word  tps;                      /* Table of pointers to structures */
    dword esn_valid;                /* ESN is valid flag */
    /* klt_magic must be last member of structure, see nva.c */
    dword klt_magic;                /* magic number for a valid KLT */
  } nvi_klt_type;

typedef PACKED struct {
  nvi_klt_type klt;
/*********Per NAM items***********************************************/
/* The old NV task runtime allocated static/fixed data structures.
 * This effectively determined the size of the nams at run time.
 * Rather than have a large chunk of code dedicated to the sizing of
 * of NAMs and the error log on a per target basis, the macros, below,
 * were created.
 */
#if(NV_MAX_NAMS == 4)
#define NAM_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3
#elif(NV_MAX_NAMS == 1)
#define NAM_DECLARE(A,B) A B
#elif(NV_MAX_NAMS == 2)
#define NAM_DECLARE(A,B) A B; A B##_1
#else
#error Unsupported number of NAMS
#endif


#define ERR_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; A B##_10; \
            A B##_11; A B##_12; A B##_13; A B##_14; \
            A B##_15; A B##_16; A B##_17; A B##_18; A B##_19; A B##_20


#if(NV_MAX_ALARMS == 6)
#define ALARM_DECLARE(A,B) A B; A B##1; A B##_2; A B##_3; A B##_4; A B##_5
#else
#error Unsupported number of Alarms
#endif


#if(NV_DIAL_STATE_TABLE_MAX_SIZE == 125)
#define DIAL_STATE_TABLE_DECLARE1(A,B) A B; A B##_1; A B##_2; A B##_3; \
            A B##_4; A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; \
            A B##_10; A B##_11; A B##_12; A B##_13; A B##_14; A B##_15; \
            A B##_16; A B##_17; A B##_18; A B##_19; A B##_20; A B##_21; \
            A B##_22; A B##_23; A B##_24; A B##_25; A B##_26; A B##_27; \
            A B##_28; A B##_29; A B##_30; A B##_31; A B##_32; A B##_33; \
            A B##_34; A B##_35; A B##_36; A B##_37; A B##_38; A B##_39; \
            A B##_40; A B##_41; A B##_42; A B##_43; A B##_44; A B##_45; \
            A B##_46; A B##_47; A B##_48; A B##_49; A B##_50; A B##_51; \
            A B##_52; A B##_53; A B##_54; A B##_55; A B##_56; A B##_57; \
            A B##_58; A B##_59; A B##_60; A B##_61; A B##_62; A B##_63; \
            A B##_64; A B##_65; A B##_66; A B##_67; A B##_68; A B##_69; \
            A B##_70; A B##_71; A B##_72; A B##_73; A B##_74; A B##_75; \
            A B##_76; A B##_77; A B##_78; A B##_79; A B##_80; A B##_81; \
            A B##_82; A B##_83; A B##_84; A B##_85; A B##_86; A B##_87; \
            A B##_88; A B##_89; A B##_90; A B##_91; A B##_92; A B##_93; \
            A B##_94; A B##_95; A B##_96; A B##_97; A B##_98; A B##_99
#define DIAL_STATE_TABLE_DECLARE2(A,B) A B; A B##_101; A B##_102;  \
            A B##_103; A B##_104; A B##_105; A B##_106; A B##_107; \
            A B##_108; A B##_109; A B##_110; A B##_111; A B##_112; \
            A B##_113; A B##_114; A B##_115; A B##_116; A B##_117; \
            A B##_118; A B##_119; A B##_120; A B##_121; A B##_122; \
            A B##_123; A B##_124             
#else
#error Unsupported number of Dial State Table entries
#endif


#if(NV_MAX_STACK_DIALS == 2)
#define STACK_DIAL_DECLARE(A,B) A B; A B##_1
#elif(NV_MAX_STACK_DIALS == 10)
#define STACK_DIAL_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9
#else
#error Unsupported number of Stack Dials
#endif


#if(NV_MAX_SPEED_DIALS + NVI_REDIAL_EXTENSIONS == 1)
#define SPEED_DIAL_DECLARE(A,B) A B
#elif(NV_MAX_SPEED_DIALS + NVI_REDIAL_EXTENSIONS == 33)
#define SPEED_DIAL_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; A B##_10; \
            A B##_11; A B##_12; A B##_13; A B##_14; A B##_15; A B##_16; \
            A B##_17; A B##_18; A B##_19; A B##_20; A B##_21; A B##_22; \
            A B##_23; A B##_24; A B##_25; A B##_26; A B##_27; A B##_28; \
            A B##_29; A B##_30; A B##_31; A B##_32
#elif(NV_MAX_SPEED_DIALS + NVI_REDIAL_EXTENSIONS == 34)
#define SPEED_DIAL_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; A B##_10; \
            A B##_11; A B##_12; A B##_13; A B##_14; A B##_15; A B##_16; \
            A B##_17; A B##_18; A B##_19; A B##_20; A B##_21; A B##_22; \
            A B##_23; A B##_24; A B##_25; A B##_26; A B##_27; A B##_28; \
            A B##_29; A B##_30; A B##_31; A B##_32; A B##_33
#elif(NV_MAX_SPEED_DIALS + NVI_REDIAL_EXTENSIONS == 100)
#define SPEED_DIAL_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; A B##_10; \
            A B##_11; A B##_12; A B##_13; A B##_14; A B##_15; A B##_16; \
            A B##_17; A B##_18; A B##_19; A B##_20; A B##_21; A B##_22; \
            A B##_23; A B##_24; A B##_25; A B##_26; A B##_27; A B##_28; \
            A B##_29; A B##_30; A B##_31; A B##_32; A B##_33; A B##_34; \
            A B##_35; A B##_36; A B##_37; A B##_38; A B##_39; A B##_40; \
            A B##_41; A B##_42; A B##_43; A B##_44; A B##_45; A B##_46; \
            A B##_47; A B##_48; A B##_49; A B##_50; A B##_51; A B##_52; \
            A B##_53; A B##_54; A B##_55; A B##_56; A B##_57; A B##_58; \
            A B##_59; A B##_60; A B##_61; A B##_62; A B##_63; A B##_64; \
            A B##_65; A B##_66; A B##_67; A B##_68; A B##_69; A B##_70; \
            A B##_71; A B##_72; A B##_73; A B##_74; A B##_75; A B##_76; \
            A B##_77; A B##_78; A B##_79; A B##_80; A B##_81; A B##_82; \
            A B##_83; A B##_84; A B##_85; A B##_86; A B##_87; A B##_88; \
            A B##_89; A B##_90; A B##_91; A B##_92; A B##_93; A B##_94; \
            A B##_95; A B##_96; A B##_97; A B##_98; A B##_99
#elif (NV_MAX_SPEED_DIALS + NVI_REDIAL_EXTENSIONS == 102)
#define SPEED_DIAL_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; A B##_10; \
            A B##_11; A B##_12; A B##_13; A B##_14; A B##_15; A B##_16; \
            A B##_17; A B##_18; A B##_19; A B##_20; A B##_21; A B##_22; \
            A B##_23; A B##_24; A B##_25; A B##_26; A B##_27; A B##_28; \
            A B##_29; A B##_30; A B##_31; A B##_32; A B##_33; A B##_34; \
            A B##_35; A B##_36; A B##_37; A B##_38; A B##_39; A B##_40; \
            A B##_41; A B##_42; A B##_43; A B##_44; A B##_45; A B##_46; \
            A B##_47; A B##_48; A B##_49; A B##_50; A B##_51; A B##_52; \
            A B##_53; A B##_54; A B##_55; A B##_56; A B##_57; A B##_58; \
            A B##_59; A B##_60; A B##_61; A B##_62; A B##_63; A B##_64; \
            A B##_65; A B##_66; A B##_67; A B##_68; A B##_69; A B##_70; \
            A B##_71; A B##_72; A B##_73; A B##_74; A B##_75; A B##_76; \
            A B##_77; A B##_78; A B##_79; A B##_80; A B##_81; A B##_82; \
            A B##_83; A B##_84; A B##_85; A B##_86; A B##_87; A B##_88; \
            A B##_89; A B##_90; A B##_91; A B##_92; A B##_93; A B##_94; \
            A B##_95; A B##_96; A B##_97; A B##_98; A B##_99; A B##_100; \
            A B##_101 
#elif(NV_MAX_SPEED_DIALS + NVI_REDIAL_EXTENSIONS == 103)
#define SPEED_DIAL_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; A B##_10; \
            A B##_11; A B##_12; A B##_13; A B##_14; A B##_15; A B##_16; \
            A B##_17; A B##_18; A B##_19; A B##_20; A B##_21; A B##_22; \
            A B##_23; A B##_24; A B##_25; A B##_26; A B##_27; A B##_28; \
            A B##_29; A B##_30; A B##_31; A B##_32; A B##_33; A B##_34; \
            A B##_35; A B##_36; A B##_37; A B##_38; A B##_39; A B##_40; \
            A B##_41; A B##_42; A B##_43; A B##_44; A B##_45; A B##_46; \
            A B##_47; A B##_48; A B##_49; A B##_50; A B##_51; A B##_52; \
            A B##_53; A B##_54; A B##_55; A B##_56; A B##_57; A B##_58; \
            A B##_59; A B##_60; A B##_61; A B##_62; A B##_63; A B##_64; \
            A B##_65; A B##_66; A B##_67; A B##_68; A B##_69; A B##_70; \
            A B##_71; A B##_72; A B##_73; A B##_74; A B##_75; A B##_76; \
            A B##_77; A B##_78; A B##_79; A B##_80; A B##_81; A B##_82; \
            A B##_83; A B##_84; A B##_85; A B##_86; A B##_87; A B##_88; \
            A B##_89; A B##_90; A B##_91; A B##_92; A B##_93; A B##_94; \
            A B##_95; A B##_96; A B##_97; A B##_98; A B##_99; A B##_100; \
            A B##_101; A B##_102
#elif(NV_MAX_SPEED_DIALS + NVI_REDIAL_EXTENSIONS == 110)
#define SPEED_DIAL_DECLARE(A,B) A B; A B##_1; A B##_2; A B##_3; A B##_4; \
            A B##_5; A B##_6; A B##_7; A B##_8; A B##_9; A B##_10; \
            A B##_11; A B##_12; A B##_13; A B##_14; A B##_15; A B##_16; \
            A B##_17; A B##_18; A B##_19; A B##_20; A B##_21; A B##_22; \
            A B##_23; A B##_24; A B##_25; A B##_26; A B##_27; A B##_28; \
            A B##_29; A B##_30; A B##_31; A B##_32; A B##_33; A B##_34; \
            A B##_35; A B##_36; A B##_37; A B##_38; A B##_39; A B##_40; \
            A B##_41; A B##_42; A B##_43; A B##_44; A B##_45; A B##_46; \
            A B##_47; A B##_48; A B##_49; A B##_50; A B##_51; A B##_52; \
            A B##_53; A B##_54; A B##_55; A B##_56; A B##_57; A B##_58; \
            A B##_59; A B##_60; A B##_61; A B##_62; A B##_63; A B##_64; \
            A B##_65; A B##_66; A B##_67; A B##_68; A B##_69; A B##_70; \
            A B##_71; A B##_72; A B##_73; A B##_74; A B##_75; A B##_76; \
            A B##_77; A B##_78; A B##_79; A B##_80; A B##_81; A B##_82; \
            A B##_83; A B##_84; A B##_85; A B##_86; A B##_87; A B##_88; \
            A B##_89; A B##_90; A B##_91; A B##_92; A B##_93; A B##_94; \
            A B##_95; A B##_96; A B##_97; A B##_98; A B##_99; A B##_100; \
            A B##_101; A B##_102; A B##_103; A B##_104; A B##_105; A B##_106; \
            A B##_107; A B##_108; A B##_109
#else
#error Unsupported number of Speed Dials
#endif


#ifdef NV_FEATURE_WEB_ITEMS
#error code not present
#endif


#ifdef NV_FEATURE_CARRIER_ITEMS
#error code not present
#endif


/*-------------------------------------------------------------------*/
/* This is the "invariant area" of the fixed pool.  The items below  */
/* marked "INVARIENT OFFSET" must *never* be moved from the          */
/* specified offsets.  The gaps between these invarient items can,   */
/* however, be used just like any other NV space -- just be sure to  */
/* reverify that the invarient offsets didn't move.                  */
/* The invarient area exists to store certain items which are        */
/* preserved across full NV rebuilds (such as A-keys and SSDs).      */
/*                                                                   */
  nvi_word_type       reserved_word_1; /* INVARIANT OFFSET = 0x0016  */
  nvi_word_type       reserved_word_2; /* Reserved to contain a ptr  */
                                       /* to the RF Cal parameters   */
                                       /* for compliance testing     */
  nvi_word_type       reserved_word_3; /* Reserved to contain the    */
                                       /* "elapsed_time" timer value */
                                       /* for formal test            */
  nvi_word_type       reserved_word_4;
  nvi_word_type       reserved_word_5;
  nvi_a_key_type      a_key_nam1_1;    /* INVARIANT OFFSET = 0x0025  */
  nvi_a_key_type      a_key_nam2_1;    /* INVARIANT OFFSET = 0x002E  */
  nvi_a_key_type      a_key_nam3_1;    /* INVARIANT OFFSET = 0x0037  */
  nvi_a_key_type      a_key_nam4_1;    /* INVARIANT OFFSET = 0x0040  */
  nvi_up_key_type     up_key_1;        /* INVARIENT OFFSET = 0x0049  */
  nvi_up_key_type     up_key_2;        /* INVARIENT OFFSET = 0x005A  */
  nvi_verno_ext_type  nv_verno_ext;    /* INVARIENT OFFSET = 0x006B  */
  /* - - - - - - - - - - - - - - - - */
  nvi_byte_type               init_mute;         /* (2 bytes) */
  nvi_byte_type               cdma_pn_mask;      /* (2 bytes) */
  nvi_boolean_type            onetouch_dial;     /* (2 bytes) */
  nvi_byte_type               alerts_lvl;        /* (2 bytes) */
  nvi_byte_type               alerts_lvl_shadow; /* (2 bytes) */
  nvi_byte_type               ringer_lvl_shadow; /* (2 bytes) */
  nvi_byte_type               beep_lvl_shadow;   /* (2 bytes) */
  nvi_byte_type               ear_lvl_shadow;    /* (2 bytes) */
  nvi_byte_type               voice_priv;        /* (2 bytes) */
  nvi_byte_type               slot_cycle_index;  /* (2 bytes) */
  byte                        spacer1;
  /* - - - - - - - - - - - - - - - - */
  nvi_ssd_type        ssd_a_nam1_1;    /* INVARIANT OFFSET = 0x0085  */
  nvi_ssd_type        ssd_a_nam2_1;    /* INVARIANT OFFSET = 0x008E  */
  nvi_ssd_type        ssd_a_nam3_1;    /* INVARIANT OFFSET = 0x0097  */
  nvi_ssd_type        ssd_a_nam4_1;    /* INVARIANT OFFSET = 0x00A0  */
  nvi_pap_password_type pap_password;  /* INVARIENT OFFSET = 0x00A9  */   
  /* - - - - - - - - - - - - - - - - */
  nvi_byte_type               mob_cai_rev;       /* (2 bytes) */
  nvi_sid_nid_list_type       sid_nid_list;      /* (8 bytes) */
  nvi_word_type               spare_1;           /* (3 bytes) */
  nvi_word_type               spare_2;           /* (3 bytes) */
  nvi_byte_type               ringer_type;       /* (2 bytes) */
  nvi_byte_type               ringer_spkr_lvl;   /* (2 bytes) */
  nvi_byte_type               beep_spkr_lvl;     /* (2 bytes) */
  nvi_boolean_type            vibrator;          /* (2 bytes) */
  nvi_boolean_type            flip_answers;      /* (2 bytes) */
  nvi_boolean_type            data_qnc_enabled;  /* (2 bytes) */
  nvi_byte_type               data_so_set;       /* (2 bytes) */
  nvi_boolean_type            sms_utc;           /* (2 bytes) */
  nvi_boolean_type            roam_ringer;       /* (2 bytes) */
  nvi_byte_type               ear_level;         /* (2 bytes) */
  nvi_byte_type               speaker_level;     /* (2 bytes) */
  nvi_byte_type               ringer_level;      /* (2 bytes) */
  nvi_byte_type               beep_level;        /* (2 bytes) */
  nvi_boolean_type            call_beep;         /* (2 bytes) */
  /* - - - - - - - - - - - - - - - - */
  nvi_ssd_type        ssd_b_nam1_1;    /* INVARIANT OFFSET = 0x00E5  */
  nvi_ssd_type        ssd_b_nam2_1;    /* INVARIANT OFFSET = 0x00EE  */
  nvi_ssd_type        ssd_b_nam3_1;    /* INVARIANT OFFSET = 0x00F7  */
  nvi_ssd_type        ssd_b_nam4_1;    /* INVARIANT OFFSET = 0x0100  */
  /* - - - - - - - - - - - - - - - - */
  nvi_byte_type               curr_nam;          /* (2 bytes) */
  nvi_boolean_type            auto_nam;          /* (2 bytes) */
  nvi_dword_type              nxtreg;            /* (5 bytes) */
  nvi_word_type               lstsid;            /* (3 bytes) */
  nvi_word_type               locaid;            /* (3 bytes) */
  nvi_boolean_type            pureg;             /* (2 bytes) */
  nvi_boolean_type            cont_key_dtmf;     /* (2 bytes) */
  nvi_boolean_type            cont_str_dtmf;     /* (2 bytes) */
  nvi_boolean_type            svc_area_alert;    /* (2 bytes) */
  nvi_boolean_type            call_fade_alert;   /* (2 bytes) */
  nvi_byte_type               auto_power;        /* (2 bytes) */
  nvi_auto_answer_type        auto_answer;       /* (3 bytes) */
  nvi_auto_redial_type        auto_redial;       /* (3 bytes) */
  nvi_boolean_type            auto_hyphen;       /* (2 bytes) */
  nvi_byte_type               back_light;        /* (2 bytes) */
  nvi_boolean_type            auto_mute;         /* (2 bytes) */
  nvi_maintrsn_type           maintrsn;          /* (3 bytes) */
  nvi_lckrsn_p_type           lckrsn_p;          /* (3 bytes) */
  nvi_byte_type               lock;              /* (2 bytes) */
  nvi_boolean_type            auto_lock;         /* (2 bytes) */
  nvi_word_type               call_rstrc;        /* (3 bytes) */
  nvi_boolean_type            horn_alert;        /* (2 bytes) */
  nvi_boolean_type            data_srvc_enabled; /* (2 bytes) */
  nvi_word_type               idle_data_timeout; /* (3 bytes) */
  byte                        spacer3;
  /* - - - - - - - - - - - - - - - - */

/*                                                                   */
/* End of invariant section of the fixed pool                        */
/* Note: Next item should have address 0x0145.                       */
/*-------------------------------------------------------------------*/
/*********Per NAM items **********************************************/

  NAM_DECLARE(nvi_word_type,           nam_chksum);
  NAM_DECLARE(nvi_min1_type,           min1);
  NAM_DECLARE(nvi_min2_type,           min2);
  NAM_DECLARE(nvi_mob_term_type,       mob_term_home);
  NAM_DECLARE(nvi_mob_term_type,       mob_term_for_sid);
  NAM_DECLARE(nvi_mob_term_type,       mob_term_for_nid);
  NAM_DECLARE(nvi_min_byte_type,       accolc);
  NAM_DECLARE(nvi_min_word_type,       min_chksum);
  NAM_DECLARE(nvi_name_type,           name_nam);
  NAM_DECLARE(nvi_imsi_11_12_type,     imsi_11_12);
  NAM_DECLARE(nvi_imsi_mcc_type,       imsi_mcc);
  NAM_DECLARE(nvi_dir_number_type,     dir_number);
  NAM_DECLARE(nvi_byte_type,           count);
  NAM_DECLARE(nvi_dword_type,          air_cnt);
  NAM_DECLARE(nvi_dword_type,          roam_cnt);
  NAM_DECLARE(nvi_dword_type,          air_timer);
  NAM_DECLARE(nvi_dword_type,          roam_timer);
  NAM_DECLARE(nvi_sys_pref_type,       system_pref);
#ifdef FEATURE_EVRC_SO_MGMT
  NAM_DECLARE(nvi_pref_voice_so_type,  pref_voice_so);
#else
  NAM_DECLARE(nvi_rsvd_item_1_type,    rsvd_item_1);
#endif
  NAM_DECLARE(nvi_enum_type,           pref_mode);

#ifdef NV_FEATURE_IMSI_T_ITEMS
  NAM_DECLARE(nvi_min1_type,           imsi_t_s1);
  NAM_DECLARE(nvi_min2_type,           imsi_t_s2);
  NAM_DECLARE(nvi_imsi_mcc_type,       imsi_t_mcc);
  NAM_DECLARE(nvi_imsi_11_12_type,     imsi_t_11_12);
  NAM_DECLARE(nvi_byte_type,           imsi_t_addr_num);       
#endif

#ifdef FEATURE_NV_TMSI
  NAM_DECLARE(nvi_byte_type,           assigning_tmsi_zone_length);
  NAM_DECLARE(nvi_qword_type,          assigning_tmsi_zone);
  NAM_DECLARE(nvi_dword_type,          tmsi_code);
  NAM_DECLARE(nvi_dword_type,          tmsi_exp_timer);
#endif /* FEATURE_NV_TMSI */

#ifdef NV_FEATURE_SSPR_ENHANCEMENTS
  NAM_DECLARE(nvi_home_sid_nid_type,   home_sid_nid);
  NAM_DECLARE(nvi_sid_nid_lock_type,   sid_nid_lock);
#else
  NAM_DECLARE(nvi_sid_nid_type,        sid_nid);
#endif

#ifdef FEATURE_OTASP_OTAPA
  NAM_DECLARE(nvi_enabled_type,        otapa_enabled);
  NAM_DECLARE(nvi_enabled_type,        nam_lock);
#endif


#if (defined(NV_FEATURE_SSPR_ENHANCEMENTS) || defined(FEATURE_NV_ANALOG))
  NAM_DECLARE(nvi_cdmach_type,         pcdmach);
  NAM_DECLARE(nvi_cdmach_type,         scdmach);
#endif

  NAM_DECLARE(nvi_byte_type,           imsi_addr_num);       
  NAM_DECLARE(nvi_dir_number_pcs_type, mob_dir_number);

#ifdef NV_FEATURE_PRL_ITEMS 
  NAM_DECLARE(nvi_mru2_table_type,     mru2_table);
  NAM_DECLARE(nvi_enabled_type,        prl_enabled);
#endif

#ifdef FEATURE_NV_ANALOG
  NAM_DECLARE(nvi_enum_type,       cdma_pref_serv);
  NAM_DECLARE(nvi_enum_type,       analog_pref_serv);  
  NAM_DECLARE(nvi_sid_lock_type,   cdma_sid_lock);
  NAM_DECLARE(nvi_sid_acq_type,    cdma_sid_acq);
  NAM_DECLARE(nvi_sid_lock_type,   analog_sid_lock);
  NAM_DECLARE(nvi_sid_acq_type,    analog_sid_acq);
  NAM_DECLARE(nvi_word_type,       analog_firstchp);
  NAM_DECLARE(nvi_word_type,       analog_home_sid);
  NAM_DECLARE(nvi_enum_type,       analog_reg);
  NAM_DECLARE(nvi_boolean_type,    analog_ex);
#endif /* FEATURE_NV_ANALOG */

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/*********The error log **********************************************/
  ERR_DECLARE(nvi_err_log_type,    err_log);

/*********RF Calibration items****************************************/
#ifdef FEATURE_NV_RF_CAL
  nvi_adj_vs_freq_type        fm_agc_adj_vs_freq;
  nvi_pwr_table_type          fm_agc_set_vs_pwr;
  nvi_freq_table_type         fm_agc_set_vs_freq;
  nvi_temp_table_type         fm_agc_set_vs_temp;
  nvi_pwr_table_type          fm_exp_hdet_vs_pwr;
  nvi_pwr_table_type          fm_err_slp_vs_pwr;
  nvi_byte_type               fm_freq_sense_gain;
  nvi_minmax_type             fm_lna_ctl;
#endif /* FEATURE_NV_RF_CAL */
  
  nvi_fm_tx_pwr_type          fm_tx_pwr;         /* (9 bytes) */
  nvi_byte_type               dm_addr;           /* (2 bytes) */
  nvi_byte_type               data_io_mode;      /* (2 bytes) */
  nvi_byte_type               dm_io_mode;        /* (2 bytes) */
  
  nvi_byte_type               send_timeout;      /* (2 bytes) */
  nvi_page_set_type           page_set;          /* (3 bytes) */
  nvi_dword_type              life_timer_g;
  nvi_dword_type              call_timer_g;
  nvi_dword_type              run_timer;
  nvi_cdmach_type             last_cdmach;
  nvi_boolean_type            time_show;
  nvi_boolean_type            send_pin;
  nvi_dword_type              pwr_dwn_cnt;
  nvi_dword_type              life_cnt;
  
  nvi_byte_type               cdma_rx_lin_off_0;
  nvi_cdma_rx_lin_type        cdma_rx_lin_slp;
  nvi_freq_int1_table_type    cdma_rx_comp_vs_freq;
  nvi_freq_int1_table_type    cdma_tx_comp_vs_freq;
  nvi_word_type               cdma_tx_lin_master_off_0;
  nvi_tx_lin_master_slp_type  cdma_tx_lin_master_slp;
  nvi_temp_table_type         cdma_tx_lin_vs_temp;
  nvi_volt_table_type         cdma_tx_lin_vs_volt;
  nvi_temp_table_type         cdma_tx_lim_vs_temp;
  nvi_freq_table_type         cdma_tx_lim_vs_freq;
  nvi_exp_hdet_vs_agc_type    cdma_exp_hdet_vs_agc;
  nvi_err_slp_vs_hdet_type    cdma_err_slp_vs_hdet;
  nvi_temp_int1_table_type    cdma_rx_lin_vs_temp;
  nvi_byte_type               cdma_adj_factor;
  nvi_temp_int1_table_type    cdma_rx_slp_vs_temp;
  nvi_temp_int1_table_type    cdma_tx_slp_vs_temp;

#ifdef NV_FEATURE_PDM_TABLES
  nvi_temp_int1_table_type    pdm1_vs_temp;
#else
  nvi_temp_int1_table_type    pa_range_vs_temp;
#endif

  nvi_temp_table_type         fm_exp_hdet_vs_temp;
  nvi_minmax_type             therm;
  nvi_minmax_type             vbatt_pa;
  nvi_minmax_type             vbatt;
  nvi_byte_type               hdet_off;
  nvi_byte_type               hdet_spn; 
  nvi_byte_type               lcd;
  nvi_byte_type               rf_config;
  nvi_byte_type               r1_rise;
  nvi_byte_type               r1_fall;
  nvi_byte_type               r2_rise;
  nvi_byte_type               r2_fall;
  nvi_byte_type               r3_rise;
  nvi_byte_type               r3_fall;
  nvi_byte_type               pa_range_step_cal;
  nvi_byte_type               lna_range_pol;
  nvi_int1_type               lna_range_rise;
  nvi_int1_type               lna_range_fall;
  nvi_word_type               lna_range_offset;

  nvi_fsc_type                fsc;
  nvi_minmax_type             fm_agc;
  nvi_fsc2_type               fsc2;
  nvi_fsc2_chksum_type        fsc2_chksum;
  nvi_wdc_type                wdc;
  nvi_byte_type               n1m;
  nvi_dword_type              hw_config; 
  nvi_minmax_type             fm_rssi;
  nvi_lock_code_type          lock_code;
  nvi_sec_code_type           sec_code;
 
  nvi_byte_type               pdm2;
  nvi_minmax_type             rx_agc_minmax;
  nvi_minmax_type             vbatt_aux;
  nvi_byte_type               pdm1;

#ifdef NV_FEATURE_PDM_TABLES
  nvi_temp_int1_table_type    pdm2_vs_temp;
  nvi_freq_int1_table_type    pdm1_vs_freq;
  nvi_freq_int1_table_type    pdm2_vs_freq;
#endif
  
  nvi_enum_type               dtaco_control;
  nvi_byte_type               dtaco_interdigit_timeout;
  nvi_sec_code_type           otksl;

  nvi_byte_type               scm;
  nvi_word_type               mob_firm_rev;
  nvi_byte_type               mob_model;
  nvi_word_type               config_chksum;
  nvi_zone_list_type          zone_list;
  nvi_dist_reg_type           dist_reg;

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
  nvi_pap_user_id_type        pap_user_id;
#endif

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif /* FEATURE_NV_ALARMS */

  nvi_byte_type               max_tx_adj;
  nvi_name_type               banner;
  nvi_temp_offset_type        fr_temp_offset;
  nvi_boolean_type            full_system_pref;
  nvi_boolean_type            vrhfk_enabled;
  nvi_boolean_type            vrhfk_voice_answer;
  nvi_boolean_type            sms_auto_delete;
  nvi_boolean_type            srda_enabled;
  nvi_boolean_type            output_ui_keys;
  nvi_boolean_type            spc_change_enabled;
  nvi_byte_type               data_auto_packet_detection;
  
#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif
  
  nvi_byte_type               data_mdr_mode;
  nvi_data_pkt_orig_str_type  data_pkt_orig_str;

#ifdef FEATURE_NV_ANALOG
  nvi_boolean_type            timed_pref_mode;
#endif

#ifdef NV_FEATURE_BELL_MODEM
#error code not present
#endif

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif

#ifdef FEATURE_NV_DIAL_PLAN
#error code not present
#endif

#ifdef FEATURE_NV_EXTENDED_UI
  nvi_boolean_type            any_key_answer;
  nvi_byte_type               back_light_hfk;
  nvi_enum_type               key_sound;
  nvi_enum_type               sorting_method;
  nvi_enum_type               language_selection;
  nvi_enum_type               menu_format;
  nvi_byte_type               voice_priv_alert;
#endif

#ifdef NV_FEATURE_ANS_SHADOW_ITEMS
#error code not present
#endif

#ifdef NV_FEATURE_SIO_PWRDWN_ITEMS
#error code not present
#endif

#ifdef NV_FEATURE_RENTAL_ITEMS
#error code not present
#endif

#ifdef NV_FEATURE_BORSCHT_ITEMS
#error code not present
#endif

#ifdef NV_FEATURE_TPR_ITEMS
#error code not present
#endif

  nvi_stack_idx_type                   stack_idx;
  STACK_DIAL_DECLARE(nvi_stdial_type,  stack);
  SPEED_DIAL_DECLARE(nvi_dial_type,    dial);

#ifdef NV_FEATURE_CARRIER_ITEMS
#error code not present
#endif 

#ifdef NV_FEATURE_WEB_ITEMS
#error code not present
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
  nvi_pa_offset_type          pa_offsets;
#endif

#ifdef FEATURE_4PAGE_TX_LINEARIZER
  nvi_tx_linearizer_type      cdma_tx_lin_master0;
  nvi_tx_linearizer_type      cdma_tx_lin_master1;
  nvi_tx_linearizer_type      cdma_tx_lin_master2;
  nvi_tx_linearizer_type      cdma_tx_lin_master3;
#endif

#ifdef FEATURE_RFR3100
#error code not present
#endif

#ifdef FEATURE_RUIM_CHV_FROM_NV
  nvi_ruim_chv_type              ruim_chv_1;
  nvi_ruim_chv_type              ruim_chv_2;
#endif

#ifdef FEATURE_IS2000
  nvi_enum_type                  pref_for_rc;
#endif

#ifdef FEATURE_DS_DEFAULT_BITRATE_NV
  nvi_enum_type                  ds_default_baud;
#endif

#ifdef FEATURE_DIAG_DEFAULT_BITRATE_NV
  nvi_enum_type                  diag_default_baud;
#endif

#ifdef FEATURE_SDEVMAP_MENU_ITEM_NV
  nvi_int1_type                  sio_dev_map_menu_item;
#endif

#ifdef FEATURE_TTY
#error code not present
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
  nvi_pa_range_offsets_type      pa_range_offsets;
  nvi_tx_comp_type               tx_comp0;
  nvi_tx_comp_type               tx_comp1;
  nvi_tx_comp_type               tx_comp2;
  nvi_tx_comp_type               tx_comp3;
#endif

  nvi_dword_type                 primary_dns;
  nvi_dword_type                 secondary_dns;

#ifdef FEATURE_IS2000_R_SCH
#error code not present
#endif

#ifdef FEATURE_DIAG_RESTART_CONFIG
  nvi_diag_restart_config_type   diag_restart_config;
#endif

/*********************************************************************/

  /* Next item marks the end of the fixed pool IF the roaming list */
  /* is to be stored in NVRAM's bank 2.  This item and the roaming */
  /* list should always be the last two items in the fixed pool.   */
  nvi_byte_type               eofp;

#ifdef NV_FEATURE_PRL_ITEMS
  #ifdef FEATURE_NV_1K_RL
    NAM_DECLARE(nvi_roaming_list_type,      roaming_list);
  #else
    NAM_DECLARE(nvi_roaming_list_fp_type,   roaming_list);
  #endif
#endif  

} nvi_fixed_type;

/* This structure defines the layout of the upper 8K address range */
#if (defined(NV_FEATURE_SI_DATA) || defined(NV_FEATURE_PRL_ITEMS) ||  \
     defined(NV_FEATURE_TRIMODE_ITEMS) || defined(FEATURE_BROADCAST_SMS))
/* (addresses 0x2000 to 0x4000)                                    */
typedef PACKED struct {
#ifdef NV_FEATURE_SI_DATA  
#error code not present
#endif

#ifdef NV_FEATURE_PRL_ITEMS
   NAM_DECLARE(nvi_roaming_list_type,  roaming_list);
#endif  

#ifdef FEATURE_BROADCAST_SMS
  nvi_word_type                  sms_bc_service_table_size;   
  nvi_sms_bc_service_table_type  sms_bc_service_table[NV_SMS_BC_TABLE_SIZE];                                                              
  nvi_byte_type                  sms_bc_config;                                                              
  nvi_byte_type                  sms_bc_user_pref;
#endif
} nvi_fixed_upper8_type;
#endif  

/* return to the default packing of data structures */
#ifdef FEATURE_UTF_WIN32
#error code not present
#endif

/*===========================================================================

FUNCTION NVA_INIT

DESCRIPTION
  This function initializes the fixed, msl, and memory pool if the nv ram is 
  being brought up for the first time. Otherwise it creates a RAM shadow 
  copy of these structures.

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S - if everything went well.

SIDE EFFECTS
  klt is rebuild upon version change, or error in existing klt values.

===========================================================================*/

nv_stat_enum_type nva_init( void );

/*===========================================================================

FUNCTION NVA_ALLOC

DESCRIPTION
  This function allocates a block of nv ram and returns it's offset
  within NVM.

DEPENDENCIES
  None.

RETURN VALUE
  The offset of the allocated block in NV RAM, if it worked.
  0 if the allocation failed. (KLT is always at zero, so zero can
  never be a valid return value).

SIDE EFFECTS
  Modifies the msl.

===========================================================================*/

word nva_alloc
(
  word size                 /* Number of bytes requested for allocation */
);


/*===========================================================================

FUNCTION NVA_FREE

DESCRIPTION
  This function free's previously allocated memory from either the
  short or the long allocation pool.

DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  NV_NOT_ALLOC_S if the block(s) to be freed were not allocated.
  NV_DONE_S if it worked.

SIDE EFFECTS
  msl will be modified.

===========================================================================*/

nv_stat_enum_type nva_free
(
  word block                            /* offset of block to be set free */
);

/*===========================================================================

FUNCTION NVA_SIZEOF

DESCRIPTION
  This function returns the number of bytes in an allocation. Note this size
  may be greater than the number of bytes originally requested when the
  block was allocated.

DEPENDENCIES
  None.

RETURN VALUE
  Size of the allocation in bytes.
  Zero is returned if the block passed in is not allocated.

SIDE EFFECTS
  None.

===========================================================================*/

word nva_sizeof
(
  word block  /* allocation to calculate the size of */
);

/*===========================================================================

FUNCTION NVA_SET_DP_CAPACITY

DESCRIPTION
  This function sets the shadow klt dynamic_pool_capacity field in the klt
  and writes it out.

DEPENDENCIES
  nva_init must have been called

RETURN VALUE
  NV_DONE_S if it worked

SIDE EFFECTS
  None

===========================================================================*/

nv_stat_enum_type nva_set_dp_capacity
(
  word  dp_capacity
);

/*===========================================================================

FUNCTION NVA_GET_DP_CAPACITY

DESCRIPTION
  This function returns the value of the dynamic_pool_capacity field in 
  the klt.

DEPENDENCIES
  nva_init must have been called

RETURN VALUE
  NV_DONE_S if it worked

SIDE EFFECTS
  None

===========================================================================*/

word nva_get_dp_capacity
(
  void
);

/*===========================================================================

FUNCTION NVA_MEMAVAIL

DESCRIPTION
  This function returns the amount of long pool memory available. 
  This includes the potential moving of nvdiv.  NVI_RSVD_MEM_SIZ 
  is not counted into the available memory, so that double buffered 
  items may still be written.

DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  The number of bytes of long pool memory - NVI_RSVD_MEM_SIZ.

SIDE EFFECTS
  None

===========================================================================*/

word nva_memavail
(
  nvi_pool_enum_type mempool     /* Long or short allocation pool info? */
);

/*===========================================================================

FUNCTION NVA_REALLOC

DESCRIPTION
  This function allocates additional memory on to the end of a long allocation
  if more memory is required. If the allocation is a short allocation,
  and more memory is required, it is promoted to a long allocation, and 
  the short allocation's data is copied over into the long allocation.

LIMITATIONS
  nva_realloc() does not shrink existing allocations

DEPENDENCIES
  nva_init must have been called.

RETURN VALUE
  Offset of the begining of the reallocated block.
  Zero is returned if the block passed in is not allocated, or if there
  is no more memory available.

SIDE EFFECTS
  May modify the msl.

===========================================================================*/

word nva_realloc
(
  word block,  /* allocation to calculate the size of */
  word newsize /* size, in bytes, which reallocation must fit */
);
/* <EJECT> */
/*===========================================================================

FUNCTION NVA_GET_BYTES

DESCRIPTION
  Memory read interface between software memory allocation layout and 
  device specific memory layout
  
DEPENDENCIES
  nva_init must be invoked prior to this call.

RETURN VALUE
  NV_DONE_S if it worked.

SIDE EFFECTS
  None

===========================================================================*/
nv_stat_enum_type nva_get_bytes
(
  void *buf,              /* buffer to read from */
  word offset,            /* offset of allocation */
  word index,             /* start byte within allocation */
  word len                /* number of bytes to write */
);
/* <EJECT> */
/*===========================================================================

FUNCTION NVA_PUT_BYTES

DESCRIPTION
  Memory write interface between software memory allocation layout and 
  device specific memory layout
  
DEPENDENCIES
  nva_init must be invoked prior to this call.

RETURN VALUE
  NV_DONE_S if it worked.

SIDE EFFECTS
  dynamic allocations may move if write failed and was reallocated.

===========================================================================*/
nv_stat_enum_type nva_put_bytes
  (
  void *buf,              /* buffer to read from */
  word offset,            /* offset of nv allocation */
  word index,             /* start byte within nv allocation */
  word len                /* number of bytes to write */
  );
/* <EJECT> */
/*===========================================================================

FUNCTION NVA_READ_FIXED

DESCRIPTION
  Read data from an offset in the fixed pool
   
DEPENDENCIES
  nva_init must be invoked prior to this call.

RETURN VALUE
  NV_DONE_S if it worked.

SIDE EFFECTS
  None 
===========================================================================*/ 
nv_stat_enum_type nva_read_fixed
( 
  void *dest_ptr,    /* Pointer to location to place read bytes */
  word index,        /* index into the fixed pool to read from */
  word len           /* Number of bytes to read */
);
/* <EJECT> */
/*===========================================================================

FUNCTION NVA_READ_TPS

DESCRIPTION
  Read an item's offset from the tps.
 
DEPENDENCIES
  nva_init must be invoked prior to this call.

RETURN VALUE
  NV_DONE_S if it worked.

SIDE EFFECTS
  None 
===========================================================================*/ 
nv_stat_enum_type nva_read_tps
( 
  void               *dest_ptr, /* Pointer to location to place read bytes */
  nvi_tps_items_type item       /* Item enum */
);
/* <EJECT> */
/*===========================================================================

FUNCTION NVA_WRITE_FIXED

DESCRIPTION
  Write data to an offset in the fixed NVM pool.
 
DEPENDENCIES
  nva_init must be invoked prior to this call.

RETURN VALUE
  NV_DONE_S if it worked.

SIDE EFFECTS
  None 
===========================================================================*/ 
nv_stat_enum_type nva_write_fixed
( 
  void *src_ptr,     /* Pointer to location to read bytes from */
  word index,        /* index into the fixed pool to write to */
  word len           /* Number of bytes to write */
);
/* <EJECT> */
/*===========================================================================

FUNCTION NVA_WRITE_TPS

DESCRIPTION
  Store an item offset into the tps structure in NVM.
 
DEPENDENCIES
  nva_init must be invoked prior to this call.

RETURN VALUE
  NV_DONE_S if it worked.

SIDE EFFECTS
  None 
===========================================================================*/ 
nv_stat_enum_type nva_write_tps
( 
  void                *src_ptr, /* Pointer to location to read bytes from */
  nvi_tps_items_type  item      /* Item enum */
);

/*===========================================================================

FUNCTION NVA_MEMCPY

DESCRIPTION
  This function copies a source nvm allocation to a destination 
  nvm allocation. If the destination is smaller than the source,
  only the destination's number of bytes are copied.

DEPENDENCIES
  Both offsets must be base addresses returned by nva_alloc.

LIMITATIONS
  This routine is only for long block to long block or short block to
  short block, copies at this time.

RETURN VALUE
  NV_DONE_S - if it worked
  Status of functions called internnaly if it failed.

SIDE EFFECTS
  None.

===========================================================================*/
nv_stat_enum_type nva_memcpy
(
  word dst_offset,            /* Who gets written to */
  word src_offset             /* Where we read from */
);

/*===========================================================================

FUNCTION NVA_STORE_BADPWRDN_INFO

DESCRIPTION
  Store information required to free an allocation which is in the midst
  of being written to, but not linked in, when we loose power.

  Note that passing in zeroes effectively states that operation which we
  were concerned with has completed, and that bad power down
  checking is now disabled.

DEPENDENCIES

RETURN VALUE
  NV_DONE_S - if everything went well.

SIDE EFFECTS
  None.

===========================================================================*/
nv_stat_enum_type nva_store_badpwrdn_info
(
  word allocation,            /* offset of newest allocation in nvm */
  word offset,                /* base offset allocation is stored */
  word index                  /* index allocation is stored */
); 


/*===========================================================================

FUNCTION NVA_STORE_BADPWRDN_INFO_TPS

DESCRIPTION
  Store information required to free an allocation which is in the midst
  of being written to, but not linked in, when we loose power. Used
  by double buffered write operations which only have tps item information.

DEPENDENCIES

RETURN VALUE
  NV_DONE_S - if everything went well.

SIDE EFFECTS
  None.

===========================================================================*/
nv_stat_enum_type nva_store_badpwrdn_info_tps
(
  word                allocation,  /* offset of newest allocation in nvm */
  nvi_tps_items_type  item         /* Item enum */
); 


/*===========================================================================

FUNCTION NVA_SET_BADPWRDN_TPS_INDEX

DESCRIPTION
  This function sets, in the MSL, the nvi_tps_items_type index of 
  the NV item being operated on (allocated or freed) so bad powerdown 
  recovery can identify which item was lost.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void nva_set_badpwrdn_tps_index
(
  nvi_tps_items_type item    /* Identity of item being allocated or freed */
);


/*===========================================================================

FUNCTION NVA_GET_TPS_PTR

DESCRIPTION
  This function returns the offset of the item pointer in the tps array
  for the specified item.

DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  Offset of the item within the tps array.                      

SIDE EFFECTS
  None.

===========================================================================*/
#ifdef FEATURE_NV_DEADCELL_RECOVERY
word nva_get_tps_ptr(nvi_tps_items_type item);
#endif /* FEATURE_NV_DEADCELL_RECOVERY */


/*===========================================================================

FUNCTION NVA_GET_TPS_OFFSET_PTR

DESCRIPTION
  This function returns the address of item pointer in the tps array
  for the specified item.

DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  Pointer to the item within the tps array.                      

SIDE EFFECTS
  None.

===========================================================================*/
#ifdef FEATURE_NV_DEADCELL_RECOVERY
PACKED word *nva_get_tps_offset_ptr(nvi_tps_items_type item);
#endif /* FEATURE_NV_DEADCELL_RECOVERY */


/*===========================================================================

FUNCTION NVA_PUSH_ERR_FRAME

DESCRIPTION
  This function increments the pointer for the error stack frame

DEPENDENCIES
  nva_err_rec_index must have been initialized.

RETURN VALUE
  NV_DONE_S is the stack did not overflow.

SIDE EFFECTS
  modifies nva_err_rec_index.

===========================================================================*/
nv_stat_enum_type  nva_push_err_stack (void );
  

/*===========================================================================

FUNCTION NVA_POP_ERR_FRAME

DESCRIPTION
  This function decrements the pointer for the error stack frame

DEPENDENCIES
  nva_err_rec_index must have been initialized.

RETURN VALUE
  NV_DONE_S is the stack did not underflow.

SIDE EFFECTS
  modifies nva_err_rec_index.

===========================================================================*/
nv_stat_enum_type  nva_pop_err_stack (void);


/*===========================================================================

FUNCTION NVA_SET_ERR_FRAME

DESCRIPTION
  This function sets the values of the error frame currently pointed to 
  on the error frame stack.

DEPENDENCIES
  nva_err_rec_index must have been initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

#ifdef FEATURE_NV_DEADCELL_RECOVERY
void  nva_set_err_frame 
  (
  nvi_write_err_type *err_rec  /* error rec data */
  );
#endif /* FEATURE_NV_DEADCELL_RECOVERY */


/*===========================================================================

FUNCTION NVA_GET_ERR_FRAME

DESCRIPTION
  This function returns the error frame currently pointed to on the
  error frame stack.

DEPENDENCIES
  If nothing is on the error frame stack, you get garbage.

RETURN VALUE
  Pointer to the error frame structure on the error frame stack.

SIDE EFFECTS
  None.

===========================================================================*/
#ifdef FEATURE_NV_DEADCELL_RECOVERY
nvi_write_err_type *nva_get_err_frame ( void );
#endif /* FEATURE_NV_DEADCELL_RECOVERY */


/*===========================================================================

FUNCTION NVA_ADDRESS_OF_ROAMING_LIST

DESCRIPTION
  This function returns the physical address in NVRAM of the roaming
  list (for multiple-NAM builds, the physical address of the NAM #1
  roaming list).  This address depends on the capacity of the installed 
  NVRAM part.

DEPENDENCIES
  The function cannot be called until NV is initialized.

LIMITATIONS
  None.

RETURN VALUE
  Absolute address of the roaming list.

SIDE EFFECTS
  None.

===========================================================================*/

word  nva_address_of_roaming_list( void );


/*===========================================================================

FUNCTION NVA_ADDRESS_OF_UPPER_8K_ITEM

DESCRIPTION
  This function returns the physical address in NVRAM of the fixed item in
  buffer #2.  This item only exists if the installed EEPROM is 16K bytes or
  larger (for smaller EEPROMS, this function returns NULL).
 
DEPENDENCIES
  The function cannot be called until NV is initialized.

LIMITATIONS
  None.

RETURN VALUE
  Absolute address of the fixed item

SIDE EFFECTS
  None.

===========================================================================*/

word  nva_address_of_upper_8k_item
(  
  nv_items_enum_type item           /* Item to access */
);

/*===========================================================================

FUNCTION NVA_REBUILD_NEEDED

DESCRIPTION
  This function returns TRUE if an NV rebuild is needed, FALSE otherwise.
  A rebuild is needed if any part of the version designator stored in
  NV does not match the corresponding build version designator.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE   - if a rebuild is needed
  FALSE  - if no rebuild is needed

SIDE EFFECTS
  None.

===========================================================================*/

boolean nva_rebuild_needed( void );

#endif /* NVA_H */

