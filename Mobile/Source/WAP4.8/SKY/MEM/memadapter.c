#include "wapconnector.h"
#include "capiclnt.h"
#include "aapiclnt.h"


#ifdef FEATURE_WAP
BOOL bTerminatewap;
#else
extern BOOL bTerminatewap;
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_WAP
extern void ProcessEvents();
#endif //#ifdef FEATURE_WAP

VOID    MEMa_cachePrepared (VOID) 
{
	CLNTa_log(0, 0, "MEMa_cachePrepared Called\n======================================\n");
    CLNTc_terminate();
    ProcessEvents();
	CLNTa_log(0, 0, "CLNTc_terminate Called\n====================================\n");
}
