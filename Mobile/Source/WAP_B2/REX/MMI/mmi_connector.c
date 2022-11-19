#include <string.h>

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"
//#include "wapadapter.h"
#include "wapconnector.h"

//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
//#include "custtrex.h"
//#include "custA7.h"
#include "customer.h" // jwpark 00.04.27


//-----------------------------------------------------------------------------------
//
// MMI ¿« Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
//#include "mmi_data.h"
#include "mmi_func.h"

extern BOOL g_bBlocked;

#ifdef FOR_MS
#ifndef WAPCORE_FOR_WARNING
static BOOL checkWTLS(UINT8 viewId, UINT8 widgetType, UINT8 widgetId);
#endif //#ifndef WAPCORE_FOR_WARNING

#ifdef PORTING_B0_WAP
extern void wap_wait(void);
#endif //PORTING_B0_WAP

extern int  wtls_match;
extern BOOL Wtlshello ; 
extern BOOL wtls_enc_start;
extern int  nLinkType;
extern int  nLinkId  ;
#endif//#ifdef FOR_MS


//------------------------------------------------------------------
extern void make_cmd_other2wap( wap_cmd_type *msg_ptr );
//------------------------------------------------------------------

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
/*																									*/
/*									SKY    M M I   Connector Functions								*/
/*																									*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/

#ifdef SOCKET_AUWAP
//////////////// AUWAP /////////////////////
////////////////////////////////////////////

void SKY_openView (UINT8 viewId,UINT8 uamode)
{
	wap_cmd_type	wap_ptr;
	extern print_msg(int a,int b);
	print_msg( 2,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
	MMIc_openView( viewId, uamode);
}

void SKY_closeView (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;
		extern print_msg(int a,int b);
	print_msg( 3,4);
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

	MMIc_closeView(viewId);
}

void SKY_loadURL (UINT8 viewId, const CHAR* url, BOOL reload)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 4,4);

	MMIc_loadURL( viewId, url, reload);
}

void SKY_reload (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;
	
	extern print_msg(int a,int b);
	print_msg( 5,4);
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
	MMIc_reload(viewId); 
}

void SKY_stop (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 6,4);	

	MMIc_stop(viewId);
}

void SKY_goBack (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;
	extern print_msg(int a,int b);
	print_msg( 6,4);
	
	MMIc_goBack(viewId); 
}

void SKY_passwordDialogResponse (UINT8 viewId, UINT8 dialogId, const CHAR *name, const CHAR *password)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 7,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************
	
	MMIc_passwordDialogResponse(viewId, dialogId, name, password);  
}

void SKY_promptDialogResponse (UINT8 viewId, UINT8 dialogId, const WCHAR *answer)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 8,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************
	
	MMIc_promptDialogResponse(viewId, dialogId,  answer);
}

void SKY_confirmDialogResponse (UINT8 viewId, UINT8 dialogId, BOOL answer)
{
	wap_cmd_type	wap_ptr;
	
	extern print_msg(int a,int b);
	print_msg( 9,4);
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************
		
	MMIc_confirmDialogResponse( viewId, dialogId, answer);
}

void SKY_alertDialogResponse (UINT8 viewId, UINT8 dialogId)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 10,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************

	MMIc_alertDialogResponse( viewId, dialogId);
}

void SKY_keySelected(UINT8 viewId, UINT8 keyId)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 11,4);
	//openView (viewId+1);
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

	MMIc_keySelected( viewId, keyId); //AUWAP
}

void SKY_textSelected (UINT8 viewId, UINT8 textId)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 12,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

	MMIc_textSelected( viewId, textId); // AUWAP
}

void SKY_imageSelected (UINT8 viewId, UINT8 imageId)
{
	wap_cmd_type	wap_ptr;
	extern print_msg(int a,int b);
	print_msg( 13,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
	
	MMIc_imageSelected( viewId, imageId);
}

void SKY_optionSelected (UINT8 viewId, UINT8 optionId)
{
	wap_cmd_type	wap_ptr;

	extern print_msg(int a,int b);
	print_msg( 14,4);
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

	MMIc_optionSelected( viewId, optionId );

}

void SKY_inputString (UINT8 viewId, UINT8 inputId, const WCHAR *text)
{
	wap_cmd_type	wap_ptr;
	extern print_msg(int a,int b);
	print_msg( 15,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

	MMIc_inputString(viewId, inputId, text);
}

void SKY_linkInfo_c (UINT8 viewId, UINT8 widgetType, UINT8 widgetId)
{
	wap_cmd_type	wap_ptr;
	extern print_msg(int a,int b);
	print_msg( 16,4);	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
	
	MMIc_linkInfo( viewId, widgetType, widgetId); // AUWAP
}

#else //SOCKET_AUWAP

void SKY_openView (UINT8 viewId,UINT8 uamode)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

	wap_ptr.hdr.cmd				= MMI_OPEN_VIEW;
	wap_ptr.open_view.viewId	= viewId;
	wap_ptr.open_view.uamode    = uamode;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_closeView (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

	wap_ptr.hdr.cmd				= MMI_CLOSE_VIEW;
	wap_ptr.close_view.viewId	= viewId;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_loadURL (UINT8 viewId, const CHAR* url, BOOL reload)
{
	wap_cmd_type	wap_ptr;
	
	wap_ptr.hdr.cmd			= MMI_LOAD_URL;
	wap_ptr.load_url.viewId	= viewId;
	wap_ptr.load_url.url	= url;

#ifdef BLOCK
	if(isReadFromCache && isErrorHappened && !strncmp(url, status_url, strlen(status_url)))
	{
		wap_ptr.load_url.reload	= TRUE;
		isReadFromCache = FALSE;
		isErrorHappened = FALSE;
		CLNTa_log(0,0,"\nø©±‚ø° µÈæÓø‘¡ˆ∑’ ¿∏»Â»Â»Â»Â»Â»Â");
	}
#endif //#ifdef BLOCK

	wap_ptr.load_url.reload	= reload;
	make_cmd_other2wap( &wap_ptr );
}

void SKY_reload (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
		
	wap_ptr.hdr.cmd			= MMI_RELOAD;
	wap_ptr.reload.viewId	= viewId;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_stop (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;
	
	wap_ptr.hdr.cmd			= MMI_STOP;
	wap_ptr.stop.viewId		= viewId;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_goBack (UINT8 viewId)
{
	wap_cmd_type	wap_ptr;
	
	wap_ptr.hdr.cmd			= MMI_GO_BACK;
	wap_ptr.go_back.viewId	= viewId;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_passwordDialogResponse (UINT8 viewId, UINT8 dialogId, const CHAR *name, const CHAR *password)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************
		
	wap_ptr.hdr.cmd				= MMI_PASSWORD_DIALOG_RESPONSE;
	wap_ptr.password.viewId		= viewId;
	wap_ptr.password.dialogId	= dialogId;
	wap_ptr.password.name		= name;
	wap_ptr.password.password	= password;
	CLNTa_log(1, 0, "SKY_passwordDialogResponse");
	make_cmd_other2wap( &wap_ptr );
}

void SKY_promptDialogResponse (UINT8 viewId, UINT8 dialogId, const WCHAR *answer)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************
		
	wap_ptr.hdr.cmd			= MMI_PROMPT_DIALOG_RESPONSE;
	wap_ptr.prompt.viewId	= viewId;
	wap_ptr.prompt.dialogId	= dialogId;
	wap_ptr.prompt.answer	= answer;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_confirmDialogResponse (UINT8 viewId, UINT8 dialogId, BOOL answer)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************
		
	wap_ptr.hdr.cmd				= MMI_CONFIRM_DIALOG_RESPONSE;
	wap_ptr.confirm.viewId		= viewId;
	wap_ptr.confirm.dialogId	= dialogId;
	wap_ptr.confirm.answer		= answer;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_alertDialogResponse (UINT8 viewId, UINT8 dialogId)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//************************************************
		
	wap_ptr.hdr.cmd			= MMI_ALERT_DIALOG_RESPONSE;
	wap_ptr.alert.viewId	= viewId;
	wap_ptr.alert.dialogId	= dialogId;
	
	make_cmd_other2wap( &wap_ptr );
}

void SKY_keySelected(UINT8 viewId, UINT8 keyId)
{
	wap_cmd_type	wap_ptr;

	//openView (viewId+1);
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************


#ifndef WAP_USE_SECURE
	//WAP WTLS∏¶ ¿ß«— ∑Á∆æ¿”...jjkyg..12/6
	if(wtls_enc_start)
	{
		Wtlshello = FALSE;
	}
	else
	{
		Wtlshello = checkWTLS(viewId, 3, keyId);
	}

	if(Wtlshello==TRUE) 
	{
		return;
	}
#endif//#ifndef WAP_USE_SECURE


	wap_ptr.hdr.cmd			= MMI_KEY_SELECTED;
	wap_ptr.key.viewId		= viewId;
	wap_ptr.key.keyId		= keyId;

	make_cmd_other2wap( &wap_ptr );
}

void SKY_textSelected (UINT8 viewId, UINT8 textId)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************

#ifndef WAP_USE_SECURE
	//WAP WTLS∏¶ ¿ß«— ∑Á∆æ¿”...jjkyg..12/6
	if(wtls_enc_start)
		Wtlshello = FALSE;
	else
	{
		#ifdef debug
			CLNTa_log(1, 0, "text 9090wtls");
		#endif//#ifdef debug
		Wtlshello=checkWTLS(viewId, 2, textId);
	}

	if(Wtlshello ==TRUE)
	{
		return;
	}
#endif//#ifndef WAP_USE_SECURE

	
	wap_ptr.hdr.cmd			= MMI_TEXT_SELECTED;
	wap_ptr.text.viewId		= viewId;
	wap_ptr.text.textId		= textId;

	make_cmd_other2wap( &wap_ptr );
}

void SKY_imageSelected (UINT8 viewId, UINT8 imageId)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
	
	wap_ptr.hdr.cmd			= MMI_IMAGE_SELECTED;
	wap_ptr.image.viewId	= viewId;
	wap_ptr.image.imageId	= imageId;

	make_cmd_other2wap( &wap_ptr );
}

void SKY_optionSelected (UINT8 viewId, UINT8 optionId)
{
	wap_cmd_type	wap_ptr;

	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************


#ifndef WAP_USE_SECURE
	//WAP WTLS∏¶ ¿ß«— ∑Á∆æ¿”...jjkyg..12/6
	if(wtls_enc_start)
	{
		Wtlshello = FALSE;
	}
	else
	{
		Wtlshello=checkWTLS(viewId, 4, optionId);
	}
	
	if(Wtlshello==TRUE)
	{
		return;
	}
#endif//#ifndef WAP_USE_SECURE
	wap_ptr.hdr.cmd			= MMI_OPTION_SELECTED;
	wap_ptr.option. viewId	= viewId;
	wap_ptr.option.optionId	= optionId;

	make_cmd_other2wap( &wap_ptr );
}

void SKY_inputString (UINT8 viewId, UINT8 inputId, const WCHAR *text)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
	
	wap_ptr.hdr.cmd			= MMI_INPUT_STRING;
	wap_ptr.input.viewId	= viewId;
	wap_ptr.input.inputId	= inputId;
	wap_ptr.input.text		= text;

	make_cmd_other2wap( &wap_ptr );
}

void SKY_linkInfo_c (UINT8 viewId, UINT8 widgetType, UINT8 widgetId)
{
	wap_cmd_type	wap_ptr;
	
	//************************************************
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//************************************************
	
	wap_ptr.hdr.cmd				= MMI_LINKINFO;
	wap_ptr.linkinfo.viewId		= viewId;
	wap_ptr.linkinfo.widgetType	= widgetType;
	wap_ptr.linkinfo.widgetId	= widgetId;

	make_cmd_other2wap( &wap_ptr );
}

#endif //SOCKET_AUWAP
//jjkyg 12/6

#ifdef FOR_MS
#ifndef WAPCORE_FOR_WARNING
static BOOL checkWTLS(UINT8 viewId, UINT8 widgetType, UINT8 widgetId)
{
	nLinkType = widgetType;
	nLinkId   = widgetId;

	SKY_linkInfo_c(viewId,widgetType,widgetId);
#ifdef PORTING_B0_WAP
	wap_wait();
#endif //PORTING_B0_WAP
	 
	//CLNTa_log(1,0,"WAP WAIT PASS");
	if(wtls_match !=0)
	{
		return TRUE;

	}
	return FALSE;
}
#endif //#ifndef WAPCORE_FOR_WARNING
#endif//#ifdef FOR_MS
