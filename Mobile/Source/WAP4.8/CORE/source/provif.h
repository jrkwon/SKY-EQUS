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

	WAP Implementation Project

==========================================================================

	File: provif.h

	Description: External functions needed for handling the InterFace to the WBXML-decoding of the Provisioning
				 Content types.

	Author: Kent Olsson, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	010315    KEOL   First version

==========================================================================*/
#ifndef _provif_h
#define _provif_h

#include "WBXMLDef.h"
#include "wbxmldec.h"
#include "prov_dec.h"


/*========================================================================

	External Provisioning InterFace Functions

==========================================================================*/

/*========================================================================
	ParseProvData
==========================================================================*/
void* parseProvData (BYTE *pbData, UINT32 pbDataLen, int id, UINT16 *iError);

/*========================================================================
	BuildDataArray
==========================================================================*/
void buildDataArray( void* vsParsedData, int iObjectID, int iTID, UINT32 iExpireDate, UINT16 iError);

/*========================================================================
	calculateMAC
==========================================================================*/
BOOL calculateMAC(PROV_MACELEMENT* pMacStr, void* vsSharedSecret, UINT16 iLenSharedSecret);

#endif
