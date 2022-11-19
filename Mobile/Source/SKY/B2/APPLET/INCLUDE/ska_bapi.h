#ifndef _SKA_BAPI_H_
#define _SKA_BAPI_H_

#define BIRTH_ANNIV         1          // 기념일 정보 타입을 나타내는 상수 값 
#define WEDDING_ANNIV       2
#define MEET_ANNIV          3
#define ETC_ANNIV           4

#define CELL	1                     // number type
#define HOME	2  
#define WORK	3
#define PAGER   4
#define FAX		5

#define NUMBER_TYPE_MIN  1
#define NUMBER_TYPE_MAX  FAX

// group order type

#define NON_SECRET_GROUP      0
#define SECRET_GROUP          1

#define VIEW_HEIGHT           5

#define GROUP_STATUS_SECRET   0x01



extern UINT8  InsertGroup(BYTE* Name, UINT8 nPos, BOOL fSecret);
extern UINT8  AddGroup(BYTE* Name, BOOL fSecret);  //2001.01.08    


extern INT16   AddNumber(pb_book_info_type* pBookInfo, BYTE* number, UINT8 type);
extern INT16   ReverseAddNumber(pb_book_info_type* pBookInfo, BYTE* number, UINT8 type);

extern INT16   AddAnnivInfo(pb_book_info_type* pBookInfo, pb_anniv_info_type* pAnnivInfo);
extern BOOL   DeleteAnnivInfo(pb_anniv_info_type* pAnnivInfo);
extern BOOL   DeleteAnniv(pb_book_info_type * pBookInfo, INT16 addr );

extern INT16  AddEmail(pb_book_info_type* pBookInfo, BYTE* EmailID);


extern INT16  MoveMultiBook(pb_group_info_type* pGroupInfo, INT16 BookAddress, UINT8 SortedBookHandle);
extern INT16  MoveBook(pb_group_info_type* pGroupInfo, INT16 BookAddress);
extern INT16  AddMultiBook(pb_group_info_type* pGroupInfo, pb_book_info_type* pBookInfo, UINT8 SortedBookHandle);
extern INT16  AddBook(pb_group_info_type* pGroupInfo, pb_book_info_type* pBookInfo);

extern BOOL   DeleteSDial(UINT8 nDial);
extern BOOL   DeleteGroup(UINT8 GroupAddress, BOOL fContent);
extern BOOL   DeleteNumberInfo(pb_number_info_type* pNumberInfo);
extern BOOL   DeleteNumber(pb_book_info_type * pBookInfo, INT16 nNumberAddress );
extern BOOL   DeleteNumberLink(pb_number_info_type* pNumberInfo);
extern BOOL   DeleteEmail(pb_book_info_type * pBookInfo, INT16 addr );
extern BOOL   DeleteGroupBook(pb_group_info_type* pGroupInfo);
extern BOOL   DeleteBookInfo(INT16 nBookAddress);
extern BOOL   DeleteBook(pb_group_info_type* pGroupInfo, pb_book_info_type* pBookInfo, BOOL fDeleteNumber);


extern INT16  SumOfNumber(BYTE *number);




#endif