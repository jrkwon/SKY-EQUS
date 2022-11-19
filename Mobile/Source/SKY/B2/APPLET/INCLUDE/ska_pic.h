/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_pic.h
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
** 2000-12-29 woodstok     Created.
**
** ============================================================================
*/

#ifndef _SKA_PIC_H_
#define _SKA_PIC_H_

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
#define POPUP_MENU_MY_PHOTO     1
#define POPUP_MENU_MY_PICTURE   2

#define NUM_ANI_DATA	5 //7
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/* 
** SKY_DisplayDateTime, SKY_DisplayDateTimeWithPic에서 필요한 미리 보기 정보 
** - 휴대폰 이름 여부, 휴대폰 이름, 시계 표시 여부 
*/
struct tagIDLE_SCREEN_PREVIEW_INFO_T
{
    /* rosa 2001-09-20 --> @why, what */
    //BOOL fDisplayWatch;
    IDLE_SCREEN_E          Screen;
    IDLE_DISPLAY_WATCH_E   Watch;
    /* <-- */
    BOOL    fDisplayName;
    BYTE    szDispalyName[MAX_BANNER_LENGTH+1];
};
typedef struct tagIDLE_SCREEN_PREVIEW_INFO_T    IDLE_SCREEN_PREVIEW_INFO_T;
typedef struct tagPICTURE_DATA_TYPE_T PICTURE_DATA_TYPE_T;
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagPICTURE_DATA_TYPE_T {
    SKY_TOKEN               PictureName;
	BUILT_IN_IMAGE_E        wPictureID;
};
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern BYTE g_szChangedName[SKY_FILE_NAME_MAX_LENGTH];
//extern CONST PICTURE_DATA_TYPE_T aPictureList[NUM_ANI_DATA];
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern PICTURE_DATA_TYPE_T * SKY_API GetBuiltInPictureList(void);
extern BOOL SKY_API IsIdleScreenPreview(void);
extern void SKY_API SKA_MainPictureSet(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainIdleDisplaySet(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainMyAnimation(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainMyFileInfo(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MyAniRename(SKY_EVENT_T *pEvent);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_PIC_H_ */
