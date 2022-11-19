/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_clst.h
**
** Description:
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2001 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-07-21 hjcho     Created.
**
** ============================================================================
*/

#ifndef _SKA_CLST_H_
#define _SKA_CLST_H_

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

#define MAX_CALL_STACK_ID_SIZE      6       /* strlen(CALL_STACK_ID) */
#define CALL_STACK_ID               (BYTE*)"SKYCS"

#ifdef FEATURE_CALL_STACK_SEPERATED
#define CALL_STACK_VERSION          1       /* 1 ~ 255 */
#else
#define CALL_STACK_VERSION          0       /* 1 ~ 255 */
#endif //FEATURE_CALL_STACK_SEPERATED

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/*
**  Call Stack Index Type Structure
*/
typedef struct tagCSINDEX_T             CALLSTACKINDEX_T;
struct tagCSINDEX_T{
  BYTE      aIndex[NV_MAX_STACK_DIALS];    /* Index array */
  BOOL      aSecret[NV_MAX_STACK_DIALS];   /* TRUE if a secret number */
};

/*
**  Call Stack Dial Type Structure
*/
typedef struct tagCSDIAL_T              CALLSTACKDIAL_T;
struct tagCSDIAL_T{
  BYTE      bAddress;                      /* Storage address       */
  BYTE      bStatus;                       /* flag bits             */
  DWORD     nTimeStamp;                    /* years to seconds      */
  BYTE      bNumDigits;                    /* Number of digits      */
  BYTE      aDigits[NV_MAX_DIAL_DIGITS+1];   /* Array of digits       */
  BYTE      aName[SKY_CALL_MAX_LTRS];      /* Name from CNAP        */
};


/*
**  Call Stack Type Structure
*/
typedef struct tagUSABLEINDEXLIST_T       CALLUSABLEINDEXLIST_T;
struct tagUSABLEINDEXLIST_T{
  INT       bCurrentPos;
  BYTE      aUsableIndex[NV_MAX_STACK_DIALS];
};


typedef struct tagCALLSTACK_T            CALLSTACK_T;
struct tagCALLSTACK_T
{
    BYTE                     abCallStack[MAX_CALL_STACK_ID_SIZE];
    BYTE                     bVersion;
    CALLUSABLEINDEXLIST_T    UsableList;
    BOOL                     fChange;                     //ui.schg
    CALLSTACKINDEX_T         Index;                       //ui.sidx
    CALLSTACKDIAL_T          Stack[NV_MAX_STACK_DIALS];   //ui.st
};

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
VOID SKY_API SKA_MainCallList(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_DisplayDetailInfo(SKY_EVENT_T* pEvent);

extern BYTE GetCallStackIndex(void);
extern void SetCallStackIndex(BYTE bCurrIndex);
extern void InitializeLastDialInfo(void);
extern void UpdateLastDialInfo(BYTE* pszNumber, BYTE bLastDialLength);
extern EDITORINFO_T* GetLastDialInfo(void);
extern void UpdateLastDialInfo(BYTE* pszNumber, BYTE bLastDialLength);
extern BOOL TransferLastDialInfo2CurrentDialInfo(void);
extern CALLSTACKINDEX_T GetCSKIndex(void);
extern void SetCSKIndex(CALLSTACKINDEX_T Index);

extern CALLSTACKDIAL_T GetCSKStack(UINT8 nIndex);
extern void SetCSKStack(UINT8 nIndex, CALLSTACKDIAL_T Data);

extern CALLSTACK_T* GetCSKData(void);
extern void SetCSKData(CALLSTACK_T* pCallStack);

extern BOOL GetCSKChange(void);
extern void SetCSKChange(BOOL fValue);

extern void SKY_API SKY_InitCallStack(void);
extern void SKY_API WriteCallStack(void);
extern void SKY_API DeleteAllCallStack(void);

#ifdef FEATURE_CALL_STACK_SEPERATED
extern VOID SetReceivedCallStack(BOOL fIsReceived);
extern BOOL GetReceivedCallStack(void);
extern void SKY_API DeleteAllCallStackReceived(void);
extern void SKY_API SKY_InitCallStackReceived(void);
extern void SKY_API WriteCallStackReceived(void);
#endif //FEATURE_CALL_STACK_SEPERATED

extern void AddCallsList (       
       BYTE            bStatus,        // call to/from, 3-way etc
       CONST BYTE*     pszNumBuf,      // buffer for the number
       BYTE            bNumLen,        // length of the number
       CONST BYTE*     pszNameBuf      // buffer for the name
);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_CLST_H_ */
