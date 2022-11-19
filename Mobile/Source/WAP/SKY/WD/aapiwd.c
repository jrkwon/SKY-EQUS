#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
#endif //#ifdef WAP_NEO

#include "tapicmmn.h"


VOID    WDa_sendRequest (const CHAR *data, UINT16 dataLength, UINT16 destinationPort, UINT16 sourcePort)
{
#ifdef WAPCORE_FOR_WARNING
	extern void  CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);
#endif //WAPCORE_FOR_WARNING
    CLNTa_log(0, 0, "WDa_sendRequest\n");
}
