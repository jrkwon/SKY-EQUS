


#if !defined(__irsir_h__)
#define		__irsir_h__


#include	"irda.h"


typedef	struct	_IrSIRFRAMECONFIG {
				byte	IrDAFAR	*RecvBuffer ;			
				uint			RecvBufferSize ;		
				uint			TblSize ;				
	volatile	byte	IrDAFAR	* IrDAFAR *RecvBufTbl ;	
	volatile	uint	IrDAFAR	*RecvSizeTbl ;			
	volatile	uint	IrDAFAR	*RecvWindow ;			
}	IrSIRFRAMECONFIG ;


#if defined(__cplusplus)
extern	"C"	{
#endif	


int		IrDAAPI	IrSIR_Open( void ) ;
void	IrDAAPI	IrSIR_Close( void ) ;
uint	IrDAAPI	IrSIR_GetCapability( const void IrDAFAR * IrDAFAR *ptr ) ;
													
void	IrDAAPI	IrSIR_SetDataInd( void (IrDACB *funcp)( void )) ;
													
void	IrDAAPI	IrSIR_SetDataCnf( void (IrDACB *funcp)( void )) ;
													
void	IrDAAPI	IrSIR_InitConnectionParameters( void ) ;
void	IrDAAPI	IrSIR_SetSpeed( dword spd ) ;		


IrDABOOL	IrDAAPI	IrSIR_Busy( void ) ;				
IrDABOOL	IrDAAPI	IrSIR_InFrame( void ) ;				
													
void	IrDAAPI	IrSIR_SetFrameBuffer( IrSIRFRAMECONFIG IrDAFAR *frameconfig ) ;
void	IrDAAPI	IrSIR_SetFrameMode( void ) ;		
void	IrDAAPI	IrSIR_SetByteMode( void ) ;			
void	IrDAAPI	IrSIR_SetRecvMode( void ) ;			
void	IrDAAPI	IrSIR_ResetRecvMode( void ) ;		
													
void	IrDAAPI	IrSIR_SetConnectionAddress( uint ca ) ;


													
void	IrDAAPI	IrSIR_SetMinimamTurnaroundTime( uint msec ) ;
void	IrDAAPI	IrSIR_SetBOF( uint bofsz ) ;		
													
void	IrDAAPI	IrSIR_SendFrame( uint datasz, void IrDAFAR *data ) ;


ulong	IrDAAPI	IrSIR_ReadTimer( void ) ;


#if defined(__cplusplus)
}
#endif	


#endif	