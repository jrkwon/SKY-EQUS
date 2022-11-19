/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_smtp.c
**
** Description: 
**     SMTP Protocol
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-02-11 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
#include "uisockapi.h"

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
#define SMTP_PORT   25
#define CMD_MAX     256
#define TEXT_TRANSFER_UNIT  80  // mail data contain 128 ASCII char.
#define	NET_MAX_SIZE	256

#define GET_HOST_NAME()	(host_name)

static const char host_name[] = "SK.Phone";
static const char char_set[] = "ks_c_5601-1987";

// Positive Response
#define SMTP_READY          "220"
#define SMTP_CLOSE          "221"
#define SMTP_OK             "250"
#define SMTP_START_MAIL     "354"
#define SMTP_USER_TRY_OTHER "551"
#define SMTP_BAD_CMD_SEQ    "503"

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

ReplyStack_T    SMTP_Reply;

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
LOCAL EMAIL_SMTP_STATE_E   l_SMTPState;
LOCAL UINT16 l_saveLen;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL BYTE  l_mmiRcvBuf[NET_MAX_SIZE];
//LOCAL BYTE  l_bound[63];

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void sSendCommand(BYTE* szCommand);
LOCAL void sGetTime(BYTE* str);
//LOCAL void SKY_API sMakeBoundary(void);


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
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

void SKY_API SKY_SendEMail(EMAIL_SMTP_STATE_E initState, BOOL fReSend,
                           HCONTROL hControl)
{
    BYTE szCommand[CMD_MAX];
    BYTE szTmp[CMD_MAX];
    BYTE *szTextBuffer;
    INT16   nLen, nPos, nSend;

    if(initState != ESS_NONE)
        l_SMTPState = initState;

    if(fReSend)
    {
        if(l_SMTPState == ESS_DATA || l_SMTPState == ESS_TEXT)
            return;
    }

    switch(l_SMTPState)
    {
    case ESS_HELO:
        sprintf((char*)szCommand, "HELO %s\r\n", GET_HOST_NAME());
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"ESS_HELO");
#endif
        break;

    case ESS_MAIL:
        sprintf((char*)szCommand, "MAIL FROM: <%s>\r\n", g_SendMail.szSender);
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"ESS_MAIL");
#endif
        break;

    case ESS_RCPT:
        if(STRCHR(g_SendMail.szSender, '@'))
            sprintf((char*)szCommand, "RCPT TO: <%s>\r\n", g_SendMail.szReceiver);
        else
            sprintf((char*)szCommand, "RCPT TO: <%s@%s>\r\n", g_SendMail.szReceiver,
            (BYTE *)SKY_GET_EMAIL_POP3());
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"ESS_RCPT");
#endif
        break;

    case ESS_DATA:
        STRCPY(szCommand,(BYTE*)"DATA\r\n");
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"ESS_DATA");
#endif
        break;

    case ESS_TEXT:
        // Date
        STRCPY(szCommand, (BYTE*)"Date: ");
	    sGetTime(&szCommand[0] + STRLEN(szCommand));
        STRCAT(szCommand, (BYTE*)"\r\n");
        sSendCommand(szCommand);

        // To
        sprintf((char*)szCommand, "To: <%s>\r\n", g_SendMail.szReceiver);
        sSendCommand(szCommand);

        // From
        sprintf((char*)szCommand, "From: <%s>\r\n", g_SendMail.szSender);
        sSendCommand(szCommand);

        // Subject
        if(STRLEN(g_SendMail.szTitle) == 0)
            SKY_EncodeBase64((BYTE*)"제목없음", szTmp);
        else
            SKY_EncodeBase64(g_SendMail.szTitle, szTmp);

        sprintf((char*)szCommand, "Subject: =?%s?B?%s?= \r\n", 
                char_set,
                szTmp);
        sSendCommand(szCommand);

        // MIME-Version
        STRCPY(szCommand, (BYTE*)"MIME-Version: 1.0\r\n");
        sSendCommand(szCommand);

        // Content-Type
/*
        if(STRLEN(g_SendMail.szLinkFile[0]))       // 사진 첨부
        {
            sMakeBoundary();
        }
        else
*/
        STRCPY(szCommand, (BYTE*)"Content-Type: text/plain;\r\n");
        sSendCommand(szCommand);

        sprintf((char*)szCommand, "\tcharset=\"%s\"\r\n", char_set);
        sSendCommand(szCommand);

        // Content-Transfer-Encoding
        STRCPY(szCommand, (BYTE*)"Content-Transfer-Encoding: base64\r\n\r\n");
        sSendCommand(szCommand);

        // Text
        STRCAT(g_SendMail.szText, (BYTE*)"\r\n");
        STRCAT(g_SendMail.szText, (BYTE*)SKY_GET_EMAIL_USERINFO());

        nLen = STRLEN(g_SendMail.szText);
        szTextBuffer = (BYTE*)SKY_Malloc(nLen * 2);

        memset(szTextBuffer, 0x00, nLen * 2);
        SKY_EncodeBase64(g_SendMail.szText, szTextBuffer);

        nLen = STRLEN(szTextBuffer);
        nPos = 0;
        while(nLen > 0)
        {
/*
            if(nLen > TEXT_TRANSFER_UNIT)
            {
                if(IsMiddleOfMultiByte(&g_SendMail.szText[nPos], TEXT_TRANSFER_UNIT))
                    nSend = TEXT_TRANSFER_UNIT;
                else
                    nSend = TEXT_TRANSFER_UNIT-1;
            }
            else
                nSend = nLen;

            memcpy(szTmp, &g_SendMail.szText[nPos], nSend);
            szTmp[nSend] = '\0';
            STRCAT(szTmp, (BYTE*)"\r\n");

            memset(szCommand, ' ', CMD_MAX);
            SKY_EncodeBase64(szTmp, szCommand);
*/

            if(nLen > TEXT_TRANSFER_UNIT)
                nSend = TEXT_TRANSFER_UNIT;
            else
                nSend = nLen;

            memcpy(szCommand, &szTextBuffer[nPos], nSend);
            szCommand[nSend] = '\0';
            sSendCommand(szCommand);

            nPos += nSend;
            nLen -= nSend;

            if(nLen <= 0)
                break;
        }

        // End
        STRCPY(szCommand, (BYTE*)"\r\n\r\n.\r\n"); 
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"ESS_TEXT");
#endif

        SKY_Free(szTextBuffer);
        break;

    case ESS_QUIT:
        STRCPY(szCommand, (BYTE*)"QUIT\r\n");
        sSendCommand(szCommand);
#ifdef EMAIL_DEBUG
        EQC_SetStaticText(hControl, (BYTE*)"ESS_QUIT");
#endif
        break;
    }
}

EMAIL_SEND_RESULT_E SKY_API SKY_ReceiveSMTPReply(HCONTROL hControl)
{
#ifdef EMAIL_DEBUG
    BYTE    szTmp[4];
#endif

	SKY_RecvReply(&SMTP_Reply, NULL);

	if (!SMTP_Reply.fValid)
		return ESR_CONTINUE;

#ifdef EMAIL_DEBUG
    memcpy(szTmp, SMTP_Reply.reply, 3);
    szTmp[3] = '\0';

    EQC_SetStaticText(hControl, szTmp);
#endif

    switch(l_SMTPState)
    {
    case ESS_HELO:
        if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_READY, 3))
        {
            l_SMTPState = ESS_MAIL;
            return ESR_NEXT;
        }
        break;

    case ESS_MAIL:
        if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_OK, 2))
        {
            l_SMTPState = ESS_RCPT;
            return ESR_NEXT;
        }
        else if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_READY, 3))
            return ESR_CONTINUE;
        break;

    case ESS_RCPT:
        if((!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_OK, 2)) || 
            (!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_USER_TRY_OTHER, 3)))
        {
            l_SMTPState = ESS_DATA;
            return ESR_NEXT;
        }
        break;

    case ESS_DATA:
        if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_START_MAIL, 3))
        {
            l_SMTPState = ESS_TEXT;
            return ESR_NEXT;
        }
        else if((!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_OK, 2)) || 
                (!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_USER_TRY_OTHER, 3)))
            return ESR_CONTINUE;
        break;

    case ESS_TEXT:
        if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_OK, 2))
        {
            l_SMTPState = ESS_QUIT;
            return ESR_NEXT;
        }
        else if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_START_MAIL, 3))
            return ESR_CONTINUE;
        break;

    case ESS_QUIT:
        if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_CLOSE, 3))
        {
            l_SMTPState = ESS_HELO;
            return ESR_SENDOK;
        }
        else if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_OK, 2))
            return ESR_CONTINUE;
        break;
    }

    if(!STRNCMP(SMTP_Reply.reply, (BYTE*)SMTP_BAD_CMD_SEQ, 3))
        return ESR_NEXT;
    else if(l_SMTPState == ESS_RCPT)
        return ESR_RCPTERR;
    else
        return ESR_FAIL;
}

void SKY_ConnectSMTPServer(void)
{
    SKY_SET_SOCK_MODE(TRUE);

	if (SKY_IS_SOCK_CONNECT())
		SvrLinkReconnect((char*)SKY_GET_EMAIL_SMTP(), SMTP_PORT);
	else
		SvrLinkConnect((char*)SKY_GET_EMAIL_SMTP(), SMTP_PORT);

	SMTP_Reply.reply[0] = 0x00;
    SMTP_Reply.nPos = 0;
    SMTP_Reply.fValid = FALSE;

    SKY_InitRecvBuffer();
}

void SKY_DisconnectServer()
{
    SKY_SET_SOCK_CONNECT(FALSE);

    if(SKY_IS_SOCK_MODE())
    {
        MSG_HIGH("SKY_DisconnectServer",0,0,0);
        SvrLinkReleasebyUi();
    }
    SKY_SET_SOCK_MODE(FALSE);
}

void sSendCommand(BYTE* szCommand)
{
	UINT16 nLen;
    
    nLen = STRLEN(szCommand);

	SMTP_Reply.reply[0] = 0x00;
    SMTP_Reply.nPos = 0;
    SMTP_Reply.fValid = FALSE;
	
	SvrLinkSend(szCommand, nLen);
}

void SKY_InitRecvBuffer(void)
{
    l_saveLen = 0;
}

INT SKY_RecvReply(ReplyStack_T *Reply, BYTE *szSave)
{
    INT16 numread;

    static BYTE szSaveBuffer[NET_MAX_SIZE];

    memset(Reply->reply, 0x00, MAX_REPLY_LENGTH);

    if(l_saveLen)
    {
        memcpy(Reply->reply, szSaveBuffer, l_saveLen);
        Reply->nPos = l_saveLen;
        memset(szSaveBuffer, 0x00, NET_MAX_SIZE);
    }
    else
        Reply->nPos = 0;

    l_saveLen = 0;
    
	while((numread = SvrLinkRcv(l_mmiRcvBuf)) > 0)
    {
        BYTE *nextPos;

        if(numread == 0)    break;
        l_mmiRcvBuf[numread] = '\0';

        if((nextPos = STRCHR(l_mmiRcvBuf, '\r')) != NULL)
        {
            if((nextPos = STRCHR(l_mmiRcvBuf, '\n')) != NULL)
            {
                l_saveLen = numread - (nextPos - l_mmiRcvBuf +1);
                if(l_saveLen > 0)
                {
                    memcpy(szSaveBuffer, nextPos+1, l_saveLen);
                    szSaveBuffer[l_saveLen] = '\0';
                }
                else
                    l_saveLen = 0;
            }
        }

        if(numread > MAX_REPLY_LENGTH - Reply->nPos)
            numread = MAX_REPLY_LENGTH - Reply->nPos;

        memcpy(Reply->reply + Reply->nPos, l_mmiRcvBuf, (UINT16)(numread - l_saveLen));
	    Reply->nPos += (UINT16)(numread - l_saveLen);

        if(l_saveLen)
            break;
    }

    Reply->fValid = TRUE;
	Reply->reply[Reply->nPos+1] = 0x00;

    if(szSave != NULL && l_saveLen > 0)
    {
        UINT16  nPos;
        nPos = l_saveLen;
        memcpy(szSave, szSaveBuffer, l_saveLen);

	    while((numread = SvrLinkRcv(l_mmiRcvBuf)) > 0)
        {
            memcpy((szSave+nPos), l_mmiRcvBuf, numread);
            nPos += numread;
        }

        szSave[nPos] = '\0';
        l_saveLen = 0;
    }
    else if(szSave != NULL)
    {
        szSave[0] = '\0';
        l_saveLen = 0;
    }

	return Reply->nPos;
}

INT SKY_RecvData(ReplyStack_T *Reply)
{
    INT16 numread;

    memset(Reply->reply, 0x00, MAX_REPLY_LENGTH);
    Reply->nPos = 0;

	while((numread = SvrLinkRcv(l_mmiRcvBuf)) > 0)
    {
        //BYTE *nextPos;

        if(numread > MAX_REPLY_LENGTH - Reply->nPos)
            numread = MAX_REPLY_LENGTH - Reply->nPos;

        memcpy(Reply->reply + Reply->nPos, l_mmiRcvBuf, numread);
	    Reply->nPos += numread;
    }

    Reply->fValid = TRUE;
	Reply->reply[Reply->nPos+1] = 0x00;

	return Reply->nPos;
}

void sGetTime(BYTE* str)
{
    const BYTE days[][4] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    const BYTE months[][4] = {"Jan" , "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	clk_julian_type	tm;
	dword		Time;

    if ((Time = SKY_GetLocalTime()) > ONE_YEAR)
	{
		clk_secs_to_julian (Time, &tm);
        
        sprintf((char*)str, "%s, %d %s %d %d:%d:%d +0900"
		    , days[tm.day_of_week], tm.day, months[tm.month - 1]
		    , tm.year, tm.hour, tm.minute, tm.second);
	}
	else
        sprintf((char*)str, "Sun, 00 Jan 2000 00:00:00 +0900");
}



/*
LOCAL void SKY_API sMakeBoundary(void)
{
    INT i, nPos;

    STRCPY(l_bound, (BYTE*)"=_");
    nPos = 2;

	srand(SKY_GetLocalTime());	// RANDOM NUMBER SEED....
	for(i = 0; i < 60; i++)
    {
		l_bound[nPos] = BASE64KEY[rand() % 64] ;
        nPos ++;
    }

    l_bound[nPos] = '\0';
}
*/