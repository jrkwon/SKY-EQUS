/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqa_nam.h
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
** 2000-11-13   hjcho      Created.
**
** ============================================================================
*/

#ifndef _EQA_NAM_H_
#define _EQA_NAM_H_

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
EQS_DCL VOID SKY_API SKA_MainNAM(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM1Entry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM2Entry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM3Entry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM4Entry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_SCI(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_SCM(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM1SubEntry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM2SubEntry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM3SubEntry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM4SubEntry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_NAM5SubEntry(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_PhoneNumber(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_Channel(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_PrefSys(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_MCC(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_MNC(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_PRL(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_SIDNID(SKY_EVENT_T* pEvent);

/* Q Page Test용으로 사용됨 */
#ifdef FEATURE_Q_PAGE
VOID SKY_API SKA_QPAGE(SKY_EVENT_T* pEvent);
#endif //FEATURE_Q_PAGE

VOID SKY_API SKA_MainSimpleNAM(SKY_EVENT_T* pEvent);

//VOID SKY_API SKA_SimpleNAM1SubEntry(SKY_EVENT_T* pEvent);
//VOID SKY_API SKA_SimpleNAM2SubEntry(SKY_EVENT_T* pEvent);
//VOID SKY_API SKA_SimpleNAM3SubEntry(SKY_EVENT_T* pEvent);
//VOID SKY_API SKA_SimpleNAM4SubEntry(SKY_EVENT_T* pEvent);
//VOID SKY_API SKA_SimpleNAM5SubEntry(SKY_EVENT_T* pEvent);


void gInitializeIMSI(UINT8 nNAM);
void gInitializeIMSIT(UINT8 nNAM);
  
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQA_NAM_H_ */
