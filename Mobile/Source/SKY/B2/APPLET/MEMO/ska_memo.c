/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_memo.c
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
** 01-01-04 rosa     Created.
**
** ======================================================================= ====
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
#define MEMO_ERROR      -1
#define MEMO_VIEW_LINE  20 //6 + 13 + 1

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagMEMO_MAKE_TYPE_E
{
    MMT_NEW,
    MMT_REMAKE
};
typedef enum tagMEMO_MAKE_TYPE_E   MEMO_MAKE_TYPE_E;

enum tagMEMO_SET_MODE_E
{
    MSM_MEMO_TIME_IMAGE,
    MSM_MEMO_DAY_TEXT,
    MSM_MEMO_SPACE,
    MSM_MEMO_TIME_TEXT,
    MSM_MEMO_DATA,
    MSM_MAX
};

enum tagMEMO_EDIT_MODE_E
{
    MDM_MEMO_IMAGE_TITLE,
    MDM_MEMO_DATA,
    MDM_MAX
};

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

LOCAL  UINT8 l_SaveMemoAddress;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
static VOID SKY_API sSKA_ListMemo(SKY_EVENT_T *pEvent);
static VOID SKY_API sSKA_ViewMemo(SKY_EVENT_T *pEvent);
static VOID SKY_API sSKA_EditMemo(SKY_EVENT_T *pEvent);

static BOOL sSortMemo(INT8  anSortedIdxT[]);


/*
** ***************************************************************************

                               SKA_MainMemo()
                                    |
                                 {child}
                                    |
                              sSKA_ListMemo()
                                    |
                 -----------------------------------------
                |                                         |
             {child}                                   {child}
                |                                         |
          sSKA_ViewMemo()  -     {sibling}      -  sSKA_EditMemo()

** ***************************************************************************
*/

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
VOID SKY_API SKA_MainMemo(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_START_CHILD(sSKA_ListMemo);
        break;

    case SE_APP_END:
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


static VOID SKY_API sSKA_ListMemo(SKY_EVENT_T *pEvent)
{
    UINT8               i,unIdx, unIdxOfNew, unAddrOfMemo;
    BYTE                abDateTime[7];
    clk_julian_type     cjMemoTime;
    static  UINT8       hList, unTotalMemo;
    static  INT8        anSortedIdxT[MAX_MEMO_COUNT];
    static  BYTE*       pabMemoList[MAX_MEMO_COUNT];
	HCONTROL            hControl;
	BYTE*               pszStr;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        /* set window */
        EQS_SetWindow(  SKY_DEF_WIN_X, 
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_DELETE_CONFIRM_NEWMEMO,
                        SKY_GET_TOKEN(TKN_MEMO_LIST_TITLE),
                        WA_SHOW_CML | WA_SHOW_HEADER);

		l_SaveMemoAddress = MAX_MEMO_COUNT;
        /* sort */
        unIdx = 0;

        for( i=0; i<MAX_MEMO_COUNT; i++ )
        {
            /* alloc. */
            pabMemoList[i] = (BYTE *)SKY_Malloc( (SIZE_T)(MEMO_VIEW_LINE) );
		}

        hList = CreateList( MAX_MEMO_COUNT );
        if(sSortMemo(anSortedIdxT))
        {           
            for( i=0; i<MAX_MEMO_COUNT; i++ )
            {
                if( anSortedIdxT[i] > MEMO_ERROR )
                {    
                    clk_secs_to_julian( SKY_GET_MEMO_TIME(anSortedIdxT[i]), &cjMemoTime );// 6 bytes
                    sprintf((char*)abDateTime, "%2d.%02d ", cjMemoTime.month, cjMemoTime.day );
                    STRCPY( pabMemoList[unIdx], abDateTime );
                    STRNCAT( pabMemoList[unIdx], SKY_GET_MEMO_DATA(anSortedIdxT[i]), 13 );
                    AddList( hList, (INT16)anSortedIdxT[i], FALSE );
                    unIdx++;
                }
            }
            unTotalMemo = unIdx;
        }
        else
        {
            unTotalMemo = unIdx;
            EQS_SetWindowCML(CL_NONE_NONE_NEWMEMO);			
        }

        hControl = EQC_ScrollList(EQS_NA, 
							      EQS_NA,
								  EQS_NA, 
								  EQC_GetScrollDisplayLine(), 
								  pabMemoList, 
								  unTotalMemo, 
								  unTotalMemo, 
                                   0);
        EQC_SetScrollListHandle(hControl, hList );
        EQC_SET_STYLE(hControl,  CS_SCROLL_LIST_BORDER |\
                                 CS_SCROLL_LIST_MAXIMIZE |
								 CS_SCROLL_LIST_NO_CML|
                                 CS_SCROLL_LIST_ENUMBERATED);
        EQS_SetIOCActive(hControl);

        break;

    case SE_APP_END:        
        /* dealloc. */
        for( i=0; i<MAX_MEMO_COUNT; i++ )
		{
            SKY_Free( pabMemoList[i] );
		}
		DeleteList(hList);
        
        break;

    case SE_APP_RESUME:
		hControl = EQS_GetNthIOC(0);

		if( EQS_GET_CHILD() == sSKA_ViewMemo )
		{
			EQC_SetScrollPhysicalIndex(hControl, pEvent->dwParam );
		}

		if( l_SaveMemoAddress != MAX_MEMO_COUNT )
		{
			unIdx = EQC_GetScrollPhysicalIndex(hControl);
			if( GetListSize(hList) && GetListValue(hList, unIdx) == l_SaveMemoAddress ) // 기존 메모 수정
			{
				if( SKY_GET_MEMO_TYPE(l_SaveMemoAddress) != MT_NONE )
				{
                    clk_secs_to_julian( SKY_GET_MEMO_TIME(l_SaveMemoAddress), &cjMemoTime );// 6 bytes
                    sprintf((char*)abDateTime, "%2d.%02d ", cjMemoTime.month, cjMemoTime.day );
                    STRCPY( pabMemoList[unIdx], abDateTime );
                    STRNCAT( pabMemoList[unIdx], SKY_GET_MEMO_DATA(l_SaveMemoAddress), 13 );
				}
			}
			else // 새메모 생성 
			{
				if( SKY_GET_MEMO_TYPE(l_SaveMemoAddress) != MT_NONE )
				{
                    clk_secs_to_julian( SKY_GET_MEMO_TIME(l_SaveMemoAddress), &cjMemoTime );// 6 bytes
                    sprintf((char*)abDateTime, "%2d.%02d ", cjMemoTime.month, cjMemoTime.day );
                    STRCPY( pabMemoList[unTotalMemo], abDateTime );
                    STRNCAT( pabMemoList[unTotalMemo], SKY_GET_MEMO_DATA(l_SaveMemoAddress), 13 );
					AddList(hList, l_SaveMemoAddress, EQS_NA);
					EQC_AddScrollData(hControl, unTotalMemo,  pabMemoList[unTotalMemo] ,FALSE);
					unTotalMemo++;
				}
			}
			EQS_SetWindowCML(CL_DELETE_CONFIRM_NEWMEMO);
		}
		EQS_SetIOCActive(hControl);
		l_SaveMemoAddress = MAX_MEMO_COUNT;
        break;
    case SE_SOFTKEY_DELETE:
        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_Q_DELETE),
                       MESSAGE_DEFAULT_TIME, 
                       MBT_YESNO,
                       MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        unIdx = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(0));
        unAddrOfMemo = (UINT8)GetListValue(hList, unIdx); /* get the address */

        SKY_SET_MEMO_TYPE(unAddrOfMemo, MT_NONE);
        SKY_SET_MEMO_TIME(unAddrOfMemo, 0);
        SKY_SET_MEMO_DATA(unAddrOfMemo, "");

        EQC_DeleteScrollData(EQS_GetNthIOC(0), unIdx);
        RemoveListItem(hList, unIdx);

		pszStr = pabMemoList[unIdx]; // 인덱스 제조정 
		for( i=unIdx; i< MAX_MEMO_COUNT-1; i++)
		{
			pabMemoList[i] = pabMemoList[i+1];
		}
		pabMemoList[i] = pszStr;

        unTotalMemo = unTotalMemo - 1;

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_CONFIRM_DELETE),
                       MESSAGE_DEFAULT_TIME, 
                       MBT_NONE,
                       MBT_SUCCESS);
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_SOFTKEY_ADD: //new
        unIdxOfNew = MAX_MEMO_COUNT;
        for( i=0; i<MAX_MEMO_COUNT; i++ )
        {
            if( SKY_GET_MEMO_TYPE(i) == MT_NONE )
            {
                unIdxOfNew = i;
                break;
            }
        }
        if(unIdxOfNew < MAX_MEMO_COUNT)
		{
            EQS_START_CHILD_EVENT(sSKA_EditMemo, SE_APP_START, MMT_NEW, unIdxOfNew);
		}
        else 
		{
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_FULL), 
                           MESSAGE_DEFAULT_TIME, 
                           MBT_NONE, 
                           MBT_FAIL );
		}
        break;

    case SE_IO_SCROLL_LIST_SELECT:
		unIdx = (UINT8)EQC_GetScrollLogicalIndex( EQS_GetNthIOC(0) );
		//unAddrOfMemo = (UINT8)GetListValue( hList, unIdx ); //get the address
		EQS_START_CHILD_EVENT(sSKA_ViewMemo, SE_APP_START, hList, unIdx);//unAddrOfMemo);
        break;

    case EQE_IO_SCROLL_LIST_CANCEL:
        if( pEvent->dwParam != EQC_DELETED )
		{
            EQS_END();
		}
        break;

    case SE_IO_MESGBOX_END:
		if( unTotalMemo == 0 )
		{
			EQS_SetWindowCML(CL_NONE_NONE_NEWMEMO);
		}
        EQS_SetIOCActive(EQS_GetNthIOC(0));      
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }    
}

static VOID SKY_API sSKA_ViewMemo(SKY_EVENT_T *pEvent)
{
    static   UINT8               unIndex;
    static   DWORD               dwSetTime;
    static   clk_julian_type     cjSetTime;
	UINT8    MemoAddress;
	HCONTROL hControl;
	BYTE     szBuf[40];
	UINT8    ListHandle;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
		{
			MARGIN_T Margin;
            COORD    xSpace, ySpace;
            COORD    xPos, yPos;
            
            EQS_StartFormatter();   
            EQS_GetControlMargin(&Margin);
            EQS_GetControlSpace(&xSpace, &ySpace);
            EQS_EndFormatter();  

			EQS_SetWindow(SKY_DEF_WIN_X, 
						  SKY_DEF_WIN_Y, 
						  SKY_DEF_WIN_DX, 
						  SKY_DEF_WIN_DY, 
						  CL_NONE_NONE_EDIT, 
						  SKY_GET_TOKEN(TKN_MEMO_VIEW_TITLE),
						  WA_SHOW_CML | WA_SHOW_HEADER);

			ListHandle = pEvent->wParam;
			unIndex = (UINT8)pEvent->dwParam;

			MemoAddress = GetListValue( ListHandle, unIndex);

			dwSetTime = SKY_GET_MEMO_TIME(MemoAddress);
			clk_secs_to_julian(dwSetTime, &cjSetTime);

			/*
			** ***************************************************************************
			** CREATE
			** ***************************************************************************
			*/
			xPos = Margin.xLeft;
			yPos = Margin.yTop ;

            //제목
			hControl = EQC_StaticText(xPos, yPos, TKN_MEMO_MAKE_IMAGE_TITLE);			
			EQC_SET_SELF_DRAW(hControl, FALSE);

			xPos = 0;//Margin.xLeft;
            yPos += EQC_GET_DY(hControl) + ySpace;

            //날짜(년월일)
			hControl = EQC_StaticText(xPos, yPos, TKN_MEMO_MAKE_DATE);
			sprintf((char*)szBuf,"%d.%d.%d",\
                    cjSetTime.year,\
					cjSetTime.month,\
					cjSetTime.day);
			EQC_SetStaticText(hControl, szBuf);
			EQC_SET_SELF_DRAW(hControl, FALSE);
            
            xPos += EQC_GET_DX(hControl) + xSpace;

            //띄워쓰기
            hControl = EQC_StaticText(xPos, yPos, TKN_NULL);

            xPos += EQC_GET_DX(hControl) + xSpace;

            //시간(시분)
			hControl = EQC_StaticText(xPos, yPos, TKN_MEMO_MAKE_DATE);
			if( cjSetTime.hour >= 12 )
			{
				if( cjSetTime.hour > 12)
					cjSetTime.hour -=12;
				sprintf((char*)szBuf,"%d:%d%s",	cjSetTime.hour, cjSetTime.minute, SKY_GET_TOKEN(TKN_IDLE_PM));
			}
			else
			{
                sprintf((char*)szBuf,"%d:%d%s",	cjSetTime.hour, cjSetTime.minute, SKY_GET_TOKEN(TKN_IDLE_AM));
			}
			EQC_SetStaticText(hControl, szBuf);
			EQC_SET_SELF_DRAW(hControl, FALSE);

			xPos = Margin.xLeft+ xSpace;
			yPos += EQC_GET_DY(hControl) + ySpace ;

            //메모내용
			hControl = EQC_TextEdit(xPos, yPos,						
						 16,
						 4,
						 TEM_KOREAN, 
						 MAX_MEMO_DATA_LENGTH, 
						 SKY_GET_MEMO_DATA(MemoAddress),
						 0); /* data */			
			EQC_FitTextEditLine(hControl);

			EQC_SET_USERDATA(hControl, ListHandle); // 메모 리스트 핸들 
			EQC_SET_SELF_DRAW(hControl, FALSE);

			if( GetListSize(ListHandle) > 1 )
			{
				EQS_SetWindowCML(CL_NONE_MOVE_EDIT);
			}

			//EQC_FitTextEditLine(hControl);
			EQC_SET_FOCUS(hControl, FALSE);
			EQC_SET_EDIT(hControl, FALSE);

			EQS_FitCanvasSize();
		}
	
        break;

    case SE_APP_END:
        break;
	case SE_APP_RESUME:
		EQS_END_EVENT(SE_APP_RESUME, EQS_NA, unIndex);
		break;
	case SE_KEY_CLEAR:
		EQS_END_EVENT(SE_APP_RESUME, EQS_NA, unIndex);
		break;
    case SE_KEY_UP:
		ListHandle = EQC_GET_USERDATA(EQS_GetNthIOC(MSM_MEMO_DATA));
		if( unIndex == 0 )
		{
			unIndex = GetListSize(ListHandle) - 1;
		}
		else
		{
			unIndex--;
		}

		MemoAddress = GetListValue(ListHandle, unIndex);
		dwSetTime = SKY_GET_MEMO_TIME(MemoAddress);
		clk_secs_to_julian(dwSetTime, &cjSetTime);

        //날짜(년월일)
        sprintf((char*)szBuf,"%d.%d.%d",\
                cjSetTime.year,\
				cjSetTime.month,\
				cjSetTime.day);
		EQC_SetStaticText(EQS_GetNthIOC(MSM_MEMO_DAY_TEXT), szBuf);

        //시간(시분)
		if( cjSetTime.hour >= 12 )
		{
			if( cjSetTime.hour > 12)
				cjSetTime.hour -=12;
            sprintf((char*)szBuf,"%d:%d%s",	cjSetTime.hour, cjSetTime.minute, SKY_GET_TOKEN(TKN_IDLE_PM));
		}
		else
		{
            sprintf((char*)szBuf,"%d:%d%s",	cjSetTime.hour, cjSetTime.minute, SKY_GET_TOKEN(TKN_IDLE_AM));
		}
		EQC_SetStaticText(EQS_GetNthIOC(MSM_MEMO_TIME_TEXT), szBuf);

		EQC_SetTextEditData(EQS_GetNthIOC(MSM_MEMO_DATA) , SKY_GET_MEMO_DATA(MemoAddress));
		EQC_FitTextEditLine(EQS_GetNthIOC(MSM_MEMO_DATA));

		EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);

		break;
	case SE_KEY_DOWN:

		ListHandle = EQC_GET_USERDATA(EQS_GetNthIOC(MSM_MEMO_DATA));
		if( unIndex ==  (GetListSize(ListHandle) - 1) )
		{
			unIndex = 0;
		}
		else
		{
			unIndex++;
		}
		MemoAddress = GetListValue(ListHandle, unIndex);
		dwSetTime = SKY_GET_MEMO_TIME(MemoAddress);
		clk_secs_to_julian(dwSetTime, &cjSetTime);

        //날짜(년월일)
        sprintf((char*)szBuf,"%d.%d.%d",\
                cjSetTime.year,\
				cjSetTime.month,\
				cjSetTime.day);
		EQC_SetStaticText(EQS_GetNthIOC(MSM_MEMO_DAY_TEXT), szBuf);

		if( cjSetTime.hour >= 12 )
		{
			if( cjSetTime.hour > 12)
				cjSetTime.hour -=12;
            sprintf((char*)szBuf,"%d:%d%s",	cjSetTime.hour, cjSetTime.minute, SKY_GET_TOKEN(TKN_IDLE_PM));
		}
		else
		{
            sprintf((char*)szBuf,"%d:%d%s",	cjSetTime.hour, cjSetTime.minute, SKY_GET_TOKEN(TKN_IDLE_AM));
		}
		EQC_SetStaticText(EQS_GetNthIOC(MSM_MEMO_TIME_TEXT), szBuf);

		EQC_SetTextEditData(EQS_GetNthIOC(MSM_MEMO_DATA) , SKY_GET_MEMO_DATA(MemoAddress));
		EQC_FitTextEditLine(EQS_GetNthIOC(MSM_MEMO_DATA));

		EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);

		break;	
    case SE_SOFTKEY_EDIT:        
        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_GO_EDIT_MODE), 
                       0, 
                       MBT_NONE,
                       MBT_CONFIRM);
        break;

	case SE_IO_MESGBOX_END:
		ListHandle = EQC_GET_USERDATA(EQS_GetNthIOC(MSM_MEMO_DATA));
		MemoAddress = GetListValue(ListHandle, unIndex);
        EQS_START_CHILD_EVENT(sSKA_EditMemo, SE_APP_START, MMT_REMAKE, MemoAddress);
		break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

static VOID SKY_API sSKA_EditMemo(SKY_EVENT_T *pEvent)
{
    HCONTROL                    hMessageBox, hControl;
    static  UINT8               MemoAddress;
    static  BYTE                aszMemoData[MAX_MEMO_DATA_LENGTH+1];
    static  DWORD               dwSetTime;
    static  clk_julian_type     cjSetTime;
    static  MEMO_MAKE_TYPE_E    MakeType;
    static  HCONTROL            hNoData;

    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);

        MakeType = (MEMO_MAKE_TYPE_E)pEvent->wParam;
        MemoAddress = (UINT8)pEvent->dwParam; 

        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY,                      
                      CL_NONE,
                      SKY_GET_TOKEN(TKN_MEMO_MAKE_TITLE),
                      WA_SHOW_HEADER);

        /* get writing time */
        if(MakeType == MMT_NEW)
		{
            dwSetTime = SKY_GetLocalTime();
		}
        else /* MakeType == MMT_REMAKE */
		{
            dwSetTime = SKY_GET_MEMO_TIME(MemoAddress);
			EQS_SetSWindowHeaderStr(SKY_GET_TOKEN(TKN_MEMO_EDIT_TITLE));
		}
        clk_secs_to_julian(dwSetTime, &cjSetTime);

        /*
        ** ***************************************************************************
        ** CREATE
        ** ***************************************************************************
        */        
        EQC_StaticText( EQS_NA, EQS_NA, TKN_MEMO_MAKE_IMAGE_TITLE ); //MDM_MEMO_IMAGE_TITLE
        hControl = EQC_TextEdit( EQS_NA,
							     EQS_NA,
								 16,
								 4,
								 TEM_KOREAN,
								 MAX_MEMO_DATA_LENGTH, 
								 SKY_GET_MEMO_DATA( MemoAddress ), 
								 0 ); //MDM_MEMO_DATA

		EQC_SET_STYLE(hControl, CS_TEXT_EDIT_BORDER |
                                CS_TEXT_EDIT_ALONE);

        EQS_SetIOCActive(EQS_GetNthIOC(MDM_MEMO_DATA));

        EQS_StartFormatter();
        EQS_SetControlPosition(EQS_GetNthIOC(MDM_MEMO_IMAGE_TITLE), 
            ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();
        EQS_SetControlPosition(EQS_GetNthIOC(MDM_MEMO_DATA), 
            ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
        EQS_EndFormatter();
        EQS_FitCanvasSize();

        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case EQE_IO_TEDIT_DONE:
    case SE_SOFTKEY_SAVE:
        EQC_GetTextEditData(EQS_GetNthIOC(MDM_MEMO_DATA), aszMemoData);
        if(STRLEN(aszMemoData))
        {
			l_SaveMemoAddress = MemoAddress;
            SKY_SET_MEMO_TYPE(MemoAddress, MT_MEMO);
            SKY_SET_MEMO_TIME(MemoAddress, SKY_GetLocalTime());
            SKY_SET_MEMO_DATA(MemoAddress, aszMemoData);

            hMessageBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_CONFIRM_SAVE), 
                                         0,//MESSAGE_DEFAULT_TIME, 
                                         MBT_NONE,
                                         MBT_SUCCESS);
            EQC_SET_USERDATA(hMessageBox, pEvent->dwParam);
        }
        else
        {
            hNoData = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_MEMO_MESSG_INPUT_CONTENT),
                                     MESSAGE_DEFAULT_TIME, 
                                     MBT_NONE, 
                                     MBT_HELP);
        }
        break;

    case SE_IO_TEDIT_CANCEL:
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
    		hMessageBox = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MEMO_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO, 
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hMessageBox, SE_KEY_CLEAR);
        }
		else
			EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
    		hMessageBox = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_MEMO_MESSG_Q_SAVE), 
							             MESSAGE_DEFAULT_TIME, 
                                         MBT_YESNO, 
                                         MBT_CONFIRM);
            EQC_SET_USERDATA(hMessageBox, SE_KEY_END);
        }
		else
			EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SE_KEY_CLEAR)
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, SE_KEY_CLEAR);
        else if(pEvent->dwParam == SE_KEY_END)
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, SE_KEY_END);
        else
            EQS_SendEvent((EQS_EVENT_ID)SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(pEvent->dwParam == SE_KEY_CLEAR)
		{
            EQS_END();
		}
        else if(pEvent->dwParam == SE_KEY_END)
		{
            EQS_END_TO(EQS_POP_TO_ROOT);
		}
		else ;

        break;

	case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hNoData)
		{
			EQS_SetIOCActive(EQS_GetNthIOC(MDM_MEMO_DATA));
		}         
        else
        {
            if(pEvent->dwParam == SE_KEY_CLEAR)
                EQS_END();
            else if(pEvent->dwParam == SE_KEY_END)
                EQS_END_TO(EQS_POP_TO_ROOT);
            else
                EQS_END();
        }
		break;
    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

static  BOOL    sSortMemo( INT8  anSortedIdxT[] )
{
    BOOL    fChecked[MAX_MEMO_COUNT], fFind, fSorted;
    UINT8   i,j,k,l;
    static  UINT8   unIdxOfMinValue;
    static  DWORD   dwMinValue;

    fSorted = FALSE;
    for( i=0; i<MAX_MEMO_COUNT; i++ )
    {
        if( SKY_GET_MEMO_TYPE(i) == MT_MEMO )
        {
            fChecked[i] = FALSE;
        }
        else
        {
            fChecked[i] = TRUE;
        }
    }

    k=0;
    for( i=0; i<MAX_MEMO_COUNT; i++ )
    {
        //find the initial small value.
        for( l=0; l<MAX_MEMO_COUNT; l++ )
        {
            if( fChecked[l] == FALSE )
            {
                dwMinValue = SKY_GET_MEMO_TIME(l);
                unIdxOfMinValue = l;
                fSorted = TRUE;
                break;
            }
        }
        if( l == MAX_MEMO_COUNT )
        {
            break;
        }

        //find the smallest value.
        fFind = FALSE;
        for( j=0; j<MAX_MEMO_COUNT; j++ )
        {
            if( fChecked[j] == FALSE )
            {
                if( SKY_GET_MEMO_TIME(j) < dwMinValue )
                {
                    dwMinValue = SKY_GET_MEMO_TIME(j);
                    unIdxOfMinValue = j;
                    fFind = TRUE;
                    //fChecked[j] = TRUE;
                }
            }
        }
        if( fFind )
        {
            anSortedIdxT[k] = unIdxOfMinValue;
            fChecked[unIdxOfMinValue] = TRUE;
        }
        else
        {
            anSortedIdxT[k] = unIdxOfMinValue;
            fChecked[unIdxOfMinValue] = TRUE;
        }
        k++;
    }

    if( fSorted )
    {
        for( i=k; i<MAX_MEMO_COUNT; i++ )
        {
            anSortedIdxT[i] = MEMO_ERROR;
        }
    }

    return  fSorted;
}


#if 0 /* 00000000000000000000000000000000000000000000000000000000000000000000000 */

#endif /* 00000000000000000000000000000000000000000000000000000000000000000000000 */