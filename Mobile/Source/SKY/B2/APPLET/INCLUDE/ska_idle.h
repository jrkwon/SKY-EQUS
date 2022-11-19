/*
** ============================================================================
**
** Project: 
**     Athens
**
** File:
**     eqa_idle.h
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
** 2000-07-21 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKA_IDLE_H_
#define _SKA_IDLE_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "cm.h" /* cm_srv_state_e_type */
//#include "eqa_call.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define COMMAND_LINE_HEIGHT             16

/* BBIG_ASCII_FONT_SIZE in eqs_grph.c */
#define BBIG_ASCII_WIDTH                8 //10 
#define BBIG_ASCII_HEIGHT               18 //20

#define DIAL_DIGIT_LINE                 2//3
#define DIAL_DIGIT_LENGTH_PER_LINE      13/* 011-1234-1234 *///(SKY_SCREEN_WIDTH / BBIG_ASCII_WIDTH) /* 15 */

#define DIAL_DIGIT_POS_X                13//0
/* screen height - command line height - (big ASCII (10x21) height * lines */
#define sPROPER_VALUE                   2
#define DIAL_DIGIT_POS_Y                82//((SKY_SCREEN_HEIGHT-EQS_SWINDOW_HEADER_HEIGHT)-(BBIG_ASCII_HEIGHT*DIAL_DIGIT_LINE)-COMMAND_LINE_HEIGHT-sPROPER_VALUE) 

#define NUMBER_EDIT_BACK_COLOR          EQS_RGB(177,251,252)     /* image color */

/* Annunciators line이 있는 경우 */
#define GET_IMG_X_IN_IDLE(Bmp)          ((SKY_SCREEN_WIDTH-((Bmp).Header.xWidth))/2)
#define GET_IMG_Y_IN_IDLE(Bmp)          ((SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT)-((Bmp).Header.Size.yHeight))/2)

/* 타이틀이 있는 경우 */
#define GET_IMG_X_TITLE(Bmp)            ((SKY_SCREEN_WIDTH-((Bmp).Header.xWidth))/2)
#define GET_IMG_Y_TITLE(Bmp)            ((SKY_SCREEN_HEIGHT-(SKY_TITLE_HEIGHT+SKY_COMMANDLINE_HEIGHT)-((Bmp).Header.Size.yHeight))/2)

/* 전체 화면을 모두 사용하는 경우 */
#define GET_IMG_X_FULLSCREEN(Bmp)       ((SKY_SCREEN_WIDTH-((Bmp).Header.xWidth))/2)
#define GET_IMG_Y_FULLSCREEN(Bmp)       ((SKY_SCREEN_HEIGHT-((Bmp).Header.Size.yHeight))/2)


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagPBOOKREAD_E{
    PB_READ_OK              = 0,
    PB_SECRET_OK            ,
    PB_READ_NO_BOOK_INFO    ,
    PB_READ_NO_GROUP_INDEX  ,
    PB_READ_NO_GROUP_INFO   
};



/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
typedef enum tagPBOOKREAD_E  READPBOOK_E;



/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern EDITORINFO_T     g_NumEditInfo;         /* declared in ska_idle.c  */
extern EDITORINFO_T     g_AlphaEditInfo;       /* declared in ska_idle.c  */
extern BOOL             g_fPausedMode;
extern ROM  SKY_TOKEN   g_aDayOfWeek[7];


/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API DisplayDateTimeInSubLCD(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute);
extern BOOL SKY_API DisplayCurrentDateTimeInSubLCD(BOOL fDrawNow);
extern void SKY_API SKA_DispClock(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_SavePhoneNumber(SKY_EVENT_T *pEvent);

extern void SKY_DisplayDateTimeWithPic(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute);
extern void SKY_DisplayDateTime(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute);
extern void SpeedDial( UINT8 nShortDial );

extern void SKY_API SKA_MainIdle(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_IdleDebugScreen(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_MissedCall(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_Dialing(SKY_EVENT_T* pEvent);

extern void SKY_API ClearNumEditInfo(void);         //defined in ska_idle.c
extern void SKY_API ClearAlphaEditInfo(void);       //defined in ska_idle.c

extern BOOL SKY_API SKY_GetActiveAlarmFlag(void);
/*
    MACRO (input/output) for gNumEditInfo                         hjcho 00.10.25
*/
extern BYTE* GetNumEditInfoBuf(void);
extern void  SetNumEditInfoBuf(BYTE* pbBuffer, UINT8 nLength);

#define GET_NUMEDIT                      (  g_NumEditInfo                        )
#define SET_NUMEDIT(NumEdit)             (  g_NumEditInfo = NumEdit              )

#define GET_NUMEDIT_SECRET               (  g_NumEditInfo.fSecret                )
#define SET_NUMEDIT_SECRET(fValue)       (  g_NumEditInfo.fSecret = fValue       )

#define GET_NUMEDIT_BUFFER               (  GetNumEditInfoBuf()                 )
#define SET_NUMEDIT_BUFFER(pBuf, nLen)   (  SetNumEditInfoBuf(pBuf, nLen)       )

#define GET_NUMEDIT_POS                  (  g_NumEditInfo.nPos                   )
#define SET_NUMEDIT_POS(nValue)          (  g_NumEditInfo.nPos = nValue          )
/*
    End MACRO definitions for gNumEditInfo
*/


/*
    MACROS(input/output) for gAlphaEditInfo                          hjcho 00.10.25
*/
extern BYTE* GetAlphaEditInfoBuf(void);
extern void  SetAlphaEditInfoBuf(BYTE* pbBuffer, UINT8 nLength);

#define GET_ALPHAEDIT                    ( g_AlphaEditInfo                         )
#define SET_ALPHAEDIT(AlphaEdit)         ( g_AlphaEditInfo = AlphaEdit             )

#define GET_ALPHAEDIT_ACT                (  g_AlphaEditInfo.fAct                   )
#define SET_ALPHAEDIT_ACT(fValue)        (  g_AlphaEditInfo.fAct = fValue          )

#define GET_ALPHAEDIT_SECRET             (  g_AlphaEditInfo.fSecret                )
#define SET_ALPHAEDIT_SECRET(fValue)     (  g_AlphaEditInfo.fSecret = fValue       )

#define GET_ALPHAEDIT_BUFFER             (  GetAlphaEditInfoBuf()                 )
#define SET_ALPHAEDIT_BUFFER(pBuf, nLen) (  SetAlphaEditInfoBuf(pBuf, nLen)       )

#define GET_ALPHAEDIT_POS                (  g_AlphaEditInfo.nPos                   )
#define SET_ALPHAEDIT_POS(nValue)        (  g_AlphaEditInfo.nPos = nValue          )

void    SKY_API InitMessageQueue(void);
void    SKY_API DisplayDateTimeInSubLCD(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute);
BOOL    SKY_API DisplayCurrentDateTimeInSubLCD(BOOL fDrawNow);
void    SKY_API SKY_DisplayDateTime(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute);
void    SKY_API SKY_DisplayDateTimeWithPic(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute);
BOOL    SKY_API DisplayDateTime(BOOL fDisplayRightNow);
void    SKY_API ClearNumEditInfo(void);
void    SKY_API ClearAlphaEditInfo(void);
READPBOOK_E IsConfidentialGroup(BYTE* aPhoneNumber);
BYTE*   SKY_API GetNumEditInfoBuf(void);
void    SKY_API ClearNumEditInfo(void);
void    SKY_API ClearAlphaEditInfo(void);
READPBOOK_E SKY_API IsConfidentialGroup(BYTE* aPhoneNumber);
BYTE*   SKY_API GetNumEditInfoBuf(void);
void    SKY_API SetNumEditInfoBuf(BYTE* pbBuffer, UINT8 nLength);
BYTE*   SKY_API GetAlphaEditInfoBuf(void);
void    SKY_API SetAlphaEditInfoBuf(BYTE* pbBuffer, UINT8 nLength );
void    SKY_API MakeHypenedPhoneNumber(BYTE abSource[], BYTE abTarget[]);
void    SKY_API SetRealPhoneNumberLength(UINT8 nLength);
UINT8   SKY_API GetRealPhoneNumberLength(void);
void    SKY_API SKA_MainIdleScreen(SKY_EVENT_T* pEvent);
void    SKY_API SpeedDial(UINT8 nShortDial);



#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_IDLE_H_ */
