/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_pmnu.h
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
** 2000-10-27 woodstok     Created.
**
** ============================================================================
*/

#ifndef _EQC_PMNU_H_
#define _EQC_PMNU_H_

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
#define CS_POPUP_BEEP           0x0001      /* 0000 0000 0000 0001 */
#define CS_POPUP_TITLE          0x0002      /* 0000 0000 0000 0010 */
#define CS_POPUP_RETURN_TOKEN   0x0004
#define CS_POPUP_NO_CML         0x0010
#define CS_POPUP_STATUS         0x0020
    
#define CS_POPUP_DEFAULT      CS_POPUP_TITLE
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

extern HCONTROL EQC_PopUpMenu(EQS_TOKEN_EX *paItem, UINT8 nLine, UINT8 nIndex);
extern HCONTROL EQC_ExceptPopUpMenu(EQS_TOKEN_EX *paItem, UINT8 nLine, UINT8 nIndex,
                                    BOOL *paActive);

extern HCONTROL EQC_PopUpMenuExtend(EQS_TOKEN_EX *paItem, UINT8 nLine, UINT8 nIndex, DWORD dwStyle,
                             WORD nTitleToken, BYTE *pszTitle, BYTE *pszStatus);

extern void EQC_SetPopupMenuTitle(HCONTROL hControl, WORD TokenID);
extern void EQC_SetExceptPopupMenu(HCONTROL hControl, EQS_TOKEN_EX exceptToken);
extern void EQC_ResetExceptPopupMenu(HCONTROL hControl);

extern void EQC_DisablePopupMenu(HCONTROL hControl);
extern void EQC_EnablePopupMenuItem(HCONTROL hControl, EQS_TOKEN_EX enableToken);

extern void EQC_SetPopupMenuTitleString(HCONTROL hControl, BYTE *pszStr);
extern void EQC_SetPopupMenuStatus(HCONTROL hControl, BYTE *pszStr);


extern void EQC_SetPopupStringItem(HCONTROL hControl, BYTE **paszString);
extern void EQC_SetPopupMenuStyle(HCONTROL hControl, WORD StyleA);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_PMNU_H_ */
