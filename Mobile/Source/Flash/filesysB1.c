/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


       F L A S H    M E M O R Y    F I L E - S Y S T E M  

GENERAL DESCRIPTION


EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 1999 by SK Teletech INCORPORATED.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.04.26   chos    cell_buf_onecell[][] move to share.c
01/31/00   jnlee   Created.
------------------------------------------------------------------------------*/


#include <memory.h>
#include <stdlib.h>
#include "comdef.h"
#include "err.h"
#include "dog.h"

//#include "eqs.h"
//#include "sky_ui.h"
#include "ska.h"
#include "ska_vcmm.h"


#include "FileSys.h"
#include "nv.h"
//#include "nvsetup.h"    
#include "nvim.h"
#include "fs.h"

//****************** in the Fsi.h *************
typedef enum {
  FSI_GC_DYNAMIC,      /* Dynamically evaluate conditions */
  FSI_GC_FAVORABLE,    /* Bias towards doing garbage collection */
  FSI_GC_IF_REQ,       /* Do GC only if required to maintain clean blocks */
  FSI_GC_ONE,          /* Do GC on at least one sector */
  FSI_GC_ALL           /* Do GC on all sectors */
} fsi_gc_directives_type;           
//***********************************************
boolean  fs_alloc_gc_eval(fsi_gc_directives_type gc_directive);


extern FN_SEQUENCE_T Fn_sequence[NV_MAX_FL_FILE_NUM]; // defined in VocCommon.c

// by chos 00.04.26
// move to share.c
//---------------------------------------------
// 256 bytes read-ahead/write-back buffer. (EXPORTED).
//byte cell_buf_onecell[MAX_CELL_NUM][FS_VOICE_WRITE_UNIT];			// FEATURE_EFS
//---------------------------------------------
static word cell_buf_offset;
static word cell_buf_num[MAX_CELL_NUM];

static boolean cell_buf_empty;
static boolean cell_buf_full[MAX_CELL_NUM];
boolean voice_is_recording=FALSE;
fs_conf_code_type vmemo_confirmation_code = {0,NV_INVALID_CONF_CODE};

static byte fl_AvailIndxNum;

#ifdef VOICE_RECOGNITION_FEATURE
static byte vd_fl_AvailIndxNum;
#endif // VOICE_RECOGNITION_FEATURE

void EnsureVoiceRecIntegrity(void);
byte GatherMissingLinkGetAvailIndxNum(void);

#ifdef VOICE_RECOGNITION_FEATURE

void EnsureVDIntegrity(void);
byte vd_GatherMissingLinkGetAvailIndxNum(void);

#endif // VOICE_RECOGNITION_FEATURE


#ifdef FEATURE_EFS
static nvim_efs_params nvim_fs_vmemo[NV_MAX_FL_FILE_NUM] = 
{
   {"vmemo0", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo1", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo2", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo3", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo4", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo5", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo6", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo7", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo8", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo9", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo10", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo11", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo12", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo13", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo14", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo15", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo16", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo17", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo18", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo19", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo20", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0}
/*
   ,
   {"vmemo21", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo22", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo23", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo24", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo25", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo26", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo27", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo28", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo29", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vmemo30", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0}
*/
};

#ifdef VOICE_RECOGNITION_FEATURE
static nvim_efs_params nvim_fs_vd[NV_VD_MAX_FL_FILE_NUM] = 
{
   {"vd0", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd1", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd2", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd3", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd4", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd5", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd6", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd7", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd8", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd9", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd10", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd11", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd12", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd13", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd14", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd15", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd16", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd17", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd18", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"vd19", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
};
#endif
byte  cell_sel = 0;
#endif	//ifdef FEATURE_EFS


typedef struct {
  fs_handle_type        handle;
  void					*buffer;
  dword                 count; 
  fs_callback_p_type    callback_function;
}flf_read_write_arg;
flf_read_write_arg flf_rw_arg;
byte flfreadwrite;

#ifdef VOICE_RECOGNITION_FEATURE
boolean vd_update_superblock(void)
{
	return TRUE;
}
#endif // VOICE_RECOGNITION_FEATURE

void backup_superblock(void)
{

}
	
/**********************************************************************
	vmemo_reserve_space			by jnlee 2000.1.15
	녹음용 영역 확보 

**********************************************************************/

/* 지우는 작업시나, 처음 부팅시 사용되지 않는 Fn을 찾아서 해당 녹음삭제*/
/* 2000.1.20 by jnlee */
void erase_unused_Fn(void)
{
	int count;
	for(count=0;count<NV_MAX_FL_FILE_NUM;count++)
	{
		int n;

		if( (gVMemoInfo.fUserVoice == TRUE) && 
			(count ==gVMemoInfo.bUserVoiceFn))
		{	; // 사용자동응답 녹음이 있으면 해당 녹음은 지우지 않는다.
		}
		else
		{
			{	// shinji 99/09/06 Draw sandclass
//				extern	void	DrawSandGlass(char*, char*);
//				DrawSandGlass(NULL,NULL);
			}	// shinji
			for(n=0;n<gVMemoInfo.bTotalRecNumber;n++)
			{
				if(Fn_sequence[n].Fn == count)
					break;
			}
			if(n==gVMemoInfo.bTotalRecNumber)
			{
				flferase(count);
			}
		}
	}
}

#ifdef VOICE_RECOGNITION_FEATURE

void vd_erase_unused_Fn(void)
{
	int count;

	for(count=0;count<NV_VD_MAX_FL_FILE_NUM;count++)
	{
		int n;
//		{	extern	void	DrawSandGlass(char*, char*);
//			DrawSandGlass(NULL,NULL);
//		}	
		for(n=0;n<vDial_tHeader.total_num;n++)
		{
			if(vDial_tHeader.Fn[n] == count)
				break;
		}
		if(n==vDial_tHeader.total_num)
		{
			vd_flferase(count);
		}
	}

}
#endif
	
boolean read_superblock(void)
{
	byte i;
    //VOICEINFO_T* voice_info;
	//nv_voice_info_type voice_info;            hjcho 01.05.12
    //voice_info = SKY_GET_VOICE_INFO();

//---------------------------------------------------------------------------------------------------
// voice_info                                   hjcho 01.05.12
	//if (NV_DONE_S != SKY_GetNV(NV_VOICE_INFO_I, (nv_item_type *)&voice_info))
	//	return FALSE;


//	ui.voice_info.total_rec_num = voice_info.total_rec_num;
//	ui.voice_info.new_ans_num = voice_info.new_ans_num;
//	ui.voice_info.is_user_voice = voice_info.is_user_voice;
//	ui.voice_info.user_voice_fn = voice_info.user_voice_fn;
//	ui.voice_info.user_voice_l = voice_info.user_voice_l; 
//	ui.voice_info.remained_byte = voice_info.remained_byte;
//	ui.voice_info.remained_time = voice_info.remained_time;

//	gVMemoInfo.bTotalRecNumber= voice_info.total_rec_num;
//	gVMemoInfo.bNewAutoAnsNumber = voice_info.new_ans_num;
//	gVMemoInfo.fUserVoice = voice_info.is_user_voice;
//	gVMemoInfo.bUserVoiceFn = voice_info.user_voice_fn;
//	gVMemoInfo.wUsedTime = voice_info.user_voice_l; 
//	gVMemoInfo.dwRemainedByte = voice_info.remained_byte;
//	gVMemoInfo.wRemainedTime = voice_info.remained_time;

    memcpy(&gVMemoInfo, SKY_GET_VOICE_INFO(), sizeof(VMEMOINFO_T));

//---------------------------------------------------------------------------------------------------
// Fn_sequence
	for (i = 0; i < NV_MAX_FL_FILE_NUM; i++)
	{
       Fn_sequence[i].address = i;
//	   if (NV_DONE_S != SKY_GetNV(NV_FN_SEQUENCE_I, (nv_item_type *)&Fn_sequence[i]))
//         	return FALSE;		
	   Fn_sequence[i] = SKY_GET_FN_SEQUENCE(i);
	}
//---------------------------------------------------------------------------------------------------
	erase_unused_Fn();
    (void)fs_alloc_gc_eval(FSI_GC_ALL);		// agian !!

	return TRUE;
}

#ifdef VOICE_RECOGNITION_FEATURE

#ifndef TREX_VOICE_APP//TREX_HAPPY
nv_voice_templete_header_type vDial_tHeader; // girin. 99.8.14
#endif
//#ifndef VOICE_RECOGNITION_FEATURE
// by happykid for removing voice recognition
nv_voice_templete_header_type vDial_tHeader; // girin. 99.8.14
//#endif

#endif // VOICE_RECOGNITION_FEATURE

#ifdef VOICE_RECOGNITION_FEATURE

boolean vd_read_superblock(void)
{

	if (NV_DONE_S != SKY_GetNV(NV_VOICE_TEMPLETE_HEADER_I, (nv_item_type *)&vDial_tHeader))
		return FALSE;

	vd_erase_unused_Fn();

	return TRUE;
}
#endif // VOICE_RECOGNITION_FEATURE

boolean flffdisk(void)
{

	if (TRUE != read_superblock())
		return FALSE;
//----------------------------------------------
	cell_buf_offset = (word)0;

	cell_buf_empty = TRUE;
	cell_buf_num[0] = 0;
	cell_buf_full[0] = FALSE;
	fl_AvailIndxNum = NV_FL_EOC;
	return TRUE;
}

#ifdef VOICE_RECOGNITION_FEATURE
boolean vd_flffdisk(void)
{
	// read FTAB, used_map, free_map, vDial_tHeader, free_start, free_end
	if (TRUE != vd_read_superblock())
		return FALSE;
 
	vd_fl_AvailIndxNum = NV_VD_FL_EOC;
//----------------------------------------------
	return TRUE;
}

#endif // VOICE_RECOGNITION_FEATURE

//-------------------------------------------------------------------------------------
void FormatVoiceFileSysDisastrous(void)
{
}

#ifdef VOICE_RECOGNITION_FEATURE

void FormatVDFileSysDisastrous(void)
{
}
#endif // VOICE_RECOGNITION_FEATURE

void EnsureVoiceRecIntegrity(void)
{
}

#ifdef VOICE_RECOGNITION_FEATURE

void EnsureVDIntegrity(void)
{
}

#endif // VOICE_RECOGNITION_FEATURE

byte GatherMissingLinkGetAvailIndxNum(void)
{ // return the number of revived cells.
	return 0;
}

int getfreeindxnum(void)
{
	int count;
	dword time_sum=0;
	
	for(count=0;count<NV_MAX_FL_FILE_NUM;count++)
	{
		if(Fn_sequence[count].Type != VTYPE_NONE)
			time_sum += Fn_sequence[count].Time;
	}

	if(time_sum >= TOTAL_REC_TIME )
	// 현존하는 파일의 시간을 다 더했더니 총 녹음가능시간을 넘는구나
	// 2000년 3월 막날
		return(0);
	else
	{
		time_sum = TOTAL_REC_TIME - time_sum;			// 1초 단위는 1킬로 바이트임 
		return(time_sum);								// 킬로 바이트로 리턴함 
	}
}

#ifdef VOICE_RECOGNITION_FEATURE

int vd_getfreeindxnum(void)
{
	// Kbyte단위로 변경 -> reserved write시에는 사용불가
	// 여유는 충분하므로, 항상 여유있음 00.2.10
	return (fs_space_available() >> 10);		
}


byte vd_GatherMissingLinkGetAvailIndxNum(void)
{ // return the number of revived cells.
	return 0;
}


#endif // VOICE_RECOGNITION_FEATURE
		
byte getfn(void)		//for EFS
{
	byte i;
	fs_rsp_msg_type rsp_msg;        /* EFS status */

	for(i = 0; i<NV_MAX_FL_FILE_NUM;i++)
	{
		fs_test (nvim_fs_vmemo[i].fname,
				NULL,
				&rsp_msg
				);
		if(rsp_msg.nametest.name_found == FALSE)
		{
			return i;
		}
	}
	return NV_MAX_FL_FILE_NUM;
}		

//for EFS
boolean flfopen(byte fn, byte op_type) // op_type 0: read, 1: write, 2: append
{
	fs_rsp_msg_type rsp_msg;
	fs_open_xparms_type fs_open_xparms;

	if(op_type == 0)	//read
	{
		//**this is used in  flfwrite!!**
		cell_buf_full[0] = FALSE;
		cell_buf_num[0] = 0;
		cell_buf_full[1] = FALSE;
		cell_buf_num[1] = 0;
		cell_sel = 0;
		cell_buf_offset = 1;			// this is used for "first read" flag
		//*******************************

		fs_open_xparms.create.cleanup_option   = FS_OC_CLOSE;
		fs_open_xparms.create.buffering_option = FS_OB_ALLOW;
		fs_open_xparms.create.attribute_mask   = FS_FA_UNRESTRICTED;

		 fs_open(nvim_fs_vmemo[fn].fname,	/* Name of file to open */
			 FS_OA_READONLY,                 /* Open mode */
			 NULL,            /* Optional parameters */
			 NULL,                         /* Callback function ptr */
			 &rsp_msg);               /* Response message ptr */

		if (rsp_msg.open.handle == FS_NULL_HANDLE)
		{
		   return FALSE;
		}
		nvim_fs_vmemo[fn].rd_handle = rsp_msg.open.handle;
		return TRUE;
	} 
	else if(op_type == 1)	//write
	{
		//**this is used in  flfwrite!!**
		cell_buf_full[0] = FALSE;
		cell_buf_num[0] = 0;
		cell_buf_full[1] = FALSE;
		cell_buf_num[1] = 0;
		cell_sel = 0;
		voice_is_recording = TRUE;		// recording will start -> no GC by jnlee 2000.1.13
		cell_buf_offset = 1;			// this is used for "first read" flag
		//*******************************

		fs_open_xparms.create.cleanup_option   = FS_OC_DISCARD;	//all or nothing
		fs_open_xparms.create.buffering_option = FS_OB_ALLOW;
		fs_open_xparms.create.attribute_mask   = FS_FA_UNRESTRICTED;

		 fs_open(nvim_fs_vmemo[fn].fname,	/* Name of file to open */
			 FS_OA_CREATE,                 /* Open mode */
			 &fs_open_xparms,            /* Optional parameters */
			 NULL,                         /* Callback function ptr */
			 &rsp_msg);               /* Response message ptr */

		if (rsp_msg.open.handle == FS_NULL_HANDLE)
		{
		   return FALSE;
		}
		nvim_fs_vmemo[fn].wr_handle = rsp_msg.open.handle;
		return TRUE;
	}
	else	// 아직 미정인 경우 
		return TRUE;
}

dword flfread(byte fn, byte *data, dword size)		//for EFS
{
	fs_rsp_msg_type rsp_msg;
	dword i,rest_size;

	if (size==0)
		return 0;
	if(nvim_fs_vmemo[fn].rd_handle == FS_NULL_HANDLE)
		return 0;

	if( (cell_buf_num[0] > (FS_VOICE_WRITE_UNIT/2)) && ( cell_buf_full[1] == FALSE) )
	{
		cell_buf_full[1] = TRUE;
		KICK_WATCHDOG();
#if TRUE	// by jnlee 00.06.26
		// 재생시 UI_PLAY_REC_SIG를 UI에 보내서 UI에서 fs_read를 하도록 함
		flfreadwrite = 0;
		flf_rw_arg.handle = nvim_fs_vmemo[fn].rd_handle;
		flf_rw_arg.buffer = cell_buf_onecell[1];
		flf_rw_arg.count = FS_VOICE_WRITE_UNIT;
		flf_rw_arg.callback_function = NULL;
		SKY_Set_Signal ( &ui_tcb, UI_PLAY_REC_SIG);
#else
		fs_read( nvim_fs_vmemo[fn].rd_handle,  /* file handle */
				 cell_buf_onecell[1],         /* where to store bytes */
				 FS_VOICE_WRITE_UNIT,           /* # bytes to read */
				 NULL,                /* callback_function */
				 &rsp_msg);              /* message ptr */ 

		if( (rsp_msg.read.status == FS_OKAY_S) && 
			(rsp_msg.read.count == size) )
		{
		}
#endif

	}else if(	(cell_buf_num[1]>(FS_VOICE_WRITE_UNIT/2) && (cell_buf_full[0]==FALSE) ) ||
				(cell_buf_offset == 1)
			)
	{
		if(cell_buf_offset == 1){		// 처음 읽기를 실행한 경우 
			// 이 경우에는 UI에서 호출하는 것이므로 UI에 다시 signal을 줄 필요없음 00.06.26
			cell_buf_offset = 0;		// 처음알림 플래그 클리어 
			cell_buf_full[0]=FALSE;		//바로 읽기 시작할 것이므로, full을 표시할 필요 없음 
			KICK_WATCHDOG();
			fs_read( nvim_fs_vmemo[fn].rd_handle,  /* file handle */
					 cell_buf_onecell[0],         /* where to store bytes */
					 FS_VOICE_WRITE_UNIT,           /* # bytes to read */
					 NULL,                /* callback_function */
					 &rsp_msg);              /* message ptr */ 

			if( (rsp_msg.read.status == FS_OKAY_S) && 
				(rsp_msg.read.count == size) )
			{
			}

		}else{
			// 처음 읽는 것이 아닌 경우, signal처리하여 ui에서 읽도록 함
			cell_buf_full[0]=TRUE;
			flfreadwrite = 0;
			flf_rw_arg.handle = nvim_fs_vmemo[fn].rd_handle;
			flf_rw_arg.buffer = cell_buf_onecell[0];
			flf_rw_arg.count = FS_VOICE_WRITE_UNIT;
			flf_rw_arg.callback_function = NULL;
			SKY_Set_Signal ( &ui_tcb, UI_PLAY_REC_SIG);

		}
	}
	
	if( (cell_buf_num[cell_sel] + size) >= FS_VOICE_WRITE_UNIT)
	{
		rest_size = cell_buf_num[cell_sel] + size - FS_VOICE_WRITE_UNIT;

		for(i=0;i<size-rest_size;i++)
		{
			data[i] = cell_buf_onecell[cell_sel][cell_buf_num[cell_sel]++];
		}
		cell_buf_num[cell_sel] = 0;
		if(cell_sel == 0) cell_sel = 1, cell_buf_num[1] =0, cell_buf_full[1] = FALSE;
		else if(cell_sel == 1) cell_sel = 0, cell_buf_num[0] =0, cell_buf_full[0] = FALSE; 

		for(;i<size;i++)
		{
			data[i] = cell_buf_onecell[cell_sel][cell_buf_num[cell_sel]++];
		}
	}
	else
	{
		for(i=0 ;i<size ;i++)
			data[i] = cell_buf_onecell[cell_sel][cell_buf_num[cell_sel]++];
	}

	return 0;		// 미사용 
}


dword flfsize(byte fn)		//for EFS
{
	fs_rsp_msg_type	rsp_msg;

	fs_file_size(nvim_fs_vmemo[fn].fname,
				NULL,
				&rsp_msg);

     if (rsp_msg.file_size.status == FS_OKAY_S)
     {
        return rsp_msg.file_size.size;
	 }
	 else
		return 0;
}


boolean flferase(byte fn)		//for EFS
{
	fs_rsp_msg_type	rsp_msg;
	
	fs_test(nvim_fs_vmemo[fn].fname,    /* Name of file */
		  NULL,							/* Callback function ptr */
		  &rsp_msg);					/* Response message ptr */
	if (rsp_msg.nametest.name_found)
	{
		 /* Remove the file */
	 fs_remove(nvim_fs_vmemo[fn].fname,	/* Name of file */
			   NULL,					/* Callback function ptr */
			   &rsp_msg);				/* Response message ptr */
	}
	return TRUE;
}

/* voice녹음시 fs_reserved_write실행후의 call back function */
void fs_reserved_write_call_back_dummy(fs_rsp_msg_type *rsp_msg) 
{
}

dword flfwrite(byte fn, byte *data, dword size)		//for EFS
{
//	fs_rsp_msg_type msg_rsp;
	dword next_size=0;
	word i;

	if(size == 0)
		return 0;

	if( (cell_buf_num[cell_sel] + size) >= FS_VOICE_WRITE_UNIT)
	{
		cell_buf_full[cell_sel]= TRUE;

		next_size = cell_buf_num[cell_sel] + size - FS_VOICE_WRITE_UNIT;

		for(i=0;i<size-next_size; i++)
			cell_buf_onecell[cell_sel][cell_buf_num[cell_sel]++] = data[i];

		cell_buf_num[cell_sel] = 0;
	}
	else
	{
		for (i=0;i<size;i++)
			cell_buf_onecell[cell_sel][cell_buf_num[cell_sel]++] = data[i];
	}
		
	if(cell_buf_full[cell_sel]== TRUE)
	{
		cell_buf_full[cell_sel]= FALSE;
			dog_kick(); 
#if TRUE	// by jnlee 00.06.26
			// 녹음시 UI_PLAY_REC_SIG를 UI에 보내서 UI에서 fs_write를 하도록 함
			flfreadwrite = 1;
			flf_rw_arg.handle = nvim_fs_vmemo[fn].wr_handle;
			flf_rw_arg.buffer = cell_buf_onecell[cell_sel];
			flf_rw_arg.count = FS_VOICE_WRITE_UNIT;
//			flf_rw_arg.callback_function = fs_reserved_write_call_back_dummy;
			// UI에서 실행하므로 callback function를 굳이 줄 필요 없어짐 
			// by jnlee 00.06.26 (callback처리하면 phone die)
			flf_rw_arg.callback_function = NULL;
			SKY_Set_Signal ( &ui_tcb, UI_PLAY_REC_SIG);
#else
			fs_write( nvim_fs_vmemo[fn].wr_handle,  /* file handle */
					cell_buf_onecell[cell_sel],         /* where to store bytes */
					FS_VOICE_WRITE_UNIT,  /* # bytes to write */
					fs_reserved_write_call_back_dummy,
					&msg_rsp);         /* message ptr */ 
#endif
		cell_sel++; 
		if(cell_sel == MAX_CELL_NUM)
		{
			cell_sel = 0, cell_buf_num[0] = 0;
		}
		else
		{
			cell_buf_num[cell_sel] = 0;
		}
		for(i=0;i<next_size;i++)
		{
			cell_buf_onecell[cell_sel][cell_buf_num[cell_sel]++] = data[i+size-next_size];
		}
	}
	return 0;		// 사용 안함 
}


boolean flfclose(byte fn)		//for EFS
{
	fs_rsp_msg_type rsp_msg;        /* EFS status */

	if(nvim_fs_vmemo[fn].rd_handle)		// read mode 
	{
		fs_close(nvim_fs_vmemo[fn].rd_handle,0,&rsp_msg);                
		nvim_fs_vmemo[fn].rd_handle= FS_NULL_HANDLE;
	}
	else if(nvim_fs_vmemo[fn].wr_handle)	//write mode
	{
		if(cell_buf_num[cell_sel])
		{
			dog_kick();
			fs_write( nvim_fs_vmemo[fn].wr_handle,  /* file handle */
					cell_buf_onecell[cell_sel],         /* where to store bytes */
					cell_buf_num[cell_sel],  /* # bytes to write */
					NULL,					// synch end	-> 마지막 프레임은 싱크모드로 종료함 
					&rsp_msg);         /* message ptr */ 
		}
		fs_close(nvim_fs_vmemo[fn].wr_handle,0,&rsp_msg);                
		nvim_fs_vmemo[fn].wr_handle= FS_NULL_HANDLE;
		voice_is_recording = FALSE;			// recording is done!! by jnlee 2000.1.13
	}
	if (rsp_msg.close.status == FS_OKAY_S)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void voice_erase_all_when_nv_setup(void)
{	
	int count;
	for(count=0;count<NV_MAX_FL_FILE_NUM;count++)
	{
			flferase(count);
	}
#ifdef VOICE_RECOGNITION_FEATURE
	for(count=0;count<NV_VD_MAX_FL_FILE_NUM;count++)
	{
			vd_flferase(count);
	}
#endif
}

/* 음성 인식 관련 파일 매니저 함수들 */
#ifdef VOICE_RECOGNITION_FEATURE
byte vd_getfn(void)		//for EFS
{
	byte i;
	fs_rsp_msg_type rsp_msg;        /* EFS status */

	for(i = 0; i<NV_VD_MAX_FL_FILE_NUM;i++)
	{
		fs_test (nvim_fs_vd[i].fname,
				NULL,
				&rsp_msg
				);
		if(rsp_msg.nametest.name_found == FALSE)
		{
			return i;
		}
	}
	return NV_VD_MAX_FL_FILE_NUM;
}

		//for EFS
boolean vd_flfopen(byte fn, byte op_type) // op_type 0: read, 1: write, 2: append
{
	fs_rsp_msg_type rsp_msg;
	fs_open_xparms_type fs_open_xparms;

	if(op_type == 0)	//read
	{

		fs_open_xparms.create.cleanup_option   = FS_OC_CLOSE;
		fs_open_xparms.create.buffering_option = FS_OB_ALLOW;
		fs_open_xparms.create.attribute_mask   = FS_FA_UNRESTRICTED;

		 fs_open(nvim_fs_vd[fn].fname,	/* Name of file to open */
			 FS_OA_READONLY,                 /* Open mode */
			 NULL,            /* Optional parameters */
			 NULL,                         /* Callback function ptr */
			 &rsp_msg);               /* Response message ptr */

		if (rsp_msg.open.handle == FS_NULL_HANDLE)
		{
		   return FALSE;
		}
		nvim_fs_vd[fn].rd_handle = rsp_msg.open.handle;
		return TRUE;
	} 
	else if(op_type == 1)	//write
	{
		fs_open_xparms.create.cleanup_option   = FS_OC_DISCARD;	//all or nothing
		fs_open_xparms.create.buffering_option = FS_OB_ALLOW;
		fs_open_xparms.create.attribute_mask   = FS_FA_UNRESTRICTED;

		 fs_open(nvim_fs_vd[fn].fname,	/* Name of file to open */
			 FS_OA_CREATE,                 /* Open mode */
			 &fs_open_xparms,            /* Optional parameters */
			 NULL,                         /* Callback function ptr */
			 &rsp_msg);               /* Response message ptr */

		if (rsp_msg.open.handle == FS_NULL_HANDLE)
		{
		   return FALSE;
		}
		nvim_fs_vd[fn].wr_handle = rsp_msg.open.handle;
		return TRUE;
	}
	else	// 아직 미정인 경우 
		return TRUE;
}


dword vd_flfread(byte fn, byte *data, dword size)		//for EFS
{
	fs_rsp_msg_type rsp_msg;
	dword i,rest_size;

	if (size==0)
		return 0;
	if(nvim_fs_vd[fn].rd_handle == FS_NULL_HANDLE)
		return 0;

	fs_read( nvim_fs_vd[fn].rd_handle,  /* file handle */
			 data,				         /* where to store bytes */
			 size,			           /* # bytes to read */
			 NULL,                /* callback_function */
			 &rsp_msg);              /* message ptr */ 

	return 0;		// 미사용 
}

boolean vd_flferase(byte fn)		//for EFS
{
	fs_rsp_msg_type	rsp_msg;
	
	fs_test(nvim_fs_vd[fn].fname,    /* Name of file */
		  NULL,							/* Callback function ptr */
		  &rsp_msg);					/* Response message ptr */
	if (rsp_msg.nametest.name_found)
	{
		 /* Remove the file */
	 fs_remove(nvim_fs_vd[fn].fname,	/* Name of file */
			   NULL,					/* Callback function ptr */
			   &rsp_msg);				/* Response message ptr */
	}
	return TRUE;
}

dword vd_flfwrite(byte fn, byte *data, dword size)		//for EFS
{
	fs_rsp_msg_type msg_rsp;
	dword next_size=0;
	word i;

	if(size == 0)
		return 0;

	fs_write( nvim_fs_vd[fn].wr_handle,  /* file handle */
			data,         /* where to store bytes */
			size,  /* # bytes to write */
			NULL,
			&msg_rsp);         /* message ptr */ 

	return 0;		// 사용 안함 
}


dword vd_flfsize(byte fn)		//for EFS
{
	fs_rsp_msg_type	rsp_msg;

	fs_file_size(nvim_fs_vd[fn].fname,
				NULL,
				&rsp_msg);

     if (rsp_msg.file_size.status == FS_OKAY_S)
     {
        return rsp_msg.file_size.size;
	 }
	 else
		return 0;
}

boolean vd_flfclose(byte fn)		//for EFS
{
	fs_rsp_msg_type rsp_msg;        /* EFS status */

	if(nvim_fs_vd[fn].rd_handle)		// read mode 
	{
		fs_close(nvim_fs_vd[fn].rd_handle,0,&rsp_msg);                
		nvim_fs_vd[fn].rd_handle= FS_NULL_HANDLE;
	}
	else if(nvim_fs_vd[fn].wr_handle)	//write mode
	{
		fs_close(nvim_fs_vd[fn].wr_handle,0,&rsp_msg);                
		nvim_fs_vd[fn].wr_handle= FS_NULL_HANDLE;
		voice_is_recording = FALSE;			// recording is done!! by jnlee 2000.1.13
	}
	if (rsp_msg.close.status == FS_OKAY_S)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#endif // VOICE_RECOGNITION_FEATURE

#ifdef FEATURE_EMAIL
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// For E-mail jmpark 000313

#define NV_MAX_DATA_FILE_NUM 29
static nvim_efs_params nvim_fs_data[NV_MAX_DATA_FILE_NUM] = 
{
//save for send mail
   {"data0", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data1", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data2", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data3", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data4", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data5", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data6", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data7", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data8", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data9", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
//save for accounts
   {"data10", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data11", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
//save for uidlllists for account 1
   {"data12", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data13", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data14", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data15", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data16", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data17", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data18", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data19", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
//save for uidllists for account 2
   {"data20", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data21", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data22", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data23", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data24", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data25", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data26", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
   {"data27", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0},
//save for received mail
   {"data28", FS_FA_UNRESTRICTED, FS_NULL_HANDLE, FS_NULL_HANDLE,0}
};


//for EFS
boolean data_flfopen(byte fn, byte op_type) // op_type 0: read, 1: write, 2: append
{

	fs_rsp_msg_type rsp_msg;
	fs_open_xparms_type fs_open_xparms;

	if(op_type == 0)	//read
	{
		fs_open_xparms.create.cleanup_option   = FS_OC_CLOSE;
		fs_open_xparms.create.buffering_option = FS_OB_ALLOW;
		fs_open_xparms.create.attribute_mask   = FS_FA_UNRESTRICTED;

		 fs_open(nvim_fs_data[fn].fname,	/* Name of file to open */
			 FS_OA_READONLY,                 /* Open mode */
			 &fs_open_xparms,            /* Optional parameters */
			 NULL,                         /* Callback function ptr */
			 &rsp_msg);               /* Response message ptr */

		if (rsp_msg.open.handle == FS_NULL_HANDLE)
		{
		   ERR("Creating read error fn=%d st=%d",
			   fn, rsp_msg.open.handle, 0);
		   return FALSE;
		}
		nvim_fs_data[fn].rd_handle = rsp_msg.open.handle;
		return TRUE;
	}
	else if(op_type == 1)	//write
	{
		//**this is used in  flfwrite!!**
		//*******************************
		fs_open_xparms.create.cleanup_option   = FS_OC_DISCARD;	//all or nothing
		fs_open_xparms.create.buffering_option = FS_OB_ALLOW;
		fs_open_xparms.create.attribute_mask   = FS_FA_UNRESTRICTED;

		 fs_open(nvim_fs_data[fn].fname,	/* Name of file to open */
//			 FS_OA_READWRITE,                 /* Open mode */
             FS_OA_CREATE,
			 &fs_open_xparms,            /* Optional parameters */
			 NULL,                         /* Callback function ptr */
			 &rsp_msg);               /* Response message ptr */

		if (rsp_msg.open.handle == FS_NULL_HANDLE)
		{
		   ERR("Creating write error fn=%d st=%d",
			   fn, rsp_msg.open.handle, 0);
		   return FALSE;
		}
		nvim_fs_data[fn].wr_handle = rsp_msg.open.handle;
		return TRUE;
	}
	else
	{// 아직 미정인 경우 
		ERR("Don't know op_type %d",op_type,0,0);
		return FALSE;
	}
}


boolean data_flfclose(byte fn)		//for EFS
{
	fs_rsp_msg_type rsp_msg;        /* EFS status */

	if(nvim_fs_data[fn].rd_handle)		// read mode 
	{
		MSG_HIGH("READ MODE",0,0,0);
		fs_close(nvim_fs_data[fn].rd_handle,0,&rsp_msg);                
		nvim_fs_data[fn].rd_handle= FS_NULL_HANDLE;
	}
	else if(nvim_fs_data[fn].wr_handle)	//write mode
	{
		MSG_HIGH("WRITE MODE",0,0,0);
		fs_close(nvim_fs_data[fn].wr_handle,0,&rsp_msg);                
		nvim_fs_data[fn].wr_handle= FS_NULL_HANDLE;
	}
	if (rsp_msg.close.status == FS_OKAY_S)
	{
		MSG_HIGH("rsp_msg.close.status == FS_OKAY_S",0,0,0);
		return TRUE;
	}
	else
	{
		MSG_HIGH("rsp_msg.close.status != FS_OKAY_S",0,0,0);
		return FALSE;
	}
}


dword data_flfwrite(byte fn, byte *data, dword size)		//for EFS
{
	fs_rsp_msg_type msg_rsp;

	if( !size )
		return FALSE;

	if(nvim_fs_data[fn].wr_handle == FS_NULL_HANDLE)
	{
		ERR("FS_WRITE error %d",fn,0,0);
		return FALSE;
	}
	dog_kick(); 
	fs_write( nvim_fs_data[fn].wr_handle,  /* file handle */
			data,         /* where to store bytes */
			size,         /* # bytes to write */
			NULL,
			&msg_rsp);         /* message ptr */ 
    MSG_HIGH("FS_WRITE n:%d size:%d",fn,msg_rsp.write.count,0);
	if(msg_rsp.write.status != FS_OKAY_S ) //|| msg_rsp.write.count != size)
	{
	    ERR("Writing Error fn=%d,st=%d,size=%d",fn,msg_rsp.write.status,msg_rsp.write.count);
		return FALSE;
	}
	else{
		return TRUE;
	}
}


dword data_flfread(byte fn, byte *data, dword size)		//for EFS
{
	fs_rsp_msg_type msg_rsp;

	if (size==0)
		return FALSE;
	if(nvim_fs_data[fn].rd_handle == FS_NULL_HANDLE)
	{
		ERR("FS_READ error %d",fn,0,0);
		return FALSE;
	}

	dog_kick();
	fs_read( nvim_fs_data[fn].rd_handle,  /* file handle */
			 data,         /* where to store bytes */
			 size,           /* # bytes to read */
			 NULL,                /* callback_function */
			 &msg_rsp);              /* message ptr */ 
    MSG_HIGH("FS_READ n:%d size:%d",fn,msg_rsp.read.count,0);
	if( msg_rsp.read.status != FS_OKAY_S ) //|| msg_rsp.read.count != size)
	{
		ERR("Reading Error fn=%d,st=%d,size=%d",
			fn, msg_rsp.read.status, msg_rsp.read.count);
		return FALSE;
	}else
		return TRUE;
}

dword data_flfsize(byte fn)		//for EFS
{
	fs_rsp_msg_type	rsp_msg;

	fs_file_size(nvim_fs_data[fn].fname,
				NULL,
				&rsp_msg);

     if (rsp_msg.file_size.status == FS_OKAY_S)
     {
        return rsp_msg.file_size.size;
	 }
	 else
	 {
		ERR("Reading Size Error fn=%d,st=%d",fn,rsp_msg.file_size.status,0);
		return 0;
	 }
}


boolean data_flferase(byte fn)		//for EFS
{
	fs_rsp_msg_type	rsp_msg;
	
	fs_test(nvim_fs_data[fn].fname,    /* Name of file */
		  NULL,							/* Callback function ptr */
		  &rsp_msg);					/* Response message ptr */
	if (rsp_msg.nametest.name_found)
	{
		 /* Remove the file */
	 fs_remove(nvim_fs_data[fn].fname,	/* Name of file */
			   NULL,					/* Callback function ptr */
			   &rsp_msg);				/* Response message ptr */
	 MSG_HIGH("Remove the file %d",fn,0,0);
	 return TRUE;
	} 
	return FALSE;
}

#endif //FEATURE_EMAIL
// For E-mail End




/*****************************************************************************
function subcmd_play_rec(void)

created by jnlee 00.06.26

녹음 및 재생시 바로 flfread 및 flfwrite에서 fs_read및 fs_write를 호출하지 않고, 
ui_task로 UI_PLAY_REC_SIG를 보냄
ui_task에서 이 함수를 호출하여 fs_read,fs_write를 실행함

* 이방식으로 사용하지 않으면 인터럽트에서 바로 fs를 부르게 되어 
rex_curr_task가 sleep등 fs보다 pri가 낮은 task가 되는 경우가 발생함 
*****************************************************************************/
boolean test_ret;
void subcmd_play_rec(void)
{
	fs_rsp_msg_type rsp_msg;

	if(flfreadwrite == 0)	//	flfread에서 부른 경우 
	{
		fs_read( flf_rw_arg.handle,  /* file handle */
				 flf_rw_arg.buffer,         /* where to store bytes */
				 flf_rw_arg.count,           /* # bytes to read */
				 flf_rw_arg.callback_function,                /* callback_function */
				 &rsp_msg);              /* message ptr */ 

		if( (rsp_msg.read.status == FS_OKAY_S) && 
			(rsp_msg.read.count ==  flf_rw_arg.count ) )
		{
			test_ret = TRUE;			
		}
		else
		{
			test_ret = FALSE;			
		}
	}
	else if(flfreadwrite == 1)	// flfwrite에서 부른 경우 
	{
		fs_write( flf_rw_arg.handle,  /* file handle */
				  flf_rw_arg.buffer,         /* where to store bytes */
				  flf_rw_arg.count,  /* # bytes to write */
				  flf_rw_arg.callback_function,
				  &rsp_msg);         /* message ptr */ 

		if( (rsp_msg.write.status == FS_OKAY_S) && 
			(rsp_msg.write.count ==  flf_rw_arg.count ) )
		{
			test_ret = TRUE;			
		}
		else
		{
			test_ret = FALSE;			
		}

	}
}
