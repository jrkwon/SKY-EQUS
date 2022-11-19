/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Headerfile : MK_QUEU.H                                                      |
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



#ifndef __MK_QUEU_H_
#define __MK_QUEU_H_

#ifdef XSYSID
  /*
  ** If partitioning is used, then redefine function names
  ** to make them unique in the whole system (over
  ** all partitions)
  */
  #define xmk_InitQueue                       XMAKENAME(XSYSID,InitQueue)
  #define xmk_FirstSignal                     XMAKENAME(XSYSID,FirstSignal)
  #define xmk_NextSignal                      XMAKENAME(XSYSID,NextSignal)
  #define xmk_InsertSignal                    XMAKENAME(XSYSID,InsertSignal)
  #define xmk_RemoveCurrentSignal             XMAKENAME(XSYSID,RemoveCurrentSignal)
  #define xmk_RemoveSignalBySignalID          XMAKENAME(XSYSID,RemoveSignalBySignalID)
  #define xmk_RemoveSignalsByProcessID        XMAKENAME(XSYSID,RemoveSignalsByProcessID)
  #define xmk_AllocSignal                     XMAKENAME(XSYSID,AllocSignal)
  #define xmk_FreeSignal                      XMAKENAME(XSYSID,FreeSignal)
  #define xmk_TestAndSetSaveState             XMAKENAME(XSYSID,TestAndSetSaveState)
  #define xmk_SaveSignalsOnly                 XMAKENAME(XSYSID,SaveSignalsOnly )
  #define xmk_QueueEmpty                      XMAKENAME(XSYSID,QueueEmpty )
  #define xmk_QueryQueue                      XMAKENAME(XSYSID,QueryQueue )
#endif /* ... XSYSID */


extern void       xmk_InitQueue XPP(( void )) ;

extern xmk_T_MESSAGE xmk_RAM_ptr xmk_FirstSignal XPP(( void )) ;

extern xmk_T_MESSAGE xmk_RAM_ptr xmk_NextSignal XPP(( void )) ;

extern void       xmk_InsertSignal XPP(( xmk_T_MESSAGE xmk_RAM_ptr )) ;

extern void       xmk_RemoveCurrentSignal XPP(( void )) ;

extern void       xmk_RemoveSignalBySignalID XPP(( xmk_T_SIGNAL )) ;

extern xmk_T_MESSAGE xmk_RAM_ptr xmk_AllocSignal XPP(( void )) ;

extern void       xmk_FreeSignal XPP(( xmk_T_MESSAGE xmk_RAM_ptr)) ;

extern void           xmk_RemoveSignalsByProcessID XPP(( xPID ));

extern xmk_T_BOOL     xmk_TestAndSetSaveState XPP(( xmk_T_STATE )) ;

#if defined (XMK_USED_SAVE) && defined (XMK_USE_EXPANDED_KERNEL)

  extern xmk_T_BOOL     xmk_SaveSignalsOnly XPP(( void )) ;

#endif

/* Manual scaling */
extern xmk_T_BOOL     xmk_QueueEmpty XPP(( void )) ;


#endif
