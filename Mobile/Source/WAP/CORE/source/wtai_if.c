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

	File: WTAI_IF.c

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991021  JPR		First version
	991101	JPR		Updated
	0010	ASPN	Changes for wtai june-2000

=========================================================================*/

#include "wtai_if.h"
#include "si_libs.h"
#ifdef CONFIG_WTA
#include "si_wtaic.h"	/* Network Common */
#endif
#include "si_ops.h"
#include "si_si.h"
#include "si_wtaia.h"	/* Public */

#ifdef CONFIG_WTAI_GSM
#include "si_wtaig.h"	/* GSM */
#endif

#ifdef CONFIG_WTAI_PDC
#include "si_wtaip.h"	/* PDC */
#endif

#ifdef CONFIG_WTAI_IS_136
#include "si_wtaii.h"	/* IS-136 */
#endif

/*========================================================================
==========================================================================
	PRIVATE FUNCTIONS
==========================================================================
=========================================================================*/
structWTAILibCall GetWTAILibFunction (UINT16 iLibIndex, UINT8 iFuncIndex)
{
	BOOL fFuncIndexOk=FALSE;
	structWTAILibCall LibFunc;

	/* Init LibFunc-struct */
	LibFunc.func=NULL;
	LibFunc.funcFinish=NULL;
	
	/* Check if legal function index */
	switch (iLibIndex)
	{
		case WTAI_LIB_PUB:
			if (iFuncIndex<WTAI_PUB_FUNC_MAX)
			{
				fFuncIndexOk=TRUE;
			}
			break;

#ifdef CONFIG_WTA
		/* Network Common */
		case WTAI_LIB_VCC:
		case WTAI_LIB_NT:
		case WTAI_LIB_PB:	
		case WTAI_LIB_MISC:
		case WTAI_LIB_CL:

			if (iFuncIndex<WTAI_NC_FUNC_MAX)
			{
				fFuncIndexOk=TRUE;
			}
			break;

		/* GSM Specific */
		#ifdef CONFIG_WTAI_GSM
		case WTAI_GSM_NF:
			if (iFuncIndex<WTAI_GSM_FUNC_MAX)
			{
				fFuncIndexOk=TRUE;
			}
			break;
		#endif

		/* PDC Specific */
		#ifdef CONFIG_WTAI_PDC
		case WTAI_PDC_NF:
			if (iFuncIndex<WTAI_PDC_FUNC_MAX)
			{
				fFuncIndexOk=TRUE;
			}
			break;
		#endif

		/* IS-136 Specific */
		#ifdef CONFIG_WTAI_IS_136
		case WTAI_IS_136_NF:
			if (iFuncIndex<WTAI_IS_136_FUNC_MAX)
			{
				fFuncIndexOk=TRUE;
			}
			break;
		#endif
#endif
	}

	/* Get function if library index and function index is ok */
	if (fFuncIndexOk)
	{
		switch (iLibIndex)
		{
			case WTAI_LIB_PUB:
				/* Local libs 0 */
				iLibIndex=0;

				/* Get function struct */
				LibFunc=arrayScriptwtaiPUB[iLibIndex][iFuncIndex];
					
				break;
#ifdef CONFIG_WTA
			/* Network Common */
			case WTAI_LIB_VCC:
			case WTAI_LIB_NT:
			case WTAI_LIB_PB:	
			case WTAI_LIB_MISC:

				/* Local libs 0 - 3 */
				iLibIndex-=WTAI_LIB_VCC;

				/* Get function struct */
				LibFunc=arrayScriptwtaiNC[iLibIndex][iFuncIndex];
				
				break;

			case WTAI_LIB_CL:

				/* Local Lib 4 */
				iLibIndex=4;

				/* Get function struct */
				LibFunc=arrayScriptwtaiNC[iLibIndex][iFuncIndex];
				
				break;

			/* GSM Specific */
			#ifdef CONFIG_WTAI_GSM
			case WTAI_GSM_NF:
				/* Local Lib 0 */
				iLibIndex=0;

				/* Get function struct */
				LibFunc=arrayScriptwtaiGSM[iLibIndex][iFuncIndex];
				break;
			#endif

			/* PDC Specific */
			#ifdef CONFIG_WTAI_PDC
			case WTAI_PDC_NF:
				/* Local Lib 0 */
				iLibIndex=0;

				/* Get function struct */
				LibFunc=arrayScriptwtaiPDC[iLibIndex][iFuncIndex];
				break;
			#endif

			/* IS-136 Specific */
			#ifdef CONFIG_WTAI_IS_136
			case WTAI_IS_136_NF:
				/* Local Lib 0 */
				iLibIndex=0;

				/* Get function struct */
				LibFunc=arrayScriptwtaiIS_136[iLibIndex][iFuncIndex];
				break;
			#endif
#endif
		}
	}

	return LibFunc;
}



/*========================================================================
==========================================================================
	EXTERNAL FUNCTIONS
==========================================================================
=========================================================================*/


/*========================================================================
	CheckWTAIScriptCall
=========================================================================*/
BOOL CheckWTAIScriptCall (UINT16 iLibIndex, UINT8 iFuncIndex, 
						  UINT8* piNbrOfArgs, UINT16* piTimeCost)
{
	structWTAILibCall LibFunc;

	/* Get wtai fucntion pointer struct */
	LibFunc=GetWTAILibFunction (iLibIndex, iFuncIndex);

	/* OK? */
	if (LibFunc.func!=NULL)
	{
		/* Function exists. Get nbr of arguments and the
		   time cost and return TRUE. */
		*piNbrOfArgs=LibFunc.nbrOfArgs;
		*piTimeCost=LibFunc.timeCost;

		return TRUE;
	}

	/* Not legal function call */
	return FALSE;
}


/*========================================================================
	SDL_CallWTAIScript
=========================================================================*/
enumErrorCode 
SDL_CallWTAIScript (int iObjectID, int iLibId, int iFuncId, void** ppvOpStack, void* pvWTAControlStruct)
{
	structWTAILibCall LibFunc;

	/* Get wtai function pointer struct */
	LibFunc=GetWTAILibFunction ((UINT16)iLibId, (UINT8)iFuncId);

	/* Check if the function exists */
	if (LibFunc.func!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Call function (Set pbURI to NULL.) */
		eResult=(LibFunc.func)( *((pstructOpS*)ppvOpStack), 
										(pWTACONTROL)pvWTAControlStruct,(UINT8)iObjectID);

		if (eResult==ERR_WAE_WMLS_WTAI_RESULT)
		{
			pstructVar pResult;
			pResult=OpS_Pop((*((pstructOpS*)ppvOpStack)));

			/* Delete Op.Stack */
			OpS_Delete ( (pstructOpS*)(ppvOpStack));

			/* Store pResult in variable ppvOpStack - type changed at 
			exit from function. */
			*ppvOpStack = pResult;

			eResult=ERR_WAE_WMLS_NONE;
		}
		else if (eResult!=ERR_WAE_WMLS_WTAI_FUNC_CALLED)
		{
			pstructVar invalidResult = Var_New();
			
			/* Delete Op.Stack */
			OpS_Delete ( (pstructOpS*)(ppvOpStack));

			
			Var_AssignInvalid( invalidResult );
			*ppvOpStack=invalidResult;
		}


		/* Return */
		return eResult;
	}

	/* Set ppvOpStack to invalid variable and return */
	{
		pstructVar invalidResult = Var_New();

		/* Delete Op.Stack */
		OpS_Delete ( (pstructOpS*)(ppvOpStack));

		Var_AssignInvalid( invalidResult );
		*ppvOpStack=invalidResult;
	}
	return ERR_WAE_WMLS_LIB;

}

/*========================================================================
	SDL_CallWTAIScriptFinish
=========================================================================*/
	/* returnera resultat från respons-funktion,
	   typ pstructVar via SDL(WTAIParam) [*ppvOpStack]
	   
	*/

enumErrorCode 
SDL_CallWTAIScriptFinish(int iLibId, int iFuncId, void** ppvOpStack, void* pvWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	structWTAILibCall LibFunc;

	/* Get wtai function pointer struct */
	LibFunc=GetWTAILibFunction ((UINT16)iLibId, (UINT8)iFuncId);

	/* Check if the function exists */
	if (LibFunc.funcFinish!=NULL)
	{
		pstructVar pResult=NULL;
		enumErrorCode eResult;

		/* Call function (Set pbURI to NULL.) */
		eResult = (LibFunc.funcFinish)(*((pstructOpS*)ppvOpStack), (pWTACONTROL)pvWTAControlStruct, iResult, pwcFieldValue);

		if (eResult==ERR_WAE_WMLS_NONE)
		{
			/* Pop result from stack */
			pResult=OpS_Pop((*((pstructOpS*)ppvOpStack)));
		}

		/* Delete Op.Stack */
		OpS_Delete ( (pstructOpS*)(ppvOpStack));

		/* Store pResult in variable ppvOpStack - type changed at 
		exit from function. */
		*ppvOpStack = pResult;
		
		/* Return */
		return eResult;
	}


	/* Set ppvOpStack to invalid variable and return */

	{
		pstructVar invalidResult = Var_New();

		/* Delete Op.Stack */
		OpS_Delete ( (pstructOpS*)(ppvOpStack));

		Var_AssignInvalid( invalidResult );
		*ppvOpStack=invalidResult;
	}
	return ERR_WAE_WMLS_LIB;
}


/*========================================================================
	SDL_OpStackFree
==========================================================================*/
void SDL_OpStackFree( void** ppvOpStack )
{
	if (ppvOpStack!=NULL)
	{
		/* Delete Op.Stack */
		OpS_Delete ( (pstructOpS*)(ppvOpStack));
	}
}

