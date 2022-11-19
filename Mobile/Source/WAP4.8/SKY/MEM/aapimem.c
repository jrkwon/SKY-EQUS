
#include <ansilibs.h>
#include <confvars.h>
#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif

#include "aapimem.h"
#include "aapiclnt.h"
#include "capimem.h"

#include "msg.h"
#include "wapcommon.h"

/*  extern in other files */
const memSizeType   cCacheSize = MAX_CACHE_SIZE;
const memSizeType   crestoredSize = 0;

char cache[MAX_CACHE_SIZE];

#ifdef FEATURE_WAP
//TMP define!!
char g_browserdb[DATABASE_STORAGESIZE];
int s_dbinitialized = 0;

//TMP for WTAI
char g_browserWTArepository[REP_STORAGESIZE];
int s_srvrepinitialized = 0;

//TMP for PUSH

char g_browserPUSHrepository[PUSH_STORAGESIZE];
int s_pushinitialized = 0;
#endif //#ifdef FEATURE_WAP



#define CACHETEST
#ifdef  CACHETEST
        int s_initialized = 0;
#else 
static  int s_initialized = 0;
#endif

void    initCache(void)
{
    s_initialized = 1;
	memset(cache, 0x00, MAX_CACHE_SIZE);
}

#ifdef FEATURE_WAP
void    initBrowserDB(void)
{
    s_dbinitialized = 1;
	memset(g_browserdb, 0x00, DATABASE_STORAGESIZE);
}

void    initBrowserRepository(void)
{
    s_srvrepinitialized = 1;
	memset(g_browserWTArepository, 0x00, REP_STORAGESIZE);
}

void    initBrowserPUSHRepository(void)
{
    s_pushinitialized = 1;
	memset(g_browserPUSHrepository, 0x00, PUSH_STORAGESIZE);
}
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_WAP
UINT32  MEMa_readCache  (UINT32 pos, UINT32 size, CHAR *buffer)
#else
memSizeType MEMa_readCache (memSizeType pos, memSizeType size, CHAR *buffer)
#endif //FEATURE_WAP
{
	CLNTa_log(0, 0, "MEMa_readCache Called\n");
	if (!s_initialized) initCache();
	memcpy(buffer, cache+pos, size);
    return size;
}

#ifdef FEATURE_WAP
UINT32  MEMa_writeCache (UINT32 pos, UINT32 size, const CHAR *buffer)
#else
memSizeType MEMa_writeCache (memSizeType pos, memSizeType size, const CHAR *buffer)
#endif //FEATURE_WAP
{
	CLNTa_log(0, 0, "MEMa_writeCache Called\n");

    if (!s_initialized)  initCache();
	if((pos+size) > cCacheSize)
	{
		return 0;
	}
	else
	{
		memcpy(cache+pos, buffer, size);
	}
    return size;
}


VOID    MEMa_cachePrepared (VOID) 
{
	extern void ProcessEvents(void);

	CLNTa_log(0, 0, "MEMa_cachePrepared Called\n");
	CLNTc_terminate();
		ProcessEvents();
}

UINT32  MEMa_readPushRepository  (UINT32 pos, UINT32 size, CHAR *buffer)
{  
#ifdef FEATURE_WAP
    MSG_MED("DUMMY readPushRepository",0,0,0);
    if (!s_pushinitialized) initBrowserPUSHRepository();
	memcpy(buffer, g_browserPUSHrepository+pos, size);
    return size;    
#endif //#ifdef FEATURE_WAP
}

UINT32  MEMa_writePushRepository (UINT32 pos, UINT32 size, const CHAR *buffer)
{    
#ifdef FEATURE_WAP
    MSG_MED("DUMMY MEMa_writePushRepository",0,0,0);
    if (!s_pushinitialized)  initBrowserPUSHRepository();
	if((pos+size) > PUSH_STORAGESIZE)
	{
		return 0;
	}
	else
	{
		memcpy(g_browserPUSHrepository+pos, buffer, size);
	}
    return size;    
#endif //#ifdef FEATURE_WAP
}

VOID	MEMa_pushRepositoryClosed (VOID)
{
    MSG_MED("MEMa_pushRepositoryClosed",0,0,0);
    //tmp
    //have to save push data in persistent memory
    //MEMc_prepareCache(MAX_CACHE_SIZE);
	//ProcessEvents();
}


UINT32  MEMa_readDatabase  (UINT32 pos, UINT32 size, CHAR *buffer)
{
#ifdef FEATURE_WAP
    MSG_MED("readdb",0,0,0);
    if (!s_dbinitialized) initBrowserDB();
	memcpy(buffer, g_browserdb+pos, size);
    return size;    
#endif //#ifdef FEATURE_WAP
}

UINT32  MEMa_writeDatabase (UINT32 pos, UINT32 size, const CHAR *buffer)
{
#ifdef FEATURE_WAP
    if (!s_dbinitialized)  initBrowserDB();
	if((pos+size) > DATABASE_STORAGESIZE)
	{
		return 0;
	}
	else
	{
		memcpy(g_browserdb+pos, buffer, size);
	}
    MSG_MED("wrtiedb",0,0,0);
    return size;       
#endif //#ifdef FEATURE_WAP
}

UINT32  MEMa_readServiceRepository  (UINT32 pos, UINT32 size, CHAR *buffer)
{
#ifdef FEATURE_WAP
    MSG_MED("DUMMY MEMa_readServiceRepository",0,0,0);
    if (!s_srvrepinitialized) initBrowserRepository();
	memcpy(buffer, g_browserWTArepository+pos, size);
    return size;    
#endif //#ifdef FEATURE_WAP
}

UINT32  MEMa_writeServiceRepository (UINT32 pos, UINT32 size, const CHAR *buffer)
{
#ifdef FEATURE_WAP
    MSG_MED("DUMMY MEMa_writeServiceRepository",0,0,0);
    if (!s_srvrepinitialized)  initBrowserRepository();
	if((pos+size) > REP_STORAGESIZE)
	{
		return 0;
	}
	else
	{
		memcpy(g_browserWTArepository+pos, buffer, size);
	}
    return size;    
#endif //#ifdef FEATURE_WAP
}
