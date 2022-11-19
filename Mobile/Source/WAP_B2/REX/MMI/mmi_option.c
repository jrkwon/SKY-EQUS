//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
#include <stdio.h>
//////////////////////////////////////////////////

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include <string.h>

#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"

//General Display Library
#include "GenDsp.h"

#if MMI_TYPE == SKY_UI  //for EQUS
#include "sky.h"
#endif


//-----------------------------------------------------------------------------------
//
// MMI 의 Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "clnt.h"

//Option 화면 구성
int g_nCurStartOption;
int g_nCurFocusOption;

extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);

//for Softkeys -- yjahn96 01.01.04
extern WORD WAP_OutWordCharLCDBufferHan(BYTE *pFont, int nPosX, int nPosY, int nFontWidth, int nFontHeight, int nDrawMode);
extern BYTE *Widening(BYTE *pFont, BYTE nBytePerChar);

#ifndef WAP_USE_SECURE
	extern BOOL Wtlshello;
	extern BOOL wtls_session;
	extern storeWML enc_use;
#endif//#ifndef WAP_USE_SECURE

#ifdef WAP_USE_DORM
	extern storeWML normal_use;
#endif//#ifdef WAP_USE_DORM
BOOL optionselect = FALSE;

#define MAX_WAP_LINES					6//3

//========================================================================================

void WAP_Building_Options(int type, void *pContent)
{
	WAP_Node *pTempNode;

	//------------------------------------------------------------------
	// 새로운 노드를 하나 만든다
	WAP_Node *pNewNode; 
	pNewNode = (WAP_Node *) wip_malloc(sizeof(WAP_Node));

	//------------------------------------------------------------------
	// 아규먼트로 받은 받은 노드의 정보(type, pContent)를 입력한다
	pNewNode->type = type;
	pNewNode->pContent = pContent;
	pNewNode->pPrevNode = NULL;
	pNewNode->pNextNode = NULL;
	pNewNode->pPrevFocusNode = NULL;
	pNewNode->pNextFocusNode = NULL;

	//------------------------------------------------------------------
	// 새 노드의 폰트 Height를 정한다
	pNewNode->font.height	= WAP_HAN_FONT_HEIGHT;
	pNewNode->font.width	= WAP_HAN_FONT_WIDTH;
	
	//------------------------------------------------------------------
	// 새 노드를 리스트(pHeadContent )의 마지막에 추가한다
	if(g_pCurSelect->pHeadOption == NULL)
	{
		g_pCurSelect->pHeadOption = pNewNode;
	}
	else
	{
		pTempNode = g_pCurSelect->pHeadOption;
    
		while(pTempNode->pNextNode != NULL)
		{
			pTempNode = (WAP_Node *)pTempNode->pNextNode;
		}

		pTempNode->pNextNode = (WAP_Node *)pNewNode;
		pNewNode->pPrevNode = (WAP_Node *)pTempNode;
	}

	//------------------------------------------------------------------
	// 새 노드가 선택 가능한(Focus 가능) 노드라면,
	// 새 노드를 선택 리스트(pHeadFocus)의 마지막에 추가한다
	if(pNewNode->type == _ELEMENT_TYPE_OPTION)
	{
		if(g_pCurSelect->pHeadFocusOption == NULL)
		{
			g_pCurSelect->pHeadFocusOption = pNewNode;
		}
		else
		{
			pTempNode = g_pCurSelect->pHeadFocusOption;
		
		    while(pTempNode->pNextFocusNode != NULL) 
			{
				pTempNode = (WAP_Node *)pTempNode->pNextFocusNode;
			}
		
			pTempNode->pNextFocusNode = pNewNode;
			pNewNode->pPrevFocusNode = pTempNode;
		}
	}
}

void WAP_Formatting_Options()
{
	int i;
	WAP_Node *pCurNode;

	//---------------------------------------
	//init
	g_nCurStartOption = 0;
	g_nCurFocusOption = 0;
	//---------------------------------------

	pCurNode = g_pCurSelect->pHeadOption;
	for(i=0; pCurNode != NULL; i++)
	{
		//Option 관련에 Index를 붙인다 //0_based
		switch(pCurNode->type)
		{
			case _ELEMENT_TYPE_OPTION  :
			{
				WAP_Content_Option *pContent;
				pContent = (WAP_Content_Option *)pCurNode->pContent; 
				pContent->indexOption = i;
				
				pContent->isSelectedTemp = pContent->isSelected;//init
				break;
			}

			case _ELEMENT_TYPE_OPTIONGROUP  :
			{
				WAP_Content_OptionGroup *pContent;
				pContent = (WAP_Content_OptionGroup *)pCurNode->pContent; 
				pContent->indexOption = i;
				break;
			}
		};
			
		//새 라인에서 시작한다
		pCurNode->x1 = 0;//이미지없음
		pCurNode->y1 = i*WAP_ASCII_FONT_HEIGHT;

		pCurNode->x2 = WAP_LCD_WIDTH-1;//=95
		pCurNode->y2 = (i+1)*WAP_ASCII_FONT_HEIGHT;

		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}// end of for

	//---------------------------------------------------
	//focus index 결정1
	g_nCurFocusOption = -1;
	pCurNode = g_pCurSelect->pHeadFocusOption;
	while(pCurNode != NULL)
	{
		WAP_Content_Option *pContent;
		pContent = (WAP_Content_Option *)pCurNode->pContent; 

		if(pCurNode->type == _ELEMENT_TYPE_OPTION
			&& pContent->isSelectedTemp)
		{
			g_nCurFocusOption = pContent->indexOption;
			break;
		}
		pCurNode = (WAP_Node *)pCurNode->pNextFocusNode;
	}
		
	//---------------------------------------------------
	//focus index 결정2
	if(g_nCurFocusOption == -1)
	{
		pCurNode = g_pCurSelect->pHeadOption;
		while(pCurNode != NULL
		   && pCurNode->type != _ELEMENT_TYPE_OPTION)
		{
			g_nCurFocusOption++;
			pCurNode = (WAP_Node *)pCurNode->pNextNode;
		}
	}
	
	//---------------------------------------------------
	//focus index 결정3
	if(g_nCurFocusOption == -1)
		g_nCurFocusOption = 0;

	//---------------------------------------------------
	//start index 결정
	if(g_nCurStartOption + (MAX_WAP_LINES-1) < g_nCurFocusOption)//다른화면
		g_nCurStartOption = g_nCurFocusOption - (MAX_WAP_LINES-1);

}

void WAP_Display_Options()
{
	int i;
	WAP_Node *pCurNode;

	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();

	//------------------------------------------------------------------
	// 옵션 화면의 타이틀을 그려준다
	//------------------------------------------------------------------
	WAP_Draw_Element_Title();
		
	//------------------------------------------------------------------
	// 현재의 시작 노드로 간다
	//------------------------------------------------------------------
	pCurNode = g_pCurSelect->pHeadOption;
	for(i=0; i<g_nCurStartOption && pCurNode != NULL; i++)
	{
		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}

	//------------------------------------------------------------------
	// 한 화면에 옵션관련(Option, OptionGroup)은 3개까지 보인다
	// 즉 g_nCurStartOption부터 최대 MAX_WAP_LINES(6)개까지 그려준다
	//------------------------------------------------------------------
	for(i=0; pCurNode != NULL && i<MAX_WAP_LINES; i++)
    {
		//------------------------------------------------------------------
	   	// 현재의 노드 type에 따라 각 노드를 그려주는 함수를 부른다
		//------------------------------------------------------------------
		switch(pCurNode->type)
		{
	        case _ELEMENT_TYPE_OPTION:
				WAP_Draw_Element_Option(pCurNode);
				break;

			case _ELEMENT_TYPE_OPTIONGROUP:
				WAP_Draw_Element_OptionGroup(pCurNode);
				break;

			default :
				break;	//Unknown 은 처리하지 않는다

		};// end of switch

		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	
     }// end of for
#if (defined WAP_B1) || (defined WAP_B2)
	WAP_DisplayCML_Options();
#else   //#if (defined WAP_B1) || (defined WAP_B2)
    WAP_DisplaySoftkey_Options();
#endif  //#if (defined WAP_B1) || (defined WAP_B2)

	WAP_Send_LCDBuffer_ToHS();
}

//-----------------------------------------------------
// yjahn96 01.03.05
void WAP_DisplayCML_Options()
{
	//------------------------------------------------------------------
	// 현재의 Focus 노드로 간다
	//------------------------------------------------------------------
	WAP_Node *pCurFocusNode;
	WAP_Content_Option *pContent;
	pCurFocusNode = g_pCurSelect->pHeadFocusOption;
	while(pCurFocusNode != NULL && pCurFocusNode->type == _ELEMENT_TYPE_OPTION)
	{
		pContent = (WAP_Content_Option *)pCurFocusNode->pContent;
		if(pContent->indexOption == g_nCurFocusOption)
			break;
		pCurFocusNode = (WAP_Node *)pCurFocusNode->pNextFocusNode;
	}

	//------------------------------------------------------------------
	//clear
    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//WAP_Display_Image_CML(WAP_OK_CML/*WAP_UPDOWN_CML*/, WAP_CML_OKEMPTY/*WAP_CML_UPDOWNEMPTY*/ ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OKEMPTY, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CETEREMPTY , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_RIGHTEMPTY ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// 화면아래 라인
	WAP_Display_Line(2, WAP_DOC_HEIGHT, WAP_LCD_WIDTH - 3, WAP_DOC_HEIGHT, BLACK);

	//===========================================================================
	// 왼쪽 키 ((선택) -> multiple select
	if(g_pCurSelect->multiSelect
		&& (g_nCurStartOption <= g_nCurFocusOption && g_nCurFocusOption <= g_nCurStartOption+(MAX_WAP_LINES-1)))//2))
	{
        WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LSELECT ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
	}
	//===========================================================================
	// 중간 키 (ok확인)
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);//ok
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CONFIRM , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//확인

	//===========================================================================
	// 오른쪽 키 (취소))
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_CANCEL ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);//취

    //===========================================================================
	// 스크롤 키
	if( g_pCurSelect->pHeadFocusOption == NULL
	|| g_pCurSelect->pHeadFocusOption->pNextFocusNode == NULL)
	{
        WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
	}
	else
	{
		//가장 처음
		if(g_nCurStartOption == 0 && pCurFocusNode->pPrevFocusNode == NULL)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_DOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//가장 마지막
		else if(pCurFocusNode->pNextNode == NULL)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UP, WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//중간
		else
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
	}
}

void WAP_DisplaySoftkey_Options()
{
	char temp[20];

	extern /*ROM*/ BYTE* /*EQS_API*/ GetBitmapFontBufferByCode(WORD wCode);	//yjahn96 00.12.28

	//------------------------------------------------------------------
	// 현재의 Focus 노드로 간다
	//------------------------------------------------------------------
	WAP_Node *pCurFocusNode;
	WAP_Content_Option *pContent;
	pCurFocusNode = g_pCurSelect->pHeadFocusOption;
	while(pCurFocusNode != NULL && pCurFocusNode->type == _ELEMENT_TYPE_OPTION)
	{
		pContent = (WAP_Content_Option *)pCurFocusNode->pContent;
		if(pContent->indexOption == g_nCurFocusOption)
			break;
		pCurFocusNode = (WAP_Node *)pCurFocusNode->pNextFocusNode;
	}

	//------------------------------------------------------------------
	//clear
	strcpy(temp, "                    ");
	WAP_Display_TextOut(0, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);

	//===========================================================================
	// 왼쪽 키 ((선택) -> multiple select
	if(g_pCurSelect->multiSelect
		&& (g_nCurStartOption <= g_nCurFocusOption && g_nCurFocusOption <= g_nCurStartOption+2))
	{
		//(선택 : {5, 0x84fe, 0x84ff}
		strcpy(temp, "\5\204\376\204\377");
#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(4*2, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(4, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
#ifdef WAPCORE_FOR_WARNING
	MSG_LOW(" Dummy for WARNING in mmi_option.c",0,0,0);
#endif //WAPCORE_FOR_WARNING
	//===========================================================================
	// 중간 키 (ok확인)
	//ok확인 : {0x88bf, 0x86be, 0x86bf}
    strcpy(temp, "\210\277\206\276\206\277");
/*
    if(SKY_IS_PHONE_OPEN()) //ui.earpieceup
	    strcpy(temp, "\210\277\206\276\206\277");
	else//for A7M
*/
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(48*2, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(48, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	//===========================================================================
	// 오른쪽 키 (취소))
	//취소)  : {0x86b6, 0x86b7, 6}
	strcpy(temp, "\206\266\206\267\6");
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(86*2, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(86, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	//===========================================================================
	// 스크롤 키
	if( g_pCurSelect->pHeadFocusOption == NULL
	|| g_pCurSelect->pHeadFocusOption->pNextFocusNode == NULL)
	{
	}
	else
	{
		BYTE *pFont;
#if !((defined WAP_B1) || (defined WAP_B2))
		BYTE *pWordFont;
#endif //WAP_B1
		WORD pSoftkey_Move[3]	= {0x86b1, 0x88ed, 0x88ee};// 이동

		//가장 처음
		if(g_nCurStartOption == 0 && pCurFocusNode->pPrevFocusNode == NULL)
		{
            pFont = GetBitmapFontBufferByCode(pSoftkey_Move[1]);//down img
/*            if(SKY_IS_PHONE_OPEN())//ui.earpieceup
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[1]);//down img
			else//for A7M
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveA[1]);//down img
*/
		}
		//가장 마지막
		else if(pCurFocusNode->pNextNode == NULL)
		{
            pFont = GetBitmapFontBufferByCode(pSoftkey_Move[2]);//up img
/*
            if(SKY_IS_PHONE_OPEN())//ui.earpieceup
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[2]);//up img
			else//for A7M
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveA[2]);//up img
*/
		}
		//중간
		else
		{
            pFont = GetBitmapFontBufferByCode(pSoftkey_Move[0]);
/*            if(SKY_IS_PHONE_OPEN())//ui.earpieceup
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[0]);
			else//for A7M
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveA[0]);
*/
		}
#if !((defined WAP_B1) || (defined WAP_B2))
		pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
		WAP_OutWordCharLCDBufferHan(pWordFont, 36*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_OutWordCharLCDBufferHan(pFont, 36, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
}

void WAP_Draw_Element_Title(void)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	char temp[WAP_MAX_TEXT];
	
	//-----------------------------------------------------------
	memset(temp, '\0', WAP_MAX_TEXT);
	if(g_pCurSelect->title == NULL)
	{
		strcpy(temp, "\242\321선택");
	}
	else
	{
		char title[WAP_MAX_TEXT];
		memset(title, '\0', WAP_MAX_TEXT);
		strcpy(title, g_pCurSelect->title);
		TruncateString((BYTE *)title, 20);//20

		sprintf(temp, "\242\321%s", title);
	}
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(0, 0, temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);


	//for decoration title
	WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH-1, WAP_ASCII_FONT_HEIGHT - 2, TRUE, WAP_WHITEGRAY);
	WAP_Display_Line(0,				WAP_ASCII_FONT_HEIGHT - 1,
					 WAP_LCD_WIDTH-1, WAP_ASCII_FONT_HEIGHT - 1,
					 WAP_BLACKGRAY);
	WAP_Display_Line(WAP_LCD_WIDTH-1, 0,
					 WAP_LCD_WIDTH-1, WAP_ASCII_FONT_HEIGHT - 1,
					 WAP_BLACKGRAY);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut_Title(0, 0, temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
/*
	WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH-1, WAP_ASCII_FONT_HEIGHT - 2, TRUE, WAP_LIGHTGRAY);
	WAP_Display_Line(0,				WAP_ASCII_FONT_HEIGHT - 1,
					 WAP_LCD_WIDTH-1, WAP_ASCII_FONT_HEIGHT - 1,
					 WAP_GRAY);
	WAP_Display_Line(WAP_LCD_WIDTH-1, 0,
					 WAP_LCD_WIDTH-1, WAP_ASCII_FONT_HEIGHT - 1,
					 WAP_LIGHTGRAY);
*/
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

void WAP_Draw_Element_Option(WAP_Node *pMe)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	char temp[WAP_MAX_TEXT];
	char label[WAP_MAX_TEXT];

	WAP_Content_Option *pOption = (WAP_Content_Option *) pMe->pContent;

	if(pOption->indexOption == g_nCurFocusOption) 
		nDrawMode = WAP_DRAW_REVERSE;//reverse

	//-----------------------------------------------------------
	memset(label, '\0', WAP_MAX_TEXT);
	if(pOption->label == NULL)
	{
		sprintf(label, "옵션%d", pOption->optionId);
	}
	else
	{
		strcpy(label, pOption->label);
		TruncateString((BYTE *)label, 20-2);//18자+icon
	}

	//-----------------------------------------------------------
	memset(temp, '\0', WAP_MAX_TEXT);
	if(g_pCurSelect->multiSelect)//multiple
	{
		//select / unselect
		if(pOption->isSelectedTemp)
			sprintf(temp, "%s%s", SF_CHECK_BOX, label);
		else
			sprintf(temp, "%s%s", SF_BOX, label);
	}
	else//single
	{
		//{ 0x88cc, 0x88cd }
		if(pOption->isSelectedTemp)
			sprintf(temp, "%s%s", SF_RADIO_BUTTON_ON, label);
		else
			sprintf(temp, "%s%s", SF_RADIO_BUTTON_OFF, label);
	}

	if(nDrawMode == WAP_DRAW_REVERSE)
	{
#if !((defined WAP_B1) || (defined WAP_B2))
		//역상일 경우, 라인 전체를 반전시키기 위해 사각형을 그려준다
		WAP_Display_Rectangle(pMe->x1, 
								WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
								pMe->x2,
								WAP_ASCII_FONT_HEIGHT + pMe->y2 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT) - 1,//title + y - offset
								TRUE,//fill
								WAP_BLACK);//color	
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
/*		//역상일 경우, 라인 전체를 반전시키기 위해 사각형을 그려준다
		WAP_Display_Rectangle(pMe->x1, 
								WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
								pMe->x2,
								WAP_ASCII_FONT_HEIGHT + pMe->y2 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT) - 1,//title + y - offset
								TRUE,//fill
								WAP_WHITE);//color
*/
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1 * 2,//for double buffering
						WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut_Focus(pMe->x1 ,
						WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

void WAP_Draw_Element_OptionGroup(WAP_Node *pMe)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	
	char temp[WAP_MAX_TEXT];

	WAP_Content_OptionGroup *pOptionGroup = (WAP_Content_OptionGroup *) pMe->pContent;

	memset(temp, '\0', WAP_MAX_TEXT);
	if(pOptionGroup->label == NULL)
	{
		strcpy(temp, "[그룹]");
	}
	else
	{
		char trunc[WAP_MAX_TEXT];
		memset(trunc, '\0', WAP_MAX_TEXT);
		strcpy(trunc, pOptionGroup->label);
		TruncateString((BYTE *)trunc, 19-1);//19자+괄호

		sprintf(temp, "[%s]", trunc);
	}
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1 * 2,//for double buffering
						WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1,
						WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

void WAP_Select_Options()//선택
{
	WAP_Node *pCurNode;
	WAP_Content_Option *pContent;

	pCurNode = g_pCurSelect->pHeadFocusOption;
	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_OPTION)
		{
			pContent = (WAP_Content_Option *)pCurNode->pContent; 
			
			if(pContent->indexOption == g_nCurFocusOption)
			{
				// 선택 상황을 반전해 준다
				pContent->isSelectedTemp = ! pContent->isSelectedTemp;
			}
			else 
			{
				// Select가 single 선택의 경우
				if(!g_pCurSelect->multiSelect)
				{
					// 다른 모든 Option은 선택을 해제한다
					pContent->isSelectedTemp = FALSE;
				}
			}
		}
		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}

	//화면을 다시 그려준다
	WAP_Display_Options();
}

void WAP_Scrolling_Options(BOOL bDown)
{
	WAP_Node *pCurFocusNode;
	WAP_Content_Option *pContent;
	
	BOOL bChangedFocus = FALSE;

	//------------------------------------------------------------------
	// 현재 Select 내에 Focus 노드가 하나도 없는 경우, focus 이동 실패
	//------------------------------------------------------------------
    if(g_pCurSelect->pHeadFocusOption == NULL) return;

	//------------------------------------------------------------------
	// 현재의 Focus 노드로 간다
	//------------------------------------------------------------------
	pCurFocusNode = g_pCurSelect->pHeadFocusOption;
	while(pCurFocusNode != NULL && pCurFocusNode->type == _ELEMENT_TYPE_OPTION)
	{
		pContent = (WAP_Content_Option *)pCurFocusNode->pContent;
		if(pContent->indexOption == g_nCurFocusOption)
			break;
		pCurFocusNode = (WAP_Node *)pCurFocusNode->pNextFocusNode;
	}

	//==================================================================
	// Scrolling 실패
	//------------------------------------------------------------------
	if(pCurFocusNode == NULL) return;

	//==================================================================
	// Scrolling 성공
	//------------------------------------------------------------------
	if(bDown)
	{
		int nNextFocusIndex = ((WAP_Content_Option *)pCurFocusNode->pNextFocusNode->pContent)->indexOption;

		//===============================================
		//가장 마지막
		if(pCurFocusNode->pNextNode == NULL) return;
		
		//===============================================
		//같은 화면 -> Focus 이동
		if( g_nCurStartOption <= nNextFocusIndex
		 && nNextFocusIndex <= g_nCurStartOption+(MAX_WAP_LINES-1))
		{
			bChangedFocus = TRUE;
			g_nCurFocusOption = nNextFocusIndex;
		}
		//===============================================
		//다음 화면 -> Window 이동
		else
		{
			g_nCurStartOption++;

//			if(!(  g_nCurStartOption <= g_nCurFocusOption
//				&& g_nCurFocusOption <= g_nCurStartOption+(MAX_WAP_LINES-1)))
            if(nNextFocusIndex <= g_nCurStartOption+(MAX_WAP_LINES-1))
			{
				bChangedFocus = TRUE;
				g_nCurFocusOption = nNextFocusIndex;
			}
		}
	}
	else//up
	{
		int nPrevFocusIndex = ((WAP_Content_Option *)pCurFocusNode->pPrevFocusNode->pContent)->indexOption;

		//===============================================
		//가장 처음
		if(g_nCurStartOption == 0 
		&& pCurFocusNode->pPrevFocusNode == NULL) return;

		//===============================================
		//같은 화면 -> Focus 이동
		if( g_nCurStartOption <= nPrevFocusIndex
		 && nPrevFocusIndex <= g_nCurStartOption+(MAX_WAP_LINES-1))
		{
			bChangedFocus = TRUE;
			g_nCurFocusOption = nPrevFocusIndex;
		}
		//===============================================
		//다음 화면 -> Window 이동
		else
		{
			g_nCurStartOption--;

//			if(!(  g_nCurStartOption <= g_nCurFocusOption
//				&& g_nCurFocusOption <= g_nCurStartOption+(MAX_WAP_LINES-1)))
            if(nPrevFocusIndex == g_nCurStartOption)
			{
				bChangedFocus = TRUE;
				g_nCurFocusOption = nPrevFocusIndex;
			}
		}
	}

	//===============================================
	// Single Select인 경우, Scroll과 동시에 반전한다
	if(!g_pCurSelect->multiSelect && bChangedFocus)//and display
		WAP_Select_Options();
	else
		WAP_Display_Options();
}

void WAP_Handle_LeftKey_Options()
{
	//선택
	if(g_pCurSelect->multiSelect)
	{
		WAP_Select_Options();
	}
}

void WAP_Handle_RightKey_Options()
{
	//취소
	g_nViewMode = WAP_VIEW_MODE_WML;
	WAP_Display();
    MSG_HIGH("",0,0,0);
}

void WAP_Handle_CenterKey_Options()//확인
{
    INT16 length,index = 0;
	INT16 selectcase;//1;single 2:multi

	UINT8 saveoption[100];

	WAP_Node *pCurNode;
	WAP_Content_Option *pContent;

	//clear
    length = 0;
	selectcase = 0;
	memset(saveoption,0x00,100);

	memset(g_pCurSelect->selectedOptions, '\0', 255);
    
	pCurNode = g_pCurSelect->pHeadFocusOption;
	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_OPTION)
		{
			pContent = (WAP_Content_Option *)pCurNode->pContent; 

			//====================================================
			// Multiple Selection
			//====================================================
			if(g_pCurSelect->multiSelect)
			{
				//선택값에 변화가 있는 경우
				if(pContent->isSelected != pContent->isSelectedTemp)
				{
					//임시값을 실제값에 저장한다
					pContent->isSelected = pContent->isSelectedTemp;
                    selectcase = 2;
					saveoption[length++] = pContent->optionId;

					// AUR 브라우져 에게 알린다(select/deselect)
					//SKY_optionSelected(pContent->viewId, pContent->optionId);
				}
			}
			//====================================================
			// Single Selection
			//====================================================
			else
			{
				//임시값을 실제값에 저장한다
				pContent->isSelected = pContent->isSelectedTemp;

				// AUR 브라우져 에게 알린다
				if(pContent->isSelected)
				{
                    selectcase = 1;
					saveoption[0] = pContent->optionId;

					//SKY_optionSelected(pContent->viewId, pContent->optionId);
				}
#ifndef WAP_USE_SECURE
				CLNTa_log(1,0,"FOR_MS REGION in mmi_option.c");
				if(Wtlshello)
				{

					enc_use.viewid  = pContent->viewId;
					enc_use.id      = pContent->optionId;
					enc_use.wmlkind = 2;

					wtls_session = TRUE;
					CLNTa_log(1,0,"wtls session =TRUE");
				}
				else
#endif//#ifndef WAP_USE_SECURE
				{
#ifdef WAP_USE_DORM		
					normal_use.viewid  = pContent->viewId;
					normal_use.id      = pContent->optionId;
					normal_use.wmlkind = 2;
#endif//#ifdef WAP_USE_DORM
				}
			}

			if(pContent->isSelected)
			{
				if(optionselect == FALSE)
					optionselect = TRUE;

				if(g_pCurSelect->multiSelect)
				{
					if(strlen(g_pCurSelect->selectedOptions) == 0)
						strcpy(g_pCurSelect->selectedOptions, pContent->label);
					else 
						sprintf(g_pCurSelect->selectedOptions, "%s,%s",
								g_pCurSelect->selectedOptions, pContent->label);
				}
				else
				{
					strcpy(g_pCurSelect->selectedOptions, pContent->label);
					//break;
				}
			}
		}//end of if(pContent->type == _ELEMENT_TYPE_OPTION)

		pCurNode = (WAP_Node *)pCurNode->pNextFocusNode;
	
	}//end of while(pCurNode != NULL)
	
	//------------------------------------------------------------
	// WML 화면(화면 전환)
	g_nViewMode = WAP_VIEW_MODE_WML;
	WAP_Formatting();
	WAP_Display();

    if(selectcase == 1)
	{
		SKY_optionSelected(1,saveoption[0]);
	}
	else if(selectcase == 2)
	{
		for(index = 0;index < length ;index++)
		{
			SKY_optionSelected(1,saveoption[index]);
		}

	}
}