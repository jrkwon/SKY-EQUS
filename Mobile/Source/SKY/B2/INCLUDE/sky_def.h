/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_def.h
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
** 2000-10-30 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_DEF_H_
#define _SKY_DEF_H_

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
/* version format n.n.n.n */
#define DMSS_VERSION_INFO                   "0.0.0.0"   /* 5000 */
#define SKY_VERSION_INFO                    "0.0.0.1"
#define EQUS_VERSION_INFO                   "0.0.0.1"

//#ifndef SKY_UI
//#define SKY_UI
//#endif

#define SKY_MODEL_B0    0
#define SKY_MODEL_B1    1
#define SKY_MODEL_B2    2

//#define SKY_MODEL_ID                        SKY_MODEL_B1
#define SKY_MODEL_NAME                      "IM-3300"

#define SKY_3100_SOUND_INTERFACE            0
#define SKY_5000_SOUND_INTERFACE            1

#define SKY_SP_ID_SK_TELECOM                "011"
#define SKY_SP_ID_SHINSEGI                  "017"

#define SKY_DEFAULT_PASSWORD                "0000"

//WAP
//#define SKY_WAP_PROXY_URL                   "wgw.n-top.com"
//#define SKY_WAP_INIT_URL                    "http://wap.n-top.com"
//#define SKY_WAP_INIT_MAIL                   "http://netmail.n-top.com/bin/wap/main.exe"
#define SKY_WAP_PROXY_URL                   "wgw.nate.com"
#define SKY_WAP_INIT_URL                    "http://wap.nate.com"
#define SKY_WAP_INIT_MAIL                   "http://netmail.nate.com/bin/wap/main.exe"
#define SKY_WAP_DORMANT_TIME                255

/* ===========================================================================
** 여기서는 모델 별로 화면 크기만을 정의한다 
** 다른 것은 make 파일에 CONFIG_SKY_XXX 형태로 정의한다.
*/
#if (SKY_MODEL_ID == SKY_MODEL_B0)
#define SKY_COLOR_SPACE          CS_4GRAY
#define SKY_SCREEN_WIDTH         120                  /* screen width */
#define SKY_SCREEN_HEIGHT        127                  /* screen height */
#define SKY_SCREEN_BUFFER_WIDTH  (SKY_SCREEN_WIDTH+8) /* screen buffer width */
#define SKY_SCREEN_BUFFER_HEIGHT (SKY_SCREEN_HEIGHT) /* screen buffer height */
#define SKY_SOUND_INTERFACE      SKY_3100_SOUND_INTERFACE   

/* screen layout definition */
#define SKY_ANNUNCIATORS_HEIGHT     14
#define SKY_COMMANDLINE_HEIGHT      14

/* sub lcd spec */
#ifdef FEATURE_SKY_SUBLCD
/* LCD size : 84 x 30 */
#define SKY_SUB_COLOR_SPACE          CS_BW
#define SKY_SUB_SCREEN_WIDTH         90                     /* screen width */
#define SKY_SUB_SCREEN_HEIGHT        16                     /* screen height */
#define SKY_SUB_SCREEN_BUFFER_WIDTH  ((PIXEL2BYTES(SKY_SUB_SCREEN_WIDTH)) << 3) /* screen buffer width */
#define SKY_SUB_SCREEN_BUFFER_HEIGHT (SKY_SUB_SCREEN_HEIGHT)    /* screen buffer height */
#define SKY_SUB_SCREEN_Y_OFFSET      3
#endif /* FEATURE_SKY_SUBLCD */
/* define in sky_feat.h
    FEATURE_SKY_COLOR_4GRAY
    FEATURE_SKY_AUDIO_YAMAHA_MA1

#define SKY_AUDIO               YAMAHA_MA1
*/
#elif (SKY_MODEL_ID == SKY_MODEL_B1)
#define SKY_COLOR_SPACE          CS_256COLOR
#define SKY_SCREEN_WIDTH         120                /* screen width */
#define SKY_SCREEN_HEIGHT        120                /* screen height */
#define SKY_SCREEN_BUFFER_WIDTH  (SKY_SCREEN_WIDTH) /* screen buffer width */
#define SKY_SCREEN_BUFFER_HEIGHT (SKY_SCREEN_HEIGHT) /* screen buffer height */
#define SKY_SOUND_INTERFACE      SKY_5000_SOUND_INTERFACE   

/* screen layout definition */
#define SKY_ANNUNCIATORS_HEIGHT     14
#define SKY_COMMANDLINE_HEIGHT      13

/* sub lcd spec */
#ifdef FEATURE_SKY_SUBLCD
/* LCD size : 84 x 30 */
#define SKY_SUB_COLOR_SPACE          CS_BW
#define SKY_SUB_SCREEN_WIDTH         84                     /* screen width */
#define SKY_SUB_SCREEN_HEIGHT        32                     /* screen height */
#define SKY_SUB_SCREEN_BUFFER_WIDTH  ((PIXEL2BYTES(SKY_SUB_SCREEN_WIDTH))<<3) /* screen buffer width */
#define SKY_SUB_SCREEN_BUFFER_HEIGHT (SKY_SUB_SCREEN_HEIGHT)    /* screen buffer height */
#define SKY_SUB_SCREEN_Y_OFFSET      2
#endif /* FEATURE_SKY_SUBLCD */

/* define in sky_feat.h
    FEATURE_SKY_COLOR_256COLOR
    FEATURE_SKY_AUDIO_YAMAHA_MA2
#define SKY_COLOR_SPACE         CS_256COLOR
#define SKY_AUDIO               YAMAHA_MA2
*/
#elif (SKY_MODEL_ID == SKY_MODEL_B2) 
#define SKY_COLOR_SPACE          CS_64KCOLOR
#define SKY_SCREEN_WIDTH         128                /* screen width */
#define SKY_SCREEN_HEIGHT        160                /* screen height */
#define SKY_SCREEN_BUFFER_WIDTH  (SKY_SCREEN_WIDTH) /* screen buffer width */
#define SKY_SCREEN_BUFFER_HEIGHT (SKY_SCREEN_HEIGHT) /* screen buffer height */
#define SKY_SOUND_INTERFACE      SKY_5000_SOUND_INTERFACE   

/* screen layout definition */
#define SKY_ANNUNCIATORS_HEIGHT     16
#define SKY_TITLE_HEIGHT            19
#define SKY_COMMANDLINE_HEIGHT      16

/* sub lcd spec */
#ifdef FEATURE_SKY_SUBLCD
/* LCD size : 84 x 30 */
#define SKY_SUB_COLOR_SPACE          CS_BW
#define SKY_SUB_SCREEN_WIDTH         84                     /* screen width */
#define SKY_SUB_SCREEN_HEIGHT        32                     /* screen height */
#define SKY_SUB_SCREEN_BUFFER_WIDTH  ((PIXEL2BYTES(SKY_SUB_SCREEN_WIDTH))<<3) /* screen buffer width */
#define SKY_SUB_SCREEN_BUFFER_HEIGHT (SKY_SUB_SCREEN_HEIGHT)    /* screen buffer height */
#define SKY_SUB_SCREEN_Y_OFFSET      2
#endif /* FEATURE_SKY_SUBLCD */

/* define in sky_feat.h
    FEATURE_SKY_COLOR_256COLOR
    FEATURE_SKY_AUDIO_YAMAHA_MA2
#define SKY_COLOR_SPACE         CS_256COLOR
#define SKY_AUDIO               YAMAHA_MA2
*/
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

#endif  /* _SKY_DEF_H_ */
