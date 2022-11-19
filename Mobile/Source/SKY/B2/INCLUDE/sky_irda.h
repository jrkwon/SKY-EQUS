/*
** ============================================================================
**
** Project: 
**     Boa
**
** File:
**     sky_irda.h
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

#ifndef _SKY_IRDA_H_
#define _SKY_IRDA_H_

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

extern BOOL IrDA_Off;
// 	This flag represents IrDA port open or not
extern BOOL fIrDAPortClosed;
// This flag represents IrDA exchange more used or not
extern BOOL fIrDAExchangeMode;

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

extern void SKY_API SKY_OpenIrDAPort(byte bPrimary);
extern void SKY_API SKY_CloseIrDAPort(void);
extern short SKY_API SKY_CheckLAPConnection(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_IRDA_H_ */
