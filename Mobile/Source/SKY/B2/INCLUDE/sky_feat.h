/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_feat.h
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
** 2000-11-01 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_FEAT_H_
#define _SKY_FEAT_H_

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
**
** make 파일에서 CONFIG_SKY_XXX를 정의한다.
** (임시로 헤더 파일에 정의해 두었다)
**
** Configuration list - 
**
** CONFIG_SKY_DEBUG             : debug
** CONFIG_SKY_UCS               : unicode system
** CONFIG_SKY_I18N              : internationalization
** CONFIG_SKY_LCL               : locale
** CONFIG_SKY_COLOR             : color system
** CONFIG_SKY_MODEL_TYPE        : model type
** CONFIG_SKY_SERVICE_PROVIDER  : service provider
*/

/*----------------------------------------------------------------------------
**  debug
*/
#define CONFIG_SKY_DEBUG
#define CONFIG_SKY_SUBLCD

/*----------------------------------------------------------------------------
**  unicode system
*/
//#define CONFIG_SKY_UCS

#if 0
/*----------------------------------------------------------------------------
**  internationalization
*/
#define CONFIG_SKY_I18N

/*----------------------------------------------------------------------------
**  locale
*/
#define SKY_LCL_KOREA           1
#define SKY_LCL_USA             2

#define CONFIG_SKY_LCL          SKY_LCL_KOREA

/*----------------------------------------------------------------------------
**  color system
*/
#define SKY_COLOR_BW            1
#define SKY_COLOR_4GRAY         2
#define SKY_COLOR_16            3
#define SKY_COLOR_256           4

#define CONFIG_SKY_COLOR        SKY_COLOR_256

#endif /* 0 */

/*----------------------------------------------------------------------------
**  audio chip
*/
#define SKY_AUDIO_BUZZER        1
#define SKY_AUDIO_YAMAHA_MA1    2
#define SKY_AUDIO_YAMAHA_MA2    3

#define CONFIG_SKY_AUDIO        SKY_AUDIO_YAMAHA_MA2

/*----------------------------------------------------------------------------
**  model type
*/
#define SKY_MODEL_TYPE_FLIP     1
#define SKY_MODEL_TYPE_FOLDER   2

#define CONFIG_SKY_MODEL_TYPE   SKY_MODEL_TYPE_FOLDER

/*----------------------------------------------------------------------------
**  service provider
*/
#define SKY_SK_TELECOM          1
#define SKY_SHINSEGI            2

#define CONFIG_SKY_SERVICE_PROVIDER     SKY_SK_TELECOM

/* --------------------------------------------------------------------------- 
** Feature list
** 
** Use these features in the implementation source code files
**
** FEATURE_SKY_DEBUG        : general purpose debugging
** FEATURE_SKY_EQS_DEBUG_MEM    : debugging for memory allocation and free
** FEATURE_SKY_ASSERT
** FEATURE_SKY_TRACE
**
** FEATURE_SKY_UCS          : Unicode system
** FEATURE_SKY_I18N         : Internationalization
** FEATURE_SKY_LCL_KOREA    : Locale Korea
** FEATURE_SKY_LCL_USA      : Locale USA
** FEATURE_SKY_CES_KS5601   : Character Encoding System - KS5601
** FEATURE_SKY_CES_EUCKR    : Character Encoding System - EUCKR
**
** FEATURE_SKY_COLOR_BW     : Black/White
** FEATURE_SKY_COLOR_4GRAY  : 4 gray-level
** FEATURE_SKY_COLOR_256    : 256 colors
**
*/

/*----------------------------------------------------------------------------
**  debug
*/
#ifdef CONFIG_SKY_DEBUG
#define FEATURE_SKY_DEBUG
#define FEATURE_SKY_DEBUG_MEM
#define FEATURE_SKY_ASSERT
#define FEATURE_SKY_TRACE
#endif

#ifdef CONFIG_SKY_SUBLCD
#define FEATURE_SKY_SUBLCD
#endif

#if 0
/*----------------------------------------------------------------------------
**  unicode system
*/
#ifdef CONFIG_SKY_UCS
#define FEATURE_SKY_UCS
#endif

/*----------------------------------------------------------------------------
**  internationalization
*/
#ifdef CONFIG_SKY_I18N
#define FEATURE_SKY_I18N
#endif  /* CONFIG_SKY_I18N */

/*----------------------------------------------------------------------------
** locale
*/
#if (CONFIG_SKY_LCL == SKY_LCL_KOREA)
#define FEATURE_SKY_LCL_KOREA
/* Korean character encoding system */
#define FEATURE_SKY_CES_KS5601          
#define FEATURE_SKY_CES_EUCKR    /* Not yet implemented */
#elif (CONFIG_SKY_LCL == SKY_LCL_USA)
#define FEATURE_SKY_LCL_USA
#endif

/*----------------------------------------------------------------------------
** color space
*/
#if (CONFIG_SKY_COLOR == SKY_COLOR_BW)
#define FEATURE_SKY_COLOR_BW
#elif (CONFIG_SKY_COLOR == SKY_COLOR_4GRAY)
#define FEATURE_SKY_COLOR_4GRAY
#elif (CONFIG_SKY_COLOR == SKY_COLOR_4)
#define FEATURE_SKY_COLOR_16
#elif (CONFIG_SKY_COLOR == SKY_COLOR_256)
#define FEATURE_SKY_COLOR_256
#endif

#endif /* 0 */

/*----------------------------------------------------------------------------
**  audio chip
*/
#if (CONFIG_SKY_AUDIO == SKY_AUDIO_YAMAHA_MA1)
#define FEATURE_SKY_AUDIO_YAMAHA_MA1
#elif (CONFIG_SKY_AUDIO == SKY_AUDIO_YAMAHA_MA2)
#define FEATURE_SKY_AUDIO_YAMAHA_MA2
#endif

/*----------------------------------------------------------------------------
**  model type
*/
#if (CONFIG_SKY_MODEL_TYPE == SKY_MODEL_TYPE_FLIP)
#define FEATURE_SKY_MODEL_TYPE_FLIP
#elif (CONFIG_SKY_MODEL_TYPE == SKY_MODEL_TYPE_FOLDER)
#define FEATURE_SKY_MODEL_TYPE_FOLDER
#endif

/*----------------------------------------------------------------------------
**  service provider
*/
#if (CONFIG_SKY_SERVICE_PROVIDER == SKY_SK_TELECOM)
#define FEATURE_SKY_SK_TELECOM              /* SK Telecom service */
#define FEATURE_SKY_MY_BELL                 /* MY BELL */
#elif (CONFIG_SKY_SERVICE_PROVIDER == SKY_SHINSEGI)
#define FEATURE_SKY_SHINSEGI                /* Shinsegi service */
#define FEATURE_SKY_POWER_ANGEL             /* POWER ANGEL */
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
}  /* extern "C" */
#endif

#endif  /* _SKY_FEAT_H_ */


