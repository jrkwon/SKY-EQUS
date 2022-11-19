/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     clntPIM.c
**
** Description: 
**     Phonebook library Function 
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-02-19 ytchoi     Created.
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sky.h"
#include "ska.h"
#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif
#include "clnt.h"
#include "clntapp.h"
#include "aapipush.h"



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
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/*EQS_API*/


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
VOID    PUSHa_newSIReceived (INT16 id, UINT8 priority)
{
    CLNTa_log(1,0,"DUMMY PUSHa_newSIReceived  ");    
}

VOID    PUSHa_SIinfo (INT16 id, UINT8 status, UINT32 created, UINT32 expires, const WCHAR *message, BOOL expired, const CHAR *url, UINT8 priority, const CHAR *initURL)
{
    CLNTa_log(1,0,"DUMMY PUSHa_SIinfo ");    
}

VOID    PUSHa_newSLReceived (INT16 id, UINT8 priority)
{
    CLNTa_log(1,0,"DUMMY PUSHa_newSLReceived ");    
}

VOID    PUSHa_SLinfo (INT16 id, UINT8 status, const CHAR *url, UINT8 priority, const CHAR *initURL)
{
    CLNTa_log(1,0,"DUMMY PUSHa_SLinfo ");    
}

VOID    PUSHa_messagechange (INT16 id, UINT8 change)
{
    CLNTa_log(1,0,"DUMMY PUSHa_messagechange ");    
}

VOID    PUSHa_requestConnection (UINT8 siaId, UINT16 stackMode, UINT8 accessType, const CHAR *address, UINT8 addresslen)
{
    CLNTa_log(1,0,"DUMMY PUSHa_requestConnection ");    
}


