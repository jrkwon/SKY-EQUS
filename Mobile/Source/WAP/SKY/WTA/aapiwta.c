#include "customer.h"
#include <string.h>

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
#endif //#ifdef WAP_NEO

#include "sky.h"

#include "aapiclnt.h"
#include "aapiwta.h"
/////////////////////////////////////////////////////////////
// A7 Porting
/////////////////////////////////////////////////////////////



extern VOID CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);


static ui_cmd_type   *ui_cmd_ptr;


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
#ifndef FEATURE_WAP
	BYTE        len;
	CHAR*       p;
	int         i, j;

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
		//ERRORó��...
	}

	if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
	{	
		//ERR_FATAL( "No UI cmd buffers",0,0,0);
	}
	else		
	{
		ui_cmd_ptr->hdr.cmd        = UI_WAP_MAKECALL_F;
		ui_cmd_ptr->hdr.task_ptr   = NULL;
		ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
		ui_cmd( ui_cmd_ptr);
		//CLNTa_log(0, 0, "WTACALL");
	}
#endif //FEATURE_WAP
}

#ifdef FEATURE_WAP
VOID    WTAIa_publicSendDTMF (UINT8 objectId, const CHAR *number)
#else
VOID    WTAIa_publicSendDTMF (const CHAR *number)
#endif //FEATURE_WAP
{
	CLNTa_log(0, 0, "WTAIa_publicSendDTMF \n");
}

INT8    WTAIa_publicPBwrite (const CHAR *number, const WCHAR *name)
{
	CLNTa_log(0, 0, "WTAIa_publicPBwrite \n");
#ifdef WAPCORE_FOR_WARNING
	MSG_LOW(" Dummy for WARNING in aapowta.c",0,0,0);
	return 0;//Dummy
#endif //WAPCORE_FOR_WARNING
}

VOID    WTAa_confirmInstallation (INT8 installId, const WCHAR *title, const WCHAR *abstract)
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

VOID    WTAa_processedByAService (BOOL processed)
{
	CLNTa_log(0, 0, "WTAa_processedByAService");
}

