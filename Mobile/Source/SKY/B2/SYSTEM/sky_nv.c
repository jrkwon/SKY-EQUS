/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_nv.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-10-30 jrkwon     Created.
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "nv.h"

#include "sky.h"
#include "ska_vcmm.h"

#include "nvimio.h"
#include "nvimw.h"
#include "dsi.h"

extern char *strcpy( char *strDestination, const char *strSource );
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define MAX_NAM_NAME_LENGTH     16

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
extern void voice_erase_all_when_nv_setup(void);
/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL NV_COMMAND_T  nvi;            /* nv command buffer */

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
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

/*===========================================================================
FUNCTION UI_WAIT_NV

DESCRIPTION
  Wait on NV actions.  Handle signals as they come in just in case NV
  takes a while.  Only time critical watchdog and sound signals are
  acknowledged.

DEPENDENCIES
  There is a command sent to NV, and this task will be signalled.  There
  are no other outstanding NV requests from this task.  Called from the
  state machine, so make no state machine calls here!

RETURN VALUE
  None

SIDE EFFECT
  Some signals are ignored till we're done with NV

===========================================================================*/
LOCAL void SKY_API sWaitNV(void)
{
    REX_SIGNAL Signal;             /* hold signals from SKY_Wait_Signal() */

    /* until signal back from NV */
    while(TRUE)
    {
        /* 키 톤은 항상 보통 길이로 난다. */
        Signal = SKY_Wait_Signal(
            UI_NV_SIG | UI_RPT_TIMER_SIG);

        /* Need to kick the watchdog */
        if( Signal & UI_RPT_TIMER_SIG ) 
        {
            SKY_KickDog();
        }

        /* The NV is done.  Exit this routine. */
        if( Signal & UI_NV_SIG ) {
            return;
        }
    }
}

/*===========================================================================
FUNCTION UI_GET_NV

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the UI task.
===========================================================================*/

NV_STATE_E SKY_API SKY_GetNV(NV_ITEM_E Item, NV_ITEM_U *pData)
{
#ifdef WIN32
    int i;
	BYTE bIndex;

    nvi.tcb_ptr = rex_self();          /* notify this task when done */
    nvi.sigs = UI_NV_SIG;
    nvi.done_q_ptr = NULL;             /* command goes on no queue when done */

    nvi.item = Item;                 /* item to read */
    nvi.cmd = NV_READ_F;

    /* Set up NV so that it will read the data into the correct location */
    nvi.data_ptr = pData;

    /* Clear the return signal, call NV, and wait for a response */
    SKY_Clear_Signal( rex_self(), UI_NV_SIG );
    
    nv_cmd( &nvi );
    
    sWaitNV();

    nvi.status = NV_DONE_S;

    /* provide our own values */
    switch( nvi.item ) 
    {           
#if defined( FEATURE_UI_FORCE_CALL ) || defined( FEATURE_UI_FORCE_MODE_SEL )
    case NV_PREF_MODE_I:
        /* Default the mode to AUTOMATIC.
        */
        pData->pref_mode.mode = NV_MODE_AUTOMATIC;
        SKY_PutNV(NV_PREF_MODE_I, pData);
        break;

    case NV_TIMED_PREF_MODE_I:
        /* Default so that mode is reset to CDMA_PREF.
        */
        pData->timed_pref_mode = TRUE;
        break;
#endif /* FEATURE_UI_FORCE_CALL || FEATURE_UI_FORCE_MODE_SEL ) */

#ifdef FEATURE_UI_SEND_PIN
    case NV_SEND_PIN_I:        /* Send PIN code */
        pData->send_pin = FALSE;
        break;
#endif

    case NV_AUTO_NAM_I:      /* Auto NAM mode */
        pData->auto_nam = TRUE;
        break;

    case NV_DIR_NUMBER_I:      /* Mobile Directory Number */
        sprintf(pData->dir_number.dir_number, "0112890012", NV_DIR_NUMB_SIZ);
        break;

#ifdef FEATURE_IS95B_TRUE_IMSI   
    case NV_IMSI_T_11_12_I:      /* IMSI Mobile Network Code (IMSI_11_12) */
    case NV_IMSI_T_MCC_I:        /* IMSI Mobile Country Code */
    case NV_IMSI_T_S1_I:
    case NV_IMSI_T_S2_I:
    case NV_IMSI_T_ADDR_NUM_I:
        nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
        break;
#endif /* FEATURE_IS95B_TRUE_IMSI */


#ifdef FEATURE_CDMA_1900
    case NV_IMSI_11_12_I:      /* IMSI Mobile Network Code (IMSI_11_12) */
    case NV_IMSI_MCC_I:        /* IMSI Mobile Country Code */
    case NV_MIN1_I:
    case NV_MIN2_I:
    case NV_IMSI_ADDR_NUM_I:
        nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
        break ;
#else /* !def FEATURE_CDMA_1900 */
    case NV_IMSI_MCC_I:        /* IMSI Mobile Country Code */
        pData->imsi_mcc.imsi_mcc = IMSI_MCC_0;
        SKY_PutNV(NV_IMSI_MCC_I, pData);
        break;

    case NV_IMSI_11_12_I:      /* IMSI Mobile Network Code (IMSI_11_12) */
        pData->imsi_11_12.imsi_11_12 = IMSI_11_12_0;
        SKY_PutNV(NV_IMSI_11_12_I, pData);
        break;
#endif /* !def FEATURE_CDMA_1900 */

    case NV_FSC_I:             /* Field Service Lock code */
        for( i=0; i<NV_FSC_SIZ; i++ ) {
          pData->fsc.fsc[i] = '0'; /* Field Service code */
        }
        SKY_PutNV( NV_FSC_I, pData );
        break;

    case NV_CURR_NAM_I:         /* Current NAM setting */
        pData->curr_nam = 0;   /* write it to nv, if never set */
        SKY_PutNV( NV_CURR_NAM_I, pData);
        break;

    case NV_CONFIG_CHKSUM_I:    /* Configuration checksum */
        pData->config_chksum.chksum = 0; /* 0 = bad checksum default */
        break;

    case NV_NAME_NAM_I:
        memset((void *) pData->name_nam.name, ' ', MAX_NAM_NAME_LENGTH);//UI_WIDE );
        break;

    case NV_SPARE_2_I:
        pData->spare_2 = 0;
        break;

    case NV_MAX_TX_ADJ_I:
        pData->max_tx_adj = 0;
        break;
    case NV_INI_MUTE_I:
        pData->init_mute = NV_MUTE_NORM;
        break;

    case NV_DM_ADDR_I:
        pData->dm_addr = 1;
        break;

    case NV_FACTORY_INFO_I:
        memcpy((void *) pData->fact_info, (BYTE*)"No Entry", 9 );
        break;

    case NV_WDC_I:              /* Warranty Date Code */
        memset((void *) pData->wdc.wdc, ' ', NV_WDC_LTRS );
        break;

#ifdef FEATURE_ACP
   case NV_ANALOG_HOME_SID_I:
        nvi.status = NV_NOTACTIVE_S ;  /* will be initialized separately */
        break;
#endif /* FEATURE_ACP */

#ifdef FEATURE_CDMA_800
    case NV_PCDMACH_I:
    case NV_SCDMACH_I:
        nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
        break;
#endif /* FEATURE_CDMA_800 */

#ifdef FEATURE_SSPR_ENHANCEMENTS
    case NV_HOME_SID_NID_I:
        nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
        break;
#else
    case NV_SID_NID_I:
        nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
        break;
#endif

#if defined(FEATURE_SSPR_ENHANCEMENTS) || defined(FEATURE_PRL_FORCE_MODE) || \
    defined(FEATURE_E911) || defined(FEATURE_HOME_ONLY)

    case NV_SYSTEM_PREF_I:
        /* Default the preferred system to NV_SP_STANDARD.
        */
        pData->system_pref.sys = NV_SP_STANDARD;
        SKY_PutNV(NV_SYSTEM_PREF_I, pData);
        break;
#endif

#ifdef FEATURE_SSPR_ENHANCEMENTS
    case NV_SID_NID_LOCK_I:
        nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
        break;

#else
#ifdef FEATURE_ACP
#ifdef FEATURE_CDMA_800
    case NV_CDMA_PREF_SERV_I:
    case NV_ANALOG_PREF_SERV_I:
        nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
        break;
#endif /* FEATURE_CDMA_800 */
#endif /* FEATURE_ACP */
#endif /* FEATURE_SSPR_ENHANCEMENTS */

#ifdef FEATURE_SSPR_800
  case NV_PRL_ENABLED_I:
      nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
      break;
#endif /* FEATURE_SSPR_800 */

    case NV_FN_SEQUENCE_I:
		for(bIndex = 0; i <NV_MAX_FL_FILE_NUM; bIndex++)
        {
		  pData->Fn_sequence.address = bIndex;
		  pData->Fn_sequence.Fn = NV_MAX_FL_FILE_NUM;
		  pData->Fn_sequence.Type = 0; // VTYPE_NONE
		  pData->Fn_sequence.Time = 0;
        }
	  break;

   case NV_VOICE_INFO_I:
        pData->voice_info.total_rec_num = 0;
		pData->voice_info.new_ans_num = 0;;
		pData->voice_info.is_user_voice = FALSE;
		pData->voice_info.user_voice_fn = NV_MAX_FL_FILE_NUM;
		pData->voice_info.user_voice_l = 0;; 

		pData->voice_info.remained_byte = TOTAL_REC_BYTE; // 64K 2.5블럭 사이즈 
		pData->voice_info.remained_time = TOTAL_REC_TIME;		 // 2 minute by jnlee 2000.1.15
		voice_erase_all_when_nv_setup();		// inserted by jnlee 2000.1.20
      break;


  case NV_FLIP_COUNT_I:      //woodstok 01.5.3
      pData->flip_count = 0;   
      break;

  case NV_LCD_I:
      pData->lcd = LCD_CONTRAST_DEFAULT;
      break;

  case NV_BACK_LIGHT_I:
	  pData->back_light = SUBLCD_CONTRAST_DEFAULT;
	  break;

  case NV_AUTO_MUTE_I://happykid 01.5.11
		pData->auto_mute = FALSE;
		break;

    /* If it's an unknown item, print an info message and pass on the */
    /* NV_NOTACTIVE_S status. */
    default:
        SKY_TRACE1(TL_HIGH, "Nonactive NV item 0x%x", nvi.item);
        nvi.status = NV_NOTACTIVE_S;
        break;
    }

#else /* ifndef WIN32 */

    int i;
	BYTE bIndex;

    nvi.tcb_ptr = rex_self();          /* notify this task when done */
    nvi.sigs = UI_NV_SIG;
    nvi.done_q_ptr = NULL;             /* command goes on no queue when done */

    nvi.item = Item;                 /* item to read */
    nvi.cmd = NV_READ_F;

    /* Set up NV so that it will read the data into the correct location */
    nvi.data_ptr = pData;

    /* Clear the return signal, call NV, and wait for a response */
    SKY_Clear_Signal( rex_self(), UI_NV_SIG );
    
    nv_cmd( &nvi );
    
    sWaitNV();

    /* Handle a NV_NOTACTIVE_S or NV_FAIL_S status internally by replacing
    ** the random data returned with a default value of our own.  Items that
    ** share the same structure are lumped together in the switch.
    */
    if( nvi.status == NV_NOTACTIVE_S || nvi.status == NV_FAIL_S ) 
    {
        nvi.status = NV_DONE_S;

        /* provide our own values */
        switch( nvi.item ) 
        {           
#if defined( FEATURE_UI_FORCE_CALL ) || defined( FEATURE_UI_FORCE_MODE_SEL )
        case NV_PREF_MODE_I:
            /* Default the mode to AUTOMATIC.
            */
            pData->pref_mode.mode = NV_MODE_AUTOMATIC;
            SKY_PutNV(NV_PREF_MODE_I, pData);
            break;

        case NV_TIMED_PREF_MODE_I:
            /* Default so that mode is reset to CDMA_PREF.
            */
            pData->timed_pref_mode = TRUE;
            break;
#endif /* FEATURE_UI_FORCE_CALL || FEATURE_UI_FORCE_MODE_SEL ) */

#ifdef FEATURE_UI_SEND_PIN
        case NV_SEND_PIN_I:        /* Send PIN code */
            pData->send_pin = FALSE;
            break;
#endif

        case NV_AUTO_NAM_I:      /* Auto NAM mode */
            pData->auto_nam = TRUE;
            break;

        case NV_DIR_NUMBER_I:      /* Mobile Directory Number */
            for( i=0; i<NV_DIR_NUMB_SIZ; i++ ) {
              pData->dir_number.dir_number[i] = '0'; /* no number */
            }
            break;

#ifdef FEATURE_IS95B_TRUE_IMSI   
        case NV_IMSI_T_11_12_I:      /* IMSI Mobile Network Code (IMSI_11_12) */
        case NV_IMSI_T_MCC_I:        /* IMSI Mobile Country Code */
        case NV_IMSI_T_S1_I:
        case NV_IMSI_T_S2_I:
        case NV_IMSI_T_ADDR_NUM_I:
            nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
            break;
#endif /* FEATURE_IS95B_TRUE_IMSI */


#ifdef FEATURE_CDMA_1900
        case NV_IMSI_11_12_I:      /* IMSI Mobile Network Code (IMSI_11_12) */
        case NV_IMSI_MCC_I:        /* IMSI Mobile Country Code */
        case NV_MIN1_I:
        case NV_MIN2_I:
        case NV_IMSI_ADDR_NUM_I:
            nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
            break ;
#else /* !def FEATURE_CDMA_1900 */
        case NV_IMSI_MCC_I:        /* IMSI Mobile Country Code */
            pData->imsi_mcc.imsi_mcc = IMSI_MCC_0;
            SKY_PutNV(NV_IMSI_MCC_I, pData);
            break;

        case NV_IMSI_11_12_I:      /* IMSI Mobile Network Code (IMSI_11_12) */
            pData->imsi_11_12.imsi_11_12 = IMSI_11_12_0;
            SKY_PutNV(NV_IMSI_11_12_I, pData);
            break;
#endif /* !def FEATURE_CDMA_1900 */

        case NV_FSC_I:             /* Field Service Lock code */
            for( i=0; i<NV_FSC_SIZ; i++ ) {
              pData->fsc.fsc[i] = '0'; /* Field Service code */
            }
            SKY_PutNV( NV_FSC_I, pData );
            break;

        case NV_CURR_NAM_I:         /* Current NAM setting */
            pData->curr_nam = 0;   /* write it to nv, if never set */
            SKY_PutNV( NV_CURR_NAM_I, pData);
            break;

        case NV_CONFIG_CHKSUM_I:    /* Configuration checksum */
            pData->config_chksum.chksum = 0; /* 0 = bad checksum default */
            break;

        case NV_NAME_NAM_I:
            memset((void *) pData->name_nam.name, ' ', MAX_NAM_NAME_LENGTH);//UI_WIDE );
            break;

        case NV_SPARE_2_I:
            pData->spare_2 = 0;
            break;

        case NV_MAX_TX_ADJ_I:
            pData->max_tx_adj = 0;
            break;
        case NV_INI_MUTE_I:
            pData->init_mute = NV_MUTE_NORM;
            break;

        case NV_DM_ADDR_I:
            pData->dm_addr = 1;
            break;

        case NV_FACTORY_INFO_I:
            memcpy((void *) pData->fact_info, (BYTE*)"No Entry", 9 );
            break;

        case NV_WDC_I:              /* Warranty Date Code */
            memset((void *) pData->wdc.wdc, ' ', NV_WDC_LTRS );
            break;

#ifdef FEATURE_ACP
       case NV_ANALOG_HOME_SID_I:
            nvi.status = NV_NOTACTIVE_S ;  /* will be initialized separately */
            break;
#endif /* FEATURE_ACP */

#ifdef FEATURE_CDMA_800
        case NV_PCDMACH_I:
        case NV_SCDMACH_I:
            nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
            break;
#endif /* FEATURE_CDMA_800 */

#ifdef FEATURE_SSPR_ENHANCEMENTS
        case NV_HOME_SID_NID_I:
            nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
            break;
#else
        case NV_SID_NID_I:
            nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
            break;
#endif

#if defined(FEATURE_SSPR_ENHANCEMENTS) || defined(FEATURE_PRL_FORCE_MODE) || \
        defined(FEATURE_E911) || defined(FEATURE_HOME_ONLY)

        case NV_SYSTEM_PREF_I:
            /* Default the preferred system to NV_SP_STANDARD.
            */
            pData->system_pref.sys = NV_SP_STANDARD;
            SKY_PutNV(NV_SYSTEM_PREF_I, pData);
            break;
#endif

#ifdef FEATURE_SSPR_ENHANCEMENTS
        case NV_SID_NID_LOCK_I:
            nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
            break;

#else
#ifdef FEATURE_ACP
#ifdef FEATURE_CDMA_800
        case NV_CDMA_PREF_SERV_I:
        case NV_ANALOG_PREF_SERV_I:
            nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
            break;
#endif /* FEATURE_CDMA_800 */
#endif /* FEATURE_ACP */
#endif /* FEATURE_SSPR_ENHANCEMENTS */

#ifdef FEATURE_SSPR_800
      case NV_PRL_ENABLED_I:
          nvi.status = NV_NOTACTIVE_S ; /* will be initialized separately */
          break;
#endif /* FEATURE_SSPR_800 */

        case NV_FN_SEQUENCE_I:
		    for(bIndex = 0; i <NV_MAX_FL_FILE_NUM; bIndex++)
            {
		      pData->Fn_sequence.address = bIndex;
		      pData->Fn_sequence.Fn = NV_MAX_FL_FILE_NUM;
		      pData->Fn_sequence.Type = 0; // VTYPE_NONE
		      pData->Fn_sequence.Time = 0;
            }
		  break;

       case NV_VOICE_INFO_I:
            pData->voice_info.total_rec_num = 0;
		    pData->voice_info.new_ans_num = 0;;
		    pData->voice_info.is_user_voice = FALSE;
		    pData->voice_info.user_voice_fn = NV_MAX_FL_FILE_NUM;
		    pData->voice_info.user_voice_l = 0;; 

		    pData->voice_info.remained_byte = TOTAL_REC_BYTE; // 64K 2.5블럭 사이즈 
		    pData->voice_info.remained_time = TOTAL_REC_TIME;		 // 2 minute by jnlee 2000.1.15
		    voice_erase_all_when_nv_setup();		// inserted by jnlee 2000.1.20
          break;


      case NV_FLIP_COUNT_I:      //woodstok 01.5.3
          pData->flip_count = 0;   
          break;

      case NV_LCD_I:
          pData->lcd = LCD_CONTRAST_DEFAULT;
          break;

      case NV_BACK_LIGHT_I:
		  pData->back_light = SUBLCD_CONTRAST_DEFAULT;
		  break;

      case NV_AUTO_MUTE_I://happykid 01.5.11
			pData->auto_mute = FALSE;
			break;

        /* If it's an unknown item, print an info message and pass on the */
        /* NV_NOTACTIVE_S status. */
        default:
            SKY_TRACE1(TL_HIGH, "Nonactive NV item 0x%x", nvi.item);
            nvi.status = NV_NOTACTIVE_S;
            break;
        }
    } 
    else 
    {
        if( nvi.status != NV_DONE_S ) 
        {
            SKY_ERROR2( "NV Read Failed Item %d Code %d", nvi.item, nvi.status);
        }
		//Added by happykid in 2001.05.08
		if(nvi.item == NV_LCD_I 
			&& ((pData->lcd < LCD_CONTRAST_MIN) || (pData->lcd > LCD_CONTRAST_MAX)))
			pData->lcd = LCD_CONTRAST_DEFAULT;

    }

#endif /* ifndef WIN32 */
    return( nvi.status );
}



/*===========================================================================
FUNCTION UI_GET_NV_FOR_CM

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.
  
  This function is defined for the purpose of supporting two UI callback
  functions: get_call_type, call_is_allowed that are used by the CM to
  determine the call type and whether the call is allowed. This function
  must be completed in a specified time as defined by the CM API.
  
  This is an example for using the call back functions.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the UI task.
===========================================================================*/
NV_STATE_E SKY_API SKY_GetNV4CM(NV_ITEM_E Item, NV_ITEM_U *pData)
{
    int i;

    nvi.tcb_ptr = rex_self();          /* notify this task when done */
    nvi.sigs = CM_NV_SIG;
    nvi.done_q_ptr = NULL;             /* command goes on no queue when done */

    nvi.item = Item;                 /* item to read */
    nvi.cmd = NV_READ_F;

    /* Set up NV so that it will read the data into the correct location */
    nvi.data_ptr = pData;


    /* Clear the return signal, call NV, and wait for a response */
    (void) SKY_Clear_Signal( rex_self(), CM_NV_SIG );
    nv_cmd( &nvi );
    SKY_Wait_Signal( CM_NV_SIG );

    /* Handle a NV_NOTACTIVE_S or NV_FAIL_S status internally by replacing
    ** the random data returned with a default value of our own.  Items that
    ** share the same structure are lumped together in the switch.
    */
    if( nvi.status == NV_NOTACTIVE_S || nvi.status == NV_FAIL_S ) 
    {
        nvi.status = NV_DONE_S;

        /* provide our own values */
        switch( nvi.item ) 
        {           
        case NV_AUTO_NAM_I:      /* Auto NAM mode */
            pData->auto_nam = TRUE;
            break;

        case NV_DIR_NUMBER_I:      /* Mobile Directory Number */
            for( i=0; i<NV_DIR_NUMB_SIZ; i++ ) {
              pData->dir_number.dir_number[i] = '0'; /* no number */
            }
            break;

        case NV_FSC_I:             /* Field Service Lock code */
            for( i=0; i<NV_FSC_SIZ; i++ ) {
              pData->fsc.fsc[i] = '0'; /* Field Service code */
            }
            SKY_PutNV(NV_FSC_I, pData );
            break;

        case NV_CURR_NAM_I:         /* Current NAM setting */
            pData->curr_nam = 0;   /* write it to nv, if never set */
            SKY_PutNV(NV_CURR_NAM_I, pData );
            break;

        case NV_CONFIG_CHKSUM_I:    /* Configuration checksum */
            pData->config_chksum.chksum = 0; /* 0 = bad checksum default */
            break;

        case NV_ERR_LOG_I:
            pData->err_log.err_count=0;
            break;

        case NV_NAME_NAM_I:
            memset( (void *) pData->name_nam.name, ' ', MAX_NAM_NAME_LENGTH);
            break;

        case NV_SPARE_2_I:
            pData->spare_2 = 0;
            break;

        case NV_MAX_TX_ADJ_I:
            pData->max_tx_adj = 0;
            break;
        case NV_INI_MUTE_I:
            pData->init_mute = NV_MUTE_NORM;
            break;

        case NV_DM_ADDR_I:
            pData->dm_addr = 1;
            break;

        case NV_FACTORY_INFO_I:
            memcpy((void *) pData->fact_info, "No Entry", 9 );
            break;

        case NV_WDC_I:              /* Warranty Date Code */
            (void) memset ( (void *) pData->wdc.wdc, ' ', NV_WDC_LTRS );
            break;

        /* If it's an unknown item, print an info message and pass on the */
        /* NV_NOTACTIVE_S status. */
        default:
            SKY_TRACE1(TL_HIGH, "Nonactive NV item 0x%x", nvi.item);
            nvi.status = NV_NOTACTIVE_S;
            break;
        }
    } 
    else 
    {
        if( nvi.status != NV_DONE_S ) 
        {
            SKY_ERROR2( "NV Read Failed Item %d Code %d", nvi.item, nvi.status);
        }
    }

    return( nvi.status );
}

/*===========================================================================
FUNCTION UI_PUT_NV

DESCRIPTION
  Write an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the UI task.
===========================================================================*/
NV_STATE_E SKY_API SKY_PutNV(NV_ITEM_E Item, NV_ITEM_U *pData)
{

    nvi.tcb_ptr = rex_self();        /* Notify this task when done */
    nvi.sigs = UI_NV_SIG;
    nvi.done_q_ptr = NULL;           /* command goes to no queue when done */

    nvi.item = Item;                 /* item to write */
    nvi.cmd = NV_WRITE_F;

    nvi.data_ptr =  pData;           /* the data to write */


    /* Clear the signal, call NV, wait for it to finish */
    (void) SKY_Clear_Signal( rex_self(), UI_NV_SIG );
    nv_cmd( &nvi );
    sWaitNV();

    if( nvi.status != NV_DONE_S ) 
    {
        SKY_ERROR2("NV Write Failed Item %d Code %d", nvi.item, nvi.status);
    }

    return( nvi.status );
}

/*===========================================================================
FUNCTION UI_REPLACE_NV

DESCRIPTION
  Replace an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the UI task.
===========================================================================*/
NV_STATE_E SKY_API SKY_ReplaceNV(NV_ITEM_E Item, NV_ITEM_U *pData)
{
    nvi.tcb_ptr = rex_self();        /* Notify this task when done */
    nvi.sigs = UI_NV_SIG;
    nvi.done_q_ptr = NULL;           /* command goes to no queue when done */

    nvi.item = Item;                 /* item to write */
    nvi.cmd = NV_REPLACE_F;

    nvi.data_ptr = pData;        /* the data to write */


    /* Clear the signal, call NV, wait for it to finish */
    (void) SKY_Clear_Signal( rex_self(), UI_NV_SIG );
    nv_cmd( &nvi );
    sWaitNV();

    if( nvi.status != NV_DONE_S ) 
    {
        SKY_ERROR2("NV Replace Failed Item %d Code %d", nvi.item, nvi.status);
    }

    return( nvi.status );
}

/*===========================================================================
FUNCTION UI_FREE_NV

DESCRIPTION
  Free an item in NV memory.  Wait till free is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the UI task.
===========================================================================*/
NV_STATE_E SKY_API SKY_FreeNV(NV_ITEM_E Item, NV_ITEM_U *pData)
{
    nvi.tcb_ptr = rex_self();        /* Notify this task when done */
    nvi.sigs = UI_NV_SIG;
    nvi.done_q_ptr = NULL;           /* command goes to no queue when done */

    nvi.item = Item;                 /* item to free */
    nvi.cmd = NV_FREE_F;

    nvi.data_ptr =  pData;           /* the data to write */

    /* Clear the signal, call NV, wait for it to finish */
    (void) SKY_Clear_Signal( rex_self(), UI_NV_SIG );
    nv_cmd( &nvi );
    sWaitNV();

    if( nvi.status != NV_DONE_S ) 
    {
        SKY_ERROR2("NV Free Failed Item %d Code %d", nvi.item, nvi.status);
    }

    return( nvi.status );
}


void SKY_SaveNV(void)
{


}

//white 2001.01.05
/*===========================================================================

FUNCTION NV_SETUP

DESCRIPTION
  This function assigns default values for each NV item.

DEPENDENCIES
  This function is called only after nvb_build_nvm().
  Don't worry, you only go as usual.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void SKY_API SKY_NV_Setup(NV_ITEM_E StartItem, NV_ITEM_E EndItem)
{
  static NV_ITEM_U      nv_item;
  NV_COMMAND_T          nv_cmd;      /* Command block */
  NV_STATE_E            status;      /* Status to return to calling procedure */
  word                  index , j;
  word                  i = 0;       /* counter */
  word                  item;
  byte                  bWrite = 1;

  nv_cmd.data_ptr = &nv_item;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for(item = StartItem; item <= EndItem; item++)
  {

	  bWrite = 1;
	  nv_cmd.item = (NV_ITEM_E)item;	

	  switch((nv_items_enum_type)item){
      case NV_ESN_I:
  
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                       
      case NV_SCM_I:

		nv_cmd.data_ptr->scm = 0x2A; //00101010
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SLOT_CYCLE_INDEX_I:

		nv_cmd.data_ptr->slot_cycle_index = 0x02;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MOB_CAI_REV_I:

#ifdef FEATURE_JSTD008
		nv_cmd.data_ptr->mob_cai_rev = 0x01;
#else
		nv_cmd.data_ptr->mob_cai_rev = 0x03;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MOB_FIRM_REV_I:

		nv_cmd.data_ptr->mob_firm_rev = 416;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MOB_MODEL_I:

#ifdef T_SURF
		nv_cmd.data_ptr->mob_model = 0x1D;
#else
		nv_cmd.data_ptr->mob_model = 0x07;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CONFIG_CHKSUM_I:

		nv_cmd.data_ptr->config_chksum.chksum = 0xFFFF;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PREF_MODE_I:

    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->pref_mode.nam = index;
        nv_cmd.data_ptr->pref_mode.mode = NV_MODE_DIGITAL_ONLY;

		nvimw_write(&nv_cmd);
	} /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_PREF_SERV_I:

    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->cdma_pref_serv.nam = index;
        nv_cmd.data_ptr->cdma_pref_serv.sys = NV_SYS_B_ONLY;

		nvimw_write(&nv_cmd);
	} /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ANALOG_PREF_SERV_I:

    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->analog_pref_serv.nam = index;
        nv_cmd.data_ptr->analog_pref_serv.sys = NV_SYS_A_ONLY;

		nvimw_write(&nv_cmd);
	} /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_SID_LOCK_I:

    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->cdma_sid_lock.nam = index;
        for (i = 0; i <NV_MAX_SID_LOCK; i++)
           nv_cmd.data_ptr->cdma_sid_lock.sid[i]= 0x0000;

		nvimw_write(&nv_cmd);
	} /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_SID_ACQ_I:

    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->cdma_sid_acq.nam = index;
        for (i = 0; i <NV_MAX_SID_ACQ; i++)
           nv_cmd.data_ptr->cdma_sid_acq.sid[i]= 0x0000;

		nvimw_write(&nv_cmd);
	} /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ANALOG_SID_LOCK_I:

    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->analog_sid_lock.nam = index;
        for (i = 0; i < NV_MAX_SID_LOCK; i++)
          nv_cmd.data_ptr->analog_sid_lock.sid[i]= 0x0000;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ANALOG_SID_ACQ_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->analog_sid_acq.nam = index;
        for (i = 0; i < NV_MAX_SID_ACQ; i++)
          nv_cmd.data_ptr->analog_sid_acq.sid[i]= 0x0000;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ANALOG_FIRSTCHP_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->analog_firstchp.nam = index;
        nv_cmd.data_ptr->analog_firstchp.channel = 0x014D; //333

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ANALOG_HOME_SID_I:

    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->analog_home_sid.nam = index;
        nv_cmd.data_ptr->analog_home_sid.sid = 0x1111; //4369
		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ANALOG_REG_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->analog_reg.nam = index;
        nv_cmd.data_ptr->analog_reg.kind = NV_WHEREABOUTS_KNOWN;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PCDMACH_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->pcdmach.nam = index;
        nv_cmd.data_ptr->pcdmach.channel_a = 779;
        nv_cmd.data_ptr->pcdmach.channel_b = 779;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SCDMACH_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->scdmach.nam = index;
        nv_cmd.data_ptr->scdmach.channel_a = 738;
        nv_cmd.data_ptr->scdmach.channel_b = 738;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
	break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PPCNCH_I:
//    for(index = 0; index <NV_MAX_NAMS; index++)
//    {
//        nv_cmd.data_ptr->ppcnch.nam = index;
//        nv_cmd.data_ptr->ppcnch.channel_a = 0;
//        nv_cmd.data_ptr->ppcnch.channel_b = 0;

//        status = nvimio_fixed_array(NVI_PPCNCH_I, 
//                                      NV_MAX_NAMS,
//                                      nv_cmd.data_ptr->ppcnch.nam,
//                                      &nv_cmd.data_ptr->ppcnch.channel_a, 
//                                      NVI_SIZEOF(ppcnch),
//                                      NV_WRITE_IO,
//                                      NV_FIXED_POS(ppcnch));
//    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SPCNCH_I:
//    for(index = 0; index <NV_MAX_NAMS; index++)
//   {
//        nv_cmd.data_ptr->spcnch.nam = index;
//        nv_cmd.data_ptr->spcnch.channel_a = 0;
//        nv_cmd.data_ptr->spcnch.channel_b = 0;

//        status = nvimio_fixed_array(NVI_SPCNCH_I, 
//                                      NV_MAX_NAMS,
//                                      nv_cmd.data_ptr->spcnch.nam,
//                                      &nv_cmd.data_ptr->spcnch.channel_a, 
//                                      NVI_SIZEOF(spcnch),
//                                      NV_WRITE_IO,
//                                      NV_FIXED_POS(spcnch));
//    } /* for */
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_NAM_CHKSUM_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->nam_chksum.nam = index;
        nv_cmd.data_ptr->nam_chksum.chksum = 0xFFFF;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_A_KEY_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->a_key.nam = index;
        for(i = 0; i < 8; i++)
            nv_cmd.data_ptr->a_key.key[i] = 0x00; //64bits

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SSD_A_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->ssd_a.nam = index;
        for(i = 0; i < 8; i++)
            nv_cmd.data_ptr->ssd_a.ssd[i] = 0x00; //64bits

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SSD_B_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->ssd_b.nam = index;
        for(i = 0; i < 8; i++)
            nv_cmd.data_ptr->ssd_b.ssd[i] = 0x00; //64bits

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_COUNT_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->count.nam = index;
        nv_cmd.data_ptr->count.count = 0x00;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

//#ifdef TREX_girin
//#else
      case NV_MIN1_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->min1.nam = index;
        nv_cmd.data_ptr->min1.min1[0] = 0x00E36BE7L; //010-0000
        nv_cmd.data_ptr->min1.min1[1] = 0x00E36BE7L; //010-0000

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MIN2_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->min2.nam = index;
        nv_cmd.data_ptr->min2.min2[0] = 0x0384; //011
        nv_cmd.data_ptr->min2.min2[1] = 0x0384; //011

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;
//#endif // TREX_girin

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_IMSI_11_12_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->imsi_11_12.nam = index;
        nv_cmd.data_ptr->imsi_11_12.imsi_11_12 = 0x00; //99

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DIR_NUMBER_I:
//#ifdef TREX_girin
//#else
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->dir_number.nam = index;
        for(i = 0; i < NV_DIR_NUMB_SIZ; i++)
            nv_cmd.data_ptr->dir_number.dir_number[i] = "0110100000"[i]; //0

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;
//#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DIR_NUMBER_PCS_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->mob_dir_number.nam = index;
        nv_cmd.data_ptr->mob_dir_number.n_digits = 0xA; //10

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_IMSI_MCC_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->imsi_mcc.nam = index;
        nv_cmd.data_ptr->imsi_mcc.imsi_mcc = 0x000; //999

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MOB_TERM_HOME_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->mob_term_home.nam = index;
        nv_cmd.data_ptr->mob_term_home.enabled[0] = TRUE;
        nv_cmd.data_ptr->mob_term_home.enabled[1] = TRUE;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MOB_TERM_FOR_SID_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->mob_term_for_sid.nam = index;
        nv_cmd.data_ptr->mob_term_for_sid.enabled[0] = TRUE;
        nv_cmd.data_ptr->mob_term_for_sid.enabled[1] = TRUE;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MOB_TERM_FOR_NID_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->mob_term_for_nid.nam = index;
        nv_cmd.data_ptr->mob_term_for_nid.enabled[0] = TRUE;
        nv_cmd.data_ptr->mob_term_for_nid.enabled[1] = TRUE;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ACCOLC_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->accolc.nam = index;
        nv_cmd.data_ptr->accolc.class[0] = 0x00;
        nv_cmd.data_ptr->accolc.class[1] = 0x00;
		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SID_NID_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->sid_nid.nam = index;
		for(i = 0; i < NV_MAX_MINS; i++)
		    for(j = 0; j < NV_MAX_SID_NID; j++)
			{
				if(j == 0){
                    nv_cmd.data_ptr->sid_nid.pair[i][j].sid = 2222;
#ifdef TREX_girin
                    nv_cmd.data_ptr->sid_nid.pair[i][j].nid = 65535;
#else
					nv_cmd.data_ptr->sid_nid.pair[i][j].nid = 1;
#endif // TREX_girin
				} else if (j == 1){
                    nv_cmd.data_ptr->sid_nid.pair[i][j].sid = 2222;
#ifdef TREX_girin
                    nv_cmd.data_ptr->sid_nid.pair[i][j].nid = 65535;
#else
					nv_cmd.data_ptr->sid_nid.pair[i][j].nid = 2;
#endif

				} else if (j == 2){
                    nv_cmd.data_ptr->sid_nid.pair[i][j].sid = 2222;
#ifdef TREX_girin
                    nv_cmd.data_ptr->sid_nid.pair[i][j].nid = 65535;
#else
					nv_cmd.data_ptr->sid_nid.pair[i][j].nid = 3;
#endif

				} else if (j == 3){
                    nv_cmd.data_ptr->sid_nid.pair[i][j].sid = 2222;
                    nv_cmd.data_ptr->sid_nid.pair[i][j].nid = 65535;
				
				}
			} /* for j */

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MIN_CHKSUM_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->min_chksum.nam = index;
        nv_cmd.data_ptr->min_chksum.chksum[0] = 0xFFFF;
        nv_cmd.data_ptr->min_chksum.chksum[1] = 0xFFFF;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CURR_NAM_I:
        //nv_cmd.data_ptr->curr_nam = 0x00;
        /* SKT, STI 통합버전에서는 시스템 초기화 후 무조건 NAM1을 Default로    */
        /* 설정해 주는 것이 아니라 Service Provider에 따라 설정되도록 바뀌었다.*/
        if(SKY_GET_SERVICE_PROVIDER() == SP_SKT)
        {
            nv_cmd.data_ptr->curr_nam = CM_NAM_1;
            SKY_SET_CURRENT_NAM(CM_NAM_1);
        }
        else if(SKY_GET_SERVICE_PROVIDER() == SP_STI)
        {
            nv_cmd.data_ptr->curr_nam = CM_NAM_2;
            SKY_SET_CURRENT_NAM(CM_NAM_2);
        }
        else
        {
            nv_cmd.data_ptr->curr_nam = CM_NAM_1;
            SKY_SET_CURRENT_NAM(CM_NAM_1);
        }
        nvimw_write(&nv_cmd);
        bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AUTO_NAM_I:
        nv_cmd.data_ptr->auto_nam = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_NAME_NAM_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->name_nam.nam = index;
#ifdef STI_MMI
		if(index == 0)
            strcpy((char *)nv_cmd.data_ptr->name_nam.name,"i-touch 017     ");
		else if(index == 1)
            strcpy((char *)nv_cmd.data_ptr->name_nam.name,"국제로밍 A      ");
		else if(index == 2)
            strcpy((char *)nv_cmd.data_ptr->name_nam.name,"국제로밍 B      ");
		else if(index == 3)
            strcpy((char *)nv_cmd.data_ptr->name_nam.name,"기타            ");
#else
		if(index == 0)
//            strcpy(nv_cmd.data_ptr->name_nam.name," 전화번호 없음  ");
            strcpy((char *)nv_cmd.data_ptr->name_nam.name,"갖고싶은번호 011");
		else if(index == 1)
            strcpy((char *)nv_cmd.data_ptr->name_nam.name," 전화번호 없음  ");
		else if(index == 2)
            strcpy((char *)nv_cmd.data_ptr->name_nam.name," 전화번호 없음  ");
		else if(index == 3)
            strcpy((char *)nv_cmd.data_ptr->name_nam.name," 전화번호 없음  ");
#endif

		nvimw_write(&nv_cmd);
        } /* for */
	    bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_NXTREG_I:
        nv_cmd.data_ptr->nxtreg = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LSTSID_I:
        nv_cmd.data_ptr->lstsid = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LOCAID_I:
        nv_cmd.data_ptr->locaid = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PUREG_I:
        nv_cmd.data_ptr->pureg = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ZONE_LIST_I:
        nv_cmd.data_ptr->zone_list.reg_zone = 0x0000;
        nv_cmd.data_ptr->zone_list.sid = 0x0000;
        nv_cmd.data_ptr->zone_list.nid = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SID_NID_LIST_I:
        nv_cmd.data_ptr->sid_nid_list.sid = 0x0000;
        nv_cmd.data_ptr->sid_nid_list.nid = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DIST_REG_I:
        nv_cmd.data_ptr->dist_reg.base_lat_reg = 0x00000000;
        nv_cmd.data_ptr->dist_reg.base_long_reg = 0x00000000;
        nv_cmd.data_ptr->dist_reg.reg_dist_reg = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LAST_CDMACH_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->last_cdmach.nam = index;
        nv_cmd.data_ptr->last_cdmach.channel_a = 0x0000;
        nv_cmd.data_ptr->last_cdmach.channel_b = 0x0000;

		nvimw_write(&nv_cmd);
	} /* for */
	bWrite = 0;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*
      case NV_CALL_TIMER_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->call_timer.nam = index;
        nv_cmd.data_ptr->call_timer.time = 0x00;

		nvimw_write(&nv_cmd);
    } 
	bWrite = 0;
		break;

*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CALL_TIMER_G_I:
        nv_cmd.data_ptr->call_timer_g.time = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AIR_TIMER_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->air_timer.nam = index;
        nv_cmd.data_ptr->air_timer.time = 0x00;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ROAM_TIMER_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->roam_timer.nam = index;
        nv_cmd.data_ptr->roam_timer.time = 0x00;

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*
      case NV_LIFE_TIMER_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->life_timer.nam = index;
        nv_cmd.data_ptr->life_timer.time = 0x00;
		nvimw_write(&nv_cmd);
    } 
	bWrite = 0;
		break;
*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LIFE_TIMER_G_I:
        nv_cmd.data_ptr->life_timer_g.time = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_RUN_TIMER_I:
        nv_cmd.data_ptr->run_timer.time = 0x00;
		break;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_STACK_IDX_I:
    for(index = 0; index <40 /*NV_MAX_ORISTACK_DIALS*/; index++)
    {
        nv_cmd.data_ptr->stack_idx.index[index] = 0xFF; // no items on stack
        nv_cmd.data_ptr->stack_idx.secret[index] = FALSE; // no items on stack

		nvimw_write(&nv_cmd);
    } /* for */
	bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PAGE_SET_I:
        nv_cmd.data_ptr->page_set.enable = FALSE;
        nv_cmd.data_ptr->page_set.rings = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PAGE_MSG_I:
        /* Obsolete item */
//        status = NV_NOTACTIVE_S;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_EAR_LVL_I:
        nv_cmd.data_ptr->ear_level = 3; // 3rd of 5 volume
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SPEAKER_LVL_I:
        nv_cmd.data_ptr->speaker_level = 3;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_RINGER_LVL_I:
        nv_cmd.data_ptr->ringer_level = 3;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_BEEP_LVL_I:
        nv_cmd.data_ptr->beep_level = 3;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CALL_BEEP_I:
        nv_cmd.data_ptr->call_beep = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CONT_KEY_DTMF_I:
        nv_cmd.data_ptr->cont_key_dtmf = TRUE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CONT_STR_DTMF_I:
        nv_cmd.data_ptr->cont_str_dtmf = TRUE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SVC_AREA_ALERT_I:
        nv_cmd.data_ptr->svc_area_alert = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CALL_FADE_ALERT_I:
        nv_cmd.data_ptr->call_fade_alert = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_BANNER_I:
        /*Banner NV Item Not Used. Why is there Item?*/
          break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// This item must not to be here //happykid say in 01.05.08
//      case NV_LCD_I:
        /* Why is there Item? */
//        nv_cmd.data_ptr->lcd = LCD_CONTRAST_DEFAULT; // contrast
//		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AUTO_POWER_I:
        nv_cmd.data_ptr->auto_power = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AUTO_ANSWER_I:
        nv_cmd.data_ptr->auto_answer.enable = FALSE;
        nv_cmd.data_ptr->auto_answer.rings = 0x01;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AUTO_REDIAL_I:
        nv_cmd.data_ptr->auto_redial.enable = TRUE;
//        nv_cmd.data_ptr->auto_redial.rings = 0x03;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AUTO_HYPHEN_I:
        nv_cmd.data_ptr->auto_hyphen = TRUE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// This item must not to be here //happykid say in 01.05.09
//      case NV_BACK_LIGHT_I:
//        nv_cmd.data_ptr->back_light = 0x0A;
//		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// This item must not to be here //happykid say in 01.05.11
//      case NV_AUTO_MUTE_I:
//        nv_cmd.data_ptr->auto_mute = TRUE;
//		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MAINTRSN_I:
        nv_cmd.data_ptr->maintrsn.maint_order = FALSE;
        nv_cmd.data_ptr->maintrsn.maint_reason = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LCKRSN_P_I:
        nv_cmd.data_ptr->lckrsn_p.lock_order = FALSE;
        nv_cmd.data_ptr->lckrsn_p.lock_reason = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LOCK_I:
        nv_cmd.data_ptr->lock = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LOCK_CODE_I:

    for(i = 0; i < NV_LOCK_CODE_SIZE; i++)
        nv_cmd.data_ptr->lock_code.digits[i] = '0';
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AUTO_LOCK_I:
        nv_cmd.data_ptr->auto_lock = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      case NV_FN_SEQUENCE_I:
		for(index = 0; index <NV_MAX_FL_FILE_NUM; index++)
		{
			  nv_cmd.data_ptr->Fn_sequence.address = index;
			  
			  nv_cmd.data_ptr->Fn_sequence.Fn = NV_MAX_FL_FILE_NUM;
			  nv_cmd.data_ptr->Fn_sequence.Type = 0; // VTYPE_NONE
			  nv_cmd.data_ptr->Fn_sequence.Time = 0;
		nvimw_write(&nv_cmd);

		}
		bWrite = 0;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_VOICE_INFO_I:
        nv_cmd.data_ptr->voice_info.total_rec_num = 0;
		nv_cmd.data_ptr->voice_info.new_ans_num = 0;
		nv_cmd.data_ptr->voice_info.is_user_voice = FALSE;
		nv_cmd.data_ptr->voice_info.user_voice_fn = NV_MAX_FL_FILE_NUM;
		nv_cmd.data_ptr->voice_info.user_voice_l = 0; 

		nv_cmd.data_ptr->voice_info.remained_byte = TOTAL_REC_BYTE; // 64K 2.5블럭 사이즈 
		nv_cmd.data_ptr->voice_info.remained_time = TOTAL_REC_TIME;		 // 2 minute by jnlee 2000.1.15
//		nv_cmd.data_ptr->voice_info.remained_byte = 245760; // NV_FL_EOC * 4 * 'cell size' = 240*4*256
//		nv_cmd.data_ptr->voice_info.remained_time = 210;

		voice_erase_all_when_nv_setup();		// inserted by jnlee 2000.1.20

		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CALL_RSTRC_I:
        nv_cmd.data_ptr->call_rstrc = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SEC_CODE_I:
    for(i = 0; i < NV_SEC_CODE_SIZE; i++)
        nv_cmd.data_ptr->sec_code.digits[i] = "000000"[i];
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_HORN_ALERT_I:
        nv_cmd.data_ptr->horn_alert = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ERR_LOG_I:
//        status = nvimio_fixed_array(NVI_ERR_LOG_I, 
//                                        NV_MAX_ERR_LOG,
//                                        nv_cmd.data_ptr->err_log.address,
//                                        &nv_cmd.data_ptr->err_log.err_count, 
//                                        NVI_SIZEOF(err_log),
//                                        NV_WRITE_IO,
//                                        NV_FIXED_POS(err_log));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_VBATT_I:
        nv_cmd.data_ptr->vbatt.min = 0x00;
        nv_cmd.data_ptr->vbatt.max = 0xFF;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_TX_PWR_I:
        nv_cmd.data_ptr->fm_tx_pwr.level_0 = 0x00;
        nv_cmd.data_ptr->fm_tx_pwr.level_1 = 0x00;
        nv_cmd.data_ptr->fm_tx_pwr.level_2 = 0x00;
        nv_cmd.data_ptr->fm_tx_pwr.level_3 = 0x00;
        nv_cmd.data_ptr->fm_tx_pwr.level_4 = 0x00;
        nv_cmd.data_ptr->fm_tx_pwr.level_5 = 0x00;
        nv_cmd.data_ptr->fm_tx_pwr.level_6 = 0x00;
        nv_cmd.data_ptr->fm_tx_pwr.level_7 = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FR_TEMP_OFFSET_I:
    for(i = 0; i < NV_FR_TEMP_OFFSET_TABLE_SIZ; i++)
        nv_cmd.data_ptr->fr_temp_offset.item[i] = 0x00;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_RSSI_I:       //1999.10.04
        nv_cmd.data_ptr->fm_rssi.min = 0xA0;//0xB0;
        nv_cmd.data_ptr->fm_rssi.max = 0x40;//0x37;

		break;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*
      case NV_SONY_ATTEN_1_I:
        nv_cmd.data_ptr->sony_atten_1 = 0x00;
		break;
*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*
      case NV_SONY_ATTEN_2_I:
        nv_cmd.data_ptr->sony_atten_2 = 0x00;
		break;
*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SPARE_1_I:
        nv_cmd.data_ptr->spare_1 = 0xff;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SPARE_2_I:
        nv_cmd.data_ptr->spare_2 = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DATA_IO_MODE_I:
        nv_cmd.data_ptr->data_io_mode = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DM_IO_MODE_I:

        nv_cmd.data_ptr->dm_io_mode = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DATA_SRVC_STATE_I:
        nv_cmd.data_ptr->data_srvc_enabled = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_IDLE_DATA_TIMEOUT_I:
        nv_cmd.data_ptr->idle_data_timeout = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MAX_TX_ADJ_I:
        nv_cmd.data_ptr->max_tx_adj = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_INI_MUTE_I:
        nv_cmd.data_ptr->init_mute = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FACTORY_INFO_I:
    for(i = 0; i < NV_FACTORY_INFO_SIZ; i++)
        nv_cmd.data_ptr->fact_info[i] = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SONY_ATTEN_3_I:
        nv_cmd.data_ptr->sony_atten_3 = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SONY_ATTEN_4_I:
        nv_cmd.data_ptr->sony_atten_4 = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SONY_ATTEN_5_I:
        nv_cmd.data_ptr->sony_atten_5 = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_PN_MASK_I:
        nv_cmd.data_ptr->cdma_pn_mask = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SEND_TIMEOUT_I:
        nv_cmd.data_ptr->send_timeout = 0x00;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_AGC_SET_VS_PWR_I:
    for(i = 0; i < NV_PWR_TABLE_SIZ; i++)
        nv_cmd.data_ptr->fm_agc_set_vs_pwr[i] = 64;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_AGC_SET_VS_FREQ_I:
    for(i = 0; i < NV_FREQ_TABLE_SIZ; i++)
        nv_cmd.data_ptr->fm_agc_set_vs_freq[i] = 64;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_AGC_SET_VS_TEMP_I:
    for(i = 0; i < NV_TEMP_TABLE_SIZ; i++)
        nv_cmd.data_ptr->fm_agc_set_vs_temp[i] = 64;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_FREQ_SENSE_GAIN_I:       //1999.10.04.
        nv_cmd.data_ptr->fm_freq_sense_gain = 0x79;//0x7A;//0x80;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_AGC_ADJ_VS_FREQ_I:
    for(i = 0; i < NV_FREQ_TABLE_SIZ; i++)
        nv_cmd.data_ptr->fm_agc_adj_vs_freq[i] = 64;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_EXP_HDET_VS_PWR_I:
    for(i = 0; i < NV_PWR_TABLE_SIZ; i++)
        nv_cmd.data_ptr->fm_exp_hdet_vs_pwr[i] = 64;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_ERR_SLP_VS_PWR_I:
    for(i = 0; i < NV_PWR_TABLE_SIZ; i++)
        nv_cmd.data_ptr->fm_err_slp_vs_pwr[i] = 64;

		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_LNA_CTL_I:
        nv_cmd.data_ptr->fm_lna_ctl.min = 0x9E;
        nv_cmd.data_ptr->fm_lna_ctl.max = 0x88;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_RX_LIN_OFF_0_I:
        nv_cmd.data_ptr->cdma_rx_lin_off_0 = 0x41;
		break;

      case NV_CDMA_RX_LIN_SLP_I:
        nv_cmd.data_ptr->cdma_rx_lin_slp[0] = 0x14;
        nv_cmd.data_ptr->cdma_rx_lin_slp[1] = 0x11;
        nv_cmd.data_ptr->cdma_rx_lin_slp[2] = 0x08;
        nv_cmd.data_ptr->cdma_rx_lin_slp[3] = 0x09;
        nv_cmd.data_ptr->cdma_rx_lin_slp[4] = 0x0A;
        nv_cmd.data_ptr->cdma_rx_lin_slp[5] = 0x0A;
        nv_cmd.data_ptr->cdma_rx_lin_slp[6] = 0x0A;
        nv_cmd.data_ptr->cdma_rx_lin_slp[7] = 0x09;
        nv_cmd.data_ptr->cdma_rx_lin_slp[8] = 0x0A;
        nv_cmd.data_ptr->cdma_rx_lin_slp[9] = 0x09;
        nv_cmd.data_ptr->cdma_rx_lin_slp[10] = 0x09;
        nv_cmd.data_ptr->cdma_rx_lin_slp[11] = 0x08;
        nv_cmd.data_ptr->cdma_rx_lin_slp[12] = 0x07;
        nv_cmd.data_ptr->cdma_rx_lin_slp[13] = 0x04;
        nv_cmd.data_ptr->cdma_rx_lin_slp[14] = 0x02;
        nv_cmd.data_ptr->cdma_rx_lin_slp[15] = 0x01;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_RX_COMP_VS_FREQ_I:
    for(i = 0; i < NV_FREQ_TABLE_SIZ; i++)
        nv_cmd.data_ptr->cdma_rx_comp_vs_freq[i] = 0x00;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_COMP_VS_FREQ_I:
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[0] = -25;//-113;//==143;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[1] = -60;//108;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[2] = 118;//37;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[3] = 104;//26;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[4] = 91;//12;//0x00;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[5] = 0x00;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[6] = 0x00;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[7] = 0x00;

        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[8] = -58;//125;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[9] = -81;//109;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[10] = 96;//39;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[11] = 83;//27;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[12] = 70;//14;//0x00;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[13] = 0x00;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[14] = 0x00;
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[15] = 0x00;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_LIN_MASTER_OFF_0_I:
#ifdef ZZB5
        nv_cmd.data_ptr->cdma_tx_lin_master_off_0 = 0x00;
#else
        nv_cmd.data_ptr->cdma_tx_lin_master_off_0 = 0x6E;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_LIN_MASTER_SLP_I:
#ifdef ZZB5
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[0] =  0x00;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[1] =  0x15;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[2] =  0x2a;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[3] =  0x23;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[4] =  0x20;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[5] =  0x1f;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[6] =  0x1d;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[7] =  0x1c;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[8] =  0x1a;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[9] =  0x17;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[10] = 0x16;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[11] = 0x15;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[12] = 0x15;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[13] = 0x15;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[14] = 0x19;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[15] = 0x19;
#else
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[0] =  0x1D;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[1] =  0x1D;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[2] =  0x1D;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[3] =  0x1B;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[4] =  0x19;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[5] =  0x18;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[6] =  0x16;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[7] =  0x15;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[8] =  0x15;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[9] =  0x14;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[10] = 0x14;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[11] = 0x16;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[12] = 0x20;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[13] = 0x50;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[14] = 0x00;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[15] = 0x00;
#endif
    for(i = 16; i < NV_CDMA_TX_LIN_MASTER_SIZ; i++)
        nv_cmd.data_ptr->cdma_tx_comp_vs_freq[i] = 0x00;

/*
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[16] = 0x09;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[17] = 0x08;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[18] = 0x09;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[19] = 0x09;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[20] = 0x08;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[21] = 0x07;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[22] = 0x07;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[23] = 0x07;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[24] = 0x06;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[25] = 0x06;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[26] = 0x06;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[27] = 0x06;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[28] = 0x03;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[29] = 0x03;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[30] = 0x0D;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[31] = 0x0D;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[32] = 0x14;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[33] = 0x13;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[34] = 0x14;
        nv_cmd.data_ptr->cdma_tx_lin_master_slp[35] = 0x13;
*/
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_LIN_VS_TEMP_I:
    for(i = 0; i < NV_TEMP_TABLE_SIZ; i++)
        nv_cmd.data_ptr->cdma_tx_lin_vs_temp[i] = 0x00;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_LIN_VS_VOLT_I:
    for(i = 0; i < NV_VOLT_TABLE_SIZ; i++)
        nv_cmd.data_ptr->cdma_tx_lin_vs_volt[i] = 64;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_LIM_VS_TEMP_I:
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[0] = 64;
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[1] = 64;
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[2] = 64;
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[3] = 64;
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[4] = 64;
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[5] = 64;
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[6] = 64;
        nv_cmd.data_ptr->cdma_tx_lim_vs_temp[7] = 64;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_LIM_VS_FREQ_I:         //99.10.04     
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[0] = 80;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[1] = 78;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[2] = 76;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[3] = 75;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[4] = 74;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[5] = 74;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[6] = 73;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[7] = 73;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[8] = 73;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[9] = 76;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[10] = 76;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[11] = 75;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[12] = 73;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[13] = 71;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[14] = 72;//80;
        nv_cmd.data_ptr->cdma_tx_lim_vs_freq[15] = 67;//80;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_EXP_HDET_VS_AGC_I:
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[0] = 2;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[1] = 4;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[2] = 10;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[3] = 19;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[4] = 29;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[5] = 38;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[6] = 50;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[7] = 66;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[8] = 87;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[9] = 107;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[10] = 127;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[11] = 152;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[12] = 176;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[13] = 201;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[14] = 226;
//        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[15] = 251;

    for(i = 0; i < NV_CDMA_EXP_HDET_VS_AGC_SIZ; i++)
        nv_cmd.data_ptr->cdma_exp_hdet_vs_agc[i] = 64;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_ERR_SLP_VS_HDET_I:
    for(i = 0; i < NV_CDMA_ERR_SLP_VS_HDET_SIZ; i++)
        nv_cmd.data_ptr->cdma_err_slp_vs_hdet[i] = 64;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_RX_LIN_VS_TEMP_I:
		for(i = 0; i < 4; i++)
			nv_cmd.data_ptr->cdma_rx_lin_vs_temp[i] = 0;

		nv_cmd.data_ptr->cdma_rx_lin_vs_temp[4] = 6;

		for(i = 5; i < 8; i++)
			nv_cmd.data_ptr->cdma_rx_lin_vs_temp[i] = 12;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_ADJ_FACTOR_I:
        nv_cmd.data_ptr->cdma_adj_factor = 0x02;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_RX_SLP_VS_TEMP_I:
	    for(i = 0; i < 4; i++)
		    nv_cmd.data_ptr->cdma_rx_slp_vs_temp[i] = 0;

		nv_cmd.data_ptr->cdma_rx_slp_vs_temp[4] = -9;

	    for(i = 5; i < 8; i++)
		    nv_cmd.data_ptr->cdma_rx_slp_vs_temp[i] = -18;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_CDMA_TX_SLP_VS_TEMP_I:
    for(i = 0; i < NV_TEMP_TABLE_SIZ; i++)
        nv_cmd.data_ptr->cdma_tx_slp_vs_temp[i] = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PA_RANGE_VS_TEMP_I:
    for(i = 0; i < NV_TEMP_TABLE_SIZ; i++)
#ifdef ZZB5
        nv_cmd.data_ptr->pa_range_vs_temp[i] = 64;
#else
        nv_cmd.data_ptr->pa_range_vs_temp[i] = 0x00;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_EXP_HDET_VS_TEMP_I:     //1999.10.04.

#ifdef ZZB5
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[0] = 0x31;//0x10;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[1] = 0x0b;//0x0D;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[2] = 0x0a;//0x15;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[3] = 0x11;//0x22;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[4] = 0x11;//0x22;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[5] = 0x11;//0x22;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[6] = 0x11;//0x22;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[7] = 0x11;//0x22;
#else
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[0] = 0x33;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[1] = 0x2B;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[2] = 0x90;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[3] = 0x90;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[4] = 0x90;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[5] = 0x90;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[6] = 0x90;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[7] = 0x90;
#endif
/*
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[0] = 0x26;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[1] = 0x11;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[2] = 0x18;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[3] = 0x46;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[4] = 0x26;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[5] = 0x11;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[6] = 0x18;
        nv_cmd.data_ptr->fm_exp_hdet_vs_temp[7] = 0x46;
*/
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_THERM_I:
        nv_cmd.data_ptr->therm.min = 0x00;
        nv_cmd.data_ptr->therm.max = 0xFF;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_VBATT_PA_I:
        nv_cmd.data_ptr->vbatt_pa.min = 0xEB;//0xDA;
        nv_cmd.data_ptr->vbatt_pa.max = 0xF4;//0xDA;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_HDET_OFF_I:
#ifdef ZZB5
        nv_cmd.data_ptr->hdet_off = 0x02;
#else
        nv_cmd.data_ptr->hdet_off = 0x00;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_HDET_SPN_I:
#ifdef ZZB5
        nv_cmd.data_ptr->hdet_spn = 0x10;
#else
        nv_cmd.data_ptr->hdet_spn = 0xFF;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_RF_CONFIG_I:
        nv_cmd.data_ptr->rf_config = 0x06;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_R1_RISE_I:
        nv_cmd.data_ptr->r1_rise = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_R1_FALL_I:
        nv_cmd.data_ptr->r1_fall = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_R2_RISE_I:
        nv_cmd.data_ptr->r2_rise = 0x9B;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_R2_FALL_I:
        nv_cmd.data_ptr->r1_fall = 0x8A;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_R3_RISE_I:
#ifdef ZZB5
        nv_cmd.data_ptr->r3_rise = 0xFF;
#else
        nv_cmd.data_ptr->r3_rise = 0x20;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_R3_FALL_I:
#ifdef ZZB5
        nv_cmd.data_ptr->r3_fall = 0xFF;
#else
        nv_cmd.data_ptr->r3_fall = 0xE0;
#endif

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PA_RANGE_STEP_CAL_I:
#ifdef ZZB5
        nv_cmd.data_ptr->pa_range_step_cal = 0xFF;
#else
        nv_cmd.data_ptr->pa_range_step_cal = 0x20;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LNA_RANGE_POL_I:
        nv_cmd.data_ptr->lna_range_pol = 0x20;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LNA_RANGE_RISE_I:
        nv_cmd.data_ptr->lna_range_rise = 0x7F;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LNA_RANGE_FALL_I:
        nv_cmd.data_ptr->lna_range_fall = 0x7F;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LNA_RANGE_OFFSET_I:
        nv_cmd.data_ptr->lna_range_offset = 0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ALERTS_LVL_I:
#ifdef ZZB5
        nv_cmd.data_ptr->alerts_lvl = 0x5F;
#else
        nv_cmd.data_ptr->alerts_lvl = 0xAF;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ALERTS_LVL_SHADOW_I:
#ifdef ZZB5
        nv_cmd.data_ptr->alerts_lvl_shadow = 0x5F;
#else
        nv_cmd.data_ptr->alerts_lvl_shadow = 0xAF;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_RINGER_LVL_SHADOW_I:
#ifdef ZZB5
        nv_cmd.data_ptr->ringer_lvl_shadow = 0x5F;
#else
        nv_cmd.data_ptr->ringer_lvl_shadow = 0xAF;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_BEEP_LVL_SHADOW_I:
#ifdef ZZB5
        nv_cmd.data_ptr->beep_lvl_shadow = 0x5F;
#else
        nv_cmd.data_ptr->beep_lvl_shadow = 0xAF;
#endif
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_EAR_LVL_SHADOW_I:
#ifdef ZZB5
        nv_cmd.data_ptr->ear_lvl_shadow = 0x5F;
#else
        nv_cmd.data_ptr->ear_lvl_shadow = 0xAF;
#endif

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_TIME_SHOW_I:
        nv_cmd.data_ptr->time_show = TRUE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MESSAGE_ALERT_I:
        nv_cmd.data_ptr->message_alert = 0x01;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_AIR_CNT_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->air_cnt.nam = index;
        nv_cmd.data_ptr->air_cnt.cnt = 0x00;

		nvimw_write(&nv_cmd);
	} /* for */
		bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ROAM_CNT_I:
    for(index = 0; index <NV_MAX_NAMS; index++)
    {
        nv_cmd.data_ptr->roam_cnt.nam = index;
        nv_cmd.data_ptr->roam_cnt.cnt = 0x00;

		nvimw_write(&nv_cmd);
	} /* for */
		bWrite = 0;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_LIFE_CNT_I:
        nv_cmd.data_ptr->life_cnt = 0x00;

		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PWR_DWN_CNT_I:
        nv_cmd.data_ptr->pwr_dwn_cnt = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_SEND_PIN_I:
        nv_cmd.data_ptr->send_pin = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ONETOUCH_DIAL_I:
        nv_cmd.data_ptr->onetouch_dial = FALSE;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_VOICE_PRIV_I:
        nv_cmd.data_ptr->voice_priv = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif 

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_ALARMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      case NV_FSC_I:
    for(index = 0; index <NV_FSC_SIZ; index++)
        nv_cmd.data_ptr->fsc.fsc[index] = '0';	
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      case NV_FSC2_I:
        nv_cmd.data_ptr->fsc2.fsc2 = 0x0000;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      case NV_FSC2_CHKSUM_I:
        nv_cmd.data_ptr->fsc2_chksum.fsc2_chksum = 0x0000;
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      case NV_WDC_I:
    for(index = 0; index <NV_WDC_LTRS; index++)
        nv_cmd.data_ptr->wdc.wdc[index] = '0';	
		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FM_AGC_I: //1999.10.04
#ifdef ZZB5
        nv_cmd.data_ptr->fm_agc.min = 35;//0x20;//0x88;
        nv_cmd.data_ptr->fm_agc.max = 35;//0x20;//0x15;
		break;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_N1M_I:
        nv_cmd.data_ptr->n1m = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_HW_CONFIG_I:
        nv_cmd.data_ptr->hw_config = 0x08;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DM_ADDR_I:
        nv_cmd.data_ptr->dm_addr = 0xFF;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Very tired.. If you want to initialize any item, you'd better change in here ...
** I think there is no more useful item at this times... Maybe it is needed to change later.
*/
      case NV_IMSI_I:
//        status = nvimio_fixed_array(NVI_IMSI_I, 
//                                   NV_MAX_NAMS,
//                                   nv_cmd.data_ptr->imsi.nam,
//                                   &nv_cmd.data_ptr->imsi.imsi, 
//                                   NVI_SIZEOF(imsi),
//                                   NV_WRITE_IO,
//                                   NV_FIXED_POS(imsi));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//
      case NV_IMSI_ADDR_NUM_I:
//        status = nvimio_fixed_array(NVI_IMSI_ADDR_NUM_I, 
//                          NV_MAX_NAMS,
//                          nv_cmd.data_ptr->imsi_addr_num.nam,
//                          &nv_cmd.data_ptr->imsi_addr_num.num, 
//                          NVI_SIZEOF(imsi_addr_num),
//                          NV_WRITE_IO,
//                          NV_FIXED_POS(imsi_addr_num));
		break;

        
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ASSIGNING_TMSI_ZONE_LEN_I:
//        status = nvimio_fixed_array(NVI_ASSIGNING_TMSI_ZONE_LEN_I, 
//                          NV_MAX_NAMS,
//                          nv_cmd.data_ptr->assigning_tmsi_zone_length.nam,
//                          &nv_cmd.data_ptr->assigning_tmsi_zone_length.length, 
//                          NVI_SIZEOF(assigning_tmsi_zone_length),
//                          NV_WRITE_IO,
//                          NV_FIXED_POS(assigning_tmsi_zone_length));  
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_ASSIGNING_TMSI_ZONE_I:
//        status = nvimio_fixed_array(NVI_ASSIGNING_TMSI_ZONE_I, 
//                          NV_MAX_NAMS,
//                          nv_cmd.data_ptr->assigning_tmsi_zone.nam,
//                          &nv_cmd.data_ptr->assigning_tmsi_zone.zone, 
//                          NVI_SIZEOF(assigning_tmsi_zone),
//                          NV_WRITE_IO,
//                          NV_FIXED_POS(assigning_tmsi_zone));
		break;

///*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_TMSI_CODE_I:
//        status = nvimio_fixed_array(NVI_TMSI_CODE_I, 
//                                   NV_MAX_NAMS,
//                                   nv_cmd.data_ptr->tmsi_code.nam,
//                                   &nv_cmd.data_ptr->tmsi_code.code, 
//                                   NVI_SIZEOF(tmsi_code),
//                                   NV_WRITE_IO,
//                                   NV_FIXED_POS(tmsi_code));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_TMSI_EXP_I:
//        status = nvimio_fixed_array(NVI_TMSI_EXP_I, 
//                          NV_MAX_NAMS,
//                          nv_cmd.data_ptr->tmsi_exp_timer.nam,
//                          &nv_cmd.data_ptr->tmsi_exp_timer.time, 
//                          NVI_SIZEOF(tmsi_exp_timer),
//                          NV_WRITE_IO,
//                          NV_FIXED_POS(tmsi_exp_timer));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//

      case NV_ROAMING_LIST_I:
//
//        /* Roaming List address depends on amount of NVRAM available */
//        item_addr = nva_address_of_roaming_list();
//        /* And so does its size */
//        item_size = NV_ROAMING_LIST_HEADER_SIZE +
//                      nv_max_size_of_roaming_list();
//
//        status = nvimio_fixed_array(NVI_ROAMING_LIST_I, 
//                          NV_MAX_NAMS,
//                          nv_cmd.data_ptr->roaming_list.nam,
//                          &nv_cmd.data_ptr->roaming_list.prl_version,
//                          item_size,
//                          NV_WRITE_IO,
//                          item_addr);
		break;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_MRU2_TABLE_I:
//        status = nvimio_fixed_array(NVI_MRU2_TABLE_I, 
//                                   NV_MAX_NAMS,
//                                   nv_cmd.data_ptr->mru2_table.nam,
//                                   nv_cmd.data_ptr->mru2_table.entry, 
//                                   NVI_SIZEOF(mru2_table),
//                                   NV_WRITE_IO,
//                                   NV_FIXED_POS(mru2_table));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_OTKSL_I:
//        status = nvimio_fixed( &nv_cmd.data_ptr->otksl, 
//                                  NVI_SIZEOF(otksl),
//                                  NV_WRITE_IO,
//                                  NV_FIXED_POS(otksl));
		break;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_TIMED_PREF_MODE_I:
        nv_cmd.data_ptr->timed_pref_mode = FALSE;
		break;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_RINGER_TYPE_I:
        nv_cmd.data_ptr->ringer_type = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_ANY_KEY_ANSWER_I:
        nv_cmd.data_ptr->any_key_answer = 0x00;
		break;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_BACK_LIGHT_HFK_I:
        nv_cmd.data_ptr->back_light_hfk = 0x00;
		break;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_KEY_SOUND_I:
        nv_cmd.data_ptr->key_sound = (nv_key_sound_enum_type)0x0000;
		break;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DIALS_SORTING_METHOD_I:
        nv_cmd.data_ptr->sorting_method = (nv_dials_sort_enum_type)0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_LANGUAGE_SELECTION_I:
        nv_cmd.data_ptr->language_selection = (nv_language_enum_type)0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_MENU_FORMAT_I:
        nv_cmd.data_ptr->menu_format = (nv_menu_format_enum_type)0x0000;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_RINGER_SPKR_LVL_I:
        nv_cmd.data_ptr->ringer_spkr_lvl = 0xFF;
		break;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_BEEP_SPKR_LVL_I:
        nv_cmd.data_ptr->beep_spkr_lvl = 0xFF;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_VIBRATOR_I:
        nv_cmd.data_ptr->vibrator = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_FLIP_ANSWERS_I:
        nv_cmd.data_ptr->flip_answers = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_DIAL_PLAN
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_VOICE_PRIV_ALERT_I:
        nv_cmd.data_ptr->voice_priv_alert = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DATA_QNC_ENABLED_I:
        nv_cmd.data_ptr->data_qnc_enabled = 1; //T:yellow F:red
		break;
//
///*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//
      case NV_DATA_SO_SET_I:
        nv_cmd.data_ptr->data_so_set = 0; //T:yellow F:red
		break;

///*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//
      case NV_DATA_AUTO_PACKET_DETECTION_I:

        nv_cmd.data_ptr->data_auto_packet_detection = 0; //T:yellow F:red
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef NV_FEATURE_WEB_ITEMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef NV_FEATURE_WEB_ITEMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef NV_FEATURE_WEB_ITEMS
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

//      /* As a "peekable" formal test item, store in reserved_word_3 of     */
//      /* the NV invariant area.                                            */
      case NV_ELAPSED_TIME_I:
//        status = nvimio_fixed( &nv_cmd.data_ptr->elapsed_time,
//                                  NVI_SIZEOF(elapsed_time),
//                                  NV_WRITE_IO,
//                                  NV_FIXED_POS(reserved_word_3));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PDM2_I:
        nv_cmd.data_ptr->pdm2 = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_RX_AGC_MINMAX_I:
//        status = nvimio_fixed( &nv_cmd.data_ptr->rx_agc_minmax, 
//                                  NVI_SIZEOF(rx_agc_minmax),
//                                  NV_WRITE_IO,
//                                  NV_FIXED_POS(rx_agc_minmax));
		break;
//
///*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//
      case NV_VBATT_AUX_I:
//        status = nvimio_fixed( &nv_cmd.data_ptr->vbatt_aux,
//                                  NVI_SIZEOF(vbatt_aux),
//                                  NV_WRITE_IO,
//                                  NV_FIXED_POS(vbatt_aux));
		break;
//
///*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//
      case NV_DTACO_CONTROL_I:
//        status = nvimio_fixed( &nv_cmd.data_ptr->dtaco_control,
//                                  NVI_SIZEOF(dtaco_control),
//                                  NV_WRITE_IO,
//                                  NV_FIXED_POS(dtaco_control));
		break;
//
///*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//
      case NV_DTACO_INTERDIGIT_TIMEOUT_I:
//        status = nvimio_fixed( &nv_cmd.data_ptr->dtaco_interdigit_timeout,
//                                  NVI_SIZEOF(dtaco_interdigit_timeout),
//                                  NV_WRITE_IO,
//                                  NV_FIXED_POS(dtaco_interdigit_timeout));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_PDM1_I:
        nv_cmd.data_ptr->pdm1 = 0x00;
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      case NV_DIALING_PLAN_I:
        nv_cmd.data_ptr->dialing_plan = 0x00;

		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      case NV_STANDING_ALARM_I:
        nv_cmd.data_ptr->standing_alarm = 0x00;

		break;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


      case NV_DATA_PKT_ORIG_STR_I:
//        nv_cmd.data_ptr->data_pkt_orig_str = 0x0; //T:yellow F:red
//        status = nvimio_fixed( &nv_cmd.data_ptr->data_pkt_orig_str, 
//                                  NVI_SIZEOF(data_pkt_orig_str),
//                                  NV_WRITE_IO,
//                                  NV_FIXED_POS(data_pkt_orig_str));
		break;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

	  case NV_REF_TEMPSENSE_I:
		  nv_cmd.data_ptr->ref_tempsense.tempsense[0] = 155;//255;
		  nv_cmd.data_ptr->ref_tempsense.tempsense[1] = 105;//128;
		  nv_cmd.data_ptr->ref_tempsense.tempsense[2] = 75;//0;
		  break;

	  case NV_CDMA_FREQ_VS_PWR_I:
		  for(i=0;i<NV_FREQ_TABLE_SIZ;i++){
			  nv_cmd.data_ptr->freq_pwr[i].ref_txslp = 2;
			  for(j=0;j<3;j++)
				  nv_cmd.data_ptr->freq_pwr[i].power[j] = 248;
		  }

		  break;

  	  case NV_PA_PWR_I:
		  nv_cmd.data_ptr->pa_pwr_p1p2.ref_txslp = 27;
		  nv_cmd.data_ptr->pa_pwr_p1p2.min = 0;
		  nv_cmd.data_ptr->pa_pwr_p1p2.max = 0;
		  break;

      case NV_DATA_MDR_MODE_I:
          nv_cmd.data_ptr->data_mdr_mode = DS_MDR_MODE_IF_AVAIL;

      case NV_DATA_IPR_MODE_I:      //white 2001.03.21
          nv_cmd.data_ptr->data_ipr_mode = 1;   //115200
            break;

      case NV_DATA_REV_MODE_I:      //white 2001.03.21
          nv_cmd.data_ptr->data_rev_mode = 0;   //음성착신모드
          break;

      case NV_FLIP_COUNT_I:      //woodstok 01.5.3
          nv_cmd.data_ptr->flip_count = 0;   
          break;

      case NV_MULTI_SID_I:       //hjcho 01.09.17
          nv_cmd.data_ptr->multi_sid.nSIDNum = 0;   
          nv_cmd.data_ptr->multi_sid.nSelIndex = 0;   

          for(i = 0; i<NV_MAX_CH_LIST_SIZE; i++)
          {
              nv_cmd.data_ptr->multi_sid.awChList[i] = 0;
              nv_cmd.data_ptr->multi_sid.awSIDList[i] = 0;
          }
          break;


     default:
		  MSG_ERROR(" No default data of item (0x%x) on 1st pwr up",item,0,0);
		  bWrite = 0;
    	}
    }
    
    if(bWrite)
			status = nvimio_io(&nv_cmd, NV_WRITE_IO);
    
}