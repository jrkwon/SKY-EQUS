/*
** ============================================================================
**
** Project: 
**     Graphics
**
** File:
**     eqs_util.h
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
** 2000-06-28 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_UTIL_H_
#define _EQS_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

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
**  Variable Declarations
**---------------------------------------------------------------------------
*/
#define MAX_SPECIAL_DIGIT          100
#define MAX_LIMIT_DIGIT             96


#define SPECIAL_DIGIT             0x85
#define SPECIAL_DIGIT_1           0xA1

#define SPECIAL_DIGIT_NEXT_BASE   0x86
#define SPECIAL_DIGIT_96          0xA0

#define GET_KB_SIZE(nSize)      ((nSize/1024) + ((nSize % 1024 > 0)? 1:0))

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
EQS_DCL SIZE_T   EQS_API STRLEN(CONST BYTE* pszString);
EQS_DCL BYTE*    EQS_API STRSTR(BYTE* pszSrc, BYTE* pszDest);
EQS_DCL BYTE*    EQS_API STRCAT(BYTE* pszHead, CONST BYTE* pszTail);
EQS_DCL BYTE*    EQS_API STRNCAT(BYTE *pszHead, CONST BYTE* pszTail, SIZE_T Count);
EQS_DCL BYTE*    EQS_API STRCPY(BYTE* pszDest, CONST BYTE* pszSrc);
EQS_DCL BYTE*    EQS_API STRNCPY(BYTE* pszDest, CONST BYTE* pszSrc, SIZE_T Count);
EQS_DCL BYTE*    EQS_API STRCHR(CONST BYTE* pszSrc, UINT8 c);
EQS_DCL INT      EQS_API STRCMP(CONST BYTE* pszSrc, CONST BYTE* pszDest);
EQS_DCL INT      EQS_API STRNCMP(CONST BYTE* pszSrc, CONST BYTE* pszDest, UINT16 nSize);
EQS_DCL BYTE*    EQS_API STRTOK(BYTE* pszSrc, CONST BYTE* pszDelimit);
EQS_DCL BYTE*    EQS_API STRUPR(BYTE* pszSrc);
EQS_DCL BYTE*    EQS_API STRLWR(BYTE* pszSrc);

EQS_DCL void     EQS_API MakeShortString( BYTE *pszSrc, BYTE nPosition);
EQS_DCL BOOL     EQS_API IsMiddleOfMultiByte(BYTE *pszSrc, UINT16 nPosition);
EQS_DCL void     EQS_API SetRect(RECT* pRect, COORD xLeft, COORD yTop, 
                                 COORD xRight, COORD yBottom);
EQS_DCL UINT8    EQS_API FindCodeSet(BYTE *pszNumber);
EQS_DCL UINT8    EQS_API FindHypen(BYTE *pszNumber, UINT8 nCodePos, UINT8 nMaxLen);
EQS_DCL UINT8    EQS_API MakeHypenedNumber(BYTE *pszSrcBuffer, BYTE *pszDestBuffer,
                               UINT8 nLen, UINT8 nCodePos, UINT8 nHypenPos);

EQS_DCL UINT8    EQS_API MakeValidString(BYTE* String, UINT8 nLength);
EQS_DCL void     EQS_API MakeOldString(BYTE* String, UINT8 nLength);

extern double EQS_API EQS_atof( char *num_string );

extern void QW_SET
(
  QWORD qw,
    /* the qword to set               */
  DWORD hi,
    /* the value for the high 32 bits */
  DWORD lo
    /* the value for the low 32 bits  */
);

extern DWORD QW_HI
(
  QWORD qw
    /* qword to return the high 32 bits from */
);

extern DWORD QW_LO
(
  QWORD qw
    /* qword to return the low 32 bits from */
);

extern void QW_EQU
(
  QWORD qw1,
    /* qw1 will be set equal to qw2       */
  QWORD qw2
    /* the qword qw1 will be set equal to */
);

extern void QW_INC
(
  QWORD  qw1,
    /* qword to increment  */
  DWORD  val
    /* value to add to qw1 */
);

extern void QW_DEC
(
  QWORD qw1,
    /* qword to decrement        */
  DWORD val
    /* value to decrement qw1 by */
);

extern void QW_SUB
(
  QWORD  difference,
    /* the result of the subtraction       */
  QWORD  subtrahend,
    /* the first argument for subtraction  */
  QWORD  subtractor
    /* the second argument for subtraction */
);

extern SHORT QW_CMP
(
  QWORD  qw1,
    /* the first qword to compare  */
  QWORD  qw2
    /* the second qword to compare */
);


#define MAX_LIST_NUMBER     50
#define MAX_LIST_SIZE      1200
typedef struct
{    
    INT16* pnValue;
    INT8*  pnType;
       
    WORD   wPos;
    WORD   wSize;
    WORD   wMaxSize;

} book_list_type;


/* list */
extern void     InitBookListTable(void);
extern UINT8    CreateList(WORD maxnumber);
extern BOOL     DeleteList(UINT8 handle );
extern BOOL     AddList(UINT8  handle,  INT16 value, INT8  type);
extern BOOL     RemoveListItem(UINT8  handle, WORD pos);
extern BOOL     RemoveListValue(UINT8 handle, INT16 Value);
extern BOOL     RemoveListType(UINT8 handle, INT8 Type);

extern BOOL     InsertList(UINT8  handle, WORD pos, INT16 value, INT8  type);
extern BOOL     SetListValue(UINT8   handle, WORD pos, INT16 value);
extern BOOL     SetListType(UINT8    handle, WORD pos, INT8 type);
extern INT16    GetListValue(UINT8   handle, WORD pos);
extern INT8     GetListType(UINT8    handle, WORD pos);
extern INT      SetCurrentPos(UINT8  handle, WORD pos);
extern INT16    FindList(UINT8       handle, INT16 Value);
extern INT16    FindListType(UINT8   handle, INT8 Type);
extern INT      ForwardList( UINT8 handle );
extern INT      BackwardList( UINT8 handle );


extern INT      GetCurrentValue(UINT8   handle);
extern INT8     GetCurrentType(UINT8    handle);

extern WORD     SetListSize(UINT8 handle, WORD wSizeA);
extern WORD     GetListSize(UINT8 handle);
extern BOOL     SwapListItem(UINT8    handle, WORD Pos1, WORD Pos2);

/*end of list structure and function*/

extern UINT8 EQS_API GetDaysOfMonth( WORD wOneYear, INT8 nOneMonth );




#ifdef __cplusplus
}
#endif

#endif  /* _EQS_UTIL_H_ */
