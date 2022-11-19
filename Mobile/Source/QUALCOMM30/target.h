#ifndef TARGET_H
#define TARGET_H
/*===========================================================================

      T A R G E T   C O N F I G U R A T I O N   H E A D E R   F I L E

DESCRIPTION
  All the declarations and definitions necessary for general configuration
  of the DMSS software for a given target environment.

Copyright (c) 1998,1999  by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/target.h_v   1.5   Jan 20 2000 11:17:32   efytal  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.02.28   chos    MODEL_DEP_H 추가하여 모델별로 compile하게 수정
00.02.28   chos    Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
08/13/99    vl     Added target T_Q2 for Q2 (kenny).
01/15/99   hrk     Integrating from COMMON_ARM.03.01.00 VU from MSM3000 subsystem.
                   Re-introduced #defines for all targets. Removal of support for
                   these targets will be done in the next phase.
12/16/98   jct     Created for MSM3000+ support

===========================================================================*/

#include	MODEL_DEP_H		// added by chos for SKTT project


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Values for TG.  These select the hardware platform.  The definitions
** start from 1, as an undefined value evaluates to 0.  Each target must
** use a unique number.
*/
#if 0
#define T_PC    1       /* PC test software, 80386/80486, running MSDOS  */
#define T_REX   4       /* REX test software, 80386/80486, REX and MSDOS */
#define T_DM    8       /* Diagnostic Monitor                            */
#define T_G     9       /* Gemini Portable hardware                      */
#define T_SP    12      /* Service Programming Tool                      */
#define T_Q     15      /* Q phones (Q-1900, Q-800)                      */             
#endif /* 0 */

#define T_PC    1       /* PC test software, 80386/80486, running MSDOS  */
#define T_P     2       /* Portable Phone hardware                       */
#define T_M     3       /* Mobile Phone hardware                         */
#define T_REX   4       /* REX test software, 80386/80486, REX and MSDOS */
#define T_S     5       /* Stretch-portable hardware                     */
#define T_B2    6       /* Beta II mobile hardware                       */
#define T_I1    7       /* ISS1 WLL hardware                             */
#define T_DM    8       /* Diagnostic Monitor                            */
#define T_G     9       /* Gemini Portable hardware                      */
#define T_I2    10      /* ISS2 WLL hardware                             */
#define T_C1    11      /* CSS1 WLL hardware                             */
#define T_SP    12      /* Service Programming Tool                      */
#define T_T     13      /* TGP (Third Generation Portable)               */
#define T_MD    14      /* 1900 MHz Module (Charon)                      */
#define T_Q     15      /* Q phones (Q-1900, Q-800)                      */
#define T_O     16      /* Odie (5GP)                                    */
#define T_Q2    17      /* Q2 (kenny)                                    */

#ifndef TG
#error TG must be defined to a known target
#endif

/* Select a header file containing general configuration for a specific target
*/
#if (TG==T_PC)
#error code not present
#elif (TG==T_REX)
#error code not present
#elif (TG==T_DM)
#error code not present
#elif (TG==T_G)
#include "targetg.h"    /* Target definitions for Gemini Portable  */
#elif (TG==T_Q)
#error code not present
#elif (TG==T_O)
#error code not present
#else
#error TG must be defined to a known target
#endif

/* Include customer specific information at this time.  Note this is a
** temporary solution and will eventually be removed.
*/
#ifdef CUST_H
#include "customer.h"

/* Certain features are changing their names from T_xxx to FEATURE_xxx.
** Additionally, they are moving from targetg.h to custxxx.h.  For now,
** I want to ignore the targetg.h settings, and instead make them match
** the custxxxx.h settings.
*/
#undef T_ACP
#ifdef FEATURE_ACP
#define T_ACP
#endif

#undef T_AUTH
#ifdef FEATURE_AUTH
#define T_AUTH
#endif

#undef T_SMS
#ifdef FEATURE_SMS
#define T_SMS
#endif

#undef T_SMS_UI
#ifdef FEATURE_UI_SMS
#define T_SMS_UI
#endif

#endif /* CUST_H */

/* Lint never thinks these files get used, but still we need them.
** So the following line shuts up lint, as we know we're right.
*/
/*lint -efile(766,target.h)   */
/*lint -efile(766,targetpc.h) */
/*lint -efile(766,targetdm.h) */
/*lint -efile(766,targetg.h)  */
/*lint -efile(766,targetsp.h) */
/*lint -efile(766,targetq.h)  */

#endif /* TARGET_H */

