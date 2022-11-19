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
// MMI �� Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"

//===================================================================================
//
// MMI�� Variable
//
//===================================================================================
WAP_Card *g_pCard = NULL;					// ���� Parsing�Ǿ� Display ���� ī�忡 ���� ������

BOOL g_bBlocked = FALSE;					// ��ü Blocked ��Ȳ (by MMIa_wait())

int g_nLCDOffset = 0;						// ���� ȭ��(Current Window)�� y ��ǥ(Offset)

int	g_nViewMode		= WAP_VIEW_MODE_WML;		// LCD ȭ�� ���
int	g_nMenuMode		= WAP_MENU_MODE_NONE;		// WAP Main Menu ���
int	g_nDlgMode		= WAP_DLG_MODE_NONE;		// WAP ��ȭ���� ���
int	g_nGraphicMode	= WAP_GRAPHIC_MODE_NONE;	// WAP Script�� Graphic ���

WAP_Node *g_pCurFocusNode = NULL;			// ���� ȭ�鿡�� Focus�� ���� ���� First�� Last ���̿� �����Ѵ�

// for �ɼ� ȭ���
WAP_Content_Select *g_pCurSelect = NULL;

// for �Է� ȭ���
WAP_Content_Input *g_pCurInput = NULL;

//for [����]Ű ��
WAP_Content_Key *g_pKeyUp = NULL;

//for ���̺�
int nDrawLinePosXForTable = 0;

//===================================================================================
//
// MMI�� Global Variable
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
	//���� Timer�� �����Ѵ�
    CancelAnimationTimer();	//stop sis animation
#ifdef WAP_NOWRAP	//yjahn96
    CancelNowrapTimer();//stop nowrap
#endif//#ifdef WAP_NOWRAP

	if(g_pKeyUp) g_pKeyUp = NULL;

	g_pCard->pHeadContent = NULL;

	//=====================================================================================
	// 2. ������ Card�� ���� ����Ÿ�� �����
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
	g_pCard = NULL; // �ʱ�ȭ
	return TRUE;
}
#endif //WAP_USE_LEAK

BOOL WAP_Terminate_WML(void)
{
	WAP_Node *pCurNode, *pTempNode;
	if(g_pCard == NULL) return FALSE;
	
	//---------------------------------------------------------------------------
	//���� Timer�� �����Ѵ�
	CancelAnimationTimer(); //ui_sistimer_set(0, FALSE);//stop sis animation

#ifdef WAP_NOWRAP	//yjahn96
	CancelNowrapTimer();    //	ui_nowrap_set(0, FALSE);//stop nowrap
#endif//#ifdef WAP_NOWRAP

	//---------------------------------------------------------------------------
	//[����]Ű�� ������ ��� �̸� �����
	if(g_pKeyUp)
	{
		if(g_pKeyUp->eventType != NULL)		wip_free((void *)g_pKeyUp->eventType);
		if(g_pKeyUp->label != NULL)			wip_free((void *)g_pKeyUp->label);

		wip_free((void *)g_pKeyUp);
		g_pKeyUp = NULL;
	}
	
	//=====================================================================================
	// 1. �� ���� ���� ����Ÿ�� �����
	//=====================================================================================
	pCurNode = g_pCard->pHeadContent;

#ifdef DOG_GABGA
	g_pCard->pHeadContent = NULL;
#endif//#ifdef DOG_GABGA

	while(pCurNode != NULL)
	{
		//===========================================================
		// 1-1. �켱 �� ����� ����Ʈ�� ���� �����
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
					// B1 project������ color SIS�� �����Ͽ�, imageData�� ���۸� �ٷ� �Ҵ���
					// ����, image Type�� SIS�� ���� wip_free()�� ���� �ʴ´�.
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

					//Select�� �޸� Option�� OptionGroup�� ���� ó���Ѵ�
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

					//Table�� �޸� Table Data�� ���� ó���Ѵ�
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
									// B1 project������ color SIS�� �����Ͽ�, imageData�� ���۸� �ٷ� �Ҵ���
									// ����, image Type�� SIS�� ���� wip_free()�� ���� �ʴ´�.
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
		// 1-2. ������ �� ��带 �����
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
	// 2. ������ Card�� ���� ����Ÿ�� �����
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
	
	g_pCard = NULL; // �ʱ�ȭ
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
	// ���ο� ��带 �ϳ� �����
	
	pNewNode = (WAP_Node *)wip_malloc(sizeof(WAP_Node));

	//------------------------------------------------------------------
	// �ƱԸ�Ʈ�� ���� ���� ����� ����(type, pContent)�� �Է��Ѵ�
	pNewNode->type = type;
	pNewNode->pContent = pContent;
	pNewNode->pPrevNode = NULL;
	pNewNode->pNextNode = NULL;
	pNewNode->pPrevFocusNode = NULL;
	pNewNode->pNextFocusNode = NULL;

	//------------------------------------------------------------------
	// �� ����� ��Ʈ Height�� ���Ѵ�
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
	// �� ��带 ����Ʈ(pHeadContent)�� �������� �߰��Ѵ�
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
	// �� ��尡 ���� ������(Focus ����) �����,
	// �� ��带 ���� ����Ʈ(pHeadFocus)�� �������� �߰��Ѵ�
	
	if(pNewNode->type == _ELEMENT_TYPE_KEY
	|| pNewNode->type == _ELEMENT_TYPE_INPUT
	|| pNewNode->type == _ELEMENT_TYPE_SELECT
	|| pNewNode->type == _ELEMENT_TYPE_LINK_TEXT
	|| pNewNode->type == _ELEMENT_TYPE_LINK_IMAGE
	//TableRow�� Focus ������ Element�� ���� �� �����Ƿ� ���ð��� ����Ʈ�� �����Ѵ�.
	//Formatting��, Focus ������ Element�� ���� ��� �� ����Ʈ���� ������ ���̴�
	|| pNewNode->type == _ELEMENT_TYPE_TABLE_ROW)//���ð����� type���� üũ
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
	// ���ο� ��带 �ϳ� �����
	WAP_Node *pNewNode; 
	pNewNode = (WAP_Node *) wip_malloc(sizeof(WAP_Node));

	//------------------------------------------------------------------
	// �ƱԸ�Ʈ�� ���� ���� ����� ����(type, pContent)�� �Է��Ѵ�
	pNewNode->type = type;
	pNewNode->pContent = pContent;
	pNewNode->pPrevNode = NULL;
	pNewNode->pNextNode = NULL;
	pNewNode->pPrevFocusNode = NULL;
	pNewNode->pNextFocusNode = NULL;
	
	//------------------------------------------------------------------
	// �� ����� ��Ʈ Height�� ���Ѵ�
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
	// �� ��带 ����Ʈ(g_pCurTableRow )�� �������� �߰��Ѵ�
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
	// �� ��尡 ���� ������(Focus ����) �����,
	// �� ��带 ���� ����Ʈ(pHeadFocus)�� �������� �߰��Ѵ�
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
	int nMaxHeight = WAP_ASCII_FONT_HEIGHT;//1 ����(13)
	
	WAP_Node *pCurNode;
	WAP_Content_TableRow* pTableRow = (WAP_Content_TableRow *)pTableNode->pContent;
		
	//-----------------------------------------------------------------------------------
	// ������ Table Row�� Focus Cell�� HeadFocus�� �ʱ�ȭ�Ѵ�
	//-----------------------------------------------------------------------------------
	pTableRow->pFocusCell = pTableRow->pHeadFocusCell;
	
	//-----------------------------------------------------------------------------------
	// ������ Table Row�� Column �� ���� �� Height �� ���Ѵ�
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
				else//�̹��� ����Ÿ�� ���ų� ó���� �� ���� ���,Alt Text�� height�� ó���Ѵ�
				{
					point pt;
					pt = Calculate_Node_EndPoint(pTableContent->altText, 0, 0,
												WAP_FONT_NORMAL, pTableRow->noOfColumns);
					nTemp = pt.y;
				}

				//�̹����� ���̸� 13�� ����� �����ش�
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
	// �� Column�� Node�� Format �Ѵ�
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
		// �� ����� x2, y2 ��ǥ ����
		// �� ��尡 Line Break�� Ư���� ������, (x2<-WAP_LCD_WIDTH)
		//------------------------------------------------------------------ 
		if(pCurNode->type == _ELEMENT_TYPE_CARD)
		{
			//Card�� (0, 0)���� �����Ѵ�
			if(pCurNode->x1 != 0)
			{
				pCurNode->x1 = 0;
				pCurNode->y1 = 0;
			}
			//Card�� Title�� ������ ȭ�鿡 �׷����� �ʴ´�
			//�� Formatting�� �� �ʿ䵵 ����
			if(g_pCard->title == NULL)
			{
				pCurNode->x2 = 0;
				pCurNode->y2 = pCurNode->y1 + pCurNode->font.height;
			}
			else
			{
				//Card�� (95, 13)���� ������. �� �� �� ���� ǥ�õȴ�
				pCurNode->x2 = WAP_LCD_WIDTH-1;//95
				pCurNode->y2 = pCurNode->y1 + pCurNode->font.height;
			}
		}
		else if(pCurNode->type == _ELEMENT_TYPE_KEY)
		{
			//Key�� �� ���ο��� �����Ѵ�
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

			//������ �� ���γ��� ǥ�õǴ� ���� �̹����� ���, ���� ���ο��� ǥ���� �ش�
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
				
				//�̹����� ���̸� 6�� ����� �����ش�
				if(((pImage->width)%WAP_ASCII_FONT_WIDTH) != 0)
					pCurNode->x2 += (WAP_ASCII_FONT_WIDTH - ((pImage->width)%WAP_ASCII_FONT_WIDTH));
			}
			else
			{
				//Image�� �� ���ο��� �����Ѵ�
				if(pCurNode->x1 != 0)
				{
					pCurNode->x1 = 0;
					pCurNode->y1 = ((WAP_Node *)pCurNode->pPrevNode)->y2;
				}
		

				pCurNode->x2 = WAP_LCD_WIDTH-1;//95

                //�̹����� LCD height(120)���� ū ���, y2�� ����
                if(pImage->height <= 117) //13*9
                    pCurNode->y2 = pCurNode->y1 + pImage->height;
                else
                    pCurNode->y2 = pCurNode->y1 + 117;

				//�̹����� ���̸� 13�� ����� �����ش�
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

			//Table�� Row�� �� ���ο��� �����Ѵ�
			if(pCurNode->x1 != 0)
			{
				pCurNode->x1 = 0;
				pCurNode->y1 = ((WAP_Node *)pCurNode->pPrevNode)->y2;
			}

			//���� Table Row �� ���� �� Column�� ����
			nMaxHeight = WAP_Formatting_TableRow(pCurNode);
			
			pCurNode->x2 = WAP_LCD_WIDTH-1;//95
			pCurNode->y2 = pCurNode->y1 + nMaxHeight;

			//---------------------------------------------------------------
			//TableRow�� Focus �ʱ�ȭ
			//---------------------------------------------------------------
			if(g_pCurTableRow == NULL)
				g_pCurTableRow = (WAP_Content_TableRow *)pCurNode->pContent;

			//---------------------------------------------------------------
			//�� TableRow�� Element�� Focus�� �ϳ��� ���ٸ�, �̸� Focus ��忡�� �����Ѵ�
			if(((WAP_Content_TableRow *)pCurNode->pContent)->pHeadFocusCell == NULL)
			{
				//�� TableRow�� ���� Focus ����� ���
				if(g_pCard->pHeadFocus == pCurNode)
				{
					//���� ��Ŀ�� ��尡 ���� ���(��Ŀ�� ��尡 �ϳ��� ���� ���)
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
					//���� ��Ŀ�� ��尡 ���� ���
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

    		//������ ��尡 �� ���ο��� ���۵Ǵ� Br�� ���, �����ش�
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
				//�׿��� x�� 0�� Br��, ���� ��ó�� ó���� �ش�
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
			//Input�� Select�� �� ���ο��� �����Ѵ�
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

			//�� ��尡 �� ���ο� ������ ���, ������ ��ŭ �÷��ش�
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

			//�� ��尡 �� ���ο� ������ ���, ������ ��ŭ �÷��ش�
			if(pCurNode->y2 == (pCurNode->y1 + pCurNode->font.height))
				pCurNode->x2 += pCurNode->x1;
		}

		//------------------------------------------------------------------
		// ���� ����� x1, y1 ��ǥ ����
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

	//���� ����Ÿ�� copy�Ѵ�
	strcpy(pDestination, pSource);

	//nVisibleSize�� ��ȿ���� ������, ���� ����Ÿ�� �״�� ����Ѵ�
	if(nVisibleSize <= 0) return;
	
	//init
	nTotCount = 0;
	nHanCount = 0;
	pText = pDestination;

	//nVisibleSize��ŭ�� ���� ����Ÿ�� copy�Ѵ�
	while(nTotCount < nVisibleSize)
	{
		if(*pText == 0) break;
		wChar = *pText;

		if ( wChar & 0x80 )//�ѱ� ó��
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
			//Input element�� visible size��ŭ�� display�Ѵ�
			memset(text, '\0', WAP_MAX_TEXT);
			GetVisibleInputData(pContent->text, text, pContent->size);
			//-----------------------------------------------------------

			memset(temp, '\0', 500);
			if(pContent->title == NULL)
				sprintf(temp, "%s�Է�:[%s]", SF_DOWN_RIGHT_PENCIL, text);
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
				sprintf(temp, "\242\321����:[%s]", pContent->selectedOptions);
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
	// �� ĳ������ ���� x ���� ���Ͽ� "����� ũ��" �����Ͽ� loop�� ����
	// loop�� ���� ���÷����� ���ڰ� ���� ��� ������ �� �ִ�
	// ĳ���� ���� �ִ밪�� 20(=120/6)�̴�
	for(i=(nLoopCount-x/nAsciiFontWidth)/nDivideWidth; i>0; i--)
	{
		/*=================================================*/
		//line break ó�� (���� ���� ���)
		/*=================================================*/
		if(((*str & 0x80) //�ѱ�
			&& (nTextWidth+nHanFontWidth > WAP_LCD_WIDTH/nDivideWidth))
		  ||(!(*str & 0x80) //�ƽ�Ű
		    && (nTextWidth+nAsciiFontWidth > WAP_LCD_WIDTH/nDivideWidth)))
		{
			return(nTextWidth - nOriginX);
		}

		if(*str == 0) break;
		wChar = *str;

		/*=================================================*/
		if ( wChar & 0x80 )              /* �ѱ� ó��      */
		/*=================================================*/
		{	  
			str++; i--;
			if (*str == 0) break;
			wChar <<= 8; wChar += *str;

			nCharWidth = nHanFontWidth;
		}
		/*=================================================*/
		else							/* �ƽ�Ű ó��     */
		/*=================================================*/
		{
			nCharWidth = nAsciiFontWidth;
		}

		/*=================================================*/
		// �� ���� ǥ�� ���� ó��
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
	// loop�� ���� ���÷����� ���ڰ� ���� ��� ������ �� �ִ�
	for(i=0; ; i++)
	{
		//================================================================
		// �� ���� ó�� �� ����
		if(*new_scr_ptr == 0) break;
		
		if(i!=0) x=0;//not frist line
		
		//================================================================
		// �� ������ ó���Ѵ�
		curWordCount = Calculate_WordCount_OneLine(new_scr_ptr, x, y, nFontType, nDivideWidth);
		
		//================================================================
		// �� ���� ó�� �� ����
		y += nAsciiFontHeight;//y �� ����

		new_scr_ptr = new_scr_ptr + curWordCount/nAsciiFontWidth;

	}//end of for
		
	pt.x = curWordCount;
	pt.y = y;

	return (pt);
}

point Calculate_Node_StartPoint(WAP_Node* pPrevNode, WAP_Node* pCurNode)
{
	point result;
	if((pPrevNode->x2 >= WAP_LCD_WIDTH-1) // ���ο� ����
	||
		//----------------------
		//for dual fonts
		//	���� ���� ���� ����� Font�� �ٸ���,
		//	���� ������ ���� ��Ʈ�� �� ���ڸ� ǥ����  ������ ���� ���
		//	�� �������� �ٲ㼭 ��ǥ���� �����Ѵ�
		(pPrevNode->font.font_index != pCurNode->font.font_index
		&& WAP_LCD_WIDTH - pPrevNode->x2 < pCurNode->font.width*2))//for double buffering
	{
		result.x = 0;	  	

		//----------------------
		//for dual fonts
		//	���� ��尡 Medium Font�� ���, ���� ��� �� �ƴ϶�
		//	���� ���� ���� ���ο� Normal Font�� �����ϴ� �� Ȯ���ؾ� �Ѵ�
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
	else							// ���ӵ� ����
	{
		if(	//----------------------
			//for dual fonts
			//	���� ���� ���� ����� Font�� �ٸ���,
			//	���� ������ ���� ����� ���ڸ� ǥ���� �� �ִ� ���
			(pPrevNode->font.font_index != pCurNode->font.font_index
			&& WAP_LCD_WIDTH - pPrevNode->x2 > pCurNode->font.width))
		{
			result.x = pPrevNode->x2;
			//----------------------
			//for dual fonts
			//	���� ��尡 Medium Font�� ���, ���� ��� �� �ƴ϶�
			//	���� ���� ���� ���ο� Normal Font�� �����ϴ� �� Ȯ���ؾ� �Ѵ�
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
	// �ִϸ��̼� �̸����⿡�� animation timer���� ������,
	// scroll���� key�Է��� ������ ȭ���� �������� ���� ��������
	// ��� animation timer�� stop�� restart��Ŵ - yjahn96 01.04.10
	CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();

	//------------------------------------------------------------------
	// ���� ȭ���� �����ϴ� ������ ���������� �׷��ش�
	// �� g_pFirst ���� g_pLast ���� �׷��ش�
	//------------------------------------------------------------------
	pCurNode = g_pCard->pHeadContent;
	while(pCurNode != NULL && pCurNode->y2 <= g_nLCDOffset)
	{
		pCurNode = pCurNode->pNextNode;
	}
	
	while(pCurNode != NULL && pCurNode->y1 < g_nLCDOffset+WAP_DOC_HEIGHT)
    {

		//------------------------------------------------------------------
	   	// ������ ��� type�� ���� �� ��带 �׷��ִ� �Լ��� �θ���
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
				break;	//Unknown �� ó������ �ʴ´�

		};// end of switch

		pCurNode = (WAP_Node *)pCurNode->pNextNode;

     }// end of while


#ifdef WAP_SCROLL_BAR //for Draw Scroll bar - yjahn96 01.05.23
    if( g_pCard->isKeyCard)
    {
        if((l_ExistCard && g_pCard->nScrollLine > 6) //card������ 6 lines���� ū���
            || (!l_ExistCard && g_pCard->nScrollLine > 7))  //card������ 7 lines���� ū ���
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

	//handset task�� display�� ��û�Ѵ�
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
	//��ü Clear
    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//WAP_Display_Image_CML(WAP_OK_CML/*WAP_UPDOWN_CML*/, WAP_CML_OKEMPTY/*WAP_CML_UPDOWNEMPTY*/ ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OKEMPTY, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CETEREMPTY , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_RIGHTEMPTY ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// ȭ��Ʒ� ����
	WAP_Display_Line(2, WAP_DOC_HEIGHT, WAP_LCD_WIDTH - 3, WAP_DOC_HEIGHT, BLACK);
    
	//===========================================================================
	// ���� Ű(�޴�)
    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_MENU , WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// �߰� Ű
	if(g_pCurFocusNode == NULL 
	|| g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset
	// Focus�� ���� TableRow�� ���
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
			case _ELEMENT_TYPE_INPUT:   //�Է�
                WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_INPUT, WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
				break;

			case _ELEMENT_TYPE_SELECT:  //����
                WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CSELECT, WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
                break;

			case _ELEMENT_TYPE_KEY:     //����
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
	// ������ Ű(����)
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_HIGH, WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// ��ũ�� Ű
	pLastNode = g_pCard->pHeadContent;
	while(pLastNode != NULL && pLastNode->y2 <= g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		pLastNode = pLastNode->pNextNode;
	}

	if((g_nLCDOffset == 0  &&  pLastNode == NULL)//1 ȭ�鸸 ����
		&& 
			//Focus ��尡 ���ų�, 1�� �����ϹǷ� ��ũ�� �� �ʿ����
			((g_pCard->pHeadFocus == NULL || g_pCard->pHeadFocus->pNextFocusNode == NULL)
			||
			// Focus�� ���� TableRow�� ���
			(g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
			&& ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL))
	)
	{
        if(l_midKeyExist)   //�߰�Ű�� �ְ�, ��ũ���� �ƴ� ���
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);

	}
	else
	{
		//���� ó�� ������
		if( g_nLCDOffset == 0 
		 && 
		 (  (g_pCard->pHeadFocus == NULL)
		 || (g_pCard->pHeadFocus != NULL
		     && (  g_pCard->pHeadFocus->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
		         || g_pCard->pHeadFocus == g_pCurFocusNode))))
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_DOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);

        }
		//���� ������ ������
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
		//�߰� ������
		else
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
        }
	}

	//===========================================================================
	//����Ʈ Ű�� ���� ������ ������ ���� ���, �̸� �׷��ش� 
/*	if(nDrawLinePosXForTable > 0)
	{
		WAP_Display_Line(0,						 WAP_SOFTKEY_Y_POS,//(0,52)
						nDrawLinePosXForTable+1, WAP_SOFTKEY_Y_POS,//(x,52)
						WAP_BLACK); //������ ����
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

	WORD pSoftkey_Menu[2]	= {0x84f7, 0x84f8};// �޴�

	WORD pSoftkey_Input[2]	= {0x86d4, 0x86d5};// �Է�
	WORD pSoftkey_Select[2]	= {0x84fe, 0x84ff};// ����
	WORD pSoftkey_Key[2]	= {0x88e8, 0x88e9};// ����
	WORD pSoftkey_Link[2]	= {0x88ea, 0x88eb};// ����

	WORD pSoftkey_Upper[2]	= {0x89d0, 0x89d1};// ����
	WORD pSoftkey_Blank[1]	= {0x81bf};

	WORD* pSoftkey_Left = NULL;
	WAP_Node *pLastNode;
	
	extern /*ROM*/ BYTE* /*EQS_API*/ GetBitmapFontBufferByCode(WORD wCode);	//yjahn96 00.12.28

	//�¿찡����� Clear
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
	// ���� Ű(�޴�)
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
	// �߰� Ű
	if(g_pCurFocusNode == NULL 
	|| g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset
	// Focus�� ���� TableRow�� ���
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
	// ������ Ű(����)
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
	// ��ũ�� Ű
	pLastNode = g_pCard->pHeadContent;
	while(pLastNode != NULL && pLastNode->y2 <= g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		pLastNode = pLastNode->pNextNode;
	}

	if((g_nLCDOffset == 0  &&  pLastNode == NULL)//1 ȭ�鸸 ����
		&& 
			//Focus ��尡 ���ų�, 1�� �����ϹǷ� ��ũ�� �� �ʿ����
			((g_pCard->pHeadFocus == NULL || g_pCard->pHeadFocus->pNextFocusNode == NULL)
			||
			// Focus�� ���� TableRow�� ���
			(g_pCurFocusNode->type == _ELEMENT_TYPE_TABLE_ROW
			&& ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL))
	)
	{
	}
	else
	{
		WORD pSoftkey_Move[3]	= {0x86b1, 0x88ed, 0x88ee};// �̵�

		//���� ó�� ������
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
		//���� ������ ������
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
		//�߰� ������
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
	//����Ʈ Ű�� ���� ������ ������ ���� ���, �̸� �׷��ش� 
	if(nDrawLinePosXForTable > 0)
	{
		WAP_Display_Line(0,						 WAP_SOFTKEY_Y_POS,//(0,52)
						nDrawLinePosXForTable+1, WAP_SOFTKEY_Y_POS,//(x,52)
						WAP_BLACK/*������ ����*/);
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
	// Focus �̵��� ������ ���, Focus �̵��� �켱������ �����Ѵ�
	// Focus �̵��� ���� ���, Window �̵��� �����Ѵ�

	//------------------------------------------------------------------
	// ��, �� ȭ�� ������  Focus �̵��� ���� �Ŀ�, 
	// Focus �� ��尡 ȭ�鿡 ���� ǥ�õ��� �ʴ� ��쿡��
	// Window �̵��� �߰��� �̷������ �Ѵ�
	// �� Focus �̵����� �̵��� ��尡 ȭ�鿡 ���� ǥ�õǾ����� �Ѵ�

	//------------------------------------------------------------------
	//Window ���� focus �̵��� �켱�Ѵ�
	if(WAP_FocusScrolling(bDown))
	{
		WAP_Display();
	}
	//------------------------------------------------------------------
	//focus �̵��� ���� ���, Window �̵��� �Ѵ�
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
	// Focus ��尡 �ϳ��� ���� ���, focus �̵� ����
	//------------------------------------------------------------------
	if(g_pCurFocusNode == NULL) return FALSE;//ȭ���̵� �Ѵ�
	
	//==================================================================
	// �������� Focus ��尡 �ϳ��� ���� ���, focus �̵� ����
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
		return FALSE;//ȭ���̵� �Ѵ�
	}
	//------------------------------------------------------------------
	// ���� Focus ���� �̵��Ѵ� (Focus Link�� �������� �̵�)
	//------------------------------------------------------------------

	//------------------------------------------------------------------
	// case 1) ���� ���� ���� Focus ��尡 ���� ���
	if(g_pCurFocusNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset)
	{
		//������ ���ο� ȭ��(g_nLCDOffset+13)�� �� ��Ŀ�� ��尡 ���δ�
		if(! (  g_pCurFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		     || g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			;
		}
		//������ ���ο� ȭ��(g_nLCDOffset+13)�� ���� ��Ŀ�� ��尡 ���δ�
		else if(pNextFocusNode != NULL
		&& ! (  pNextFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		     || pNextFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
		}
		return FALSE;//ȭ���̵� �Ѵ�
	}

	//------------------------------------------------------------------
	//case 2) ���� ȭ�鿡 ����
	else if(g_nLCDOffset < g_pCurFocusNode->y2 
	     && pNextFocusNode->y1 < g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		BOOL bSuccess = ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
		if(!bSuccess) return FALSE;//ȭ���̵� �Ѵ�
		
		//------------------------------------
		//for dual fonts
		if(g_pCurFocusNode->y1 + g_pCurFocusNode->font.height > g_nLCDOffset + WAP_DOC_HEIGHT)
			return FALSE;//ȭ���̵� �Ѵ�
		//------------------------------------

		return TRUE;//ȭ���̵� �� �Ѵ�
	}
	//------------------------------------------------------------------
	//case 3) ����� ���
	/*else if(g_pCurFocusNode->y2 - g_pCurFocusNode->font.height == pNextFocusNode->y1
		|| 
		(g_pCurFocusNode->y2 == pNextFocusNode->y1
		   && g_pCurFocusNode->x2 >= WAP_LCD_WIDTH-1//95
		   && pNextFocusNode->x1 == 0 ))
	{
		ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);	
		return FALSE;//ȭ���̵� �Ѵ�
	}*/
	//------------------------------------------------------------------
	//case 4) �׿��� ���
	else
	{
		//�� ��Ŀ�� ����� ���� ������ ���ο� �ɸ� ���
		if(g_pCurFocusNode->y2 == g_nLCDOffset + WAP_DOC_HEIGHT)
		{
			//������ ���ο� ȭ��(g_nLCDOffset+13)���� �� ��Ŀ�� ��尡 ���δ�
			if(! ( g_pCurFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				//����� ���
				if( (g_pCurFocusNode->y2 - g_pCurFocusNode->font.height == pNextFocusNode->y1
				    || 
					(g_pCurFocusNode->y2 == pNextFocusNode->y1
					   && g_pCurFocusNode->x2 >= WAP_LCD_WIDTH-1//95
					   && pNextFocusNode->x1 == 0 ))
				&& 
				//�� ��尡 1 ������ ���
					g_pCurFocusNode->y2 - g_pCurFocusNode->y1 == WAP_ASCII_FONT_HEIGHT)
				{
					ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
				}
			}
			//������ ���ο� ȭ��(g_nLCDOffset+13)�� ���� ��Ŀ�� ��尡 ���δ�
			else if(! (pNextFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		        || pNextFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
			}
		}
		//������ ���ο� ȭ��(g_nLCDOffset+13)���� �� ��Ŀ�� ��尡 ���δ�
		else if(! (g_pCurFocusNode->y2 <= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			;
		else
		{
			ChangeFocusNode(TRUE, g_pCurFocusNode->pNextFocusNode);
		}
		return FALSE;//ȭ���̵� �Ѵ�
	}
}

BOOL WAP_FocusScrolling_Up(void)
{
	WAP_Node *pPrevFocusNode;
	pPrevFocusNode = g_pCurFocusNode->pPrevFocusNode;

	//------------------------------------------------------------------
	// Focus ��尡 �ϳ��� ���� ���, focus �̵� ����
	//------------------------------------------------------------------
    if(g_pCurFocusNode == NULL) return FALSE;//ȭ���̵� �Ѵ�
	
	//==================================================================
	// �������� Focus ��尡 �ϳ��� ���� ���, focus �̵� ����
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
			// Element������ LCD�� �ִ� display Line��(4) ���� ���� ���,
			// Scroll up�� ȭ���̵��� ���� �ʴ´�.  -- yjahn96 00.12.20
			//------------------------------------------------------------
			if(g_pCurFocusNode->y2 <= WAP_DOC_HEIGHT)
				return TRUE;	//ȭ���̵� �� �Ѵ�
		}
#endif //#ifdef	WAP_NOWRAP	
		return FALSE;//ȭ���̵� �Ѵ�
	}
	//------------------------------------------------------------------
	// ���� Focus ���� �̵��Ѵ� (Focus Link�� �������� �̵�)
	//------------------------------------------------------------------
	
	//------------------------------------------------------------------
	// case 1) ���� ���� ���� Focus ��尡 ���� ���
	if(g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset)
	{
		//������ ���ο� ȭ��(g_nLCDOffset-13)�� �� ��Ŀ�� ��尡 ���δ�
		if(! (  g_pCurFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		     || g_pCurFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			;
		}
		//������ ���ο� ȭ��(g_nLCDOffset-13)�� ���� ��Ŀ�� ��尡 ���δ�
		else if(pPrevFocusNode != NULL
		&& ! (  pPrevFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		     || pPrevFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
		{
			ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		}
		return FALSE;//ȭ���̵� �Ѵ�
	}

	//------------------------------------------------------------------
	//case 2) ���� ȭ�鿡 ����
	else if(g_nLCDOffset < pPrevFocusNode->y2 
	     && g_pCurFocusNode->y1 < g_nLCDOffset + WAP_DOC_HEIGHT)
	{
		BOOL bSuccess = ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		if(!bSuccess) return FALSE;//ȭ���̵� �Ѵ�

		//------------------------------------
		//for dual fonts
		if(g_pCurFocusNode->y1 < g_nLCDOffset)
			return FALSE;//ȭ���̵� �Ѵ�
		//------------------------------------

		return TRUE;//ȭ���̵� �� �Ѵ�
	}
	//------------------------------------------------------------------
	//case 3) ����� ���
	/*else if(pPrevFocusNode->y2 - pPrevFocusNode->font.height == g_pCurFocusNode->y1
		|| 
		(pPrevFocusNode->y2 == g_pCurFocusNode->y1
		   && pPrevFocusNode->x2 >= WAP_LCD_WIDTH-1//95
		   && g_pCurFocusNode->x1 == 0 ))
	{
		ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		return FALSE;//ȭ���̵� �Ѵ�
	}*/
	//------------------------------------------------------------------
	//case 4) �׿��� ���
	else
	{
		//�� ��Ŀ�� ����� ������ ó�� ���ο� �ɸ� ���
		if(g_pCurFocusNode->y1 == g_nLCDOffset)
		{
			//������ ���ο� ȭ��(g_nLCDOffset-13)���� �� ��Ŀ�� ��尡 ���δ�
			if(! ( g_pCurFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				//����� ���
				if(	(pPrevFocusNode->y2 - pPrevFocusNode->font.height == g_pCurFocusNode->y1
					|| 
					(pPrevFocusNode->y2 == g_pCurFocusNode->y1
						&& pPrevFocusNode->x2 >= WAP_LCD_WIDTH-1//95
						&& g_pCurFocusNode->x1 == 0 ))
				&& 
				//�� ��尡 1 ������ ���
					g_pCurFocusNode->y2 - g_pCurFocusNode->y1 == WAP_ASCII_FONT_HEIGHT)
				{
					ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
				}
			}
			//������ ���ο� ȭ��(g_nLCDOffset-13)�� ���� ��Ŀ�� ��尡 ���δ�
			else if(! (pPrevFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		        || pPrevFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			{
				ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
			}
		}
		//������ ���ο� ȭ��(g_nLCDOffset-13)���� �� ��Ŀ�� ��尡 ���δ�
		else if(! (g_pCurFocusNode->y2 <= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT
		        || g_pCurFocusNode->y1 >= g_nLCDOffset-WAP_ASCII_FONT_HEIGHT + WAP_DOC_HEIGHT))
			;
		else
		{
			ChangeFocusNode(FALSE, g_pCurFocusNode->pPrevFocusNode);
		}
		return FALSE;//ȭ���̵� �Ѵ�
	}
}

LOCAL BOOL ChangeFocusNode(BOOL bDown, WAP_Node* pNewFocusNode)
{
	WAP_Content_TableRow *pTableRow;
	
	if(pNewFocusNode->type != _ELEMENT_TYPE_TABLE_ROW)
	{
#ifdef	WAP_NOWRAP
		//���ο� Focus ���� �̵��Ѵ�
		//NoWrap timer------------------------------
		//g_pCurFocusNode = pNewFocusNode;
		SetFocusNode(pNewFocusNode);
#else
		//���ο� Focus ���� �̵��Ѵ�
		g_pCurFocusNode = pNewFocusNode;
#endif//#ifdef	WAP_NOWRAP
		return TRUE;
	}

	//���ο� Focus ��尡 TableRow�� ���
	pTableRow = (WAP_Content_TableRow *)pNewFocusNode->pContent;

	//�� TableRow�� Focus�� �ϳ��� ����
	if(pTableRow->pHeadFocusCell == NULL)
	{
		//Focus ���� �̵����� �ʴ´�
		//�� TableRow�� Focus�� �����Ѵ�
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
		//���ο� Focus ���(TableRow)�� �̵��Ѵ�
		//NoWrap timer------------------------------
		//g_pCurFocusNode = pNewFocusNode;
		SetFocusNode(pNewFocusNode);
#else
		//���ο� Focus ���(TableRow)�� �̵��Ѵ�
		g_pCurFocusNode = pNewFocusNode;
#endif//#ifdef	WAP_NOWRAP
		g_pCurTableRow = (WAP_Content_TableRow *)g_pCurFocusNode->pContent;

		return TRUE;
	}
}

#ifdef	WAP_NOWRAP
char tmpText[WAP_MAX_TEXT/(WAP_HS_SCRN_WIDE-2)+2][WAP_HS_SCRN_WIDE-1];	//16�ھ� ���� �� �ִ� 19byte¥�� buffer ���� : (256/16)+2 = 18 
void SetFocusNode(WAP_Node* pNewFocusNode)
{
	//label���� check!!
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

		//label���̰� 18�̻��� �� ã��!!!
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
	//  ���� ȭ���� �������� �ʴ´� (���� ȭ���� ������ ȭ���̴�)
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
	// ���� ȭ������ �̵��Ѵ�
	//======================================================================
	else
	{
		//----------------------------------------------------------
		//for dual fonts
		//���� ���� ����� font height��ŭ �̵��Ѵ�
		WAP_Node *pCurNode = g_pCard->pHeadContent;
		while(pCurNode != NULL)
		{
			//���� ȭ���� ���� ��� ã��
			if((pCurNode->y1 <= g_nLCDOffset && pCurNode->y2 > g_nLCDOffset)
			//���� ��尡 ������, ���� ������ ���� ������ ���� ������ ��� ã��
			&& (pCurNode->pNextNode == NULL || pCurNode->pNextNode->y1 > g_nLCDOffset))
			{
				if(pCurNode->font.font_index == WAP_FONT_MEDIUM)
				{
					//----------------------
					//for dual fonts
					//	���� ��尡 Medium Font�� ���, ���� ��� �� �ƴ϶�
					//	���� ���� ���� ���ο� Normal Font�� �����ϴ� �� Ȯ���ؾ� �Ѵ�
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
	//  ���� ȭ���� �������� �ʴ´� (���� ȭ���� ù ȭ���̴�)
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
	// ���� ȭ������ �̵��Ѵ�
	//======================================================================
	else
	{
		//----------------------------------------------------------
		//for dual fonts
		//���� ���� ����� font height��ŭ �̵��Ѵ�
		WAP_Node *pCurNode = g_pCard->pHeadContent;
		while(pCurNode != NULL)
		{
			//���� ȭ���� ���� ��� ã��
			if(pCurNode->y1 < g_nLCDOffset && pCurNode->y2 >= g_nLCDOffset)
			{
				if(pCurNode->font.font_index == WAP_FONT_MEDIUM)
				{
					//----------------------
					//for dual fonts
					//	���� ��尡 Medium Font�� ���, ���� ��� �� �ƴ϶�
					//	���� ���� ���� ���ο� Normal Font�� �����ϴ� �� Ȯ���ؾ� �Ѵ�
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
	// TableRow�� ��/�� ��ũ���� �������� üũ�Ѵ�
	//------------------------------------------------------------------
	if(//Focus ��尡 �ϳ��� ���� ���
		g_pCurFocusNode == NULL
   	   //Focus ��尡 ���� ȭ�鿡 ���� ���
	|| (g_pCurFocusNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
	    || g_pCurFocusNode->y2 <= g_nLCDOffset)
	   //Focus ��尡 Table�� �ƴ� ���
	|| g_pCurFocusNode->type != _ELEMENT_TYPE_TABLE_ROW
	   //Focus�� Table�� Link�� �ϳ��� ������ ���� ���
	|| ((WAP_Content_TableRow *)g_pCurFocusNode->pContent)->pHeadFocusCell == NULL)
		return;
	
	//------------------------------------------------------------------
	// �� ��Ŀ�� Column�� �̵��Ѵ�
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
//	�� Element�� Display �Լ���
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
		//Card�� Title�� ������ ȭ�鿡 �׷����� �ʴ´�
		return;
	}
	else if(length > WAP_HS_SCRN_WIDE)//18���̻�
	{
		char temp2[WAP_HS_SCRN_WIDE+1];
		memset(temp2, '\0', WAP_HS_SCRN_WIDE+1);

		strncpy(temp2, g_pCard->title, WAP_HS_SCRN_WIDE);
		TruncateString((BYTE *)temp2, WAP_HS_SCRN_WIDE);//21��
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
	//���� ��尡 ���ų� Break�� ��츸, Align�� �����ȴ�
	int nAlign = WAP_TEXT_ALIGN_LEFT;

	if(pMe->x1 == 0
	&& (
			pMe->pNextNode == NULL//�� ��尡 ������ ���
		||	((WAP_Node *)pMe->pNextNode)->type == _ELEMENT_TYPE_BR//���� ��尡 BR
		||	(((WAP_Node *)pMe->pNextNode)->x1 == 0 
			&& ((WAP_Node *)pMe->pNextNode)->y1 == pMe->y2)))//���� ����
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

			//Focus�� ������ Cell ��ü�� �����Ѵ�
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
	// y���� ������� ���� ȭ�鿡 ������ �ʴ� �����̶�� ���̴�
	// �̴� �� ��尡 �� ���, ���� �߻��ϴ� �����̴�
	// �̸� ���� Text�� Ptr�� ������Ű��, �� �۾��� ����
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

			y += WAP_ASCII_FONT_HEIGHT;//y �� ����
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

			//Focus�� ������ Cell ��ü�� �����Ѵ�
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
		//Table�� �� Cell�� init�� �ش�
/*		WAP_Display_Rectangle(	pCurNode->x1, 
								pCurNode->y1 - g_nLCDOffset,
								pCurNode->x2,
								pCurNode->y2 - g_nLCDOffset,
								TRUE,//fill
								WAP_WHITE);	//color	
*/
		//Table�� �� Cell�� ������ �׷��ش�
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

		//Table�� �� Cell�� Box�� �׷��ش�
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
		sprintf(temp, "%s�Է�", SF_DOWN_RIGHT_PENCIL);//strcpy(temp, "\210\354�Է�");
	}
	else
	{
		sprintf(temp, "%s%s", SF_DOWN_RIGHT_PENCIL, pInput->title);//sprintf(temp, "\210\354%s", pInput->title);
	}
	
	//-----------------------------------------------------------
	//Input element�� visible size��ŭ�� display�Ѵ�
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
		strcpy(temp, "\242\321����");
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
	// ��
	//-----------------------------------------------------------
	memset(label, '\0', WAP_MAX_TEXT);
	if(pKey->label == NULL)
	{
		if(strcmp(pKey->eventType, "prev") == 0	|| strcmp(pKey->eventType, "PREV") == 0)
			strcpy(label, "����");
		else if(strcmp(pKey->eventType, "help") == 0 || strcmp(pKey->eventType, "HELP") == 0)
			strcpy(label, "����");
		/*else
			strcpy(label, "����");*/
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
			TruncateString((BYTE *)label, WAP_HS_SCRN_WIDE-2);//18��+icon
			NoWrap_num = 0;
		}
#else
		strcpy(label, pKey->label);
		TruncateString(label, WAP_HS_SCRN_WIDE-2);//18��+icon
#endif//#ifdef	WAP_NOWRAP
	}

	//-----------------------------------------------------------
	// ������
	//-----------------------------------------------------------
	memset(temp, '\0', 20);

#ifdef	WAP_NOWRAP
	if(length > (WAP_HS_SCRN_WIDE-2) && pMe == g_pCurFocusNode)
	{
		memset(label, '\0', WAP_MAX_TEXT);
		strncpy(label, tmpText[curNoWrap],WAP_HS_SCRN_WIDE-2);
	}
#endif//#ifdef	WAP_NOWRAP

	if(pKey->nSKT_Number == WAP_KEY_SK_PREV)//---����
	{
		//sprintf(temp, "\201\302%s", label);//(����)
		sprintf(temp, "\242\270%s", label);//����
	}
	else if(pKey->nSKT_Number == WAP_KEY_SK_NEXT)//---����
	{
		//sprintf(temp, "\201\303%s", label);//(���)
		sprintf(temp, "\242\272%s", label);//����
	}
	else if(pKey->nSKT_Number == WAP_KEY_SK_CALL)//---��ȭ
	{
		sprintf(temp, "%s%s", SF_CELLULAR, label);//(��ȭ)
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
			sprintf(temp, "\242\270%s", label);//����
		else if(strcmp(pKey->eventType, "help") == 0 || strcmp(pKey->eventType, "HELP") == 0)
			sprintf(temp, "%s%s", SF_INFORMATION, label);//����
		else
			sprintf(temp, "\242\272%s", label);//����
	}

#if !((defined WAP_B1) || (defined WAP_B2))
	if(nDrawMode == WAP_DRAW_REVERSE)
	{
		//������ ���, ���� ��ü�� ������Ű�� ���� �簢���� �׷��ش�
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



	//Formatting �� ������ �̹��� ��ǥ���� �������Ѵ�
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
	//Align�� ���� ����, x��ǥ���� ������ �ش�
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
	// 1. BMP �̹��� ����Ÿ�� �׷��ش�
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
	// 2. WBMP �̹��� ����Ÿ�� �׷��ش�
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
	// 3. SIS �̹��� ����Ÿ�� �׷��ش�
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
	// 4. �׿��� ���, alt text�� �簢���� �׷��ش�
	//---------------------------------------------------------------
	else 
	{
		int length = strlen(pImage->altText);

		if(nTableColumns > 0)//Table data
		{
			char temp[WAP_MAX_TEXT];

			memset(temp, '\0', WAP_MAX_TEXT);
			
            if(length <= 0)  memcpy(temp, "�׸�", strlen((char*)"�׸�"));    //if(pImage->altText == NULL)
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
			
			if(length > WAP_HS_SCRN_WIDE)//18���̻�
			{
				char temp2[WAP_HS_SCRN_WIDE+1];
				memset(temp2, '\0', WAP_HS_SCRN_WIDE+1);

				strncpy(temp2, pImage->altText, WAP_HS_SCRN_WIDE);
				TruncateString((BYTE *)temp2, WAP_HS_SCRN_WIDE);//18��
				strcpy(temp, temp2);
			}
			else
			{
                if(length <= 0)  strncpy(temp, "�׸�", strlen((char*)"�׸�"));    //if(pImage->altText == NULL)
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
				//sis image�� ���� ȭ�鿡 ���ϰ��
				if(!( pCurNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT || pCurNode->y2 <= g_nLCDOffset))
				{
					if(pImage->nTotalFrame > 1)	return TRUE;	//frame ������ 2�� �̻��ϰ�� timer start
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

				    //sis image�� ���� ȭ�鿡 ���ϰ��
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

		//sis image�� ���� ȭ�鿡 ���ϰ��
		if(! (   g_pCurFocusNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
			|| g_pCurFocusNode->y2 <= g_nLCDOffset)   )
		{
			//MSG_HIGH("-- WAP_Draw_Element_Key Call!! --",0,0,0);
			WAP_Draw_Element_Key(g_pCurFocusNode);
			
			//handset task�� display�� ��û�Ѵ�
			WAP_Send_LCDBuffer_ToHS();
		}
	}
}
#endif//#ifdef	WAP_NOWRAP

void TruncateString(BYTE *strText, int nMaxChar)
{
	//�� ���ξȿ� ���� ���
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

		memcpy(&(tmp_str[(nMaxChar-2)]), SF_DOTDOTDOT, 2); //���ڼ��� ������� "..." �߰�
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
    if(ExistCard) //card������
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
	// Focus ��尡 ���� ���
	if(g_pCurFocusNode == NULL 
	|| g_pCurFocusNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
	|| g_pCurFocusNode->y2 <= g_nLCDOffset)
		return -1;
	
	//------------------------------------------------
	//clear buffer
	//WAP_Clear_LCDDataBuffer();

	switch(g_pCurFocusNode->type)
	{

		case _ELEMENT_TYPE_INPUT://�Է�
		{
			//-------------------------------------------------------------
			// yjahn96 01.04.11 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

			g_pCurInput = (WAP_Content_Input *)g_pCurFocusNode->pContent; 
			g_nViewMode = WAP_VIEW_MODE_WML_INPUT;

			return _ELEMENT_TYPE_INPUT;
		}
		case _ELEMENT_TYPE_SELECT://����
		{	
			//-------------------------------------------------------------
			// yjahn96 01.04.11 - for Animation timer stop
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation

			g_pCurSelect = (WAP_Content_Select *)g_pCurFocusNode->pContent;

			//------------------------------------------------------------
			// �ɼ� ȭ��
			g_nViewMode = WAP_VIEW_MODE_WML_OPTION;

			WAP_Formatting_Options();
			WAP_Display_Options();

			return _ELEMENT_TYPE_SELECT;
		}
		case _ELEMENT_TYPE_KEY: //����
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
			// WML�������� Vendor�� üũ�Ͽ� download�� content�� ã���� ����� ����
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
		case _ELEMENT_TYPE_LINK_TEXT://����
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

		case _ELEMENT_TYPE_LINK_IMAGE://����
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
				case _ELEMENT_TYPE_LINK_TEXT://����
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

				case _ELEMENT_TYPE_LINK_IMAGE://����
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
		//������ �˸��� ����� ������ �ϵ��� �Ѵ�
//		load_incard  = TRUE;
//		loadingkind  = TRUE;

		SKY_keySelected(g_pKeyUp->viewId, g_pKeyUp->keyId);
		return TRUE;
	}
	return FALSE;
}

//=============================================================================
// Do element�� ���� ���� Ű ���
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
			//���� ȭ�鿡 ������ �ʴ� ���, ȭ���� �̵��Ѵ�
			if(pCurNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
			|| pCurNode->y2 <= g_nLCDOffset)
			{
				g_nLCDOffset = pCurNode->y2 - WAP_DOC_HEIGHT;
				if(g_nLCDOffset < 0) g_nLCDOffset = 0;
			}
			
			//----------------------------------------------------
			//�� ��带 Focus�� �ٲٰ�, Repaint�Ѵ�
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
			//������ �˸��� ����� ������ �ϵ��� �Ѵ�
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
			//�̹��� ������
			if(pImage->imageData != NULL)
			{
				*size = sizeof(pImage->imageData);
			}

			//-------------------------------------
			//������ ����
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
	
	//file�� ������ full���� check!!
    if(SKY_GetFileNumberInDataFile(SDF_IMAGE) >= SKY_MAX_NUM_FILES)
    {
		ERR("Filenum_Full",0,0,0);
		return FALSE;	//file�� ���� full
	}
	else
	{
		DWORD nFileSize = 0 , nDiskSpace;

        //��ũ ������
        nDiskSpace = SKY_GetFreeSpaceInDataFile();

		//File ������ = header ���� + Image ������
		GetCurrentImageInfo(&image_frame, &image_size, &image_type, &image_bitcount);

        if(image_type == WAP_IMAGE_TYPE_BMP || image_type == WAP_IMAGE_TYPE_WBMP)
        {
			if(image_bitcount == 1)	//B/W
			{
				// header + frame���� * (120/8*120)�̹��� ũ��
    			nFileSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_BW_IMAGE_SIZE);        
			}
			else if(image_bitcount == 2)	//4 gray
			{
				// header + frame���� * (120/4*120)�̹��� ũ��
    			nFileSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_4GRAY_IMAGE_SIZE);        
			}
			else if(image_bitcount == 8)	//256 color
			{
				// header + frame���� * (120*120)�̹��� ũ��
    			nFileSize = sizeof(BITMAP_HEADER) + (image_frame * WAP_256COLOR_IMAGE_SIZE);        
			}
        }
        else if(image_type == WAP_IMAGE_TYPE_SIS)
        {
			if(image_bitcount == 1)	// B/W
			{
				// header + frame���� * (120*120)�̹��� ũ��
    			nFileSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_BW_IMAGE_SIZE); 
			}
			else if(image_bitcount == 2)	// 4 gray
			{
				// header + frame���� * (120*120)�̹��� ũ��
    			nFileSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_4GRAY_IMAGE_SIZE); 
			}
			else if(image_bitcount == 8)	//256 color
			{
				// header + frame���� * (120*120)�̹��� ũ��
    			nFileSize = sizeof(BITMAP_HEADER)+sizeof(ANIMATION_HEADER_T) + (image_frame * WAP_256COLOR_IMAGE_SIZE); 
			}
        }

        // ������ ������ �ִ��� Ȯ��
    	if(nDiskSpace < nFileSize )  return FALSE;	//������ ������ ���� ���
		else                         return TRUE;	//������ ������ �ִ� ���
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
// EIF WAP ���� N Key Loading �� ���� 
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
			//���� ȭ�鿡 ������ �ʴ� ���, ȭ���� �̵��Ѵ�
			if(pCurNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
			|| pCurNode->y2 <= g_nLCDOffset)
			{
				g_nLCDOffset = pCurNode->y2 - WAP_DOC_HEIGHT;
				if(g_nLCDOffset < 0) g_nLCDOffset = 0;
			}
			
			//----------------------------------------------------
			//�� ��带 Focus�� �ٲٰ�, Repaint�Ѵ�
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
			//������ �˸��� ����� ������ �ϵ��� �Ѵ�
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
// EIF WAP ���� Link Text UOPEN �� ���� 
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
			//���� ȭ�鿡 ������ �ʴ� ���, ȭ���� �̵��Ѵ�
			if(pCurNode->y1 >= g_nLCDOffset+WAP_DOC_HEIGHT
			|| pCurNode->y2 <= g_nLCDOffset)
			{
				g_nLCDOffset = pCurNode->y2 - WAP_DOC_HEIGHT;
				if(g_nLCDOffset < 0) g_nLCDOffset = 0;
			}
			
			//----------------------------------------------------
			//�� ��带 Focus�� �ٲٰ�, Repaint�Ѵ�
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
			//������ �˸��� ����� ������ �ϵ��� �Ѵ�

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

