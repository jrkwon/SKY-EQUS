/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_sms.h
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
** 2000-12-16   huni     Created.
**
** ============================================================================
*/

#ifndef _SKA_SMS_H_
#define _SKA_SMS_H_

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
#define LCD_COLUME_SIZE		        18
#define MAX_RECEIVE_MESSAGE	        40 // 20->40 huni Max Size 40
#define MAX_VIEW_BUF_COLUME_SIZE    25	

#define SMS_MESG_DELETE_ORDER       0x01
#define SMS_MESG_DELETE_ALL_ORDER   0x02        
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
extern BYTE    *paMsgListBuf[MAX_RECEIVE_MESSAGE];
extern BYTE    *paMsgViewBuf;
extern INT8    nListType; 
extern INT8    nListIdx; 
	
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
extern VOID SKA_MainSMS(SKY_EVENT_T *pEvent);
extern VOID SKA_MessageNotif(SKY_EVENT_T* pEvent);  //white 2001.03.21
extern INT8  g_GetLength( BYTE *CheckStr, INT8  nBufLen);

extern BYTE g_MakeHyphenedNumber (
    BYTE      *Intxt,            // number to add hyphen
    BYTE       nLength,          // length of number   
    BYTE      *Outtxt            // result text   
);

extern VOID SKY_API SKY_InitSmsData(void);
extern VOID SKY_API SKA_DeleteAllMsg(SKY_EVENT_T* pEvent);
extern VOID SKA_ViewTextMessage(SKY_EVENT_T* pEvent); 

// 2001.8.13 huni
extern VOID SKA_ReceiveMsg(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_MsgAlarm(SKY_EVENT_T* pEvent);

extern  INT8 g_NewMsgCount(VOID);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_SMS_H_ */
