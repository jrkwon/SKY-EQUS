/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 T A S K    R E S O U R C E    F I L E

GENERAL DESCRIPTION
  This file contains declarations for task resources, and the task_start
  function.

EXTERNALIZED FUNCTIONS
  task_start
    This function performs task startup processing.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 1990, 1991, 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1998, 1999 by QUALCOMM Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/task.c_v   1.4   01 Feb 2001 08:47:08   kishores  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------   
01/20/01   ks      Added support for TDSO task.
06/05/00   ck      Changed the feature defines around Authentication task  such
                   that it gets defined if FEATURE_AUTH or FEATURE_RUIM is
                   defined.
10/08/99   lcc     Merged in RPC support from PLT archive.
08/17/99   tac     Merged in support for VS and EFS tasks from branch.
08/09/99   rp      Added support for EFS task based on FEATURE_EFS.
08/05/99   ak      Added udp test task, and fixed #defines for sockets
                   test tasks.
06/17/99   sh      modified comments for CM task
04/28/99   ms/sk   Added declaration for Error Fatal Stack.
03/30/99    ak     Added hooks for sockets test app.
02/05/99    sk     Removed int_stack declaration.
12/16/98   jct     Removed 80186 support and unsued tasks
08/25/98   ck      Added DH task.
08/13/98   ces     Add QMIP task.
01/27/98   ldg     Removed the DS_SLIM switch.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
01/17/98   dhh     Added condition to include IDS support.
01/16/98   dhh     Added support for IDS task. 
05/24/96   jjw     Gemini & ISS2 Data Services release.
10/31/95   ras     added #if's for initial CSS build from ref. "spooky"
07/14/95   jjw     IS-99 integration clean-up
06/13/95   rdh     Made T_SLEEP handling explicit and consistent DMSS-wide.
06/07/95   dna     Added Authentication Task Definitions
10/10/94   jjw     Added conditionally-compiled Data Protocol Task
02/02/94   jjw     Added conditionally-compiled Data Task
12/08/93   jah     Made inclusion of the sleep task conditional
03/11/93   ptw     Added sleep task.
07/09/92    ip     Initial release to PVCS.
04/30/92    ip     First porting of file from Brassboard to DMSS.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "rex.h"
#include "task.h"
#include "mc.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*---------------------------------------------------------------------------

                    STACK FOR ERROR FATAL PROCESSING

ERR_FATAL can use this stack in worst case.

---------------------------------------------------------------------------*/

byte err_fatal_stack[Err_Fatal_Stack_Size];

/*---------------------------------------------------------------------------

                    WATCHDOG TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            dog_stack [DOG_STACK_SIZ];   /* Stack for DOG task.        */
rex_tcb_type    dog_tcb;                     /* TCB for the DOG task.      */

/*---------------------------------------------------------------------------

                     SOUND TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            snd_stack [SND_STACK_SIZ];   /* Stack for SND task.        */
rex_tcb_type    snd_tcb;                     /* TCB for SND task.          */

/*---------------------------------------------------------------------------

                NON-VOLATILE DATABASE TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            nv_stack [NV_STACK_SIZ];     /* Stack for NV task.         */
rex_tcb_type    nv_tcb;                      /* TCB for NV task.           */

/*---------------------------------------------------------------------------

                EMBEDDED FILE SYSTEM TASK DECLARATIONS

---------------------------------------------------------------------------*/

#if defined FEATURE_EFS
word            fs_stack [FS_STACK_SIZ];     /* Stack for File System task.*/
rex_tcb_type    fs_tcb;                      /* TCB for File System task.  */
#endif /* FEATURE_EFS */

/*---------------------------------------------------------------------------

                      TRANSMIT TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            tx_stack [TX_STACK_SIZ];     /* Stack for TX task.         */
rex_tcb_type    tx_tcb;                      /* TCB for TX task.           */

/*---------------------------------------------------------------------------

                   RECEIVE TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            rx_stack [RX_STACK_SIZ];     /* Stack for RX task.         */
rex_tcb_type    rx_tcb;                      /* TCB for RX task.           */

/*---------------------------------------------------------------------------

                   SEARCHER TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            srch_stack [SRCH_STACK_SIZ]; /* Stack for SRCH task.       */
rex_tcb_type    srch_tcb;                    /* TCB for SRCH task.         */

/*---------------------------------------------------------------------------

                   LAYER2(RXTX) TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            rxtx_stack [RXTX_STACK_SIZ]; /* Stack for RXTX task.       */
rex_tcb_type    rxtx_tcb;                    /* TCB for RXTX task.         */

/*---------------------------------------------------------------------------

                   MAIN CONTROL TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            mc_stack [MC_STACK_SIZ];     /* Stack for MC task.         */
rex_tcb_type    mc_tcb;                      /* TCB for MC task.           */

#ifdef FEATURE_DIAG_RPC
#error code not present
#endif /* #ifdef FEATURE_DIAG_RPC */

/*---------------------------------------------------------------------------

                     HANDSET TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            hs_stack [HS_STACK_SIZ];     /* Stack for HS task.         */
rex_tcb_type    hs_tcb;                      /* TCB for the HS task.       */

#ifdef FEATURE_IDS
#error code not present
#endif

/*---------------------------------------------------------------------------

                     DIAGNOSTIC TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            diag_stack [DIAG_STACK_SIZ]; /* Stack for DIAG Task.       */
rex_tcb_type    diag_tcb;                    /* TCB for DIAG task.         */


/*---------------------------------------------------------------------------

                     CALL MANAGER TASK DECLARATIONS

---------------------------------------------------------------------------*/

#ifdef FEATURE_CM
word            cm_stack [CM_STACK_SIZ];     /* Stack for CM Task.         */
rex_tcb_type    cm_tcb;                      /* TCB for CM task.           */
#endif /* FEATURE_CM */

/*---------------------------------------------------------------------------

                     USER INTERFACE TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            ui_stack [UI_STACK_SIZ];     /* Stack for UI Task.         */
rex_tcb_type    ui_tcb;                      /* TCB for UI task.           */


/*---------------------------------------------------------------------------

                       SLEEP TASK DECLARATIONS

---------------------------------------------------------------------------*/

#ifdef T_SLEEP
word            sleep_stack [SLEEP_STACK_SIZ];/* Stack for Sleep Task.     */
rex_tcb_type    sleep_tcb;                    /* TCB for Sleep task.       */
#endif

#ifdef FEATURE_DS

/*---------------------------------------------------------------------------

                     DATA PROTOCOL TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            ps_stack [PS_STACK_SIZ];     /* Stack for DATA Pro. Task.  */
rex_tcb_type    ps_tcb;                      /* TCB for DATA task.         */

/*---------------------------------------------------------------------------

                     DATA SERVICES TASK DECLARATIONS

---------------------------------------------------------------------------*/

word            ds_stack [DS_STACK_SIZ];     /* Stack for DATA Task.       */
rex_tcb_type    ds_tcb;                      /* TCB for DATA task.         */

#endif

/*---------------------------------------------------------------------------

                     EMAIL APPLICATION TASK DECLARATIONS

---------------------------------------------------------------------------*/
#ifdef FEATURE_EMAIL
word            eml_stack [EML_STACK_SIZ];     /* Stack for Email App. Task.       */
rex_tcb_type    eml_tcb;                      /* TCB for DATA Email App. Task.       */
#endif //#ifdef FEATURE_EMAIL

/*---------------------------------------------------------------------------

                     WAP APPLICATION TASK DECLARATIONS

---------------------------------------------------------------------------*/
//#ifdef FEATURE_VAS
//word            vas_stack [VAS_STACK_SIZ];     /* Stack for Email App. Task.       */
//rex_tcb_type    vas_tcb;                      /* TCB for DATA Email App. Task.       */
//#endif //FEATURE_VAS
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/

#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/06
/*---------------------------------------------------------------------------

                     EMAIL APPLICATION TASK DECLARATIONS

---------------------------------------------------------------------------*/
word            sock_stack [SOCK_STACK_SIZ];     /* Stack for Sock App. Task.       */
rex_tcb_type    sock_tcb;                      /* TCB for Sock App. Task.       */
/*---------------------------------------------------------------------------*/
#endif	//	#ifdef	FEATURE_SKT_SOCKAPI

#ifdef FEATURE_DS_SOCKETS_TEST_TCP
/*---------------------------------------------------------------------------

                     SOCKETS TEST TCP APP TASK DECLARATIONS

---------------------------------------------------------------------------*/

word           tcptest_stack [TCPTEST_STACK_SIZ]; /* Stack for TCP testtask*/
rex_tcb_type   tcptest_tcb;                       /* TCB for TCP test task */
#endif

#ifdef FEATURE_DS_SOCKETS_TEST_UDP
/*---------------------------------------------------------------------------

                     SOCKETS TEST UDP APP TASK DECLARATIONS

---------------------------------------------------------------------------*/

word           udptest_stack [UDPTEST_STACK_SIZ]; /* Stack for UDP test task*/
rex_tcb_type   udptest_tcb;                       /* TCB for UDP test task */
#endif



#ifdef FEATURE_QMIP
#error code not present
#endif

/*---------------------------------------------------------------------------

                     AUTHENTICATION TASK DECLARATIONS

---------------------------------------------------------------------------*/

#if defined (FEATURE_AUTH) || defined (FEATURE_RUIM)
word            auth_stack [AUTH_STACK_SIZ];   /* Stack for Auth Task.  */
rex_tcb_type    auth_tcb;                      /* TCB for Auth task.    */

#endif /* FEATURE_AUTH || FEATURE_RUIM */

/*---------------------------------------------------------------------------

                            DH TASK DECLARATIONS

---------------------------------------------------------------------------*/

#ifdef FEATURE_DH
word            dh_stack [DH_STACK_SIZ];   /* Stack for DH Task.  */
rex_tcb_type    dh_tcb;                      /* TCB for DH task.    */

#endif /*FEATURE_DH */

/*---------------------------------------------------------------------------

                     VOICE SERVICES TASK DECLARATIONS

---------------------------------------------------------------------------*/

#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

/*---------------------------------------------------------------------------

                            TDSO TASK DECLARATIONS

---------------------------------------------------------------------------*/

#ifdef FEATURE_IS2000_TDSO
word            tdso_stack [TDSO_STACK_SIZ];   /* Stack for TDSO Task.  */
rex_tcb_type    tdso_tcb;                        /* TCB for TDSO task.    */
#endif /* FEATURE_IS2000_TDSO */

