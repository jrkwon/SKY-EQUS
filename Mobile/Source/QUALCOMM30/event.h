#ifndef EVENT_H
#define EVENT_H

/*===========================================================================

                   Event Reporting Services

General Description
  All declarations and definitions necessary to support the static
  system event reporting service.

Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                          Edit History

$Header:   O:/src/asw/COMMON/vcs/event.h_v   1.8.1.4   29 Mar 2001 17:21:40   ldefauw  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/19/01   jal     Changed bit field declarations in event_id_type and
                   event_call_control_state_change_type from uint16 to
                   plain int in order to quiet compiler warnings.
03/13/01   jal     Moved data structure definitions outside
                   FEATURE_DIAG_NO_EVENTS--Call Processing needs some
		   of these to be present, and we don't want them to
		   conditionally compile code based on the presence of
		   this feature.
03/07/01   fc      Fixed merge error.
03/06/01   fc      Changed some event ID names, modified some event
                   payload types (jlindqui checkin)
02/16/01   sfh     Added events and type definitions.
11/17/00   jal     Bit fields in event_id_type came out in inverted order
                   from what we wanted.  Fixed.
11/13/00   lcc     Added event and type definitions for some events.
11/10/00   lad     Made obsolete event_report_data() a macro to NULL.
11/09/00   jal     Took the old event IDs out (again!)
11/07/00   jal     Renamed event_extra_data_type to event_payload_type
10/17/00   jal     Changes for new event accumulation mechanism
05/15/00   lad     Changed truncated timsteamp from uint32 to uint16.
04/11/00   lad     Increased # of customer reserved event IDs to 0x100.
12/16/99   lad     Added support for event reporting service.
01/21/99   lad     Created file.

===========================================================================*/

#include "customer.h"   /* Customer specific definitions.   */
#include "comdef.h"     /* Definitions for byte, word, etc. */
#include "rex.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#ifndef FEATURE_DIAG_NO_EVENTS

#ifdef FEATURE_DIAG_NON_STREAMING
#error Event reporting requires diag version 8 or better!
#endif

/*-------------------------------------------------------------------
 To ensure event reporting reliability, A certain number of bytes
 are reserved for event reporting.  These reside inside the #ifndef
 since the reserved space macro changes based on this feature's
 presence.
-------------------------------------------------------------------*/
#define EVENT_RESERVED_SPACE_SIZE    100 /* buffer for event reports.       */
#define EVENT_MAX_EVENT_BUFFER_SIZE  300 /* maximum size for event buffer.  */
#define EVENT_TIMER_LEN             1000 /* time (ms) for event stale timer */

#endif /* FEATURE_DIAG_NO_EVENTS */

#define EVENT_MAX_PAYLOAD_LEN         80 /* max payload data item length    */

typedef enum
{
  EVENT_DROP_ID = 0,

  EVENT_BAND_CLASS_CHANGE = 0x0100,/* Includes band class as payload */
  EVENT_CDMA_CH_CHANGE,            /* Includes cdma channel as payload */
  EVENT_BS_P_REV_CHANGE,           /* Includes BS p_rev as payload */
  EVENT_P_REV_IN_USE_CHANGE,       /* Includes p_rev_in_use as payload */
  EVENT_SID_CHANGE,                /* Includes SID as payload */
  EVENT_NID_CHANGE,                /* Includes NID as payload */
  EVENT_PZID_CHANGE,               /* Includes PZID as payload */
  EVENT_PDE_SESSION_END,           /* No payload */
  EVENT_OP_MODE_CHANGE,               /* Includes operation mode as payload */
  EVENT_MESSAGE_RECEIVED,             /* Includes channel and message ID as 
                                         payload */
  EVENT_MESSAGE_TRANSMITTED,          /* Includes channel and message ID as 
                                         payload */
  EVENT_TIMER_EXPIRED,                /* Includes timer ID as payload */
  EVENT_COUNTER_THRESHOLD,            /* Includes counter ID as payload */
  EVENT_CALL_PROCESSING_STATE_CHANGE, /* Includes new state and old state as
                                         payload */ 
  EVENT_CALL_CONTROL_INSTANTIATED,    /* Includes con_ref as payload */
  EVENT_CALL_CONTROL_STATE_CHANGE,    /* Includes con_ref, old substate and
                                         new substate as payload */
  EVENT_CALL_CONTROL_TERMINATED,      /* Includes con_ref as payload */
  EVENT_REG_ZONE_CHANGE,              /* Includes reg_zone as payload */ 
  EVENT_SLOTTED_MODE_OPERATION,       /* Includes enter/exit bit as payload */
  EVENT_QPCH_IN_USE,                  /* Includes enable/disable bit as payload */
  EVENT_IDLE_HANDOFF,                 /* Includes pn_offset as payload */
  EVENT_ACCESS_HANDOFF,               /* Includes pn_offset as payload */
  EVENT_ACCESS_PROBE_HANDOFF,         /* Includes pn_offset as payload */
  EVENT_SOFT_HANDOFF,   
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_HARD_HANDOFF_FREQ_CHANGE, 
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_HARD_HANDOFF_FRAME_OFFSET_CHANGE, 
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_HARD_HANDOFF_DISJOINT_ASET,
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_UNSUCCESSFUL_HARD_HANDOFF,    /* No payload */
  EVENT_TMSI_ASSIGNED,                /* Includes TMSI as payload */
  EVENT_SERVICE_NEGOTIATION_COMPLETED,/* No payload */
  EVENT_SO_NEGOTIATION_COMPLETED,     /* No payload */
  EVENT_ENTER_CONTROL_HOLD_MODE,      /* No payload */
  EVENT_EXIT_CONTROL_HOLD_MODE,       /* No payload */
  EVENT_START_FWD_SUPP_BURST_ASSGN,   /* No payload */
  EVENT_END_FWD_SUPP_BURST_ASSGN,     /* No payload */
  EVENT_START_REV_SUPP_BURST_ASSGN,   /* No payload */
  EVENT_END_REV_SUPP_BURST_ASSGN,     /* No payload */
  EVENT_DTX,                          /* No payload */
  EVENT_T_ADD_ABORT,                  /* No payload */
  EVENT_CH_IND_CHANGE,                /* Include ch_ind as payload */
  EVENT_TRANSMITTER_DISABLED,         /* No payload */
  EVENT_TRANSMITTER_ENABLED,          /* No payload */
  EVENT_SMS_RECEIVED,                 /* No payload */
  EVENT_SMS_SENT,                     /* No payload */
  EVENT_INACTIVITY_TIMER_EXPIRED,     /* No payload */
  EVENT_DORMANT_TIMER_EXPIRED,        /* No payload */
  EVENT_ACCESS_ATTEMPT_FAIL_MAX_PROBES_SENT, /* No payload */
  EVENT_ACCESS_ATTEMPT_FAIL_LOSS_OF_PC_OR_FCCC, /* No payload */
  EVENT_PCH_ACQUIRED,                 /* Includes pagech and pn_offset
                                         as payload */
  EVENT_BCCH_ACQUIRED,                /* Includes walsh code for BCCH and 
                                         pn_offset as payload */
  EVENT_FFCH_ACQUIRED,                /* No payload */
  EVENT_FDCCH_ACQUIRED,               /* No payload */
  EVENT_FFCH_PLUS_DCCH_ACQUIRED,      /* No payload */
  EVENT_REGISTRATION_PERFORMED,       /* Includes reg_type as payload */ 
  EVENT_NEW_SYSTEM_IDLE_HANDOFF,      /* No payload */
  EVENT_SYSTEM_RESELECTION,           /* Includes ecio and ps as payload */
  EVENT_RESCAN,                       /* No payload */
  EVENT_PROTOCOL_MISMATCH,            /* No payload */
  EVENT_LOCK,                         /* No payload */
  EVENT_UNLOCK,                       /* No payload */
  EVENT_ACCESS_DENIED,                /* No payload */
  EVENT_NDSS_OFF,                     /* No payload */
  EVENT_RELEASE,                      /* No payload */
  EVENT_ERROR,                        /* No payload */
  EVENT_REDIRECTION,                  /* No payload */
  EVENT_REGISTRATION_REJECTED,        /* No payload */
  EVENT_WRONG_SYSTEM,                 /* No payload */
  EVENT_WRONG_NETWORK,                /* No payload */
  EVENT_LOSS_OF_ACQ_AFTER_SLEEP,      /* No payload */
  EVENT_POWER_DOWN,                   /* No payload */
  EVENT_CALL_RELEASE_REQUEST,         /* No payload */
  EVENT_SERVICE_INACTIVE,             /* No payload */
  EVENT_EXTENDED_RELEASE,             /* No payload */

  EVENT_RSVD_START = 0x0800,

  EVENT_MAX_ID
} event_id_enum_type;


/*--------------------------------------
  Special bit flags in the event ID.
--------------------------------------*/
#define EVENT_PAY_LENGTH   0x3
#define EVENT_PAY_TWO_BYTE 0x2
#define EVENT_PAY_ONE_BYTE 0x1
#define EVENT_PAY_NONE     0x0

/* Use this structure.  It may not be ANSI, but all */
/* our compilers recognize it.                      */
typedef PACKED struct
{
  int id               : 12;
  int reserved         : 1;
  int payload_length   : 2;
  int time_trunc_flag  : 1;
} event_id_type;

typedef PACKED union
{
  event_id_type event_id;

  PACKED struct
  {
    event_id_type  event_id;
    uint64 ts;

  } full_ts;

  PACKED struct
  {
    event_id_type  event_id;
    word ts; /* 2nd and 3rd (from LSB) bytes of the full time stamp */

  } half_ts;
} event_type;

typedef PACKED struct
{
  byte length;
  byte extra_data[1];
} event_payload_type;

typedef PACKED union
{
  event_id_type event_id;

  PACKED struct {
    event_id_type event_id;
    uint64 ts;
    dword drop_cnt;
  } full_ts;

  PACKED struct {
    event_id_type event_id;
    dword ts;
    dword drop_cnt;
  } half_ts;
} event_drop_type;

/*===========================================================================

                        TYPE DEFINITIONS FOR EVENTS

===========================================================================*/
typedef unsigned char event_band_class_change_type;
typedef unsigned short event_cdma_ch_change_type;
typedef unsigned char event_bs_p_rev_change_type;
typedef unsigned char event_p_rev_in_use_change_type;
typedef unsigned short event_sid_change_type;
typedef unsigned short event_nid_change_type;
typedef unsigned short event_pzid_change_type;
typedef unsigned char event_op_mode_change_type;

typedef PACKED struct
{
   unsigned char channel;
   unsigned char msg_id;
} event_message_type;

typedef unsigned char event_timer_expired_type;
typedef unsigned char event_counter_threshold_type;

typedef PACKED struct
{
   unsigned char old_state;
   unsigned char new_state;
} event_call_processing_state_change_type;

typedef unsigned char event_call_control_instantiated_type;

typedef PACKED struct
{
   int con_ref      : 8;
   int old_substate : 4;
   int new_substate : 4;
} event_call_control_state_change_type;

typedef unsigned char event_call_control_terminated_type;
typedef unsigned short event_reg_zone_change_type;
typedef unsigned char event_slotted_mode_operation_type;
typedef unsigned char event_qpch_in_use_type;
typedef unsigned short event_idle_handoff_type;
typedef unsigned short event_ms_access_handoff_type;
typedef unsigned short event_ms_access_probe_handoff_type;

typedef PACKED struct
{
  unsigned short pn[6];
  unsigned char pn_in_sch_aset[6];
} event_ms_handoff_type;

typedef unsigned long event_tmsi_assigned_type;
typedef unsigned char event_end_rev_supp_burst_assgn_type;
typedef unsigned char event_ch_ind_type;

typedef PACKED struct
{
   unsigned char pagech;
   unsigned short pn_offset;
} event_pch_acquired_type;

typedef PACKED struct
{
   unsigned char bcch;
   unsigned short pn_offset;
} event_bcch_acquired_type;

typedef unsigned char event_registration_performed_type;

typedef PACKED struct
{
   unsigned char ecio;
   unsigned char ps;
} event_system_reselection_type;

#ifndef FEATURE_DIAG_NO_EVENTS

extern rex_timer_type     event_stale_timer;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION EVENT_INIT

DESCRIPTION
  Initialize event buffers.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void event_init( void );


/*===========================================================================

FUNCTION EVENT_REPORT

DESCRIPTION
  Report an event.

DEPENDENCIES
  Only one event ID per event.  DO NOT DUPLICATE EVENTS IDS!!!

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void event_report( event_id_enum_type event_id );


/*===========================================================================

FUNCTION EVENT_REPORT_PAYLOAD

DESCRIPTION
  Report an event with payload data.

DEPENDENCIES
  Only one event ID per event.  DO NOT DUPLICATE EVENTS IDS!!!

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void event_report_payload( event_id_enum_type event_id,
                               byte length,
                               void *data
                             );

/*===========================================================================

FUNCTION EVENT_RETIRE

DESCRIPTION
  Retire (commit) the existing event reporting buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The event report buffer will be commited and global variables reset.

===========================================================================*/
extern void event_control
(
  boolean event_on /* TRUE to turn event reporting on. FALSE for OFF */
);

#else /* ifndef FEATURE_DIAG_NO_EVENTS */

#define EVENT_RESERVED_SPACE_SIZE 0

/*----------------------------------------------------
 Since event reporting is not enabled, all external
 functions should be ignored.
----------------------------------------------------*/
#define event_report(x)               0
#define event_report_payload(x, y, z) 0
#define event_control(x)              0

#endif /* FEATURE_DIAG_NO_EVENTS */

/* All calls to event_report_data were temporary, remove them
** via a macro for now.
*/
#define event_report_data(x, y, z) 0

#endif /* EVENT_H */
