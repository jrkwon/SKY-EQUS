#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include "aapiussd.h"
#include "capiussd.h"
#include "aapiclnt.h"
#include "cmmnrsrc.h"


VOID    USSDa_sendInvokeProcessRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength)
{
    CLNTa_log(0, 0, "USSDa_sendInvokeProcessRequest\nUINT8 \t dataCodingScheme \t %u\nconst CHAR * \t data \t %s\nUINT8 \t stringLength \t %u\n", (int)dataCodingScheme, data, (int)stringLength);

#ifndef CBASIC_WITH_NO_CONNECTORS
    /*  RETURN  */
    USSDc_receivedResultProcessRequest(dataCodingScheme, data, stringLength);
#endif
}


VOID    USSDa_sendResultRequest (UINT8 dataCodingScheme, const CHAR *data, UINT8 stringLength)
{
    #ifdef LOG_INTERNAL
    char    *str = OctetString2HexString(data, stringLength);
    CLNTa_log(0, 0, "USSDa_sendResultRequest\nUINT8 \t dataCodingScheme \t %u\nconst CHAR * \t data \t %s\nUINT8 \t stringLength \t %u\n", (int)dataCodingScheme, str, (int)stringLength);
    OSConnectorFree(str);
    #endif

    /*  RETURN  */
#ifndef CBASIC_WITH_NO_CONNECTORS
    USSDc_receivedInvokeRequest(dataCodingScheme, data, stringLength);
#endif
}


VOID    USSDa_sendAbort (VOID)
{
    CLNTa_log(0, 0, "USSDa_sendAbort\nVOID\n");

    /*  RETURN  */
#ifndef CBASIC_WITH_NO_CONNECTORS
    USSDc_receivedRelease();
#endif
}
