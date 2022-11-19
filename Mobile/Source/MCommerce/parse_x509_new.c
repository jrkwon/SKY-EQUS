/* ASN.1 object dumping code, copyright Peter Gutmann
   <pgut001@cs.auckland.ac.nz>, based on ASN.1 dump program by David Kemp
   <dpkemp@missi.ncsc.mil>, with contributions from various people including
   Matthew Hamrick <hamrick@rsa.com>, Bruno Couillard
   <bcouillard@chrysalis-its.com>, Hallvard Furuseth
   <h.b.furuseth@usit.uio.no>, Geoff Thorpe <geoff@raas.co.nz>, David Boyce
   <d.boyce@isode.com>, and several other people whose names I've misplaced.
   Available from http://www.cs.auckland.ac.nz/~pgut001/dumpasn1.c.
   Last updated 22 February 2000.

   This version of dumpasn1 requires a config file dumpasn1.cfg to be present
   in the same location as the program itself or in a standard directory
   where binaries live (it will run without it but will display a warning
   message, you can configure the path either by hardcoding it in or using an
   environment variable as explained further down).  The config file is
   available from http://www.cs.auckland.ac.nz/~pgut001/dumpasn1.cfg.

   This code assumes that the input data is binary, having come from a MIME-
   aware mailer or been piped through a decoding utility if the original
   format used base64 encoding.  Bruno Couillard has created a modified
   version which will read raw base64-encoded data (ie without any MIME
   encapsulation or other headers) directly, at the expense of being somewhat
   non-portable.  Alternatively, you can use utilities like uudeview (which
   will strip virtually any kind of encoding, MIME, PEM, PGP, whatever) to
   recover the binary original.

   You can use this code in whatever way you want, as long as you don't try
   to claim you wrote it.

   Editing notes: Tabs to 4, phasers to stun

   Edited by Byung-Jun Yoon
   2000. 8.
 
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cert.h"
#include "util.h"

/* Useful defines */

#ifndef TRUE
  #define FALSE	0
  #define TRUE	( !FALSE )
#endif /* TRUE */

/* SunOS 4.x doesn't define seek codes or exit codes or FILENAME_MAX (it does
   define _POSIX_MAX_PATH, but in funny locations and to different values
   depending on which include file you use).  Some OS's also define
   FILENAME_MAX to silly values (eg 14 bytes), so we replace it with a more
   sensible setting if necessary */

#ifndef EXIT_FAILURE
  #define EXIT_FAILURE	1
  #define EXIT_SUCCESS	( !EXIT_FAILURE )
#endif /* No exit() codes defined */
#ifndef FILENAME_MAX
  #define FILENAME_MAX	512
#else
  #if FILENAME_MAX < 128
	#undef FILENAME_MAX
	#define FILENAME_MAX	512
  #endif /* FILENAME_MAX < 128 */
#endif /* FILENAME_MAX */

/* Some OS's don't define the min() macro */

#ifndef min
  #define min(a,b)		( ( a ) < ( b ) ? ( a ) : ( b ) )
#endif /* !min */

/* The level of recursion can get scary for deeply-nested structures so we
   use a larger-than-normal stack under DOS */

#ifdef  __TURBOC__
  extern unsigned _stklen = 16384;
#endif /* __TURBOC__ */

/* When we dump a nested data object encapsulated within a larger object, the
   length is initially set to a magic value which is adjusted to the actual
   length once we start parsing the object */

#define LENGTH_MAGIC	177545L

/* Tag classes */

#define CLASS_MASK		0xC0	/* Bits 8 and 7 */
#define UNIVERSAL		0x00	/* 0 = Universal (defined by ITU X.680) */
#define APPLICATION		0x40	/* 1 = Application */
#define CONTEXT			0x80	/* 2 = Context-specific */
#define PRIVATE			0xC0	/* 3 = Private */

/* Encoding type */

#define FORM_MASK		0x20	/* Bit 6 */
#define PRIMITIVE		0x00	/* 0 = primitive */
#define CONSTRUCTED		0x20	/* 1 = constructed */

/* Universal tags */

#define TAG_MASK		0x1F	/* Bits 5 - 1 */
#define EOC				0x00	/*  0: End-of-contents octets */
#define BOOLEAN			0x01	/*  1: Boolean */
#define INTEGER			0x02	/*  2: Integer */
#define BITSTRING		0x03	/*  2: Bit string */
#define OCTETSTRING		0x04	/*  4: Byte string */
#define NULLTAG			0x05	/*  5: NULL */
#define OID				0x06	/*  6: Object Identifier */
#define OBJDESCRIPTOR	0x07	/*  7: Object Descriptor */
#define EXTERNAL		0x08	/*  8: External */
#define REAL			0x09	/*  9: Real */
#define ENUMERATED		0x0A	/* 10: Enumerated */
#define EMBEDDED_PDV	0x0B	/* 11: Embedded Presentation Data Value */
#define UTF8STRING		0x0C	/* 12: UTF8 string */
#define SEQUENCE		0x10	/* 16: Sequence/sequence of */
#define SET				0x11	/* 17: Set/set of */
#define NUMERICSTRING	0x12	/* 18: Numeric string */
#define PRINTABLESTRING	0x13	/* 19: Printable string (ASCII subset) */
#define T61STRING		0x14	/* 20: T61/Teletex string */
#define VIDEOTEXSTRING	0x15	/* 21: Videotex string */
#define IA5STRING		0x16	/* 22: IA5/ASCII string */
#define UTCTIME			0x17	/* 23: UTC time */
#define GENERALIZEDTIME	0x18	/* 24: Generalized time */
#define GRAPHICSTRING	0x19	/* 25: Graphic string */
#define VISIBLESTRING	0x1A	/* 26: Visible string (ASCII subset) */
#define GENERALSTRING	0x1B	/* 27: General string */
#define UNIVERSALSTRING	0x1C	/* 28: Universal string */
#define BMPSTRING		0x1E	/* 30: Basic Multilingual Plane/Unicode string */

/* Length encoding */

#define LEN_XTND  0x80		/* Indefinite or long form */
#define LEN_MASK  0x7F		/* Bits 7 - 1 */

/* Various checks to perform on strings */

typedef enum {
	STR_NONE,				/* No special checking */
	STR_UTCTIME,			/* Check it's UTCTime */
	STR_PRINTABLE,			/* Check it's a PrintableString */
	STR_IA5,				/* Check it's an IA5String */
	} STR_CHECK;

/* Structure to hold info on an ASN.1 item */

typedef struct {
	int id;						/* Identifier */
	int tag;					/* Tag */
	long length;				/* Data length */
	int indefinite;				/* Item has indefinite length */
	int headerSize;				/* Size of tag+length */
	int header[ 8 ];			/* Tag+length data */
	} ASN1_ITEM;

enum { 	SEQ_START			= 0,
		SEQ_VERSION			= 1,
		SEQ_SERIAL			= 2,
		SEQ_SIG_ALGO		= 3,
		SEQ_ISSUER			= 4,
		SEQ_NOTBEFORE		= 5,
		SEQ_NOTAFTER		= 6,
		SEQ_SUBJECT			= 7,
		SEQ_PUBKEY_ALGO		= 8,
		SEQ_PUBKEY			= 9,
		SEQ_SIGNATURE		=10,
		SEQ_END				=11 
	};

const char * DN_OID[][2] =
	{	{"2 5 4 6", "C"},		// countryName
		{"2 5 4 7", "L"},		// localityName
		{"2 5 4 8", "SP"},		// stateOfProvinceName
		{"2 5 4 10", "O"},		// organizationName
		{"2 5 4 11", "OU"},		// organizationalUnitName
		{"2 5 4 3", "CN"},		// commonName
		{"1 2 840 113549 1 9 1", "Email"},		// emilAddress
		{NULL, NULL}			// END of LIST
	};
	

static long	nCount = 0;
static long	nDataLen = 0;
static int	nSeq   = 0;
//swjung

//static unsigned char serialNum[32];

/* Config options */

/*
// Try to print BIT STRING's as constr.
static int tryBSConstructed = FALSE;
// Try to print OCTET STRING's as constr.
static int tryOSConstructed = FALSE;
*/

static int zeroLengthAllowed = FALSE;/* Zero-length items allowed */

// define max OID Size

#define MAX_OID_SIZE	32


//---------------------------
//	
//	Object Identification/
//	Description Routine
//
//---------------------------

/* Return descriptive strings for universal tags */

#if 0

char *idstr( const int tagID )
	{
	switch( tagID )
		{
		case EOC:
			return( "End-of-contents octets" );
		case BOOLEAN:
			return( "BOOLEAN" );
		case INTEGER:
			return( "INTEGER" );
		case BITSTRING:
			return( "BIT STRING" );
		case OCTETSTRING:
			return( "OCTET STRING" );
		case NULLTAG:
			return( "NULL" );
		case OID:
			return( "OBJECT IDENTIFIER" );
		case OBJDESCRIPTOR:
			return( "ObjectDescriptor" );
		case EXTERNAL:
			return( "EXTERNAL" );
		case REAL:
			return( "REAL" );
		case ENUMERATED:
			return( "ENUMERATED" );
		case EMBEDDED_PDV:
			return( "EMBEDDED PDV" );
		case UTF8STRING:
			return( "UTF8String" );
		case SEQUENCE:
			return( "SEQUENCE" );
		case SET:
			return( "SET" );
		case NUMERICSTRING:
			return( "NumericString" );
		case PRINTABLESTRING:
			return( "PrintableString" );
		case T61STRING:
			return( "TeletexString" );
		case VIDEOTEXSTRING:
			return( "VideotexString" );
		case IA5STRING:
			return( "IA5String" );
		case UTCTIME:
			return( "UTCTime" );
		case GENERALIZEDTIME:
			return( "GeneralizedTime" );
		case GRAPHICSTRING:
			return( "GraphicString" );
		case VISIBLESTRING:
			return( "VisibleString" );
		case GENERALSTRING:
			return( "GeneralString" );
		case UNIVERSALSTRING:
			return( "UniversalString" );
		case BMPSTRING:
			return( "BMPString" );
		default:
			return( "Unknown (Reserved)" );
		}
	}

#endif

//--------------------------
//
//	get values
//
//--------------------------

static void getHexValue( unsigned char * inBuf, long length, unsigned char * outBuf)
{
	memcpy(outBuf, &inBuf[nCount], length);
	nCount += length;
}


static void getBitString( unsigned char * inBuf, const int length, const int unused, unsigned char * outBuf)
{
	int noBits = ( length * 8 ) -unused;

	if( length )
		outBuf[0] = inBuf[nCount++];

	if( noBits > 8 )
		outBuf[1] = inBuf[nCount++];
}


static void getString( unsigned char * inBuf, long length, char * outBuf)
{
	memcpy(outBuf, &inBuf[nCount], length);
	outBuf[length] = '\0';
	nCount += length;
}



//-------------------------
//
//	ASN.1 Parsing Routines
//
//-------------------------

/* Get an integer value */

static long getValue( unsigned char * inBuf, const long length )
	{
	long value;
	char ch;
	int i;

	ch = inBuf[nCount++];
	value = ch;
	for( i = 0; i < length - 1; i++ )
		value = ( value << 8 ) | inBuf[nCount++];
	return( value );
	}

/* Get an ASN.1 objects tag and length */

int getItem( unsigned char *inBuf, ASN1_ITEM *item )
	{
	int tag, length, index = 0;

	memset( item, 0, sizeof( ASN1_ITEM ) );
	item->indefinite = FALSE;
	tag = item->header[ index++ ] = inBuf[nCount++];
	item->id = tag & ~TAG_MASK;
	tag &= TAG_MASK;
	if( tag == TAG_MASK )
		{
		int value;

		/* Long tag encoded as sequence of 7-bit values.  This doesn't try to
		   handle tags > INT_MAX, it'd be pretty peculiar ASN.1 if it had to
		   use tags this large */
		tag = 0;
		do
			{
			value = inBuf[nCount++];
			tag = ( tag << 7 ) | ( value & 0x7F );
			item->header[ index++ ] = value;
			}
		while( value & LEN_XTND && nCount<nDataLen);
		}
	item->tag = tag;

	if( nCount>=nDataLen )
		return( FALSE );
	
	length = item->header[ index++ ] = inBuf[nCount++];
	item->headerSize = index;
	if( length & LEN_XTND )
		{
		int i;

		length &= LEN_MASK;
		if( length > 4 )
			{
				return( FALSE );
			}
		item->headerSize += length;
		item->length = 0;
		if( !length )
			item->indefinite = TRUE;
		for( i = 0; i < length; i++ )
			{
			int ch = inBuf[nCount++];

			item->length = ( item->length << 8 ) | ch;
			item->header[ i + index ] = ch;
			}
		}
	else
		item->length = length;

	return( TRUE );
	}

#if 0

static int checkEncapsulate( unsigned char *inBuf, const int tag )
	{
	int level = ( tag == BITSTRING ) ? tryBSConstructed : tryOSConstructed;
	int ch;


	/* If we're not looking for encapsulated objects, return */
	if( !level )
		return( FALSE );

	/* Get the first character and see if it's an INTEGER or SEQUENCE */

	ch = inBuf[nCount];
	if( ch == INTEGER || ch == ( SEQUENCE | CONSTRUCTED ) )
		return( TRUE );

	/* All sorts of weird things get bundled up in octet strings in
	   certificate extensions */
	if( tag == OCTETSTRING && ch == BITSTRING )
		return( TRUE );

	/* If we're looking for all sorts of things which might be encapsulated,
	   check for these as well.  At the moment we only check for a small
	   number of possibilities, this list will probably change as more
	   oddities are discovered, the idea is to keep the amount of burrowing
	   we do to a minimum in order to reduce problems with false positives */
	if( level > 1 && tag == OCTETSTRING )
		{
		int length;

		if( ch == IA5STRING )
			/* Verisign extensions */
			return( TRUE );

		/* For the following possibilities we have to look ahead a bit
		   further and check the length as well */

		length = inBuf[nCount+1];

		if( ( ch == OID && length < 9 ) || \
			( ch == ENUMERATED && length == 1 ) || \
			( ch == GENERALIZEDTIME && length == 15 ) )
			/* CRL per-entry extensions */
			return( TRUE );
		}

	return( FALSE );
	}

#endif

/* Check whether a zero-length item is OK */

int zeroLengthOK( const ASN1_ITEM *item )
	{
	/* If we can't recognise the type from the tag, reject it */
	if( ( item->id & CLASS_MASK ) != UNIVERSAL )
		return( FALSE );

	/* The following types are zero-length by definition */
	if( item->tag == EOC || item->tag == NULLTAG )
		return( TRUE );

	/* A real with a value of zero has zero length */
	if( item->tag == REAL )
		return( TRUE );

	/* Everything after this point requires input from the user to say that
	   zero-length data is OK (usually it's not, so we flag it as a
	   problem) */
	if( !zeroLengthAllowed )
		return( FALSE );

	/* String types can have zero length except for the Unrestricted
	   Character String type ([UNIVERSAL 29]) which has to have at least one
	   octet for the CH-A/CH-B index */
	if( item->tag == OCTETSTRING || item->tag == NUMERICSTRING || \
		item->tag == PRINTABLESTRING || item->tag == T61STRING || \
		item->tag == VIDEOTEXSTRING || item->tag == VISIBLESTRING || \
		item->tag == IA5STRING || item->tag == GRAPHICSTRING || \
		item->tag == GENERALSTRING || item->tag == UNIVERSALSTRING || \
		item->tag == BMPSTRING || item->tag == UTF8STRING || \
		item->tag == OBJDESCRIPTOR )
		return( TRUE );

	/* SEQUENCE and SET can be zero if there are absent optional/default
	   components */
	if( item->tag == SEQUENCE || item->tag == SET )
		return( TRUE );

	return( FALSE );
	}


/* Parse a constructed ASN.1 object */

static int parseAsn1( unsigned char * inBuf, const int level, long length, const int isIndefinite , X509 * x509);

static int parseConstructed( unsigned char *inBuf, int level, const ASN1_ITEM *item , X509 * x509)
	{
	int result;

	result = parseAsn1( inBuf, level + 1, item->length, item->indefinite, x509 );
	if( result )
		{
			return( 1 );
		}
	return ( 0 );

	}

/* Print a single ASN.1 object */

static int parseASN1object( unsigned char *inBuf, ASN1_ITEM *item, int level, X509 * x509 )
	{
	char buffer[ MAX_OID_SIZE ];
	char tmpBuf[ 512 ];
	long value;
	int x, y;


	if( ( item->id & CLASS_MASK ) != UNIVERSAL )
		{
//swjung
#ifdef BLOCK
		static const char *const classtext[] =
			{ "UNIVERSAL ", "APPLICATION ", "", "PRIVATE " };
#endif //#ifdef BLOCK

		/* Print the object type */
//		fprintf( output, "[%s%d]",
//				 classtext[ ( item->id & CLASS_MASK ) >> 6 ], item->tag );

		/* Perform a sanity check */
		if( ( item->tag != NULLTAG ) && ( item->length < 0 ) )
			{
				return 1;	
			}

		if( !item->length && !item->indefinite )
			{
				return 1;
			}

		/* If it's constructed, print the various fields in it */

		if( ( item->id & FORM_MASK ) == CONSTRUCTED )
			{
			return parseConstructed( inBuf, level, item, x509 );
			}

		getHexValue( inBuf, item->length, (unsigned char* )tmpBuf);

		return 0;
		}

	if( nSeq == SEQ_START && item->tag==SEQUENCE && level==1 ){
		x509->certOffset	= nCount - item->headerSize;
		x509->certSize		= item->headerSize + item->length;
	//	printf(">>CERT offset:%ld size:%ld\n\n", x509->certOffset, x509->certSize);
	}

	/* Perform a sanity check */
	if( ( item->tag != NULLTAG ) && ( item->length < 0 ) )
		{
			return 1;
		}

	/* If it's constructed, print the various fields in it */
	if( ( item->id & FORM_MASK ) == CONSTRUCTED )
		{
		return parseConstructed( inBuf, level, item, x509 );
		}

	/* It's primitive */
	if( !item->length && !zeroLengthOK( item ) )
		{
			return 1;
		}

	switch( item->tag )
		{
		case BOOLEAN:
			x = inBuf[nCount++];
			if( x != 0 && x != 0xFF ){
				return 1;
			}
			break;

		case INTEGER:
		case ENUMERATED:
			if( item->length > 4 ){
				getHexValue( inBuf, item->length, (unsigned char *) tmpBuf);
				if(nSeq == SEQ_START){
					x509->version = 0;
					x509->serialLen = item->length;
					memcpy(x509->serial.str, tmpBuf, min(item->length,X509_MAX_SERIAL_LEN));
					nSeq+=2;
				}
				if(nSeq == SEQ_VERSION){
					x509->serialLen = item->length;
					memcpy(x509->serial.str, tmpBuf, min(item->length,X509_MAX_SERIAL_LEN));
					nSeq++;
				}
			}
			else
				{
				value = getValue( inBuf, item->length );
				if(nSeq == SEQ_START){
					x509->version = value;
					// this may be a serial number, so set the
					// serialLen to the item length
					// if encounter the serial number later, 
					// this value will be updated with the actual
					// serial number length
					x509->serialLen = item->length;
					nSeq++;
				}
				else if(nSeq == SEQ_VERSION){
					x509->serialLen = item->length;
					x509->serial.num = value;
					nSeq++;
				}

				if( value < 0 )
					{
						return 1;
					}
				}
			break;

		case BITSTRING:
			x = inBuf[nCount++];
			item->length -= 1;
			if( item->length <= 2 )
				{
				/* It's short enough to be a bit flag, dump it as a sequence
				   of bits */
				getBitString( inBuf, (int) item->length, x, (unsigned char *)tmpBuf);
				break;
				}
		case OCTETSTRING:

#if 0
			if( checkEncapsulate( inBuf, item->tag ) )
				{
				/* It's something encapsulated inside the string, print it as
				   a constructed item */
				parseConstructed( inBuf, level + 1, item , x509);
				break;
				}
#endif
			getHexValue( inBuf, item->length, (unsigned char *) tmpBuf);

			if(nSeq == SEQ_PUBKEY_ALGO){
				memcpy( x509->pubKey, tmpBuf, item->length);
				x509->pubKeyLen = item->length;
				nSeq++;
			}
			else if(nSeq == SEQ_PUBKEY && level==1){
				memcpy( x509->signature, tmpBuf, item->length);
				x509->sigLen = item->length;
				nSeq++;
			}

			break;

		case OID:
			/* Hierarchical Object Identifier: The first two levels are
			   encoded into one byte, since the root level has only 3 nodes
			   (40*x + y).  However if x = joint-iso-itu-t(2) then y may be
			   > 39, so we have to add special-case handling for this */
			if( item->length > MAX_OID_SIZE )
				{
					return 1;
				}
			memcpy(buffer, &inBuf[nCount], item->length);
			nCount += item->length;

			/* Pick apart the OID */
			x = ( unsigned char ) buffer[ 0 ] / 40;
			y = ( unsigned char ) buffer[ 0 ] % 40;
			if( x > 2 )
				{
				/* Handle special case for large y if x = 2 */
				y += ( x - 2 ) * 40;
				x = 2;
				}

			sprintf( tmpBuf, "%d %d", x, y);

			value = 0;
			for( x = 1; x < item->length; x++ )
				{
				value = ( value << 7 ) | ( buffer[ x ] & 0x7F );
				if( !( buffer[ x ] & 0x80 ) )
					{
					sprintf( &tmpBuf[strlen(tmpBuf)], " %ld", value );

					value = 0;
					}
				}

			if(nSeq == SEQ_VERSION){
				x509->serial.num = x509->version;
				x509->version = 0;
				nSeq++;
			}
			if(nSeq == SEQ_SERIAL){
				strcpy(x509->sig_algo, tmpBuf);
				nSeq++;
			} 
			else if(nSeq == SEQ_SIG_ALGO && level==5){
				int i=0;
				const char * p;
				for( ; (p=DN_OID[i][1] )!= NULL; ++i){
					if(strcmp(DN_OID[i][0],tmpBuf)==0)
						break;
				}
				
				if(DN_OID[i][1]==NULL){
					p = "?";
				}
				strcat(x509->issuer, "/");
				strcat(x509->issuer, p);
				strcat(x509->issuer, "=");
			}
			else if(nSeq == SEQ_NOTAFTER && level==5){
				int i=0;
				const char * p;
				for( ; (p=DN_OID[i][1]) != NULL; ++i){
					if(strcmp(DN_OID[i][0],tmpBuf)==0)
						break;
				}
				
				if(DN_OID[i][1]==NULL){
					p = "?";
				}
				strcat(x509->subject, "/");
				strcat(x509->subject, p);
				strcat(x509->subject, "=");
			}
			else if(nSeq == SEQ_NOTAFTER && level==4){
				strcpy(x509->pubKeyAlgo, tmpBuf);
				nSeq += 2;
			}

#if 0
			else if(nSeq == SEQ_PUBKEY && level==2){
				//signature algorithm OID
			}
#endif

			break;

		case EOC:
		case NULLTAG:
			break;

		case OBJDESCRIPTOR:
		case GENERALIZEDTIME:
		case GRAPHICSTRING:
		case VISIBLESTRING:
		case GENERALSTRING:
		case UNIVERSALSTRING:
		case NUMERICSTRING:
		case T61STRING:
		case VIDEOTEXSTRING:
		case BMPSTRING:
		case UTF8STRING:
		case PRINTABLESTRING:
		case IA5STRING:
			getString( inBuf, item->length, tmpBuf);
			if(nSeq == SEQ_SIG_ALGO){
				strcat(x509->issuer, tmpBuf);
			}
			else if(nSeq == SEQ_NOTAFTER){
				strcat(x509->subject, tmpBuf);
			}
			break;
		case UTCTIME:
			getString( inBuf, item->length, tmpBuf);
			if(nSeq == SEQ_SIG_ALGO){
				strcpy(x509->notBefore, tmpBuf);
				nSeq += 2;
			}
			else if(nSeq == SEQ_NOTBEFORE){
				strcpy(x509->notAfter, tmpBuf);
				nSeq++;
			}
			break;

		default:
			getHexValue( inBuf, item->length, (unsigned char *) tmpBuf);
			return 1;
		}

		return 0;
	}

/* Print a complex ASN.1 object */

static int parseAsn1( unsigned char *inBuf, const int level, long length,
			   const int isIndefinite, X509 * x509 )
	{
	ASN1_ITEM item;
	long lastPos = nCount;
	int seenEOC = FALSE;

	/* Special-case for zero-length objects */
	if( !length && !isIndefinite )
		return( 0 );

	while( getItem( inBuf, &item ) )
		{
		/* If the length isn't known and the item has a definite length, set
		   the length to the items length */
		if( length == LENGTH_MAGIC && !item.indefinite )
			length = item.headerSize + item.length;

		/* Print details on the item */
		if( !seenEOC )
			{
				if(parseASN1object( inBuf, &item, level, x509 )){
					return 1;
				}
			}

		if( level == 1 )
			length = length - 0;

		/* If it was an indefinite-length object (no length was ever set) and
		   we've come back to the top level, exit */
		if( length == LENGTH_MAGIC )
			return( 0 );

		length -= nCount - lastPos;
		lastPos = nCount;
		if( isIndefinite )
			{
			if( seenEOC )
				return( 0 );
			}
		else
			if( length <= 0 )
				{
				if( length < 0 )
					return( ( int ) -length );
				return( 0 );
				}
			else
				if( length == 1 )
					{
					const int ch = inBuf[nCount++];

					/* No object can be one byte long, try and recover.  This
					   only works sometimes because it can be caused by
					   spurious data in an OCTET STRING hole or an incorrect
					   length encoding.  The following workaround tries to
					   recover from spurious data by skipping the byte if
					   it's zero or a non-basic-ASN.1 tag, but keeping it if
					   it could be valid ASN.1 */
					if( ch && ch <= 0x31 )
						nCount--;
					else
						return( 1 );
					}
		}

	/* If we see an EOF and there's supposed to be more data present,
	   complain */
	if( length && length != LENGTH_MAGIC )
		{
			return( 1 );
		}
	return( 0 );
	}

//
// Initialize some variables that are needed during the procedure
//

void initParseProc(long length)
{
	nCount	= 0;
	nSeq	= 0;
	nDataLen = length;
}

//
// parse DER encoded ASN.1 Object to a x509 certificate structure
//

int parseDER2X509(unsigned char * der, long length, X509 * x509)
{
	int nRet;
	initParseProc( length );
	memset( x509, 0, sizeof(X509) );
	nRet = parseAsn1( der, 0, LENGTH_MAGIC, 0, x509);

	// check if the object is correctly parsed
	// and if all the certificate information is extracted
	
	if( nRet!=0 || nSeq != SEQ_SIGNATURE ){
		return 1;
	}
	else{
		return 0;
	}
}


