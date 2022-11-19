/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     eqsqlcd_B1.h
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
** 2000.08.01  tasik     Make it fit for B1
** 2000-07-18 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQSQLCDB1_H_
#define _EQSQLCDB1_H_

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

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
// #define LCD_CONVERT_TEMPORARY //LCD의 데이터 변환인데 잠정적이다.

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
EQS_DCL void hs_lcd_on(boolean on);
EQS_DCL void hs_lcd_init( void );
EQS_DCL VOID EQS_API EQS_RefreshDisplayDevice(VOID);
//00.08.02



#define HS_CALL_LED_ON() \
	BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_HI_V)

#define HS_CALL_LED_OFF() \
	BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V)



#define HS_RGBLED(bred,bgreen,bblue) FALSE
							
/*GPIO의 할당을 모두 하기 전에 아래의 매크로는 잠시 막아 놓는다.*/
/*
	BIO_OUT(ENC_GPIO_1, BIO_LED_RED_M | BIO_LED_BLUE_M, \
	((bred) ? BIO_LED_RED_HI_V:BIO_LED_RED_LO_V) | ((bblue) ? BIO_LED_BLUE_HI_V : BIO_LED_BLUE_LO_V)); \
    BIO_OUT(ENC_GPIO_1, BIO_LED_GREEN_M, ((bgreen) ? BIO_LED_GREEN_HI_V : BIO_LED_GREEN_LO_V))
*/
#ifdef __cplusplus
}
#endif

#endif  /* _EQSQLCDB1_H_ */
