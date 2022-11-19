/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_s017.h
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
** 01-07-13   rosa     Created.
**
** ============================================================================
*/

#ifndef _SKA_S017_H_
#define _SKA_S017_H_

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
extern void SKY_API SKA_Main017Service(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017VMConnect(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017VMSet(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017VMCancel(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017VMEdit(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017ACRegister(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017ACCancel(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017ACEdit(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017RCRegister(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017RCCancel(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017RCEdit(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017CWRegister(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017CWCancel(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017CWEdit(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017PRConnect(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017PREdit(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017PMConnect(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_017HotMenu3Edit(SKY_EVENT_T *pEvent);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_S017_H_ */
