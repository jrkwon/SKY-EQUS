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

#include "capipush.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"
#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    PUSHc_loadSI (UINT8 viewId, INT16 id)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(viewId, 0, "PUSHc_loadSI\nINT16 \t id \t %i\n", (int)id);
    }
#endif

    pMsg->Id = ePUSHc_loadSI;

    pMsg->vUINT8[0] = viewId;
    pMsg->vINT16 = id;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    PUSHc_deleteSI (INT16 id, UINT8 selection)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

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

    pMsg->Id = ePUSHc_deleteSI;

    pMsg->vINT16 = id;
    pMsg->vUINT8[0] = selection;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    PUSHc_getSIinfo (INT16 id, UINT8 selection)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

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

    pMsg->Id = ePUSHc_getSIinfo;

    pMsg->vINT16 = id;
    pMsg->vUINT8[0] = selection;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    PUSHc_loadSL (UINT8 viewId, INT16 id)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(viewId, 0, "PUSHc_loadSL\n INT16 \t id \t %i\n", (int)id);
    }
#endif

    pMsg->Id = ePUSHc_loadSL;

    pMsg->vUINT8[0] = viewId;
    pMsg->vINT16 = id;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    PUSHc_deleteSL (INT16 id, UINT8 selection)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

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

    pMsg->Id = ePUSHc_deleteSL;

    pMsg->vINT16 = id;
    pMsg->vUINT8[0] = selection;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    PUSHc_getSLinfo (INT16 id, UINT8 selection)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

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

    pMsg->Id = ePUSHc_getSLinfo;

    pMsg->vINT16 = id;
    pMsg->vUINT8[0] = selection;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID   PUSHc_requestConnectionDone( UINT8 siaId, UINT8 channelId, BOOL success)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "PUSHc_requestConnectionDone\n UINT8 \t siaId \t %i\nUINT8 \t channelId \t %i\nBOOL \t success \t %i\n", (int)siaId,(int)channelId,(int)success);
    }
#endif

    pMsg->Id = ePUSHc_requestConnectionDone;

    pMsg->vUINT8[0] = siaId;
	pMsg->vUINT8[1] = channelId;
    pMsg->vBOOL[0] = success;

    bSendMsg = SendMsg(pMsg);

}

#endif
