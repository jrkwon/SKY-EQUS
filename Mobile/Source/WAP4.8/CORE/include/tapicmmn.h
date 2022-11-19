/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
#include "Bunny.h"

#if (MODEL_ID == MODEL_B0) ||(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)

#include "eqs_type.h"

#define MEM_ADDRESS_ALIGNMENT 4

#ifndef TAPICMMN_H
#define TAPICMMN_H

#include "wiptrgt.h"

#ifdef FEATURE_WAP
#if     defined TRGT_OSE
    typedef INT16   memSizeType;
#else
    typedef INT32   memSizeType;
#endif
#endif //FEATURE_WAP
#endif

#else //# if(MODEL_ID == MODEL_B0)

#error here comes a neo challenger
#ifndef TAPICMMN_H
#define TAPICMMN_H


#include "wiptrgt.h"


#if defined TRGT_VXWORKS			
	#include "types/vxTypesOld.h"
#endif


/*  Generic type definitions    */
#if !defined TRGT_VXWORKS && !defined TRGT_SEOS
	/*  INT8    :   8 bit signed integer    */
	typedef signed char     INT8;
	/*  UINT8   :   8 bit unsigned integer  */
	typedef unsigned char   UINT8;
	/*  INT16   :   16 bit signed integer   */
	typedef signed short    INT16;
	/*  UINT16  :   16 bit unsigned integer */
	typedef unsigned short  UINT16;
#ifndef TBNG
	/*  INT32   :   32 bit signed integer   */
	typedef signed long     INT32;
	/*  UINT32  :   32 bit unsigned integer */
	typedef unsigned long   UINT32;
#endif
#endif


#ifdef TRGT_SEOS
	/*  INT8    :   8 bit signed integer    */
	typedef signed char     INT8;
	/*  UINT8   :   8 bit unsigned integer  */
	typedef unsigned char   UINT8;
	/*  INT16   :   16 bit signed integer   */
	typedef signed int    INT16;
	/*  UINT16  :   16 bit unsigned integer */
	typedef unsigned int  UINT16;
	/*  INT32   :   32 bit signed integer   */
	typedef signed long     INT32;
	/*  UINT32  :   32 bit unsigned integer */
	typedef unsigned long   UINT32;
#endif


/*  FLOAT32 :   32 bit real */
/*  If no real type exist for the host device, map to any integer. See also the static configuration variable HAS_REAL. */
#ifdef HAS_FLOAT
	typedef float           FLOAT32;
#else
	typedef INT32           FLOAT32;
#endif

/*  BOOL    :   1 bit integer   */
#if !defined TRGT_VXWORKS && !defined _WINDOWS_
	#ifdef TRGT_REX
		typedef char            BOOL;
	#elif defined TRGT_SDLM
		typedef UINT8			BOOL;
	#else
		typedef int             BOOL;
	#endif
#endif

/*  BYTE    :   8 bit unsigned integer  */
#if !defined TRGT_SDLM
		typedef unsigned char BYTE;
#else
		typedef UINT8 BYTE;
#endif

/*  CHAR    :   8 bit character  */
#if !defined TRGT_SEOS
		typedef char     CHAR;
#elif defined TRGT_G23
		typedef INT8	 CHAR;		
#else
		typedef unsigned char     CHAR;
#endif

/*  UCHAR   :   8 bit unsigned character    */
#ifndef TRGT_VXWORKS
	#ifdef TRGT_SDLM
		typedef UINT8        UCHAR;
	#else
		typedef unsigned char   UCHAR;
	#endif
#endif

/*  WCHAR   :   16 bit unsigned character   */
#ifndef _WINDOWS_
	#ifdef TRGT_SDLM
		typedef UINT16        WCHAR;
	#elif defined TRGT_SEOS
		typedef unsigned int WCHAR;
   	#elif defined TRGT_G23
		typedef int WCHAR;
	#else
		typedef unsigned short  WCHAR;
	#endif
#endif

/*  VOID    :   A special type indicating the absence of any value. */
#ifndef VOID
	#define VOID            void
#endif


/*  Constants   */
/*  NULL    :   Integer value 0 */
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


/*
device dependent constant that describes
the minimum alignment for memory addresses that
malloc returns. Ex:
struct
{
    char    *t; <- address 0x00000000
    char    *p; <- address 0x00000004
}
As the example illustrates takes the two variables in
the struct normally 8 bytes on a device with 32 bit
addresses. Some devices needs, however, further padding.
On a sparc processor must memory addresses be aligned
with 8 bytes. The struct would in that case take 16 bytes.
*/
#define MEM_ADDRESS_ALIGNMENT 4


#endif
#endif //MODEL_ID
