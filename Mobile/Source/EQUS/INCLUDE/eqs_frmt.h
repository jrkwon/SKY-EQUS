/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqs_frmt.h
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
** 2000-10-12 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_FRMT_H_
#define _EQS_FRMT_H_

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

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/* 
** the element size 
** ----------------
** ES_AUTO      set the element size by the information of control itself
** ES_PERCENT   the percent size of the remaining width
** ES_PIXEL     pixel size (is possible but not recommended)
** ES_CHAR      the count of characters (only need characters to be displayed)
*/
enum tagELE_SIZE_E {
    ES_NO,
    ES_AUTO,
    ES_PERCENT,
    ES_PIXEL,
    ES_CHAR
};
typedef enum tagELE_SIZE_E          ELE_SIZE_E;

enum tagELE_HORIZ_ALIGN_E {
    EHA_NO,
    EHA_LEFT,
    EHA_CENTER,
    EHA_RIGHT
};
typedef enum tagELE_HORIZ_ALIGN_E   ELE_HORIZ_ALIGN_E;

enum tagELE_VERT_ALIGN_E {
    EVA_NO,
    EVA_TOP,
    EVA_CENTER,
    EVA_BOTTOM
};
typedef enum tagELE_VERT_ALIGN_E    ELE_VERT_ALIGN_E;

typedef struct tagMARGIN_T          MARGIN_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagMARGIN_T {
    COORD xLeft, xRight;
    COORD yTop, yBottom;
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
extern void EQS_API EQS_StartFormatter(void);
extern void EQS_API EQS_EndFormatter(void);
extern void EQS_API EQS_SetMargin(COORD xLeft, COORD yTop, 
                                  COORD xRight, COORD yBottom);
extern void EQS_API EQS_NewLine(void);
extern void EQS_API EQS_SetControlPosition(HCONTROL hControl, 
                                           ELE_SIZE_E Size, UINT8 nSize,
                                           ELE_HORIZ_ALIGN_E HorizAlign,
                                           ELE_VERT_ALIGN_E VertAlign,
                                           BOOL fFixedPos);
extern void EQS_API EQS_AutoFormatting(UINT8 nControl,
                                ELE_HORIZ_ALIGN_E HorizAlign,
                                ELE_VERT_ALIGN_E VertAlign);

extern void EQS_API EQS_GetControlMargin(MARGIN_T* pMargin);
extern void EQS_API EQS_GetControlSpace(COORD* pxSpace, COORD* pySpace);
extern void EQS_API EQS_SetControlMargin(MARGIN_T* pMargin);
extern void EQS_API EQS_SetControlSpace(COORD xSpace, COORD ySpace);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_FRMT_H_ */
