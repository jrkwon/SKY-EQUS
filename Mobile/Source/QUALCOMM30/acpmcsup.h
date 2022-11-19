#ifndef ACPMCSUP_H
#define ACPMCSUP_H
/*===========================================================================
                        
                     A  C  P  M  C  S  U  P

DESCRIPTION
  Contains miscellaneous ACP MC support routines

Copyright (c) 1992,1993,1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1995,1996,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpmcsup.h_v   1.0   Aug 11 2000 11:40:46   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/13/99   lh      CM and SMS phase 2 implementations.
07/28/99   lh      CM and two-way SMS support added.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
10/11/96   jjw     DOTG/AMPS data capability integration fixes
05/24/96   jjw     Gemini & ISS2 Data Services release.
11/29/95   jca     Deleted function acpmcsup_check_sid_acq_list().
06/15/95   jca     Added function acpmcsup_decode_called_address().
02/26/92   jca     Added acpmcsup_check_locked_out_sids and
                   acpmcsup_check_sid_acq_list procedures.
12/11/92   JCA     Changed ACP_MAX_DIALED_DIGITS --> NV_MAX_DIAL_DIGITS
11/16/82   JCA     Added acpmcsup_set_roam_status() function.
07/09/92   JCA     Initial release to DMSS PVCS.
02/14/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "acp553.h"
#include "mc.h"
#include "nv.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
  Misc constants for MC states
---------------------------------------------------------------------------*/

/* The following macros are used when calling acpmcsup_autonomous_reg_update
   to enhance code readability. */

#define SUCCESS  1   /* Equates to Boolean TRUE value */
#define FAILURE  0   /* Equates to Boolean FALSE value */

/*--------------------------------------------------------------------------
   Overhead message word types (equated from actual field values)
---------------------------------------------------------------------------*/

typedef enum {
  ACPMCSUP_RESCAN_ACT             = ACP553_RESCAN_ACT,
  ACPMCSUP_REGINCR_ACT            = ACP553_REGINCR_ACT,
  ACPMCSUP_LOCAID_ACT             = ACP553_LOCAID_ACT,
  ACPMCSUP_CDMA_AVAILABLE_ACT     = ACP553_CDMA_AVAILABLE_ACT,
  ACPMCSUP_NEW_ACCESS_CHAN_ACT    = ACP553_NEW_ACCESS_CHAN_ACT,
  ACPMCSUP_RANDOM_CHALLENGE_A_ACT = ACP553_RANDOM_CHALLENGE_A_ACT,
  ACPMCSUP_OVERLOAD_CTL_ACT       = ACP553_OVERLOAD_CTL_ACT,
  ACPMCSUP_ACCESS_TYPE_PARMS_ACT  = ACP553_ACCESS_TYPE_PARMS_ACT,
  ACPMCSUP_ACCESS_ATT_PARMS_ACT   = ACP553_ACCESS_ATT_PARMS_ACT,
  ACPMCSUP_RANDOM_CHALLENGE_B_ACT = ACP553_RANDOM_CHALLENGE_B_ACT,
  ACPMCSUP_LOCAL_CONTROL_1_ACT    = ACP553_LOCAL_CONTROL_1_ACT,
  ACPMCSUP_LOCAL_CONTROL_2_ACT    = ACP553_LOCAL_CONTROL_2_ACT,
  ACPMCSUP_REGISTRATION_ID        = ACP553_REG_ID_OHD
} acpmcsup_ovrhd_train_type;

/* <EJECT>  */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/* <EJECT>  */
/*===========================================================================

FUNCTION ACPMCSUP_PARSE_OVRHD_MSG_TRAIN

DESCRIPTION
  This function parses thru an overhead message train searching for the 
  passed overhead message type. If a match is found the passed pointer
  is set to point to the word and the return status will be TRUE. Otherwise
  the return status is FALSE. The actual check will look at either the ACT
  field (for global action messages) or the OHD field (for the registration
  ID message.

  Sample Code in how to use this procedure:
  -----------------------------------------

  acp553_rxmsg_type rxmsg;
  acp553_rxword_type *rxword;
  acpmcsup_parse_ovrhd_train_type action;

  if (acpmcsup_parse_ovrhd_msg_train(&rxmsg,&rxword,ACPMCSUP_REGINCR_ACT)) {
      regid = ACPRX_OH_REGID( rxword );
  }


DEPENDENCIES
  The address pointed to by rxmsg_ptr must contain an overhead message.

RETURN VALUE
  TRUE if the specified message word was found, otherwise FALSE.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmcsup_parse_ovrhd_msg_train
(
  acp553_rxmsg_type  *rxmsg_ptr,  /* pointer to acp553_rx_msg_type         */
  acp553_rxword_type **wrd_ptr,   /* Address of pointer to a acp553_rxword */
  acpmcsup_ovrhd_train_type code  /* Code to look for in the ovhd train    */
);

/* <EJECT>  */
/*==========================================================================

FUNCTION ACPMCSUP_SET_ACCESS_CHANS
 
DESCRIPTION
  This function will determine then set the first access channel 
  (FIRSTCHAs) and the last access channel (LASTCHAs) according to
  CAI sections 2.6.1.2.2 and 2.6.2.1. (they are functionally identical).
  
DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 

extern void acpmcsup_set_access_chans( void);

/* <EJECT>  */
/*===========================================================================

FUNCTION ACPMCSUP_VALID_CHANNEL_NUMBER

DESCRIPTION
  This function determines whether the given channel number is within
  the set allocated to cellular systems.

DEPENDENCIES
  None

RETURN VALUE
  TRUE   The channel number is valid.
  FALSE  The channel number is invalid.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmcsup_valid_channel_number
(
  word chanpos   /* The channel number to validate */
);

/* <EJECT>  */
/*==========================================================================

FUNCTION ACPMCSUP_UPDATE_NUMERIC_VALUES
  
DESCRIPTION
  This function will update the acp553 numeric values using the passed
  overhead msg train per CAI section 2.6.2.1 and AMPS section 4.2.6.
  
DEPENDENCIES
  The passed overhead train must be for the same SID as the mobile (ie.
  SIDr == SIDs).
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 

extern void acpmcsup_update_numeric_values
(
  acp553_rxmsg_type *msg_train_ptr
);

/* <EJECT>  */
/*===========================================================================

FUNCTION ACPMCSUP_ENCODE_CALLED_ADDRESS

DESCRIPTION
  This function converts called-address digits received from UI
  (in ASCII format) to the binary coded format specified in the
  CAI Table 2.7.1.1-1 (one digit per byte).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmcsup_encode_called_address
(
  byte number_of_digits,
    /* How many digits in the called address */
  byte ascii_called_address[NV_MAX_DIAL_DIGITS+1],
    /* Input called-address digits (in ASCII format) */
  byte encoded_called_address[NV_MAX_DIAL_DIGITS]
    /* Output called-address digits (in binary coded format) */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCSUP_DECODE_CALLED_ADDRESS

DESCRIPTION
  This function converts binary coded called-address digits (one digit
  per byte as specified in IS-95 Table 2.7.1.1-1) to ASCII format.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmcsup_decode_called_address
(
  byte number_of_digits,
    /* How many digits in the called address */
  byte encoded_called_address[NV_MAX_DIAL_DIGITS],
    /* Input called-address digits (in binary coded format) */
  byte ascii_called_address[NV_MAX_DIAL_DIGITS]
    /* Output called-address digits (in ASCII format) */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCSUP_AUTONOMOUS_REG_UPDATE

DESCRIPTION
  This function performs the protocol operations as specified in
  CAI Section 2.6.3.11 Autonomous Registration Update.

DEPENDENCIES
  The processing this function performs is dependent upon the value
  of the first-registration ID status global variable
  (acp553.first_reg_status_enabled).

RETURN VALUE
  None

SIDE EFFECTS
  This function will (possibly) modify the following global variables:
    acp553.sid_sp
    acp553.nxtref_sp

===========================================================================*/

extern void acpmcsup_autonomous_reg_update
(
  boolean success
    /* Indicates whether registration attempt was successful */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCSUP_SET_ROAM_STATUS

DESCRIPTION
  This function sets the roam status based on the SID and home system
  variables.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void acpmcsup_set_roam_status( void );

/* <EJECT> */
/*==========================================================================

FUNCTION ACPMCSUP_CHECK_LOCKED_OUT_SIDS
  
DESCRIPTION
  This function checks the passed SID against the list of locked-out SIDs
  and returns TRUE if any match is found; otherwise FALSE is returned.
   
DEPENDENCIES
  None

RETURN VALUE
  TRUE if the passed SID matches any of the locked-out SIDs, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/ 

extern boolean acpmcsup_check_locked_out_sids
(
  word sid
    /* SID to use for checking */
);

#endif
