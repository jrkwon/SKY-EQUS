#ifndef DLOADARM_H
#define DLOADARM_H
/*===========================================================================

          ARM Boot Downloader Header File

DESCRIPTION
  This file contains global declarations and external references
  for the boot block downloader facility specific to the ARM.

Copyright (c) 1990,1991,1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999           by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/



/*===========================================================================

                           EDIT HISTORY FOR FILE
  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/dloadarm.h_v   1.1   10 Sep 1999 18:10:28   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/11/99   kk      Merge from SS baseline.
03/17/99   ms      Renamed bb_uart_init to dload_uart_init
                   due to Virtual Boot Block changes.
12/15/98   jc      Merge from et0400
11/06/98   jtg     Added PACKED to enum for ARM port into mainline.
09/13/98   hcg     Removed some #defines - Moved to another file.
07/30/98   hcg     Implemented security feature.
07/26/98   hcg     Revised for coding standard, removed unused code
06/01/98   hcg     Ported from Flashprg by ptw

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Status Code Enumeration
   This lists the status result codes passed around in the program.

   This enum is used to index a table of response packets, so these
   values map exactly to possible responses. */

typedef enum
  {
  ACK,                    /* Success. Send an acknowledgement.           */
  NAK_INVALID_FCS,        /* Failure: invalid frame check sequence.      */
  NAK_INVALID_DEST,       /* Failure: destination address is invalid.    */
  NAK_INVALID_LEN,        /* Failure: operation length is invalid.       */
  NAK_EARLY_END,          /* Failure: packet was too short for this cmd. */
  NAK_TOO_LARGE,          /* Failure: packet was too long for my buffer. */
  NAK_INVALID_CMD,        /* Failure: packet command code was unknown.   */
  NAK_FAILED,             /* Failure: operation did not succeed.         */
  NAK_WRONG_IID,          /* Failure: intelligent ID code was wrong.     */
  NAK_BAD_VPP,            /* Failure: programming voltage out of spec    */
  NAK_VERIFY_FAILED,      /* Failure: readback verify did not match      */
  NAK_NO_SEC_CODE,        /* Failure: not permitted without unlock       */
  NAK_BAD_SEC_CODE        /* Failure: invalid security code              */
  } response_code_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/


/*===========================================================================

                           MACRO DEFINITIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

extern void boot_error(int error);
extern void boot_hw_powerdown(void);
extern void boot_powerdown_entry(void);

/*===========================================================================

FUNCTION process_packets

DESCRIPTION
  This function is the main loop implementing the DMSS Async Download
  Protocol.  It loops forever, processing packets as they arrive.

DEPENDENCIES
  All necessary initialization for normal CPU operation must have
  been performed, and the UART must have been initialized, before
  entering this function.

RETURN VALUE
  This function does not return.

SIDE EFFECTS
  None.

===========================================================================*/

extern void process_packets(void);

/*===========================================================================

FUNCTION do_write

DESCRIPTION
  This function writes a provided string of bytes into a specified
  block of RAM memory.

DEPENDENCIES
  Address checks assume that the maximum allowed block size
  (MAX_WRITE_SIZE) is smaller than the block of memory reserved
  for this program.

RETURN VALUE
  Returns a response_code_type enum indicating what happened.  This
  code can be used directly to select a response packet to send back.

SIDE EFFECTS
  None.

===========================================================================*/

extern response_code_type do_write
(
  byte  *buf,
    /* Pointer to the string of bytes to write */

  dword addr,
    /* 20-bit linear physical address of the first byte of memory to write */

  word  len
    /* Count of bytes to write */
);

/*===========================================================================

FUNCTION do_erase

DESCRIPTION
  This function erases a specified block of RAM memory.

DEPENDENCIES
  Address checks assume that the maximum allowed block size
  (MAX_WRITE_SIZE) is smaller than the block allocated for this
  program.

RETURN VALUE
  Returns a response_code_type enum indicating what happened.  This
  code can be used directly to select a response packet to send back.

SIDE EFFECTS
  None.

===========================================================================*/
extern response_code_type do_erase
(
  dword addr,
    /* 20-bit linear physical address of the first byte of memory to erase */

  dword len
    /* Count of bytes to erase */
);

/*===========================================================================

FUNCTION dload_uart_init

DESCRIPTION
  This function initializes the MSM 2.0 UART to 115,200 bps, 8N1, with
  no interrupts enabled but both receiver and transmitter running.

  Actually, at the moment this function implements the clock/2
  workaround for a clocking bug, so it gets 19,200 bps.

DEPENDENCIES
  If the transmit FIFO is not empty, the UART must be initialized enough
  for it to eventually empty itself, or else this routine will wait
  forever.

RETURN VALUE
  None.

SIDE EFFECTS
  The watchdog is reset.

===========================================================================*/

extern void dload_uart_init(void);


/*===========================================================================

FUNCTION uart_drain

DESCRIPTION
  This function waits for the last character in the UART's transmit
  FIFO to be transmitted.  This allows the caller to be sure that all
  characters are transmitted.

DEPENDENCIES
  If the transmit FIFO is not empty, the UART must be initialized enough
  for it to eventually empty itself, or else this routine will wait
  forever.

RETURN VALUE
  None.

SIDE EFFECTS
  The watchdog is reset.

===========================================================================*/

extern void uart_drain(void);


/*===========================================================================

FUNCTION uart_receive_byte

DESCRIPTION
  This function receives a single byte from the UART by polling.

DEPENDENCIES
  The UART must be initialized and enabled, or else this routine will
  wait forever.

RETURN VALUE
  If a character is received without error, returns the value of the
  character received.  If an error occurs, returns UART_RX_ERR.

SIDE EFFECTS
  The watchdog is reset.

===========================================================================*/

extern int uart_receive_byte(void);

/*===========================================================================

FUNCTION uart_transmit_byte

DESCRIPTION
  This function transmits a single byte through the UART.

DEPENDENCIES
  The UART transmitter must be initialized and enabled, or this routine
  will wait forever.

RETURN VALUE
  None.

SIDE EFFECTS
  The watchdog may be reset.

===========================================================================*/

extern void uart_transmit_byte
(
  byte  chr
    /* Character to be transmitted */
);

#endif /* DLOADARM_H */
