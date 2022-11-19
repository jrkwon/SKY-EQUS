/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
#include "wiptrgt.h"

#include "cmmnrsrc.h"
#include "WAEctype.h"
#include "aapiclnt.h"
#include "wipprdct.h"
#include "wipvrsn.h"

#ifdef USE_WIP_MALLOC
#include "wip_mem.h"
#endif

#ifndef CBASIC_WITH_NO_CONNECTORS
#include "wiptimer.h"
#include "msgqueue.h"
#endif

#if defined LOG_INTERNAL
#include <stdio.h>
extern int logCount;
#endif

#if     defined NO_GLOBAL_VARS
#include "userdata.h"
#endif


/**********************************************************
	ABOUT THIS SYSTEM
**********************************************************/

#ifndef WIP_PRODUCT
    #error  WIP_PRODUCT not defined
#endif

#ifndef WIP_VERSION
    #error  WIP_VERSION not defined
#endif

#ifndef WAP_SERIAL
#define WAP_SERIAL "00000000"
#endif

#define ABOUT__PRODUCT_VERSION  WIP_PRODUCT ## "-" ## WIP_VERSION

#define ABOUT__SERIAL           "Serial No: " ## WAP_SERIAL

const char * const aboutStrings[3] =
{
    ABOUT__PRODUCT_VERSION,
    ABOUT__SERIAL,
    NULL
};


/**********************************************************
	SECTION FOR HEADER MANAGEMENT FOR MEMORY ALLOCATOR
**********************************************************/


#if !defined USE_WIP_MALLOC

typedef struct meminfo
{
    void *prev;
    void *next;
    #ifdef USE_MEMORY_GUARD
    UINT32 size;
    #endif
    #ifdef LOG_INTERNAL
    long line;
    long id;
    char file[FILENAME_MAX];
    #endif
} meminfo;


#define PREV(memory) (((meminfo *)memory)->prev)
#define NEXT(memory) (((meminfo *)memory)->next)


/* round up to nearest multiple of MEM_ADDRESS_ALIGNMENT */
#define ROUND2(n) \
        ( ( n + MEM_ADDRESS_ALIGNMENT - 1) & ~(MEM_ADDRESS_ALIGNMENT - 1) )


#define SIZE_OF_HEAD ROUND2(sizeof(meminfo))


#define NO_OF_VOIDSTAR (SIZE_OF_HEAD / sizeof(void*))


#endif


/**********************************************************
	CODE FOR MEMORY ALLOCATOR
**********************************************************/


static UINT8 handleException(void);


#ifdef USE_MEMORY_GUARD
static BOOL memoryGuardCheck(UINT32 size);
static void memoryGuardAdjust(VOID *memory, UINT32 size);
static void memoryGuardReset(void);
#endif


#ifdef LOG_INTERNAL
static void memoryLogNew(VOID *memory, UINT32 size, const char *file, int line);
static void memoryLogDelete(VOID *memory, const char *file, int line);
static void memoryLogPrint(VOID *list);
#endif


#ifndef USE_WIP_MALLOC
static void memoryListNew(VOID *memory);
static void memoryListDelete(VOID *memory);
static void memoryListDeleteAll(void);
#ifdef LOG_INTERNAL
static void memoryListPrint(void);
#endif
#endif


#ifndef HAS_SETJMP
	extern BOOL exception;
#endif

#define RETURN 1
#define RETRY 2


#ifdef LOG_INTERNAL
VOID* WIPAlloc(UINT32 size, const char *file, int line)
#else
VOID* WIPAlloc(UINT32 size)
#endif
{
    VOID *memory = NULL;

	#if !defined USE_WIP_MALLOC
		size = size + SIZE_OF_HEAD;
	#endif

	#ifdef USE_MEMORY_GUARD
		if (memoryGuardCheck(size))
		{
			/* handleException does not return if the macro
			   HAS_SETJMP is set, and the memory
			   allocation is done from SDL when it is
			   unsafe to return NULL. */
			UINT8 res = handleException();
			if (res == RETURN)
				return NULL;
			/* else res == RETRY */
		}
	#endif

#ifndef HAS_SETJMP
retry:
#endif

	#ifdef USE_WIP_MALLOC
		memory = wip_gcmalloc(size);
	#else
		memory = malloc(size);
	#endif

	if (memory == NULL)
	{
        UINT8 res = 0;
		/* handleException does not return if the macro
		   HAS_SETJMP is set, and the memory
		   allocation is done from SDL when it is
		   unsafe to return NULL. */
		res = handleException();
		if (res == RETURN)
		{
#ifdef WAP_USE_LEAK
	/*=============================================================================
			Out of memory가 일어난 경우에는 모든 wip_malloc block을 clear하므로
			만약 wip_free를 행하면 이상한 현상이 일어난다. 
			이를 방지하기 위하여, 다시 Browser을 시작시킬때까지 모든 wip_free행동을
			막을 수 있는 flag를 만든다.
			시작 싯점: out of memory가 나는 경우 
			막는 싯점: Browser start하는 싯점 
	 ==============================================================================*/
			extern BOOL blocknullfree;
			blocknullfree = TRUE;
#endif//#ifdef WAP_USE_LEAK			
			return NULL;
		}
	#ifndef HAS_SETJMP
		else /* res == RETRY */
			goto retry;
	#endif
	}

	if (memory)
	{
		#ifdef USE_MEMORY_GUARD
			memoryGuardAdjust(memory, size);
		#endif

		#if defined LOG_INTERNAL
			memoryLogNew(memory, size, file, line);
		#endif

		#if !defined USE_WIP_MALLOC
			memoryListNew(memory);
		#endif

		#if !defined USE_WIP_MALLOC
			memory = (void **)memory + NO_OF_VOIDSTAR;
		#endif
	}

    return memory;
}


#ifdef LOG_INTERNAL
VOID WIPFree( VOID* memory, const char *file, int line )
#else
VOID WIPFree( VOID* memory )
#endif
{
	void *t;

    if (!memory)
        return;

	#if !defined USE_WIP_MALLOC
	    t = (void **)memory - NO_OF_VOIDSTAR;
	#else
		t = (void **)memory;
	#endif

	#ifdef USE_MEMORY_GUARD
		memoryGuardAdjust(t, 0);
	#endif

	#if defined LOG_INTERNAL
		memoryLogDelete(t, file, line);
	#endif

	#if !defined USE_WIP_MALLOC
		memoryListDelete(t);
	#endif

	#ifdef USE_WIP_MALLOC
		wip_free(t);
	#else
		free(t);
	#endif
}


void deleteAllMemory(void)
{
	#ifdef USE_MEMORY_GUARD
		memoryGuardReset();
	#endif


	#if !defined USE_WIP_MALLOC

		#if defined LOG_INTERNAL
			memoryListPrint();
		#endif

		memoryListDeleteAll();
	#else
		#if defined LOG_INTERNAL
		memoryLogPrint (0);
		#endif
		wip_gc();
	#endif
}


/**********************************************************
	EXCEPTION HANDLER USED BY MEMORY ALLOCATOR
**********************************************************/


#ifndef NO_GLOBAL_VARS
	#ifdef HAS_SETJMP
		#ifdef XSCT_CMICRO
			extern jmp_buf jmpbuf;
		#endif
	#else
			extern char *safetyblock;
	#endif
	BOOL unsafeToReturnNull = TRUE;
	BOOL executingSDL = FALSE;
#endif


static UINT8 handleException(void)
{
#ifdef XSCT_CMICRO
	if (executingSDL && unsafeToReturnNull)
	{
		#ifdef HAS_SETJMP
			CLNTa_error(0, ERR_OUT_OF_MEMORY, ERRTYPE_FATAL);
			longjmp(jmpbuf, 0); /* exception! long jump to CLNTc_start */
		#else
			if (!exception)
			{
				exception = TRUE;
				OSConnectorFree(safetyblock);
				CLNTa_error(0, ERR_OUT_OF_MEMORY, ERRTYPE_FATAL);
				return RETRY;
			}
		#endif
	}
	else if (!executingSDL) /* in connector function */
	{
#ifdef WAP_USE_LEAK
		extern int isReStartComleted;
#endif//#ifdef WAP_USE_LEAK
		deleteAllTimers();
		deleteAllMemory();
		DeleteQueue();
#ifdef WAP_USE_LEAK		
		isReStartComleted = 1;
#endif//#ifdef WAP_USE_LEAK
		CLNTa_error(0, ERR_OUT_OF_MEMORY, ERRTYPE_FATAL);
#ifdef WAP_USE_LEAK
		SendCmdtoUI();
#endif//#ifdef WAP_USE_LEAK
	}
#endif

	return RETURN;
}


/* TRUE means that the memory allocator never returns
   if out of memory occurs (if HAS_SETJMP). If !HAS_SETJMP
   the MBM buffers are freed. The memory allocator tries
   then to allocate one more time and returns after that.
   If we are lucky the functions return safaly to CLNT_run
   were the exception is trigered.
   FALSE means that the memory allocator returns NULL
   if out of memory occurs.
   The function returns the old value. */
BOOL SetMemoryGuard(BOOL trueOrFalse)
{
	BOOL oldValue = unsafeToReturnNull;
	unsafeToReturnNull = trueOrFalse;
	return oldValue;
}


/**********************************************************
	MEMORY COUNTER USED BY MEMORY ALLOCATOR
**********************************************************/


#ifdef USE_MEMORY_GUARD


static long memcount = 0;


static BOOL memoryGuardCheck(UINT32 size)
{
	BOOL overTheLimit = (memcount + size) > MEMORY_LIMIT;

	if (overTheLimit)
		return TRUE;
	else
		return FALSE;
}


static void memoryGuardAdjust(VOID *memory, UINT32 size)
{
#ifdef USE_WIP_MALLOC
	if (size != 0) /* new memory */
	{
		BOOL alreadyWarned = memcount > MEMORY_WARNING;
		BOOL warning;

		memcount += wip_memsize (memory);
		warning = memcount > MEMORY_WARNING;

		#ifndef CBASIC_WITH_NO_CONNECTORS
		if (warning && !alreadyWarned)
		{
			Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
			pMsg->Id = eDiscardAllContext;
			SendMsg(pMsg);
			CLNTa_error(0, ERR_MEMORY_WARNING, ERRTYPE_CRITICAL);
		}
		#endif
	}
	else /* delete memory */
	{
		memcount -= wip_memsize (memory);
	}
#else
	meminfo *t = (meminfo *) memory;

	if (size != 0) /* new memory */
	{
		BOOL alreadyWarned = memcount > MEMORY_WARNING;
		BOOL warning;

		t->size = size;
		memcount += size;
		warning = memcount > MEMORY_WARNING;

		#ifndef CBASIC_WITH_NO_CONNECTORS
		if (warning && !alreadyWarned)
		{
			Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
			pMsg->Id = eDiscardAllContext;
			SendMsg(pMsg);
			CLNTa_error(0, ERR_MEMORY_WARNING, ERRTYPE_CRITICAL);
		}
		#endif
	}
	else /* delete memory */
	{
		memcount -= t->size;
	}
#endif
}

static void memoryGuardReset(void)
{
	memcount = 0;
}


#endif


/**********************************************************
	LOG FACILITY USED BY MEMORY ALLOCATOR
**********************************************************/


#ifdef LOG_INTERNAL


static int memid = 0;


static FILE *OpenMemFile(void)
{
	static FILE   *s_pMEMFILE = NULL;

	if (!s_pMEMFILE)
		s_pMEMFILE = fopen("WAP.mem", "wb");
	else
		s_pMEMFILE = fopen("WAP.mem", "a+b");

	return s_pMEMFILE;
}


static void memoryLogNew(VOID *memory, UINT32 size, const char *file, int line)
{
	FILE    *pFILE = OpenMemFile();
#ifdef USE_WIP_MALLOC
    size = wip_memsize (memory);
    fprintf (pFILE, "%d\t+%d = %d\tLogNo: %d\t%s[%d]\n", memid, size, memcount, logCount, file, line);
#else
	meminfo *t = (meminfo *) memory;
    size_t  fileLen = strlen(file);

    strncpy(t->file, file, fileLen);
	t->file[fileLen] = 0;
	t->line = line;
	t->id = memid;

	fprintf(pFILE, "%d\t+%d = %d\tLogNo: %d\t%s[%d]\n", memid, size, memcount, logCount, file, line);
#endif
	fclose(pFILE);

	memid++;
}


static void memoryLogDelete(VOID *memory, const char *file, int line)
{
	FILE    *pFILE = OpenMemFile();
#ifdef USE_WIP_MALLOC

	fprintf (pFILE, "%d\t-%d = %d\tLogNo: %d\t%s[%d]\n", 0,
           wip_memsize (memory), memcount, logCount, file, line);
#else
	meminfo *t = (meminfo *)memory;

	fprintf(pFILE, "%d\t-%d = %d\tLogNo: %d\t%s[%d]\n", t->id, t->size, memcount, logCount, file, line);
#endif
	fclose(pFILE);
}


static void memoryLogPrint(VOID *list)
{
    const char  cszStart[] = "\n************ START EJ AVALLOKERAT MINNE *************\n";
    const char  cszEnd[] =     "************ SLUT  EJ AVALLOKERAT MINNE *************\n\n";
    FILE    *pFILE = OpenMemFile();
#ifdef USE_WIP_MALLOC
    fprintf(pFILE, cszStart);
    wip_printalloc(pFILE);
#else
    void    *i = list;

    fprintf(pFILE, cszStart);

    while (i != NULL)
    {
        void    *t0 = i;
        meminfo *t = (meminfo *)t0;
        fprintf(pFILE, "id: %d\tsize: %d\t%s[%d]\n", t->id, t->size, t->file, t->line);
        i = NEXT(i);
    }

#endif
    fprintf(pFILE, cszEnd);
    fclose(pFILE);
}


#endif


/**********************************************************
	MEMORY BLOCK LIST USED BY MEMORY ALLOCATOR
**********************************************************/


#if !defined USE_WIP_MALLOC


#ifndef NO_GLOBAL_VARS
	void *memblocks = NULL;
#endif


static void memoryListNew(VOID *memory)
{
	PREV(memory) = NULL;
	if (memblocks)
	{
		PREV(memblocks) = memory;
		NEXT(memory) = memblocks;
	}
	else
	{
		NEXT(memory) = NULL;
	}
	memblocks = memory;
}

static void memoryListDelete(VOID *memory)
{
	if (PREV(memory) && NEXT(memory))
	{
		NEXT(PREV(memory)) = NEXT(memory);
		PREV(NEXT(memory)) = PREV(memory);
	}
	else if (PREV(memory))
	{
		NEXT(PREV(memory)) = NULL;
	}
	else if (NEXT(memory))
	{
		PREV(NEXT(memory)) = NULL;
		memblocks = NEXT(memory);
	}
	else
	{
		memblocks = NULL;
	}
}


static void memoryListDeleteAll(void)
{
	void *i = memblocks;

	while (i != NULL)
	{
		void *t0 = i;
		i = NEXT(i);
		free(t0);
	}

	memblocks = NULL;
}


#if defined LOG_INTERNAL
static void memoryListPrint(void)
{
	memoryLogPrint(memblocks);
}
#endif


#endif


/**********************************************************
	CODE FOR LOG FUNCTIONALITY
**********************************************************/


#if defined LOG_EXTERNAL

static void char2hex(char ch, char *str)
{
    unsigned char   h_nibble = (ch >> 4) & 0x0F;
    unsigned char   l_nibble = ch & 0x0F;

    if (h_nibble < 10)
        str[0] = 48 + h_nibble;
    else
        str[0] = 55 + h_nibble;

    if (l_nibble < 10)
        str[1] = 48 + l_nibble;
    else
        str[1] = 55 + l_nibble;
}

char *OctetString2HexString(const CHAR *octetData, UINT32 octetDataLen)
{
    unsigned int    i;
    char    *str, *istr;

    if (octetData == NULL || octetDataLen == 0)
        return NULL;

    istr = str = (char *) OSConnectorAlloc(sizeof(char) * (octetDataLen * 3));

    for (i = 0; i < octetDataLen; i++)
    {
        char2hex(octetData[i], istr);
        istr += 2;
        *istr++ = ':';
    }

    /*
    The last character is a ':'.
    Replce it with a termninating zero('\0').
    */
    --istr;
    *istr = '\0';

    return str;
}

/*  Mainly used in the connectorfunctions, but may be used anywhere. */
const CHAR  *cszNULL = "(NULL)";
static  const WCHAR s_cwzNULL[] = {'(','N','U','L','L',')','\0'};
const WCHAR *cwzNULL = s_cwzNULL;   /*  The compilers differ => Strange construction! */

#endif


/**********************************************************
	END OF CODE FOR LOG FUNCTIONALITY
**********************************************************/


BOOL CharsEqual( WCHAR a, WCHAR b )
{
    return (BOOL)((((a >= 'A') && (a <= 'Z')) ? (a+32) : (a)) == (((b >= 'A') && (b <= 'Z')) ? (b+32) : (b)));
}


BOOL CompareWideStringNoCase( WCHAR *a, WCHAR *b, UINT32 count )
{
    UINT32  i = 0;

    while (i < count)
    {
        if ( ! CharsEqual( *a, *b ))
        {
            return FALSE;
        }
        a++;
        b++;
        i++;
    }

    return TRUE;
}


/* Array of all 8-bit characters in ISO-Latin-1, lower case only.
   Used by the routine Downcase below. */
const BYTE latin1_lc[256] = {
  0, 1, 2, 3, 4, 5, 6, 7,
  8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23,
  24, 25, 26, 27, 28, 29, 30, 31,
  ' ', '!', '"', '#', '$', '%', '&', '\'',
  '(', ')', '*', '+', ',', '-', '.', '/',
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', ':', ';', '<', '=', '>', '?',
  '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
  'x', 'y', 'z', '[', '\\', ']', '^', '_',
  '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
  'x', 'y', 'z', '{', '|', '}', '~', 127,
  128, 129, 130, 131, 132, 133, 134, 135,
  136, 137, 138, 139, 140, 141, 142, 143,
  144, 145, 146, 147, 148, 149, 150, 151,
  152, 153, 154, 155, 156, 157, 158, 159,
  160, 161, 162, 163, 164, 165, 166, 167,
  168, 169, 170, 171, 172, 173, 174, 175,
  176, 177, 178, 179, 180, 181, 182, 183,
  184, 185, 186, 187, 188, 189, 190, 191,
  224, 225, 226, 227, 228, 229, 230, 231,
  232, 233, 234, 235, 236, 237, 238, 239,
  240, 241, 242, 243, 244, 245, 246, 215,
  248, 249, 250, 251, 252, 253, 254, 223,
  224, 225, 226, 227, 228, 229, 230, 231,
  232, 233, 234, 235, 236, 237, 238, 239,
  240, 241, 242, 243, 244, 245, 246, 247,
  248, 249, 250, 251, 252, 253, 254, 255};

/* Copy the string src to dst, while changing all uppercase letters
   to lower case. */
void
DowncaseString (BYTE *dst, BYTE *src)
{
  while (*src) {
    *dst++ = latin1_lc[*src++];
  }
  *dst = '\0';
}

/* Return the lower case version of character a. */
BYTE
DowncaseByte (BYTE a)
{
  return latin1_lc[a];
}

/* Compare two strings without regard to case, using at most n bytes.
 * Return 1, 0, or -1, according to if string a comes before b,
 * is equal to, or comes after b in lexicographic ordering. */
INT32 ByteStringNCmpNoCase (BYTE *a, BYTE *b, UINT32 n)
{
  while ((n-- > 0) && *a && (latin1_lc[*a] == latin1_lc[*b])) {
    a++;
    b++;
  }
  if (n == 0)
    return 0;
  else if (latin1_lc[*a] < latin1_lc[*b])
    return 1;
  else if (latin1_lc[*a] > latin1_lc[*b])
    return -1;
  else
    return 0;
}


void char2ucsTable(char *b)
{
    const unsigned char n[110] = {22,126,15,138,33,92,20,130,113,40,248,124,124,198,12,166,155,220,249,188,9,10,104,100,249,188,92,146,157,18,11,166,155,220,249,252,17,100,162,10,13,126,79,160,44,166,155,220,250,188,131,218,105,100,250,252,65,100,107,200,156,222,119,218,242,158,64,92,156,222,119,218,242,158,67,166,155,220,251,124,157,2,97,228,43,190,127,156,242,130,116,230,123,190,167,138,92,232,41,40,253,60,186,72,43,140,102,70,69,0};
    int i = 0;
    do {
        unsigned short w;
        int a = 11;
        w = (unsigned short)(n[i++] << 0x08);
        w |= (unsigned short)(n[i++]);
        while (a > 0)
        {
            *b = ((w >> a) & 0x1F) + 0x40;
            *b = (*b - 0x40) ? *b : 0x00;
            *b = (*b - 0x5E) ? *b : 0x2E;
            *b = (*b - 0x5F) ? *b : 0x00;
            b++;
            a -= 0x05;
        }
    } while (i < 110);
    *b++ = 0;
    *b = 0;
}


/*************************************************************************/


INT16 w_strlen(const WCHAR* str) /* wcslen */
{
    INT16    len = -1;

    do
    {
        len++;
    }
    while (*str++);

    return(len);
}


WCHAR* w_strcpy(WCHAR* dst, const WCHAR* src) /* wcscpy */
{
    const   WCHAR* p = src;
            WCHAR* q = dst;

    while (*q++ = *p++)
        ;

    return(dst);
}


WCHAR* w_strcat(WCHAR* dst, const WCHAR* src) /* wcscat */
{
    const   WCHAR* p = src;
            WCHAR* q = dst;

    while (*q++)
        ;

    q--;

    while (*q++ = *p++)
        ;

    return(dst);
}


INT8 w_strcmp(const WCHAR* str1, const WCHAR* str2) /* wcscmp */
{
    const   INT16* p1 = (INT16*)str1;
    const   INT16* p2 = (INT16*)str2;
    INT16   c1, c2;

    while ((c1 = *p1++) == (c2 = *p2++))
    {
        if (!c1)
        {
            return(0);
        }
    }

    return (INT8) (c1 - c2);
}

WCHAR* w_cstr2wstr(const CHAR* src) /* mbstowcs */
{
    if (src != NULL)
    {
        size_t      len = strlen(src) + 1;
        WCHAR       *dst = NEWARRAY(WCHAR, len);
        const CHAR  *p = src;
        WCHAR       *q = dst;

        while (*q++ = (WCHAR)(*p++))
            ;

        return dst;
    }

    return NULL;
}

WCHAR* w_strdup (const WCHAR* str)
{
    WCHAR   *ws = NEWARRAY (WCHAR, STRINGLENGTH (str) + 1);

    if (ws != NULL)
    {
        COPYSTRING (ws, str);
    }

    return ws;
}


/*******************************************************************************/


BYTE *n_strdup (const BYTE *str)
{
    BYTE    *bs = NEWARRAY (BYTE, B_STRINGLENGTH ((char*)str) + 1);

    if (bs != NULL) {
        B_COPYSTRING ((char*)bs, (char*)str);
    }

    return bs;
}


/**** Memory READ functions -- start *******************************************************************/

/* System include statements */

/* Parameters */

#define BIT_MASK_5            (BYTE)0x1F
#define BIT_MASK_4            (BYTE)0x0F
#define BIT_MASK_3            (BYTE)0x07
#define BIT_MASK_16_OF_32   (UINT32)0xFFFF0000

/*-----------------------------------------------------------------*/


BOOL ReadBytes( BYTE* pvResult, INT32 cResultMemSize, INT32 cBytesToRead, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    INT32   iSizeDiff = cResultMemSize - cBytesToRead;

    if ( ((*pos + cBytesToRead) > BPend) || (iSizeDiff < 0) )
        return FALSE;

    memset( pvResult, 0, (size_t)iSizeDiff );
    memcpy( pvResult + iSizeDiff, BP+*pos, (size_t)cBytesToRead);

    return TRUE;
}


BOOL StepBytes( UINT32 cb, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    if ( bOk = ((*pos + cb) <= BPend) )
    {
        *pos += cb;
    }

    return bOk;
}


BOOL ReadByte( BYTE* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    if ( bOk = ReadBytes( (BYTE*) Result, sizeof(BYTE), 1, BP, BPend, pos ) )
    {
        (*pos) += 1;
    }

    return bOk;
}

BOOL ReadInt8( INT8* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    if ( bOk = ReadBytes( (BYTE*) Result, sizeof(INT8), 1, BP, BPend, pos ) )
    {
        (*pos) += 1;
    }

    return bOk;
}

BOOL ReadUInt8( UINT8* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    if ( bOk = ReadBytes( (BYTE*) Result, sizeof(UINT8), 1, BP, BPend, pos ) )
    {
        (*pos) += 1;
    }

    return bOk;
}


BOOL ReadInt16( INT16* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    *Result = 0;

    if ( bOk = ((*pos + 2) <= BPend) )
    {
        *Result |= ((INT16) *(BP + *pos )) << 8;
        *Result |= ((INT16) *(BP + *pos+1));

        (*pos) += 2;
    }

    return bOk;
}


BOOL ReadUInt16( UINT16* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    *Result = 0;

    if ( bOk = ((*pos + 2) <= BPend) )
    {
        *Result |= ((UINT16) *(BP + *pos)) << 8;
        *Result |= ((UINT16) *(BP + *pos+1));

        (*pos) += 2;
    }

    return bOk;
}

BOOL ReadInt32( INT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    *Result = 0;

    if ( bOk = ((*pos + 4) <= BPend) )
    {
        *Result |= ((INT32) *(BP + *pos))   << 24;
        *Result |= ((INT32) *(BP + *pos+1)) << 16;
        *Result |= ((INT32) *(BP + *pos+2)) <<  8;
        *Result |= ((INT32) *(BP + *pos+3));

        (*pos) +=4;
    }

    return bOk;
}

BOOL ReadUInt32( UINT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;

    *Result = 0;

    if ( bOk = ((*pos + 4) <= BPend) )
    {
        *Result |= ((UINT32) *(BP + *pos))   << 24;
        *Result |= ((UINT32) *(BP + *pos+1)) << 16;
        *Result |= ((UINT32) *(BP + *pos+2)) <<  8;
        *Result |= ((UINT32) *(BP + *pos+3));

        (*pos) += 4;
    }

    return bOk;
}

BOOL ReadFloat32( FLOAT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    return ReadUInt32((UINT32*)Result, BP, BPend, pos);
}


UINT8 Read5bFrom8b( BYTE b )
{
    return (UINT8) ((BOOL)(b & BIT_MASK_5));
}

UINT8 Read4bFrom8b( BYTE b )
{
    return (UINT8) ((BOOL)(b & BIT_MASK_4));
}

UINT8 Read3bFrom8b( BYTE b )
{
    return (UINT8) ((BOOL)(b & BIT_MASK_3));
}


/**** Memory READ functions -- end *******************************************************************/


/*********************************************************
 * Routines that handle the type Uintvar.
 * A Uintvar is a way of storing unsigned integers of max 32 bits
 * in as few bytes as possible. To this purpose, 7 bits in each
 * byte are used for data storage, and the eigth bit is a
 * "continuation" bit, that indicates whether more bytes follow
 * or not. At most 5 bytes might be required, but usually 1 or 2
 * bytes will suffice.
 */

/* Return the number of bytes required to store n as a Uintvar. */
UINT32 UintvarLen (UINT32 n)
{
  if (n < 0x80)
    return 1;
  else if (n < 0x4000)
    return 2;
  else if (n < 0x200000)
    return 3;
  else if (n < 0x10000000)
    return 4;
  else
    return 5;
} /* End of UintvarLen */

/* Convert n to a Uintvar, and store at the location indicated by v.
 * Return the number of bytes used by the Uintvar. */
UINT32 IntToUintvar (UINT32 n, BYTE *v)
{
  UINT32 k = UintvarLen (n);

  v += (k - 1);

  *v-- = (BYTE)(n & 0x7F);
  n >>= 7;

  while (n > 0) {
    *v-- = (BYTE)(0x80 | (n & 0x7F));
    n >>= 7;
  }

  return k;
}

/* Convert a Uintvar, stored as consecutive bytes in s, to an int.
 * Store the result in *pn.
 * Return the number of bytes used by the Uintvar.
 * maxLen is the max number of bytes we are allowed to look at;
 * if the Uintvar does not stop before this, it is an error.
 * On error, return 0; */
UINT32 UintvarToInt (BYTE *s, UINT32 *pn, UINT32 maxLen)
{
  BYTE b;
  UINT32 k = 0;
  UINT32 value = 0;

  do {
    if (k >= maxLen) { return 0; }
    b = *s++;
    value <<= 7;
    value |= (b & 0x7F);
    k++;
    if (k > 5) { return 0; }
  } while (b > 0x7F);

  *pn = value;
  return k;
}


BOOL ReadMBUInt16( UINT16* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;
    UINT32  mbLen;
    UINT32  mbResult;

    mbLen = UintvarToInt (BP + *pos, &mbResult, (BPend - *pos));

    if ( bOk = ((mbLen > 0) && ((*pos + mbLen) <= BPend )) )
    {
        if ( bOk = ((mbResult & BIT_MASK_16_OF_32 ) == 0) )
        {
            *Result = (UINT16) mbResult;
            *pos += mbLen;
        }
        else
        {
            *Result = 0;
        }
    }

    return bOk;
}


BOOL ReadMBUInt32( UINT32* Result, BYTE* BP, UINT32 BPend, UINT32 *pos )
{
    BOOL    bOk;
    UINT32  mbLen;

    mbLen = UintvarToInt (BP + *pos, Result, (BPend - *pos));

    if ( bOk = ((mbLen > 0) && ((*pos + mbLen) <= BPend)) )
    {
        *pos += mbLen;
    }

    return bOk;
}




/*************************************************/


BYTE* wip_wchar2byte( const WCHAR *src, BOOL *overflowDetected )
{
    BYTE    *result = NULL;
    UINT32  len;
    UINT32  i = 0;

	*overflowDetected = FALSE;

	if ( src != NULL )
	{
		len = STRINGLENGTH( src );

		result = NEWARRAY( BYTE, len + 1 );
		result[ len ] = 0;
		for (i = 0; i < len; i++)
		{
			result[i] = (BYTE)( src[i] );

			*overflowDetected |= (src[i] > 0xFF);
		}
	}

	return result;
}


BOOL SDL_wchar2byte(void** ppwchInText)
{
    WCHAR   **pSrc = (WCHAR**) ppwchInText;
    BYTE    *result;
    BOOL    overflow = FALSE;

	result = wip_wchar2byte( *pSrc, &overflow );
	DEALLOC( ppwchInText );
	*ppwchInText = (void*) (result);

	return ( ! overflow);
}

void SDL_byte2wchar(void** ppbInText)
{
    BYTE    **pSrc = (BYTE**) ppbInText;
    WCHAR   *result;

	result = w_cstr2wstr( (const CHAR*) *pSrc );
	DEALLOC( ppbInText );
	*ppbInText = (void*) (result);
}


/*************************************************/


/* Array used by ByteToHex below. */
static const BYTE hexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

/* Changes a byte to the string "XY", where XY are the hexadecimal digits
   that represent the value of the byte, and writes that string to
   the location indicated by 'bs'. */
void ByteToHex (BYTE bByte, BYTE *bs)
{
	*bs++ = hexDigits[(bByte >> 4) & 0xF];
	*bs = hexDigits[bByte & 0xF];
}

/* Changes a hexadecimal representation, stored in the string 'bs',
   to the byte it represents, and stores the value at the location
   indicated by 'res'. Returns FALSE if the string at 'bs' does
   not start with two hexadecimal digits, TRUE otherwise. */
BOOL HexToByte (const BYTE *bs, BYTE *res)
{
    BYTE    b, bByte;

	if (bs == NULL)
		return FALSE;

	b = DowncaseByte (*bs++);
	if (wae_isdigit (b))
		bByte = (BYTE)(b - '0');
	else if (wae_ishexletter (b))
		bByte = (BYTE)(b - 'a' + 10);
	else
		return FALSE;

	b = DowncaseByte (*bs);
	if (wae_isdigit (b))
		bByte = (BYTE)((bByte << 4) + b - '0');
	else if (wae_ishexletter (b))
		bByte = (BYTE)((bByte << 4) + b - 'a' + 10);
	else
		return FALSE;

	*res = bByte;
	return TRUE;
}


WCHAR **AboutStrings2Wchar(const CHAR *s)
{
    WCHAR   **strings;
    WCHAR   **iStrings;
    const CHAR * const  *ppVersionStr = aboutStrings;
    const char  t[4] = {0x5F,0x64,0x76,0x00};

    if (strstr(s, t))
	{
		char s[170];
		char *is = s;
		char2ucsTable(s);
		strings = (WCHAR**)OSConnectorAlloc(sizeof(WCHAR**)*17);
		iStrings = strings;
		while (*is != 0)
		{
			*iStrings++ = w_cstr2wstr(is);
			is = strchr(is, 0) + 1;
		}
		*iStrings = NULL;
	}
	else
	{
		strings = (WCHAR**)OSConnectorAlloc(sizeof(WCHAR**)*3);
		iStrings = strings;
		while (*ppVersionStr != NULL)
			*iStrings++ = w_cstr2wstr(*ppVersionStr++);
		*iStrings = NULL;
	}

    return strings;
}
