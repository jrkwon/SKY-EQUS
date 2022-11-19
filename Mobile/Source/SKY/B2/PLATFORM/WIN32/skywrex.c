/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     skyrrex.c
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
** 2000-12-04 redstar     create
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
//#include "rex.h"        /* REX services                            */
#include "hsig.h"



#include "sky.h"

WORD WIN_UI_Signal = 0x00;
WORD WIN_HS_Signal = 0x00;
#ifdef FEATURE_VAS
WORD WIN_VAS_Signal = 0x00;
#endif//feature_vas
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


void SKY_Clear_Signal(rex_tcb_type *pTcb, rex_sigs_type param)
{
	if( pTcb == &ui_tcb )
	{
		WIN_UI_Signal = WIN_UI_Signal & ~param;
	}
#ifdef FEATURE_VAS
	else if( pTcb == &vas_tcb )
	{
		WIN_VAS_Signal = WIN_VAS_Signal & ~param;
	}
#endif
}
void SKY_Set_Signal(rex_tcb_type *pTcb, rex_sigs_type param)
{
	if(  pTcb == &ui_tcb )
	{
		WIN_UI_Signal = WIN_UI_Signal | param;
	}
    else if( pTcb == &hs_tcb )
    {
        WIN_HS_Signal = WIN_HS_Signal | param;
    }
#ifdef FEATURE_VAS
    else if( pTcb == &vas_tcb )
    {
        WIN_VAS_Signal = WIN_VAS_Signal | param;
    }
#endif
    else
    {
        WIN_UI_Signal = WIN_UI_Signal | param;
    }
}

extern rex_sigs_type SKY_Wait_Signal( rex_sigs_type  sig)
{

   static int  number = 0;
   number++;
   if( number ==1 )
       return UI_KEY_SIG;
   if( sig & UI_NV_SIG  )
       return  UI_NV_SIG;
   else 	
   {
   	   while( TRUE )
   	   {
            if( sig & UI_RPT_TIMER_SIG )
            {
   	   	        if( WIN_UI_Signal & UI_RPT_TIMER_SIG )
   	   	        {
                    WIN_UI_Signal = WIN_UI_Signal & ~UI_RPT_TIMER_SIG;
   	   	    	    return UI_RPT_TIMER_SIG;
   	   	        }
            }

            if( sig & UI_TIMERS_SIG )
            {
   	   	        if( WIN_UI_Signal & UI_TIMERS_SIG )
   	   	        {
                    WIN_UI_Signal = WIN_UI_Signal & ~UI_TIMERS_SIG;
   	   	    	    return UI_TIMERS_SIG;
   	   	        }
            }

            if( sig & UI_KEY_SIG )
            {
   	   	        if( WIN_UI_Signal & UI_KEY_SIG )
   	   	        {
                    WIN_UI_Signal = WIN_UI_Signal & ~UI_KEY_SIG;
   	   	    	    return UI_KEY_SIG;
   	   	        }
            }
            if( sig & UI_CMD_Q_SIG )
            {
   	   	        if( WIN_UI_Signal & UI_CMD_Q_SIG )
   	   	        {
                    WIN_UI_Signal = WIN_UI_Signal & ~UI_CMD_Q_SIG;
   	   	    	    return UI_CMD_Q_SIG;
   	   	        }
            }
            if( sig & UI_MULTI_STOP_SIG )
            {
   	   	        if( WIN_UI_Signal & UI_MULTI_STOP_SIG )
   	   	        {
                    WIN_UI_Signal = WIN_UI_Signal & ~UI_MULTI_STOP_SIG;
   	   	    	    return UI_MULTI_STOP_SIG;
   	   	        }
            }
            if( sig & UI_RING_SIG )
            {
   	   	        if( WIN_UI_Signal & UI_RING_SIG )
   	   	        {
                    WIN_UI_Signal = WIN_UI_Signal & ~UI_RING_SIG;
   	   	    	    return UI_RING_SIG;
   	   	        }
            }
   	   	}
   	}   	  
}

extern rex_sigs_type SKY_Wait_HSignal( rex_sigs_type  sig)
{

   while( TRUE )
   {

        if( sig & HS_RPT_TIMER_SIG )
        {
            if( WIN_HS_Signal & HS_RPT_TIMER_SIG )
            {
                WIN_HS_Signal = WIN_HS_Signal & ~HS_RPT_TIMER_SIG;
                return HS_RPT_TIMER_SIG;
            }
        }
        if( sig & HS_CMD_Q_SIG )
        {
            if( WIN_HS_Signal & HS_CMD_Q_SIG )
            {
                WIN_HS_Signal = WIN_HS_Signal & ~HS_CMD_Q_SIG;
                return HS_CMD_Q_SIG;
            }
        }
        if( sig & HS_GPIO_TIMER_SIG )
        {
            if( WIN_HS_Signal & HS_GPIO_TIMER_SIG )
            {
                WIN_HS_Signal = WIN_HS_Signal & ~HS_GPIO_TIMER_SIG;
                return HS_GPIO_TIMER_SIG;
            }
        }
   	}
 	  
}

#ifdef FEATURE_VAS
extern rex_sigs_type SKY_Wait_VASSignal( rex_sigs_type  sig)
{

   while( TRUE )
   {
        if( sig & VAS_RPT_TIMER_SIG )
        {
            if( WIN_VAS_Signal & VAS_RPT_TIMER_SIG )
            {
                WIN_VAS_Signal = WIN_VAS_Signal & ~VAS_RPT_TIMER_SIG;
                return VAS_RPT_TIMER_SIG;
            }
        }
        if( sig & VAS_CMD_Q_SIG )
        {
            if( WIN_VAS_Signal & VAS_CMD_Q_SIG )
            {
                WIN_VAS_Signal = WIN_VAS_Signal & ~VAS_CMD_Q_SIG;
                return VAS_CMD_Q_SIG;
            }
        }

		if( sig & SOCKET_CB_SIG )
		{ 
			if( WIN_VAS_Signal & SOCKET_CB_SIG )
			{
                WIN_VAS_Signal = WIN_VAS_Signal & ~SOCKET_CB_SIG;
                return SOCKET_CB_SIG;
			}
		}

		if( sig & NETWORK_CB_SIG )
		{ 
			if( WIN_VAS_Signal & NETWORK_CB_SIG )
			{
                WIN_VAS_Signal = WIN_VAS_Signal & ~NETWORK_CB_SIG;
                return NETWORK_CB_SIG;
			}
		}

		if( sig & VAS_EVT_PROCESS_SIG )
		{ 
			if( WIN_VAS_Signal & VAS_EVT_PROCESS_SIG )
			{
                WIN_VAS_Signal = WIN_VAS_Signal & ~VAS_EVT_PROCESS_SIG;
                return VAS_EVT_PROCESS_SIG;
			}
		}

		if( sig & VAS_TIMER_PROCESS_SIG1 )
		{ 
			if( WIN_VAS_Signal & VAS_TIMER_PROCESS_SIG1 )
			{
                WIN_VAS_Signal = WIN_VAS_Signal & ~VAS_TIMER_PROCESS_SIG1;
                return VAS_TIMER_PROCESS_SIG1;
			}
		}
		if( sig & VAS_TIMER_PROCESS_SIG2 )
		{ 
			if( WIN_VAS_Signal & VAS_TIMER_PROCESS_SIG2 )
			{
                WIN_VAS_Signal = WIN_VAS_Signal & ~VAS_TIMER_PROCESS_SIG2;
                return VAS_TIMER_PROCESS_SIG2;
			}
		}
		if( sig & VAS_TIMER_PROCESS_SIG3 )
		{ 
			if( WIN_VAS_Signal & VAS_TIMER_PROCESS_SIG1 )
			{
                WIN_VAS_Signal = WIN_VAS_Signal & ~VAS_TIMER_PROCESS_SIG3;
                return VAS_TIMER_PROCESS_SIG3;
			}
		}
		if( sig & VAS_TIMER_PROCESS_SIG4 )
		{ 
			if( WIN_VAS_Signal & VAS_TIMER_PROCESS_SIG4 )
			{
                WIN_VAS_Signal = WIN_VAS_Signal & ~VAS_TIMER_PROCESS_SIG4;
                return VAS_TIMER_PROCESS_SIG4;
			}
		}

   }//while
}

extern rex_timer_cnt_type SKY_Clear_Timer(rex_timer_type *p_timer);

#endif//feature_vas


extern rex_timer_cnt_type SKY_Set_Timer
(
  rex_timer_type *p_timer,      /* pointer to timer to set */
  rex_timer_cnt_type  p_cnt     /* value to set the timer   */
)

{
   return (rex_timer_cnt_type) 1;	
	
}


extern void SKY_API       SKY_InitHS(void)
{
	
}

#ifdef FEATURE_VAS
extern void SKY_API       SKY_InitVAS(void)
{
	
}
#endif//feature_vas

void SKY_Set_Clock
(
  WORD   signal,
  clk_cb_type *call_back_ptr,
  void (*routine_ptr)( DWORD ms_interval ),
  DWORD ms_first,
  DWORD ms_periodic,
  BOOL repeat
)
{

    if( repeat == FALSE )
        SetWinTimer( routine_ptr,signal, ms_first,  repeat );
    else        
	    SetWinTimer( routine_ptr,signal, ms_periodic, repeat );
	
}
