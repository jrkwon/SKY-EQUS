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
#include "cssls.h"

#include "msg.h"
#include "wapcommon.h"



/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define EncDecBufSize 1500
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

extern SSLS*   spSKT;
stPaq  g_stpPaq;	
int    g_npErr;
int    g_nMode;
unsigned char*   g_cpUrl = NULL; 
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
extern int ssls_open_send(
		stPaq *stpPaq,
		int *npErr,
		char *cpUrl, int nMode,
		uc	ucCipher);

extern int ency_write(primitiveC *ipPrim,
					  unsigned char *ucpSend, int nSendLen,
					  int *npErr, char* Rec);
extern int ency_read( unsigned char *ucpRecv, int nBufLen, 
					  int *npErr,char* cpRec, int nReadLen);
extern int ssls_recv_hello(stPaq *stpPaq, int *npErr, 
					uc *ucRecBuf, int nRecBufSize,
					char *cpUrl, int nMode);
extern boolean CheckspSkt(void);

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
	int ret = 0;

    MSG_HIGH("SECa_clientHello",0,0,0);    

	g_npErr = 0;
	g_cpUrl = NULL;
	g_nMode = 0;

	ret = ssls_open_send(&g_stpPaq, &g_npErr, (char*)g_cpUrl, g_nMode, T_SEED);

	if(ret != 1)
	{
		MSG_ERROR("SECa_clientHello fail",0,0,0);
		return -1;
	}
	/* let core know the contents */
	*string = (CHAR*)wip_malloc(spSKT->ulBufSize + 1 );
	memcpy(*string, spSKT->ucpBuffer, spSKT->ulBufSize);

	return (INT16)(spSKT->ulBufSize);
}

INT16   SECa_serverHello (const CHAR *string, INT16 length)
{
	int ret = -1;

    displayloading(6);

    MSG_HIGH("SECa_serverHello [%d][%d]", *string, length, 0);

	if(CheckspSkt() == FALSE) 
	{
		MSG_ERROR("SECa_serverHello fail",0,0,0);
		return -1;
	}
    ret = ssls_recv_hello(&g_stpPaq, &g_npErr, 
					(uc*) string, (int) length,
					(char*) g_cpUrl, T_SEED);
	if(ret != 0)
	{
		MSG_ERROR("SECa_serverHello fail",0,0,0);
		return -1;
	}

    return 0;  //success 0, fail -1
}

INT16   SECa_keyExchange (CHAR **string)
{
    MSG_HIGH("SECa_keyExchange [%d]",spSKT->ulBufSize,0,0);    

	/* let core know the contents */
	*string = (CHAR*)wip_malloc(spSKT->ulBufSize + 1 );
	memcpy(*string, spSKT->ucpBuffer, spSKT->ulBufSize);

	return (INT16)(spSKT->ulBufSize);
}


INT16   SECa_finished (const CHAR *string, INT16 length)
{
	int ret = -1;

	MSG_HIGH("SECa_finished [%d][%d]", *string, length, 0);

    ret = ssls_recv_hello(&g_stpPaq, &g_npErr, 
					(uc*) string, (int) length,
					(char*) g_cpUrl, T_SEED);
	if(ret != 0)
	{
		MSG_ERROR("SECa_finished fail",0,0,0);
		return -1;
	}

    return 0;  //success 0, fail -1
}

INT16   SECa_encrypt (const CHAR *string, INT16 length, CHAR **outString)
{
	int  ret = -1;

	if(length > EncDecBufSize)
	{
		MSG_ERROR("length exceed the length of buffer [%d]",length,0,0);
		return -1;
	}

	*outString = (CHAR*)wip_malloc(EncDecBufSize);
    ret = ency_write(NULL,	(unsigned char *) string, (int) length,
					&g_npErr, *outString);
    
	MSG_HIGH("SECa_encrypt str[%d] len[%d] retlen[%d]", 
				*string, length, ret); 

	if(ret == 0)
	{
		MSG_ERROR("SECa_encrypt fail",0,0,0);
		return -1;
	}

    return (INT16)ret;
}

INT16   SECa_decrypt (const CHAR *string, INT16 length, CHAR **outString)
{
	int  ret = -1;

	if(length > EncDecBufSize)
	{
		MSG_ERROR("length exceed the length of buffer [%d]",length,0,0);
		return -1;
	}

	*outString = (CHAR*)wip_malloc(EncDecBufSize);
	ret = ency_read( (unsigned char*)*outString, EncDecBufSize, &g_npErr, 
		             (char*)string, length);

    MSG_HIGH("SECa_decrypt str[%d] len[%d] retlen[%d]", 
		         *string, length, ret);  	
	if(ret == 0)
	{
		MSG_ERROR("SECa_encrypt fail",0,0,0);
		return -1;
	}

    return ret;
}

