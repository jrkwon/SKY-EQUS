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
#include "capimmi.h"
#include "cmmnrsrc.h"

#if     defined LOG_EXTERNAL
#include "aapiclnt.h"
#endif


EXPORT_C VOID    MMIc_startUserAgent (UINT8 objectId, UINT8 uaMode)
{
	XMK_SEND_TMP_VARS

    yPDef_OpenView var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_startUserAgent\n");
#endif

    var.Param1 = objectId;
    var.Param2 = uaMode;

    XMK_SEND_ENV( ENV,
                  OpenView,
                  xDefaultPrioSignal,
                  sizeof( yPDef_OpenView ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID    MMIc_terminateUserAgent (UINT8 objectId)
{
	XMK_SEND_TMP_VARS

    yPDef_CloseView var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_terminateUserAgent\n");
#endif

    var.Param1 = objectId;

    XMK_SEND_ENV( ENV,
                  CloseView,
                  xDefaultPrioSignal,
                  sizeof( yPDef_CloseView ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID    MMIc_loadURL (UINT8 objectId, const CHAR* url, BOOL reload)
{
	XMK_SEND_TMP_VARS

    yPDef_LoadURL var;
    char    *sz;

#if     defined LOG_EXTERNAL
    {
        const CHAR  *cszUrl = url;
        if (!url)
        {
            cszUrl = cszNULL;
        }
        CLNTa_log(objectId, 0, "MMIc_loadURL\nconst CHAR * \t url \t %s\nBOOL \t reload \t %u\n", cszUrl, (int)reload);
    }
#endif

    var.Param3 = reload;
    var.Param1 = objectId;
    sz = NULL;
    if (url)
    {
        size_t  len = strlen(url) + 1;
        sz = (CHAR*)OSConnectorAlloc(len*sizeof(CHAR));
        if (!sz)
            return;
        strncpy(sz, url, len);
    }
    var.Param2 = sz;

    XMK_SEND_ENV( ENV,
                  LoadURL,
                  xDefaultPrioSignal,
                  sizeof( yPDef_LoadURL ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID    MMIc_reload (UINT8 objectId)
{
	XMK_SEND_TMP_VARS

    yPDef_ReloadCard var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_reload\n");
#endif

    var.Param1 = objectId;

    XMK_SEND_ENV( ENV,
                  ReloadCard,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ReloadCard ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID    MMIc_stop (UINT8 objectId)
{
	XMK_SEND_TMP_VARS

    yPDef_Abort var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_stop\n");
#endif

    var.Param1 = objectId;

    XMK_SEND_ENV( ENV,
                  Abort,
                  xDefaultPrioSignal,
                  sizeof( yPDef_Abort ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID    MMIc_goBack (UINT8 objectId)
{
	XMK_SEND_TMP_VARS

    yPDef_GoBack var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_goBack\n");
#endif

    var.Param1 = objectId;

   XMK_SEND_ENV( ENV,
                  GoBack,
                  xDefaultPrioSignal,
                  sizeof( yPDef_GoBack ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID   MMIc_passwordDialogResponse (UINT8 objectId, UINT16 dialogId, const CHAR *name, const CHAR *password)
{
	XMK_SEND_TMP_VARS

    yPDef_PasswordDialogResponse var;
    size_t  len;
    CHAR    *pCHAR;

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
        CLNTa_log(objectId, 0, "MMIc_passwordDialogResponse\nUINT16 \t dialogId \t %u\nconst CHAR * \t name \t %s\nconst CHAR * \t password \t %s\n", (int)dialogId, cszName, cszPassword);
    }
#endif

    var.Param1 = objectId;
    var.Param2 = dialogId;
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
    var.Param3 = pCHAR;
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
    var.Param4 = pCHAR;

    XMK_SEND_ENV( ENV,
                  PasswordDialogResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PasswordDialogResponse ),
                  &var,
                  GLOBALPID(XPTID_RES_ME,0));
}


EXPORT_C VOID    MMIc_promptDialogResponse (UINT8 objectId, UINT8 dialogId, const WCHAR *answer)
{
	XMK_SEND_TMP_VARS

    yPDef_PromptDialogResponse var;
    WCHAR   *pWCHAR;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *pcWCHAR = answer;
        if (!pcWCHAR)
        {
            pcWCHAR = cwzNULL;
        }
        CLNTa_log(objectId, 0, "MMIc_promptDialogResponse\nUINT8 \t dialogId \t %u\nconst WCHAR * \t answer \t %S\n", (int)dialogId, pcWCHAR);
    }
#endif

    var.Param1 = objectId;
    var.Param2 = dialogId;
    pWCHAR = NULL;
    if (answer)
    {
        size_t  len = w_strlen(answer) + 1;
        pWCHAR = (WCHAR*)OSConnectorAlloc(len*sizeof(WCHAR));
        if (!pWCHAR)
            return;
        memcpy(pWCHAR, answer, len*sizeof(WCHAR));
    }
    var.Param3 = pWCHAR;

    XMK_SEND_ENV( ENV,
                  PromptDialogResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PromptDialogResponse ),
                  &var,
                  GLOBALPID(XPTID_WMLS_SM,0));
}


EXPORT_C VOID    MMIc_confirmDialogResponse (UINT8 objectId, UINT8 dialogId, BOOL answer)
{
	XMK_SEND_TMP_VARS

    yPDef_ConfirmDialogResponse var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_confirmDialogResponse\nUINT8 \t dialogId \t %u\nBOOL \t answer \t %u\n", (int)dialogId, (int)answer);
#endif

    var.Param1 = objectId;
    var.Param2 = dialogId;
    var.Param3 = answer;

   XMK_SEND_ENV( ENV,
                  ConfirmDialogResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ConfirmDialogResponse ),
                  &var,
                  GLOBALPID(XPTID_WMLS_SM,0));
}


EXPORT_C VOID    MMIc_alertDialogResponse (UINT8 objectId, UINT8 dialogId)
{
	XMK_SEND_TMP_VARS

    yPDef_AlertDialogResponse var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_alertDialogResponse\nUINT8 \t dialogId \t %u\n", (int)dialogId);
#endif

    var.Param1 = objectId;
    var.Param2 = dialogId;

   XMK_SEND_ENV( ENV,
                  AlertDialogResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_AlertDialogResponse ),
                  &var,
                  GLOBALPID(XPTID_WMLS_SM,0));
}


EXPORT_C VOID    MMIc_keySelected (UINT8 objectId, UINT8 keyId)
{
	XMK_SEND_TMP_VARS

    yPDef_KeySelected var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_keySelected\nUINT8 \t keyId \t %u\n", (int)keyId);
#endif

    var.Param1 = objectId;
    var.Param2 = keyId;

    XMK_SEND_ENV( ENV,
                  KeySelected,
                  xDefaultPrioSignal,
                  sizeof( yPDef_KeySelected ),
                  &var,
                  GLOBALPID(XPTID_MMI_DISP,0));
}


EXPORT_C VOID    MMIc_textSelected (UINT8 objectId, UINT8 textId)
{
	XMK_SEND_TMP_VARS

    yPDef_TextSelected var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_textSelected\nUINT8 \t textId \t %u\n", (int)textId);
#endif

    var.Param1 = objectId;
    var.Param2 = textId;

    XMK_SEND_ENV( ENV,
                  TextSelected,
                  xDefaultPrioSignal,
                  sizeof( yPDef_TextSelected ),
				  &var,
                  GLOBALPID(XPTID_MMI_DISP,0));
}


EXPORT_C VOID    MMIc_imageSelected (UINT8 objectId, UINT8 imageId)
{
	XMK_SEND_TMP_VARS

    yPDef_ImageSelected var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_imageSelected\nUINT8 \t imageId \t %u\n", (int)imageId);
#endif

    var.Param1 = objectId;
    var.Param2 = imageId;

    XMK_SEND_ENV( ENV,
                  ImageSelected,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ImageSelected ),
                  &var,
                  GLOBALPID(XPTID_MMI_DISP,0));
}


EXPORT_C VOID    MMIc_optionSelected (UINT8 objectId, UINT8 optionId)
{
	XMK_SEND_TMP_VARS

    yPDef_ChangeOptionState var;
 
#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_optionSelected\nUINT8 \t optionId \t %u\n", (int)optionId);
#endif

    var.Param1 = objectId;
    var.Param2 = optionId;

    XMK_SEND_ENV( ENV,
                  ChangeOptionState,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ChangeOptionState ),
                  &var,
                  GLOBALPID(XPTID_MMI_DISP,0));
}


EXPORT_C VOID    MMIc_inputString (UINT8 objectId, UINT8 inputId, const WCHAR *text)
{
	XMK_SEND_TMP_VARS

    yPDef_InputString var;
	WCHAR *pWCHAR;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *pcWText = text;
        if (!pcWText)
        {
            pcWText = cwzNULL;
        }
        CLNTa_log(objectId, 0, "MMIc_inputString\nUINT8 \t inputId \t %u\nconst WCHAR * \t url \t %s\n", (int)inputId, pcWText);
    }
#endif

    var.Param1 = objectId;
    var.Param2 = inputId;
    pWCHAR = NULL;
    if (text)
    {
        size_t  len = w_strlen(text) + 1;
        pWCHAR = (WCHAR*)OSConnectorAlloc(len*sizeof(WCHAR));
        if (!pWCHAR)
            return;
        memcpy(pWCHAR, text, len*sizeof(WCHAR));
    }
    var.Param3 = pWCHAR;

    XMK_SEND_ENV( ENV,
                  InputString,
                  xDefaultPrioSignal,
                  sizeof( yPDef_InputString ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID    MMIc_linkInfo (UINT8 objectId, UINT8 widgetType , UINT8 widgetId)
{
	XMK_SEND_TMP_VARS

    yPDef_LinkInfo var;

#if     defined LOG_EXTERNAL
    CLNTa_log(objectId, 0, "MMIc_linkInfo\nUINT8 \t widgetType \t %u\nUINT8 widgetId \t %u\n", widgetType, widgetId);
#endif

    var.Param1 = objectId;
    var.Param2 = widgetType;
    var.Param3 = widgetId;

   XMK_SEND_ENV( ENV,
                  LinkInfo,
                  xDefaultPrioSignal,
                  sizeof( yPDef_LinkInfo ),
                  &var,
                  GLOBALPID(XPTID_MMI_DISP,0));
}


#ifdef CAN_SIGN_TEXT
EXPORT_C VOID    MMIc_textSigned (UINT8 objectId, UINT8 signId, UINT8 algorithm, const CHAR *signature, UINT16 sigLen, const CHAR *hashedKey, UINT16 hashedKeyLen, const CHAR *certificate, UINT16 certificateLen, UINT8 certificateType, UINT16 err)
{
	XMK_SEND_TMP_VARS

    yPDef_TextSigned var;
     CHAR    *pCHAR;

#if     defined LOG_EXTERNAL
    {
        const CHAR  *_signature = cszNULL;
        const CHAR  *_hashedKey = cszNULL;
        const CHAR  *_certificate = cszNULL;
        char    *strSignature = NULL;
        char    *strHashedKey = NULL;
        char    *strCertificate = NULL;
        if (signature)
        {
            _signature = strSignature = OctetString2HexString(signature, sigLen);
        }
        if (hashedKey)
        {
            _hashedKey = strHashedKey = OctetString2HexString(hashedKey, hashedKeyLen);
        }
        if (certificate)
        {
            _certificate = strCertificate = OctetString2HexString(certificate, certificateLen);
        }
        CLNTa_log(objectId, 0, "MMIc_textSigned\nUINT8 \t signId \t %u\nUINT8 \t algorithm \t %u\nconst \t CHAR *\t signature \t %s\nUINT16 \t sigLen \t %u\nconst CHAR * \t hashedKey \t %s\nUINT16 \t hashedKeyLen \t %u\nconst CHAR * \t certificate \t %s\nUINT16 \t certificateLen \t %u\nUINT8 \t certificateType \t %u, UINT16 \t err \t %u\n", (int)signId, (int)algorithm, _signature, (int)sigLen, _hashedKey, (int)hashedKeyLen, _certificate, (int)certificateLen, (int)certificateType, (int)err);
/*
Values for algorithm parameter in MMIc_textSigned:
#define SIGN_ALG_RSA        1
#define SIGN_ALG_ECDSA      2

Values for certificateType parameter in MMIc_textSigned:
#define SIGN_WTLS_CERTIFICATE   2
#define SIGN_X509_CERTIFICATE   3
#define SIGN_X968_CERTIFICATE   4
#define SIGN_URL_CERTIFICATE    5

Values for err parameter in MMIc_textSigned:
#define SIGN_NO_ERROR               0
#define SIGN_MISSING_CERTIFICATE    1
#define SIGN_USER_CANCELED          2
#define SIGN_OTHER_ERROR            3
*/
        OSConnectorFree(strSignature);
        OSConnectorFree(strHashedKey);
        OSConnectorFree(strCertificate);
    }
#endif

    var.Param1 = objectId;
    var.Param2 = signId;
    var.Param3 = algorithm;
    pCHAR = 0;
    if (sigLen)
    {
        if (signature)
        {
            pCHAR = (CHAR *)OSConnectorAlloc(sigLen);
            if (!pCHAR)
                return;
            memcpy(pCHAR, signature, sigLen);
        }
        else
        {
            sigLen = 0;
        }
    }
    var.Param4 = sigLen;
    var.Param5 = pCHAR;
    pCHAR = 0;
    if (hashedKeyLen)
    {
        if (hashedKey)
        {
            pCHAR = (CHAR *)OSConnectorAlloc(hashedKeyLen);
            if (!pCHAR)
                return;
            memcpy(pCHAR, hashedKey, hashedKeyLen);
        }
        else
        {
            hashedKeyLen = 0;
        }
    }
    var.Param7 = hashedKeyLen;
    var.Param6 = pCHAR;
    pCHAR = 0;
    if (certificateLen)
    {
        if (certificate)
        {
            pCHAR = (CHAR *)OSConnectorAlloc(certificateLen);
            if (!pCHAR)
                return;
            memcpy(pCHAR, certificate, certificateLen);
        }
        else
        {
            certificateLen = 0;
        }
    }
    var.Param9 = certificateLen;
    var.Param8 = pCHAR;
    var.Param10 = certificateType;
    var.Param11 = err;

    XMK_SEND_ENV( ENV,
                  TextSigned,
                  xDefaultPrioSignal,
                  sizeof( yPDef_TextSigned ),
                  &var,
                  GLOBALPID(XPTID_WMLS_SM,0));
}
#endif
