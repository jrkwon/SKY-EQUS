/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_menu.h
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
** 2000-11-07 girin     Created.
**
** ============================================================================
*/

#ifndef _EQC_MENU_H_
#define _EQC_MENU_H_

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
#define TO_FACIFY_ARM_COMPILER          12


#define CS_MENU_LIST_ENUMERATED       0x0001   /* 0000 0000 0000 0001 */
#define CS_MENU_LIST_BORDER           0x0002   /* 0000 0000 0000 0010 */
#define CS_MENU_LIST_SCROLL_BAR       0x0004   /* 0000 0000 0000 0100 */
#define CS_MENU_LIST_LEFT_ALIGN       0x0008
#define CS_MENU_LIST_RIGHT_ALIGN      0x0010
#define CS_MENU_LIST_CENTER_ALIGN     0x0020
#define CS_MENU_LIST_SHOW_TITLE       0x0040
#define CS_MENU_LIST_ICON             0x0080
#define CS_MENU_LIST_FULL_SCREEN      0x0100
#define CS_MENU_LIST_CAPTION          0x0200
#define CS_MENU_LIST_NO_CML           0x0400
        


#define CS_MENU_LIST_DEFAULT          \
    (CS_MENU_LIST_BORDER | CS_MENU_LIST_LEFT_ALIGN | CS_MENU_LIST_SHOW_TITLE | CS_MENU_LIST_ENUMERATED)

#define CS_MENU_LIST_ICON_DEFAULT    \
    (CS_MENU_LIST_LEFT_ALIGN | CS_MENU_LIST_SHOW_TITLE | CS_MENU_LIST_ICON | CS_MENU_LIST_CAPTION | CS_MENU_LIST_ENUMERATED )
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagMNODE_BODY_T      MNODE_BODY_T;
typedef struct tagMNODE_T           MNODE_T;



/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

struct tagMNODE_BODY_T
{
    ACCEL_KEY_T                   AccelKey;
    BYTE*                         pszTitle;
    EQS_TOKEN_EX                  TokenID;
    CONST ANIMATION_INFO_T        *pAnimatedIcon;    // Menu Animation Info.
    BOOL                          fIsLeaf;
    void*                         pNext;
};

struct tagMNODE_T
{
    ROM MNODE_T         *pParentMnode;
    UINT8               nMnodeLine;
    UINT8               nHiddenLine;
    EQS_TOKEN_EX        TitleTokenID;
    ROM MNODE_BODY_T    *pMbodyArray[TO_FACIFY_ARM_COMPILER];
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

extern HCONTROL EQS_API EQC_MenuList(COORD x, COORD y, UINT8 nChar, UINT8 nLine, 
                              ROM MNODE_T* pMnode, UINT8 nStartIndx, BOOL fIcon);

extern void     EQS_API EQC_SetMenuListStyle(HCONTROL hControl, WORD StyleA);
extern void     EQS_API EQC_SetMenuIconMode(HCONTROL hControl, BOOL fIcon);
extern void     EQS_API EQC_SetMenuImage(HCONTROL hControl, CONST BITMAP* pImage);
extern void     EQS_API EQC_SetMenuBackColor(HCONTROL hControl, COLOR BackColor);
extern void     EQS_API EQC_SetMenuItemText(HCONTROL hControl, UINT8 nTopIndex, BYTE* pszStr);
extern BYTE*    EQS_API EQC_GetMenuItemText(HCONTROL hControl, UINT8 nTopIndex);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_MENU_H_ */
