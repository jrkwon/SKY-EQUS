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
/*
 * Cache.h
 *
 * Module for storing and retrieving WML pages in a cache.
 *
 * Created by Anders Edenbrandt.
 *
 * Revision history:
 *   991008, AED: Complete rewrite to use the Storage module.
 *   991123, AED: Added new function, Cache_DeleteMatchingPrefix.
 *   000114, AED: Added etag parameter to Cache_URLisPresent.
 *   000808, IPN: Added WmlBackNav parameter to Cache_URLisPresent.
 *   001120, AED: New parameter in Cache_URLisPresent and Cache_Write,
 *                to distinguish between content belonging to WTA and
 *                normal content.
 *                Removed Cache_ConfirmURL. No longer used.
 *   010410, IPN: Added the function Cache_GetParameters.
 *
 */

#ifndef _Cache_h
#define _Cache_h

#include "cmmnrsrc.h"

/*
 * Cache_Init
 *
 * This function should be called once the WAP Client's physical
 * cachememory is ready to be accessed. If the new size is less
 * than the previous size, then arbitrary data might be lost.
 * This function also sets the new size in the WAEMAINOBJECT.
 * If iFirstInit is equal to 1, the HASREAD flag in each cache
 * record is cleared.
 */
VOID
Cache_Init (UINT32 liNewSize,      /* The new size of the cache */
            VOID   *pvWAEMaStruct, /* Pointer to the WAEMAINOBJECT */
            UINT16  iFirstInit     /* Flag */
            );

/*
 * Cache_PrepareSizeChange
 *
 * This function prepares the cache to be shut down,
 * possibly changing its size. After a call to this function the cache
 * cannot be accessed without first calling Cache_Init.
 * At the time of call, the old size is still in effect.
 * If the new size is less than the previous size, then
 * cache records that do not fit are removed on a first-in first-out basis.
 */
VOID
Cache_PrepareSizeChange (UINT32 liNewSize);

/*
 * Cache_ URLisPresent
 *
 * Check if a URL is in the cache.
 * "pvUrl" is the URL as a null-terminated byte string.
 * "iWmlBackNav" the normal caching rules is not used when
 *   the request comes from the history.
 * "isWTA": if this is > 0, only content belonging to WTA is considered,
 *    otherwise only content NOT belonging to WTA is considered
 * "uiLastMod" points to a location where the last modified time
 * will be stored, in case the function returns 2, and similarly
 * "etag" points to a location where a BYTE-pointer to an entity tag
 * will be stored.
 *
 * Return values:
 *   0 => the URL is not in the cache, or has expired and cannot be
 *        revalidated.
 *   1 => the URL is in the cache and has not expired.
 *   2 => the URL is in the cache, has expired, but can be revalidated;
 *        in this case, the last modified time is returned in
 *        the 'uiLastMod' parameter and the entity tag in "etag".
 */
INT16
Cache_URLisPresent (void *pvUrl,  /* URL, as null-terminated byte string */
                    UINT8 iWmlBackNav, /* the request comes from the history */
                    UINT8 isWTA,       /* */
                    UINT32 *uiLastMod,      /* return last modified time */
                    BYTE   **etag           /* return entity tag */
                    );

/*
 * Cache_Read
 *
 * Given a URL, read the corresponding Header and Body from the cache.
 * "pvUrl" is the URL, as a null-terminated byte string,
 * "pvHeader" points to a location where a pointer to the header
 * contents will be stored, and "liHeaderLen" points to a location
 * where the length of the header will be stored.
 * Similarly, "pvBody" points to a location where a pointer to the body
 * contents will be stored, and "liBodLen" points to a location
 * where the length of the body will be stored.
 *
 * Returns 1 if successful, otherwise 0.
 */
INT16
Cache_Read (void* pvUrl,
            void **pvHeader, UINT32 *liHeaderLen,
            void **pvBody, UINT32 *liBodLen
            );

/*
 * Cache_Write
 *
 * Write a URL, Header and Body to the cache.
 * "pvUrl" is the URL, as a null-terminated byte string,
 * "isWTA", if > 0, this content is flagged as belonging to WTA
 * "pvHeader" points to the header content and "iHeaderLen" is the length
 * of the header. Similarly, "pvBody" points to the body content and
 * "iBodLen" is the length of the body.
 *
 * Returns 1 if successful, otherwise 0.
 */
INT16
Cache_Write (void *pvUrl,
             UINT8 isWTA,
             void *pvHeader, UINT32 iHeaderLen,
             void *pvBody, UINT32 iBodLen
             );

/*
 * Cache_Delete
 *
 * Delete a record from the cache, given its URL.
 * "pvUrl" is the URL as a null-terminated byte string.
 *
 * Returns 1 if succesful, otherwise 0.
 */
INT16
Cache_Delete (void *pvUrl);


INT16
Cache_Delete_Push (void *pvUrl);

/*
 * Cache_DeleteMatchingPrefix
 *
 * Delete all records in the cache whose URL has the given
 * URL as a prefix.
 *
 * Returns the number of records that were deleted.
 */
INT16
Cache_DeleteMatchingPrefix (void *pvUrl);

/*
 * Cache_GetParameters
 *
 * Compute Expire and LastModified from a Header.
 */
void
Cache_GetParameters (BYTE* pbHeader, UINT32 uiHeadLen,
                     UINT32 *uiExp,
                     UINT32 *uiLastMod,
                     UINT8  *uiMustRevalidate,
                     UINT32 *uiDate,
                     BYTE   **pbEtag);

#endif
