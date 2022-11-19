#ifndef PPPIPCP_H
#define PPPIPCP_H

/*===========================================================================

                   M O D E L    H E A D E R    F I L E

DESCRIPTION

  Copyright 1992-1995 William Allen Simpson
  Licensed to Qualcomm -- 95 Jan 21

  Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
  Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/pppipcp.h_v   1.0   23 Oct 2000 17:32:28   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/28/99   mvl     Added code review changes
08/20/99   mvl     Added declaration of ipcp_save_work_opt()
07/28/99   mvl     changed the NAK_TRY value to 3 from 10 as 10 nak cycles
                   will take more than 3 seconds.  Avoids infinite loop.
06/18/99   mvl     removed references to two address option which is not
                   supported.
06/08/99   mvl     Changed the size of the 'slots' field in the IPCP option
                   value field to a uint8 (which is what the protocol size is
                   for this field).
05/20/99   na      Changed header for ipcp_init() for network model builds.
05/20/99   mvl     Added typedef for various structures
11/09/95   jjw     IS-99 Compliant Release
===========================================================================*/

/*===========================================================================

  INCLUDE FILES

===========================================================================*/
#include "pppfsm.h"
#include "slhc.h"
#ifdef	DNSIP
#include "psi.h"	//	psi_mode is referenced
#endif /* DNSIP */

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#define IPCP_REQ_TRY   20    /* REQ attempts */
#define IPCP_NAK_TRY    3    /* NAK attempts */
#define IPCP_TERM_TRY  10    /* tries on TERM REQ */
#define IPCP_TIMEOUT    3    /* Seconds to wait for response */

/*---------------------------------------------------------------------------
  IPCP option types
---------------------------------------------------------------------------*/

#define IPCP_COMPRESS       2
#define IPCP_1ADDRESS       3
#define IPCP_N_COMPRESS    (1 << IPCP_COMPRESS)
#define IPCP_N_1ADDRESS    (1 << IPCP_1ADDRESS)

#ifdef DNSIP
#define IPCP_D1ADDRESS      4
#define IPCP_D2ADDRESS      5
#define IPCP_N_D1ADDRESS     (1 << IPCP_D1ADDRESS)
#define IPCP_N_D2ADDRESS     (1 << IPCP_D2ADDRESS)
#define IPCP_RD1ADDRESS      0x81 //Real value of Primary DNS
#define IPCP_RD2ADDRESS      0x83 //Real value of Secondary DNS
#endif /* DNSIP */

#define IPCP_OPTION_LIMIT     3
#define IPCP_OPTION_LIMIT_DNS 5    


/*---------------------------------------------------------------------------
  Size of the accepted options
---------------------------------------------------------------------------*/
#define IPCP_COMPRESS_SIZ 4
#define IPCP_1ADDRESS_SIZ 6

/*---------------------------------------------------------------------------
  Table for IPCP configuration requests
---------------------------------------------------------------------------*/

struct ipcp_value_s
{
  ip4a address;                    /* address for this side */
#ifdef DNSIP
  ip4a dns1;                       /* primary DNS    */
  ip4a dns2;                       /* Secondary DNS  */  
#endif
  uint16 compression;               /* Compression protocol */
  uint8  slots;                               /* Slots (0-n)*/
  uint8  slot_compress;    /* Slots may be compressed (flag)*/
};
typedef struct ipcp_value_s ipcp_value_type;

#define IPCP_SLOT_DEFAULT              16   /* Default # of slots */
#define IPCP_SLOT_HI      SLHC_ARRAY_SIZE   /* Maximum # of slots */
#define IPCP_SLOT_LO                    1   /* Minimum # of slots */
#define IPCP_SLOT_COMPRESS           0x01   /* May compress slot id */

/* Idiosyncrasies:
 *  local.want.other  stores copy of assigned address.
 *  local.work.other  used by the 2addresses negotiation only.
 *
 *  remote.want.other  stores copy of assigned address.
 *  remote.work.other  used by the 2addresses negotiation only.
 */

/* IPCP control block */

struct ipcp_pdv_s
{
  struct slcompress *slhcp;  /* pointer to compression block */
};
typedef struct ipcp_pdv_s ipcp_pdv_type;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION IPCP_SAVE_WORK_OPT()

DESCRIPTION
   This function stores the value of the IPCP option that is passed in. It is
   being saved into the work and work_pdv the want values are saved by the
   caller.

DEPENDENCIES
  ipcp_init() and ipcp_init() should have been called before this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void ipcp_save_work_opt
(
  struct fsm_side_s *opts,       /* pointer to where the values are stored */
  byte opt_num,                                       /* the option number */
  byte *opt_val                                 /* the value of the option */
);
#endif /* FEATURE_DS_NET_MODEL */


#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION IPCP_SET_PPP_RESYNC_FNC

DESCRIPTION
  This function sets the callback function pointer for the IPCP established
  function.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ipcp_set_ppp_resync_fnc
(
  void (fnc_ptr)()
);
#endif /* FEATURE_DS_NET_MODEL */


#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION IPCP_PPP_IN_RESYNC

DESCRIPTION
  This function returns the state of the ipcp_ppp_resync_fnc_ptr variable

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  This variable is set
  FALSE: This variable is not set

SIDE EFFECTS
  None
===========================================================================*/
boolean ipcp_ppp_in_resync
(
  void
);
#endif /* FEATURE_DS_NET_MODEL */


#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION IPCP_INIT

DESCRIPTION
  This function will Initialize the IPCP configuration structure on the
  given interface( Rm or Um).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ipcp_init
(
  iface_stack_enum_type iface,                 /* The interface (Rm or Um) */
  struct ppp_s *ppp_p               /* PPP control block for the interface */
);


#else /* FEATURE_DS_NET_MODEL not defined */
/*===========================================================================
FUNCTION IPCP_INIT

DESCRIPTION
  This function will Initialize the IPCP configuration structure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ipcp_init
(
  struct ppp_s *ppp_p
);
#endif  /* FEATURE_DS_NET_MODEL */


#endif /* _PPPIPCP_H */
