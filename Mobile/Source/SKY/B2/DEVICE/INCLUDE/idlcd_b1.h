/*
** ============================================================================
**
** Project: 
**     Athens
**
** File:
**     eqsqlcd.h
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
** 2000-07-18 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQSQIDLCD_H_
#define _EQSQIDLCD_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "eqs.h"
#include "bio.h"
#include "hsig.h"
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
EQS_DCL void hs_idlcd_on(boolean on);
EQS_DCL void hs_idlcd_init( void );
EQS_DCL VOID EQS_API EQS_RefreshDisplayDevice(VOID);
/*
#define HS_CALL_LED_ON() \
  BIO_OUT( ENC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_ON_V )

#define HS_CALL_LED_OFF() \
  BIO_OUT( ENC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_OFF_V )

#define HS_RGBLED(bred,bgreen,bblue) \
	BIO_OUT(DEC_GPIO_0, BIO_LED_RED_M | BIO_LED_BLUE_M, \
	((bred) ? BIO_LED_RED_HI_V:BIO_LED_RED_LO_V) | ((bblue) ? BIO_LED_BLUE_HI_V : BIO_LED_BLUE_LO_V)); \
    BIO_OUT(DEC_GPIO_1, BIO_LED_GREEN_M, ((bgreen) ? BIO_LED_GREEN_HI_V : BIO_LED_GREEN_LO_V))
*/
#ifdef __cplusplus
}
#endif

#endif  /* _EQSQIDLCD_H_ */
