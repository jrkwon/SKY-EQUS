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
 * Cache.c
 *
 * Module for storing and retrieving WML pages in a cache.
 *
 * Created by Anders Edenbrandt.
 *
 * Revision history: 
 *   990926, AED: Complete rewrite to utilize the new Storage module.
 *   991110, AED: All public routines now test if the cache has
 *                been initialized. The behaviour of
 *                Cache_PrepareSizeChange has changed to effect a
 *                shutdown of the cache, freeing all associated
 *                data structures.
 *   991123, AED: added new function, Cache_DeleteMatchingPrefix.
 *   000321, IPN: Check at CacheControl-MaxAge when calculating Expire values
 *                and a minor correction in Expire-calculation.
 *   000321, IPN: A minor changes in Cache_Write when the 
 *                Must-Revalidate flag is set.
 *   000410, IPN: Handles MaxAge and MustRevalidate simultaneous in a response. 
 *
 */
#include "confvars.h"
#include "Cache.h"
#include "Storage.h"

#include "aapimem.h"

#include "CmmnRsrc.h"
#include "WAEDef.h"
#include "URL.h"
#include "HdrUtil.h"

/* Flag values for the 'flags' field in the cache records. */
#define PRESENT         0x01    /* The record has not been deleted. */
#define HASREAD         0x02    /* The record has been read at least once. */
#define MUSTREVALIDATE  0x04    /* This record must be revalidated. */


/* Each record in the cache starts with this: */
typedef struct {
  UINT32 url_hash;       /* The hash value of the URL */
  UINT16 url_len;        /* The size of the URL, in bytes. */
  UINT16 header_len;     /* The size of the header. */
  UINT32 body_len;       /* The size of the body. */
  UINT32 expire;         /* The expiration date, or 0 if it has none. */
  UINT32 last_mod;       /* The last modified date of the record. */
  UINT16 flags;          /* Flag values, see above. */
} CacheRecord;


/* Some information about each record in the cache is kept in memory,
 * in a doubly linked list. The list is ordered by IDs, which means
 * it has the oldest cached value first. */
typedef struct lnode {
  struct lnode *next, *prev;
  UINT32       id;             /* To identify the Storage block */
  UINT32       hashval;        /* Hashed value of the URL */
} ListNode;

static ListNode reclist = {&reclist, &reclist, 0, 0};

/* Macros to add and remove nodes from the list. */
#define list_out(p) do { \
  (p)->next->prev = (p)->prev; \
  (p)->prev->next = (p)->next; \
} while (0)

#define list_addbefore(p, pb) do { \
  (p)->prev = (pb)->prev; \
  (p)->next = (pb); \
  (pb)->prev->next = (p); \
  (pb)->prev = (p); \
} while (0)


/*
 * The Cache: implemented as a Storage Object.
 */
static struct {
  UINT8         isInitialized;
  StorageObject store;
  WAEMAINOBJECT *waeMainObj;
} theCache = {0};


/*
 * Cache_Find
 *
 * Locate a record with a specific URL in the cache.
 * "url" is the URL, as a null-terminated byte string.
 * Returns a pointer to the node in the list, or NULL if it was not found.
 * Also, the record for the found cache entry is stored in the location
 * pointed to by "rec".
 */
static ListNode *
Cache_Find (BYTE *url, CacheRecord *rec)
{
  BYTE      *tmp_url;
  ListNode *p;
  UINT32    hv;

  if (!b_HashURL (url, &hv))
    return NULL;

  for (p = reclist.next; p != &reclist; p = p->next) {
    if (p->hashval == hv) {
      if (!Storage_Get (&(theCache.store), p->id, 0,
                        sizeof (CacheRecord), rec)) {
        return NULL;
      }
      if ((tmp_url = NEWARRAY (BYTE, rec->url_len + 1)) == NULL) {
        return NULL;
      }
      if (!Storage_Get (&(theCache.store), p->id, sizeof (CacheRecord),
                        rec->url_len, tmp_url)) {
        DEALLOC (&tmp_url);
        return NULL;
      }
      tmp_url[rec->url_len] = '\0';
      if (b_EqualURL (url, tmp_url, ALL_COMP)) {
        DEALLOC (&tmp_url);
        return p;
      }
      DEALLOC (&tmp_url);
    }
  }

  return NULL;
}


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
            )
{
  CacheRecord rec;
  UINT32      *ida;
  INT16       i;
  ListNode    *p, *q;
  UINT16      len;

  theCache.waeMainObj = (WAEMAINOBJECT *)pvWAEMaStruct;
  if (theCache.waeMainObj != NULL) {
    theCache.waeMainObj->liCacheSize = liNewSize;
  }

  if (!Storage_Init (&(theCache.store), liNewSize,
                     (ReadFunction *)MEMa_readCache,
                     (WriteFunction *)MEMa_writeCache)) {
    return;
  }

  /* Free the list of cached records */
  while (reclist.next != &reclist) {
    p = reclist.next;
    list_out (p);
    DEALLOC (&p);
  }

  if (!Storage_GetAllBlockIds (&(theCache.store), &ida, &len)) {
    return;
  }
  for (i = 0; i < len; i++) {
    p = NEWARRAY (ListNode, 1);
    if (p == NULL)
      return;
    p->id = ida[i];
    if (!Storage_Get (&(theCache.store), ida[i], 0,
                      sizeof (CacheRecord), &rec)) {
      DEALLOC (&p);
      return;
    }
    p->hashval = rec.url_hash;

    /* Sort into reclist, sort ascending on Storage ID */
    for (q = reclist.next; (q != &reclist) && (q->id < p->id); q = q->next);
    list_addbefore (p, q);

    if (iFirstInit && (rec.flags & HASREAD)) {
      rec.flags &= ~HASREAD;
      if (!Storage_Put (&(theCache.store), ida[i], 0,
                        sizeof (CacheRecord), &rec)) {
        return;
      }
    }
  }

  DEALLOC (&ida);
  theCache.isInitialized = 1;
}


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
Cache_PrepareSizeChange (UINT32 liNewSize)
{
  ListNode *p;

  if (!theCache.isInitialized) {
      MEMa_cachePrepared ();
      return;
  }

  /* Remove oldest until bytesUsed is less than liNewSize */
  while (Storage_BytesUsed (&(theCache.store)) > liNewSize) {
    p = reclist.next;
    if (p == &reclist)
      break;
    list_out (p);
    Storage_DeleteBlock (&(theCache.store), p->id);
    DEALLOC (&p);
  }

  /* Do a complete compaction */
  while (Storage_Compact (&(theCache.store))) {
  }

  Storage_ChangeSize (&(theCache.store), liNewSize);
  theCache.waeMainObj->liCacheSize = liNewSize;

  /* Free the list of cached records */
  while (reclist.next != &reclist) {
    p = reclist.next;
    list_out (p);
    DEALLOC (&p);
  }
  Storage_Finalize (&(theCache.store));
  theCache.isInitialized = 0;

  MEMa_cachePrepared ();
}


/*
 * Cache_ URLisPresent
 *
 * Check if a URL is in the cache.
 * "pvUrl" is the URL as a null-terminated byte string.
 * "uiLastMod" points to a location where the last modified time
 * will be stored, in case the fucntion returns 2.
 *
 * Return values:
 *   0 => the URL is not in the cache, or has expired and cannot be
 *        revalidated.
 *   1 => the URL is in the cache and has not expired.
 *   2 => the URL is in the cache, has expired, but can be revalidated;
 *        in this case, the last modified time is returned in
 *        the 'uiLastMod' parameter.
*/
INT16
Cache_URLisPresent (void *pvUrl, UINT32 *uiLastMod)
{
  CacheRecord rec;
  UINT32      currentTime;
  UINT8       cacheMode;
  ListNode    *p;

  if (!theCache.isInitialized) {
    return 0;
  }
  if (uiLastMod != NULL)
    *uiLastMod = 0;

  if ((pvUrl == NULL) || (uiLastMod == NULL)) {
    return 0;
  }

  /* Search for the record with matching URL */
  p = (ListNode    *)Cache_Find ((BYTE *)pvUrl, (CacheRecord *)&rec);

  if (p == NULL) {
    /* URL is not in the cache */
    return 0;
  }

  cacheMode = (UINT8)(theCache.waeMainObj->iCacheMode);

  currentTime = CLNTa_currentTime( );


  if ((rec.expire > currentTime) || (rec.expire == 0)) {
    /* URL is in the cache and has not expired, or will never expire. */
    return 1;
  }

  else if (rec.last_mod != 0) {
    /* URL is in the cache, has expired, but has a last modified date.
       In this case, we might get away by revalidating the page with
       the server. Exception: the cache mode says we don't have
       to check; in this case we treat it as if it had not expired. */
    if (!(rec.flags & MUSTREVALIDATE) &&
        ((cacheMode == CACHE_MODE_NEVER_CHECK) ||
         ((cacheMode == CACHE_MODE_FIRSTTIME_CHECK) &&
          (rec.flags & HASREAD)))) {
      return 1;
    }
    else {
      *uiLastMod = rec.last_mod;
      return 2;
    }
  }

  else {
    /* URL is in cache, has expired, and cannot be revalidated: remove it. */
    Storage_DeleteBlock (&(theCache.store), p->id);
    list_out (p);
    DEALLOC (&p);

    return 0;
  }
}


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
            )
{
  CacheRecord rec;
  ListNode    *p;

  if (!theCache.isInitialized) {
    return 0;
  }

  if ((pvUrl == NULL)
      || (pvHeader == NULL)
      || (liHeaderLen == NULL)
      || (pvBody == NULL)
      || (liBodLen == NULL))
    return 0;

  p = (ListNode    *)Cache_Find ((BYTE*)pvUrl, (CacheRecord *)&rec);
  if (p == NULL)
    return 0;

  if (!(rec.flags & HASREAD)) {
    rec.flags |= HASREAD;
    if (!Storage_Put (&(theCache.store), p->id, 0,
                      sizeof (CacheRecord), &rec)) {
      return 0;
    }
  }

  *pvHeader = NULL;
  *liHeaderLen = 0;
  *pvBody = NULL;
  *liBodLen = 0;

  if (rec.header_len > 0) {
    if ((*pvHeader = NEWARRAY (BYTE, rec.header_len)) == NULL) {
      return 0;
    }
    if (!Storage_Get (&(theCache.store), p->id,
                      sizeof (CacheRecord) + rec.url_len,
                      rec.header_len, *pvHeader)) {
      return 0;
    }
  }

  if (rec.body_len > 0) {
    if ((*pvBody = NEWARRAY (BYTE, rec.body_len)) == NULL) {
      DEALLOC (pvHeader);
      return 0;
    }
    if (!Storage_Get (&(theCache.store), p->id,
                      sizeof (CacheRecord) + rec.url_len + rec.header_len,
                      rec.body_len, *pvBody)) {
      DEALLOC (pvHeader);
      DEALLOC (pvBody);
      return 0;
    }
  }

  *liHeaderLen = rec.header_len;
  *liBodLen = rec.body_len;

  return 1;
}


/*
 * Cache_GetParameters
 *
 * Compute Expire and LastModified from a Header.
 */
static void
Cache_GetParameters (BYTE* pbHeader, UINT32 uiHeadLen,
                     UINT32 *uiExp,
                     UINT32 *uiLastMod,
                     UINT8  *uiMustRevalidate)
{
  pHEADERDEF      pHeaderList = NULL;
  pHEADERELEMENT  pHeaderElement = NULL;
  UINT32          uiDate = 0;
  UINT32          uiAge = 0;
  UINT32          uiExpire = 0;
  UINT32          uiNow = 0;
  unsigned char   ucCacheControl = 0;
  UINT32          uiDeltaSec = 0;
  BOOL            bError = FALSE;

  *uiExp = 0;
  if (pbHeader == NULL) {
    return;
  }

  pHeaderList = WSP_PreParseHeaders (pbHeader, uiHeadLen, &bError);
  if (bError || (pHeaderList == NULL)) {
    return;
  }

  if (pHeaderList->pHeadList != NULL) {
    pHeaderElement = WSP_GetHeaderWK (1, Field_Cache_Control,
                                      pHeaderList->pHeadList);
    if (pHeaderElement != NULL) {
      ucCacheControl = WSP_GetCacheControlByte (pHeaderElement,
                                                &uiDeltaSec, &bError);
      /* Must-Revalidate, Proxy-Revalidate */
      if ((ucCacheControl == 137 || ucCacheControl == 138) && !bError ) {
        *uiMustRevalidate = 1;
      }	/* Max-Age */
	  else if( ucCacheControl == 130 && !bError ){

		uiNow = CLNTa_currentTime ();

		*uiExp = uiNow + uiDeltaSec;
      }
		/* Second CacheDirektive */
	  if( pHeaderElement->pNext != NULL ){
		  pHeaderElement = WSP_GetHeaderWK (1, Field_Cache_Control,
                                      pHeaderElement->pNext);
		  if (pHeaderElement != NULL) {
			  ucCacheControl = WSP_GetCacheControlByte (pHeaderElement,
		                                            &uiDeltaSec, &bError);
		      /* Must-Revalidate, Proxy-Revalidate */
		      if ((ucCacheControl == 137 || ucCacheControl == 138) && !bError ) {
			    *uiMustRevalidate = 1;
		      }	/* Max-Age */
			  else if( ucCacheControl == 130 && !bError ){

				uiNow = CLNTa_currentTime ();

				*uiExp = uiNow + uiDeltaSec;
			  }
		  }
	  }
    }
    bError = FALSE;
	if( *uiExp == 0 ){
		pHeaderElement = WSP_GetHeaderWK (1, Field_Expires,
										  pHeaderList->pHeadList);
		if (pHeaderElement != NULL) {
		  uiExpire = WSP_GetExpires (pHeaderElement, &bError);
		  if (uiExpire > 0) {
			pHeaderElement = WSP_GetHeaderWK (1, Field_Date,
											  pHeaderList->pHeadList);
			if (pHeaderElement != NULL) {
			  uiDate = WSP_GetDate (pHeaderElement, &bError);
			  if (uiDate > 0) {
				pHeaderElement = WSP_GetHeaderWK (1, Field_Age,
												  pHeaderList->pHeadList);
				if (pHeaderElement != NULL) {
				  uiAge = WSP_GetAge (pHeaderElement, &bError);
				}

				uiNow = CLNTa_currentTime ();

			    if( uiExpire > (uiDate + uiAge) )
				  *uiExp = uiNow + (uiExpire - (uiDate + uiAge));
			    else
				  *uiExp = uiNow;
			  }
			}
		  }
		}
	}
    bError = FALSE;
    pHeaderElement = WSP_GetHeaderWK (1, Field_Last_Modified,
                                      pHeaderList->pHeadList);
    if (pHeaderElement != NULL) {
      *uiLastMod = WSP_GetLastModified (pHeaderElement, &bError);
    }
  }
  DeleteHEADERDEF (pHeaderList);
}


/*
 * Cache_Write
 *
 * Write a URL, Header and Body to the cache.
 * "pvUrl" is the URL, as a null-terminated byte string,
 * "pvHeader" points to the header content and "iHeaderLen" is the length
 * of the header. Similarly, "pvBody" points to the body content and
 * "iBodLen" is the length of the body.
 *
 * Returns 1 if successful, otherwise 0.
 */
INT16
Cache_Write (void *pvUrl,
             void *pvHeader, UINT32 iHeaderLen,
             void *pvBody, UINT32 iBodLen
             )
{
  CacheRecord rec;
  UINT32      pos, tot_len, newid;
  UINT16      url_len;
  UINT32      currentTime, expire = 0, lastModified = 0;
  UINT8       mustRevalidate = 0;
  ListNode    *p;

  if (!theCache.isInitialized) {
    return 0;
  }
  if ((pvUrl == NULL)
      || (pvHeader == NULL)
      || (iHeaderLen == 0)
      || (pvBody == NULL)
      || (iBodLen == 0)) {
    return 0;
  }

  Cache_GetParameters ((BYTE* )pvHeader, (UINT32)iHeaderLen, (UINT32 *)&expire, (UINT32 *)&lastModified,
                       (UINT8  *)&mustRevalidate);

  if (expire == 0 && mustRevalidate == 0) {
    if (theCache.waeMainObj != NULL) {

      currentTime = CLNTa_currentTime ();

      expire = currentTime + theCache.waeMainObj->iDefaultExpireTime;
    }
  }
  else if (expire == 0 && mustRevalidate != 0){
      /* Dummy value to mark that the content has expired. */
      expire = 1;
  }

  /* Compute the required amount of free space. */
  url_len = (UINT16)B_STRINGLENGTH ((char*)pvUrl);
  tot_len = sizeof (CacheRecord) + url_len + iHeaderLen + iBodLen;
  if (tot_len > theCache.store.size - 4) {
    /* Too large a record, it cannot possibly fit. */
    return 0;
  }

  /* Check if URL is presently in cache; if so, remove it. */
  p = (ListNode    *)Cache_Find ((BYTE*)pvUrl, (CacheRecord *)&rec);
  if (p != NULL) {
    Storage_DeleteBlock (&(theCache.store), p->id);
    list_out (p);
    DEALLOC (&p);
  }
#ifdef PORTING_B0_WAP
  if (cfg_wae_cc_cacheCompact > 0) {
#else 
  if (0 > 0) {
#endif //PORTING_B0_WAP
     /* Delete the oldest elements in the cache, until we have enough
     * free bytes for the new element; then do compaction to collect
     * the free areas into one that is large enough. In this manner
     * we delete as few old records as possible. Hence, this strategy
     * favors memory usage over speed. */
    while (Storage_NumBytesFree (&(theCache.store)) < tot_len) {
      p = reclist.next;
      if (p == &reclist)
        break;
      list_out (p);
      Storage_DeleteBlock (&(theCache.store), p->id);
      DEALLOC (&p);
    }

    newid = 0;
    /* Try to allocate a new block; if it fails, do some compaction. */
    while (!Storage_AllocateBlock (&(theCache.store), tot_len, &newid)) {
      if (!Storage_Compact (&(theCache.store))) {
        /* Compaction did not accomplish anything: give up. */
        return 0;
      }
    }
  }
  else {
    /* Delete the oldest elements in the cache, until the allocation
       * succeeds. No compaction is done. This strategy favors speed
       * over memory usage. */
    newid = 0;
    while (!Storage_AllocateBlock (&(theCache.store), tot_len, &newid)) {
      p = reclist.next;
      if (p == &reclist) {
        /* There are no more records in the cache. The new record
         * is simply too large to fit in the cache: give up.
         * (This should never happen.) */
        return 0;
      }
      list_out (p);
      Storage_DeleteBlock (&(theCache.store), p->id);
      DEALLOC (&p);
    }

    p = NEWARRAY (ListNode, 1);
    if (p == NULL)
      return 0;
    p->id = newid;

    /* Add to list, place it last. */
    list_addbefore (p, &reclist);
    b_HashURL ((const BYTE *)pvUrl, (UINT32 *)&(p->hashval));
  }

  /* Write new cache record */
  rec.url_hash = p->hashval;
  rec.url_len = url_len;
  rec.header_len = (UINT16)iHeaderLen;
  rec.body_len = iBodLen;
  rec.expire = expire;
  rec.last_mod = lastModified;
  rec.flags = PRESENT | HASREAD;
  if (mustRevalidate)
    rec.flags |= MUSTREVALIDATE;
  if (!Storage_Put (&(theCache.store), p->id, 0,
                    sizeof (CacheRecord), &rec)) {
    return 0;
  }

  /* Write URL, header and body. */
  pos = sizeof (CacheRecord);
  if (!Storage_Put (&(theCache.store), p->id, pos, rec.url_len, pvUrl)) {
    return 0;
  }
  pos += rec.url_len;
  if (!Storage_Put (&(theCache.store), p->id, pos,
                    rec.header_len, pvHeader)) {
    return 0;
  }
  pos += rec.header_len;
  if (!Storage_Put (&(theCache.store), p->id, pos,
                    rec.body_len, pvBody)) {
    return 0;
  }

  return 1;
}


/*
 * Cache_Delete
 *
 * Delete a record from the cache, given its URL.
 * "pvUrl" is the URL as a null-terminated byte string.
 *
 * Returns 1 if succesful, otherwise 0.
 */
INT16
Cache_Delete (void *pvUrl)
{
  CacheRecord rec;
  ListNode    *p;

  if (!theCache.isInitialized) {
    return 0;
  }
  if (pvUrl == NULL)
    return 0;

  p = (ListNode    *)Cache_Find ((BYTE *)pvUrl, (CacheRecord *)&rec);
  if (p == NULL) {
    return 1;
  }

  Storage_DeleteBlock (&(theCache.store), p->id);
  list_out (p);
  DEALLOC (&p);

  return 1;
}


/*
 * Cache_ConfirmURL
 *
 * Check if a URL is in the cache.
 * Return values:
 *   0 => the URL is not in the cache, or has expired and cannot be
 *        revalidated
 *   1 => the URL is in the cache and has not expired
 *   2 => the URL is in the cache, has expired, but can be revalidated
 */
INT16
Cache_ConfirmURL (void *pvUrl)
{
  UINT32 dummy;

  if (!theCache.isInitialized) {
    return 0;
  }
  return Cache_URLisPresent (pvUrl, &dummy);
}


/*
 * Cache_DeleteMatchingPrefix
 *
 * Delete all records in the cache whose URL has the given
 * URL as a prefix.
 *
 * Returns the number of records that were deleted.
 */
INT16
Cache_DeleteMatchingPrefix (void *pvUrl)
{
  CacheRecord rec;
  ListNode    *p, *q;
  INT16       n = 0;
  BYTE        *tmp_url;

  if (!theCache.isInitialized ||
      (pvUrl == NULL)) {
    return 0;
  }

  p = reclist.next;
  while (p != &reclist) {
    q = p;
    p = p->next;

    if (!Storage_Get (&(theCache.store), q->id, 0,
                      sizeof (CacheRecord), &rec)) {
      return n;
    }
    if ((tmp_url = NEWARRAY (BYTE, rec.url_len + 1)) == NULL) {
      return n;
    }
    if (!Storage_Get (&(theCache.store), q->id, sizeof (CacheRecord),
                      rec.url_len, tmp_url)) {
      DEALLOC (&tmp_url);
      return n;
    }
    tmp_url[rec.url_len] = '\0';
    if (b_IsPrefix ((BYTE *)pvUrl, (BYTE *)tmp_url)) {
      list_out (q);
      Storage_DeleteBlock (&(theCache.store), q->id);
      DEALLOC (&q);
      n++;
    }
    DEALLOC (&tmp_url);
  }

  return n;
}

void CacheReinit(void)
{
	reclist.next    = &reclist;
	reclist.prev    = &reclist;
	reclist.id      = 0;
	reclist.hashval = 0;
}


#ifdef LOG_INTERNAL
/*
 * Cache_Debug
 *
 * Routine used for debugging purposes.
 * This routine prints the URLs of all records in the cache
 * on standard error.
 */
void
Cache_Debug (void)
{
  UINT32 *ida;
  INT16  i;
  CacheRecord rec;
  char tmp[256];
  UINT16 len;

  if (!theCache.isInitialized) {
    return;
  }
  Storage_GetAllBlockIds (&(theCache.store), &ida, &len);
  for (i = 0; i < len; i++) {
    Storage_Get (&(theCache.store), ida[i], 0, sizeof (CacheRecord), &rec);
    Storage_Get (&(theCache.store), ida[i], sizeof (CacheRecord),
                 rec.url_len, tmp);
    tmp[rec.url_len] = '\0';
    fprintf (stderr, "'%s'\n", tmp);
  }
}
#endif
