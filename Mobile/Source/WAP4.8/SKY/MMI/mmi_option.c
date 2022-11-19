//////////////////////////////////////////////////
// A7 Porting
//////////////////////////////////////////////////
#include <stdio.h>
//////////////////////////////////////////////////

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
// MMI �� Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"
#include "clnt.h"

//Option ȭ�� ����
int g_nCurStartOption;
int g_nCurFocusOption;

extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);


BOOL optionselect = FALSE;

#define MAX_WAP_LINES					7//3

//========================================================================================

void WAP_Building_Options(int type, void *pContent)
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
	pNewNode->font.height	= WAP_HAN_FONT_HEIGHT;
	pNewNode->font.width	= WAP_HAN_FONT_WIDTH;
	
	//------------------------------------------------------------------
	// �� ��带 ����Ʈ(pHeadContent )�� �������� �߰��Ѵ�
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
	// �� ��尡 ���� ������(Focus ����) �����,
	// �� ��带 ���� ����Ʈ(pHeadFocus)�� �������� �߰��Ѵ�
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
		//Option ���ÿ� Index�� ���δ� //0_based
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
			
		//�� ���ο��� �����Ѵ�
		pCurNode->x1 = 0;//�̹�������
		pCurNode->y1 = i*WAP_ASCII_FONT_HEIGHT;

		pCurNode->x2 = WAP_LCD_WIDTH-1;//=95
		pCurNode->y2 = (i+1)*WAP_ASCII_FONT_HEIGHT;

		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}// end of for

	//---------------------------------------------------
	//focus index ����1
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
	//focus index ����2
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
	//focus index ����3
	if(g_nCurFocusOption == -1)
		g_nCurFocusOption = 0;

	//---------------------------------------------------
	//start index ����
	if(g_nCurStartOption + (MAX_WAP_LINES-1) < g_nCurFocusOption)//�ٸ�ȭ��
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
	// �ɼ� ȭ���� Ÿ��Ʋ�� �׷��ش�
	//------------------------------------------------------------------
	WAP_Draw_Element_Title();
		
	//------------------------------------------------------------------
	// ������ ���� ���� ����
	//------------------------------------------------------------------
	pCurNode = g_pCurSelect->pHeadOption;
	for(i=0; i<g_nCurStartOption && pCurNode != NULL; i++)
	{
		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}

	//------------------------------------------------------------------
	// �� ȭ�鿡 �ɼǰ���(Option, OptionGroup)�� 3������ ���δ�
	// �� g_nCurStartOption���� �ִ� MAX_WAP_LINES(6)������ �׷��ش�
	//------------------------------------------------------------------
	for(i=0; pCurNode != NULL && i<MAX_WAP_LINES; i++)
    {
		//------------------------------------------------------------------
	   	// ������ ��� type�� ���� �� ��带 �׷��ִ� �Լ��� �θ���
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
				break;	//Unknown �� ó������ �ʴ´�

		};// end of switch

		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	
     }// end of for
	
    //clear
    WAP_Display_Rectangle(0, WAP_DOC_HEIGHT, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, TRUE, WAP_BASIC_BACK_COLOR);//WHITE);
    WAP_DisplayCML_Options();

	WAP_Send_LCDBuffer_ToHS();
}

void WAP_DisplayCML_Options()
{
	//------------------------------------------------------------------
	// ������ Focus ���� ����
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
	// ȭ��Ʒ� ����
	WAP_Display_Line(1, WAP_DOC_HEIGHT+3, WAP_LCD_WIDTH - 2, WAP_DOC_HEIGHT+3, BLACK);

	//===========================================================================
	// ���� Ű ((����) -> multiple select
	if(g_pCurSelect->multiSelect
		&& (g_nCurStartOption <= g_nCurFocusOption && g_nCurFocusOption <= g_nCurStartOption+(MAX_WAP_LINES-1)))//2))
	{
        WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LSELECT ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
	}
	//===========================================================================
	// �߰� Ű (okȮ��)
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);//ok
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CONFIRM , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//Ȯ��

	//===========================================================================
	// ������ Ű (���))
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_CANCEL ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);//��

    //===========================================================================
	// ��ũ�� Ű
	if( g_pCurSelect->pHeadFocusOption == NULL
	|| g_pCurSelect->pHeadFocusOption->pNextFocusNode == NULL)
	{
        WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
	}
	else
	{
		//���� ó��
		if(g_nCurStartOption == 0 && pCurFocusNode->pPrevFocusNode == NULL)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_DOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//���� ������
		else if(pCurFocusNode->pNextNode == NULL)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UP, WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//�߰�
		else
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
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
		strcpy(temp, "\242\321����");
	}
	else
	{
		char title[WAP_MAX_TEXT];
		memset(title, '\0', WAP_MAX_TEXT);
		strcpy(title, g_pCurSelect->title);
		TruncateString((BYTE *)title, WAP_HS_SCRN_WIDE-2);//18

		sprintf(temp, "\242\321%s", title);
	}

	WAP_Display_TextOut_Title(0, 0, temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
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
		sprintf(label, "�ɼ�%d", pOption->optionId);
	}
	else
	{
		strcpy(label, pOption->label);
		TruncateString((BYTE *)label, WAP_HS_SCRN_WIDE-2);//16��+icon
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

	WAP_Display_TextOut_Focus(pMe->x1 ,
						WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
}

void WAP_Draw_Element_OptionGroup(WAP_Node *pMe)
{
	int nDrawMode = WAP_DRAW_NORMAL;//normal
	
	char temp[WAP_MAX_TEXT];

	WAP_Content_OptionGroup *pOptionGroup = (WAP_Content_OptionGroup *) pMe->pContent;

	memset(temp, '\0', WAP_MAX_TEXT);
	if(pOptionGroup->label == NULL)
	{
		strcpy(temp, "[�׷�]");
	}
	else
	{
		char trunc[WAP_MAX_TEXT];
		memset(trunc, '\0', WAP_MAX_TEXT);
		strcpy(trunc, pOptionGroup->label);
		TruncateString((BYTE *)trunc, WAP_HS_SCRN_WIDE-2);//19-1);//19��+��ȣ

		sprintf(temp, "[%s]", trunc);
	}

    WAP_Display_TextOut(pMe->x1,
						WAP_ASCII_FONT_HEIGHT + pMe->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT),//title + y - offset
						temp,
						WAP_FONT_NORMAL, nDrawMode, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
}

void WAP_Select_Options()//����
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
				// ���� ��Ȳ�� ������ �ش�
				pContent->isSelectedTemp = ! pContent->isSelectedTemp;
			}
			else 
			{
				// Select�� single ������ ���
				if(!g_pCurSelect->multiSelect)
				{
					// �ٸ� ��� Option�� ������ �����Ѵ�
					pContent->isSelectedTemp = FALSE;
				}
			}
		}
		pCurNode = (WAP_Node *)pCurNode->pNextNode;
	}

	//ȭ���� �ٽ� �׷��ش�
	//WAP_Display_Options();
}

void WAP_DrawPartialUpdate_Options(WAP_Node *pCurNode)
{
	switch(pCurNode->type)
	{
	    case _ELEMENT_TYPE_OPTION:
			WAP_Draw_Element_Option(pCurNode);
			break;

		case _ELEMENT_TYPE_OPTIONGROUP:
			WAP_Draw_Element_OptionGroup(pCurNode);
			break;

		default :
			break;	//Unknown �� ó������ �ʴ´�

	};// end of switch
}

void WAP_Display_Options_Partial(BOOL bDown)
{
	WAP_Node *pCurFocusNode;
	WAP_Content_Option *pContent;

    int x1, y1, x2, y2;
	WAP_Node *pFirstFocusNode, *pSecondFocusNode;    

	//------------------------------------------------------------------
	// ������ Focus ���� ����
	//------------------------------------------------------------------
	pCurFocusNode = g_pCurSelect->pHeadFocusOption;
	while(pCurFocusNode != NULL && pCurFocusNode->type == _ELEMENT_TYPE_OPTION)
	{
		pContent = (WAP_Content_Option *)pCurFocusNode->pContent;
		if(pContent->indexOption == g_nCurFocusOption)
			break;
		pCurFocusNode = (WAP_Node *)pCurFocusNode->pNextFocusNode;
	}

    if(bDown)// ���� node�� ���� ��常 update
    {
        pFirstFocusNode = pCurFocusNode->pPrevFocusNode;
        pSecondFocusNode = pCurFocusNode;
    }
    else    //���� node�� ���� ��常 update    
    {
        pFirstFocusNode = pCurFocusNode;
        pSecondFocusNode = pCurFocusNode->pNextFocusNode;
    }

    WAP_DrawPartialUpdate_Options(pFirstFocusNode);
    WAP_DrawPartialUpdate_Options(pSecondFocusNode);

//	if(! (   g_pCurFocusNode->y1 >= g_nLCDOffset + WAP_DOC_HEIGHT
//		|| g_pCurFocusNode->y2 <= g_nLCDOffset)   )
	{
        x1 = 0;//pFirstFocusNode->x1;
        y1 = (WAP_ASCII_FONT_HEIGHT + pFirstFocusNode->y1 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT)) + WAP_HEAD_HEIGHT; //title + y - offset + annunciator
        x2 = WAP_LCD_TOTAL_ADDRESS_X-1;
        y2 = (WAP_ASCII_FONT_HEIGHT + pSecondFocusNode->y2 - (g_nCurStartOption * WAP_ASCII_FONT_HEIGHT)) + WAP_HEAD_HEIGHT;
		//handset task�� display�� ��û�Ѵ�
		SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), x1, y1, x2, y2);//WAP_Send_LCDBuffer_ToHS();
	}

    //-------------------------
    //command line
    WAP_Display_Rectangle(0, WAP_DOC_HEIGHT, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, TRUE, WAP_BASIC_BACK_COLOR);//WHITE);//clear
    WAP_DisplayCML_Options();
    SKY_RefreshDisplayRect((BYTE *)GetScreenBuffer(), 0, WAP_HEAD_HEIGHT + WAP_SOFTKEY_Y_POS, WAP_LCD_TOTAL_ADDRESS_X-1, WAP_LCD_TOTAL_ADDRESS_Y-1);
}

void WAP_Scrolling_Options(BOOL bDown)
{
	WAP_Node *pCurFocusNode;
	WAP_Content_Option *pContent;
	
	BOOL bChangedFocus = FALSE;
    BOOL bOnlyFocusMove = FALSE;

	//------------------------------------------------------------------
	// ���� card�� ���� ���, ����
	//------------------------------------------------------------------
	if(g_pCard == NULL)	//yjahn96 01.10.24
		return;
		
	//------------------------------------------------------------------
	// ���� Select ���� Focus ��尡 �ϳ��� ���� ���, focus �̵� ����
	//------------------------------------------------------------------
    if(g_pCurSelect->pHeadFocusOption == NULL) return;

	//------------------------------------------------------------------
	// ������ Focus ���� ����
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
	// Scrolling ����
	//------------------------------------------------------------------
	if(pCurFocusNode == NULL) return;

	//==================================================================
	// Scrolling ����
	//------------------------------------------------------------------
	if(bDown)
	{
		int nNextFocusIndex = ((WAP_Content_Option *)pCurFocusNode->pNextFocusNode->pContent)->indexOption;

		//===============================================
		//���� ������
		if(pCurFocusNode->pNextNode == NULL) return;
		
		//===============================================
		//���� ȭ�� -> Focus �̵�
		if( g_nCurStartOption <= nNextFocusIndex
		 && nNextFocusIndex <= g_nCurStartOption+(MAX_WAP_LINES-1))
		{
			bChangedFocus = TRUE;
			g_nCurFocusOption = nNextFocusIndex;

            bOnlyFocusMove = TRUE;
		}
		//===============================================
		//���� ȭ�� -> Window �̵�
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
		//���� ó��
		if(g_nCurStartOption == 0 
		&& pCurFocusNode->pPrevFocusNode == NULL) return;

		//===============================================
		//���� ȭ�� -> Focus �̵�
		if( g_nCurStartOption <= nPrevFocusIndex
		 && nPrevFocusIndex <= g_nCurStartOption+(MAX_WAP_LINES-1))
		{
			bChangedFocus = TRUE;
			g_nCurFocusOption = nPrevFocusIndex;

            bOnlyFocusMove = TRUE;
		}
		//===============================================
		//���� ȭ�� -> Window �̵�
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
	// Single Select�� ���, Scroll�� ���ÿ� �����Ѵ�
	if(!g_pCurSelect->multiSelect && bChangedFocus)//and display
    {
        WAP_Select_Options();

        if(bOnlyFocusMove == TRUE)
            WAP_Display_Options_Partial(bDown);
        else
            WAP_Display_Options();
    }
    else
    {
        if(bOnlyFocusMove == TRUE)
            WAP_Display_Options_Partial(bDown);
        else
            WAP_Display_Options();
    }
}

void WAP_Handle_LeftKey_Options()
{
	//------------------------------------------------------------------
	// ���� card�� ���� ���, ����
	//------------------------------------------------------------------
	if(g_pCard == NULL)	//yjahn96 01.10.24
		return;

	//����
	if(g_pCurSelect->multiSelect)
	{
		WAP_Select_Options();
	    //ȭ���� �ٽ� �׷��ش�
	    WAP_Display_Options();
	}
}

void WAP_Handle_RightKey_Options()
{
	//���
	g_nViewMode = WAP_VIEW_MODE_WML;
	WAP_Display();
    MSG_HIGH("",0,0,0);
}

void WAP_Handle_CenterKey_Options()//Ȯ��
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

	//------------------------------------------------------------------
	// ���� card�� ���� ���, ����
	//------------------------------------------------------------------
	if(g_pCard == NULL)	//yjahn96 01.10.24
		return;
	
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
				//���ð��� ��ȭ�� �ִ� ���
				if(pContent->isSelected != pContent->isSelectedTemp)
				{
					//�ӽð��� �������� �����Ѵ�
					pContent->isSelected = pContent->isSelectedTemp;
                    selectcase = 2;
					saveoption[length++] = pContent->optionId;

					// AUR ������ ���� �˸���(select/deselect)
					//SKY_optionSelected(pContent->viewId, pContent->optionId);
				}
			}
			//====================================================
			// Single Selection
			//====================================================
			else
			{
				//�ӽð��� �������� �����Ѵ�
				pContent->isSelected = pContent->isSelectedTemp;

				// AUR ������ ���� �˸���
				if(pContent->isSelected)
				{
                    selectcase = 1;
					saveoption[0] = pContent->optionId;

					//SKY_optionSelected(pContent->viewId, pContent->optionId);
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
	// WML ȭ��(ȭ�� ��ȯ)
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