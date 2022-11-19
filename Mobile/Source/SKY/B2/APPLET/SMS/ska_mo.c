/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_mo.c
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
** 2000-12-25 reo     Created.
**ㅇ
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

//FEATURE_UASMS
#include "uixuasms.h"
#include "uasms.h"
#include "uasmsi.h" 
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define FILE_NAME_MOSEND            SKY_SMSMO_DIR""SKY_DIR_CHAR"MOSEND.SKY"
#define FILE_NAME_MOSENT            SKY_SMSMO_DIR""SKY_DIR_CHAR"MOSENT.SKY"
#define FILE_NAME_MOSAVE            SKY_SMSMO_DIR""SKY_DIR_CHAR"MOSAVE.SKY"

/* SKA_SMSEditMessage 에서 사용되는 MESGBOX USER DATA */
#define NO_OTHER_PARTY_CALL_NUM         0x01
#define MSG_EDIT_CANCEL                 0x02
#define TEXT_EDIT_POPUP_END             0x04
#define NO_MSG_CONTENT_IN_SAVEBOX       0x08
#define NOT_MORE_MOSAVE                 0x10
#define FINISH_MAKE_MOSAVE              0x20
#define ONLY_ADD_DONGBO                 0x40

#define NUM_OF_MSG_KIND     3
#define NUM_OF_SEND_RETRIAL 2

#define MO_MSG_LANG_ENGLISH    0   //7bit ascii
#define MO_MSG_LANG_KOREAN     1


#define MESG_DELETE_CONFIRM     0x10

/* BMP or Animation Position */
// huni 2001.8.9 SKY_ANNUNCIATORS_HEIGHT     19
#define SMS_SEND_ANI_X  19 // (20 -> 19)
#define SMS_SEND_ANI_Y  27 - SKY_ANNUNCIATORS_HEIGHT // (24 -> 27)
#define SMS_SEND_STATUS_TITLE_Y   76 - SKY_ANNUNCIATORS_HEIGHT 
#define SMS_SEND_STATUS_Y     102 - SKY_ANNUNCIATORS_HEIGHT

//#define SMS_SEND_FRAME_X          4
//#define SMS_SEND_FRAME_Y          94 - SKY_ANNUNCIATORS_HEIGHT

#define SMS_SEND_FRAME_X          20
#define SMS_SEND_FRAME_Y          83 - SKY_ANNUNCIATORS_HEIGHT

#define SMS_READ_VALUE          1

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagSMS_SENDING{  //SS 
    SSA_SEND_ANIMATION,
    SSA_SEND_IMAGE, // SSA_FRAME_TITLE,	// 
    SSA_FRAME_TITLE,
    SSA_FRAME_STATUS_1,
    SSA_FRAME_STATUS_2,
//    SSA_FRAME_STATUS_3,
    SSA_MAX
};


enum tagSMS_VIEW_MOSAVE_E { //SVMSAVE
    SVMSAVE_DATE_TITLE,  
    SVMSAVE_DATE,
    SVMSAVE_MESG_TITLE,
    SVMSAVE_MESG,
    SVMSAVE_MAX
};

enum tagSMS_VIEW_MOSENT_E { //SVMSENT
    SVMSENT_DATE_TITLE,
    SVMSENT_DATE,
    SVMSENT_MESG_TITLE,
    SVMSENT_MESG,
    SVMSENT_RCV_NUM_TITLE,
    SVMSENT_RCV_NUM,
    SVMSENT_SEND_NUM_TITLE,
    SVMSENT_SEND_NUM,
    SVMSENT_MAX
};

enum tagSMS_VIEW_MOSEND_E { //SVMSEND
    SVMSEND_DATE_TITLE,
    SVMSEND_DATE,
    SVMSEND_MESG_TITLE,
    SVMSEND_MESG,
    SVMSEND_RCV_NUM_TITLE,
    SVMSEND_RCV_NUM,
    SVMSEND_SEND_NUM_TITLE,
    SVMSEND_SEND_NUM,
    SVMSEND_MAX
};

//메시지 재구성 
// huni modify 01.9.4 For Dong-Bo
enum tagSMS_EDIT_MSG_E {  //SEM 
    SEM_MESG_CONTENT_TITLE,
    SEM_MESG_CONTENT,
    SEM_MESG_KIND_TITLE,
    SEM_MESG_KIND,
    SEM_MY_NUM_TITLE,
    SEM_MY_NUM,
    SEM_OTHER_PARTY_NUM_TITLE,
    SEM_OTHER_BLACK_NUM01,
    SEM_OTHER_PARTY_NUM01,
    SEM_OTHER_BLACK_NUM02,
    SEM_OTHER_PARTY_NUM02,
    SEM_OTHER_BLACK_NUM03,
    SEM_OTHER_PARTY_NUM03,
    SEM_OTHER_BLACK_NUM04,
    SEM_OTHER_PARTY_NUM04,
    SEM_OTHER_BLACK_NUM05,
    SEM_OTHER_PARTY_NUM05,
    SEM_OTHER_BLACK_NUM06,
    SEM_OTHER_PARTY_NUM06,
    SEM_OTHER_BLACK_NUM07,
    SEM_OTHER_PARTY_NUM07,
    SEM_OTHER_BLACK_NUM08,
    SEM_OTHER_PARTY_NUM08,
    SEM_OTHER_BLACK_NUM09,
    SEM_OTHER_PARTY_NUM09,
    SEM_OTHER_BLACK_NUM10,
    SEM_OTHER_PARTY_NUM10,
    SEM_OTHER_BLACK_NUM11,
    SEM_OTHER_PARTY_NUM11,
    SEM_OTHER_BLACK_NUM12,
    SEM_OTHER_PARTY_NUM12,
    SEM_OTHER_BLACK_NUM13,
    SEM_OTHER_PARTY_NUM13,
    SEM_OTHER_BLACK_NUM14,
    SEM_OTHER_PARTY_NUM14,
    SEM_OTHER_BLACK_NUM15,
    SEM_OTHER_PARTY_NUM15,
    SEM_OTHER_BLACK_NUM16,
    SEM_OTHER_PARTY_NUM16,
    SEM_OTHER_BLACK_NUM17,
    SEM_OTHER_PARTY_NUM17,
    SEM_OTHER_BLACK_NUM18,
    SEM_OTHER_PARTY_NUM18,
    SEM_OTHER_BLACK_NUM19,
    SEM_OTHER_PARTY_NUM19,
    SEM_OTHER_BLACK_NUM20,
    SEM_OTHER_PARTY_NUM20, 
    SEM_MAX
};
/*
enum tagSMS_EDIT_MSG_E {  //SEM 
    SEM_MESG_CONTENT_TITLE,
    SEM_MESG_CONTENT,
    SEM_OTHER_PARTY_NUM_TITLE,
    SEM_OTHER_PARTY_NUM,
    SEM_MY_NUM_TITLE,
    SEM_MY_NUM,
    SEM_MESG_KIND_TITLE,
    SEM_MESG_KIND,
    SEM_MAX
};
*/
enum tagSMS_CALL_MSG_E {   //SCM 보낼메시지 
    SCM_SCROLL_LIST_1,
    SCM_MAX
};

enum tagSMS_MOSENT_MSG_E {  //SMM 보낸메시지 
    SMM_SCROLL_LIST_1,
    SMM_MAX
};

enum tagSMS_MOSAVE_MSG_E {  //SMS 문장보관
    SMS_SCROLL_LIST_1,
    SMS_MAX
};

enum tagSMS_MAKE_MOSAVE_E {   //SMMS 새 문장    
    SMMS_MAKE_MOSAVE_TITLE,
    SMMS_MAKE_MOSAVE,
    SMMS_MAX
};    

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

SMSMO_UI_T                              g_aSMSSendMsg[MAX_SEND_MSG]; // huni send 30 (20 ->30)
SMSMO_UI_T                              g_aSMSSentMsg[MAX_SENT_MSG]; // huni sent 30 (20 ->40 ->30)
SMSMO_UI_T                              g_aSMSSaveMsg[MAX_SMS_MO];

UINT8                                   g_nMOSendSize = 0;
UINT8                                   g_nMOSentSize = 0;
UINT8                                   g_nMOSaveSize = 0;

SMSMO_SEND_T                            g_UIMOBuffer;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

LOCAL SMSMO_SEND_T                      l_MOSendBuf;        //initialize에 사용 
LOCAL SMSMO_SEND_T                      l_MOSentBuf;
LOCAL SMSMO_SEND_T                      l_MOSaveBuf;

LOCAL UINT8                             l_unTrial = 0;        // SMS retrial 횟수
LOCAL BYTE*                             l_pszMsgKind[NUM_OF_MSG_KIND];
LOCAL SMSMO_SEND_T*                     l_pCurViewElement;

LOCAL BYTE*                             l_paViewBuf[MAX_SENT_MSG]; // huni sent 30 (20 ->40 ->30)

LOCAL UINT8                             l_nMOSendDelIndex = 0;

// huni add 01.9.4 For Dong-Bo 
LOCAL SMSMO_DONGBO_SEND_T               l_DongboSendBuf;

STATIC              UINT8               g_nDongboLast; // Last Dongbo Data
STATIC              UINT8               g_nDongboNow;  // Now RcvCallBack Data
STATIC              UINT8               g_nDongboTotal;  // Now RcvCallBack Data

LOCAL               BYTE                szTmpStr[LCD_COLUME_SIZE+1];

//STATIC              UINT8               g_nDongboLast;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL VOID sInitSMSMOSent(VOID);
LOCAL VOID sInitSMSMOSentMsg(VOID);
LOCAL VOID sInitSMSMOSend(VOID);
LOCAL VOID sInitSMSMOSendMsg(VOID);
LOCAL VOID sInitSMSMOSave(VOID);
LOCAL VOID sInitSMSMOSaveMsg(VOID);

LOCAL VOID SKY_API sSKA_MOSentMsg(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_MOSendMsg(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_MOSaveMsg(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_ViewMOSentMsg(SKY_EVENT_T *pEvent);
LOCAL VOID SKY_API sSKA_ViewMOSaveMsg(SKY_EVENT_T *pEvent);

VOID FillSMSMOSendMsg(BYTE* pNumber, BYTE* pMsg, MO_SEND_TYPE_E type) // 수신->발신 data fill
{
    memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));    
    if(type == MO_SEND_REPLY)
    {
        STRCPY(g_UIMOBuffer.pszRcvNumber, pNumber);
        g_UIMOBuffer.nRcvNumberSize = STRLEN(g_UIMOBuffer.pszRcvNumber);
    }
    else if(type == MO_SEND_FORWARD)
    {
        STRCPY(g_UIMOBuffer.pszMsg, pMsg);
        g_UIMOBuffer.nMsgSize = STRLEN(g_UIMOBuffer.pszMsg);
    }        

    /* 자기 번호를 얻는 함수 ?*/
    STRCPY(g_UIMOBuffer.pszSendNumber, GetMyNumber());
    g_UIMOBuffer.nSendNumberSize= STRLEN(g_UIMOBuffer.pszSendNumber);    
    
}
//////////////////////////////////////////////////////////////////////
//           SMS MO Save EFS

VOID DeleteAllSaveMsg(VOID)
{
    INT8    i;
    HFILE   hFile;

    if(SKY_IsFileOrDirExist(FILE_NAME_MOSAVE) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    /* initialize */
    memset(&l_MOSaveBuf, NULL, sizeof(SMSMO_SEND_T));
    g_nMOSaveSize = 0;

    SKY_WriteFile(hFile, &g_nMOSaveSize, sizeof(MO_FILE_HEADER));  
    for(i = 0; i < MAX_SMS_MO; i++)
    {
        SKY_WriteFile(hFile, &(l_MOSaveBuf), sizeof(l_MOSaveBuf));
        g_aSMSSaveMsg[i].nAddr = i;
        g_aSMSSaveMsg[i].nAttr = l_MOSaveBuf.nAttr;
        g_aSMSSaveMsg[i].dwMadeTime = l_MOSaveBuf.dwMadeTime;
    }
    SKY_CloseFile(hFile);
}
    
LOCAL VOID sInitSMSMOSave(VOID)
{
    INT8            i;
    HFILE           hFile;
    UINT8           nUsedCnt, nUnusedCnt;

    nUsedCnt = nUnusedCnt = 0;
    if(SKY_IsFileOrDirExist(FILE_NAME_MOSAVE) == FALSE)
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);


        /* initialize */
        memset(&l_MOSaveBuf, NULL, sizeof(SMSMO_SEND_T));
        g_nMOSaveSize = 0;

        SKY_WriteFile(hFile, &g_nMOSaveSize, sizeof(MO_FILE_HEADER));  
        for(i = 0; i < MAX_SMS_MO; i++)
        {
            SKY_WriteFile(hFile, &(l_MOSaveBuf), sizeof(l_MOSaveBuf));
            g_aSMSSaveMsg[i].nAddr = i;
            g_aSMSSaveMsg[i].nAttr = l_MOSaveBuf.nAttr;
            g_aSMSSaveMsg[i].dwMadeTime = l_MOSaveBuf.dwMadeTime;
        }
        SKY_CloseFile(hFile);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        memset(&l_MOSaveBuf, NULL, sizeof(SMSMO_SEND_T));
        SKY_ReadFile(hFile, &g_nMOSaveSize, sizeof(MO_FILE_HEADER));
        if(g_nMOSaveSize != 0)
        {
            for(i = 0; i < MAX_SMS_MO; i++)
            {
                SKY_ReadFile(hFile, &(l_MOSaveBuf), sizeof(l_MOSaveBuf));

                if(l_MOSaveBuf.nAttr & SMS_USED)
                {
                    g_aSMSSaveMsg[nUsedCnt].nAddr = i;
                    g_aSMSSaveMsg[nUsedCnt].nAttr = l_MOSaveBuf.nAttr;
                    g_aSMSSaveMsg[nUsedCnt].dwMadeTime = l_MOSaveBuf.dwMadeTime;
                    nUsedCnt++;
                }
                else
                {
                    g_aSMSSaveMsg[MAX_SMS_MO-1-nUnusedCnt].nAddr = i;
                    g_aSMSSaveMsg[MAX_SMS_MO-1-nUnusedCnt].nAttr = l_MOSaveBuf.nAttr;
                    g_aSMSSaveMsg[MAX_SMS_MO-1-nUnusedCnt].dwMadeTime = l_MOSaveBuf.dwMadeTime;
                    nUnusedCnt++;
                }                
            }
        }
        /* close the file */
        SKY_CloseFile(hFile);         
    }
}
    
LOCAL VOID sInitSMSMOSaveMsg(VOID)
{
    HFILE   hFile;
    UINT8   i, nUsedCnt, nUnusedCnt;

    nUsedCnt = nUnusedCnt = 0;
    if(SKY_IsFileOrDirExist(SKY_SMSMO_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_SMSMO_DIR);
    }
    /* open the file */
    hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        sInitSMSMOSave();
    }
    else
    {
        memset(&l_MOSaveBuf, NULL, sizeof(SMSMO_SEND_T));
        SKY_ReadFile(hFile, &g_nMOSaveSize, sizeof(MO_FILE_HEADER));
        if(g_nMOSaveSize != 0)
        {
            for(i = 0; i < MAX_SMS_MO; i++)
            {
                SKY_ReadFile(hFile, &(l_MOSaveBuf), sizeof(l_MOSaveBuf));

                if(l_MOSaveBuf.nAttr & SMS_USED)
                {
                    g_aSMSSaveMsg[nUsedCnt].nAddr = i;
                    g_aSMSSaveMsg[nUsedCnt].nAttr = l_MOSaveBuf.nAttr;
                    g_aSMSSaveMsg[nUsedCnt].dwMadeTime = l_MOSaveBuf.dwMadeTime;
                    nUsedCnt++;
                }
                else
                {
                    g_aSMSSaveMsg[MAX_SMS_MO-1-nUnusedCnt].nAddr = i;
                    g_aSMSSaveMsg[MAX_SMS_MO-1-nUnusedCnt].nAttr = l_MOSaveBuf.nAttr;
                    g_aSMSSaveMsg[MAX_SMS_MO-1-nUnusedCnt].dwMadeTime = l_MOSaveBuf.dwMadeTime;
                    nUnusedCnt++;
                }                
            }
        }
        /* close the file */
        SKY_CloseFile(hFile);        
    }    
}

VOID SKY_SeekReadMOSaveT( UINT8 nIndex, SMSMO_SEND_T* pReadRecord)
{
    HFILE   hFile;
    DWORD   dwCount;

    hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_READWRITE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    dwCount = SKY_SeekReadFile( 
                hFile, FS_SO_SET, nIndex*sizeof(SMSMO_SEND_T) + sizeof(MO_FILE_HEADER),  // UINT8 : header size                
                pReadRecord, sizeof(SMSMO_SEND_T) );
    SKY_CloseFile( hFile );
}

VOID SKY_SeekWriteMOSaveT( UINT8 nIndex, SMSMO_SEND_T* pWriteRecord, MO_FILE_HEADER* pUsedNum)
{

    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MOSAVE) == FALSE)
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    SKY_WriteFile(hFile, pUsedNum, sizeof(MO_FILE_HEADER));
    dwCount = SKY_SeekWriteFile( 
                hFile, FS_SO_SET, nIndex*sizeof(SMSMO_SEND_T) + sizeof(MO_FILE_HEADER),
                pWriteRecord, sizeof(SMSMO_SEND_T) );
    SKY_CloseFile( hFile );
}

LOCAL VOID sSortMOSaveUIBuffer(VOID)
{
    UINT8       i, j;
    SMSMO_UI_T  Temp;

	for (i = 0; i < g_nMOSaveSize - 1; i++) 
    {
		for (j = i + 1; j < g_nMOSaveSize; j++) 
        {
			if (g_aSMSSaveMsg[i].dwMadeTime < g_aSMSSaveMsg[j].dwMadeTime) 
            {
				Temp = g_aSMSSaveMsg[i];
				g_aSMSSaveMsg[i] = g_aSMSSaveMsg[j];
				g_aSMSSaveMsg[j] = Temp;
			}
		}
	}    
}

VOID SaveSMSMOSaveMsg(SMSMO_SEND_T* pUIMOSave)
{
	DWORD		    Time;
    UINT8           i, j;
    UINT8           nFileIndex, nUIIndex;
    SMSMO_UI_T      Temp;

    sInitSMSMOSaveMsg();

    //Sorting
    if(g_nMOSaveSize != 0)
    {
	    for (i = 0; i < g_nMOSaveSize - 1; i++) 
        {
		    for (j = i + 1; j < g_nMOSaveSize; j++) 
            {
			    if (g_aSMSSaveMsg[i].dwMadeTime < g_aSMSSaveMsg[j].dwMadeTime) 
                {
				    Temp = g_aSMSSaveMsg[i];
				    g_aSMSSaveMsg[i] = g_aSMSSaveMsg[j];
				    g_aSMSSaveMsg[j] = Temp;
			    }
		    }
	    }
    }

    //get unused or oldest global buffer
    if(g_nMOSaveSize >= MAX_SMS_MO)
    {
        nUIIndex = MAX_SMS_MO-1;
        nFileIndex = g_aSMSSaveMsg[nUIIndex].nAddr;
    }
    else
    {       
        nUIIndex = g_nMOSaveSize;
        nFileIndex = g_aSMSSaveMsg[nUIIndex].nAddr;  //get ui global buffer
        g_nMOSaveSize++; 
    }    

    pUIMOSave->nAttr |= SMS_USED;
    pUIMOSave->nAddr = nFileIndex;
    if((Time = SKY_GetLocalTime())!= SKY_INVALID_SYS_TIME)
    {
        pUIMOSave->dwMadeTime = Time;
    }
    
    g_aSMSSaveMsg[nUIIndex].nAttr = pUIMOSave->nAttr;
    g_aSMSSaveMsg[nUIIndex].nAddr = pUIMOSave->nAddr;
    g_aSMSSaveMsg[nUIIndex].dwMadeTime = pUIMOSave->dwMadeTime;

    SKY_SeekWriteMOSaveT(nFileIndex, pUIMOSave, &g_nMOSaveSize);  
}    

VOID DeleteSMSMOSaveMsg(UINT8 nDelIndex)
{
    STATIC SMSMO_SEND_T*    pDeleteElement;
    SMSMO_UI_T              Temp;
    UINT8                   i;

    g_aSMSSaveMsg[nDelIndex].nAttr &= ~SMS_USED;

    /* file buffer */
    pDeleteElement = (SMSMO_SEND_T*)SKY_Calloc(1, sizeof(SMSMO_SEND_T));
    pDeleteElement->nAttr = g_aSMSSaveMsg[nDelIndex].nAttr;   // UNUSED
    pDeleteElement->nAddr = g_aSMSSaveMsg[nDelIndex].nAddr;

    /* global buffer write */
    Temp = g_aSMSSaveMsg[nDelIndex];
    for(i = nDelIndex; i < g_nMOSaveSize; i++)
    {
        g_aSMSSaveMsg[i] = g_aSMSSaveMsg[i+1];        
    }
    g_aSMSSaveMsg[g_nMOSaveSize - 1] = Temp;           

    g_nMOSaveSize--;

    /* file write */
    SKY_SeekWriteMOSaveT(pDeleteElement->nAddr, pDeleteElement, &g_nMOSaveSize);

    SKY_Free(pDeleteElement);
}

BYTE** AddMOSaveScrollList(BYTE* pszNewStr, UINT8 nLenScrollList)
{
    INT8   nIndex;

    if(nLenScrollList == MAX_SMS_MO)
    {
        SKY_Free(l_paViewBuf[0]);
        l_paViewBuf[0] = (BYTE *)SKY_Calloc(MAX_VIEW_BUF_COLUME_SIZE, sizeof(BYTE));            
    }
    else
    {
        if(nLenScrollList == 0)
        {
            l_paViewBuf[0] = (BYTE *)SKY_Calloc(MAX_VIEW_BUF_COLUME_SIZE, sizeof(BYTE));            
        }            
        else
        {
            for( nIndex = nLenScrollList-1 ; nIndex >= 0; nIndex--)
            {
                l_paViewBuf[nIndex] = l_paViewBuf[nIndex+1];
            }
            l_paViewBuf[0] = (BYTE *)SKY_Calloc(MAX_VIEW_BUF_COLUME_SIZE, sizeof(BYTE));            
        }
    }

    //not necessary formatting function
    if( STRLEN(pszNewStr) < MAX_VIEW_BUF_COLUME_SIZE )
        STRCPY(l_paViewBuf[0], pszNewStr);
    else    STRNCPY(l_paViewBuf[0], pszNewStr, MAX_VIEW_BUF_COLUME_SIZE-1);
    
    return(l_paViewBuf);
}

BYTE** MakeMOSaveScrollList(VOID)
{
    UINT8           i,j;
    SMSMO_UI_T      Temp;
    SMSMO_SEND_T*   pTemp;

    sInitSMSMOSaveMsg();

    //Sorting
    if(g_nMOSaveSize != 0)
    {
	    for (i = 0; i < g_nMOSaveSize - 1; i++) 
        {
		    for (j = i + 1; j < g_nMOSaveSize; j++) 
            {
			    if (g_aSMSSaveMsg[i].dwMadeTime < g_aSMSSaveMsg[j].dwMadeTime) 
                {
				    Temp = g_aSMSSaveMsg[i];
				    g_aSMSSaveMsg[i] = g_aSMSSaveMsg[j];
				    g_aSMSSaveMsg[j] = Temp;
			    }
		    }
	    }
    }

    //Make List
    pTemp = (SMSMO_SEND_T *)SKY_Calloc(1,sizeof(SMSMO_SEND_T));
    
    memset(l_paViewBuf, NULL, sizeof(l_paViewBuf));

    for( i = 0 ; i < g_nMOSaveSize; i++)
    {
        l_paViewBuf[i] = (BYTE *)SKY_Calloc(MAX_VIEW_BUF_COLUME_SIZE, sizeof(BYTE)); //LCD_COLUME_SIZE+2 : 20
        
        SKY_SeekReadMOSaveT(g_aSMSSaveMsg[i].nAddr, pTemp);

        ui_sms_auto_format_one_line(MAX_VIEW_BUF_COLUME_SIZE-1, 
                                    pTemp->pszMsg,
                                    pTemp->nMsgSize,
                                    l_paViewBuf[i], MAX_VIEW_BUF_COLUME_SIZE-1);

    }

    SKY_Free(pTemp);

    return l_paViewBuf;        
}

VOID GetMOSaveSize(UINT8* pnMOSaveTotalNum)
{
    HFILE   hFile;

    hFile = SKY_OpenFile(FILE_NAME_MOSAVE, FS_AM_READWRITE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    SKY_ReadFile(hFile, pnMOSaveTotalNum, sizeof(MO_FILE_HEADER));
    SKY_CloseFile(hFile);
}
//////////////////////////////////////////////////////////////////////
//           SMS MO Sent  EFS
VOID DeleteAllSentMsg(VOID)
{
    INT8    i;
    HFILE   hFile;

    if(SKY_IsFileOrDirExist(FILE_NAME_MOSENT) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    /* initialize */
    memset(&l_MOSentBuf, NULL, sizeof(SMSMO_SEND_T));
    g_nMOSentSize = 0;

    SKY_WriteFile(hFile, &g_nMOSentSize, sizeof(MO_FILE_HEADER));  
    for(i = 0; i < MAX_SENT_MSG; i++) // huni sent 30 (20 ->40 ->30)
    {
        SKY_WriteFile(hFile, &(l_MOSentBuf), sizeof(l_MOSentBuf));
        g_aSMSSentMsg[i].nAddr = i;
        g_aSMSSentMsg[i].nAttr = l_MOSentBuf.nAttr;
        g_aSMSSentMsg[i].dwMadeTime = l_MOSentBuf.dwMadeTime;
    }
    SKY_CloseFile(hFile);
}
    
LOCAL VOID sInitSMSMOSent(VOID)
{
    INT8            i;
    HFILE           hFile;
    UINT8           nUsedCnt, nUnusedCnt;

    nUsedCnt = nUnusedCnt = 0;
    if(SKY_IsFileOrDirExist(FILE_NAME_MOSENT) == FALSE)
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);


        /* initialize */
        memset(&l_MOSentBuf, NULL, sizeof(SMSMO_SEND_T));
        g_nMOSentSize = 0;

        SKY_WriteFile(hFile, &g_nMOSentSize, sizeof(MO_FILE_HEADER));  
        for(i = 0; i < MAX_SENT_MSG; i++) // huni sent 30 (20 ->40 ->30)
        {
            SKY_WriteFile(hFile, &(l_MOSentBuf), sizeof(l_MOSentBuf));
            g_aSMSSentMsg[i].nAddr = i;
            g_aSMSSentMsg[i].nAttr = l_MOSentBuf.nAttr;
            g_aSMSSentMsg[i].dwMadeTime = l_MOSentBuf.dwMadeTime;
        }
        SKY_CloseFile(hFile);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        memset(&l_MOSentBuf, NULL, sizeof(SMSMO_SEND_T));
        SKY_ReadFile(hFile, &g_nMOSentSize, sizeof(MO_FILE_HEADER));
        if(g_nMOSentSize != 0)
        {
            for(i = 0; i < MAX_SENT_MSG; i++) // huni sent 30 (20 ->40 ->30)
            {
                SKY_ReadFile(hFile, &(l_MOSentBuf), sizeof(l_MOSentBuf));

                if(l_MOSentBuf.nAttr & SMS_USED)
                {
                    g_aSMSSentMsg[nUsedCnt].nAddr = i;
                    g_aSMSSentMsg[nUsedCnt].nAttr = l_MOSentBuf.nAttr;
                    g_aSMSSentMsg[nUsedCnt].dwMadeTime = l_MOSentBuf.dwMadeTime;
                    nUsedCnt++;
                }
                else
                {
                    g_aSMSSentMsg[MAX_SENT_MSG-1-nUnusedCnt].nAddr = i; // huni sent 30 (20 ->40 ->30)
                    g_aSMSSentMsg[MAX_SENT_MSG-1-nUnusedCnt].nAttr = l_MOSentBuf.nAttr; // huni sent 30 (20 ->40 ->30)
                    g_aSMSSentMsg[MAX_SENT_MSG-1-nUnusedCnt].dwMadeTime = l_MOSentBuf.dwMadeTime; // huni sent 30 (20 ->40 ->30)
                    nUnusedCnt++;
                }                
            }
        }
        /* close the file */
        SKY_CloseFile(hFile);         
    }
}
    
LOCAL VOID sInitSMSMOSentMsg(VOID)
{
    HFILE   hFile;
    UINT8   i, nUsedCnt, nUnusedCnt;

    nUsedCnt = nUnusedCnt = 0;
    if(SKY_IsFileOrDirExist(SKY_SMSMO_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_SMSMO_DIR);
    }
    /* open the file */
    hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        sInitSMSMOSent();
    }
    else
    {
        memset(&l_MOSentBuf, NULL, sizeof(SMSMO_SEND_T));
        SKY_ReadFile(hFile, &g_nMOSentSize, sizeof(MO_FILE_HEADER));
        if(g_nMOSentSize != 0)
        {
            for(i = 0; i < MAX_SENT_MSG; i++) // huni sent 40 (20 ->40)
            {
                SKY_ReadFile(hFile, &(l_MOSentBuf), sizeof(l_MOSentBuf));

                if(l_MOSentBuf.nAttr & SMS_USED)
                {
                    g_aSMSSentMsg[nUsedCnt].nAddr = i;
                    g_aSMSSentMsg[nUsedCnt].nAttr = l_MOSentBuf.nAttr;
                    g_aSMSSentMsg[nUsedCnt].dwMadeTime = l_MOSentBuf.dwMadeTime;
                    nUsedCnt++;
                }
                else
                {
                    g_aSMSSentMsg[MAX_SENT_MSG-1-nUnusedCnt].nAddr = i; // huni sent 30 (20 ->40 ->30)
                    g_aSMSSentMsg[MAX_SENT_MSG-1-nUnusedCnt].nAttr = l_MOSentBuf.nAttr; // huni sent 30 (20 ->40 ->30)
                    g_aSMSSentMsg[MAX_SENT_MSG-1-nUnusedCnt].dwMadeTime = l_MOSentBuf.dwMadeTime; // huni sent 30 (20 ->40 ->30)
                    nUnusedCnt++;
                }                
            }
        }
        /* close the file */
        SKY_CloseFile(hFile);        
    }    
}

VOID SKY_SeekReadMOSentT( UINT8 nIndex, SMSMO_SEND_T* pReadRecord)
{
    HFILE   hFile;
    DWORD   dwCount;

    hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_READWRITE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    dwCount = SKY_SeekReadFile( 
                hFile, FS_SO_SET, nIndex*sizeof(SMSMO_SEND_T) + sizeof(MO_FILE_HEADER),  // UINT8 : header size                
                pReadRecord, sizeof(SMSMO_SEND_T) );
    SKY_CloseFile( hFile );
}

VOID SKY_SeekWriteMOSentT( UINT8 nIndex, SMSMO_SEND_T* pWriteRecord, MO_FILE_HEADER* pUsedNum)
{

    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MOSENT) == FALSE)
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    SKY_WriteFile(hFile, pUsedNum, sizeof(MO_FILE_HEADER));
    dwCount = SKY_SeekWriteFile( 
                hFile, FS_SO_SET, nIndex*sizeof(SMSMO_SEND_T) + sizeof(MO_FILE_HEADER),
                pWriteRecord, sizeof(SMSMO_SEND_T) );
    SKY_CloseFile( hFile );
}


VOID SaveSMSMOSentMsg(SMSMO_SEND_T* pUIMOSent)
{
	DWORD		    Time;
    UINT8           i, j, nFileIndex, nUIIndex;
    SMSMO_UI_T      Temp;

    sInitSMSMOSentMsg();

    //Sorting
    if(g_nMOSentSize != 0)
    {
	    for (i = 0; i < g_nMOSentSize - 1; i++) 
        {
		    for (j = i + 1; j < g_nMOSentSize; j++) 
            {
			    if (g_aSMSSentMsg[i].dwMadeTime < g_aSMSSentMsg[j].dwMadeTime) 
                {
				    Temp = g_aSMSSentMsg[i];
				    g_aSMSSentMsg[i] = g_aSMSSentMsg[j];
				    g_aSMSSentMsg[j] = Temp;
			    }
		    }
	    }
    }

    //get unused or oldest global buffer
    if(g_nMOSentSize >= MAX_SENT_MSG) // huni sent 30 (20 ->40 ->30)
    {
        nUIIndex = MAX_SENT_MSG-1; // huni sent 30 (20 ->40 ->30)
        nFileIndex = g_aSMSSentMsg[nUIIndex].nAddr;
    }
    else
    {
        nUIIndex = g_nMOSentSize;
        nFileIndex = g_aSMSSentMsg[nUIIndex].nAddr;  //get ui global buffer
        g_nMOSentSize++; 
    }
    

    pUIMOSent->nAttr |= SMS_USED;
    pUIMOSent->nAddr = nFileIndex;
    if((Time = SKY_GetLocalTime())!= SKY_INVALID_SYS_TIME)
    {
        pUIMOSent->dwMadeTime = Time;
    }
    
    g_aSMSSentMsg[nUIIndex].nAttr = pUIMOSent->nAttr;
    g_aSMSSentMsg[nUIIndex].nAddr = pUIMOSent->nAddr;
    g_aSMSSentMsg[nUIIndex].dwMadeTime = pUIMOSent->dwMadeTime;

    SKY_SeekWriteMOSentT(nFileIndex, pUIMOSent, &g_nMOSentSize);  
}    

VOID DeleteSMSMOSentMsg(UINT8 nDelIndex)
{
    STATIC SMSMO_SEND_T*    pDeleteElement;
    SMSMO_UI_T              Temp;
    UINT8                   i;

    g_aSMSSentMsg[nDelIndex].nAttr &= ~SMS_USED;

    /* file buffer */
    pDeleteElement = (SMSMO_SEND_T*)SKY_Calloc(1, sizeof(SMSMO_SEND_T));
    pDeleteElement->nAttr = g_aSMSSentMsg[nDelIndex].nAttr;   // UNUSED
    pDeleteElement->nAddr = g_aSMSSentMsg[nDelIndex].nAddr;

    /* global buffer write */
    Temp = g_aSMSSentMsg[nDelIndex];
    for(i = nDelIndex; i < g_nMOSentSize; i++)
    {
        g_aSMSSentMsg[i] = g_aSMSSentMsg[i+1];        
    }
    g_aSMSSentMsg[g_nMOSentSize - 1] = Temp;           

    g_nMOSentSize--;

    /* file write */
    SKY_SeekWriteMOSentT(pDeleteElement->nAddr, pDeleteElement, &g_nMOSentSize);

    SKY_Free(pDeleteElement);
}

BYTE** MakeMOSentScrollList(VOID)
{
    UINT8           i,j;
    SMSMO_UI_T      Temp;
    SMSMO_SEND_T*   pTemp;

    sInitSMSMOSentMsg();

    //Sorting
    if(g_nMOSentSize != 0)
    {
	    for (i = 0; i < g_nMOSentSize - 1; i++) 
        {
		    for (j = i + 1; j < g_nMOSentSize; j++) 
            {
			    if (g_aSMSSentMsg[i].dwMadeTime < g_aSMSSentMsg[j].dwMadeTime) 
                {
				    Temp = g_aSMSSentMsg[i];
				    g_aSMSSentMsg[i] = g_aSMSSentMsg[j];
				    g_aSMSSentMsg[j] = Temp;
			    }
		    }
	    }
    }

    //Make List
    pTemp = (SMSMO_SEND_T *)SKY_Calloc(1,sizeof(SMSMO_SEND_T));
    
    memset(l_paViewBuf, NULL, sizeof(l_paViewBuf));

    for( i = 0 ; i < g_nMOSentSize; i++)
    {
        l_paViewBuf[i] = (BYTE *)SKY_Calloc(MAX_VIEW_BUF_COLUME_SIZE, sizeof(BYTE)); 
        
        SKY_SeekReadMOSentT(g_aSMSSentMsg[i].nAddr, pTemp);

        ui_sms_auto_format_one_line(MAX_VIEW_BUF_COLUME_SIZE-1, 
                                    pTemp->pszMsg,
                                    pTemp->nMsgSize,
                                    l_paViewBuf[i], MAX_VIEW_BUF_COLUME_SIZE-1);

    }

    SKY_Free(pTemp);

    return l_paViewBuf;        
}


VOID GetMOSentSize(UINT8* pnMOSentTotalNum)
{
    HFILE   hFile;

    hFile = SKY_OpenFile(FILE_NAME_MOSENT, FS_AM_READWRITE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    SKY_ReadFile(hFile, pnMOSentTotalNum, sizeof(MO_FILE_HEADER));
    SKY_CloseFile(hFile);
}

//////////////////////////////////////////////////////////////////////
//           SMS MO Send  EFS
VOID DeleteAllSendMsg(VOID)
{
    INT8    i;
    HFILE   hFile;

    if(SKY_IsFileOrDirExist(FILE_NAME_MOSEND) == FALSE)
    {
        /* there is not the file, create the file */
        hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    /* initialize */
    memset(&l_MOSendBuf, NULL, sizeof(SMSMO_SEND_T));
    g_nMOSendSize = 0;

    SKY_WriteFile(hFile, &g_nMOSendSize, sizeof(MO_FILE_HEADER));  
    for(i = 0; i < MAX_SEND_MSG; i++) // huni send 30 (20 ->30)
    {
        SKY_WriteFile(hFile, &(l_MOSendBuf), sizeof(l_MOSendBuf));
        g_aSMSSendMsg[i].nAddr = i;
        g_aSMSSendMsg[i].nAttr = l_MOSendBuf.nAttr;
        g_aSMSSendMsg[i].dwMadeTime = l_MOSendBuf.dwMadeTime;
    }
    SKY_CloseFile(hFile);
}
    
LOCAL VOID sInitSMSMOSend(VOID)
{
    INT8            i;
    HFILE           hFile;
    UINT8           nUsedCnt, nUnusedCnt;

    nUsedCnt = nUnusedCnt = 0;
    if(SKY_IsFileOrDirExist(FILE_NAME_MOSEND) == FALSE)
    {        
        hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);


        /* initialize */
        memset(&l_MOSendBuf, NULL, sizeof(SMSMO_SEND_T));
        g_nMOSendSize = 0;

        SKY_WriteFile(hFile, &g_nMOSendSize, sizeof(MO_FILE_HEADER));  
        for(i = 0; i < MAX_SEND_MSG; i++) // huni send 30 (20 ->30)
        {
            SKY_WriteFile(hFile, &(l_MOSendBuf), sizeof(l_MOSendBuf));
            g_aSMSSendMsg[i].nAddr = i;
            g_aSMSSendMsg[i].nAttr = l_MOSendBuf.nAttr;
            g_aSMSSendMsg[i].dwMadeTime = l_MOSendBuf.dwMadeTime;
        }
        SKY_CloseFile(hFile);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

        memset(&l_MOSendBuf, NULL, sizeof(SMSMO_SEND_T));
        SKY_ReadFile(hFile, &g_nMOSendSize, sizeof(MO_FILE_HEADER));

        if(g_nMOSendSize != 0)
        {
            for(i = 0; i < MAX_SEND_MSG; i++) // huni send 30 (20 ->30)
            {
                SKY_ReadFile(hFile, &(l_MOSendBuf), sizeof(l_MOSendBuf));

                if(l_MOSendBuf.nAttr & SMS_USED)
                {
                    g_aSMSSendMsg[nUsedCnt].nAddr = i;
                    g_aSMSSendMsg[nUsedCnt].nAttr = l_MOSendBuf.nAttr;
                    g_aSMSSendMsg[nUsedCnt].dwMadeTime = l_MOSendBuf.dwMadeTime;
                    nUsedCnt++;
                }
                else
                {
                    g_aSMSSendMsg[MAX_SEND_MSG-1-nUnusedCnt].nAddr = i; // huni send 30 (20 ->30)
                    g_aSMSSendMsg[MAX_SEND_MSG-1-nUnusedCnt].nAttr = l_MOSendBuf.nAttr; // huni send 30 (20 ->30)
                    g_aSMSSendMsg[MAX_SEND_MSG-1-nUnusedCnt].dwMadeTime = l_MOSendBuf.dwMadeTime; // huni send 30 (20 ->30)
                    nUnusedCnt++;
                }                
            }
        }
        /* close the file */
        SKY_CloseFile(hFile);         
    }
}

LOCAL VOID sInitSMSMOSendMsg(VOID)
{
    HFILE   hFile;
    UINT8   i, nUsedCnt, nUnusedCnt;

    nUsedCnt = nUnusedCnt = 0;
    if(SKY_IsFileOrDirExist(SKY_SMSMO_DIR) == FALSE)
    {
        SKY_MakeDir(SKY_SMSMO_DIR);
    }
    /* open the file */
    hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_READWRITE);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        sInitSMSMOSend();
    }
    else
    {
        memset(&l_MOSendBuf, NULL, sizeof(SMSMO_SEND_T));
        SKY_ReadFile(hFile, &g_nMOSendSize, sizeof(MO_FILE_HEADER));
        if(g_nMOSendSize != 0)
        {
            for(i = 0; i < MAX_SEND_MSG; i++) // huni send 30 (20 ->30)
            {
                SKY_ReadFile(hFile, &(l_MOSendBuf), sizeof(l_MOSendBuf));

                if(l_MOSendBuf.nAttr & SMS_USED)
                {
                    g_aSMSSendMsg[nUsedCnt].nAddr = i;
                    g_aSMSSendMsg[nUsedCnt].nAttr = l_MOSendBuf.nAttr;
                    g_aSMSSendMsg[nUsedCnt].dwMadeTime = l_MOSendBuf.dwMadeTime;
                    nUsedCnt++;
                }
                else
                {
                    g_aSMSSendMsg[MAX_SEND_MSG-1-nUnusedCnt].nAddr = i; // huni send 30 (20 ->30)
                    g_aSMSSendMsg[MAX_SEND_MSG-1-nUnusedCnt].nAttr = l_MOSendBuf.nAttr; // huni send 30 (20 ->30)
                    g_aSMSSendMsg[MAX_SEND_MSG-1-nUnusedCnt].dwMadeTime = l_MOSendBuf.dwMadeTime; // huni send 30 (20 ->30)
                    nUnusedCnt++;
                }                
            }
        }
        /* close the file */
        SKY_CloseFile(hFile);        
    }    
}

VOID SKY_SeekReadMOSendT( UINT8 nIndex, SMSMO_SEND_T* pReadRecord)
{
    HFILE   hFile;
    DWORD   dwCount;

    hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_READWRITE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    dwCount = SKY_SeekReadFile( 
                hFile, FS_SO_SET, nIndex*sizeof(SMSMO_SEND_T) + sizeof(MO_FILE_HEADER),  // UINT8 : header size                
                pReadRecord, sizeof(SMSMO_SEND_T) );
    SKY_CloseFile( hFile );
}

VOID SKY_SeekWriteMOSendT( UINT8 nIndex, SMSMO_SEND_T* pWriteRecord, MO_FILE_HEADER* pUsedNum)
{

    HFILE hFile;
    DWORD dwCount;

    if(SKY_IsFileOrDirExist(FILE_NAME_MOSEND) == FALSE)
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_CREATE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }
    else
    {
        hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_READWRITE);
        SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);
    }

    SKY_WriteFile(hFile, pUsedNum, sizeof(MO_FILE_HEADER));
    dwCount = SKY_SeekWriteFile( 
                hFile, FS_SO_SET, nIndex*sizeof(SMSMO_SEND_T) + sizeof(MO_FILE_HEADER),
                pWriteRecord, sizeof(SMSMO_SEND_T) );
    SKY_CloseFile( hFile );
}


VOID SaveSMSMOSendMsg(SMSMO_SEND_T* pUIMOSend)
{
	DWORD		    Time;
    UINT8           i, j, nFileIndex, nUIIndex;
    SMSMO_UI_T      Temp;

    sInitSMSMOSendMsg();

    //Sorting
    if(g_nMOSendSize != 0)
    {
	    for (i = 0; i < g_nMOSendSize - 1; i++) 
        {
		    for (j = i + 1; j < g_nMOSendSize; j++) 
            {
			    if (g_aSMSSendMsg[i].dwMadeTime < g_aSMSSendMsg[j].dwMadeTime) 
                {
				    Temp = g_aSMSSendMsg[i];
				    g_aSMSSendMsg[i] = g_aSMSSendMsg[j];
				    g_aSMSSendMsg[j] = Temp;
			    }
		    }
	    }
    }

    //get unused or oldest global buffer
    if(g_nMOSendSize >= MAX_SEND_MSG) // huni send 30 (20 ->30)
    {
        nUIIndex = MAX_SEND_MSG-1; // huni send 30 (20 ->30)
        nFileIndex = g_aSMSSendMsg[nUIIndex].nAddr;
    }
    else
    {
        nUIIndex = g_nMOSendSize;
        nFileIndex = g_aSMSSendMsg[nUIIndex].nAddr;  //get ui global buffer
        g_nMOSendSize++; 
    }
    

    pUIMOSend->nAttr |= SMS_USED;
    pUIMOSend->nAddr = nFileIndex;
    if((Time = SKY_GetLocalTime())!= SKY_INVALID_SYS_TIME)
    {
        pUIMOSend->dwMadeTime = Time;
    }
    
    g_aSMSSendMsg[nUIIndex].nAttr = pUIMOSend->nAttr;
    g_aSMSSendMsg[nUIIndex].nAddr = pUIMOSend->nAddr;
    g_aSMSSendMsg[nUIIndex].dwMadeTime = pUIMOSend->dwMadeTime;

    SKY_SeekWriteMOSendT(nFileIndex, pUIMOSend, &g_nMOSendSize);  
}


VOID DeleteSMSMOSendMsg(UINT8 nDelIndex)
{
    STATIC SMSMO_SEND_T*    pDeleteElement;
    SMSMO_UI_T              Temp;
    UINT8                   i;

    g_aSMSSendMsg[nDelIndex].nAttr &= ~SMS_USED;

    /* file buffer */
    pDeleteElement = (SMSMO_SEND_T*)SKY_Calloc(1, sizeof(SMSMO_SEND_T));
    pDeleteElement->nAttr = g_aSMSSendMsg[nDelIndex].nAttr;   // UNUSED
    pDeleteElement->nAddr = g_aSMSSendMsg[nDelIndex].nAddr;

    /* global buffer write */
    Temp = g_aSMSSendMsg[nDelIndex];
    for(i = nDelIndex; i < g_nMOSendSize; i++)
    {
        g_aSMSSendMsg[i] = g_aSMSSendMsg[i+1];        
    }
    g_aSMSSendMsg[g_nMOSendSize - 1] = Temp;           

    g_nMOSendSize--;

    /* file write */
    SKY_SeekWriteMOSendT(pDeleteElement->nAddr, pDeleteElement, &g_nMOSendSize);

    SKY_Free(pDeleteElement);
}

BYTE** MakeMOSendScrollList(VOID)
{
    UINT8           i,j;
    SMSMO_UI_T      Temp;
    SMSMO_SEND_T*   pTemp;

    sInitSMSMOSendMsg();

    //Sorting
    if(g_nMOSendSize != 0)
    {
	    for (i = 0; i < g_nMOSendSize - 1; i++) 
        {
		    for (j = i + 1; j < g_nMOSendSize; j++) 
            {
			    if (g_aSMSSendMsg[i].dwMadeTime < g_aSMSSendMsg[j].dwMadeTime) 
                {
				    Temp = g_aSMSSendMsg[i];
				    g_aSMSSendMsg[i] = g_aSMSSendMsg[j];
				    g_aSMSSendMsg[j] = Temp;
			    }
		    }
	    }
    }

    //Make List
    pTemp = (SMSMO_SEND_T *)SKY_Calloc(1,sizeof(SMSMO_SEND_T));
    
    memset(l_paViewBuf, NULL, sizeof(l_paViewBuf));

    for( i = 0 ; i < g_nMOSendSize; i++)
    {
        l_paViewBuf[i] = (BYTE *)SKY_Calloc(MAX_VIEW_BUF_COLUME_SIZE, sizeof(BYTE)); 
        
        SKY_SeekReadMOSendT(g_aSMSSendMsg[i].nAddr, pTemp);

        if(pTemp->nMsgSize == 0)
        {
            STRCPY(l_paViewBuf[i], (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT)); 
        }
        else
        {
            ui_sms_auto_format_one_line(MAX_VIEW_BUF_COLUME_SIZE-1, 
                                        pTemp->pszMsg,
                                        pTemp->nMsgSize,
                                        l_paViewBuf[i], MAX_VIEW_BUF_COLUME_SIZE-1);
        }
    }

    SKY_Free(pTemp);

    return l_paViewBuf;        
}

VOID GetMOSendSize(UINT8* pnMOSendTotalNum)
{
    HFILE   hFile;

    hFile = SKY_OpenFile(FILE_NAME_MOSEND, FS_AM_READWRITE);
    SKY_ASSERT(hFile != SKY_INVALID_FILE_HANDLE);

    SKY_ReadFile(hFile, pnMOSendTotalNum, sizeof(MO_FILE_HEADER));
    SKY_CloseFile(hFile);
}
VOID SKY_API SKY_InitMOSend(VOID)
{
    sInitSMSMOSendMsg();
}
VOID SKY_API SKY_InitMOSent(VOID)
{
    sInitSMSMOSentMsg();
}
VOID SKY_API SKY_InitMOSave(VOID)
{
    sInitSMSMOSaveMsg();
}
VOID FreeSMSMOScrollList(UINT8 nMOSize)
{
    UINT8   i;
    for( i = 0; i < nMOSize; i++ )
    {
        SKY_Free(l_paViewBuf[i]); 
    }        
}
#ifndef WIN32 // for window version

#ifdef FEATURE_SKTT_EQUS_LOCATION
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SendLocationMsg()
**
** Description: 
**     sSKA_SMSEditMsg
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SendLocationMsg(SMSMO_SEND_T *sms_mo)
{

    UINT8   i;
    uasms_client_bd_type            *bd_ptr; // IN
    uasms_client_message_type       *cl_ptr; // OUT
    UINT8   nLang;

///////Formatting
    bd_ptr = & uinuasms_mo_bds[0];

    bd_ptr->mask = UASMS_MASK_BD_MSG_ID 
                 | UASMS_MASK_BD_USER_DATA 
                 // | UASMS_MASK_BD_USER_RESP 
                 // | UASMS_MASK_BD_MC_TIME   
                 // | UASMS_MASK_BD_VALID_ABS 
                 // | UASMS_MASK_BD_VALID_REL 
                 // | UASMS_MASK_BD_DEFER_ABS 
                 // | UASMS_MASK_BD_DEFER_REL 
                 | UASMS_MASK_BD_PRIORITY  
//                 | UASMS_MASK_BD_PRIVACY   
//                 | UASMS_MASK_BD_REPLY_OPTION 
//                 | UASMS_MASK_BD_NUM_OF_MSGS 
//                 | UASMS_MASK_BD_ALERT     
//                 | UASMS_MASK_BD_LANGUAGE  
                 | UASMS_MASK_BD_CALLBACK ;
                 

    bd_ptr->message_id.type       = UASMS_BD_TYPE_SUBMIT;   //2
    bd_ptr->message_id.id_number  = 2;

	nLang = MO_MSG_LANG_ENGLISH;       // English : 7bit ASCII

    for (i = 0; i < sms_mo->nMsgSize; i++) 
    {
        if (sms_mo->pszMsg[i] & 0x80) 
        {
            nLang = MO_MSG_LANG_KOREAN;   // 한영혼용  KSC5601
            break;
        }
    }		
  
    bd_ptr->user_data.encoding = (uasms_user_data_encoding_e_type)0x10;
    bd_ptr->user_data.data_len = sms_mo->nMsgSize;        
    bd_ptr->user_data.number_of_digits = sms_mo->nMsgSize;
    memcpy((byte *)bd_ptr->user_data.data, sms_mo->pszMsg, sms_mo->nMsgSize);

    bd_ptr->priority       = (uasms_priority_e_type)sms_mo->nPriority;


    bd_ptr->reply_option.user_ack_requested     = FALSE;
    bd_ptr->reply_option.delivery_ack_requested = TRUE;

    bd_ptr->num_messages   = 3;             //?? don't know

    bd_ptr->alert_on_delivery = TRUE;

    bd_ptr->callback.digit_mode  = UASMS_DIGIT_MODE_4_BIT;
    
    bd_ptr->callback.number_of_digits =
        uinuasms_ascii2dtmf((char *) sms_mo->pszSendNumber, (byte*)bd_ptr->callback.digits );

    bd_ptr->callback.number_of_digits = sms_mo->nSendNumberSize ;

    /* TL fields */

    cl_ptr = & uinuasms_mo_msgs[0];

    cl_ptr->teleservice         = (uasms_teleservice_e_type)TI_LOCATION;

////// 착신번호 
    cl_ptr->address.digit_mode  = UASMS_DIGIT_MODE_4_BIT;   //0
    cl_ptr->address.number_mode = UASMS_NUMBER_MODE_NONE_DATA_NETWORK;  //0

    cl_ptr->address.number_of_digits =
        uinuasms_ascii2dtmf((char *)sms_mo->pszRcvNumber, (byte*)cl_ptr->address.digits );    

    cl_ptr->subaddress.number_of_digits = 0;

    cl_ptr->is_tl_ack_requested = TRUE;

    cl_ptr->is_broadcast = FALSE; 

//////// Sending.....  
    uinuasms_send_MO(0);    //0: transaction id : in case of SKY, it is fixed to 0

}
 
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_LocationSendMsg()
**
** Description: 
**     SKA_LocationSendMsg
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_LocationSendMsg(SKY_EVENT_T *pEvent)
{
    HCONTROL                    hResult, hComplete;
    uasms_status_e_type         RetStatus;
    BYTE                        szTmpBuf[1];
   	LOCATION_INFO_T*            pLocation;


    szTmpBuf[0] = '\0';

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      130, 
                      CL_NONE, //CML_DELETE_NONE_CANCEL,
                      NULL, 
                      WA_SHOW_HEADER);

        memset(&uinuasms_mo_status[0], NULL, sizeof(uasms_status_e_type));   

        if(ui_is_in_conversation())
        {
            SendLocationMsg(&g_UIMOBuffer);
        }
        else
        {
            uasms_DTC_connect();           
        }
        
        EQC_SET_STYLE(EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_LOCATION_SENDING),
                            MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_HELP),
                            CS_MESSAGE_BOX_IGNORE_KEYS | 
                            CS_MESSAGE_BOX_DEFAULT |
                            CS_MESSAGE_BOX_TITLE);

        EQS_FitCanvasSize();

        EQS_StartTimer(EQS_TIMER_1, 10*1000, FALSE); 

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(AB_ALWAYS);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        /* TO IDLE */
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    /* ignore call originated */
    case SE_CALL_ORIGINATED :
        if(ui_curr_call_is_sms())
        {
            SKY_TRACE(TL_HIGH, "UI SMS MO ORIGINATED");        
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();   
        }
        break;

    case SE_CALL_RELEASED:
        if(IsCallTypeSMS())
        {
            SKY_TRACE(TL_HIGH, "UI SMS MO RELEASED");
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();       
        }
        break;

    case SE_CALL_ENDED:
        SKY_TRACE(TL_HIGH, "UI SMS MO FAIL:call end");      
        EQS_PASS_CURRENT_EVENT();              
        break;

    case SE_CALL_COMMAND_ERROR :

        /* call command error */;
        if(IsCallTypeSMS())
        {
            SKY_TRACE(TL_HIGH, "UI SMS MO FAIL:command error");
        }
        else
        {        
            EQS_PASS_CURRENT_EVENT();       
        }  
        break;


    case SE_CALL_CONNECT:
        if(ui_curr_call_is_sms())
        {
            SendLocationMsg(&g_UIMOBuffer);
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();
        }
        break;

    case SE_SMS_MO_STATUS_MESSAGE:
        RetStatus = (uasms_status_e_type)(GET_EVENT_PARAM()->sms_mo_status.status);
        SKY_TRACE1(TL_HIGH, "mo status = %d", RetStatus);

        switch(GET_EVENT_PARAM()->sms_mo_status.status)
        {
        case UASMS_ACCESS_TOO_LARGE_S:      //Traffic Channel 연결 
            break;

        case UASMS_WAITING_FOR_TL_ACK_S:    // SMS 전송 성공 : layer 2 ack
            break;

        case UASMS_OK_S:                                    // SMS 전송 성공 : layer 3 ack
			pLocation =(LOCATION_INFO_T *)&SKY_GET_LOCATION_INFO();
			if( g_UIMOBuffer.pszMsg[0] == LOCATION_INFORM_OFF ) 
            {
				if( pLocation->reportwait )
				{
					pLocation->reportwait = 0;
				}

				pLocation->onoffstate = 0;
				SKY_SET_LOCATION_INFO(*pLocation);
			}
			else 
            {
				(void)SaveLocationReport(pLocation);
			}

            hResult = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_LOCATION_SEND_FINISHED),
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hResult, SMS_RESULT_SUCCESS);
            break; 
            
        default:
           
            /*
                UASMS_PHONE_NOT_READY_S:
                UASMS_NOT_ALLOWED_IN_AMPS_S:
                UASMS_CANNOT_SEND_BROADCAST_S:
                UASMS_INVALID_TRANSACTION_ID_S:            
                UASMS_NETWORK_NOT_READY_S:
                UASMS_OUT_OF_RESOURCES_S:
                UASMS_DTC_TOO_LARGE_S:
            */
            SKY_TRACE(TL_HIGH, "SMS MO FAIL:unexpected status");

            hComplete = EQS_GetModal();
            if(hComplete != INVALID_CONTROL_HANDLE)
            {
                if((EQC_GET_TYPE(hComplete) == CT_MESSAGE_BOX)
                    &&((EQC_GET_USERDATA(hComplete) == SMS_RESULT_SUCCESS) 
                        || (EQC_GET_USERDATA(hComplete) == SMS_RESULT_FAIL)))
                {                                    
                    break;
                }
            } 

            if(GET_EVENT_PARAM()->sms_mo_status.error_class == UASMS_ERROR_PERM)
            {
                if(l_unTrial < NUM_OF_SEND_RETRIAL-1)
                {
                    l_unTrial++;
                    hResult = EQC_BeepMessageBox(
                        (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SEND_RETRIAL),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    EQC_SET_USERDATA(hResult, SMS_RESULT_RETRY);
                }
                else
                {
                    l_unTrial = 0;
                    hResult = EQC_BeepMessageBox(
									(BYTE *)SKY_GET_TOKEN(TKN_LOCATION_NOT_SEND),
                                   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                    EQC_SET_USERDATA(hResult, SMS_RESULT_FAIL);
                }
            }            
            break;
        }        
        break;

    case SE_IO_MESGBOX_END:   
        if(pEvent->dwParam == SMS_RESULT_RETRY)
        {
            memset(&uinuasms_mo_status[0], NULL, sizeof(uasms_status_e_type));   
//            SendLocationMsg(&g_UIMOBuffer);           
            if(ui_is_in_conversation())
            {
                SendLocationMsg(&g_UIMOBuffer);
            }
            else
            {
                uasms_DTC_connect();           
            }         

            EQC_SET_STYLE(EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_LOCATION_SENDING),
                                MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_HELP),
                                CS_MESSAGE_BOX_IGNORE_KEYS | 
                                CS_MESSAGE_BOX_DEFAULT |
                                CS_MESSAGE_BOX_TITLE);
            
            EQS_StartTimer(EQS_TIMER_1, 10*1000, FALSE); 
        }
		else if((pEvent->dwParam == SMS_RESULT_SUCCESS)||
                (pEvent->dwParam == SMS_RESULT_FAIL))//hResultMessage1: 성공 , hResultMessage2: 실패 
		{  
            if(!ui_is_in_conversation())
            {
                if(ui_curr_call_is_sms())
                {
                    uasms_DTC_disconnect();
                }
                EQS_END_PARAM(EQS_NA, pEvent->dwParam);
            }
            else
            {
			    EQS_END_PARAM(EQS_NA, pEvent->dwParam);
            }
		}
        break;

    case SE_TM_TIMER1:
        SKY_TRACE(TL_HIGH, "SMS MO FAIL:timeout");
        if(l_unTrial < NUM_OF_SEND_RETRIAL-1)   //위치추적 재전송 횟수:2
        {
            l_unTrial++;
            hResult = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SEND_RETRIAL),
                           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hResult, SMS_RESULT_RETRY); 
        }
        else
        {
            l_unTrial = 0;
            hResult = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_LOCATION_NOT_SEND),
                           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            EQC_SET_USERDATA(hResult, SMS_RESULT_FAIL); 
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif // #ifdef FEATURE_SKTT_EQUS_LOCATION  
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SendSMSMsg()
**
** Description: 
**     sSKA_SMSEditMsg
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SendSMSMsg(SMSMO_SEND_T *sms_mo, UINT16 nTeleserviceID)
{
    UINT8   i;
    uasms_client_bd_type            *bd_ptr; // IN
    uasms_client_message_type       *cl_ptr; // OUT
    UINT8   nLang;

///////Formatting
    memset((void *)& uinuasms_mo_bds[0], NULL, sizeof(uasms_client_bd_type));
    memset((void *)& uinuasms_mo_msgs[0], NULL, sizeof(uasms_client_message_type));

    bd_ptr = & uinuasms_mo_bds[0];

    bd_ptr->mask = UASMS_MASK_BD_MSG_ID 
                 | UASMS_MASK_BD_USER_DATA 
                 // | UASMS_MASK_BD_USER_RESP 
                 // | UASMS_MASK_BD_MC_TIME   
                 // | UASMS_MASK_BD_VALID_ABS 
                 // | UASMS_MASK_BD_VALID_REL 
                 // | UASMS_MASK_BD_DEFER_ABS 
                 // | UASMS_MASK_BD_DEFER_REL 
                 | UASMS_MASK_BD_PRIORITY  
//                 | UASMS_MASK_BD_PRIVACY   
//                 | UASMS_MASK_BD_REPLY_OPTION 
//                 | UASMS_MASK_BD_NUM_OF_MSGS 
//                 | UASMS_MASK_BD_ALERT     
                 | UASMS_MASK_BD_LANGUAGE 
                 | UASMS_MASK_BD_CALLBACK; 
                 

    bd_ptr->message_id.type       = UASMS_BD_TYPE_SUBMIT;   //2
    bd_ptr->message_id.id_number  = 2;

	nLang = MO_MSG_LANG_ENGLISH;       // English : 7bit ASCII

    for (i = 0; i < sms_mo->nMsgSize; i++) 
    {
        if (sms_mo->pszMsg[i] & 0x80) 
        {
            nLang = MO_MSG_LANG_KOREAN;   // 한영혼용  KSC5601
            break;
        }
    }		
  
    if(nLang == MO_MSG_LANG_KOREAN)
    {
        bd_ptr->user_data.encoding = (uasms_user_data_encoding_e_type)0x10;
        bd_ptr->user_data.data_len = sms_mo->nMsgSize;        
        bd_ptr->user_data.number_of_digits = sms_mo->nMsgSize;
        memcpy((byte *)bd_ptr->user_data.data, sms_mo->pszMsg, sms_mo->nMsgSize);
        bd_ptr->language       = (uasms_language_e_type)0xFE;      //SKT 규격 

    } 
    else if(nLang == MO_MSG_LANG_ENGLISH)
    {
        bd_ptr->user_data.encoding = UASMS_ENCODING_ASCII;
        bd_ptr->user_data.number_of_digits = 
            uinuasms_pack_ascii((const char *)sms_mo->pszMsg,
                          (byte*) bd_ptr->user_data.data,
                          (byte*) & bd_ptr->user_data.data_len,
                          (byte*) & bd_ptr->user_data.padding_bits );       
        bd_ptr->language       = UASMS_LANGUAGE_ENGLISH;
    }
    else
    {
        bd_ptr->user_data.encoding = UASMS_ENCODING_ASCII;

    }        

    bd_ptr->priority       = (uasms_priority_e_type)sms_mo->nPriority;

    bd_ptr->reply_option.user_ack_requested     = FALSE;
    bd_ptr->reply_option.delivery_ack_requested = TRUE;

    bd_ptr->num_messages   = 3;             //?? don't know

    bd_ptr->alert_on_delivery = TRUE;

    bd_ptr->callback.digit_mode  = UASMS_DIGIT_MODE_4_BIT;
    
    bd_ptr->callback.number_of_digits =
        uinuasms_ascii2dtmf((char *) sms_mo->pszSendNumber, (byte*)bd_ptr->callback.digits );

    bd_ptr->callback.number_of_digits = sms_mo->nSendNumberSize ;
    
    if (bd_ptr->callback.number_of_digits == 0)
    {           
        bd_ptr->mask &= ~UASMS_MASK_BD_CALLBACK;
    }
    /* TL fields */

    cl_ptr = & uinuasms_mo_msgs[0];

    cl_ptr->teleservice         = (uasms_teleservice_e_type)nTeleserviceID;

////// 착신번호 
    cl_ptr->address.digit_mode  = UASMS_DIGIT_MODE_4_BIT;   //0
    cl_ptr->address.number_mode = UASMS_NUMBER_MODE_NONE_DATA_NETWORK;  //0

    cl_ptr->address.number_of_digits =
        uinuasms_ascii2dtmf((char *)sms_mo->pszRcvNumber, (byte*)cl_ptr->address.digits );    

    cl_ptr->subaddress.number_of_digits = 0;

    cl_ptr->is_tl_ack_requested = TRUE;

    cl_ptr->is_broadcast = FALSE; 

//////// Sending.....  
    uinuasms_send_MO(0);    //0: transaction id : in case of SKY, it is fixed to 0
}

//////////////////////////////////////////////////////////////////////////////////////
//  sMsgSendControls.. For DongBo. huni add 01.9.11
//

void sMsgSendControls(void)
{
    BYTE                        szBuf1[MAX_SMS_DIAL+1]; //call back number
    UINT8                       nPos, nHyphenpos;
    UINT16                      nLen, nStartX;

    // Initialize Buffer
    memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
    g_UIMOBuffer.nRcvNumberSize = 0;

    STRNCPY(g_UIMOBuffer.pszRcvNumber, l_DongboSendBuf.pszRcvNumber[g_nDongboNow], MAX_DONGBO_DIAL);
    g_UIMOBuffer.nRcvNumberSize = l_DongboSendBuf.nRcvNumberSize[g_nDongboNow];

    g_nDongboNow++;

    memset(&uinuasms_mo_status[0], NULL, sizeof(uasms_status_e_type));        
    uasms_DTC_connect();

    memset(szBuf1, NULL, MAX_SMS_DIAL+1);
    memset(szTmpStr, NULL, LCD_COLUME_SIZE + 1);

    STRCPY( szBuf1, g_UIMOBuffer.pszRcvNumber );

    nPos = FindCodeSet(szBuf1);
    nHyphenpos = FindHypen( szBuf1, nPos, 8);  // 8 : Max Length (Except 011, 02 length )

    // Clear
    nStartX = 0;
    EQC_SET_X(EQS_GetNthIOC(SSA_FRAME_TITLE), nStartX);
    EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_TITLE), (BYTE *)SKY_GET_TOKEN(TKN_BLANK));

    MakeHypenedNumber( szBuf1, szTmpStr,
                       (UINT8)g_GetLength(szBuf1, LCD_COLUME_SIZE-2), 
                       nPos, 
                       nHyphenpos );

    nLen = EQS_CharWidth((BYTE)' ')* STRLEN(szTmpStr); 
    nStartX = (EQS_GetMaxWidth()- nLen)/2;
    EQC_SET_X(EQS_GetNthIOC(SSA_FRAME_STATUS_1), nStartX);
    EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_STATUS_1), szTmpStr);

//    EQC_SET_STYLE( EQS_GetNthIOC(SSA_FRAME_STATUS_2), 
//                   CS_STATIC_TEXT_DEFAULT | CS_STATIC_TEXT_ORING);

    EQS_StartTimer(EQS_TIMER_1, 10*1000, FALSE); 
}
  
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_SMSSendMsg()
**
** Description: 
**     SKA_SMSSendMsg
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_SMSSendMsg(SKY_EVENT_T *pEvent)
{
    STATIC UINT16               nTI;

    HCONTROL                    hResult, hComplete;
    uasms_status_e_type         RetStatus;
    BYTE                        szTmpBuf[1];
    BYTE                        szBuf1[MAX_SMS_DIAL+1]; //call back number
    UINT8                       i, nPos, nHyphenpos;
    UINT16                      nLen, nStartX;
    CHAR                        tmpBuf[LCD_COLUME_SIZE*4+1];

    szTmpBuf[0] = '\0';
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      130, 
                      CL_NONE, //CML_DELETE_NONE_CANCEL,
                      NULL, 
                      WA_SHOW_HEADER);

        memset(&uinuasms_mo_status[0], NULL, sizeof(uasms_status_e_type));        
        uasms_DTC_connect();

        nTI = (UINT16)pEvent->dwParam;
        g_nDongboNow = 1;

        EQC_Animation(SMS_SEND_ANI_X, SMS_SEND_ANI_Y, &g_AniSendMessage, NULL);

        /* 프레임 */
//        EQC_Image(SMS_SEND_FRAME_X, SMS_SEND_FRAME_Y, (BITMAP*)&g_BmpGeneralPannel);

        EQC_SET_STYLE(EQS_GetNthIOC(SSA_SEND_ANIMATION), CS_ANIMATION_DEFAULT);
        EQC_StartAnimation(EQS_GetNthIOC(SSA_SEND_ANIMATION), TRUE);

//        nLen = EQS_CharWidth((BYTE)' ')* STRLEN((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SENDING_STATUS)); 
//        nStartX = (EQS_GetMaxWidth()- nLen)/2;
//        EQC_StaticText(nStartX, SMS_SEND_STATUS_TITLE_Y, TKN_SMS_MSG_SENDING_STATUS);
        EQC_Image(SMS_SEND_FRAME_X, SMS_SEND_FRAME_Y, (BITMAP*)&g_BmpCallPannelTransMessage); // Replace Frame

        // Frame Clear
        EQC_StaticText(0, SMS_SEND_STATUS_Y, TKN_BLANK);

        memset(szBuf1, NULL, MAX_SMS_DIAL+1);
        memset(szTmpStr, NULL, LCD_COLUME_SIZE + 1);

        STRCPY( szBuf1, g_UIMOBuffer.pszRcvNumber );

        nPos = FindCodeSet(szBuf1);
        nHyphenpos = FindHypen( szBuf1, nPos, 8);  // 8 : Max Length (Except 011, 02 length )

        MakeHypenedNumber( szBuf1, szTmpStr,
                           (UINT8)g_GetLength(szBuf1, LCD_COLUME_SIZE-2), 
                           nPos, 
                           nHyphenpos );

        nLen = EQS_CharWidth((BYTE)' ')* STRLEN(szTmpStr); 
        nStartX = (EQS_GetMaxWidth()- nLen)/2;

        EQC_StaticText(nStartX, SMS_SEND_STATUS_Y, TKN_NULL);
        EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_STATUS_1), szTmpStr);

//        EQC_SET_STYLE( EQS_GetNthIOC(SSA_FRAME_STATUS_2), 
//                       CS_STATIC_TEXT_DEFAULT | CS_STATIC_TEXT_ORING);

        nLen = EQS_CharWidth((BYTE)' ')* STRLEN((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SENDING)); 
        nStartX = (EQS_GetMaxWidth()- nLen)/2;
        EQC_StaticText(nStartX, SMS_SEND_STATUS_Y+CharHeight(SMS_SPACE), TKN_SMS_MSG_SENDING);

//        EQC_SET_STYLE( EQC_StaticText(nStartX, SMS_SEND_STATUS_Y+CharHeight(SMS_SPACE), TKN_SMS_MSG_SENDING), 
//                       CS_STATIC_TEXT_DEFAULT | CS_STATIC_TEXT_ORING);

        EQS_StartTimer(EQS_TIMER_1, 10*1000, FALSE); 

        SKY_SET_NO_OFF_BACKLIGHT(TRUE);

        SKY_BacklightOn(AB_ALWAYS);
        EQS_FitCanvasSize();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        if(ui_curr_call_is_sms())
        {
            uasms_DTC_disconnect();
        }
        SKY_SET_NO_OFF_BACKLIGHT(FALSE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    /* ignore call originated */
    case SE_CALL_ORIGINATED :
        if(ui_curr_call_is_sms())
        {
            SKY_TRACE(TL_HIGH, "UI SMS MO ORIGINATED");        
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();   
        }
        break;

    case SE_CALL_RELEASED:
        if(IsCallTypeSMS())
        {
            SKY_TRACE(TL_HIGH, "UI SMS MO RELEASED");
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();       
        }
        break;


    case SE_CALL_COMMAND_ERROR :

        /* call command error */;
        if(IsCallTypeSMS())
        {
            SKY_TRACE(TL_HIGH, "UI SMS MO FAIL:command error");
        }
        else
        {        
            EQS_PASS_CURRENT_EVENT();       
        }  
        break;

    case SE_CALL_ENDED:
        SKY_TRACE(TL_HIGH, "UI SMS MO FAIL:call end");
        EQS_PASS_CURRENT_EVENT();       
        break;

    case SE_CALL_CONNECT:
        if(ui_curr_call_is_sms())
        {
            SendSMSMsg(&g_UIMOBuffer, nTI);
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();
        }

        break;
    case SE_CALL_INCOMING:
        /* 내용 입력 */
        if((EQS_GET_ANCESTOR(2) == sSKA_MOSendMsg) && ( g_nDongboNow == 1 ) ) //보낼 메시지함에 2번 저장되는 것을 막기 위함 
        {
            DeleteSMSMOSendMsg(l_nMOSendDelIndex);
        }
        SaveSMSMOSendMsg(&g_UIMOBuffer);
        
        if( g_nDongboTotal > g_nDongboNow ) // Remain other data
        {
            for( i = g_nDongboNow; i < g_nDongboTotal; i++ )
            {
                // Initialize Buffer
                memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
                g_UIMOBuffer.nRcvNumberSize = 0;

                STRNCPY(g_UIMOBuffer.pszRcvNumber, l_DongboSendBuf.pszRcvNumber[i], MAX_DONGBO_DIAL);
                g_UIMOBuffer.nRcvNumberSize = l_DongboSendBuf.nRcvNumberSize[i];

                SaveSMSMOSendMsg(&g_UIMOBuffer);
            }
        } 
        EQS_PASS_CURRENT_EVENT();
        break;

    case SE_SMS_MO_STATUS_MESSAGE:
        RetStatus = (uasms_status_e_type)(GET_EVENT_PARAM()->sms_mo_status.status);
        SKY_TRACE1(TL_HIGH, "mo status = %d", RetStatus);

        switch(GET_EVENT_PARAM()->sms_mo_status.status)
        {
        case UASMS_ACCESS_TOO_LARGE_S:      //Traffic Channel 연결 
//            uasms_DTC_connect();
            break;

        case UASMS_WAITING_FOR_TL_ACK_S:    // SMS 전송 성공 : layer 2 ack
            break;

        case UASMS_OK_S:                    // SMS 전송 성공 : layer 3 ack
            if(ui_curr_call_is_sms())
            {
                uasms_DTC_disconnect();
            }

            if((EQS_GET_ANCESTOR(2) == sSKA_MOSendMsg) && ( g_nDongboNow == 1 ) )
            {
                DeleteSMSMOSendMsg(l_nMOSendDelIndex);
            }
            // 보낸메시지에 저장 
            SaveSMSMOSentMsg(&g_UIMOBuffer);

            // 착발신 이력에 저장 
            if(IsConfidentialGroup((BYTE *)g_UIMOBuffer.pszRcvNumber) != PB_SECRET_OK)
            {
                AddCallsList(CT_SMS_MO, g_UIMOBuffer.pszRcvNumber, 
                                g_UIMOBuffer.nRcvNumberSize, szTmpBuf);
            }

            if( g_nDongboTotal > g_nDongboNow ) // Remain other data
            {
                sMsgSendControls(); // Add Control
            }
            else // Last Data
            {
                hResult = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SEND_FINISHED),
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
                EQC_SET_USERDATA(hResult, SMS_RESULT_SUCCESS);
            }
            break;  
            
        default:
           
            /*
                UASMS_PHONE_NOT_READY_S:
                UASMS_NOT_ALLOWED_IN_AMPS_S:
                UASMS_CANNOT_SEND_BROADCAST_S:
                UASMS_INVALID_TRANSACTION_ID_S:            
                UASMS_NETWORK_NOT_READY_S:
                UASMS_OUT_OF_RESOURCES_S:
                UASMS_DTC_TOO_LARGE_S:
            */
            SKY_TRACE(TL_HIGH, "SMS MO FAIL:unexpected status");
            
            hComplete = EQS_GetModal();
            if(hComplete != INVALID_CONTROL_HANDLE)
            {
                if((EQC_GET_TYPE(hComplete) == CT_MESSAGE_BOX)
                    &&((EQC_GET_USERDATA(hComplete) == SMS_RESULT_SUCCESS) 
                        || (EQC_GET_USERDATA(hComplete) == SMS_RESULT_FAIL)))
                {                                    
                    break;
                }
            }                

            if(GET_EVENT_PARAM()->sms_mo_status.error_class == UASMS_ERROR_PERM)
            {
                if(l_unTrial < NUM_OF_SEND_RETRIAL)
                {
//                    EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_STATUS_2),
//                                        (BYTE *)SKY_GET_TOKEN(TKN_BLANK));
                    if(ui_curr_call_is_sms())
                    {
                        uasms_DTC_disconnect();
                    }

                    l_unTrial++;

                    EQC_SetStaticText( EQS_GetNthIOC(SSA_FRAME_STATUS_2),
                                       (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SEND_RETRIAL));

                    EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_APP , EQS_NA);
                    EQS_StartTimer(EQS_TIMER_2, 3*1000, FALSE); 
                }
                else
                {
                    l_unTrial = 0;
                    memset(tmpBuf, NULL, LCD_COLUME_SIZE*4+1);

                    if((EQS_GET_ANCESTOR(2) == sSKA_MOSendMsg) && ( g_nDongboNow == 1 ) )
                    {
                        sprintf( (CHAR *)tmpBuf, (const CHAR *)SKY_GET_TOKEN(TKN_SMS_MSG_NOT_SEND), szTmpStr);
                        hResult = EQC_BeepMessageBox(
							        (BYTE *)tmpBuf,
							        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                    }
                    else
                    {   
                        SaveSMSMOSendMsg(&g_UIMOBuffer);    

                        sprintf( (CHAR *)tmpBuf, (const CHAR *)SKY_GET_TOKEN(TKN_SMS_MSG_FAIL), szTmpStr);
                        hResult = EQC_BeepMessageBox(
							        (BYTE *)tmpBuf,
							        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
                    }
                    EQC_SET_USERDATA(hResult, SMS_RESULT_FAIL);
                }
            }            
            break;
        }        
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SMS_RESULT_SUCCESS)   //성공 
        { 
            if( EQS_GET_PARENT() == SKA_SMSEditMessage) 
            {
                EQS_END_DEST_EVENT((EQS_APPLET *)SKA_SendMsg, SE_APP_RESUME, EQS_NA, SMS_SEND_SUCCESS);
            }
            else
            {
                EQS_END_EVENT(SE_APP_RESUME, EQS_NA, SMS_SEND_SUCCESS);  
            }
        }
        else if(pEvent->dwParam == SMS_RESULT_FAIL) //실패 
        {
            if(ui_curr_call_is_sms())
            {
                uasms_DTC_disconnect();
            }

            if( g_nDongboTotal > g_nDongboNow ) // Remain other data
            {
                sMsgSendControls(); // Add Control
            }
            else
            {
                if( EQS_GET_PARENT() == SKA_SMSEditMessage)
                {
                    EQS_END_DEST_EVENT((EQS_APPLET *)SKA_SendMsg, SE_APP_RESUME, EQS_NA, SMS_SEND_FAIL);
                }
                else
                {
                    EQS_END_EVENT(SE_APP_RESUME, EQS_NA, SMS_SEND_FAIL);
                }
            }
        }

        break;

    case SE_TM_TIMER1:
        SKY_TRACE(TL_HIGH, "SMS MO FAIL:timeout");
        if(ui_curr_call_is_sms())
        {
            uasms_DTC_disconnect();
        }

        if(l_unTrial < NUM_OF_SEND_RETRIAL)
        {
//            EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_STATUS_2),
//                                (BYTE *)SKY_GET_TOKEN(TKN_BLANK));

            l_unTrial++;

            EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_STATUS_2),
                                (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SEND_RETRIAL));

            EQS_StartTimer(EQS_TIMER_2, 3*1000, FALSE); 
        }
        else
        {
            l_unTrial = 0;
            memset(tmpBuf, NULL, LCD_COLUME_SIZE*4+1);
            if( EQS_GET_ANCESTOR(2) == sSKA_MOSendMsg )
            {
                sprintf( (CHAR *)tmpBuf, (const CHAR *)SKY_GET_TOKEN(TKN_SMS_MSG_NOT_SEND), szTmpStr);
                hResult = EQC_BeepMessageBox(
							(BYTE *)tmpBuf,
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            }
            else
            {                
                SaveSMSMOSendMsg(&g_UIMOBuffer); 
                
                sprintf( (CHAR *)tmpBuf, (const CHAR *)SKY_GET_TOKEN(TKN_SMS_MSG_FAIL), szTmpStr);
                hResult = EQC_BeepMessageBox(
							(BYTE *)tmpBuf,
							MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);
            }

            EQC_SET_USERDATA(hResult, SMS_RESULT_FAIL);
        }
        break;

    case SE_TM_TIMER2:
//        EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_STATUS_2),
//                            (BYTE *)SKY_GET_TOKEN(TKN_BLANK));
        memset(&uinuasms_mo_status[0], NULL, sizeof(uasms_status_e_type));
        uasms_DTC_connect();
//        SendSMSMsg(&g_UIMOBuffer,nTI);        

        EQC_SetStaticText(EQS_GetNthIOC(SSA_FRAME_STATUS_2),
                            (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_SENDING));

        EQS_StartTimer(EQS_TIMER_1, 10*1000, FALSE); 
        break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
        if(ui_curr_call_is_sms())
        {
            uasms_DTC_disconnect();
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();       
        }
        break;

    case SE_KEY_CLEAR:    
        if(ui_curr_call_is_sms())
        {
            uasms_DTC_disconnect();
            EQS_END();
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();       
        }
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#else

LOCAL VOID SKY_API SKA_SMSSendMsg(SKY_EVENT_T *pEvent)
{

}    
#endif // ifndef WIN32    

BYTE* sFormatDateInfo(DWORD dwTime)
{
    STATIC clk_julian_type  tm;
    BOOL                    fAM;
    UINT8                   unHour;

    STATIC CHAR s_szFormatTime[LCD_COLUME_SIZE] = {NULL, };
    clk_secs_to_julian(dwTime, &tm);

    if(tm.hour < 12)
    {
        fAM = TRUE;
        unHour = tm.hour;
    }
    else
    {
        fAM = FALSE;
        if(tm.hour == 24)
            unHour = 0;
        else
            unHour = tm.hour - 12;
    }

    sprintf(s_szFormatTime, (CHAR *)SKY_GET_TOKEN(TKN_SMS_DATE), tm.month, tm.day, unHour, tm.minute,
                        (fAM)? (CHAR *)SKY_GET_TOKEN(TKN_IDLE_AM) : (CHAR *)SKY_GET_TOKEN(TKN_IDLE_PM));
    return((BYTE *)s_szFormatTime);   
}

LOCAL VOID SKY_API sSKA_ViewMOSaveMsg(SKY_EVENT_T *pEvent)
{
    STATIC UINT8            s_nDelIndex;

    HCONTROL                hText, hDelMesgBox;
    UINT8                   nCtrl;
    

    switch(pEvent->EventID)
    {   
    case SE_APP_START:
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      130, 
                      CL_NONE_PASTE_NONE,
                      SKY_GET_TOKEN(TKN_SMS_COMMON_PHRASE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        if(EQS_GET_ANCESTOR(2) == SKA_SendMsg)
        {
            EQS_SetWindowCML(CL_DELETE_WRITE_NONE);
        }            

        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_MSG_WRITE_DATE);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_CHRI_CONTENTS);
        hText = EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 3, TEM_KOREAN, 80, 
                        (BYTE*)l_pCurViewElement->pszMsg, 0);
        EQC_GetTextEditLine(hText);
        EQC_FitTextEditLine(hText);
        
        s_nDelIndex = pEvent->dwParam; //delete 시 index 값 

        EQC_SetStaticText(EQS_GetNthIOC(SVMSAVE_DATE), 
                            (BYTE *)sFormatDateInfo(l_pCurViewElement->dwMadeTime));
        // Formatting..
        EQS_StartFormatter();        

        for(nCtrl = 0; nCtrl < SVMSAVE_MAX; nCtrl++)
        {
            if( (nCtrl == SVMSAVE_DATE)
                ||(nCtrl == SVMSAVE_MESG) )
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
            }
            else
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            }

            EQS_NewLine();
        }

        EQS_EndFormatter();  
        
        EQC_SET_STYLE(EQS_GetNthIOC(SVMSAVE_MESG),
            CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY );

        EQC_SET_FOCUS(EQS_GetNthIOC(SVMSAVE_MESG), FALSE);
        
        EQS_StartTimer(EQS_TIMER_1, 1*1000, TRUE);

        EQS_FitCanvasSize();

        break;


    case SE_APP_END :   /* The very last event */ 
        EQS_StopTimer(EQS_TIMER_1);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_TM_TIMER1:
        EQS_SendEvent((EQS_EVENT_ID)SE_KEY_DOWN, EQS_DRAW_LEVEL_APP, EQS_NA);
        break;

    case SE_KEY_UP:
        EQS_StopTimer(EQS_TIMER_1);
        EQS_PASS_CURRENT_EVENT();
        break;

    case SE_SOFTKEY_DELETE:
        if(EQS_GET_ANCESTOR(2) == SKA_SendMsg)
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_23),  //문장을 삭제할까요?:
                MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);       
        }
        break;

    case SE_SOFTKEY_PASTE:
        if(EQS_GET_ANCESTOR(2) == SKA_SMSEditMessage)
        {
            EQS_END_PARAM(EQS_NA, pEvent->EventID);
        }
        break;

    case SE_SOFTKEY_WRITE:
        if(EQS_GET_ANCESTOR(2) == SKA_SendMsg)
        {
            EQS_START_CHILD(SKA_SMSEditMessage);
            g_UIMOBuffer = *l_pCurViewElement;                  
            memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1); 
            g_UIMOBuffer.nRcvNumberSize = 0; 
        }
        break;

    case SE_IO_MESGBOX_YES:
        //delete from data list
        DeleteSMSMOSaveMsg(s_nDelIndex);     
        hDelMesgBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_24),//문장이 삭제되었습니다 
                                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        EQC_SET_USERDATA(hDelMesgBox, MESG_DELETE_CONFIRM); 
        break;
    
    case SE_IO_MESGBOX_NO:
        EQS_END();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == MESG_DELETE_CONFIRM)
        {
            EQS_END_PARAM(EQS_NA, MESG_DELETE_CONFIRM);
        }        
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL VOID SKY_API sSKA_MakeMOSaveMsg(SKY_EVENT_T *pEvent)
{
    UINT8       nCtrl;
    HCONTROL    hMsg;

    switch(pEvent->EventID)
    {   
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      120, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_SMS_COMMON_PHRASE), 
                      WA_SHOW_HEADER);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_MAKE_SAVE_MSG);
        EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 5, TEM_KOREAN, MAX_SMS_TEXT, NULL, 0);

        memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));    
                // Formatting..
        EQS_StartFormatter();
        
        for(nCtrl = 0; nCtrl < SMMS_MAX; nCtrl++)
        {
            if(nCtrl == SMMS_MAKE_MOSAVE)
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            }
            else
            {

                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            }
            EQS_NewLine();
        }

        EQS_EndFormatter();

        EQC_SET_STYLE( EQS_GetNthIOC(SMMS_MAKE_MOSAVE), CS_TEXT_EDIT_DEFAULT|CS_TEXT_EDIT_ALONE);

        EQS_SetIOCActive(EQS_GetNthIOC(SMMS_MAKE_MOSAVE));
        EQS_FitCanvasSize();
        break;

    case SE_IO_TEDIT_DONE:
        if(pEvent->dwParam > 0)
        {
            g_UIMOBuffer.nMsgSize = EQC_GetTextEditData(EQS_GetNthIOC(SMMS_MAKE_MOSAVE), g_UIMOBuffer.pszMsg);
            SaveSMSMOSaveMsg(&g_UIMOBuffer);

            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_SAVED),
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, FINISH_MAKE_MOSAVE);
        }
        else
        {
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT_IN_SAVEBOX),
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, NO_MSG_CONTENT_IN_SAVEBOX); 
        }
        break;
        
    case SE_IO_TEDIT_CANCEL:
        EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        SaveSMSMOSaveMsg(&g_UIMOBuffer);
        hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_SAVED),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        EQC_SET_USERDATA(hMsg, FINISH_MAKE_MOSAVE);
        break;

    case SE_IO_MESGBOX_NO:
        EQS_END_TO(EQS_POP_TO_ROOT);        
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == FINISH_MAKE_MOSAVE)
        {
            EQS_END_PARAM(TRUE, (DWORD)g_UIMOBuffer.pszMsg);            
        }    
        else if(pEvent->dwParam == NO_MSG_CONTENT_IN_SAVEBOX)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(SMMS_MAKE_MOSAVE));        
        }
        break;

    case SE_KEY_END:
        g_UIMOBuffer.nMsgSize = EQC_GetTextEditData(EQS_GetNthIOC(SMMS_MAKE_MOSAVE), g_UIMOBuffer.pszMsg);
        if(g_UIMOBuffer.nMsgSize > 0)
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_ASK_SAVE),
                            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);             
        }
        else
        {
            EQS_PASS_CURRENT_EVENT();
        }
        break;       

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }        
}    

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Xxx()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_MOSaveMsg(SKY_EVENT_T *pEvent)
{
    STATIC  UINT8       s_nDelIndex;
    STATIC  HCONTROL    s_hScrlList;

    UINT8               nIndex, nRepIndex, nScrollList;
    BYTE**              ppScrollList;
    HCONTROL            hMsg;

    SKY_TOKEN paMOSavePopMenu[]=
    {
        TKN_SMS_MOSAVE_DELETE,  //  삭제   
        TKN_SMS_MOSAVE_DELETE_ALL    //  전부삭제
    };


    switch(pEvent->EventID)
    {   
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      130, 
                      CL_NONE_CONFIRM_PASTE,
                      SKY_GET_TOKEN(TKN_SMS_COMMON_PHRASE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        if(EQS_GET_PARENT() == SKA_SendMsg) 
        {
            if( g_nMOSaveSize > 0 )
            {
                EQS_SetWindowCML(CL_MENU_SELECT_NEWARTICLE);                       
            }
            else
            {
                EQS_SetWindowCML(CL_NONE_NONE_NEWARTICLE);                       
            }
        }
     
        ppScrollList = MakeMOSaveScrollList();

        s_hScrlList = EQC_ScrollList(EQS_NA, EQS_NA, EQS_NA, EQS_NA,
            ppScrollList, g_nMOSaveSize, g_nMOSaveSize, 0);

        // first message
        l_pCurViewElement = (SMSMO_SEND_T *)SKY_Calloc(1, sizeof(SMSMO_SEND_T));

        SKY_SeekReadMOSaveT(g_aSMSSaveMsg[0].nAddr, l_pCurViewElement);

        EQC_SET_STYLE( EQS_GetNthIOC(SMS_SCROLL_LIST_1),
                        CS_SCROLL_LIST_DEFAULT 
                        | CS_SCROLL_LIST_ENUMBERATED 
                        | CS_SCROLL_LIST_MAXIMIZE
                        | CS_SCROLL_LIST_NO_CML );

        EQS_SetIOCActive(EQS_GetNthIOC(SMS_SCROLL_LIST_1));
    
        EQS_FitCanvasSize();        
            
        break;
        
    case SE_APP_END :   /* The very last event */ 
        if(EQS_GET_PARENT() == SKA_SendMsg)
        {
            SKY_Free(l_pCurViewElement);
        }

        FreeSMSMOScrollList(g_nMOSaveSize);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
       
        if(EQS_GET_PARENT() == SKA_SMSEditMessage)
        {
            if( EQS_GET_CHILD() == sSKA_ViewMOSaveMsg )
            {
                if( pEvent->dwParam == SE_SOFTKEY_PASTE )
                {
                    EQS_END_PARAM(EQS_NA, pEvent->dwParam);            
                }
            }
        }
        else
        {
            if( (EQS_GET_CHILD() == sSKA_ViewMOSaveMsg)
                &&(pEvent->dwParam == MESG_DELETE_CONFIRM) )
            {
                //delete from scroll list control
                EQC_DeleteScrollData( EQS_GetNthIOC(SMM_SCROLL_LIST_1), s_nDelIndex );

                //replace another message
                if(g_nMOSaveSize == 0)
                {
                    EQS_END();
                }
                else
                {
                    nRepIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SMS_SCROLL_LIST_1));

                    SKY_SeekReadMOSaveT(g_aSMSSaveMsg[nRepIndex].nAddr, l_pCurViewElement);                
                }
            }
            else if( EQS_GET_CHILD() == sSKA_MakeMOSaveMsg )
            {
                if( pEvent->wParam == TRUE )
                {
                    nScrollList = EQC_GetScrollMaxDataLine(EQS_GetNthIOC(SMS_SCROLL_LIST_1));     
                    ppScrollList = AddMOSaveScrollList((BYTE *)pEvent->dwParam, nScrollList);
                    EQC_AddScrollData(EQS_GetNthIOC(SMS_SCROLL_LIST_1), 0, ppScrollList[0], FALSE);
                    sSortMOSaveUIBuffer(); //Sorting the g_aSMSSaveMsg structure
                }     
                if( g_nMOSaveSize > 0 )
                {
                    EQS_SetWindowCML(CL_MENU_SELECT_NEWARTICLE);                       
                }
                else
                {
                    EQS_SetWindowCML(CL_NONE_NONE_NEWARTICLE);                       
                }                
            }
        }

        EQS_SetIOCActive(EQS_GetNthIOC(SMS_SCROLL_LIST_1));

        break;

    case SE_IO_SCROLL_LIST_CHG:
        nIndex = (UINT8)pEvent->dwParam;
        SKY_SeekReadMOSaveT(g_aSMSSaveMsg[nIndex].nAddr, l_pCurViewElement);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        nIndex = (UINT8)pEvent->dwParam;
        SKY_SeekReadMOSaveT(g_aSMSSaveMsg[nIndex].nAddr, l_pCurViewElement);
        s_nDelIndex = nIndex;

        EQS_START_CHILD_EVENT(sSKA_ViewMOSaveMsg, SE_APP_START, EQS_NA, nIndex);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            //delete from data list
            s_nDelIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SMS_SCROLL_LIST_1));
            DeleteSMSMOSaveMsg(s_nDelIndex);
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_24),//메시지가 삭제되었습니다 
                                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER); 
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            DeleteAllSaveMsg();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_16),//메시지가 삭제되었습니다 
                                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER); 
        }
        break;
    
    case SE_IO_MESGBOX_NO:
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            //delete from scroll list control
            EQC_DeleteScrollData( EQS_GetNthIOC(SMM_SCROLL_LIST_1), s_nDelIndex );

            //replace another message
            if(g_nMOSaveSize == 0)
            {
                EQS_END();
            }
            else
            {
                nRepIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SMM_SCROLL_LIST_1));

                SKY_SeekReadMOSaveT(g_aSMSSaveMsg[nRepIndex].nAddr, l_pCurViewElement);
            }
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            EQS_END();
        }
        break;

    case SE_IO_POPUP_SELECT:
        if(EQS_GET_PARENT() == SKA_SendMsg)       
        {
            switch(pEvent->dwParam)
            {
            case 0:
                hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_23),  //메시지를 삭제할까요?:
                        MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM); 
                EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);
                break;
            case 1:                
                hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_12),  //메시지를 전부 삭제할까요?:
                        MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM); 
                EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);
                break;
            }
        }           
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX*)paMOSavePopMenu, 2, 0);
        break;
       
    case SE_SOFTKEY_PASTE:
        EQS_END_PARAM(EQS_NA, pEvent->EventID);
        break;

    case SE_SOFTKEY_NEWARTICLE:
        if( EQS_GET_PARENT() == SKA_SendMsg )
        {
            if( g_nMOSaveSize >= MAX_SMS_MO)
            {
                hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NOT_SAVE_IN_SAVEBOX),
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            }
            else
            {
                EQS_START_CHILD(sSKA_MakeMOSaveMsg);
            }
        }
        break;

    case SE_KEY_CLEAR:
        EQS_END_PARAM(EQS_NA, pEvent->EventID);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL VOID SKY_API sSKA_ViewMOSentMsg(SKY_EVENT_T *pEvent)
{
    STATIC UINT8            s_nDelIndex;

    HCONTROL                hText, hDelMesgBox;
    UINT8                   nCtrl;
    BYTE*                   TmpBuf;
    BYTE                    szNameBuf[SMS_SCREEN_SIZE+1];

    switch(pEvent->EventID)
    {   
    case SE_APP_START:
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      130, 
                      CL_DELETE_WRITE_NONE,                        
                      SKY_GET_TOKEN(TKN_SMS_OUTGONE_BOX), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_MSG_SENT_DATE);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_CHRI_CONTENTS);
        hText = EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 3, TEM_KOREAN, 80, 
                        (BYTE*)l_pCurViewElement->pszMsg, 0);
        EQC_GetTextEditLine(hText);
        EQC_FitTextEditLine(hText);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_OTHER_PARTY_CALL_NUM);

        memset(szNameBuf, NULL, SMS_SCREEN_SIZE+1);

        if( ( TmpBuf = FindNumber((BYTE*)l_pCurViewElement->pszRcvNumber) ) != NULL )
        {
            if(STRLEN(TmpBuf) > SMS_SCREEN_SIZE )
            {
                (void) memcpy ( szNameBuf, 
                                (const void *) TmpBuf,
                                SMS_SCREEN_SIZE );
            }
            else
            {
                (void) memcpy ( szNameBuf, 
	  			                (const void *) TmpBuf, 
                                STRLEN(TmpBuf) );
            }
            EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 1, TEM_KOREAN, 
                                    SMS_SCREEN_SIZE, szNameBuf, 0);
        }
        else
        {            
            EQC_NumberEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 
                        1, 32, (BYTE *)l_pCurViewElement->pszRcvNumber );
        }

        
        if(l_pCurViewElement->nSendNumberSize > 0 )
        {
            EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_CALLBACK);
            EQC_NumberEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 1, 32,
                        (BYTE *)l_pCurViewElement->pszSendNumber);
        }         
        
        s_nDelIndex = pEvent->dwParam; //delete 시 index 값 

        EQC_SetStaticText(EQS_GetNthIOC(SVMSENT_DATE), 
                            (BYTE *)sFormatDateInfo(l_pCurViewElement->dwMadeTime));

        // Formatting..
        EQS_StartFormatter();
        
        if(l_pCurViewElement->nSendNumberSize > 0)
        {
            for(nCtrl = 0; nCtrl < SVMSENT_MAX; nCtrl++)
            {
                if( (nCtrl == SVMSENT_DATE)
                    ||(nCtrl == SVMSENT_MESG)
                    ||(nCtrl == SVMSENT_RCV_NUM)
                    ||(nCtrl == SVMSENT_SEND_NUM) )
                {
                    EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                        ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                    EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
                }
                else
                {

                    EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                            ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);           
                }
                EQS_NewLine();
            }
        }
        else
        {
            for(nCtrl = 0; nCtrl < SVMSENT_MAX-2; nCtrl++)
            {
                if( (nCtrl == SVMSENT_DATE)
                    ||(nCtrl == SVMSENT_MESG)
                    ||(nCtrl == SVMSENT_RCV_NUM) )
                {
                    EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                        ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                    EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
                }
                else
                {

                    EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                            ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);           
                }
                EQS_NewLine();
            }        
        }

        EQS_EndFormatter();  
        
        EQC_SET_STYLE(EQS_GetNthIOC(SVMSAVE_MESG),
            CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY );

        EQC_SET_FOCUS(EQS_GetNthIOC(SVMSENT_MESG), FALSE);
        EQC_SET_FOCUS(EQS_GetNthIOC(SVMSENT_RCV_NUM), FALSE);

        if(l_pCurViewElement->nSendNumberSize > 0)
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(SVMSENT_SEND_NUM), FALSE);
        }

        EQS_StartTimer(EQS_TIMER_1, 1*1000, TRUE);

        EQS_FitCanvasSize();

        break;


    case SE_APP_END :   /* The very last event */ 
        EQS_StopTimer(EQS_TIMER_1);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_TM_TIMER1:
        EQS_SendEvent((EQS_EVENT_ID)SE_KEY_DOWN, EQS_DRAW_LEVEL_APP, EQS_NA);
        break;

    case SE_KEY_UP:
        EQS_StopTimer(EQS_TIMER_1);
        EQS_PASS_CURRENT_EVENT();
        break;

    case SE_SOFTKEY_DELETE:
        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_01),  //메시지를 삭제할까요?:
                MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);       
        break;

    case SE_SOFTKEY_WRITE:
        EQS_START_CHILD(SKA_SMSEditMessage);
        g_UIMOBuffer = *l_pCurViewElement;                  
        break;

    case SE_IO_MESGBOX_YES:
        //delete from data list
        DeleteSMSMOSentMsg(s_nDelIndex);     
        hDelMesgBox = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_02),//메시지가 삭제되었습니다 
                                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        EQC_SET_USERDATA(hDelMesgBox, MESG_DELETE_CONFIRM); 
        break;
    
    case SE_IO_MESGBOX_NO:
        EQS_END();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == MESG_DELETE_CONFIRM)
        {
            EQS_END_PARAM(EQS_NA, MESG_DELETE_CONFIRM);
        }        
        break;

    case SE_KEY_SEND:
        if( l_pCurViewElement->nRcvNumberSize > 0 )
        {            
            SET_NUMEDIT_BUFFER(l_pCurViewElement->pszRcvNumber, l_pCurViewElement->nRcvNumberSize);
            SET_NUMEDIT_POS(l_pCurViewElement->nRcvNumberSize);                

            EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        }
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Xxx()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_MOSentMsg(SKY_EVENT_T *pEvent)
{
    STATIC  UINT8       s_nDelIndex;

    UINT8               nIndex, nRepIndex;
    BYTE**              ppScrollList;
    HCONTROL            hMsg;  

    SKY_TOKEN paMOSentPopMenu[]=
    {
        TKN_SMS_TEXT_DELETE,  //  삭제 
        TKN_SMS_DELETE_ALL    //  전부 삭제
    };    

    switch(pEvent->EventID)
    {   
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      130, 
                      CL_MENU_CONFIRM_NONE,
                      SKY_GET_TOKEN(TKN_SMS_OUTGONE_BOX), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        ppScrollList = MakeMOSentScrollList();

        EQC_ScrollList(2, 0, LCD_COLUME_SIZE, 3,
            ppScrollList, g_nMOSentSize, g_nMOSentSize, 0);

        // first message
        l_pCurViewElement = (SMSMO_SEND_T *)SKY_Calloc(1, sizeof(SMSMO_SEND_T));

        SKY_SeekReadMOSentT(g_aSMSSentMsg[0].nAddr, l_pCurViewElement);

        EQC_SET_STYLE(EQS_GetNthIOC(SMM_SCROLL_LIST_1),
            CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED |CS_SCROLL_LIST_MAXIMIZE);

        EQS_SetIOCActive( EQS_GetNthIOC(SMM_SCROLL_LIST_1));

        EQS_FitCanvasSize();

        break;
        
    case SE_APP_END :   /* The very last event */ 
        SKY_Free(l_pCurViewElement);
        FreeSMSMOScrollList(g_nMOSentSize);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_SetIOCActive( EQS_GetNthIOC(SMM_SCROLL_LIST_1));

        if(pEvent->dwParam == MESG_DELETE_CONFIRM)
        {
            //delete from scroll list control
            EQC_DeleteScrollData( EQS_GetNthIOC(SMM_SCROLL_LIST_1), s_nDelIndex );

            //replace another message
            if(g_nMOSentSize == 0)
            {
                EQS_END();
            }
            else
            {
                nRepIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SMM_SCROLL_LIST_1));

                SKY_SeekReadMOSentT(g_aSMSSentMsg[nRepIndex].nAddr, l_pCurViewElement);
            }
        } 
        break;

    case SE_IO_SCROLL_LIST_CHG:
        nIndex = (UINT8)pEvent->dwParam;
        SKY_SeekReadMOSentT(g_aSMSSentMsg[nIndex].nAddr, l_pCurViewElement);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        nIndex = (UINT8)pEvent->dwParam;
        s_nDelIndex = nIndex;
        SKY_SeekReadMOSentT(g_aSMSSentMsg[nIndex].nAddr, l_pCurViewElement);

        EQS_START_CHILD_EVENT(sSKA_ViewMOSentMsg, SE_APP_START, EQS_NA, nIndex);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            s_nDelIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SMM_SCROLL_LIST_1));
            DeleteSMSMOSentMsg(s_nDelIndex);
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_02),//메시지가 삭제되었습니다 
                                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);
        
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            DeleteAllSentMsg();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_14),//메시지가 삭제되었습니다 
                                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);        
        }
        break;
    
    case SE_IO_MESGBOX_NO:
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER )
        {
            //delete from scroll list control
            EQC_DeleteScrollData( EQS_GetNthIOC(SMM_SCROLL_LIST_1), s_nDelIndex );

            //replace another message
            if(g_nMOSentSize == 0)
            {
                EQS_END();
            }
            else
            {
                nRepIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SMM_SCROLL_LIST_1));
                SKY_SeekReadMOSentT(g_aSMSSentMsg[nRepIndex].nAddr, l_pCurViewElement);
            }        
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER )
        {
            EQS_END();
        }
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam)
        {
        case 0:
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_01),  //메시지를 삭제할까요?:
                    MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);  
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);
            break;
        case 1:
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_07),  //메시지를 삭제할까요?:
                    MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);
            break;
        }
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX*)paMOSentPopMenu, 2, 0);
        break;

    case SE_KEY_CLEAR:
        EQS_END();
        break;
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Xxx()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
LOCAL VOID SKY_API sSKA_MOSendMsg(SKY_EVENT_T *pEvent)
{
    STATIC  UINT8       s_nDelIndex;

    UINT8               nIndex, nRepIndex;
    BYTE**              ppScrollList;
    HCONTROL            hMsg; 

    SKY_TOKEN paMOSendPopMenu[] = 
    {
        TKN_SMS_TEXT_DELETE,
        TKN_SMS_DELETE_ALL
    };
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      130, 
                      CL_MENU_CONFIRM_NONE, 
                      SKY_GET_TOKEN(TKN_SMS_OUTGOING_BOX), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        ppScrollList = MakeMOSendScrollList();

        EQC_ScrollList(0, 0, LCD_COLUME_SIZE, EQC_GetScrollDisplayLine(),
            ppScrollList, g_nMOSendSize, g_nMOSendSize, 0);
       
        // first message
        l_pCurViewElement = (SMSMO_SEND_T *)SKY_Calloc(1, sizeof(SMSMO_SEND_T));

        SKY_SeekReadMOSendT(g_aSMSSendMsg[0].nAddr, l_pCurViewElement);

        EQC_SET_STYLE(EQS_GetNthIOC(SCM_SCROLL_LIST_1),
            CS_SCROLL_LIST_DEFAULT | CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );

        EQS_SetIOCActive(EQS_GetNthIOC(SCM_SCROLL_LIST_1));

        EQS_FitCanvasSize();

        break;
        
    case SE_APP_END :   /* The very last event */ 
        FreeSMSMOScrollList(g_nMOSendSize);
        SKY_Free(l_pCurViewElement);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */

        EQS_SetIOCActive(EQS_GetNthIOC(SCM_SCROLL_LIST_1));
        break;

    case SE_IO_SCROLL_LIST_CHG:
        nIndex = (UINT8)pEvent->dwParam;
        SKY_SeekReadMOSendT(g_aSMSSendMsg[nIndex].nAddr, l_pCurViewElement);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        nIndex = (UINT8)pEvent->dwParam;
        s_nDelIndex = nIndex;
        SKY_SeekReadMOSendT(g_aSMSSendMsg[nIndex].nAddr, l_pCurViewElement);

        g_UIMOBuffer = *l_pCurViewElement;
        l_nMOSendDelIndex = s_nDelIndex;  
        EQS_START_CHILD(SKA_SMSEditMessage);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            //delete from data list
            s_nDelIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SCM_SCROLL_LIST_1));
            DeleteSMSMOSendMsg(s_nDelIndex);
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_02),//메시지가 삭제되었습니다 
                                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);        
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            DeleteAllSendMsg();
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_15),//메시지가 삭제되었습니다 
                                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);        
        }
        break;
    
    case SE_IO_MESGBOX_NO:
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            //delete from scroll list control
            EQC_DeleteScrollData( EQS_GetNthIOC(SCM_SCROLL_LIST_1), s_nDelIndex );

            //replace another message
            if(g_nMOSendSize == 0)
            {
                EQS_END();
            }
            else
            {
                nRepIndex = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SCM_SCROLL_LIST_1));
                SKY_SeekReadMOSendT(g_aSMSSendMsg[nRepIndex].nAddr, l_pCurViewElement);
            }            
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            EQS_END();
        }
        break;

    case SE_IO_POPUP_SELECT:
        switch(pEvent->dwParam)
        {
        case 0:
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_01),  //메시지를 삭제할까요?:
                    MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);              
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);
            break;
        case 1:
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_06),  //메시지를 삭제할까요?:
                MESSAGE_BOX_TIME_UNLIMIT, MBT_YESNO, MBT_CONFIRM);              
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);
            break;
        }
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX*)paMOSendPopMenu, 2, 0);        
        break;

    case SE_KEY_CLEAR:
        EQS_END();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
// 메시지 작성 재구성 
VOID SKY_API SKA_SMSEditMessage(SKY_EVENT_T *pEvent)
{
    STATIC pb_number_info_type  s_NumberInfo;
    STATIC HCONTROL             s_hPopMenu, s_hTextPopMenu;
    STATIC UINT8                s_nCurPosBefore, nNowCtrl, nCntBlank, hList;

    UINT8                       nCtrl, i, j, nPriority, nIndex, nCurPosAfter, nShort;
    HCONTROL                    hFocusedHnd, hMsg, hControl, hMsgCtrl;
    UINT8                       nSize;
    CHAR                        tmpBuf[LCD_COLUME_SIZE*3+1];

    BOOL                        fNull;

    STATIC BOOL pabSMSMsgMenuItem[7] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };

    SKY_TOKEN paSMSMsgMenuItem[] = {
        TKN_SMS_SAVE_MSG,
        TKN_SMS_CALL_EXIST_MSG,
        TKN_SMS_SEARCH_PBOOK,
        TKN_SMS_SEARCH_RECEIVE_HISTORY,
        TKN_SMS_SEARCH_SEND_HISTORY,
        TKN_SMS_SEND_LATER,
        TKN_SMS_DELALL_SENDNUMBER
    };

    SKY_TOKEN paSMSTextEditMenuItem[] = {
        TKN_SMS_SAVE_MSG,
        TKN_SMS_CALL_EXIST_MSG
    };
   
    switch(pEvent->EventID)
    {

    case SE_APP_START :   /* The very first event */
        
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
//                      120, 
                      CL_MENU_INPUT_SEND,//CML_MENU, 
                      SKY_GET_TOKEN(TKN_SMS_EDIT_MSG_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
                    
        if(EQS_GET_PARENT() == SKA_SendMsg)
        {
            memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));
            /* 자기 번호를 얻는 함수 ?*/
            STRCPY(g_UIMOBuffer.pszSendNumber, GetMyNumber());
            g_UIMOBuffer.nSendNumberSize = STRLEN(g_UIMOBuffer.pszSendNumber); 
        }
        else
        {
            if( (EQS_GET_PARENT() != sSKA_ViewMOSentMsg)
                &&(EQS_GET_PARENT() != sSKA_MOSendMsg) )
            {
                /* 자기 번호를 얻는 함수 ?*/
                STRCPY(g_UIMOBuffer.pszSendNumber, GetMyNumber());
                g_UIMOBuffer.nSendNumberSize = STRLEN(g_UIMOBuffer.pszSendNumber);
            }
            else
            {
                if((EQS_GET_PARENT() == sSKA_ViewMOSentMsg)
                    &&(g_UIMOBuffer.nSendNumberSize == 0))
                {
                    /* 자기 번호를 얻는 함수 ?*/
                    STRCPY(g_UIMOBuffer.pszSendNumber, GetMyNumber());
                    g_UIMOBuffer.nSendNumberSize = STRLEN(g_UIMOBuffer.pszSendNumber);                
                }
            }
        }

        memset(&l_DongboSendBuf, NULL, sizeof(SMSMO_DONGBO_SEND_T));

        g_nDongboLast = 0;
        g_nDongboNow = 0; 

        if(STRLEN(g_UIMOBuffer.pszRcvNumber) < MAX_DONGBO_DIAL)
        {
            STRCPY(l_DongboSendBuf.pszRcvNumber[0], g_UIMOBuffer.pszRcvNumber);
            l_DongboSendBuf.nRcvNumberSize[0] = STRLEN(g_UIMOBuffer.pszRcvNumber);
        }
        else
        {
            STRNCPY(l_DongboSendBuf.pszRcvNumber[0], g_UIMOBuffer.pszRcvNumber, MAX_DONGBO_DIAL-1);
            l_DongboSendBuf.nRcvNumberSize[0] = MAX_DONGBO_DIAL-1;
        }


        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_EDIT_MSG); //메시지내용
        EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 5, TEM_KOREAN, MAX_SMS_TEXT, (BYTE *)(g_UIMOBuffer.pszMsg), 0);

        // Msg Kind Scroll List
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_MSG_KIND);               // 편지종류         
        
        l_pszMsgKind[0] = (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_KIND_NORMAL);
        l_pszMsgKind[1] = (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_KIND_URGENT);
        l_pszMsgKind[2] = (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_KIND_EMERGENCY);
        
        switch(g_UIMOBuffer.nPriority)
        {
        case UASMS_PRIORITY_URGENT:
            nIndex = 1;
            break;
        case UASMS_PRIORITY_EMERGENCY:
            nIndex = 2;
            break;
        default:
            nIndex = 0;
            break;
        }            
        EQC_ScrollCombo(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, NUM_OF_MSG_KIND, l_pszMsgKind, NUM_OF_MSG_KIND, NUM_OF_MSG_KIND, nIndex);        
         /* get the number of MO Send Message */
        GetMOSendSize(&g_nMOSendSize);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_CALLBACK);           // 연락받을 번호 
        EQC_NumberEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 1, MAX_SMS_DIAL, (BYTE*)g_UIMOBuffer.pszSendNumber);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_OTHER_PARTY_CALL_NUM); // 상대방 전화번호 

        for( i =0; i < MAX_DONGBO_SIZE; i ++)
        {
            EQC_StaticText(EQS_NA, EQS_NA, (WORD)(TKN_SMS_DONGBO_NULL01 + i));
            hControl = EQC_NumberEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE-2, 1, MAX_DONGBO_DIAL, (BYTE*)l_DongboSendBuf.pszRcvNumber[i]);
            if( i > 0 )
            {
                EQC_SET_FOCUS(hControl, FALSE);
            }
        }

        // Formatting..
        EQS_StartFormatter();
        
        for(nCtrl = 0; nCtrl < SEM_MAX; nCtrl++)
        {
            if( (nCtrl == SEM_MESG_CONTENT) 
                ||(nCtrl == SEM_MY_NUM)
                ||(nCtrl == SEM_MESG_KIND) )
            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
            }
            else if((nCtrl >= SEM_OTHER_BLACK_NUM01) && (nCtrl <= SEM_OTHER_PARTY_NUM20)) 
            {
                if (nCtrl == SEM_OTHER_BLACK_NUM01)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM1);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM02)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM2);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM03)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM3);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM04)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM4);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM05)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM5);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM06)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM6);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM07)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM7);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM08)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM8);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM09)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM9);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM10)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM10);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM11)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM11);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM12)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM12);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM13)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM13);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM14)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM14);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM15)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM15);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM16)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM16);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM17)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM17);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM18)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM18);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM19)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM19);  
                else if (nCtrl == SEM_OTHER_BLACK_NUM20)
                    EQC_SetStaticText(EQS_GetNthIOC(nCtrl), (BYTE*)SF_BLACK_NUM20); 
                
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
                nCtrl++;
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

                EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
            }
            else
            {

                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            }

            EQS_NewLine();

        }

        EQS_EndFormatter();

        if(STRLEN(g_UIMOBuffer.pszRcvNumber) > 0)
        {
            EQC_SET_USERDATA(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01), SMS_READ_VALUE);
            EQC_SET_FOCUS(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM02), TRUE);
            g_nDongboLast = 1;
        }

        for(nCtrl = SEM_OTHER_PARTY_NUM01; nCtrl < SEM_OTHER_PARTY_NUM20 +1; nCtrl = nCtrl+2)
        {
            EQC_SET_STYLE(EQS_GetNthIOC(nCtrl), 
                        CS_NUMBER_EDIT_DEFAULTS | CS_NUMBER_EDIT_CURSOR );
        }

        EQC_SET_STYLE(EQS_GetNthIOC(SEM_MY_NUM), 
                    CS_NUMBER_EDIT_DEFAULTS | CS_NUMBER_EDIT_CURSOR );

        EQC_SET_STYLE(EQS_GetNthIOC(SEM_MESG_KIND),
                    EQC_GET_STYLE(EQS_GetNthIOC(SEM_MESG_KIND))| CS_SCROLL_LIST_ENUMBERATED );

        EQC_SET_STYLE(EQS_GetNthIOC(SEM_MESG_CONTENT),
            EQC_GET_STYLE(EQS_GetNthIOC(SEM_MESG_CONTENT))| CS_TEXT_EDIT_NO_LCML );

        EQS_FitCanvasSize();    
        break;

    case SE_APP_END :     /* The very last event */
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_RESUME :  /* end child */
//        nNowCtrl = EQS_GetIOCOrder(EQS_GetFocusedIOC());
        if( EQS_GetFocusedIOC() == INVALID_CONTROL_HANDLE )
            nNowCtrl = SEM_OTHER_PARTY_NUM01 + (2 * g_nDongboLast);
        else
            nNowCtrl = EQS_GetIOCOrder(EQS_GetFocusedIOC());

        if((EQS_GET_CHILD() == SKA_SearchBookNumber))
        {

            hList = pEvent->dwParam;
            if( hList > 0 ) // hList ==0 취소로 빠져나옴
            {
                nSize = GetListSize(hList);

                if( nCntBlank == 0 ) // FULL MAX_DONGBO_SIZE
                {
                    EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_DONOT_ADD_TWENTY),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    DeleteList(hList);
                }
                else if( nSize  > nCntBlank ) // OVER MAX_DONGBO_SIZE
                {
                    memset(tmpBuf, NULL, LCD_COLUME_SIZE*3+1);

                    if( nCntBlank > 9 )
                    {
                        sprintf( (CHAR *)tmpBuf, (const CHAR *)SKY_GET_TOKEN(TKN_SMS_ONLY_ADD_TENOVER), nCntBlank);
                        hMsgCtrl = EQC_MessageBox((BYTE *)tmpBuf, MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM); 
                    }
                    else
                    {
                        sprintf( (CHAR *)tmpBuf, (const CHAR *)SKY_GET_TOKEN(TKN_SMS_ONLY_ADD_TENBELOW), nCntBlank);
                        hMsgCtrl = EQC_MessageBox((BYTE *)tmpBuf, MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM); 
                    }
                    EQC_SET_USERDATA(hMsgCtrl, ONLY_ADD_DONGBO);
                }
                else
                {
                    for(i=0; i < nSize; i++)
                    {
                        s_NumberInfo.address = GetListValue(hList,i);
                        if( ReadNumber(&s_NumberInfo) == TRUE )
                        { 
                            // Initialize Buffer
                            memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
                            g_UIMOBuffer.nRcvNumberSize = 0;

                            STRCPY(g_UIMOBuffer.pszRcvNumber, s_NumberInfo.digits);
                            g_UIMOBuffer.nRcvNumberSize = STRLEN(s_NumberInfo.digits);
                        }

                        EQC_SET_USERDATA(EQS_GetNthIOC(nNowCtrl), SMS_READ_VALUE);
                        EQC_SetNumberEditValue(EQS_GetNthIOC(nNowCtrl), g_UIMOBuffer.pszRcvNumber);

                        if( ( g_nDongboLast < MAX_DONGBO_SIZE ) && 
                            ( (g_nDongboLast*2) <= (nNowCtrl - SEM_OTHER_PARTY_NUM01) ) )
                        {
                            g_nDongboLast++;
                            EQC_SET_FOCUS((EQS_GetNthIOC(nNowCtrl) + 2), TRUE);
                        }

                        nNowCtrl = nNowCtrl + 2;

                        for( j = nNowCtrl; j < SEM_OTHER_PARTY_NUM20 + 1; j = j+2 )
                        {
                            if( EQC_GET_USERDATA( EQS_GetNthIOC(j) ) != 0 )
                            {
                                nNowCtrl = nNowCtrl + 2;
                            }
                            else
                                break;
                        }
                    }
                    DeleteList(hList);
                }
            }
            EQS_SetIOCFocus(EQS_GetNthIOC(nNowCtrl)); 
        }
        else if((EQS_GET_CHILD() == SKA_ViewSearchNumber))
        {
            s_NumberInfo.address = pEvent->dwParam;

            if( ReadNumber(&s_NumberInfo) == TRUE )
            { 
                STRCPY(g_UIMOBuffer.pszRcvNumber, s_NumberInfo.digits);
                g_UIMOBuffer.nRcvNumberSize = STRLEN(s_NumberInfo.digits);
            }

            EQC_SET_USERDATA(EQS_GetNthIOC(nNowCtrl), SMS_READ_VALUE);
            EQC_SetNumberEditValue(EQS_GetNthIOC(nNowCtrl),        
                                        g_UIMOBuffer.pszRcvNumber);

            EQS_SetIOCFocus(EQS_GetNthIOC(nNowCtrl)); 
        }
        else if(EQS_GET_CHILD() == SKA_MainCallList)
        {
            if(GetCallStackIndex() != 255)
            { 
                /* 착발신 이력의 상세정보에서 전화번호에 대한 편집이 가능해 졌기 때문에 */
                /* Call Stack Index를 이용한 번호전달에 문제 발생  01.06.05 hjcho       */
                //pCallStack = GetCSKData();
                //STRCPY(g_UIMOBuffer.pszRcvNumber,pCallStack->Stack[GetCallStackIndex()].aDigits);
                //g_UIMOBuffer.nRcvNumberSize = STRLEN(g_UIMOBuffer.pszRcvNumber);

//                EQC_SetNumberEditValue(EQS_GetNthIOC(nNowCtrl), l_DongboSendBuf.pszRcvNumber[g_nDongboNow]); 
                g_UIMOBuffer.nRcvNumberSize = EQC_SetNumberEditValue(EQS_GetNthIOC(nNowCtrl), g_UIMOBuffer.pszRcvNumber);
                EQC_SET_USERDATA(EQS_GetNthIOC(nNowCtrl), SMS_READ_VALUE);
                SetCallStackIndex(255);
            }

//            if(EQS_GetIOCOrder(EQS_GetFocusedIOC()) >= SEM_OTHER_PARTY_NUM01 ) 
            if( nNowCtrl >= SEM_OTHER_PARTY_NUM01 ) 
            {
                if( ( g_nDongboLast < MAX_DONGBO_SIZE ) && 
                    ( g_UIMOBuffer.nRcvNumberSize > 0 ) && 
                    ( (g_nDongboLast*2) <= ( nNowCtrl - SEM_OTHER_PARTY_NUM01 ) ) )
                {
                    g_nDongboLast++;
//                    EQC_SET_FOCUS(EQS_GetNthIOC(EQS_GetIOCOrder(EQS_GetFocusedIOC()) + 2), TRUE);
                    EQC_SET_FOCUS(EQS_GetNthIOC(nNowCtrl + 2), TRUE);
                }
            }

            EQS_SetIOCFocus(EQS_GetNthIOC(nNowCtrl)); // imsi huni   
        } 
        /* 01.06.08 착발신 이력이 잠금 상태일 경우 */
        else if(EQS_GET_CHILD() == SKA_MainStackCode)
             break;

        else if(EQS_GET_CHILD() == sSKA_MOSaveMsg)
        {
            if( (pEvent->dwParam == SE_SOFTKEY_PASTE)
                ||(pEvent->dwParam == SE_IO_SCROLL_LIST_SELECT) )
            {
                nCurPosAfter =
                    EQC_SetTextEditDataPos(EQS_GetNthIOC(SEM_MESG_CONTENT), 
                            l_pCurViewElement->pszMsg, s_nCurPosBefore);
                
                if( nCurPosAfter == 0 )
                {
                    hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NOT_CALL_EXIST_MSG),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    EQC_SET_USERDATA(hMsg, TEXT_EDIT_POPUP_END);
                }
                else
                {
                    EQS_SetIOCActive(EQS_GetNthIOC(SEM_MESG_CONTENT));

                    EQC_SetTextEditPos(EQS_GetNthIOC(SEM_MESG_CONTENT), nCurPosAfter);               
                    g_UIMOBuffer.nMsgSize = 
                        EQC_GetTextEditData(EQS_GetNthIOC(SEM_MESG_CONTENT), g_UIMOBuffer.pszMsg);
                }
            }
            else        //by SE_KEY_CLEAR
            {
                EQC_SetTextEditPos(EQS_GetNthIOC(SEM_MESG_CONTENT), s_nCurPosBefore);              
                EQS_SetIOCActive(EQS_GetNthIOC(SEM_MESG_CONTENT));            
            }
            SKY_Free(l_pCurViewElement);
        }
        else if(EQS_GET_CHILD() == SKA_MainPbookCode)
        {
            if(pEvent->wParam == BOOKCODE_IN_MO)
            {
                if(!FindNumberInBook(g_UIMOBuffer.pszRcvNumber))
                {
                    EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NO_DATA_FROM_PBOOK),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                }   
                break;
            }
        }
        break;

    case SE_SOFTKEY_MENU:
//        nNowCtrl = SEM_OTHER_PARTY_NUM01 + (2 * g_nDongboNow);
        if( EQS_GetFocusedIOC() == INVALID_CONTROL_HANDLE )
            nNowCtrl = SEM_OTHER_PARTY_NUM01 + (2 * g_nDongboLast);
        else
            nNowCtrl = EQS_GetIOCOrder(EQS_GetFocusedIOC());

        memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
        g_UIMOBuffer.nRcvNumberSize = 0;

        if( nNowCtrl >= SEM_OTHER_PARTY_NUM01 )
        {
            g_UIMOBuffer.nRcvNumberSize = 
                    EQC_GetNumberEditValue(EQS_GetNthIOC(nNowCtrl), g_UIMOBuffer.pszRcvNumber);
            
            nCntBlank = 0;

            for( j = nNowCtrl; j < SEM_OTHER_PARTY_NUM20 + 1; j = j+2 )
            {
                if( EQC_GET_USERDATA( EQS_GetNthIOC(j) ) == 0 )
                    nCntBlank++;
            }
        }
        else
        {
            g_UIMOBuffer.nRcvNumberSize = 
                    EQC_GetNumberEditValue(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01), g_UIMOBuffer.pszRcvNumber); 
            nNowCtrl = EQS_GetIOCOrder(SEM_OTHER_PARTY_NUM01);
        }

                /* msg */
        memset(g_UIMOBuffer.pszMsg, NULL, MAX_SMS_TEXT + 1);
        g_UIMOBuffer.nMsgSize = 0;

        g_UIMOBuffer.nMsgSize =
            EQC_GetTextEditData(EQS_GetNthIOC(SEM_MESG_CONTENT), g_UIMOBuffer.pszMsg);
        g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;

        hFocusedHnd = EQS_GetFocusedIOC();
        if( hFocusedHnd == EQS_GetNthIOC(nNowCtrl) || EQS_GetFocusedIOC() == INVALID_CONTROL_HANDLE ) 
//        if( hFocusedHnd == EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01) ) 
        {
                                                // TKN_SMS_SAVE_MSG,
                                                // TKN_SMS_CALL_EXIST_MSG
            pabSMSMsgMenuItem[2] = TRUE;        // TKN_SMS_SEARCH_PBOOK    
            pabSMSMsgMenuItem[3] = TRUE;        // TKN_SMS_SEARCH_RECEIVE_HISTORY
            pabSMSMsgMenuItem[4] = TRUE;        // TKN_SMS_SEARCH_SEND_HISTORY
            pabSMSMsgMenuItem[5] = TRUE;        // TKN_SMS_SEND_LATER
            pabSMSMsgMenuItem[6] = TRUE;        // TKN_SMS_DELALL_SENDNUMBER
        }
        else if( (hFocusedHnd == EQS_GetNthIOC(SEM_MESG_KIND))
                    ||(hFocusedHnd == EQS_GetNthIOC(SEM_MY_NUM)) )
        {
            pabSMSMsgMenuItem[5] = TRUE;    //TKN_SMS_SEND_LATER          
        }
        else if( hFocusedHnd == EQS_GetNthIOC(SEM_MESG_CONTENT) )
        {
            pabSMSMsgMenuItem[0] = TRUE;
            pabSMSMsgMenuItem[1] = TRUE;
            pabSMSMsgMenuItem[5] = TRUE;

            s_nCurPosBefore = EQC_GetTextEditLen(EQS_GetNthIOC(SEM_MESG_CONTENT));              
        }

        s_hPopMenu = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paSMSMsgMenuItem,
                                                    7, 0, pabSMSMsgMenuItem);            
        for(i = 0 ;i < 7; i++)
        {
            pabSMSMsgMenuItem[i] = FALSE;    
        }
        EQC_SET_STYLE(s_hPopMenu, EQC_GET_STYLE(s_hPopMenu) | CS_POPUP_RETURN_TOKEN );

        break;

    case SE_IO_NEDIT_DONE:
        nNowCtrl = EQS_GetIOCOrder(EQS_GetFocusedIOC());

        if(EQS_GetIOCOrder(pEvent->wParam) >= SEM_OTHER_PARTY_NUM01 ) 
//        if(pEvent->wParam == EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01) ) 
        {
            memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
            g_UIMOBuffer.nRcvNumberSize = 0;

            g_UIMOBuffer.nRcvNumberSize = 
                    EQC_GetNumberEditValue(EQS_GetNthIOC(nNowCtrl), // imsi huni
                                            g_UIMOBuffer.pszRcvNumber);

            if( g_UIMOBuffer.nRcvNumberSize > 0 )
            {
                EQC_SET_USERDATA(EQS_GetNthIOC(nNowCtrl), SMS_READ_VALUE);
            }
            else
            {
                EQC_SET_USERDATA(EQS_GetNthIOC(nNowCtrl), 0);
            }

            g_UIMOBuffer.pszRcvNumber[g_UIMOBuffer.nRcvNumberSize] = NULL;

            if( ( g_nDongboLast < MAX_DONGBO_SIZE ) && 
                ( g_UIMOBuffer.nRcvNumberSize > 0 ) && 
                ( (g_nDongboLast*2) <= ( nNowCtrl - SEM_OTHER_PARTY_NUM01 ) ) )
            {
                g_nDongboLast++;
                EQC_SET_FOCUS(EQS_GetNthIOC(EQS_GetIOCOrder(pEvent->wParam) + 2), TRUE);
            }

        }
        else if(pEvent->wParam ==  EQS_GetNthIOC(SEM_MY_NUM) )
        {
            memset(g_UIMOBuffer.pszSendNumber, NULL, MAX_SMS_DIAL + 1);
            g_UIMOBuffer.nSendNumberSize = 0;

            g_UIMOBuffer.nSendNumberSize= 
                    EQC_GetNumberEditValue(EQS_GetNthIOC(SEM_MY_NUM),
                                            g_UIMOBuffer.pszSendNumber);
            g_UIMOBuffer.pszSendNumber[g_UIMOBuffer.nSendNumberSize] = NULL; 
        }
        break;

    case SE_IO_TEDIT_DONE:
        g_UIMOBuffer.nMsgSize
       	=  EQC_GetTextEditData(EQS_GetNthIOC(SEM_MESG_CONTENT), g_UIMOBuffer.pszMsg);
        g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;
        break;

    case SE_IO_TEDIT_KEY_F1:
        memset(g_UIMOBuffer.pszMsg, NULL, MAX_SMS_TEXT + 1);
        g_UIMOBuffer.nMsgSize = 0;

        g_UIMOBuffer.nMsgSize =
            EQC_GetTextEditData(EQS_GetNthIOC(SEM_MESG_CONTENT), g_UIMOBuffer.pszMsg);
        g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;

        s_nCurPosBefore = pEvent->dwParam;
        s_hTextPopMenu = EQC_PopUpMenu((EQS_TOKEN_EX*)paSMSTextEditMenuItem, 2, 0);
        EQC_SET_STYLE(s_hTextPopMenu, EQC_GET_STYLE(s_hTextPopMenu) | CS_POPUP_RETURN_TOKEN );        
        break;

    case SE_IO_SCROLL_LIST_SELECT:
    case SE_IO_SCROLL_LIST_CHG:
        switch(pEvent->dwParam)
        {
        case 0:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_NORMAL; 
            break;
        case 1:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_URGENT;
            break;
        case 2:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_EMERGENCY;
            break;
        }

        break; 

    case SE_IO_POPUP_CANCEL :
        if(pEvent->wParam == s_hTextPopMenu)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(SEM_MESG_CONTENT));            
            EQC_SetTextEditPos(EQS_GetNthIOC(SEM_MESG_CONTENT), s_nCurPosBefore);               
        }            
        break;

    case SE_IO_POPUP_SELECT:    
        switch(pEvent->dwParam)
        {
        case TKN_SMS_SEARCH_PBOOK:     
            //전화번호부 찾기
            if(!SKY_IS_PHONE_BOOK_LOCK())
            {
                if(!FindNumberInBook(g_UIMOBuffer.pszRcvNumber))
                {
                    EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NO_DATA_FROM_PBOOK),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                }               
            }
            else
            {
                EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, \
                    BOOKCODE_IN_MO, TKN_SMS_SEARCH_PBOOK);
            }
            break;

        case TKN_SMS_SEARCH_RECEIVE_HISTORY: 
#ifdef FEATURE_CALL_STACK_SEPERATED
            SetReceivedCallStack(TRUE);
#endif//FEATURE_CALL_STACK_SEPERATED
            /* 01.06.08 착발신 이력이 잠금 상태일 경우 */
            EQS_START_CHILD(SKA_MainStackCode);
            break;

        case TKN_SMS_SEARCH_SEND_HISTORY:
#ifdef FEATURE_CALL_STACK_SEPERATED
            SetReceivedCallStack(FALSE);
#endif//FEATURE_CALL_STACK_SEPERATED
            /* 01.06.08 착발신 이력이 잠금 상태일 경우 */
            EQS_START_CHILD(SKA_MainStackCode); 
            break;

        case TKN_SMS_SAVE_MSG:
            //문장 저장
            if(g_UIMOBuffer.nMsgSize == 0)
            {
                hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NOT_SAVE_DATA), 
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); //저장할 문장이 없습니다.
            }
            else
            {
                if( g_nMOSaveSize == MAX_SMS_MO )
                {
                    hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NOT_SAVE_IN_SAVEBOX), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); //더이상 문장 보관함에 저장할 수 없습니다
                }
                else
                {                
                    SaveSMSMOSaveMsg(&g_UIMOBuffer);
                    hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_SAVE_SAVE_BOX),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS); //저장되었습니다
                }
            }
            EQC_SET_USERDATA(hMsg, TEXT_EDIT_POPUP_END);                
            break;

        case TKN_SMS_CALL_EXIST_MSG:
            //메시지 불러오기 
            if(g_nMOSaveSize == 0)
            {
                hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NO_SAVE_DATA),
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); //문자메시지가 없습니다
                EQC_SET_USERDATA(hMsg, TEXT_EDIT_POPUP_END);
            }
            else
            {             
                EQS_START_CHILD(sSKA_MOSaveMsg);
            }
            break;

        case TKN_SMS_SEND_LATER:    //메시지 저장 
            if(EQS_GET_PARENT() == sSKA_MOSendMsg)
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_ALREADY_MOSEND),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);           
            }
            else
            {
                fNull = FALSE;
                if( g_UIMOBuffer.nRcvNumberSize == 0 )
                {
                    fNull = TRUE;
                    g_UIMOBuffer.nRcvNumberSize = 
                            EQC_GetNumberEditValue(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01), g_UIMOBuffer.pszRcvNumber);
                    SaveSMSMOSendMsg(&g_UIMOBuffer);

                    if( g_UIMOBuffer.nRcvNumberSize == 0 )
                    {
                        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_SAVE_SEND_BOX),
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
                    }
                    else
                    {
                        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_SAVE_SEND_BOX2),
                                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
                    }
                    memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
                    g_UIMOBuffer.nRcvNumberSize = 0;
                }
                else
                {
                    SaveSMSMOSendMsg(&g_UIMOBuffer);

                    EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_SAVE_SEND_BOX1),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
                }
            }
            break;

        case TKN_SMS_DELALL_SENDNUMBER:    //번호 전부 삭제 
            for( i= 0; i < g_nDongboLast; i++ )
            {
                memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
                g_UIMOBuffer.nRcvNumberSize = 0;

                EQC_SetNumberEditValue( EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01 + i*2), 
                                        g_UIMOBuffer.pszRcvNumber);

                EQC_SET_USERDATA(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01 + i*2), 0);
                EQC_SET_FOCUS(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01 + i*2), FALSE);
            }
            g_nDongboLast = 0;
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_DELALL_DONE),
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
            
            EQC_SET_FOCUS(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01), TRUE);
            EQS_SetIOCActive(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01));

            break;
        }
        break;
    
    case SE_SOFTKEY_SEND://SE_SOFTKEY_CONTINUE:

        /*fill the message buffer */
        /* other party number */

        if( EQS_GetActiveIOC() != INVALID_CONTROL_HANDLE &&
            EQC_GET_TYPE( EQS_GetActiveIOC()) == CT_NUMBER_EDIT )
        {
                memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);

                g_UIMOBuffer.nRcvNumberSize = 
                        EQC_GetNumberEditValue(EQS_GetActiveIOC(), g_UIMOBuffer.pszRcvNumber);

                if ( g_UIMOBuffer.nRcvNumberSize > 0 )
                {
                    EQC_SET_USERDATA(EQS_GetActiveIOC(), SMS_READ_VALUE);

                    if( ( g_nDongboLast < MAX_DONGBO_SIZE ) &&
                        ( (g_nDongboLast*2) <= ( EQS_GetIOCOrder(EQS_GetActiveIOC()) - SEM_OTHER_PARTY_NUM01 ) ) )
                    {
                        g_nDongboLast++;
                    }

                } else
                    EQC_SET_USERDATA(EQS_GetActiveIOC(), 0);
        }

        memset(&l_DongboSendBuf, NULL, sizeof(SMSMO_DONGBO_SEND_T));
        g_nDongboNow = 0;
        g_nDongboTotal = 0;

        for ( i=0; i < g_nDongboLast; i++ )
        {
            // Initialize Buffer
            memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
            g_UIMOBuffer.nRcvNumberSize = 0;

            if( EQC_GET_USERDATA( EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01 + (i * 2) ) ) != 0 )
            {
                g_UIMOBuffer.nRcvNumberSize = 
                        EQC_GetNumberEditValue(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01 + (i * 2) ), g_UIMOBuffer.pszRcvNumber);

                STRNCPY(l_DongboSendBuf.pszRcvNumber[g_nDongboNow], g_UIMOBuffer.pszRcvNumber, MAX_DONGBO_DIAL);
                l_DongboSendBuf.nRcvNumberSize[g_nDongboNow] = 
                                      MIN( STRLEN(l_DongboSendBuf.pszRcvNumber[g_nDongboNow]), MAX_DONGBO_DIAL);
                g_nDongboNow++;
            }
        }

        g_nDongboTotal = g_nDongboNow;

        memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
        g_UIMOBuffer.nRcvNumberSize = 0;

        STRCPY(g_UIMOBuffer.pszRcvNumber, l_DongboSendBuf.pszRcvNumber[0]);
        g_UIMOBuffer.nRcvNumberSize = l_DongboSendBuf.nRcvNumberSize[0];

        /* my party number */
        memset(g_UIMOBuffer.pszSendNumber, NULL, MAX_SMS_DIAL + 1);
        g_UIMOBuffer.nSendNumberSize = 0;

        g_UIMOBuffer.nSendNumberSize= 
                EQC_GetNumberEditValue(EQS_GetNthIOC(SEM_MY_NUM),
                                        g_UIMOBuffer.pszSendNumber);
        g_UIMOBuffer.pszSendNumber[g_UIMOBuffer.nSendNumberSize] = NULL; 

        /* msg */
        memset(g_UIMOBuffer.pszMsg, NULL, MAX_SMS_TEXT + 1);
        g_UIMOBuffer.nMsgSize = 0;

        g_UIMOBuffer.nMsgSize =
            EQC_GetTextEditData(EQS_GetNthIOC(SEM_MESG_CONTENT), g_UIMOBuffer.pszMsg);
        g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;

        /* priority */
        nPriority = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SEM_MESG_KIND));
        switch(nPriority)
        {
        case 0:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_NORMAL;
            break;

        case 1:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_URGENT;
            break;

        case 2:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_EMERGENCY;
            break;
        }

        if(g_UIMOBuffer.nRcvNumberSize)
        {
            if(g_UIMOBuffer.nMsgSize == 0)  // message가 없는 경우 
            {
                STRCPY(g_UIMOBuffer.pszMsg, (BYTE *)SKY_GET_TOKEN(TKN_SMS_CALL_ME));  //전화해주십시오 
                g_UIMOBuffer.nMsgSize = (UINT8)STRLEN(g_UIMOBuffer.pszMsg);
            }

            EQS_START_CHILD_EVENT(SKA_SMSSendMsg, SE_APP_START, EQS_NA, TI_DEFAULT);            
        }
        else
        {
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CALL_NUM), 
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hMsg, NO_OTHER_PARTY_CALL_NUM);
        }    
        break;

    case SE_IO_MESGBOX_END:
        if( pEvent->dwParam == NO_OTHER_PARTY_CALL_NUM )
        {
            EQS_SetIOCFocus(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01));              
        }
        else if( pEvent->dwParam == TEXT_EDIT_POPUP_END )
        {
            EQS_SetIOCActive(EQS_GetNthIOC(SEM_MESG_CONTENT));
            EQC_SetTextEditPos(EQS_GetNthIOC(SEM_MESG_CONTENT), s_nCurPosBefore);              
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(pEvent->dwParam == ONLY_ADD_DONGBO)
        {
            DeleteList(hList);
            EQS_SetIOCFocus(EQS_GetNthIOC(nNowCtrl)); 
        }
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == MSG_EDIT_CANCEL)
        {
            EQS_END();
        }
        else if(pEvent->dwParam == ONLY_ADD_DONGBO)
        {
            for(i=0; i < nCntBlank; i++)
            {
                s_NumberInfo.address = GetListValue(hList,i);
                if( ReadNumber(&s_NumberInfo) == TRUE )
                { 
                    // Initialize Buffer
                    memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
                    g_UIMOBuffer.nRcvNumberSize = 0;

                    STRCPY(g_UIMOBuffer.pszRcvNumber, s_NumberInfo.digits);
                    g_UIMOBuffer.nRcvNumberSize = STRLEN(s_NumberInfo.digits);
                }

                EQC_SET_USERDATA(EQS_GetNthIOC(nNowCtrl), SMS_READ_VALUE);
                EQC_SetNumberEditValue(EQS_GetNthIOC(nNowCtrl), g_UIMOBuffer.pszRcvNumber);

                if( ( g_nDongboLast < MAX_DONGBO_SIZE ) && 
                    ( (g_nDongboLast*2) <= (nNowCtrl - SEM_OTHER_PARTY_NUM01) ) )
                {
                    g_nDongboLast++;
                    EQC_SET_FOCUS((EQS_GetNthIOC(nNowCtrl) + 2), TRUE);
                }

                nNowCtrl = nNowCtrl + 2;

                for( j = nNowCtrl; j < SEM_OTHER_PARTY_NUM20 + 1; j = j+2 )
                {
                    if( EQC_GET_USERDATA( EQS_GetNthIOC(j) ) != 0 )
                    {
                        nNowCtrl = nNowCtrl + 2;
                    }
                    else
                        break;
                }
            }
            DeleteList(hList);
            EQS_SetIOCFocus(EQS_GetNthIOC(nNowCtrl)); 
        }
        break;

    case SE_CALL_INCOMING:
        if( EQS_GetActiveIOC() != INVALID_CONTROL_HANDLE &&
            EQC_GET_TYPE( EQS_GetActiveIOC()) == CT_NUMBER_EDIT )
        {
                memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);

                g_UIMOBuffer.nRcvNumberSize = 
                        EQC_GetNumberEditValue(EQS_GetActiveIOC(), g_UIMOBuffer.pszRcvNumber);

                if ( g_UIMOBuffer.nRcvNumberSize > 0 )
                {
                    EQC_SET_USERDATA(EQS_GetActiveIOC(), SMS_READ_VALUE);

                    if( ( g_nDongboLast < MAX_DONGBO_SIZE ) &&
                        ( (g_nDongboLast*2) <= ( EQS_GetIOCOrder(EQS_GetActiveIOC()) - SEM_OTHER_PARTY_NUM01 ) ) )
                    {
                        g_nDongboLast++;
                    }

                } else
                    EQC_SET_USERDATA(EQS_GetActiveIOC(), 0);
        }

        memset(&l_DongboSendBuf, NULL, sizeof(SMSMO_DONGBO_SEND_T));
        g_nDongboNow = 0;
        g_nDongboTotal = 0;

        for ( i=0; i < g_nDongboLast; i++ )
        {
            // Initialize Buffer
            memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
            g_UIMOBuffer.nRcvNumberSize = 0;

            if( EQC_GET_USERDATA( EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01 + (i * 2) ) ) != 0 )
            {
                g_UIMOBuffer.nRcvNumberSize = 
                        EQC_GetNumberEditValue(EQS_GetNthIOC(SEM_OTHER_PARTY_NUM01 + (i * 2) ), g_UIMOBuffer.pszRcvNumber);

                STRNCPY(l_DongboSendBuf.pszRcvNumber[g_nDongboNow], g_UIMOBuffer.pszRcvNumber, MAX_DONGBO_DIAL);
                l_DongboSendBuf.nRcvNumberSize[g_nDongboNow] = 
                                      MIN( STRLEN(l_DongboSendBuf.pszRcvNumber[g_nDongboNow]), MAX_DONGBO_DIAL);
                g_nDongboNow++;
            }
        }

        g_nDongboTotal = g_nDongboNow;

        memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
        g_UIMOBuffer.nRcvNumberSize = 0;

        STRCPY(g_UIMOBuffer.pszRcvNumber, l_DongboSendBuf.pszRcvNumber[0]);
        g_UIMOBuffer.nRcvNumberSize = l_DongboSendBuf.nRcvNumberSize[0];

        /* my party number */
        memset(g_UIMOBuffer.pszSendNumber, NULL, MAX_SMS_DIAL + 1);
        g_UIMOBuffer.nSendNumberSize = 0;

        g_UIMOBuffer.nSendNumberSize= 
                EQC_GetNumberEditValue(EQS_GetNthIOC(SEM_MY_NUM),
                                        g_UIMOBuffer.pszSendNumber);
        g_UIMOBuffer.pszSendNumber[g_UIMOBuffer.nSendNumberSize] = NULL; 

        /* msg */
        memset(g_UIMOBuffer.pszMsg, NULL, MAX_SMS_TEXT + 1);
        g_UIMOBuffer.nMsgSize = 0;

        g_UIMOBuffer.nMsgSize =
            EQC_GetTextEditData(EQS_GetNthIOC(SEM_MESG_CONTENT), g_UIMOBuffer.pszMsg);
        g_UIMOBuffer.pszMsg[g_UIMOBuffer.nMsgSize] = NULL;

        /* priority */
        nPriority = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SEM_MESG_KIND));
        switch(nPriority)
        {
        case 0:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_NORMAL;
            break;

        case 1:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_URGENT;
            break;

        case 2:
            g_UIMOBuffer.nPriority = UASMS_PRIORITY_EMERGENCY;
            break;
        }

        if( EQS_GET_PARENT() == sSKA_MOSendMsg )
        {
            DeleteSMSMOSendMsg(l_nMOSendDelIndex);
        }
        SaveSMSMOSendMsg(&g_UIMOBuffer); 
        
        g_nDongboNow = 1;
        
        if( g_nDongboTotal > g_nDongboNow ) // Remain other data
        {
            for( i = g_nDongboNow; i < g_nDongboTotal; i++ )
            {
                // Initialize Buffer
                memset(g_UIMOBuffer.pszRcvNumber, NULL, MAX_SMS_DIAL + 1);
                g_UIMOBuffer.nRcvNumberSize = 0;

                STRNCPY(g_UIMOBuffer.pszRcvNumber, l_DongboSendBuf.pszRcvNumber[i], MAX_DONGBO_DIAL);
                g_UIMOBuffer.nRcvNumberSize = l_DongboSendBuf.nRcvNumberSize[i];

                SaveSMSMOSendMsg(&g_UIMOBuffer);
            }
        } 

        EQS_PASS_CURRENT_EVENT();            
        break;

    case SE_IO_FOCUS_CHG:
        if( pEvent->wParam == EQS_GetNthIOC(SEM_MESG_KIND))
        {
            EQS_SetWindowCML(CL_MENU_SELECT_SEND);
        }
        else
        {
            EQS_SetWindowCML(CL_MENU_INPUT_SEND);            
        }
        break;

    case SE_KEY_CLEAR:
		if(EQS_IsControlDataChanged())
		{
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_EDIT_MSG_CANCEL),
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM); //취소하시겠습니까?
            EQC_SET_USERDATA(hMsg, MSG_EDIT_CANCEL);
        }
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
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_SendMsg()
**
** Description: 
**     Entrace
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID SKY_API SKA_SendMsg(SKY_EVENT_T *pEvent)
{

    BYTE*   paSMSSentBoxItem[4];
    STATIC  HCONTROL    hSMSMOMainMenu;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE,
                      SKY_GET_TOKEN(TKN_SMS_SENT_BOX), 
                      WA_SHOW_CML);

        paSMSSentBoxItem[0] = (BYTE *)SKY_GET_TOKEN(TKN_SMS_EDIT);
        paSMSSentBoxItem[1] = (BYTE *)SKY_GET_TOKEN(TKN_SMS_OUTGOING_BOX);
        paSMSSentBoxItem[2] = (BYTE *)SKY_GET_TOKEN(TKN_SMS_OUTGONE_BOX);
        paSMSSentBoxItem[3] = (BYTE *)SKY_GET_TOKEN(TKN_SMS_COMMON_PHRASE);


        hSMSMOMainMenu = EQC_ScrollList(5, 25, LCD_COLUME_SIZE, EQC_GetScrollDisplayLine() ,
                            paSMSSentBoxItem, 4, 4, 0);

        EQC_SET_STYLE(hSMSMOMainMenu,
                     CS_SCROLL_LIST_DEFAULT |
                     CS_SCROLL_LIST_BORDER  |
                     CS_SCROLL_LIST_ENUMBERATED |
                     CS_SCROLL_LIST_MAXIMIZE );                  
       
        EQC_SetScrollTitle(hSMSMOMainMenu, (BYTE*)SKY_GET_TOKEN(TKN_SMS_SENT_BOX));        
        EQC_ShowScrollTitle(hSMSMOMainMenu, TRUE );        

        EQS_SetIOCActive(hSMSMOMainMenu);       

        GetMOSendSize(&g_nMOSendSize);
        GetMOSentSize(&g_nMOSentSize);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_SetIOCActive(hSMSMOMainMenu);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
        switch(pEvent->dwParam)
        {
        case 0:
            EQS_START_CHILD(SKA_SMSEditMessage);
            break;
        case 1:
            /* get the number of MO Send Message */
            if(g_nMOSendSize == 0)
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NO_SAVE_MSG),
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); //문자메시지가 없습니다
            }            
            else
            {
                EQS_START_CHILD(sSKA_MOSendMsg);
            }
            break;
        case 2:
            /* get the number of MO Sent Message */
            if(g_nMOSentSize == 0)
            {
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NO_SAVE_MSG),
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); //문자메시지가 없습니다            
            }
            else
            {                
                EQS_START_CHILD(sSKA_MOSentMsg);
            }
            break;
        case 3:
            EQS_START_CHILD(sSKA_MOSaveMsg);            
            break;
        }
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive(hSMSMOMainMenu);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


