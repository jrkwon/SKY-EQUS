#ifndef _SKA_BUTL_H_
#define _SKA_BUTL_H_

#define  A7_NV_MAX_DIAL_DIGITS      32  
#define  A7_NV_MAX_MEMO_SIZ         64 
#define  A7_NV_MAX_LTRS             16

typedef  struct {
  INT16       nBookAddress;
  INT16       nNumAddress;
} TelInfo;

typedef PACKED struct {
    INT       address;                     /* Storage address */
    BYTE      status;                       /* flag bits */
    BYTE      num_email;                   /* Number of digits */
    BYTE      email[A7_NV_MAX_DIAL_DIGITS];   /* Array of digits */
    BYTE      exist;
    BYTE      group;                        /* group 0: 그룹[기타] */
    BYTE      multi_num_digits[4];          /* 0: 휴대폰  1:집 2:사무실 3:호출기 */
    BYTE      multi_digits[4][A7_NV_MAX_DIAL_DIGITS];   

    BYTE      anniv_contidx; /* 기념일 내용 item index 0:없음 1:생일 2: 결혼 3:첫만남 4:편집 */
    BYTE      anniv_month;   /* 기념일 달 */
    BYTE      anniv_day;     /* 기념일 일 */
    BYTE	  anniv_lunar;   /* 기념일 0: 양력 1:음력 */
    BYTE      anniv_content[A7_NV_MAX_LTRS]; /* anniv_contidx가 4인 경우: 편집된 내용 */
    BYTE      memo[A7_NV_MAX_MEMO_SIZ];
    BYTE      letters[A7_NV_MAX_LTRS];         /* Name associated with number */
    BYTE      sync_flag;           /* Cmodify flag for WAP sync */
    DWORD     tstamp;              /* Time stamp for WAP sync : elasped time since 1980.1.6*/
} NV_DIAL_TYPE;


enum  PBook_Error_E
{    
    PBE_NO_ERROR = 0,
    PBE_FULL_GROUP,
    PBE_FULL_BOOK,
    PBE_FULL_NUMBER,
    PBE_FULL_ANNIV,
    PBE_FULL_EMAIL,
    PBE_INVALID_GROUP,
    PBE_INVALID_BOOK,
    PBE_INVALID_NUMBER,
    PBE_INVALID_ANNIV,
    PBE_INVALID_EMAIL
};
typedef enum PBook_Error_E  PBOOK_ERROR_E;





/*  type for utility function  */
extern UINT8    GetBookList(UINT8 GroupAddress);
extern UINT8    GetEmailList(void);

extern void     MoveGroup(UINT8 FromGroup, UINT8 ToGroup);
extern UINT8    SortGroup(void);               // 2001.01.05
extern UINT8    GetGroupList(void);
extern void     SortBookName(UINT8 Handle);
extern INT16    GetNumOfGroupMember(UINT8 GroupAddress);
extern BOOL     GetGroupInfo(pb_group_info_type* pGroupInfo, UINT8 nPos);
extern BYTE*    GetGroupName(UINT8 nIndex);
extern BOOL     SetGroupInfo(pb_group_info_type* pGroupInfo, UINT8 nPos);

extern BOOL     ReadGroup(pb_group_info_type*  pGroupInfo);
extern BOOL     WriteGroup(pb_group_info_type* pGroupInfo);
extern BOOL     ReadBook(pb_book_info_type* pBookInfo);
extern BOOL     WriteBook(pb_book_info_type* pBookInfo);
extern BOOL     ReadSDial(pb_sdial_type*   pSDial);
extern BOOL     WriteSDial(pb_sdial_type*  pSDial);
extern BOOL     ReadNumber(pb_number_info_type* pNumberInfo);
extern BOOL     WriteNumber(pb_number_info_type* pNumberInfo);
extern BOOL     ReadEmail(pb_email_info_type*  pEmailInfo);
extern BOOL     WriteEmail(pb_email_info_type* pEmailInfo);
extern BOOL     ReadAnniv(pb_anniv_info_type* pAnnivInfo);
extern BOOL     WriteAnniv(pb_anniv_info_type* pAnnivInfo);

extern BOOL     GetBookNumberIndex(INT16 nBookAddress, INT16* pnNumber);


extern void     BookCleanUp(void);

extern INT16    CountActiveRecord( PB_Type nv_type );
extern INT16    CountFreeRecord( PB_Type nv_type );

extern BOOL     IsValid( PB_Type  nv_type , INT16 address );
extern BOOL     IsPartialMatchString(BYTE* Name, BYTE* ComparedName);

/* rosa 2001-09-24 -->strcmp(...)의 정확한 return값 위해(임시) */
//extern INT8     KOREAN_STRCMP(BYTE* pszStr1, BYTE* pszSTR2 );
extern INT32    KOREAN_STRCMP(BYTE* pszStr1, BYTE* pszSTR2 );
/* <-- */
extern UINT8    GetKoreanCho(BYTE* Name);
extern void     PhoneBookUpdate(void);     // Save Status of Phone Book

/* basic utility function for book */

extern INT16    FindBook(BYTE* szGroup,BYTE* name);
extern INT16    FindDuplicatedBook(INT8 nGroupAddress,INT16 nBookAddress, BYTE* szName);
extern UINT8    SearchName( BYTE* Name, SEARCH_MODE_E Mode);
extern UINT8    SearchGroup(BYTE* gName);
extern INT16    SearchNumber(BYTE* pszNumber, SEARCH_MODE_E Mode);
extern BYTE*    FindNumber(BYTE* szNumber);         // for full name search


extern BOOL     GetTelInfo(BYTE* szNumber, TelInfo* pTelInfo);
extern BOOL     GetShortDialInfo(UINT8 DialAddress, TelInfo* pTelInfo);
extern BOOL     SetShortDialInfo(pb_sdial_type* pSDial);

extern PBOOK_ERROR_E  WriteOldBook(NV_DIAL_TYPE*  pDial);
extern PBOOK_ERROR_E  SaveOldBook(BYTE* szGroup, NV_DIAL_TYPE*  pDial, INT16* nBookAddress);
extern BOOL           MakeOldBook(INT16 BookAddress, NV_DIAL_TYPE* pDial);
extern BOOL           DeltePictureInBook(DWORD dwPictureIndex);

extern UINT8  GetBirthStar(UINT8 nMonth, UINT8 nDay);
extern INT16 Search011Number(BYTE* pszNumber);
extern void GetNumberTypeSFont(UINT8 nSource, BYTE szTypeFontBuf[]); // Added by hjcho 01-04-22



/* basic utility function for schedule */
BOOL	StackInit(INT16 nSize);
void	StackFree(void);
BOOL IsStackEmpty(void);
BOOL	Push(INT16 data);
INT16   Pop(void);


extern BOOL FindCallerNameInBook(UINT8 Handle, BYTE*pName, UINT8 nNameLen, UINT8 nStartIndex, UINT8 nSize);
extern BOOL FindSMSNameInBook(BYTE**ppNumber, BOOL* pfaMatch, UINT8 DataSize);




#endif