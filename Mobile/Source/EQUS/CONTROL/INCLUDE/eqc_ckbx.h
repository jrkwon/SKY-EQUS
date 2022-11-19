/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_chbx.h
**
** Description:
**     Check Box Control
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-10-20 redstar       
**
** ============================================================================
*/

#ifndef _EQC_CKBX_H_
#define _EQC_CKBX_H_

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
#define CS_CHECK_BOX_BORDER            0x0001   /* 0000 0000 0000 0001 */
#define CS_CHECK_BOX_MAXIMIZE          0x0002
#define CS_CHECK_BOX_HMAXIMIZE         0x0004
#define CS_CHECK_BOX_LIST              0x0008

#define CS_CHECK_BOX_DEFAULT           CS_CHECK_BOX_BORDER


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
HCONTROL    EQS_API EQC_CheckBox(COORD x, COORD y, BYTE**paszStr, UINT8 nSizeA);
/* <-- Add here --> get/set functions */
void        EQS_API EQC_SetCheckBox(HCONTROL hControl,   UINT8 nPos);
void        EQS_API EQC_ResetCheckBox(HCONTROL hControl, UINT8 nPos);
/* <-- Add here --> */ 
UINT8       EQS_API EQC_GetCheckBoxSize(HCONTROL hControl);
BOOL        EQS_API EQC_GetCheckBox(HCONTROL hControl, UINT8 nPos);

void        EQS_API EQC_SetCheckBoxIndex(HCONTROL hControl, UINT8 nIndex);
UINT8       EQS_API EQC_GetCheckBoxIndex(HCONTROL hControl);
UINT8       EQS_API EQC_GetCheckBoxDisplayLine(COORD StartPosY);
void        EQS_API EQC_SetCheckBoxStyle(HCONTROL hControl, WORD StyleA);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_CKBX_H_ */

