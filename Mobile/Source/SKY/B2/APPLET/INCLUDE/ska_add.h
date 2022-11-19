/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_add.h
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
** 4334-01-03 white     Created.
**
** ============================================================================
*/

#ifndef _SKA_ADD_H_
#define _SKA_ADD_H_

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
#define NUM_COLOR               6//8        
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
extern void SKY_API SKA_MainAutoLCDBacklight(SKY_EVENT_T *pEvent); 
extern void SKY_API SKA_MainAutoSubLCDBacklight(SKY_EVENT_T *pEvent); 
extern void SKY_API SKA_SubLcdColorSel(SKY_EVENT_T *pEvent);  
extern void SKY_API SKA_AutoReceive(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_GroupLcdSel(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainAreaNumber(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainAutoRedial(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainAutoFnc(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainAutoBacklight(SKY_EVENT_T *pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_ADD_H_ */
