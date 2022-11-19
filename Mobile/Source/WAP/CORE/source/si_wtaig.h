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

	File: si_wtaiG.h

	Description: GSM-specific wtai script functions.

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991028  JPR		First version
	991101	JPR		Updated
	000308	JPR		Correction in define
	001011	ASPN	Lower-case filenames

=========================================================================*/

#ifndef SIWTAIG_H
#define SIWTAIG_H


#include "si_si.h"
#include "cmmnrsrc.h"
#include "si_libs.h"
#include "wta_ua.h"
#include "wtai_if.h"

#define WTAI_GSM_FUNC_MAX	8

extern const structWTAILibCall arrayScriptwtaiGSM[1][8];

/* All functions takes five parameters. The first three is used (only) if
   the function is called from a script. If this is the case, the pbURI 
   parameter MUST be set to NULL. The pstruct is the operand stack used by
   the script. The iLibIndex and iFuncIndex are the identifiers of the 
   script function.

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

#define wtaigFunctionParameters pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID
#define wtaigFunctionParametersFinish pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue

/* Network Functions *****************************************************/
enumErrorCode WTAI_GSM_NF_hold (wtaigFunctionParameters);
enumErrorCode WTAI_GSM_NF_retrieve (wtaigFunctionParameters);
enumErrorCode WTAI_GSM_NF_transfer (wtaigFunctionParameters);
enumErrorCode WTAI_GSM_NF_deflect (wtaigFunctionParameters);
enumErrorCode WTAI_GSM_NF_multiparty (wtaigFunctionParameters);
enumErrorCode WTAI_GSM_NF_separate (wtaigFunctionParameters);
enumErrorCode WTAI_GSM_NF_sendUSSD (wtaigFunctionParameters);
enumErrorCode WTAI_GSM_NF_netinfo (wtaigFunctionParameters);

enumErrorCode WTAI_GSM_NF_hold_finish (wtaigFunctionParametersFinish);
enumErrorCode WTAI_GSM_NF_retrieve_finish (wtaigFunctionParametersFinish);
enumErrorCode WTAI_GSM_NF_transfer_finish (wtaigFunctionParametersFinish);
enumErrorCode WTAI_GSM_NF_deflect_finish (wtaigFunctionParametersFinish);
enumErrorCode WTAI_GSM_NF_multiparty_finish (wtaigFunctionParametersFinish);
enumErrorCode WTAI_GSM_NF_separate_finish (wtaigFunctionParametersFinish);
enumErrorCode WTAI_GSM_NF_sendUSSD_finish (wtaigFunctionParametersFinish);
enumErrorCode WTAI_GSM_NF_netinfo_finish (wtaigFunctionParametersFinish);

/* ************************************************************************/

#endif
