/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_rbtn.h
**
** Description:
**      Animation Data 
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

#ifndef _EQC_RBTN_H_
#define _EQC_RBTN_H_

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
//#define RBUTTON_DEFAULTS  0x0001
/* CONTROL_STYLE : bit flag */
#define CS_RADIO_BUTTON_BORDER            0x0001   /* 0000 0000 0000 0001 */
#define CS_RADIO_BUTTON_MAXIMIZE          0x0002
#define CS_RADIO_BUTTON_HMAXIMIZE         0x0004

#define CS_RADIO_BUTTON_DEFAULT           CS_RADIO_BUTTON_BORDER

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

extern HCONTROL EQS_API EQC_RadioButton(COORD x, COORD y, \
                                        UINT8 nChar, UINT8 nLine, \
                                        BYTE** ppszInit,  UINT8 nIndex);

extern void        EQS_API EQC_SetRadioButton(HCONTROL hControl,   UINT8 nPos);
extern UINT8       EQS_API EQC_GetRadioButton(HCONTROL hControl);

extern UINT8       EQS_API EQC_GetRadioButtonDisplayLine(COORD StartPosY);
extern void        EQS_API EQC_SetRadioButtonStyle(HCONTROL hControl, WORD StyleA);

#ifdef __cplusplus
}
#endif

#endif // _EQC_RBTN_H_ 
/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
