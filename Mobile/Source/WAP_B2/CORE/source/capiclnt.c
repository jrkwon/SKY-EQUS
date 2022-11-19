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
#include "wiptrgt.h"

#include "aapiclnt.h"
#include "capiclnt.h"
#include "cmmnrsrc.h"
#include "msgqueue.h"
#include "trnscode.h"
#include "ml_typ.h"
#include "mk_queu.h"
#include "config.h"
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

#if defined USE_CHARSET_PLUGIN
VOID initialiseCharsetPlugin(VOID);
#endif

#ifdef WAP_USE_LEAK
void SendCmdtoUI(void);
#endif//#ifdef WAP_USE_LEAK


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
            deleteAllMemory();
            DeleteQueue();
            DeleteAllConfig();
#ifdef WAP_USE_LEAK
			SendCmdtoUI();
#endif//#ifdef WAP_USE_LEAK
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
            deleteAllMemory();
            DeleteQueue();
            DeleteAllConfig();
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

#define MAX_NO_OF_XDORMANT_STATES 36

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
    #if defined USE_WIP_MALLOC && defined INITIALISE_WIP_MALLOC
        if (firstRun)
        {
            wip_initmalloc(wip_memory, WIP_MALLOC_MEM_SIZE);
        }
    #endif

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

    InitQueue();

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
    deleteAllTimers();
    deleteAllMemory();
    DeleteQueue();
    DeleteAllConfig();
    CLNTa_terminated();
}


EXPORT_C VOID   CLNTc_initialised (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_initialised\nVOID\n");
    }
#endif

    pMsg->Id = eCLNTc_initialised;

    bSendMsg = SendMsg(pMsg);
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


EXPORT_C VOID    CLNTc_setIntConfig (UINT8 viewId, ConfigInt kind, UINT32 value)
{
    ConfigValue cnfgValue;

#if     defined LOG_EXTERNAL
    {
        char    szFormat[256];
        size_t  strlength, end = 0;
        const char  *csz;
        const char  *cszConfig = NULL;

        strlength = strlen(csz = "CLNTc_setIntConfig\nConfigInt \t kind \t ");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (kind)
        {
            case configSTACKMODE:
                cszConfig = "configSTACKMODE\n";
                break;

            case configBEARER:              /*  Currently supported bearers are: UDP = 0, USSD = 2 and SMS = 3. */
                strlength = strlen(csz = "configBEARER\nUINT32 \t value \t %u");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                /*  Currently supported bearers are: UDP = 0, USSD = 2 and SMS = 3. */
                switch (value)
                {
                    case BEARER_UDP:    /* UDP  */
                        strlength = strlen(csz = " \t (UDP)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    case BEARER_USSD:   /* USSD */
                        strlength = strlen(csz = " \t (USSD)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    case BEARER_SMS:    /* SMS  */
                        strlength = strlen(csz = " \t (SMS)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    default:
                        strlength = strlen(csz = " \t (Currently not a supported bearer.)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                }
                break;
            case configUSSD_SC_TYPE:        /*  Currently supported service code types are: none= 0, msisdn= 1 and ip= 2.   */
                strlength = strlen(csz = "configUSSD_SC_TYPE\nUINT32 \t value \t %u");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                /*  Currently supported service code types are: none= 0, msisdn= 1 and ip= 2.   */
                switch (value)
                {
                    case 0:     /* none     */
                        strlength = strlen(csz = " \t (none)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    case 1:     /* msisdn   */
                        strlength = strlen(csz = " \t (msisdn)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    case 2:     /* ip       */
                        strlength = strlen(csz = " \t (ip)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    default:
                        strlength = strlen(csz = " \t (Currently not a supported service code.)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                }
                break;
            case configHISTORY_SIZE:        /*  How many URLs should be held in the history.    */
                cszConfig = "configHISTORY_SIZE\n";
                break;
            case configTIMEOUT:             /*  The time in seconds the client can wait, when downloading has stalled, before the transaction is cancelled. */
                cszConfig = "configTIMEOUT\n";
                break;
            case configCACHE_AGE:           /*  The time in seconds a cached item shall be in the cache, if no "expires date" is given with the downloaded item.    */
                cszConfig = "configCACHE_AGE\n";
                break;
            case configCACHE_MODE:          /*  Supported cache modes are:
                                                0: Always check if a newer version of the item is available on server
                                                1: After the Generic WAP Client has been started, check the first time a cached item is accessed  if a newer version is available on server
                                                2: Never check if newer version is available on server. (Always use cached version) */
                strlength = strlen(csz = "configCACHE_MODE\nUINT32 \t value \t %u");
                strncpy(&szFormat[end], csz, strlength);
                end += strlength;
                switch (value)
                {
                    case 0:
                        strlength = strlen(csz = ":    Always check if a newer version of the item is available on server\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    case 1:
                        strlength = strlen(csz = ":    After the Generic WAP Client has been started, check the first time a cached item is accessed  if a newer version is available on server\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    case 2:
                        strlength = strlen(csz = ":    Never check if newer version is available on server. (Always use cached version)");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                    default:
                        strlength = strlen(csz = ":    (Currently not a supported cache mode.)\n");
                        strncpy(&szFormat[end], csz, strlength);
                        end += strlength;
                        break;
                }
                break;
            case configDISPLAY_IMAGES:      /*  1 if images can be displayed, 0 otherwise.  */
                cszConfig = "configDISPLAY_IMAGES\n";
                break;
            case configUPDATE_IMAGES:       /*  1 if images can be displayed after the card has been displayed, 0 otherwise.    */
                cszConfig = "configUPDATE_IMAGES\n";
                break;
            default:
                cszConfig = "UNVALID ConfigInt\n";
                break;
        }
        if (cszConfig)
        {
            strlength = strlen(cszConfig);
            strncpy(&szFormat[end], cszConfig, strlength);
            end += strlength;
            strlength = strlen(csz = "UINT32 \t value \t %u\n");
            strncpy(&szFormat[end], csz, strlength);
            end += strlength;
        }

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, (int)value);
    }
#endif

    cnfgValue.u = value;
    NewConfig(viewId, (UINT8)kind, cnfgValue, 0);
}


EXPORT_C VOID    CLNTc_setStrConfig (UINT8 viewId, ConfigStr kind, const CHAR *value, UINT8 length)
{
    CHAR    *pCHAR;
    ConfigValue cnfgValue;

#if     defined LOG_EXTERNAL
    {
        const unsigned char cNmbrOfFldsIPAdr = 4;
        char    szFormat[256];
        size_t  strlength, end = 0;
        char    *sz;
        const char  *csz, *cszValue = value;
        BOOL    bFree = FALSE;

        strlength = strlen(csz = "CLNTc_setStrConfig\nConfigStr \t kind \t ");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (kind)
        {
            case configUDP_IP_SRC:       /*  ip address  a.b.c.d -> abcd, i.e., the bytes from left to right.    */
                csz = "configUDP_IP_SRC\n";
                cszValue = sz = OctetString2HexString(value, cNmbrOfFldsIPAdr);
                if (!sz)
                    return;
                bFree = TRUE;
                break;
            case configUDP_IP_GW:        /*  ip address  a.b.c.d -> abcd, i.e., the bytes from left to right.    */
                csz = "configUDP_IP_GW\n";
                cszValue = sz = OctetString2HexString(value, cNmbrOfFldsIPAdr);
                if (!sz)
                    return;
                bFree = TRUE;
                break;
            case configSMS_C:           /*  msisdn number for SMS center    e.g. "+460703123456"    */
                csz = "configSMS_C\n";
                break;
            case configSMS_GW:          /*  msisdn number for WAP server    e.g. "+460703123456"    */
                csz = "configSMS_GW\n";
                break;
            case configUSSD_C:          /*  msisdn number for USSD center   e.g. "+460703123456"    */
                csz = "configUSSD_C\n";
                break;
            case configUSSD_SC:         /*  Three types of service codes, depending on the value of the configUSSD_SC_TYPE variable. Examples:  no SC. ""  msisdn: "+460703123456"  ip: "abcd" */
                csz = "configUSSD_SC\n";
                break;
            case configAUTH_PASS_GW:    /*  The password for the WAP proxy server   */
                csz = "configAUTH_PASS_GW\n";
                break;
            case configAUTH_ID_GW:      /*  The user id for the WAP proxy server    */
                csz = "configAUTH_ID_GW\n";
                break;
            case configWSP_Language:    /*  A string */
                csz = "configWSP_Language\n";
                break;
            default:
                csz = "ConfigStr\n";
                break;
        }
        strlength = strlen(csz);
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        strlength = strlen(csz = "const CHAR * \t value \t %s\n");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        strlength = strlen(csz = "UINT8 \t length \t %u\n");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, cszValue, length);
        if (bFree)
        {
            OSConnectorFree(sz);
        }
    }
#endif

    pCHAR = NULL;
    if (length)
    {
        if (value)
        {
            pCHAR = (CHAR*)OSConnectorAlloc(length);
            if (!pCHAR)
                return;
            memcpy(pCHAR, value, length);
        }
        else
        {
            length = 0;
        }
    }

    cnfgValue.p = pCHAR;
    NewConfig(viewId, (UINT8)kind, cnfgValue, length);
}


EXPORT_C VOID    CLNTc_usePort (UINT8 viewId, UINT8 id, UINT16 port, BOOL success)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(viewId, 0, "CLNTc_usePort\nUINT8 \t id \t %u\nUINT16 \t port \t %u\nBOOL \t success \t %u\n", (int)id, (int)port, (int)success);
    }
#endif

    pMsg->Id = eCLNTc_usePort;

    pMsg->vUINT8[0] = viewId;
    pMsg->vUINT8[1] = id;
    pMsg->vUINT16[0] = port;
    pMsg->vBOOL = success;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    CLNTc_closePort (UINT16 port)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_closePort\nUINT16 \t port \t %u\n", (int)port);
    }
#endif

    pMsg->Id = eCLNTc_closePort;

    pMsg->vUINT16[0] = port;

    bSendMsg = SendMsg(pMsg);
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
    pMsg->sz = pCHAR;

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
    pMsg->sz = pCHAR;
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
		// B1 for WARNING 
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
#ifdef WAPCORE_FOR_WARNING
			pcWCHAR = pcvariableType->name;
			if (pcWCHAR)
#else//WAPCORE_FOR_WARNING
			if (pcWCHAR = pcvariableType->name)
#endif //WAPCORE_FOR_WARNING            
            {
                len = w_strlen(pcWCHAR) + 1;
                pWCHAR = (WCHAR *)OSConnectorAlloc(len * sizeof(WCHAR));
                if (!pWCHAR)
                    return;
                w_strcpy(pWCHAR, pcWCHAR);
            }
            pVariableType->name = pWCHAR;
            pWCHAR = NULL;
			pcWCHAR = pcvariableType->value;
#ifdef WAPCORE_FOR_WARNING
			pcWCHAR = pcvariableType->name;
			if (pcWCHAR)
#else//WAPCORE_FOR_WARNING
            if (pcWCHAR = pcvariableType->value)
#endif //WAPCORE_FOR_WARNING
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


EXPORT_C VOID    CLNTc_getContent (const CHAR *url, UINT8 urlID, BOOL isOTAallowed)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *pCHAR;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const CHAR  *_url = url;
        if (!_url)
        {
            _url = cszNULL;
        }
        CLNTa_log(0, 0, "CLNTc_getContent\nconst CHAR * \t url \t %s\nUINT8 \t urlID \t %u\nBOOL \t isOTAallowed \t %u\n", _url, (int)urlID, (int)isOTAallowed);
    }
#endif

    pMsg->Id = eCLNTc_getContent;

    pMsg->vBOOL = isOTAallowed;
    pMsg->vUINT8[0] = urlID;
    pCHAR = NULL;
    if (url)
    {
        size_t  len = strlen(url);
        /*  Add one extra byte for the terminating zero.*/
        pCHAR = (CHAR*)OSConnectorAlloc(++len);
        if (!pCHAR)
            return;
        memcpy(pCHAR, url, len);
		CLNTa_log(1, 0, "[CLNTc_getContent]URL[%s]\n",pCHAR);	
    }
    pMsg->sz = pCHAR;

    bSendMsg = SendMsg(pMsg);
}


EXPORT_C VOID    CLNTc_confirmDownload (INT16 confirmId, BOOL isOTAallowed)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "CLNTc_confirmDownload\nINT16 \t confirmId \t %i\nBOOL \t isOTAallowed \t %u\n", (int)confirmId, (int)isOTAallowed);
    }
#endif

    pMsg->Id = eCLNTc_confirmDownload;

    pMsg->vBOOL = isOTAallowed;
    pMsg->vINT16 = confirmId;

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


#ifdef USE_PROPRIETARY_WMLS_LIBS

EXPORT_C VOID    CLNTc_WMLSLibFuncResponse(UINT16 invokeId, const WMLSvar *resultVar)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    WMLSvar *pWMLSvar;
	CHAR    *pCHAR;

    if (!pMsg)
        return;

    pMsg->Id = eCLNTc_WMLSLibFuncResponse;

    pMsg->vUINT16[0] = invokeId;
    pWMLSvar = (WMLSvar*)OSConnectorAlloc(sizeof(WMLSvar));
    if (!pWMLSvar)
        return;
    memcpy(pWMLSvar, resultVar, sizeof(WMLSvar));
    //pWMLSvar->stringVal = NULL; /*  Is this necessary? See declaration of struct in tapiclnt.h */

	//CLNTa_log(0, 0, "CLNTc_WMLSLibFuncResponse] Type[%d] ",resultVar->type );

    if (resultVar->type == 2)
    {
		unsigned int strLength;

		strLength =(unsigned int)resultVar->stringLengthInBytes;
		
        pCHAR = (CHAR*)OSConnectorAlloc(strLength+2);

        if (!pCHAR)
            return;

		//I am not sure that stringVal is null terminating or not. So change strcpy to memcpy^^
        //strcpy(pCHAR, resultVar->stringVal);
		memcpy(pCHAR, resultVar->stringVal, strLength);

		WIPFree(resultVar->stringVal);

        pWMLSvar->stringVal = pCHAR;

		//CLNTa_log(0, 0, "CLNTc_WMLSLibFuncResponse \npCHAR[%x],result[%x]", pCHAR, resultVar->stringVal);
		//CLNTa_log(0, 0, "CLNTc_WMLSLibFuncResponse \n±¸Á¶Ã¼[%x], ¸â¹ö[%x]", pWMLSvar, pWMLSvar->stringVal);
    }
    pMsg->pWMLSvar = pWMLSvar;

    bSendMsg = SendMsg(pMsg);
}


#endif


