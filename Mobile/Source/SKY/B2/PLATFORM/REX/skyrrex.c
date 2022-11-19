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
#include "sky.h"
#include "sky_hs.h"

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
	
(void) rex_clr_sigs( pTcb , param );	
	
}
void SKY_Set_Signal(rex_tcb_type *pTcb, rex_sigs_type param)
{
	(void) rex_set_sigs( pTcb, param );
}

extern rex_sigs_type SKY_Wait_Signal( rex_sigs_type  sig)
{

    return rex_wait( sig );

}

extern rex_sigs_type SKY_Wait_HSignal( rex_sigs_type  sig)
{

    return rex_wait( sig );

}

#ifdef FEATURE_VAS
extern rex_sigs_type SKY_Wait_VASSignal( rex_sigs_type  sig)



{

    return rex_wait( sig );

}


rex_timer_cnt_type SKY_Clear_Timer
(
  rex_timer_type *p_timer      /* pointer to timer to set */
)
{
	return rex_clr_timer( p_timer);
}
#endif//feature_vas

rex_timer_cnt_type SKY_Set_Timer
(
  rex_timer_type *p_timer,      /* pointer to timer to set */
  rex_timer_cnt_type  p_cnt     /* value to set the timer   */
)
{
	return rex_set_timer( p_timer, p_cnt);
}
	
void SKY_API       SKY_InitHS(void)
{
	InitHS();
}

void SKY_Set_Clock
(
  WORD   signal,
  clk_cb_type *call_back_ptr,
  void (*routine_ptr)( int4 ms_interval ),
  int4 ms_first,
  int4 ms_periodic,
  BOOL repeat
)
{
	clk_reg( call_back_ptr, routine_ptr, ms_first, ms_periodic, repeat);
	
}

#ifdef	FEATURE_WAVELET	//	khkeim 01/01/17
void SKY_Timed_Wait	
(
	WORD	nMsTime
)
{
#ifdef	FEATURE_WAVELET_OFFLINE	//	khekim 01/01/17
	extern	rex_timer_type  ui_rpt_timer;

	// Wait for nMsTime (millisend unit
	(void)rex_timed_wait(UI_RPT_TIMER_SIG, &ui_rpt_timer, nMsTime);
#else	//	#ifdef	FEATURE_WAVELET_OFFLINE
	// Wait for nMsTime (millisend unit
//	(void)rex_timed_wait(SOCK_RPT_TIMER_SIG, &sock_rpt_timer, nMsTime);
#endif	//	#ifdef	FEATURE_WAVELET_OFFLINE
}
#endif	//	#ifdef	FEATURE_WAVELET
