#ifndef MCSYSPR_H
#define MCSYSPR_H
/*===========================================================================

                  M C S Y S P R   H E A D E R   F I L E

DESCRIPTION
  This file contains definitions and external interfaces to MCSYSPR.C,
  MCSYSPRA.C, and MCSYSPRD.C.

Copyright (c) 1997, 1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mcsyspr.h_v   1.7   24 Apr 2001 12:47:26   kkaya  $

when       who     what, where, why
--------   ---     -------------------------------------------------------
04/24/01   kk      Added new function call mcsyspr_ndss_on_progress()
02/13/01   ht      Rename and modify the bit position of sys_is_negative.
01/24/01   ych     Merged T-53/JCDMA features for SK.
12/19/00   ht      Add function prototype mcsyspr_same_prl_sys_as_acquired().
12/11/00   ht      Free up two bits in the "mode" of mcsyspr_pref_stat_type,
                   one bit is used to implement FEATURE_PRL_EXACT_NID_MATCH.                   
11/08/00   ht      Added function prototype mcsyspr_service_icon_off().
07/12/00   kk      added variable mcsys_connect_rxed added.
04/06/00   va      Merged the following from PP (MSM3100_CP.03.00.17):
03/22/00   vn      Changed roam_ind size to 8 bits if FEATURE_IS683A_PRL is
                   defined - same as ROAM_IND_LENGTH value - CR 13281.
03/16/00   vn      Merged fix for CR7346.
03/09/00   vn      Merged Wildcard SID feature.
03/06/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           kk      return cause after redirection failure added.
           kk      new function prototype mcsyspr_e911_callback_next_channel()
                   added.
           ht      Merge in Trimode code Alpha release.
           ych     Added support for Trimode Silent Redial
           ht      Add mcsyspr_last_active_cdma_channel_type definition.
           vn      Added a new scan mode GEN_RET_IF_FAIL to support peeking
                   back at redirecting system after GSRM to a CDMA system.
           ht      (cah)Remove SID/NID Lockout List references for Trimode.
           vn      Set AMPS Idle timer to 2 min. for trimode.
01/27/00   va      Merged in the following changes: 
                   added new flag to track ndss originations from a 
                   non preferred system.
                   Fixed CR12051 by removing MCSYSPR_SCAN_LIST_AVAILABLE
                   and instead leaving the scan list mode alone for available
                   scanning.  Changing it from MAX_ACCESS_PROBES was the bug.
10/22/99   abh     Added mcsyspr_rl_enabled flag indicating if PRL enabled
08/24/99   kmp     Changed prototype of prl_format() to return boolean
                   instead of void.
07/30/99   fc      Put in changes based on code review comments on Analog 
                   Registration Randomization.
07/06/99   abh     Added prototype for the function mcsyspr_find_last_sys_in_sl() 
07/16/99   ht      Add fixes for CR6955 to attempt acquisition on the actual
                   lost channel after a new system exit.  This includes
                   externalizing some functions in mcsysci.c.
05/12/99   kmp     Merge the IS-95B NDSS and System Reselection Features.
05/03/99   kk      NDSS function prototype and ndss flag added.
           kk      Added the new scan mode and the new scan list mode
                   for system reselection
04/15/99   abh     Fixed some lint warnings.
04/08/99   dna     Add channel to SL is not just for SSPR800
02/12/99   ck      Externalised mcc_roaming_list
11/21/98   dna     Fix CR7142.  Change AMPS timeout to 2 min. for dual-mode.
11/03/98   ych     Fixed the compile problem with previous checkin.
10/28/98   ych     Added prototype for mcsyspr_add_analog_sys_to_sl()
10/21/98   pms     Removed some suspicious _near variables.
10/05/98   ht      Add extern for PRL reformat support.
10/02/98   pms     Neared more variables to save ROM space.
09/28/98   ht      Multi-Geo SID structure is allocated based on projected
                   system table size allowed in NV.
09/22/98           Added support to correctly process CDMA available GAM
                   when the serving AMPS system and the new CDMA system are
                   on the same tier.
09/15/98   pms     Backed out _near pointers
09/08/98   ht      Add pri field in mcsyspr_pref_stat_type.  Modify the
                   declarations of mcsyspr_roaming_list.
09/04/98   pms     Neared the variables to save ROM space.
08/31/98   abh     Took out mcsyspr_system_pref outside the feature definitions
                   to fix compilation problems in Module.
08/21/98   ych     Added prototype for mcsyspr_add_channel_to_sl().
08/06/98   ych     Fixed the previous checkin AMPS_IDLE_TIMEOUT mistake.
                   Also added otasp related function prototypes.
08/04/98   ych     Merged E911, SSPR-800, and Acq. Improvements. Also added
                   SSPR CDMA available GAM processing support.Modified
                   MCSYSPR_AMPS_IDLE_TIMEOUT to be 1 minute for all targets.
07/22/98   abh     Merged E911, SSPR-800, Acq. Improvements
06/17/98   wjn     Added Feature E911 Support.
05/26/98   jrs     Added an external definition for mcsyspr_alt_system.
04/20/98   ht      Add #defines to support prioritizing systems between geo region
04/08/98   abh     Added function prototypes needed for splitting mcsyspr.c
                   to create mcsysci.c, prl.c & mcsyssup.c
04/03/98   ych     Replaced MCSYSPR_IDLE_TIMEOUT with MCSYSPR_CDMA_IDLE_TIMEOUT
                   for cdma entry reasons and MCSYSPR_AMPS_IDLE_TIMEOUT for
                   AMPS entry reasons.
03/27/98   ht      Add extern word mcsyspr_curr_geo_region and multi-geo-region
                   SID data structure
03/20/98   abh     Enhancements for Home Only
03/17/98   ych     Removed the extra headers which were introduced during
                   the 1.9.2.2 -> 1.14 -> 1.15 merge.
03/16/98   ych     Modified mcsyspr_scan_list_type to include db_roam_type.
03/05/98   ych     Featurized MCSYSPR_IDLE_TIMEOUT. When FEATURE_IDLE_
                   TIMEOUT_ONE is defined, the MCSYSPR_IDLE_TIMEOUT is 1
                   else, MCSYSPR_IDLE_TIMEOUT is set to three.
02/18/98   dna     Moved mcsyspr_valid_cellular_cdmach and
                   mcsyspr_valid_pcs_cdma_chan to mcsyspr.
02/02/98   abh     Merged Changes for PRL Force Mode.
01/28/97   wjn     Added micro definitions for PRL Version function support.
01/16/98   ych     Added "customer.h"...
01/15/98   ych     Changed the MCSYSPR_IDLE_TIMEOUT value to 1 from 3
11/10/97   ych     Added prototype for mcsyspr_get_amps_system.
10/27/97   jrs     Added a new way to build the scan list: return if fail mode.
08/06/97   jrs     Added an extern for mcsyspr_get_roam_ind().
07/01/97   jrs     Added some externs.
06/06/97   jrs     Modified OTASP to support the new MC to MCSYSPR interface.
05/30/97   jrs     Added the extern mcsyspr_prev_asl_index.
05/20/97   ych     Added a new state: MCSYSPR_LPM_STATE for LPM.
04/16/97   jrs     Updates for deep sleep so that every channel is scanned
                   during each wakeup.
03/24/97   jrs     Changed mru_table to mru2_table. Removed debug code.
01/20/97   jrs     Created module.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "customer.h"
#include "mc.h"
#include "prl.h"
#include "prli.h"
#include "db.h"
#include "mcsys.h"
#include "mcc.h"


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                         Data Definitions                                */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* This type is used to map a block to its associated preferred CDMA       */
/* channels                                                                */
typedef struct
{
  byte block;
  word cdma_channel;
} mcsyspr_block_to_cdma_chan_type;

/* This is the type of the scan list                                       */
typedef struct
{
  byte mode;                        /* The mode: CDMA or analog            */
  byte band_class;                  /* The band class: cellular or PCS     */
  nv_cs_union_type cs;              /* CDMA channel or cellular system     */
  word acquired_sid;                /* The SID acquired on this channel    */
  word acquired_nid;                /* The NID acquired on this channel    */
  db_roam_type roam_ind;            /* Roaming Indicator                   */
} mcsyspr_scan_list_type;

/* This is the type of the alternate scan list                             */
typedef struct
{
  byte mode;                        /* The mode: CDMA or analog            */
  byte band_class;                  /* The band class: cellular or PCS     */
  nv_cs_union_type cs;              /* CDMA channel or cellular system     */
} mcsyspr_alt_scan_list_type;

/* This structure is used to keep track of the most recent active
 * CDMA channel and band calss information.
 * It is updated by Call Processing via a function call provided
 * by mcsyspr module.
 */
typedef struct
{
  word ocdmach;               /* Original CDMA channel            */
  byte oband_class;           /* Original band class: cellular or PCS */
  word cdmach;                /* The CDMA channel            */
  byte band_class;            /* The band class: cellular or PCS     */
} mcsyspr_last_active_cdma_channel_type;

extern mcsyspr_last_active_cdma_channel_type mcsyspr_last_active_cdma_channel;

#ifdef FEATURE_E911
/* This is the type of the e911 available list                             */
typedef struct
{
  byte band_class;                  /* The band class: cellular or PCS     */
  nv_cs_union_type cs;              /* CDMA channel or cellular system     */
} mcsyspr_e911_avail_list_type;

/* This is the type of the CDMA channel from which a (G)SRM was received.  */
/* Used to build/rebuild the E911 Scan List when "REDIRECTION" is enabled. */
typedef mcsyspr_e911_avail_list_type mcsyspr_e911_redir_chan_type;
#endif /* FEATURE_E911 */

/* The possible states the phone can be in                                 */
typedef enum
{
  MCSYSPR_ONLINE_DIGITAL_STATE,
  MCSYSPR_OFFLINE_DIGITAL_STATE,
  MCSYSPR_RESET_STATE,
  MCSYSPR_POWERDOWN_STATE,
  MCSYSPR_ONLINE_ANALOG_STATE,
  MCSYSPR_OFFLINE_ANALOG_STATE
#ifdef FEATURE_LPM
#error code not present
#endif /* FEATURE_LPM */
} mcsyspr_state_enum_type;

/* This enum keeps track of the mode in which we are scanning              */
typedef enum
{
  MCSYSPR_SCAN_NORMAL,                  /* Normal scan list operation      */
  MCSYSPR_SCAN_AVAILABLE_LIST,          /* Scanning the available list     */
  MCSYSPR_SCAN_ALT_LIST,                /* Scanning the alternate scan list*/
  MCSYSPR_SCAN_SYSTEM_LOST,             /* Scanning in lost system mode    */
  MCSYSPR_SCAN_DEEP_SLEEP,              /* Time to go to sleep             */
  MCSYSPR_SCAN_WAKEUP,                  /* Scanning during wakeup from deep*/
                                        /* sleep                           */
  MCSYSPR_SCAN_ORIG_INTERRUPTION,       /* Our scan was interrupted by an  */
                                        /* origination                     */
  MCSYSPR_SCAN_PERIODIC_RESCAN,         /* Periodic rescan for more        */
                                        /* desirable service               */
  MCSYSPR_SCAN_REACQUIRE_SPECIFIC,      /* Attempting to reacquire a       */
                                        /* specific system                 */
  MCSYSPR_SCAN_PROVISIONING,            /* Scanning in OTASP provisioning  */
                                        /* mode                            */
  MCSYSPR_SCAN_PREF_PROVISIONING,       /* Scanning in OTASP provisioning  */
                                        /* Preferred (Non-standard) mode   */
  MCSYSPR_SCAN_MAX_ACCESS_PROBES,       /* Scanning in Max Access Probes   */
                                        /* mode                            */
  MCSYSPR_SCAN_CDMA_LOCKED,             /* Scanning after a CDMA Lock Order*/
                                        /* has been received               */
  MCSYSPR_SCAN_E911_NORMAL,             /* Sequentially Scanning E911 Scan */
                                        /* for E911 Origination Service    */
  MCSYSPR_SCAN_E911_SYSTEM_LOST,        /* Scanning for E911 Origination   */
                                        /* Service after System loss       */
  MCSYSPR_SCAN_E911_LOCK,               /* Scanning for E911 origination   */
                                        /* after a CDMA Lock Order         */
  MCSYSPR_SCAN_E911_CALLBACK,           /* Scanning for E911 Callback      */
                                        /* Service                         */
#if defined (FEATURE_SSPR_800) && defined(FEATURE_SSPR_ENHANCEMENTS)
  MCSYSPR_SCAN_CDMA_AVAIL_GAM,          /* Scanning when CDMA available    */
                                        /* GAM is received                 */
#endif /* defined (FEATURE_SSPR_800) && defined(FEATURE_SSPR_ENHANCEMENTS) */
#ifdef FEATURE_IS95B_SYS_RESELEC
  MCSYSPR_SCAN_SYS_RESELEC,             /* Scanning in the system          */
                                        /* reselection mode                */
#endif /* FEATURE_IS95B_SYS_RESELEC */

#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  MCSYSPR_SCAN_SILENT_REDIAL,           /* Scanning in silent redial mode  */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
  MCSYSPR_SCAN_COUNT
} mcsyspr_scan_mode_type;

/* This enum keeps track of the way the scan list was built                */
typedef enum
{
  MCSYSPR_SCAN_LIST_NORMAL,       /* This is how the scan list is normally */
                                  /* built:                                */
                                  /*   the channels from the MRU table     */
                                  /*   the channels from the roaming list  */

  MCSYSPR_SCAN_LIST_SYSTEM_LOST,  /* The scan list is built this way after */
                                  /* CDMA returns a system lost exit code: */
                                  /*   the channel that was lost           */
                                  /*   other chans from the same geo region*/
                                  /*   the channels from the MRU table     */
                                  /*   the remaining chans in the roaming  */
                                  /*     list                              */

  MCSYSPR_SCAN_LIST_AUTO_NAM,     /* The scan list is built this way when  */
                                  /* we make an auto NAM switch. The scan  */
                                  /* list is built normally, then the      */
                                  /* acquired channel (if it is in the     */
                                  /* scan list) is move into the first     */
                                  /* position so that it will be acquired  */
                                  /* rapidly.                              */

  MCSYSPR_SCAN_LIST_PROVISIONING, /* The scan list is built this way when  */
                                  /* system determination is entered with  */
                                  /* a provisioning indication:            */
                                  /*   MRU channels that match the         */
                                  /*     selected block                    */
                                  /*   the remaining channels in the       */
                                  /*     roaming list                      */

  MCSYSPR_SCAN_LIST_MAX_ACCESS,   /* The scan list is built this way after */
                                  /* a max access probes exit from CDMA:   */
                                  /*   Any analog systems in the geo region*/
                                  /*   Other systems in the same geo region*/
                                  /*   Remaining chans from the MRU table  */
                                  /*   Remaining chans from the roaming    */
                                  /*   The CDMA channel upon which we      */
                                  /*     received the max access probes    */
                                  /*     exit                              */

  MCSYSPR_SCAN_LIST_CDMA_LOCKED,  /* The scan list is built this way after */
                                  /* a CDMA Lock Order has been received:  */
                                  /*   Any analog systems in the geo region*/
                                  /*   The CDMA channel upon which we      */
                                  /*     received the lock order           */
                                  /*   Other chans in the same geo region  */
                                  /*   Remaining chans from the MRU table  */
                                  /*   Remaining chans from the roaming    */
                                  /*     list                              */

  MCSYSPR_SCAN_LIST_RET_IF_FAIL,  /* The scan list is built this way after */
                                  /* receiving a Global Service            */
                                  /* Redirection Message that redirects    */
                                  /* us to AMPS:                           */
                                  /*   The CDMA system that redirected us  */
                                  /*   The AMPS system we were redirected  */
                                  /*     to                                */
                                  /*   The MRU table                       */
                                  /*   The acquisition table               */

  MCSYSPR_SCAN_LIST_E911          /* The scan list is built this way after */
                                  /* receiving a MC_PREF_SYS_CHANGED       */
                                  /* command from UI with a new mode of    */
                                  /* NV_MODE_E911:                         */
                                  /*   Analog systems if FEATURE_ACP       */
                                  /*   The CDMA channels from E911         */
                                  /*     available List                    */
                                  /*   The CDMA channels from MRU Table    */
                                  /*   The CDMA channels from Acq. Table   */
                                  /*   The remaining preferred CDMA        */
                                  /*     channels                          */

#ifdef FEATURE_IS95B_SYS_RESELEC
  ,MCSYSPR_SCAN_LIST_SYS_RESELEC  /* The scan list is built this way after */
                                  /* CDMA returns a system reselect exit   */
                                  /* code:                                 */
                                  /*   the analog systems received         */
                                  /*   the channels from the MRU table     */
                                  /*   the channels from the roaming list  */
#endif /* FEATURE_IS95B_SYS_RESELEC */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  ,MCSYSPR_SCAN_LIST_GEN_RET_IF_FAIL  /* The scan list is built this way after */
                                      /* receiving a Global Service            */
                                      /* Redirection Message that redirects    */
                                      /* us to another system:                 */
                                      /*   The CDMA system that redirected us  */
                                      /*   The system we were redirected to    */
                                      /*   The MRU table                       */
                                      /*   The acquisition table               */
#endif /* FEATURE_CDMA_800 && FEATURE_CDMA_1900 */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  ,MCSYSPR_SCAN_LIST_SILENT_REDIAL /* Silent Redial Scan list               */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
} mcsyspr_scan_list_mode_type;

/* This enum keeps track of where the serving system was found             */
typedef enum
{
  MCSYSPR_SOURCE_NO_SYS,          /* No serving system                     */
  MCSYSPR_SOURCE_PREFERRED,       /* Serving system is a Preferred RL      */
                                  /* entry                                 */
  MCSYSPR_SOURCE_NEGATIVE,        /* Serving system is a Negative RL entry */
  MCSYSPR_SOURCE_AVAILABLE_LIST,  /* Serving system is from the available  */
                                  /* list                                  */
  MCSYSPR_SOURCE_REDIRECTION,     /* We were redirected to the serving     */
                                  /* system                                */
  MCSYSPR_SOURCE_E911_AVAILABLE   /* Serving system is an E911 system      */
} mcsyspr_source_type;

/* This enum keeps track of what kind of scan we are performing during a   */
/* wakeup from deep sleep                                                  */
typedef enum
{
  MCSYSPR_WAKEUP_MRU0,
  MCSYSPR_WAKEUP_MICRO,
  MCSYSPR_WAKEUP_FULL
} mcsyspr_wakeup_type;

#define MCSYSPR_MG_NO_MATCH  0       /* Multi-geo SID No match */

#define MCSYSPR_OVERLAP_GEO      1   /* Overlapped geo region */
#define MCSYSPR_NO_OVERLAP_GEO   0   /* Non-overlapped geo region */

typedef struct
{
  union
  {
    int          status;      /* status */
    struct
    {
      unsigned int sys_is_neg:1;  /* system is negative */

#ifdef FEATURE_WCARD_SID_MGMT
      unsigned int wsid:1;
#endif
      unsigned int mode:2;      /* phone mode                   */
#ifdef FEATURE_IS683A_PRL       /* Bits in roam_ind same as 
                                   ROAM_IND_LENGTH              */
      unsigned int roam:8;      /* roam indicator               */
#else
      unsigned int roam:3;      /* roam indicator               */
#endif
#ifdef FEATURE_PRL_EXACT_NID_MATCH
      unsigned int match_nid:1;
#endif
      unsigned int pri:1;       /* priority */
      unsigned int geo:1;       /* geo region overlapped status */

#ifdef FEATURE_IS683A_PRL
 #if defined(FEATURE_WCARD_SID_MGMT) && defined(FEATURE_PRL_EXACT_NID_MATCH)
      unsigned int fill:1;     /* spare                        */
 #elif defined(FEATURE_WCARD_SID_MGMT) || defined(FEATURE_PRL_EXACT_NID_MATCH)
      unsigned int fill:2;     /* spare                        */
 #else
      unsigned int fill:3;     /* spare                        */
 #endif //defined(FEATURE_WCARD_SID_MGMT) && defined(FEATURE_PRL_EXACT_NID_MATCH)
#else
 #if defined(FEATURE_WCARD_SID_MGMT) && defined(FEATURE_PRL_EXACT_NID_MATCH)
      unsigned int fill:6;     /* spare                        */
 #elif defined(FEATURE_WCARD_SID_MGMT) || defined(FEATURE_PRL_EXACT_NID_MATCH)
      unsigned int fill:7;     /* spare                        */
 #else
      unsigned int fill:8;     /* spare                        */
 #endif //defined(FEATURE_WCARD_SID_MGMT) && defined(FEATURE_PRL_EXACT_NID_MATCH)
#endif
    } var;
  } stat;
} mcsyspr_pref_stat_type;

/* The desired operating modes */
// Warning: The MCSYSPR_MAX_DESIRED imposes the maximum operating modes
// allowed, if more modes are needed, the size of "mode" in
// mcsyspr_pref_stat_type needed to be changed as well
#define MCSYSPR_MODE_NOT_DESIRED        0x0
#define MCSYSPR_ANALOG_DESIRED          0x1
#define MCSYSPR_DIGITAL_DESIRED         0x2
#define MCSYSPR_MAX_DESIRED             0x3 // see warning in this section

/* Information about the serving system is kept in this structure          */
typedef struct
{
  byte mode;                  /* The mode: CDMA or analog                  */
  byte band_class;            /* The band class: cellular or PCS           */
  nv_cs_union_type cs;        /* CDMA channel or cellular system           */
  mcsyspr_source_type source; /* Where the system was found                */
  int rl_entry;               /* Index into RL if system is from RL        */
  word sid;                   /* The system's SID                          */
  word nid;                   /* The system's NID                          */
  db_roam_type roam_ind;      /* The system's roaming indicator value      */
  boolean more_desirable;     /* Are there more desirable entries?         */
} mcsys_serving_system_type;

/* Redirecting CDMA System information */
typedef struct
{
  byte band_class;      /* The band class: cellular or PCS           */
  nv_cs_union_type cs;  /* CDMA channel or cellular system           */
}mcsyspr_redirng_cdma_sys_type;

/* mode, band_class information */
typedef struct
{  
  byte  mode;           /* The mode: AMPS or CDMA */  
  byte  band_class;     /* The band class: cellular or PCS */
}mcsyspr_mode_bandclass_type;

#ifdef FEATURE_SSPR_800
/* If the phone is currently using a default roaming list, and which one */
typedef enum
{
  PRL_SOURCE_NV,
  PRL_SOURCE_A_ONLY,
  PRL_SOURCE_B_ONLY,
  PRL_SOURCE_STANDARD,
  PRL_SOURCE_OTASP
} mcsyspr_rl_source_enum;
#endif /* FEATURE_SSPR_800 */

/* saves silent redial exit parameters */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
typedef struct
{
  mc_sr_name_type type;     /* silent redial type */
  boolean         home_only;/* silent originate on roaming systems or not */
} mcsyspr_sr_type;
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* The number of entries in the scan list                                  */
#define MCSYSPR_NUM_SCAN_LIST_ENTRIES 100

/* There are 42 preferred PCS channels                                     */
#define MCSYSPR_NUM_PREF_PCS_CHANS    42

/* The number of entries in the alternate scan list                        */
#define MCSYSPR_ALT_SCAN_LIST_SIZE    MCSYSPR_NUM_SCAN_LIST_ENTRIES

#ifdef FEATURE_E911
/* Make sure the ALT_SCAN_LIST is at least as big as SCAN_LIST;            */
/* Otherwise, function "mcsysprd_e911_bld_sl_on_redir(...)" will not work. */
#if (MCSYSPR_ALT_SCAN_LIST_SIZE < MCSYSPR_NUM_SCAN_LIST_ENTRIES)
#error ALT_SCAN_LIST_TOO_SMALL
#endif

/* The number of entries in the e911 available list                        */
#define MCSYSPR_NUM_E911_AL_ENTRIES  MCSYSPR_NUM_SCAN_LIST_ENTRIES
#endif /* FEATURE_E911 */

/* The idle timeout specifies the number of minutes to stay in the idle    */
/* state before returning to system determination so that more desirable   */
/* channels can be scanned. The idle timeout is specified in minutes, with */
/* zero meaning that idle timeout is disabled, 3 meaning either it is a    */
/* CDMA idle time out or GSR idle time out, and 1 meaning AMPS idle time   */
/* out                                                                     */
#define MCSYSPR_IDLE_NONE               0

#if (defined( FEATURE_SSPR_800 ) && defined( FEATURE_CDMA_800 ))
/* On 800MHz dual mode/trimode systems, it is OK to stay on AMPS a little 
 * trimode in order to miss less pages.  Dual band phones are typically roaming
 * on AMPS, so we would rather miss more pages and get back to CDMA sooner.
 */
#define MCSYSPR_AMPS_IDLE_TIMEOUT       2
#else
#define MCSYSPR_AMPS_IDLE_TIMEOUT       1
#endif /* (defined( FEATURE_SSPR_800 ) && !defined( FEATURE_CDMA_1900 )) */

#ifdef FEATURE_JCDMA
#error code not present
#else
#ifdef FEATURE_SKT_CAI
#define MCSYSPR_CDMA_IDLE_TIMEOUT       0
#else
#define MCSYSPR_CDMA_IDLE_TIMEOUT       3
#endif //FEATURE_SKT_CAI
#endif /* FEATURE_JCDMA */

#define MCSYSPR_GSR_IDLE_TIMEOUT        3

/* NO_LAST_SERVICE is used to set the last_index_with_service              */
/* variable to a value that indicates that service was not previously      */
/* available                                                               */
#define MCSYSPR_NO_LAST_SERVICE -1

/* NO_RL_ENTRY is used to indicate a null roaming list entry               */
#define MCSYSPR_NO_RL_ENTRY     -1

/* This is the number of seconds that we will stay in deep sleep           */
#ifdef FEATURE_JCDMA
#error code not present
#else
#define MCSYSPR_DZ_SECONDS  (3*60)
#endif /* FEATURE_JCDMA */

/* This is the minimum number of minutes that we spend doing acquisition   */
/* attempts before deep sleeping                                           */
#ifdef FEATURE_JCDMA
#error code not present
#else
#define MCSYSPR_MIN_INITIAL_ACQ_TIME      15
#endif /* FEATURE_JCDMA */

#ifdef FEATURE_OTASP
#define MCSYSPR_NUM_OTASP_DIALED_DIGITS 5

/* This is the last digit of the OTSAP originate feature code for the      */
/* first PCS block (i.e., the last digit of "*22802")                      */
#define MCSYSPR_OTASP_FC_FIRST_BLOCK '2'
#endif /* FEATURE_OTASP */

#ifdef FEATURE_E911
/* Index of the first CDMA channel in the E911 Scan List.
 * The first CDMA channel follows Analog A & B in the E911 Scan List
 * if the phone is analog capable; otherwise, the first CDMA channel is
 * the first element of the E911 scan list.
 */
#ifdef FEATURE_ACP
#define MCSYSPR_START_CDMA_IN_E911_SL  2
#else
#define MCSYSPR_START_CDMA_IN_E911_SL  0
#endif /* FEATURE_ACP */
#endif /* FEATURE_E911 */

/* These are masks that are used to define what types of records are to be */
/* added to the scan list                                                  */
#define MCSYSPR_ACQ_REC_MODE_ANALOG   0x0001
#define MCSYSPR_ACQ_REC_MODE_CDMA     0x0002
#define MCSYSPR_ACQ_REC_BAND_CELLULAR 0x0004
#define MCSYSPR_ACQ_REC_BAND_PCS      0x0008
#define MCSYSPR_ACQ_REC_ALL_TYPES     0xffff

/* Special SID - all 15 bits are 1's - for identifying PRL version  number
 * stored in the corresponding NID field in a system record
 */
#define MCSYSPR_SPECIAL_SID_FOR_PRL_VER 32767

/* Default PRL Version number for a invalid PRL or a RPL with no special
 * SID in a system record
 */
#define MCSYSPR_DEFAULT_PRL_VER 0

#ifdef FEATURE_SSPR_ENHANCEMENTS
#define MCSYSPR_POWERUP_HOLD_ORIG_TIME 60
#endif


/* Multi-geo-region SID */
#ifdef FEATURE_IS683A_PRL
/* The following defines assume that in average a system table record
 * occupies 4 bytes
 * MCSYSPR_SYS_TABLE_SIZE is used to allocates space required for
 * multi-geo structures
 */
#define MCSYSPR_SYS_TABLE_SIZE  ((NV_ROAMING_LIST_MAX_SIZE)/4)
#else
/* For pre-IS-683A PRL, the max system table size is 255 */
#define MCSYSPR_SYS_TABLE_SIZE 255       /* for pre IS-683A PRL */
#endif

#define MCSYSPR_MG_SID_MAX (MCSYSPR_SYS_TABLE_SIZE/2)
typedef struct {
  word  sid[MCSYSPR_MG_SID_MAX];              /* SID */
  byte  group_size[MCSYSPR_MG_SID_MAX];       /* SID group size */
  int   mem_rl_entry[MCSYSPR_SYS_TABLE_SIZE]; /* index to sys table */
} mcsyspr_multi_geo_sid_type;

#define MCSYSPR_NO_GEO_REGION      0     /* No current geo region */
#define MCSYSPR_NEW_GEO_BIT_MASK  0x7
#define MCSYSPR_SID_IS_NOT_MG     -1

#define MCSYSPR_GEO_BIT_PER_BYTE   8

#define MCSYSPR_NULL_SID 0    /* A SID value 0 is a wildcard SID
                               * it can not be counted as a multi-geo
                               * region SID
                               */

#define MCSYSPR_CURR_GEO_REGION (mcsyspr_curr_geo_region < 0 ?  \
                                 -mcsyspr_curr_geo_region :     \
                                 mcsyspr_curr_geo_region)

/* Macros to use the 16th bit of mcsyspr_multi_geo_sid.mem_rl_entry[]
 * entries (words), which are really 14 bit system table indicies, to flag
 * if the entry is a match.
 */
#define MG_GET_RL_ENTRY(x) ((x) & 0x7FFF)
#define MGSS_RL_ENTRY(x)   ( (((x) & 0x8000) != 0) ? MCSYSPR_NO_RL_ENTRY :(x) )
#define MG_NOMATCH(x)      ((x) |= (int) 0x8000)
#define MG_CLR_NOMATCH(x)  ((x) &= 0x7FFF)

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                          External Data                                  */
/*                                                                         */
/*-------------------------------------------------------------------------*/

extern mcsyspr_scan_list_type mcsyspr_scan_list[];
extern int mcsyspr_scan_list_count;
extern int mcsyspr_scan_list_index;
extern mcsyspr_scan_list_mode_type mcsyspr_scan_list_mode;
extern int mcsyspr_last_scan_list_index;
extern boolean mcsyspr_rebuild_scan_list;
extern mcsyspr_alt_scan_list_type mcsyspr_alt_scan_list[];
extern int mcsyspr_alt_scan_list_count;
extern int mcsyspr_alt_scan_list_index;
extern boolean mcsyspr_mru2_table_changed[];
extern db_items_value_type mcsyspr_db_item;
extern mcsys_entry_type mcsyspr_last_entry_params;
extern mcc_entry_type mcsyspr_cdma_entry;
extern mcc_exit_type mcsyspr_cdma_exit;
extern acp_entry_type mcsyspr_analog_entry;
extern acp_exit_type mcsyspr_analog_exit;
extern boolean mcsyspr_cdma_locked;
extern boolean mcsyspr_redirection_enabled;
extern mcc_redirection_type mcsyspr_redir_rec;
extern byte mcsyspr_curr_sys_selection_num;
extern byte mcsyspr_total_sys_selections;
extern int mcsyspr_last_index_with_service;
extern mcsyspr_scan_mode_type mcsyspr_scan_mode;
extern mcsyspr_scan_mode_type mcsyspr_last_scan_mode;
extern boolean mcsyspr_scan_mru_0;
extern int mcsyspr_first_more_desirable;
extern int mcsyspr_last_more_desirable;
extern int mcsyspr_more_desirable_index;
extern int mcsyspr_more_desirables_scanned;
extern boolean mcsyspr_acq_timer_active;
extern int mcsyspr_full_scan_list_index;
extern int mcsyspr_mini_scan_list_index;
extern int mcsyspr_micro_scan_list_index;
extern dword mcsyspr_acq_timer;
extern nv_ram_roaming_list_type mcsyspr_roaming_list;
#if defined(FEATURE_OTASP) && !defined(FEATURE_NSOTASP)
extern nv_roaming_list_type mcc_roaming_list;
#endif
extern int mcsyspr_num_acq_records;
extern int mcsyspr_num_sys_records;
extern boolean mcsyspr_use_mini_scan;
extern boolean mcsyspr_system_selected;
extern mcsys_serving_system_type mcsyspr_serving_system;
extern mcsys_serving_system_type mcsyspr_alt_system;
extern nv_item_type mcsyspr_nv_data_buf;
extern byte mcsyspr_current_nam;
extern mcsyspr_redirng_cdma_sys_type mcsyspr_redirng_cdma_sys;

/* This is used to determine to determine whether MCSYSPR_NO_RL_ENTRY      */
/* returned by mcsyspr_match_rl_entry() is a a real "no entry" or a "entry */
/* with less preference" case. Note that this is a temporary fix and please*/
/* do not use this for any other purpose. For more details, refer to       */
/* reference of this variable in mcsyspr_mg_match_rl_entry                 */
extern boolean mcsyspr_mg_no_rl_entry;

extern nv_mru2_table_type mcsyspr_mru2_table[NV_MAX_NAMS];
#ifdef FEATURE_SSPR_ENHANCEMENTS
extern nv_sid_nid_pair_type mcsyspr_home_sid_nid_list
         [NV_MAX_NAMS][NV_MAX_HOME_SID_NID];
#else
extern nv_sid_nid_pair_type mcsyspr_home_sid_nid_list
         [NV_MAX_NAMS][NV_MAX_SID_NID];
#endif

#ifdef FEATURE_IS95B_NDSS
extern boolean mcsyspr_ndss_on;
extern boolean mcsyspr_ndss_pref_orig; 
extern byte mcsyspr_return_cause;
#endif /* FEATURE_IS95B_NDSS */

extern word mcsyspr_nv_prl_size;

#ifdef FEATURE_IS683A_PRL
extern word mcsyspr_nv_num_acq_records;
extern word mcsyspr_nv_num_sys_records;
#else
extern byte mcsyspr_nv_num_acq_records;
extern byte mcsyspr_nv_num_sys_records;
#endif /* FEATURE_IS683A_PRL */

extern int mcsyspr_curr_geo_region;
extern mcsyspr_block_to_cdma_chan_type  const
  mcsyspr_block_to_cdma_channel[MCSYSPR_NUM_PREF_PCS_CHANS];

#ifdef FEATURE_OTASP
extern char mcsyspr_otasp_dialed_digits[];
extern word mcsyspr_otasp_service_option;
extern mc_activate_code_type mcsyspr_otasp_activate_code;
#ifndef FEATURE_NSOTASP
extern byte mcsyspr_otasp_dialed_digit_cnt;
#endif
#endif /* FEATURE_OTASP */

#ifdef FEATURE_ACP
extern word mcsyspr_analog_home_sid[];
extern boolean mcsyspr_handoff_from_cdma;
#endif /* FEATURE ACP */

#if defined(FEATURE_PRL_FORCE_MODE) || defined(FEATURE_HOME_ONLY) || \
    defined(FEATURE_E911)
extern nv_mode_enum_type mcsyspr_pref_mode;
extern nv_mode_enum_type mcsyspr_last_pref_mode_otasp;
#endif /* FEATURE_PRL_FORCE_MODE || FEATURE_HOME_ONLY || FEATURE_E911 */

extern nv_sys_pref_enum_type mcsyspr_system_pref;

#ifdef FEATURE_SSPR_800
extern word mcsyspr_nv_pch_a;             /* RO: Primary channel A          */
extern word mcsyspr_nv_pch_b;             /* RO: Primary channel B          */
extern word mcsyspr_nv_sch_a;             /* RO: Secondary channel A        */
extern word mcsyspr_nv_sch_b;             /* RO: Secondary channel B        */

extern nv_system_type mcsyspr_cdma_home_side[NV_MAX_NAMS]; /* System A or B */

#ifdef FEATURE_ACP
extern nv_system_type mcsyspr_analog_home_side[NV_MAX_NAMS];/*System A or B */
#endif /* FEATURE_ACP */

extern prli_acq_rec_type mcsyspr_cell_cdma_acq;
extern mcsyspr_rl_source_enum mcsyspr_rl_source;
#endif /* FEATURE_SSPR_800 */

#ifdef FEATURE_SSPR_ENHANCEMENTS
#ifdef FEATURE_ACP
extern word mcsyspr_analog_nonroam_sys_list[NV_MAX_NAMS][NV_MAX_HOME_SID_NID];
#if (!(defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900)))
extern word mcsyspr_analog_sid_lock_list[NV_MAX_NAMS][NV_MAX_SID_NID_LOCK];
#endif /* (!(defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900))) */
#endif /* FEATURE_ACP */
#if (!(defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900)))
extern nv_sid_nid_pair_type
   mcsyspr_cdma_sid_lock_list[NV_MAX_NAMS][NV_MAX_SID_NID_LOCK];
#endif /* (!(defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900))) */

#ifdef FEATURE_SSPR_800
/* mcsyspr_powerup_time is the secs. remaining for Powerup Hold Orig */
extern long mcsyspr_powerup_time;

/* Variable to keep track of time spent after powerup/NAM Change etc */
extern dword mcsyspr_last_powerup_check;
#endif /* FEATURE_SSPR_800 */

#endif /* FEATURE_SSPR_ENHANCEMENTS */

#ifdef FEATURE_SSPR_ENHANCEMENTS /* Powerup Hold Orig */
extern boolean mcsyspr_powerup_scanning;
#endif /* FEATURE_SSPR_ENHANCEMENTS */

#ifdef FEATURE_E911
extern mcsyspr_e911_avail_list_type
               mcsyspr_e911_avail_list[MCSYSPR_NUM_E911_AL_ENTRIES];
extern int2 mcsyspr_e911_avail_list_count;
extern mcsyspr_e911_redir_chan_type mcsyspr_redirecting_channel;
extern mcsys_svc_acq_mode_type mcsyspr_svc_acq_mode;
extern boolean mcsys_connect_rxed;  
#ifdef FEATURE_ACP
extern int2 mcsyspr_e911_analog_scan_index;
#endif /* FEATURE_ACP */
extern int2 mcsyspr_e911_cdma_scan_index;
extern int2 mcsyspr_e911_lost_sys_index;
#endif /* FEATURE_E911 */

#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
extern boolean mcsyspr_sr_home_only;
/* Function prototype */
mcsyspr_state_enum_type mcsyspr_proc_silent_redial
(
  mcsyspr_sr_type
);
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */

/* PRL_REFORMATTED flag */
extern boolean prl_reformatted;

#ifdef FEATURE_FASTPRL
extern boolean prl_reformat(void);
#endif
extern void mcsyspr_init_mg_sid(void);

#ifdef FEATURE_SSPR_800
/* Flag indicating if PRL in NV is enabled */
extern boolean mcsyspr_rl_enabled;
#endif /* FEATURE_SSPR_800 */

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                             Macros                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Macro to set the NO_SVC indicator in the database                       */
#define SET_DB_NO_SVC(svc) \
  mcsyspr_db_item.no_svc = svc; \
  db_put(DB_NO_SVC, &mcsyspr_db_item)

/* Macro to set the acq mode                                               */
#define SET_DB_ACQ_MODE(mode) \
  mcsyspr_db_item.acq_mode = mode; \
  db_put(DB_ACQ_MODE, &mcsyspr_db_item)

/* Macro to set the roaming indicator in the database                      */
#define SET_DB_ROAM(flag) \
  mcsyspr_db_item.roam = flag; \
  db_put(DB_ROAM, &mcsyspr_db_item)

/* Macro to set the digital indicator in the database                      */
#define SET_DB_DIGITAL(mode) \
  mcsyspr_db_item.digital = (mode == NV_MODE_CDMA) ? TRUE : FALSE; \
  db_put(DB_DIGITAL, &mcsyspr_db_item)
/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                        Function Prototypes                              */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* From mcsyspr.c */
nv_stat_enum_type mcsyspr_get_nv_item(nv_items_enum_type item_code,
                                      nv_item_type *item_ptr);
void mcsyspr_read_home_sid_nid(int nam);
void mcsyspr_init_rl_params(void);
word mcsyspr_get_prl_pcs_block(word cdma_channel);
nv_block_or_sys_enum_type mcsyspr_get_block(word cdma_channel,
  word band_class);
#ifdef FEATURE_AVAIL_HOME_SIDE_PREF
nv_system_type mcsyspr_get_cellular_side(word cdma_channel);
#endif /* FEATURE_AVAIL_HOME_SIDE_PREF */
void mcsyspr_build_scan_list(mcsyspr_scan_list_mode_type mode);
void mcsyspr_clear_available_list(void);
boolean mcsyspr_get_new_mode_or_sys(void);
void mcsyspr_set_service_indicator(void);
void mcsyspr_set_db_hold_orig(void);
db_roam_type mcsyspr_get_roam_ind(byte mode, word band_class, word sid,
                                  word nid);
mcsyspr_state_enum_type mcsyspr_use_redir_criteria(void);
mcsyspr_state_enum_type mcsyspr_nam_changed(void);
mcsyspr_state_enum_type mcsyspr_auto_nam_change(byte new_nam);
dword mcsyspr_calculate_acq_time(void);
#ifdef FEATURE_JCDMA
#error code not present
#else
boolean mcsyspr_deep_sleep(void);
#endif /* FEATURE_JCDMA */
boolean mcsyspr_valid_cellular_cdmach(word chan);
boolean mcsyspr_valid_pcs_cdma_chan(word chan);
void mcsyspr_add_channel_to_asl(word band_class, word cdma_channel);

#ifdef FEATURE_IS95B
void mcsys_set_analog_regran_parameter(acp_redirection_type*, 
                                       mcc_redirection_type*); 
#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS95B_NDSS
void mcsysprd_set_org_online_digital(word idle);
boolean mcsyspr_ndss_on_progress ( void);
#endif /* FEATURE_IS95B_NDSS */

#ifdef FEATURE_OTASP
mcsyspr_state_enum_type mcsyspr_otasp_provisioning(
                            mc_activate_code_type activate_code,
                            word                  service_option,
                            byte                  dialed_digit_count,
                            byte                  *dialed_digits);

mcsyspr_state_enum_type mcsysprd_otasp_failure(void);

mcsyspr_state_enum_type mcsysprd_otasp_end_activation(void);

/*===========================================================================

FUNCTION MCSYSPR_OTASP_PROVISIONING_ORIG

DESCRIPTION
  Returns to the Serving System with the entry reason set as ACTIVATION.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
mcsyspr_state_enum_type mcsyspr_otasp_provisioning_orig(
  mc_activate_code_type   activate_code,       /* The activation code */
  word                    service_option,      /* The service option */
  byte                    dialed_digit_count,  /* The dialed digit count */
  byte                    *dialed_digits       /* Pointer to dialed digits */
  );

#endif /* FEATURE_OTASP */

#if defined(FEATURE_PRL_FORCE_MODE) || defined(FEATURE_HOME_ONLY) || \
    defined(FEATURE_E911)
mcsyspr_state_enum_type mcsyspr_pref_sys_changed(void);
#endif /* FEATURE_PRL_FORCE_MODE || FEATURE_HOME_ONLY || FEATURE_E911 */

#ifdef FEATURE_E911
mcsyspr_state_enum_type mcsyspr_e911_set_next_sys_chan(void);
mcsyspr_state_enum_type mcsyspr_e911_orig_acq_failed(void);
mcsyspr_state_enum_type mcsyspr_e911_callback_next_channel(void);
mcsyspr_state_enum_type mcsyspr_e911_orig_sys_lost(boolean max_access_probe);
#endif /* FEATURE_E911 */

/*===========================================================================

FUNCTION MCSYSPR_BUILD_AVAIL_ASL

DESCRIPTION
  This function builds the alternate scan list when the serving system is
  an available system (not in the PRL).  This consists of all channels in
  the acquisition table, in the order they are listed.

DEPENDENCIES

RETURN VALUE
  TRUE indicates that there are roaming list entries with more desirable
  service, and their channels have been placed into the alternate scan list.
  FALSE indicates that there are no roaming list entries with more desirable
  service than the acquired system.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcsyspr_build_avail_asl (void);

/* From mcsysprd.c */
void mcsysprd_set_online_digital(word idle);
mcsyspr_state_enum_type mcsysprd_enter_online_digital(void);
void mcsysprd_set_offline_digital(void);
mcsyspr_state_enum_type mcsysprd_enter_offline_digital(void);

#ifdef FEATURE_E911
void mcsysprd_e911_bld_sl_on_redir(boolean init_enter_redir);
#endif /* FEATURE_E911 */

/* From mcsyspra.c */
void mcsyspra_set_online_analog(word idle);
mcsyspr_state_enum_type mcsyspra_enter_online_analog(void);
void mcsyspra_set_offline_analog(void);
mcsyspr_state_enum_type mcsyspra_enter_offline_analog(void);

#ifdef FEATURE_ACP
/*===========================================================================

FUNCTION MCSYSPR_GET_AMPS_SYSTEM

DESCRIPTION
  Based on the channel number, this function determines which
  AMPS system the passed channel number is associated with.
  Returns TRUE, if the given channel is a valid one. Else, it returns false.

  Note that, in case of a invalid AMPS channel, this function does not the set
  the cell_system.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if the cell_system is succesfully set
  FALSE, if unable to set the cell_system

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcsyspr_get_amps_system
(
  nv_system_type *cell_system,
  word chan
);

/*===========================================================================

FUNCTION MCSYSPR_ADD_ANALOG_SYS_TO_SL

DESCRIPTION
  This function adds an analog cellular system to the scan list. If the
  system is already in the scan list, then it is not added again.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcsyspr_add_analog_sys_to_sl
(
  nv_system_type system     /* The system to add to the scan list */
);
#endif /* FEATURE_ACP */

/* From mcsysci.c */
extern boolean mcsyspr_continue_in_idle(byte mode, word band_class,
                                        word sid, word nid);
extern boolean mcsyspr_e911_continue_in_idle(byte mode, byte band_class,
                                             word sid, word nid);

/* For mcsyssup.c */
extern mcsyspr_wakeup_type mcsyspr_wakeup_mode;

/* From mcsyssup.c */
extern byte mcsyspr_move_to_next_channel(void);
extern void mcsyspr_e911_inc_cdma_scan_ind(void);
extern byte mcsyspr_e911_next_channel(void);

/* From prl.c */
extern mc_name_type mcsyspr_read_roaming_list(byte nam);
extern boolean mcsyspr_extract_acq_record(
  prli_acq_rec_type *acq_rec, /* Pointer to where to build the acq record */
  int index                   /* Index of the acquisition entry to extract */
);
extern boolean mcsyspr_extract_sys_record
(
  prli_sys_rec_type *sys_rec,     /* Pointer to where to build sys record */
  int index                       /* Index of the system record to extract */
);
extern int mcsyspr_find_first_in_geo(int sys_index);
extern int mcsyspr_find_last_in_geo(int sys_index);
extern int mcsyspr_find_first_in_tier(int sys_index);
extern int mcsyspr_find_last_in_tier(int sys_index);
extern boolean mcsyspr_same_geo_region(int first_sys_index,int second_sys);
extern int mcsyspr_match_rl_entry(
  byte mode,              /* The mode of the acquired system */
  word band_class,        /* The band class of the acquired system */
  nv_cs_union_type cs,    /* The acquired CDMA channel or analog system */
  word sid,               /* The acquired SID */
  word nid,               /* The acquired NID */
  boolean check_acceptable /* Check the acceptability of the system only */
);
extern boolean mcsyspr_more_desirable_service
(
  byte mode,          /* The mode of the acquired system */
  word band_class,    /* The band class of the acquired system */
  int rl_entry,      /* The acquired system's index into the system table */
  word  sid,        /* System ID */
  word  nid         /* Network ID */
);
extern boolean mcsyspr_get_preferred_only(void);
extern db_roam_type mcsyspr_get_default_roam_ind(void);
extern boolean mcsyspr_build_alt_scan_list(boolean init,
                                           int  geo_region,
                                           int  rl_entry
                                           );
extern db_roam_type mcsyspr_determine_roam_ind
(
  db_roam_type proposed_roam_ind, /* The proposed roaming indicator value */
  word sid,                       /* The acquired SID */
  word nid,                       /* The acquired NID (CDMA Mode Only) */
  byte mode,          /* The mode of the acquired system */
  word band_class     /* The band class of the acquired system */
);
extern int mcsyspr_find_sys_in_scan_list
(
  byte mode,            /* The mode of the system */
  nv_cs_union_type cs,  /* The CDMA channel or cellular system */
  word band_class       /* The band class of the system */
);
extern int mcsyspr_find_last_sys_in_scan_list
(
  byte mode,            /* The mode of the system */
  nv_cs_union_type cs,  /* The CDMA channel or cellular system */
  word band_class       /* The band class of the system */
);
extern int mcsyspr_find_last_sys_in_sl
(
  byte mode,            /* The mode of the system */
  nv_cs_union_type cs,  /* The CDMA channel or cellular system */
  word band_class       /* The band class of the system */
);
extern boolean mcsyspr_map_more_desirable_svc
(
  int rl_entry      /* The system table index of the first system within
                       the geographical region where we received the max
                       access probes exit */
);

extern void mcsyspr_add_acq_rec_to_asl
(
  prli_acq_rec_type *acq_rec    /* The acq_rec to add to the alt scan list */
);

boolean mcsyspr_more_preferred_system
(
  int  rl_entry,        /* index into the system table */
  int  *prefer_beg,     /* the beginning entry of more preferred sys */
  int  *prefer_end      /* the ending entry of more preferred sys */
);
int mcsyspr_locate_geo_region
(
  int   rl_entry,               /* system table index */
  int   *rle_begp               /* the first entry in the geo region */
);


/*=========================================================================
  FUNCTION MCSYSPR_RESELECTION_WRAPPER

  DESCRIPTION
    The interface between SSPR rescan related code and the command
    interpreter is provided through this function.

    The command interpreter will be called from this function. Also, once
    returned from the command interpreter, this is the place where SSPR
    related initialization is done.

  DEPENDENCIES
    None.

  RETURN VALUE
    Next state to be entered, either MCSYSPR_ONLINE_DIGITAL_STATE or
    MCSYSPR_ONLINE_ANALOG_STATE.

  SIDE EFFECTS
    None.

===========================================================================*/
extern mcsyspr_state_enum_type mcsyspr_reselection_wrapper
(
  void
);


/*===========================================================================

FUNCTION MCSYSPR_SORT_ASL

DESCRIPTION
  Once the ASL is built, entries are sorted according to their occurrence and
  position in the MRU table. In other words, all the ASL entries with a
  matching MRU entry are bubbled up to the top and arranged according to their
  position in the MRU table.

  This function is called anytime service is provided on a CDMA channel or
  an AMPS system

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
extern void mcsyspr_sort_asl(void);


#if defined (FEATURE_SSPR_800) && defined(FEATURE_SSPR_ENHANCEMENTS)
/*===========================================================================

FUNCTION MCSYSPR_SET_CDMA_AVAIL_GAM

DESCRIPTION
  This function based on the preferred mode will determine whether the CDMA
  available GAM needs to be set or not

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if the CDMA available gam is to be ignored
  FALSE, if the CDMA available gam is not to be ignored

SIDE EFFECTS
  None.
===========================================================================*/

boolean mcsyspr_set_cdma_avail_gam(void);

#endif /* defined (FEATURE_SSPR_800) && defined(FEATURE_SSPR_ENHANCEMENTS) */

/*===========================================================================

FUNCTION MCSYSPR_ADD_CHANNEL_TO_SL

DESCRIPTION
  This function adds a CDMA channel to the scan list. If the channel is
  already in the scan list, then it is not added again.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcsyspr_add_channel_to_sl
(
  word band_class,    /* The band class of the channel to add */
  word cdma_channel   /* The CDMA channel to add to the scan list */
);


/*===========================================================================

FUNCTION MATCH_CHANS

DESCRIPTION
  This function determines if a given channel is in the given acquisition
  record.  The acquisition record is PCS using channels or cellular CDMA
  using channels.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the given channel is represented by the given acquisition
  record, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean match_chans
(
  nv_cs_union_type given_cs, /* The PCS channel */
  prli_acq_rec_type *acq_rec /* The acq record to match the channel to */
);

#ifdef FEATURE_SSPR_800

/*===========================================================================

FUNCTION MATCH_CELL_CHAN_TO_AB

DESCRIPTION
  This function determines if a given channel is in the given acquisition
  record.  The acquisition record is cellular CDMA using channels.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the given channel is represented by the given acquisition
  record, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean match_cell_chan_to_ab
(
  nv_cs_union_type given_cs, /* The PCS channel */
  prli_acq_rec_type *acq_rec /* The acq record to match the channel to */
);
#endif /* FEATURE_SSPR_800 */

#ifdef FEATURE_CDMA_1900

/*===========================================================================

FUNCTION MATCH_PCS_SYS_TO_BLOCK

DESCRIPTION
  This function determines if a given PCS system is part of an acquisition
  record.  The acquisition record is PCS using blocks.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the given PCS system is represented by the given acquisition
  record, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean match_pcs_sys_to_block
(
  nv_cs_union_type given_cs, /* The PCS channel */
  prli_acq_rec_type *acq_rec /* The acq record to match the channel to */
);
#endif /* FEATURE_CDMA_1900 */


#ifdef FEATURE_ACP
/*===========================================================================

FUNCTION MATCH_AMPS_SYS_TO_AB

DESCRIPTION
  This function determines if a given analog system is part of an acquisition
  record.  The acquisition record is either cellular analog or cellular
  CDMA preferred (cellular generic).

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the given analog system is represented by the given acquisition
  record, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean match_amps_sys_to_ab
(
  nv_cs_union_type given_cs, /* The AMPS system (A or B) */
  prli_acq_rec_type *acq_rec /* The acq record to match the system to*/
);
#endif /* FEATURE_ACP */


/*===========================================================================


FUNCTION MCSYSPR_SERVICE_ICON_OFF

DESCRIPTION
  This function turns off the service icon.

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
void mcsyspr_service_icon_off(void);


/*===========================================================================


FUNCTION MCSYSPR_SAME_PRL_SYS_AS_ACQUIRED

DESCRIPTION
  This function checks if the sid/nid/mode/band_class of the system
  has changed since acquisition.
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - sid/nid/mode/band_class are the same as acquired
  FALSE - No.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcsyspr_same_prl_sys_as_acquired
(
  word sid, word nid, byte mode, byte band_class
);


#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */ 
#endif /* MCSYSPR_H */

