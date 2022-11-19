#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
#endif //#ifdef WAP_NEO

#include <string.h>

#include "aapimmi.h"
#include "capimmi.h"
#include "aapiclnt.h"

//General Display Library
#include "GenDsp.h"

//-----------------------------------------------------------------------------------
//
// MMI �� Data structure
//
//-----------------------------------------------------------------------------------
#include "tapicmmn.h"
#include "mmi_data.h"
#include "mmi_func.h"

extern int g_nDlgId;
extern char *g_pDlgMessage;

static point ptEnd;
int nDlgOffset = 0;
int isDialogsRunning = 0;

extern void WAP_Repaint(void);

void WAP_Check_Dialog(void)
{
	if(!isDialogsRunning)
		isDialogsRunning = TRUE;
}

void WAP_Build_Dialog(void)
{
	//------------------------------------------------------------------
	//Init
	nDlgOffset = 0;

	//------------------------------------------------------------------
	//Formatting
	ptEnd = Calculate_Node_EndPoint(g_pDlgMessage, 0, 13, WAP_FONT_NORMAL, 1);

	//------------------------------------------------------------------
	//Display
	WAP_Display_Dialog();
	WAP_Check_Dialog();
}

void WAP_Display_Dialog(void)
{
	//------------------------------------------------
	//clear buffer
	WAP_Clear_LCDDataBuffer();

	//-----------------------------------------------------------
	// Ÿ��Ʋ ����
	if(nDlgOffset == 0)
	{
		char title[20];

		if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)
			strcpy(title, "Ȯ���ϼ���");
		else if(g_nDlgMode == WAP_DLG_MODE_ALERT)
			strcpy(title, "�˸��ϴ�");
		else
			strcpy(title, "��ȭ����");

        WAP_Display_TextOut_Title(0, 1, title,
							//WAP_FONT_MEDIUM, WAP_DRAW_REVERSE, FALSE, WAP_TEXT_ALIGN_CENTER,
							WAP_FONT_NORMAL, WAP_DRAW_REVERSE, FALSE, WAP_TEXT_ALIGN_CENTER,
							1, 0);
	}

	//------------------------------------------------------------------
	// �޼��� ����
	{
		char* temp; 
		temp = (char *)wip_malloc(strlen(g_pDlgMessage)+1);
		memset(temp, '\0', sizeof(temp));
		strcpy(temp, g_pDlgMessage);

		WAP_Display_TextOut(0, WAP_ASCII_FONT_HEIGHT-nDlgOffset, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_CENTER,
							1, 0);

		wip_free((void *)temp);
	}
	
	//------------------------------------------------------------------
	//����Ʈ Ű�� �׷��ش�
	WAP_DisplayCML_Dialog();

	//------------------------------------------------------------------
	//for decoration
	// 1. background (WAP_WHITEGRAY)
	//WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, TRUE, WAP_WHITEGRAY);
	// 2. outline frame (WAP_BLACK)
	WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, FALSE, LIGHTGRAY);

    //------------------------------------------------------------------
	//handset task�� display�� ��û�Ѵ�
	WAP_Send_LCDBuffer_ToHS();
}

//-----------------------------------------------------
// yjahn96 01.03.05
void WAP_DisplayCML_Dialog(void)
{
    BOOL l_midKeyExist = FALSE;

	//��ü Clear
    WAP_Display_Image_CML(WAP_LEFT_CML, WAP_CML_LEFTEMPTY ,WAP_LEFT_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);//WAP_Display_Image_CML(WAP_OK_CML/*WAP_UPDOWN_CML*/, WAP_CML_OKEMPTY/*WAP_CML_UPDOWNEMPTY*/ ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OKEMPTY, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CETEREMPTY , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);
    WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_RIGHTEMPTY ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);

	//===========================================================================
	// ȭ��Ʒ� ����
	WAP_Display_Line(2, WAP_DOC_HEIGHT , WAP_LCD_WIDTH - 3, WAP_DOC_HEIGHT, BLACK);

	//===========================================================================
	// �߰� Ű
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//��
	{
        l_midKeyExist = TRUE;
        WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS); //ok
        WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_YES , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//��
	}
	else if(g_nDlgMode == WAP_DLG_MODE_ALERT)//Ȯ��
	{
        l_midKeyExist = TRUE;
        WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS); //ok
        WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CONFIRM , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//Ȯ��
    }
	//===========================================================================
	// ������ Ű
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//�ƴϿ�
	{
        WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_NO ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);
	}

	//===========================================================================
	// ��ũ�� Ű
	if(nDlgOffset == 0 && ptEnd.y <= WAP_DOC_HEIGHT)
	{
        if(l_midKeyExist)   //�߰�Ű�� �ְ�, ��ũ���� �ƴ� ���
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
	}
	else
	{
		//01.04.04 ytchoi to solve warning
		//WORD pSoftkey_Move[3]	= {0x86b1, 0x88ed, 0x88ee};// �̵�

        //���� ó�� ������
		if(nDlgOffset == 0)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_DOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//���� ������ ������
		else if(ptEnd.y <= nDlgOffset + WAP_DOC_HEIGHT)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UP ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//�߰� ������
		else
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
	}
}



/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
/*																						*/
/*								Key Handlers											*/
/*																						*/
/*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*/
void WAP_Scrolling_Message(BOOL bDown)
{
	if(bDown)
	{
		if(nDlgOffset + WAP_DOC_HEIGHT < ptEnd.y)
		{
			nDlgOffset += WAP_ASCII_FONT_HEIGHT/*WAP_DOC_HEIGHT*/;
			WAP_Display_Dialog();
		}
	}
	else
	{
		if(nDlgOffset > 0)
		{
			nDlgOffset -= WAP_ASCII_FONT_HEIGHT/*WAP_DOC_HEIGHT*/;
			WAP_Display_Dialog();
		}
	}
}

void WAP_InitDlgData(void)
{
	//------------------------------------------------------
	// init
	if(g_pDlgMessage) wip_free((void *)g_pDlgMessage);
	g_pDlgMessage = NULL;
	g_nDlgMode = WAP_DLG_MODE_NONE;
}

void WAP_Alert_Dialog(void)
{
	if(g_nDlgMode != WAP_DLG_MODE_ALERT) return;

	WAP_InitDlgData();
	WAP_Repaint();

	//------------------------------------------------------
	// response
	SKY_alertDialogResponse(g_pCard->viewId, g_nDlgId);
}

void WAP_OK_Dialog(void)
{
	if(g_nDlgMode != WAP_DLG_MODE_CONFIRM) return;

	WAP_InitDlgData();
	WAP_Repaint();

	//------------------------------------------------------
	// response
	SKY_confirmDialogResponse(g_pCard->viewId, g_nDlgId, TRUE);
}	

void WAP_Cancel_Dialog(void)
{
	if(g_nDlgMode != WAP_DLG_MODE_CONFIRM) return;

	WAP_InitDlgData();
	WAP_Repaint();

	//------------------------------------------------------
	// response
	SKY_confirmDialogResponse(g_pCard->viewId, g_nDlgId, FALSE);
}

void WAP_Close_Dialog(void)
{
	WAP_InitDlgData();

	//------------------------------------------------------
	// response
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)
	{
		SKY_confirmDialogResponse(g_pCard->viewId, g_nDlgId, FALSE);
	}
	else if(g_nDlgMode == WAP_DLG_MODE_ALERT)
	{
		SKY_alertDialogResponse(g_pCard->viewId, g_nDlgId);
	}
    MSG_HIGH("",0,0,0);
}