/*==========================================================================
                   E M B E D D E D   F I L E   S Y S T E M

DESCRIPTION
	이 프로그램은 Qualcom의 EFS에서 stream data(voice)를 저장을 목적으로 함
===========================================================================*/
/*
		64K SECTOR (Manager SECTOR)
	 -------------------
	|  SECTOR header	|	256 byte
	|-------------------|
	|	Manager File1	|
	|-------------------|
	|	Manager File2	|	최종 Manager file만 사용됨 
	|					|
	|-------------------|
	|FFFFFFFFFFFFFFFFFFF|
	|FFFFFFFFFFFFFFFFFFF|
	 -------------------


		64K SECTOR (DATA SECTOR)
	 -------------------
	|  SECTOR header	|	256 byte
	|-------------------|
	|					|
	|	BLOCK			|	fragment는 256byte 단위 
	|	(Fragment의		|	
	|	  배수 )		|
	|-------------------|
	|					|
	|					|
	|					|
	|					|
	 -------------------

*/
/*===========================================================================

                           EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12-23-99  jnlee    256byte 단위가 아니라 byte 단위로 write 되도록 수정 
12-2-99   jnlee    create 
===========================================================================*/

#include "customer.h"
#ifdef FEATURE_EFS
//#include "err.h"
#include "fs.h"
#include "fs_dev.h"
#include "fs_devi.h"
#include "clk.h"
#include "fs_strm.h"

/* Manager Block RAM table 
   실제는 Flash에 저장되어 있음 */
Stream_File_Table_Format stream_file_table[MAX_STREAM_ID];

/* 현재까지 읽은 데이터 포인터 */
Stream_Data_Read stream_data_read_spot = {0,0,0,0,0,0};

/* 현재 write하고 있는 데이터 정보 */
Stream_Data_Write stream_data_write_spot;

/******************************************************************		
	stream data write		
******************************************************************/
fs_stream_status fsi_intel_stream_write (byte *buffer,dword offset,dword count)
{
	if(FlashWriteRAM == NULL)
		return ( FS_STRM_FAIL_S );	
	else
		/* call RAM program */
		if((FlashWriteRAM)(buffer,offset+FS_STREAM_OFFSET,count)!=FS_OKAY_S)
			return(FS_STRM_FAIL_S);
	return(FS_STRM_OKAY_S);
}

/******************************************************************		
	stream data write		(word 데이터 쓰기)
******************************************************************/
fs_stream_status fsi_intel_stream_write_word (word input,dword offset)
{
	byte temp_buf[2];	
	if(FlashWriteRAM == NULL)
		return ( FS_STRM_FAIL_S );	
	else
	{
		temp_buf[0] = input;
		temp_buf[1] = input>>8;
		/* call RAM program */
		if((FlashWriteRAM)(temp_buf,offset+FS_STREAM_OFFSET,sizeof(word))!=FS_OKAY_S)
			return(FS_STRM_FAIL_S);
	}
	return(FS_STRM_OKAY_S);
}

/******************************************************************		
	Erase stream data area		
******************************************************************/
fs_stream_status fsi_intel_stream_erase (dword offset)
{
	if(FlashEraseRAM == NULL)
		return ( FS_STRM_FAIL_S );	
	else
		/* call RAM program */
		if((FlashEraseRAM)(offset+FS_STREAM_OFFSET) !=FS_OKAY_S)
			return(FS_STRM_FAIL_S);
	return(FS_STRM_OKAY_S);
}
/*****************************************************************/


/**********************************************************************
	fsi_intel_stream_erase_w_erase_count
description
	해당 섹터를 지우나, erase_count는 하나 증가시킴 
argument
	sector_id	: 지우려는 섹터 
**********************************************************************/
fs_stream_status fsi_intel_stream_erase_w_erase_count(dword sector_id)
{
	word erase_count;
	byte temp_buf[membersizeof(Stream_File_Sector_Header,erase_count)];
	Stream_File_Sector_Header *sector_header;

	/* 지우려는 섹터에 일단 무효화 표시를 함 -> 이번에 못지우면 다음이라도 지우도록 */
	if(	fsi_intel_stream_write_word(FS_SECTOR_INVALID,sector_id*FS_STREAM_SECTOR_SIZE) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);
	sector_header = (Stream_File_Sector_Header *)((dword)sector_id*FS_STREAM_SECTOR_SIZE+FS_READ_OFFSET);
	erase_count = sector_header->erase_count;
	erase_count++;		//하나 증가 			
	// sector erase
	if( fsi_intel_stream_erase(sector_id*FS_STREAM_SECTOR_SIZE) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);
	//erase count write -> little endian에 유의 
	temp_buf[0]=erase_count;
	temp_buf[1]=erase_count>>8;
	temp_buf[2]=erase_count>>16;
	temp_buf[3]=erase_count>>24;
	//erase count write
	if(	fsi_intel_stream_write(temp_buf,
		sector_id*FS_STREAM_SECTOR_SIZE+memberoffsetof(Stream_File_Sector_Header,erase_count),
		membersizeof(Stream_File_Sector_Header,erase_count)) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);

	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	find_manager_ptr
description
	manager섹터의 최종으로 저장된 마지막 포인터를 넘겨줌 
option
	FS_MNG_LAST		최종 
	FS_NMG_EMPTY	최종 다음 
**********************************************************************/
byte* find_manager_ptr(fs_check_manager_option option)
{
	byte *Manager_Ptr;
	word manager_size=0;
	int i;

	for(i=0;i<MAX_STREAM_SECTOR;i++)
	{
		if(	*(word *)(i*FS_STREAM_SECTOR_SIZE + FS_READ_OFFSET) == 0x0f0f )		// manager block
			break;
	}
	if(i == MAX_STREAM_SECTOR )			// 못찾았으면 ?
		return((byte *)0);						// 매니저 블럭이 없음 


	Manager_Ptr = (byte *)(i*FS_STREAM_SECTOR_SIZE + FS_READ_OFFSET + FS_REAL_STREAM_OFFSET);

	if(*(Manager_Ptr + manager_size ) == 0xff)		// 처음부터 비어 있으면..
	{
		manager_size =0;
	}
	else
	{
		// manager sector의 끝이전까지 확인 
		while((dword)Manager_Ptr <= (dword)FS_STREAM_SECTOR_SIZE+ i*FS_STREAM_SECTOR_SIZE + FS_READ_OFFSET)		
		{
			manager_size = *(word *)Manager_Ptr;	 	// 시이즈는 실제 테이블 사이즈+2byte (사이즈 정보)
			if(*(Manager_Ptr + manager_size)==0xff)		// 사이즈를 더한 이후의 포인터가 빈 공간이면
				break;									// 여기가 마지막 테이블임 
			Manager_Ptr += manager_size;				// 다음 포인터로 이동 
		}
	}

	if(option == FS_MNG_LAST)				// 유효한 마지막 포인터 
		return(Manager_Ptr-FS_READ_OFFSET);
	else if(option == FS_MNG_EMPTY)			// 빈 공간 포인터 
		return(Manager_Ptr+manager_size-FS_READ_OFFSET);
	else
		return((byte *)0xff);
}

/*************************
	Manager table 읽기 
	RAM 테이블에 저장함 
**************************/
fs_stream_status Init_Stream_Manager_Table(void)
{
	byte *Manager_Ptr;
	int i,j,k;

	Manager_Ptr = find_manager_ptr(FS_MNG_LAST);

	/******* 스트림 영역 전체 초기화!!!!! *************/
	if(Manager_Ptr == 0)		// 아직 Manager 섹터가 없으면 ..
	{
		//영역 전체 clear
		for(i=0;i<MAX_STREAM_SECTOR ;i++)
		{
			if(fsi_intel_stream_erase_w_erase_count(i)!=FS_STRM_OKAY_S)
				return(FS_STRM_FAIL_S);
		}
		// 처음 섹터를 Manager블럭으로 함 
		if(	fsi_intel_stream_write_word(FS_SECTOR_MAN_VALID,0) != FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);		
	}
	/****************************************************/

	Manager_Ptr+=2+FS_READ_OFFSET;									// size정보 다음 포인터 

	for(j=0;j<MAX_STREAM_ID;j++)
	{
		/* 파일의 정보 */
		if( (stream_file_table[j].file_number = *Manager_Ptr) == FS_END_MARK_B)
			return(FS_STRM_OKAY_S);		// all file is end
		Manager_Ptr += 1;			
		stream_file_table[j].file_sector_size = *Manager_Ptr;
		Manager_Ptr += 1;
		stream_file_table[j].file_size = *(word *)Manager_Ptr;
		Manager_Ptr += 2;
		for(k=0;k<MAX_STREAM_SECTOR;k++)
		{
			/* 파일의 블럭당 정보 */
			stream_file_table[j].block_link[k].sector = *Manager_Ptr;
			Manager_Ptr++;
			stream_file_table[j].block_link[k].offset = *Manager_Ptr;
			Manager_Ptr++;
			stream_file_table[j].block_link[k].size = *(word *)Manager_Ptr;
			Manager_Ptr += 2;
			if(stream_file_table[j].block_link[k].sector == FS_END_MARK_B)
				break;			// 1 file information end

		}
	}
	return(FS_STRM_OKAY_S);			//여기서 나가면 table 에러임 
}

/**********************************************************************
	fs_stream_check_space
	지정 섹터에서 지정한 영역의 용량 계산 (fragment=256byte 단위임 )

	option
		FS_CHECK_NOT_USE : 아직 미사용 용량 
		FS_CHECK_INVALID : 사용할 수 없는 영역 = 지우려고 마킹해놓은 영역

**********************************************************************/
byte fs_stream_check_space(byte sector_id, fs_space_check_option option)
{
	Stream_File_Sector_Header *sector_header;
	int i;
	byte result=0;

	sector_header = (Stream_File_Sector_Header *)( (sector_id * FS_STREAM_SECTOR_SIZE)
													+FS_DATA_OFFSET+FS_STREAM_OFFSET);

	if(option == FS_CHECK_NOT_USE)
	{
		if( (sector_header->fragment_status[0] & 0x0f ) == (byte)FS_FRAG_NOT_USE )		//시작 니블 
			result++;
		for(i=1;i<MAX_FRAGMENT_IN_SECTOR/2+1;i++)
		{	// 한 니블이 한 fragment를 지정함 
			if( (sector_header->fragment_status[i] & 0xf0 ) == (byte)FS_FRAG_NOT_USE<<4 )
				result++;
			if( (sector_header->fragment_status[i] & 0xf ) == (byte)FS_FRAG_NOT_USE )
				result++;
		}
	}
	else if(option == FS_CHECK_INVALID)
	{
		if(		((sector_header->fragment_status[0] & 0xf ) == (byte)FS_FRAG_INVALID)
			||	((sector_header->fragment_status[0] & 0xf ) == (byte)FS_FRAG_PROCESS))		//시작한 니블 
			result++;
		for(i=1;i<MAX_FRAGMENT_IN_SECTOR/2+1;i++)
		{	// 한 니블이 한 fragment를 지정함 
			if(		((sector_header->fragment_status[i] & 0xf0 ) == (byte)FS_FRAG_INVALID<<4) 
				||	((sector_header->fragment_status[i] & 0xf0 ) == (byte)FS_FRAG_PROCESS<<4))
				result++;
			if(		((sector_header->fragment_status[i] & 0xf ) == (byte)FS_FRAG_INVALID)
				||	((sector_header->fragment_status[i] & 0xf ) == (byte)FS_FRAG_PROCESS))
				result++;
		}
	}
	return(result);
}


/**********************************************************************
	fsi_stream_file_open

	파일 열기

argument	
	stream_number:	읽기를 할 경우에만 사용함 

	read_write: FS_OA_READONLY -> 읽기 위한 파일 열기  
				FS_OA_CREATE   -> 쓰기 위한 퍄일 열기 
**********************************************************************/
fs_stream_status fsi_stream_file_open(byte stream_number,fs_stream_access_type read_write)
{
	int i,j,blank_sector;
	word status;
	
	/***********읽기 위한 open ***************/
	if(read_write == FS_ACC_READ)		
	{
		if( stream_data_read_spot.sector ==0		&& stream_data_read_spot.offset ==0 &&
			stream_data_read_spot.file_size ==0	&& stream_data_read_spot.link_size ==0 )	
			//현재 스트림 데이터는 하나도열려 있지 않음 
		{
			//시작 link 블럭 저장 
			stream_data_read_spot.stream_number = stream_number;
			stream_data_read_spot.link_number = 0;
			stream_data_read_spot.file_size = stream_file_table[stream_number].file_size 
											|((dword)stream_file_table[stream_number].file_sector_size<<16);
			stream_data_read_spot.sector = stream_file_table[stream_number].block_link[0].sector;
			stream_data_read_spot.offset = stream_file_table[stream_number].block_link[0].offset;
			stream_data_read_spot.link_size   = stream_file_table[stream_number].block_link[0].size;
			return(FS_STRM_OKAY_S);
		}
		else
			return(FS_STRM_FAIL_S);			// 이미 열려 있음 
	}
	/***********쓰기 위한 open ***************/
	else if(read_write == FS_ACC_WRITE)		
	{
		blank_sector =0;
		for(i=0;i<MAX_STREAM_SECTOR;i++)
		{
			status = *(word*)(i*FS_STREAM_SECTOR_SIZE+FS_READ_OFFSET);
			if( status == FS_SECTOR_MAN_VALID)
			{	// manager블럭 
				stream_data_write_spot.manager_sector_space = fs_stream_check_space(i,FS_CHECK_NOT_USE);
				stream_data_write_spot.data_sector_space[i]=0;
			}
			else if( status == FS_SECTOR_NOT_USE )
			{	// 미사용 블럭 
				stream_data_write_spot.data_sector_space[i] = MAX_FRAGMENT_IN_SECTOR;	
				blank_sector++;
			}
			else	
			{	//데이터 블럭의 미사용 영역 (256바이트 단위)
				stream_data_write_spot.data_sector_space[i]= fs_stream_check_space(i,FS_CHECK_NOT_USE);
			}
		}
		if(blank_sector > 2)	//저장 가능 
		{
			for(i=0;i<MAX_STREAM_SECTOR;i++)
			{
				if(stream_data_write_spot.data_sector_space[i] !=0)
				{	// 저장 시작할 위치 잡기 
					for(j=0;j<MAX_STREAM_ID;j++){
						if(stream_file_table[j].file_number==FS_END_MARK_B){
							stream_data_write_spot.stream_number = j;
							break;
						}
					}
					stream_data_write_spot.link_number = 0;	
					stream_data_write_spot.link_size = stream_data_write_spot.data_sector_space[i];
					stream_data_write_spot.sector = i;	
					stream_data_write_spot.offset 
						= MAX_FRAGMENT_IN_SECTOR - stream_data_write_spot.link_size + 1;	//앞의 한 frag는 비운다.
					stream_data_write_spot.link_size<<=8;		// 256바이트 단위를 바이트 단위로 변경함 
					break;
				}
			}
			/* RAM상의 메니저파일에 일단 진행 사항 저장 */
//			stream_file_table[stream_data_write_spot.stream_number].file_number = stream_data_write_spot.stream_number;
			// 중간에 잘못될 것을 대비해서 일단은 파일명을 ff로 함 -> 저장완료후 번호 넣어줌 
			stream_file_table[stream_data_write_spot.stream_number].file_number = FS_END_MARK_B;
			stream_file_table[stream_data_write_spot.stream_number].file_size = 0;
			stream_file_table[stream_data_write_spot.stream_number].block_link[0].size = 0;
			stream_file_table[stream_data_write_spot.stream_number].block_link[0].sector = i;
			stream_file_table[stream_data_write_spot.stream_number].block_link[0].offset = stream_data_write_spot.offset;
			return(FS_STRM_OKAY_S);
		}
		else					//더이상 저장할 곳이 없다!!
			return(FS_STRM_FAIL_S);	
	}
	/*********조건 에러 -> 이곳으로 들어오면 안됨!!!**************/
	else			
	{
		return(FS_STRM_FAIL_S);
	}
}

/**********************************************************************
	find_min_erased_ff_sector
	가장 적게 지워진 경혐이 있는 섹터를 찾는다.

return : 찾은 섹터 번호
**********************************************************************/
int find_min_erased_ff_sector(void)
{
	int i,ret;
	dword wear_level;
	Stream_File_Sector_Header* sector_header;

	ret=-1;
	wear_level = 0xffffffff;
	for(i=0;i<MAX_STREAM_SECTOR;i++)
	{	
		sector_header = (Stream_File_Sector_Header *)(FS_STREAM_SECTOR_SIZE * i +FS_READ_OFFSET);
		if(sector_header->status  ==  FS_SECTOR_NOT_USE )
		{
			if(sector_header->erase_count < wear_level)
			{
				ret=i;			//wear level이 최소인 섹터
				wear_level = sector_header->erase_count;
			}
		}
	}
	return(ret);
}

/**********************************************************************
	fsi_strm_mng_save

	지정 어드레스에 manager file저장 
**********************************************************************/
fs_stream_status fsi_strm_mng_save(byte *Manager_Ptr)
{
	byte temp_buf[sizeof(Stream_Block_Link) * MAX_STREAM_SECTOR];
	byte *Manager_Ptr_P;
	word manager_size;
	int i,j,counter;

	Manager_Ptr_P = Manager_Ptr;		//시작 어드레스는 저장해둠 
	Manager_Ptr+=2;
	manager_size=2;						//사이즈는 2부터 시작함 
	for(i=0;i<MAX_STREAM_ID ;i++)
	{
		temp_buf[0] = stream_file_table[i].file_number;
		temp_buf[1] = stream_file_table[i].file_sector_size;
		temp_buf[2] = stream_file_table[i].file_size;
		temp_buf[3] = stream_file_table[i].file_size >> 8;
		if(	fsi_intel_stream_write(temp_buf,(dword)Manager_Ptr,4) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		Manager_Ptr +=4;
		manager_size +=4;
		if(temp_buf[0] == FS_END_MARK_B)		//end of manager file
			break;

		for(j = 0, counter =0; j < MAX_STREAM_SECTOR; j++)
		{
			temp_buf[counter++] = stream_file_table[i].block_link[j].sector;
			temp_buf[counter++] = stream_file_table[i].block_link[j].offset;
			temp_buf[counter++] = stream_file_table[i].block_link[j].size ;					
			temp_buf[counter++] = stream_file_table[i].block_link[j].size >> 8;					
			if(temp_buf[counter-4] == FS_END_MARK_B)
				break;			// end of block_link
		}
		if(	fsi_intel_stream_write(temp_buf,(dword)Manager_Ptr,counter) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		Manager_Ptr= Manager_Ptr+counter;
		manager_size +=counter;
	}
	temp_buf[0]=(byte)manager_size;
	temp_buf[1]=(byte)manager_size>>8;
	//현재 저장한 manager file의 사이즈 저장 
	if(	fsi_intel_stream_write(temp_buf,(dword)Manager_Ptr_P,2) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);

	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fsi_stream_manager_save

	manager file저장 
**********************************************************************/
fs_stream_status fsi_stream_manager_save(void)
{
	int j;
	byte *Manager_Ptr,*Manager_Ptr2;
	Stream_File_Sector_Header *sector_header;

	Manager_Ptr = find_manager_ptr(FS_MNG_EMPTY);

	if( ( (dword)Manager_Ptr + (sizeof(Stream_File_Table_Format)*MAX_STREAM_ID+2) )		// 2: size inform byte
			>= FS_STREAM_SECTOR_SIZE )	//저장할 여유가 있는가?
	{		// 여유가 없는 경우 
		// 새로 블럭 하나 만들기	

		// 가장 적게 지워진 블럭을 찾는다.  j=sector id  
		if((j = find_min_erased_ff_sector())==-1)
			return(FS_STRM_FAIL_S);

		Manager_Ptr2 = (byte *)(j*FS_STREAM_SECTOR_SIZE);
		if(fsi_strm_mng_save(Manager_Ptr2+FS_REAL_STREAM_OFFSET) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		sector_header = (Stream_File_Sector_Header *)(Manager_Ptr2);
		//새로운 manager sector로 지정함 
		if(	fsi_intel_stream_write_word(FS_SECTOR_MAN_VALID,(dword)Manager_Ptr2) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		// 기존의 manager sector는 지워버림 
		if(fsi_intel_stream_erase_w_erase_count
			( (dword)Manager_Ptr / FS_STREAM_SECTOR_SIZE * FS_STREAM_SECTOR_SIZE)!= FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

	}
	else
	{			// 여유가 있는 경우 
		if(fsi_strm_mng_save(Manager_Ptr) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
	}
	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fragment_status_write

description
	header의 fragment_status에 상태를 저장함
argument
	sector_id	: 고칠 섹터 헤더의 sector번호 
	write_addr	: 256byte단위의 fragment로서 fragment번호와 관계됨
	status		: 0xe,0xe,0등으로 새로 저장할 fragment의 상태임 
					FS_FRAG_NOT_USE,INVALID,VALID
**********************************************************************/
fs_stream_status fragment_status_write(dword sector_id, dword write_addr,word status)
{
	dword frag_addr;
	word frag;

	sector_id *= FS_STREAM_SECTOR_SIZE;		// sector시작어드레스 byte단위 
	frag_addr = sector_id + memberoffsetof(Stream_File_Sector_Header,fragment_status);
	switch(write_addr & 3)
	{
	// little endian이므로 순서에 유의 
	case 2:
		frag=0x0fff |(status<<12);	break;		//1번 니블 
	case 3:
		frag=0xf0ff |(status<<8);	break;		//2번 니블 
	case 0:
		frag=0xff0f |(status<<4);	break;		//3번 니블 
	case 1:
		frag=0xfff0 |(status);		break;		//4번 니블 
	default:
		frag=0xffff;	break;		//error경우 -> 발생 불가 
	}
	write_addr >>=2;				// 4개의 256byte단위를 하나로 묶음 
	frag_addr +=(write_addr<<1);	// 워드 단위로 저장함  

	/* 헤더의 fragment status에 write상태 정보쓰기   */
	if(	fsi_intel_stream_write((byte *)&frag, frag_addr, sizeof(frag)) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);

	return(FS_STRM_OKAY_S);

}


/**********************************************************************
	fsi_stream_file_close

	파일 닫기 
**********************************************************************/
fs_stream_status fsi_stream_file_close(fs_stream_access_type read_write)
{
	if(read_write == FS_ACC_READ)	// 읽기 위한 open file 을 clse
	{
		// 초기화 of stream 읽기 포인터 정보 
		stream_data_read_spot.sector = 0;
		stream_data_read_spot.offset = 0;
		stream_data_read_spot.file_size = 0;
		stream_data_read_spot.link_size = 0;
		return(FS_STRM_OKAY_S);
	}
	else if(read_write == FS_ACC_WRITE)	// 쓰기 위한 open file 을 close
	{
		int i,j;
		Stream_Block_Link *b_link;
		// write하고 마지막에 0xff,0xffff를 넣어서 마지막임을 표시함 
		stream_file_table[stream_data_write_spot.stream_number].file_number = stream_data_write_spot.stream_number;
		stream_file_table[stream_data_write_spot.stream_number+1].file_number = FS_END_MARK_B;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number+1].sector = FS_END_MARK_B;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number+1].offset = FS_END_MARK_B;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number+1].size = FS_END_MARK_W;

		//write한 영역에 해당 fragment를 전부 fs_frag_valid로 변환 
		for(i=0;i<=stream_data_write_spot.link_number;i++)
		{
			b_link = (Stream_Block_Link *)(&stream_file_table[stream_data_write_spot.stream_number].block_link[i]);
			if(b_link->size !=0)		// size가 0이 아닌 경우에만 실행함 
				for(j=b_link->offset; j<=( ((b_link->size-1)>>8) + b_link->offset ) ;j++)
				{
					fragment_status_write(b_link->sector,j,FS_FRAG_VALID);
				}
		}
		//매니지먼트 영역 플래쉬에 저장 
		if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

	}
	else								// 조건 에러 
		return(FS_STRM_FAIL_S);

	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fsi_stream_file_read

description
	무조건 256 byte단위로 데이터 읽어옴  
output 
	buffer: 읽은 데이터를 넣는 버퍼 
	count:	읽을 바이트 개수 
**********************************************************************/

fs_stream_status fsi_stream_file_read(byte* buffer, word count)
{
	byte st_no = stream_data_read_spot.stream_number;
	byte lnk_no = stream_data_read_spot.link_number;
	dword read_ptr;
	int i;

	//읽을 파일의 남은 사이즈 보다 크게 읽는 것은 금지한다. 
	if(count>stream_data_read_spot.file_size)
		count=stream_data_read_spot.file_size;

	if(stream_data_read_spot.file_size == 0)		/* 파일을 더이상 읽을 수 없다 */
		return(FS_STRM_EOF_S);	

	if(stream_data_read_spot.link_size == 0)		/* 현재의 블럭을 다읽었으므로 다음 블럭으로 */
	{
		lnk_no++;
		stream_data_read_spot.sector = stream_file_table[st_no].block_link[lnk_no].sector;
		stream_data_read_spot.offset = stream_file_table[st_no].block_link[lnk_no].offset;
		stream_data_read_spot.link_size   = stream_file_table[st_no].block_link[lnk_no].size;
	}

	/* 지정 블럭에서 실제 읽을 위치를 계산함 */
	/* 섹터는 64K 단위이고, 옵셋은 256byte 단위임 */
	read_ptr = (dword)stream_data_read_spot.sector<<8 | (dword)stream_data_read_spot.offset;	// sector,offset을 함께 읽음 

	read_ptr <<=8;			//256바이트단위이므로 실제 바이트 단위의 address 로 변환함 

	/* 블럭의 사이즈에서 현재까지 읽은 양의 차이는 블럭 시작에서 얼마나 현재 진행됐는지를 알려줌 */
	read_ptr += (dword)(stream_file_table[st_no].block_link[lnk_no].size - stream_data_read_spot.link_size);

	read_ptr += FS_READ_OFFSET;

	for(i=0;i<count;i++)
	{
		*buffer++ = *(byte *)read_ptr++;
	}

	stream_data_read_spot.link_size-=count ;		/* 읽은 byte 감소 */
	stream_data_read_spot.file_size-=count ;		/* 읽은 byte 감소 */
	return(FS_STRM_OKAY_S);	
}



/**********************************************************************
	fsi_stream_file_write

description
	무조건 256 byte단위로 데이터 저장함 
output 
	buffer: 저장할 데이터 버퍼 
**********************************************************************/
fs_stream_status fsi_stream_file_write(byte *buffer,word count)
{
	dword dest_addr,sector_id,temp_file_size;
	int i,j;
	word first_count,frag_count,byte_count;

	dest_addr  = (dword)stream_data_write_spot.sector<<8 | (dword)stream_data_write_spot.offset;
	dest_addr <<= 8;		// byte 단위로 변경 
	dest_addr += ( ((word)stream_data_write_spot.data_sector_space[stream_data_write_spot.sector]<<8)
						 -stream_data_write_spot.link_size);

	if(count <  (stream_data_write_spot.link_size & 0xff) )
	{
		first_count=count;
		frag_count = 0;
		byte_count = 0;
	}
	else
	{
		first_count=stream_data_write_spot.link_size & 0xff;
		//쓸량의 frag수 
		frag_count=(count-first_count)>>8;
		//쓸량의 나머지 byte수 (물론 256바이트 미만임)
		byte_count=(count-first_count)&0xff;
	}

	//이전 fragment에 연속해서 넣을 경우임 
	if(first_count)
	{
		/* WRITE on FLash Memory !!!! */
		if(	fsi_intel_stream_write(buffer, dest_addr, first_count) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		buffer+=first_count;

		/* RAM상의 매니저 파일 저장 */
		temp_file_size = stream_file_table[stream_data_write_spot.stream_number].file_size 
					  |(stream_file_table[stream_data_write_spot.stream_number].file_sector_size <<16);
		temp_file_size += first_count;
		stream_file_table[stream_data_write_spot.stream_number].file_size = temp_file_size &0xffff;
		stream_file_table[stream_data_write_spot.stream_number].file_sector_size = temp_file_size >>16;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number].size+=first_count;

		stream_data_write_spot.link_size-=first_count;			//쓴량만큼  감소

		if(stream_data_write_spot.link_size == 0)	
		{	// 다음 빈 공간 찾기 
			for(i=stream_data_write_spot.sector+1;i<MAX_STREAM_SECTOR;i++)
			{
				if(stream_data_write_spot.data_sector_space[i] !=0)
				{	// 저장 시작할 위치 잡기 
					stream_data_write_spot.link_number++;	
					stream_data_write_spot.link_size
						= stream_data_write_spot.data_sector_space[i];
					stream_data_write_spot.sector = i;	
					// 1번 fragment부터 시작함 
					stream_data_write_spot.offset 
						= MAX_FRAGMENT_IN_SECTOR - stream_data_write_spot.link_size+1;
					stream_data_write_spot.link_size<<=8;
					break;
				}
			}
			stream_file_table[stream_data_write_spot.stream_number].\
				block_link[stream_data_write_spot.link_number].\
					sector = i;

			stream_file_table[stream_data_write_spot.stream_number].\
				block_link[stream_data_write_spot.link_number].\
					offset = stream_data_write_spot.offset;

			// destination address 
			dest_addr  = (dword)stream_data_write_spot.sector<<8 | (dword)stream_data_write_spot.offset;
			dest_addr <<= 8;		// byte 단위로 변경 
			dest_addr += ( ((word)stream_data_write_spot.data_sector_space[stream_data_write_spot.sector]<<8)
								 -stream_data_write_spot.link_size);

		}
		else
			// destination address 
			dest_addr +=first_count;


	}
	// 1fragment단위로 쓰는 경우 
	for(j=0;j<frag_count;j++)
	{
		/* WRITE on FLash Memory !!!! */
		if(	fsi_intel_stream_write(buffer, dest_addr, MIN_STREAM_BLOCK_SIZE) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		buffer+=MIN_STREAM_BLOCK_SIZE;

		sector_id  = (dword)stream_data_write_spot.sector;

		/* 새로운 sector에 쓰기 시작했다면 sector status를 valid로 */
		if((dest_addr&0xffff)==FS_REAL_STREAM_OFFSET)
		{
			if(	fsi_intel_stream_write_word(FS_SECTOR_VALID, sector_id*FS_STREAM_SECTOR_SIZE) 
				!= FS_STRM_OKAY_S )
				return(FS_STRM_FAIL_S);
		}

//		dest_addr >>= 8;		// 다시 256바이트 단위로 변경 
		fragment_status_write(sector_id,(dest_addr>>8)-sector_id*(MAX_FRAGMENT_IN_SECTOR+1),FS_FRAG_PROCESS);

		/* RAM상의 매니저 파일 저장 */
		temp_file_size = stream_file_table[stream_data_write_spot.stream_number].file_size 
					  |(stream_file_table[stream_data_write_spot.stream_number].file_sector_size <<16);
		temp_file_size += MIN_STREAM_BLOCK_SIZE;
		stream_file_table[stream_data_write_spot.stream_number].file_size = temp_file_size &0xffff;
		stream_file_table[stream_data_write_spot.stream_number].file_sector_size = temp_file_size >>16;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number].size+=MIN_STREAM_BLOCK_SIZE;

		stream_data_write_spot.link_size-=MIN_STREAM_BLOCK_SIZE;			//256 byte 감소
		
		if(stream_data_write_spot.link_size == 0)	
		{	// 다음 빈 공간 찾기 
			for(i=stream_data_write_spot.sector+1;i<MAX_STREAM_SECTOR;i++)
			{
				if(stream_data_write_spot.data_sector_space[i] !=0)
				{	// 저장 시작할 위치 잡기 
					stream_data_write_spot.link_number++;	
					stream_data_write_spot.link_size
						= stream_data_write_spot.data_sector_space[i];
					stream_data_write_spot.sector = i;	
					// 1번 fragment부터 시작함 
					stream_data_write_spot.offset 
						= MAX_FRAGMENT_IN_SECTOR - stream_data_write_spot.link_size+1;
					stream_data_write_spot.link_size<<=8;
					break;
				}
			}
			stream_file_table[stream_data_write_spot.stream_number].\
				block_link[stream_data_write_spot.link_number].\
					sector = i;

			stream_file_table[stream_data_write_spot.stream_number].\
				block_link[stream_data_write_spot.link_number].\
					offset = stream_data_write_spot.offset;

			// destination address 
			dest_addr  = (dword)stream_data_write_spot.sector<<8 | (dword)stream_data_write_spot.offset;
			dest_addr <<= 8;		// byte 단위로 변경 
			dest_addr += ( ((word)stream_data_write_spot.data_sector_space[stream_data_write_spot.sector]<<8)
								 -stream_data_write_spot.link_size);
		}
		else
			// destination address 
			dest_addr += MIN_STREAM_BLOCK_SIZE;
	}

	// 새로운 fragment에 쓰는 처음 찌꺼기 
	if(byte_count)
	{
		/* WRITE on FLash Memory !!!! */
		if(	fsi_intel_stream_write(buffer, dest_addr, byte_count) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

		sector_id  = (dword)stream_data_write_spot.sector;

		/* 새로운 sector에 쓰기 시작했다면 sector status를 valid로 */
		if((dest_addr&0xffff)==FS_REAL_STREAM_OFFSET)
		{
			if(	fsi_intel_stream_write_word(FS_SECTOR_VALID, sector_id*FS_STREAM_SECTOR_SIZE) 
				!= FS_STRM_OKAY_S )
				return(FS_STRM_FAIL_S);
		}

		dest_addr >>= 8;		// 다시 256바이트 단위로 변경 

		fragment_status_write(sector_id,dest_addr-sector_id*(MAX_FRAGMENT_IN_SECTOR+1),FS_FRAG_PROCESS);

		/* RAM상의 매니저 파일 저장 */
		temp_file_size = stream_file_table[stream_data_write_spot.stream_number].file_size 
					  |(stream_file_table[stream_data_write_spot.stream_number].file_sector_size <<16);
		temp_file_size += byte_count;
		stream_file_table[stream_data_write_spot.stream_number].file_size = temp_file_size &0xffff;
		stream_file_table[stream_data_write_spot.stream_number].file_sector_size = temp_file_size >>16;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number].size+=byte_count;

		stream_data_write_spot.link_size-=byte_count;			//256 byte 감소
	}
	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fsi_stream_file_erase
description
	지정한 스트림 번호의 내용을 지움 
stream_id
	지우려는 스트림 번호 
**********************************************************************/
fs_stream_status fsi_stream_file_erase(byte stream_id)
{
	word i,j,k,m,count[MAX_STREAM_SECTOR],max_invalid=0;
	word *dest,*src;
	byte *dest2, *src2;
	Stream_File_Sector_Header *sector_header;
	word max_invalid_sector_id;
	byte temp_buf[256],temp_offset;
	int	sector_id;



	/*  header에서 지우려는 스트림의 해당fragment정보를 무효화 함 */
	for(i=0; i<MAX_STREAM_SECTOR; i++)
	{
		if(stream_file_table[stream_id].block_link[i].sector !=FS_END_MARK_B)
		{
			for(j=0;j<=((stream_file_table[stream_id].block_link[i].size-1)>>8);j++)
			{
				fragment_status_write
					(stream_file_table[stream_id].block_link[i].sector,
					 stream_file_table[stream_id].block_link[i].offset+j,
					 FS_FRAG_INVALID);
			}
		}
	}

	/* 지우는 파일의 다음부터 전부 하나씩 앞으로 당김 */
	for(i=(word)stream_id;i<MAX_STREAM_ID-1 ;i++)
	{
		dest = (word *)&stream_file_table[i];
		src  = (word *)&stream_file_table[i+1];

		// file_number,dummy,file_size이동  4byte
		for(j=0; j<2; j++)		
			*dest++ = *src++;		

		//마지막 파일인가?
		if(stream_file_table[i].file_number ==FS_END_MARK_B)		
			break;

		// link정보 이동 (sector,offset,size)
		for( ; j < (sizeof(Stream_File_Table_Format)/4);j++)	
		{
			*dest++ = *src++;
			*dest++ = *src++;
			if( (*(dest-1)==FS_END_MARK_W) && (*dest == FS_END_MARK_W) )
				break;		// 1 file 이동 완료 
		}
	}

	//일단 현재까지의 stream_file_table에 저장함 -> 아래의 gc에서 에러나더라도 재 부팅시 복구 되도록 ..
	//mamanger 파일 update
	if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);


	max_invalid =0;		// 섹터중 가장큰 invalid fragment 수  
	max_invalid_sector_id =0;		// 섹터중 가장큰 invalid fragment를 가진 섹터 

	/* 많이 지워진 섹터를 찾는다. */
	for(i=0; i<MAX_STREAM_SECTOR; i++)
	{
		sector_header = (Stream_File_Sector_Header *)
			(FS_STREAM_SECTOR_SIZE * i + FS_READ_OFFSET);
		//manager 섹터인 경우나, 미사용 섹터인 경우에는 invalid가 0
		if(sector_header->status == FS_SECTOR_MAN_VALID ||
			sector_header->status == FS_SECTOR_NOT_USE )	
			count[i]=0;
		//진행하다 죽은 섹터는  무조건 지우도록 조처함
		else if(sector_header->status == FS_SECTOR_PROCESS ||
				sector_header->status == FS_SECTOR_MAN_PROCESS ||
				sector_header->status == FS_SECTOR_INVALID	)	
		{
			// 바로 지워버리고, 섹터당 erase count를 증가함 
			if(fsi_intel_stream_erase_w_erase_count(i)!=FS_STRM_OKAY_S)
				return(FS_STRM_FAIL_S);
			count[i]=0;
		}
		//나머지의 경우에는 실제적은 invalid fragment수를 측정 
		else
		{
			count[i]=fs_stream_check_space(i,FS_CHECK_INVALID);
			if(count[i] == MAX_FRAGMENT_IN_SECTOR)
			{	// 전부 지워진 경우 바로 지워버린다. 
				if(fsi_intel_stream_erase_w_erase_count(i)!=FS_STRM_OKAY_S)
					return(FS_STRM_FAIL_S);
				// 지웠으니 이제 0입니다.
				count[i]= 0;	
			}
		}

		// invalid가 최대인 sector id 저장 
		if(max_invalid < count[i])
		{
			max_invalid = count[i];
			max_invalid_sector_id = i;
		}
	}		


	//지우려는 블럭의 내용을 다른 블럭으로 이동함
	if(max_invalid > COLLECTION_THRESHOLD)
	{
		/***************************************************************
							Garbage Collection 
		***************************************************************/
		// 이동할 블럭을 일단 작업중 표시를 해줌 
		if( (sector_id = (word)find_min_erased_ff_sector()) == 0xffff)
			return(FS_STRM_FAIL_S);
		dest2 = (byte *)(sector_id * FS_STREAM_SECTOR_SIZE);
		temp_offset = 1;		
		// 작업중 표시함 (0xfefe)
		if(	fsi_intel_stream_write_word(FS_SECTOR_PROCESS, (dword)dest2 ) 
			!= FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);
		// 실제 데이터를 저장하기 시작하는 위치 
		dest2 = dest2 + FS_REAL_STREAM_OFFSET;

		for(i=0;i<MAX_STREAM_ID;i++)
		{
			if(stream_file_table[i].file_number == FS_END_MARK_B)
				break;	// 마지막까지 검사끝 
			for(j=0;j<MAX_STREAM_SECTOR;j++)
			{
				//1 file 검사 끝 
				if(stream_file_table[i].block_link[j].sector==FS_END_MARK_B)
					break;	
				//걸렸다!!->지우는 섹터에 데이터가 있는 파일 찾았다
				else if(stream_file_table[i].block_link[j].sector == max_invalid_sector_id)
				{
					src2 = (byte *)(( max_invalid_sector_id * FS_STREAM_SECTOR_SIZE ) + FS_READ_OFFSET
							+ stream_file_table[i].block_link[j].offset * MIN_STREAM_BLOCK_SIZE);

					for(k=0;k<stream_file_table[i].block_link[j].size;k++)
					{
						for(m=0;m<MIN_STREAM_BLOCK_SIZE;m++)
							temp_buf[m] = *src2++;

						/* WRITE on FLash Memory !!!! */
						if(	fsi_intel_stream_write(temp_buf, (dword)dest2, MIN_STREAM_BLOCK_SIZE) != FS_STRM_OKAY_S)
							return(FS_STRM_FAIL_S);

						fragment_status_write(sector_id, temp_offset,FS_FRAG_VALID);
						temp_offset++;
						dest2+= MIN_STREAM_BLOCK_SIZE;
					}
					stream_file_table[i].block_link[j].sector = sector_id;
					stream_file_table[i].block_link[j].offset = 
							temp_offset - stream_file_table[i].block_link[j].size;
													
				}
			}
		}

		// 콜럭션된 섹터 작업완료 표시함  (0xf0f0)
		if(	fsi_intel_stream_write_word(FS_SECTOR_VALID, (dword)(sector_id*FS_STREAM_SECTOR_SIZE)) 
			!= FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);

		//mamanger 파일 update
		if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

		// 쓰레기 섹터 지우기 전에 invalid 표시 
		if(	fsi_intel_stream_write_word(FS_SECTOR_INVALID, (dword)(max_invalid_sector_id*FS_STREAM_SECTOR_SIZE)) 
			!= FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);

		// 기존의 섹터를 지워버림 
		if(fsi_intel_stream_erase_w_erase_count(max_invalid_sector_id)!=FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

	}
	// Garbage collection 하지 않고 그냥 새로운 file_table만 저장함 
	else
	{
		//mamanger 파일 update
		if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
	}
	return(FS_STRM_OKAY_S);
}


#endif /* FEATURE_EFS */