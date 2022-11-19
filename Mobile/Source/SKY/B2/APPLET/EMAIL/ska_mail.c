/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_mail.c
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
** 2001-02-08 woodstok     Created.
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

#define EMAIL_TEXT_WIDTH    16//18
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
enum tag_EMAIL_ACCOUNT_E {
//    EAC_NAME_TITLE,
//    EAC_NAME,
    EAC_ID_TITLE,
    EAC_ID,
    EAC_PASSWD_TITLE,
    EAC_PASSWD,
    EAC_SMTP_TITLE,
    EAC_SMTP,
    EAC_POP3_TITLE,
    EAC_POP3,
    EAC_SIG_TITLE,
    EAC_SIG,
    EAC_MAX
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
const BYTE BASE64KEY[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

LOCAL void SKY_API sAddEmailAccountControls(void);
LOCAL BOOL ch_isupper(BYTE ch);
LOCAL BOOL ch_isdigit(BYTE ch);





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainEMailAccount()
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
//#define WAP_TEST
void SKY_API SKA_EnterEmailMenu(SKY_EVENT_T *pEvent)
{
#ifdef WAP_TEST
    static BYTE *paEmailMenu[6];
#else
    static BYTE *paEmailMenu[4];
#endif//WAP_TEST
    static HCONTROL s_hHnd;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_EMAIL_TITLE), 
                      WA_SHOW_HEADER | WA_SHOW_CML);

        paEmailMenu[0] = (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_IN_BOX);
        paEmailMenu[1] = (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_SENT_BOX);
        paEmailMenu[2] = (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_EDIT);
        paEmailMenu[3] = (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_SET);
#ifdef WAP_TEST
        paEmailMenu[4] = (BYTE*)SKY_GET_TOKEN(TKN_NTOP_FLAG_TEST_1);
        paEmailMenu[5] = (BYTE*)SKY_GET_TOKEN(TKN_NTOP_FLAG_TEST_2);
#endif //WAPE_TEST

#ifdef WAP_TEST
        s_hHnd = EQC_ScrollList(EQS_NA, EQS_NA, 18, 6, paEmailMenu, 6, 6, 0); 
#else
        s_hHnd = EQC_ScrollList(EQS_NA, EQS_NA, 18, 4, paEmailMenu, 4, 4, 0); 
#endif //WAP_TEST
        EQC_SET_STYLE(s_hHnd, CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE);
        EQS_SetIOCActive( s_hHnd );
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_SetIOCActive( s_hHnd );
        break;
        
    case SE_IO_SCROLL_LIST_SELECT:
        switch(pEvent->dwParam){
        case 0:
            EQS_START_CHILD( SKA_MainMailInBox );
            break;
        case 1:
            EQS_START_CHILD( SKA_MainMailOutBox );
            break;
        case 2:
            EQS_START_CHILD( SKA_MainMailSend );
            break;
        case 3:
            EQS_START_CHILD( SKA_MainEMailAccount );
            break;
#ifdef WAP_TEST
        case 4:
            EQS_START_CHILD( SKA_WAPFLAG_TEST1 );
            break;
        case 5:
            EQS_START_CHILD( SKA_WAPFLAG_TEST2 );
            break;
#endif //WAP_TEST
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*
** 공백만으로된 문자열이면 TRUE
*/
LOCAL BOOL SKY_API sIsSpaceOnlyString(BYTE *pszStr)
{
    DWORD i;

    for(i = 0; pszStr[i] != '\0'; i++)
    {
        if(pszStr[i] != ' ')
            return FALSE;
    }
    return TRUE;
}


void SKY_API SKA_MainEMailAccount(SKY_EVENT_T *pEvent)
{
    BYTE   szTmpStr[MAIL_USERID+MAIL_SMTP+2]; /* strlen of USERID+SMTP+'@'+NULL */
    static BOOL fEndMessage, fSaveMessage, fIdle;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_SET), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        sAddEmailAccountControls();

        fEndMessage=fSaveMessage = fIdle = FALSE;
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_SOFTKEY_CANCEL:
        EQS_End();
        break;

    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

    case SE_IO_MESGBOX_YES:
        if(!fSaveMessage)
            break;
        // Go through...
    case SE_SOFTKEY_SAVE:
        // Save
//        EQC_GetTextEditData(EQS_GetNthIOC(EAC_NAME), szTmpStr);
//        SKY_SET_EMAIL_USERNAME(szTmpStr);

        EQC_GetTextEditData(EQS_GetNthIOC(EAC_ID), szTmpStr);
        if(STRCHR(szTmpStr, '@'))
        {
             EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_ID_ERR), 
                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
             break;
        }

        if(szTmpStr[0] == '\0' || sIsSpaceOnlyString(szTmpStr))
        {
             EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_EMAIL_ACCOUNT_NAME_ERR), 
                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
             break;
        }

        SKY_SET_EMAIL_ACCOUNT(TRUE);

        SKY_SET_EMAIL_USERID(szTmpStr);

        EQC_GetTextEditData(EQS_GetNthIOC(EAC_PASSWD), szTmpStr);
        SKY_SET_EMAIL_PASSWORD(szTmpStr);

        EQC_GetTextEditData(EQS_GetNthIOC(EAC_SMTP), szTmpStr);
        SKY_SET_EMAIL_SMTP(szTmpStr);

        EQC_GetTextEditData(EQS_GetNthIOC(EAC_POP3), szTmpStr);
        SKY_SET_EMAIL_POP3(szTmpStr);

        EQC_GetTextEditData(EQS_GetNthIOC(EAC_SIG), szTmpStr);
        SKY_SET_EMAIL_USERINFO(szTmpStr);

        sprintf((char*)szTmpStr, "%s@%s",
                SKY_GET_EMAIL_USERID(),
                SKY_GET_EMAIL_SMTP());

        if(STRLEN(szTmpStr) > MAX_EMAIL_ADDR-1)
            szTmpStr[MAX_EMAIL_ADDR-1] = NULL;     // Cut 

        SKY_SET_EMAIL_ADDRESS(szTmpStr);

        EQC_GetTextEditData(EQS_GetNthIOC(EAC_POP3), szTmpStr);
        SKY_SET_EMAIL_SERVER(szTmpStr);

         EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

         fEndMessage = TRUE;
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


LOCAL void SKY_API sAddEmailAccountControls(void)
{
    HCONTROL hControl;

    // 사용자 이름
/*
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_ACCOUNT_NAME);
    if(SKY_IS_EMAIL_ACCOUNT())
        hControl = EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_KOREAN, 
                     MAIL_USERNAME, (BYTE *)SKY_GET_EMAIL_USERNAME(), 0);
    else
        hControl = EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_KOREAN, 
                     MAIL_USERNAME, (BYTE *)"", 0);
*/
    //EQC_SetFormatString(hControl, (BYTE*)"aa\\-AA");

    // 사용자 ID
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_ACCOUNT_ID);
    if(SKY_IS_EMAIL_ACCOUNT())
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_USERID-1, (BYTE *)SKY_GET_EMAIL_USERID(), 0);
    else
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_USERID-1, (BYTE *)"", 0);

    // 비밀번호
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_ACCOUNT_PASSWD);
    if(SKY_IS_EMAIL_ACCOUNT())
        EQC_SET_STYLE(EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_PASSWORD-1, (BYTE *)SKY_GET_EMAIL_PASSWORD(), 0),
                     CS_TEXT_EDIT_DEFAULT|CS_TEXT_EDIT_PASSWORD);
    else
        EQC_SET_STYLE(EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_PASSWORD-1, (BYTE *)"", 0),
                     CS_TEXT_EDIT_DEFAULT|CS_TEXT_EDIT_PASSWORD);

    // 보낼 메일서버
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_ACCOUNT_SMTP);
//    if(SKY_IS_EMAIL_ACCOUNT())
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_SMTP-1, (BYTE *)SKY_GET_EMAIL_SMTP(), 0);
//    else
//        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
//                     MAIL_SMTP, (BYTE *)"", 0);

    // 받는 메일서버
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_ACCOUNT_POP3);
    if(SKY_IS_EMAIL_ACCOUNT())
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_POP3-1, (BYTE *)SKY_GET_EMAIL_POP3(), 0);
    else
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 1, TEM_ENGLISH_LOWER, 
                     MAIL_POP3-1, (BYTE *)"", 0);

    // 편지 맺음말
    EQC_StaticText(EQS_NA, EQS_NA, TKN_EMAIL_ACCOUNT_SIG);
    if(SKY_IS_EMAIL_ACCOUNT())
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 2, TEM_KOREAN, 
                     MAIL_USERINFO-1, (BYTE *)SKY_GET_EMAIL_USERINFO(), 0);
    else
        EQC_TextEdit(EQS_NA, EQS_NA, EMAIL_TEXT_WIDTH, 2, TEM_KOREAN, 
                     MAIL_USERINFO-1, (BYTE *)"", 0);

    EQS_AutoFormatting(EAC_MAX, EHA_LEFT, EVA_CENTER);
}


////////////////////////////////////////////////////////////////////
//// Encoding/Decoding Function
////////////////////////////////////////////////////////////////////

void SKY_EncodeBase64(BYTE *src, BYTE *encoded)
{
    BYTE* ENC;
    INT   size;
    INT   i, k, n;
    
    size = STRLEN(src);
    ENC = encoded;
    //ENC  = (BYTE *)SKY_Malloc(((sizeof(src) + 2) / 3) * 4 + 1);
    memset(ENC, 0x00, sizeof(ENC));

    {
        for(i = 0; i < size/3; i++)
        {
            ENC[i*4+0] = BASE64KEY[  src[i*3+0] / (unsigned char)4];
            ENC[i*4+1] = BASE64KEY[((src[i*3+0] & (unsigned char)3)    * (unsigned char)16) | (src[i*3+1] / (unsigned char)(16*1))];
            ENC[i*4+2] = BASE64KEY[((src[i*3+1] & (unsigned char)0x0f) * (unsigned char)4)  | (src[i*3+2] / (unsigned char)(16*4))];
            ENC[i*4+3] = BASE64KEY[ (src[i*3+2] & (unsigned char)0x03f)];
        }
        
        n = size / 3;
        k = size - n * 3;
        if (k == 1)
        {
            ENC[i*4+0] = BASE64KEY[  src[n*3+0] / (unsigned char)4];                         
            ENC[i*4+1] = BASE64KEY[((src[n*3+0] & (unsigned char)3)    * (unsigned char)16) | (src[n*3+1] / (unsigned char)(16*1))];
            ENC[i*4+2] = '=';
            ENC[i*4+3] = '=';
        }
        if (k == 2)
        { 
            ENC[i*4+0] = BASE64KEY[  src[n*3+0] / (unsigned char)4];
            ENC[i*4+1] = BASE64KEY[((src[n*3+0] & (unsigned char)3)    * (unsigned char)16) | (src[n*3+1] / (unsigned char)(16*1))];                              
            ENC[i*4+2] = BASE64KEY[((src[n*3+1] & (unsigned char)0x0f) * (unsigned char)4)  | (src[n*3+2] / (unsigned char)(16*4))];                              
            ENC[i*4+3] = '=';
        }
    }


	ENC[i*4+4] = '\0';

    /* COPY */
/*
    size = STRLEN(ENC);
    for(i=0, n=0, k=0; i < size; i++, k++)
    {
        encoded[n++] = ENC[i];
        if(k == 79)
        {
            encoded[n++] = 10;
            encoded[n++] = 13;
            k = 0;
        }
    }

    SKY_Free(ENC);
*/
}


int SKY_EncodeBase64_Size(unsigned char *src, unsigned char *encoded, int size)
{
    unsigned char* ENC;
    int        i, k, n;
    
    ENC = encoded;
//  memset(ENC, 0x00, sizeof(ENC));
    {
        for(i = 0; i < size/3; i++)
        {
            ENC[i*4+0] = BASE64KEY[  src[i*3+0] / (unsigned char)4];
            ENC[i*4+1] = BASE64KEY[((src[i*3+0] & (unsigned char)3)    * (unsigned char)16) | (src[i*3+1] / (unsigned char)(16*1))];
            ENC[i*4+2] = BASE64KEY[((src[i*3+1] & (unsigned char)0x0f) * (unsigned char)4)  | (src[i*3+2] / (unsigned char)(16*4))];
            ENC[i*4+3] = BASE64KEY[ (src[i*3+2] & (unsigned char)0x03f)];
        }
        
        n = size / 3;
        k = size - n * 3;
        if (k == 1)
        {
            ENC[i*4+0] = BASE64KEY[  src[n*3+0] / (unsigned char)4];                         
            ENC[i*4+1] = BASE64KEY[((src[n*3+0] & (unsigned char)3)    * (unsigned char)16) | (src[n*3+1] / (unsigned char)(16*1))];
            ENC[i*4+2] = '=';
            ENC[i*4+3] = '=';
        }
        if (k == 2)
        { 
            ENC[i*4+0] = BASE64KEY[  src[n*3+0] / (unsigned char)4];
            ENC[i*4+1] = BASE64KEY[((src[n*3+0] & (unsigned char)3)    * (unsigned char)16) | (src[n*3+1] / (unsigned char)(16*1))];                              
            ENC[i*4+2] = BASE64KEY[((src[n*3+1] & (unsigned char)0x0f) * (unsigned char)4)  | (src[n*3+2] / (unsigned char)(16*4))];                              
            ENC[i*4+3] = '=';
        }
    }
	ENC[i*4+4] = '\0';
	return STRLEN(ENC);
}


INT16 decode_base64(INT16 c)
{
	if ('A' <= c && c <= 'Z')
		return c - 'A';
	if ('a' <= c && c <= 'z')
		return c - 'a' + 26;
	if ('0' <= c && c <= '9')
		return c - '0' + 52;
	if ( c == '+')
		return 62;
	if ( c == '/')
		return 63;
	if ( c == '=')
		return -1;
	return -2;
}


INT16 decoding_base64(BYTE* dst, BYTE* src)
{
	INT16 count = 0;
	INT16 buf[4];	
	
	for (count = 0 ;count < 4; count++)
	{
		if (*src == '?')
		{
			if (*(src+1) == '=')
			{
				return -1;
			}
		}

		if ( (buf[count] = decode_base64(*src++)) == -2 )
			return 0;
	}

	*dst++ = (buf[0] << 2) | (buf[1] >>4);			
	if (buf[2] == -1) 
		return 1;

	*dst++ = (buf[1] << 4) | (buf[2] >>2);
	if (buf[3] == -1) 
		return 2;
		
	*dst++ = (buf[2] << 6) | (buf[3]);
	return 3;
}

BOOL isbase64(BYTE c)
{	
	UINT16 i;

	for(i=0;i<sizeof(BASE64KEY);i++)
		if(c==BASE64KEY[i])
			return TRUE;

	return FALSE;
}

BYTE numbase64(BYTE c)
{
	byte i;

	for(i=0;i<sizeof(BASE64KEY);i++)
		if(c==BASE64KEY[i])
			return i;
	if(c=='=')
		return 65;

	return 0;
}

UINT16 decbase64(BYTE *c,BYTE *b)
{	
	UINT16 n;

	if(c[2] == 65)
	{	n = 1;
		c[2] = c[3] = 0;
	}
	else if(c[3] == 65)
	{	   n = 2;
		c[3] = 0;
	}
	else
		n = 3;

	/* el primer byte comprende el primer grupo y los 2
	   primeros bits del 2do grupo
	 */
	b[0] = (c[0] << 2) | (c[1] >> 4);
	/* los 4 ultimos bits del 2do grupo y los 4 primeros
	   bits del 3er grupo
	 */
	b[1] = ((c[1] & 0x0f) << 4) | (c[2] >> 2);
	/* los 2 ultimos bits del 3 grupo y todos los del
	   4to.
	 */
	b[2] = ((c[2] & 0x3) << 6) | c[3];

	return n;
}


INT16 SKY_DecodeBase64(BYTE *writeto, BYTE* convert, UINT16 nLen, UINT16 nMaxLen)
{
	BYTE* start = writeto;
    BYTE* convertStart = convert;

	BYTE c[3],b[4];
	UINT16 i;

	for(;;)
	{	
		if(writeto - start > nMaxLen)
			break;

		for(i=0;i<4;i++)
		{	c[i] = *convert;
			convert ++;
		
			if(convert - convertStart > nLen)
				break;

			while(!isbase64(c[i]))
			{	if(c[i]=='=')
					break;

				if(convert - convertStart > nLen)
					break;

				c[i] = *convert;
				convert ++;
			}
			
			c[i] = numbase64(c[i]);
		}
		
		if(i==0 || (convert - convertStart > nLen))
			break;

		i = decbase64(c,b);

		memcpy(writeto, b, i);
		writeto += i;
	}

	*writeto = 0;
	return writeto - start;
}

INT16 SKY_DecodeTitle(BYTE *writeto, BYTE* convert, UINT16 nLen)
{	
	BYTE* start = writeto;
	BOOL	fParsed = FALSE;

	UINT16	jq;
	BOOL status = FALSE;
	BYTE q[2];

	memset(q, 0x00, 2);
	jq = 0;

	while (*convert)
	{
        if(((writeto - start) > nLen) || fParsed)
            break;

		if (*convert == '=')
		{
			convert++;
			if (*convert == '?')
			{
				// start of encoding
				// skip to charset
				convert++;
				while (*convert && *convert != '?')
					if (*convert++ == 0) return 0;
				
				convert++;
				if ((*convert) && (*convert == 'B' || *convert == 'b'))
				{
					int ret;
					while (*convert && *convert != '?')
						if (*convert++ == 0) return 0;
					convert++;
					
					do {
						ret = decoding_base64(writeto, convert);
						if (ret == 0)
							break;
						if (ret == -1)	// End of Title
						{
							*writeto = 0;
							return writeto - start;
						}
						writeto += ret;
						convert += 4;
					} while (ret == 3);

					while (*convert && *convert != '?')
						if (*convert++ == 0) return 0;

					while (*convert && *convert != '=')
						if (*convert++ == 0) return 0;

					convert++;

					fParsed = TRUE;
				}
				else if (*convert && *convert == 'Q')
				{
					while (*convert && *convert != '?')
						if (*convert++ == 0) return 0;
					convert++;

					while (*convert)
					{
						if (*convert == '?')
						{
							if (convert[1] == 0)
								return -1;
							if (convert[1] == '=')
							{
								*writeto = 0;
								return writeto - start;
							}
						}

						switch(*convert) {
						case '=':
							status = TRUE;
							break;
						case '_':
							*writeto = ' ';
							writeto++;
							break;
						case ' ':
							if(status)
								status = FALSE;
						default:
							if(status)
							{
								q[jq++] = *convert;
								if(jq > 1) 
								{
									 *writeto = 
									  ((q[0] - (ch_isdigit (q[0]) ? '0' :  ((ch_isupper (q[0]) ? 'A' : 'a') - 10))  ) << 4) +
									   (q[1] - (ch_isdigit (q[1]) ? '0' :  ((ch_isupper (q[1]) ? 'A' : 'a') - 10))  );
									 status = FALSE;
									 jq = 0;
									 writeto++;
								}
							}
							else 
							{
								*writeto = *convert;
								writeto++;
							}
							break;
						}

						convert++;
					}

					while (*convert && *convert != '=')
						if (*convert++ == 0) return 0;
					convert++;

					fParsed = TRUE;
				}
				else if(*convert == 0)
				{
					return 0;
				}
			}
			else 
			{				
				*writeto = '=';
				writeto++;
				if (*convert == 0)
					break;
				*writeto = *convert;
				writeto++;
				convert++;
			}
		}

		else 
		{
			*writeto = *convert;
			writeto++;
			convert++;
		}
	}

	*writeto = 0;
	return writeto - start;
}


LOCAL BOOL ch_isupper(BYTE ch)
{ // A - Z
	if( (ch >= 'A') && (ch <= 'Z') )
		return TRUE;
	else
		return FALSE;
}

LOCAL BOOL ch_isdigit(BYTE ch)
{ // 0 - 9
	if( (ch >= '0') && (ch <= '9') ) 
		return TRUE;
	else 
		return FALSE;
}

INT16 SKY_DecodePrintable(BYTE *writeto, BYTE* convert, UINT16 nLen, UINT16 nMaxLen)
{	
	BYTE* start = writeto;
	UINT16	i, jq;
	BOOL status = FALSE;
	BYTE q[2];

	memset(q, 0x00, 2);
	jq = 0;

    memset(writeto, NULL, nMaxLen);

	for(i=0; i< nLen; i++, convert++)
	{
        if(writeto - start > nMaxLen)   break;

		switch(*convert) {
		case '=':
			status = TRUE;
			break;
		case '_':
			*writeto = ' ';
			writeto++;
			break;
		case ' ':
			if(status)
				status = FALSE;
		default:
			if(status)
			{
                if(*convert == '\r' || *convert == '\n')
                {
                    status = FALSE;
                    jq = 0;
                    break;
                }
				q[jq++] = *convert;
				if(jq > 1) 
				{
					 *writeto = 
					  ((q[0] - (ch_isdigit (q[0]) ? '0' :  ((ch_isupper (q[0]) ? 'A' : 'a') - 10))  ) << 4) +
					   (q[1] - (ch_isdigit (q[1]) ? '0' :  ((ch_isupper (q[1]) ? 'A' : 'a') - 10))  );
					 status = FALSE;
					 jq = 0;
					 writeto++;
				}
			}
			else 
			{
                if(*convert == '\r' || *convert == '\n')
                    break;

				*writeto = *convert;
				writeto++;
			}
			break;
		}
	}

	if(status)
	{
		q[jq++] = *convert;
		if(jq > 1) 
		{
			 *writeto = 
			  ((q[0] - (ch_isdigit (q[0]) ? '0' :  ((ch_isupper (q[0]) ? 'A' : 'a') - 10))  ) << 4) +
			   (q[1] - (ch_isdigit (q[1]) ? '0' :  ((ch_isupper (q[1]) ? 'A' : 'a') - 10))  );
			 status = FALSE;
			 jq = 0;
			 writeto++;
		}
	}
	*writeto = 0;
	return writeto - start;
}