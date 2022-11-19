#ifndef PPPLCP_H
#define PPPLCP_H

/*===========================================================================

                   M O D E L    H E A D E R    F I L E

DESCRIPTION

  Copyright 1992-1995 William Allen Simpson
  Licensed to Qualcomm -- 95 Jan 21        

  Copyright (c) 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
  Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
  Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
  Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.  
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/ppplcp.h_v   1.0   23 Oct 2000 17:32:30   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/24/00   snn     Increased the value of Max_passwd_len from 14 to 16, 
                   because during resync, the CHAP password used has a 
                   length of 16.
03/10/00   snn     added CHAP support
10/28/99   mvl     Added code review changes
09/27/99   snn     Changed ppp_auth_info_type to a typedef to follow coding
                   standards. Fixed commenting styles. Added comments for 
                   the PAP prototypes.
08/25/99   snn     Updated the sizes of the arrays in PAP data structure
08/19/99   mvl     Added header for lcp_save_work_opt()
08/09/99   snn     Added support for PAP timeouts.
07/26/99   mvl     Changed the NAK_TRY value to 3 from 10 as 10 nak cycles
                   will take more than 3 seconds.  Avoids infinite loop.
07/26/99   mvl     removed references to quality reporting period which is
                   not supported.
07/26/99   na      Changed header for lcp_init() for network model builds.
07/26/99   mvl     Added typedef for various structures
07/09/99   snn     Added support for PAP implementation
11/09/95   jjw     IS-99 Compliant Release

===========================================================================*/

#include "pppfsm.h"
#include "customer.h"       /* Customer Specific Features */
#ifdef FEATURE_DS_NET_MODEL
#include "dsnetmdl.h"
#endif

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
#define LCP_REQ_TRY   20    /* REQ attempts                                */
#define LCP_NAK_TRY   3     /* NAK attempts dropped from 10 to 3 bacause 10
                              was too high for radio environment.          */
#define LCP_TERM_TRY  10    /* tries on TERM REQ                           */
#define LCP_TIMEOUT    3    /* Seconds to wait for response                */

/* LCP option types */

#define LCP_MRU           0x01
#define LCP_ACCM          0x02
#define LCP_AP            0x03
#define LCP_QMP           0x04
#define LCP_MAGIC         0x05
#define LCP_PFC           0x07
#define LCP_ACFC          0x08
#define LCP_OPTION_LIMIT  0x08  /* highest # we can handle                 */

#define LCP_N_MRU     (1 << LCP_MRU)
#define LCP_N_ACCM    (1 << LCP_ACCM)
#define LCP_N_AP      (1 << LCP_AP)
#define LCP_N_QMP     (1 << LCP_QMP)
#define LCP_N_MAGIC   (1 << LCP_MAGIC)
#define LCP_N_PFC     (1 << LCP_PFC)
#define LCP_N_ACFC    (1 << LCP_ACFC)

/*---------------------------------------------------------------------------
  Table for LCP configuration requests
  Note that the chap_digest is not a option, as other members of this table.
---------------------------------------------------------------------------*/
struct lcp_value_s 
{
  uint16 mru;                   /* Maximum Receive Unit                    */
  uint32 accm;                  /* Async Control Char Map                  */
  uint16 authentication;        /* Authentication protocol                 */
#ifdef FEATURE_DS_CHAP
  uint8  chap_digest;           /* Digest algorithm used for CHAP          */
#endif /* FEATURE_DS_CHAP */
  uint16 monitoring;            /* Quality Monitoring protocol             */
  uint32 magic_number;          /* Magic number value                      */
};
typedef struct lcp_value_s lcp_value_type;

/*---------------------------------------------------------------------------
  Other configuration option values
---------------------------------------------------------------------------*/
#define LCP_ACCM_DEFAULT  0xffffffffL
#define LCP_MRU_DEFAULT   1500
#define LCP_MRU_HI        4096
#define LCP_MRU_LO        128

#ifdef FEATURE_DS_PAP

/*---------------------------------------------------------------------------
  Message Id's for the PAP packets 
---------------------------------------------------------------------------*/
#define PAP_AUTH_REQ 0x01    /* Authenticate Request                       */
#define PAP_AUTH_ACK 0x02    /* Authenticate Acknowledge                   */
#define PAP_AUTH_NAK 0x03    /* Authenticate Nak                           */

/*---------------------------------------------------------------------------
  The time-out value for the PAP. This will be given a value of 3 seconds. 
  The value should be given in milliseconds.
---------------------------------------------------------------------------*/
#define PPP_PAP_TIMER_VAL 3000

/*---------------------------------------------------------------------------
  Constant to load the initial value of the Retry counter. If mobile does 
  not receive auth-ack/auth-nak from the base station, then the mobile 
  retries PPP_PAP_RETRY_COUNTER_VAL number of times, and then terminates the 
  link between base station and the mobile.
---------------------------------------------------------------------------*/
#define PPP_PAP_RETRY_COUNTER_VAL 5

#endif /* FEATURE_DS_PAP  */

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP))

/*---------------------------------------------------------------------------
  The maximum size of the User Id and the password that can be given to the 
  user id and the password in the PAP. This size limitation comes from the NV 
  data structure. NV cannot accept more than 128 bytes for a structure. If NV 
  structure should be modified then tools should also be modified. The 
  User_id can have a length given by NV_MAX_PAP_USER_ID_LENGTH and Password 
  can have a maximum length of NV_MAX_PAP_PASSWORD_LENGTH bytes.
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
  Maximum length permissable for user id. Note that these should be equal to
  or greater than NV_MAX_PAP_USER_ID_LENGTH. The greater than case, may be
  necessary to store the TE2 user-id for network model packet calls.
---------------------------------------------------------------------------*/
#define MAX_USER_ID_LEN 64 

/*---------------------------------------------------------------------------
  Maximum length permissable for password used during authentication, i.e.
  PAP or CHAP. Note that this should be equal to or greater than 
  NV_MAX_PAP_PASSWORD_LENGTH. The greater than case, may be necessary to 
  store the TE2 user-id for network model packet calls.
  
  The length has been made as 16 because the size of the array has to be 
  greater than size of the Resync Password. The length of the resync password
  is given by CHAP_RESYNC_PASSWD_LENGTH and is defined to value of 16. 
  
  Note that in network model, the Password used during resync for CHAP is 
  different than that used initially.
---------------------------------------------------------------------------*/
#define MAX_PASSWD_LEN  16

typedef struct 
{
    /*-----------------------------------------------------------------------
      Store the length of the user_id. Used for length field in PAP 
      config-req
    -----------------------------------------------------------------------*/
    byte user_id_len;                                                        
    
    /*-----------------------------------------------------------------------
      Array to store the the user_id. 
    -----------------------------------------------------------------------*/
    char user_id_info[MAX_USER_ID_LEN]; 
    
    /*-----------------------------------------------------------------------
      Store the length of the password. Used for password length field in PAP 
      config-req  
    -----------------------------------------------------------------------*/
    byte passwd_len;                    
    
    /*----------------------------------------------------------------------
      Array to store the password      
    -----------------------------------------------------------------------*/
    char passwd_info[MAX_PASSWD_LEN];   
    
} ppp_auth_info_type;  

extern ppp_auth_info_type ppp_auth_info; 

#endif /* FEATURE_DS_PAP || FEATURE_DS_CHAP  */

#define PPP_NO_AUTH 0x1
#define PPP_DO_PAP  0x2
#define PPP_DO_CHAP 0x3

#ifdef FEATURE_DS_CHAP

/*---------------------------------------------------------------------------
This structure contains some of the CHAP elements.
It may be noted that, we did not use any element to store the response, 
because it is calculated always. 
The user_id and the secret are used from the ppp_auth_info sturcture.
byte chap_in_id, dsm_item_s* chap_challenge_ptr: 

These two have been made as global variables because, we need to call 
ppp_send_chap_pkt() from two places:
	On receiving a CHAP challenge.
	On timing out.
---------------------------------------------------------------------------*/
typedef struct 
{
    /*----------------------------------------------------------------------
      Structure to store the challenge that is received from the Base 
      station.      
    -----------------------------------------------------------------------*/
    struct dsm_item_s *chap_challenge_ptr;
    
    /*----------------------------------------------------------------------
      Identifier number. This is used to calculate the response, hence it
      is stored here.      
    -----------------------------------------------------------------------*/
    uint8 chap_in_id;
}lcp_chap_info_type;

extern lcp_chap_info_type lcp_chap_info;


/*---------------------------------------------------------------------------
  The time-out value for the CHAP. This will be given a value of 3 seconds. 
  The value should be given in milliseconds.
---------------------------------------------------------------------------*/
#define PPP_CHAP_TIMER_VAL 3000

/*---------------------------------------------------------------------------
  Constant to load the initial value of the Retry counter. If mobile does 
  not receive Success/Failure from the base station, then the mobile 
  retries PPP_CHAP_RETRY_COUNTER_VAL number of times, and then terminates the 
  link between base station and the mobile.
---------------------------------------------------------------------------*/
#define PPP_CHAP_RETRY_COUNTER_VAL 5

/*---------------------------------------------------------------------------
  Length of the Code field in the CHAP packet 
---------------------------------------------------------------------------*/
#define PPP_CHAP_CODE_SIZE 		    1 

/*---------------------------------------------------------------------------
  Length of the ID field in the CHAP packet 
---------------------------------------------------------------------------*/
#define PPP_CHAP_ID_SIZE 		    1 

/*---------------------------------------------------------------------------
  Length of the "Length" field in the CHAP packet 
---------------------------------------------------------------------------*/
#define PPP_CHAP_LENGTH_SIZE 		2 

/*---------------------------------------------------------------------------
  Length of the Value Size field in CHAP packet 
---------------------------------------------------------------------------*/
#define PPP_CHAP_VALUE_SIZE_SIZE 	1 

/*---------------------------------------------------------------------------
  Size of the MD5 output. This is 16bytes.
---------------------------------------------------------------------------*/
#define PPP_CHAP_MD5_SIZE           16


#define PPP_CHAP_CHALLENGE		    1 /* Code for CHAP Challenge           */
#define PPP_CHAP_RESPONSE		    2 /* Code for CHAP Response            */
#define PPP_CHAP_SUCCESS			3 /* Code for CHAP Success             */
#define PPP_CHAP_FAILURE			4 /* Code for CHAP Failure             */

#define PPP_CHAP_MD5                5 /* Value of the MD5 algorithm        */

#endif /* FEATURE_DS_CHAP */

/*---------------------------------------------------------------------------
  Flag contains PPP_NO_AUTH if authentication is not negotiated. It contains
  PPP_DO_PAP for PAP and PPP_DO_CHAP for CHAP.
---------------------------------------------------------------------------*/
extern uint8 ppplcp_do_auth;

/*---------------------------------------------------------------------------
  LCP control block
---------------------------------------------------------------------------*/
struct lcp_pdv_s 
{
  int dummy;
};

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION PPP_READY

DESCRIPTION
  This function will Check for PPP Network-Layer Protocol Phase 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_ready(struct ppp_s *ppp_p);

/*===========================================================================
FUNCTION PPP_UP

DESCRIPTION
  This function will indicate that this layer is up

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_up(struct ppp_s *ppp_p);


/*===========================================================================
FUNCTION PPP_DOWN

DESCRIPTION
  This function will indicate that this layer is down

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_down(struct ppp_s *ppp_p);

#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION LCP_SAVE_WORK_OPT()

DESCRIPTION
   This function stores the value of the LCP option that is passed in. It is
   being saved into the work and work_pdv the want values are saved by the
   caller.

DEPENDENCIES
  lcp_init() and ipcp_init() should have been called before this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void lcp_save_work_opt
(
  struct fsm_side_s *opts,       /* pointer to where the values are stored */
  byte opt_num,                                       /* the option number */
  byte *opt_val                                 /* the value of the option */
);


/*===========================================================================
FUNCTION LCP_INIT

DESCRIPTION
  This function will initialize the LCP configuration structure for the given
  interface (Rm or Um).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lcp_init
(
  iface_stack_enum_type iface,                 /* The interface (Rm or Um) */
  struct ppp_s *ppp_p               /* PPP control block for the interface */
);
#else /* FEATURE_DS_NET_MODEL not defined */
/*===========================================================================

FUNCTION LCP_INIT

DESCRIPTION
  This function will initialize the LCP configuration structure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lcp_init
(
  struct ppp_s *ppp_p
);
#endif  /* FEATURE_DS_NET_MODEL */

#ifdef FEATURE_DS_PAP                                                   

/*===========================================================================

FUNCTION PPP_SEND_PAP_PKT

DESCRIPTION
  Builds and transfers the PAP packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ppp_send_pap_pkt( void );

/*===========================================================================

FUNCTION PPP_PAP_TIMEOUT

DESCRIPTION
  This function should be called when the PAP timeout occurs. It checks the
  number of retries done. If all the allowed retries are not done, then it
  transmits a PAP packet again, hoping for a response from the Base
  station. If all the requests are done, then it closes the PPP link.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Informs the calling function to retry
  FALSE: Informs calling function, retries are over and to 
         terminate the call.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean ppp_pap_timeout ( void );

#endif /* FEATURE_DS_PAP   */

#ifdef FEATURE_DS_CHAP                                                   

/*===========================================================================

FUNCTION PPP_SEND_CHAP_PKT

DESCRIPTION
  Builds and transfers the CHAP packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ppp_send_chap_pkt( void );

/*===========================================================================

FUNCTION PPP_CHAP_TIMEOUT

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Informs the calling function to retry
  FALSE: Informs calling function, retries are over and to 
         terminate the call.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean ppp_chap_timeout ( void );

#endif /* FEATURE_DS_CHAP   */

#if (defined (FEATURE_DS_PAP ) || defined (FEATURE_DS_CHAP) )
/*===========================================================================

FUNCTION PPP_AUTH_TIMEOUT

DESCRIPTION
  

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Informs the calling function to retry
  FALSE: Informs calling function, retries are over and to 
         terminate the call.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean ppp_auth_timeout ( void );

#endif /* FEATURE_DS_CHAP   */


#endif /* _PPPLCP_H */
