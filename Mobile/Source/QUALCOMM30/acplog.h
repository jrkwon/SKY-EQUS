#ifndef ACPLOG_H
#define ACPLOG_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                Analog Call Processing Log Interface

GENERAL DESCRIPTION
   This file contains all the definitions necessary to perform
   logging of AMPS data.   
    
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/COMMON/vcs/acplog.h_v   1.1   22 Nov 2000 07:30:48   jqiu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/15/00   jq      Changed the old logging API to the new more efficient API
11/10/00   jq      Merge the file from QCP/KWC.
09/28/99   nnz     Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "log.h"

#if defined(FEATURE_DIAG_V7_LOG_COMPATIBILITY)
/* General Analog Log Entry Type */
typedef struct
{
  log_desc_type desc;
  log_analog_info_type entry;   
} acp_log_gen_type;

/* Analog FORW/REVS Channel Log Entry Type */
typedef struct 
{
  log_desc_type desc;
  log_analog_channel_type entry;   
} acp_log_channel_type;

/* Analog Handoff Log Entry Type */
typedef struct               
{
  log_desc_type desc;
  log_analog_handoff_type entry;   
} acp_log_handoff_type;

/* Word Sync Count Info Log Entry Type */
typedef struct 
{
  log_desc_type desc;
  log_analog_ws_count_type entry;
} acp_log_ws_count_type;

/* FM Slot Stats Log Entry Type */
typedef struct 
{
  log_desc_type desc;
  log_analog_fmslot_type entry;
} acp_log_fmslot_type;
#endif



/*===========================================================================

FUNCTION ACP_LOG_INIT

DESCRIPTION
   This function initializes the free queues and related buffers for logging 
   AMPS information. This function should be called during start up one time
   for initialization.
   
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS


===========================================================================*/
extern void acp_log_init(void);


/*===========================================================================

FUNCTION ACP_PUT_GEN_LOG

DESCRIPTION
   This function logs the general analog info such as Channel, SID, Power,
   RSSI and SAT. When enabled this function logs data every 250ms. 
      
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS


===========================================================================*/
extern void acp_put_gen_log
(
  log_analog_info_type *log_data_ptr
);

/*===========================================================================

FUNCTION ACP_PUT_CHANNEL_LOG

DESCRIPTION
   This function logs the data on the FORWARD and REVERSE Channels. It logs 
   both the Voice and Control Channel information in either direction. 
         
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS


===========================================================================*/
extern void acp_put_channel_log
(
  log_analog_channel_type *log_data_ptr
);


/*===========================================================================

FUNCTION ACP_PUT_FMSLOT_LOG

DESCRIPTION
   This function logs the FM_SLOTTED statistics periodically. 
            
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS


===========================================================================*/
extern void acp_put_fmslot_log
(
  log_analog_fmslot_type *log_data_ptr
);


/*===========================================================================

FUNCTION ACP_PUT_WS_COUNT_LOG

DESCRIPTION
   This function logs the Word Sync Count periodically. 
            
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS


===========================================================================*/
extern void acp_put_ws_count_log
(
  log_analog_ws_count_type *log_data_ptr
);


/*===========================================================================

FUNCTION ACP_PUT_HANDOFF_LOG

DESCRIPTION
   This function is called whenever a Handoff occurs in the Analog Domain to
   log handoff specific data. 
               
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS


===========================================================================*/
extern void acp_put_handoff_log
(
  log_analog_handoff_type *log_data_ptr
);
#endif /* ACPLOG_H */

