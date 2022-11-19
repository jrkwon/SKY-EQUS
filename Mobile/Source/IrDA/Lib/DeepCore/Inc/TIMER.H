


#if !defined(__timer_h__)
#define		__timer_h__


#include	"irda.h"


#if defined(__cplusplus)
extern	"C"	{
#endif	


int		IrDAAPI	StartTimer( dword msec, void (IrDACB *func)( void )) ;
void	IrDAAPI	StopTimer( int hndl ) ;
void	IrDAAPI	SetTimer( dword IrDAFAR *timer, dword msec ) ;
dword	IrDAAPI	GetTimer( dword timer ) ;


#if defined(__cplusplus)
}
#endif	


#endif	