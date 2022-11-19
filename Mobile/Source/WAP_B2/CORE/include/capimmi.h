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


IMPORT_C VOID    MMIc_openView (UINT8 viewId, UINT8 uaMode);

IMPORT_C VOID    MMIc_closeView (UINT8 viewId);

IMPORT_C VOID    MMIc_loadURL (UINT8 viewId, const CHAR* url, BOOL reload);

IMPORT_C VOID    MMIc_reload (UINT8 viewId);

IMPORT_C VOID    MMIc_stop (UINT8 viewId);

IMPORT_C VOID    MMIc_goBack (UINT8 viewId);

IMPORT_C VOID    MMIc_passwordDialogResponse (UINT8 viewId, UINT8 dialogId, const CHAR *name, const CHAR *password);

IMPORT_C VOID    MMIc_promptDialogResponse (UINT8 viewId, UINT8 dialogId, const WCHAR *answer);

IMPORT_C VOID    MMIc_confirmDialogResponse (UINT8 viewId, UINT8 dialogId, BOOL answer);

IMPORT_C VOID    MMIc_alertDialogResponse (UINT8 viewId, UINT8 dialogId);

IMPORT_C VOID    MMIc_keySelected (UINT8 viewId, UINT8 keyId);

IMPORT_C VOID    MMIc_textSelected (UINT8 viewId, UINT8 textId);

IMPORT_C VOID    MMIc_imageSelected (UINT8 viewId, UINT8 imageId);

IMPORT_C VOID    MMIc_optionSelected (UINT8 viewId, UINT8 optionId);

IMPORT_C VOID    MMIc_inputString (UINT8 viewId, UINT8 inputId, const WCHAR *text);

IMPORT_C VOID    MMIc_linkInfo (UINT8 viewId, UINT8 widgetType, UINT8 widgetId);

#endif
