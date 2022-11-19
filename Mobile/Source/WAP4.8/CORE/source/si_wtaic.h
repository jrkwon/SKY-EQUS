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

	File: si_wtaiC.h

	Description: Network common wtai script functions. Header file.

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991026  JPR		First version
	991101	JPR		Updated
	000216	JPR		Minor corrections
	001011	ASPN	Lower-case filenames
	001024	ASPN	Finish functions added
    010315  ASPN    WTAI_NC_NT_markAsRead added.
    010402  ASPN    si_int.h included.

=========================================================================*/

#ifndef SIWTAIC_H 
#define SIWTAIC_H

#include "errcodes.h"
#include "si_si.h"
#include "cmmnrsrc.h"
#include "si_libs.h"
#include "si_int.h"
#include "wta_ua.h"
#include "wtai_if.h"

#define WTAI_NC_FUNC_MAX	6

extern const structWTAILibCall arrayScriptwtaiNC[5][6];

/*  The functions in this file are called to perform WTAI script functions.
    Most of them operate in pairs. In this description the two kinds are called
    "activate" and "finish" functions. All "finish" function has the suffix
    "_finish".

    The "activate" functions take three parameters.
     The operand stack used by the Script Interpreter (SI).
     The WTAControlStruct which holds the current state of the WTA user agent.
     The object id which is used to convey the result back to the calling object.

    The "finish" functions take four parameters.
     The operand stack used by the SI.
     The WTAControlStruct which holds the state of the WTA user agent.
     The integer result and the field value, holding the return values from
      the script function call.

    The "activate" function is called to initiate a WTAI Script function call.
    This call often results in a adapter function call.

    If an adaptor function has been called the return values of this function call
    are returnd to the SI by a call to corresponding "finish" function.

    If no adaptor function has been called a script variable is created and given
    an apropriate value. This variable is pushed onto the operand stack and by this
    means returned to the Script interpreter (SI). Finally an appropriate
    enumErrorCode is returned. No "finish" function will be called.

    When the "finish" function is called a script variable is created and given
    the return value. This variable is pushed onto the operand stack and by this
    means returned to the Script interpreter (SI). Finally an appropriate
    enumErrorCode is returned.
*/

#define wtaicFunctionParameters pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID
#define wtaicFunctionParametersFinish pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue

/* Voice Call Control ****************************************************/

enumErrorCode WTAI_NC_VC_setup (wtaicFunctionParameters);
enumErrorCode WTAI_NC_VC_accept (wtaicFunctionParameters);
enumErrorCode WTAI_NC_VC_release (wtaicFunctionParameters);
enumErrorCode WTAI_NC_VC_sendDTMF (wtaicFunctionParameters);
enumErrorCode WTAI_NC_VC_callStatus (wtaicFunctionParameters);
enumErrorCode WTAI_NC_VC_list (wtaicFunctionParameters);


enumErrorCode WTAI_NC_VC_setup_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_VC_accept_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_VC_release_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_VC_sendDTMF_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_VC_callStatus_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_VC_list_finish (wtaicFunctionParametersFinish);

/* Network Text **********************************************************/

enumErrorCode WTAI_NC_NT_send (wtaicFunctionParameters);
enumErrorCode WTAI_NC_NT_list (wtaicFunctionParameters);
enumErrorCode WTAI_NC_NT_remove (wtaicFunctionParameters);
enumErrorCode WTAI_NC_NT_getFieldValue (wtaicFunctionParameters);
enumErrorCode WTAI_NC_NT_markAsRead (wtaicFunctionParameters);

enumErrorCode WTAI_NC_NT_send_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_NT_list_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_NT_remove_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_NT_getFieldValue_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_NT_markAsRead_finish (wtaicFunctionParametersFinish);

/* Phone Book ************************************************************/

enumErrorCode WTAI_NC_PB_write (wtaicFunctionParameters);
enumErrorCode WTAI_NC_PB_search (wtaicFunctionParameters);
enumErrorCode WTAI_NC_PB_remove (wtaicFunctionParameters);
enumErrorCode WTAI_NC_PB_getFieldValue (wtaicFunctionParameters);
enumErrorCode WTAI_NC_PB_change (wtaicFunctionParameters);

enumErrorCode WTAI_NC_PB_write_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_PB_search_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_PB_remove_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_PB_getFieldValue_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_PB_change_finish (wtaicFunctionParametersFinish);

/* Miscelleaneous *********************************************************/

enumErrorCode WTAI_NC_MISC_indication (wtaicFunctionParameters);
enumErrorCode WTAI_NC_MISC_endcontext (wtaicFunctionParameters);
enumErrorCode WTAI_NC_MISC_setProtection (wtaicFunctionParameters);
enumErrorCode WTAI_NC_MISC_getProtection (wtaicFunctionParameters);

enumErrorCode WTAI_NC_MISC_indication_finish (wtaicFunctionParametersFinish);

/* Call Logs **************************************************************/

enumErrorCode WTAI_NC_CL_dialled (wtaicFunctionParameters);
enumErrorCode WTAI_NC_CL_missed (wtaicFunctionParameters);
enumErrorCode WTAI_NC_CL_received (wtaicFunctionParameters);
enumErrorCode WTAI_NC_CL_getFieldValue (wtaicFunctionParameters);

enumErrorCode WTAI_NC_CL_dialled_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_CL_missed_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_CL_received_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_CL_getFieldValue_finish (wtaicFunctionParametersFinish);

/* ************************************************************************/

#endif
