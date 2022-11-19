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
extern void WAP_DisplaySoftkey(void);
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
extern void WAP_DisplaySoftkey_Options(void);
extern void WAP_DisplayCML_Options(void);

//++ for display
	extern void WAP_Draw_Element_Title(void);
	extern void WAP_Draw_Element_Option(WAP_Node *pMe);
	extern void WAP_Draw_Element_OptionGroup(WAP_Node *pMe);
//-- for display

extern void WAP_Scrolling_Options(BOOL bDown);

/********************************************************************************************************/
/*																										*/
/*									SKY Adapter Functions													*/
/*																										*/
/********************************************************************************************************/
extern void SKY_wait (UINT8 viewId, BOOL isWait);
extern void SKY_status (UINT8 viewId, UINT8 status, const CHAR *URL);
extern void SKY_unknownContent (UINT8 viewId, const CHAR *data, UINT16 length, const CHAR *contentType, const CHAR *URL);
extern void SKY_passwordDialog (UINT8 viewId, UINT8 dialogId, const CHAR *realm, INT8 type);
extern void SKY_promptDialog (UINT8 viewId, UINT8 dialogId, const WCHAR *message, const WCHAR *defaultMessage);
extern void SKY_confirmDialog (UINT8 viewId, UINT8 dialogId, const WCHAR *message, const WCHAR *ok, const WCHAR *cancel);
extern void SKY_alertDialog (UINT8 viewId, UINT8 dialogId, const WCHAR *message);
//extern void SKY_newCard (UINT8 viewId, const WCHAR *title, BOOL isList, BOOL isRefresh, const CHAR *URL, BOOL isBookmarkable, const WCHAR * const *history);
extern void SKY_newCard (UINT8 viewId, CHAR *title, BOOL isList, BOOL isRefresh, CHAR *URL, BOOL isBookmarkable, const WCHAR * const *history);
extern void SKY_showCard (UINT8 viewId);
extern void SKY_cancelCard (UINT8 viewId);
extern void SKY_newKey (UINT8 viewId, UINT8 keyId, const WCHAR *eventType, const WCHAR *label, BOOL isOptional);
extern void SKY_newText (UINT8 viewId, UINT8 textId, const WCHAR *text, BOOL isLink, const WCHAR *linkTitle, INT8 format);
extern void SKY_newImage (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType, const WCHAR *altText, const WCHAR *localSrc, BOOL isLink, const WCHAR *linkTitle, INT8 vSpace, INT8 hSpace, INT16 width, INT16 height, INT8 isPercent, INT8 align);
extern void SKY_completeImage (UINT8 viewId, UINT8 imageId, const CHAR *imageData, UINT16 imageSize, const CHAR *imageType);
extern void SKY_newParagraph (UINT8 viewId, INT8 align, BOOL wrap);
extern void SKY_closeParagraph (UINT8 viewId);
extern void SKY_newBreak (UINT8 viewId);
extern void SKY_newTable (UINT8 viewId, const WCHAR *title, INT8 noOfColumns, const CHAR *align);
extern void SKY_newTableData (UINT8 viewId);
extern void SKY_closeTable (UINT8 viewId);
extern void SKY_newFieldSet (UINT8 viewId, const WCHAR *title);
extern void SKY_closeFieldSet (UINT8 viewId);
extern void SKY_newSelect (UINT8 viewId, const WCHAR *title, BOOL multiSelect, INT8 tabIndex);
extern void SKY_closeSelect (UINT8 viewId);
extern void SKY_newOption (UINT8 viewId, UINT8 optionId, const WCHAR *label, const WCHAR *title, BOOL isSelected);
extern void SKY_newOptionGroup (UINT8 viewId, const WCHAR *label);
extern void SKY_closeOptionGroup (UINT8 viewId);
extern void SKY_newInput (UINT8 viewId, UINT8 inputId, const WCHAR *title, const WCHAR *text, BOOL isPassword, BOOL emptyOk, const WCHAR *format, INT8 size, INT8 nChars, INT8 tabIndex);
extern void SKY_getInputString (UINT8 viewId, UINT8 inputId);
extern void SKY_linkInfo (UINT8 viewId, UINT8 widgetType , UINT8 widgetId, const CHAR *URL);

//-------------------------------------------------------------------------------
// WML Script Graphic 관련 Adapter 함수
//-------------------------------------------------------------------------------
extern void SKY_graphicsStart(void);
extern void SKY_graphicsEnd(void);
extern void SKY_graphicsPoint(int x, int y);
extern void SKY_graphicsLine(int x1, int y1, int x2, int y2);
extern void SKY_graphicsRectangle(int x, int y, int width, int height, BOOL isFill);
extern void SKY_graphicsCircle(int x, int y, int radius, BOOL isFill);

/********************************************************************************************************/
/*																										*/
/*									SKY Connector Functions												*/
/*																										*/
/********************************************************************************************************/
extern void SKY_openView (UINT8 viewId,UINT8 uamode);
extern void SKY_closeView (UINT8 viewId);
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