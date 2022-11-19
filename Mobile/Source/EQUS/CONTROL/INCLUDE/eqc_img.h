/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_img.h
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
** 2000-10-17 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQC_IMG_H_
#define _EQC_IMG_H_

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
#define CS_IMAGE_BORDER     0x0001
/* default control style */
#define CS_IMAGE_DEFAULT    EQS_DEFAULT_STYLE

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

HCONTROL EQS_API EQC_Image(COORD x, COORD y, BITMAP* pBitmap);
void EQS_API EQC_SetImage(HCONTROL hControl, BITMAP* pBitmap);
BITMAP* EQS_API EQC_GetBitmap(HCONTROL hControl);

BOOL EQS_API EQC_SetImageViewArea(HCONTROL hControl, RECT* pRect);
RECT EQS_API EQC_GetImageViewArea(HCONTROL hControl);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_IMG_H_ */
