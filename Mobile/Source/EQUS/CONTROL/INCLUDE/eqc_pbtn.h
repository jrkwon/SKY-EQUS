/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_pbtn.h
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
** 2000-10-25 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQC_PBTN_H_
#define _EQC_PBTN_H_

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
#define CS_PUSH_BUTTON_BORDER          0x0001   /* 0000 0000 0000 0001 */
#define CS_PUSH_BUTTON_CENTER          0x0002

#define CS_PUSH_BUTTON_DEFAULT         CS_PUSH_BUTTON_BORDER

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
HCONTROL    EQS_API EQC_PushButton(COORD x, COORD y, WORD Caption);
/* <-- Add here --> get/set functions */
void        EQS_API EQC_SetPushButtonCaption(HCONTROL hControl, BYTE* szString, UINT8 nWidth);
BYTE *      EQS_API EQC_GetPushButton(HCONTROL hControl);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_PBTN_H_ */
