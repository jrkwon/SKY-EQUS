/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           Men Machine Interface Command Execution

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  
=============================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/25/99   swjung  Make functions for handling adapter functions
============================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "task.h"
#include "rex.h" 
#include "queue.h"
#include "ui.h"

#include "waptask.h"
#include "wapadapter.h"
#include "wapconnector.h"

//-----------------------------------------------------------------------------------
//
// MMI ÀÇ Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"

#ifdef WAP_USE_MAIL
extern q_type  ui_adapter_cmd_q; 
extern q_type  ui_adapter_cmd_free_q; 


/*===========================================================================

FUNCTION SEND_CMD_WAP2UI

DESCRIPTION
  
DEPENDENCIES
  
RETURN VALUE
  None.
SIDE EFFECTS



===========================================================================*/
void send_cmd_wap2ui ( ui_cmd_type *cmd_ptr )
{
  (void) q_link( cmd_ptr, &cmd_ptr->hdr.link );     /* init link */
  q_put( &ui_cmd_q, &cmd_ptr->hdr.link );   /* and queue it */
  (void) rex_set_sigs( &ui_tcb, UI_CMD_Q_SIG ); /* signal the WAP task */
  CLNTa_log(0,0,"\n-----[send_cmd_wap2ui]---");
}


/*===========================================================================

FUNCTION MAKE_CMD_WAP2UI

DESCRIPTION
  

DEPENDENCIES


RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
void make_cmd_wap2ui( ui_cmd_type *msg_ptr )
{
  ui_cmd_type	*item_ptr;  /* Temp pointer to the item in the queue */
  q_link_type			link;       /* Save link from being written over     */

  if (( item_ptr = (ui_cmd_type*)q_get( &ui_cmd_free_q )) != NULL) {

    link = item_ptr->hdr.link;
    
	*item_ptr = *msg_ptr;

    item_ptr->hdr.link = link;
    
	item_ptr->hdr.task_ptr = NULL;
    
	item_ptr->hdr.done_q_ptr = &ui_cmd_free_q;

    send_cmd_wap2ui( item_ptr );
  }
  else {
    ERR_FATAL( "make_cmd_wap2ui: Free queue is EMPTY!", 0, 0, 0 );
  }
} /* end make_cmd_wap2ui() */
#endif //#ifdef WAP_USE_MAIL


/*===========================================================================

FUNCTION UI_GET_CMD()

DESCRIPTION
  Check event and call mmi application function correspond to mmi adapter function

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/

void ui_get_cmd( ui_cmd_type *cmd_ptr )
{
#ifdef BLOCK
	switch (cmd_ptr->hdr.cmd) 
	{

		case MMI_WAIT:
		{
			SKY_wait (cmd_ptr->wait.viewId,
					  cmd_ptr->wait.isWait);
			break;
		}

		case MMI_STATUS:		
		{
			SKY_status (cmd_ptr->status.viewId,
						cmd_ptr->status.status,
						cmd_ptr->status.URL);
			break;
		}

		case MMI_UNKNOWN_CONTENT:		
		{
			SKY_unknownContent (cmd_ptr->unknown_content.viewId,
								cmd_ptr->unknown_content.data,
								cmd_ptr->unknown_content.length,
								cmd_ptr->unknown_content.contentType,
								cmd_ptr->unknown_content.URL);
			break;	
		}
		case MMI_PASSWORD_DIALOG:
		{
			SKY_passwordDialog (cmd_ptr->password_dialog.viewId,
								cmd_ptr->password_dialog.dialogId,
								cmd_ptr->password_dialog.realm,
								cmd_ptr->password_dialog.type);
			break;
		}
		case MMI_PROMPT_DIALOG:
		{
			SKY_promptDialog (  cmd_ptr->prompt_dialog.viewId,
								cmd_ptr->prompt_dialog.dialogId,
								cmd_ptr->prompt_dialog.message,
								cmd_ptr->prompt_dialog.defaultMessage);
			break;
		}
		case MMI_CONFIRM_DIALOG:
		{
			SKY_confirmDialog ( cmd_ptr->confirm_dialog.viewId,
								cmd_ptr->confirm_dialog.dialogId,
								cmd_ptr->confirm_dialog.message,
								cmd_ptr->confirm_dialog.ok,
								cmd_ptr->confirm_dialog.cancel);
			break;
		}
		case MMI_ALERT_DIALOG:
		{
			SKY_alertDialog (	cmd_ptr->alert_dialog.viewId,
								cmd_ptr->alert_dialog.dialogId,
								cmd_ptr->alert_dialog.message);
			break;	
		}
		case MMI_NEW_CARD:		
		{
			SKY_newCard(cmd_ptr->new_card.viewId,
						cmd_ptr->new_card.title,   
						cmd_ptr->new_card.isList,    
						cmd_ptr->new_card.isRefresh,
						cmd_ptr->new_card.URL,
						cmd_ptr->new_card.isBookmarkable,
						cmd_ptr->new_card.history);
		    break;
		}
		case MMI_SHOW_CARD:		
		{
			SKY_showCard(cmd_ptr->show_card.viewId);
			break;
		}
		case MMI_CANCEL_CARD:		
		{
			SKY_cancelCard(cmd_ptr->cancel_card.viewId);
			break;
		}
		case MMI_NEW_KEY:		
		{
			SKY_newKey (cmd_ptr->new_key.viewId,
						cmd_ptr->new_key.keyId,
						cmd_ptr->new_key.eventType,
						cmd_ptr->new_key.label,
						cmd_ptr->new_key.isOptional);
			break;
		}
		case MMI_NEW_TEXT:		
		{
			SKY_newText(cmd_ptr->new_text.viewId,
						cmd_ptr->new_text.textId,
						cmd_ptr->new_text.text,
						cmd_ptr->new_text.isLink,
						cmd_ptr->new_text.linkTitle,
						cmd_ptr->new_text.format);
			break;
		}
		case MMI_NEW_IMAGE:		
		{
			SKY_newImage (	cmd_ptr->new_image.viewId,
							cmd_ptr->new_image.imageId,
							cmd_ptr->new_image.imageData,
							cmd_ptr->new_image.imageSize,
							cmd_ptr->new_image.imageType,
							cmd_ptr->new_image.altText,
							cmd_ptr->new_image.localSrc,
							cmd_ptr->new_image.isLink,
							cmd_ptr->new_image.linkTitle,
							cmd_ptr->new_image.vSpace,
							cmd_ptr->new_image.hSpace,
							cmd_ptr->new_image.width,
							cmd_ptr->new_image.height,
							cmd_ptr->new_image.isPercent,
							cmd_ptr->new_image.align);
			break;	
		}
		case MMI_COMPLETE_IMAGE:		
		{
			SKY_completeImage ( cmd_ptr->complete_image.viewId,
								cmd_ptr->complete_image.imageId,
								cmd_ptr->complete_image.imageData,
								cmd_ptr->complete_image.imageSize,
								cmd_ptr->complete_image.imageType);
			break;
		}
		case MMI_NEW_PARAGRAPH:		
		{
			SKY_newParagraph (  cmd_ptr->new_paragraph.viewId,
								cmd_ptr->new_paragraph.align,
								cmd_ptr->new_paragraph.wrap);
			break;	
		}
		case MMI_CLOSE_PARAGRAPH:		
		{
			SKY_closeParagraph (cmd_ptr->close_paragraph.viewId);
			break;
		}
		case MMI_NEW_BREAK:		
		{
			SKY_newBreak (cmd_ptr->new_break.viewId);
			break;
		}
		case MMI_NEW_SELECT:		
		{
			SKY_newSelect (	cmd_ptr->new_select.viewId,
							cmd_ptr->new_select.title,
							cmd_ptr->new_select.multiSelect,
							cmd_ptr->new_select.tabIndex);
			break;	
		}
		case MMI_CLOSE_SELECT:		
		{
			SKY_closeSelect (cmd_ptr->close_select.viewId);
			break;	
		}
		case MMI_NEW_OPTION:		
		{
			SKY_newOption (	cmd_ptr->new_option.viewId,
							cmd_ptr->new_option.optionId,
							cmd_ptr->new_option.label,
							cmd_ptr->new_option.title,
							cmd_ptr->new_option.isSelected);
			break;	
		}
		case MMI_NEW_OPTION_GROUP:		
		{
			SKY_newOptionGroup (cmd_ptr->new_option_group.viewId,
								cmd_ptr->new_option_group.label);
			break;		
		}
		case MMI_CLOSE_OPTION_GROUP:		
		{
			SKY_closeOptionGroup (cmd_ptr->close_option_group.viewId);
			break;	
		}
		case MMI_NEW_INPUT:		
		{
			SKY_newInput (	cmd_ptr->new_input.viewId,
							cmd_ptr->new_input.inputId,
							cmd_ptr->new_input.title,
							cmd_ptr->new_input.text,
							cmd_ptr->new_input.isPassword,
							cmd_ptr->new_input.emptyOk,
							cmd_ptr->new_input.format,
					 		cmd_ptr->new_input.size,
							cmd_ptr->new_input.nChars,
							cmd_ptr->new_input.tabIndex);
			break;
		}
		case MMI_GET_INPUT_STRING:		
		{
			SKY_getInputString (cmd_ptr->get_input_string.viewId,
								cmd_ptr->get_input_string.inputId);
			break;
		}
		case MMI_LINK_INFO:		
		{
			SKY_linkInfo (  cmd_ptr->link_info.viewId,
							cmd_ptr->link_info.widgetType,
							cmd_ptr->link_info.widgetId,
							cmd_ptr->link_info.URL);
			break;
		}
		default:
			break;
	}
#endif //#ifdef BLOCK
}
