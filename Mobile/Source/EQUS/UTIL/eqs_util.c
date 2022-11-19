/*
** ===========================================================================
**
** Project: 
**     Graphics
**
** File:
**     eqs_util.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-07-07 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <string.h>
#include <ctype.h>
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
//redstar  2000.11.15
LOCAL book_list_type ListTable[MAX_LIST_NUMBER];
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* string ---------------------------------------------------------------- */

SIZE_T EQS_API STRLEN(CONST BYTE* pszString)
{
    return (SIZE_T)strlen((CHAR*)pszString);
}


BYTE* EQS_API STRCAT(BYTE* pszHead, CONST BYTE* pszTail)
{
    return (BYTE*)strcat((CHAR*)pszHead, (CHAR*)pszTail);
}

BYTE* EQS_API STRNCAT(BYTE *pszHead, CONST BYTE* pszTail, SIZE_T Count)
{
    return (BYTE *)strncat((CHAR*) pszHead,(CHAR*) pszTail, Count );
}

BYTE* EQS_API STRCPY(BYTE* pszDest, CONST BYTE* pszSrc)
{
    return (BYTE*)strcpy((CHAR*)pszDest, (CHAR*)pszSrc);
}

BYTE* EQS_API STRNCPY(BYTE* pszDest, CONST BYTE* pszSrc, SIZE_T Count)
{
    return (BYTE*)strncpy((CHAR*)pszDest, (CHAR*)pszSrc, Count);
}

BYTE* EQS_API STRCHR(CONST BYTE* pszSrc, UINT8 c)
{
    return (BYTE*)strchr((CHAR*)pszSrc, c);
}

INT EQS_API STRCMP(CONST BYTE* pszSrc, CONST BYTE* pszDest)
{
		
	return (INT)strcmp((CHAR*)pszSrc, (CHAR*)pszDest);
}

INT EQS_API STRNCMP(CONST BYTE* pszSrc, CONST BYTE* pszDest, UINT16 nSize)
{
    return (INT)strncmp((CHAR*)pszSrc, (CHAR*)pszDest, nSize);
}

BYTE* EQS_API STRSTR(BYTE* pszSrc, BYTE* pszDest)
{
    return (BYTE*) strstr((CHAR*)pszSrc, (CHAR*) pszDest);
}

BYTE* EQS_API STRTOK(BYTE* pszSrc, CONST BYTE* pszDelimit)
{
    return (BYTE*) strtok((CHAR*)pszSrc, (CONST CHAR*) pszDelimit);
}

BYTE* EQS_API STRUPR(BYTE* pszSrc)
{
    UINT16 i;

    for(i = 0; i < STRLEN(pszSrc); i++)
        pszSrc[i] = toupper(pszSrc[i]);

	return pszSrc;
}

BYTE* EQS_API STRLWR(BYTE* pszSrc)
{
    UINT16 i;

    for(i = 0; i < STRLEN(pszSrc); i++)
        pszSrc[i] = tolower(pszSrc[i]);

	return pszSrc;
}

/* miscellaneous ----------------------------------------------------------- */
BOOL EQS_API IsMiddleOfMultiByte(BYTE *pszSrc, UINT16 nPosition)
{
    UINT16 i;
	UINT16 nMSBCnt;

    /* 
    ** jrkwon 2001.07.09 
    ** 마지막 멀티바이트 문자가 잘린 경우를 고려해서 '=' 뺐다.
    */
    //if( STRLEN(pszSrc) <= nPosition ) 
    if( STRLEN(pszSrc) < nPosition ) 
        return FALSE;
	for (nMSBCnt = i = 0; i < nPosition; i++) 
    {
		if( *pszSrc++ & (BYTE)0x80 )
			nMSBCnt++;
	}
	
    return (nMSBCnt % 2) ? TRUE : FALSE;
}

void EQS_API MakeShortString( BYTE *pszSrc, BYTE nPosition)
{
    if(nPosition > STRLEN(pszSrc)-1)
        return;

    if( IsMiddleOfMultiByte(pszSrc, (BYTE)(nPosition-2)) ) 
    {
    	memcpy(pszSrc+(nPosition-3), SF_DOTDOTDOT, 2); /* 글자 수가 많은 경우 "..." 추가 */
        *(pszSrc+nPosition-1) = '\0';
    }
    else
    {
    	memcpy(pszSrc+(nPosition-2), SF_DOTDOTDOT, 2); /* 글자 수가 많은 경우 "..." 추가 */
        *(pszSrc+nPosition) = '\0';
    }
}

void EQS_API SetRect(RECT* pRect, COORD xLeft, COORD yTop, COORD xRight, COORD yBottom)
{
    pRect->xLeft = xLeft;
    pRect->yTop = yTop;
    pRect->xRight = xRight;
    pRect->yBottom = yBottom;
}

double EQS_API EQS_atof( char *num_string )
{
	double   value = 0, float_value = 0.1;
	UINT8       leng, i, j;
	int  minus_flag = 0, dot_flag = 0;
	
	leng = strlen(num_string);

	for( i=0; i< leng; i++) {
		if (num_string[i] == '-')  minus_flag = 1;
		if (num_string[i] == '.') {
			dot_flag = 1;
			i++;
			break;
		} else if ((num_string[i] >= '0') && (num_string[i] <= '9'))
  	              value = value * 10.0 + (double)(num_string[i] - '0');
		       else
				   ; // error
	}

	if (dot_flag) {
        for( j = i; j < leng ; j++) {
			value = value + ((double)(num_string[j] - '0') * float_value);
			float_value = float_value * 0.1;
		}
	}
	
	if (minus_flag)
		value = value * (-1);

	return value;

} 

UINT8 EQS_API FindCodeSet(BYTE *pszNumber)
{
	UINT8 i;
    BYTE **ppCodeSet;

    ppCodeSet = (BYTE **)aLocaleTable[EQS_GetCountry()].ppNeedHypenCode;

    for(i = 0; ppCodeSet[i] != NULL; i++ )
    {
        if(!STRNCMP(pszNumber, ppCodeSet[i], STRLEN(ppCodeSet[i])))
            return (UINT8)STRLEN(ppCodeSet[i]);
    }

    return 0;
}

#define NEED_HYPEN_POSITION        4

UINT8 EQS_API FindHypen(BYTE *pszNumber, UINT8 nCodePos, UINT8 nMaxLen)
{
	UINT8 nPos;

    if((STRLEN(pszNumber) - nCodePos) 
        <= NEED_HYPEN_POSITION)
        return 0;

    if((STRLEN(pszNumber) - nCodePos) 
        > nMaxLen)
        return 0;

    if(STRCHR(pszNumber, 'T') != NULL)
        return 0;

    if(STRCHR(pszNumber, 'P') != NULL)
        return 0;

    nPos = STRLEN(pszNumber) - NEED_HYPEN_POSITION;

    return nPos;
}

UINT8 EQS_API MakeHypenedNumber(BYTE *pszSrcBuffer, BYTE *pszDestBuffer,
                               UINT8 nLen, UINT8 nCodePos, UINT8 nHypenPos)
{
    UINT8 nCopyLen, nIndex;

    nCopyLen = nLen;

    if(nCodePos > nLen) nCodePos = 0;
    if(nHypenPos > nLen) nHypenPos = 0;

    nIndex = 0;

    // Start->CodePos
    if(nCodePos > 0)
    {
        STRNCPY(pszDestBuffer, pszSrcBuffer, nCodePos);
        nIndex += nCodePos;
        memcpy(&pszDestBuffer[nIndex], (BYTE*)")", 1);
        nIndex++;

        nCopyLen++;
    }

    // CodePos->HypenPos
    if(nHypenPos > 0) 
    {
        STRNCPY(&pszDestBuffer[nIndex], 
        &pszSrcBuffer[nCodePos], (UINT16)(nHypenPos-nCodePos));
        nIndex += nHypenPos-nCodePos;
        memcpy(&pszDestBuffer[nIndex], (BYTE*)"-", 1);
        nIndex++;

        nCopyLen++;

        // HypenPos->End
        STRNCPY(&pszDestBuffer[nIndex], 
        &pszSrcBuffer[nHypenPos], (UINT16)(nLen-nHypenPos));
    }
    else
    {
        // CodePos->End
        STRNCPY(&pszDestBuffer[nIndex], 
        &pszSrcBuffer[nCodePos], (UINT16)(nLen-nCodePos));
    }

    return nCopyLen;
}

/*===========================================================================

FUNCTION QW_SET

DESCRIPTION
  This macro sets the high order 32 bits of a specified qword to a
  specified 32 bit value, and the low order 32 bits to another specified
  value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void QW_SET
(
  QWORD qw,
    /* the qword to set               */
  DWORD hi,
    /* the value for the high 32 bits */
  DWORD lo
    /* the value for the low 32 bits  */
)
{
  *(DWORD *)qw++  = lo;

  *(DWORD *)qw    = hi;
 
} /* qw_set */

/*==========================================================================

FUNCTION QW_HI

DESCRIPTION
  This macro returns the most significant 32 bits of a specified qword.

DEPENDENCIES
  None.

RETURN VALUE
  A DWORD equal to the high order 32 bit bits of the qword.

SIDE EFFECTS
  None.

===========================================================================*/
DWORD QW_HI
(
  QWORD qw
    /* qword to return the high 32 bits from */
)
{
 return *((DWORD *)qw+1);
} /* qw_hi */


/*===========================================================================

FUNCTION QW_LO

DESCRIPTION
  This macro returns the least significant 32 bits of a specified qword.

DEPENDENCIES
  None.

RETURN VALUE
  A DWORD equal to the low order 32 bits of the qword.

SIDE EFFECTS
  None.

===========================================================================*/
DWORD QW_LO
(
  QWORD qw
    /* qword to return the low 32 bits from */
)
{
 return *(DWORD *)qw;
} /* qw_lo */


/*===========================================================================

FUNCTION QW_EQU

DESCRIPTION
  This macro sets the value of one specified qword equal to another
  specified qword.

  This macro should be used if either or both of the quaDWORDs is not aligned
  to a 32-bit word boundary in memory.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void QW_EQU
(
  QWORD qw1,
    /* qw1 will be set equal to qw2       */
  QWORD qw2
    /* the qword qw1 will be set equal to */
)
{
  INT x;
  BYTE *p1 = (BYTE *) qw1, *p2 = (BYTE *) qw2;

  for( x = 0; x < sizeof(QWORD); x++ )
  {
    p1[x] = p2[x];
  }
} /* qw_equ_misaligned */


/*===========================================================================

FUNCTION QW_INC

DESCRIPTION
  This macro increments a specified qword by a specified DWORD value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void QW_INC
(
  QWORD  qw1,
    /* qword to increment  */
  DWORD  val
    /* value to add to qw1 */
)
{

  if ((0xFFFFFFFF - (*(DWORD *)qw1)) < val)
     {
      (*((DWORD *)qw1+1))++;
     }
  *(DWORD *)qw1 += val;

} /* qw_inc */


/*===========================================================================

FUNCTION QW_DEC

DESCRIPTION
  This macro decrements a specified qword by a specified value.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void QW_DEC
(
  QWORD qw1,
    /* qword to decrement        */
  DWORD val
    /* value to decrement qw1 by */
)
{
    if (*(DWORD *)qw1 < val)
       {
        (*((DWORD *)qw1+1))--;
       }
    *(DWORD *)qw1 -= val;

} /* qw_dec */

void QW_SUB
(
  QWORD  difference,
    /* the result of the subtraction       */
  QWORD  subtrahend,
    /* the first argument for subtraction  */
  QWORD  subtractor
    /* the second argument for subtraction */
)
{
    if( *(DWORD *)subtrahend < *(DWORD *)subtractor )
    {
        *((DWORD *)difference+1) =  *((DWORD *)subtrahend+1) - 1;
        *((DWORD *)difference)   =  0xffffffff - *(DWORD *)subtractor + *(DWORD *)subtrahend +1;
    }
    else
    {
        *((DWORD *)difference+1) =  *((DWORD *)subtrahend+1);
        *((DWORD *)difference)   =  *(DWORD *)subtrahend - *(DWORD *)subtractor ;
    }
    *((DWORD *)difference+1)  -= *((DWORD *)subtractor+1);

}

/*===========================================================================

FUNCTION QW_CMP

DESCRIPTION
  This macro compares 2 specified qwords, and returns a value based on
  their relation to each other.

DEPENDENCIES
  None.

RETURN VALUE
  -1  if qword1 < qword2
   0  if qword1 = qword2
  +1  if qword1 > qword2

SIDE EFFECTS
  None.

===========================================================================*/
short QW_CMP
(
  QWORD  qw1,
    /* the first qword to compare  */
  QWORD  qw2
    /* the second qword to compare */
)
{

  if( *((DWORD *)qw1+1) > *((DWORD *)qw2+1) )
  {
      return  1;
  }
  else if( *((DWORD *)qw1+1) < *((DWORD *)qw2+1) )
  {
      return -1;
  }
  else
  {
      if( *((DWORD *)qw1) > *((DWORD *)qw2) )
      {
          return 1;
      }
      else if( *((DWORD *)qw1) < *((DWORD *)qw2) )
      {
          return -1;
      }
      return 0;
  }

} /* qw_cmp */


UINT8  EQS_API MakeValidString(BYTE* String, UINT8 nLength)
{
    INT16 i;

    i = nLength -1;
    while( String[i] == ' ')
    {          
        i--;
        if( i < 0 )
            break;
    }
    String[i+1] = 0;
    return (i+1);
}

void  EQS_API MakeOldString(BYTE* String, UINT8 nLength)
{
    UINT8 i;

    i = STRLEN(String);
    for( i = STRLEN(String); i<nLength; i++)
        String[i] = ' '; 
}
     
UINT8 EQS_API GetDaysOfMonth( WORD wOneYear, INT8 nOneMonth )
{
    UINT8 anDaysOfMonth[] =
    {
      //   1   2   3   4   5   6   7   8   9   10  11  12
        0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    UINT8 nDay;

    if( nOneMonth == 2 )
    {
        BOOL fLongFeb = FALSE;

        /* 
        ** 연도 값이 4로 나뉘어지면 윤년입니다. 
        ** 하지만 다시 100으로 나뉘어지면 평년입니다. 
        ** 그러나 100으로 나뉘어지더라도 다시 400으로 나뉘어지면 윤년입니다. 
        ** 따라서 1900년은 윤년이 아니지만, 2000년은 윤년입니다
        */
        if( wOneYear%4 == 0 )
        {
            if( wOneYear%100 == 0 ) 
            {
                fLongFeb = (wOneYear%400 == 0) ? TRUE : FALSE;
            }
            else
            {
                fLongFeb = TRUE;
            }
        }

        nDay = fLongFeb ? 29 : 28;
    }
    else
    {
        nDay = anDaysOfMonth[nOneMonth];
    }

    return nDay;
}



//redstar   2000.11.15

void InitBookListTable(void)
{   
    INT   i;

    for(i=0; i< MAX_LIST_NUMBER; i++)
    {
        ListTable[i].wMaxSize = 0;
        ListTable[i].pnType   = NULL;
        ListTable[i].pnValue  = NULL;

    }

}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     CreateList
**
** Description: 
**     
**
** Input:
**     
**
** Output:
**     
**
** Return value:
**     list handle  number ( handle number is greater than 0 )
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/


UINT8 CreateList(WORD wMaxSizeA)
{
    INT8  handle;
    INT   i;

    for(i=0; i< MAX_LIST_NUMBER; i++)
    {
        if( ListTable[i].wMaxSize == 0 )
        {
            ListTable[i].wMaxSize = wMaxSizeA;
            ListTable[i].wPos  = 0;
            ListTable[i].wSize = 0;
            break;
        }
    }
    if( i >= MAX_LIST_NUMBER  || wMaxSizeA == 0)
    {
		ASSERT( i < MAX_LIST_NUMBER );
        return 0;
    }
    handle = i;
    ListTable[handle].pnType  = (INT8*)  SKY_Malloc((SIZE_T)(sizeof(INT8)*wMaxSizeA));
    ListTable[handle].pnValue = (INT16*) SKY_Malloc((SIZE_T)(sizeof(INT16)*wMaxSizeA));
    memset( ListTable[handle].pnType, 0, ListTable[handle].wMaxSize);
    memset( ListTable[handle].pnValue,0, ListTable[handle].wMaxSize);
       
    return (handle+1);
    
}

BOOL DeleteList(UINT8 handle )
{
    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return FALSE;

    if( ListTable[handle-1].wMaxSize != 0 )
    {
        ListTable[handle-1].wMaxSize = 0;
        SKY_Free( ListTable[handle-1].pnType );
        SKY_Free( ListTable[handle-1].pnValue);
        ListTable[handle-1].pnType   = NULL;
        ListTable[handle-1].pnValue  = NULL;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     AddList
**
** Description: 
**     
**
** Input:
**     handle: list handle number
**     str   : string pointer of list data
**     value : value of list data
**     type  : type  of list data
**
** Output:
**     
**
** Return value:
**     Success  or Fail
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

BOOL AddList(UINT8  handle,  INT16 value, INT8  type)
{
    INT8 nIndex;
    WORD nPos;

    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return FALSE;

    nIndex = handle-1;

    if( ListTable[nIndex].wSize >= ListTable[nIndex].wMaxSize )
        return FALSE;

    nPos = ListTable[nIndex].wSize;

    ListTable[nIndex].pnValue[nPos] = value;
    ListTable[nIndex].pnType[nPos]  = type;
    ListTable[nIndex].wSize++;

    return TRUE;
}


BOOL RemoveListItem(UINT8  handle, WORD pos)
{
    INT8 nIndex;
    INT  i;

    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return FALSE;

    nIndex = handle-1;

    if( pos >= ListTable[nIndex].wSize )
        return FALSE;

    for(i=pos+1; i<ListTable[nIndex].wSize; i++)
    {
        ListTable[nIndex].pnValue[i-1] = ListTable[nIndex].pnValue[i];
        ListTable[nIndex].pnType[i-1]  = ListTable[nIndex].pnType[i] ;
    }
    ListTable[nIndex].wSize--;

    return TRUE;
}

BOOL RemoveListValue(UINT8 handle, INT16 Value)
{
    INT8 nIndex;
    INT  i, k;

    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return FALSE;

    nIndex = handle-1;


    for(i=0; i<ListTable[nIndex].wSize; i++)
    {
        if( ListTable[nIndex].pnValue[i] == Value )
            break;
    }
    if( i >= ListTable[nIndex].wSize )
        return FALSE;

    for(k= i+1; k<ListTable[nIndex].wSize; k++)
    {
        ListTable[nIndex].pnValue[k-1] = ListTable[nIndex].pnValue[k];
        ListTable[nIndex].pnType[k-1]  = ListTable[nIndex].pnType[k] ;
    }

    ListTable[nIndex].wSize--;

    return TRUE;
}



BOOL RemoveListType(UINT8 handle, INT8 Type)
{
    INT8 nIndex;
    INT  i, k;

    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return FALSE;

    nIndex = handle-1;


    for(i=0; i<ListTable[nIndex].wSize; i++)
    {
        if( ListTable[nIndex].pnType[i] == Type )
            break;
    }
    if( i >= ListTable[nIndex].wSize )
        return FALSE;

    for(k= i+1; k<ListTable[nIndex].wSize; k++)
    {
        ListTable[nIndex].pnValue[k-1] = ListTable[nIndex].pnValue[k];
        ListTable[nIndex].pnType[k-1]  = ListTable[nIndex].pnType[k] ;
    }

    ListTable[nIndex].wSize--;

    return TRUE;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     InsertList
**
** Description: 
**     
**
** Input:
**     handle   : list handle number
**     position : position value of list data
**     str      : string pointer of list data
**     value    : value of list data
**     type     : type  of list data
**
** Output:
**     
**
** Return value:
**     Success  or Fail
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

BOOL InsertList(UINT8  handle, WORD pos, INT16 value, INT8  type)
{

    INT8 nIndex;
    INT  i;

    if( handle == 0 || handle > MAX_LIST_NUMBER ||  pos >= MAX_LIST_SIZE)
        return FALSE;

    nIndex = handle - 1;

    if(pos > ListTable[nIndex].wSize ||\
       ListTable[nIndex].wSize == ListTable[nIndex].wMaxSize )
        return FALSE;

    for(i= ListTable[nIndex].wSize-1; i>= pos; i-- )
    {
        ListTable[nIndex].pnValue[i+1] = ListTable[nIndex].pnValue[i];
        ListTable[nIndex].pnType[i+1]    = ListTable[nIndex].pnType[i];
    }

    ListTable[nIndex].pnValue[pos] = value;
    ListTable[nIndex].pnType[pos]  = type;

    ListTable[nIndex].wSize++;
    return TRUE;

}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetListValue
**
** Description: 
**     
**
** Input:
**     handle   : list handle number
**     pos      : position value of list data
**     value    : value of list data
**
** Output:
**     
**
** Return value:
**     Success  or Fail
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

BOOL SetListValue(UINT8   handle, WORD pos, INT16 value)
{

    INT8 nIndex;

    if( handle == 0 || handle > MAX_LIST_NUMBER || pos >= MAX_LIST_SIZE)
        return FALSE;

    nIndex = handle - 1;

    if( pos >= ListTable[nIndex].wSize )
        return FALSE;

    ListTable[nIndex].pnValue[pos] = value;

    return TRUE;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetListType
**
** Description: 
**     
**
** Input:
**     handle   : list handle number
**     pos      : position value of list data
**     type     : type of list data
**
** Output:
**     
**
** Return value:
**     Success  or Fail
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

BOOL SetListType(UINT8    handle, WORD pos, INT8 type)
{
    INT8 nIndex;

    if( handle == 0 || handle > MAX_LIST_NUMBER || pos >= MAX_LIST_SIZE)
        return FALSE;

    nIndex = handle - 1;

    if( pos >= ListTable[nIndex].wSize )
        return FALSE;

    ListTable[nIndex].pnType[pos] = type;

    return TRUE;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetListValue
**
** Description: 
**     
**
** Input:
**     handle   : list handle number
**     pos      : position value of list data
**     
**
** Output:
**     
**
** Return value:
**    value variable in list data
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

INT16   GetListValue(UINT8   handle, WORD pos)
{

    INT8 nIndex;

    nIndex = handle - 1;
       
    if( handle == 0 || handle > MAX_LIST_NUMBER ||  pos >= ListTable[handle-1].wSize)
        return FALSE;

    return ListTable[nIndex].pnValue[pos];

}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetListType
**
** Description: 
**     
**
** Input:
**     handle   : list handle number
**     pos      : position value of list data
**     
**
** Output:
**     
**
** Return value:
**    type variable in list data
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

INT8  GetListType(UINT8    handle, WORD pos)
{

    INT8 nIndex;

    nIndex = handle - 1;
       
    if( handle == 0 || handle > MAX_LIST_NUMBER || pos >= ListTable[handle-1].wSize)
        return FALSE;

    return ListTable[nIndex].pnType[pos];
}

BOOL  SwapListItem(UINT8    handle, WORD Pos1, WORD Pos2)
{

    INT8  nIndex;
    INT16 TempValue;
    INT8  TempType;

    nIndex = handle - 1;
       
    if( handle == 0 || handle > MAX_LIST_NUMBER || \
        Pos1 >= ListTable[handle-1].wSize || \
        Pos2 >= ListTable[handle-1].wSize || Pos1 == Pos2)
        return FALSE;

    TempValue = ListTable[nIndex].pnValue[Pos1];
    TempType  = ListTable[nIndex].pnType[Pos1];

    ListTable[nIndex].pnValue[Pos1] = ListTable[nIndex].pnValue[Pos2];
    ListTable[nIndex].pnType[Pos1]  = ListTable[nIndex].pnType[Pos2];
    ListTable[nIndex].pnValue[Pos2] = TempValue;
    ListTable[nIndex].pnType[Pos2]  = TempType;
    
    return TRUE;
}

INT16    FindList(UINT8       handle, INT16 Value)
{

    INT16  nIndex;
    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return -1;

    for( nIndex=0; nIndex< ListTable[handle-1].wSize; nIndex++)
    {
        if( ListTable[handle-1].pnValue[nIndex] == Value )
            return nIndex;
    }
    return -1;
}


INT16    FindListType(UINT8       handle, INT8 Type)
{

    INT16  nIndex;
    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return -1;

    for( nIndex=0; nIndex< ListTable[handle-1].wSize; nIndex++)
    {
        if( ListTable[handle-1].pnType[nIndex] == Type )
            return nIndex;
    }
    return -1;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetCurrentPos
**
** Description: 
**     
**
** Input:
**     handle   : list handle number
**     pos      : position value of list data
**     
**
** Output:
**     
**
** Return value:
**    handle for list data
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

INT   SetCurrentPos(UINT8  handle, WORD pos)
{

    if( handle == 0 || handle > MAX_LIST_NUMBER || pos >= ListTable[handle-1].wSize)
        return -1;

    ListTable[handle-1].wPos = pos;   
    return ListTable[handle-1].wPos;

}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     ForwardList
**
** Description: 
**     
**
** Input:
**     handle   : list handle number     
**
** Output:
**     
**
** Return value:
**    handle for list data
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

INT  ForwardList( UINT8 handle )
{

    if( handle == 0 || handle > MAX_LIST_NUMBER )    
        return -1;

    ListTable[handle-1].wPos++;
    if( ListTable[handle-1].wPos >= ListTable[handle-1].wSize )
        return -1;
    return ListTable[handle-1].wPos;    
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     BackwardList
**
** Description: 
**     
**
** Input:
**     handle   : list handle number     
**
** Output:
**     
**
** Return value:
**    handle for list data
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

INT BackwardList( UINT8 handle )
{
    if( handle == 0 || handle > MAX_LIST_NUMBER )    
        return -1;

	if( ListTable[handle-1].wPos == 0 )
		return -1;

    ListTable[handle-1].wPos--;
    return ListTable[handle-1].wPos;
}

WORD SetListSize(UINT8 handle, WORD wSizeA)
{
    if( handle == 0 || handle > MAX_LIST_NUMBER )
       return 0;

	if( wSizeA <= ListTable[handle-1].wMaxSize )
	{
		return ListTable[handle-1].wSize = wSizeA;
	}
	else
		return 0;
}

WORD GetListSize(UINT8 handle)
{

    if( handle == 0 || handle > MAX_LIST_NUMBER )    
       return 0;

    return ListTable[handle-1].wSize;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCurrentValue
**
** Description: 
**     
**
** Input:
**     handle   : list handle number     
**
** Output:
**     
**
** Return value:
**    value variable in list data
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/

INT   GetCurrentValue(UINT8   handle)
{

    WORD pos;

    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return -1;    

    pos = ListTable[handle-1].wPos;
    return ListTable[handle-1].pnValue[pos];
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCurrentType
**
** Description: 
**     
**
** Input:
**     handle   : list handle number     
**
** Output:
**     
**
** Return value:
**    type variable in list data
**
** Side effects:
**     
**
** ---------------------------------------------------------------------------
*/
INT8  GetCurrentType(UINT8    handle)
{

    WORD pos;

    if( handle == 0 || handle > MAX_LIST_NUMBER )
        return -1;    

    pos = ListTable[handle-1].wPos;
    return ListTable[handle-1].pnType[pos];
}