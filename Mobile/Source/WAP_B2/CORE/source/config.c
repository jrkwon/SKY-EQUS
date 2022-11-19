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

#include "config.h"
#include "cmmnrsrc.h"


typedef struct configType
{
    UINT8       viewId;
    UINT8       kind;
    ConfigValue value;
    ConfigSize  size;   /*  Used to determine if it is a pointer, ie it has to be set. */
    struct configType *next;
} configType, *pconfigType;


static  configType  *configList = NULL;   /* header */
static  configType  *prevConfig = NULL;
static  configType  *currentConfig = NULL;


VOID    NewConfig(UINT8 viewId, UINT8 kind, ConfigValue value, ConfigSize size)
{
    configType  *t;

    if (t = NEWSTRUCT(configType))
    {
        t->viewId = viewId;
        t->kind = kind;
        t->value = value;
        t->size = size;

        if (configList)
            t->next = configList;
        else
            t->next = NULL;

        configList = t;
    }
}


BOOL    GetFirstConfig(UINT8 *viewId, UINT8 *kind, ConfigValue *value, ConfigSize *size)
{
    if (!configList)
        return FALSE;

    currentConfig = configList;
    prevConfig = NULL;

    *viewId = currentConfig->viewId;
    *kind = currentConfig->kind;
    *value = currentConfig->value;
    *size = currentConfig->size;

    return TRUE;
}


BOOL    GetNextConfig(UINT8 *viewId, UINT8 *kind, ConfigValue *value, ConfigSize *size)
{
    if (!currentConfig)             /* get the first */
    {
        currentConfig = configList;
        prevConfig = NULL;
    }
    else if (currentConfig->next)   /* check that the next is valid */
    {
      prevConfig = currentConfig;
      currentConfig = currentConfig->next;
    }
    else
    {
        return FALSE;
    }

    if (!currentConfig)
        return FALSE;

    *viewId = currentConfig->viewId;
    *kind = currentConfig->kind;
    *value = currentConfig->value;
    *size = currentConfig->size;

    return TRUE;
}


VOID    DeleteCurrentConfig(VOID)
{
    if (currentConfig)
    {
        if (prevConfig)
        {
            prevConfig->next = currentConfig->next;
            OSConnectorFree(currentConfig);
            currentConfig = prevConfig;
        }
        else  /* first */
        {
            configList = currentConfig->next;
            OSConnectorFree(currentConfig);
            currentConfig = NULL;
        }
    }
}


VOID    DeleteAllConfig(VOID)
{
    configType  *t;

    while (t = configList)
    {
        /* free any string config */
        if (t->size)
        {
            OSConnectorFree(t->value.p);
        }

        configList = t->next;
        OSConnectorFree(t);
    }
    currentConfig = NULL;
}
