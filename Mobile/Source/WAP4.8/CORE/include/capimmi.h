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
#ifndef CAPIMMI_H
#define CAPIMMI_H

#include "tapicmmn.h"
#include "tapimmi.h"


IMPORT_C VOID    MMIc_startUserAgent (UINT8 objectId, UINT8 uaMode);

IMPORT_C VOID    MMIc_terminateUserAgent (UINT8 objectId);

IMPORT_C VOID    MMIc_loadURL (UINT8 objectId, const CHAR* url, BOOL reload);

IMPORT_C VOID    MMIc_reload (UINT8 objectId);

IMPORT_C VOID    MMIc_stop (UINT8 objectId);

IMPORT_C VOID    MMIc_goBack (UINT8 objectId);

IMPORT_C VOID    MMIc_passwordDialogResponse (UINT8 objectId, UINT16 dialogId, const CHAR *name, const CHAR *password);

IMPORT_C VOID    MMIc_promptDialogResponse (UINT8 objectId, UINT8 dialogId, const WCHAR *answer);

IMPORT_C VOID    MMIc_confirmDialogResponse (UINT8 objectId, UINT8 dialogId, BOOL answer);

IMPORT_C VOID    MMIc_alertDialogResponse (UINT8 objectId, UINT8 dialogId);

IMPORT_C VOID    MMIc_keySelected (UINT8 objectId, UINT8 keyId);

IMPORT_C VOID    MMIc_textSelected (UINT8 objectId, UINT8 textId);

IMPORT_C VOID    MMIc_imageSelected (UINT8 objectId, UINT8 imageId);

IMPORT_C VOID    MMIc_optionSelected (UINT8 objectId, UINT8 optionId);

IMPORT_C VOID    MMIc_inputString (UINT8 objectId, UINT8 inputId, const WCHAR *text);

IMPORT_C VOID    MMIc_linkInfo (UINT8 objectId, UINT8 widgetType, UINT8 widgetId);

#ifdef CAN_SIGN_TEXT
IMPORT_C VOID    MMIc_textSigned (UINT8 objectId, UINT8 signId, UINT8 algorithm, const CHAR *signature, UINT16 sigLen, const CHAR *hashedKey, UINT16 hashedKeyLen, const CHAR *certificate, UINT16 certificateLen, UINT8 certificateType, UINT16 err);
#endif

#endif
