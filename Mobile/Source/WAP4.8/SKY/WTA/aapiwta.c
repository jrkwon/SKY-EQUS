#include "customer.h"
#include <string.h>
#include "sky.h"
#include "aapiclnt.h"
#include "aapiwta.h"
#include "capiwta.h"
////////////////////////////////////////////////////////////
// A7 Porting
/////////////////////////////////////////////////////////////



extern VOID CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

CHAR WTAnumBuf[15];
int  WTAlength =0;

#ifdef FEATURE_WAP
VOID    WTAIa_publicMakeCall (UINT8 objectId, const CHAR *number)
#else
VOID    WTAIa_publicMakeCall (const CHAR *number)
#endif //FEATURE_WAP
{
	BYTE        len;
	CHAR*       p;
	int         i, j;
    CLNTa_log(1,0,"objectId=%d",objectId);

	len =  strlen(number);
	CLNTa_log(1,0,"argument %s\nlength=%d",number,len);

	if (len > 0)
	{
		p = (CHAR*)number;
		j = 0;
		memset(WTAnumBuf,0x00,15);
		WTAlength = 0;
		for(i=0;i<len;i++)
		{
			CLNTa_log(1,0,"HERE WTA");    
			if((p[i]>=0x30)&&(p[i]<=0x3a))
			{
			   WTAnumBuf[j] = p[i]; j++;
			}
		}
		CLNTa_log(1,0," %s",WTAnumBuf);
		WTAlength = j;		
	}
	else
	{
		//ERRORÃ³¸®...
	}
    
	EQS_SEND_EVENT(SE_WAP_MAKECALL, EQS_NA, EQS_NA);

}

#ifdef FEATURE_WAP
VOID    WTAIa_publicSendDTMF (UINT8 objectId, const CHAR *number)
#else
VOID    WTAIa_publicSendDTMF (const CHAR *number)
#endif //FEATURE_WAP
{
	CLNTa_log(0, 0, "WTAIa_publicSendDTMF \n");
}

VOID    WTAa_confirmInstallation (INT8 installId, const WCHAR *wtaChannelId, const WCHAR *title, const WCHAR *abstract)
{
	CLNTa_log(0, 0, "WTAa_confirmInstallation ");
}

VOID    WTAa_retryGetInstallationResult (INT8 installId)
{
	CLNTa_log(0, 0, "WTAa_retryGetInstallationResult ");
}

VOID    WTAa_showInstallationResult (INT8 installId, const CHAR *url)
{
	CLNTa_log(0, 0, "WTAa_showInstallationResult ");
}

VOID    WTAa_services (const ServiceType * const *services)
{
	CLNTa_log(0, 0, "WTAa_services ");
}

VOID	WTAa_serviceDeleted (const WCHAR *wtaChannelId)
{
    CLNTa_log(0, 0, "WTAa_serviceDeleted ");
}


VOID    WTAa_processedByAService (BOOL processed)
{
	CLNTa_log(0, 0, "WTAa_processedByAService");
}

