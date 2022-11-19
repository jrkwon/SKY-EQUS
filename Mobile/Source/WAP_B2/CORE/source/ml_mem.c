/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Modulname    : ML_MEM.C                                                     |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  This module is a template for                                               |
|    - implementation of a dynamic memory management,                          |
|    - implementation of some string and memory functions.                     |
|                                                                              |
|  The functions for dynamic memory management from this module can be used    |
|  (from the mk_cpu module for example) if the flag XMK_USE_SDL_MEM is set     |
|  (use the BodyBuilder).                                                      |
|  The functions for string and memory from this module can be used if the     |
|  appropriate flag XMK_USE_<functionname> is set (again, use the BodyBuilder).|
|                                                                              |
|  Using the functions contained in this module makes sense or is absolutely   |
|  necessary, if :                                                             |
|                                                                              |
|  -the C-Compiler or RTOS which is to be used, does not support dynamic memory|
|   management or string and memory functions,                                 |
|                                                                              |
|  -if the functionality of dynamic memory management provided from the C      |
|   compiler or RTOS does not meet the requirements of the user.               |
|  The possible differences between C compilers/RTOS memory functions and the  |
|  memory functions of this module are                                         |
|  -the search- and maintenance policy of dynamic memory management,           |
|   the users may wish to allocate memoryblocks always of the same size, for   |
|   instance, which sometimes allows it to reduce code and increase            |
|   performance)                                                               |
|  -the testability of dynamic memory management of this module might be better|
|   because if the dynamic memory management functions of this module are used,|
|   there is a Cmicro Tester command that allows it to observe the memory      |
|   occupation and more.                                                       |
|                                                                              |
|  To offer more comfort, an additional functionality represented by the       |
|  function "xmk_Memshrink ()" is exported. An application running, possibly   |
|  doesn't know the exact size of the memory block to be allocated. It is      |
|  then possible to allocate a maximum-size-block, do some evaluations within  |
|  the application, and then, if the exact size of the memory block is known,  |
|  to "shrink down" the previously requested memory block by calling           |
|  xmk_Memshrink ().                                                           |
|                                                                              |
|  CAUTION:                                                                    |
|  ----------------                                                            |
|  -The functions xmk_Malloc/xmk_Calloc/xmk_Free/xmk_Memshrink are able to     |
|   maintain one memory pool only.                                             |
|  -The functions xmk_Malloc/xmk_Calloc/xmk_Free/xmk_Memshrink are in principle|
|   not able to handle reentrancy.                                             |
|  -The call to xmk_Malloc/xmk_Calloc and then xmk_Memshrink is to be          |
|   considered as an critical path.                                            |
|                                                                              |
|  Basic idea is, that the user provides memory and calls "xmk_MemInit()" of   |
|  this module.                                                                |
|                                                                              |
|  Scaling of this module : See headerfile and manual.                         |
|                                                                              |
|  M1 Errormessages, which are routed to the ErrorHandler                      |
|  --------------------------------------------------------                    |
|   ERR_N_INIT_SDL_MEM    - Memory was not initialized but a try to allocate   |
|                           memory occured.                                    |
|   ERR_N_MEM_PARAM       - Invalid parameter in function call                 |
|   ERR_N_MEM_NO_FREE     - No more memory available                           |
|   ERR_N_MEM_ILLMBLOCK   - A try occured to free a block that was never       |
|                           allocated.                                         |
|                                                                              |
|  M2 Exported functions of this module :                                      |
|  --------------------------------------------------------                    |
|    void   xmk_MemInit     ( )                                          *1    |
|    void * xmk_Malloc      ( )                                          *1    |
|    void * xmk_Calloc      ( )                                          *1    |
|    void   xmk_Free        ( )                                          *1    |
|    void   xmk_Memshrink   ( )                                        *1*2    |
|    size_t xmk_GetOccupiedMem ( )                                     *1*3    |
|    size_t xmk_GetFreeMem  ( )                                        *1*3    |
|    void   xmk_QueryMemory ( )                                        *1*4    |
|                                                                              |
|    size_t xmk_EvaluateExp2Size ( size_t rl )                           *5    |
|                                                                              |
|    void   memset     ( )                                               *6    |
|    void   memcpy     ( )                                               *6    |
|    int    strlen     ( )                                               *6    |
|    char * strcpy     ( )                                               *6    |
|    char * strncpy    ( )                                               *6    |
|    int    strcmp     ( )                                               *6    |
|                                                                              |
|  The above functions are conditionally compiled with :                       |
|                                                                              |
|        *1  XMK_USE_SDL_MEM                                                   |
|        *2  XMK_USE_memshrink                                                 |
|        *3  XMK_USE_EXPANDED_KERNEL                                           |
|        *4  XMK_USE_PROFILE                                                   |
|        *5  XMK_USE_MIN_BLKSIZE                                               |
|        *6  XMK_USE_<functionname>, like XMK_USE_memset                       |
|                                                                              |
|  conditionally by specifying a flag XMK_USE_<functionname>,                  |
|  i.e.XMK_USE_strcpy. Functions marked with *2 can be conditionally           |
|  compiled by specifying XMK_USE_SDL_MEM.                                     |
|  Functions marked with *3 can be conditionally compiled by specifying        |
|  XMK_USE_MIN_BLKSIZE.                                                        |
|                                                                              |
|  M3 Static functions of this module :                                        |
|  --------------------------------------------------------                    |
|    ....            _memalloc       ( ... )                                   |
|    ....            _memshrink      ( ... )                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-MHDR*/

/*
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


#ifndef __ML_MEM_C_
#define __ML_MEM_C_

/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typ.h"

/*============================================================================*/
/*-IMPORT*/

#ifdef XSYSID
  /*
  ** CAUTION !
  ** ============
  ** This module cannot be used when partitioning is used. The reason
  ** for this is that the functions of this module operate on global
  ** variables and partitioned systems may preempt each other when
  ** an Real Time Operating System is used.
  ** However, users can use this module as a basis for implementing
  ** their own memory management functions.
  */
  #error "ERROR_in_ml_mem_c_Module_cannot_be_used_for_Partitioning"
#endif /* ... XSYSID */

/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/

/*--------------------  Constants,  Macros  ----------------------------------*/

#ifndef XMK_memset_NAME
  #define XMK_memset_NAME memset
#endif

#ifndef XMK_memcpy_NAME
  #define XMK_memcpy_NAME memcpy
#endif

#ifndef XMK_strlen_NAME
  #define XMK_strlen_NAME strlen
#endif

#ifndef XMK_strcpy_NAME
  #define XMK_strcpy_NAME strcpy
#endif

#ifndef XMK_strncpy_NAME
  #define XMK_strncpy_NAME strncpy
#endif

#ifndef XMK_strcmp_NAME
  #define XMK_strcmp_NAME strcmp
#endif

#ifndef XMK_MEM_MIN_BLKSIZE
/*
** Define a minimum blocksize of 64 bytes,
** if not specified otherwise.
** 
** CAUTION !
** ----------
** Do not specify negative values, or zero.
*/
#define XMK_MEM_MIN_BLKSIZE 64
#endif

/*
** This define is essential for making the functions of this 
** module work appropriate. 
**
** Define the CPU's word size outside this module
** For example 80486 CPU does have a word size of 32Bit.
*/
#ifndef XMK_CPU_WORD_SIZE
  /*
  ** If the XMK_CPU_WORD_SIZE is not defined
  ** externally, then it gets the default values.
  */
  #if defined(ARM_THUMB)
    #define XMK_CPU_WORD_SIZE 4 /* 4 Bytes are one word */
  #elif defined(_GCC_)
    #define XMK_CPU_WORD_SIZE 8 /* 8 Bytes are one word */
  #else
    #define XMK_CPU_WORD_SIZE 1 /* 1 Bytes is one word  */
  #endif
#endif /* ... XMK_CPU_WORD_SIZE */

#ifdef XMK_MAX_MALLOC_SIZE
  #if (XMK_MAX_MALLOC_SIZE % XMK_CPU_WORD_SIZE) != 0
    #error "ERROR02_in_ml_mem_c_BufferSizeCannotBeDevidedThroughWordSize"
  #endif
#endif

#ifdef XMK_USE_MIN_BLKSIZE
  #if (XMK_MEM_MIN_BLKSIZE % XMK_CPU_WORD_SIZE) != 0
    #error "ERROR01_in_ml_mem_c_BufferSizeCannotBeDevidedThroughWordSize"
  #endif
#endif

#ifdef XMK_ADD_PRINTF_MEMORY
  /*
  ** Users may enable the use of printf within this module
  ** only if it can be ensured, that printf does not call
  ** one of the functions within this module. 
  ** 
  ** CAUTION !!
  ** ============
  ** This would introduce recursive function calls
  ** with an program crash caused by a stack overflow.
  ** Users do not enable XMK_ADD_PRINTF_MEMORY if they
  ** want to go for safe operation.
  */
  #undef XMK_ADD_PRINTF_MEMORY
#endif

/*
** Prevent collision in naming (some C compilers do define p or/and rsize)
*/
#ifdef p
#undef p
#endif

#ifdef rl
#undef rl
#endif

#ifdef rsize
#undef rsize
#endif

#ifdef p1
#undef p1
#endif

#ifdef p2
#undef p2
#endif

#ifdef c1
#undef c1
#endif

#ifdef c2
#undef c2
#endif

#ifdef next
#undef next
#endif

#ifdef prev
#undef prev
#endif

/*--------------------  Typedefinitions     ----------------------------------*/

#ifndef xmk_T_BOOL
  typedef xmk_T_BOOL ;
  #define XMK_TRUE  (1==1)
  #define XMK_FALSE (!XMK_TRUE)
#endif

#ifndef NULL
  #define NULL (void *)0L
#endif


typedef struct _T_MBLOCK
{
  struct _T_MBLOCK * next ;
  struct _T_MBLOCK * prev ;

  #if defined(XMK_ADD_PROFILE) || defined(_GCC_)
    /*
    ** Used to store the blocksize per each block 
    */
    size_t         used ; 
  #else
    xmk_T_BOOL     used ;
  #endif

  #if defined(ARM_THUMB) 
   /*
   ** this is used to align structures
   ** to word margins. A word is 32 bit.
   */
   unsigned char _filler_ [3];
  #elif defined(_GCC_)
   /*
   ** this is used to align structures
   ** to word margins. A word is 64 bits == 8 bytes on UNIX
   ** So calculate 8 plus 3 for the filler 
   */
   unsigned char _filler_ [8];
  #endif

} xmk_T_MBLOCK ;


/*--------------------  Functions    -----------------------------------------*/

#ifdef XMK_USE_SDL_MEM
  static xmk_T_MBLOCK * __memalloc  XPP(( size_t ));
  static void           __memshrink XPP(( xmk_T_MBLOCK * , size_t size ));
#endif

/*--------------------  Variables    ----------------------------------------*/

#if defined(XMK_ADD_PROFILE) || (defined(XMK_USE_DEBUGGING) && defined(XMK_ADD_CQUERY_MEM))
  #ifndef NO_GLOBAL_VARS
    /*
    ** counter for the amount of memory that is currently occupied
    */
    int xmk_cur_memory_fill;

    /*
    ** counter for the current number of blocks in the memory pool
    */
    int xmk_cur_blk_cnt;

    /*
    ** counter for the maximum number of blocks in the memory pool
    */
    int xmk_max_blk_cnt;

    /*
    ** Largest memory block that was ever requested and allocated
    */
    int xmk_largest_blk;
  #endif

  #define XMK_M_INIT_MAX \
    xmk_cur_memory_fill=0; \
    xmk_largest_blk=0; \
    xmk_cur_blk_cnt=0; \
    xmk_max_blk_cnt=0;

  #define XMK_M_SET_MAX_BLKSIZE(PAR) \
    if (PAR > xmk_largest_blk ) \
      xmk_largest_blk = PAR;

  #define XMK_M_SET_MAX_BLOCKS \
    if (xmk_cur_blk_cnt > xmk_max_blk_cnt ) \
      xmk_max_blk_cnt = xmk_cur_blk_cnt;

  #define XMK_M_INCR_ACT \
    xmk_cur_blk_cnt ++;

  #define XMK_M_DECR_ACT \
    if (xmk_cur_blk_cnt) xmk_cur_blk_cnt --;

  #define XMK_M_ADD_CUR_MEM(PAR) \
    xmk_cur_memory_fill += PAR;

  #define XMK_M_SUB_CUR_MEM(PAR) \
    xmk_cur_memory_fill -= PAR;

#else

  #define XMK_M_INIT_MAX
  #define XMK_M_SET_MAX_BLKSIZE
  #define XMK_M_SET_MAX_BLOCKS
  #define XMK_M_INCR_ACT
  #define XMK_M_DECR_ACT
  #define XMK_M_ADD_CUR_MEM
  #define XMK_M_SUB_CUR_MEM

#endif /* ... XMK_ADD_PROFILE || (XMK_ADD_CQUERY_MEM && XMK_USE_DEBUGGING) */

#ifndef NO_GLOBAL_VARS
  #if defined(XMK_USE_SDL_MEM)
    /*
    ** control, if memory is initialized when allocating memory
    */
    static xmk_OPT_INT xmk_MemIsInitialized = XMK_FALSE;

    /*
    ** Root pointer = pointer to first element of linked list
    */
    static xmk_T_MBLOCK * first_block ;

    /*
    ** Pointer to last element of the linked list
    */
    static xmk_T_MBLOCK * xmk_LastMemoryBlockPtr = (xmk_T_MBLOCK *) NULL  ;

  #endif /* ... defined(XMK_USE_SDL_MEM) */
#endif /*NO_GLOBAL_VARS*/

/*============================================================================*/
/*-MGG*/


#ifdef XMK_USE_SDL_MEM

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_MemInit                                                  |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is to be called by the user, before dynamic memory            |
|  management can be used. The user has to specify the beginning and the end   |
|  of the area to be used for dyn.MM.                                          |
|                                                                              |
|  Parameter    :                                                              |
|                                                                              |
|    _mem_begin :                                                              |
|                                                                              |
|    _mem_end   :                                                              |
|                                                                              |
|  Return       : void                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_MemInit( void * _mem_begin, void * _mem_end )
#else
  void xmk_MemInit( _mem_begin, _mem_end )
  void * _mem_begin;
  void * _mem_end;
#endif

/*-FDEF E*/

{
  xmk_T_MBLOCK * first ;
  xmk_T_MBLOCK * last  ;
  char * _mbegin; 
  char * _mend; 

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("xmk_MemInit");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  /*
  ** Is memory already initialized ?
  ** NOTE:It is not possible to initialize memory
  ** again after it first initialized the first time.
  ** (it could be possible by setting
  **  xmk_MemIsInitialized to XMK_FALSE before)
  */
  if ( xmk_MemIsInitialized == XMK_TRUE )
  {
    XMK_END_CRITICAL_PATH;
    return;
  }

  XMK_M_INIT_MAX;

  #if defined(ARM_THUMB) || defined(_GCC_)
    /*
    ** Adjust the _mem_end - pointer so that it points to
    ** an address which can be divided through XMK_CPU_WORD_SIZE
    ** It may of course not be incremented !
    */
    while (1)
    {
      if ( ( ( (int)_mem_end) % XMK_CPU_WORD_SIZE) != 0 )
      {
        _mem_end --;
      }
      else
        break; /* DONE */
    }

  #endif

  _mbegin = (char *) _mem_begin;
  _mend   = (char *) _mem_end  ;

  first_block = NULL ;

  if( !_mem_begin )
  {
    #ifdef XMK_ADD_PRINTF_MEMORY
      XMK_TRACE_EXIT("xmk_MemInit");
    #endif
    return ;
  } /* END IF */

  if( ( _mend - _mbegin ) < ( 2 * sizeof( xmk_T_MBLOCK ) ) )
  {
    #ifdef XMK_ADD_PRINTF_MEMORY
          XMK_TRACE_EXIT("xmk_MemInit");
    #endif
    XMK_END_CRITICAL_PATH;
    return ;
  } /* END IF */

  first = ( xmk_T_MBLOCK * )_mem_begin ;
  last  = ( xmk_T_MBLOCK * )_mem_end ;

  /*
  ** For making the below work, 
  ** it must be ensured that the "last" pointer does point
  ** to an address which can be divided through 4.
  ** It might be enough to specify the 2.nd parameter
  ** in the call to xmk_MemInit appropriately.
  */
  last-- ;

  first->used   = XMK_FALSE ;
  first->next   = last ;
  first->prev   = NULL ;

  last->used    = XMK_TRUE ; /* was XMK_FALSE before */
  last->prev    = first ;
  last->next    = NULL ;

  first_block   = first ;

  xmk_LastMemoryBlockPtr = last;

  xmk_MemIsInitialized = XMK_TRUE;

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("xmk_MemInit");
  #endif
  return ;
} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_Malloc                                                   |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function tries to allocate a block of memory from the memory pool.     |
|  The caller specifies the size of the block and the function returns with    |
|  a pointer to the allocated block, upon successful completion. Otherwise,    |
|  if no more memory or not enough memory is available, the function returns   |
|  with a NULL pointer.  It is up to the user how to continue when a NULL poin-|
|  ter is returned. However, the ErrorHandler is called with the error number  |
|  ERR_N_MEM_NO_FREE. It is also an error if a block with a size of 0 is       |
|  requested, in which case the ErrorHandler is called with the error number   |
|  ERR_N_MEM_PARAM.                                                            |
|                                                                              |
|  The following is conditionally compiled with XMK_USE_MIN_BLKSIZE :          |
|                                                                              |
|  The requested block size is rounded to a value of XMK_MEM_MIN_BLKSIZE, if   |
|  the XMK_MEM_MIN_BLKSIZE is greater than the requested block size.           |
|                                                                              |
|  It is rounded to a value of power of 2, example for a minimum block size    |
|  of 64 :                                                                     |
|                                                                              |
|    rsize = 3     -----------> Use XMK_MEM_MIN_BLKSIZE = 64                   |
|    rsize = 63    -----------> Use XMK_MEM_MIN_BLKSIZE = 64                   |
|    rsize = 65    -----------> Use 2expN = 128                                |
|    rsize = 127   -----------> Use 2expN = 128                                |
|    rsize = 129   -----------> Use 2expN = 256                                |
|                                                                              |
|  The algorithm uses a "first fit" strategy.                                  |
|                                                                              |
|  Parameter    :                                                              |
|                                                                              |
|    size_t rsize                                                              |
|                                                                              |
|  Return       : void *                                                       |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
void * xmk_Malloc( size_t rsize )
#else
void * xmk_Malloc( rsize )
size_t rsize;
#endif

/*-FDEF E*/

{
  register xmk_T_MBLOCK * xmk_MemPtr ;
  #if defined(ARM_THUMB) || defined(_GCC_)
    unsigned int __TempSize;
  #endif

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("xmk_Malloc");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  #ifdef XMK_USE_MAX_ERR_CHECK
    if (! xmk_MemIsInitialized)
    {
      ErrorHandler (ERR_N_INIT_SDL_MEM);
      XMK_END_CRITICAL_PATH;
      return ((void *) NULL);      
    }    

    /*
    ** At least 1 Byte requested ?
    */
    if (!rsize)
    {
      ErrorHandler (ERR_N_MEM_PARAM);
      XMK_END_CRITICAL_PATH;
      return ((void*) NULL);
    }
  #endif

  #ifdef XMK_USE_MIN_BLKSIZE
    /* 
    ** Calculate the next higher block size than the one requested.
    ** At least a minimum of XMK_MEM_MIN_BLKSIZE will be allocated (plus 
    ** the overhead from the xmk_T_MBLOCK structure)
    */
    rsize = xmk_EvaluateExp2Size (rsize);
  #endif /* ... XMK_USE_MIN_BLKSIZE */
 
  #if defined(ARM_THUMB) || defined(_GCC_)
    /*
    ** Memory is allocated in blocks. Each
    ** block is a multiple of XMK_CPU_WORD_SIZE, e.g if XMK_CPU_WORD_SIZE is 4,
    ** then the block size is 4, 8, 12, 16, ...
    */
    __TempSize = rsize % XMK_CPU_WORD_SIZE;
    if (__TempSize)
    {
      rsize += (XMK_CPU_WORD_SIZE - __TempSize);
    }
  #endif
 
  xmk_MemPtr = __memalloc( rsize ) ;

  if( xmk_MemPtr == NULL )
  {
    ErrorHandler (ERR_N_MEM_NO_FREE) ;
  } /* END IF */

  else
  {
    XMK_M_INCR_ACT;
    XMK_M_SET_MAX_BLOCKS;
    XMK_M_SET_MAX_BLKSIZE(rsize);
    xmk_MemPtr++ ;
  } /* END ELSE */

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("xmk_Malloc");
  #endif

  XMK_END_CRITICAL_PATH;

  return( ( void* ) xmk_MemPtr ) ;
} /* END OF FUNCTION */



/* ====================================================================== */
/* =                                                                    = */
/* =   _memalloc                                                        = */
/* =                                                                    = */
/* ====================================================================== */
#ifndef XNOPROTO
  static xmk_T_MBLOCK * __memalloc( size_t rsize ) 
#else
  static xmk_T_MBLOCK * __memalloc( rsize ) 
  size_t  rsize ;
#endif
{
  register xmk_T_MBLOCK * block,  * rover ;

  register unsigned long bsize ,
                         max ;

  block  = NULL ;
  rover  = first_block ;
  max    = -1L ;

  if( first_block == NULL )
  {
    return( NULL ) ;
  } /* END IF */

  rsize += sizeof( xmk_T_MBLOCK ) ;

  do
  {
    if( rover->used == XMK_FALSE )
    {
      bsize = ( unsigned long )rover->next - ( unsigned long )rover ;

      if( ( bsize > rsize ) &&
          ( bsize < max   )
        )
      {
        block = rover ;

        if( rsize == ( max = bsize ) )
        {
          break ; /* -> skip to the end of loop */
        } /* END IF */
      } /* END IF */

      rover = rover->next ;
    } /* END IF */

    if (rover) /* bug fix */
      rover = rover->next ;
  }
  while( rover != NULL ) ;

  /*
  ** If there was a free block found
  */
  if( block != NULL )
  {
    XMK_M_ADD_CUR_MEM(rsize);

    #ifdef XMK_ADD_PROFILE 
      block->used = rsize ;
    #else
      block->used = XMK_TRUE ;
    #endif

    __memshrink( block , rsize ) ;
  } /* END IF */

  return( block ) ;
} /* END OF FUNCTION */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_Free                                                     |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the free () implementation.                 |
|                                                                              |
|  Parameter    : void  * xmk_MemPtr                                           |
|                                                                              |
|  Return       : void                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_Free ( void  * xmk_MemPtr )
#else
  void xmk_Free ( xmk_MemPtr )
  void * xmk_MemPtr;
#endif

/*-FDEF E*/

{
  register xmk_T_MBLOCK * block ;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("xmk_Free");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  #ifdef XMK_USE_MAX_ERR_CHECK
    if (! xmk_MemIsInitialized)
    {
      ErrorHandler (ERR_N_INIT_SDL_MEM);
      XMK_END_CRITICAL_PATH;
      return;
    }  
  #endif

  block = ( xmk_T_MBLOCK * )xmk_MemPtr ;

  block-- ;

  if( ( block->next->prev == block ) &&
        block->used
    )
  {
    XMK_M_DECR_ACT;
    XMK_M_SUB_CUR_MEM(block->used);

    block->used = XMK_FALSE ;

    if( !block->next->used )
    {
      block->next = block->next->next ;
      block->next->prev = block ;
    } /* END IF */

    if( ( block->prev      != NULL  ) &&
         !block->prev->used
      )
    {
      block->prev->next = block->next ;
      block->next->prev = block->prev ;
    } /* END IF */
  } /* end if( ... ) */

  else
  {
    ErrorHandler (ERR_N_MEM_ILLMBLOCK) ;
  } /* END ELSE */

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("xmk_Free");
  #endif
} /* END OF FUNCTION */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_Calloc                                                   |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the similar calloc () implementation.       |
|                                                                              |
|  Parameter    :                                                              |
|                                                                              |
|  Return       :                                                              |
|                                                                             |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void * xmk_Calloc( size_t nitems, size_t size )
#else
  void * xmk_Calloc( nitems, size )
  size_t nitems;
  size_t size;
#endif

/*-FDEF E*/

{
  char * pMemBlock ;
  unsigned long RequestedSize;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("xmk_Calloc");
  #endif

  RequestedSize = nitems * size;

  if (RequestedSize > XMK_MAX_MALLOC_SIZE )
      return NULL;

  pMemBlock = xmk_Malloc( RequestedSize ) ;

  if( pMemBlock != NULL )
  {
    register char* _pMemBlock ;

    _pMemBlock = ( char *)pMemBlock ;

    for( ; RequestedSize ; _pMemBlock[ --RequestedSize ] = 0 ) ;
  } /* END IF */

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("xmk_Calloc");
  #endif

  return( pMemBlock ) ;
} /* END OF FUNCTION */

#ifdef XMK_USE_memshrink

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_Memshrink                                                |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for an extension of the Standard of the dynamic |
|  Memory Management supported by an ordinary C-Compiler. It allows you to     |
|  shrink down a memory area previously requested with xmk_Malloc.             |
|                                                                              |
|  Parameter    :                                                              |
|                                                                              |
|    pMemBlock :                                                               |
|                                                                              |
|    NewSize   :                                                               |
|                                                                              |
|  Return       :                                                              |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_Memshrink( void * pMemBlock, size_t NewSize )
#else
  void xmk_Memshrink(pMemBlock, NewSize )
  void    * pMemBlock ;
size_t    NewSize ;
#endif

/*-FDEF E*/

{
  xmk_T_MBLOCK * _pMemBlock ;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("xmk_Memshrink");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  _pMemBlock = ( xmk_T_MBLOCK * )pMemBlock ;

  _pMemBlock-- ;

  if( ( _pMemBlock->next->prev == _pMemBlock ) &&
        _pMemBlock->used
    )
  {
    __memshrink( _pMemBlock, NewSize ) ;
  } /* END IF */

  else
  {
    ErrorHandler (ERR_N_MEM_ILLMBLOCK) ;
  } /* END ELSE */

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("xmk_Memshrink");
  #endif
} /* END OF FUNCTION */

#endif /* ... XMK_USE_memshrink */

/* ====================================================================== */
/* =                                                                    = */
/* =   _memshrink                                                       = */
/* =                                                                    = */
/* ====================================================================== */
#ifndef XNOPROTO
  static void __memshrink( xmk_T_MBLOCK * pMemBlock, size_t NewSize )
#else
  static void __memshrink( pMemBlock, NewSize )
  xmk_T_MBLOCK * pMemBlock;
  size_t    NewSize
#endif
{
  register xmk_T_MBLOCK  * pNewBlock ;
  register unsigned long BlockSize ;

  BlockSize = ( unsigned long )pMemBlock->next - ( unsigned long )pMemBlock ;

  if( BlockSize > ( sizeof( xmk_T_MBLOCK ) + NewSize + 50 ) )
  {
    pNewBlock = ( xmk_T_MBLOCK * )( ( char * )pMemBlock + NewSize ) ;

    pNewBlock->next       = pMemBlock->next ;
    pNewBlock->prev       = pMemBlock ;
    pNewBlock->prev->next = pNewBlock->next->prev = pNewBlock ;

    pNewBlock->used = XMK_FALSE ;
  } /* END IF */
} /* END OF FUNCTION */


#if defined(XMK_ADD_PROFILE) && defined(XMK_USE_EXPANDED_KERNEL)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_GetOccupiedMem                                           |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function returns the amount of occupied memory.                        |
|                                                                              |
|  Parameter    : void                                                         |
|                                                                              |
|  Return       : size_t                                                       |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  size_t xmk_GetOccupiedMem ( void )
#else
  size_t xmk_GetOccupiedMem (  )
#endif

/*-FDEF E*/

{
  return (xmk_cur_memory_fill);
}

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_GetFreeMem                                               |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function returns the amount of free memory.                            |
|  If for example a value of 100 bytes of free memory is returned to the       |
|  caller, and the caller wants to have 100 bytes of memory with the next      |
|  xmk_Malloc/xmk_Calloc call, there will be a failure.  The reason is, that   |
|  there is an extra overhead per each allocated block. The overhead can be    |
|  calculated by looking to the xmk_T_MBLOCK structure.                        |
|                                                                              |
|  Another reason for "cannot get more memory" might be that there are memory  |
|  leaks. Memory leaks occur if the memory is free-ed in another order than it |
|  was allocated. In that case the memory is cut into pieces and there is no   |
|  no piece which is large enough.                                             |
|                                                                              |
|  For preventing memory leaks, the following guidelines could help the user:  |
|                                                                              |
|   * Restrict the use of dynamic memory allocation, whereever possible        |
|   * Use fixed blocksizes wherever possible (see macro XMK_MEM_MIN_BLKSIZE    |
|     and XMK_USE_MIN_BLKSIZE)                                                 |
|   * Set the XMK_MEM_MIN_BLKSIZE value to the greatest memory block size      |
|     that is ever allocated.                                                  |
|                                                                              |
|  Parameter    : void                                                         |
|                                                                              |
|  Return       : size_t                                                       |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  size_t xmk_GetFreeMem ( void )
#else
  size_t xmk_GetFreeMem (  )
#endif

/*-FDEF E*/

{
  return (XMK_MAX_MALLOC_SIZE - xmk_cur_memory_fill);
}


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_CleanPool                                                |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function tries to clean the memory pool for memory leaks.              |
|  Memory leaks occur when there are too many allocations and deallocations on |
|  too different block sizes in an unfavourable order.                         |
|                                                                              |
|  The memory pool can only be cleaned if there are no allocated blocks left.  |
|  This means that the memory pool must be empty, which may occur only, if     |
|      *there are no signals in the queue, nor save or unsaved signals         |
|      *No SDL predefined sorts are used based on dynamic memory allocation    |
|      *the user deallocated any requested memory                              |
|      *the Cmicro Tester is not used (because there are some memory alloca-   |
|       tions)                                                                 |
|                                                                              |
|  The function may be called from time to time by the user.                   |
|                                                                              |
|  Exactly spoken the functions performs a memory reinitialization, which is   |
|  very fast.                                                                  |
|                                                                              |
|  Any profiling information is not reinitialized, e.g.can be still requested  |
|  after the memory pool was cleaned, with the following exceptions :          |
|      *the amount of used memory is initialized to 0                          |
|                                                                              |
|                                                                              |
|  Parameter    : void                                                         |
|                                                                              |
|  Return       : size_t                                                       |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  int xmk_CleanPool ( void )
#else
  int xmk_CleanPool (  )
#endif

/*-FDEF E*/

{
  
  char * _mbegin; 
  char * _mend; 

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("xmk_CleanPool");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  if ( xmk_cur_memory_fill != 0 )
  {
    /* 
    ** Cannot clean up (reinitialize) memory
    */
    return (XMK_FALSE);
  }

  /*
  ** Destroy all blocks in between first and last
  ** by initializing the links
  */
  first_block->next             = xmk_LastMemoryBlockPtr ;
  xmk_LastMemoryBlockPtr->prev  = first_block ;

  XMK_END_CRITICAL_PATH;

  /* 
  ** Memory pool cleaned up
  */
  return (XMK_TRUE);
}

#endif /* ... defined(XMK_ADD_PROFILE) && defined(XMK_USE_EXPANDED_KERNEL) */

#if defined(XMK_ADD_PROFILE) || (defined(XMK_USE_DEBUGGING) && defined(XMK_ADD_CQUERY_MEM))

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_QueryMemory                                              |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|     The function evaluates the current memory and returns the following      |
|     information to the caller :                                              |
|                                                                              |
|     * Size of memory pool                                                    |
|     * Largest Block that was ever requested.                                 |
|     * How many blocks are currently in the pool ?                            |
|     * A pointer to the physical Address of the Memory.                       |
|                                                                              |
|  Parameter    : xmk_M_STATE    *minfo                                        |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_QueryMemory ( xmk_T_CMD_QUERY_MEMORY_CNF xmk_RAM_ptr minfo )
#else
  void xmk_QueryMemory ( minfo )
  xmk_T_CMD_QUERY_MEMORY_CNF xmk_RAM_ptr minfo;
#endif

/*-FDEF E*/

{
  #ifdef XMK_ADD_PRINTF_MEM
    XMK_FUNCTION("xmk_QueryMemory");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  minfo->mem_pool_size       = XMK_MAX_MALLOC_SIZE ;
  minfo->current_memory_fill = xmk_cur_memory_fill;
  minfo->current_blocks      = xmk_cur_blk_cnt ;
  minfo->peak_amount_blocks  = xmk_max_blk_cnt ;
  minfo->largest_block       = xmk_largest_blk ;
  minfo->overhead            = sizeof (xmk_T_MBLOCK) ;

  #ifdef XMK_USE_MIN_BLKSIZE
    minfo->min_blk_size      = XMK_MEM_MIN_BLKSIZE; 
  #else
    /* No minimum blocksize defined */
    minfo->min_blk_size      = 0; 
  #endif

  #if defined(IARC51) || defined(KEIL_C51)
    minfo->address             = (long)(int) first_block  ;
  #else
    minfo->address             = (long) first_block  ;
  #endif 

  #ifdef XMK_ADD_PRINTF
    PRINTF (("M-STATE:Memory pool size incl.overhead     =%d\n", minfo->mem_pool_size ));
    PRINTF (("M-STATE:Current memory fill                =%d\n", minfo->current_memory_fill ));
    PRINTF (("M-STATE:Current amount of blocks in pool   =%d\n", minfo->current_blocks ));
    PRINTF (("M-STATE:Peak hold: Amount of blocks        =%d\n", minfo->peak_amount_blocks ));
    PRINTF (("M-STATE:Peak hold: Largest block           =%d\n", minfo->largest_block ));
    PRINTF (("M-STATE:Overhead per block (in bytes)      =%d\n", minfo->overhead ));
    PRINTF (("M-STATE:Minimum block size                 =%d\n", minfo->min_blk_size ));
    PRINTF (("M-STATE:Memory pool address (hex)          =%x\n", minfo->address  ));
    PRINTF (("M-STATE:(Probably there are memory leaks)\n"));
  #endif

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_QueryMemory");
  #endif
}

/*
+------------------------------------------------------------------------------+
+---------------------------- End of function ---------------------------------+
+------------------------------------------------------------------------------+
*/

#endif /* ...defined(XMK_ADD_PROFILE) || (defined(XMK_USE_DEBUGGING) && defined(XMK_ADD_CQUERY_MEM)) */

#endif /* ... XMK_USE_SDL_MEM */


#ifdef XMK_USE_MIN_BLKSIZE

/*+FHDR S*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_EvaluateExp2Size                                         |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function evaluates from the given length a length value, which is      |
|  is in any case a 2 exp N value. This is used to reduce the risc of memory   |
|  leaks that sometimes occur in dynamic memory management systems.            |
|                                                                              |
|  It may be used for the memory functions of this module but also for the     |
|  memory functions of an RTOS or C compiler.                                  |
|                                                                              |
|  If the minimum blocksize is in any case greater than the greatest block     |
|  that would ever be requested, then there is no risc for memory leaks.       |
|                                                                              |
|  The return result may be one of the following only :                        |
|  >is in any case a minimum of XMK_MEM_MIN_BLKSIZE, but might be a value, e.g.|
|  64,128,256,512,1024,2048,4096,8192,16384,32768,65536                        |
|                                                                              |
|  Parameter    :                                                              |
|    size_t  rl           - requested blocksize                                |
|                                                                              |
|  Return       :                                                              |
|    int                  - calculated blocksize                               |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR S*/
 
/*+FDEF S*/
 
#ifndef XNOPROTO
  size_t xmk_EvaluateExp2Size ( size_t rl )
#else
  size_t xmk_EvaluateExp2Size ( rl )
  size_t rl;
#endif
 
/*-FDEF E*/

{
  size_t res;
  int    i;
  int    x;
 
  if (rl <= XMK_MEM_MIN_BLKSIZE) return (XMK_MEM_MIN_BLKSIZE);
  if (rl >= 65536)
  {
    ErrorHandler (ERR_N_MEM_NO_FREE);
    return (rl);
  }
 
  x=rl;
  for (i=0; i < 32; i++)
  {
    x = x >> 1;
    if (!x)
    {
      res=1 << i;
      if (rl > res)
        res= (res << 1);
      return (res);
    }
  }
     
  return (rl) ;
 
} /* END OF FUNCTION */
 
#endif /* ... XMK_USE_MIN_BLKSIZE */

#ifdef XMK_USE_memset

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : memset                                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the memset () implementation.               |
|                                                                              |
|  CAUTION :                                                                   |
|  -------------                                                               |
|  Take care in the case of using preemption !                                 |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void  * XMK_memset_NAME ( void * p, xmk_OPT_INT val, size_t length)
#else
  void * XMK_memset_NAME ( p, val, length)
  void         * p;
  xmk_OPT_INT  val;
  size_t       length;
#endif

/*-FDEF E*/

{
  char * p1;
  
  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("memset");
  #endif

  p1 = p;

   while (length --)
   { *p1 = val; p1++; };

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("memset");
  #endif

  return (p);
} /* END OF FUNCTION */

#endif /* ... XMK_USE_memset */


#ifdef XMK_USE_memcpy

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : memcpy                                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the memcpy () implementation.               |
|                                                                              |
|  CAUTION :                                                                   |
|  -------------                                                               |
|  Take care in the case of using preemption !                                 |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void * XMK_memcpy_NAME ( void * dest, const void * source, size_t length )
#else
  void * XMK_memcpy_NAME ( dest, source, length)
  void *       dest;
  const void * source;
  size_t       length;
#endif

/*-FDEF E*/

{
  char *d;
  char *s;
  
  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("memcpy");
  #endif

  d = (char *) dest;
  s = (char *) source ;
  
  while (length --)
  {
    *d =  *s;
    d++; s++;
  };

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("memcpy");
  #endif

  return (dest);
} /* END OF FUNCTION */

#endif /* ... XMK_USE_memcpy */

#ifdef XMK_USE_strlen

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : strlen                                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the strlen () implementation.               |
|                                                                              |
|  CAUTION :                                                                   |
|  -------------                                                               |
|  Take care in the case of using preemption !                                 |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  size_t  XMK_strlen_NAME ( const char * p )
#else
  size_t  XMK_strlen_NAME ( p )
  const char * p;
#endif

/*-FDEF E*/

{
  int cnt = 0;

  /*#ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("strlen");
  #endif
  */
  while (*p)
  {
    cnt ++;
    p++;
  };

  /*
  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("strlen");
  #endif
  */

  return (cnt);

} /* END OF FUNCTION */

#endif /* ... XMK_USE_strlen */

#ifdef XMK_USE_strcpy

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : strcpy                                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the strcpy () implementation.               |
|                                                                              |
|  CAUTION :                                                                   |
|  -------------                                                               |
|  Take care in the case of using preemption !                                 |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  char * XMK_strcpy_NAME ( char * pdest, const char * psource )
#else
  char * XMK_strcpy_NAME ( pdest, psource )
  char * pdest;
  const char * psource;
#endif

/*-FDEF E*/

{
  char * p_return;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("strcpy");
  #endif

  p_return = pdest;
  while (*psource)
  {
    *pdest  = *psource;
    pdest   ++ ;
    psource ++ ;
  };

  *pdest ='\0';

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("strcpy");
  #endif

  return (p_return);

} /* END OF FUNCTION */

#endif /* ... XMK_USE_strcpy */

#ifdef XMK_USE_strncpy

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : strncpy                                                      |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the strncpy () implementation.              |
|                                                                              |
|  CAUTION :                                                                   |
|  -------------                                                               |
|  Take care in the case of using preemption !                                 |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  char * XMK_strncpy_NAME ( char * pdest, const char * psource, size_t length )
#else
  char * XMK_strncpy_NAME ( pdest, psource, length )
  char * pdest;
  const char * psource;
  size_t length;
#endif

/*-FDEF E*/

{

  char *s = pdest;
  register char c;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("strncpy");
  #endif

  --pdest;

  if (length >= 4)
  {
    size_t length4 = length >> 2;

    for (;;)
    {
      c = *psource++;
      *++pdest = c;
      if (c == '\0')
        break;

      c = *psource++;
      *++pdest = c;
      if (c == '\0')
        break;

      c = *psource++;
      *++pdest = c;
      if (c == '\0')
        break;

      c = *psource++;
      *++pdest = c;
      if (c == '\0')
        break;

      if (--length4 == 0)
        goto last_chars;
    }
    length = length - (pdest - s) - 1;
    if (length == 0)
      return s;
    goto zero_fill;
  }

last_chars:
  length &= 3;
  if (length == 0)
    return s;

  do
  {
    c = *psource++;
    *++pdest = c;
    if (--length == 0)
      return s;
  }
  while (c != '\0');

zero_fill:
  do
    *++pdest = '\0';
  while (--length > 0);

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("strncpy");
  #endif

  return s;

} /* END OF FUNCTION */

#endif /* ... XMK_USE_strncpy */

#ifdef XMK_USE_strcmp

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : strcmp                                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is a template for the strcmp () implementation.               |
|                                                                              |
|  CAUTION :                                                                   |
|  -------------                                                               |
|  Take care in the case of using preemption !                                 |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  int XMK_strcmp_NAME (const char * p1, const char * p2)
#else
  int XMK_strcmp_NAME (p1, p2)
  const char * p1;
  const char * p2;
#endif

/*-FDEF E*/

{

  unsigned char c1, c2;

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_FUNCTION("strcmp");
  #endif

  do
  {
    c1 = (unsigned char) *p1++;
    c2 = (unsigned char) *p2++;
    if (c1 == '\0')  return (c1 - c2);

  }
  while (c1 == c2);

  #ifdef XMK_ADD_PRINTF_MEMORY
    XMK_TRACE_EXIT("strcmp");
  #endif

  return (c1 - c2);

} /* END OF FUNCTION */

#endif /* ... XMK_USE_strcmp */

#endif /* __ML_MEM_C_ */
