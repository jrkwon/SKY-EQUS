#include <string.h>
#if !(defined WAP_B1 || defined WAP_B2)
#include "ui.h"
#endif //WAP_B1
#include "err.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Below adapter functions are called directly from WAP task without sending mail command to UI task
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "waptask.h"
#include "wapconnector.h"

//extern boolean load_incard;
extern INT16 w_strlen(const WCHAR* str); /* wcslen */
extern WCHAR* w_strcpy(WCHAR* dst, const WCHAR* src); /* wcscpy */

void    MMIa_wait (unsigned char viewId, char isWait)
{
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
	//SKY_wait(viewId, isWait);
}

#ifdef SOCKET_AUWAP
extern void wait_ani_start(int type, int time);
extern void wait_ani_stop(void);
extern boolean load_incard;

void    MMIa_status (unsigned char viewId, unsigned char status, const char *URL)
{
	ui_cmd_type   *ui_cmd_ptr; 

	{
		extern print_msg(int a,int b);
		print_msg( status,0);
	}

// AUWAP
	if( status == 1 || status == 3 ) {
		wait_ani_start( 1, 150 );
	} 
	if( status == 2 || status == 4 ) {
		wait_ani_stop();
	}// AUWAP

	if(status == 8)//read from cache
	{
		load_incard = TRUE;
		if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
		{	
			//ERR_FATAL( "No UI cmd buffers",0,0,0);
		}
		else		
		{
			ui_cmd_ptr->hdr.cmd        = UI_WAP_MMI_WAIT_F;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
		}
	}

	if(status == 11)//Received from Network
	{
		load_incard = TRUE;
		if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
		{	
			//ERR_FATAL( "No UI cmd buffers",0,0,0);
		}
		else		
		{
		/*===================================================
					  NT로 부터 데이타 수신 (2000.03.08)				
		 ===================================================*/
			ui_cmd_ptr->hdr.cmd        = UI_WAP_RCV_FROM_NT_F;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
			CLNTa_log(1,0,"NT data 수신");
		}
	}

	SKY_status(viewId, status, URL);
}

#else //SOCKET_AUWAP

void    MMIa_status (unsigned char viewId, unsigned char status, const char *URL)
{
	SKY_status(viewId, status, URL);
}
#endif //SOCKET_AUWAP

void    MMIa_unknownContent (unsigned char viewId, const char *data, unsigned short length, const char *contentType, const char *URL)
{
	SKY_unknownContent(viewId, data, length, contentType, URL);
}


void    MMIa_passwordDialog (unsigned char viewId, unsigned char dialogId, const char *realm, signed char type)
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

#ifdef SOCKET_AUWAP
void    MMIa_newCard (unsigned char viewId, const unsigned short *title, char isList, char isRefresh, const char *URL, char isBookmarkable, const unsigned short * const *history)
{
	load_incard = TRUE;
	{
		extern print_msg(int a,int b);
		print_msg( 0,2);
	}
	SKY_newCard(viewId, (CHAR *)title, isList, isRefresh, (CHAR *)URL, isBookmarkable, history);
}


void    MMIa_showCard (unsigned char viewId)
{
	{
		extern print_msg(int a,int b);
		print_msg( 0,3);
	}
	SKY_showCard(viewId);
}

#else //SOCKET_AUWAP

void    MMIa_newCard (unsigned char viewId, const unsigned short *title, char isList, char isRefresh, const char *URL, char isBookmarkable, const unsigned short * const *history)
{
	//load_incard = TRUE;
	//SKY_newCard(viewId, title, isList, isRefresh, URL, isBookmarkable, history);
	wap_cmd_type* wap_ptr;

	signed short	lenOfTitle;
	char	*pTitle;

	signed short	lenOfURL;
	char			*pURL;

	if (( wap_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
	{
		wap_ptr->hdr.task_ptr   = NULL;
		wap_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
	}
	else {
		ERR_FATAL( "MMIa_newCard: Free queue is EMPTY!", 0, 0, 0 );
	}
	
	wap_ptr->hdr.cmd = WAP_NEW_CARD;
	wap_ptr->mmi_new.viewId = viewId;
	

	if(title == NULL)
	{
		pTitle = NULL;
	}
	else
	{
		lenOfTitle = KSCStrLenOfUni((WCHAR *)title);
#ifdef WAP_USE_LEAK
		pTitle = (char *)WIPAlloc(lenOfTitle+1);
		if(!pTitle) return;
#else //#ifdef WAP_USE_LEAK
		pTitle = (char *)wip_malloc(lenOfTitle+1);
#endif //#ifdef WAP_USE_LEAK
		memset(pTitle,0x00,lenOfTitle+1);
		Uni2KSCString((WCHAR *)title, (unsigned char *)pTitle);
	}
	wap_ptr->mmi_new.title = pTitle;

	//isList
	wap_ptr->mmi_new.isList = isList;

	//isRefresh
	wap_ptr->mmi_new.isRefresh = isRefresh;
	
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
	wap_ptr->mmi_new.url = pURL;

	//Bookmark
	wap_ptr->mmi_new.isBookmarkable = isBookmarkable;

	//history --> 사용하지 않는다.
/*
	if(history == NULL)
	{
		pHistory = NULL;
	}
	else
	{
		lenOfHistory = w_strlen((unsigned short*)history);
		pHistory = (unsigned short*)wip_malloc(sizeof(unsigned short) * (lenOfHistory+1));
		memset(pHistory,0x00,sizeof(unsigned short) * (lenOfHistory+1));
		w_strcpy(pHistory, (unsigned short*)history);
	}
	wap_ptr->mmi_new.history = pHistory;
*/
	wap_ptr->mmi_new.history = NULL;
	send_cmd_other2wap(wap_ptr);

//	SKY_newCard(wap_ptr->mmi_new.viewId,wap_ptr->mmi_new.title,wap_ptr->mmi_new.isList,wap_ptr->mmi_new.isRefresh,wap_ptr->mmi_new.url,
//					wap_ptr->mmi_new.isBookmarkable,wap_ptr->mmi_new.history);
}


void    MMIa_showCard (unsigned char viewId)
{

	wap_cmd_type* wap_ptr;

	if (( wap_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
	{
		wap_ptr->hdr.task_ptr   = NULL;
		wap_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
	}
	else {
		ERR_FATAL( "MMIa_showCard: Free queue is EMPTY!", 0, 0, 0 );
	}

	wap_ptr->hdr.cmd = WAP_SHOW_CARD;
	wap_ptr->mmi_show.viewId = viewId;
	send_cmd_other2wap(wap_ptr);

//	extern void send_connect2ui(void);
//	SKY_showCard(viewId);
//	send_connect2ui();
}
#endif //SOCKET_AUWAP

void    MMIa_cancelCard (unsigned char viewId)
{
	SKY_cancelCard(viewId);
}


void    MMIa_newKey (unsigned char viewId, unsigned char keyId, const unsigned short *eventType, const unsigned short *label, char isOptional)
{
	SKY_newKey(viewId, keyId, eventType, label, isOptional);
}



void    MMIa_newText(unsigned char viewId, unsigned char textId, const unsigned short *text, char isLink, const unsigned short *linkTitle, signed char format)
{
	SKY_newText(viewId, textId, text, isLink, linkTitle, format);
}


void    MMIa_newImage (unsigned char viewId, unsigned char imageId, const char *imageData, unsigned short imageSize, const char *imageType,
					   const unsigned short *altText, const unsigned short *localSrc, char isLink, const unsigned short *linkTitle,
					   signed char vSpace, signed char hSpace, signed short width, signed short height, signed char isPercent, signed char align)
{
	SKY_newImage(viewId, imageId, imageData, imageSize,	imageType,
				 altText, localSrc, isLink,	linkTitle, 
				 vSpace, hSpace, width,	height,	isPercent, align);
}


void    MMIa_completeImage (unsigned char viewId, unsigned char imageId, const char *imageData, unsigned short imageSize, const char *imageType)
{
	SKY_completeImage(viewId, imageId, imageData, imageSize, imageType);
}



void    MMIa_newParagraph (unsigned char viewId, signed char align, char wrap)
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


void    MMIa_newInput (unsigned char viewId, unsigned char inputId, const unsigned short *title, const unsigned short *text, char isPassword, char emptyOk, const unsigned short *format, signed char size, signed char nchars, signed char tabIndex)
{
	SKY_newInput(viewId, inputId, title, text, isPassword, emptyOk, format, size, nchars, tabIndex);
}

void    MMIa_getInputString (unsigned char viewId, unsigned char inputId)
{
	SKY_getInputString(viewId, inputId);
}


void    MMIa_linkInfo (unsigned char viewId, unsigned char widgetType , unsigned char widgetId, const char *URL)
{
	SKY_linkInfo(viewId, widgetType, widgetId, URL);
}
