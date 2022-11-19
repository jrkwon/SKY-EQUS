/*************************************************************************
*   Copyright (c) 1998-1999 SKTeletech, Inc. All rights reserved.         *
*                                                                        *
*    THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Phone.com, Inc.      *
*    The copyright notice above does not evidence any                    *
*    actual or intended publication of such source code.                 *
**************************************************************************/

#ifndef __UPTYPES__
#define __UPTYPES__

#ifdef __cplusplus
extern "C"      {
#endif

typedef	unsigned char	U8;		/* unsigned 8 bit integer */
typedef	unsigned short	U16;	/* unsigned 16 bit integer */
typedef	unsigned long	U32;	/* unsigned 32 bit integer */
typedef	signed char		S8;		/* signed 8 bit integer */
typedef	signed short	S16;	/* signed 16 bit integer */
typedef	signed long		S32;	/* signed 32 bit integer */

#if defined(SIMULATE_16BIT_INT) && !defined(_WINDOWS)
typedef	signed short	SINT;	/* signed integer of at least 16 bits */
typedef	unsigned short	UINT;	/* unsigned integer of at least 16 bits */
#else
typedef	signed int		SINT;	/* signed integer of at least 16 bits */
typedef	unsigned int	UINT;	/* unsigned integer of at least 16 bits */
#endif

typedef	unsigned long	INTP;	/* can hold a pointer or a 32 bit integer */


/*
#ifndef TRUE
#define TRUE	kTrue
#define FALSE	kFalse
#endif
#ifndef true
#define true	kTrue
#define false	kFalse
#endif
*/

#ifdef __cplusplus
}
#endif

#endif


