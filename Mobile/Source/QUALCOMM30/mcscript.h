#ifndef MCSCRIPT_H
#define MCSCRIPT_H
/*===========================================================================

                  M C S C R I P T   H E A D E R    F I L E

DESCRIPTION
 This file contains function prototypes and definition for MC scripter
 module.

 Copyright (c) 1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/mcscript.h_v   1.0   Aug 11 2000 11:44:08   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/06/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           ram     For Trimode targets, made the AMPS Idle Timout value
                   to 120 seconds.
01/26/98   ych     mcscript_scan_type enum is modified to add an item. Also,
                   MCSCRIPT_SCAN_TYPE_COUNT has been moved out of enum and
                   #defined.                   
11/21/98   dna     Fix CR7142.  Change AMPS timeout to 2 min. for dual-mode.
08/12/98   ych     Fixed the PVCS header problem
08/06/98   ych     Fixed the Lint warnings
08/04/98   ych     Changed the MCSCRIPT_AMPS_IDLE_TIMEOUT to 60 secs from 180
                   secs.
04/16/98   ych     Created file.
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "db.h"
#include "nv.h"
#include "mcsys.h"
#include "mcsyspr.h"
#include "clk.h"


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Try on all the sytems from the given list. */
#define MCSCRIPT_DO_ALL 255

/* Starting row must be between 0 and 254. */
#define MCSCRIPT_MIN_START_ROW 0
#define MCSCRIPT_MAX_START_ROW 254

/* Number of entries scanned must be between 1 and 254. */
#define MCSCRIPT_MIN_LIMIT 1
#define MCSCRIPT_MAX_LIMIT 254

/* If the wrap_index argument of ...WRAP_...cmds is this, */
/* do not wrap.                                            */
#define MCSCRIPT_NOWRAP 255

/* While in rescan mode, for each idle_time_out, mobile shall scan */
/* for upto approximately 10 secs before returning to last system  */
/* with service.                                                   */
#define MCSCRIPT_SCAN_TIMEOUT 10

/* Idle time outs for AMPS and CDMA */
#if ( defined( FEATURE_SSPR_800 ) && \
    defined( FEATURE_CDMA_800 ) && \
    defined( FEATURE_CDMA_1900 ) )
/* If it is a Trimode Target, make the AMPS timeout 120 seconds so that
we can stay in AMPS longer and miss less pages. */
#define MCSCRIPT_AMPS_IDLE_TIMEOUT 120
#elif (defined( FEATURE_SSPR_800 ) && !defined( FEATURE_CDMA_1900 ))
/* On 800MHz dual mode systems, it is OK to stay on AMPS a little longer
 * in order to miss less pages.  Dual band phones are typically roaming
 * on AMPS, so we would rather miss more pages and get back to CDMA sooner.
 */
#define MCSCRIPT_AMPS_IDLE_TIMEOUT 120
#else
#define MCSCRIPT_AMPS_IDLE_TIMEOUT 60
#endif /* (defined( FEATURE_SSPR_800 ) && !defined( FEATURE_CDMA_1900 )) */

#ifdef FEATURE_SKT_CAI
#define MCSCRIPT_CDMA_IDLE_TIMEOUT 0
#else
#define MCSCRIPT_CDMA_IDLE_TIMEOUT 180
#endif

/* Scan_types are enumerations of searcher scan types (like MICRO, */
/* MINI, etc.). The order of this enum's shall be preserved is such*/
/* a way that the scan_types are listed in the order of increasing */
/* sensitivity. This order is used in the mcscript_chk_sys_scanned */
/* Also, note that any time the following enum is changed, make    */
/* to update the MCSCRIPT_SCAN_TYPE_COUNT to the new value.        */
/* MCSCRIPT_SCAN_TYPE_COUNT was made a seperate #define instead    */
/* of the last item in the enum to accomodate ..INIT_SCANTYPE and  */
/* still maintain the correct count of scan types.                 */
typedef enum
{
  MCSCRIPT_INIT_SCANTYPE = 0,
  MCSCRIPT_MICRO = 1,
  MCSCRIPT_MINI = 2,
  MCSCRIPT_POWERON_MINI = 3,
  MCSCRIPT_FAST_FULL = 4,
  MCSCRIPT_FULL = 5
} mcscript_scan_type;

/* Count of Scan types...this is count of scan types defined in */
/* mcscript_scan_type enum                                      */
#define MCSCRIPT_SCAN_TYPE_COUNT 5

/* This enum keeps track of the all the lists. The order of enums's  */
/* shall be preserved as mcscript_list_ptrs[][] is initialized based */
/* on this order. Please refer tomcscript_list_ptrs[][] comments for */
/* more details.                                                     */
typedef enum
{
  MCSCRIPT_ASL = 0,           /* Alternate Scan List */
  MCSCRIPT_SL,                /* Scan List */
  MCSCRIPT_AVAIL,             /* Available List */
  MCSCRIPT_LIST_ID_COUNT      /* List type count */
}mcscript_list_type;

/* This enum keeps track of all the valid script commands. */
typedef enum
{
  OP_START_SCRIPT,
  OP_TAG,
  OP_DO_LIST_CNT,
  OP_SET_WRAP_INDEX,
  OP_DO_LIST_WRAP_CNT,
  OP_DO_LIST_TIME,
  OP_DO_LIST_WRAP_TIME,
  OP_START_TIMER,
  OP_SET_TIME_AWAY,
  OP_TIME_AWAY,
  OP_INC_JUMP_VAR,
  OP_INC_JUMP,
  OP_JUMP,
  OP_JUMP_TIME,
  OP_SET_SCAN_MODE,
  OP_END_SCRIPT,
  COMMAND_COUNT
} mcscript_cmd_type;

/* This enum keeps track of supported MSM versions. */
typedef enum
{
  MCSCRIPT_MSM22 = 0,         /* Ver. 2.2 */
  MCSCRIPT_MSM23,             /* Ver. 2.3 */
  MCSCRIPT_MSMTYPE_COUNT      /* Count */
}mcscript_msm_type;

/* This structure is used to point a given script. */
typedef struct
{
  byte* scr_ptr;
}mcscript_entry_type;

/* Scripter return type, SSPR uses this structure to determine the */
/* system/channel to scan.                                         */
typedef struct {
  mcscript_list_type list_id;       /* Select the next system from this list */
  mcscript_scan_type scan_type;     /* and scan using this scan_type         */
  byte list_row;                    /* on the channel/system at this index   */
  mcsyspr_scan_mode_type scan_mode; /* set the scan_mode to this             */
  int amps_time_out;               /* set the AMPS idle_time_out to this val*/
  int cdma_time_out;               /* set the CDMA idle_time_out to this val*/
}mcscript_rtn_type;

/* this type is used to keep track of the scan_type used on all of the */
/* systems scanned during this pass.                                   */
typedef struct
{
  mcscript_scan_type scan_type;
}mcscript_syslist_type;


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                          External Data                                  */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/* This variable is initialized to the script to be executed and then */
/* passed on to the script engine.                                    */
extern mcscript_entry_type mcscript_rescan_entry;


/*-------------------------------------------------------------------------*/
/*                                                                         */
/*                             Macros                                      */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Macros to replace commands defined through the script.*/
#define START_SCRIPT() \
  (byte) OP_START_SCRIPT

#define TAG(identifier) \
  (byte)OP_TAG, (byte)(identifier)

#define DO_LIST_CNT(list_id, scan_type, start_row, limit) \
  (byte)OP_DO_LIST_CNT, (byte)(list_id), (byte)(scan_type), \
    (byte)(start_row), (byte)(limit)

#define SET_WRAP_INDEX(list_id, scan_type, index) \
  (byte)OP_SET_WRAP_INDEX, (byte) (list_id), (byte) (scan_type), (byte) (index)

#define DO_LIST_WRAP_CNT(list_id, scan_type, limit, wrap_index) \
  (byte)OP_DO_LIST_WRAP_CNT, (byte)(list_id), (byte)(scan_type), \
    (byte)(limit), (byte)(wrap_index)

#define DO_LIST_TIME(list_id, scan_type, start_row, time_in_seconds) \
  (byte)OP_DO_LIST_TIME, (byte)(list_id), (byte)(scan_type), \
    (byte)(start_row), WORD_LO(time_in_seconds), WORD_HI(time_in_seconds)

#define DO_LIST_WRAP_TIME(list_id, scan_type, time_in_seconds, wrap_index) \
  (byte)OP_DO_LIST_WRAP_TIME, (byte)(list_id), (byte)(scan_type), \
  WORD_LO(time_in_seconds), WORD_HI(time_in_seconds), (byte) (wrap_index)

#define START_TIMER() \
  (byte)OP_START_TIMER

#define SET_TIME_AWAY(amps_time_out, cdma_time_out) \
  (byte)OP_SET_TIME_AWAY, WORD_LO(amps_time_out), WORD_HI(amps_time_out), \
    WORD_LO(cdma_time_out), WORD_HI(cdma_time_out)

#define TIME_AWAY() \
  (byte)OP_TIME_AWAY

#define INC_JUMP_VAR(tag_identifier, list_var, scan_var, offset) \
  (byte)OP_INC_JUMP_VAR, (byte) (tag_identifier), (byte) (list_var), \
    (byte)(scan_var), (byte)(offset)

#define INC_JUMP(tag_identifier, count) \
  (byte)OP_INC_JUMP, (byte)(tag_identifier), (byte)(count)

#define JUMP(tag_identifier) \
  (byte)OP_JUMP, (byte)(tag_identifier)

#define JUMP_TIME(tag_identifier, time_in_seconds) \
  (byte)OP_JUMP_TIME, (byte)(tag_identifier), WORD_LO(time_in_seconds), \
    WORD_HI(time_in_seconds)

#define END_SCRIPT() \
  (byte)OP_END_SCRIPT

#define SET_SCAN_MODE(scan_mode) \
  (byte)OP_SET_SCAN_MODE, (byte)(scan_mode)

/* This Macro combines two bytes and returns the resultant word.*/
#define GET_WORD(p) \
  (word) ((*((byte*)(p))) | (*((byte*)(p)+1) << 8))

/* This Macro calculates the MAX_SYS_SCANNED by choosing the max */
/* of MCSYSPR_ALT_SCAN_LIST_SIZE, MCSYSPR_NUM_SCAN_LIST_ENTRIES. */
#define MCSCRIPT_MAX_SYS_SCANNED \
  ((MCSYSPR_ALT_SCAN_LIST_SIZE) > (MCSYSPR_NUM_SCAN_LIST_ENTRIES) ? \
   (MCSYSPR_ALT_SCAN_LIST_SIZE) : (MCSYSPR_NUM_SCAN_LIST_ENTRIES))



/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

  FUNCTION MCSCRIPT_SCRIPT_ENGINE

  DESCRIPTION
    This function is the core scripter function. This function process the
    script passed by the calling function.

  DEPENDENCIES
    None.

  RETURN VALUE
    A Structure with the following elements:

    - list_id is the enumeration of the scan list whose entry shall be used
      by the caller in acquiring service.
    - scan_type is the search scan type (i.e. is MICRO, MINI etc.) that needs
      to be used on the list_id's list_row entry
    - list_row is the index value of the list_id entry that will be next
      used in acquiring service.
    - scan_mode is the SSPR scan mode to be set on exit from the command
      intrepretar.
    - amps_idle_time_out The idle timeout specifies the number of seconds to
      stay in the idle state, when providing service on a less desriable
      AMPS system, before returning to system determination so that more
      desirable systems can be scanned.
    - cdma_idle_time_out The idle timeout specifies the number of seconds to
      stay in the idle state, when providing service on a less desriable
      CDMA system, before returning to system determination so that more
      desirable systems can be scanned.

  SIDE EFFECTS
    None.

===========================================================================*/
extern mcscript_rtn_type* mcscript_engine
(
  mcscript_entry_type* script
);


/*===========================================================================

FUNCTION MCSCRIPT_INIT_RESCAN_SCR

DESCRIPTION
  This function initializes SSPR re_scan script related variables. This
  function will first check for the current MSM version and sets the
  script pointer to point to appropriate script.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void mcscript_init_rescan_scr
(
  void
);
#endif /* MCSCRIPT_H */
