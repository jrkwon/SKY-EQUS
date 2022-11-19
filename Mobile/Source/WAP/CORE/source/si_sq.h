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

/***********************
File: si_sq.h + c
Author: Klas Hermodsson (KHN)

Description:
SQ stands for Script Queue. This module encapsulates the script interpreter (SI) 
(see the si_si files). It can handle several independent script invokations. The SQ functions as a 
dispatcher and makes sure the invokations are queues and executed properly. 
The SQ can execute an invokation until it is done before letting another invokation execute. 
It can also let the invokations take turn and execute them pseudo-simulataneously. 
This mode is called round robin. Which mode the SQ will execute scripts in is determined 
when calling the SQ_Init function.

The SQ manages a queue of invoke heads (see structInvokeHead below). There will be one
invoke head per script invokation. When an invokation is finished executing the SQ will
remove the invoke head and its subcomponents.

Each invoke head holds a stack of BP elements (structBPElement). 
There will be a BP element in the stack per script file involved in the invokation. 
The BP element holds a WMLScript file in its encoded form (sometimes referred to 
as bytecode package or BP) and its corresponding script interpreter (SI).

The SQ is the component that SDL interfaces with to run scripts. The actual low level
interface between SDL and C is found in the si_sdl files.

The central functions to the SQ are SQ_InvokeScript and SQ_Exec.
SQ_InvokeScript takes care of an incoming invokation.
SQ_Exec is the main script handling thread function and that the SDL that 
encapsulates WMLS calls in the following mannor (simplified):

while (WAP client alive)
{
	SQ_Exec()
	<handle incoming signals>
}


Example of SQ operation: 
When a script invokation reaches the SQ an invoke head is created and put in the queue. 
The invoke head has itself createf a BP element which now points to the script file.
The next time SQ_Exec is called a SI is created for the script file. If everything went ok
the SI is executed for one time slice (see si_si files). If the script file calls an external
function in another script file the invoke head is put in a wait state and the SQ_Exec returns
with a return value that says that an external file is needed. The SDL must then call 
SQ_GetExternalCallParams to get the URL to the new script file. If SQ_Exec is called and 
the new script file has not been loaded yet (and no other invokations exist) the function
will return directly signaling that has nothing to process for the moment. When the 
WAP client has successfully loaded the new script file it calls SQ_LoadDoneReceived.
The next time SQ_Exec is called a SI is created for the new file and is executed for a 
time slice. And so on.
When a script file is done executing the result of the script (there is always a return value)
is given back to the caller (either another script file or the component that invoked the script).

************************/

#ifndef _SI_SQ_H
#define _SI_SQ_H


#include "cmmnrsrc.h"
#include "si_si.h"
#include "wmldef.h"
#include "confvars.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "aapiclnt.h"
    #include "tapiclnt.h"
#endif


typedef struct BPElement {
	BYTE									*BP;								/* A pointer to the encoded WMLS file */
	UINT32								BPlen;							/* the length in number of bytes */
	INT16									IANAcharset;				/* header field from WSP for the BP file. Is used to determine string encoding of WMLS strings in the BP file */
	WCHAR									*callURL;						/* The absolute URL to this WMLS file */
	BOOL									isInitialBPInvoked;	/*	TRUE: the callURL includes funcName and args. 
																								FALSE the funcName and args can be fetched from next SI  */
	pstructSI							theSI;							/* The interpreter that handles this WMLS file */
	struct BPElement			*next;							/* the next in this stack of BP elements */

} structBPElement, *pstructBPElement;



typedef struct InvokeHead {
	UINT8									invokeID;				/* the ID for an invokation so that when SQ_Exec returns one can find which invokation was executed */
	UINT32								UApidAsInt;			/* the SDL process ID of the user agent that invoked the script. Used to send signals about script execution back to the caller. */
	pUA										theUAStruct;		/* A pointer to the user agent. must NOT be deallocated when InvokeHead is deleted. It is used for some scriptlib functions */
	BOOL									isDone;					/* TRUE: this invoke thread is done and is waiting for someone to collect the result 
																							FALSE: not done */
	BOOL									isInWaitState;	/* TRUE if waiting for external resources. FALSE: if ready to run */
	enumErrorCode					errorCode;			/* if error occurs the whole thread is deallocated and the errCode is stored here */
	pstructVar						theResult;			/* when the script is done, the call list is deallocated and the result i stored in this attribute. */
	BOOL									browserPrev;		/* TRUE if a WMLBrowser.prev is to be performed when the script call is done */
	WCHAR									*browserGoUrl;	/* if a WMLBrowser.go is to be performed when the script call is done the URL is stored here */
	pstructBPElement			BPcallList;			/* actually a call stack of WMLS files. I.e the first script file of the call will be in the "bottom" of the stack */
	struct InvokeHead			*next;					/* next in this list of invoke heads */

} structInvokeHead, *pstructInvokeHead;


typedef struct SQ {
	UINT16								timeSlice;					/* the config param to be used for all created SIs. (see si_si files) */
	UINT8									nextInvokeID;				/* this is so that one knows which invokeID to give to the next Invoke */
	BOOL									doRoundRobinScheduling;
	BOOL									onlyOneScriptPerUA;
	BOOL									handlesTopPriorityREQ;	
		/* if the SQ handles top priority requests: a top priority request gets the benifit of getting its 
		invokation first in line and will start executing next time SQ_Exec is called. Another top priority req 
		will do the same thing so top priority req can interrupt each other*/
	pstructInvokeHead			currentInvoke;	/* steps through the InvokeHead elements if round robin scheduling is used otherwise it will be the first in the invokeList. */
	pstructInvokeHead			invokeList;		/* the queue of script invokes */

} structSQ, *pstructSQ;



/* enumScriptStatus is the return type from a SQ_Exec call.
It tells about the script thread it executed for its time slice. */
typedef enum {
	eSs_more = 1,		/* There is more to process in the invoke queue */
	eSs_done,				/* a script thread is done. Th ereturn value should now be collected. */
	eSs_done_prev,	/* done (see above) and the UA should perform a PREV action */
	eSS_done_go,		/* done and the UA should perform a GO action with a url found in si->goURL */
	eSs_error,			/* the script thread resulted in an error */
	eSs_extFunc,		/* the script thread calls an external script file that has to be fetched */
	eSs_lib					/* the script thread calls a type of scriptlib function that has to be handled in SDL */
} enumScriptStatus;






/*==========================================
	SQ_Init
============================================

---Purpose: 
To create a new SQ.

---Params:
timeSlice					the amount of time units an SI will execute the most at a timne.
roundRobin				TRUE: the script invokes will share execution time evenly in round robin fashion.
										FALSE: The invoke first in the queue will execute til it's done. Then a new 
										invoke will start to execute.
oneScriptPerUA		TRUE: Only one script invoke per user agent is allowed.
										FALSE: multiple script invokes per user agent is allowed.
handlesTopPrio		TRUE: a script with top priority (see SQ_InvokeScript) will be put first in the 
										invoke queue and will start executing next SQ_exec is called.
	
---Return:
The newly created SQ.

------------------------------------------------------------------------*/
pstructSQ SQ_Init( UINT16 timeSlice, BOOL roundRobin, BOOL oneScriptPerUA, BOOL handlesTopPrio );


/*==========================================
	SQ_Terminate
============================================

---Purpose: 
To deallocate an SQ and all of its components.

---Params:
pThis			The SQ to be terminated. The pointer will be set to NULL when the termination is done.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_Terminate( pstructSQ *pThis );


/*==========================================
	SQ_InvokeScript
============================================

---Purpose: 
To put a new invoke in the script queue.

---Params:
thisx						The SQ that manages the scripts.
UApidIndex			The index into the array (kept in SDL) that holds the process ID (pID) 
									to the SDL user agent that invokes this script.
theUA						User agent struct that holds some information needed when performing 
									some of the script lib functions.
pBP							The compiled script file to be executed.
									NOTE: The SQ will take care of deallocation of this parameter.
BPlen						The length of the BP in bytes.
IANAcharset			The IANA charset code that WSP sent as a header for this script.
									Tells how some string constants are encoded.
pCallURL				The absolute URL that was used to load this script file. 
									Includes fragment with function name and parameters.
									NOTE: The SQ will take care of deallocation of this parameter.
isTopPriority		If TRUE this script wants to be executed ASAP. See SQ_Init for 
									how these are handled.
	
---Return:
>0:	The invoke ID that will be used to identify the script thread caused by this invokation. 
			E.g. SQ_Exec will return such an id so it's possible to tell what script thread was executed.
0:	Something went wrong and the invokation was not queued. E.g. a user agent tries to have 
			two scripts running when the config param oneScriptPerUA has been set to TRUE.

------------------------------------------------------------------------*/
UINT8 SQ_InvokeScript( pstructSQ thisx, UINT32 UApidInt, pUA theUA, BYTE **pBP, UINT32 BPlen, INT16 IANAcharset, WCHAR **pCallURL, 
											 BOOL isTopPriority );


/*==========================================
	SQ_AbortScriptsForSpecificUA
============================================

---Purpose: 
Stops and deallocates all script threads invoked by a certain user agent
(There is no way to abort just a single script thread via its invokeID at this time).

---Params:
thisx				The SQ.
UApidIndex	The index into the SDL kept array of user agent process IDs. The
							index is also stored in each invoke head so its easy to decide
							which invoke heads to abort.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_AbortScriptsForSpecificUA( pstructSQ thisx, UINT32 UApidInt );


/*==========================================
	SQ_Exec
============================================

---Purpose: 
The central function of the SQ. This function is to be called regularly.
e.g:
while (running application)
{
	SQ_exec
	(take care of incoming signals etc.)
}

The function executes a script thread (one invoke head) for one time slice (max) 
and then returns. Which script thread depends on the mode of the SQ (see SQ_Init above).

---Params:
pInvokeIDThatExecuted		Returns the invoke ID of the script thread that was executed.
pUApidIndex							Returns the index into the SDL kept array of process
													IDs to the user agent which invoked the script thread
													that was executed.
	
---Return:
eSs_more				If this is returned and InvokeIDThatExecuted != 0 then there is more to process.
									If InvokeIDThatExecuted == 0 then SQ_exec is in a wait state so it is not
									necessary to call it again until the SQ receives any new input.
eSs_done				This is returned if a script thread is done and has a return value to be 
									retrieved by calling SQ_GetResult (see below).
eSs_done_prev		The same as eSs_done but the user agent should also perform a PREV action.
									Fetching of the PREV parameters must be done. This is done by calling
									SQ_GetPrevParams (this is done in addition to fetching the result).
eSS_done_go			The same as eSs_done but the user agent should also perform a GO action.
									Fetching of the GO parameters must be done. This is done by calling
									SQ_GetGoParams (this is done in addition to fetching the result).
eSs_error				An error ocurred in the script thread. The error code must be fetched
									and is done by calling SQ_GetError.
eSs_extFunc			The script thread is calling a function in another script file.
									Call SQ_GetExternalCallParams to get the URL of the file to be loaded.
									(see SQ_LoadDoneReceived and SQ_LoadErrorReceived for more info)
eSs_lib					The script thread is calling a script lib function that needs
									to communicate with the outside. Call SQ_GetLibCallParams to get 
									the data on which function needs help to finish.

------------------------------------------------------------------------*/
enumScriptStatus SQ_Exec( pstructSQ thisx, UINT8 *pInvokeIDThatExecuted, UINT32 *pUApidInt );

/*==========================================
	SQ_GetResult
============================================

---Purpose: 
To get the result from a script thread that is done executing.

---Params:
thisx				The SQ
invokeID		The id of the script thread (this was returned by SQ_Exec).
pResultStr	Returns the result of the script thread converted to a string.
							NOTE: The receiver is responsible for deallocating this parameter.
	
---Return:
TRUE:		Everything went ok. The script thread was done and had a return
					value that hadn't yet been collected.
FALSE:	Something went wrong. E.g. incorrect invokeID, the script thread
					is not done or the result has already been collected.

------------------------------------------------------------------------*/
BOOL SQ_GetResult( pstructSQ thisx, UINT8 invokeID,WCHAR **pResultStr );

/*==========================================
	SQ_GetGoParams
============================================

---Purpose: 
To get the URL to be used in the GO action commanded by the
script thread.

---Params:
thisx			The SQ.
invokeID	The id of the script thread (this was returned by SQ_Exec).
pGoUrl		Returns the URL.
						NOTE: The receiver is responsible for deallocating this parameter.
	
---Return:
TRUE:		Everything went ok.
FALSE:	Something went wrong. E.g. incorrect invokeID or the script thread
					is not done.

------------------------------------------------------------------------*/
BOOL SQ_GetGoParams( pstructSQ thisx, UINT8 invokeID, BYTE **pGoUrl );

/*==========================================
	SQ_GetPrevParams
============================================

---Purpose: 
Actually, this function is obsolete since no parameters
are provided by the WMLBrowser.go script lib function.
This function is still here because the function has not been
removed in order to avoid too much code change.
It is still called.

---Params:
thisx			The SQ.
invokeID	The id of the script thread (this was returned by SQ_Exec).
	
---Return:
TRUE:		Everything went ok.
FALSE:	Something went wrong. E.g. incorrect invokeID or the script thread
					is not done.

------------------------------------------------------------------------*/
BOOL SQ_GetPrevParams( pstructSQ thisx, UINT8 invokeID );

/*==========================================
	SQ_GetError
============================================

---Purpose:
To get the error code of the script thread where
an error was detected. The error is one of the fatal errors 
described in the WMLScript language specification 1.1 
section 13.3.

---Params:
thisx			The SQ.
invokeID	The id of the script thread (this was returned by SQ_Exec).
errCode		Returns a WMLS error code (see errcodes.h).
	
---Return:
TRUE:		Everything went ok.
FALSE:	Something went wrong. E.g. incorrect invokeID or the script thread
					is not done.

------------------------------------------------------------------------*/
BOOL SQ_GetError( pstructSQ thisx, UINT8 invokeID, enumErrorCode *errCode );

/*==========================================
	SQ_GetExternalCallParams
============================================

---Purpose: 
To get the URL to the new script file to be loaded.

---Params:
thisx			The SQ.
invokeID	The id of the script thread (this was returned by SQ_Exec).
pTheURL		Returns the absolute URL to the script file tobe loaded.
						NOTE: The receiver is responsible for deallocating this parameter.
	
---Return:
TRUE:		Everything went ok.
FALSE:	Something went wrong. E.g. incorrect invokeID.

------------------------------------------------------------------------*/
BOOL SQ_GetExternalCallParams( pstructSQ thisx, UINT8 invokeID, BYTE **pTheURL );

/*==========================================
	SQ_GetLibCallParams
============================================

---Purpose: 
To get the parameters needed to perform the part of a script lib function that
resides externally to the SQ environment (e.g. in SDL).

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
pLibIndex		Returns the script library index of the function that 
							needs external help.
pFuncIndex	Returns the script library function index for the function that 
							needs external help.
pArgOpS			Returns an operand stack which holds the arguments for 
							the lib function.
							NOTE: The receiver is responsible for deallocating this parameter.

---Return:
TRUE:		Everything went ok.
FALSE:	Something went wrong. E.g. incorrect invokeID.


------------------------------------------------------------------------*/
BOOL SQ_GetLibCallParams( pstructSQ thisx, UINT8 invokeID, UINT16 *pLibIndex, UINT8 *pFuncIndex, 
															pstructOpS *pArgOpS );

/*==========================================
	SQ_ReturnValueToScript
============================================

See SQ_ReturnStringValueToScript below.

------------------------------------------------------------------------*/
VOID SQ_ReturnValueToScript( pstructSQ thisx, UINT8 invokeID, WCHAR **theValueStr );


/*==========================================
	SQ_ReturnVarValueToScript
============================================



------------------------------------------------------------------------*/
VOID SQ_ReturnVarValueToScript( pstructSQ thisx, UINT8 invokeID, pstructVar *ppResultVar );


/*==========================================
	SQ_ReturnStringValueToScript
============================================

---Purpose: 
To return a string to script thread.
E.g. the Dialogs.alert script lib function has requested completion of the
function. When SDL receives the result of the function it calls this
function to return the result over to the SQ and the script thread
that made the request.

NOTE: If this function is called and the script thread never asked for 
help the parameters are properly taken care of anyway.

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
theValue		The string to be returned to the sript thread.
							NOTE: The SQ will take care of deallocation of this parameter.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_ReturnStringValueToScript( pstructSQ thisx, UINT8 invokeID, WCHAR **theValue );

/*==========================================
	SQ_ReturnBoolValueToScript
============================================

---Purpose: 
To return a bool value to script thread.
E.g. the Dialogs.confirm script lib function has requested completion of the
function. When SDL receives the result of the function it calls this
function to return the result over to the SQ and the script thread
that made the request.

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
theValue		The bool to be returned to the sript thread.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_ReturnBoolValueToScript( pstructSQ thisx, UINT8 invokeID, BOOL theValue );

/*==========================================
	SQ_ReturnIntValueToScript
============================================

---Purpose: 
To return an integer value to script thread.
E.g. the URL.loadString script lib function has requested completion of the
function. The load failed and so the result is an integer error code which is
returned to the script thread by calling this function.

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
theValue		The integer to be returned to the sript thread.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_ReturnIntValueToScript( pstructSQ thisx, UINT8 invokeID, INT32 theValue );

/*==========================================
	SQ_LoadStringDone
============================================

---Purpose: 
To take care of the successful result of a URL.loadString call and
return it to the proper script thread.

NOTE: If the load failed, SQ_ReturnIntValueToScript must then be called to
transfer the integer error code to the script thread.

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
charSet			The IANA mibEnum for the charset that the string is in.
pValueStr		The string to be transfered over to the script thread.
							NOTE: The SQ will take care of deallocation of this parameter.
strLen			The length of ValueStr in bytes.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_LoadStringDone( pstructSQ thisx, UINT8 invokeID, INT16 charSet, BYTE **pValueStr, UINT32 strLen );

/*==========================================
	SQ_LibraryErrorReceived
============================================

---Purpose: 
This function is to be called if the external part of
a lib function call failed.

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_LibraryErrorReceived( pstructSQ thisx, UINT8 invokeID );

/*==========================================
	SQ_GetViewId
============================================

---Purpose: 
To get the view id of the user agent that invoked the script thread has.
This is used when logging certain events,

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
pResult			Returns the view id.
	
---Return:
TRUE:		Everything went ok.
FALSE:	Something went wrong. E.g. incorrect invokeID.

------------------------------------------------------------------------*/
BOOL SQ_GetViewId( pstructSQ thisx, UINT8 invokeID, UINT8 *pResult );

/*==========================================
	SQ_LoadErrorReceived
============================================

---Purpose: 
This function is to be called when the loading of 
another script file fails. I.e. a script thread 
calls a function in another script file. This new file has to be loaded.
The loading fails. Then this function is called.

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_LoadErrorReceived( pstructSQ thisx, UINT8 invokeID );

/*==========================================
	SQ_LoadDoneReceived
============================================

---Purpose: 
The loading of a new script file is successful.

---Params:
thisx				The SQ.
invokeID		The id of the script thread (this was returned by SQ_Exec).
pBP					The contents of the new encoded script file.
							NOTE: The SQ will take care of deallocation of this parameter.
BPlen				The length of BP in bytes.
IANAcharset	The IANA mibEnum for the charset that some string constants may be
							encoded in.

---Return:
-

------------------------------------------------------------------------*/
VOID SQ_LoadDoneReceived( pstructSQ thisx, UINT8 invokeID, BYTE **pBP, UINT32 BPlen, INT16 IANAcharset );

/*==========================================
	SQ_GetAnArgumentAsString
============================================

---Purpose: 
This functions pops an operand from the operand stack
and converts it into a string in the same way 
the script lib function String.toString does.
It is used by SDL to retrieve arguments from the
operand stack when part of a script lib function resides
outside the SQ environment (see also SQ_GetLibCallParams).

NOTE: The popped operand is removed after it has been 
converted into a string.

---Params:
argOpS			The operand stack (see si_ops files) that contains 
							an argument that needs to be converted into a string.
pArgAsStr		Returns the argument in a string format.
							NOTE: The receiver is responsible for deallocating this parameter.
							
---Return:
TRUE:		The conversion went fine.
FALSE:	Conversion failed or the stack is empty.

------------------------------------------------------------------------*/
BOOL SQ_GetAnArgumentAsString( pstructOpS argOpS, WCHAR **pArgAsStr );

/*==========================================
	SQ_DeleteArgOpS
============================================

---Purpose: 
This function deallocates the operand stack and all
of its components.

---Params:
pArgOpS		The stack to be deallocated.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SQ_DeleteArgOpS( pstructOpS *pArgOpS );





#ifdef CAN_SIGN_TEXT
  VOID SQ_TextSigned( pstructSQ thisx, UINT8 invokeId, UINT8 algorithm, UINT16 sigLen, CHAR *signature, CHAR *hashedKey, UINT16 hashedKeyLen, CHAR *certificate, UINT16 certificateLen, UINT8 certificateType, UINT16 err);
#endif

  
#ifdef USE_PROPRIETARY_WMLS_LIBS
VOID SQ_ProprietaryLibraryFunctionResponse( pstructSQ thisx, UINT8 invokeID, WMLSvar** pResultVar );
#endif


/* _SI_SQ_H */
#endif
