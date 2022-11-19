/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_vms.c
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
** 2000-12-28   huni     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include <stdlib.h>

#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define FILE_NAME_VMS               SKY_VMS_DIR""SKY_DIR_CHAR"VMS.SKY"

#define INIT_VMS_RECORD_ADDRESS     0
#define INIT_VMS_RECORD_ATTR        0
#define INIT_VMS_RECORD_USED        0
#define INIT_VMS_RECORD_DATE        0
#define INIT_VMS_RECORD_DIALSIZE    0
#define INIT_VMS_RECORD_DIAL        ' '

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
BYTE                g_VmsSize;
vms_buf_type        a_VmsBuffer[MAX_VMS_MSG];
vms_nv_type         VmsRecord;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
STATIC	INT8		sVmsList[MAX_VMS_MSG];
BYTE                *paVmsListBuf[MAX_VMS_MSG];
STATIC  INT8        nVmsIndex;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
static VOID SKY_API sInitVms( VOID );
VOID SKY_SeekReadVmsT( UINT8 unIdxOfVms, vms_nv_type* pReadRecord );
VOID SKY_SeekWriteVmsT( UINT8 unIdxOfVms, vms_nv_type* pWriteRecord );
void sDeleteVmsMessage ( void );

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

///////////////////////////////////////////////////////////////////////////////
//	Initialize list
//
void	sInitVmsList()
{
    vms_buf_type *pVms1, *pVms2;
    DWORD		nDate1, nDate2;
    INT8		i, j;
    BYTE		temp;

    // Retrive used buffer
    for (i = 0, j = 0; i < MAX_VMS_MSG; i++) 
    {
        if (a_VmsBuffer[i].nUsed == 1) 
        {
	        sVmsList[j++] = i;
        }
    }

    g_VmsSize = j;

    // Sort the list
    for (i = 0; i < g_VmsSize - 1; i++) 
    {
        for (j = i + 1; j < g_VmsSize; j++) 
        {
            pVms1 = &a_VmsBuffer[sVmsList[i]];
            nDate1 = (pVms1->nDate[0] << 24) | (pVms1->nDate[1] << 16) | (pVms1->nDate[2] << 8) | pVms1->nDate[3];
            pVms2 = &a_VmsBuffer[sVmsList[j]];
            nDate2 = (pVms2->nDate[0] << 24) | (pVms2->nDate[1] << 16) | (pVms2->nDate[2] << 8) | pVms2->nDate[3];
            if (nDate1 < nDate2) 
            {
                temp = sVmsList[i];
                sVmsList[i] = sVmsList[j];
                sVmsList[j] = temp;
            }
        }
    }

}

INT8 g_VoiceCount()
{
    INT8    bCount, i;

    bCount = 0;

	sInitVmsList();

    for (i = 0; i < MAX_VMS_MSG; i++) 
    {           
        if ((a_VmsBuffer[i].nUsed == 1) &&
	          (a_VmsBuffer[i].nAttr == 0x01)) 
        {       
            // New Voice Mail
            bCount++;
        }
    }

    return bCount;
}


///////////////////////////////////////////////////////////////////////////////
//	Find next free buffer
//
UINT8	sFindFreeVmsBuffer()
{
	UINT8	i;

	sInitVmsList();

	if (g_VmsSize < MAX_VMS_MSG) 
    {
		for (i = 0; i < MAX_VMS_MSG; i++)
			if (a_VmsBuffer[i].nUsed != 1) 
            {
				return i;
			}
	}

	// return oldest buffer
	return sVmsList[MAX_VMS_MSG - 1];
}


///////////////////////////////////////////////////////////////////////////////
//	Add new VMS
//
VOID g_NewVmsSave(UI_COMMAND_U *pCommand)
{
    UINT8			nIndex = sFindFreeVmsBuffer();
//    vms_nv_type*	pVms = &a_VmsBuffer[nIndex];
    static          vms_nv_type	    *pWriteVms;
    DWORD			nDate;
    CHAR			txt[3];
    vms_buf_type    *pVms = &a_VmsBuffer[nIndex];

    pWriteVms = (vms_nv_type*)SKY_Malloc( (SIZE_T)(sizeof(vms_nv_type)) );

    // Set used flag
    pWriteVms->nUsed = 1;
    pVms->nUsed = 1;

    // Copy date
    ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.yr, txt);
    nDate =            atoi(txt);
    ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.mon, txt);
    nDate = nDate*13 + atoi(txt);
    ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.day, txt);
    nDate = nDate*32 + atoi(txt);
    ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.hrs, txt);
    nDate = nDate*24 + atoi(txt);
    ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.min, txt);
    nDate = nDate*60 + atoi(txt);
    ui_sms_bcd_ascii_cvt(pCommand->sms.buf_ptr->data.msg_data.mc_time.sec, txt);
    nDate = nDate*60 + atoi(txt);

    pVms->nDate[0] = pWriteVms->nDate[0] = (nDate >> 24) & 0xFF;
    pVms->nDate[1] = pWriteVms->nDate[1] = (nDate >> 16) & 0xFF;
    pVms->nDate[2] = pWriteVms->nDate[2] = (nDate >>  8) & 0xFF;
    pVms->nDate[3] = pWriteVms->nDate[3] =  nDate        & 0xFF;

    // Copy VMS data
    pWriteVms->nAttr = 0x01;
    pVms->nAttr = 0x01;
    pWriteVms->nDialSize = pCommand->sms.buf_ptr->data.msg_data.call_back.num_fields;
    memcpy((void *)pWriteVms->szDial, (const void *)pCommand->sms.buf_ptr->data.msg_data.call_back.chari, pWriteVms->nDialSize);

    // Update NV
    sInitVmsList();
    SKY_SeekWriteVmsT( nIndex, pWriteVms );
    SKY_Free( pWriteVms );

    // release SMS buffer
    if (pCommand->sms.buf_ptr != NULL)
    {
        q_put(&ui_sms_free_q, &pCommand->sms.buf_ptr->link);
        pCommand->sms.buf_ptr = NULL;
    }
}
///////////////////////////////////////////////////////////////////////////////
//	VMS Menu List
//
void s_MakeVmsList ( void )
{

    STATIC  BYTE    pViewBuf[MAX_VMS_MSG][LCD_COLUME_SIZE - 1];
    STATIC  BYTE    szCallBackNameBuf[LCD_COLUME_SIZE-1];
    STATIC  BYTE    szCallBack[33];

    STATIC  BYTE    s_pszCallBack[MAX_VMS_MSG][33];
    STATIC  BOOL    s_pfCallBackName[MAX_VMS_MSG];  
    BYTE*   paCallBackName[MAX_VMS_MSG];

    INT8            i,  sLineNumber;
    vms_nv_type     ReadVms;
    vms_nv_type     *pReadVms;
    UINT8           nPos, nHyphenpos;

    sLineNumber = 0;
    pReadVms = &ReadVms;

    memset(s_pfCallBackName, NULL, sizeof(BOOL)*MAX_VMS_MSG);
    
    for(i = 0; i < MAX_VMS_MSG; i++)
    {
        memset( (void *)&pViewBuf[i][0], ' ', LCD_COLUME_SIZE - 2 );
        pViewBuf[i][LCD_COLUME_SIZE-2] = 0;        
        memset(s_pszCallBack[i], NULL, sizeof(BYTE)*33); 
        paCallBackName[i] = s_pszCallBack[i];
    }

    for( i = 0; i < g_VmsSize; i++ )
    {
        memset(pReadVms, NULL, sizeof(vms_nv_type));
        SKY_SeekReadVmsT( sVmsList[i], pReadVms );
        memcpy(s_pszCallBack[i], pReadVms->szDial, pReadVms->nDialSize);
    }
    
    FindSMSNameInBook((BYTE **)paCallBackName, s_pfCallBackName, g_VmsSize);

    // Make menu text
    for (i = 0; i < g_VmsSize; i++) {

        memset(szCallBack, NULL, 33);
        memset(szCallBackNameBuf, NULL, LCD_COLUME_SIZE - 1);

        if(  s_pszCallBack[i][0] != NULL )
        {
            if( s_pfCallBackName[i] == TRUE ) 
            {
                if(STRLEN(s_pszCallBack[i]) > LCD_COLUME_SIZE-2)
                {
                    (void) memcpy ( szCallBackNameBuf, 
                                    (const void *) s_pszCallBack[i],
                                    LCD_COLUME_SIZE-2 );
                }
                else
                {
                    (void) memcpy ( szCallBackNameBuf, 
	  			                    (const void *) s_pszCallBack[i], 
                                    STRLEN(s_pszCallBack[i]) );
                }

                (void) memcpy( (void *)&pViewBuf[sLineNumber][0],
                                (const void *)szCallBackNameBuf, LCD_COLUME_SIZE - 2);

            }
            else
            {            
                nPos = FindCodeSet(s_pszCallBack[i]);
                nHyphenpos = FindHypen( s_pszCallBack[i], nPos, 8);  // 8 : Max Length (Except 011, 02 length )

                MakeHypenedNumber( s_pszCallBack[i], szCallBack,
                                   (UINT8)g_GetLength(s_pszCallBack[i], LCD_COLUME_SIZE - 1), 
                                   nPos, 
                                   nHyphenpos );     
                (void) memcpy( (void *)&pViewBuf[sLineNumber][0],
                                (const void *)szCallBack, LCD_COLUME_SIZE - 1);
            }

        }

        paVmsListBuf[sLineNumber] = pViewBuf[sLineNumber];
        sLineNumber++;
    }
}


LOCAL BYTE* s_DisplayVmsInfo( INT8  nIndex)
{

	STATIC  vms_nv_type *pReadVms;
    STATIC  BYTE        szBuf1[LCD_COLUME_SIZE+1];

    BYTE            szBuf2[LCD_COLUME_SIZE+1];
    DWORD           nDate;
    BYTE            nYear, nMonth, nDay, nHour, nMin, nSec;
    UINT8           nPos, nHyphenpos;
    vms_buf_type    *pVms = &a_VmsBuffer[sVmsList[nIndex]];
    BYTE            szTmpStr[32+1];
    BYTE            *TmpBuf;
    INT8            nLen;


    pReadVms = (vms_nv_type*)SKY_Malloc( (SIZE_T)(sizeof(vms_nv_type)) );
    SKY_SeekReadVmsT( sVmsList[nIndex], pReadVms );

    if ( g_VmsSize > 0 )
    {
        memset(szBuf1, NULL, LCD_COLUME_SIZE+1);
        memset(szBuf2, NULL, LCD_COLUME_SIZE + 1);
        
        if (pReadVms->nAttr != 0x02) {
            pReadVms->nAttr = 0x02;
            pVms->nAttr = 0x02;
            SKY_SeekWriteVmsT( sVmsList[nIndex], pReadVms );
            ui_sms_update_icons();
        }

	    nDate  = (pReadVms->nDate[0] << 24) | (pReadVms->nDate[1] << 16) | (pReadVms->nDate[2] << 8) | pReadVms->nDate[3];
	    nSec   = nDate%60;	nDate = nDate/60;
	    nMin   = nDate%60;	nDate = nDate/60;
	    nHour  = nDate%24;	nDate = nDate/24;
	    nDay   = nDate%32;	nDate = nDate/32;
	    nMonth = nDate%13;	nDate = nDate/13;
	    nYear  = nDate;

        if(nHour == 0)
        {
 	        sprintf( (char*)szBuf1, "%2d%s %2d%s %2d:%02d%cM",
		                nMonth, SKY_GET_TOKEN(TKN_DAT_MONTH), nDay, SKY_GET_TOKEN(TKN_DAT_DAY), nHour, nMin, (nHour < 12) ? 'A' : 'P');           
        }
        else
        {            
	        sprintf( (char*)szBuf1, "%2d%s %2d%s %2d:%02d%cM",
		                nMonth, SKY_GET_TOKEN(TKN_DAT_MONTH), nDay, SKY_GET_TOKEN(TKN_DAT_DAY), ((nHour + 23) % 12) + 1, nMin, (nHour < 12) ? 'A' : 'P');
        }

        memset(szTmpStr, 0, 32);
        memcpy((void *)szTmpStr, (const void *)pReadVms->szDial, pReadVms->nDialSize);

        if( ( TmpBuf = FindNumber((BYTE*)szTmpStr) ) != NULL )
        {
            (void) memcpy ( szBuf2, 
	  			            (const void *) TmpBuf, 
                            STRLEN(TmpBuf) );
        }
        else
        {
            memset(szTmpStr, 0, 32);
            nLen = ( pReadVms->nDialSize > 16 ) ? 16 : pReadVms->nDialSize;
            memcpy((void *)szTmpStr, (const void *)pReadVms->szDial, nLen);
            nPos = FindCodeSet(szTmpStr);
            nHyphenpos = FindHypen( szTmpStr, nPos, 8);  // 8 : Max Length (Except 011, 02 length )

            MakeHypenedNumber( szTmpStr, szBuf2,
                               (UINT8)g_GetLength(szTmpStr, LCD_COLUME_SIZE-2), 
                               nPos, 
                               nHyphenpos );
        }
        SKY_Free( pReadVms );

        return szBuf1;
    }
}

BOOL    gfIsThereVms()
{

    sInitVmsList();

    if (g_VmsSize > 0)
        return TRUE;
    else
        return FALSE;

}

VOID SKA_VoiceMessage(SKY_EVENT_T* pEvent)
{
    STATIC  INT8        sWindowSize;
    STATIC  INT8        nCallBackLength;
    STATIC  BYTE        szCallBackBuf[LCD_COLUME_SIZE-1];
    STATIC  HCONTROL    s_hHnd;

    HCONTROL            hMsg;
    vms_nv_type*	    pReadVms;

    SKY_TOKEN paVmsPopMenu[]=
    {
        TKN_SMS_TEXT_DELETE,            
        TKN_SMS_DELETE_ALL   //  전부 삭제
    };


    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        sInitVmsList();

        if (g_VmsSize == 0) 
        {
            EQS_END();
            break;
        }
        else
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                200, // SKY_DEF_WIN_DY, 
                CL_MENU_SORISAM_NONE,
                SKY_GET_TOKEN(TKN_SMS_SUB_MENU2), 
                WA_SHOW_CML | WA_SHOW_HEADER);

            s_MakeVmsList();
            nVmsIndex = 0;
            sWindowSize =  5;
            s_hHnd = EQC_ScrollList(EQS_NA, EQS_NA, 18, sWindowSize, paVmsListBuf, g_VmsSize, g_VmsSize, 0); 
            EQC_SET_STYLE( s_hHnd, EQC_GET_STYLE(s_hHnd) |
                                CS_SCROLL_LIST_DEFAULT |
                                CS_SCROLL_LIST_ENUMBERATED |
                                CS_SCROLL_LIST_NO_CML |
                                CS_SCROLL_LIST_MAXIMIZE |
                                CS_SCROLL_LIST_STATUS_BAR );

            EQC_SetScrollStatusBarText(s_hHnd, s_DisplayVmsInfo( nVmsIndex ));
            EQS_SetIOCActive( s_hHnd );

            EQS_FitCanvasSize();    
        }
        break;
        
    case SE_APP_END :     /* The very last event */
        ui_sms_update_icons();
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_DRAW :    /* draw */
        break;


    case SE_IO_SCROLL_LIST_SELECT:
    case SE_SOFTKEY_SORISAM:
        EQC_SET_FOCUS(s_hHnd, FALSE);
        nVmsIndex = (INT8)pEvent->dwParam;

        memset(szCallBackBuf, 0, LCD_COLUME_SIZE-2);
        szCallBackBuf[LCD_COLUME_SIZE-2] = 0;

        /* jrkwon 2001.11.21 --> 신세기,SK텔레콤 음성사서함 처리 */
        if(SKY_GET_SERVICE_PROVIDER() == SP_STI) 
        {
            nCallBackLength = STRLEN(SKY_GET_S017_VOICE_MAIL_CONNECT());
            memcpy((char *)szCallBackBuf, (BYTE*)SKY_GET_S017_VOICE_MAIL_CONNECT(), nCallBackLength); // 신세기
        }
        else /* if(SKY_GET_SERVICE_PROVIDER() == SP_SKT) or NO DIR_NUM */
        {
            nCallBackLength = STRLEN(SKY_GET_011SERVICE_SORISAM_CON());
            memcpy((char *)szCallBackBuf, (BYTE*)SKY_GET_011SERVICE_SORISAM_CON(), nCallBackLength); // SK텔레콤
        }
        /* <-- */

        SET_NUMEDIT_BUFFER(szCallBackBuf, nCallBackLength);
        SET_NUMEDIT_POS(nCallBackLength);                

        EQS_START_SIBLING(SKA_StartOrigination, FALSE);  
        break;

    case SE_IO_SCROLL_LIST_CHG:
        nVmsIndex = EQC_GetScrollPhysicalIndex(s_hHnd);
        EQC_SetScrollStatusBarText(s_hHnd, s_DisplayVmsInfo( nVmsIndex ));
        break;

    case SE_KEY_SEND :
        nVmsIndex = EQC_GetScrollPhysicalIndex(s_hHnd);
        pReadVms = (vms_nv_type*)SKY_Malloc( (SIZE_T)(sizeof(vms_nv_type)) );
        SKY_SeekReadVmsT( sVmsList[nVmsIndex], pReadVms );

        memset(szCallBackBuf, 0, LCD_COLUME_SIZE-2);
        szCallBackBuf[LCD_COLUME_SIZE-2] = 0;

        if(pReadVms->nDialSize > 0) {
            memcpy((char *)szCallBackBuf, (const void *)pReadVms->szDial, pReadVms->nDialSize);
            nCallBackLength = pReadVms->nDialSize;		
        } 
        else {
            /* jrkwon 2001.11.21 --> 신세기,SK텔레콤 음성사서함 처리 */
            if(SKY_GET_SERVICE_PROVIDER() == SP_STI) 
            {
                nCallBackLength = STRLEN(SKY_GET_S017_VOICE_MAIL_CONNECT());
                memcpy((char *)szCallBackBuf, (BYTE*)SKY_GET_S017_VOICE_MAIL_CONNECT(), nCallBackLength); // 신세기
            }
            else /* if(SKY_GET_SERVICE_PROVIDER() == SP_SKT) or NO DIR_NUM */
            {
                nCallBackLength = STRLEN(SKY_GET_011SERVICE_SORISAM_CON());
                memcpy((char *)szCallBackBuf, (BYTE*)SKY_GET_011SERVICE_SORISAM_CON(), nCallBackLength); // SK텔레콤
            }
            /* <-- */
        }

        SKY_Free( pReadVms );

        SET_NUMEDIT_BUFFER(szCallBackBuf, nCallBackLength);
        SET_NUMEDIT_POS(nCallBackLength);                
                
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX*)paVmsPopMenu, 2, 0);
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam)
        {
        case 0:
            nVmsIndex = EQC_GetScrollPhysicalIndex(s_hHnd);
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_VMS_MESSAGE_02), 
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);
            break;
        case 1:
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_VMS_MESSAGE_04), 
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);
            break;
        }
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            sDeleteVmsMessage();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_VMS_MESSAGE_03), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);                       
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            gDeleteAllVms();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_VMS_MESSAGE_05), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS); 
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);
        }
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            sInitVmsList();

            if (g_VmsSize > 0) 
            {
                EQC_DeleteScrollData(s_hHnd, nVmsIndex);
                nVmsIndex =  EQC_GetScrollPhysicalIndex(s_hHnd); ; 
                EQC_SetScrollStatusBarText(s_hHnd, s_DisplayVmsInfo( nVmsIndex ));
                EQS_SetIOCActive( s_hHnd );
            }        
            else
            {
                EQS_END();
            }
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            EQS_END();
        }
        break;

    case SE_SMS_MESSAGE:
        EQS_DeleteControl(s_hHnd);

        SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());  
        
        s_MakeVmsList();
        nVmsIndex = 0;
        s_hHnd = EQC_ScrollList(EQS_NA, EQS_NA, 18, EQS_NA, paVmsListBuf, g_VmsSize, g_VmsSize, 0); 
        EQC_SET_STYLE( s_hHnd, EQC_GET_STYLE(s_hHnd) |
                            CS_SCROLL_LIST_DEFAULT |
                            CS_SCROLL_LIST_ENUMBERATED |
                            CS_SCROLL_LIST_NO_CML |
                            CS_SCROLL_LIST_MAXIMIZE |
                            CS_SCROLL_LIST_STATUS_BAR );

        EQC_SetScrollStatusBarText(s_hHnd, s_DisplayVmsInfo( nVmsIndex ));
        EQS_SetIOCActive( s_hHnd );
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam == EQC_DELETED)
            break;

    case SE_KEY_CLEAR:
        EQS_END();
        break;

    default :
		SKY_DefEventProc(pEvent);
        break;
    }
}

void sDeleteVmsMessage ( void )
{
	static          vms_nv_type	    *pReadVms;
    vms_buf_type    *pVms;

    pReadVms = (vms_nv_type*)SKY_Malloc( (SIZE_T)(sizeof(vms_nv_type)) );

	//	Delete EFS memory
    pReadVms->nAddress          = INIT_VMS_RECORD_ADDRESS;
    pReadVms->nAttr             = INIT_VMS_RECORD_ATTR;
    pReadVms->nUsed             = INIT_VMS_RECORD_USED;
    pReadVms->nDate[0]          = INIT_VMS_RECORD_DATE;
    pReadVms->nDate[1]          = INIT_VMS_RECORD_DATE;
    pReadVms->nDate[2]          = INIT_VMS_RECORD_DATE;
    pReadVms->nDate[3]          = INIT_VMS_RECORD_DATE;
    pReadVms->nDialSize         = INIT_VMS_RECORD_DIALSIZE;
    memset(pReadVms->szDial, INIT_VMS_RECORD_DIAL, MAX_SMS_DIAL); 

    SKY_SeekWriteVmsT( sVmsList[nVmsIndex], pReadVms );

	//	Delete RAM memory
	pVms = (vms_buf_type*)&a_VmsBuffer[sVmsList[nVmsIndex]];
	pVms->nUsed = INIT_VMS_RECORD_USED;
    pVms->nAttr = INIT_VMS_RECORD_ATTR;

	sInitVmsList();
    SKY_Free( pReadVms );

	ui_sms_update_icons();

}


/////////////////////////////////////////////////////////////////////////////////////////
////        VMS EFS
/////////////////////////////////////////////////////////////////////////////////////////

VOID SKY_API SKY_InitVMS(VOID)
{
    HFILE   hFile;
    DWORD   dwCount;
    INT8    i;

    if(SKY_IsFileOrDirExist(SKY_VMS_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_VMS_DIR);
    }

    /* open the file */
    hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        /* set the initial value */
        sInitVms();
    }
    else
    {
        /* if file is already exist, read the data into the global buffer */
        for(i = 0; i < MAX_VMS_MSG; i++)
        {
            dwCount = SKY_ReadFile(hFile, &(VmsRecord), sizeof(VmsRecord));
            a_VmsBuffer[i].nAttr        = VmsRecord.nAttr;
            a_VmsBuffer[i].nUsed        = VmsRecord.nUsed;
            a_VmsBuffer[i].nDate[0]     = VmsRecord.nDate[0];
            a_VmsBuffer[i].nDate[1]     = VmsRecord.nDate[1];
            a_VmsBuffer[i].nDate[2]     = VmsRecord.nDate[2];
            a_VmsBuffer[i].nDate[3]     = VmsRecord.nDate[3];
//            a_VmsBuffer[i].nDialSize    = VmsRecord.nDialSize;
//            memcpy(a_VmsBuffer[i].szDial, VmsRecord.szDial, MAX_SMS_DIAL); 
            a_VmsBuffer[i].nAddress  = i;
        }
        /* close the file */
        SKY_CloseFile(hFile);
    }
    ui_sms_update_icons();
}

VOID SKY_API sInitVms( VOID )
{
    INT8    i;
    HFILE   hFile;
    DWORD   dwCount;

    /* jrkown : 2000.01.20 
    ** when try to open a file just after closing the file,
    ** fatal error occurs. --> change the method 
    */
    if(SKY_IsFileOrDirExist(FILE_NAME_VMS) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        VmsRecord.nAddress       = INIT_VMS_RECORD_ADDRESS;
        VmsRecord.nAttr          = INIT_VMS_RECORD_ATTR;
        VmsRecord.nUsed          = INIT_VMS_RECORD_USED;
        VmsRecord.nDate[0]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[1]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[2]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[3]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDialSize      = INIT_VMS_RECORD_DIALSIZE;
        memset(VmsRecord.szDial, INIT_VMS_RECORD_DIAL, MAX_SMS_DIAL); 

        for(i = 0; i < MAX_VMS_MSG; i++)
        {
            SKY_WriteFile(hFile, &(VmsRecord), sizeof(VmsRecord));

            a_VmsBuffer[i].nAttr        = VmsRecord.nAttr;
            a_VmsBuffer[i].nUsed        = VmsRecord.nUsed;
            a_VmsBuffer[i].nDate[0]     = VmsRecord.nDate[0];
            a_VmsBuffer[i].nDate[1]     = VmsRecord.nDate[1];
            a_VmsBuffer[i].nDate[2]     = VmsRecord.nDate[2];
            a_VmsBuffer[i].nDate[3]     = VmsRecord.nDate[3];
            a_VmsBuffer[i].nAddress  = i;
        }
        SKY_CloseFile(hFile);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        /* if file is already exist, read the data into the global buffer */
        for(i = 0; i < MAX_VMS_MSG; i++)
        {
            dwCount = SKY_ReadFile(hFile, &(VmsRecord), sizeof(VmsRecord));
            a_VmsBuffer[i].nAttr        = VmsRecord.nAttr;
            a_VmsBuffer[i].nUsed        = VmsRecord.nUsed;
            a_VmsBuffer[i].nDate[0]     = VmsRecord.nDate[0];
            a_VmsBuffer[i].nDate[1]     = VmsRecord.nDate[1];
            a_VmsBuffer[i].nDate[2]     = VmsRecord.nDate[2];
            a_VmsBuffer[i].nDate[3]     = VmsRecord.nDate[3];
            a_VmsBuffer[i].nAddress  = i;
        }

        SKY_CloseFile(hFile);
    }
}

VOID SKY_SeekReadVmsT( UINT8 unIdxOfVms, vms_nv_type* pReadRecord )
{
    HFILE   hFile;
    DWORD   dwCount;

    hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    dwCount = SKY_SeekReadFile( 
                hFile, FS_SO_SET, unIdxOfVms*sizeof(vms_nv_type), pReadRecord, sizeof(vms_nv_type) );
    SKY_CloseFile( hFile );
}


VOID SKY_SeekWriteVmsT( UINT8 unIdxOfVms, vms_nv_type* pWriteRecord )
{

    HFILE hFile;
    DWORD dwCount;

    hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_CREATE);
    }
    dwCount = SKY_SeekWriteFile( 
                hFile, FS_SO_SET, unIdxOfVms*sizeof(vms_nv_type), pWriteRecord, sizeof(vms_nv_type) );
    SKY_CloseFile( hFile );
}

VOID SKY_API gDeleteAllVms( VOID )
{
    INT8    i;
    HFILE   hFile;

    if(SKY_IsFileOrDirExist(FILE_NAME_VMS) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        VmsRecord.nAddress       = INIT_VMS_RECORD_ADDRESS;
        VmsRecord.nAttr          = INIT_VMS_RECORD_ATTR;
        VmsRecord.nUsed          = INIT_VMS_RECORD_USED;
        VmsRecord.nDate[0]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[1]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[2]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[3]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDialSize      = INIT_VMS_RECORD_DIALSIZE;
        memset(VmsRecord.szDial, INIT_VMS_RECORD_DIAL, MAX_SMS_DIAL); 

        for(i = 0; i < MAX_VMS_MSG; i++)
        {
            SKY_WriteFile(hFile, &(VmsRecord), sizeof(VmsRecord));

            a_VmsBuffer[i].nAttr        = VmsRecord.nAttr;
            a_VmsBuffer[i].nUsed        = VmsRecord.nUsed;
            a_VmsBuffer[i].nDate[0]     = VmsRecord.nDate[0];
            a_VmsBuffer[i].nDate[1]     = VmsRecord.nDate[1];
            a_VmsBuffer[i].nDate[2]     = VmsRecord.nDate[2];
            a_VmsBuffer[i].nDate[3]     = VmsRecord.nDate[3];
            a_VmsBuffer[i].nAddress  = i;
        }
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_VMS, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        VmsRecord.nAddress       = INIT_VMS_RECORD_ADDRESS;
        VmsRecord.nAttr          = INIT_VMS_RECORD_ATTR;
        VmsRecord.nUsed          = INIT_VMS_RECORD_USED;
        VmsRecord.nDate[0]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[1]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[2]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDate[3]       = INIT_VMS_RECORD_DATE;
        VmsRecord.nDialSize      = INIT_VMS_RECORD_DIALSIZE;
        memset(VmsRecord.szDial, INIT_VMS_RECORD_DIAL, MAX_SMS_DIAL); 

        for(i = 0; i < MAX_VMS_MSG; i++)
        {
            SKY_WriteFile(hFile, &(VmsRecord), sizeof(VmsRecord));

            a_VmsBuffer[i].nAttr        = VmsRecord.nAttr;
            a_VmsBuffer[i].nUsed        = VmsRecord.nUsed;
            a_VmsBuffer[i].nDate[0]     = VmsRecord.nDate[0];
            a_VmsBuffer[i].nDate[1]     = VmsRecord.nDate[1];
            a_VmsBuffer[i].nDate[2]     = VmsRecord.nDate[2];
            a_VmsBuffer[i].nDate[3]     = VmsRecord.nDate[3];
            a_VmsBuffer[i].nAddress  = i;
        }
    }

	ui_sms_update_icons();
    g_VmsSize = 0;

    SKY_CloseFile(hFile);
}