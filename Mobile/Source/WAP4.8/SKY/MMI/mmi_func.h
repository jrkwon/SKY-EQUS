#ifndef MMI_FUNCTION_H
#define MMI_FUNCTION_H

#include "mmi_data.h"

/********************************************************************************************************/
/*																										*/
/*									Memory 관련 함수													*/
/*																										*/
/********************************************************************************************************/
extern void *wip_malloc (UINT32 size);
extern void wip_free (void* mem);

/********************************************************************************************************/
/*																										*/
/*									Char 관련 함수														*/
/*																										*/
/********************************************************************************************************/
extern int KSCStrLenOfUni(WCHAR* uStr);
extern int UniLenOfKSCStr(char* kStr);

extern int Uni2KSCString(WCHAR* uniString, unsigned char* kscString);
extern int KSC2UniString(unsigned char* kscString, WCHAR* uniString);

/********************************************************************************************************/
/*																										*/
/*									MMI 관련 Variables													*/
/*																										*/
/********************************************************************************************************/
extern BOOL g_bBlocked;
extern WAP_Card *g_pCard;
extern BOOL g_bTerminatedWAP;

extern WAP_Node *g_pCurFocusNode;			// 현재 화면에서 Focus를 가진 노드로 First와 Last 사이에 존재한다
extern int       g_nLCDOffset;				// 현재 화면(Current Window)의 y 좌표(Offset)

/********************************************************************************************************/
/*																										*/
/*								View/ WapMenu/ Dialog/ Script Mode										*/
/*																										*/
/********************************************************************************************************/

extern int g_nViewMode;
#define		WAP_VIEW_MODE_BLOCKED					0//???
#define		WAP_VIEW_MODE_NONE						10
#define		WAP_VIEW_MODE_WML						1
#define 	WAP_VIEW_MODE_WML_OPTION				2
#define 	WAP_VIEW_MODE_WML_INPUT					3


extern int g_nMenuMode;
#define		WAP_MENU_MODE_BLOCKED					0//???
#define		WAP_MENU_MODE_NONE						10
#define 	WAP_MENU_MODE_MAIN						1


extern int g_nDlgMode;
#define		WAP_DLG_MODE_BLOCKED					0//???
#define		WAP_DLG_MODE_NONE						10
#define 	WAP_DLG_MODE_PROMPT						1
#define 	WAP_DLG_MODE_CONFIRM					2
#define 	WAP_DLG_MODE_ALERT						3
#define 	WAP_DLG_MODE_PASSWORD					4


extern int g_nGraphicMode;
#define		WAP_GRAPHIC_MODE_BLOCKED				0//???
#define		WAP_GRAPHIC_MODE_NONE					10
#define 	WAP_GRAPHIC_MODE_DRAWING				1
#define 	WAP_GRAPHIC_MODE_DRAW					2

/********************************************************************************************************/
/*																										*/
/*									Event Handle 관련 함수												*/
/*																										*/
/********************************************************************************************************/
extern void WAP_MainHandle_Event(int Event);
extern void WAP_Handle_SMS(int Event);
extern void WAP_Handle_DIALOG(int Event);
extern void WAP_Handle_MENU_MAIN(int Event);
extern void WAP_Handle_WML(int Event);
extern void WAP_Handle_WML_OPTION(int Event);


/********************************************************************************************************/
/*																										*/
/*									WML 화면 관련														*/
/*																										*/
/********************************************************************************************************/

/*------------------------------------------------------------------------------*/
/*	-WML-																		*/
/*								Initialize/Terminate							*/
/*																				*/
/*------------------------------------------------------------------------------*/
//extern void WAP_Initialize_WML(void);
extern BOOL WAP_Terminate_WML(void);

/*------------------------------------------------------------------------------*/
/*	-WML-																		*/
/*								Build/Format									*/
/*																				*/
/*------------------------------------------------------------------------------*/
extern void WAP_Building(UINT8 type, void* pContent, int nFontType);
extern void WAP_Formatting(void);
	
//++ for Fomatting
	extern char* GetCurDisplayedData(WAP_Node *pCurNode);
	extern point Calculate_Node_StartPoint(WAP_Node* pPrevNode, WAP_Node* pCurNode);
	extern point Calculate_Node_EndPoint(char* str, int x, int y, int nFontType, int nDivideWidth);
	extern int Calculate_WordCount_OneLine(char* str, int x, int y, int nFontType, int nDivideWidth);
//-- for Fomatting

extern void TruncateString(BYTE *strText, int nMaxChar);

/*------------------------------------------------------------------------------*/
/*	-WML-																		*/
/*								Display											*/
/*																				*/
/*------------------------------------------------------------------------------*/
extern void WAP_Display(void);
extern void WAP_DisplayCML(void);

#ifdef WAP_SCROLL_BAR //for Scroll bar - yjahn96 01.05.23
extern void WAP_DisplayScrollBar(BOOL ExistCard);
#endif  //#ifdef WAP_SCROLL_BAR 

extern void WAP_Send_LCDBuffer_ToHS(void);
extern void WAP_Clear_LCDDataBuffer(void);

//++ for Display
	extern void WAP_Draw_Element_Card(WAP_Node *pMe);
	extern void WAP_Draw_Element_Text(WAP_Node *pMe, int nTableColumns, int nCurColumn);
	extern void WAP_Draw_Element_Input(WAP_Node *pMe);
	extern void WAP_Draw_Element_Select(WAP_Node *pMe);
	extern void WAP_Draw_Element_Key(WAP_Node *pMe);
	extern void WAP_Draw_Element_Image(WAP_Node *pMe, int nTableColumns, int nCurColumn);
	extern void WAP_Draw_Element_TableRow(WAP_Node *pMe);
//-- for Display

// Timer
extern void CancelAnimationTimer(void);
extern void RestartAnimationTimer(unsigned long ms);
extern void CancelNowrapTimer(void);
extern void RestartNowrapTimer(unsigned long ms);


/*------------------------------------------------------------------------------*/
/*	-WML-																		*/
/*								Key Handlers									*/
/*																				*/
/*------------------------------------------------------------------------------*/
#ifdef WAP_NOWRAP
extern void WAP_CheckKeyCard(void);	// yjahn96 00.10.24
extern void SetFocusNode(WAP_Node* pNewFocusNode);//yjahn96 00.10.27
#endif//#ifdef WAP_NOWRAP

extern void WAP_Scrolling(BOOL bDown);
//++ for scrolling
	extern BOOL WAP_FocusScrolling(BOOL bDown);
	extern BOOL WAP_FocusScrolling_Up(void);
	extern BOOL WAP_FocusScrolling_Down(void);

	extern BOOL WAP_WindowScrolling(BOOL bDown);
	extern BOOL WAP_WindowScrolling_Up(void);
	extern BOOL WAP_WindowScrolling_Down(void);

extern void WAP_Scrolling_TableRow(BOOL bRight);

extern int WAP_Handle_CenterKey(void);
extern BOOL WAP_Handle_RightKey(void);
extern BOOL WAP_Handle_NumKey_ForDo(int nKey);

/********************************************************************************************************/
/*																										*/
/*									Option 화면 관련													*/
/*																										*/
/********************************************************************************************************/
extern WAP_Content_Select *g_pCurSelect;
extern int g_nCurFocusOption;

extern void WAP_Building_Options(int type, void *pContent);
extern void WAP_Formatting_Options(void);

extern void WAP_Display_Options(void);
extern void WAP_DisplayCML_Options(void);

//++ for display
	extern void WAP_Draw_Element_Title(void);
	extern void WAP_Draw_Element_Option(WAP_Node *pMe);
	extern void WAP_Draw_Element_OptionGroup(WAP_Node *pMe);
//-- for display

extern void WAP_Scrolling_Options(BOOL bDown);

extern void WAP_Select_Options(void);
extern void WAP_Handle_LeftKey_Options(void);
extern void WAP_Handle_RightKey_Options(void);
extern void WAP_Handle_CenterKey_Options(void);


/********************************************************************************************************/
/*																										*/
/*									Dialog 화면 관련													*/
/*																										*/
/********************************************************************************************************/
extern void WAP_Display_Dialog(void);
extern void WAP_DisplayCML_Dialog(void);

extern void WAP_Build_Dialog(void);
extern void WAP_Scrolling_Message(BOOL bDown);

extern void WAP_Alert_Dialog(void);
extern void WAP_OK_Dialog(void);
extern void WAP_Cancel_Dialog(void);
extern void WAP_Close_Dialog(void);

/********************************************************************************************************/
/*																										*/
/*									n.TOP Menu 화면 관련												*/
/*																										*/
/********************************************************************************************************/
extern void WAP_Display_Menu(void);
extern void WAP_DisplayCML_Menu(void);
extern void WAP_Draw_Menu_Item(int nIndex);

extern void WAP_Build_Menu(void);
extern void WAP_Scrolling_Menu(BOOL bDown);

extern int WAP_Handle_CenterKey_MainMenu(void);
extern void WAP_Handle_NumKey_MainMenu(int nKey);


/********************************************************************************************************/
/*																										*/
/*									SKY Adapter Functions													*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/*									SKY Connector Functions												*/
/*																										*/
/********************************************************************************************************/
extern void SKY_startUserAgent (UINT8 viewId,UINT8 uamode);
extern void SKY_terminateUserAgent (UINT8 viewId);
extern void SKY_loadURL (UINT8 viewId, const CHAR* url, BOOL reload);
extern void SKY_reload (UINT8 viewId);
extern void SKY_stop (UINT8 viewId);
extern void SKY_goBack (UINT8 viewId);
extern void SKY_passwordDialogResponse (UINT8 viewId, UINT8 dialogId, const CHAR *name, const CHAR *password);
extern void SKY_promptDialogResponse (UINT8 viewId, UINT8 dialogId, const WCHAR *answer);
extern void SKY_confirmDialogResponse (UINT8 viewId, UINT8 dialogId, BOOL answer);
extern void SKY_alertDialogResponse (UINT8 viewId, UINT8 dialogId);
extern void SKY_keySelected (UINT8 viewId, UINT8 keyId);
extern void SKY_textSelected (UINT8 viewId, UINT8 textId);
extern void SKY_imageSelected (UINT8 viewId, UINT8 imageId);
extern void SKY_optionSelected (UINT8 viewId, UINT8 optionId);
extern void SKY_inputString (UINT8 viewId, UINT8 inputId, const WCHAR *text);
extern void SKY_linkInfo_c (UINT8 viewId, UINT8 widgetType, UINT8 widgetId);

#endif//MMI_FUNCTION_H//