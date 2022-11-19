/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_type.h
**
** Description:
**     Type definitions
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-04-03 redstar    add event category  type
** 2000-03-21 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_TYPE_H_
#define _EQS_TYPE_H_

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#ifndef NULL
#define NULL    0
#endif

/* not to generate macro redefintion warning */
#undef CONST
#undef ROM
//#undef LOCAL
#undef STATIC

#ifdef TRUE
#undef TRUE
#undef FALSE
#endif

#ifdef ON
#undef ON
#undef OFF
#endif

#define CONST                       const
#define ROM                         const       /* going to the rom */
#ifndef LOCAL
#define LOCAL                       static      /* scope */
#endif
#define STATIC                      static      /* memory category */

#define EQS_API                      /* __cdecl, __stdcall, __fastcall */
#define EQS_DCL                      extern

#define IS_RESULT_OK(r)             ((r) > 0)
#define IS_RESULT_ERROR(r)          ((r) <= 0)

#define TRUE                        (1)
#define FALSE                       (!TRUE)

#define ON                          (1)
#define OFF                         (!ON)

#define EQS_NA                       0 /* Not Applicable */
#define UNUSED(x)                   ((void)(x))

#define MOD_INCREMENT(indx, base)   ((indx + 1) % base)
#define MOD_DECREMENT(indx, base)   (((indx) - 1 + (base)) % (base))

#define MAX_DWORD_NUMBER	        0xffffffff
#define MAKE_WORD(HBYTE, LBYTE) ((WORD)((HBYTE << 8) + LBYTE))
#define MAKE_DWORD(HWORD, LWORD) ((DWORD)((HWORD << 16) + LWORD))
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

//typedef unsigned char   BOOL;
typedef unsigned char   ONOFF;

//typedef signed char     CHAR;
typedef char            CHAR;
#ifndef BYTE_TYPE_DEFINED
typedef unsigned char   BYTE;
#define BYTE_TYPE_DEFINED
#endif
typedef short           SHORT;
typedef int             INT;
typedef unsigned int    UINT;
typedef long            LONG;
typedef float           FLOAT;
typedef double          DOUBLE;
typedef void            VOID;
typedef unsigned short  SIZE_T; /* used in strxxx, malloc, free */
typedef unsigned char   HCONTROL; /* handle to I/O Control */
typedef unsigned char   HTIMER  ; /* handle to Timer       */
typedef unsigned char   HWINDOW; // slide window handle.
typedef unsigned short  WCHAR;
typedef unsigned char   UCHAR;
/*  INT8    :   8 bit signed integer    */
typedef signed char     INT8;
/*  UINT8   :   8 bit unsigned integer  */
typedef unsigned char   UINT8;
/*  INT16   :   16 bit signed integer   */
typedef signed short    INT16;
/*  UINT16  :   16 bit unsigned integer */
typedef unsigned short  UINT16;
/*  INT32   :   32 bit signed integer   */
typedef signed long     INT32;
/*  UINT32  :   32 bit unsigned integer */
typedef unsigned long   UINT32;
#ifndef WORD_TYPE_DEFINED
typedef unsigned short  WORD;
#define WORD_TYPE_DEFINED
#endif
/* rosa 01-03-07 --> @why, what */
#ifndef DWORD_TYPE_DEFINED
typedef unsigned long   DWORD;
#define DWORD_TYPE_DEFINED
#endif
/* <-- */
typedef unsigned long   QWORD[2];   // redstar   2001.1.2

typedef char            BOOL;
//typedef char            BYTE; //WAP CORE Type

/* ytchoi 01.04.26
   WAP CORE에서 float연산을 위해 change */
typedef float           FLOAT32; 
//	typedef INT32           FLOAT32;

#ifndef VOID
	#define VOID            void
#endif

#ifndef NULL
	#define NULL    0
#endif
/*  TRUE    :   Integer value 1 */
#ifndef TRUE
	#define TRUE    1
#endif
/*  FALSE   :   Integer value 0 */
#ifndef FALSE
	#define FALSE   0
#endif


/* result codes */
enum tagEQS_RESULT {
    RESULT_OK                    = 1,
    
    RESULT_ERROR                 = 0, 
    RESULT_UNEXPECTED            = -1,
    RESULT_NOT_IMPLEMENTED       = -2,
    RESULT_NOT_SUPPORTED         = -3,
    RESULT_INVALID_ARG           = -4,
    RESULT_OUT_OF_MEMORY         = -5,
    RESULT_DUPLICATE             = -6,
    RESULT_ABORTED               = -7,
    RESULT_IN_USE                = -8,
    RESULT_INSUFFICIENT_SPACE    = -9,
	RESULT_EVENT_QUEUE_FULL		 = -10,
	RESULT_EVENT_QUEUE_EMPTY	 = -11,
    RESULT_FILE_OPEN_ERROR       = -32,
    RESULT_FILE_IO_ERROR         = -33,
    RESULT_FILE_ERROR            = -34,
    RESULT_CORRUPT_DATA          = -37,
    RESULT_LIMIT_EXCEEDED        = -45,
    RESULT_DISALLOWED            = -46,
    RESULT_INVALID_HANDLE        = -47,
    RESULT_VERSION_MISMATCH      = -49
};
typedef enum tagEQS_RESULT        EQS_RESULT;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#endif  /* _EQS_TYPE_H_ */
