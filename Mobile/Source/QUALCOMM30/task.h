#ifndef TASK_H
#define TASK_H
/*===========================================================================

            T A S K    R E S O U R C E S    H E A D E R    F I L E

DESCRIPTION
  This file contains declarations defining global task resources.

Copyright (c) 1990, 1991, 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1994, 1995 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1998, 1999 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 2000, 2001 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/task.h_v   1.6   01 May 2001 19:18:42   rajeevg  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/30/01   gr      Added NV_PRI_HI, SRCH_PRI_MD and SLEEP_PRI_HI.
02/15/00   ro      Added VS_OP_COMPLETE_SIG.
01/20/01   ks      Added support for TDSO task.
06/05/00   ck      Changed the feature defines around Authentication task  such
                   that it gets defined if FEATURE_AUTH or FEATURE_RUIM is
                   defined.
10/08/99   lcc     Merged in RPC support from PLT archive.
08/09/99   rp      Added support for EFS task based on FEATURE_EFS.
08/05/99   ak      Added hooks for udp test task and fixed #defines for
                   sockets test tasks.
04/28/99   ms/sk   Added size declaration for Error Fatal Stack.
04/02/99    ms     Reduced stack sizes.
03/30/99    ak     Added hooks for sockets test task.
03/18/99    ms     Changed declaration of irq_stack to byte [] from byte *.
03/04/98   hcg     Changed generic browser feature to FEATURE_DS_BROWSER_INTERFACE.
02/05/99    sk     Replaced int_stack with irq_stack.
02/04/99    ms     Support for FEATURE_ENHANCED_STANDBY_II in Searcher.
12/16/98   jct     Removed 80186 support and unused tasks
11/13/98   dhh     Lowered IDS priority to 280 from 300.
10/08/98   jct     Merged in ARM stack size changes with current task defs
09/24/98   ck      Increased the stack size for DH task to 400 words.
08/25/98   ck      Added DH task.
08/13/98   ces     Add QMIP task.
06/08/98   ldg     Swapped PS task priority below DS.
05/27/98   na      Increased the PS stack size from 500 words to 800 words.
04/14/98   ldg     Moved data task priorities to below all but SLEEP.
02/02/98   smo     Added new SRCH_PRI_LO value for searcher priority switch.
01/27/98   ldg     Removed the DS_SLIM switch.
01/27/98   ldg     FEATURE_xxx updates by hand.
01/17/98   dhh     Added condition to include IDS support.
01/16/98   dhh     Added support for IDS task.
12/23/96   JGR     Add support for Unwired Planet with ifdef FEATURE_DS_UP
05/24/96   jjw     Gemini & ISS2 Data Services release.
02/01/96   rdh     Added MC_PRI_HI and SRCH_PRI_HI for idle sleep speedup.
10/31/95   ras     added #if's for initial CSS build from ref. "spooky"
08/22/95   jah     Reduced the Sleep and Handset tasks to 300 & 400 words.
07/28/95   day     Returned MC_STACK_SIZ to 500.
07/26/95   jah     Changed MC_STACK_SIZ from 500 to 600 for SMS.
06/09/95   jca     Removed TASK_RSVD_1000_SIG common signal definition.
06/07/95   jah     Moved NV to priority 80
06/07/95   dna     Added Authentication Task Definitions
10/16/94   jjw     Added Data Protocol Task Definitions (compile-time 
                   inclusion)
04/13/94   jjw     Added Data Task Definitions (compile-time inclusion)
03/10/94   jah     Changed #if (T_SLEEP!=0) to #ifdef T_SLEEP
12/09/93   jah     Made sleep task conditional on T_SLEEP.
03/11/93   ptw     Added definitions for sleep task.
07/22/92   ip      Added includes for comdef.h and rex.h.
04/30/92   ip      Removed all task specific declarations.  This file now
                   holds only stack definitions, TCB definitions, priorities
                   definitions, and common (global) signals.
04/23/92   ip      Initial porting of file from Brassboard to DMSS.

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "rex.h"
#include "customer.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------

                     COMMON SIGNALS DEFINITIONS

---------------------------------------------------------------------------*/

#define  TASK_START_SIG       0x8000
#define  TASK_STOP_SIG        0x4000
#define  TASK_OFFLINE_SIG     0x2000

#if defined FEATURE_EFS
#define  FS_OP_COMPLETE_SIG   0x40000000
#endif

#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

/*---------------------------------------------------------------------------

                    INTERRUPT STACK DECLARATIONS

---------------------------------------------------------------------------*/

extern byte  irq_stack[ ];

/*---------------------------------------------------------------------------

                    ERROR FATAL STACK DECLARATION

---------------------------------------------------------------------------*/

#define Err_Fatal_Stack_Size   512

/*---------------------------------------------------------------------------

                    WATCHDOG TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  DOG_PRI       1000   /* REX priority for the Watchdog Task        */

#define  DOG_STACK_SIZ  (186*2)   /* Size (words) of Watchdog Task stack       */

extern word            dog_stack[ ];
extern rex_tcb_type    dog_tcb;

/*---------------------------------------------------------------------------

                     SOUND TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  SND_PRI        900   /* REX priority for the Sound Task           */
#define  SND_STACK_SIZ  604    /* Size (words) of Sound Task stack          */

extern word            snd_stack[ ];
extern rex_tcb_type    snd_tcb;

/*---------------------------------------------------------------------------

                NON-VOLATILE DATABASE TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  NV_PRI          80   /* REX priority for the Non-volatile Task    */
#define  NV_PRI_HI      (SRCH_PRI + 3) /* NV priority for OTASP commit     */

#define  NV_STACK_SIZ   (1024*2)   /* Size (words) of Non-volatile Task stack   */
extern word            nv_stack[ ];
extern rex_tcb_type    nv_tcb;

/*---------------------------------------------------------------------------

                EMBEDDED FILE SYSTEM TASK DECLARATIONS

---------------------------------------------------------------------------*/

#ifdef FEATURE_EFS
#define  FS_PRI          78   /* REX priority for the File System Task     */
#define  FS_STACK_SIZ   (1024*2)  /* Size (words) of File System Task stack    */
extern word            fs_stack[ ];
extern rex_tcb_type    fs_tcb;
#endif

/*---------------------------------------------------------------------------

                      TRANSMIT TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  TX_PRI         700   /* REX priority for the Transmit Task        */
#define  TX_STACK_SIZ   (504*2)    /* Size (words) of Transmit Task stack       */
extern rex_tcb_type    tx_tcb;
extern word            tx_stack[ ];


/*---------------------------------------------------------------------------

                       RECEIVE TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  RX_PRI         600   /* REX priority for the Receive Task         */
#define  RX_STACK_SIZ   (572*2)    /* Size (words) of Receive Task stack        */
extern word            rx_stack[ ];
extern rex_tcb_type    rx_tcb;


/*---------------------------------------------------------------------------

                       SEARCHER TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  SRCH_PRI       500      /* REX priority for the Searcher Task     */

#if defined FEATURE_ENHANCED_STANDBY_II
#define  SRCH_PRI_MAX (DOG_PRI-2)/* SRCH priority for near instant response*/
#endif

#define  SRCH_PRI_HI (RX_PRI+1)  /* SRCH priority for fast MC response     */
#define  SRCH_PRI_LO (UI_PRI-1)  /* SRCH priority for unthrottled ACQ srch */
#define  SRCH_PRI_MD  (MC_PRI-2) /* SRCH priority for 8X mode              */

#define  SRCH_STACK_SIZ  (706*2)     /* Size (words) of Searcher Task stack    */

extern word            srch_stack[ ];
extern rex_tcb_type    srch_tcb;

/*---------------------------------------------------------------------------

                   LAYER 2 (RXTX) TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  RXTX_PRI       450   /* REX priority for the RXTX Task            */
#define  RXTX_STACK_SIZ (348*2)   /* Size (words) of RXTX Task stack           */

extern word            rxtx_stack[ ];
extern rex_tcb_type    rxtx_tcb;


/*---------------------------------------------------------------------------

                   MAIN CONTROL TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  MC_PRI         400   /* REX priority for the Main Control Task    */
#define  MC_PRI_HI  (RX_PRI+2)   /* MC priority for fast RX responsiveness */
#define  MC_PRI_TC  (SRCH_PRI+1) /* MC priority while on TC to improve
                                    Hard Handoff performance */

#define  MC_STACK_SIZ   (922*2)   /* Size (words) of Main Control Task stack   */

extern word            mc_stack[ ];
extern rex_tcb_type    mc_tcb;



#ifdef FEATURE_DIAG_RPC
#error code not present
#endif /* #ifdef FEATURE_DIAG_RPC */

/*---------------------------------------------------------------------------

                      HANDSET TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  HS_PRI         300   /* REX priority for the Handset Task         */
//#define  HS_STACK_SIZ   372   /* Size (words) of Handset Task stack        */
#define  HS_STACK_SIZ   (512*2)   /* Size (words) of Handset Task stack        */

extern word            hs_stack[ ];
extern rex_tcb_type    hs_tcb;

#ifdef FEATURE_QMIP
#error code not present
#endif /* FEATURE_QMIP */


/*---------------------------------------------------------------------------

                     DIAGNOSTIC TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  DIAG_PRI       200   /* REX priority for the Diagnostic Task      */
#define  DIAG_STACK_SIZ (368*2)   /* Size (words) of Diagnostic Task stack     */
extern word            diag_stack[ ];
extern rex_tcb_type    diag_tcb;


/*---------------------------------------------------------------------------

                     CALL MANAGER TASK DECLARATIONS

---------------------------------------------------------------------------*/
#ifdef FEATURE_CM
#define  CM_PRI         150    /* REX priority for the Diagnostic Task      */
#define  CM_STACK_SIZ   1024   /* Size (words) of Diagnostic Task stack     */
extern word            cm_stack[ ];
extern rex_tcb_type    cm_tcb;
#endif /* FEATURE_CM */

/*---------------------------------------------------------------------------

                    USER INTERFACE TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  UI_PRI         100   /* REX priority for the User Interface task  */

#ifdef FEATURE_DS_BROWSER_INTERFACE
#define  UI_STACK_SIZ   (4096*2)   /* Size (words) of User Interface Task stack */
#else
//ytchoi stack too small 
//#define  UI_STACK_SIZ   (1024*2)   /* Size (words) of User Interface Task stack */
#define  UI_STACK_SIZ   (4096*2)   /* Size (words) of User Interface Task stack */
#endif
extern word            ui_stack[ ];
extern rex_tcb_type    ui_tcb;


#ifdef T_SLEEP
/*---------------------------------------------------------------------------

                       SLEEP TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  SLEEP_PRI        50   /* REX priority for the Sleep task          */
#define  SLEEP_PRI_HI     (NV_PRI - 3)
  /* Level to which searcher will bump Sleep task when drastically starved
  ** of CPU. This is just above AUTH, PS, DS,..etc, tasks which are possibly
  ** CPU-bound (and also are not always defined).                          */
#define  SLEEP_STACK_SIZ (206*2)   /* Size (words) of Sleep Task stack         */

extern word            sleep_stack[ ];
extern rex_tcb_type    sleep_tcb;

#endif /* T_SLEEP */


#ifdef FEATURE_DS

/*---------------------------------------------------------------------------

                    DATA SERVICES TASK DECLARATIONS

---------------------------------------------------------------------------*/


#define  DS_PRI          70   /* REX priority for the Datat Services task  */
#define  DS_STACK_SIZ  1024   /* Size (words) of Data Task stack           */

extern word            ds_stack[ ];
extern rex_tcb_type    ds_tcb;

/*---------------------------------------------------------------------------

                    DATA PROTOCOL TASK DECLARATIONS

---------------------------------------------------------------------------*/


#define  PS_PRI          60   /* REX priority for the Protocol Srvcs task  */
#define  PS_STACK_SIZ  1104   /* Size (words) of Protocl Srvcs Task stack  */
extern word            ps_stack[ ];
extern rex_tcb_type    ps_tcb;

#endif /* FEATURE_DS */


//#ifdef FEATURE_VAS
/*---------------------------------------------------------------------------

                    VAS App. TASK DECLARATIONS

---------------------------------------------------------------------------*/


//#define  VAS_PRI       87      /* REX priority for the vas app. task  */
//#define  VAS_STACK_SIZ  (4000)   /* Size (words) of Data Task stack           */

//extern word            vas_stack[ ];
//extern rex_tcb_type    vas_tcb;
//#endif//FEATURE_VAS

#ifdef FEATURE_EMAIL
/*---------------------------------------------------------------------------

                    Email App. TASK DECLARATIONS

---------------------------------------------------------------------------*/


#define  EML_PRI         82    /* REX priority for the Email app. task  */
#define  EML_STACK_SIZ  (2048)   /* Size (words) of Data Task stack           */

extern word            eml_stack[ ];
extern rex_tcb_type    eml_tcb;
#endif //#ifdef FEATURE_EMAIL

#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/06
/*---------------------------------------------------------------------------

                    SOCK App. TASK DECLARATIONS

---------------------------------------------------------------------------*/
//jmpark conflict with EFS sys and should be higher than EFS.
#define  SOCK_PRI         85  /* REX priority for the Email app. task  */
#define  SOCK_STACK_SIZ  (512+612)   /* Size (words) of Data Task stack           */

extern word            sock_stack[ ];
extern rex_tcb_type    sock_tcb;
#endif	//	#ifdef	FEATURE_SKT_SOCKAPI


#ifdef FEATURE_DS_SOCKETS_TEST_TCP
/*---------------------------------------------------------------------------

                     SOCKETS TCP TEST APP TASK DECLARATION

---------------------------------------------------------------------------*/


#define  TCPTEST_PRI          55 /* REX priority for the TCP test app task */
#define  TCPTEST_STACK_SIZ  1600 /* Size (words) of TCP test app stack     */
extern word            tcptest_stack[ ];
extern rex_tcb_type    tcptest_tcb;
#endif

#ifdef FEATURE_DS_SOCKETS_TEST_UDP
/*---------------------------------------------------------------------------

                     SOCKETS UDP TEST APP TASK DECLARATION

---------------------------------------------------------------------------*/
#define UDPTEST_PRI           52  /* REX priority for the UDP test app task */
#define UDPTEST_STACK_SIZ   1600  /* Size of UDP test app stack */
extern  word        udptest_stack[];
extern  rex_tcb_type udptest_tcb;
#endif



#if defined (FEATURE_AUTH) || defined (FEATURE_RUIM)
/*---------------------------------------------------------------------------

                    AUTHENTICATION TASK DECLARATIONS

---------------------------------------------------------------------------*/


#define  AUTH_PRI        75   /* REX priority for the Authentication task  */
#define  AUTH_STACK_SIZ 1024  /* Size (words) of Authentication Task stack */
extern word            auth_stack[ ];
extern rex_tcb_type    auth_tcb;

#endif /* FEATURE_AUTH or FEATURE_RUIM*/

#ifdef FEATURE_DH
/*---------------------------------------------------------------------------

                         DH TASK DECLARATIONS

---------------------------------------------------------------------------*/


#define  DH_PRI         62   /* REX priority for the DH task  */
#define  DH_STACK_SIZ  1024  /* Size (words) of DH Task stack */

extern word            dh_stack[ ];
extern rex_tcb_type    dh_tcb;

#endif /* FEATURE_DH */

#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

#ifdef FEATURE_IS2000_TDSO
/*---------------------------------------------------------------------------

                     TDSO TASK DECLARATIONS

---------------------------------------------------------------------------*/

#define  TDSO_PRI          250   /* REX priority for the VS Task              */
#define  TDSO_STACK_SIZ   512   /* Size (words) of VS Task stack             */

extern word            tdso_stack[ ];
extern rex_tcb_type    tdso_tcb;

#endif /* FEATURE_IS2000_TDSO */
#endif /* TASK_H */

