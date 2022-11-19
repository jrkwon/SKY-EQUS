/*==========================================================================
                   E M B E D D E D   F I L E   S Y S T E M
===========================================================================*/

/*==========================================================================
when       who     what, where, why
--------   ---     ----------------------------------------------------------
12-23-99  jnlee    256byte ������ �ƴ϶� byte ������ write �ǵ��� ���� 
12-2-99   jnlee    create 
===========================================================================*/

/* ������ ����� FLASH MEMORY�� Stream Data���� ���� offset*/
#define FS_STREAM_OFFSET	0x60000		

/* ������ ����� FLASH MEMORY ���� offset
	read�� ��쿡�� �����				
	write�� ��쿡�� fsi_intel_write���� �ڵ������� �ذ��� */
#define FS_DATA_OFFSET		0x300000		

/* ���������� ������ ����ϴ� offset */
#define FS_READ_OFFSET	(FS_STREAM_OFFSET+FS_DATA_OFFSET)

/* flash memory �� ���� ������ */
#define FS_STREAM_SECTOR_SIZE 	0x10000		

/* Stream Data���� �κ��� sector��  */
#define MAX_STREAM_SECTOR	10		
/* �ִ� 30���� stream �����͸� ������ */	
#define MAX_STREAM_NUM		30		// �������� 
#define MAX_STREAM_ID		(MAX_STREAM_NUM+1)	// ���������� �ϳ� �� ����. �������� ff
/* ���� ����� ������ ���� ������ offset byte */
#define FS_REAL_STREAM_OFFSET		256
/* �� ������ �ּ� ���� fragment size*/
#define MIN_STREAM_BLOCK_SIZE		256
/* 1 sector ���� �ִ� gragment�� */
#define	MAX_FRAGMENT_IN_SECTOR		255
/* 1 sector���� invalid�����Ͱ� ���� �̻��̸� �� ���͸� ������ */
#define	COLLECTION_THRESHOLD		128

/* ����ü�� offset ��� */
#define  memberoffsetof(type, member)  ((dword)&((type *)0)->member)
/* ����ü�� ��� ������ ��� */
#define  membersizeof(type, member) (sizeof(((type *)0)->member))

/* Stream Data File Table ����ü */
typedef struct {
	byte sector;			//sector number
	byte offset;			//offset (base 256bytes)
	word size;				//size(256bytes) of this block ->���� byte ������ ���� 0~64K
}Stream_Block_Link;

typedef struct {
	byte file_number;
	byte file_sector_size;		// 64k������ ������ 
	word file_size;			// size (256 byte����) -> byte ������ ����
	Stream_Block_Link block_link[MAX_STREAM_SECTOR];
	// 4byte ������ align�ǵ��� ������ ��
}Stream_File_Table_Format;

/****************************************************************
 Manager Block RAM table : ������ Flash�� ����Ǿ� ���� 
*****************************************************************/
extern Stream_File_Table_Format stream_file_table[MAX_STREAM_ID];


/* flash Memory Sector header */
typedef struct {
	// ffff:�̻�� , fefe:������ , f0f0;��밡��, efef:�Ŵ��� ������ 0f0f:�Ŵ�����
	word	status;			
	// ������ sector id
	word	sector_id;		
	//������� ������ Ƚ�� -> wear leveling�� ���� 
	dword	erase_count;	
	//nibble ������ 256byte���� ���¸� ������ -> f:�̻�� , e:writing�� c:vaild data   0:invalid data
	byte	fragment_status[MAX_FRAGMENT_IN_SECTOR/2 +1]; 	
}Stream_File_Sector_Header;

#define  FS_SECTOR_NOT_USE	((word)0xffff)			/* ���� �����Ͱ� ����ȵ� sector */
#define  FS_SECTOR_PROCESS	((word)0xfefe)			/* ���� �����͸� ���� �ִ� ��  */
#define  FS_SECTOR_VALID	((word)0xf0f0)			/* ���� ��ȿ�� �����͸� ���������� */
#define  FS_SECTOR_MAN_PROCESS	((word)0xefef)		/* ���� �Ŵ�����Ʈ �����͸� ���� �ִ� ��  */
#define  FS_SECTOR_MAN_VALID	((word)0x0f0f)		/* ���� ��ȿ�� �Ŵ�����Ʈ �����͸� ���������� */
#define  FS_SECTOR_INVALID		((word)0)				/* ���� ��ȿ�� �����͸� ���������� */


#define  FS_FRAG_NOT_USE	0x0f		/* ���� �����Ͱ� ����ȵ� fragment */
#define  FS_FRAG_PROCESS	0x0e		/* ���� �����͸� ���� �ִ� ��  */
#define  FS_FRAG_VALID		0x0c		/* ���� ��ȿ�� �����͸� ���������� */
#define  FS_FRAG_INVALID	0			/* ���Ұ����� fragment -> ������ ��  */

#define	 FS_END_MARK_B		0xff		/* byte end mark */
#define	 FS_END_MARK_W		0xffff		/* word end mark */



/* ������� ���� ������ ������ */
typedef struct{
	dword file_size;			//��ü ������-������� ���� ������ 
	byte stream_number;			//stream number
	byte link_number;			//link ��ȣ 
	byte sector;				//
	byte offset;
	word link_size;				// �� ������ - ������� ���� ������ 
}Stream_Data_Read;

/* ���� write�ϰ� �ִ� ������ ���� */
typedef struct{
	byte stream_number;
	byte link_number;			
	byte sector;				//
	byte offset;
	word link_size;				//������� ������ ������(256byte ����)->byte������ ���� 
/* ���� �� ���� �� ��� �ִ� ���� */
	byte data_sector_space[MAX_STREAM_SECTOR];
	byte manager_sector_space;
}Stream_Data_Write;


typedef enum {
  FS_CHECK_NOT_USE,            /* not yet used                                */
  FS_CHECK_INVALID             /* Reserved for file system testing              */
} fs_space_check_option;

typedef enum {
  FS_MNG_LAST,				/* ������ Manager file ã�� */
  FS_MNG_EMPTY				/* �� ������	            */
} fs_check_manager_option;

typedef enum {
	FS_ACC_READ,
	FS_ACC_WRITE
}fs_stream_access_type;

typedef enum {
	FS_STRM_OKAY_S,
	FS_STRM_FAIL_S,
	FS_STRM_EOF_S
}fs_stream_status;

/* �ܺο� interface�Ǵ� �Լ��� */
fs_stream_status Init_Stream_Manager_Table(void);
fs_stream_status fsi_stream_file_open(byte,fs_stream_access_type);
fs_stream_status fsi_stream_file_close(fs_stream_access_type);
fs_stream_status fsi_stream_file_read(byte*,word);
fs_stream_status fsi_stream_file_write(byte*,word);
fs_stream_status fsi_stream_file_erase(byte);
