#include	<stdio.h>
#include	"string.h"
#include	"obexlib.h"
#include	"okmisc.h"

#ifdef	USE_MONITOR

#define	OBX_DEBUG

#endif

#ifdef	OBX_DEBUG
#include	"console.h"

typedef	struct	{
	int			code ;
	const char	*str ;
}	OBEXRESPSTR ;


static	OBEXRESPSTR		obexRespStr[]	= {
	{ 0x00, "RESERVED"						},
	{ 0x01, "RESERVED"						},
	{ 0x02, "RESERVED"						},
	{ 0x03, "RESERVED"						},
	{ 0x04, "RESERVED"						},
	{ 0x05, "RESERVED"						},
	{ 0x06, "RESERVED"						},
	{ 0x07, "RESERVED"						},
	{ 0x08, "RESERVED"						},
	{ 0x09, "RESERVED"						},
	{ 0x0A, "RESERVED"						},
	{ 0x0B, "RESERVED"						},
	{ 0x0C, "RESERVED"						},
	{ 0x0D, "RESERVED"						},
	{ 0x0E, "RESERVED"						},
	{ 0x0F, "RESERVED"						},
	{ 0x10,	"Continue"						},
	{ 0x11, "Switching Protocols"			},
	{ 0x20, "OK, Success"					},
	{ 0x21, "Created"						},
	{ 0x22, "Accepted"						},
	{ 0x23, "Non-Authoritative Information"	},
	{ 0x24, "No Content"					},
	{ 0x25, "Reest Content"					},
	{ 0x26, "Partial Content"				},
	{ 0x30, "Multiple Choices"				},
	{ 0x31, "Moved Permanently"				},
	{ 0x32, "Moved temporarily"				},
	{ 0x33, "See Other"						},
	{ 0x34, "Not modified"					},
	{ 0x35,	"Use Proxy"						},
	{ 0x40,	"Bad Request"					},
	{ 0x41, "Unauthorized"					},
	{ 0x42, "Payment requied"				},
/********************************************/
	{ 0x46, "No Acceptable"					},
	{ 0x47, "Proxy Authentication required"	},
/********************************************/

	{ 0x60, "Internal Server Error"			},
	{ 0x61, "Not Implemented"				},
	{ 0x62, "Bad Gateway"					},
	{ 0x63, "Service Unavailable"			},
	{ 0x64, "Gateway Timeout"				},
	{ 0x65, "HTTP version not supported"	},
	{ -1, 0}} ;

#endif

const char	*obexRespCode( int respcode )
{
#ifdef	OBX_DEBUG
	OBEXRESPSTR		*ors ;
	int				cd ;

	cd		= respcode & 0x7F ;
	for( ors = obexRespStr ; ors->code != -1 ; ors++ ) {
		if( ors->code == cd ){
#ifdef	OBX_DEBUG
			cprintf("RESP:[%s]\n",ors->str);
#endif
			return( ors->str ) ;
		}
	}
#endif
	return( "UNKNOWN" ) ;
}


word		obexGetHeaderSize( const byte IrDAFAR *header )
{
	word	tmpw ;

	switch( header[0] & 0xC0 ) {
	case	0x00 :	/*null terminated Unicode text, length prefixed with 2 byte unsigned interger*/
		GetBigEndian16( &header[1], &tmpw ) ;
		return( tmpw ) ;
	case	0x40 :	/*byte stream, length prefixed with 2 byte unsigned interger*/
		GetBigEndian16( &header[1], &tmpw ) ;
		return( tmpw ) ;
	case	0x80 :	/*1 byte quantity										*/
		return( 1 + 1 ) ;
	case	0xC0 :	/*4 byte quantity - transmitted in network byte order	*/
		return( 1 + 4 ) ;
	}

	/*ここには来るはずが無い...コンパイラとハードのバグを除く*/
	return( 0 ) ;
}

void		dspHeader( const unsigned char *header, int headersz )
{
#ifdef	OBX_DEBUG
	int		i, j ;
	word	sz ;

	if( headersz <= 0 )
		return ;

	for( i=0 ; i<headersz ; ) {
		sz		= obexGetHeaderSize( &header[i] ) ;
		cprintf( "    cmd:0x%02X, size:%d\n", header[i], sz ) ;

		switch( header[i] ) {
		case	0x01 :
			cprintf( "      name=\"" ) ;
			for( j=0 ; j < sz - 3 && header[i+3+j+1] ; j+=2 )
				cprintf( "%c", header[i+3+j+1] ) ;
			cprintf( "\"\n" ) ;
			break ;
		default :
			cprintf( "      %02X", header[i] ) ;
			for( j=1 ; j<sz ; j++ )
				printf( ".%02X", header[i+j] ) ;
			cprintf( "\n" ) ;
		}

		i		+= sz ;
	}
#endif
}

int	obexSetName( char *des, int bufsz, const char *name )
{
int		sz ;
int		nmsz ;

	sz		= 0 ;
	nmsz		= 3 + (( strlen( name ) + 1 ) * 2 );
	des[sz++]	= 0x01 ;
	des[sz++]	= ( nmsz >> 8 ) & 0xFF ;
	des[sz++]	= nmsz & 0xFF ;
	for( ; *name ; name ++ ) {
		des[sz++]	= 0 ;
		des[sz++]	= *name ;
	}
	des[sz++]	= 0 ;
	des[sz++]	= 0 ;
	return( sz ) ;
}

int	obexSetSize( char *des, int bufsz, ulong size )
{
int		sz ;
	sz			= 0 ;
	des[sz++]	= (char)0xC3 ;
	sz			+= MiscSetBigEndian32( &des[sz], size ) ;
	return( sz ) ;
}

IrDABOOL	obexGetNameOption( char *des, int dessz, byte *opt, word optsz )
{
long	size ;
dword	sz ;
word	namesz ;
	*des	= 0 ;
	for( size = (long)optsz ; size > 0 ; ) {
		if( *opt != 0x01 ) {
			sz	= obexGetHeaderSize( opt ) ;
			opt		+= sz ;	size	-= sz ;
			continue ;
		}
		opt			++ ;
		namesz		= ((word)(*opt++)) << 8 ;
		namesz		+= *opt++ ;

		for( ; dessz > 1 && namesz > 0 && ( opt[0] || opt[1] ) ; ) {
			opt		++ ;	*des++	= *opt++ ;
			namesz	-= 2 ;	dessz	-- ;
		}
		*des	= 0 ;
		return( FALSE ) ;
	}
	return( TRUE ) ;
}

IrDABOOL	obexGetSizeOption( dword *pSize, byte *opt, word optsz )
{
long	size ;
dword	sz ;
	for( size = (long)optsz ; size > 0 ; ) {
		if( *opt != 0xC3 ) {
			sz	= obexGetHeaderSize( opt ) ;
			opt		+= sz ;	size	-= sz ;
			continue ;
		}
		opt			++ ;
		MiscGetBigEndian32( opt, pSize ) ;
		return( FALSE ) ;
	}
	return( TRUE ) ;
}

