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
#include "aapissec.h"



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
INT16   SECa_clientHello(CHAR **string)
{
    CLNTa_log(1,0,"DUMMY SECa_clientHello");    
    return 0;
}

INT16   SECa_serverHello (const CHAR *string, INT16 length)
{
    CLNTa_log(1,0,"DUMMY SECa_serverHello");    
    return 0;
}

INT16   SECa_keyExchange (CHAR **string)
{
    CLNTa_log(1,0,"DUMMY SECa_keyExchange");    
    return 0;
}

INT16   SECa_finished (const CHAR *string, INT16 length)
{
    CLNTa_log(1,0,"DUMMY SECa_finished");    
    return 0;
}

INT16   SECa_encrypt (const CHAR *string, INT16 length, CHAR **outString)
{
    CLNTa_log(1,0,"DUMMY SECa_encrypt");    
    return 0;
}

INT16   SECa_decrypt (const CHAR *string, INT16 length, CHAR **outString)
{
    CLNTa_log(1,0,"DUMMY SECa_decrypt");    
    return 0;
}
