#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include <ansilibs.h>
#include <confvars.h>
#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif

#include "aapimem.h"
#include "aapiclnt.h"

#include "msg.h"
#include "ui.h"
#include "waptask.h"

/*  extern in other files */
const memSizeType   cCacheSize = MAX_CACHE_SIZE;
const memSizeType   crestoredSize = 0;

char cache[MAX_CACHE_SIZE];

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
#ifdef WAPCORE_FOR_WARNING
	MSG_LOW(" Dummy for WARNING in aapimem.c",0,0,0);
#endif //WAPCORE_FOR_WARNING
}

memSizeType MEMa_readCache (memSizeType pos, memSizeType size, CHAR *buffer)
{
	if (!s_initialized) initCache();
	memcpy(buffer, cache+pos, size);
    return size;
}

memSizeType MEMa_writeCache (memSizeType pos, memSizeType size, const CHAR *buffer)
{
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
