#ifndef DMDDOWN_H
#define DMDDOWN_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    M S M 5 0 0 0  D E M O D U L A T O R  D O W N L O A D  S E R V I C E S
                           H E A D E R  F I L E

DESCRIPTION
  This file contains declaration for MSM5000 demodulator download routines.

REFERENCES

Copyright (c) 1995,1996,1997,1998,1999 by QUALCOMM, Incorporated.  
All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/dmddown.h_v   1.2   24 Aug 2000 17:51:50   robertom  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/19/00  rmd       Defined the index of the demodulator RAM memory. 
01/17/00  eh        Update header



===========================================================================*/


/*===========================================================================

                     INCLUDE FILE FOR THIS HEADER FILE

===========================================================================*/
#include "target.h"                    /* Target specific definitions      */

#define DEM_FIRMWARE_START_DOWNLOAD     0x00
#define DEM_FIRMWARE_END_DOWNLOAD       0x01

#define DEMOD_FIRMWARE_SEL              0x0e    /*Index of the demodulator RAM memory*/

/* TYPEDEFS, ENUMS & STRUCTS. 
 * --------------------------
 */

typedef struct                 
{
   const word * ins_data;               /* Ptr. to inst data words in flash. */
   word  ins_start_address;             /* 1st Dest. address in IRAM. */
   word  ins_data_length;               /* # of word to be written. */   
} demod_download_data;

typedef volatile enum {
  DEMOD_NOTHING_DOWNLOADED = 0,       /* a) Power-up state only.       */
  DEMOD_BUSY_DOWNLOADING,             /* b) Download in progress       */
  DEMOD_DOWNLOAD_COMPLETED,           /* c) Demod instruction data is  */
  DEMOD_MAX_STATI                     /*    resident in Demod memory.  */
} demod_download_status_type;         


/* GLOBAL VARIABLES. 
 * -----------------
 */
extern demod_download_status_type     demod_download_status;


/* FUNCTION PROTOTYPES. 
 * --------------------
 */

/*===========================================================================

FUNCTION DEMOD_DOWNLOAD_IMAGE

DESCRIPTION
  This function downloads a complete demod image into the Demod memory

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void demod_download_image(void);

/*===========================================================================

FUNCTION DEMOD_CHECK_DOWNLOAD_STATUS

DESCRIPTION
  This function checks and returns the current demod download status to a calling 
  routine.
  
 
DEPENDENCIES
  None.

RETURN VALUE
  demod_download_status - the current download status of the demod.

SIDE EFFECTS
  None.
  
===========================================================================*/
demod_download_status_type demod_check_download_status(void);


#endif /* DMDDOWN_H */
/* End of File. */
