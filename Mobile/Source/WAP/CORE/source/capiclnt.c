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
#include "msgqueue.h"
#include "trnscode.h"
#include "ml_typw.h"
#include "mk_queu.h"
#include "wipdb.h"
#include "dbfunct.h"


#ifdef USE_WIP_MALLOC
#include "wip_mem.h"
#endif

#if defined USE_CHARSET_PLUGIN
    #include "charset.h"
#endif

#ifdef NO_GLOBAL_VARS
    #include "userdata.h"
#endif

#if defined CONFIG_WTLS || defined CAN_SIGN_TEXT
    #include "aapicrpt.h"
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

#ifdef LOG_EXTERNAL
    #include <ctype.h>
#endif

extern DB_ref globalRef; /* Equal to getRef(DB_root, "globl", &error). */
                         /* Internal variable in wipdb.c. Don't touch! */


BYTE* duplicateStr(const char* p, UINT16 length) {
    BYTE* ptr = NULL;
    
    if (length > 0 && p != NULL && length < 65535) {
        ptr = (BYTE*) OSConnectorAlloc(length + 1);
        if (ptr) {
            memcpy(ptr, p, length);
            ptr[length] = '\0';
        }
    }
    return ptr;
}



BYTE* duplicateMem(const char* p, UINT16 length) {
    BYTE* ptr = NULL;
    
    if (length > 0 && p != NULL && length < 65534) {
        ptr = (BYTE*) OSConnectorAlloc(length + 2);
        if (ptr) {
            memcpy(ptr + 2, p, length);
            *(UINT16*) ptr = length + 2;
        }
    }
    return ptr;
}



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
            DeleteQueue();
			authenticateTerminate();
			dataChannelTerminate();
            dbTerminate();
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
            DeleteQueue();
			authenticateTerminate();
			dataChannelTerminate();
            dbTerminate();
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
    if ( ((xmk_FirstSignal() == NULL) || xmk_SaveSignalsOnly()) && (FirstMsg() == NULL) )
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

//jjkyg modified because, AU state is increased to 89 more.. so modified define value to 128
//#define MAX_NO_OF_XDORMANT_STATES 36
#define MAX_NO_OF_XDORMANT_STATES 128

//#if defined USE_WIP_MALLOC && defined INITIALISE_WIP_MALLOC
//char    wip_memory[WIP_MALLOC_MEM_SIZE];
//#endif

#endif

EXPORT_C VOID    CLNTc_start (VOID)
{
    Msg *pMsg;
    int bSendMsg;

    #ifndef NO_GLOBAL_VARS
    extern XCONST XPDTBL xmk_ROM_ptr xPDTBL[MAX_SDL_PROCESS_TYPES+1];  /* <root-process-table> */
    static xdormantProcessInstance xdormantProcessInstances[MAX_NO_OF_XDORMANT_STATES];
    static char firstRun = 1;
    static char noOfXdormantProcessInstance = 0;
    #endif

    /* initialise the memory allocator */
//    #if defined USE_WIP_MALLOC && defined INITIALISE_WIP_MALLOC
//        if (firstRun)
//        {
//            wip_initmalloc(wip_memory, WIP_MALLOC_MEM_SIZE);
//        }
//    #endif

    #if defined USE_CHARSET_PLUGIN
        initialiseCharsetPlugin();
    #endif

    pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    if (!pMsg)
        return;

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
                    xdormantProcessInstances[noOfXdormantProcessInstance].process = Process;
                    xdormantProcessInstances[noOfXdormantProcessInstance].instance = Instance;
                    noOfXdormantProcessInstance++;
                    #if defined LOG_INTERNAL
                    if (noOfXdormantProcessInstance > MAX_NO_OF_XDORMANT_STATES)
                        CLNTa_log(0, 0, "Error: increase the number of MAX_NO_OF_XDORMANT_STATES in CLNTc_start!\nVOID\n");
                    #endif
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

	dbInit();
	dataChannelInit();
	authenticateInit();

    InitQueue();

#if defined CONFIG_WTLS || defined CAN_SIGN_TEXT
    CRYPTa_initialise ();
#endif

    pMsg->Id = eCLNTc_start;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    CLNTc_terminate (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_terminate\nVOID\n");
    }
#endif

    pMsg->Id = eCLNTc_terminate;

    bSendMsg = SendMsg(pMsg);
}

/* Is called when the Generic WAP Client is done with its cleanup */
VOID   TerminationDone(VOID)
{
#if defined CONFIG_WTLS || defined CAN_SIGN_TEXT
    (void)CRYPTa_terminate ();
#endif
    deleteAllTimers();
    DeleteQueue();
	authenticateTerminate();
	dataChannelTerminate();
    dbTerminate();
    deleteAllMemory();
    CLNTa_terminated();
}


EXPORT_C VOID    CLNTc_timerExpired (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_timerExpired\nVOID\n");
    }
#endif

    pMsg->Id = eCLNTc_timerExpired;

    bSendMsg = SendMsg(pMsg);
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
        setInt(ref, (BYTE*) "hstSz", value);
        break;
    case configCACHE_AGE:
        setInt(globalRef, (BYTE*) "cAge",  value);
        break;
    case configCACHE_MODE:
        setInt(globalRef, (BYTE*) "cMode", value);
        break;
    case configDISPLAY_IMAGES:
        if ((ref = getObjectRef(objectId)) == DB_null)
            ref = addObject(objectId);
        setInt(ref, (BYTE*) "dsplI", value);
        break;
    case configUPDATE_IMAGES:
        if ((ref = getObjectRef(objectId)) == DB_null)
            ref = addObject(objectId);
        setInt(ref, (BYTE*) "updtI", value);
        break;
    case configPUSH_SECURITY_LEVEL:
        setInt(globalRef, (BYTE*) "pshSL", value);
        break;
    case configDEFAULT_CHANNEL:
        setDefaultDataChannel(objectId, (UINT8) value);
        break;
    }
}



EXPORT_C VOID  CLNTc_setStrConfig (UINT8 objectId, ConfigStr kind, const CHAR *value, UINT16 length)
{
    BYTE* temp;
    BOOL  error;

#ifdef LOG_EXTERNAL
    const unsigned char cNmbrOfFldsIPAdr = 4;
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
        if (!isprint(szFormat[strlength]))
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
        error = setMem(globalRef, (BYTE*) "wsp l", temp = duplicateMem(value, length));
        break;
    case configPROFILE:
        error = setStr(globalRef, (BYTE*) "profl", temp = duplicateStr(value, length));
        break;
    case configPROFILE_DIFF:
        error = setMem(globalRef, (BYTE*) "pDiff", temp = duplicateMem(value, length));
        break;
    case configUSERAGENT:
        error = setStr(globalRef, (BYTE*) "ua",    temp = duplicateStr(value, length));
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
        setInt(ref, (BYTE*) "accTp", value);
        break;
    case configONLINE:
        setInt(ref, (BYTE*) "onln", value);
        break;
    case configCLIENT_LOCAL_PORT:
        setInt(ref, (BYTE*) "cltLP", value);
        break;
    case configUSSD_GW_TYPE:
        setInt(ref, (BYTE*) "usdGT", value);
        break;
    case configTIMEOUT:
        setInt(ref, (BYTE*) "tmout", value);
        break;
    case configSTACKMODE:
        setInt(ref, (BYTE*) "stckm", value);
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
        if (!isprint(szFormat[strlength]))
            szFormat[strlength] = '?';
    end += length;

    strlength = strlen(csz = "\nUINT8 \t\tlength \t%u\n\n");
    strncpy(&szFormat[end], csz, strlength);
    end += strlength;

    szFormat[end] = '\0';
    printf(szFormat, length);
    CLNTa_log(objectId, 0, szFormat, length);
    if (bFree)
        OSConnectorFree(sz);
#endif

    error = FALSE;
    if ((ref = getChannelRef(objectId, channelId)) == DB_null)
        ref = addDataChannel(objectId, channelId);
    switch (kind) {
    case configUDP_IP_SRC:
        error = setMem(ref, (BYTE*) "udpIS", temp = duplicateMem(value,(UINT16) length));
        break;
    case configUDP_IP_GW:
        error = setMem(ref, (BYTE*) "udpIG", temp = duplicateMem(value,(UINT16) length));
        break;
    case configSMS_C:
        error = setMem(ref, (BYTE*) "smsC",  temp = duplicateMem(value,(UINT16) length));
        break;
    case configSMS_GW:
        error = setMem(ref, (BYTE*) "smsGW", temp = duplicateMem(value,(UINT16) length));
        break;
    case configUSSD_C:
        error = setMem(ref, (BYTE*) "usdC",  temp = duplicateMem(value,(UINT16) length));
        break;
    case configUSSD_GW:
        error = setMem(ref, (BYTE*) "usdGW", temp = duplicateMem(value,(UINT16) length));
        break;
    case configAUTH_PASS_GW:
        if (value == NULL)
            break;
        if (length != 0 && value[length-1]=='\0')
            length--;
        source = getPtr(ref, (BYTE*) "cred", FALSE, &error);
        if (error == DB_err_success) {
            for (s=source+2; *s++;)
                ;
            temp = OSConnectorAlloc(totLen = length + (s - source) + 1);
            if (temp == NULL)
                return;
            for (s=temp+2, source+=2; *s++ = *source; source++)
                ;
        } else if (error == DB_err_field) {
            temp = OSConnectorAlloc(totLen = length + 4);
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
        error = setMem(ref, (BYTE*) "cred", temp);
        break;
    case configAUTH_ID_GW:
        if (value == NULL) {
            deleteItem(ref, (BYTE*) "cred");
            break;
        }
        if (length != 0 && value[length-1]=='\0')
            length--;
        source = getPtr(ref, (BYTE*) "cred", FALSE, &error);
        if (error == DB_err_success) {
            s = source;
            for (source+=2; *source++;)
                ;
            temp = OSConnectorAlloc(totLen = length + *(UINT16*)s - (source - s) + 3);
        } else if (error == DB_err_field)
            temp = OSConnectorAlloc(totLen = length + 4);
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
        error = setMem(ref, (BYTE*) "cred", temp);
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
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;

    if (!pMsg)
        return;

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

    pMsg->Id = eCLNTc_file;

    pMsg->vUINT8[0] = fileId;
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
    pMsg->vUINT16[0] = length;
    pMsg->pCHAR[0] = pCHAR;

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
    pMsg->sz[0] = pCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    CLNTc_functionResult (UINT8 functionId, const CHAR *data, UINT16 length, const CHAR *contentType, const variableType * const *attributes)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;
    variableType    **ppVariableType;

    if (!pMsg)
        return;

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

    pMsg->Id = eCLNTc_functionResult;

    pMsg->vUINT8[0] = functionId;

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
    pMsg->vUINT16[0] = length;
    pMsg->pCHAR[0] = pCHAR;
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
    pMsg->sz[0] = pCHAR;
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
    pMsg->ppVariableType = ppVariableType;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    CLNTc_getContent (const CHAR *url, UINT8 urlID, BOOL reload, const CHAR *acceptHeader)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;
    size_t  len;

    if (!pMsg)
        return;

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

    pMsg->Id = eCLNTc_getContent;

    pMsg->vUINT8[0] = urlID;
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
    pMsg->sz[0] = pCHAR;
    pMsg->vBOOL[0] = reload;
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
    pMsg->sz[1] = pCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    CLNTc_postContent (const CHAR *url, UINT8 urlID, BOOL reload, const CHAR *acceptHeader, const CHAR *data, UINT16 dataLen, BOOL moreData, const CHAR *contentType, UINT8 sendMode, const CHAR *contentDisp, UINT32 totalSize)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    size_t  len;
    CHAR    *sz, *pCHAR;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_postContent\nconst CHAR * \t url \t %s\nUINT8 \t urlID \t %u\nBOOL \t reload \t %u\nconst CHAR * \t acceptHeader \t %s\nUINT16 \t dataLen \t %u\nBOOL \t moreData \t %u\nconst CHAR * \t contentType \t %s\nUINT8 \t sendMode \t %u\nconst CHAR * \t contentDisp \t %s\nUINT32 \t totalSize \t %u\n", url, (int)urlID, (int)reload, acceptHeader, (int)dataLen, (int)moreData, contentType, (int)sendMode, contentDisp, totalSize);
    }
#endif

    pMsg->Id = eCLNTc_postContent;

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
    pMsg->sz[0] = sz;
    pMsg->vUINT8[0] = urlID;
    pMsg->vBOOL[0] = reload;
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
    pMsg->sz[1] = sz;
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
    pMsg->pCHAR[0] = pCHAR;
    pMsg->vUINT16[0] = dataLen;
	pMsg->vBOOL[1] = moreData;
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
    pMsg->sz[2] = sz;
    pMsg->vUINT8[1] = sendMode;
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
    pMsg->sz[3] = sz;
	pMsg->vUINT32[0] = totalSize;
    
    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    CLNTc_postMoreContent (UINT8 urlID, const CHAR *data, UINT16 dataLen, BOOL moreData)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR *pCHAR;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_postMoreContent\nUINT8 \t urlID \t %u\nUINT16 \t dataLen \t %u\nBOOL \t moreData \t %u\n", (int)urlID, (int)dataLen, (int)moreData);
    }
#endif

    pMsg->Id = eCLNTc_postMoreContent;

    pMsg->vUINT8[0] = urlID;
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
    pMsg->pCHAR[0] = pCHAR;
    pMsg->vUINT16[0] = dataLen;
	pMsg->vBOOL[0] = moreData;
    
    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    CLNTc_acknowledgeContent (UINT8 urlID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_acknowledgeContent\nUINT8 \t urlID \t %u\n", (int)urlID);
    }
#endif

    pMsg->Id = eCLNTc_acknowledgeContent;

    pMsg->vUINT8[0] = urlID;
    
    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    CLNTc_cancelContent (UINT8 urlID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (pMsg == NULL)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_cancelContent\nUINT8 \t urlID \t %u\n", (int)urlID);
    }
#endif

    pMsg->Id = eCLNTc_cancelContent;

    pMsg->vUINT8[0] = urlID;
    
    bSendMsg = SendMsg(pMsg);
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
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

    pMsg->Id = eCLNTc_requestConnectionDone;

    pMsg->vUINT8[0] = objectId;
    pMsg->vBOOL[0] = success;

    bSendMsg = SendMsg(pMsg);

}


EXPORT_C VOID    CLNTc_setupConnectionDone (UINT8 objectId, UINT8 channelId, BOOL success )
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

    pMsg->Id = eCLNTc_setupConnectionDone;

    pMsg->vUINT8[0] = objectId;
	pMsg->vUINT8[1] = channelId;
    pMsg->vBOOL[0] = success;

    bSendMsg = SendMsg(pMsg);

}

EXPORT_C VOID    CLNTc_closeConnection (UINT8 objectId, UINT8 channelId, BOOL deleteChannel )
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

    pMsg->Id = eCLNTc_closeConnection;

    pMsg->vUINT8[0] = objectId;
	pMsg->vUINT8[1] = channelId;
    pMsg->vBOOL[0] = deleteChannel;

    bSendMsg = SendMsg(pMsg);

}

#ifdef USE_PROPRIETARY_WMLS_LIBS

EXPORT_C VOID    CLNTc_WMLSLibFuncResponse(UINT16 invokeId, const WMLSvar *resultVar)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    WMLSvar *pWMLSvar;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        char    szFormat[256];
        size_t  strlength, end = 0;
        const char  *csz;

        strlength = strlen(csz = "CLNTc_WMLSLibFuncResponse\nUINT16 \t invokeId \t %u\nconst \t WMLSvar * \t resultVar \t %p\n \t ");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        if (resultVar)
        {
            strlength = strlen(csz = "UINT8 \t type \t %u\n \t ");
            strncpy(&szFormat[end], csz, strlength);
            end += strlength;
            switch (resultVar->type)
            {
                case 0: /* = integer */
                    strlength = strlen(csz = "INT32 \t intVal \t %i\n");
                    strncpy(&szFormat[end], csz, strlength);
                    end += strlength;
                    szFormat[end] = '\0';
                    CLNTa_log(0, 0, szFormat, (int)invokeId, resultVar, (int)resultVar->type, (int)resultVar->intVal);
                    break;
                case 1: /* = float */
                    strlength = strlen(csz = "FLOAT32 \t floatVal \t %f\n");
                    strncpy(&szFormat[end], csz, strlength);
                    end += strlength;
                    szFormat[end] = '\0';
                    CLNTa_log(0, 0, szFormat, (int)invokeId, resultVar, (int)resultVar->type, (float)resultVar->floatVal);
                    break;
                case 2: /* = string */
                    strlength = strlen(csz = "INT16 \t stringIANAcharset \t %i\n \t UINT32 \t stringLengthInBytes \t %u\n \t CHAR * \t stringVal \t %p\n");
                    strncpy(&szFormat[end], csz, strlength);
                    end += strlength;
                    if (!resultVar->stringVal)
                    {
                        strlength = strlen(csz = " \t ");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        strlength = strlen(csz = cszNULL);
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                    }
                    szFormat[end] = '\0';
                    CLNTa_log(0, 0, szFormat, (int)invokeId, resultVar, (int)resultVar->type, (int)resultVar->stringIANAcharset, (int)resultVar->stringLengthInBytes, resultVar->stringVal);
                    break;
                case 3: /* = boolean */
                    strlength = strlen(csz = "BOOL \t boolVal \t %u\n");
                    strncpy(&szFormat[end], csz, strlength);
                    end += strlength;
                    szFormat[end] = '\0';
                    CLNTa_log(0, 0, szFormat, (int)invokeId, resultVar, (int)resultVar->type, (int)resultVar->boolVal);
                    break;
                case 4: /* = invalid */
                    strlength = strlen(csz = " \t (invalid)\n");
                    strncpy(&szFormat[end], csz, strlength);
                    end += strlength;
                    szFormat[end] = '\0';
                    CLNTa_log(0, 0, szFormat, (int)invokeId, resultVar, (int)resultVar->type);
                    break;
                default:
                    strlength = strlen(csz = " \t (Currently not a supported type.)\n");
                    strncpy(&szFormat[end], csz, strlength);
                    end += strlength;
                    szFormat[end] = '\0';
                    CLNTa_log(0, 0, szFormat, (int)invokeId, resultVar, (int)resultVar->type);
                    break;
            }
        }
        else
        {
            strlength = strlen(csz = cszNULL);
            strncpy(&szFormat[end], csz, strlength);
            end += strlength;
        }

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, (int)invokeId, resultVar);
    }
#endif

    pMsg->Id = eCLNTc_WMLSLibFuncResponse;

    pMsg->vUINT16[0] = invokeId;
    pWMLSvar = (WMLSvar*)OSConnectorAlloc(sizeof(WMLSvar));
    if (!pWMLSvar)
        return;
    memcpy(pWMLSvar, resultVar, sizeof(WMLSvar));
    pWMLSvar->stringVal = NULL; /*  Is this necessary? See declaration of struct in tapiclnt.h */
    if (resultVar->type == 2)
    {
        size_t  len = (size_t)(resultVar->stringLengthInBytes) * sizeof(CHAR);  /* memcpy only accepts size_t anyway */
        CHAR    *pCHAR = (CHAR*)OSConnectorAlloc(len);
        if (!pCHAR)
            return;
        memcpy(pCHAR, resultVar->stringVal, len);
        pWMLSvar->stringVal = pCHAR;
    }
    pMsg->pWMLSvar = pWMLSvar;

    bSendMsg = SendMsg(pMsg);
}

#endif
