


#if !defined(__ptimer_h__)
#define	__ptimer_h__


#include	"irda.h"
#include	"timer.h"
#include	"irsir.h"


void	IrDAAPI	SetTimer( dword IrDAFAR *timer, dword msec )
{
	*timer	= IrSIR_ReadTimer() + msec ;
}


dword	IrDAAPI	GetTimer( dword timer )
{
	dword	now ;


	now		= IrSIR_ReadTimer() ;
	if( now >= timer )
		return( 0 ) ;


	return( timer - now ) ;
}


#endif	
