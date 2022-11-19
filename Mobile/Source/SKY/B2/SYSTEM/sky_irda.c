/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     sky_irda.c
**
** Description: 
**     IrDA Function
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-04 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"

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
extern void MDM_GetSetting(int, int);
extern boolean		IrDA_Check_Rx_Buffer ( void );
extern byte		IrDA_GetOneByte ( void );
extern short	MDM_CheckLAPConnection(void);
/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
BOOL IrDA_Off = FALSE;

// 	This flag represents IrDA port open or not
BOOL fIrDAPortClosed = TRUE;
// This flag represents IrDA exchange more used or not
BOOL fIrDAExchangeMode = FALSE;

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

void SKY_API SKY_OpenIrDAPort(byte bPrimary)
{
#ifndef WIN32
	static byte PrevPrimary = 0xFF;

	if(!fIrDAPortClosed)
	{
		// 현재 IrDA port가 열려있고 p-to-p mode이고 이전의 mode과 같은 모드이면
		if(fIrDAExchangeMode && bPrimary == PrevPrimary)
			return;
		else
			ChangeIrDAMode ( 0 );
	}

	fIrDAExchangeMode = TRUE;
	if(bPrimary)
		MDM_GetSetting(TRUE,FALSE); // Set to primary
	else
		MDM_GetSetting(FALSE,TRUE); // Set to secondary
	PrevPrimary = bPrimary;
	ChangeIrDAMode ( 1 );
	fIrDAPortClosed = FALSE;
#endif
}

void SKY_API SKY_CloseIrDAPort(void)
{
#ifndef WIN32
	while ( IrDA_Check_Rx_Buffer () ) IrDA_GetOneByte ();

	ChangeIrDAMode ( 0 );
	
	fIrDAPortClosed = TRUE;
#endif
}

short SKY_API SKY_CheckLAPConnection(void)
{
	short			Check;
	
	Check = MDM_CheckLAPConnection();
	return Check;
}