/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     gvmShare.c
**
** Description: 
**     Define Shared Buffer for GVM
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-02-03 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

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
**  Global variables
**---------------------------------------------------------------------------
*/
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

void	GVMInitVariable ( void )
{
	extern byte	Load$$APP_SHARE_RAM$$Base[];
    extern BYTE	Image$$APP_SHARE_RAM$$Base[];
	extern byte	Image$$APP_SHARE_RAM$$Length[];
	extern byte	Image$$APP_SHARE_RAM$$ZI$$Base[];
	extern byte	Image$$APP_SHARE_RAM$$ZI$$Length[];
	dword	*pSrc;
	dword	*pDest;
	dword	*pStop;

	pStop = (dword*)( Image$$APP_SHARE_RAM$$Base +
		(dword)Image$$APP_SHARE_RAM$$Length );
	for ( pSrc = (dword*)Load$$APP_SHARE_RAM$$Base,
			pDest = (dword*)Image$$APP_SHARE_RAM$$Base;
		pDest < pStop;
		pSrc ++, pDest ++ ) 
	{
		*pDest = *pSrc;
	}

	pStop = (dword*)( Image$$APP_SHARE_RAM$$ZI$$Base +
		(dword)Image$$APP_SHARE_RAM$$ZI$$Length );
	for ( pDest = (dword*)Image$$APP_SHARE_RAM$$ZI$$Base; 
		pDest < pStop;
		pDest ++ ) 
	{
		*pDest = 0;
	}
}