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
	FILE: WSPCapa.h
  
	Functions for WSP Capabilities Encoding/Decoding for WAP.
	Header file.

	Rev history:
		990831	JPR		Created
		990909	JPR		Updated
=========================================================================*/
   
#ifndef WSPCAPA_H
#define WSPCAPA_H

#include "CmmnRsrc.h"
#include "HdrUtil.h"


/*========================================================================

	Definitions.

=========================================================================*/

/* List element */
typedef struct tCAPABILITY
{
	BYTE bCapability;
	BYTE* pbData;
	BYTE* pbEnd;
	struct tCAPABILITY *pNext;
} CAPABILITY, *pCAPABILITY;


/* List head */
typedef struct tCAPABILITIESDEF
{
	BYTE* pbData;
	INT32 iDataLength;
	pCAPABILITY pList; 	
} CAPABILITIESDEF, *pCAPABILITIESDEF;

/* Capabilities */
#define CLIENT_SDU_SIZE 0
#define SERVER_SDU_SIZE 1
#define PROTOCOL_OPTIONS 2
#define METHOD_MOR 3
#define PUSH_MOR 4
#define EXTENDED_METHODS 5
#define HEADER_CODE_PAGES 6
#define ALIASES 7


/*========================================================================

	Functions for handling capabilities.

=========================================================================*/


/*========================================================================
	WSP_PreParseCapabilities
==========================================================================
	The function parses the instream and builds a structure of the 
	capabilities in the data. The error flag is set if error. The input
	data is stored in the pCAPABILITIESDEF and MUST NOT be removed after
	the fuction has returned, unless it returns NULL.

	Input: Data, length of data, and pointer to BOOL
	Output: pCAPABILITIESDEF, or NULL if out of memory.
==========================================================================*/
pCAPABILITIESDEF WSP_PreParseCapabilities (BYTE* pbData, INT32 iLength,
										   BOOL* pfError);


/*========================================================================
	WSP_GetCapabilityWK
==========================================================================
	The function returns the value of the specified capability.
	
	(Method-MOR, Push-MOR, Client-SDU-Size, Server-SDU-Size supported)

	Input: Byte value of the requested capability, and the Capability 
		   where the search should be started from.
	Output: UINT32, the error flag is set if error.
==========================================================================*/
UINT32 WSP_GetCapabilityWK (BYTE bCapability, pCAPABILITY pCapa, BOOL* pfError);


/*========================================================================
	WSP_DeleteCapabilitiesDef
==========================================================================
	The function removes the structure created by the function
	WSP_PreParseCapabilities.

	Input: pCAPABILITIESDEF
	Output: -
==========================================================================*/
void WSP_DeleteCapabilitiesDef (pCAPABILITIESDEF* ppCapabilities);


#endif
