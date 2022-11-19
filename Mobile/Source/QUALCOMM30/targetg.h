#ifndef TARGETG_H

#define TARGETG_H
/*===========================================================================

              PORTABLE  TARGET  CONFIGURATION  HEADER  FILE

DESCRIPTION
  All the declarations and definitions necessary for general configuration
  of the software for a Gemini target environment.

Copyright (c) 1994,1995,1996,1997,1998,1999,2000,2001 by QUALCOMM, Incorporated.
All Rights Reserved.
===========================================================================*/

/*===========================================================================
                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/MSM5000/VCS/targetg.h_v   1.4   02 Apr 2001 20:22:20   knordin  $

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.03.01   chos    RAM memory map 변경
00.03.01   chos    Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
09/26/00   nxb     RAM_SIZE should be 512K not 256K affect RAM test.
09/20/98   hcg     Removed conditional compiles - Only included if TG==T_G
                   Added comments.
09/15/98   udp     Replaced _ARM_ --> T_ARM
09/13/98   hcg     Updated for MSM3000 support.
04/18/97   dna     Moved feature related items to custg.h and removed target
                   defines which have been replaced with feature defines.
03/05/97   jjn     Deleted FEATURE_UI_SEND_PIN and FEATURE_UI_FORCE_CALL
                   and added them to custq.h
01/16/97   BK      Added comments next to the features that did not have any.
01/13/97   BK      Added FEATURE_CAR_KIT_DTMF.
01/07/97   rdh     Added FEATURE_UI_SILENT_REDIAL option.
12/23/96   smo     Added FEATURE_PILOT_SNIFFER option for TMOBILE.
10/03/96   cah     Added FEATURE_UI_FORCE_MODE_SEL.
05/04/96   rdh     Made MC handoff debug #ifdef T_HO_DEBUG to save ROM.
04/23/96   rdh     Added T_SRCHLOG feature switch, used in field eval only.
04/10/96   bk      Turned on FEATURE_UI_FORCE_CALL and SEND_PIN temporarily.
04/03/96   jah     Added T_MSM2 (Uses the MSM 2 family CDMA chip) and
                   T_AUTO_ICU (Auto-detect ICU type)
02/28/96   dna     Added T_BB to specify that the Gemini has a Boot Block.
12/27/95   gb      Added gemini TMOBILE condition.
10/10/95   DAK     Moved conditional compilation defines to uiint.h
09/25/95   DAK     Added new defines for conditional compilation of code.
08/23/95   dna     Turned on T_AUTH
08/22/95   jah     Turned on T_SMS_UI
07/12/95   jah     Turned on T_SMS option, and some comments
07/06/95   rah     Removed T_13K definition.
06/14/95   gb      Added T_13K for target Rate Set 2 support.
06/13/95   rdh     Made T_SLEEP handling explicit and consistent DMSS-wide.
06/06/95   jah     Added T_CPU_HZ, defined T_SLEEP
05/10/95   jah     defined T_ACP
04/27/94   jah     created file

===========================================================================*/

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define T_MSM2   1      /* MSM 2 family CDMA chip support                    */

#define T_MSM2P  1      /* MSM 2p CDMA chip support                          */

#define T_MSM23  1      /* MSM 2.3 CDMA chip support                         */

#undef  T_AUTO_ICU      /* Auto-detect ICU support                           */

#define T_DLOAD  1      /* Download support                                  */

#define T_SLEEP  1      /* Sleep support                                     */

#undef T_BB             /* Boot block support                                */

#define T_MSM3   1      /* MSM 3 family CDMA chip support                    */

/***************************************************************************

                        Well-Known Memory Locations

***************************************************************************/
#if defined (T_MSM3)

// modified by chos 00.03.01 for D1
//	RAM memory map 변경
#include "target.h"
#if	( MODEL_ID == MODEL_D1 )||(MODEL_ID==MODEL_A10)

#define RAM_BASE         0x01380000  /* base of RAM for MSM3000 targets */
#define RAM_SIZE         0x000FFFFF  /* RAM size - currently 1M */

#elif	( MODEL_ID == MODEL_B1 )
// #define RAM_BASE         0x011C0000  /* base of RAM for MSM3000 targets */
									 // 일단 256kb ram을 기준으로 생각함 00.08.15
#define RAM_BASE         0x01180000  /* 512kb ram기준 사용 01.01.17 */
#define RAM_SIZE         0x000FFFFF  /* RAM size - currently 1M */

#elif	( MODEL_ID == MODEL_B2 )
// #define RAM_BASE         0x011C0000  /* base of RAM for MSM3000 targets */
									 // 일단 256kb ram을 기준으로 생각함 00.08.15
#define RAM_BASE         0x01180000  /* 512kb ram기준 사용 01.01.17 */
#define RAM_SIZE         0x000FFFFF  /* RAM size - currently 1M */

#else	// #if	( MODEL_ID == MODEL_D1 )
#define RAM_BASE         0x01000000  /* base of RAM for MSM3000 targets */
#define RAM_SIZE         0x0007FFFF  /* RAM size - currently 256K */
#endif	// #if	( MODEL_ID == MODEL_D1 )

#define IRQ_OFFSET       0x0000      /* offset from base of RAM to IRQ
					instruction that is vectored through
					RAM */

#define FIQ_OFFSET       0x0004      /* offset from base of RAM to FIQ 
					instruction that is vectored through
					RAM */

#endif

#define APP_ENTRY_POINT  0x00004000  /* entry into application space - Boot block
				        reserves 16K */
				      
#define MOB_MODEL_LOC    0x00000024  /* mobile model number location */

/*----------------------------------------------------------------------------
               CHECK BLOCK SEGMENT & OFFSET POSITIONS

  The last bytes before the boot block segment are reserved for the check
  block.  The different items in the check block are located with different
  offsets from the beginning of the block.  The check block begins on a 16
  byte (segment) boundary.
----------------------------------------------------------------------------*/

#if defined(T_ARM)

#define ROM_CHECK_LOC            0x000FFFDC  /* check location for ROM test in 
		 				  boot sequence */
#define SEC_CODE_BYTES 8                     /* length of security code 
						(in bytes) */
#define SEC_CODE_LOC             0x44        /* location in ROM of security 
						code */
#else

#define BOOT_CHECK_SEG           0x00FBFE    /* Segment of check block */
#define BOOT_CHECK_LOC           0x00000     /* Offset of pre-determined value 
						used to validate ROM */
#endif

/*----------------------------------------------------------------------------
                      ROM VALIDATION NUMBER VALUES

  The hex image flashed into ROM should have this value written at the
  location in the check block specified above.  This value is different
  for the different targets, so the wrong code can't get flashed in and
  still be considered valid.
----------------------------------------------------------------------------*/

#if defined(T_MSM3)
#define ROM_CHECK_NUM 0x0001D     /* specific check value for SURF,
				     NGP+MSM3 */
#else
#define ROM_CHECK_NUM 0x00007     /* Specific check value for Gemini */
#endif

#ifdef CUST_MOB_MODEL

/* Set mob_model as specified in customer build files
*/
#define MODEL_NUMBER  CUST_MOB_MODEL

#else

#if defined (T_MSM3)
#define MODEL_NUMBER  0x1D
#else
#define MODEL_NUMBER  0x07    /* Gemini or Module     */
#endif

#endif

/* The CPU clock rate of this target (13.5 MHz)
*/
#define T_CPU_HZ 13500000L

#endif /* TARGETG_H */
