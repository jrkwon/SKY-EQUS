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
 * cache.c
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
 *   991227, IPN: A minor changes in Cache_Write when the 
 *                Must-Revalidate flag is set.
 *   991229, IPN: Changes in Cache_GetParameters. Added two new 
 *                parameters (Date and ETag), Check at CacheControl-MaxAge 
 *                when calculating Expire values.
 *   000114, AED: Added handling of Date and ETag. Both items can
 *                be stored in the cache block, and the function
 *                Cache_URLisPresent now returns the etag as well as
 *                the last modified date, in case the entry should
 *                be revalidated. Furthermore, Cache_Write will not
 *                replace an entry that has a more recent date-field.
 *   000125, AED: Cache_PrepareSizeChange now calls MEMa_cachePrepared
 *                even if the cache is not currently initialized.
 *   000127, IPN: A minor changes in Cache_GetParameters 
 *   000128, AED: The ETag string extracted from the request headers
 *                is handled as a dynamically allocated, temporary string.
 *   000321, IPN: A minor correction in Expire-calculation.
 *   000411, IPN: Handles MaxAge and MustRevalidate simultaneous in a response. 
 *   000511, AED: Should be possible to call Cache_Init after a restart.
 *   000808, IPN: Added WmlBackNav parameter to Cache_URLisPresent. The normal
 *                caching rules is not used when request comes from the history.
 *   001107, HEAD:Different handling of expire in cache_getParameters
 *   001120, AED: New parameter in Cache_URLisPresent and Cache_Write,
 *                to distinguish between content belonging to WTA and
 *                normal content.
 *                Removed Cache_ConfirmURL. No longer used.
 *                Correction in Cache_Write, the alternative that does
 *                  compaction.
 *                Correction in Cache_Init, potential memory leak.
 *   001212, AED: Added use of File API. Configured with FILE_CACHE.
 *   010403, IPN: Changes in Cache_GetParameters. Check also after CacheControl -
 *                EncodingVersion 1.3 and 1.4
 *   010410, IPN: The function Cache_GetParameters is now available outside this file.
 *	 010430, MALU:Cast warnings removed.
 *   010507, IPN: Removed some varnings.
 *                
 */
#include "confvars.h"
#include "cache.h"

#ifdef FILE_CACHE
#include "aapifile.h"
#else
#include "storage.h"
#endif

#include "aapimem.h"

#include "cmmnrsrc.h"
#include "waedef.h"
#include "url.h"
#include "hdrutil.h"
#include "wml_uafn.h"

/* Flag values for the 'flags' field in the cache records. */
#define PRESENT         0x01    /* The record has not been deleted. */
#define HASREAD         0x02    /* The record has been read at least once. */
#define MUSTREVALIDATE  0x04    /* This record must be revalidated. */
#define BELONGS_TO_WTA  0x08

/* Each record in the cache starts with this: */
typedef struct {
  UINT32 url_hash;       /* The hash value of the URL */
  UINT16 url_len;        /* The size of the URL, in bytes. */
  UINT16 header_len;     /* The size of the header. */
  UINT32 body_len;       /* The size of the body. */
  UINT32 etag_len;       /* Length of Entity Tag, in bytes. */
  UINT32 expire;         /* The expiration date, or 0 if it has none. */
  UINT32 date;           /* Date of response to request. */
  UINT32 last_mod;       /* The last modified date of the record. */
  UINT16 flags;          /* Flag values, see above. */
#ifdef FILE_CACHE
  UINT32 creation_time;  /* Used for evicting the oldest when cache is full. */
#endif
} CacheRecord;


/* Some information about each record in the cache is kept in memory,
 * in a doubly linked list. The list is ordered by IDs, which means
 * it has the oldest cached value first. */
typedef struct lnode {
  struct lnode *next, *prev;
#ifdef FILE_CACHE
  UINT32       creation_time;
#endif
  UINT32       id;             /* To identify the Storage block */
  UINT32       hashval;        /* Hashed value of the URL */
  UINT8        isWTA;          /* The WTA flag is set */
} ListNode;

#ifdef FILE_CACHE
static ListNode reclist = {&reclist, &reclist, 0, 0, 0, 0};
#else
static ListNode reclist = {&reclist, &reclist, 0, 0, 0};
#endif

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
#ifdef FILE_CACHE
  UINT32        size;
#else
  StorageObject store;
#endif
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
Cache_Find (BYTE *url, CacheRecord *rec, INT16 isWTA)
{
  BYTE      *tmp_url;
  ListNode *p;
  UINT32    hv;

  if (!b_HashURL (url, &hv))
    return NULL;

  for (p = reclist.next; p != &reclist; p = p->next) {
    if (((isWTA < 0) || (isWTA == p->isWTA)) && (p->hashval == hv)) {
#ifdef FILE_CACHE
      if (FILEa_read ('C', p->id, rec, 0, sizeof (CacheRecord))
          != sizeof (CacheRecord)) {
#else      
      if (!Storage_Get (&(theCache.store), p->id, 0,
                        sizeof (CacheRecord), rec)) {
#endif
        return NULL;
      }
      if ((tmp_url = NEWARRAY (BYTE, rec->url_len + 1)) == NULL) {
        return NULL;
      }
#ifdef FILE_CACHE
      if (FILEa_read ('C', p->id, tmp_url, sizeof (CacheRecord),
                      rec->url_len) != rec->url_len) {
#else
      if (!Storage_Get (&(theCache.store), p->id, sizeof (CacheRecord),
                        rec->url_len, tmp_url)) {
#endif
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

  theCache.waeMainObj = (WAEMAINOBJECT*) pvWAEMaStruct;
  if (theCache.waeMainObj != NULL) {
    theCache.waeMainObj->liCacheSize = liNewSize;
  }

#ifndef FILE_CACHE
  if (!Storage_Init (&(theCache.store), liNewSize,
                     (ReadFunction *)MEMa_readCache,
                     (WriteFunction *)MEMa_writeCache)) {
    return;
  }
#endif

  /* Set list of cached records to be empty. */
  reclist.next = &reclist;
  reclist.prev = &reclist;

#ifdef FILE_CACHE
  theCache.size = 0;
  if ((i = FILEa_getFileIds ('C', NULL, 0)) < 0) {
    return;
  }
  len = (UINT16)i;
  ida = OSConnectorAlloc (sizeof (UINT32) * len);
  if (ida == NULL) {
    return;
  }
  if ((i = FILEa_getFileIds ('C', ida, len)) < 0) {
    OSConnectorFree (ida);
    return;
  }
  len = (UINT16)i;
#else  
  if (!Storage_GetAllBlockIds (&(theCache.store), &ida, &len)) {
    return;
  }
#endif

  for (i = 0; i < len; i++) {
#ifdef FILE_CACHE
    if (ida[i] == 0)
      continue;
#endif
    p = NEWARRAY (ListNode, 1);
    if (p == NULL)
      return;
    p->id = ida[i];
#ifdef FILE_CACHE
    theCache.size += FILEa_getSize ('C', ida[i]);
    if (FILEa_read ('C', ida[i], &rec, 0, sizeof (CacheRecord))
        != sizeof (CacheRecord)) {
#else
    if (!Storage_Get (&(theCache.store), ida[i], 0,
                      sizeof (CacheRecord), &rec)) {
#endif
      DEALLOC (&p);
      return;
    }
#ifdef FILE_CACHE
    p->creation_time = rec.creation_time;
#endif
    p->hashval = rec.url_hash;
    p->isWTA = ((rec.flags & BELONGS_TO_WTA) ? 1 : 0);

    /* Sort into reclist, sort ascending after creation time. */
#ifdef FILE_CACHE
    for (q = reclist.next; (q != &reclist) &&
           (q->creation_time < p->creation_time);
         q = q->next);
#else
    for (q = reclist.next; (q != &reclist) && (q->id < p->id);
         q = q->next);
#endif
    list_addbefore (p, q);

    if (iFirstInit && (rec.flags & HASREAD)) {
      rec.flags &= ~HASREAD;
#ifdef FILE_CACHE
      if (FILEa_write ('C', ida[i], &rec, 0, sizeof (CacheRecord))
          != sizeof (CacheRecord)) {
#else
      if (!Storage_Put (&(theCache.store), ida[i], 0,
                        sizeof (CacheRecord), &rec)) {
#endif
        OSConnectorFree (ida);
        return;
      }
    }
  }
#ifdef FILE_CACHE
  while (theCache.size > liNewSize) {
    p = reclist.next;
    if (p == &reclist)
      break;
    list_out (p);
    theCache.size -= FILEa_getSize ('C', p->id);
    FILEa_delete ('C', p->id);
    DEALLOC (&p);
  }
#endif
  OSConnectorFree (ida);
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

#ifdef FILE_CACHE
  while (theCache.size > liNewSize) {
    p = reclist.next;
    if (p == &reclist)
      break;
    list_out (p);
    theCache.size -= FILEa_getSize ('C', p->id);
    FILEa_delete ('C', p->id);
    DEALLOC (&p);
  }
#else
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
#endif
  theCache.waeMainObj->liCacheSize = liNewSize;

  /* Free the list of cached records */
  while (reclist.next != &reclist) {
    p = reclist.next;
    list_out (p);
    DEALLOC (&p);
  }
#ifndef FILE_CACHE
  Storage_Finalize (&(theCache.store));
#endif

  theCache.isInitialized = 0;

  MEMa_cachePrepared ();
}


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
                    )
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
  p = Cache_Find ((BYTE*) pvUrl, &rec, (INT16)(isWTA ? 1 : 0));

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
  else if ( iWmlBackNav && !(rec.flags & MUSTREVALIDATE) ){
	/* URL is in the cache and has expired, but the request comes 
	  from the history and the MustRevalidate flag is not set. */
    return 1;
  }
  else if ((rec.last_mod != 0) || (rec.etag_len > 0)) {
    /* URL is in the cache, has expired, but has a last modified date
       or an entity tag.
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
      if (rec.etag_len > 0) {
        *etag = NEWARRAY (BYTE, rec.etag_len + 1);
        if (*etag == NULL) {
          return 0;
        }
#ifdef FILE_CACHE
        FILEa_read ('C', p->id, *etag,
                    sizeof (CacheRecord) + rec.url_len + rec.header_len +
                    rec.body_len, rec.etag_len);
#else
        Storage_Get (&(theCache.store), p->id,
                     sizeof (CacheRecord) + rec.url_len + rec.header_len +
                     rec.body_len, rec.etag_len, *etag);
#endif
        (*etag)[rec.etag_len] = '\0';
      }
      else {
        *etag = NULL;
      }
      *uiLastMod = rec.last_mod;
      return 2;
    }
  }

  else {
    /* URL is in cache, has expired, and cannot be revalidated: remove it. */
#ifdef FILE_CACHE
    theCache.size -= FILEa_getSize ('C', p->id);
    FILEa_delete ('C', p->id);
#else
    Storage_DeleteBlock (&(theCache.store), p->id);
#endif
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

  p = Cache_Find ((BYTE*) pvUrl, &rec, -1);
  if (p == NULL)
    return 0;

  if (!(rec.flags & HASREAD)) {
    rec.flags |= HASREAD;
#ifdef FILE_CACHE
    if (FILEa_write ('C', p->id, &rec, 0, sizeof (CacheRecord))
        != sizeof (CacheRecord)) {
#else
    if (!Storage_Put (&(theCache.store), p->id, 0,
                      sizeof (CacheRecord), &rec)) {
#endif
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
#ifdef FILE_CACHE
    if (FILEa_read ('C', p->id, *pvHeader,
                    sizeof (CacheRecord) + rec.url_len,
                    rec.header_len) != rec.header_len) {
#else
    if (!Storage_Get (&(theCache.store), p->id,
                      sizeof (CacheRecord) + rec.url_len,
                      rec.header_len, *pvHeader)) {
#endif
      return 0;
    }
  }

  if (rec.body_len > 0) {
    if ((*pvBody = NEWARRAY (BYTE, rec.body_len)) == NULL) {
      DEALLOC (pvHeader);
      return 0;
    }
#ifdef FILE_CACHE
    if (FILEa_read ('C', p->id, *pvBody,
                    sizeof (CacheRecord) + rec.url_len + rec.header_len,
                    rec.body_len) != (INT32)rec.body_len) {
#else
    if (!Storage_Get (&(theCache.store), p->id,
                      sizeof (CacheRecord) + rec.url_len + rec.header_len,
                      rec.body_len, *pvBody)) {
#endif
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
void
Cache_GetParameters (BYTE* pbHeader, UINT32 uiHeadLen,
                     UINT32 *uiExp,
                     UINT32 *uiLastMod,
                     UINT8  *uiMustRevalidate,
                     UINT32 *uiDate,
                     BYTE   **pbEtag)
{
  pHEADERDEF      pHeaderList = NULL;
  pHEADERELEMENT  pHeaderElement = NULL;
  UINT32          uiAge = 0;
  UINT32          uiExpire = 0;
  UINT32          uiNow = 0;
  unsigned char   ucCacheControl = 0;
  UINT32          uiDeltaSec = 0;
  BOOL            bError = FALSE;
  UINT8           uiEncVer = 0;

  *uiDate = 0;
  *uiExp = 0;
  *pbEtag = NULL;
  if (pbHeader == NULL)
    return;

  pHeaderList = WSP_PreParseHeaders (pbHeader, uiHeadLen, &bError);
  if (bError || (pHeaderList == NULL))
    return;

  if (pHeaderList->pHeadList == NULL) {
    DeleteHEADERDEF (pHeaderList);
    return;
  }

  /* Cache-Control Header */
  pHeaderElement = WSP_GetHeaderWK (1, Field_Cache_Control,
                                    pHeaderList->pHeadList);
  if( pHeaderElement == NULL ){
    pHeaderElement = WSP_GetHeaderWK ( 1, Field_Cache_Control_1_3, 
                                      pHeaderList->pHeadList );
	uiEncVer++;
    if( pHeaderElement == NULL ){
      pHeaderElement = WSP_GetHeaderWK ( 1, Field_Cache_Control_1_4,
                                        pHeaderList->pHeadList );
	  uiEncVer++;
	}
  }
  do {
    if (pHeaderElement == NULL)
      break;
    ucCacheControl = WSP_GetCacheControlByte (pHeaderElement,
                                              &uiDeltaSec, &bError);
    /* Must-Revalidate, Proxy-Revalidate */
    if ((ucCacheControl == 137 || ucCacheControl == 138) && !bError ) {
      *uiMustRevalidate = 1;
    }		/* Max-Age */
    else if ((ucCacheControl == 130) && !bError) {
      uiNow = CLNTa_currentTime ();
      *uiExp = uiNow + uiDeltaSec;
    }
    /* Second CacheDirective */
    if (pHeaderElement->pNext == NULL)
      break;
	if( uiEncVer == 0 )
      pHeaderElement = WSP_GetHeaderWK (1, Field_Cache_Control,
                                        pHeaderElement->pNext);
    else if( uiEncVer == 1 )
      pHeaderElement = WSP_GetHeaderWK (1, Field_Cache_Control_1_3, 
                                        pHeaderElement->pNext);
    else if( uiEncVer == 2 )
        pHeaderElement = WSP_GetHeaderWK (1, Field_Cache_Control_1_4,
                                          pHeaderElement->pNext);
    if (pHeaderElement == NULL)
      break;
    ucCacheControl = WSP_GetCacheControlByte (pHeaderElement,
                                              &uiDeltaSec, &bError);
    /* Must-Revalidate, Proxy-Revalidate */
    if (((ucCacheControl == 137) || (ucCacheControl == 138)) && !bError) {
      *uiMustRevalidate = 1;
    }	/* Max-Age */
    else if ((ucCacheControl == 130) && !bError) {
      uiNow = CLNTa_currentTime ();
      *uiExp = uiNow + uiDeltaSec;
    }
  } while (0);


  bError = FALSE;
  /* Date Header */
  pHeaderElement = WSP_GetHeaderWK (1, Field_Date,
                                    pHeaderList->pHeadList);
  if (pHeaderElement != NULL)
    *uiDate = WSP_GetDate (pHeaderElement, &bError);

  if ((*uiExp == 0) && (*uiDate > 0)) {
    bError = FALSE;
    /* Expire Header */
    pHeaderElement = WSP_GetHeaderWK (1, Field_Expires,
                                      pHeaderList->pHeadList);
    if (pHeaderElement != NULL) {
      uiExpire = WSP_GetExpires (pHeaderElement, &bError);
      if ((uiExpire == 0) || bError)
        *uiExp = 1;
      else if (uiExpire > 0) {
        /* Age Header */
        pHeaderElement = WSP_GetHeaderWK (1, Field_Age,
                                          pHeaderList->pHeadList);
        if (pHeaderElement != NULL) {
          uiAge = WSP_GetAge (pHeaderElement, &bError);
        }
        uiNow = CLNTa_currentTime ();
        if (uiExpire > (*uiDate + uiAge))
          *uiExp = uiNow + (uiExpire - (*uiDate + uiAge));
        else
          *uiExp = uiNow;
      }
    }
  }

  bError = FALSE;
  /* Last-Modified Header */
  pHeaderElement = WSP_GetHeaderWK (1, Field_Last_Modified,
                                    pHeaderList->pHeadList);
  if (pHeaderElement != NULL) {
    *uiLastMod = WSP_GetLastModified (pHeaderElement, &bError);
  }

  bError = FALSE;
  /* E-Tag Header */
  pHeaderElement = WSP_GetHeaderWK (1, Field_Etag,
                                    pHeaderList->pHeadList);
  if (pHeaderElement != NULL) {
    BYTE   *tmp = WSP_GetEtag (pHeaderElement);
    UINT16 etag_len = B_STRINGLENGTH (tmp);

    *pbEtag = NEWARRAY (BYTE, etag_len + 1);
    if (*pbEtag != NULL) {
      B_COPYSTRING (*pbEtag, tmp);
    }        
  }

  DeleteHEADERDEF (pHeaderList);
}


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
             )
{
  CacheRecord rec;
  UINT32      pos, tot_len, newid;
  UINT16      etag_len = 0, url_len = 0;
  UINT32      currentTime, expire = 0, lastModified = 0, date = 0;
  UINT8       mustRevalidate = 0;
  BYTE*       etag = NULL;
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

  Cache_GetParameters ((BYTE*) pvHeader, iHeaderLen, &expire, &lastModified,
                       &mustRevalidate, &date, &etag );

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
  url_len = B_STRINGLENGTH (pvUrl);
  if (etag != NULL) {
    etag_len = B_STRINGLENGTH (etag);
  }
  tot_len = sizeof (CacheRecord) + url_len + iHeaderLen + iBodLen + etag_len;
#ifdef FILE_CACHE
  if (tot_len > theCache.waeMainObj->liCacheSize) {
#else
  if (tot_len > theCache.store.size - 4) {
#endif
    /* Too large a record, it cannot possibly fit. */
    DEALLOC (&etag);
    return 0;
  }

  /* Check if URL is presently in cache; if so, remove it. */
  p = Cache_Find ((BYTE*) pvUrl, &rec, (INT16)(isWTA ? 1 : 0));
  if (p != NULL) {
    if ((rec.date != 0) && (date != 0) && (date < rec.date)) {
      DEALLOC (&etag);
      return 1;
    }
#ifdef FILE_CACHE
    theCache.size -= FILEa_getSize ('C', p->id);
    FILEa_delete ('C', p->id);
#else
    Storage_DeleteBlock (&(theCache.store), p->id);
#endif
    list_out (p);
    DEALLOC (&p);
  }

#ifdef FILE_CACHE
  while (theCache.size + tot_len > theCache.waeMainObj->liCacheSize) {
    p = reclist.next;
    if (p == &reclist)
      break;
    list_out (p);
    theCache.size -= FILEa_getSize ('C', p->id);
    FILEa_delete ('C', p->id);
    DEALLOC (&p);
  }

  if (FILEa_create ('C', &newid) < 0) {
    return 0;
  }
  theCache.size += tot_len;
#else
  if (cfg_wae_cc_cacheCompact > 0) {
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
        DEALLOC (&etag);
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
        DEALLOC (&etag);
        return 0;
      }
      list_out (p);
      Storage_DeleteBlock (&(theCache.store), p->id);
      DEALLOC (&p);
    }
  }
#endif

  p = NEWARRAY (ListNode, 1);
  if (p == NULL) {
    DEALLOC (&etag);
    return 0;
  }
  p->id = newid;

  /* Add to list, place it last. */
#ifdef FILE_CACHE
  p->creation_time = currentTime;
#endif
  list_addbefore (p, &reclist);
  b_HashURL ((BYTE*) pvUrl, &(p->hashval));
  p->isWTA = (isWTA ? 1 : 0);

  /* Write new cache record */
  rec.url_hash = p->hashval;
  rec.url_len = url_len;
  rec.header_len = (UINT16)iHeaderLen;
  rec.body_len = iBodLen;
  rec.etag_len = etag_len;
  rec.expire = expire;
  rec.date = date;
  rec.last_mod = lastModified;
  rec.flags = PRESENT | HASREAD;
  if (mustRevalidate)
    rec.flags |= MUSTREVALIDATE;
  if (isWTA)
    rec.flags |= BELONGS_TO_WTA;
#ifdef FILE_CACHE
  rec.creation_time = p->creation_time;
#endif
#ifdef FILE_CACHE
  if (FILEa_write ('C', p->id, &rec, 0, sizeof (CacheRecord))
      != sizeof (CacheRecord)) {
#else
  if (!Storage_Put (&(theCache.store), p->id, 0,
                    sizeof (CacheRecord), &rec)) {
#endif
    DEALLOC (&etag);
    return 0;
  }

  /* Write URL, header,  body and ETag. */
  pos = sizeof (CacheRecord);
#ifdef FILE_CACHE
  if (FILEa_write ('C', p->id, pvUrl, pos, rec.url_len)
      != (INT32)rec.url_len) {
#else
  if (!Storage_Put (&(theCache.store), p->id, pos, rec.url_len, pvUrl)) {
#endif
    DEALLOC (&etag);
    return 0;
  }
  pos += rec.url_len;
  if (rec.header_len > 0) {
#ifdef FILE_CACHE
    if (FILEa_write ('C', p->id, pvHeader, pos, rec.header_len)
        != (INT32)rec.header_len) {
#else
    if (!Storage_Put (&(theCache.store), p->id, pos,
                      rec.header_len, pvHeader)) {
#endif
      DEALLOC (&etag);
      return 0;
    }
  }
  pos += rec.header_len;
  if (rec.body_len > 0) {
#ifdef FILE_CACHE
    if (FILEa_write ('C', p->id, pvBody, pos, rec.body_len)
        != (INT32)rec.body_len) {
#else
    if (!Storage_Put (&(theCache.store), p->id, pos,
                      rec.body_len, pvBody)) {
#endif
      DEALLOC (&etag);
      return 0;
    }
  }
  pos += rec.body_len;
  if (rec.etag_len > 0) {
#ifdef FILE_CACHE
    if (FILEa_write ('C', p->id, etag, pos, rec.etag_len)
        != (INT32)rec.etag_len) {
#else
    if (!Storage_Put (&(theCache.store), p->id, pos, rec.etag_len, etag)) {
#endif
      DEALLOC (&etag);
      return 0;
    }
  }

  DEALLOC (&etag);
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

  p = Cache_Find ((BYTE*) pvUrl, &rec, -1);
  if (p == NULL) {
    return 1;
  }

#ifdef FILE_CACHE
  theCache.size -= FILEa_getSize ('C', p->id);
  FILEa_delete ('C', p->id);
#else
  Storage_DeleteBlock (&(theCache.store), p->id);
#endif
  list_out (p);
  DEALLOC (&p);

  return 1;
}

/*
 * Cache_Delete_Push
 *
 * Delete a record from the cache, given its URL.
 * "pvUrl" is the URL as a null-terminated byte string.
 *
 * This function ignores the query-part of the URL.
 *
 * Returns 1 if succesful, otherwise 0.
 */

INT16
Cache_Delete_Push (void *pvUrl)
{
  CacheRecord rec;
  ListNode    *p;
  void        *pvNormUrl;
  URL         url;
  void        *pvNoQUrl;

  if (!theCache.isInitialized) {
    return 0;
  }
  if (pvUrl == NULL)
    return 0;

  /* Normalize */
  pvNormUrl = SDL_Normalise(NULL, pvUrl);
  if (pvNormUrl == NULL)
    return 0;
  
  /* Remove query */
  if(!URL_FromByteString(pvNormUrl, &url))
    return 0;
  URL_RemoveQuery(&url);
  pvNoQUrl = URL_ToByteString(&url);
  if (pvNoQUrl == NULL)
    return 0;
  p = Cache_Find (pvNoQUrl, &rec, -1);
  DEALLOC (&pvNoQUrl);
  if (p == NULL) {
    return 1;
  }

#ifdef FILE_CACHE
  theCache.size -= FILEa_getSize ('C', p->id);
  FILEa_delete ('C', p->id);
#else
  Storage_DeleteBlock (&(theCache.store), p->id);
#endif
  list_out (p);
  DEALLOC (&p);

  return 1;
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

#ifdef FILE_CACHE
    if (FILEa_read ('C', q->id, &rec, 0, sizeof (CacheRecord))
        != sizeof (CacheRecord)) {
#else
    if (!Storage_Get (&(theCache.store), q->id, 0,
                      sizeof (CacheRecord), &rec)) {
#endif
      return n;
    }
    if ((tmp_url = NEWARRAY (BYTE, rec.url_len + 1)) == NULL) {
      return n;
    }
#ifdef FILE_CACHE
    if (FILEa_read ('C', q->id, tmp_url, sizeof (CacheRecord),
                    rec.url_len) != rec.url_len) {
#else
    if (!Storage_Get (&(theCache.store), q->id, sizeof (CacheRecord),
                      rec.url_len, tmp_url)) {
#endif
      DEALLOC (&tmp_url);
      return n;
    }
    tmp_url[rec.url_len] = '\0';
    if (b_IsPrefix ((BYTE*) pvUrl, tmp_url)) {
      list_out (q);
#ifdef FILE_CACHE
      theCache.size -= FILEa_getSize ('C', q->id);
      FILEa_delete ('C', q->id);
#else
      Storage_DeleteBlock (&(theCache.store), q->id);
#endif
      DEALLOC (&q);
      n++;
    }
    DEALLOC (&tmp_url);
  }

  return n;
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
  UINT32     *ida;
  INT16       i;
  CacheRecord rec;
  char        tmp[256];
  UINT16      len;

  if (!theCache.isInitialized) {
    return;
  }
#ifdef FILE_CACHE
  len = FILEa_getFileIds ('C', NULL, 0);
  ida = OSConnectorAlloc (sizeof (UINT32) * len);
  len = FILEa_getFileIds ('C', ida, len);
#else
  Storage_GetAllBlockIds (&(theCache.store), &ida, &len);
#endif
  for (i = 0; i < len; i++) {
#ifdef FILE_CACHE
    if (ida[i] == 0)
      continue;
    FILEa_read ('C', ida[i], &rec, 0, sizeof (CacheRecord));
    FILEa_read ('C', ida[i], tmp, sizeof (CacheRecord), rec.url_len);
#else
    Storage_Get (&(theCache.store), ida[i], 0, sizeof (CacheRecord), &rec);
    Storage_Get (&(theCache.store), ida[i], sizeof (CacheRecord),
                 rec.url_len, tmp);
#endif
    tmp[rec.url_len] = '\0';
#ifdef FILE_CACHE
    fprintf (stderr, "'%s' <%d>\n", tmp, rec.creation_time);
#else
    fprintf (stderr, "'%s'\n", tmp);
#endif
  }
  OSConnectorFree (ida);
}
#endif
