/*
** ============================================================================
**
** Project: 
**     Graphics
**
** File:
**     eqs_gui.h
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
** 2000-09-19 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_GUI_H_
#define _EQS_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* iniial value of color scheme */
#define INIT_COLOR_SCHEME       CS_STANDARD
/* rosa 2001-09-25 --> @why, what */
#define INIT_COLOR_SCHEME_BACK  CSB_WHITE
#define INIT_COLOR_SCHEME_FORE  CSF_STANDARD
/* <-- */

#define INIT_SCROLLBAR_SIZE 6

#define R_SCROLLBAR_SIZE    g_GUI_Theme.nScrollBarSize

/* 3D color */
#define R_WHITE             g_aColorScheme[g_GUI_Theme.ColorScheme].ThreeDColor.White
#define R_LIGHTGRAY         g_aColorScheme[g_GUI_Theme.ColorScheme].ThreeDColor.LightGray
#define R_GRAY              g_aColorScheme[g_GUI_Theme.ColorScheme].ThreeDColor.Gray
#define R_BLACK             BLACK

/* caption color */
#define CAPTION_FORE_COLOR  g_aColorScheme[g_GUI_Theme.ColorScheme].CaptionColor.Fore
#define CAPTION_BACK_COLOR  g_aColorScheme[g_GUI_Theme.ColorScheme].CaptionColor.Back
/* text color*/
#define TEXT_FORE_COLOR     g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Fore
#define TEXT_BACK_COLOR     g_aColorScheme[g_GUI_Theme.ColorScheme].TextColor.Back
/* select text color */
#define SELECT_FORE_COLOR   g_aColorScheme[g_GUI_Theme.ColorScheme].SelectColor.Fore
#define SELECT_BACK_COLOR   g_aColorScheme[g_GUI_Theme.ColorScheme].SelectColor.Back
/* draw color */
#define FORE_COLOR          g_aColorScheme[g_GUI_Theme.ColorScheme].Color.Fore
#define BACK_COLOR          g_aColorScheme[g_GUI_Theme.ColorScheme].Color.Back
/* popup color */
#define POPUP_R_WHITE                   g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Border.White
#define POPUP_R_LIGHTGRAY               g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Border.LightGray
#define POPUP_R_GRAY                    g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Border.Gray
#define POPUP_CAPTION_R_WHITE           g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Caption.Border.White
#define POPUP_CAPTION_R_LIGHTGRAY       g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Caption.Border.LightGray
#define POPUP_CAPTION_R_GRAY            g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Caption.Border.Gray
#define POPUP_CAPTION_TEXT_FORE_COLOR   g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Caption.Text.Fore
#define POPUP_CAPTION_TEXT_BACK_COLOR   g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Caption.Text.Back
#define POPUP_NORMAL_FORE_COLOR         g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Normal.Fore
#define POPUP_NORMAL_BACK_COLOR         g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Normal.Back
#define POPUP_SELECT_FORE_COLOR         g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Select.Fore
#define POPUP_SELECT_BACK_COLOR         g_aColorScheme[g_GUI_Theme.ColorScheme].PopupColor.Select.Back
/* list color */
#define LIST_R_WHITE                    g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Border.White
#define LIST_R_LIGHTGRAY                g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Border.LightGray
#define LIST_R_GRAY                     g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Border.Gray
#define LIST_CAPTION_R_WHITE            g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Caption.Border.White
#define LIST_CAPTION_R_LIGHTGRAY        g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Caption.Border.LightGray
#define LIST_CAPTION_R_GRAY             g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Caption.Border.Gray
#define LIST_CAPTION_TEXT_FORE_COLOR    g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Caption.Text.Fore
#define LIST_CAPTION_TEXT_BACK_COLOR    g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Caption.Text.Back
#define LIST_NORMAL_FORE_COLOR          g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Normal.Fore
#define LIST_NORMAL_BACK_COLOR          g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Normal.Back
#define LIST_SELECT_FORE_COLOR          g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Select.Fore
#define LIST_SELECT_BACK_COLOR          g_aColorScheme[g_GUI_Theme.ColorScheme].ListColor.Select.Back
/* control color - normal */
#define CN_R_WHITE                      g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Normal.Border.White
#define CN_R_LIGHTGRAY                  g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Normal.Border.LightGray
#define CN_R_GRAY                       g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Normal.Border.Gray
#define CN_TEXT_FORE_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Normal.Text.Fore
#define CN_TEXT_BACK_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Normal.Text.Back
/* control color - disable */
#define CD_R_WHITE                      g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Disable.Border.White
#define CD_R_LIGHTGRAY                  g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Disable.Border.LightGray
#define CD_R_GRAY                       g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Disable.Border.Gray
#define CD_TEXT_FORE_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Disable.Text.Fore
#define CD_TEXT_BACK_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Disable.Text.Back
/* control color - select */
#define CS_R_WHITE                      g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Select.Border.White
#define CS_R_LIGHTGRAY                  g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Select.Border.LightGray
#define CS_R_GRAY                       g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Select.Border.Gray
#define CS_TEXT_FORE_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Select.Text.Fore
#define CS_TEXT_BACK_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Select.Text.Back
/* control color - active */
#define CA_R_WHITE                      g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Active.Border.White
#define CA_R_LIGHTGRAY                  g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Active.Border.LightGray
#define CA_R_GRAY                       g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Active.Border.Gray
#define CA_TEXT_FORE_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Active.Text.Fore
#define CA_TEXT_BACK_COLOR              g_aColorScheme[g_GUI_Theme.ColorScheme].ControlColor.Active.Text.Back



/* TEMP TEMP TEMP */
#define MENU_BACK_COLOR     g_aColorScheme[g_GUI_Theme.ColorScheme].MenuBackColor

#define EQS_CURSOR_BACK_COLOR    EQS_RGB(233,106,0)
#define EQS_CURSOR_FORE_COLOR    EQS_RGB(255,255,255)
/* TEMP TEMP TEMP */



#define BUTTON_WIDTH_AUTO        -1
#define BUTTON_INNER_SPACE       4
#define TEXT_HEIGHT_UPPER_SPACE              1
#define TEXT_HEIGHT_LOWER_SPACE              1

#define BUTTON_WIDTH(pButton)  EQS_TextWidth(pButton->pszText)\
                               + (BUTTON_INNER_SPACE << 2)

#define BUTTON_HEIGHT(pButton)   EQS_TextHeight(pButton->pszText)\
                                 + TEXT_HEIGHT_UPPER_SPACE\
                                 + TEXT_HEIGHT_LOWER_SPACE + 4//6

#define WINDOW_BORDER_SIZE      2
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagOBJECT_TYPE
{
    OT_WINDOW,
    OT_VSCROLL,
    OT_HSCROLL,
    OT_BUTTON
    /*
    OT_MENUBAR,
    OT_BITMAP,
    OT_TEXT,
    OT_TEXTEDIT,
    OT_LIST,
    OT_LINEUP,
    OT_LINEDOWN,
    OT_PAGEUP,
    OT_PAGEDOWN,
    OT_THUMB,
    OT_CHECKBOX,
    */
};
typedef enum tagOBJECT_TYPE         OBJECT_TYPE;

//enum tagWINDOW_STYLE
//{
//    WS_NOBORDER,    /* no border */
//    WS_BORDER,      /* border */
//    WS_CAPTION      /* title (implies the WS_BORDER style) */
//};
//typedef enum tagWINDOW_STYLE        WINDOW_STYLE;

enum tagCOLOR_SCHEME_E {
    CS_STANDARD,
#ifdef FEATURE_EQS_COLOR_256
    CS_GREEN,
    CS_BLUEGREEN,
    CS_VIOLET,
    CS_CARMINE,
    CS_YELLOW,
#elif defined FEATURE_EQS_COLOR_64K 
    /* background color is white */
    CS_RED_WHITE,
    CS_GREEN_WHITE,
    CS_PURPLE_WHITE,
    CS_MARIN_WHITE,
    CS_ORANGE_WHITE,
    /* background color is black */
    CS_STANDARD_BLACK,
    CS_RED_BLACK,
    CS_GREEN_BLACK,
    CS_PURPLE_BLACK,
    CS_MARIN_BLACK,
    CS_ORANGE_BLACK,
#endif /* FEATURE_EQS_COLOR_XXX */
    CS_MAX
};
typedef enum tagCOLOR_SCHEME_E      COLOR_SCHEME_E;

/* rosa 2001-09-25 --> @why, what */
enum tagCOLOR_SCHEME_BACK_E {
    CSB_WHITE,
    CSB_BLACK,
    CSB_MAX
};
typedef enum tagCOLOR_SCHEME_BACK_E      COLOR_SCHEME_BACK_E;

enum tagCOLOR_SCHEME_FORE_E {
    CSF_STANDARD,
    CSF_RED,
    CSF_GREEN,
    CSF_PURPLE,
    CSF_MARIN,
    CSF_ORANGE,
    CSF_MAX
};
typedef enum tagCOLOR_SCHEME_FORE_E      COLOR_SCHEME_FORE_E;
/* <-- */

typedef BYTE                WINDOW_STYLE;
#define WS_NULL             0x00        /* no style */
#define WS_BORDER           0x01
#define WS_CAPTION          0x02
#define WS_FULLSCREEN       0x04
#define WS_POPUP            (WS_BORDER|WS_CAPTION)


/*
struct tagWINDOW_CAPTION
{
    BYTE            *pszTitle;
    COLOR           TextColor;
    COLOR           BarColor;
};
typedef struct tagWINDOW_CAPTION           WINDOW_CAPTION;
*/


enum tagSCROLL_BAR_TYPE {
    SBT_HORIZ,
    SBT_VERT
};
typedef enum tagSCROLL_BAR_TYPE     SCROLL_BAR_TYPE;

typedef struct tagOBJECT_HEADER     OBJECT_HEADER;
typedef struct tagWINDOW            WINDOW;
typedef struct tagSCROLL_BAR_INFO   SCROLL_BAR_INFO;
typedef struct tagSCROLL_BAR        SCROLL_BAR;
typedef struct tagBUTTON            BUTTON;

typedef struct tagCOLORS_T          COLORS_T;
typedef struct tagTHREED_COLOR_T    THREED_COLOR_T;
typedef struct tagCOLOR_SCHEME_T    COLOR_SCHEME_T;
typedef struct tagGUI_THEME_T       GUI_THEME_T;
typedef struct tagTEXT_COLOR_T      TEXT_COLOR_T;
typedef struct tagLIST_COLOR_T      LIST_COLOR_T;
typedef struct tagCONTROL_COLOR_T   CONTROL_COLOR_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagOBJECT_HEADER
{
    OBJECT_TYPE Type;
    void        *pNext;
};

struct tagWINDOW
{
    OBJECT_HEADER   Header;
    WINDOW_STYLE    Style;
//    WINDOW_CAPTION  Caption;
    BYTE            *pszCaption;
    RECT            Frame;
    BYTE            *pbBackImage;
};

struct tagSCROLL_BAR_INFO {
    INT16 nMin;         /* the minimun scrolling position */
    INT16 nMax;         /* the maximum scrolling position */
    INT16 nPageSize;    /* the page size. A scroll bar uses this value to 
                           determine the appropriate size of 
                           the proportional scroll box */
    INT16 nPos;         /* the position of the scroll box */
};

struct tagSCROLL_BAR {
    OBJECT_HEADER       Header;
    SCROLL_BAR_INFO     Info;
    RECT                Frame;
};

struct tagBUTTON {
    OBJECT_HEADER       Header;
    BYTE                *pszText;
    BOOL                fActive;
    RECT                Frame;
};

struct tagCOLORS_T {
    COLOR Fore;
    COLOR Back;
};

struct tagTHREED_COLOR_T {
    COLOR White;
    COLOR LightGray;        /* background = text background */
    COLOR Gray;
//    COLOR Black;
};

struct tagTEXT_COLOR_T {
    THREED_COLOR_T  Border;
    COLORS_T        Text;
};

struct tagLIST_COLOR_T {
    THREED_COLOR_T  Border;
    TEXT_COLOR_T    Caption;
    COLORS_T        Normal;
    COLORS_T        Select;
};

struct tagCONTROL_COLOR_T {
    TEXT_COLOR_T    Normal;
    TEXT_COLOR_T    Disable;
    TEXT_COLOR_T    Select;
    TEXT_COLOR_T    Active;
};

struct tagCOLOR_SCHEME_T {
    THREED_COLOR_T  ThreeDColor;    /* the relative colors for 3d effect */
    COLORS_T        CaptionColor;   /* caption font color and background color */
    COLORS_T        TextColor;      /* font color and background color */
    COLORS_T        SelectColor;    /* select item color in the full screen list */
    COLORS_T        Color;          /* draw color */

    LIST_COLOR_T    PopupColor;
    LIST_COLOR_T    ListColor;

    CONTROL_COLOR_T ControlColor;
};

struct tagGUI_THEME_T {
    COLOR_SCHEME_E  ColorScheme;
    FONT_TYPE       FontType;
    INT8            nScrollBarSize;    /* the size of scroll bar */
};

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern ROM COLOR_SCHEME_T  g_aColorScheme[];
extern GUI_THEME_T         g_GUI_Theme;

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/* ------
** Window
*/
EQS_DCL VOID        EQS_API EQS_PreOpenWindow(WINDOW *pWin, 
                                              WINDOW_STYLE Style, 
//                                              WINDOW_CAPTION *pCaption);
                                              BYTE *pszCaption);

EQS_DCL void EQS_API EQS_SetCaptionHeight(WINDOW *pWin, INT8 nCaptionHeight);
EQS_DCL INT8 EQS_API EQS_GetCaptionHeight(WINDOW *pWin);

EQS_DCL EQS_RESULT  EQS_API EQS_OpenWindow(WINDOW *pWin, RECT *pRect, 
                                           BOOL fBackSave);
EQS_DCL VOID        EQS_API EQS_CloseWindow(WINDOW *pWin);

/* ----------
** Scroll bar
*/
EQS_DCL VOID        EQS_API EQS_AddScrollBar(WINDOW* pWin, 
                                             SCROLL_BAR* pScrollBar,
                                             SCROLL_BAR_TYPE Type,
                                             COORD x, COORD y, INT16 nLength,
                                             INT16 nMin, INT16 nMax, 
                                             INT16 nPageSize, 
                                             INT16 nPos);
EQS_DCL void        EQS_API EQS_Draw3DBox(COORD xLeft, COORD yTop, 
                                          COORD xRight, COORD yBottom);
EQS_DCL VOID        EQS_API EQS_DrawScrollBar(SCROLL_BAR* pScrollBar);
EQS_DCL SCROLL_BAR_INFO* 
                    EQS_API EQS_GetScrollBarInfo(SCROLL_BAR* pScrollBar, 
                                                 SCROLL_BAR_INFO* pInfo);
EQS_DCL VOID        EQS_API EQS_SetScrollBarInfo(SCROLL_BAR* pScrollBar, 
                                                 SCROLL_BAR_INFO* pInfo, 
                                                 BOOL fRedraw);
/* ------
** Button
*/
EQS_DCL void        EQS_API EQS_AddButton(WINDOW* pWin, BUTTON* pButton,
                                          COORD x, COORD y, INT8 nWidth);
EQS_DCL void        EQS_API EQS_Draw3DButton(COORD xLeft, COORD yTop, 
                                             COORD xRight, COORD yBottom, 
                                             BOOL fFocus);
EQS_DCL void        EQS_API EQS_DrawButton(BUTTON *pButton, BOOL fFocus);


/* -----
** Theme
*/
EQS_DCL void EQS_API EQS_SetTheme(COLOR_SCHEME_E ColorScheme, 
                                  FONT_TYPE FontType,
                                  INT8 nScrollBarSize);
EQS_DCL COLOR_SCHEME_E EQS_API EQS_GetCurrentColorScheme(void);
EQS_DCL void EQS_API EQS_SetCurrentColorScheme(COLOR_SCHEME_E ColorScheme);
EQS_DCL FONT_TYPE EQS_API EQS_GetCurrentFontType(void);
EQS_DCL void EQS_API EQS_SetCurrentFontType(FONT_TYPE FontType);
EQS_DCL INT8 EQS_API EQS_GetCurrentScrollSize(void);
EQS_DCL void EQS_API EQS_SetCurrentScrollSize(INT8 nScrollBarSize);
EQS_DCL void EQS_API EQS_Draw3DReverseBorder(COORD x, COORD y, COORD dx, COORD dy);
EQS_DCL void EQS_API EQS_Draw3DBorder(COORD x, COORD y, COORD dx, COORD dy);

extern void EQS_API EQS_DrawTitlePannel(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_GUI_H_ */
