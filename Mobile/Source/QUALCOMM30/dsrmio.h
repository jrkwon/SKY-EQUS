#ifndef DSRMIO_H
#define DSRMIO_H
/*===========================================================================

                     DSRMIO  HEADER  FILE

DESCRIPTION
  This file defines MACRO which specify the interface between the
  DATA SERVICES task and the drivers which provide the external 
  interface.  This is the Rm interface specified in IS-707 when
  the serial port driver is used.  It is a data stream interface
  when the SSS modem driver is used (there are no commands).
  
  This file defines a series of macros which expand into function
  calls.  Depending on the FEATURE_ANALOG_DATA switch, the functions
  either interface the serial driver (sio_ functions) or the modem
  driver (ecfm_ functions).

Copyright (c) 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999,2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/dsrmio.h_v   1.2   23 Feb 2001 09:45:48   dpendlet  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/19/01   dwp     Rename DS_DISABLE_DATA_MODE_CMD to DS_DISABLE_SIO_CMD.
01/25/01   ych/na  Removed FEATURE_NEW_SIO - it is always turned on now.
04/25/99   ak      Changed the set_autodetect, set_rawdata, and set_pkt calls
                   (under new sio), to instead use the new ioctl function call,
                   instead of the open/close paradigm.
04/15/99   smp     In DSRMIO_SET_AUTODETECT_MODE, set the rx & tx bitrates based
                   on the AT+IPR setting, instead of the ds_sio_autodetect_bitrate
                   variable.
01/17/99   ak      New SIO code under a compile time switch.
09/15/98   ces     add defintion DSRMIO_SET_SW_FC_FS when compiling QMIP
                   CR 6377.
09/15/98   tkuo    Add DM Macro Function
08/26/98   tkuo    Add FEATURE_QMIP for QMIP Data Service
05/29/98   dbd     Changed macro names from ASYNCxxx to DSRMIOxxx
03/31/98   dbd     Added #include customer.h
03/24/98   dbd     Initial version of file - split off from dsec.h


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "ds.h"
#include "sio.h"
#include "dsmgr.h"
#ifdef FEATURE_ANALOG_DATA
#error code not present
#endif
#ifdef FEATURE_QMIP
#error code not present
#endif
 
/*===========================================================================

                     MACRO DEFINITIONS FOR RM INTERFACE
 
  The following macros call functions for both sio_ and ecfm_:
  
DSRMIO_DS_RX_CANCEL()                              
DSRMIO_DS_TX_CANCEL()                              
DSRMIO_INBOUND_FLOW_DISABLE()                              
DSRMIO_INBOUND_FLOW_ENABLE()                              
DSRMIO_SETUP_DS_RX(rx_watermark_ptr,pkt_sig_mask,escape_sig_mask)
DSRMIO_SETUP_DS_TX(tx_ptr,rearm_autodetect)         
DSRMIO_SET_RAWDATA_MODE(tx_wm_ptr,rx_wm_ptr)           

  The following macros call functions for sio_ 
  and return fixed values for ecfm_:
  
DSRMIO_DTR_ASSERTED() // returns TRUE always for ecfm                             
                  
  The following macros call functions for sio_ and generate no code for ecfm:                      

DSRMIO_DCD_ASSERT()                       
DSRMIO_DCD_DEASSERT() 
                      
DSRMIO_DISABLE_DTR_EVENT()                       
DSRMIO_ENABLE_DTR_EVENT(sig_mask)  
          
DSRMIO_DSR_ASSERT()                       
DSRMIO_DSR_DEASSERT()   
                    
DSRMIO_DS_SET_FLOW_CTL(tx_flow,rx_flow)
        
DSRMIO_RI_ASSERT()                       
DSRMIO_RI_DEASSERT() 
                      
DSRMIO_SCHEDULE_DM_ENTER()                       
DSRMIO_SCHEDULE_DM_EXIT()  
                     
DSRMIO_SET_AUTODETECT_MODE()                       
DSRMIO_SET_DM_MODE(baudrate)               
DSRMIO_SET_PKT_MODE()                       

     

===========================================================================*/

/*===========================================================================

                     These macros control the flow
                     of data to / from the io device

===========================================================================*/

/*-------------------------------------------------------------
 DSRMIO_DS_RX_CANCEL
--------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_DS_RX_CANCEL() \
  /* Do Nothing. Act of closing stream takes care of this  */

#endif /* ds_rx_cancel */


/*--------------------------------------------------------------
 DSRMIO_DS_TX_CANCEL
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_DS_TX_CANCEL() \
  /* Do Nothing.  Act of closing stream takes care of this  */

#endif /* ds_tx_cancel */

/*---------------------------------------------------------------
    DSRMIO_INBOUND_FLOW_DISABLE
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_INBOUND_FLOW_DISABLE(dsi_stream_id, param) \
  sio_ioctl ((dsi_stream_id), SIO_IOCTL_INBOUND_FLOW_DISABLE, &(param))

#endif /* FEATURE_ANALOG_DATA*/

/*---------------------------------------------------------------
    DSRMIO_INBOUND_FLOW_ENABLE
---------------------------------------------------------------*/
#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_INBOUND_FLOW_ENABLE(dsi_stream_id, param) \
  sio_ioctl( (dsi_stream_id), SIO_IOCTL_INBOUND_FLOW_ENABLE, &(param))

#endif /*FEATURE_ANALOG_DATA*/

/*---------------------------------------------------------------
    DSRMIO_SETUP_DS_RX
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_SETUP_DS_RX(rx_watermark_ptr, pkt_sig_mask, escape_sig_mask)\
  /* Do Nothing.  Act of opening stream takes care of this. */

#endif /* FEATURE_ANALOG_DATA */


/*---------------------------------------------------------------
    DSRMIO_SETUP_DS_TX
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_SETUP_DS_TX(dsi_stream_id, tx_ptr, rearm_autodetect, param) \
  if ( rearm_autodetect)                                                     \
  {                                                                          \
     sio_ioctl ((dsi_stream_id), SIO_IOCTL_ENABLE_AUTODETECT, &(param));     \
  }                                                                          \
  sio_transmit ((dsi_stream_id), (tx_ptr));                             
#endif /*FEATURE_ANALOG_DATA*/

/*===========================================================================

                     These macros control RS-232 protocol
                     functions for the serial port
                     and NO-OP for the DSP

===========================================================================*/

/*  *** Carrier detect pin ***  */
/*---------------------------------------------------------------
    DSRMIO_DCD_ASSERT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else

  #define DSRMIO_DCD_ASSERT(dsi_stream_id, param) \
  sio_ioctl( (dsi_stream_id), SIO_IOCTL_CD_ASSERT, &(param))
    
#endif /*FEATURE_ANALOG_DATA*/

/*---------------------------------------------------------------
    DSRMIO_DCD_DEASSERT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else

  #define DSRMIO_DCD_DEASSERT(dsi_stream_id, param) \
  sio_ioctl( (dsi_stream_id), SIO_IOCTL_CD_DEASSERT, &(param))

#endif /* FEATURE_ANALOG_DATA */

/*  *** DTR pin *** */
/*---------------------------------------------------------------
    DSRMIO_DISABLE_DTR_EVENT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_DISABLE_DTR_EVENT(dsi_stream_id, param) \
  sio_ioctl( (dsi_stream_id), SIO_IOCTL_DISABLE_DTR_EVENT, &(param));

#endif /* FEATURE_ANALOG_DATA */

/*---------------------------------------------------------------
    DSRMIO_ENABLE_DTR_EVENT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_ENABLE_DTR_EVENT(dsi_stream_id, sig_mask, func_ptr, param) \
  param.enable_dte_ready_event = func_ptr;                                  \
  sio_ioctl( (dsi_stream_id), SIO_IOCTL_ENABLE_DTR_EVENT, &(param));

#endif /* FEATURE_ANALOG_DATA */

/*---------------------------------------------------------------
    DSRMIO_DTR_ASSERTED
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else

  #define DSRMIO_DTR_ASSERTED(dsi_stream_id, param, return_val) \
  param.dte_ready_asserted = &(return_val);                     \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_DTE_READY_ASSERTED, &(param));

#endif /*FEATURE_ANALOG_DATA*/

/*---------------------------------------------------------------
    DSRMIO_DS_SET_FLOW_CTL
---------------------------------------------------------------*/
/*  *** Set hardware or XON / XOFF flow control *** */

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_DS_SET_FLOW_CTL(dsi_stream_id, tx_flow, rx_flow,            \
                                 tx_flow_ctl_ptr, rx_flow_ctl_ptr, fail_safe,\
                                 param)                                      \
  *(tx_flow_ctl_ptr) = sio_convert_flow_ctl( (tx_flow), (fail_safe));        \
  *(rx_flow_ctl_ptr) = sio_convert_flow_ctl( (rx_flow), (fail_safe));        \
  sio_ioctl ((dsi_stream_id), SIO_IOCTL_SET_FLOW_CTL, &param) 

#endif /* FEATURE_ANALOG_DATA */

/*  *** DSR pin *** */
/*---------------------------------------------------------------
    DSRMIO_DSR_ASSERT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else

  #define DSRMIO_DSR_ASSERT(dsi_stream_id, param) \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_DSR_ASSERT, &(param));

#endif /* FEATURE_ANALOG_DATA */

/*---------------------------------------------------------------
    DSRMIO_DSR_DEASSERT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else

  #define DSRMIO_DSR_DEASSERT(dsi_stream_id, param) \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_DSR_DEASSERT, &(param));

#endif /* FEATURE_ANALOG_DATA */

/*  *** Ring indicator  *** */
/*---------------------------------------------------------------
    DSRMIO_RI_ASSERT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else

  #define DSRMIO_RI_ASSERT(dsi_stream_id, param) \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_RI_ASSERT, &(param));

#endif /* FEATURE_ANALOG_DATA */

/*---------------------------------------------------------------
    DSRMIO_RI_DEASSERT
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#else

  #define DSRMIO_RI_DEASSERT(dsi_stream_id, param) \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_RI_DEASSERT, &(param));

#endif /* FEATURE_ANALOG_DATA */

/*===========================================================================

                     These macros control the serial port
                     mode and data stream.  NO-OP for DSP

===========================================================================*/

/*---------------------------------------------------------------
    ASYNC_SCHEDULE_DM_EXIT
---------------------------------------------------------------*/
#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

#define ASYNC_SCHEDULE_DM_EXIT() \
diag_close = TRUE

#endif /* sio_schedule_dm_exit */


/*---------------------------------------------------------------
    DSRMIO_SCHEDULE_DM_ENTER
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_SCHEDULE_DM_ENTER(ds_cmd_item)      \
  (ds_cmd_item).cmd_name = DS_DISABLE_SIO_CMD; \
  ds_cmd(&(ds_cmd_item))                                                         

#endif /* FEATURE_ANALOG_DATA */

/*---------------------------------------------------------------
    DSRMIO_SCHEDULE_DM_EXIT
---------------------------------------------------------------*/
#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

#define DSRMIO_SCHEDULE_DM_EXIT() \
diag_close = TRUE

#endif /* sio_schedule_dm_exit */

/*---------------------------------------------------------------
    DSRMIO_SET_AUTODETECT_MODE
---------------------------------------------------------------*/

#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_SET_AUTODETECT_MODE(isave, param)                        \
  INTLOCK_SAV(isave);                                                     \
                                                                          \
  ds_sio_open.stream_mode    = SIO_DS_AUTODETECT_MODE;                    \
  ds_sio_open.rx_queue       = &ds_from_sio_wm_item;                      \
  ds_sio_open.tx_queue       = &dsi_to_sio_pkt_autodt_wm_item;            \
  ds_sio_open.rx_func_ptr    = NULL;                                      \
  ds_sio_open.port_id        = ds_sio_port_id;                            \
  ds_sio_open.tail_char_used = FALSE;                                     \
  ds_sio_open.rx_flow        = SIO_FCTL_BEST;                             \
  ds_sio_open.tx_flow        = SIO_FCTL_BEST;                             \
                                                                          \
  ds_sio_open.rx_bitrate       = (sio_bitrate_type)ds_atcop_ipr_val;      \
  ds_sio_open.tx_bitrate       = (sio_bitrate_type)ds_atcop_ipr_val;      \
                                                                          \
  param.open_ptr             = &ds_sio_open;                              \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_CHANGE_MODE, &param);            \
                                                                          \
  param.record_pkt_func_ptr = dsi_set_pkt_signal;                         \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_RECORD_PKT_FUNC_PTR, &(param));  \
  INTFREE_SAV(isave)

#endif /* FEATURE_ANALOG_DATA */

/*---------------------------------------------------------------
    DSRMIO_SET_DM_MODE
---------------------------------------------------------------*/
#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

#define DSRMIO_SET_DM_MODE(baudrate) \
/* this function does not exist for normal sio*/

#endif /* sio_set_dm_mode */

/*---------------------------------------------------------------
    DSRMIO_SET_PKT_MODE
---------------------------------------------------------------*/
#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_SET_PKT_MODE( isave, param)                            \
  INTLOCK_SAV(isave);                                                   \
  ds_sio_open.stream_mode    = SIO_DS_PKT_MODE;                         \
  ds_sio_open.rx_queue       = &ds_from_sio_wm_item;                    \
  ds_sio_open.tx_queue       = &dsi_to_sio_pkt_autodt_wm_item;          \
  ds_sio_open.rx_func_ptr    = NULL;                                    \
  ds_sio_open.port_id        = ds_sio_port_id;                          \
  ds_sio_open.tail_char_used = FALSE;                                   \
  ds_sio_open.rx_flow        = SIO_FCTL_BEST;                           \
  ds_sio_open.tx_flow        = SIO_FCTL_BEST;                           \
                                                                        \
  ds_sio_open.rx_bitrate  = SIO_BITRATE_BEST;                           \
  ds_sio_open.tx_bitrate  = SIO_BITRATE_BEST;                           \
                                                                        \
  param.open_ptr             = &ds_sio_open;                            \
  sio_ioctl( (dsi_stream_id), SIO_IOCTL_CHANGE_MODE, &(param));         \
                                                                        \
  INTFREE_SAV(isave);

#endif /* FEATURE_ANALOG_DATA */

/*---------------------------------------------------------------
    DSRMIO_SET_RAWDATA_MODE
---------------------------------------------------------------*/
#ifdef FEATURE_ANALOG_DATA
#error code not present
#elif defined FEATURE_QMIP
#error code not present
#else

  #define DSRMIO_SET_RAWDATA_MODE(tx_wm_ptr, rx_wm_ptr, isave, param)   \
                                                                        \
  INTLOCK_SAV(isave);                                                   \
                                                                        \
  ds_sio_open.stream_mode    = SIO_DS_RAWDATA_MODE;                     \
  ds_sio_open.rx_queue       = NULL;                                    \
  ds_sio_open.tx_queue       = (tx_wm_ptr);                             \
  ds_sio_open.rx_func_ptr    = ps_tcp_enqueue;                          \
  ds_sio_open.port_id        = ds_sio_port_id;                          \
  ds_sio_open.tail_char_used = FALSE;                                   \
  ds_sio_open.rx_flow        = SIO_FCTL_BEST;                           \
  ds_sio_open.tx_flow        = SIO_FCTL_BEST;                           \
                                                                        \
  ds_sio_open.rx_bitrate  = SIO_BITRATE_BEST;                           \
  ds_sio_open.tx_bitrate  = SIO_BITRATE_BEST;                           \
                                                                        \
  param.open_ptr             = &ds_sio_open;                            \
  sio_ioctl( (dsi_stream_id), SIO_IOCTL_CHANGE_MODE,&(param));          \
                                                                        \
  param.record_escape_func_ptr = dsi_set_escape_code_signal;            \
  sio_ioctl ( (dsi_stream_id), SIO_IOCTL_RECORD_ESC_FUNC_PTR, &param);  \
  INTFREE_SAV(isave);

#endif /* FEATURE_ANALOG_DATA */

/*===========================================================================*/

#endif  /*  DSRMIO_H  */

