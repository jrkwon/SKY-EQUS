#ifndef CLK_H
#define CLK_H
/*===========================================================================

           C L O C K   S E R V I C E S    H E A D E R    F I L E

DESCRIPTION
  This header file contains all the definitions necessary for accessing
  the clock services.

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1994, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999, 2000       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/clk.h_v   1.4   12 Sep 2000 08:47:28   evanmell  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/00   dgy     Added FEATURE_IS2000_QPCH.
06/15/00   jcw     Added support for 27MHz div 1 operation
12/03/99   ajn     Added function for altering catnap lengths.
10/01/98   scdb    Merge all ARM changes into the mainline
09/01/98   scdb    Added _cdecl to some functions to save ROM
05/08/98   scdb    Removed obsolete declaration of clk_set_baud_clk
03/02/98   scdb    First round of walkthrough changes, mostly cosmetic
03/01/98   scdb    Deleted clk_mode_ctl
                   Added clk_disable, clk_catnap_enable, clk_unexpected_enable
11/11/97   scdb    Changed certain #ifdefs from T_MSM2P to "T_MSM2"
10/30/97   scdb    Completely revamped the way that system time is maintained.
                   Added CLK_SAVE_TICKSTATE, clk_uptime_ms,
                   clk_predict_first_wakeup, clk_predict_next_wakeup,
                   clk_calculate_backlog, clk_catchup_backlog,
                   clk_inform_searchparams.
                   Removed clk_uptime_suspend, clk_uptime_suspended
                   Removed archaic declaration for clk_pwr_up
                   Removed clk_uptime_update
                   clk_ms_after_tick is dead code and has been commented out.
                   Though the code for them has not changed, the effective
                   behavior of clock callbacks and rex timers is different.
09/18/96   rdh     Added alternate clk_up_time maintenance technique.
08/15/95   jah     Added clk_mode_ctl() prototype & clk_mode_type, for MSM2P.
06/27/95   jah     Added tramp_call_ptr_type to call-back, for MSM2P
07/25/94   jah     Added prototype for clk_busy_wait().
06/14/93   jah     Added day_of_week to the end of clk_julian_type.
05/20/93   jah     Deleted CLK_DIV_ACP_VC, and added CLK_DIV_ACP_MC and
                   CLK_DIV_ACP_RX to really fix up the ACP speed problems.
05/19/93   jah     Added CLK_DIV_ACP_VC for ACP Voice channel speed-ups.
03/31/93   jah     Updated comments.
03/16/93   jah     Added CLK_DIVIDE() macro.
03/16/93   jah     Changed call-back to use int4 instead of int2, and added
                   clk_divide().
09/16/92   jah     Corrected documentation in clk_reg().
08/24/92   jah     Added clk_uptime()
05/12/92   jah     Revised clk_reg/dereg to sorted-linked-list algorithm.
02/28/92   jah     Ported from brassboard and added call-back services

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "target.h"     /* Target specific definitions          */
#include "qw.h"         /* Definition of qword                  */
#include "tramp.h"      /* Interrupt trampoline service         */

#ifdef T_ARM
#include "msm.h"        /* Needed for the pause timer           */
#endif

/*
 * It has been determined that there is a bug in MSM 2.2 which will also be
 * present in MSM2.3, having to do with the generator of the 5 millisecond
 * interrupt. The effect of the bug is that the periodic tick isn't really
 * happening every 5 milliseconds; it's just a tadge slow.
 *
 * The following symbol enables code which occasionally adds a bit of extra
 * time, in a controlled fashion, to uptime to compensate.
 *
 * This hardware bug will be fixed in MSM 3.
 * However, note that some of the capability will still be needed, since it
 * also compensates for the fact that TCXO changes speed when the RF
 * hardware is turned off.
 */
#ifdef T_MSM2
#define CLK_TIMENUDGE
#endif


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Clock call-back structure.  Note that although this structure is allocated
** by the routine calling clk_reg(), none of the fields should be modified or
** accessed by the calling module.  Any initialization or manipulation of this
** structure type is to be done by the routines in this module.
*/
typedef struct clk_cb_struct {
  struct clk_cb_struct *prev_ptr; /* Link to previous active list entry       */
  struct clk_cb_struct *next_ptr; /* Link to next active list entry           */
  int4 cntdown;                   /* Countdown until expiration, milliseconds */
  void (*routine_ptr)(int4);  /* Routine to call on expiration            */
  int4 crnt_ms;                   /* This interval, milliseconds              */
  int4 next_ms;                   /* Next interval, milliseconds              */
  boolean repeat;                 /* Whether to repeat                        */
#ifdef T_MSM2P
  tramp_call_ptr_type call_ptr;   /* Call-pointer for clock to use            */
#endif
} clk_cb_type;

/* Julian time structure
*/
typedef struct {
  word year;            /* Year [1980..2100]                            */
  word month;           /* Month of year [1..12]                        */
  word day;             /* Day of month [1..31] or day of year [1..366] */
  word hour;            /* Hour of day [0..23]                          */
  word minute;          /* Minute of hour [0..59]                       */
  word second;          /* Second of minute [0..59]                     */
  word day_of_week;     /* Day of the week [0..6] Monday..Sunday        */
} clk_julian_type;

#ifdef T_MSM2

/*
 * clk_uptime_type holds an "uptime" value, which is a structure containing a
 * time interval of less than 100 years (but nonetheless easier
 * to manipulate than a quad-word).
 * In proper use, the 'ms' field should be less than 1000.
 */
typedef struct {
  dword secs;    /* Number of seconds since the phone came up */
  word  ms;      /* Number of milliseconds in the most recent second, */
                 /* not yet finished or included in 'secs' */
} clk_uptime_type;

#endif

/* Milliseconds between calls to rex_tick(), updates to the time-of-day clock,
** and calls to interval call-back routines.
*/
#define CLK_MS_PER_TICK         5

/*
** The number of milliseconds in a second
*/
#define CLK_MS_PER_SEC          1000

/* The hardware clock is driven by an external input which this service
** refers to as the baud clock.  This divisor converts baud to cycles per
** CLK_MS_PER_TICK, which is the clock update interval used by this service.
** Note that this has implications on what clock tick intervals can be chosen.
** This number effects the actual hardware clock, so it must be accurate.
*/
#define CLK_BAUD_DIV    (1000 / CLK_MS_PER_TICK)

/* Clock divide control is "voted", so that multiple callers can ask
** for the clock to divide or run normally.  A counting semaphore is
** not appropriate due to the problems with the callers counting
** divide/restore's properly.  Each caller has a specifed mask which
** identifies it.
**
** There is a mask for 'nobody', which will update the hardware for
** control flows that want to update the hardware (e.g. after an interrupt)
** but do not have a vote in whether or not the clock is divided.
*/
#define CLK_DIV_NOBODY  0       /* Mask for no-change                   */
#define CLK_DIV_SND     1       /* Sound Task                           */
#define CLK_DIV_ACP     2       /* Analog Call Processing, general      */
#define CLK_DIV_CDMA    4       /* CDMA                                 */
#define CLK_DIV_ACP_MC  8       /* Analog Call Processing, main control */
#define CLK_DIV_ACP_RX  16       /* Analog Call Processing, receive      */

#define CLK_ALL_DIV_MASKS \
  (CLK_DIV_SND | CLK_DIV_ACP | CLK_DIV_CDMA | CLK_DIV_ACP_MC | CLK_DIV_ACP_RX )


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION CLK_INIT

DESCRIPTION
  Initialize the timer hardware and initialize the time-of-day to 0.
  Auto-register rex_tick() to be called each CLK_MS_PER_TICK.

DEPENDENCIES
  Depends on interrupt priority CLK_INT_TIM_CTL being disabled when this
  routine is called.

RETURN VALUE
  None

SIDE EFFECTS
  Enables timer 0, disables timers 1 & 2.

===========================================================================*/
extern void clk_init( void );


/*===========================================================================

FUNCTION CLK_DEF

DESCRIPTION
  Initialize the passed clock call-back structure.  This routine must be
  called prior to passing the call-back structure to clk_reg().

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_def
(
  clk_cb_type *call_back_ptr    /* Call-back structure to initialize */
);


/*===========================================================================

FUNCTION CLK_REG

DESCRIPTION
  Register a call-back structure to have a specified routine be called at
  the first interval (ms_first), and periodically every ms_periodic
  milliseconds after the first interval as described below.  Note that the
  order in which routines are registered does not define the order in which
  they are called-back.

DEPENDENCIES
  Clk_def() must have been called to initialize the call-back structure prior
  to passing it to this routine.

RETURN VALUE
  None

SIDE EFFECTS
  Registers a routine to be called at a later time by the clock tick ISR.

===========================================================================*/
extern void clk_reg
(
  clk_cb_type *call_back_ptr,
    /* Call-back structure to register
    */

  void (*routine_ptr)( int4 ms_interval ),
    /* Routine to call at the specified times.  When/if the routine is
    ** called, it's single parameter (ms_interval) is the interval
    ** responsible for the call.  If the call-back was delayed longer
    ** than the interval (ms_first or ms_periodic), ms_interval is the
    ** length of the actual interval and not the requested interval.
    */

  int4 ms_first,
    /* Interval before first call to routine().  If ms_first is 1,
    ** the first call to routine() is on the next clock tick.  If the
    ** interval is not a multiple of CLK_MS_PER_TICK, the routine will
    ** be called on the tick following the end of the requested interval.
    */

  int4 ms_periodic,
    /* Interval after first/successive calls to routine().  if ms_periodic
    ** is 0, the routine is automatically de-registered after the first call.
    ** If the interval is not a multiple of CLK_MS_PER_TICK, the routine will
    ** be called on the tick following the end of the requested interval.
    */

  boolean repeat
    /* If repeat is FALSE, the routine is de-registered after the second
    ** call.  If repeat is TRUE, the routine will be repeatedly called
    ** every ms_periodic milliseconds until de-registered.
    */
);


/*===========================================================================

FUNCTION CLK_DEREG

DESCRIPTION
  De-register a routine registered by calling clk_reg.

DEPENDENCIES
  Clk_def() must have been called to initialize the call-back structure
  prior to passing it to this routine.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_dereg
(
  clk_cb_type *call_back_ptr    /* Call-back structure to de-register */
);


/*===========================================================================

FUNCTION CLK_SET_MS

DESCRIPTION
  This procedure sets the time-of-day clock in millisecond units.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_set_ms
(
  qword set_val         /* Elapsed milliseconds since 6 Jan 1980 00:00:00 */
);


/*===========================================================================

FUNCTION CLK_READ_SECS

DESCRIPTION
  This procedure returns the current value of the time-of-day clock rounded
  to the nearest second. 

DEPENDENCIES
  Clk_init() must have already been called.

RETURN VALUE
  The current value of the time-of-day clock rounded to the nearest second.
  If the phone has never received and processed a Sync Channel Message
  from a CDMA cell, the value returned will be sometime in 1980.

SIDE EFFECTS
  None

===========================================================================*/
extern dword clk_read_secs( void );

#ifdef FEATURE_NUI_D1 
void NU_Clk_Read( dword* SecA, dword* MsA );

#endif /* FEATURE_NUI_D1 */




/*===========================================================================

FUNCTION CLK_READ_MS

DESCRIPTION
  This procedure returns the current value of the time-of-day clock in 
  millisecond units. 

DEPENDENCIES
  Clk_init() must have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_read_ms
(
  qword raw_val     /* Returned value: time-of-day in millisecond units */
);


/*===========================================================================

FUNCTION CLK_UPTIME

DESCRIPTION
  Return the number of seconds the phone has been on.

DEPENDENCIES
  None

RETURN VALUE
  Number of seconds since the phone powered on. The value is not rounded.

SIDE EFFECTS
  None

===========================================================================*/
extern dword clk_uptime( void );


/*===========================================================================

FUNCTION CLK_PREDICT_FIRST_WAKEUP

DESCRIPTION
  This is called during setup of the first catnap of a series of catnaps
  making up a "slotted sleep" or "deep sleep". The purpose is to calculate,
  and store, the expected "uptime" value when the wakeup interrupt happens
  at the end of the catnap.

DEPENDENCIES
  This function must only be called from sleep_set_sleep_timer.

RETURN VALUE
  Amount of time, in 4.92 Mhz clocks, that the wakeup interrupt should be
  shifted by in order to prevent a predicted collision with a periodic tick.


SIDE EFFECTS
  None

===========================================================================*/
int clk_predict_first_wakeup(

  /*
   * This contains the number of milliseconds between the PNROLL which will
   * make the clock start running and when the wakeup interrupt will happen.
   */
  word length

);


/*===========================================================================

  FUNCTION CLK_UPTIME_ADD

  DESCRIPTION
    Add a (word) number of milliseconds to an "clk_uptime_type"

  DEPENDENCIES
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void clk_uptime_add(clk_uptime_type *targ, word incr);


/*===========================================================================

FUNCTION CLK_INFORM_SEARCHPARAMS

DESCRIPTION
  Used to inform CLOCK of the expected duration of catnaps.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  This function arms clk_predict_first_wakeup to run once.

===========================================================================*/
void clk_inform_searchparams(

  word catnap_length
    /*
     * This is the duration of a catnap, in units of 26.666 ms. (In other
     * words, a 320 millisecond catnap is represented here as "12".
     * What CLK really wants to know is the time interval between successive
     * wakeup interrupts.)
     */
);


#if ( ( defined FEATURE_ENHANCED_STANDBY_III ) || ( defined FEATURE_IS2000_QPCH ) )
/*===========================================================================

FUNCTION CLK_ALTER_CATNAP_LENGTH

DESCRIPTION
  Used to inform CLOCK of a change duration of catnaps.

DEPENDENCIES
  May only be called during a catnap wakeup, after a call
  to clk_catnap_enable( ).

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void clk_alter_catnap_length(

  word new_catnap_length
    /*
     * This is the duration of a catnap, in units of 26.666 ms. (In other
     * words, a 320 millisecond catnap is represented here as "12".
     * What CLK really wants to know is the time interval between successive
     * wakeup interrupts.)
     */
);

#endif /* FEATURE_ENHANCED_STANDBY_III || FEATURE_IS2000_QPCH */

/*===========================================================================

FUNCTION CLK_DISABLE

DESCRIPTION
  This disables the periodic interrupt. Uptime and time-of-day freeze,
  REX timers and clock callbacks cease to expire, and power consumption
  decreases markedly.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void clk_disable(void);


/*===========================================================================

FUNCTION CLK_CATNAP_ENABLE

DESCRIPTION
  This function enables the periodic timekeeping when the wakeup interrupt
  happens and does time accounting for the catnap just past.

DEPENDENCIES
  It must only be called from the wakeup ISR.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void clk_catnap_enable(
  int remaining
    /* Number of catnaps beyond the one just finished which are scheduled
       for the current sleep series */
);


/*===========================================================================

FUNCTION CLK_UNEXPECTED_ENABLE

DESCRIPTION
  Reenable the periodic tick when any interrupt except the wakeup interrupt
  turns the phone on while the phone is asleep. Some time accounting takes
  place, REX timers and clock callbacks can expire, but uptime/TOD may not
  be brought completely up to date.

DEPENDENCIES
  It should be called from any interrupt service routine which can
  prematurely terminate a catnap, and only from them.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void clk_unexpected_enable(void);


/*===========================================================================

FUNCTION CLK_UPTIME_MS

DESCRIPTION
  Returns the phone uptime in units of milliseconds via a parameter.

DEPENDENCIES
  clk_init must have been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void clk_uptime_ms(
  qword uptime_ms   /* Returned uptime, in millisecond units. */
);


/*===========================================================================

FUNCTION CLK_SECS_TO_JULIAN

DESCRIPTION
  This procedure converts a specified number of elapsed seconds   
  since the base date to its equivalent Julian date and time.     

DEPENDENCIES
  None

RETURN VALUE
  The specified Julian date record is filled in with equivalent date/time,
  and returned into the area pointed to by julian_ptr.

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_secs_to_julian
(
  dword secs,                   /* Number of seconds since base date    */
  clk_julian_type *julian_ptr   /* Pointer to Julian date record        */
);
  

/*===========================================================================

FUNCTION CLK_JULIAN_TO_SECS

DESCRIPTION
  This procedure converts a specified Julian date and time to an  
  equivalent number of elapsed seconds since the base date.    

DEPENDENCIES
  None

RETURN VALUE
  Number of elapsed seconds since base date.       

SIDE EFFECTS
  None

===========================================================================*/
extern dword clk_julian_to_secs
(
  clk_julian_type *julian_ptr   /* Pointer to Julian date record */
);


/*===========================================================================

FUNCTION CLK_DIVIDE

DESCRIPTION
  Petition to divide the clock, or demand that the clock no longer
  be divided.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If all voters agree to divide the clock, it is divided.  If any
  disagree, the clock is restored to full-rate.

===========================================================================*/
extern void clk_divide
(
  word mask,
    /* ballot of a clock divide request associated with caller */
  boolean divide
    /* True = divide clock, False = restore clock.             */
);


/*===========================================================================

MACRO CLK_DIVIDE

DESCRIPTION
  A target-independent wrapper for clk_divide().  Compiles as a single ';'
  on Mobiles, as a clk_divide() call on Portables.

PARAMETERS
  xxx_mask      Mask of bits voting to turn down/up the clock
  xxx_divide    Value (TRUE=divide or FALSE=don't divide)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#if (TG==T_P)
#error code not present
#else
#define CLK_DIVIDE(xxx_mask,xxx_divide)
#endif


/*===========================================================================

FUNCTION CLK_BUSY_WAIT

DESCRIPTION
  Perform a busy-wait for the specified number of microseconds.
  Interrupts are not blocked by this routine.  If blocking is desired,
  it should be done by the caller.

  For ARM targets (MSM 3 and above), if the called value is not a constant
  then the code will not be optimized and the delay will be longer. Also, if
  the build is not optimized, then the delays will be longer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if defined(T_ARM)

extern void clk_empty_function (void);
extern void clk_long_busy_wait (int microseconds);

/* Time constants for ARM clock busy wait using the pause timer. */

/* Number of core clock ticks in 1 microsecond.  We currently base
   this on a 27Mhz clock. */
#define CLK_WAIT_CORE_TICKS 27

/* Divider on the pause timer.  The pause timer counts the value
   loaded into it times this value. */
#define CLK_WAIT_PAUSE_DIVIDER 64



/* Constant offset for the pause timer.  First, add in 1 less than the
   PAUSE_DIVIDER, this rounds the value we write up (must wait at
   least the given amount of time).  Then we subtract off the overhead
   of writing to the pause register.  When optimizing, the compiler
   will typically emit something as follows:
      LDR  r0, #pause_high_part      ; 2 clocks
      SHL  r0, #shift_value          ; 2 clocks
      LDR  r1, #value                ; 2 clocks
      STRH r1, [r0+offset]           ; 2+1 clocks
   for a total of 9 clocks.  Since the pause timer counts in external
   clocks (the 27Mhz, not the 13.5 Mhz), this must be doubled if the
   CPU is running at div 2. */

#ifdef FEATURE_CPU_CLK_DIV1
#define CLK_CPU_MCLK_DIV 1
#else
#define CLK_CPU_MCLK_DIV 2
#endif


#define CLK_WAIT_OVERHEAD_DELAY ((CLK_WAIT_PAUSE_DIVIDER - 1) - CLK_CPU_MCLK_DIV * 9)

/* Compute the pause timer value given a specific number of
   microseconds. */
#define CLK_PAUSE_VALUE(microseconds)           \
       (((microseconds) * CLK_WAIT_CORE_TICKS   \
         + CLK_WAIT_OVERHEAD_DELAY)             \
        / CLK_WAIT_PAUSE_DIVIDER)

/* The implementation of clock busy wait. */
#if defined(CLK_BUSY_WAIT_DEBUG)
# define clk_busy_wait(microseconds) clk_long_busy_wait (microseconds)
#else
# define clk_busy_wait(microseconds)                                    \
  do {                                                                  \
    if (microseconds <= 1)                                              \
      clk_empty_function();                                             \
    else if (microseconds <= 2) {                                       \
      clk_empty_function ();                                            \
      clk_empty_function ();                                            \
    } else if (microseconds <= 50)                                      \
      MSM_OUTH (PAUSE_TIMER_WH, CLK_PAUSE_VALUE (microseconds));        \
    else                                                                \
      clk_long_busy_wait (microseconds);                                \
  } while (0)
#endif /* not CLK_BUSY_WAIT_DEBUG. */

#else

/* The 186 version. */
extern void clk_busy_wait
(
  word microseconds
    /* Number of microseconds to wait */
);

#endif

#if defined(T_ARM)
/*===========================================================================

FUNCTION CLK_NANO_WAIT

DESCRIPTION
  Perform a very short busy wait.  Takes about 200ns.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern volatile byte clk_nano_location;
#define clk_nano_wait() (clk_nano_location = 1)

#endif /* T_ARM */

#endif /* CLK_H */

