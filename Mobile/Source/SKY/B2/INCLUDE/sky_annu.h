/*
** ============================================================================
**
** Project: 
**     Bunny
**
** File:
**     sky_annu.h
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
** 2000-12-13 girin     Created.
**
** ============================================================================
*/

#ifndef _SKY_ANNU_H_
#define _SKY_ANNU_H_

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
enum tagSMS_ICON_E
{
    SMS_ICON_NEW_MESSAGE = 0,
    SMS_ICON_READ_MESSAGE,
    SMS_ICON_NO_MESSAGE
};
typedef enum tagSMS_ICON_E SMS_ICON_T;

enum tagBELLVIB_ICON_E
{
    BELLVIB_ICON_NO_SOUND_NO_VIB,
    BELLVIB_ICON_BELL_AND_VIB,
    BELLVIB_ICON_VIB_AFTER_BELL,
    BELLVIB_ICON_BELL_ONLY,
    BELLVIB_ICON_VIB_ONLY,
    BELLVIB_ICON_ETIQUETTE
};
typedef enum tagBELLVIB_ICON_E  BELLVIB_ICON_E;

enum tagIN_USE_STATE_E
{
    IUS_DISCONNECT,
    IUS_CONNECT,
    IUS_DORMANT
};
typedef enum tagIN_USE_STATE_E IN_USE_STATE_E;

enum tagPROTOCOL_REVISION_E
{
    PR_IS95A, 
    PR_IS95B,
    PR_IS95C
};
typedef enum tagPROTOCOL_REVISION_E PROTOCOL_REVISION_E;

struct tagHALF_BAR_T
{
    UINT8 nInIdle;
    UINT8 nInTalk;
};
typedef struct tagHALF_BAR_T HALF_BAR_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern BOOL g_fNoService;
#define SKY_IS_NOSERVICE()          g_fNoService

extern HALF_BAR_T g_HalfBar;// = { 10, 13 };


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
extern BOOL SKY_CheckAnnunciators(void);
/* undersea 2001-08-27 --> @GVM/SKVM에서 256 partial display를 하기 위함 */
#if defined(FEATURE_GVM) || defined(FEATURE_SKVM)
extern void SKY_Draw256Annunciators(void);
#endif
/* <-- */
extern void SKY_DrawAnnunciators(void); 
extern void SKY_API SKY_DrawSubAnnunciators(void);
extern IN_USE_STATE_E SKY_API SKY_GetInUseState(void);

extern BOOL SKY_SetThenCompareBattLevel(void);
extern BOOL SKY_SetThenCompareRSSI(UINT16 nRSSI_Index);
extern BOOL SKY_SetThenCompareInUseState(IN_USE_STATE_E InUseState);
extern BOOL SKY_SetThenCompareSMS(SMS_ICON_T SmsIcon);
extern BOOL SKY_SetThenCompareBellVib(BELLVIB_ICON_E BellVibIcon);
extern IN_USE_STATE_E SKY_GetInUseState(void);

extern void SKY_DisplayBellVibIcon(void);
extern BOOL SKY_SetThenCompareProtocolRevision(PROTOCOL_REVISION_E Protocol);

#ifdef SW_RESET_ENABLE
extern void SKY_SaveSWResetData(void);
extern void SKY_RestoreSWResetData(void);
#endif

extern void SKY_API SKY_DrawSubAnnunciatorsIcon(void);
extern PROTOCOL_REVISION_E SKY_API SKY_GET_PROTOCOL_REV(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_ANNU_H_ */
