#ifndef _SKA_PBO_H_
#define _SKA_PBO_H_



enum tagBOOK_FUNCTION_E 
{
    BFT_NON_FUNCTION_E    = 0,
    BFT_SEARCH_BOOK_E,           // ��ȭ�ο��� ���ϴ� ��ȣ�� ã�� ��
	BFT_SEARCH_SMS_NUMBER_E,     // SMS���� ��ȭ�� ã�⸦�ؼ� �����ϰ��� �Ҷ� 
    BFT_EDIT_PBOOK_E,            // ��ȭ�� ���� 
    BFT_SAVE_DATA_E,
    BFT_SAVE_NEW_DATA_E,         // ���̸��� ����
    BFT_SAVE_OLD_DATA_E,         // ���� �̸��� ����
    BFT_EDIT_SDIAL_E,            // ���� ���̾� ����
	BFT_SET_SDIAL_E,             // ���� ���̾� ���� 
    BFT_EDIT_GROUP_E,            // �׷� ���� 
    BFT_VIEW_IRDA_E,             // IRDA���� ����׸� ����
    BFT_VIEW_IRDA_GROUP_E,       // IRDA���� �׷켱�� 
    BFT_VIEW_EMAIL_LIST_E,
	BFT_GET_GROUP_TEL_E,         // �׷� ������� 
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