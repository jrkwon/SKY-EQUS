#ifndef SRV_H
#define SRV_H
/*===========================================================================

              S E R V I C E    C O N F I G U R A T I O N    &

                          N E G O T I A T I O N

DESCRIPTION
  This file contains declarations for use with the Service Configuration
  and Negotiation Services.

Copyright (c) 1995 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/srv.h_v   1.2   17 Dec 2000 23:40:06   htung  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/17/00   ht      Add function srv_set_evrc_usage() prototype.
11/02/00   lcc     Added support for FEATURE_COMPLETE_SNM.
07/28/00   fc      Mainline FEATURE_CS_ACTION_TIME_TRANS.
06/12/00   lcc     Added prototype for srv_update_nn_scr_from_pcnm.
04/17/00   va      Made srv_get_pref_RCs function return nothing
03/28/00   ry      Removed the #ifdef FEATURE_IS2000 around #include "mccdma.h"
                   since the definition of mccdma_chnasn_type is unconditionally
                   needed for the prototype srv_init()
03/21/00   ry      Added IS2000 featurization
12/20/99   va      Support for updating non-neg SCR info from the power control message.
11/15/99   va      IS2000 Changes for voice:
                   New function srv_get_pref_rcs()
                   New function srv_get_pending_cfg()
                   Support for storing Non-Negotiable service config.
11/03/99   lcc     Merged from GD4021:
           ry      Added SRV_RIF_HHO_EC to srv_event_code_type and the
                   corresponding event parameter in srv_event_type
07/30/99   lcc     Exported srv_alt_cfg which is now used in snm.c.
07/19/99   nmn     Added declaration for snm_get_alt_so_list, surrounded by
                   #ifdefs for FEATURE_IS95B or FEATURE_SPECIAL_MDR.
06/15/99   doj     Added support for service config via handoff message.
05/10/99   kmp     Merged the IS-95B changes below.
           jq      Modified srv_init() to include default_config parameter of
                      ECAM
11/30/98   lcc     Obsoleted srv_alt_so and added prototype for srv_cmd_cfg.
06/08/95   rdb     Created file. Deja vu all over again.

===========================================================================*/

#include "comdef.h"
#include "queue.h"
#include "caii.h"
#include "mccdma.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*--------------------------------------------------------------------------
                             STATUS CODES

The following status codes are for use in the 'status' field of the
srv_event_type defined below.
--------------------------------------------------------------------------*/

typedef enum
{
  SRV_DONE_S,      /* Event successfully processed */
  SRV_NOTNOW_S,    /* Inappropriate to process the specified event
                      at this time. Caller may try again later. */
  SRV_BADPARM_S,   /* Illegal parameter(s) specified */
  SRV_NOTSUP_S,    /* Unsupported feature or operation specified */
  SRV_ERR_S        /* Internal error occurred */
} srv_status_type;


/*--------------------------------------------------------------------------
                               EVENT CODES

The following event codes are for use in the 'ec' field of the
srv_event_type defined below.
--------------------------------------------------------------------------*/

typedef enum
{
  SRV_SIG_EC,       /* Serv config signal set */
  SRV_UI_EC,        /* Input from user interface */

  SRV_REQ_MSG_EC,   /* Service request message received */
  SRV_RSP_MSG_EC,   /* Service response message received */
  SRV_CON_MSG_EC,   /* Service connect message received */
  SRV_CTL_MSG_EC,   /* Service option control message received */

  SRV_HO_DIR_EC,    /* Any handoff message used as service connect message */

  SRV_TC_WFO_EC,    /* Waiting for order substate entered */
  SRV_TC_WFA_EC,    /* Waiting for answer substate entered */
  SRV_TC_CON_EC,    /* Conversation substate entered */
  SRV_TC_REL_EC,     /* Release substate entered */
  #ifdef FEATURE_IS95B_MAHHO
  SRV_RIF_HHO_EC,    /* Restore given SCR immediately */
  #endif
  #ifdef FEATURE_IS2000
  SRV_PWR_CTRL_EC,   /*Save relevant parameters from PCM into the Non-Neg SCR*/
  #endif
  SRV_TOTAL_COMMANDS
} srv_event_code_type;


typedef struct
{
  srv_event_code_type  ec;      /* Event code  (srv_event_code_type) */
  srv_status_type      status;  /* Status code (srv_status_type) */

  union                         /* Union of all event parameters. Note */
  {                             /* not all events require parameters.  */
    struct  /* Corresponds to SRV_UI_EC */
    {
      word  so;
        /* Service option number */
    } ui;

    struct  /* Corresponds to SRV_REQ_MSG_EC */
    {
      caii_ftc_srv_req_type  *msg_ptr;
        /* Ptr to service request message */
    } req_msg;

    struct  /* Corresponds to SRV_RSP_MSG_EC */
    {
      caii_ftc_srv_rsp_type  *msg_ptr;
        /* Ptr to service response message */
    } rsp_msg;

    struct  /* Corresponds to SRV_CON_MSG_EC */
    {
      caii_srv_con_type  *msg_ptr;
        /* Ptr to service connect message */
    } con_msg;

    struct /* Corresponds to SRV_HO_DIR_EC */
    {
      caii_ho_msg_type *msg_ptr;
        /* Ptr to handoff message */
    } ho_msg;

    struct  /* Corresponds to SRV_CTL_MSG_EC */
    {
      caii_ftc_srv_ctl_type  *msg_ptr;
        /* Ptr to service option control message */
    } ctl_msg;

#ifdef FEATURE_IS95B_MAHHO
    struct  /* Corresponds to SRV_RIF_HHO_EC */
    {
      caii_srv_cfg_type  *srv_cfg_ptr;
        /* Ptr to service configuration records */
      #ifdef FEATURE_IS2000
      caii_non_neg_srv_cfg_type *nn_srv_cfg_ptr;
        /* Pointer to Non Neg SCR */
      #endif
    } rif_hho;
#endif /* FEATURE_IS95B_MAHHO */
#ifdef FEATURE_IS2000
   struct
   {
     mccdma_pwr_ctrl_info_type *pwr_ctrl_info_ptr;
   }pcm_info;
#endif

  } ep;  /* Event parameter union */

} srv_event_type;


typedef struct
{
  caii_complete_srv_cfg_type cfg;   /* Holds pending service configuration */
  byte cfg_msg_type;       /* The CAI message responsible for this config. */
} srv_pending_cfg_type;

/*--------------------------------------------------------------------------
                     EXTERNALIZED VARIABLES

--------------------------------------------------------------------------*/

// !!!Caution (see snm.c)!!!
// negotiation_info holds pointers to the following data structure.  Since the
// following structures are legacy code, they are still defined as what they
// were before.  They should eventually be moved into negotiation_info and be
// hidden.

extern caii_complete_srv_cfg_type  srv_current_cfg;
  /* Holds current active service configuration */

extern srv_pending_cfg_type  srv_pending_cfg;
    /* Holds pending service configuration. */

extern caii_srv_cfg_type  srv_proposed_cfg;
  /* For building a proposed service configuration to send. */


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION SRV_INIT

DESCRIPTION
  This function intializes the Service Configuration and Negotiation
  Services. It should be called whenever the mobile station enters the
  Traffic Channel state for the purpose of making a call.

DEPENDENCIES
  Assumes that srv_timer (a rex timer) has been defined and initialized.

RETURN VALUE
  SRV_DONE_S    - Initialization successful.
  SRV_BADPARM_S - Invalid initialization parameter(s) specified.
  SRV_NOTSUP_S  - Unsupported service option specified.

SIDE EFFECTS

===========================================================================*/

extern srv_status_type  srv_init
(
  boolean  mob_orig,
    /* Specifies whether the call is mobile originated (TRUE) or
       mobile terminated (FALSE). */

  word  so,
    /* Specifies which service option number was included in either the
       origination message or the page response message. */

  mccdma_chnasn_type *chn_assign
     /* Pointer to the channel assignment message */
);


/*===========================================================================

FUNCTION SRV_RPT_EVENT

DESCRIPTION
  This function reports a specified event to the Service Configuration
  and Negotiation Services.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/

extern void srv_rpt_event
(
  srv_event_type  *e_ptr
    /* Pointer to a block describing the event */
);


#ifndef FEATURE_COMPLETE_SNM
/*===========================================================================

FUNCTION SRV_EVAL_SO

DESCRIPTION
  This function evaluates a specified service option number to see if it is
  supported by the mobile station.

DEPENDENCIES

RETURN VALUE
  IF SO is supported, the same SO is returned; otherwise an alternate SO is
  returned.  If no alternate SO exists, CAI_SO_REJ is returned.

SIDE EFFECTS

===========================================================================*/

extern word  srv_eval_so
(
  word  so
    /* The service option number to be evaluated */
);


/*===========================================================================

FUNCTION SRV_EVAL_CFG

DESCRIPTION
  This function evaluates a specified service configuration to determine
  whether the mobile station can support it.

DEPENDENCIES

RETURN VALUE
  TRUE  - Mobile station can support specified service configuration.
  FALSE - Mobile station cannot support it.

SIDE EFFECTS

===========================================================================*/

extern boolean  srv_eval_cfg
(
  caii_srv_cfg_type *cfg_ptr
    /* Pointer to configuration to evaluate */
);


/*===========================================================================

FUNCTION SRV_SET_DEF_CFG

DESCRIPTION
      This function

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

extern srv_status_type  srv_set_def_cfg
(
  word  so,
    /* The service option for which a default service configuration
       is to be constructed. */

  caii_srv_cfg_type  *cfg_ptr,
    /* Pointer to place to construct the default configuration. */

  boolean  set_con
    /* Specifies whether or not to include a service option connection
       in the default service configuration. */
);


/*===========================================================================

FUNCTION SRV_ALT_CFG

DESCRIPTION
  This function evaluates a specified service configuration to determine
  whether the mobile station can support an alternative configuration.
  This function is used only for non-SNM managed SOs and will be obsolete
  when all SOs are migrated to SNM.  It is called by snm_eval_config when
  the SO negotiation is not started by a SNM managed SO.

DEPENDENCIES

RETURN VALUE
  TRUE  - if the mobile station can support an alternative service
          configuration. The alternative configuration is returned
          in *alt_ptr.
  FALSE - if the mobile station cannot support an alternative configuration.

SIDE EFFECTS

===========================================================================*/

boolean  srv_alt_cfg
(
  caii_srv_cfg_type  *cfg_ptr,
    /* Pointer to service configuration for which an alternative
       is desired. */

  caii_srv_cfg_type  *alt_ptr
    /* Pointer to place to construct the alternative service
       configuration. */
);
#endif // FEATURE_COMPLETE_SNM


/*===========================================================================

FUNCTION SRV_GET_CUR_CFG

DESCRIPTION
  This function returns the current service configuration.

DEPENDENCIES
  Only valid during Traffic Channel Operation.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void  srv_get_cur_cfg
(
  caii_srv_cfg_type  *cfg_ptr
    /* Pointer to record in which to return the current service
       configuration. */
);


#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION SRV_GET_CUR_NON_NEG_CFG

DESCRIPTION
  This function returns the current non negotiable service configuration.

DEPENDENCIES
  Only valid during Traffic Channel Operation.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void  srv_get_cur_non_neg_cfg
(
  caii_non_neg_srv_cfg_type  *cfg_ptr
    /* Pointer to record in which to return the current non neg service
       configuration. */
);


/*===========================================================================

FUNCTION SRV_GET_PENDING_CFG

DESCRIPTION
  This function returns the pending service configuration.

DEPENDENCIES
  Only valid during Traffic Channel Operation.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void  srv_get_pending_cfg
(
  caii_complete_srv_cfg_type  *cfg_ptr
    /* Pointer to record in which to return the pending service
       configuration. */
);


/*===========================================================================

FUNCTION srv_get_pref_RCs

DESCRIPTION
  This function returns the preferred RC for the forward and reverse primary
  channel. This preference is sent back in the page resp or the origination message.
  Once all SOs are migrated to srv, this will be obsolete. Avoid calling this function
  directly. Always call the corresponding function in SNM

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void srv_get_pref_RCs
(
  word so, // The SO for which pref RCs are needed
  cai_radio_config_type * fwd_rc_ptr, // Storage for returned fwd RC
  cai_radio_config_type * rev_rc_ptr // Storage for return  rev RC
);
#endif /* FEATURE_IS2000 */



/*===========================================================================

FUNCTION SRV_CMD_CFG

DESCRIPTION
      This function commands the multiplex sublayer and service option
      applications to begin using a specified service configuration at
      a specified action time.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void  srv_cmd_cfg
(
  caii_srv_cfg_type  *cfg_ptr
);

#ifndef FEATURE_COMPLETE_SNM
#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
/*===========================================================================

FUNCTION SRV_GET_ALT_SO_LIST

DESCRIPTION
      This function evaluates a given service option, returning the appropriate
      list of alternative service options.

DEPENDENCIES
      alt_so points to an array of bytes large enough to handle the maximum
      number of alternative service options allowed (max_alt_so).  This i
      maximum is the lesser of the two values: max_num_alt_so received from
      ESPM, or internal array size (defined in cai.h as CAI_AC_ALT_SO_MAX).

      The so_num must have already been checked to be a valid service option.

RETURN VALUE
      Returns the number of alternative service options in the alt_so list.

SIDE EFFECTS
      The values that alt_so point to will be changed to the alternative
      service options.

===========================================================================*/

byte srv_get_alt_so_list
(
  word so_num,       // SO to evaluate
  byte max_alt_so,   // maximum number of SOs to include in alt_so list
  word alt_so[]      // array where alt_so list will be returned
);
#endif // FEATURE_COMPLETE_SNM

#endif //FEATURE_IS95B || FEATURE_SPECIAL_MDR

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION SRV_UPDATE_NN_SCR_FROM_PCNM

DESCRIPTION
  This function updates the Non neg SCR based on the info from the
  Power Control Message.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void srv_update_nn_scr_from_pcnm
(
  const caii_pwr_ctrl_type* pcnm
);
#endif  // FEATURE_IS2000

/*===========================================================================

FUNCTION SRV_SET_EVRC_USAGE

DESCRIPTION
  This function sets the EVRC capability status based on the NV.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/

void  srv_set_evrc_usage
(
  boolean status      /* Enabled is TRUE */
);

#endif /* SRV_H */
