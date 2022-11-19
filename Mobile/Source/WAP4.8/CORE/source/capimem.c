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
#include "wiptrgt.h"
#include "ml_typw.h" 
#include "wap.ifc"
#include "capimem.h"
#include "cmmnrsrc.h"
#include "dbfunct.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    MEMc_initCache (UINT32 cacheSize)
{
	XMK_SEND_TMP_VARS
	yPDef_InitCache var;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_initCache\nUINT32 \t cacheSize \t %u\n", cacheSize);
#endif

    var.Param1 = cacheSize;

    XMK_SEND_ENV( ENV,
                  InitCache,
                  xDefaultPrioSignal,
                  sizeof( yPDef_InitCache ),
                  &var,
                  GLOBALPID(XPTID_RES_CC,0));
}

EXPORT_C VOID    MEMc_prepareCache (UINT32 availablePersistentMemory)
{
	XMK_SEND_TMP_VARS
	yPDef_PrepareCache var;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_prepareCache\nUINT32 \t availablePersistentMemory \t %u\n", availablePersistentMemory);
#endif

    var.Param1 = availablePersistentMemory;

    XMK_SEND_ENV( ENV,
                  PrepareCache,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PrepareCache ),
                  &var,
                  GLOBALPID(XPTID_RES_CC,0));
}

EXPORT_C VOID    MEMc_clearDatabase( UINT8 kind )
{

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_clearDatabase\nUINT8 \t kind \t %u\n", kind);
#endif

	if( kind == DATABASE_AUTHENTICATION )
		deleteHostAuth( );
	else if( kind == DATABASE_COOKIE )
		deleteAllCookies( );

}

#if defined CONFIG_PUSH

EXPORT_C VOID    MEMc_initPushRepository (UINT32 repositorySize)
{
	XMK_SEND_TMP_VARS
    yPDef_InitPushRepository var;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_initPushRepository\nUINT32 \t repositorySize \t %u\n", repositorySize);
#endif

    var.Param1 = repositorySize;

    XMK_SEND_ENV( ENV,
                  InitPushRepository,
                  xDefaultPrioSignal,
                  sizeof( yPDef_InitPushRepository ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}

#endif

#if defined CONFIG_PUSH

EXPORT_C VOID    MEMc_closePushRepository (UINT32 availablePersistentMemory)
{
	XMK_SEND_TMP_VARS
    yPDef_ClosePushRepository var;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "MEMc_closePushRepository\nUINT32 \t availablePersistentMemory \t %u\n", availablePersistentMemory);
#endif

    var.Param1 = availablePersistentMemory;

    XMK_SEND_ENV( ENV,
                  ClosePushRepository,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ClosePushRepository ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}

#endif