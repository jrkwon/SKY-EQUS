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
#ifndef AAPIMMI_H
#define AAPIMMI_H


#include "tapicmmn.h"


typedef INT8    AuthenticationType;

#define AUTH_SERVER (AuthenticationType)1
#define AUTH_PROXY  (AuthenticationType)2


VOID    MMIa_wait (UINT8 objectId, BOOL isWait);

VOID    MMIa_status (UINT8 objectId, UINT8 status, const CHAR *URL);

VOID    MMIa_unknownContent (UINT8 objectId, const CHAR *data, UINT16 length, const CHAR *contentType, const CHAR *URL);

VOID    MMIa_passwordDialog (UINT8 objectId, UINT16 dialogId, const CHAR *realm, INT8 type);

VOID    MMIa_promptDialog (UINT8 objectId, UINT8 dialogId, const WCHAR *message, const WCHAR *defaultInput);

VOID    MMIa_confirmDialog (UINT8 objectId, UINT8 dialogId, const WCHAR *message, const WCHAR *ok, const WCHAR *cancel);

VOID    MMIa_alertDialog (UINT8 objectId, UINT8 dialogId, const WCHAR *message);

VOID    MMIa_newCard (UINT8 objectId, const WCHAR *title, BOOL isList, BOOL isRefresh, const CHAR *URL, BOOL isBookmarkable, const WCHAR * const *history);

VOID    MMIa_showCard (UINT8 objectId);

VOID    MMIa_cancelCard (UINT8 objectId);

VOID    MMIa_newKey (UINT8 objectId, UINT8 keyId, const WCHAR *eventType, const WCHAR *label, BOOL isOptional);

VOID    MMIa_newText (UINT8 objectId, UINT8 textId, const WCHAR *text, BOOL isLink, const WCHAR *linkTitle, WCHAR accessKey, INT8 format);

VOID    MMIa_newImage (UINT8 objectId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, WCHAR accessKey, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align);

VOID    MMIa_completeImage (UINT8 objectId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType);

VOID    MMIa_setLanguage (UINT8 objectId, const CHAR *language);

VOID    MMIa_newParagraph (UINT8 objectId, INT8 align, BOOL wrap, BOOL preformatted);

VOID    MMIa_closeParagraph (UINT8 objectId);

VOID    MMIa_newBreak (UINT8 objectId);

VOID    MMIa_newTable (UINT8 objectId, const WCHAR *title, INT8 noOfColumns, const CHAR *align);

VOID    MMIa_newTableData (UINT8 objectId);

VOID    MMIa_closeTable (UINT8 objectId);

VOID    MMIa_newFieldSet (UINT8 objectId, const WCHAR *title);

VOID    MMIa_closeFieldSet (UINT8 objectId);

VOID    MMIa_newSelect (UINT8 objectId, const WCHAR *title, BOOL multiSelect, INT8 tabIndex);

VOID    MMIa_closeSelect (UINT8 objectId);

VOID    MMIa_newOption (UINT8 objectId, UINT8 optionId, const WCHAR *label, const WCHAR *title, BOOL isSelected);

VOID    MMIa_newOptionGroup (UINT8 objectId, const WCHAR *label);

VOID    MMIa_closeOptionGroup (UINT8 objectId);

VOID    MMIa_newInput (UINT8 objectId, UINT8 inputId, const WCHAR *title, const WCHAR *text, BOOL isPassword, BOOL emptyOk, const WCHAR *format, INT8 size, INT8 nChars, INT8 tabIndex, WCHAR accessKey);

VOID    MMIa_getInputString (UINT8 objectId, UINT8 inputId);

VOID    MMIa_linkInfo (UINT8 objectId, UINT8 widgetType , UINT8 widgetId, const CHAR *URL);

#ifdef CAN_SIGN_TEXT
VOID    MMIa_signText (UINT8 objectId, UINT8 signId, const WCHAR *text, const CHAR *data, UINT16 dataLen, UINT8 keyIdType, const CHAR *keyId, UINT16 keyIdLen, UINT8 options);
#endif

#endif
