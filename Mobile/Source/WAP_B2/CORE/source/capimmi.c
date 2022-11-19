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

#include "capimmi.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    MMIc_openView (UINT8 viewId, UINT8 uaMode)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_openView\n");
#endif

    pMsg->Id = eMMIc_openView;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = uaMode;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_closeView (UINT8 viewId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_closeView\n");
#endif

    pMsg->Id = eMMIc_closeView;

    pMsg->vUINT8[0] = viewId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_loadURL (UINT8 viewId, const CHAR* url, BOOL reload)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    char    *sz;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        const CHAR  *cszUrl = url;
        if (!url)
        {
            cszUrl = cszNULL;
        }
        CLNTa_log(viewId, 0, "MMIc_loadURL\nconst CHAR * \t url \t %s\nBOOL \t reload \t %u\n", cszUrl, (int)reload);
    }
#endif

    pMsg->Id = eMMIc_loadURL;

    pMsg->vBOOL = reload;
    pMsg->vUINT8[0] = viewId;
    sz = NULL;
    if (url)
    {
        size_t  len = strlen(url) + 1;
        sz = (CHAR*)OSConnectorAlloc(len*sizeof(CHAR));
        if (!sz)
            return;
        strncpy(sz, url, len);
    }
    pMsg->sz = sz;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_reload (UINT8 viewId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_reload\n");
#endif

    pMsg->Id = eMMIc_reload;

    pMsg->vUINT8[0] = viewId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_stop (UINT8 viewId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_stop\n");
#endif

    pMsg->Id = eMMIc_stop;

    pMsg->vUINT8[0] = viewId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_goBack (UINT8 viewId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_goBack\n");
#endif

    pMsg->Id = eMMIc_goBack;

    pMsg->vUINT8[0] = viewId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID   MMIc_passwordDialogResponse (UINT8 viewId, UINT8 dialogId, const CHAR *name, const CHAR *password)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    size_t  len = 0;
    CHAR    *pCHAR;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        const CHAR  *cszName = name;
        const CHAR  *cszPassword = password;
        if (!cszName)
        {
            cszName = cszNULL;
        }
        if (!cszPassword)
        {
            cszPassword = cszNULL;
        }
        CLNTa_log(viewId, 0, "MMIc_passwordDialogResponse\nUINT8 \t dialogId \t %u\nconst WHAR * \t name \t %s\nconst CHAR * \t password \t %s\n", (int)dialogId, name, password);
    }
#endif

    pMsg->Id = eMMIc_passwordDialogResponse;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = dialogId;
    pCHAR = NULL;
    if (name)
    {
        /*  Add one character for the terminating zero. */
        len = strlen(name) + 1;
        pCHAR = (CHAR*)OSConnectorAlloc(len*sizeof(CHAR));
        if (!pCHAR)
            return;
        strncpy(pCHAR, name, len);
    }
    pMsg->pCHAR[0] = pCHAR;
    pCHAR = NULL;
    if (password)
    {
        /*  Add one character for the terminating zero. */
        len += strlen(password) + 1;
        pCHAR = (CHAR*)OSConnectorAlloc(len*sizeof(CHAR));
        if (!pCHAR)
            return;
        strncpy(pCHAR, password, len);
    }
    pMsg->pCHAR[1] = pCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_promptDialogResponse (UINT8 viewId, UINT8 dialogId, const WCHAR *answer)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    WCHAR   *pWCHAR;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *pcWCHAR = answer;
        if (!pcWCHAR)
        {
            pcWCHAR = cwzNULL;
        }
        CLNTa_log(viewId, 0, "MMIc_promptDialogResponse\nUINT8 \t dialogId \t %u\nconst WCHAR * \t answer \t %S\n", (int)dialogId, pcWCHAR);
    }
#endif

    pMsg->Id = eMMIc_promptDialogResponse;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = dialogId;
    pWCHAR = NULL;
    if (answer)
    {
        size_t  len = w_strlen(answer) + 1;
        pWCHAR = (WCHAR*)OSConnectorAlloc(len*sizeof(WCHAR));
        if (!pWCHAR)
            return;
        memcpy(pWCHAR, answer, len*sizeof(WCHAR));
    }
    pMsg->pWCHAR = pWCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_confirmDialogResponse (UINT8 viewId, UINT8 dialogId, BOOL answer)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_confirmDialogResponse\nUINT8 \t dialogId \t %u\nBOOL \t answer \t %u\n", (int)dialogId, (int)answer);
#endif

    pMsg->Id = eMMIc_confirmDialogResponse;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = dialogId;
    pMsg->vBOOL = answer;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_alertDialogResponse (UINT8 viewId, UINT8 dialogId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_alertDialogResponse\nUINT8 \t dialogId \t %u\n", (int)dialogId);
#endif

    pMsg->Id = eMMIc_alertDialogResponse;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = dialogId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_keySelected (UINT8 viewId, UINT8 keyId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_keySelected\nUINT8 \t keyId \t %u\n", (int)keyId);
#endif

    pMsg->Id = eMMIc_keySelected;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = keyId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_textSelected (UINT8 viewId, UINT8 textId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_textSelected\nUINT8 \t textId \t %u\n", (int)textId);
#endif

    pMsg->Id = eMMIc_textSelected;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = textId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_imageSelected (UINT8 viewId, UINT8 imageId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_imageSelected\nUINT8 \t imageId \t %u\n", (int)imageId);
#endif

    pMsg->Id = eMMIc_imageSelected;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = imageId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_optionSelected (UINT8 viewId, UINT8 optionId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_optionSelected\nUINT8 \t optionId \t %u\n", (int)optionId);
#endif

    pMsg->Id = eMMIc_optionSelected;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = optionId;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_inputString (UINT8 viewId, UINT8 inputId, const WCHAR *text)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    WCHAR   *pWCHAR;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *pcWText = text;
        if (!pcWText)
        {
            pcWText = cwzNULL;
        }
        CLNTa_log(viewId, 0, "MMIc_inputString\nUINT8 \t inputId \t %u\nconst WCHAR * \t url \t %S\n", (int)inputId, pcWText);
    }
#endif

    pMsg->Id = eMMIc_inputString;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = inputId;
    pWCHAR = NULL;
    if (text)
    {
        size_t  len = w_strlen(text) + 1;
        pWCHAR = (WCHAR*)OSConnectorAlloc(len*sizeof(WCHAR));
        if (!pWCHAR)
            return;
        memcpy(pWCHAR, text, len*sizeof(WCHAR));
    }
    pMsg->pWCHAR = pWCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    MMIc_linkInfo (UINT8 viewId, UINT8 widgetType , UINT8 widgetId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(viewId, 0, "MMIc_linkInfo\nUINT8 \t widgetType \t %u\nUINT8 widgetId \t %u\n", widgetType, widgetId);
#endif

    pMsg->Id = eMMIc_linkInfo;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = widgetType;
    pMsg->vUINT8[2] = widgetId;

    bSendMsg = SendMsg(pMsg);
}
