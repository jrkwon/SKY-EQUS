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
#ifndef _SI_WTAI_H
#define _SI_WTAI_H

#include "cmmnrsrc.h"
#include "si_si.h"

#define WTAI_LIB_INDEX_START   512

/*
#ifndef WTA_FULL_ENABLED
*/
	/* only public WTA */
	#define WTAI_LIB_INDEX_END   512
/*
#else
*/
	/* Full WTA */
/*
	#define WTAI_LIB_INDEX_END   516
#endif
*/

enumErrorCode WTAI_CallLibraryFunction( pstructSI si, UINT16 libIndex, UINT8 funcIndex );

/* Public WTA *************************************************************/
enumErrorCode Call_WTAPublic_makeCall( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_WTAPublic_sendDTMF( pstructSI si, UINT16 libIndex, UINT8 funcIndex );

#endif
/* _SI_WTAI_H */
