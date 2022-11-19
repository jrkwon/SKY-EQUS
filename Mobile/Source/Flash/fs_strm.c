/*==========================================================================
                   E M B E D D E D   F I L E   S Y S T E M

DESCRIPTION
	�� ���α׷��� Qualcom�� EFS���� stream data(voice)�� ������ �������� ��
===========================================================================*/
/*
		64K SECTOR (Manager SECTOR)
	 -------------------
	|  SECTOR header	|	256 byte
	|-------------------|
	|	Manager File1	|
	|-------------------|
	|	Manager File2	|	���� Manager file�� ���� 
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
	|	BLOCK			|	fragment�� 256byte ���� 
	|	(Fragment��		|	
	|	  ��� )		|
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
12-23-99  jnlee    256byte ������ �ƴ϶� byte ������ write �ǵ��� ���� 
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
   ������ Flash�� ����Ǿ� ���� */
Stream_File_Table_Format stream_file_table[MAX_STREAM_ID];

/* ������� ���� ������ ������ */
Stream_Data_Read stream_data_read_spot = {0,0,0,0,0,0};

/* ���� write�ϰ� �ִ� ������ ���� */
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
	stream data write		(word ������ ����)
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
	�ش� ���͸� ���쳪, erase_count�� �ϳ� ������Ŵ 
argument
	sector_id	: ������� ���� 
**********************************************************************/
fs_stream_status fsi_intel_stream_erase_w_erase_count(dword sector_id)
{
	word erase_count;
	byte temp_buf[membersizeof(Stream_File_Sector_Header,erase_count)];
	Stream_File_Sector_Header *sector_header;

	/* ������� ���Ϳ� �ϴ� ��ȿȭ ǥ�ø� �� -> �̹��� ������� �����̶� ���쵵�� */
	if(	fsi_intel_stream_write_word(FS_SECTOR_INVALID,sector_id*FS_STREAM_SECTOR_SIZE) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);
	sector_header = (Stream_File_Sector_Header *)((dword)sector_id*FS_STREAM_SECTOR_SIZE+FS_READ_OFFSET);
	erase_count = sector_header->erase_count;
	erase_count++;		//�ϳ� ���� 			
	// sector erase
	if( fsi_intel_stream_erase(sector_id*FS_STREAM_SECTOR_SIZE) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);
	//erase count write -> little endian�� ���� 
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
	manager������ �������� ����� ������ �����͸� �Ѱ��� 
option
	FS_MNG_LAST		���� 
	FS_NMG_EMPTY	���� ���� 
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
	if(i == MAX_STREAM_SECTOR )			// ��ã������ ?
		return((byte *)0);						// �Ŵ��� ���� ���� 


	Manager_Ptr = (byte *)(i*FS_STREAM_SECTOR_SIZE + FS_READ_OFFSET + FS_REAL_STREAM_OFFSET);

	if(*(Manager_Ptr + manager_size ) == 0xff)		// ó������ ��� ������..
	{
		manager_size =0;
	}
	else
	{
		// manager sector�� ���������� Ȯ�� 
		while((dword)Manager_Ptr <= (dword)FS_STREAM_SECTOR_SIZE+ i*FS_STREAM_SECTOR_SIZE + FS_READ_OFFSET)		
		{
			manager_size = *(word *)Manager_Ptr;	 	// ������� ���� ���̺� ������+2byte (������ ����)
			if(*(Manager_Ptr + manager_size)==0xff)		// ����� ���� ������ �����Ͱ� �� �����̸�
				break;									// ���Ⱑ ������ ���̺��� 
			Manager_Ptr += manager_size;				// ���� �����ͷ� �̵� 
		}
	}

	if(option == FS_MNG_LAST)				// ��ȿ�� ������ ������ 
		return(Manager_Ptr-FS_READ_OFFSET);
	else if(option == FS_MNG_EMPTY)			// �� ���� ������ 
		return(Manager_Ptr+manager_size-FS_READ_OFFSET);
	else
		return((byte *)0xff);
}

/*************************
	Manager table �б� 
	RAM ���̺� ������ 
**************************/
fs_stream_status Init_Stream_Manager_Table(void)
{
	byte *Manager_Ptr;
	int i,j,k;

	Manager_Ptr = find_manager_ptr(FS_MNG_LAST);

	/******* ��Ʈ�� ���� ��ü �ʱ�ȭ!!!!! *************/
	if(Manager_Ptr == 0)		// ���� Manager ���Ͱ� ������ ..
	{
		//���� ��ü clear
		for(i=0;i<MAX_STREAM_SECTOR ;i++)
		{
			if(fsi_intel_stream_erase_w_erase_count(i)!=FS_STRM_OKAY_S)
				return(FS_STRM_FAIL_S);
		}
		// ó�� ���͸� Manager������ �� 
		if(	fsi_intel_stream_write_word(FS_SECTOR_MAN_VALID,0) != FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);		
	}
	/****************************************************/

	Manager_Ptr+=2+FS_READ_OFFSET;									// size���� ���� ������ 

	for(j=0;j<MAX_STREAM_ID;j++)
	{
		/* ������ ���� */
		if( (stream_file_table[j].file_number = *Manager_Ptr) == FS_END_MARK_B)
			return(FS_STRM_OKAY_S);		// all file is end
		Manager_Ptr += 1;			
		stream_file_table[j].file_sector_size = *Manager_Ptr;
		Manager_Ptr += 1;
		stream_file_table[j].file_size = *(word *)Manager_Ptr;
		Manager_Ptr += 2;
		for(k=0;k<MAX_STREAM_SECTOR;k++)
		{
			/* ������ ���� ���� */
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
	return(FS_STRM_OKAY_S);			//���⼭ ������ table ������ 
}

/**********************************************************************
	fs_stream_check_space
	���� ���Ϳ��� ������ ������ �뷮 ��� (fragment=256byte ������ )

	option
		FS_CHECK_NOT_USE : ���� �̻�� �뷮 
		FS_CHECK_INVALID : ����� �� ���� ���� = ������� ��ŷ�س��� ����

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
		if( (sector_header->fragment_status[0] & 0x0f ) == (byte)FS_FRAG_NOT_USE )		//���� �Ϻ� 
			result++;
		for(i=1;i<MAX_FRAGMENT_IN_SECTOR/2+1;i++)
		{	// �� �Ϻ��� �� fragment�� ������ 
			if( (sector_header->fragment_status[i] & 0xf0 ) == (byte)FS_FRAG_NOT_USE<<4 )
				result++;
			if( (sector_header->fragment_status[i] & 0xf ) == (byte)FS_FRAG_NOT_USE )
				result++;
		}
	}
	else if(option == FS_CHECK_INVALID)
	{
		if(		((sector_header->fragment_status[0] & 0xf ) == (byte)FS_FRAG_INVALID)
			||	((sector_header->fragment_status[0] & 0xf ) == (byte)FS_FRAG_PROCESS))		//������ �Ϻ� 
			result++;
		for(i=1;i<MAX_FRAGMENT_IN_SECTOR/2+1;i++)
		{	// �� �Ϻ��� �� fragment�� ������ 
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

	���� ����

argument	
	stream_number:	�б⸦ �� ��쿡�� ����� 

	read_write: FS_OA_READONLY -> �б� ���� ���� ����  
				FS_OA_CREATE   -> ���� ���� ���� ���� 
**********************************************************************/
fs_stream_status fsi_stream_file_open(byte stream_number,fs_stream_access_type read_write)
{
	int i,j,blank_sector;
	word status;
	
	/***********�б� ���� open ***************/
	if(read_write == FS_ACC_READ)		
	{
		if( stream_data_read_spot.sector ==0		&& stream_data_read_spot.offset ==0 &&
			stream_data_read_spot.file_size ==0	&& stream_data_read_spot.link_size ==0 )	
			//���� ��Ʈ�� �����ʹ� �ϳ������� ���� ���� 
		{
			//���� link �� ���� 
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
			return(FS_STRM_FAIL_S);			// �̹� ���� ���� 
	}
	/***********���� ���� open ***************/
	else if(read_write == FS_ACC_WRITE)		
	{
		blank_sector =0;
		for(i=0;i<MAX_STREAM_SECTOR;i++)
		{
			status = *(word*)(i*FS_STREAM_SECTOR_SIZE+FS_READ_OFFSET);
			if( status == FS_SECTOR_MAN_VALID)
			{	// manager�� 
				stream_data_write_spot.manager_sector_space = fs_stream_check_space(i,FS_CHECK_NOT_USE);
				stream_data_write_spot.data_sector_space[i]=0;
			}
			else if( status == FS_SECTOR_NOT_USE )
			{	// �̻�� �� 
				stream_data_write_spot.data_sector_space[i] = MAX_FRAGMENT_IN_SECTOR;	
				blank_sector++;
			}
			else	
			{	//������ ���� �̻�� ���� (256����Ʈ ����)
				stream_data_write_spot.data_sector_space[i]= fs_stream_check_space(i,FS_CHECK_NOT_USE);
			}
		}
		if(blank_sector > 2)	//���� ���� 
		{
			for(i=0;i<MAX_STREAM_SECTOR;i++)
			{
				if(stream_data_write_spot.data_sector_space[i] !=0)
				{	// ���� ������ ��ġ ��� 
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
						= MAX_FRAGMENT_IN_SECTOR - stream_data_write_spot.link_size + 1;	//���� �� frag�� ����.
					stream_data_write_spot.link_size<<=8;		// 256����Ʈ ������ ����Ʈ ������ ������ 
					break;
				}
			}
			/* RAM���� �޴������Ͽ� �ϴ� ���� ���� ���� */
//			stream_file_table[stream_data_write_spot.stream_number].file_number = stream_data_write_spot.stream_number;
			// �߰��� �߸��� ���� ����ؼ� �ϴ��� ���ϸ��� ff�� �� -> ����Ϸ��� ��ȣ �־��� 
			stream_file_table[stream_data_write_spot.stream_number].file_number = FS_END_MARK_B;
			stream_file_table[stream_data_write_spot.stream_number].file_size = 0;
			stream_file_table[stream_data_write_spot.stream_number].block_link[0].size = 0;
			stream_file_table[stream_data_write_spot.stream_number].block_link[0].sector = i;
			stream_file_table[stream_data_write_spot.stream_number].block_link[0].offset = stream_data_write_spot.offset;
			return(FS_STRM_OKAY_S);
		}
		else					//���̻� ������ ���� ����!!
			return(FS_STRM_FAIL_S);	
	}
	/*********���� ���� -> �̰����� ������ �ȵ�!!!**************/
	else			
	{
		return(FS_STRM_FAIL_S);
	}
}

/**********************************************************************
	find_min_erased_ff_sector
	���� ���� ������ ������ �ִ� ���͸� ã�´�.

return : ã�� ���� ��ȣ
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
				ret=i;			//wear level�� �ּ��� ����
				wear_level = sector_header->erase_count;
			}
		}
	}
	return(ret);
}

/**********************************************************************
	fsi_strm_mng_save

	���� ��巹���� manager file���� 
**********************************************************************/
fs_stream_status fsi_strm_mng_save(byte *Manager_Ptr)
{
	byte temp_buf[sizeof(Stream_Block_Link) * MAX_STREAM_SECTOR];
	byte *Manager_Ptr_P;
	word manager_size;
	int i,j,counter;

	Manager_Ptr_P = Manager_Ptr;		//���� ��巹���� �����ص� 
	Manager_Ptr+=2;
	manager_size=2;						//������� 2���� ������ 
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
	//���� ������ manager file�� ������ ���� 
	if(	fsi_intel_stream_write(temp_buf,(dword)Manager_Ptr_P,2) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);

	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fsi_stream_manager_save

	manager file���� 
**********************************************************************/
fs_stream_status fsi_stream_manager_save(void)
{
	int j;
	byte *Manager_Ptr,*Manager_Ptr2;
	Stream_File_Sector_Header *sector_header;

	Manager_Ptr = find_manager_ptr(FS_MNG_EMPTY);

	if( ( (dword)Manager_Ptr + (sizeof(Stream_File_Table_Format)*MAX_STREAM_ID+2) )		// 2: size inform byte
			>= FS_STREAM_SECTOR_SIZE )	//������ ������ �ִ°�?
	{		// ������ ���� ��� 
		// ���� �� �ϳ� �����	

		// ���� ���� ������ ���� ã�´�.  j=sector id  
		if((j = find_min_erased_ff_sector())==-1)
			return(FS_STRM_FAIL_S);

		Manager_Ptr2 = (byte *)(j*FS_STREAM_SECTOR_SIZE);
		if(fsi_strm_mng_save(Manager_Ptr2+FS_REAL_STREAM_OFFSET) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		sector_header = (Stream_File_Sector_Header *)(Manager_Ptr2);
		//���ο� manager sector�� ������ 
		if(	fsi_intel_stream_write_word(FS_SECTOR_MAN_VALID,(dword)Manager_Ptr2) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		// ������ manager sector�� �������� 
		if(fsi_intel_stream_erase_w_erase_count
			( (dword)Manager_Ptr / FS_STREAM_SECTOR_SIZE * FS_STREAM_SECTOR_SIZE)!= FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

	}
	else
	{			// ������ �ִ� ��� 
		if(fsi_strm_mng_save(Manager_Ptr) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
	}
	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fragment_status_write

description
	header�� fragment_status�� ���¸� ������
argument
	sector_id	: ��ĥ ���� ����� sector��ȣ 
	write_addr	: 256byte������ fragment�μ� fragment��ȣ�� �����
	status		: 0xe,0xe,0������ ���� ������ fragment�� ������ 
					FS_FRAG_NOT_USE,INVALID,VALID
**********************************************************************/
fs_stream_status fragment_status_write(dword sector_id, dword write_addr,word status)
{
	dword frag_addr;
	word frag;

	sector_id *= FS_STREAM_SECTOR_SIZE;		// sector���۾�巹�� byte���� 
	frag_addr = sector_id + memberoffsetof(Stream_File_Sector_Header,fragment_status);
	switch(write_addr & 3)
	{
	// little endian�̹Ƿ� ������ ���� 
	case 2:
		frag=0x0fff |(status<<12);	break;		//1�� �Ϻ� 
	case 3:
		frag=0xf0ff |(status<<8);	break;		//2�� �Ϻ� 
	case 0:
		frag=0xff0f |(status<<4);	break;		//3�� �Ϻ� 
	case 1:
		frag=0xfff0 |(status);		break;		//4�� �Ϻ� 
	default:
		frag=0xffff;	break;		//error��� -> �߻� �Ұ� 
	}
	write_addr >>=2;				// 4���� 256byte������ �ϳ��� ���� 
	frag_addr +=(write_addr<<1);	// ���� ������ ������  

	/* ����� fragment status�� write���� ��������   */
	if(	fsi_intel_stream_write((byte *)&frag, frag_addr, sizeof(frag)) != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);

	return(FS_STRM_OKAY_S);

}


/**********************************************************************
	fsi_stream_file_close

	���� �ݱ� 
**********************************************************************/
fs_stream_status fsi_stream_file_close(fs_stream_access_type read_write)
{
	if(read_write == FS_ACC_READ)	// �б� ���� open file �� clse
	{
		// �ʱ�ȭ of stream �б� ������ ���� 
		stream_data_read_spot.sector = 0;
		stream_data_read_spot.offset = 0;
		stream_data_read_spot.file_size = 0;
		stream_data_read_spot.link_size = 0;
		return(FS_STRM_OKAY_S);
	}
	else if(read_write == FS_ACC_WRITE)	// ���� ���� open file �� close
	{
		int i,j;
		Stream_Block_Link *b_link;
		// write�ϰ� �������� 0xff,0xffff�� �־ ���������� ǥ���� 
		stream_file_table[stream_data_write_spot.stream_number].file_number = stream_data_write_spot.stream_number;
		stream_file_table[stream_data_write_spot.stream_number+1].file_number = FS_END_MARK_B;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number+1].sector = FS_END_MARK_B;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number+1].offset = FS_END_MARK_B;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number+1].size = FS_END_MARK_W;

		//write�� ������ �ش� fragment�� ���� fs_frag_valid�� ��ȯ 
		for(i=0;i<=stream_data_write_spot.link_number;i++)
		{
			b_link = (Stream_Block_Link *)(&stream_file_table[stream_data_write_spot.stream_number].block_link[i]);
			if(b_link->size !=0)		// size�� 0�� �ƴ� ��쿡�� ������ 
				for(j=b_link->offset; j<=( ((b_link->size-1)>>8) + b_link->offset ) ;j++)
				{
					fragment_status_write(b_link->sector,j,FS_FRAG_VALID);
				}
		}
		//�Ŵ�����Ʈ ���� �÷����� ���� 
		if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

	}
	else								// ���� ���� 
		return(FS_STRM_FAIL_S);

	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fsi_stream_file_read

description
	������ 256 byte������ ������ �о��  
output 
	buffer: ���� �����͸� �ִ� ���� 
	count:	���� ����Ʈ ���� 
**********************************************************************/

fs_stream_status fsi_stream_file_read(byte* buffer, word count)
{
	byte st_no = stream_data_read_spot.stream_number;
	byte lnk_no = stream_data_read_spot.link_number;
	dword read_ptr;
	int i;

	//���� ������ ���� ������ ���� ũ�� �д� ���� �����Ѵ�. 
	if(count>stream_data_read_spot.file_size)
		count=stream_data_read_spot.file_size;

	if(stream_data_read_spot.file_size == 0)		/* ������ ���̻� ���� �� ���� */
		return(FS_STRM_EOF_S);	

	if(stream_data_read_spot.link_size == 0)		/* ������ ���� ���о����Ƿ� ���� ������ */
	{
		lnk_no++;
		stream_data_read_spot.sector = stream_file_table[st_no].block_link[lnk_no].sector;
		stream_data_read_spot.offset = stream_file_table[st_no].block_link[lnk_no].offset;
		stream_data_read_spot.link_size   = stream_file_table[st_no].block_link[lnk_no].size;
	}

	/* ���� ������ ���� ���� ��ġ�� ����� */
	/* ���ʹ� 64K �����̰�, �ɼ��� 256byte ������ */
	read_ptr = (dword)stream_data_read_spot.sector<<8 | (dword)stream_data_read_spot.offset;	// sector,offset�� �Բ� ���� 

	read_ptr <<=8;			//256����Ʈ�����̹Ƿ� ���� ����Ʈ ������ address �� ��ȯ�� 

	/* ���� ������� ������� ���� ���� ���̴� �� ���ۿ��� �󸶳� ���� ����ƴ����� �˷��� */
	read_ptr += (dword)(stream_file_table[st_no].block_link[lnk_no].size - stream_data_read_spot.link_size);

	read_ptr += FS_READ_OFFSET;

	for(i=0;i<count;i++)
	{
		*buffer++ = *(byte *)read_ptr++;
	}

	stream_data_read_spot.link_size-=count ;		/* ���� byte ���� */
	stream_data_read_spot.file_size-=count ;		/* ���� byte ���� */
	return(FS_STRM_OKAY_S);	
}



/**********************************************************************
	fsi_stream_file_write

description
	������ 256 byte������ ������ ������ 
output 
	buffer: ������ ������ ���� 
**********************************************************************/
fs_stream_status fsi_stream_file_write(byte *buffer,word count)
{
	dword dest_addr,sector_id,temp_file_size;
	int i,j;
	word first_count,frag_count,byte_count;

	dest_addr  = (dword)stream_data_write_spot.sector<<8 | (dword)stream_data_write_spot.offset;
	dest_addr <<= 8;		// byte ������ ���� 
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
		//������ frag�� 
		frag_count=(count-first_count)>>8;
		//������ ������ byte�� (���� 256����Ʈ �̸���)
		byte_count=(count-first_count)&0xff;
	}

	//���� fragment�� �����ؼ� ���� ����� 
	if(first_count)
	{
		/* WRITE on FLash Memory !!!! */
		if(	fsi_intel_stream_write(buffer, dest_addr, first_count) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		buffer+=first_count;

		/* RAM���� �Ŵ��� ���� ���� */
		temp_file_size = stream_file_table[stream_data_write_spot.stream_number].file_size 
					  |(stream_file_table[stream_data_write_spot.stream_number].file_sector_size <<16);
		temp_file_size += first_count;
		stream_file_table[stream_data_write_spot.stream_number].file_size = temp_file_size &0xffff;
		stream_file_table[stream_data_write_spot.stream_number].file_sector_size = temp_file_size >>16;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number].size+=first_count;

		stream_data_write_spot.link_size-=first_count;			//������ŭ  ����

		if(stream_data_write_spot.link_size == 0)	
		{	// ���� �� ���� ã�� 
			for(i=stream_data_write_spot.sector+1;i<MAX_STREAM_SECTOR;i++)
			{
				if(stream_data_write_spot.data_sector_space[i] !=0)
				{	// ���� ������ ��ġ ��� 
					stream_data_write_spot.link_number++;	
					stream_data_write_spot.link_size
						= stream_data_write_spot.data_sector_space[i];
					stream_data_write_spot.sector = i;	
					// 1�� fragment���� ������ 
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
			dest_addr <<= 8;		// byte ������ ���� 
			dest_addr += ( ((word)stream_data_write_spot.data_sector_space[stream_data_write_spot.sector]<<8)
								 -stream_data_write_spot.link_size);

		}
		else
			// destination address 
			dest_addr +=first_count;


	}
	// 1fragment������ ���� ��� 
	for(j=0;j<frag_count;j++)
	{
		/* WRITE on FLash Memory !!!! */
		if(	fsi_intel_stream_write(buffer, dest_addr, MIN_STREAM_BLOCK_SIZE) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
		buffer+=MIN_STREAM_BLOCK_SIZE;

		sector_id  = (dword)stream_data_write_spot.sector;

		/* ���ο� sector�� ���� �����ߴٸ� sector status�� valid�� */
		if((dest_addr&0xffff)==FS_REAL_STREAM_OFFSET)
		{
			if(	fsi_intel_stream_write_word(FS_SECTOR_VALID, sector_id*FS_STREAM_SECTOR_SIZE) 
				!= FS_STRM_OKAY_S )
				return(FS_STRM_FAIL_S);
		}

//		dest_addr >>= 8;		// �ٽ� 256����Ʈ ������ ���� 
		fragment_status_write(sector_id,(dest_addr>>8)-sector_id*(MAX_FRAGMENT_IN_SECTOR+1),FS_FRAG_PROCESS);

		/* RAM���� �Ŵ��� ���� ���� */
		temp_file_size = stream_file_table[stream_data_write_spot.stream_number].file_size 
					  |(stream_file_table[stream_data_write_spot.stream_number].file_sector_size <<16);
		temp_file_size += MIN_STREAM_BLOCK_SIZE;
		stream_file_table[stream_data_write_spot.stream_number].file_size = temp_file_size &0xffff;
		stream_file_table[stream_data_write_spot.stream_number].file_sector_size = temp_file_size >>16;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number].size+=MIN_STREAM_BLOCK_SIZE;

		stream_data_write_spot.link_size-=MIN_STREAM_BLOCK_SIZE;			//256 byte ����
		
		if(stream_data_write_spot.link_size == 0)	
		{	// ���� �� ���� ã�� 
			for(i=stream_data_write_spot.sector+1;i<MAX_STREAM_SECTOR;i++)
			{
				if(stream_data_write_spot.data_sector_space[i] !=0)
				{	// ���� ������ ��ġ ��� 
					stream_data_write_spot.link_number++;	
					stream_data_write_spot.link_size
						= stream_data_write_spot.data_sector_space[i];
					stream_data_write_spot.sector = i;	
					// 1�� fragment���� ������ 
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
			dest_addr <<= 8;		// byte ������ ���� 
			dest_addr += ( ((word)stream_data_write_spot.data_sector_space[stream_data_write_spot.sector]<<8)
								 -stream_data_write_spot.link_size);
		}
		else
			// destination address 
			dest_addr += MIN_STREAM_BLOCK_SIZE;
	}

	// ���ο� fragment�� ���� ó�� ��� 
	if(byte_count)
	{
		/* WRITE on FLash Memory !!!! */
		if(	fsi_intel_stream_write(buffer, dest_addr, byte_count) != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

		sector_id  = (dword)stream_data_write_spot.sector;

		/* ���ο� sector�� ���� �����ߴٸ� sector status�� valid�� */
		if((dest_addr&0xffff)==FS_REAL_STREAM_OFFSET)
		{
			if(	fsi_intel_stream_write_word(FS_SECTOR_VALID, sector_id*FS_STREAM_SECTOR_SIZE) 
				!= FS_STRM_OKAY_S )
				return(FS_STRM_FAIL_S);
		}

		dest_addr >>= 8;		// �ٽ� 256����Ʈ ������ ���� 

		fragment_status_write(sector_id,dest_addr-sector_id*(MAX_FRAGMENT_IN_SECTOR+1),FS_FRAG_PROCESS);

		/* RAM���� �Ŵ��� ���� ���� */
		temp_file_size = stream_file_table[stream_data_write_spot.stream_number].file_size 
					  |(stream_file_table[stream_data_write_spot.stream_number].file_sector_size <<16);
		temp_file_size += byte_count;
		stream_file_table[stream_data_write_spot.stream_number].file_size = temp_file_size &0xffff;
		stream_file_table[stream_data_write_spot.stream_number].file_sector_size = temp_file_size >>16;
		stream_file_table[stream_data_write_spot.stream_number].\
			block_link[stream_data_write_spot.link_number].size+=byte_count;

		stream_data_write_spot.link_size-=byte_count;			//256 byte ����
	}
	return(FS_STRM_OKAY_S);
}


/**********************************************************************
	fsi_stream_file_erase
description
	������ ��Ʈ�� ��ȣ�� ������ ���� 
stream_id
	������� ��Ʈ�� ��ȣ 
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



	/*  header���� ������� ��Ʈ���� �ش�fragment������ ��ȿȭ �� */
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

	/* ����� ������ �������� ���� �ϳ��� ������ ��� */
	for(i=(word)stream_id;i<MAX_STREAM_ID-1 ;i++)
	{
		dest = (word *)&stream_file_table[i];
		src  = (word *)&stream_file_table[i+1];

		// file_number,dummy,file_size�̵�  4byte
		for(j=0; j<2; j++)		
			*dest++ = *src++;		

		//������ �����ΰ�?
		if(stream_file_table[i].file_number ==FS_END_MARK_B)		
			break;

		// link���� �̵� (sector,offset,size)
		for( ; j < (sizeof(Stream_File_Table_Format)/4);j++)	
		{
			*dest++ = *src++;
			*dest++ = *src++;
			if( (*(dest-1)==FS_END_MARK_W) && (*dest == FS_END_MARK_W) )
				break;		// 1 file �̵� �Ϸ� 
		}
	}

	//�ϴ� ��������� stream_file_table�� ������ -> �Ʒ��� gc���� ���������� �� ���ý� ���� �ǵ��� ..
	//mamanger ���� update
	if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
		return(FS_STRM_FAIL_S);


	max_invalid =0;		// ������ ����ū invalid fragment ��  
	max_invalid_sector_id =0;		// ������ ����ū invalid fragment�� ���� ���� 

	/* ���� ������ ���͸� ã�´�. */
	for(i=0; i<MAX_STREAM_SECTOR; i++)
	{
		sector_header = (Stream_File_Sector_Header *)
			(FS_STREAM_SECTOR_SIZE * i + FS_READ_OFFSET);
		//manager ������ ��쳪, �̻�� ������ ��쿡�� invalid�� 0
		if(sector_header->status == FS_SECTOR_MAN_VALID ||
			sector_header->status == FS_SECTOR_NOT_USE )	
			count[i]=0;
		//�����ϴ� ���� ���ʹ�  ������ ���쵵�� ��ó��
		else if(sector_header->status == FS_SECTOR_PROCESS ||
				sector_header->status == FS_SECTOR_MAN_PROCESS ||
				sector_header->status == FS_SECTOR_INVALID	)	
		{
			// �ٷ� ����������, ���ʹ� erase count�� ������ 
			if(fsi_intel_stream_erase_w_erase_count(i)!=FS_STRM_OKAY_S)
				return(FS_STRM_FAIL_S);
			count[i]=0;
		}
		//�������� ��쿡�� �������� invalid fragment���� ���� 
		else
		{
			count[i]=fs_stream_check_space(i,FS_CHECK_INVALID);
			if(count[i] == MAX_FRAGMENT_IN_SECTOR)
			{	// ���� ������ ��� �ٷ� ����������. 
				if(fsi_intel_stream_erase_w_erase_count(i)!=FS_STRM_OKAY_S)
					return(FS_STRM_FAIL_S);
				// �������� ���� 0�Դϴ�.
				count[i]= 0;	
			}
		}

		// invalid�� �ִ��� sector id ���� 
		if(max_invalid < count[i])
		{
			max_invalid = count[i];
			max_invalid_sector_id = i;
		}
	}		


	//������� ���� ������ �ٸ� ������ �̵���
	if(max_invalid > COLLECTION_THRESHOLD)
	{
		/***************************************************************
							Garbage Collection 
		***************************************************************/
		// �̵��� ���� �ϴ� �۾��� ǥ�ø� ���� 
		if( (sector_id = (word)find_min_erased_ff_sector()) == 0xffff)
			return(FS_STRM_FAIL_S);
		dest2 = (byte *)(sector_id * FS_STREAM_SECTOR_SIZE);
		temp_offset = 1;		
		// �۾��� ǥ���� (0xfefe)
		if(	fsi_intel_stream_write_word(FS_SECTOR_PROCESS, (dword)dest2 ) 
			!= FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);
		// ���� �����͸� �����ϱ� �����ϴ� ��ġ 
		dest2 = dest2 + FS_REAL_STREAM_OFFSET;

		for(i=0;i<MAX_STREAM_ID;i++)
		{
			if(stream_file_table[i].file_number == FS_END_MARK_B)
				break;	// ���������� �˻糡 
			for(j=0;j<MAX_STREAM_SECTOR;j++)
			{
				//1 file �˻� �� 
				if(stream_file_table[i].block_link[j].sector==FS_END_MARK_B)
					break;	
				//�ɷȴ�!!->����� ���Ϳ� �����Ͱ� �ִ� ���� ã�Ҵ�
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

		// �ݷ��ǵ� ���� �۾��Ϸ� ǥ����  (0xf0f0)
		if(	fsi_intel_stream_write_word(FS_SECTOR_VALID, (dword)(sector_id*FS_STREAM_SECTOR_SIZE)) 
			!= FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);

		//mamanger ���� update
		if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

		// ������ ���� ����� ���� invalid ǥ�� 
		if(	fsi_intel_stream_write_word(FS_SECTOR_INVALID, (dword)(max_invalid_sector_id*FS_STREAM_SECTOR_SIZE)) 
			!= FS_STRM_OKAY_S )
			return(FS_STRM_FAIL_S);

		// ������ ���͸� �������� 
		if(fsi_intel_stream_erase_w_erase_count(max_invalid_sector_id)!=FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);

	}
	// Garbage collection ���� �ʰ� �׳� ���ο� file_table�� ������ 
	else
	{
		//mamanger ���� update
		if(fsi_stream_manager_save() != FS_STRM_OKAY_S)
			return(FS_STRM_FAIL_S);
	}
	return(FS_STRM_OKAY_S);
}


#endif /* FEATURE_EFS */