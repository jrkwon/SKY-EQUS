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
#include "tapimem.h"


VOID    MEMa_cachePrepared (VOID);

memSizeType MEMa_readCache (memSizeType pos, memSizeType size, CHAR *buffer);

memSizeType MEMa_writeCache (memSizeType pos, memSizeType size, const CHAR *buffer);

#ifdef CONFIG_WTA
memSizeType MEMa_readRepository (memSizeType pos, memSizeType size, CHAR *buffer);

memSizeType MEMa_writeRepository (memSizeType pos, memSizeType size, const CHAR *buffer);
#endif

#ifdef CONFIG_PUSH
memSizeType MEMa_readPushBuffer (memSizeType pos, memSizeType size, CHAR *buffer);

memSizeType MEMa_writePushBuffer (memSizeType pos, memSizeType size, const CHAR *buffer);
#endif

#endif
