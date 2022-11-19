/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_irph.h
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
** 2001-01-04 woodstok     Created.
**
** ============================================================================
*/

#ifndef _SKA_IRPH_H_
#define _SKA_IRPH_H_

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

extern void SKY_API SKA_MainIrDAPhoneReceive(SKY_EVENT_T *pEvent);

extern void SKY_API SKY_OpenIrDAPort(byte bPrimary);
extern void SKY_API SKY_CloseIrDAPort(void);


extern void SKY_API SKA_MainIrDAPC(SKY_EVENT_T *pEvent);

// PhoneBook
extern void SKY_API SKA_MainIrDABookAll(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainIrDABookGroup(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainIrDABookName(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainIrDABookSDial(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainIrDABookMyInfo(SKY_EVENT_T *pEvent);

extern void SKY_API SKA_MainIrDAAnimation(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainIrDAMelody(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainIrDAPhoto(SKY_EVENT_T *pEvent);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_IRPH_H_ */
