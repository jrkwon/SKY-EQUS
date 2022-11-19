/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Headerfile : MK_OUTP.H                                                      |
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



#ifndef __MK_OUTP_H_
#define __MK_OUTP_H_

#ifdef XSYSID
  /*
  ** If partitioning is used, then redefine function names
  ** to make them unique in the whole system (over
  ** all partitions)
  */
  #define xmk_SendSimple                      XMAKENAME(XSYSID,SendSimple )
  #define xmk_Send                            XMAKENAME(XSYSID,Send)
  #define xmk_Determine_Receiver              XMAKENAME(XSYSID,Determine_Receiver )
  #define xmk_EnvSend                         XMAKENAME(XSYSID,EnvSend)
#endif /* ... XSYSID */


/*
** Definition of the Simple Output function with few parameters ...
*/
#ifdef XMK_USE_xmk_SendSimple
  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    /*
    ** Each signal contains a Receiver PID
    */
    extern void xmk_SendSimple XPP(( xmk_T_SIGNAL, xPID )) ;
  #else
    /*
    ** No signal contains a Receiver PID
    */
    extern void xmk_SendSimple XPP(( xmk_T_SIGNAL )) ;
  #endif /* ... XMK_USE_RECEIVER_PID_IN_SIGNAL */
#endif

/*
** Definition of the complex Output function with lot of parameters ...
*/
#ifdef XMK_USED_SIGNAL_WITH_PARAMS
  #define XOUTP_PARA_NON_LAST xmk_T_MESS_LENGTH,void xmk_RAM_ptr,
  #define XOUTP_PARA_LAST     xmk_T_MESS_LENGTH,void xmk_RAM_ptr
#else
  #define XOUTP_PARA_NON_LAST
  #define XOUTP_PARA_LAST
#endif

#if defined(XMK_USE_SIGNAL_PRIORITIES) && defined(XMK_USE_RECEIVER_PID_IN_SIGNAL)
  /*
  ** Each signal contains a Signal priority
  ** Each signal contains a Receiver PID
  */
  extern void xmk_Send XPP(( xmk_T_SIGNAL,
                             xmk_T_PRIO, 
                             XOUTP_PARA_NON_LAST
                             xPID ));
#endif

#if defined(XMK_USE_SIGNAL_PRIORITIES) && !defined(XMK_USE_RECEIVER_PID_IN_SIGNAL)
  /*
  ** Each signal contains a Signal priority
  ** No   signal contains a Receiver PID
  */
  extern void xmk_Send XPP(( xmk_T_SIGNAL,
                             xmk_T_PRIO,
                             XOUTP_PARA_LAST )) ;
#endif

#if !defined(XMK_USE_SIGNAL_PRIORITIES) && defined(XMK_USE_RECEIVER_PID_IN_SIGNAL)
  /*
  ** No   signal contains a Signal priority
  ** Each signal contains a Receiver PID
  */
  extern void xmk_Send XPP(( xmk_T_SIGNAL,
                             XOUTP_PARA_NON_LAST
                             xPID )) ;

#endif

#if !defined(XMK_USE_SIGNAL_PRIORITIES) && !defined(XMK_USE_RECEIVER_PID_IN_SIGNAL)

  #ifdef XMK_USED_SIGNAL_WITH_PARAMS
    /*
    ** No   signal contains a Signal priority
    ** No   signal contains a Receiver PID
    */
    extern void xmk_Send XPP(( xmk_T_SIGNAL,
                               XOUTP_PARA_LAST )) ;
  #else
    /*
    ** No   signal contains a Signal priority
    ** No   signal contains a Receiver PID
    ** No   signal contains parameters
    */
    extern void xmk_Send XPP(( xmk_T_SIGNAL ));
  #endif

#endif


#undef  XOUTP_PARA_NON_LAST
#undef  XOUTP_PARA_LAST

extern xPID xmk_Determine_Receiver XPP (( xmk_T_PROCESS ));

#endif
