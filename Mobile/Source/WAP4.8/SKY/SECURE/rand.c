#include <string.h>
#include "const.h"

extern void DLX_RAND_Bytes( unsigned char *buf,int num);
extern void DLX_RAND_Seed( unsigned char *buf,int num);
extern unsigned long ui_get_local_time(void);

void RAND_buffer(unsigned char *ucpBuf, int nBufLen)
{
	static int 	nInit = -1;
	ul			ulTime;

	if (nInit == -1) {
		//ulTime = (ul)(ui_get_local_time()+ELAPSED_SECONDS);
		ulTime = 0x00000000;
		DLX_RAND_Seed((unsigned char *)&ulTime, sizeof(ulTime));
		nInit = 0;
	}
	DLX_RAND_Bytes(ucpBuf, nBufLen);
}
