/*+MHDR*/
/*
# clearcase: CmicroPatchRel2.3.3
+------------------------------------------------------------------------------+
|  Headerfile : MK_SCHE.H                                                      |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Copyright by Telelogic AB 1993 - 1998                                       |
|  Copyright by S&P Media GmbH Germany 1993 - 1998                             |
|                                                                              |
|  This Program is owned by Telelogic and is protected by national             |
|  copyright laws and international copyright treaties. Telelogic              |
|  grants you the right to use this Program on one computer or in              |
|  one local computer network at any one time.                                 |
|  Under this License you may only modify the source code for the purpose      |
|  of adapting it to your environment. You must reproduce and include          |
|  any copyright and trademark notices on all copies of the source code.       |
|  You may not use, copy, merge, modify or transfer the Program except as      |
|  provided in this License.                                                   |
|  Telelogic does not warrant that the Program will meet your                  |
|  requirements or that the operation of the Program will be                   |
|  uninterrupted and error free. You are solely responsible that the           |
|  selection of the Program and the modification of the source code            |
|  will achieve your intended results and that the results are actually        |
|  obtained.                                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-MHDR*/



#ifndef __MK_SCHE_H_
#define __MK_SCHE_H_

#ifdef XSYSID
  /*
  ** If partitioning is used, then redefine function names
  ** to make them unique in the whole system (over
  ** all partitions)
  */
  #define xmk_StartProcesses                  XMAKENAME(XSYSID,StartProcesses )
  #define xmk_ProcessSignal                   XMAKENAME(XSYSID,ProcessSignal )
  #define xmk_CreateProcess                   XMAKENAME(XSYSID,CreateProcess )
  #define xmk_HandleSystemSignal              XMAKENAME(XSYSID,HandleSystemSignal )
  #define xmk_CheckSignal                     XMAKENAME(XSYSID,CheckSignal )
  #define xmk_IsAnyProcessAlive               XMAKENAME(XSYSID,IsAnyProcessAlive )
  #define xmk_IfExist                         XMAKENAME(XSYSID,IfExist )
  #define xmk_CheckNullPointerValue           XMAKENAME(XSYSID,CheckNullPointerValue )
  #define xmk_InitPreemptionVars              XMAKENAME(XSYSID,InitPreemptionVars )
  #define xmk_DisablePreemption               XMAKENAME(XSYSID,DisablePreemption )
  #define xmk_EnablePreemption                XMAKENAME(XSYSID,EnablePreemption)
  #define xmk_FetchHighestPrioLevel           XMAKENAME(XSYSID,FetchHighestPrioLevel )
  #define xmk_CheckIfSchedule                 XMAKENAME(XSYSID,CheckIfSchedule)
  #define xmk_SwitchPrioLevel                 XMAKENAME(XSYSID,SwitchPrioLevel)
  #define xmk_KillProcess                     XMAKENAME(XSYSID,KillProcess )
  #define xmk_GetProcessState                 XMAKENAME(XSYSID,GetProcessState )
  #define xmk_SetProcessState                 XMAKENAME(XSYSID,SetProcessState )
  #define xmk_GetProcessInstanceData          XMAKENAME(XSYSID,GetProcessInstanceData )
  #define xmk_StopAllProcesses                XMAKENAME(XSYSID,StopAllProcesses )
#endif /* ... XSYSID */

#ifndef NO_GLOBAL_VARS
  extern unsigned char   xmk_InterruptsDisabled;
#endif

extern  void           xmk_StartProcesses XPP(( void )) ;

extern  void           xmk_ProcessSignal XPP(( void )) ;

extern  xmk_T_INSTANCE xmk_CreateProcess XPP(( xmk_T_PROCESS )) ;

#ifdef XMK_USE_PREEMPTIVE
extern  void            xmk_DisablePreemption XPP(( void )) ;

extern  void            xmk_EnablePreemption XPP(( void )) ;

extern  void            xmk_SwitchPrioLevel XPP(( xmk_T_PRIOLEVEL )) ;

extern  xmk_T_PRIOLEVEL xmk_FetchHighestPrioLevel XPP(( void )) ;

extern  void            xmk_InitPreemptionVars XPP(( void )) ;

extern  void            xmk_CheckIfSchedule XPP(( void )) ;
#endif

#ifndef XMK_USE_PREEMPTIVE
  #ifndef NO_GLOBAL_VARS
    extern  xSYMBOLNR xmk_SymbolNrVar ;     /* exported by Cmicro Kernel */
  #endif
#endif

#ifdef  XMK_USE_SDL_SYSTEM_STOP
  extern xmk_T_BOOL         xmk_IsAnyProcessAlive XPP (( void ));
#endif

extern   xmk_T_BOOL         xmk_KillProcess XPP ((xPID));


extern   xmk_T_BOOL         xmk_IfExist XPP (( xPID ));

#ifdef XMK_USE_MAX_ERR_CHECK
  extern   void             xmk_CheckNullPointerValue XPP (( void * ));
#endif

extern   xmk_T_STATE        xmk_GetProcessState XPP (( xPID ));

extern   xmk_OPT_INT        xmk_SetProcessState XPP (( xPID, xmk_T_STATE ));

extern   void xmk_RAM_ptr   xmk_GetProcessInstanceData XPP (( xPID ));

#endif

