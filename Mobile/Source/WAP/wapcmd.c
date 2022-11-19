/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           Wireless Application Protocol Command Execution

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  

=============================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/10/99   swjung  Define functions for calling connector function 
============================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "comdef.h"
#include "task.h"
#include "rex.h" 
#include "queue.h"
#include "err.h"
#include "msg.h"


#include "waptask.h"
#include "wapadapter.h"
#include "wapconnector.h"

#include "tapicmmn.h"
#include "capiclnt.h"
#include "capimmi.h"
#include "capimem.h"
#include "capiudp.h"
#include "msg.h"
#include <memory.h>
#include "psi.h"
#include "psmisc.h"
#include "dog.h"
#include "dsm.h"
#include "clnt.h"
#include "errcodes.h"
#include "aapiclnt.h"

#include "mmi_func.h"
#if MMI_TYPE == SKY_UI  
#include "sky.h"
#else
#include "ui.h"
#endif
#ifdef SOCKET_AUWAP
#include "dssocket.h"
#endif //SOCKET_AUWAP

#ifdef WAP_USE_LEAK
	VOID* WIPAlloc(UINT32 size);
    void ClearMallocData(void* pmalloc,BOOL nullreturn);
#endif//#ifdef WAP_USE_LEAK
/*===========================================================================

FUNCTION SEND_CMD_OTHER2WAP

DESCRIPTION
  Allows other tasks to queue up messages to the WAP.  Grab a buffer from the
  wap_cmd_free_q, fill it in, and call ui_cmd with it.  All this will
  do is queue it and set a signal for the WAP task.  When the command
  is done, the buffer will be queued as directed in the command header,
  and a task you specify may be signaled.

DEPENDENCIES
  wap_cmd_q already initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void send_cmd_other2wap ( wap_cmd_type *cmd_ptr )
{
#ifndef FEATURE_WAP
  (void) q_link( cmd_ptr, &cmd_ptr->hdr.link );   /* init link */
  q_put( &wap_cmd_q, &cmd_ptr->hdr.link );        /* and queue it */
  (void) rex_set_sigs( &wap_tcb, WAP_CMD_Q_SIG ); /* signal the WAP task */
#endif //#ifndef FEATURE_WAP
}


/*===========================================================================

FUNCTION MAKE_CMD_OTHER2WAP

DESCRIPTION
  Allows other tasks to queue up messages to the WAP.  Grab a buffer from the
  wap_cmd_free_q, fill it in, and call ui_cmd with it.  All this will
  do is queue it and set a signal for the WAP task.  When the command
  is done, the buffer will be queued as directed in the command header,
  and a task you specify may be signaled.

DEPENDENCIES
  wap_cmd_q already initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void make_cmd_other2wap( wap_cmd_type *msg_ptr )
{
  wap_cmd_type	*item_ptr;  /* Temp pointer to the item in the queue */
  q_link_type	link;       /* Save link from being written over     */

	MSG_HIGH("called by mmi connecter ftn",0,0,0);

  /* --------------------------------
  ** Get an item from WAP's free queue
  ** -------------------------------- */
   if (( item_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) {
    /* ------------------------
    ** Save link before copying
    ** ------------------------ */
    link = item_ptr->hdr.link;

    /* -------------------------
    ** Copy data into queue item
    ** ------------------------- */
    *item_ptr = *msg_ptr;

    /* ----------------------
    ** Put back the link info
    ** ---------------------- */
    item_ptr->hdr.link = link;

    /* -------------------------
    ** Fill in remaining details
    ** ------------------------- */
    item_ptr->hdr.task_ptr   = NULL;
    item_ptr->hdr.done_q_ptr = &wap_cmd_free_q;

    /* ------------
    ** Send command
    ** ------------ */
    send_cmd_other2wap( item_ptr );    
  }
  else {
    ERR_FATAL( "make_cmd_other2wap: Free queue is EMPTY!", 0, 0, 0 );
  }
} /* end make_cmd_other2wap() */


/*===========================================================================

FUNCTION WAP_GET_CMD()

DESCRIPTION
  Check event and call connector function.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern void  CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);
extern void  wip_free(void* mem);
extern void *wip_malloc (UINT32 size);
extern void wap_check_and_run(void);
extern boolean exit_comp;
extern boolean load_data;

#ifdef FEATURE_WAP
boolean browserTerComp;
#else
extern boolean browserTerComp;
#endif //FEATURE_WAP

#ifndef LAST_USE
	extern char save_source[4];
	extern char save_dest[4];
#endif//#ifndef LAST_USE

#ifdef BROWSER_UPDATE
#define REDIRECT

#define PDUTYPE     0x78
#define RESULTPDU   0x10
#define ABORTPDU    0x20
#define INVOKEPDU   0x08
#define ACKPDU      0x18
#define SEG_INVOKE  0x28
#define SEG_RLT     0x30
#define NACK        0x38

#define WTPPDU (RESULTPDU | ABORTPDU | INVOKEPDU | ACKPDU | SEG_INVOKE | SEG_RLT |NACK )	    

#define REDIRECTTYPE 0x03
#define PERMANENT    0x80
#define Bearer     0x80
#define PortNum    0x40
#define Addlength  0x3f

#endif//#ifdef BROWSER_UPDATE

#define CONTENT_TYPEWTLS  "application/vnd.wap.skt_e2e_wml"//Wml 문서 
#define CONTENT_TYPEWMLS  "application/vnd.wap.skt_e2e_wmls"//for script

#ifdef WAP_USE_LEAK
	int UDP_decData_co(char* encdata, char* decdata , int datalen, UINT32* declength, BOOL WML);
#else
	BOOL UDP_decData_co(char* encdata, char* decdata , int datalen, UINT32* declength, BOOL WML);
#endif//#ifdef WAP_USE_LEAK


char*   cDecdata;
UINT32  iDeclength   = 0;
extern  BOOL wtls_enc_start;

#ifndef FEATURE_WAP
extern  boolean bStopKey;
#endif //#ifndef FEATURE_WAP

#ifdef FEATURE_WAP
BOOL Wtlshello;
#else
extern  BOOL Wtlshello;
#endif //FEATURE_WAP

#ifndef DIRECT_RESPONSE  
	UINT16 ContentURLid = 0;
#endif//DIRECT_RESPONSE

#ifdef REDIRECT
/*===============================================================================
						FOR REDIRECT Processing
 ================================================================================*/
#ifdef WAP_USE_LEAK
	int processredirect(CHAR* redirectdata, UINT16* len);
#else
	BOOL processredirect(CHAR* redirectdata, UINT16* len);
#endif//#ifdef WAP_USE_LEAK
ui_cmd_type   *ui_cmd_ptr;      
#endif//#ifdef REDIRECT

#ifdef WAP_USE_SECURE
	extern void process_secure_client(CHAR* data,UINT16 datalen,UINT16 sport);
#endif//#ifdef WAP_USE_SECURE

boolean b_setclk = FALSE;
extern clk_cb_type wap_draw_timer;

void send_connect2ui(void)
{
#ifndef FEATURE_WAP
	ui_cmd_type   *pcmd;      
	if(( pcmd = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
	{
		CLNTa_log(1, 0, "showcard No UI cmd buffers");
	}
	else		
	{
		extern BOOL toneback_on;
		extern BOOL g_fWAP_FIRST_CONNECT;
		toneback_on = TRUE;
		if(g_fWAP_FIRST_CONNECT == TRUE)	//초기 접속시에만 탐
		{
			g_fWAP_FIRST_CONNECT = FALSE;
			pcmd->hdr.cmd        = UI_WAP_SHOWCARD_F;
		}
		else
		{
			pcmd->hdr.cmd        = UI_WAP_DISPLAY_F;
		}

		pcmd->hdr.task_ptr   = NULL;pcmd->hdr.done_q_ptr = &ui_cmd_free_q;
		ui_cmd( pcmd);
	}	
#endif //#ifndef FEATURE_WAP
}

#ifndef SOCKET_AUWAP


BOOL SKY_API sDisplayCurrentDateTimeInSubLCD(BOOL fDrawNow)
{
    DWORD dwCurrentTime;
    STATIC WORD s_wMinute = 0;
    clk_julian_type julian;
    BOOL fChangeMinute = FALSE;

    dwCurrentTime = SKY_GetLocalTime();
            
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &julian);
        if(fDrawNow || (s_wMinute != julian.minute))
        {
            DisplayDateTimeInSubLCD(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
            s_wMinute = julian.minute;
            fChangeMinute = TRUE;
        }
    }
    return fChangeMinute;
}

void wap_get_cmd( wap_cmd_type *cmd_ptr )
{
#ifndef FEATURE_WAP
  dsm_item_type *item_ptr; 
    
  //source ip
  ip4a sip;
  CHAR *sipaddr;
  static char saddr[4] ;
  UINT8 sourceLength; 
		
  //destination ip
  ip4a dip;
  CHAR *dipaddr;
  static char daddr[4];
  UINT8  destLength; 
  
  char* data;
  UINT16 datalen; 
    
  //dest port
  UINT16 ddport;
  
  //source port
  UINT16 ssport;

  switch (cmd_ptr->hdr.cmd) {
	
	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
	/*																									*/
	/*									C L I E N T														*/
	/*																									*/
	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
    
    /* jrkwon 2001.05.04 --> display for sub LCD */
    case WAP_SLCD_DISP_TIME :
        sDisplayCurrentDateTimeInSubLCD(FALSE);
        break;
    case WAP_SLCD_DISP_ANNUN :
        SKY_DrawSubAnnunciatorsIcon();
        break;
    /* <-- */
#ifdef WAP_USE_SECURE
	case WAP_SECURE_CMD:
	{
		process_secure_client(NULL,0,0);
		break;
	}
#endif//#ifdef WAP_USE_SECURE
	case WAP_START_NETWORK:
	{
		extern void do_ds_upenter(void);
		do_ds_upenter();
		break;
	}
  
	case WAP_MMI_WAIT:
	{
		SKY_wait(cmd_ptr->mmi_wait.viewId,cmd_ptr->mmi_wait.isWait);
		break;
	}

	case WAP_NEW_CARD:
	{
		SKY_newCard(cmd_ptr->mmi_new.viewId,cmd_ptr->mmi_new.title,cmd_ptr->mmi_new.isList,cmd_ptr->mmi_new.isRefresh,cmd_ptr->mmi_new.url,
					cmd_ptr->mmi_new.isBookmarkable,cmd_ptr->mmi_new.history);
		break;
	}

	case WAP_SHOW_CARD:
	{
		extern BOOL g_fWAP_FIRST_CONNECT;

		if(g_fWAP_FIRST_CONNECT == TRUE)	//초기 접속시에만 탐
		{
			extern unsigned char showcardId;
			showcardId = cmd_ptr->mmi_show.viewId;
		}
		else
		{
			SKY_showCard(cmd_ptr->mmi_show.viewId);
		}
		send_connect2ui();
		break;
	}	
	case WAP_TEMINATE_EVENT:
	{
		extern void process_terminate_event(void);
		extern boolean load_data;
        extern BOOL bTerminatewap ;
        
		if(load_data)
		{
#ifndef WAP_USE_INIT			
            bTerminatewap = TRUE;MEMc_prepareCache(MAX_CACHE_SIZE);
#else
            CLNTa_log(1,0,"mmi_stop clear in wapcmd.c");
            MMIc_stop(1);
#ifndef FEATURE_WAP
            MMIc_closeView(1);            
#endif //#ifndef FEATURE_WAP
#endif//#ifndef WAP_USE_INIT		
			load_data = FALSE;            
		}
		process_terminate_event();
		break;
	}
	
	case WAP_STOP_EVENT:
	{
		extern void process_stop_event(void);
		extern boolean load_data;
#ifdef WAP_USE_SECURE
		extern boolean bSetKey;
		extern CHAR SaveWtls[];
		//extern void ClearWTLSdata(void);
		extern void SendStopPacket(void);
		extern void resetalldata(void);
		extern BOOL dispWtls;

		dispWtls   = FALSE;
#endif//#ifdef WAP_USE_SECURE
 		if(load_data && !Wtlshello) 
        {
            MMIc_stop(1);
		}
#ifdef WAP_USE_SECURE
		else if(Wtlshello)
		{
			if(!bSetKey) memset(SaveWtls,0x00,1000);			

			resetalldata();
			SendStopPacket();
			//ClearWTLSdata();
		}
#endif//#ifdef WAP_USE_SECURE

		process_stop_event();
		break;
	}

	case WAP_GOBACK_EVENT:
	{
		extern void process_goback_event(void);
		MMIc_goBack(1);
		process_goback_event();
		break;
	}

	case WAP_BROWSER_START:		
	{
		extern void wap_set_start(void);
        extern void wap_init_browser(void);
	    wap_init_browser();
		wap_set_start();
		break;
	}
		
	case WAP_TERMINATE_COMP:	
	    browserTerComp = TRUE;
		break;

    case CLNT_TERMINATE:		
	{
		CLNTc_terminate();
		break;
	}
	case CLNT_FILE:	
	{
		CLNTc_file(cmd_ptr->file.fileId, cmd_ptr->file.data , cmd_ptr->file.length , cmd_ptr->file.contentType);
		break;
	}
	case CLNT_GET_CONTENT:		
	{
#ifdef ABORT_IN_CONTENT		
		extern BOOL bCall_content;
		extern boolean isStringResponse;		

	#ifndef DIRECT_RESPONSE
		extern boolean isIntegerResponse;
		if(isIntegerResponse ||isStringResponse) 
	#else
		if(isStringResponse)
	#endif//#ifndef DIRECT_RESPONSE
			bCall_content = TRUE;
 
	#ifndef DIRECT_RESPONSE
		ContentURLid  = cmd_ptr->get_content.urlId;
	#endif//DIRECT_RESPONSE

#endif//#ifdef ABORT_IN_CONTENT		

		CLNTa_log(1, 0, "CLNTc_getContent is called  %s",cmd_ptr->get_content.url);
#ifdef BROWSER_UPDATE
		/*======================================================
		                jjkyg 00/08/20 
				bStopKey를 UI에서 control함 
		 =======================================================*/
		//bStopKey = FALSE;
#endif//#ifdef BROWSER_UPDATE

		CLNTc_getContent(cmd_ptr->get_content.url, cmd_ptr->get_content.urlId, cmd_ptr->get_content.isOTAallowed);
		break;
	}
	case CLNT_SET_INT_CONFIG:		
	{
		CLNTc_setIntConfig(cmd_ptr->set_int_config.viewId, cmd_ptr->set_int_config.kind, cmd_ptr->set_int_config.value);
#ifndef FEATURE_WAP
		CLNTc_initialised();
#endif //FEATURE_WAP
		break;
	}
	case CLNT_SET_STR_CONFIG:		
	{
		CLNTc_setStrConfig(cmd_ptr->set_str_config.viewId, cmd_ptr->set_str_config.kind, (const CHAR*)cmd_ptr->set_str_config.value, cmd_ptr->set_str_config.length);
#ifndef FEATURE_WAP
		CLNTc_initialised();
#endif //#ifndef FEATURE_WAP
		break;
	}

//jjkyg 2000/06/07 for CLNTc_Useport
#ifdef BROWSER_UPDATE
	case CLNT_USE_PORT:
	{
#ifndef FEATURE_WAP
		CLNTc_usePort(cmd_ptr->use_port.viewId,cmd_ptr->use_port.id,cmd_ptr->use_port.port,
				  	  cmd_ptr->use_port.success);		
#endif //#ifndef FEATURE_WAP
		break;
	}
	case CLNT_CLOSE_PORT:
	{
#ifndef FEATURE_WAP
		CLNTc_closePort(cmd_ptr->close_port.port);
#endif //FEATURE_WAP
		break;
	}
#endif//ifdef BROWSER_UPDATE
	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
	/*																									*/
	/*									M M I															*/
	/*																									*/
	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
	case MMI_OPEN_VIEW:		
	{
#ifndef FEATURE_WAP
		MMIc_openView(cmd_ptr->open_view.viewId, cmd_ptr->open_view.uamode);
#endif //FEATURE_WAP
		break;
	}
	case MMI_CLOSE_VIEW:	
	{
#ifndef FEATURE_WAP
		MMIc_closeView(cmd_ptr->close_view.viewId);
#endif //#ifndef FEATURE_WAP
		break;
	}
	case MMI_LOAD_URL:		
	{
		MMIc_loadURL(cmd_ptr->load_url.viewId, cmd_ptr->load_url.url, cmd_ptr->load_url.reload);
		break;
	}
	case MMI_RELOAD:
	{
		MMIc_reload(cmd_ptr->reload.viewId);
		break;
	}
	case MMI_STOP:
	{
		if(load_data && !Wtlshello) 
        {
              CLNTa_log(1,0," MMI_STOP in wapcmd");
              MMIc_stop(1);           
		}
		break;
	}
	case MMI_GO_BACK:
	{
		MMIc_goBack(cmd_ptr->go_back.viewId);
		break;
	}
	case MMI_PASSWORD_DIALOG_RESPONSE:
	{
		MMIc_passwordDialogResponse(cmd_ptr->password.viewId, cmd_ptr->password.dialogId, cmd_ptr->password.name, cmd_ptr->password.password);
		break;
	}
	case MMI_PROMPT_DIALOG_RESPONSE:
	{
		MMIc_promptDialogResponse(cmd_ptr->prompt.viewId, cmd_ptr->prompt.dialogId, cmd_ptr->prompt.answer);
		break;
	}
	case MMI_CONFIRM_DIALOG_RESPONSE:
	{
		MMIc_confirmDialogResponse(cmd_ptr->confirm.viewId, cmd_ptr->confirm.dialogId, cmd_ptr->confirm.answer);
		break;
	}
	case MMI_ALERT_DIALOG_RESPONSE:
	{
		MMIc_alertDialogResponse(cmd_ptr->alert.viewId, cmd_ptr->alert.dialogId);
		break;
	}
	case MMI_KEY_SELECTED:
	{
		MSG_HIGH("키 선택 view:%d, key:%d",cmd_ptr->key.viewId, cmd_ptr->key.keyId,0);
		MMIc_keySelected(cmd_ptr->key.viewId, cmd_ptr->key.keyId);
		break;
	}
	case MMI_TEXT_SELECTED:
	{
		MMIc_textSelected(cmd_ptr->text.viewId, cmd_ptr->text.textId);
		break;
	}
	case MMI_IMAGE_SELECTED:
	{
		MMIc_imageSelected(cmd_ptr->image.viewId, cmd_ptr->image.imageId);
		break;
	}
	case MMI_OPTION_SELECTED:
	{
		MSG_HIGH("옵션선택 view:%d, option:%d",cmd_ptr->option.viewId, cmd_ptr->option.optionId,0);
		MMIc_optionSelected(cmd_ptr->option.viewId, cmd_ptr->option.optionId);
		break;
	}
	case MMI_INPUT_STRING:
	{
		MMIc_inputString(cmd_ptr->input.viewId, cmd_ptr->input.inputId, cmd_ptr->input.text);
		break;
	}
	case MMI_LINKINFO:
	{
		MMIc_linkInfo(cmd_ptr->linkinfo.viewId, cmd_ptr->linkinfo.widgetType, cmd_ptr->linkinfo.widgetId);
		CLNTa_log(1,0,"LINK INFO reachded...");
		break;
	}

	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
	/*																									*/
	/*									M E M O R Y														*/
	/*																									*/
	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
	case MEM_INIT_CACHE:
	{
		MEMc_initCache ( cmd_ptr->init_cache.cacheSize,  cmd_ptr->init_cache.restoredSize);
		break;
	}
	case MEM_PREPARE_CACHE:
	{
		CLNTa_log(1,0,"==================================\n");
		CLNTa_log(1,0,"CAHCE PREPARE CALLED\n");
		CLNTa_log(1,0,"=================================\n");
		MEMc_prepareCache(cmd_ptr->prepare_cache.availablePersistentMemory);
		break;
	}
	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
	/*																									*/
	/*									U D P															*/
	/*																									*/
	/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
	case UDP_RECEIVED_REQUEST:
	{
		#ifdef BROWSER_UPDATE	
			extern BOOL   bCreateReq;
			extern BOOL   initialaccess;
			extern uint16 SAport;
		#endif
		/////////////////////////////////////////////////////////
		// A7 porting
		/////////////////////////////////////////////////////////
		   item_ptr = (dsm_item_type *) q_get( &psi_udp_q);

		   if(item_ptr == NULL) 
		   {
			   CLNTa_log(1,0," UDP data invalid");
			  /*=====================================
					WTP일 경우에는 다르게 해야 함 
			   =====================================*/
			#ifdef BROWSER_UPDATE
			    UDPc_errorRequest(2,SAport);			   
			#else
				UDPc_errorRequest(2,1000);			   
			#endif
			   break;
		   }

		//dest ip	
			dsm_pullup(&item_ptr,&dip,sizeof(dip));
			dipaddr = &daddr[0];
			(void)put32((uint8*)dipaddr,(dword)dip);
			destLength = 4 ;
 
		//dest port
			dsm_pullup(&item_ptr,&ddport,2);
			ddport = ntohs(ddport);
			CLNTa_log(1,0,"dest port %d",ddport);
			
		// destination len
			dsm_pullup(&item_ptr,&datalen,2);
			CLNTa_log(1,0,"data len %d",datalen);

		//source port
			dsm_pullup(&item_ptr,&ssport,2);
			ssport = ntohs(ssport);
			CLNTa_log(1,0,"src port %d",ssport);
			
		//source ip
			dsm_pullup(&item_ptr,&sip,sizeof(sip));
			sipaddr = &saddr[0];

			//remove_error
			//(void)put32((char*)sipaddr,(dword)sip);
			put32((byte *)sipaddr,(dword)sip);
			sourceLength = 4;

		
			data = (char*)wip_malloc(datalen+1);
			memset(data,0x00,datalen+1);

			(void)dsm_extract( item_ptr, 0, data, (word)datalen);
			dsm_free_packet(&item_ptr);
			
			if(((PDUTYPE & data[0])& WTPPDU) == 0)
			{
				wip_free(data);UDPc_errorRequest(2,SAport);			   
			}

#ifdef WAP_USE_SECURE			
			if(	(strstr(data,CONTENT_TYPEWTLS)!=NULL) ||(strstr(data,CONTENT_TYPEWMLS)!=NULL) )
			{
#endif//#ifdef WAP_USE_SECURE
				if(wtls_enc_start)
				{
			#ifdef WAP_USE_LEAK
					int nRet  = 0; 
			#else
					BOOL  nRet  = FALSE; 
			#endif//#ifdef WAP_USE_LEAK
                    char* ptemp = NULL;
					bStopKey = TRUE;

					if((PDUTYPE & data[0]) == RESULTPDU)
					{
						CLNTa_log(1,0," RESULT PDU Method %x",data[3]);
						if(data[3] == 0x03){ CLNTa_log(1,0,"Redirect message received in WAPCMD.C"); }
						else if(data[3] == 0x02)//connectreply
						{
							/*=====================================================================
											COnnetReply를 받았을 경우에 대한 처리
							  =====================================================================*/
							bStopKey = FALSE;
							UDPc_receivedRequest(data,datalen,(const CHAR*)save_source,4,(const CHAR*)save_dest,4,ddport,ssport);
							wip_free(data);	
							break;
						}
					}
					else if((PDUTYPE & data[0]) == ACKPDU)
					{
						CLNTa_log(1,0,"Receive ACK PDU TID1=%x, TID2=%x",data[1],data[2]);
						if(initialaccess) bCreateReq = FALSE;

						bStopKey = FALSE;	
						UDPc_receivedRequest(data,datalen,(const CHAR*)save_source,4,(const CHAR*)save_dest,4,ddport,ssport);	
						wip_free(data);
						break;							
					}
					else
					{
						CLNTa_log(1,0,"Other one %d ",data[0]);
					}

					if(initialaccess)
					{
						//Hold on ack 인듯함
						if((PDUTYPE & data[0])== ACKPDU) bCreateReq = FALSE;
						else bCreateReq = FALSE;
					}				

					CLNTa_log(1,0,"detect ENC DATA1");				
					iDeclength = 0;

					//For size checking...
					if(datalen > 2000)//encryption data는 2000byte를 넘어서는 않됨
					{
						bStopKey = FALSE;
						wip_free(data);	UDPc_errorRequest(2,ddport);
						break;
					}
				/*======================================================================
					Dec data는 아무리 커도 enc data보다 작으므로 enc data length
	 				로 설정 해도  된다.
				 ======================================================================*/
					ptemp    =  data;
					cDecdata = (char*)wip_malloc(datalen);
				/*=========================================================================*/
                    if(cDecdata == NULL)
					{
				#ifndef WAP_USE_LEAK
						wip_free(data);
				#endif//#ifndef WAP_USE_LEAK
						break;
					}
					if((strstr(data,CONTENT_TYPEWTLS))!=NULL)
					{
						CLNTa_log(1,0," CONTENT_TYPEWTLS DETECT");
						nRet = UDP_decData_co(ptemp,cDecdata,datalen,&iDeclength,TRUE);
					}
					else if((strstr(data,CONTENT_TYPEWMLS))!=NULL)
					{
						CLNTa_log(1,0," CONTENT_TYPEWMLS DETECT");
						nRet = UDP_decData_co(ptemp,cDecdata,datalen,&iDeclength,FALSE);
					}
					else
					{
						CLNTa_log(1,0," WTLS RECEIVE DATA ERROR OCCUR");
						bStopKey = FALSE;
						wip_free(data);	wip_free(cDecdata);
						UDPc_errorRequest(2,ddport);
						break;
					}

			#ifndef WAP_USE_LEAK
					if(!nRet)
			#else
					if(nRet == 0)
			#endif//#ifndef WAP_USE_LEAK
					{ 
						CLNTa_log(1,0,"RETURN ERROR IN WTLS");
						bStopKey = FALSE;
						wip_free(data);	wip_free(cDecdata);
						UDPc_errorRequest(2,ddport);
						break;
					}
            #ifdef WAP_USE_LEAK
					else if(nRet == 2)
					{
						CLNTa_log(1,0,"RETURN NULL ERROR IN WTLS");
						bStopKey = FALSE;		
						break;
					}
			#endif//#ifdef WAP_USE_LEAK
					else
					{
					/*=========================================================================
							- Initial access일 경우 End key및 CLR_k와 같은 종료를 일시 막기위한 것
							- jjkyg 00/06/19 porting
					==========================================================================*/
						CLNTa_log(1,0,"WTLS enc data received..");
					#ifndef LAST_USE
						UDPc_receivedRequest(cDecdata,(int)iDeclength,save_source,4,save_dest,4,ddport,ssport);
					#else
						UDPc_receivedRequest(cDecdata,(int)iDeclength,(const CHAR*)sipaddr,4,(const CHAR*)dipaddr,4,ddport,ssport);
					#endif//#ifndef LAST_USE
				/*=====================================================================
						12 byte가 입력될 경우에는 이상동작을 하므로 키를 풀어줌
				 =====================================================================*/
						bStopKey = FALSE;wip_free(data);wip_free(cDecdata);
					}
				}
#ifdef WAP_USE_SECURE
				else if(Wtlshello)
				{
					if((PDUTYPE & data[0]) == RESULTPDU)
					{
						if(data[3] == 0x03)//Redirect message
						{
                            int tempRet3 = 0;
							tempRet3 = processredirect(&data[3],&datalen);

							if(tempRet3 == 0)
                            {
								wip_free(data);UDPc_errorRequest(2,ddport);
								break;
							}
                #ifdef WAP_USE_LEAK
							//Null return
							else if(tempRet3 == 2)
							{
								break;
							}
				#endif//#ifdef WAP_USE_LEAK
							else//processredirect == TRUE
							{
								extern BOOL save_redirecturl(void);			
								extern void redirect_terminate(void);
								wip_free(data);save_redirecturl();redirect_terminate();
#ifdef PORTING_B0_WAP // WTLS관련 State에서 필요한 Event!! So 현재 보안모듈로 바뀐 다음엔 쓰지 않아서 막음 !!
								if(	(ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q))!= NULL )
								{
									ui_cmd_ptr->hdr.cmd        = UI_REDIRECT_REQ_F;
									ui_cmd_ptr->hdr.task_ptr   = NULL;
									ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
									ui_cmd( ui_cmd_ptr);
								}
#endif //PORTING_B0_WAP
								break;
							}//end of processdirect =TRUE
						}//end of data[3] = 0x03
						else if(data[3] == 0x04)
						{
							process_secure_client(data,datalen,ddport);
							wip_free(data);datalen = 0;
						}
						else//reply 및 reconnect가 아닐경우 
						{
							UDPc_receivedRequest(data,datalen,(const CHAR*)save_source,4,(const CHAR*)save_dest,4,ddport,ssport);
							wip_free(data);datalen = 0;
						}
					}//if(PDUTYPE & data[0]) == RESULTPDU)
				}//else if(Wtlshello)
#endif//#ifdef WAP_USE_SECURE
#ifdef WAP_USE_SECURE			
			}//if(	(strstr(data,CONTENT_TYPEWTLS)!=NULL) ||(strstr(data,CONTENT_TYPEWMLS)!=NULL) )
#endif//#ifdef WAP_USE_SECURE			
			else
			{				
#ifndef WAP_USE_SECURE
				if(	(strstr(data,CONTENT_TYPEWTLS)!=NULL) || (strstr(data,CONTENT_TYPEWMLS)!=NULL) )
				{
					extern int typeOfContent;
					typeOfContent = SecTLS;
					CLNTa_log(1,0,"WTLS start in WAPCMD.c");
				}
#endif//WAP_USE_SECURE

                CLNTa_log(1,0,"Send data to Core");
				if((PDUTYPE & data[0]) == RESULTPDU)
				{
					//CLNTa_log(1,0,"Receive RESULT PDU first = %x TID1=%x, TID2=%x",data[0],data[1],data[2]);
					CLNTa_log(1,0,"RESULT PDU Method %x",data[3]);
					if(data[3] == 0x04)
					{
						CLNTa_log(1,0,"Real data length %d",datalen);
					}
		#ifdef REDIRECT
			#ifndef WAP_USE_SECURE
					if(Wtlshello)//clnt_getcontent시 Redirect message의 수신 
					{
						if(data[3] == 0x03)//Redirect message
						{
							if(!processredirect(&data[3],&datalen))
							{
								wip_free(data);UDPc_errorRequest(2,ddport);
								break;
							}
							else//processredirect == TRUE
							{
								extern BOOL save_redirecturl(void);			
								extern void redirect_terminate(void);
								wip_free(data);	
								save_redirecturl();redirect_terminate();

								if(
									(ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q))!= NULL
								  )
								{
#ifdef PORTING_B0_WAP
									ui_cmd_ptr->hdr.cmd        = UI_REDIRECT_REQ_F;
									ui_cmd_ptr->hdr.task_ptr   = NULL;
									ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
									ui_cmd( ui_cmd_ptr);
#endif //PORTING_B0_WAP
								}	
								break;
							}//end of processdirect =TRUE
						}//end of data[3] = 0x03
					}//end of Wtlshello
			#endif//#ifndef WAP_USE_SECURE
		#endif//#ifdef REDIRECT
				}
				else if((PDUTYPE & data[0]) == ACKPDU)
				{
					CLNTa_log(1,0,"Receive ACK PDU TID1=%x, TID2=%x",data[1],data[2]);
				}
				/*=========================================================================
						- Initial access일 경우 End key및 CLR_k와 같은 종료를 일시 막기위한 것
						- jjkyg 00/06/19 porting
				==========================================================================*/
				if(initialaccess)
				{
					//Hold on ack 인듯함
					if((PDUTYPE & data[0])== ACKPDU) bCreateReq = FALSE;
					else bCreateReq = FALSE;
				}

#ifdef WAP_USE_SECURE
				/*=========================================================================
					보안 handshake phase에서 Reply를 받았으나 status가 OK(0x20)이 
					아닐시의 처리흘 위함임 
				==========================================================================*/
				if((PDUTYPE & data[0]) == RESULTPDU)
				{
					if(Wtlshello)
					{
						//만약 Reply Pdu일 경우에는 ....
						if(data[3] == 0x04)
						{
							if(  ((0x40 <= data[4]) && (data[4] <= 0x4f)) || ((0x60 <= data[4]) && (data[4] <= 0x65))  )
							{
								CLNTa_error(1, data[4], ERRTYPE_INFORMATION); 
								wip_free(data);datalen = 0;
					#ifndef LAST_USE
								memset(save_source,0,4);memset(save_dest,0,4);	
					#endif//#ifndef BROWSER_UPDATE
								break;
							}
						}						
					}
				}
#endif//#ifdef WAP_USE_SECURE

				UDPc_receivedRequest(data,datalen,(const CHAR*)save_source,4,(const CHAR*)save_dest,4,ddport,ssport);
				wip_free(data);datalen = 0;
			#ifndef LAST_USE
				memset(save_source,0,4);memset(save_dest,0,4);	
			#endif//#ifndef BROWSER_UPDATE
			}
		break;
	}//end of case

	case UDP_ERROR_REQUEST:
	{
		UDPc_errorRequest(cmd_ptr->udp_error.message, cmd_ptr->udp_error.destinationPort);
		break;
	}
  }//end of switch
#endif //#ifndef FEATURE_WAP	
}

/*==============================================================
				Extern 변수 & GLobal 변수
  ==============================================================*/
const int   Normalwml      =  0x94;
const int   NormalScript   =  0x95;

/*==============================================================
						FUNCTION
 ==============================================================*/
#ifndef FEATURE_WAP
#ifdef WAP_USE_SECURE
extern int      ency_read(unsigned char *ucpRecv, int nBufLen, int *npErr,char* cpRec,int nReadLen);
#endif //#ifdef WAP_USE_SECURE
#endif //#ifndef FEATURE_WAP

extern UINT32   UintvarToInt (BYTE *s, UINT32 *pn, UINT32 maxLen);
extern UINT32   UintvarLen (UINT32 n);
extern UINT32   IntToUintvar (UINT32 n, BYTE *v);

/*=======================================================================================
                   WTP version용 Secure data processing을 위하여 사용
 =======================================================================================*/
#ifdef WAP_USE_LEAK
	int UDP_decData_co(char* encdata, char* decdata , int datalen, UINT32* declength, BOOL WML)
#else
	BOOL UDP_decData_co(char* encdata, char* decdata , int datalen, UINT32* declength, BOOL WML)
#endif//#ifdef WAP_USE_LEAK
{
#ifndef FEATURE_WAP
  UINT16    SDU_size = datalen;
  UINT32    headlen, bodylen;
  UINT32    hn;
  BYTE      PDU_WTP[3];
  BYTE      PDU_TYPE;
  BYTE      PDU_STATUS;         

  char      *pSavehd  = NULL;
  char      *pSavebd  = NULL;
  char      *pTempdec = NULL;
  char      *cSaveHD  = NULL;
  char      *ptempHD2 = NULL;

  int       templength  = 0;
  int       pErr;
  UINT32    i =0;
  static    UINT16 sz = 0;

  sz = 0;

  if ((encdata == NULL) || (SDU_size <= 3)) { return FALSE; }

  /*============================================================
	1.WTP일 경우에는, WTP header필드가 Result PDU일 경우 3byte임
	  3byte save...
	2.Type필드 및 Status필드를 save함.
   =============================================================*/
  memcpy(PDU_WTP,encdata,3);
  encdata+=3;

  PDU_TYPE   = *encdata++;//METHOD
  PDU_STATUS = *encdata++;//Status

  //REPLY인지 확인
  if(PDU_TYPE !=4)
  { 
	  CLNTa_log(1,0,"PDU not REPLY.."); return FALSE; 
  }
  
  /*==========================================================
    1. header length extract 
    2. header length = content type length + header length
   ============================================================*/
  if ((hn = UintvarToInt ((BYTE *)encdata, &headlen, 5)) == 0) { return FALSE; }
  encdata += hn;//header length가 차지하고 있는 BYTE

  if ((int)(headlen + 3 + 2 + hn) > SDU_size) { return FALSE; }
  bodylen = SDU_size - 3 - 2 - hn - headlen;

  //For size checking...
  if(bodylen > 2000)//encryption data는 2000byte를 넘어서는 않됨  
  {
#ifndef FEATURE_WAP
	bStopKey = FALSE;
#endif //#ifndef FEATURE_WAP
	return FALSE;
  }

  /*=============================================================
						Header extract
   =============================================================*/
  if (headlen > 0) 
  {
    CLNTa_log(1,0,"ENC header len %d", headlen);
	pSavehd = (char*)wip_malloc(headlen);
	if (pSavehd == NULL) 
	{
      /* Error, no memory available. */
#ifdef WAP_USE_LEAK
      return 2;
#else
	  return FALSE;
#endif//#ifdef WAP_USE_LEAK
    }
    memcpy(pSavehd,encdata,headlen);encdata += headlen;
  }
  else{//header가 없으므로 잘못되었음
	CLNTa_log(1,0,"Not find Header");
    return(FALSE);
  }
  /*=============================================================
						BODY extract
   =============================================================*/
  if(bodylen > 0)
  {
   	 pSavebd = (char*)wip_malloc(bodylen);
     if(pSavebd == NULL)
	 {        
#ifdef WAP_USE_LEAK
        ClearMallocData((char*)pSavehd,TRUE);
		return 2;
#else
        wip_free(pSavehd);
		return FALSE;
#endif//#ifdef WAP_USE_LEAK
	 }
	 memcpy(pSavebd,encdata,bodylen);

	 CLNTa_log(1,0,"BODY LENGTH %d",bodylen);
  }//if(bodylen > 0)
  else
  {
    return FALSE;
  }
   /*============================================================================
					    	decryption data
		1. decryption 데이타를 일단 할당한다.
		    -->이때 받은 데이타 만큼만 wip_malloc한다.
			-->아무리 커도 받은 데이타 이상으로 decrtption데이타 클 순 없으므로 
			   이렇게 해도 된다.
		2. 이를 enc_read()로 넘긴다.
    ============================================================================*/
     templength  = 0;
     pTempdec    = (char*)wip_malloc(bodylen);//temporal decoding data
     if(pTempdec == NULL)
	 {
#ifdef WAP_USE_LEAK
        ClearMallocData(pSavehd,TRUE);
		ClearMallocData(pSavebd,TRUE);
		return 2;
#else
		return FALSE;
#endif//#ifdef WAP_USE_LEAK
	 }

	 templength  = ency_read((unsigned char *)pTempdec, bodylen,&pErr,pSavebd,bodylen);//body length 
	 if(templength < 0)
	 {
		wip_free(pSavehd);wip_free(pSavebd);
		templength = 0;
		return FALSE;
	}
#ifdef WAP_USE_LEAK
	else if(templength == 2)
	{
		ClearMallocData(pSavehd,TRUE);
		ClearMallocData(pSavebd,TRUE);
		ClearMallocData(pTempdec,TRUE);
		templength = 0;
		return 2;
	}
#endif//#ifdef WAP_USE_LEAK
	CLNTa_log(1,0,"templength %d", templength);

	/*=====================================================================================
			            Encryption이 완성되었을 경우에는 header를 
			                normal한 경우의 것을 사용한다.
	 ======================================================================================*/
	if(WML)
		sz = sizeof(CONTENT_TYPEWTLS);//32
	else
		sz = sizeof(CONTENT_TYPEWMLS);
	/*========================================================================================
	   - Header에 포함된 string은 NULL terminate임
	   1. 첫번째로 들어온 CONTENT_TYPEWTLS제거
	   2. 두번째도 있는지 살펴봄
	 ========================================================================================*/
	//1번째 
	headlen    = headlen - sz;
	ptempHD2   = (char*)wip_malloc(headlen);
    if(ptempHD2 == NULL)
	{	
#ifdef WAP_USE_LEAK
        ClearMallocData(pSavehd,TRUE);
		ClearMallocData(pSavebd,TRUE);
		ClearMallocData(pTempdec,TRUE);
		return 2;
#else
		return FALSE;
#endif//#ifdef WAP_USE_LEAK
	}
	memcpy(ptempHD2,pSavehd+sz,headlen);

	headlen = headlen + 1;

	cSaveHD = (char*)wip_malloc(headlen);
    if(cSaveHD == NULL)
	{		
#ifdef WAP_USE_LEAK
        ClearMallocData(pSavehd,TRUE);
		ClearMallocData(pSavebd,TRUE);
		ClearMallocData(pTempdec,TRUE);
		ClearMallocData(ptempHD2,TRUE);
		return 2;
#else
		return FALSE;
#endif//#ifdef WAP_USE_LEAK
	}
	memset(cSaveHD,0,headlen);

	if(WML)
	{
		cSaveHD[0] =  Normalwml;
	}
	else{
		cSaveHD[0] =  NormalScript;
	}

	memcpy(&cSaveHD[1],ptempHD2,headlen-1);

	/*=====================================================================================
			                UDP data로 다시 Merge한다.
		Total length = WTP header(3) + METHOD(1) + STATUS(1) + HEADERLEN field LENGTH 
		               + HEADER LENTH(CONTENT TYPE + HEADER) + BODY
	 ======================================================================================*/
	*declength = 5 + UintvarLen(headlen) + headlen + templength;
	
	CLNTa_log(1,0,"TID processing when secure");
	memcpy(decdata,PDU_WTP,3);   //TID
	memcpy(decdata+3,&PDU_TYPE,1);  //METHOD
	memcpy(decdata+4,&PDU_STATUS,1);//STATUS
	
	i = IntToUintvar(headlen,(BYTE*)(decdata+5));//headlength
	
	memcpy(decdata+5+i,cSaveHD,headlen);//header
	
	memcpy(decdata+5+i+headlen,pTempdec,templength);

	wip_free(pSavehd);pSavehd = NULL;
	wip_free(pSavebd);pSavebd = NULL;
	wip_free(pTempdec);pTempdec = NULL;
	wip_free(cSaveHD);cSaveHD = NULL;
	wip_free(ptempHD2);ptempHD2 = NULL;
#endif //#ifndef FEATURE_WAP
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
#ifdef REDIRECT
#ifdef WAP_USE_LEAK
	int ExtractRedirect(CHAR* buf);
#else
	BOOL ExtractRedirect(CHAR* buf);	
#endif//#ifdef WAP_USE_LEAK

#ifdef REDIRECT

#ifndef FEATURE_WAP
extern void init_buffer(redirect_config* buf);
extern redirect_config redirect;
extern BOOL isfirstdirect;
#endif //#ifndef FEATURE_WAP

#endif //#ifdef REDIRECT

extern byte get_length( byte *chk_str, byte buff_length); 



#ifdef WAP_USE_LEAK
	int ExtractRedirect(CHAR* buf)
#else
	BOOL ExtractRedirect(CHAR* buf)
#endif//#ifdef WAP_USE_LEAK
{
#ifndef FEATURE_WAP
	CHAR *ptemp;
	ptemp = buf;

	//network include
	if((*ptemp & Bearer) == Bearer)
	{
		redirect.bNetinclude  = TRUE;		
	}

	if((*ptemp & PortNum) == PortNum)
	{
		redirect.bPortNuminclude = TRUE;
	}

	if((*ptemp & Addlength)!=0)
	{
		redirect.Addresslen = (*ptemp & Addlength);
	}
	ptemp++;

	if(	redirect.bNetinclude)
	{
		redirect.BearerType = *ptemp++;
	}

	if(redirect.bPortNuminclude)
	{
		memcpy(&redirect.Portnumber,ptemp,2);
		ptemp+=2;
	}

	//address가 string인지 아닌지 먼저 살핀다.
	if((*ptemp >= 0x30)&&(*ptemp <0x7B))
	{
		int length =0;
		CHAR* pptemp = NULL;

		pptemp = (CHAR*)ptemp;
		length = get_length((unsigned char*)pptemp,(unsigned char)redirect.Addresslen ); 
		CLNTa_log(1,0,"receive length %d",length);

		redirect.Addresslen = length;
		redirect.Address =(CHAR*)wip_malloc(redirect.Addresslen+1);
		memset(redirect.Address,0x00,redirect.Addresslen+1);
		strncpy(redirect.Address,(CHAR*)ptemp,redirect.Addresslen);
		redirect.bCorrect = TRUE;
	}
	else
	{
		redirect.bCorrect = FALSE;
		if(redirect.Address!=NULL)
		{	
			CLNTa_log(1,0,"Here is not enter when normal state");
			wip_free(redirect.Address);
			redirect.Address = NULL;
		}
	}
	return redirect.bCorrect;
#endif //FEATURE_WAP
}

#ifdef WAP_USE_LEAK
	int  processredirect(CHAR* redirectdata,UINT16* len)
#else
	BOOL processredirect(CHAR* redirectdata,UINT16* len)
#endif//#ifdef WAP_USE_LEAK
{
#ifndef FEATURE_WAP
  static UINT32 size = 0;
  CHAR*  ptemp;	
  BYTE   pdutype;	
  BYTE   flag ;

  isfirstdirect = TRUE; init_buffer(&redirect);
  CLNTa_log(1,0,"Receive Redirect Message IN security SESSION");	
  size =get_length((unsigned char*)redirectdata,*len);	
  
  if(size< 2)
  {
	  redirect.bCorrect = FALSE; return FALSE; 
  }
  
  ptemp = redirectdata;

  pdutype = *ptemp++;size--;
  if(pdutype != REDIRECTTYPE)
  {
	redirect.bCorrect = FALSE; return FALSE; 
  }
	
  flag = *ptemp++;size--;
  if (flag & PERMANENT) 
  {
	CLNTa_log(1,0,"Permanent FLAG bit in secure");
	redirect.bPerm_or_not = TRUE;
  }
  else if(flag == 0x00)//temparay flag
  {
	CLNTa_log(1,0,"temp FLAG bit in secure");
	redirect.bPerm_or_not = FALSE;
  }

  if(size > 0)
  {
	if(ExtractRedirect(ptemp)==TRUE)
	{
		return TRUE;
	}	
  }
  
  return FALSE;
#endif //#ifndef FEATURE_WAP
}
#endif//#ifdef REDIRECT

#ifdef WAP_USE_PZID2
void SendUDPError(void)
{
    extern uint16 SAport;
	wap_cmd_type* wap_event_ptr;
    if (( wap_event_ptr = (wap_cmd_type*)q_get( &wap_cmd_free_q )) != NULL) 
	{
		wap_event_ptr->hdr.task_ptr   = NULL;
		wap_event_ptr->hdr.done_q_ptr = &wap_cmd_free_q;
	}
	else {
		ERR_FATAL( "SendUDPError: Free queue is EMPTY!", 0, 0, 0 );
	}
	wap_event_ptr->hdr.cmd = UDP_ERROR_REQUEST;
    wap_event_ptr->udp_error.message =2;
    wap_event_ptr->udp_error.destinationPort=SAport;
	send_cmd_other2wap(wap_event_ptr);
}
#endif//#ifdef WAP_USE_PZID2

#ifdef WAP_USE_LEAK
void ClearMallocData(void* pmalloc,BOOL nullreturn)
{
	if(pmalloc!=NULL)
	{
		if(!nullreturn)	wip_free((void*)pmalloc);
		
		pmalloc = NULL;
	}
}
#endif //WAP_USE_LEAK


#else //SOCKET_AUWAP

/////////////////////////////////////////////////////////////////////////
////////////////////// NEW_GENERATION AUWAP//////////////////////////////
/////////////////////////////////////////////////////////////////////////
extern void  uiscrn_ann
(
  word  mask,                     /* mask of annunciators */
  word  value                     /* annunciator values to mask in */
);

typedef enum 
{
	BEARER_INIT_S = 0,
	BEARER_PPPOPENING_S,
	BEARER_PPPOPEN_S,
	BEARER_CLOSING_S,
	BEARER_CLOSE_S
} BEARER_STATE;

static db_items_value_type dbi;
static BEARER_STATE bearer_state = BEARER_INIT_S;

sint15 g_apID, g_sock_fd;

int is_connected = 0;

//extern rex_timer_type  wait_ani_timer;

extern void wait_ani_start(int type, int time);
extern void wait_ani_stop(void);
extern LOCAL void send_dnsquery(char* url);
void wap_disconnect(void);

void set_bearer_state(BEARER_STATE state)
{
	if ( state == BEARER_PPPOPENING_S ) {
		wait_ani_start( 0, 150 );
	} else {
		wait_ani_stop();
	}
	bearer_state = state;
}

void net_callback_fcn(void *p) 
{
#ifndef FEATURE_WAP
	CLNTa_log(1,0,"call network callback");
//	MSG_HIGH("net_callback_fcn",0,0,0);
    (void) rex_set_sigs(&wap_tcb, (sint31)NET_CB_SIG);
#endif //#ifndef FEATURE_WAP
}

void sock_callback_fcn(void *p)
{
#ifndef FEATURE_WAP
	CLNTa_log(1,0,"call socket callback");
//	MSG_HIGH("sock_callback_fcn",0,0,0);
    (void) rex_set_sigs(&wap_tcb, (sint31)SOCK_CB_SIG);
#endif //#ifndef FEATURE_WAP
}

BEARER_STATE get_bearer_state()
{
	return bearer_state;
}

void proc_sockcb()
{
#ifndef FEATURE_WAP
	sint15		errno;
	sint31		mask=0;

//	if (NetworkGetState() == NET_PPPOPEN_S)
//	{
		mask = dss_getnextevent(g_apID, (short*)&g_sock_fd, &errno);

		if (mask & DS_READ_EVENT)
		{
			MSG_HIGH("Net data arrived", 0, 0, 0);
//			/* Enable DS_READ_EVENT again */
//			if (dss_async_select((sint15)g_sock_fd, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
//			{
//				MSG_HIGH("dss_async_select(DS_READ_EVENT) error %d", errno, 0, 0);
//			}
			/* Notification of net data arrived */
			(void) rex_set_sigs( &wap_tcb, READ_EVT_SIG );
		}
//	}
#endif //#ifndef FEATURE_WAP
}

void proc_netcb()
{
	sint15 errno;
	sint15 lerrno;
	switch(get_bearer_state())
	{
		case BEARER_PPPOPENING_S:
			(void)dss_netstatus(g_apID, &errno); 
			switch(errno)
			{
				case DS_ENETISCONN :
					MSG_HIGH("Connected : DS_ENETISCONN", 0, 0, 0);
					set_bearer_state(BEARER_PPPOPEN_S);
					/* Enable DS_READ_EVENT at first*/
					if (dss_async_select((sint15)g_sock_fd, (sint31)DS_READ_EVENT, &lerrno)==DSS_ERROR)
					{
						MSG_HIGH("dss_async_select(DS_READ_EVENT) error %d", errno, 0, 0);
					}
					
					send_dnsquery("wgw.n-top.com");
					is_connected = 1;
					break;
				case DS_EBADAPP :
					MSG_HIGH("Connect fail : DS_EBADAPP", 0, 0, 0);
					set_bearer_state(BEARER_CLOSE_S);
					is_connected = 0;
					break;
				case DS_ENETNONET :
					MSG_HIGH("Connect fail : DS_ENETNONET", 0, 0, 0);
					set_bearer_state(BEARER_CLOSE_S);
					is_connected = 0;
					break;
				case DS_ENETINPROGRESS :
					MSG_HIGH("Connect fail : DS_ENETINPROGRESS", 0, 0, 0);
					set_bearer_state(BEARER_CLOSE_S);
					is_connected = 0;
					break;
				case DS_ENETCLOSEINPROGRESS :
					MSG_HIGH("Connect fail : DS_ENETCLOSEINPROGRESS", 0, 0, 0);
					set_bearer_state(BEARER_CLOSE_S);
					is_connected = 0;
					break;
				default:
					break;
			}
			break;
		case BEARER_CLOSING_S:
			(void)dss_netstatus(g_apID, &errno); 
			switch(errno)
			{
				case DS_ENETISCONN :
					MSG_HIGH("Disconnect fail : DS_ENETISCONN", 0, 0, 0);
					is_connected = 1;
					break;
				case DS_EBADAPP :
					MSG_HIGH("Disconnect fail : DS_EBADAPP", 0, 0, 0);
					set_bearer_state(BEARER_CLOSE_S);
					is_connected = 0;
					break;
				case DS_ENETNONET :
					MSG_HIGH("Disconnected  : DS_ENETNONET", 0, 0, 0);
					set_bearer_state(BEARER_CLOSE_S);
					is_connected = 0;
					break;
				case DS_ENETINPROGRESS :
					MSG_HIGH("Disconnect fail : DS_ENETINPROGRESS", 0, 0, 0);
					is_connected = 0;
					break;
				case DS_ENETCLOSEINPROGRESS :
					MSG_HIGH("Disconnect fail : DS_ENETCLOSEINPROGRESS", 0, 0, 0);
					is_connected = 0;
					break;
				default:
					break;
			}
			wap_disconnect();
			break;
		default:
			(void)dss_netstatus(g_apID, &errno); 
			switch(errno)
			{
				case DS_ENETISCONN :
					MSG_HIGH("Undetermined state : DS_ENETISCONN", 0, 0, 0);
					is_connected = 1;
					break;
				case DS_EBADAPP :
					MSG_HIGH("Undetermined state : DS_EBADAPP", 0, 0, 0);
					is_connected = 0;
					break;
				case DS_ENETNONET :
					MSG_HIGH("Undetermined state  : DS_ENETNONET", 0, 0, 0);
					is_connected = 0;
					break;
				case DS_ENETINPROGRESS :
					MSG_HIGH("Undetermined state : DS_ENETINPROGRESS", 0, 0, 0);
					is_connected = 0;
					break;
				case DS_ENETCLOSEINPROGRESS :
					MSG_HIGH("Undetermined state : DS_ENETCLOSEINPROGRESS", 0, 0, 0);
					is_connected = 0;
					break;
				default:
					break;
			}
			break;
	}
}

extern const    memSizeType   cCacheSize;
extern const    memSizeType   crestoredSize;
extern byte wait_animation;
extern byte clnta_err_flag;
extern INT16 wip_initmalloc (void *mem, UINT32 memsize);
/////////////////////////////////////////////////////////////////////////////
// WAP shares about 50k size memory with IrDA region
/////////////////////////////////////////////////////////////////////////////
extern char Image$$APP_SHARE_RAM$$Base[];
unsigned char *pSharedRAM = (unsigned char*)Image$$APP_SHARE_RAM$$Base;
/////////////////////////////////////////////////////////////////////////////

void clnt_activate()
{
#ifndef FEATURE_WAP
	wip_initmalloc(pSharedRAM, 50000);			 
    CLNTc_start();
    CLNTc_run();

    MEMc_initCache(cCacheSize, crestoredSize);
    CLNTc_run();

    MMIc_openView(VIEW_ID_WML, WML_UA);
    CLNTc_run();
	
//  MMIc_openView(VIEW_ID_WTA, WTA_UA);	  
 
    CLNTc_setIntConfig(0, configBEARER, BEARER_UDP);
    CLNTc_run();
    CLNTc_setStrConfig(0, configUDP_IP_SRC, "\x11\x22\x33\x44", 4);
    CLNTc_run();
    CLNTc_setStrConfig(0, configUDP_IP_GW, "\x55\x66\x77\x88", 4);
    CLNTc_run();
//    CLNTc_setStrConfig(0, configSMS_C, "460703123450", 12);
//    CLNTc_run();
//    CLNTc_setStrConfig(0, configSMS_GW, "460703123451", 12);
//    CLNTc_run();
    CLNTc_setIntConfig(0, configHISTORY_SIZE, 15);
    CLNTc_run();
    CLNTc_setIntConfig(0, configTIMEOUT, 40);
    CLNTc_run();
    CLNTc_setIntConfig(0, configCACHE_MODE,1);
    CLNTc_run();
    CLNTc_setIntConfig(0, configCACHE_AGE, 60*60);
    CLNTc_run(); 
    CLNTc_setIntConfig(0, configDISPLAY_IMAGES, TRUE);
    CLNTc_run();
    CLNTc_setIntConfig(0, configUPDATE_IMAGES, FALSE);
    CLNTc_run();

	/*
     ** WAP WSP ISO 639 language assignment code : Korean 0x3D
    */
    CLNTc_setStrConfig(0, configWSP_Language, "\x3D", 1);
    CLNTc_run();

    CLNTc_setIntConfig(0, configSTACKMODE, MODE_CO_WSP);
    CLNTc_run();

    CLNTc_initialised(); 
    CLNTc_run();
//	wap_set_start();
//	MSG_HIGH("WAP start", 0,0,0);
#endif //#ifndef FEATURE_WAP
}

void wap_activate()
{
#ifndef FEATURE_WAP
	wait_animation = 0;
	clnta_err_flag=0;

	clnt_activate();
	wap_set_start();
	MSG_HIGH("WAP start", 0,0,0);
#endif //#ifndef FEATURE_WAP
}


extern BOOL WAP_Terminate_WML(void);
extern uint16 SAport;
void wap_deactivate()
{
#ifndef FEATURE_WAP
	CLNTc_closePort(SAport);
	WAP_Terminate_WML();
	CLNTc_run();
	while ( CLNTc_wantsToRun() ) {
		CLNTc_run();
	}
	MEMc_prepareCache(10000);
	CLNTc_run();
	MSG_HIGH("WAP terminate", 0,0,0);
#endif //#ifndef FEATURE_WAP
}


/* That would be moved */
extern byte sorted_groupname[][];
byte backsorted_groupname[3000];

byte   gwdns_query =0;
uint32 wapgw_ip;

#ifdef FEATURE_WAP
extern ip4a primaryIP;
extern ip4a secondaryIP;
#endif //FEATURE_WAP

PACKED struct dnsheader {
	unsigned short 	id;
	
	unsigned int	rd		: 1;
	unsigned int	tc		: 1;
	unsigned int	aa		: 1;
	unsigned int	opcode	: 4;
	unsigned int	qr		: 1;
	
	unsigned int	rcode	: 4;
	unsigned int	z		: 3;
	unsigned int	ra		: 1;

	unsigned short 	qdcount;
	unsigned short 	ancount;
	unsigned short 	nscount;
	unsigned short 	arcount;
};

LOCAL char* getnext_delimeter(const char* str, char ch)

{
	while (*str && *str != ch)
		str++;
	return (char*)str;
}

int send_dnsbuf[25];
LOCAL void send_dnsquery(char* url)
{

	sint15 sockfd;                    /* socket related variables */
	uint16 flags;
	struct sockaddr_in toaddr;
	uint16 addrlen;
	sint15 errno;
//	sint15 nbytes;
	sint15 bytes_per_write;


	struct dnsheader header;          /* dns header related variables */
	byte* data_buf;
	char* next;
    char* name;
	int namelen;
	int len;
	int pos = 0;
	uint16 *p_short;
	static char destip[4];
	int t,i;

	char tmp_urlbuf[100];

	gwdns_query = 1;
	memset(destip,0x00,4);
	memset((void *)&header,0,sizeof(struct dnsheader));
//	memset(g_buf,0,sizeof(g_buf));
	memset(tmp_urlbuf, 0, 100);
	strcpy(tmp_urlbuf, url);
	
	header.id = dss_htons(0x01);
	header.rd = 1;
	header.qdcount = dss_htons(0x01);
	
	data_buf  = (byte *)send_dnsbuf;
//	data_buf  = (byte *)sorted_groupname;
//	memset(data_buf,0,4000);
	memset(data_buf,0,100);
	memcpy(data_buf,(void *)&header,sizeof(struct dnsheader));
	pos = sizeof(struct dnsheader);
	
    namelen = strlen(url);
	name    = tmp_urlbuf;
	next    = getnext_delimeter(name,'.');
    
	while(1)
	{
		len = next - name;
		data_buf[pos++] = len;
		strncpy((char *)(data_buf + pos),(const char *)name, len);
		pos += len;
		if (*next == 0x00)
		{
			break;		
		}
		name  = next + 1;
		next = getnext_delimeter(name,'.');		
	}

	data_buf[pos] = 0x00;
	pos = pos+1;

    p_short =(uint16*)(data_buf + pos);
	
	if (((pos-sizeof(struct dnsheader))%2) == 0)
	{
		*p_short++ = dss_htons(0x01);	// Query Type: 1 = IP address
		*p_short   = dss_htons(0x01);	// Query Class: 1 = Internet Address
	}
	else
	{
		*p_short++ = 0x01;	// Query Type: 1 = IP address
		*p_short   = 0x01;	// Query Class: 1 = Internet Address
	}
	pos+=4;


	sockfd = g_sock_fd;
	memset( (char *) &toaddr, 0, sizeof(toaddr) );
	toaddr.sin_family = AF_INET;
//	toaddr.sin_addr.s_addr = dss_htonl(secondaryIP);
	toaddr.sin_addr.s_addr = dss_htonl(primaryIP);
//	toaddr.sin_addr.s_addr = primaryIP;

	toaddr.sin_port = dss_htons(53);
	flags = 0;
//	nbytes = 4000;

//	MSG_HIGH("pos[%d], port[%d]", pos, toaddr.sin_port, 0);

	bytes_per_write = dss_sendto( sockfd, 
								  (byte *)data_buf,
//								  nbytes,
								  pos,
//								  32,
								  flags,
								  (struct sockaddr *)&toaddr,
								  sizeof(struct sockaddr),
								  &errno
								);
	if( bytes_per_write == DSS_ERROR ) {
		MSG_HIGH("dss_sendto(dnsquery) error[%d]", errno, 0, 0);
	} else {
		MSG_HIGH("dss_sendto(dnsquery) bytes[%d]", bytes_per_write, 0, 0);
	}
}



void getdnsip(unsigned char *dnsdata, unsigned char *result)
{
	int pos,offset;
	char g_buf[80];
	int length;
	char *data_ptr;


	if (((struct dnsheader*)dnsdata)->rcode != 0x00) {
		MSG_HIGH("rcode error %x",((struct dnsheader*)dnsdata)->rcode,0,0);
	} if (((struct dnsheader*)dnsdata)->ancount < 1 ) {
		MSG_HIGH("answer length error %d",((struct dnsheader*)dnsdata)->ancount,0,0);
	}

	pos = sizeof(struct dnsheader);
	MSG_HIGH("HERE8",0,0,0);
	//일단 보낸 domain name과 같을 것이므로 생략
	//Question field
	if(((struct dnsheader*) dnsdata)->ancount < 1  ) 
	{
		MSG_HIGH("UDP fail",0,0,0);
	}
	else
	{
		while( dnsdata[pos] )
			pos += dnsdata[pos] + 1;
	
		pos++;
		pos += 2;	/* QTYPE */
		pos += 2;	/* QCLASS */
				
		data_ptr = g_buf;
		//Response field557

		if ( (dnsdata[pos] & 0xc0) == 0xc0 )
		{
			/* Message Compress */
			MSG_ERROR("Message Compress1",0,0,0);
			offset = ((dnsdata[pos] & 0x3f) << 8) + dnsdata[pos + 1];
			pos += 2;
			while( dnsdata[offset] )
			{
				strncpy((char *)data_ptr,(const char *)(dnsdata + offset + 1), dnsdata[offset]);			
				data_ptr += dnsdata[offset];
				offset += dnsdata[offset] + 1;
				if (dnsdata[offset])
					*data_ptr++ = '.';
			}
			*data_ptr = 0x00;
		}
		else
		{
			while( dnsdata[pos] )
			{
				MSG_ERROR("No Message Compress %d",dnsdata[pos],0,0);
				strncpy((char *)data_ptr,(const char *)(dnsdata + pos + 1), dnsdata[pos]);			
				data_ptr += dnsdata[pos];
				pos += dnsdata[pos] + 1;
				if (dnsdata[pos])				
					*data_ptr++ = '.';
				if ( (dnsdata[pos] & 0xc0) == 0xc0 )
				{
					/*Message Compress2 modified jmpark 000321*/
					MSG_ERROR("Message Compress2",0,0,0);
					offset = ((dnsdata[pos] & 0x3f) <<8 + dnsdata[offset] );
					pos += 2;
					while( dnsdata[offset] )
					{
						strncpy((char *)data_ptr, (const char *)(dnsdata+offset+1),dnsdata[offset]);
						data_ptr += dnsdata[offset];
						offset += dnsdata[offset] +1;
						if(dnsdata[offset])
							*data_ptr++ = '.';
						break;
					}
					*data_ptr = 0x00;
				}//if
			}//while  
			*data_ptr = 0x00;
		}
		pos+=8;//type/class/TTL
		//length field
		//고칠것 
		length =0;
		//length = ntohs(*((uint16*) (dnsdata + pos)));
		length = 4;
		//length   = dss_ntohs(((uint16*)&dnsdata[pos])[0]);
		pos+=2;
		memcpy(result,dnsdata+pos,length);
	}
}

void received_request()
{
	sint15 sockfd;
	byte *buffer;
	sint15 nbytes;
	uint16 flags;
	struct sockaddr_in fromaddr;
	uint16 addrlen;
	sint15 errno;
	int i=0;

	sint15 bytes_per_read;

	unsigned char daddr[4];     // Local address Ip_addr
	uint16 dport;           // Local port

	unsigned char saddr[4];          // WAP gateway address
	uint16 sport;                // WAP gateway port

	uint32 tmp_addr; 
	unsigned char tmp_dns[5]={0,0,0,0,0};
	unsigned char *ptmpstr;
	


	// Initialize the parameter of dss_recvfrom
	sockfd = g_sock_fd;
	buffer = (byte *)backsorted_groupname;

	nbytes = 4000;     // 4000 bytes may be enough ?
    flags  = 0;        // flags are unsupported 
	memset ( (char *) &fromaddr, 0, sizeof(fromaddr) );

	// Read socket
	bytes_per_read = dss_recvfrom( sockfd,
		                           (byte *)buffer,
								   nbytes,
								   flags,
								   (struct sockaddr *)&fromaddr,
								   &addrlen,
								   &errno
								  );

	// Check result and send to client
	if( bytes_per_read != DSS_ERROR ) {
		MSG_HIGH("dss_recvfrom bytes[%d]", bytes_per_read, 0, 0);

		/* Enable DS_READ_EVENT again */
		if (dss_async_select((sint15)g_sock_fd, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("dss_async_select(DS_READ_EVENT) error %d", errno, 0, 0);
		}		

		/* make destination address : local address */
//		tmp_addr = dss_htonl(Ip_addr);
		tmp_addr = Ip_addr;
		ptmpstr = daddr;
		(void)put32(ptmpstr,tmp_addr);

//	MSG_HIGH("dest %d.%d.%d",daddr[0],daddr[1],daddr[2]);
//	MSG_HIGH("dest %d",daddr[3],0,0);



		/* make destination port : local port */
		dport = SAport;
//		MSG_HIGH("dest port %d", dport, 0, 0);

		/* make source address : wap gateway address */
		tmp_addr = dss_htonl(fromaddr.sin_addr.s_addr);
		ptmpstr = saddr;
		(void)put32(ptmpstr,tmp_addr);

//	MSG_HIGH("src %d.%d.%d",saddr[0],saddr[1],saddr[2]);
//	MSG_HIGH("src %d",saddr[3],0,0);

        /* make source port : wap gateway port */
		sport = dss_htons(fromaddr.sin_port);
//		MSG_HIGH("src port %d", sport, 0, 0);

		if ( gwdns_query ) {
			gwdns_query = 0;
			/* result from dns query for WAP gateway domain name */
			getdnsip(buffer, (unsigned char*)tmp_dns); 

			CLNTc_setStrConfig(0, configUDP_IP_SRC, (const CHAR *)daddr, 4);
			CLNTc_run();
			CLNTc_setStrConfig(0, configUDP_IP_GW, (const CHAR *)tmp_dns, 4);
			CLNTc_run();
#ifndef FEATURE_WAP
			CLNTc_initialised(); 
#endif //#ifndef FEATURE_WAP
			CLNTc_run();
			MMIc_loadURL(1, "http://wap.n-top.com",0);
			CLNTc_run();
			MSG_HIGH("GW dns query fin %d.%d.%d", tmp_dns[0], tmp_dns[1], tmp_dns[2]);
			MSG_HIGH("GW dns query fin %d", tmp_dns[3], 0, 0);
		} else {
			/* receive request to client */
			UDPc_receivedRequest((const CHAR*)buffer,bytes_per_read,(const CHAR*)daddr,4,(const CHAR*)saddr,4,dport,sport);	
			MSG_HIGH("UDPc_receivedRequest called", 0,0,0);
		}
	}else if ( bytes_per_read == DSS_ERROR && errno == DS_EWOULDBLOCK ) {
		MSG_HIGH("dss_recvfrom DS_EWOULDBLOCK %d", errno, 0, 0);
		/* Enable DS_READ_EVENT */
		if (dss_async_select((sint15)sockfd, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
		{
			MSG_HIGH("dss_async_select(DS_READ_EVENT) error %d", errno, 0, 0);
		}
	} else {
		MSG_HIGH("dss_recvfrom error %d", errno, 0, 0);
	}

}

int wap_connect()
{
	sint15 errno, ret;

/* step 1 : client start */
	wap_activate();

	set_bearer_state(BEARER_INIT_S);

	CLNTa_log(1,0,"wap_connect 시작");

/* step 2 : dss_open_netlib */
	g_apID = dss_open_netlib(&net_callback_fcn, &sock_callback_fcn, &errno); 

	if(g_apID == DSS_ERROR) {
		MSG_HIGH("dss_open_netlib FAIL errno[%d]", errno,0,0);
		return -1;	
	}

	CLNTa_log(1,0,"socket network library 시작");

/* step 3 : open socket */
	g_sock_fd = dss_socket(g_apID, AF_INET, SOCK_DGRAM, IPPROTO_UDP, &errno);

	if(g_sock_fd == DSS_ERROR) {
		MSG_HIGH("dss_socket FAIL errno[%d]", errno,0,0);
		return -1;
	}

	CLNTa_log(1,0,"socket 생성");
	/* Enable DS_READ_EVENT at first*/
//	if (dss_async_select((sint15)g_sock_fd, (sint31)DS_READ_EVENT, &errno)==DSS_ERROR)
//	{
//		MSG_HIGH("dss_async_select(DS_READ_EVENT) error %d", errno, 0, 0);
//	}	


/* step 4 : ppp open */
	ret = dss_pppopen(g_apID, &errno);
	CLNTa_log(1,0,"during PPP open");

	if ( ret == DSS_ERROR ) {
		if( errno == DS_EWOULDBLOCK) { // waiting until ppp open completed
			CLNTa_log(1,0,"during PPP ewouldblock");
		    set_bearer_state(BEARER_PPPOPENING_S);
			return 0;
		} else {                       // ppp open fail
			return -1;
		}
	} else if ( ret == DSS_SUCCESS ) { // PPP already open
		CLNTa_log(1,0,"during PPP success");
		set_bearer_state(BEARER_PPPOPEN_S);
		return 0;
	}

}

void wap_disconnect()
{
	sint15 errno;
	sint15 ret;

	switch(get_bearer_state())
	{
		case BEARER_PPPOPENING_S:	
		case BEARER_PPPOPEN_S:
//			CLNTc_closePort(SAport);
			ret = dss_pppclose(g_apID, &errno);
			if(ret == DSS_SUCCESS)
			{
				ret = dss_close(g_sock_fd, &errno);
				if( ret == DSS_SUCCESS ) {
					MSG_HIGH("dss_close success", 0,0,0);
				} else {
					MSG_HIGH("dss_close fail [%d]", errno,0,0);
				}
				ret = dss_close_netlib(g_apID, &errno);
				if( ret == DSS_SUCCESS ) {
					MSG_HIGH("dss_close_netlib success", 0,0,0);
				} else {
					MSG_HIGH("dss_close_netlib faile [%d]", errno,0,0);
				}
				set_bearer_state(BEARER_INIT_S);
				wap_deactivate();
			}
			else
			{
				set_bearer_state(BEARER_CLOSING_S);
			}
			break;
		case BEARER_CLOSE_S:
			ret = dss_close(g_sock_fd, &errno);
			if( ret == DSS_SUCCESS ) {
				MSG_HIGH("dss_close success", 0,0,0);
			} else {
				MSG_HIGH("dss_close fail [%d]", errno,0,0);
			}
			ret = dss_close_netlib(g_apID, &errno);
			if( ret == DSS_SUCCESS ) {
				MSG_HIGH("dss_close_netlib success", 0,0,0);
			} else {
				MSG_HIGH("dss_close_netlib faile [%d]", errno,0,0);
			}
			set_bearer_state(BEARER_INIT_S);
			wap_deactivate();
			break;
		default:
			MSG_ERROR("Invalid curruent network state", 
					  0, 0, 0);
			break;
	}
}
extern wap_display_exit_message(void);
extern void main_HandleKeys(int nKey);
void wap_get_cmd( wap_cmd_type *cmd_ptr )
{
	int ret;

	switch (cmd_ptr->hdr.cmd) {
		case WAP_CONNECT:
		{
			MSG_HIGH("WAP_TASK : recv WAP_CONNECT cmd",0,0,0);
			ret = wap_connect();
			break;
		}
		
		case WAP_DISCONNECT:
		{
			MSG_HIGH("WAP_TASK : recv WAP_DISCONNECT cmd",0,0,0);
			wait_ani_stop();
			wap_display_exit_message();
			wap_disconnect();
			break;
		}

		case CLNT_USE_PORT:
		{
#ifndef FEATURE_WAP
			CLNTc_usePort(cmd_ptr->use_port.viewId,cmd_ptr->use_port.id,cmd_ptr->use_port.port,
				  		  cmd_ptr->use_port.success);		
#endif //#ifndef FEATURE_WAP
			break;
		}

		case WAP_KEY_EVENT:
		{
			MSG_HIGH("WAP_TASK : get event %d",cmd_ptr->key_event.key_event,0,0);
			main_HandleKeys(cmd_ptr->key_event.key_event);
			break;
		}

		default:
			break;
	 }

}


#endif // Not SOCKET_AUWAP
