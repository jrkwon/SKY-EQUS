/*+HDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
| Modulname : ML_MEM.H                                                         |
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
/*-HDR*/


#ifndef __ML_MEM_H_
#define __ML_MEM_H_

#ifdef XSYSID
  /*
  ** If partitioning is used, then the ml_mem module 
  ** cannot be used
  */
  #error "Error_ml_mem_c_cannot_be_used_for_partitioning"
#endif /* ... XSYSID */

#ifdef XNOPROTO
#define XPP(x) ()
#else
#define XPP(x) x
#endif

#ifdef XMK_USE_SDL_MEM

  extern void   xmk_MemInit          XPP(( void *, void *)) ;
  extern size_t xmk_EvaluateExp2Size XPP(( size_t rl ));
  extern void * xmk_Malloc           XPP(( size_t )) ;
  extern void   xmk_Free             XPP(( void *)) ;
  extern void * xmk_Calloc           XPP(( size_t, size_t )) ;
  
  #ifdef XMK_USE_memshrink
    extern void  xmk_Memshrink       XPP(( void *, size_t )) ;
  #endif

  #if defined(XMK_ADD_PROFILE) && defined(XMK_USE_EXPANDED_KERNEL)
    extern size_t xmk_GetOccupiedMem XPP(( void ));
    extern size_t xmk_GetFreeMem     XPP(( void ));
    extern int    xmk_CleanPool      XPP(( void ));
  #endif

  #ifdef XMK_USE_memset
    /*
    ** the xmk_OPT_INT below may be defined as char only, if the value
    ** is less then 256 in any case ! 
    */
    extern void * memset      XPP(( void *, xmk_OPT_INT, size_t )) ;
  #endif

  #ifdef XMK_USE_memcpy
    extern void * memcpy      XPP(( void *, const void *, size_t )) ;
  #endif

  #ifdef XMK_USE_strlen
    extern size_t strlen      XPP((const char *));
  #endif

  #ifdef XMK_USE_strcpy
    extern char * strcpy      XPP(( char *, const char * ));
  #endif

  #ifdef XMK_USE_strcmp
    extern int strcmp         XPP((const char *, const char *));
  #endif

#endif /* ... XMK_USE_SDL_MEM */

#endif /* ... __ML_MEM_H */
