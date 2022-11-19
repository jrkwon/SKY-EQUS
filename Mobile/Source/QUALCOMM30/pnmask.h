#ifndef PNMASK_H
#define PNMASK_H
/*===========================================================================

            S H O R T    P I L O T    P N    S H I F T    M A S K S

DESCRIPTION
  This file contains declarations for the shift mask tables for the
  I and Q short PN sequences.

Copyright(c) 1991,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright(c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright(c) 1999,2000      by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/shared/srch/vcs/pnmask.h_v   1.1   07 Aug 2000 17:26:30   dyang  $

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/06/99   ejv     ajn: Mainlined T_MSM3 support.  Renamed the I/Q mask tables.
01/25/99   ejv     Added FEATURE_NO_VBB around comments for clarity.
01/13/99   ejv     Removed #ifndef T_MSM3 code.
01/06/99   ejv     Removed _cdecl - not necessary for ARM.
10/16/98   ejv     Merged changes to support Panther.
09/28/98   ejv     Added _cdecl to force C calling for Pascal builds.
02/28/96   dna     Changed external references to PN tables for inclusion in
                   the Boot Block.  Removed old PN talbes.
09/20/95   rdh     Made PN mask autoswitching a #ifdef compile time option.
11/02/94   jah     ROM'd PN mask tables.
10/24/94   rdh     Mainlined changes for PN mask autoswitching.
08/30/94   rdh     New PN masks from You-chen Jou.
05/17/91   rdb     Created file based on table information provided by
                   You-chen Jou and processed by Do Nguyen.

===========================================================================*/

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*
** The I-MASK and Q-MASK tables
*/
extern word pn_i_masks[];
extern word pn_q_masks[];
     
#endif /* PNMASK_H */
