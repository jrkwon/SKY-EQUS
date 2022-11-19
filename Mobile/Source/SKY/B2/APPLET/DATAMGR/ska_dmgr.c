/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_dmgr.c
**
** Description: 
**     전화부의 인덱스들은 1 ~ MAX 까지//
**
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 4334-01-18 white     Created.
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdlib.h>
#include "sky.h"
#include "ska.h"

#include <stdio.h>
#ifndef WIN32
#include "mobile.h" 
#endif

#define sMAX_MELODY_COUNT       50
#define sMAX_IMAGE_COUNT        50
//#define sMAX_PHOTO_COUNT        50
//#define     NEED_MIN_SPACE              IMAGE_VIEWER_TOTAL_SIZE + SKY_MAX_CAMERA_IMAGE_FILE_LEN + 4
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define	DM_OK				0x0000
#define	DM_NG				0x0001
#define	DM_WRONG_PW			0x0002

#define	DM_PING				0x0100
#define	DM_PONG				0x0101
#define	DM_GET_PHONE_SPEC	0x0102
#define	DM_PHONE_SPEC_DATA	0x0103

#define	DM_GET_USED_BOOK	0x0200
#define	DM_USED_BOOK_LIST	0x0201
#define	DM_GET_BOOK			0x0202
#define	DM_BOOK_DATA		0x0203
#define	DM_SET_BOOK			0x0204
#define	DM_DEL_BOOK			0x0205

//EQUS B0, B1 Only
#define DM_GET_NUM          0x0206
#define DM_NUM_DATA         0x0207
#define DM_SET_NUM          0x0210
#define DM_DEL_NUM          0x0211
//End

#define DM_GET_USED_GROUP   0x0212
#define DM_USED_GROUP_LIST  0x0213
#define	DM_GET_GROUP		0x0214
#define	DM_GROUP_DATA		0x0215
#define	DM_SET_GROUP		0x0216
#define	DM_SET_GROUP2		0x0217

#define	DM_GET_SPEED_DIAL	0x0220
#define	DM_SPEED_DIAL_DATA	0x0221
#define	DM_SET_SPEED_DIAL	0x0222
#define	DM_SET_SPEED_DIAL2	0x0223

#define	DM_GET_USED_SMS		0x0300
#define	DM_USED_SMS_LIST	0x0301

#define	DM_GET_MO			0x0310
#define	DM_MO_DATA			0x0311
#define	DM_SET_MO			0x0312
#define	DM_DEL_MO			0x0313
#define	DM_SEND_MO			0x0314

#define	DM_GET_MT			0x0320
#define	DM_MT_DATA			0x0321
#define	DM_NEW_MT			0x0322
#define	DM_DEL_MT			0x0323

#define	DM_SET_MELODY		0x0400
#define	DM_SET_ANIMATION	0x0401
#define	DM_GET_MELODY_REQ	0x0402
#define	DM_GET_MELODY_RSP	0x0403
#define	DM_GET_ANIMATION_REQ	0x0404
#define	DM_GET_ANIMATION_RSP	0x0405

#define	DM_GET_ALARM		0x0500
#define	DM_ALARM_DATA		0x0501
#define	DM_SET_ALARM		0x0502

#define DM_SET_ALARM2       0x0807          //2001.04.27

#define	DM_GET_MEMO			0x0503
#define	DM_MEMO_DATA		0x0504
#define	DM_SET_MEMO			0x0505

//EQUS Only
#define DM_GET_USED_SCHEDULE    0x0600
#define DM_USED_SCHEDULE_LIST   0x0601
#define DM_GET_SCHEDULE     0x0602
#define DM_SCHEDULE_DATA    0x0603
#define DM_SET_SCHEDULE     0x0604
#define DM_SET_SCHEDULE2    0x0605

#define DM_GET_HOLIDAY      0x0610
#define DM_HOLIDAY_DATA     0x0611
#define DM_SET_HOLIDAY      0x0612
#define DM_SET_HOLIDAY2     0x0613
//EQUS end.

////////////////////////////////////////////////
// Data manager message OP codes - for MM Data!

#define DM_GET_MMINFO		0x0700
#define DM_MMINFO_DATA		0x0701

#define DM_GET_FILELIST		0x0702
#define DM_FILELIST_DATA	0x0703

#define DM_GET_FILEINFO		0x0704
#define DM_FILEINFO_DATA	0x0705

#define DM_MMFILE_DELETE 	    0x0706 
#define DM_MMFILE_DELETE_RSP	0x0707 

#define DM_FILEDOWN_REQ		0x0710
#define DM_FILEDOWN_RSP		0x0711

#define DM_FILEUP_REQ		0x0712
#define DM_FILEUP_RSP		0x0713

/////////////////////////////////////
// Data manager message OP codes - for Phone Environment
#define DM_GET_PHONE_ENV    0x0800
#define DM_PHONE_ENV_DATA   0x0801

#define DM_SET_PHONE_ENV    0x0802

//2001/03/23 EQUS_ONLY 전화번호부의 메모 데이터 따로 나감.
#define DM_GET_BOOK_EMAIL        0x0803
#define DM_BOOK_EMAIL_DATA        0x0804
#define DM_SET_BOOK_EMAIL         0x0806
#define DM_DEL_BOOK_EMAIL         0x0805
//EQUS_END

#define DM_SEND_END         0x0807
#define DM_SET_END          0x0808

#define	DM_END				0xFFFF
#define GROUP_DELETE        2

enum {
    MEL_TYPE_PCM = 0,
    MEL_TYPE_MA2 = 1,
    MEL_TYPE_MMF = 2
};


#ifdef	FEATURE_IP_AIRDATA		
#define	DM_DISCONNECT		0x0900
#define IP_BLOCK_TRANSFER_SIZE	1000//500
#define IP_DM_MAX_WRT_LEN 90//80
#define	DM_MAX_SEND_MSG_LEN	2500
#define	DM_MAX_RECV_MSG_LEN	256
#define	IP_DMAN_SEND_LINE	3
#endif	// #ifdef	FEATURE_IP_AIRDATA		

#define BLOCK_TRANSFER_SIZE  1000//100//50
#define	DM_MAX_MSG_LEN	2500
#define	DM_MAX_WRT_LEN	90//80

#define	DMAN_SEND_DELAY		25
#define	DMAN_SEND_LINE		3

#define DMAN_FILESAVE_TIMEOUT   2000        // 2Sec
#define OK_MESSAGE_INDEX        5
#define ANI_TYPE_MAX            5


#if SKY_MODEL_ID == SKY_MODEL_B1
#define	SPEC_Model              "IM-3100"
#elif SKY_MODEL_ID == SKY_MODEL_B2
#define	SPEC_Model              "IM-00B2"
#endif

#define	SPEC_BookSize			MAX_BOOK_NUMBER//UI_NV_DIALS
#define	SPEC_SDialSize			MAX_SDIAL_NUMBER
#define	SPEC_GroupSize			MAX_GROUP_NUMBER//NV_MAX_GROUP
#define	SPEC_BookNameLen		MAX_BOOK_NAME//NV_MAX_LTRS
#define	SPEC_NumLen			    PB_MAX_DIGIT_LENGTH//NV_MAX_DIAL_DIGITS 
#define	SPEC_HomeAddrLen		0
#define	SPEC_OfficeAddrLen		0

#define	SPEC_EmailLen			PB_MAX_STRING_LENGTH//NV_MAX_DIAL_DIGITS

#define	SPEC_UrlLen				0
#define	SPEC_AnnivLen			MAX_ANNIV_NAME//NV_MAX_LTRS
#define	SPEC_MemoLen			PB_MAX_MEMO_LENGTH//NV_MAX_MEMO_SIZ
#define	SPEC_GroupNameLen		MAX_GROUP_NAME//NV_MAX_LTRS
#define	SPEC_HiddenLen			1

#define	SPEC_Clip1Size			0
#define	SPEC_Clip1Name			
#define	SPEC_Clip2Size			0
#define	SPEC_Clip2Name
#define	SPEC_Clip3Size			0
#define	SPEC_Clip3Name

#define	SPEC_SmsTextLen			80
#define	SPEC_SmsDialLen			PB_MAX_DIGIT_LENGTH//NV_MAX_DIAL_DIGITS
#define	SPEC_MoSize				30
#define	SPEC_MtSize				40

#define SPEC_MAXMMDATA          10//MM_MAX_MMEDIA_AVAIL_MEM
#define	SPEC_AniSize			10//MAX_MYANI_SIZE
#define	SPEC_AniWidth			120
#define	SPEC_AniHeight			120
#define	SPEC_AniLength			14400            // 120 * 120
#define SPEC_AniMaxLength       (14400 * MAX_ANIMATION_FRAME)     // Max 50 Frame
#define SPEC_MelType            MEL_TYPE_MA2
#define SPEC_MelodyMexLength    30000           // MAX 30k

#define	SPEC_MaxAlarmSize		MAX_ALARM_COUNT
#define	SPEC_AlarmNameLen		MAX_ALARM_TITLE + 1//NV_MAX_LTRS
#define	SPEC_MaxMemoSize		MAX_MEMO_COUNT//NV_MAX_MEMOS
#define	SPEC_MemoNameLen		MAX_MEMO_DATA_LENGTH//NV_MAX_MEMO

#define SPEC_MaxScheduleSize    SCHL_MAX_RECORD_COUNT - 1
#define SPEC_ScheduleNameLen    SCHL_MAX_EVENT_DATA_LENGTH
#define SPEC_MaxHolidaySize     SCHL_MAX_COUNT_MY_HOLIDAY
#define SPEC_HolidayNameLen     MAX_SIZE_TITLE_LINE

#define SPEC_MaxMMFileNameLen   20//SKY_FILE_NAME_MAX_LENGTH

#define DM_MELODY_TYPE          1
#define DM_ANI_TYPE             2
#define DM_PHT_TYPE             3

#define DM_FILE_NO_WORK         0
#define DM_FILE_DOWNLOAD        1
#define DM_FILE_UPLOAD          2

#define MM_OK   1
#define MM_NG   2
#define MM_FILE_NOT_EXIST   3

extern void resetBioSW9M(void);
BOOL FileRenameTemp2Target(const BYTE* pszPathName, DWORD size);

VOID    DmSendMessage(UINT16 OpCode, INT Length);
VOID    DmSendOk(VOID);
VOID    DmSendNg(VOID);
VOID    DmSendWrongPassword(VOID);
UINT8   DmReceivePing(VOID);
UINT8   DmReceiveGetPhoneSpec(VOID);
UINT8	DmReceiveSetEndDM(VOID);
UINT8	DmSendEndDM(VOID);
UINT8   DmReceiveGetUsedBook(VOID);
UINT8   DmReceiveGetBook(VOID);
UINT8   DmReceiveSetBook(VOID);
UINT8   DmReceiveDelBook(VOID);
UINT8   DmReceiveGetNum(VOID);
UINT8   DmReceiveSetNum(VOID);
UINT8   DmReceiveDelNum(VOID);
UINT8   DmReceiveGetUsedGroup(VOID);
UINT8   DmReceiveGetGroup(VOID);
UINT8   DmReceiveSetGroup(VOID);
UINT8   DmReceiveSetGroup2(VOID);
UINT8   DmReceiveGetSpeedDial(VOID);
UINT8   DmReceiveSetSpeedDial(VOID);
UINT8   DmReceiveSetSpeedDial2(VOID);
UINT8   DmReceiveGetUsedSMS(VOID);
UINT8   DmReceiveGetMO(VOID);
UINT8   DmReceiveSetMO(VOID);
UINT8   DmReceiveDelMO(VOID);
UINT8   DmReceiveSendMO(VOID);
UINT8   DmReceiveGetMT(VOID);
UINT8   DmReceiveNewMT(VOID);
UINT8   DmReceiveDelMt(VOID);
UINT8   DmReceiveSetMelody(VOID);
UINT8   DmReceiveSetAnimation(VOID);
UINT8   DmReceiveGetMelodyReq(VOID);
UINT8   DmReceiveGetAnimationReq(VOID);
UINT8   DmReceiveGetAlarm(VOID);
UINT8   DmReceiveSetAlarm(VOID);
UINT8   DmReceiveSetAlarm2(VOID);
UINT8   DmReceiveGetMemo(VOID);
UINT8   DmReceiveSetMemo(VOID);

UINT8   DmReceiveGetUsedSchedule(VOID);
UINT8   DmReceiveGetSchedule(VOID);
UINT8   DmReceiveSetSchedule(VOID);
UINT8   DmReceiveSetSchedule2(VOID);

UINT8   DmReceiveGetHoliday(VOID);
UINT8   DmReceiveSetHoliday(VOID);
UINT8   DmReceiveSetHoliday2(VOID);

UINT8   DmReceiveGetMMInfo(VOID);
UINT8   DmReceiveGetFileList(VOID);
UINT8   DmReceiveGetFileInfo(VOID);
UINT8   DmReceiveFileDeleteReq(VOID);
UINT8   DmReceiveFileDownReq(VOID);
UINT8   DmReceiveFileUpReq(VOID);
UINT8   DmReceiveGetPhoneEnv(VOID);
UINT8   DmReceiveSetPhoneEnv(VOID);

UINT8   DmReceiveGetBookEmail(VOID);
UINT8   DmReceiveSetBookEmail(VOID);
UINT8   DmReceiveDelBookEmail(VOID);

#ifdef FEATURE_IP_AIRDATA
UINT8      DmReceiveDisconnect(VOID);
#endif

struct	_DmTableType	{
	UINT16	OpCode;					// OP code
	UINT8	(*JobFunction)(VOID);	// Service function
};



CONST struct _DmTableType DmTable[] = {
	DM_PING,			DmReceivePing,
	DM_GET_PHONE_SPEC,	DmReceiveGetPhoneSpec,
	DM_GET_USED_BOOK,	DmReceiveGetUsedBook,
	DM_GET_BOOK,		DmReceiveGetBook,
	DM_SET_BOOK,		DmReceiveSetBook,
	DM_DEL_BOOK,		DmReceiveDelBook,
    DM_GET_NUM,         DmReceiveGetNum,
    DM_SET_NUM,         DmReceiveSetNum,
    DM_DEL_NUM,         DmReceiveDelNum,
    
    DM_GET_BOOK_EMAIL,   DmReceiveGetBookEmail,
    DM_SET_BOOK_EMAIL,   DmReceiveSetBookEmail,
    DM_DEL_BOOK_EMAIL,   DmReceiveDelBookEmail,

    DM_GET_USED_GROUP,  DmReceiveGetUsedGroup,
    DM_GET_GROUP,		DmReceiveGetGroup,
	DM_SET_GROUP,		DmReceiveSetGroup,
	DM_SET_GROUP2,		DmReceiveSetGroup2,
	DM_GET_SPEED_DIAL,	DmReceiveGetSpeedDial,
	DM_SET_SPEED_DIAL,	DmReceiveSetSpeedDial,
	DM_SET_SPEED_DIAL2,	DmReceiveSetSpeedDial2,
	DM_GET_USED_SMS,	DmReceiveGetUsedSMS,
	DM_GET_MO,			DmReceiveGetMO,
	DM_SET_MO,			DmReceiveSetMO,
	DM_DEL_MO,			DmReceiveDelMO,
	DM_SEND_MO,			DmReceiveSendMO,
	DM_GET_MT,			DmReceiveGetMT,
	DM_NEW_MT,			DmReceiveNewMT,
	DM_DEL_MT,			DmReceiveDelMt,
	DM_SET_MELODY,		DmReceiveSetMelody,
	DM_SET_ANIMATION,	DmReceiveSetAnimation,
    DM_GET_MELODY_REQ,  DmReceiveGetMelodyReq,
    DM_GET_ANIMATION_REQ,   DmReceiveGetAnimationReq,
	DM_GET_ALARM,       DmReceiveGetAlarm,
	DM_SET_ALARM,		DmReceiveSetAlarm,
    DM_SET_ALARM2,      DmReceiveSetAlarm2,
	DM_GET_MEMO,		DmReceiveGetMemo,
	DM_SET_MEMO,		DmReceiveSetMemo,
    
    DM_GET_USED_SCHEDULE,   DmReceiveGetUsedSchedule,
    DM_GET_SCHEDULE,    DmReceiveGetSchedule,
    DM_SET_SCHEDULE,    DmReceiveSetSchedule,
    DM_SET_SCHEDULE2,   DmReceiveSetSchedule2,
    DM_GET_HOLIDAY,     DmReceiveGetHoliday,
    DM_SET_HOLIDAY,     DmReceiveSetHoliday,
    DM_SET_HOLIDAY2,    DmReceiveSetHoliday2,

    DM_GET_MMINFO,      DmReceiveGetMMInfo,
    DM_GET_FILELIST,    DmReceiveGetFileList,
    DM_GET_FILEINFO,    DmReceiveGetFileInfo,
    DM_MMFILE_DELETE,   DmReceiveFileDeleteReq,
    DM_FILEDOWN_REQ,    DmReceiveFileDownReq,
    DM_FILEUP_REQ,      DmReceiveFileUpReq,

    DM_GET_PHONE_ENV,   DmReceiveGetPhoneEnv,
    DM_SET_PHONE_ENV,   DmReceiveSetPhoneEnv,
    DM_SET_END,         DmReceiveSetEndDM,
    DM_SEND_END,        DmSendEndDM,

#ifdef	FEATURE_IP_AIRDATA		
	DM_DISCONNECT,		DmReceiveDisconnect,
#endif	// #ifdef	FEATURE_IP_AIRDATA		

	DM_END,				NULL
};

#define	BCD_2_ASCII(n)			(((n) >> 4)*10 + ((n) & 0x0F))

#define	ADD_BYTE(len, data)		{ dmSendBuffer[len++] = data; }
#define	ADD_WORD(len, data)		{ dmSendBuffer[len++] = (data) >> 8; dmSendBuffer[len++] = (data) & 0xFF; }
#define	ADD_STR(len, n, data)	{ STRNCPY((CHAR*)&dmSendBuffer[len], (CHAR*)data, n); len += n; }

#define	GET_BYTE(pos, data)		{ data = dmRcvBuffer[pos++]; }
#define	GET_WORD(pos, data)		{ data = (dmRcvBuffer[pos] << 8) | (dmRcvBuffer[pos+1]); pos += 2; }
#define	GET_STR(pos, n, data)	{ STRNCPY((CHAR*)data, (CHAR*)&dmRcvBuffer[pos], n); pos += n; }
#define	ASCII2DTMF(ch)	(((ch) == '0') ? 0xA : (((ch) == '*') ? 0xB : (((ch) == '#') ? 0xC : ((ch) - '0'))))

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

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
INT16   OkMessageIndex[OK_MESSAGE_INDEX] = { 0, 0, 0, 0, 0};

UINT8	DmMode = 0;				// DataManager Mode
STATIC UINT16	dmPhonePassword;
STATIC UINT8	dmRcvState;

#ifndef	FEATURE_IP_AIRDATA		
STATIC UINT8	dmRcvBuffer[DM_MAX_MSG_LEN];
STATIC UINT8	dmSendBuffer[DM_MAX_MSG_LEN];
CHAR	dmSendOutBuffer[DM_MAX_WRT_LEN*2 + 50/*20*/];

#else	// #ifndef	FEATURE_IP_AIRDATA		
STATIC UINT8	dmRcvBuffer[DM_MAX_RECV_MSG_LEN];
STATIC UINT8	dmSendBuffer[DM_MAX_SEND_MSG_LEN];
CHAR	dmSendOutBuffer[IP_DM_MAX_WRT_LEN*2 + 50/*20*/];

#endif	// #ifndef	FEATURE_IP_AIRDATA		

STATIC UINT16	dmRcvLength;
STATIC INT	    dmRcvCount;
STATIC UINT16	dmRcvPassword;
STATIC UINT16	dmRcvOpCode;
STATIC UINT8	dmRcvChecksum;

STATIC clk_cb_type      dmMoreTimer;
STATIC UINT8            dmMoreTimerAlive = 0;

INT     dmSendCount;
INT     dmSendLength;

INT     DmWriteDialFlag = 0;

extern UINT8	MoStatus;
CHAR        DmMoDial[SPEC_SmsDialLen + 1];
UINT16      DmMoTI;
BYTE            bDataTransBlock[1000];  

LOCAL       BYTE    l_memoryAlloc; 
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
#define DATA_SERVICE_ANI_X  30
#define DATA_SERVICE_ANI_Y  15

#define DATA_SERVICE_STATE_X   26
#define DATA_SERVICE_STATE_Y   60

void SKY_API SKA_IN_DataManager(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        SKY_SET_DATAMANAGER(TRUE);
        
        if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
        {
            DmSendNg();
            EQS_END();   
        }else
        {
            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE,
                NULL,//SKY_GET_TOKEN(TKN_IP_AIR_DATAMGR), 
                WA_SHOW_HEADER);
            
            l_memoryAlloc = 0;

            EQC_Image(DATA_SERVICE_ANI_X, DATA_SERVICE_ANI_Y, (BITMAP*)&g_BmpConnectDataService);  
            EQC_StaticText(DATA_SERVICE_STATE_X, DATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_IN_DATA_MANAGER1);
            EQC_StaticText(DATA_SERVICE_STATE_X, DATA_SERVICE_STATE_Y+18, (EQS_TOKEN)TKN_IN_DATA_MANAGER2);
        }
        break;

    case SE_DM_MO_CMD_F:        // 2001.02.09 Send Mo in DataManager.
		EQS_START_CHILD_EVENT(SKA_SMSSendMsg, SE_APP_START, EQS_NA, TI_DEFAULT);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        resetBioSW9M();
        if(SKY_GET_DATAMANAGER())
            DmSendEndDM();
        SKY_SET_DATAMANAGER(FALSE);
        l_memoryAlloc = 0;
        EQS_ReleaseDC();
        break;

    case SE_SOFTKEY_CANCEL:
    case SE_KEY_CLEAR:
    case SE_KEY_EXT_POWER_OFF:
        DmSendEndDM();
        EQS_END();
        break;


    case SE_KEY_END:
        DmSendEndDM();
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_DM_OUT_PROCESSING:
        EQS_END();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
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
**     DmReadAscii(CHAR *src)
**
** Description: 
**     Convert 2 chars ASCII text to hexadecimal
**
** Input:
**     CHAR *src
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     UINT8
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

UINT8   DmReadAscii(CHAR *src)
{
	INT	num, n1=0, n2=0;	

	//DebugOut("\t\tRead %c(%x)%c(%x)", *src, *src, *(src+1), *(src+1));

	if ('0' <= *src && *src <= '9')
		n1 = *src - '0';
	else if ('A' <= *src && *src <= 'F')
		n1 = *src - 'A' + 10;
	else if ('a' <= *src && *src <= 'f')
		n1 = *src - 'a' + 10;

	src++;

	if ('0' <= *src && *src <= '9')
		n2 = *src - '0';
	else if ('A' <= *src && *src <= 'F')
		n2 = *src - 'A' + 10;
	else if ('a' <= *src && *src <= 'f')
		n2 = *src - 'a' + 10;
	
	num = (n1 << 4) | n2;

	return num;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     DmWriteAscii(CHAR *dest, CHAR data)
**
** Description: 
**     Convert hexadecimal BYTE to 2 chars ASCII text
**
** Input:
**     (CHAR *dest, CHAR data)
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

VOID	DmWriteAscii(CHAR *dest, CHAR data)
{
	INT	n1, n2;

	n1 = (data >> 4) & 0x0F;
	n2 = data & 0x0F;

	if (n1 < 10)
		*dest = '0' + n1;
	else
		*dest = 'A' + n1 - 10;

	if (n2 < 10)
		*(dest + 1) = '0' + n2;
	else
		*(dest + 1) = 'A' + n2 - 10;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     DmRemoveTailSpace(CHAR* str, INT len)
**
** Description: 
**     Remove tail space of string
**
** Input:
**     
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

VOID	DmRemoveTailSpace(CHAR* str, INT len)
{
	INT	pos = 0;

	// Remove tail space
	while (len >= 1 && str[len - 1] == ' ') {
		str[--len] = '\0';
	}

	// Remove dash
	while (pos <= len && str[pos] != '\0') {
		if (str[pos] == '-') {
			memcpy(&str[pos], &str[pos+1], len - pos - 1);
			str[--len] = '\0';
		}
		else
			pos++;
	}
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     Dm_GetMMInfoState(DWORD *pAvailMMData, DWORD *pAniDataSize, DWORD *pMelDataSize)
**
** Description: 
**     Get Multimedia Info
**
** Input:
**     
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
VOID Dm_GetMMInfoState(DWORD *pAvailMMData, DWORD *pAniDataSize, DWORD *pMelDataSize, DWORD *pPhtDataSize)
{
    *pAvailMMData = SKY_GetFreeSpaceInDataFile();
    *pAniDataSize = SKY_GetUsedSpaceInDataFile(SDF_IMAGE);
    *pMelDataSize = SKY_GetUsedSpaceInDataFile(SDF_MELODY);
    *pPhtDataSize = SKY_GetUsedSpaceInDataFile(SDF_PHOTO);
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveMessage(UINT8 *src)
**
** Description: 
**     Receive chars from AT Command parsing routine
**     and make Data Manager message
**
** Input:
**     
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
UINT8	DmReceiveMessage(UINT8 *src)		// AT*SKTT0*XXXXXX\0
{
	INT		n, i;
    BYTE    szPassword[MAX_PASSWORD_LENGTH];

    STRCPY(szPassword, SKY_GET_PASSWORD());
    
    if(l_memoryAlloc > 10)
    {
        l_memoryAlloc = 0;
        SKY_ReinitMemory();
    }

	if (*src == '0') {
		dmRcvState = 0;
		memset(dmRcvBuffer, 0, DM_MAX_MSG_LEN);
		dmRcvLength = 0;
		dmRcvCount = 0;
		dmRcvPassword = 0;
		dmRcvOpCode = 0;
		dmRcvChecksum = 0;
	}

	// check invalid command
	if (*src != '0' && *src != '1')	{
		//DebugOut("\tWrong header");
		return 4;
	}
	src++;
	if (*src != '*') {
		//DebugOut("\tWrong header");
		return 4;
	}
	src++;

	// character parsing
	while (*src != 0x00) {

		n = DmReadAscii((CHAR *)src);	
		src += 2;

		switch (dmRcvState) {
		case 0:	// wait first length BYTE
			dmRcvLength = n << 8;
			dmRcvState = 1;
			break;

		case 1:	// wait second length BYTE
			dmRcvLength |= n;
			dmRcvState = 2;
			break;

		case 2: // wait first password BYTE
			dmRcvPassword = n << 8;
			dmRcvChecksum = n;
			dmRcvState = 3;
			break;

		case 3:	// wait second password BYTE
			dmRcvPassword |= n;
			dmRcvChecksum += n;

			dmRcvState = 4;
			break;

		case 4: // wait first OP code BYTE
			dmRcvOpCode = n << 8;
			dmRcvChecksum += n;
			dmRcvState = 5;
			break;

		case 5:	// wait second OP code BYTE
			dmRcvOpCode |= n;
			dmRcvChecksum += n;


			dmPhonePassword = (ASCII2DTMF(szPassword[0]) << 12)
							| (ASCII2DTMF(szPassword[1]) <<  8)
							| (ASCII2DTMF(szPassword[2]) <<  4)
							|  ASCII2DTMF(szPassword[3]);

			if (dmRcvOpCode != DM_PING &&
				dmRcvPassword != dmPhonePassword) {		// Invalid password
				DmSendWrongPassword();
				return 0;
			}

            if(dmRcvOpCode != DM_PING)
            {
                if(!SKY_GET_AIRDATAMANAGER_ORG() && !SKY_GET_AIRDATAMANAGER_RCV() && \
                    !SKY_GET_IP_AIRDATAMANAGER() && !SKY_GET_DATAMANAGER())       //데이터매니저 애플릿 상태에서만 전송할 수 있다.
                        return 0;
            }

			if (dmRcvLength > DM_MAX_MSG_LEN) {			// Invalid length
				return 4;
			}
#ifdef	DMAN_DEBUG		
			DebugOut(" [Op %04X Len %d ] ", dmRcvOpCode, dmRcvLength);
#endif

			dmRcvCount = 0; 
			dmRcvState = 6;
			break;

		case 6:	// data receiving & checksum check
			if (dmRcvCount < dmRcvLength - 4) {
				dmRcvBuffer[dmRcvCount++] = n;
				dmRcvChecksum += n;
			}
			else {
				if (n != dmRcvChecksum) { 				// Invalid checksum
					return 4;
				}
#ifdef	DMAN_DEBUG		
				DebugOut(" [Sum %02X] ", n);
#endif

				// Receive complete
				for (i = 0; DmTable[i].OpCode != DM_END; i++) {
					if (DmTable[i].OpCode == dmRcvOpCode) {
                        
						DmMode = 1;
                        //set to NonSleep during Flash handling
						if (BIO_GET_SW ( BIO_SW9_M ) == 0) 
                        {
							bio_set_sw ( ( bio_sw_type ) (bio_switches|BIO_SW9_M) );
						}
                        // Do Datamanager function
						return DmTable[i].JobFunction();
					}
				}
				return 4;
			}
			break;
		}
	}

	return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     VOID	DmSendMessageMore(int4 ms_interval)
**
** Description: 
**     
**
** Input:
**     
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
VOID	DmSendMessageMore(int4 ms_interval)
{
	INT	count;
	INT	i;
	static UINT16	dmMaxWrtLen;		
	
    if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV() || \
        SKY_GET_IP_AIRDATAMANAGER() ||  SKY_GET_DATAMANAGER())       //데이터매니저 애플릿 상태에서만 전송할 수 있다.
    {    

    //MSG_ERROR("DmSendMessageMore", 0, 0, 0);

    #ifdef	FEATURE_IP_AIRDATA		
	    if (SKY_GET_IP_AIRDATAMANAGER())
		    dmMaxWrtLen = IP_DM_MAX_WRT_LEN;
	    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA		
		    dmMaxWrtLen = DM_MAX_WRT_LEN;

	    // Reset timer
	    if (dmMoreTimerAlive == 1) {
		    clk_dereg(&dmMoreTimer);
		    dmMoreTimerAlive = 0;
	    }

    #ifdef	FEATURE_IP_AIRDATA		
	    if (SKY_GET_IP_AIRDATAMANAGER())
		    count = IP_DMAN_SEND_LINE;
	    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA		
		    count = DMAN_SEND_LINE;

	    while (count > 0 && dmSendCount < dmSendLength) {
		    // Make a line
		    STRCPY(dmSendOutBuffer, "*SKTT1*");
		    //for (i = 0; dmSendCount < dmSendLength && i < DM_MAX_WRT_LEN; i++)
		    for (i = 0; dmSendCount < dmSendLength && i < dmMaxWrtLen; i++)
			    DmWriteAscii(&dmSendOutBuffer[7 + i*2], dmSendBuffer[dmSendCount++]);
		    dmSendOutBuffer[7 + i*2] = '\0';
            MSG_HIGH("DMSENDOUTBUFFER %s",dmSendOutBuffer,0,0);
		    // Send a line
    #ifdef	FEATURE_SKT_AIRDATA	
		    if (SKY_GET_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	//	#ifdef	FEATURE_SKT_AIRDATA	
    #ifdef	FEATURE_IP_AIRDATA		
		    if (SKY_GET_IP_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA		
		    {
			    at_send_extended_text(dmSendOutBuffer);
		    }

		    count--;
	    }

	    if (dmSendCount < dmSendLength) {				// More data remain
		    // Timer version for multiple send
		    clk_reg(&dmMoreTimer, DmSendMessageMore, 500, 500, FALSE); 
		    dmMoreTimerAlive = 1;
	    }
	    else {											// Send OK
		    // Send OK
		    STRCPY(dmSendOutBuffer, "OK");
    #ifdef	FEATURE_SKT_AIRDATA	
		    if (SKY_GET_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	//	#ifdef	FEATURE_SKT_AIRDATA
    #ifdef	FEATURE_IP_AIRDATA		
		    if (SKY_GET_IP_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, (WORD)STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA		
		    {
			    at_send_extended_text(dmSendOutBuffer);
		    }
	    }
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     VOID	DmSendMessage(UINT16 OpCode, INT Length)
**
** Description: 
**     
**
** Input:
**     
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

VOID	DmSendMessage(UINT16 OpCode, INT Length)
{
	UINT8	checksum;
	INT	i;
	STATIC UINT16 dmMaxWrtLen;

    if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV() || \
        SKY_GET_IP_AIRDATAMANAGER() || SKY_GET_DATAMANAGER())       //데이터매니저 애플릿 상태에서만 전송할 수 있다.
    {

    #ifdef	DMAN_DEBUG	
	    DebugOut(" [Send %04X] ", OpCode);
    #endif

    //	MSG_ERROR("LENGHT: %d", Length, 0, 0);

    #ifdef	FEATURE_IP_AIRDATA		
	    if (SKY_GET_IP_AIRDATAMANAGER())
		    dmMaxWrtLen = IP_DM_MAX_WRT_LEN;
	    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA		
		    dmMaxWrtLen = DM_MAX_WRT_LEN;

	    // Append checksum
	    checksum = (OpCode >> 8) + (OpCode & 0xFF);
	    for (i = 0; i < Length; i++)
		    checksum += dmSendBuffer[i];
	    dmSendBuffer[Length++] = checksum;

	    // Make header
	    STRCPY(dmSendOutBuffer, "*SKTT0*");
	    DmWriteAscii(&dmSendOutBuffer[ 7], (Length + 3) >> 8);
	    DmWriteAscii(&dmSendOutBuffer[ 9], (Length + 3) & 0xFF);
	    DmWriteAscii(&dmSendOutBuffer[11], 0);
	    DmWriteAscii(&dmSendOutBuffer[13], 0);
	    DmWriteAscii(&dmSendOutBuffer[15], OpCode >> 8);
	    DmWriteAscii(&dmSendOutBuffer[17], OpCode & 0xFF);


	    if (Length < dmMaxWrtLen - 6) {
		    // Append short data
		    for (i = 0; i < Length; i++)
			    DmWriteAscii(&dmSendOutBuffer[19 + i*2], dmSendBuffer[i]);
		    // Flush header & data
		    dmSendOutBuffer[19 + i*2] = '\0';

    #ifdef	FEATURE_SKT_AIRDATA	
		    if (SKY_GET_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	//	#ifdef	FEATURE_SKT_AIRDATA
    #ifdef	FEATURE_IP_AIRDATA	
		    if (SKY_GET_IP_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA	
		    {
			    at_send_extended_text(dmSendOutBuffer);
		    }
		    // Send OK
		    STRCPY(dmSendOutBuffer, "OK");

    #ifdef	FEATURE_SKT_AIRDATA
		    if (SKY_GET_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer,STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	//	#ifdef	FEATURE_SKT_AIRDATA	
    #ifdef	FEATURE_IP_AIRDATA	
		    if (SKY_GET_IP_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, (WORD)STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA
		    {
			    at_send_extended_text(dmSendOutBuffer);
		    }
	    }
	    else {
		    // Flush header data
		    dmSendOutBuffer[19] = '\0';

    #ifdef	FEATURE_SKT_AIRDATA
		    if (SKY_GET_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	//	#ifdef	FEATURE_SKT_AIRDATA
    #ifdef	FEATURE_IP_AIRDATA	
		    if (SKY_GET_IP_AIRDATAMANAGER())
		    {
			    AirDataManagerTX((BYTE *)dmSendOutBuffer, STRLEN(dmSendOutBuffer));
		    }
		    else
    #endif	// #ifdef	FEATURE_IP_AIRDATA
		    {
			    at_send_extended_text(dmSendOutBuffer);
		    }

		    // Send long data
		    dmSendCount = 0;

		    // Timer version for multiple send
		    dmSendLength = Length;
		    clk_reg(&dmMoreTimer, DmSendMessageMore, DMAN_SEND_DELAY, DMAN_SEND_DELAY, FALSE); 
		    dmMoreTimerAlive = 1;
	    }
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     VOID	DmSendMessage(UINT16 OpCode, INT Length)
**
** Description: 
**     
**
** Input:
**     
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
VOID	DmSendOk(VOID)
{
    INT len;
    BYTE idx = 0;

    len = 0;
    ADD_WORD(len, dmRcvOpCode);

    //EQUS MODEL에만 적용
    for(idx = 0; idx < OK_MESSAGE_INDEX ; idx++)
        ADD_WORD(len, OkMessageIndex[idx]);
    //EQUS. 

    DmSendMessage(DM_OK, len);
}
VOID	DmSendNg(VOID)
{
    INT len;

    len = 0;
    ADD_WORD(len, dmRcvOpCode);
    DmSendMessage(DM_NG, len);
}
VOID	DmSendWrongPassword(VOID)
{
    INT len;

    len = 0;
    ADD_WORD(len, dmRcvOpCode);

    DmSendMessage(DM_WRONG_PW, len);
}

UINT8	DmReceivePing(VOID)	// Send Pong
{
    ui_cmd_type	    *cmd_ptr;	// Pointer to UI command
   
    if(!SKY_GET_AIRDATAMANAGER_ORG() && !SKY_GET_AIRDATAMANAGER_RCV() && \
        !SKY_GET_IP_AIRDATAMANAGER())
    {
        if ((cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q )) != NULL) {
	        cmd_ptr->hdr.cmd        = UI_DM_IN_PROCESSING_F;
	        cmd_ptr->hdr.task_ptr   = NULL;
	        cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
	        ui_cmd( cmd_ptr );
        }
        else {
	        //DebugOut("\tCmd alloc fail");			// Send message
	        DmSendNg();
	        return 1;
        }
    }
//    INT bSuccess = FALSE;

    //if (g_bIrDAExchangeMode)          //IRDA 전송중인지 알 수 있는 플래그
    /*{
        DmSendNg();
        return 0;
    }
    */
	DmSendMessage(DM_PONG, 0);
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetPhoneSpec(VOID)
**
** Description: 
**     Send PhoneBookData
**
** Input:
**     
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
UINT8	DmReceiveGetPhoneSpec(VOID)	// Send PhoneBookData
{
    //BYTE                bPhoneNum[12];
    INT					len;
    DWORD               AvailMMData, AniDataSize, MelDataSize, PhtDataSize;
    nv_dir_number_type  nv_dir_num;

    DmMode = 1; 
    
    // Clear send buffer
    l_memoryAlloc = 0;
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    len = 0;

    // Set phone spec to buffer
    //STRCPY(bPhoneNum, GetMyNumber());

    nv_dir_num.nam = SKY_GET_CURRENT_NAM();
    (void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );
    
    if( nv_dir_num.dir_number[0] != '0' )  {			// 011-XXXX-XXXX
        ADD_STR(len, 1, "0");						// PhoneNumber
        ADD_STR(len, 10, nv_dir_num.dir_number);
    }
    else {											// 011-XXX-XXXX
        ADD_STR(len, 10, nv_dir_num.dir_number);	// PhoneNumber
        ADD_BYTE(len, 0);
    }

    ADD_STR(len, 16, SPEC_Model);								        // Model


    PBookClose();
    if(SKY_IsFileOrDirExist(SKY_TEMP_FILE_NAME))
    {
        if(SKY_GET_DM_TEMP_FILE() != SKY_INVALID_FILE_HANDLE)
            SKY_CloseFile(SKY_GET_DM_TEMP_FILE());
    
        SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
        SKY_DeleteFile(SKY_TEMP_FILE_NAME);
    }
  
	STRCPY(&dmSendBuffer[len], SKY_GET_BANNER());
    
    //DebugOut("\tBanner %s", &dmSendBuffer[len]);
    len += 16;  // Add for Banner String Size

    ADD_WORD(len, SPEC_BookSize);			// BookSize
    ADD_BYTE(len, SPEC_SDialSize);			// SDialSize
    ADD_BYTE(len, SPEC_GroupSize);			// GroupSize
    ADD_BYTE(len, SPEC_BookNameLen);		// BookNameLen

    ADD_BYTE(len, SPEC_NumLen);			// HomeNumLen
    ADD_BYTE(len, SPEC_HomeAddrLen);		// HomeAddrLen
    ADD_BYTE(len, SPEC_OfficeAddrLen);		// OfficeAddrLen
    ADD_BYTE(len, SPEC_EmailLen);			// EmailLen
    ADD_BYTE(len, SPEC_UrlLen);				// UrlLen

    ADD_BYTE(len, SPEC_AnnivLen);			// AnnivLen
    ADD_BYTE(len, SPEC_MemoLen);			// MemoLen
    ADD_BYTE(len, SPEC_GroupNameLen);		// GroupNameLen
    ADD_BYTE(len, SPEC_HiddenLen);			// HiddenLen

    ADD_BYTE(len, SPEC_Clip1Size);			// Clip1Size
    ADD_BYTE(len, SPEC_Clip2Size);			// Clip2Size
    ADD_BYTE(len, SPEC_Clip3Size);			// Clip3Size

    ADD_BYTE(len, SPEC_SmsTextLen);			// SmsTextLen
    ADD_BYTE(len, SPEC_SmsDialLen);			// SmsDialLen
    ADD_BYTE(len, SPEC_MoSize);				// MoSize
    ADD_BYTE(len, SPEC_MtSize);				// MtSize

    Dm_GetMMInfoState(&AvailMMData, &AniDataSize, &MelDataSize, &PhtDataSize);

    ADD_WORD(len, (WORD)(SPEC_MAXMMDATA >> 16));
    ADD_WORD(len, (WORD)(SPEC_MAXMMDATA & 0x0000FFFF));
    ADD_WORD(len, (WORD)(AvailMMData >> 16));
    ADD_WORD(len, (WORD)(AvailMMData & 0x0000FFFF));
    ADD_WORD(len, (WORD)(AniDataSize >> 16));
    ADD_WORD(len, (WORD)(AniDataSize & 0x0000FFFF));
    ADD_WORD(len, (WORD)(MelDataSize >> 16));
    ADD_WORD(len, (WORD)(MelDataSize & 0x0000FFFF));

    ADD_BYTE(len, SPEC_AniWidth);			// AniWidth
    ADD_BYTE(len, SPEC_AniHeight);			// AniHeight
    ADD_WORD(len, SPEC_AniLength);			// AniLength
    ADD_WORD(len, (WORD)(SPEC_AniMaxLength >> 16));
    ADD_WORD(len, (WORD)(SPEC_AniMaxLength & 0x0000FFFF));

    ADD_BYTE(len, SPEC_MelType);
    ADD_WORD(len, (WORD)(SPEC_MelodyMexLength >> 16));
    ADD_WORD(len, (WORD)(SPEC_MelodyMexLength & 0x0000FFFF));

    ADD_BYTE(len, SPEC_MaxAlarmSize);		// MaxAlarmSize
    ADD_BYTE(len, SPEC_AlarmNameLen);		// AlarmNameLen
    ADD_BYTE(len, SPEC_MaxMemoSize);		// MaxMemoSize
    ADD_BYTE(len, SPEC_MemoNameLen);		// MemoNameLen

    ADD_BYTE(len, SPEC_MaxScheduleSize);
    ADD_BYTE(len, SPEC_ScheduleNameLen);
    ADD_BYTE(len, SPEC_MaxHolidaySize);
    ADD_BYTE(len, SPEC_HolidayNameLen);

    PBookFilesOpen();
    // Send message
    DmSendMessage(DM_PHONE_SPEC_DATA, len);
    return 0;
}

UINT8	DmSendEndDM(VOID)
{
    DmSendMessage(DM_SEND_END, 0);
    return 0;
}

UINT8	DmReceiveSetEndDM(VOID)
{
    ui_cmd_type	    *cmd_ptr;	// Pointer to UI command
    
    if(!SKY_GET_AIRDATAMANAGER_ORG() && !SKY_GET_AIRDATAMANAGER_RCV() && \
        !SKY_GET_IP_AIRDATAMANAGER())
    {
        if ((cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q )) != NULL) {
	        cmd_ptr->hdr.cmd        = UI_DM_OUT_PROCESSING_F;
	        cmd_ptr->hdr.task_ptr   = NULL;
	        cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
	        ui_cmd( cmd_ptr );
        }
        else {
	        //DebugOut("\tCmd alloc fail");			// Send message
	        DmSendNg();
	        return 1;
        }
    }
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetUsedBook(VOID)
**
** Description: 
**     Send UsedBookList
**
** Input:
**     
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

UINT8	DmReceiveGetUsedBook(VOID)	// Send UsedBookList
{
	UINT16			UsedSize;
	INT				len;
	INT				i;

	// Clear send buffer
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	UsedSize = 0;
	len = 2;

	// Make used list
	for (i = 1; i <= SPEC_BookSize; i++) {
		if (IS_VALID_BOOK(i) > 0) {
			UsedSize++;
			ADD_WORD(len, i);			// BookIndex
		}
	}

    dmSendBuffer[0] = UsedSize >> 8;
	dmSendBuffer[1] = UsedSize & 0xFF;

	// Send message
	DmSendMessage(DM_USED_BOOK_LIST, len);
	return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetBook(VOID)
**
** Description: 
**     Send BookData
**     
** Input:
**     
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

UINT8	DmReceiveGetBook(VOID)	// Send BookData
{
    pb_book_info_type   BookInfo;
    pb_anniv_info_type  AnnivInfo;
//    pb_email_info_type   EMailInfo;
	INT16               BookIndex;
	INT                 pos;
	INT                 len;
    BYTE                count;

	// Check index range
	pos = 0;
	GET_WORD(pos, BookIndex);
	if (BookIndex <= 0 || BookIndex > SPEC_BookSize) {
		DmSendNg();							// Out of range
		return 0;
	}
    
	// Clear send buffer
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	len = 0;
	ADD_WORD(len, BookIndex);									// BookIndex

	// Unused item
	if (IS_VALID_BOOK(BookIndex) == 0) {
		// Send message
		DmSendMessage(DM_BOOK_DATA, len);
		return 0;
	}

	// Read dial 
	BookInfo.address = BookIndex;		// Range is 1..300
    if(!ReadBook(&BookInfo))
    {
        DmSendNg();		// read error
        return 0;
    }

    if( BookInfo.multi_group != 0 )
    {
        ADD_BYTE(len, 2);
        ADD_BYTE(len, BookInfo.group_address);  
        ADD_BYTE(len, BookInfo.multi_group);
    }else
    {    
        ADD_BYTE(len, 1);           //전화부 데이타는 최소한 하나의 그룹에 속함
        ADD_BYTE(len, BookInfo.group_address);  
        ADD_BYTE(len, BookInfo.group_address);  
    }

	if (STRCMP((CHAR *)BookInfo.name, "")) {
		ADD_STR(len, SPEC_BookNameLen, BookInfo.name); 
	}
	else {
		ADD_STR(len, SPEC_BookNameLen, "(이름없음)");     // Name
	}

//EQUS에서는 NUM Data 따로 간다. 
/*
	strncpy((CHAR *)&dmSendBuffer[len],
		(CHAR *)nv_dial.multi_digits[1], nv_dial.multi_num_digits[1]);	// HomeNum	
	len += SPEC_HomeNumLen;
	strncpy((CHAR *)&dmSendBuffer[len],
		(CHAR *)nv_dial.multi_digits[2], nv_dial.multi_num_digits[2]);	// OfficeNum
	len += SPEC_OfficeNumLen;
	strncpy((CHAR *)&dmSendBuffer[len],
		(CHAR *)nv_dial.multi_digits[0], nv_dial.multi_num_digits[0]);	// MobileNum
	len += SPEC_MobileNumLen;
	strncpy((CHAR *)&dmSendBuffer[len], 
		(CHAR *)nv_dial.multi_digits[3], nv_dial.multi_num_digits[3]);	// PagerNum
	len += SPEC_PagerNumLen;
	// Skip FaxNum	
	// Skip HomeAddr
	// Skip OfficeAddr
	// Skip Email
*/

    //redstars-memo
	ADD_STR(len, SPEC_MemoLen ,	BookInfo.szText);					// Memo
    AnnivInfo.address = BookInfo.anniv_address;
    count = 0;
    while( ReadAnniv(&AnnivInfo) >  0)            // Anniv Count
    {
        count++;
        AnnivInfo.address = AnnivInfo.next_index;
    }
    ADD_BYTE(len, count);
    
    AnnivInfo.address = BookInfo.anniv_address;
    while(ReadAnniv(&AnnivInfo) >0 )
    {
        ADD_WORD(len, AnnivInfo.address);
        ADD_BYTE(len, AnnivInfo.type);
        ADD_STR(len, SPEC_AnnivLen, AnnivInfo.content);
        ADD_BYTE(len, AnnivInfo.month);
        ADD_BYTE(len, AnnivInfo.day);
        ADD_BYTE(len, AnnivInfo.fLunar);
        ADD_WORD(len, AnnivInfo.year);

        AnnivInfo.address = AnnivInfo.next_index;
    }
    
    
    for(;count < PERSONAL_ANNIV_LIMIT;count++)
    {
        len += 2 + 1 + SPEC_AnnivLen + 5;
    }

/*      //2001/03/23 Memo Data 따로 
    EMailInfo.address = BookInfo.email_address;
    count = 0;
    while(ReadEmail(&EMailInfo) > 0 )
    {
        count++;
        EMailInfo.address = EMailInfo.next_index;
    }   
    ADD_BYTE(len, count);
    
    EMailInfo.address = BookInfo.email_address;
    while(ReadEmail(&EMailInfo) >0 )
    {
        ADD_WORD(len, EMailInfo.address);
        ADD_STR(len, SPEC_MemoLen,EMailInfo.szMailID);
        EMailInfo.address = EMailInfo.next_index;
    }
    
    for(;count < PERSONAL_MEMO_LIMIT;count++)
    {
        len += 2 + SPEC_MemoLen;
    }
*/
    // Send message
	DmSendMessage(DM_BOOK_DATA, len);
	return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetBook(VOID)
**
** Description: 
**     
**
** Input:
**     
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

UINT8	DmReceiveSetBook(VOID)
{
    pb_book_info_type   BookInfo;
    pb_anniv_info_type  AnnivInfo;
//    pb_email_info_type   EMailInfo;
    pb_group_info_type  GroupInfo;
	INT16               BookIndex;
	BYTE                group1, group2;
	INT                 pos;
    BYTE                count;
    BYTE                i;
    BYTE                Anniv;
    BYTE                idx = 0;
	
	// Check index range
	pos = 0;
    
    for(idx = 0; idx < OK_MESSAGE_INDEX ; idx++)
        OkMessageIndex[idx] = 0;

    idx = 0;
	GET_WORD(pos, BookIndex);
    if (BookIndex < 0 || BookIndex > SPEC_BookSize)
    {
		DmSendNg();		// Out of range
		return 0;
	}
    GET_BYTE(pos, count);
    GET_BYTE(pos, group1);
    GET_BYTE(pos, group2);

	// Make dial data
	if(BookIndex == 0 )                     //데이터 생성
    {
        memset((VOID *)&BookInfo, 0, sizeof(BookInfo));
    	BookInfo.address = BookIndex;	// Range is 1..300
        BookInfo.group_address = group1;
        if(count == 2)
        {
            BookInfo.multi_group = group2;
        }else
        {
            BookInfo.multi_group = 0;
        }
	    GET_STR(pos, SPEC_BookNameLen, BookInfo.name);				// Name
		//redstars-memo
        GET_STR(pos, SPEC_MemoLen,	BookInfo.szText);					// memo

        if (!STRNCMP((CHAR *)BookInfo.name, "                ", SPEC_BookNameLen))
        {
		    STRCPY((CHAR *)BookInfo.name, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NO_NAME));
	    }
        GroupInfo.address = group1;
        BookInfo.address = AddBook(&GroupInfo, &BookInfo);
        if(BookInfo.address <= 0)
        {
		    DmSendNg();
		    return 1;         
        }
        ui_put_pb( PB_BOOK_INFO_I,  &BookInfo);
        OkMessageIndex[idx++] = BookInfo.address;

    }else   //if(BookInfo.address != 0)       //데이터 수정 -> 기존 데이타를 가져온다.
    {
        BookInfo.address = BookIndex;
        ReadBook(&BookInfo);
        BookInfo.sync_flag  = 'M';
        BookInfo.group_address = group1;
        if(count == 2)
        {
            BookInfo.multi_group = group2;
        }else
        {
            BookInfo.multi_group = 0;
        }
	    GET_STR(pos, SPEC_BookNameLen, BookInfo.name);				// Name
		//redstars-memo
        GET_STR(pos, SPEC_MemoLen,	BookInfo.szText);					// EMail

	    if (!STRNCMP((CHAR *)BookInfo.name, "                ", SPEC_BookNameLen))
        {
		    STRCPY((CHAR *)BookInfo.name, (BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NO_NAME));
	    }
        if(!WriteBook(&BookInfo))
        {
		    DmSendNg();
		    return 1;    
        }
        OkMessageIndex[idx++] = BookInfo.address;
    }
    
    STRCPY(g_szPersonName[BookInfo.address-1],BookInfo.name);
    g_PersonGroup[BookInfo.address-1] = BookInfo.group_address;
    g_PersonMultiGroup[BookInfo.address-1] = BookInfo.multi_group;
    
    //DmGetDial(&nv_dial);

/*
    // Setting for WAP PIMS
    if (IS_VALID_BOOK(BookIndex + 1) == 0 )                 //!= 1)
    { // 만약 새로운 데이타이면 flag를 create로 한다.
        BookInfo.sync_flag = 'C';
    }
    else
    { // 기존에 데이타가 있는 경우 sync된 경우만 modified로 변경한다.
        if (BookInfo.sync_flag  == 'S' || BookInfo.sync_flag == 's')
            
    }
*/    
    //nv_dial.exist = 1;					                            // Unused item

    // Change Number Order
/*	GET_STR(pos, SPEC_HomeNumLen, nv_dial.multi_digits[1]);			// HomeNum
	DmRemoveTailSpace((CHAR *)nv_dial.multi_digits[1], SPEC_HomeNumLen);
	nv_dial.multi_num_digits[1] = strlen((CHAR *)nv_dial.multi_digits[1]);
	GET_STR(pos, SPEC_OfficeNumLen, nv_dial.multi_digits[2]);		// OfficeNum
	DmRemoveTailSpace((CHAR *)nv_dial.multi_digits[2], SPEC_OfficeNumLen);
	nv_dial.multi_num_digits[2] = strlen((CHAR *)nv_dial.multi_digits[2]);
	GET_STR(pos, SPEC_MobileNumLen, nv_dial.multi_digits[0]);		// MobileNum
	DmRemoveTailSpace((CHAR *)nv_dial.multi_digits[0], SPEC_MobileNumLen);
	nv_dial.multi_num_digits[0] = strlen((CHAR *)nv_dial.multi_digits[0]);
	GET_STR(pos, SPEC_PagerNumLen, nv_dial.multi_digits[3]);		// PagerNum
	DmRemoveTailSpace((CHAR *)nv_dial.multi_digits[3], SPEC_PagerNumLen);
	nv_dial.multi_num_digits[3] = strlen((CHAR *)nv_dial.multi_digits[3]);
	// Skip FaxNum	
	// Skip HomeAddr
	// Skip OfficeAddr
*/
    // Skip Url

    count = 0;
    GET_BYTE(pos, count);                               // Anniv Count
    for(i = 0 ; i <count ; i++)
    {
        GET_WORD(pos, AnnivInfo.address);
        GET_BYTE(pos, Anniv);
        if(AnnivInfo.address != 0)          
        {
            if(Anniv == 0)         //기념일 삭제
            {
                DeleteAnniv(&BookInfo, AnnivInfo.address);
                OkMessageIndex[idx++] = 0x0000;
            }else                           
            {                       //기념일 수정
                ReadAnniv(&AnnivInfo);
                AnnivInfo.type = Anniv;
                GET_STR(pos, SPEC_AnnivLen, AnnivInfo.content);
                GET_BYTE(pos, AnnivInfo.month);
                GET_BYTE(pos, AnnivInfo.day);
                GET_BYTE(pos, AnnivInfo.fLunar);
                GET_WORD(pos, AnnivInfo.year);
                AnnivInfo.book_address = BookInfo.address;
                WriteAnniv(&AnnivInfo);
                OkMessageIndex[idx++] = AnnivInfo.address;
            }

        }else                               
        {                               //기념일 생성
            GET_STR(pos, SPEC_AnnivLen, AnnivInfo.content);
            GET_BYTE(pos, AnnivInfo.month);
            GET_BYTE(pos, AnnivInfo.day);
            GET_BYTE(pos, AnnivInfo.fLunar);
            GET_WORD(pos, AnnivInfo.year);
            AnnivInfo.type = Anniv;
            AddAnnivInfo(&BookInfo, &AnnivInfo);
            OkMessageIndex[idx++] = AnnivInfo.address;
            WriteAnniv(&AnnivInfo);
            WriteBook(&BookInfo);
        }
    }

/*
    for( ; count < PERSONAL_ANNIV_LIMIT ; count++);
    {
        pos += 2 + 1 + SPEC_AnnivLen + 5;
    }
*/
/*
    count = 0;
    GET_BYTE(pos, count);
    for(i = 0; i <PERSONAL_MEMO_LIMIT ; i++)
    {
        GET_WORD(pos, EMailInfo.address);
        
        if(EMailInfo.address != 0)           //메모 수정
        {
            ReadEmail(&EMailInfo);
            GET_STR(pos, SPEC_MemoLen, EMailInfo.szMailID);
        }else               
        {
            GET_STR(pos, SPEC_MemoLen, EMailInfo.szMailID);
            if(!STRCMP((CHAR *)BookInfo.name, ""))//메모 삭제
                DeleteEmail(&BookInfo, EMailInfo.address);
            else            //메모 생성
            {
                EMailInfo.address = AddEmail(&BookInfo, EMailInfo.szMailID);
                ReadEmail(&EMailInfo);
                OkMessageIndex[idx++] = EMailInfo.address;
            }
        }
        GET_STR(pos, SPEC_MemoLen, EMailInfo.szMailID);
        
        EMailInfo.book_address = BookInfo.address;
        if(STRCMP((CHAR *)EMailInfo.szMailID,""))
            WriteEmail(&EMailInfo);
    }
*/    
/*
    for( ; count < PERSONAL_MEMO_LIMIT ; count++);
    {
        pos += 2 + SPEC_MemoLen;
    }
*/    
	// Send message

    DmSendOk();
    PhoneBookUpdate();
	return 0;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveDelBook(VOID)
**  
** Description: 
**     
**
** Input:
**     
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

UINT8	DmReceiveDelBook(VOID)
{
    pb_book_info_type   BookInfo;
    INT                 BookIndex;
	INT                 pos;

    // Check index range
    pos = 0;
    GET_WORD(pos, BookIndex);
    if (BookIndex <= 0 || BookIndex > SPEC_BookSize) 
    {
        DmSendNg();		// Out of range
        return 0;
    }

    if (IS_VALID_BOOK(BookIndex) == 1) 
    {	
        // Make dial data
        memset((VOID *)&BookInfo, 0, sizeof(BookInfo));
        BookInfo.address = BookIndex ;	// Range is 1..300
        BookInfo.sync_flag = 'M';
        DeleteBookInfo(BookInfo.address);
    }

    DmSendOk();
    PhoneBookUpdate();
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetBookEmail(VOID)	// Send BookEmailData
**
** Description: 
**     Send BookEmailData
**
** Input:
**     
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
UINT8   DmReceiveGetBookEmail(VOID)
{
    pb_book_info_type   BookInfo;
    pb_email_info_type   EMailInfo;
    INT                 count, pos,len, BookIndex;

    pos = 0;
    len = 0;
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    GET_WORD(pos, BookIndex);
    if (BookIndex <= 0 || BookIndex > SPEC_BookSize) {
        DmSendNg();							// Out of range
        return 0;
    }

    BookInfo.address = BookIndex;
    if(!ReadBook(&BookInfo))
    {
        DmSendNg();   // read error
        return 0;
    }

    EMailInfo.address = BookInfo.email_address;
    count = 0;
    while(ReadEmail(&EMailInfo)>0)
    {
        count++;
        EMailInfo.address = EMailInfo.next_index;
    } 
    ADD_WORD(len, BookIndex);
    ADD_BYTE(len, count);
    
    EMailInfo.address = BookInfo.email_address;
    while(ReadEmail(&EMailInfo)>0)
    {
        ADD_WORD(len, EMailInfo.address);
        ADD_STR(len, SPEC_EmailLen, EMailInfo.szMailID);
        EMailInfo.address = EMailInfo.next_index;
    }

/*    
    for(;count < PERSONAL_MEMO_LIMIT;count++)
    {
        len += 2 + SPEC_MemoLen;
    }
*/
    DmSendMessage(DM_BOOK_EMAIL_DATA, len);
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetBookEmail(VOID)	// Set BookMemoData
**
** Description: 
**     Set BookMemoData
**
** Input:
**     
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
UINT8   DmReceiveSetBookEmail(VOID)
{
    pb_book_info_type   BookInfo;
    pb_email_info_type   EMailInfo;
    INT                 count, pos, idx, i, BookIndex;

	pos = 0;
    idx = 0;

    for(idx = 0; idx < OK_MESSAGE_INDEX ; idx++)
        OkMessageIndex[idx] = 0;

    idx = 0;
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	GET_WORD(pos, BookIndex);
	if (BookIndex < 0 || BookIndex > SPEC_BookSize)
    {
		DmSendNg();		// Out of range
		return 0;
	}

    BookInfo.address = BookIndex;
    if(!ReadBook(&BookInfo))
    {
        DmSendNg();   // read error
        return 0;
    }

    count = 0;
    GET_BYTE(pos, count);
    for(i = 0; i <count ; i++)
    {
        GET_WORD(pos, EMailInfo.address);
        if(EMailInfo.address != 0)
            ReadEmail(&EMailInfo);            
                
        if(EMailInfo.address != 0)           
        {
            GET_STR(pos, SPEC_EmailLen, EMailInfo.szMailID);
            if(!STRCMP((CHAR *)EMailInfo.szMailID, ""))//메모 삭제
            {
                DeleteEmail(&BookInfo, EMailInfo.address);
                OkMessageIndex[idx++] = 0x0000;
            }else
            {
                EMailInfo.book_address = BookInfo.address;
                WriteEmail(&EMailInfo);
                OkMessageIndex[idx++] = EMailInfo.address;
            }
        }else               
        {   
            GET_STR(pos, SPEC_EmailLen, EMailInfo.szMailID);          //메모 생성
            EMailInfo.address = AddEmail(&BookInfo, EMailInfo.szMailID);
            ReadEmail(&EMailInfo);
            OkMessageIndex[idx++] = EMailInfo.address;
            //EMailInfo.book_address = BookInfo.address;
            //WriteEmail(&EMailInfo);
            ui_put_pb(PB_BOOK_INFO_I, &BookInfo);
        }
    }
    DmSendOk();
    PhoneBookUpdate();
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveDelBookEmail(VOID)	// Del BookMemo
**
** Description: 
**     Del BookEmail
**
** Input:
**     
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
UINT8   DmReceiveDelBookEmail(VOID)
{
    pb_book_info_type   BookInfo;
    pb_email_info_type   EMailInfo;
    INT                 pos, BookIndex;

	pos = 0;
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    GET_WORD(pos, BookIndex);
    if (BookIndex <= 0 || BookIndex > SPEC_BookSize)
    {
        DmSendNg();		// Out of range
        return 0;
    }

    BookInfo.address = BookIndex;
    if(!ReadBook(&BookInfo))
    {
        DmSendNg();   // read error
        return 0;
    }

    EMailInfo.address = BookInfo.email_address;
    while(ReadEmail(&EMailInfo) > 0 )
    {
        DeleteEmail(&BookInfo, EMailInfo.address);
        EMailInfo.address = EMailInfo.next_index;
    }   
    DmSendOk();
    PhoneBookUpdate();
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetNum(VOID)	// Send NumData
**
** Description: 
**     Send BookData
**
** Input:
**     
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
UINT8	DmReceiveGetNum(VOID)	// Send NumData
{
    pb_book_info_type       BookInfo;
    pb_number_info_type     NumberInfo;
	INT16                   BookIndex;
	INT                     pos;
	INT                     len;
    BYTE                    count;
    
	// Check index range
	pos = 0;
	GET_WORD(pos, BookIndex);
	if (BookIndex <= 0 || BookIndex > SPEC_BookSize) {
		DmSendNg();							// Out of range
		return 0;
	}

	// Clear send buffer
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	len = 0;
	ADD_WORD(len, BookIndex);							

    if (IS_VALID_BOOK(BookIndex) == 0) {
    // Send message
        DmSendMessage(DM_NUM_DATA, len);
        return 0;
    }

    BookInfo.address = BookIndex;
    if(!ReadBook(&BookInfo))
    {
        DmSendNg();   // read error
        return 0;
    }
    
    NumberInfo.address = BookInfo.number_address;
    count = 0;
    while( ReadNumber(&NumberInfo) > 0 )
    {
        count++;
        NumberInfo.address = NumberInfo.next_index;
    }
    ADD_BYTE(len, count);

    NumberInfo.address = BookInfo.number_address;
    while(ReadNumber(&NumberInfo) > 0 )
    {
        ADD_WORD(len, NumberInfo.address);
        ADD_STR(len, SPEC_NumLen , NumberInfo.digits);
        ADD_BYTE(len, NumberInfo.number_type);
        NumberInfo.address = NumberInfo.next_index;
    }
    for(;count < PERSONAL_NUMBER_LIMIT ; count++)
    {
        len+=2 + SPEC_NumLen + 1;
    }

	// Send message
	DmSendMessage(DM_NUM_DATA, len);
	return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetNum(VOID)
**
** Description: 
**     Send BookData
**
** Input:
**     
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

UINT8	DmReceiveSetNum(VOID)
{
    pb_book_info_type       BookInfo;
    pb_number_info_type     NumberInfo;
    INT16                   BookIndex;
    INT                     pos;
    BYTE                    count;
    BYTE                    i,j;
    BYTE                    idx = 0;
	
    // Check index range
    pos = 0;
    GET_WORD(pos, BookIndex);
    if (BookIndex <= 0 || BookIndex > SPEC_BookSize)
    {
        DmSendNg();		// Out of range
        return 0;
    }

    for(idx = 0; idx < OK_MESSAGE_INDEX ; idx++)
        OkMessageIndex[idx]  = 0;

    // Make dial data
    memset((VOID *)&BookInfo, 0, sizeof(BookInfo));
    BookInfo.address = BookIndex;	// Range is 1..300 not 0..299
    
    idx = 0;
    //기존의 데이타를 가져온다.
    ReadBook(&BookInfo);
    count = 0;
    GET_BYTE(pos, count);
    j = 0;
    for(i = 0; i <count ; i++)
    {
        GET_WORD(pos, NumberInfo.address);
        if(NumberInfo.address != 0)     
        {
            ReadNumber(&NumberInfo);
            GET_STR(pos, SPEC_NumLen, NumberInfo.digits);
            GET_BYTE(pos, NumberInfo.number_type);
            if(NumberInfo.number_type == 0)    //데이터 삭제 
            {
                DeleteNumber(&BookInfo, NumberInfo.address);
                OkMessageIndex[idx++] = 0x0000;
            }else                               //데이터 수정 
            {
                WriteNumber(&NumberInfo);
                OkMessageIndex[idx++] = NumberInfo.address;
                g_PersonNumberType[BookInfo.address - 1][j++] = NumberInfo.number_type;
            }
        }else                           
        {                                       //데이터 생성
            GET_STR(pos, SPEC_NumLen, NumberInfo.digits);
            GET_BYTE(pos, NumberInfo.number_type);
            NumberInfo.address = ReverseAddNumber(&BookInfo,NumberInfo.digits, NumberInfo.number_type);
            ReadNumber(&NumberInfo);
            j++;
            OkMessageIndex[idx++] = BookInfo.number_address;
        }
        WriteBook(&BookInfo);
    } 

    for(; j < PERSONAL_NUMBER_LIMIT; j++)
    {
        g_PersonNumberType[BookInfo.address - 1][j] = BOOK_NOT_USE;
    }

    // Send message
    DmSendOk();
    PhoneBookUpdate();
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveDelNum(VOID)
**  
** Description: 
**     
**
** Input:
**     
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

UINT8	DmReceiveDelNum(VOID)
{
    pb_book_info_type   BookInfo;
    INT                 BookIndex;
    INT16               NumberIndex;
    INT                 pos;

    // Check index range
    pos = 0;
    GET_WORD(pos, BookIndex);
    if (BookIndex <= 0 || BookIndex > SPEC_BookSize) 
    {
        DmSendNg();   // Out of range
        return 0;
    }
    
    BookInfo.address = BookIndex;
    while(ReadBook(&BookInfo) > 0)
    {
        NumberIndex = BookInfo.number_address;
        DeleteNumber(&BookInfo, NumberIndex);
        WriteBook(&BookInfo);
    }

    DmSendOk();
    PhoneBookUpdate();
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8   DmReceiveGetUsedGroup(VOID)
**
** Description: 
**     Send GroupData
**
** Input:
**     
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
UINT8   DmReceiveGetUsedGroup(VOID)
{
	INT			UsedSize;
	INT				len;
	INT				i;

	// Clear send buffer
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	UsedSize = 0;
	len = 1;

	// Make used list
	for (i = 1; i <= SPEC_GroupSize; i++) {
		if (IS_VALID_GROUP(i) > 0) {
			UsedSize++;
			ADD_BYTE(len, i);			// BookIndex
		}
	}

    dmSendBuffer[0] = UsedSize;// >> 8;
	//dmSendBuffer[1] = UsedSize & 0xFF;

	// Send message
	DmSendMessage(DM_USED_GROUP_LIST, len);
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetGroup(VOID)
**
** Description: 
**     Send GroupData
**
** Input:
**     
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
UINT8	DmReceiveGetGroup(VOID)	// Send GroupData
{
    pb_group_info_type  GroupInfo;
    INT                 len, pos;
    BYTE                GroupIndex;

    pos = 0;
    GET_BYTE(pos, GroupIndex);
    // Clear send buffer
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    len = 0;

    // Make group data
    ADD_BYTE(len, GroupIndex );
    if (IS_VALID_GROUP(GroupIndex) > 0)
    {   
        // Read group from NV
        GroupInfo.address = GroupIndex;
        if (!ReadGroup(&GroupInfo)) 
        {
            DmSendNg();				// NV read error
            return 0;
        }
        // Make data
        ADD_STR(len, SPEC_GroupNameLen, GroupInfo.group_name);// Name
        ADD_BYTE(len, (GroupInfo.status == SECRET_GROUP) ? 1 : 0);// Hidden

        ADD_WORD(len, GroupInfo.Bell.wBellID);  // GroupBell ID Setting
        if (SKY_IS_MY_BELL(GroupInfo.Bell.wBellID))           // User Meoldy
        {
            STRNCPY((CHAR *)&dmSendBuffer[len], 
                (CHAR *)GroupInfo.Bell.szFileName, SKY_FILE_EXT_NAME_MAX_LENGTH);	// PagerNum
            len += 20;
        }
    }
    else 
    {                                                           // No data contains
        DmSendNg();				// NV read error
        return 0;
    }
	// Send message
    DmSendMessage(DM_GROUP_DATA, len);
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetGroup(VOID)
**
** Description: 
**     
**
** Input:
**     
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

UINT8	DmReceiveSetGroup(VOID)
{
    pb_group_info_type  GroupInfo;
    INT                 pos;
    INT                 index, idx = 0, MelodyId;
    UINT8               hidden;
    CHAR                szFileName[SPEC_MaxMMFileNameLen];
    BYTE                group_name[MAX_GROUP_NAME+1];
    
    // Reset read pos
    pos = 0;

    for(idx = 0; idx < OK_MESSAGE_INDEX ; idx++)
        OkMessageIndex[idx]  =0;

    idx = 0;

    // Make group data
    for (index = 1; index <= SPEC_GroupSize; index++) 
    {
        //GroupInfo.address = index;//이상하다. 왜 예전엔 GET_BYTE를 안해왔지?
        memset((VOID *)&GroupInfo, 0, sizeof(GroupInfo));
        GET_BYTE(pos, index);										// Index
        GroupInfo.address = index;
        GET_STR(pos, SPEC_GroupNameLen, group_name);      // Name
        GET_BYTE(pos, hidden);                                      // Hidden

        if(GroupInfo.address != 0)                          
        {
            if(hidden == 2)//그룹 정보 삭제
            {
                DeleteGroup(GroupInfo.address, TRUE);
                OkMessageIndex[idx++] = 0x0000;
            }else
            {               //그룹 정보 수정
                ReadGroup(&GroupInfo);
                hidden = (hidden == 1) ? SECRET_GROUP : NON_SECRET_GROUP;
                STRCPY(GroupInfo.group_name, group_name);
                GroupInfo.status = hidden;
                GET_WORD(pos, MelodyId);								// Clip1: GroupBell
            
                if(MelodyId == 0xfffe)
                {
                    GroupInfo.Bell.wBellID = INIT_BELL_ID;
                }else
                    GroupInfo.Bell.wBellID = MelodyId;
                if (SKY_IS_MY_BELL(MelodyId))
                {                                                           // User Meoldy
                    GET_STR(pos, 20, szFileName);
                    STRNCPY((BYTE*)GroupInfo.Bell.szFileName,szFileName, SKY_FILE_EXT_NAME_MAX_LENGTH);
                }
                WriteGroup(&GroupInfo);
                OkMessageIndex[idx++] = GroupInfo.address;
                STRCPY(g_szGroupName[GroupInfo.address - 1],GroupInfo.group_name);
            }
        }else
        {  
            hidden = (hidden == 1) ? SECRET_GROUP : NON_SECRET_GROUP;//그룹 정보 생성
            STRCPY(GroupInfo.group_name, group_name);
            GroupInfo.address = AddGroup(GroupInfo.group_name, hidden);
            ReadGroup(&GroupInfo);
            GET_WORD(pos, MelodyId);								    // Clip1: GroupBell
        
            if(MelodyId == 0xfffe)
            {
                GroupInfo.Bell.wBellID = INIT_BELL_ID;
            }else
                GroupInfo.Bell.wBellID = MelodyId;

            if (SKY_IS_MY_BELL(MelodyId))
            {                                                           // User Meoldy
                GET_STR(pos, 20, szFileName);
                STRNCPY((BYTE*)GroupInfo.Bell.szFileName,szFileName, SKY_FILE_EXT_NAME_MAX_LENGTH);
            }
            WriteGroup(&GroupInfo);
            OkMessageIndex[idx++] = GroupInfo.address;
            STRCPY(g_szGroupName[GroupInfo.address - 1],GroupInfo.group_name);
        }
    }
    // Send message
    DmSendOk();
    PhoneBookUpdate();
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetGroup2(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveSetGroup2(VOID)
{
    pb_group_info_type  GroupInfo;
    INT                 pos;
    INT                 index, idx;
    UINT8				hidden;
    INT                 MelodyId;
    CHAR                szFileName[20];
    BYTE                group_name[MAX_GROUP_NAME+1];

    // Reset read pos
    pos = 0;
    idx = 0;
    // Make group data

    memset((VOID *)&GroupInfo, 0, sizeof(GroupInfo));
    for(idx = 0; idx < OK_MESSAGE_INDEX ; idx++)
        OkMessageIndex[idx]  =0;

    idx = 0;
    GET_BYTE(pos, index);										// Index
    GroupInfo.address = index;
    GET_STR(pos, SPEC_GroupNameLen, group_name);      // Name
    GET_BYTE(pos, hidden);                                      // Hidden

    if(GroupInfo.address != 0)                          
    {
        if(hidden == 2)//그룹 정보 삭제
        {
            DeleteGroup(GroupInfo.address, TRUE);
            OkMessageIndex[idx++] = 0x0000;
        }else
        {               //그룹 정보 수정
            ReadGroup(&GroupInfo);
            hidden = (hidden == 1) ? SECRET_GROUP : NON_SECRET_GROUP;
            STRCPY(GroupInfo.group_name, group_name);
            GroupInfo.status = hidden;
            GET_WORD(pos, MelodyId);								// Clip1: GroupBell
            
            if(MelodyId == 0xfffe)
            {
                GroupInfo.Bell.wBellID = INIT_BELL_ID;
            }else
                GroupInfo.Bell.wBellID = MelodyId;
            if (SKY_IS_MY_BELL(MelodyId))
            {                                                           // User Meoldy
                GET_STR(pos, 20, szFileName);
                STRNCPY((BYTE*)GroupInfo.Bell.szFileName,szFileName, SKY_FILE_EXT_NAME_MAX_LENGTH);
            }
            WriteGroup(&GroupInfo);
            OkMessageIndex[idx++] = GroupInfo.address;
            STRCPY(g_szGroupName[GroupInfo.address - 1],GroupInfo.group_name);
        }
    }else
    {  
        hidden = (hidden == 1) ? SECRET_GROUP : NON_SECRET_GROUP;//그룹 정보 생성
        STRCPY(GroupInfo.group_name, group_name);
        GroupInfo.address = AddGroup(GroupInfo.group_name, hidden);
        ReadGroup(&GroupInfo);
        GET_WORD(pos, MelodyId);								    // Clip1: GroupBell
        
        if(MelodyId == 0xfffe)
        {
            GroupInfo.Bell.wBellID = INIT_BELL_ID;
        }else
            GroupInfo.Bell.wBellID = MelodyId;

        if (SKY_IS_MY_BELL(MelodyId))
        {                                                           // User Meoldy
            GET_STR(pos, 20, szFileName);
            STRNCPY((BYTE*)GroupInfo.Bell.szFileName,szFileName, SKY_FILE_EXT_NAME_MAX_LENGTH);
        }
        WriteGroup(&GroupInfo);
        OkMessageIndex[idx++] = GroupInfo.address;
        STRCPY(g_szGroupName[GroupInfo.address - 1],GroupInfo.group_name);
    }
    // Send message
    DmSendOk();
    PhoneBookUpdate();
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetSpeedDial(VOID)
**
** Description: 
**     Send SpeedDialData
**
** Input:
**     
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

UINT8	DmReceiveGetSpeedDial(VOID)	// Send SpeedDialData
{
    pb_sdial_type       Sdial;
    INT                 UsedSDialSize;
    INT                 len;
    INT                 SpeedIndex;
    
    // Clear send buffer
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    UsedSDialSize = 0;
    len = 1;

	// Make used list
    for (SpeedIndex = 1; SpeedIndex <= SPEC_SDialSize; SpeedIndex++) 
    {
        if(IS_VALID_SDIAL(SpeedIndex)> 0) 
        {
            // Read NV
            Sdial.address = SpeedIndex;
            {
                if (ReadSDial(&Sdial)) 
                {
                    UsedSDialSize++;
                    ADD_BYTE(len, SpeedIndex);                  // SpeedIndex
                    ADD_WORD(len, Sdial.dial_address);         // BookIndex
                }
            }
        }
    }
    dmSendBuffer[0] = UsedSDialSize;
    // Send message
    DmSendMessage(DM_SPEED_DIAL_DATA, len);
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetSpeedDial(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveSetSpeedDial(VOID)
{
    pb_sdial_type       Sdial;
    INT                 UsedSDialSize;
    INT                 pos;
    WORD                wNumIndex;
    INT                 SpeedIndex;
    INT                 i;

    // Read used size
    pos = 0;
    GET_BYTE(pos, UsedSDialSize);

    // Get speed dial list
    for (i = 1; i <= UsedSDialSize; i++) 
    {
        GET_BYTE(pos, SpeedIndex);            // SpeedIndex
        GET_WORD(pos, wNumIndex);  
        if(SPEC_SDialSize <  SpeedIndex)
        {
            DmSendNg();   // Out of range
            return 0;        
        }else
        {
            Sdial.address = SpeedIndex;
            if(wNumIndex == 0)                 //단축 다이얼 삭제
                DeleteSDial(SpeedIndex);
            else
            {
                ReadSDial(&Sdial);
                Sdial.dial_address = wNumIndex;
                WriteSDial(&Sdial);
            }
        }
    }
    // Send message
    DmSendOk();
    PhoneBookUpdate();
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetSpeedDial2(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveSetSpeedDial2(VOID)
{
    pb_sdial_type       Sdial;
    INT                 pos;
    WORD                wNumIndex;
    INT                 SpeedIndex;

    // Get speed dial list
    pos = 0;
    GET_BYTE(pos, SpeedIndex);				// SpeedIndex
    GET_WORD(pos, wNumIndex);				// BookIndex
    
    if(SPEC_SDialSize <  SpeedIndex)
    {
        DmSendNg();   // Out of range
        return 0;        
    }else
    {
        Sdial.address = SpeedIndex;
        if(wNumIndex == 0)     //단축 다이얼 삭제
            DeleteSDial(SpeedIndex);
        else
        {
            ReadSDial(&Sdial);
            Sdial.dial_address = wNumIndex;
            WriteSDial(&Sdial);
        }
    }
	// Send message
	DmSendOk();
    PhoneBookUpdate();
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetUsedSMS(VOID)
**
** Description: 
**     
**
** Input:
**     
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

UINT8	DmReceiveGetUsedSMS(VOID)
{
    INT	UsedMtSize, UsedMoSize;
    INT	len;

    // Clear send buffer
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    UsedMoSize = 0;
    len = 0;

    //Count used MO
    MakeMOSaveScrollList();                     //Init
    ADD_BYTE(len, g_nMOSaveSize);				// UsedMOIndex

    // Count used MT
    UsedMtSize = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt
                + ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt 
                + ui_sms_list[UI_SMS_TXT_OLD].cnt;
    ADD_BYTE(len, UsedMtSize);				// UsedMTIndex
    // Send message
    DmSendMessage(DM_USED_SMS_LIST, len);
    FreeSMSMOScrollList(g_nMOSaveSize);
    return 0;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetMO(VOID)
**
** Description: 
**     Send MO Data
**
** Input:
**     
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

UINT8	DmReceiveGetMO(VOID)	// Send MO Data
{
    SMSMO_SEND_T    FileMoData;
    INT             MOIndex;
    WORD            wYear;
    BYTE            bYear, bMonth, bDay, bHour, bMin, bSec;
    INT             pos;
    INT             len;
    clk_julian_type julian;

    // Read MO index
    pos = 0;
    GET_BYTE(pos, MOIndex);

    // Clear send buffer
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    len = 0;
    
    MakeMOSaveScrollList();     //Init
    ADD_BYTE(len, MOIndex);
    if(g_aSMSSaveMsg[MOIndex].nAttr & SMS_USED)
    {
        SKY_SeekReadMOSaveT(g_aSMSSaveMsg[MOIndex].nAddr , &FileMoData);
        ADD_BYTE(len, FileMoData.nAttr);
        clk_secs_to_julian(FileMoData.dwMadeTime, &julian);
        
        wYear = julian.year;
        bYear = wYear - 2000;
        bMonth = julian.month;
        bDay = julian.day;
        bHour = julian.hour;
        bMin = julian.minute;
        bSec = julian.second;

        ADD_BYTE(len, bYear);
        ADD_BYTE(len, bMonth);
        ADD_BYTE(len, bDay);
        ADD_BYTE(len, bHour);
        ADD_BYTE(len, bMin);
        ADD_BYTE(len, bSec);
        ADD_STR(len,  FileMoData.nMsgSize, FileMoData.pszMsg);
        len+= (SPEC_SmsTextLen - FileMoData.nMsgSize);
        
    }else
    {
        ADD_BYTE(len, FileMoData.nAttr);
    }

    // Send message
    DmSendMessage(DM_MO_DATA, len);
    FreeSMSMOScrollList(g_nMOSaveSize);
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetMO(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveSetMO(VOID)
{
    //SMSMO_SEND_T    FileMoData;
    WORD            wDate;
    INT           nYear, nMonth, nDay, nHour, nMin, nSec;
    INT             pos;
    clk_julian_type julian;

    memset((VOID*)&g_UIMOBuffer, 0, sizeof(g_UIMOBuffer));
    // Read sms date
    pos = 0;
    GET_BYTE(pos, nYear);
    GET_BYTE(pos, nMonth);
    GET_BYTE(pos, nDay);
    GET_BYTE(pos, nHour);
    GET_BYTE(pos, nMin);
    GET_BYTE(pos, nSec);

    julian.year = nYear + 2000;
    julian.month = nMonth;
    julian.day = nDay;
    julian.hour = nHour;
    julian.minute = nMin;
    julian.second = nSec;

    wDate = clk_julian_to_secs(&julian);
    g_UIMOBuffer.dwMadeTime = wDate;

    // Read sms text
    GET_STR(pos, SPEC_SmsTextLen, g_UIMOBuffer.pszMsg);
    g_UIMOBuffer.nMsgSize = STRLEN(g_UIMOBuffer.pszMsg);

    SaveSMSMOSaveMsg(&g_UIMOBuffer);
    // Send message
    DmSendOk();
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveDelMO(VOID)
**
** Description: 
**     
**
** Input:
**     
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

UINT8	DmReceiveDelMO(VOID)
{
    INT           MOIndex;
    INT             pos;

    // Read MO index
    pos = 0;
    GET_BYTE(pos, MOIndex);

    //Clear send buffer
    MakeMOSaveScrollList();     //Init
    DeleteSMSMOSaveMsg(MOIndex);

    // Send message
    DmSendOk();
    FreeSMSMOScrollList(g_nMOSaveSize);
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     MENU_STATE_T DmNullProc(MENU_STATE_T State, UINT8 KeyIn)
**
** Description: 
**     
**
** Input:
**     
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

VOID  DmNullProc(INT State, UINT8 KeyIn)
{
//	STATIC	ds_cmd_type	ds_cmd_item;				// item to hold cmd to dsmgr
//	extern	ds_sigs_type dsi_requested_sig_mask;
//	extern	INT			skt_MoState;

	// MOACK
/*	if (skt_MoState != 255) {
		if (MoStatus == 3 MO_SUCCESS) {
			ds_cmd_item.cmd_name = DS_ESMS_MO_OK;
		}
		else {
			ds_cmd_item.cmd_name = DS_ESMS_MO_NG;
		}

		ds_cmd_item.parms.srv_opt = skt_MoState;
		ds_cmd(&ds_cmd_item);
		dsi_requested_sig_mask |= DS_CMD_Q_SIG;
		skt_MoState = 255;
	}

*/	//return MENU_EXIT_S;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSendMO(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveSendMO(VOID)
{
    ui_cmd_type	    *cmd_ptr;	// Pointer to UI command
    INT			MOIndex;
    INT		    	pos;

    // No idle mode

/*
    if (ui.call == TRUE || ui.idling != TRUE) {
	    //DebugOut("\tNo idle mode");			// Send message
	    DmSendNg();
	    return 1;
    }
*/   
    MakeMOSaveScrollList();     //Init

    memset((VOID*)&g_UIMOBuffer, 0, sizeof(g_UIMOBuffer));
    // Read sms info
    pos = 0;
    GET_BYTE(pos, MOIndex);

    // Get SMS buffer
    SKY_SeekReadMOSaveT(g_aSMSSaveMsg[MOIndex].nAddr , &g_UIMOBuffer);

    // callback
    GET_STR(pos, SPEC_SmsDialLen, g_UIMOBuffer.pszSendNumber);
    g_UIMOBuffer.nSendNumberSize= STRLEN((CHAR *)g_UIMOBuffer.pszSendNumber);

    // call
    GET_STR(pos, SPEC_SmsDialLen, g_UIMOBuffer.pszRcvNumber );
    g_UIMOBuffer.nRcvNumberSize= STRLEN((CHAR *)g_UIMOBuffer.pszRcvNumber);

    // Send SMS
    if ((cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q )) != NULL) {
	    cmd_ptr->hdr.cmd        = UI_DM_MO_CMD_F;
	    cmd_ptr->hdr.task_ptr   = NULL;
	    cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
	    ui_cmd( cmd_ptr );
    }
    else {
	    //DebugOut("\tCmd alloc fail");			// Send message
	    DmSendNg();
	    return 1;
    }
    FreeSMSMOScrollList(g_nMOSaveSize);
    // Send message
    DmSendOk();
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetMT(VOID)
**
** Description: 
**     
**
** Input:
**     
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

UINT8	DmReceiveGetMT(VOID)
{
    INT             MTIndex, MTListNum;
    INT             pos;
    INT             len;
    INT             i, j;
    BYTE            nv_addr;
    nv_sms_type     nv_sms;
    smsi_msg_type   nv_smsi;

    // Read MO index
    pos = 0;
    GET_BYTE(pos, MTIndex);

    // Clear send buffer
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    len = 0;
    ADD_BYTE(len, MTIndex);

    MTListNum = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt + ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt + ui_sms_list[UI_SMS_TXT_OLD].cnt;

    // Out of range
    if (MTIndex >= MTListNum) { 
	    ADD_BYTE(len, 0);
	    DmSendMessage(DM_MT_DATA, len);
	    return 0;
    }

    // Find location
    if (MTIndex < MTListNum - ui_sms_list[UI_SMS_TXT_OLD].cnt) { // New message

	    if(ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0) {

		    if(ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0) {

			    if(MTIndex >= ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) {
				    
				    i = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - (MTIndex - ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) - 1;
				    nv_addr = ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].nv_idx [i];

					    if (ui_sms_text_list[nv_addr][0] == 2|| ui_sms_text_list[nv_addr][0] == 3) {
					    ADD_BYTE(len, 0x05);	// Attribute new-lock
					    }
					    else {
						    ADD_BYTE(len, 0x01);	// Attribute new
					    }
			    }

			    else {

				    i = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - MTIndex - 1;
				    nv_addr = ui_sms_list [UI_SMS_TXT_NEW_URGENT].nv_idx [i]; 				
					    if (ui_sms_text_list[nv_addr][0] == 2|| ui_sms_text_list[nv_addr][0] == 3) {
					    ADD_BYTE(len, 0x05);	// Attribute new-lock
					    }
					    else {
						    ADD_BYTE(len, 0x01);	// Attribute new
					    }
			    }
		    }

		    else {
			    i = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - MTIndex - 1;
			    nv_addr = ui_sms_list [UI_SMS_TXT_NEW_URGENT].nv_idx [i]; 
				    if (ui_sms_text_list[nv_addr][0] == 2|| ui_sms_text_list[nv_addr][0] == 3) {
				    ADD_BYTE(len, 0x05);	// Attribute new-lock
				    }
				    else {
					    ADD_BYTE(len, 0x01);	// Attribute new
				    }
		    }
	    }

	    else {
		    
		    i = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - MTIndex - 1;
		    nv_addr = ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].nv_idx [i];			
			    if (ui_sms_text_list[nv_addr][0] == 2|| ui_sms_text_list[nv_addr][0] == 3) {
			    ADD_BYTE(len, 0x05);	// Attribute new-lock
			    }
			    else {
				    ADD_BYTE(len, 0x01);	// Attribute new
			    }
	    }
    }

    else {													  // Old messages
	    i = MTIndex;
	    i -= ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt + ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt;
	    j = ui_sms_list[UI_SMS_TXT_OLD].cnt - i - 1;
	    nv_addr = ui_sms_list[UI_SMS_TXT_OLD].nv_idx[j];

	    if (ui_sms_text_list[nv_addr][0] == 2|| ui_sms_text_list[nv_addr][0] == 3) {
		    ADD_BYTE(len, 0x07);	// Attribute lock
	    }
	    else {
		    ADD_BYTE(len, 0x03);	// Attribute normal
	    }
    }

    // Read MT from NV
    nv_sms.address = nv_addr;
    {
	    if (SKY_GetNV( NV_SMS_I, (nv_item_type *) &nv_sms)!= NV_DONE_S) 
        {
		    //DebugOut("\tRead MT NV error");			// Send message
		    DmSendNg();
		    return 1;
	    }
    }
    // Unpack SMS
    ui_sms_nv_data(FALSE, &nv_sms, &nv_smsi);	

    // Make data
    ADD_BYTE(len, BCD_2_ASCII(nv_smsi.msg_data.mc_time.yr));		// MakeDate
    ADD_BYTE(len, BCD_2_ASCII(nv_smsi.msg_data.mc_time.mon));
    ADD_BYTE(len, BCD_2_ASCII(nv_smsi.msg_data.mc_time.day));
    ADD_BYTE(len, BCD_2_ASCII(nv_smsi.msg_data.mc_time.hrs));
    ADD_BYTE(len, BCD_2_ASCII(nv_smsi.msg_data.mc_time.min));
    ADD_BYTE(len, BCD_2_ASCII(nv_smsi.msg_data.mc_time.sec));

    ADD_STR(len, nv_smsi.msg_data.user_data.num_fields,
			     nv_smsi.msg_data.user_data.chari);					// SmsText
    len += (SPEC_SmsTextLen - nv_smsi.msg_data.user_data.num_fields);

    ADD_STR(len, nv_smsi.msg_data.call_back.num_fields,
	    nv_smsi.msg_data.call_back.chari);							// CallbackNumber
    len += (SPEC_SmsDialLen - nv_smsi.msg_data.call_back.num_fields);

    // Send message
    DmSendMessage(DM_MT_DATA, len);
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveNewMT(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveNewMT(VOID)
{
/*	nv_dial_type	dial;
	INT				i, j, k;

	for (i = 1; i <= 300; i++) {		// For all magic number

		dog_report( DOG_UI_RPT );
		dog_report( DOG_DS_RPT );

		//DebugOut("\tMake... %d", i);

		memset((VOID *)&dial, 0, sizeof(dial));
		dial.address = i;
		dial.exist = 1;
		// group
		dial.group = rand()%4;
		// name
		dial.letters[0] = 'A' + (i/15);
		for (j = 1; j < 16; j++) {
			dial.letters[j] = 'A' + (rand()%26);
		}
		// dial
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 10; k++) {
				dial.multi_digits[j][k] = '0' + (rand()%10);
			}
			dial.multi_num_digits[j] = 10;
		}

		// Write to NV
		if (DmPutDial(&dial) == 0) {
			//DebugOut("\tCmd alloc fail");			// Send message
			DmSendNg();
			return 1;
		}
	}
*/
	// Send message
	DmSendOk();
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveDelMt(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveDelMt(VOID)
{
	nv_sms_type	nv_sms;
	INT			MTIndex, MTListNum;
	INT			pos;
	INT			list, index, i;

    // Read MO index
	pos = 0;
	GET_BYTE(pos, MTIndex);

	MTListNum = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt + ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt + ui_sms_list[UI_SMS_TXT_OLD].cnt;

	// Find location
	if (MTIndex < MTListNum - ui_sms_list[UI_SMS_TXT_OLD].cnt) { // New message

		if(ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0) {

			if(ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0) {

				if(MTIndex >= ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) {
					
					index = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - (MTIndex - ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt) - 1;
					list = UI_SMS_TXT_NEW_NOT_URGENT;
				}

				else {

					index = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - MTIndex - 1;
					list = UI_SMS_TXT_NEW_URGENT;
				}
			}

			else {
				index = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - MTIndex - 1;
				list = UI_SMS_TXT_NEW_URGENT;
			}
		}

		else {
			
			index = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - MTIndex - 1;
			list = UI_SMS_TXT_NEW_NOT_URGENT;
		}
	}

	else {													  // Old messages		
		MTIndex -= ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt + ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt;
		index = ui_sms_list[UI_SMS_TXT_OLD].cnt - MTIndex - 1;
		list = UI_SMS_TXT_OLD;
	}

	// Free NV
    nv_sms.address = ui_sms_list[list].nv_idx [index];

	// Free NV memory
	if (nv_sms.address <= MAX_NV_MSGS)	// Check valid address
		(void) SKY_FreeNV( NV_SMS_I, ( nv_item_type * )&nv_sms );

    // Move token to avail list
    ui_sms_list[UI_SMS_AVAIL].nv_idx[ui_sms_list[UI_SMS_AVAIL].cnt] = ui_sms_list [list].nv_idx[index];
    ui_sms_list[UI_SMS_AVAIL].cnt++;

    // Repack
    for (i = index + 1; i < ui_sms_list[list].cnt; i++)
		ui_sms_list[list].nv_idx[i - 1] = ui_sms_list [list].nv_idx[i];
    ui_sms_list[list].cnt--;

    if( IsSmsNewMsg() == FALSE)
    {
        SKY_SET_NEW_MESSAGE(FALSE);
        SKY_SetThenCompareSMS(SMS_ICON_NO_MESSAGE);
        SKY_DrawAnnunciators();
    }

    // Send message
	DmSendOk();
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetMelody(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveSetMelody(VOID)
{
    INT       MelodyType;
    INT         MelodyId;
    INT         pos;
    CHAR        szMelodyName[SKY_FILE_EXT_NAME_MAX_LENGTH+1];       // NUM_LCD_COLUMNS

    // Reset read pos
    pos = 0;

    PBookClose();
    if(SKY_IsFileOrDirExist(SKY_TEMP_FILE_NAME))
    {
        if(SKY_GET_DM_TEMP_FILE() != SKY_INVALID_FILE_HANDLE)
            SKY_CloseFile(SKY_GET_DM_TEMP_FILE());
    
        SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
        SKY_DeleteFile(SKY_TEMP_FILE_NAME);
    }

    // Read Melody Setup Info.
    GET_BYTE(pos, MelodyType);
    GET_WORD(pos, MelodyId);


    if (SKY_IS_MY_BELL(MelodyId))           //사용자 멜로디
    {
        // Read Melody Name
        GET_STR(pos, SPEC_MaxMMFileNameLen, szMelodyName);
    }

    if (MelodyType == 0x00)                                 // Default Bell Setting
    {
        SKY_SET_BELL_ID(MelodyId);
        
        if (SKY_IS_MY_BELL(MelodyId))
            SKY_SET_BELL_FILE_NAME(szMelodyName);
    }
    else if (MelodyType == 0x01)                            // Alarm Bell Setting
    {
        SKY_SET_ALARM_BELL_ID(MelodyId);
        
        if (SKY_IS_MY_BELL(MelodyId))
            SKY_SET_ALARM_BELL_TYPE_FILE_NAME(szMelodyName);
    }
    else if (MelodyType == 0x02)                            // Two Number Bell Setting
    {
        SKY_SET_NUMBER_PLUS_BELL_ID(MelodyId);
        
        if (SKY_IS_MY_BELL(MelodyId))
            SKY_SET_NUMBER_PLUS_BELL_FILENAME(szMelodyName);
    }
    else
    {
        PBookFilesOpen();
        DmSendNg();
        return 0;
    }

	// Send message
    PBookFilesOpen();
	DmSendOk();
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveSetAnimation(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8	DmReceiveSetAnimation(VOID)
{
    PICTURE_TYPE_T  Picture;
    INT             pos;
    INT             AniIndex;
    CHAR            szAniName[SPEC_MaxMMFileNameLen];
    INT             AniOption;

    // Reset read pos
    pos = 0;

    PBookClose();
    if(SKY_IsFileOrDirExist(SKY_TEMP_FILE_NAME))
    {
        if(SKY_GET_DM_TEMP_FILE() != SKY_INVALID_FILE_HANDLE)
            SKY_CloseFile(SKY_GET_DM_TEMP_FILE());
    
        SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
        SKY_DeleteFile(SKY_TEMP_FILE_NAME);
    }

    GET_BYTE(pos, AniIndex);
//    if (AniIndex < ANI_TYPE_POWER_ON || AniIndex >= ANI_TYPE_NV_MAX)

    if (AniIndex >= ANI_TYPE_MAX)
    {
        DmSendNg();		// Out of range
        return 0;
    }
    GET_WORD(pos, AniOption);
    Picture.wPictureID = AniOption; //Picture ID
    STRCPY(Picture.szFileName, "");

    if (AniOption == sMY_PICTURE || AniOption == sMY_PHOTO)            //사용자 지정 애니메이션 || 사용자 사진
    {
        GET_STR(pos, SPEC_MaxMMFileNameLen, szAniName);
        STRCPY(Picture.szFileName, szAniName);
    }
    
    switch(AniIndex)
    {
    case 0x00:                  //IDLE
        SKY_SET_IDLE_PICTURE(Picture);
        if(Picture.wPictureID == sPICTURE_NOT_USED )   //그림 사용안함
        {
            SKY_SET_IDLE_SCREEN(IS_CLOCK);
        }else
        {
            SKY_SET_IDLE_SCREEN(IS_PICTURE);
        }
        //SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);
        //SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);
        break;

    case 0x01:                  //Paging
        SKY_SET_INCOMING_PICTURE(Picture);
        break;

    case 0x02:                  //Power On
        SKY_SET_POWER_ON_PICTURE(Picture);
        break;

    case 0x03:                  //Power Off
        SKY_SET_POWER_OFF_PICTURE(Picture);
        break;

    case 0x04:                  //WAP Strt.
        SKY_SET_WAP_PICTURE(Picture);
        break;

    default:
        PBookFilesOpen();
        DmSendNg();             // Out of range
        return 0;
    }

    // Send message
    DmSendOk();
    PBookFilesOpen();
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8		DmReceiveGetMelodyReq(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8		DmReceiveGetMelodyReq(VOID)
{
    WORD            wMelodyId;
    INT             MelodyType;
    INT             pos, len;

    // Reset read pos
    pos = 0;

    // Read Melody Setup Info.
    GET_BYTE(pos, MelodyType);

    len = 0;
    ADD_BYTE(len, MelodyType);
    if (MelodyType == 0x00)                                 // Default Bell Setting
    {
        wMelodyId = SKY_GET_BELL_ID();
        ADD_WORD(len, wMelodyId);
        if (SKY_IS_MY_BELL(wMelodyId))
        {
            ADD_STR(len, SPEC_MaxMMFileNameLen, SKY_GET_BELL_FILE_NAME());
        }
    }
    else if (MelodyType == 0x01)                            // Alarm Bell Setting
    {
        wMelodyId = SKY_GET_ALARM_BELL_ID();
        ADD_WORD(len, wMelodyId);
         
        if (SKY_IS_MY_BELL(wMelodyId))
        {
            ADD_STR(len, SPEC_MaxMMFileNameLen , SKY_GET_ALARM_BELL_FILE_NAME());
        }
    }
    else if (MelodyType == 0x02)                            // Two Number Bell Setting
    {
        wMelodyId = SKY_GET_NUMBER_PLUS_BELL_ID();
        ADD_WORD(len, wMelodyId);
    
        if (SKY_IS_MY_BELL(wMelodyId))
        {
            ADD_STR(len, SPEC_MaxMMFileNameLen , SKY_GET_NUMBER_PLUS_BELL_FILENAME());
        }
    }
    else
    {
        DmSendNg();
        return 0;
    }
    
    // Send message
    DmSendMessage(DM_GET_MELODY_RSP, len);
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8		DmReceiveGetAnimationReq(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8		DmReceiveGetAnimationReq(VOID)
{
    PICTURE_TYPE_T  Picture;
    INT             pos, len;
    BYTE            AniIndex;

    // Reset read pos
    pos = 0;

    GET_BYTE(pos, AniIndex);
    if (AniIndex >= ANI_TYPE_MAX)
    {
        DmSendNg();		// Out of range
        return 0;
    }

    switch(AniIndex)
    {
    case 0x00:
        Picture = SKY_GET_IDLE_PICTURE();
        break;
    case 0x01:
        Picture = SKY_GET_INCOMING_PICTURE();
        break;
    case 0x02:
        Picture = SKY_GET_POWER_ON_PICTURE();
        break;
    case 0x03:
        Picture = SKY_GET_POWER_OFF_PICTURE();
        break;
    case 0x04:
        Picture = SKY_GET_WAP_PICTURE();
        break;
    default:
        DmSendNg();		// Out of range
        return 0;
    }

    len = 0;
    ADD_BYTE(len, AniIndex);
    ADD_WORD(len, Picture.wPictureID);
    if(Picture.wPictureID == 0xFFFE || Picture.wPictureID == 0xFFFD)
    {
        ADD_STR(len, SPEC_MaxMMFileNameLen,Picture.szFileName);
    }
    
    // Send message
    DmSendMessage(DM_GET_ANIMATION_RSP, len);
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetAlarm(VOID)
**
** Description: 
**     Send Alarm Data
**
** Input:
**     
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
UINT8	DmReceiveGetAlarm(VOID)	// Send Alarm Data
{
	clk_julian_type	tm;
	INT             UsedAlarmSize;
	INT             len;
	INT             i;

	// Clear send buffer
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	UsedAlarmSize = 0;
	len = 1;

	for (i = 0; i < SPEC_MaxAlarmSize; i++) {
        if (SKY_GET_ALARM_TIME_TYPE(i))     
        {
            ADD_BYTE(len, i);
			ADD_STR(len, SPEC_AlarmNameLen , SKY_GET_ALARM_TITLE(i));		// AlarmMemo
            
			if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_NONE)
            {
                ADD_BYTE(len, 0);
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_AFTER_10M)
            {
                ADD_BYTE(len, 1);
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_AFTER_20M)
            {
                ADD_BYTE(len, 2);
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_AFTER_30M)
            {
                ADD_BYTE(len, 3);
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_AFTER_1H)
            {
                ADD_BYTE(len, 4);
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_SET_ANYTIME)
            {
                ADD_BYTE(len, 5);
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_DAY)
            {
                ADD_BYTE(len, 6);    
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_WEEK)
            {
                ADD_BYTE(len, 7);  
            }else if(SKY_GET_ALARM_TIME_TYPE(i) == ATT_EVERY_MONTH)
            {
                ADD_BYTE(len, 8);
            }
            
            if(SKY_IS_ALARM_TIME_ACTIVE(i))
            {
                ADD_BYTE(len, 1);
            }else
            {
                ADD_BYTE(len, 0);
            }

            if(SKY_IS_ALARM_HOLIDAY_ACTIVE(i))
            {
                ADD_BYTE(len, 1);
            }else
            {
                ADD_BYTE(len, 0);
            }
            // AlarmType
            //ADD_BYTE(len, ((ui.ealarm[i].Attr & 0x20) ? 1 : 0));			// AlarmState (X)
			clk_secs_to_julian(SKY_GET_ALARM_TIME(i), &tm );
			ADD_BYTE(len, tm.month);								// AlarmDate
			ADD_BYTE(len, tm.day);
            ADD_BYTE(len, tm.day_of_week);
			ADD_BYTE(len, tm.hour);
			ADD_BYTE(len, tm.minute);

			UsedAlarmSize++;
		}
	}
	dmSendBuffer[0] = UsedAlarmSize;

	// Send message
	DmSendMessage(DM_ALARM_DATA, len);
	return 0;
}

UINT8	DmReceiveSetAlarm2(VOID)
{
    clk_julian_type	tm;
    DWORD           cur_time;
    BYTE            bAlarmIndex, bData;
    BYTE            AlarmType, bMonth, bDay, bWeek, bHour, bMin;
    INT             pos;
    ALARM_TIME_T    AlarmData;
    clk_julian_type	 cjCurrentTime;

    // Clear send buffer
    pos = 0;
    cur_time = SKY_GetLocalTime ();
    clk_secs_to_julian(cur_time, &tm);								// Set current time to tm

    // Read data
    GET_BYTE(pos, bAlarmIndex);									// UsedAlarmSize

    // Set used alarm
    GET_STR(pos, SPEC_AlarmNameLen, AlarmData.szTitle);         // AlarmMemo
    GET_BYTE(pos, AlarmType);                                   // AlarmType
    //GET_BYTE(pos, AlarmState);								// AlarmState   (X)
    GET_BYTE(pos, bData);
    
    if(bData)
    {
        SKY_SET_ALARM_TIME_ACTIVE(bAlarmIndex, TRUE);
    }
    else
    {
        SKY_SET_ALARM_TIME_ACTIVE(bAlarmIndex, FALSE);
    }

    GET_BYTE(pos, bData);
    if(bData)
    {
        SKY_SET_ALARM_HOLIDAY_ACTIVE(bAlarmIndex, TRUE);
    }
    else
    {
        SKY_SET_ALARM_HOLIDAY_ACTIVE(bAlarmIndex, FALSE);
    }

    SKY_SET_ALARM_TIME_USED(bAlarmIndex, FALSE);

    GET_BYTE(pos, bMonth);                                    // AlarmDate
    tm.month = bMonth;
    GET_BYTE(pos, bDay);
    tm.day = bDay;
    GET_BYTE(pos, bWeek);
    tm.day_of_week = bWeek;
    GET_BYTE(pos,bHour);
    tm.hour = bHour;
    GET_BYTE(pos, bMin);
    tm.minute = bMin;
    tm.second = 0;
    AlarmData.dwTime = clk_julian_to_secs(&tm);
    
    if( AlarmType == ATT_SET_ANYTIME ||\
	    AlarmType == ATT_EVERY_DAY ||\
	    AlarmType == ATT_EVERY_WEEK ||\
	    AlarmType == ATT_EVERY_MONTH )
    {

		clk_secs_to_julian(SKY_GetLocalTime(), &cjCurrentTime);
		if(  cjCurrentTime.hour > tm.hour )
		{
			SKY_SET_ALARM_TIME_USED(bAlarmIndex, TRUE);
		}
		else if( cjCurrentTime.hour == tm.hour )
		{
			if( cjCurrentTime.minute >= tm.minute )
			{
				SKY_SET_ALARM_TIME_USED(bAlarmIndex, TRUE);
			}
		}
		else ;
    }

    SKY_SET_ALARM_TIME_TYPE(bAlarmIndex, (ALARM_TIME_TYPE_E)AlarmType);
    SKY_SET_ALARM_TIME(bAlarmIndex, AlarmData.dwTime);
    SKY_SET_ALARM_TITLE(bAlarmIndex, AlarmData.szTitle);
    SKY_WRITE_CONFIG(SC_APP);
    // Send message
    DmSendOk();
    return 0;
}
UINT8	DmReceiveSetAlarm(VOID)
{
    clk_julian_type	tm;
    DWORD           cur_time;
    BYTE            UsedAlarmSize, bData, bAlarmIndex;
    BYTE            AlarmType, bMonth, bDay, bWeek, bHour, bMin;
    INT             pos;
    INT             i;
    ALARM_TIME_T    AlarmData;
    clk_julian_type	 cjCurrentTime;

    // Clear send buffer
    pos = 0;
    cur_time = SKY_GetLocalTime ();
    clk_secs_to_julian(cur_time, &tm);								// Set current time to tm

    // Read data
    GET_BYTE(pos, UsedAlarmSize);									// UsedAlarmSize

    // Set used alarm
    for (i = 0; i < UsedAlarmSize; i++) {
        GET_BYTE(pos, bAlarmIndex);	
        GET_STR(pos, SPEC_AlarmNameLen, AlarmData.szTitle);         // AlarmMemo
        GET_BYTE(pos, AlarmType);                                   // AlarmType
        //GET_BYTE(pos, AlarmState);								// AlarmState   (X)

        GET_BYTE(pos, bData);
        if(bData)
        {
            SKY_SET_ALARM_TIME_ACTIVE(bAlarmIndex, TRUE);
        }
        else
        {
            SKY_SET_ALARM_TIME_ACTIVE(bAlarmIndex, FALSE);
        }

        GET_BYTE(pos, bData);
        if(bData)
        {
            SKY_SET_ALARM_HOLIDAY_ACTIVE(bAlarmIndex, TRUE);
        }
        else
        {
            SKY_SET_ALARM_HOLIDAY_ACTIVE(bAlarmIndex, FALSE);
        }
        
        SKY_SET_ALARM_TIME_USED(bAlarmIndex, FALSE);

        GET_BYTE(pos, bMonth);                                    // AlarmDate
        tm.month = bMonth;
        GET_BYTE(pos, bDay);
        tm.day = bDay;
        GET_BYTE(pos, bWeek);
        tm.day_of_week = bWeek;
        GET_BYTE(pos,bHour);
        tm.hour = bHour;
        GET_BYTE(pos, bMin);
        tm.minute = bMin;
        tm.second = 0;
        AlarmData.dwTime = clk_julian_to_secs(&tm);

        if( AlarmType == ATT_SET_ANYTIME ||\
	        AlarmType == ATT_EVERY_DAY ||\
	        AlarmType == ATT_EVERY_WEEK ||\
	        AlarmType == ATT_EVERY_MONTH )
        {
		    clk_secs_to_julian(SKY_GetLocalTime(), &cjCurrentTime);
		    if(  cjCurrentTime.hour > tm.hour )
		    {
			    SKY_SET_ALARM_TIME_USED(bAlarmIndex, TRUE);
		    }
		    else if( cjCurrentTime.hour == tm.hour )
		    {
			    if( cjCurrentTime.minute >= tm.minute )
			    {
				    SKY_SET_ALARM_TIME_USED(bAlarmIndex, TRUE);
			    }
		    }
		    else ;
        }

        SKY_SET_ALARM_TIME_TYPE(bAlarmIndex, (ALARM_TIME_TYPE_E)AlarmType);
        SKY_SET_ALARM_TIME(bAlarmIndex, AlarmData.dwTime);
        SKY_SET_ALARM_TITLE(bAlarmIndex, AlarmData.szTitle);
    }

	// Clear empty alarm
/*    for (;i < SPEC_MaxAlarmSize; i++) {
        AlarmType = ATT_NONE;
        SKY_SET_ALARM_TIME_TYPE(i, (ALARM_TIME_TYPE_E)AlarmType);
    }
*/
    SKY_WRITE_CONFIG(SC_APP);
    // Send message
    DmSendOk();
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetMemo(VOID)
**
** Description: 
**     Send Memo Data
**
** Input:
**     
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
UINT8	DmReceiveGetMemo(VOID)	// Send Memo Data
{
	INT				nUsedMemoSize;
	INT				len, i;
    BYTE            szData[SPEC_MemoNameLen];

	// Init
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	nUsedMemoSize = 0;
	len = 1;

	for (i = 0; i < SPEC_MaxMemoSize; i++) {
		if (SKY_GET_MEMO_TYPE(i) > 0) {
			nUsedMemoSize++;
			STRCPY(szData ,SKY_GET_MEMO_DATA(i));
            ADD_BYTE(len, i);                                       // MemoIndex
			ADD_STR(len, SPEC_MemoNameLen, szData);                   // MemoText
		}
	}
	dmSendBuffer[0] = nUsedMemoSize;

    // Send message
	DmSendMessage(DM_MEMO_DATA, len);
	return 0;
}

UINT8	DmReceiveSetMemo(VOID)
{
	CHAR			memo[SPEC_MemoNameLen + 1];
	INT8            nMemoIndex;
	BYTE            pos;

	// Get memo index
	pos = 0;
	GET_BYTE(pos, nMemoIndex);
    GET_STR(pos, SPEC_MemoNameLen, memo);
	SKY_SET_MEMO_TYPE(nMemoIndex, MT_MEMO);                     //데이타 존재
	SKY_SET_MEMO_TIME(nMemoIndex, SKY_GetLocalTime());          //현재 시각
    SKY_SET_MEMO_DATA(nMemoIndex, memo);                        

	// Send message
	DmSendOk();
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	
UINT8   DmReceiveGetUsedGroup(VOID)
**
** Description: 
**     Send GroupData
**
** Input:
**     
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
UINT8   DmReceiveGetUsedSchedule(VOID)
{
    SCHL_RECORD_T   ScheduleRecord;
	INT  			UsedSize;
	INT				len;
	INT				i;

	// Clear send buffer
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
	UsedSize = 0;
	len = 1;

	// Make used list
	for (i = 0; i < SPEC_MaxScheduleSize; i++) {
        SKY_SeekReadSchlT(i, &ScheduleRecord);
		if (ScheduleRecord.Type != SRT_NONE) {
			UsedSize++;
			ADD_BYTE(len, i);			// BookIndex
		}
	}

    dmSendBuffer[0] = UsedSize;
	// Send message
	DmSendMessage(DM_USED_SCHEDULE_LIST, len);
	return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetSchedule(VOID)
**
** Description: 
**     Send Schedule Data
**
** Input:
**     
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
UINT8	DmReceiveGetSchedule(VOID)	// Send Schedule Data
{
    SCHL_RECORD_T   ScheduleRecord;
    INT				nScheduleIdx;
    INT				len, pos;
    BYTE            szData[SCHL_MAX_EVENT_DATA_LENGTH + 1];
    BYTE            bMonth, bDay, bHr, bMin;
    WORD            wYear;
    clk_julian_type julian;

    pos = 0;
    GET_BYTE(pos, nScheduleIdx);

    // Init
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    //nUsedScheduleSize = 0;
    len = 0;
    
    // nScheduleIdx == 100 일 경우는 기념일의 알람 정보를 요구하는 것임.
    SKY_SeekReadSchlT(nScheduleIdx, &ScheduleRecord);
    if (ScheduleRecord.Type != SRT_NONE) {
        STRCPY(szData ,ScheduleRecord.abData);
        ADD_BYTE(len, nScheduleIdx);                                       // Schedule Index
        ADD_STR(len, SPEC_ScheduleNameLen, szData);                   // Schedule Data
        ADD_BYTE(len, ScheduleRecord.Type);
        clk_secs_to_julian(ScheduleRecord.dwStartTime, &julian);
        wYear = julian.year;
        bMonth = julian.month;
        bDay = julian.day;
        bHr = julian.hour;
        bMin = julian.minute;
        ADD_WORD(len, wYear);
        ADD_BYTE(len, bMonth);
        ADD_BYTE(len, bDay);
        ADD_BYTE(len, bHr);
        ADD_BYTE(len, bMin);

        clk_secs_to_julian(ScheduleRecord.dwEndTime, &julian);
        wYear = julian.year;
        bMonth = julian.month;
        bDay = julian.day;
        bHr = julian.hour;
        bMin = julian.minute;
        ADD_WORD(len, wYear);
        ADD_BYTE(len, bMonth);
        ADD_BYTE(len, bDay);
        ADD_BYTE(len, bHr);
        ADD_BYTE(len, bMin);

        ADD_BYTE(len, ScheduleRecord.Recursive.Type);
        clk_secs_to_julian(ScheduleRecord.Recursive.dwEndTime, &julian);
        wYear = julian.year;
        bMonth = julian.month;
        bDay = julian.day;
        bHr = julian.hour;
        bMin = julian.minute;
        ADD_WORD(len, wYear);
        ADD_BYTE(len, bMonth);
        ADD_BYTE(len, bDay);
        ADD_BYTE(len, bHr);
        ADD_BYTE(len, bMin);

        ADD_BYTE(len, ScheduleRecord.Alarm.Type);

        clk_secs_to_julian(ScheduleRecord.Alarm.dwTime, &julian);
        wYear = julian.year;
        bMonth = julian.month;
        bDay = julian.day;
        bHr = julian.hour;
        bMin = julian.minute;
        ADD_WORD(len, wYear);
        ADD_BYTE(len, bMonth);
        ADD_BYTE(len, bDay);
        ADD_BYTE(len, bHr);
        ADD_BYTE(len, bMin);
    }
    // Send message
    DmSendMessage(DM_SCHEDULE_DATA, len);
    return 0;
}

UINT8	DmReceiveSetSchedule(VOID)
{
    SCHL_RECORD_T   ScheduleRecord;
    BYTE            abData[SCHL_MAX_EVENT_DATA_LENGTH + 1];
    INT				nUsedScheduleSize;
    INT				pos, i, nScheduleindex, nType;
    BYTE            bMonth, bDay, bHr, bMin;
    WORD            wYear;
    clk_julian_type julian;
    DWORD           current_time;
    
    current_time = SKY_GetLocalTime();
    clk_secs_to_julian(current_time, &julian);

	// Get memo index
    pos = 0;
    GET_BYTE(pos, nUsedScheduleSize);
    for (i = 0; i < nUsedScheduleSize; i++) {
        GET_BYTE(pos, nScheduleindex);                   //index
        
        GET_STR(pos, SPEC_ScheduleNameLen, abData);     //string
        STRNCPY(ScheduleRecord.abData, abData, SPEC_ScheduleNameLen);
        
        GET_BYTE(pos, nType);                           //Type
        ScheduleRecord.Type = (SCHL_RECORD_TYPE_E)nType;
        SKY_SET_SCHL_PRE_CONFIG_TYPE(nScheduleindex,  (SCHL_RECORD_TYPE_E)nType);
        
        GET_WORD(pos, wYear);
        GET_BYTE(pos, bMonth);
        GET_BYTE(pos, bDay);
        GET_BYTE(pos, bHr);
        GET_BYTE(pos, bMin);
        julian.year = wYear;
        julian.month = bMonth;
        julian.day = bDay;
        julian.hour = bHr;
        julian.minute = bMin;
        ScheduleRecord.dwStartTime = clk_julian_to_secs(&julian);
        SKY_SET_SCHL_PRE_CONFIG_START_TIME(nScheduleindex,ScheduleRecord.dwStartTime);
        
        GET_WORD(pos, wYear);
        GET_BYTE(pos, bMonth);
        GET_BYTE(pos, bDay);
        GET_BYTE(pos, bHr);
        GET_BYTE(pos, bMin);
        julian.year = wYear;
        julian.month = bMonth;
        julian.day = bDay;
        julian.hour = bHr;
        julian.minute = bMin;
        ScheduleRecord.dwEndTime = clk_julian_to_secs(&julian);

        GET_BYTE(pos, nType);
        ScheduleRecord.Recursive.Type = nType;
        SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(nScheduleindex, ScheduleRecord.Recursive.Type)
        
        GET_WORD(pos, wYear);
        GET_BYTE(pos, bMonth);
        GET_BYTE(pos, bDay);
        GET_BYTE(pos, bHr);
        GET_BYTE(pos, bMin);
        julian.year = wYear;
        julian.month = bMonth;
        julian.day = bDay;
        julian.hour = bHr;
        julian.minute = bMin;
        ScheduleRecord.Recursive.dwEndTime = clk_julian_to_secs(&julian);
        SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TIME(nScheduleindex, ScheduleRecord.Recursive.dwEndTime)

        GET_BYTE(pos, nType);
        ScheduleRecord.Alarm.Type = (SCHL_ALARM_TIME_TYPE_E)nType;
        SKY_SET_SCHL_PRE_CONFIG_ALARM_TYPE(nScheduleindex, ScheduleRecord.Alarm.Type);

        GET_WORD(pos, wYear);
        GET_BYTE(pos, bMonth);
        GET_BYTE(pos, bDay);
        GET_BYTE(pos, bHr);
        GET_BYTE(pos, bMin);
        julian.year = wYear;
        julian.month = bMonth;
        julian.day = bDay;
        julian.hour = bHr;
        julian.minute = bMin;
        ScheduleRecord.Alarm.dwTime = clk_julian_to_secs(&julian);

        switch(ScheduleRecord.Alarm.Type)
        {
            case SATT_NONE:
                ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime;
                ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime;
                break;

            case SATT_BEFORE_10M:
                ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime- TIME_SECS_10_MIN;
                ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_10_MIN;
                break;

            case SATT_BEFORE_20M:
                ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime- TIME_SECS_20_MIN;
                ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_20_MIN;
                break;

            case SATT_BEFORE_30M:
                ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime - TIME_SECS_30_MIN;
                ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_30_MIN;       
                break;

            case SATT_BEFORE_1H:
                ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime - TIME_SECS_1_HOUR;
                ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_1_HOUR;
                break;

            case SATT_BEFORE_1D:
                ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime - TIME_SECS_1_DAY;
                ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_1_DAY;
                break;
        }
        SKY_SET_SCHL_PRE_CONFIG_ALARM_TIME(nScheduleindex, ScheduleRecord.Alarm.dwTime);
        SKY_SET_SCHL_PRE_CONFIG_ALARM_END_TIME(nScheduleindex, ScheduleRecord.Alarm.dwEndTime);
        
        SKY_SeekWriteSchlT(nScheduleindex, &ScheduleRecord);
    }
    // Send message
    DmSendOk();
    return 0;
}

UINT8	DmReceiveSetSchedule2(VOID)
{
    SCHL_RECORD_T   ScheduleRecord;
    BYTE            abData[SCHL_MAX_EVENT_DATA_LENGTH + 1];
    INT				pos, nScheduleindex, nTemp;
    BYTE            bMonth, bDay, bHr, bMin;
    WORD            wYear;
    clk_julian_type julian;
    DWORD           current_time;
    ui_cmd_type	    *cmd_ptr;	// Pointer to UI command
    
    current_time = SKY_GetLocalTime();
    clk_secs_to_julian(current_time, &julian);
    
    // Get SCHEDULE index
    pos = 0;
    GET_BYTE(pos, nScheduleindex);
    
    //nScheduleindex == 100 일 경우는 기념일 알람 정보 

    GET_STR(pos, SPEC_ScheduleNameLen, abData);
    STRNCPY(ScheduleRecord.abData, abData, SPEC_ScheduleNameLen);
    ScheduleRecord.abData[SCHL_MAX_EVENT_DATA_LENGTH + 1] = 0;
    GET_BYTE(pos, nTemp);
    ScheduleRecord.Type = (SCHL_RECORD_TYPE_E)nTemp;
    SKY_SET_SCHL_PRE_CONFIG_TYPE(nScheduleindex, ScheduleRecord.Type);

    GET_WORD(pos, wYear);        
    GET_BYTE(pos, bMonth);
    GET_BYTE(pos, bDay);
    GET_BYTE(pos, bHr);
    GET_BYTE(pos, bMin);
    julian.year = wYear;
    julian.month = bMonth;
    julian.day = bDay;
    julian.hour = bHr;
    julian.minute = bMin;
    ScheduleRecord.dwStartTime = clk_julian_to_secs(&julian);
    SKY_SET_SCHL_PRE_CONFIG_START_TIME(nScheduleindex, ScheduleRecord.dwStartTime);

    GET_WORD(pos, wYear);        
    GET_BYTE(pos, bMonth);
    GET_BYTE(pos, bDay);
    GET_BYTE(pos, bHr);
    GET_BYTE(pos, bMin);
    julian.year = wYear;
    julian.month = bMonth;
    julian.day = bDay;
    julian.hour = bHr;
    julian.minute = bMin;
    ScheduleRecord.dwEndTime = clk_julian_to_secs(&julian);
    
    GET_BYTE(pos, nTemp);
    ScheduleRecord.Recursive.Type = nTemp;
    SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TYPE(nScheduleindex, ScheduleRecord.Recursive.Type)
    
    GET_WORD(pos, wYear);    
    GET_BYTE(pos, bMonth);
    GET_BYTE(pos, bDay);
    GET_BYTE(pos, bHr);
    GET_BYTE(pos, bMin);
    julian.year = wYear;
    julian.month = bMonth;
    julian.day = bDay;
    julian.hour = bHr;
    julian.minute = bMin;
    ScheduleRecord.Recursive.dwEndTime = clk_julian_to_secs(&julian);
    SKY_SET_SCHL_PRE_CONFIG_RECURSIVE_TIME(nScheduleindex, ScheduleRecord.Recursive.dwEndTime)
    
    GET_BYTE(pos, nTemp);
    ScheduleRecord.Alarm.Type = (SCHL_ALARM_TIME_TYPE_E)nTemp;
    SKY_SET_SCHL_PRE_CONFIG_ALARM_TYPE(nScheduleindex, ScheduleRecord.Alarm.Type);

    GET_WORD(pos, wYear);    
    GET_BYTE(pos, bMonth);
    GET_BYTE(pos, bDay);
    GET_BYTE(pos, bHr);
    GET_BYTE(pos, bMin);
    julian.year = wYear;
    julian.month = bMonth;
    julian.day = bDay;
    julian.hour = bHr;
    julian.minute = bMin;
    ScheduleRecord.Alarm.dwTime = clk_julian_to_secs(&julian);

    switch(ScheduleRecord.Alarm.Type)
    {
        case SATT_NONE:
            ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime;
            ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime;
            break;

        case SATT_BEFORE_10M:
            ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime- TIME_SECS_10_MIN;
            ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_10_MIN;
            break;

        case SATT_BEFORE_20M:
            ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime- TIME_SECS_20_MIN;
            ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_20_MIN;
            break;

        case SATT_BEFORE_30M:
            ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime - TIME_SECS_30_MIN;
            ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_30_MIN;       
            break;

        case SATT_BEFORE_1H:
            ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime - TIME_SECS_1_HOUR;
            ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_1_HOUR;
            break;

        case SATT_BEFORE_1D:
            ScheduleRecord.Alarm.dwTime = ScheduleRecord.dwStartTime - TIME_SECS_1_DAY;
            ScheduleRecord.Alarm.dwEndTime = ScheduleRecord.Recursive.dwEndTime - TIME_SECS_1_DAY;
            break;
    }
    SKY_SET_SCHL_PRE_CONFIG_ALARM_TIME(nScheduleindex, ScheduleRecord.Alarm.dwTime);
    SKY_SET_SCHL_PRE_CONFIG_ALARM_END_TIME(nScheduleindex, ScheduleRecord.Alarm.dwEndTime);
    
    SKY_SeekWriteSchlT(nScheduleindex, &ScheduleRecord);

    if ((cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q )) != NULL) {
	    cmd_ptr->hdr.cmd        = UI_DM_UPDATE_IDLE_F;
	    cmd_ptr->hdr.task_ptr   = NULL;
	    cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
	    ui_cmd( cmd_ptr );
    }
    else {
	    //DebugOut("\tCmd alloc fail");			// Send message
	    DmSendNg();
	    return 1;
    }

    DmSendOk();
    // Send message
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8	DmReceiveGetHoliday(VOID)
**
** Description: 
**     Send Holiday Data
**
** Input:
**     
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
UINT8	DmReceiveGetHoliday(VOID)	// Send Holiday Data
{
    INT				nUsedHolidaySize;
    INT				len, i;
    
    // Init
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);
    nUsedHolidaySize = 0;
    len = 1;

    for (i = 0; i < SPEC_MaxHolidaySize; i++) {
        if (g_MyHolidayT[i].Type != SHT_NONE) {
            nUsedHolidaySize++;
            ADD_BYTE(len, i);  
            ADD_BYTE(len, g_MyHolidayT[i].Type);        // Holiday Type
            ADD_BYTE(len, g_MyHolidayT[i].fLunar);        // Lunar
            ADD_WORD(len, g_MyHolidayT[i].wYear);        //Year
            ADD_BYTE(len, g_MyHolidayT[i].unMonth);        //Month
            ADD_BYTE(len, g_MyHolidayT[i].unDay);        //Day
            ADD_BYTE(len, g_MyHolidayT[i].fConsecutive);        //Consecutive
            ADD_STR(len, SPEC_HolidayNameLen, g_MyHolidayT[i].pszName);
        }
    }
    dmSendBuffer[0] = nUsedHolidaySize;

    // Send message
    DmSendMessage(DM_HOLIDAY_DATA, len);
    return 0;
}

UINT8	DmReceiveSetHoliday(VOID)
{
    CHAR			szHoliday[SPEC_HolidayNameLen + 1];
    INT				nUsedHolidaySize;
    INT             nHolidayIndex, nTemp;
    BYTE            pos , i;

    // Get memo index
    pos = 0;
    GET_BYTE(pos, nUsedHolidaySize);                //사용중인 개수

    for(i = 0; i <nUsedHolidaySize ; i ++)
    {
        GET_BYTE(pos, nHolidayIndex);
        GET_BYTE(pos, nTemp);
        g_MyHolidayT[nHolidayIndex].Type = (SCHL_HOLIDAY_TYPE_E)nTemp;
        GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].fLunar);
        GET_WORD(pos, g_MyHolidayT[nHolidayIndex].wYear);
        GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].unMonth);
        GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].unDay);
        GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].fConsecutive);
        GET_STR(pos, SPEC_HolidayNameLen, szHoliday);
        STRNCPY(g_MyHolidayT[nHolidayIndex].pszName, szHoliday, SPEC_HolidayNameLen);
        SaveMyHoliday(nHolidayIndex);
    }
    // Send message
    DmSendOk();
    return 0;
}

UINT8	DmReceiveSetHoliday2(VOID)
{
    CHAR            szHoliday[SPEC_HolidayNameLen];
    INT8            nHolidayIndex, nTemp;
    BYTE            pos;

    // Get memo index
    pos = 0;
    GET_BYTE(pos, nHolidayIndex);
    GET_BYTE(pos, nTemp);
    g_MyHolidayT[nHolidayIndex].Type = (SCHL_HOLIDAY_TYPE_E)nTemp;
    GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].fLunar);
    GET_WORD(pos, g_MyHolidayT[nHolidayIndex].wYear);
    GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].unMonth);
    GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].unDay);
    GET_BYTE(pos, g_MyHolidayT[nHolidayIndex].fConsecutive);
    GET_STR(pos, SPEC_HolidayNameLen, szHoliday);
    STRNCPY(g_MyHolidayT[nHolidayIndex].pszName, szHoliday, SPEC_HolidayNameLen);
    SaveMyHoliday(nHolidayIndex);

    // Send message
    DmSendOk();
    return 0;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     
**
** Description: 
**     Send BookData
**
** Input:
**     
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
UINT8 DmReceiveGetMMInfo(VOID)
{
	INT				len;

    DWORD           AvailMMData;
    DWORD           AniDataSize;
    DWORD           MelDataSize;
    DWORD           PhtDataSize;

	// Init
	memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);

    Dm_GetMMInfoState(&AvailMMData, &AniDataSize, &MelDataSize, &PhtDataSize);

    len = 0;
    ADD_WORD(len, (WORD)(SPEC_MAXMMDATA >> 16));
    ADD_WORD(len, (WORD)(SPEC_MAXMMDATA & 0x0000FFFF));
    ADD_WORD(len, (WORD)(AvailMMData >> 16));
    ADD_WORD(len, (WORD)(AvailMMData & 0x0000FFFF));
    ADD_WORD(len, (WORD)(AniDataSize >> 16));
    ADD_WORD(len, (WORD)(AniDataSize & 0x0000FFFF));
    ADD_WORD(len, (WORD)(MelDataSize >> 16));
    ADD_WORD(len, (WORD)(MelDataSize & 0x0000FFFF));
    ADD_WORD(len, (WORD)(PhtDataSize >> 16));
    ADD_WORD(len, (WORD)(PhtDataSize & 0x0000FFFF));
    
	// Send message
	DmSendMessage(DM_MMINFO_DATA, len);
    return 0;
}

LOCAL BYTE ppFileList[SKY_MAX_NUM_FILES][SPEC_MaxMMFileNameLen]; 
LOCAL BYTE ppPhotoName[SKY_MAX_NUM_FILES][SPEC_MaxMMFileNameLen];


UINT8 DmReceiveGetFileList(VOID)
{
    INT             len, pos, i, nNum;
    UINT8           ReqAttrib;
//    CHAR            szFileName[SKY_PATH_NAME_MAX_LENGTH];
    DWORD           dwFileSize;
    HFILE           hFile;

    pos = 0;
    
    GET_WORD(pos, ReqAttrib);

    PBookClose();
    if(SKY_IsFileOrDirExist(SKY_TEMP_FILE_NAME))
    {
        if(SKY_GET_DM_TEMP_FILE() != SKY_INVALID_FILE_HANDLE)
            SKY_CloseFile(SKY_GET_DM_TEMP_FILE());
    
        SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
        SKY_DeleteFile(SKY_TEMP_FILE_NAME);
    }

    switch (ReqAttrib)
    {
    case DM_MELODY_TYPE:
        nNum = SKY_GetFileNumberInDataFile(SDF_MELODY);
        
        //for(i=0; i < nNum ; i++)
        //    ppFileList[i] = (BYTE*)SKY_Malloc(SPEC_MaxMMFileNameLen);
         SKY_GetFileExtList(SKY_MELODY_DIR, ppFileList);
        break;

    case DM_ANI_TYPE:
        nNum = SKY_GetFileNumberInDataFile(SDF_IMAGE);

        //for(i=0; i < nNum ; i++)
        //    ppFileList[i] = (BYTE*)SKY_Malloc(SPEC_MaxMMFileNameLen);
         SKY_GetFileExtList(SKY_IMAGE_DIR, ppFileList);
        break;

    case DM_PHT_TYPE:
        nNum = SKY_GetFileNumberInDataFile(SDF_PHOTO);
/*
        for(i=0; i < nNum ; i++)
        {
            ppFileList[i] = (BYTE*)SKY_Malloc(SPEC_MaxMMFileNameLen);
            ppPhotoName[i] = (BYTE*)SKY_Malloc(SPEC_MaxMMFileNameLen);
        }
*/
         SKY_GetFileExtList(SKY_PHOTO_DIR, ppFileList);

        for(i=0; i < nNum ; i++)
        {
            //memcpy(szFileName, ppFileList[i],16);
            //szFileName[16] = 0;
            SKY_OpenDataFile(SDF_PHOTO, ppFileList[i], &hFile, SFT_READ);
            SKY_ReadDataBlock(hFile, ppPhotoName[i], SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_CloseDataFile(hFile);
            //STRCPY(ppPhotoName[i], szFileName);
        }
        break;

    default:
/*
        for(i=0; i < nNum ; i++)
            SKY_Free(ppFileList[i]);
    
        if(ReqAttrib ==  DM_PHT_TYPE)
        {
            for(i=0; i < nNum ; i++)
                SKY_Free(ppPhotoName[i]);
        }
*/
        DmSendNg();		// Out of range
        PBookFilesOpen();
        return 0;
        break;
    }

    // Buffer Init
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);

    len = 0;
    ADD_WORD(len, ReqAttrib);
    ADD_WORD(len, nNum);
    l_memoryAlloc++;

    for (i = 0; i < nNum; i++)
    {
        // format structure
        
        switch(ReqAttrib)
        {
        case DM_MELODY_TYPE:
            //memcpy(szFileName, ppFileList[i], 14);
            //szFileName[14] = 0;
        
            ADD_STR(len, SPEC_MaxMMFileNameLen, ppFileList[i]);
            dwFileSize = SKY_GetDataFileSize(SDF_MELODY, ppFileList[i]);
            break;

        case DM_ANI_TYPE:
            //memcpy(szFileName, ppFileList[i], 14);
            //szFileName[14] = 0;

            ADD_STR(len, SPEC_MaxMMFileNameLen, ppFileList[i]);
            dwFileSize = SKY_GetDataFileSize(SDF_IMAGE, ppFileList[i]);
            break;

        case DM_PHT_TYPE:

            //memcpy(szFileName, ppFileList[i], 16);
            //szFileName[16] = 0;

            ADD_STR(len, SPEC_MaxMMFileNameLen, ppFileList[i]);
            //memcpy(szFileName, ppPhotoName[i], 14);
            //szFileName[14] = 0;

            ADD_STR(len, SPEC_MaxMMFileNameLen, ppPhotoName[i]);
            dwFileSize = SKY_GetDataFileSize(SDF_PHOTO, ppFileList[i]);
            break;

        default:
            //memcpy(szFileName, ppFileList[i], 14);
            //szFileName[14] = 0;

            ADD_STR(len, SPEC_MaxMMFileNameLen, ppFileList[i]);
            dwFileSize = 0;
            break;
        }

        ADD_WORD(len, dwFileSize >> 16);
        ADD_WORD(len, dwFileSize & 0x0000FFFF);
        //szFileName[0] = 0;
    }
/*
    for(i=0; i < nNum ; i++)
        SKY_Free(ppFileList[i]);
    
    if(ReqAttrib ==  DM_PHT_TYPE)
    {
        for(i=0; i < nNum ; i++)
            SKY_Free(ppPhotoName[i]);
    }
*/
    // Send message
    DmSendMessage(DM_FILELIST_DATA, len);
    PBookFilesOpen();

    return 0;

}

UINT8 DmReceiveGetFileInfo(VOID)
{
    INT   len, pos, ReqAttrib;
    CHAR            szFileName[SPEC_MaxMMFileNameLen];
    DWORD           dwFileSize;
    HFILE           hFile;
    BITMAP_HEADER   BitmapHeader;
    ANIMATION_HEADER_T AniHeader;

    pos = 0;
    len = 0;
    GET_WORD(pos, ReqAttrib);
    GET_STR(pos, SPEC_MaxMMFileNameLen, szFileName);

    switch(ReqAttrib)
    {
    case DM_MELODY_TYPE:
        dwFileSize = SKY_GetDataFileSize(SDF_MELODY, szFileName);
        break;

    case DM_ANI_TYPE:
        dwFileSize = SKY_GetDataFileSize(SDF_IMAGE, szFileName);
        break;

    case DM_PHT_TYPE:
        dwFileSize = SKY_GetDataFileSize(SDF_PHOTO, szFileName);
        break;

    default:
	    DmSendNg();		// Out of range
	    return 0;
    }

    // Find file with attribute & file name
    // Get exist flag and size
    // 만약 화일이 존재하지 않는다면 attrib는 0xffff로 된다.

    // Buffer Init
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);

    ADD_WORD(len, ReqAttrib);
    ADD_STR(len, SPEC_MaxMMFileNameLen, szFileName);
    ADD_WORD(len, dwFileSize>> 16);
    ADD_WORD(len, dwFileSize & 0x0000FFFF);

    switch (ReqAttrib)
    {
    case DM_MELODY_TYPE:
        ADD_STR(len, 4, "ma2");
        break;

    case DM_ANI_TYPE:
    case DM_PHT_TYPE:
        if(ReqAttrib == DM_ANI_TYPE)
        {
            if(SKY_OpenDataFile(SDF_IMAGE,szFileName,&hFile, SFT_READ) != SDR_OK)
            {
                DmSendNg();
                return 0;
            }
        }
        else
        {
            if(SKY_OpenDataFile(SDF_PHOTO,szFileName,&hFile, SFT_READ) != SDR_OK)
            {
                DmSendNg();
                return 0;
            }
        }
        if(ReqAttrib == DM_ANI_TYPE)
        {
            SKY_ReadDataBlock(hFile, &BitmapHeader, sizeof(BITMAP_HEADER));
        
            if(BitmapHeader.abID[0] == 'I' && BitmapHeader.abID[1] == 'M' && BitmapHeader.abID[2] == 'G')
            {
                ADD_BYTE(len, 01);          //Attribute
                SKY_CloseDataFile(hFile);   
                ADD_BYTE(len, BitmapHeader.nBitsPerPixel);
            }
            else if(BitmapHeader.abID[0] == 'A' && BitmapHeader.abID[1] == 'N' && BitmapHeader.abID[2] == 'I')
            {
                ADD_BYTE(len, 00);              //MUlti Frame
                SKY_ReadDataBlock(hFile, &AniHeader, sizeof(AniHeader));
                SKY_CloseDataFile(hFile); 
                ADD_BYTE(len, BitmapHeader.nBitsPerPixel);
                ADD_BYTE(len, AniHeader.nFrame);
                ADD_WORD(len, (WORD)(AniHeader.awDelayTime[0] >> 16));
                ADD_WORD(len, (WORD)(AniHeader.awDelayTime[0] & 0x0000FFFF));
            }else
            {
                DmSendNg();
                return 0;
            }
        }
        else            //Photo File
        {
            ADD_BYTE(len, 01);
            SKY_CloseDataFile(hFile);   
            ADD_BYTE(len, SKY_COLOR_SPACE);
        }
        break;

    default:
        DmSendNg();
        return 0;
        break;

    }

    DmSendMessage(DM_FILEINFO_DATA, len);
    return 0;
}
//redstars
#ifdef WIN32
#define FS_FILENAME_MAX_LENGTH  65
#endif
UINT8 DmReceiveFileDownReq(VOID)
{

    INT                len, i, pos = 0;
    STATIC CHAR        szFileName[SPEC_MaxMMFileNameLen];
    CHAR        szFullFileName[FS_FILENAME_MAX_LENGTH+1];
    BYTE        bAniType = 0, bFrame, MsgType, RspResult = 0;
    STATIC WORD s_OldSeq = 0, s_wBlockSize = 0 , s_wRestFieldSize;
    WORD        wNewSeq = 0, wSeqRsp = 0, wTemp , nNumBlock;
    STATIC WORD s_wAttrib = 0;
    STATIC DWORD       dwTarget;
    DWORD   dwPhoto;
    STATIC HFILE       s_hFile;
    BITMAP_HEADER   BitmapHeader;
    ANIMATION_HEADER_T AniHeader;
    
    len = 0;
    szFullFileName[0] = 0;
    GET_BYTE(pos, MsgType);
    GET_STR(pos, SPEC_MaxMMFileNameLen, szFileName);

    switch(MsgType)
    {
    case 0x01:              // File Information
        RspResult = 0x00;   // NAK
        wSeqRsp = 0x00;     // File Down에서 항상 Info가 첫번째 Block.
        GET_WORD(pos, wNewSeq);
        s_OldSeq = wNewSeq;

        GET_WORD(pos, s_wRestFieldSize);
        GET_WORD(pos, s_wAttrib);

        GET_WORD(pos, wTemp);          //FileSize => Double Size
        dwTarget = ((DWORD)wTemp) << 16;
        GET_WORD(pos, wTemp);
        dwTarget |= wTemp;

        GET_WORD(pos, s_wBlockSize);
        GET_WORD(pos, nNumBlock);

        if(s_wAttrib == DM_PHT_TYPE)
        { 
            if(STRLEN(szFileName) > SKY_FILE_NAME_MAX_LENGTH)
            {
                szFileName[SKY_MAX_CAMERA_IMAGE_FILE_LEN] = 0;
            }
        }else
        {   
            szFileName[SKY_FILE_NAME_MAX_LENGTH] = 0;
        }


        if(s_wAttrib == DM_ANI_TYPE || s_wAttrib == DM_PHT_TYPE)
        {
            GET_BYTE(pos, bAniType);
            GET_BYTE(pos, bFrame);
        

            if(s_wAttrib == DM_PHT_TYPE)
            {
                BitmapHeader.abID[0] = 'I';
                BitmapHeader.abID[1] = 'M';
                BitmapHeader.abID[2] = 'G';            
            }else
            {
                if(bAniType == 0)
                {
                    BitmapHeader.abID[0] = 'I';
                    BitmapHeader.abID[1] = 'M';
                    BitmapHeader.abID[2] = 'G';
                }
                else if(bAniType == 1)
                {
                    BitmapHeader.abID[0] = 'A';
                    BitmapHeader.abID[1] = 'N';
                    BitmapHeader.abID[2] = 'I';
                    AniHeader.nFrame = bFrame;
                    for(i = 0; i<MAX_ANIMATION_FRAME; i++)
                        AniHeader.awDelayTime[i] = 0;
                }
                else 
                {
                    // Unknown Type
                    DmSendNg();		// Out of range
                    return 0;
                }

            }

            BitmapHeader.fPalette = FALSE;
            BitmapHeader.nBitsPerPixel = 8;     //256C
            BitmapHeader.Size.xWidth = SKY_SCREEN_BUFFER_WIDTH; //size
            BitmapHeader.Size.yHeight = SKY_SCREEN_BUFFER_HEIGHT;
            BitmapHeader.TransparentColor = MAX_COLOR;
            BitmapHeader.xWidth = SKY_SCREEN_BUFFER_WIDTH;
        
        }

        PBookClose();

        if(SKY_IsFileOrDirExist(SKY_TEMP_FILE_NAME))
        {
            if(SKY_GET_DM_TEMP_FILE() != SKY_INVALID_FILE_HANDLE)
                SKY_CloseFile(SKY_GET_DM_TEMP_FILE());
    
            SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
            SKY_DeleteFile(SKY_TEMP_FILE_NAME);
        }

        if (s_wAttrib == DM_ANI_TYPE)       // Animation Type
        {
            if(SKY_IsDataFile(SDF_IMAGE,szFileName))
            {
                RspResult = 0x04;
                PBookFilesOpen();
                DmSendNg();
                return 0;
            }
            if(SKY_GetFileNumberInDataFile(SDF_IMAGE) >= sMAX_IMAGE_COUNT || \
                SKY_GetFreeSpaceInDataFile() < dwTarget)
            {
                RspResult = 0x03;
                PBookFilesOpen();
                DmSendNg();
                return 0;
            }
            //SKY_WriteDataBlock(hFile, &BitmapHeader, sizeof(BitmapHeader));
            if(bAniType == 1)
            {
                //SKY_WriteDataBlock(hFile, &AniHeader, sizeof(AniHeader));
            }
            RspResult = 0x01;
        }
        else if (s_wAttrib == DM_MELODY_TYPE)  // Melody Type
        {
            if(SKY_IsDataFile(SDF_MELODY,szFileName) )
            {
                RspResult = 0x04;
                PBookFilesOpen();
                DmSendNg();
                return 0;

            }
            if(SKY_GetFileNumberInDataFile(SDF_MELODY) >= sMAX_MELODY_COUNT || \
                SKY_GetFreeSpaceInDataFile() < dwTarget)
            {
                RspResult = 0x03;
                PBookFilesOpen();
                DmSendNg();
                return 0;            
            }
            RspResult = 0x01;
        }
        else if(s_wAttrib == DM_PHT_TYPE)
        {
            if(SKY_GetFileNumberInDataFile(SDF_PHOTO) >= MAX_PHOTO_COUNT || \
                SKY_GetFreeSpaceInDataFile() < dwTarget)
            {
                RspResult = 0x03;
                PBookFilesOpen();
                DmSendNg();
                return 0;            
            }
            if(SKY_IsDataFile(SDF_PHOTO,szFileName))
            {
                do
                {
                    dwPhoto = atol(szFileName);
                    dwPhoto++;
                    ConvertCameraImageFileName(szFileName,dwPhoto);
                }while(SKY_IsDataFile(SDF_PHOTO,szFileName));
            }
            RspResult = 0x01;
            //SKY_WriteDataBlock(hFile, &BitmapHeader, sizeof(BitmapHeader));
        }
        else      
        {
            // Unknown Type
            PBookFilesOpen();
            DmSendNg();		// Out of range
            return 0;
        }
        PBookFilesOpen();

        s_hFile = SKY_OpenTempFile();
        SKY_SET_DM_TEMP_FILE(s_hFile);

        if(s_hFile == SKY_INVALID_FILE_HANDLE)
        {
            RspResult = 0x03;
            DmSendNg();
            return 0;
        }

        break;

    case 0x02:  // Block Transfer
        RspResult = 0x00;   // NAK
        wSeqRsp = wNewSeq;

        GET_WORD(pos, wNewSeq);
        if (wNewSeq == s_OldSeq) // Retry Pass
        {
            RspResult = 0x01;   // ACK
            wSeqRsp = wNewSeq;
            break;
        }

        if (wNewSeq != (s_OldSeq + 1)) // Next Block
            break;

        GET_WORD(pos, s_wRestFieldSize);
        if (s_wBlockSize != s_wRestFieldSize) 
            break;

        //GET_BYTE(pos, wAttrib);
        for (i = 0; i < s_wRestFieldSize; i++)
        {
            GET_BYTE(pos, bDataTransBlock[i]);
        }

        SKY_WriteDataBlock(s_hFile, bDataTransBlock, s_wRestFieldSize);

        if(s_wAttrib == DM_PHT_TYPE)
        {
            bDataTransBlock[SKY_MAX_CAMERA_IMAGE_FILE_LEN+1] = 0;
            if(SKY_IsCameraData(bDataTransBlock))
            {
                RspResult = 0x04;
                DmSendNg();
                return 0;
            }
        }

        MSG_HIGH("[EQUS DATAMANAGER DOWN1] %d %d",wNewSeq,s_wRestFieldSize,0);
        
        s_OldSeq = wNewSeq;
        RspResult = 0x01;   // ACK
        wSeqRsp = wNewSeq;
        break;

    case 0x03:  // Last Block Transfer
        RspResult = 0x00;   // NAK
        wSeqRsp = wNewSeq;

        GET_WORD(pos, wNewSeq);
        if (wNewSeq != (s_OldSeq + 1)) // Next Block
            break;

        GET_WORD(pos, s_wRestFieldSize);
        for (i = 0; i < s_wRestFieldSize; i++)
        {
            GET_BYTE(pos, bDataTransBlock[i]);
        }
        SKY_WriteDataBlock(s_hFile, bDataTransBlock, s_wRestFieldSize);
        SKY_CloseDataFile(s_hFile);
        SKY_SET_DM_TEMP_FILE(SKY_INVALID_FILE_HANDLE);
            
        PBookClose();
        switch(s_wAttrib)
        {
        case DM_ANI_TYPE:
            STRCPY(szFullFileName, SKY_IMAGE_DIR);
            STRCAT(szFullFileName, "/");
            STRCAT(szFullFileName, szFileName);
            if(!FileRenameTemp2Target(szFullFileName,dwTarget))
            {
                DmSendNg();
                return 0;
            }
            //SKY_RenameFile(SKY_TEMP_FILE_NAME, szFullFileName);
            break;

        case DM_MELODY_TYPE:
            STRCPY(szFullFileName, SKY_MELODY_DIR);
            STRCAT(szFullFileName, "/");
            STRCAT(szFullFileName, szFileName);
            if(!FileRenameTemp2Target(szFullFileName,dwTarget))
            {
                DmSendNg();
                return 0;
            }
            //SKY_RenameFile(SKY_TEMP_FILE_NAME, szFullFileName);
            break;

        case DM_PHT_TYPE:
            STRCPY(szFullFileName, SKY_PHOTO_DIR);
            STRCAT(szFullFileName, "/");
            STRCAT(szFullFileName, szFileName);
            if(!FileRenameTemp2Target(szFullFileName,dwTarget))
            {
                DmSendNg();
                return 0;
            }
            //SKY_RenameFile(SKY_TEMP_FILE_NAME, szFullFileName);
            break;

        default:
            DmSendNg();
            return 0;
            break;
        }
        PBookFilesOpen();
        s_OldSeq = wNewSeq;
        RspResult = 0x02;   // DownLoad Finish
        wSeqRsp = wNewSeq;
        break;

    default:
        DmSendNg();		// Out of range
        return 0;
    }

    // Buffer Init
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);

    ADD_BYTE(len, RspResult);
    ADD_WORD(len, wSeqRsp);

    // Send message
    DmSendMessage(DM_FILEDOWN_RSP, len);
    return 0;
}
 
UINT8 DmReceiveFileUpReq(VOID)
{
    INT             pos = 0, len = 0;
    DWORD           i;
    BYTE            bMsgType;
    WORD            wNewSeq = 0, wSeqRsp = 0;
    BYTE            szFileName[SPEC_MaxMMFileNameLen];
    STATIC HFILE    hFile;
    STATIC BYTE     s_bRspResult = 0;
    STATIC WORD     s_wOldSeq = 0, s_wRestFieldSize = 0, s_wBlockSize, s_wBlockNum, wAttrib = 0;
    STATIC DWORD    s_dwReadFile = 0, s_dwFileSize = 0;
//    BYTE    pData, eData;
    STATIC BOOL s_fFirstBlock = FALSE;
//    BYTE            bDataTransBlock[500];    

    GET_BYTE(pos, bMsgType);
    GET_WORD(pos, wNewSeq);

    switch(bMsgType)
    {
    case 0x01:  // File Information
        s_bRspResult = 0x00;       // NAK
        s_wOldSeq = wNewSeq;

        GET_WORD(pos, wAttrib);
        GET_STR(pos, SPEC_MaxMMFileNameLen, szFileName);       // Reading done!

#ifdef	FEATURE_IP_AIRDATA		
        if (SKY_GET_IP_AIRDATAMANAGER())
            s_wBlockSize = IP_BLOCK_TRANSFER_SIZE;
        else
#endif	// #ifdef	FEATURE_IP_AIRDATA	
            s_wBlockSize = BLOCK_TRANSFER_SIZE;

        if (wAttrib == DM_ANI_TYPE)
        {

            if (SKY_OpenDataFile(SDF_IMAGE,szFileName,&hFile, SFT_READ) != SDR_OK)
            { 
                DmSendNg();	
                return 0;
            }

            s_dwFileSize = SKY_GetDataFileSize(SDF_IMAGE, szFileName);
            if (s_dwFileSize == 0)
            {
                DmSendNg();	
                return 0;
            }
        }
        else if (wAttrib == DM_MELODY_TYPE)
        {

            if (SKY_OpenDataFile(SDF_MELODY,szFileName,&hFile, SFT_READ) != SDR_OK)
            {
                DmSendNg();	
                return 0;
            }

            s_dwFileSize = SKY_GetDataFileSize(SDF_MELODY, szFileName);
            if (s_dwFileSize == 0)
            {
                DmSendNg();	
                return 0;
            }
        }
        else if (wAttrib == DM_PHT_TYPE)
        {

            if (SKY_OpenDataFile(SDF_PHOTO,szFileName,&hFile, SFT_READ) != SDR_OK)
            {
                DmSendNg();	
                return 0;
            }

            s_dwFileSize = SKY_GetDataFileSize(SDF_PHOTO, szFileName);
            if (s_dwFileSize == 0)
            {
                DmSendNg();	
                return 0;
            }
        }
        else 
        {   
            DmSendNg();	// Unknown Type
            return 0;
        }
        s_wBlockNum = (WORD)((s_dwFileSize + s_wBlockSize - 1) / s_wBlockSize); //블럭의 Size를 맞추기 위해
        s_dwReadFile = 0;
        s_wRestFieldSize = 8;          // File Size(4byte) + Block Size(2byte) + Numer of blocks(2byte)

        s_bRspResult = 0x01;       // File Information
        wSeqRsp = wNewSeq;
        break;

    case 0x02:  // Block Transfer
        s_bRspResult = 0x00;   // NAK
        wSeqRsp = wNewSeq;

        if (wNewSeq == s_wOldSeq) // Retry Action!
        {
            s_bRspResult = 0x02;
            break;
        }

        if (wNewSeq != (WORD)(s_wOldSeq + 1)) // Next Block
        {
            DmSendNg();
            return 0;
            break;
        }

        if ((s_dwReadFile + s_wBlockSize) >= s_dwFileSize)
        {   // Last Block!
            s_wBlockSize = s_dwFileSize - s_dwReadFile;
            s_wRestFieldSize = s_wBlockSize; 
            s_bRspResult = 0x04;
        }
        else
        {   // Block Continue
            s_wRestFieldSize = s_wBlockSize; 
            s_bRspResult = 0x02;
        }
        //dwPosition = s_wBlockSize * s_wOldSeq;      // ==s_dwReadFile
        //SKY_SeekFile(hFile, FS_SO_SET,dwPosition);
        SKY_ReadDataBlock(hFile, bDataTransBlock, s_wBlockSize);
        //SKY_CloseDataFile(hFile);
        s_dwReadFile += s_wBlockSize;

        MSG_HIGH("[EQUS DATAMANAGER1] %d %d %d",wNewSeq,s_wBlockSize,s_dwReadFile);
        MSG_HIGH("[EQUS DATAMANAGER2] %d ",s_bRspResult, 0, 0);
        s_wOldSeq = wNewSeq;
        wSeqRsp = wNewSeq;
        break;

    default:
	    DmSendNg();		// Out of range
	    return 0;
    }

    // Buffer Init
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);

    ADD_BYTE(len, s_bRspResult);
    ADD_WORD(len, wSeqRsp);
    ADD_WORD(len, s_wRestFieldSize);

    switch(s_bRspResult)
    {
    case 0x01:      // File Information
        ADD_WORD(len, s_dwFileSize >> 16);
        ADD_WORD(len, s_dwFileSize & 0x0000FFFF);
        ADD_WORD(len, s_wBlockSize);
        ADD_WORD(len, s_wBlockNum);
        MSG_HIGH("[EQUS DATAMANAGER0] %d %d %d",s_dwFileSize,s_wBlockSize,s_wBlockNum);
        s_fFirstBlock = TRUE;
        break;

    case 0x02:      // Block Transfer
/*        if (wAttrib == DM_MELODY_TYPE)
        {
            i = 0;
            if(s_fFirstBlock)
            {
                for (i = 0; i < 4; i++)
                    ADD_BYTE(len, bDataTransBlock[i]);
                s_fFirstBlock = FALSE;
            }
            for (; i < s_wBlockSize ; i++)
            {
                pData = bDataTransBlock[i++];
                eData = bDataTransBlock[i];
                ADD_BYTE(len, eData);
                ADD_BYTE(len, pData);
            }
        }else
*/
        {
            for (i = 0; i < s_wBlockSize; i++)
                ADD_BYTE(len, bDataTransBlock[i]);
        }
        break;

    case 0x04:
/*        if (wAttrib == DM_MELODY_TYPE)
        {
            for (i = 0; i < s_wBlockSize; i++)
            {
                pData = bDataTransBlock[i++];
                if(i < s_wBlockSize)
                {
                    eData = bDataTransBlock[i];
                    ADD_BYTE(len, eData);
                }
                ADD_BYTE(len, pData);
            }
        }else
*/        {
            for (i = 0; i < s_wBlockSize; i++)
                ADD_BYTE(len, bDataTransBlock[i]);
        }
        SKY_CloseDataFile(hFile);
        break;
    }

    // Send message
    DmSendMessage(DM_FILEUP_RSP, len);
    return 0;
}

UINT8      DmReceiveFileDeleteReq(VOID)
{
    INT	            len, pos;
    WORD            wReqAttrib;
    CHAR		    szFileName[SPEC_MaxMMFileNameLen];
    BYTE            bRet, index;
    PICTURE_TYPE_T  Picture;
    pb_group_info_type  GroupInfo;

    pos = 0;
    GET_WORD(pos, wReqAttrib);
    GET_STR(pos, SPEC_MaxMMFileNameLen, szFileName);

    bRet = MM_OK;
    switch(wReqAttrib)
    {
    case DM_ANI_TYPE:
        Picture = SKY_GET_IDLE_PICTURE();
        if(Picture.wPictureID == 0xFFFE)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_IDLE_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_IDLE_PICTURE(Picture);

                SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

                //SKY_SET_BANNER(INIT_BANNER);
                SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);
            }
        }
        Picture = SKY_GET_INCOMING_PICTURE();
        if(Picture.wPictureID == 0xFFFE)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_INCOMING_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_INCOMING_PICTURE(Picture);        
            }
        }
        Picture = SKY_GET_POWER_ON_PICTURE();
        if(Picture.wPictureID == 0xFFFE)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_POWER_ON_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_POWER_ON_PICTURE(Picture);        
            }
        }
        Picture = SKY_GET_POWER_OFF_PICTURE();
        if(Picture.wPictureID == 0xFFFE)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_POWER_OFF_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_POWER_OFF_PICTURE(Picture);
            }
        
        }
        Picture = SKY_GET_WAP_PICTURE();
        if(Picture.wPictureID == 0xFFFE)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_WAP_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_WAP_PICTURE(Picture);        
            }
        }

        SKY_DeleteDataFile(SDF_IMAGE, szFileName);
        break;

    case DM_MELODY_TYPE:
        if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))
        {
            if(!STRCMP(SKY_GET_BELL_FILE_NAME(), szFileName))
                SKY_SET_BELL_ID(INIT_BELL_ID);
        }
        //Two Number Bell
        if(SKY_IS_MY_BELL(SKY_GET_NUMBER_PLUS_BELL_ID()))
        {
            if(!STRCMP(SKY_GET_NUMBER_PLUS_BELL_FILENAME(),szFileName))
                SKY_SET_NUMBER_PLUS_BELL_ID(INIT_BELL_ID);        
        }
        // 알람벨
        if(SKY_IS_MY_BELL(SKY_GET_ALARM_BELL_ID()))
        {
            if(!STRCMP(SKY_GET_ALARM_BELL_FILE_NAME(), szFileName))
                SKY_SET_ALARM_BELL_ID(INIT_ALARM_BELL_ID);
        }

        // 스케쥴러 알람벨
        if(SKY_IS_MY_BELL(SKY_GET_SCHL_ALERT_BELL_ID()))
        {
            if(!STRCMP(SKY_GET_SCHL_ALERT_BELL_FILE_NAME(),szFileName))
                SKY_SET_SCHL_ALERT_BELL_ID(INIT_SCHL_ALERT_BELL_ID);
        }
        // 그룹벨
        for(index = 0; index < g_nGroup; index ++)
        {
            GetGroupInfo(&GroupInfo, index);

            if(SKY_IS_MY_BELL(GroupInfo.Bell.wBellID))
            {
                if(!STRCMP(GroupInfo.Bell.szFileName, szFileName))
                {
                    GroupInfo.Bell.wBellID = 0;   // NOT Assign.
                    SetGroupInfo(&GroupInfo, index);
                }
            }
        }
        SKY_DeleteDataFile(SDF_MELODY, szFileName);
        break;

    case DM_PHT_TYPE:
        Picture = SKY_GET_IDLE_PICTURE();
        if(Picture.wPictureID == 0xFFFD)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_IDLE_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_IDLE_PICTURE(Picture);

                SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
                SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

                //SKY_SET_BANNER(INIT_BANNER);
                SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);
            }
        }
        Picture = SKY_GET_INCOMING_PICTURE();
        if(Picture.wPictureID == 0xFFFD)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_INCOMING_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_INCOMING_PICTURE(Picture);        
            }
        }
        Picture = SKY_GET_POWER_ON_PICTURE();
        if(Picture.wPictureID == 0xFFFD)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_POWER_ON_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_POWER_ON_PICTURE(Picture);        
            }
        }
        Picture = SKY_GET_POWER_OFF_PICTURE();
        if(Picture.wPictureID == 0xFFFD)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_POWER_OFF_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_POWER_OFF_PICTURE(Picture);
            }
        
        }
        Picture = SKY_GET_WAP_PICTURE();
        if(Picture.wPictureID == 0xFFFD)
        {
            if(!STRCMP(Picture.szFileName, szFileName))
            {
                Picture.wPictureID = INIT_WAP_PICTURE;
                STRCPY(Picture.szFileName, (BYTE*)"");
                SKY_SET_WAP_PICTURE(Picture);        
            }
        }
        SKY_DeleteDataFile(SDF_PHOTO, szFileName);

        if(IS_MY_PHOTO_ALL(SKY_GET_IDLE_PICTURE().wPictureID) && \
            (SKY_GetFileNumberInDataFile(SDF_PHOTO) == 0))
        {
            Picture.wPictureID = INIT_IDLE_PICTURE;
            STRCPY((BYTE *)Picture.szFileName, (BYTE*)"");
            SKY_SET_IDLE_PICTURE(Picture);

            SKY_SET_IDLE_SCREEN(INIT_IDLE_SCREEN);
            SKY_SET_IDLE_DISPLAY_WATCH(INIT_IDLE_DISPLAY_WATCH);

            //SKY_SET_BANNER(INIT_BANNER);
            SKY_SET_IDLE_DISPLAY_NAME(INIT_IDLE_DISPLAY_NAME);                            
        }

        break;

    default:
        bRet = MM_NG;
        break;
    }

    // Buffer Init
    memset(dmSendBuffer, 0, DM_MAX_MSG_LEN);

    len = 0;
    switch(bRet)
    {
    case MM_NG:
        ADD_BYTE(len, 0x00);
        break;

    case MM_OK:
        ADD_BYTE(len, 0x01);
        break;

    case MM_FILE_NOT_EXIST:
        ADD_BYTE(len, 0x02);
        break;
    }

    ADD_STR(len, SPEC_MaxMMFileNameLen, szFileName);
    // Send message
    DmSendMessage(DM_MMFILE_DELETE_RSP, len);
    return 0;
}
//#endif // FEATURE_MMFUNC_TO_NV_FOR_DMGR

UINT8      DmReceiveGetPhoneEnv(VOID)
{
	INT	            len;
    UINT8           Temp;
    CHAR            szTemp[10];
    CHAR            szTime[8 + 1];
    clk_julian_type	tmStart, tmEnd;
    
    len = 0;

#ifndef WIN32
    Temp = mob_sw_rev[5] - '0';
    ADD_BYTE(len, Temp);
    Temp = mob_sw_rev[6] - '0';
    ADD_BYTE(len, Temp);
#endif

//#if (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FOLDER)
    ADD_BYTE(len, SKY_GET_HOW_TO_ANSWER());
/*
#elif (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FLIP)
    ADD_BYTE(len, SKY_GET_FLIP_KEY());
#endif
*/
    ADD_STR(len, 32, SKY_GET_MEMORY_DIAL_NUMBER());
    
    memset(szTemp, 0, 4);
    if (SKY_IS_AUTO_AREA_CODE())
    {
        Temp = 1;
        STRCPY(szTemp ,SKY_GET_AREA_CODE());
    }
    else
    {
        Temp = 0;
    }
    ADD_BYTE(len, Temp);
    ADD_STR(len, 4, szTemp);
    
    /*Sub Backlight*/
    if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_10SECOND)  //자동조명  
    {
        ADD_BYTE(len,0);
        ADD_STR(len, 8, "00000000");

    }else if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_ALWAYS) //항상 켜짐
    {
        ADD_BYTE(len,1);
        ADD_STR(len, 8, "00000000");
    
    }else if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME)    //시간 지정
    {
        ADD_BYTE(len,2);
        clk_secs_to_julian( SKY_GET_AUTO_SUBBACKLIGHT_START_TIME(), &tmStart );
        clk_secs_to_julian( SKY_GET_AUTO_SUBBACKLIGHT_END_TIME(), &tmEnd );
        sprintf(szTime, "%02d%02d%02d%02d", tmStart.hour, tmStart.minute,
                                                    tmEnd.hour, tmEnd.minute);
        ADD_STR(len, 8, szTime);

    }else //if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_NO)  //조명 사용안함
    {
        ADD_BYTE(len,3);    
        ADD_STR(len, 8, "00000000");
    }

    /*Backlight*/
    if(SKY_GET_AUTO_BACKLIGHT() == AB_10SECOND)  //자동조명  
    {
        ADD_BYTE(len,0);
        ADD_STR(len, 8, "00000000");

    }else if(SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS) //항상 켜짐
    {
        ADD_BYTE(len,1);
        ADD_STR(len, 8, "00000000");
    
    }else if(SKY_GET_AUTO_BACKLIGHT() == AB_TIME)    //시간 지정
    {
        ADD_BYTE(len,2);
        clk_secs_to_julian( SKY_GET_AUTO_BACKLIGHT_START_TIME(), &tmStart );
        clk_secs_to_julian( SKY_GET_AUTO_BACKLIGHT_END_TIME(), &tmEnd );
        sprintf(szTime, "%02d%02d%02d%02d", tmStart.hour, tmStart.minute,
                                                    tmEnd.hour, tmEnd.minute);
        ADD_STR(len, 8, szTime);

    }else //if(SKY_GET_AUTO_BACKLIGHT() == AB_NO)  //조명 사용안함
    {
        ADD_BYTE(len,3);    
        ADD_STR(len, 8, "00000000");
    }

    ADD_BYTE(len, SKY_IS_AUTO_REDIAL());       // 자동 재 다이얼

    ADD_BYTE(len, SKY_IS_HANDS_FREE_AUTO_RECEIPT());      // 핸즈프리 자동 착신
    ADD_BYTE(len, SKY_IS_EAR_PHONE_AUTO_RECEIPT());       // 이어 마이크 자동 착신

	// Send message

    DmSendMessage(DM_PHONE_ENV_DATA, len);

    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     UINT8      DmReceiveSetPhoneEnv(VOID)
**
** Description: 
**     
**
** Input:
**     
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
UINT8      DmReceiveSetPhoneEnv(VOID)
{
	INT	            pos;
    UINT8           BackLight;
    CHAR            szTime[9];
    BYTE            AutoReceipt;
    UINT8           uTemp;
    INT             i, folder;
    BYTE            size;
    BOOL            fArea;
    BYTE            szArea[MAX_AREA_CODE_LENGTH];
    DWORD           tmp_STime, tmp_ETime; 
    
//#if (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FOLDER)
/*
#elif (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FLIP)
    FLIP_KEY_E      flip;
    BYTE            szDial[MAX_DIAL_DIGIT_LENGTH - 1];    
#endif
*/
	pos = 0;

    // 플립키 설정
//#if (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FOLDER)
    GET_BYTE(pos, folder);
    SKY_SET_HOW_TO_ANSWER((HOW_TO_ANSWER_E)folder); //dsi_put_nv_item(NV_FOLDERANS_I, (nv_item_type *)&ui.folderans);
    pos += 32;
/*
#elif (FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FLIP)

    GET_BYTE(pos, flip);
    if (flip)
    {
        GET_STR(pos, 32, szDial);
    }
    else
    {
        pos += 32;
    }
    SKY_SET_FLIP_KEY(flip);
    //SKY_SET_MEMORY_DIAL_NUMBER(szDial);       왜 메모리 다이얼은 설정 안될까?
    //dsi_put_nv_item(NV_FLIP_KEY_ACT_I, (nv_item_type *)&ui.flip_key_act);
#endif //(FEATURE_SKY_MODEL_TYPE == SKY_MODEL_FOLDER)
*/
    // 자동 지역 번호
    GET_BYTE(pos, fArea);
    if (fArea)
    {
        
        GET_STR(pos, 4, szArea);
        szArea[3] = 0;
        size = STRLEN(szArea);
        for (i = 0; i < size; i++)
        {
            szArea[i] = szArea[i] - '0';
        }
        SKY_SET_AREA_CODE(szArea);
    }
    else
    {
        pos += 4;
    }
    SKY_SET_AUTO_AREA_CODE(fArea);
    
    /*Sub Backlight*/
    GET_BYTE(pos, BackLight);

    switch(BackLight)
    {
    case AB_10SECOND:
        SKY_SET_AUTO_SUBBACKLIGHT(AB_10SECOND);
		pos += 8;
        break;

    case AB_ALWAYS:
        SKY_SET_AUTO_SUBBACKLIGHT(AB_ALWAYS);
        pos += 8;
        break;
	
    case AB_TIME:
        GET_STR(pos, 8, szTime);
        
        SKY_SET_AUTO_SUBBACKLIGHT(AB_TIME);
        
        tmp_STime = (szTime[0] - '0') * 10 * 60 * 60;
        tmp_STime += (szTime[1] - '0') * 60 * 60;
        tmp_STime += (szTime[2] - '0') * 10 * 60;
        tmp_STime += (szTime[3] - '0') * 60;

        tmp_ETime = (szTime[4] - '0') * 10 * 60 * 60;
        tmp_ETime += (szTime[5] - '0') * 60 * 60;
        tmp_ETime += (szTime[6] - '0') * 10 * 60;
        tmp_ETime += (szTime[7] - '0') * 60;
		
		SKY_SET_AUTO_SUBBACKLIGHT_START_TIME(tmp_STime);
        SKY_SET_AUTO_SUBBACKLIGHT_END_TIME(tmp_ETime);
        break;

    case AB_NO:
        SKY_SET_AUTO_SUBBACKLIGHT(AB_NO);
		pos += 8;
        break;
	
    default:
        DmSendNg();
        return 0;
    }

    /*Backlight*/
    GET_BYTE(pos, BackLight);

    switch(BackLight)
    {
    case AB_10SECOND:
        SKY_SET_AUTO_BACKLIGHT(AB_10SECOND);
        pos += 8;
        break;

    case AB_ALWAYS:
        SKY_SET_AUTO_BACKLIGHT(AB_ALWAYS);
        pos += 8;
        break;
	
    case AB_TIME:
        GET_STR(pos, 8, szTime);

        SKY_SET_AUTO_BACKLIGHT(AB_TIME);
        
        tmp_STime = (szTime[0] - '0') * 10 * 60 * 60;
        tmp_STime += (szTime[1] - '0') * 60 * 60;
        tmp_STime += (szTime[2] - '0') * 10 * 60;
        tmp_STime += (szTime[3] - '0') * 60;

        tmp_ETime = (szTime[4] - '0') * 10 * 60 * 60;
        tmp_ETime += (szTime[5] - '0') * 60 * 60;
        tmp_ETime += (szTime[6] - '0') * 10 * 60;
        tmp_ETime += (szTime[7] - '0') * 60;
		
		SKY_SET_AUTO_BACKLIGHT_START_TIME(tmp_STime);
        SKY_SET_AUTO_BACKLIGHT_END_TIME(tmp_ETime);
		break;

    case AB_NO:
		SKY_SET_AUTO_BACKLIGHT(AB_NO);
		pos += 8;
        break;
	
    default:
        DmSendNg();
        return 0;
    }

    GET_BYTE(pos, uTemp);       // 자동 재 다이얼
    if (uTemp)
    {
        SKY_SET_AUTO_REDIAL(TRUE);	
    }
    else
    {
        SKY_SET_AUTO_REDIAL(FALSE);	
    }

	
    GET_BYTE(pos, AutoReceipt);           // 핸즈프리 자동 착신

	SKY_SET_HANDS_FREE_AUTO_RECEIPT(AutoReceipt);
    
    GET_BYTE(pos, AutoReceipt);           // 이어 마이크 자동 착신
	SKY_SET_EAR_PHONE_AUTO_RECEIPT(AutoReceipt);
    
	// Send message
	DmSendOk();
    return 0;
}
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     
**
** Description: 
**     Send BookData
**
** Input:
**     
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
#ifdef	FEATURE_IP_AIRDATA
UINT8      DmReceiveDisconnect(VOID)
{

	if (SKY_IS_SOCK_MODE())
	{
		EQS_SEND_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
	}
	return 0;
}
#endif	// #ifdef	FEATURE_IP_AIRDATA

#ifdef	DMAN_DEBUG
VOID	DebugOut(CHAR *pFormat, ...)
{
	static CHAR	buf[256];
	va_list		pArg;

	memset(buf, 0, 256);
	va_start(pArg, pFormat);
	vsprintf(buf, pFormat, pArg);

#ifdef	FEATURE_SKT_AIRDATA	
	if (SKY_GET_AIRDATAMANAGER())
	{
		AirDataManagerTX((BYTE *)buf, STRLEN(buf));
	}
	else
#endif	//	#ifdef	FEATURE_SKT_AIRDATA	
#ifdef	FEATURE_IP_AIRDATA
	if (SKY_GET_IP_AIRDATAMANAGER())
	{
		AirDataManagerTX((BYTE *)buf, STRLEN(buf));
	}
	else
#endif	// #ifdef	FEATURE_IP_AIRDATA
	{
		at_send_extended_text(buf);
	}
}
#endif
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     
**
** Description: 
**     Send BookData
**
** Input:
**     
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

VOID    resetBioSW9M(VOID)
{
    // shinji 99/08/31	Non-sleep for Data Manager Flash handling
    if (DmMode == 1) 
    {
	    DmMode = 0;
        // BIO_SW9_M 가 설정되어 있으면
	    // BIO_SW9_M 을 토글하여 sleep mode 로 들어가도록 한다.
	    if (BIO_GET_SW ( BIO_SW9_M )) 
        {
		    bio_set_sw ( ( bio_sw_type ) (bio_switches&~BIO_SW9_M) );
	    }
    }
}

/* 다음 function은 PCM File 시험용임. SMS 및 Datamanager과의 연동시 제거하여야 함. */
#ifdef FEATURE_EFS_PCM_TEST
/*==============================================================================*/
/*  DM_EFS_put_sound                                                            */
/*                                                                              */
/*  데이타매니저와의 연동이 없어 DmReceiveFileDownReq(VOID) 대신 사용하는 함수  */
/*                                                                              */
/*==============================================================================*/
boolean DM_EFS_put_sound(CONST CHAR* file_name, SHORT size, BYTE *data)
{
	static BYTE		*pAlloc;
  static SHORT	Size;
	static BYTE	*pMelData;
  static WORD RestFieldSize;
  static dm_file_open   stFileStoreOpen;

  Size= size;
  pMelData = data;

  stFileStoreOpen.Attrib = DM_MELODY_TYPE;
  memset(stFileStoreOpen.szFileName,0,sizeof(stFileStoreOpen.szFileName));
  memcpy(stFileStoreOpen.szFileName,file_name,STRLEN(file_name));
  stFileStoreOpen.nFileSize = size;
  stFileStoreOpen.nBlockSize = 32;
//stFileStoreOpen.nNumBlock =0;
//stFileStoreOpen.pAlloc = NULL;
//stFileStoreOpen.Ret

	/*
				//Title이 현재 존재하는지 check!! -- 존재하면 delete후, 저장 
				IsValid = mm_IsValidMA1File((CHAR *)musicTitle);
				if(IsValid){		//존재하는 경우
					mm_DeleteMelFile((CHAR *)musicTitle);
				}
				*/

  if (mm_nv_RequestToNV(MM_NV_MEL_FILE_STORE_OPEN, (VOID *)&stFileStoreOpen) != NV_DONE_S)
  //	if(mm_MelFileStoreOpen(file_name, Size, 32, &pAlloc) != MM_OK)
  {
    mm_nv_RequestToNV(MM_NV_FILE_STORE_CLOSE, /*(VOID *)&bSuccess*/FALSE);
    //mm_FileStoreClose(FALSE);
    return FALSE;
  }

	
	{
    /*
		memcpy(pDown, 
								mm_DefaultMMediaFileType[MM_MEL_DIR_ID], //[0]
								MM_NUM_FILE_TYPE); //4
						mm_FileBlockStore(MM_NUM_FILE_TYPE); //4
						mbSize -= MM_NUM_FILE_TYPE; //4
    */
	
	  while(Size > 0)
  	{
		  if(Size >= 32)   
			{
			  memcpy(pAlloc, pMelData, 32);
			  mm_nv_RequestToNV(MM_NV_FILE_BLOCK_STORE, /*(VOID *)&RestFieldSize*/32);
				//if ( mm_FileBlockStore(32) != MM_OK ) 
				//  MSG_HIGH("mm_FileBlockStore FAIL...",0,0,0);
				//  return FALSE;							
				pMelData += 32;
				Size -= 32;
			}
			else
			{			
			  memcpy(pAlloc, pMelData, Size);
			  mm_nv_RequestToNV(MM_NV_FILE_BLOCK_STORE, /*(VOID *)&RestFieldSize*/Size);
				//if ( mm_FileBlockStore(Size) != MM_OK ) 				
				//  MSG_HIGH("mm_FileBlockStore LAST FAIL...",0,0,0);
				//  return FALSE;							
				Size = 0;
				break;
			}
		}

		mm_nv_RequestToNV(MM_NV_FILE_STORE_CLOSE, /*(VOID *)&bSuccess*/TRUE);
		//mm_FileStoreClose(TRUE);

	}
	return TRUE;

} /*DM_EFS_put_sound()*/

#endif //ifdef FEATURE_EFS_PCM_TEST
//#endif	// SKT_DM_ESMS

BOOL FileRenameTemp2Target(const BYTE* pszPathName, DWORD size)
{
    HFILE hFile, hTargetFile;
    DWORD   dw_Total;

    dw_Total = size;
    
    hFile = SKY_OpenFile(SKY_TEMP_FILE_NAME,FS_AM_READONLY);
    if(hFile == SKY_INVALID_FILE_HANDLE)
    {
        return FALSE;
    }

    hTargetFile = SKY_OpenFile(pszPathName, FS_AM_CREATE);
    if(hTargetFile == SKY_INVALID_FILE_HANDLE)
    {
        SKY_CloseDataFile(hFile);   
        return FALSE;
    }

    while(1)
    {
        if(dw_Total > 1000)
        {
            SKY_ReadDataBlock(hFile, &bDataTransBlock, 1000);
            SKY_WriteFile(hTargetFile, &bDataTransBlock, 1000);
        }else
        {
            SKY_ReadDataBlock(hFile, &bDataTransBlock, dw_Total);
            SKY_WriteFile(hTargetFile, &bDataTransBlock, dw_Total);   
            SKY_CloseDataFile(hFile);   
            SKY_CloseDataFile(hTargetFile);   
            break;
        }
        dw_Total -= 1000;
    }
    return TRUE;
}

struct tagDATA_FILE_CONFIG_T {
    BYTE* pszDataDir;
    UINT8 nMaxCount;
}; 
typedef struct tagDATA_FILE_CONFIG_T    DATA_FILE_CONFIG;

extern DATA_FILE_CONFIG l_DataFileConfig[];

BOOL FileRenameForMMFile(SKY_DATA_FILE_E SkyDataFile, const BYTE* pszSrcName, const BYTE* pszDestName)
{
    HFILE hFile, hTargetFile;
    DWORD   dw_Total, dwAvailMem;

    dwAvailMem = SKY_GetFreeSpaceInDataFile();

    dw_Total = SKY_GetDataFileSize(SkyDataFile, pszSrcName);
    
    if(dw_Total > dwAvailMem)
        return FALSE;

    if(SKY_OpenDataFile(SkyDataFile, pszSrcName, &hFile, SFT_READ) != SDR_OK)
    {
        return FALSE;
    }

    if(SKY_CreateDataFile(SkyDataFile, pszDestName, &hTargetFile) != SDR_OK)
    {
        SKY_CloseDataFile(hFile);   
        return FALSE;
    }

    while(1)
    {
        if(dw_Total > 1000)
        {
            SKY_ReadDataBlock(hFile, &bDataTransBlock, 1000);
            SKY_WriteFile(hTargetFile, &bDataTransBlock, 1000);
        }else
        {
            SKY_ReadDataBlock(hFile, &bDataTransBlock, dw_Total);
            SKY_WriteFile(hTargetFile, &bDataTransBlock, dw_Total);   
            SKY_CloseDataFile(hFile);   
            SKY_CloseDataFile(hTargetFile);   
            break;
        }
        dw_Total -= 1000;
    }

    SKY_DeleteDataFile(SkyDataFile, pszSrcName);
    return TRUE;
}