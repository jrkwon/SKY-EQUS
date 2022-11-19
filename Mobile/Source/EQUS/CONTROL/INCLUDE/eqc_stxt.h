/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_stxt.h
**
** Description:
**      Static Text Control
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

#ifndef _EQC_STXT_H_
#define _EQC_STXT_H_

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
/* CONTROL_TYPE -- bit flag */
#define CS_STATIC_TEXT_NOBORDER     0x0001            /* 0000 0000 0000 0001 */
#define CS_STATIC_TEXT_BORDER       0x0002            /* 0000 0000 0000 0010 */
#define CS_STATIC_TEXT_LEFT         0x0004
#define CS_STATIC_TEXT_RIGHT        0x0008
#define CS_STATIC_TEXT_CENTER       0x0010
#define CS_STATIC_TEXT_CAPTION      0x0020
#define CS_STATIC_TEXT_NO_CHANGE    0x0040
#define CS_STATIC_TEXT_FLOW         0x0080
#define CS_STATIC_TEXT_BLINK        0x0100
#define CS_STATIC_TEXT_ORING        0x0200

/* default control style */
#define CS_STATIC_TEXT_DEFAULT    EQS_DEFAULT_STYLE


enum tagSTATIC_TEXT_FLOW_TYPE_E
{
    STF_NONE,       //default flow mode
    STF_ALL_SHOW,
    STF_MAX_TYPE
};
typedef enum tagSTATIC_TEXT_FLOW_TYPE_E  STATIC_TEXT_FLOW_TYPE_E;

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
extern HCONTROL EQS_API EQC_StaticText(COORD x, COORD y, WORD TokenID);
extern void     EQS_API EQC_SetStaticTextActive(HCONTROL hControl, BOOL fActive);    //white 201.06.08
extern void     EQS_API EQC_SetStaticText(HCONTROL hControl, CONST BYTE* pszStr);
extern void     EQS_API EQC_ShowStaticText(HCONTROL hControl, BOOL fShow);

extern void     EQS_API EQC_SetStaticTextStyle(HCONTROL hControl, WORD StyleA);
extern void     EQS_API EQC_SetStaticTextBlink(HCONTROL hControl, WORD Time);
extern void     EQS_API EQC_SetStaticTextAttribute(HCONTROL hControl, DRAW_MODE DrawMode);
extern void     EQS_API EQC_SetStaticTextColor(HCONTROL hControl, COLOR TextColor);
extern COLOR    EQS_API EQC_GetStaticTextColor(HCONTROL hControl);
extern CONST BYTE* EQS_API EQC_GetStaticText(HCONTROL hControl);

extern void     EQS_API EQC_SetStaticTextFlowMode(HCONTROL hControl, STATIC_TEXT_FLOW_TYPE_E FlowMode);
extern UINT8    EQS_API EQC_GetStaticTextPreviousLength(HCONTROL hControl);

#ifdef __cplusplus
}
#endif

#endif // _EQC_STXT_H_ 
/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/




