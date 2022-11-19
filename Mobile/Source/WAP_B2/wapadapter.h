#ifndef WAPADPTER_H
#define WAPADPTER_H

/*===========================================================================

           Wireless Applicatin Protocol Adapter Function Header
		   
DESCRIPTION

           This is a header file for wap adapter function structure
=============================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/10/99   swjung  Define adapter function structure, its member items.
============================================================================*/


#include "rex.h"
#include "queue.h"

#include "tapimem.h"
//#include "wapconnector.h"
//#include "tapicmmn.h"
//#include "aapiwta.h"
//typedef signed long		memSizeType;
 
typedef enum    WTAType
{
    wtaMakeCall = 101,  /*  Used when a call should be set up */
    wtaSendDTMF = 102   /*  Used when a tone sequence shall be sent.    */
} WTAType;

#ifdef WAP_USE_MAIL
/* 
** Commands enumeration to be sent to the WAP TASK for WAP Browser 
** CLNT -> MMI -> MEMORY -> WTA -> USSD -> SMS -> UDP order
*/
typedef enum 
{
	/* CLIENT API */
    CLNT_TERMINATED,                    
	CLNT_CURRENT_TIME,                   
	CLNT_SET_TIMER,                     
    CLNT_RESET_TIMER,                       
	CLNT_GET_FILE,                         
	CLNT_NONSUPPORTED_SCHEME,                      
	CLNT_CONTENT,     
	CLNT_CONFIRM_DOWNLOAD,       
	CLNT_ERROR,      
	CLNT_LOG, 
	
    /* MMI API */
    MMI_WAIT,                    
	//MMI_STATUS,                   
	MMI_UNKNOWN_CONTENT,                           
	MMI_PASSWORD_DIALOG,     
	MMI_PROMPT_DIALOG,       
	MMI_CONFIRM_DIALOG,      
	MMI_ALERT_DIALOG,        
	MMI_NEW_CARD,                 
	MMI_SHOW_CARD,                
	MMI_CANCEL_CARD,               
	MMI_NEW_KEY,              
	MMI_NEW_TEXT,                 
	MMI_NEW_IMAGE,   
	MMI_COMPLETE_IMAGE,        
	MMI_NEW_PARAGRAPH,                 
	MMI_CLOSE_PARAGRAPH,                
	MMI_NEW_BREAK,               
	MMI_NEW_TABLE,              
	MMI_NEW_TABLE_DATA,                 
	MMI_NEW_CLOSE_TABLE,
	MMI_NEW_FIELD_SET,        
	MMI_CLOSE_FIELD_SET,                 
	MMI_NEW_SELECT,                
	MMI_CLOSE_SELECT,               
	MMI_NEW_OPTION,              
	MMI_NEW_OPTION_GROUP, 
	MMI_CLOSE_OPTION_GROUP, 
	MMI_NEW_INPUT,  
	MMI_GET_INPUT_STRING,                 
	MMI_LINK_INFO,  


	/* WTA API */
	WTA_PUBLIC_WTAI,    

	/* MEMORY API */
	MEM_CACHE_PREPARED,                   
	MEM_READ_CACHE,                
	MEM_WRITE_CACHE,  
	
	/* USSD API */
	USSD_SEND_INVOKE_PROCESS_REQUEST,    
	USSD_SEND_RESULT_REQUEST,                    
	USSD_SEND_ABORT,                            

	/* SMS API */
	SMS_SEND_REQUEST,                  

	/* UDP API */
	UDP_SEND_REQUEST,              
	UDP_CLOSE_PORT                
	
} wap_adapter_name_type;




/*
** Structure definition for wap header type
*/
typedef struct 
{
	q_link_type                link;              /* Queue link */
	wap_adapter_name_type      cmd;               /* Command to execute */
	rex_tcb_type               *task_ptr;         /* Pointer to requesting task TCB */
	rex_sigs_type              sigs;              /* Signal to set upon cmd completion */
	wap_status_type            status;            /* Completion code status */
	q_type                     *done_q_ptr;       /* Queue to place this cmd on when done */
} wap_adapter_hdr_type;


/*
** Structure definitions for adapter functions 
** -------------------------------------------
** CLNT   adapter function structures(10)
** MMI    adapter function structures(30)
** MEMORY adapter function structures(03)
** USSD   adapter function structures(03)
** SMS    adapter function structures(01)
** UDP    adapter function structures(02)
** -------------------------------------------
** TOTAL  adapter function structures(49)
** -------------------------------------------
*/

typedef struct 
{
    ui_hdr_type     hdr;
} clnt_terminated_type;

typedef struct 
{
    ui_hdr_type     hdr;
} clnt_current_time_type;

typedef struct 
{
    ui_hdr_type     hdr;
	unsigned char					 timerId;
    unsigned long                   timeInterval;
} clnt_set_timer_type;

typedef struct 
{
    ui_hdr_type     hdr;
	unsigned char					 timerId;
} clnt_reset_timer_type;

typedef struct 
{
    ui_hdr_type     hdr;
	unsigned char				     fileId;
    const char				 *URL;
} clnt_get_file_type;

typedef struct 
{
    ui_hdr_type     hdr;
	unsigned char					 viewId;
	const char				 *URL;
} clnt_nonsupported_scheme_type;

typedef struct 
{
    ui_hdr_type     hdr;
	unsigned char					 urlID;
	const char				 *data;
	unsigned short					 length;
	char					 *contentType;
	signed short					 errorNo;
} clnt_content_type;

typedef struct 
{
    ui_hdr_type     hdr;
	unsigned short					 confirmId;
} clnt_confirm_download_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	signed short					 errorNo; 
	unsigned char					 errorType;
} clnt_error_type;

typedef struct
{
    ui_hdr_type     hdr;
	unsigned char					 viewId; 
	signed short					 logNo;
	const char				 *format;
} clnt_log_type;


/*
** MMI API
*/
typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	char					 isWait;
} mmi_wait_type;


typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	unsigned char					 status; 
	const char				 *URL;
} mmi_status_type;



typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	const char				 *data; 
	unsigned short					 length; 
	const char				 *contentType;
	const char				 *URL;
} mmi_unknown_content_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	unsigned char					 dialogId; 
	const char				 *realm; 
	signed char					 type;
} mmi_password_dialog_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	unsigned char					 dialogId;
	const unsigned short				 *message;
	const unsigned short				 *defaultMessage;
} mmi_prompt_dialog_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	unsigned char					 dialogId;
	const unsigned short				 *message;
	const unsigned short				 *ok; 
	const unsigned short				 *cancel;
} mmi_confirm_dialog_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	unsigned char					 dialogId;
	const unsigned short				 *message;
} mmi_alert_dialog_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	const unsigned short				 *title;
	char					 isList;
	char					 isRefresh;
	const char				 *URL;
	char					 isBookmarkable;
	const unsigned short		* const *history;
} mmi_new_card_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;  
} mmi_show_card_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
} mmi_cancel_card_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	unsigned char					 keyId;
	const unsigned short				 *eventType;
	const unsigned short				 *label;
	char					 isOptional;
} mmi_new_key_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	unsigned char					 textId;
	const unsigned short				 *text;
	char					 isLink;
	const unsigned short				 *linkTitle;
	signed char					 format;
} mmi_new_text_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	unsigned char					 imageId;
	const char				 *imageData;
	unsigned short					 imageSize;
	const char				 *imageType;
	const unsigned short				 *altText;
	const unsigned short				 *localSrc;
	char					 isLink;
	const unsigned short				 *linkTitle;
	signed char					 vSpace;
	signed char					 hSpace;
	signed short					 width;
	signed short					 height;
	signed char					 isPercent;
	signed char					 align;
} mmi_new_image_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	unsigned char					 imageId;
	const char				 *imageData;
	unsigned short					 imageSize;
	const char				 *imageType;
} mmi_complete_image_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	signed char					 align;
	char					 wrap;
} mmi_new_paragraph_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
} mmi_close_paragraph_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;  
} mmi_new_break_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	const unsigned short				 *title;
	signed char					 noOfColumns;
	const char				 *align;
} mmi_new_table_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;  
} mmi_new_table_data_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;  
} mmi_new_close_table_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	const unsigned short				 *title;
} mmi_new_field_set_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;  
} mmi_close_field_set_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	const unsigned short				 *title;
	char					 multiSelect;
	signed char					 tabIndex;
} mmi_new_select_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
} mmi_close_select_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	unsigned char					 optionId;
	const unsigned short				 *label;
	const unsigned short				 *title;
	char					 isSelected;
} mmi_new_option_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	const unsigned short				 *label;
} mmi_new_option_group_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
} mmi_close_option_group_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId;
	unsigned char					 inputId;
	const unsigned short				 *title;
	const unsigned short				 *text;
	char					 isPassword;
	char					 emptyOk;
	const unsigned short				 *format;
	signed char					 size;
	signed char					 nChars;
	signed char					 tabIndex;
} mmi_new_input_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	unsigned char					 inputId;
} mmi_get_input_string_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 viewId; 
	unsigned char					 widgetType;
	unsigned char					 widgetId;
	const char				 *URL;
} mmi_link_info_type;



/* 
** WTA adapter function structure(01)
*/                    
typedef struct
{
	ui_hdr_type     hdr;
	WTAType					 operation; 
	char					 *argument;
} wta_public_wtai_type;

/* 
** MEMORY Connector function structure(03)
*/
 
typedef struct
{
	ui_hdr_type     hdr;
} mem_cache_prepared_type;

typedef struct
{
	ui_hdr_type     hdr;
	memSizeType				 pos; 
	memSizeType				 size;
	char					 *buffer;
} mem_read_cache_type;

typedef struct
{
	ui_hdr_type     hdr;
	memSizeType				 pos; 
	memSizeType				 size;
	const char				 *buffer;
} mem_write_cache_type;

            
/*
** USSD   Connector function structure(03)
*/
typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 dataCodingScheme;
	const char				 *data; 
	unsigned char					 stringLength;
} ussd_send_invoke_process_request_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned char					 dataCodingScheme;
	const char				 *data;
	unsigned char					 stringLength;
} ussd_send_result_request_type;

typedef struct
{
	ui_hdr_type     hdr;
} ussd_send_abort_type;


/*
** SMS    Connector function structure(01)
*/


typedef struct
{
	ui_hdr_type     hdr;
	const char				 *smsc;
	unsigned char					 smscLength; 
	const char				 *destination;
	unsigned char					 destLength;
	const char				 *data; 
	unsigned char					 dataLength;
} sms_send_request_type;


/*
** UDP    Connector function structure(02)
*/
typedef struct
{
	ui_hdr_type     hdr;
	const char				 *data; 
	unsigned short					 dataLength; 
	const char				 *destination; 
	unsigned char					 destLength; 
	const char				 *source; 
	unsigned char					 sourceLength; 
	unsigned short					 destinationPort; 
	unsigned short					 sourcePort;
} udp_send_request_type;

typedef struct
{
	ui_hdr_type     hdr;
	unsigned short					 sourcePort;
} udp_close_port_type;


/*
** Union definition for wap command type
** wap_cmd_type represents the kind of 
** adapter function to be called by WAP task
*/
typedef union {
    ui_hdr_type                      hdr;

    /* CLIENT API */
	clnt_terminated_type                      terminated;
	clnt_current_time_type                    current_time;
	clnt_set_timer_type                       set_timer;
    clnt_reset_timer_type                     reset_timer;
	clnt_get_file_type                        get_file;
	clnt_nonsupported_scheme_type             nonsupported_scheme;          
	clnt_content_type                         content;
	clnt_confirm_download_type                confirm_download;
	clnt_error_type                           error;
	clnt_log_type                             log;
	
    /* MMI API */
    mmi_wait_type                             wait;
	mmi_status_type                           status;
	mmi_unknown_content_type                  unknown_content;         
	mmi_password_dialog_type                  password_dialog;
	mmi_prompt_dialog_type                    prompt_dialog;
	mmi_confirm_dialog_type                   confirm_dialog;
	mmi_alert_dialog_type                     alert_dialog;
	mmi_new_card_type                         new_card;
	mmi_show_card_type                        show_card;
	mmi_cancel_card_type                      cancel_card;
	mmi_new_key_type                          new_key;
	mmi_new_text_type                         new_text;
	mmi_new_image_type                        new_image;
	mmi_complete_image_type                   complete_image;
	mmi_new_paragraph_type                    new_paragraph;    
	mmi_close_paragraph_type                  close_paragraph;
	mmi_new_break_type                        new_break;  
	mmi_new_table_type                        new_table;
	mmi_new_table_data_type                   new_table_data; 
	mmi_new_close_table_type                  new_close_table;
	mmi_new_field_set_type                    new_field_set;
	mmi_close_field_set_type                  close_filed_set;    
	mmi_new_select_type                       new_select;
	mmi_close_select_type                     close_select;
	mmi_new_option_type                       new_option;
	mmi_new_option_group_type                 new_option_group;  
	mmi_close_option_group_type               close_option_group;  
	mmi_new_input_type                        new_input;
	mmi_get_input_string_type                 get_input_string;     
	mmi_link_info_type                        link_info;
   
	/* WTA API */
	wta_public_wtai_type                      public_wtai;

	/* MEMORY API */
	mem_cache_prepared_type                   cache_prepared;
	mem_read_cache_type                       read_cache; 
	mem_write_cache_type                      write_cache;
	
	/* USSD API */
	ussd_send_invoke_process_request_type     ussd_s_invoke;
	ussd_send_result_request_type             ussd_s_result;       
	ussd_send_abort_type                      ussd_s_abort;      

	/* SMS API */
	sms_send_request_type                     sms_s_request;

	/* UDP API */
	udp_send_request_type                     udp_s_request;
	udp_close_port_type                       udp_close_port;   
} wap_adapter_cmd_type;
#endif //#ifdef WAP_USE_MAIL

#endif /* WAPADPTR_H */