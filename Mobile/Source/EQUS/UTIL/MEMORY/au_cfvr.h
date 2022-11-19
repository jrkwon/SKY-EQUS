/*
 * Copyright (C) 1999, AU-System Radio AB. All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System Radio AB, and may be used and copied
 * only in accordance with the terms of this agreement.
 *
 * AU-System Radio AB assumes no responsibility
 * or liability for any errors or inaccuracies in this software,
 * or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 *
 */
#ifndef _AU_CFVR_H
#define _AU_CFVR_H

/* define USE_WIP_MALLOC if an internal memory allocator shall be used
   in favor of malloc and free. */
/* #define USE_WIP_MALLOC */

/* if internal memory allocator is to be used... */

#ifdef USE_WIP_MALLOC
	#ifdef LOG_ENABLE
		#define WIP_MALLOC_MEM_SIZE (75-5)*1000
	#else //#ifdef LOG_ENABLE
		#define WIP_MALLOC_MEM_SIZE 75*1000
	#endif //#ifdef LOG_ENABLE

	 /* define this if the memory allocator
		shall be used externally as well.
		In that case must also wip_initmalloc
		be called by the application. */
		//Replace wip memory initialization function 
		//from capiclnt.c CLNTc_start() to waptask.c browser_init()
//	#define INITIALISE_WIP_MALLOC
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
#define MEMORY_WARNING		90000
#define MEMORY_LIMIT		100000
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
    cfg_wae_ua_defaultRequestTimeout = 60,   /* number of seconds before a URL request times out */ 
    cfg_wae_ua_defaultHistorySize = 10,      /* sets the historysize to use at startup, the size can be dynamically changed */
    cfg_wae_ua_defaultDisplayImages = 1,     /* 1 if images can be displayed, 0 otherwise */
    cfg_wae_ua_defaultUpdateImages = 0,      /* 1 if images can be updated dynamically, 0 otherwise */
    cfg_wae_ua_defaultCacheExpireTime = 65000,  /* sets the default expire time (when no other is specified in the header) */
    cfg_wae_ua_imageMaxNbr = 30,             /* sets the maximum number of pictures that a user agent will handle */
    cfg_wae_ua_defaultPrio = 10,             /* sets the priority of default requests OTA - low value= lowpriority se also cfg_wae_ua_scriptPrio,cfg_wae_ua_imagePrio*/
    cfg_wae_ua_scriptPrio = 20,              /* sets the priority for script requests OTA */
    cfg_wae_ua_imagePrio = 5,                /* sets the priority when requesting images OTA - a low value indicates a lower priority=longer wait */
    cfg_wae_ua_fileCharEncoding = 106,       /* the default textencoding used in local files 106 = UCS 2 Unicode */
    /* wml ua end ------------------------------------------------------------*/

    /* wae cache control -----------------------------------------------------*/
    cfg_wae_cc_cachePrivate = 1,  /* the CacheControl cache content that have the cache-directive Private. 0: No Cache, 1: Cache */
    /* wae cache control end -------------------------------------------------*/

    /* wae wspif -------------------------------------------------------------*/
    cfg_wae_wspif_redirectPost = 0,  /* which method are used when you try to redirect a Post-request. 0: GET, 1: POST  */
    cfg_wae_wspif_connectMode = 1,  /* used only in WSP-ConnectionMode, 0: Connect to a WSP-session immediate when a UA is opened.   1: Connect to a WSP-session when the first request from an UA is started. */
    /* wae wspif end ---------------------------------------------------------*/

    /* WSP -------------------------------------------------------------------*/
    delay_before_loop_WSP = 30,
    /* WSP end ---------------------------------------------------------------*/

    /* WDP -------------------------------------------------------------------*/
    delay_before_loopSMS = 100, /*Wait n 1/10 of seconds before loop of signal*/
    delay_before_loopUDCP = 100,
    delay_before_loopUDP = 30,
    MaxReassTime = 3000,  /* number of 1/10 of seconds a reassemblation is performed before it is considered a failure*/
    LowestMaxUssdLength = 70, /* lowest maximal USSD data length (the USSD data lenght is within a range, this is the lowest*/
    MaxPDUsize = 5120,
    /* WDP end ---------------------------------------------------------------*/

    cfgEnd  /*  at least one compiler doesn't accept a comma after the last enum */
} ConfigVars;

/* browser project should exchange WAPPER with
   the name and version of the browser e.g "Mozilla/4.0" */
#define HTTP_HEADER_USER_AGENT "WAPPER"
//for SKTSPEC..
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
#define IM_1400    "04"

#define AU_SYSTEM  "00"
#define ERRISON    "01"
#define NOKIA      "02"

#define WAP_VER    "11"

#define LCDPIXCEL  "120096" 
#define LCDSIZE    "1605"
#define LCDCOLOR   "02"

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
	#define PARALLELL_PARSE_ENABLED
	#define TASKFAILURE_REPARSE_RAWDATA
#elif PARSE_MODE == 2
	#define PARALLELL_PARSE_ENABLED
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
