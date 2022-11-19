/*==========================================================================
                   E M B E D D E D   F I L E   S Y S T E M
===========================================================================*/

/*==========================================================================
when       who     what, where, why
--------   ---     ----------------------------------------------------------
12-23-99  jnlee    256byte 단위가 아니라 byte 단위로 write 되도록 수정 
12-2-99   jnlee    create 
===========================================================================*/

/* 데이터 저장용 FLASH MEMORY중 Stream Data저장 시작 offset*/
#define FS_STREAM_OFFSET	0x60000		

/* 데이터 저장용 FLASH MEMORY 시작 offset
	read할 경우에만 사용함				
	write인 경우에는 fsi_intel_write에서 자동적으로 해결함 */
#define FS_DATA_OFFSET		0x300000		

/* 실제적으로 읽을때 사용하는 offset */
#define FS_READ_OFFSET	(FS_STREAM_OFFSET+FS_DATA_OFFSET)

/* flash memory 한 섹터 사이즈 */
#define FS_STREAM_SECTOR_SIZE 	0x10000		

/* Stream Data저장 부분의 sector수  */
#define MAX_STREAM_SECTOR	10		
/* 최대 30개의 stream 데이터를 저장함 */	
#define MAX_STREAM_NUM		30		// 실제개수 
#define MAX_STREAM_ID		(MAX_STREAM_NUM+1)	// 실제수보다 하나 더 많음. 마지막은 ff
/* 섹터 헤더로 부터의 실제 데이터 offset byte */
#define FS_REAL_STREAM_OFFSET		256
/* 블럭 단위의 최소 단위 fragment size*/
#define MIN_STREAM_BLOCK_SIZE		256
/* 1 sector 안의 최대 gragment수 */
#define	MAX_FRAGMENT_IN_SECTOR		255
/* 1 sector에서 invalid데이터가 다음 이상이면 그 섹터를 정리함 */
#define	COLLECTION_THRESHOLD		128

/* 구조체의 offset 계산 */
#define  memberoffsetof(type, member)  ((dword)&((type *)0)->member)
/* 구조체의 멤버 사이즈 계산 */
#define  membersizeof(type, member) (sizeof(((type *)0)->member))

/* Stream Data File Table 구조체 */
typedef struct {
	byte sector;			//sector number
	byte offset;			//offset (base 256bytes)
	word size;				//size(256bytes) of this block ->수정 byte 단위로 수정 0~64K
}Stream_Block_Link;

typedef struct {
	byte file_number;
	byte file_sector_size;		// 64k단위의 사이즈 
	word file_size;			// size (256 byte단위) -> byte 단위로 수정
	Stream_Block_Link block_link[MAX_STREAM_SECTOR];
	// 4byte 단위로 align되도록 유의할 것
}Stream_File_Table_Format;

/****************************************************************
 Manager Block RAM table : 실제는 Flash에 저장되어 있음 
*****************************************************************/
extern Stream_File_Table_Format stream_file_table[MAX_STREAM_ID];


/* flash Memory Sector header */
typedef struct {
	// ffff:미사용 , fefe:진행중 , f0f0;사용가능, efef:매니저 진행중 0f0f:매니저블럭
	word	status;			
	// 논리적인 sector id
	word	sector_id;		
	//현재까지 지워진 횟수 -> wear leveling에 사용됨 
	dword	erase_count;	
	//nibble 단위로 256byte블럭의 상태를 보여줌 -> f:미사용 , e:writing중 c:vaild data   0:invalid data
	byte	fragment_status[MAX_FRAGMENT_IN_SECTOR/2 +1]; 	
}Stream_File_Sector_Header;

#define  FS_SECTOR_NOT_USE	((word)0xffff)			/* 아직 데이터가 저장안된 sector */
#define  FS_SECTOR_PROCESS	((word)0xfefe)			/* 현재 데이터를 쓰고 있는 중  */
#define  FS_SECTOR_VALID	((word)0xf0f0)			/* 현재 유효한 데이터를 가지고있음 */
#define  FS_SECTOR_MAN_PROCESS	((word)0xefef)		/* 현재 매니지먼트 데이터를 쓰고 있는 중  */
#define  FS_SECTOR_MAN_VALID	((word)0x0f0f)		/* 현재 유효한 매니지먼트 데이터를 가지고있음 */
#define  FS_SECTOR_INVALID		((word)0)				/* 현재 무효한 데이터를 가지고있음 */


#define  FS_FRAG_NOT_USE	0x0f		/* 아직 데이터가 저장안된 fragment */
#define  FS_FRAG_PROCESS	0x0e		/* 현재 데이터를 쓰고 있는 중  */
#define  FS_FRAG_VALID		0x0c		/* 현재 유효한 데이터를 가지고있음 */
#define  FS_FRAG_INVALID	0			/* 사용불가능한 fragment -> 지워질 것  */

#define	 FS_END_MARK_B		0xff		/* byte end mark */
#define	 FS_END_MARK_W		0xffff		/* word end mark */



/* 현재까지 읽은 데이터 포인터 */
typedef struct{
	dword file_size;			//전체 사이즈-현재까지 읽은 사이즈 
	byte stream_number;			//stream number
	byte link_number;			//link 번호 
	byte sector;				//
	byte offset;
	word link_size;				// 블럭 사이즈 - 현재까지 읽은 사이즈 
}Stream_Data_Read;

/* 현재 write하고 있는 데이터 정보 */
typedef struct{
	byte stream_number;
	byte link_number;			
	byte sector;				//
	byte offset;
	word link_size;				//현재까지 저장한 사이즈(256byte 단위)->byte단위로 수정 
/* 현재 각 섹터 당 비어 있는 영역 */
	byte data_sector_space[MAX_STREAM_SECTOR];
	byte manager_sector_space;
}Stream_Data_Write;


typedef enum {
  FS_CHECK_NOT_USE,            /* not yet used                                */
  FS_CHECK_INVALID             /* Reserved for file system testing              */
} fs_space_check_option;

typedef enum {
  FS_MNG_LAST,				/* 최종의 Manager file 찾기 */
  FS_MNG_EMPTY				/* 빈 포인터	            */
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

/* 외부와 interface되는 함수들 */
fs_stream_status Init_Stream_Manager_Table(void);
fs_stream_status fsi_stream_file_open(byte,fs_stream_access_type);
fs_stream_status fsi_stream_file_close(fs_stream_access_type);
fs_stream_status fsi_stream_file_read(byte*,word);
fs_stream_status fsi_stream_file_write(byte*,word);
fs_stream_status fsi_stream_file_erase(byte);
