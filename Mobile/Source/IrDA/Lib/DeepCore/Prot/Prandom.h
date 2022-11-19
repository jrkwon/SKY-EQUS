


#if !defined(__prandom_h__)
#define __prandom_h__


#include	"okmisc.h"
#include	"irsir.h"


#undef	USE8bit


#if defined(USE8bit)
static	byte		prvrnd8 ;	
#endif	
static	word		prvrnd16 ;	
static	dword		prvrnd32 ;	


void	IrDAAPI	IrDA_SetRandomSeed( dword seed )
{
#if defined(USE8bit)
	prvrnd8		= (byte)( seed & 0xFF ) ;
#endif	
	prvrnd16	= (word)( seed & 0xFFFF ) ;
	prvrnd32	= seed;
}


void		MiscInitRandomize( void )
{
	prvrnd32	= IrSIR_ReadTimer() ;
	prvrnd16	= (word)prvrnd32 ;
#if defined(USE8bit)
	prvrnd8		= (byte)prvrnd32 ;
#endif	
}


byte		MiscRandom8( byte max )
{
#if !defined(USE8bit)
	word	tmpw ;
#endif	
#if defined(USE8bit)
	prvrnd8 = ( prvrnd8 << 2 ) + prvrnd8 + 1 ;
	if( max == 0xFF )
		return( prvrnd8 ) ;
	return( prvrnd8 % ( max + 1 )) ;
#else	
	tmpw    = MiscRandom16( max );
	return( (byte)tmpw ) ;
#endif	
}


word		MiscRandom16( word max )
{
	int		i, j ;
	word	rnd ;


	j	= ( prvrnd16 % 13 ) + 1 ;
	for( i = 0 ; i < j ; i++ )
		prvrnd16	= prvrnd16 * 257 + 1 ;


	rnd		= prvrnd16 + (word)IrSIR_ReadTimer() ;
	if( max == 0xffff )
		return( rnd ) ;


	return( rnd % ( max + 1 )) ;
}


dword		MiscRandom32( dword max )
{
	int		i ;
	dword	tmpdw ;
	word	maxh, maxl ;


	maxh	= (word)( max >> 16 ) ;
	maxl	= (word)( max & 0xFFFFL ) ;
	for( i=0 ; i<100 ; i++ ) {
		tmpdw		= IrDA_LSift((dword)MiscRandom16( maxh ), 16 ) + (dword)MiscRandom16( maxl ) ;
		if( tmpdw <= max )
			return( tmpdw ) ;
	}


	return( max ) ;
}


#endif	
