#ifndef RING_H
#define RING_H
/*===========================================================================

              R I N G E R   D R I V E R   H E A D E R   F I L E

DESCRIPTION

  This header file contains all the definitions necessary for other
  tasks to interface with the ringer driver.
  Definitions of the ringer control registers in the MSM 2.2 and 2.3.

REFERENCES
  MSM2.2 User's Manual.

Copyright (c) 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.03.02   chos    GPIO 변경에 따른 관련 부분 수정
00.03.02   chos    Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
03/04/99    st     Creation (mainly cut and paste from sndring.h and sndihw.h)

===========================================================================*/

#include "processor.h"  /* Definition of chip selects           */
#include "target.h"     /* Target specific definitions          */
#include "bio.h"        /* Bit I/O macros                       */

#if defined(T_MSM3)
#include "msm.h"
#endif /* defined(T_MSM3) */

/* Target uses Ringer Enable Line.
*/
#if ((TG==T_G) || (TG==T_I2))
// by chos 00.03.02 for not use BIO_RINGER_EN_M
//#define RING_USE_RINGER_ENABLE
#endif

/* <EJECT> */
/*===========================================================================

                            DATA DECLARATIONS

===========================================================================*/
#if defined(T_MSM3) /* MSM3000's registers are dword aligned */
#define RINGER_A    SND_RINGER_A
#define RINGER_B    SND_RINGER_B
#define RING_D_LSB  0x0             /* Offset LS Byte "D" Register  */      
#define RING_D_MSB  0x4             /* Offset MS Byte "D" Register  */
#define RING_N_LSB  0x8             /* Offset LS Byte "N" Register  */      
#define RING_N_MSB  0xC             /* Offset MS Byte "N" Register  */
#define RING_M      0x10            /* Offset One Byte "M" Register */
#else
/*---------------------------------------------------------------------------
  MSM2Px macros, commands, definitions, and types
---------------------------------------------------------------------------*/
#define RINGER_A    (P_PCS3_IO+0x40)/* 1st Port Ringer "A"          */
#define RINGER_B    (P_PCS3_IO+0x4A)/* 1st Port Ringer "B"          */
#define RING_D_LSB  0x0             /* Offset LS Byte "D" Register  */      
#define RING_D_MSB  0x2             /* Offset MS Byte "D" Register  */
#define RING_N_LSB  0x4             /* Offset LS Byte "N" Register  */      
#define RING_N_MSB  0x6             /* Offset MS Byte "N" Register  */
#define RING_M      0x8             /* Offset One Byte "M" Register */
#endif /* defined(T_MSM3) */

#define RINGER_OUT(ringer,reg,val)  outp((ringer)+(reg),(val))

#define RNG_D_JAM_HIGH  0x00    /* Duty Cycle = 0 Jams M/N Output High  */
#define RNG_M_STOP      0x00    /* This Value in "M" stops Ringer       */
#define RNG_M_START     0x40    /* This Bit in "M" Starts Ringer        */

/* Structure containing the two values required to generate a
** tone using the M & N counters for the ringer.
*/
typedef struct {
   byte m;
   int  n;
} ring_mn_counter_type;

/* <EJECT> */
/*===========================================================================

MACRO RINGER_STOP

DESCRIPTION
  Stop the M/N counter driving the Ringer.

PARAMETERS
  None

DEPENDENCIES
  This macro may be used in an error handler or interrupt routine.
  It does not call any subroutines or use any variables. It consists
  solely of I/O instructions.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define RINGER_STOP() \
  RINGER_OUT(RINGER_A,RING_M,RNG_M_STOP); \
  RINGER_OUT(RINGER_B,RING_M,RNG_M_STOP);

/* <EJECT> */
/*===========================================================================

FUNCTION ring_init

DESCRIPTION
  Generic Low Level Hardware Initialization for ringer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  ring_msm23_plus_inuse is set.

===========================================================================*/
extern void ring_init ( void );
/* <EJECT> */
/*===========================================================================

FUNCTION ring_click

DESCRIPTION
  Turn the ringer on and starts counter.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void ring_click ( void );
/* <EJECT> */
/*===========================================================================

FUNCTION ring_tone

DESCRIPTION
  Start a DTMF emitting from the ringer.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void ring_tone
( 
  word  freq_a,             /* frequency in hz of 1st tone to play */
  word  freq_b,             /* frequency in hz of 2nd tone to play */
  int   ring_vol,           /* duty cycle of tone, 5000 = 50% */
  word  duration_ms         /* duration of tone, 0 means turn ringer off */
);
/* <EJECT> */
#ifdef FEATURE_VIBRATOR
#error code not present
#endif
/* <EJECT> */
/*===========================================================================

FUNCTION ring_stop_ring

DESCRIPTION
  Stop a tone emitting from the ringer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
LOCAL void ring_stop_ring( void );
/* <EJECT> */
/*===========================================================================

FUNCTION ring_compute_ringer_freq

DESCRIPTION
  This function returns the (m,n) pair for a given frequency to be
  played by the ringer.

DEPENDENCIES

RETURN VALUE
  Returns the (m,n) pair structure ring_mn_counter_type.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL void ring_compute_ringer_freq
(
  word frequency,
    /* Frequency in hz for which the m,n values are to be computed */
  ring_mn_counter_type *mn_out_val
    /* (m,n) pair for the frequency entered                        */
);
/* <EJECT> */
#endif /* RING_H */

