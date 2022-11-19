/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_mail.h
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
** ============================================================================
*/

#ifndef _SKA_MAIL_H_
#define _SKA_MAIL_H_

#ifdef __cplusplus
extern "C" {
#endif
    
//#define EMAIL_DEBUG
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
#define MAIL_SENDER    64
#define MAIL_RECEIVER  128
#define MAIL_TITLE     64
#define MAIL_UIDL      50
#define MAIL_TEXT      500
#define MAIL_DATE       20

#define MAIL_MAX_LINK  1

#define MAX_RECV_MAIL_SIZE  10*1024

#define MAX_INBOX_MAIL  20
#define MAX_SAVED_MAIL  20

// 받은편지함    
#define INBOX_LIST_FILE     "InBox.lst"
#define INBOX_DATA_FILE     "InBox.dat"

// 보낼편지함
#define OUTBOX_LIST_FILE    "OutBox.lst"
#define OUTBOX_DATA_FILE    "OutBox.dat"

/* rosa 2001-09-26 --> @why, what */
#define GET_EMAIL_STATUS_X(StrWidth)        ((SKY_SCREEN_WIDTH-StrWidth)/2)
/* <-- */

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef struct tagEMAIL_SEND_T EMAIL_SEND_T;
typedef struct tagEMAIL_LIST_T EMAIL_LIST_T;
typedef struct tagEMAIL_RECV_T EMAIL_RECV_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

struct tagEMAIL_SEND_T {
    UINT8   nMailNumber;
    BYTE    szSender[MAIL_SENDER];
    BYTE    szReceiver[MAIL_RECEIVER];
    BYTE    szTitle[MAIL_TITLE];
    BYTE    szText[MAIL_TEXT];
//    BYTE    szLinkFile[MAIL_MAX_LINK][SKY_FILE_EXT_NAME_MAX_LENGTH];
};

struct tagEMAIL_LIST_T{
    UINT8   nMailNumber;    // if NULL, not exist.
    BYTE    szSender[MAIL_SENDER];
    BYTE    szReceiver[MAIL_RECEIVER];
    BYTE    szTitle[MAIL_TITLE];
    BYTE    szUIDL[MAIL_UIDL];
    DWORD   wTime;      
};

struct tagEMAIL_RECV_T{
    UINT8   nMailNumber;
    BYTE    szSender[MAIL_SENDER];
    BYTE    szTitle[MAIL_TITLE];
    BYTE    szText[MAIL_TEXT];
    BYTE    szDate[MAIL_DATE];
};

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern EMAIL_SEND_T g_SendMail;
extern EMAIL_RECV_T  g_RecvMail;

extern EMAIL_LIST_T g_MailList[MAX_SAVED_MAIL];
extern UINT8                  g_MailListNum[MAX_SAVED_MAIL];
extern UINT8				   g_MailNum;
extern BYTE    g_RecvBuffer[MAX_RECV_MAIL_SIZE];
extern INT16   g_RecvLen;

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API SKA_EnterEmailMenu(SKY_EVENT_T *pEvent);
    
extern void SKY_API SKA_MainEMailAccount(SKY_EVENT_T *pEvent);

extern void SKY_API SKA_MainMailInBox(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainMailOutBox(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainMailSend(SKY_EVENT_T *pEvent);

extern void SKY_API SKY_DeleteInBoxMail(void);


extern void SKY_EncodeBase64(BYTE *src, BYTE *encoded);
extern INT16 SKY_DecodeTitle(BYTE *writeto, BYTE* convert, UINT16 nLen);
extern INT16 SKY_DecodeBase64(BYTE* dst, BYTE* src, UINT16 nLen, UINT16 nMaxLen);
extern INT16 SKY_DecodePrintable(BYTE *writeto, BYTE* convert, UINT16 nLen, UINT16 nMaxLen);

extern void SKA_InitMail(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_MAIL_H_ */
