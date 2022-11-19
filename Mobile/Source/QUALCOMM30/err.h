#ifndef ERR_H
#define ERR_H
/*===========================================================================

   E R R O R   H A N D L I N G   S E R V I C E S   H E A D E R    F I L E

DESCRIPTION
  The error handling services (ERR) provide services for other units to
  log errors and query on logged errors.  The unit is implemented as a set
  of callable functions.  Errors are stored in NV EEPROM error log array
  which is composed of inidividual error records, one per each unique error
  reported (see NV.H for details of error log).  Errors are classified as
  non-fatal or fatal.  Non-fatal errors return to the caller after the error
  is logged.  Fatal errors cause a reset or powerdown of the system after
  the error is logged.  Depending on the level of MSG reporting enabled,
  errors are also sent to the DM via the MSG unit.  Depending on the
  conditional compile switches defined by the MSG utility, error messages
  can selectively be printed to the DM screen during development.  Upper
  layers within ERR allow storing of errors in a write-through memory cache
  so that a record of current error log and error counts is available for
  immediate query.

Copyright (c) 1992-2000 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/err.h_v   1.3   28 Feb 2000 05:24:10   ldefauw  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/00   lad     Changed macros to parse as single expressions.
06/30/99   jct     Added PC_EMULATOR support for ERR_FATAL, made inclusion
                   of target.h conditional, added decoupling of inclusion of
                   nv.h based on assertion of FEATURE_ERR_DECOUPLE_NVM.  
                   This allows resuse of ERR_FATAL in environment where NV is
                   not supported.
06/02/99   lad     Fixed multi-line macros to evaluate as single expression.
04/28/99   ms/sk   Inserted call to clear SL register in ERR_FATAL.
11/05/98   jct     Merged in changes for ARM targets.  Make calls to err_
                   functions const correct.
01/22/97   rdh     Used Pascal directive on err_*_put_log to save ROM.
07/24/95   jah     Changed ERR/ERR_FATAL() macro to use msg_file if MSG_FILE
                   is defined.
06/18/93   jah     Removed MSG_FATAL from phone version of ERR_FATAL() macro,
                   as it takes code space and the phone resets before the DM
                   can get the results from MSG.
10/29/92   ip      Updated for release of full error logging.
08/03/92   ip      Removed '{' and '}' from ERR_...
07/06/92   ip      Fixed target include files, added '{' and '}' to ERR_...
05/13/92   ip      Removed parentheses around 'format' in MSG_... calls.
03/07/92   ip      First porting of file from brassboard to subscriber.

===========================================================================*/

/*===========================================================================

                              INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#ifdef TG
   #include "target.h"
#endif
#include "msg.h"

/* If the user has requested that ERR be decoupled from NVM, then the
** definition of nv_err_log_type is provided here since nv.h is not 
** included.  Note that this is protected in the event nv.h was
** actually included before the inclusion of err.h.  If the feature
** to decouple NVM from ERR is NOT asserted, then include nv.h as
** normal.  This fasciliates target environments which do not have
** NVM support.
*/ 
#ifndef FEATURE_ERR_DECOUPLE_NVM
   #include "nv.h"
#else
   #ifndef nv_err_log_type
      typedef PACKED struct { 
        byte      address;      /* Storage address 0 to       */
                                /*  NV_MAX_ERR_LOG-1          */
        byte      err_count;    /* Number of occurances       */
                                /* (0=empty,FF=full)          */
        byte      file_name[8]; /* File name string */
        word      line_num;     /* Line number in file */
        boolean   fatal;        /* TRUE if fatal error */
      } nv_err_log_type;
   #endif
#endif


/*===========================================================================

                             DATA DECLARATIONS

===========================================================================*/

/* Status returned from err_get_log and err_clr_log.                       */

typedef enum {
  ERR_DONE_S,        /* Request completed OK */
  ERR_NONE_S         /* No such log number */
} err_stat_type;

/* Typedefs for reporting number of errors logged and ignored.             */

typedef word  err_logged_type;
typedef word  err_ignored_type;


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION ERR_INIT

DESCRIPTION
  This function checks if NV has been built.  If so then it loads up the
  error log cache from NV, it initializes error counts and local variables,
  and it sets error services to online.  If NV has not been built then
  error log is not loaded and error services remain offline and error
  logging does not take place.  Any access to NV is performed here directly,
  without relying on the NV task.  Thus error service can be initialized
  and used early in the life of the DMSS and right after boot.  Note
  that if NV has not been built, error services provide no error logging
  capabilities.  However NV is built once, after initial factory startup,
  and rarely thereafter.  Thus except for the first ever powerup of the
  unit after NV build or re-build, error logging will take place as usual.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Error log is loaded from NVM into RAM resident cache.

===========================================================================*/

extern void err_init (void);


/*===========================================================================

FUNCTION ERR_LOGGED

DESCRIPTION
  Return to the caller the number of unique errors logged.

DEPENDENCIES
  err_init must be called before this call, otherwise the
  value returned is zero.

RETURN VALUE
  The number of unique errors logged.

SIDE EFFECTS
  None.

===========================================================================*/

extern err_logged_type err_logged (void);


/*===========================================================================

FUNCTION ERR_IGNORED

DESCRIPTION
  Return to the caller the total number of errors ignored.

DEPENDENCIES
  err_init must be called before this call otherwise the value
  returned is zero.

RETURN VALUE
  The total number of errors ignored because NV error log was full.
  Note that this number is reset on every powerup and the value
  returned is errors ignored since last powerup.

SIDE EFFECTS
  None.

===========================================================================*/

extern err_ignored_type err_ignored (void);


/*===========================================================================

FUNCTION ERR_GET_LOG

DESCRIPTION
  Copy to the caller's pointer a specific error record in the error
  log cache.  The error record is identified by its number, from 0 to
  NV_MAX_ERR_LOG.

DEPENDENCIES
  err_init must be called before this call, otherwise no operation
  takes place and ERR_NONE_S completion status is returned.

RETURN VALUE
  ERR_DONE_S    - Requested error log existed
  ERR_NONE_S    - No such log number (out of range).

SIDE EFFECTS
  None.

===========================================================================*/

extern err_stat_type err_get_log (
  byte            log_num,     /* Number of log to get */
  nv_err_log_type *log_ptr     /* Pointer to log buffer */
);


/*===========================================================================

FUNCTION ERR_CLR_LOG

DESCRIPTION
  This function clears a specific error record both in cache and in NV.
  An error record is cleared simply by clearing the error count field
  in it.

DEPENDENCIES
  err_init must be called before this call, otherwise no operation
  takes place and ERR_NONE_S completion status is returned.

RETURN VALUE
  ERR_DONE_S    - Requested error record existed and cleared.
  ERR_NONE_S    - Error record out of range or clear failed.

SIDE EFFECTS
  The specified error record is cleared in NV and in cache and is now
  available to accept a new unique error.

===========================================================================*/

extern err_stat_type err_clr_log (
   byte log_num    /* Number of log to clear */
);


/*===========================================================================

FUNCTION ERR_PUT_LOG

DESCRIPTION
  This function is invoked from the ERR(...) macro.  It then calls
  other local functions to store the error.

DEPENDENCIES
  err_init must be called before this call, otherwise no operation
  takes place.  This function must not be called directly; it may
  be called only from the ERR macro.

RETURN VALUE
  None.

SIDE EFFECTS
  Attempt will be made to write an error record to NV and cache.

===========================================================================*/

extern void err_put_log (
  word           line,            /* Line number */
  const char    *file_ptr         /* File name pointer */
);


/*===========================================================================

FUNCTION ERROR_FATAL_PUT_LOG

DESCRIPTION
  This function is invoked from the ERR_FATAL(...) macro.  First and
  immediately this function kills the PA.  Next an attempt is made to
  log the error to NV.  If there was room in the error log for a new
  error record, or if incrementing an existing error record count was
  below threshold then we force reset of the DMSS.  If the error
  could not be added to the log, or if it had exceeded count threshold
  then we force a powerdown.  The purpose of this is to let the DMSS
  reset upon fatal error but only up to a certain threshold.  After
  this threshold the unit will powerdown, to avoid continual reset
  cycle and possible thrashing of NV write cycle limit.

DEPENDENCIES
  err_init must have been called first, otherwise we reset but we
  do not log anything to NV.  This function must not be called
  directly; it may only be called from the ERR_FATAL macro.

RETURN VALUE
  None.

SIDE EFFECTS
  ***********************************************************
  ********** THERE IS NO RETURN FROM THIS FUNCTION **********
  ***********************************************************

===========================================================================*/

/* lint -function(abort, err_fatal_put_log) */
extern void err_fatal_put_log (
  word   line,                  /* From __LINE__ */
  const char   *file_ptr,       /* From __FILE__ */
  dword         code1,
  dword         code2,
  dword         code3
);


/*===========================================================================

MACRO ERR

DESCRIPTION
  Log an error to error log and update cache and return to caller.
  Error is sent to the MSG utility to be routed to DM or PC screen,
  depending on conditional compile switches defined in MSG.  If host
  is a PC then NV logging does not take place and only MSG output
  is used.

DEPENDENCIES
  If MSG_FILE is defined, ERR uses it as the filename.

RETURN VALUE
  None.

SIDE EFFECTS
  When the host is target hardware a call is generated to err_put_log
  to queue log request to NV task.  As a result unexpected execution
  times are likely to take place.  ERR must only be called because of
  an error condition (never as an information announcement -- use MSG
  for that) therefore these timing side effect are OK.

===========================================================================*/

#ifdef MSG_FILE // jwpark 00.08.29
//#if defined(MSG_FILE) && (MSG_LEVEL!=MSG_LVL_NONE)
#define ERR_FILE_NAME   msg_file
#else	// #ifdef MSG_FILE
#define ERR_FILE_NAME   __FILE__
#endif	// #ifdef MSG_FILE

#ifdef ERR_DEBUG

#define  ERR(format, code1, code2, code3)          \
  do {                                             \
    MSG_ERROR (format, (code1), (code2), (code3)); \
    err_put_log ( __LINE__, ERR_FILE_NAME );       \
/*lint -e717 */                                    \
  } while (0)                                      \
/*lint +e717 */

#elif ((TG==T_PC) || (TG==T_REX))
#error code not present
#else

#define  ERR(format, code1, code2, code3)          \
  do {                                             \
    MSG_ERROR (format, (code1), (code2), (code3)); \
    err_put_log ( __LINE__, ERR_FILE_NAME );       \
/*lint -e717 */                                    \
  } while(0)                                       \
/*lint +e717 */

#endif


/*===========================================================================

MACRO ERR_FATAL

DESCRIPTION
  Log an error to error log, update the cache (for debug purpose only)
  and RESET THE SYSTEM.  Control DOES NOT RETURN to caller.  Error is
  sent to the MSG utility to be routed to DM or PC screen, depending
  on conditional compile switches defined in MSG.  If host is a PC
  then NV logging does not take place.  Instead a MSG output is made
  and the program exits (simulating hardware reset of sorts).
	
DEPENDENCIES
  If MSG_FILE is defined, ERR_FATAL uses it as the filename.

RETURN VALUE
  None.  Control does NOT return to caller.

SIDE EFFECTS
  When the host is target hardware a call is generated to err_put_log
  to queue log request to NV task.  As a result unexpected execution
  times are likely to take place.  ERR_FATAL must only be called because
  of an error condition (never as an information announcement -- use MSG
  for that) therefore these timing side effects are OK.

===========================================================================*/

extern void  boot_clear_sl(void);

#ifdef PC_EMULATOR_H

   #define PC_EMULATOR_ERR
   #include PC_EMULATOR_H
   #undef PC_EMULATOR_ERR

#elif defined ERR_DEBUG

#define  ERR_FATAL(format, code1, code2, code3)  \
do {                                             \
  boot_clear_sl();                               \
  MSG_FATAL(format, (code1), (code2), (code3));  \
  err_fatal_put_log ( __LINE__, ERR_FILE_NAME, (code1), (code2), (code3)); \
/*lint -e717 */                                  \
} while (0)                                      \
/*lint +e717 */

#elif ((TG==T_PC) || (TG==T_REX))
#error code not present
#else

#define  ERR_FATAL(format, code1, code2, code3)  \
do {                                             \
  boot_clear_sl();                               \
  err_fatal_put_log ( __LINE__, ERR_FILE_NAME, (code1), (code2), (code3)); \
/*lint -e717 */                                  \
} while (0)                                      \
/*lint +e717 */

#endif

#endif /* ERR_H */

