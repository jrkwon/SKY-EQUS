/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_hcmd.c
**
** Description: 
**     The core functions that provide EQS functionality.
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-11-27   redstar    modify
** 00-11-04   jrkwon     re-created
** 00-10-12   redstar    created
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <string.h>

#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "msg.h"
#include "err.h"
#include "mod.h"
#include "rxc.h"

#include "sky.h"


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#ifndef FEATURE_UASMS

#ifdef FEATURE_ACP
#define NAMPS_STX '\\'   /* Start-of-TX character */
#define NAMPS_ETX 3      /* End-of_TX character */
static char nvoice[ UI_WIDE*2+16 ]; /* voice mail message */
#endif /* FEATURE_ACP */

#endif /* FEATURE_UASMS */

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  General Declarations
**---------------------------------------------------------------------------
*/
extern void SetupCalledPartyNumber(UINT8 nLength, BYTE bpszNumberBuffer[]);
extern int  SetupCNAP(BYTE* pszNameBuf, BYTE nNameLen);


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

UI_COMMAND_U        g_EventParam;

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
BROADCAST_SMS_T     g_BroadcastSMS;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

#ifdef FEATURE_SKT_EIF  //redstar 2001.02.11
extern SKY_EVENT_E EIF_Event;

extern BOOL  g_fSMSNewMsg;      //redstar  2001.02.12  for sms new message
extern byte  g_nNewMsg ;        //redstar  2001.02.12  for newmsg command

#endif //FEATURE_SKT_EIF

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

#ifdef FEATURE_IS95B_EXT_DISP 
/* Whether the CLI stored represents the whole caller information from
** the same OTA message, (including number and name)
*/
BOOL fCallerIDComplete;
extern void ClearCLI(void);
#else
BOOL fCallerIDComplete;      /* must be fixed!!! */
extern void ClearCLI(void);  /* hjcho 01.02.09 */
#endif

extern void ui_update_cm_call_info(const cm_call_info_s_type* cm_call_info_ptr);
extern  void g_NewMybellSave(UI_COMMAND_U *pCommand);
extern word ui_sms_store_new_msg(ui_cmd_type *cmd_ptr);
extern void ui_update_cm_inband_info(const cm_inband_info_s_type* cm_inband_info_ptr);
extern boolean ui_curr_call_allow_sound(void);

#ifdef FEATURE_SKTT_EQUS_LOCATION
extern WORD SetRcvLocationSmsMsg(ui_cmd_type *cmd_ptr);
#endif

#ifdef FEATURE_SKVM
extern WORD SetRcvSKVMDataSmsMsg(ui_cmd_type *cmd_ptr);
#endif
/*
**---------------------------------------------------------------------------
**  Local Function Declaration
**---------------------------------------------------------------------------
*/
LOCAL void SKY_API sDefaultCallEventHandler(CONST ui_call_event_type *call_event_ptr);
LOCAL void SKY_API sDefaultPhoneEventHandler(const ui_ph_event_type *ph_event_ptr);
LOCAL void SKY_API sNotifyVoicePrivacyChange(const cm_call_info_s_type *call_info_ptr);
LOCAL void SKY_API sUpdateVoiceAsDataDisplay(cm_answer_voice_e_type  voice_as_data);
LOCAL void SKY_API sValidateSysPref(void);
LOCAL void SKY_API sValidateModePref(void);
LOCAL int  SKY_API sDisplayExtInfo(UI_COMMAND_U *pCommand);
LOCAL void SKY_API sSetUpCalledPartyNumber(UI_COMMAND_U *pCommand);


/*
**---------------------------------------------------------------------------
**  Local Function Definitions
**---------------------------------------------------------------------------
*/


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
BOOL IsCallerIDComplete(void)
{
    return fCallerIDComplete;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void SetCallerIDComplete(BOOL fValue)
{
    fCallerIDComplete = fValue;
}



/*===========================================================================
FUNCTION default_handle_call_event

DESCRIPTION
  Default process of call event

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void SKY_API sDefaultCallEventHandler(CONST ui_call_event_type *call_event_ptr)
{
    /* uiscall.c */
    ui_update_cm_call_info( &(call_event_ptr->call_info) );
}


/*===========================================================================
FUNCTION default_handle_ph_event

DESCRIPTION
  Default process of phone event

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void SKY_API sDefaultPhoneEventHandler(const ui_ph_event_type *ph_event_ptr)
{
	//--> mod_event_from_ph_event(ph_event_ptr);
    //CALL_SET_CM_PHONE_INFO(ph_event_ptr->ph_info);
    SKY_SET_PHONE_INFO( ph_event_ptr->ph_info);
}



/*===========================================================================
FUNCTION notify_voice_privacy_change

DESCRIPTION
  Update UI voice privacy indication
  change display and may play sound

DEPENDENCIES
  ui.areabeep and ui.ignore_privacy

RETURN VALUE
  None

SIDE EFFECTS
  ui.ignore_privacy may be changed
===========================================================================*/
LOCAL void SKY_API sNotifyVoicePrivacyChange(const cm_call_info_s_type *call_info_ptr)
{
    SKY_ASSERT(call_info_ptr->call_state == CM_CALL_STATE_CONV);
    /* SKY : do nothing */    
}

/*===========================================================================
FUNCTION update_ui_voice_as_data_disp

DESCRIPTION
  change banner display per voice as data options

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void SKY_API sUpdateVoiceAsDataDisplay(cm_answer_voice_e_type  voice_as_data)
{
    switch (voice_as_data) 
    {
    case CM_ANSWER_VOICE_AS_VOICE:
        //(void)uiscrn_deact(UI_VOICE_AS_DATA_BANNER_F);
        break;

    case CM_ANSWER_VOICE_AS_FAX_ONCE:
    case CM_ANSWER_VOICE_AS_FAX_ALWAYS:
        //(void)uiscrn_bind_act(UI_VOICE_AS_DATA_BANNER_F, (char *)UI_FAX_ONLY_TXT);      
        break;

    case CM_ANSWER_VOICE_AS_MODEM_ONCE:
    case CM_ANSWER_VOICE_AS_MODEM_ALWAYS:
         //(void)uiscrn_bind_act(UI_VOICE_AS_DATA_BANNER_F, (char *)UI_DATA_ONLY_TXT);      
        break;

    default:
        ASSERT(FALSE);
        break;
  }
}

/*===========================================================================
FUNCTION validate_sys_pref

DESCRIPTION
  Validate system preference mode, some valid CM modes may not be
  valid for UI, change it to UI-valid system pref mode in those cases

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void SKY_API sValidateSysPref(void)
{
    /* The following table contains valid the preferred system setting
    ** at power up, with the default setting be the first item in the table.
    **
    ** For PRL enabled build (FEATURE_PRL_FORCE_MODE is enabled)
    ** the valid values are Standard, and Home only. If the target supports AMPS
    ** A side and B side are also valid.
    ** For non-PRL enabled AMPS capable build, the valid values are Home preferred,
    ** and Home only. If the target supports AMPS A side and B side are also valid.
    **
    ** If the value at power-up is found invalid, for PRL
    ** enabled build, it is default as Standard.
    ** For non-PRL enabled build, it is default as Home Preferred.
    **
    ** Since the system preference enumerated type for SSPR enhanced target
    ** and non-SSPR enhanced target are different, the table are defined
    ** depending on whether the feature is enabled.
    */

    static const cm_sys_pref_e_type sys_pref_init_tbl[] = 
    {
        CM_SYS_PREF_STANDARD,  /* Default setting */
        CM_SYS_PREF_HOME_ONLY,
#if ( defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) )
        /* For TriMode, A & B are not valid options */
#elif defined(FEATURE_CDMA_800) 
        CM_SYS_PREF_A_ONLY,
        CM_SYS_PREF_B_ONLY,
#endif
    };

    int i;
    for (i=0; i<ARR_SIZE(sys_pref_init_tbl); i++) 
    {
        if (sys_pref_init_tbl[i] == SKY_GET_PHONE_INFO().sys_pref) 
        {
            break;    
        }
    }

    if (i == ARR_SIZE(sys_pref_init_tbl) ) 
    {
        // no matching, initial value not valid
        ui_set_ph_sys_pref(sys_pref_init_tbl[0]);
    }
}



/*===========================================================================
FUNCTION validate_mode_pref

DESCRIPTION
  Validate mode preference mode, some valid CM modes may not be
  valid for UI, change it to UI-valid mode pref in those cases

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
LOCAL void SKY_API sValidateModePref(void)
{
    /* The following table contains valid the preferred mode setting
    ** at power up, with the default setting be the first item in the table.
    **
    ** For PRL enabled build (FEATURE_SSPR_ENHANCEMENTS or
    ** FEATURE_PRL_FORCE_MODE or FEATURE_HOME_ONLY
    ** is enabled) the valid values are Automatic, Digital only and Home
    ** only, depending on which UI menu is enabled.
    ** For non-PRL enabled AMPS capable build,  the only valid value is
    ** Digital Preferred.
    ** For non-PRL enabled non-AMPS capable (PCS only) build, the only valid
    ** value is Digital Only.
    **
    ** If the value at power-up is found invalid, for PRL
    ** enabled build, it is default as Automatic.
    ** For non-PRL enabled AMPS capable build, it is default as Digital
    ** Preferred.
    ** For non-PRL enabled non-AMPS capable build, it is default as Digital
    ** Only.
    **
    */
    static const cm_mode_pref_e_type mode_pref_init_tbl[] = 
    {
#if defined(FEATURE_ACP)
      #if defined(FEATURE_SSPR_ENHANCEMENTS) || defined(FEATURE_PRL_FORCE_MODE)
      CM_MODE_PREF_AUTOMATIC,    /* Default setting */
      #else  /* !FEATURE_SSPR_ENHANCEMENTS  && !FEATURE_PRL_FORCE_MODE */
      CM_MODE_PREF_DIGITAL_PREF, /* Default setting */
      #endif /* !FEATURE_SSPR_ENHANCEMENTS  && !FEATURE_PRL_FORCE_MODE */
#else  /* !FEATURE_ACP */
      CM_MODE_PREF_DIGITAL_ONLY, /* Default setting */
#endif /* FEATURE_ACP */
  
    /* TriMode */
#if ( defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) )
      CM_MODE_PREF_CELL_CDMA_ONLY,
      CM_MODE_PREF_PCS_CDMA_ONLY,
#endif /* TriMode */
    
    /* Other valid value */
#if defined(FEATURE_UI_FORCE_MODE_SEL) || \
    defined (FEATURE_UI_PRL_FORCE_MODE_SEL)
      CM_MODE_PREF_DIGITAL_ONLY,
#endif /* FEATURE_UI_FORCE_MODE_SEL || FEATURE_UI_PRL_FORCE_MODE_SEL */
#ifdef FEATURE_UI_HOME_ONLY
#error code not present
#endif /* FEATURE_UI_HOME_ONLY */
    };

    int i;
    for (i=0; i<ARR_SIZE(mode_pref_init_tbl); i++) 
    {
        if (mode_pref_init_tbl[i] == SKY_GET_PHONE_INFO().mode_pref) 
        {
          break;
        }    
    }

    if (i == ARR_SIZE(mode_pref_init_tbl) ) 
    {
        // no matching, initial value not valid
        ui_set_ph_mode_pref(mode_pref_init_tbl[0], CM_MODE_TERM_PERMANENT, 0);
    }
}




/*===========================================================================

FUNCTION sSetUpCalledPartyNumber

DESCRIPTION
  Setup Called Party Number

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Set CDNIP info.

===========================================================================*/
LOCAL void SKY_API sSetUpCalledPartyNumber(UI_COMMAND_U *pCommand)
{
	BYTE         pszTmpBuf[20];
	UINT8        i;
    
    for(i=0; i<pCommand->call_event.call_info.num.len; i++)
    {
      pszTmpBuf[i] = pCommand->call_event.call_info.num.buf[i];
	}
	pszTmpBuf[i] = '\0';

	SetupCalledPartyNumber(pCommand->call_event.call_info.num.len, pszTmpBuf);  
}


#ifdef FEATURE_IS95B_EXT_DISP   

/*===========================================================================

FUNCTION UI_EXT_DISP

DESCRIPTION
  Handles an extended display information (UI_EXT_DISP_F) command
  from the MC.

  If the extended display info record is of normal type, for each
  CNAP record, update the CNAP information and setup CNAP display.

  Otherwise, ignore the whole extended display info message.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Change CNAP information and CNAP display.

===========================================================================*/
LOCAL int SKY_API sDisplayExtInfo(UI_COMMAND_U *pCommand)
{
    cm_ext_disp_itr_type  itr; /* Extended display info iterator */
    cm_ext_disp_subrecord rec; /* Extended display info subrecord */
    int                   nRecLength;

    nRecLength = 0;

    SKY_TRACE(TL_MED, "Got an extended display from MC");

    /* Current UI handles normal extended display only */
    if (pCommand->call_event.call_info.ext_disp_type != CAI_EXT_DISP_TYPE_NORMAL)
    {
        SKY_TRACE(TL_HIGH, "Not normal extended display, discard");
        return nRecLength;
    }

    /* Loop through sub-records one-by-one.
    ** If it is CNAP, set up the CLI display. Note that only the lastr
    ** CNAP record is shown on screen.
    */
    for (cm_ext_disp_itr_reset(&itr, &(pCommand->call_event.call_info.alpha));
       cm_ext_disp_itr_more(&itr);
       cm_ext_disp_itr_next(&itr))
    {
        /* Get a sub-record from the message */
        cm_ext_disp_itr_get(&itr,&rec);

        /* Check if the sub-record contains CNAP information.
        ** Current UI handles CNAP only. The rest not supported are ignored.
        */
        if (rec.display_tag == CAI_EXT_DISP_CALLING_NAME)
        {
            /* It is the first of the caller information, clear the one
            ** previously received. Assume there are more to follow in
            ** the same extended display message.
            */
            //if (fCallerIDComplete)                 hjcho 01.02.13
            //{
            //    ClearCLI();
            //    fCallerIDComplete = FALSE;
            //}
            /* Set up the information as CNAP */
            nRecLength = SetupCNAP(rec.chari, rec.display_len);            
        }
        /* if (rec.display_tag == CM_EXTDISP_TAG_CNAP) */
  } /* for (cmcall_ext_disp_itr_reset... */

  /* Save whether it is the last caller information received for
  ** this OTA message.
  */
  
//  fCallerIDComplete =
//    (pCommand->ext_disp.hdr.last_record)? TRUE:FALSE;

  return nRecLength;
} /* end ui_ext_display */

#endif /* FEATURE_IS95B_EXT_DISP */


/*
**---------------------------------------------------------------------------
**  Global Function Definitions
**---------------------------------------------------------------------------
*/

/*===========================================================================
FUNCTION UI_HANDLE_CMDS

DESCRIPTION
  Handle commands to the user interface task
  Input events to UI state machine if necessary
  Many of them copied from uistate_cmds()
  Need to simplify it further

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
/* UI SMS operational status */
typedef enum
{
  UI_SMS_OP_IN_PROG,   /* UI SMS operation in progress      */
  UI_SMS_OP_NV_ERR,    /* NV status error returned          */
  UI_SMS_OP_NO_IDX,    /* No NV index to add or delete      */
  UI_SMS_OP_DUPLICATE, /* Received message is a duplicate   */
  UI_SMS_OP_OK         /* UI_SMS operation successful       */
} ui_sms_op_status_type;

void SKY_API SKY_HandleCommands(UI_COMMAND_U *pCommand)
{
    //static char *lowerpower_txt_ptr; /* ptr to text field for UI_LOWPOWER_F */
#ifndef FEATURE_UASMS
    byte temp_disp[250];// HS_SCRN_WIDE * 2];  <-- temp
#else
    ui_sms_op_status_type sto_status;
    BOOL    fT1AskRequested;
    /* SMS store status */
   #ifdef FEATURE_BROADCAST_SMS
   int i;
   #endif /* FEATURE_BROADCAST_SMS */
#endif

    switch( pCommand->hdr.cmd )
    {
	//asdf 2001.09.18 partial display에서 빠져 나올때 Annunciator부분에서 update가 안될때도 있어서 
	//강제로 그려줌 
	case UI_END_PARTIAL_CMD:
		//SKY_API SKY_PartialDisplay_OFF();
		
        /*SKY_DrawAnnunciators();
		SKY_UpdateScreen();	*/
        SKY_DoEvent(SE_SYS_UPDATE_DISPLAY);
		break;

#ifdef FEATURE_SMS_PACKET
    case UI_GET_SMS_F:

        uismsl_get_sms_message( );
        break; /* UI_GET_SMS_F */

    case UI_DELETE_SMS_F:
        ui_sms_delete( pCommand->sms_delete.erase_code );
        break; /* UI_DELETE_SMS_F */

#endif /* FEATURE_SMS_PACKET */



#ifdef FEATURE_NV_WRITE_ONLINE

    case UI_NV_WRITE_ONLINE_F:
        uistate_update_nv_shadow( pCommand->nv_write_online.item, &pCommand->nv_write_online.item_data );
        break; /* UI_NV_WRITE_ONLINE_F */
#endif /* #ifdef FEATURE_NV_WRITE_ONLINE */



#ifdef FEATURE_AUDIO_CONTROL
        #error code not present
#endif /* FEATURE_AUDIO_CONTROL */

    /* Analog/CDMA Abbreviated Alert ( High & Low Tones 25ms on, 25 ms off )  for 2 seconds */
    case UI_CALL_ABRV_ALERT_F:
        SKY_PlaySoundInHandset(UI_CALL_SND, SND_ABRV_ALERT, MOD_SOUND );        
        //SKY_DoEvent((word)UI_DOABRVALERT_F );
        SKY_DoEvent( SE_SND_ABBR_ALERT );
        break;

    /* Analog Abbreviated Reorder (Reorder for 4 seconds) */
    case UI_CALL_ABRV_REORDER_F:
        SKY_PlayCallFailSound(SND_ABRV_REORDER);
        break;

    /* Analog Abbreviated Intercept (Intercept for 4 seconds) */
    case UI_CALL_ABRV_INTERCEPT_F:
        SKY_PlayCallFailSound(SND_ABRV_INTERCEPT);
        break;

    case UI_CALL_DISPLAY_F:            /* Display CDMA info (Alert or Flash) */
#ifdef FEATURE_IS95B_EXT_DISP 
        /* Save whether it is the last caller information received for this OTA message.*/
//        if (fCallerIDComplete) 
//        {
//            ClearCLI();
//            fCallerIDComplete = FALSE;
//        }

        fCallerIDComplete = pCommand->call_event.call_info.is_last_cdma_info_rec? TRUE:FALSE;
#endif // FEATURE_IS95B_EXT_DISP    

        /* 
        **   CM_DISPLAY_F
        **
        **   alpha.buf[] = "02/05 20:32 0112980012"
        **   alpha.len = 22
        */
        SET_EVENT_PARAM( *pCommand );          
        SKY_DoEvent(SE_CALL_DISPLAY);

        //SKY_PutNewInfoMessage(pCommand->hdr.cmd,
        //                      pCommand->call_event.call_info.alpha.len,
        //                      (BYTE *)pCommand->call_event.call_info.alpha.buf );

        break;

#ifdef FEATURE_IS95B_EXT_DISP 
    case UI_CALL_EXT_DISPLAY_F:
        {
            int nNameLength;

            nNameLength = sDisplayExtInfo(pCommand);

            if(nNameLength > 0)
            {
              SKY_DoEvent(SE_CALL_EXT_DISPLAY);
              /* Save whether it is the last caller information received for
              ** this OTA message.
              */
              fCallerIDComplete = (pCommand->call_event.call_info.is_last_cdma_info_rec)? TRUE:FALSE;
            }
        }
        break;
#endif //FEATURE_IS95B_EXT_DISP

#ifndef FEATURE_UASMS

    case UI_SMS_MSG_WAITING_F:        /* AMPS/CDMA display message count */
        /* Otherwise, use the normal display mechanism */
        (void) memcpy ( &temp_disp, UI_MWN_TXT, (size_t)(HS_SCRN_WIDE*2) );
        temp_disp[0] = "0123456789"[pCommand->sms_event.sms_info.msgs/100];
        temp_disp[1] = "0123456789"[(pCommand->sms_event.sms_info.msgs%100)/10];
        temp_disp[2] = "0123456789"[pCommand->sms_event.sms_info.msgs%10];
        SKY_PutNewInfoMessage( pCommand->hdr.cmd, HS_SCRN_WIDE * 2, (char *)temp_disp );
        break;

#ifdef FEATURE_ACP
    case UI_SMS_N_VOICE_F: 
        {          /* Handle NAMPS voice message */
            int i, j;

            (void)memcpy( nvoice, UI_NVOICE1_TXT, UI_WIDE ); /* messages text */
            nvoice[0] = (char)pCommand->sms_event.sms_info.data[0]; /* number of message */
            nvoice[1] = (char)pCommand->sms_event.sms_info.data[1];
            if( pCommand->sms_event.sms_info.data[2] == '!' ) 
            { /* urgent */
                (void)memcpy( &nvoice[UI_WIDE], UI_NVOICE2_TXT, UI_WIDE );
                i = UI_WIDE*2;
            } 
            else 
            {
                i = UI_WIDE;
            }
            for( j=3; j<14; j++ ) 
            {     /* traverse NAMPS message */
                nvoice[i] = (char)pCommand->sms_event.sms_info.data[j];
                if( nvoice[i++] == NAMPS_ETX ) 
                { /* end of message */
                    break;
                }
            }
            SKY_PutNewInfoMessage( pCommand->hdr.cmd, i, nvoice );          
        }
    break;      
#endif /* FEATURE_ACP */

#endif /* FEATURE_UASMS */

    case UI_SMS_MO_STATUS_F:
#ifdef FEATURE_UASMS
        SET_EVENT_PARAM( *pCommand );
        //SKY_DoEvent( (word)UI_SMS_STS_MSG_F );
        SKY_DoEvent(SE_SMS_MO_STATUS_MESSAGE);
#else
        SKY_TRACE(TL_MED, "UI_SMS_MO_STATUS_F ignored");
#endif /* FEATURE_UASMS */
        break;

    case UI_SMS_STATUS_F:
#ifdef FEATURE_UASMS
        //SKY_DoEvent( (word)UI_SMS_STS_MSG_F );
        SKY_DoEvent(SE_SMS_STATUS_MESSAGE);
#else
        SKY_TRACE(TL_MED, "UI_SMS_STATUS_F ignored");
#endif /* FEATURE_UASMS */
        break;

    case UI_MYBELL_F:
        fT1AskRequested = ( pCommand->sms.buf_ptr->data.sms_param_m & SMSI_BEARER_REPLY ) ? TRUE : FALSE;
        g_NewMybellSave( pCommand );
        SKY_DoEvent(SE_SMS_MYBELL);
        if( fT1AskRequested == TRUE )
        {
            /* ack the msg to the SMS API
            */
            (void) uasms_ack_message( pCommand->sms.tid,
                                    UASMS_ERROR_NONE,
                                    UASMS_OK_S );
        }
        break;

#ifdef FEATURE_SKVM
	case UI_SKVM_STARTED:
		SKY_DoEvent(SE_SKVM_STARTED);
		break;
	case UI_SKVM_STOPPED:
		SKY_DoEvent(SE_SKVM_STOPPED);
		break;
    case UI_SKVM_STOPPED_TO_GVM:
		SKY_DoEvent(SE_SKVM_STOPPED_TO_GVM);
		break;
    case UI_SKVM_SMS_SEND:
        SKY_DoEvent(SE_SKVM_SMS_SEND);
		break;
    case UI_SKVM_VOICECALL_CONNECT:
        SKY_DoEvent(SE_SKVM_VOICECALL_CONNECT);
		break;
    case UI_SKVM_PAUSED:
		SKY_DoEvent(SE_SKVM_PAUSED);
		break;
	case UI_SKVM_RESUMED:
		SKY_DoEvent(SE_SKVM_RESUMED);
		break;
	case UI_SKVM_RECEIVED_DATA:
		SKY_DoEvent(SE_SKVM_RECEIVED_DATA);
		break;
    case UI_SKVM_DATA_CMDS:		
        fT1AskRequested = ( pCommand->sms.buf_ptr->data.sms_param_m & SMSI_BEARER_REPLY ) ? TRUE : FALSE;
		sto_status = (ui_sms_op_status_type)SetRcvSKVMDataSmsMsg( pCommand );
		SKY_TRACE(TL_HIGH, "UI_SKVMDATA_F ");

        if( fT1AskRequested == TRUE ){
			(void) uasms_ack_message( pCommand->sms.tid,
            UASMS_ERROR_NONE,
            UASMS_OK_S );
		}
		/* Store was successful then */
		/* send SMS message event.   */
		if ( sto_status == UI_SMS_OP_OK )
		{
			SKY_DoEvent(SE_SKVM_INPUT_DATA);

		}
        break;
	case UI_SKVM_PLAY_ALARM:
		SKY_DoEvent(SE_SKVM_PLAY_ALARM);
		break;
#endif
    case UI_WAP_MAIL_CMDS:     // Wap Mail (65490)
    case UI_WAP_PUSH_CMDS:     // Wap Push (65491)
#ifdef FEATURE_SKVM
    case UI_SKVM_LOAD_CMDS:		
#endif
#ifdef FEATURE_GVM	/* GVM 다운로드용 SMS (TI:63000) added by undersea 01.10.30 */
	case UI_GVM_DOWNLOADSMS_CMD_F:
#endif
    case UI_SMS_F:
#ifdef FEATURE_DS_UP
    #error code not present
#endif

#ifdef FEATURE_UASMS        /* Attempt to store new SMS message */
        fT1AskRequested = ( pCommand->sms.buf_ptr->data.sms_param_m & SMSI_BEARER_REPLY ) ? TRUE : FALSE;
        sto_status = (ui_sms_op_status_type)ui_sms_store_new_msg( pCommand );

        if( fT1AskRequested == TRUE )
        {
            /* ack the msg to the SMS API
            */
            if( sto_status == UI_SMS_OP_OK || sto_status == UI_SMS_OP_DUPLICATE )
            {
                (void) uasms_ack_message( pCommand->sms.tid,
                                        UASMS_ERROR_NONE,
                                        UASMS_OK_S );
            }
            else
            {
                (void) uasms_ack_message( pCommand->sms.tid,
                                        UASMS_ERROR_TEMP,
                                        UASMS_DESTINATION_RESOURCE_SHORTAGE_S );
            }
        } /* if tl ack is requested */

        /* Store was successful then */
        /* send SMS message event.   */
        /* Otherwise, indicate that  */
        /* SMS message was rejected. */
        if ( sto_status == UI_SMS_OP_OK )
        {
            //SKY_DoEvent( (word)UI_SMS_MSG_F );
            SKY_DoEvent(SE_SMS_MESSAGE);
#ifdef FEATURE_SKT_EIF //redstar  2001.02.13
            if( g_fSMSNewMsg )          //redstar 2001.02.13
            {
	            ds_cmd_type	ds_cmd_item;				// item to hold cmd to dsmgr
		        extern	rex_sigs_type dsi_requested_sig_mask;
                extern UINT8   g_nNewMsgTry;

                g_nNewMsgTry = 3;
		        ds_cmd_item.cmd_name = DS_EIF_MT_NEW;
		        ds_cmd(&ds_cmd_item);
		        dsi_requested_sig_mask |= DS_CMD_Q_SIG;
            }
#endif  //FEATURE_SKT_EIF
                
        }
        /* New message is a duplicate */
        /* toss message.              */
        else if ( sto_status == UI_SMS_OP_DUPLICATE )
        {
            ;
        }
        /* Store was unsuccessful, create */
        /* SMS status event indicating    */
        /* that the message was rejected. */
        else
        { 
            //SKY_DoEvent( (word)UI_SMS_STS_MSG_F );
            SKY_DoEvent(SE_SMS_STATUS_MESSAGE);
        }

#endif /* FEATURE_UASMS */
      break;

/* reo 2001.03.16 {-- */
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case UI_LOCATION_F:
        fT1AskRequested = ( pCommand->sms.buf_ptr->data.sms_param_m & SMSI_BEARER_REPLY ) ? TRUE : FALSE;

		sto_status = (ui_sms_op_status_type)SetRcvLocationSmsMsg( pCommand );
		SKY_TRACE(TL_HIGH, "UI_LOCATION_F ");

		/* Store was successful then */
		/* send SMS message event.   */
		if ( sto_status == UI_SMS_OP_OK )
		{
			SKY_DoEvent(SE_LOCATION_INFORM );
		}

        if( fT1AskRequested == TRUE )
        {
            /* ack the msg to the SMS API
            */
            (void) uasms_ack_message( pCommand->sms.tid,
                                    UASMS_ERROR_NONE,
                                    UASMS_OK_S );
        }
#endif //FEATURE_SKTT_EQUS_LOCATION
/* reo 2001.03.16 --} */

    case UI_INBAND_REV_BURST_DTMF_F:     /* Reverse burst DTMF from UI or another app */
        SKY_TRACE(TL_MED, "UI_INBAND_REV_BURST_DTMF_F ignored");
        break;

    case UI_INBAND_FWD_BURST_DTMF_F:     /* Forwared burst DTMF from the base station */
        ui_update_cm_inband_info(&(pCommand->inband_event.inband_info));
        //SKY_DoEvent(UI_INBAND_FWD_BURST_DTMF_E);
        SKY_DoEvent(SE_INBAND_FORWARD_BURST_DTMF);
        break;

    case UI_INBAND_REV_START_CONT_DTMF_F: /* Reverse start cont DTMF from UI or another app */
        SKY_TRACE(TL_MED, "UI_INBAND_REV_START_CONT_DTMF_F ignored");
        break;

    case UI_INBAND_REV_STOP_CONT_DTMF_F:  /* Reverse stop cont DTMF from UI or another app */
        SKY_TRACE(TL_MED, "UI_INBAND_REV_STOP_CONT_DTMF_F ignored");
        break;

#ifdef FEATURE_HANDLE_FWD_DTMF
    case UI_INBAND_FWD_STOP_CONT_DTMF_F: /* Forward stop cont DTMF from the base station */
        SKY_TRACE(TL_MED, "UI_INBAND_FWD_STOP_CONT_DTMF_F ignored");
        break;

    case UI_INBAND_FWD_START_CONT_DTMF_F: /* Forward start cont DTMF from the base station */
        SKY_TRACE(TL_MED, "UI_INBAND_FWD_START_CONT_DTMF_F ignored");
        break;

#endif /* FEATURE_HANDLE_FWD_DTMF */

#ifdef FEATURE_IS95B_NDSS
    case UI_CALL_NDSS_START_F:     /* NDSS start */
        SKY_TRACE(TL_MED, "UI_CALL_NDSS_START_F ignored");
        break;

    case UI_CALL_NDSS_CONNECT_F:     /* NDSS connect */
        SKY_TRACE(TL_MED, "UI_CALL_NDSS_CONNECT_F ignored");
        break;
#endif    
    // processing for the signal to ring from the base station
    // now only used for cdma ring, but later for both
    case UI_CALL_SIGNAL_F:
#ifdef FEATURE_IS95B_EXT_DISP   
//        if (fCallerIDComplete) 
//        {
//            ClearCLI();
//            fCallerIDComplete = FALSE;
//        }
#endif      
        if (ui_curr_call_allow_sound()) 
        {
            SET_EVENT_PARAM(*pCommand);
            //SKY_DoEvent((word)UI_CALL_INCOMING_E);
            SKY_DoEvent(SE_CALL_SIGNAL);

#if 0   // NOT Used!!!!!!
#ifdef FEATURE_PHONE_VR_HFK
#error code not present
#endif /* FEATURE_PHONE_VR_HFK */
            // If the signal is set to the default, use the user ringer settings  
            if ((pCommand->call_event.call_info.signal.signal_type == CAI_SIG_IS54B) &&
                (pCommand->call_event.call_info.signal.signal      == CAI_IS54B_L) &&
                (pCommand->call_event.call_info.signal.alert_pitch == CAI_PITCH_MED))
            {
                /* sky doesn't support roam ringer */
                //if (ui.roam_ringer && ui.ph_info.roam_status)
                if(SKY_GET_PHONE_INFO().roam_status)
                {
//                    SKY_PlaySound(UI_ALERT_SND, SND_ROAM_RING);
                    SKY_PlayAlertSignal();
                }
                else
                {
#ifdef FEATURE_MULTIPLE_RINGER_TYPES
#error code not present
#else
//                    SKY_PlaySound(UI_ALERT_SND, SND_ALERT);
                    SKY_PlayAlertSignal();
#endif /* FEATURE_MULTIPLE_RINGER_TYPES */
                }
            }
            else
            {
                SKY_PlaySignal(
                    (SIGNAL_TYPE_E)(pCommand->call_event.call_info.signal.signal_type),
                    (TONE_SIGNAL_E)(pCommand->call_event.call_info.signal.signal),
                    (PITCH_LEVEL_E)(pCommand->call_event.call_info.signal.alert_pitch)
                    );
                // in old UI, we turn on SNI if it is ISDN or IS-54B alert
                // we turn on SNI already while receiving CALL_INCOM
            }
#endif  
///////////////////////////////////////////////////////////////////////////////////////
//            End. Not used
///////////////////////////////////////////////////////////////////////////////////////
        }
#ifdef FEATURE_IS95B_EXT_DISP 
        /* Save whether it is the last caller information received for this OTA message.*/
        fCallerIDComplete = (pCommand->call_event.call_info.is_last_cdma_info_rec)? TRUE:FALSE;
#endif //FEATURE_IS95B_EXT_DISP
        break;

#ifdef FEATURE_DS_UP
#error code not present
#endif

    case UI_PH_MAINTREQ_F:           /* Maintenance requested */
        SKY_DoEvent(SE_PH_MAINTREQ);  
        
        /* 에쿠스 스케쥴러 내부가 아닌데, 메시지를 어떻게 표시할까?? */
        //(void) uiscrn_act ( UI_MAINT_REQ_F );  //redstar
        //(void) uiscrn_deact ( UI_BANNER_F );
        //uiscrn_update ();
        break;


    case UI_PH_STANDBY_SLEEP_F:      /* Entering low power sleep mode */

        /* TEMPORARY change, until deep sleep timing bug fixed. */
        /* Entering deep sleep causes system time to drift.  Flag this. */
        //ui.systime_valid = FALSE;

        /* deep sleep에 들어 갈 때, 화면 표시?? */

        /* Make sure we update service and RSSI indicators */
/*        
      lowerpower_txt_ptr = uiscrn_alloc_field( UI_LOWPOWER_F );
      if ( ui.earpieceup ) {      // earpiece up       
        (void) memcpy ( &lowerpower_txt_ptr[UI_WIDE],
                                 UI_EARUP_DEEPSLEEP_F, UI_WIDE*2 );
      }
      else {
        (void) memcpy ( &lowerpower_txt_ptr[UI_WIDE],
                                 UI_EARDN_DEEPSLEEP_F, UI_WIDE*2 );
      }
      (void) uiscrn_act ( UI_LOWPOWER_F );        
*/      
        
      break;

    case UI_PH_STANDBY_WAKE_F:       /* Exiting low power sleep mode */
        /* deep sleep에서 깨어 날 때, 화면 표시?? */
        
/*    
      (void) uiscrn_deact ( UI_LOWPOWER_F );
      uiscrn_update ();
*/
        break;


    case UI_CALL_INCOMING_F:     /* incoming call */
        sDefaultCallEventHandler(&(pCommand->call_event));
#ifdef FEATURE_OTASP_OTAPA 
        if (pCommand->call_event.call_info.call_type != CM_CALL_TYPE_OTAPA)
        {
            if(pCommand->call_event.call_info.call_type == CM_CALL_TYPE_SMS)    //added by reo: because of traffic channel sms delivery
            {                                                                   //2001.10.13
                SKY_AnswerSMSCall();
            } 
            else
            {
                SET_EVENT_PARAM(*pCommand);
                //SKY_DoEvent((word)UI_CALL_INCOMING_E);
                SKY_DoEvent(SE_CALL_INCOMING);
            }
        }
#else /*FEATURE_OTASP_OTAPA */
        if(pCommand->call_event.call_info.call_type == CM_CALL_TYPE_SMS) //added by reo: because of traffic channel sms delivery
        {                                                                //2001.10.13   
            SKY_AnswerSMSCall();
        }
        else
        {
            SET_EVENT_PARAM(*pCommand);
            //SKY_DoEvent((WORD)UI_CALL_INCOMING_E);
            SKY_DoEvent(SE_CALL_INCOMING);
        }
#endif /*FEATURE_OTASP_OTAPA */
        break;

    case UI_CALL_CALLER_ID_F:    /* caller id, could be call waiting */
        sDefaultCallEventHandler(&(pCommand->call_event));
#ifdef FEATURE_OTASP_OTAPA 
        if (pCommand->call_event.call_info.call_type != CM_CALL_TYPE_OTAPA)
#endif /* FEATURE_OTASP_OTAPA  */
        {
#ifdef FEATURE_IS95B_EXT_DISP 
//            if (fCallerIDComplete) 
//            {
//                ClearCLI();
//                fCallerIDComplete = FALSE;
//            }

            /*  Save whether it is the last caller information received for this OTA message.  */

            fCallerIDComplete = pCommand->call_event.call_info.is_last_cdma_info_rec? TRUE:FALSE;
#endif //FEATURE_IS95B_EXT_DISP

            SET_EVENT_PARAM( *pCommand );          
            //SKY_DoEvent( (word)UI_CALL_CALLER_ID_E );
            SKY_DoEvent(SE_CALL_CALLER_ID);
        }
        break;

    case UI_CALL_ORIGINATED_F:   /* a call is originated */
         sDefaultCallEventHandler(&(pCommand->call_event));
         //SKY_DoEvent((WORD)UI_TOIDLE_F);
         // jrkwon : I hate to go to the idle. SKY_DoEvent(SE_SYS_TO_IDLE);
         SET_EVENT_PARAM( *pCommand );
         //SKY_DoEvent((WORD)UI_CALL_ORIGINATED_E);
         SKY_DoEvent(SE_CALL_ORIGINATED);

         break;

    case UI_CALL_ANSWERED_F:     /* the call is answered */
        // some tasks answered the call
        // UI does not change state
        // the display will change with CALL_CONNECT
        sDefaultCallEventHandler(&(pCommand->call_event));
        break;

    case UI_CALL_CONNECT_F:      /* the call is connected */
        sDefaultCallEventHandler(&(pCommand->call_event));
#ifdef FEATURE_OTASP_OTAPA 
        if (pCommand->call_event.call_info.call_type != CM_CALL_TYPE_OTAPA)
        {
            //SKY_DoEvent((WORD)UI_TOIDLE_F);                
            // jrkwon : I hate to go to the idle. SKY_DoEvent(SE_SYS_TO_IDLE);
            SET_EVENT_PARAM( *pCommand );            
            //SKY_DoEvent((WORD)UI_CALL_CONNECT_E);
            SKY_DoEvent(SE_CALL_CONNECT);
        }
#else
        //SKY_DoEvent((WORD)UI_TOIDLE_F);             
        // jrkwon : I hate to go to the idle. SKY_DoEvent(SE_SYS_TO_IDLE);
        SET_EVENT_PARAM( *pCommand );        
        //SKY_DoEvent((WORD)UI_CALL_CONNECT_E);
        SKY_DoEvent(SE_CALL_CONNECT);
#endif /*FEATURE_OTASP_OTAPA */
        break;

    case UI_CALL_ENDED_F:        /* the call is ended */
        sDefaultCallEventHandler(&(pCommand->call_event));
#ifdef FEATURE_OTASP_OTAPA 
        if (pCommand->call_event.call_info.call_type != CM_CALL_TYPE_OTAPA)
#endif /* FEATURE_OTASP_OTAPA  */
        {
#ifdef FEATURE_IS95B_EXT_DISP   
            if (fCallerIDComplete) 
            {
//              ClearCLI();                  /* hjcho 2001.02.12 */
		        fCallerIDComplete = FALSE;
            }
#endif      
            if ( ISSMS(pCommand->call_event.call_info.call_type) ) 
            {
                // hacking for SMS option, will be resolved by Samir/Jason in the future
                // if (pCommand->call_event.call_info.end_status == CM_CALL_END_REL_NORMAL ||
                //     pCommand->call_event.call_info.end_status == CM_CALL_END_REL_SO_REJ){
                SKY_DoEvent(SE_CALL_RELEASED); //redstar
            }
            //SKY_DoEvent((WORD)UI_TOIDLE_F);
            // jrkwon : I hate to go to the idle. SKY_DoEvent(SE_SYS_TO_IDLE);
            SET_EVENT_PARAM( *pCommand );
            //SKY_DoEvent((WORD)UI_CALL_ENDED_E);
            SKY_DoEvent(SE_CALL_ENDED);
        }
        break;

    case UI_CALL_SO_CHANGED_F:   /* the service option is changed */
        sDefaultCallEventHandler(&(pCommand->call_event));
#ifdef FEATURE_OTASP_OTAPA 
        if (pCommand->call_event.call_info.call_type != CM_CALL_TYPE_OTAPA)
#endif /* FEATURE_OTASP_OTAPA  */
        {
            SET_EVENT_PARAM( *pCommand );
            //SKY_DoEvent((WORD)UI_CALL_SO_CHANGED_E);
            SKY_DoEvent(SE_CALL_SO_CHANGED);
        }
        break;

    case UI_CALL_PRIVACY_CHANGED_F:   /* the voice privacy is changed */
        sDefaultCallEventHandler(&(pCommand->call_event));
        sNotifyVoicePrivacyChange( &(pCommand->call_event.call_info) );        
        SET_EVENT_PARAM( *pCommand );
        //SKY_DoEvent((WORD)UI_CALL_PRIVACY_CHANGED_E);
        SKY_DoEvent(SE_CALL_PRIVACY_CHANGED);
        break;

    case UI_CALL_FLASHED_F:      /* a flash is sent */
        sDefaultCallEventHandler(&(pCommand->call_event));
#ifdef FEATURE_OTASP_OTAPA 
        if (pCommand->call_event.call_info.call_type != CM_CALL_TYPE_OTAPA)
#endif /* FEATURE_OTASP_OTAPA  */
        {
            //SKY_DoEvent((WORD)UI_TOIDLE_F);
            // jrkwon : I hate to go to the idle. SKY_DoEvent(SE_SYS_TO_IDLE);
            SET_EVENT_PARAM( *pCommand );
            //SKY_DoEvent((WORD)UI_CALL_FLASHED_E);
            SKY_DoEvent(SE_CALL_FLASHED);
        }
        break;

    case UI_CALL_CMD_ERR_F:      /* some error happened to call commands */
        SET_EVENT_PARAM( *pCommand );
        SKY_DoEvent(SE_CALL_COMMAND_ERROR);
        break;

    case UI_PH_INUSE_STATE_F:    /* inuse state is changed */
        /* set the phone information usig in SKY ui */
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );
        /* process the phone in use event */
        SKY_DoEvent(SE_PH_INUSE_STATE);
        /* display phone is in use by pCommand->ph_event.ph_info.is_in_use */
        break;

    case UI_PH_SRV_STATE_F:      /* service state is changed */
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );

        SET_EVENT_PARAM( *pCommand );
        /* ?? ph_info를 새로 업데이트 하기 전에 이벤트를 먼저 처리하면 안되는 거 아냐?? */
        //SKY_DoEvent((WORD)UI_PH_SRV_STATE_E);
        SKY_DoEvent(SE_PH_SERVICE_STATE);
        break;

    case UI_PH_OPRT_MODE_F:      /* operating mode is changed */
        // send this event to state machine, ignored in
        // most cases except LPM, now UI still depends on signal
        // for power off, reset, offline
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );

        SET_EVENT_PARAM( *pCommand );
        //SKY_DoEvent((WORD)UI_PH_OPRT_MODE_E);
        SKY_DoEvent(SE_PH_OPERATING_MODE);
        break;

    case UI_PH_CDMA_LOCK_MODE_F: /* CDMA lock mode is changed */
        // display "maintainence required" if locked
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );

        if (pCommand->ph_event.ph_info.cdma_lock_mode == CM_CDMA_LOCK_MODE_ON) 
        {
            SKY_DoEvent(SE_PH_MAINTREQ); //redstar
        } 
        else
        {
            SKY_DoEvent(SE_PH_UNLOCK); //woodstok
        }
        break;

    case UI_PH_MODE_PREF_F:      /* prefered mode is changed */
    case UI_PH_SYS_PREF_F:       /* prefered system is changed */
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );
        break;

    case UI_PH_ANSWER_VOICE_F:   /* answer voice as data setting is changed*/
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );
        sUpdateVoiceAsDataDisplay(pCommand->ph_event.ph_info.answer_voice);
        break;

    case UI_PH_RES_LEVEL_F:      /* restriction level is changed */
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );
        break;

    case UI_PH_CURR_NAM_F:       /* current NAM is changed */
    case UI_PH_NAM_SEL_F:        /* NAM selection is changed */     
        //SET_NP_CURRENTNAM( (BYTE))pCommand->ph_event.ph_info.curr_nam );
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );
        SKY_SET_CURRENT_NAM(pCommand->ph_event.ph_info.curr_nam );
        break;


    case UI_PH_ROAM_STATUS_F:    /* roaming status is changed */
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );
        SET_EVENT_PARAM( *pCommand );
        //SKY_DoEvent((WORD)UI_PH_ROAM_STATUE_E);
        SKY_DoEvent(SE_PH_ROAM_STATUS);
        break;

    case UI_PH_INFO_AVAIL_F:     /* Phone information is now available */
        ui_set_ph_info_avail(TRUE);
        // UI need to validate system pref and mode pref  
        sDefaultPhoneEventHandler( &(pCommand->ph_event) );
        sValidateSysPref();
        sValidateModePref();     
        
        SET_EVENT_PARAM( *pCommand );
        //SKY_DoEvent((WORD)UI_PH_INFO_AVAIL_E);
        SKY_DoEvent(SE_PH_INFO_AVAIL);
        break;

    case UI_PH_RSSI_F:   /* new RSSI level */
        // if we are here we know that there is definitely a RSSI 
        // level change greater or equal to the delta we subscribe for         
        SKY_SET_PHONE_INFO((&(pCommand->ph_event))->ph_info);
        // now call the function to update the RSSI icon
        //fast_update_rssi_icon( &(pCommand->ph_event) );  //redstar
        SKY_DoEvent(SE_PH_RSSI);
        break;

    case UI_PH_CMD_ERR_F:        /* some error happened to phone commands */
        // ignore phone command error for now
        break;

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
    case UI_BCSMS_CMD_ERR_F:
        // ignore Broadcast SMS command error for now
        break;

    case UI_BCSMS_CONFIG_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_CONFIG_F received");
        SET_BCSMS_CONFIG((&(pCommand->bcsms_event))->bcsms_info.bc_config);
        break;

    case UI_BCSMS_PREF_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_PREF_F received");
        SET_BCSMS_PREF((&(pCommand->bcsms_event))->bcsms_info.bc_pref );
        break;

    case UI_BCSMS_TABLE_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_TABLE_F received");
        GET_BCSMS_TABLE().size = (&(pCommand->bcsms_event))->bcsms_info.bc_table.size;
        for (i = 0; i < GET_BCSMS_TABLE().size; i++) 
        {
            GET_BCSMS_TABLE().entries[i].info = (&(pCommand->bcsms_event))->bcsms_info.bc_table.entries[i];
            GET_BCSMS_TABLE().entries[i].active = TRUE;
        }  
        break;

    case UI_BCSMS_SRV_IDS_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_SRV_IDS_F received");
        for (i=0; i<(&(pCommand->bcsms_event))->bcsms_info.bc_srv_ids.size; i++) 
        {
            SKY_TRACE1(TL_MED, "service:  %d",(&(pCommand->bcsms_event))->bcsms_info.bc_srv_ids.entries[i].service);
            SKY_TRACE1(TL_MED, "language: %d",(&(pCommand->bcsms_event))->bcsms_info.bc_srv_ids.entries[i].language);
        }
        break;

    case UI_BCSMS_SRV_INFO_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_SRV_INFO_F received");
        SKY_TRACE1(TL_MED, "service:  %d",(&(pCommand->bcsms_event))->bcsms_info.bc_srv_info.srv_id.service);
        SKY_TRACE1(TL_MED, "language: %d",(&(pCommand->bcsms_event))->bcsms_info.bc_srv_info.srv_id.language);
        SKY_TRACE1(TL_MED, "selected: %d",(&(pCommand->bcsms_event))->bcsms_info.bc_srv_info.selected);
        SKY_TRACE1(TL_MED, "priority: %d",(&(pCommand->bcsms_event))->bcsms_info.bc_srv_info.priority);
        break;

    case UI_BCSMS_SRV_ADDED_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_SRV_ADDED_F received");

        for (i=0; i<GET_BCSMS_TABLE().size; i++) 
        {
            if (GET_BCSMS_TABLE().entries[i].info.srv_id.service == (&(pCommand->bcsms_event))->bcsms_info.bc_deleted_srv.service && 
                GET_BCSMS_TABLE().entries[i].info.srv_id.language == (&(pCommand->bcsms_event))->bcsms_info.bc_deleted_srv.language ) 
            {
                GET_BCSMS_TABLE().entries[i].info = (&(pCommand->bcsms_event))->bcsms_info.bc_srv_info;
                GET_BCSMS_TABLE().entries[i].active = TRUE;
                break;
            }
        }   
        break;

    case UI_BCSMS_SRV_DELETED_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_SRV_DELETED_F received");
        for (i=0; i<GET_BCSMS_TABLE().size; i++) 
        {
            if (GET_BCSMS_TABLE().entries[i].info.srv_id.service == (&(pCommand->bcsms_event))->bcsms_info.bc_deleted_srv.service && 
                GET_BCSMS_TABLE().entries[i].info.srv_id.language == (&(pCommand->bcsms_event))->bcsms_info.bc_deleted_srv.language ) 
            {
                GET_BCSMS_TABLE().entries[i].active = FALSE;
                break;
            }
        }      
        break;

    case UI_BCSMS_SRV_UPDATED_F:
        SKY_TRACE(TL_MED, "UI_BCSMS_SRV_UPDATED_F received");
        for (i=0; i<GET_BCSMS_TABLE().size; i++) 
        {
            if (GET_BCSMS_TABLE().entries[i].info.srv_id.service == (&(pCommand->bcsms_event))->bcsms_info.bc_srv_info.srv_id.service && 
                GET_BCSMS_TABLE().entries[i].info.srv_id.language == (&(pCommand->bcsms_event))->bcsms_info.bc_srv_info.srv_id.language ) 
            {
                GET_BCSMS_TABLE().entries[i].info = (&(pCommand->bcsms_event))->bcsms_info.bc_srv_info;
                break;
            }
        }      
        break;

    case UI_BCSMS_ENABLE_FAILURE_F:  /* lower layer (CM & CP) failure */
        SKY_TRACE(TL_MED, "UI_BCSMS_ENABLE_FAILURE_F received");
        break;

    case UI_BCSMS_DISABLE_FAILURE_F: /* lower layer (CM & CP) failure */
        SKY_TRACE(TL_MED, "UI_BCSMS_DISABLE_FAILURE_F received");
        break;

#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

    case UI_SS_SRV_CHANGED_F:       /* Serving system is changed */
          break;

    case UI_DATACALL_F:  /* Tossed it way now for DS testing,
                           Remove it when CP disabled this message */
        SKY_DoEvent(SE_DATA_CALL); 
          break;

	case UI_NDSS_RECONNECT_F:
		SKY_DoEvent(SE_NDSS_RECONNECT); 
		break;

    // addition for CM2.0
    case UI_CALL_CALLED_PARTY_F:       /* Just ignore for now  */
	    //sSetUpCalledPartyNumber(pCommand);
        SET_EVENT_PARAM( *pCommand );
		SKY_DoEvent(SE_CALL_CALLED_PARTY);
        break;

    // addition for CM2.0
    case UI_CALL_CONNECTED_NUM_F:       /* Serving system is changed */
        SKY_TRACE(TL_MED, "UI_CALL_CONNECTED_NUM_F ignored");
        break;

    case UI_INBAND_CMD_ERR_F: /* some error happend to inband commands */
        /* If the command was not sent to CM due to certain errors, the
        return status is handled else where right after inband commands */
        break;

    case UI_RECVDATA_F:
        {
            SKY_DoEvent(SE_RECV_DATA);
            break;
        }

#ifdef FEATURE_GVM
    case UI_PTP_CONNECT_SUCCESS_F:
        {
            SKY_DoEvent(SE_PTP_CONNECT_SUCCESS);
            break;
        }
    case UI_PTP_CONNECT_FAIL_F:
        {
            SKY_DoEvent(SE_PTP_CONNECT_FAIL);
            break;
        }
    case UI_PTP_RELEASED_F:
        {
            SKY_DoEvent(SE_PTP_RELEASED);
            break;
        }
	
	case UI_SOCK_TCPCONNECT_SUCCESS_F:
		{
			SKY_DoEvent(SE_TCPCONNECT_SUCCESS);
			break;
		}

    case UI_SOCK_TCPCONNECT_FAIL_F:
		{
			SKY_DoEvent(SE_TCPCONNECT_FAIL);
			break;
		}
    case UI_SOCK_RELEASED_F:
		{
			SKY_DoEvent(SE_SOCK_RELEASED);
			break;
		}
    case UI_TONETGAME_F:
        {
			SKY_DoEvent(SE_TONETGAME);
			break;
        }
#endif //FEATURE_GVM

#ifdef FEATURE_SKTT_PM
    case UI_PHOTOMAIL_F:
        {
			SKY_DoEvent(SE_PHOTOMAIL);
			break;
        }
#endif//pm
	case UI_DIAG_KEYPAD_TEST_F:
		{
			if(pCommand->diagkeytest.mode)
			{
				SKY_SET_DIAG_KEYPAD_TEST(TRUE);
			}
			else
			{
				SKY_SET_DIAG_KEYPAD_TEST(FALSE);
			}
			SKY_DoEvent(SE_KEYPAD_TEST);
			break;
		}

#ifdef FEATURE_USE_IRDA
#ifndef FEATURE_IRDA_SIGNAL
	case UI_IRDA_EX_RX_DATA_CMD:
        {
		    SKY_DoEvent(SE_IRDA_EX_RX_DATA_F);
		    break;
        }
#endif
#endif
    case UI_DM_MO_CMD_F:
        {
            SKY_DoEvent(SE_DM_MO_CMD_F);
            break;
        }

    case UI_DTR_DISPLAY_F: // white 2001.03.20
		SKY_SET_DTR_DISP(pCommand->dtr_disp.mode);
		SKY_DoEvent(SE_DTR_DISPLAY_CHANGE_F);
		break;


#ifdef FEATURE_MULTI_SID
    case UI_MULTI_SID_F:
        SKY_DoEvent(SE_SYS_MULTI_SID);
        break;
#endif //FEATUER_MULTI_SID

#ifdef FEATURE_SKT_EIF
    case UI_EIF_CMD_F:
        SKY_DoEvent(EIF_Event);
        break;
#endif //FEATURE_SKT_EIF

#ifdef FEATURE_UI_DORMANT
	case UI_PKT_DORM_F:
		SKY_DoEvent(SE_DATA_DORM_F);
		break;

	case UI_PKT_IDLE_F:
		SKY_DoEvent(SE_DATA_IDLE_F);
		break;
#endif
#ifdef FEATURE_SKVM
    case UI_TO_SKVM_F:
        {
			SKY_DoEvent(SE_TOSKVM);
			break;
        }
#endif //FEATURE_SKVM
#ifdef FEATURE_WAP_WAVELET
    case UI_TOWAVELET_F:
        {
			SKY_DoEvent(SE_TOWAVELET);
			break;
        }
#endif //FEATURE_WAP_WAVELET

    case UI_DM_UPDATE_IDLE_F:         //white 2001.06.12 for DataManager Schedule
        SKY_DoEvent(SE_DM_UPDATE_IDLE);
        break;

    case UI_DM_IN_PROCESSING_F:         //white 2001.06.12 for DataManager IDLE
        SKY_DoEvent(SE_DM_IN_PROCESSING);
        break;

    case UI_DM_OUT_PROCESSING_F:         //white 2001.06.12 for DataManager IDLE
        SKY_DoEvent(SE_DM_OUT_PROCESSING);
        break;

#ifdef FEATURE_VAS
    case UI_NET_CONNECTED_F:
	     SKY_DoEvent(SE_NET_CONNECTED);
	     break;
	
    case UI_NET_DISCONNECTED_F:
		 SKY_DoEvent(SE_NET_DISCONNECTED);
		 break;

    case UI_SUCCESS_F:
		SKY_DoEvent(SE_SUCCESS_F);
		break;

	case UI_FAIL_F:
		SKY_DoEvent(SE_FAIL_F);
		break;

#ifdef FEATURE_WAP

	case WAP_ENTER_CMD:
		wap_enter_cmd();
		break;

	case WAP_EXIT_CMD:
		wap_exit_cmd();
		break;

	case WAP_CONNECTION_DONE_CMD:
		wap_connection_done_cmd(pCommand->Command.objectID, 
			                    pCommand->Command.channelID,     
								pCommand->Command.value);
		break;

#endif //#ifdef FEATURE_WAP
	case WAP_KEY_CMD:
		wap_key_cmd(pCommand->KeyInput.KeyCode);
		break;

	case WAP_GOTOURL_CMD:
		wap_gotourl_cmd(pCommand->GotoUrl.GotoUrl);
		break;

//for test
	case VAS_PPPOPEN_TEST_CMD:
		vas_pppopen_test_cmd();
		break;
	case VAS_UDPTXRX_CMD:
		vas_udptxrx_cmd();
		break;
	case VAS_TCPTXRX_CMD:
		vas_tcptxrx_cmd();
		break;
	case VAS_TXSEND_CMD:
		vas_txsend_cmd();
		break;

	case VAS_RXDATA_CMD:
		vas_rxdata_cmd();
		break;
	case VAS_MULTISOCKET_CMD:
		vas_multisocket_cmd();
		break;
//for test
	case VAS_RELEASE_CMD:
		vas_release_cmd();
		break;
#endif//feature_vas

#ifdef FEATURE_WAP
    /*========================================================================
    ** ytchoi 01/07/16
	** AU4.3 Event  Now according to #define, enum name may conflict!!
    */ 
    case UI_WAP_REPORT_ERROR_F:
		{
			SKY_DoEvent(SE_WAP_REPORTERROR);
			break;
		}

    case UI_MUXNEGO_SUCCESS_F:
		{
			SKY_DoEvent(SE_MUXNEGO_SUCCESS);
			break;
		}

	#if defined (WAP95C_TEST) || defined (FEATURE_WAP_MUXOPTION)
	case UI_WAP_SO_REQ_F:
		{
			SKY_DoEvent(SE_WAP_SO_REQ);
			break;
		}  
	#endif //#if defined (WAP95C_TEST) || defined (FEATURE_WAP_MUXOPTION)

#endif //#ifdef FEAUTE_WAP

#ifdef FEATURE_WAVELET // yoursun 01/06/08
	case UI_WAV_PLAY_CMD_F:
		SKY_DoEvent(SE_WAV_PLAY);
		break;
	case UI_WAV_STREAM_CMD_F:
		SKY_DoEvent(SE_WAV_STREAM);
		break;
#endif //#ifdef FEATURE_WAVELET

    default:
        SKY_ERROR1("Bad UI_CMD 0x%x", pCommand->hdr.cmd);
        //SKY_ASSERT(FALSE);      /* error fatal if in debug mode */
        break;
    } /* switch( pCommand->hdr.cmd ) */
}
