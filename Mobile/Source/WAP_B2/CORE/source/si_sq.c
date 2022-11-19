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
/*********
History
-------
990824 (KHN) The interception of the Lang.exit call was not correctly
               getting the WMLBrowser invoked action (i.e. go/prev) if any.
               This is now corrected.

000316 (KHN) Added handling for a new type of returnInfo in the SI struct. 
               (i.e. SI_RETURN_EXTERNAL_NO_PARAMS)
               This was made in order to recognize the situation when
							 an adaptor call has been made and the script is now waiting
							 for a result without passing back parameters to SDL.
						 Also added calling SI_New... functions with invokeID as a new
						   parameter. This was done to enable script lib functions to
							 call adaptorfunctions directly without having to return
							 parameters to SDL when it is possible to make the call in C.

000322 (KHN) Added the function SQ_ProprietaryLibraryFunctionResponse that
               is only available if USE_PROPRIETARY_WMLS_LIBS is defined.

000327 (KHN) Made changes so that if an error was detected when a
               a proprietary script function has responded then
							 this is signalled back to the script Q.
**********/


#include "si_sq.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "si_libs.h"
#endif

/*******************************/


pstructBPElement SQ_NewBPElement( BYTE *BP, UINT32 BPlen, INT16 IANAcharset,
		WCHAR **pCallURL, BOOL firstBPinCallList )
{
	pstructBPElement				result = NEWSTRUCT( structBPElement );
	
	result->BP = BP;
	result->BPlen = BPlen;
	result->IANAcharset = IANAcharset;
	result->callURL = *pCallURL;
	result->isInitialBPInvoked = firstBPinCallList;
	result->theSI = NULL;
	result->next = NULL;

	*pCallURL = NULL;

	return result;
}



VOID SQ_DeleteBPElement( pstructBPElement *pThis )
{
	if (*pThis != NULL)
	{
		DEALLOC( &((*pThis)->BP) );
		DEALLOC( &((*pThis)->callURL) );
		SI_Delete( &((*pThis)->theSI) );

		DEALLOC( pThis );
	}
}




VOID SQ_CreateAndAddBPElement( pstructInvokeHead thisx, BYTE *BP, UINT32 BPlen, 
		INT16 IANAcharset, WCHAR **callURL, BOOL firstBPinCallList )
{
	/* ASSERT( thisx != NULL ) */

	pstructBPElement	newEl = SQ_NewBPElement( BP, BPlen, IANAcharset, callURL, firstBPinCallList );

	newEl->next = thisx->BPcallList;
	thisx->BPcallList = newEl;
}



pstructInvokeHead SQ_NewInvokeHead( pstructSQ thisx, UINT32 UApidInt, pUA theUA, 
		BYTE **pBP, UINT32 BPlen, INT16 IANAcharset, WCHAR **pCallURL )
{
	pstructInvokeHead				result = NEWSTRUCT( structInvokeHead );


	result->invokeID = thisx->nextInvokeID;
	thisx->nextInvokeID = (thisx->nextInvokeID == UINT8_MAX) ? (1) : (thisx->nextInvokeID + 1);
	result->UApidAsInt = UApidInt;
	result->theUAStruct = theUA;
	result->isDone = FALSE;
	result->isInWaitState = FALSE;
	result->errorCode = ERR_WAE_WMLS_NONE;
	result->theResult = NULL;
	result->browserPrev = FALSE;
	result->browserGoUrl = NULL;
	result->BPcallList = NULL;

	SQ_CreateAndAddBPElement( result, *pBP, BPlen, IANAcharset, pCallURL, TRUE );
	*pBP = NULL;

	result->next = NULL;

	return result;
}



VOID SQ_ClearBPCallList( pstructInvokeHead thisx )
{
	pstructBPElement			el = NULL;
	pstructBPElement			tmpEl = NULL;

	if (thisx != NULL)
	{
		el = thisx->BPcallList;
		thisx->BPcallList = NULL;
		while (el != NULL)
		{
			tmpEl = el->next;
			SQ_DeleteBPElement( &el );
			el = tmpEl;
		}
	}
}



VOID SQ_DeleteInvokeHead( pstructInvokeHead *pThis )
{
	if (*pThis != NULL)
	{
		Var_Delete( &( (*pThis)->theResult ) );
		DEALLOC( &( (*pThis)->browserGoUrl ) );
		SQ_ClearBPCallList( *pThis );

		DEALLOC( pThis );
	}
}



VOID SQ_RemoveAndDeleteInvokeHeadFromQ( pstructSQ theSQ, pstructInvokeHead *pThis )
{
	pstructInvokeHead				theInvokeBeforeThis = NULL;

	if (theSQ != NULL)
	{
		if (theSQ->invokeList == *pThis)
		{	/* the first InvokeHead is to be removed */
			theSQ->invokeList = theSQ->invokeList->next;
		}
		else
		{
			theInvokeBeforeThis = theSQ->invokeList;
			if (theInvokeBeforeThis != NULL)
			{
				while ((theInvokeBeforeThis->next != NULL) && (theInvokeBeforeThis->next != *pThis))
				{
					theInvokeBeforeThis = theInvokeBeforeThis->next;
				}

				if (theInvokeBeforeThis->next != NULL)
				{	/* found the invokeHead */
					/* take it out of the q */
					theInvokeBeforeThis->next = theInvokeBeforeThis->next->next;
				}
			}
		}
	}
	SQ_DeleteInvokeHead( pThis );
}


pstructInvokeHead SQ_FindInvokeHead( pstructSQ thisx, UINT8 invokeID )
{
	pstructInvokeHead			invokeEl = NULL;

	
	if (thisx == NULL)
	{	/* error! */
		return NULL;
	}

	invokeEl = thisx->invokeList;
	while ((invokeEl != NULL) && (invokeEl->invokeID != invokeID))
	{
		invokeEl = invokeEl->next;
	}

	return invokeEl;
}

/*******************************/



pstructSQ SQ_Init( UINT16 timeSlice, BOOL roundRobin, BOOL oneScriptPerUA, BOOL handlesTopPrio )
{
	pstructSQ		result = NEWSTRUCT( structSQ );

	if (result != NULL)
	{
		result->timeSlice = timeSlice;
		result->nextInvokeID = 1;					/* the next invoke will get this invokeID */
		result->doRoundRobinScheduling = roundRobin;
		result->onlyOneScriptPerUA = oneScriptPerUA;
		result->handlesTopPriorityREQ = handlesTopPrio;
		result->currentInvoke = NULL;
		result->invokeList = NULL;
	}
	return result;
}


VOID SQ_Terminate( pstructSQ *pThis )
{
	pstructInvokeHead			el = NULL;
	pstructInvokeHead			next = NULL;
	
	if (*pThis != NULL)
	{
		/* first clear the InvokeList */
		el = (*pThis)->invokeList;
		while (el != NULL)
		{
			next = el->next;
			SQ_DeleteInvokeHead( &el );
			el = next;
		}

		/* now dealloc the SQ itself */
		DEALLOC( pThis );
	}
}




UINT8 SQ_InvokeScript( pstructSQ thisx, UINT32 UApidInt, pUA theUA, BYTE **pBP,
		UINT32 BPlen, INT16 IANAcharset, WCHAR **pCallURL, BOOL isTopPriority )
/* returns 0 if something went wrong */
{
	UINT8									resultInvokeID = 0;
	pstructInvokeHead			el = NULL;
	pstructInvokeHead			newEl = NULL;
	BOOL									uaHasScriptInvoke = FALSE;

		/*
		if the UA has a script invokation in the queue (Q)
		This variable is only important if onlyOneScriptPerUA is set to TRUE for the scriptQ
		*/

	if ((thisx != NULL) && (theUA != NULL) && (*pBP != NULL) && (*pCallURL != NULL) )
	{
		newEl = SQ_NewInvokeHead( thisx, UApidInt, theUA, pBP, BPlen, IANAcharset, pCallURL );
		if (newEl == NULL)
		{	/* something went wrong */
			return 0;
		}
		/* find last element in q and check if the UA has more invokations in the q */
		if (thisx->invokeList != NULL)
		{
			el = thisx->invokeList;
			while (el->next != NULL)
			{
				uaHasScriptInvoke = (uaHasScriptInvoke) ? (TRUE):(UApidInt == el->UApidAsInt);
				el = el->next;
			}
			/* last element must also be checked */
			uaHasScriptInvoke = (uaHasScriptInvoke) ? (TRUE):(UApidInt == el->UApidAsInt);
		}
		/* el is either NULL (no invokes in q) or pointing to the last in the invoke list */

		if (uaHasScriptInvoke && thisx->onlyOneScriptPerUA)
		{	/* the UA has already one script invoked and additional invokes are not allowed! */
			SQ_DeleteInvokeHead( &newEl );
			return 0; /* ERROR! */
		}

		if (isTopPriority && thisx->handlesTopPriorityREQ)
		{	/* if top priority then this invoke will be inserted first in the q */
			newEl->next = thisx->invokeList;
			thisx->invokeList = newEl;
			thisx->currentInvoke = NULL;	/* this will make this invoke the next script run */

			resultInvokeID = newEl->invokeID;
			newEl = NULL;
		}
		else
		{	/* insert invoke last in the q */

			if (el != NULL)
			{
				el->next = newEl;
			}
			else
			{	/* no invokes in the q */
				thisx->invokeList = newEl;
			}
			resultInvokeID = newEl->invokeID;
			newEl = NULL;
		}
	}
	else
	{
		/* couldn't invoke script so memory must be deallocated */
		DEALLOC( pBP );
		DEALLOC( pCallURL );
	}

	return resultInvokeID;
}



VOID SQ_AbortScriptsForSpecificUA( pstructSQ thisx, UINT32 UApidInt )
{
	pstructInvokeHead			el = NULL;
	pstructInvokeHead			lastEl = NULL;

	if (thisx != NULL)
	{
		el = thisx->invokeList;
		while (el != NULL)
		{
			if (el->UApidAsInt == UApidInt)
			{	/* this invoke is to be removed */

				if (el == thisx->currentInvoke)
				{
					if (thisx->doRoundRobinScheduling)
					{	/* make the previous invoke the current since it will be advanced to the next when scheduling */
						thisx->currentInvoke = lastEl;
					}
					else
					{	/* make NULL the current. This will make the first in the list the next to be taken care of */
						thisx->currentInvoke = NULL;
					}
				}

				/* remove it from the q */
				if (lastEl == NULL)
				{	/* this invoke is first in the list */
					thisx->invokeList = el->next;
					SQ_DeleteInvokeHead( &el );
					el = thisx->invokeList;
				}
				else
				{	/* normal remove */
					lastEl->next = el->next;
					SQ_DeleteInvokeHead( &el );
					el = lastEl->next;
				}

				if (thisx->onlyOneScriptPerUA)
				{	/* there can be only one invoke from this UA thus there is no need to search any further */
					return;
				}
			}
			else
			{	/* el is not an invoke to be aborted (removed from the q) */
				lastEl = el;
				el = el->next;
			}
		}

	}
}




enumScriptStatus SQ_Exec( pstructSQ thisx, UINT8 *pInvokeIDThatExecuted, UINT32 *pUApidInt )
{
	pstructInvokeHead			invokeEl = NULL;
	pstructBPElement			BPel = NULL;
	BYTE									*theBP = NULL;
	UINT32								BPlen = 0;
	INT16									IANAcharset = -1;
	enumErrorCode					siErrCode;

	WCHAR									*baseURL = NULL;
	UINT32								baseURLlen = 0;
	UINT32								i = 0;

	WCHAR									*goURL = NULL;
	BOOL									goPrev = FALSE;
	pstructVar						siResult = NULL;

	WCHAR									*callURL = NULL;

	BYTE									*funcName = NULL;
	pstructOpS						argOpS = NULL;


	/*first make sure there is invokes to be procesed */
	if (thisx->invokeList == NULL)
	{
		/* signal that there is nothing to proccess for now */
		*pInvokeIDThatExecuted = 0;
		return eSs_more;
	}

	/* find the InvokeHead to execute a block of script for */
	if (thisx->doRoundRobinScheduling)
	{
		/* schedule to the next invoke */
		if ((thisx->currentInvoke != NULL) && (thisx->currentInvoke->next != NULL))
		{
			thisx->currentInvoke = thisx->currentInvoke->next;
		}
		else
		{	/* reached the end of the q and thus the first becomes the current invoke */
			thisx->currentInvoke = thisx->invokeList;
		}
	}
	else
	{	/*  */
		thisx->currentInvoke = thisx->invokeList;
	}
	invokeEl = thisx->currentInvoke;

	/* make sure that we have an invoke that is ready to run */
	if (thisx->doRoundRobinScheduling)
	{
		/* traverse the q until a runnable invoke is found */
		while (invokeEl->isDone || invokeEl->isInWaitState)
		{
			invokeEl = invokeEl->next;
		}
		thisx->currentInvoke = invokeEl;

		if (invokeEl == NULL)
		{	/* we have come to the end of the q */
			/* signal that there is nothing to proccess for now */
			*pInvokeIDThatExecuted = 0;
			return eSs_more;
		}
	}
	else
	{
		if (invokeEl->isDone && invokeEl->errorCode != ERR_WAE_WMLS_NONE)
		{
			/* signal that there is an script that failed */
			*pInvokeIDThatExecuted = invokeEl->invokeID;
			*pUApidInt = invokeEl->UApidAsInt;
			return eSs_error;
		}
		else if (invokeEl->isInWaitState)
		{
			/* signal that there is nothing to proccess for now */
			*pInvokeIDThatExecuted = 0;
			return eSs_more;
		}
	}

	/* Ok. We have now found an invoke that is ready to be executed */

	/* first see if we must create a SI or there is one ready to run directly */
	/* ASSERT( invokeEl->BPcallList != NULL) */
	if (invokeEl->BPcallList->theSI == NULL)
	{
		/* we must create an SI */
		/* first get the BP */
		BPlen = invokeEl->BPcallList->BPlen;
		theBP = invokeEl->BPcallList->BP;
		IANAcharset = invokeEl->BPcallList->IANAcharset;

		if ((theBP == NULL) || (BPlen == 0))
		{
			/* incorrect BP! The BPcallList must be emptied and ERR_WAE_WMLS_VERIFICATION returned */
			invokeEl->isDone = TRUE;
			invokeEl->errorCode = ERR_WAE_WMLS_VERIFICATION;
			SQ_ClearBPCallList( invokeEl );
			*pInvokeIDThatExecuted = invokeEl->invokeID;
			*pUApidInt = invokeEl->UApidAsInt;

			return eSs_error;
		}

		if (invokeEl->BPcallList->isInitialBPInvoked)
		{	/* first BP invoked */

			if ( invokeEl->theUAStruct->pbURLBase != NULL )
			{
				baseURLlen = B_STRINGLENGTH( (const char *)invokeEl->theUAStruct->pbURLBase );

				baseURL = NEWARRAY( WCHAR, baseURLlen + 1 );
				baseURL[ baseURLlen ] = 0;
				for (i = 0; i < baseURLlen; i++)
				{
					baseURL[i] = (WCHAR)( invokeEl->theUAStruct->pbURLBase[i] );
				}

			}
			
			invokeEl->BPcallList->theSI = SI_NewWithArgString( invokeEl->invokeID, theBP, BPlen, IANAcharset, invokeEl->theUAStruct, invokeEl->BPcallList->callURL,
						baseURL, thisx->timeSlice, &siErrCode );

			DEALLOC( &baseURL );
		}
		else
		{	/* this BP called from another SI in the BPCallStack */
			/* then get funcName and args from the next BP in the BPCallList */
			/* check to see that these params exist */
			if ((invokeEl->BPcallList->next->theSI != NULL) &&
					SI_GetExtCalllParams_rest( invokeEl->BPcallList->next->theSI,
																			&funcName, &argOpS ) )
			{
				/* we have everything we need to build an SI */
				invokeEl->BPcallList->theSI = SI_NewWithArgOpS( invokeEl->invokeID, theBP, BPlen, IANAcharset,
        		invokeEl->theUAStruct, &funcName, &argOpS, invokeEl->BPcallList->callURL,
            invokeEl->BPcallList->next->theSI->baseURL,
						thisx->timeSlice, &siErrCode  );
			}
			/* ELSE the SI is still NULL and verification error occurs */
		}

		if (invokeEl->BPcallList->theSI == NULL)
		{
			/* BP incorrect (or the call function didn't exist/have the right number of args) */
			invokeEl->isDone = TRUE;
			invokeEl->errorCode = siErrCode;
			SQ_ClearBPCallList( invokeEl );
			*pInvokeIDThatExecuted = invokeEl->invokeID;
			*pUApidInt = invokeEl->UApidAsInt;

			return eSs_error;
		}
	}
	/* we now have an SI ready to run */

	siErrCode = SI_ExecBlock( invokeEl->BPcallList->theSI );


	/* if the error was is EXIT then it actually not really an error but it means that the
  execution should be aborted and a return value and control should be given back to the calling UA.
  Any Go or prev values should be correctly fetched. */
	if (siErrCode == ERR_WAE_WMLS_EXIT)
	{	/* if the WMLS lib func Lang.exit was called */
		pstructBPElement	pEl = NULL;
		pstructSI					lastSIWithBrowserAction = NULL;
    BOOL 							fDone;

		invokeEl->isDone = TRUE;
		siErrCode = ERR_WAE_WMLS_NONE;

		siResult = SI_GetReturnVal( invokeEl->BPcallList->theSI );

		invokeEl->isDone = TRUE;
		invokeEl->errorCode = siErrCode;
		invokeEl->theResult = siResult;
		siResult = NULL;
		invokeEl->browserGoUrl = NULL;
		invokeEl->browserPrev = FALSE;

    /* get the browser action invoked, if any */
    /* have to go through the BP list and get the last
       BP which invoked a browser action */
    /* since it is a stack then we only have to find the first one (i.e the latest) */
		fDone = FALSE;
    pEl = invokeEl->BPcallList;
    while (pEl != NULL && !fDone )
    {
    	if (pEl->theSI && pEl->theSI->WMLBrowserActionInvoked)
      {
      	lastSIWithBrowserAction = pEl->theSI;
        fDone = TRUE;
      }
			pEl = pEl->next;      
    }
    if (lastSIWithBrowserAction != NULL)
    {	/* there was a browser action invoked so get it */
			if (lastSIWithBrowserAction->WMLBrowser_goURL != NULL)
      {
      	invokeEl->browserGoUrl = lastSIWithBrowserAction->WMLBrowser_goURL;
				lastSIWithBrowserAction->WMLBrowser_goURL = NULL;
      }
      else
      {
      	invokeEl->browserPrev = lastSIWithBrowserAction->WMLBrowser_prev;
      }
    }

		SQ_ClearBPCallList( invokeEl );
		*pInvokeIDThatExecuted = invokeEl->invokeID;
		*pUApidInt = invokeEl->UApidAsInt;

		if (invokeEl->browserPrev)
		{
			return eSs_done_prev;
		}
		else if (invokeEl->browserGoUrl != NULL)
		{
			return eSS_done_go;
		}
		else
		{
			return eSs_done;
		}
	}

	if (siErrCode != ERR_WAE_WMLS_NONE)
	{	/* if an error has occured */
		invokeEl->isDone = TRUE;
		invokeEl->errorCode = siErrCode;
		SQ_ClearBPCallList( invokeEl );
		*pInvokeIDThatExecuted = invokeEl->invokeID;
		*pUApidInt = invokeEl->UApidAsInt;

		return eSs_error;
	}
	else
	{	/* no error */
		switch (invokeEl->BPcallList->theSI->returnInfo)
		{
			case SI_RETURN_DONE:
				if (invokeEl->BPcallList->theSI->WMLBrowserActionInvoked)
				{	/* get the go/prev parameters */
					goURL = invokeEl->BPcallList->theSI->WMLBrowser_goURL;
					invokeEl->BPcallList->theSI->WMLBrowser_goURL = NULL;		/* this has been taken over */
					goPrev = invokeEl->BPcallList->theSI->WMLBrowser_prev;
				}
				else
				{	/* no get/prev parameters */
					goURL = NULL;
					goPrev = FALSE;
				}
				siResult = SI_GetReturnVal( invokeEl->BPcallList->theSI );
				if (siResult == NULL)
				{	/* ERROR: stack underflow! */
					DEALLOC( &goURL );
					invokeEl->isDone = TRUE;
					invokeEl->errorCode = ERR_WAE_WMLS_STACK_UNDERFLOW;
					SQ_ClearBPCallList( invokeEl );
					*pInvokeIDThatExecuted = invokeEl->invokeID;
					*pUApidInt = invokeEl->UApidAsInt;

					return eSs_error;
				}

				/* ELSE: OK. We have now extracted results from the SI  */
				/* now we must put the result to the caller (another si or else the invoke is done) */

				if (invokeEl->BPcallList->next != NULL)
				{	/* another si must be given the results and the old si must be removed */
					/* ASSERT( invokeEl->BPcallList->next->theSI != NULL ) */
					SI_ReceiveCalledSIReturnVal( invokeEl->BPcallList->next->theSI, &siResult,
																				invokeEl->BPcallList->theSI->WMLBrowserActionInvoked,
																				goPrev, &goURL );
					/* now remove the old BPelement with the finished SI */
					BPel = invokeEl->BPcallList;
					invokeEl->BPcallList = invokeEl->BPcallList->next;
					SQ_DeleteBPElement( &BPel );

					*pInvokeIDThatExecuted = invokeEl->invokeID;
					*pUApidInt = invokeEl->UApidAsInt;
					return eSs_more;
				}
				else
				{	/* the invoke is done and the results must be stored in the invokeHead */
					invokeEl->isDone = TRUE;
					invokeEl->errorCode = ERR_WAE_WMLS_NONE;
					invokeEl->theResult = siResult;
					siResult = NULL;
					invokeEl->browserGoUrl = goURL;
					goURL = NULL;
					invokeEl->browserPrev = goPrev;

					SQ_ClearBPCallList( invokeEl );
					*pInvokeIDThatExecuted = invokeEl->invokeID;
					*pUApidInt = invokeEl->UApidAsInt;

					if (invokeEl->browserPrev)
					{
						return eSs_done_prev;
					}
					else if (invokeEl->browserGoUrl != NULL)
					{
						return eSS_done_go;
					}
					else
					{
						return eSs_done;
					}
				}

			case SI_RETURN_MORE:
				/* just return control */
				*pInvokeIDThatExecuted = invokeEl->invokeID;
				*pUApidInt = invokeEl->UApidAsInt;
				return eSs_more;

			case SI_RETURN_EXTERNAL:
				invokeEl->isInWaitState = TRUE;
				*pInvokeIDThatExecuted = invokeEl->invokeID;
				*pUApidInt = invokeEl->UApidAsInt;
				/* ASSERT( invokeEl->BPcallList->theSI->returnParams != NULL ) */
				if (invokeEl->BPcallList->theSI->returnParams->islibCall)
				{
					return eSs_lib;
				}
				else
				{
					/* a call to a new BP has been made thus a new BPelement must be created to hold
						the callUrl and to later be filled with BP and its SI */
					if (SI_GetExtCalllParams_URL( invokeEl->BPcallList->theSI, &callURL ))
					{
						SQ_CreateAndAddBPElement( invokeEl, NULL, 0, IANA_CHARSET_INVALID,
		            &callURL, FALSE );
						return eSs_extFunc;
					}
					else
					{	/* ERROR: no call url for the external call */
						invokeEl->isDone = TRUE;
						invokeEl->errorCode = ERR_WAE_WMLS_LOAD;
						SQ_ClearBPCallList( invokeEl );
						*pInvokeIDThatExecuted = invokeEl->invokeID;
						*pUApidInt = invokeEl->UApidAsInt;
											
						return eSs_error;
					}
				}

			case SI_RETURN_EXTERNAL_NO_PARAMS:
				/* signal that there is nothing to proccess for now, not until there is an external response (e.g. through an adaptor call) */
				invokeEl->isInWaitState = TRUE;
				*pInvokeIDThatExecuted = 0;
				return eSs_more;

			default:
				/* an error has occured since the returnInfo value was incorrect */
				invokeEl->isDone = TRUE;
				invokeEl->errorCode = siErrCode;
				SQ_ClearBPCallList( invokeEl );
				*pInvokeIDThatExecuted = invokeEl->invokeID;
				*pUApidInt = invokeEl->UApidAsInt;

				return eSs_error;

		}
	}
}



BOOL SQ_GetResult( pstructSQ thisx, UINT8 invokeID, WCHAR **pResultStr )
/* the pResultStr must be NULL!!! */
{
	pstructVar						theResultVar = NULL;
	pstructInvokeHead			invokeEl = NULL;

	
	if ((thisx == NULL) || (*pResultStr != NULL))
	{	/* error! */
		return FALSE;
	}

	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return FALSE;
	}

	if (( ! invokeEl->isDone) || (invokeEl->theResult == NULL))
	{	/* no return value to collect */
		return FALSE;
	}

	theResultVar = invokeEl->theResult;
	invokeEl->theResult = NULL;	/* we are now the owner of the var  */

	if ((invokeEl->browserGoUrl == NULL) && ( ! invokeEl->browserPrev))
	{	/* no more results to retrieve and the invoke is removed */
		SQ_RemoveAndDeleteInvokeHeadFromQ( thisx, &invokeEl );
	}

	/* now convert the var to a string */
	if (theResultVar->type == typeInvalid) {
		Var_NewString( theResultVar, 7 );
		theResultVar->val.theString[0] = WC('i');
		theResultVar->val.theString[1] = WC('n');
		theResultVar->val.theString[2] = WC('v');
		theResultVar->val.theString[3] = WC('a');
		theResultVar->val.theString[4] = WC('l');
		theResultVar->val.theString[5] = WC('i');
		theResultVar->val.theString[6] = WC('d');
	}
	else {
		if ( ! (VCR_OK == Var_Convert( theResultVar, typeString ))) {
			/* could not convert */
			Var_Delete( &theResultVar );
			return FALSE;
		}
	}

	/* now snatch the string... */ 
	*pResultStr = theResultVar->val.theString;

	/* ...and dealloc the variable */
	theResultVar->val.theString = NULL;
	theResultVar->theStringLen = 0;
	Var_Delete( &theResultVar );

	/* done! */
	return TRUE;
}


BOOL SQ_GetPrevParams( pstructSQ thisx, UINT8 invokeID )
{
	pstructInvokeHead			invokeEl = NULL;


	if (thisx == NULL)
	{	/* error! */
		return FALSE;
	}

	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return FALSE;
	}

	if (( ! invokeEl->isDone) || ( ! invokeEl->browserPrev))
	{	/* no value to collect */
		return FALSE;
	}

	/* the one getting the params are now the owner of the strings  */

	if (invokeEl->theResult == NULL)
	{	/* no more results to retrieve and the invoke is removed */
		SQ_RemoveAndDeleteInvokeHeadFromQ( thisx, &invokeEl );
	}

	/* done! */
	return TRUE;
}


BOOL SQ_GetGoParams( pstructSQ thisx, UINT8 invokeID, BYTE **pGoUrl )
{
	pstructInvokeHead			invokeEl = NULL;
  BOOL									fDummy;


	if ((thisx == NULL) || (*pGoUrl != NULL))
	{	/* error! */
		return FALSE;
	}

	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return FALSE;
	}

	if (( ! invokeEl->isDone) || (invokeEl->browserPrev))
	{	/* no value to collect */
		return FALSE;
	}

	/* make a BYTE* copy that the receiver is responsible for destroying */
	*pGoUrl = wip_wchar2byte( invokeEl->browserGoUrl, &fDummy );	/* the fDummy is used beacuse we don't care about overflow */
	DEALLOC( &invokeEl->browserGoUrl ); /* original not needed any more */

	if (invokeEl->theResult == NULL)
	{	/* no more results to retrieve and the invoke is removed */
		SQ_RemoveAndDeleteInvokeHeadFromQ( thisx, &invokeEl );
	}

	/* done! */
	return TRUE;
}



BOOL SQ_GetError( pstructSQ thisx, UINT8 invokeID, enumErrorCode *errCode )
{
	pstructInvokeHead			invokeEl = NULL;


	if (thisx == NULL)
	{	/* error! */
		return FALSE;
	}

	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return FALSE;
	}

	if (( ! invokeEl->isDone) || (invokeEl->errorCode == ERR_WAE_WMLS_NONE))
	{	/* no value to collect */
		return FALSE;
	}

	*errCode = invokeEl->errorCode;

	SQ_RemoveAndDeleteInvokeHeadFromQ( thisx, &invokeEl );

	/* done! */
	return TRUE;
}



BOOL SQ_GetExternalCallParams( pstructSQ thisx, UINT8 invokeID, BYTE **pTheURL )
{
	pstructInvokeHead			invokeEl = NULL;
	BOOL									fDummy;
	
	if ((thisx == NULL) || (*pTheURL != NULL))
	{	/* ERROR: not all parameters are correct! */
		return FALSE;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return FALSE;
	}

	/* now see if there is an SI with external params */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI == NULL) &&
			( ! invokeEl->BPcallList->isInitialBPInvoked) &&
			(invokeEl->BPcallList->callURL != NULL))
	{
		/* everything went fine */
		/* make a BYTE* copy of the URL. (The original is still needed) */
		*pTheURL = wip_wchar2byte( invokeEl->BPcallList->callURL , &fDummy); /* we dont' care about overflow, that's why we use fDummy */
		return TRUE;
	}
	else
	{	/* something went wrong */
		return FALSE;
	}
}



BOOL SQ_GetLibCallParams( pstructSQ thisx, UINT8 invokeID, UINT16 *pLibIndex, UINT8 *pFuncIndex, 
															pstructOpS *pArgOpS )
{
	pstructInvokeHead			invokeEl = NULL;

	
	if ((thisx == NULL) || (*pArgOpS != NULL))
	{	/* ERROR: not all parameters are correct! */
		return FALSE;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return FALSE;
	}

	/* now see if we can retrieve the SI external params */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI != NULL) &&
			(SI_GetLibCalllParams( invokeEl->BPcallList->theSI, pLibIndex, pFuncIndex, pArgOpS ) ) )
	{
		/* everything went fine */
		return TRUE;
	}
	else
	{	/* something went wrong */
		return FALSE;
	}
}



BOOL SQ_GetAnArgumentAsString( pstructOpS argOpS, WCHAR **pArgAsStr )
{
	pstructVar			theArgument = NULL;
	
	if ((argOpS == NULL) || (*pArgAsStr != NULL))
	{
		return FALSE;
	}

	theArgument = OpS_Pop( argOpS );

	if ((theArgument != NULL) && 
			(VCR_OK == Var_Convert( theArgument, typeString )))
	{
		/* now snatch the string... */ 
		*pArgAsStr = theArgument->val.theString;

		/* ...and dealloc the variable */
		theArgument->val.theString = NULL;
		theArgument->theStringLen = 0;
		Var_Delete( &theArgument );

		/* done! */
		return TRUE;
	}
	else
	{
		Var_Delete( &theArgument );
		return FALSE;
	}
}



VOID SQ_ReturnValueToScript( pstructSQ thisx, UINT8 invokeID, WCHAR **theValueStr )
/* this function becomes the owner of theValueStr  */
{
	SQ_ReturnStringValueToScript( thisx, invokeID, theValueStr );
}


VOID SQ_ReturnVarValueToScript( pstructSQ thisx, UINT8 invokeID, pstructVar *ppResultVar )
/* this function becomes the owner of theValueStr  */
{
	pstructInvokeHead			invokeEl = NULL;

	
	if (thisx == NULL)
	{	/* ERROR: not all parameters are correct! */
		Var_Delete( ppResultVar );
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return;
	}

	/* now see if we have an SI that is waiting for an extrenal result */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI != NULL) &&
			(invokeEl->BPcallList->theSI->inWaitState) )
	{
		SI_ReceiveReturnVal( invokeEl->BPcallList->theSI, ppResultVar );

		invokeEl->isInWaitState = FALSE;
		/* ready to execute again */

		/* done! */
		Var_Delete( ppResultVar );	/* should now be NULL but this is just in case */
		return;
	}
	else
	{	/* something went wrong */
		Var_Delete( ppResultVar );
		return;
	}
}


VOID SQ_ReturnStringValueToScript( pstructSQ thisx, UINT8 invokeID, WCHAR **theValueStr )
/* this function becomes the owner of theValueStr  */
{
	pstructInvokeHead			invokeEl = NULL;
	pstructVar						resultVar;
	UINT32								strLen = 0;

	
	if (thisx == NULL)
	{	/* ERROR: not all parameters are correct! */
		DEALLOC( theValueStr );
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return;
	}

	/* now see if we have an SI that is waiting for an extrenal result */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI != NULL) &&
			(invokeEl->BPcallList->theSI->inWaitState) )
	{
		strLen = (*theValueStr == NULL) ? (0) : (STRINGLENGTH( *theValueStr ));
		resultVar = Var_New();
		Var_AssignStringDirect( resultVar, strLen, theValueStr );
		SI_ReceiveReturnVal( invokeEl->BPcallList->theSI, &resultVar );

		invokeEl->isInWaitState = FALSE;
		/* ready to execute again */

		/* done! */
		Var_Delete( &resultVar );	/* theValueStr should now be NULL but this is just in case */
		DEALLOC( theValueStr );		/* theValueStr should now be NULL but this is just in case */
		return;
	}
	else
	{	/* something went wrong */
		DEALLOC( theValueStr );
		return;
	}
}


VOID SQ_ReturnBoolValueToScript( pstructSQ thisx, UINT8 invokeID, BOOL theValue )
{
	pstructInvokeHead			invokeEl = NULL;
	pstructVar						resultVar;

	
	if (thisx == NULL)
	{	/* ERROR: not all parameters are correct! */
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return;
	}

	/* now see if we have an SI that is waiting for an extrenal result */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI != NULL) &&
			(invokeEl->BPcallList->theSI->inWaitState) )
	{
		resultVar = Var_New();
		Var_AssignBool( resultVar, theValue );
		SI_ReceiveReturnVal( invokeEl->BPcallList->theSI, &resultVar );

		invokeEl->isInWaitState = FALSE;
		/* ready to execute again */

		/* done! */
		Var_Delete( &resultVar );	/* theValueStr should now be NULL but this is just in case */
		return;
	}
}

VOID SQ_ReturnIntValueToScript( pstructSQ thisx, UINT8 invokeID, INT32 theValue )
{
	pstructInvokeHead			invokeEl = NULL;
	pstructVar						resultVar;

	
	if (thisx == NULL)
	{	/* ERROR: not all parameters are correct! */
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		return;
	}

	/* now see if we have an SI that is waiting for an extrenal result */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI != NULL) &&
			(invokeEl->BPcallList->theSI->inWaitState) )
	{
		resultVar = Var_New();
		Var_AssignInt( resultVar, theValue );
		SI_ReceiveReturnVal( invokeEl->BPcallList->theSI, &resultVar );

		invokeEl->isInWaitState = FALSE;
		/* ready to execute again */

		/* done! */
		Var_Delete( &resultVar );	/* theValueStr should now be NULL but this is just in case */
		return;
	}
}


VOID SQ_LoadErrorReceived( pstructSQ thisx, UINT8 invokeID )
{
	pstructInvokeHead			invokeEl = NULL;

	
	if (thisx == NULL)
	{	/* can't do anything */
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if ((invokeEl != NULL) &&
			(invokeEl->isInWaitState) )
	{
		invokeEl->isInWaitState = FALSE;
		invokeEl->isDone = TRUE;
		invokeEl->errorCode = ERR_WAE_WMLS_LOAD;
		SQ_ClearBPCallList( invokeEl );
	}
}



VOID SQ_LoadDoneReceived( pstructSQ thisx, UINT8 invokeID, BYTE **pBP, UINT32 BPlen, INT16 IANAcharset )
{
	pstructInvokeHead			invokeEl = NULL;

	if (thisx == NULL)
	{	/* can't do anything */
		/* dealloc the BP */
		DEALLOC( pBP );
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if ((invokeEl != NULL) &&
			(invokeEl->isInWaitState) &&
			(invokeEl->BPcallList != NULL) &&
			(invokeEl->BPcallList->theSI == NULL) &&
			(invokeEl->BPcallList->BP == NULL) &&
			(invokeEl->BPcallList->isInitialBPInvoked == FALSE) )
	{

		invokeEl->BPcallList->BPlen = BPlen;
		invokeEl->BPcallList->BP = *pBP;
  	invokeEl->BPcallList->IANAcharset = IANAcharset;
		*pBP = NULL;
		invokeEl->isInWaitState = FALSE;
		/* ready to execute again */
	}	
	else
	{
		/* dealloc the BP */
		DEALLOC( pBP );
	}
}


VOID SQ_LoadStringDone( pstructSQ thisx, UINT8 invokeID, INT16 charSet, BYTE **pValueStr, UINT32 strLen )
/* this function becomes the owner of theValueStr  */
{
	pstructInvokeHead			invokeEl = NULL;

	if (thisx == NULL)
	{	/* ERROR: not all parameters are correct! */
		DEALLOC( pValueStr );
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		DEALLOC( pValueStr );
		return;
	}

	/* now see if we have an SI that is waiting for an extrenal result */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI != NULL) &&
			(invokeEl->BPcallList->theSI->inWaitState) )
	{
		SI_LoadStringDone( invokeEl->BPcallList->theSI, charSet, pValueStr, strLen );

		invokeEl->isInWaitState = FALSE;

		DEALLOC( pValueStr );		/* theValueStr should now be NULL but this is just in case */
		return;
	}
	else
	{	/* something went wrong */
		DEALLOC( pValueStr );
		return;
	}
}



VOID SQ_LibraryErrorReceived( pstructSQ thisx, UINT8 invokeID )
{
	pstructInvokeHead			invokeEl = NULL;

	
	if (thisx == NULL)
	{	/* can't do anything */
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if ((invokeEl != NULL) &&
			(invokeEl->isInWaitState) )
	{
		invokeEl->isInWaitState = FALSE;
		invokeEl->isDone = TRUE;
		invokeEl->errorCode = ERR_WAE_WMLS_LIB;
		SQ_ClearBPCallList( invokeEl );
	}
}


BOOL SQ_GetViewId( pstructSQ thisx, UINT8 invokeID, UINT8 *pResult )
{
	pstructInvokeHead			invokeEl = NULL;

	
	if (thisx == NULL)
	{	/* can't do anything */
		return FALSE;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if ((invokeEl != NULL) &&
			(invokeEl->theUAStruct != NULL) )
	{
		*pResult = invokeEl->theUAStruct->iViewID;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}




VOID SQ_DeleteArgOpS( pstructOpS *pArgOpS )
{
	OpS_Delete( pArgOpS );
}



#ifdef USE_PROPRIETARY_WMLS_LIBS

VOID SQ_ProprietaryLibraryFunctionResponse( pstructSQ thisx, UINT8 invokeID, WMLSvar** pResultVar )
{
	pstructInvokeHead			invokeEl = NULL;

	if (thisx == NULL)
	{	/* ERROR: not all parameters are correct! */
		WMLSvar_Delete( pResultVar, FALSE );
		return;
	}

	/* find the invoke which has the supplied invoke ID */
	invokeEl = SQ_FindInvokeHead( thisx, invokeID );
	if (invokeEl == NULL)
	{	/* no invoke that matched the invoke ID */
		WMLSvar_Delete( pResultVar, FALSE );
		return;
	}

	/* now see if we have an SI that is waiting for an external result */
	
	if ( (invokeEl->BPcallList != NULL) && 
			(invokeEl->BPcallList->theSI != NULL) &&
			(invokeEl->BPcallList->theSI->inWaitState) )
	{
		enumErrorCode anErrCode;
		
		anErrCode = Lib_ProprietaryLibraryFunctionResponse( invokeEl->BPcallList->theSI, pResultVar );

		if (anErrCode != ERR_WAE_WMLS_NONE)
		{
			invokeEl->isDone = TRUE;
			invokeEl->errorCode = anErrCode;
		}

		invokeEl->isInWaitState = FALSE;
		return;
	}
	else
	{	/* something went wrong */
		WMLSvar_Delete( pResultVar, FALSE );
		return;
	}
}

/* USE_PROPRIETARY_WMLS_LIBS */
#endif
