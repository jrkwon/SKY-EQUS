#include "tapicmmn.h"


VOID    WDa_sendRequest (const CHAR *data, UINT16 dataLength, UINT16 destinationPort, UINT16 sourcePort)
{
    extern void    CLNTa_log (unsigned char viewId, signed short logNo, const char *format, ...);
    CLNTa_log(0, 0, "WDa_sendRequest\n");
}
