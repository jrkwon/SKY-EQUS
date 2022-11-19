#ifndef DOG_H
#define DOG_H
/*===========================================================================

            W A T C H D O G    T A S K    H E A D E R    F I L E

DESCRIPTION
  This file contains external procedure declarations necessary for
  accessing procedures contained within the watchdog task file.

Copyright (c) 1990, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1995, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/dog.h_v   1.3   01 Feb 2001 08:48:30   kishores  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/20/01   ks      Added DOG_TDSO_RPT & DOG_TDSO_RPT_TIME for TDSO task.
09/20/99   vl      Changed DOG_VS_TASK to DOG_VS_RPT.
08/10/99   kk      Added support for VR task. Merge from branch.
08/09/99   rp      Added support for EFS task.
08/05/99   ak      Added support for sockets udp test app.
07/28/99   kk      Pink Panther merge. Support for CM added.
03/30/99   ak      Added support for sockets test app.
07/23/98   kss     Added support for DH, QMIP, and VR tasks.  Now supports
                   up to 32 tasks (uses 32-bit report).
01/17/98   dhh     Added condition for IDS support.
01/15/98   dhh     Added support for IDS task. 
10/31/95   ras     added #if's for initial CSS build from ref. "spooky"
07/13/95   jjw     Added IS-99 PS task definitions
06/07/95   jah     Changed DOG_CHECK_TIME to 59 seconds to allow low priority
                   tasks to be blocked to near the failure check limit spec'd
                   in IS-95 (2.5.1).
06/07/95   dna     Added Authentication Task Definitions
03/21/94   jah     Added prototype for dog_autokick()
02/02/94   jah     Added DOG_DS_RPT_TIME
11/30/93   jah     Converted from REX signals to calls to dog_report().
08/16/93   jah     Added DOG_SLEEP_RPT_TIME
04/27/93   jah     Externalized dog_kick().
03/25/93   jah     Changed DOG_CHECK_TIME to 3 seconds from 1.
03/11/93   jah     Added DOG_SLEEP_RPT_SIG
03/11/92   jah     Ported from brassboard, added primes

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc. */
#include "clk.h"        /* Definitions for CLK_MS_PER_TICK  */
#include "task.h"       /* Task definitions                 */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* This signal is set when the dog_wakeup_timer expires.
*/
#define DOG_WAKEUP_TIMER_SIG    0x0800

/* Reserved signals
*/
#define DOG_RSVD_0400_SIG       0x0400
#define DOG_RSVD_0200_SIG       0x0200
#define DOG_RSVD_0100_SIG       0x0100
#define DOG_RSVD_0080_SIG       0x0080
#define DOG_RSVD_0040_SIG       0x0040
#define DOG_RSVD_0020_SIG       0x0020
#define DOG_RSVD_0010_SIG       0x0010
#define DOG_RSVD_0008_SIG       0x0008
#define DOG_RSVD_0004_SIG       0x0004
#define DOG_RSVD_0002_SIG       0x0002
#define DOG_RSVD_0001_SIG       0x0001

/*------------------------------------------------------------------------*/

/* Data type for watchdog reports
*/
typedef unsigned long dog_report_type;

/* The following semaphores are periodically set by the other software tasks
** in order to prove that they are executing regularly.
*/

#define DOG_UNUSED_1_RPT    0x80000000UL  /* Unused                     */
#define DOG_UNUSED_2_RPT    0x40000000UL  /* Unused                     */
#define DOG_UNUSED_3_RPT    0x20000000UL  /* Unused                     */
#define DOG_UNUSED_4_RPT    0x10000000UL  /* Unused                     */
//#define DOG_UNUSED_5_RPT    0x08000000UL  /* Unused                     */
#define DOG_TDSO_RPT        0x08000000UL  /* TDSO Task                  */

//#ifdef FEATURE_VAS
#define DOG_VAS_RPT         0x04000000UL  /* VAS        Task            */
//#endif//feature_vas

#ifdef	FEATURE_SKT_SOCKAPI	//	khekim 00/04/06
#define DOG_SOCK_RPT	    0x02000000UL  /* Unused                     */
#else	//	#ifdef	FEATURE_SKT_SOCKAPI
#define DOG_UNUSED_7_RPT    0x02000000UL  /* Unused                     */
#endif	//	#ifdef	FEATURE_SKT_SOCKAPI
#ifdef FEATURE_EMAIL
#define DOG_EML_RPT         0x01000000UL  /*email app */
#else
#define DOG_UNUSED_8_RPT    0x01000000UL  /* Unused                     */
#endif//FEATURE_EMAIL
#define DOG_FS_RPT          0x00800000UL  /* EFS Task                   */
#define DOG_UDPTEST_RPT     0x00400000UL  /* UDP test task              */

#ifdef FEATURE_CM
#define DOG_CM_RPT          0x00100000UL  /* Call Manager               */
#endif

#define DOG_TCPTEST_RPT     0x00080000UL  /* TCP test task              */
#define DOG_DH_RPT          0x00040000UL  /* Diffie-Helman Task         */
#define DOG_VS_RPT          0x00020000UL  /* Voice Services Task        */
#define DOG_QMIP_RPT        0x00010000UL  /* QMIP Task                  */
#define DOG_IDS_RPT         0x00008000UL  /* IDS Task                   */
#define DOG_PD_RPT          0x00004000UL  /* Protocol Distribution Task */
#define DOG_PS_RPT          0x00002000UL  /* Data Protocol Services     */
#define DOG_AUTH_RPT        0x00001000UL  /* Authentication Task        */
#define DOG_DS_RPT          0x00000800UL  /* Data Services Task         */
#define DOG_DIAG_RPT        0x00000400UL  /* Diagnostic Task            */
#define DOG_HS_RPT          0x00000200UL  /* Handset Task               */
#define DOG_MC_RPT          0x00000100UL  /* Main Control Task          */
#define DOG_NV_RPT          0x00000080UL  /* Non-volatile Database Task */
#define DOG_RX_RPT          0x00000040UL  /* Receive Task               */
#define DOG_RXTX_RPT        0x00000020UL  /* Layer 2 task               */
#define DOG_SND_RPT         0x00000010UL  /* Sound Task                 */
#define DOG_SRCH_RPT        0x00000008UL  /* Searcher Task              */
#define DOG_TX_RPT          0x00000004UL  /* Transmit Task              */
#define DOG_UI_RPT          0x00000002UL  /* User Interface Task        */
#define DOG_SLEEP_RPT       0x00000001UL  /* Sleep Task                 */

/*------------------------------------------------------------------------*/

/* Convert 'dog time', which is clock intervals, to REX ticks,
** which are milliseconds.
*/
#define DOG_TO_REX_TIME(t)      ((t) * CLK_MS_PER_TICK)

/* This specifies, in milliseconds, the time interval in which the monitored
** tasks must report to the Watchdog Task.  If the tasks exceed this period,
** the Watchdog Task logs an error and causes a system reset.
**
** Note: the maximum allowed grace period is 60 seconds, as specified by
** TIA/EIA/IS-95, section 2.5.1 (Malfunction timer).  The times used here
** are of a much shorter duration.
*/
#if MODEL_ID == MODEL_B2
#define DOG_CHECK_TIME        100000	// nv초기화시 부팅에서 dog가 발생 01.10.15 by jnlee
#else
#define DOG_CHECK_TIME         59000
#endif

/* Some report times which overlap infrequently due to being prime.  The
** purpose of not overlapping is to minimize the number of tasks which
** become runnable at the same time due to their report timers expiring.
** Keeping this event infrequent helps to maintain minimum task execution
** delays.  Some randomization (1-2 clock intervals) as a result of the
** tasks starting sequentially.
**
** Note that for a safety margin, none of the primes selected for report
** times should be greater than approximately DOG_CHECK_TIME / 1.5
** Next primes are 181, 191, 193, 197, 199, 211, 223, 
** 227, 229, 233, 239, 241, and 251.
*/

//#ifdef FEATURE_VAS
#define DOG_VAS_RPT_TIME        DOG_TO_REX_TIME( 211)
//#endif //FEATURE_VAS

#define DOG_FS_RPT_TIME         DOG_TO_REX_TIME( 193 )
#define DOG_UDPTEST_RPT_TIME    DOG_TO_REX_TIME( 191 )
#ifdef FEATURE_CM
#define DOG_CM_RPT_TIME         DOG_TO_REX_TIME( 989 ) //HDTEMPO was 181
#endif

#ifdef FEATURE_EMAIL
#define DOG_EML_RPT_TIME		DOG_TO_REX_TIME( 197 )
#endif //#ifdef FEATURE_EMAIL2

#ifdef FEATURE_SKT_SOCKAPI	//	khekim 00/04/06
#define DOG_SOCK_RPT_TIME		DOG_TO_REX_TIME( 193 )
#endif //#ifdef FEATURE_SKT_SOCKAPI

#define DOG_TCPTEST_RPT_TIME    DOG_TO_REX_TIME( 181 )
#define DOG_DH_RPT_TIME         DOG_TO_REX_TIME( 179 )
#define DOG_VS_RPT_TIME         DOG_TO_REX_TIME( 173 )
#define DOG_QMIP_RPT_TIME       DOG_TO_REX_TIME( 167 )
#define DOG_IDS_RPT_TIME        DOG_TO_REX_TIME( 163 )
#define DOG_PD_RPT_TIME         DOG_TO_REX_TIME( 157 )
#define DOG_PS_RPT_TIME         DOG_TO_REX_TIME( 151 )
#define DOG_AUTH_RPT_TIME       DOG_TO_REX_TIME( 149 )
#define DOG_DS_RPT_TIME         DOG_TO_REX_TIME( 139 )
#define DOG_DIAG_RPT_TIME       DOG_TO_REX_TIME( 137 )
#define DOG_HS_RPT_TIME         DOG_TO_REX_TIME( 131 )
#define DOG_MC_RPT_TIME         DOG_TO_REX_TIME( 127 )
#define DOG_NV_RPT_TIME         DOG_TO_REX_TIME( 113 )
#define DOG_RX_RPT_TIME         DOG_TO_REX_TIME( 109 )
#define DOG_RXTX_RPT_TIME       DOG_TO_REX_TIME( 107 )
#define DOG_SND_RPT_TIME        DOG_TO_REX_TIME( 103 )
#define DOG_SRCH_RPT_TIME       DOG_TO_REX_TIME( 101 )
#define DOG_TX_RPT_TIME         DOG_TO_REX_TIME(  97 )
#define DOG_UI_RPT_TIME         DOG_TO_REX_TIME( 981 )  // HDTEMPO was 89
#define DOG_SLEEP_RPT_TIME      DOG_TO_REX_TIME(  83 )
#define DOG_TDSO_RPT_TIME       DOG_TO_REX_TIME( 197 )

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION DOG_REPORT

DESCRIPTION
  Report to the watchdog task, so that it can tell that the monitored
  task is still functioning properly.

DEPENDENCIES
  None

RETURN VALUE
  None  

SIDE EFFECTS
  None

===========================================================================*/
extern void dog_report
(
  dog_report_type report
    /* Tell the watchdog task which task is reporting */
);


/*===========================================================================

FUNCTION DOG_AUTOKICK

DESCRIPTION
  This procedure enables/disables the auto-kick feature.  Auto-kick
  is used when the microprocessor is expected to be asleep.

DEPENDENCIES
  This should only be called when the sleep timer has been programmed
  or when the processor wakes up after the sleep timer expires.

RETURN VALUE
  None  

SIDE EFFECTS
  None

===========================================================================*/
extern void dog_autokick
(
  boolean enable
);


/*===========================================================================

FUNCTION DOG_KICK

DESCRIPTION
  This procedure resets the watchdog timer circuit.  This is
  not to be used except in unusual start-up/shutdown conditions.

DEPENDENCIES
  None

RETURN VALUE
  None  

SIDE EFFECTS
  Resets the circuit so that we have another N milliseconds before
  the circuit will reset the system.

===========================================================================*/
extern void dog_kick( void );


/*===========================================================================

FUNCTION DOG_TASK 

DESCRIPTION
  This procedure contains the main processing loops for the watchdog task.
  It checks every DOG_CHECK_TIME milliseconds to see if every other software
  task has set its report signal.  If a task has not set its signal, dog_task
  calls the ERR_FATAL macro to log the error and reset the microprocessor.

DEPENDENCIES
  This procedure should be called by rex_def_task.  Also, each task
  must set its report signal at least once every DOG_CHECK_TIME milliseconds.

RETURN VALUE
  None, this procedure does not return.

SIDE EFFECTS
  The microprocessor may be reset during this task.

===========================================================================*/
extern void dog_task
(
  dword param
    /* The least significant rex_sigs_type portion of this dword is
    ** a mask of task report signals to be monitored.
    */
);

#endif  /* DOG_H */
