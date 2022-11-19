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

	File: si_wtaia.c

	Description: Network common wtai script functions. Implementation file.


=========================================================================*/

#include "si_wtaia.h"
#include "aapiwta.h"
#include "capiwta.h"
#include "wmlelm.h"
#include "xml_conv.h"
#include "wmlif.h"


#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif

/* {Function Name, Time Cost, Nbr of Arguments} */
const structWTAILibCall arrayScriptwtaiPUB[1][3] =
{
	/* Public */
	{ 
		{WTAI_PUB_publicMakeCall, WTAI_PUB_publicMakeCallFinish, 1, 1},
		{WTAI_PUB_publicSendDTMF, WTAI_PUB_publicSendDTMFFinish, 1, 1},
		{WTAI_PUB_publicAddPBEntry, WTAI_PUB_publicAddPBEntryFinish, 1, 2},
	},


};


/*========================================================================
==========================================================================
	EXTERNAL FUNCTIONS
==========================================================================
=========================================================================*/

/* makeCall(ObjectID, Number); */
enumErrorCode 
WTAI_PUB_publicMakeCall (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	BYTE* pbStr=NULL;
	pWTAControlStruct = pWTAControlStruct; /* Getting rid of warning */

	if (OpS!=NULL)
	{
		BOOL fOverflow=FALSE;
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pNumber=OpS_Pop(OpS);

		/* Check if the number parameter is a string. */
		if ((Var_Convert( pNumber, typeString )==VCR_OK))
		{
			/* Convert to CHAR */
	   		pbStr = wip_wchar2byte( pNumber->val.theString, &fOverflow );
			if (! fOverflow)
			{
				/* Call wtai function */
				WTAIa_publicMakeCall ( iObjectID,(CHAR*)pbStr);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
			DEALLOC( &pbStr );
		}

		Var_Delete( &pNumber );
	
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}

/* sendDTMF(ObjectID, Dtmf); */
enumErrorCode 
WTAI_PUB_publicSendDTMF (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	BYTE* pbStr=NULL;
	pWTAControlStruct = pWTAControlStruct; /* Getting rid of warning */

	if (OpS!=NULL)
	{
		BOOL fOverflow=FALSE;
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pDtmf=OpS_Pop(OpS);

		/* Check if the number parameter is a string and the mode parameter 
		   is an integer. */
		if ((Var_Convert( pDtmf, typeString )==VCR_OK))
		{
			/* Convert to CHAR */
	   		pbStr = wip_wchar2byte( pDtmf->val.theString, &fOverflow );
			if (! fOverflow)
			{
				/* Call wtai function */
				WTAIa_publicSendDTMF ( iObjectID,(CHAR*)pbStr);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
			DEALLOC( &pbStr );
		}

		Var_Delete( &pDtmf );
	
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}

/* addPBEntry(ObjectID, Number, Name); */
enumErrorCode 
WTAI_PUB_publicAddPBEntry (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	BYTE* pbStr=NULL;
	pWTAControlStruct = pWTAControlStruct; /* Getting rid of warning */

	if (OpS!=NULL)
	{
		BOOL fOverflow=FALSE;
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pNumber=OpS_Pop(OpS);
		pstructVar pName=OpS_Pop(OpS);

		/* Check if the number parameter is a string and the mode parameter 
		   is an integer. */
		if ((Var_Convert( pNumber, typeString )==VCR_OK)&&
			(Var_Convert( pName, typeString )==VCR_OK))
		{
			/* Convert to CHAR */
	   		pbStr = wip_wchar2byte( pNumber->val.theString, &fOverflow );
			
			if (! fOverflow)
			{
				/* Call wtai function */
				WTAIa_publicAddPBEntry ( iObjectID,(CHAR*)pbStr,
													(WCHAR*)pName->val.theString);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
			DEALLOC( &pbStr );
		}


		Var_Delete( &pName );
		Var_Delete( &pNumber );
	
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}


enumErrorCode 
WTAI_PUB_publicMakeCallFinish (pstructOpS OpS,
						 pWTACONTROL pWTAControlStruct,
						 int iResult,
						 WCHAR* pwcFieldValue)
{
	pWTAControlStruct = pWTAControlStruct;	/* Getting rid of warning */
	pwcFieldValue = pwcFieldValue;			/* Getting rid of warning */

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		/* If no handle, return invalid */
		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else if ( iResult == WTA_INVALID )
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

enumErrorCode 
WTAI_PUB_publicSendDTMFFinish (pstructOpS OpS,
						 pWTACONTROL pWTAControlStruct,
						 int iResult,
						 WCHAR* pwcFieldValue)
{
	pWTAControlStruct = pWTAControlStruct;	/* Getting rid of warning */
	pwcFieldValue = pwcFieldValue;			/* Getting rid of warning */

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		/* If no handle, return invalid */
		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else if ( iResult == WTA_INVALID )
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

enumErrorCode 
WTAI_PUB_publicAddPBEntryFinish (pstructOpS OpS,
						 pWTACONTROL pWTAControlStruct,
						 int iResult,
						 WCHAR* pwcFieldValue)
{
	pWTAControlStruct = pWTAControlStruct;	/* Getting rid of warning */
	pwcFieldValue = pwcFieldValue;			/* Getting rid of warning */

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		/* If no handle, return invalid */
		if ( iResult == WTA_SUCCESS)
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else if ( iResult == WTA_INVALID )
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
