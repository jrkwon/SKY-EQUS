/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
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
#ifndef _CONFVARS_H
#define _CONFVARS_H

#include "wipprdct.h"


/* define USE_WIP_MALLOC if an internal memory allocator shall be used
   in favor of malloc and free. */
#define USE_WIP_MALLOC //PORTING_B0_WAP ytchoi

/* if internal memory allocator is to be used... */
#if defined USE_WIP_MALLOC
#define WIP_MALLOC_MEM_SIZE 50000
 /* define this if the memory allocator
    shall be used externally as well.
    In that case must also wip_initmalloc
    be called by the application. */

//////////////////////////////////////////////////////////////////////////////////////
// Undefine INITIALISE_WIP_MALLOC so as not to remake dynamic memory pool once again,
// which was made in waptask.c file
//////////////////////////////////////////////////////////////////////////////////////
//#define INITIALISE_WIP_MALLOC
#endif

/* define USE_MEMORY_GUARD if an upper limit of memory usage is desirable */
/* #define USE_MEMORY_GUARD */
/* Memory guard is however always on when running in internal log mode */
#if defined LOG_INTERNAL && !defined USE_MEMORY_GUARD
#define USE_MEMORY_GUARD
#endif

/* If memory counting is active, the limits for
   warnings and errors are to be assigned here */
#if defined USE_MEMORY_GUARD
#define MEMORY_WARNING   900000
#define MEMORY_LIMIT    1000000
#endif

/* Static size of the repository memory.
*/
#ifdef CONFIG_WTA
#define REP_STORAGESIZE 14400
#endif

/* Static size of the push memory.
*/
#ifdef CONFIG_PUSH
#define PUSH_STORAGESIZE 14400
#endif

/* If other character sets shall be supported
   this flag should be defined.
   To make it work, add the files in
   source\optional\charset to your project.
   Currently the character set KSC5601 is
   supported by this package */
#define USE_CHARSET_PLUGIN

typedef enum ConfigVars
{
    /* Stack Manager ---------------------------------------------------------*/
    MaxStartUpTime = 150, /*1/10 of seconds until startup is considered failed*/
    /* Stack Manager end -----------------------------------------------------*/

    /* wmls ------------------------------------------------------------------*/
    cfg_wmls_timeSlice = 10,        /* how many timeunits the interpreter will execute before returning control to the sdl system */
    cfg_wmls_roundRobin = 0,        /* 0: false, 1: true */
    cfg_wmls_oneScriptPerUa = 1,    /* 0: false, 1: true */
    cfg_wmls_handleTopPriority = 0, /* 0: false, 1: true */
    /* wmls end --------------------------------------------------------------*/

    /* wml display -----------------------------------------------------------*/
    cfg_wml_disp_do_inline = 1,     /* Confvars to tell the wml user agent*/
    cfg_wml_disp_img_inline = 1,    /* if the elements specified are to be*/
    cfg_wml_disp_anchor_inline = 1, /* rendered inline (i.e., if white    */
    cfg_wml_disp_a_inline = 1,      /* space immediately before and after */
    cfg_wml_disp_table_inline = 0,  /* the element should be preserved or */
    cfg_wml_disp_input_inline = 1,  /* not). 1 is preserve space, 0 remove*/
    cfg_wml_disp_select_inline = 0,
    /* wml display end -------------------------------------------------------*/

    /* wml ua ----------------------------------------------------------------*/
    cfg_wae_ua_methodPostCharsetOverride = 0,/* defines if the post method should be WAP conf. (=0) i.e the "charset" param. is set in http field "content-type" - or not wap conf. (=1) i.e. no "charset" param. is sent (to support some older web servers) */     
    /* AUS WAP BROWSER INTERNAL, shall not be configured */
    cfg_wae_ua_defaultRequestTimeout = 60,   /* number of seconds before a URL request times out */
    cfg_wae_ua_defaultHistorySize = 10,      /* sets the historysize to use at startup, the size can be dynamically changed */
    cfg_wae_ua_defaultDisplayImages = 1,     /* 1 if images can be displayed, 0 otherwise */
    cfg_wae_ua_defaultUpdateImages = 0,      /* 1 if images can be updated dynamically, 0 otherwise */
    /* END AUS WAP BROWSER INTERNAL */
    cfg_wae_ua_defaultCacheExpireTime = 65000,  /* sets the default expire time (when no other is specified in the header) */
    cfg_wae_ua_imageMaxNbr = 30,             /* sets the maximum number of pictures that a user agent will handle */
    cfg_wae_ua_defaultPrio = 10,             /* sets the priority of default requests OTA - low value= lowpriority se also cfg_wae_ua_scriptPrio,cfg_wae_ua_imagePrio*/
    cfg_wae_ua_scriptPrio = 20,              /* sets the priority for script requests OTA */
    cfg_wae_ua_imagePrio = 5,                /* sets the priority when requesting images OTA - a low value indicates a lower priority=longer wait */
    cfg_wae_ua_fileCharEncoding = 106,       /* the default textencoding used in local files 106 = UCS 2 Unicode */
    cfg_wae_ua_current_time_is_gmt = 0,         /* 0 if CLNTa_currentTime returns local time, 1 if CLNTa_currentTime returns GMT time. */
    /* wml ua end ------------------------------------------------------------*/

    /* wae cache control -----------------------------------------------------*/
    cfg_wae_cc_cachePrivate = 1,    /* the CacheControl cache content that have the cache-directive Private. 0: No Cache, 1: Cache */
    cfg_wae_cc_cacheCompact = 0,    /* 1: compact the cache when necessary, 0: do not compact */	
    /* wae cache control end -------------------------------------------------*/

    /* wae wspif -------------------------------------------------------------*/
    cfg_wae_wspif_redirectPost = 0, /* which method are used when you try to redirect a Post-request. 0: GET, 1: POST  */
    cfg_wae_wspif_connectMode = 1,  /* used only in WSP-ConnectionMode, 0: Connect to a WSP-session immediate when a UA is opened.   1: Connect to a WSP-session when the first request from an UA is started. */
    cfg_wae_wspif_ReSendingTimeout = 30,    /* number of seconds before a URL request is resending if no response has detected. A value less then 5 seconds is not recommended. This parameter is only in use for images and when stackmode is ConnectionLessis and the bearer is UDP */
    cfg_wae_wspif_FileTimeout = 30,  /* number of seconds GWC is waiting for a response after CLNTa_getFile was called. If the value is zero no timer is set. */
    cfg_wae_wspif_FunctionTimeout = 60,    /* number of seconds GWC is waiting for a response after CLNTa_callFunction was called. If the value is zero no timer is set. */	
    /* wae wspif end ---------------------------------------------------------*/

    /* wae wta Repository -------------------------------------------------------------*/
    cfg_wae_wta_Rep_maxcompact = 3, /* Number of allowed compactisations of the Repository memory before a scheduling must be done */
    /* wae Repository end ---------------------------------------------------------*/

    /* WSP -------------------------------------------------------------------*/
    delay_before_loop_WSP = 30,
    /* WSP end ---------------------------------------------------------------*/

    /* WTP -------------------------------------------------------------------*/
    no_of_retransmissions_UDP_WTP = 8,
    retransmission_interval_UDP_WTP = 50,
    wait_timeout_interval_UDP_WTP = 400,
    no_of_retransmissions_SMS_WTP = 4,
    retransmission_interval_SMS_WTP = 600,
    wait_timeout_interval_SMS_WTP = 3000,
    no_of_retransmissions_USSD_WTP = 4,
    retransmission_interval_USSD_WTP = 600,
    wait_timeout_interval_USSD_WTP = 600,
    /* WTP end ---------------------------------------------------------------*/

    /* WDP -------------------------------------------------------------------*/
    /* AUS WAP BROWSER INTERNAL, shall not be configured */
    delay_before_loopSMS = 100, /*Wait n 1/10 of seconds before loop of signal*/
    delay_before_loopUDCP = 100,
    delay_before_loopUDP = 30,
    /* END AUS WAP BROWSER INTERNAL */
    MaxReassTime = 3000,  /* number of 1/10 of seconds a reassemblation is performed before it is considered a failure*/
    LowestMaxUssdLength = 70, /* lowest maximal USSD data length (the USSD data lenght is within a range, this is the lowest*/
    MaxPDUsize = 5120,
    /* WDP end ---------------------------------------------------------------*/

    cfgEnd  /*  at least one compiler doesn't accept a comma after the last enum */
} ConfigVars;

/* browser project should exchange WAPPER with
   the name and version of the browser e.g "Mozilla/4.0" */
#define HTTP_HEADER_USER_AGENT "WAPPER"

/*==============================================================================
							  User Agent 필드
 ==============================================================================*/
/*
//무선 사업자(3byte)
#define CARRIER    "SK"

//서비스 방식(1byte)
#define WAP        '1'
#define SKT95C     '4' //95C망을 잡았을 경우에만 사용

//단말기 type(1byte)
#define CELLULAR   '1'

//단말기제조업체(1byte)
#define GEGO       'E' //단말기 Type -->SKTT 0x04

//단말기모델 (1byte)
#define A10        'I' //단말기 모델 0x08

//=== 서비스 확장 필드 ====
#define Phonehw    'g' //0x20
#define WAPSPEC    'J' //0x09
#define VSI        'A' //0x00 -->  Vertical   Service Indication
#define HSI1       '6' //0x3A -->  Horizontal Service Indication
#define HSI2       'A' //0x00 -- > Horizontal Service Indication
#define HSI3       'A' //0x00 -- > Horizontal Service Indication
//=== 서비스 확장 필드 ====

#define LCDPIXCEL  "080050" 
#define LCDSIZE    "1005"
#define SNDPOLY    '2'
#define LCDCOLOR   '2'
*/
/*==============================================================================
							 Temp User Agent 필드
 ==============================================================================*/
#define SKT        "SKT"

#define WAP        '1'
#define SK_WEB     '2'
#define STINGER    '3'

#define CELLULAR   '1'
#define PDA        '2'
#define SMARTPHONE '3'

#define GEGO       "SK"

#define IM_1000    "01"
#define IM_1100    "02"
#define IM_1200    "03"
#define IM_2000    "04"
#define IM_2100    "05"
#define IM_2000M   "06"
#define IM_2400    "08"


#define AU_SYSTEM  "00"
#define ERRISON    "01"
#define NOKIA      "02"


//GVM VERSION
#define WAP_VER    "11"

#define LCDPIXCEL  "120080" 
#define LCDSIZE    "1605"
#define LCDCOLOR   "02"
#define LCDCOLOR_MONO "01"
/*==============================================================================*/

/* what image types are accepted by the application */
#define ACCEPT_IMAGE "image/*"


/*
PARSE_MODE can be defined to 0, 1 or 2.
0: Do not save old card, i.e. throw it away before new card is opened.
Saves a lot of RAM but makes the browser to not be WAP conformant.
1: Save raw data. If the new card fails to open, the old card is opened
again from the raw data. Consumes as much extra memory as the WML deck
takes in compiled format.
2: Save parse tree. When the raw data is parsed a parse tree is built.
If the parse tree for a old card is saved when moving to a new card,
and the new card fails to open, re-opening of the old card is much faster.
The drawback is the memory consumption, which is the biggest of the
three possible modes.
*/
#define PARSE_MODE 1
/* Internal */
#if PARSE_MODE == 0
    /* nothing to turn on. */
#elif PARSE_MODE == 1
    #define TASKFAILURE_REPARSE_RAWDATA
#elif PARSE_MODE == 2
    #define PARALLELL_PARSE_ENABLED
#endif
#if defined TASKFAILURE_REPARSE_RAWDATA && defined PARALLELL_PARSE_ENABLED
    #error TASKFAILURE_REPARSE_RAWDATA && PARALLELL_PARSE_ENABLED may not both be defined
#endif
/* End internal */


/*when using the "infolink" functionality these values indicates what will be 
displayed when no URL is present */
#define REFRESH_TASK_INFO "REFRESH"
#define PREV_TASK_INFO "PREVIOUS"

#ifndef USE_CHARSET_PLUGIN
    /* Number of items in the AcceptCharSet array */
    #define NR_OF_ACCEPTCHARSET 4
    /* The different items in the AcceptCharSet array */
    #define INIT_ACCEPTCHARSET {    3 /* IANA_CHARSET_USASCII */ ,\
                                    4 /* IANA_CHARSET_LATIN1 */ ,\
                                  106 /* IANA_CHARSET_UTF8 */ ,\
                                 1000 /* IANA_CHARSET_UCS2*/}
#else
    /* Number of items in the AcceptCharSet array */
    #define NR_OF_ACCEPTCHARSET 5
    /* The different items in the AcceptCharSet array */
    #define INIT_ACCEPTCHARSET {    3 /* IANA_CHARSET_USASCII */ ,\
                                    4 /* IANA_CHARSET_LATIN1 */ ,\
                                  106 /* IANA_CHARSET_UTF8 */ ,\
                                 1000 /* IANA_CHARSET_UCS2*/ ,\
                                   36 /* KSC5610 */}
#endif

#endif
