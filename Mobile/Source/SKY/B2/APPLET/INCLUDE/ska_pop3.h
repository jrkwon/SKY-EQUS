/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_pop3.h
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
** ============================================================================
*/

#ifndef _SKA_POP3_H_
#define _SKA_POP3_H_

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

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

enum tag_EMAIL_RECV_RESULT_E {
    ERR_CONTINUE,
    ERR_NEXT,
    ERR_NOMAIL,
    ERR_RECVOK,
	ERR_FULL,
    ERR_NONEWMAIL,
    ERR_FAIL,
    ERR_USER,
    ERR_PASS
};

typedef enum tag_EMAIL_RECV_RESULT_E EMAIL_RECV_RESULT_E;

enum tag_EMAIL_POP3_STATE_E {
    EPS_READY,
    EPS_USER,
    EPS_PASS,
    EPS_STAT,
    EPS_LIST,
	EPS_LAST,
    EPS_UIDL,
    EPS_RETR,
    EPS_DATA,
    EPS_QUIT,
    EPS_NONE
};

typedef enum tag_EMAIL_POP3_STATE_E EMAIL_POP3_STATE_E;

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

extern void SKY_ConnectPOP3Server(void);
extern void SKY_API SKY_SendRequest(EMAIL_POP3_STATE_E initState, BOOL fReSend, BOOL fNew,
                                HCONTROL hControl);
extern void SKY_API  SKY_ParseEmail(UINT16 nLen);

extern EMAIL_RECV_RESULT_E SKY_API SKY_ReceivePOP3Reply(HCONTROL hControl, BOOL fNew);
    
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_POP3_H_ */
