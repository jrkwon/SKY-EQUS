#ifndef TRAMP_H
#define TRAMP_H
/*===========================================================================

      I N T E R R U P T   T R A M P O L I N E   H E A D E R   F I L E

DESCRIPTION
  All the declarations and definitions necessary to support interaction
  with the QUALCOMM CDMA ASIC interrupt controllers.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 1992, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999, 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/tramp.h_v   1.6   04 Feb 2000 16:28:44   rajeevg  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/00   jct     Integrated MSM5000 support
06/14/99   tac     Support for pre-emptive interrupts added.
02/10/99   tac     Re-arranged interrupts in the enum to reflect table-driven
                   approach implemented in trampm5.c.
01/12/98   ak      Added TRAMP_DP_RX_DATA2_ISR #define in order to support
                   new ISR.
12/16/98    ms     Removed 80186 support.
11/28/98   jkl     Revised according to code review comments.
11/05/98   ms      Merged in MSM3000 support.
06/27/95   jah     Added tramp_int_call()
03/06/95   jah     Added MSM2p support
10/30/92   jah     Removed Sleep/Key interrupt, added GPIO_INT_0..4
03/20/92   jah     Ported module from brassboard srch.h

===========================================================================*/

#include "comdef.h"
#ifdef TG
   #ifndef TARGET_H
      #include "target.h"   /* Definition for TG, and target specific defs */
   #endif
#endif

#include "queue.h"      /* Queue declaration package                   */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Interrupt service routine types.  The associated comment line describes
** the interrupt source.  Do not change the order/contents without updating
** the interrupt table in trampm5.c. Also note that the order implies a 
** "pseudo-priority".  This pseudo-priority does not mean that interrupts
** preempt one another.  What it means is that when the interrupt status 
** registers are examined, the interrupts appearing earlier in the list are
** checked for prior to checking for those lower in the list.  t.  e.g. the 
** 1.25 millisecond interrupt is always checked for first.
*/

typedef enum {
  TRAMP_125_MS_ISR = 0, /* 1.25 millisecond interrupt           */
  TRAMP_PNROLL_ISR,     /* PN counter roll-over, 26.6 ms        */
#ifdef T_MSM5000
  TRAMP_SYS_TIME_INT1_ISR,  /* System Timer 1 interrupt         */
  TRAMP_RX_QP_ISR,          /* Quick Paging Channel Interrupt   */
#endif
  TRAMP_TX_ISR,         /* Encoder Transmit Frame Interrupt     */
  TRAMP_RX_ISR,         /* Decoder receive frame interrupt      */
  TRAMP_SDUMP_ISR,      /* Demodulator status dump              */
  TRAMP_TX_WBD_ISR,     /* FM wideband data transmit interrupt  */
  TRAMP_RX_WBD_ISR,     /* FM wideband data receive interrupt   */
  TRAMP_UART_ISR,       /* UART interrupt                       */
  TRAMP_GPIO_INT_0_ISR, /* Demodulator GPIO_INT_0 interrupt     */
  TRAMP_GPIO_INT_1_ISR, /* Demodulator GPIO_INT_1 interrupt     */
  TRAMP_GPIO_INT_2_ISR, /* Demodulator GPIO_INT_2 interrupt     */
  TRAMP_GPIO_INT_3_ISR, /* Demodulator GPIO_INT_3 interrupt     */
  TRAMP_GPIO_INT_4_ISR, /* Demodulator GPIO_INT_4 interrupt     */
  TRAMP_VOC_ENC_ISR,    /* Vocoder encode interrupt             */
  TRAMP_VOC_DEC_ISR,    /* Vocoder decode interrupt             */
  TRAMP_SAT_LOCK_ISR,   /* FM SAT Lock interrupt                */
  TRAMP_KEY_SENSE_ISR,  /* Key-sense interrupt                  */
  TRAMP_WAKEUP_ISR,     /* Sleep wake-up interrupt              */
  TRAMP_DP_RX_DATA_ISR,          /* DP rx interrupt             */
  TRAMP_SEARCH_DMA_REQUEST_ISR,  /* Search DMA interrupt        */
  TRAMP_SLEEP_FEE_ISR,           /* Sleep Fee interrupt         */
  TRAMP_SBI_ISR,                 /* SBI interrupt               */
  TRAMP_AUX_PCM_DIN_ISR,         /* Aux PCM interrupt           */
  TRAMP_GP_TIMER_ISR,            /* GP Timer interrupt          */
  TRAMP_UART2_ISR,               /* UART2 interrupt             */
  TRAMP_DP_RX_DATA2_ISR,         /* DP RX2 interrupt            */
  TRAMP_TICK_ISR,       /* Programmable timer tick              */
  TRAMP_ISR_CNT         /* Count of defined ISR's               */

} tramp_isr_type;       /* typedef enum ...     */

/* The MSM5000 overloads the SYS_TIME_INT2 position to be either
** the System Timer 2 interrupt or the Quick Paging Channel
** interrupt.  Since we only want one entry in the interrupt table
** for tramp, the overload is reflected via this definition of
** the QP_ISR to the TIME_INT2_ISR
*/
#ifdef T_MSM5000
  #define TRAMP_SYS_TIME_INT2_ISR TRAMP_RX_QP_ISR  
#endif

/*
** Define these for backward compatibility.
*/
#ifdef T_MSM5000
  #define TRAMP_SYS_TIME_INT1 TRAMP_SYS_TIME_INT1_ISR
  #define TRAMP_SYS_TIME_INT2 TRAMP_SYS_TIME_INT2_ISR
#endif

/* Type for the parameter to pass to the queued function call 
*/

typedef union {
  int4 arg_int4;
  void *arg_ptr;
} tramp_call_arg_type;

/* Type for queued function calls */

typedef struct {
  q_link_type   link;                      /* Queue link                  */
  boolean       in_use;                    /* True if in-use              */

  void (*call_ptr)( tramp_call_arg_type ); /* pointer to function to call */
  tramp_call_arg_type arg;                 /* parameter for function      */

} tramp_call_ptr_type;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION TRAMP_INIT

DESCRIPTION
  Initialize the interrupt vector for the interrupt trampoline and
  configure the microprocessor and CDMA ASIC interrupt controllers
  for operation with it.

DEPENDENCIES
  This must be done *after* the ASIC has been initialized,
  as the initialization procedure resets the interrupt block(s).

RETURN VALUE
  None

SIDE EFFECTS
  This disables all of the multiplexed CDMA ASIC interrupts.

===========================================================================*/
extern void tramp_init( void );


/*===========================================================================

FUNCTION TRAMP_SET_ISR

DESCRIPTION
  Specify the address of the interrupt service routine (ISR) for a given
  interrupt.  If the address is NULL, the interrupt is disabled by masking
  it out in the CDMA ASIC interrupt controller.

DEPENDENCIES
  tramp_init() must have been previously called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tramp_set_isr
(
  tramp_isr_type which_isr,     /* Select which ISR is being set */
  void (*isr_ptr)( void )       /* The routine to use as the ISR */
);



/*===========================================================================

FUNCTION TRAMP_REINIT

DESCRIPTION
  Reinitialize interrupt masks after dmod reset

DEPENDENCIES
  tramp_init() must have been previously called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tramp_reinit( void );


/*===========================================================================

FUNCTION TRAMP_QUEUE_CALL

DESCRIPTION
  Queue a function call to be called during interrupt processing at lower
  priority than any other interrupt processing.

DEPENDENCIES
  tramp_init() must have been previously called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tramp_queue_call
(
  tramp_call_ptr_type *call_ptr /* Call-pointer to queue */
);


/*===========================================================================

FUNCTION TRAMP_CALL_DEF

DESCRIPTION
  Initialize a call-pointer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tramp_call_def
(
  tramp_call_ptr_type *call_ptr /* Call-pointer to initialize */
);

#endif  /* TRAMP_H */
