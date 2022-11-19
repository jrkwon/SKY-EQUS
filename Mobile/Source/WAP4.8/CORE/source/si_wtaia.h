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


/* Voice Call Control ****************************************************/

enumErrorCode WTAI_PUB_publicMakeCall (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID);
enumErrorCode WTAI_PUB_publicSendDTMF (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID);
enumErrorCode WTAI_PUB_publicAddPBEntry (pstructOpS OpS, pWTACONTROL pWTAControlStruct, UINT8 iObjectID);

enumErrorCode WTAI_PUB_publicMakeCallFinish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue);
enumErrorCode WTAI_PUB_publicSendDTMFFinish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue);
enumErrorCode WTAI_PUB_publicAddPBEntryFinish (pstructOpS OpS, pWTACONTROL pWTAControlStruct, int iResult, WCHAR* pwcFieldValue);

/* ************************************************************************/

#endif
