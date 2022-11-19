/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_dsvc.h
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
** 4334-02-08 white     Created.
**
** ============================================================================
*/

#ifndef _SKA_DSVC_H_
#define _SKA_DSVC_H_

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
extern BOOL g_fRelease;
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
extern VOID AirDataManagerTX(BYTE* input_ptr , INT length);
extern void SKY_API SKA_AirDmgrGet(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_AirDmgrSend(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_IPDmgr(SKY_EVENT_T *pEvent);
extern VOID AirDataManagerRX(byte* input_ptr, word length);
extern void SKY_API DataManagerEnd(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_InProgIPAirDM(SKY_EVENT_T *pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_DSVC_H_ */
