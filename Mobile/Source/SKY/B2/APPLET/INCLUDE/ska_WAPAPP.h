/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     
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
** 2000-12-20 ytchoi     Created.
**
** ============================================================================
*/


#ifndef _SKA_WAPAPP_H_
#define _SKA_WAPAPP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "wapconnector.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define WAPMAIL_SIZE     86//일단 a10size와 동일 하게 !!
#define URL_MAX  100
#define MAX_GOTOURL_LEN       70

#define INITURL_IS_WAPPUSH    1 //초기 접속시 Push URL을 load하기위해 
#define INITURL_IS_WAPMAIL    2 //초기 접속시 E-mail URL을 load하기위해 
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef enum
{
    WAPGO_NONE = 0,
	WAPGO_MAKECALL,
    WAPGO_INCOMINGCALL,
	WAPGO_SMS,
	WAPGO_GVM,
	WAPGO_SKVM,
    WAPGO_WAVELET,  
    WAPGO_INITERROR,
    WAPGO_ABNORMALEXIT,

} wapexit_enum_type;

typedef enum
{
    WAPGOMINOR_NONE = 0,
	WAPGOMINOR_MAKECALL,
    WAPGOMINOR_SMS,
	WAPGOMINOR_INCOMINGCALL,
	WAPGOMINOR_DI_ID,
    WAPGOMINOR_DI_PROMPT,
    WAPGOMINOR_INPUT,
    WAPGOMINOR_NEWURL,
    WAPGOMINOR_PHONEBOOK
} wapminor_enum_type;

typedef struct {
	BYTE      Memo[WAPMAIL_SIZE];	                
	byte      Attr;
	byte      time[4];		
} WAP_MAIL_TYPE;

enum
{
	INIT_WAPTIME,
	START_WAPTIME,
	MID_WAPTIME,
	END_WAPTIME
};


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern UCHAR g_szGoToURL[MAX_GOTOURL_LEN];
extern UCHAR g_pszPushUrl[URL_MAX];
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern wapexit_enum_type s_towhere;
extern wapminor_enum_type s_towhereminor;
extern int g_errorNumber;
extern int g_errorType;
extern CHAR  gWAPinit_url[130];
extern CHAR  gWAPproxy_url[130];
extern CHAR  gWAPEmail_Url[130];
extern CHAR  temp_url[200];
extern int InWapMailorPush;
extern BOOL g_WAPfirstDisplay;
extern INT g_KindofPUSH;
extern BOOL g_bWAPinitAccessFail;

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/* jrkwon 2001-05-04 */

VOID SKY_API SKA_MainWAP(SKY_EVENT_T* pInEvent); //n.Top key동작 applet 
VOID SKY_API SKA_WAPHANDLE_KEY(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_ChooseMenu(SKY_EVENT_T *pEvent);
#ifdef FEATURE_WAPMAIL
VOID SKY_API SKA_SendWAPMsg(SKY_EVENT_T* pEvent); //n.Top메일작성 applet
#endif //FEATURE_WAPMAIL
#ifdef INCOMING_CALL_IN_WAP
BOOL SKY_API SKY_IS_DATA_CALL(void);
#endif //#ifdef INCOMING_CALL_IN_WAP
extern void WAP_READ_INITDATA(void);
extern void WAPUsedTime(char UsedTimeType);


#endif  /* #ifndef _SKA_WAPAPP_H_ */