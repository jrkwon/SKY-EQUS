/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_scrl.h
**
** Description:
**       
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-10-12   redstar       Created.
**
** ===========================================================================
*/

#ifndef _EQC_SCRL_H_
#define _EQC_SCRL_H_

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
/* CONTROL_STYLE : bit flag */
#define CS_SCROLL_LIST_ENUMBERATED      0x0001   /* 0000 0000 0000 0001 */
#define CS_SCROLL_LIST_BORDER           0x0002   /* 0000 0000 0000 0010 */
#define CS_SCROLL_LIST_SCROLL_BAR       0x0004   /* 0000 0000 0000 0100 */
#define CS_SCROLL_LIST_SWAP             0x0008   /* 0000 0000 0000 1000 */
#define CS_SCROLL_LIST_COMBO            0x0010   /* 0000 0000 0001 0000 */
#define CS_SCROLL_LIST_CHECK_BOX        0x0020   /* 0000 0000 0010 0000 */
#define CS_SCROLL_LIST_LEFT_ALIGN       0x0040
#define CS_SCROLL_LIST_COMBO_LIST       0x0040
#define CS_SCROLL_LIST_NO_CML           0x0080
#define CS_SCROLL_LIST_CENTER_ALIGN     0x0100
#define CS_SCROLL_LIST_WRAP             0x0200
#define CS_SCROLL_LIST_FIXED            0x0400
#define CS_SCROLL_LIST_MAXIMIZE         0x0800
#define CS_SCROLL_LIST_HMAXIMIZE        0x1000
#define CS_SCROLL_LIST_POPUP            0x2000
#define CS_SCROLL_LIST_STATUS_BAR       0x4000
#define CS_SCROLL_LIST_TAB              0x8000

        
#define CS_SCROLL_LIST_DEFAULT          (CS_SCROLL_LIST_BORDER| CS_SCROLL_LIST_SCROLL_BAR)


#define CS_SCROLL_LIST_NO_ARROW_TITLE      0
#define CS_SCROLL_LIST_LEFT_ARROW_TITLE    1
#define CS_SCROLL_LIST_RIGHT_ARROW_TITLE   2
#define CS_SCROLL_LIST_BOTH_ARROW_TITLE    3


/*
**----------------------------------------------------------------------------
**  Type Definitions
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
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
extern HCONTROL EQS_API EQC_ScrollList(COORD x, COORD y, UINT8 nChar, 
                                       UINT8 nViewLine, BYTE **ppszInit, 
                                       UINT8 nPhysicalLine, WORD nMaxLine,
                                       UINT8 nIndex);
extern HCONTROL EQS_API EQC_ScrollCombo(COORD x, COORD y, UINT8 nChar, UINT8 nViewLine, 
                                        BYTE **ppszInit, UINT8 nPhysicalLine, 
                                        WORD nMaxLine, UINT8 nIndex);

extern UINT8 EQS_API  EQS_API EQC_GetScrollPhysicalIndex(HCONTROL  hControl);
extern void  EQS_API  EQC_SetScrollPhysicalIndex(HCONTROL  hControl, UINT8 nIndex);
extern void  EQS_API  EQC_SetScrollLogicalIndex(HCONTROL hControl, WORD iIndex);
extern WORD  EQS_API  EQC_GetScrollLogicalIndex(HCONTROL hControl);
extern UINT8 EQS_API  EQC_GetScrollCursor(HCONTROL hControl);
extern BOOL  EQS_API  EQC_SetScrollCursor(HCONTROL hControl, UINT8 nCursor);


extern BYTE* EQS_API  EQC_GetScrollData(HCONTROL hControl, UINT8 nPos);
extern void  EQS_API  EQC_SetScrollData(HCONTROL hControl, UINT8 nIndex, \
                                            BYTE* pszStr);

extern INT16 EQS_API  EQC_DeleteScrollData(HCONTROL  hControl, \
                                               UINT8 nLogicalIndex);

extern INT16 EQS_API  EQC_AddScrollData(HCONTROL hControl,UINT8 nIndex, \
                                       BYTE* pszStr, BOOL fIncreaseViewLine);



extern UINT8 EQS_API  EQC_GetScrollViewLine(HCONTROL hControl);
extern void  EQS_API  EQC_SetScrollViewLine(HCONTROL hControl, UINT8 nLine);

extern UINT8 EQS_API  EQC_GetScrollDataLine(HCONTROL hControl);

extern WORD  EQS_API  EQC_GetScrollMaxDataLine(HCONTROL hControl);
extern void  EQS_API  EQC_SetScrollMaxDataLine(HCONTROL hControl, UINT8 nLogicalLine);

extern void  EQS_API  EQC_SetScrollComboSize(HCONTROL hControl, UINT8 nChar);
extern UINT8 EQS_API  EQC_GetScrollComboSize(HCONTROL hControl);
extern void  EQS_API  EQC_SetScrollList(HCONTROL hControl, UINT8 nPos);
extern void  EQS_API  EQC_ResetScrollList(HCONTROL hControl, UINT8 nPos);
extern BOOL  EQS_API  EQC_GetScrollList(HCONTROL hControl, UINT8 nPos);

extern void  EQS_API  EQC_SetScrollListHandle(HCONTROL hControl, UINT8 hList);
extern UINT8 EQS_API  EQC_GetScrollListHandle(HCONTROL hControl);

extern void  EQS_API  EQC_SetScrollTitle(HCONTROL hControl, BYTE* pszStr);
extern BYTE* EQS_API  EQC_GetScrollMemory(HCONTROL hControl);

extern BYTE* EQS_API  EQC_GetScrollTitle(HCONTROL hControl);
extern void  EQS_API  EQC_ShowScrollTitle(HCONTROL hControl, BOOL fShow);
extern BOOL  EQS_API  EQC_IsShowScrollTitle(HCONTROL hContol);
extern void  EQS_API  EQC_FreeScrollMemory(HCONTROL hControl, BOOL fFree);
extern void  EQS_API  EQC_ScrollAdjust(HCONTROL hControl);
extern void  EQS_API  EQC_SetLogicalList(HCONTROL hControl, BOOL fLogical);

extern void  EQS_API  EQC_RePositionScrollList(HCONTROL hControl);
extern UINT8 EQS_API  EQC_GetScrollDisplayLine(void);

extern void  EQS_API  EQC_SetScrollStyle(HCONTROL hControl, WORD StyleA);
extern BOOL  EQS_API  EQC_SetScrollAccelKey(HCONTROL hControl,UINT8 nIndex, ACCEL_KEY_T AKey);
extern ACCEL_KEY_T    EQS_API  EQC_GetScrollAccelKey(HCONTROL hControl,UINT8 nIndex);
extern void  EQS_API  EQC_SetScrollStatusBarText(HCONTROL hControl, BYTE* szStr);
extern BYTE* EQS_API  EQC_GetScrollStatusBarText(HCONTROL hControl); 


extern BOOL EQC_SetScrollTab(HCONTROL hControl, UINT8 nTabSize, UINT8 nViewTab,\
							 BOOL fEqualSize, UINT8 nIndex);
extern BOOL EQC_SetScrollTabData(HCONTROL hControl, WORD* pwDataSize, BYTE** ppText);

extern UINT8 EQC_GetScrollTabSize(HCONTROL hControl);
extern UINT8 EQC_GetScrollCurrentTab(HCONTROL hControl);
extern WORD  EQC_GetScrollTabIndex(HCONTROL hControl, UINT8 nTab);
extern UINT8 EQC_GetScrollTabViewLine(HCONTROL hControl);
extern WORD  EQC_GetScrollTabDataSize(HCONTROL hControl, UINT8 nTab);
extern WORD  EQC_GetScrollCurrentTabDataSize(HCONTROL hControl);
extern UINT8 EQC_GetScrollTabPos(HCONTROL hControl, WORD nIndex);

extern BOOL  EQC_SetScrollTabSize(HCONTROL hControl, UINT8 nTabSize);
extern BOOL  EQC_SetScrollCurrentTab(HCONTROL hControl, UINT8 nCurrentTab);
extern BOOL  EQC_SetScrollTabViewLine(HCONTROL hControl, UINT8 nTabViewLine);
extern BOOL  EQC_SetScrollTabDataSize(HCONTROL hControl, UINT8 nTab, WORD nDataSize);
extern BOOL  EQC_SetScrollCurrentTabDataSize(HCONTROL hControl, WORD nDataSize);

extern void   EQC_SetScrollTitleStyle(HCONTROL hControl, UINT8 TStyle);
extern UINT8  EQC_GetScrollTitleStyle(HCONTROL hControl);


extern BOOL EQC_IsShowScrollListBar(HCONTROL hControl);
extern void EQC_SetShowScrollBar(HCONTROL hControl, BOOL fShow);
extern void EQC_DeActiveScrollList(HCONTROL hControl);

extern BOOL EQC_IsScrollListAccelKeySetting(HCONTROL hControl);
extern BOOL EQC_SetScrollListAccelKeySetting(HCONTROL hControl, BOOL fSetting);

extern void EQC_SetScrollFlowMode(HCONTROL hControl, BOOL fSetting);
extern BOOL EQC_GetScrollFlowMode(HCONTROL hControl);

#ifdef __cplusplus
}
#endif

#endif // _EQC_SCRL_H_ 
/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
