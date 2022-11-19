/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_pop3.c
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
** 2001-02-23 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdlib.h>
#include "ska.h"
#include "uisockapi.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define GET_NEW_MAIL

#define POP3_PORT   110
#define CMD_MAX     256
#define	NET_MAX_SIZE	64

#define GET_HOST_NAME()	(host_name)

#define POP3_OK     "+OK"
#define POP3_ERR    "-ERR"
#define POP3_ENDLIST    "."
#define POP3_ENDMAIL1    "\r\n."
#define POP3_ENDMAIL2    ".\r"

#define NUM_CODE    5
#define NUM_DISCRETE    6
#define NUM_COMPOSITE   7

#define	MAX_NESTED		3
#define EMAIL_WAIT_TIME     10000 // 10Sec
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagEMAIL_LIST_TYPE_T EMAIL_LIST_TYPE_T;

typedef struct tagCONTENT_CODES_T CONTENT_CODES_T;
typedef struct tagDISCRETE_TYPES_T DISCRETE_TYPES_T;
typedef struct tagCOMPOSITE_TYPE_T COMPOSITE_TYPE_T;

enum tag_CONTENT_CODE_E {
    ENCODING_7BIT,  
    ENCODING_8BIT,  
    ENCODING_BASE64,
    ENCODING_PRINTABLE,
    ENCODING_QUOTED,
    ENCODING_UNKNOWN
};
typedef enum tag_CONTENT_CODE_E CONTENT_CODE_E;

enum tag_DISCRETE_TYPE_E {
    TYPE_TEXT,  
    TYPE_AUDIO,
    TYPE_IMAGE,
    TYPE_VIDEO,
    TYPE_APP,    
    TYPE_MULTI,
    TYPE_UNKNOWN
};

typedef enum tag_DISCRETE_TYPE_E DISCRETE_TYPE_E;

enum tag_COMPOSITE_TYPE_E {
    TYPE_PLAIN,
    TYPE_HTML,
    TYPE_JPEG,
    TYPE_GIF,
    TYPE_BMP,	
    TYPE_MIXED,
    TYPE_ALTERNATE,
    TYPE_UNKNOWN_COMPOSITE
};
typedef enum tag_COMPOSITE_TYPE_E COMPOSITE_TYPE_E;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagEMAIL_LIST_TYPE_T {
    UINT16  nNumber;
    UINT16  nSize;  
    BYTE    szUIDL[MAIL_UIDL];
};

struct tagCONTENT_CODES_T {
    BYTE    TYPE[20];
    CONTENT_CODE_E  ID;   
};

struct tagDISCRETE_TYPES_T {
    BYTE    TYPE[20];
    DISCRETE_TYPE_E  ID;   
};

struct tagCOMPOSITE_TYPE_T {
    BYTE    TYPE[20];
    COMPOSITE_TYPE_E  ID;   
};

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
ReplyStack_T                POP3_Reply;
BYTE                        SavedReply[MAX_REPLY_LENGTH + 1];

LOCAL EMAIL_POP3_STATE_E    l_POP3State;
LOCAL UINT16                l_nMailNumber;
LOCAL UINT16                l_nTotalMailNumber;
LOCAL UINT16                l_nCurrentMail;
LOCAL EMAIL_LIST_TYPE_T          MailList[MAX_SAVED_MAIL];

LOCAL const CONTENT_CODES_T l_codes[NUM_CODE] = {
	"7bit",		ENCODING_7BIT,
	"8bit",		ENCODING_8BIT,
	"base64",	ENCODING_BASE64,
	"quoted-printable",	ENCODING_PRINTABLE,
	"quoted",	ENCODING_QUOTED
};


LOCAL const DISCRETE_TYPES_T l_discrete[NUM_DISCRETE] = {
	"text",			TYPE_TEXT,
    "audio",    	TYPE_AUDIO,
	"image",		TYPE_IMAGE,
	"video",		TYPE_VIDEO,
	"application",	TYPE_APP,
	"multipart",	TYPE_MULTI
};

LOCAL const COMPOSITE_TYPE_T l_composite[NUM_COMPOSITE] = {
	"plain",		TYPE_PLAIN,
	"html",			TYPE_HTML,
	"jpg",			TYPE_JPEG,
	"gif",			TYPE_GIF,
	"bmp",			TYPE_BMP,	
	"mixed",		TYPE_MIXED,
	"alternative",	TYPE_ALTERNATE
};

LOCAL UINT16  l_nDepth = 0;
LOCAL UINT16  l_nBodyLen = 0;

LOCAL BYTE	l_szBoundary[MAX_NESTED+2][82]; // +2 for safety

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL BOOL SKY_API sSaveCurrentMail(void);
LOCAL UINT8 SKY_API sGetRecvMailNum(BOOL fInit, BOOL fNew);
LOCAL BYTE* SKY_API sFindString(BYTE *szSrc, BYTE *szString, UINT16 nSrcLen);
LOCAL UINT16 SKY_API sDecodeBody_TextPlain(BYTE *pszSrc, BYTE *pszDest,
										 CONTENT_CODE_E EncodeType, UINT16 nLen,
										 UINT16 nMaxLen);
LOCAL UINT16 SKY_API sDecodeBody_TextHtml(BYTE *pszSrc, BYTE *pszDest,
										CONTENT_CODE_E EncodeType, UINT16 nLen,
										UINT16 nMaxLen);

LOCAL void SKY_API sDecode_MultiBody(BYTE* pszPos, 
									 COMPOSITE_TYPE_E CompositeType, 
									 CONTENT_CODE_E EncodeType, 
									 BYTE	*pszBoundary,
									 UINT16 nLen);

LOCAL void SKY_API sRemoveCRLF(void);
LOCAL DISCRETE_TYPE_E SKY_API sFindDiscreteType(BYTE *pszStart, UINT16 nLen);
LOCAL COMPOSITE_TYPE_E SKY_API sFindCompositeType(BYTE *pszStart, UINT16 nLen);
LOCAL CONTENT_CODE_E SKY_API sFindEncodingType(BYTE *pszStart, UINT16 nLen);

LOCAL UINT8 SKY_API sGetRecvMailNum(BOOL fInit, BOOL fNew)
{
    static UINT8   nStart;
    UINT8   i, j;
    BOOL fFound;

#ifdef GET_NEW_MAIL
    if(fInit)
        nStart = l_nMailNumber;
#else
    if(fInit)
        nStart = 1;
#endif

#ifdef GET_NEW_MAIL
    for(i = nStart; i > 0; i--)
#else
    for(i = nStart; i <= MAX_SAVED_MAIL; i++)
#endif
    {
        fFound = FALSE;

        for(j = 0; j < MAX_SAVED_MAIL; j++)
        {
            if(g_MailList[j].nMailNumber == 0)
                continue;

            if(!STRCMP(g_MailList[j].szUIDL, MailList[i-1].szUIDL))
            {
                fFound = TRUE;
                break;
            }
        }

        if(!fFound)
            break;
    }

    if(fNew)
        nStart = i;

#ifdef GET_NEW_MAIL
    nStart --;
    return nStart+1;
#else
    nStart ++;
    return nStart-1;
#endif
}

void SKY_ConnectPOP3Server(void)
{
    SKY_SET_SOCK_MODE(TRUE);

    MSG_HIGH("SKY_ConnectPOP3Server",0,0,0);

	if (SKY_IS_SOCK_CONNECT())
		SvrLinkReconnect((char*)SKY_GET_EMAIL_POP3(), POP3_PORT);
	else
		SvrLinkConnect((char*)SKY_GET_EMAIL_POP3(), POP3_PORT);

	POP3_Reply.reply[0] = 0x00;
    POP3_Reply.nPos = 0;
    POP3_Reply.fValid = FALSE;

    SKY_InitRecvBuffer();
}

void sSendCommand(BYTE* szCommand)
{
	UINT16 nLen;
    
    nLen = STRLEN(szCommand);

	POP3_Reply.reply[0] = 0x00;
    POP3_Reply.nPos = 0;
    POP3_Reply.fValid = FALSE;
	
	SvrLinkSend(szCommand, nLen);
}

void SKY_API SKY_SendRequest(EMAIL_POP3_STATE_E initState, BOOL fReSend, BOOL fNew,
                                HCONTROL hControl)
{
    BYTE szCommand[CMD_MAX];
    BYTE    szTmp[20];
    UINT8   i;

    if(initState != EPS_NONE)
        l_POP3State = initState;

    switch(l_POP3State)
    {
    case EPS_READY:
        // Init Mail List
        for(i = 0; i < MAX_SAVED_MAIL; i++)
        {
            MailList[i].nNumber = 0;
            MailList[i].nSize = 0;
            MailList[i].szUIDL[0] = '\0';
        }
        break;

    case EPS_USER:
        SKY_TRACE(TL_HIGH, "SendPOP3:EPS_USER");
        sprintf((char*)szCommand, "USER %s\r\n", SKY_GET_EMAIL_USERID());
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"EPS_USER");
#else
        EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_AUTH));
        /* rosa 2001-09-26 --> @why, what */
        //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_AUTH))));
        /* <-- */

#endif
        break;

    case EPS_PASS:
        SKY_TRACE(TL_HIGH, "SendPOP3:EPS_PASS");
        sprintf((char*)szCommand, "PASS %s\r\n", SKY_GET_EMAIL_PASSWORD());
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"EPS_PASS");
#else
		EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_AUTH));
        /* rosa 2001-09-26 --> @why, what */
        //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_AUTH))));
        /* <-- */
#endif
        break;

    case EPS_STAT:
        STRCPY(szCommand,(BYTE*)"STAT\r\n");
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"EPS_STAT");
#else
        EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_STATE));
        /* rosa 2001-09-26 --> @why, what */
        //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_STATE))));
        /* <-- */
#endif
        SKY_TRACE(TL_HIGH, "SendPOP3:EPS_STAT");
        break;

    case EPS_LIST:
        SKY_TRACE(TL_HIGH, "SendPOP3:EPS_LIST");
        STRCPY(szCommand,(BYTE*)"LIST\r\n");
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"EPS_LIST");
#else
		EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_STATE));
        /* rosa 2001-09-26 --> @why, what */
        //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_STATE))));
        /* <-- */
#endif
        break;

    case EPS_UIDL:
        SKY_TRACE(TL_HIGH, "SendPOP3:EPS_UIDL");
        sprintf((char*)szCommand, "UIDL\r\n");
        sSendCommand(szCommand);
		EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_STATE));
        /* rosa 2001-09-26 --> @why, what */
        //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_STATE))));
        /* <-- */
        break;

    case EPS_RETR:
        SKY_TRACE1(TL_HIGH, "SendPOP3:EPS_RETR %d", l_nCurrentMail);
        sprintf((char*)szCommand, "RETR %d\r\n", l_nCurrentMail);
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        sprintf((char*)szTmp, "EPS_RETR:%d", l_nCurrentMail);
        EQC_SetStaticText(hControl, szTmp);
#else
		sprintf((char*)szTmp, (char*)SKY_GET_TOKEN(TKN_EMAIL_RECV_CNT_RECEIVING), 
				(l_nMailNumber-l_nCurrentMail+1), l_nTotalMailNumber);
		EQC_SetStaticText(hControl, szTmp);
        /* rosa 2001-09-26 --> @why, what */
        //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)szTmp)));
        /* <-- */
#endif
        break;

    case EPS_DATA:
#ifdef EMAIL_DEBUG
        sprintf((char*)szTmp, "EPS_DATA:%d", l_nCurrentMail);
        EQC_SetStaticText(hControl, szTmp);
#endif
        break;

    case EPS_QUIT:
        SKY_TRACE(TL_HIGH, "SendPOP3:EPS_QUIT");
        STRCPY(szCommand, (BYTE*)"QUIT\r\n");
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"EPS_QUIT");
#else
		EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_QUIT));
        /* rosa 2001-09-26 --> @why, what */        
        //EQC_SET_X(hControl, GET_EMAIL_STATUS_X(EQS_TextWidth((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_QUIT))));
        /* <-- */
#endif

        EQS_StartTimer(EQS_TIMER_3, EMAIL_WAIT_TIME, FALSE);
        break;
    }
}

LOCAL INT SKY_API sRecvPOP3Data(void)
{
    if(l_POP3State == EPS_DATA)
        return SKY_RecvData(&POP3_Reply);
    else if(l_POP3State == EPS_RETR)
        return SKY_RecvReply(&POP3_Reply, SavedReply);
    else
        return SKY_RecvReply(&POP3_Reply, NULL);
}

EMAIL_RECV_RESULT_E SKY_API SKY_ReceivePOP3Reply(HCONTROL hControl, BOOL fNew)
{
    BYTE    szTmp[50];
    BYTE*   pszToken;
    BYTE*   pTmpPos;
    static  UINT16  nCurrent;
    BYTE    szReply[MAX_REPLY_LENGTH + 1];
    BYTE    szSave[3];
    EMAIL_RECV_RESULT_E ReturnValue;
    UINT16 nMailNum, i;
    
    ReturnValue = ERR_CONTINUE;

    while(sRecvPOP3Data() > 0 && ReturnValue == ERR_CONTINUE)
    {
	    if (!POP3_Reply.fValid)
		    continue;

        if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_ERR, 4) &&
            l_POP3State != EPS_DATA)
        {
            if(l_POP3State == EPS_USER)
                ReturnValue = ERR_USER;
            else if(l_POP3State == EPS_PASS)
                ReturnValue = ERR_PASS;
            else
                ReturnValue = ERR_FAIL;
            break;
        }

        switch(l_POP3State)
        {
        case EPS_READY:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_READY");

            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_OK, 3))
            {
                l_POP3State = EPS_USER;
                ReturnValue = ERR_NEXT;
            }
            break;

        case EPS_USER:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_USER");

            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_OK, 3))
            {
                l_POP3State = EPS_PASS;
                ReturnValue = ERR_NEXT;
            }
            break;

        case EPS_PASS:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_PASS");

            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_OK, 3))
            {
                l_POP3State = EPS_STAT;
                ReturnValue = ERR_NEXT;
            }
            break;

        case EPS_STAT:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_STAT");

            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_OK, 3))
            {
                pszToken = STRTOK(POP3_Reply.reply, (BYTE*)" ");
                pszToken = STRTOK(NULL, (BYTE*)" ");

                l_nTotalMailNumber = atoi((char*)pszToken);

                if(l_nTotalMailNumber > MAX_SAVED_MAIL)
                    l_nMailNumber = MAX_SAVED_MAIL;
                else
                    l_nMailNumber = l_nTotalMailNumber;

                if(l_nMailNumber == 0)
                {
                    l_POP3State = EPS_QUIT;
                    ReturnValue = ERR_NOMAIL;
                }
				else if(l_nMailNumber > (MAX_SAVED_MAIL - g_MailNum))
				{
					l_POP3State = EPS_LIST;
					nCurrent = 0;
					ReturnValue = ERR_FULL;
				}
				else
				{
					l_POP3State = EPS_LIST;
					nCurrent = 0;
					ReturnValue = ERR_NEXT;
				}
            }
            break;

        case EPS_LIST:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_LIST");

            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_ENDLIST, 1))
            {
                l_POP3State = EPS_UIDL;
                ReturnValue = ERR_NEXT;
                nCurrent = 0;
                break;
            }
            else
            {
                pszToken = STRTOK(POP3_Reply.reply, (BYTE*)"\n");
           
                if(STRNCMP(pszToken, (BYTE*)POP3_OK, 3) != 0)
                {
                    if(nCurrent < l_nMailNumber && nCurrent < MAX_SAVED_MAIL)
                    {
                        STRCPY(szReply, pszToken);   
                        pTmpPos = STRCHR(szReply, ' ');

                        if(pTmpPos != NULL)
                        {
                            STRNCPY(szTmp, szReply, (UINT8)(pTmpPos - szReply));
                            szTmp[pTmpPos - szReply] = '\0';
                            if(szTmp[0] < '0' && szTmp[0] > '9')
                                break;
                            MailList[nCurrent].nNumber = atoi((char*)szTmp);

                            STRCPY(szTmp, pTmpPos);
                            MailList[nCurrent].nSize = atol((char*)szTmp);   
							if(MailList[nCurrent].nSize > MAX_RECV_MAIL_SIZE)
								MailList[nCurrent].nSize = MAX_RECV_MAIL_SIZE;

                            nCurrent ++;
                        }
                    }
                }

                while((pszToken = STRTOK(NULL, (BYTE*)"\n")) != NULL)
                {
                    if(!STRNCMP(pszToken, (BYTE*)POP3_ENDLIST, 1))
                    {
                        l_POP3State = EPS_UIDL;
                        ReturnValue = ERR_NEXT;
                        nCurrent = 0;
                        break;
                    }

                    if(nCurrent < l_nMailNumber && nCurrent < MAX_SAVED_MAIL)
                    {
                        STRCPY(szReply, pszToken);   
                
                        pTmpPos = STRCHR(szReply, ' ');

                        if(pTmpPos != NULL)
                        {
                            STRNCPY(szTmp, szReply, (UINT8)(pTmpPos - szReply));
                            szTmp[pTmpPos - szReply] = '\0';
                            if(szTmp[0] < '0' && szTmp[0] > '9')
                                break;
                            MailList[nCurrent].nNumber = atoi((char*)szTmp);

                            STRCPY(szTmp, pTmpPos);
                            if(szTmp[0] < '0' && szTmp[0] > '9')
                                break;
                            MailList[nCurrent].nSize = atol((char*)szTmp);   
							if(MailList[nCurrent].nSize > MAX_RECV_MAIL_SIZE)
								MailList[nCurrent].nSize = MAX_RECV_MAIL_SIZE;

                            nCurrent ++;
                        }
                    }
                }
            }

//            if(l_POP3State == EPS_RETR && ReturnValue == ERR_NEXT)
//                SKY_DeleteInBoxMail();

            break;

        case EPS_UIDL:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_UIDL");
            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_ENDLIST, 1))
            {
                l_nCurrentMail = sGetRecvMailNum(TRUE, fNew);

#ifdef GET_NEW_MAIL
                if(l_nCurrentMail <= 0)
#else
                if(l_nCurrentMail > l_nMailNumber)
#endif
                {
                    l_POP3State = EPS_QUIT;
                    ReturnValue = ERR_NONEWMAIL;
                }
                else
                {
                    l_POP3State = EPS_RETR;
                    ReturnValue = ERR_NEXT;
                }
                break;
            }
            else
            {
                pszToken = STRTOK(POP3_Reply.reply, (BYTE*)"\n");
           
                if(pszToken == NULL)    break;

                if(STRNCMP(pszToken, (BYTE*)POP3_OK, 3) != 0)
                {
                    if(nCurrent < l_nMailNumber && nCurrent < MAX_SAVED_MAIL)
                    {
                        STRCPY(szReply, pszToken);   
                        pTmpPos = STRCHR(szReply, ' ');

                        if(pTmpPos != NULL)
                        {
                            STRNCPY(szTmp, szReply, (UINT8)(pTmpPos - szReply));
                            szTmp[pTmpPos - szReply] = '\0';
                            if(szTmp[0] < '0' && szTmp[0] > '9')
                                break;
                            nMailNum = atoi((char*)szTmp);

                            for(i = 0; i < MAX_SAVED_MAIL; i++)
                            {
                                if(MailList[i].nNumber == nMailNum)
                                    break;
                            }

                            if(i == MAX_SAVED_MAIL)
                                break;

                            if(STRLEN(pTmpPos) > MAIL_UIDL)
                            {
                                memcpy(MailList[i].szUIDL, pTmpPos, MAIL_UIDL);
                                MailList[i].szUIDL[MAIL_UIDL-1] = '\0';
                            }
                            else
                                STRCPY(MailList[i].szUIDL, pTmpPos);
                            nCurrent ++;
                        }
                    }
                }

                while((pszToken = STRTOK(NULL, (BYTE*)"\n")) != NULL)
                {
                    if(!STRNCMP(pszToken, (BYTE*)POP3_ENDLIST, 1))
                    {
                        l_nCurrentMail = sGetRecvMailNum(TRUE, fNew);

#ifdef GET_NEW_MAIL
                        if(l_nCurrentMail <= 0)
#else
                        if(l_nCurrentMail > l_nMailNumber)
#endif
                        {
                            l_POP3State = EPS_QUIT;
                            ReturnValue = ERR_NONEWMAIL;
                        }
                        else
                        {
                            l_POP3State = EPS_RETR;
                            ReturnValue = ERR_NEXT;
                        }
                        break;
                    }

                    if(nCurrent < l_nMailNumber && nCurrent < MAX_SAVED_MAIL)
                    {
                        STRCPY(szReply, pszToken);   
                
                        pTmpPos = STRCHR(szReply, ' ');

                        if(pTmpPos != NULL)
                        {
                            UINT16 nMailNum, i;

                            STRNCPY(szTmp, szReply, (UINT8)(pTmpPos - szReply));
                            szTmp[pTmpPos - szReply] = '\0';
                            if(szTmp[0] < '0' && szTmp[0] > '9')
                                break;
                            nMailNum = atoi((char*)szTmp);

                            for(i = 0; i < MAX_SAVED_MAIL; i++)
                            {
                                if(MailList[i].nNumber == nMailNum)
                                    break;
                            }

                            if(i == MAX_SAVED_MAIL)
                                break;

                            if(STRLEN(pTmpPos) > MAIL_UIDL)
                            {
                                memcpy(MailList[i].szUIDL, pTmpPos, MAIL_UIDL);
                                MailList[i].szUIDL[MAIL_UIDL-1] = '\0';
                            }
                            else
                                STRCPY(MailList[i].szUIDL, pTmpPos);
                            nCurrent ++;
                        }
                    }
                }
            }
            break;

        case EPS_RETR:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_RETR");

            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_OK, 3))
            {
                l_POP3State = EPS_DATA;
                ReturnValue = ERR_NEXT;

                g_RecvLen = 0;
                g_RecvBuffer[0] = '\0';

                pTmpPos = STRSTR(POP3_Reply.reply, (BYTE*)"\r\n");
                if(pTmpPos)
                {
                    g_RecvLen = POP3_Reply.nPos - (pTmpPos-POP3_Reply.reply + 2);
                    memcpy(g_RecvBuffer, (BYTE*)(pTmpPos+2), g_RecvLen);
                }

                if(STRLEN(SavedReply))
                {
                    memcpy(&g_RecvBuffer[g_RecvLen], SavedReply, STRLEN(SavedReply));
                    g_RecvLen += STRLEN(SavedReply);
                }
            }
            break;

        case EPS_DATA:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_DATA");
            pTmpPos = NULL;

            pTmpPos = STRSTR(POP3_Reply.reply, (BYTE*)"\r\n.");
            if(pTmpPos == NULL)
            {
                pTmpPos = STRSTR(POP3_Reply.reply, (BYTE*)"\n.");
                if(pTmpPos && szSave[2] != '\r')
                    pTmpPos = NULL;
            }

            if(pTmpPos == NULL)
            {
                pTmpPos = STRSTR(POP3_Reply.reply, (BYTE*)".\r");
                if(pTmpPos && (szSave[2] != '\n' || szSave[1] != '\r'))
                    pTmpPos = NULL;
            }
                
            if(pTmpPos != NULL)
            {
                if(g_RecvLen + (pTmpPos - POP3_Reply.reply) < MAX_RECV_MAIL_SIZE)
                {
                    memcpy(&g_RecvBuffer[g_RecvLen], POP3_Reply.reply, 
                                (UINT16)(pTmpPos - POP3_Reply.reply));
                    g_RecvLen += (UINT16)(pTmpPos - POP3_Reply.reply);
                }

#ifdef GET_NEW_MAIL
                if(l_nCurrentMail <= 1)
#else
                if(l_nCurrentMail == l_nMailNumber)
#endif
                {
                    sSaveCurrentMail();

                    l_POP3State = EPS_QUIT;
                    ReturnValue = ERR_NEXT;
                }
                else
                {
                    if(!sSaveCurrentMail())
                    {
                        l_POP3State = EPS_QUIT;
                        ReturnValue = ERR_NEXT;
                        break;
                    }

                    l_nCurrentMail = sGetRecvMailNum(FALSE, fNew);

#ifdef GET_NEW_MAIL
                    if(l_nCurrentMail <= 0)
#else
                    if(l_nCurrentMail > l_nMailNumber)
#endif
                    {
                        l_POP3State = EPS_QUIT;
                        ReturnValue = ERR_NEXT;
                    }
                    else
                    {
                        g_RecvLen = 0;
                        g_RecvBuffer[0] = '\0';

                        l_POP3State = EPS_RETR;
                        ReturnValue = ERR_NEXT;
                    }
                }
            }
            else
            {
                if((g_RecvLen+POP3_Reply.nPos) < MAX_RECV_MAIL_SIZE)
                {
                    memcpy((void*)&g_RecvBuffer[g_RecvLen], (void*)POP3_Reply.reply, (UINT16)POP3_Reply.nPos);
                    g_RecvLen += POP3_Reply.nPos;
                }
            }

/*
			sprintf(szTmp, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_RECVRATE),
					(UINT8)((g_RecvLen / MailList[l_nCurrentMail].nSize) % 100));

			EQC_SetStaticText(hControl, szTmp);
*/

            memcpy(szSave, (POP3_Reply.reply + (POP3_Reply.nPos - 3)), 3);
            break;

        case EPS_QUIT:
            SKY_TRACE(TL_HIGH, "ReceivePOP3:EPS_QUIT");

            if(!STRNCMP(POP3_Reply.reply, (BYTE*)POP3_OK, 3))
            {
                l_POP3State = EPS_READY;
                if(l_nMailNumber == 0)
                    ReturnValue = ERR_NOMAIL;
                else
                    ReturnValue = ERR_RECVOK;
            }
            break;
        }
    }

    return ReturnValue;
}


LOCAL BOOL SKY_API sSaveCurrentMail(void)
{
    BYTE    szBuffer[10];
    HFILE   hFile;
	UINT8	nSaveMailNum, i;
    DWORD   dwTime;
    clk_julian_type	tm;

    for(i = 0; i < MAX_SAVED_MAIL; i++)
    {
        if(g_MailList[i].nMailNumber == 0)
            break;
    }

    if(i == MAX_SAVED_MAIL)
        return FALSE;

    nSaveMailNum = i+1;

    sprintf((char*)szBuffer, "Recv%2d", nSaveMailNum);

    if(!SKY_IsDataFile(SDF_MAIL,szBuffer))
    {
        if(SKY_CreateDataFile(SDF_MAIL, szBuffer, &hFile) != SDR_OK)
            return FALSE;
    }
    else
        SKY_OpenDataFile(SDF_MAIL, szBuffer, &hFile, SFT_WRITE);

    g_RecvBuffer[g_RecvLen] = '\0';
	memset((void*)&g_RecvMail, 0x00, sizeof(g_RecvMail));
    SKY_ParseEmail(g_RecvLen);

    // Add Recevied Time
    if ((dwTime = SKY_GetLocalTime()) > ONE_YEAR)
	{
        clk_secs_to_julian (dwTime, &tm);
        sprintf((char*)g_RecvMail.szDate, (char*)"%02d월%02d일 %02d:%02d", 
                              tm.month, tm.day, tm.hour, tm.minute);
    }
    else
    {
        sprintf((char*)g_RecvMail.szDate, (char*)"%02d월%02d일 %2d:%2d", 
                              1, 1, 0, 0);
    }
    SKY_WriteDataBlock(hFile, (CONST void *)&g_RecvMail, sizeof(g_RecvMail));
    SKY_CloseDataFile(hFile);

    memset((void*)&g_MailList[nSaveMailNum -1], NULL, sizeof(EMAIL_LIST_T));

    g_MailList[nSaveMailNum -1].nMailNumber = (UINT8)nSaveMailNum;

    // Sender
    if(STRLEN(g_RecvMail.szSender) > MAIL_SENDER)
    {
        STRNCPY(g_MailList[nSaveMailNum -1].szSender,  g_RecvMail.szSender, MAIL_SENDER);
        g_MailList[nSaveMailNum -1].szSender[MAIL_SENDER-1] = '\0';
    }
    else
    {
        STRCPY(g_MailList[nSaveMailNum -1].szSender,  g_RecvMail.szSender);
    }

    // Title
    if(STRLEN(g_RecvMail.szTitle) > MAIL_TITLE)
    {
        STRNCPY(g_MailList[nSaveMailNum -1].szTitle,  g_RecvMail.szTitle, MAIL_TITLE);
        g_MailList[nSaveMailNum -1].szTitle[MAIL_TITLE-1] = '\0';
    }
    else
    {
        STRCPY(g_MailList[nSaveMailNum -1].szTitle, g_RecvMail.szTitle);
    }

    // UIDL
    if(STRLEN(MailList[l_nCurrentMail-1].szUIDL) > MAIL_UIDL)
    {
        STRNCPY(g_MailList[nSaveMailNum -1].szUIDL, MailList[l_nCurrentMail-1].szUIDL, MAIL_UIDL);
        g_MailList[nSaveMailNum -1].szUIDL[MAIL_UIDL-1] = '\0';
    }
    else
    {
        STRCPY(g_MailList[nSaveMailNum -1].szUIDL, MailList[l_nCurrentMail-1].szUIDL);
    }

    for(i = 0; i < MAX_SAVED_MAIL; i++)
    {
        if(g_MailListNum[i] <= 0 || g_MailListNum[i] > MAX_INBOX_MAIL) //(g_MailListNum[i] == 0)
        {
            g_MailListNum[i] = nSaveMailNum;
            break;
        }
    }

    if(SKY_OpenDataFile(SDF_MAIL,(BYTE*) INBOX_LIST_FILE, &hFile, SFT_WRITE) != SDR_OK)
        SKY_CreateDataFile(SDF_MAIL, (BYTE*)INBOX_LIST_FILE, &hFile);

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailListNum, 
            (DWORD)(sizeof(UINT8)*MAX_SAVED_MAIL));

    SKY_WriteDataBlock(hFile, (CONST void *)g_MailList, 
            (DWORD)(sizeof(EMAIL_LIST_T)*MAX_SAVED_MAIL));

    SKY_CloseDataFile(hFile);

    return TRUE;
}

LOCAL BYTE* SKY_API sFindString(BYTE *szSrc, BYTE *szString, UINT16 nSrcLen)
{
    UINT8   nLen, nMatch;
    UINT16  i, j;
	BYTE	bOther;

    if(szSrc == NULL || szString == NULL)
        return NULL;

    nLen = (UINT8)STRLEN(szString);

    nMatch = 0;
    for(i = 0; i < nSrcLen; i++)
    {
		if('A' <= szString[nMatch] && szString[nMatch] <= 'Z')
			bOther = szString[nMatch] - 'A' + 'a';
		else if('a' <= szString[nMatch] && szString[nMatch] <= 'z')
			bOther = szString[nMatch] - 'a' + 'A';
		else
			bOther = NULL;

        if(szSrc[i] == szString[nMatch] || (bOther != NULL && szSrc[i] == bOther))
        {
            for(j = 1; j < nLen; j++)
            {
				if('A' <= szString[j] && szString[j] <= 'Z')
					bOther = szString[j] - 'A' + 'a';
				else if('a' <= szString[j] && szString[j] <= 'z')
					bOther = szString[j] - 'a' + 'A';
				else
					bOther = NULL;

                if(bOther == NULL && szSrc[i+j] != szString[j])
                    break;

                if(szSrc[i+j] != szString[j] && (bOther != NULL && szSrc[i+j] != bOther))
                    break;
            }

            if(j == nLen)
                return (BYTE*)(szSrc + i);
            else
                nMatch = 0;
        }
    }

    return NULL;
}

void SKY_API  SKY_ParseEmail(UINT16 nLen)
{
    BYTE    *pszPos, *pszPos1, *pszPos2, *pszPos3;
	UINT16	i;
    DISCRETE_TYPE_E  DiscreteType;
    COMPOSITE_TYPE_E  CompositeType;
    CONTENT_CODE_E  EncodeType;
	BYTE	szBoundary[82];
	clk_julian_type	tm;
	DWORD   dwTime;
    UINT16 nTmpLen;

    
    g_RecvMail.nMailNumber = (UINT8)l_nCurrentMail;

    for(i = 0; i < nLen-1; i++)
    {
        if(g_RecvBuffer[i] == '\0')
            g_RecvBuffer[i] = ' ';
    }

    // From
    pszPos = sFindString(g_RecvBuffer, (BYTE*)"From:", nLen);

    if(pszPos != NULL)
    {
	    pszPos2 = STRCHR((BYTE*)(pszPos + 1), '\r');
        pszPos1 = STRCHR(pszPos, '<');

		if(pszPos1 == NULL || pszPos1 > pszPos2)
			pszPos1 = (BYTE*)(pszPos + 5);
		else
			pszPos2 = STRCHR((BYTE*)(pszPos1 + 1), '>');

        if(pszPos1 != NULL && pszPos2 != NULL)
        {
            nTmpLen = pszPos2 - pszPos1 - 1;
            if(nTmpLen > MAIL_SENDER-1)
                nTmpLen = MAIL_SENDER-1;

		    memcpy(g_RecvMail.szSender, (void*)(pszPos1 + 1), nTmpLen);

		    g_RecvMail.szSender[nTmpLen] = '\0';
        }
        else
        {
            g_RecvMail.szSender[0] = '\0';
        }
    }

    // Title
    pszPos = sFindString(g_RecvBuffer, (BYTE*)"Subject:", nLen);
    if(pszPos != NULL)
    {
		memset(g_RecvMail.szTitle, 0x00, MAIL_TITLE);

        pszPos2 = STRCHR((BYTE*)(pszPos + 1), '\r');
        if(pszPos2 == NULL)
        {
            STRCPY(g_RecvMail.szTitle, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NO_SUBJECT));
        }
		else if((pszPos1 = sFindString(pszPos, (BYTE*)"=?", (pszPos2-pszPos))) != NULL)
		{
			SKY_DecodeTitle(g_RecvMail.szTitle, pszPos1, MAIL_TITLE);
		}
		else
		{
			pszPos1 = STRCHR(pszPos, ' ');
			pszPos2 = STRCHR((BYTE*)(pszPos1 + 1), '\r');

			if(pszPos2)
			{
                nTmpLen = pszPos2 - pszPos1 - 1;
                if(nTmpLen > MAIL_TITLE-1)
                    nTmpLen = MAIL_TITLE-1;

				memcpy(g_RecvMail.szTitle, (BYTE*)(pszPos1 + 1), nTmpLen);

				g_RecvMail.szTitle[nTmpLen] = '\0';
			}
			else
			{
                nTmpLen = STRLEN(pszPos1+1);

                if(nTmpLen > MAIL_TITLE-1)
                    nTmpLen = MAIL_TITLE-1;

				STRNCPY(g_RecvMail.szTitle, pszPos1+1, nTmpLen);
                g_RecvMail.szTitle[nTmpLen] = '\0';
			}
		}
    }
	else
		STRCPY(g_RecvMail.szTitle, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NO_SUBJECT));

    // Mail Type
	DiscreteType = sFindDiscreteType(g_RecvBuffer, nLen);
	CompositeType = sFindCompositeType(g_RecvBuffer, nLen);
	EncodeType = sFindEncodingType(g_RecvBuffer, nLen);

	if((DiscreteType != TYPE_TEXT && DiscreteType != TYPE_MULTI) ||
		EncodeType == ENCODING_UNKNOWN)
	{
        STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
        return;
    }

    // Text Parsing
    pszPos = sFindString(g_RecvBuffer, (BYTE*)"\r\n\r\n", nLen);
    if(pszPos != NULL)
    {
        pszPos += 4;

        if(DiscreteType == TYPE_TEXT)
        {
			switch(CompositeType){
			case TYPE_PLAIN:
				sDecodeBody_TextPlain(pszPos, g_RecvMail.szText, EncodeType, 
                            (UINT16)STRLEN(pszPos), MAIL_TEXT);
				break;
			case TYPE_HTML:
				sDecodeBody_TextHtml(pszPos, g_RecvMail.szText, EncodeType, 
                            (UINT16)STRLEN(pszPos), MAIL_TEXT);
				break;
			default:
				STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
				break;
			}
        }
		else if(DiscreteType == TYPE_MULTI)
		{
			l_nDepth = 0;
			l_nBodyLen = 0;

			// Find boundary
			pszPos1 = STRSTR(g_RecvBuffer, (BYTE*)"boundary");
			if(pszPos1 == NULL)
			{
				STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
				return;
			}

            pszPos2 = sFindString(pszPos1, (BYTE*)"\r\n", STRLEN(pszPos1));
			if(pszPos2 == NULL)
			{
				STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
				return;
			}

			pszPos1 = sFindString(pszPos1, (BYTE*)"=", pszPos2 - pszPos1);
			if(pszPos1 == NULL)
			{
				STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
				return;
			}

			pszPos3 = sFindString((BYTE*)(pszPos1+1), (BYTE*)"\"", pszPos2 - pszPos1);
			if(pszPos3 != NULL)
			{
                pszPos1 = pszPos3;

                pszPos2 = sFindString((BYTE*)(pszPos1+1), (BYTE*)"\"", pszPos2 - pszPos3);
                if(pszPos2 == NULL)
                {
				    STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
				    return;
                }
			}

			memset(szBoundary, '-', 2);
			memcpy((BYTE*)(szBoundary+2), (BYTE*)(pszPos1+1), (UINT16)(pszPos2 - pszPos1 - 1));
			szBoundary[pszPos2 - pszPos1+1] = '\0';

			sDecode_MultiBody(pszPos2, CompositeType, EncodeType, szBoundary,
						(UINT16)(nLen - (pszPos - g_RecvBuffer)));

		}
		else
		{
			STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
		}

		sRemoveCRLF();
    }
    else
    {
        STRCPY(g_RecvMail.szText, (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_RECV_NOT_SUPPORT_BODY));
    }
}


LOCAL void SKY_API sRemoveCRLF(void)
{
	BYTE *pszPos1;
    UINT16  i;

	// Title
    pszPos1 = g_RecvMail.szTitle;

    for(i = 0; i < STRLEN(pszPos1); i++)
    {
        if(pszPos1[i] < 31)
            pszPos1[i] = ' ';
    }
/*
    while((pszPos1 = STRCHR(pszPos1, '\r')) != NULL)
    {
        *pszPos1 = ' ';
        pszPos1++;
    }

    pszPos1 = g_RecvMail.szTitle;
    while((pszPos1 = STRCHR(pszPos1, '\n')) != NULL)
    {
        *pszPos1 = ' ';
        pszPos1++;
    }
*/

	// Body
    pszPos1 = g_RecvMail.szText;

    for(i = 0; i < STRLEN(pszPos1); i++)
    {
        if(pszPos1[i] < 31)
            pszPos1[i] = ' ';
    }
/*
    while((pszPos1 = STRCHR(pszPos1, '\r')) != NULL)
    {
        *pszPos1 = ' ';
        pszPos1++;
    }

    pszPos1 = g_RecvMail.szText;
    while((pszPos1 = STRCHR(pszPos1, '\n')) != NULL)
    {
        *pszPos1 = ' ';
        pszPos1++;
    }
*/
}




LOCAL void SKY_API sDecode_MultiBody(BYTE* pszPos, 
									 COMPOSITE_TYPE_E CompositeType, 
									 CONTENT_CODE_E EncodeType, 
									 BYTE	*pszBoundary,
									 UINT16 nLen)
{
	BYTE	*pszPos1, *pszPos2;	// for Part handling
	BYTE	*pszPos3, *pszPos4;	// for Parsing part
    BYTE    *pszPosCur;

    DISCRETE_TYPE_E  DiscreteType1;
    COMPOSITE_TYPE_E  CompositeType1;
    CONTENT_CODE_E  EncodeType1;
	UINT16  nBodyLen;
    BOOL    fParse;

	l_nDepth++;

    if(pszPos == NULL)
        return;

	// Find boundary
	pszPos1 = STRSTR(pszPos, (BYTE*)"boundary");
	l_szBoundary[l_nDepth][0] = '\0';
	if(pszPos1 != NULL && l_nDepth < MAX_NESTED)
	{
		pszPos1 = STRCHR(pszPos1, '\"');
        if(pszPos1 != NULL)
        {
		    pszPos2 = STRCHR((BYTE*)(pszPos1+1), '\"');

            if(pszPos2 != NULL)
            {
		        memset(l_szBoundary[l_nDepth], '-', 2);
		        memcpy((BYTE*)(&l_szBoundary[l_nDepth][2]), (BYTE*)(pszPos1+1), (UINT16)(pszPos2 - pszPos1 - 2));
		        l_szBoundary[l_nDepth][pszPos2 - pszPos1] = '\0';
            }
        }
	}

    fParse = FALSE;
	pszPosCur = pszPos;

	while(TRUE)
	{
		pszPos1 = STRSTR(pszPosCur, pszBoundary);
		if(pszPos1 == NULL)		break;

		pszPos2 = STRSTR((BYTE*)(pszPos1+1), pszBoundary);
        if(pszPos2 == NULL)
            pszPos2 = pszPos1 + STRLEN(pszPos1);

		// Mail Type
		DiscreteType1 = sFindDiscreteType(pszPos1, nLen);
		CompositeType1 = sFindCompositeType(pszPos1, nLen);
		EncodeType1 = sFindEncodingType(pszPos1, nLen);

		if(EncodeType1 == ENCODING_UNKNOWN)
			EncodeType1 = EncodeType;

		if(DiscreteType1 != TYPE_TEXT && DiscreteType1 != TYPE_MULTI) 
		{
			pszPos1 = pszPos2;
            pszPosCur = pszPos1;
			continue;
		}

		// Text Parsing
        if(DiscreteType1 != TYPE_MULTI)
    	    pszPos3 = sFindString(pszPos1, (BYTE*)"Content-Transfer-Encoding:", (pszPos2 - pszPos1));
        else
            pszPos3 = sFindString(pszPos1, (BYTE*)"Content-Type", (pszPos2 - pszPos1));

        if(pszPos3)
        {
	        pszPos4 = sFindString(pszPos3, (BYTE*)"\r\n", (pszPos2 - pszPos1));
        }
        else
        {
			pszPos1 = pszPos2;
            pszPosCur = pszPos1;
			continue;
        }
    
        if(DiscreteType1 != TYPE_MULTI)
        {
       		pszPos3 = sFindString(pszPos1, (BYTE*)"\r\n\r\n", (pszPos2 - pszPos1));
            if(pszPos3)
                pszPos4 = pszPos3;
        }

		if(pszPos4 != NULL)
		{
            if(DiscreteType1 != TYPE_MULTI)
			    pszPos4 += 4;

			if(DiscreteType1 == TYPE_TEXT)
			{
				switch(CompositeType1){
				case TYPE_PLAIN:
					nBodyLen = sDecodeBody_TextPlain(pszPos4, (BYTE*)(g_RecvMail.szText + l_nBodyLen), 
									EncodeType1, (UINT16)(pszPos2 - pszPos4), (UINT16)(MAIL_TEXT - l_nBodyLen));

					l_nBodyLen += (nBodyLen - 1);
                    fParse = TRUE;
					break;
				case TYPE_HTML:
					nBodyLen = sDecodeBody_TextHtml(pszPos4, (BYTE*)(g_RecvMail.szText + l_nBodyLen), 
									EncodeType1, (UINT16)(pszPos2 - pszPos4), (UINT16)(MAIL_TEXT - l_nBodyLen));
					l_nBodyLen += (nBodyLen - 1);
                    fParse = TRUE;
					break;
				default:
					break;
				}
			}
			else if(DiscreteType1 == TYPE_MULTI)
			{
				if(l_nDepth > MAX_NESTED)
					continue;

				if(STRLEN(l_szBoundary[l_nDepth]) == 0)
					continue;

				sDecode_MultiBody(pszPos4, CompositeType1, EncodeType1, l_szBoundary[l_nDepth],
								(UINT16)(pszPos2 - pszPos4));
                fParse = TRUE;
			}
		}

		if(CompositeType != TYPE_ALTERNATE && fParse)
			pszPos1 = pszPos2;
		else
			break;

        pszPosCur = pszPos1;
	}
}


LOCAL UINT16 SKY_API sDecodeBody_TextPlain(BYTE *pszSrc, BYTE *pszDest,
										 CONTENT_CODE_E EncodeType, UINT16 nLen,
										 UINT16	nMaxLen)
{
	UINT16	nLen1;

    nLen1 = nLen;

    switch(EncodeType){
    case ENCODING_7BIT:
    case ENCODING_8BIT:
    case ENCODING_QUOTED:
		if(nLen1 > nMaxLen)
			nLen1 = nMaxLen;
        memcpy(pszDest, pszSrc, nLen1);
		pszDest[nLen1] = '\0';
		nLen1++;
        break;
    case ENCODING_BASE64:
        nLen1 = SKY_DecodeBase64(pszDest, pszSrc, nLen, nMaxLen);
        break;
    case ENCODING_PRINTABLE:
		nLen1 = SKY_DecodePrintable(pszDest, pszSrc, nLen, nMaxLen);
        break;
    }

	return nLen1;
}


LOCAL UINT16 SKY_API sDecodeBody_TextHtml(BYTE *pszSrc, BYTE *pszDest,
										CONTENT_CODE_E EncodeType, UINT16 nLen,
										UINT16 nMaxLen)
{
	UINT16	nLen1, i, j;
    BYTE    pszTmp[MAIL_TEXT*10];
    BOOL    fChar;
    BYTE    *pszBody;

    nLen1 = nLen;

    switch(EncodeType){
    case ENCODING_7BIT:
    case ENCODING_8BIT:
    case ENCODING_QUOTED:
		if(nLen1 > (MAIL_TEXT*10))
			nLen1 = MAIL_TEXT*10 -1;
        memcpy(pszTmp, pszSrc, nLen1);
		pszDest[nLen1] = '\0';
		nLen1++;
        break;
    case ENCODING_BASE64:
        nLen1 = SKY_DecodeBase64(pszTmp, pszSrc, nLen, MAIL_TEXT*10);
        break;
    case ENCODING_PRINTABLE:
		nLen1 = SKY_DecodePrintable(pszTmp, pszSrc, nLen, MAIL_TEXT*10);
        break;
    }

    fChar = FALSE;

    // Find <body> tag
    pszBody = STRSTR(pszTmp, (BYTE*)"<TITLE>");
    if(pszBody == NULL)
        pszBody = STRSTR(pszTmp, (BYTE*)"<title>");
    if(pszBody == NULL)
        pszBody = STRSTR(pszTmp, (BYTE*)"<Title>");

    if(pszBody == NULL)
    {
        STRCPY(pszDest, (BYTE*)"내용없음");
        return (UINT16)(9);
    }
    pszBody += 7;

	// Add tag parsing.
    for(i = 0, j = 0; i < nLen1; i++)
    {
        if(pszBody[i] == '<')
        {
            while(pszBody[i] != '>')
            {
                i++;
                if(i >= nLen1)  break;
            }
        }
        else if(pszBody[i] == '\r' || pszBody[i] == '\n')
        {
            continue;
        }
        else
        {
            if(pszBody[i] == ' ')
            {
                if(j == 0)  continue;
                if( pszDest[j - 1] == ' ')  continue;
            }
            pszDest[j] = pszBody[i];
            j++;
        }

        if(j >= nMaxLen || i >= nLen1)
            break;
    }

	return nLen1;
}

LOCAL DISCRETE_TYPE_E SKY_API sFindDiscreteType(BYTE *pszStart, UINT16 nLen)
{
	DISCRETE_TYPE_E DiscreteType;
	BYTE	*pszPos1, *pszPos2;
	UINT8 i;

	DiscreteType = TYPE_UNKNOWN;

	pszPos1 = sFindString(pszStart, (BYTE*)"Content-Type:", nLen);

	if(pszPos1 != NULL)
	{
	    pszPos2 = sFindString(pszPos1, (BYTE*)"\r\n", nLen);
        if(pszPos2 == NULL)
            return DiscreteType;

		for(i = 0; i < NUM_DISCRETE; i++)
		{
			if(sFindString(pszPos1, (BYTE*)l_discrete[i].TYPE, (UINT16)(pszPos2 - pszPos1)))
			{
				DiscreteType = l_discrete[i].ID;
				break;
			}
		}
	}
	else
		DiscreteType = TYPE_TEXT;

	return DiscreteType;
}

LOCAL COMPOSITE_TYPE_E SKY_API sFindCompositeType(BYTE *pszStart, UINT16 nLen)
{
	COMPOSITE_TYPE_E CompositeType;
	BYTE	*pszPos1, *pszPos2;
	UINT8	i;

	CompositeType = TYPE_UNKNOWN_COMPOSITE;

	pszPos1 = sFindString(pszStart, (BYTE*)"Content-Type:", nLen);

	if(pszPos1 != NULL)
	{
      	pszPos2 = sFindString(pszPos1, (BYTE*)"\r\n", nLen);
        if(pszPos2 == NULL) 
            return CompositeType;

		for(i = 0; i < NUM_COMPOSITE; i++)
		{
			if(sFindString(pszPos1, (BYTE*)l_composite[i].TYPE, (UINT16)(pszPos2 - pszPos1)))
			{
				CompositeType = l_composite[i].ID;
				break;
			}
		}
	}
	else
		CompositeType = TYPE_PLAIN;

	return CompositeType;
}

LOCAL CONTENT_CODE_E SKY_API sFindEncodingType(BYTE *pszStart, UINT16 nLen)
{
	CONTENT_CODE_E EncodeType;
	BYTE	*pszPos1, *pszPos2;
	UINT8	i;

	EncodeType = ENCODING_UNKNOWN;

	pszPos1 = sFindString(pszStart, (BYTE*)"Content-Transfer-Encoding:", nLen);

	if(pszPos1 != NULL)
	{
        pszPos2 = sFindString(pszPos1, (BYTE*)"\r\n", nLen);
        if(pszPos2 == NULL)
            return EncodeType;

		for(i = 0; i < NUM_CODE; i++)
		{
			if(sFindString((BYTE*)(pszPos1), (BYTE*)l_codes[i].TYPE, (UINT16)(pszPos2 - pszPos1)))
            {
				EncodeType = l_codes[i].ID;
                break;
            }
		}
	}
	else
		EncodeType = ENCODING_7BIT;

	return EncodeType;
}