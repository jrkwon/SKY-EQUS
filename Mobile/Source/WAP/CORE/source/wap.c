/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */

/* Program generated by SDT Cmicro, version 1.33 */

#ifndef XSCT_CMICRO
#define XSCT_CMICRO
#endif

#define C_MICRO_1_3
#define XMK_C_TIME 977233142
#include "ml_typw.h"
#include "ctypes.h"
#include "buffmgt.h"
#include "glbldecl.h"
#include "wdp_ul.h"
#include "wtlspckg.h"
#include "wtls_wdp.h"
#include "wdppckg.h"
#include "wdp_udp.h"
#include "wspcmwtp.h"
#include "wtppckg.h"
#include "wtp_wdp.h"
#include "sptp_tls.h"
#include "wspclwdp.h"
#include "waepckg.h"
#include "wtapckg.h"
#include "pushpckg.h"
#include "wae_wsp.h"
#include "stackmgr.h"
#include "wap.h"


/*************************************************************************
**                  #CODE directives, #BODY sections                    **
*************************************************************************/

/*************************************************************************
**                  SECTION Variables and Functions                     **
*************************************************************************/

/*************************************************************************
**  Root process table
*************************************************************************/

XCONST XPDTBL *xPDTBL[MAX_SDL_PROCESS_TYPES+1] =
{
  &yPDTBL_z00008_WAE_ME,
  &yPDTBL_z00010F_UA_ME,
  &yPDTBL_z00010G_MMI_DISP,
  &yPDTBL_z00010H_MMI_AGNT,
  &yPDTBL_z00010I_WML_UA,
  &yPDTBL_z00010J_WTA_IF,
  &yPDTBL_z00010K_CONT_UA,
  &yPDTBL_z00024_WMLS_SM,
  &yPDTBL_z00036_Push_ME,
  &yPDTBL_z00037_Push_LL,
  &yPDTBL_z000407_RepH_ME,
  &yPDTBL_z000408_RepH_Install,
  &yPDTBL_z000409_RepH_LL,
  &yPDTBL_z000508_RES_ME,
  &yPDTBL_z000509_RES_CC,
  &yPDTBL_z00050A_RES_FC,
  &yPDTBL_z000603_WSPIF_ST,
  &yPDTBL_z000604_WSPIF_SM,
  &yPDTBL_z000605_WSPIF_TR,
  &yPDTBL_z000606_WSPIF_DI,
  &yPDTBL_z17_StackCtrl,
  &yPDTBL_z20B_ClientSession,
  &yPDTBL_z20C_ClientMethod,
  &yPDTBL_z20D_ClientDispatcher,
  &yPDTBL_z20E_ClientPush,
  &yPDTBL_z20F_WSP_ME,
  &yPDTBL_z20G_ConnectionLess,
  &yPDTBL_z300A_TransactionManager,
  &yPDTBL_z300B_Initiator,
  &yPDTBL_z300C_Responder,
  &yPDTBL_z3014_MEProcess,
  &yPDTBL_z48_WTLS_CM,
  &yPDTBL_z49_WTLS_ME,
  &yPDTBL_z4A_WTLS_Main,
  &yPDTBL_z4B_WTLS_Record,
  &yPDTBL_z59_WDPhandler,
  &yPDTBL_z5A_WDPmanagement,
  X_END_ROOT_TABLE
};

/*************************************************************************
**  Symbol trace table
*************************************************************************/

#ifdef XMK_ADD_TEST_OPTIONS
XSYMTRACETBL *xSYMTRACETBL[MAX_SDL_PROCESS_TYPES+1] =
{
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  (XSYMTRACETBL_ENTRY *)  NULL,
  X_END_SYMTRACE_TABLE
};
#endif

/*************************************************************************
**  Timer table
*************************************************************************/

/*-------------Timer ROM table--------------*/

#ifdef XMK_USE_TIMER_ROM_TABLE
XCONST XTIMER_ROM_TABLE_ENTRY xTimerRomTable[MAX_SDL_TIMER_TYPES+MAXN] =
{
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z00010I_WML_UA, z00010I2B_TimerEvent,
    Timer_without_default_value, 0)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z000407_RepH_ME, z0004070R_TimerEvent,
    Timer_without_default_value, 5)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z000605_WSPIF_TR, z00060511_ReqTimeOut,
    Timer_without_default_value, 6)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z17_StackCtrl, z177_MaxWait,
    Timer_without_default_value, 11)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z20C_ClientMethod, z20C09_Abort,
    Timer_without_default_value, 12)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z20E_ClientPush, z20E3_Abort,
    Timer_without_default_value, 17)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300B_Initiator, z300B13_RetryInterval,
    Timer_without_default_value, 18)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300B_Initiator, z300B14_AckInterval,
    Timer_without_default_value, 23)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300B_Initiator, z300B15_DelayTransmission,
    Timer_without_default_value, 28)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300C_Responder, z300C0H_AckInterval,
    Timer_without_default_value, 33)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z48_WTLS_CM, z482Z_ConnectionTimer,
    Timer_without_default_value, 34)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z4A_WTLS_Main, z4A0R_maxWaitForConfirmation,
    Timer_without_default_value, 49)
  X_END_TIMER_ROM
};
#endif

/*-------------Timer RAM table--------------*/

#ifdef XMK_USE_TIMER_RAM_TABLE
XTIMER_RAM_TABLE_ENTRY xTimerRamTable[MAX_SDL_TIMER_INSTS+MAXI];
#endif

/*****
* PROCESS StackCtrl
* <<SYSTEM WAP/BLOCK Stack_Mgr>>
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr,4,1)
******/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** Tables for process StackCtrl
** #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr,4,1)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*--------------Instance Data---------------*/
#define X_MAX_INST_z17_StackCtrl 1
static yVDef_z17_StackCtrl *yINSTD_z17_StackCtrl[X_MAX_INST_z17_StackCtrl];

/*--------------Process State---------------*/
static xSTATE yPSTATETBL_z17_StackCtrl[X_MAX_INST_z17_StackCtrl] = {
  XSTARTUP
};

/*---------------State Index----------------*/
static XCONST xSITBL xSTATE_INDEX_z17_StackCtrl[8] = {0, 2, 3, 4, 5, 6, 7, 9};

/*---------------Transition-----------------*/
#define XMAX_TRANS_z17_StackCtrl 9
static XCONST xTR_TABLE_ENTRY yTRTBL_z17_StackCtrl[XMAX_TRANS_z17_StackCtrl] = {

    /* IDLE */
  XMK_B z_StackMgr_0_StartWAPStack, 1 XMK_E,
  XMK_B z_StackMgr_9_TerminateWAPStack, 2 XMK_E

    /* AwaitWAEReadySignal */,
  XMK_B z_StackMgr_8_Ready, 3 XMK_E

    /* AwaitWSPReadySignal */,
  XMK_B z_StackMgr_8_Ready, 4 XMK_E

    /* AwaitWTPReadySignal */,
  XMK_B z_StackMgr_8_Ready, 5 XMK_E

    /* AwaitWTLSReadySignal */,
  XMK_B z_StackMgr_8_Ready, 6 XMK_E

    /* AwaitWDPReadySignal */,
  XMK_B z_StackMgr_8_Ready, 7 XMK_E

    /* AwaitReadySignals */,
  XMK_B z_StackMgr_8_Ready, 9 XMK_E,
  XMK_B z177_MaxWait, 8 XMK_E
};

/*-----------Process-PID-Values-------------*/
#ifdef XMK_USE_PID_ADDRESSING
static xPIDTable yPID_TBL_z17_StackCtrl[X_MAX_INST_z17_StackCtrl];
#endif

/*-----------Process Description------------*/
#define X_PRIO_z17_StackCtrl  xDefaultPrioProcess
XCONST XPDTBL yPDTBL_z17_StackCtrl = {
#ifdef XMK_USE_PID_ADDRESSING
  yPID_TBL_z17_StackCtrl,
#endif
  (xINSTD *)yINSTD_z17_StackCtrl,
  sizeof(yVDef_z17_StackCtrl),
  X_MAX_INST_z17_StackCtrl,
#ifdef XMK_USE_TIMESLICE
  X_TIMESLICE_z17_StackCtrl,
#endif
#ifdef XMK_USE_PREEMPTIVE
  X_PRIO_z17_StackCtrl,
#endif
  (xmk_T_TRANS_ADDRESS) yPAD_z17_StackCtrl,
  yTRTBL_z17_StackCtrl,
  xSTATE_INDEX_z17_StackCtrl,
  yPSTATETBL_z17_StackCtrl
};

/*+++++
* Function for process StackCtrl
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr,4,1)
++++++*/
YPAD_FUNCTION(yPAD_z17_StackCtrl)
{
  YPAD_YSVARP
  YPAD_YVARP(yVDef_z17_StackCtrl)
  YPAD_TEMP_VARS
  /* INTEGRATION: commented out statement */
  /* SDL_Boolean  yDcn_SDL_Boolean; */
  YPRSNAME_VAR("StackCtrl")

  BEGIN_PAD
  switch (XSYMBOLNUMBER) {

/*-----
* START
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),119(5,25),1)
------*/
  case 0:
    BEGIN_START_TRANSITION
    XAT_FIRST_SYMBOL(0, 0)
    yAssF_SDL_Natural(yVarP->z178_n, yTstA_SDL_Natural(SDL_INTEGER_LIT(0)),
      XASS);
    yAssF_SDL_Natural(yVarP->z179_NbrOfLayers,
      yTstA_SDL_Natural(SDL_INTEGER_LIT(0)), XASS);
    xDef_SDL_Natural(&yVarP->z17A_Wait);
    xDef_SDL_Real(&yVarP->z17B_R);
    yAssF_SDL_Duration(yVarP->z17C_Diff, SDL_DURATION_LIT(1.0, 1, 0), XASS);
    xDef_SDL_Natural(&yVarP->z17D_Layer);
    INIT_TIMER_VAR(yTim_MaxWait)
    XBETWEEN_SYMBOLS(10, 10, 285)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),122(5,40),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(IDLE, z170_IDLE, "IDLE")

/*-----
* INPUT StartWAPStack
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),128(55,40),1)
------*/
  case 1:
    XDEBUG_LABEL(IDLE_StartWAPStack)
    XAT_FIRST_SYMBOL(1, 1)
    XOS_TRACE_INPUT("StartWAPStack")
    XBETWEEN_SYMBOLS(11, 11, 302)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),677(55,55),1)
------*/
    yAssF_SDL_Natural(yVarP->z179_NbrOfLayers,
      yTstA_SDL_Natural(SDL_INTEGER_LIT(0)), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_STMTS(12, 12, 313)

/*-----
* ASSIGNMENT n := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),677(55,55),2)
------*/
    yAssF_SDL_Natural(yVarP->z178_n, yTstA_SDL_Natural(SDL_INTEGER_LIT(0)),
      XASS);
#ifdef XTRACE
    xTraceAssign("n := ");
#endif
    XBETWEEN_SYMBOLS(13, 13, 324)

/*-----
* TASK
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),671(55,70),1)
------*/
#ifdef XTRACE
    xTraceTask(" ");
#endif
yVarP->z17A_Wait = MaxStartUpTime;

    XBETWEEN_SYMBOLS(14, 14, 335)

/*-----
* ASSIGNMENT R := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),242(55,85),1)
------*/
    yAssF_SDL_Real(yVarP->z17B_R, xFloat_SDL_Integer(yVarP->z17A_Wait), XASS);
#ifdef XTRACE
    xTraceAssign("R := ");
#endif
    XBETWEEN_STMTS(15, 15, 345)

/*-----
* ASSIGNMENT Diff := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),242(55,85),2)
------*/
    yAssF_SDL_Duration(yVarP->z17C_Diff, xMultRD_SDL_Duration(yVarP->z17B_R,
      yVarP->z17C_Diff), XASS);
#ifdef XTRACE
    xTraceAssign("Diff := ");
#endif
    XBETWEEN_SYMBOLS(16, 16, 356)

/*-----
* OUTPUT StartWAE
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),134(55,100),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWAE, z_StackMgr_2_StartWAE, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWAE, z_StackMgr_2_StartWAE,
      TO_PROCESS(WAE_ME, z00008_WAE_ME), 0, "StartWAE")
    XBETWEEN_SYMBOLS(17, 17, 365)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),176(55,115),1)
------*/
    yAssF_SDL_Natural(yVarP->z179_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z179_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(18, 18, 377)

/*-----
* OUTPUT StartWSP
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),140(55,130),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWSP, z_StackMgr_3_StartWSP, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWSP, z_StackMgr_3_StartWSP,
      TO_PROCESS(WSP_ME, z20F_WSP_ME), 0, "StartWSP")
    XBETWEEN_SYMBOLS(19, 19, 386)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),185(55,145),1)
------*/
    yAssF_SDL_Natural(yVarP->z179_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z179_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(20, 20, 398)

/*-----
* OUTPUT StartWTP
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),878(55,160),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWTP, z_StackMgr_4_StartWTP, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWTP, z_StackMgr_4_StartWTP,
      TO_PROCESS(MEProcess, z3014_MEProcess), 0, "StartWTP")
    XBETWEEN_SYMBOLS(21, 21, 407)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),881(55,175),1)
------*/
    yAssF_SDL_Natural(yVarP->z179_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z179_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(22, 22, 419)

/*-----
* OUTPUT StartWTLS
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),746(55,190),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWTLS, z_StackMgr_5_StartWTLS, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWTLS, z_StackMgr_5_StartWTLS,
      TO_PROCESS(WTLS_ME, z49_WTLS_ME), 0, "StartWTLS")
    XBETWEEN_SYMBOLS(23, 23, 428)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),749(55,205),1)
------*/
    yAssF_SDL_Natural(yVarP->z179_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z179_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(24, 24, 440)

/*-----
* OUTPUT StartWDP
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),146(55,220),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWDP, z_StackMgr_6_StartWDP, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWDP, z_StackMgr_6_StartWDP,
      TO_PROCESS(WDPmanagement, z5A_WDPmanagement), 0, "StartWDP")
    XBETWEEN_SYMBOLS(25, 25, 449)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),191(55,235),1)
------*/
    yAssF_SDL_Natural(yVarP->z179_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z179_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(26, 26, 461)

/*-----
* SET MaxWait
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),251(55,280),1)
------*/
    SDL_SET_DUR(xPlus_SDL_Time(SDL_NOW, yVarP->z17C_Diff), yVarP->z17C_Diff,
      MaxWait, z177_MaxWait, yTim_MaxWait, "MaxWait")
    XBETWEEN_SYMBOLS(27, 27, 469)

/*-----
* NEXTSTATE AwaitReadySignals
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),158(55,295),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitReadySignals, z176_AwaitReadySignals,
      "AwaitReadySignals")

/*-----
* INPUT TerminateWAPStack
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),401(155,40),1)
------*/
  case 2:
    XDEBUG_LABEL(IDLE_TerminateWAPStack)
    XAT_FIRST_SYMBOL(2, 2)
    XOS_TRACE_INPUT("TerminateWAPStack")
    XBETWEEN_SYMBOLS(28, 28, 487)

/*-----
* OUTPUT TerminateWAE
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),476(155,55),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWAE, z_StackMgr_A_TerminateWAE,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWAE, z_StackMgr_A_TerminateWAE,
      TO_PROCESS(WAE_ME, z00008_WAE_ME), 0, "TerminateWAE")
    XBETWEEN_SYMBOLS(29, 29, 497)

/*-----
* NEXTSTATE AwaitWAEReadySignal
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),665(155,70),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWAEReadySignal, z171_AwaitWAEReadySignal,
      "AwaitWAEReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),785(205,40),1)
------*/
  case 3:
    XDEBUG_LABEL(AwaitWAEReadySignal_Ready)
    XAT_FIRST_SYMBOL(3, 3)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17D_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_8_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(30, 30, 517)

/*-----
* OUTPUT TerminateWSP
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),791(205,55),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWSP, z_StackMgr_B_TerminateWSP,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWSP, z_StackMgr_B_TerminateWSP,
      TO_PROCESS(WSP_ME, z20F_WSP_ME), 0, "TerminateWSP")
    XBETWEEN_SYMBOLS(31, 31, 527)

/*-----
* NEXTSTATE AwaitWSPReadySignal
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),815(205,70),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWSPReadySignal, z172_AwaitWSPReadySignal,
      "AwaitWSPReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),905(255,40),1)
------*/
  case 4:
    XDEBUG_LABEL(AwaitWSPReadySignal_Ready)
    XAT_FIRST_SYMBOL(4, 4)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17D_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_8_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(32, 32, 547)

/*-----
* OUTPUT TerminateWTP
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),914(255,55),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWTP, z_StackMgr_C_TerminateWTP,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWTP, z_StackMgr_C_TerminateWTP,
      TO_PROCESS(MEProcess, z3014_MEProcess), 0, "TerminateWTP")
    XBETWEEN_SYMBOLS(33, 33, 557)

/*-----
* NEXTSTATE AwaitWTPReadySignal
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),938(255,70),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWTPReadySignal, z173_AwaitWTPReadySignal,
      "AwaitWTPReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1205(305,40),1)
------*/
  case 5:
    XDEBUG_LABEL(AwaitWTPReadySignal_Ready)
    XAT_FIRST_SYMBOL(5, 5)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17D_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_8_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(34, 34, 577)

/*-----
* OUTPUT TerminateWTLS
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),995(305,55),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWTLS, z_StackMgr_D_TerminateWTLS,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWTLS,
      z_StackMgr_D_TerminateWTLS, TO_PROCESS(WTLS_ME, z49_WTLS_ME), 0,
      "TerminateWTLS")
    XBETWEEN_SYMBOLS(35, 35, 588)

/*-----
* NEXTSTATE AwaitWTLSReadySignal
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1013(305,70),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWTLSReadySignal, z174_AwaitWTLSReadySignal,
      "AwaitWTLSReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1055(205,145),1)
------*/
  case 6:
    XDEBUG_LABEL(AwaitWTLSReadySignal_Ready)
    XAT_FIRST_SYMBOL(6, 6)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17D_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_8_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(36, 36, 608)

/*-----
* OUTPUT TerminateWDP
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1076(205,160),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWDP, z_StackMgr_E_TerminateWDP,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWDP, z_StackMgr_E_TerminateWDP,
      TO_PROCESS(WDPmanagement, z5A_WDPmanagement), 0, "TerminateWDP")
    XBETWEEN_SYMBOLS(37, 37, 618)

/*-----
* NEXTSTATE AwaitWDPReadySignal
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1088(205,175),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWDPReadySignal, z175_AwaitWDPReadySignal,
      "AwaitWDPReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1130(255,145),1)
------*/
  case 7:
    XDEBUG_LABEL(AwaitWDPReadySignal_Ready)
    XAT_FIRST_SYMBOL(7, 7)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17D_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_8_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(38, 38, 638)

/*-----
* TASK
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1250(255,160),1)
------*/
#ifdef XTRACE
    xTraceTask(" ");
#endif
TerminationDone();

    XBETWEEN_SYMBOLS(39, 39, 649)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(1),1253(255,175),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(IDLE, z170_IDLE, "IDLE")

/*-----
* INPUT MaxWait
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),326(5,40),1)
------*/
  case 8:
    XDEBUG_LABEL(AwaitReadySignals_MaxWait)
    XAT_FIRST_SYMBOL(8, 8)
    XOS_TRACE_INPUT("MaxWait")
    INPUT_TIMER_VAR(yTim_MaxWait)
    XBETWEEN_SYMBOLS(40, 40, 667)

/*-----
* OUTPUT StartResult
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),353(5,55),1)
------*/
    ALLOC_SIGNAL_PAR(StartResult, z_StackMgr_1_StartResult,
      yPDef_z_StackMgr_1_StartResult)
    yAssF_SDL_Natural(((yPDef_z_StackMgr_1_StartResult *)OUTSIGNAL_DATA_PTR)->Param1,
      yTstA_SDL_Natural(SDL_INTEGER_LIT(0)), XASSPARA);
    SDL_OUTP_PAR_ENV(xDefaultPrioSignal, StartResult, z_StackMgr_1_StartResult,
      ENV, yPDef_z_StackMgr_1_StartResult, "StartResult")
    XBETWEEN_SYMBOLS(41, 41, 679)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),323(5,145),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(IDLE, z170_IDLE, "IDLE")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),347(55,40),1)
------*/
  case 9:
    XDEBUG_LABEL(AwaitReadySignals_Ready)
    XAT_FIRST_SYMBOL(9, 9)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17D_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_8_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(42, 42, 698)

/*-----
* ASSIGNMENT n := ...
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),338(55,55),1)
------*/
    yAssF_SDL_Natural(yVarP->z178_n, yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->
      z178_n, SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("n := ");
#endif
    XBETWEEN_SYMBOLS(43, 43, 709)

/*-----
* DECISION
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),341(55,70),1)
------*/
#ifdef XTRACE
    yAssF_SDL_Boolean(yDcn_SDL_Boolean, yEqF_SDL_Integer(yVarP->z178_n, yVarP->
      z179_NbrOfLayers), XASS);
    if (yDcn_SDL_Boolean) {
#else
    if (yEqF_SDL_Integer(yVarP->z178_n, yVarP->z179_NbrOfLayers)) {
#endif
#ifdef XTRACE
    xTraceDecision("TRUE");
#endif
      XBETWEEN_SYMBOLS(44, 44, 725)

/*-----
* OUTPUT StartResult
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),332(55,115),1)
------*/
      ALLOC_SIGNAL_PAR(StartResult, z_StackMgr_1_StartResult,
        yPDef_z_StackMgr_1_StartResult)
      yAssF_SDL_Natural(((yPDef_z_StackMgr_1_StartResult *)OUTSIGNAL_DATA_PTR)->Param1,
        yTstA_SDL_Natural(SDL_INTEGER_LIT(1)), XASSPARA);
      SDL_OUTP_PAR_ENV(xDefaultPrioSignal, StartResult,
        z_StackMgr_1_StartResult, ENV, yPDef_z_StackMgr_1_StartResult,
        "StartResult")
      XBETWEEN_SYMBOLS(45, 45, 738)

/*-----
* RESET MaxWait
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),314(55,130),1)
------*/
      SDL_RESET(MaxWait, z177_MaxWait, yTim_MaxWait, "MaxWait")
      XBETWEEN_SYMBOLS(46, 46, 745)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),329(55,145),1)
------*/
      XAT_LAST_SYMBOL
      SDL_NEXTSTATE(IDLE, z170_IDLE, "IDLE")
    } else {
#ifdef XTRACE
    xTraceDecision("FALSE");
#endif
      XBETWEEN_SYMBOLS(47, 47, 757)

/*-----
* NEXTSTATE -
* #SDTREF(SDL,D:\WIP\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(2),335(30,145),1)
------*/
      XAT_LAST_SYMBOL
      SDL_DASH_NEXTSTATE
    }
  }
  END_PAD(yPAD_z17_StackCtrl)
}

extern void yInit XPP((void))
{
  extern void yInit_z0_WAE XPP((void)); /* INTEGRATION: added declaration */
  extern void yInit_z2_WSP XPP((void)); /* INTEGRATION: added declaration */
  extern void yInit_z3_WTPProvider XPP((void)); /* INTEGRATION: added declaration */
  yInit_WTAPackage();
  yInit_PushPackage();
  yInit_WAEPackage();
  yInit_z0_WAE();
  yInit_z2_WSP();
  yInit_z3_WTPProvider();
}
