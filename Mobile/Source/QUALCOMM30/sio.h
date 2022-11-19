#ifndef SIO_H

#define SIO_H

/*===========================================================================

                S E R I A L    I / O    S E R V I C E S

                       H E A D E R    F I L E

DESCRIPTION
  This file contains types and declarations associated with the DMSS
  Serial I/O Services.

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999, 2000 by QUALCOMM, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   O:/src/asw/COMMON/vcs/sio.h_v   1.6   20 Feb 2001 16:42:04   dpendlet  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/19/01   dwp     Reordered sio_bitrate_type to coincide w/new
                   nv_sio_baudrate_type.
02/13/01   dwp     Removed sizing of sio_bitrate_type.
01/26/01   ych     Merged T-53/JCDMA features for SK.
11/27/00   dwp     Change multiple SIO_PORT_BT_RFCOMM's to SIO_PORT_BT_SPP.
                   Add SIO_PORT_BT_NA. Remove SIO_IOCTL_BT_ENABLE and
                   SIO_IOCTL_BT_DISABLE.
11/18/00   dwp     Added additional requirement of FEATURE_USB on port 
                   definition SIO_PORT_USB_MMC.
11/14/00   dwp     Added support for FEATURE_MMC which is used in support of
                   FEATURE_RUNTIME_DEVMAP.
09/05/00   dwp     Additional FEATURE_BT modifications.
07/27/00   dwp     Added support for FEATURE_USB_DIAG and FEATURE_BT.
07/20/00   jwh     Added support for SIO_IOCTL_GET_FLOW_CTL and
                   FEATURE_SIO_INT_LOCKOUT.
06/08/00   jwh     Update comments.
05/22/00   jwh     Changed sio_bitrate_type and sio_port_id_type enum
                   featurization to allow compile time checking and keep
                   numeric values consistent for NV. Add sio_is_baud_valid().
04/15/00   jwh     Add SIO_IOCTL_CHANGE_BAUD_NOW, clarify operation of 
                   SIO_IOCTL_CHANGE_BAUD_RATE.
04/11/00   jwh     Make SIO_BITRATE_230400 defn unconditional.
02/16/00   jwh     Removed support for OLD SIO.  Added support for 230,400
                   baud for MSM5000.
09/09/99   jwh     Changes to NEW SIO API: sio_port_id_type name changes and
                   now #ifdef'd with FEATUREs; deleted sio_priority_type and
                   sio_bandwidth_type and the four associated (unused) 
                   members of sio_open_type; added SIO_IOCTL_FLUSH_TX.  Also
                   improved some comments.  Merged with MSM3100 1.0C.
08/05/99   mk      Added support for old SIO if T_MSM31 defined.
04/25/99   ak      Added ioctl def's to support mode change and baudrate 
                   change via an ioctl, versus using open/close.
04/09/99   jwh     Made sio_hdr_type typedef PACKED to avoid ARM 2.5 error.
02/17/99   ak      Added extern for sio_tx_flush.
01/17/99   ak      Featurized file (after INCLUDE FILES section), based on
                   whether user wants old or new SIO.  (FEATURE_NEW_SIO)
06/08/98   jjn     Exports new partitioned SIO.
10/29/97   jjn     Implemented DM Enter and DM Exit
10/10/97   grl     Allowed customerization for DM
09/09/97   ldg     (jjn) Changed and renamed flow control functions.
06/16/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
01/20/97   jjn     Implemented 617 transparency in SIO
05/24/96   jjw     Gemini & ISS2 Data Services release.
04/23/96   rdh     Added support for 115.2K DM.
11/09/95   jjw     Completed changes to support fully compliant IS-99
08/18/95   jmk     Took out T_SP conditional compile.
08/02/95   jmk     Added T_SP conditional compilation (exclude rex, protos, etc)
07/21/95   jjw     Fixed boo-boo within #ifdef DS definitions
07/13/95   jjw     Changes to support IS-99 protocol integration
03/23/95   rdh     Added ISS1 to sio_baud_clock_ctl prototype.
01/22/95   jjw     Added 2nd generation definitions and function prototypes
05/25/94   jjw     Made change to Ring buffer type parameter
05/18/94   jjw     Added changes for DM multi-drop
08/19/93   jjw     Modfied some existing function names, added all Data 
                   services functionality
12/07/93   jah     Added prototype for sio_baud_clock_ctl().
06/07/93   jjw     Made code review changes, added some more changes for Beta
                   II initial functionality
05/14/93   jjw     Added T_B2 compile-time directive for Beta II target
05/11/93   jjw     Cleaned up for code review, phase 1
03/12/93   jjw     Added typedef for Async. SIO channel A usage.
01/28/92   jah     Improved comments before passing this module to Jim Willkie.
08/25/92   jah     Improved error logging to differentiate errors more clearly.
07/23/92   jah     Added comments.
06/22/92   jah     Ported from brassboard, removed async.

===========================================================================*/

/*===========================================================================

                        INCLUDE FILES

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.                */
#include "queue.h"      /* Queue declaration package                       */
#include "rex.h"        /* REX multitasking package                        */
#include "target.h"     /* Target specific definitions                     */
#include "customer.h"   /* Customer Specific Definitions                   */
#include "dsm.h"        /* Data service buffer definition                  */
#include "ds.h"         /* Data Services Interface                         */


#ifdef FEATURE_NEW_SIO

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
 SIO flow control types.
---------------------------------------------------------------------------*/

typedef enum
{
  SIO_FCTL_BEST = 0,                   /* Best Flow control method.  Can
                                          mean default or no change.     */
  SIO_FCTL_OFF,                        /* Flow control disabled          */
  SIO_XONXOFF_STRIP_FCTL_FS,           /* Use fail-safe XON/XOFF flow 
                                          control but strip XON/XOFF 
                                          characters from stream         */
  SIO_XONXOFF_STRIP_FCTL_NFS,          /* Use non-failsafe XON/XOFF flow
                                          control but strip XON/XOFF 
                                          characters from stream         */
  SIO_CTSRFR_FCTL,                     /* Use CTS/RFR flow control       */
  SIO_XONXOFF_NSTRIP_FCTL_FS,          /* Use fail-safe XON/XOFF flow 
                                          control and leave in stream    */
  SIO_XONXOFF_NSTRIP_FCTL_NFS,         /* Use non-failsafe XON/XOFF flow 
                                          control and leave in stream    */                                        
  SIO_MAX_FLOW                         /* For bounds checking only       */
} sio_flow_ctl_type;


/*---------------------------------------------------------------------------
 SIO Stream Identifier type.  
---------------------------------------------------------------------------*/

typedef int2 sio_stream_id_type;

#define SIO_NO_STREAM_ID -1


/*---------------------------------------------------------------------------
 Maximum number of streams allowed.  At this time, only 2 simultaneous 
 streams are required.
---------------------------------------------------------------------------*/
#define SIO_MAX_STREAM 20

typedef enum    
{
  SIO_DS_AUTODETECT_MODE,          /* AutoDetect Mode (Backwards comp.)    */
  SIO_DS_RAWDATA_MODE,             /* RawData Mode (Backwards compatible)  */
  SIO_DS_PKT_MODE,                 /* Packet mode                          */
  SIO_DM_MODE,                     /* DM mode                              */
  SIO_GENERIC_MODE,                /* Generic Mode.  Most streams should 
                                      use this mode.                       */
  SIO_MAX_MODE                     /* Maximum Mode Value.                  */
} sio_stream_mode_type;


/*---------------------------------------------------------------------------
 SIO BITRATE type.  The unit of measure is bits per second.
 The "ILLEGAL" entries are to retain compile time range checking 
 while keeping the actual values invariant for NVRAM consistency.
---------------------------------------------------------------------------*/
/* !!!! Must keep siors232.c:sio_is_baud_valid() in sync with this. !!!! */
typedef enum     
{
#ifdef FEATURE_AUTOBAUD
  SIO_BITRATE_AUTO = 0,            /* Do AutoBaud.                         */
#else
  SIO_BITRATE_ILLEGAL_1,
#endif
#ifdef FEATURE_UART_TCXO_CLK_FREQ
  SIO_BITRATE_ILLEGAL_3,
  SIO_BITRATE_ILLEGAL_4,
  SIO_BITRATE_ILLEGAL_5,
  SIO_BITRATE_ILLEGAL_6,
  SIO_BITRATE_ILLEGAL_7,
#else
  SIO_BITRATE_45,
  SIO_BITRATE_50,
  SIO_BITRATE_75,
  SIO_BITRATE_110,
  SIO_BITRATE_150,
#endif 
  SIO_BITRATE_300,
  SIO_BITRATE_600,
  SIO_BITRATE_1200,                   
  SIO_BITRATE_2400,
  SIO_BITRATE_4800,
  SIO_BITRATE_9600,
#ifdef FEATURE_JCDMA_DS
#error code not present
#else
  SIO_BITRATE_ILLEGAL_8,
#endif
  SIO_BITRATE_19200,
  SIO_BITRATE_38400,
  SIO_BITRATE_57600, 
  #ifdef FEATURE_JCDMA_DS
#error code not present
#else
  SIO_BITRATE_ILLEGAL_9,
#endif
SIO_BITRATE_115200,
#ifdef FEATURE_UART_TCXO_CLK_FREQ
  SIO_BITRATE_230400,
#else
  SIO_BITRATE_ILLEGAL_2,
#endif 
  SIO_BITRATE_BEST = 0x7FFE,  /* Best bitrate (default, fastest, etc) */
  SIO_BITRATE_MAX = 0x7FFF    /* For bounds checking only             */
} sio_bitrate_type;


/*---------------------------------------------------------------------------
 SIO Port Identifier type.  This allows SIO operation over multiple ports.
 Only define sio_port_id_type enum values for ports that actually exist 
 (hence the #ifdef's below).
 The "ILLEGAL" entry(ies) are to retain compile time range checking
 while keeping the actual values invariant for NVRAM consistency.
---------------------------------------------------------------------------*/
typedef enum    
{
  SIO_PORT_NULL = -1,              /* Non-existent Port                    */
  SIO_PORT_UART_MAIN = 0,          /* Main UART port (RS232)               */
#ifdef FEATURE_SECOND_UART
  SIO_PORT_UART_AUX,               /* Auxiliary UART port (RS232)          */
#else
  SIO_PORT_ILLEGAL_1,              /* Auxiliary UART port placeholder      */
#endif 
#ifdef FEATURE_USB_CDC_ACM
#error code not present
#else
  SIO_PORT_ILLEGAL_2,              /* USB CDC ACM placeholder              */
#endif
#ifdef FEATURE_USB_DIAG
#error code not present
#else
  SIO_PORT_ILLEGAL_3,              /* DIAG on USB placeholder              */
#endif
#if defined(FEATURE_USB) && defined(FEATURE_MMC)
#error code not present
#else
  SIO_PORT_ILLEGAL_4,              /* MMC on USB placeholder               */
#endif
#ifdef FEATURE_BT
#error code not present
#else
  SIO_PORT_ILLEGAL_5,              /* BlueTooth placeholders               */
#endif
#if defined(FEATURE_BT) && defined(FEATURE_BT_NA)
#error code not present
#else
  SIO_PORT_ILLEGAL_6,              /* BlueTooth placeholders               */
#endif
#ifdef FEATURE_QMIP
#error code not present
#endif 
  SIO_PORT_MAX                     /* For bounds checking only             */
} sio_port_id_type;


/*---------------------------------------------------------------------------
 Identifier for stream which is opened.
---------------------------------------------------------------------------*/
typedef int2 stream_id_type;


/*---------------------------------------------------------------------------
 Type of stream which is being opened.
---------------------------------------------------------------------------*/

typedef enum
{
  SIO_STREAM_RX_ONLY = 0,                      /* Stream only used for RX  */
  SIO_STREAM_TX_ONLY,                          /* Stream only used for TX  */
  SIO_STREAM_RXTX,                             /* Stream used for RX & TX  */
  SIO_STREAM_MAX                               /* Used for bounds checking */
} sio_stream_type;




/*---------------------------------------------------------------------------
 Function pointer type for use by SIO RX ISR to pass to caller incoming data
 packaged in DSM item.
---------------------------------------------------------------------------*/

typedef void (*sio_rx_func_ptr_type)( dsm_item_type **);

/*---------------------------------------------------------------------------
 SIO Open Type.  A variable of this type needs to be passed to sio_open.
 sio_open will not modify any of the parameters (hence declaration of
 parameter to sio_open as "const").
---------------------------------------------------------------------------*/

typedef struct
{
  sio_stream_id_type    stream_id;          /* Stream ID Type.  Filled in
                                               by SIO for internal use.    */
  sio_stream_type       stream_type;        /* Type of stream being opened.
                                               Filled in by SIO for 
                                               internal use only.          */
  sio_stream_mode_type  stream_mode;        /* Stream Mode Type            */
  dsm_watermark_type   *rx_queue;           /* Received Data Queue         */
  dsm_watermark_type   *tx_queue;           /* Transmit Data Queue         */
  sio_bitrate_type      rx_bitrate;         /* Bit-rate for reception      */
  sio_bitrate_type      tx_bitrate;         /* Bit-rate for transmission   */
  sio_port_id_type      port_id;            /* Port which needs to be used */
  boolean               tail_char_used;     /* Is tail character used ?    */
  
  byte                  tail_char;          /* If tail character is used,
                                               this is the tail character. */

  sio_rx_func_ptr_type  rx_func_ptr;        /* If non-NULL, this function 
                                               will be called for each 
                                               packet which is received.   */
  sio_flow_ctl_type     tx_flow;            /* TX flow control method      */
  sio_flow_ctl_type     rx_flow;            /* RX flow control method      */
} sio_open_type;


/*---------------------------------------------------------------------------
 SIO Status type.  Indicates the different results of SIO operation.
---------------------------------------------------------------------------*/

typedef enum
{
  SIO_DONE_S=0,                    /* Done                                 */
  SIO_BADP_S,                      /* Bad parameter                        */
  SIO_UNAVAIL_S,                   /* No more streams can be allocated.    */
  SIO_BUSY_S,                      /* Command is still being processed.    */
  SIO_RX_NOT_OPEN_S,               /* RX was not opened for this stream    */
  SIO_TX_NOT_OPEN_S,               /* TX was not opened for this stream    */
  SIO_BITRATE_CHANGE_FAILED_S,     /* Attempt to change bitrate failed     */
  SIO_MAX_S                        /* For bounds checking only             */
} sio_status_type;


/*---------------------------------------------------------------------------
 SIO IOCTL command type.  These are the commands which can be carried out by
 sio_ioctl.
---------------------------------------------------------------------------*/

typedef enum
{
  SIO_IOCTL_CD_ASSERT=0,             /* Assert Carrier Detection Indicator   */
  SIO_IOCTL_CD_DEASSERT,             /* Deassert Carrier Detection Indicator */
  SIO_IOCTL_RI_ASSERT,               /* Assert Ring Indication               */
  SIO_IOCTL_RI_DEASSERT,             /* Dassert Ring Indication              */
  SIO_IOCTL_ENABLE_DTR_EVENT,        /* Register clock call-back which 
                                        informs caller of change in state of
                                        data terminal ready indication       */  
  SIO_IOCTL_DISABLE_DTR_EVENT,       /* De-register clock call-back for
                                        informing of change in state of 
                                        data terminal ready indication       */
  SIO_IOCTL_INBOUND_FLOW_ENABLE,     /* Enable inbound flow using current
                                        flow control method                  */
  SIO_IOCTL_INBOUND_FLOW_DISABLE,    /* Disable inbound fow using current
                                        flow control method                  */
  SIO_IOCTL_DSR_ASSERT,              /* Assert Data Set Ready Indication     */
  SIO_IOCTL_DSR_DEASSERT,            /* Deassert Data Set Ready Indication   */
  SIO_IOCTL_DTE_READY_ASSERTED,      /* Return a boolean (in paramter) which
                                        indicates whether or not DTE ready
                                        signal is asserted                   */
  SIO_IOCTL_SET_FLOW_CTL,            /* Allows setting of flow control 
                                        method                               */
  SIO_IOCTL_GET_FLOW_CTL,            /* Allows getting the flow control
                                          methods                            */
  SIO_IOCTL_ENABLE_AUTODETECT,       /* Enable AutoDetect.  Has no effect if
                                        AutoDetect already enabled.          */
  SIO_IOCTL_RECORD_ESC_FUNC_PTR,     /* Register clock call-back function
                                        which informs caller of arrival of
                                        escape code ( +++ )                  */
  SIO_IOCTL_RECORD_PKT_FUNC_PTR,     /* Register clock call-back function
                                        whcih informs caller of arrival of
                                        PPP packet                           */
  SIO_IOCTL_BEGIN_FAX_IMAGE,         /* Used by caller to signal beginning
                                        of fax image.  From the beginning of
                                        fax image to the end of fax image on
                                        given line, software flow control 
                                        needs to be ignored because they 
                                        may be unescaped data bytes          */
  SIO_IOCTL_CHANGE_BAUD_RATE,        /* Used to change the baud rate of the  
                                        serial port.  THIS IS FOR BACKWARD
                                        COMPATIBILITY WITH DATA SERVICES ONLY.
                                        The change takes place _only_  _after_
                                        a transition from a non-empty TX FIFO
                                        to an empty TX FIFO. If the TX FIFO
                                        is empty when this ioctl is received, 
                                        no baud change occurs until more data 
                                        is Tx'd.                             */
  SIO_IOCTL_CHANGE_BAUD_NOW,         /* Used to change the baud rate of the
                                        serial port.  Change takes place 
                                        immediately without regard to any data
                                        in the TX FIFO. Hence it is up to the 
                                        app to flush the tx beforehand. The 
                                        usage is otherwise identical to 
                                        SIO_IOCTL_CHANGE_BAUD_RATE.          */
  SIO_IOCTL_FLUSH_TX,                /* Register call-back function which
                                        informs caller when all pending
                                        output data has been transmitted
                                        WARNING: Unreliable results may occur
                                        if data is queued after this function
                                        call is made and before the callback
                                        function has been called.            */
  SIO_IOCTL_CHANGE_MODE              /* Change the serial port's mode, i.e.,
                                        from AUTODETECT to RAWDATA for
                                        example                              */
} sio_ioctl_cmd_type;


/*---------------------------------------------------------------------------
 SIO IOCTL parameter types.  These are to be used in conjunction with the 
 above command type.
---------------------------------------------------------------------------*/

/* Default parameter to be used with ioctl command.  Should only be used as
   place-holder for operations which do not require parameters.            */
typedef word sio_ioctl_default_param_type;

/* Flow control type.  Need to specify TX flow and RX flow control methods.
   Because standard allows setting of these methods separately, SIO
   interface has to do so as well.                                         */  
typedef struct sio_ioctl_flow_ctl {
  sio_flow_ctl_type tx_flow;
  sio_flow_ctl_type rx_flow;
} sio_ioctl_flow_ctl_type;

/*---------------------------------------------------------------------------
 Function pointer type which points to a function that takes no arguments
 and returns nothing (i.e. void).
---------------------------------------------------------------------------*/

typedef void (*sio_vv_func_ptr_type)( void);
#ifdef	FEATURE_SKT_DTRSENSE_DS	//	khekim 00/02/22
typedef void (*sio_vv_param_func_ptr_type)( boolean);
#endif	//	#ifdef	FEATURE_SKT_DTRSENSE_DS

/*---------------------------------------------------------------------------
  Aggregate SIO IOCTL parameter type.
---------------------------------------------------------------------------*/

typedef union sio_ioctl_param {
  sio_ioctl_default_param_type   carrier_detect_assert;
  sio_ioctl_default_param_type   carrier_detect_deassert;
  sio_ioctl_default_param_type   ring_indicator_assert;
  sio_ioctl_default_param_type   ring_indicator_deassert;
#ifdef	FEATURE_SKT_DTRSENSE_DS	//	khekim 00/02/22
  sio_vv_param_func_ptr_type     enable_dte_ready_event;
#else	//	#ifdef	FEATURE_SKT_DTRSENSE_DS	//	khekim 00/02/22
  sio_vv_func_ptr_type           enable_dte_ready_event;
#endif	//	#ifdef	FEATURE_SKT_DTRSENSE_DS	//	khekim 00/02/22
  sio_ioctl_default_param_type   disable_dte_ready_event;
  sio_ioctl_default_param_type   inbound_flow_enable;
  sio_ioctl_default_param_type   inbound_flow_disable;
  sio_ioctl_default_param_type   data_set_ready_assert;
  sio_ioctl_default_param_type   data_set_ready_deassert;
  boolean                       *dte_ready_asserted;
  sio_ioctl_flow_ctl_type        flow_ctl;
  sio_ioctl_default_param_type   rearm_autobaud;
  sio_vv_func_ptr_type           record_escape_func_ptr;
  sio_vv_func_ptr_type           record_pkt_func_ptr;
  sio_ioctl_default_param_type   begin_fax_image;
  sio_bitrate_type               bitrate;
  sio_stream_mode_type           stream_mode;
  sio_open_type                 *open_ptr;
  sio_vv_func_ptr_type           record_flush_func_ptr;
} sio_ioctl_param_type;

/*---------------------------------------------------------------------------
 This is the function pointer type which will be passed to clk_reg.
---------------------------------------------------------------------------*/

typedef void (*sio_clk_routine_ptr_type)( int4 ms_interval);

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION SIO_INIT                                    EXTERNALIZED FUNCTION

DESCRIPTION
  Initializes SIO.  
  
DEPENDENCIES
  None
    
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/

extern void sio_init
(
  void
);


/*===========================================================================

FUNCTION SIO_CONVERT_FCTL                           EXTERNALIZED FUNCTION

DESCRIPTION
  This function will convert flow control values dictated by standard to
  SIO flow control values.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

extern sio_flow_ctl_type sio_convert_flow_ctl
( 
  ds_flow_ctl_type  flow_ctl,              /* Flow control method          */
  boolean           fail_safe              /* Fail safe                    */
);


/*===========================================================================

FUNCTION SIO_OPEN                           EXTERNALIZED FUNCTION

DESCRIPTION
  This function opens a stream across the serial link.  
    
DEPENDENCIES
  sio_init must have been called.  sio_init needs to be called by just one
  task and only once upon power-up.
      
RETURN VALUE
  SIO_NO_STREAM_ID:  The open request could not be completed. (ERROR)

  If successful, the "stream_id" is returned for use in future SIO requests.
    
SIDE EFFECTS
  None
  
===========================================================================*/

extern sio_stream_id_type sio_open
( 
  sio_open_type *open_ptr                      /* Configurations of stream */
);


/*===========================================================================

FUNCTION SIO_CLOSE                           EXTERNALIZED FUNCTION

DESCRIPTION
  This procedure does all the book-keeping necessary to close a stream.  
  Optional tcb_ptr and sigs can be passed to this function, if task wants
  to be signalled when the last bit of pending transmission leaves phone.
                  
DEPENDENCIES
  None
      
RETURN VALUE
  None
  
SIDE EFFECTS
  close_func_ptr will be called when all pending data has left transmitter.
  
===========================================================================*/

extern void sio_close
( 
  sio_stream_id_type stream_id,               /* Stream ID                 */
  void             (*close_func_ptr)(void)    /* Function to call when 
                                                 transmission is complete. */
);


/*===========================================================================

FUNCTION SIO_TRANSMIT                           EXTERNALIZED FUNCTION

DESCRIPTION
  This function will transmit a packet or if transmitter is busy, schedule
  the transmission of a packet.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/

extern void sio_transmit
( 
  sio_stream_id_type stream_id,    /* Stream ID                            */
  dsm_item_type *tx_ptr            /* Packet which needs to be transmitted */
);


/*===========================================================================

FUNCTION SIO_FLUSH_TX                           EXTERNALIZED FUNCTION

DESCRIPTION
  This function will transmit any queued data and then call the registered
  callback.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/

extern void sio_flush_tx
( 
  sio_stream_id_type stream_id,               /* Stream ID                 */
  void             (*close_func_ptr)(void)    /* Function to call when 
                                                 transmission is complete. */
);


/*===========================================================================

FUNCTION SIO_IOCTL                                      EXTERNALIZED FUNCTION

DESCRIPTION
  Allows control of open stream.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

void sio_ioctl
(
  sio_stream_id_type    stream_id, /* Stream ID                            */ 
  sio_ioctl_cmd_type    cmd,       /* IOCTL command                        */
  sio_ioctl_param_type *param      /* Parameter to be used for command     */
);


/*===========================================================================

FUNCTION SIO_DO_ESCAPING                                EXTERNALIZED FUNCTION

DESCRIPTION
  This routine provides XOR escaping functionality for the use of callers.
  Both Control character and Escape character will be escaped by substituting
  the character with the following two characters:
  
  <escape_char> <src_byte ^ escape_mask>

DEPENDENCIES
  src_ptr & dest_ptr cannot be the same nor overlapping.
  
RETURN VALUE
  Returns the number of bytes that were stored into dest_ptr.
  
  Also, via pointers, returns src_used.
    
SIDE EFFECTS
  None

===========================================================================*/

extern word sio_do_escaping
( 
  byte        control_char,
  /* This is the character which is being escaped
  */

  byte        escape_char,
  /* This is the character which will indicate that escaping was 
  ** performed
  */

  byte        escape_mask,
  /* This is the mask with which the escaping is performed.  
  ** For instance, if control_char is found, the escaped version 
  ** is escape_char control_char ^ escape_mask .
  */

  const byte *src_ptr, 
  /* This is the pointer that contains the data that needs to be 
  ** escaped
  */

  byte       *dest_ptr, 
  /* This is the pointer into which the escaped data should be 
  ** stored.
  */

  word        src_length, 
  /* This is the number of bytes to convert.
  */

  word        dest_length,
  /* This is the number of bytes that can be stored in dest_ptr.
  */

  word       *src_used
  /* This will contain the number of values which were read from 
  ** src_ptr. 
  */
);


/*===========================================================================

FUNCTION SIO_DO_UNESCAPING                              EXTERNALIZED FUNCTION

DESCRIPTION
  This routine provides XOR unescaping functionality for the use of callers.
  This routine is the inverse of sio_do_unescaping.

  When escape_char is found, it is discarded and the next character is 
  unescaped as follows <src_byte ^ escape_mask>.  This character is stored
  in dest_ptr.
  
  When an unescaped control_char is found, this routine will exit immediately 
  and indicate that an unescaped control character was found, in the parameter
  found_control_char.
  
  Source and destination can be the same or overlapping.
  
DEPENDENCIES
  When an escape character straddles two packets, escaping will indicate this
  fact.  The caller needs to pass this parameter (unscathed) to 
  sio_do_unescaping in order for un-escaping to be done correctly.
  
RETURN VALUE
  Returns the number of bytes that were unescaped from src_ptr.
  
  Also, via pointers, found_control_char, escaping and src_used are returned.
    
SIDE EFFECTS
  None
  
===========================================================================*/

extern word sio_do_unescaping
( 
  byte        control_char,
  /* This is the character which was escaped.
  */

  byte        escape_char,
  /* This is the character which indicates that escaping was performed.
  */

  byte        escape_mask,
  /* This is the mask with which the escaping is performed.  For instance, 
  ** if control_char is found, the escaped version is 
  ** <escape_char><control_char ^ escape_mask>.
  */

  const byte *src_ptr, 
  /* This is the pointer that contains the data that needs to be un-escaped.
  */

  byte       *dest_ptr, 
  /* This is the pointer into which the un-escaped data should be stored.
  */

  word        src_length, 
  /* This is the number of bytes to convert from src_ptr.
  */

  word        dest_length,
  /* This is the number of bytes that can be stored in dest_ptr.
  */

  boolean    *found_control_char,
  /* This boolean indicates whether or not a Control character was found.  
  */

  boolean    *escaping,
  /* This boolean is set to true if this routine is in the middle of 
  ** un-escaping a byte at the end of the src_ptr.  This boolean needs
  ** to be handed back to this routine untouched.  The first time this 
  ** routine is called for a given "packet", this boolean needs to have 
  ** been set to FALSE.
  */

  word       *src_used
  /* This will contain the number of values which were read from src_ptr. 
  */
);

/*===========================================================================

FUNCTION SIO_GET_STREAM_ID                                  REGIONAL FUNCTION

DESCRIPTION
  Return stream ID which is not currently allocated (i.e. opened).  
  INTLOCK_SAV statement and INTFREE_SAV statement makes this function
  re-entrant.
      
DEPENDENCIES
  None
                                                       
RETURN VALUE
  First available stream ID. 
  If no stream identifiers are available, return SIO_NO_STREAM_ID.
    
SIDE EFFECTS
  None
  
===========================================================================*/

sio_stream_id_type sio_get_stream_id( void);

/*===========================================================================

FUNCTION SIO_PUT_STREAM_ID                                  REGIONAL FUNCTION

DESCRIPTION
  Allows caller to deallocate or put back stream ID previously allocated.
  INTLOCK_SAV & INTFREE_SAV statements make this procedure re-entrant.
      
DEPENDENCIES
  None
                                                       
RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/

void sio_put_stream_id( stream_id_type stream_id);

/*===========================================================================

FUNCTION SIO_STREAM_OPEN                                    REGIONAL FUNCTION

DESCRIPTION
  Allows caller to ascertain whether or not stream is open.
      
DEPENDENCIES
  None
                                                       
RETURN VALUE
  TRUE:  If stream open
  FALSE: If stream is not open
  
SIDE EFFECTS
  None
  
===========================================================================*/

boolean sio_stream_open( stream_id_type stream_id);

/*===========================================================================

FUNCTION SIO_INITIALIZE_STREAM_OPENED                       REGIONAL FUNCTION

DESCRIPTION
  Allows caller to initialize data structure used in indicating whether or
  not certain streams are open.
      
DEPENDENCIES
  None
                                                       
RETURN VALUE
  
SIDE EFFECTS
  None
  
===========================================================================*/

void sio_initialize_stream_opened( void);

/*===========================================================================

FUNCTION SIO_DISABLE_DEVICE                             EXTERNALIZED FUNCTION

DESCRIPTION
  This routine disables given device, including turning off clock to save
  power.  This is different from UART powerdown feature which attempts to
  save power with minimal loss of data.  Instead, this function is meant 
  for customers who may not want to use both UARTs.
  
  Effects of this function may be reversed by calling sio_init.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

extern void sio_disable_device
(
  sio_port_id_type   port_id              /* SIO Port ID                   */  
);

/*===========================================================================

FUNCTION SIO_IS_BAUD_VALID                             EXTERNALIZED FUNCTION

DESCRIPTION
  This routine checks if the specified baud rate is valid under the 
  current feature set.  

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the baud rate is valid under the current feature set.
  FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/

extern boolean sio_is_baud_valid
(
  sio_bitrate_type   check_baud              /* SIO baud rate              */
);


#ifdef FEATURE_SIO_INT_LOCKOUT
/*===========================================================================

FUNCTION SIORS_PREVENT_INTS

DESCRIPTION
  This function masks all MAIN UART interrupts to avoid critical encoder
  section.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void siors_prevent_ints
(
  void
);

#endif /* FEATURE_SIO_INT_LOCKOUT */

#ifdef FEATURE_SIO_INT_LOCKOUT
/*===========================================================================
FUNCTION SIORS_RESTORE_INTS

DESCRIPTION
  This function restores the MAIN UART interrupt mask to the stored shadow
  register.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void siors_restore_ints
(
  void
);

#endif /* FEATURE_SIO_INT_LOCKOUT */

#ifdef FEATURE_QMIP
#error code not present
#endif /* FEATURE_QMIP */


#else /* not FEATURE_NEW_SIO*/

/*
** The OLD SIO driver is no longer supported!
*/

#error Must define FEATURE_NEW_SIO

#endif /*FEATURE_NEW_SIO*/

#endif /* SIO_H */

