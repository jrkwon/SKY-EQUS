/*
** ===========================================================================
**
** Project: 
**     Athens
**
** File:
**     eqa_off.c
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
** 2000-07-20 jrkwon     Created.
**
** ===========================================================================
*/



/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "comdef.h"
#include "target.h"

#include "sky.h"
#include "ska.h"
//#include "eqs.h"



/*

#include "uiuint.h"



#include "uiutxt.h"

#include "uixscrn.h"
#include "hs.h"
#include "nv.h"
*/


//#include "eqs_vari.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

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

/*
** ***************************************************************************
  ui.xxxx 

  !ui.earpieceup    earpiece down at power up
  ui.pwr = TRUE     Power up
** ***************************************************************************
*/

/*
** ***************************************************************************
  variable or function                 define               use   
  ------------------------------------ -------------------- ----------------
  ui_mc_initiated_pwrdown              uisstrt.c            uisstrt.c
                                                            uisoff.c
  ui_pwrdown_complete                  uisstrt.c            uisstrt.c
                                                            uisoff.c
  ui_power_down                        uixcm.c
** ***************************************************************************
*/

VOID SKY_API SKA_Garbage(SKY_EVENT_T *p)
{
  
   EQS_GetDC(TRUE);
   EQS_ReleaseDC();

}

/*
** jrkwon 2001-02-14
** This applet is for only using in case of being power on.
** Off state function uses both being power on and off in the 
** Qualcomm original source code.
** but, EQUS-SKY system doesn't. 
** Being power off is happened in Start up state in EQUS-SKY system.
** so, the power down related parts are useless.
*/

VOID SKY_API SKA_MainOff(SKY_EVENT_T *pEvent)
{
    STATIC BOOL fPoweringDown = FALSE;

    if( pEvent->EventID == SE_APP_END)
        return;
    // Optimization for FEATURE_LPM 
    if((pEvent->EventID == SE_KEY_POWER_OFF) && !fPoweringDown)
    {
#ifdef FEATURE_OTASP
        /* if MC initiated power down, we do not need to tell MC again */
        // jrkwon 2001-02-14 if ( ui_mc_initiated_pwrdown == FALSE ) 
        if(g_fMCInitiatedPowerDown == FALSE)
        {
#ifdef FEATURE_LPM
#error code not present
#endif
            SKY_PowerOff(POM_DOWN);//ui_power_down(); 
            
            fPoweringDown = TRUE;
        } 
        else 
        {
            // jrkwon 2001-02-14 ui_mc_initiated_pwrdown = FALSE ; /* clear flag */
            g_fMCInitiatedPowerDown = FALSE; /* clear flag */
        }
        // jrkwon 2001-02-14 ui_pwrdown_complete = TRUE ; /* we can ack to TASK_STOP_SIG */
        g_fPowerDownComplete = TRUE;

#else  /* FEATURE_OTASP */

#ifdef FEATURE_LPM
#error code not present
#endif


     SKY_PowerOff(POM_DOWN);//ui_power_down();

     fPoweringDown = TRUE;

#endif  /* FEATURE_OTASP */
    }

    if ( fPoweringDown ) 
    {
        // the phone is powering down, ignore all events
        /* jrkwon 2000-07-20 --> no meaning to EQUS */
        // ui.event = UI_NONE_F;
        /* <-- */

        EQS_END();
        return;
    }

    
    //SKY_SET_GETKEY(TRUE);  //ui_get_keys( TRUE );            /* accept keys */
    SKY_AcceptKey(TRUE); /* accept keys */

#ifdef FEATURE_UI_SNI
    /* jrkwon 2000-07-20 --> do not use uiscrn_xxx */
    //uiscrn_ann( HS_AN_SNI, 0 );     /* Turn off SNI LED */
    /* <-- */
#endif

    /* check EQE_KEY_END event at SK SKY */
    if (pEvent->EventID == SE_KEY_END) 
    {
#ifdef FEATURE_HOLD_PWRKEY_TO_PWRUP
#error code not present
#endif /* FEATURE_HOLD_PWRKEY_TO_PWRUP */

        /* jrkwon 2000-07-20 --> SKY doesn't use UI_LOCKED_S */
        //if ( !ui.earpieceup )
        //{         /* earpiece down at power up */
        //    SKA_RootLocked(pEvent);     /* Keypad locked */
        //    ui.pwr = TRUE ;
        //}
        //else
        /* <-- */
        {
            EQS_START_SIBLING((EQS_APPLET *)SKA_MainStartUp, TRUE);    /* enter start state */
			SKY_SET_POWER_ON(TRUE);//ui.pwr = TRUE;
        }
    }
    else if(pEvent->EventID == SE_KEY_EXT_POWER_ON || pEvent->EventID == SE_KEY_HANDS_FREE_ON) 
    {
#ifdef FEATURE_LPM
#error code not present
#else        
        EQS_START_SIBLING((EQS_APPLET *)SKA_MainStartUp, TRUE);        /* enter start state */

        SKY_SET_POWER_ON(TRUE);//ui.pwr = TRUE;		
#endif
    }
    /* jrkwon 2000-07-20 --> There is no need to clear the event for EQUS */
    // ui.event = UI_NONE_F;
    /* <-- */
}
