#ifndef SLEEP_H
#define SLEEP_H
/*===========================================================================

              S L E E P    T A S K   H E A D E R    F I L E

DESCRIPTION
  This file contains declarations for use by the Sleep Task.
  This module is used in the portable only.

      Copyright (c) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
                    2001
                    by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

#include "comdef.h"
#include "target.h"
#include "queue.h"
#include "rex.h"
#include "qw.h"
#include "rf.h"
#ifdef CUST_H
#include "customer.h"
#endif
#include "srch.h"


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/sleep.h_v   1.7   06 Mar 2001 13:07:22   evanmell  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/06/01   ejv     Moved some PM1000 defines into sleep.c.  Simplified RF
                   warmup constants.
02/20/01   ejv     Added dsb's changes for MSM5100.
01/22/01   ejv     Mainlined T_SLEEP.
01/09/01   ejv     Changed function Sleep() to uP_Sleep().
10/23/00   ry      Added OKTS SIG/MASK for Bluetooth.
09/06/00   eh      Reduced RF_WARMUP_TIME_MS to 7ms from 12ms, CLK_OVERHEAD_MS
                   to 5ms from 8ms, and TCXO_WARMUP_MS to 1ms from 5ms.
06/26/00   ejv     Externed new function sleep_clock_freq_estimate_abort.
06/02/00   nxb     Re-added FEATURE_GPIO_30.
06/01/00   day     Added RF_WARMUP_TIME_MS_MAX since RF_TUNE_CHAN_WAIT does
                   not always define the maximum warmup time for all targets.
05/24/00   jah     Added OKTS signal for FEATURE_RUIM.
05/18/00   day     Added warmup defines for T_O target.
05/11/00   rp      Added SLEEP_USB_OKTS_SIG when FEATURE_USB is defined.
                   Removed boolean sleep_usb_clock_rgm as it no longer needed.
04/28/00   day     Remove sleep warmup macro calculation and move into code 
                   before setting up for sleep.
04/14/00   rp      Added boolean sleep_usb_clock_rgm to vote not to sleep
                   when USB is active.            
03/17/00   ajn     Reimplemented Sleep Calculations.
01/27/00   ejv     Define SLEEP_CHGT_OKTS_MASK for FEATURE_FFA.
01/20/00   jc      Added SLEEP_CHGT_OKTS_SIG when FEATURE_PM1000 is defined.
                   Added TCXO_WARMUP_SC_TICKS.
09/11/99   rm/mk   Added FEATURE_UP_CLK_SWITCHING support.
08/26/99   ejv     Changed the RF warmup time to 12msec for SURF.  Removed
                   T_Q checking.
08/06/99   ejv     Extended sleep mask to 32 bits.  Added bits for VS task
                   and VOC task.  Use masks for sigs which may not be used.
                   Removed FEATURE_GPIO_30.
07/23/99   wjn     Replaced the compile-time checking with run-time checking
                   on "BAND_IS_PCS" for macro WARMUP_AND_RETUNE_MS definition.
06/28/99   yus     Merged IS95B features to the ASW tip.
06/28/99   yus     Added #ifdef SRCH_PRE_IS95B_BUILD for pre-95b backward build.
06/18/99   yus     Changed #ifdef FEATURE_JSTD008 to #if( BAND_IS PCS )
06/10/99   wli     Externed sleep_fee_sample_collected.
06/08/99   wli     Redesign of slow clock. 
04/26/99   rm      Rename SLEEP_OK_MASK to SLEEP_OK_MASK1 and redefined SLEEP_OK_MASK
                   to SLEEP_OK_MASK1 for Surf target and for other  target  assigned 
                   SLEEP_OK_MASK to bitwise or of SLEEP_OK_MASK1 and SLEEP_CHGT_OKTS.
04/21/99   ejv     Set DUTY_CYCLE_SC_HI based on target.  Removed
                   SLEEP_MOUSEMODE_MASK since it is never used.
04/20/99   rm      Removed SLEEP_CHGT_OKTS_SIG from SLEEP_OK_MASK from SURF target.
04/20/99   rm      Added sleep_tcxo_on to maintain the state of TCXO.
04/08/99   ejv     Rearranged declaration of srchdz_ok_to_compute_nominal_clk
                   so types were in proper order.
03/09/99   rnc     Defined a new constant DUTY_CYCLE_SC_HI which is the 
                   duty cyle of the slow clock's high level in percentage (0 to 100 %)
                   The variable CHPX8_PER_SC_HI is derived from DUTY_CYCLE_SC_HI
                   and indicates the chipx8's in a slow clock's high level.
03/08/99   aks     Added SLEEP_CHGT_OKTS_SIG to give the Charger module a vote
                   to Sleep.
02/16/99   ejv     Edited WARMUP_TIME pre-compile calculation and added
                   prototype for new Sleep() function.
02/01/99   ejv     Added FEATURE_NO_TCXO_VOTE which allows no-voting tcxo_
                   shutdown if defined.  Also edited FAST_WARMUP code so other
                   targets can use this feature.
12/16/98   ejv     Added TCXO_SHUTDOWN and SLOW_CLOCK code.
10/16/98   ejv     Merged changes to support Panther.
09/18/98   ejv     Added 5ms to fast warmup time for PCS builds due to
                   addition of rf_tune_to_chan function in srchzz.c.
07/08/98   ajn     Added FEATURE_FAST_WARMUP
06/29/98   ajn     Added interrupt latency parameter for FAST RF warm-up.
05/13/98   ajn     Added code to reduce RF warmup time.
04/13/98   dgy     Include customer.h only if CUST_H is defined.
03/17/98   scdb    Merged CLOCK 2.0 changes. This removes the SRCH_TIMEKEEP_10
                   define.
03/09/98   smo     Explicitly added include for target.h.
03/04/98   smo     Put back FEATURE_xxx Updates (manually) wich were deleted.
02/20/98   smo     Removed new clock support code via ifdef SRCH_TIMEKEEP_10.
                   This represents use of timekeeping VU clock01.00.00.
02/20/98   kss/smo Added functions to disable/re-enable idle mode.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
10/31/97   scdb    Added NOMINAL_CLOCKS_PER_MS constant
                   sleep_set_warmup_timer is no longer a public function.
                   This mechanism has been folded into sleep_set_sleep_timer
                   because the warmup timer interval is no longer constant.
06/19/97   rdh     Added Q support, removed pre-NGP/pre-MSM2 support.
03/14/97   jjn     Added (TG==T_MD) where-ever appropriate for the Module
11/22/96   rdh     Added Hutch's T_T target definitions.
09/25/96   dak     Added T_I2 to a an #ifdef to support temp calibration
                   for ISS2.
09/18/96   rdh     Added alternate clk_up_time maintenance technique.
07/16/96   thh     Add signal to trigger RF temp comp process.
02/22/96   smo     Removed unused timer constant for double-timer
02/16/96   smo     Added supoort for extending OSC warmup when waking up.
02/01/96   rdh     Tightening up timing on SAFEWIN and warmup times.
09/07/95   rdh     Mods to tick calc and setting sleep timer for slow-open SCI.
07/21/95   rdh     Made SLEEP_ERROR_SCALE smaller to avoid overflow problems.
07/18/95   rdh     Added new value for known drift in Gemini based on new spec.
06/15/95   rdh     Railing TRK_LO_ADJ high now for Gemini. A bit more stable.
06/15/95   rdh     Added RXC OKTS sig as RXC must control chipx8 before sleep.
06/13/95   rdh     Made T_SLEEP handling explicit and consistent DMSS-wide.
06/01/95   rdh     Mods for search driver break-up.
05/27/95   rdh     ISS2/Gemini integration, made sleep+warmup multiple of 26ms.
04/26/95   rdh     Added sleep timer tick adjust to remove TCXO drift.
04/18/95   rdh     Added T_I2 for ISS2 operation.
04/13/95   rdh     Added new Gemini TCXO/4 sleep timer clock rates.
03/30/95   rdh     Distributed wakeup functions back to ZZ/DZ.
03/23/95   rdh     Moved sleep/wakeup fncts from srchzz for deep sleep.
06/17/94   jah     Added OKTS signal for data services
01/18/94   jah     Added comments from the code review.
12/21/93   jah     Added SLEEP_LOG_OKTS_SIG
08/31/92   ptw     Created file.

===========================================================================*/


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                             Signal Masks
--------------------------------------------------------------------------*/

#define  SLEEP_START_SIG          TASK_START_SIG
  /* After creation and initialization, the Sleep Task waits for the Main
     Control Task to set this signal before beginning its normal execution. */

#define  SLEEP_STOP_SIG           TASK_STOP_SIG
  /* The Main Control Task never sets this signal to tell the Sleep Task to
     stop execution. */

#define  SLEEP_OFFLINE_SIG        TASK_OFFLINE_SIG
  /* This signal is set to tell Sleep to go into offline mode.
     Actually,  this doesn't do anything to Sleep. */

#define  SLEEP_RSVD_23_SIG        0x00800000
#define  SLEEP_RSVD_22_SIG        0x00400000
#define  SLEEP_RSVD_21_SIG        0x00200000

#ifdef   FEATURE_VAS
#define  SLEEP_VAS_OKTS_SIG       0x00100000
#endif
  /* Signals not presently used - can still use MSByte also */

#ifdef FEATURE_BT
#error code not present
#endif /* FEATURE_BT */

#ifdef FEATURE_RUIM
#define  SLEEP_RUIM_OKTS_SIG      0x00040000
#endif

#ifdef FEATURE_USB
#error code not present
#endif

#ifdef FEATURE_VS_TASK
#error code not present
#endif

#ifdef FEATURE_VOC_TASK
#error code not present
#endif

#define  SLEEP_CHGT_OKTS_SIG      0x00000400

#define  SLEEP_RF_TEMP_COMP_SIG   0x00000200
  /* This signal is when the RF driver wants Sleep to trigger its temp
     comp process. */

#define  SLEEP_RXC_OKTS_SIG       0x00000100

#ifdef FEATURE_DS
#define  SLEEP_DS_OKTS_SIG        0x00000080
#endif

#define  SLEEP_SND_OKTS_SIG       0x00000040
#define  SLEEP_LOG_OKTS_SIG       0x00000020
#define  SLEEP_DIAG_OKTS_SIG      0x00000010
#define  SLEEP_UI_OKTS_SIG        0x00000008
#define  SLEEP_HS_OKTS_SIG        0x00000004
#define  SLEEP_SRCH_OKTS_SIG      0x00000002
  /* Signals set by other tasks to indicate that CPU Sleep is OK,
     and cleared by other tasks to veto CPU sleep. */

#define  SLEEP_RPT_TIMER_SIG      0x00000001
  /* This signal is set when the sleep_rpt_timer expires.
     This timer is used to enable Sleep to kick the watchdog on time
     during a wait for some event. */


#ifdef FEATURE_BT
#error code not present
#else
#define SLEEP_BT_OKTS_MASK        0
#endif /* FEATURE_BT */

#ifdef FEATURE_RUIM
#define SLEEP_RUIM_OKTS_MASK      SLEEP_RUIM_OKTS_SIG
#else
#define SLEEP_RUIM_OKTS_MASK      0
#endif

#ifdef FEATURE_USB
#error code not present
#else
#define SLEEP_USB_OKTS_MASK       0
#endif

#ifdef FEATURE_VS_TASK
#error code not present
#else
#define SLEEP_VS_OKTS_MASK        0
#endif

#ifdef FEATURE_VOC_TASK
#error code not present
#else
#define SLEEP_VOC_OKTS_MASK       0
#endif

#ifdef FEATURE_DS
#define SLEEP_DS_OKTS_MASK        SLEEP_DS_OKTS_SIG
#else
#define SLEEP_DS_OKTS_MASK        0
#endif

#ifdef FEATURE_VAS
#define SLEEP_VAS_OKTS_MASK		  SLEEP_VAS_OKTS_SIG	
#else
#define SLEEP_VAS_OKTS_MASK		  0
#endif

#ifdef T_SURF
#if defined ( FEATURE_PM1000 ) || defined ( FEATURE_FFA )
#error code not present
#else
#define SLEEP_CHGT_OKTS_MASK      0
#endif
#else
#define SLEEP_CHGT_OKTS_MASK      SLEEP_CHGT_OKTS_SIG
#endif

#define SLEEP_OK_MASK   \
  ( SLEEP_SRCH_OKTS_SIG \
  | SLEEP_HS_OKTS_SIG   \
  | SLEEP_UI_OKTS_SIG   \
  | SLEEP_LOG_OKTS_SIG  \
  | SLEEP_DIAG_OKTS_SIG \
  | SLEEP_SND_OKTS_SIG  \
  | SLEEP_RXC_OKTS_SIG  \
  | SLEEP_VOC_OKTS_MASK \
  | SLEEP_VS_OKTS_MASK  \
  | SLEEP_DS_OKTS_MASK  \
  | SLEEP_CHGT_OKTS_MASK \
  | SLEEP_USB_OKTS_MASK  \
  | SLEEP_RUIM_OKTS_MASK \
  | SLEEP_BT_OKTS_MASK  )

#ifdef FEATURE_TCXO_SHUTDOWN
  #define TCXO_WARMUP_MS 1
  #define TCXO_WARMUP_US (TCXO_WARMUP_MS * 1000)
    /* It takes about 1ms for TCXO to be stable on SURF */
    
  #ifdef FEATURE_NO_TCXO_VOTE
    #ifdef FEATURE_GPIO_30
#error code not present
    #else
//      #define TCXO_GPIO_SEL  0x08
//      #define TCXO_GPIO_ON   0x08
//      #define TCXO_GPIO_OFF  0x00
      #define TCXO_GPIO_SEL	0x40			// msrock 00.04.24
      #define TCXO_GPIO_ON	0x40
      #define TCXO_GPIO_OFF	0x00
    #endif /* FEATURE_GPIO_30 */
  #endif /* FEATURE_NO_TCXO_VOTE */
  
#elif defined ( FEATURE_UP_CLOCK_SWITCHING )
#error code not present
#endif /* FEATURE_UP_CLOCK_SWITCHING */

/*-------------------------------------------------------------------------
      Constants for configuring the safe window to sleep without being
      forced to wait for a PNROLL.
-------------------------------------------------------------------------*/

#define  SAFEWIN_MASK  0x3F
  /* The bit mask for the 6 bits we use to check the safe window. */

#define  SAFEWIN_ZERO_VALUE  0x3F
  /* This value is used for zero safe window size.  */

#define  SAFEWIN_SLEEP  10
  /* How long we must allow for the processor to complete processing
     before the next 26.666...ms boundary, in units of 0.41666...ms.
     These are units of 512 chips aligned (0) at the PN Roll.  Set this
     parameter to SAFEWIN_ZERO_VALUE to disallow bypassing SS1. */


/*-------------------------------------------------------------------------
    WARMUP TIME
-------------------------------------------------------------------------*/

  #define RF_WARMUP_TIME_MS_MAX 45 
    /* Maximum = Total RF warmup time (40ms) plus 5ms clock tick */
  
#ifdef FEATURE_FAST_WARMUP
  
  /* The following values are experimental.  Other targets
     may require different values. */
  #if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
  #define WARMUP_TIME_MS 24
  #else
  #define WARMUP_TIME_MS 7
  #endif
    /* Loop settling, processing, etc. */

  #define CLK_OVERHEAD_MS   5
    /* Latency in event handling */

  #define RF_RETUNE_TIME    5
    /* Extra time needed to retune for PCS */

#else /* ! FEATURE_FAST_WARMUP */

  #define  WARMUP_TIME_MS    RF_WARMUP_TIME_MS_MAX
    /* Desired RF Warmup time, in milliseconds */
    
#endif /* FEATURE_FAST_WARMUP */
     
/*-------------------------------------------------------------------------
    Constants for sleep_set_sleep_timer()
-------------------------------------------------------------------------*/

#define NOMINAL_CLOCKS_PER_26MS  131200L
   /* Number of 4.9152 MHz TCXO/4 clocks in a 26.666... ms period,
     assuming that the clock error is zero. */

#define NOMINAL_CLOCKS_PER_MS    4920L
   /* Number of 4.92 MHz TCXO/4 clocks in one millisecond,
     assuming that the clock error is zero. */


#define  WARMUP_REGISTER  ((RF_TUNE_CHAN_WAIT * NOMINAL_CLOCKS_PER_MS + 63L) / 64L)
  /* # milliseconds at TCXO/4 clock (4.9152 MHz), in units of
     64 clock cycles. (The warmup timer hardware can only be
     set to multiples of 64 clock cycles.)  This define is used 
     in lifetest builds only. */

#define KNOWN_FREQ_ERROR (15L * 8L)
  /* Sleep timer-induced freq error drift in 8-chips per 1.28 sec interval */

#define ROLLS_PER_1280MS      48L
  /* Number of PN rolls per 1.28 seconds. */

#define SLEEP_ERROR_SCALE     128L
  /* Scale factor for freq error estimates */

#define CHIPS_PER_26MS        32768L
  /* Number of chips per 26.66... ms */

#define CLOCKS_PER_CHIP     (NOMINAL_CLOCKS_PER_26MS / CHIPS_PER_26MS)
  /* Number of nominal TCXO/4 clocks per chip */

#define WARMUP_CLOCKS  (WARMUP_REGISTER*64L)
  /* Conversion from register value to number of TCXO clocks used.
     Deriving this value from the register value may seem backwards,
     but it forces WARMUP_CLOCKS to be a possible value
     (i.e., a multiple of 64).  WARMUP_CLOCKS is used in lifetest
     builds only. */


#ifdef FEATURE_TCXO_SHUTDOWN
#ifdef FEATURE_NO_TCXO_VOTE
extern boolean srch_tcxo_pnroll;
extern boolean sleep_uart_clock_rgm;
extern boolean sleep_tcxo_on;
  /* Used to check whether or not TCXO has been turned back on after sleep. */
#endif

#elif defined ( FEATURE_UP_CLOCK_SWITCHING )
#error code not present
#endif 

typedef enum {

  SLEEP_CLOCK_ROUGH_ESTIMATE=0,   /* Short duration estimation (7ms) */
  SLEEP_CLOCK_GOOD_ESTIMATE=1     /* Long duration estimation (1sec) */

} sleep_clock_estimate_enum_type;



/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#ifndef T_MSM_5100
/*
;;;==========================================================================
;;; int uP_Sleep(int Address, int Ck1, int Ck2)
;;; 
;;; The purpose of "uP_Sleep" is to put ARM in Sleep mode with all external
;;; Chip selects high.  This is accomplished with the STMIA instruction
;;; which does a multiple register store to UP_CLK_CTL1 and UP_CLK_CTL2.
;;; Procedure inputs:
;;;                   R0 = Address of UP_CLK_CTL1
;;;                   R1 = initial value to store to UP_CLK_CTL1
;;;                   R2 = Value to store in UP_CLK_CTL2
;;; 
;;; 
;;; Additional registers
;;;                   R3 = Used as a mask to zero the LSB for second 
;;;                        UP_CLK_CTL2 store.
;;; Return            
;;;                   R0 = Address given as the input

    Note that this function is located in the boot code, in bootapp.s.

;;;==========================================================================
*/
int uP_Sleep(int Address, int Ck1, int Ck2);
#endif /* T_MSM_5100 */


/*===========================================================================

FUNCTION SLEEP_FORCE_WAKEUP

DESCRIPTION
  This function forces a wakeup by toggling the WAKE_UP_ENABLE bit.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Demod wakeup enable bit.

===========================================================================*/
void sleep_force_wakeup( void );

                     
/*===========================================================================

FUNCTION SLEEP_SET_SLEEP_TIMER

DESCRIPTION
  This function is called before each catnap. It programs the sleep timer and 
  the warmup timer for next catnap.

DEPENDENCIES
  "sleep_clock_freq_estimate_complete( )" had returned TRUE
  "sleep_set_warmup_time( )"

RETURN VALUE
  None

SIDE EFFECTS
  MSM is armed for a sleep cycle of the specified duration.  This sleep
  cycle starts on (a) the next PN roll, or (b) the end of the current
  catnap.

===========================================================================*/

extern void sleep_set_sleep_timer
(
  word  catnap_count,
    /* Number of catnaps elapsed in this sleep cycle */

  int4  catnap_length_chips
    /* Number of chips to catnap for */
);



/*===========================================================================

FUNCTION SLEEP_SET_WARMUP_TIME

DESCRIPTION
   Sets the warmup time to the given number of milliseconds, for the up
   coming sleep sequence.
 
DEPENDENCIES
   Must be called before a sleep cycle is initiated.  Requires an accurate
   value set is "sleep_sclk_freq".
   
RETURN VALUE
   None

SIDE EFFECTS
   Sets "sleep_warmup_ms".

===========================================================================*/

extern void sleep_set_warmup_time
(
  word warmup_ms
    /* Length of RF warmup, in milliseconds */
);

/*===========================================================================

FUNCTION SLEEP_ERROR

DESCRIPTION
  This function is used to notify the sleep controller of the perceived
  "error" in the most recent sleep interval's length, which the sleep
  controller may use to fine tune its estimate of the sleep clock.

DEPENDENCIES
  An initial estimate of the sleep clock frequency must be known, from
  sleep_clock_freq_estimate_complete( ).

RETURN VALUE
  None

SIDE EFFECTS
  Sleep clock frequency estimate is update.

===========================================================================*/
extern void sleep_error
(
  int2 chipx8_error
    /* Perceived error in the sleep duration of the most recent sleep */
);


/*===========================================================================

FUNCTION SLEEP_CLOCK_FREQ_ESTIMATE_ABORT

DESCRIPTION
    This function uninstalls the sleep fee ISR and conditionally clears
    2 FEE related flags.
   
DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/

extern void sleep_clock_freq_estimate_abort ( void );
                     

/*=============================================================================

FUNCTION SLEEP_CLOCK_FREQ_BEGIN_ESTIMATE

DESCRIPTION
   This function begins the process of estimating the frequency of the sleep
   clock.  If the sleep clock is driven by a known frequency source, such as
   TCXO/4, nothing is done.  If it is driven by a variable frequency source,
   such as the "Slow Clock" (30kHz to 60kHz), an estimation procedure is
   started.
   
DEPENDENCIES
   None 

RETURN VALUE
   None   

SIDE EFFECTS
  May install an ISR for Frequency Estimation.

===========================================================================*/
extern void sleep_clock_freq_begin_estimate(void);


/*===========================================================================

FUNCTION SLEEP_CLOCK_FREQ_ESTIMATE_COMPLETE

DESCRIPTION
   This function determines whether an estimate of the Sleep Clock's
   Frequency is available.  A "Rough" estimate, or a "Good" estimate
   may be requested. 

DEPENDENCIES
  None, although calling sleep_clock_freq_begin_estimate( ) in advance by
  a sufficient amount will ensure an estimate will be immediately available.

RETURN VALUE
  TRUE if a frequency estimate is available
  FALSE if a frequency estimate is not available

SIDE EFFECTS
  May start the frequency estimation process, it not currently running and
  no estimate is available.
  
  May uninstall the Frequency Estimation ISR.
   
===========================================================================*/
extern boolean sleep_clock_freq_estimate_complete
(
  sleep_clock_estimate_enum_type  estimate_type
);


/*===========================================================================

FUNCTION SLEEP_CLOCK_GET_FREQ_ESTIMATE

DESCRIPTION
   This function returns the sleep clock frequency estimate. 

DEPENDENCIES
  sleep_clock_freq_estimate_complete must have returned TRUE at some point
  in the past.

RETURN VALUE
  The frequency estimate, in Hertz.

SIDE EFFECTS
  None.
   
===========================================================================*/
extern uint4 sleep_clock_get_freq_estimate( void );


/*===========================================================================

FUNCTION SLEEP_SAFE_WINDOW

DESCRIPTION
   
DEPENDENCIES
  
RETURN VALUE
  
SIDE EFFECTS
  
===========================================================================*/

extern boolean  sleep_safe_window
(
  word  safe_window_size, 
    /* The duration of the safe window required, in units of 0.41666...ms
       (one LSBit of the MSByte of the combiner position register). These
       are units of 512 chips aligned (0) at the PN Roll. */

  dword *time_until_roll
    /* Time in milliseconds until the PN roll */
);


/*===========================================================================

FUNCTION SLEEP_TASK 

DESCRIPTION
  This procedure contains the main processing loop for the sleep task.
  It continually loops, looking for a consensus that the CPU may be
  put to sleep.  Lacking a consensus, it still halts the processor, but
  in "idle" mode -- in which any normal interrupt will restart processing
  with minimal delay.

DEPENDENCIES
  This procedure should be called by rex_def_task.

RETURN VALUE
  This procedure does not return.

SIDE EFFECTS
  The CPU may be put to sleep by this task.

===========================================================================*/

extern void sleep_task
(
  dword ignored
    /* Parameter received from Main Control task - ignored */
    /*lint -esym(715,ignored)
    ** Have lint not complain about the ignored parameter 'ignored' which is
    ** specified to make this routine match the template for rex_def_task().
    */
);

#if defined ( FEATURE_TCXO_SHUTDOWN ) || defined ( FEATURE_UP_CLOCK_SWITCHING )
/*===========================================================================

FUNCTION SLEEP_TCXO_SHUTDOWN_ISR

DESCRIPTION    This function is invoked to set a flag that will enable the
               sleep task to power down TCXO.

DEPENDENCIES   This function is to be called through the interrupt trampoline.

RETURN VALUE   None.

SIDE EFFECTS   srch_tcxo_pnroll flag is set.

===========================================================================*/
extern void sleep_tcxo_shutdown_isr(void);

#if defined ( FEATURE_NO_TCXO_VOTE ) || defined ( FEATURE_UP_CLOCK_SWITCHING )
/*===========================================================================

FUNCTION SLEEP_POWER_ON_TCXO

DESCRIPTION    This function is invoked to turn on TCXO by enabling GPIO9
               and setting up a pause timer for TCXO to be stable


RETURN VALUE   None.

SIDE EFFECTS   TCXO is powered on.

===========================================================================*/
extern void sleep_power_on_tcxo(void);

/*===========================================================================

FUNCTION SLEEP_POWER_DOWN_TCXO

DESCRIPTION    This function is invoked to turn off TCXO by enabling GPIO9

RETURN VALUE   None.

SIDE EFFECTS   TCXO is powered on.

===========================================================================*/
extern void sleep_power_down_tcxo(void);
#endif /* FEATURE_NO_TCXO_VOTE */
#endif /* FEATURE_TCXO_SHUTDOWN */

#endif /* SLEEP_H */

