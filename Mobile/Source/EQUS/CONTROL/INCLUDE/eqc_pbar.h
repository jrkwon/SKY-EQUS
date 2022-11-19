/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_pbar.h
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
** 2000-10-23 redstar       Created.
**
** ============================================================================
*/

#ifndef _EQC_PBAR_H_
#define _EQC_PBAR_H_

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
#define CS_PROGRESS_BAR_BORDER            0x0001   /* 0000 0000 0000 0001 */
#define CS_PROGRESS_BAR_NORMAL            0x0002
#define CS_PROGRESS_BAR_ALONE             0x0004
#define CS_PROGRESS_BAR_DEFAULT           EQS_DEFAULT_STYLE

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
/* <-- Add here --> get/set functions */


HCONTROL EQS_API EQC_ProgressBar(COORD x, COORD y, UINT8 nWidth, UINT8 nHeight, \
                                 WORD nMinA, WORD nMaxA,WORD nInitValue, \
                                 BOOL  fContinue);

void  EQS_API EQC_SetProgressBar(HCONTROL hControl, WORD nValueA);
WORD  EQS_API EQC_GetProgressBar(HCONTROL hControl);
void  EQS_API EQC_SetProgressBarStep(HCONTROL hControl, UINT8 nStep);
UINT8 EQS_API EQC_GetProgressBarStep(HCONTROL hControl);
void  EQS_API EQC_SetProgressBarLimit(HCONTROL hControl, WORD  wMin,  WORD  wMax);
void  EQS_API EQC_GetProgressBarLimit(HCONTROL hControl, WORD* pwMin, WORD* pwMax);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_PBAR_H_ */
