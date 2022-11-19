/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   U A R T   D R I V E R  H E A D E R 

 GENERAL DESCRIPTION
  This header file contains the definitions of the MSM2.x UART 
  hardware registers needed for the boot downloader.

  Copyright (c) 1995,1996,1997,1998,1999 by QUALCOMM Incorporated.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                           EDIT HISTORY FOR FILE
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/dloaduart.h_v   1.2   04 Oct 1999 15:26:38   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/22/99   lcc     Merged in MSM5000 support from PLT archive.
08/11/99   kk      Merge from SS baseline.
08/05/98   hcg     created, taken from flashprg

===========================================================================*/



/*===========================================================================

                            INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Error return code from uart_receive_byte */
/* This int value must not be any valid unsigned char value. */
#define  UART_RX_ERR    (-1)
#define  UART_TIMEOUT   (-2)

#define  TIMEOUT_ENABLED   TRUE
#define  TIMEOUT_DISABLED  FALSE

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* MSM 2.x UART Registers used */

#if !defined (T_MSM3)
#define  PCS5_BASE      0x0280
#define  UART_BASE      (PCS5_BASE + 0x56)
#endif /* !T_MSM3 */

#if !defined (T_MSM3)
#define  UART_MR1       (UART_BASE + 0)   /* UART Mode Register 1          */
#endif /* !T_MSM3 */

#define  UART_MR1_RFRC     0x80           /* automatic RFR control         */
#define  UART_MR1_CTSC     0x40           /* automatic CTS control         */

#if !defined (T_MSM3)
#define  UART_MR2       (UART_BASE + 2)   /* UART Mode Register 2          */
#endif /* !T_MSM3 */

#define  UART_MR2_8BPC     0x30           /* 8 bits per character          */
#define  UART_MR2_1SB      0x04           /* 1 stop bit                    */

#if !defined (T_MSM3)
#define  UART_SR        (UART_BASE + 4)   /* UART status register          */
#endif /* !T_MSM3 */

#define  UART_SR_HUNT_RXD  0x80           /* hunt character received       */
#define  UART_SR_BREAK_RXD 0x40           /* break received                */
#define  UART_SR_PF_ERR    0x20           /* Parity or Framing error       */
#define  UART_SR_OVR_ERR   0x10           /* Overrun error                 */
#define  UART_SR_TXEMT     0x08           /* Transmitter empty             */
#define  UART_SR_TXRDY     0x04           /* Transmitter ready             */
#define  UART_SR_RXFULL    0x02           /* Receiver full                 */
#define  UART_SR_RXRDY     0x01           /* Receiver ready                */

#if !defined (T_MSM3)
#define  UART_CSR       (UART_BASE + 4)   /* UART clock select register    */
#endif /* !T_MSM3 */

#define  UART_CSR_384K_BPS 0xdd           /* 38,400 bps                    */
                /* This is really only 19.2 kbps with the clock workaround */
#define  UART_CSR_1152K_BPS 0xff          /* 115,200 bps                   */
                /* This is really only 57.6 kbps with the clock workaround */

#if !defined (T_MSM3)
#define  UART_RFIFO     (UART_BASE + 6)   /* UART Receive FIFO             */
#define  UART_TFIFO     (UART_BASE + 6)   /* UART Transmit FIFO            */
#endif /* !T_MSM3 */
                                          /* Yes, they're the same address */
#if !defined (T_MSM3)
#define  UART_CR        (UART_BASE + 8)   /* UART Command Register         */
#endif /* !T_MSM3 */

#define  UART_CR_RESET_ERR 0x30           /* reset error status            */
#define  UART_CR_RESET_RX  0x10           /* reset receiver                */
#define  UART_CR_RX_ENA    0x01           /* enable receiver               */
#define  UART_CR_RESET_TX  0x20           /* reset transmitter             */
#define  UART_CR_TX_ENA    0x04           /* enable transmitter            */

#if !defined (T_MSM3)
#define  UART_ISR       (UART_BASE + 0x0a)/* UART Interrupt Status Register */
#endif /* !T_MSM3 */
#define  UART_ISR_TXLEV    0x01           /* tx FIFO at or below the mark  */

#if !defined (T_MSM3)
#define  UART_IMR       (UART_BASE + 0x0a)/* UART Interrupt Mask Register  */
#endif /* !T_MSM3 */
                                          /* Yes, they're the same address */
#if !defined (T_MSM3)
#define  UART_TXWAT     (UART_BASE + 0x0e)/* UART Transmitter low watermark */
#define  UART_RXWAT     (UART_BASE + 0x10)/* UART Receiver high watermark  */
#endif /* !T_MSM3 */

#if !defined (T_MSM3)
#define  UART_MVR       (UART_BASE + 0x14)/* UART Clock synth M value LSB  */
#endif /* !T_MSM3 */

#ifdef   UART_HALF_BAUD
#define  UART_MVR_V        0x60
#else
#define  UART_MVR_V        0xc0
#endif

#if !defined (T_MSM3)
#define  UART_NVR       (UART_BASE + 0x16)/* UART Clock synth N value LSB  */
#endif /* !T_MSM3 */

#ifdef   UART_HALF_BAUD
#define  UART_NVR_V        0x97
#else
#define  UART_NVR_V        0xaf
#endif

#if !defined (T_MSM3)
#define  UART_DVR       (UART_BASE + 0x18)/* UART Clock synth D value LSB  */
#endif /* !T_MSM3 */
#define  UART_DVR_V        0x80

#if !defined (T_MSM3)
#define  UART_MNDR      (UART_BASE + 0x1a)/* UART Clock synth M/N/D MSBs   */
#endif /* !T_MSM3 */
#define  UART_MNDR_V       0x19

#if !defined (T_MSM3)
#define MSMU_IRDA   (0x02F2)           /* UART IRDA Register               */
#endif /* !T_MSM3 */

/* For UART IRDA Register */
#define MSMU_IRDA_DISABLE    0x00      /* Disable UART IRDA transceiver    */

/* MSM 2.3 clock regime parameters */
#if !defined (T_MSM3)
#define CLK_RGM_CLEAR      0x00
#define CLK_RGM_ENABLE     0x00
#define CLK_RGM_UART_M     0x02         /* UART, TCXO/4 */
#endif /* !T_MSM3 */


/* macro for writes to MSM registers - Includes delay */
#define MSMU_OUT( reg, val) (void)outp( reg, val) //; __asm { orr r1,r1;orr r1,r1;orr r1,r1 }

/* macro for 16 bit writes to MSM registers */
#define MSMUW_OUT( reg, val) (void)outpw( reg, val) 

/*-------------------------------------------------------------------------*/

/* Selection of which receiver status bits we consider to be errors. */

#define  UART_SR_RX_ERROR  (UART_SR_BREAK_RXD |       \
                            UART_SR_PF_ERR    |       \
                            UART_SR_OVR_ERR )

/* Setting for TX Watermark.  Set this to 30 rather than 31, because
   of the known bug in the UART. */
#define  UART_TXWAT_VAL    30
