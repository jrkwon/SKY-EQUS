/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


							WAP Address Assign Module

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/17/2001 jmpark  Create WAP address initialize for SIS, PhotoMail
===========================================================================*/
/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "msg.h"
#include "wapadapter.h"
#include "wapconnector.h"
#include "clnt.h"
#include "confvars.h"
#include <memory.h>

#include "aapimem.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "capimem.h"
#include "capimmi.h"
#include "netuser.h"
#include "tapicmmn.h"

#include "sisdecoder.h"

//////////////////////////////////////////////////////////////////////////
char           Image$$APP_SHARE_RAM$$Base[]; //dummy
unsigned char *pShareRAM = (unsigned char*)Image$$APP_SHARE_RAM$$Base;
unsigned char *m_gObjBuf, *m_gCodeBuf, *m_gImageBuf, *m_gUncompBuf;
//////////////////////////////////////////////////////////////////////////


void AddressAssign(void)
{
#if 1
	m_gObjBuf    = (unsigned char*)&pShareRAM[MAX_SISRAM_START];//SIS Area
#else
	unsigned int act_addr = 0;
	char* temp_addr = (char*)pShareRAM ;

	temp_addr += (char)0x10;
	act_addr = (unsigned int)temp_addr;
	act_addr &= 0xfffffff0;

	m_gObjBuf    = (unsigned char*)act_addr + MAX_SISRAM_START;//SIS Area
#endif//0
	m_gCodeBuf   = &m_gObjBuf[MAX_OBJBUF_SIZE];
	m_gImageBuf  = &m_gCodeBuf[MAX_CODEBUF_SIZE];     //SIS Decoded Data Area
	m_gUncompBuf = &m_gImageBuf[MAX_IMAGEBUF_SIZE];   //SIS Uncompressed Data Area

//  Application should initailize the correspondent memory before action
//	memset(m_gObjBuf, 0, MAX_OBJBUF_SIZE + MAX_CODEBUF_SIZE);
}


void InitWipMalloc(void)
{
	extern INT16 wip_initmalloc (void *mem, UINT32 memsize);

	memset(pShareRAM, 0 ,WIP_MALLOC_MEM_SIZE);  
	wip_initmalloc(pShareRAM, WIP_MALLOC_MEM_SIZE);
}

enum {
	BMP = 1,    //BMP_DATA
	CNT = 2,    //CONTENT_SIZE
	TRS = 3     //TRANS_DATA
};

unsigned char* SeekWorkingArea(int work)
{
	switch(work)
	{
	case BMP://1
	case TRS://3
		memset((void*) m_gCodeBuf, 0, MAX_CODEBUF_SIZE);
		return m_gCodeBuf;
		break;
	case CNT://2
		memset((void*) m_gImageBuf, 0, MAX_IMAGEBUF_SIZE);
		return m_gImageBuf;
		break;
	default:
		return NULL;
		break;
	}
}



