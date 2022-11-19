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
#ifndef CAPIMEM_H
#define CAPIMEM_H


#include "tapimem.h"


IMPORT_C VOID    MEMc_initCache (memSizeType cacheSize, memSizeType restoredSize);


IMPORT_C VOID    MEMc_prepareCache (memSizeType availablePersistentMemory);


#endif
