#ifndef DS_DSSNET_H
#define DS_DSSNET_H
/*===========================================================================

          S O C K E T S  N E T W O R K   H E A D E R  F I L E
                   
DESCRIPTION

 The Data Services Network library header file Contains shared variables and 
 enums, as well as declarations for functions.

EXTERNALIZED FUNCTIONS
 
  dssnet_open_network()
    Commands the DS manager task to bring up the traffic channel and sync 
    up PPP.   

  dssnet_close_network()
    Commands the DS manager task to bring down the traffic channel and 
    terminate PPP.

  dssnet_enter_socket_mode()
    Puts the DS manager task into socket mode.

  dssnet_exit_socket_mode()
    Takes the DS manager task out of socket mode.

  dssnet_get_netstate()
    Returns the current state of the network subsystem.

  dssnet_ppp_up_cb()
    Called when PPP has been established, it sets the state of 
    the network subsystem to the open state.

  dssnet_ppp_down_cb()
    Called when the network subsystem no longer available, it sets
    the state of the network subsystem to the closed state.

Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/dssnet.h_v   1.0   23 Oct 2000 17:32:16   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/14/98   na/hcg     Created module.

===========================================================================*/


/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATURE_DS_SOCKETS

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
 Type and static variable defining the state of the network.
---------------------------------------------------------------------------*/
typedef enum
{
  DSSNET_CLOSED_STATE=0,                                  
  DSSNET_OPEN_STATE,
  DSSNET_OPENING_STATE,
  DSSNET_CLOSING_STATE                       
} dssnet_netstate_enum_type;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION DSSNET_OPEN_NETWORK

DESCRIPTION
  Tells dsmgr to bring up the traffic channel and to establish PPP.
  
  The network should not be opened when this function is called. That is, the
  netstate should be either CLOSED or CLOSING.
  
  This function is called from the context of the socket client's task.

DEPENDENCIES
  None.

RETURN VALUE
  None.
            
SIDE EFFECTS
  Traffic channel origination is invoked.
===========================================================================*/
extern void dssnet_open_network
(
  void
);

/*===========================================================================

FUNCTION DSSNET_CLOSE_NETWORK

DESCRIPTION
  Tells dsmgr to bring down the traffic channel and terminate PPP.
  
  The network should not be closed when this function is called. That is, the
  netstate should be either OPEN or OPENING.
  
  This function is called from the context of the socket client's task.

DEPENDENCIES
  None.

RETURN VALUE
  None.
            
SIDE EFFECTS
  Traffic channel is brought down.
===========================================================================*/
extern void dssnet_close_network
(
  void
);

/*===========================================================================

FUNCTION DSSNET_ENTER_SOCKET_MODE

DESCRIPTION
  Tells dsmgr to enter socket mode.
  
  This function is called from the context of the socket client's task.

DEPENDENCIES
  None.

RETURN VALUE
  None.
            
SIDE EFFECTS
  The DS manager is put into socket mode.
===========================================================================*/
extern void dssnet_enter_socket_mode
(
  void
);

/*===========================================================================

FUNCTION DSSNET_EXIT_SOCKET_MODE

DESCRIPTION
  Tells dsmgr to exit socket mode.
  
  This function is called from the context of the socket client's task.

DEPENDENCIES
  None.

RETURN VALUE
  None.
            
SIDE EFFECTS
  The DS manager is taken out of socket mode, and put back into
  autodetect mode.
===========================================================================*/
extern void dssnet_exit_socket_mode
(
  void
);

/*===========================================================================

FUNCTION DSSNET_GET_NETSTATE

DESCRIPTION
  Returns the current state of the network subsystem, which can be open, 
  opening, closed, or closing.
  
  This function is called from the context of the socket client's task.

DEPENDENCIES
  None.

RETURN VALUE
  The current state of the network subsystem.
            
SIDE EFFECTS
  None.
===========================================================================*/
extern dssnet_netstate_enum_type dssnet_get_netstate
(
  void
);

/*===========================================================================

FUNCTION DSSNET_PPP_UP_CB

DESCRIPTION

  Called when PPP has been established, it sets the state of the network 
  subsystem to the open state.
  
  This function is called from the context of the PS task.

DEPENDENCIES
  None.

RETURN VALUE
  None.
            
SIDE EFFECTS
  None.
===========================================================================*/
extern void dssnet_ppp_up_cb
(
  void
);

/*===========================================================================

FUNCTION DSSNET_PPP_DOWN_CB

DESCRIPTION
  Called when the network subsystem no longer available, it sets the state 
  of the network subsystem to the closed state.

  This function is called from the context of the PS task.

DEPENDENCIES
  None.

RETURN VALUE
  None.
            
SIDE EFFECTS
  None.
===========================================================================*/
extern void dssnet_ppp_down_cb
(
  void
);

#endif /* FEATURE_DS_SOCKETS */

#endif /* DS_DSSNET_H */

