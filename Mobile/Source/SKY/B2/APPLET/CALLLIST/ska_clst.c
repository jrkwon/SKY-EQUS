/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_clst.c
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
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define CALLHISTORY_SYSTEM_CHAR_HEIGHT	 EQS_CharHeight('A')
#define CALLHISTORY_SYSTEM_CHAR_WIDTH	 EQS_CharWidth('A')
#define	SIZE_NUMBER_BUFFER               32
#define FILE_NAME_CALLSTACK              SKY_CALLSTACK_DIR""SKY_DIR_CHAR"CALLSTACK.SKY"
#define INIT_CALLSTACK_INDEX             255
#ifdef FEATURE_CALL_STACK_SEPERATED
#define	FILE_NAME_CALLSTACK_RECEIVED     SKY_CALLSTACK_DIR""SKY_DIR_CHAR"CALLSTACK_RCV.SKY"
#endif // FEATURE_CALL_STACK_SEPERATED


#define NUM_CALLHISTORY_HOTMENU             6
#define NUM_CALLHISTORY_DETAILINFO_HOTMENU  2
#define NUM_CURRENT_CALLHISTORY_HOTMENU		3
#define NUM_CALL_HISTORY					NV_MAX_STACK_DIALS


/* 
** NUM_CALL_SCROLL value will be ...
** B1 : 8 (view item count + 1)
** B2 : 9 (view item count + 1)
*/
#if SKY_MODEL_ID == SKY_MODEL_B1
#define  NUM_CALL_SCROLL                8
#elif SKY_MODEL_ID == SKY_MODEL_B2
#define  NUM_CALL_SCROLL                9
#endif


SKY_TOKEN paCurrentCallInfoHotMenu[NUM_CURRENT_CALLHISTORY_HOTMENU] =
{
  TKN_CALL_HISTORY_SMS,     /* 메시지작성 */
  TKN_VMEMO_DELETE_ONE,     /* 삭제       */
  TKN_VMEMO_DELETE          /* 전체삭제   */
};

enum tagCALLHISTORYDETAILINFO{
  CH_TO_SMS = 0,
  CH_DELETE_ONE,
  CH_DELETE_ALL 
};


enum tagMODE_E
{
  FROM_DISPCLOCK,
  FROM_SMS,
  FROM_PHOTO_MAIL
};

typedef enum tagMODE_E           MODE_E;



// 착발신 이력의 상세정보 보기에 사용됨
enum tagCALLHISTORY
{
  CH_DATE_TIME = 0,
  CH_NAME_CNAP,
  CH_NAME,
  CH_NUMBER_IMAGE,
  CH_NUMBER,
  CH_MAX
};


enum tagPOPUP_E
{
  PU_CALLHISTORY_NEW_SAVE,
  PU_CALLHISTORY_OLD_SAVE,
  PU_CALLHISTORY_PHONEBOOK,
  PU_CALLHISTORY_SMS,
  PU_CALLHISTORY_DELETE_ONE,
  PU_CALLHISTORY_DELETE_ALL
};


SKY_TOKEN paCallHistory[NUM_CALLHISTORY_HOTMENU] =    /* Hot Menu!! */
{
  TKN_PBOOK_SAVE_NEW_DATA,        /* 새이름에 저장  */
  TKN_PBOOK_SAVE_OLD_DATA,        /* 기존이름에 저장*/
  TKN_CALL_HISTORY_PHONEBOOK,     /* 전화부         */
  TKN_CALL_HISTORY_SMS,           /* SMS            */
  TKN_VMEMO_DELETE_ONE,           /* 삭제           */    
  TKN_VMEMO_DELETE                /* 전부삭제       */
};


UINT8 sInitCallList(void);


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
BYTE	 gCurrCallStackIndex = INIT_CALLSTACK_INDEX;
STATIC EDITORINFO_T LastDial;
BYTE*    szCallHistory[NUM_CALL_SCROLL+1];
BYTE     szList[NUM_CALL_SCROLL+1][MAX_DIAL_DIGIT_LENGTH+1];



/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void sUpdateCallHistoryList(BYTE* pszBuffer, INT16 nStackIndex);



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define SCROLLIST_SIZE   33
void sUpdateAllCallHistoryList(UINT8 nStartPoint, SKY_EVENT_E EventID)
{
  UINT8           nListHandle, nSize, nLoop, nIndex, nEndIndex, nStackIndex, nBufferIndex;
  BYTE            szBuf[NUM_CALL_HISTORY][SCROLLIST_SIZE];
  BYTE            szTmpBuf[MAX_DIAL_DIGIT_LENGTH+1];
  HCONTROL        hControl;
  CALLSTACK_T*    pCallStack;

  pCallStack = GetCSKData(); 
  hControl = EQS_GetNthIOC(0);


/* 전체 리스트를 새로 그려 줄 때 gfReceivedCallStack의 값을 확인한 뒤 */
/* 스크롤 리스트의 타이틀을 새로 써준다.                              */
/* 발신 이력을 통해 SMS 작성으로 들어간 뒤 다시 팝업 메뉴를 이용해서  */
/* 착신 이력에 접근한 뒤 상세정보, TOSMS를 통해 SMS 작성하는 곳으로   */
/* 접근 했다가 취소 키를 이용해 Pop을 할 경우, 타이틀과 내용이 틀리는 */
/* 버그가 발생한다.                                                   */
#ifdef FEATURE_CALL_STACK_SEPERATED    
    if(GetReceivedCallStack())
    {
       EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALL_HISTORY_CALLED_BY));  
    }
    else
    {
       EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALL_HISTORY_CALL_TO));  
    }
#else
    EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALL_HISTORY));  
#endif// FEATURE_CALL_STACK_SEPERATED  

  nListHandle = EQC_GetScrollListHandle(hControl);

  nSize = GetListSize(nListHandle);
  if(  nSize > NUM_CALL_SCROLL )
	   nSize = NUM_CALL_SCROLL;

  for( nLoop = 0; nLoop < NUM_CALL_SCROLL; nLoop++)
	  szBuf[nLoop][0] = 0;

  if(EventID == SE_IO_SCROLL_LIST_TOP)
  {
    FindCallerNameInBook(nListHandle, (BYTE*)szBuf, SCROLLIST_SIZE, 0, nSize);
  }
  else if(EventID == SE_IO_SCROLL_LIST_BOTTOM)
  {    
    FindCallerNameInBook(nListHandle, (BYTE*)szBuf, SCROLLIST_SIZE, nStartPoint, nSize);
  }

  nEndIndex = nStartPoint+nSize;

  if( nEndIndex > GetListSize(nListHandle) )  //redstar 
	  nEndIndex = GetListSize(nListHandle);

  for( nIndex = nStartPoint; nIndex < nEndIndex; nIndex++)
  { 
	nStackIndex = GetListValue(nListHandle, nIndex);
	nBufferIndex = nIndex - nStartPoint;


	if(STRLEN(pCallStack->Stack[nStackIndex].aName) != 0)   /* CNAP로 날아온 이름이 있는 경우 */
	{
		STRCPY(szTmpBuf, pCallStack->Stack[nStackIndex].aName);
	}
	else
	{
		if( STRLEN(szBuf[nBufferIndex]) == 0 )
        {
			if(STRLEN(pCallStack->Stack[nStackIndex].aDigits) == 0)
            {
               STRCPY(szTmpBuf, SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_NO_NUMBER_INFO));
            }
            else
            {
               //STRCPY(szTmpBuf, pCallStack->Stack[nStackIndex].aDigits);
               MakeHypenedPhoneNumber(pCallStack->Stack[nStackIndex].aDigits, szTmpBuf);
            }
        }
		else
        {
			STRCPY(szTmpBuf, szBuf[nBufferIndex]);
        }
	}

	switch(pCallStack->Stack[nStackIndex].bStatus)
	{
	case CT_CALLED_BY:
	  sprintf((CHAR*)EQC_GetScrollData(hControl, nBufferIndex), 
              (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
              SF_ALERT_CALL1_2,
			  //SF_ALERT_CALL2_2,
			  szTmpBuf);      
	  break;

	case CT_CALL_TO:
	  sprintf((CHAR*)EQC_GetScrollData(hControl, nBufferIndex), 
              (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
              SF_ORIGINATE_CALL1_2,
			  //SF_ORIGINATE_CALL2_2,
			  szTmpBuf);      
	  break;

	case CT_MISSED_CALL:
	  sprintf((CHAR*)EQC_GetScrollData(hControl, nBufferIndex), 
              (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
              SF_MISSED_CALL1_2,
			  //SF_MISSED_CALL2_2,
			  szTmpBuf);      
	  break;

	case CT_SMS_MO:
	  sprintf((CHAR*)EQC_GetScrollData(hControl, nBufferIndex), 
              (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
              SF_MESSAGE_SENT1_2,
			  //SF_MESSAGE_SENT2_2,
			  szTmpBuf);      
	  break;

	case CT_SMS_MT:
	  sprintf((CHAR*)EQC_GetScrollData(hControl, nBufferIndex), 
              (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
              SF_MESSAGE_RECEIVED1_2,
			  //SF_MESSAGE_RECEIVED2_2,
			  szTmpBuf);     
	  break;
    
    case CT_CALLED_BY_INPROG:
      sprintf((CHAR*)EQC_GetScrollData(hControl, nBufferIndex), 
              (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
              SF_LINE_BUSY_RECEIPT1_2,
			  //SF_LINE_BUSY_RECEIPT2_2,
			  szTmpBuf);
		  break;    

	}
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
READPBOOK_E sSearchPBookWithNumber(BYTE* aPhoneNumber, INT16* nAddress)
{
  INT16 nPBookAddress;
  
  nPBookAddress = SearchNumber(aPhoneNumber, BSM_NUMBER_FULL_MATCH);
  *nAddress = nPBookAddress;

  if( nPBookAddress > 0 )
  {
    if( IS_SECRET_GROUP(g_PersonGroup[nPBookAddress-1]) == TRUE )
    {
      return PB_SECRET_OK;
    }
    else
    {
      return PB_READ_OK;
    }
  }
  else
  {
      return PB_READ_NO_BOOK_INFO;
  }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL UINT8 sGetNumberOfCurrentCallHistory()
{
  UINT8           nLoop, nNumOfCallHistory;
  CALLSTACK_T*    pCallStack;
  
  pCallStack = GetCSKData();
  nNumOfCallHistory = 0;

  for(nLoop = 0; nLoop < NUM_CALL_HISTORY; nLoop ++)
  {
     if(pCallStack->Index.aIndex[nLoop] != INIT_CALLSTACK_INDEX)  
         nNumOfCallHistory++;
  }
  
  if(nNumOfCallHistory > 0) nNumOfCallHistory--;


  return nNumOfCallHistory;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL UINT8 sIsData()
{
   UINT8           nVSize;
   UINT8           ListHandle;    

   ListHandle = sInitCallList();
   nVSize = GetListSize(ListHandle);  
   if(nVSize == 0)
   {
	   DeleteList(ListHandle);
	   return 0;
   }
   else
   {
	   return ListHandle;
   }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     LOCAL VOID DisplayCallList(BYTE * aNumBuf)
**
** Description: 
**     Display call list info.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define CALLHISTORY_SIZE  18
//STATIC  HCONTROL  hCallListScrollList;


LOCAL UINT8 sDisplayCallList(UINT8 ListHandle)
{
   HCONTROL        hControl;
   UINT8           nVSize;
   CALLSTACK_T*    pCallStack;
    
   pCallStack = GetCSKData();
  
   nVSize = GetListSize(ListHandle);  
   if(nVSize == 0)  return nVSize;
   if(nVSize > NUM_CALL_HISTORY) nVSize = NUM_CALL_SCROLL;

   hControl = EQC_ScrollList(EQS_NA, EQS_NA, CALLHISTORY_SIZE, EQC_GetScrollDisplayLine(), \
                             szCallHistory, EQC_GetScrollDisplayLine()+1, nVSize, 0);
   

   EQC_SetLogicalList(hControl, TRUE); // IO 이벤트를 받을 수 있다.
   EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_BORDER | CS_SCROLL_LIST_MAXIMIZE |
                            CS_SCROLL_LIST_ENUMBERATED));
   EQC_SetScrollListHandle(hControl,ListHandle);

#ifdef FEATURE_CALL_STACK_SEPERATED    
    if(GetReceivedCallStack())
    {
       EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALL_HISTORY_CALLED_BY));  
    }
    else
    {
       EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALL_HISTORY_CALL_TO));  
    }
#else
    EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALL_HISTORY));  
#endif// FEATURE_CALL_STACK_SEPERATED

   EQC_ShowScrollTitle(hControl, TRUE);
   EQS_SetIOCActive(hControl);

//   hCallListScrollList = hControl;
   return nVSize;
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     CNAP name must be saved in Call Stack.
**     When a user wants to see a detailed information in a call history,
**     We compare CNAP name and Phonebook name.
**     If two names are different then we will show the two names, CANP name and
**     phonebook name.
**     Otherwise we will show one name.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void AddCallsList (                 
  BYTE            bStatus,         // call to/from, 3-way etc
  CONST BYTE*     pszNumBuf,       // buffer for the number
  BYTE            bNumLen,         // length of the number
  CONST BYTE*     pszNameBuf       // buffer for CNAP Name
)
{  
    UINT8           nloop;       
    BYTE            bIndx;       
    CALLSTACK_T*    pCallStack;  
	BOOL            fSameCallToDialNumber;
    BOOL            fOldCallStackValue;

	fSameCallToDialNumber = FALSE;
    bIndx = INIT_CALLSTACK_INDEX; 

#ifdef FEATURE_CALL_STACK_SEPERATED    
    fOldCallStackValue = GetReceivedCallStack();
    if((bStatus == CT_CALL_TO) || (bStatus == CT_SMS_MO))
    {
        SetReceivedCallStack(FALSE);
    }
    else
    {
        SetReceivedCallStack(TRUE);
    }
#endif// FEATURE_CALL_STACK_SEPERATED    

    pCallStack = GetCSKData();      // Read Call Stack


	if(bStatus == CT_CALL_TO)
	{
		bIndx = pCallStack->Index.aIndex[0];
		if(STRCMP(pszNumBuf, pCallStack->Stack[bIndx].aDigits) == 0)
		{
			fSameCallToDialNumber = TRUE;
		}
		else
		{
			fSameCallToDialNumber = FALSE;
		}			              
	}


	/* 동일한 곳으로 여러번 발신할 경우에 마지막에 발신한 하나의 데이터만 */
	/* 발신이력에 남도록 한다.                                            */
    if(bStatus == CT_CALL_TO && fSameCallToDialNumber)
	{
        bIndx = pCallStack->Index.aIndex[0]; 

		pCallStack->Stack[bIndx].bAddress = bIndx;
		pCallStack->Stack[bIndx].nTimeStamp = SKY_GetLocalTime();
	}
	else
	{
		if(pCallStack->UsableList.bCurrentPos >= 0)
		{
			bIndx = pCallStack->UsableList.aUsableIndex[0];
			for(nloop=0 ; nloop<NV_MAX_STACK_DIALS-1 ; nloop++)
			{
			  pCallStack->UsableList.aUsableIndex[nloop] = 
						  pCallStack->UsableList.aUsableIndex[nloop+1];
			}
			pCallStack->UsableList.aUsableIndex[NV_MAX_STACK_DIALS-1] = INIT_CALLSTACK_INDEX;
			pCallStack->UsableList.bCurrentPos--;
		}
		else
		{
			bIndx = pCallStack->Index.aIndex[NV_MAX_STACK_DIALS-1];
		}

		for(nloop = NV_MAX_STACK_DIALS-1; nloop>0 ; nloop--)
		{
			pCallStack->Index.aIndex[nloop] = pCallStack->Index.aIndex[nloop-1];
			pCallStack->Index.aSecret[nloop] = pCallStack->Index.aSecret[nloop-1];
		}         


		pCallStack->Index.aIndex[0] = bIndx;
		pCallStack->Index.aSecret[0] = FALSE;
  
		pCallStack->Stack[bIndx].bAddress = bIndx;
		pCallStack->Stack[bIndx].nTimeStamp = SKY_GetLocalTime();
		pCallStack->Stack[bIndx].bStatus = bStatus;

		if(bNumLen > sizeof(pCallStack->Stack[0].aDigits))
		{
		  pCallStack->Stack[bIndx].bNumDigits = sizeof(pCallStack->Stack[0].aDigits);
		}
		else
		{
		  pCallStack->Stack[bIndx].bNumDigits = bNumLen;
		}
    
		/* CNAP 데이터가 있었던 buffer를 clear한다. */
		if(pCallStack->Stack[bIndx].aName[0] != '\0')  
		{      
		  memset(pCallStack->Stack[bIndx].aName, '\0', SKY_CALL_MAX_LTRS);	  
		}

		(void) memcpy(pCallStack->Stack[bIndx].aDigits, pszNumBuf, pCallStack->Stack[bIndx].bNumDigits);
		pCallStack->Stack[bIndx].aDigits[pCallStack->Stack[bIndx].bNumDigits] = '\0';
	}

    
	/* 발신일 경우에만 한해서 Last Dial Infomation을 갱신한다. */    
    //if(bStatus == CT_CALL_TO)
    //{
    //  UpdateLastDialInfo((BYTE*)pszNumBuf, pCallStack->Stack[bIndx].bNumDigits);
    //
    //  /* 파일에 저장한다. */
    //  SKY_SET_LASTDIALINFO_BUFF(pCallStack->Stack[bIndx].aDigits, pCallStack->Stack[bIndx].bNumDigits);
    //  SKY_SET_LASTDIALINFO_SECRET(FALSE);
    //  SKY_SET_LASTDIALINFO_POS(pCallStack->Stack[bIndx].bNumDigits);
    //}

    if((pszNameBuf[0] != '\0') && ((bStatus == CT_CALLED_BY) || 
                                   (bStatus == CT_CALLED_BY_INPROG) ||
                                   (bStatus == CT_MISSED_CALL)))
    {
      (void) STRCPY(pCallStack->Stack[bIndx].aName, pszNameBuf);      
    }    

#ifdef FEATURE_CALL_STACK_SEPERATED    
    if(GetReceivedCallStack())
    {
       WriteCallStackReceived();        
    }
    else
    {
        WriteCallStack();   
    }
    SetReceivedCallStack(fOldCallStackValue);
#else
    WriteCallStack();   
    SetReceivedCallStack(fOldCallStackValue);
#endif// FEATURE_CALL_STACK_SEPERATED 

    if(bStatus == CT_SMS_MT)
    {
        EQS_SEND_EVENT(SE_CALL_STACK_CHANGED, 0, 0);
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define MAX_CHAR_NUM_IN_DETAIL_INFO  14
#define PHONE_NUMBER_PREFIX_SKT        "011"
#define PHONE_NUMBER_PREFIX_STI        "017"


READPBOOK_E sGetNameWithNum(BYTE* aPhoneNumber, BYTE aName[], BYTE aNameInPBook[])
{
  INT16                  nBookAddress;  
  UINT8                  /*i,j,*/nType;
  INT8                   nDiff,  nCount;
  BYTE                   szTypeFontBuf[5], szBuf[SIZE_NUMBER_BUFFER];
  BYTE                   szTmpNameBuf[MAX_BOOK_NAME+1];
  pb_number_info_type    numberinfo;
  pb_book_info_type      sPBookInfo;

 
  nBookAddress = SearchNumber(aPhoneNumber, BSM_NUMBER_FULL_MATCH);   

  if( nBookAddress > 0 )
  {
    if( IS_SECRET_GROUP(g_PersonGroup[nBookAddress-1]) == TRUE )
    {
      aName[0] = '\0';
      return PB_SECRET_OK;
    }
    else
    {
      sPBookInfo.address = nBookAddress;	  
	  if(ui_get_pb(PB_BOOK_INFO_I, &sPBookInfo) == TRUE)
	  {
        numberinfo.address = sPBookInfo.number_address;
		nCount = 0; 
        while( ui_get_pb( PB_NUMBER_INFO_I, &numberinfo) > 0 && nCount < PERSONAL_NUMBER_LIMIT )
		{
		   nCount++;
		   if( numberinfo.digits[0] == 0 || aPhoneNumber[0] == 0 )
			   break;

           nDiff = STRLEN((BYTE*)aPhoneNumber) - STRLEN((BYTE*)numberinfo.digits);
		   
		   if(nDiff == 0)
		   {
			 STRCPY(szBuf, aPhoneNumber);
             if(STRCMP((BYTE*)numberinfo.digits, szBuf) == 0) 
             {			 
		        nType = numberinfo.number_type;
		        break;
             }
		     else
             {
               numberinfo.address = numberinfo.next_index;
             }
		   }
		   else if(nDiff == 3)
		   {
             
             if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
             {
                STRCPY(szBuf, (BYTE*)PHONE_NUMBER_PREFIX_SKT);
             }
             else
             {
                STRCPY(szBuf, (BYTE*)PHONE_NUMBER_PREFIX_STI);
             }

			 STRCAT(szBuf, (BYTE*)numberinfo.digits);
             if(STRCMP((BYTE*)aPhoneNumber, szBuf) == 0) 
             {			 
		        nType = numberinfo.number_type;
		        break;
             }
		     else
             {
               numberinfo.address = numberinfo.next_index;
             }
		   }
		   else if(nDiff == -3)
		   {             
             if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
             {
                STRCPY(szBuf, (BYTE*)PHONE_NUMBER_PREFIX_SKT);
             }
             else
             {
                STRCPY(szBuf, (BYTE*)PHONE_NUMBER_PREFIX_STI);
             }			        
			 STRCAT(szBuf, (BYTE*)aPhoneNumber);
             if(STRCMP((BYTE*)numberinfo.digits, szBuf) == 0) 
             {			 
		        nType = numberinfo.number_type;
		        break;
             }
			 else
			 {
              numberinfo.address = numberinfo.next_index;
			 }
		   }
		   else
		   {
              numberinfo.address = numberinfo.next_index;
		   }                
		 }
	 }	
      GetNumberTypeSFont(nType, szTypeFontBuf);

	  STRCPY(szTmpNameBuf, g_szPersonName[nBookAddress-1]);
	  STRCPY(aNameInPBook, g_szPersonName[nBookAddress-1]);
      MakeShortString(szTmpNameBuf, MAX_CHAR_NUM_IN_DETAIL_INFO);
	  //sprintf((char*)aName,"%s%s", g_szPersonName[nBookAddress-1], szTypeFontBuf);
	  sprintf((char*)aName,"%s%s", szTmpNameBuf, szTypeFontBuf);
	  return PB_READ_OK;
    }
  }
  else
  {
      return PB_READ_NO_BOOK_INFO;
  }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     TransferLastDialInfo2CurrentDialInfo
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL TransferLastDialInfo2CurrentDialInfo()
{
    EDITORINFO_T* sLastDial;      
    
    sLastDial = GetLastDialInfo();

    if(sLastDial->nPos == 0) return FALSE;
    else
    {
      SET_NUMEDIT_SECRET(FALSE);
      SET_NUMEDIT_POS(sLastDial->nPos);
      SET_NUMEDIT_BUFFER(sLastDial->aBuf, sLastDial->nPos);

      return TRUE;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void InitializeLastDialInfo(void)
{
    LastDial.nPos    = 0;
    LastDial.aBuf[0] = '\0';
    LastDial.fSecret = FALSE;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void UpdateLastDialInfo(BYTE* pszNumber, BYTE bLastDialLength)
{
	LastDial.fSecret = FALSE;
    LastDial.nPos    = bLastDialLength;
    memcpy(LastDial.aBuf, pszNumber, LastDial.nPos);
    LastDial.aBuf[bLastDialLength] = '\0';
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
//void UpdateLastDialInfoAsDeleteOneItem(void)            
//{
//    CALLSTACK_T*    pCallStack;
//    UINT8           bIndex, bStackIndex;
//
//
//    pCallStack = GetCSKData();      // Read Call Stack
//    
//    for(bIndex = 0; bIndex < NV_MAX_STACK_DIALS; bIndex++)
//    {
//      if(pCallStack->Index.aIndex[bIndex] != INIT_CALLSTACK_INDEX)
//      {
//         bStackIndex = pCallStack->Index.aIndex[bIndex];
//
//         if(pCallStack->Stack[bStackIndex].bStatus == CALL_TO)
//         {
//             UpdateLastDialInfo(pCallStack->Stack[bStackIndex].aDigits, 
//                                pCallStack->Stack[bStackIndex].bNumDigits);
//             break;
//         }
//      }
//      else
//      {
//          InitializeLastDialInfo();
//          break;
//      }
//    }
//}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
EDITORINFO_T* GetLastDialInfo(void)
{
    CALLSTACK_T*    pCallStack;
    UINT8           bIndex, bStackIndex;

#ifdef FEATURE_CALL_STACK_SEPERATED    
    /*발신이력을 검사한다 */
    SetReceivedCallStack(FALSE);
#endif// FEATURE_CALL_STACK_SEPERATED 

    pCallStack = GetCSKData();      // Read Call Stack
    
    for(bIndex = 0; bIndex < NV_MAX_STACK_DIALS; bIndex++)
    {
      if(pCallStack->Index.aIndex[bIndex] != INIT_CALLSTACK_INDEX)
      {
         bStackIndex = pCallStack->Index.aIndex[bIndex];

         if(pCallStack->Stack[bStackIndex].bStatus == CT_CALL_TO)
         {
             UpdateLastDialInfo(pCallStack->Stack[bStackIndex].aDigits, 
                                pCallStack->Stack[bStackIndex].bNumDigits);
             break;
         }
      }
      else
      {
          InitializeLastDialInfo();
          break;
      }
    }	

	return &LastDial;
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BYTE GetCallStackIndex()
{
    return gCurrCallStackIndex;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetCallStackIndex(BYTE bCurrIndex)
{
    gCurrCallStackIndex = bCurrIndex;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
STATIC BYTE   pszBuf[20];
#define CALLHISTORY_DETAILINFO_CALLNUMBER_XPOS   (DIAL_DIGIT_POS_X - 9)
#define CALLHISTORY_DETAILINFO_CALLNUMBER_YPOS   (DIAL_DIGIT_POS_Y - 5)

#define CALLHISTORY_CALL_TIME_XPOS           (SKY_SCREEN_WIDTH - (CALLHISTORY_SYSTEM_CHAR_WIDTH * 17) + 2)//22
#define CALLHISTORY_CALL_TIME_YPOS           1

#define CALLHISTORY_NAME_CNAP_XPOS	        4
#define CALLHISTORY_NAME_CNAP_YPOS	        (CALLHISTORY_SYSTEM_CHAR_HEIGHT * 3)

#define CALLHISTORY_NAME_INPHONEBOOK_XPOS			 CALLHISTORY_NAME_CNAP_XPOS
#define CALLHISTORY_NAME_INPHONEBOOK_YPOS			 (CALLHISTORY_SYSTEM_CHAR_HEIGHT * 4) + 1



LOCAL void sInitCurrentCallInfoDisp(void)
{
    //UINT8       nCtrl;
    HCONTROL    hControl;    

    // 날짜시간 정보                     CH_DATE_TIME
    hControl = EQC_StaticText(CALLHISTORY_CALL_TIME_XPOS, 
		                      CALLHISTORY_CALL_TIME_YPOS, 
				              (EQS_TOKEN)TKN_BLANK);
	EQC_SET_SELF_DRAW(hControl, FALSE);

    hControl = EQC_StaticText(CALLHISTORY_NAME_INPHONEBOOK_XPOS, 
		                      CALLHISTORY_NAME_INPHONEBOOK_YPOS, 
				              (EQS_TOKEN)TKN_BLANK);
	EQC_SET_SELF_DRAW(hControl, FALSE);

	
    hControl = EQC_StaticText(CALLHISTORY_NAME_CNAP_XPOS, 
	                          CALLHISTORY_NAME_CNAP_YPOS, 
				              (EQS_TOKEN)TKN_BLANK);
	EQC_SET_SELF_DRAW(hControl, FALSE);

	
	EQC_Image( 
		       CALLHISTORY_DETAILINFO_CALLNUMBER_XPOS, 
		       CALLHISTORY_DETAILINFO_CALLNUMBER_YPOS, 
			   (BITMAP*)&g_BmpCallPannel
			  );

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE,
                              MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );

	EQC_SET_SELF_DRAW(hControl, FALSE);	
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);        

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT       | 
                            CS_NUMBER_EDIT_NO_CML           |
                            CS_NUMBER_EDIT_IGNORE_CANCEL    |
                            CS_NUMBER_EDIT_RETURN_UPDOWN_KEY|
                            CS_NUMBER_EDIT_ORING            |
                            CS_NUMBER_EDIT_DRAW_ALL_CONTROL);
    EQC_SetNumberEditBigFont(hControl, TRUE);   // Set Big Font
	
    EQS_SetIOCActive(hControl);
}

#define sGENERAL_FRAME_HEIGHT            46
#define sGENERAL_FRAME_WIDTH             120

#define CALLHISTORY_DETAILINFO_CALLNUMBER_XPOS_RIGHT    (CALLHISTORY_DETAILINFO_CALLNUMBER_XPOS + sGENERAL_FRAME_WIDTH)  
#define CALLHISTORY_DETAILINFO_CALLNUMBER_YPOS_BOTTOM   (CALLHISTORY_DETAILINFO_CALLNUMBER_YPOS + sGENERAL_FRAME_HEIGHT)

//LOCAL void sDrawGeneralFrameInCallHistory(void)
//{
//    EQS_Rectangle(CALLHISTORY_DETAILINFO_CALLNUMBER_XPOS,  
//                  CALLHISTORY_DETAILINFO_CALLNUMBER_YPOS,
//                  CALLHISTORY_DETAILINFO_CALLNUMBER_XPOS_RIGHT,
//                  CALLHISTORY_DETAILINFO_CALLNUMBER_YPOS_BOTTOM);
//                  
//}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL IsInPhoneBook(UINT8 nCurrIndex, BYTE pszPhoneNumber[], INT16* nPBookAddress)
{
  CALLSTACK_T*  pCallStack;
  BYTE          bLength;

  pCallStack = GetCSKData();
  
  bLength = pCallStack->Stack[nCurrIndex].bNumDigits;
  pCallStack->Stack[nCurrIndex].aDigits[bLength] = '\0';

  STRCPY(pszPhoneNumber, pCallStack->Stack[nCurrIndex].aDigits);


  if(sSearchPBookWithNumber(pCallStack->Stack[nCurrIndex].aDigits, nPBookAddress) == PB_READ_OK)
  {
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
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL sUpdateDispCurrentCallInfo(UINT8 nCurrIndex, DWORD nLogicalIndex, 
                                BYTE szTitleHeader[],   MODE_E Mode)
{
    clk_julian_type   sJulian;    /* julian time conversion */
    CALLSTACK_T*      pCallStack;  	
    BYTE              bMode, bLength;
    BOOL              bAM, fNameInfo, fNumberInfo;
    BYTE              s_pszBuf[MAX_DIAL_DIGITS+1];
	BYTE              s_pszTmpCNAPBuf[MAX_DIAL_DIGITS+1];
	BYTE              s_pszPBookName[MAX_DIAL_DIGITS+1];

    pCallStack = GetCSKData();        
    pszBuf[0] = '\0';
    fNameInfo = TRUE;
    fNumberInfo = FALSE;

	EQC_SET_SELF_DRAW(EQS_GetNthIOC(CH_NUMBER), FALSE);

    //날짜
    clk_secs_to_julian((DWORD)pCallStack->Stack[nCurrIndex].nTimeStamp, &sJulian);
    bAM = (BOOL) ((sJulian.hour < 12) ? TRUE : FALSE);
    if(bAM == FALSE && sJulian.hour != 12)
       sJulian.hour -= 12;
    else if(bAM == FALSE && sJulian.hour == 24)
       sJulian.hour = 0;

    if (EQS_GetCountry() == COUNTRY_KOR)
    {
        sprintf((CHAR*)s_pszBuf, "%2d.%02d(%s)%2d:%02d%s", 
                                 sJulian.month, 
                                 sJulian.day, 
                                 (CHAR*)SKY_GET_TOKEN(g_aDayOfWeek[sJulian.day_of_week]),
                                 sJulian.hour,
                                 sJulian.minute,
                                 (bAM) ? SKY_GET_TOKEN(TKN_IDLE_AM): SKY_GET_TOKEN(TKN_IDLE_PM));
    }
    else
    {
        /* 다국어 버전을 위한 고려가 있어야 한다. */

    }
    EQC_SetStaticText(EQS_GetNthIOC(CH_DATE_TIME), s_pszBuf);    
    pszBuf[0] = '\0';

    //착신 또는 발신자 이름
    bLength = pCallStack->Stack[nCurrIndex].bNumDigits;
    pCallStack->Stack[nCurrIndex].aDigits[bLength] = '\0';   
    

    if(sGetNameWithNum(pCallStack->Stack[nCurrIndex].aDigits, pszBuf, s_pszPBookName) == PB_READ_OK)
    {       
       if(Mode == FROM_DISPCLOCK)
       {
        /* 번호정보가 있는 경우에 명령어 라인 세팅 */   
        fNumberInfo = TRUE;
        EQS_SetWindowCML(CL_MENU_PBOOK_CANCEL); 
       }       
	   

       if(STRLEN(pCallStack->Stack[nCurrIndex].aName) != 0) /* CNAP정보가 있는 경우 */
       {		   		 
		 STRCPY(s_pszTmpCNAPBuf, pCallStack->Stack[nCurrIndex].aName);	  
         MakeShortString(s_pszTmpCNAPBuf, MAX_CHAR_NUM_IN_DETAIL_INFO);		   
		   
         
		 if(STRCMP(pCallStack->Stack[nCurrIndex].aName, s_pszPBookName) == 0)
         {
			/* 두 이름이 동일할 경우 */
            EQC_SetStaticText(EQS_GetNthIOC(CH_NAME_CNAP), (BYTE*)"");
			sprintf((char*)s_pszBuf, "%s%s", SF_CNAP, pszBuf);
            EQC_SetStaticText(EQS_GetNthIOC(CH_NAME), s_pszBuf);
         }
         else
         {
            /* 두 이름이 동일하지 않을 경우 */
			sprintf((char*)s_pszBuf, "%s%s",SF_PHONEBOOK, pszBuf);
            EQC_SetStaticText(EQS_GetNthIOC(CH_NAME), s_pszBuf);

			sprintf((char*)s_pszBuf, "%s%s",SF_CNAP, s_pszTmpCNAPBuf);
			EQC_SetStaticText(EQS_GetNthIOC(CH_NAME_CNAP), s_pszBuf);
         }
       }
       else  /* CNAP 이름 정보가 없을 경우 */
       {
		  EQC_SetStaticText(EQS_GetNthIOC(CH_NAME), (BYTE*)"");		   
	  
		  sprintf((char*)s_pszBuf, "%s%s", SF_PHONEBOOK, pszBuf);
		  EQC_SetStaticText(EQS_GetNthIOC(CH_NAME), s_pszBuf);
       }
    }
    else if((STRLEN(pCallStack->Stack[nCurrIndex].aName) != 0) && (pszBuf[0] == '\0'))            
    {     		
	  STRCPY(s_pszTmpCNAPBuf, pCallStack->Stack[nCurrIndex].aName);	  
      MakeShortString(s_pszTmpCNAPBuf, MAX_CHAR_NUM_IN_DETAIL_INFO);
        
      /* CNAP로 날아온 이름 정보만 있는 경우 */
      fNumberInfo = FALSE;
	  EQC_SetStaticText(EQS_GetNthIOC(CH_NAME_CNAP), (BYTE*)"");

      sprintf((char*)s_pszBuf, "%s%s", SF_CNAP, s_pszTmpCNAPBuf);
      EQC_SetStaticText(EQS_GetNthIOC(CH_NAME), s_pszBuf);            
    }
	else if((STRLEN(pCallStack->Stack[nCurrIndex].aName) == 0) && (pszBuf[0] == '\0'))
	{
      fNameInfo = FALSE;
      fNumberInfo = FALSE;

      EQC_SetStaticText(EQS_GetNthIOC(CH_NAME), (BYTE*)"");      
      EQC_SetStaticText(EQS_GetNthIOC(CH_NAME_CNAP), (BYTE*)"");
	}
	
    s_pszBuf[0] = '\0';

    //전화번호
    if(pCallStack->Stack[nCurrIndex].bNumDigits > 0)
    {
      if(Mode == FROM_DISPCLOCK)
      {        
        /* 번호정보가 있는 경우에 명령어 라인 세팅 */   
          if(fNumberInfo == TRUE)
          {
            EQS_SetWindowCML(CL_MENU_PBOOK_CANCEL); 
          }
          else
          {
            EQS_SetWindowCML(CL_MENU_SAVE_CANCEL); 
          }
      }
	  EQC_SetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), pCallStack->Stack[nCurrIndex].aDigits);
    }
    else
    {
      if(Mode == FROM_DISPCLOCK)
      {
        EQS_SetWindowCML(CL_MENU_NONE_CANCEL);
      }             
        
      if(fNameInfo == FALSE)
      {
        sprintf((char*)s_pszBuf, "%s%s", SF_BULB, SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_NO_NUMBER_INFO));
        EQC_SetStaticText(EQS_GetNthIOC(CH_NAME), s_pszBuf);
      }
	  EQC_SetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), (BYTE*)"");
    }  
    
	pszBuf[0] = '\0';
    bMode = pCallStack->Stack[nCurrIndex].bStatus;

    switch(bMode)
    {
    case CT_CALLED_BY:
      sprintf((CHAR*)pszBuf, "%c%c%s", SPECIAL_DIGIT, SPECIAL_DIGIT_1+nLogicalIndex,
                                       SKY_GET_TOKEN(TKN_CALL_CALLEDBY));
      break;

    case CT_CALL_TO:
      sprintf((CHAR*)pszBuf, "%c%c%s", SPECIAL_DIGIT,SPECIAL_DIGIT_1+nLogicalIndex,
                                       SKY_GET_TOKEN(TKN_CALL_CALLTO));
      break;

    case CT_MISSED_CALL:
      sprintf((CHAR*)pszBuf, "%c%c%s", SPECIAL_DIGIT,SPECIAL_DIGIT_1+nLogicalIndex,
                                       SKY_GET_TOKEN(TKN_CALL_MISSEDCALL));
      break;

    case CT_SMS_MO:
      sprintf((CHAR*)pszBuf, "%c%c%s", SPECIAL_DIGIT,SPECIAL_DIGIT_1+nLogicalIndex,
                                       SKY_GET_TOKEN(TKN_CALL_SMSMO));
      break;

    case CT_SMS_MT:
      sprintf((CHAR*)pszBuf, "%c%c%s", SPECIAL_DIGIT,SPECIAL_DIGIT_1+nLogicalIndex,
                                       SKY_GET_TOKEN(TKN_CALL_SMSMT));
      break;

    case CT_CALLED_BY_INPROG:
     sprintf((CHAR*)pszBuf, "%c%c%s", SPECIAL_DIGIT,SPECIAL_DIGIT_1+nLogicalIndex,
                                      SKY_GET_TOKEN(TKN_CALL_CALLTO_INPROG));
      break;
    }
    STRCPY(szTitleHeader, pszBuf);
    EQS_SetSWindowHeaderStr((BYTE*)szTitleHeader);
    pszBuf[0] = '\0';
    EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);   


    if((pCallStack->Stack[nCurrIndex].bNumDigits == 0) && (fNameInfo == FALSE))
       return FALSE;
    else
       return TRUE;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
UINT8 g_nCallStackChanged = 0;


UINT8 SKY_GET_CALL_STACK_CHANGED(void)
{
    return g_nCallStackChanged;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKY_SET_CALL_STACK_CHANGED()
**
** Description: 
**            
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_SET_CALL_STACK_CHANGED(UINT8 nCallStackChanged)
{
  g_nCallStackChanged = nCallStackChanged;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_DisplayDetailInfo(SKY_EVENT_T* pEvent)
{    
    STATIC  UINT8    nStackIndex;
    STATIC  DWORD    nLogicalIndex;
    HCONTROL         hControl;
    STATIC  HCONTROL hCtrlDeleteOne, hCtrlDeleteAll, hMainPopup;
    STATIC  HCONTROL hCtrlSuccessDeleteALL, hCtrlSuccessDeleteOne;
    STATIC  BYTE     szTitleHeader[20];
    BYTE             szPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit, nListHandle, nCurrIndex, i, j, nSize, nLength, nRealPhoneNumber;
	INT16            Index;
	STATIC  INT16    nPBookAddress;
    CALLSTACK_T*     pCallStack;

      
    switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START:        
        EQS_GetDC(TRUE);
        STRCPY(szTitleHeader, SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_TITLE));
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_MENU_PBOOK_CANCEL, //CL_MENU_PBOOK_DELETE, 
            szTitleHeader, 
            (WA_SHOW_CML|WA_SHOW_HEADER));
        SKY_SET_CALL_STACK_CHANGED(0);
        sInitCurrentCallInfoDisp();
        break;

    case SE_APP_END:
        SKY_SET_CALL_STACK_CHANGED(0);
		EQS_ReleaseDC();
        break;

    case SE_APP_DRAW :
		/*넘버 에디터 편집을 위해 TRUE로 만들어 준다.*/
        EQC_SET_SELF_DRAW(EQS_GetNthIOC(CH_NUMBER), TRUE);
        //sDrawGeneralFrameInCallHistory();
        break;

    case SE_APP_RESUME:
        if(SKY_GET_CALL_STACK_CHANGED() && (GetReceivedCallStack() == TRUE))
        {            
            UINT8 nCurrentData, nDeleteData;
            
            if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
            {
              EQS_DeleteControl(hControl);
            }

            EQS_SetIOCActive(EQS_GetNthIOC(CH_NUMBER));
            hControl    = EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER));
            nListHandle = EQC_GetScrollListHandle(hControl);
            pCallStack  = GetCSKData();

            nCurrentData = EQC_GetScrollMaxDataLine(hControl);
            nDeleteData = nCurrentData + SKY_GET_CALL_STACK_CHANGED();

            if(nDeleteData > NUM_CALL_HISTORY)
            {
                nDeleteData = nDeleteData - NUM_CALL_HISTORY;

                while( nDeleteData > 0 )
                {
                    RemoveListItem(nListHandle, nCurrentData-1);
                    nCurrentData--;
                    nDeleteData--;
                }
                EQC_SetScrollMaxDataLine(hControl, nCurrentData);
            }

            for(Index=SKY_GET_CALL_STACK_CHANGED()-1; Index >=0 ; Index--)
            {
                InsertList(nListHandle,0, pCallStack->Index.aIndex[Index], EQS_NA);
            }
            EQC_SetScrollMaxDataLine(hControl, nCurrentData+SKY_GET_CALL_STACK_CHANGED());

            nStackIndex   = sGetNumberOfCurrentCallHistory();
            Index = GetListValue(nListHandle, 0);
            nLogicalIndex = 0;
            sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);
            SKY_SET_CALL_STACK_CHANGED(0);
        }
        else
        {
         EQS_SetIOCActive(EQS_GetNthIOC(CH_NUMBER));
		 nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
         Index = GetListValue(nListHandle, nLogicalIndex);
         sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);
        }
		break;

    case SE_IO_SCROLL_LIST_SELECT :    // This event comes from 
                                       // SKA_MainCallList(SKY_EVENT_T *pEvent) 
        nStackIndex   = sGetNumberOfCurrentCallHistory();
        Index         = pEvent->wParam;
        EQC_SET_USERDATA(EQS_GetNthIOC(CH_NUMBER), pEvent->dwParam);
		nLogicalIndex = EQC_GetScrollLogicalIndex(pEvent->dwParam);
        sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);
        break;

    case SE_CALL_STACK_CHANGED:        // 착신이력을 보고 있을 때, SMS가 착신되면 사용자가
                                       // 보고 있는 착신이력의 내용과 단말기 내부에서 유지되는
                                       // 착신이력이 달라지게 된다. 이 때 SE_CALL_STACK_CHANGED
                                       // 라는 이벤트를 이용해서 최신정보에 기반해서 착신이력을
                                       // 새로 그려준다.
        if(GetReceivedCallStack() == TRUE)
        {
            if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
            {
              EQS_DeleteControl(hControl);
            }
            hControl    = EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER));
            nListHandle = EQC_GetScrollListHandle(hControl);
            pCallStack  = GetCSKData();

            if(EQC_GetScrollMaxDataLine(hControl) < NUM_CALL_HISTORY)
            {
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
                EQC_SetScrollMaxDataLine(hControl, EQC_GetScrollMaxDataLine(hControl)+1);
            }
            else
            {
                RemoveListItem(nListHandle, NUM_CALL_HISTORY-1);
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
            }

            nStackIndex   = sGetNumberOfCurrentCallHistory();
            Index = GetListValue(nListHandle, 0);
            nLogicalIndex = 0;
            sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);
        }
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_SEND :

		if(EQS_GetNthIOC(CH_NUMBER) != INVALID_CONTROL_HANDLE )
		{
			nNumDigit = EQC_GetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), szPhoneNumber);   
			if(szPhoneNumber[0] != '\0')
			{             
			  SET_NUMEDIT_BUFFER(szPhoneNumber, nNumDigit);
			  SET_NUMEDIT_POS(nNumDigit); 	  

			  if(nNumDigit == 1)
			  { 
				nRealPhoneNumber = szPhoneNumber[0] - '0';   
				if( nRealPhoneNumber > 0 && nRealPhoneNumber < 10 )
				{
					if(SKY_IS_PHONE_BOOK_LOCK())
					{
					   EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
					}
					else
					{
						SpeedDial( nRealPhoneNumber ); 
					}
				}
				else if(szPhoneNumber[0] == '#' || szPhoneNumber[0] == '*' || szPhoneNumber[0] == '0')
				{
					EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				}
			  }
			  else if(nNumDigit == 2)
			  {
				if( isdigit(szPhoneNumber[0]) && isdigit(szPhoneNumber[1]) )
                {
				    nRealPhoneNumber = (szPhoneNumber[0] -'0')*10 + (szPhoneNumber[1]-'0'); 
				    if(SKY_IS_PHONE_BOOK_LOCK() && (nRealPhoneNumber > 0 && nRealPhoneNumber < 100) )
				    {
					    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
				    }
				    else if(nRealPhoneNumber > 0 && nRealPhoneNumber < 100)         
				    {
					    SpeedDial( nRealPhoneNumber ); 
				    }
				    else
				    {
					    EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				    }
                }
                else
                {
                    EQS_START_SIBLING(SKA_StartOrigination, FALSE);
                }
			  }
			  else
			  {
				 EQS_START_SIBLING(SKA_StartOrigination, FALSE);
			  }
			}
		}
		break;      


    case SE_IO_NEDIT_PRESS_LONGKEY:
        if(SKY_IS_PHONE_BOOK_LOCK() && (pEvent->wParam == SE_IO_NEDIT_PRESS_LONGKEY))
        {
            SpeedDial( pEvent->dwParam );        
            break;
        }

        EQC_GetNumberEditValue(pEvent->wParam, szPhoneNumber);
        nNumDigit = STRLEN(szPhoneNumber);
        
        if( nNumDigit == 1 )
        {                                   
            nRealPhoneNumber = szPhoneNumber[0] - '0';			
        }
        else if( nNumDigit == 2)
        {
            nRealPhoneNumber = (szPhoneNumber[0] -'0')*10 + (szPhoneNumber[1]-'0');
        }
        else
		{
			nRealPhoneNumber = 0;
		}

		if( nRealPhoneNumber < 1 || nRealPhoneNumber > MAX_SDIAL_NUMBER )
			break;
           
        if(SKY_IS_PHONE_BOOK_LOCK())
        {
           EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_IO_NEDIT_PRESS_LONGKEY, nRealPhoneNumber); 
        }else
            SpeedDial( nRealPhoneNumber );        
            
        break;

       
    case SE_IO_NEDIT_PRESS_UPKEY:
    case SE_KEY_UP:
         if(nLogicalIndex > 0)    nLogicalIndex--;
         else                     nLogicalIndex = nStackIndex;
  
         nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
         Index = GetListValue(nListHandle, nLogicalIndex);
         sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);
        break;

    case SE_IO_NEDIT_PRESS_DOWNKEY: 
    case SE_KEY_DOWN:
         if(nLogicalIndex < nStackIndex)    nLogicalIndex++;
         else                               nLogicalIndex = 0;

         nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
         Index = GetListValue(nListHandle, nLogicalIndex);
         sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);        
        break;

    case SE_SOFTKEY_CANCEL:
    case SE_IO_NEDIT_NODIGIT:
    case SE_KEY_CLEAR:
         EQS_END_PARAM(EQS_NA, nLogicalIndex);
        break;

    case SE_IO_NEDIT_DONE:
    case SE_SOFTKEY_PBOOK:
    case SE_SOFTKEY_SAVE:
        pCallStack  = GetCSKData();
		nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
		Index = GetListValue(nListHandle, nLogicalIndex);
        if(pCallStack->Stack[Index].bNumDigits > 0)
        {
            if(SKY_IS_PHONE_BOOK_LOCK())
            {
                if(pEvent->wParam != BOOKCODE_VIEW_IN_LIST)
                {
                    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, 
                                        BOOKCODE_VIEW_IN_LIST, EQS_NA);
                    break;
                }
            
            }

            if(IsInPhoneBook(Index, szPhoneNumber, &nPBookAddress))
	        {
                EQS_START_CHILD_EVENT(SKA_ViewBookContent, SE_APP_START, 
                                    nPBookAddress, BFT_NON_FUNCTION_E);
	        }
	        else
	        {
                EQS_START_CHILD_EVENT(SKA_SavePhoneNumber, SE_APP_START,\
  				                    EQS_GetNthIOC(CH_NUMBER), EQS_NA);
	        }
        }
        else
        {
            EQS_SetIOCActive(EQS_GetNthIOC(CH_NUMBER));
        }
		break;

    case SE_IO_NEDIT_CANCEL:
    case SE_SOFTKEY_MENU:
        pCallStack  = GetCSKData();
		nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
		Index = GetListValue(nListHandle, nLogicalIndex);
        
        if(pCallStack->Stack[Index].bNumDigits > 0)
        {
          BOOL      paActive[NUM_CURRENT_CALLHISTORY_HOTMENU] = {TRUE, TRUE, TRUE};
          hMainPopup = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paCurrentCallInfoHotMenu, NUM_CURRENT_CALLHISTORY_HOTMENU, 
                                            0, paActive);            
          EQC_EnablePopupMenuItem(hMainPopup, TKN_CALL_HISTORY_SMS);
          EQC_EnablePopupMenuItem(hMainPopup, TKN_VMEMO_DELETE_ONE);
          EQC_EnablePopupMenuItem(hMainPopup, TKN_VMEMO_DELETE);
        }
        else
        {
          BOOL      paActive[NUM_CURRENT_CALLHISTORY_HOTMENU] = {FALSE, TRUE, TRUE};
          hMainPopup = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paCurrentCallInfoHotMenu, NUM_CURRENT_CALLHISTORY_HOTMENU, 
                                            0, paActive);            
          EQC_EnablePopupMenuItem(hMainPopup, TKN_VMEMO_DELETE_ONE);
          EQC_EnablePopupMenuItem(hMainPopup, TKN_VMEMO_DELETE);
        }
        break;


    case SE_SOFTKEY_DELETE:
         hCtrlDeleteOne = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE), MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_POPUP_CANCEL:
         EQS_SetIOCActive(EQS_GetNthIOC(CH_NUMBER));
        break;

    case SE_IO_POPUP_SELECT :
		if(pEvent->wParam == hMainPopup) /* soft key를 이용해 활성화된 pop up 메뉴 */
		{
			switch(pEvent->dwParam)
			{
			 case CH_TO_SMS:
			   memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
			   nListHandle   = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
			   Index = GetListValue(nListHandle, nLogicalIndex);
			   
               //pCallStack    = GetCSKData();
			   //nLength= pCallStack->Stack[Index].bNumDigits;        
			   //pCallStack->Stack[Index].aDigits[nLength] = '\0';
			   //STRCPY(g_UIMOBuffer.pszRcvNumber, pCallStack->Stack[Index].aDigits);
               EQC_GetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), g_UIMOBuffer.pszRcvNumber);
			   g_UIMOBuffer.nRcvNumberSize = STRLEN(g_UIMOBuffer.pszRcvNumber);
               if(SKY_IS_SMS_LOCK())
                   EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_IN_DETAIL_LIST, EQS_NA);
               else
			        EQS_START_CHILD(SKA_SMSEditMessage);
			   break;

             case CH_DELETE_ONE:
               hCtrlDeleteOne = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE), MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO, MBT_CONFIRM);
               break;

			 case CH_DELETE_ALL:
#ifdef FEATURE_CALL_STACK_SEPERATED    
                if(GetReceivedCallStack())
                {
                  hCtrlDeleteAll = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE_CALLED), 
                                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                }
                else
                {
                  hCtrlDeleteAll = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE_CALLTO), 
                                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM); 
                }
#else
				hCtrlDeleteAll = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASEALL), 
                                                 MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
#endif// FEATURE_CALL_STACK_SEPERATED 
			   break;
			}
		}
       break;

    case SE_IO_MESGBOX_END :
		/* 착/발신 이력의 상세정보에서 단축다이얼을 이용한 발신을 시도할 때       */
		/* 단축다이얼이 지정되지 않았다는 메시지 박스가 뜬 뒤에는 Number Editor의 */
		/* 커서 위치가 맨 앞으로 가는 현상이 발생한다. 이를 방지하기 위해 강제로  */
		/* Active 시킨다.                                                         */
		EQS_SetIOCActive(EQS_GetNthIOC(CH_NUMBER));
        if(pEvent->wParam == hCtrlSuccessDeleteOne)
        {
           nSize = EQC_GetScrollMaxDataLine(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));

           if(nSize == 0)  /* 삭제 후 스크롤 리스트에 데이터가 하나도 없는 경우 */
           {               /* 빠져 나간다.                                      */

#ifdef FEATURE_CALL_STACK_SEPERATED    
                if(GetReceivedCallStack() == FALSE)
#endif// FEATURE_CALL_STACK_SEPERATED
                {
                    InitializeLastDialInfo();
                }
                EQS_END_TO(EQS_POP_TO_ROOT);
           }
           else
           {
             nStackIndex--;
             nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
             if(nStackIndex < nLogicalIndex) /* 맨 마지막 데이터를 지웠을 경우 */
             {
                nLogicalIndex--;
                sUpdateDispCurrentCallInfo(GetListValue(nListHandle, nLogicalIndex), 
                                           nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);
             }
             else
             {
                sUpdateDispCurrentCallInfo(GetListValue(nListHandle, nLogicalIndex), 
                                           nLogicalIndex, szTitleHeader, FROM_DISPCLOCK);
             }
           }
//#ifdef FEATURE_CALL_STACK_SEPERATED    
//           if(GetReceivedCallStack() == FALSE)
//#endif// FEATURE_CALL_STACK_SEPERATED
//           {
//                UpdateLastDialInfoAsDeleteOneItem();
//           }
        }
        else if(pEvent->wParam == hCtrlSuccessDeleteALL)
        {
                EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlDeleteOne)
        {
           nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
           pCallStack  = GetCSKData();
           Index = GetListValue(nListHandle, nLogicalIndex);
           for(i = 0 ; i < NUM_CALL_HISTORY; i++)
           {
              if(pCallStack->Index.aIndex[i] == Index)
              {
                 for(j = i; j < NUM_CALL_HISTORY-1; j++)
                 {
                    pCallStack->Index.aIndex[j] = pCallStack->Index.aIndex[j+1];
                 }

                 pCallStack->Index.aIndex[j] = INIT_CALLSTACK_INDEX;
                 pCallStack->UsableList.bCurrentPos++;
                 pCallStack->UsableList.aUsableIndex[pCallStack->UsableList.bCurrentPos] = Index;                 
                 break;
              }
           } 
#ifdef FEATURE_CALL_STACK_SEPERATED    
           if(GetReceivedCallStack())
           {
              WriteCallStackReceived(); 
           }
           else
           {
              WriteCallStack();   
           }
#else
           WriteCallStack();   
#endif// FEATURE_CALL_STACK_SEPERATED
           RemoveListItem(nListHandle,nLogicalIndex);

           nCurrIndex = EQC_DeleteScrollData(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)), nLogicalIndex);/* 화면에 처음 뿌려질 데이터의 인덱스 */
           nSize = EQC_GetScrollMaxDataLine(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
           if( nSize > NUM_CALL_SCROLL )  nSize = NUM_CALL_SCROLL;

           for( i=0; i< nSize; i++)
           {
             Index = GetListValue(nListHandle, (i+nCurrIndex));
             sUpdateCallHistoryList(EQC_GetScrollData(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)), i), Index);
           }
          hCtrlSuccessDeleteOne = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_SUCCESS_ERASE), 
                                                 1, MBT_NONE, MBT_SUCCESS); 
       }
       else if(pEvent->wParam == hCtrlDeleteAll)  /* Call Stack에서 모든 아이템을 지울 경우 */
       {          
#ifdef FEATURE_CALL_STACK_SEPERATED    
           if(GetReceivedCallStack() == FALSE)
#endif// FEATURE_CALL_STACK_SEPERATED
           {           
                InitializeLastDialInfo();
           }
#ifdef FEATURE_CALL_STACK_SEPERATED    
         if(GetReceivedCallStack())
         {
           DeleteAllCallStackReceived(); 
         }
         else
         {
           DeleteAllCallStack();  
         }
#else
        DeleteAllCallStack();   
#endif// FEATURE_CALL_STACK_SEPERATED
         hCtrlSuccessDeleteALL = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_SUCCESS_ERASE), 
                                                 1, MBT_NONE, MBT_SUCCESS); 

//       EQS_END_TO(EQS_POP_TO_ROOT);
       }
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_KEY_MANNERS:
		break;

    default : /* no exit until OK or END or CLR or PWR pressed or earpiece */
        EQS_DEF_EVENT_PROC(pEvent);
		break ;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_DisplayDetailInfoForSMS(SKY_EVENT_T* pEvent)
{    
    STATIC UINT8     nStackIndex;
    STATIC DWORD     nLogicalIndex;
    STATIC  BYTE     szTitleHeader[20];
    BYTE             szPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit, nListHandle, Index, nRealPhoneNumber;
    HCONTROL         hControl;
    CALLSTACK_T*     pCallStack;
//  READPBOOK_E      PBResult;
      
    switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START:        
        EQS_GetDC(TRUE);
        STRCPY(szTitleHeader, SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_TITLE));
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
			CL_NONE_TOSMS_CANCEL,
            szTitleHeader, 
            (WA_SHOW_CML|WA_SHOW_HEADER));
        SKY_SET_CALL_STACK_CHANGED(0);
        sInitCurrentCallInfoDisp();
        break;

    case SE_APP_END:
        SKY_SET_CALL_STACK_CHANGED(0);
		EQS_ReleaseDC();
        break;

    case SE_APP_DRAW :
        /*넘버 에디터 편집을 위해 TRUE로 만들어 준다.*/
        EQC_SET_SELF_DRAW(EQS_GetNthIOC(CH_NUMBER), TRUE);
        //sDrawGeneralFrameInCallHistory();
        break;

    case SE_CALL_STACK_CHANGED:
        if(GetReceivedCallStack() == TRUE)
        {
            if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
            {
              EQS_DeleteControl(hControl);
            }
            hControl    = EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER));
            nListHandle = EQC_GetScrollListHandle(hControl);
            pCallStack  = GetCSKData();

            if(EQC_GetScrollMaxDataLine(hControl) < NUM_CALL_HISTORY)
            {
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
                EQC_SetScrollMaxDataLine(hControl, EQC_GetScrollMaxDataLine(hControl)+1);
            }
            else
            {
                RemoveListItem(nListHandle, NUM_CALL_HISTORY-1);
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
            }

            nStackIndex   = sGetNumberOfCurrentCallHistory();
            Index = GetListValue(nListHandle, 0);
            nLogicalIndex = 0;
            sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_SMS);
        }
        break;

    case SE_IO_SCROLL_LIST_SELECT :    // This event comes from 
        nStackIndex   = sGetNumberOfCurrentCallHistory();
        Index         = pEvent->wParam;
        EQC_SET_USERDATA(EQS_GetNthIOC(CH_NUMBER), pEvent->dwParam);
		nLogicalIndex = EQC_GetScrollLogicalIndex(pEvent->dwParam);
        sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_SMS);
        break;

    case SE_IO_NEDIT_DONE:
    case SE_SOFTKEY_TOSMS:
     	nListHandle   = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
		Index = GetListValue(nListHandle, nLogicalIndex);
        SetCallStackIndex(Index);
        memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
        EQC_GetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), g_UIMOBuffer.pszRcvNumber);
        g_UIMOBuffer.nRcvNumberSize = STRLEN(g_UIMOBuffer.pszRcvNumber);
        EQS_END();
        break;


    case SE_SOFTKEY_CANCEL:
    case SE_IO_NEDIT_NODIGIT:
    case SE_KEY_CLEAR:
         EQS_END_PARAM(EQS_NA, nLogicalIndex);
        break;


    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_SEND :
				if(EQS_GetNthIOC(CH_NUMBER) != INVALID_CONTROL_HANDLE )
		{
			nNumDigit = EQC_GetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), szPhoneNumber);   
			if(szPhoneNumber[0] != '\0')
			{             
			  SET_NUMEDIT_BUFFER(szPhoneNumber, nNumDigit);
			  SET_NUMEDIT_POS(nNumDigit); 	  

			  if(nNumDigit == 1)
			  { 
				nRealPhoneNumber = szPhoneNumber[0] - '0';   
				if( nRealPhoneNumber > 0 && nRealPhoneNumber < 10 )
				{
					if(SKY_IS_PHONE_BOOK_LOCK())
					{
					   EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
					}
					else
					{
						SpeedDial( nRealPhoneNumber ); 
					}
				}
				else if(szPhoneNumber[0] == '#' || szPhoneNumber[0] == '*' || szPhoneNumber[0] == '0')
				{
					EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				}
			  }
			  else if(nNumDigit == 2)
			  {
                 if( isdigit(szPhoneNumber[0]) && isdigit(szPhoneNumber[1]) )
                 {
				    nRealPhoneNumber = (szPhoneNumber[0] -'0')*10 + (szPhoneNumber[1]-'0');     
				    if(SKY_IS_PHONE_BOOK_LOCK() && (nRealPhoneNumber > 0 && nRealPhoneNumber < 100))
				    {
					    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
				    }
				    else if(nRealPhoneNumber > 0 && nRealPhoneNumber < 100)         
				    {
					    SpeedDial( nRealPhoneNumber ); 
				    }
				    else
				    {
					    EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				    }
                 }
                 else
                 {
                     EQS_START_SIBLING(SKA_StartOrigination, FALSE);
                 }
			  }
			  else
			  {
				 EQS_START_SIBLING(SKA_StartOrigination, FALSE);
			  }
			}
		}
		break;       

		
    case SE_IO_NEDIT_PRESS_LONGKEY:
        if(SKY_IS_PHONE_BOOK_LOCK() && (pEvent->wParam == SE_IO_NEDIT_PRESS_LONGKEY))
        {
            SpeedDial( pEvent->dwParam );        
            break;
        }

        EQC_GetNumberEditValue(pEvent->wParam, szPhoneNumber);
        nNumDigit = STRLEN(szPhoneNumber);
        
        if( nNumDigit == 1 )
        {                                   
            nRealPhoneNumber = szPhoneNumber[0] - '0';			
        }
        else if( nNumDigit == 2)
        {
            nRealPhoneNumber = (szPhoneNumber[0] -'0')*10 + (szPhoneNumber[1]-'0');
        }
        else
		{
			nRealPhoneNumber = 0;
		}

		if( nRealPhoneNumber < 1 || nRealPhoneNumber > MAX_SDIAL_NUMBER )
			break;
           
        if(SKY_IS_PHONE_BOOK_LOCK())
        {
           EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_IO_NEDIT_PRESS_LONGKEY, nRealPhoneNumber); 
        }else
            SpeedDial( nRealPhoneNumber );        
            
        break;
    
    case SE_IO_NEDIT_PRESS_UPKEY:        
    case SE_KEY_UP:
         if(nLogicalIndex > 0)    nLogicalIndex--;
         else                     nLogicalIndex = nStackIndex;
  
         nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
         Index = GetListValue(nListHandle, nLogicalIndex);
         sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_SMS);
        break;

    case SE_IO_NEDIT_PRESS_DOWNKEY:
    case SE_KEY_DOWN:
         if(nLogicalIndex < nStackIndex)    nLogicalIndex++;
         else                               nLogicalIndex = 0;

         nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
         Index = GetListValue(nListHandle, nLogicalIndex);
         sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_SMS);        
        break;

	/* 착발신 이력에서 단축다이얼로 발신을 시도할 때, 단축다이얼이 없는 경우에는 */
	/* 지정되지 않은 단축다이얼이라는 메시지 박스가 뜬 뒤                        */
	/* Number Editor의 포커스가 맨 앞으로 오는 경우가 있다. 이를 방지하기 위해   */
	/* 사용됨                                                                    */
    case SE_IO_MESGBOX_END:
		EQS_SetIOCActive(EQS_GetNthIOC(CH_NUMBER));
		break;

    case SE_KEY_MANNERS:
		break;


    default : /* no exit until OK or END or CLR or PWR pressed or earpiece */
        EQS_DEF_EVENT_PROC(pEvent);
		break ;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#ifdef FEATURE_SKTT_PM
LOCAL VOID SKY_API sSKA_DisplayDetailInfoForPhotoMail(SKY_EVENT_T* pEvent)
{    
    STATIC UINT8     nStackIndex;
    STATIC DWORD     nLogicalIndex;
    STATIC  BYTE     szTitleHeader[20];
    BYTE             szPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit, nListHandle, Index, nRealPhoneNumber;
    HCONTROL         hControl;
    CALLSTACK_T*     pCallStack;
//  READPBOOK_E      PBResult;
      
    switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START:        
        EQS_GetDC(TRUE);
        STRCPY(szTitleHeader, SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_TITLE));
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
			CL_NONE_SELECT_CANCEL,
            szTitleHeader, 
            (WA_SHOW_CML|WA_SHOW_HEADER));
        sInitCurrentCallInfoDisp();
        SKY_SET_CALL_STACK_CHANGED(0);
        break;

    case SE_APP_END:
        SKY_SET_CALL_STACK_CHANGED(0);
		EQS_ReleaseDC();
        break;

    case SE_APP_DRAW :
        /*넘버 에디터 편집을 위해 TRUE로 만들어 준다.*/
        EQC_SET_SELF_DRAW(EQS_GetNthIOC(CH_NUMBER), TRUE);
        //sDrawGeneralFrameInCallHistory();
        break;

    case SE_CALL_STACK_CHANGED:
        if(GetReceivedCallStack() == TRUE)
        {
            if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
            {
              EQS_DeleteControl(hControl);
            }
            hControl    = EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER));
            nListHandle = EQC_GetScrollListHandle(hControl);
            pCallStack  = GetCSKData();

            if(EQC_GetScrollMaxDataLine(hControl) < NUM_CALL_HISTORY)
            {
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
                EQC_SetScrollMaxDataLine(hControl, EQC_GetScrollMaxDataLine(hControl)+1);
            }
            else
            {
                RemoveListItem(nListHandle, NUM_CALL_HISTORY-1);
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
            }

            nStackIndex   = sGetNumberOfCurrentCallHistory();
            Index = GetListValue(nListHandle, 0);
            nLogicalIndex = 0;
            sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_PHOTO_MAIL);
        }
        break;

    case SE_IO_SCROLL_LIST_SELECT :    // This event comes from 
                                        // SKA_MainCallList(SKY_EVENT_T *pEvent) 
        nStackIndex   = sGetNumberOfCurrentCallHistory();
        Index         = pEvent->wParam;
		EQC_SET_USERDATA(EQS_GetNthIOC(CH_NUMBER), pEvent->dwParam);
		nLogicalIndex = EQC_GetScrollLogicalIndex(pEvent->dwParam);
        sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_PHOTO_MAIL);
        break;


    case SE_IO_NEDIT_DONE:
    case SE_SOFTKEY_SELECT:
        nListHandle   = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
		Index = GetListValue(nListHandle, nLogicalIndex);
        SetCallStackIndex(Index);
        /* This Buffer is defined in ska_vm.h */
        g_PhoneNumBuf[0] = '\0';
        EQC_GetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), g_PhoneNumBuf);
        EQS_END();
        break;


    case SE_SOFTKEY_CANCEL:
    case SE_IO_NEDIT_NODIGIT:
    case SE_KEY_CLEAR:
         EQS_END_PARAM(EQS_NA, nLogicalIndex);
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_SEND :
				if(EQS_GetNthIOC(CH_NUMBER) != INVALID_CONTROL_HANDLE )
		{
			nNumDigit = EQC_GetNumberEditValue(EQS_GetNthIOC(CH_NUMBER), szPhoneNumber);   
			if(szPhoneNumber[0] != '\0')
			{             
			  SET_NUMEDIT_BUFFER(szPhoneNumber, nNumDigit);
			  SET_NUMEDIT_POS(nNumDigit); 	  

			  if(nNumDigit == 1)
			  { 
				nRealPhoneNumber = szPhoneNumber[0] - '0';   
				if( nRealPhoneNumber > 0 && nRealPhoneNumber < 10 )
				{
					if(SKY_IS_PHONE_BOOK_LOCK())
					{
					   EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
					}
					else
					{
						SpeedDial( nRealPhoneNumber ); 
					}
				}
				else if(szPhoneNumber[0] == '#' || szPhoneNumber[0] == '*' || szPhoneNumber[0] == '0')
				{
					EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				}
			  }
			  else if(nNumDigit == 2)
			  {
                  if( isdigit(szPhoneNumber[0]) && isdigit(szPhoneNumber[1]) )
                  {
				    nRealPhoneNumber = (szPhoneNumber[0] -'0')*10 + (szPhoneNumber[1]-'0');     
				    if(SKY_IS_PHONE_BOOK_LOCK() && (nRealPhoneNumber > 0 && nRealPhoneNumber < 100))
				    {
					    EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
				    }
				    else if(nRealPhoneNumber > 0 && nRealPhoneNumber < 100)         
				    {
					    SpeedDial( nRealPhoneNumber ); 
				    }
				    else
				    {
					    EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				    }
                  }
                  else
                  {
                     EQS_START_SIBLING(SKA_StartOrigination, FALSE);
                  }
			  }
			  else
			  {
				 EQS_START_SIBLING(SKA_StartOrigination, FALSE);
			  }
			}
		}
		break;        

    case SE_IO_NEDIT_PRESS_LONGKEY:
        if(SKY_IS_PHONE_BOOK_LOCK() && (pEvent->wParam == SE_IO_NEDIT_PRESS_LONGKEY))
        {
            SpeedDial( pEvent->dwParam );        
            break;
        }

        EQC_GetNumberEditValue(pEvent->wParam, szPhoneNumber);
        nNumDigit = STRLEN(szPhoneNumber);
        
        if( nNumDigit == 1 )
        {                                   
            nRealPhoneNumber = szPhoneNumber[0] - '0';			
        }
        else if( nNumDigit == 2)
        {
            nRealPhoneNumber = (szPhoneNumber[0] -'0')*10 + (szPhoneNumber[1]-'0');
        }
        else
		{
			nRealPhoneNumber = 0;
		}

		if( nRealPhoneNumber < 1 || nRealPhoneNumber > MAX_SDIAL_NUMBER )
			break;
           
        if(SKY_IS_PHONE_BOOK_LOCK())
        {
           EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_IO_NEDIT_PRESS_LONGKEY, nRealPhoneNumber); 
        }else
            SpeedDial( nRealPhoneNumber );        
            
        break;


    case SE_IO_NEDIT_PRESS_UPKEY:        
    case SE_KEY_UP:
         if(nLogicalIndex > 0)    nLogicalIndex--;
         else                     nLogicalIndex = nStackIndex;
  
         nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
         Index = GetListValue(nListHandle, nLogicalIndex);
         sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_PHOTO_MAIL);
        break;

    case SE_IO_NEDIT_PRESS_DOWNKEY:        
    case SE_KEY_DOWN:
         if(nLogicalIndex < nStackIndex)    nLogicalIndex++;
         else                               nLogicalIndex = 0;

         nListHandle = EQC_GetScrollListHandle(EQC_GET_USERDATA(EQS_GetNthIOC(CH_NUMBER)));
         Index = GetListValue(nListHandle, nLogicalIndex);
         sUpdateDispCurrentCallInfo(Index, nLogicalIndex, szTitleHeader, FROM_PHOTO_MAIL);        
        break;

	case SE_IO_MESGBOX_END:
		EQS_SetIOCActive(EQS_GetNthIOC(CH_NUMBER));
		break;

    case SE_KEY_MANNERS:
		break;

    default : /* no exit until OK or END or CLR or PWR pressed or earpiece */
        EQS_DEF_EVENT_PROC(pEvent);
		break ;
    }
}
#endif// FEATURE_SKTT_PM





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitCallList
**
** Description: 
**     Display calls list on LCD
**     Inform time, phone number, name, mode(call to, called by, missed)
**     
** Input:
**
**
** Output:
**     
**
** Return value:
**     
**
** Side effects:
**     the content of sIndexNameBuf, sStatusTimeBuf, sNumberBuf
**
** ---------------------------------------------------------------------------
*/
UINT8 sInitCallList()
{
  BYTE            szTmpBuf[MAX_DIAL_DIGIT_LENGTH+1];
  UINT8           nLoop, nStackIndex;              /* 물리적으로 작은 버퍼를 이용하여 논리적으로 */
  CALLSTACK_T*    pCallStack;                      /* 많은 데이터를 보여주려고 할 경우           */
  UINT8           ListHandle, Index, nSize;
  BYTE            szBuf[NUM_CALL_HISTORY][MAX_DIAL_DIGIT_LENGTH+1];
     

    
  pCallStack = GetCSKData();
  ListHandle = CreateList(NUM_CALL_HISTORY);
  Index  = 0;

  for(nLoop = 0; nLoop < NUM_CALL_HISTORY; nLoop++)
  {  
    nStackIndex = pCallStack->Index.aIndex[nLoop];
    
    if(nStackIndex != INIT_CALLSTACK_INDEX)    //used
    {		      
		AddList(ListHandle,nStackIndex,EQS_NA);
	}
  }

  nSize = GetListSize(ListHandle);
  if( nSize > NUM_CALL_SCROLL )
	  nSize = NUM_CALL_SCROLL;

  for( nLoop = 0; nLoop < NUM_CALL_SCROLL; nLoop++)
	  szBuf[nLoop][0] = 0;

  FindCallerNameInBook(ListHandle, (BYTE*)szBuf, 33, 0, nSize);


  for( Index = 0; Index < NUM_CALL_SCROLL; Index++)
  { 
	    nStackIndex = GetListValue(ListHandle, Index);

		if(STRLEN(pCallStack->Stack[nStackIndex].aName) != 0)   /* CNAP로 날아온 이름이 있는 경우 */
		{
			STRCPY(szTmpBuf, pCallStack->Stack[nStackIndex].aName);
		}
		else
		{
			if( Index < nSize )
			{
				if(STRLEN(szBuf[Index]) == 0)
                {
				   if(pCallStack->Stack[nStackIndex].aDigits[0] != '\0')
                   {
                      //STRCPY(szTmpBuf, pCallStack->Stack[nStackIndex].aDigits);
                      MakeHypenedPhoneNumber(pCallStack->Stack[nStackIndex].aDigits, szTmpBuf);
                   }
                   else
                   {
                      STRCPY(szTmpBuf, SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_NO_NUMBER_INFO));
                   }
                }
				else
                {
				  STRCPY(szTmpBuf, szBuf[Index] );
                }
			}
		}

		switch(pCallStack->Stack[nStackIndex].bStatus)
		{
		case CT_CALLED_BY:
		  sprintf((CHAR*)szList[Index], (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                      SF_ALERT_CALL1_2,
												                      //SF_ALERT_CALL2_2,
												                      szTmpBuf);          

		  break;

		case CT_CALL_TO:

		  sprintf((CHAR*)szList[Index], (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                  SF_ORIGINATE_CALL1_2,
												                  //SF_ORIGINATE_CALL2_2,
												                  szTmpBuf);      
		  break;

		case CT_MISSED_CALL:

		  sprintf((CHAR*)szList[Index], (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                     SF_MISSED_CALL1_2,
												                     //SF_MISSED_CALL2_2,
												                     szTmpBuf);      

		  break;

		case CT_SMS_MO:

		  sprintf((CHAR*)szList[Index], (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                    SF_MESSAGE_SENT1_2,
												                    //SF_MESSAGE_SENT2_2,
												                    szTmpBuf);      

		  break;

		case CT_SMS_MT:

		  sprintf((CHAR*)szList[Index], (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                SF_MESSAGE_RECEIVED1_2,
												                //SF_MESSAGE_RECEIVED2_2,
												                szTmpBuf);      

		  break;

        case CT_CALLED_BY_INPROG:

          sprintf((CHAR*)szList[Index], (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                               SF_LINE_BUSY_RECEIPT1_2,
												               //SF_LINE_BUSY_RECEIPT2_2,
												               szTmpBuf);
		  break;

		}
		szCallHistory[Index] = szList[Index];	
  }
    
  if( Index < NUM_CALL_SCROLL)
  {
     for( nLoop = Index; nLoop <= NUM_CALL_SCROLL; nLoop++)
     {
       sprintf((CHAR*)szList[nLoop], "%s%s", SKY_GET_TOKEN(TKN_BLANK), SKY_GET_TOKEN(TKN_NULL));
       szCallHistory[nLoop] = szList[nLoop];
     }
  }
  szCallHistory[NUM_CALL_SCROLL] = szList[NUM_CALL_SCROLL];  

  return ListHandle;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
#define sMAX_NAME_LENGTH (MAX_DIAL_DIGIT_LENGTH+1)

void sUpdateCallHistoryList(BYTE* pszBuffer, INT16 nStackIndex)
{
	CALLSTACK_T*    pCallStack;
	BYTE            szTmpBuffer[MAX_DIAL_DIGIT_LENGTH+1];
    BYTE            szPBookName[sMAX_NAME_LENGTH];    /* 전화번호종류를 나타내는 special font를 제외한 */
                                                      /* 전화부에 저장된 이름 문자열만 나온다.         */
	
	pCallStack   =  GetCSKData();

	if(nStackIndex != INIT_CALLSTACK_INDEX)   
	{
		if(STRLEN(pCallStack->Stack[nStackIndex].aName) != 0)   /* CNAP로 날아온 이름이 있는 경우 */
		{
		  STRCPY(szTmpBuffer, pCallStack->Stack[nStackIndex].aName);
		}
		else if(sGetNameWithNum(pCallStack->Stack[nStackIndex].aDigits, szTmpBuffer, szPBookName) == PB_READ_OK)
		{
		  if(szTmpBuffer[0] == '\0')  
          {
             STRCPY(szTmpBuffer, pCallStack->Stack[nStackIndex].aDigits);
          }
		}
		else  /* Phone Number를 szTmpBuf에 복사 */
		{
          if(STRLEN(pCallStack->Stack[nStackIndex].aDigits) == 0)
          {
            STRCPY(szTmpBuffer, SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_NO_NUMBER_INFO));
          }
          else
          {		  
            //STRCPY(szTmpBuffer, pCallStack->Stack[nStackIndex].aDigits);
            MakeHypenedPhoneNumber(pCallStack->Stack[nStackIndex].aDigits, szTmpBuffer);
          }
		}
	}

	switch(pCallStack->Stack[nStackIndex].bStatus)
	{
		case CT_CALLED_BY:
    	  sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                         SF_ALERT_CALL1_2, 
			                                                             //SF_ALERT_CALL2_2,
										                                 szTmpBuffer);      
		break;

		case CT_CALL_TO:
		  sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                     SF_ORIGINATE_CALL1_2,
			                                                         //SF_ORIGINATE_CALL2_2,
										                             szTmpBuffer);      
		break;

		case CT_MISSED_CALL:
		  sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                        SF_MISSED_CALL1_2, 
			                                                            //SF_MISSED_CALL2_2,
										                                szTmpBuffer);      
		break;

		case CT_SMS_MO:
		  sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                       SF_MESSAGE_SENT1_2,
			                                                           //SF_MESSAGE_SENT2_2,
			                                                           szTmpBuffer);      
		break;

		case CT_SMS_MT:
		  sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                   SF_MESSAGE_RECEIVED1_2,
			                                                       //SF_MESSAGE_RECEIVED2_2,
										                           szTmpBuffer); 
		break;
        
		case CT_CALLED_BY_INPROG:
          sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_IDLE_CALLHISTORY_FMT_SCROLL), 
                                                                  SF_LINE_BUSY_RECEIPT1_2,
											                      //SF_LINE_BUSY_RECEIPT2_2,
											                      szTmpBuffer);
		  break;

	}  
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API SKA_MainCallList(SKY_EVENT_T *pEvent)
{
    HCONTROL  hControl;
    STATIC HCONTROL  hCtrlPopUp;
    STATIC HCONTROL  hCtrlDeleteOne, hCtrlDeleteAll, hCtrlMsg, hCtrlDeleteAllEnd, hCtrlDeleteOneEnd;
    STATIC BYTE      aPhoneNumber[NE_MAX_SIZE];
    UINT8            /*nStackIndex, */nListHandle, /*nTotalEntry,*/ nLength, nCurrIndex, nSize;
    UINT8            nLogicalIndex;
    CALLSTACK_T*     pCallStack;
    INT16            i, j, Index;
	STATIC INT16     nPBookAddress;
	STATIC UINT8     Handle;    
    

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
		Handle = sIsData();
        if( Handle )    /* 착발신 이력에 데이터가 있는 경우 */
        {
         if((EQS_GET_PARENT() == SKA_MainIdleScreen)   || 
            (EQS_GET_PARENT() == SKA_MissedCall) ||
            (EQS_GET_PARENT() == SKA_IdleDebugScreen))
         {
          EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_MENU_CONFIRM_NONE, //CL_MENU_CONFIRM_DELETE, 
                        NULL, 
                        WA_SHOW_CML);
         }
         else if(EQS_GET_PARENT() == SKA_SMSEditMessage)                        /* SMS 연동 */
         {
          EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_CONFIRM_TOSMS, 
                        NULL, 
                        WA_SHOW_CML);
         }
#ifdef FEATURE_SKTT_PM
		 else if((EQS_GET_PARENT() == SKA_RedactPM) || 
			     (EQS_GET_PARENT() == SKA_EditPM))
		 {
           EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        //CL_NONE_CONFIRM_ASSIGN, 
                        CL_NONE_SELECT_INDETAIL, 
                        NULL, 
                        WA_SHOW_CML);
		 }
#endif//FEATURE_SKTT_PM
         sDisplayCallList(Handle);
         EQS_FitCanvasSize();
        }
        else            /* 착발신 이력에 데이터가 없는 경우 */
        {
#ifdef FEATURE_CALL_STACK_SEPERATED    
            if(GetReceivedCallStack())
            {
               hCtrlMsg = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_NO_CALLHISTORY_CALLED), 
                                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            }
            else
            {
               hCtrlMsg = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_NO_CALLHISTORY_CALLTO), 
                                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);                
            }
#else
            hCtrlMsg = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_NO_CALLHISTORY), MESSAGE_DEFAULT_TIME, MBT_NONE, 
                                       MBT_HELP);
#endif// FEATURE_CALL_STACK_SEPERATED
        }
        SKY_SET_CALL_STACK_CHANGED(0);
        break;

    case SE_APP_END :   /* The very last event */
        SKY_SET_CALL_STACK_CHANGED(0);
		EQS_ReleaseDC();
        break;

	case SE_APP_RESUME :
        if(EQS_GET_CHILD() == SKA_DisplayDetailInfo)
        {       
            if(SKY_GET_CALL_STACK_CHANGED() && (GetReceivedCallStack()==TRUE))
            {
                UINT8 nCurrentData, nDeleteData;

                if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
                {
                  EQS_DeleteControl(hControl);
                }

                hControl    = EQS_GetNthIOC(0);
                nListHandle = EQC_GetScrollListHandle(hControl);
                pCallStack  = GetCSKData();

                nCurrentData = EQC_GetScrollMaxDataLine(hControl);
                nDeleteData = nCurrentData + SKY_GET_CALL_STACK_CHANGED();

                if(nDeleteData > NUM_CALL_HISTORY)
                {
                    nDeleteData = nDeleteData - NUM_CALL_HISTORY;

                    while( nDeleteData > 0 )
                    {
                        RemoveListItem(nListHandle, nCurrentData-1);
                        nCurrentData--;
                        nDeleteData--;
                    }
                    EQC_SetScrollMaxDataLine(hControl, nCurrentData);
                }

                for(Index=SKY_GET_CALL_STACK_CHANGED()-1; Index >=0 ; Index--)
                {
                    InsertList(nListHandle,0, pCallStack->Index.aIndex[Index], EQS_NA);
                }
                EQC_SetScrollMaxDataLine(hControl, nCurrentData + SKY_GET_CALL_STACK_CHANGED());
                EQC_SetScrollLogicalIndex(hControl, 0);		  
                EQS_SetIOCActive(hControl);     /* Scroll list를 Active 시킴 */
                SKY_SET_CALL_STACK_CHANGED(0);  
            }
            else
            {          
                 hControl     = EQS_GetNthIOC(0);
		         EQC_SetScrollLogicalIndex(hControl, pEvent->dwParam);
                 EQS_SetIOCActive(hControl);  /* Scroll list를 Active 시킴 */
            }
        }
        else
        {          
          if(GetCallStackIndex() != INIT_CALLSTACK_INDEX)  /* 선택을 하고 종료되는 경우 */
          {
			EQS_END();
          }
          else                            /* 취소키를 이용해서 종료되는 경우 */
          {
                if(SKY_GET_CALL_STACK_CHANGED() && (GetReceivedCallStack() == TRUE))
                {
                    UINT8 nCurrentData, nDeleteData;

                    if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
                    {
                      EQS_DeleteControl(hControl);
                    }

                    hControl    = EQS_GetNthIOC(0);
                    nListHandle = EQC_GetScrollListHandle(hControl);
                    pCallStack  = GetCSKData();

                    nCurrentData = EQC_GetScrollMaxDataLine(hControl);
                    nDeleteData = nCurrentData + SKY_GET_CALL_STACK_CHANGED();

                    if(nDeleteData > NUM_CALL_HISTORY)
                    {
                        nDeleteData = nDeleteData - NUM_CALL_HISTORY;

                        while( nDeleteData > 0 )
                        {
                            RemoveListItem(nListHandle, nCurrentData-1);
                            nCurrentData--;
                            nDeleteData--;
                        }
                        EQC_SetScrollMaxDataLine(hControl, nCurrentData);
                    }

                    for(Index=SKY_GET_CALL_STACK_CHANGED()-1; Index >=0 ; Index--)
                    {
                        InsertList(nListHandle,0, pCallStack->Index.aIndex[Index], EQS_NA);
                    }
                    EQC_SetScrollMaxDataLine(hControl, nCurrentData + SKY_GET_CALL_STACK_CHANGED());
                    EQC_SetScrollLogicalIndex(hControl, 0);		  
                    EQS_SetIOCActive(hControl);     /* Scroll list를 Active 시킴 */
                    SKY_SET_CALL_STACK_CHANGED(0);
                }
                else
                {
                    hControl     = EQS_GetNthIOC(0);
			        EQC_SetScrollLogicalIndex(hControl, pEvent->dwParam);
                    EQS_SetIOCActive(hControl);  /* Scroll list를 Active 시킴 */
                }
          }
        }
		break;

    case SE_APP_DRAW :
        break;        

    case SE_SYS_TO_IDLE:
        break;

    case SE_CALL_STACK_CHANGED:
        if(GetReceivedCallStack() == TRUE)
        {
            if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
            {
              EQS_DeleteControl(hControl);
            }
            hControl    = EQS_GetNthIOC(0);
            nListHandle = EQC_GetScrollListHandle(hControl);
            pCallStack  = GetCSKData();        
            if(EQC_GetScrollMaxDataLine(hControl) < NUM_CALL_HISTORY)
            {
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
                EQC_SetScrollMaxDataLine(hControl, EQC_GetScrollMaxDataLine(hControl)+1);
            }
            else
            {
                RemoveListItem(nListHandle, NUM_CALL_HISTORY-1);
                InsertList(nListHandle,0, pCallStack->Index.aIndex[0], EQS_NA);
            }
            EQC_SetScrollLogicalIndex(hControl, 0);
        }
        break;
  
    case SE_IO_SCROLL_LIST_SELECT :
        nListHandle = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
        nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        Index = GetListValue(nListHandle, nLogicalIndex);   

        if((EQS_GET_PARENT() == SKA_MainIdleScreen)   || 
		   (EQS_GET_PARENT() == SKA_MissedCall) ||
           (EQS_GET_PARENT() == SKA_IdleDebugScreen))
        {
          EQS_START_CHILD_EVENT(SKA_DisplayDetailInfo, 
                                SE_IO_SCROLL_LIST_SELECT, 
                                Index,        /* Call Stack에서의 Index */
                                EQS_GetNthIOC(0));
        }
        else if(EQS_GET_PARENT() == SKA_SMSEditMessage)   /* SMS 연동 */
        {
          EQS_START_CHILD_EVENT(sSKA_DisplayDetailInfoForSMS, 
                                SE_IO_SCROLL_LIST_SELECT, 
                                Index, 
                                EQS_GetNthIOC(0));
        }
#ifdef FEATURE_SKTT_PM
		else if((EQS_GET_PARENT() == SKA_RedactPM) ||
			    (EQS_GET_PARENT() == SKA_EditPM))
		{
        pCallStack  = GetCSKData();

        if((EQS_GET_PARENT() == SKA_RedactPM) || 
           (EQS_GET_PARENT() == SKA_EditPM))
        {
           g_PhoneNumBuf[0] = '\0';
           STRCPY(g_PhoneNumBuf, pCallStack->Stack[Index].aDigits);
        }
        SetCallStackIndex(Index);
        EQS_END();
		}
#endif //FEATURE_SKTT_PM
        break;

 
    case SE_IO_SCROLL_LIST_UP :    /* 화면의 갱신이 필요할 경우에만 발생하는 이벤트 */
    case SE_IO_SCROLL_LIST_DOWN :
        hControl    = pEvent->wParam;
        nCurrIndex  = pEvent->dwParam;  /* Logical Index */
        nListHandle = EQC_GetScrollListHandle(hControl);
        Index = GetListValue(nListHandle, nCurrIndex);
        sUpdateCallHistoryList(EQC_GetScrollData(hControl, EQC_GetScrollPhysicalIndex(hControl)),
                               Index);
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
         nLogicalIndex = pEvent->dwParam;
		 sUpdateAllCallHistoryList((UINT8) nLogicalIndex, pEvent->EventID);
        break;

#ifdef FEATURE_SKTT_PM
    case SE_SOFTKEY_INDETAIL:
        nListHandle = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
        nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        Index = GetListValue(nListHandle, nLogicalIndex);
        EQS_START_CHILD_EVENT(sSKA_DisplayDetailInfoForPhotoMail, 
                              SE_IO_SCROLL_LIST_SELECT, 
                              Index, 
                              EQS_GetNthIOC(0));
        break;
#endif //FEATURE_SKTT_PM


    case SE_SOFTKEY_MENU:
        nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
		nListHandle = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
		Index = GetListValue(nListHandle, nLogicalIndex);
        pCallStack = GetCSKData();
		nLength= pCallStack->Stack[Index].bNumDigits;        
        pCallStack->Stack[Index].aDigits[nLength] = '\0';

		if(IsInPhoneBook(Index, pCallStack->Stack[Index].aDigits, &nPBookAddress))
		{
          BOOL      paActive[NUM_CALLHISTORY_HOTMENU] = {FALSE, FALSE, TRUE, TRUE, TRUE, TRUE };
          hCtrlPopUp = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paCallHistory, NUM_CALLHISTORY_HOTMENU, 0,
                                              paActive);            
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_CALL_HISTORY_PHONEBOOK);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_CALL_HISTORY_SMS);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE_ONE);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE);
		}
        else if(pCallStack->Stack[Index].aDigits[0] == '\0') /* 번호정보가 없는 경우 */
        {
          BOOL      paActive[NUM_CALLHISTORY_HOTMENU] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE };
          hCtrlPopUp = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paCallHistory, NUM_CALLHISTORY_HOTMENU, 0,
                                              paActive);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE_ONE);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE);
        }
		else  /* Phone Book에 없는 데이터의 경우 */
		{
          BOOL      paActive[NUM_CALLHISTORY_HOTMENU] = {TRUE, TRUE, FALSE, TRUE, TRUE, TRUE };
          hCtrlPopUp = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paCallHistory, NUM_CALLHISTORY_HOTMENU, 0,
                                              paActive);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_PBOOK_SAVE_NEW_DATA);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_PBOOK_SAVE_OLD_DATA);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_CALL_HISTORY_SMS);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE_ONE);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE);          
		}
        break;

    case SE_SOFTKEY_DELETE:
        hCtrlDeleteOne = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE), MESSAGE_DEFAULT_TIME, 
                                              MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_SOFTKEY_PBOOK:  
        if(pEvent->wParam == BOOKCODE_VIEW_IN_LIST)
        {
            EQS_START_CHILD_EVENT(SKA_ViewBookContent, SE_APP_START, 
                                  nPBookAddress, BFT_NON_FUNCTION_E);
            break;
        }
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam)
        {
        case PU_CALLHISTORY_NEW_SAVE:
	        nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
	        nListHandle   = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
	        Index = GetListValue(nListHandle, nLogicalIndex);
	        pCallStack    = GetCSKData();
	        nLength= pCallStack->Stack[Index].bNumDigits;        
	        pCallStack->Stack[Index].aDigits[nLength] = '\0';
            SaveOnNewBook(pCallStack->Stack[Index].aDigits, TRUE);
	        break;		   

        case PU_CALLHISTORY_OLD_SAVE:
	        nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
	        nListHandle   = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
	        Index = GetListValue(nListHandle, nLogicalIndex);
	        pCallStack    = GetCSKData();
	        nLength= pCallStack->Stack[Index].bNumDigits;        
	        pCallStack->Stack[Index].aDigits[nLength] = '\0';
            SaveOnOldBook(pCallStack->Stack[Index].aDigits, TRUE);              
	        break;                 

        case PU_CALLHISTORY_PHONEBOOK:
            if(!SKY_IS_PHONE_BOOK_LOCK())
            {
                EQS_START_CHILD_EVENT(SKA_ViewBookContent, SE_APP_START, 
                                      nPBookAddress, BFT_NON_FUNCTION_E);
            }
            else
            {
                EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, 
                                        BOOKCODE_VIEW_IN_LIST, nPBookAddress);
            }
           break;

        case PU_CALLHISTORY_SMS:
           memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
           nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
           nListHandle   = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
           Index = GetListValue(nListHandle, nLogicalIndex);

           pCallStack    = GetCSKData();

           nLength= pCallStack->Stack[Index].bNumDigits;        
           pCallStack->Stack[Index].aDigits[nLength] = '\0';
           STRCPY(g_UIMOBuffer.pszRcvNumber, pCallStack->Stack[Index].aDigits);
           g_UIMOBuffer.nRcvNumberSize = nLength;

           if(!SKY_GET_SMS_LOCK_MODE())
           {
                EQS_START_CHILD(SKA_SMSEditMessage);
           }
           else
           {
                EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, 
                                      SMSCODE_IN_LIST, EQS_NA);               
           }
           break;

        case PU_CALLHISTORY_DELETE_ONE:
           hCtrlDeleteOne = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE), 
                                            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
           break;           

        case PU_CALLHISTORY_DELETE_ALL:
#ifdef FEATURE_CALL_STACK_SEPERATED    
                if(GetReceivedCallStack())
                {
                  hCtrlDeleteAll = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE_CALLED), 
                                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                }
                else
                {
                  hCtrlDeleteAll = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE_CALLTO), 
                                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM); 
                }
#else
				hCtrlDeleteAll = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASEALL), 
                                                 MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
#endif// FEATURE_CALL_STACK_SEPERATED
           break;
        }
        break;
    
    case SE_SOFTKEY_CANCEL:
        EQS_END();
        break;

    case SE_SOFTKEY_TOSMS:
       /* 착발신 이력에서 전화 번호를 가지고 온다. */
        nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        nListHandle   = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
        Index = GetListValue(nListHandle, nLogicalIndex);
        pCallStack  = GetCSKData();

        if(EQS_GET_PARENT() == SKA_SMSEditMessage)
        {
           memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
           nLength= pCallStack->Stack[Index].bNumDigits;        
           pCallStack->Stack[Index].aDigits[nLength] = '\0';
           STRCPY(g_UIMOBuffer.pszRcvNumber, pCallStack->Stack[Index].aDigits);
           g_UIMOBuffer.nRcvNumberSize = nLength;
        }
        else if((EQS_GET_PARENT() == SKA_RedactPM) || 
                (EQS_GET_PARENT() == SKA_EditPM))
        {
           g_PhoneNumBuf[0] = '\0';
           STRCPY(g_PhoneNumBuf, pCallStack->Stack[Index].aDigits);
        }
        SetCallStackIndex(Index);
        EQS_END();
        break;   

    case SE_IO_SCROLL_LIST_CANCEL :
        EQS_END();
        break;

    case SE_IO_MESGBOX_END :
        if((pEvent->wParam == hCtrlMsg) || 
		   (pEvent->wParam == hCtrlDeleteAllEnd))
        {
#ifdef FEATURE_CALL_STACK_SEPERATED    
           if(GetReceivedCallStack() == FALSE)
#endif// FEATURE_CALL_STACK_SEPERATED
           {
                /* 발신이력이 전부삭제 되었을 경우만 */
                /* Last Dial Info를 초기화 한다.     */
                InitializeLastDialInfo();
           }
           EQS_END();
        }
		else if(pEvent->wParam == hCtrlDeleteOneEnd)
		{
		  hControl = EQS_GetNthIOC(0);
		  nSize = EQC_GetScrollMaxDataLine(hControl);
          if(nSize == 0)  /* 삭제 후 스크롤 리스트에 데이터가 하나도 없는 경우 */
          {               /* 빠져 나간다.                                      */
#ifdef FEATURE_CALL_STACK_SEPERATED    
            if(GetReceivedCallStack() == FALSE)
#endif// FEATURE_CALL_STACK_SEPERATED
            {
                 InitializeLastDialInfo();
            }
            EQS_END();
          }
          else            /* 스크롤 리스트에 데이터가 있는 경우 다시 그려준다. */
          {                        
            EQS_SEND_EVENT(SE_APP_DRAW,(WORD)hControl ,0);
          }
		}
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlDeleteOne)
        {
           hControl = EQS_GetNthIOC(0);
           nListHandle = EQC_GetScrollListHandle(hControl);
           nCurrIndex  = EQC_GetScrollLogicalIndex(hControl);
           pCallStack  = GetCSKData();

           Index = GetListValue(nListHandle,nCurrIndex);
           for(i = 0 ; i < NUM_CALL_HISTORY; i++)
           {
              if(pCallStack->Index.aIndex[i] == Index)
              {
                 for(j = i; j < NUM_CALL_HISTORY-1; j++)
                 {
                    pCallStack->Index.aIndex[j] = pCallStack->Index.aIndex[j+1];
                 }
                 
                 pCallStack->Index.aIndex[j] = INIT_CALLSTACK_INDEX;
                 pCallStack->UsableList.bCurrentPos++;
                 pCallStack->UsableList.aUsableIndex[pCallStack->UsableList.bCurrentPos] = Index;
                 break;
              }
           } 
#ifdef FEATURE_CALL_STACK_SEPERATED    
           if(GetReceivedCallStack())
           {
              WriteCallStackReceived(); 
           }
           else
           {
              WriteCallStack();   
           }
#else
           WriteCallStack();   
#endif// FEATURE_CALL_STACK_SEPERATED
           RemoveListItem(nListHandle,nCurrIndex);
           nCurrIndex = EQC_DeleteScrollData(hControl, nCurrIndex);/* 화면에 처음 뿌려질 데이터의 인덱스 */

           nSize = EQC_GetScrollMaxDataLine(hControl);
           if( nSize > NUM_CALL_SCROLL )  nSize = NUM_CALL_SCROLL;

           for( i=0; i< nSize; i++)
           {
             Index = GetListValue(nListHandle, (i+nCurrIndex));
             sUpdateCallHistoryList(EQC_GetScrollData(hControl, i), Index);
           }
//#ifdef FEATURE_CALL_STACK_SEPERATED    
//           if(GetReceivedCallStack() == FALSE)
//#endif// FEATURE_CALL_STACK_SEPERATED
//           {
//               UpdateLastDialInfoAsDeleteOneItem();
//           }
           hCtrlDeleteOneEnd = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_SUCCESS_ERASE), MESSAGE_DEFAULT_TIME, 
                                             MBT_NONE, MBT_SUCCESS);          
       }
       else if(pEvent->wParam == hCtrlDeleteAll)  /* Call Stack에서 모든 아이템을 지울 경우 */
       {          
#ifdef FEATURE_CALL_STACK_SEPERATED    
         if(GetReceivedCallStack())
         {
           DeleteAllCallStackReceived(); 
         }
         else
         {
           DeleteAllCallStack();  
         }
#else
        DeleteAllCallStack();   
#endif// FEATURE_CALL_STACK_SEPERATED
		 hCtrlDeleteAllEnd = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_SUCCESS_ERASE), MESSAGE_DEFAULT_TIME, 
                                             MBT_NONE, MBT_SUCCESS);
       }
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_KEY_0 ://HS_0_K:
    case SE_KEY_1 ://HS_1_K:
    case SE_KEY_2 ://HS_2_K:
    case SE_KEY_3 ://HS_3_K:
    case SE_KEY_4 ://HS_4_K:
    case SE_KEY_5 ://HS_5_K:
    case SE_KEY_6 ://HS_6_K:
    case SE_KEY_7 ://HS_7_K:
    case SE_KEY_8 ://HS_8_K:
    case SE_KEY_9 ://HS_9_K:
    case SE_KEY_STAR  ://HS_STAR_K:
    case SE_KEY_POUND ://HS_POUND_K:
	case SE_KEY_CLEAR :// HS_CLR_K:
     	break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_SEND ://HS_SEND_K:
      if(Handle !=0) /*착발신 이력에 데이터가 있는 경우 */
      {
          nLogicalIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
          nListHandle   = EQC_GetScrollListHandle(EQS_GetNthIOC(0));
          Index = GetListValue(nListHandle, nLogicalIndex);
          pCallStack    = GetCSKData();

          nLength= pCallStack->Stack[Index].bNumDigits;        
          pCallStack->Stack[Index].aDigits[nLength] = '\0';
          STRCPY(aPhoneNumber, pCallStack->Stack[Index].aDigits);

          if(aPhoneNumber[0] != '\0')
          {                          
            SET_NUMEDIT_BUFFER(aPhoneNumber, nLength);
            SET_NUMEDIT_POS(nLength);  
            EQS_START_SIBLING(SKA_StartOrigination, FALSE);                                
          }
      }
		break;

    case SE_KEY_END ://HS_END_K:
        if(EQS_GET_PARENT() == SKA_SMSEditMessage)
		{
			EQS_END_TO(EQS_POP_TO_ROOT);
		}
#ifdef FEATURE_SKTT_PM
        else if((EQS_GET_PARENT() == SKA_RedactPM) || 
		   (EQS_GET_PARENT() == SKA_EditPM))
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
#endif// FEATURE_SKTT_PM
        else
        {
            EQS_END();
        }
		break;  

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}






/*
**-----------------------------------------------------------------------------
**  Call Stack 관련 함수들                                            Start!!  
**-----------------------------------------------------------------------------
*/
STATIC CALLSTACK_T    gCallStack;

#ifdef FEATURE_CALL_STACK_SEPERATED
STATIC CALLSTACK_T    gCallStackReceived;
STATIC BOOL           gfReceivedCallStack = TRUE;

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetReceivedCallStack()
**
** Description: 
**    Set Received Call Stack flag
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SetReceivedCallStack(BOOL fIsReceived)
{
   gfReceivedCallStack = fIsReceived;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetReceivedCallStack()
**
** Description: 
**    Set Received Call Stack flag
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL GetReceivedCallStack(void)
{
   return gfReceivedCallStack;
}
#endif //FEATURE_CALL_STACK_SEPERATED

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCSKIndex()
**
** Description: 
**     Return Call Stack Index
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
CALLSTACKINDEX_T GetCSKIndex(void)
{
    return gCallStack.Index;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetCSKIndex(CALLSTACKINDEX_T)
**
** Description: 
**     Set global variable gCallStack.Index with argument Index
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetCSKIndex(CALLSTACKINDEX_T Index)
{
    gCallStack.Index = Index;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCSKStack(UINT8 nIndex)     
**
** Description: 
**     Return Nth Call Stack Dial info.
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
CALLSTACKDIAL_T GetCSKStack(UINT8 nIndex)
{
    return gCallStack.Stack[nIndex];
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetCSKStack()
**
** Description: 
**     
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetCSKStack(UINT8 nIndex, CALLSTACKDIAL_T Data)
{
    gCallStack.Stack[nIndex] = Data;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCSKData(void)
**
** Description: 
**     Return the address of global variable gCallStack
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
CALLSTACK_T* GetCSKData(void)
{   
#ifdef FEATURE_CALL_STACK_SEPERATED    
    if(GetReceivedCallStack())
    {
       return &gCallStackReceived;
    }
    else
    {
       return &gCallStack;
    }
#else
    return &gCallStack;
#endif //FEATURE_CALL_STACK_SEPERATED     
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetCSKData(CALLSTACK_T* pszCallStack)
**
** Description: 
**     Set Call Stack Data with a input parameter
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetCSKData(CALLSTACK_T* pszCallStack)
{
    CALLSTACK_T* pCallStack;
    pCallStack = &gCallStack;

    memcpy(pCallStack, pszCallStack, sizeof(gCallStack));
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCSKChange()
**
** Description: 
**     Return gCallStack.fChange
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL GetCSKChange()
{
  return gCallStack.fChange;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetCSKChange(BOOL fValue)
**
** Description: 
**     Set Call Stack Change Flag with a input argument
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetCSKChange(BOOL fValue)
{
  gCallStack.fChange = fValue;
}


#ifdef FEATURE_CALL_STACK_SEPERATED
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sCreateNewCallStackReceived()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sCreateNewCallStackReceived(void)
{
    INT8    i;
    HFILE   hFile;

    /* there is not the file, create the file */
    hFile = SKY_OpenFile(FILE_NAME_CALLSTACK_RECEIVED, FS_AM_CREATE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    memset(&gCallStackReceived, 0, sizeof(CALLSTACK_T));    

    STRCPY(gCallStackReceived.abCallStack, CALL_STACK_ID);
    gCallStackReceived.bVersion = CALL_STACK_VERSION;

    gCallStackReceived.UsableList.bCurrentPos = NV_MAX_STACK_DIALS-1;

    for(i = 0; i < NV_MAX_STACK_DIALS ; i ++)
    {
        gCallStackReceived.Index.aIndex[i] = INIT_CALLSTACK_INDEX;
        gCallStackReceived.UsableList.aUsableIndex[i] = i;
        gCallStackReceived.Stack[i].aDigits[0] = '\0';
        gCallStackReceived.Stack[i].aName[0]   = '\0';
    }
    gCallStackReceived.fChange = FALSE;
    SKY_WriteFile(hFile, &gCallStackReceived, sizeof(gCallStackReceived));
    SKY_CloseFile(hFile);
}



void SKY_API SKY_InitCallStackReceived(void)
{
    //INT8    i;
    HFILE   hFile;
    DWORD   dwCount;

    /* Call Stack is not exist */
    if(SKY_IsFileOrDirExist(SKY_CALLSTACK_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_CALLSTACK_DIR);
    }

    /* Call Stack is not exist */
    if(SKY_IsFileOrDirExist(FILE_NAME_CALLSTACK_RECEIVED) == FALSE)
    {
        sCreateNewCallStackReceived();
    }
    else    /* Call Stack is already exist */
    {
        hFile = SKY_OpenFile(FILE_NAME_CALLSTACK_RECEIVED, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
        dwCount = SKY_ReadFile(hFile, &gCallStackReceived, sizeof(gCallStackReceived));
        SKY_CloseFile(hFile);

        if((STRCMP(gCallStackReceived.abCallStack, CALL_STACK_ID) != 0)
            || (gCallStackReceived.bVersion != CALL_STACK_VERSION))
        /* data file structure is changed */
        {
            SKY_DeleteFile(FILE_NAME_CALLSTACK_RECEIVED);

            /* ... */
            sCreateNewCallStackReceived();
        }
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**      WriteCallStackReceived()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API WriteCallStackReceived()
{
   HFILE  hFile;
   DWORD  dwCount;

   hFile = SKY_OpenFile(FILE_NAME_CALLSTACK_RECEIVED, FS_AM_READWRITE);
   if(hFile == SKY_INVALID_FILE_HANDLE)
   {
       hFile = SKY_OpenFile(FILE_NAME_CALLSTACK_RECEIVED, FS_AM_CREATE);
   }
   dwCount = SKY_WriteFile(hFile, &gCallStackReceived, sizeof(gCallStackReceived));
   SKY_CloseFile(hFile);
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API DeleteAllCallStackReceived(void)
{
    INT8    i;
    HFILE   hFile;

        /* Call Stack is not exist */
    if(SKY_IsFileOrDirExist(SKY_CALLSTACK_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_CALLSTACK_DIR);
    }    /* Call Stack is not exist */

    if(SKY_IsFileOrDirExist(FILE_NAME_CALLSTACK_RECEIVED) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_CALLSTACK_RECEIVED, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
        memset(&gCallStackReceived, 0, sizeof(CALLSTACK_T));
        STRCPY(gCallStackReceived.abCallStack, CALL_STACK_ID);
        gCallStackReceived.bVersion = CALL_STACK_VERSION;
        gCallStackReceived.UsableList.bCurrentPos = NV_MAX_STACK_DIALS-1;
    
        for(i = 0; i < NV_MAX_STACK_DIALS ; i ++)
        {
            gCallStackReceived.Index.aIndex[i] = INIT_CALLSTACK_INDEX;
            gCallStackReceived.UsableList.aUsableIndex[i] = i;
        }
        gCallStackReceived.fChange = FALSE;
        SKY_WriteFile(hFile, &gCallStackReceived, sizeof(gCallStackReceived));
        SKY_CloseFile(hFile);
    }
    else    /* Call Stack is already exist */
    {
        hFile = SKY_OpenFile(FILE_NAME_CALLSTACK_RECEIVED, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
        memset(&gCallStackReceived, 0, sizeof(CALLSTACK_T));
        STRCPY(gCallStackReceived.abCallStack, CALL_STACK_ID);
        gCallStackReceived.bVersion = CALL_STACK_VERSION;
        gCallStackReceived.UsableList.bCurrentPos = NV_MAX_STACK_DIALS-1;
    
        for(i = 0; i < NV_MAX_STACK_DIALS ; i ++)
        {
            gCallStackReceived.Index.aIndex[i] = INIT_CALLSTACK_INDEX;
            gCallStackReceived.UsableList.aUsableIndex[i] = i;
        }
        gCallStackReceived.fChange = FALSE;
        SKY_WriteFile(hFile, &gCallStackReceived, sizeof(gCallStackReceived));
        SKY_CloseFile(hFile);
    }
}
#endif//FEATURE_CALL_STACK_SEPERATED



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sCreateNewCallStack(void)
{
    INT8    i;
    HFILE   hFile;

    /* there is not the file, create the file */
    hFile = SKY_OpenFile(FILE_NAME_CALLSTACK, FS_AM_CREATE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    memset(&gCallStack, 0, sizeof(CALLSTACK_T));    

    STRCPY(gCallStack.abCallStack, CALL_STACK_ID);
    gCallStack.bVersion = CALL_STACK_VERSION;

    gCallStack.UsableList.bCurrentPos = NV_MAX_STACK_DIALS-1;

    for(i = 0; i < NV_MAX_STACK_DIALS ; i ++)
    {
        gCallStack.Index.aIndex[i] = INIT_CALLSTACK_INDEX;
        gCallStack.UsableList.aUsableIndex[i] = i;
        gCallStack.Stack[i].aDigits[0] = '\0';
        gCallStack.Stack[i].aName[0]   = '\0';
    }
    gCallStack.fChange = FALSE;
    SKY_WriteFile(hFile, &gCallStack, sizeof(gCallStack));
    SKY_CloseFile(hFile);
}



void SKY_API SKY_InitCallStack(void)
{
    //INT8    i;
    HFILE   hFile;
    DWORD   dwCount;

    /* Call Stack is not exist */
    if(SKY_IsFileOrDirExist(SKY_CALLSTACK_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_CALLSTACK_DIR);
    }

    /* Call Stack is not exist */
    if(SKY_IsFileOrDirExist(FILE_NAME_CALLSTACK) == FALSE)
    {
        sCreateNewCallStack();
    }
    else    /* Call Stack is already exist */
    {
        hFile = SKY_OpenFile(FILE_NAME_CALLSTACK, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
        dwCount = SKY_ReadFile(hFile, &gCallStack, sizeof(gCallStack));
        SKY_CloseFile(hFile);

        if((STRCMP(gCallStack.abCallStack, CALL_STACK_ID) != 0)
            || (gCallStack.bVersion != CALL_STACK_VERSION))
        /* data file structure is changed */
        {
            SKY_DeleteFile(FILE_NAME_CALLSTACK);

            /* ... */
            sCreateNewCallStack();
        }

    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**      WriteCallStack()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API WriteCallStack()
{
   HFILE  hFile;
   DWORD  dwCount;

   hFile = SKY_OpenFile(FILE_NAME_CALLSTACK, FS_AM_READWRITE);
   if(hFile == SKY_INVALID_FILE_HANDLE)
   {
       hFile = SKY_OpenFile(FILE_NAME_CALLSTACK, FS_AM_CREATE);
   }
   dwCount = SKY_WriteFile(hFile, &gCallStack, sizeof(gCallStack));
   SKY_CloseFile(hFile);
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API DeleteAllCallStack(void)
{
    INT8    i;
    HFILE   hFile;

        /* Call Stack is not exist */
    if(SKY_IsFileOrDirExist(SKY_CALLSTACK_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_CALLSTACK_DIR);
    }    /* Call Stack is not exist */

    if(SKY_IsFileOrDirExist(FILE_NAME_CALLSTACK) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_CALLSTACK, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
        memset(&gCallStack, 0, sizeof(CALLSTACK_T));
        STRCPY(gCallStack.abCallStack, CALL_STACK_ID);
        gCallStack.bVersion = CALL_STACK_VERSION;
        gCallStack.UsableList.bCurrentPos = NV_MAX_STACK_DIALS-1;
    
        for(i = 0; i < NV_MAX_STACK_DIALS ; i ++)
        {
            gCallStack.Index.aIndex[i] = INIT_CALLSTACK_INDEX;
            gCallStack.UsableList.aUsableIndex[i] = i;
        }
        gCallStack.fChange = FALSE;
        SKY_WriteFile(hFile, &gCallStack, sizeof(gCallStack));
        SKY_CloseFile(hFile);
    }
    else    /* Call Stack is already exist */
    {
        hFile = SKY_OpenFile(FILE_NAME_CALLSTACK, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
        memset(&gCallStack, 0, sizeof(CALLSTACK_T));
        STRCPY(gCallStack.abCallStack, CALL_STACK_ID);
        gCallStack.bVersion = CALL_STACK_VERSION;
        gCallStack.UsableList.bCurrentPos = NV_MAX_STACK_DIALS-1;
    
        for(i = 0; i < NV_MAX_STACK_DIALS ; i ++)
        {
            gCallStack.Index.aIndex[i] = INIT_CALLSTACK_INDEX;
            gCallStack.UsableList.aUsableIndex[i] = i;
        }
        gCallStack.fChange = FALSE;
        SKY_WriteFile(hFile, &gCallStack, sizeof(gCallStack));
        SKY_CloseFile(hFile);
    }
}
