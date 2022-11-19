/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_tedt.h
**
** Description:
**     EQUS Text Editor header file
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-10-13 woodstok     Created.
**
** ============================================================================
*/

#ifndef _EQC_TEDT_H_
#define _EQC_TEDT_H_

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
/* TEXT_EDIT Style */
#define CS_TEXT_EDIT_PASSWORD           0x0001  /* 0000 0000 0000 0001 */
#define CS_TEXT_EDIT_VSCROLL            0x0002  /* 0000 0000 0000 0010 */
#define CS_TEXT_EDIT_HIDDEN             0x0008  /* 0000 0000 0000 1000 */
#define CS_TEXT_EDIT_CENTER_ALIGN       0x0010  /* 0000 0000 0001 0000 */
#define CS_TEXT_EDIT_RIGHT_ALIGN        0x0020  /* 0000 0000 0010 0000 */
#define CS_TEXT_EDIT_BORDER             0x0040  /* 0000 0000 0100 0000 */
#define CS_TEXT_EDIT_READONLY           0x0080  /* 0000 0000 1000 0000 */
#define CS_TEXT_EDIT_NO_CML             0x0100
#define CS_TEXT_EDIT_ORING              0x0200
#define CS_TEXT_EDIT_USE_SKFONT         0x0400
#define CS_TEXT_EDIT_ALONE              0x0800
#define CS_TEXT_EDIT_NO_LCML            0x1000
#define CS_TEXT_EDIT_NO_CCML            0x2000

#define CS_TEXT_EDIT_DEFAULT            CS_TEXT_EDIT_BORDER

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagTEXT_EDIT_MODE_E
{
    TEM_ENGLISH_UPPER,
    TEM_ENGLISH_LOWER,
    TEM_SYMBOL,
    TEM_NUMBER,
    TEM_KOREAN,
    TEM_MAX
};
typedef enum tagTEXT_EDIT_MODE_E        TEXT_EDIT_MODE_E;

typedef struct tagEDIT_MODE_SELECT_T EDIT_MODE_SELECT_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

struct tagEDIT_MODE_SELECT_T {
    UINT8   nModeNum;
    UINT8   CurrentMode;
    TEXT_EDIT_MODE_E    ModeSequence[TEM_MAX];
	BOOL	fEnableDigDown;
};

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

extern HCONTROL EQS_API EQC_TextEdit(COORD x, COORD y, UINT8 nChar, UINT8 nLine, 
                              TEXT_EDIT_MODE_E Mode, UINT16 nMaxLen, 
                              BYTE *pszStr, UINT16 nInitPos);

extern UINT16   EQS_API EQC_GetTextEditData(HCONTROL hControl, BYTE *pszBuf);
extern UINT16   EQS_API EQC_GetTextEditLine(HCONTROL hControl);
extern void   EQS_API EQC_FitTextEditLine(HCONTROL hControl);

//redstar  11.04
extern UINT16   EQS_API EQC_SetTextEditData(HCONTROL hControl, BYTE *pszBuf);
extern void     EQS_API EQC_SetFormatString(HCONTROL hControl, BYTE *pszFormat);
extern void EQC_SetFormatLength(HCONTROL hControl, UINT8 nLen);
extern void EQC_SetTextEditModeSequence(HCONTROL hControl, EDIT_MODE_SELECT_T modeSeq);

extern void EQC_SetSKYHangulMode(BOOL fSkyHangul);

extern UINT16 EQC_SetTextEditDataPos(HCONTROL hControl, BYTE *pszBuf, UINT16 nPos);
extern void EQC_SetTextEditPos(HCONTROL hControl, UINT16 nPos);

extern UINT16 EQC_GetTextEditLen(HCONTROL hControl);
extern UINT16 EQC_GetTextEditPos(HCONTROL hControl);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_TEDIT_H_ */
