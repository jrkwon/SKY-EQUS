#ifndef WAPCNCTR_H
#define WAPCNCTR_H

/*===========================================================================

           Wireless Applicatin Protocol Connector Function Header
		   
DESCRIPTION

           This is a header file for wap connector function structure
=============================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/10/99   swjung  Define connector function structure, its member items.
============================================================================*/
#include "rex.h"
#include "queue.h"

#include "tapicmmn.h"
#include "tapimem.h"
#include "capiclnt.h"
#include "clnt.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "tapiclnt.h"
#endif


/* 
** Commands enumeration to be sent to the WAP TASK 
** CLIENT -> MMI -> MEMORY -> USSD -> SMS -> UDP order
*/
typedef enum 
{
	/* CLIENT API */
    CLNT_TERMINATE,                    
	CLNT_FILE,                   
	CLNT_GET_CONTENT,
	CLNT_SET_INT_CONFIG,
	CLNT_SET_STR_CONFIG,
//jjkyg 2000/06/07
	CLNT_USE_PORT,
	CLNT_CLOSE_PORT,
#ifdef USE_PROPRIETARY_WMLS_LIBS
	CLNT_WMLS_RESPONSE,
#endif //#ifdef USE_PROPRIETARY_WMLS_LIBS

    /* MMI API */
    MMI_OPEN_VIEW,                    
	MMI_CLOSE_VIEW,                   
	MMI_LOAD_URL,                     
    MMI_RELOAD,                       
	MMI_STOP,                         
	MMI_GO_BACK,                      
	MMI_PASSWORD_DIALOG_RESPONSE,     
	MMI_PROMPT_DIALOG_RESPONSE,       
	MMI_CONFIRM_DIALOG_RESPONSE,      
	MMI_ALERT_DIALOG_RESPONSE,        
	MMI_KEY_SELECTED,                 
	MMI_TEXT_SELECTED,                
	MMI_IMAGE_SELECTED,               
	MMI_OPTION_SELECTED,              
	MMI_INPUT_STRING,                 
	MMI_LINKINFO,                     

	/* MEMORY API */
	MEM_INIT_CACHE,                   
	MEM_PREPARE_CACHE,                

	/* USSD API */
	USSD_RECEIVED_RESULT_PROCESS_REQUEST,    
	USSD_RECEIVED_ERROR,                    
	USSD_RECEIVED_INVOKE_REQUEST,           
	USSD_RECEIVED_RELEASE,                  

	/* SMS API */
	SMS_SENT_REQUEST,                  
    SMS_RECEIVED_REQUEST,               
	SMS_RECEIVED_ERROR,                 

	/* UDP API */
	UDP_RECEIVED_REQUEST,              
	UDP_ERROR_REQUEST,                
	WAP_TERMINATE_COMP,
	WAP_BROWSER_START
	
	/*Adapter event*/
	,WAP_MMI_WAIT
	,WAP_NEW_CARD
	,WAP_SHOW_CARD
	,WAP_CLNT_CONTENT
	
	/*key event type*/
	,WAP_STOP_EVENT
	,WAP_GOBACK_EVENT
	,WAP_TEMINATE_EVENT
	,WAP_DRAW_EVENT
	,WAP_DRAW_STOP_EVENT

	/*Network event*/
	,WAP_START_NETWORK
#ifdef WAP_USE_SECURE
	,WAP_SECURE_CMD
#endif//#ifdef WAP_USE_SECURE
#ifdef SOCKET_AUWAP
	,WAP_CONNECT
	,WAP_DISCONNECT
	,WAP_START
	,WAP_KEY_EVENT
#endif //SOCKET_AUWAP
    /* jrkwon 2001.05.04 --> display for sub LCD */
    ,
    WAP_SLCD_DISP_TIME,
    WAP_SLCD_DISP_ANNUN,
    /* <-- */
    WAP_NAME_MAX
} wap_name_type;

/* 
** Statuses from the WAP task, in command buffer 
*/
typedef enum
{
	WAP_DONE_S,                      /* General OK return */
	WAP_BUSY_S,                      /* Command is being processed */
	WAP_BADCMD_S,                    /* Invalid command */
	WAP_BADPARM_S                    /* Bad Parameters */
} wap_status_type;

/*
** Structure definition for wap header type
*/
typedef struct 
{
	q_link_type        link;              /* Queue link */
	wap_name_type      cmd;               /* Command to execute */
	rex_tcb_type       *task_ptr;         /* Pointer to requesting task TCB */
	rex_sigs_type      sigs;              /* Signal to set upon cmd completion */
	wap_status_type    status;            /* Completion code status */
	q_type             *done_q_ptr;       /* Queue to place this cmd on when done */
} wap_hdr_type;


/*
** Structure definitions for connector functions 
** -------------------------------------------
** CLNT   Connector function structures(05)
** MMI    Connector function structures(16)
** MEMORY Connector function structures(02)
** USSD   Connector function structures(04)
** SMS    Connector function structures(03)
** UDP    Connector function structures(03)
** -------------------------------------------
** TOTAL  Connector function structures(33)
** -------------------------------------------
*/

/* 
** CLIENT Connector function structure(03)
*/

typedef struct 
{
    wap_hdr_type     hdr;
} terminate_end_type;

typedef struct 
{
    wap_hdr_type     hdr;
} clnt_terminate_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            fileId;
    CHAR			 *data;
    UINT16           length;
	const CHAR		 *contentType;
} clnt_file_type;

typedef struct 
{
    wap_hdr_type     hdr;
	const CHAR		 *url;
	UINT8            urlId;
    BOOL			 isOTAallowed;
} clnt_get_content_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
	ConfigInt        kind;
	UINT32  		 value;
} clnt_set_int_config_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
	ConfigStr        kind;
	const CHAR		 *value;
	UINT8            length;
} clnt_set_str_config_type;

//jjkyg 2000/06/07
typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
	UINT8            id;
	UINT16           port;
	BOOL             success;
} clnt_use_port_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT16           port;
} clnt_close_port_type;

#ifdef USE_PROPRIETARY_WMLS_LIBS

typedef struct 
{
    wap_hdr_type     hdr;
	UINT16           invokeId;
	WMLSvar          *pWMLSvar;
} clnt_wmls_response_type;
#endif //#ifdef USE_PROPRIETARY_WMLS_LIBS

/* 
** MMI Connector function structure(16)
*/
typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
	UINT8            uamode;
} mmi_open_view_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
} mmi_close_view_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
    const CHAR       *url;
    BOOL             reload;
} mmi_load_url_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
} mmi_reload_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
} mmi_stop_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
} mmi_go_back_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
	UINT8            dialogId;
	const CHAR       *name;
	const CHAR       *password;
} mmi_password_dialog_response_type;

typedef struct 
{
    wap_hdr_type     hdr;
	UINT8            viewId;
	UINT8            dialogId;
	const WCHAR      *answer;
} mmi_prompt_dialog_response_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            viewId; 
	UINT8            dialogId; 
	BOOL             answer;
} mmi_confirm_dialog_response_type;

typedef struct
{
    wap_hdr_type     hdr;
	UINT8            viewId; 
	UINT8            dialogId;
} mmi_alert_dialog_response_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            viewId; 
	UINT8            keyId;
} mmi_key_selected_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            viewId;  
	UINT8            textId;
} mmi_text_selected_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            viewId;  
	UINT8            imageId;
} mmi_image_selected_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            viewId; 
	UINT8            optionId;
} mmi_option_selected_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            viewId;  
	UINT8            inputId; 
	const WCHAR      *text;
} mmi_input_string_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            viewId; 
	UINT8            widgetType; 
	UINT8            widgetId;
} mmi_linkinfo_type;

/* 
** MEMORY Connector function structure(02)
*/
 
typedef struct
{
	wap_hdr_type     hdr;
	memSizeType      cacheSize; 
	memSizeType      restoredSize;
} mem_init_cache_type;

typedef struct
{
	wap_hdr_type     hdr;
	memSizeType      availablePersistentMemory; 
} mem_prepare_cache_type;

/*
** USSD   Connector function structure(04)
*/
typedef struct
{
	wap_hdr_type     hdr;
	UINT8            dataCodingScheme;
	const CHAR       *data; 
	UINT8            stringLength;
} ussd_received_result_process_request_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            message; 
	BOOL             isKept;
} ussd_received_error_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            dataCodingScheme; 
	const CHAR       *data;
	UINT8            stringLength;
} ussd_received_invoke_request_type;

typedef struct
{
	wap_hdr_type     hdr;
} ussd_received_release_type;

/*
** SMS    Connector function structure(03)
*/
typedef struct
{
	wap_hdr_type     hdr;
} sms_sent_request_type;

typedef struct
{
	wap_hdr_type     hdr;
	const CHAR       *source; 
	UINT8            sourceLength; 
	const CHAR       *data; 
	UINT8            dataLength;
} sms_received_request_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            message;
} sms_received_error_type;

/*
** UDP    Connector function structure(03)
*/
typedef struct
{
	wap_hdr_type     hdr;
} udp_received_request_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8            message;
	UINT16           destinationPort;
} udp_error_request_type;

/* Adapter part */
//MMIa_wait
typedef struct
{
	wap_hdr_type     hdr;
	unsigned char  viewId;
	char  isWait;
}wap_mmi_wait_type;

//MMIa_newcard
typedef struct
{
	wap_hdr_type     hdr;
	unsigned char viewId;
	char *title;
	char isList;
	char isRefresh;
	char *url;
	char isBookmarkable;
	const unsigned short * const *history;
}wap_mmi_new_card_type;

//mmi_showcard
typedef struct
{
	wap_hdr_type     hdr;
	unsigned char    viewId;
}wap_mmi_show_card_type;

typedef struct
{
	wap_hdr_type     hdr;
	UINT8 urlID;	
	UINT16 length;
}wap_clnt_content_type;

#ifdef SOCKET_AUWAP
typedef struct  // AUWAP
{
	wap_hdr_type     hdr;
	int              key_event;
} wap_key_event_type;
#endif //SOCKET_AUWAP

/*
** Union definition for wap command type
** wap_cmd_type represents the kind of 
** connector function to be called by WAP task
*/
typedef union {
    wap_hdr_type                              hdr;

	terminate_end_type                        end;

	/* CLNT API(05) */
	clnt_terminate_type						  terminate;
	clnt_file_type							  file;
	clnt_get_content_type					  get_content;
	clnt_set_int_config_type				  set_int_config;
	clnt_set_str_config_type				  set_str_config;

	//jjkyg 2000/06/07
	clnt_use_port_type                        use_port;
	clnt_close_port_type					  close_port;
#ifdef USE_PROPRIETARY_WMLS_LIBS
	clnt_wmls_response_type					  wmls_response;
#endif //#ifdef USE_PROPRIETARY_WMLS_LIBS

	/* MMI API(16) */
    mmi_open_view_type                        open_view;
	mmi_close_view_type                       close_view;
	mmi_load_url_type                         load_url;
    mmi_reload_type                           reload;
	mmi_stop_type                             stop;
	mmi_go_back_type                          go_back;
	mmi_password_dialog_response_type         password;
	mmi_prompt_dialog_response_type           prompt;
	mmi_confirm_dialog_response_type          confirm;
	mmi_alert_dialog_response_type            alert;
	mmi_key_selected_type                     key;
	mmi_text_selected_type                    text;
	mmi_image_selected_type                   image;
	mmi_option_selected_type                  option;
	mmi_input_string_type                     input;
	mmi_linkinfo_type                         linkinfo;

    /* MEMORY API */
	mem_init_cache_type                       init_cache;
	mem_prepare_cache_type                    prepare_cache;

	/* USSD API */
	ussd_received_result_process_request_type ussd_r_result;
	ussd_received_error_type                  ussd_r_error;
	ussd_received_invoke_request_type         ussd_r_invoke;
	ussd_received_release_type                ussd_r_release;

	/* SMS API */
	sms_sent_request_type                     sms_s_request;
    sms_received_request_type                 sms_r_request;
	sms_received_error_type                   sms_r_error;

	/* UDP API */
	udp_received_request_type                 udp_r_request;
	udp_error_request_type                    udp_error;
	
	/* Adapter API */
	wap_mmi_wait_type                         mmi_wait;
	wap_mmi_new_card_type					  mmi_new;	
	wap_mmi_show_card_type					  mmi_show;
	wap_clnt_content_type                     clnt_type;  
#ifdef SOCKET_AUWAP
	/* AUWAP : send key event from UI to WAP task */
	wap_key_event_type						  key_event;
#endif //SOCKET_AUWAP
} wap_cmd_type;
#endif /* WAPCNCTR_H */