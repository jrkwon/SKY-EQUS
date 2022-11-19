/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


      U S E R   I N T E R F A C E   S M S   L I S T   O P E R A T I O N

GENERAL DESCRIPTION
  Controls and manages the SMS lists of stored messages in NV.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

      Copyright (c) 1991-1997,1998 by QUALCOMM, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/msm3100/vcs/uiusmsl.c_v   2.4   17 Nov 1999 19:17:10   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/09/99   HQ      Reject page or text message if there is no VC message
                   currently stored and there is only one slot available.
04/27/98   skk     Fixed a problem where Blank line is shown in the old text message 
                   list screen for CPT messages with NO call back number. cr 4758
04/16/98   skk     Fixed a problem where SMS was storing a message without properly 
                   updating control list, resulting in that message disappearing and
                   a discrepency in NV memory size cr5085
                   Also fixed a problem where in certain cases an extra message is 
                   deleted when deleting duplicates.
10/27/97   jjn     Fixed bug (CR# 4263) which caused VMN SMS messages to be
                   always flagged as "old"
06/03/97   day     Delete extra declaration of same variables to be compliant with 
                         ANSI C standard.
06/02/97   day     Declare variables as static to be compliant with ANSI C standard.
05/22/97   SIU     Added function ui_sms_update_icons for envelope and 
                   V icon display. Updated Copyright statement.
04/22/97   BK      Regional naming convention for SMS var.
02/28/97   jjn     Disabled on-board ringer functionality for the module
                   because the module doesn't have an on-board ringer.
                   Also support DM packet that retrieves SMS messages
07/30/96   jgr     Added new sms buffers system, changes are ifdef NEW_SMS.
05/10/96   BK      Fixed another icon bug 1281.
05/09/96   BK      Fixed common icon bug, added means to track icon condition.
04/18/96   BK      New UI branching, created from uismsl.c version 1.18
04/01/96   BK      FEATURE_UI_ENVELOPE_VM_ICON added in.
01/03/96   CAH     AMPS Message Waiting Order and CDMA MWI Info records are
                     now converted to IS91 Voice Mail Notification and sent
                     to the SMS handler.
10/24/95   day     Added SMS control list save after new message is received.
10/20/95   day     Fixed call back number detection with no text for
                   messages/pages and automatic deletion of duplicate
                   messages/pages.
10/18/95   day     Added text match detection with matching call back
                   numbers or no call back numbers for messages/pages and 
                   automatic deletion of duplicate message/page.
10/10/95   day     Reuse user_data_length field in SMS control information
                   stored in SMS NV index 0 to indicate if there are
                   available SMS NV indicies to store SMS messages.  This
                   information is read in the MC task when processing SMS
                   messages and setting status in SMS Acknowledge message.
09/29/95   day     Added CDMA SMS message's duplicate detection processing.
09/21/95   day     Added ellipse mark for speed dial name longer than
                   can be displayed in scroll text.
09/20/95   day     Added alert reminder logic for unacknowledged 
                   notifications.
09/19/95   day     Change memcpy to memset calls for blanking screens.
09/13/95   day     Use #define value for urgent message check.  Added
                   auto-format routine for character strings.  Return any
                   unreadable SMS message to available SMS message list
                   when read error encountered.
09/06/95   day     Eliminate terminator box.  Blink voice icon if urgent.
                   Designate high priority to be urgent or emergency.
08/29/95   day     Return operational status after storing new messages.
08/22/95   day     Optimized SMS list structures to save RAM.
08/22/95   day     Added call back number speed dial name match for scroll
                   text.
08/22/95   day     Added call back number save and prepend functions.
08/10/95   day     Created

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"

#ifdef FEATURE_UASMS
#include "ska.h"

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/* List of different categories of SMS message lists. */
ui_sms_list_type ui_sms_list[MAX_SMS_LIST];

/* SMS NV list of message line text data */
byte ui_sms_text_list[MAX_NV_MSGS+1][UI_SMS_WIDE];

/* SMS NV list of CDMA message's duplicate */
/* detection data                          */
ui_sms_detect_list_type ui_sms_detect_list[MAX_NV_MSGS+1];

/* Used for tagging the latest SMS message received.   */
ui_sms_read_info_type ui_sms_read_info;

/* SMS list read operation information */
ui_sms_read_op_type sms_read;

/* Structure identifying the item to be */
/* written, read or freed from NV.      */
nv_sms_type ui_sms_nv_item;
BYTE      pFormatBuf[MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE + 1];
/* Define characters required for building */
/* first line of text used in message      */
/* scrolling.                              */
#define ELLIPSE             0xDB
#define URGENT              0x2A
#define SMS_MINIMUM_MEMORY  350
#define MAX_SMS_SIZE        40 // 20->40 huni Max Size 40

#define SEPERATE            0x0B
#define SEPERATE_SHINSEGI   0x1B

#ifdef FEATURE_SKTT_PM
#define PHOTOMAIL_URL       "mms.n-top.com/pmsd"        
#endif
/////////////////////////////////////////////////////
// Temp 
/////////////////////////////////////////////////////
typedef struct {                  /* editor buffer type */
  boolean act;                    /* active or not*/
  boolean secret;                 /* secret number or not */
  byte    buf[UI_MAX_DIALED_DIGITS]; /* the data */
  byte    pos;                    /* length of the data */
} ui_edit_type;

#ifdef FEATURE_SKVM

typedef struct {                  /* editor buffer type */
	CHAR LineBuf[80];
	BYTE Len;
} simple_line; 

#define MSD_SEPERATOR           0x0A
#endif

extern byte   b_unpackb ( byte buf[ ], word pos, word len );



void ui_sms_bcd_to_julian (smsi_abs_time_type mc_time, clk_julian_type *julian)
{
  byte text_ptr[2] ;
  /* Hours are given as 24 hour clock time */
  /* Translate BCD hours to ascii format */
  (void) ui_sms_bcd_ascii_cvt (mc_time.yr, (char *) &text_ptr[0]);
  julian->year = ((text_ptr[0] - 0x30) * 10) +  (text_ptr[1] - 0x30);
  if (julian->year < 80)
    julian->year = julian->year + 2000 ;
  else
    julian->year = julian->year + 1900 ;

  (void) ui_sms_bcd_ascii_cvt (mc_time.mon, (char *) &text_ptr[0]);
  julian->month = ((text_ptr[0] - 0x30) * 10) + (text_ptr[1] - 0x30);
  
  (void) ui_sms_bcd_ascii_cvt (mc_time.day, (char *) &text_ptr[0]);
  julian->day = ((text_ptr[0] - 0x30) * 10) + (text_ptr[1] - 0x30);
  
  (void) ui_sms_bcd_ascii_cvt (mc_time.hrs, (char *) &text_ptr[0]);
  julian->hour = ((text_ptr[0] - 0x30) * 10) +  (text_ptr[1] - 0x30);

  (void) ui_sms_bcd_ascii_cvt (mc_time.min, (char *) &text_ptr[0]);
  julian->minute = ((text_ptr[0] - 0x30) * 10) +  (text_ptr[1] - 0x30);

  (void) ui_sms_bcd_ascii_cvt (mc_time.sec, (char *) &text_ptr[0]);
  julian->second = ((text_ptr[0] - 0x30) * 10) +  (text_ptr[1] - 0x30);

} /* end ui_sms_bcd_to_julian () */

boolean ui_sms_alert_status;
boolean reading_new;
BOOL    g_fSmsJustRead = FALSE;

/* Unpacked SMS message */
smsi_msg_type ui_sms_msg;
ui_sms_edit_info_type list_info;
ui_edit_type call_back_buf;
byte number_buf[UI_SMS_WIDE*3];
ui_sms_nv_list_type ui_sms_nv_list[MAX_NV_MSGS];

byte ui_sms_auto_format_one_line
(
  UINT8   nCharNum,           /* One line character num          */
  byte    *input_txt,         /* Pointer to text to be formatted */
  word    input_len,          /* Number of bytes to be formatted */
  byte    *format_txt,        /* Pointer to formatted text       */
  word    format_len          /* Size of formatted text          */
)
{
    byte    line_num = 0;
    int     length = 0, k = 0 ;

    while (( length < input_len) && (line_num < format_len/nCharNum)) 
    {
        if((input_txt[length] == 0x0D) ||     // for return key 
	         (input_txt[length] == 0x0A) ) 
        {
            if(k != 0)
            {
                memset(&format_txt[line_num*nCharNum + k],
                            SMS_SPACE,
                            nCharNum-k);
                line_num++;
            }
            length ++;
            k = 0;
        } 
        else if (input_txt[length] == 0x00) 
        {
            length ++;
        } 
        else 
        {
	        if( k == nCharNum-1 ) 
            {
                if(input_txt[length] & 0x80) 
                {
                    if(!IsMiddleOfMultiByte((byte*)&(format_txt[line_num*nCharNum]), k+1)) 
                    {
                        *(format_txt+line_num*nCharNum+k) = input_txt[length]; 
                        line_num++;
                        k = -1;
                    } 
                    else 
                    {
                        *(format_txt+line_num*nCharNum+k) = SMS_SPACE;
                        line_num++;
//                        *(format_txt+line_num*nCharNum) = input_txt[length];
                        k = -1;
	                }	
                } 
                else 
                {
                    *(format_txt+line_num*nCharNum+k) = input_txt[length];
                    line_num++ ;
                    k = -1;
                }
            } 
            else 
            {
                *(format_txt+line_num*nCharNum+k) = input_txt[length];
            }
            length++;
            k++;
        }
    }

    if(k != 0) 
    {
        line_num++;
    }

    return ( line_num );
}


byte ui_sms_auto_format
(
  boolean word_wrap,          /* Indicate if wrapping words      */
  byte    *input_txt,         /* Pointer to text to be formatted */
  word    input_len,          /* Number of bytes to be formatted */
  byte    *format_txt,        /* Pointer to formatted text       */
  word    format_len          /* Size of formatted text          */
)
{
    byte    line_num = 0;
    int     length = 0, k = 0 ;

    while (( length < input_len) && (line_num < format_len/SMS_SCREEN_SIZE)) 
    {
/*
        if((input_txt[length] == 0x0D) ||     // for return key 
	         (input_txt[length] == 0x0A) ) 
        {
            if(k != 0)
            {
                memset(&format_txt[line_num*SMS_SCREEN_SIZE + k],
                            SMS_SPACE,
                            SMS_SCREEN_SIZE-k);
                line_num++;
            }
            length ++;
            k = 0;
        } 
        else if (input_txt[length] == 0x00) 
        {
            length ++;
        } 
*/
        if( input_txt[length] == 0x00 )
        {
            length++;
        }
        else 
        {
	        if( k == SMS_SCREEN_SIZE-1 ) 
            {
                if(input_txt[length] & 0x80) 
                {
                    if(!IsMiddleOfMultiByte((byte*)&(format_txt[line_num*SMS_SCREEN_SIZE]), k+1)) 
                    {
                        *(format_txt+line_num*SMS_SCREEN_SIZE+k) = input_txt[length]; 
                        line_num++;
                        k = -1;
                    } 
                    else 
                    {
                        *(format_txt+line_num*SMS_SCREEN_SIZE+k) = SMS_SPACE;
                        line_num++;
                        *(format_txt+line_num*SMS_SCREEN_SIZE) = input_txt[length];
                        k = -1;
	                }	
                } 
                else 
                {
                    *(format_txt+line_num*SMS_SCREEN_SIZE+k) = input_txt[length];
                    line_num++ ;
                    k = -1;
                }
            } 
            else 
            {
                *(format_txt+line_num*SMS_SCREEN_SIZE+k) = input_txt[length];
            }
            length++;
            k++;
        }
    }

    if(k != 0) 
    {
        line_num++;
    }

    return ( line_num );
}


void ui_sms_update_icons
(
void
)
{
    static  INT8    nVmsCheck, nMBCheck, i;

    nVmsCheck = 0;
    nMBCheck = 0;

    // Retrive used buffer
    for (i = 0; i < MAX_VMS_MSG; i++) 
    {
        if (a_VmsBuffer[i].nUsed == 1) 
        {
	        nVmsCheck = 1;
            break;
        }
    }

	if(nVmsCheck == 1) 
    {
        /* Turn on New voice mail icon */
		for (i = 0; i < MAX_VMS_MSG; i++) 
        {
			if (a_VmsBuffer[i].nAttr == 0x01) 
            {
	           nVmsCheck = 2;
			   break;
			}
		}
	}

    // Retrive used buffer
    for (i = 0; i < MAX_MYBELL_SIZE; i++) 
    {
        if (a_MybellBuffer[i].nUsed == 1) 
        {
	        nMBCheck = 1;
            break;
        }
    }

	if(nMBCheck == 1) 
    {
        /* Turn on New voice mail icon */
		for (i = 0; i < MAX_MYBELL_SIZE; i++) 
        {
			if (a_MybellBuffer[i].nAttr & 0x01) 
            {
	           nMBCheck = 2;
			   break;
			}
		}
	}

    /* Message or digital page icon */
    /* There is new urgent text, blink icon */
    if ( ( ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0 )  || 
         ( ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0 ) ||
         ( nVmsCheck == 2 ) ||
         ( nMBCheck == 2 ) )
    {
        if (SKY_SetThenCompareSMS(SMS_ICON_NEW_MESSAGE))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
            /* jrkwon 2001.05.03 : when not idle screen. force to draw */
            /* jrkwon 2001-05-04 --> */
#ifdef FEATURE_WAP
            if(SKY_IS_IN_WAP())
                ;
            //ytchoi 
            // 같은 task이기 때문에 그냥 event로 바꿔야 된다.
//                SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
            else
#endif//FEATURE_WAP
            /* <-- */
                SKY_DrawSubAnnunciators();
        }
//        /* Blink msg text message icon */
//        uiscrn_annblink( HS_AN_MSG );
    }
#if 0
    /* No new text messages at all */
    else if ( ( ui_sms_list[UI_SMS_TXT_OLD].cnt > 0 ) || ( nVmsCheck == 1 ) || ( nMBCheck == 1 ) )
    {
//        if (SKY_SetThenCompareSMS(SMS_ICON_READ_MESSAGE))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
//        /* Turn off text msg icon */
//        uiscrn_ann( HS_AN_MSG, HS_AN_MSG );
    }
#endif
    else
    {
        if (SKY_SetThenCompareSMS(SMS_ICON_NO_MESSAGE))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
//        /* Turn off text msg icon */
//        uiscrn_ann( HS_AN_MSG, 0 );
    }

}


void ui_sms_power_down
(
void
)
{
  byte                  i;
    /* Loop counter index */
  byte                  j;
    /* Loop counter index */
  nv_stat_enum_type     nv_status = NV_DONE_S;
    /* NV return status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize SMS control list length */
  ui_sms_nv_item.length = 0;

  /* Reuse user_data_length in SMS control */
  /* information to indicate if there are  */
  /* available SMS NV indicies to store    */
  /* SMS messages.                         */
  ui_sms_nv_item.user_data_length = ui_sms_list[UI_SMS_AVAIL].cnt;

  /* Create SMS control list from SMS lists */
  /* Loop through each SMS list */
  for (i=0; i<MAX_SMS_LIST; i++)
  {
    /* Loop through each valid entry in an SMS list */
    for (j=0; j<ui_sms_list[i].cnt; j++)
    {
      /* Set SMS list category and order of each NV address into */
      /* SMS control list.  The index to the nv list represents  */
      /* an array index starting at 0.  Since the stored message */
      /* nv addresses start at 1, offset each address by 1 to    */
      /* begin nv list array address at 0.                       */
      ui_sms_nv_list[ui_sms_list[i].nv_idx[j]-1].list_cat = i;
      ui_sms_nv_list[ui_sms_list[i].nv_idx[j]-1].order_idx = j;
    }
  }

  /* Copy SMS control list to NV SMS item data format */
  (void) memcpy ( (void *) ui_sms_nv_item.data, (const void *) &ui_sms_nv_list, 
                   sizeof( ui_sms_nv_list ));

  /* Place SMS validation marker to indicate good data */
  ui_sms_nv_item.data[sizeof( ui_sms_nv_list )] = 0xDB;
  
  /* Set SMS control list length */
  ui_sms_nv_item.length = (sizeof ( ui_sms_nv_list ) + 1 );

  /* Set NV SMS address to SMS control list address 0 */
  ui_sms_nv_item.address = 0;

  /* Status not used in SMS control list */
  /* so initialize to known value.       */
  ui_sms_nv_item.status = 0;

  /* Write SMS control list to NV SMS index 0 */
  nv_status = SKY_PutNV( NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item );
}

void ui_sms_power_up
(
void
)
{
  byte                  i;
    /* Loop counter index */
  byte                  j;
    /* Loop counter index */
  nv_stat_enum_type     nv_status = NV_DONE_S;
    /* NV return status   */
  ui_sms_op_status_type status;
    /* SMS operational return status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Initialize SMS power up status */
  status = UI_SMS_OP_IN_PROG;

#ifdef NO_USE
  /* Initialize SMS state */
  state = UI_SMS_IDLE_S;

  /* Initialize SMS stack info */
  sms_stack_pos = 0;
  sms_stack[0] = UI_SMS_IDLE_S;

  /* Initialize SMS indicators */
  refresh_disp = FALSE;
  maj_state_return = FALSE;
  manual_scroll = FALSE;
  sts_timer_wait = FALSE;
  ui_sms_alert_status = FALSE;
  restart_sni = FALSE;

  /* Initialize next SMS alert reminder */
  audio_alert_time = 0;
  visual_alert_off_time = 0;

#endif

  /* Initialize all SMS lists count to indicate */
  /* no entries in their lists.                 */
  for (i=0; i<MAX_SMS_LIST; i++)
  {
    ui_sms_list[i].cnt = 0;
  }
   
  /* Initialize match text against old messages */
  /* indicator for all SMS messages.            */
  for (i=0; i<(MAX_NV_MSGS+1); i++)
  {
    ui_sms_detect_list [i].match_old_text = FALSE;
  }

#ifdef NO_USE
  /* Initialize SMS display structure info */
  ui_sms_disp_info.display = UI_SMS_DISP_NONE;
  ui_sms_disp_info.new_disp = FALSE;
  ui_sms_disp_info.soft_disp = FALSE;
  ui_sms_disp_info.sms_list = UI_SMS_AVAIL;
  ui_sms_disp_info.sms_idx = 0;
  ui_sms_disp_info.cur_line = 0;
  ui_sms_disp_info.scr_line = 0;
  ui_sms_disp_info.line_len = 0;
  ui_sms_disp_info.cb_len = 0;
#endif

  /* Initialize reading markers used to indicate */
  /* where the new SMS message was stored in NV. */
  ui_sms_read_info.new_read_list = UI_SMS_TXT_NEW_URGENT;
  ui_sms_read_info.new_read_idx = 0;

  /* Initialize list info message pointer */
  list_info.sms_msg = &ui_sms_msg;

  /* Read SMS control list from NV SMS index 0 */
  ui_sms_nv_item.address = 0;

  nv_status = SKY_GetNV( NV_SMS_I, (nv_item_type *) &ui_sms_nv_item );

  /* Successfully retrieved SMS control list */
  if ( nv_status == NV_DONE_S ) 
  {
    /* Unpack SMS control list from NV SMS item format */
    (void) memcpy ((void *) &ui_sms_nv_list, 
                    (const void *) ui_sms_nv_item.data, sizeof ( ui_sms_nv_list ));

    /* If SMS validation marker is not present then data is bogus. */
    /* Reset SMS control list.                       */
    if ( ui_sms_nv_item.data[sizeof( ui_sms_nv_list )] != 0xDB )
    {
      /* Invalid SMS control list, reset all SMS lists */
      status = UI_SMS_OP_NO_IDX;

      /* Print debug error message */
#ifndef WIN32
      ERR( "UI SMS - SMS control list invalid", 0, 0, 0 );
#endif
    }
    /* Valid SMS control list.  Read out SMS list data */
    else
    {
      /* Create SMS message lists from SMS control list */
      for (i=0; i<MAX_NV_MSGS; i++)
      {
        /* Increment SMS list entry count */
        ui_sms_list[ui_sms_nv_list[i].list_cat].cnt++;

        /* Based on the saved list category and order within */
        /* the list, set stored SMS message's NV address.    */
        /* The SMS control list index + 1 indicates the SMS  */
        /* NV address used to store a given message.  Stored */
        /* SMS NV message addresses start at 1 since address */
        /* 0 is reserved for SMS control list storage.       */
        ui_sms_list[ui_sms_nv_list[i].list_cat].
          nv_idx[ui_sms_nv_list[i].order_idx] = i + 1;

        /* Go thru SMS list with stored messages, retrieve */
        /* message from NV and format first line of text   */
        /* for each entry.                                 */
        if ( ui_sms_nv_list[i].list_cat != UI_SMS_AVAIL )
        {
          /* Read SMS message from NV */
          ui_sms_nv_item.address = i + 1;

          nv_status = SKY_GetNV( NV_SMS_I, (nv_item_type *) &ui_sms_nv_item );

          /* Successfully retrieved SMS message */
          if ( nv_status == NV_DONE_S )
          {
            /* Setup list information for sms list manipulation */
            list_info.src_list = ui_sms_nv_list[i].list_cat;
            list_info.src_idx = ui_sms_nv_list[i].order_idx;
			list_info.protect = ui_sms_nv_item.protect;

            /* Unpack SMS message from NV SMS packed format  */
            /* and put message into list information used to */
            /* save first line of text or number of a stored */
            /* message.                                      */
			memset ((void*)list_info.sms_msg->msg_data.user_data.chari,0x00,SMSI_USER_DATA_MAX);
            ui_sms_nv_data ( FALSE, &ui_sms_nv_item, list_info.sms_msg );

            /* Save first line of user data or call back number */
            /* in SMS source list data.                         */
            ui_sms_save_list_text ();
          }
          /* Error encountered in reading saved messages. */
          /* Cannot determine if SMS control list is      */
          /* corrupted or single message, so reset entire */
          /* SMS control list.  Saved messages will be    */
          /* lost.                                        */
          else
          {
            /* Read error encountered, delete existing */
            /* messages and reset all SMS lists.       */
            status = UI_SMS_OP_NO_IDX;

            /* Print debug error message */
#ifndef WIN32
            ERR( "UI SMS - SMS message read error", 0, 0, 0 );
#endif
          }
        }
      }
    }
  }
  /* No SMS control list has been saved in NV    */
  /* i.e. - no saved SMS messages, so initialize */
  /* the SMS available list to all NV indicies   */
  /* and save SMS control list to NV to reserve  */
  /* memory.                                     */
  else
  {
    /* No SMS control list saved, reset all SMS lists */
    status = UI_SMS_OP_NO_IDX;

    /* Print debug error message */
#ifndef WIN32
    ERR( "UI SMS - No SMS control list saved", 0, 0, 0 );
#endif //
  }

  /* Check for duplicate entries or skips if */
  /* no error causing full reset has been    */
  /* found.                                  */
  if ( status != UI_SMS_OP_NO_IDX )
  {
    for (i=0; i<MAX_NV_MSGS; i++)
    {
      /* Message placed out of range of its SMS list count. */
      /* Reset all SMS lists.                               */
      if ( ui_sms_nv_list[i].order_idx >=
           ui_sms_list[ui_sms_nv_list[i].list_cat].cnt )
      {
        /* Set status to cause SMS */
        /* lists reset.            */
        status = UI_SMS_OP_NO_IDX;

        /* Print debug error message */
#ifndef WIN32
        ERR( "UI SMS - Message index out of range", 0, 0, 0 );
#endif
      }

      /* Check each new entry agains previously read  */
      /* entries for SMS list duplicates if no errors */
      /* have been found yet.                         */
      if ( status != UI_SMS_OP_NO_IDX )
      {
        /* Loop through previously checked entries */
        for (j=0; j<i; j++)
        {
          /* If duplicate list entry for different NV indicies */
          /* found, then delete message and return NV index to */
          /* the available list.                               */
          if (( ui_sms_nv_list[i].list_cat == ui_sms_nv_list[j].list_cat ) &&
              ( ui_sms_nv_list[i].order_idx == ui_sms_nv_list[j].order_idx ))
          {
            /* Set status to cause SMS */
            /* lists reset.            */
            status = UI_SMS_OP_NO_IDX;

            /* Print debug error message */
#ifndef WIN32
            ERR( "UI SMS - Duplicate entry found", 0, 0, 0 );
#endif
          }
        }
      }
    }
  }

  /* If SMS control list is not valid or non-existant  */
  /* then initialize all SMS lists and write out valid */
  /* SMS control list to NV.                           */
  if ( status == UI_SMS_OP_NO_IDX )
  {
    /* Initialize all SMS lists count to indicate */
    /* no entries in their lists.                 */
    for (i=0; i<MAX_SMS_LIST; i++)
    {
      ui_sms_list[i].cnt = 0;
    }

    /* Initialize list of available SMS NV indicies to  */
    /* all NV indexes if no SMS control list available. */
    ui_sms_list[UI_SMS_AVAIL].cnt = MAX_NV_MSGS;
    for (i=0; i<MAX_NV_MSGS; i++)
    {
      /* Setup NV free address */
      ui_sms_nv_item.address = i + 1;

      /* Free memory allocation for specified item stored in NV.  */
      /* Ignore return status since we attempting error recovery. */
      (void) SKY_FreeNV( NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item );

      ui_sms_list[UI_SMS_AVAIL].nv_idx[i] = i + 1;
    }

    /* Perform the SMS power down processing which   */
    /* saves SMS control list in NV.  SMS control    */
    /* list size will not change. Need to ensure     */
    /* that there is always enough NV memory to save */
    /* the SMS control list so allocate space now.   */
    (void) ui_sms_power_down ();
  }

  /* SMS lists not reset so setup icon and */
  /* read status based on lists.           */
  if ( status == UI_SMS_OP_IN_PROG )
  {
    /* Perform the SMS power down processing which   */
    /* saves SMS control list in NV.  Rewrite SMS    */
    /* control list to store current indication of   */
    /* available SMS NV indicies for SMS message     */
    /* storage.                                      */
    (void) ui_sms_power_down ();

    /* Determine what icon should be displayed and   */
    /* if there are urgent messages then blink icon. */

    ui_sms_update_icons ();

    /* Check for new messages */

    if ( ( ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0 ) ||
         ( ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0 ) )
    {
      /* Set indicator to reading new messages */
//      reading_new = TRUE;
    }
  }
}


void g_DoOldTextMatch
(
void
)
{
    nv_stat_enum_type nv_status = NV_DONE_S;
    int i;
    /* NV return status   */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Check for messages that required text */
    /* matching.                             */
    for (i=1; i<(MAX_NV_MSGS+1); i++)
    {
        /* Determine if text matching is required */
        /* for this message.                      */
        if ( ui_sms_detect_list [i].match_old_text )
        {
            /* Reset text match indicator. */
            ui_sms_detect_list [i].match_old_text = FALSE;

            /* Retrieve SMS NV index to access  */
            /* message's text data.             */
            ui_sms_nv_item.address = i;

            /* Read SMS message text from NV */
            nv_status = SKY_GetNV( NV_SMS_I, (nv_item_type *) &ui_sms_nv_item );

            /* Successfully retrieved SMS message */
            /* so check user data against new     */
            /* message's user data.               */
            if ( nv_status == NV_DONE_S )
            {
                /* Unpack SMS message from NV SMS packed format for */
                /* text matching.                                   */
                ui_sms_nv_data ( FALSE, &ui_sms_nv_item, &ui_sms_msg );

                /* Check for duplicate text against all old */
                /* stored messages.                         */
                if ( ui_sms_text_match ( FALSE ))
                {
                    /* Since text matching uses the ui_sms_nv_item to read */
                    /* SMS message, repack new SMS message into NV SMS     */
                    /* format and update the new nv item information.      */
                    ui_sms_nv_data ( TRUE, &ui_sms_nv_item, &ui_sms_msg );

                    /* Reset SMS NV index to new */
                    /* message.                  */
                    ui_sms_nv_item.address = i;

                    /* Tag message to be stored as a duplicate. */
                    ui_sms_nv_item.status |= UI_SMS_STS_DUPLICATE;

                    /* Update message status in NV */
                    (void) SKY_PutNV( NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item );
                }
            }
        }
    }
}


/////////////////////////////////////////////////////

/* Temporary buffer for speed dial name */
byte alpha_buf [UI_SMS_WIDE];

/* Unpacked SMS message for text match detection */
smsi_msg_type ui_sms_detect_msg;

/* Auto-formatted stored SMS message for text match detection */
byte ui_sms_detect_text[SMSI_USER_DATA_MAX];

/* Auto-formatted new SMS message for text match detection */
byte ui_sms_new_text[SMSI_USER_DATA_MAX];

/* Number of duplicate messages to be deleted. */
byte ui_sms_dup_cnt;

/* List of duplicate messages to be deleted. */
ui_sms_nv_list_type ui_sms_dup_list[MAX_NV_MSGS];

#ifdef FEATURE_SMS_PACKET

LOCAL byte    uismsl_read_status;
LOCAL byte    uismsl_delete_status;
LOCAL boolean uismsl_vmn_new = TRUE;

#endif /* FEATURE_SMS_PACKET */
/* Number of bytes in the Voice Mail message is occupying in NV */
/* added this for sms.c so linker would not complain. */
word sms_voice_mail_mem_size;     /* VMN NV reservation */



/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_NV_DATA

DESCRIPTION
  Pack or unpack SMS data stored in or retrieved from NV.
  
DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ui_sms_nv_data 
(
  boolean       packing,
  nv_sms_type   *pack_data_ptr,
  smsi_msg_type *unpack_data_ptr
)
{
  word byte_pos = 0;
    /* Current byte counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize unpacked data lengths when unpacking */
  /* in case of left over data.                      */
  if (!packing)
  {
    /* Reset user data length */
    unpack_data_ptr->msg_data.user_data.num_fields = 0;

    /* Reset call back number length */
    unpack_data_ptr->msg_data.call_back.num_fields = 0;

    /* Reset address length */
    unpack_data_ptr->msg_data.address.num_fields = 0;

    /* Reset subaddress length */
    unpack_data_ptr->msg_data.subaddress.num_fields = 0;
  }

  /* SMS teleservice is required for SMS data storage in NV */
  if (packing)
  {
    /* Packing SMS data */
    (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                    (const void *) &unpack_data_ptr->tele_srv,
                     sizeof (smsi_teleservice_type) );
  }
  else
  {
    /* Unpacking SMS data */
    (void) memcpy ((void *) &unpack_data_ptr->tele_srv,
                    (const void *) &pack_data_ptr->data [byte_pos],
                     sizeof (smsi_teleservice_type) );
  }

  /* Increment packed data byte position */
  byte_pos += sizeof (smsi_teleservice_type);

  /* SMS message category is required for SMS data storage in NV */
  if (packing)
  {
    /* Packing SMS data */
    (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                    (const void *) &unpack_data_ptr->sms_msg_cat,
                     sizeof (smsi_msg_cat_type) );
  }
  else
  {
    /* Unpacking SMS data */
    (void) memcpy ((void *) &unpack_data_ptr->sms_msg_cat,
                    (const void *) &pack_data_ptr->data [byte_pos],
                     sizeof (smsi_msg_cat_type) );
  }

  /* Increment packed data byte position */
  byte_pos += sizeof (smsi_msg_cat_type);

  /* SMS message identifier is required for SMS data storage in NV */
  if (packing)
  {
    /* Packing SMS data */
    (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                    (const void *) &unpack_data_ptr->sms_msg_id,
                     sizeof (smsi_msg_id_type) );
  }
  else
  {
    /* Unpacking SMS data */
    (void) memcpy ((void *) &unpack_data_ptr->sms_msg_id,
                    (const void *) &pack_data_ptr->data [byte_pos],
                     sizeof (smsi_msg_id_type) );
  }

  /* Increment packed data byte position */
  byte_pos += sizeof (smsi_msg_id_type);

  /* Update packing/unpacking optional parameters mask. */
  /* Determine which optional parameters are being      */
  /* packed or unpacked.                                */
  if (packing)
  {
   (void) memcpy ((void * ) &pack_data_ptr->param_mask, &unpack_data_ptr->sms_param_m, 4);
  }
  else
  {
   (void) memcpy (&unpack_data_ptr->sms_param_m, (const void *) &pack_data_ptr->param_mask, 4);
  }

  /* The following SMS optional parameters are stored in this */
  /* arbitrary order of occurrence.                           */
  /*-----------*/
  /* User Data */
  /*-----------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_USER_DATA) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                      (const void *) unpack_data_ptr->msg_data.user_data.chari,
                       unpack_data_ptr->msg_data.user_data.num_fields );

      /* Update packed data user data byte length */
      pack_data_ptr->user_data_length = 
        unpack_data_ptr->msg_data.user_data.num_fields;

      /* Increment packed data byte position */
      byte_pos += unpack_data_ptr->msg_data.user_data.num_fields;
    }
    else
    {
      /* Unpacking SMS data */
      (void) memcpy ((void *) unpack_data_ptr->msg_data.user_data.chari,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       pack_data_ptr->user_data_length );

      /* Update unpacked data user data byte length */
      unpack_data_ptr->msg_data.user_data.num_fields =
        pack_data_ptr->user_data_length;

      /* Increment packed data byte position */
      byte_pos += pack_data_ptr->user_data_length;
    }
  }
  /* No user data parameter detected in SMS message */
  else
  {
    if (packing)
    {
      /* Indicate zero length for user data */
      pack_data_ptr->user_data_length = 0;
    }
  }

  /*------------------*/
  /* Call Back Number */
  /*------------------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_CALL_BACK) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      /* Include 4 bytes of mode/numbering info with call back number */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos], 
               (const void *) &unpack_data_ptr->msg_data.call_back,
                (unpack_data_ptr->msg_data.call_back.num_fields + 4) );

      /* Update packed data call back number byte length */
      pack_data_ptr->call_back_length = 
        unpack_data_ptr->msg_data.call_back.num_fields;

      /* Increment packed data byte position */
      byte_pos += (unpack_data_ptr->msg_data.call_back.num_fields + 4);
    }
    else
    {
      /* Unpacking SMS data */
      /* Include 4 bytes of mode/numbering info with call back number */
      (void) memcpy ((void *) &unpack_data_ptr->msg_data.call_back,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       (pack_data_ptr->call_back_length + 4) );

      /* Update unpacked data call back number byte length */
      unpack_data_ptr->msg_data.call_back.num_fields =
        pack_data_ptr->call_back_length;

      /* Increment packed data byte position */
      byte_pos += (pack_data_ptr->call_back_length + 4);
    }
  }
  /* No call back number parameter detected in SMS message */
  else
  {
    if (packing)
    {
      /* Indicate zero length for call back number */
      pack_data_ptr->call_back_length = 0;
    }
  }


  /*--------------------*/
  /* Number Of Messages */
  /*--------------------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_NUM_OF_MSGS) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                      &unpack_data_ptr->msg_data.num_of_msgs,
                       sizeof (smsi_num_of_msgs_type) );
    }
    else
    {
      /* Unpacking SMS data */
      (void) memcpy (&unpack_data_ptr->msg_data.num_of_msgs,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       sizeof (smsi_num_of_msgs_type) );
    }

    /* Increment packed data byte position */
    byte_pos += sizeof (smsi_num_of_msgs_type);
  }

  /*--------------------*/
  /* Priority Indicator */
  /*--------------------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_PRIORITY) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                      &unpack_data_ptr->msg_data.priority,
                       sizeof (smsi_indicator_type) );
    }
    else
    {
      /* Unpacking SMS data */
      (void) memcpy (&unpack_data_ptr->msg_data.priority,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       sizeof (smsi_indicator_type) );
    }

    /* Increment packed data byte position */
    byte_pos += sizeof (smsi_indicator_type);
  }

  /*---------------------------*/
  /* Message Center Time Stamp */
  /*---------------------------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_MC_TIME_STAMP) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                     (const void *)  &unpack_data_ptr->msg_data.mc_time,
                       sizeof (smsi_abs_time_type) );
    }
    else
    {
      /* Unpacking SMS data */
      (void) memcpy ((void *) &unpack_data_ptr->msg_data.mc_time,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       sizeof (smsi_abs_time_type) );
    }

    /* Increment packed data byte position */
    byte_pos += sizeof (smsi_abs_time_type);
  }

  /*---------------------*/
  /* Destination Address */
  /*---------------------*/
  /* Pack or unpack origination/destination address parameter */
  if (((unpack_data_ptr->sms_param_m & SMSI_ORIG_ADDR) != 0) ||
     ((unpack_data_ptr->sms_param_m & SMSI_DEST_ADDR) != 0))
  {
    if (packing)
    {
      /* Packing SMS data */
      /* Include 5 bytes of mode/numbering and length info with address */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                     (const void *)  &unpack_data_ptr->msg_data.address,
                       (unpack_data_ptr->msg_data.address.num_fields + 5) );

    }
    else
    {
      /* Unpacking SMS data */
      /* First 5 bytes include mode/numbering and address length info */
      (void) memcpy ((void *) &unpack_data_ptr->msg_data.address,
                     (const void *)  &pack_data_ptr->data [byte_pos], 5);

      /* Unpacking SMS address data */
      (void) memcpy ((void *) &unpack_data_ptr->msg_data.address.chari,
                     (const void *) &pack_data_ptr->data [byte_pos+5],
                       unpack_data_ptr->msg_data.address.num_fields );
    }

    /* Increment packed data byte position */
    byte_pos += (unpack_data_ptr->msg_data.address.num_fields + 5);
  }

  /*------------------------*/
  /* Destination Subaddress */
  /*------------------------*/
  /* Pack or unpack origination/destination subaddress parameter */
  if (((unpack_data_ptr->sms_param_m & SMSI_ORIG_SUBADDR) != 0) || 
     ((unpack_data_ptr->sms_param_m & SMSI_DEST_SUBADDR) != 0)) 
  {
    if (packing)
    {
      /* Packing SMS data */
      /* Include 3 bytes of mode/numbering and length info with address */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                     (const void *)  &unpack_data_ptr->msg_data.subaddress,
                       (unpack_data_ptr->msg_data.subaddress.num_fields + 3) );

    }
    else
    {
      /* Unpacking SMS data */
      /* First 3 bytes include mode/numbering and address length info */
      (void) memcpy ((void *) &unpack_data_ptr->msg_data.subaddress,
                     (const void *) &pack_data_ptr->data [byte_pos], 3);

      /* Unpacking SMS subaddress data */
      (void) memcpy ((void *) &unpack_data_ptr->msg_data.subaddress.chari,
                     (const void *)  &pack_data_ptr->data [byte_pos+3],
                       unpack_data_ptr->msg_data.subaddress.num_fields );
    }

    /* Increment packed data byte position */
    byte_pos += (unpack_data_ptr->msg_data.subaddress.num_fields + 3);
  }

  /*--------------*/
  /* Reply Option */
  /*--------------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_REPLY_OPTION) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                     (const void *) &unpack_data_ptr->msg_data.reply_opt,
                       sizeof (smsi_reply_opt_type) );
    }
    else
    {
      /* Unpacking SMS data */
      (void) memcpy ((void *) &unpack_data_ptr->msg_data.reply_opt,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       sizeof (smsi_reply_opt_type) );
    }

    /* Increment packed data byte position */
    byte_pos += sizeof (smsi_reply_opt_type);
  }

  /*-------------------*/
  /* Privacy Indicator */
  /*-------------------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_PRIVACY) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                      &unpack_data_ptr->msg_data.privacy,
                       sizeof (smsi_indicator_type) );
    }
    else
    {
      /* Unpacking SMS data */
      (void) memcpy (&unpack_data_ptr->msg_data.privacy,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       sizeof (smsi_indicator_type) );
    }

    /* Increment packed data byte position */
    byte_pos += sizeof (smsi_indicator_type);
  }

  /*--------------------*/
  /* Language Indicator */
  /*--------------------*/
  if ((unpack_data_ptr->sms_param_m & SMSI_LANGUAGE) != 0) 
  {
    if (packing)
    {
      /* Packing SMS data */
      (void) memcpy ((void *) &pack_data_ptr->data [byte_pos],
                      &unpack_data_ptr->msg_data.lang,
                       sizeof (smsi_language_type) );
    }
    else
    {
      /* Unpacking SMS data */
      (void) memcpy (&unpack_data_ptr->msg_data.lang,
                      (const void *) &pack_data_ptr->data [byte_pos],
                       sizeof (smsi_language_type) );
    }

    /* Increment packed data byte position */
    byte_pos += sizeof (smsi_language_type);
  }
 
  /* Return packed byte length for NV storage purposes */
  if (packing)
  {
    pack_data_ptr->length = byte_pos;
  }
}

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_DUPLICATE

DESCRIPTION
  Compares current message's message id and Message Center timestamp
  with stored messages' information to determine if current message
  is a retransmission of the exact same message as one of the stored 
  messages.  If a duplicate is found, this implies that the base
  station resent the same message because acknowledgements were not
  received by the base station.  Currently, duplicate messages are
  dropped and ignored if found.  This function returns a boolean 
  status; true if duplicate.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_sms_duplicate
(
void
)
{
  byte i;
    /* Loop counter index */
  byte j;
    /* Loop counter index */
  boolean duplicate;
    /* Duplicate detection return status */
  byte nv_addr;
    /* SMS NV address of current message */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize return status */
  duplicate = FALSE;

  /* Check if current message has a valid message id.      */
  /* Only messages with valid message ids will checked for */
  /* duplicate detection.  Zero is not a valid message id. */
  if ( ui_sms_msg.sms_msg_id.message_id != 0 ) 
  {
    /* Search for match in all lists */
    /* containing stored messages.   */
    for (i=1; i<MAX_SMS_LIST; i++)
    {
      /* Loop through each stored message */
      /* in the SMS list.                 */
      for (j=0; j<ui_sms_list[i].cnt; j++)
      {
        /* Retrieve SMS NV index to access  */
        /* message's duplicate information. */
        nv_addr = ui_sms_list [i].nv_idx [j];

        /* Check if message ids match. */
        if (( ui_sms_msg.sms_msg_id.message_id == 
              ui_sms_detect_list [nv_addr].message_id ))
        {
          /* If both messages have a Message Center */
          /* timestamp to be checked, then check    */
          /* timestamp.                             */
          if ((( ui_sms_msg.sms_param_m & SMSI_MC_TIME_STAMP) != 0) &&
               ( ui_sms_detect_list [nv_addr].check_time ) &&
               ( ui_sms_detect_list [nv_addr].mc_time.yr ==
                 ui_sms_msg.msg_data.mc_time.yr ) &&
               ( ui_sms_detect_list [nv_addr].mc_time.mon ==
                 ui_sms_msg.msg_data.mc_time.mon ) &&
               ( ui_sms_detect_list [nv_addr].mc_time.day ==
                 ui_sms_msg.msg_data.mc_time.day ) &&
               ( ui_sms_detect_list [nv_addr].mc_time.hrs ==
                 ui_sms_msg.msg_data.mc_time.hrs ) &&
               ( ui_sms_detect_list [nv_addr].mc_time.min ==
                 ui_sms_msg.msg_data.mc_time.min ) &&
               ( ui_sms_detect_list [nv_addr].mc_time.sec ==
                 ui_sms_msg.msg_data.mc_time.sec ))
          {
            /* Duplicate message found.  */
            /* Search finished, so exit. */
            duplicate = TRUE;
            break;
          }
          /* If both messages do not have a Message Center */
          /* timestamp to be checked, then message is a    */
          /* duplicate.                                    */
          else if ((( ui_sms_msg.sms_param_m & SMSI_MC_TIME_STAMP) == 0) &&
                    ( !ui_sms_detect_list [nv_addr].check_time ))
          {
            /* Duplicate message found.  */
            /* Search finished, so exit. */
            duplicate = TRUE;
            break;
          }
        }
      }
      /* If duplicate has been  */
      /* found, then exit loop. */
      if ( duplicate )
      {
        break;
      }
    }
  }

  /* Return duplicate detection status */
  return ( duplicate );
}

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_TEXT_MATCH

DESCRIPTION
  Compares user data text or call back number in the new message or 
  page against the text in the stored unread or read messages or 
  pages to determine duplicate message.  If the messages have 
  same text and different call back numbers, then message is not 
  considered a duplicate.  Any stored messages considered a duplicate 
  are deleted and the new message is tagged as a duplicate.  Calling 
  function indicates if text matching should be performed against 
  stored unread messages or stored read messages.  Since text matching 
  causes an automatic delete of stored messages, the message to be 
  compared must NOT be included in the list of stored messages to 
  be compared against.  Other text matching rules must be followed: 
  1) When text matching the new message against unread messages, 
  text matching must be performed BEFORE the new message is added to 
  NV since the message delete could obsolete the bookmark info on 
  the new message when added to NV.  2) When text matching the new 
  message against old read messages, be sure the current SMS state 
  assumes the user cannot be reading old messages.  Text matching 
  is performed in two separate places in order to maintain the 
  maximum amount of SMS NV memory and maximum number of available 
  SMS NV indicies at all times, and to prevent the concurrent read 
  by the user and automatic deletion of the same message.  Assumes 
  new message to be compared is in ui_sms_msg.  This function is not 
  performed on voice mail.  Function returns boolean indicating if 
  text or call back number match was found.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_sms_text_match
(
boolean unread
)
{
  byte i;
    /* Loop counter index */
  byte j;
    /* Loop counter index */
  byte k;
    /* Loop counter index */
  boolean duplicate;
    /* Duplicate detection return status */
  nv_stat_enum_type  nv_status = NV_DONE_S;
    /* NV return status */
  byte start_list;
    /* Indicates the SMS list to start text matching */
  byte stop_list;
    /* Indicates the SMS list to stop text matching */
  byte old_line_len;
    /* Number of display lines required for stored SMS text */
  byte new_line_len;
    /* Number of display lines required for stored SMS text */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize return status and */
  /* duplicate message count.     */
  duplicate = FALSE;
  ui_sms_dup_cnt = 0;

  /* Indicate start and stop list */
  /* for text match.              */
  if ( unread )
  {
    /* Check against all new message lists */
    start_list = UI_SMS_TXT_NEW_URGENT;
    stop_list  = UI_SMS_TXT_NEW_NOT_URGENT;
  }
  /* Check against old message list */
  else
  {
    start_list = UI_SMS_TXT_OLD;
    stop_list  = UI_SMS_TXT_OLD;
  }

  /* Search for match in all lists */
  /* containing stored messages.   */
  for (i=start_list; i<=stop_list; i++)
  {
    /* Loop through each stored message */
    /* in the SMS list.  Start at the   */
    /* end of each list so the delete   */
    /* will not move the location of    */
    /* the next message to be deleted.  */
    for (j=ui_sms_list[i].cnt; j>0; j--)
    {
      /* Retrieve SMS NV index to access  */
      /* message's duplicate information. */
      ui_sms_nv_item.address = ui_sms_list [i].nv_idx [j-1];

      /* Read SMS message text from NV */
      nv_status = SKY_GetNV( NV_SMS_I, (nv_item_type *) &ui_sms_nv_item );

      /* Successfully retrieved SMS message */
      /* so check user data against new     */
      /* message's user data.               */
      if ( nv_status == NV_DONE_S )
      {
        /* Unpack SMS message from NV SMS packed format and */
        /* put message into duplicate detection buffer.     */
        ui_sms_nv_data ( FALSE, &ui_sms_nv_item, &ui_sms_detect_msg );

        /* Check for duplicate user data text or    */
        /* call back numbers if both messages have  */
        /* text and/or call back numbers.           */

        /* Check for messages with call back        */
        /* numbers with the same length and no      */
        /* user data text.                          */
        if ((( ui_sms_msg.sms_param_m & SMSI_CALL_BACK) != 0 ) &&
            (( ui_sms_detect_msg.sms_param_m & SMSI_CALL_BACK) != 0 ) &&
             ( ui_sms_msg.msg_data.call_back.num_fields == 
               ui_sms_detect_msg.msg_data.call_back.num_fields ) &&
            (( ui_sms_msg.sms_param_m & SMSI_USER_DATA) == 0 ) &&
            (( ui_sms_detect_msg.sms_param_m & SMSI_USER_DATA) == 0 ))
        {
          /* Initialize return value to indicate duplicate. */
          duplicate = TRUE;

          /* Check if numbers match.  If not, no duplicate. */
          /* Loop through call back numbers. */
          for (k=0; k<ui_sms_msg.msg_data.call_back.num_fields; k++)
          {
            if ( ui_sms_msg.msg_data.call_back.chari[k] != 
                 ui_sms_detect_msg.msg_data.call_back.chari[k] )
            {
              /* Not a duplicate message so stop */
              /* this search.                    */
              duplicate = FALSE;
              break;
            }
          }
///////////////////////////////////////////
          if ( duplicate )
          {
              if ( ( ( ui_sms_msg.sms_param_m & SMSI_MC_TIME_STAMP ) != 0 ) &&
                   ( ( ui_sms_detect_msg.sms_param_m & SMSI_MC_TIME_STAMP ) != 0 ) )
              {
                  if ( ( ui_sms_msg.msg_data.mc_time.yr  == ui_sms_detect_msg.msg_data.mc_time.yr  ) &&
                       ( ui_sms_msg.msg_data.mc_time.mon == ui_sms_detect_msg.msg_data.mc_time.mon ) &&
                       ( ui_sms_msg.msg_data.mc_time.day == ui_sms_detect_msg.msg_data.mc_time.day ) &&
                       ( ui_sms_msg.msg_data.mc_time.hrs == ui_sms_detect_msg.msg_data.mc_time.hrs ) &&
                       ( ui_sms_msg.msg_data.mc_time.min == ui_sms_detect_msg.msg_data.mc_time.min ) &&
                       ( ui_sms_msg.msg_data.mc_time.sec == ui_sms_detect_msg.msg_data.mc_time.sec ) )
                  {
                      duplicate = TRUE;
                  }
                  else
                  {
                      duplicate = FALSE;
                  }
              }
              else
              {
                  duplicate = FALSE;
              }
          }
//////////////////////////////////////////
        }
        /* Check for messages with user data text */
        /* with call back numbers with the same   */
        /* length or no call back numbers.        */
        else if ((( ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0 ) &&
                 (( ui_sms_detect_msg.sms_param_m & SMSI_USER_DATA) != 0 ) &&
               (((( ui_sms_msg.sms_param_m & SMSI_CALL_BACK) != 0 ) &&
                 (( ui_sms_detect_msg.sms_param_m & SMSI_CALL_BACK) != 0 ) &&
                  ( ui_sms_msg.msg_data.call_back.num_fields == 
                    ui_sms_detect_msg.msg_data.call_back.num_fields )) ||
                ((( ui_sms_msg.sms_param_m & SMSI_CALL_BACK) == 0 ) &&
                 (( ui_sms_detect_msg.sms_param_m & SMSI_CALL_BACK) == 0 ))))
        {
          /* Initialize return value to indicate duplicate. */
          duplicate = TRUE;

          /* If call back number available then check */
          /* if numbers match.  If not, no duplicate. */
          if (( ui_sms_msg.sms_param_m & SMSI_CALL_BACK) != 0 ) 
          {
            /* Loop through call back numbers. */
            for (k=0; k<ui_sms_msg.msg_data.call_back.num_fields; k++)
            {
              if ( ui_sms_msg.msg_data.call_back.chari[k] != 
                   ui_sms_detect_msg.msg_data.call_back.chari[k] )
              {
                /* Not a duplicate message so stop */
                /* this search.                    */
                duplicate = FALSE;
                break;
              }
            }
          }

//////////////////////////////////////////
          if ( duplicate )
          {
              if ( ( ( ui_sms_msg.sms_param_m & SMSI_MC_TIME_STAMP ) != 0 ) &&
                   ( ( ui_sms_detect_msg.sms_param_m & SMSI_MC_TIME_STAMP ) != 0 ) )
              {
                  if ( ( ui_sms_msg.msg_data.mc_time.yr  == ui_sms_detect_msg.msg_data.mc_time.yr  ) &&
                       ( ui_sms_msg.msg_data.mc_time.mon == ui_sms_detect_msg.msg_data.mc_time.mon ) &&
                       ( ui_sms_msg.msg_data.mc_time.day == ui_sms_detect_msg.msg_data.mc_time.day ) &&
                       ( ui_sms_msg.msg_data.mc_time.hrs == ui_sms_detect_msg.msg_data.mc_time.hrs ) &&
                       ( ui_sms_msg.msg_data.mc_time.min == ui_sms_detect_msg.msg_data.mc_time.min ) &&
                       ( ui_sms_msg.msg_data.mc_time.sec == ui_sms_detect_msg.msg_data.mc_time.sec ) )
                  {
                      duplicate = TRUE;
                  }
                  else
                  {
                      duplicate = FALSE;
                  }
              }
              else
              {
                  duplicate = FALSE;
              }
          }
//////////////////////////////////////////

          /* If call back numbers match, then */
          /* check user data text.            */
          if ( duplicate )
          {
            /* Clear out text before auto-format and text match. */
            (void) memset ( &ui_sms_detect_text, ' ', SMSI_USER_DATA_MAX );
            (void) memset ( &ui_sms_new_text, ' ', SMSI_USER_DATA_MAX );

            /* Auto-format stored message before comparing. */
            old_line_len = ui_sms_auto_format (
              FALSE,
              (byte *) ui_sms_detect_msg.msg_data.user_data.chari, 
              ui_sms_detect_msg.msg_data.user_data.num_fields,
              ui_sms_detect_text,
              SMSI_USER_DATA_MAX);

            /* Auto-format new message before comparing. */
            new_line_len = ui_sms_auto_format (
              FALSE,
              (byte *) ui_sms_msg.msg_data.user_data.chari, 
              ui_sms_msg.msg_data.user_data.num_fields,
              ui_sms_new_text,
              SMSI_USER_DATA_MAX);

            /* If lengths are not the same, then */
            /* message cannot be a duplicate.    */
            if ( old_line_len == new_line_len )
            {
              for (k=0; k<(old_line_len*UI_SMS_WIDE); k++)
              {
                /* If text character does not match stored message */
                /* then stop search and reset value to not a       */
                /* duplicate.                                      */
                if ( ui_sms_new_text[k] != ui_sms_detect_text[k] )
                {
                  /* Not a duplicate message so stop */
                  /* this search.                    */
                  duplicate = FALSE;
                  break;
                }
              }
            }
            /* Text lengths do not match. */
            else
            {
              /* Not a duplicate message. */
              duplicate = FALSE;
            }
          }
        }

        /* If duplicate found then mark */
        /* stored message for deletion. */
        if ( duplicate )
        {
          /* Reset duplicate message indicator. */
          duplicate = FALSE;

          /* Save location of duplicate message. */
          ui_sms_dup_list [ui_sms_dup_cnt].list_cat = i;
          ui_sms_dup_list [ui_sms_dup_cnt].order_idx = j-1;

          /* Increment duplicate message count */
          ui_sms_dup_cnt++;
        }
      }
    }
  }

  /* If duplicates found then   */
  /* delete duplicate messages. */
  if ( ui_sms_dup_cnt > 0 )
  {
    /* Indicate duplicates were found. */
    duplicate = TRUE;

    /* Loop through delete list */
    for (i=0; i<ui_sms_dup_cnt; i++)
    {
      /* Setup delete operation from list info. */
      /* Delete message or page with duplicate  */
      /* text.                                  */
      list_info.cmd = UI_SMS_DELETE;
      list_info.src_list = ui_sms_dup_list [i].list_cat;
      list_info.src_idx  = ui_sms_dup_list [i].order_idx;

      /* Delete SMS message or page from NV storage */
      (void) ui_sms_list_cmd ();
    }
  }

  /* Return text match detection status */
  return ( duplicate );
}

void ui_sms_save_list_text
(
void
)
{
  byte i;
    /* Loop counter index */
  byte num_len;
    /* Length of hyphenated number */
  byte num_pos;
    /* Starting position of hyphenated number */
  byte nv_addr;
    /* SMS NV address of current message */
#ifdef FEATURE_SKVM
  byte cnt=0;
#endif

    CHAR        cSeperate;
    UINT16      nMsgSize;
    BYTE*       pStartMsg;
    BYTE*       pSeperate;
    
    nMsgSize = 0;
    pStartMsg = NULL;
    i = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Setup current message's NV address */
  nv_addr = ui_sms_list [list_info.src_list].nv_idx [list_info.src_idx];

  /* Initialize source text info with blanks */
  (void) memset ( &ui_sms_text_list [nv_addr][0], NULL, UI_SMS_WIDE );
  (void) memset ( number_buf, NULL, UI_SMS_WIDE*3 );
  (void) memset ( alpha_buf, NULL, UI_SMS_WIDE );

  /* Set stored message's message id.                        */
  /* Only messages with valid message ids will checked for   */
  /* duplicate detection.  Zero is not a valid message id.   */
  ui_sms_detect_list [nv_addr].message_id = 
    list_info.sms_msg->sms_msg_id.message_id;

  /* Check if Message Center timestamp is available and */
  /* message has valid message id.                      */
  if ((( list_info.sms_msg->sms_param_m & SMSI_MC_TIME_STAMP) != 0) &&
       ( list_info.sms_msg->sms_msg_id.message_id != 0 ))
  {
    /* Indicate Message Center timestamp needs to */
    /* be checked for duplicate detection.        */
    ui_sms_detect_list [nv_addr].check_time = TRUE;
    ui_sms_detect_list [nv_addr].mc_time = 
      list_info.sms_msg->msg_data.mc_time;
  }
  /* No Message Center timestamp available. */
  else
  {
    /* Do not check Message Center timestamp */
    /* for duplicate detection.              */
    ui_sms_detect_list [nv_addr].check_time = FALSE;
  }

  /* Save first line of user data in SMS source list data */
  /* if message teleservice.                              */
  if (( list_info.sms_msg->tele_srv == SMSI_CPT_95) || 
       ( list_info.sms_msg->tele_srv == SMSI_CMT_95 ) ||
       ( list_info.sms_msg->tele_srv == SMSI_SHINSEGI_WAP_PUSH) ||
       ( list_info.sms_msg->tele_srv == SMSI_WAP_PUSH) ||
       ( list_info.sms_msg->tele_srv == SMSI_WAP_MAIL ) ||
#ifdef FEATURE_SKVM
       ( list_info.sms_msg->tele_srv == SMSI_SKVM_LOAD ) ||
#endif  
       ( list_info.sms_msg->tele_srv == SMSI_CMT_91_SHORT_MSG ) ||
       /* undersea 2001-07-09 --> @why, what */
#ifdef FEATURE_GVM
       ( list_info.sms_msg->tele_srv == SMSI_GVM ) || 
#endif
       /* <-- */
       ( list_info.sms_msg->tele_srv == SMSI_AWI_95 ))
  {
     
    /* Format user data for scroll list display  */
    /* into temporary buffer to determine format */
    /* length. Do not word wrap in auto-format.  */                  
    /* num_len is the number of display lines,   */
    /* not the number of characters or bytes.    */
    if((list_info.sms_msg->sms_param_m & SMSI_USER_DATA) != 0) 
    {
        if( list_info.sms_msg->tele_srv == SMSI_WAP_PUSH ||
            list_info.sms_msg->tele_srv == SMSI_SHINSEGI_WAP_PUSH ||
            list_info.sms_msg->tele_srv == SMSI_WAP_MAIL ) 
        {
            if(list_info.sms_msg->tele_srv == SMSI_SHINSEGI_WAP_PUSH)
            {
                cSeperate = SEPERATE_SHINSEGI;
            }
            else
            {
                cSeperate = SEPERATE;
            }

            pSeperate = (BYTE *)memchr((BYTE *)list_info.sms_msg->msg_data.user_data.chari,
                                cSeperate, list_info.sms_msg->msg_data.user_data.num_fields);

            for( i = 0; i < ui_sms_msg.msg_data.user_data.num_fields; i++) 
            {
                if(ui_sms_msg.msg_data.user_data.chari[i] == cSeperate)
                {
                    break;
                }
            }
            i++;

            if(list_info.sms_msg->tele_srv == SMSI_SHINSEGI_WAP_PUSH)
            {
                pStartMsg = (BYTE *)list_info.sms_msg->msg_data.user_data.chari;
                nMsgSize = i - 1;                    
            }
            else    //wap push/wap mail
            {
                if(pSeperate == NULL)
                {
                    (void) memcpy ( (void *)number_buf, 
                                (BYTE*)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT),
                                STRLEN((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT))); 
                    (void) memcpy ( &ui_sms_text_list [nv_addr][1], number_buf,
                                        (UI_SMS_WIDE - 1) );                
                }
                else
                {
                    pStartMsg = pSeperate + 1; 
                    nMsgSize = list_info.sms_msg->msg_data.user_data.num_fields - i;                
                }
            }

            if(pStartMsg != NULL)
            {                
                if(nMsgSize > MAX_VIEW_BUF_COLUME_SIZE-1)
                    nMsgSize = MAX_VIEW_BUF_COLUME_SIZE-1;
                num_len = ui_sms_auto_format_one_line (
                                            MAX_VIEW_BUF_COLUME_SIZE-1,
                                            (byte *) pStartMsg, 
                                            nMsgSize,
                                            number_buf, MAX_VIEW_BUF_COLUME_SIZE-1 );           
            }

        }//wapmail or wappush 
        /* undersea 2001-07-09 
            --> SMS를 통한 GVM 다운로드 중
                SMS 리스트를 보여줄때 문자열 설명만 뽑아서 보여주기 위함
        */
#ifdef FEATURE_GVM
        else if (list_info.sms_msg->tele_srv == SMSI_GVM )   {
            BYTE *pMsgBuf;
            BYTE *pBuf;

            pMsgBuf = (BYTE*)list_info.sms_msg->msg_data.user_data.chari;
            pMsgBuf += 5; // "SWAP,"
            pBuf = STRTOK(pMsgBuf, (CONST BYTE*)",");

            num_len = ui_sms_auto_format_one_line (
                                    MAX_VIEW_BUF_COLUME_SIZE-1,
                                    pBuf,
                                    //(byte *) list_info.sms_msg->msg_data.user_data.chari, 
                                    list_info.sms_msg->msg_data.user_data.num_fields,
                                    number_buf, MAX_VIEW_BUF_COLUME_SIZE-1 );
        }        
#endif //FEATURE_GVM       
#ifdef	FEATURE_SKVM
		else if (list_info.sms_msg->tele_srv == SMSI_SKVM_LOAD)
		{
            byte *pMsgBuf;
            pMsgBuf = (byte*)list_info.sms_msg->msg_data.user_data.chari;
			for( i = 0; i < ui_sms_msg.msg_data.user_data.num_fields; i++) 
            {
				if (*pMsgBuf == MSD_SEPERATOR)
					break;
				pMsgBuf++;
			}
			pMsgBuf++;
			num_len = ui_sms_auto_format_one_line (
                                    MAX_VIEW_BUF_COLUME_SIZE-1,
                                    (byte*)pMsgBuf,
                                    list_info.sms_msg->msg_data.user_data.num_fields,
                                    number_buf, MAX_VIEW_BUF_COLUME_SIZE-1 );
		}
#endif  //FEATURE_SKVM
        else 
        {
            num_len = ui_sms_auto_format_one_line (
                                    MAX_VIEW_BUF_COLUME_SIZE-1,
                                    (byte *) list_info.sms_msg->msg_data.user_data.chari, 
                                    list_info.sms_msg->msg_data.user_data.num_fields,
                                    number_buf, MAX_VIEW_BUF_COLUME_SIZE-1 );

        }

        (void) memcpy ( &ui_sms_text_list [nv_addr][1], number_buf,
                            (UI_SMS_WIDE - 1) );

    }
    else 
    {
        (void) memcpy ( (void *)number_buf, 
                    (BYTE*)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT),
                    STRLEN((BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT))); 
        (void) memcpy ( &ui_sms_text_list [nv_addr][1], number_buf,
                            (UI_SMS_WIDE - 1) );
    }    
  }
  /* Save number of messages for voice mail in text portion */
  /* of list data.                                          */
  else if ((( ui_sms_msg.tele_srv == SMSI_VMN_95 ) ||
            ( ui_sms_msg.tele_srv == SMSI_CMT_91_VOICE_MAIL ) ||
            ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
           (( ui_sms_msg.sms_param_m & SMSI_NUM_OF_MSGS) != 0))
  {
    (void) memcpy ( &ui_sms_text_list [nv_addr][1],
                        &list_info.sms_msg->msg_data.num_of_msgs, 
                        sizeof (smsi_num_of_msgs_type) );
  }

    // For Priority & Protect ...
    ui_sms_text_list [nv_addr][0] = list_info.protect;  // 보호 or 해제 
}

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_LIST_CMD

DESCRIPTION
  Command edits to the SMS lists - delete item from list,
  move item from one list to another and add new item to list.
  NV indicies (1-23) are assigned to separate SMS lists.  The
  pool of indicies are never deleted from or added to.  They
  are moved from one SMS list to another depending on how their
  SMS list status changes.  NV index is reserved for storing the 
  SMS control list - this list indicates which of the other
  indicies (1-23) is assigned to a specific SMS list.  Commands
  to NV will be issued for adding new item (allocating/writing) or 
  deleting (freeing) an item.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
ui_sms_op_status_type ui_sms_list_cmd
(
void
)
{
  byte i;
    /* Loop counter index */
  nv_stat_enum_type  nv_status = NV_DONE_S;
    /* NV return status */
  ui_sms_op_status_type status;
    /* SMS operational return status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize operational status */
  status = UI_SMS_OP_IN_PROG;

  /* if deleting an item from a list, then  */
  /* free item allocation in NV.            */
  /* Using specified source list and index. */
  if ( list_info.cmd == UI_SMS_DELETE ) 
  {
    if ( ui_sms_list [list_info.src_list].cnt > list_info.src_idx )
    {
      /* Setup nv item address to be freed */
      ui_sms_nv_item.address = 
        ui_sms_list [list_info.src_list].nv_idx [list_info.src_idx];

      /* Free only valid addresses */
      if ( ui_sms_nv_item.address <= MAX_NV_MSGS )
      {
        /* Free memory allocation for specified item stored in NV. */
        nv_status = SKY_FreeNV( NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item );

        /* Allow delete even if SMS NV free operation is not */
        /* successful so NV address is not orphaned on valid */
        /* message list.                                     */
        /* Setup list info for freed address to be returned  */
        /* available list.                                   */
        /* Always return the freed NV address to the bottom  */
        /* of the sms available list.                        */
        list_info.dest_list = UI_SMS_AVAIL;
        list_info.dest_idx = ui_sms_list [UI_SMS_AVAIL].cnt;
      }
      else
      {
        status = UI_SMS_OP_NV_ERR;
      }
    }
    /* No NV indicies available to free */
    /* from indicated SMS list so set   */
    /* status to indicate that no NV    */
    /* index was available.             */
    else
      status = UI_SMS_OP_NO_IDX;
  }

  /* if adding a new item to a list, then */
  /* allocate and write item in NV.       */
  /* Using specified destination list.    */
  if ( list_info.cmd == UI_SMS_ADD )
  {
    /* one slot is reserved for voice mail.
     * If dest_list is VC, ui_sms_list[UI_SMS_AVAIL].cnt is at least 1,
     * since the VC entry is already deleted prior to this operation.
     */
    if( ui_sms_list[UI_SMS_AVAIL].cnt > 1 ||
        list_info.dest_list == UI_SMS_VC  ||  /* adding VC is always possible */
        ( list_info.dest_list!=UI_SMS_VC &&
          ui_sms_list[UI_SMS_AVAIL].cnt==1 &&
          ui_sms_list[UI_SMS_VC].cnt==1 ) )
                           /* if not adding VC, avail_cnt==vc_cnt==1 is fine */
    {
      /* Pack rxd SMS message into NV SMS format and update */
      /* the new nv item information.                       */
      ui_sms_nv_data ( TRUE, &ui_sms_nv_item, list_info.sms_msg );

      /* Always pick the next available NV address from the top */
      /* of the sms available list.                             */
      ui_sms_nv_item.address = ui_sms_list [UI_SMS_AVAIL].nv_idx [0];

	  if((list_info.sms_msg->sms_param_m & SMSI_PRIORITY) != 0)
		  ui_sms_nv_item.protect = 1;  // 문자메시지 보호기능을 위해서
	  else
		  ui_sms_nv_item.protect = 0;  // 문자메시지 보호기능을 위해서

      /* Write new item into NV. */
      nv_status = SKY_PutNV( NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item );

      if ( nv_status == NV_DONE_S )
      {
        /* Setup list information for sms list manipulation */
        list_info.src_list = UI_SMS_AVAIL;
        list_info.src_idx = 0;
        list_info.dest_idx = ui_sms_list [list_info.dest_list].cnt;
        list_info.protect = ui_sms_nv_item.protect;

        /* Save first line of user data or call back number */
        /* in SMS source list data.                         */
        ui_sms_save_list_text ();
      }
      /* Space not available in NV then */
      /* set status to indicate that no */
      /* NV index was available.        */
      else if ( nv_status == NV_FULL_S )
      {
        status = UI_SMS_OP_NO_IDX;
      }
      /* All other NV statuses are considered */
      /* errors.                              */
      else
      {
        status = UI_SMS_OP_NV_ERR;
      }
    }
    /* Ran out of available NV indicies so */
    /* set status to indicate that no NV   */
    /* index was available.                */
    else
      status = UI_SMS_OP_NO_IDX;
  }
  
  /* If no errors encountered, then move index from source list to */
  /* destination list and repack source list items.                */
  if ( status == UI_SMS_OP_IN_PROG ) 
  {
    /* move NV address from source list to destination list */
    ui_sms_list [list_info.dest_list].nv_idx [list_info.dest_idx] =
      ui_sms_list [list_info.src_list].nv_idx [list_info.src_idx];

    /* Increment destination list item count */
    ui_sms_list [list_info.dest_list].cnt++;

    /* repack source file items below the item removed */
    for (i=(list_info.src_idx+1); i<ui_sms_list [list_info.src_list].cnt; i++)
    {
      ui_sms_list [list_info.src_list].nv_idx [i-1] =
        ui_sms_list [list_info.src_list].nv_idx [i];
    }

    /* Decrement source list item count */
    ui_sms_list [list_info.src_list].cnt--;

    /* If available list count indicates no */
    /* indicies, then update SMS control    */
    /* information to indicate no indicies  */
    /* available.  If available list count  */
    /* was none available and now has an    */
    /* available index, then update SMS     */
    /* control information to indicate that */
    /* SMS indicies are available.          */
    /* To minimize the number of NV writes, */
    /* this indication will only be updated */
    /* when the available index count       */
    /* toggles to and from 0.  MC task only */
    /* needs to know if it has available    */
    /* indicies, not exactly how many are   */
    /* available.                           */
    if ((( list_info.src_list == UI_SMS_AVAIL ) &&
         ( ui_sms_list [list_info.src_list].cnt == 0 ) &&
         ( list_info.cmd == UI_SMS_ADD )) ||
        (( list_info.dest_list == UI_SMS_AVAIL ) &&
         ( ui_sms_list [list_info.dest_list].cnt == 1 ) &&
         ( list_info.cmd == UI_SMS_DELETE )))
    {
      /* Update SMS control information with  */
      /* indication of no available indicies. */
      /* Write control info into NV using     */
      /* power-down function.                 */
      (void) ui_sms_power_down ();
    }

    /* SMS operation successfully completed */
    status = UI_SMS_OP_OK;
  }

  return( status );
}

#ifdef FEATURE_SKTT_EQUS_LOCATION

UINT8 WhatLocationType(VOID)
{
    if (ui_sms_msg.msg_data.user_data.num_fields > 0)
	{
		if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_REMOTE_SETTING)
			return LOCATION_REMOTE_SETTING;
		else if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_INFORM)
            return LOCATION_INFORM;
        else if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_DELETE_SETTING)
            return LOCATION_DELETE_SETTING;
        else if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_INFORM_OFF)
            return LOCATION_INFORM_OFF;
        else if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_INFORM_IN_CALL)
            return LOCATION_INFORM_IN_CALL;
        else if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_INFORM_REQUEST)
			return LOCATION_INFORM_REQUEST;
        else
            return 0x00;
	}
	else	return 0x00;
}
/*===========================================================================
FUNCTION uis_SetRcvLocationSmsMsg

DESCRIPTION

DEPENDENCIES

SIDE EFFECTS
===========================================================================*/
WORD SetRcvLocationSmsMsg
(
  ui_cmd_type *cmd_ptr
)
{
	LOCATION_INFO_T		location;
	BYTE i, temp, u_bit, l_bit;
  
	ui_sms_op_status_type    status;  
	
	/* Initialize SMS operational status */
	status = UI_SMS_OP_IN_PROG;

	/* Copy new message to regional data */
	memcpy ( &ui_sms_msg, &cmd_ptr->sms.buf_ptr->data,sizeof( smsi_msg_type ));

	if (ui_sms_msg.msg_data.user_data.num_fields == 0)		// No data
	{
		SKY_TRACE(TL_HIGH,"SMS num_field ERROR");
		// release SMS buffer
		if (cmd_ptr->sms.buf_ptr != NULL)
		{
			 q_put(&ui_sms_free_q, &cmd_ptr->sms.buf_ptr->link);
			 cmd_ptr->sms.buf_ptr = NULL;
		}
		return(status);
	}

	location = SKY_GET_LOCATION_INFO();

	if (ui_sms_msg.msg_data.user_data.num_fields > 0) {
		
		SKY_TRACE1(TL_HIGH, "sms message = %d",ui_sms_msg.msg_data.user_data.chari[0]);

		if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_REMOTE_SETTING ) // LAM3 원격설정 message
		{
			status = UI_SMS_OP_OK;

            if(!location.rcvinfo)   
                location.onoffstate = 0x00;
            if(location.lam3ver == ui_sms_msg.msg_data.user_data.chari[1])
                return(status);

			location.rcvinfo = 0x01;
			location.lam3ver = ui_sms_msg.msg_data.user_data.chari[1];
			
			if(ui_sms_msg.msg_data.user_data.chari[2] < 2)
				ui_sms_msg.msg_data.user_data.chari[2] = 2;  // 최소 주기는 2분

			location.cycle = ui_sms_msg.msg_data.user_data.chari[2];
			// Timer ON
			SKY_SetLocationTimer ((int)(location.cycle * 60 * 1000), TRUE);

			u_bit = ui_sms_msg.msg_data.user_data.chari[3] & 0xf0;
			if(u_bit == 0xa0 )
				u_bit = 0;
			l_bit = ui_sms_msg.msg_data.user_data.chari[3] & 0x0f;
			if(l_bit == 0x0a)
				l_bit = 0;
			
			temp = ((u_bit >> 4) * 10) + l_bit;
			location.starttime = temp;
					
			u_bit = ui_sms_msg.msg_data.user_data.chari[4] & 0xf0;
			if(u_bit == 0xa0 )
				u_bit = 0;
			l_bit = ui_sms_msg.msg_data.user_data.chari[4] & 0x0f;
			if(l_bit == 0x0a)
				l_bit = 0;
			
			temp = ((u_bit >> 4) * 10) + l_bit;
			location.startmin = temp;
			
			u_bit = ui_sms_msg.msg_data.user_data.chari[5] & 0xf0;
			if(u_bit == 0xa0 )
				u_bit = 0;
			l_bit = ui_sms_msg.msg_data.user_data.chari[5] & 0x0f;
			if(l_bit == 0x0a)
				l_bit = 0;
			
			temp = ((u_bit >> 4) * 10) + l_bit;
			location.endtime = temp;
					
			u_bit = ui_sms_msg.msg_data.user_data.chari[6] & 0xf0;
			if(u_bit == 0xa0 )
				u_bit = 0;
			l_bit = ui_sms_msg.msg_data.user_data.chari[6] & 0x0f;
			if(l_bit == 0x0a)
				l_bit = 0;
			
			temp = ((u_bit >> 4) * 10) + l_bit;
			location.endmin = temp;

			for (i = 0; i < 5; i++) {
				location.cfrmid[i] = ui_sms_msg.msg_data.user_data.chari[7+i];
			}

//Write File
			SKY_SET_LOCATION_INFO(location);									
		}
		else if (ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_DELETE_SETTING ) // LAM3 삭제 message
		{
			status = UI_SMS_OP_OK;

			location.lam3ver = 0x00;
			location.rcvinfo = 0x00;
			location.onoffstate = 0x00;

			SKY_SET_LOCATION_INFO(location);

			// Timer OFF
			SKY_SetLocationTimer ((int)0, FALSE);
		}
		else if(ui_sms_msg.msg_data.user_data.chari[0] == LOCATION_INFORM_REQUEST) // 즉시 위치통보
		{
			location.reportwait = 0x01;

			location.lastpn =     0;
			location.lastsid =    0;	
			location.lastnid =    0;
			location.lastzone =   0;
			location.lastbaseid = 0;

			SKY_SET_LOCATION_INFO(location);	
			
			SKY_SetLocationTimer ((int)(3 * 1000), FALSE);
		} 
	}

	// release SMS buffer
	if (cmd_ptr->sms.buf_ptr != NULL)
	{
		 q_put(&ui_sms_free_q, &cmd_ptr->sms.buf_ptr->link);
		 cmd_ptr->sms.buf_ptr = NULL;
	}

	return(status);
}
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

#ifdef FEATURE_SKVM
/*===========================================================================
FUNCTION SetRcvSKVMDataSmsMsg

DESCRIPTION

DEPENDENCIES

SIDE EFFECTS
===========================================================================*/
WORD SetRcvSKVMDataSmsMsg(ui_cmd_type *cmd_ptr)
{
	ui_sms_op_status_type    status;  
	
	/* Initialize SMS operational status */
	status = UI_SMS_OP_IN_PROG;

	/* Copy new message to regional data */
	memcpy ( &ui_sms_msg, &cmd_ptr->sms.buf_ptr->data,sizeof( smsi_msg_type ));

	if ((ui_sms_msg.msg_data.user_data.num_fields == 0)||		// No data
		(ui_sms_msg.msg_data.user_data.num_fields > 80))		// Size Over
	{
		SKY_TRACE(TL_HIGH,"SMS num_field ERROR");
		// release SMS buffer
		if (cmd_ptr->sms.buf_ptr != NULL)
		{
			 q_put(&ui_sms_free_q, &cmd_ptr->sms.buf_ptr->link);
			 cmd_ptr->sms.buf_ptr = NULL;
		}
		return(status);
	}
	
	// Application Data 저장 
	memset((void *)g_pszSKVMInputData,0x00,DATA_MAX);
    memcpy((void *)g_pszSKVMInputData,(void *)&ui_sms_msg.msg_data.user_data.chari[0],ui_sms_msg.msg_data.user_data.num_fields); 
	g_iSKVMInputDataLen = ui_sms_msg.msg_data.user_data.num_fields;
	status = UI_SMS_OP_OK;
	return status;
}
#endif //FEATURE_SKVM
/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_STORE_NEW_MSG

DESCRIPTION
  Store new received SMS message in NV and display appropriate
  icon.  Blink icon if message indicates urgent priority.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
word ui_sms_store_new_msg
(
  UI_COMMAND_U *pCommand
)
{
  ui_sms_op_status_type    status;
    /* SMS operation status */
  word    nv_memory;
  byte protect_flag = FALSE, i;
  BYTE szTemp[SMSI_CALL_BACK_MAX + 1];  
  BYTE szTmpNameBuffer[1];

  szTmpNameBuffer[0] = '\0';


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize SMS operational status */
  status = UI_SMS_OP_IN_PROG;

  memset(&ui_sms_msg, NULL, sizeof( smsi_msg_type ));        
  /* Copy new message to regional data */
  (void) memcpy ( &ui_sms_msg, &pCommand->sms.buf_ptr->data,sizeof( smsi_msg_type ));
    
  // 착발신 이력에 저장 

  memset(szTemp, NULL, SMSI_CALL_BACK_MAX+1);
  if((ui_sms_msg.sms_param_m & SMSI_CALL_BACK) != 0)
  {  
    memcpy(szTemp, (byte *)ui_sms_msg.msg_data.call_back.chari,
                  ui_sms_msg.msg_data.call_back.num_fields);
    if(IsConfidentialGroup((BYTE *)szTemp)!= PB_SECRET_OK)
    {
      AddCallsList(CT_SMS_MT, szTemp, ui_sms_msg.msg_data.call_back.num_fields, szTmpNameBuffer);
//      AddCallsList(SMS_MT, szTemp, ui_sms_msg.msg_data.call_back.num_fields);
    }
  }  

#if 0  
  if (( ui_sms_msg.sms_param_m & SMSI_CALL_BACK) != 0) 
  {
      status = UI_SMS_OP_IN_PROG;
  }
  else
  {
      status = UI_SMS_OP_IN_PROG;
  }
#endif
  /* Check for duplicate message. */
  if ( ui_sms_duplicate () )
  {
    /* new sms buffer system define by NEW_SMS. Used only with uin* files */
    /* release SMS buffer */
    if (pCommand->sms.buf_ptr != NULL)
    {
        q_put(&ui_sms_free_q, &pCommand->sms.buf_ptr->link);
        pCommand->sms.buf_ptr = NULL;
    }
    else
    {
#ifndef WIN32
        ERR("Bad sms buffer pointer in UI", 0,0,0);
#endif // ndef WIN32
    }
    
    /* Indicate duplicate message found. */
    status = UI_SMS_OP_DUPLICATE;
  }
  /* Not a duplicate message,       */
  /* process and store new message. */
  else
  {
    /* Initialize message status before storing in NV. */
    ui_sms_nv_item.status = 0;

    if((ui_sms_msg.sms_param_m & SMSI_PRIORITY) != 0)
        ui_sms_nv_item.protect = 1;  // 문자메시지 보호기능을 위해서
    else
        ui_sms_nv_item.protect = 0;  // 문자메시지 보호기능을 위해서

    /* Determine if new message is voice by checking */
    /* SMS teleservice and if number of message      */
    /* parameter is present.                         */
    if ((( ui_sms_msg.tele_srv == SMSI_VMN_95 ) ||
         ( ui_sms_msg.tele_srv == SMSI_CMT_91_VOICE_MAIL ) ||
         ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
        (( ui_sms_msg.sms_param_m & SMSI_NUM_OF_MSGS) != 0))
    {

        if (!(( ui_sms_msg.msg_data.num_of_msgs[0] == 0x30) &&
            ( ui_sms_msg.msg_data.num_of_msgs[1] == 0x30))) 
        {
	        g_NewVmsSave(pCommand);  // Save VMS
        }

      /* Delete any existing voice mail.  Only */
      /* one voice mail can exists at a time.  */
      if ( ui_sms_list [UI_SMS_VC].cnt > 0 ) 
      {
        /* Setup delete operation from SMS list. */
        /* Delete any existing voice mail */
        list_info.cmd = UI_SMS_DELETE;
        list_info.src_list = UI_SMS_VC;
        list_info.src_idx = 0;

        /* Delete SMS voice mail from NV storage */
        status = ui_sms_list_cmd ();
      }

      /* Setup list info for NV write and list entry setup */
      list_info.cmd = UI_SMS_ADD;

      /* Default voice mail list */
      list_info.dest_list = UI_SMS_VC;
    }
    /* Page or message received */
    else
    {
        /* new sms buffer system define by NEW_SMS. Used only with uin* files */
        /* release SMS buffer */
        if (pCommand->sms.buf_ptr != NULL)
        {
            q_put(&ui_sms_free_q, &pCommand->sms.buf_ptr->link);
            pCommand->sms.buf_ptr = NULL;
        }
        else
        {
    #ifndef WIN32
            ERR("Bad sms buffer pointer in UI", 0,0,0);
    #endif // ndef WIN32
        }

      /* Check for duplicate text against all new */
      /* stored messages.  Do text match before   */
      /* setting destination list information     */
      /* since text match may change destination  */
      /* list information.                        */
      if ( ui_sms_text_match ( TRUE ))
      {
        /* Tag message to be stored as a duplicate. */
        ui_sms_nv_item.status |= UI_SMS_STS_DUPLICATE;
      }

/////////////////////////////////////////////////////////////////////////////////
//      ADD FOR SK Teletech SMS Structure huni 2000.12.28
/////////////////////////////////////////////////////////////////////////////////

      nv_memory = nv_memavail();

      if ( ui_sms_list [UI_SMS_TXT_OLD].cnt + ui_sms_list [UI_SMS_TXT_NEW_URGENT].cnt 
                                            + ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].cnt >= MAX_SMS_SIZE 
					|| ( nv_memory < SMS_MINIMUM_MEMORY ))  /* Calculate NV memory used for SMS 350 */
      {
        /* Setup delete operation from SMS list. */
        list_info.cmd = UI_SMS_DELETE;
		if(ui_sms_list [UI_SMS_TXT_OLD].cnt > 0) 
			list_info.src_list = UI_SMS_TXT_OLD;
		else if(ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0)
			list_info.src_list = UI_SMS_TXT_NEW_NOT_URGENT;
		else
			list_info.src_list = UI_SMS_TXT_NEW_URGENT;
        list_info.src_idx = 0;

		if(ui_sms_list [UI_SMS_TXT_OLD].cnt > 0) {
			for(i=0; i < ui_sms_list [UI_SMS_TXT_OLD].cnt; i++) {
				ui_sms_nv_item.address = ui_sms_list [UI_SMS_TXT_OLD].nv_idx [i];
			    SKY_GetNV (NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item);             
				if(ui_sms_nv_item.protect < 2) {
					list_info.src_list = UI_SMS_TXT_OLD;
					list_info.src_idx = i;
					protect_flag = TRUE;
					break;
				}
			}
		}

		if(ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0 && protect_flag == FALSE) {
			for(i=0; i < ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].cnt; i++) {
				ui_sms_nv_item.address = ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].nv_idx [i];
			    SKY_GetNV (NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item);             
				if(ui_sms_nv_item.protect < 2) {
					list_info.src_list = UI_SMS_TXT_NEW_NOT_URGENT;
					list_info.src_idx = i;
					protect_flag = TRUE;
					break;
				}
			}
		}

		if(ui_sms_list [UI_SMS_TXT_NEW_URGENT].cnt > 0 && protect_flag == FALSE) {
			for(i=0; i < ui_sms_list [UI_SMS_TXT_NEW_URGENT].cnt; i++) {
				ui_sms_nv_item.address = ui_sms_list [UI_SMS_TXT_NEW_URGENT].nv_idx [i];
			    SKY_GetNV (NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item);             
				if(ui_sms_nv_item.protect < 2) {
					list_info.src_list = UI_SMS_TXT_NEW_URGENT;
					list_info.src_idx = i;
					protect_flag = TRUE;
					break;
				}
			}
		}

		/* Perform the deletion. */
		(void) ui_sms_list_cmd();
	  }
/////////////////////////////////////////////////////////////////////////////////

      /* Default text list to not urgent */
      list_info.dest_list = UI_SMS_TXT_NEW_NOT_URGENT;

      /* Change list if message is marked urgent or emergency */
      if ((( ui_sms_msg.sms_param_m & SMSI_PRIORITY) != 0) &&
           ( ui_sms_msg.msg_data.priority >= UI_SMS_URGENT_IND))
      {
        list_info.dest_list = UI_SMS_TXT_NEW_URGENT;
      }

      /* Set add command.  Delete may have been */
      /* performed during text matching so set  */
      /* after text matching.                   */
      list_info.cmd = UI_SMS_ADD;
    }

    /* Save new message to NV and add NV info to SMS list */
    status = ui_sms_list_cmd ();

    /* If SMS message was successfully saved then  */
    /* determine which and how to display the icon */
    if ( status == UI_SMS_OP_OK )
    {
      /* Update reading bookmark information for new */
      /* SMS message.                                */
      ui_sms_read_info.new_read_list = list_info.dest_list;
      ui_sms_read_info.new_read_idx  = list_info.dest_idx;

      /* If adding message or page, then set */
      /* indicator to check text against old */
      /* stored messages.                    */
      if (( list_info.cmd == UI_SMS_ADD ) &&
         (( list_info.dest_list == UI_SMS_TXT_NEW_URGENT ) ||
          ( list_info.dest_list == UI_SMS_TXT_NEW_NOT_URGENT )))
      {
        /* Indicate message needs to be text matched */
        /* against stored old messages.              */
        ui_sms_detect_list
          [ui_sms_list [list_info.dest_list].nv_idx[list_info.dest_idx]].
          match_old_text = TRUE;
      }

      /* Determine what icon should be displayed and */
      /* if the message is marked urgent then blink  */
      /* icon.                                       */


      /* Update the icons */
      ui_sms_update_icons ();
      
      /* Set alert reminder */
      ui_sms_alert_status = TRUE;

  
      if ((( ui_sms_msg.tele_srv == SMSI_VMN_95 ) ||
           ( ui_sms_msg.tele_srv == SMSI_CMT_91_VOICE_MAIL ) ||
           ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
          (( ui_sms_msg.sms_param_m & SMSI_NUM_OF_MSGS) != 0))
      {
        /* If no voice mail messages, turn off voice mail icon */
        if (( ui_sms_msg.msg_data.num_of_msgs[0] == 0x30) &&
            ( ui_sms_msg.msg_data.num_of_msgs[1] == 0x30)) 
        {
          /* Reset alert reminder */
          ui_sms_alert_status = FALSE;
        }
      }

      /* Perform the SMS power down processing which  */
      /* saves SMS control list in NV.  Rewrite SMS   */
      /* control list to save current message status. */
      (void) ui_sms_power_down ();
    }
  }
  /* Return operational status */
  return ( status );
}

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_READ_VOICE

DESCRIPTION
  Determines if there are any voice mail to be read.  The message read
  will be unpacked and placed in ui_sms_msg.  Destination list and
  index in list info gives the location of the current message that
  was read.
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_sms_read_voice 
(
  ui_sms_read_op_type *read_op
    /* Read operation information */
)
{
  nv_stat_enum_type  nv_status = NV_DONE_S;
    /* NV return status */
  ui_sms_op_status_type status;
    /* SMS operation return status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize status */
  status = UI_SMS_OP_IN_PROG;

  /* Initialize return read operation info */
  read_op->new = TRUE;
  read_op->msg_read = FALSE;

  /* Initialize source list information for */
  /* SMS list manipualation and reading     */
  /* message from NV.                       */
  list_info.src_list = UI_SMS_VC;
  list_info.src_idx = 0;

  if ( ui_sms_list [UI_SMS_VC].cnt > 0 ) 
  {
    /* Indicate message to be read */
    read_op->msg_read = TRUE;
    read_op->new = FALSE;
  }

  /* Stored voice mail found, read message from NV */
  if ( read_op->msg_read )
  {
    /* Read SMS message from NV */
    ui_sms_nv_item.address = 
      ui_sms_list [list_info.src_list].nv_idx[list_info.src_idx];

    nv_status = SKY_GetNV( NV_SMS_I, (nv_item_type *) &ui_sms_nv_item );

    /* Successfully retrieved SMS message */
    if ( nv_status == NV_DONE_S )
    {
      /* Unpack SMS message from NV SMS packed format  */
      /* and put message into list information.        */
      ui_sms_nv_data ( FALSE, &ui_sms_nv_item, list_info.sms_msg );
    }
    /* NV read error reported */
    else
    {
      /* Reset read return status */
      read_op->msg_read = FALSE;

      /* Return unreadable SMS message */
      /* to available SMS list.        */
      list_info.cmd = UI_SMS_DELETE;

      /* Delete SMS message from NV storage */
      status = ui_sms_list_cmd ();
    }
  }
}

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_READ_TEXT

DESCRIPTION
  Determines if there are any text messages to be read from new or
  old lists.  Read operation indicates how the SMS lists should be
  searched for the next read and if a text message was read and if
  it was from a new (unread) or old (read) list.  The message read
  will be unpacked and placed in ui_sms_msg.  Destination list and
  index in list info gives the location of the current message that
  was read.
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_sms_read_text 
(
  ui_sms_read_op_type *read_op
    /* Read operation information */
)
{
  nv_stat_enum_type  nv_status = NV_DONE_S;
    /* NV return status */
  ui_sms_op_status_type status;
    /* SMS operation return status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize status */
  status = UI_SMS_OP_IN_PROG;

  /* Initialize return read operation info */
  read_op->new = TRUE;
  read_op->msg_read = FALSE;

  /* Initialize source list information for */
  /* SMS list manipualation and reading     */
  /* message from NV.                       */
  if (( read_op->cmd == UI_SMS_READ_ONE ) ||
      ( read_op->cmd == UI_SMS_READ_NOTIF ))
  {
    list_info.src_list = read_op->read_list;
  }
  else
  {
    list_info.src_list = UI_SMS_TXT_OLD;
  }

  /* Determine which list to read from and */
  /* indicate if text message is present.  */
  if (( ui_sms_list [read_op->read_list].cnt > 0 ) &&
     (( read_op->cmd == UI_SMS_READ_ONE ) ||
      ( read_op->cmd == UI_SMS_READ_NOTIF )))
  {
    list_info.src_idx = read_op->read_idx;

    /* Indicate if message is from the new or old list */
    if ( read_op->read_list == UI_SMS_TXT_OLD )
    {
      read_op->new = FALSE;
    }
    else
    {
      read_op->new = TRUE;
    }

    /* Indicate message to be read */
    read_op->msg_read = TRUE;
  }
  else if (( ui_sms_list [UI_SMS_TXT_NEW_URGENT].cnt > 0 ) &&
      ( read_op->cmd != UI_SMS_READ_OLD ))
  {
    list_info.src_list = UI_SMS_TXT_NEW_URGENT;
    list_info.src_idx = 0;

    /* Indicate message to be read */
    read_op->msg_read = TRUE;
  }
  else if (( ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0 ) &&
      ( read_op->cmd != UI_SMS_READ_OLD ))
  {
    list_info.src_list = UI_SMS_TXT_NEW_NOT_URGENT;
    list_info.src_idx = 0;

    /* Indicate message to be read */
    read_op->msg_read = TRUE;
  }
  /* These read command imply only new messages so */
  /* don't read from old message list.             */
  else if (( ui_sms_list [UI_SMS_TXT_OLD].cnt > 0 ) &&
           ( read_op->cmd != UI_SMS_READ_NEW ) &&
           ( read_op->cmd != UI_SMS_READ_NOTIF ))
  {
    /* Read old - Continue reading from read bookmark */
    if ( read_op->cmd == UI_SMS_READ_OLD )
    {
      /* Use read index for next message */
      list_info.src_idx = read_op->read_idx;
    }
    else
    {
      /* Read any - Read from the top of the old */
      list_info.src_idx = 0;
    }

    /* Indicate message to be read */
    read_op->new = FALSE;
    read_op->msg_read = TRUE;
  }

  /* Stored text message found, read message from NV */
  if ( read_op->msg_read )
  {
    /* Read SMS message from NV */
    ui_sms_nv_item.address = 
      ui_sms_list [list_info.src_list].nv_idx[list_info.src_idx];

    nv_status = SKY_GetNV( NV_SMS_I, (nv_item_type *) &ui_sms_nv_item );

    /* Successfully retrieved SMS message */
    if ( nv_status == NV_DONE_S )
    {
      /* Determine if message info needs  */
      /* to be move from new to old list. */
      if (( list_info.src_list != UI_SMS_TXT_OLD ) &&
//          ( read_op->cmd != UI_SMS_READ_NOTIF ))
          ( read_op->cmd != UI_SMS_READ_NOTIF ) && (g_fSmsJustRead == TRUE)) // 읽어야만 이동
      {
        //cr5085
        g_DoOldTextMatch ();
        /* Move new (unread) text message to */
        /* old (read) text message list.     */
        /* FIFO presentation so start at the */
        /* top of the SMS list.  Move new    */
        /* to bottom of the old list.        */
        list_info.cmd = UI_SMS_MOVE;
        list_info.dest_list = UI_SMS_TXT_OLD;
        list_info.dest_idx = ui_sms_list [UI_SMS_TXT_OLD].cnt;

        nListType = UI_SMS_TXT_OLD;
        nListIdx = ui_sms_list [UI_SMS_TXT_OLD].cnt;

        /* Move message info from new list to old list */
        /* for text messages.  After move, message's   */
        /* list location will be in the destination    */
        /* list and index.                             */
        status = ui_sms_list_cmd ();

        /* Check status for processing errors */
        if ( status != UI_SMS_OP_OK )
        {
          /* Reset read return status */
          read_op->msg_read = FALSE;
        }
      }

      /* Unpack SMS message from NV SMS packed format  */
      /* and put message into list information.        */
      ui_sms_nv_data ( FALSE, &ui_sms_nv_item, list_info.sms_msg );

	  // Save Message after read new message... huni
      (void) ui_sms_power_down ();  
    }
    /* NV read error reported */
    else
    {
      /* Reset read return status */
      read_op->msg_read = FALSE;

      /* Return unreadable SMS message */
      /* to available SMS list.        */
      list_info.cmd = UI_SMS_DELETE;

      /* Delete SMS message from NV storage */
      status = ui_sms_list_cmd ();
    }
  }
}

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_CREATE_N_VOICE_MSG

DESCRIPTION
  Creates an SMS Voice Mail Notification message command based on a
  Number of Messages count.  The Teleservice type created will be
  IS-91 Extended Protocol Voice Mail, Point to Point, with Message
  Identifier = 0, No Bearer Reply.
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_create_n_voice_msg
(
  ui_cmd_type *cmd_ptr,      /* Command structure to store VMN info into */
  word count                 /* Number of Messages count */
)
{
  /* Dummy up an IS-91 Extended Protocol - Voice Mail message */
  cmd_ptr->sms.buf_ptr->data.tele_srv = SMSI_CMT_91_VOICE_MAIL;

  /* Point to Point Category */
  cmd_ptr->sms.buf_ptr->data.sms_msg_cat = SMS_PTP;

  /* Message ID type 0 */
  cmd_ptr->sms.buf_ptr->data.sms_msg_id.message_type = 1;    /* IS637 - 1 = Deliver */
  cmd_ptr->sms.buf_ptr->data.sms_msg_id.message_id = 0;

  /* Optional parameters - There is Bearer Data, and Number of Messages */
  cmd_ptr->sms.buf_ptr->data.sms_param_m = ( SMSI_BEARER_DATA | SMSI_NUM_OF_MSGS);

  /* Fill out the message data - the Number of Messages */
  /* Make sure it's in range */
  if ( count > 99 ) {
    count = 99;
  }
  cmd_ptr->sms.buf_ptr->data.msg_data.num_of_msgs[0] = ( count / 10 ) + 0x30 ;
  cmd_ptr->sms.buf_ptr->data.msg_data.num_of_msgs[1] = ( count % 10 ) + 0x30 ;
}


//////////////////////////////////////////////////////////////////////
//         From Uissms.c
//////////////////////////////////////////////////////////////////////
char *ui_sms_bcd_ascii_cvt
(
  char bcd_num,                 /* 2 BCD numbers to convert */
  char *txt                     /* ascii conversion for up to 2 numbers */
)
{
  byte i;
    /* Loop index counter */
  byte pos = 0;
    /* Starting position of BCD number */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Convert 2 BCD numbers to 2 ASCII numbers */
  for (i=0; i < 2; i++) 
  {
    /* Move BCD number to byte array */
#ifndef WIN32
    *txt = b_unpackb( (byte *) &bcd_num, pos, 4 );
#endif // ndef WIN32

    /* If BCD number = 10, then translate to */
    /* ASCII 0.                              */
    if (*txt == 10) 
    {
      *txt = 0x30;
    }
    /* Otherwise, add ASCII offset to translate. */
    else
    {
      *txt += 0x30;
    }
    /* Move to next BCD number and ASCII byte */
    pos += 4;
    txt++;
  }
  return( txt );
}

void ui_sms_utc_to_local (dword utc_time, clk_julian_type *julian)
{
  dword local_time;   /* System time */
  int ltm_off;     /* Leap time offset */
  /* DB item */
  db_items_value_type db_item;  /* SMS UTC time */

  local_time = utc_time;

  /* Check if the time is valid */
  if (local_time >= ONE_YEAR)
  {

    /* Get the time zone */
    db_get(DB_LTM_OFF, &db_item);

    ltm_off = (int)db_item.ltm_off;

    if (ltm_off & 0x20)
    {
      /* extend 6 bit signed to 16 bit signed */
      ltm_off |= 0xFFC0;
    }
    local_time += 60 * 30 * ((dword) ltm_off);

  } /* end if (systime >= UI_ONE_YEAR) */
  
  /* Get the Julian time */
  clk_secs_to_julian(local_time, julian);

} /* end ui_sms_utc_to_local () */

char *ui_sms_ascii_cvt
(
  word num,                     /* number to convert */
  char *txt                     /* ascii conversion for up to 2 numbers */
)
{
  *txt++ = (byte) (num/10 + '0');
  *txt++ = (byte) (num%10 + '0');
  return( txt );
}


TELESEVICE_KIND_TYPE g_TeleId()
{

    TELESEVICE_KIND_TYPE    s_TeleIdType = COMMON_SMS;

    if (ui_sms_msg.tele_srv == SMSI_WAP_MAIL)
        s_TeleIdType = WAP_MAIL;
    else if((ui_sms_msg.tele_srv == SMSI_WAP_PUSH)
            ||(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH))
        s_TeleIdType = WAP_PUSH;
#ifdef FEATURE_SKVM
	else if(ui_sms_msg.tele_srv == SMSI_SKVM_LOAD)
		s_TeleIdType = SKVM_LOAD;
	else if(ui_sms_msg.tele_srv == SMSI_SKVM_DATA)
		s_TeleIdType = SKVM_DATA;
#endif
/* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
    else if (ui_sms_msg.tele_srv == SMSI_GVM) 
        s_TeleIdType = GVM_DOWNLOAD;
#endif
/* <-- */
    else
        s_TeleIdType = COMMON_SMS;

    return ( s_TeleIdType );
}
void ReadNewMessage()
{    /* Setup for new message read from NV */
    sms_read.cmd = UI_SMS_READ_NOTIF;
    sms_read.read_list = ui_sms_read_info.new_read_list;
    sms_read.read_idx  = ui_sms_read_info.new_read_idx;

    /* Determine which category to read - text or voice */
    if (( sms_read.read_list == UI_SMS_TXT_NEW_URGENT ) ||
        ( sms_read.read_list == UI_SMS_TXT_NEW_NOT_URGENT ))
    {
        /* Read new text message */
        nListType = sms_read.read_list;               // List Type To Read
        nListIdx = ui_sms_list[nListType].cnt - 1;    // List Index

        ui_sms_read_text (&sms_read);
    }
    else
    {
        /* Read new voice mail */
        ui_sms_read_voice (&sms_read);
    }
}
ui_sms_title_type g_MessageNotif
(
    BYTE    *pCallback,
    BYTE    *pTimeStamp
)
{

    ui_sms_title_type    sKind = NOT_URGENT_TEXT_SMS; 
//    BYTE      szTmpStr[32+1];


  clk_julian_type julian_ptr;   
    /* Julian time used for SMS UTC time implementation */
//  dword utc_time_sec;            
    /* Time in seconds used for SMS UTC time implementation */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Determine if new message is voice by checking */
  /* SMS teleservice and if number of message      */
  /* parameter is present.                         */
  if ((( ui_sms_msg.tele_srv == SMSI_VMN_95 ) ||
     ( ui_sms_msg.tele_srv == SMSI_CMT_91_VOICE_MAIL ) ||
     ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
     (( ui_sms_msg.sms_param_m & SMSI_NUM_OF_MSGS) != 0))
  {

    /* Build voice notification display     */
    /* based on number of new message value */
    if (!(( ui_sms_msg.msg_data.num_of_msgs[0] == 0x30) &&
          ( ui_sms_msg.msg_data.num_of_msgs[1] == 0x30))) 
    {
      /* Display asterisks in header if  */
      /* message is urgent or emergency. */
      if ((( ui_sms_msg.sms_param_m & SMSI_PRIORITY) != 0) &&
           ( ui_sms_msg.msg_data.priority >= UI_SMS_URGENT_IND))
      {
        /* Setup urgent voice mail notification message */
          sKind = URGENT_VOICE_SMS;
      }
      /* Voice mail is not urgent */
      else
      {
        /* Setup voice mail notification message */
          sKind = NOT_URGENT_VOICE_SMS;
      }
    }
  }
  /* Check if message or page notification */
  else 
  {
    /* If message priority is urgent or emergency, */
    /* blink icon and display asterisks around     */
    /* 'MESSAGE!' header.                          */
    if ((( ui_sms_msg.sms_param_m & SMSI_PRIORITY) != 0) &&
         ( ui_sms_msg.msg_data.priority >= UI_SMS_URGENT_IND))
    {
      /* Display message identifier if message */
      if ((( ui_sms_msg.tele_srv == SMSI_CMT_95 ) ||
         ( ui_sms_msg.tele_srv == SMSI_CMT_91_SHORT_MSG ) ||
         ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
         (( ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0))
      {
        /* Setup message notification */
            sKind = URGENT_TEXT_SMS;
      }
      /* Page notification - urgent */
      else
      {
        /* Setup page notification */
        sKind = URGENT_TEXT_SMS;
      }
    }
    /* Not an urgent text message */
    else
    {
      /* Display message identifier if message */
      if ((( ui_sms_msg.tele_srv == SMSI_CMT_95 ) ||
         ( ui_sms_msg.tele_srv == SMSI_CMT_91_SHORT_MSG ) ||
         ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
         (( ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0))
      {
        /* Setup message notification */
        sKind = NOT_URGENT_TEXT_SMS;
      }
      /* Page notification */
      else
      {
        /* Setup page notification */
        sKind = NOT_URGENT_TEXT_SMS;
      }
    }
  }

  /* Display call back number if page or message */
  if (( ui_sms_msg.sms_param_m & SMSI_CALL_BACK) != 0)
  {
      (void) memcpy ( pCallback, 
	  			    (const void *) ui_sms_msg.msg_data.call_back.chari, 
                    ui_sms_msg.msg_data.call_back.num_fields );
#if 0
    if( ( TmpBuf = FindNumber((BYTE*)szTmpStr) ) != NULL )
    {
        (void) memcpy ( pCallback, 
	  			        (const void *) TmpBuf, 
                        STRLEN(TmpBuf) );
    }
    else
    {
        if ( ui_sms_msg.msg_data.call_back.num_fields > SMS_SCREEN_SIZE )
        {
            /* Setup auto-hyphenate edit buffer for call back number */
            (void) memcpy ( pCallback, 
	  			            (const void *) ui_sms_msg.msg_data.call_back.chari, 
                            SMS_SCREEN_SIZE );
        }
        else
        {
            /* Setup auto-hyphenate edit buffer for call back number */
            (void) memcpy ( pCallback, 
	  			            (const void *) ui_sms_msg.msg_data.call_back.chari, 
                            ui_sms_msg.msg_data.call_back.num_fields );
        }
    }
#endif
  }

  /* Display time stamp if given and portion of message. */
  if (( ui_sms_msg.sms_param_m & SMSI_MC_TIME_STAMP) != 0)
  {
    (void) ui_sms_bcd_to_julian ( ui_sms_msg.msg_data.mc_time , &julian_ptr); /* SMS UTC time */
 
//    utc_time_sec = clk_julian_to_secs(&julian_ptr);                           /* SMS UTC time */
//    (void) ui_sms_utc_to_local (utc_time_sec,&julian_ptr);                    /* SMS UTC time */
    
    /* cr4630 */
    /* Format month */     /* SMS UTC time */
    /* Translate Julian month to ascii format */
    (void) ui_sms_ascii_cvt ( (word) julian_ptr.month,
             ( char * ) &(pTimeStamp[0]) );

    /* Eliminate leading zero in month */
    if ( pTimeStamp[0] == '0')
    {
      pTimeStamp[0] = SMS_SPACE;
    }

    /* Insert '/' separator in time stamp */
	(void) memcpy ( &pTimeStamp[2], "월", 2);

    /* Format day */
    /* Translate Julian day to ascii format */
	(void) ui_sms_ascii_cvt ( (word) julian_ptr.day,
					 ( char * ) &(pTimeStamp[4]) );

	/* Translate 1 digit day */
	if ( pTimeStamp[4] == '0')
	{
		/* Move day over 1 space to the left */
		/* and blank out 2nd number.         */
		pTimeStamp[4] = SMS_SPACE;

	}
	(void) memcpy ( &pTimeStamp[6], "일", 2);


    /* Hours are given as 24 hour clock time */
    /* Translate Julian hours to ascii format */
	(void) ui_sms_ascii_cvt ( (word) julian_ptr.hour,
									( char * ) &(pTimeStamp[8]) );

	/* Translate to 12 hour clock time for pm hours. */
	if ((( pTimeStamp[8] == '1' ) &&
			 ( pTimeStamp[9] >= '2' )) ||
			(( pTimeStamp[8] == '2' ) &&
			 ( pTimeStamp[9] < '4' )))
	{
		/* Translate hours to 12 hour clock time */
		/* if not noon hour.                     */
		if (!(( pTimeStamp[8] == '1' ) &&
					( pTimeStamp[9] == '2' )))
		{
			if (( pTimeStamp[8] == '1' ) ||
					( pTimeStamp[9] >= '2' ))
			{
				/* Adjust second ASCII value by 2 for */
				/* all values except '0' and '1'.     */
				pTimeStamp[9] -= 2;
			}
			/* For hours 20 or 21, add 8 and subtract 10 */
			else
			{
				/* Adjust first ASCII value by 1. */
				pTimeStamp[8]--;

				/* Adjust second ASCII value by 8 for */
				/* '0' and '1'.                       */
				pTimeStamp[9] += 8;
			}

			/* Adjust first ASCII value by 1. */
			pTimeStamp[8]--;
		}

		/* Insert 'pm' in time stamp */
		pTimeStamp[13] = 'P';
	}
	else
	{
#if 0 
		/* Check for midnight hour and translate to 12 */
		if (( pTimeStamp[8] == '0' ) &&
				( pTimeStamp[9] == '0' ))
		{
			pTimeStamp[8]   = '1';
			pTimeStamp[9] = '2';
		}
#endif
		/* Insert 'am' in time stamp */
		pTimeStamp[13] = 'A';
	}

	pTimeStamp[14] = 'M';

	/* Eliminate leading zero in hours */
	if ( pTimeStamp[8] == '0' )
	{
		pTimeStamp[8] = SMS_SPACE;
	}

	/* Insert ':' separator in time stamp */
	pTimeStamp[10] = ':';

	/* Translate BCD minutes to ascii format */
	/* and insert into display message.      */
	(void) ui_sms_ascii_cvt ( (word) julian_ptr.minute,
							  ( char * )&(pTimeStamp[11]) );
  }

    return ( sKind );
}

VOID    g_DeleteTextMessage( void )
{
    ui_sms_op_status_type status;

    /* Setup delete operation from SMS list. */
    list_info.cmd = UI_SMS_DELETE;
    list_info.src_list = nListType;
    list_info.src_idx = nListIdx;

    /* Delete SMS message from NV storage */
    status = ui_sms_list_cmd ();

    if ( status == UI_SMS_OP_OK )
    {
        /* Update SMS control information after  */
        /* message has been deleted.  Write      */
        /* control info into NV using power-down */
        /* function.                             */
        (void) ui_sms_power_down ();
    }

    /* Update the icons */
    ui_sms_update_icons ();

}

INT8    gReadTextMessage ( void)
{
    INT8    sTextSize = 0;

  /* Setup for text message read */
  /* Check new then old list     */

  sms_read.cmd = UI_SMS_READ_ANY;  

  g_fSmsJustRead = FALSE; 

  /* Determine if there is text messages to read */
  ui_sms_read_text (&sms_read);

  /* If no messages remaining on SMS list of new urgent */
  /* messages then turn off blinking on icon.           */
  ui_sms_update_icons ();

  if( ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt >0 ||
      ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt >0 ||
      ui_sms_list[UI_SMS_TXT_OLD].cnt >0 )
  {
    sTextSize = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt
              + ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt
              + ui_sms_list[UI_SMS_TXT_OLD].cnt ;
  }
  else
  {
    sTextSize = 0;
  }

    return ( sTextSize );
}

void gMakeMsgList ( void )
{
  INT8      i,  sLineNumber;
  static BYTE pViewBuf[MAX_RECEIVE_MESSAGE][UI_SMS_WIDE];
  BYTE      nv_addr;

  sLineNumber = 0;
  for(i = 0; i < MAX_RECEIVE_MESSAGE; i++)
  {
      memset((void *)&pViewBuf[i][0], NULL, UI_SMS_WIDE);
  }

  for (i=ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt; i>0; i--)
  {
    nv_addr = ui_sms_list [UI_SMS_TXT_NEW_URGENT].nv_idx[i-1];

    if( ui_sms_text_list[nv_addr][0] == 2 )
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_PROT_MESSAGE, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 3 )
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_PRI_PROTECT, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 0 ) 
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_MESSAGE, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 1 ) 
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_PRI_MESSAGE, 2);

    (void) memcpy ( (void *)&pViewBuf[sLineNumber][2],
                        (const void *)&ui_sms_text_list[nv_addr][1], UI_SMS_WIDE - 4 );

    paMsgListBuf[sLineNumber] = pViewBuf[sLineNumber];

    sLineNumber++;
  }

  for (i=ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt; i>0; i--)
  {
    nv_addr = ui_sms_list [UI_SMS_TXT_NEW_NOT_URGENT].nv_idx[i-1];

    if( ui_sms_text_list[nv_addr][0] == 2 )
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_PROT_MESSAGE, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 3 )
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_PRI_PROTECT, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 0 ) 
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_MESSAGE, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 1 ) 
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_UNREAD_PRI_MESSAGE, 2);

    (void) memcpy ( (void *)&pViewBuf[sLineNumber][2],
                        (const void *)&ui_sms_text_list[nv_addr][1], UI_SMS_WIDE - 4 );

    paMsgListBuf[sLineNumber] = pViewBuf[sLineNumber];

    sLineNumber++;
  }

  for (i=ui_sms_list[UI_SMS_TXT_OLD].cnt; i>0; i--)
  {
    nv_addr = ui_sms_list [UI_SMS_TXT_OLD].nv_idx[i-1];

    if( ui_sms_text_list[nv_addr][0] == 2 )
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_READ_PROT_MESSAGE, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 3 )
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_READ_PRI_PROTECT, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 0 ) 
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_READ_MESSAGE, 2);
    else if ( ui_sms_text_list[nv_addr][0] == 1 ) 
	    (void) memcpy( (void *)&pViewBuf[sLineNumber][0], SF_READ_PRI_MESSAGE, 2);

    (void) memcpy ( (void *)&pViewBuf[sLineNumber][2],
                        (const void *)&ui_sms_text_list[nv_addr][1], UI_SMS_WIDE - 4 );

    paMsgListBuf[sLineNumber] = pViewBuf[sLineNumber];

    sLineNumber++;
  }
}

void gMakeProtect ( void )
{
    BYTE      nv_addr;

    nv_addr = ui_sms_list [nListType].nv_idx [nListIdx];
    if( ui_sms_text_list[nv_addr][0] == 0 )     //0x02 protect
	    ui_sms_text_list[nv_addr][0] = 2;
    else if( ui_sms_text_list[nv_addr][0] == 1 )
	    ui_sms_text_list[nv_addr][0] = 3;
    else if( ui_sms_text_list[nv_addr][0] == 2 )
	    ui_sms_text_list[nv_addr][0] = 0;
    else if( ui_sms_text_list[nv_addr][0] == 3 )
	    ui_sms_text_list[nv_addr][0] = 1;

    ui_sms_nv_item.address = nv_addr;
    SKY_GetNV (NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item);
    ui_sms_nv_item.protect = ui_sms_text_list[nv_addr][0];
    SKY_PutNV (NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item );
}

BOOL IsMsgProtect( void )
{
    BYTE      nv_addr;

    nv_addr = ui_sms_list [nListType].nv_idx [nListIdx];
    if(ui_sms_text_list[nv_addr][0] >= 0x02)
        return TRUE;
    else
        return FALSE;
}
            
UINT8 FormatWapMailMessage()
{
    UINT8 nLine, i;
  
    /* Initialize line number for display write */
    nLine = 0;
    memset( pFormatBuf, NULL, MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE+1 );

    if((ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0)
    {
        for(i = 0; i < ui_sms_msg.msg_data.user_data.num_fields; i++)
        {
            if(ui_sms_msg.msg_data.user_data.chari[i] == SEPERATE)  break;
        }
        i++;
        if(i >= ui_sms_msg.msg_data.user_data.num_fields)
        {
            STRCPY(pFormatBuf,(BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
        }
        else 
        {
            nLine = ui_sms_auto_format (
                        TRUE,
                        (BYTE *)&ui_sms_msg.msg_data.user_data.chari[i], 
                        ui_sms_msg.msg_data.user_data.num_fields - i,
                        pFormatBuf,
                        ((MAX_MSG_VIEW_LINE-5)*SMS_SCREEN_SIZE));      // 메일 확인 2라인을 남겨둠       
        }
    }
    else
    {
        STRCPY(pFormatBuf,(BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
    } 
    
    paMsgViewBuf = pFormatBuf;
    return nLine;
}

#ifdef FEATURE_SKTT_PM
UINT8 FormatWapPushMessage(BOOL* pfIsPhotoMail)
{
    UINT8 nLine, i; // formatted line number
    CHAR        cSeperate;
    UINT16      nMsgSize, nUrlSize;
    BYTE*       pStartMsg;
    
    nMsgSize = 0;
    pStartMsg = NULL;
    i = 0;

    if(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH)
    {
        cSeperate = SEPERATE_SHINSEGI;
    }
    else
    {
        cSeperate = SEPERATE;
    }                
    
#ifdef FEATURE_WAP
 // Initialize line number for display write 
    nLine = 0;
    memset( pFormatBuf, NULL, MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE+1 );

    if((ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0)
    {
        memset(g_pszPushUrl, NULL, URL_MAX);
        for(i = 0; i < ui_sms_msg.msg_data.user_data.num_fields; i++)
        {            
            if(ui_sms_msg.msg_data.user_data.chari[i] != cSeperate)
            {
                if(ui_sms_msg.tele_srv == SMSI_WAP_PUSH)
                {
                    g_pszPushUrl[i] = ui_sms_msg.msg_data.user_data.chari[i];
                }
            }
            else
                break;
        }

        i++;

        if(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH)
        {
            if(i == 1)
            {
                STRCPY(pFormatBuf, (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
                nLine = 1;
            }
            else
            {
                nUrlSize = ui_sms_msg.msg_data.user_data.num_fields - i;
                if(nUrlSize > URL_MAX-1)
                {
                    nUrlSize = URL_MAX-1;
                }
                memcpy(g_pszPushUrl, (BYTE *)&ui_sms_msg.msg_data.user_data.chari[i], nUrlSize); 
                
                pStartMsg = (BYTE *)ui_sms_msg.msg_data.user_data.chari;
                nMsgSize = i - 1;
            }
        }
        else
        {
            if(i >= ui_sms_msg.msg_data.user_data.num_fields) 
            {
                STRCPY(pFormatBuf, (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
				nLine = 1;                   
            }
            else
            {
                pStartMsg = (BYTE *)&ui_sms_msg.msg_data.user_data.chari[i];
                nMsgSize = ui_sms_msg.msg_data.user_data.num_fields - i;
            }            
        }

        if(strstr((char *)g_pszPushUrl,(char *)PHOTOMAIL_URL)!= NULL)
		{
            *pfIsPhotoMail = TRUE;   
        }
        else
        {
            *pfIsPhotoMail = FALSE;
        }

        if(pStartMsg != NULL)
        {
            nLine = ui_sms_auto_format(
                                    TRUE, 
                                    (BYTE *) pStartMsg,
                                    nMsgSize,
                                    pFormatBuf, 
                                    ((MAX_MSG_VIEW_LINE-4)*SMS_SCREEN_SIZE));
        }
    }
    else
    {
        STRCPY(pFormatBuf, (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
        nLine++;
    }
    
    paMsgViewBuf = pFormatBuf;

#endif  //FEATURE_WAP ..
    return nLine;
}

#else //FEATURE_SKTT_PM
UINT8 FormatWapPushMessage()
{
    UINT8 nLine, i; // formatted line number
 
#ifdef FEATURE_WAP
 /* Initialize line number for display write */
    nLine = 0;
    memset( pFormatBuf, NULL, MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE+1 );

    if((ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0)
    {
        memset(g_pszPushUrl, NULL, URL_MAX);
        for(i = 0; i < ui_sms_msg.msg_data.user_data.num_fields; i++)
        {
            if(ui_sms_msg.msg_data.user_data.chari[i] != SEPERATE)
                g_pszPushUrl[i] = ui_sms_msg.msg_data.user_data.chari[i];
            else
                break;
        }
        i++;

        nLine = ui_sms_auto_format(
                    TRUE, 
                    (BYTE *) &ui_sms_msg.msg_data.user_data.chari[i],
                    ui_sms_msg.msg_data.user_data.num_fields - i,
                    pFormatBuf, 
                    ((MAX_MSG_VIEW_LINE-4)*SMS_SCREEN_SIZE));
    }
    else
    {
        STRCPY(pFormatBuf, (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
        nLine++;
    }
    
    paMsgViewBuf = pFormatBuf;
#endif
    return nLine;
}
#endif

#ifdef FEATURE_SKVM
UINT8 FormatSkvmLoadMessage(){

    UINT8 nLine, i;

    nLine = 0;

    
	memset( pFormatBuf, NULL, MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE+1 );
	
	memset(g_pszmsdURL, NULL, MSDURL_MAX);

    if((ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0)
    {

		for(i=0; i < ui_sms_msg.msg_data.user_data.num_fields; i++) 
		{
			if (ui_sms_msg.msg_data.user_data.chari[i] != MSD_SEPERATOR)
			    g_pszmsdURL[i] = ui_sms_msg.msg_data.user_data.chari[i];
			else break;
		}
		i++;
		nLine = ui_sms_auto_format(
					TRUE, 
					(BYTE *) &ui_sms_msg.msg_data.user_data.chari[i],
					ui_sms_msg.msg_data.user_data.num_fields - i,
					pFormatBuf, 
					((MAX_MSG_VIEW_LINE-4)*SMS_SCREEN_SIZE));
	
    }
    else
    {
        STRCPY(pFormatBuf, (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
        nLine++;
    }

    paMsgViewBuf = pFormatBuf;
	return nLine;
	
}
#endif

/* undersea 2001-07-09 --> @why, what */
#ifdef FEATURE_GVM
UINT8 FormatGVMMessage()
{
    UINT8 nLine, i;    
    
    /* Initialize line number for display write */
    nLine = 0;
    memset( pFormatBuf, NULL, MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE+1 );

    if((ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0)
    {

		for(i=0; i < ui_sms_msg.msg_data.user_data.num_fields; i++) 
		{
			if (ui_sms_msg.msg_data.user_data.chari[i] != SEPERATE)
			    pFormatBuf[i] = ui_sms_msg.msg_data.user_data.chari[i];
			else break;
		}
        nLine++;		
    }
    else
    {
        STRCPY(pFormatBuf, (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
        nLine++;
    }    
    sParsingGVMMessage(pFormatBuf, TRUE);    
    
    return nLine;
}

VOID sParsingGVMMessage(BYTE* pMsg, BOOL fIsNotify) {

    STATIC  BYTE    *s_szUserID;
    STATIC  UINT16  s_nScriptCPID;
    STATIC  UINT16  s_nScriptID;
    STATIC  BYTE    *s_szDlsIP;
    STATIC  UINT16  s_nDlsPort;
    STATIC  BYTE    *s_szSvrIP;
    STATIC  UINT16  s_nSvrPort;
    STATIC  BYTE    *s_szRetURL;
    STATIC  BOOL    s_fFlag=0;

    STATIC  BYTE    *pMsgBuf;
    STATIC  BYTE    *pBuf;


    pMsgBuf = pMsg;
    pBuf = STRTOK(pMsgBuf, (CONST BYTE*)",");
    if (STRCMP(pBuf, (CONST BYTE*)"SWAP") == 0) {   // 맞는 SMS 이다
        pMsgBuf = pMsgBuf + STRLEN(pBuf) + 1;

        paMsgViewBuf = STRTOK(pMsgBuf, (CONST BYTE*)","); // 컨텐츠 설명
        pMsgBuf = pMsgBuf + STRLEN(paMsgViewBuf) + 1;

        pBuf = STRTOK(pMsgBuf, (CONST BYTE*)","); // FLAG
        s_fFlag = atoi((const char*)pBuf);
        pMsgBuf = pMsgBuf + STRLEN(pBuf) + 1;

        if (s_fFlag || (!s_fFlag && !fIsNotify)) {  // Flag가 1이여만 direct download가 가능함.
            s_szUserID = STRTOK(pMsgBuf, (CONST BYTE*)","); // UserID
            pMsgBuf = pMsgBuf + STRLEN(s_szUserID) + 1;

            pBuf = STRTOK(pMsgBuf, (CONST BYTE*)","); // CP ID
            s_nScriptCPID = atoi((const char*)pBuf);
            pMsgBuf = pMsgBuf + STRLEN(pBuf) + 1;

            pBuf = STRTOK(pMsgBuf, (CONST BYTE*)","); //  ID
            s_nScriptID = atoi((const char*)pBuf);
            pMsgBuf = pMsgBuf + STRLEN(pBuf) + 1;

            s_szDlsIP = STRTOK(pMsgBuf, (CONST BYTE*)","); // Download server ip
            pMsgBuf = pMsgBuf + STRLEN(s_szDlsIP) + 1;

            pBuf = STRTOK(pMsgBuf, (CONST BYTE*)","); //  download server port
            s_nDlsPort = atoi((const char*)pBuf);
            pMsgBuf = pMsgBuf + STRLEN(pBuf) + 1;

            s_szSvrIP = STRTOK(pMsgBuf, (CONST BYTE*)","); // game server ip
            pMsgBuf = pMsgBuf + STRLEN(s_szSvrIP) + 1;

            pBuf = STRTOK(pMsgBuf,(CONST BYTE*)","); //  game server port
            s_nSvrPort = atoi((const char*)pBuf);
            pMsgBuf = pMsgBuf + STRLEN(pBuf) + 1;

            s_szRetURL = STRTOK(pMsgBuf, (CONST BYTE*)","); // return url
                       
            g_fDownloadRequestFromSMS = TRUE;                
            GoToGVM((char*)s_szUserID, s_nScriptCPID, s_nScriptID, (char*)s_szDlsIP, s_nDlsPort, 
                    (char*)s_szSvrIP, s_nSvrPort, (char*)s_szRetURL, NULL);
        }
    }
}
#endif
/* <-- */


ui_sms_kind_type g_FormatTextMessage
( 
    BYTE    *pTimeStamp,
    BYTE    *pCallback,
    INT8    *nLine
)
{
    INT8      sLineLength, i;
    UINT16    nPos;
    clk_julian_type julian_ptr; 
    CHAR        cSeperate;
    UINT16      nMsgSize, nUrlSize;
    BYTE*       pStartMsg;     
    
#ifdef FEATURE_SKVM
	BYTE	j,k;
	simple_line	DspLine[3];
#endif
    
    ui_sms_kind_type    sKind = NORMAL_SMS; 

    // Initialize Buffer
    nMsgSize = 0;
    pStartMsg = NULL;

    nPos = 0;
    sLineLength = 0;
    memset( pFormatBuf, NULL, MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE+1 );

    sms_read.cmd = UI_SMS_READ_ONE;
    sms_read.read_list = nListType;   // ui_sms_disp_info.sms_list;
    sms_read.read_idx = nListIdx;     // ui_sms_disp_info.sms_idx;

    if ( ( nListType == UI_SMS_TXT_NEW_URGENT ) || ( nListType == UI_SMS_TXT_NEW_NOT_URGENT ))
        g_fSmsJustRead = TRUE; 
    else
        g_fSmsJustRead = FALSE; 

    ui_sms_read_text (&sms_read);

    /* Update the icon */
    ui_sms_update_icons ();

    if ((
       ( ui_sms_msg.tele_srv == SMSI_CMT_95 ) ||
       ( ui_sms_msg.tele_srv == SMSI_CMT_91_SHORT_MSG ) ||
       ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
      (( ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0))
    {
        /* Display urgent priority if specified */
        if ((( ui_sms_msg.sms_param_m & SMSI_PRIORITY) != 0) &&
           ( ui_sms_msg.msg_data.priority >= UI_SMS_URGENT_IND))
        {            
            if(ui_sms_msg.msg_data.priority == URGENT_SMS)
            {
                sKind = URGENT_SMS;
            }
            else if(ui_sms_msg.msg_data.priority == EMERGENCY_SMS)
            {
                sKind = EMERGENCY_SMS;
            }
        }
        else
        {
          sKind = NORMAL_SMS;
        }
    }
    /* Indicate message type is a page */
    else if ((( ui_sms_msg.tele_srv == SMSI_CPT_95 ) ||
       ( ui_sms_msg.tele_srv == SMSI_CMT_91_CLI ) ||
       ( ui_sms_msg.tele_srv == SMSI_AWI_95 )) &&
      (( ui_sms_msg.sms_param_m & SMSI_CALL_BACK ) != 0) &&
      (( ui_sms_msg.sms_param_m & SMSI_NUM_OF_MSGS ) == 0))
    {
        /* Display urgent priority if specified */
        if ((( ui_sms_msg.sms_param_m & SMSI_PRIORITY) != 0) &&
           ( ui_sms_msg.msg_data.priority >= UI_SMS_URGENT_IND))
        {
//          (void) memcpy ( &pFormatBuf[nPos], sUrgentTitle, SMS_SCREEN_SIZE);
            if(ui_sms_msg.msg_data.priority == URGENT_SMS)
            {
                sKind = URGENT_SMS;
            }
            else if(ui_sms_msg.msg_data.priority == EMERGENCY_SMS)
            {
                sKind = EMERGENCY_SMS;
            }
        }        
        else
        {
//          (void) memcpy ( &pFormatBuf[nPos], sTextTitle, SMS_SCREEN_SIZE);
          sKind = NORMAL_SMS;
        }

    }
    else if ( ui_sms_msg.tele_srv == SMSI_WAP_MAIL )
    {
        sKind = WAP_MAIL_SMS;
    }
    else if ( (ui_sms_msg.tele_srv == SMSI_WAP_PUSH)
                ||(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH) )
    {
        sKind = WAP_PUSH_SMS;
    }
#ifdef FEATURE_SKVM
    else if ( ui_sms_msg.tele_srv == SMSI_SKVM_LOAD )
    {
        sKind = SKVM_LOAD_SMS;
    }              
    else if ( ui_sms_msg.tele_srv == SMSI_SKVM_DATA )
    {
        sKind = SKVM_DATA_SMS;
    } 
#endif //FEATURE_SKVM      
    /* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
    else if ( ui_sms_msg.tele_srv == SMSI_GVM) 
    {
        sKind = GVM_DL_SMS;
    }
#endif
/* <-- */    

    /* Display time stamp if given and portion of message. */
    if (( ui_sms_msg.sms_param_m & SMSI_MC_TIME_STAMP) != 0)
    {
        (void) ui_sms_bcd_to_julian ( ui_sms_msg.msg_data.mc_time , &julian_ptr); /* SMS UTC time */

        /* Format month */     /* SMS UTC time */
        /* Translate Julian month to ascii format */
        (void) ui_sms_ascii_cvt ( (word) julian_ptr.month,
                 ( char * ) &(pTimeStamp[0]) );

        /* Eliminate leading zero in month */
        if ( pTimeStamp[0] == '0')
        {
          pTimeStamp[0] = SMS_SPACE;
        }

        /* Insert '/' separator in time stamp */
        (void) memcpy ( &pTimeStamp[2], "월", 2);

        /* Format day */
        /* Translate Julian day to ascii format */
        (void) ui_sms_ascii_cvt ( (word) julian_ptr.day,
				         ( char * ) &(pTimeStamp[4]) );

        /* Translate 1 digit day */
        if ( pTimeStamp[4] == '0')
        {
	        /* Move day over 1 space to the left */
	        /* and blank out 2nd number.         */
	        pTimeStamp[4] = SMS_SPACE;

        }
        (void) memcpy ( &pTimeStamp[6], "일", 2);


        /* Hours are given as 24 hour clock time */
        /* Translate Julian hours to ascii format */
        (void) ui_sms_ascii_cvt ( (word) julian_ptr.hour,
								        ( char * ) &(pTimeStamp[8]) );

        /* Translate to 12 hour clock time for pm hours. */
        if ((( pTimeStamp[8] == '1' ) &&
		         ( pTimeStamp[9] >= '2' )) ||
		        (( pTimeStamp[8] == '2' ) &&
		         ( pTimeStamp[9] < '4' )))
        {
	        /* Translate hours to 12 hour clock time */
	        /* if not noon hour.                     */
	        if (!(( pTimeStamp[8] == '1' ) &&
				        ( pTimeStamp[9] == '2' )))
	        {
		        if (( pTimeStamp[8] == '1' ) ||
				        ( pTimeStamp[9] >= '2' ))
		        {
			        /* Adjust second ASCII value by 2 for */
			        /* all values except '0' and '1'.     */
			        pTimeStamp[9] -= 2;
		        }
		        /* For hours 20 or 21, add 8 and subtract 10 */
		        else
		        {
			        /* Adjust first ASCII value by 1. */
			        pTimeStamp[8]--;

			        /* Adjust second ASCII value by 8 for */
			        /* '0' and '1'.                       */
			        pTimeStamp[9] += 8;
		        }

		        /* Adjust first ASCII value by 1. */
		        pTimeStamp[8]--;
	        }

	        /* Insert 'pm' in time stamp */
	        pTimeStamp[13] = 'P';
        }
        else
        {
#if 0 
	        /* Check for midnight hour and translate to 12 */
	        if (( pTimeStamp[8] == '0' ) &&
			        ( pTimeStamp[9] == '0' ))
	        {
		        pTimeStamp[8]   = '1';
		        pTimeStamp[9] = '2';
	        }
#endif
	        /* Insert 'am' in time stamp */
	        pTimeStamp[13] = 'A';
        }

        pTimeStamp[14] = 'M';

        /* Eliminate leading zero in hours */
        if ( pTimeStamp[8] == '0' )
        {
	        pTimeStamp[8] = SMS_SPACE;
        }   

        /* Insert ':' separator in time stamp */
        pTimeStamp[10] = ':';

        /* Translate BCD minutes to ascii format */
        /* and insert into display message.      */
        (void) ui_sms_ascii_cvt ( (word) julian_ptr.minute,
						          ( char * )&(pTimeStamp[11]) );
    }

    /* Display user data and length if available for */
    /* message or page.                              */
    if ((ui_sms_msg.sms_param_m & SMSI_USER_DATA) != 0) 
    {
	    /* Format user data and update display line */
	    /* length after formatting.                 */
#ifdef FEATURE_WAP
        if( (ui_sms_msg.tele_srv == SMSI_WAP_PUSH)
            ||(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH)
            ||(ui_sms_msg.tele_srv == SMSI_WAP_MAIL) )
        {
            if(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH)
            {
                cSeperate = SEPERATE_SHINSEGI;
            }
            else
            {
                cSeperate = SEPERATE;
            }

            if((memchr((byte *)ui_sms_msg.msg_data.user_data.chari,cSeperate,ui_sms_msg.msg_data.user_data.num_fields))==NULL)
            {
			    i =0;
                pStartMsg = (BYTE *)ui_sms_msg.msg_data.user_data.chari;
                nMsgSize = ui_sms_msg.msg_data.user_data.num_fields;
			    goto format;                      
		    }

            if((ui_sms_msg.tele_srv == SMSI_WAP_PUSH) || (ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH))
            {
                memset(g_pszPushUrl, NULL, URL_MAX);
            }
            
            for(i=0; i < ui_sms_msg.msg_data.user_data.num_fields; i++) 
            {
                if(ui_sms_msg.msg_data.user_data.chari[i] != cSeperate)
                {
                    if (ui_sms_msg.tele_srv == SMSI_WAP_PUSH) 
                    {                      
                        g_pszPushUrl[i] = ui_sms_msg.msg_data.user_data.chari[i];
                    }
			    }
                else 
                    break;
            }

            i++;

            if(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH)
            {
                if(i == 1)
                {
                    STRCPY(&pFormatBuf[nPos], (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
				    sLineLength = 1;                     
                }
                else
                {
                    nUrlSize = ui_sms_msg.msg_data.user_data.num_fields - i;
                    if(nUrlSize > URL_MAX-1)
                    {
                        nUrlSize = URL_MAX-1;
                    }
                    memcpy(g_pszPushUrl, (BYTE *)&ui_sms_msg.msg_data.user_data.chari[i], nUrlSize); 
                    
                    pStartMsg = (BYTE *)ui_sms_msg.msg_data.user_data.chari;
                    nMsgSize = i - 1;
                }
            }
            else
            {
                if(i >= ui_sms_msg.msg_data.user_data.num_fields) 
                {
                    STRCPY(&pFormatBuf[nPos], (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
				    sLineLength = 1;                   
                }
                else
                {
                    pStartMsg = (BYTE *)&ui_sms_msg.msg_data.user_data.chari[i];
                    nMsgSize = ui_sms_msg.msg_data.user_data.num_fields - i;
                }                    
            }

#ifdef FEATURE_SKTT_PM
            if(((ui_sms_msg.tele_srv == SMSI_WAP_PUSH)
                ||(ui_sms_msg.tele_srv == SMSI_SHINSEGI_WAP_PUSH))                        
                && (strstr((char *)g_pszPushUrl,(char *)PHOTOMAIL_URL)!= NULL))
            {                
                sKind = WAP_PUSH_PHOTOMAIL;           
            }
#endif //FEATURE_SKTT_PM         

format:
            if(pStartMsg != NULL)
            {
                sLineLength = ui_sms_auto_format (
                            TRUE,
                            (byte *) pStartMsg, 
                            nMsgSize,
                            (byte *) &pFormatBuf[nPos],
                            ((MAX_MSG_VIEW_LINE-5)*SMS_SCREEN_SIZE));      // 메일 확인 1라인을 남겨둠
            }
        }
        else
#endif //FEATURE_WAP
#ifdef FEATURE_SKVM
		if (ui_sms_msg.tele_srv == SMSI_SKVM_LOAD)
		{
            if((memchr((byte *)ui_sms_msg.msg_data.user_data.chari,MSD_SEPERATOR,ui_sms_msg.msg_data.user_data.num_fields))==NULL)
            {
			    i =0;
			    goto format2;
		    }

			memset(g_pszmsdURL, NULL, MSDURL_MAX);
			
			for(i=0; i < ui_sms_msg.msg_data.user_data.num_fields; i++) 
            {
				if(ui_sms_msg.msg_data.user_data.chari[i] != MSD_SEPERATOR)
				   g_pszmsdURL[i] = ui_sms_msg.msg_data.user_data.chari[i];
				else break;
            }
format2:
			 if(i >= ui_sms_msg.msg_data.user_data.num_fields) 
            {
		       STRCPY(&pFormatBuf[nPos], (BYTE *)SKY_GET_TOKEN(TKN_SMS_MSG_NO_CONTENT));
               sLineLength = 1; 
            }


            if(i < ui_sms_msg.msg_data.user_data.num_fields)

            {
                sLineLength = ui_sms_auto_format (
                            TRUE,
                            (byte *) &ui_sms_msg.msg_data.user_data.chari[i], 
                            ui_sms_msg.msg_data.user_data.num_fields - i,
                            (byte *) &pFormatBuf[nPos],
                            ((MAX_MSG_VIEW_LINE-5)*SMS_SCREEN_SIZE));      // 메일 확인 1라인을 남겨둠
            }
        }
		else
#endif
        {        
            sLineLength = ui_sms_auto_format (
	                        TRUE,
	                        (byte *) ui_sms_msg.msg_data.user_data.chari, 
	                        ui_sms_msg.msg_data.user_data.num_fields,
	                        (byte *) &pFormatBuf[nPos],
	                        ((MAX_MSG_VIEW_LINE-5)*SMS_SCREEN_SIZE));      // 회신번호 2라인을 남겨둠
        }
    } 
    else {
        (void) memcpy ( (void*) &pFormatBuf[nPos],     // 메시지
		                    "(내용없음)      ", SMS_SCREEN_SIZE);
        sLineLength = 1;
    }

    /* Set final text message line length */
    *nLine = sLineLength;
    nPos += sLineLength * SMS_SCREEN_SIZE;

    /* Display call back number if page or message */
    if (( ui_sms_msg.sms_param_m & SMSI_CALL_BACK) != 0)
    {
        if( ui_sms_msg.msg_data.call_back.num_fields > 32)
        {
            (void) memcpy ( pCallback, 
                            (const void *) ui_sms_msg.msg_data.call_back. chari,
                            32);
        }
        else
        {
            (void) memcpy ( pCallback, 
	  			        (const void *) ui_sms_msg.msg_data.call_back.chari, 
                        ui_sms_msg.msg_data.call_back.num_fields );  
        }
    }

    /* undersea 2001-08-20 --> @why, what */
#ifdef FEATURE_GVM
    if (sKind == GVM_DL_SMS) 
        sParsingGVMMessage(pFormatBuf, FALSE);
    else 
#endif
    /* <-- */    
        paMsgViewBuf = pFormatBuf;

    return ( sKind );
}

void gDeleteMsg
( 
  void
)
{
    INT8    i;

  /* Initialize all SMS lists count to indicate */
  /* no entries in their lists.                 */
  for (i=0; i<MAX_SMS_LIST; i++)
  {
    ui_sms_list[i].cnt = 0;
  }

  /* Initialize list of available SMS NV indicies to  */
  /* all NV indexes if no SMS control list available. */
  ui_sms_list[UI_SMS_AVAIL].cnt = MAX_NV_MSGS;
  for (i=0; i<MAX_NV_MSGS; i++)
  {
    /* Setup NV free address */
    ui_sms_nv_item.address = i + 1;

    /* Free memory allocation for specified item stored in NV.  */
    /* Ignore return status since we attempting error recovery. */
    (void) SKY_FreeNV( NV_SMS_I, ( nv_item_type * )&ui_sms_nv_item );

    ui_sms_list[UI_SMS_AVAIL].nv_idx[i] = i + 1;
  }

  /* Perform the SMS power down processing which   */
  /* saves SMS control list in NV.  SMS control    */
  /* list size will not change. Need to ensure     */
  /* that there is always enough NV memory to save */
  /* the SMS control list so allocate space now.   */
  (void) ui_sms_power_down ();
}

//////////////////////////////////////////////////////////////////////

#ifdef FEATURE_SMS_PACKET

/*===========================================================================
FUNCTION UI_SMS_NEW_TXT_MSG

DESCRIPTION
  Returns True is there are any new SMS text messages
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_sms_new_txt_msg
(
  void
)
{
  if (ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0)
  {
    return TRUE;

  } /* if ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt > 0 */
  else if (ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0)
  {
    return TRUE;

  } /* if ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0 */
  else
  {
    return FALSE;

  } /* else */

} /* ui_sms_new_txt_msg */

/*===========================================================================
FUNCTION UI_SMS_OLD_TXT_MSG

DESCRIPTION
  Returns True is there are any old SMS messages that haven't been deleted
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_sms_old_txt_msg
(
  void
)
{
  if (ui_sms_list[UI_SMS_TXT_OLD].cnt > 0)
  {
    return TRUE;

  } /* if ui_sms_list[UI_SMS_TXT_OLD].cnt > 0 */
  else 
  {
    return FALSE;

  } /* else */
} /* uis_sms_old_txt_msg */

/*===========================================================================
FUNCTION UI_SMS_VC_MSG

DESCRIPTION
  Returns True is there are any SMS voice messages
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_sms_vc_msg
(
  void
)
{
  if (ui_sms_list[UI_SMS_VC].cnt > 0)
  {
    return TRUE;

  } /* if ui_sms_list[UI_SMS_VC].cnt > 0 */
  else
  {
    return FALSE;

  } /* else */

} /* ui_sms_vc_msg */

/*===========================================================================
FUNCTION UI_SMS_DELETE_SINGLE

DESCRIPTION
  Deletes a single SMS message, as directed by "new" and "text".  If new is
  TRUE, delete the very first one.  Otherwise delete the very last one

DEPENDENCIES
  Should only be called after a read operation

SIDE EFFECTS
  None

===========================================================================*/

boolean ui_sms_delete_single( boolean new, boolean text )
{
  ui_sms_op_status_type status;
    /* SMS operation status */

  list_info.cmd = UI_SMS_DELETE;

  if ( text)
  {
    
    /* Any message on either of the 
    ** NEW_TXT lists are un-read.  So,
    ** don't erase them
    */

    if ( ui_sms_old_txt_msg())
    {
      list_info.src_list = UI_SMS_TXT_OLD;

      if ( new)
      {

        list_info.src_idx = ui_sms_list[UI_SMS_TXT_OLD].cnt - 1;

      } /* if new */
      else
      {

        list_info.src_idx = 0;

      } /* else */

      /* Actually do the delete operation
      */

      status = ui_sms_list_cmd();

      if ( status == UI_SMS_OP_OK)
      {
        /* Update SMS control information after  */
        /* message has been deleted.  Write      */
        /* control info into NV using power-down */
        /* function.                             */
        (void) ui_sms_power_down();

        return TRUE;
      } /* if status == UI_SMS_OP_OK */

    } /* if ui_sms_old_txt_msg */

    
    /* If control reaches here, there
    ** was no Text SMS message to be
    ** deleted
    */

    return FALSE;


  } /* if text */
  else
  {
    
    if ( ui_sms_vc_msg())
    {

      list_info.src_list = UI_SMS_VC;

      if ( new)
      {
      
        list_info.src_idx = ui_sms_list[UI_SMS_VC].cnt - 1;

      } /* if new */
      else
      {

        list_info.src_idx = 0;

      } /* else */

      /* Actually do the delete operation
      */

      status = ui_sms_list_cmd();

      if ( status == UI_SMS_OP_OK )
      {
        /* Update SMS control information after  */
        /* message has been deleted.  Write      */
        /* control info into NV using power-down */
        /* function.                             */
        (void) ui_sms_power_down();

        /* When the VMN message is deleted (there*/
        /* can be only one).  Then, the next one */
        /* that is received is un-read to begin  */
        /* with and therefore is new             */

        uismsl_vmn_new = TRUE;

        return TRUE;

      } /* if status == UI_SMS_OP_OK */


    } /* if ui_sms_vc_msg */

    /* If control reaches here, there
    ** was no Text SMS message to be
    ** deleted
    */

    return FALSE;

  } /* else */

} /* ui_sms_delete_single */


/*===========================================================================
FUNCTION UI_CHECK_MORE_SMS_TO_READ

DESCRIPTION
  Check if there are any of the specified type of message available 
  for deletion

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean ui_check_more_sms_to_delete
(
  boolean                 text
)
{
  if ( text )
  {
    return ( ui_sms_old_txt_msg() );

  } /* if text */
  else
  {
    return ( ui_sms_vc_msg() );

  } /* else */

} /* ui_check_more_sms_to_delete */


/*===========================================================================
FUNCTION UISMSL_RETURN_SMS

DESCRIPTION
  This function returns the result of an SMS read operation to the calling
  task

DEPENDENCIES
  This function should be called only after the completion of a successful
  SMS read command and before the commencement of another.

SIDE EFFECTS
  None

===========================================================================*/
smsi_msg_type uismsl_return_sms( void )
{
  return ui_sms_msg;
} /* uismsl_return_sms */


/*===========================================================================
FUNCTION UISMSL_RETURN_READ_STATUS

DESCRIPTION
  Returns the status of the read operation

DEPENDENCIES
  Should be called after the read operation is completed

SIDE EFFECTS
  None

===========================================================================*/
byte uismsl_return_read_status( void )
{
  return uismsl_read_status;
} /* uismsl_return_sms */


/*===========================================================================
FUNCTION UISMSL_RETURN_DELETE_STATUS

DESCRIPTION
  Returns the status of the delete operation

DEPENDENCIES
  Should be called after the delete operation is completed

SIDE EFFECTS
  None
===========================================================================*/
byte uismsl_return_delete_status( void )
{
  return uismsl_delete_status;
} /* uismsl_return_sms */


/*===========================================================================
FUNCTION UI_SMS_DELETE

DESCRIPTION
  Deletes the SMS messages that are indicated by erase_code

DEPENDENCIES
  Should only be called after a read operation

SIDE EFFECTS
  Will erase varying numbers of SMS message, depending on the erase_code

===========================================================================*/
void ui_sms_delete
(
  mod_sms_erase_code_type erase_code
)
{
  boolean cont;
  boolean text = FALSE;
 
  uismsl_delete_status = TRUE;

  switch( erase_code)
  {
    case MOD_SMS_DONT_ERASE:
    /* Don't do anything, since nothing needs to be erased */

      return;

    break;

    case MOD_SMS_ERASE_ALL:

      /* Delete all voice SMS messages (mostly likely
      ** just one and then fall through and delete 
      ** all the text SMS messages
      */
      
      if ( ( ui_check_more_sms_to_delete( TRUE)  == FALSE) &&
           ( ui_check_more_sms_to_delete( FALSE) == FALSE))
      {
        /* If we're told to delete and there aren't
        ** any SMS messages to delete, delete failed
        */
        
        uismsl_delete_status = FALSE;
        return;
      }

      while( ui_check_more_sms_to_delete( (boolean)FALSE))
      {
        /* uismsl_delete_status will be true if all of the
        ** return values are true.  The very first return
        ** value is the most likely to be true.  If the
        ** very first return value is FALSE, all others
        ** are likely to be false also
        */

        uismsl_delete_status &= ( ui_sms_delete_single( (boolean)TRUE, (boolean)FALSE));

      } /* while cont */

      
      /* FALL THROUGH, Fall Through. fall through.  Fall through */
      
    case MOD_SMS_ERASE_ALL_TEXT:

      text = TRUE;

      /* FALL THROUGH, Fall Through. fall through.  Fall through */

    case MOD_SMS_ERASE_ALL_VOICE:

      if ( ( erase_code == MOD_SMS_ERASE_ALL_VOICE) && ( text))
      {
#ifndef WIN32
        ERR_FATAL( "ui_sms_delete: Bad Fall Through.  The logic has somehow been corrupted\n", 0, 0, 0);        
#endif // 
      } /* if erase_code == MOD_SMS_ERASE_ALL_VOICE */

      if ( ( ui_check_more_sms_to_delete( text) == FALSE) &&
           ( ( erase_code == MOD_SMS_ERASE_ALL_TEXT)      ||
             ( erase_code == MOD_SMS_ERASE_ALL_VOICE)))
      {
        /* If we're told to delete and there aren't
        ** any SMS messages to delete, delete failed
        */

        uismsl_delete_status = FALSE; 
        return;
      }

      while( ui_check_more_sms_to_delete( text))
      {
        /* uismsl_delete_status will be true if all of the
        ** return values are true.  The very first return
        ** value is the most likely to be true.  If the
        ** very first return value is FALSE, all others
        ** are likely to be false also
        */

        uismsl_delete_status &= ( ui_sms_delete_single( (boolean)TRUE, text));

      } /* while cont */

      return;

    break; /* ERASE_ALL_VOICE || ERASE_ALL_TEXT || ERASE_ALL */

    case MOD_SMS_ERASE_NEWEST_TEXT:

      uismsl_delete_status = ui_sms_delete_single( (boolean)TRUE, (boolean)TRUE);
      return;

    break; /* MOD_SMS_ERASE_NEWEST_TEXT */

    case MOD_SMS_ERASE_OLDEST_TEXT:

      uismsl_delete_status = ui_sms_delete_single( (boolean)FALSE, (boolean)TRUE);
      return;

    break; /* MOD_SMS_ERASE_OLDEST_TEXT */

    case MOD_SMS_ERASE_NEWEST_VOICE:
      
      uismsl_delete_status = ui_sms_delete_single ( (boolean)TRUE, (boolean)FALSE);
      return;

    break; /* MOD_SMS_ERASE_NEWEST_VOICE */

    case MOD_SMS_ERASE_OLDEST_VOICE:
      
      uismsl_delete_status = ui_sms_delete_single( (boolean)FALSE, (boolean)FALSE);
      return;

    break; /* MOD_SMS_ERASE_OLDEST_VOICE */

    default:

#ifndef WIN32
      ERR_FATAL( "Undefined erase code in GET_SMS message", 0, 0, 0);
#endif //     
      /* The following statement does not get executed, 
      ** but this is an attempt to appease the compiler
      ** and lint demons
      */

      return;

    break;
  }

} /* ui_sms_delete */


/*===========================================================================

FUNCTION UISMSL_GET_SMS_MESSAGE

DESCRIPTION
  Gets an SMS message.  

DEPENDENCIES
  None

RETURN VALUE
  Indicates whether or not there was an SMS message to read, whether or
  not there are more to read, whether the SMS message was voice or text,
  whether or not the SMS was a new (un-read) message, etc  

SIDE EFFECTS
  None

===========================================================================*/

void uismsl_get_sms_message( void )
{
  ui_sms_read_op_type sms_read;

  /* Initialize return value to the default
  */

  uismsl_read_status = 0;
  
  /* QC phones count key pad input
  ** as acknowledgement of SMS 
  ** message.  Module has to take
  ** the execution of this procedure
  ** as acknowledgement of SMS
  ** message
  */

  ui_sms_alert_status = FALSE;

  if ( ui_sms_new_txt_msg() ) 
  {
    /* There's a new (un-read) text SMS
    ** message
    */
    sms_read.cmd         = UI_SMS_READ_ONE;

    if (ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt > 0)
    {
      sms_read.read_list = UI_SMS_TXT_NEW_URGENT;
      sms_read.read_idx  = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt - 1;
    }
    else
    {
      sms_read.read_list = UI_SMS_TXT_NEW_NOT_URGENT;
      sms_read.read_idx  = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt - 1;
    }
        
    ui_sms_read_text( &sms_read);

    uismsl_read_status |= MOD_SMS_TXT_MESSAGE;

    if ( ui_sms_vc_msg() )
      uismsl_read_status |= MOD_SMS_MORE_MESSAGES;

  } /* else if ui_sms_new_txt_msg */
  else if ( ui_sms_vc_msg() )
  {
    /* There's a voice SMS message 
    ** available
    */

    sms_read.cmd       = UI_SMS_READ_ONE;
    sms_read.read_list = UI_SMS_VC;
    sms_read.read_idx  = 0; /* There can be only one */

    ui_sms_read_voice( &sms_read);

    if ( uismsl_vmn_new)
    {
      sms_read.new = TRUE;  
    }

    uismsl_vmn_new = FALSE;

  } /* else if ui_sms_vc_msg */
  else if( ui_sms_old_txt_msg() )
  {
    /* If we're told to read an SMS
    ** message and there are only 
    ** old SMS text messages to be
    ** read, read an old SMS text
    ** message
    */
    
    sms_read.cmd       = UI_SMS_READ_ONE;
    sms_read.read_list = UI_SMS_TXT_OLD;
    sms_read.read_idx  = ui_sms_list[UI_SMS_TXT_OLD].cnt - 1;

    ui_sms_read_text( &sms_read );

    uismsl_read_status |= MOD_SMS_TXT_MESSAGE;

  } /* else if ui_sms_old_txt_msg */
  else
  {
    /* else do nothing.  The default flag will indicate that
    ** there were no SMS messages to be read.  Return
    */

    return;

  }

  /* The fact that control flow has reached
  ** here indicates that there was some SMS
  ** message to be read
  */

  uismsl_read_status |= MOD_SMS_MESSAGES_EXIST;

  if ( sms_read.msg_read == FALSE )
  {
    /* Message Read failed for 
    ** some reason
    */

    uismsl_read_status |= MOD_SMS_READ_FAIL; 
   
  } /* if sms_read.msg_read == FALSE */

  if ( sms_read.new )
  {
    /* The SMS message that was read
    ** was new
    */

    uismsl_read_status |= MOD_SMS_NEW_MESSAGE;

  } /* if sms_read.new */

  if ( ui_sms_new_txt_msg() )
  {
    /* There are more text SMS messages
    ** to be read
    */

    uismsl_read_status |= MOD_SMS_MORE_MESSAGES;

  } /* if ui_sms_new_txt_msg */

  
  return;

} /* uismsl_get_sms_message */

#endif /* FEATURE_SMS_PACKET */

#endif  /* FEATURE_UASMS defined */

