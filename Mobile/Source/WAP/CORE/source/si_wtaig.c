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

	File: si_wtaiG.c

	Description: GSM specific wtai script functions. Implementation file.

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991028  JPR		First version
	991104	JPR		Updated
	000221	JPR		Functions updated
	000229	JPR		Correction in adapter calls
	001011	ASPN	Lower-case filenames
	0010	ASPN	Major makover

=========================================================================*/

#include "si_wtaig.h"
#include "aapiwta.h"
#include "capiwta.h"

#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif


/* {Function Name, Finish Function Name, Time Cost, Nbr of Arguments} */
const structWTAILibCall arrayScriptwtaiGSM[1][8] =
{
	/* GSM Network Common */
	{ 
		{WTAI_GSM_NF_hold,			WTAI_GSM_NF_hold_finish,		1, 1},
		{WTAI_GSM_NF_retrieve,		WTAI_GSM_NF_retrieve_finish,	1, 1},
		{WTAI_GSM_NF_transfer,		WTAI_GSM_NF_transfer_finish,	1, 2},
		{WTAI_GSM_NF_deflect,		WTAI_GSM_NF_deflect_finish,		1, 2},
		{WTAI_GSM_NF_multiparty,	WTAI_GSM_NF_multiparty_finish,	1, 0},
		{WTAI_GSM_NF_separate,		WTAI_GSM_NF_separate_finish,	1, 1},
		{WTAI_GSM_NF_sendUSSD,		WTAI_GSM_NF_sendUSSD_finish,	1, 4},
		{WTAI_GSM_NF_netinfo,		WTAI_GSM_NF_netinfo_finish,		1, 1},

	}
};



/*========================================================================
==========================================================================
	PRIVATE FUNCTIONS
==========================================================================
=========================================================================*/


/* Definitions for help function */
enum GSMFunctions {GSMFunc_Separate, GSMFunc_Hold, GSMFunc_Retrieve};
typedef enum GSMFunctions enumGSMFunc;

/* Help function for WTAI_GSM_NF_separate, WTAI_GSM_NF_hold, and
   WTAI_GSM_NF_retrieve */
enumErrorCode WTAI_GSM_HelpFunc (pstructOpS OpS, enumGSMFunc eWhichFunc, UINT8 iObjectID)
{
	/* Get parameters from Opstack NOTE! The "popping"-order is 
	   important */
	enumErrorCode eResult=ERR_WAE_WMLS_NONE;
	pstructVar pCallId=OpS_Pop(OpS);

	/* Check if the id is an integer (handle to call) */
	if (Var_Convert( pCallId, typeInteger) == VCR_OK )
	{
		if (pCallId->val.theInt<128)
		{
			/* Call wtai function */
			if (eWhichFunc==GSMFunc_Separate)
			{
				WTAIa_GSMSeparate ( iObjectID, NULL, (INT8)pCallId->val.theInt );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
			else if (eWhichFunc==GSMFunc_Hold)
			{
				WTAIa_GSMHold ( iObjectID, NULL, (INT8)pCallId->val.theInt );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
			else if (eWhichFunc==GSMFunc_Retrieve)
			{
				WTAIa_GSMRetrieve ( iObjectID, NULL, (INT8)pCallId->val.theInt );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}
	}
	Var_Delete( &pCallId );

	return eResult;

}


/*========================================================================
==========================================================================
	EXTERNAL FUNCTIONS
==========================================================================
=========================================================================*/

/* hold(id); */
enumErrorCode WTAI_GSM_NF_hold (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{	
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		/* Call help function */
		return WTAI_GSM_HelpFunc (OpS, GSMFunc_Hold, iObjectID);
	}

	return ERR_WAE_WMLS_LIB;
}

enumErrorCode WTAI_GSM_NF_hold_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* retrieve(id); */
enumErrorCode WTAI_GSM_NF_retrieve (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		/* Call help function */
		return WTAI_GSM_HelpFunc (OpS, GSMFunc_Retrieve, iObjectID);
	}

	return ERR_WAE_WMLS_NONE;
}

enumErrorCode WTAI_GSM_NF_retrieve_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;
	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* GSMTransfer(Bid, Cid); */
enumErrorCode WTAI_GSM_NF_transfer (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pCId=OpS_Pop(OpS);
		pstructVar pBId=OpS_Pop(OpS);

		/* Check if the ids are integers (handles to the calls) */
		if ((Var_Convert( pCId, typeInteger) == VCR_OK ) &&
			(Var_Convert( pBId, typeInteger) == VCR_OK ))
		{
			/* Check if INT8 */
			if ((pCId->val.theInt<128)&&(pBId->val.theInt<128))
			{
				/* Call wtai function */
				WTAIa_GSMTransfer ( iObjectID, NULL, 
									(INT8)pBId->val.theInt,(INT8)pCId->val.theInt);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}

		Var_Delete( &pBId );
		Var_Delete( &pCId );
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}

enumErrorCode WTAI_GSM_NF_transfer_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* GSMDeflect(id, dest); */
enumErrorCode WTAI_GSM_NF_deflect (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		BYTE* pbDest=NULL;
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pDest=OpS_Pop(OpS);
		pstructVar pCallId=OpS_Pop(OpS);

		/* Check if the CallId is an integer (handle to the call) 
		   and if the pDest is a string. */
		if ((Var_Convert(pCallId,typeInteger) == VCR_OK ) &&
			(Var_Convert(pDest,typeString) == VCR_OK ))
		{
			BOOL fOverflow=FALSE;

			/* Convert to CHAR and check if callid is INT8 */
	   		pbDest = wip_wchar2byte( pDest->val.theString, &fOverflow );
			if ((!fOverflow)&&(pCallId->val.theInt<128))
			{
				/* Call wtai function */
				WTAIa_GSMDeflect ( iObjectID, NULL, 
								   (INT8)pCallId->val.theInt,(CHAR*)pbDest);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}
		Var_Delete( &pDest );
		Var_Delete( &pCallId );
		DEALLOC(&pbDest);

		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}
enumErrorCode WTAI_GSM_NF_deflect_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* GSMMultiparty (void); */
enumErrorCode WTAI_GSM_NF_multiparty (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		/* Call wtai function */
		WTAIa_GSMMultiparty (iObjectID, NULL);

		return ERR_WAE_WMLS_WTAI_FUNC_CALLED;

	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode WTAI_GSM_NF_multiparty_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult >= 0)
		{
			Var_AssignInt(pResult, iResult);
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* separate(id); */
enumErrorCode WTAI_GSM_NF_separate (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		/* Call help function */
		return WTAI_GSM_HelpFunc (OpS, GSMFunc_Separate, iObjectID);
	}

	return ERR_WAE_WMLS_LIB;
}

enumErrorCode WTAI_GSM_NF_separate_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}




/* GSMSendUSSD (message,codingScheme,type,transactionId); */
enumErrorCode WTAI_GSM_NF_sendUSSD (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{

	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pTransactionId=OpS_Pop(OpS);
		pstructVar pType=OpS_Pop(OpS);
		pstructVar pCodingScheme=OpS_Pop(OpS);
		pstructVar pMessage=OpS_Pop(OpS);


		/* Check if the parameters can be converted to expected types */
		if ( (Var_Convert( pTransactionId,	typeInteger ) == VCR_OK ) &&
			 (Var_Convert( pMessage,		typeString	) == VCR_OK ) &&
			 (Var_Convert( pCodingScheme,	typeString  ) == VCR_OK ) &&
			 (Var_Convert( pType,			typeString  ) == VCR_OK ) )
		{
			if ((pType->val.theInt >= 0 ) && (pType->val.theInt < 3 ))
			{
				BOOL fOverflow;
				BYTE* pbTransactionId=NULL;

				/* Convert to CHAR */
	   			pbTransactionId = wip_wchar2byte( pTransactionId->val.theString, &fOverflow );
				if (! fOverflow)
				{
					/* Call wtai function */
					WTAIa_GSMSendUSSD ( iObjectID, NULL, pMessage->val.theString, pCodingScheme->val.theString,
									 (INT8)pType->val.theInt, (CHAR*)pbTransactionId);
					eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
				}

				DEALLOC (&pbTransactionId);
			}
		}

		Var_Delete( &pTransactionId );
		Var_Delete( &pType );
		Var_Delete( &pCodingScheme );
		Var_Delete( &pMessage );
	
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}

enumErrorCode WTAI_GSM_NF_sendUSSD_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_INVALID )
		{
			Var_AssignInvalid(pResult);
		}
		else
		{
			Var_AssignInt(pResult, iResult);
		}
		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}



/* GSMNetinfo (type); */
enumErrorCode WTAI_GSM_NF_netinfo (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		pstructVar pType=OpS_Pop(OpS);

		/* Check if the type parameter is an integer. */
		if (Var_Convert( pType, typeInteger ) == VCR_OK)
		{
			/* Call wtai function */
			if ( (pType->val.theInt >= 0) && (pType->val.theInt <= 2) )
			{
				WTAIa_GSMNetinfo( iObjectID, NULL, (INT8)pType->val.theInt);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}
		Var_Delete( &pType );
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}
enumErrorCode WTAI_GSM_NF_netinfo_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	enumErrorCode eResult = ERR_WAE_WMLS_LIB;
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		/* Create result variable */
		pstructVar pResult=Var_New();
		WCHAR* pwchLocation=NEWARRAY (WCHAR,9);

		if (iResult==WTA_INVALID)
		{
			Var_AssignInvalid(pResult);
		}
		else if (pwcFieldValue!=NULL)
		{
			UINT8 i;

			/* wtai call successful - assign string.
			   Convert string to WCHAR. Each octet is stored 
			   in a separate WCHAR-character 
			*/
			for (i=0; i<8; i++)
			{
				pwchLocation[i] = (WCHAR) pwcFieldValue[i];
			}

			/* Set termination character */
			pwchLocation[8] = (WCHAR) (0);

			Var_AssignStringDirect ( pResult, 8, &pwchLocation );
		}


		if (pwcFieldValue!=NULL)
		{
			/* Delete incoming string */
			#ifdef USE_WIP_MALLOC
				wip_free(pwcFieldValue);
			#else
				free(pwcFieldValue);
			#endif
		}

		/* Push result onto stack */
		OpS_Push( OpS, &pResult );

		Var_Delete( &pResult );

		eResult = ERR_WAE_WMLS_NONE;
	}

	if (pwcFieldValue!=NULL)
	{
		/* Delete incoming string */
		#ifdef USE_WIP_MALLOC
			wip_free(pwcFieldValue);
		#else
			free(pwcFieldValue);
		#endif
	}

	return ERR_WAE_WMLS_LIB;
}