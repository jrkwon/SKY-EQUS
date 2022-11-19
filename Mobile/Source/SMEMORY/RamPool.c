/*******************************************************************************

	RAM POOL FILE
	Created by chul at 2000.4.3

	This file include the SRAM variables shared by several MMI files

*******************************************************************************/
#include <stdlib.h>		
#include <memory.h>
#include <stdio.h>		

#include "target.h"
#include "customer.h"
#include "comdef.h"

#include <wapmemory.h>
 
#include "uistate.h"
#include "uixscrn.h"
#include "uiuint.h"
#include "uixsnd.h"
#include "uiutxt.h"
#include "uiutxti.h"
#include "hs.h"
#include "nv.h" // instead of #include	<ee_data.h> in A-project
#include "hsig.h"
//#include "err.h"
#include "uiCommon.h"

char				ui_MMFileList[MM_MAX_MMEDIA_FILES * NUM_LCD_COLUMNS];

char				ui_MMFileListInt[MM_MAX_MMEDIA_FILES * MM_MAX_FPATH_NAME];

byte				ui_MMFileBlockBuffer[MM_ANI_FRAME_4C_SIZE];

mm_FileHandle_Type	ui_MMFileHandle;

//mm_AN1Seq_Type		ui_AniSeqBuffer[256];


//mm_FileHandle_Type	ui_MMAniPlayFileHandle;

mm_AN1Seq_Type		ui_MMAniPlaySeqBuffer[256];

byte				ui_MMAniPlayBuffer[MM_ANI_FRAME_4C_SIZE];
