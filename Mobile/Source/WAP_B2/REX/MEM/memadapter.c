#include "wapconnector.h"
#include "capiclnt.h"
#include "aapiclnt.h"


extern BOOL bTerminatewap;
VOID    MEMa_cachePrepared (VOID) 
{
	CLNTa_log(0, 0, "MEMa_cachePrepared Called\n======================================\n");
	
	if(bTerminatewap)
	{
		bTerminatewap = FALSE;CLNTc_terminate();
	}
	else//resize¿ë
	{
		CLNTa_log(0, 0, "Resize cache.");	
	}
	CLNTa_log(0, 0, "CLNTc_terminate Called\n====================================\n");
}
