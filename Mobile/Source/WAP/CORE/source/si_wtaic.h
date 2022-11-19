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
	0010	ASPN	Major makover

=========================================================================*/

#ifndef SIWTAIC_H 
#define SIWTAIC_H

#include "errcodes.h"
#include "si_si.h"
#include "cmmnrsrc.h"
#include "si_libs.h"
#include "wta_ua.h"
#include "wtai_if.h"

#define WTAI_NC_FUNC_MAX	6

extern const structWTAILibCall arrayScriptwtaiNC[5][6];

/* All functions takes three parameters. If the function is called from 
   a script, the pbURI parameter MUST be set to NULL. The pstruct is the 
   operand stack used by the script.

   If the WTAI-function is called from an URI, the si parameter MUST be
   NULL. The URI (not NULL) contains the input parameter for the WTAI 
   function. NOTE! Not all functions are possible to call via an URI.
   Consult the WTAI specifications for more information.

   After the correct adapter call is made, the result is transformed into
   its correct form. If the function was called by a script, a script 
   variable is created with the correct result. NOTE! A result MUST
   always be created and returned to the Script Interpreter.

   If the WTAI function was called by an URI, the result string(s) are
   stored in a variable list created by the function CreateVariableList
   in the file WTA_UA.h.
  
   If the function was called by a script, the result variable is pushed
   onto the operand stack. An appropriate enumErrorCode is returned.

   If the function was called by an URI, the result variable list created
   is stored in the pWTAControlStruct (the ppWTAIResult variable).
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

enumErrorCode WTAI_NC_NT_send_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_NT_list_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_NT_remove_finish (wtaicFunctionParametersFinish);
enumErrorCode WTAI_NC_NT_getFieldValue_finish (wtaicFunctionParametersFinish);

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
