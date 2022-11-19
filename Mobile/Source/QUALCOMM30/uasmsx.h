#ifdef FEATURE_UASMS /* SMS API */

#ifndef UASMSX_H
#define UASMSX_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           U A P I   S H O R T   M E S S A G E   S E R V I C E S (SMS)
             
             ---------- Message Encoding and Decoding Services

DESCRIPTION
  This module defines the data types and functions for the Message Services
  (SMS) message encoding and decoding services that are used by the client 
  software and are part of the UAPI (User Application Programming Interface). 

Copyright (c) 1998-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/uasmsx.h_v   1.3   31 Jan 2001 17:14:06   hqu  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/29/01   HQ      Properly feature-ized FEATURE_NEWUASMS.
10/09/00   HQ      Added handling of message tag.
08/25/00   vt      Moved uasmsx_encode_bearer_data() and 
                   uasmsx_decode_bearer_data() to uasms.h
10/28/99   HQ      Added flag into TL translation function for indicating
                   MO or MT msg.

===========================================================================*/


#ifdef __cplusplus
extern "C" {
#endif

         
/* <EJECT> */
/*===========================================================================

                             INCLUDE FILES

===========================================================================*/
#include "uasms.h"
#include "uasmsi.h"

         
/* <EJECT> */
/*===========================================================================
                             
                       API FUNCTION DECLARATIONS
                             
===========================================================================*/

#ifndef FEATURE_NEWUASMS
/*=========================================================================
FUNCTION 
  uasmsx_encode_bearer_data

DESCRIPTION
  This function encodes the SMS bearer data from the client 
  structure to the raw format.
  
DEPENDENCIES
  None

RETURN VALUE
  Encoding result. UASMS_OK_S means encoding succeeded; otherwise it failed.

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uasmsx_encode_bearer_data
(
  const uasms_client_bd_type                * cl_bd_ptr,       /* IN */
  uasms_raw_bd_type                         * raw_bd_ptr       /* OUT */
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasmsx_decode_bearer_data

DESCRIPTION
  This function decodes the SMS bearer data from the raw format
  to the client structure.
  
DEPENDENCIES
  None

RETURN VALUE
  Encoding result. UASMS_OK_S means encoding succeeded; otherwise it failed.

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uasmsx_decode_bearer_data
(
  const uasms_raw_bd_type            * raw_bd_ptr,   /* IN */
  uasms_client_bd_type               * cl_bd_ptr     /* OUT */
);
#endif /* FEATURE_NEWUASMS */

/* <EJECT> */
/*===========================================================================
                             
                       OTHER EXTERNAL FUNCTIONS
                             
===========================================================================*/

/*=========================================================================
FUNCTION 
  uasmsx_convert_cl2tl

DESCRIPTION
  This function converts the SMS TL data from the client structure
  to the internal structure.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
extern void uasmsx_convert_cl2tl
(
  const uasms_client_message_type     * cl_ptr,    /* IN */
  uasms_tl_message_type               * tl_ptr     /* OUT */
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasmsx_convert_tl2cl

DESCRIPTION
  This function converts the SMS TL data from the internal structure
  to the client structure.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
extern void uasmsx_convert_tl2cl
(
  const uasms_tl_message_type        * tl_ptr,   /* IN */
  uasms_client_message_type          * cl_ptr    /* OUT */
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasmsx_encode_CDMA_tl

DESCRIPTION
  This function encodes the SMS TL fields in CDMA mode.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uasmsx_encode_CDMA_tl
(
  const uasms_tl_message_type        * tl_msg_ptr,
  const uasms_raw_bd_type            * raw_bd_ptr,
  uasms_OTA_message_type             * OTA_msg_ptr   /* OUT */
);



/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasmsx_decode_CDMA_tl

DESCRIPTION
  This function decodes the SMS TL fields in CDMA mode.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uasmsx_decode_CDMA_tl
(
  const uasms_OTA_message_type     * OTA_msg_ptr,   /* IN */
  uasms_tl_message_type            * tl_ptr,        /* OUT */
  uasms_raw_bd_type                * raw_bd_ptr     /* OUT */
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasmsx_decode_AWISMS_tl

DESCRIPTION
  This function decodes the AWI SMS TL fields in Analog mode.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uasmsx_decode_AWISMS_tl
(
  const uasms_OTA_message_type       * OTA_msg_ptr,
  uasms_tl_message_type              * tl_msg_ptr,      /* OUT */
  uasms_raw_bd_type                  * raw_bd_ptr       /* OUT */
);



#ifdef __cplusplus
}
#endif

#endif /* UASMSX_H */

#endif /* FEATURE_UASMS */
