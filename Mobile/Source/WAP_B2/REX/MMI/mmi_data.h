#ifndef MMI_DATA_H
#define MMI_DATA_H

#define _ELEMENT_TYPE_CARD			0
#define _ELEMENT_TYPE_TEXT			1
#define _ELEMENT_TYPE_BR			2		
#define _ELEMENT_TYPE_KEY			3
#define _ELEMENT_TYPE_INPUT			4
#define _ELEMENT_TYPE_SELECT		5
#define _ELEMENT_TYPE_OPTIONGROUP	6
#define _ELEMENT_TYPE_OPTION		7
#define _ELEMENT_TYPE_IMAGE			8
#define _ELEMENT_TYPE_LINK_TEXT		9
#define _ELEMENT_TYPE_LINK_IMAGE	10
#define _ELEMENT_TYPE_TABLE_ROW		11
#define _ELEMENT_TYPE_WTLS_SESSION  12

typedef struct _POINT
{
	int x;
	int y;

} point;

typedef struct _FONT_INFOMATION
{
	// SKY에서 지원하는 Font의 인덱스
	int font_index;

	// 폰트의 넓이/높이
	int width;
	int height;

} font_info;

typedef struct _WAP_NODE
{
    int x1, y1;        //시작점
    int x2, y2;        //끝 점

	int type;
    void *pContent;
	font_info font;

    struct _WAP_NODE *pPrevNode;
	struct _WAP_NODE *pNextNode;
	
    struct _WAP_NODE *pPrevFocusNode;
	struct _WAP_NODE *pNextFocusNode;

} WAP_Node;

typedef struct _WAP_CARD
{
    UINT8 viewId;

    char* title;
    BOOL isList;	 
    BOOL isRefresh; 
	char* URL;
	BOOL isBookmarkable;
	char* history;
    WAP_Node *pHeadContent;
    WAP_Node *pHeadFocus;
#ifdef WAP_NOWRAP   
	BOOL isKeyCard;
#endif//#ifdef WAP_NOWRAP

#ifdef WAP_SCROLL_BAR //for Scroll bar - yjahn96 01.05.23
    byte nScrollLine;
#endif  //#ifdef WAP_SCROLL_BAR
} WAP_Card;

typedef struct _WAP_CONTENT_PARAGRAPH
{
	UINT8 viewId;
	INT8 align;
	BOOL wrap;

} WAP_Content_Paragraph;


typedef struct _WAP_CONTENT_TEXT
{
	UINT8 viewId;
	UINT8 textId;
	char* text;
	BOOL isLink;
	char* linkTitle;
	INT8 format;

	int align;

	//for Table
	int nTableColumn;

} WAP_Content_Text;


#define WAP_KEY_SK_UP			1000
#define WAP_KEY_SK_PREV			2000
#define WAP_KEY_SK_NEXT			4000
#define WAP_KEY_SK_CALL			8000
typedef struct _WAP_CONTENT_KEY
{
	UINT8 viewId;
	UINT8 keyId;
	char* eventType;
	char* label;
	BOOL isOptional;

	//------------------------
	// SK Telecom의 키 번호
	//------------------------
	int nSKT_Number;

} WAP_Content_Key;

#define WAP_MAX_INPUT			128
typedef struct _WAP_CONTENT_INPUT
{
	UINT8 viewId;
	UINT8 inputId;
	char* title;
	char text[WAP_MAX_INPUT+1];
	BOOL isPassword;
	BOOL emptyOk;
	char* format;
	INT8 size;
	INT8 nChars;
	INT8 tabIndex;

} WAP_Content_Input;


typedef struct _WAP_CONTENT_SELECT
{
	UINT8 viewId;
	char* title;
	BOOL multiSelect;
	INT8 tabIndex;

	//------------------------
	// Option 화면 구성
	//------------------------
	WAP_Node* pHeadOption;
	WAP_Node* pHeadFocusOption;

	char selectedOptions[255];

} WAP_Content_Select;

typedef struct _WAP_CONTENT_OPTIONGROUP
{
	UINT8 viewId;
	char* label;

	//------------------------
	// Option 화면 구성
	//------------------------
	int indexOption;

} WAP_Content_OptionGroup;

typedef struct _WAP_CONTENT_OPTION
{
	UINT8 viewId;
	UINT8 optionId;
	char* label;
	char* title;
	BOOL isSelected;

	//------------------------
	// Option 화면 구성
	//------------------------
	int indexOption;
	BOOL isSelectedTemp;

} WAP_Content_Option;


#define WAP_IMAGE_TYPE_ETC			0
#define WAP_IMAGE_TYPE_BMP			1
#define WAP_IMAGE_TYPE_WBMP			2
#define WAP_IMAGE_TYPE_SIS			3

typedef struct _WAP_CONTENT_IMAGE
{
	UINT8 viewId;
	UINT8 imageId;
	char* imageData;
	UINT16 imageSize;
	int imageType;
	char* altText;
	char* localSrc;
	BOOL isLink;
	char* linkTitle;
	INT8 vSpace;
	INT8 hSpace;
	INT16 width;
	INT16 height;
	INT8 isPercent;
	INT8 align;

	//for color image
	int nBitCount;

	//for Table
	int nTableColumn;

	//for Animation (SIS)
	int nCurFrame;
	int nTotalFrame;

} WAP_Content_Image;


typedef struct _WAP_CONTENT_TABLE_ROW
{
	UINT8 viewId;
	INT8  noOfColumns;
	
	int align;

	//------------------------
	// Table 화면 구성
	//------------------------
	WAP_Node* pHeadCell;
	WAP_Node* pHeadFocusCell;
	WAP_Node* pFocusCell;
	
} WAP_Content_TableRow;

#endif//MMI_DATA_H