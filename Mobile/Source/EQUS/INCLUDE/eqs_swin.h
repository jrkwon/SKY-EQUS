/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqs_swin.h
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
** 2000-10-04 girin     Created.
**
** ============================================================================
*/

#ifndef _EQS_SWIN_H_
#define _EQS_SWIN_H_

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
// Slide Window Attribute.
#define WA_NONE                         0x0000
#define WA_SCROLL_BAR                   0x0001
#define WA_3D_FRAME                     0x0002
#define WA_SHOW_HEADER                  0x0004
#define WA_SHOW_CML                     0x0008
#define WA_DEFAULT                      (WA_SHOW_CML)



/* ----------- old 
#define DEF_CML_WIDTH          120
#define DEF_CML_HEIGHT         16
#define DEF_CML_BPP            2
---------------- */
#ifdef CONFIG_EQS_COLOR_64K

#define DEF_CML_WIDTH           128 /* screen width */
#define DEF_CML_HEIGHT          16  /* command line height */
#define DEF_CML_BPP              8   /* 256 color */

#define EQS_SWINDOW_HEADER_X            0
#define EQS_SWINDOW_HEADER_Y            0

#define EQS_SWINDOW_ANNUNCIATOR_HEIGHT  16
#define EQS_SWINDOW_HEADER_HEIGHT       19 //14
#define EQS_SWINDOW_HEADER_MAX_NCHAR    18 // Max. character num.

#define EQS_SWINDOW_TOP_MARGIN           2
#define EQS_SWINDOW_BOTTOM_MARGIN        3

#else

#define DEF_CML_WIDTH           120 /* screen width */
#define DEF_CML_HEIGHT          13  /* command line height */
#define DEF_CML_BPP             8   /* 256 color */

#define EQS_SWINDOW_HEADER_X            0
#define EQS_SWINDOW_HEADER_Y            0
#define EQS_SWINDOW_HEADER_HEIGHT       15 //14
#define EQS_SWINDOW_HEADER_MAX_NCHAR    20 // Max. character num.

#endif

#define SWINDOW_DUMMY_DEFINE_TO_FACIFY_ARM_COMPILER         1

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

// SWindow is a part of EQUS core, so sould it export its data structures 
// to external world, the dwellers in ET use this SWindow facilities.

typedef struct tagCML_ITEM_T            CML_ITEM_T;
typedef struct tagCML_INFO_T            CML_INFO_T;
typedef struct tagCML_T                 CML_T;

struct tagCML_ITEM_T
{
    ROM INT             nSoftKeyEvent;
    ROM INT             nBitmapIndex;
};

struct tagCML_T
{
    ROM CML_ITEM_T          LeftCML;
    ROM CML_ITEM_T          CenterCML;
    ROM CML_ITEM_T          RightCML;

    //ROM INT                 EventForF1;
    //ROM INT                 EventForCenter;
    //ROM INT                 EventForF2;
};


struct tagCML_INFO_T
{
    ROM CML_T               *pCommandLine[SWINDOW_DUMMY_DEFINE_TO_FACIFY_ARM_COMPILER];
    ROM UINT8               nNumCML; // the number of CML in (*pCommandLine) array.

//    ROM BITMAP              *pCMLBitMap; // BitMap info. contains the width and height
//                                         // of one CML.

    ROM BITMAP              *pLeftBitMap; // BitMap info. contains the width and height
                                         // of one CML.
    ROM BITMAP              *pCenterBitMap; // BitMap info. contains the width and height
                                         // of one CML.
    ROM BITMAP              *pRightBitMap; // BitMap info. contains the width and height
                                         // of one CML.
};

enum tagIOC_CML_E {
    IOC_CML_NONE = 0,
    
    IOC_CML_BLANK_LEFT,
    IOC_CML_CANCEL_LEFT,
    IOC_CML_POPUP_MENU_CANCEL_LEFT,
	IOC_CML_KOREAN_AM_LEFT,
	IOC_CML_KOREAN_PM_LEFT,
	IOC_CML_AM_LEFT,
	IOC_CML_PM_LEFT,	
	IOC_CML_LUNAR_LEFT,
	IOC_CML_SUN_LEFT,
	IOC_CML_NEXT_LEFT,
    
    IOC_CML_BLANK_CENTER,
    IOC_CML_SELECT_CENTER,
    IOC_CML_DONE_CENTER,
	IOC_CML_SETUP_CENTER,
	IOC_CML_UNSETUP_CENTER,
	IOC_CML_INPUT_CENTER,
	IOC_CML_MODIFY_CENTER,

    IOC_CML_BLANK_RIGHT,
    IOC_CML_KOREAN_RIGHT,
    IOC_CML_CAPITAL_ENGLISH_RIGHT,
    IOC_CML_ENGLISH_RIGHT,
    IOC_CML_NUMBER_RIGHT,
    IOC_CML_SYMBOL_RIGHT,

    IOC_CML_MAX
};

typedef enum tagIOC_CML_E      IOC_CML_E;

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
extern void     EQS_API EQS_InitCMLInfo(ROM CML_INFO_T* pCMLInfo);
extern void     EQS_API EQS_ProcessSWindowEvent(EQS_EVENT *pInEvent);
extern void     EQS_API EQS_ProcessCMLEvent(EQS_EVENT *pInEvent);
extern void     EQS_API EQS_DrawIOCCML(IOC_CML_E IOCCML);
extern void     EQS_API EQS_UpdateIOCCML(void);
extern BOOL     EQS_API EQS_DeserveEventByCML(EQS_EVENT_ID EventID);
extern UINT8    EQS_API EQS_GetSWindowHeaderHeight(void);
extern UINT8    EQS_API EQS_GetCMLHeight(void);

extern  void    EQS_DrawCmdLine(UINT8 nCML);

extern void EQS_API EQS_InitSWindowHeader(void (*pfnDrawSWindowHeader)(void));

    
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_SWIN_H_ */
