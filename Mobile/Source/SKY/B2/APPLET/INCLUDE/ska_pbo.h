#ifndef _SKA_PBO_H_
#define _SKA_PBO_H_



enum tagBOOK_FUNCTION_E 
{
    BFT_NON_FUNCTION_E    = 0,
    BFT_SEARCH_BOOK_E,           // 전화부에서 원하는 번호를 찾을 때
	BFT_SEARCH_SMS_NUMBER_E,     // SMS에서 전화부 찾기를해서 선택하고자 할때 
    BFT_EDIT_PBOOK_E,            // 전화부 편집 
    BFT_SAVE_DATA_E,
    BFT_SAVE_NEW_DATA_E,         // 새이름에 저장
    BFT_SAVE_OLD_DATA_E,         // 기존 이름에 저장
    BFT_EDIT_SDIAL_E,            // 단축 다이얼 편집
	BFT_SET_SDIAL_E,             // 단축 다이얼 지정 
    BFT_EDIT_GROUP_E,            // 그룹 편집 
    BFT_VIEW_IRDA_E,             // IRDA에서 사람항목 선택
    BFT_VIEW_IRDA_GROUP_E,       // IRDA에서 그룹선택 
    BFT_VIEW_EMAIL_LIST_E,
	BFT_GET_GROUP_TEL_E,         // 그룹 동보기능 
    BFT_ERROR_HAPPEN_E
};
typedef enum tagBOOK_FUNCTION_E  BOOK_FUNCTION_E;

BOOL SKY_API FindNumberInBook(BYTE*  pszNumber);

VOID SKY_API SKA_BookMain(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_SpeedSearch(SKY_EVENT_T* pEvent);    
VOID SKY_API SKA_SearchName(SKY_EVENT_T* pEvent);    
VOID SKY_API SKA_SearchNumber(SKY_EVENT_T* pEvent);    
VOID SKY_API SKA_BookInput(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_ViewPersonInfo(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_BookInCalling(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_SearchBookNumber(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_EditBookContent(SKY_EVENT_T* pEvent);


VOID SKY_API SKA_BookTestInit(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_BookTestDataSave(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_BookTestGroupSave(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_BookTestMemberSave(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_BookTestApplet(SKY_EVENT_T* pInEvent);

                  
VOID SKY_API SKA_EditBook(SKY_EVENT_T* pEvent);    
VOID SKY_API SKA_011Service(SKY_EVENT_T* pEvent); 
VOID SKY_API SKA_EditSDial(SKY_EVENT_T* pEvent);   
VOID SKY_API SKA_EditGroup(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_ViewInfo(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_ViewGroupList(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_ViewBookList(SKY_EVENT_T *pInEvent);
VOID SKY_API SKA_ViewBookContent(SKY_EVENT_T *pInEvent);
VOID SKY_API SKA_SearchBook(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_InitPBookData(SKY_EVENT_T* pEvent);

VOID SKY_API SKA_SelectNumber(SKY_EVENT_T* pEvent);
VOID SKY_API SKA_ViewNumber(SKY_EVENT_T* pEvent);

VOID SKY_API SKA_ViewSearchNumber(SKY_EVENT_T* pI460t);
VOID SKY_API SKA_ViewSearchName( SKY_EVENT_T* pInEvent );

VOID SKY_API SKA_SelectNumberType(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_EditGroupOrder(SKY_EVENT_T* pInEvent);
VOID SKY_API SKA_ViewIRDAList(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_ViewIRDAGroup(SKY_EVENT_T *pEvent);
VOID SKY_API SKA_ViewEmailList(SKY_EVENT_T *pEvent);

VOID SKY_API SaveOnNewBook(BYTE* szNumber, BOOL fLock);
VOID SKY_API SaveOnOldBook(BYTE* szNumber, BOOL fLock);

VOID SKY_API SKA_ViewPhoneBookInfo(SKY_EVENT_T* pEvent);

VOID SKY_API SKA_BookDefaultProcess(SKY_EVENT_T *pEvent);

VOID SKY_API SKA_GetSMSGroupTelNumber(SKY_EVENT_T *pEvent);
#ifdef FEATURE_CHARACTER_BELL
VOID SKY_API SKA_CBSearchNumber(SKY_EVENT_T* pEvent);
#endif // FEATURE_CHARACTER_BELL
#endif