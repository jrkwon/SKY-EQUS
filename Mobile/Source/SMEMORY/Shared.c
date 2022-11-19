/*******************************************************************************

	RAM POOL FILE
	Created by chul at 2000.4.11

	This file include the SRAM variables shared by WAP
	The following variables will not be intialized

*******************************************************************************/
//#include <stdlib.h>		
//#include <memory.h>
//#include <stdio.h>		

#include "target.h"
#include "customer.h"
#include "comdef.h"

//#include <wapmemory.h>
 
// not yet on build:: #include "hsig.h"
// not yet on build:: #include "IrDA\irDAEx.h"
/***** IrDA exchange related variables  *****/
// Rx, Tx message buffer
// not yet on build:: IREX_MSG_TYPE	irex_RxMsg, irex_TxMsg;
// not yet on build:: byte			irex_SendBuf[MAX_PACKET_LENGTH]; // = (byte*)sorted_groupname;
// not yet on build:: byte			irex_RxBuf[MAX_PACKET_LENGTH];


/***** for voice record *********************/
#include "FileSys.h"


//---------------------------------------------
// 256 bytes read-ahead/write-back buffer. (EXPORTED).
byte cell_buf_onecell[MAX_CELL_NUM][FS_VOICE_WRITE_UNIT] = {0,};			// FEATURE_EFS
//---------------------------------------------
// girin. comment. 2000.12.27
// I don't know why I should initialize above array to make it 
// valid pre-defined Linker symbol of 'Image$$APP_SHARE_RAM$$Base'.