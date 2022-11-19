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
#ifndef WML_MMI_H
#define WML_MMI_H

/*--------------------------------------------------------------
                       MMI create functions
----------------------------------------------------------------*/

void SDL_CreateDeck(void *pUserAgent);
void SDL_CreateCard(void* pUserAgent, void *pCard, BOOL fRefresh, void* pbURL, BOOL fNewContext);
void SDL_ClearCard(void* pUserAgent, BOOL fNewContext );
void SDL_CreateImage(void *pUserAgent, void *pElement, void* pData, int iDataLen, void* pbContentTypeAsString, int iImageID, void* pLinkElement);
void SDL_UpdateImage(void *pUserAgent, void *pImageElement, void* pData, int iDataLen, void* pchContentTypeAsString);
void SDL_CreateInput(void *pUser, void* pInputElement, int iInputId);
void SDL_CreateSelect(void *pUserAgent, void* pSelectElement);
void SDL_CloseSelect(void *pUser);
void SDL_CreateKey(void *pUser,void *pDoElement, int iDoRef);
void SDL_CreateOptGrp(void *pUserAgent, void* pOptGrpElement);
void SDL_CloseOptGrp(void *pUser);
void SDL_CreateOption(void* pUserAgent, void* pOptElement, void* pchLabelText, int iOptionID);
void SDL_CreateField(void *pUserAgent,void *pFieldElement);
void SDL_CloseField(void* pUser);
void SDL_CreateTab(void *pUserAgent,void *pTabElement);
void SDL_CreateBR(void *pUserAgent);
BOOL SDL_CreateParagraph(void *pUser,void *pElement);
void SDL_CloseParagraph(void *pUserAgent);
void SDL_CreateTable(void* pUserAgent, void* pTableElement);
void SDL_CloseTable(void* pUserAgent);
void SDL_CreateTableCell(void* pUserAgent);

void SDL_ShowCard(void *pUser);
void SDL_CancelCard(void *pUser);

BOOL SDL_CreateText(void *pUserAgent,void* pTextElement, int iTextID, int iEmphMode, void* pLinkElement, int iWhiteSpace, int fPreformatted );
int SDL_UpdateEmph(void* pEmphElement, BYTE* pEmphMode, BOOL fModeOn);
int SDL_CheckDoDuplicates(void *pUser, void *pDoElement, void* pDoList);
void* SDL_UpdateOption(void *pUserAgent, void* pOptionList, int OptionID);

#endif
