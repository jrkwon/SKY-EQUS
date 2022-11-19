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

/* #define WMLS_CORRECT_FLOAT2STRING */


/* define USE_WIP_MALLOC if an internal memory allocator shall be used
   in favor of malloc and free. */
/* #define USE_WIP_MALLOC */

/* Handle scriptlibrary*/
/* #define USE_PROPRIETARY_WMLS_LIBS */

/* if internal memory allocator is to be used... */
#if defined USE_WIP_MALLOC
#define WIP_MALLOC_MEM_SIZE 75*1000
 /* define this if the memory allocator
    shall be used externally as well.
    In that case must also wip_initmalloc
    be called by the application. */
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

/*
To add support for WMLS Library function Crypto.signText.
This is only available in a WTSL configuration.
*/
#ifdef CONFIG_WTLS
/*#define CAN_SIGN_TEXT*/
#endif

/* Static size of the repository memory.
*/
#ifdef CONFIG_WTA
#ifdef FEATURE_WAP
//TMP
#define REP_STORAGESIZE 8000
#else
#define REP_STORAGESIZE 8000
#endif //#ifdef FEATURE_WAP

/* With, or without GSM extensions in WTAI?
   Turn it on by enabling this constant. */
#define CONFIG_WTAI_GSM
#endif

/* Static size of the push memory.
*/
#ifdef CONFIG_PUSH
#ifdef FEATURE_WAP
//TMP
#define PUSH_STORAGESIZE 4500
#else

#define PUSH_STORAGESIZE 4500
#endif //#ifdef FEATURE_WAP
#endif

/* Static size of the database memory.
*/
#ifdef FEATURE_WAP
#define DATABASE_STORAGESIZE 4000
#else
#define DATABASE_STORAGESIZE 4000
#endif //#ifdef FEATURE_WAP


/* If other character sets shall be supported
   this flag should be defined.
   To make it work, add the files in
   source\optional\charset to your project.
   Currently the character set KSC5601 is
   supported by this package */
#define USE_CHARSET_PLUGIN

/* Handle Large Data Transfer */
#define LARGE_DATA_TRANSFER_ENABLED

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
    cfg_wml_disp_do_inline = 0,     /* Confvars to tell the wml user agent*/
    cfg_wml_disp_img_inline = 1,    /* if the elements specified are to be*/
    cfg_wml_disp_anchor_inline = 1, /* rendered inline (i.e., if white    */
    cfg_wml_disp_a_inline = 1,      /* space immediately before and after */
    cfg_wml_disp_table_inline = 1,  /* the element should be preserved or */
    cfg_wml_disp_input_inline = 1,  /* not). 1 is preserve space, 0 remove*/
    cfg_wml_disp_select_inline = 1,
    /* wml display end -------------------------------------------------------*/

    /* wml ua ----------------------------------------------------------------*/
    cfg_wae_ua_methodPostCharsetOverride = 0,/* defines if the post method should be WAP conf. (=0) i.e the "charset" param. is set in http field "content-type" - or not wap conf. (=1) i.e. no "charset" param. is sent (to support some older web servers) */     

    cfg_wae_wspif_AuthenticationItems = 10,    /* maximum number of items GWC stores in the Authenticationlist */	
    cfg_wae_ua_imageMaxNbr = 30,             /* sets the maximum number of pictures that a user agent will handle */
    cfg_wae_ua_fileCharEncoding = 106,       /* the default textencoding used in local files 106 = UCS 2 Unicode */
    cfg_wae_ua_current_time_is_gmt = 0,         /* 0 if CLNTa_currentTime returns local time, 1 if CLNTa_currentTime returns GMT time. */
    /* wml ua end ------------------------------------------------------------*/

    /* wae cache control -----------------------------------------------------*/
    cfg_wae_cc_cachePrivate = 1,    /* the CacheControl cache content that have the cache-directive Private. 0: No Cache, 1: Cache */
    cfg_wae_cc_cacheCompact = 0,    /* 1: compact the cache when necessary, 0: do not compact */
    /* wae cache control end -------------------------------------------------*/

    /* wae wspif -------------------------------------------------------------*/
    cfg_wae_wspif_redirectPost = 0, /* which method are used when you try to redirect a Post-request. 0: GET, 1: POST  */
    cfg_wae_wspif_FileTimeout = 30,  /* number of seconds GWC is waiting for a response after CLNTa_getFile was called. If the value is zero no timer is set. */
    cfg_wae_wspif_FunctionTimeout = 60,    /* number of seconds GWC is waiting for a response after CLNTa_callFunction was called. If the value is zero no timer is set. */	
    /* wae wspif end ---------------------------------------------------------*/

    /* wae wta Repository -------------------------------------------------------------*/
    cfg_wae_wta_Rep_maxcompact = 3, /* Number of allowed compactisations of the Repository memory before a scheduling must be done */
    /* wae Repository end ---------------------------------------------------------*/

    /* wae Push -------------------------------------------------------------*/
    cfg_wae_push_compare_authority = 1, /* For incoming push indication, 0: do not perform authority comparison between push content and X-Wap-header, 1: perform authority comparison between push content and X-Wap-header. */
    cfg_wae_push_notify_sl = 1, /* For incoming Service Loadings: 0: Handle them entirely within the Generic WAP Client. No notifications are made to the WAP application. 1: Let the WAP application handle them. Notify the WAP application using the functions PUSHa_newSLreceived. */
    cfg_wae_push_in_buffer_size = 5, /* The number of incoming push-signals that can be received by the push handler when it is busy executing other tasks, i.e, the end-user chooses to interact on them. */
	cfg_wae_push_notify_change = 1, /* For indication of push message that has been replaced or deleted by an incoming push message: 0 The client will not be notified about the change, 1: The client will be notifed about the change */
	/* wae Push end ---------------------------------------------------------*/

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
    WTP_SAR_GROUP_SIZE = 5120,
    WTP_SAR_SEGMENT_SIZE = 1024,
    /* WTP end ---------------------------------------------------------------*/

    /* WDP -------------------------------------------------------------------*/
    MaxReassTime = 3000,  /* number of 1/10 of seconds a reassemblation is performed before it is considered a failure*/
    LowestMaxUssdLength = 70, /* lowest maximal USSD data length (the USSD data lenght is within a range, this is the lowest*/
    //MaxPDUsize = 5120,
    MaxPDUsize = 10240,
    /* WDP end ---------------------------------------------------------------*/

    cfgEnd  /*  at least one compiler doesn't accept a comma after the last enum */
} ConfigVars;


/* The maximum size of data that can be sent/retrieved using the content handler */
#define CONTENT_UA_MAX_MESSAGE_SIZE 261120

/* what image types are accepted by the application */
//#define ACCEPT_IMAGE "image/gif, image/jpeg"
#define ACCEPT_IMAGE "image/*"

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

#ifdef FEATURE_WAP
#define SKT        "SKT"
#define S_WAP        '1'
#define SK_WEB     '2'
#define STINGER    '3'
#define SKT95C     '4' //95C망을 잡았을 경우에만 사용
#define CELLULAR_95A   '1'
#define CELLULAR_95C   '2'
#define PDA        '3'
#define SMARTPHONE '4'
#define GEGO       "SK"
#define IM_1000    "01"
#define IM_1100    "02"
#define IM_1200    "03"
#define IM_2000    "04"
#define IM_2100    "05"
#define IM_2000M   "06"
#define IM_2400    "08"
#define IM_3000    "09"
#define IM_3100    "10"

#define AU_SYSTEM  "00"
#define ERRISON    "01"
#define NOKIA      "02"

//GVM VERSION
#define WAP_VER    "11"
#define LCDPIXCEL  "120080" 
#define LCDPIXCEL_B1  "120120"

#define LCDSIZE    "1605" 
#define LCDSIZE_B1    "2008"

#define LCDCOLOR   "02"
#define LCDCOLOR_MONO  "01"
#define LCDCOLOR_256   "08"
#endif //#ifdef FEATURE_WAP

	
	/* AUS WAP BROWSER INTERNAL, shall not be configured */




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

#define HANDLE_NOKIA_DEVIATIONS_FROM_STANDARD

typedef enum InternalConfigVars
{
    cfg_wae_ua_defaultPrio = 10,             /* sets the priority of default requests OTA - low value= lowpriority se also cfg_wae_ua_scriptPrio,cfg_wae_ua_imagePrio*/
    cfg_wae_ua_imagePrio = 5,                /* sets the priority when requesting images OTA - a low value indicates a lower priority=longer wait */

	cfgInternalEnd  /*  at least one compiler doesn't accept a comma after the last enum */

} InternalConfigVars;

/* END AUS WAP BROWSER INTERNAL */


#endif
