#ifndef CAVE_H
#define CAVE_H
/*===========================================================================

                   C A V E    H E A D E R    F I L E

DESCRIPTION
  This file contains definitions for interface to the CAVE
  (Cellular Authentication, Voice Privacy, and Encryption)
  functions.

   Copyright (c) 1994, 1995, 1999 QUALCOMM INCORPORATED.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/cave.h_v   1.0   Aug 11 2000 11:43:20   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/24/00   ck      Externalised the function CAVE_update_keys() that sets 
                   the CMEA and VPM key generated by the RUIM. Undefed the
                   other functions based on FEATURE_RUIM
04/06/00   va      Merged the following from MSM3100_CP.03.00.17
           ry      Added OTAPA support
06/17/99   ck      Changed the parameters to CAVE_generate_key 
01/20/99   ck      Added the function CAVE_update_A_key_683
05/15/96   dna     Modified interface to CAVE_generate_key
06/07/95   dna     Modified file for initial Authentication implementation
04/20/94   ptw     Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "qw.h"
#include "ulpn.h"
#include "auth.h"
#include "authi.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* ----------------------------------------------
** During SSD Update, the new SSD is used in the
** auth_signature, otherwise the old SSD is used.
** ---------------------------------------------- */
#define  CAVE_SSD_USE_NEW  TRUE
#define  CAVE_SSD_USE_OLD  FALSE

/* Defines whether or not auth_signature is to save its register values */
#define  CAVE_SAVE_REGISTERS        TRUE
#define  CAVE_DONT_SAVE_REGISTERS   FALSE

/* Length of SDD_A and SSD_B */
#define  CAVE_SSD_LEN           64

/* Length of the A-key */
#define  CAVE_A_KEY_LEN         64

/* Number of decimal digits of the entered A-key which are the A-key data */
#define  CAVE_DATA_DIGITS       20

/* Number of decimal digits of the entered A-key which are the checksum */
#define  CAVE_CHECKSUM_DIGITS   6

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#if defined (FEATURE_RUIM_AUTH_SIMLN) || !defined (FEATURE_RUIM)
/*===========================================================================

FUNCTION CAVE_init

DESCRIPTION
  This function is called to perform initialization for the CAVE
  operations.  This mainly involves setting some variables from their
  values in semi-permanent memory.

DEPENDENCIES
  This function should only be called once on powerup or NAM change.

RETURN VALUE
  AUTH_INIT_OK         if initialization is successful
  AUTH_INIT_FAILED     if initialization fails
  AUTH_INIT_BAD_SSD    if SSD is unitialized or invalid

SIDE EFFECTS
  None.

===========================================================================*/

auth_init_status_type CAVE_init
(
  byte   NAM_index
      /* Which NAM to take the CAVE variables from */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_validate_A_key

DESCRIPTION
  This function checks the internal check digits of a manually-entered
  authentication key.

  The manually entered key consists of two parts.  The last six digits,
  taken as a numeric value, are the check digits.  The first 0 to 20
  digits, taken as a numeric value, is the A-key.  This function runs
  the A-key (and some other stuff) through the CAVE algorithm.  The
  output should match the value in the check digits.

DEPENDENCIES
  CAVE_init must be called before calling this function.

RETURN VALUE
  Returns TRUE if the A-key is validated, FALSE if not.

SIDE EFFECTS
  None.

===========================================================================*/

boolean  CAVE_validate_A_key
(
  const byte full_A_key[AUTH_A_KEY_DIGITS]
    /* A-key to be validated, given as ascii digits */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_update_A_key

DESCRIPTION
  This function installs a new A-key in the phone.

DEPENDENCIES
  None.

RETURN VALUE
  Returns TRUE if the A-key is updated successfully, FALSE if not.

SIDE EFFECTS
  NV is updated: the new A-key is stored, and SSD is cleared.

===========================================================================*/

boolean  CAVE_update_A_key
(
  const byte full_A_key[AUTH_A_KEY_DIGITS],
    /* A-key to be put into NV, given as ascii digits */

  byte   NAM_index
    /* Which NAM is to be updated */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_generate_SSD

DESCRIPTION
  This function generates new A and B values of Shared Secret Data.

DEPENDENCIES
  CAVE_init must be called before this function is called.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void  CAVE_generate_SSD
(
  qword  rand_ssd
    /* The random input to the SSD generation process */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_update_SSD

DESCRIPTION
  This function installs new A and B values of SSD into the phone.

DEPENDENCIES
  None.

RETURN VALUE
  Returns TRUE if the SSD is updated successfully, FALSE if not.

SIDE EFFECTS
  NV is updated: the new SSD is stored.

===========================================================================*/

boolean CAVE_update_SSD(void);

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_auth_signature

DESCRIPTION
  This function computes an authentication signature as specified in
  "Common Cryptographic Algorithms".

DEPENDENCIES
  CAVE_init must have been called before this function is called.

RETURN VALUE
  Returns the computed AUTH_SIGNATURE.

SIDE EFFECTS
  If save_regs is nonzero, the saved_* values are updated.

===========================================================================*/

dword  CAVE_auth_signature
(
  dword     rand_challenge,
    /* The random number unique to this authentication challenge */

  dword     auth_data,
    /* 24 bits of data to be signed */

  boolean   use_new_SSD,
    /* TRUE if new_SSD_A and new_SSD_B are to be used, FALSE if the
       current SSD_A and SSD_B are to be used. */

  boolean   save_registers
    /* TRUE if this authentication is to be used as seed for message
       encryption and voice privacy, FALSE if not.  If TRUE, the
       saved_* variables will be updated. */

#ifdef FEATURE_OTASP_OTAPA
 ,boolean   validating_spasm
    /* TRUE if we are calculating the auth_otapa for SPASM validation, 
       where SSD_AUTH input parameter is set to exclusive OR of SSD_A
       and A_KEY. */
#endif /* FEATURE_OTASP_OTAPA */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_generate_key

DESCRIPTION
  This function generates the CMEA key (used for message encryption)
  and voice privacy mask for a call.

DEPENDENCIES
  CAVE_auth_signature must have been called with save_registers set
  to TRUE before this function may be called.

RETURN VALUE
  None.

SIDE EFFECTS
  Changes value of CAVE_lfsr, CAVE_offset_1 and 2, CAVE_mixing_reg.
  Also, it sets the flag used by other tasks (MC) to determine if it is
  OK to call the encryption routines.

===========================================================================*/

void  CAVE_generate_key
(
  boolean do_VPM,
    /* Indicates whether or not the full VPM calculation is to be done */
  boolean *valid_CMEA_key_flag_ptr,
    /* Pointer allows immediate update of this flag for other tasks */
  boolean use_new_SSD
    /* If authr that was computed for re-auth response was based on new ssd,
       this is set to TRUE */
);

#endif /* FEATURE_AUTH_SIMLN || ! FEATURE_RUIM */

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_encrypt

DESCRIPTION
  This function encrypts a message buffer, in place, using the CMEA
  algorithm from "Common Cryptographic Algorithms".

DEPENDENCIES
  CAVE_generate_key must have been called before this function is called.

RETURN VALUE
  None.  (But the message buffer is encrypted, in place.)

SIDE EFFECTS
  None.

===========================================================================*/

void  CAVE_encrypt
(
  byte   msg_buf[],
    /* Message buffer to be encrypted */

  int    msg_len
    /* Number of bytes in the message buffer */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CAVE_private_lcm

DESCRIPTION
  This function returns the private long code mask for CDMA voice privacy.
  The private LCM is defined to be '01' concatenated with the 40 LSBs of VPM.

DEPENDENCIES
  CAVE_generate_key must have been called before this function is called.

RETURN VALUE
  Returns the private long code mask.

SIDE EFFECTS
  None.

===========================================================================*/

ulpn_type CAVE_private_lcm(void);

#if defined (FEATURE_AUTH_SIMLN) || ! defined (FEATURE_RUIM)

/*===========================================================================

FUNCTION CAVE_update_A_key_683

DESCRIPTION
  This function installs a new A-key in the phone generated using IS-683A.

DEPENDENCIES
  None.

RETURN VALUE
  Returns TRUE.

SIDE EFFECTS
  NV is updated: the new A-key is stored

===========================================================================*/

boolean  CAVE_update_A_key_683
(
  qword new_A_key,
    /* A-key to be put into NV */

  byte NAM_index
    /* Which NAM is to be updated */
);
#endif /* FEATURE_AUTH_SIMLN || ! FEATURE_RUIM */

#ifdef FEATURE_RUIM
/*===========================================================================

FUNCTION CAVE_update_keys

DESCRIPTION
  This function updates the private long code mask for CDMA voice privacy.
  Also updates the CMEA key that is used by CAVE.

DEPENDENCIES
  Depends on Intel byte order.

RETURN VALUE
  None

SIDE EFFECTS
  Update the CMEA and VPM.

===========================================================================*/

void CAVE_update_keys
(
  byte *data
);
#endif /* FEATURE_RUIM */

#endif /* CAVE_H */
