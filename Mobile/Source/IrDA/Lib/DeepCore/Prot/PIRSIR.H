


#if !defined(__pirsir_h__)
#define	__pirsir_h__
#if defined(MAKE_DOCUMENT)


#include	"irsir.h"


int		IrDAAPI	IrSIR_Open( void )
{
}


void	IrDAAPI	IrSIR_Close( void )
{
}


uint	IrDAAPI	IrSIR_GetCapability( const void IrDAFAR * IrDAFAR *ptr )
{
}


void	IrDAAPI	IrSIR_SetDataInd( void (IrDACB *funcp)( void ))
{
}


void	IrDAAPI	IrSIR_SetDataCnf( void (IrDACB *funcp)( void ))
{
}


void	IrDAAPI	IrSIR_InitConnectionParameters( void )
{
}


void	IrDAAPI	IrSIR_SetSpeed( dword spd )
{
}


IrDABOOL	IrDAAPI	IrSIR_Busy( void )
{
}


IrDABOOL	IrDAAPI	IrSIR_InFrame( void )
{
}


void	IrDAAPI	IrSIR_SetFrameBuffer( IrSIRFRAMECONFIG IrDAFAR *frameconfig )
{
}


void	IrDAAPI	IrSIR_SetFrameMode( void )
{
}


#if 0
void	IrDAAPI	IrSIR_SetByteMode( void )
{
}
#endif


void	IrDAAPI	IrSIR_SetRecvMode( void )
{
}


void	IrDAAPI	IrSIR_ResetRecvMode( void )
{
}


void	IrDAAPI	IrSIR_SetConnectionAddress( uint ca )
{
}


void	IrDAAPI	IrSIR_SetMinimamTurnaroundTime( uint msec )
{
}


void	IrDAAPI	IrSIR_SetBOF( uint bofsz )
{
}


void	IrDAAPI	IrSIR_SendFrame( uint datasz, void IrDAFAR *data )
{
}


ulong	IrDAAPI	IrSIR_ReadTimer( void )
{
}


void		IrDAAPI	IrDA_SetPowerLevel( uint level )
{
}


uint		IrDAAPI	IrDA_GetPowerLevel( void )
{
}


IrDABOOL	IrDAAPI	IrDA_GetPowerMode( void )
{
}


void	IrDAAPI	IrDA_PowerOn( void )
{
}


void	IrDAAPI	IrDA_PowerOff( void )
{
}


int		IrDAAPI	StartTimer( dword msec, void (IrDACB *func)( void ))
{
}


void	IrDAAPI	StopTimer( int hndl )
{
}


void	IrDAAPI	SetTimer( dword IrDAFAR *timer, dword msec )
{
}


dword	IrDAAPI	GetTimer( dword timer )
{
}


#endif	
#endif	
