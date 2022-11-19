/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Headerfile : MK_MAIN.H                                                      |
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



#ifndef __MK_MAIN_H_
#define __MK_MAIN_H_

#ifdef XSYSID
  /*
  ** If partitioning is used, then redefine function names
  ** to make them unique in the whole system (over
  ** all partitions)
  */
  #define xmk_InitSDL                         XMAKENAME(XSYSID,InitSDL)
  #define xmk_RunSDL                          XMAKENAME(XSYSID,RunSDL )
  #define xmk_MicroTesterInit                 XMAKENAME(XSYSID,MicroTesterInit)
  #define xmk_MicroTesterDeinit               XMAKENAME(XSYSID,MicroTesterDeinit)
#endif /* ... XSYSID */


#ifdef XMK_USE_HIGH_OPTIMIZATION

  extern void xmk_InitSDL   XPP(( void ));
  extern void xmk_RunSDL    XPP(( void ));

#else

  extern void xmk_InitSDL   XPP(( void ));
  extern int  xmk_RunSDL    XPP(( unsigned char ));

#endif /* ... XMK_USE_HIGH_OPTIMIZATION */

  extern void xmk_MicroTesterInit   XPP(( void ));
  extern void xmk_MicroTesterDeinit XPP(( void ));

#endif
