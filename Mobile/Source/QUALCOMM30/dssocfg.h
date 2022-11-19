#ifndef DSSOCFG_H
#define DSSOCFG_H
/*===========================================================================

                          D A T A   S E R V I C E S

          S E R V I C E   O P T I O N   C O N F I G U R A T I O N

                           H E A D E R    F I L E

DESCRIPTION
  This header file contains functions that are intended to be used internally
  within the data services unit for configuring and managing data service
  options using the Service Negotiation Manager (SNM).
  
  The Packet Data (LSPD & HSPD), Async Data & G3 Fax service options are
  configured and managed through the Service Negotiation Manager.


   Copyright (c) 1998,1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/dssocfg.h_v   1.0   23 Oct 2000 17:32:16   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/10/00   ak      Added extern for originating pkt so array.
02/07/00   ak      Added extern for prev_change_handler that works with CM.
02/02/00   ak      Added extern for ds_socfg_reg_initializers.
09/01/99   smp     Renamed ds_base_station_prev to ds_socfg_base_station_prev
                   and added explanation for its use.
08/13/99   smp     ds_socfg_set_pkt_page_rsp_so() should be an external (not
                   local) function.
08/11/99   smp     Added functions for async data & g3 fax service options.
                   Also, made some functions local instead of external, in
                   order to simplify the external interface.
04/26/99   smp     Added prototype for ds_socfg_init_service_options().
02/10/99   smp     Added P_REV change handler function and external variable.
12/29/98   smp     Created file.

===========================================================================*/

#ifdef FEATURE_DS_IS2000
#include "dsi.h"
#endif

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_DS_IS2000
/*---------------------------------------------------------------------------
  Table of originating service options, dependent on various phone params
  as well as the BS p_rev.
---------------------------------------------------------------------------*/
extern const word  ds_socfg_pkt_orig_so_tbl[DS_NUM_BS_PREVS]
                                           [NUM_MDR_MODES]
                                           [DS_SO_SET_COUNT]
                                           [DS_RATESET_COUNT];
#endif /* FEATURE_DS_IS2000 */

#ifdef FEATURE_SPECIAL_MDR
/*---------------------------------------------------------------------------
  Stores the current Base Station's Protocol Revision. This is used to check
  if MDR service options should be enabled, for FEATURE_SPECIAL_MDR only.
  MDR service options are enabled only if the Base Station Protocol Revision
  is 5.
---------------------------------------------------------------------------*/
extern byte ds_socfg_base_station_prev;

#endif /* FEATURE_SPECIAL_MDR */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_DS_IS2000
/*===========================================================================

FUNCTION DS_SOCFG_GET_P_REV_INDEX

DESCRIPTION
  Based on the p_rev_in_use, returns the index into the enum list of 
  p_revs.
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

word ds_socfg_get_p_rev_index(void);
#endif /* FEATURE_DS_IS2000 */

/*===========================================================================

FUNCTION DS_SOCFG_INIT_PKT_SO

DESCRIPTION
  This function performs the all the power-up initialization for the packet
  data service options. This includes registering all the packet data service
  options with the SNM and enabling the appropriate service options, based
  on the MDR Mode and Service Option Set in use.

  This function should be called just once when the Data Task is initialized.

DEPENDENCIES
  This function should be called only after ATCOP has been initialized.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_init_pkt_so(void);


/*===========================================================================

FUNCTION DS_SOCFG_SET_PKT_SO_STATE

DESCRIPTION
  This function enables or disables each defined packet data service option,
  based on the MDR Mode and the Service Option Set in use. This function
  should be called each time the MDR Mode or the Service Option Set in use
  changes.

DEPENDENCIES
  Uses the global variable ds_atcop_qcmdr_val to determine the current
  setting of the MDR Mode.
  Uses the global variable ds_atcop_qcso_val to determine the current setting
  of the Service Option Set.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_set_pkt_so_state(void);


/*===========================================================================

FUNCTION DS_SOCFG_SET_PKT_PAGE_RSP_SO

DESCRIPTION
  This function sets the page response service option for each defined packet
  data service option, based on the MDR Mode. By setting the page response
  service option based on the MDR Mode, the service negotiation process with
  the base station can be sped up.

  This function should be called each time the MDR Mode changes.

DEPENDENCIES
  The packet data service options should have been initialized by calling
  ds_socfg_init_pkt_so(), prior to calling this function.
  Uses the global variable ds_atcop_qcmdr_val to determine the current
  setting for the MDR Mode.  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_set_pkt_page_rsp_so(void);


#if defined(FEATURE_DS_IS2000)
/*===========================================================================

FUNCTION DS_SOCFG_PREV_CHANGE_HANDLER

DESCRIPTION
  Enable/Disable the appropriate Packet Data service options and set up the
  originating Packet Data service options, based on the new base station
  protocol revision.  This allows the phone to intelligently know whether
  or not to try IS95B or 3G data service options.
  
  This function is a callback registered with CM in dsi_cm_init.  It is
  called whenever the prev_in_use changes.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void ds_socfg_prev_change_handler
(
  cm_ss_event_e_type         ss_event,      /* Type of change that occured */
  const cm_ss_info_s_type   *ss_info_ptr    /* Holds the new sys info      */
);

#elif defined(FEATURE_SPECIAL_MDR)
/*===========================================================================

FUNCTION DS_SOCFG_PREV_CHANGE_HANDLER

DESCRIPTION
  This function performs the necessary actions, in relation to configuring
  data service options, when the base station protocol revision changes.
  MDR service options are disabled if the base station protocol revision is
  not equal to 5.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The global ds_base_station_prev is changed to new_prev.

===========================================================================*/

void ds_socfg_prev_change_handler
(
  byte  old_prev,
    /* Old base station protocol revision */

  byte  new_prev
    /* New base station protocol revision */
);

#endif /* FEATURE_DS_IS2000 */


/*===========================================================================

FUNCTION DS_SOCFG_INIT_ASYNC_SO

DESCRIPTION
  This function performs the all the power-up initialization for the async
  data service options. This includes registering all the async data service
  options with the SNM and enabling the appropriate service options, based
  on the Service Option Set in use.

  This function should be called just once when the Data Task is initialized.

DEPENDENCIES
  This function should be called only after ATCOP has been initialized.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_init_async_so(void);


/*===========================================================================

FUNCTION DS_SOCFG_SET_ASYNC_SO_STATE

DESCRIPTION
  This function enables or disables each defined async data service option,
  based on the Service Option Set in use. This function should be called
  each time the Serivce Option Set in use changes.

DEPENDENCIES
  Uses the global variable ds_atcop_qcso_val to determine the current setting
  of the Service Option Set.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_set_async_so_state(void);


/*===========================================================================

FUNCTION DS_SOCFG_INIT_G3FAX_SO

DESCRIPTION
  This function performs the all the power-up initialization for the G3 fax
  service options. This includes registering all the G3 fax service options
  with the SNM and enabling the appropriate service options, based on the
  Service Option Set in use.

  This function should be called just once when the Data Task is initialized.

DEPENDENCIES
  This function should be called only after ATCOP has been initialized.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_init_g3fax_so(void);


/*===========================================================================

FUNCTION DS_SOCFG_SET_G3FAX_SO_STATE

DESCRIPTION
  This function enables or disables each defined group 3 fax service option,
  based on the Service Option Set in use. This function should be called
  each time the Serivce Option Set in use changes.

DEPENDENCIES
  Uses the global variable ds_atcop_qcso_val to determine the current setting
  of the Service Option Set.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_set_g3fax_so_state(void);

#ifdef FEATURE_DS_IS2000
/*===========================================================================

FUNCTION DS_SOCFG_REG_INITIALIZERS

DESCRIPTION
  Register initializer functions used by snm when a Service Option is
  connected.

DEPENDENCIES
  This function should only be called after all SO's have been added to the
  SNM.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ds_socfg_reg_initializers(void);
#endif /* FEATURE_DS_IS2000 */
#endif /* DSSOCFG_H */

