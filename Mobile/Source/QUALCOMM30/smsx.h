#ifdef FEATURE_SMS
#ifndef SMSX_H
#define SMSX_H
/*===========================================================================

         S M S   T R A N S L A T I O N  D A T A   S T R U C T U R E S

DESCRIPTION
  This file contains data structures and functions for SMS translation
  services.  Translating SMS messages to and from external and internal
  formats.

Copyright (c) 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header:   L:/src/asw/COMMON/vcs/smsx.h_v   1.2   14 Dec 2000 13:14:00   chonda  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/14/00   cah     Removed uasmsi.h inclusion.
12/13/00   cah     Replaced obsolete smsi.h inclusion with uasmsi.h.
05/10/99   kmp     Changed T_SMS to FEATURE_SMS
06/29/95   day     Add #ifdef T_SMS to compile file with/without SMS code.
06/25/95   day     Created file. 

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"               /* Basic definitions */
#include "sms.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*     TYPES FOR SMS PARAMETER VALUES                                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* SMS Translation Status Codes                       */
/* Used to indicate status of SMS message translation */
typedef enum
{
SMSX_DONE_S = 0,        /* Successful translation completed    */
SMSX_IN_PROG_S,         /* SMS message translation in progress */
SMSX_INV_MSG_S,         /* Invalid SMS message type detected   */
SMSX_INV_PARM_S,        /* Invalid SMS parameter type detected */
SMSX_INV_TELESRV_S      /* Invalid SMS teleservice detected    */
} smsx_status_type;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/* Procedure which translates SMS messages from external to internal */
/* format                                                            */
extern word smsx_ext_to_int
       (
       word sms_len,
         /* Length of sms message in bytes */
       sms_parse_type  sms_parse,
         /* sms message parsing format type */
       byte *rx_sms_ptr,    
         /* pointer to received message to be translated */
       smsi_msg_type *int_sms_ptr
         /* pointer to place where translated message should be placed */
       );

/* Procedure which translates SMS messages from internal to external   */
/* format                                                              */
extern word smsx_int_to_ext
       (
       word *len_ptr,
         /* pointer to where message length, in bits should be placed */
       smsi_msg_type *int_msg_ptr,  
         /* pointer to internal message to be translated */
       byte *tx_sms_ptr
         /* pointer to place where translated message should be placed */
       );

#endif /* SMSX_H */
#endif /* FEATURE_SMS */

