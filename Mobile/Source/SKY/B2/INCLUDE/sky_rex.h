/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     sky_rex.h
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
** 2000-12-04 redstar     Created.
**
** ============================================================================
*/

#ifndef _SKY_REX_H_
#define _SKY_REX_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
#ifdef	FEATURE_WAVELET	//	khkeim 01/01/17
extern void SKY_Timed_Wait(WORD	nMsTime);
#endif	//	#ifdef	FEATURE_WAVELET

extern void SKY_Clear_Signal(rex_tcb_type *pTcp, rex_sigs_type param);

extern void SKY_Set_Signal(rex_tcb_type *pTcp, rex_sigs_type param);

extern rex_sigs_type SKY_Wait_Signal( rex_sigs_type  sig);
extern rex_sigs_type SKY_Wait_HSignal( rex_sigs_type  sig);

#ifdef FEATURE_VAS
extern rex_sigs_type SKY_Wait_VASSignal( rex_sigs_type  sig);
extern rex_timer_cnt_type SKY_Clear_Timer(rex_timer_type *p_timer);
#endif//feature_vas

extern rex_timer_cnt_type SKY_Set_Timer
(
  rex_timer_type *p_timer,      /* pointer to timer to set */
  rex_timer_cnt_type  p_cnt     /* value to set the timer   */
);


void SKY_Set_Clock
(
  WORD   signal,
  clk_cb_type *call_back_ptr,
  void (*routine_ptr)(int4 ms_interval ),
  int4 ms_first,
  int4 ms_periodic,
  BOOL repeat
);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_REX_H_ */
