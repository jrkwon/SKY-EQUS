/*
 * Copyright (C) Ericsson Mobile Communications AB, 2001.
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
#define XMK_C_TIME 995439254
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
  &yPDTBL_z00010J_PROV_UA,
  &yPDTBL_z00010K_WTA_IF,
  &yPDTBL_z00010L_CONT_UA,
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
  &yPDTBL_z58_WDPhandler,
  &yPDTBL_z59_WDPmanagement,
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
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z17_StackCtrl, z17A_MaxWait,
    Timer_without_default_value, 11)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z20C_ClientMethod, z20C09_Abort,
    Timer_without_default_value, 12)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z20E_ClientPush, z20E3_Abort,
    Timer_without_default_value, 17)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300B_Initiator, z300B14_RetryInterval,
    Timer_without_default_value, 18)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300B_Initiator, z300B15_AckInterval,
    Timer_without_default_value, 23)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300B_Initiator, z300B16_DelayTransmission,
    Timer_without_default_value, 28)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z300C_Responder, z300C0H_AckInterval,
    Timer_without_default_value, 33)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z48_WTLS_CM, z4831_ConnectionTimer,
    Timer_without_default_value, 34)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z48_WTLS_CM, z4832_OpeningTimer,
    Timer_without_default_value, 49)
  XDEFTIMER_ROM_TABLE_ENTRY(XPTID_z4A_WTLS_Main, z4A0S_maxWaitForConfirmation,
    Timer_without_default_value, 64)
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
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr,4,1)
******/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** Tables for process StackCtrl
** #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr,4,1)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*--------------Instance Data---------------*/
#define X_MAX_INST_z17_StackCtrl 1
static yVDef_z17_StackCtrl *yINSTD_z17_StackCtrl[X_MAX_INST_z17_StackCtrl];

/*--------------Process State---------------*/
static xSTATE yPSTATETBL_z17_StackCtrl[X_MAX_INST_z17_StackCtrl] = {
  XSTARTUP
};

/*---------------State Index----------------*/
static XCONST xSITBL xSTATE_INDEX_z17_StackCtrl[9] = {0, 2, 3, 5, 6, 7, 8, 9,
  10};

/*---------------Transition-----------------*/
#define XMAX_TRANS_z17_StackCtrl 10
static XCONST xTR_TABLE_ENTRY yTRTBL_z17_StackCtrl[XMAX_TRANS_z17_StackCtrl] = {

    /* IDLE */
  XMK_B z_StackMgr_0_StartWAPStack, 1 XMK_E,
  XMK_B z_StackMgr_A_TerminateWAPStack, 5 XMK_E

    /* WaitForCryptInitResponse */,
  XMK_B z_StackMgr_2_CRYPTcInitialiseResponse, 2 XMK_E

    /* AwaitReadySignals */,
  XMK_B z_StackMgr_9_Ready, 4 XMK_E,
  XMK_B z17A_MaxWait, 3 XMK_E

    /* AwaitWAEReadySignal */,
  XMK_B z_StackMgr_9_Ready, 6 XMK_E

    /* AwaitWSPReadySignal */,
  XMK_B z_StackMgr_9_Ready, 7 XMK_E

    /* AwaitWTPReadySignal */,
  XMK_B z_StackMgr_9_Ready, 8 XMK_E

    /* AwaitWTLSReadySignal */,
  XMK_B z_StackMgr_9_Ready, 9 XMK_E

    /* AwaitWDPReadySignal */,
  XMK_B z_StackMgr_9_Ready, 10 XMK_E
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
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr,4,1)
++++++*/
YPAD_FUNCTION(yPAD_z17_StackCtrl)
{
  YPAD_YSVARP
  YPAD_YVARP(yVDef_z17_StackCtrl)
  YPAD_TEMP_VARS
  /* INTEGRATION: commented out statement */
  /* SDL_Integer  yDcn_SDL_Integer; */
  /* INTEGRATION: commented out statement */
  /* SDL_Boolean  yDcn_SDL_Boolean; */
  YPRSNAME_VAR("StackCtrl")

  BEGIN_PAD
  switch (XSYMBOLNUMBER) {

/*-----
* START
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),119(40,15),1)
------*/
  case 0:
    BEGIN_START_TRANSITION
    XAT_FIRST_SYMBOL(0, 0)
    yAssF_SDL_Natural(yVarP->z17B_n, yTstA_SDL_Natural(SDL_INTEGER_LIT(0)),
      XASS);
    yAssF_SDL_Natural(yVarP->z17C_NbrOfLayers,
      yTstA_SDL_Natural(SDL_INTEGER_LIT(0)), XASS);
    xDef_SDL_Natural(&yVarP->z17D_Wait);
    xDef_SDL_Real(&yVarP->z17E_R);
    yAssF_SDL_Duration(yVarP->z17F_Diff, SDL_DURATION_LIT(1.0, 1, 0), XASS);
    xDef_SDL_Natural(&yVarP->z17G_Layer);
    yAssF_SDL_Natural(yVarP->z17H_needsCryptInit,
      yTstA_SDL_Natural(SDL_INTEGER_LIT(0)), XASS);
    xDef_SDL_PId(&yVarP->z17I_mypid);
    xDef_SDL_Integer(&yVarP->z17J_result);
    INIT_TIMER_VAR(yTim_MaxWait)
    XBETWEEN_SYMBOLS(11, 11, 298)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),122(40,30),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(IDLE, z172_IDLE, "IDLE")

/*-----
* INPUT StartWAPStack
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),128(85,40),1)
------*/
  case 1:
    XDEBUG_LABEL(IDLE_StartWAPStack)
    XAT_FIRST_SYMBOL(1, 1)
    XOS_TRACE_INPUT("StartWAPStack")
    XBETWEEN_SYMBOLS(12, 12, 315)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),677(85,55),1)
------*/
    yAssF_SDL_Natural(yVarP->z17C_NbrOfLayers,
      yTstA_SDL_Natural(SDL_INTEGER_LIT(0)), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_STMTS(13, 13, 326)

/*-----
* ASSIGNMENT n := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),677(85,55),2)
------*/
    yAssF_SDL_Natural(yVarP->z17B_n, yTstA_SDL_Natural(SDL_INTEGER_LIT(0)),
      XASS);
#ifdef XTRACE
    xTraceAssign("n := ");
#endif
    XBETWEEN_SYMBOLS(14, 14, 337)

/*-----
* TASK
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),671(85,70),1)
------*/
#ifdef XTRACE
    xTraceTask(" ");
#endif
yVarP->z17D_Wait = MaxStartUpTime;

    XBETWEEN_SYMBOLS(15, 15, 348)

/*-----
* ASSIGNMENT R := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),242(85,85),1)
------*/
    yAssF_SDL_Real(yVarP->z17E_R, xFloat_SDL_Integer(yVarP->z17D_Wait), XASS);
#ifdef XTRACE
    xTraceAssign("R := ");
#endif
    XBETWEEN_STMTS(16, 16, 358)

/*-----
* ASSIGNMENT Diff := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),242(85,85),2)
------*/
    yAssF_SDL_Duration(yVarP->z17F_Diff, xMultRD_SDL_Duration(yVarP->z17E_R,
      yVarP->z17F_Diff), XASS);
#ifdef XTRACE
    xTraceAssign("Diff := ");
#endif
    XBETWEEN_SYMBOLS(17, 17, 369)

/*-----
* TASK
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1328(85,100),1)
------*/
#ifdef XTRACE
    xTraceTask(" ");
#endif
#if defined CONFIG_WTLS || defined CAN_SIGN_TEXT
  yVarP->z17H_needsCryptInit = 1;
#endif

    XBETWEEN_SYMBOLS(18, 18, 382)

/*-----
* DECISION
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1331(85,120),1)
------*/
#ifdef XTRACE
    yAssF_SDL_Integer(yDcn_SDL_Integer, yVarP->z17H_needsCryptInit, XASS);
    if (yEqF_SDL_Integer(yDcn_SDL_Integer, SDL_INTEGER_LIT(1))) {
#else
    if (yEqF_SDL_Integer(yVarP->z17H_needsCryptInit, SDL_INTEGER_LIT(1))) {
#endif
#ifdef XTRACE
    xTraceDecision("PATH: Integer");
#endif
      XBETWEEN_SYMBOLS(19, 19, 397)
      {

/*-----
* ASSIGNMENT mypid := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1343(85,135),1)
------*/
        yAssF_SDL_PId(yVarP->z17I_mypid, SDL_SELF, XASS);
#ifdef XTRACE
        xTraceAssign("mypid := ");
#endif
      }
      XBETWEEN_SYMBOLS(20, 20, 409)

/*-----
* TASK
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1337(85,150),1)
------*/
#ifdef XTRACE
      xTraceTask(" ");
#endif
  CRYPTa_initialise (yVarP->z17I_mypid);

      XBETWEEN_SYMBOLS(21, 21, 420)

/*-----
* NEXTSTATE WaitForCryptInitResponse
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1349(85,165),1)
------*/
      XAT_LAST_SYMBOL
      SDL_NEXTSTATE(WaitForCryptInitResponse, z173_WaitForCryptInitResponse,
        "WaitForCryptInitResponse")
    } else {
#ifdef XTRACE
    xTraceDecision("ELSE");
#endif
        /* Null transition */
    }

  L_grst521:;

/*-----
* JOIN SEND_START_SIGNALS
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1649(90,200),1)
------*/
    goto L_SEND_START_SIGNALS;

/*-----
* INPUT CRYPTcInitialiseResponse
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1355(85,180),1)
------*/
  case 2:
    XDEBUG_LABEL(WaitForCryptInitResponse_CRYPTcInitialiseResponse)
    XAT_FIRST_SYMBOL(2, 2)
    XOS_TRACE_INPUT("CRYPTcInitialiseResponse")
    yAssF_SDL_PId(yVarP->z17I_mypid,
      ((yPDef_z_StackMgr_2_CRYPTcInitialiseResponse *)ySVarP)->Param1, XASS);
    yAssF_SDL_Integer(yVarP->z17J_result,
      ((yPDef_z_StackMgr_2_CRYPTcInitialiseResponse *)ySVarP)->Param2, XASS);

/*-----
* JOIN grst521
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1649(90,200),1)
------*/
    goto L_grst521;

/*-----
* INPUT MaxWait
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1502(265,55),1)
------*/
  case 3:
    XDEBUG_LABEL(AwaitReadySignals_MaxWait)
    XAT_FIRST_SYMBOL(3, 3)
    XOS_TRACE_INPUT("MaxWait")
    INPUT_TIMER_VAR(yTim_MaxWait)
    XBETWEEN_SYMBOLS(22, 22, 472)

/*-----
* OUTPUT StartResult
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1505(265,70),1)
------*/
    ALLOC_SIGNAL_PAR(StartResult, z_StackMgr_1_StartResult,
      yPDef_z_StackMgr_1_StartResult)
    yAssF_SDL_Natural(((yPDef_z_StackMgr_1_StartResult *)OUTSIGNAL_DATA_PTR)->Param1,
      yTstA_SDL_Natural(SDL_INTEGER_LIT(0)), XASSPARA);
    SDL_OUTP_PAR_ENV(xDefaultPrioSignal, StartResult, z_StackMgr_1_StartResult,
      ENV, yPDef_z_StackMgr_1_StartResult, "StartResult")
    XBETWEEN_SYMBOLS(23, 23, 484)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1511(265,160),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(IDLE, z172_IDLE, "IDLE")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1514(315,55),1)
------*/
  case 4:
    XDEBUG_LABEL(AwaitReadySignals_Ready)
    XAT_FIRST_SYMBOL(4, 4)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17G_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_9_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(24, 24, 503)

/*-----
* ASSIGNMENT n := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1517(315,70),1)
------*/
    yAssF_SDL_Natural(yVarP->z17B_n, yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->
      z17B_n, SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("n := ");
#endif
    XBETWEEN_SYMBOLS(25, 25, 514)

/*-----
* DECISION
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1520(315,85),1)
------*/
#ifdef XTRACE
    yAssF_SDL_Boolean(yDcn_SDL_Boolean, yEqF_SDL_Integer(yVarP->z17B_n, yVarP->
      z17C_NbrOfLayers), XASS);
    if (yDcn_SDL_Boolean) {
#else
    if (yEqF_SDL_Integer(yVarP->z17B_n, yVarP->z17C_NbrOfLayers)) {
#endif
#ifdef XTRACE
    xTraceDecision("TRUE");
#endif
      XBETWEEN_SYMBOLS(26, 26, 530)

/*-----
* OUTPUT StartResult
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1523(315,130),1)
------*/
      ALLOC_SIGNAL_PAR(StartResult, z_StackMgr_1_StartResult,
        yPDef_z_StackMgr_1_StartResult)
      yAssF_SDL_Natural(((yPDef_z_StackMgr_1_StartResult *)OUTSIGNAL_DATA_PTR)->Param1,
        yTstA_SDL_Natural(SDL_INTEGER_LIT(1)), XASSPARA);
      SDL_OUTP_PAR_ENV(xDefaultPrioSignal, StartResult,
        z_StackMgr_1_StartResult, ENV, yPDef_z_StackMgr_1_StartResult,
        "StartResult")
      XBETWEEN_SYMBOLS(27, 27, 543)

/*-----
* RESET MaxWait
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1529(315,145),1)
------*/
      SDL_RESET(MaxWait, z17A_MaxWait, yTim_MaxWait, "MaxWait")
      XBETWEEN_SYMBOLS(28, 28, 550)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1532(315,160),1)
------*/
      XAT_LAST_SYMBOL
      SDL_NEXTSTATE(IDLE, z172_IDLE, "IDLE")
    } else {
#ifdef XTRACE
    xTraceDecision("FALSE");
#endif
      XBETWEEN_SYMBOLS(29, 29, 562)

/*-----
* NEXTSTATE -
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),1535(290,160),1)
------*/
      XAT_LAST_SYMBOL
      SDL_DASH_NEXTSTATE
    }

/*-----
* INPUT TerminateWAPStack
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1361(30,50),1)
------*/
  case 5:
    XDEBUG_LABEL(IDLE_TerminateWAPStack)
    XAT_FIRST_SYMBOL(5, 5)
    XOS_TRACE_INPUT("TerminateWAPStack")
    XBETWEEN_SYMBOLS(42, 42, 580)

/*-----
* OUTPUT TerminateWAE
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1364(30,65),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWAE, z_StackMgr_B_TerminateWAE,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWAE, z_StackMgr_B_TerminateWAE,
      TO_PROCESS(WAE_ME, z00008_WAE_ME), 0, "TerminateWAE")
    XBETWEEN_SYMBOLS(43, 43, 590)

/*-----
* NEXTSTATE AwaitWAEReadySignal
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1367(30,80),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWAEReadySignal, z175_AwaitWAEReadySignal,
      "AwaitWAEReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1385(30,95),1)
------*/
  case 6:
    XDEBUG_LABEL(AwaitWAEReadySignal_Ready)
    XAT_FIRST_SYMBOL(6, 6)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17G_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_9_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(44, 44, 610)

/*-----
* OUTPUT TerminateWSP
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1388(30,110),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWSP, z_StackMgr_C_TerminateWSP,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWSP, z_StackMgr_C_TerminateWSP,
      TO_PROCESS(WSP_ME, z20F_WSP_ME), 0, "TerminateWSP")
    XBETWEEN_SYMBOLS(45, 45, 620)

/*-----
* NEXTSTATE AwaitWSPReadySignal
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1391(30,125),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWSPReadySignal, z176_AwaitWSPReadySignal,
      "AwaitWSPReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1406(30,140),1)
------*/
  case 7:
    XDEBUG_LABEL(AwaitWSPReadySignal_Ready)
    XAT_FIRST_SYMBOL(7, 7)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17G_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_9_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(46, 46, 640)

/*-----
* OUTPUT TerminateWTP
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1409(30,155),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWTP, z_StackMgr_D_TerminateWTP,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWTP, z_StackMgr_D_TerminateWTP,
      TO_PROCESS(MEProcess, z3014_MEProcess), 0, "TerminateWTP")
    XBETWEEN_SYMBOLS(47, 47, 650)

/*-----
* NEXTSTATE AwaitWTPReadySignal
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1412(30,170),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWTPReadySignal, z177_AwaitWTPReadySignal,
      "AwaitWTPReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1583(105,45),1)
------*/
  case 8:
    XDEBUG_LABEL(AwaitWTPReadySignal_Ready)
    XAT_FIRST_SYMBOL(8, 8)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17G_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_9_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(48, 48, 670)

/*-----
* OUTPUT TerminateWTLS
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1586(105,60),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWTLS, z_StackMgr_E_TerminateWTLS,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWTLS,
      z_StackMgr_E_TerminateWTLS, TO_PROCESS(WTLS_ME, z49_WTLS_ME), 0,
      "TerminateWTLS")
    XBETWEEN_SYMBOLS(49, 49, 681)

/*-----
* NEXTSTATE AwaitWTLSReadySignal
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1589(105,75),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWTLSReadySignal, z178_AwaitWTLSReadySignal,
      "AwaitWTLSReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1604(105,95),1)
------*/
  case 9:
    XDEBUG_LABEL(AwaitWTLSReadySignal_Ready)
    XAT_FIRST_SYMBOL(9, 9)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17G_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_9_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(50, 50, 701)

/*-----
* OUTPUT TerminateWDP
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1607(105,110),1)
------*/
    ALLOC_SIGNAL_NPAR(TerminateWDP, z_StackMgr_F_TerminateWDP,
      XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, TerminateWDP, z_StackMgr_F_TerminateWDP,
      TO_PROCESS(WDPmanagement, z59_WDPmanagement), 0, "TerminateWDP")
    XBETWEEN_SYMBOLS(51, 51, 711)

/*-----
* NEXTSTATE AwaitWDPReadySignal
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1610(105,125),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitWDPReadySignal, z179_AwaitWDPReadySignal,
      "AwaitWDPReadySignal")

/*-----
* INPUT Ready
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1628(105,140),1)
------*/
  case 10:
    XDEBUG_LABEL(AwaitWDPReadySignal_Ready)
    XAT_FIRST_SYMBOL(10, 10)
    XOS_TRACE_INPUT("Ready")
    yAssF_SDL_Natural(yVarP->z17G_Layer, yTstA_SDL_Natural(
      ((yPDef_z_StackMgr_9_Ready *)ySVarP)->Param1), XASS);
    XBETWEEN_SYMBOLS(52, 52, 731)

/*-----
* DECISION
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1670(105,155),1)
------*/
#ifdef XTRACE
    yAssF_SDL_Integer(yDcn_SDL_Integer, yVarP->z17H_needsCryptInit, XASS);
    if (yEqF_SDL_Integer(yDcn_SDL_Integer, SDL_INTEGER_LIT(1))) {
#else
    if (yEqF_SDL_Integer(yVarP->z17H_needsCryptInit, SDL_INTEGER_LIT(1))) {
#endif
#ifdef XTRACE
    xTraceDecision("PATH: Integer");
#endif
      XBETWEEN_SYMBOLS(53, 53, 746)

/*-----
* TASK
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1673(105,175),1)
------*/
#ifdef XTRACE
      xTraceTask(" ");
#endif
CRYPTa_terminate ();

    } else {
#ifdef XTRACE
    xTraceDecision("ELSE");
#endif
        /* Null transition */
    }
    XBETWEEN_SYMBOLS(54, 54, 763)

/*-----
* TASK
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1631(105,190),1)
------*/
#ifdef XTRACE
    xTraceTask(" ");
#endif
TerminationDone();

    XBETWEEN_SYMBOLS(55, 55, 774)

/*-----
* NEXTSTATE IDLE
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Terminate),1634(105,205),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(IDLE, z172_IDLE, "IDLE")

  L_SEND_START_SIGNALS:
    XBETWEEN_SYMBOLS(30, 30, 784)

/*-----
* OUTPUT StartWAE
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),134(195,45),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWAE, z_StackMgr_3_StartWAE, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWAE, z_StackMgr_3_StartWAE,
      TO_PROCESS(WAE_ME, z00008_WAE_ME), 0, "StartWAE")
    XBETWEEN_SYMBOLS(31, 31, 793)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),176(195,60),1)
------*/
    yAssF_SDL_Natural(yVarP->z17C_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z17C_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(32, 32, 805)

/*-----
* OUTPUT StartWSP
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),140(195,75),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWSP, z_StackMgr_4_StartWSP, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWSP, z_StackMgr_4_StartWSP,
      TO_PROCESS(WSP_ME, z20F_WSP_ME), 0, "StartWSP")
    XBETWEEN_SYMBOLS(33, 33, 814)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),185(195,90),1)
------*/
    yAssF_SDL_Natural(yVarP->z17C_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z17C_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(34, 34, 826)

/*-----
* OUTPUT StartWTP
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),878(195,105),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWTP, z_StackMgr_5_StartWTP, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWTP, z_StackMgr_5_StartWTP,
      TO_PROCESS(MEProcess, z3014_MEProcess), 0, "StartWTP")
    XBETWEEN_SYMBOLS(35, 35, 835)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),881(195,120),1)
------*/
    yAssF_SDL_Natural(yVarP->z17C_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z17C_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(36, 36, 847)

/*-----
* OUTPUT StartWTLS
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),746(195,135),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWTLS, z_StackMgr_6_StartWTLS, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWTLS, z_StackMgr_6_StartWTLS,
      TO_PROCESS(WTLS_ME, z49_WTLS_ME), 0, "StartWTLS")
    XBETWEEN_SYMBOLS(37, 37, 856)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),749(195,150),1)
------*/
    yAssF_SDL_Natural(yVarP->z17C_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z17C_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(38, 38, 868)

/*-----
* OUTPUT StartWDP
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),146(195,165),1)
------*/
    ALLOC_SIGNAL_NPAR(StartWDP, z_StackMgr_7_StartWDP, XSIGNALHEADERTYPE)
    SDL_OUTP_NPAR(xDefaultPrioSignal, StartWDP, z_StackMgr_7_StartWDP,
      TO_PROCESS(WDPmanagement, z59_WDPmanagement), 0, "StartWDP")
    XBETWEEN_SYMBOLS(39, 39, 877)

/*-----
* ASSIGNMENT NbrOfLayers := ...
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),191(195,180),1)
------*/
    yAssF_SDL_Natural(yVarP->z17C_NbrOfLayers,
      yTstA_SDL_Natural(xPlus_SDL_Integer(yVarP->z17C_NbrOfLayers,
      SDL_INTEGER_LIT(1))), XASS);
#ifdef XTRACE
    xTraceAssign("NbrOfLayers := ");
#endif
    XBETWEEN_SYMBOLS(40, 40, 889)

/*-----
* SET MaxWait
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),251(195,195),1)
------*/
    SDL_SET_DUR(xPlus_SDL_Time(SDL_NOW, yVarP->z17F_Diff), yVarP->z17F_Diff,
      MaxWait, z17A_MaxWait, yTim_MaxWait, "MaxWait")
    XBETWEEN_SYMBOLS(41, 41, 897)

/*-----
* NEXTSTATE AwaitReadySignals
* #SDTREF(SDL,D:\WIP\Maintenance\4.x\System\SystemConfigurations\base_push_wta_wtls_co_korea\sdl\stackmgr\stackctrl.spr(Startup),158(195,210),1)
------*/
    XAT_LAST_SYMBOL
    SDL_NEXTSTATE(AwaitReadySignals, z174_AwaitReadySignals,
      "AwaitReadySignals")
  }
  END_PAD(yPAD_z17_StackCtrl)
}

extern void yInit XPP((void))
{
  extern void yInit_z0_WAE XPP((void)); /* INTEGRATION: added declaration */
  extern void yInit_z2_WSP XPP((void)); /* INTEGRATION: added declaration */
  extern void yInit_z3_WTPProvider XPP((void)); /* INTEGRATION: added declaration */
  yInit_WTLS();
  yInit_WTLS_WDP();
  yInit_WDPpckg();
  yInit_WDP_UDP();
  yInit_WTPpckg();
  yInit_WTP_WDP();
  yInit_WDP_UpperLayer();
  yInit_WSPWTP_WTLS();
  yInit_WSPConnectionMode_WTP();
  yInit_WSPConnectionLess_WDP();
  yInit_WTAPackage();
  yInit_PushPackage();
  yInit_WAE_WSP();
  yInit_Global_Declarations();
  yInit_WAEPackage();
  yInit_z0_WAE();
  yInit_z2_WSP();
  yInit_z3_WTPProvider();
}
