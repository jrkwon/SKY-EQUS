/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_nedt.h
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
** 2000-10-17 woodstok     Created.
**
** ============================================================================
*/

#ifndef _EQC_NEDT_H_
#define _EQC_NEDT_H_

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
#define CS_NUMBER_EDIT_BORDER               0x0001     /* 0000 0000 0000 0001 */
#define CS_NUMBER_EDIT_RIGHT2LEFT           0x0002     /* 0000 0000 0000 0010 */
#define CS_NUMBER_EDIT_CURSOR               0x0004     /* 0000 0000 0000 0100 */
#define CS_NUMBER_EDIT_PLAIN_NUM            0x0008     /* 0000 0000 0000 1000 */
#define CS_NUMBER_EDIT_PASSWD               0x0010     /* 0000 0000 0001 0000 */
#define CS_NUMBER_EDIT_NO_CML               0x0020
#define CS_NUMBER_EDIT_TP_PAUSE             0x0040
#define CS_NUMBER_EDIT_ORING                0x0080
#define CS_NUMBER_EDIT_IGNORE_OKKEY         0x0100
#define CS_NUMBER_EDIT_ALONE                0x0200
#define CS_NUMBER_EDIT_RETURN_UPDOWN_KEY    0x0400
#define CS_NUMBER_EDIT_IGNORE_CANCEL        0x0800
#define CS_NUMBER_EDIT_IGNORE_AUTOFOCUSING  0x1000
#define CS_NUMBER_EDIT_DIGIT_ONLY           0x2000
#define CS_NUMBER_EDIT_DRAW_ALL_CONTROL     0x4000

#define CS_NUMBER_EDIT_DEFAULTS          CS_NUMBER_EDIT_BORDER | CS_NUMBER_EDIT_CURSOR  \
                                         | CS_NUMBER_EDIT_TP_PAUSE

#define NE_MAX_SIZE       34 

enum tag_NUMBER_EDIT_RETURN_E {
    NER_OK_KEY,
    NER_DOWN_KEY,
    NER_UP_KEY
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

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern HCONTROL   EQS_API EQC_NumberEdit(COORD x, COORD y, UINT8 nChar, UINT8 nLine, 
                                  UINT8 nMaxLen, BYTE* pszInitNum );

//BYTE*      EQS_API EQC_GetNumberEditValue(HCONTROL hControl);
extern UINT8 EQC_GetNumberEditValue( HCONTROL hControl, BYTE *pszBuf );
extern UINT8 EQC_GetLocalCodeLength(HCONTROL hControl);
extern EQS_RESULT EQS_API EQC_SetNumberEditValue(HCONTROL hControl, 
                                          BYTE* pszNewDialNum);
extern void EQC_NotifyNumberEditMatch( HCONTROL hControl, BYTE* pszMatchNum, UINT8 nIndex );
extern void EQC_NotifyNumberEditMatchLen( HCONTROL hControl, UINT8 nLength );
extern void EQC_SetNumberEditBigFont( HCONTROL hControl, BOOL fSet );
extern void EQC_SetLocalAreaCode( HCONTROL hControl, BYTE* pszLocalCode);
extern void EQC_SetNumberEditBackColor(HCONTROL hControl, COLOR backColor);
extern void EQC_SetNumberEditColor(HCONTROL hControl, COLOR ForeColor);
extern UINT16 EQC_GetNumberEditPos(HCONTROL hControl);

extern EQS_RESULT EQC_SetNumberEditPrefix( HCONTROL hControl, UINT16 nPrefix );
extern void EQC_SetNumberEditPos(HCONTROL hControl, UINT16 nPos);
    
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_NEDT_H_ */
