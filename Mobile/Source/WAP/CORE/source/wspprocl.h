/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * All rights reserved.
 */

/*
 * WSPproCL.h
 *
 * Procedures for WSP Connectionless
 *
 * Created by Anders Edenbrandt, March 1999.
 *
 * Revision history:
 *     990423, AED:  Changed to pass data as void* between WAE and WSP,
 *                    instead of using MBM buffers.
 *     990909, AED:  Changed to use PDU buffers instead of MBM buffers.
 *	   990928, FKN:  Added ExtractPushPDU_CL	
 *
 */
#ifndef _WSPproc_h
#define _WSPproc_h

#include "wiptrgt.h"

#if defined XSCT_CMICRO
    #include "ml_typw.h"
#else
    #include "scttypes.h"
#endif

#include "ctypes.h"
#include "pdubuf.h"
#include "glbldecl.h"
#include "wae_wsp.h"

/*
 * Check address fields.
 * Returns FALSE if the addresses have incorrect syntax.
 */
extern SDL_Boolean
ValidAddressFieldsFromWAE (SUnitMethodInvokeReqType *SUnitMethodInvokeReqData);


/*
 * Create a PDU for a GET request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL if memory allocation fails.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateGetPDU_CL (SUnitMethodInvokeReqType *SUnitMethodInvokeReqData);


/*
 * Create a PDU for a POST request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL if memory allocation fails.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreatePostPDU_CL (SUnitMethodInvokeReqType *SUnitMethodInvokeReqData);


/*
 * Extract the fields from a reply PDU.
 * Copies extracted data to newly allocated buffers in the parameter
 * "SUnitMethodResultIndData", and deallocates the given PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer is not
 * released.
 * NOTE: it is the caller's responsibility to deallocate the buffers
 * in the result structure.
 */
extern SDL_Boolean
ExtractReplyPDU_CL (pdubuf *PDU,
                    SUnitMethodResultIndType *SUnitMethodResultIndData);



/*
 * Extract the fields from a Push PDU.
 * Sets two VOIDSTAR out-parameters and two integer
 * out-parameters (containing headers and data from the PDU as well as
 * their lengths), and then releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffers
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractPushPDU_CL (pdubuf *PDU,
                    SUnitPushIndType *SUnitPushIndData);

#endif


