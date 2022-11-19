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
#ifndef AAPIMEM_H
#define AAPIMEM_H


#include "tapicmmn.h"
#include "wipprdct.h"


UINT32  MEMa_readCache  (UINT32 pos, UINT32 size, CHAR *buffer);
UINT32  MEMa_writeCache (UINT32 pos, UINT32 size, const CHAR *buffer);
VOID    MEMa_cachePrepared (VOID);


UINT32  MEMa_readDatabase  (UINT32 pos, UINT32 size, CHAR *buffer);
UINT32  MEMa_writeDatabase (UINT32 pos, UINT32 size, const CHAR *buffer);


#ifdef CONFIG_WTA
UINT32  MEMa_readServiceRepository  (UINT32 pos, UINT32 size, CHAR *buffer);
UINT32  MEMa_writeServiceRepository (UINT32 pos, UINT32 size, const CHAR *buffer);
#endif


#ifdef CONFIG_PUSH
UINT32  MEMa_readPushRepository  (UINT32 pos, UINT32 size, CHAR *buffer);
UINT32  MEMa_writePushRepository (UINT32 pos, UINT32 size, const CHAR *buffer);
VOID	MEMa_pushRepositoryClosed (VOID);
#endif


#endif
