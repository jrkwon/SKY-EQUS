/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     clntPIM.c
**
** Description: 
**     Phonebook library Function 
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-02-19 ytchoi     Created.
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sky.h"
#include "ska.h"
#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif
#include "clnt.h"
#include "clntapp.h"
#include "aapiwta.h"



/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

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
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/*EQS_API*/


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
VOID    WTAIa_publicAddPBEntry (UINT8 objectId, const CHAR *number, const WCHAR *name)
{
    CLNTa_log(1,0,"DUMMY WTAIa_publicAddPBEntry  ");    
}

VOID    WTAIa_voiceCallSetup (UINT8 objectId, const WCHAR *wtaChannelId, const CHAR *number)
{
    CLNTa_log(1,0,"DUMMY WTAIa_voiceCallSetup ");    
}

VOID    WTAIa_voiceCallAccept (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle)
{
    CLNTa_log(1,0,"DUMMY WTAIa_voiceCallAccept ");    
}

VOID    WTAIa_voiceCallRelease (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, BOOL enableResponse)
{
    CLNTa_log(1,0,"DUMMY WTAIa_voiceCallRelease ");    
}

VOID    WTAIa_voiceCallSendDTMF (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, const CHAR *dtmf)
{
    CLNTa_log(1,0,"DUMMY WTAIa_voiceCallSendDTMF ");    
}

VOID    WTAIa_voiceCallCallStatus (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, INT8 field)
{
    CLNTa_log(1,0,"DUMMY WTAIa_voiceCallCallStatus ");    
}

VOID    WTAIa_voiceCallList (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst)
{
    CLNTa_log(1,0,"DUMMY WTAIa_voiceCallList");    
}



VOID    WTAIa_netTextSend (UINT8 objectId, const WCHAR *wtaChannelId, const CHAR *number, const WCHAR *text)
{
    CLNTa_log(1,0,"DUMMY WTAIa_netTextSend ");    
}

VOID    WTAIa_netTextList (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst, INT8 messageType)
{
    CLNTa_log(1,0,"DUMMY WTAIa_netTextList ");    
}

VOID    WTAIa_netTextRemove (UINT8 objectId, const WCHAR *wtaChannelId, INT16 msgHandle)
{
    CLNTa_log(1,0,"DUMMY WTAIa_netTextRemove ");    
}

VOID    WTAIa_netTextGetFieldValue (UINT8 objectId, const WCHAR *wtaChannelId, INT16 msgHandle, INT8 field)
{
    CLNTa_log(1,0,"DUMMY WTAIa_netTextGetFieldValue ");    
}

VOID    WTAIa_netTextMarkAsRead (UINT8 objectId, const WCHAR *wtaChannelId, INT16 msgHandle)
{
    CLNTa_log(1,0,"DUMMY WTAIa_netTextMarkAsRead");    
}


VOID    WTAIa_phoneBookWrite (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index, const CHAR *number, const WCHAR *name)
{
    CLNTa_log(1,0,"DUMMY WTAIa_phoneBookWrite");    
}

VOID    WTAIa_phoneBookSearch (UINT8 objectId, const WCHAR *wtaChannelId, INT8 field, const WCHAR *value)
{
    CLNTa_log(1,0,"DUMMY WTAIa_phoneBookSearch");    
}

VOID    WTAIa_phoneBookRemove (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index)
{
    CLNTa_log(1,0,"DUMMY WTAIa_phoneBookRemove");    
}

VOID    WTAIa_phoneBookGetFieldValue (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index, INT8 field)
{
    CLNTa_log(1,0,"DUMMY WTAIa_phoneBookGetFieldValue");    
}

VOID    WTAIa_phoneBookChange (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index, INT8 field, const WCHAR *newValue)
{
    CLNTa_log(1,0,"DUMMY WTAIa_phoneBookChange");    
}



VOID    WTAIa_miscSetIndicator (UINT8 objectId, const WCHAR *wtaChannelId, INT8 type, INT8 newState)
{
    CLNTa_log(1,0,"DUMMY WTAIa_miscSetIndicator");    
}



VOID    WTAIa_callLogDialled (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst)
{
    CLNTa_log(1,0,"DUMMY WTAIa_callLogDialled");    
}

VOID    WTAIa_callLogMissed (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst)
{
    CLNTa_log(1,0,"DUMMY WTAIa_callLogMissed");    
}

VOID    WTAIa_callLogReceived (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst)
{
    CLNTa_log(1,0,"DUMMY WTAIa_callLogReceived");    
}

VOID    WTAIa_callLogGetFieldValue (UINT8 objectId, const WCHAR *wtaChannelId, INT16 logHandle, INT8 field)
{
    CLNTa_log(1,0,"DUMMY WTAIa_callLogGetFieldValue");    
}



VOID    WTAIa_GSMHold (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMHold");    
}

VOID    WTAIa_GSMRetrieve (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMRetrieve");    
}

VOID    WTAIa_GSMTransfer (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandleB, INT16 callHandleC)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMTransfer");    
}

VOID    WTAIa_GSMDeflect (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, const CHAR *number)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMDeflect");    
}

VOID    WTAIa_GSMMultiparty (UINT8 objectId, const WCHAR *wtaChannelId)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMMultiparty");    
}

VOID    WTAIa_GSMSeparate (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMSeparate");    
}

VOID    WTAIa_GSMSendUSSD (UINT8 objectId, const WCHAR *wtaChannelId, const WCHAR *message, const WCHAR *codingScheme, INT8 type, const CHAR *transactionId)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMSendUSSD");    
}

VOID    WTAIa_GSMNetinfo (UINT8 objectId, const WCHAR *wtaChannelId, INT8 type)
{
    CLNTa_log(1,0,"DUMMY WTAIa_GSMNetinfo");    
}
