;============================================================================
;
;                   U A R T   P O L L
;
; GENERAL DESCRIPTION
;  This module contains a very simple-minded polling UART driver
;  for the DMSS boot downloader facility.  It also supports a
;  simple timeout functionality.
;
; EXTERNALIZED FUNCTIONS
;  uart_poll_asm
;      Polls UART status register to determine if byte is ready to
;      receive or not.
;
; INITIALIZATION AND SEQUENCING REQUIREMENTS
;  All necessary initialization for normal CPU operation must have
;  been performed before entering this module.  dload_uart_init() should
;  be called before any other functions.
;
; Copyright (c) 1995,1996,1997,1998,1999 by QUALCOMM Incorporated.
; All Rights Reserved.
;============================================================================


;============================================================================
;
;                           EDIT HISTORY FOR FILE
;  This section contains comments describing changes made to the module.
;  Notice that changes are listed in reverse chronological order.
;
;  $Header:   L:/src/asw/MSM5000/VCS/dloadpoll.s_v   1.2   05 Oct 1999 11:59:58   lchan  $
;
;  when       who     what, where, why
;  --------   ---     -------------------------------------------------------
;  03/17/99   ms      Removed comments related to bb_ for Virtual Boot Block.
;  11/18/98   udp     Replaced msm3rev2.h with generic msm3reg.h
;  09/13/98   hcg     Updated for MSM3000 archive.
;  08/05/98   hcg     Created
;
;============================================================================



;============================================================================
;
;
;                     INCLUDE FILES FOR MODULE
;
;============================================================================
#include "arm.h"
#include "armasm.h"
#ifdef T_MSM5000
  #include "msm50reg.h"
#else
  #include "msm3reg.h"
#endif
#include "dloaduart.h"


;============================================================================
;
;            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE
;
; This section contains local definitions for constants, macros, types,
; variables and other items needed by this module.
;
;============================================================================
#if defined (T_MSM3)
#define  MSMIO_BASE   CHIP_BASE
#else
#define  MSMIO_BASE   IO_BASE
#endif

#define  TIMER_OFFSET 0x0005f00f

;============================================================================
;
; FUNCTION uart_poll_asm
;
; DESCRIPTION
;  This function polls the UART status register.  The function loops
;  until it is determined that a byte is ready to be received, or a
;  timeout occurs, provided the timeout functionality is enabled.
;  Timeout should only be enabled for the intial communication with the
;  PST and is currently set to ~ 3 seconds.  This is currently an
;  empirically timed value.  A counter is
;  set, and counted until overflow, indicating the timeout has occurred.
;
;  The UART must be initialized and enabled, or else this routine will
;  timeout.
;
; RETURN VALUE
;  If a character is received without error, returns the UART status.
;  If an error occurs, returns UART_RX_ERR.  If timeout
;  is enabled and a timeout occurs, returns UART_TIMEOUT.
;
; SIDE EFFECTS
;  The watchdog timer is reset.
;
;============================================================================

        AREA    UARTPOLL, CODE, READONLY

        CODE16

uart_sr
        DCD     UART_SR_RB

time_offset
        DCD     TIMER_OFFSET

watch_dog
        DCD     SLEEP_CTL_WB

msmio_base
        DCD     MSMIO_BASE

        LEAF_NODE_16 uart_poll_asm

        push    {r4-r7}                 // save registers

        mov     r5, r0                  // move timeout enable flag
        cmp     r5, #1                  // Is it set?
        bne     rcv_loop                // no, just start rcving
        mov     r6, #0                  // yes, set-up timeout
        mvn     r7, r6                  // calculate timeout value
        ldr     r4, =time_offset        // read the timer offset
        ldr     r4, [r4]
        sub     r7, r7, r4

rcv_loop
        ldr     r4, =msmio_base         // MSM IO BASE
        ldr     r4, [r4]
        ldr     r2, =uart_sr            // offset for UART
        ldr     r2, [r2]
        mov     r0, r2
        ldrb    r0, [r0]                    // read byte from register
        mov     r1, #UART_SR_RB__RXRDY_MASK  // load mask
        and     r0, r0, r1              // see if lowest bit is set
                                        // if it's set, we should return here
        bne     rcv_done                // received a byte, so return

        mov     r2, #SLEEP_CTL_WB__WATCH_DOG_MASK  // pet the doggie


        ldr     r3, =watch_dog          // get offset for SLEEP_CTL
        ldr     r3, [r3]
        mov     r4, r3
        strb    r2, [r4]                // write to port
        mov     r2, #0                  // clear out R2
        strb    r2, [r4]                // write to port

        cmp     r5, #1                  // is timeout flag enabled?
        bne     rcv_loop                // no, so keep looping
        add     r7, r7, #1              // add to counter
        bcs     rcv_timeout             // we've timed out
        b       rcv_loop                // keep looping

rcv_timeout
        mov     r3, #0
        add     r0, r3, #UART_TIMEOUT   // indicate timeout has occurred
rcv_done                                // status is implicitly returned here

        pop     {r4-r7}                 // restore registers

        LEAF_NODE_END_16

        END
