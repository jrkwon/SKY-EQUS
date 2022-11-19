#ifndef CUST4SCE_H
#define CUST4SCE_H
/*===========================================================================

            " M S M 5 0 0 0 -  S U R F "   H E A D E R   F I L E

DESCRIPTION
  Configuration for SURF Target using MSM5000 ARM Processor.
  This is a Cellular build with IS95B, NV Item Manager and 
  Embedded File System.

  Copyright (c) 1999, 2000 by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/cust4sce.h_v   1.18   11 Apr 2000 21:57:48   knordin  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/11/00   kjn	   Moved all the FEATURE defines which are common to all builds to
				   custsurf.h.
04/05/00   kjn	   Undefined FEATURE_DETECT_SUP_MUTING.
04/05/00   kjn	   Added & defined FEATURE_OTASP_HOLD_CMD_IN_COMMIT,
				   FEATURE_UI_8K_SUPPORT, FEATURE_UI_8K_VOICE_SUPPORT.
04/03/00   va      Defined FEATURE_SRCH_DYNAMIC_BAND_CLASS feature
03/27/00   va      defined the following:
                   FEATURE_DETECT_SUP_NUTING
                   FEATURE_UI_DEBUG_SLOTTED_MODE
                   FEATURE_SEARCH2 (16x search)
                   FEATURE_SEARCH2_ACQ (16x search)
                   FEATURE_SEARCH2_IDLE (16x search)
                   FEATURE_UART_TCXO_CLK_FREQ 
03/07/00   kjn     Added FEATURE_DIAG_DEFAULT_BITRATE_38400. 
02/29/00   kjn     Added FEATURE_DIAG_V7_LOG_IFACE, FEATURE_DIAG_NON_STREAMING, 
					FEATURE_DIAG_NO_EVENTS. 
02/23/00    eh     Added T_MSM5000A_SUPPORT
02/07/00   va      Added FEATURE_VOC_SAT_CONTROL, RXC_ACTION_TIME_PROCESSING.
                   Removed FEATURE_INT_25_FIQ
01/19/00   lcc     Removed FEATURE_SPECIAL_MDR which is not supported in this
                   archive.  Removed TEMP_HACKS_FOR_FOR_TIGER.
                   Also undefined features not for this release.
11/03/99   lcc     Deleted FEATURE_VIRTUAL_COMBINER which is mainlined.
                   Added and defined FEATURE_SERIAL_DEVICE_MAPPER.
11/02/99   lcc     Added and defined FEATURE_IS95B_ACC_PRB_BKOFF_DLY.
10/20/99   lcc     Added block for IS2000 features.  Enabled MDR.
10/07/99   lcc     Initial release based on cust4sc.h from Panther archive.

===========================================================================*/

#include "custsurf.h"

/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
                            General Interest
---------------------------------------------------------------------------*/

/* Customer's Over the Air Model Number.  Note that these must be carefully
** assigned and that QualComm only has a few assigned to it.
*/
#define  CUST_MOB_MODEL 48

/*---------------------------------------------------------------------------
                            Call Processing
---------------------------------------------------------------------------*/

/* Analog Call Processing
*/
#define FEATURE_ACP

/*  FM sleep improvements
*/
#define FEATURE_FM_SLOTTED

/* FM slotted debug information
*/
#undef FM_SLOTTED_DEBUG

/* Supports CDMA at 800MHz
*/
#define FEATURE_CDMA_800

/* This feature allows the use of a roaming list as defined in IS-683A.
** SSPR_800 can operate without a roaming list, in which case it behaves similar
** to the old algorithm, but not exactly the same.
*/
#define FEATURE_SSPR_800

/* Feature to prevent roam side available systems from being acquired
*/
#define FEATURE_AVAIL_HOME_SIDE_PREF


/* needs to be turned on to enable all the functionality needed for FEATURE_SSPR_800.
*/
#define  FEATURE_SSPR_ENHANCEMENTS

/* These features need to be turned on to enable all the functionality needed for
** FEATURE_OTASP.
*/
#define  FEATURE_PRL_FORCE_MODE

/*---------------------------------------------------------------------------
                             User Interface
---------------------------------------------------------------------------*/

/* UI To Support "Force Call" in its.
*/
#define  FEATURE_UI_FORCE_CALL
#define  FEATURE_UI_FORCE_MODE_SEL


/*---------------------------------------------------------------------------
                          Non-Volatile Memory
---------------------------------------------------------------------------*/

/* Use NV Item Manager with Embedded file system.
*/
#define FEATURE_NV_ITEM_MGR

/* Embedded file system.
*/
#define FEATURE_EFS

/* Use flash on SURF for the embedded file system.
*/
#define FEATURE_SURF_FLASH

/*---------------------------------------------------------------------------
                            MSM5000 Features
---------------------------------------------------------------------------*/

/* Enable all SAT writes under vocoder control */
#define FEATURE_VOC_SAT_CONTROL

/*---------------------------------------------------------------------------
                            P_REV definition
---------------------------------------------------------------------------*/

#ifdef FEATURE_IS2000
  /* MOB_P_REV definition needed for run-time P_REV checking
  ** It is defined as 6 for IS-95B compliant mobiles
  */
#define MOB_P_REV 6
#elif defined( FEATURE_IS95B)
  /* MOB_P_REV definition needed for run-time P_REV checking
  ** It is defined as 4 for IS-95B compliant mobiles
  */
#define MOB_P_REV 4
#else
  /* MOB_P_REV definition needed for run-time P_REV checking
  ** It is defined as 3 to assist the P_REV1+ solution for MDR
  */
#define MOB_P_REV 3
#endif

/*---------------------------------------------------------------------------
                            IS-95B definition
---------------------------------------------------------------------------*/
#ifdef FEATURE_IS95B

  /* TMSI support is required for IS-95B Band Class 0
  ** This is included only if FEATURE_IS95B is included
  */
  #define FEATURE_TMSI

  #else

  /* Disable TMSI since it is not valid for Band Class 0 P_REV = 3 mobiles.
  */
  #undef FEATURE_TMSI

#endif /* FEATURE_IS95B */


#endif /* CUST4SCE_H */
