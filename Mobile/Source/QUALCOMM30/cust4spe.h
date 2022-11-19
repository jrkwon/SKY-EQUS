#ifndef CUST4SPE_H
#define CUST4SPE_H
/*===========================================================================

            " M S M 5 0 0 0 -  S U R F   P C S"   H E A D E R   F I L E

DESCRIPTION
  Configuration for SURF Target using MSM5000 ARM Processor.
  This is a PCS build with IS95B, NV Item Manager and 
  Embedded File System.

  Copyright (c) 1999, 2000 by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/cust4spe.h_v   1.18   11 Apr 2000 21:54:10   knordin  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/05/00   kjn	   Undefined FEATURE_DETECT_SUP_MUTING.
04/05/00   kjn	   Added & defined FEATURE_OTASP_HOLD_CMD_IN_COMMIT,
				   FEATURE_UI_8K_SUPPORT, FEATURE_UI_8K_VOICE_SUPPORT.
04/03/00   va      Defined FEATURE_SRCH_DYNAMIC_BAND_CLASS feature
03/27/00   va      defined the following:
                   FEATURE_DETECT_SUP_MUTING
                   FEATURE_UI_DEBUG_SLOTTED_MODE
                   FEATURE_SEARCH2 (16x search)
                   FEATURE_SEARCH2_ACQ (16x search)
                   FEATURE_SEARCH2_IDLE (16x search)
                   FEATURE_UART_TCXO_CLK_FREQ 
03/07/00   kjn     Added FEATURE_DIAG_DEFAULT_BITRATE_38400. 
02/29/00   kjn     Added FEATURE_DIAG_V7_LOG_IFACE, FEATURE_DIAG_NON_STREAMING, 
					FEATURE_DIAG_NO_EVENTS. 
02/23/00    eh     Added T_MSM5000A_SUPPORT
02/12/00   hrk     Added FEATURE_RLGC
02/07/00   va      Added and undefed FEATURE_VOC_SAT_CONTROL. Added RXC_ACTION_TIME_PROCESSING.
                   Removed FEATURE_INT_25_FIQ
02/07/00   va      Merged from Branch for TR0302
                   Removed FEATURE_SPECIAL_MDR which is not supported in this
                   archive.  Removed TEMP_HACKS_FOR_FOR_TIGER.
                   Also undefined features not for this release.
11/03/99   lcc     Deleted FEATURE_VIRTUAL_COMBINER which is mainlined.
                   Added and defined FEATURE_SERIAL_DEVICE_MAPPER.
11/02/99   lcc     Added and defined FEATURE_IS95B_ACC_PRB_BKOFF_DLY.
10/20/99   lcc     Added block for IS2000 features.  Enabled MDR.
10/07/99   lcc     Initial release to support MSM5000 release 0.3.  Based on
                   cust4spe.h from Panther archive.

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
#define  CUST_MOB_MODEL 49

/*---------------------------------------------------------------------------
                            Call Processing
---------------------------------------------------------------------------*/


/* Supports CDMA at 1900MHz
*/
#define FEATURE_CDMA_1900

/* Support for Temporary Mobile Station Identifier
*/
#define FEATURE_TMSI

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
                             User Interface
---------------------------------------------------------------------------*/


/* OTASP activation for blocks A through F will be made from a menu item *
*/
#define FEATURE_UI_OTASP_MENU

/*---------------------------------------------------------------------------
                            MSM5000 Features
---------------------------------------------------------------------------*/
#ifdef T_MSM5000
  
  /*---------------------------------------------------------------------------
   Features to use the improved TX AGC for MSM5000
   This feature is disabled by default and should be enabled by customers who
   has this feature supported in hardware.
  ---------------------------------------------------------------------------*/
  #define FEATURE_PA_STATE_SYNC
  #undef FEATURE_PA_RANGE_DIG_COMP
  #undef FEATURE_4PAGE_TX_LINEARIZER
  /* this means the H/W for PDM compensation is still there but not used */
  #undef FEATURE_PDM1_HW_DISABLE
  #undef FEATURE_PDM2_HW_DISABLE
  
#endif


/*---------------------------------------------------------------------------
                            P_REV definitions
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
  ** It is defined as 1 to assist the P_REV1+ solution for MDR
  */
#define MOB_P_REV 1
#endif 

#endif /* CUST4SPE_H */
