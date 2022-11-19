#ifndef RFICAP_H
#define RFICAP_H

/*===========================================================================

             R F   C a p a b i l i t i e s   H e a d e r   F i l e

DESCRIPTION
  This header file that defines the band/mode capabilities and macros which
  are used by the RF unit.  This header file is internal to the RF unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/rficap.h_v   1.3   26 May 2000 10:16:06   fstearns  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.10.12   kmh     RF 추가 definition들을 Cust(Project명).h로 옮겨 정리한다.
                   이 때 D1은 제외한다.
00.03.03   chos    PDM1, PDM2 사용을 D1 방식으로 하기 위해
                   PDM1, PDM2 관련 routine disable
00.03.03   chos    Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
05/26/00   fas     Changed FEATURE_PA_RANGE_DIG_COMP to 
                   FEATURE_PA_RANGE_TEMP_FREQ_COMP.
05/27/99   kmp     Changed FEATURE_JSTD008 to FEATURE_CDMA_1900
04/18/99   snn     Undefined RF_HAS_PA_RANGE_CAL for PCS version.
                   Defined RF_CAP_PDM1_FOR_TX , RF_CAP_PDM2_FOR_TX for PCS version. 
                   This was done because the PCS targets use PDM1 and PDM2.
01/06/99   ychan   Modified for targets NGP, SURF, Q-1900.
01/06/99   ychan   Updated Copyright date.
06/21/98   jjn     Updated from code review.
04/09/98   thh     Added retune at RxTx transition.  Added a table
                   describing all current RF capability.
03/17/98   thh     Rx AGC limits and PDM1 and PDM2 in Tx will be used;
                   so the following the defined:
                     RF_LIMIT_RX_AGC_OUTPUT
                     RF_CAP_PDM1_FOR_TX
                     RF_CAP_PDM2_FOR_TX
02/10/98   thh     Added Q-target support.
                   Added new capability definition RF_CAP_RETUNE_RXTX.  When
                   RF_CAP_RXTX_RETUNE is defined RF card will get a request to
                   re-tune at RX/RXTX transition.
02/08/98   jjn     For targets that do not use RF_CONFIG NV item, defined
                   RF_TARGET_LACKS_RF_CONFIG_NV
02/03/98   kss     Moved Module RF to new RF architecture
11/17/97   jjn     Partitioned ISS2 RF to conform to the new RF architecture
08/14/97   thh     Initial version.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"     /* Target definitions   */
#include "customer.h"   /* Customer definitions */


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=

                                COMMON MACROS

=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/* EJECT  */
/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                            RF UNIT CAPABILITY

Definition                    Description
----------                    -------------------------------------------------
RF_TARGET_HAS_CHARGER         When enabled, the RF driver will notify the
                              Charger when it transitions to and out of RxTx.

RF_LIMIT_RX_AGC_OUTPUT        When enabled, the RF driver will attempt to read
                              the Rx AGC output limits from NV and set them
                              if it determines that the limits are valid.

RF_CAP_PDM1_FOR_RX            When enabled, the RF driver will use PDM1 for
                              RX_GAIN_COMP.  This is the default unless
                              specifically turned off!

RF_CAP_PDM1_FOR_TX            When enabled, the RF driver will enabled PDM1
                              when it enters RxTx.  PDM1 is disabled, its
                              control register will be set to a power-save
                              value when the driver exits RxTx.

RF_CAP_PDM2_FOR_TX            Same as PDM1 for PDM2.

RF_CAP_RXTX_RETUNE            When enabled, the MSM RF driver will request a
                              retune from the Card Driver.  This interface
                              allows the Card driver to re-program its
                              synthesizer when transitioning in and out of
                              RxTx.

RF_TARGET_LACKS_RF_CONFIG_NV  Currently, this feature is only enabled for ISS2
                              targets.  This features should be defined for all
                              targets which do not use the NV item RF_CONFIG_I.
                              When enabled, RF_TX_LIM_VS_TEMP is taken as
                              evidence that RF CAL was performed rather than
                              the writing of RF_CONFIG_I, since this value is
                              never read.
                              
RF_HAS_PA_RANGE_CAL           Target has PA Range Calibration.

===========================================================================*/

/* Establish defaults */
// by chos 00.03.03
//	PDM1, PDM2 사용을 D1 방식으로 하기 위해 PDM1, PDM2 관련 routine disable
//	D1에서는 PDM1,2를 아래와 같이 사용
//		PDM1 -> VCACNT
//		PDM2 -> VGGDA
//#define RF_CAP_PDM1_FOR_RX
#undef RF_CAP_PDM1_FOR_RX

/* -----------------------------------------------------------------------
**                           Odie & Mr. Peabody
** ----------------------------------------------------------------------- */
#if (TG==T_O)
#error code not present
#elif (TG==T_Q)
#error code not present
#elif (TG==T_T)
#error code not present
#elif (TG==T_G)
#ifdef FEATURE_CDMA_1900
#undef RF_HAS_PA_RANGE_CAL
#else
  #ifndef FEATURE_PA_RANGE_TEMP_FREQ_COMP
// by chos 00.03.03
//	PDM1, PDM2 사용을 D1 방식으로 하기 위해 PDM1, PDM2 관련 routine disable
//	D1에서는 PDM1,2를 아래와 같이 사용
//		PDM1 -> VCACNT
//		PDM2 -> VGGDA
//    #define RF_HAS_PA_RANGE_CAL
    #undef RF_HAS_PA_RANGE_CAL
  #endif
#endif

/* The target has PCS capability if FEATURE_CDMA_1900 is defined
*/
#ifdef FEATURE_CDMA_1900
#define RF_HAS_PCS
#else
#undef RF_HAS_PCS
#endif

/* The target has 800Mhz-CDMA capability if FEATURE_CDMA_800 is defined
*/
#ifdef FEATURE_CDMA_800
#define RF_HAS_CDMA
#else
#undef RF_HAS_CDMA
#endif

/* The target has AMPS capability if FEATURE_ACP is defined
*/
#ifdef FEATURE_ACP
#define RF_HAS_FM
#else
#undef RF_HAS_FM
#endif

/* PDM1 & PDM2 is used to set the gain in the PA for PCS unless its done digitally
*/
#ifdef FEATURE_CDMA_1900
  #ifndef FEATURE_PA_RANGE_TEMP_FREQ_COMP
    #undef RF_CAP_PDM1_FOR_RX
    #define RF_CAP_PDM1_FOR_TX
    #define RF_CAP_PDM2_FOR_TX
  #endif
#endif

#undef RF_TARGET_HAS_CHARGER


// D1이외의 Project에서는 아래 내용의 RF Definition들이 모두 Cust(project명).h
// 로 옮겨간다. 
#if (MODEL_ID == MODEL_D1)
#ifdef FEATURE_SKTT_CAL			// kmh insert for calibration 00.06.13
/*---------------------------------------------------------------------------
  온도 보상 조정을 위한 추가 부분 Definition 
----------------------------------------------------------------------------*/
	#define CAGC_LNA_GN_POL_V CAGC_LNA_GN_POL_NRM_V
	#undef RF_HAS_TEMP_COMP
	#undef RF_HAS_PA_RANGE_CAL  /* Not use in A10 */
#endif	// #ifdef FEATURE_SKTT_CAL
#endif	// #if (MODEL_ID == MODEL_D1)

/* -----------------------------------------------------------------------
**                                Modules
** ----------------------------------------------------------------------- */
#elif (TG==T_MD)
#error code not present
#elif (TG==T_I2)
#error code not present
#elif (TG==T_PC)
#error code not present
#else
#error code not present
#endif

#endif /* RFICAP_H */

