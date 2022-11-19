/*
** ============================================================================
**
** Project: 
**     Graphics
**
** File:
**     eqs_feat.h
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
** 2000-06-27 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_FEAT_H_
#define _EQS_FEAT_H_

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
/* --------------------------------------------------------------------------- 
** Configuration list - 
**
** You can add these configurations in the make file.(recommended)
** 
** CONFIG_EQS_DEBUG         :
**
** CONFIG_EQS_UCS           : Unicode system
**
** CONFIG_EQS_I18N          : Internationalization
**
** CONFIG_EQS_LCL_KOREA     : Locale Korea
** CONFIG_EQS_LCL_USA       : Locale USA
**
** CONFIG_EQS_COLOR_BW      : Black/White
** CONFIG_EQS_COLOR_4GRAY   : 4 gray-level
** CONFIG_EQS_COLOR_16      : 16 colors
** CONFIG_EQS_COLOR_256     : 256 colors
**
** CONFIG_EQS_GRAPH_BASIC   :
** CONFIG_EQS_GRAPH_ENHANCED
** CONFIG_EQS_GRAPH_DRAWTEXT
** CONFIG_EQS_COMPOUND_HANGUL_FONT : display Hangul font made by compound type
** CONFIG_EQS_HANJA         : display Hanja
** CONFIG_EQS_VWF           : Variable Width Font. not yet implemented 
** CONFIG_EQS_CLIPPING      : No clipping is faster than clipping
** CONFIG_EQS_FONT_COLOR    : Set/GetTextColor is enabled
*/

#define CONFIG_EQS_DEBUG
//#define CONFIG_EQS_UCS
#define CONFIG_EQS_I18N
#define CONFIG_EQS_LCL_KOREA
//#define CONFIG_EQS_LCL_USA

//#define CONFIG_EQS_COLOR_BW
//#define CONFIG_EQS_COLOR_4GRAY
//#define CONFIG_EQS_COLOR_16
//#define CONFIG_EQS_COLOR_256

#define CONFIG_EQS_GRAPH_BASIC
//#define CONFIG_EQS_GRAPH_ENHANCED
//#define CONFIG_EQS_GRAPH_DRAWTEXT
//#define CONFIG_EQS_COMPOUND_HANGUL_FONT /* KSSM */
//#define CONFIG_EQS_HANJA
#define CONFIG_EQS_CLIPPING
#define CONFIG_EQS_FONT_COLOR

//#define CONFIG_EQS_VWF /* Variable Width Font */

#define CONFIG_EQS_SLIDING_WINDOW

//#define CONFIG_EQS_ALLOCATED_SCREEN_BUFFER

/* --------------------------------------------------------------------------- 
** Feature list
** 
** Use these features in the implementation source code files
**
** FEATURE_EQS_DEBUG        : general purpose debugging
** FEATURE_EQS_EQS_DEBUG_MEM    : debugging for memory allocation and free
** FEATURE_EQS_ASSERT
** FEATURE_EQS_TRACE
**
** FEATURE_EQS_UCS          : Unicode system
** FEATURE_EQS_I18N         : Internationalization
** FEATURE_EQS_LCL_KOREAN   : Locale Korean
** FEATURE_EQS_LCL_ENGLISH  : Locale English
** FEATURE_EQS_CES_KS5601   : Character Encoding System - KS5601
** FEATURE_EQS_CES_EUCKR    : Character Encoding System - EUCKR
**
** FEATURE_EQS_COLOR_BW      : Black/White
** FEATURE_EQS_COLOR_4GRAY   : 4 gray-level
** FEATURE_EQS_COLOR_256     : 256 colors
**
** FEATURE_EQS_GRAPH_BASIC
** FEATURE_EQS_GRAPH_ENHANCED
** FEATURE_EQS_GRAPH_DRAWTEXT
**
** FEATURE_EQS_COMPOUND_HANGUL_FONT
** FEATURE_EQS_HANJA
** FEATURE_EQS_VWF
*/

#ifdef CONFIG_EQS_DEBUG
#define FEATURE_EQS_DEBUG
#define FEATURE_EQS_DEBUG_MEM
#define FEATURE_EQS_ASSERT
#define FEATURE_EQS_TRACE
#endif

#ifdef CONFIG_EQS_UCS
#define FEATURE_EQS_UCS
#endif

#ifdef CONFIG_EQS_I18N
#define FEATURE_EQS_I18N
#endif  /* CONFIG_EQS_I18N */

/*
** Specific locale support
*/
#ifdef CONFIG_EQS_LCL_KOREA
#define FEATURE_EQS_LCL_KOREA
/* Korean character encoding system */
#define FEATURE_EQS_CES_KS5601          
#define FEATURE_EQS_CES_EUCKR    /* Not yet implemented */
#elif defined CONFIG_EQS_LCL_USA
#define FEATURE_EQS_LCL_USA
#endif

/*
** Select color space
*/
#if defined(CONFIG_EQS_COLOR_BW)
#define FEATURE_EQS_COLOR_BW
#elif defined(CONFIG_EQS_COLOR_4GRAY)
#define FEATURE_EQS_COLOR_4GRAY
#elif defined(CONFIG_EQS_COLOR_16)
#define FEATURE_EQS_COLOR_16
#elif defined(CONFIG_EQS_COLOR_256)
#define FEATURE_EQS_COLOR_256
#elif defined(CONFIG_EQS_COLOR_64K)
#define FEATURE_EQS_COLOR_64K
#else
#error The color space settings error !!!!
#endif


#if defined(CONFIG_EQS_GRAPH_BASIC)
#define FEATURE_EQS_GRAPH_BASIC
#elif defined(CONFIG_EQS_GRAPH_ENHANCED)
#define FEATURE_EQS_GRAPH_ENHANCED
#endif 

#ifdef CONFIG_EQS_GRAPH_DRAWTEXT
#define FEATURE_EQS_GRAPH_DRAWTEXT
#endif

#ifdef CONFIG_EQS_COMPOUND_HANGUL_FONT
#define FEATURE_EQS_COMPOUND_HANGUL_FONT
#endif

#ifdef CONFIG_EQS_HANJA
#define FEATURE_EQS_HANJA
#endif

#ifdef CONFIG_EQS_VWF /* Variable Width Font */
#define FEATURE_EQS_VWF
#endif

#ifdef CONFIG_EQS_CLIPPING
#define FEATURE_EQS_CLIPPING
#endif

#ifdef CONFIG_EQS_FONT_COLOR
#define FEATURE_EQS_FONT_COLOR
#endif

#ifdef CONFIG_EQS_SLIDING_WINDOW
#define FEATURE_EQS_SLIDING_WINDOW
//#define FEATURE_EQS_SLIDING_WINDOW_X /* not yet used */
#define FEATURE_EQS_SLIDING_WINDOW_Y
#endif /* CONFIG_EQS_SLIDING_WINDOW */

#ifdef   CONFIG_EQS_ALLOCATED_SCREEN_BUFFER
#define FEATURE_EQS_ALLOCATED_SCREEN_BUFFER
#endif

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
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}
#endif

#endif  /* _EQS_FEAT_H_ */

 