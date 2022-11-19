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

void WAP_Display_Dialog(void);
void WAP_DisplaySoftkey_Dialog(void);
void WAP_DisplayCML_Dialog(void);

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

#if !((defined WAP_B1) || (defined WAP_B2))
		//for decoration
		WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH, WAP_ASCII_FONT_HEIGHT - 1, TRUE, WAP_BLACK);
		WAP_Display_TextOut(0, 1, title,
							WAP_FONT_MEDIUM, WAP_DRAW_REVERSE, FALSE, WAP_TEXT_ALIGN_CENTER,
							1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
        WAP_Display_TextOut_Title(0, 1, title,
							//WAP_FONT_MEDIUM, WAP_DRAW_REVERSE, FALSE, WAP_TEXT_ALIGN_CENTER,
							WAP_FONT_NORMAL, WAP_DRAW_REVERSE, FALSE, WAP_TEXT_ALIGN_CENTER,
							1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
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
#if (defined WAP_B1) || (defined WAP_B2)
	WAP_DisplayCML_Dialog();
#else   //#if (defined WAP_B1) || (defined WAP_B2)
    WAP_DisplaySoftkey_Dialog();
#endif  //#if (defined WAP_B1) || (defined WAP_B2)

	//------------------------------------------------------------------
	//for decoration
	// 1. background (WAP_WHITEGRAY)
	//WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, TRUE, WAP_WHITEGRAY);
	// 2. outline frame (WAP_BLACK)
#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, FALSE, WAP_WHITEGRAY);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_Display_Rectangle(0, 0, WAP_LCD_WIDTH, WAP_LCD_HEIGHT, FALSE, WAP_LIGHTGRAY);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
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
	WAP_Display_Line(2, WAP_DOC_HEIGHT, WAP_LCD_WIDTH - 3, WAP_DOC_HEIGHT, BLACK);

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

//for Softkeys -- yjahn96 01.01.04
extern WORD WAP_OutWordCharLCDBufferHan(BYTE *pFont, int nPosX, int nPosY, int nFontWidth, int nFontHeight, int nDrawMode);
extern BYTE *Widening(BYTE *pFont, BYTE nBytePerChar);

void WAP_DisplaySoftkey_Dialog(void)
{
	char temp[20];

	extern /*ROM*/ BYTE* /*EQS_API*/ GetBitmapFontBufferByCode(WORD wCode);	//yjahn96 00.12.28

	//clear
	strcpy(temp, "                    ");
	WAP_Display_TextOut(0, WAP_SOFTKEY_Y_POS, temp,
						WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
						1, 0);
	{
	BYTE *pFont;
#if !((defined WAP_B1) || (defined WAP_B2))
	BYTE *pWordFont;
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))

	WORD pSoftkey_Blank[1]	= {0x81bf};
	pFont = GetBitmapFontBufferByCode(pSoftkey_Blank[0]);
#if !((defined WAP_B1) || (defined WAP_B2))
	pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
    WAP_OutWordCharLCDBufferHan(pWordFont, 96*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pWordFont, 108*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
	WAP_OutWordCharLCDBufferHan(pFont, 96, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
	WAP_OutWordCharLCDBufferHan(pFont, 108, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
	//===========================================================================
	// �߰� Ű
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//��
	{
		//ok�� : {0x88bf, 0x85be, 0x85bf}
        strcpy(temp, "\210\277\205\276\205\277");
//To be implemented swjung
#ifdef BLOCK
        if(ui.earpieceup)
			strcpy(temp, "\210\277\205\276\205\277");
		else//for A7M
#endif //#ifdef BLOCK

#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(48*2, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(48, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
	else if(g_nDlgMode == WAP_DLG_MODE_ALERT)//Ȯ��
	{
		//okȮ�� : {0x88bf, 0x86be, 0x86bf}
        strcpy(temp, "\210\277\206\276\206\277");
//To be implemented swjung
#ifdef BLOCK
        if(ui.earpieceup)
			strcpy(temp, "\210\277\206\276\206\277");
		else//for A7M
#endif //#ifdef BLOCK
#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(48*2, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(48, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}
	//===========================================================================
	// ������ Ű
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//�ƴϿ�
	{
		//�ƴϿ�)  : {0x85bc, 0x85bd, 6}
		strcpy(temp, "\205\274\205\275\6");
#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(86*2, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_Display_TextOut(86, WAP_SOFTKEY_Y_POS, temp,
							WAP_FONT_NORMAL, WAP_DRAW_NORMAL, FALSE, WAP_TEXT_ALIGN_LEFT,
							1, 0);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
	}

	//===========================================================================
	// ��ũ�� Ű
	if(nDlgOffset == 0 && ptEnd.y <= WAP_DOC_HEIGHT)
	{
	}
	else
	{
		BYTE *pFont;
#if !((defined WAP_B1) || (defined WAP_B2))
		BYTE *pWordFont;
#endif //WAP_B1
		WORD pSoftkey_Move[3]	= {0x86b1, 0x88ed, 0x88ee};// �̵�
#ifdef WAPCORE_FOR_WARNING
		MSG_LOW(" Dummy for WARNING in mmi_dialog.c",0,0,0);
#endif //WAPCORE_FOR_WARNING
		//���� ó�� ������
		if(nDlgOffset == 0)
		{
            pFont = GetBitmapFontBufferByCode(pSoftkey_Move[1]);//down img
//To be implemented swjung
#ifdef BLOCK
			if(ui.earpieceup)
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[1]);//down img

			else//for A7M
#endif //#ifdef BLOCK
		}
		//���� ������ ������
		else if(ptEnd.y <= nDlgOffset + WAP_DOC_HEIGHT)
		{
            pFont = GetBitmapFontBufferByCode(pSoftkey_Move[2]);//up img
//To be implemented
#ifdef BLOCK
			if(ui.earpieceup)
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[2]);//up img
			else//for A7M
#endif //#ifdef BLOCK
		}
		//�߰� ������
		else
		{
            pFont = GetBitmapFontBufferByCode(pSoftkey_Move[0]);
//To be implemented
#ifdef BLOCK
			if(ui.earpieceup)
				pFont = GetBitmapFontBufferByCode(pSoftkey_MoveOK[0]);
			else//for A7M
#endif //#ifdef BLOCK
		}
	
#if !((defined WAP_B1) || (defined WAP_B2))
		pWordFont = Widening(pFont, WAP_HAN_FONT_BUFFER_SIZE_PER_CHAR);
		WAP_OutWordCharLCDBufferHan(pWordFont, 36*2, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#else   //#if !((defined WAP_B1) || (defined WAP_B2))
		WAP_OutWordCharLCDBufferHan(pFont, 36, WAP_SOFTKEY_Y_POS, WAP_HAN_FONT_WIDTH, WAP_HAN_FONT_HEIGHT, WAP_DRAW_NORMAL);
#endif  //#if !((defined WAP_B1) || (defined WAP_B2))
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