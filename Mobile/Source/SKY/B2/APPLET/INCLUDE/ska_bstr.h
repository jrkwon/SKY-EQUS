#ifndef _SKA_BSTR_H_
#define _SKA_BSTR_H_

#define PSYS_FILE           SKY_PBK_DIR""SKY_DIR_CHAR"system.pbk"
#define PGROUP_FILE         SKY_PBK_DIR""SKY_DIR_CHAR"group.pbk"
#define PBOOK_FILE          SKY_PBK_DIR""SKY_DIR_CHAR"book.pbk"
#define PNUMBER_FILE        SKY_PBK_DIR""SKY_DIR_CHAR"number.pbk"
#define PEMAIL_FILE         SKY_PBK_DIR""SKY_DIR_CHAR"memo.pbk"
#define PANNIV_FILE         SKY_PBK_DIR""SKY_DIR_CHAR"anniv.pbk"

#ifndef WIN32
#include "fs.h"

extern fs_rsp_msg_type           BookSysFile;
extern fs_rsp_msg_type           GroupFile;
extern fs_rsp_msg_type           BookFile;
extern fs_rsp_msg_type           NumberFile;
extern fs_rsp_msg_type           EmailFile;
extern fs_rsp_msg_type           AnnivFile;

#else
#include <stdio.h>

extern FILE*           BookSysFile;
extern FILE*           GroupFile;
extern FILE*           BookFile;
extern FILE*           NumberFile;
extern FILE*           EmailFile;
extern FILE*           AnnivFile;

#endif 

#define MAX_SDIAL_NUMBER                 99
#define MAX_GROUP_NUMBER                 21//40
#define MAX_BOOK_NUMBER                 1200
#define MAX_PHONE_NUMBER                1200
#define MAX_ANNIV_NUMBER                600
#define MAX_EMAIL_NUMBER                600

#define PB_MAX_STRING_LENGTH             32
#define PB_MAX_DIGIT_LENGTH              32
#define PB_MAX_MEMO_LENGTH               64

#define MAX_ANNIV_NAME                   16
#define MAX_GROUP_NAME                   16
#define MAX_BOOK_NAME                    16

#define PB_MAX_LINK                       4

#define PERSONAL_GROUP_LIMIT    2     // 사람이 속할수 있는 최대 Group 수
#define PERSONAL_NUMBER_LIMIT   5
#define PERSONAL_MEMO_LIMIT     1
#define PERSONAL_ANNIV_LIMIT    2
#define PERSONAL_EMAIL_LIMIT    2

                      
typedef struct
{    
    UINT8   group_info;   
    INT16   anniv_info[12];                          // each month
    INT16   email_info;
    INT16   number_info;
    INT16   number_search[10];                       // for speed  full number search
    INT16   number_info_limit;                       // for speed  full partial number search

    INT16   free_number_info;                         // free list
    INT16   free_anniv_info;
    INT16   free_email_info;
    UINT8   nFreeSdial;
    UINT8   nFreeGroup;
    UINT16  nFreeBook;
    UINT16  nFreeNumber;
    UINT16  nFreeEMail;
    UINT16  nFreeAnniv;
	UINT16  nFreeMemo;
	UINT16  nFreePicture;

} pb_active_list_type;


typedef  struct {
  UINT8       address;                      /* Storage address */
  UINT8       status;                       /* flag bits */
  WORD        dial_address;                 /* address of ponbook item */
} pb_sdial_type;


#define MAX_POST_LEN     10
#define MAX_ADDRESS_LEN  60
#define MAX_TEL_LEN      12

typedef  struct{
  BYTE   MobileTel[MAX_TEL_LEN+1];
  BYTE   HomeTel[MAX_TEL_LEN+1];
  BYTE   Name[MAX_BOOK_NAME+1];  
  BYTE   Address[MAX_ADDRESS_LEN+1];
  BYTE   Email[PB_MAX_STRING_LENGTH+1];
  WORD   wYear;
  UINT8  nMonth;
  UINT8  nDay;
  BOOL   fLunar;
  DWORD  dwPictureIndex;
  UINT8  XPos;
  UINT8  YPos;
} pb_info_type;  // 자기정보 

// total telephone book size  ( 140080 bytesor 118480 bytes )
// maximum book size  = 576 bytes( book itself: 76 byte + 500 bytes(tel:5+memo:3+anniv:2)

typedef  struct 
{   
    BYTE            address;
    INT16           first_book_address; 
    BYTE            group_name[MAX_GROUP_NAME+1];    
    WORD            wMember;                                 //11.03
    BYTE            status;                                  //secret or not
    BYTE            next_index;
    BELL_TYPE_T     Bell;
//#ifdef FEATURE_COLOR_ID_LCD
    ID_LCD_COLOR_E  alert_color;
//#endif
    DWORD           tstamp;                                 /*   time stamp for WAP sync   */
}  pb_group_info_type;                                      // 52 bytes *40 = 2080

typedef  struct{ 
    INT16 address;
    INT16 next_index;
    UINT8 group_address;                      /*   address information of group infomation  */     
    UINT8 multi_group;                        /*   address of multi group */
    INT16 number_address;   
    INT16 anniv_address;         
    INT16 email_address;
    BYTE  name[MAX_BOOK_NAME+1];
    BYTE  szText[PB_MAX_MEMO_LENGTH+1];       // memo
    DWORD dwImageIndex;
    INT16 other_link[PB_MAX_LINK];           /* link to melody ,picture and so on  */
    BYTE  sync_flag;                        /* Cmodify flag for WAP sync          */
    BYTE  mmi_flag;                         /* flag for MMI sync                  */
    DWORD tstamp;                           /* time stamp for WAP sync: elasped time since 1980.1.6 */    
} pb_book_info_type;                        // 76 bytes * 300 = 22800

typedef  struct                             // email
{
    INT16 address;
    INT16 book_address;
    INT16 next_index;
    BYTE  szMailID[PB_MAX_STRING_LENGTH+1];
} pb_email_info_type;                       // 72 bytes  * 600 = 43200

typedef  struct
{   
    INT16  address;      
    INT16  next_index;   
    INT16  slink;                                /* for speed number search */
    INT16  book_address; 
    INT16  call_number;                          /*  call frequency for search by frequency       */
    UINT8  number_type;                          /* 1: CELL  2:HOME  3:WORK   4:PAGER  5:FAX */    
    BYTE   digits[PB_MAX_DIGIT_LENGTH+1];
} pb_number_info_type;                           // 44 bytes * 1200 = 52800

typedef  struct
{   
    INT16   address;
    INT16   book_address;   
    INT16   next_index;                  // connect with anniv infomation of same book information
    INT16   link;                        // connect with same  month
    UINT8   type;                        /* NO_ANNIV:없음 BIRTH_ANNIV:생일 WEDDING_ANNIV: 결혼 MEET_ANNIV: 첫만남 ETC_ANNIV: 편집  */
    BYTE    content[MAX_ANNIV_NAME+1];   
    UINT8   day;
    UINT8   month;   
    BOOL    fLunar;
    WORD    year;
} pb_anniv_info_type;                    // 32 bytes * 600 = 19200

typedef enum 
{    
    FREE_GROUP_INFO_NUM,
    FREE_BOOK_INDEX_NUM,
    FREE_BOOK_INFO_NUM,
    FREE_NUMBER_INFO_NUM,
    FREE_ANNIV_INFO_NUM
} PhoneInfoType;

typedef enum
{
    PB_SYS_I = 0,
    PB_ACTIVE_LIST_I,  
    PB_GROUP_INFO_I,
    PB_BOOK_INDEX_I,
    PB_BOOK_INFO_I,
    PB_NUMBER_INFO_I,    
    PB_EMAIL_INFO_I,  
    PB_ANNIV_INFO_I,
    PB_SDIAL_I,
    PB_INFO_I,
    PB_LIST_HANDLE_I
} PB_Type; 

typedef enum
{
    INFO_FREE_NUM,
    INFO_ACTIVE_NUM
} Info_Type;

typedef void pb_item_type;

typedef enum
{
    PHONE_NUMBER_E=6,
    WORK_NUMBER_E=5,
    HOME_NUMBER_E=4,
    FAX_NUMBER_E=3,
    PAGER_NUMBER_E=2,
    ETC_NUMBER_E =1,
    NAME_E = 50,
    GROUP_E =51,
    ANNIV_E =52,
    EMAIL_E  =53
        
} Book_Content_type;
/*  type infomation for SKY scheduler  */

typedef enum
{
    PSYS_FILE_E,
    PGROUP_FILE_E,
    PBOOK_FILE_E,
    PNUMBER_FILE_E,
    PEMAIL_FILE_E,
    PANNIV_FILE_E
} PB_File_type;


enum  tagBOOK_INFOLINK_E
{
    BIT_PERSONAL_BELL=0,
    BIT_PERSONAL_PICTURE=1,
    BIT_XXXXX_XXXX1=2,
    BIT_XXXXX_XXXX2=3
};
typedef enum tagBOOK_INFOLINK_E  BOOK_INFOLINK_E;


enum tagSEARCH_MODE_E
{
    BSM_NUMBER_FULL_MATCH,
    BSM_NUMBER_PART_MATCH,
    BSM_NAME_FULL_MATCH,
    BSM_NAME_PART_MATCH,
    BSM_NAME_NUMBER_MATCH,
    BSM_NO_MATCH
};
typedef enum tagSEARCH_MODE_E   SEARCH_MODE_E;

#define PBOOK_HEADER_POS        0                                      // sys file position
#define NUMBER_INFO_ALLOC_POS   sizeof(SKY_FILE_HEADER_T)
#define ANNIV_INFO_ALLOC_POS    (NUMBER_INFO_ALLOC_POS + sizeof(INT16) * MAX_PHONE_NUMBER )
#define EMAIL_INFO_ALLOC_POS    (ANNIV_INFO_ALLOC_POS  + sizeof(INT16) * MAX_ANNIV_NUMBER )
#define ACTIVE_LIST_POS         (EMAIL_INFO_ALLOC_POS   + sizeof(INT16) * MAX_EMAIL_NUMBER)
#define SDIAL_POS               ( ACTIVE_LIST_POS      + sizeof( pb_active_list_type) )
#define PB_INFO_POS             ( SDIAL_POS            + sizeof( pb_sdial_type)*MAX_SDIAL_NUMBER)


#define GROUP_INFO_POS          sizeof(SKY_FILE_HEADER_T)       // group  info file position
#define BOOK_INFO_POS           sizeof(SKY_FILE_HEADER_T)       // book   info file position
#define NUMBER_INFO_POS         sizeof(SKY_FILE_HEADER_T)       // number info file position
#define ANNIV_INFO_POS          sizeof(SKY_FILE_HEADER_T)       // anniv  info file position
#define EMAIL_INFO_POS          sizeof(SKY_FILE_HEADER_T)       // email  info file position


#define BOOK_NOT_USE            0



extern pb_active_list_type      active_list;


extern INT16   g_SDialNumber[MAX_SDIAL_NUMBER]; 
extern BYTE    g_szGroupName[MAX_GROUP_NUMBER][MAX_GROUP_NAME+1];    // redstar  2001.01.05
extern INT8    g_GroupOrder[MAX_GROUP_NUMBER];                       // 그룹 순서 
extern UINT8   g_nGroup;                                            
extern BYTE    g_szPersonName[MAX_BOOK_NUMBER][MAX_BOOK_NAME+1];
extern UINT8   g_PersonGroup[MAX_BOOK_NUMBER];
extern UINT8   g_PersonMultiGroup[MAX_BOOK_NUMBER];
extern UINT8   g_PersonNumberType[MAX_BOOK_NUMBER][PERSONAL_NUMBER_LIMIT];

#define        VALID_SDIAL_NUMBER       (MAX_SDIAL_NUMBER-active_list.nFreeSdial)
#define        VALID_GROUP_NUMBER       (g_nGroup)
#define        VALID_BOOK_NUMBER        (MAX_BOOK_NUMBER-active_list.nFreeBook)
#define        IS_VALID_BOOK(k)         ( g_szPersonName[k-1][0] != BOOK_NOT_USE )
#define        IS_VALID_GROUP(k)        ( g_szGroupName[k-1][0]  != BOOK_NOT_USE )
#define        IS_VALID_SDIAL(k)        IsValidSDial(k)
#define        POSITIVE(n)              (n >= 0 ?   n  : -(n))
#define        NEGATIVE(n)              (n >  0 ? -(n) :   n)
#define        GET_BOOK_GROUP(n)        (g_PersonGroup[n-1])
#define        GET_GROUP_ORDER(n)       (g_GroupOrder[n-1] >= 0 ? g_GroupOrder[n-1] : (-g_GroupOrder[n-1]))
#define        SET_GROUP_ORDER(n, v)    (g_GroupOrder[n-1] = (v))
//#define        SET_GROUP_ORDER(n, v)    g_GroupOrder[n-1] >= 0 ? (g_GroupOrder[n-1] = (v)) : (g_GroupOrder[n-1] = -(v))
#define        IS_SECRET_GROUP(n)       ( g_GroupOrder[n-1] < 0 )
#define        SET_SECRET_GROUP(n,f)    f == TRUE ?  (g_GroupOrder[n-1] = NEGATIVE(g_GroupOrder[n-1])) : \
                                                     (g_GroupOrder[n-1] = POSITIVE(g_GroupOrder[n-1]))


void InitSysFile(void);
void InitGroupFile(BYTE* sByteBuffer, UINT16 nSize);
void InitBookFile(BYTE* sByteBuffer,  UINT16 nSize);
void InitNumberFile(BYTE* sByteBuffer,UINT16 nSize);
void InitMemoFile(BYTE* sByteBuffer,  UINT16 nSize);
void InitAnnivFile(BYTE* sByteBuffer, UINT16 nSize);


extern void     init_pb_node( PB_Type  NvType , void* data );
extern INT16    alloc_pb_node( PB_Type nv_type );
extern BOOL     free_pb_node( PB_Type nv_type , INT16 address);

extern INT16    pb_get_address( PB_Type  nv_type ,  INT16 address );
extern INT16    pb_put_address( PB_Type  nv_type ,  INT16 address , INT16 data );
extern void     pb_get_header( PB_Type  nv_type , SKY_FILE_HEADER_T* pHeader );
extern void     pb_put_header( PB_Type  nv_type , SKY_FILE_HEADER_T* pHeader );

extern INT16    ui_get_pb( PB_Type  nv_type ,  void*  data );
extern INT16    ui_put_pb( PB_Type  nv_type ,  void*  data );

extern void     PBookFilesOpen(void);
extern void     PBookOpen(void);
extern void     PBookClose(void);
extern BOOL     PBookInit(PB_Type  booktype);

extern INT16    Read_number(void* pBuffer, INT16 StartAddress, UINT8 nNumber);

extern INT16    Read_anniv(void* pBuffer, INT16 StartAddress, UINT8 nNumber);
extern INT16    Read_email(void* pBuffer, INT16 StartAddress, UINT8 nNumber);

extern BOOL     IsValidSDial(UINT8 Address);
extern BOOL     IsFullPBook(void);
extern BOOL     IsFullGroup(void);

#endif