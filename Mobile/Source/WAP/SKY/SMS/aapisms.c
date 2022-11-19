#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
#endif //#ifdef WAP_NEO

#include <ctype.h>
#include <string.h>

#include "aapiclnt.h"
#include "cmmnrsrc.h"


static  CHAR    hex2dec(CHAR ch)
{
    if ((ch >= '0') && (ch <= '9')) /* isdigit() */
        ch -= 48;
    else if ((ch >= 'A') && (ch <= 'Z')) /* isupper() */
        ch -= 55;   /* 10 + ch - 65 */
    else
        ch -= 87;   /* 10 + ch - 97 */
    return ch;
}

static  CHAR *  response(size_t *len, CHAR *data, char nbr)
{
    size_t  i, length;
    CHAR    *d;

    *len = length = strlen(data) / 2;
    d = (CHAR *) WIPAlloc(length);

    for (i = 0; i < length; i++)
    {
        CHAR    ch;
        ch = hex2dec(data[i * 2]);
        ch = (CHAR) (ch << 4);
        ch |= hex2dec(data[(i * 2) + 1]);
        d[i] = ch;
    }

    d[0] = 0xB;
    d[1] = 0x5;
    d[2] = 0x4;
/*
    d[3] = 0x;
    d[4] = 0x;
*/
    d[5] = 0x0;
    d[6] = 0xF;
    d[7] = 0;
    d[8] = 0x3;
    d[9] = 0;
    d[10] = 0x3;
    d[11] = nbr;
    if (nbr == 1)
    {
        d[12+1] = 0x04; /* id for reply */
        d[12+2] = 0x20; /* status */
        d[12+3] = 0x01; /* length of header part */
    }

    return d;
}

static  CHAR *  responseDeck1(size_t *len, char nbr)
{
    CHAR    *d = NULL;

    /* content wml = 94, resten body */
    /*  SMS */
    switch (nbr)
    {
        /*  The header for the first segment of the message is longer than the the rest of the segments. */
        case 1:
            d = response(len, "                                9401046a007f67600348656C6C6F20", 1);
            break;
        case 2:
            d = response(len, "                        776F726C6421", 2);
            break;
        case 3:
            d = response(len, "                        00010101", 3);
            break;
    }

    return d;
}


VOID    SMSa_sendRequest (const CHAR *smsc, UINT8 smscLength, const CHAR *destination, UINT8 destLength, const CHAR *data, UINT8 dataLength)
{
    #ifdef LOG_INTERNAL
    char    *str1 = OctetString2HexString(smsc, smscLength);
    char    *str2 = OctetString2HexString(destination, destLength);
    char    *str3 = OctetString2HexString(data, dataLength);
    CLNTa_log(0, 0, "SMSa_sendRequest\nconst CHAR * \t smsc \t %s\nconst CHAR * \t destination \t %s\nconst CHAR * \t data \t %s\nUINT8 \t dataLength \t %u\n", str1, str2, str3, (int)dataLength);
    WIPFree(str1);
    WIPFree(str2);
    WIPFree(str3);
    #endif

#ifndef CBASIC_WITH_NO_CONNECTORS
#ifndef FEATURE_WAP
    SMSc_sentRequest();
#endif //#ifndef FEATURE_WAP

    /*  RETURN  */

    /*  DECK 1  */
    {
        size_t  len;
        CHAR    *response;
        CHAR    *source;
        UINT8   sourceLength;
        char    i;

        for (i = 1; i <= 3; i++)
        {
            response = responseDeck1(&len, i);
            source = (CHAR *)destination;
            sourceLength = destLength;

            response[3] = data[5];
            response[4] = data[6];

            if (i == 1)
            {
                response[12] = data[7]; /* TId */
            }
#ifndef FEATURE_WAP
            SMSc_receivedRequest(source, sourceLength, response, (UINT8)len);
#endif //FEATURE_WAP

            WIPFree(response);
        }
    }
#endif
}
