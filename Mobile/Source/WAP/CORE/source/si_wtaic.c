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

	File: si_wtaiC.c

	Description: Network common wtai script functions. Implementation file.

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991026  JPR		First version
	991104	JPR		Updated
	000209	JPR		Corrections
	000217	JPR		Functions updated	
	000221	JPR		Phonebook functions implemented
	000225	JPR		SendDtmf-functionality updated
	000228	FAR		URI functionality updated
	000229	JPR		Correction in adapter calls
	001011	ASPN	Lower-case filenames

=========================================================================*/

#include "si_wtaic.h"
#include "aapiwta.h"
#include "capiwta.h"
#include "wmlelm.h"
#include "xml_conv.h"
#include "wmlif.h"
#include "msgqueue.h"



#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif
 

/* {Function Name, Time Cost, Nbr of Arguments} */
const structWTAILibCall arrayScriptwtaiNC[5][6] =
{
	/* Voice Call Control */
	{ 
		{WTAI_NC_VC_setup,			WTAI_NC_VC_setup_finish,		1, 2},
		{WTAI_NC_VC_accept,			WTAI_NC_VC_accept_finish,		1, 2},
		{WTAI_NC_VC_release,		WTAI_NC_VC_release_finish,		1, 1},				
		{WTAI_NC_VC_sendDTMF,		WTAI_NC_VC_sendDTMF_finish,		1, 2},
		{WTAI_NC_VC_callStatus,		WTAI_NC_VC_callStatus_finish,	1, 2},			
		{WTAI_NC_VC_list,			WTAI_NC_VC_list_finish,			1, 1}
	},

	/* Network Text */
	{ 
		{WTAI_NC_NT_send,			WTAI_NC_NT_send_finish,			1, 2},		
		{WTAI_NC_NT_list,			WTAI_NC_NT_list_finish,			1, 2},
		{WTAI_NC_NT_remove,			WTAI_NC_NT_remove_finish,		1, 1},	
		{WTAI_NC_NT_getFieldValue,	WTAI_NC_NT_getFieldValue_finish,1, 2},
		{NULL, NULL, 0, 0},								
		{NULL, NULL, 0, 0}
	},

	/* Phone Book */ 
	{ 
		{WTAI_NC_PB_write,			WTAI_NC_PB_write_finish,		1, 3},		
		{WTAI_NC_PB_search,			WTAI_NC_PB_search_finish,		1, 2},
		{WTAI_NC_PB_remove,			WTAI_NC_PB_remove_finish,		1, 1},	
		{WTAI_NC_PB_getFieldValue,	WTAI_NC_PB_getFieldValue_finish,1, 2},
		{WTAI_NC_PB_change,			WTAI_NC_PB_change_finish,		1, 3},	
		{NULL, NULL, 0, 0}
	},

	/* Call Logs */
	{ 
		{WTAI_NC_CL_dialled,		WTAI_NC_CL_dialled_finish,		1, 1},	
		{WTAI_NC_CL_missed,			WTAI_NC_CL_missed_finish,		1, 1},
		{WTAI_NC_CL_received,		WTAI_NC_CL_received_finish,		1, 1},	
		{WTAI_NC_CL_getFieldValue,	WTAI_NC_CL_getFieldValue_finish,1, 2},
		{NULL, NULL, 0, 0},								
		{NULL, NULL, 0, 0}
	},
		
	/* Miscelleaneous */
	{ 
		{WTAI_NC_MISC_indication,	WTAI_NC_MISC_indication_finish,	1, 2},
		{WTAI_NC_MISC_endcontext,	NULL,							1, 0},
		{WTAI_NC_MISC_getProtection,NULL,							1, 0},
		{WTAI_NC_MISC_setProtection,NULL,							1, 1},	
		{NULL, NULL, 0, 0},
		{NULL, NULL, 0, 0}							
	}

};


/*========================================================================
==========================================================================
	PRIVATE FUNCTIONS
==========================================================================
=========================================================================*/



/*========================================================================
==========================================================================
	EXTERNAL FUNCTIONS
==========================================================================
=========================================================================*/


/* Voice Call Control ****************************************************/


/* voiceCallSetup(number, mode); */
enumErrorCode 
WTAI_NC_VC_setup (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	BYTE* pbStr=NULL;
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		BOOL fOverflow = FALSE;
		enumErrorCode eResult = ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pMode=OpS_Pop(OpS);
		pstructVar pNumber=OpS_Pop(OpS);

		/* Check if the number parameter is a string and the mode parameter 
		   is an integer. */
		if ( (Var_Convert( pNumber, typeString )==VCR_OK) &&
			 (Var_Convert( pMode, typeInteger )==VCR_OK) )
		{
			/* Check mode */
			if ((pMode->val.theInt==1)||(pMode->val.theInt==0))
			{
				/* Convert to CHAR */
	   			pbStr = wip_wchar2byte( pNumber->val.theString, &fOverflow );
				if (! fOverflow)
				{
					/* Call wtai function */
					WTAIa_voiceCallSetup ( iObjectID, NULL, (const CHAR*)pbStr); /* objectId, wtaChannelId, number */
					OpS_Push(OpS, &pMode);
					eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
				}
				DEALLOC( &pbStr );
			}
		}

		Var_Delete( &pNumber );
		Var_Delete( &pMode );	
		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_VC_setup_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		/* Create result variable */
		pstructVar pResult = Var_New();
		/* Get parameters from Opstack NOTE! The "popping"-order is 
	       important */
		pstructVar pMode = OpS_Pop(OpS);

		if ( (iResult >= 0) && (Var_Convert( pMode, typeInteger )==VCR_OK) )
		{
			if ( pMode->val.theInt==0 )
			{
				AddCall(& pWTAControlStruct->pCallList, (INT8)iResult);
			}
			Var_AssignInt(pResult, iResult);
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		Var_Delete( &pMode );
		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* voiceCallAccept(callHandle, mode); */
enumErrorCode
WTAI_NC_VC_accept (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult = ERR_WAE_WMLS_NONE;
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pMode=OpS_Pop(OpS);
		pstructVar pCallId=OpS_Pop(OpS);

		/* Check if the id parameter is a integer less than 128 (INT8) */
		if ( (Var_Convert(pCallId, typeInteger)==VCR_OK) && 
			 (pCallId->val.theInt<128) )
		{
			/* Call wtai function */
			WTAIa_voiceCallAccept ( iObjectID, NULL, (INT8)pCallId->val.theInt );
			OpS_Push( OpS, &pMode );
			eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
		}

		Var_Delete( &pMode );
		Var_Delete( &pCallId );
		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_VC_accept_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		/* Create result variable */
		pstructVar pResult=Var_New();

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pMode=OpS_Pop(OpS);
		
		if ( (iResult == WTA_SUCCESS) && (Var_Convert( pMode, typeInteger )==VCR_OK) )
		{
			if ( pMode->val.theInt==0 )
			{
				AddCall(& pWTAControlStruct->pCallList, (INT8)iResult);
			}
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else
		{
			/* invalid will be the result if the function fails */
			Var_AssignInvalid( pResult );
		}
		Var_Delete( &pMode );
		OpS_Push( OpS, &pResult );
		return ERR_WAE_WMLS_NONE;
	}

	return ERR_WAE_WMLS_LIB;
}


/* voiceCallRelease(callHandle); */
enumErrorCode 
WTAI_NC_VC_release (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pCallId=OpS_Pop(OpS);

		/* Check if the id parameter is a integer less than 128 (INT8) */
		if ( (Var_Convert(pCallId, typeInteger)==VCR_OK) && (pCallId->val.theInt<128) )
		{
			/* Call wtai function */
			WTAIa_voiceCallRelease ( iObjectID, NULL, (INT8)pCallId->val.theInt );
			eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
		}

		Var_Delete( &pCallId );
		return eResult;
	}
    
	return ERR_WAE_WMLS_LIB;
}
enumErrorCode 
WTAI_NC_VC_release_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
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


/* voiceCallsendDTMF(callHandle, dtmf); */
enumErrorCode 
WTAI_NC_VC_sendDTMF (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	BYTE* pbStr=NULL;
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pDtmf=OpS_Pop(OpS);
		pstructVar pId=OpS_Pop(OpS);

		/* Check if the dtmf parameter is a string and the id is an integer */
		if  (( Var_Convert(pDtmf, typeString)==VCR_OK ) && 
			 ( Var_Convert(pId, typeInteger)==VCR_OK ) &&
			 (pId->val.theInt < 128) )
		{
			BOOL fOverflow=FALSE;

			/* Convert to CHAR */
	   		pbStr = wip_wchar2byte( pDtmf->val.theString, &fOverflow );
			if (! fOverflow)
			{
				/* Call wtai function */
				WTAIa_voiceCallSendDTMF (iObjectID, NULL, (INT8)pId->val.theInt, (const CHAR*)pbStr);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
			DEALLOC( &pbStr );
		}

		Var_Delete( &pDtmf );
		Var_Delete( &pId );
		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_VC_sendDTMF_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
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


CallStatusType WTAI_GetCallStatusType (WCHAR* pwchString, BOOL* pfError)
{
	WCHAR pwchWtai_Number[]={'n','u','m','b','e','r',0};
	WCHAR pwchWtai_Status[]={'s','t','a','t','u','s',0};
	WCHAR pwchWtai_Mode[]={'m','o','d','e',0};
	WCHAR pwchWtai_Name[]={'n','a','m','e',0};
	WCHAR pwchWtai_Duration[]={'d','u','r','a','t','i','o','n',0};

	if (pwchString!=NULL)
	{
		if (COMPARESTRING(pwchString,pwchWtai_Number)==0) 
		{
			return WTA_VC_CS_NUMBER;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Status)==0) 
		{
			return WTA_VC_CS_STATUS;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Mode)==0) 
		{
			return WTA_VC_CS_MODE;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Name)==0) 
		{
			return WTA_VC_CS_NAME;
		}
		else if (COMPARESTRING(pwchString,pwchWtai_Duration)==0) 
		{
			return WTA_VC_CS_DURATION;
		}
	}

/*  Modified by GBU,000524, changed order of statements
	return 0;
	*pfError=TRUE;
*/
    *pfError=TRUE;
    return 0;
}



/* voiceCallCallStatus (callHandle, field); */
enumErrorCode 
WTAI_NC_VC_callStatus (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pField=OpS_Pop(OpS);
		pstructVar pCallId=OpS_Pop(OpS);

		/* Check if the parameter pField is a string and if the pCallId is an 
		   integer (handle to the call) */ 
		if ((Var_Convert(pField, typeString)==VCR_OK)&&(Var_Convert(pCallId, typeInteger)==VCR_OK))
		{
			CallStatusType stField;
			BOOL fError;

			if ( pCallId->val.theInt < 128 )
			{
				/* Transform the field string to an enum */
				stField=WTAI_GetCallStatusType (pField->val.theString,&fError);

				if (!fError)
				{
					/* Call wtai function */
					WTAIa_voiceCallCallStatus ( iObjectID, NULL, (INT8)pCallId->val.theInt,stField);
					eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
				}
			}
		}

		Var_Delete( &pCallId );
		Var_Delete( &pField  );
		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_VC_callStatus_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	enumErrorCode eResult=ERR_WAE_WMLS_LIB;
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( (iResult==WTA_SUCCESS) )
		{
			if (pwcFieldValue==NULL)
			{
				Var_NewString( pResult, 0 ); /* Create an empty string */
			}
			else
			{
				WCHAR* pwcFieldValueCopy=NULL;

				/* Copy string */
				pwcFieldValueCopy = (WCHAR*)(GenCopyString (2,pwcFieldValue));

				if (pwcFieldValueCopy != NULL)
				{
					/* OK - store string in result (string is NOT copied) */
					Var_AssignStringDirect( pResult, 
											STRINGLENGTH( pwcFieldValueCopy ), &pwcFieldValueCopy );
				}
			}
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		/* Push result onto stack */
		OpS_Push( OpS, &pResult );
		eResult=ERR_WAE_WMLS_NONE;
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

	return eResult;
}


/* voiceCallList (returnFirst); */
enumErrorCode 
WTAI_NC_VC_list (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{

		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pReturnFirst=OpS_Pop(OpS);

		/* Check if the parameter if the pCallId is an integer (handle to the call) */ 
		/* Is *typeInteger* the right type? aspn*/
		if (Var_Convert(pReturnFirst, typeInteger)==VCR_OK)
		{
			/* Call wtai function */
			WTAIa_voiceCallList ( iObjectID, NULL, (BOOL)pReturnFirst->val.theInt );
			eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
		}

		Var_Delete( &pReturnFirst );
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}
enumErrorCode 
WTAI_NC_VC_list_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult >= 0 )
		{
			Var_AssignInt(pResult, iResult); /* return a callHandle */
		}
		else
		{
			Var_AssignInvalid(pResult); /* return invalid */
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* Network Text **********************************************************/



/* netTextSend (number, text); */
enumErrorCode 
WTAI_NC_NT_send (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{

		BYTE* pbNumber=NULL;
		BOOL fOverflow=FALSE;
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pText=OpS_Pop(OpS);
		pstructVar pNumber=OpS_Pop(OpS);

		/* Check if the number and text parameters are strings */
		if ((Var_Convert( pNumber, typeString)==VCR_OK) && 
							(Var_Convert( pText, typeString)==VCR_OK))
		{
			/* Convert to CHAR */
	   		pbNumber = wip_wchar2byte( pNumber->val.theString, &fOverflow );
			if (! fOverflow)
			{
				/* Call wtai function */
				WTAIa_netTextSend ( iObjectID, NULL, (CHAR*)pbNumber, pText->val.theString );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
			DEALLOC( &pbNumber );
		}

		Var_Delete( &pNumber );
		Var_Delete( &pText );		
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_NT_send_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
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
		else /* Errorcode OR msgHandle */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}

/* netTextList (returnFirst, messageType); */
enumErrorCode 
WTAI_NC_NT_list (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */

		/* Is *typeInteger* the right type for returnFirst? popping order? aspn*/
		pstructVar pReturnFirst=OpS_Pop(OpS);
		pstructVar pMessageType=OpS_Pop(OpS);

		/* Check if the textIndex is an integer */
		if ( (Var_Convert( pReturnFirst, typeInteger ) == VCR_OK) &&
			 (Var_Convert( pMessageType, typeInteger ) == VCR_OK) )
		{
			/* Call wtai function */
			WTAIa_netTextList ( iObjectID, NULL, (BOOL)pReturnFirst->val.theInt, (INT8)pMessageType->val.theInt );
			eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
		}
		Var_Delete( &pReturnFirst );
		Var_Delete( &pMessageType );

		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_NT_list_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
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
		else /* Errorcode OR msgHandle */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}

/* netTextRemove (msgHandler); */
enumErrorCode 
WTAI_NC_NT_remove (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pMsgHandle=OpS_Pop(OpS);
	

		/* Check if the textIndex is a integer */
		if (Var_Convert( pMsgHandle, typeInteger ) == VCR_OK)
		{
			/* Call wtai function */
			WTAIa_netTextRemove ( iObjectID, NULL, (INT16)pMsgHandle->val.theInt );
			eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
		}

		Var_Delete( &pMsgHandle );
		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_NT_remove_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS )
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

NetTextGetType WTAI_GetTextMessageType (WCHAR* pwchString, BOOL* pfError)
{
	WCHAR pwchWtai_Text[]={'t','e','x','t',0};
	WCHAR pwchWtai_Tstamp[]={'t','s','t','a','m','p',0};
	WCHAR pwchWtai_Address[]={'a','d','d','r','e','s','s',0};
	WCHAR pwchWtai_Status[]={'s','t','a','t','u','s',0};
	WCHAR pwchWtai_Read[]={'r','e','a','d',0};
	WCHAR pwchWtai_TstampOff[]={'t','s','t','a','m','p','_','o','f','f',0};
	WCHAR pwchWtai_TstampDevice[]={'t','s','t','a','m','p','_','d','e','v','i','c','e',0};


	if (pwchString!=NULL)
	{
		if (COMPARESTRING(pwchString,pwchWtai_Text)==0) 
		{
			return WTA_NT_GET_TEXT;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Tstamp)==0) 
		{
			return WTA_NT_GET_TSTAMP;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Address)==0) 
		{
			return WTA_NT_GET_ADDRESS;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Status)==0) 
		{
			return WTA_NT_GET_STATUS;
		}
		else if (COMPARESTRING(pwchString,pwchWtai_Read)==0) 
		{
			return WTA_NT_GET_READ;
		}
		else if (COMPARESTRING(pwchString,pwchWtai_TstampOff)==0) 
		{
			return WTA_NT_GET_TSTAMP_OFF;
		}
		else if (COMPARESTRING(pwchString,pwchWtai_TstampDevice)==0) 
		{
			return WTA_NT_GET_TSTAMP_DEVICE;
		}
	}

/*  Modified by GBU,000524, changed order of statements
	return 0;
	*pfError=TRUE;
*/
    *pfError=TRUE;
    return 0;
}


/* netTextGetFieldValue(msgHandle, field); */
enumErrorCode 
WTAI_NC_NT_getFieldValue (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pField=OpS_Pop(OpS);
		pstructVar pMsgHandle=OpS_Pop(OpS);
	

		/* Check if the pField is a string and the pStruct (the handle) is an integer */
		if ( (Var_Convert( pField, typeString) == VCR_OK ) && 
			 (Var_Convert( pMsgHandle, typeInteger) == VCR_OK ))
		{
			NetTextGetType stField;
			BOOL fError;

			/* Transform the field string to an enum */
			stField=WTAI_GetTextMessageType (pField->val.theString,&fError);

			if (!fError)
			{
				/* Call wtai function */
				WTAIa_netTextGetFieldValue ( iObjectID, NULL, (INT16)pMsgHandle->val.theInt, stField );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}

		Var_Delete( &pField );
		Var_Delete( &pMsgHandle );
		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}
enumErrorCode 
WTAI_NC_NT_getFieldValue_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	enumErrorCode eResult=ERR_WAE_WMLS_LIB;
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( (iResult==WTA_SUCCESS) )
		{
			if ( pwcFieldValue==NULL )
			{
				Var_NewString( pResult, 0 ); /* Create an empty string */
			}
			else
			{
				WCHAR* pwcFieldValueCopy=NULL;

				/* Copy string */
				pwcFieldValueCopy = (WCHAR*)(GenCopyString (2,pwcFieldValue));

				if (pwcFieldValueCopy != NULL)
				{
					/* OK - store string in result (string is NOT copied) */
					Var_AssignStringDirect( pResult, 
											STRINGLENGTH( pwcFieldValueCopy ), &pwcFieldValueCopy );
				}
				else
				{
					Var_AssignInvalid(pResult);
				}
			}
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		/* Push result onto stack */
		OpS_Push( OpS, &pResult );
		eResult=ERR_WAE_WMLS_NONE;
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
	return eResult;
}

/* Phone Book ************************************************************/


/* phoneBookWrite(index, number, name); */
enumErrorCode 
WTAI_NC_PB_write (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pName=OpS_Pop(OpS);
		pstructVar pNumber=OpS_Pop(OpS);
		pstructVar pId=OpS_Pop(OpS);
	
		/* Check if the Id is an integer */
		if (( Var_Convert( pId, typeInteger ) == VCR_OK ) &&
			( Var_Convert( pName, typeString ) == VCR_OK ) &&
			( Var_Convert( pNumber, typeString ) == VCR_OK ))
		{			
			BYTE* pbNumber=NULL;
			BOOL fOverflow;
			
			/* Convert number to CHAR */
	   		pbNumber = wip_wchar2byte( pNumber->val.theString, &fOverflow );
			if (! fOverflow)
			{
				/* Call wtai function */
				WTAIa_phoneBookWrite ( iObjectID, NULL, (INT16)pId->val.theInt, (const CHAR*)pbNumber, (const WCHAR*)pName->val.theString );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
	
			}
			DEALLOC (&pbNumber);
		}

		Var_Delete( &pName );
		Var_Delete( &pNumber );
		Var_Delete( &pId );

		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_PB_write_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_INVALID)
		{
			Var_AssignInvalid(pResult);
		}
		else /* phone book entry index OR error-code*/
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


PhoneBookSearchType WTAI_SearchPhoneBookType (WCHAR* pwchString, BOOL* pfError)
{
	WCHAR pwchWtai_Continue[]={'c','o','n','t','i','n','u','e',0};
	WCHAR pwchWtai_Name[]={'n','a','m','e',0};
	WCHAR pwchWtai_Number[]={'n','u','m','b','e','r',0};

	if (pwchString!=NULL)
	{
		if (COMPARESTRING(pwchString,pwchWtai_Continue)==0) 
		{
			return WTA_PB_SEARCH_CONTINUE;
		}
		else if (COMPARESTRING(pwchString,pwchWtai_Name)==0) 
		{
			return WTA_PB_SEARCH_NAME;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Number)==0) 
		{
			return WTA_PB_SEARCH_NUMBER;
		} 
	}

    *pfError=TRUE;
    return 0;
}

/* phoneBookSearch(field, value); */
enumErrorCode 
WTAI_NC_PB_search (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pValue=OpS_Pop(OpS);
		pstructVar pField=OpS_Pop(OpS);

		/* Check if the textIndex is a integer */
		if ((Var_Convert( pValue, typeString ) == VCR_OK) &&
			(Var_Convert( pField, typeString ) == VCR_OK) )
		{
			BOOL fError=FALSE;
			WCHAR* pwchValue=NULL;
			PhoneBookSearchType stField = WTAI_SearchPhoneBookType (pField->val.theString, &fError);

			if (!fError)
			{
				/* Cut string from variable */
				pwchValue=pValue->val.theString;
				pValue->val.theString=NULL;

				WML_TrimTextWCHAR (pwchValue,TRIMTEXT_BOTH);

				/* Call wtai function */
				WTAIa_phoneBookSearch ( iObjectID, NULL, stField, pwchValue );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;

				DEALLOC (&pwchValue);
			}
		}

		Var_Delete( &pValue );
		Var_Delete( &pField );

		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_PB_search_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
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
		else /* phone book entry index */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


/* phoneBookRemove(index); */
enumErrorCode 
WTAI_NC_PB_remove (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pId=OpS_Pop(OpS);

		/* Check if the parameter if the pId is an integer */ 
		if (Var_Convert(pId, typeInteger)==VCR_OK)
		{
			/* Call wtai function */
			WTAIa_phoneBookRemove ( iObjectID, NULL, (INT16)pId->val.theInt);
			eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
		}

		Var_Delete( &pId );

		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_PB_remove_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS )
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else if ( iResult == WTA_INVALID )
		{
			Var_AssignInvalid(pResult);
		}
		else /* Errorcode */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}


PhoneBookGetType WTAI_GetPhoneBookType (WCHAR* pwchString, BOOL* pfError)
{
	WCHAR pwchWtai_Name[]={'n','a','m','e',0};
	WCHAR pwchWtai_Number[]={'n','u','m','b','e','r',0};

	if (pwchString!=NULL)
	{
		if (COMPARESTRING(pwchString,pwchWtai_Name)==0) 
		{
			return WTA_PB_GET_NAME;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Number)==0) 
		{
			return WTA_PB_GET_NUMBER;
		} 
	}

    *pfError=TRUE;
    return 0;
}


/* phoneBookGetFieldValue(index, field); */
enumErrorCode 
WTAI_NC_PB_getFieldValue (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pField=OpS_Pop(OpS);
		pstructVar pIndex=OpS_Pop(OpS);

		/* Check if the parameters are strings */ 
		if (( Var_Convert (pIndex, typeString ) == VCR_OK ) &&
			( Var_Convert (pField, typeString ) == VCR_OK ) )
		{
			BOOL fError;
			PhoneBookGetType stField = WTAI_GetPhoneBookType ( pField->val.theString, &fError );

			if (!fError)
			{
				/* Call wtai function */
				WTAIa_phoneBookGetFieldValue ( iObjectID, NULL, (INT16)pIndex->val.theInt, stField );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}
		Var_Delete( &pField );
		Var_Delete( &pIndex );

		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}

enumErrorCode 
WTAI_NC_PB_getFieldValue_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	enumErrorCode eResult=ERR_WAE_WMLS_LIB;
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( (iResult==WTA_SUCCESS) )
		{
			if (pwcFieldValue==NULL)
			{
				Var_NewString( pResult, 0 ); /* Create an empty string */
			}
			else
			{
				WCHAR* pwcFieldValueCopy=NULL;

				/* Copy string */
				pwcFieldValueCopy = (WCHAR*)(GenCopyString (2,pwcFieldValue));

				if (pwcFieldValueCopy != NULL)
				{
					/* OK - store string in result (string is NOT copied) */
					Var_AssignStringDirect( pResult, 
											STRINGLENGTH( pwcFieldValueCopy ), &pwcFieldValueCopy );
				}
				else
				{
					Var_AssignInvalid(pResult);
				}
			}
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		/* Push result onto stack */
		OpS_Push( OpS, &pResult );
		eResult=ERR_WAE_WMLS_NONE;
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
	return eResult;
}

PhoneBookChangeType WTAI_ChangePhoneBookType (WCHAR* pwchString, BOOL* pfError)
{
	WCHAR pwchWtai_Name[]={'n','a','m','e',0};
	WCHAR pwchWtai_Number[]={'n','u','m','b','e','r',0};

	if (pwchString!=NULL)
	{
		if (COMPARESTRING(pwchString,pwchWtai_Name)==0) 
		{
			return WTA_PB_CHANGE_NAME;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Number)==0) 
		{
			return WTA_PB_CHANGE_NUMBER;
		} 
	}

    *pfError=TRUE;
    return 0;
}

/* phoneBookChange(index, field, newValue); */
enumErrorCode 
WTAI_NC_PB_change (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pValue=OpS_Pop(OpS);
		pstructVar pField=OpS_Pop(OpS);
		pstructVar pId=OpS_Pop(OpS);

		/* Check if the parameter can be converted */ 
		if (( Var_Convert (pId, typeInteger) == VCR_OK ) &&
			( Var_Convert (pValue, typeString) == VCR_OK ) &&
			( Var_Convert (pField, typeString) == VCR_OK ) )
		{
			BOOL fError;
			PhoneBookChangeType stField = WTAI_ChangePhoneBookType (pField->val.theString, &fError);

			if (!fError)
			{
				/* Call wtai function */
				WTAIa_phoneBookChange ( iObjectID, NULL, (INT16)pId->val.theInt,
											 stField, pValue->val.theString);
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}
		Var_Delete( &pId );
		return eResult;
	}
	return ERR_WAE_WMLS_LIB;
}
enumErrorCode 
WTAI_NC_PB_change_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS )
		{
			Var_NewString( pResult, 0 ); /* Create an empty string */
		}
		else if ( iResult == WTA_INVALID )
		{
			Var_AssignInvalid(pResult);
		}
		else /* Errorcode */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}

/* Call Logs **************************************************************/

/*	Help function for call logs 
	
	iFunction:
	
	  1 = WTAIa_callLogDialled
	  2 = WTAIa_callLogMissed
	  3 = WTAIa_callLogReceived
				
*/
enumErrorCode 
WTAI_NC_CL_HelpFunction (pstructOpS OpS, UINT8 iFunction, UINT8 iObjectID)
{
	if (OpS!=NULL)
	{
		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pReturnFirst=OpS_Pop(OpS);
	
		/* Check if the pReturnFirst is an integer */
		if (Var_Convert( pReturnFirst, typeInteger ) == VCR_OK )
		{
			/* Call wtai function */
			switch (iFunction)
			{
			case 1:
				WTAIa_callLogDialled ( iObjectID, NULL, (BOOL)pReturnFirst->val.theInt );
				break;
			case 2:
				WTAIa_callLogMissed ( iObjectID, NULL, (BOOL)pReturnFirst->val.theInt );
				break;
			case 3:
				WTAIa_callLogReceived ( iObjectID, NULL, (BOOL)pReturnFirst->val.theInt );
				break;
			}	
		}
		Var_Delete( &pReturnFirst );

		return ERR_WAE_WMLS_NONE;
	}

	return ERR_WAE_WMLS_LIB;
}


/* dialled (id); */
enumErrorCode 
WTAI_NC_CL_dialled (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	/* 1 = WTAIa_callLogDialled */
	return WTAI_NC_CL_HelpFunction (OpS, 1, iObjectID);
}
enumErrorCode 
WTAI_NC_CL_dialled_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	iResult=iResult;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_INVALID )
		{
			Var_AssignInvalid(pResult);
		}
		else /* logHandle entry index */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}

/* missed (id); */
enumErrorCode 
WTAI_NC_CL_missed (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	/* 2 = WTAIa_callLogMissed */
	return WTAI_NC_CL_HelpFunction (OpS, 2, iObjectID);
}
enumErrorCode 
WTAI_NC_CL_missed_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	iResult=iResult;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_INVALID )
		{
			Var_AssignInvalid(pResult);
		}
		else /* logHandle entry index */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}

/* received (id); */
enumErrorCode 
WTAI_NC_CL_received (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	/* 3 = WTAIa_callLogReceived */
	return WTAI_NC_CL_HelpFunction (OpS, 3, iObjectID);
}
enumErrorCode 
WTAI_NC_CL_received_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	iResult=iResult;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_INVALID )
		{
			Var_AssignInvalid(pResult);
		}
		else /* logHandle entry index */
		{
			Var_AssignInt(pResult, iResult);
		}

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_NONE;
	}
	return ERR_WAE_WMLS_LIB;
}

CallLogGetType WTAI_GetCallLogType (WCHAR* pwchString, BOOL* pfError)
{
	WCHAR pwchWtai_Number[]={'n','u','m','b','e','r',0};
	WCHAR pwchWtai_Tstamp[]={'t','s','t','a','m','p',0};
	WCHAR pwchWtai_explanation[]={'e','x','p','l','a','n','a','t','i','o','n',0};

	if (pwchString!=NULL)
	{
		if (COMPARESTRING(pwchString,pwchWtai_Number)==0) 
		{
			return WTA_CL_GET_NUMBER;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_Tstamp)==0) 
		{
			return WTA_CL_GET_TSTAMP;
		} 
		else if (COMPARESTRING(pwchString,pwchWtai_explanation)==0) 
		{
			return WTA_CL_GET_EXPLANATION;
		} 
	}

    *pfError=TRUE;
    return 0;
}


/* callLogGetFieldValue(logHandle,field);*/
enumErrorCode 
WTAI_NC_CL_getFieldValue (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pField=OpS_Pop(OpS);
		pstructVar pLogHandle=OpS_Pop(OpS);

		/* Check if the pField is a string and the pStruct (the handle) is an integer */
		if ( (Var_Convert( pField, typeString ) == VCR_OK ) &&
			 (Var_Convert( pLogHandle, typeInteger ) == VCR_OK) )
		{
			CallLogGetType stField;
			BOOL fError;

			/* Transform the field string to an enum */
			stField=WTAI_GetCallLogType (pField->val.theString,&fError);

			if (!fError)
			{
				/* Call wtai function */
				WTAIa_callLogGetFieldValue ( iObjectID, NULL, (INT16)pLogHandle->val.theInt, stField );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}

		Var_Delete( &pField );
		Var_Delete( &pLogHandle );
		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}
enumErrorCode 
WTAI_NC_CL_getFieldValue_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	enumErrorCode eResult=ERR_WAE_WMLS_LIB;
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if (iResult==WTA_SUCCESS)
		{
			if (pwcFieldValue==NULL)
			{
				Var_NewString( pResult, 0 ); /* Create an empty string */
			}
			else
			{
				WCHAR* pwcFieldValueCopy=NULL;

				/* Copy string */
				pwcFieldValueCopy = (WCHAR*)(GenCopyString (2,pwcFieldValue));

				if (pwcFieldValueCopy != NULL)
				{
					/* OK - store string in result (string is NOT copied) */
					Var_AssignStringDirect( pResult, 
											STRINGLENGTH( pwcFieldValueCopy ), &pwcFieldValueCopy );
				}
			}
		}
		else
		{
			Var_AssignInvalid(pResult);
		}

		/* Push result onto stack */
		OpS_Push( OpS, &pResult );
		eResult=ERR_WAE_WMLS_NONE;
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
	return eResult;
}

/* Miscelleaneous *********************************************************/

/* miscSetIndicator(type, newState); */
enumErrorCode 
WTAI_NC_MISC_indication (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct=pWTAControlStruct;

	if (OpS!=NULL)
	{
		enumErrorCode eResult=ERR_WAE_WMLS_NONE;

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pNewState=OpS_Pop(OpS);
		pstructVar pType=OpS_Pop(OpS);

		/* Check if the variables can be converted to integers */
		if ((Var_Convert( pType, typeInteger ) == VCR_OK) &&
			(Var_Convert( pNewState, typeInteger ) == VCR_OK) )
		{
			/* Check if integers within bounds */
			if ((pType->val.theInt >=0) && (pType->val.theInt <= 7 ) &&
				(pNewState->val.theInt >= 0) )
			{
				/* Call wtai function */
				WTAIa_miscSetIndicator( iObjectID, NULL, (INT8)pType->val.theInt,
										(INT8)pNewState->val.theInt );
				eResult=ERR_WAE_WMLS_WTAI_FUNC_CALLED;
			}
		}

		Var_Delete( &pNewState );
		Var_Delete( &pType );

		return eResult;
	}

	return ERR_WAE_WMLS_LIB;
}
enumErrorCode 
WTAI_NC_MISC_indication_finish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue)
{
	pWTAControlStruct=pWTAControlStruct;
	pwcFieldValue=pwcFieldValue;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		if ( iResult == WTA_SUCCESS )
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

/* MiscEndContext ()*/
enumErrorCode 
WTAI_NC_MISC_endcontext (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	iObjectID=iObjectID;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();	

		/* End context () */
		pWTAControlStruct->fEndContext = TRUE;

		Var_NewString( pResult, 0 ); /* Create an empty string */

		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_WTAI_RESULT;
	}
	return ERR_WAE_WMLS_LIB;
}

/* MiscGetProtection () */
enumErrorCode
WTAI_NC_MISC_getProtection (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	iObjectID=iObjectID;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();	

		if (pWTAControlStruct->fContextProtected == TRUE)
		{
			Var_AssignInt(pResult, TRUE);
		}
		else
		{
			Var_AssignInt(pResult, FALSE);
		}
		OpS_Push(OpS, &pResult);
		return ERR_WAE_WMLS_WTAI_RESULT;
	}
	return ERR_WAE_WMLS_LIB;
}

/* MiscSetProtection (mode); */
enumErrorCode 
WTAI_NC_MISC_setProtection (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID)
{
	pWTAControlStruct->fContextProtected = TRUE;
	iObjectID=iObjectID;

	if (OpS!=NULL)
	{
		pstructVar pResult=Var_New();

		/* Get parameters from Opstack NOTE! The "popping"-order is 
		   important */
		pstructVar pMode=OpS_Pop(OpS);
	
		/* Check if the pMode is an integer */
		if (Var_Convert( pMode, typeInteger ) == VCR_OK)
		{
			/* Call wtai function */
			if (pMode->val.theInt==0)
			{
				/* Do not protect context */
				pWTAControlStruct->fContextProtected=FALSE;
			}
			else if (pMode->val.theInt==1)
			{
				/* Protect context */
				pWTAControlStruct->fContextProtected=TRUE;
			}

		}

		Var_NewString( pResult, 0 ); /* Create an empty string */
		/* Push result onto stack */
		OpS_Push( OpS, &pResult );

		Var_Delete( &pMode );
		Var_Delete( &pResult );
		return ERR_WAE_WMLS_WTAI_RESULT;
	}
	
	return ERR_WAE_WMLS_LIB;
}

