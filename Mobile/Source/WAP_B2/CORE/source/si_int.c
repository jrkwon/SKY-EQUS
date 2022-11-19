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
#include "si_int.h"

#include "si_misc.h"
#include "si_var.h"


typedef enum {
	ss_start							= 0x01,
	ss_digit_start				= 0x02,
	ss_leading_zeroes			= 0x04,
	ss_digit_loop					= 0x08,
	ss_finished						= 0x10,
	ss_failure						= 0x20
} enumScanState;

#define Fetch() (charUsed++,*p++)
#define IsDigit(c) ((c>=WC('0'))&&(c<=WC('9')))
#define IsFinalState(ss) (ss & (ss_finished | ss_failure))



BOOL String2Int(const WCHAR *aStr, UINT8 *charsUsed, INT32 *aInt, BOOL *isOverflow)
{
	enumScanState	scanState = ss_start;
	const WCHAR		*p = aStr;
	WCHAR					c;
	BOOL					sign = TRUE;
	INT32					val = 0, 
								newval = 0,
                tempval;
	UINT8					charUsed = 0;


	*isOverflow = FALSE;

	c = Fetch();
	while ( c != 0 && IsWhitespaceChar(c) )
	{
		c = Fetch();
	}

	while ( !IsFinalState(scanState) ) 
	{
		switch (scanState)
		{
			case ss_start:
				if (c == WC('+'))
				{
					c = Fetch();
				}
				else if (c == WC('-'))
				{
					c = Fetch();			
					sign = FALSE;
				}
				scanState = ss_digit_start;
				break;
			
			case ss_digit_start:
				if (!IsDigit(c))
				{
					scanState = ss_failure;
				}
				else if (c == WC('0'))
				{
					scanState = ss_leading_zeroes;
					c = Fetch();
				}
				else
				{
					scanState = ss_digit_loop;
				}
				break;
			
			case ss_leading_zeroes:
				if (c == WC('0'))
				{
					c = Fetch();
				}
				else
				{
					scanState = ss_digit_loop;
				}
				break;
			
			case ss_digit_loop:				
				if (!IsDigit(c))
				{
					scanState = ss_finished;
					break;
				}

        if ( ! IntMUL_no_overflow( val, 10, &newval ))
        {
					*isOverflow = TRUE;
					scanState = ss_failure;
					break;
        }
				tempval = newval + (c - WC('0'));
				if ( tempval < newval )
				{
					*isOverflow = TRUE;
					scanState = ss_failure;
					break;
				}
				val = tempval;
				c = Fetch();
				break;
		}
	}

	if ( scanState == ss_finished )
	{
		if ( sign == FALSE )
		{
			val = -val;
		}
		*aInt = val;
		*charsUsed = charUsed -1;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


VOID Int2String(const INT32 aInt, WCHAR *str)
{
	BOOL		sign = TRUE;
	INT32		val = aInt;
	WCHAR		res[12];
	WCHAR		*p,*p2;
	UINT16	numDigits;
	WCHAR		wc_0[] = { '0', 0 };

	/* Initial housekeeping */
	if ( val == 0 )
	{
		COPYSTRING(str, wc_0);
		return;
	}
	else if ( val < 0 )
	{
		sign = FALSE;
	}

	/* Extract the digits */
	numDigits = 0;
	p = res;
	while ( val )
	{
/*  Modified by GBU,000103  added cast */
/*
		*p++ = ABS(val % 10) + WC('0');
*/
        *p++ = (WCHAR)ABS(val % 10) + WC('0');
        val /= 10;
		numDigits++;
	}

	/* Add the minus sign if applicable */
	if ( sign == FALSE )
	{
		*p++ = WC('-');
		numDigits++;
	}

	/* Copy the digits to the resulting string */
	p--;
	p2 = str;
	while( numDigits )
	{
		*p2++ = *p--;
		numDigits--;
	}
	*p2 = 0;
}
