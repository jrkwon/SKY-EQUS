/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_key.c
**
** Description: 
**     @
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-11-20   redstar    modify
** 00-11-06   jrkwon     re-created
** 00-10-12   redstar    created
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#include "target.h"
#include "comdef.h"
#include "customer.h"
#include "mod.h"
#include "rxc.h"

#include "ska.h"


//extern BOOL SKY_IsInCall(void);
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/* limit continuous DTMF time reporting to two seconds */
#define MAX_DTMF_RPT_TIME 2000
/* Key press burst DTMF length in milliseconds */
#define BURST_DTMF_LEN 190 //95

#define PWR_OFF_CNT 2

/* Key held down repeat time in ms - 1st repeat, then subsequent repeats */
#define KEY_REPEAT_TIME  1600// 1000 //hjcho 01.05.07 - 녹음/재생에서 재생 이동시 
                                     //Key event가 두번 씩 발생하는 것 방지
#define KEY_NATE_REPEAT_TIME 1000
#define KEY_REPEAT_TIME2 300

#define NO_ONETOUCH_TIME 2500      /* 2.5 sec timeout err msg for no one touch dial */
#define AUTO_CURSOR_TIME 1500      /* 1.5 second auto cursor timeout */

#define TONE_DURATION_IN_MS            100 /* tone duration in millisecond */
/* Enforce a minimum length for the key beep, Use 200 msec */
#define MAX_TONE_DURATION_IN_MS        200 


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

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
extern UINT8 g_nKeyFlag;
#endif  //FEATURE_SKT_EIF 


/* Keep track of which key is held down, if any */
SKY_EVENT_E g_KeyDownKey = SE_NONE;
INT8        g_nPowerOffCount  = PWR_OFF_CNT;    /* count down for power off */

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
STATIC DWORD l_dwNeedStopDTMF;
/* key down time calculation. time-of-day in millisecond */
STATIC QWORD l_qwTime;      

/*
**---------------------------------------------------------------------------
**  Local Function Declaration
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Local Function Definitions
**---------------------------------------------------------------------------
*/


/*===========================================================================
FUNCTION SKY_KeyUp

DESCRIPTION
  Fake a key up in case we need to play a sound or multitone even while a
  key is pressed down.

DEPENDENCIES
  It won't cause any harm if there is no key down.

RETURN VALUE
  Nothing

SIDE EFFECTS
  Stops key tone, if any.
  Sends continuous DTMF stop command to MC if needed.

===========================================================================*/
void SKY_API SKY_KeyUp(void)
{
    DWORD dwNeedStopDTMF;
    
    /*
    ** If there is an outstanding MC_START_CONT_DTMF_F, end it with
    ** a MC_STOP_CONT_DTMF_F
    */
    dwNeedStopDTMF = l_dwNeedStopDTMF; 
    if( dwNeedStopDTMF != 0 )   /* stop continuous DTMF */
    {                              
        clk_read_ms( l_qwTime );
        l_dwNeedStopDTMF = qw_lo( l_qwTime ) - dwNeedStopDTMF;
        if( l_dwNeedStopDTMF  > MAX_DTMF_RPT_TIME ) 
        {
            l_dwNeedStopDTMF = MAX_DTMF_RPT_TIME;
        }

        if (ui_stop_cont_dtmf((WORD)l_dwNeedStopDTMF))/* command sent OK */ 
        {
            l_dwNeedStopDTMF = 0;
        }
        else 
        {
            l_dwNeedStopDTMF = dwNeedStopDTMF;
        }         
    }  

    SKY_DeregKeyCB();
    UI_ENABLE_SLEEP();       /* okay to sleep */

    /* If there is a key down... */
    if( GET_KEY_DOWN_KEY() != SE_NONE ) 
    { 
        /* ui.burst is always TRUE in the SKY */
        //if(ui.burst == FALSE) 
        //{             /* And it's not a burst DTMF... */      
        //SKY_StopTone();
        //}           
        SET_KEY_DOWN_KEY( SE_NONE );  /* There is no key down now */  
    }
}


/*
**---------------------------------------------------------------------------
**  Global Function Definitions
**---------------------------------------------------------------------------
*/


/*===========================================================================
FUNCTION  SKY_HandleKeys

DESCRIPTION
  Handle a key pressed.

DEPENDENCIES
  REX is running.  NV and DIAG and HS are running.

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void SKY_EmptyKey(void);

void SKY_API SKY_HandleKeys(void)
{
//    STATIC WORD s_wTime;            /* for key beep */
    STATIC QWORD s_qwKeyToneTime;   /* time key went down */
	UINT8 nKeyCount = 0;            //01.06.24 redstar

    SKY_EVENT_E  KeyCode;           /* hold key temporarily */    
    QWORD qwKeyToneTime2;
    DWORD dwNeedStopDTMF;
    NV_ITEM_U nv_data;
    
   
    while((KeyCode = SKY_GetKey()) != SE_KEY_NONE)
    {
        SKY_DeregKeyCB();        
        
        /* Display key press information for debugging */
        if ( KeyCode != SE_KEY_RELEASE ) 
        {
            SKY_TRACE1(TL_HIGH, "Key pressed = %d", KeyCode);
        } 

		nKeyCount++;
		if( SKY_IS_IN_WAP() )
		{
			if( nKeyCount >= 6 )
			{
				SKY_DeregKeyCB();
				nKeyCount = 0;
				SKY_EmptyKey();
				break;
			}
		}
		else
		{
			if( nKeyCount >= 16 )
			{
				SKY_DeregKeyCB();
				nKeyCount = 0;
				SKY_EmptyKey();
				break;
			}
		}

		//SKY_DeregKeyCB();
        switch( KeyCode ) 
        {
        case SE_KEY_EXT_POWER_ON:
            SKY_TRACE(TL_HIGH, "EXT POWER ON");
            SKY_SET_EXTERNAL_POWER(TRUE);            
            //ui_backlight_which();           
            SKY_SetupBacklight();  /* set up backlight */
            break;
            
        case SE_KEY_EXT_POWER_OFF:
            SKY_TRACE(TL_HIGH, "EXTERNAL POWER OFF");

            /* Don't power down if phone yanked out of handyphone */
            /* ui.handyphone is always FALSE .. do not need to check */
// 원본 Qualcomm Source는 아래부분이 첨가 되어 있슴.
// 그러나, 핸즈프리 크래들 상에서 외부 전원 오프 시에도 폰의 전원 상태를 유지하시 위해서        
/*
            if ( SKY_IS_HANDS_FREE()  
                 && SKY_IS_EXTERNAL_POWER() && !SKY_IsInCall()) 
            {
                // we are in cradle ( i.e attached to car kit )  
                // There is no call up. We just lost external    
                // power. Can it be that ignition is turned off ?
                SKY_DoEvent( SE_SYS_TO_IDLE );     // back to idle    
                SKY_DoEvent( SE_KEY_POWER_OFF );   // lets power down    
            }
*/
            SKY_SET_EXTERNAL_POWER(FALSE);
            SKY_SET_CHARGING(FALSE);   
            
            SKY_SET_NO_OFF_BACKLIGHT(FALSE);
            SKY_BacklightOff();
            SKY_SubBacklightOff();
            SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            if(SKY_GET_DATAMANAGER())
            {
                DmSendEndDM();
                SKY_SET_DATAMANAGER(FALSE);
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            
            //SKY_SetupBacklight();        /* set up backlight*/  
            break;

        case SE_KEY_CHARGING_ON:                 /* Charging on    */
            SKY_TRACE(TL_HIGH, "CHARGING ON");
            SKY_SET_CHARGING(TRUE);
            break;
        case SE_KEY_CHARGING_OFF:                /* Charging off   */
            SKY_TRACE(TL_HIGH, "CHARGING OFF");
            SKY_SET_CHARGING(FALSE);
            //uiscrn_batt( NO_BATT, FALSE );     /* turn battery icon off */
            break;
                     
        case SE_KEY_HANDS_FREE_OFF:            /* Phone off hook */
            SKY_TRACE(TL_HIGH, "PHONE OFF HOOK-Phone was lifed from hands-free cradle");
            SKY_SET_HANDS_FREE(FALSE);
//            SKY_SET_EXTERNAL_POWER(FALSE);
            /* change only device setting, not mute setting */
            SKY_SetSoundDevice(SND_MUTE_MAX, SND_MUTE_MAX, FALSE);
            /*
            if((SKY_IsInCall() == FALSE) || ui_curr_call_is_sms())
                SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
            else
                SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
            */
            SKY_SET_NO_OFF_BACKLIGHT(FALSE);
            SKY_BacklightOff();
            SKY_SubBacklightOff();
            SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            //SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            break;

        case SE_KEY_HANDS_FREE_ON:         /* Hands free mode */
            SKY_TRACE(TL_HIGH, "PHONE ON HOOK-Phone was placed in hands-free cradle");
            SKY_SET_HANDS_FREE(TRUE);
            SKY_SET_EXTERNAL_POWER(TRUE);
            /* change only device setting, not mute setting */
            SKY_SetSoundDevice(SND_MUTE_MAX, SND_MUTE_MAX, FALSE);
            /*if(SKY_IsInCall() == FALSE)
                SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
            else
                SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
            */
            SKY_SET_NO_OFF_BACKLIGHT(TRUE);
            SKY_BacklightOn(AB_ALWAYS);
            SKY_SubBacklightOn(AB_ALWAYS);
            break;

        case SE_KEY_EAR_JACK_ON:
            SKY_SET_EAR_JACK_ON();
            /* change only device setting, not mute setting */
            SKY_SetSoundDevice(SND_MUTE_MAX, SND_MUTE_MAX, FALSE);
            break;

        case SE_KEY_EAR_JACK_OFF:
            SKY_SET_EAR_JACK_OFF();
            /* change only device setting, not mute setting */
            SKY_SetSoundDevice(SND_MUTE_MAX, SND_MUTE_MAX, FALSE);
            break;

#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)       
        case SE_KEY_CAMERA_JACK_ON:
            SKY_SET_CAMERA_ON();
            break;

        case SE_KEY_CAMERA_JACK_OFF:
            SKY_SET_CAMERA_OFF();
            break;
#endif
        case SE_KEY_PHONE_OPEN:
            /* flip or folder is open */
            SKY_SET_PHONE_OPEN();
            /* UI_BKLT_WHEN_EARPC_UP */
            SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());    //white 2001.01.05.
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());          //white 2001.06.13

            if((SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS) || SKY_IS_ALERT())
            {
                SKY_Backlight( TRUE );   //redstar
            }
            break;

        case SE_KEY_PHONE_CLOSE:
            if(SKY_GetNV(NV_FLIP_COUNT_I, &nv_data) == NV_DONE_S)
            {
                nv_data.flip_count += 1;
                SKY_PutNV(NV_FLIP_COUNT_I, &nv_data);
            }

            /* flip or folder is close */
            SKY_SET_PHONE_CLOSE();
            /* UI_BKLT_WHEN_EARPC_UP */

            /* always turn off the backlight when folder is closed */
            //if(SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS)
            //{
            SKY_Backlight( FALSE );
            /* jrkwon 2001.06.28 --> don't turn on sub lcd backlight while being shut down */
            if(SKY_IS_SHUT_DOWN() == FALSE)
                SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT()); //white 2001.01.05.
            //}
            /*
            ** when it's calling, originating, alerting, 
            ** the ear jack is put, ignore PHONE_CLOSE event 
            */
#if 0
            if(SKY_IS_EAR_JACK_ON()
                && (SKY_IsInCall() || SKY_IS_ORIGINATE() || SKY_IS_ALERT()))
            {
            //     EQS_SET_EVENT(SE_NONE);
            }
            else
            {
                /* go to idle */          
                SKY_AddEvent( SE_SYS_TO_IDLE );
            }
#endif
            /*
            if (!(ui_missed_calls && ui.idling) || !ui_curr_call_is_sms()) 
            {
                // add TOIDLE event so not all states need to process earpiece
                // down event (if not displaying missed call or sms call)		  
                ui_add_event( (word)UI_TOIDLE_F );
            }
            */
            break;
        case SE_KEY_RELEASE:      /* Key released */

            if( SKY_IS_POWER_ON() == FALSE )
            {
				// by jnlee 01.03.13	
				// power on시 키가 눌려 있으면 sleep안들어가는것 수정
	            UI_ENABLE_SLEEP(); /* okay to sleep */
                break;
            }                      
            /* start backlight countdown time */
            if((SKY_GET_AUTO_BACKLIGHT() != AB_NO)//== AB_TIME) 
                && SKY_IS_PHONE_OPEN() && (SKY_IS_ALERT() == FALSE))
            {
                if(!SKY_GET_IGNORE_BACKLIGHT())
                    SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            }            

            //SET_KEY_DOWN_KEY( SE_KEY_NONE );                       

            SKY_DeregKeyCB();   /* no need to repeat */
            UI_ENABLE_SLEEP(); /* okay to sleep */

            /* stop current continuous DTMF */
            dwNeedStopDTMF = l_dwNeedStopDTMF;
            if( dwNeedStopDTMF != 0 ) 
            {                   
                clk_read_ms( l_qwTime );
                l_dwNeedStopDTMF = qw_lo(l_qwTime) - l_dwNeedStopDTMF;
                if(l_dwNeedStopDTMF > MAX_DTMF_RPT_TIME)
                {
                    l_dwNeedStopDTMF = MAX_DTMF_RPT_TIME;
                }
                /* command sent OK */
                if (SKY_StopContinuosDTMF((WORD)l_dwNeedStopDTMF))
                {
                    l_dwNeedStopDTMF = 0;
                }
                else 
                {
                    l_dwNeedStopDTMF = dwNeedStopDTMF;
                }
            }          

            /* If there is a key down, and we're not in burst DTMF mode, */
            /* Enforce a minimum length for the key beep. Use 200 msec.  */
            if( GET_KEY_DOWN_KEY() != SE_NONE ) 
            {
//#if 0 /* 00000000000000000000000000000000000000000000000000000000000000 */
                /* ui.burst is always TRUE in the SKY system */
                clk_read_ms( qwKeyToneTime2 );
#ifndef WIN32
                if( qw_hi( qwKeyToneTime2 ) - qw_lo( s_qwKeyToneTime ) < MAX_TONE_DURATION_IN_MS ) 
                {
                    if(GET_KEY_DOWN_TONE() != SND_FIRST_TONE)
                    { 
//                        SKY_PlayTone(UI_BEEP_SND, GET_KEY_DOWN_DTMF(), TONE_DURATION_IN_MS);
                    } 
                    else if(GET_KEY_DOWN_DTMF() != SND_FIRST_TONE)
                    {
                        SKY_PlayDTMF(GET_KEY_DOWN_DTMF(), TONE_DURATION_IN_MS);
                    }              

                } 
//Deleted by happykid in 2001.02.22
//                else 
  //              {
    //                /* We've played long enough, shut up */
      //              SKY_StopTone();
        //        }              
#endif
//#endif /* 00000000000000000000000000000000000000000000000000000000000000 */
                /* We've played long enough, shut up */
          //      SKY_StopTone();


				if( GET_KEY_DOWN_KEY() == SE_KEY_F2) // redstar 2001.03.22 for key  ntop - gvm 
				{
					KeyCode = SE_KEY_F2;
				}
				else if( GET_KEY_DOWN_KEY() == SE_KEY_F1) // redstar 2001.10.04 for wavelet
				{
					KeyCode = SE_KEY_F1;
				}				
				else if( GET_KEY_DOWN_KEY() == SE_KEY_AUTO_ANSWER )
				{
					KeyCode = SE_KEY_RELEASE_AUTO_ANSWER;
				}
				else ;

                SET_KEY_DOWN_KEY( SE_NONE );    /* no key down */
            }

            SET_KEY_DOWN_DTMF(SND_FIRST_TONE);
            SET_KEY_DOWN_TONE(SND_FIRST_TONE);
            break;
       
        case SE_KEY_0:
        case SE_KEY_1:
        case SE_KEY_2:
        case SE_KEY_3:
        case SE_KEY_4:
        case SE_KEY_5:
        case SE_KEY_6:
        case SE_KEY_7:
        case SE_KEY_8:
        case SE_KEY_9:
            /* fall through  */
            /* handle all other "normal" keys */
        default:
            if(SKY_IS_PHONE_CLOSE())
            {
                /* these keys are active in the phone is closed */
                if(KeyCode != SE_KEY_MANNERS && KeyCode != SE_KEY_RECORD 
                    && KeyCode != SE_KEY_PLAY && KeyCode != SE_KEY_AUTO_ANSWER
                    && KeyCode != SE_KEY_EAR_CLICK && KeyCode != SE_KEY_EAR_LONG_CLICK
                    && KeyCode != SE_KEY_EAR_DOUBLE_CLICK)
                {
                    KeyCode = SE_KEY_RELEASE;            
                    break;
                }
                /* these keys make sub lcd backlight on */
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
                else if(KeyCode == SE_KEY_RECORD || KeyCode == SE_KEY_AUTO_ANSWER)
#elif SKY_MODEL_ID == SKY_MODEL_B0
                else if(KeyCode == SE_KEY_RECORD || KeyCode == SE_KEY_AUTO_ANSWER
                        || KeyCode == SE_KEY_PLAY)
#endif
                {
                    /*
                    ** jrkwon : 2001-02-27 --> 
                    ** even if phone is closed, turn on sub LCD backlight
                    */
                    SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
                    /* <-- */
                }
            }

#if 0 /* 00000000000000000000000000000000000000000000000000000000000 */
            if(SKY_IS_PHONE_OPEN() == FALSE && SKY_IS_POWER_ON())
            {
                KeyCode = SE_KEY_RELEASE;            
                break;
            }
#endif /* 00000000000000000000000000000000000000000000000000000000000 */
            if(SKY_GET_AUTO_BACKLIGHT() != AB_NO 
                //&& SKY_GET_AUTO_BACKLIGHT() != AB_10SECOND
                && SKY_IS_PHONE_OPEN() && SKY_IS_ALERT() == FALSE)
            {
                if(!SKY_GET_IGNORE_BACKLIGHT())
                    SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
            }

            /* one touch dialing is always TRUE in SKY */
            
            /* the SKY use the HS_END_K as HS_PWR_KEY by pressing down */
            /* if HS_END_K is pressed for a while, that means HS_PWR_K */
            if((SKY_IS_POWER_ON() == FALSE && KeyCode == SE_KEY_END) == FALSE)
            //if((SKY_IS_POWER_ON() == FALSE && KeyCode == SE_KEY_POWER) == FALSE)
            {				
				/* 최초 착발신 이력 진입을 위해 up key를 누를 경우에 전화부 검색 시간으로   */
				/* 인해 phone에서는 long 키로 인식. scroll list가 두 칸씩 움직이는 경우     */
				/* 발생. 이를 방지하기 위해 시간을 up/down key에 한해서 long key로 인식하는 */
				/* 시간을 두 배로 늘임   --> redstar  01.04.09                      
				*/					
				if( KeyCode == SE_KEY_UP || KeyCode == SE_KEY_DOWN ||\
					KeyCode == SE_KEY_RECORD  || KeyCode == SE_KEY_AUTO_ANSWER ||\
					KeyCode == SE_KEY_PLAY )
				{
					SKY_RegKeyCB( (int4)KEY_REPEAT_TIME, (int4)KEY_REPEAT_TIME2, TRUE );					
				}
				else
				{					
					SKY_RegKeyCB( (int4)KEY_NATE_REPEAT_TIME, (int4)KEY_REPEAT_TIME2, TRUE );
				}
            }            
            UI_DISABLE_SLEEP(); /* not okay to sleep */

          
            SET_KEY_DOWN_KEY( KeyCode );        /* mark key as held down */

#ifndef WIN32
			if( KeyCode == SE_KEY_F2 || KeyCode == SE_KEY_F1)   // redstar 2001.10.04
			{
				KeyCode = SE_KEY_RELEASE;
			}			
#endif
                          
            SET_POWER_OFF_COUNT( PWR_OFF_CNT );
                                                   /* before the send key held down event occurs */                 
            /* Not yet making any sound */
            SET_KEY_DOWN_DTMF(SND_FIRST_TONE);
            SET_KEY_DOWN_TONE(SND_FIRST_TONE);

            clk_read_ms( s_qwKeyToneTime ); /* get time of start */

            /* ui.burst is always TRUE in SKY */
            //if( KeyCode == SE_KEY_END)
            //{          
            //    s_wTime = BURST_DTMF_LEN;             /* bursts last only 95 ms */          
            //} 
            //else 
            //{
            //    s_wTime = 0;                          /* continuous are forever */          
            //}
            /* Now play the appopriate sound for this key */
#ifndef WIN32
            //SKY_PlayKeySound(KeyCode, s_wTime);
            SKY_PlayKeySound(KeyCode, BURST_DTMF_LEN);
#endif

            break;
        }  /* switch key */

        /* Now put the key on our own circular queue for eventual */
        /* processing by the state machine                        */

        if( KeyCode != SE_KEY_RELEASE )
        {         
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
            if( g_nKeyFlag == 2 )
            {		
                ds_cmd_type	ds_cmd_item;				// item to hold cmd to dsmgr
		        extern	rex_sigs_type dsi_requested_sig_mask;

		        ds_cmd_item.cmd_name = DS_EIF_KEY_SCAN;
		        ds_cmd_item.parms.srv_opt = KeyCode;
		        ds_cmd(&ds_cmd_item);
		        dsi_requested_sig_mask |= DS_CMD_Q_SIG;		       
            }
#endif  //FEATURE_SKT_EIF

            SKY_AddEvent( KeyCode );
        }
    }  /* Got a key from handset */

    if(SKY_IS_POWER_ON() == FALSE)
    {
        /* handset task missed the POWER key */
        if(SKY_IS_EXTERNAL_POWER() == FALSE)
        {  
            //SKY_DoEvent( SE_KEY_END );
            SKY_DoEvent( SE_KEY_EXT_POWER_ON );
        }
/*
        if(SKY_IS_EXTERNAL_POWER() == FALSE)
        {  
            //SKY_DoEvent( SE_KEY_END );
            SKY_DoEvent( SE_KEY_POWER );
        }
        else 
        {
            SKY_DoEvent( SE_KEY_EXT_POWER_ON );
        }
*/
    } 
}

