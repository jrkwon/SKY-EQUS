//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
//#include "custtrex.h"
//#include "custA7.h"
#include "customer.h"  // jwpark 00.04.27
#include <stdio.h>
//////////////////////////////////////////////////

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

#include <string.h>
#include <stdlib.h>

#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"

#include "aapiclnt.h"
#include "msg.h"

// for Queue
#include "queue.h"

//General Display Library
#include "GenDsp.h"
#include "clnt.h"
#include "sisdecoder.h"
#include "wapconnector.h" //yjahn96 01.04.
#ifdef  FEATURE_SKT_EIF
#include  "ska_call.h"
#endif  //#ifdef  FEATURE_SKT_EIF

//-----------------------------------------------------------------------------------
//
// MMI 의 Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"

//===================================================================================
//
// MMI의 Variable
//
//===================================================================================
WAP_Card *g_pCard = NULL;					// 현재 Parsing되어 Display 중인 카드에 대한 포인터

BOOL g_bBlocked = FALSE;					// 전체 Blocked 상황 (by MMIa_wait())

int g_nLCDOffset = 0;						// 현재 화면(Current Window)의 y 좌표(Offset)

int	g_nViewMode		= WAP_VIEW_MODE_WML;		// LCD 화면 모드
int	g_nMenuMode		= WAP_MENU_MODE_NONE;		// WAP Main Menu 모드
int	g_nDlgMode		= WAP_DLG_MODE_NONE;		// WAP 대화상자 모드
int	g_nGraphicMode	= WAP_GRAPHIC_MODE_NONE;	// WAP Script의 Graphic 모드

WAP_Node *g_pCurFocusNode = NULL;			// 현재 화면에서 Focus를 가진 노드로 First와 Last 사이에 존재한다

// for 옵션 화면용
WAP_Content_Select *g_pCurSelect = NULL;

// for 입력 화면용
WAP_Content_Input *g_pCurInput = NULL;

//for [상위]키 용
WAP_Content_Key *g_pKeyUp = NULL;

//for 테이블
int nDrawLinePosXForTable = 0;

//===================================================================================
//
// MMI의 Global Variable
//
//-----------------------------------------------------------------------------------

//jjkyg for FOR_MS 12/6

#ifdef FOR_MS
	storeWML enc_use;
	extern BOOL Wtlshello;
#endif

#if defined(WAP_USE_DORM) || defined(WAP_USE_SKT_NEW) 
	storeWML normal_use;
#endif//#ifdef WAP_USE_DORM //WAP_USE_SKT_NEW 

extern void CancelAnimationTimer(void); //extern void far ui_sistimer_set (int4  interval, boolean repeat);

#ifdef WAP_NOWRAP	//yjahn96
	extern void CancelNowrapTimer(void);
	extern void RestartNowrapTimer(unsigned long ms);
#endif//#ifdef WAP_NOWRAP

////////////////////////////////////////////////////////////////////
boolean Check_Play_SIS_Animation(void); //yjahn96 01.04.10
extern void CancelAnimationTimer(void);
extern void RestartAnimationTimer(unsigned long ms);
////////////////////////////////////////////////////////////////////

//for Softkeys -- yjahn96 01.01.04
extern WORD WAP_OutWordCharLCDBufferHan(BYTE *pFont, int nPosX, int nPosY, int nFontWidth, int nFontHeight, int nDrawMode);
extern BYTE *Widening(BYTE *pFont, BYTE nBytePerChar);

#ifdef WAP_USE_LEAK
BOOL WAP_Terminate_WML_Test(void)
{
	WAP_Node *pCurNode, *pTempNode;
	if(g_pCard == NULL) return FALSE;
	
	//---------------------------------------------------------------------------
	//관련 Timer를 종료한다
    CancelAnimationTimer();	//stop sis animation
#ifdef WAP_NOWRAP	//yjahn96
    CancelNowrapTimer();//stop nowrap
#endif//#ifdef WAP_NOWRAP

	if(g_pKeyUp) g_pKeyUp = NULL;

	g_pCard->pHeadContent = NULL;

	//=====================================================================================
	// 2. 현재의 Card와 관련 데이타를 지운다
	//=====================================================================================
	if(g_pCard != NULL)
	{
		if(g_pCard->title != NULL)	
		{
			g_pCard->title = NULL;
		}
		if(g_pCard->URL != NULL)
		{
			g_pCard->URL = NULL;
		}
		if(g_pCard->history != NULL)
		{
			g_pCard->history = NULL;
		}
	}
	g_pCard = NULL; // 초기화
	return TRUE;
}
#endif //WAP_USE_LEAK

BOOL WAP_Terminate_WML(void)
{
	WAP_Node *pCurNode, *pTempNode;
	if(g_pCard == NULL) return FALSE;
	
	//---------------------------------------------------------------------------
	//관련 Timer를 종료한다
	CancelAnimationTimer(); //ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96
	CancelNowrapTimer();    //	ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP

	//---------------------------------------------------------------------------
	//[상위]키가 존재할 경우 이를 지운다
	if(g_pKeyUp)
	{
		if(g_pKeyUp->eventType != NULL)		wip_free((void *)g_pKeyUp->eventType);
		if(g_pKeyUp->label != NULL)			wip_free((void *)g_pKeyUp->label);

		wip_free((void *)g_pKeyUp);
		g_pKeyUp = NULL;
	}
	
	//=====================================================================================
	// 1. 각 노드와 관련 데이타를 지운다
	//=====================================================================================
	pCurNode = g_pCard->pHeadContent;

#ifdef DOG_GABGA
	g_pCard->pHeadContent = NULL;
#endif//#ifdef DOG_GABGA

	while(pCurNode != NULL)
	{
		//===========================================================
		// 1-1. 우선 각 노드의 컨텐트를 먼저 지운다
		//===========================================================
		switch(pCurNode->type)
		{
			case _ELEMENT_TYPE_KEY: 
			{
				WAP_Content_Key *pContent = (WAP_Content_Key *)(pCurNode->pContent); 
			#ifdef DOG_GABGA
				pCurNode->pContent = NULL; 
			#endif//#ifdef DOG_GABGA
				if(pContent != NULL)
				{
					if(pContent->eventType != NULL)		wip_free((void *)pContent->eventType);
					if(pContent->label != NULL)			wip_free((void *)pContent->label);

					wip_free((void *)pContent);
					
			#ifdef DOG_GABGA
					pContent = NULL;
			#endif//#ifdef DOG_GABGA
				}
				break;
			}
			
			case _ELEMENT_TYPE_TEXT : 
			case _ELEMENT_TYPE_LINK_TEXT:
			{
				WAP_Content_Text *pContent = (WAP_Content_Text *)pCurNode->pContent; 
			#ifdef DOG_GABGA
				pCurNode->pContent = NULL;			
			#endif//#ifdef DOG_GABGA

				if(pContent != NULL)
				{
					if(pContent->text != NULL)			wip_free((void *)pContent->text);
					if(pContent->linkTitle != NULL)		wip_free((void *)pContent->linkTitle);

					wip_free((void *)pContent);
			#ifdef DOG_GABGA
					pContent = NULL;
			#endif//#ifdef DOG_GABGA			
				}
				break;
			}
			
			case _ELEMENT_TYPE_IMAGE: 
			case _ELEMENT_TYPE_LINK_IMAGE:
			{
				WAP_Content_Image *pContent = (WAP_Content_Image *)pCurNode->pContent; 
			#ifdef DOG_GABGA
				pCurNode->pContent = NULL; 
			#endif//#ifdef DOG_GABGA

				if(pContent != NULL)
				{
#if !((defined WAP_B1) || (defined WAP_B2))
					if(pContent->imageData != NULL)	wip_free((void *)pContent->imageData);
					if(pContent->altText != NULL)	wip_free((void *)pContent->altText);
#else	//#if !((defined WAP_B1) || (defined WAP_B2))
					//---------------------------------------------------
					// B1 project에서는 color SIS를 지원하여, imageData에 버퍼를 바로 할당함
					// 따라서, image Type이 SIS인 경우는 wip_free()를 하지 않는다.
					if(pContent->imageType != WAP_IMAGE_TYPE_SIS)
					{
						if(pContent->imageData != NULL)	wip_free((void *)pContent->imageData);
					}
					if(pContent->altText != NULL)	wip_free((void *)pContent->altText);
#endif	//#if !((defined WAP_B1) || (defined WAP_B2))

					wip_free((void *)pContent);
			#ifdef DOG_GABGA
					pContent = NULL;
			#endif//#ifdef DOG_GABGA
				}
				break;
			}

			case _ELEMENT_TYPE_SELECT: 
			{
				WAP_Content_Select *pContent = (WAP_Content_Select *)pCurNode->pContent; 
			#ifdef DOG_GABGA
				pCurNode->pContent = NULL; 
			#endif//#ifdef DOG_GABGA
				if(pContent != NULL)
				{
					WAP_Node *pOptionNode;

					//Select에 달린 Option과 OptionGroup을 먼저 처리한다
					pOptionNode = pContent->pHeadFocusOption;
			#ifdef DOG_GABGA
					pContent->pHeadFocusOption = NULL;
			#endif//#ifdef DOG_GABGA

					while(pOptionNode != NULL)
					{
						switch(pOptionNode->type)
						{
							case _ELEMENT_TYPE_OPTION: 
							{
								WAP_Content_Option *pOptionContent = (WAP_Content_Option *)pOptionNode->pContent;
							#ifdef DOG_GABGA
								pOptionNode->pContent = NULL;
							#endif//#ifdef DOG_GABGA
								if(pOptionContent != NULL)
								{
									if(pOptionContent->label != NULL)		wip_free((void *)pOptionContent->label);
									if(pOptionContent->title != NULL)		wip_free((void *)pOptionContent->title);
									wip_free((void *)pOptionContent);
							#ifdef DOG_GABGA
									pOptionContent = NULL;
							#endif//#ifdef DOG_GABGA
								}
								break;
							}
							case _ELEMENT_TYPE_OPTIONGROUP: 
							{
								WAP_Content_OptionGroup *pOptionContent	= (WAP_Content_OptionGroup *)pOptionNode->pContent;
							#ifdef DOG_GABGA
								pOptionNode->pContent=NULL;
							#endif//#ifdef DOG_GABGA
								if(pOptionContent != NULL)
								{
									if(pOptionContent->label != NULL)	wip_free((void *)pOptionContent->label);
									wip_free((void *)pOptionContent);
							#ifdef DOG_GABGA
									pOptionContent = NULL;
							#endif//#ifdef DOG_GABGA
								}
								break;
							}
						}//switch -> select

						pTempNode = pOptionNode;
						pOptionNode = (WAP_Node *)pOptionNode->pNextNode;
						if(pTempNode != NULL) wip_free((void *)pTempNode);
				#ifdef DOG_GABGA
						pTempNode = NULL;
				#endif//#ifdef DOG_GABGA
					}//while -> select

					if(pContent->title != NULL)	wip_free((void *)(pContent->title));
					wip_free((void *)pContent);
			#ifdef DOG_GABGA
					pOptionNode = NULL;
			#endif//#ifdef DOG_GABGA
				}
			#ifdef DOG_GABGA
				pContent = NULL;
			#endif//#ifdef DOG_GABGA
				break;
			}

			case _ELEMENT_TYPE_INPUT: 
			{
				WAP_Content_Input *pContent = (WAP_Content_Input *)pCurNode->pContent; 
				if(pContent != NULL)
				{
					if(pContent->title != NULL)			wip_free((void *)pContent->title);
					if(pContent->format != NULL)		wip_free((void *)pContent->format);
					
					wip_free((void *)pContent);
			#ifdef DOG_GABGA
					pContent = NULL;
			#endif//#ifdef DOG_GABGA
				}
				break;
			}

			case _ELEMENT_TYPE_TABLE_ROW:
			{
				extern BOOL bTable;
				WAP_Content_TableRow *pContent = (WAP_Content_TableRow *)pCurNode->pContent; 
				bTable = FALSE;//init
			#ifdef DOG_GABGA
				pCurNode->pContent = NULL; 
			#endif//#ifdef DOG_GABGA
				if(pContent != NULL)
				{
					WAP_Node *pTableNode;

					//Table에 달린 Table Data을 먼저 처리한다
					pTableNode = (WAP_Node *)pContent->pHeadCell;
			#ifdef DOG_GABGA
					pContent->pHeadCell = NULL;
			#endif//#ifdef DOG_GABGA
					while(pTableNode != NULL)
					{
						switch(pTableNode->type)
						{
							case _ELEMENT_TYPE_TEXT : 
							case _ELEMENT_TYPE_LINK_TEXT:
							{
								WAP_Content_Text *pTableContent = (WAP_Content_Text *)pTableNode->pContent; 
						#ifdef DOG_GABGA
								pTableNode->pContent = NULL; 
						#endif//#ifdef DOG_GABGA
								if(pTableContent != NULL)
								{
									if(pTableContent->text != NULL)		wip_free((void *)pTableContent->text);
									if(pTableContent->linkTitle != NULL)wip_free((void *)pTableContent->linkTitle);

									wip_free((void *)pTableContent);
						#ifdef DOG_GABGA
 									pTableContent = NULL;
						#endif//#ifdef DOG_GABGA
								}
								break;
							}
							case _ELEMENT_TYPE_IMAGE: 
							case _ELEMENT_TYPE_LINK_IMAGE:
							{
								WAP_Content_Image *pTableContent = (WAP_Content_Image *)pTableNode->pContent; 
						#ifdef DOG_GABGA
								pTableNode->pContent = NULL; 
						#endif//#ifdef DOG_GABGA								
								if(pTableContent != NULL)
								{
#if !((defined WAP_B1) || (defined WAP_B2))
									if(pTableContent->imageData != NULL)wip_free((void *)pTableContent->imageData);
									if(pTableContent->altText != NULL)	wip_free((void *)pTableContent->altText);
#else	//#if !((defined WAP_B1) || (defined WAP_B2))
									//---------------------------------------------------
									// B1 project에서는 color SIS를 지원하여, imageData에 버퍼를 바로 할당함
									// 따라서, image Type이 SIS인 경우는 wip_free()를 하지 않는다.
									if(pTableContent->imageType != WAP_IMAGE_TYPE_SIS)
									{
										if(pTableContent->imageData != NULL)wip_free((void *)pTableContent->imageData);
									}
									if(pTableContent->altText != NULL)	wip_free((void *)pTableContent->altText);
#endif	//#if !((defined WAP_B1) || (defined WAP_B2))

									wip_free((void *)pTableContent);
						#ifdef DOG_GABGA
									pTableContent = NULL;
						#endif//#ifdef DOG_GABGA								
								}
								break;
							}
						}//switch -> Table

						pTempNode = pTableNode;
						pTableNode = (WAP_Node *)pTableNode->pNextNode;
						if(pTempNode != NULL) wip_free((void *)pTempNode);
				#ifdef DOG_GABGA
						pTempNode = NULL;
				#endif//#ifdef DOG_GABGA
					}//while -> Table
			#ifdef DOG_GABGA
					pTableNode = NULL;
			#endif//#ifdef DOG_GABGA

					wip_free((void *)pContent);
			#ifdef DOG_GABGA
					pContent = NULL;
			#endif//#ifdef DOG_GABGA
				}
				break;
			}//Table

		}// end of switch

		//===========================================================
		// 1-2. 다음은 각 노드를 지운다
		//===========================================================
		pTempNode = pCurNode;
		pCurNode = (WAP_Node *)pCurNode->pNextNode;

	#ifdef DOG_GABGA
		(WAP_Node *)pCurNode->pNextNode = NULL;
	#endif//#ifdef DOG_GABGA

		if(pTempNode != NULL) 
		{
			wip_free((void *)pTempNode);
	#ifdef DOG_GABGA
			pTempNode = NULL;
	#endif//#ifdef DOG_GABGA				
		}
	}// end of while
#ifdef DOG_GABGA
	pCurNode = NULL;
#endif//#ifdef DOG_GABGA

	g_pCard->pHeadContent = NULL;

	//=====================================================================================
	// 2. 현재의 Card와 관련 데이타를 지운다
	//=====================================================================================
	if(g_pCard != NULL)
	{
		if(g_pCard->title != NULL)
		{
			wip_free((void *)(g_pCard->title));
			g_pCard->title = NULL;
		}
		if(g_pCard->URL != NULL)
		{
			wip_free((void *)(g_pCard->URL));
			g_pCard->URL = NULL;
		}
		if(g_pCard->history != NULL)
		{
			wip_free((void *)(g_pCard->history));
			g_pCard->history = NULL;
		}
		wip_free((void *)g_pCard);
	}
	
	g_pCard = NULL; // 초기화
	return TRUE;
}

extern WAP_Content_TableRow *g_pCurTableRow;
extern boolean browserTerComp;
void WAP_Building(UINT8 type, void *pContent, int nFontType)
{
	WAP_Node *pTempNode;
	WAP_Node *pNewNode; 

	if(browserTerComp) return;
	
	//------------------------------------------------------------------
	// 새로운 노드를 하나 만든다
	
	pNewNode = (WAP_Node *)wip_malloc(sizeof(WAP_Node));

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
	pNewNode->font.font_index = nFontType;

    if(nFontType == WAP_FONT_MEDIUM)
	{
		pNewNode->font.height	= WAP_MED_ASCII_FONT_HEIGHT;
		pNewNode->font.width	= WAP_MED_ASCII_FONT_WIDTH;
	}
	else
	{
		pNewNode->font.height	= WAP_ASCII_FONT_HEIGHT;
		pNewNode->font.width	= WAP_ASCII_FONT_WIDTH;
	}
	
	//------------------------------------------------------------------
	// 새 노드를 리스트(pHeadContent)의 마지막에 추가한다
	if(g_pCard->pHeadContent == NULL)
	{
		g_pCard->pHeadContent = pNewNode;
	}
	else
	{
		pTempNode = g_pCard->pHeadContent;
    
		while(pTempNode->pNextNode != NULL)
		{
			pTempNode = pTempNode->pNextNode;
		}

		pTempNode->pNextNode = pNewNode;
		pNewNode->pPrevNode = pTempNode;
	}

	//------------------------------------------------------------------
	// 새 노드가 선택 가능한(Focus 가능) 노드라면,
	// 새 노드를 선택 리스트(pHeadFocus)의 마지막에 추가한다
	
	if(pNewNode->type == _ELEMENT_TYPE_KEY
	|| pNewNode->type == _ELEMENT_TYPE_INPUT
	|| pNewNode->type == _ELEMENT_TYPE_SELECT
	|| pNewNode->type == _ELEMENT_TYPE_LINK_TEXT
	|| pNewNode->type == _ELEMENT_TYPE_LINK_IMAGE
	//TableRow는 Focus 가능한 Element를 가질 수 있으므로 선택가능 리스트에 포함한다.
	//Formatting시, Focus 가능한 Element가 없을 경우 이 리스트에서 제거할 것이다
	|| pNewNode->type == _ELEMENT_TYPE_TABLE_ROW)//선택가능한 type인지 체크
	{
		if(g_pCard->pHeadFocus == NULL)
		{
			g_pCard->pHeadFocus = pNewNode;
			g_pCurFocusNode = g_pCard->pHeadFocus;
		}
		else
		{
			pTempNode = g_pCard->pHeadFocus;

		    while(pTempNode->pNextFocusNode != NULL) 
			{
				pTempNode = pTempNode->pNextFocusNode;
			}
		
			pTempNode->pNextFocusNode = pNewNode;
			pNewNode->pPrevFocusNode = pTempNode;
		}
	}
}

void WAP_Building_Table(int type, void *pContent, int nFontType)
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
	pNewNode->font.font_index = nFontType;
	if(nFontType == WAP_FONT_MEDIUM)
	{
		pNewNode->font.height	= WAP_MED_ASCII_FONT_HEIGHT;
		pNewNode->font.width	= WAP_MED_ASCII_FONT_WIDTH;
	}
	else
	{
		pNewNode->font.height	= WAP_ASCII_FONT_HEIGHT;
		pNewNode->font.width	= WAP_ASCII_FONT_WIDTH;
	}

    //------------------------------------------------------------------
	// 새 노드를 리스트(g_pCurTableRow )의 마지막에 추가한다
	if(g_pCurTableRow->pHeadCell == NULL)
	{
		g_pCurTableRow->pHeadCell = pNewNode;
	}
	else
	{
		pTempNode = g_pCurTableRow->pHeadCell;
    
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
	if(pNewNode->type == _ELEMENT_TYPE_LINK_TEXT
	|| pNewNode->type == _ELEMENT_TYPE_LINK_IMAGE)
	{
		if(g_pCurTableRow->pHeadFocusCell == NULL)
		{
			g_pCurTableRow->pHeadFocusCell = pNewNode;
		}
		else
		{
			pTempNode = g_pCurTableRow->pHeadFocusCell;
		
		    while(pTempNode->pNextFocusNode != NULL) 
			{
				pTempNode = (WAP_Node *)pTempNode->pNextFocusNode;
			}
		
			pTempNode->pNextFocusNode = pNewNode;
			pNewNode->pPrevFocusNode = pTempNode;
		}
	}
}

int WAP_Formatting_TableRow(WAP_Node* pTableNode)
{
	int nTemp = 0;
	int nMaxHeight = WAP_ASCII_FONT_HEIGHT;//1 라인(13)
	
	WAP_Node *pCurNode;
	WAP_Content_TableRow* pTableRow = (WAP_Content_TableRow *)pTableNode->pContent;
		
	//-----------------------------------------------------------------------------------
	// 현재의 Table Row의 Focus Cell을 HeadFocus로 초기화한다
	//-----------------------------------------------------------------------------------
	pTableRow->pFocusCell = pTableRow->pHeadFocusCell;
	
	//-----------------------------------------------------------------------------------
	// 현재의 Table Row의 Column 중 가장 긴 Height 를 구한다
	//-----------------------------------------------------------------------------------
	pCurNode = (WAP_Node *)pTableRow->pHeadCell;
	while(pCurNode != NULL)
	{
		switch(pCurNode->type)
		{
			case _ELEMENT_TYPE_TEXT : 
			case _ELEMENT_TYPE_LINK_TEXT:
			{
				point pt;
				char* pDisplayedTextData;
				//WAP_Content_Text *pTableContent = (WAP_Content_Text *)pCurNode->pContent; 
				pDisplayedTextData = GetCurDisplayedData(pCurNode);
				pt = Calculate_Node_EndPoint(pDisplayedTextData, 0, 0,
											pCurNode->font.font_index, pTableRow->noOfColumns);
				nTemp = pt.y;
				break;
			}
			case _ELEMENT_TYPE_IMAGE: 
			case _ELEMENT_TYPE_LINK_IMAGE:
			{
				WAP_Content_Image *pTableContent = (WAP_Content_Image *)pCurNode->pContent; 

				if(pTableContent->imageData != NULL 
				&& (   pTableContent->imageType == WAP_IMAGE_TYPE_BMP
					|| pTableContent->imageType == WAP_IMAGE_TYPE_WBMP))
				{
					nTemp = pTableContent->height;
				}
				else//이미지 데이타가 없거나 처리할 수 없는 경우,Alt Text의 height로 처리한다
				{
					point pt;
					pt = Calculate_Node_EndPoint(pTableContent->altText, 0, 0,
												WAP_FONT_NORMAL, pTableRow->noOfColumns);
					nTemp = pt.y;
				}

				//이미지의 높이를 13의 배수로 맞춰준다
				if(nTemp % WAP_ASCII_FONT_HEIGHT != 0)
					nTemp += (WAP_ASCII_FONT_HEIGHT - (nTemp%WAP_ASCII_FONT_HEIGHT));

				break;
			}
			default :
			{
				nTemp = 0;
				break;
			}

		}//switch

		if(nTemp > nMaxHeight) nMaxHeight = nTemp;
		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}

	//-----------------------------------------------------------------------------------
	// 각 Column의 Node를 Format 한다
	//-----------------------------------------------------------------------------------
	//pTableRow = (WAP_Content_TableRow *)pTableNode->pContent;
	pCurNode = (WAP_Node *)pTableRow->pHeadCell;
	while(pCurNode != NULL)
	{
		int nCurColumnIndex = 0;
		switch(pCurNode->type)
		{
			case _ELEMENT_TYPE_TEXT : 
			case _ELEMENT_TYPE_LINK_TEXT:
			{
				WAP_Content_Text *pTableContent = (WAP_Content_Text *)pCurNode->pContent; 
				nCurColumnIndex = pTableContent->nTableColumn;
								
				break;
			}
			case _ELEMENT_TYPE_IMAGE: 
			case _ELEMENT_TYPE_LINK_IMAGE:
			{
				WAP_Content_Image *pTableContent = (WAP_Content_Image *)pCurNode->pContent; 
				nCurColumnIndex = pTableContent->nTableColumn;

				break;
			}
		}//switch

		pCurNode->x1 = (WAP_LCD_WIDTH / pTableRow->noOfColumns) * (nCurColumnIndex-1);
		pCurNode->x2 = (WAP_LCD_WIDTH / pTableRow->noOfColumns) * nCurColumnIndex;
		if(pCurNode->x2 == WAP_LCD_WIDTH) pCurNode->x2 = pCurNode->x2-1;

		pCurNode->y1 = pTableNode->y1;
		pCurNode->y2 = pTableNode->y1 + nMaxHeight;

		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}

	return nMaxHeight;
}

void WAP_Formatting(void)
{
	WAP_Node *pCurNode, *pNextNode;
	void *pContent;
	point pt;
	char* pDisplayedTextData;
    int temp;

	pCurNode = g_pCard->pHeadContent;

	//init first node
	pCurNode->x1 = 0;
	pCurNode->y1 = 0;

	while ( pCurNode != NULL )
	{
		pDisplayedTextData = GetCurDisplayedData(pCurNode);

		//------------------------------------------------------------------
		// 현 노드의 x2, y2 좌표 결정
		// 현 노드가 Line Break의 특성을 가지면, (x2<-WAP_LCD_WIDTH)
		//------------------------------------------------------------------ 
		if(pCurNode->type == _ELEMENT_TYPE_CARD)
		{
			//Card는 (0, 0)에서 시작한다
			if(pCurNode->x1 != 0)
			{
				pCurNode->x1 = 0;
				pCurNode->y1 = 0;
			}
			//Card의 Title이 없으면 화면에 그려주지 않는다
			//즉 Formatting을 할 필요도 없다
			if(g_pCard->title == NULL)
			{
				pCurNode->x2 = 0;
				pCurNode->y2 = pCurNode->y1 + pCurNode->font.height;
			}
			else
			{
				//Card는 (95, 13)에서 끝난다. 즉 한 줄 내에 표시된다
				pCurNode->x2 = WAP_LCD_WIDTH-1;//95
				pCurNode->y2 = pCurNode->y1 + pCurNode->font.height;
			}
		}
		else if(pCurNode->type == _ELEMENT_TYPE_KEY)
		{
			//Key는 새 라인에서 시작한다
			if(pCurNode->x1 != 0)
			{
				pCurNode->x1 = 0;
				pCurNode->y1 = pCurNode->y1 + pCurNode->font.height;
			}

			pCurNode->x2 = WAP_LCD_WIDTH-1;//95
			pCurNode->y2 = pCurNode->y1 + pCurNode->font.height;
		}
		else if(pCurNode->type == _ELEMENT_TYPE_IMAGE
			 || pCurNode->type == _ELEMENT_TYPE_LINK_IMAGE)
		{
			WAP_Content_Image *pImage = (WAP_Content_Image *)pCurNode->pContent;

			//현재의 한 라인내에 표시되는 작은 이미지의 경우, 현재 라인에서 표현해 준다
			if(pImage->height <= WAP_ASCII_FONT_HEIGHT)//inline image
			{
				if(pCurNode->x1 + pImage->width > WAP_LCD_WIDTH)
				{
					pCurNode->x1 = 0;
					pCurNode->y1 = ((WAP_Node *)pCurNode->pPrevNode)->y2;
				}
#if !((defined WAP_B1) || (defined WAP_B2))
				pCurNode->x2 = pCurNode->x1 + pImage->width * 2;//for double buffering
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
				pCurNode->x2 = pCurNode->x1 + pImage->width;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
				pCurNode->y2 = pCurNode->y1 + WAP_ASCII_FONT_HEIGHT;
				
				//이미지의 넓이를 6의 배수로 맞춰준다
				if(((pImage->width)%WAP_ASCII_FONT_WIDTH) != 0)
					pCurNode->x2 += (WAP_ASCII_FONT_WIDTH - ((pImage->width)%WAP_ASCII_FONT_WIDTH));
			}
			else
			{
				//Image는 새 라인에서 시작한다
				if(pCurNode->x1 != 0)
				{
					pCurNode->x1 = 0;
					pCurNode->y1 = ((WAP_Node *)pCurNode->pPrevNode)->y2;
				}
		

				pCurNode->x2 = WAP_LCD_WIDTH-1;//95

                //이미지가 LCD height(120)보다 큰 경우, y2값 조절
                if(pImage->height <= 117) //13*9
                    pCurNode->y2 = pCurNode->y1 + pImage->height;
                else
                    pCurNode->y2 = pCurNode->y1 + 117;

				//이미지의 높이를 13의 배수로 맞춰준다
                if(((pImage->height)%WAP_ASCII_FONT_HEIGHT) != 0)
                {
//                    pCurNode->y2 += (WAP_ASCII_FONT_HEIGHT - ((pImage->height)%WAP_ASCII_FONT_HEIGHT));
                    temp = (WAP_ASCII_FONT_HEIGHT - ((pImage->height)%WAP_ASCII_FONT_HEIGHT));
                    pCurNode->y2 += temp;
                }
			}
		}
		else if(pCurNode->type == _ELEMENT_TYPE_TABLE_ROW)
		{
			int nMaxHeight;

			//Table의 Row는 새 라인에서 시작한다
			if(pCurNode->x1 != 0)
			{
				pCurNode->x1 = 0;
				pCurNode->y1 = ((WAP_Node *)pCurNode->pPrevNode)->y2;
			}

			//현재 Table Row 중 가장 긴 Column의 높이
			nMaxHeight = WAP_Formatting_TableRow(pCurNode);
			
			pCurNode->x2 = WAP_LCD_WIDTH-1;//95
			pCurNode->y2 = pCurNode->y1 + nMaxHeight;

			//---------------------------------------------------------------
			//TableRow의 Focus 초기화
			//---------------------------------------------------------------
			if(g_pCurTableRow == NULL)
				g_pCurTableRow = (WAP_Content_TableRow *)pCurNode->pContent;

			//---------------------------------------------------------------
			//현 TableRow의 Element중 Focus가 하나도 없다면, 이를 Focus 노드에서 제거한다
			if(((WAP_Content_TableRow *)pCurNode->pContent)->pHeadFocusCell == NULL)
			{
				//현 TableRow가 시작 Focus 노드의 경우
				if(g_pCard->pHeadFocus == pCurNode)
				{
					//이후 포커스 노드가 없는 경우(포커스 노드가 하나도 없는 경우)
					if(pCurNode->pNextFocusNode == NULL)
					{
						g_pCard->pHeadFocus = NULL;
						g_pCurFocusNode = NULL;
					}
					else
					{
						g_pCard->pHeadFocus = pCurNode->pNextFocusNode;
						g_pCurFocusNode = pCurNode->pNextFocusNode;

						pCurNode->pNextFocusNode->pPrevFocusNode = NULL;
					}
				}
				else
				{
					//이후 포커스 노드가 없는 경우
					if(pCurNode->pNextFocusNode == NULL)
					{
						pCurNode->pPrevFocusNode->pNextFocusNode = NULL;
					}
					else
					{
						pCurNode->pPrevFocusNode->pNextFocusNode = pCurNode->pNextFocusNode;
						pCurNode->pNextFocusNode->pPrevFocusNode = pCurNode->pPrevFocusNode;
					}
				}

				pCurNode->pPrevFocusNode = NULL;
				pCurNode->pNextFocusNode = NULL;
			}
		}
		else if(pCurNode->type == _ELEMENT_TYPE_BR)
		{
			WAP_Node *pPrevNode = (WAP_Node *)pCurNode->pPrevNode;

    		//마지막 노드가 새 라인에서 시작되는 Br일 경우, 없애준다
			if(pCurNode->x1 == 0 && pCurNode->pNextNode == NULL)
			{
				pCurNode->x1 = pPrevNode->x2;
				pCurNode->y1 = pPrevNode->y1;
				pCurNode->x2 = pPrevNode->x2;
				pCurNode->y2 = pPrevNode->y2;
			}
			else if(pCurNode->x1 == 0
			&& (pPrevNode != NULL &&
					(  pPrevNode->type == _ELEMENT_TYPE_TEXT 
					|| pPrevNode->type == _ELEMENT_TYPE_LINK_TEXT
					|| pPrevNode->type == _ELEMENT_TYPE_IMAGE
					|| pPrevNode->type == _ELEMENT_TYPE_LINK_IMAGE
					|| pPrevNode->type == _ELEMENT_TYPE_INPUT
					|| pPrevNode->type == _ELEMENT_TYPE_SELECT)))
			{
				//그외의 x가 0인 Br은, 없는 것처럼 처리해 준다
				pCurNode->x2 = 0;
				pCurNode->y2 = pCurNode->y1 + pCurNode->font.height;
			}
			else
			{
				pCurNode->x2 = WAP_LCD_WIDTH-1;//95
				pCurNode->y2 = pCurNode->y1 + pCurNode->font.height;
			}
		}
		else if(pCurNode->type == _ELEMENT_TYPE_INPUT
			 || pCurNode->type == _ELEMENT_TYPE_SELECT)
		{
			//Input과 Select는 새 라인에서 시작한다
			if(pCurNode->x1 != 0)
			{
				pCurNode->x1 = 0;
				pCurNode->y1 = pCurNode->y1 + pCurNode->font.height;
			}

			pContent = pCurNode->pContent;
			pt = Calculate_Node_EndPoint(pDisplayedTextData, pCurNode->x1, pCurNode->y1,
										WAP_FONT_NORMAL, 1);
					
			pCurNode->x2 = pt.x;
			pCurNode->y2 = pt.y;

			//한 노드가 한 라인에 존재할 경우, 시작점 만큼 늘려준다
			if(pCurNode->y2 == (pCurNode->y1 + pCurNode->font.height))
				pCurNode->x2 += pCurNode->x1;
		}
		else//_ELEMENT_TYPE_TEXT
		{
			pContent = pCurNode->pContent;
			pt = Calculate_Node_EndPoint(pDisplayedTextData, pCurNode->x1, pCurNode->y1,
										pCurNode->font.font_index, 1);
					
			pCurNode->x2 = pt.x;
			pCurNode->y2 = pt.y;

			//한 노드가 한 라인에 존재할 경우, 시작점 만큼 늘려준다
			if(pCurNode->y2 == (pCurNode->y1 + pCurNode->font.height))
				pCurNode->x2 += pCurNode->x1;
		}

		//------------------------------------------------------------------
		// 다음 노드의 x1, y1 좌표 결정
		//------------------------------------------------------------------ 
		if(pCurNode->pNextNode != NULL)
		{
			pt = Calculate_Node_StartPoint(pCurNode, pCurNode->pNextNode);
		
			pNextNode = (WAP_Node *)(pCurNode->pNextNode);
			pNextNode->x1 = pt.x;
			pNextNode->y1 = pt.y;
		}

		pCurNode = (WAP_Node *)pCurNode->pNextNode;

	}// end of while
		
} // end of WAP_Formatting()

void GetVisibleInputData(char* pSource, char* pDestination, int nVisibleSize)
{
	int wChar;
	int nTotCount, nHanCount;
	char *pText;

	//원본 데이타를 copy한다
	strcpy(pDestination, pSource);

	//nVisibleSize가 유효하지 않으면, 원본 데이타를 그대로 사용한다
	if(nVisibleSize <= 0) return;
	
	//init
	nTotCount = 0;
	nHanCount = 0;
	pText = pDestination;

	//nVisibleSize만큼만 원본 데이타를 copy한다
	while(nTotCount < nVisibleSize)
	{
		if(*pText == 0) break;
		wChar = *pText;

		if ( wChar & 0x80 )//한글 처리
		{	  
			pText++;
			nHanCount++;
		}

		pText++;
		nTotCount++;		
	}
	pDestination[nTotCount + nHanCount] = NULL;
}

char* GetCurDisplayedData(WAP_Node *pCurNode)
{
	switch(pCurNode->type)
	{
		case _ELEMENT_TYPE_CARD  :
		{
			WAP_Card *pContent = (WAP_Card *)(pCurNode->pContent);
			return pContent->title;
		}
		case _ELEMENT_TYPE_TEXT  :
		case _ELEMENT_TYPE_LINK_TEXT:
		{
			WAP_Content_Text *pContent = (WAP_Content_Text *)(pCurNode->pContent);
			return pContent->text;
		}
	    case _ELEMENT_TYPE_INPUT :  
		{
			char temp[500];
			char text[WAP_MAX_TEXT];
			WAP_Content_Input *pContent = (WAP_Content_Input *)(pCurNode->pContent);

			//-----------------------------------------------------------
			//Input element의 visible size만큼만 display한다
			memset(text, '\0', WAP_MAX_TEXT);
			GetVisibleInputData(pContent->text, text, pContent->size);
			//-----------------------------------------------------------

			memset(temp, '\0', 500);
			if(pContent->title == NULL)
				sprintf(temp, "%s입력:[%s]", SF_DOWN_RIGHT_PENCIL, text);
			else
				sprintf(temp, "%s%s:[%s]", SF_DOWN_RIGHT_PENCIL, pContent->title, text);
		
			return (temp);
		}
	    case _ELEMENT_TYPE_SELECT:
		{
			char temp[WAP_MAX_TEXT];
			WAP_Content_Select *pContent = (WAP_Content_Select *)(pCurNode->pContent);

			memset(temp, '\0', WAP_MAX_TEXT);
			if(pContent->title == NULL)
				sprintf(temp, "\242\321선택:[%s]", pContent->selectedOptions);
			else
				sprintf(temp, "\242\321%s:[%s]", pContent->title, pContent->selectedOptions);
			return (temp);
		}
	    case _ELEMENT_TYPE_OPTION:
		{
			WAP_Content_Option *pContent = (WAP_Content_Option *)(pCurNode->pContent);
			return pContent->label;
		}
		case _ELEMENT_TYPE_KEY:
		{
			WAP_Content_Key *pContent = (WAP_Content_Key *)(pCurNode->pContent);
			return pContent->label;
		}
		case _ELEMENT_TYPE_IMAGE:
		case _ELEMENT_TYPE_LINK_IMAGE:
			break;

	};// end of switch
	
	return NULL;
}

int Calculate_WordCount_OneLine(char* str, int x, int y, int nFontType, int nDivideWidth)
{
	WORD wChar;
	
	int i;
	int nCharWidth;
	int nOriginX = x;
	int nTextWidth = x;
	
	//--------------------------------------------------------
	int nLoopCount;
	int nHanFontWidth, nAsciiFontWidth;

    if(nFontType == WAP_FONT_MEDIUM)
    {
		nLoopCount = WAP_MED_SCREEN_WIDTH;
		nHanFontWidth	= WAP_MED_HAN_FONT_WIDTH;
		nAsciiFontWidth	= WAP_MED_ASCII_FONT_WIDTH;
	}
	else
	{
		nLoopCount = WAP_SCREEN_WIDTH;
		nHanFontWidth	= WAP_HAN_FONT_WIDTH;
		nAsciiFontWidth	= WAP_ASCII_FONT_WIDTH;
	}

	//================================================================
	// 총 캐릭터의 수를 x 값에 의하여 "충분히 크게" 결정하여 loop를 돈다
	// loop는 현재 디스플레이할 문자가 없는 경우 종료할 수 있다
	// 캐릭터 수의 최대값은 20(=120/6)이다
	for(i=(nLoopCount-x/nAsciiFontWidth)/nDivideWidth; i>0; i--)
	{
		/*=================================================*/
		//line break 처리 (다음 문자 고려)
		/*=================================================*/
		if(((*str & 0x80) //한글
			&& (nTextWidth+nHanFontWidth > WAP_LCD_WIDTH/nDivideWidth))
		  ||(!(*str & 0x80) //아스키
		    && (nTextWidth+nAsciiFontWidth > WAP_LCD_WIDTH/nDivideWidth)))
		{
			return(nTextWidth - nOriginX);
		}

		if(*str == 0) break;
		wChar = *str;

		/*=================================================*/
		if ( wChar & 0x80 )              /* 한글 처리      */
		/*=================================================*/
		{	  
			str++; i--;
			if (*str == 0) break;
			wChar <<= 8; wChar += *str;

			nCharWidth = nHanFontWidth;
		}
		/*=================================================*/
		else							/* 아스키 처리     */
		/*=================================================*/
		{
			nCharWidth = nAsciiFontWidth;
		}

		/*=================================================*/
		// 한 문자 표시 후의 처리
		/*=================================================*/
		nTextWidth += nCharWidth;
		x += nCharWidth;
		str++;
		
	}/* end of for */
	
	return( nTextWidth - nOriginX );
}

point Calculate_Node_EndPoint(char* str, int x, int y, int nFontType, int nDivideWidth)
{
	int i;
	point pt;
	int curWordCount = 0;
	char* new_scr_ptr = str;

	//--------------------------------------------------------
	int nAsciiFontWidth, nAsciiFontHeight;

    if(nFontType == WAP_FONT_MEDIUM)
	{
		nAsciiFontWidth	= WAP_MED_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_MED_ASCII_FONT_HEIGHT;
	}
	else
	{
		nAsciiFontWidth	= WAP_ASCII_FONT_WIDTH;
		nAsciiFontHeight= WAP_ASCII_FONT_HEIGHT;
	}

	//================================================================
	// loop는 현재 디스플레이할 문자가 없는 경우 종료할 수 있다
	for(i=0; ; i++)
	{
		//================================================================
		// 한 라인 처리 전 조정
		if(*new_scr_ptr == 0) break;
		
		if(i!=0) x=0;//not frist line
		
		//================================================================
		// 한 라인을 처리한다
		curWordCount = Calculate_WordCount_OneLine(new_scr_ptr, x, y, nFontType, nDivideWidth);
		
		//================================================================
		// 한 라인 처리 후 조정
		y += nAsciiFontHeight;//y 값 조정

		new_scr_ptr = new_scr_ptr + curWordCount/nAsciiFontWidth;

	}//end of for
		
	pt.x = curWordCount;
	pt.y = y;

	return (pt);
}

point Calculate_Node_StartPoint(WAP_Node* pPrevNode, WAP_Node* pCurNode)
{
	point result;
	if((pPrevNode->x2 >= WAP_LCD_WIDTH-1) // 새로운 라인
	||
		//----------------------
		//for dual fonts
		//	이전 노드와 현재 노드의 Font가 다르고,
		//	남은 여백이 현재 폰트의 한 글자를 표시할  여분이 없는 경우
		//	새 라인으로 바꿔서 좌표값을 결정한다
		(pPrevNode->font.font_index != pCurNode->font.font_index
		&& WAP_LCD_WIDTH - pPrevNode->x2 < pCurNode->font.width*2))//for double buffering
	{
		result.x = 0;	  	

		//----------------------
		//for dual fonts
		//	이전 노드가 Medium Font일 경우, 이전 노드 뿐 아니라
		//	이전 노드와 같은 라인에 Normal Font가 존재하는 지 확인해야 한다
		if(pPrevNode->font.font_index == WAP_FONT_MEDIUM)
		{
			WAP_Node* pNode = pPrevNode->pPrevNode;
			while(pNode != NULL && pNode->y2 - pNode->font.height == pPrevNode->y1)
			{
				if(pNode->font.font_index == WAP_FONT_NORMAL)
				{
					result.y = pPrevNode->y2 + (WAP_ASCII_FONT_HEIGHT - WAP_MED_ASCII_FONT_HEIGHT);//+3
					return (result);
				}
				pNode = pNode->pPrevNode;
			}
		}
		//----------------------

		result.y = pPrevNode->y2;
		return (result);
	}
	else							// 연속된 라인
	{
		if(	//----------------------
			//for dual fonts
			//	이전 노드와 현재 노드의 Font가 다르고,
			//	남은 여백이 현재 노드의 글자를 표시할 수 있는 경우
			(pPrevNode->font.font_index != pCurNode->font.font_index
			&& WAP_LCD_WIDTH - pPrevNode->x2 > pCurNode->font.width))
		{
			result.x = pPrevNode->x2;
			//----------------------
			//for dual fonts
			//	이전 노드가 Medium Font일 경우, 이전 노드 뿐 아니라
			//	이전 노드와 같은 라인에 Normal Font가 존재하는 지 확인해야 한다
			if(pPrevNode->font.font_index == WAP_FONT_MEDIUM)
			{
				WAP_Node* pNode = pPrevNode->pPrevNode;
				while(pNode != NULL && pNode->y2 - pNode->font.height == pPrevNode->y1)
				{
					if((pNode->font.font_index == WAP_FONT_NORMAL) 
						&&((pPrevNode->y2 - pPrevNode->font.height) != pPrevNode->y1)
						)
					{
						result.y = pPrevNode->y1 + WAP_ASCII_FONT_HEIGHT;
						return (result);
					}
					pNode = pNode->pPrevNode;
				}
			}		
			result.y = pPrevNode->y2 - pPrevNode->font.height;
			return (result);
		}
		else
		{
			result.x = pPrevNode->x2;	
			result.y = pPrevNode->y2 - pPrevNode->font.height;
			return (result);
		}
		//result.x = pPrevNode->x2;	
		//result.y = pPrevNode->y2 - pPrevNode->font.height;
		//return (result);
	}
}

void WAP_Send_LCDBuffer_ToHS(void)
{
	//yjahn96 00.12.28 -------------------
//	extern void /*SKY_API*/ SKY_RefreshDisplay(void);
//	SKY_RefreshDisplay();	
	extern void EQS_AppletDrawDirect(void);
	EQS_AppletDrawDirect();
	// <--	uia_UpdateAnimation();
}

#ifdef WAP_USE_SKT_NEW
	extern boolean g_bDrawBar;
	extern void WAP_DrawLoadingBarToBackBuff(int nAniTextMode, BOOL bAniBarDirection);
#endif//#ifdef WAP_USE_SKT_NEW
void WAP_Display(void)
{
	WAP_Node *pCurNode;

#ifdef WAP_SCROLL_BAR //for Draw Scroll bar - yjahn96 01.05.23
    BOOL l_ExistCard = FALSE;        
#endif  //#ifdef WAP_SCROLL_BAR 

	if(g_pCard == NULL) 
	{
        CLNTa_log(1, 0, "g_pCard == NULL");
		return;
	}
	if(g_bTerminatedWAP) 
	{
        CLNTa_log(1, 0, "g_bTerminatedWAP set");
		return;
	}
	
	//------------------------------------------------------------------
	// 애니메이션 미리보기에서 animation timer돌고 있을때,
	// scroll등의 key입력이 있을때 화면이 느려지는 현상 방지위해
	// 잠시 animation timer를 stop후 restart시킴 - yjahn96 01.04.10
	CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();

	//------------------------------------------------------------------
	// 현재 화면을 구성하는 노드들을 연속적으로 그려준다
	// 즉 g_pFirst 부터 g_pLast 까지 그려준다
	//------------------------------------------------------------------
	pCurNode = g_pCard->pHeadContent;
	while(pCurNode != NULL && pCurNode->y2 <= g_nLCDOffset)
	{
		pCurNode = pCurNode->pNextNode;
	}
	
	while(pCurNode != NULL && pCurNode->y1 < g_nLCDOffset+WAP_DOC_HEIGHT)
    {

		//------------------------------------------------------------------
	   	// 현재의 노드 type에 따라 각 노드를 그려주는 함수를 부른다
		//------------------------------------------------------------------
		switch(pCurNode->type)
		{
			case _ELEMENT_TYPE_CARD  :
                WAP_Draw_Element_Card(pCurNode);
#ifdef WAP_SCROLL_BAR //for Draw Scroll bar - yjahn96 01.05.23
                l_ExistCard = TRUE;        
#endif  //#ifdef WAP_SCROLL_BAR 
                break;

			case _ELEMENT_TYPE_TEXT  :
			case _ELEMENT_TYPE_LINK_TEXT:
				WAP_Draw_Element_Text(pCurNode, 1, 0);
				break;

	        case _ELEMENT_TYPE_INPUT :  
				WAP_Draw_Element_Input(pCurNode);
				break;

	        case _ELEMENT_TYPE_SELECT:
				WAP_Draw_Element_Select(pCurNode);
				break;

			case _ELEMENT_TYPE_KEY:
				WAP_Draw_Element_Key(pCurNode);
				break;

			case _ELEMENT_TYPE_IMAGE:
			case _ELEMENT_TYPE_LINK_IMAGE:
				WAP_Draw_Element_Image(pCurNode, 0, 0);//for Table
				break;

			case _ELEMENT_TYPE_TABLE_ROW:
				WAP_Draw_Element_TableRow(pCurNode);
				break;
	
			default :
				break;	//Unknown 은 처리하지 않는다

		};// end of switch

		pCurNode = (WAP_Node *)pCurNode->pNextNode;

     }// end of while


#ifdef WAP_SCROLL_BAR //for Draw Scroll bar - yjahn96 01.05.23
    if( g_pCard->isKeyCard)
    {
        if((l_ExistCard && g_pCard->nScrollLine > 6) //card있을때 6 lines보다 큰경우
            || (!l_ExistCard && g_pCard->nScrollLine > 7))  //card없을때 7 lines보다 큰 경우
        {
            WAP_DisplayScrollBar(l_ExistCard);        
        }
    }
#endif  //#ifdef WAP_SCROLL_BAR 


#ifdef WAP_USE_SKT_NEW
    if(!g_bDrawBar)
#endif//#ifdef WAP_USE_SKT_NEW
    {
#if (defined WAP_B1) || (defined WAP_B2)
	    WAP_DisplayCML();
#else   //#if (defined WAP_B1) || (defined WAP_B2)
        WAP_DisplaySoftkey();
#endif  //#if (defined WAP_B1) || (defined WAP_B2)
    }
#ifdef WAP_USE_SKT_NEW
    else
    {
        WAP_DrawLoadingBarToBackBuff(2,TRUE);
    }
#endif//#ifdef WAP_USE_SKT_NEW

	//handset task에 display를 요청한다
	WAP_Send_LCDBuffer_ToHS();

	//---------------------------------------------
	// animation timer start!!  - yjahn96 01.04.10
	if(Check_Play_SIS_Animation() == TRUE &&  g_nViewMode == WAP_VIEW_MODE_WML)
	{
		RestartAnimationTimer(600);    //		ui_sistimer_set(200, TRUE);
	} 
}

void WAP_DisplayCML(void)
{
	WAP_Node *pLastNode;

    BOOL l_midKeyExist = FALSE;
	//전체 Clear
    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//WAP_Display_Image_CML(WAP_OK_CML/*WAP_UPDOWN_CML*/, WAP_CML_OKEMPTY/*WAP_CML_UPDOWNEMPTY*/ ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OKEMPTY, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CETEREMPTY , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_RIGHTEMPTY ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// 화면아래 라인
	WAP_Display_Line(2, WAP_DOC_HEIGHT, WAP_LCD_WIDTH - 3, WAP_DOC_HEIGHT, BLACK);
    
	//===========================================================================
	// 왼쪽 키(메뉴)
    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_MENU , WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// 중간 키
	if(g_pCurFocusNode == NULL 
	|| g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset
	// Focus가 없는 TableRow인 경우
	|| (g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
	    && ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL))
	{
        WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//WAP_Display_Image_CML(WAP_OK_CML/*WAP_UPDOWN_CML*/, WAP_CML_OKEMPTY/*WAP_CML_UPDOWNEMPTY*/ ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
        WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OKEMPTY, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);
        WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CETEREMPTY , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
    }
	else
	{
        l_midKeyExist = TRUE;
        WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS); //ok
		switch(g_pCurFocusNode->type)
		{
			case _ELEMENT_TYPE_INPUT:   //입력
                WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_INPUT, WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
				break;

			case _ELEMENT_TYPE_SELECT:  //선택
                WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CSELECT, WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
                break;

			case _ELEMENT_TYPE_KEY:     //실행
                WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_KEY, WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
				break;

			case _ELEMENT_TYPE_LINK_TEXT:
			case _ELEMENT_TYPE_LINK_IMAGE:
			case _ELEMENT_TYPE_TABLE_ROW:
                WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_LINK, WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
				break;

			default :
                l_midKeyExist = FALSE;
                WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OKEMPTY, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS); //ok
                WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CETEREMPTY, WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
				break;
		}
	}

	//===========================================================================
	// 오른쪽 키(상위)
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_HIGH, WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// 스크롤 키
	pLastNode = g_pCard->pHeadContent;
	while(pLastNode != NULL && pLastNode->y2 <= g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		pLastNode = pLastNode->pNextNode;
	}

	if((g_nLCDOffset == 0  &&  pLastNode == NULL)//1 화면만 존개
		&& 
			//Focus 노드가 없거나, 1개 존재하므로 스크롤 할 필요없음
			((g_pCard->pHeadFocus == NULL || g_pCard->pHeadFocus->pNextFocusNode == NULL)
			||
			// Focus가 없는 TableRow인 경우
			(g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
			&& ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL))
	)
	{
        if(l_midKeyExist)   //중간키는 있고, 스크롤은 아닌 경우
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);

	}
	else
	{
		//가장 처음 페이지
		if( g_nLCDOffset == 0 
		 && 
		 (  (g_pCard->pHeadFocus == NULL)
		 || (g_pCard->pHeadFocus != NULL
		     && (  g_pCard->pHeadFocus->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
		         || g_pCard->pHeadFocus == g_pCurFocusNode))))
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_DOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);

        }
		//가장 마지막 페이지
		else if(pLastNode == NULL
			&& (   (   g_pCurFocusNode != NULL
				    && g_pCurFocusNode->y2 >= g_nLCDOffset 
				    && g_pCurFocusNode->y2 <= g_nLCDOffset + WAP_DOC_HEIGHT
				    && g_pCurFocusNode->pNextFocusNode == NULL)
			    || ( g_pCurFocusNode == NULL
			         || (g_pCurFocusNode != NULL && g_pCurFocusNode->y2 <= g_nLCDOffset))))
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UP ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//중간 페이지
		else
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
        }
	}

	//===========================================================================
	//소프트 키로 인해 지워진 라인이 있을 경우, 이를 그려준다 
/*	if(nDrawLinePosXForTable > 0)
	{
		WAP_Display_Line(0,						 WAP_SOFTKEY_Y_POS,//(0,52)
						nDrawLinePosXForTable+1, WAP_SOFTKEY_Y_POS,//(x,52)
						WAP_BLACK); //검정색 라인
		nDrawLinePosXForTable = 0;
	}
*/
}

void WAP_DisplaySoftkey(void)
{
	BYTE *pFont;
#if !((defined WAP_B1) || (defined WAP_B2))
	BYTE *pWordFont;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
    
	unsigned char pLeft[2]	= {0x05, 0x00};
	unsigned char pRight[2]	= {0x06, 0x00};

	WORD pSoftkey_Menu[2]	= {0x84f7, 0x84f8};// 메뉴

	WORD pSoftkey_Input[2]	= {0x86d4, 0x86d5};// 입력
	WORD pSoftkey_Select[2]	= {0x84fe, 0x84ff};// 선택
	WORD pSoftkey_Key[2]	= {0x88e8, 0x88e9};// 실행
	WORD pSoftkey_Link[2]	= {0x88ea, 0x88eb};// 연결

	WORD pSoftkey_Upper[2]	= {0x89d0, 0x89d1};// 상위
	WORD pSoftkey_Blank[1]	= {0x81bf};

	WORD* pSoftkey_Left = NULL;
	WAP_Node *pLastNode;
	
	extern /*ROM*/ BYTE* /*EQS_API*/ GetBitmapFontBufferByCode(WORD wCode);	//yjahn96 00.12.28

	//좌우가운데여백 Clear
	pFont = GetBitmapFontBufferByCode(pSoftkey_Blank[0]);
#if !((defined WAP_B1) || (defined WAP_B2))
	pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
	WAP_OutWordCharLCDBufferHan(pWordFont, 0*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pWordFont, 34*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pWordFont, 84*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pWordFont, 108*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_OutWordCharLCDBufferHan(pFont, 0, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pFont, 34, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pFont, 84, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pFont, 108, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	//===========================================================================
	// 왼쪽 키(메뉴)
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_NormalTextOutXY((BYTE*)pLeft, 4*2, WAP_SOFTKEY_Y_POS, WAP_DRAW_NORMAL, 1, 0);
	pFont = GetBitmapFontBufferByCode(pSoftkey_Menu[0]);
	pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
	WAP_OutWordCharLCDBufferHan(pWordFont, 10*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	pFont = GetBitmapFontBufferByCode(pSoftkey_Menu[1]);
	pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
	WAP_OutWordCharLCDBufferHan(pWordFont, 22*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_NormalTextOutXY((BYTE*)pLeft, 4, WAP_SOFTKEY_Y_POS, WAP_DRAW_NORMAL, 1, 0);
	pFont = GetBitmapFontBufferByCode(pSoftkey_Menu[0]);
	WAP_OutWordCharLCDBufferHan(pFont, 10, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	pFont = GetBitmapFontBufferByCode(pSoftkey_Menu[1]);
	WAP_OutWordCharLCDBufferHan(pFont, 22, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	//===========================================================================
	// 중간 키
	if(g_pCurFocusNode == NULL 
	|| g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset
	// Focus가 없는 TableRow인 경우
	|| (g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
	    && ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL))
	{
		pFont = GetBitmapFontBufferByCode(pSoftkey_Blank[0]);
#if !((defined WAP_B1) || (defined WAP_B2))
		pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
		WAP_OutWordCharLCDBufferHan(pWordFont, 36*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		WAP_OutWordCharLCDBufferHan(pWordFont, 48*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		WAP_OutWordCharLCDBufferHan(pWordFont, 60*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		WAP_OutWordCharLCDBufferHan(pWordFont, 72*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_OutWordCharLCDBufferHan(pFont, 36, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		WAP_OutWordCharLCDBufferHan(pFont, 48, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		WAP_OutWordCharLCDBufferHan(pFont, 60, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		WAP_OutWordCharLCDBufferHan(pFont, 72, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
	else
	{
		switch(g_pCurFocusNode->type)
		{
			case _ELEMENT_TYPE_INPUT:
				pSoftkey_Left = pSoftkey_Input;
				break;

			case _ELEMENT_TYPE_SELECT: 
				pSoftkey_Left = pSoftkey_Select;
				break;

			case _ELEMENT_TYPE_KEY: 
				pSoftkey_Left = pSoftkey_Key;
				break;

			case _ELEMENT_TYPE_LINK_TEXT:
			case _ELEMENT_TYPE_LINK_IMAGE:
			case _ELEMENT_TYPE_TABLE_ROW:
				pSoftkey_Left = pSoftkey_Link;
				break;

			default :
				pSoftkey_Left = pSoftkey_Blank;
				break;
		}

        pFont = GetBitmapFontBufferByCode(0x88bf);//ok
#ifdef PORTING_B0_WAP
        if(ui.earpieceup)
			pFont = GetBitmapFontBufferByCode(0x88bf);//ok
		else//for A7M
			pFont = GetBitmapFontBufferByCode(0x8ac0);//A
#endif //PORTING_B0_WAP
#if !((defined WAP_B1) || (defined WAP_B2))
		pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
		WAP_OutWordCharLCDBufferHan(pWordFont, 48*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		pFont = GetBitmapFontBufferByCode(pSoftkey_Left[0]);
		pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
		WAP_OutWordCharLCDBufferHan(pWordFont, 60*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		pFont = GetBitmapFontBufferByCode(pSoftkey_Left[1]);
		pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
		WAP_OutWordCharLCDBufferHan(pWordFont, 72*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_OutWordCharLCDBufferHan(pFont, 48, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		pFont = GetBitmapFontBufferByCode(pSoftkey_Left[0]);
		WAP_OutWordCharLCDBufferHan(pFont, 60, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
		pFont = GetBitmapFontBufferByCode(pSoftkey_Left[1]);
		WAP_OutWordCharLCDBufferHan(pFont, 72, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	}

	//===========================================================================
	// 오른쪽 키(상위)
#if !((defined WAP_B1) || (defined WAP_B2))
	pFont = GetBitmapFontBufferByCode(pSoftkey_Upper[0]);
	pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
	WAP_OutWordCharLCDBufferHan(pWordFont, 86*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	pFont = GetBitmapFontBufferByCode(pSoftkey_Upper[1]);
	pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
	WAP_OutWordCharLCDBufferHan(pWordFont, 98*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	
	WAP_NormalTextOutXY((BYTE*)pRight, 110*2, WAP_SOFTKEY_Y_POS, WAP_DRAW_NORMAL, 1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	pFont = GetBitmapFontBufferByCode(pSoftkey_Upper[0]);
	WAP_OutWordCharLCDBufferHan(pFont, 86, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	pFont = GetBitmapFontBufferByCode(pSoftkey_Upper[1]);
	WAP_OutWordCharLCDBufferHan(pFont, 98, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	
	WAP_NormalTextOutXY((BYTE*)pRight, 110, WAP_SOFTKEY_Y_POS, WAP_DRAW_NORMAL, 1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	//===========================================================================
	// 스크롤 키
	pLastNode = g_pCard->pHeadContent;
	while(pLastNode != NULL && pLastNode->y2 <= g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		pLastNode = pLastNode->pNextNode;
	}

	if((g_nLCDOffset == 0  &&  pLastNode == NULL)//1 화면만 존개
		&& 
			//Focus 노드가 없거나, 1개 존재하므로 스크롤 할 필요없음
			((g_pCard->pHeadFocus == NULL || g_pCard->pHeadFocus->pNextFocusNode == NULL)
			||
			// Focus가 없는 TableRow인 경우
			(g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
			&& ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL))
	)
	{
	}
	else
	{
		WORD pSoftkey_Move[3]	= {0x86b1, 0x88ed, 0x88ee};// 이동

		//가장 처음 페이지
		if( g_nLCDOffset == 0 
		 && 
		 (  (g_pCard->pHeadFocus == NULL)
		 || (g_pCard->pHeadFocus != NULL
		     && (  g_pCard->pHeadFocus->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
		         || g_pCard->pHeadFocus == g_pCurFocusNode))))
		{
				pFont = GetBitmapFontBufferByCode(pSoftkey_Move[1]);//down img
#ifdef PORTING_B0_WAP
            else if(ui.earpieceup)
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[1]);//down img
			else//for A7M
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveA[1]);//down img
#endif //PORTING_B0_WAP
		}
		//가장 마지막 페이지
		else if(pLastNode == NULL
			&& (   (   g_pCurFocusNode != NULL
				    && g_pCurFocusNode->y2 >= g_nLCDOffset 
				    && g_pCurFocusNode->y2 <= g_nLCDOffset + WAP_DOC_HEIGHT
				    && g_pCurFocusNode->pNextFocusNode == NULL)
			    || ( g_pCurFocusNode == NULL
			         || (g_pCurFocusNode != NULL && g_pCurFocusNode->y2 <= g_nLCDOffset))))
		{
				pFont = GetBitmapFontBufferByCode(pSoftkey_Move[2]);//up img
#ifdef PORTING_B0_WAP
            else if(ui.earpieceup)
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[2]);//up img
			else//for A7M
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveA[2]);//up img
#endif //PORTING_B0_WAP
		}
		//중간 페이지
		else
		{
				pFont = GetBitmapFontBufferByCode(pSoftkey_Move[0]);
#ifdef PORTING_B0_WAP
            else if(ui.earpieceup)
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[0]);
			else//for A7M
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveA[0]);
#endif //PORTING_B0_WAP
		}
#if !((defined WAP_B1) || (defined WAP_B2))
		pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
		WAP_OutWordCharLCDBufferHan(pWordFont, 36*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_OutWordCharLCDBufferHan(pFont, 36, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}

	//===========================================================================
	//소프트 키로 인해 지워진 라인이 있을 경우, 이를 그려준다 
	if(nDrawLinePosXForTable > 0)
	{
		WAP_Display_Line(0,						 WAP_SOFTKEY_Y_POS,//(0,52)
						nDrawLinePosXForTable+1, WAP_SOFTKEY_Y_POS,//(x,52)
						WAP_BLACK/*검정색 라인*/);
		nDrawLinePosXForTable = 0;
	}
}

#ifdef WAP_NOWRAP	//yjahn96
int NoWrap_num;
int curNoWrap;
#endif//#ifdef WAP_NOWRAP

void WAP_Scrolling(BOOL bDown)
{
	//------------------------------------------------------------------
	// Focus 이동이 가능한 경우, Focus 이동을 우선적으로 수행한다
	// Focus 이동이 없는 경우, Window 이동을 수행한다

	//------------------------------------------------------------------
	// 단, 한 화면 내에서  Focus 이동이 있은 후에, 
	// Focus 된 노드가 화면에 전부 표시되지 않는 경우에는
	// Window 이동이 추가로 이루어져야 한다
	// 즉 Focus 이동으로 이동된 노드가 화면에 무두 표시되어져야 한다

	//------------------------------------------------------------------
	//Window 내의 focus 이동이 우선한다
	if(WAP_FocusScrolling(bDown))
	{
		WAP_Display();
	}
	//------------------------------------------------------------------
	//focus 이동이 없을 경우, Window 이동을 한다
	else if(WAP_WindowScrolling(bDown))
	{
		WAP_Display();
	}
}

BOOL WAP_FocusScrolling(BOOL bDown)
{
	BOOL bNotMoveWindow;
	if(bDown) 
		bNotMoveWindow = WAP_FocusScrolling_Down();
	else
		bNotMoveWindow = WAP_FocusScrolling_Up();

	return bNotMoveWindow;
}

LOCAL BOOL ChangeFocusNode(BOOL bDown, WAP_Node* pNewFocusNode);
BOOL WAP_FocusScrolling_Down(void)
{
	WAP_Node *pNextFocusNode;
	pNextFocusNode = g_pCurFocusNode->pNextFocusNode;

	//------------------------------------------------------------------
	// Focus 노드가 하나도 없는 경우, focus 이동 실패
	//------------------------------------------------------------------
	if(g_pCurFocusNode == NULL) return FALSE;//화면이동 한다
	
	//==================================================================
	// 다음에는 Focus 노드가 하나도 없는 경우, focus 이동 실패
	//------------------------------------------------------------------
	if(pNextFocusNode == NULL){
#ifdef WAP_NOWRAP
		if(g_pCard->isKeyCard)
		{
			//NoWrap timer------------------------------
			//g_pCurFocusNode = g_pCard->pHeadFocus;
			SetFocusNode(g_pCard->pHeadFocus);
		}
#endif//#ifdef WAP_NOWRAP
		return FALSE;//화면이동 한다
	}
	//------------------------------------------------------------------
	// 다음 Focus 노드로 이동한다 (Focus Link의 다음으로 이동)
	//------------------------------------------------------------------

	//------------------------------------------------------------------
	// case 1) 현재 문서 내에 Focus 노드가 없는 경우
	if(g_pCurFocusNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset)
	{
		//다음의 새로운 화면(g_nLCDOffset+13)에 현 포커스 노드가 보인다
		if(! (  g_pCurFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		     || g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			;
		}
		//다음의 새로운 화면(g_nLCDOffset+13)에 다음 포커스 노드가 보인다
		else if(pNextFocusNode != NULL
		&& ! (  pNextFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		     || pNextFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
		}
		return FALSE;//화면이동 한다
	}

	//------------------------------------------------------------------
	//case 2) 같은 화면에 존재
	else if(g_nLCDOffset < g_pCurFocusNode->y2 
	     && pNextFocusNode->y1 < g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		BOOL bSuccess = ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
		if(!bSuccess) return FALSE;//화면이동 한다
		
		//------------------------------------
		//for dual fonts
		if(g_pCurFocusNode->y1 + g_pCurFocusNode->font.height > g_nLCDOffset + WAP_DOC_HEIGHT)
			return FALSE;//화면이동 한다
		//------------------------------------

		return TRUE;//화면이동 안 한다
	}
	//------------------------------------------------------------------
	//case 3) 연결된 노드
	/*else if(g_pCurFocusNode->y2 - g_pCurFocusNode->font.height == pNextFocusNode->y1
		|| 
		(g_pCurFocusNode->y2 == pNextFocusNode->y1
		   && g_pCurFocusNode->x2 >= WAP_LCD_WIDTH-1//95
		   && pNextFocusNode->x1 == 0 ))
	{
		ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);	
		return FALSE;//화면이동 한다
	}*/
	//------------------------------------------------------------------
	//case 4) 그외의 경우
	else
	{
		//현 포커스 노드의 끝이 마지막 라인에 걸린 경우
		if(g_pCurFocusNode->y2 == g_nLCDOffset + WAP_DOC_HEIGHT)
		{
			//다음의 새로운 화면(g_nLCDOffset+13)에도 현 포커스 노드가 보인다
			if(! ( g_pCurFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				//연결된 노드
				if( (g_pCurFocusNode->y2 - g_pCurFocusNode->font.height == pNextFocusNode->y1
				    || 
					(g_pCurFocusNode->y2 == pNextFocusNode->y1
					   && g_pCurFocusNode->x2 >= WAP_LCD_WIDTH-1//95
					   && pNextFocusNode->x1 == 0 ))
				&& 
				//현 노드가 1 라인인 경우
					g_pCurFocusNode->y2 - g_pCurFocusNode->y1 == WAP_ASCII_FONT_HEIGHT)
				{
					ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
				}
			}
			//다음의 새로운 화면(g_nLCDOffset+13)에 다음 포커스 노드가 보인다
			else if(! (pNextFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		        || pNextFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
			}
		}
		//다음의 새로운 화면(g_nLCDOffset+13)에도 현 포커스 노드가 보인다
		else if(! (g_pCurFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			;
		else
		{
			ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
		}
		return FALSE;//화면이동 한다
	}
}

BOOL WAP_FocusScrolling_Up(void)
{
	WAP_Node *pPrevFocusNode;
	pPrevFocusNode = g_pCurFocusNode->pPrevFocusNode;

	//------------------------------------------------------------------
	// Focus 노드가 하나도 없는 경우, focus 이동 실패
	//------------------------------------------------------------------
    if(g_pCurFocusNode == NULL) return FALSE;//화면이동 한다
	
	//==================================================================
	// 이전에는 Focus 노드가 하나도 없는 경우, focus 이동 실패
	//------------------------------------------------------------------
	if(pPrevFocusNode == NULL){
#ifdef WAP_NOWRAP
		if(g_pCard->isKeyCard && g_nLCDOffset == 0)
		{
			WAP_Node* pLastNode;
			pLastNode = g_pCard->pHeadFocus;

			while(pLastNode != NULL)
			{
				if(pLastNode->pNextFocusNode !=NULL)
					pLastNode = (WAP_Node *)pLastNode->pNextFocusNode;
				else
					break;
			}
			//NoWrap timer------------------------------
			//g_pCurFocusNode = pLastNode;
			SetFocusNode(pLastNode);

			//------------------------------------------------------------
			// Element갯수가 LCD의 최대 display Line수(4) 보다 작은 경우,
			// Scroll up시 화면이동은 하지 않는다.  -- yjahn96 00.12.20
			//------------------------------------------------------------
			if(g_pCurFocusNode->y2 <= WAP_DOC_HEIGHT)
				return TRUE;	//화면이동 안 한다
		}
#endif //#ifdef	WAP_NOWRAP	
		return FALSE;//화면이동 한다
	}
	//------------------------------------------------------------------
	// 이전 Focus 노드로 이동한다 (Focus Link의 이전으로 이동)
	//------------------------------------------------------------------
	
	//------------------------------------------------------------------
	// case 1) 현재 문서 내에 Focus 노드가 없는 경우
	if(g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset)
	{
		//다음의 새로운 화면(g_nLCDOffset-13)에 현 포커스 노드가 보인다
		if(! (  g_pCurFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		     || g_pCurFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			;
		}
		//다음의 새로운 화면(g_nLCDOffset-13)에 이전 포커스 노드가 보인다
		else if(pPrevFocusNode != NULL
		&& ! (  pPrevFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		     || pPrevFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		}
		return FALSE;//화면이동 한다
	}

	//------------------------------------------------------------------
	//case 2) 같은 화면에 존재
	else if(g_nLCDOffset < pPrevFocusNode->y2 
	     && g_pCurFocusNode->y1 < g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		BOOL bSuccess = ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		if(!bSuccess) return FALSE;//화면이동 한다

		//------------------------------------
		//for dual fonts
		if(g_pCurFocusNode->y1 < g_nLCDOffset)
			return FALSE;//화면이동 한다
		//------------------------------------

		return TRUE;//화면이동 안 한다
	}
	//------------------------------------------------------------------
	//case 3) 연결된 노드
	/*else if(pPrevFocusNode->y2 - pPrevFocusNode->font.height == g_pCurFocusNode->y1
		|| 
		(pPrevFocusNode->y2 == g_pCurFocusNode->y1
		   && pPrevFocusNode->x2 >= WAP_LCD_WIDTH-1//95
		   && g_pCurFocusNode->x1 == 0 ))
	{
		ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		return FALSE;//화면이동 한다
	}*/
	//------------------------------------------------------------------
	//case 4) 그외의 경우
	else
	{
		//현 포커스 노드의 시작이 처음 라인에 걸린 경우
		if(g_pCurFocusNode->y1 == g_nLCDOffset)
		{
			//다음의 새로운 화면(g_nLCDOffset-13)에도 현 포커스 노드가 보인다
			if(! ( g_pCurFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				//연결된 노드
				if(	(pPrevFocusNode->y2 - pPrevFocusNode->font.height == g_pCurFocusNode->y1
					|| 
					(pPrevFocusNode->y2 == g_pCurFocusNode->y1
						&& pPrevFocusNode->x2 >= WAP_LCD_WIDTH-1//95
						&& g_pCurFocusNode->x1 == 0 ))
				&& 
				//현 노드가 1 라인인 경우
					g_pCurFocusNode->y2 - g_pCurFocusNode->y1 == WAP_ASCII_FONT_HEIGHT)
				{
					ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
				}
			}
			//다음의 새로운 화면(g_nLCDOffset-13)에 이전 포커스 노드가 보인다
			else if(! (pPrevFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		        || pPrevFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
			}
		}
		//다음의 새로운 화면(g_nLCDOffset-13)에도 현 포커스 노드가 보인다
		else if(! (g_pCurFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			;
		else
		{
			ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		}
		return FALSE;//화면이동 한다
	}
}

LOCAL BOOL ChangeFocusNode(BOOL bDown, WAP_Node* pNewFocusNode)
{
	WAP_Content_TableRow *pTableRow;
	
	if(pNewFocusNode->type != _ELEMENT_TYPE_TABLE_ROW)
	{
#ifdef	WAP_NOWRAP
		//새로운 Focus 노드로 이동한다
		//NoWrap timer------------------------------
		//g_pCurFocusNode = pNewFocusNode;
		SetFocusNode(pNewFocusNode);
#else
		//새로운 Focus 노드로 이동한다
		g_pCurFocusNode = pNewFocusNode;
#endif//#ifdef	WAP_NOWRAP
		return TRUE;
	}

	//새로운 Focus 노드가 TableRow일 경우
	pTableRow = (WAP_Content_TableRow *)pNewFocusNode->pContent;

	//현 TableRow에 Focus가 하나도 없다
	if(pTableRow->pHeadFocusCell == NULL)
	{
		//Focus 노드로 이동하지 않는다
		//현 TableRow의 Focus를 해제한다
		if(bDown)//down
		{
			g_pCurFocusNode->pNextFocusNode = pNewFocusNode->pNextFocusNode;
			pNewFocusNode->pNextFocusNode->pPrevFocusNode = g_pCurFocusNode;
		}
		else//up
		{
			g_pCurFocusNode->pPrevFocusNode = pNewFocusNode->pPrevFocusNode;
			pNewFocusNode->pPrevFocusNode->pNextFocusNode = g_pCurFocusNode;
		}
		pNewFocusNode->pNextFocusNode = NULL;
		pNewFocusNode->pPrevFocusNode = NULL;

		return FALSE;
	}
	else
	{
#ifdef	WAP_NOWRAP
		//새로운 Focus 노드(TableRow)로 이동한다
		//NoWrap timer------------------------------
		//g_pCurFocusNode = pNewFocusNode;
		SetFocusNode(pNewFocusNode);
#else
		//새로운 Focus 노드(TableRow)로 이동한다
		g_pCurFocusNode = pNewFocusNode;
#endif//#ifdef	WAP_NOWRAP
		g_pCurTableRow = (WAP_Content_TableRow *)g_pCurFocusNode->pContent;

		return TRUE;
	}
}

#ifdef	WAP_NOWRAP
char tmpText[WAP_MAX_TEXT/(WAP_HS_SCRN_WIDE-2)+2][WAP_HS_SCRN_WIDE-1];	//16자씩 넣을 수 있는 19byte짜리 buffer 갯수 : (256/16)+2 = 18 
void SetFocusNode(WAP_Node* pNewFocusNode)
{
	//label길이 check!!
	int length, i;
	char label[WAP_MAX_TEXT];

	WAP_Content_Key *pKey = (WAP_Content_Key *) pNewFocusNode->pContent;

	//clear
	for(i=0; i<WAP_MAX_TEXT/(WAP_HS_SCRN_WIDE-2)+2; i++)
		memset(tmpText[i], NULL, WAP_HS_SCRN_WIDE-1);

	if(g_pCurFocusNode->type == _ELEMENT_TYPE_KEY)//old focus
	{
	    CancelNowrapTimer();    //		ui_nowrap_set(0, FALSE);//kill timer
		MSG_HIGH("kill timer",0,0,0);
	}

	if(pNewFocusNode->type == _ELEMENT_TYPE_KEY)//new focus
	{
		memset(label, '\0', WAP_MAX_TEXT);
		strcpy(label, pKey->label);
		length = strlen(label);

		//label길이가 18이상인 것 찾기!!!
		if(length > 18){
			curNoWrap = 0;
			RestartNowrapTimer(1000);   //			ui_nowrap_set(1000, TRUE);//set timer
			MSG_HIGH("timer set ",0,0,0);
		}
	}

	//assign focus
	g_pCurFocusNode = pNewFocusNode;
}
#endif//#ifdef	WAP_NOWRAP

BOOL WAP_WindowScrolling(BOOL bDown)
{
	if(bDown)
		return(WAP_WindowScrolling_Down());
	else
		return(WAP_WindowScrolling_Up());
}

BOOL WAP_WindowScrolling_Down(void)
{
	WAP_Node *pLastNode;
	pLastNode = g_pCard->pHeadContent;
	while(pLastNode != NULL && pLastNode->y2 <= g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		pLastNode = pLastNode->pNextNode;
	}

	//======================================================================
	//  다음 화면이 존재하지 않는다 (현재 화면이 마지막 화면이다)
	//======================================================================
	if(pLastNode == NULL)
	{
#ifdef	WAP_NOWRAP
		if(g_pCard->isKeyCard)
		{
			g_nLCDOffset = 0;
			return TRUE;
		}
		else
#endif//#ifdef	WAP_NOWRAP
		return FALSE;//nothing
	}
	//======================================================================
	// 다음 화면으로 이동한다
	//======================================================================
	else
	{
		//----------------------------------------------------------
		//for dual fonts
		//현재 시작 노드의 font height만큼 이동한다
		WAP_Node *pCurNode = g_pCard->pHeadContent;
		while(pCurNode != NULL)
		{
			//현재 화면의 시작 노드 찾기
			if((pCurNode->y1 <= g_nLCDOffset && pCurNode->y2 > g_nLCDOffset)
			//다음 노드가 있으면, 다음 노드들중 같은 라인의 가장 마지막 노드 찾기
			&& (pCurNode->pNextNode == NULL || pCurNode->pNextNode->y1 > g_nLCDOffset))
			{
				if(pCurNode->font.font_index == WAP_FONT_MEDIUM)
				{
					//----------------------
					//for dual fonts
					//	현재 노드가 Medium Font일 경우, 현재 노드 뿐 아니라
					//	현재 노드와 같은 라인에 Normal Font가 존재하는 지 확인해야 한다
					BOOL bMixFontLine = FALSE;
					WAP_Node* pNode = pCurNode->pPrevNode;
					while(pNode != NULL)
					{
						if(pNode->y2 - pNode->font.height == pCurNode->y1
						&& pNode->font.font_index == WAP_FONT_NORMAL)
						{
							bMixFontLine = TRUE;
							break;
						}
						pNode = pNode->pPrevNode;
					}
					//----------------------

					if(bMixFontLine)
						g_nLCDOffset += WAP_ASCII_FONT_HEIGHT;//15
					else
						g_nLCDOffset += WAP_MED_ASCII_FONT_HEIGHT;//13
				}
				else
				{
					g_nLCDOffset += WAP_ASCII_FONT_HEIGHT;//16->13
				}

				return TRUE;
			}
			pCurNode = pCurNode->pNextNode;
		}
	}
	return FALSE;
}

BOOL WAP_WindowScrolling_Up(void)
{
	//======================================================================
	//  이전 화면이 존재하지 않는다 (현재 화면이 첫 화면이다)
	//======================================================================
	if(g_nLCDOffset == 0)
	{
#ifdef	WAP_NOWRAP
		if(g_pCard ->isKeyCard)
		{
			WAP_Node* pLastNode;
			pLastNode = g_pCard->pHeadContent;

			while(pLastNode != NULL)
			{
				if(pLastNode->pNextNode !=NULL)
					pLastNode = (WAP_Node *)pLastNode->pNextNode;
				else
					break;
			}
			g_nLCDOffset = pLastNode->y2 - WAP_DOC_HEIGHT;		
			return TRUE;
		}
		else
#endif//#ifdef	WAP_NOWRAP
		return FALSE;//nothing
	}
	//======================================================================
	// 이전 화면으로 이동한다
	//======================================================================
	else
	{
		//----------------------------------------------------------
		//for dual fonts
		//현재 시작 노드의 font height만큼 이동한다
		WAP_Node *pCurNode = g_pCard->pHeadContent;
		while(pCurNode != NULL)
		{
			//현재 화면의 시작 노드 찾기
			if(pCurNode->y1 < g_nLCDOffset && pCurNode->y2 >= g_nLCDOffset)
			{
				if(pCurNode->font.font_index == WAP_FONT_MEDIUM)
				{
					//----------------------
					//for dual fonts
					//	현재 노드가 Medium Font일 경우, 현재 노드 뿐 아니라
					//	현재 노드와 같은 라인에 Normal Font가 존재하는 지 확인해야 한다
					BOOL bMixFontLine = FALSE;
					WAP_Node* pNode = pCurNode->pPrevNode;
					while(pNode != NULL)
					{
						if(pNode->y2 - pNode->font.height == pCurNode->y1
						&& pNode->font.font_index == WAP_FONT_NORMAL)
						{
							bMixFontLine = TRUE;
							break;
						}
						pNode = pNode->pPrevNode;
					}
					//----------------------

					if(bMixFontLine)
						g_nLCDOffset -= WAP_ASCII_FONT_HEIGHT;//15
					else
						g_nLCDOffset -= WAP_MED_ASCII_FONT_HEIGHT;//13
				}
				else
				{
					g_nLCDOffset -= WAP_ASCII_FONT_HEIGHT;//15->13
				}

				if(g_nLCDOffset < 0) g_nLCDOffset = 0;

				return TRUE;
			}
			pCurNode = pCurNode->pNextNode;
		}
	}
	return FALSE;
}

void WAP_Scrolling_TableRow(BOOL bRight)
{
	WAP_Content_TableRow *pTableRow;

	//------------------------------------------------------------------
	// TableRow의 좌/우 스크롤이 가능한지 체크한다
	//------------------------------------------------------------------
	if(//Focus 노드가 하나도 없는 경우
		g_pCurFocusNode == NULL
   	   //Focus 노드가 현재 화면에 없는 경우
	|| (g_pCurFocusNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
	    || g_pCurFocusNode->y2 <= g_nLCDOffset)
	   //Focus 노드가 Table이 아닌 경우
	|| g_pCurFocusNode->type != _ELEMENT_TYPE_TABLE_ROW
	   //Focus인 Table이 Link를 하나도 가지지 않은 경우
	|| ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL)
		return;
	
	//------------------------------------------------------------------
	// 현 포커스 Column을 이동한다
	//------------------------------------------------------------------
	pTableRow = (WAP_Content_TableRow *)g_pCurFocusNode->pContent;
		
	if(bRight)//right (->)
	{
		if(pTableRow->pFocusCell->pNextFocusNode != NULL)
		{
			pTableRow->pFocusCell = pTableRow->pFocusCell->pNextFocusNode;
			WAP_Display();
		}
	}
	else//left (<-)
	{
		if(pTableRow->pFocusCell->pPrevFocusNode != NULL)
		{
			pTableRow->pFocusCell = pTableRow->pFocusCell->pPrevFocusNode;
			WAP_Display();
		}
	}
}

//===========================================================================
//
//	각 Element의 Display 함수들
//
//===========================================================================
void WAP_Draw_Element_Card(WAP_Node *pMe)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	char temp[22];
	int length;

	length = strlen(g_pCard->title);

	memset(temp, '\0', 22);
	
	if(g_pCard->title == NULL)
	{
		//Card의 Title이 없으면 화면에 그려주지 않는다
		return;
	}
	else if(length > WAP_HS_SCRN_WIDE)//18자이상
	{
		char temp2[WAP_HS_SCRN_WIDE+1];
		memset(temp2, '\0', WAP_HS_SCRN_WIDE+1);

		strncpy(temp2, g_pCard->title, WAP_HS_SCRN_WIDE);
		TruncateString((BYTE *)temp2, WAP_HS_SCRN_WIDE);//21자
		strcpy(temp, temp2);
	}
	else
	{
		strcpy(temp, g_pCard->title);
	}

#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1 * 2,//for double buffering
						pMe->y1 - g_nLCDOffset,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_CENTER,
						1, 0);

    //for decoration title
	WAP_Display_Rectangle(pMe->x1, pMe->y1 - g_nLCDOffset,
						  pMe->x2-1, pMe->y2 - g_nLCDOffset - 2,
						  TRUE, WAP_WHITEGRAY);
	WAP_Display_Line(pMe->x1, pMe->y2 - g_nLCDOffset - 1,
					 pMe->x2, pMe->y2 - g_nLCDOffset - 1,
					 WAP_BLACKGRAY);
	WAP_Display_Line(pMe->x2, pMe->y1 - g_nLCDOffset,
					 pMe->x2, pMe->y2 - g_nLCDOffset - 1,
					 WAP_BLACK);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
    WAP_Display_TextOut_Title(pMe->x1,
						pMe->y1 - g_nLCDOffset,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_CENTER,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

void WAP_Draw_Element_Text(WAP_Node *pMe, int nTableColumns, int nCurColumn)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	WAP_Content_Text *pText = (WAP_Content_Text *) pMe->pContent;
	
	//----------------------------------------------------------
	//다음 노드가 없거나 Break인 경우만, Align이 지원된다
	int nAlign = WAP_TEXT_ALIGN_LEFT;

	if(pMe->x1 == 0
	&& (
			pMe->pNextNode == NULL//현 노드가 마지막 노드
		||	((WAP_Node *)pMe->pNextNode)->type == _ELEMENT_TYPE_BR//다음 노드가 BR
		||	(((WAP_Node *)pMe->pNextNode)->x1 == 0 
			&& ((WAP_Node *)pMe->pNextNode)->y1 == pMe->y2)))//다음 라인
	{
		nAlign = pText->align;
	}
#if !((defined WAP_B1) || (defined WAP_B2))
	//----------------------------------------------------------
	if(nTableColumns == 1 && nCurColumn == 0)//not Table Data
	{
		if(pMe == g_pCurFocusNode)
			nDrawMode = WAP_DRAW_REVERSE;//reverse
	}
	else//Table Data
	{
		if(g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
		&& pMe == g_pCurTableRow->pFocusCell)
		{
			nDrawMode = WAP_DRAW_REVERSE;//reverse

			//Focus된 현재의 Cell 전체를 반전한다
			WAP_Display_Rectangle(pMe->x1, 
								pMe->y1 - g_nLCDOffset,
								pMe->x2,
								pMe->y2 - g_nLCDOffset,
								TRUE,//fill
								WAP_BLACK);	//color
		}
	}
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	//================================================================
	// y값이 음수라는 것은 화면에 보이지 않는 영역이라는 것이다
	// 이는 한 노드가 긴 경우, 자주 발생하는 현상이다
	// 이를 위해 Text의 Ptr을 증가시키고, 본 작업에 들어간다
	{

		int x = pMe->x1;
		int y = pMe->y1 - g_nLCDOffset;
		char *pStr = pText->text;
/*		int curWordCount;

		while(y < 0)
		{
			if(*pStr == 0) break;

			if(pStr != pText->text) x = 0;//new line
			//x *= 2;//for double buffering
			curWordCount = WAP_NormalTextOutXY((BYTE*)pStr, x, y, nDrawMode, 1, 0);

			y += WAP_ASCII_FONT_HEIGHT;//y 값 조정
			pStr = pStr + curWordCount/WAP_ASCII_FONT_WIDTH;
		}
*/
		if(pStr != pText->text) x = 0;//new line

#if !((defined WAP_B1) || (defined WAP_B2))		
		x *= 2;//for double buffering

        WAP_Display_TextOut(x, y, pStr,
							pMe->font.font_index, nDrawMode, pText->isLink, nAlign,
							nTableColumns, nCurColumn);//for Table
#else  //#if !((defined WAP_B1) || (defined WAP_B2))

	if(nTableColumns == 1 && nCurColumn == 0)//not Table Data
	{
		if(pMe == g_pCurFocusNode)
			nDrawMode = WAP_DRAW_REVERSE;//reverse

        WAP_Display_TextOut(x, y, pStr,
							pMe->font.font_index, nDrawMode, pText->isLink, nAlign,
							nTableColumns, nCurColumn);//for Table
	}
	else//Table Data
	{
		if(g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
		&& pMe == g_pCurTableRow->pFocusCell)
		{
			nDrawMode = WAP_DRAW_REVERSE;//reverse

			//Focus된 현재의 Cell 전체를 반전한다
/*			WAP_Display_Rectangle(pMe->x1, 
								pMe->y1 - g_nLCDOffset,
								pMe->x2,
								pMe->y2 - g_nLCDOffset,
								TRUE,//fill
								WAP_BLACK);	//color
*/
			WAP_Display_TextOut_Focus(x, y, pStr,
								pMe->font.font_index, nDrawMode, pText->isLink, nAlign,
								nTableColumns, nCurColumn);//for Table
		}

        WAP_Display_TextOut(x, y, pStr,
							pMe->font.font_index, nDrawMode, pText->isLink, nAlign,
							nTableColumns, nCurColumn);//for Table
	}
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	}
}

void WAP_Draw_Element_TableRow(WAP_Node *pMe)
{
	WAP_Content_TableRow* pTableRow = (WAP_Content_TableRow *)pMe->pContent;
	WAP_Node *pCurNode = (WAP_Node *)pTableRow->pHeadCell;
	while(pCurNode != NULL)
	{
		//Table의 각 Cell을 init해 준다
/*		WAP_Display_Rectangle(	pCurNode->x1, 
								pCurNode->y1 - g_nLCDOffset,
								pCurNode->x2,
								pCurNode->y2 - g_nLCDOffset,
								TRUE,//fill
								WAP_WHITE);	//color	
*/
		//Table의 각 Cell의 내용을 그려준다
		switch(pCurNode->type)
		{
			case _ELEMENT_TYPE_TEXT : 
			case _ELEMENT_TYPE_LINK_TEXT:
			{
				WAP_Content_Text *pTableContent = (WAP_Content_Text *)pCurNode->pContent; 
				WAP_Draw_Element_Text(pCurNode,
										pTableRow->noOfColumns,
										pTableContent->nTableColumn);
				break;
			}
			case _ELEMENT_TYPE_IMAGE: 
			case _ELEMENT_TYPE_LINK_IMAGE:
			{
				WAP_Content_Image *pTableContent = (WAP_Content_Image *)pCurNode->pContent; 
				WAP_Draw_Element_Image(pCurNode,
										pTableRow->noOfColumns,
										pTableContent->nTableColumn);
				break;
			}
		}

		//Table의 각 Cell에 Box를 그려준다
		WAP_Display_Rectangle(	pCurNode->x1, 
								pCurNode->y1 - g_nLCDOffset,
								pCurNode->x2,
								pCurNode->y2 - g_nLCDOffset,
								FALSE,//fill
								WAP_BLACK);	//color	
		if(pCurNode->y2 - g_nLCDOffset == WAP_SOFTKEY_Y_POS)//52
			nDrawLinePosXForTable = pCurNode->x2;

		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}
}

void WAP_Draw_Element_Input(WAP_Node *pMe)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	char temp[WAP_MAX_TEXT];
	char text[WAP_MAX_TEXT];

	WAP_Content_Input *pInput = (WAP_Content_Input *) pMe->pContent;
	if(pMe == g_pCurFocusNode) nDrawMode = WAP_DRAW_REVERSE;//reverse
		
	//-----------------------------------------------------------
	memset(temp, '\0', WAP_MAX_TEXT);
	if(pInput->title == NULL)
	{
		sprintf(temp, "%s입력", SF_DOWN_RIGHT_PENCIL);//strcpy(temp, "\210\354입력");
	}
	else
	{
		sprintf(temp, "%s%s", SF_DOWN_RIGHT_PENCIL, pInput->title);//sprintf(temp, "\210\354%s", pInput->title);
	}
	
	//-----------------------------------------------------------
	//Input element의 visible size만큼만 display한다
	memset(text, '\0', WAP_MAX_TEXT);
	GetVisibleInputData(pInput->text, text, pInput->size);
	//-----------------------------------------------------------

	if(pInput->isPassword)
	{
		char password[WAP_MAX_INPUT+1];
		memset(password, '\0', WAP_MAX_INPUT+1);
		memset(password, '*', strlen(text));

		sprintf(temp, "%s:[%s]", temp, password);
	}
	else
	{
		sprintf(temp, "%s:[%s]", temp, text);
	}
		
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1 * 2,//for double buffering
						pMe->y1 - g_nLCDOffset,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1,
						pMe->y1 - g_nLCDOffset,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

void WAP_Draw_Element_Select(WAP_Node *pMe)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	char temp[WAP_MAX_TEXT];

	WAP_Content_Select *pSelect = (WAP_Content_Select *) pMe->pContent;
	if(pMe == g_pCurFocusNode) nDrawMode = WAP_DRAW_REVERSE;//reverse
	
	//-----------------------------------------------------------
	memset(temp, '\0', WAP_MAX_TEXT);
	if(pSelect->title == NULL)
	{
		strcpy(temp, "\242\321선택");
	}
	else
	{
		sprintf(temp, "\242\321%s", pSelect->title);
	}
	sprintf(temp, "%s:[%s]", temp, pSelect->selectedOptions);

#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1 * 2,//for double buffering
						pMe->y1 - g_nLCDOffset,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_TextOut(pMe->x1,
						pMe->y1 - g_nLCDOffset,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

void WAP_Draw_Element_Key(WAP_Node *pMe)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	char label[WAP_MAX_TEXT];
	char temp[WAP_MAX_TEXT];

#ifdef	WAP_NOWRAP
	int length = 0, label_num=0, data_leng=0;
#endif//#ifdef	WAP_NOWRAP

	WAP_Content_Key *pKey = (WAP_Content_Key *) pMe->pContent;
	if(pMe == g_pCurFocusNode) nDrawMode = WAP_DRAW_REVERSE;//reverse

	//-----------------------------------------------------------
	// 라벨
	//-----------------------------------------------------------
	memset(label, '\0', WAP_MAX_TEXT);
	if(pKey->label == NULL)
	{
		if(strcmp(pKey->eventType, "prev") == 0	|| strcmp(pKey->eventType, "PREV") == 0)
			strcpy(label, "이전");
		else if(strcmp(pKey->eventType, "help") == 0 || strcmp(pKey->eventType, "HELP") == 0)
			strcpy(label, "도움말");
		/*else
			strcpy(label, "다음");*/
	}
	else
	{
#ifdef	WAP_NOWRAP
		extern void	WAP_HanCopy(char* pDest, char* pSrc, byte nLen);

		WAP_HanCopy((char *)label, (char *)pKey->label, WAP_MAX_TEXT-1);
		length = strlen(label);
		
		if(length > (WAP_HS_SCRN_WIDE-2) && pMe == g_pCurFocusNode)
		{
			do{
				WAP_HanCopy((char *)tmpText[label_num], (char *)label + data_leng, WAP_HS_SCRN_WIDE-2);
				tmpText[label_num][WAP_HS_SCRN_WIDE-2] = '\0';
				data_leng = data_leng + strlen((char *)tmpText[label_num]);
				label_num++;
			}
			while(data_leng != strlen((char *)label));
			NoWrap_num = label_num;
		}
		else{
			TruncateString((BYTE *)label, WAP_HS_SCRN_WIDE-2);//18자+icon
			NoWrap_num = 0;
		}
#else
		strcpy(label, pKey->label);
		TruncateString(label, WAP_HS_SCRN_WIDE-2);//18자+icon
#endif//#ifdef	WAP_NOWRAP
	}

	//-----------------------------------------------------------
	// 아이콘
	//-----------------------------------------------------------
	memset(temp, '\0', 20);

#ifdef	WAP_NOWRAP
	if(length > (WAP_HS_SCRN_WIDE-2) && pMe == g_pCurFocusNode)
	{
		memset(label, '\0', WAP_MAX_TEXT);
		strncpy(label, tmpText[curNoWrap],WAP_HS_SCRN_WIDE-2);
	}
#endif//#ifdef	WAP_NOWRAP

	if(pKey->nSKT_Number == WAP_KEY_SK_PREV)//---이전
	{
		//sprintf(temp, "\201\302%s", label);//(녹음)
		sprintf(temp, "\242\270%s", label);//이전
	}
	else if(pKey->nSKT_Number == WAP_KEY_SK_NEXT)//---다음
	{
		//sprintf(temp, "\201\303%s", label);//(재생)
		sprintf(temp, "\242\272%s", label);//다음
	}
	else if(pKey->nSKT_Number == WAP_KEY_SK_CALL)//---전화
	{
		sprintf(temp, "%s%s", SF_CELLULAR, label);//(전화)
	}
	else if(0 < pKey->nSKT_Number && pKey->nSKT_Number < 10)
	{
		switch(pKey->nSKT_Number)
		{
			case 1: sprintf(temp, "%s%s", SF_BLACK_NUM1, label); break;
			case 2: sprintf(temp, "%s%s", SF_BLACK_NUM2, label); break;
			case 3: sprintf(temp, "%s%s", SF_BLACK_NUM3, label); break;
			case 4: sprintf(temp, "%s%s", SF_BLACK_NUM4, label); break;
			case 5: sprintf(temp, "%s%s", SF_BLACK_NUM5, label); break;
			case 6: sprintf(temp, "%s%s", SF_BLACK_NUM6, label); break;
			case 7: sprintf(temp, "%s%s", SF_BLACK_NUM7, label); break;
			case 8: sprintf(temp, "%s%s", SF_BLACK_NUM8, label); break;
			case 9: sprintf(temp, "%s%s", SF_BLACK_NUM9, label); break;
		}
	}
	else
	{
		if(strcmp(pKey->eventType, "prev") == 0	|| strcmp(pKey->eventType, "PREV") == 0)
			sprintf(temp, "\242\270%s", label);//이전
		else if(strcmp(pKey->eventType, "help") == 0 || strcmp(pKey->eventType, "HELP") == 0)
			sprintf(temp, "%s%s", SF_INFORMATION, label);//도움말
		else
			sprintf(temp, "\242\272%s", label);//다음
	}

#if !((defined WAP_B1) || (defined WAP_B2))
	if(nDrawMode == WAP_DRAW_REVERSE)
	{
		//역상일 경우, 라인 전체를 반전시키기 위해 사각형을 그려준다
		WAP_Display_Rectangle(pMe->x1, 
								pMe->y1 - g_nLCDOffset,
								pMe->x2,
								pMe->y2 - g_nLCDOffset - 1,
								TRUE,//fill
								WAP_BLACK);//color	
	}

	WAP_Display_TextOut(pMe->x1 * 2,//for double buffering
						pMe->y1 - g_nLCDOffset,
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
        WAP_Display_TextOut_Focus(pMe->x1,
								pMe->y1 - g_nLCDOffset,
								temp,
								WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
								1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
}

void WAP_Draw_Element_Image(WAP_Node *pMe, int nTableColumns, int nCurColumn)
{
#ifdef SIS_1
    extern boolean g_fFirstSISImageDraw;    //yjahn96 01.03.02
#endif  //#ifdef SIS_1

	//yjahn96 00.12.28 -------------------
	extern byte* /*EQS_API*/ GetScreenBuffer(void);
    byte* LCDWordAniBuffer;
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	WAP_Content_Image *pImage = (WAP_Content_Image *) pMe->pContent;
	int nForAlignHeight = 0;
	int nForAlignWidth = 0;

    int rest = (pImage->height % WAP_ASCII_FONT_HEIGHT);


    //SetScreenChangable(FALSE);
    LCDWordAniBuffer = (byte* )GetScreenBuffer();
    //SetScreenChangable(TRUE);



	//Formatting 시 조정된 이미지 좌표값을 재조정한다
    if(rest != 0)  //if((pImage->height % WAP_ASCII_FONT_HEIGHT) != 0)
    {
//		nForAlignHeight = (WAP_ASCII_FONT_HEIGHT - (pImage->height % WAP_ASCII_FONT_HEIGHT)) / 2;
        int temp;

        temp = WAP_ASCII_FONT_HEIGHT - rest;
		nForAlignHeight = temp/2;
    }

	//for center align
	if(nTableColumns <= 0)//not Table data
	{
		if(pImage->height <= WAP_ASCII_FONT_HEIGHT)//inline image
		{
			nForAlignWidth = ((pMe->x2 - pMe->x1) - pImage->width) / 2;
		}
		else if(pMe->x1 == 0 && pImage->width < WAP_LCD_WIDTH)
		{
			nForAlignWidth = (WAP_LCD_WIDTH - pImage->width) / 2;
		}
	}
	else
	{
		if(pImage->width < WAP_LCD_WIDTH/nTableColumns)
			nForAlignWidth = (WAP_LCD_WIDTH/nTableColumns - pImage->width) / 2;
	}
	/*
	//Align의 값에 따라서, x좌표값을 조정해 준다
	switch(pImage->align)
	{
		case WAP_TEXT_ALIGN_LEFT:
		{
			break;
		}
		case WAP_TEXT_ALIGN_CENTER:
		{
			if(pImage->width < WAP_LCD_WIDTH)
				nForAlignWidth = (WAP_LCD_WIDTH - pImage->width) / 2;
			break;
		}
		case WAP_TEXT_ALIGN_RIGHT:
		{
			if(pImage->width < WAP_LCD_WIDTH)
				nForAlignWidth = WAP_LCD_WIDTH - pImage->width;
			break;
		}
	}*/

	if(pMe == g_pCurFocusNode) nDrawMode = WAP_DRAW_REVERSE;//reverse

	//---------------------------------------------------------------
	// 1. BMP 이미지 데이타를 그려준다
	//---------------------------------------------------------------
	if(pImage->imageData != NULL && pImage->imageType == WAP_IMAGE_TYPE_BMP)
	{
		WAP_Display_Image_BMP(	(char*)LCDWordAniBuffer,
								(char*)pImage->imageData, 
								pImage->width, 
								pImage->height,
								pMe->x1 + nForAlignWidth, 
								pMe->y1 + nForAlignHeight - g_nLCDOffset,
								nDrawMode,
								0,//align
								pImage->nBitCount);//bit per pixel

		if(nTableColumns <= 0//not Table data
		&& pImage->isLink)
		{
			WAP_Display_Rectangle(pMe->x1, 
								pMe->y1 - g_nLCDOffset,
								pMe->x2,
								pMe->y2 - g_nLCDOffset,
								FALSE,//fill
								WAP_BLACK);	//color	

			if(pMe->y2 - g_nLCDOffset == WAP_SOFTKEY_Y_POS)//52
				nDrawLinePosXForTable = pMe->x2;
		}
	}
	//---------------------------------------------------------------
	// 2. WBMP 이미지 데이타를 그려준다
	//---------------------------------------------------------------
	else if(pImage->imageData != NULL && pImage->imageType == WAP_IMAGE_TYPE_WBMP)
	{

		WAP_Display_Image_WBMP(	(char*)LCDWordAniBuffer,
								(char*)pImage->imageData, 
								pImage->width, 
								pImage->height,
								pMe->x1 + nForAlignWidth, 
								pMe->y1 + nForAlignHeight - g_nLCDOffset,
								nDrawMode,
								0);//align
		if(nTableColumns <= 0//not Table data
		&& pImage->isLink)
		{
			WAP_Display_Rectangle(pMe->x1, 
								pMe->y1 - g_nLCDOffset,
								pMe->x2,
								pMe->y2 - g_nLCDOffset,
								FALSE,//fill
								WAP_BLACK);	//color	

			if(pMe->y2 - g_nLCDOffset == WAP_SOFTKEY_Y_POS)//52
				nDrawLinePosXForTable = pMe->x2;
		}
	}
	//---------------------------------------------------------------
	// 3. SIS 이미지 데이타를 그려준다
	//---------------------------------------------------------------
	else if(pImage->imageData != NULL && pImage->imageType == WAP_IMAGE_TYPE_SIS)
	{
#ifdef SIS_1
        if((g_fFirstSISImageDraw == TRUE) && (pImage->nCurFrame == 0))
        {
            g_fFirstSISImageDraw = FALSE;
            Play_SIS_Animation();
        }
#endif  //#ifdef SIS_1

        WAP_Display_Image_SIS(	(char*)LCDWordAniBuffer,
								(char*)pImage->imageData,//(char*)(pImage->imageData + (pImage->width * pImage->height * pImage->nCurFrame)/8), 
								pImage->width, 
								pImage->height,
								pMe->x1 + nForAlignWidth, 
								pMe->y1 + nForAlignHeight - g_nLCDOffset,
								nDrawMode,
								0,  //align
                                pImage->nBitCount);

		if(nTableColumns <= 0//not Table data
		&& pImage->isLink)
		{
			WAP_Display_Rectangle(pMe->x1, 
								pMe->y1 - g_nLCDOffset,
								pMe->x2,
								pMe->y2 - g_nLCDOffset,
								FALSE,//fill
								WAP_BLACK);	//color	

			if(pMe->y2 - g_nLCDOffset == WAP_SOFTKEY_Y_POS)//52
				nDrawLinePosXForTable = pMe->x2;
		}
	}
	//---------------------------------------------------------------
	// 4. 그외의 경우, alt text와 사각형을 그려준다
	//---------------------------------------------------------------
	else 
	{
		int length = strlen(pImage->altText);

		if(nTableColumns > 0)//Table data
		{
			char temp[WAP_MAX_TEXT];

			memset(temp, '\0', WAP_MAX_TEXT);
			
            if(length <= 0)  memcpy(temp, "그림", strlen((char*)"그림"));    //if(pImage->altText == NULL)
            else  memcpy(temp, pImage->altText, length+1);


#if !((defined WAP_B1) || (defined WAP_B2))
			WAP_Display_TextOut(pMe->x1 * 2,//for double buffering 
							pMe->y1 - g_nLCDOffset,
							temp,
							WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
							nTableColumns, nCurColumn);//for Table
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
			WAP_Display_TextOut(pMe->x1,
							pMe->y1 - g_nLCDOffset,
							temp,
							WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
							nTableColumns, nCurColumn);//for Table
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

		}//if(Table data)
		else
		{
			char temp[WAP_HS_SCRN_WIDE+1];
			memset(temp, '\0', WAP_HS_SCRN_WIDE+1);
			
			if(length > WAP_HS_SCRN_WIDE)//18자이상
			{
				char temp2[WAP_HS_SCRN_WIDE+1];
				memset(temp2, '\0', WAP_HS_SCRN_WIDE+1);

				strncpy(temp2, pImage->altText, WAP_HS_SCRN_WIDE);
				TruncateString((BYTE *)temp2, WAP_HS_SCRN_WIDE);//18자
				strcpy(temp, temp2);
			}
			else
			{
                if(length <= 0)  strncpy(temp, "그림", strlen((char*)"그림"));    //if(pImage->altText == NULL)
                else  strcpy(temp, pImage->altText);
			}

#if !((defined WAP_B1) || (defined WAP_B2))
			WAP_Display_TextOut(pMe->x1 * 2,//for double buffering 
							pMe->y1 - g_nLCDOffset + pMe->font.height,
							temp,
							WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_CENTER,
							1, 0);//for Table

			WAP_Display_Rectangle(pMe->x1, 
							pMe->y1+1 - g_nLCDOffset,
							pMe->x2,
							pMe->y2-1 - g_nLCDOffset,
							FALSE,//fill
							WAP_BLACK);	//color
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
			WAP_Display_TextOut(pMe->x1, 
							pMe->y1 - g_nLCDOffset + pMe->font.height,
							temp,
							WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_CENTER,
							1, 0);//for Table

			WAP_Display_Rectangle(pMe->x1, 
							pMe->y1+1 - g_nLCDOffset,
							pMe->x2,
							pMe->y2-1 - g_nLCDOffset,
							FALSE,//fill
							WAP_BLACK);	//color
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	
		}//if( ! Table data )
	}
}

boolean Check_Play_SIS_Animation(void)
{
	WAP_Node *pCurNode ;
	
	if(g_pCard == NULL) return FALSE;
	
	pCurNode = g_pCard->pHeadContent;

	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_IMAGE
		|| pCurNode->type == _ELEMENT_TYPE_LINK_IMAGE)
		{
			WAP_Content_Image* pImage = (WAP_Content_Image *)pCurNode->pContent;

			if(pImage->imageType == WAP_IMAGE_TYPE_SIS)
			{
				//sis image가 현재 화면에 보일경우
				if(!( pCurNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT || pCurNode->y2 <= g_nLCDOffset))
				{
					if(pImage->nTotalFrame > 1)	return TRUE;	//frame 갯수가 2개 이상일경우 timer start
					else	return FALSE;
				}			
			}
		}
		pCurNode = pCurNode->pNextNode;
	}//end of while	
	return FALSE;
}

void Play_SIS_Animation(void)
{
	extern void WAP_Repaint(void);

	int result;

	BOOL bNeedRepaint = FALSE;
	WAP_Node *pCurNode = g_pCard->pHeadContent;

    CLNTa_log(1, 0, "-- Play_SIS_() is called --");

	if(g_pCard == NULL) return;
	if(g_bTerminatedWAP) return;

	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_IMAGE
		|| pCurNode->type == _ELEMENT_TYPE_LINK_IMAGE)
		{
			WAP_Content_Image* pImage = (WAP_Content_Image *)pCurNode->pContent;

			if(pImage->imageType == WAP_IMAGE_TYPE_SIS)
			{
                CLNTa_log(1, 0, "-- CurFrame : %d --",pImage->nCurFrame);
    
        		memset((char*)m_gUncompBuf, 0, MAX_UNCOMPBUF_SIZE);
				result = sisDecode( (unsigned char *)m_gImageBuf, 
					                (unsigned char *)m_gUncompBuf,
										pImage->nCurFrame) ;
				if(result == 0)
				{
					pImage->width	= WAP_LCD_WIDTH-1;//95
					pImage->height  = WAP_ASCII_FONT_HEIGHT*3;//13*3
					MSG_ERROR("sisDecode Fail",0,0,0);
				}
                //else if(result == 1)
                {
                    pImage->imageData = (char*)m_gUncompBuf;
                
				    pImage->nCurFrame++;
				    if(pImage->nCurFrame >= pImage->nTotalFrame )
					    pImage->nCurFrame = 0;//init

				    //sis image가 현재 화면에 보일경우
				    if(! (   pCurNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
						|| pCurNode->y2 <= g_nLCDOffset)   )
					{
						bNeedRepaint = TRUE;
					}
                }
			}
		}
		pCurNode = pCurNode->pNextNode;

	}//end of while


	if(bNeedRepaint && g_nViewMode == WAP_VIEW_MODE_WML)
	{
		//for Display 
		WAP_Repaint();
	}

}

#ifdef	WAP_NOWRAP
void Support_NoWrapping(void)
{
	if(g_pCard == NULL) return;
	if(g_bTerminatedWAP) return;

	if(g_pCurFocusNode != NULL && g_pCurFocusNode->type == _ELEMENT_TYPE_KEY)
	{
		if(curNoWrap >= 0 && curNoWrap < NoWrap_num)
		{
			curNoWrap++;
			if(curNoWrap == NoWrap_num)
				curNoWrap = 0;
		}
		else curNoWrap = 0;

		//sis image가 현재 화면에 보일경우
		if(! (   g_pCurFocusNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
			|| g_pCurFocusNode->y2 <= g_nLCDOffset)   )
		{
			//MSG_HIGH("-- WAP_Draw_Element_Key Call!! --",0,0,0);
			WAP_Draw_Element_Key(g_pCurFocusNode);
			
			//handset task에 display를 요청한다
			WAP_Send_LCDBuffer_ToHS();
		}
	}
}
#endif//#ifdef	WAP_NOWRAP

void TruncateString(BYTE *strText, int nMaxChar)
{
	//한 라인안에 들어가는 경우
	if(strlen((const char *)strText) <= nMaxChar) return;
	else
	{
		extern BOOL /*EQS_API*/ IsMiddleOfMultiByte(BYTE *pszSrc, UINT16 nPosition);
		//extern boolean IsMiddleofHan(BYTE *chk_str, BYTE position);

		BYTE tmp_str[WAP_HS_SCRN_WIDE];
		memset(tmp_str, ' ', WAP_HS_SCRN_WIDE-1);
		if( IsMiddleOfMultiByte(strText, (nMaxChar-2) )) {
			memcpy(tmp_str, strText, (nMaxChar-2) - 1 );
		} else {
			memcpy(tmp_str, strText, (nMaxChar-2) );
		}

		memcpy(&(tmp_str[(nMaxChar-2)]), SF_DOTDOTDOT, 2); //글자수가 많은경우 "..." 추가
		memcpy(strText, tmp_str, nMaxChar);
		
		strText[nMaxChar] = NULL;
	}
}

#ifdef WAP_SCROLL_BAR //for Scroll bar - yjahn96 01.05.23
void WAP_DisplayScrollBar(BOOL ExistCard)
{
	SCROLL_BAR       ScrollBar;
	SCROLL_BAR_INFO  SBarInfo;
	UINT8 nWidth;

    COORD x, y;
    INT8 nHeight, nPos;

    nWidth = R_SCROLLBAR_SIZE;
    if(ExistCard) //card있을때
    {
        x = WAP_LCD_WIDTH - nWidth + 1;//Rect.xRight - nWidth+1
        y = WAP_HEAD_HEIGHT + WAP_ASCII_FONT_HEIGHT;//Rect.yTop
        nHeight = 6*WAP_ASCII_FONT_HEIGHT;
        nPos = 0;
    }
    else
    {
        x = WAP_LCD_WIDTH - nWidth + 1;//Rect.xRight - nWidth+1
        y = WAP_HEAD_HEIGHT;//WAP_SOFTKEY_HEIGHT;//Rect.yTop
        nHeight = 7*WAP_ASCII_FONT_HEIGHT;
        nPos = g_nLCDOffset/WAP_ASCII_FONT_HEIGHT;
    }


	EQS_AddScrollBar(NULL, &ScrollBar,SBT_VERT,
					 x,//Rect.xRight - nWidth+1,
					 y,//Rect.yTop,
					 nHeight,0,\
					 g_pCard->nScrollLine,//CurrentAppletState.dyCanvasSize-nHeight-1,
					 nHeight/WAP_ASCII_FONT_HEIGHT,0);			
	SBarInfo.nMax = g_pCard->nScrollLine - nHeight/WAP_ASCII_FONT_HEIGHT;//CurrentAppletState.dyCanvasSize-nHeight-1;
	SBarInfo.nMin = 0;
	SBarInfo.nPageSize = nHeight/WAP_ASCII_FONT_HEIGHT;
	SBarInfo.nPos = nPos;//sSlideWindow.yWindowOffset;
    EQS_SetScrollBarInfo(&ScrollBar, &SBarInfo, TRUE);  

} 
#endif  //#ifdef WAP_SCROLL_BAR

void WAP_Clear_LCDDataBuffer(void)
{
	//yjahn96 00.12.28 -------------------
//	extern void /*EQS_API*/ ClearScreen(void);
//	/*EQS_API*/ ClearScreen();
    //extern VOID EQS_ClearRegion(COORD xLeft, COORD yTop, COORD xRight, COORD yBottom);
    //COLOR OldBackColor;

    //OldBackColor = EQS_GetBackColor();
    //EQS_SetBackColor(EQS_GetTextBackColor());
    //EQS_ClearRegion(0, 0/*16*/, 119 , 119);

//    EQS_FillRectangle(0, WAP_HEAD_HEIGHT-1, 119, 119, EQS_GetTextBackColor());
    //SetScreenChangable(FALSE);

    EQS_FillRectangle(0, WAP_HEAD_HEIGHT, 159, 159, WHITE);//WAP_BASIC_BACK_COLOR);//EQS_GetBackColor());

    //SetScreenChangable(TRUE);

    //EQS_SetBackColor(OldBackColor);
	//memset(LCDWordAniBuffer, '\0', WAP_LCD_PIXELS);
}

//11/9 jjkyg
//extern BOOL	 load_incard ; 
//extern BOOL	 loadingkind ; 

int WAP_Handle_CenterKey(void)
{
	//------------------------------------------------
	// Focus 노드가 없는 경우
	if(g_pCurFocusNode == NULL 
	|| g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset)
		return -1;
	
	//------------------------------------------------
	//clear buffer
	//WAP_Clear_LCDDataBuffer();

	switch(g_pCurFocusNode->type)
	{

		case _ELEMENT_TYPE_INPUT://입력
		{
			//-------------------------------------------------------------
			// yjahn96 01.04.11 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

			g_pCurInput = (WAP_Content_Input *)g_pCurFocusNode->pContent; 
			g_nViewMode = WAP_VIEW_MODE_WML_INPUT;

			return _ELEMENT_TYPE_INPUT;
		}
		case _ELEMENT_TYPE_SELECT://선택
		{	
			//-------------------------------------------------------------
			// yjahn96 01.04.11 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

			g_pCurSelect = (WAP_Content_Select *)g_pCurFocusNode->pContent;

			//------------------------------------------------------------
			// 옵션 화면
			g_nViewMode = WAP_VIEW_MODE_WML_OPTION;

			WAP_Formatting_Options();
			WAP_Display_Options();

			return _ELEMENT_TYPE_SELECT;
		}
		case _ELEMENT_TYPE_KEY: //실행
		{
			WAP_Content_Key *pContent = (WAP_Content_Key *)g_pCurFocusNode->pContent; 
			
			//-------------------------------------------------------------
			// yjahn96 01.04.03 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96 01.03.21
			CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP
//			load_incard  = TRUE;
//			loadingkind  = TRUE;

			////////////////////////////////////////////////////////////////////////////
			// WML문서상의 Vendor을 체크하여 download할 content를 찾을때 사용할 예정
			////////////////////////////////////////////////////////////////////////////
#ifdef BLOCK
			if(strncmp(pContent->eventType, "vnd.sky", strlen("vnd.sky") ) == 0)
			{
				int i, len;
				char tmp[20]={0};
				char buf[20]={0};

				strncpy(tmp, pContent->eventType, strlen(pContent->eventType));

				for(i=0; i<MAX_VND_NUM ;i++)
				{
					if(strncmp(tmp, &vendorType[i], len=strlen(&vendorType[i])) == 0)
					{
						strncpy(buf, &tmp[len], strlen(tmp)-len);

						wap_call_get_content(i, atoi(buf));
	
						return -1;//no animation
					}
				}
			}
			else
#endif //#ifdef BLOCK
			{
				SKY_keySelected(pContent->viewId, pContent->keyId);

#ifndef WAP_USE_SECURE
				if(Wtlshello)
				{
					enc_use.viewid  = pContent->viewId;
					enc_use.id      = pContent->keyId;
					enc_use.wmlkind = 1;
					return _ELEMENT_TYPE_WTLS_SESSION;
				}
				else
#endif//#ifndef WAP_USE_SECURE
				{
#ifdef WAP_USE_DORM
					normal_use.viewid  = pContent->viewId;
					normal_use.id      = pContent->keyId;
					normal_use.wmlkind = 1;				
#endif//#ifdef WAP_USE_DORM
				}
			}
			return _ELEMENT_TYPE_KEY;
		}
		case _ELEMENT_TYPE_LINK_TEXT://연결
		{
			WAP_Content_Text *pContent = (WAP_Content_Text *)g_pCurFocusNode->pContent; 

			//-------------------------------------------------------------
			// yjahn96 01.04.11 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

//			load_incard  = TRUE;
//			loadingkind  = TRUE;
			
			SKY_textSelected(pContent->viewId, pContent->textId);
#ifdef FOR_MS
#ifndef WAP_USE_SECURE
			if(Wtlshello)
			{
				enc_use.viewid  = pContent->viewId;
				enc_use.id      = pContent->textId;
				enc_use.wmlkind = 3;
				return _ELEMENT_TYPE_WTLS_SESSION;
			}
			else
#endif //#ifndef WAP_USE_SECURE
#endif//#ifdef FOR_MS
			{
#ifdef WAP_USE_DORM
				normal_use.viewid  = pContent->viewId;
				normal_use.id      = pContent->textId;
				normal_use.wmlkind = 3;
#endif//#ifdef WAP_USE_DORM
			}
			return _ELEMENT_TYPE_LINK_TEXT;
		}

		case _ELEMENT_TYPE_LINK_IMAGE://연결
		{
			WAP_Content_Image* pContent = (WAP_Content_Image *)g_pCurFocusNode->pContent; 

			//-------------------------------------------------------------
			// yjahn96 01.04.11 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

//			load_incard  = TRUE;
//			loadingkind  = TRUE;

			SKY_imageSelected(pContent->viewId, pContent->imageId);
			
			return _ELEMENT_TYPE_LINK_IMAGE;
		}
		case _ELEMENT_TYPE_TABLE_ROW://Table
		{
			WAP_Content_TableRow *pContent = (WAP_Content_TableRow *)g_pCurFocusNode->pContent;
			if(pContent->pHeadFocusCell == NULL) return -1;

			switch(pContent->pFocusCell->type)
			{
				case _ELEMENT_TYPE_LINK_TEXT://연결
				{
					WAP_Node *pCurFocusCell = (WAP_Node *)(pContent->pFocusCell);
					WAP_Content_Text *pCell = (WAP_Content_Text *)(pCurFocusCell->pContent); 

					//-------------------------------------------------------------
					// yjahn96 01.04.11 - for Animation timer stop
					CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

//					load_incard  = TRUE;
//					loadingkind  = TRUE;

					SKY_textSelected(pCell->viewId, pCell->textId);

#ifdef FOR_MS
					if(Wtlshello)
					{
						enc_use.viewid  = pCell->viewId;
						enc_use.id      = pCell->textId;
						enc_use.wmlkind = 3;
						return _ELEMENT_TYPE_WTLS_SESSION;
					}
					else
#endif//#ifdef FOR_MS
					{
#ifdef WAP_USE_DORM
						normal_use.viewid  = pCell->viewId;
						normal_use.id      = pCell->textId;
						normal_use.wmlkind = 3;
#endif//#ifdef WAP_USE_DORM
					}
					return _ELEMENT_TYPE_LINK_TEXT;
				}

				case _ELEMENT_TYPE_LINK_IMAGE://연결
				{
					WAP_Node *pCurFocusCell = (WAP_Node *)(pContent->pFocusCell);
					WAP_Content_Image* pCell = (WAP_Content_Image *)(pCurFocusCell->pContent); 

					//-------------------------------------------------------------
					// yjahn96 01.04.11 - for Animation timer stop
					CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

//					load_incard  = TRUE;
//					loadingkind  = TRUE;

					SKY_imageSelected(pCell->viewId, pCell->imageId);
					
					return _ELEMENT_TYPE_LINK_IMAGE;
				}
			}
		}

	}//end of switch
	return -1;
}

BOOL WAP_Handle_RightKey(void)
{
	if(g_pKeyUp)
	{
		//----------------------------------------------------
		//선택을 알리고 연결된 동작을 하도록 한다
//		load_incard  = TRUE;
//		loadingkind  = TRUE;

		SKY_keySelected(g_pKeyUp->viewId, g_pKeyUp->keyId);
		return TRUE;
	}
	return FALSE;
}

//=============================================================================
// Do element를 위한 숫자 키 사용
//=============================================================================
BOOL WAP_Handle_NumKey_ForDo(int nKey)
{
	WAP_Node *pCurNode = g_pCard->pHeadFocus;

	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_KEY
		&& ((WAP_Content_Key *)pCurNode->pContent)->nSKT_Number == nKey)
		{
			//----------------------------------------------------
			//현재 화면에 보이지 않는 경우, 화면을 이동한다
			if(pCurNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
			|| pCurNode->y2 <= g_nLCDOffset)
			{
				g_nLCDOffset = pCurNode->y2 - WAP_DOC_HEIGHT;
				if(g_nLCDOffset < 0) g_nLCDOffset = 0;
			}
			
			//----------------------------------------------------
			//현 노드를 Focus로 바꾸고, Repaint한다
#ifdef WAP_NOWRAP
			//NoWrap timer------------------------------
			//g_pCurFocusNode = pCurNode;
			SetFocusNode(pCurNode);
#else
			g_pCurFocusNode = pCurNode;
#endif//#ifdef WAP_NOWRAP

			WAP_Display();

			//-------------------------------------------------------------
			// yjahn96 01.04.03 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96 01.03.21
			CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP

			//----------------------------------------------------
			//선택을 알리고 연결된 동작을 하도록 한다
//			load_incard  = TRUE;
//			loadingkind  = TRUE;

			SKY_keySelected( ((WAP_Content_Key *)pCurNode->pContent)->viewId,
							 ((WAP_Content_Key *)pCurNode->pContent)->keyId);
#ifndef WAP_USE_SECURE
			if(Wtlshello)
			{
				enc_use.viewid  = ((WAP_Content_Key *)pCurNode->pContent)->viewId;
				enc_use.id      = ((WAP_Content_Key *)pCurNode->pContent)->keyId;
				enc_use.wmlkind = 1;
			}
			else
#endif//#ifndef WAP_USE_SECURE
			{
#ifdef WAP_USE_DORM
				normal_use.viewid  = ((WAP_Content_Key *)pCurNode->pContent)->viewId;
				normal_use.id      = ((WAP_Content_Key *)pCurNode->pContent)->keyId;
				normal_use.wmlkind = 1;				
#endif//#ifdef WAP_USE_DORM
			}
			return TRUE;
		}
		pCurNode = pCurNode->pNextFocusNode;

	}//end of while
	return FALSE;
}

void GetCurrentImageInfo(int* frame, dword* size, int*imageType, int* nBitCount)
{
	WAP_Node *pCurNode = g_pCard->pHeadContent;

	*size = 0;
	*frame = 0;
    *imageType = 0;

	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_IMAGE
		|| pCurNode->type == _ELEMENT_TYPE_LINK_IMAGE)
		{
			WAP_Content_Image* pImage = (WAP_Content_Image *)pCurNode->pContent;

			//-------------------------------------
			//이미지 사이즈
			if(pImage->imageData != NULL)
			{
				*size = sizeof(pImage->imageData);
			}

			//-------------------------------------
			//프레임 갯수
			if(pImage->imageType == WAP_IMAGE_TYPE_BMP)
			{
                *imageType = WAP_IMAGE_TYPE_BMP;
				*frame = 1;//to monochrome				

				if(pImage->nBitCount == 1)//monochrome
				{
					*nBitCount = 1;//to monochrome				
				}
				else if(pImage->nBitCount == 4)//16 color
				{
					*nBitCount = 2;//to 4 gray
				}
				else if(pImage->nBitCount == 8)//256 color
				{
					*nBitCount = 8;//to 256 color
				}
			}
			if(pImage->imageType == WAP_IMAGE_TYPE_WBMP)
			{
                *imageType = WAP_IMAGE_TYPE_WBMP;
				*frame = 1;
				*nBitCount = 1;
			}
			else if(pImage->imageType == WAP_IMAGE_TYPE_SIS)
			{
                *imageType = WAP_IMAGE_TYPE_SIS;
				*frame = pImage->nTotalFrame;
				*nBitCount = pImage->nBitCount;
			}
		}

		pCurNode = pCurNode->pNextNode;
	}
}

BOOL CheckDiskSpaceForSetImage(void)
{
	int image_frame = 0;
	DWORD image_size = 0;
    int image_type = 0;
	int image_bitcount = 0;
	
	//file의 갯수가 full인지 check!!
    if(SKY_GetFileNumberInDataFile(SDF_IMAGE) >= SKY_MAX_NUM_FILES)
    {
		ERR("Filenum_Full",0,0,0);
		return FALSE;	//file의 갯수 full
	}
	else
	{
		DWORD nFileSize = 0 , nDiskSpace;

        //디스크 사이즈
        nDiskSpace = SKY_GetFreeSpaceInDataFile();

		//File 사이즈 = header 정보 + Image 사이즈
		GetCurrentImageInfo(&image_frame, &image_size, &image_type, &image_bitcount);

        if(image_type == WAP_IMAGE_TYPE_BMP || image_type == WAP_IMAGE_TYPE_WBMP)
        {
			if(image_bitcount == 1)	//B/W
			{
				// header + frame갯수 * (120/8*120)이미지 크기
    			nFileSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_BW_IMAGE_SIZE);        
			}
			else if(image_bitcount == 2)	//4 gray
			{
				// header + frame갯수 * (120/4*120)이미지 크기
    			nFileSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_4GRAY_IMAGE_SIZE);        
			}
			else if(image_bitcount == 8)	//256 color
			{
				// header + frame갯수 * (120*120)이미지 크기
    			nFileSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_256COLOR_IMAGE_SIZE);        
			}
        }
        else if(image_type == WAP_IMAGE_TYPE_SIS)
        {
			if(image_bitcount == 1)	// B/W
			{
				// header + frame갯수 * (120*120)이미지 크기
    			nFileSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_BW_IMAGE_SIZE); 
			}
			else if(image_bitcount == 2)	// 4 gray
			{
				// header + frame갯수 * (120*120)이미지 크기
    			nFileSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_4GRAY_IMAGE_SIZE); 
			}
			else if(image_bitcount == 8)	//256 color
			{
				// header + frame갯수 * (120*120)이미지 크기
    			nFileSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_256COLOR_IMAGE_SIZE); 
			}
        }

        // 저장할 공간이 있는지 확인
    	if(nDiskSpace < nFileSize )  return FALSE;	//저장할 공간이 없는 경우
		else                         return TRUE;	//저장할 공간이 있는 경우
	}
}

#ifdef	WAP_NOWRAP
void WAP_CheckKeyCard(void)
{
	WAP_Node* pCurNode;
	pCurNode = g_pCard->pHeadContent;

	g_pCard->isKeyCard = TRUE;//init

    #ifdef WAP_SCROLL_BAR //for Scroll bar - yjahn96 01.05.23
	g_pCard->nScrollLine = 0;
    #endif  //#ifdef WAP_SCROLL_BAR 

    while(pCurNode != NULL)
	{
        if(!(  pCurNode->type == _ELEMENT_TYPE_KEY
		    || pCurNode->type == _ELEMENT_TYPE_BR
			|| pCurNode->type == _ELEMENT_TYPE_CARD))
		{
            #ifdef WAP_SCROLL_BAR //for Scroll bar - yjahn96 01.05.23
        	g_pCard->nScrollLine = 0;
            #endif  //#ifdef WAP_SCROLL_BAR 
			g_pCard->isKeyCard = FALSE;//not keyCard
			return;
		}

        #ifdef WAP_SCROLL_BAR //for Scroll bar - yjahn96 01.05.23
        if(pCurNode->type == _ELEMENT_TYPE_KEY)
        	g_pCard->nScrollLine ++;
        #endif  //#ifdef WAP_SCROLL_BAR 

        pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}
}
#endif//#ifdef	WAP_NOWRAP




#ifdef FEATURE_SKT_EIF//EIF_WAP				// yjahn96 01.03.16
//=============================================================================
// EIF WAP 에서 N Key Loading 을 위해 
//=============================================================================
BOOL WAP_Handle_NumKey_ForDo_EIFWAP(int nKey)
{
	extern int NKey_keyID[10];							

	WAP_Node *pCurNode = g_pCard->pHeadFocus;

	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_KEY
		&& ((WAP_Content_Key *)pCurNode->pContent)->keyId == NKey_keyID[nKey - 1])
		{
			//----------------------------------------------------
			//현재 화면에 보이지 않는 경우, 화면을 이동한다
			if(pCurNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
			|| pCurNode->y2 <= g_nLCDOffset)
			{
				g_nLCDOffset = pCurNode->y2 - WAP_DOC_HEIGHT;
				if(g_nLCDOffset < 0) g_nLCDOffset = 0;
			}
			
			//----------------------------------------------------
			//현 노드를 Focus로 바꾸고, Repaint한다
#ifdef WAP_NOWRAP
			//NoWrap timer------------------------------
			//g_pCurFocusNode = pCurNode;
			SetFocusNode(pCurNode);
#else
			g_pCurFocusNode = pCurNode;
#endif//#ifdef WAP_NOWRAP
			WAP_Display();

			//-------------------------------------------------------------
			// yjahn96 01.04.03 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96 01.03.21
			CancelNowrapTimer();	//ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP

			//----------------------------------------------------
			//선택을 알리고 연결된 동작을 하도록 한다
//			load_incard  = TRUE;
//			loadingkind  = TRUE;

			SKY_keySelected( ((WAP_Content_Key *)pCurNode->pContent)->viewId,
							 ((WAP_Content_Key *)pCurNode->pContent)->keyId);
			if(Wtlshello)
			{
				enc_use.viewid  = ((WAP_Content_Key *)pCurNode->pContent)->viewId;
				enc_use.id      = ((WAP_Content_Key *)pCurNode->pContent)->keyId;
				enc_use.wmlkind = 1;
			}
			else
			{
#ifdef WAP_USE_DORM
				normal_use.viewid  = ((WAP_Content_Key *)pCurNode->pContent)->viewId;
				normal_use.id      = ((WAP_Content_Key *)pCurNode->pContent)->keyId;
				normal_use.wmlkind = 1;				
#endif//#ifdef WAP_USE_DORM
			}
			return TRUE;
		}
		pCurNode = pCurNode->pNextFocusNode;

	}//end of while
	return FALSE;
}


//=============================================================================
// EIF WAP 에서 Link Text UOPEN 을 위해 
//=============================================================================
BOOL WAP_Handle_LinkText_ForDo_EIFWAP(int nKey)
{
	WAP_Node *pCurNode = g_pCard->pHeadFocus;

	while(pCurNode != NULL)
	{
		if(pCurNode->type == _ELEMENT_TYPE_LINK_TEXT
		&& ((WAP_Content_Text *)pCurNode->pContent)->textId == nKey)
		{
			//----------------------------------------------------
			//현재 화면에 보이지 않는 경우, 화면을 이동한다
			if(pCurNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
			|| pCurNode->y2 <= g_nLCDOffset)
			{
				g_nLCDOffset = pCurNode->y2 - WAP_DOC_HEIGHT;
				if(g_nLCDOffset < 0) g_nLCDOffset = 0;
			}
			
			//----------------------------------------------------
			//현 노드를 Focus로 바꾸고, Repaint한다
#ifdef WAP_NOWRAP
			//NoWrap timer------------------------------
			//g_pCurFocusNode = pCurNode;
			SetFocusNode(pCurNode);
#else
			g_pCurFocusNode = pCurNode;
#endif//#ifdef WAP_NOWRAP
			WAP_Display();

			//-------------------------------------------------------------
			// yjahn96 01.04.03 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

			//----------------------------------------------------
			//선택을 알리고 연결된 동작을 하도록 한다

			SKY_textSelected( ((WAP_Content_Text *)pCurNode->pContent)->viewId, 
				              ((WAP_Content_Text *)pCurNode->pContent)->textId);

#ifdef FOR_MS
			if(Wtlshello)
			{
				enc_use.viewid  = ((WAP_Content_Text *)pCurNode->pContent)->viewId;
				enc_use.id      = ((WAP_Content_Text *)pCurNode->pContent)->textId;
				enc_use.wmlkind = 3;
//				return _ELEMENT_TYPE_WTLS_SESSION;
			}
			else
#endif//#ifdef FOR_MS
			{
#ifdef WAP_USE_DORM
				normal_use.viewid  = ((WAP_Content_Text *)pCurNode->pContent)->viewId;
				normal_use.id      = ((WAP_Content_Text *)pCurNode->pContent)->textId;
				normal_use.wmlkind = 3;
#endif//#ifdef WAP_USE_DORM
			}
			return TRUE;
		}
		pCurNode = pCurNode->pNextFocusNode;

	}//end of while
	return FALSE;

}

#endif		// #ifdef FEATURE_SKT_EIF//EIF_WAP

