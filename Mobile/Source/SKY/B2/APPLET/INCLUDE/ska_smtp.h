/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_smtp.h
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
** 2001-02-11 woodstok     Created.
**
** ============================================================================
*/

#ifndef _SKA_SMTP_H_
#define _SKA_SMTP_H_

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
#define MAX_REPLY_LENGTH	256
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagReplyStack_T ReplyStack_T;

enum tag_EMAIL_SEND_RESULT_E {
    ESR_CONTINUE,
    ESR_NEXT,
    ESR_SENDOK,
    ESR_FAIL,
    ESR_RCPTERR
};

typedef enum tag_EMAIL_SEND_RESULT_E EMAIL_SEND_RESULT_E;

enum tag_EMAIL_SMTP_STATE_E {
    ESS_HELO,
    ESS_MAIL,
    ESS_RCPT,
    ESS_DATA,
    ESS_TEXT,
    ESS_QUIT,
    ESS_NONE
};

typedef enum tag_EMAIL_SMTP_STATE_E EMAIL_SMTP_STATE_E;
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagReplyStack_T {
    BOOL fValid;
	BYTE reply[MAX_REPLY_LENGTH + 1];
	INT nPos;
};

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

extern void SKY_API SKY_SendEMail(EMAIL_SMTP_STATE_E initState, BOOL fReSend, 
                                  HCONTROL hControl);
extern void SKY_ConnectSMTPServer(void);
extern void SKY_DisconnectServer(void);

extern EMAIL_SEND_RESULT_E SKY_API SKY_ReceiveSMTPReply(HCONTROL hControl);

extern INT SKY_RecvReply(ReplyStack_T *Reply, BYTE *szSave);
extern INT SKY_RecvData(ReplyStack_T *Reply);

extern void SKY_InitRecvBuffer(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SMTP_H_ */
