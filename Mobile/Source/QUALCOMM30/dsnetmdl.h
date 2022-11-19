#ifndef DSNETMDL_H
#define DSNETMDL_H
/*===========================================================================

                   PPP NETWORK MODEL HEADER FILE

DESCRIPTION

 The PPP Network model header file. Contains shared variables and enums, as
 well as declarations for functions.

Copyright (c) 1998,1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/dsnetmdl.h_v   1.0   23 Oct 2000 17:32:12   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/24/00   snn     Added support for the CHAP resync.
10/27/99   mvl     Code review changes and clean up.
08/20/99   mvl     Changed OUT_IFACE() macro so compiler doesn't throw
                   warnings.
08/10/99   snn     added support for FEATURE_DS_PAP in network model.
04/12/99   mvl     moved the OUT_IFACE() macro from the C file to this file.
10/04/98   mvl     created file
===========================================================================*/


/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "dsm.h"
#include "pppfsm.h"


/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  Enum for the PPP mode for snoop.
---------------------------------------------------------------------------*/
typedef enum
{
  PPP_NONE_MODE,            /* When there is no PPP on the Rm interface    */
  PPP_RELAY_MODE,           /* relay mode - no snooping                    */
  PPP_PSEUDO_NET_MODE,      /* pseudo network mode - snoop don't unframe   */
  PPP_FULL_NET_MODE,        /* full network mode - unframe AND snoop       */
  PPP_INTERNAL_MODE         /* Um PPP packets go up though stacks in phone */
} netmdl_mode_enum_type;

/*---------------------------------------------------------------------------
  Enum for the PPP state.
---------------------------------------------------------------------------*/
typedef enum
{
  PPP_OOC_META_STATE,                             /* the out of call state */
  PPP_INIT_META_STATE,                            /* the init state        */
  PPP_UP_META_STATE,                              /* the UP state          */
  PPP_RESYNC_META_STATE,                          /* the resync state      */
  PPP_RELAY_META_STATE                            /* the relay state       */
} netmdl_meta_state_enum_type;

/*---------------------------------------------------------------------------
  Enum for the PPP stack - on Rm interface or Um interface.
---------------------------------------------------------------------------*/
typedef  enum
{
  IFACE_NO_STACK= -1,                /* the lower bound value              */
  IFACE_UM_PPP_STACK=0,              /* The Um interface                   */
  IFACE_RM_PPP_STACK,                /* the Rm interface                   */
  IFACE_MAX_STACK                    /* max value of enum, bounds checking */
} iface_stack_enum_type;

/*---------------------------------------------------------------------------
  Enum for the unframe_mode state for ppp_asy_in.

  The state machine is described in the header for the ppp_asy_in()
  function.
---------------------------------------------------------------------------*/
typedef enum
{
  FINDING_7E  = 0,              /* looking for 7E                          */
  FINDING_SOP,                  /* looking for start of packet - after 7Es */
  SNOOPING,                     /* early proto detect - looking for proto  */
  FINDING_EOP,                  /* looking for end of packet               */
  UNFRAMING                     /* unframing to pass to proto msg detect   */
} iface_unframe_mode_enum_type;

/*---------------------------------------------------------------------------
  This struct maintains all the static variables for the function 
  ppp_asy_in.
---------------------------------------------------------------------------*/
typedef struct
{
  word prev_pkt_size;             /* size of the previous packet           */
  word rx_pkt_size;               /* received packet size                  */
  uint16 calc_fcs;                /* the FCS for a packet on this iface    */
  dsm_item_type* pkt_head_ptr;    /* pointer to packet being build         */
  dsm_item_type* pkt_tail_ptr;    /* pointer to tail of packet being built */
  dsm_item_type* orig_head_ptr;   /* pointer to packet that was passed in  */
  dsm_item_type* orig_tail_ptr;   /* pointer to tail of passed in packet   */
  boolean escape;                 /* was last character the escape char?   */
  iface_unframe_mode_enum_type unframe_mode;   /* current unframing mode   */
} asy_in_var_type;

extern struct ppp_s ppp_cb_array[IFACE_MAX_STACK];  /* PPP ctrl block array*/

/*---------------------------------------------------------------------------
  The interface control block type
---------------------------------------------------------------------------*/
typedef struct
{
  netmdl_mode_enum_type mode;           /* the mode this interface is in   */
  void  (*tx_f_ptr)( dsm_item_type **); /* tx function                     */
  asy_in_var_type input_params;         /* Static variables for iface input*/
  struct ppp_s*   ppp_cb_ptr;           /* the PPP control block           */
  byte curr_ppp_id;                     /* the PPP id that is expected     */
} iface_cb_type;

/*---------------------------------------------------------------------------
  The interface control block array.
---------------------------------------------------------------------------*/
extern iface_cb_type iface_cb_array[IFACE_MAX_STACK];

/*---------------------------------------------------------------------------
  This flag will inform if the previous network PPP call had PAP.
---------------------------------------------------------------------------*/
#if ( (defined FEATURE_DS_PAP) || (defined FEATURE_DS_CHAP) )
extern boolean ds_prev_nw_ppp_call_had_auth;
#endif /* FEATURE_DS_PAP */

#ifdef FEATURE_DS_CHAP_RESYNC

/*---------------------------------------------------------------------------
  PPP_CHAP_RESYNC_PASSWD contains the Password to be used during the Resync.
  
  Length of the Resync password. Note that this should be equal to or lesser
  than MAX_PASSWD_LEN. 
  
  The length is obtained by subtracting 1 from sizeof() because, of the "\0"
  appended at the end of the string, Packet-One000001. 
  CHAP_RESYNC_PASSWD_LENGTH is equal to the length of this password
---------------------------------------------------------------------------*/
#define PPP_CHAP_RESYNC_PASSWD  "Packet-One000001"

#define CHAP_RESYNC_PASSWD_LENGTH (sizeof(PPP_CHAP_RESYNC_PASSWD) -1 )

extern const char *ppp_chap_resync_passwd;
#endif /* FEATURE_DS_CHAP_RESYNC */



/*===========================================================================

                      MACRO  DEFINITIONS

===========================================================================*/
#define OUT_IFACE(iface) \
  ((iface_stack_enum_type) \
   ((iface) == IFACE_UM_PPP_STACK ? IFACE_RM_PPP_STACK : IFACE_UM_PPP_STACK))

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION   NETMDL_SET_MODE()

DESCRIPTION
  This function takes a new mode as arguments and sets both the Rm state
  variable to the new mode and also the appropriate switches.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void netmdl_set_mode
(
  iface_stack_enum_type iface,             /* iface to change              */
  netmdl_mode_enum_type new_iface_mode     /* new mode for that iface      */
);

/*===========================================================================
FUNCTION   NETMDL_SET_META_STATE()

DESCRIPTION
  This function sets the state of the PPP Meta state machine.  This is mostly
  registereing the callbacks that are associated with that state.  The
  notable exception to this is the Resync State, which doesn't have any
  callback associated with it, but does have a fairly complex algorithm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void netmdl_set_meta_state
(
  netmdl_meta_state_enum_type new_ppp_meta_state /* the new meta state     */
);

#endif /* DSNETMDL_H */
