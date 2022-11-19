#ifndef RUIMI_H
#define RUIMI_H
/*===========================================================================

                        I N T E R N A L   R U I M
                      T A S K   D E F I N I T I O N S

DESCRIPTION
  This contains all the internal declarations for the RUIM which runs
  in the context of the AUTH Task. It also contains the interface
  between the custom ruim modules and the MAIN RUIM state machine.

Copyright (c) 1995, 1999, 2000 by QUALCOMM, Incorporated.All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/ruimi.h_v   1.1   01 Dec 2000 11:06:48   ckrishna  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/01/00   ck      Added support to parse the esn field in the Update SSD
                   command.
04/27/00   ck      Created Module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "auth.h"
#include "ruim.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

typedef PACKED struct
{
  byte randssd[56];     /* Randssd */
  byte proc_control[8]; /* Process Control bits */
  byte esn[32];
  byte esn_pad1[16];
  byte esn_pad2[8];
} ruim_update_ssd_req_siz_type;

typedef PACKED struct 
{
  byte rands[32];       /* Rands */
} ruim_bs_chal_req_siz_type;

typedef PACKED struct 
{
  byte authbs[24];      /* Randbs */
} ruim_conf_ssd_req_siz_type;

typedef PACKED struct
{
  byte rand_type[8];     /* rand type */
  byte rand[32];         /* rand */
  byte randu[24];        /* rand for unique challenge */
  byte dig_len[8];       /* number of digits */
  byte digits[8];        /* arrray of digits */
  byte proc_control[8];  /* Process control bits */
  byte esn[32];          /* ESN */
  byte esn_pad1[16];     /* padding for ESN */
  byte esn_pad2[8];      /* padding for ESN */
} ruim_run_cave_req_siz_type;

typedef PACKED struct {
  byte auth_sig[24];     /* auth signature */
} ruim_run_cave_rsp_siz_type;

typedef PACKED struct {
  byte randbs[32];       /* RANDBS */
} ruim_bs_chal_rsp_siz_type;

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*=========================================================================*/


/* <EJECT> */                                                               
/*===========================================================================

FUNCTION AUTH_RUIM_COMMAND

DESCRIPTION
  This procedure is the main state machine for RUIM command processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

extern void auth_ruim_command
(
  auth_cmd_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_RUIM_SET_CMD_STATE

DESCRIPTION
  This procedure sets the RUIM state and is called during custom command
  processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets the state of ruim_state variable in auth task.

===========================================================================*/

extern void auth_ruim_set_cmd_state
(
  ruim_state_type state
);

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_RUIM_SET_SEL_ST_CTR

DESCRIPTION
  This procedure sets the RUIM Select state Counter and is called during 
  custom command processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets the value of select_state_ctr variable in auth task.

===========================================================================*/

extern void auth_ruim_set_sel_st_ctr
(
  byte sel_st_ctr
);

/* <EJECT> */
/*===========================================================================

FUNCTION CUSTOM_CMD_PROCESSOR

DESCRIPTION
  This procedure processes the custom commands( implemented by the customers)
  sent to the Authentication task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void custom_cmd_processor
(
  auth_cmd_type *cmd_ptr,
    /* Pointer to received command */
  ruim_req_buf_type *req_ptr,
    /* Pointer to request buffer */
  ruim_rsp_buf_type *rsp_ptr,
    /* Pointer to response buffer */
  ruim_resp_callback_type callback_fn
  /* Callback function pointer */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CUSTOM_RUIM_COMMAND

DESCRIPTION
  This procedure is the state machine for the custom RUIM commands.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

extern void custom_ruim_command
(
  auth_cmd_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION CUSTOM_RUIM_COMMAND_RESPONSE

DESCRIPTION
  This procedure processes the response that has been recd from the RUIM.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

extern ruim_apdu_status_type custom_ruim_command_response
(
 ruim_rsp_buf_type *rsp_ptr,
 auth_cmd_type     *cmd
);

/* <EJECT> */
/*===========================================================================

FUNCTION CUSTOM_PROCESS_RESPONSE

DESCRIPTION
  This procedure processes the response that has been recd from the RUIM 
  at the completion of the command processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

extern void custom_ruim_process_response
(
  ruim_rsp_buf_type *rsp_ptr,
  auth_cmd_type *cmd_ptr
);

/*========================================================================

FUNCTION CUSTOM_SEND_ERR_RPT

DESCRIPTION
  This function sends an error report for the command that is being requested.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Releases the command buffer. 

===========================================================================*/
void custom_send_err_rpt 
(
  auth_cmd_type *cmd_ptr                    /* the command to queue up */
);


#endif /* RUIMI_H */




