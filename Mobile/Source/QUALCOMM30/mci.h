#ifndef MCI_H
#define MCI_H
/*===========================================================================

          M A I N   C O N T R O L   T A S K    H E A D E R  

DESCRIPTION
  This file contains global declarations and external references
  required by internal MC units and subtasks.

Copyright (c) 1990,1991,1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1995,1996,1997 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1998           by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

   This section contains comments describing changes made to this file.
   Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mci.h_v   1.1   16 Feb 2001 12:56:26   fchan  $
   
when       who     what, where, why
--------   ---     -------------------------------------------------------
02/16/01   fc      Added support for system determination 2.0. All changes 
                   are featurized under FEATURE_SD20.
05/12/99   kmp     Changed T_AUTH to FEATURE_AUTH
09/04/98   pms     Neared the variables to save ROM space.
08/25/98   ck      Externalized mc_dh_q
01/29/98   sk      Added nv_get_item, nv_put_item, mcid_chk as externs.
02/15/96   dna     Included target.h to provide T_AUTH define for extern.
06/07/95   dna     Added External Authentication Task Report Queue
07/09/92   ip      Initial release.

===========================================================================*/

#include "queue.h"
#include "target.h"

/* Is vendor id validation supported ? */
#ifdef FEATURE_ID
#error code not present
#endif



/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/* The MC command queue is declared external here so that the MCC and      */
/* ACPMC subtasks can access it to get commands arriving to it.  The       */
/* report timer is regional so that MC and MCSYS can use it.               */

extern  q_type mc_cmd_q;
extern  rex_timer_type    mc_rpt_timer;

#ifdef FEATURE_AUTH
/* The auth report queue is used by MCC and ACPMC.                         */
extern  q_type mc_auth_q;
#endif /* FEATURE_AUTH */

#if (defined (FEATURE_DH) && defined (FEATURE_DH_EXP))
/* The dh report queue is used by MCC.                                     */
extern  q_type            mc_dh_q;
#endif /* FEATURE_DH and FEATURE_DH_EXP*/

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_ID
#error code not present
#endif /* FEATURE_ID */

#endif /* MCI_H */

