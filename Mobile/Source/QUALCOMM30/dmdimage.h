
#ifndef DMDIMAGE_H
#define DMDIMAGE_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    M S M 5 0 0 0  D E M O D U L A T O R  D O W N L O A D  S E R V I C E S
                           H E A D E R  F I L E

DESCRIPTION
  This file contains declaration for MSM5000 demodulator download image.

REFERENCES

Copyright (c) 1995,1996,1997,1998,1999 by QUALCOMM, Incorporated.  
All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/dmdimage.h_v   1.2   24 Aug 2000 18:08:28   robertom  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/17/00    rmd    Added support for MSM5000A and MSM5000(commercial) demod firmware
1/18/00     eh     Update Header



===========================================================================*/


/*===========================================================================

                     INCLUDE FILE FOR THIS HEADER FILE

===========================================================================*/


/* TYPEDEFS, ENUMS & STRUCTS. 
 * --------------------------
 */


/* GLOBAL VARIABLES. 
 * -----------------
 */

extern const demod_download_data demod_image;

#ifdef T_MSM5000A_SUPPORT
extern const demod_download_data demod_image_MSM5000A;
#endif /*T_MSM5000A_SUPPORT*/

/* FUNCTION PROTOTYPES. 
 * --------------------
 */


#endif /* DMDIMAGE_H */
/* End of File. */
