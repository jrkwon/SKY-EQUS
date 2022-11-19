/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_mo.h
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
** 2000-12-25 reo      Created.
**
** ============================================================================
*/

#ifndef _SKA_MO_H_
#define _SKA_MO_H_

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
#define MAX_SMS_MO          20
#define MAX_SENT_MSG        30 // huni sent 30 (20 ->40 ->30)
#define MAX_SEND_MSG        30 // huni send 30 (20 ->30)
#define	MAX_SMS_DIAL		32
#define	MAX_SMS_TEXT		80
#define DATE_SIZE           4

// huni add 01.9.4 For Dong-Bo
#define MAX_DONGBO_SIZE     20
#define	MAX_DONGBO_DIAL		20

#define MO_FILE_HEADER      UINT8

#define SMS_USED            0x01

#define SMS_SEND_FAIL       0
#define SMS_SEND_SUCCESS    1

/* Teleservice ID define */
#define TI_DEFAULT          4098

#ifdef FEATURE_SKVM
#define TI_LOAD UASMS_TELESERVICE_SKVM_LOAD // 64100
#define TI_DATA UASMS_TELESERVICE_SKVM_DATA // 64101
#endif // FEATURE_SKVM

#define TI_LOCATION			UASMS_TELESERVICE_LOCATION
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef enum tagMO_SEND_TYPE_E {
    MO_SEND_REPLY,
    MO_SEND_FORWARD
}MO_SEND_TYPE_E;


enum tagSMS_RESULT_MSG_E {  //SMS 메시지보관
    SMS_RESULT_FAIL = 1,
    SMS_RESULT_SUCCESS,
    SMS_RESULT_RETRY
};

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*          MOSEND.SKY file structure
    header : the number of used messages (1byte)
    body   : the series of used messages or unuused messages
*/
// huni add 01.9.4 For Dong-Bo
struct tagSMSMO_DONGBO_SEND_T {
    UINT8   nSendTotalSize;
	UINT8	nRcvNumberSize[MAX_DONGBO_SIZE];
	BYTE	pszRcvNumber[MAX_DONGBO_SIZE][MAX_DONGBO_DIAL + 1];
};
typedef struct  tagSMSMO_DONGBO_SEND_T  SMSMO_DONGBO_SEND_T;

struct tagSMSMO_SEND_T {
    UINT8   nAddr;      //index in file    
    UINT8   nAttr;
    DWORD   dwMadeTime; //sec
	UINT8	nID;
    UINT8   nPriority;
	UINT8	nRcvNumberSize;
	BYTE	pszRcvNumber[MAX_SMS_DIAL + 1];
    UINT8   nSendNumberSize;
    BYTE    pszSendNumber[MAX_SMS_DIAL + 1];
	UINT8	nMsgSize;
	BYTE	pszMsg[MAX_SMS_TEXT + 1];    
};
typedef struct  tagSMSMO_SEND_T  SMSMO_SEND_T;

#ifdef FEATURE_SKTT_BROADCAST_SMS
struct tagSMSMO_SEND_T {
    UINT8   nAddr;
    UINT8   nAttr;
    DWORD   dwMadeTime; //sec
	UINT8	nID;
    UINT8   nPriority;
	UINT8	nRcvNumberSize;
	BYTE	pszRcvNumber[MAX_SMS_DIAL + 1];
    UINT8   nSendNumberSize;
    BYTE    pszSendNumber[MAX_SMS_DIAL + 1];
	UINT8	nMsgSize;
	BYTE	pszMsg[MAX_SMS_TEXT + 1];    
};
typedef struct tagSMSMO_SEND_T  SMSMO_SEND_T;
  
#endif //FEATURE_SKTT_BROADCAST_SMS
/*
struct tagSMSMO_SENT_T {
	UINT8   nAttr;
	UINT8	nID;
	UINT8	anDate[DATE_SIZE];
    UINT8   nPriority;
	UINT8	nRcvNumberSize;
	BYTE    pszRcvNumber[MAX_SMS_DIAL + 1];
    UINT8   nSendNumberSize;
    BYTE    pszSendNumber[MAX_SMS_DIAL + 1];
	UINT8	nMsgSize;
	BYTE	pszMsg[MAX_SMS_TEXT + 1];    
};
typedef struct  tagSMSMO_SENT_T  SMSMO_SENT_T;
*/

struct tagSMSMO_UI_T {
    UINT8   nAddr;
    UINT8   nAttr;
    DWORD   dwMadeTime; //secs
    UINT8   nID;
};
typedef struct  tagSMSMO_UI_T  SMSMO_UI_T;

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern SMSMO_UI_T								g_aSMSSendMsg[MAX_SEND_MSG]; // huni send 30 (20 ->30)
extern SMSMO_UI_T                               g_aSMSSentMsg[MAX_SENT_MSG]; // huni sent 30 (20 ->40 ->30)
extern SMSMO_UI_T                               g_aSMSSaveMsg[MAX_SMS_MO]; // for DataManager

extern UINT8                                    g_nMOSendSize;
extern UINT8                                    g_nMOSentSize;
extern UINT8                                    g_nMOSaveSize;          //for DataManager
extern SMSMO_SEND_T                             g_UIMOBuffer;

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/* APPLET */
extern VOID SKA_SendMsg(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_SMSSendMsg(SKY_EVENT_T *pEvent);
extern VOID SKY_API SKA_SMSEditMessage(SKY_EVENT_T *pEvent);
#ifdef FEATURE_SKTT_EQUS_LOCATION  
extern VOID SKY_API SKA_LocationSendMsg(SKY_EVENT_T *pEvent);
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
/* UTILITY */
extern VOID SKY_API SKY_InitMOSend(VOID);
extern VOID SKY_API SKY_InitMOSent(VOID);
extern VOID SKY_API SKY_InitMOSave(VOID);

extern VOID FillSMSMOSendMsg(BYTE* pNumber, BYTE* pMsg, MO_SEND_TYPE_E type);

// mo save
extern VOID SKY_SeekReadMOSaveT( UINT8 nIndex, SMSMO_SEND_T* pReadRecord);
extern VOID SKY_SeekWriteMOSaveT( UINT8 nIndex, SMSMO_SEND_T* pWriteRecord, MO_FILE_HEADER* pUsedNum);
extern VOID SaveSMSMOSaveMsg(SMSMO_SEND_T* pUIMOSend);
extern VOID DeleteSMSMOSaveMsg(UINT8 nDelIndex);
extern BYTE** MakeMOSaveScrollList(VOID);
extern VOID GetMOSaveSize(UINT8* pnMOSentTotalNum);
extern VOID DeleteAllSaveMsg(VOID);

// mo sent
extern VOID SKY_SeekReadMOSentT( UINT8 nIndex, SMSMO_SEND_T* pReadRecord);
extern VOID SKY_SeekWriteMOSentT( UINT8 nIndex, SMSMO_SEND_T* pWriteRecord, MO_FILE_HEADER* pUsedNum);
extern VOID SKY_API SKA_SMSEditNumber(SKY_EVENT_T *pEvent);
extern VOID SaveSMSMOSentMsg(SMSMO_SEND_T* pUIMOSend);
extern VOID DeleteSMSMOSentMsg(UINT8 nDelIndex);
extern BYTE** MakeMOSentScrollList(VOID);
extern VOID GetMOSentSize(UINT8* pnMOSentTotalNum);
extern VOID DeleteAllSentMsg(VOID);

// mo send
extern VOID SKY_SeekReadMOSendT( UINT8 nIndex, SMSMO_SEND_T* pReadRecord);
extern VOID SKY_SeekWriteMOSendT( UINT8 nIndex, SMSMO_SEND_T* pWriteRecord, MO_FILE_HEADER* pUsedNum);
extern VOID SaveSMSMOSendMsg(SMSMO_SEND_T* pUIMOSend);
extern VOID DeleteSMSMOSendMsg(UINT8 nDelIndex);
extern BYTE** MakeMOSendScrollList(VOID);
extern VOID GetMOSendSize(UINT8* pnMOSendTotalNum);
extern VOID DeleteAllSendMsg(VOID);

extern VOID SendSMSMsg(SMSMO_SEND_T *sms_mo, UINT16 nTeleserviceID);
extern VOID FreeSMSMOScrollList(UINT8 nMOSize);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_MO_H_ */
