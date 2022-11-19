/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_code.h
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
** 4333-12-21 white     Created.
**
** ============================================================================
*/

#ifndef _SKA_CODE_H_
#define _SKA_CODE_H_

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
#define BOOKCODE_IN_CALL            1
#define BOOKCODE_NEW_IN_LIST        3
#define BOOKCODE_OLD_IN_LIST        4
#define BOOKCODE_VIEW_IN_LIST       5
#define BOOKCODE_SEARCH_IN_IDLE     6
#define BOOKCODE_RETURN_SEARCHING   7
#define BOOKCODE_SPEED_DIAL         8
#define BOOKCODE_IN_MO              9
#define BOOKCODE_IN_PHOTOMAIL       10
#define BOOKCODE_IN_EMAIL           11
#define BOOKCODE_IN_DATASRV         12
#define BOOKCODE_IN_IRDA            13
#define BOOKCODE_IN_CALL_LIST       14//
#define BOOKCODE_IN_DETAIL_LIST     15//
#define BOOKCODE_IN_DIAL            16
#define BOOKCODE_IN_SCHEDULE        17
#define BOOKCODE_IN_VM              18
#define BOOKCODE_IN_DIALING         19
#define BOOKCODE_SEARCH_IN_CBELL    20

#define RECCODE_IN_CALL         1

#define SMSCODE_INIT                        1
#define SMSCODE_KEY                         2
#define SMSCODE_MESSAGE                     3
#define SMSCODE_IN_LIST                     4
#define SMSCODE_IN_DETAIL_LIST              5//
#define SMSCODE_IN_VIEW_CONTENT             6   //SKA_ViewBookContent
#define SMSCODE_IN_VIEW_SEARCH_NAME         7   //SKA_ViewSearchName
#define SMSCODE_IN_VIEW_SEARCH_NUM          8    //SKA_ViewSearchNumber
#define SMSCODE_IN_VIEW_BOOK_LIST           9       //SKA_ViewBookList
#define SMSCODE_IN_VIEW_BOOK_LIST_EMAIL     10

#define MENUCODE_IN_CALL    1
#define INTERCODE_IN_CALL   1

#define NUM_CODE_4          4


#define NATE_CODE_CHOOSE_MENU  100

#define PASS_X      (SKY_SCREEN_WIDTH - (EQS_CharWidth('A') * NUM_CODE_4)) / 2 //55//44//43
#define PASS_Y      80//96//65//65//69//40 + 5
#define IMAGE_X     15//16
#define IMAGE_Y     28//TEXT_Y

enum tag_PASS_INPUT_E {
    PI_PASSWORD_IMAGE,
    //PI_PASSWORD_TITLE,
    PI_PASSWORD_EDIT,
    PI_MAX
};    
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
**  Variable Declarations
**---------------------------------------------------------------------------
*/

extern  UINT8 IS_PLAY;
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API SKA_MainAllCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainInterCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainPbookCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainMenuCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainStackCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainRecCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainMemoCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainCameraCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainNtopCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainSmsCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainSmsCode2(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainSystemSet(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainPbookCodeInIrda(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainMySelfInfoCode(SKY_EVENT_T *pEvent);
void SKY_API SKA_MainSelfInfoCode4View(SKY_EVENT_T *pEvent);

extern VOID SKY_API SKA_DeleteAllMsgCode(SKY_EVENT_T *pEvent);

#ifdef FEATURE_SKTT_EQUS_LOCATION
extern VOID SKY_API SKA_LocationCode(SKY_EVENT_T *pEvent);
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

extern void SKY_API TextOutInputCodeMessage(void);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_CODE_H_ */
