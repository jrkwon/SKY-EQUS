


#if !defined(__plapbm_h__)
#define	__plapbm_h__


#include	"irda.h"
#include	"irlap.h"
#include	"irsir.h"
#include	"hlap.h"
#include	"okmisc.h"
#include	"timer.h"


		byte	IrDAFAR	LAP_SendCmdBuffer[IrLAP_CMDBUFSZ] ;	


															
volatile byte	IrDAFAR	* IrDAFAR RecvAddrTbl[IrLAPRECVTBLSIZE] ;
volatile uint	IrDAFAR	RecvSizeTbl[IrLAPRECVTBLSIZE] ;		
volatile uint	IrDAFAR	RecvFrameSize ;						


#if defined(IrDASECONDARY)
#include	"plapsm.h"
#endif	
#if defined(IrDAPRIMARY)
#include	"plappm.h"
#endif	


#endif	
