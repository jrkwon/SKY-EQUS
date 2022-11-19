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
 * WTPsuppt.h
 *
 * Created by Anders Edenbrandt, Wed Mar 15 15:44:14 2000.
 *
 * Revision history:
 *
 */
#ifndef _WTPsuppt_h
#define _WTPsuppt_h

#include "wiptrgt.h"
#if defined XSCT_CMICRO
#include "ml_typ.h"
#else
#include "scttypes.h"
#endif
#include "ctypes.h"
#include "GlblDecl.h"

#include "pdubuf.h"
#include "BuffMgt.h"
#include "WTPpckg.h"

/*
 * Routines that handle transaction tables:
 */

void *
ttbl_init (void);

void
ttbl_clear (void **ttbl);

void
ttbl_insert (void **ttbl, TransactionType *t);

void
ttbl_delete (void **ttbl, SDL_Integer handle);

SDL_PId
ttbl_find (void *ttbl, SDL_Integer handle, TransactionType *t);

SDL_PId
ttbl_findByTID (void *ttbl, AddressType src, SDL_Natural tid,
                SDL_Natural n, TransactionType *t);

SDL_PId
ttbl_findByPort (void *ttbl, SDL_Integer portnumber);

SDL_PId
ttbl_pop (void **ttbl);

void
ttbl_insertByTime (void **ttbl, TransactionType *t, SDL_Duration waitTime);

SDL_Duration
ttbl_checkExpired (void **ttbl);


/*
 * Routines for the TID caches:
 */

void *
tidcache_init (void);

void
tidcache_clear (void **tbl);

SDL_Natural
tidcache_get (void *tbl, DeviceAddress addr);

void
tidcache_set (void **tbl, DeviceAddress addr, SDL_Natural val);


/*
 * Routines for generating TIDs and handles:
 */

void
tid_init (void);

SDL_Natural
tid_new (void);

void
handle_init (void);

SDL_Natural
handle_new (void);

#endif
