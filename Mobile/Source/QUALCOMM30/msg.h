#ifndef MSG_H
#define MSG_H
/*===========================================================================

   M E S S A G E   L O G G I N G   S E R V I C E S   H E A D E R   F I L E

DESCRIPTION
  All the declarations and definitions necessary to support reporting messages
  for errors and debugging.

Copyright (c) 1992-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/msg.h_v   1.0.1.1   06 Mar 2001 15:58:52   jlindqui  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/05/01   jal     Extended msg_put_str() to take the string in any
                   position, not just as the first parameter.
03/01/00   jal     Added string argument hack version of msg_put()
05/15/00   lad     Externalized msg_nice[].
04/11/00   lad     Externalized msg_cnt_ignored.
02/28/00   lad     Added support for streaming DIAG.
02/02/00   lad     Changed macros to parse as a single expression.
06/30/99   jct     Added PC_EMULATOR_MSG definition for capturing inclusion
                   of the PC_EMULATOR feature outside of the makefile and
                   this file
06/02/99   lad     Fixed multi-line macros to evaluate as single expression.
04/19/99    sk     Removed warning for unsigned comparison (ARM 2.50)
03/31/99   lad     Changed dropped msg handling to be correctly noted.
03/25/99    ms     Incorporated optimized MSG routines.
11/15/98   jct     Added const keyword to char* msg_file for ARM targets
11/05/98   jct     Merged all ARM changes with main archive
09/15/98   udp     Replaced _ARM_ --> T_ARM
02/14/97   rdh     Used Pascal directive on msg_put to save ROM.
02/06/96   fkm     Put ()s around Macros to Make Lint Happy
07/24/95   jah     Made MSG_FILE option accessible to the Error Services to
                   save more ROM and RAM.
06/28/95   jah     Added MSG_FILE option to reduce ROM usage for filenames.
10/16/92   jah     Allow MSG_THRESHOLD to be specified before the include of
                   this header file by adding an #ifdef.
07/22/92   jah     Changed T_PC MSG_BASE to use only printf(), helps ACP UT.
07/08/92   jah     Clear lint error 571 out of MSG_BASE for the phone targets
05/19/92   jah     Changed to allow dynamic message filtering
03/03/92   jah     created file

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.            */

#ifdef TG
   #include "target.h"  /* Definition for TG, and target specific defs */
#endif
#include "qw.h"         /* definition of qword                         */


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Message Priorities
*/
#define MSG_LVL_LOW     0       /* Lowest priority messages                */
#define MSG_LVL_MED     1       /* Medium priority messages                */
#define MSG_LVL_HIGH    2       /* High priority messages                  */
#define MSG_LVL_ERROR   3       /* Error messages                          */
#define MSG_LVL_FATAL   4       /* Fatal error messages                    */
#define MSG_LVL_NONE    255     /* No messages          (highest priority) */

/* MSG_LEVEL is the minimum message priority which can be logged.  If MSG_LEVEL
** is not defined, default is none.  If MSG_LEVEL is set to MSG_LVL_NONE, then
** there will be no references in the object (no calls to msg_put()).
*/
#ifndef MSG_LEVEL               /* If level is not defined, default is none */
#define MSG_LEVEL       MSG_LVL_NONE
#endif

#if (TG!=T_PC)

/* MSG_THRESHOLD is a "local" message threshold for the module including
** this header file.  This variable can be set programmatically, or by
** being poked via the Diagnostic Task.  Messages below the specified level
** will not be logged to the DM for display.  A default definition is defined
** below, in the event that a "local" message threshold is not needed.  To
** redefine this symbol, first #undef it and the #define it to a variable
** which contains the minimum messages logging level.
**
** The variable name must have a prefix matching the SDM requirements for
** a global variable, and this service mandates that the suffix be _msg_level.
**
** For a module with a prefix of 'pen' with the local message threshold
** initialized to 'no messages':
**
**      word pen_msg_level = MSG_LVL_NONE;
**
**      #undef MSG_THRESHOLD
**      #define MSG_THRESHOLD   pen_msg_level
**
** Note that MSG_THRESHOLD is not used in the PC target environment.
*/
#ifndef MSG_THRESHOLD
#define MSG_THRESHOLD   msg_limit
#endif

/* This is the global message level limiter.  It is compared to the
** message level at the line the macro is referenced, at run-time.
**
** The content of this variable is set by msg_set_limit(), and is not
** to be set or read by any external module.  It's sole purpose is to
** be referenced by the MSG_* macros defined below.
*/
extern word msg_limit;

/* This is the array of "nice" values for message levels.  These values are 
** used in a similar fashion as the UNIX "nice" command.  The higher the 
** number, the less priority the item has.
*/
extern int16 msg_nice[];

#endif /* TG != T_PC */

/*===========================================================================

                      MACRO FUNCTION DECLARATIONS

===========================================================================*/

/* If MSG_FILE is defined, use that as the filename, and allocate a single
** character string to contain it.  Note that this string is shared with
** the Error Services, to conserve ROM and RAM.
*/
//kjh '00.7.25 #if defined(MSG_FILE) && (MSG_LEVEL!=MSG_LVL_NONE)

#ifdef	_FOR_BROWSER
static const char *msg_file;
#else	//#ifdef	_FOR_BROWSER
//	#if defined(MSG_FILE) && (MSG_LEVEL!=MSG_LVL_NONE)
	#ifdef MSG_FILE
	static const char msg_file[] = MSG_FILE;
	#else	// #ifdef MSG_FILE
	static const char msg_file[] = "";
	#endif	// #ifdef MSG_FILE
#endif	//#ifdef	_FOR_BROWSER


/*===========================================================================
    Message Store Type.
===========================================================================*/
typedef struct {
         byte   level;      /* Severity level / Priority of this message. */
         word   line;       /* Line number into source file               */
   const char * file_ptr;   /* Pointer to source file name                */
   const char * xx_fmt;     /* Printf style format string                 */

} msg_store_type;


/*===========================================================================

MACRO MSG_FATAL, MSG_ERROR, MSG_HIGH, MSG_MED, MSG_LOW

DESCRIPTION
  Output a message to be sent to be picked up by the Diag Task.  The
  message levels are controlled by selecting the proper macro function.

    MSG_FATAL   fatal
    MSG_ERROR   error
    MSG_HIGH    high
    MSG_MED     medium
    MSG_LOW     low

  Generation of message generating code is controlled by the setting
  of the MSG_LEVEL #define (defined above and on the 'cl' command line).

PARAMETERS
  x_fmt Format string for message (printf style).  Note, this parameter
        must be a string literal (e.g. "Tuned to %lx"), not a variable.
        This is because the value is compiled into ROM, and can clearly
        not be a dynamic data type.
  a     1st parameter for format string
  b     2nd parameter for format string
  c     3rd parameter for format string

DEPENDENCIES
  msg_init() must be called prior to this macro referencing msg_put().
  This macro, is multiple C expressions, and cannot be used as if it
  is a single expression.

RETURN VALUE
  None

SIDE EFFECTS
  On the target hardware, the file and format strings are placed in ROM.

===========================================================================*/

/* Fatal priority messages (highest)
*/
#if MSG_LEVEL < MSG_LVL_NONE

#ifdef PC_EMULATOR_H
#error code not present
#elif (TG==T_PC)
#error code not present
#else

/* Baseline message macro for the non-PC (DMSS hardware) environment
**
** The ROM directive places the block-local variables into the ROM, and
** it is initialized to have the contents of the string literals x_fmt
** and __FILE__.  If MSG_FILE is defined, use that as the filename, and
** allocate a single character string to contain it.
**
** The xx_msg_limit is to avoid having the compiler use stack space to
** make a local copy of msg_limit.  Since using the stack causes C language
** REX ISR's to jump into the weeds, this is highly desirable.
**
** lint error 571 has to do with lint analyzing the dword casts on the
** parameters to msg_put() and noticing that signed types are being
** cast to unsigned (not a problem here).
*/
#if defined MSG_FILE || defined T_ARM

#ifdef _FOR_BROWSER // youandi 01.03.23

#define MSG_BASE(level,x_format,a,b,c)                        \
do {                                                          \
  static const msg_store_type xx_store;						  \
  if ((!(level < msg_limit)) && (!(level < MSG_THRESHOLD))) { \
    msg_put(&xx_store, (dword)(a), (dword)(b), (dword)(c));   \
  }                                                           \
} while(0)

#else

#define MSG_BASE(level,x_format,a,b,c)                        \
do {                                                          \
  static const msg_store_type xx_store =                      \
    {level, __LINE__, msg_file, x_format };                   \
  if ((!(level < msg_limit)) && (!(level < MSG_THRESHOLD))) { \
    /*lint -e571 */                                           \
    msg_put(&xx_store, (dword)(a), (dword)(b), (dword)(c));   \
    /*lint +e571 */                                           \
  }                                                           \
/*lint -e717 */                                               \
} while(0)                                                    \
/*lint +e717 */

#endif

#if defined( FEATURE_DIAG_MSG_STRING_ARG )
#error code not present
#endif /* FEATURE_DIAG_MSG_STRING_ARG */

#endif  /* MSG_FILE || T_ARM */
#endif  /* TG==T_PC */

/* Fatal error priority messages
*/
#define MSG_FATAL(x_fmt,a,b,c)  MSG_BASE(MSG_LVL_FATAL, x_fmt, (a), (b), (c))
#else
#define MSG_FATAL(x_fmt,a,b,c)  (void) msg_file /* Suppress compiler warning */
#endif

/* Error priority messages
*/
#if MSG_LEVEL < MSG_LVL_FATAL
#define MSG_ERROR(x_fmt,a,b,c)  MSG_BASE(MSG_LVL_ERROR, x_fmt, (a), (b), (c))
#else
#define MSG_ERROR(x_fmt,a,b,c)  (void) msg_file /* Suppress compiler warning */
#endif

/* High priority messages
*/
#if MSG_LEVEL < MSG_LVL_ERROR
#define MSG_HIGH(x_fmt,a,b,c)   MSG_BASE(MSG_LVL_HIGH, x_fmt, (a), (b), (c))
#else
#define MSG_HIGH(x_fmt,a,b,c)   (void) msg_file /* Suppress compiler warning */
#endif

/* Medium priority messages
*/
#if MSG_LEVEL < MSG_LVL_HIGH
#define MSG_MED(x_fmt,a,b,c)    MSG_BASE(MSG_LVL_MED, x_fmt, (a), (b), (c))
#else
#define MSG_MED(x_fmt,a,b,c)    (void) msg_file /* Suppress compiler warning */
#endif

/* Low priority messages
*/
#if MSG_LEVEL < MSG_LVL_MED
#define MSG_LOW(x_fmt,a,b,c)    MSG_BASE(MSG_LVL_LOW, x_fmt, (a), (b), (c))
#else
#define MSG_LOW(x_fmt,a,b,c)    (void) msg_file /* Suppress compiler warning */
#endif

#if defined( FEATURE_DIAG_MSG_STRING_ARG )
#error code not present
#else
#define MSG_STR_ERROR(x_fmt,s,a,b,c)    (void) msg_file /* Suppress compiler warning */
#define MSG_STR_HIGH(x_fmt,s,a,b,c)    (void) msg_file /* Suppress compiler warning */
#define MSG_STR_MED(x_fmt,s,a,b,c)    (void) msg_file /* Suppress compiler warning */
#define MSG_STR_LOW(x_fmt,s,a,b,c)    (void) msg_file /* Suppress compiler warning */
#endif
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define      MSG_FILE_LEN (12+1) /* Allows full DOS file name + NULL   */
#define      MSG_FMT_LEN  40     /* Max len of format string + NULL    */

#if defined(FEATURE_DIAG_NON_STREAMING)
typedef PACKED struct {
  byte  level;                    /* Severity level of this message     */
  char  file[ MSG_FILE_LEN ];     /* Holds source file name             */
  word  line;                     /* Line number in source file         */
  char  fmt[ MSG_FMT_LEN ];       /* printf style format string         */
  dword code1;                    /* parameters for the format string   */
  dword code2;
  dword code3;
  qword time;                     /* Time at which error occurred       */
} msg_buf_type;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Return status for msg_get() to let the Diag Task know the status
** of the Queue of messages (not-empty, empty)
*/
typedef enum {
  MSG_DONE_S,                     /* A message was available (returned) */
  MSG_EMPTY_S                     /* No messages were available         */
} msg_status_type;
#endif /* defined(FEATURE_DIAG_NON_STREAMING) */

extern dword msg_cnt_ignored;     /* Number of dropped messages         */


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION MSG_INIT

DESCRIPTION
  Initialize the message reporting services

DEPENDENCIES
  Can only be called by one task.

RETURN VALUE
  None

SIDE EFFECTS
  Sets the counts to 0, and throws away any buffered messages.

===========================================================================*/
extern void msg_init( void );


/*===========================================================================

FUNCTION MSG_TOTAL

DESCRIPTION
  Call this to get a count of the total number of messages logged to
  this service.

DEPENDENCIES
  None

RETURN VALUE
  Number of messages reported so far, this will wrap to 0 at 2^32 - 1.

SIDE EFFECTS
  None

===========================================================================*/
extern dword msg_total( void );


/*===========================================================================

FUNCTION MSG_PUT

DESCRIPTION
  *** Do not call directly; use macro MSG_* defined in msg.h ***

  Put a message in the system message pool.

DEPENDENCIES
  msg_init() must have been called previously.  A free buffer must
  be available or the message will be ignored (never buffered).

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void msg_put
(
 const msg_store_type * fmt_ptr, /* Message store pointer                  */
       dword            code1,   /* 1st parameter to go with format string */
       dword            code2,   /* 2nd parameter to go with format string */
       dword            code3    /* 3rd parameter to go with format string */
);

#if defined( FEATURE_DIAG_MSG_STRING_ARG )
#error code not present
#endif


#if defined(FEATURE_DIAG_NON_STREAMING)
/*===========================================================================

FUNCTION MSG_QTY

DESCRIPTION
  Return the number of message reports waiting to be serviced

DEPENDENCIES
  None

RETURN VALUE
  Number of messages waiting in the message pool

SIDE EFFECTS
  None

===========================================================================*/
extern word msg_qty( void );


/*===========================================================================

FUNCTION MSG_GET

DESCRIPTION
  Copy a message buffer into the location specified.

DEPENDENCIES
  None

RETURN VALUE
  MSG_DONE_S    Messages were available
  MSG_EMPTY_S   No messages were available

SIDE EFFECTS
  Frees up a message buffer for more messages

===========================================================================*/
extern dword msg_get
(
  msg_buf_type *msg_buf_ptr,     /* where to place the message data, if any */
  PACKED dword *msg_drop_cnt     /* Number of dropped messages. */
);
#endif /* defined(FEATURE_DIAG_NON_STREAMING) */


/*===========================================================================

FUNCTION MSG_SET_LIMIT

DESCRIPTION
  Set the minimum message level to log.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void msg_set_limit
(
  word msg_level_limit          /* Minimum message level to log */
);

#endif /* MSG_H */

