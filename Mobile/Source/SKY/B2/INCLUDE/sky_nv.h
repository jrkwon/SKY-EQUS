/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_nv.h
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
** 2000-10-30 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_NV_H_
#define _SKY_NV_H_

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
extern NV_STATE_E SKY_API SKY_GetNV(NV_ITEM_E Item, NV_ITEM_U *pData);
extern NV_STATE_E SKY_API SKY_GetNV4CM(NV_ITEM_E Item, NV_ITEM_U *pData);
extern NV_STATE_E SKY_API SKY_PutNV(NV_ITEM_E Item, NV_ITEM_U *pData);
extern NV_STATE_E SKY_API SKY_ReplaceNV(NV_ITEM_E Item, NV_ITEM_U *pData);
extern NV_STATE_E SKY_API SKY_FreeNV(NV_ITEM_E Item, NV_ITEM_U *pData);

extern void  SKY_API SKY_SaveNV(void);
extern void SKY_API SKY_NV_Setup(NV_ITEM_E StartItem, NV_ITEM_E EndItem);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_NV_H_ */

