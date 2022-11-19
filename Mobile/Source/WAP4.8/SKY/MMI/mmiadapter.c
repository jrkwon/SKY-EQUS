#ifndef FEATURE_WAP
#include <string.h>

#include "err.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Below adapter functions are called directly from WAP task without sending mail command to UI task
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "wapconnector.h"
#include "aapimmi.h"

//extern boolean load_incard;
extern INT16 w_strlen(const WCHAR* str); /* wcslen */
extern WCHAR* w_strcpy(WCHAR* dst, const WCHAR* src); /* wcscpy */

void    MMIa_wait (unsigned char viewId, char isWait)
{
#ifndef FEATURE_WAP
	wap_cmd_type* wap_ptr;

	if (( wap_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
	{
		wap_ptr->hdr.task_ptr   = NULL;
		wap_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
	}
	else {
		ERR_FATAL( "MMIa_wait: Free queue is EMPTY!", 0, 0, 0 );
	}

	wap_ptr->hdr.cmd = WAP_MMI_WAIT;
	wap_ptr->mmi_wait.viewId = viewId;
	wap_ptr->mmi_wait.isWait = isWait;
	send_cmd_other2wap(wap_ptr);    
#endif //#ifndef FEATURE_WAP
	SKY_wait(viewId, isWait);
}

void    MMIa_status (unsigned char viewId, unsigned char status, const char *URL)
{
    CLNTa_log(1,0,"status %d",status);
	SKY_status(viewId, status, URL);
}

void    MMIa_unknownContent (unsigned char viewId, const char *data, unsigned short length, const char *contentType, const char *URL)
{
	SKY_unknownContent(viewId, data, length, contentType, URL);
}

#ifdef FEATURE_WAP
VOID    MMIa_passwordDialog (UINT8 viewId, UINT16 dialogId, const CHAR *realm, INT8 type)
#else
void    MMIa_passwordDialog (unsigned char viewId, unsigned char dialogId, const char *realm, signed char type)
#endif //FEATURE_WAP
{
	SKY_passwordDialog(viewId, dialogId, realm, type);
}


void    MMIa_promptDialog (unsigned char viewId, unsigned char dialogId, const unsigned short *message, const unsigned short *defaultMessage)
{
	SKY_promptDialog(viewId, dialogId, message, defaultMessage);
}


void    MMIa_confirmDialog (unsigned char viewId, unsigned char dialogId, const unsigned short *message, const unsigned short *ok, const unsigned short *cancel)
{
	SKY_confirmDialog(viewId, dialogId, message, ok, cancel);
}


void    MMIa_alertDialog (unsigned char viewId, unsigned char dialogId, const unsigned short *message)
{
	SKY_alertDialog(viewId, dialogId, message);
}

void    MMIa_newCard (unsigned char viewId, const unsigned short *title, char isList, char isRefresh, const char *URL, char isBookmarkable, const unsigned short * const *history)
{
	signed short	lenOfTitle;
	char	*pTitle;

	signed short	lenOfURL;
	char			*pURL;

	if(title == NULL)
	{
		pTitle = NULL;
	}
	else
	{
		lenOfTitle = KSCStrLenOfUni((WCHAR *)title);
		pTitle = (char *)wip_malloc(lenOfTitle+1);
		memset(pTitle,0x00,lenOfTitle+1);
		Uni2KSCString((WCHAR *)title, (unsigned char *)pTitle);
	}
		
	//URL
	if(URL == NULL)
	{
		pURL = NULL;
	}
	else
	{
		extern char tempurlbuffer[];

		lenOfURL = strlen(URL);
		pURL = (char *)wip_malloc(lenOfURL+1);
		memset(pURL,0x00,lenOfURL+1);
		strcpy(pURL, URL);

		memset(tempurlbuffer,0x00,200);
		if(lenOfURL <200) 
		{
			strncpy(tempurlbuffer,URL,lenOfURL);
		}
		else
		{
			strncpy(tempurlbuffer,URL,200);
		}
	}

	//Bookmark
	//wap_ptr->mmi_new.history = NULL;
	SKY_newCard(viewId,pTitle,isList,isRefresh,pURL,
				isBookmarkable,NULL);
}


void    MMIa_showCard (unsigned char viewId)
{
	SKY_showCard(viewId);
}

void    MMIa_cancelCard (unsigned char viewId)
{
	SKY_cancelCard(viewId);
}


void    MMIa_newKey (unsigned char viewId, unsigned char keyId, const unsigned short *eventType, const unsigned short *label, char isOptional)
{
	SKY_newKey(viewId, keyId, eventType, label, isOptional);
}

#ifdef FEATURE_WAP
VOID    MMIa_newText (UINT8 viewId, UINT8 textId, const WCHAR *text, BOOL isLink, const WCHAR *linkTitle, WCHAR accessKey, INT8 format)
#else
void    MMIa_newText(unsigned char viewId, unsigned char textId, const unsigned short *text, char isLink, const unsigned short *linkTitle, signed char format)
#endif //FEATURE_WAP
{
	SKY_newText(viewId, textId, text, isLink, linkTitle, format);
}

#ifdef FEATURE_WAP
VOID    MMIa_newImage (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, WCHAR accessKey, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align)
#else
void    MMIa_newImage (unsigned char viewId, unsigned char imageId, const char *imageData, unsigned short imageSize, const char *imageType,
					   const unsigned short *altText, const unsigned short *localSrc, char isLink, const unsigned short *linkTitle,
					   signed char vSpace, signed char hSpace, signed short width, signed short height, signed char isPercent, signed char align)
#endif //FEATURE_WAP
{
	SKY_newImage(viewId, imageId, imageData, imageSize,	imageType,
				 altText, localSrc, isLink,	linkTitle, 
				 vSpace, hSpace, width,	height,	isPercent, align);
}


void    MMIa_completeImage (unsigned char viewId, unsigned char imageId, const char *imageData, unsigned short imageSize, const char *imageType)
{
	SKY_completeImage(viewId, imageId, imageData, imageSize, imageType);
}


#ifdef FEATURE_WAP
VOID    MMIa_newParagraph (UINT8 viewId, INT8 align, BOOL wrap, BOOL preformatted)
#else
void    MMIa_newParagraph (unsigned char viewId, signed char align, char wrap)
#endif //FEATURE_WAP
{
	SKY_newParagraph(viewId, align, wrap);
}


void    MMIa_closeParagraph (unsigned char viewId)
{
	SKY_closeParagraph(viewId);
}


void    MMIa_newBreak (unsigned char viewId)
{
	SKY_newBreak(viewId);
}


void    MMIa_newTable (unsigned char viewId, const unsigned short *title, signed char noOfColumns, const char *align)
{
	SKY_newTable(viewId, title, noOfColumns, align);
}


void    MMIa_newTableData (unsigned char viewId)
{
	SKY_newTableData(viewId);
}


void    MMIa_closeTable (unsigned char viewId)
{
	SKY_closeTable(viewId);
}


void    MMIa_newFieldSet (unsigned char viewId, const unsigned short *title)
{
	SKY_newFieldSet(viewId, title);
}


void    MMIa_closeFieldSet (unsigned char viewId)
{
	SKY_closeFieldSet(viewId);
}


void    MMIa_newSelect (unsigned char viewId, const unsigned short *title, char multiSelect, signed char tabIndex)
{
	SKY_newSelect(viewId, title, multiSelect, tabIndex);
}


void    MMIa_closeSelect (unsigned char viewId)
{
	SKY_closeSelect(viewId);
}


void    MMIa_newOption (unsigned char viewId, unsigned char optionId, const unsigned short *label, const unsigned short *title, char isSelected)
{
	SKY_newOption(viewId, optionId, label, title, isSelected);
}


void    MMIa_newOptionGroup (unsigned char viewId, const unsigned short *label)
{
	SKY_newOptionGroup(viewId, label);
}


void    MMIa_closeOptionGroup (unsigned char viewId)
{
	SKY_closeOptionGroup(viewId);
}

#ifdef FEATURE_WAP
VOID    MMIa_newInput (UINT8 viewId, UINT8 inputId, const WCHAR *title, const WCHAR *text, BOOL isPassword, BOOL emptyOk, const WCHAR *format, INT8 size, INT8 nChars, INT8 tabIndex, WCHAR accessKey)
#else
void    MMIa_newInput (unsigned char viewId, unsigned char inputId, const unsigned short *title, const unsigned short *text, char isPassword, char emptyOk, const unsigned short *format, signed char size, signed char nchars, signed char tabIndex)
#endif //FEATURE_WAP
{
#ifndef FEATURE_WAP
	SKY_newInput(viewId, inputId, title, text, isPassword, emptyOk, format, size, nchars, tabIndex);
#endif //FEATURE_WAP
}

void    MMIa_getInputString (unsigned char viewId, unsigned char inputId)
{
	SKY_getInputString(viewId, inputId);
}


void    MMIa_linkInfo (unsigned char viewId, unsigned char widgetType , unsigned char widgetId, const char *URL)
{
	SKY_linkInfo(viewId, widgetType, widgetId, URL);
}

#ifdef FEATURE_WAP
VOID    MMIa_setLanguage (UINT8 viewId, const CHAR *language)
{
    CLNTa_log(1,0,"DUMMY MMIa_setLanguage");
}
#endif //FEATURE_WAP
#endif  //#ifndef FEATURE_WAP