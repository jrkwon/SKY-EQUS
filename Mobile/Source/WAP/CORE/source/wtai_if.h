/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
/*========================================================================

	WAP WTA Implementation Project

==========================================================================

	File: WTAI_IF.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991021  JPR		First version
	991101	JPR		Updated

=========================================================================*/

#ifndef WTAIIF_H
#define WTAIIF_H

#include "cmmnrsrc.h"
#include "si_si.h"
#include "capiclnt.h"


typedef struct tWTACONTROL 
{
	void* pCallList;
	BOOL fContextProtected;
	BOOL fEndContext;
	UINT8 iViewID;
	variableType** ppWTAIResult;
} WTACONTROL, *pWTACONTROL;

/* WTAI specific script struct */
typedef struct 
{
	enumErrorCode		(*func)( pstructOpS, pWTACONTROL, UINT8);
	enumErrorCode		(*funcFinish)( pstructOpS, pWTACONTROL, int, WCHAR*);
	UINT16				timeCost;
	UINT8				nbrOfArgs;
} structWTAILibCall;


/*========================================================================
==========================================================================
	DEFINITIONS
==========================================================================
=========================================================================*/
#define WTAI_LIB_PUB	512		/* Public WTAI */
#define WTAI_LIB_VCC	513		/* Network common Voice Call Control */
#define WTAI_LIB_NT		514		/* Network common Network Text */
#define WTAI_LIB_PB		515		/* Network common Phone Book */
#define WTAI_LIB_MISC	516		/* Network common Miscellaneous */
#define WTAI_LIB_CL		519		/* Network common Call Logs */
#define WTAI_GSM_NF		518		/* GSM Spec. Lib. Network Functions */
#define WTAI_PDC_NF		520		/* PDC Spec. Lib. Network Functions */
#define WTAI_IS_136_NF	517		/* IS-136 Spec. Lib. Network Functions */


/*========================================================================
==========================================================================
	EXTERNAL FUNCTIONS
==========================================================================
=========================================================================*/


/*========================================================================
	CheckWTAIScriptCall
==========================================================================
	The function is called to determine if a WTAI function exists or not. 
	If the functoin exists, TRUE is returned and the number of arguments
	taken by the function is stored in the piNbrOfArgs parameter. The 
	time cost of the wtai function is stored in the piTimeCost parameter.
	If the function does not exist, FALSE is returned.

	Input: iLibIndex (IN), iFuncIndex (IN), piNbrOfArgs (OUT), and
		   piTimeCost (OUT)
	Output: TRUE if the function exists, FALSE otherwise
==========================================================================*/
BOOL CheckWTAIScriptCall (UINT16 iLibIndex, UINT8 iFuncIndex, 
						  UINT8* piNbrOfArgs, UINT16* piTimeCost);



/*========================================================================
	SDL_CallWTAIScript
==========================================================================
	The function calls a wtai function based on the iLibId and the iFuncId
	parameters. If the function does not exist an error is returned. 
	Otherwise the correct function is called, the operand stack is popped,
	and the appropriate adapter call is made.

	The returned value of the adapter call is transformed into a script 
	variable which the ppvOpStack variable points to. (The OpStack also 
	deleted.)

	NOTE! The type of the parameter ppvOpStack is (structOpS**) when the
	function call is made, but is changed to (structVar**) when the function
	returns.
	  
	Input: iInvokeId (UINT8), iLibId (UINT16), iFuncId (UINT8), 
		   ppvOpStack (structOpS**), and pvWTAControlStruct (WTACONTROL*)
	Output: enumErrorCode, the result is the ppvOpStack (see note above)
==========================================================================*/
enumErrorCode SDL_CallWTAIScript (int iObjectID, int iLibId, int iFuncId, void** ppvOpStack, 
								  void* pvWTAControlStruct);


enumErrorCode 
SDL_CallWTAIScriptFinish(int iLibId, int iFuncId, void** ppvOpStack, void* pvWTAControlStruct, int iResult, WCHAR* pwcFieldValue);



/*========================================================================
	SDL_OpStackFree
==========================================================================
	The function removes an script operand stack (structOpS).
  
	Input: ppvOpStack (structOpS**)
	Output: -
==========================================================================*/
void SDL_OpStackFree( void** ppvOpStack );


#endif
