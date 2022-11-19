#if !defined(__obexlib_h__)
#define		__obexlib_h__

#include	"irda.h"

extern	const char	*obexRespCode( int respcode ) ;
extern	word		obexGetHeaderSize( const unsigned char *header ) ;
extern	void		dspHeader( const byte *header, int headersz ) ;
extern	int			obexSetName( char *des, int bufsz, const char *name );
extern	int			obexSetSize( char *des, int bufsz, ulong size );
extern	IrDABOOL	obexGetNameOption( char *des, int dessz, byte *opt, word optsz );
extern	IrDABOOL	obexGetSizeOption( dword *size, byte *opt, word optsz );
extern	void		obexUniCopy(short *dst,short *src);
extern	int			obexUniLen(short *str);
extern	void 		obexMakeUPutPacket( const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz );
extern	int			obexSendUPutPacket(void);

#endif	/*!__obexlib_h__*/
