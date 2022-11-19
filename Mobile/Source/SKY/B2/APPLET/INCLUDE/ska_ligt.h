/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_ligt.h
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
** 4333-12-20 white     Created.
**
** ============================================================================
*/

#ifndef _SKA_LIGT_H_
#define _SKA_LIGT_H_

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
extern BYTE ContrastLevel;    //Added by happykid in 2001.05.09
extern BYTE SubContrastLevel;
    /*
    **---------------------------------------------------------------------------
    **  Function(external use only) Declarations
    **---------------------------------------------------------------------------
    */
extern void SKY_API SKA_MainBright(SKY_EVENT_T *pEvent);   
extern void SKY_API SKA_MainBatteryInfo(SKY_EVENT_T *pEvent); // girin. 2000.12.29 
extern void SKY_API SKA_ViewSMB(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_BatteryTest(SKY_EVENT_T *pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_LIGT_H_ */
