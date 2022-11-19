/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
            M E M O R Y  T E S T
      
GENERAL DESCRIPTION
   This file contains routines to test Internal RAM and vocoder instruction
   RAM

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 1991 through 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================
 
    EDIT HISTORY FOR MODULE

$log$

when       who    what, where, why
--------   ---    ----------------------------------------------------------
09/06/00   sr     Initial revision. 
===========================================================================*/

#ifndef RELOC_H
#define RELOC_H


#define DISPLAY *((volatile unsigned int *)(0x2800000))
extern int iram_reloc_code(unsigned long int base, unsigned int lg);

#endif

