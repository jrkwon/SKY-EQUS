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
 * WTPsuppt.c
 *
 * Created by Anders Edenbrandt, Fri Mar 17 11:10:10 2000.
 *
 * Revision history:
 *
 */
#include "WTPsuppt.h"

#include "aapiclnt.h"
#ifdef TRGT_WIN32
#include <stdio.h>
#endif

/*
 * Routines that handle transaction tables:
 */

typedef struct st_ttbl_node {
  struct st_ttbl_node *next;
  TransactionType      tr;
  unsigned long        exptime;
} ttbl_node;

void *
ttbl_init (void)
{
  return 0;
}

void
ttbl_clear (void **tbl)
{
  ttbl_node *tn;

  if (tbl == NULL) {
    return;
  }

  tn = (ttbl_node *)*tbl;
  while (tn) {
    ttbl_node *tmp = tn;

    tn = tn->next;
    DEALLOC (&tmp);
  }

  *tbl = 0;
}

void
ttbl_insert (void **tbl, TransactionType *tr)
{
  ttbl_node *tmp;

  if (tbl == NULL) {
    return;
  }
  tmp = NEWARRAY (ttbl_node, 1);
  if (tmp == NULL) {
    return;
  }
  tmp->next = (struct st_ttbl_node *)*tbl;
  tmp->tr = *tr;

  *tbl = tmp;
}

void
ttbl_delete (void **tbl, SDL_Integer handle)
{
  ttbl_node *tn, *qn;

  if (tbl == NULL) {
    return;
  }

  qn = NULL;
  tn = (ttbl_node *)*tbl;

  while (tn) {
    if (tn->tr.handle == handle) {
      if (qn == NULL) {
        *tbl = tn->next;
      }
      else {
        qn->next = tn->next;
      }
      DEALLOC (&tn);
      break;
    }
    qn = tn;
    tn = tn->next;
  }
}


SDL_PId
ttbl_find (void *tbl, SDL_Integer handle, TransactionType *t)
{
  ttbl_node *tb = (ttbl_node *)tbl;

  for (; tb; tb = tb->next) {
    if (tb->tr.handle == handle) {
      *t = tb->tr;
      return tb->tr.pid;
    }
  }

  return SDL_NULL;
}

SDL_PId
ttbl_findByTID (void *tbl, AddressType src, SDL_Natural tid,
                SDL_Natural dir, TransactionType *t)
{
  ttbl_node *tb = (ttbl_node *)tbl;

  for (; tb; tb = tb->next) {
    if (dir == TR_INITIATOR) {
      if ((tb->tr.dir == dir) &&
          (tb->tr.tid == tid) &&
          (tb->tr.clientAddress.PortNumber == src.PortNumber) &&
          (yEqF_DeviceAddress (tb->tr.clientAddress.Address, src.Address))) {
        *t = tb->tr;
        return tb->tr.pid;
      }
    }
    else if ((tb->tr.dir == dir) &&
             (tb->tr.tid == tid) &&
             (tb->tr.serverAddress.PortNumber == src.PortNumber) &&
             (yEqF_DeviceAddress (tb->tr.serverAddress.Address, src.Address))) {
      *t = tb->tr;
      return tb->tr.pid;
    }
  }

  return SDL_NULL;
}

SDL_PId
ttbl_findByPort (void *tbl, SDL_Integer portnumber)
{
  ttbl_node *tb = (ttbl_node *)tbl;

  for (; tb; tb = tb->next) {
    if (((tb->tr.dir == TR_INITIATOR) &&
         (tb->tr.clientAddress.PortNumber == portnumber)) ||
        ((tb->tr.dir == TR_RESPONDER) &&
         (tb->tr.serverAddress.PortNumber == portnumber))) {
      return tb->tr.pid;
    }
  }

  return SDL_NULL;
}

SDL_PId
ttbl_pop (void **tbl)
{
  ttbl_node *tn;
  SDL_PId   tmpPid;

  if ((tbl == NULL) || (*tbl == NULL)) {
    return SDL_NULL;
  }

  tn = (ttbl_node *)*tbl;
  *tbl = tn->next;
  tmpPid = tn->tr.pid;
  DEALLOC (&tn);

  return tmpPid;
}

void
ttbl_insertByTime (void **tbl, TransactionType *t, SDL_Duration waitTime)
{
  unsigned long now = CLNTa_currentTime ();
#if defined XSCT_CMICRO
  unsigned long exptime = now + waitTime / 10;
#else
  unsigned long exptime = now + waitTime.s / 10;
#endif
  ttbl_node *tn, *ptn, *qtn;

  if (tbl == NULL) {
    return;
  }
  tn = NEWARRAY (ttbl_node, 1);
  if (tn == NULL) {
    return;
  }
  tn->tr = *t;
  tn->exptime = exptime;

  qtn = NULL;
  for (ptn = (ttbl_node *)*tbl; ptn; qtn = ptn, ptn = ptn->next) {
    if (exptime < ptn->exptime) {
      break;
    }
  }

  tn->next = ptn;
  if (qtn == NULL) {
    *tbl = tn;
  }
  else {
    qtn->next = tn;
  }
}

SDL_Duration
ttbl_checkExpired (void **tbl)
{
  unsigned long now = CLNTa_currentTime ();
  ttbl_node *tn, *ptn;
#ifdef TRGT_WIN32
  FILE *logf;
#endif

  if (tbl == NULL) {
    return SDL_TIME_LIT (0, 0, 0);
  }

  ptn = (ttbl_node *)*tbl;
  while (ptn && (now >= ptn->exptime)) {
    tn = ptn;
    ptn = ptn->next;
    DEALLOC (&tn);
  }

  *tbl = ptn;
  if (ptn == NULL) {
#ifdef TRGT_WIN32
    logf = fopen ("debug.log", "a");
    fprintf (logf, "0\n");
    fclose (logf);
#endif
    return SDL_TIME_LIT (0, 0, 0);
  }
  else {
#ifdef TRGT_WIN32
    logf = fopen ("debug.log", "a");
    fprintf (logf, "%d\n", (ptn->exptime - now) * 10);
    fclose (logf);
#endif
#if defined XSCT_CMICRO
    return SDL_DURATION_LIT (0, (ptn->exptime - now) * 10, 0);
#else
    return SDL_DURATION_LIT (0, (ptn->exptime - now), 0);
#endif
  }
}


/*
 * Routines for the TID caches:
 */

typedef struct st_tidcache_node {
  struct st_tidcache_node *next;
  SDL_Natural             tid;
  DeviceAddress           addr;
} tidcache_node;

void *
tidcache_init (void)
{
  return 0;
}

void
tidcache_clear (void **tbl)
{
  tidcache_node *tn;

  if (tbl == NULL) {
    return;
  }

  tn = (tidcache_node *)*tbl;
  while (tn) {
    tidcache_node *tmp = tn;

    tn = tn->next;
    DEALLOC (&tmp);
  }

  *tbl = 0;
}

SDL_Natural
tidcache_get (void *tbl, DeviceAddress addr)
{
  tidcache_node *tn = (tidcache_node *)tbl;
  
  for (; tn; tn = tn->next) {
    if (yEqF_DeviceAddress (tn->addr, addr)) {
      return tn->tid;
    }
  }

  return 32768;
}

void
tidcache_set (void **tbl, DeviceAddress addr, SDL_Natural val)
{
  tidcache_node **tmp = (tidcache_node **)tbl;
  tidcache_node *tn;
  
  if (tmp == NULL) {
    return;
  }
  
  for (tn = *tmp; tn; tn = tn->next) {
    if (yEqF_DeviceAddress (tn->addr, addr)) {
      tn->tid = val;
      return;
    }
  }

  tn = NEWARRAY (tidcache_node, 1);
  tn->next = *tmp;
  tn->addr = addr;
  tn->tid = val;

  *tbl = tn;
}


/*
 * Routines for generating TIDs and handles:
 */

static unsigned int current_tid = 0;
static int current_handle = 0x10000;

void
tid_init (void)
{
  srand (CLNTa_currentTime ());

  current_tid = rand () % 32768;
}

SDL_Natural
tid_new (void)
{
  unsigned int tmp = current_tid;

  current_tid++;
  if (current_tid >= 32768) {
    current_tid = 0;
  }

  return tmp;
}

void
handle_init (void)
{
  current_handle = 0x10000;
}

SDL_Natural
handle_new (void)
{
  current_handle++;

  return current_handle;
}
