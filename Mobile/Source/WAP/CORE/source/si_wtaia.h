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

	File: si_wtaia.h

	
=========================================================================*/

#ifndef SIWTAIA_H 
#define SIWTAIA_H

#include "errcodes.h"
#include "si_si.h"
#include "cmmnrsrc.h"
#include "si_libs.h"
#include "wtai_if.h"

#define WTAI_PUB_FUNC_MAX	3

extern const structWTAILibCall arrayScriptwtaiPUB[1][3];

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


/* Voice Call Control ****************************************************/

enumErrorCode WTAI_PUB_publicMakeCall (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID);
enumErrorCode WTAI_PUB_publicSendDTMF (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID);
enumErrorCode WTAI_PUB_publicAddPBEntry (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID);

enumErrorCode WTAI_PUB_publicMakeCallFinish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue);
enumErrorCode WTAI_PUB_publicSendDTMFFinish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue);
enumErrorCode WTAI_PUB_publicAddPBEntryFinish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue);

/* ************************************************************************/

#endif
