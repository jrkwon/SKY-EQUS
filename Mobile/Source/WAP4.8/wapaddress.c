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
extern char           Image$$APP_SHARE_RAM$$Base[]; //dummy
unsigned char *pShareRAM = (unsigned char*)Image$$APP_SHARE_RAM$$Base;
unsigned char *m_gRXBuf, *m_gObjBuf, *m_gCodeBuf, *m_gImageBuf, *m_gUncompBuf;
//////////////////////////////////////////////////////////////////////////


void AddressAssign(void)
{
	m_gRXBuf     = (unsigned char*)&pShareRAM[MAX_NTOP_SIZE];
	m_gObjBuf    = (unsigned char*)&pShareRAM[MAX_SISRAM_START];
	m_gCodeBuf   = &m_gObjBuf[MAX_OBJBUF_SIZE];
	m_gImageBuf  = &m_gCodeBuf[MAX_CODEBUF_SIZE];     //SIS Decoded Data Area
	m_gUncompBuf = &m_gImageBuf[MAX_IMAGEBUF_SIZE];   //SIS Uncompressed Data Area
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
	TRS = 3,    //TRANS_DATA
	RXBUF  = 4, //DECODE COMPRESSED IMAGE
	COMP_IMG = 5,
	DECOMP_IMG =6
};

unsigned char* SeekWorkingArea(int work)
{
	switch(work)
	{
	case BMP://1
	case TRS://3
	case COMP_IMG://5
		memset((void*) m_gCodeBuf, 0, MAX_CODEBUF_SIZE);
		return m_gCodeBuf;
		break;
	case CNT://2
		memset((void*) m_gImageBuf, 0, MAX_IMAGEBUF_SIZE);
		return m_gImageBuf;
		break;
	case RXBUF://4
		memset((void*) m_gRXBuf, 0, MAX_RXBUF_SIZE);
		return m_gRXBuf;
		break;
	case DECOMP_IMG://6
		memset((void*) m_gObjBuf, 0, MAX_OBJBUF_SIZE);
		return m_gObjBuf;
		break;
	default:
		return NULL;
		break;
	}
}


