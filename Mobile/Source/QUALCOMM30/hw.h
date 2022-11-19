#ifndef HW_H
#define HW_H
/*===========================================================================

                H A R D W A R E   U T I L I T I E S
                      H E A D E R    F I L E

DESCRIPTION
  This file contains delarations necessary to use the utilities contained
  in hw.c.

Copyright (c) 1992, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/hw.h_v   1.3   19 Dec 2000 13:23:58   bcalder  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/19/00   bgc     Added MSM5000C.
02/16/00    eh     Added revision for MSM5000 
04/12/99   rm      Added definitions for Lynx (MSM2310)
08/04/09   ychan   Added new revision value MSM_30_A2.
06/26/98   kss     Added definitions for Lynx (MSM2310)
03/20/98   aks     Added definitions for the alternate LCD and the enhanced
                   Audio circuit capability/configuration.
12/10/97   aks     Modified and added definitions for MSM, up to and including
                   2.3 rev A4.
11/02/96   jah     Configured for TGP (T_T), by adding comments for a block
                   of definitions which are obsolete.
03/08/96   jah     Added HW_IS_Q186() to determine native CPU type, and
                   added definitions for MSM 2.2 A0, A1, A2, and B0
11/28/95   jah     Added prototypes for hw_init() and hw_capability().
05/10/95   jah     Added ISS-1 as phone with old-style EEPROM
05/10/95   jah     Added hw_version() and Gemini support
10/30/92   jah     Added hw_force_offline().
08/06/92   jah     Corrected EEPROM length
07/06/92   jah     Added hw_reset()
05/29/92   jah     Created/Ported from the brassboard

===========================================================================*/

#include "target.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*-------------------------------------------------------------------------*/

/* Optional Hardware capabilities available on this phone
**
** The masks are to be used with the return value from hw_capability()
** to determine whether or not an option is available/enabled.
*/
#define HW_CAP_BASE_V           0x00000000      /* Base configuration */

#define HW_CAP_ENH_RINGER_M     0x00000001      /* Enhanced Ringer    */
#define HW_CAP_ENH_RINGER_EN_V  0x00000001      /* enabled/disabled   */
#define HW_CAP_ENH_RINGER_DIS_V 0x00000000

#define HW_CAP_ALT_LCD_M        0x00000002      /* Alternate LCD      */
#define HW_CAP_ALT_LCD_EN_V     0x00000002      /* enabled/disabled   */
#define HW_CAP_ALT_LCD_DIS_V    0x00000000

#define HW_CAP_ENH_AUDIO_M      0x00000004      /* Enhanced Audio Circuit */
#define HW_CAP_ENH_AUDIO_EN_V   0x00000004      /* enabled/disabled       */
#define HW_CAP_ENH_AUDIO_DIS_V  0x00000000

typedef unsigned long hw_capability_type;

/* Values for each rev of the MSM ASIC.
*/
#define MSM_2_REV_3     0x0101
#define MSM_2_REV_4     0x0201        /* MSM 2P  Rev 4  */
#define MSM_2_REV_5     0x0301        /* MSM 2P1 Rev 5  */

#define MSM_22_A0       0x1002        /* MSM 2.2 Rev A0 */
#define MSM_22_A1       0x1102        /* MSM 2.2 Rev A1 */
#define MSM_22_A2       0x1202        /* MSM 2.2 Rev A2 */
#define MSM_22_B0       0x1303        /* MSM 2.2 Rev B0 */
#define MSM_22_B1       0x1403        /* MSM 2.2 Rev B1 */
#define MSM_22_B2       0x1404        /* MSM 2.2 Rev B2 */
#define MSM_22_B3       0x1404        /* MSM 2.2 Rev B3 */

#define MSM_23_A0       0x2005        /* MSM 2.3 Rev A0 */
#define MSM_23_A1       0x2105        /* MSM 2.3 Rev A1 */
#define MSM_23_A2       0x2205        /* MSM 2.3 Rev A2 */
#define MSM_23_A3       0x2305        /* MSM 2.3 Rev A3 */
#define MSM_23_A4       0x2405        /* MSM 2.3 Rev A4 */

#define MSM_2310_A0     0x3000        /* MSM 2310 */

#define MSM_30_A0       0x4000        /* MSM 3.0 Rev A0, only upper byte is valid. */
#define MSM_30_A2       0x4100        /* MSM 3.0 Rev A1/A2. Same ID for both. */
#define MSM_30_A3       0x4200        /* MSM 3.0 Rev 3 */
#define MSM_30B_A0      0x4300        
#define MSM_30C_A0      0x4400

#ifdef T_MSM5000
#define MSM_50_A0       0x6000        
#define MSM_50B_A0      0x6400
#define MSM_50C_A0      0x6500
#endif /*ifdef T_MSM5000*/

/* Mask for masking off the firmware id from id values */
#define HW_MSM_VERSION_M 0xFF00


/*===========================================================================

                            MACRO DECLARATIONS

===========================================================================*/


/*===========================================================================

MACRO HW_IS_Q186

DESCRIPTION
  Determine whether or not the native processor is a Q186.

PARAMETERS
  None

DEPENDENCIES
  If running MICE mode, the CPU must match the native CPU in order
  for this test to indicate the type of CPU being used.

RETURN VALUE
  TRUE   -- The native processor is a Q186
  FALSE  -- The native processor is not an Q186

SIDE EFFECTS
  None

===========================================================================*/

#define HW_IS_Q186() (hw_version() >= 0x1200)


/*===========================================================================

MACRO HW_MSM_VERSION

DESCRIPTION
  Strips vocoder firmware information from the MSM version id value;
  useful if only the MSM hardware version is wanted.

PARAMETERS
  The 16-bit id value to have firmware info masked off

DEPENDENCIES
  None

RETURN VALUE
  version id with firmware info masked off.

SIDE EFFECTS
  None

===========================================================================*/

#define HW_MSM_VERSION( version ) ( ( version ) & HW_MSM_VERSION_M )



/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION HW_VALID_ADDR

DESCRIPTION
  This procedure verifies that an address is within legal range.

DEPENDENCIES
  None

RETURN VALUE
  True if the address is legal, False if not.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean hw_valid_addr
(
  void *ptr,            /* Address to validate                       */
  word len              /* Length of memory block pointed to by *ptr */
);


/*===========================================================================

FUNCTION HW_POWER_OFF

DESCRIPTION
  Turn off the power to the phone.

DEPENDENCIES
  None

RETURN VALUE
  Does not return

SIDE EFFECTS
  None

===========================================================================*/
extern void hw_power_off( void );


/*===========================================================================

FUNCTION HW_RESET

DESCRIPTION
  Reset the phone.

DEPENDENCIES
  None

RETURN VALUE
  Does not return

SIDE EFFECTS
  None

===========================================================================*/
extern void hw_reset( void );


/*===========================================================================

FUNCTION HW_FORCE_OFFLINE

DESCRIPTION
  See if the phone is to be forced into offline.

DEPENDENCIES
  None

RETURN VALUE
  True if the phone is to be forced offline, False if not.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean hw_force_offline( void );


/*===========================================================================

FUNCTION HW_VERSION

DESCRIPTION
  Return the hardware version

DEPENDENCIES
  None

RETURN VALUE
  16-bit hardware version

SIDE EFFECTS
  None

===========================================================================*/
extern word hw_version( void );


/*===========================================================================

FUNCTION HW_INIT

DESCRIPTION
  This function determines the options this hardware supports,
  and returns that information to the caller.

DEPENDENCIES
  None

RETURN VALUE
  Mask of options the hardware is supports.

SIDE EFFECTS
  None

===========================================================================*/
extern void hw_init
(
  dword config_value /* Configuration value read from NVM */
);


/*===========================================================================

FUNCTION HW_CAPABILITY

DESCRIPTION
  This function determines the options this hardware supports,
  and returns that information to the caller.

DEPENDENCIES
  hw_init() must be called before hw_capability() is called.

RETURN VALUE
  Mask of options the hardware is supports.

SIDE EFFECTS
  None

===========================================================================*/
extern hw_capability_type hw_capability( void );
#if MODEL_ID == MODEL_A10
extern byte confirm;
#endif
#endif  /* HW_H */

