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
#include "wipprdct.h"

#ifdef CONFIG_PUSH
#include "ml_typw.h" 
#include "wap.ifc"
#include "capipush.h"
#include "cmmnrsrc.h"
#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    PUSHc_loadSI (UINT8 viewId, INT16 id)
{
	XMK_SEND_TMP_VARS

    yPDef_LoadSI var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(viewId, 0, "PUSHc_loadSI\nINT16 \t id \t %i\n", (int)id);
    }
#endif

    var.Param1 = viewId;
    var.Param2 = id;

    XMK_SEND_ENV( ENV,
                  LoadSI,
                  xDefaultPrioSignal,
                  sizeof( yPDef_LoadSI ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}


EXPORT_C VOID    PUSHc_deleteSI (INT16 id, UINT8 selection)
{
	XMK_SEND_TMP_VARS

    yPDef_DeleteSI var;

#if     defined LOG_EXTERNAL
    {
        char    szFormat[256];
        size_t  strlength, end = 0;
        const char  *csz;

        strlength = strlen(csz = "PUSHc_deleteSI\nINT16 \t id \t %i\nUINT8 \t selection \t %u \t (");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (selection)
        {
            case PUSH_DEL_ALL:
                strlength = strlen(csz = "PUSH_DEL_ALL)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_DEL_EXP:
                strlength = strlen(csz = "PUSH_DEL_EXP)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_DEL_NON_EXP:
                strlength = strlen(csz = "PUSH_DEL_NON_EXP)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_DEL_LOADED:
                strlength = strlen(csz = "PUSH_DEL_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_DEL_NON_LOADED:
                strlength = strlen(csz = "PUSH_DEL_NON_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            default:
                strlength = strlen(csz = "Currently not a supported selection.)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
        }

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, (int)id, (int)selection);
    }
#endif

    var.Param1 = id;
    var.Param2 = selection;

    XMK_SEND_ENV( ENV,
                  DeleteSI,
                  xDefaultPrioSignal,
                  sizeof( yPDef_DeleteSI ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}


EXPORT_C VOID    PUSHc_getSIinfo (INT16 id, UINT8 selection)
{
	XMK_SEND_TMP_VARS

    yPDef_GetSIinfo var;
 
#if     defined LOG_EXTERNAL
    {
        char    szFormat[256];
        size_t  strlength, end = 0;
        const char  *csz;

        strlength = strlen(csz = "PUSHc_getSIinfo\nINT16 \t id \t %i\nUINT8 \t selection \t %u \t (");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (selection)
        {
            case PUSH_SHOW_ALL:
                strlength = strlen(csz = "PUSH_SHOW_ALL)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_SHOW_EXP:
                strlength = strlen(csz = "PUSH_SHOW_EXP)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_SHOW_NON_EXP:
                strlength = strlen(csz = "PUSH_SHOW_NON_EXP)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_SHOW_LOADED:
                strlength = strlen(csz = "PUSH_SHOW_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_SHOW_NON_LOADED:
                strlength = strlen(csz = "PUSH_SHOW_NON_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            default:
                strlength = strlen(csz = "Currently not a supported selection.)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
        }

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, (int)id, (int)selection);
    }
#endif

    var.Param1 = id;
    var.Param2 = selection;

    XMK_SEND_ENV( ENV,
                  GetSIinfo,
                  xDefaultPrioSignal,
                  sizeof( yPDef_GetSIinfo ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}


EXPORT_C VOID    PUSHc_loadSL (UINT8 viewId, INT16 id)
{
	XMK_SEND_TMP_VARS

    yPDef_LoadSL var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(viewId, 0, "PUSHc_loadSL\n INT16 \t id \t %i\n", (int)id);
    }
#endif

    var.Param1 = viewId;
    var.Param2 = id;

    XMK_SEND_ENV( ENV,
                  LoadSL,
                  xDefaultPrioSignal,
                  sizeof( yPDef_LoadSL ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}

EXPORT_C VOID    PUSHc_deleteSL (INT16 id, UINT8 selection)
{
	XMK_SEND_TMP_VARS

    yPDef_DeleteSL var;

#if     defined LOG_EXTERNAL
    {
        char    szFormat[256];
        size_t  strlength, end = 0;
        const char  *csz;

        strlength = strlen(csz = "PUSHc_deleteSL\nINT16 \t id \t %i\nUINT8 \t selection \t %u \t (");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (selection)
        {
            case PUSH_DEL_ALL:
                strlength = strlen(csz = "PUSH_DEL_ALL)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_DEL_LOADED:
                strlength = strlen(csz = "PUSH_DEL_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_DEL_NON_LOADED:
                strlength = strlen(csz = "PUSH_DEL_NON_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            default:
                strlength = strlen(csz = "Currently not a supported selection.)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
        }

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, (int)id, (int)selection);
    }
#endif

    var.Param1 = id;
    var.Param2 = selection;        
    XMK_SEND_ENV( ENV,
                  DeleteSL,
                  xDefaultPrioSignal,
                  sizeof( yPDef_DeleteSL ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}


EXPORT_C VOID    PUSHc_getSLinfo (INT16 id, UINT8 selection)
{
	XMK_SEND_TMP_VARS

    yPDef_GetSLinfo var;

#if     defined LOG_EXTERNAL
    {
        char    szFormat[256];
        size_t  strlength, end = 0;
        const char  *csz;

        strlength = strlen(csz = "PUSHc_getSLinfo\nINT16 \t id \t %i\nUINT8 \t selection \t %u \t (");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (selection)
        {
            case PUSH_SHOW_ALL:
                strlength = strlen(csz = "PUSH_SHOW_ALL)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_SHOW_LOADED:
                strlength = strlen(csz = "PUSH_SHOW_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            case PUSH_SHOW_NON_LOADED:
                strlength = strlen(csz = "PUSH_SHOW_NON_LOADED)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
            default:
                strlength = strlen(csz = "Currently not a supported selection.)\n");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                break;
        }

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, (int)id, (int)selection);
    }
#endif

    var.Param1 = id;
    var.Param2 = selection;

    XMK_SEND_ENV( ENV,
                  GetSLinfo,
                  xDefaultPrioSignal,
                  sizeof( yPDef_GetSLinfo ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}


EXPORT_C VOID   PUSHc_requestConnectionDone( UINT8 siaId, UINT8 channelId, BOOL success)
{
	XMK_SEND_TMP_VARS

    yPDef_PushReqConnectionDone var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "PUSHc_requestConnectionDone\n UINT8 \t siaId \t %i\nUINT8 \t channelId \t %i\nBOOL \t success \t %i\n", (int)siaId,(int)channelId,(int)success);
    }
#endif

    var.Param1 = siaId;
	var.Param2 = channelId;
    var.Param3 = success;

    XMK_SEND_ENV( ENV,
                  PushReqConnectionDone,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PushReqConnectionDone ),
                  &var,
                  GLOBALPID( XPTID_Push_ME,0));
}

EXPORT_C VOID    PUSHc_checkIfAnyUnloadedMsg (void)
{
	XMK_SEND_TMP_VARS

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "PUSHc_checkIfAnyUnloadedMsg\n");
    }
#endif

    XMK_SEND_ENV( ENV,
                  CheckIfAnyUnloadedMsg,
                  xDefaultPrioSignal,
                  0,
                  NULL,
                  GLOBALPID(XPTID_Push_ME,0));
}


EXPORT_C VOID    PUSHc_changeStatus (INT16 id, UINT8 status)
{
	XMK_SEND_TMP_VARS

    yPDef_ChangeStatus var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "PUSHc_changeStatus\nINT16 \t id \t %i\n", (int)id);
    }
#endif

    var.Param1 = id;
	var.Param2 = status;

    XMK_SEND_ENV( ENV,
                  ChangeStatus,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ChangeStatus ),
                  &var,
                  GLOBALPID(XPTID_Push_ME,0));
}


EXPORT_C VOID PUSHc_removePushMessage(INT8 type)
{
	XMK_SEND_TMP_VARS

	yPDef_RemovePushMessage var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "PUSHc_removePushMessage\n INT8 \t type\n", (int)type);
    }
#endif

    var.Param1 = type;

    XMK_SEND_ENV( ENV,
                  RemovePushMessage,
                  xDefaultPrioSignal,
                  sizeof( yPDef_RemovePushMessage ),
                  &var,
                  GLOBALPID( XPTID_Push_ME,0));
}

#endif
