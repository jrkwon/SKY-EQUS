#ifndef DS_SNOOP_H
#define DS_SNOOP_H
/*===========================================================================

                   SNOOP HEADER FILE

DESCRIPTION

 The Data Services Snoop Header File. Contains shared variables and enums, as
 well as declarations for functions.

Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/ds_snoop.h_v   1.0   23 Oct 2000 17:31:44   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/27/99   mvl     Code review updates: general cleanup.
08/31/98  na/mvl   Created module.
===========================================================================*/


/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "psglobal.h"
#include "customer.h"       /* Customer Specific Features */
#include "dsnetmdl.h"
#include "ip.h"
#include "dsm.h"

/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/
#define SNOOP_ANY_MSG      0     /* snoop all messages of a protocol       */
#define PPP_PROTO_SIZ      2     /* Size of PPP protocol field             */
#define PPP_ACF_PROTO_SIZ  4     /* Proto field without Addr & Ctl compres */

/*---------------------------------------------------------------------------
  Type that is used as the return value for snoop_early_proto_detect().  It
  is used to determine what action needs to be taken on the return from that
  function.
---------------------------------------------------------------------------*/
typedef enum
{
  SNOOP_IGNORE_PKT,        /* packet not interesting, ignore it            */
  SNOOP_DO_UNFRAMING,      /* has protocol we are looking for, unframe it! */
  SNOOP_NEXT_TWO_BYTES     /* need two more bytes to determine action      */
} snoop_proto_ret_val_enum_type;

/*---------------------------------------------------------------------------
  This is the type declaration for all of the callback functions that are
  registered for snooping.
---------------------------------------------------------------------------*/
typedef boolean (*snoop_cb_f_ptr_type)(iface_stack_enum_type iface,
                                       uint16 protocol,
                                       dsm_item_type ** item_head_ptr);

/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

                      PUBLIC FUNCTION DECLARATIONS

= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

/*===========================================================================
FUNCTION  SNOOP_INIT()

DESCRIPTION
  This function initiailzes the snoop data structures, namely it zeros out
  the data structure that contains all of the call back functions.  This
  function is called whenever a non packet call is made (i.e. sockets,
  async...)

DEPENDENCIES
  It is assumed that one can zero out a multidimentional array by knowing
  it's base address, the range of all of it's indices and the size of the
  base data type.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void snoop_init(void);


/*===========================================================================
FUNCTION  SNOOP_REG_EVENT()

DESCRIPTION
  This function stores a function pointer that will be used as a callback
  when a particular PPP protocol and message are seen.

DEPENDENCIES
  None

RETURN VALUE
  A 32bit opaque identifier that can be used to unregister the function that
  has been registered.

SIDE EFFECTS
  If this function is called for a protocol and message combination that
  already has a function call associated with it, it will remove the old
  function and replace it with the new function that is being added.
===========================================================================*/
uint32 snoop_reg_event
(
  iface_stack_enum_type iface,    /* the stack the registration is for     */
  uint16                protocol, /* the protocol the registration is for  */
  byte                  msg_type, /* the message the registration is for   */
  snoop_cb_f_ptr_type   f_ptr     /* the callback funtion being registered */
);


/*===========================================================================
FUNCTION SNOOP_UNREG_EVENT()

DESCRIPTION
  This function removes a particular callback function from the snoop
  registration data structure.  This stops it from being called when its
  associated protocol and message are found.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void snoop_unreg_event
(
  uint32 reg_tag        /* the xparent reg tag returned by snoop_reg_event */
);


/*===========================================================================
FUNCTION  SNOOP_UNREG_ALL()

DESCRIPTION
  This function removes all callback functions that are registered for ALL
  protocols.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void snoop_unreg_all(void);

/*===========================================================================
FUNCTION  SNOOP_EARLY_PROTO_DETECT()

DESCRIPTION
  This function does early protocol detection of unframed PPP packets. It is
  passed a dsm_item that contains either the first two or four UNESCAPED
  bytes of a PPP packet. These are used to determine if this is a packet we
  are interested in.

  The item_ptr argument must have either 2 or 4 bytes.

  The item_ptr->kind field specifies whether this pkt. was on the Um or the
  Rm interface.

DEPENDENCIES
  This function expects that the first two or four bytes of the PPP packet
  are the first two or four bytes of the first DSM item.

RETURN VALUE
  One of three values:
  SNOOP_IGNORE_PKT:     Return to looking only for the 7E
  SNOOP_DO_UNFRAMING:   Unframe whole packet are send to snoop
  SNOOP_NEXT_TWO_BYTES: Not enough information here, need to look at next
                            two bytes.

SIDE EFFECTS
  None
===========================================================================*/
snoop_proto_ret_val_enum_type snoop_early_proto_detect
(
  iface_stack_enum_type in_iface,       /* iface (Rm or Um) pkt arrived on */
  const dsm_item_type * item_ptr        /* Item containing input PPP pkt.  */
);

/*===========================================================================
FUNCTION  SNOOP_PROTO_MSG_DETECT()

DESCRIPTION
  This function is given a pointer to a dsm_item_ptr and looks at the
  protocol and msg fields to determine if a callback function has been
  registered for this protocol and message combination.

DEPENDENCIES
  This function expects that the first four bytes of the PPP packet are in
  the first DSM item.
  It is also expected that the FF03 has been removed from the front and that
  the protocol ID has been stripped from the packet. The protocol of the 
  packet is known and is passed as an input parameter.

RETURN VALUE
  TRUE: this function needs to be called again with the same DSM item -
        implies that there is a new callback that needs to process this
        packet
  FALSE: no need to call this function again

SIDE EFFECTS
  None
===========================================================================*/
boolean snoop_proto_msg_detect
(
  iface_stack_enum_type iface,         /* iface (Rm or Um) pkt coming on   */
  dsm_item_type **item_head_ptr,       /* pointer to packet chain of data  */
  uint16 protocol                      /* protocol of packet               */
);

#endif /* DS_SNOOP_H */
