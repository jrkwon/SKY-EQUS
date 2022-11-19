/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_efs.h
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
** 2000-12-15 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKA_EFS_H_
#define _SKA_EFS_H_

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
extern void SKY_API SKA_MainDeleteAll(SKY_EVENT_T *pEvent);
/*extern void SKY_API SKA_MainDeleteImage(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainDeleteMelody(SKY_EVENT_T *pEvent);*/
extern void SKY_API SKA_MainInitDir(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainImageList(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainMelodyList(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainViewUsage(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainWriteFile(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainReadFile(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainWriteBlock(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainReadBlock(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_MainMakeTestData(SKY_EVENT_T *pEvent);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_EFS_H_ */
