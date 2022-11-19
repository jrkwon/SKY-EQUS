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
#include "capiclnt.h"
#include "wiptrgt.h"
#include "aapiclnt.h"
#include "cmmnrsrc.h"
#include "ml_typw.h"
#include "wap.ifc"
#include "trnscode.h"
#include "mk_queu.h"
#include "wipdb.h"
#include "dbfunct.h"
#include "waectype.h"


#ifdef USE_WIP_MALLOC
#include "wip_mem.h"
#endif

#if defined USE_CHARSET_PLUGIN
    #include "charset.h"
#endif

#ifdef NO_GLOBAL_VARS
    #include "userdata.h"
#endif

#ifndef NO_GLOBAL_VARS
    #ifdef HAS_SETJMP
        /* global variable for exception handling in CLNTc_run. */
        jmp_buf jmpbuf;
    #else
        /* global variable for alternative exception handling */
        BOOL    exception;
        char    *safetyblock;
        #define SIZE_OF_SAFETY_BLOCK 1000
    #endif
#endif


extern DB_ref globalRef; /* Equal to getRef(DB_root, "globl", &error). */
                         /* Internal variable in wipdb.c. Don't touch! */


EXPORT_C VOID    CLNTc_run (VOID)
{
    /* defined in cmmnrsrc.c */
    executingSDL = 1;

    #ifdef HAS_SETJMP
        if (setjmp(jmpbuf) == 0)
        {
            /*
                Process one signal and return.
                It is important that this value not exceeds 1. Otherwise, the
                function may be stucked in an internal loop in the function
                processSignal in mk_sche.c. Instead, processing power should
                be regulated for this function with the function CLNTc_wantsToRun.
            */
            xmk_RunSDL(1);
        }
        else
        {
            /*
                A longjmp from WIPAlloc. If we are here there is no more
                memory, and the situation could not be worse.
                Terminate and re-initialise again.
            */
            deleteAllTimers();
			authenticateTerminate();
			cookiesTerminate();
			dataChannelTerminate();
            db_dbTerminate();
            deleteAllMemory();
        }
    #else
        xmk_RunSDL(1);
        if (exception == TRUE)
        {
            /*
                If we are here there is no more
                memory, and the situation could not be worse.
                Terminate and re-initialise again.
            */
            exception = FALSE;
            deleteAllTimers();
			authenticateTerminate();
			cookiesTerminate();
			dataChannelTerminate();
            db_dbTerminate();
            deleteAllMemory();
        }
    #endif

    executingSDL = 0;
    SetMemoryGuard(TRUE);
}


EXPORT_C BOOL    CLNTc_wantsToRun (VOID)
{
    /*
        IF ((1 || 0) && 1) THEN RETURN 0
        If there is no signals in the cmicro queue and there is no messages in
        the connector call queue, we don't want to run.

        IF ((0 || 1) && 1) THEN RETURN 0
        If there is signals in the cmicro signal queue and all of them are saved
        for later pocessing, and there is nothing in the connector call queue,
        we don't want to run.

        IF ((1 || 0) && 0) THEN RETURN 1
        IF ((0 || 0) && 1) THEN RETURN 1
        IF ((0 || 1) && 1) THEN RETURN 1
        Otherwise we want to run as soon as possible.
    */
    if ( ((xmk_FirstSignal() == NULL) || xmk_SaveSignalsOnly()))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

#ifndef NO_GLOBAL_VARS

typedef struct xdormantProcessInstance
{
    char process;
    char instance;
} xdormantProcessInstance;

#define MAX_NO_OF_XDORMANT_STATES 128

#if defined USE_WIP_MALLOC && defined INITIALISE_WIP_MALLOC
char    wip_memory[WIP_MALLOC_MEM_SIZE];
#endif

#endif

EXPORT_C VOID    CLNTc_start (VOID)
{
    XMK_SEND_TMP_VARS

    #ifndef NO_GLOBAL_VARS
    extern XCONST XPDTBL xmk_ROM_ptr xPDTBL[MAX_SDL_PROCESS_TYPES+1];  /* <root-process-table> */
    static xdormantProcessInstance xdormantProcessInstances[MAX_NO_OF_XDORMANT_STATES];
    static char firstRun = 1;
    static char noOfXdormantProcessInstance = 0;
    #endif

    /* initialise the memory allocator */
    #if defined USE_WIP_MALLOC && defined INITIALISE_WIP_MALLOC
        if (firstRun)
        {
            wip_initmalloc(wip_memory, WIP_MALLOC_MEM_SIZE);
        }
    #endif

    #if defined USE_CHARSET_PLUGIN
        initialiseCharsetPlugin();
    #endif

    #ifndef HAS_SETJMP
        exception = FALSE;
        safetyblock = (char *)OSConnectorAlloc(sizeof(char) * SIZE_OF_SAFETY_BLOCK);
    #endif

    #if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_start\nVOID\n");
    }
    #endif

    if (firstRun) /* prepare for a later restart */
    {
        XCONST    XPDTBL      xmk_ROM_ptr pProcessType;
        int Process = 0;
        /* this loop ensures that all processes are initialised correctly
           after a restart without a prior termination of the program.
           All processes that not is in state XSTARTUP is in state XDORMANT.
           Therefore must all XDORMANT state processes be remembered so that
           they can be restarted properly. */
        while ( (pProcessType = xPDTBL [Process]) != X_END_ROOT_TABLE )
        {
            int Instance = 0;
            while( Instance < pProcessType->MaxInstances )
            {
                if (pProcessType->ProcessStateTable[ Instance ] == XDORMANT)
                {
                    if (noOfXdormantProcessInstance >= MAX_NO_OF_XDORMANT_STATES)
                    {
                    	#if defined LOG_INTERNAL
                        CLNTa_log(0, 0, "Error: increase the number of MAX_NO_OF_XDORMANT_STATES in CLNTc_start!\nVOID\n");
                    	#endif
                    	break;
                    }
                    else
                    {
                    	xdormantProcessInstances[noOfXdormantProcessInstance].process = Process;
                    	xdormantProcessInstances[noOfXdormantProcessInstance].instance = Instance;
                    	noOfXdormantProcessInstance++;
                    }
                }
                Instance++;
            }
            Process++;
        }
        firstRun = 0;
    }
    else /* re-initialise process tables */
    {
        XCONST    XPDTBL      xmk_ROM_ptr pProcessType;
        int iXdormantProcessInstance = 0;
        int Process = 0;
        while ( (pProcessType = xPDTBL [Process]) != X_END_ROOT_TABLE )
        {
            int Instance = 0;
            while( Instance < pProcessType->MaxInstances )
            {
                pProcessType->ProcessStateTable[ Instance ] = XSTARTUP;
                Instance++;
            }
            Process++;
        }
        while ( iXdormantProcessInstance < noOfXdormantProcessInstance )
        {
            pProcessType = xPDTBL [xdormantProcessInstances[iXdormantProcessInstance].process];
            pProcessType->ProcessStateTable[xdormantProcessInstances[iXdormantProcessInstance].instance] = XDORMANT;
            iXdormantProcessInstance++;
        }
    }

    xmk_InitQueue();
    xmk_InitSDL();

	db_dbInit();
	dataChannelInit();
	authenticateInit();
	cookiesInit();

	XMK_SEND_ENV( ENV,
				  StartWAPStack,
				  xDefaultPrioSignal,
				  0,
				  NULL,
				  GLOBALPID(XPTID_StackCtrl,0));
}


EXPORT_C VOID    CLNTc_terminate (VOID)
{
	XMK_SEND_TMP_VARS

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_terminate\nVOID\n");
    }
#endif

    XMK_SEND_ENV( ENV,
                  TerminateWAPStack,
                  xDefaultPrioSignal,
                  0,
                  NULL,
                  GLOBALPID(XPTID_StackCtrl,0));
}

/* Is called when the Generic WAP Client is done with its cleanup */
VOID   TerminationDone(VOID)
{
    deleteAllTimers();
	authenticateTerminate();
	cookiesTerminate();
	dataChannelTerminate();
    db_dbTerminate();
    deleteAllMemory();
    CLNTa_terminated();
}


EXPORT_C VOID    CLNTc_timerExpired (VOID)
{

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_timerExpired\nVOID\n");
    }
#endif

    timerExpired();
}



EXPORT_C VOID  CLNTc_setIntConfig (UINT8 objectId, ConfigInt kind, UINT32 value)
{
    DB_ref ref;

#ifdef LOG_EXTERNAL
    char        szFormat[256];
    size_t      strlength, end = 0;
    const char  *csz;
    const char  *cszParam  = NULL;

    strlength = strlen(csz = "CLNTc_setIntConfig: \t");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    switch (kind) {
    case configHISTORY_SIZE:        /*  How many URLs should be held in the history.    */
        csz = "configHISTORY_SIZE";
        break;
    case configCACHE_AGE:           /*  The time in seconds a cached item shall be in the cache, if no "expires date" is given with the downloaded item.    */
        csz = "configCACHE_AGE";
        break;
    case configCACHE_MODE:          /*  Supported cache modes are:
                                        0: Always check if a newer version of the item is available on server
                                        1: After the Generic WAP Client has been started, check the first time a cached item is accessed  if a newer version is available on server
                                        2: Never check if newer version is available on server. (Always use cached version) */
        csz = "configCACHE_MODE";
        switch (value) {
        case 0:
            cszParam = ":  Always check if a newer version of the item is available on server.";
            break;
        case 1:
            cszParam = ":  After the Generic WAP Client has been started, check the first time a cached item is accessed if a newer version is available on server.";
            break;
        case 2:
            cszParam = ":  Never check if newer version is available on server. (Always use cached version)";
            break;
        default:
            cszParam = ":  (Currently not a supported cache mode.)";
            break;
        }
       break;
    case configDISPLAY_IMAGES:      /*  1 if images can be displayed, 0 otherwise.  */
        csz = "configDISPLAY_IMAGES";
        break;
    case configUPDATE_IMAGES:       /*  1 if images can be displayed after the card has been displayed, 0 otherwise.    */
        csz = "configUPDATE_IMAGES";
        break;
    case configPUSH_SECURITY_LEVEL:
        csz = "configPUSH_SECURITY_LEVEL";
        break;
    case configDEFAULT_CHANNEL:
        csz = "configDEFAULT_CHANNEL";
        break;
    case configCOOKIES_ENABLED:
        csz = "configCOOKIES_ENABLED";
        break;
    case configBA_PERSISTENT_ENABLED:
        csz = "configBA_PERSISTENT_ENABLED";
        break;
    default:
        csz = "INVALID ConfigInt";
        break;
    }
    
    strlength = strlen(csz);
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    strlength = strlen(csz = "\nUINT32 \t\tvalue \t%u");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    if (cszParam != NULL) {
        strlength = strlen(cszParam);
        strncpy(&szFormat[end], cszParam, strlength);
        end += strlength;
    }

    szFormat[end++] = '\n';
    szFormat[end++] = '\n';
    szFormat[end] = '\0';
    CLNTa_log(objectId, 0, szFormat, (int)value);
#endif

    switch (kind) {
    case configHISTORY_SIZE:
        if ((ref = getObjectRef(objectId)) == DB_null)
            ref = addObject(objectId);
        db_setInt(ref, (BYTE*) HISTORY_SIZE, value);
        break;
    case configCACHE_AGE:
        db_setInt(globalRef, (BYTE*) CACHE_AGE, value);
        break;
    case configCACHE_MODE:
        db_setInt(globalRef, (BYTE*) CACHE_MODE, value);
        break;
    case configDISPLAY_IMAGES:
        if ((ref = getObjectRef(objectId)) == DB_null)
            ref = addObject(objectId);
        db_setInt(ref, (BYTE*) DISPLAY_IMAGES, value);
        break;
    case configUPDATE_IMAGES:
        if ((ref = getObjectRef(objectId)) == DB_null)
            ref = addObject(objectId);
        db_setInt(ref, (BYTE*) UPDATE_IMAGES, value);
        break;
    case configPUSH_SECURITY_LEVEL:
        db_setInt(globalRef, (BYTE*) PUSH_SECURITY_LEVEL, value);
        break;
    case configDEFAULT_CHANNEL:
        setDefaultDataChannel(objectId, (UINT8) value);
        break;
    case configCOOKIES_ENABLED:
        db_setInt(globalRef, (BYTE*) COOKIES_ENABLED, value);
        break;
    case configBA_PERSISTENT_ENABLED:
        if( value == 0 )
        	setPersistentAuthStatus( TRUE, 0 );
        else
        	setPersistentAuthStatus( TRUE, 2 );
        break;
    }
}



EXPORT_C VOID  CLNTc_setStrConfig (UINT8 objectId, ConfigStr kind, const CHAR *value, UINT16 length)
{
    BYTE* temp;
    BOOL  error;

#ifdef LOG_EXTERNAL
    char        szFormat[512];
    size_t      strlength, end = 0;
    const char  *csz;

    strlength = strlen(csz = "CLNTc_setStrConfig: \t");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    switch (kind) {
    case configWSP_Language:
        csz = "configWSP_Language";
        break;
    case configPROFILE:
        csz = "configPROFILE";
        break;
    case configPROFILE_DIFF:
        csz = "configPROFILE_DIFF";
        break;
    case configUSERAGENT:
        csz = "configUSERAGENT";
        break;
    default:
        csz = "INVALID ConfigStr";
        break;
    }

    strlength = strlen(csz);
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;
    
    strlength = strlen(csz = "\nconst CHAR * \tvalue \t");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;
    
    memcpy(&szFormat[end], value, length);
    for (strlength = end; strlength < end+length; strlength++)
        if (!wae_isascii (szFormat[strlength]) ||
            wae_isctrl (szFormat[strlength]))
          szFormat[strlength] = '?';
    end += length;
    
    strlength = strlen(csz = "\nUINT8 \t\tlength \t%u\n\n");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    szFormat[end] = '\0';
    CLNTa_log(objectId, 0, szFormat, length);
#endif

	objectId = objectId;

    error = FALSE;
    switch (kind) {
    case configWSP_Language:
        error = db_setMem(globalRef, (BYTE*) WSP_LANGUAGE, temp = db_createMem(value, length));
        break;
    case configPROFILE:
        error = db_setStr(globalRef, (BYTE*) PROFILE,      temp = db_createStr(value, length));
        break;
    case configPROFILE_DIFF:
        error = db_setMem(globalRef, (BYTE*) PROFILE_DIFF, temp = db_createMem(value, length));
        break;
    case configUSERAGENT:
        error = db_setStr(globalRef, (BYTE*) USER_AGENT,   temp = db_createStr(value, length));
        break;
    }
    if (error)
        OSConnectorFree(temp);
}



EXPORT_C VOID  CLNTc_setDCHIntConfig (UINT8 objectId, UINT8 channelId, ConfigInt kind, UINT32 value)
{
    DB_ref ref;

#ifdef LOG_EXTERNAL
    char        szFormat[256];
    size_t      strlength, end = 0;
    const char  *csz;
    const char  *cszParam  = NULL;

    strlength = strlen(csz = "CLNTc_setDCHIntConfig: \t");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    switch (kind) {
    case configACCESS_TYPE:
        /* Currently supported bearers are:
         * ANY_UDP = 0, GSM_USSD = 2, GSM_SMS = 3, GSM_CSD = 10, GSM_GPRS = 11,
         * and Bluetooth = 150.
         */
        csz = "configACCESS_TYPE";
        switch (value) {
        case BEARER_ANY_UDP:    /* Unknown UDP bearer */
            cszParam = " \t (Any UDP)";
            break;
        case BEARER_GSM_USSD:   /* GSM USSD */
            cszParam = " \t (GSM USSD)";
            break;
        case BEARER_GSM_SMS:    /* GSM SMS  */
            cszParam = " \t (GSM SMS)";
            break;
        case BEARER_GSM_CSD:    /* GSM CSD  */
            cszParam = " \t (GSM CSD)";
            break;
        case BEARER_GSM_GPRS:   /* GSM GPRS  */
            cszParam = " \t (GSM GPRS)";
            break;
        case BEARER_BT:         /* Bluetooth  */
            cszParam = " \t (Bluetooth)";
            break;
        case BEARER_ANY:        /* Unknown bearer  */
            cszParam = " \t (Unknown bearer)";
            break;
        default:
            cszParam = " \t (Currently not a supported bearer.)";
            break;
        }
        break;
    case configONLINE:
        csz = "configONLINE";
        break;
    case configCLIENT_LOCAL_PORT:
        csz = "config";
        break;
    case configUSSD_GW_TYPE:
        csz = "configUSSD_GW_TYPE";
        /*  Currently supported service code types are: none= 0, msisdn= 1 and ip= 2.   */
        switch (value) {
        case 0:     /* none     */
            cszParam = " \t (none)";
            break;
        case 1:     /* msisdn   */
            cszParam = " \t (msisdn)";
            break;
        case 2:     /* ip       */
            cszParam = " \t (ip)";
            break;
        default:
            cszParam = " \t (Currently not a supported service code.)";
            break;
        }
        break;
    case configTIMEOUT:    /*  The time in seconds the client can wait, when downloading has stalled, before the transaction is cancelled. */
        csz = "configTIMEOUT";
        break;
    case configSTACKMODE:
        csz = "configSTACKMODE";
        switch (value) {
        case MODE_CL_WSP:
            cszParam = " \t (MODE_CL_WSP)";
            break;
        case MODE_CL_WTLS:
            cszParam = " \t (MODE_CL_WTLS)";
            break;
        case MODE_CL_WTLS_WTA:
            cszParam = " \t (MODE_CL_WTLS_WTA)";
            break;
        case MODE_CO_WSP:
            cszParam = " \t (MODE_CO_WSP)";
            break;
        case MODE_CO_WTLS:
            cszParam = " \t (MODE_CO_WTLS)";
            break;
        case MODE_CO_WTLS_WTA:
            cszParam = " \t (MODE_CO_WTLS_WTA)";
            break;
        default:
            cszParam = " \t (Currently not a supported stack mode.)";
            break;
        }
        break;
    case configEXPIRES:
        csz = "configEXPIRES";
        break;
    case configMASTER_GW:
        csz = "configMASTER_GW";
        break;
    case configALLOW_WTLS_CLASS_1:
        csz = "configALLOW_WTLS_CLASS_1";
        break;
    default:
        csz = "INVALID ConfigDCHInt";
        break;
    }

    strlength = strlen(csz);
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    strlength = strlen(csz = "\nUINT32 \t\tvalue \t%u");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    if (cszParam != NULL) {
        strlength = strlen(cszParam);
        strncpy(&szFormat[end], cszParam, strlength);
        end += strlength;
    }

    szFormat[end++] = '\n';
    szFormat[end++] = '\n';
    szFormat[end] = '\0';
    
    CLNTa_log(objectId, 0, szFormat, (int)value);
#endif

    if ((ref = getChannelRef(objectId, channelId)) == DB_null)
        ref = addDataChannel(objectId, channelId);
    switch (kind) {
    case configACCESS_TYPE:
        db_setInt(ref, (BYTE*) ACCESS_TYPE,       value);
        break;
    case configONLINE:
        db_setInt(ref, (BYTE*) ONLINE,            value);
        break;
    case configCLIENT_LOCAL_PORT:
        db_setInt(ref, (BYTE*) CLIENT_LOCAL_PORT, value);
        break;
    case configUSSD_GW_TYPE:
        db_setInt(ref, (BYTE*) USSD_GW_TYPE,      value);
        break;
    case configTIMEOUT:
#ifdef FEATURE_WAP        
        db_setInt(ref, (BYTE*) WAP_TIMEOUT,       value);
#else
        db_setInt(ref, (BYTE*) TIMEOUT,           value);
#endif //#ifdef FEATURE_WAP
        break;
    case configSTACKMODE:
        db_setInt(ref, (BYTE*) STACKMODE,         value);
        break;
    case configEXPIRES:
        db_setInt(ref, (BYTE*) CHANNEL_EXPIRES,   value);
        break;
    case configMASTER_GW:
        db_setInt(ref, (BYTE*) MASTER_GW,         value);
        break;
    case configALLOW_WTLS_CLASS_1:
        db_setInt(ref, (BYTE*) ALLOW_WTLS_CLASS_1,value);
        break;
    }
}



EXPORT_C VOID  CLNTc_setDCHStrConfig (UINT8 objectId, UINT8 channelId, ConfigStr kind, const CHAR *value, UINT8 length)
{
    DB_ref ref;
    BYTE*  temp;
    BYTE*  source;
    BYTE*  s;
    UINT16 totLen; 
    UINT8  error;

#ifdef LOG_EXTERNAL
    const unsigned char cNmbrOfFldsIPAdr = 4;
    char          szFormat[512];
    size_t        strlength, end = 0;
    char          *sz;
    const char    *csz;
    BOOL          bFree = FALSE;

    strlength = strlen(csz = "CLNTc_setDCHStrConfig: \t");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    switch (kind) {
    case configUDP_IP_SRC:       /*  ip address  a.b.c.d -> abcd, i.e., the bytes from left to right.    */
        csz = "configUDP_IP_SRC";
        sz = OctetString2HexString(value, cNmbrOfFldsIPAdr);
        if (!sz)
            return;
        bFree = TRUE;
        break;
    case configUDP_IP_GW:        /*  ip address  a.b.c.d -> abcd, i.e., the bytes from left to right.    */
        csz = "configUDP_IP_GW";
        sz = OctetString2HexString(value, cNmbrOfFldsIPAdr);
        if (!sz)
            return;
        bFree = TRUE;
        break;
    case configSMS_C:           /*  msisdn number for SMS center    e.g. "+460703123456"    */
        csz = "configSMS_C";
        break;
    case configSMS_GW:          /*  msisdn number for WAP server    e.g. "+460703123456"    */
        csz = "configSMS_GW";
        break;
    case configUSSD_C:          /*  msisdn number for USSD center   e.g. "+460703123456"    */
        csz = "configUSSD_C";
        break;
    case configUSSD_GW:         /*  Three types of service codes, depending on the value of the configUSSD_SC_TYPE variable. Examples:  no SC. ""  msisdn: "+460703123456"  ip: "abcd" */
        csz = "configUSSD_GW";
        break;
    case configAUTH_PASS_GW:    /*  The password for the WAP proxy server   */
        csz = "configAUTH_PASS_GW";
        break;
    case configAUTH_ID_GW:      /*  The user id for the WAP proxy server    */
        csz = "configAUTH_ID_GW";
        break;
    case configSTARTPAGE:
        csz = "configSTARTPAGE";
        break;
    case configPROXY_PROVIDER_ID:
        csz = "configPROXY_PROVIDER_ID";
        break;
    case configADD_HOST:
        csz = "configADD_HOST";
        break;
    case configDELETE_HOST:
        csz = "configDELETE_HOST";
        break;
    default:
        csz = "INVALID configDCHStr";
        break;
    }
    strlength = strlen(csz);
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    strlength = strlen(csz = "\nconst CHAR * \tvalue \t");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    memcpy(&szFormat[end], value, length);
    for (strlength = end; strlength < end+length; strlength++)
        if (!wae_isascii (szFormat[strlength]) ||
            wae_isctrl (szFormat[strlength]))
          szFormat[strlength] = '?';
    end += length;

    strlength = strlen(csz = "\nUINT8 \t\tlength \t%u\n\n");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    szFormat[end] = '\0';
    CLNTa_log(objectId, 0, szFormat, length);
    if (bFree)
        OSConnectorFree(sz);
#endif

    error = FALSE;
    if ((ref = getChannelRef(objectId, channelId)) == DB_null)
        ref = addDataChannel(objectId, channelId);
    switch (kind) {
    case configUDP_IP_SRC:
        error = db_setMem(ref, (BYTE*) UDP_IP_SRC, temp = db_createMem(value,(UINT16) length));
        break;
    case configUDP_IP_GW:
        error = db_setMem(ref, (BYTE*) UDP_IP_GW,  temp = db_createMem(value,(UINT16) length));
        break;
    case configSMS_C:
        error = db_setMem(ref, (BYTE*) SMS_C,      temp = db_createMem(value,(UINT16) length));
        break;
    case configSMS_GW:
        error = db_setMem(ref, (BYTE*) SMS_GW,     temp = db_createMem(value,(UINT16) length));
        break;
    case configUSSD_C:
        error = db_setMem(ref, (BYTE*) USSD_C,     temp = db_createMem(value,(UINT16) length));
        break;
    case configUSSD_GW:
        error = db_setMem(ref, (BYTE*) USSD_GW,    temp = db_createMem(value,(UINT16) length));
        break;
    case configAUTH_PASS_GW:
        if (value == NULL)
            break;
        if (length != 0 && value[length-1]=='\0')
            length--;
        source = (BYTE*) db_getPtr(ref, (BYTE*) CREDENTIALS, FALSE, &error);
        if (error == DB_err_success) {
            for (s=source+2; *s++;)
                ;
            temp = (BYTE*) OSConnectorAlloc(totLen = length + (s - source) + 1);
            if (temp == NULL)
                return;
            for (s=temp+2, source+=2; *s++ = *source; source++)
                ;
        } else if (error == DB_err_field) {
            temp = (BYTE*) OSConnectorAlloc(totLen = length + 4);
            if (temp == NULL)
                return;
            s = temp + 2;
            *s++ = '\0';
        } else
            return;
        
        for (; length > 0; length--)
            *s++ = (const BYTE) *value++;
        *s = '\0';

        *(UINT16*) temp = totLen; 
        error = db_setMem(ref, (BYTE*) CREDENTIALS, temp);
        break;
    case configAUTH_ID_GW:
        if (value == NULL) {
            db_deleteItem(ref, (BYTE*) CREDENTIALS);
            break;
        }
        if (length != 0 && value[length-1]=='\0')
            length--;
        source = (BYTE*) db_getPtr(ref, (BYTE*) CREDENTIALS, FALSE, &error);
        if (error == DB_err_success) {
            s = source;
            for (source+=2; *source++;)
                ;
            temp = (BYTE*) OSConnectorAlloc(totLen = length + *(UINT16*)s - (source - s) + 3);
        } else if (error == DB_err_field)
            temp = (BYTE*) OSConnectorAlloc(totLen = length + 4);
        else
            return;

        if (temp == NULL)
            return;
        for (s=temp+2; length > 0; length--)
            *s++ = (const BYTE) *value++;
        *s++ = '\0';

        if (error == DB_err_success) {
            for (; *s++ = *source; source++)
                ;
        } else
            *s = '\0';

        *(UINT16*) temp = totLen; 
        error = db_setMem(ref, (BYTE*) CREDENTIALS, temp);
        break;
    case configSTARTPAGE:
		error = db_setStr(ref, (BYTE*) STARTPAGE,  temp = db_createStr(value, (UINT16) length));
        break;
    case configPROXY_PROVIDER_ID:
		error = db_setStr(ref, (BYTE*) PROXY_PROVIDER_ID,  temp = db_createStr(value, (UINT16) length));
        break;
    case configADD_HOST:
        addHost(objectId, channelId, (const BYTE*) value, length);
        break;
    case configDELETE_HOST:
        deleteHost(objectId, channelId, (const BYTE*) value, length);
        break;
    }
    if (error)
        OSConnectorFree(temp);
}



EXPORT_C VOID    CLNTc_file (UINT8 fileId, CHAR *data, UINT16 length, const CHAR *contentType)
{
	XMK_SEND_TMP_VARS

	yPDef_GetFileResponse var;

    CHAR    *pCHAR;

 #if     defined LOG_EXTERNAL
    {
        const CHAR  *_contentType = contentType;
        CHAR    *pData;
        const CHAR  *_data = cszNULL;
        BOOL    bFree = FALSE;
        if (data)
        {
            _data = pData = OctetString2HexString(data, length);
            bFree = TRUE;
        }
        if (!_contentType)
        {
            _contentType = cszNULL;
        }
        CLNTa_log(0, 0, "CLNTc_file\nUINT8 \t fileId \t %u\nCHAR * \t data \t %s\nUINT16 \t length \t %u\nconst CHAR * \t contentType \t %s\n", (int)fileId, _data, (int)length, _contentType);
        if (bFree)
        {
            OSConnectorFree(pData);
        }
    }
#endif

	var.Param1 = fileId;

    pCHAR = NULL;
    if (length)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(length);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, length);
        }
        else
        {
            length = 0;
        }
    }
    var.Param3 = length;
    var.Param2 = pCHAR;

    pCHAR = NULL;
    if (contentType)
    {
        size_t  len = strlen(contentType);
        /*  Add one extra byte for the terminating zero.*/
        pCHAR = (CHAR*)OSConnectorAlloc(++len);
        if (!pCHAR)
            return;
        memcpy(pCHAR, contentType, len);
    }
    var.Param4 = pCHAR;

    XMK_SEND_ENV( ENV,
                  GetFileResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_GetFileResponse ),
                  &var,
                  GLOBALPID(XPTID_RES_FC,0));
}


EXPORT_C VOID    CLNTc_functionResult (UINT8 functionId, const CHAR *data, UINT16 length, const CHAR *contentType, const variableType * const *attributes)
{
	XMK_SEND_TMP_VARS

	yPDef_CallFunctionResult var;
    CHAR    *pCHAR;
    variableType    **ppVariableType;

#if     defined LOG_EXTERNAL
    {
        const CHAR  *_data, *_contentType;
        char    *strData = NULL;
        _data = cszNULL;
        if (length)
        {
            if (data)
            {
                _data = strData = OctetString2HexString(data, length);
            }
        }
        if (contentType)
        {
            _contentType = contentType;
        }
        CLNTa_log(0, 0, "CLNTc_functionResult\nUINT8 \t functionId \t %u\nconst CHAR * \t data \t %s\nUINT16 \t length \t %u\nconst CHAR * \t contentType \t %s\nconst CHAR * \t attributes \t %x\n", (int)functionId, _data, (int)length, _contentType, attributes);
        OSConnectorFree(strData);
    }
#endif

    var.Param1 = functionId;

    pCHAR = NULL;
    if (length)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(length);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, length);
        }
        else
        {
            length = 0;
        }
    }
    var.Param3 = length;
    var.Param2 = pCHAR;
    pCHAR = NULL;
    if (contentType)
    {
        size_t  len = strlen(contentType);
        if (len)
        {
            /*  Add one for the terminating zero.*/
            pCHAR = (CHAR *)OSConnectorAlloc(++len);
            if (!pCHAR)
                return;
            strncpy(pCHAR, contentType, len);
        }
    }
    var.Param4 = pCHAR;
    ppVariableType = NULL;
    if (attributes)
    {
        const variableType * const  *pcpcVariableType;
        unsigned    i;
        const variableType  *pcvariableType;

        i = 0;
        pcpcVariableType = attributes;
        while (*pcpcVariableType++)
        {
            i++;
        }

        /*  Add one for a terminating NULL.*/
        ++i;
        ppVariableType = (variableType**)OSConnectorAlloc(i * sizeof(variableType*));

        i = 0;
        pcpcVariableType = attributes;
        while (pcvariableType = *pcpcVariableType++)
        {
            variableType    *pVariableType;
            const WCHAR *pcWCHAR;
            WCHAR   *pWCHAR;
            size_t  len;
            pVariableType = (variableType *)OSConnectorAlloc(sizeof(variableType));
            if (!pVariableType)
                return;
            pWCHAR = NULL;
            if (pcWCHAR = pcvariableType->name)
            {
                len = w_strlen(pcWCHAR) + 1;
                pWCHAR = (WCHAR *)OSConnectorAlloc(len * sizeof(WCHAR));
                if (!pWCHAR)
                    return;
                w_strcpy(pWCHAR, pcWCHAR);
            }
            pVariableType->name = pWCHAR;
            pWCHAR = NULL;
            if (pcWCHAR = pcvariableType->value)
            {
                len = w_strlen(pcWCHAR) + 1;
                pWCHAR = (WCHAR *)OSConnectorAlloc(len * sizeof(WCHAR));
                if (!pWCHAR)
                    return;
                w_strcpy(pWCHAR, pcWCHAR);
            }
            pVariableType->value = pWCHAR;
            ppVariableType[i++] = pVariableType;
        }
        ppVariableType[i] = NULL;
    }
    var.Param5 = ppVariableType;

    XMK_SEND_ENV( ENV,
                  CallFunctionResult,
                  xDefaultPrioSignal,
                  sizeof( yPDef_CallFunctionResult ),
                  &var,
                  GLOBALPID(XPTID_RES_FC,0));
}


EXPORT_C VOID    CLNTc_getContent (const CHAR *url, UINT8 urlID, BOOL reload, const CHAR *acceptHeader)
{
	XMK_SEND_TMP_VARS

    yPDef_GetContent var;
    CHAR    *pCHAR;
    size_t  len;

#if     defined LOG_EXTERNAL
    {
        const CHAR  *_url = url;
        const CHAR  *cszAcceptHeader = acceptHeader;
        if (!_url)
        {
            _url = cszNULL;
        }
        if (!acceptHeader)
        {
            cszAcceptHeader = cszNULL;
        }
        CLNTa_log(0, 0, "CLNTc_getContent\nconst CHAR * \t url \t %s\nUINT8 \t urlID \t %u\nBOOL \t reload \t %u\nconst CHAR * \t acceptHeader \t %s\n", _url, (int)urlID, (int)reload, cszAcceptHeader);
    }
#endif

    var.Param2 = urlID;

    pCHAR = NULL;
    if (url)
    {
        len = strlen(url);
        /*  Add one extra byte for the terminating zero.*/
        pCHAR = (CHAR*)OSConnectorAlloc(++len);
        if (!pCHAR)
            return;
        memcpy(pCHAR, url, len);
    }
    var.Param1 = pCHAR;
    var.Param3 = reload;
    pCHAR = NULL;
    if (acceptHeader)
    {
        len = strlen(acceptHeader);
        /*  Add one extra byte for the terminating zero.*/
        pCHAR = (CHAR*)OSConnectorAlloc(++len);
        if (!pCHAR)
            return;
        memcpy(pCHAR, acceptHeader, len);
    }
    var.Param4 = pCHAR;

    XMK_SEND_ENV( ENV,
                  GetContent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_GetContent ),
                  &var,
                  GLOBALPID(XPTID_CONT_UA,0));
}


EXPORT_C VOID    CLNTc_postContent (const CHAR *url, UINT8 urlID, BOOL reload, const CHAR *acceptHeader, const CHAR *data, UINT16 dataLen, BOOL moreData, const CHAR *contentType, UINT8 sendMode, const CHAR *contentDisp, UINT32 totalSize)
{
	XMK_SEND_TMP_VARS

	yPDef_PostContent var;
    size_t  len;
    CHAR    *sz, *pCHAR;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_postContent\nconst CHAR * \t url \t %s\nUINT8 \t urlID \t %u\nBOOL \t reload \t %u\nconst CHAR * \t acceptHeader \t %s\nUINT16 \t dataLen \t %u\nBOOL \t moreData \t %u\nconst CHAR * \t contentType \t %s\nUINT8 \t sendMode \t %u\nconst CHAR * \t contentDisp \t %s\nUINT32 \t totalSize \t %u\n", url, (int)urlID, (int)reload, acceptHeader, (int)dataLen, (int)moreData, contentType, (int)sendMode, contentDisp, totalSize);
    }
#endif

    sz = NULL;
    if (url)
    {
        len = strlen(url);
        /*  Add one extra byte for the terminating zero.*/
        sz = (CHAR*)OSConnectorAlloc(++len);
        if (!sz)
            return;
        memcpy(sz, url, len);
    }
    var.Param1 = sz;
    var.Param2 = urlID;
    var.Param3 = reload;
    sz = NULL;
    if (acceptHeader)
    {
        len = strlen(acceptHeader);
        /*  Add one extra byte for the terminating zero.*/
        sz = (CHAR*)OSConnectorAlloc(++len);
        if (!sz)
            return;
        memcpy(sz, acceptHeader, len);
    }
    var.Param4 = sz;
    pCHAR = NULL;
    if (dataLen)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(dataLen);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, dataLen);
        }
        else
        {
            dataLen = 0;
        }
    }
    var.Param5 = pCHAR;
    var.Param6 = dataLen;
	var.Param7 = moreData;
    sz = NULL;
    if (contentType)
    {
        len = strlen(contentType);
        /*  Add one extra byte for the terminating zero.*/
        sz = (CHAR*)OSConnectorAlloc(++len);
        if (!sz)
            return;
        memcpy(sz, contentType, len);
    }
    var.Param8 = sz;
    var.Param9 = sendMode;
    sz = NULL;
    if (contentDisp)
    {
        len = strlen(contentDisp);
        /*  Add one extra byte for the terminating zero.*/
        sz = (CHAR*)OSConnectorAlloc(++len);
        if (!sz)
            return;
        memcpy(sz, contentDisp, len);
    }
    var.Param10 = sz;
	var.Param11 = totalSize;
    
    XMK_SEND_ENV( ENV,
                  PostContent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PostContent ),
                  &var,
                  GLOBALPID(XPTID_CONT_UA,0));
}

EXPORT_C VOID    CLNTc_postMoreContent (UINT8 urlID, const CHAR *data, UINT16 dataLen, BOOL moreData)
{
	XMK_SEND_TMP_VARS

    yPDef_PostMoreContent var;
    CHAR *pCHAR;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_postMoreContent\nUINT8 \t urlID \t %u\nUINT16 \t dataLen \t %u\nBOOL \t moreData \t %u\n", (int)urlID, (int)dataLen, (int)moreData);
    }
#endif

    var.Param1 = urlID;

    pCHAR = NULL;
    if (dataLen)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(dataLen);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, dataLen);
        }
        else
        {
            dataLen = 0;
        }
    }
    var.Param2 = pCHAR;
    var.Param3 = dataLen;
	var.Param4 = moreData;
    
    XMK_SEND_ENV( ENV,
                  PostMoreContent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PostMoreContent ),
                  &var,
                  GLOBALPID(XPTID_CONT_UA,0));
}

EXPORT_C VOID    CLNTc_acknowledgeContent (UINT8 urlID)
{
	XMK_SEND_TMP_VARS

	yPDef_AcknowledgeContent var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_acknowledgeContent\nUINT8 \t urlID \t %u\n", (int)urlID);
    }
#endif

    var.Param1 = urlID;
    
    XMK_SEND_ENV( ENV,
                  AcknowledgeContent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_AcknowledgeContent ),
                  &var,
                  GLOBALPID(XPTID_CONT_UA,0));
}

EXPORT_C VOID    CLNTc_cancelContent (UINT8 urlID)
{
	XMK_SEND_TMP_VARS

    yPDef_CancelContent var;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_cancelContent\nUINT8 \t urlID \t %u\n", (int)urlID);
    }
#endif

    var.Param1 = urlID;
    
    XMK_SEND_ENV( ENV,
                  CancelContent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_CancelContent ),
                  &var,
                  GLOBALPID(XPTID_CONT_UA,0));
}


EXPORT_C VOID    CLNTc_setTranscoders (
fPtr_Iana2Unicode_canConvert canConvert,
fPtr_Iana2Unicode_calcLen calcLen,
fPtr_Iana2Unicode_convert convert,
fPtr_Iana2Unicode_getNullTermByteLen nullLen)
{
#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_setTranscoders\nfPtr_Iana2Unicode_canConvert \t canConvert \t %p\nfPtr_Iana2Unicode_calcLen \t calcLen \t %p\nfPtr_Iana2Unicode_convert \t convert \t %p\nfPtr_Iana2Unicode_getNullTermByteLen \t nullLen \t %p\n", canConvert, calcLen, convert, nullLen);
    }
#endif

    Iana2Unicode_setFuncPtrs(canConvert, calcLen, convert, nullLen);
}


EXPORT_C VOID    CLNTc_requestConnectionDone (UINT8 objectId, BOOL success )
{
	XMK_SEND_TMP_VARS

	yPDef_RequestConnectionDone var;

    var.Param1 = objectId;
    var.Param2 = success;

    XMK_SEND_ENV( ENV,
                  RequestConnectionDone,
                  xDefaultPrioSignal,
                  sizeof( yPDef_RequestConnectionDone ),
                  &var,
                  GLOBALPID(XPTID_RES_ME,0));
}


EXPORT_C VOID    CLNTc_setupConnectionDone (UINT8 objectId, UINT8 channelId, BOOL success )
{
	XMK_SEND_TMP_VARS

	yPDef_SetupConnectionDone var;

    var.Param1 = objectId;
	var.Param2 = channelId;
    var.Param3 = success;

    XMK_SEND_ENV( ENV,
                  SetupConnectionDone,
                  xDefaultPrioSignal,
                  sizeof( yPDef_SetupConnectionDone ),
                  &var,
                  GLOBALPID(XPTID_RES_ME,0));
}

EXPORT_C VOID    CLNTc_closeConnection (UINT8 objectId, UINT8 channelId, BOOL deleteChannel )
{
	XMK_SEND_TMP_VARS

	yPDef_CloseConnection var;
 
    var.Param1 = objectId;
	var.Param2 = channelId;
    var.Param3 = deleteChannel;

    XMK_SEND_ENV( ENV,
                  CloseConnection,
                  xDefaultPrioSignal,
                  sizeof( yPDef_CloseConnection ),
                  &var,
                  GLOBALPID(XPTID_RES_ME,0));
}

/* Provisioning connectors */

EXPORT_C VOID    CLNTc_decodeProvisioningData (const CHAR *data, UINT32 dataLen)
{
	XMK_SEND_TMP_VARS

	 CHAR    *pCHAR;

	yPDef_InputStringProvE2Esec var;

	pCHAR = NULL;
    if (dataLen)
    {
        if (data)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(dataLen);
            if (!pCHAR)
                return;
            memcpy(pCHAR, data, (UINT16)dataLen);
        }
        else
        {
            dataLen = 0;
        }
    }

    var.Param1 = pCHAR;
    var.Param2 = dataLen;
	var.Param3 = -1;
	var.Param4 = -1;
	var.Param5 = 0;
	var.Param6 = 0;
	var.Param7 = NULL;

    XMK_SEND_ENV( ENV,
                  InputStringProvE2Esec,
                  xDefaultPrioSignal,
                  sizeof( yPDef_InputStringProvE2Esec ),
                  &var,
                  GLOBALPID(XPTID_PROV_UA,0));
}


EXPORT_C VOID    CLNTc_e2eSecurityCnfgrd (UINT8 id, BOOL success )
{
	XMK_SEND_TMP_VARS

	yPDef_E2ESecurityConfigured var;
 
    var.Param1 = id;
	var.Param2 = success;

    XMK_SEND_ENV( ENV,
                  E2ESecurityConfigured,
                  xDefaultPrioSignal,
                  sizeof( yPDef_E2ESecurityConfigured ),
                  &var,
                  GLOBALPID(XPTID_RES_ME,0));
}


EXPORT_C VOID    CLNTc_provSharedSecret (UINT16 iListID, CHAR* vcSharedSecret, UINT16 iLenSharedSecret, BOOL bLastTry)
{
	XMK_SEND_TMP_VARS

	 CHAR    *pCHAR;

	yPDef_ProvSharedSecret var;

	pCHAR = NULL;
    if (iLenSharedSecret)
    {
        if (vcSharedSecret)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(iLenSharedSecret);
            if (!pCHAR)
                return;
            memcpy(pCHAR, vcSharedSecret, (UINT16)iLenSharedSecret);
        }
        else
        {
            iLenSharedSecret = 0;
        }
    }

    var.Param1 = iListID;
    var.Param2 = pCHAR;
	var.Param3 = iLenSharedSecret;
	var.Param4 = bLastTry;

    XMK_SEND_ENV( ENV,
                  ProvSharedSecret,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ProvSharedSecret ),
                  &var,
                  GLOBALPID(XPTID_PROV_UA,0));
}
/* End provisioning connectors */


#ifdef USE_PROPRIETARY_WMLS_LIBS

EXPORT_C VOID    CLNTc_WMLSLibFuncResponse(UINT16 invokeId, const WMLSvar *resultVar)
{
	XMK_SEND_TMP_VARS

	yPDef_WMLSLibFuncResponse var;
    WMLSvar *pWMLSvar;

    var.Param1 = invokeId;

    pWMLSvar = (WMLSvar*)OSConnectorAlloc(sizeof(WMLSvar));

    if (!pWMLSvar)
        return;
	memset(pWMLSvar, 0, sizeof(WMLSvar));
    memcpy(pWMLSvar, resultVar, sizeof(WMLSvar));
    pWMLSvar->stringVal = NULL; /*  Is this necessary? See declaration of struct in tapiclnt.h */
    if (resultVar->type == 2)
    {
        size_t  len = (size_t)(resultVar->stringLengthInBytes) * sizeof(CHAR);  /* memcpy only accepts size_t anyway */
        CHAR    *pCHAR = (CHAR*)OSConnectorAlloc(len);

        if (!pCHAR)  return;

		memset(pCHAR, 0, len);
        memcpy(pCHAR, resultVar->stringVal, len);
        pWMLSvar->stringVal = pCHAR;
    }
    var.Param2 = pWMLSvar;

    XMK_SEND_ENV( ENV,
                  WMLSLibFuncResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WMLSLibFuncResponse ),
                  &var,
                  GLOBALPID(XPTID_WMLS_SM,0));
}

#endif
