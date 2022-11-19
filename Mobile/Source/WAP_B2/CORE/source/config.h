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
#ifndef CONFIG_H
#define CONFIG_H

#include "tapicmmn.h"


typedef union   ConfigValue
{
    UINT32  u;
    void    *p;
} ConfigValue;

typedef unsigned char    ConfigSize;


VOID    NewConfig(UINT8 viewId, UINT8 kind, ConfigValue, ConfigSize);


BOOL    GetFirstConfig(UINT8 *viewId, UINT8 *kind, ConfigValue *, ConfigSize *);

BOOL    GetNextConfig(UINT8 *viewId, UINT8 *kind, ConfigValue *, ConfigSize *);

VOID    DeleteCurrentConfig(VOID);


VOID    DeleteAllConfig(VOID);

#endif
