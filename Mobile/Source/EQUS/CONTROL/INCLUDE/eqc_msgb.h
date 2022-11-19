/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_msgb.h
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

#ifndef _EQC_MSGB_H_
#define _EQC_MSGB_H_

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
#define CS_MESSAGE_BOX_BEEP         0x0001      /* 0000 0000 0000 0001 */
// If pszTitle is not NULL
#define CS_MESSAGE_BOX_TITLE        0x0002      /* 0000 0000 0000 0010 */
// If MessageBox type is MBT_OK or MBT_YESNO
#define CS_MESSAGE_BOX_SOFTKEY      0x0004      /* 0000 0000 0000 0100 */
// If MessageBox type is MBT_NONE
#define CS_MESSAGE_BOX_IGNORE_KEYS  0x0008      /* 0000 0000 0000 1000 */
// System message box -- won't be purged on applet transition
#define CS_MESSAGE_BOX_SYSTEM       0x0010      /* 0000 0000 0001 0000 */
#define CS_MESSAGE_BOX_CENTER_ALIGN 0x0020
#define CS_MESSAGE_BOX_FIXED_SIZE   0x0040

    
#define CS_MESSAGE_BOX_DEFAULT      CS_MESSAGE_BOX_SOFTKEY | CS_MESSAGE_BOX_CENTER_ALIGN | CS_MESSAGE_BOX_TITLE
#define CS_SYSMESSAGE_BOX_DEFAULT   CS_MESSAGE_BOX_SOFTKEY | CS_MESSAGE_BOX_SYSTEM | CS_MESSAGE_BOX_CENTER_ALIGN \
                                    | CS_MESSAGE_BOX_TITLE
                                    
#define MESSAGE_BOX_MAX_MESG_LEN    60
#define MESSAGE_BOX_TIME_UNLIMIT    0xFFFF

#define MESSAGE_DEFAULT_TIME        2
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagMESSAGE_BOX_TYPE_E
{
    MBT_NONE,       // nSec or Anykey event
    MBT_OK,         // OK Key
    MBT_YESNO,      // Yes/No Key (Default : Yes)
    MBT_NOYES,      // Yes/No Key (Default : No)
    MBT_CANCEL,     // Cancel Key
    MBT_EVENT,      // predefined Event via EQC_AddMessageBoxEvent()
    MBT_MAX_TYPE
};
typedef enum tagMESSAGE_BOX_TYPE_E  MESSAGE_BOX_TYPE_E;

enum tagMESSAGE_BOX_TITLE_E
{
    MBT_SUCCESS,
    MBT_FAIL,
    MBT_HELP,
    MBT_CONFIRM,
    MBT_SYSTEM,
    MBT_NOTITLE
};
typedef enum tagMESSAGE_BOX_TITLE_E  MESSAGE_BOX_TITLE_E;
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
extern HCONTROL EQS_API EQC_MessageBox(BYTE* pszMessage, WORD nSec, 
                                MESSAGE_BOX_TYPE_E Type, MESSAGE_BOX_TITLE_E TitleType);

extern HCONTROL EQS_API EQC_BeepMessageBox(BYTE* pszMessage, WORD nSec, 
                                MESSAGE_BOX_TYPE_E Type, MESSAGE_BOX_TITLE_E TitleType);

extern HCONTROL EQS_API EQC_NoBeepMessageBox(BYTE* pszMessage, WORD nSec, 
                                MESSAGE_BOX_TYPE_E Type, MESSAGE_BOX_TITLE_E TitleType);

extern HCONTROL EQS_API EQC_FixedMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType, 
                        UINT8 nChar, UINT8 nLine);

extern HCONTROL EQS_API EQC_SystemMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType);

extern HCONTROL EQS_API EQC_BeepSystemMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType);

// For MBT_EVENT type MessageBox
extern EQS_RESULT EQS_API EQC_AddMessageBoxEvent(HCONTROL hControl, 
                                          EQS_EVENT_ID Event);

extern EQS_RESULT EQS_API EQC_SetCurrentTheme(HCONTROL hControl, 
                                          COLOR_SCHEME_E Theme);

extern void EQC_SetMessageBoxStyle(HCONTROL hControl, WORD StyleA);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQC_MSGB_H_ */
