#ifndef SIORS232_H
#define SIORS232_H
/*===========================================================================

              S I O   D E V I C E   L A Y E R   H E A D E R 

DESCRIPTION
   This is the header file for the interface layer of SIO.  Therefore, this 
   header file provides the complete interface to SIO services.

   Copyright (c) 1998 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/siors232.h_v   1.0   04 Nov 1999 15:55:44   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/22/99   jwh     Changed SIODEV_H to SIORS232_H.
10/12/99   jwh     File name changed from siodev.h to siors232.h.  Changed 
                   extern function names from sio_RS2132... to sio_rs232... 
02/17/99   ak      added extern for function sio_rs232_flush_tx.
11/18/98   jjn     Added QMIP headers.
07/01/98   jjn     Created file.
===========================================================================*/

/*===========================================================================
 Include Files.
===========================================================================*/

#include "sio.h"                             /* SIO Server header file     */
#include "clk.h"                             /* Clock services header file */
#include "customer.h"                        /* Customer feature files     */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*===========================================================================
 FUNCTION/PROCEDURE prototypes.
===========================================================================*/              

/*===========================================================================

FUNCTION SIO_RS232_CHNG_BITRATE

DESCRIPTION
  This function changes the Bit Rate and does the necessary record keeping.
  
DEPENDENCIES
  Should only be called from sio_open.  open_ptr should be the same pointer
  which was passed to sio_open.
  
  Should be called before sio_enable_receiver or sio_enable_transmitter.
  
RETURN VALUE
  SIO_DONE_S: Bit-rate was set correctly.
  Currently, SIO just dies if requested bit-rate is invalid.  However, for
  devices other than RS-232 serial devices, this interface may return
  something other than SIO_DONE_S.
    
SIDE EFFECTS
  None
  
===========================================================================*/

extern sio_status_type sio_rs232_chng_bitrate
( 
  const sio_open_type       *open_ptr           /* Stream configuration    */
);                                    


/*===========================================================================

FUNCTION SIO_RS232_OPEN_STREAM

DESCRIPTION
  This function allocates stream as indicated in open_ptr.
    
DEPENDENCIES
  None
  
RETURN VALUE
  SIO_DONE_S: TX was successfully allocated.
  SIO_UNAVAIL_S: TX could not be allocated.  
  
SIDE EFFECTS
  None
  
===========================================================================*/

extern sio_status_type sio_rs232_open_stream
(
  const sio_open_type     *open_ptr
);


/*===========================================================================

FUNCTION SIO_RS232_CLOSE_STREAM

DESCRIPTION
  This function deallocates given stream.
    
DEPENDENCIES
  None
  
RETURN VALUE
  None
   
SIDE EFFECTS
  None
  
===========================================================================*/

extern void sio_rs232_close_stream
(
  sio_stream_id_type stream_id,             /* Stream ID                   */
  void             (*close_func_ptr)(void)  /* Function to call when 
                                               pending transmission is 
                                               complete                    */
);


/*===========================================================================

FUNCTION SIO_RS232_FLUSH_TX

DESCRIPTION
  This function flushes the TX.

DEPENDENCIES
 User should not queue more data until after the registered callback
 is executed.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void sio_rs232_flush_tx
(
  sio_stream_id_type stream_id,             /* Stream ID                   */
  sio_port_id_type   port_id,               /* Port ID                     */
  void             (*flush_func_ptr)(void)  /* Function to call when 
                                               pending transmission is 
                                               complete                    */
);


/*===========================================================================

FUNCTION SIO_RS232_DEV_INIT

DESCRIPTION
  Initialize the MSM UART serial interface for operation and initialize 
  data structures.

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

extern void sio_rs232_dev_init
(
  sio_port_id_type   port_id              /* SIO Port ID                   */  
);


/*===========================================================================

FUNCTION SIO_RS232_HW_TRANSMIT                   

DESCRIPTION
  This function transmits over MSMs RS-232 UART.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void sio_rs232_dev_transmit
(
  sio_stream_id_type stream_id,           /* Stream ID                     */
  sio_port_id_type   port_id,             /* SIO Port ID                   */
  dsm_item_type     *tx_ptr               /* Packet for transmission       */
);


/*===========================================================================

FUNCTION SIO_RS232_IOCTL                                EXTERNALIZED FUNCTION

DESCRIPTION
  Allows control of open stream, given that the device is an RS232 serial
  device.
    
DEPENDENCIES
  None
                                                  
RETURN VALUE
  None
    
SIDE EFFECTS

===========================================================================*/
void sio_rs232_ioctl
(
  sio_stream_id_type    stream_id,             /* Stream ID                */
  sio_port_id_type      port_id,               /* SIO Port ID              */
  sio_ioctl_cmd_type    cmd,                   /* Command                  */
  sio_ioctl_param_type *param                  /* Parameter                */
);


#ifdef T_MSM3
/*===========================================================================

FUNCTION SIO_RS232_DISABLE_DEVICE                       EXTERNALIZED FUNCTION

DESCRIPTION
  This routine disables given device, including turning off clock to save
  power.  This is different from UART powerdown feature which attempts to
  save power with minimal loss of data.  Instead, this function is meant 
  for customers who may not want to use both UARTs.
  
  Effects of this function may be reversed by calling sio_rs232_dev_init.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void sio_rs232_disable_device
(
  sio_port_id_type   port_id              /* SIO Port ID                   */  
);
#endif /* #ifdef T_MSM3 */

#ifdef FEATURE_QMIP
#error code not present
#endif /* #ifdef FEATURE_QMIP */

//	by chos 00.03.19 for other module
#ifdef	FEATURE_SKT_DTRSENSE_DS
extern void (*siors_dtr_func_ptr)(boolean);
#else	//	#ifdef	FEATURE_SKT_DTRSENSE_DS
extern void (*siors_dtr_func_ptr)(void);
#endif	//	#ifdef	FEATURE_SKT_DTRSENSE_DS


// tasik 00.08.10
#ifdef	FEATURE_USE_IRDA

// define constants for IrDA interface
#define	SIO_PORT_IRDA		SIO_PORT_UART_MAIN
#define	IRDA_MSMU_RF		MSMU_RF
#define	IRDA_MSMU_SR		MSMU_SR
#define	IRDA_MSMU_CR		MSMU_CR

extern dsm_item_type		*sio_tx_current_wm_item_ptr[ SIO_PORT_MAX ];
extern dsm_watermark_type	*sio_tx_watermark_queue_ptr[ SIO_PORT_MAX];

// chos test 00.04.14
// PC 에서 Phone 으로 data 주는 속도 조정
//extern int	nIrDACTSCount;

void	siors_dtr_event_isr ( void );

void	StartStopIrDA ( void );
int		IrDA_SendTx ( dsm_item_type *pTx );
void	IrDA_rx_isr ( int lsap, int status );

boolean		IrDA_Check_Rx_Buffer ( void );
byte	IrDA_GetOneByte ( void );

#ifdef	FEATURE_IRDA_UPGRADE
void	IrDA_rx_Process ( void );
#endif	// #ifdef	FEATURE_IRDA_UPGRADE


#endif	// #ifdef	FEATURE_USE_IRDA
// finish chos

#endif /* SIORS232_H */

