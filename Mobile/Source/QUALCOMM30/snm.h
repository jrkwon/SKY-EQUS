#ifndef SNM_H
#define SNM_H
/*===========================================================================

          S E R V I C E    N E G O T I A T I O N   M A N A G E R
         S U P P O R T   F O R   M E D I U M   D A T A   R A T E

GENERAL DESCRIPTION
  This module contains definitions and interfaces for service negotiation
  manager (SNM).  These functions are created for support of MDR Release 1.0 and
  it is taken into consideration that a service options will only be gradually
  transitioned into SNM.  Initially, only MDR test and DS service options will
  be managed by SNM.

DESCRIPTION
  This module contains definitions and interfaces for service configuration
  and negotiation for Data Services service options.  This functions are
  created for support of MDR Release 1.0.  Note that once Call Manager is
  implemented, these function may become obsolete.

   Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/




/*===========================================================================

                      EDIT HISTORY FOR FILE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/snm.h_v   1.4   28 Mar 2001 11:24:58   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/31/01   fc      Added prototype for snm_update_non_neg_configuration.
03/14/01   va      Increased MAX_SUPPORTED_SO to 35.
01/17/01   lcc     Added function prototypes for snm_is2000_scr_init and
                   snm_is2000_nnscr_init.
11/02/00   lcc     Added function snm_eval_so for FEATURE_COMPLETE_SNM.
06/22/00   va      Support for caching the pref RC NV item.
06/12/00   va      Support for mux negotiation
06/05/00   lcc     Corrected some featurization.
06/02/00   lcc     Added prototypes for snm_reg_rlp_blob_generator,
                   snm_get_complete_pending_config, and snm_get_current_nnscr.
05/05/00   lcc     Put back the prototype of snm_initialize_connection when FEATURE_IS2000
                   is not defined.  It was removed by accident.
04/24/00   lcc     Added rlp_blob related stuff.  Separated snm_update_configuration
                   into 2 functions: snm_update_configuration and snm_continue_update_configuration
                   Some minor cleanup as well.
04/17/00   va      Made snm_get_pref_RCs return nothing.
04/05/00   va      Support for mux opt list.
03/15/00   ry      Added IS2000 featurization
02/03/00   lcc     Added support for RC list.
11/15/99   va      IS2000 Voice Changes:
                   snm_get_pref_rcs() function.
                   Support for storing non negotiable service configuration and
                   evaluating it snm_eval_non_neg_scr()
07/30/99   lcc     Added typedef for snm_eval_status_type.
07/19/99   nmn     Added declaration of snm_get_alt_so_list, surrounded by
                   #ifdef of FEATURE_IS95B or FEATURE_SPECIAL_MDR.
06/30/99   kmp     Remove #includes of msg.h and err.h since they are not
                   necessary.
04/29/99   lcc     Corrected some comments and some cleanup.
02/16/99   nmn     Cleaned, linted, and tested SOCM support
02/12/99   lcc     Added several function to get information from SNM as well as
                   changed some names to conform to standard.
12/09/98   lcc     Changed snm_initialize_connection to return a status.
12/03/98   lcc     Minor comment related changes.  Code not affected.
11/25/98   lcc     Initial release

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "cai.h"
#include "caii.h"
#include "dec.h"


/*===========================================================================

                      SOME CONSTANTS USED IN THIS FILE

===========================================================================*/
#define MAX_ALT_SO        CAI_AC_ALT_SO_MAX   // Maximum number of
                                              // alternate service options
#define MAX_COMP_SO       2   // Maximum number of compatible service options
#ifdef UNIT_TEST
  #define MAX_SUPPORTED_SO  8
#else
  #define MAX_SUPPORTED_SO  35  // Reserve enough space for all SOs we support now
#endif
#define MAX_CON           1   // Maximum number of service option connections
                              // in a service configuration
#ifdef FEATURE_IS2000
#define RCLIST_MAX        3   // Maximum number of RCs in negotiation parameters
#endif /* FEATURE_IS2000 */

/*===========================================================================

                      TYPE DEFINITIONS

===========================================================================*/
typedef enum
{
  SNM_VALID,                  // The fields were completely valid
  SNM_INVALID_MSG_TYP,        // Reject: Bad Message Type
  SNM_INVALID_FIELD_VALUE,    // Reject: Invalid Field Value
  SNM_MESSAGE_NOT_SUPPORTED,  // Reject: Message not Supported
  SNM_UNSPEC                  // Reject: Unspecified reason
} snm_socm_return_type;

typedef enum
{
  SNM_FAILURE = FALSE,
  SNM_SUCCESS
} snm_status_type;

// Type to indicate the result of the evaluation of a service configuration.
typedef enum
{
  SNM_REJECT_CFG = 0,
  SNM_ACCEPT_CFG,
  SNM_ALT_CFG
} snm_eval_status_type;

// Type to indicate the status of a service option
typedef enum
{
  SNM_DISABLED,
  SNM_ENABLED
} snm_so_status_type;

// Type to mux. options to use in a service configuration
// Used by function snm_gen_config
typedef enum
{
  SNM_DEFAULT_MUX,
  SNM_MAXIMUM_MUX
} snm_mux_config_type;

// Type to define the default configuration for a service option
typedef struct
{
  struct {
    word  mux;              // Multiplex option
    dec_rate_set_type   rate_set;
    byte  tx_rates;         // Transmission rates used
    cai_traffic_type_type  allowed_traffic_type;
  } forward;
  struct {
    word  mux;              // Multiplex option
    dec_rate_set_type   rate_set;
    byte  tx_rates;         // Transmission rates used
    cai_traffic_type_type  allowed_traffic_type;
  } reverse;
} snm_config_type;

// Type to indicate forward and reverse multiplex options
typedef struct
{
  word  forward;    // Forward link multiplex option
  word  reverse;    // Reverse link multiplex option
} snm_mux_pair_type;

// Type to indicate forward and reverse rate set
typedef struct
{
  dec_rate_set_type  forward;    // Forward link rate set
  dec_rate_set_type  reverse;    // Reverse link rate set
} snm_rate_set_pair_type;

#ifdef FEATURE_IS2000
typedef struct
{
  byte num_of_rc;
  cai_radio_config_type list[RCLIST_MAX];
} snm_rc_list_type;

/* SCH mux Option list type */
typedef struct
{
  byte num_mos;
  word list[CAI_MAX_MULT_OPT_SCH];
} snm_sch_mo_list_type;

typedef enum
{
  SNM_FORWARD,
  SNM_REVERSE
} snm_fwd_rev_type;
#endif

// Type to define the negotiation parameters of a service option
typedef struct
{
  byte  min_p_rev;    // Minimum P_REV to accept this service option
  // Alternate to suggest if disabled or rejected.  Terminated by CAI_SO_NULL.
  word  alternate_so[MAX_ALT_SO];
  // Alternate to suggest if SO not supported.  Terminated by CAI_SO_NULL.
  word  compatible_so[MAX_COMP_SO];
  snm_mux_pair_type  max_mux_to_use;
  #ifdef FEATURE_IS2000
  snm_rc_list_type  forward_rc;
  snm_rc_list_type  reverse_rc;
  snm_sch_mo_list_type fwd_sch_mo_list;
  snm_sch_mo_list_type rev_sch_mo_list;
  #endif
} snm_neg_par_type;

// Type to capture the characteristics of each service option
typedef struct
{
  word  service_option_num;       // Service option number
  snm_so_status_type status;
  word page_response_so;          // Service option to respond when paged
  snm_neg_par_type negotiation_pars;
  snm_config_type default_config; // Parameters for generating default config.
  // Points to initializer of connection
  void (*connection_initializer)(const caii_srv_cfg_type *cfg, word con);
  // Points to SOCM handler
  snm_socm_return_type (*socm_handler)(word length, byte* field, word serv_opt);
  #ifdef FEATURE_IS2000
  uint8 (*rlp_blob_generator)(byte* blob);
  #endif
} snm_so_descriptor_type;

typedef struct
{
  word neg_seed;                      // Negotiation seed
  word beginning_so;                  // First SO used - i.e.
                                      // SO in orig or page response
  word next_so_index;                 // Points to next SO to propose
  word* alt_so_list;                  // List of alternate service options
  // Pointers to service option descriptors of each connection in config.
  snm_so_descriptor_type* connection_sod;
  // Whe we support multiple connecitons, above members may have to be in
  // arrays

  boolean max_mux_proposed;           // Indicates if max mux. options was proposed
  boolean current_config_valid;       // Indicates if current_config contains a negotiated config.
  caii_srv_cfg_type* proposed_config; // Points to proposed configuration
  caii_complete_srv_cfg_type* pending_config;  // Points to pending configuration, neg and non-neg
  caii_complete_srv_cfg_type* current_config;  // Points to current negotiable configuration
                                      // No need to store non-neg SCR
  #ifdef FEATURE_IS2000
  cai_radio_config_type snm_pref_for_rc;
  boolean  snm_pref_rc_valid;
  word prev_so;   /* Previous SO we were evaluating */
  byte current_for_rc_index; /* Maintain entry for only fwd RC list*/
  byte current_f_mux_index;
  byte current_r_mux_index;
  #endif
} negotiation_info_type;

typedef struct
{
  word total_so;    // Number of SOs in array
  snm_so_descriptor_type so[MAX_SUPPORTED_SO];
} snp_type;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION snm_find_so

DESCRIPTION
  This function locates a SO in SNP.  It returns failure is the SO is not
  in SNP.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

snm_status_type snm_find_so
(
  word  so,   // Service option number
  word* index // Variable to hold index to SO found
);

/*===========================================================================

FUNCTION snm_initialize_snm

DESCRIPTION
  This function initializes the Service Negotiation Manager (SNM).  For the time
  being, all it does is to initialize SNP to contain no service options and to
  initialize some members in negotiation_info that needs initialized only once.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void snm_initialize_snm
(
  void
);

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION snm_set_user_pref_for_rc

DESCRIPTION
  This function caches the preferred  for RC value that is read from NV by MC.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void snm_set_user_pref_for_rc( cai_radio_config_type pref_for_rc  );
#endif

/*===========================================================================

FUNCTION snm_add_so

DESCRIPTION
  This function adds a service option descriptor to SNP.

DEPENDENCIES
  SNP must be initialized (snm_initialize_snm does it).

RETURN VALUE
  SUCCESS if the SO is added successfully;otherwise if there is no more
  room in SNP, or if a same SO already exists, it returns FAILURE.

SIDE EFFECTS
  None.

===========================================================================*/

snm_status_type snm_add_so
(
  word  so_number,  // Service option number
  snm_neg_par_type*  neg_par,
  const snm_config_type* default_config
);

/*===========================================================================

FUNCTION snm_get_page_response_so

DESCRIPTION
  This function returns the page_response_so member of the SO descriptor of
  the service option passed in as parameter.  If the SO is not found in SNP,
  the same service option is returned.

DEPENDENCIES
  None.

RETURN VALUE
  See description.

SIDE EFFECTS
  None.

===========================================================================*/

word snm_get_page_response_so
(
  word  so_number // Service option number
);

/*===========================================================================

FUNCTION snm_set_page_response_so

DESCRIPTION
  This function changes the page_response_so member of a SO descriptor.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if page response changed successfully; otherwise if the SO is not
  found in SNP, it returns FAILURE.

SIDE EFFECTS
  None.

===========================================================================*/

snm_status_type  snm_set_page_response_so
(
  word  so_number,  // Service option number
  word  response_so
);

/*===========================================================================

FUNCTION snm_enable_so

DESCRIPTION
  This function enables a service option.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if SO successfully enabled; otherwise if the SO is not found in SNP,
  FAILURE is returned.

SIDE EFFECTS
  None.

===========================================================================*/

snm_status_type snm_enable_so
(
  word so_number
);

/*===========================================================================

FUNCTION snm_disable_so

DESCRIPTION
  This function disables a service option.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if SO successfully disabled; otherwise if the SO is not found in SNP,
  FAILURE is returned.

SIDE EFFECTS
  None.

===========================================================================*/

snm_status_type snm_disable_so
(
  word so_number
);

/*===========================================================================

FUNCTION snm_get_rate_set

DESCRIPTION
  This function is used to obtain the rate set used in forward and reverse
  links for a given service option.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if SO is located in SNP; otherwise FAILURE is returned.

SIDE EFFECTS
  None.

===========================================================================*/

snm_status_type snm_get_rate_set
(
  word so_number,
  snm_rate_set_pair_type* rate_set_pair
);

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION snm_get_pref_RCs

DESCRIPTION
  This function returns the preferred RC for the forward and reverse primary
  channel. This preference is sent back in the page resp or the origination message.
  The SNM has a RC list stored for each SO and it returns the first in the list.
  For SOs not managed by SNM, it calls a corresponding function in srv

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void snm_get_pref_RCs
(
  word so, // The SO for which pref RCs are needed
  cai_radio_config_type * fwd_rc_ptr, // Storage for returned fwd RC
  cai_radio_config_type * rev_rc_ptr // Storage for return  rev RC
);

/*===========================================================================

FUNCTION snm_get_user_pref_for_rc

DESCRIPTION
  This function returns the user perferred RC as stored in NV that is
  cached by SNM.
  We need an interface for this because this value is needed by SRV also.
  Once it's all SOs are migrated to SNM, this value will be local to SNM.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if there is a user preferred RC, FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean snm_get_user_pref_for_rc
(
  cai_radio_config_type * user_pref_for_rc
);

#endif

/*===========================================================================

FUNCTION snm_eval_paged_so

DESCRIPTION
  This function evaluates the SO paged by the base station (after translating
  with snm_get_page_response_so).  It does it by looking into SNP and see
  if the SO exists.  If it does, negotiation_info is updated with this SO as
  the seed for negotiation.  If this SO is enabled, the same SO is returned;
  otherwise, the list of alternative SOs will be traversed until the first
  enabled SO is found.  If an alternative does not exist or if they are all
  disabled, CAI_SO_REJ will be returned.

  If the SO is not in SNP, the list of compatible SOs (compati-ble_service_options)
  of each SO will be scanned.  The first enabled SO containing the evaluated SO
  in its compatible SO list will be returned, which will also be used as the seed.
  If this fails also, CAI_SO_REJ is returned.

  This function also initializes negotiation_info to prepare for negotiation
  on the traffic channel.

DEPENDENCIES
  None.

RETURN VALUE
  If SO is supported, the same SO is returned; otherwise an alternate SO is
  returned.  If no alternate SO exists, CAI_SO_REJ is returned.

SIDE EFFECTS
  None.
===========================================================================*/

word snm_eval_paged_so
(
  word paged_so
    /* The service option number to be evaluated */
);

/*===========================================================================

FUNCTION snm_so_is_enabled

DESCRIPTION
  This function evaluates if a SO can be used under the current operating
  conditions.  For SNM managed SOs, SNP will be consulted to see if
  the SO is enabled.  For other SOs, srv_eval_so is used, which support
  smooth transition of SOs to SNM.  Eventually, when all SOs are captured
  in SNP, the call to srv_eval_so will be obsolete.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if SO is supported, FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/

boolean snm_so_is_enabled
(
  word so
    /* The service option number to be evaluated */
);

/*===========================================================================

FUNCTION snm_gen_config

DESCRIPTION
  This function builds a service configuration with default or max. mux.
  for a specified service option based on information stored in SNP.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS   - Service configuration successfully built.
  FAILURE   - service option not in SNP

SIDE EFFECTS
  None.

===========================================================================*/

snm_status_type snm_gen_config
(
  word  so,
    // The service option for which a default service configuration
    // is to be constructed

  caii_srv_cfg_type  *cfg_ptr,
    // Pointer to place to construct the default configuration.

  boolean  set_connection,
    // Specifies whether or not to include a service option connection
    // in the default service configuration.
  snm_mux_config_type mux
    // Specifies whether default or maximum mux. options to use
);

/*===========================================================================

FUNCTION snm_eval_config

DESCRIPTION
  This function evaluates a specified service configuration to see if it is
  supported by the mobile station.  If it is, it returns with SUCCESS status.
  There are several failure scenarios:
  1. Config. contains more than 1 connection
  2. Service option not supported or enabled
  3. Mux. option not acceptable
  For (1), the list of SOs is scanned through to see if one of them can be
  supported.  If so, a configuration will be generated as a counter-proposal;
  otherwise, it will be handled like (2).  For (2), starting from the first SO,
  an alternative for that SO will be sought.  If such an SO is located, a
  configuration based on that will be proposed.  If not found, the error status
  will be indicated in negotiation_info.  For (3), a configuration will be
  generated by taking the min. of the base station's mux. options and the mobile's
  max. mux. options (unless max. mux. options have not been proposed before and
  if so, max. mux. will be proposed).  If there is no alternative to propose,
  the return config., the first connection will be set to CAI_SO_REJ.

DEPENDENCIES
  negotiation_info must have been initialized.

RETURN VALUE
  SUCCESS if configuration is acceptable, FAILURE otherwise.

SIDE EFFECTS
  negotiation_info is updated if necessary.
===========================================================================*/

snm_eval_status_type snm_eval_config
(
  caii_srv_cfg_type *cfg_ptr,
    // The service configuration to be evaluated
  caii_srv_cfg_type *alt_cfg_ptr,
    // Location to put alternate service configuration if desired
  boolean suggest_alt
    // Indicates if a alternative configuration is to be generated
);

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION snm_eval_non_neg_scr

DESCRIPTION
  This function evaluates the non negotiable service configuration that
  is passed into it.  This makes sure we can accept all the parameters that
  the BS is giving us. This record needs to be evaluated along with the SCR
  to make sure if we have valid FPC parameters if we operating in new RCs etc.
  Non negotiable SCRs unlike SCRs are cumulative i.e new record does not completely
  wipe out the old record. We modify what is specified in this record and leave
  the rest as is. It is important to reset this record in current config properly.

DEPENDENCIES

RETURN VALUE
  SNM_SUCCESS if configuration is acceptable, SNM_FAILURE otherwise.

SIDE EFFECTS
===========================================================================*/

extern snm_status_type snm_eval_non_neg_scr
(
  caii_non_neg_srv_cfg_type *nn_cfg_ptr,
    // The Non-Neg service configuration record to be evaluated
  caii_srv_cfg_type *neg_cfg_ptr
    //  The SCR that goes with the Non-neg part
);

/*===========================================================================

FUNCTION snm_reg_rlp_blob_generator

DESCRIPTION
  This function binds a RLP blob generator to a service option descriptor.
  If an old blob generator already exists, it overwrites it.

DEPENDENCIES
  None

RETURN VALUE
  SNM_SUCCESS if generator registered; SNM_FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_reg_rlp_blob_generator
(
  word so,
  uint8 (*rlp_blob_generator)(byte* blob)
);
#endif

/*===========================================================================

FUNCTION snm_orig_init

DESCRIPTION
  Performs some SNM initialization required for originations.

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS if successfully initialized; FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_orig_init
(
  word  originating_so
);

/*===========================================================================

FUNCTION snm_gen_son_config

DESCRIPTION
  Generates the configuration established with Service Option Negotiation.

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS if successfully initialized; FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_gen_son_config
(
  word  so
);

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION snm_update_non_neg_configuration

DESCRIPTION
  This function is called precisely at the action time of Service Connect 
  Message or General Handoff Direction Message or Universal Handoff Direction
  Message to update the non-negotiatiable current configuration to what was
  given in the message.  

DEPENDENCIES
  negotiation_info.pending_cfg must point to the pending configuration.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void snm_update_non_neg_configuration
(
  caii_non_neg_srv_cfg_type nnscr
);
#endif /* FEATURE_IS2000 */

/*===========================================================================

FUNCTION snm_update_configuration

DESCRIPTION
  This function is called precisely at the action time of Service Connect Message
  to update the current configuration to what was given in the message.  Note that
  this function can be called from device driver's context (e.g. RXC) so it must
  only perform the absolute minimum.  This function perform the portion of SCR
  processing that's time critical (which includes calling the connection initializer).
  The function snm_activate_config does the other part of SCR processing that's not
  time critical.

DEPENDENCIES
  negotiation_info.pending_cfg must point to the pending configuration.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void snm_update_configuration
(
  void
);

/*===========================================================================

FUNCTION snm_continue_update_config

DESCRIPTION
  This function is called at the action time of Service Connect Message to
  perform any processing related to the SCR that's not time critical and is
  called from MC's context.

DEPENDENCIES
  snm_update_configuration must have be called prior to this so that the
  current configuration contains what to be used.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void snm_continue_update_config
(
  void
);

#ifndef FEATURE_IS2000
/*===========================================================================

FUNCTION snm_initialize_connection

DESCRIPTION
  Calls the initializer function (if defined) of a particular connection of
  the current configuration.

DEPENDENCIES
  None

RETURN VALUE
  SNM_SUCCESS if the initializer was called; SNM_FAILURE if initializer was not called
  for any reason (e.g. not registered).

SIDE EFFECTS
  None
===========================================================================*/
snm_status_type snm_initialize_connection
(
  word index
);
#endif  // !FEATURE_IS2000

/*===========================================================================

FUNCTION snm_first_orig_config

DESCRIPTION
  Generates the first configuration to be proposed for an origination.  Max.
  mux. options will be used.  The generated configuration will be placed in
  *negotiation_info.proposed_cfg (pointing to srv_proposed_cfg).
  negotiation_info.max_mux_proposed will be set to TRUE.

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS if configuration successfully generated; FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_first_orig_config
(
  word so
);

/*===========================================================================

FUNCTION snm_reg_connection_initializer

DESCRIPTION
  This function binds a connection initializer to a service option descriptor.
  If an old initializer already exists, it overwrites it.

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS if initializer registered; FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_reg_connection_initializer
(
  word so,
  void (*initializer)(const caii_srv_cfg_type*, word )
);

/*===========================================================================

FUNCTION snm_get_total_SOs

DESCRIPTION
  This function returns the total number of service options maintained by SNM.

DEPENDENCIES
  SNM must be initialized.

RETURN VALUE
  Number of SOs in SNM.

SIDE EFFECTS
  None
===========================================================================*/

word snm_get_total_SOs
(
  void
);

/*===========================================================================

FUNCTION snm_get_SO_number

DESCRIPTION
  This function returns service option number of the nth SO in SNM.

DEPENDENCIES
  SNM must be initialized.

RETURN VALUE
  Service option number of quried SO.  If the entry does not exist, CAI_SO_REJ
  is returned.

SIDE EFFECTS
  None
===========================================================================*/

word snm_get_SO_number
(
  word index
);

/*===========================================================================

FUNCTION snm_get_current_config

DESCRIPTION
  This function returns the current negotiable part of service configuration.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if valid configuration has been negotiated.  FAILURE otherwise.
  If successful, pointer to current config is returned in function parameter.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_get_current_config
(
  const caii_srv_cfg_type  **config
);

/*===========================================================================

FUNCTION snm_get_complete_pending_config

DESCRIPTION
  This function returns a pointer to the complete pending service configuration.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

const caii_complete_srv_cfg_type* snm_get_complete_pending_config
(
   void
);

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION snm_get_current_nnscr

DESCRIPTION
  This function returns the current non-negotiable part of service configuration.

DEPENDENCIES
  None.

RETURN VALUE
  SNM_SUCCESS if valid configuration has been negotiated.  SNM_FAILURE otherwise.
  If successful, pointer to current config is returned in function parameter.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_get_current_nnscr
(
  const caii_non_neg_srv_cfg_type  **nnscr
);
#endif  // FEATURE_IS2000

/*===========================================================================

FUNCTION snm_set_max_for_sups

DESCRIPTION
  This function changes the maximum number of forward supplementals to be used
  with a service option.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if serivce option exists and number of supplementals are valid;
  FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_set_max_for_sups
(
  word  service_option,
  byte  num_sup
);

#if defined(FEATURE_IS2000) && defined(FEATURE_3GDATA_PROTOTYPE)
/*===========================================================================

FUNCTION snm_set_rc_list

DESCRIPTION
  This function changes the specified RC list of a service option.

DEPENDENCIES
  None.

RETURN VALUE
  SNM_SUCCESS if serivce option exists and number of RCs is valid;
  SNM_FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_set_rc_list
(
  word              service_option,
  snm_fwd_rev_type  fwd_rev,
  snm_rc_list_type* rc_list
);

/*===========================================================================

FUNCTION snm_set_mux_list

DESCRIPTION
  This function changes the specified SCH mux list of a service option.
  The DS can alter its preference of mux options.

DEPENDENCIES
  None.

RETURN VALUE
  SNM_SUCCESS if serivce option exists and number of RCs is valid;
  SNM_FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_set_sch_mux_list
(
  word              service_option,
  snm_fwd_rev_type  fwd_rev,
  snm_mux_list_type* mux_list
);

/*===========================================================================

FUNCTION snm_set_min_p_rev

DESCRIPTION
  This function changes the min_p_rev of a service option.  This is a test
  function used for internal testing only.

DEPENDENCIES
  None.

RETURN VALUE
  SNM_SUCCESS if serivce option exists.
  SNM_FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_set_min_p_rev
(
  word              service_option,
  byte              p_rev
);
#endif // defined(FEATURE_IS2000) && defined(FEATURE_3GDATA_PROTOTYPE)

#if defined(FEATURE_SKT_95C_DS) || defined(FEATURE_SKT_SOCK_SUP_95AC) || defined(FEATURE_SKT_INTRAFFIC_MONEGO)	//	khekim 00/12/16
extern	snm_status_type snm_set_sch_mux_list
(
  word              service_option,
  snm_fwd_rev_type  fwd_rev,
  const snm_sch_mo_list_type	*mux_list
);

extern snm_status_type snm_set_rc_list
(
  word              service_option,
  snm_fwd_rev_type  fwd_rev,
  snm_rc_list_type* rc_list
);
#endif	//	#if defined(FEATURE_SKT_95C_DS) || defined(FEATURE_SKT_SOCK_SUP_95AC) || defined(FEATURE_SKT_INTRAFFIC_MONEGO)

/*===========================================================================

FUNCTION snm_set_cont_msg_handler

DESCRIPTION
  This function sets the Service Option Control Message handler function
  pointer.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS if serivce option exists,
  FAILURE otherwise.

SIDE EFFECTS
  None
===========================================================================*/

snm_status_type snm_set_cont_msg_handler
(
  word so,
  snm_socm_return_type (*header_ptr)(word length, byte* field, word serv_opt)
);

/*===========================================================================

FUNCTION snm_process_srv_opt_ctl_msg

DESCRIPTION
  This function returns the Service Option Control Message handler.

DEPENDENCIES
  None.

RETURN VALUE
  Return value from the call to the socm_handler, VALID if no handler
  exists, UNSPEC if the SO does not exist.

SIDE EFFECTS
  None
===========================================================================*/

snm_socm_return_type snm_process_srv_opt_ctl_msg
(
  caii_rx_msg_type* msg_ptr
);

#if defined(FEATURE_IS95B) || defined(FEATURE_SPECIAL_MDR)
/*===========================================================================

FUNCTION snm_get_alt_so_list

DESCRIPTION
  This function evaluates a given service option, returning the appropriate
  list of alternative service options.

DEPENDENCIES
  alt_so points to an array of bytes large enough to handle the maximum
  number of alternative service options allowed (max_alt_so).  This
  maximum is the lesser of the two values: max_num_alt_so received from
  ESPM, or internal array size (defined in cai.h as CAI_AC_ALT_SO_MAX).

  The so_num must have already been checked to be a valid service option.

RETURN VALUE
  Returns the number of alternative service options in the alt_so list.

SIDE EFFECTS
  The values that alt_so point to will be changed to the alternative
  service options.
===========================================================================*/

byte snm_get_alt_so_list
(
  word so_num,
  byte max_alt_so,
  word alt_so[]
);

#endif //FEATURE_IS95B || FEATURE_SPECIAL_MDR

#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO
extern void	GetCurrentMuxOptionFromSO
(
	word	nSO,
	word	*pwFwdMuxOption,
	word	*pwRevMuxOption
);
#endif	//	#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO


#ifdef FEATURE_COMPLETE_SNM
/*===========================================================================

FUNCTION snm_eval_so

DESCRIPTION
  This function evaluates the SO specified in the function parameter and returns
  the same SO if it is supported, otherwise a alternative SO is returned.  If there
  is no alternative, CAI_SO_REJ is returned.  Unlike snm_eval_paged_so, the function
  does not affect the internal state of SNM and thus can be called at any time.
  Note 1: If the SO is not defined in SNM, no attempt is made to look for its
  occurrence in the compatible SO lists.
  Note 2: p_rev_in_use is not considered in evaluating the SO so the returned SO may
  not be allowed in the current p_rev.

DEPENDENCIES
  None.

RETURN VALUE
  See description.

SIDE EFFECTS
  None.
===========================================================================*/

uint16 snm_eval_so
(
  uint16 so
    /* The service option number to be evaluated */
);
#endif // FEATURE_COMPLETE_SNM

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION snm_is2000_scr_init

DESCRIPTION
  Initialize the IS2000 fields in a SCR.  Useful when dealing with P_REV lower
  than 6 service configurations.

DEPENDENCIES
  Input configuration must have valid non-IS2000 fields filled.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void snm_is2000_scr_init
(
  caii_srv_cfg_type  *cfg_ptr
    // Pointer to configuration to be initialized
);

/*===========================================================================

FUNCTION snm_is2000_nnscr_init

DESCRIPTION
  Initialize the fields in a NNSCR.  Useful when dealing with P_REV lower
  than 6 service configurations.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void snm_is2000_nnscr_init
(
  caii_non_neg_srv_cfg_type  *nnscr
    // Pointer to nnscr to be initialized
);
#endif // FEATURE_IS2000

#endif /* SNM_H */
