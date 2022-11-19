#ifndef DSMGR_H
#define DSMGR_H
/*===========================================================================

                   D A T A   T A S K   M A N A G E R
                        H E A D E R    F I L E

DESCRIPTION
  This file contains all interface functions for the Data Task State machine
   unit.

 Copyright (c) 1993-1997 by QUALCOMM Incorporated.  All Rights Reserved.
 Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
 Copyright (c) 2000,2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   O:/src/asw/COMMON/vcs/dsmgr.h_v   1.2   26 Jan 2001 09:48:02   ydharc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/02/01   na      Removed FEATURE_NEW_SIO( always on).
01/11/01   dwp     Added prototypes for ds_rdm_open & ds_rdm_close.
03/10/00   snn     Added Support for CHAP
01/17/99   ak      Put new SIO code under a compile time switch.
08/13/98   dbd     ATZ and AT&F now tear down call in online command mode
12/17/97   ldg     Moved DS stats accessor function prototype and typedef to
                   ds.h.
07/31/97   ldg     Added accessor function for UI to see some DS vars.
11/09/95   jjw     Completed changes to support fully compliant IS-99
01/22/95   jjw     Small mod's to support use of IS-99 data structures
06/06/94   jjw     Changes from DSMGR code review
04/21/94   jjw     Changes from DSCTL code review
08/17/93   jjw     Initial version of file


===========================================================================*/





/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "rlp.h"
#include "ds.h"
#include "dsi.h"
#include "rex.h"
#include "mc.h"


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
  Public part of the watermark and Queue allocation for SIO to either 
  ATCoP (Async) or to  RLP (Packet Service).  Rest of the LOCAL stuff is
  in dsmgr.c  AK.
---------------------------------------------------------------------------*/
extern dsm_watermark_type dsi_to_sio_pkt_autodt_wm_item;


/*===========================================================================

FUNCTION DSI_STATE_INIT

DESCRIPTION
  This function will initialize the Data Task Manager state machine. This
  includes initialization of the RLP unit.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void dsi_state_init( void);

/*===========================================================================

FUNCTION DSI_MGR

DESCRIPTION
  This function is the central dispatch point for the Data Service Manager.
  This function uses the passed event signals mask to determine the functions
  to perform.
  
DEPENDENCIES
  None.

RETURN VALUE
  A signal mask for the next set of state signals to use for the wait 
  function.

SIDE EFFECTS
  None.
===========================================================================*/

extern ds_sigs_type dsi_mgr
(
  ds_sigs_type signal_mask                /* mask of signals that occurred */
);


/*===========================================================================

FUNCTION DSI_END_PROTOCOL_OPERATION

DESCRIPTION
  This function will tell psmgr to end protocols. If the callstate is active,
  psmgr is told to close protocols gracefully, otherwise it is told to just
  abort.

DEPENDENCIES
  This function is called in service modes which have protocols running in
  the mobile (rawdata and UP).

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void dsi_end_protocol_operation( void);

/*===========================================================================

FUNCTION DSI_SET_PKT_SIGNAL

DESCRIPTION
  Set signal which indicates that a PPP packet has been received.  This
  procedure can be passed to SIO as function pointer.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
extern void dsi_set_pkt_signal(void);

/*===========================================================================

FUNCTION DSI_SET_ESCAPE_CODE_SIGNAL

DESCRIPTION
  Set signal which indicates that '+++' escape code has been received.  This
  procedure can be passed to SIO as function pointer.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/
extern void dsi_set_escape_code_signal(void);

// by chos 00.03.21 for IrDA interface
#ifdef	FEATURE_SKT_DTRSENSE_DS
void	dsi_dte_ready_event(boolean dtrValue);
#else	/* FEATURE_SKT_DTRSENSE_DS */
void	dsi_dte_ready_event();
#endif	/* FEATURE_SKT_DTRSENSE_DS */

// by chos 00.05.16 for IrDA interface
#ifdef	FEATURE_IRDA_UPGRADE
void	DS_Irda_Process ( void );
#endif	/* FEATURE_IRDA_UPGRADE */

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP) )
/*===========================================================================
FUNCTION DSI_GET_AUTH_FROM_NV

DESCRIPTION
  This function gets the PAP item from the NV items for the Data task 
  and stores them in the appropriate variables.

DEPENDENCIES
  The NV task must be started and running.

RETURN VALUE
  None

SIDE EFFECTS
 Each call to 'dsi_get_auth_from_nv' results in a Wait and watchdog kicking
===========================================================================*/
extern void dsi_get_auth_from_nv(void);
#endif /* FEATURE_DS_PAP Or FEATURE_DS_CHAP */

#ifdef FEATURE_RUNTIME_DEVMAP
#error code not present
#endif /* FEATURE_RUNTIME_DEVMAP */

#endif /* DSMGR_H */

