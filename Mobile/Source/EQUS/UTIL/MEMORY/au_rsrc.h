/*
 * Copyright (C) 1999, AU-System Radio AB. All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System Radio AB, and may be used and copied
 * only in accordance with the terms of this agreement.
 *
 * AU-System Radio AB assumes no responsibility
 * or liability for any errors or inaccuracies in this software,
 * or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 *
 */
#ifndef _AU_RSRC_H
#define _AU_RSRC_H



#include "au_cmmn.h" // <-- #include "tapicmmn.h" // girin. 2000.12.20
#ifdef BLOCK
#include "tapimmi.h"
#include "errcodes.h"
#include "confvars.h"
#include "ansilibs.h"

/* Configuration of GWC */
#define CONFIG_CO

/************************/
/* WAE constants        */
/************************/

#define CACHE_MODE_ALWAYS_CHECK     0
#define CACHE_MODE_FIRSTTIME_CHECK  1
#define CACHE_MODE_NEVER_CHECK      2

/* User Agent Mode */
#define User_Agent_WML  (UINT8) 1
#define User_Agent_WTA  (UINT8) 2

/* IANA Codes */
#define IANA_CHARSET_USASCII (INT16)3
#define IANA_CHARSET_LATIN1  (INT16)4
#define IANA_CHARSET_UTF8    (INT16)106
#define IANA_CHARSET_UCS2    (INT16)1000
#define IANA_CHARSET_KSC5601 (INT16)36


#define IANA_CHARSET_INVALID (INT16)-1

#define TIMEOUT_VALUE_INVALID (UINT16)39567

/* accept modes */
#define ACCEPT_ALL "*/*"
#define ACCEPT_DEFAULT "application/vnd.wap.wmlc,application/vnd.wap.wbxml,application/vnd.wap.wmlscriptc,*/*"
#define ACCEPT_SCRIPT "application/vnd.wap.wmlscriptc"


#endif //#ifdef BLOCK
/***********************/
/* Basic type info     */
/***********************/

#define INT8_MIN            (-128)
#define INT8_MAX            127
#define UINT8_MAX           0xFF
#define INT16_MIN           (-32768)
#define INT16_MAX           32767
#define UINT16_MAX          0xFFFF
#define INT32_MIN           (-2147483647L-1)
#define INT32_MAX           2147483647L
#define UINT32_MAX          0xFFFFFFFFUL

#ifdef HAS_FLOAT
    #define FLOAT32_MAX              3.402823466e+38F
    #define FLOAT32_MIN              1.17549435e-38F
    #define FLOAT32_INTMAX_AS_FLOAT  2.14748352e10F
    	/* FLOAT32_INTMAX_AS_FLOAT is the largest float that is not larger than INT32_MAX */
#else
    #define FLOAT32_MAX              INT32_MAX
    #define FLOAT32_MIN              INT32_MIN
    #define FLOAT32_INTMAX_AS_FLOAT  2147483647L
#endif
#ifdef BLOCK

/**********************************************************
	ABOUT THIS SYSTEM
**********************************************************/

extern const char * const aboutStrings[3];


/*
Is called when the Generic WAP Client is done with its cleanup
*/
VOID   TerminationDone(VOID);


/***********************/
/* Memory stuff        */
/***********************/

void deleteAllMemory(void);


#ifndef NO_GLOBAL_VARS
extern BOOL executingSDL;
#endif


#ifdef LOG_INTERNAL

	#define OSConnectorAlloc(size) WIPAlloc(size, __FILE__, __LINE__)
	#define OSConnectorFree(block) WIPFree(block, __FILE__, __LINE__)

	/* Allocates a struct */
	#define NEWSTRUCT(type) (p##type)WIPAlloc(sizeof(type), __FILE__, __LINE__)

	/* Allocates an array of elements */
	#define NEWARRAY(type,num) (type*)WIPAlloc((num)*sizeof(type), __FILE__, __LINE__)

	/* Deletes a block of memory. TAKES A POINTER TO THE POINTER! */
	#define DEALLOC(pp) if(*(pp)!=NULL){WIPFree((VOID*)*(pp), __FILE__, __LINE__);*(pp)=NULL;}

	VOID* WIPAlloc( UINT32 size, char *file, int lineNo );
	VOID WIPFree( VOID* block, char *file, int lineNo );

    /* TRUE means that the memory allocator never returns
       if out of memory occurs. FALSE means that the memory
       allocator returns NULL if out of memory occurs.
       It returns the old value. */
    BOOL SetMemoryGuard(BOOL);

#else

	#define OSConnectorAlloc(size) WIPAlloc(size)
	#define OSConnectorFree(block) WIPFree(block)

	/* Allocates a struct */
	#define NEWSTRUCT(type) (p##type)WIPAlloc(sizeof(type))

	/* Allocates an array of elements */
	#define NEWARRAY(type,num) (type*)WIPAlloc((num)*sizeof(type))

	/* Deletes a block of memory. TAKES A POINTER TO THE POINTER! */
	#define DEALLOC(pp) if(*(pp)!=NULL){WIPFree((VOID*)*(pp));*(pp)=NULL;}

	VOID* WIPAlloc( UINT32 size );
	VOID WIPFree( VOID* block );

    /* TRUE means that the memory allocator never returns
       if out of memory occurs. FALSE means that the memory
       allocator returns NULL if out of memory occurs.
       It returns the old value. */
	BOOL SetMemoryGuard(BOOL);

#endif


/***********************/
/* Log functionality   */
/***********************/

#ifdef LOG_INTERNAL
    #ifndef LOG_EXTERNAL
        #define LOG_EXTERNAL
    #endif
#endif

#if defined LOG_EXTERNAL

char *  OctetString2HexString(const CHAR *octetData, UINT32 octetDataLen);

#endif


/***********************/
/* Misc utility macros */
/***********************/

/* WCHAR */

BOOL CompareWideStringNoCase( WCHAR *a, WCHAR *b, UINT32 count );
    /* a-z and A-Z are not compared case sensitive */

INT16 w_strlen(const WCHAR* str); /* wcslen */
WCHAR* w_strcpy(WCHAR* dst, const WCHAR* src); /* wcscpy */
WCHAR* w_strcat(WCHAR* dst, const WCHAR* src); /* wcscat */
INT8 w_strcmp(const WCHAR* str1, const WCHAR* str2); /* wcscmp */
WCHAR* w_cstr2wstr(const CHAR* src); /* mbstowcs */

/* BYTE */

/* Copy the string src to dst, while changing all uppercase letters
   to lower case. */
void DowncaseString (BYTE *dst, BYTE *src);

/* Return the lower case version of a byte character. */
BYTE DowncaseByte (BYTE a);

/* Compare two strings without regard to case, using at most n bytes.
 * Return 1, 0, or -1, according to if string a comes before,
 * is equal to, or comes after b in lexicographic ordering. */
INT32 ByteStringNCmpNoCase (BYTE *a, BYTE *b, UINT32 n);


BYTE *n_strdup (const BYTE *str);

#define ABS(a) ( ((a)<0) ? ((-(a))):((a)) )
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

/* WCHAR macros */

#define COPYSTRINGN(dst,src,count) memcpy(dst,src,(count)*sizeof(WCHAR))
#define w_strncpy(dst, src, n) memcpy(dst, src, (n)*sizeof(WCHAR))
#define COMPARESTRINGN(a,b,s) memcmp(a,b,(s)*sizeof(WCHAR))
#define CONCATSTRING(a,b) w_strcat(a,b)
#define COPYSTRING(a,b) w_strcpy(a,b)
#define COMPARESTRING(a,b) w_strcmp(a,b)
#define STRINGLENGTH(a) w_strlen(a)

/* BYTE macros */

#define B_COPYSTRINGN(dst,src,count) memcpy(dst,src,(count)*sizeof(BYTE))
#define B_COPYSTRING(a,b) strcpy(a,b)
#define B_COMPARESTRING(a,b) strcmp(a,b)
#define B_STRINGLENGTH(a) (UINT16)strlen((CHAR *)a)
#define B_STRSTRING(a,b) strstr(a,b)
#define n_strlen(str) strlen(str)
#define n_strncpy(dst, src, n) strncpy(dst, src, n)
#define n_strchr(str, chr) strchr(str, chr)
#define B_STRDUP(a) n_strdup(a)

/*********************/
/* Uintvar functions */
/*********************/

/* Return the number of bytes required to store n as a Uintvar. */
UINT32 UintvarLen (UINT32 n);

/* Convert n to a Uintvar, and store at the location indicated by v.
 * Return the number of bytes used by the Uintvar. */
UINT32 IntToUintvar (UINT32 n, BYTE *v);

/* Convert a Uintvar, stored as consecutive bytes in s, to an int.
 * Store the result in *pn.
 * maxLen is the max number of bytes we are allowed to look at;
 * if the Uintvar does not stop before this, it is an error.
 * Return the number of bytes used by the Uintvar. On error, return 0; */
UINT32 UintvarToInt (BYTE *s, UINT32 *pn, UINT32 maxLen);


/****************************/
/* Memory read functions    */
/****************************/

BOOL StepBytes( UINT32 cb, BYTE* BP, UINT32 BPend, UINT32 *pos );

BOOL ReadBytes( BYTE* pvResult, INT32 cResultMemSize, INT32 cBytesToRead, BYTE* BP, UINT32 BPend, UINT32 *pos );

BOOL ReadByte( BYTE* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadInt8( INT8* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadUInt8( UINT8* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadInt16( INT16* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadUInt16( UINT16* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadInt32( INT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadUInt32( UINT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadFloat32( FLOAT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );

BOOL ReadMBUInt16( UINT16* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );
BOOL ReadMBUInt32( UINT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos );

UINT8 Read5bFrom8b( BYTE b );
UINT8 Read4bFrom8b( BYTE b );
UINT8 Read3bFrom8b( BYTE b );

/****************************/
/* String convert functions */
/****************************/

BYTE*  wip_wchar2byte( const WCHAR *src, BOOL *overflowDetected );
#define wip_byte2wchar(src) w_cstr2wstr((const CHAR*)src)

BOOL SDL_wchar2byte(void** ppwchInText);
void SDL_byte2wchar(void** ppbInText);


/* Changes a byte to the string "XY", where XY are the hexadecimal digits
   that represent the value of the byte, and writes that string to
   the location indicated by 'bs'. */
void ByteToHex (BYTE bByte, BYTE *bs);

/* Changes a hexadecimal representation, stored in the string 'bs',
   to the byte it represents, and stores the value at the location
   indicated by 'res'. Returns FALSE if the string at 'bs' does
   not start with two hexadecimal digits, TRUE otherwise. */
BOOL HexToByte (const BYTE *bs, BYTE *res);

#endif //#ifdef BLOCK
#endif
