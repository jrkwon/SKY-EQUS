#include <string.h>
#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"
#include "wapconnector.h"
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

/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
/*																									*/
/*									SKY    M M I   Connector Functions								*/
/*																									*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
void SKY_startUserAgent (UINT8 viewId,UINT8 uamode)
{
/*	wap_cmd_type	wap_ptr;
	
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

	wap_ptr.hdr.cmd				= MMI_OPEN_VIEW;
	wap_ptr.open_view.viewId	= viewId;
	wap_ptr.open_view.uamode    = uamode;
	
	make_cmd_other2wap( &wap_ptr );
*/
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

    MMIc_startUserAgent(viewId, uamode);
    ProcessEvents();
}

void SKY_terminateUserAgent (UINT8 viewId)
{
/*
	wap_cmd_type	wap_ptr;
	
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

	wap_ptr.hdr.cmd				= MMI_CLOSE_VIEW;
	wap_ptr.close_view.viewId	= viewId;
	
	make_cmd_other2wap( &wap_ptr );
*/
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

    MMIc_terminateUserAgent(viewId);
    ProcessEvents();
}

void SKY_loadURL (UINT8 viewId, const CHAR* url, BOOL reload)
{
/*	wap_cmd_type	wap_ptr;
	
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
    */
    MMIc_loadURL(viewId, url, reload);
    ProcessEvents();
}

void SKY_reload (UINT8 viewId)
{
    /*
	wap_cmd_type	wap_ptr;
	
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------
		
	wap_ptr.hdr.cmd			= MMI_RELOAD;
	wap_ptr.reload.viewId	= viewId;
	
	make_cmd_other2wap( &wap_ptr );
    */
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

    MMIc_reload(viewId);
    ProcessEvents();
}

void SKY_stop (UINT8 viewId)
{
    /*
	wap_cmd_type	wap_ptr;
	
	wap_ptr.hdr.cmd			= MMI_STOP;
	wap_ptr.stop.viewId		= viewId;
	
	make_cmd_other2wap( &wap_ptr );
    */
    //if(load_data && !Wtlshello) 
    {
          CLNTa_log(1,0," MMI_STOP in wapcmd");
          MMIc_stop(1);           
          ProcessEvents();
	}
}

void SKY_goBack (UINT8 viewId)
{
    /*
	wap_cmd_type	wap_ptr;
	
	wap_ptr.hdr.cmd			= MMI_GO_BACK;
	wap_ptr.go_back.viewId	= viewId;
	
	make_cmd_other2wap( &wap_ptr );
    */
    MMIc_goBack(viewId);
    ProcessEvents();
}

void SKY_passwordDialogResponse (UINT8 viewId, UINT8 dialogId, const CHAR *name, const CHAR *password)
{
    /*
	wap_cmd_type	wap_ptr;
	
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//---------------------------------------------
		
	wap_ptr.hdr.cmd				= MMI_PASSWORD_DIALOG_RESPONSE;
	wap_ptr.password.viewId		= viewId;
	wap_ptr.password.dialogId	= dialogId;
	wap_ptr.password.name		= name;
	wap_ptr.password.password	= password;
	CLNTa_log(1, 0, "SKY_passwordDialogResponse");
	make_cmd_other2wap( &wap_ptr );
    */

    MMIc_passwordDialogResponse(viewId, dialogId, name, password);
    ProcessEvents();
}

void SKY_promptDialogResponse (UINT8 viewId, UINT8 dialogId, const WCHAR *answer)
{
    /*
	wap_cmd_type	wap_ptr;
	
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//---------------------------------------------
		
	wap_ptr.hdr.cmd			= MMI_PROMPT_DIALOG_RESPONSE;
	wap_ptr.prompt.viewId	= viewId;
	wap_ptr.prompt.dialogId	= dialogId;
	wap_ptr.prompt.answer	= answer;
	
	make_cmd_other2wap( &wap_ptr );
    */
    MMIc_promptDialogResponse(viewId, dialogId, answer);
    ProcessEvents();
}

void SKY_confirmDialogResponse (UINT8 viewId, UINT8 dialogId, BOOL answer)
{
    /*
	wap_cmd_type	wap_ptr;
	
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//---------------------------------------------
		
	wap_ptr.hdr.cmd				= MMI_CONFIRM_DIALOG_RESPONSE;
	wap_ptr.confirm.viewId		= viewId;
	wap_ptr.confirm.dialogId	= dialogId;
	wap_ptr.confirm.answer		= answer;
	
	make_cmd_other2wap( &wap_ptr );
    */
    MMIc_confirmDialogResponse(viewId, dialogId, answer);
    ProcessEvents();
}

void SKY_alertDialogResponse (UINT8 viewId, UINT8 dialogId)
{
    /*
	wap_cmd_type	wap_ptr;
	
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	//if(g_bBlocked)	return;
	//---------------------------------------------
		
	wap_ptr.hdr.cmd			= MMI_ALERT_DIALOG_RESPONSE;
	wap_ptr.alert.viewId	= viewId;
	wap_ptr.alert.dialogId	= dialogId;
	
	make_cmd_other2wap( &wap_ptr );
    */
    MMIc_alertDialogResponse(viewId, dialogId);
    ProcessEvents();
}

void SKY_keySelected(UINT8 viewId, UINT8 keyId)
{    
    MMIc_keySelected(viewId, keyId);
    ProcessEvents();
}

void SKY_textSelected (UINT8 viewId, UINT8 textId)
{
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------
    MMIc_textSelected(viewId, textId);
    ProcessEvents();
}

void SKY_imageSelected (UINT8 viewId, UINT8 imageId)
{
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

    MMIc_imageSelected(viewId, imageId);
    ProcessEvents();
}

void SKY_optionSelected (UINT8 viewId, UINT8 optionId)
{
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------
    MMIc_optionSelected(viewId, optionId);
    ProcessEvents();
}

void SKY_inputString (UINT8 viewId, UINT8 inputId, const WCHAR *text)
{
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

    MMIc_inputString(viewId, inputId, text);
    ProcessEvents();
}

void SKY_linkInfo_c (UINT8 viewId, UINT8 widgetType, UINT8 widgetId)
{
	//---------------------------------------------
	//AUR is Blocked now, so you must wait
	if(g_bBlocked)	return;
	//---------------------------------------------

    MMIc_linkInfo(viewId, widgetType, widgetId);
    ProcessEvents();
}

