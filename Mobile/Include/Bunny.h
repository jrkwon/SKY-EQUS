

/*****************************************************************************
 *
 *		Common(Global, Model dependent) header for Ori project
 *
 *		Copyright (C) 1999,2000 SK Telecom
 *
 *		'Bomber(B1)' project 
 *
 *****************************************************************************
 *
 *		FileName : Bomber.h
 *		Revision : 0.00.001
 *		 Created : 00.06.14		by tasik
 *
 *****************************************************************************
 *
 *		Revision detail
 *		---------------
 *
 *		Rev	0.00.001	00.02.28 09:00	chos
 *			created this module ( based "Dooly.h" )
 *
 *****************************************************************************/

#ifndef __INCLUDE_ORI_HEADER
	#define	__INCLUDE_ORI_HEADER


#include	<Common.h>

// compile for browser
#ifdef	_FOR_BROWSER
	#define	__packed
	#define	__global_reg(x)
#pragma warning( disable : 4068 )		// Disable warning message
#endif	// #ifdef	_FOR_BROWSER


/*****************************************************************************
 *	define model ID
 *****************************************************************************/

#define	MODEL_SKTT

#define	MODEL_ID		MODEL_B2


/*****************************************************************************
 *	dependent service provider
 *****************************************************************************/
#define	SERVICE_PROVIDER		SP_SKTELECOM


/*****************************************************************************
 *	dependent country
 *****************************************************************************/
#define	COUNTRY					COUNTRY_KOREA


/*****************************************************************************
 *	dependent LCD controler
 *****************************************************************************/
#define LCD_TYPE				NEC


/*****************************************************************************
 *	MMI SELECTION
 *****************************************************************************/
// MakeFile에서 선언하므로 이곳에서는 필요 없다
//#define	 MMI_TYPE	MMI_QUALCOMM
//#define	 MMI_TYPE	MMI_SK
//#define	 MMI_TYPE	MMI_SKME


/*****************************************************************************
 *	Use YAMAHA Melody chip
 *****************************************************************************/
#define  YAMAHA_MELODY // Added by happykid 99.12.28
#define  OKI_CODEC

/*****************************************************************************
 *		Sample use define  ---------->
 *****************************************************************************/

	// SKTT common code

	#ifdef	MODEL_SKTT

		/* Here SKTT common code ( A7, A10, B1, D1 ... ) */

 	#else	// #ifdef	MODEL_SKTT

		/* Here Qualcomm code */

 	#endif	// #ifdef	MODEL_SKTT


	// SKTT model dependent code

	#if	MODEL_ID == MODEL_B2

		/* Here D1 code */

 	#else	// #if	MODEL_ID == MODEL_D1

		/* Here other model code ( B5 or C5 ) */

 	#endif	// #if	MODEL_ID == MODEL_D1

/*****************************************************************************
 *		<----------  Sample use define
 *****************************************************************************/

#endif	// #ifndef __INCLUDE_ORI_HEADER
