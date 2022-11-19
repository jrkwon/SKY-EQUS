#ifndef CLKI_H
#define CLKI_H
/*===========================================================================

   C L O C K   S E R V I C E S   I N T E R N A L   H E A D E R    F I L E

DESCRIPTION
  This header file contains all the internal definitions used by the
  DMSS clock services.

Copyright (c) 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997, 1998       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/clki.h_v   1.1   10 Sep 1999 18:11:52   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/01/98   scdb    clk_uptime_val becomes a regional local
                   clk_tick_isr required a regional local declaration
10/30/97   scdb    Completely revamped the way that system time is maintained.
                   Added regional functions clk_uptime_add, clk_tickphase,
                   and macro CLK_UPTIME_DIFF
09/18/96   rdh     Added alternate clk_up_time maintenance technique.
11/09/94   gb      Changed TIMER_0 INT priority to 3.
03/29/94   jah     Initial revision

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "target.h"     /* Target specific definitions          */
#include "msg.h"        /* MSG macros, MSG_THRESHOLD            */
#include "clk.h"        /* General clock definitions            */


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
**                           Base Date
**
** Time values used by the Clock Services are offsets from a base date.
** All times are based on Midnight, January 6, 1980. This is sometimes known
** as the "epoch"; it's the specified time(0) for GPS, and also the specified
** time(0) for IS95.
**---------------------------------------------------------------------------
*/

/* This is the year upon which all time values used by the Clock Services
** are based.  NOTE:  The user base day (GPS) is Jan 6 1980, but the internal
** base date is Jan 1 1980 to simplify calculations
*/
#define BASE_YEAR       1980


/*
** Some calculations are made easier by basing a date on January 1 instead
** of January 6. These constants make that conversion easy.
*/

#define CLK_OFFSET_MS   432000000L
   /* 5 days (duration between Jan 1 and Jan 6), expressed as milliseconds. */

#define CLK_OFFSET_S    432000L
   /* 5 days (duration between Jan 1 and Jan 6), expressed as seconds. */

/*---------------------------------------------------------------------------
**                     80186 Timer #0 Control
**
** The clock is setup to run continuously generating a priority 2 interrupt
** each time the max count is reached.  The count is counted up 1 for each
** pulse of the baud clock input.
**---------------------------------------------------------------------------
*/
#define CLK_TIM0_CTL \
  ( P_TIM_EN_V | P_TIM_INHBAR_V | P_TIM_INT_V | P_TIM_EXT_V | P_TIM_CONT_V )

#define CLK_INT_TIM_CTL (P_INT_PRI3_V)  /* Priority of timer interrupt */

/*---------------------------------------------------------------------------
**     Maximum time interval for a call-back update, in milliseconds
**
** When there is a large change in system time, the call-back's are called
** with the delta.  To avoid having to use qwords for the call-backs, the
** maximum update is limited to a convenient value which will fit in a
** 32 bit data type.
**---------------------------------------------------------------------------
*/
#define CLK_CB_MAX_MS_UPDATE ((int4)(1000L * 60L * 60L * 24L)) /* 1 day */

#if (TG==T_M)
#error code not present
#endif /* TG==T_M */


#if ((TG==T_M) || (TG==T_B2))
#error code not present
#endif /* TG==T_M or T_B2 */


/*---------------------------------------------------------------------
**                Regional Message Threshold
**
** This is the regional message threshold for this module.
**---------------------------------------------------------------------
*/

extern word clk_msg_level;

#undef MSG_THRESHOLD
#define MSG_THRESHOLD   clk_msg_level

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

extern clk_cb_type clk_tick_active;    /* Tick call-back active list */

/*
** clk_powerup_caltime contains the time of day value when uptime was zero.
** It's calculated and stored when the phone receives a synchronization
** message from a CDMA cell, which happens when we acquire signal and after
** any call.
*/
extern qword clk_powerup_caltime;



/*
** clk_uptime_val is the core of system time keeping.
** It contains the number of seconds and milliseconds since the phone
** powered up. (Actually, it's the time since the clock module initialized.
** But that's close enough for our purposes.)
*/
extern clk_uptime_type clk_uptime_val;


#ifdef CLK_TIMENUDGE

/*
** In MSM 2 there is a bug in the hardware which generates the periodic
** interrupt. The effect is to make the hardware add one extra clock period
** of the TCXO/4 clock to each interrupt interval. This may not sound like
** much, but it adds up. If the system uses a 5 millisecond tick, the
** effect is a 41 PPM drift slow. Over a week, this is about 24 seconds.
** It would be nice to be able to correct for this, since it's systematic.
**
** So we do. The way is simple in principle: keep track of elapsed time, and
** when a certain amount of it has elapsed, add the amount of time which was
** lost to correct for the time slip. This process is known as "nudging".
**
** Because of the way the system works, the amount is different if TCXO is
** tracking normally (when the RF hardware is up) than it is when TCXO is
** railed (because the RF hardware is shut down for powersaving, the most
** common mode of operation). We're not trying for microscopic accuracy
** with this so much as we're trying to prevent long term cumulative drift.
** Rather then jumping through hoops to keep both in the same accumulator,
** it's easier simply to account for them separately and nudge whenever either
** of them says to.
**
** The term "railed" refers to the state when the crystal isn't being
** controlled and the crystal oscillator runs at the maximum or minimum
** frequency that it can (depending on how the hardware is set up).
** In our case, when we run railed, we run at the maximum speed.
**
** We run in railed mode when the receiver is turned off, because there's
** nothing for the oscillator control circuit to track to. It would be nice
** if the oscillator continued to track, but that's impossible.
**
** Operationally, time handled by clk_catchup_backlog is always considered
** railed time. When the crystal is railed, it runs about 9 PPM faster
** than when tracking. That's based on information from the crystal
** vendors, but it's an average. When the crystals are new, it's lower
** than that. As the phone (and crystal) ages it will rise; it
** will also vary depending on the temperature of the phone, and from one
** crystal to another, and from one vendor to another.
** This mechanism is not perfect but it's certainly better than not
** fixing the problem at all.
**
** When the clock is "tracking", the crystal error is 0 because our time is
** synchronized to time at the cell.
**
** The combination of these two means that when the clock is tracking in MSM2,
** we're 41 PPM slow, and when it's railed, we're 32 PPM slow. That's what this
** mechanism is designed to compensate for.
**
** In future, when this gets fixed in the silicon, we'll be dead on when
** tracking and 9PPM fast when railed.
** (NOTE TO ME!! THIS CODE ISN'T SET UP TO CORRECT FOR BEING TOO FAST!!!)
**
** Time handled by the periodic tick is railed time if SRCH has set
** its "OK To Sleep" bit, and tracking time otherwise. Time which is
** "caught up" is always railed time. (That's because time which is caught
** up is by definition time spent with the periodic tick disabled, which can
** only happen if SEARCH has voted "OK TO SLEEP", which it never does unless
** it's turned off the receiver.)
**
** The "PAYOFF" constants are how much time of each kind must accumulate
** for an increment (a "nudge") to take place. The "INCREMENT" is how much
** time to catchup after that time. Therefore, the "PAYOFF" time tells how long
** it takes for us to fall behind by the "INCREMENT", thus requiring a nudge
** for us to break even.
**
** The 41PPM drift due to the bug in the tick generator will be fixed in
** future silicon, but time spent "railed" will still be off and must still
** be compensated for.
**
** With this compensation, the phone will typically keep time accurate to
** within 5 PPM. Often it is much better than that. One phone tested kept
** time over five hours to 1/3 PPM, a matter of about 1 second worth of
** drift in five weeks.
*/

/*
** TRACKING drift is 40.65PPM slow (i.e. 4065 parts per 100 million)
*/
#define CLK_TRACKING_DRIFT        4065
   /* Drift rate, in PPM, when TCXO is tracking */
#define CLK_TRACKING_INCREMENT 1
   /* How much we want to add, in ms, to compensate for drift while tracking */
#define CLK_TRACKING_PAYOFF   \
           ((100000000L * CLK_TRACKING_INCREMENT) / CLK_TRACKING_DRIFT)
   /* How much time, in ms, must we spend tracking before we need to
      add CLK_TRACKING_INCREMENT */

extern int4 clk_tracking_account;
   /* Contains the amount of time we need to spend tracking, in ms, until
      the next time we need to add CLK_TRACKING_INCREMENT. This can be
      negative indicating that a nudge is past due. */

#endif /* CLK_TIMENUDGE */

#if (TG==T_G || TG==T_I2 || TG==T_Q || TG==T_T || TG==T_O)
#define CLK_TARGET_DRIFT 900
#else
#error code not present
#endif

/*
** RAILED drift is 9 PPM below CLK_TRACKING_DRIFT
** In MSM 3 and later, it's simply 9 PPM.
** (That is, in both cases, 900 parts per 100 million.)
*/
#ifdef CLK_TRACKING_DRIFT
#define CLK_RAILED_DRIFT          (CLK_TRACKING_DRIFT-CLK_TARGET_DRIFT)
   /* Drift rate, in PPM, when TCXO is railed in MSM 2 */
#else
#define CLK_RAILED_DRIFT          (CLK_TARGET_DRIFT)
   /* Drift rate, in PPM, when TCXO is railed in MSM 3 */
#error THIS CODE DOESN'T YET HANDLE MSM 3 CORRECTLY!!!
   /* That's because railed time is FAST, not SLOW, and this code is
      written to catch up lost time, not to decrement for fast time. */
#endif
#define CLK_RAILED_INCREMENT       1
   /* How much we want to add, in ms, to compensate for drift while railed */
#define CLK_RAILED_PAYOFF   \
           ((100000000L * CLK_RAILED_INCREMENT) / CLK_RAILED_DRIFT)
   /* How much time, in ms, must we spend railed before we need to
      add CLK_RAILED_INCREMENT */

extern int4 clk_railed_account;
   /* Contains the amount of time we need to spend railed, in ms, until
      the next time we need to add CLK_RAILED_INCREMENT. This can be
      negative indicating that a nudge is past due. */

#define CLK_NUDGE_FULL (-2147483648L)
   /* This is the largest negative value an int4 can contain. */


/*
** The following structure is used for communication with the
** function clk_tickphase.
** The calling function creates one of these and passes a reference.
** clk_tickphase will fill it in: 'time' contains the number of
** milliseconds since the last 5-ms interrupt, 'reg_value' contains the
** raw value read from the hardware from which 'time' was calculated.
*/
typedef struct {
    word time;
    int2 reg_value;
} clk_tickphase_type;



/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

  FUNCTION CLK_TICK_ISR

  DESCRIPTION
    This function is the clock tick interrupt handler.  It increments
    the second and millisecond time, and calls clk_tick_handler() to handle
    the call-backs.

  DEPENDENCIES
    Called from the Trampoline ISR.

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void clk_tick_isr(void);


/*===========================================================================

FUNCTION CLK_CB_INSERT

DESCRIPTION
  Insert a call-back structure into the active list.

DEPENDENCIES
  Clk_def() must have been called to initialize the call-back structure prior
  to passing it to this routine.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_cb_insert
(
  clk_cb_type *call_back_ptr
    /* Call-back structure to insert in active list */
);


/*===========================================================================

FUNCTION CLK_CB_DELETE

DESCRIPTION
  Delete a call-back structure from the active list.  Note that this
  sets the previous and next pointers of the call-back structure to
  NULL.

DEPENDENCIES
  Clk_def() must have been called to initialize the call-back structure prior
  to passing it to this routine.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_cb_delete
(
  clk_cb_type *call_back_ptr
    /* Call-back structure to delete from the active list */
);


/*===========================================================================

  MACRO CLK_UPTIME_DIFF

  DESCRIPTION
    Calculate and return a (int2) difference in milliseconds between two
    clk_uptime_types (specifically, returns left-right)

  DEPENDENCIES
    None
    
  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
#define CLK_UPTIME_DIFF(left, right)  \
   ( ((int2)(((int4)(left).secs)-((int4)(right).secs)))*CLK_MS_PER_SEC \
            + (int2)(left).ms - (int2)(right).ms)



/*===========================================================================

  FUNCTION CLK_TICK_HANDLER

  DESCRIPTION
    This function is called by the clock tick ISR and when time is updated.
    This routine ages/expires the active call-back structures.

  DEPENDENCIES
    None
    
  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
extern void clk_tick_handler
(
  clk_cb_type *active,  /* Active list of call-back structures         */
  int4 interval         /* Interval between this and the previous call */
);

/*===========================================================================

FUNCTION CLK_TICKPHASE

DESCRIPTION
  This function reads a hardware register and calculates from that the
  number of milliseconds since the last time a 5-ms tick took place.

DEPENDENCIES
  The calling code is responsible for masking interrupts before calling
  this function.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void clk_tickphase(
  clk_tickphase_type *retval  /* Structure through which to return the phase */
);

#endif /* CLKI_H */

