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
// MMI 의 Data structure
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
	// 타이틀 쓰기
	if(nDlgOffset == 0)
	{
		char title[20];

		if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)
			strcpy(title, "확인하세요");
		else if(g_nDlgMode == WAP_DLG_MODE_ALERT)
			strcpy(title, "알립니다");
		else
			strcpy(title, "대화상자");

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
	// 메세지 쓰기
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
	//소프트 키를 그려준다
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
	//handset task에 display를 요청한다
	WAP_Send_LCDBuffer_ToHS();
}

//-----------------------------------------------------
// yjahn96 01.03.05
void WAP_DisplayCML_Dialog(void)
{
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
	// 중간 키
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//예
	{
        l_midKeyExist = TRUE;
        WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS); //ok
        WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_YES , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//예
	}
	else if(g_nDlgMode == WAP_DLG_MODE_ALERT)//확인
	{
        l_midKeyExist = TRUE;
        WAP_Display_Image_CML(WAP_OK_CML, WAP_CML_OK, WAP_OK_CML_X, WAP_SOFTKEY_Y_POS); //ok
        WAP_Display_Image_CML(WAP_CENTER_CML, WAP_CML_CONFIRM , WAP_CENTER_CML_X, WAP_SOFTKEY_Y_POS);//확인
    }
	//===========================================================================
	// 오른쪽 키
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//아니요
	{
        WAP_Display_Image_CML(WAP_RIGHT_CML, WAP_CML_NO ,WAP_RIGHT_CML_X, WAP_SOFTKEY_Y_POS);
	}

	//===========================================================================
	// 스크롤 키
	if(nDlgOffset == 0 && ptEnd.y <= WAP_DOC_HEIGHT)
	{
        if(l_midKeyExist)   //중간키는 있고, 스크롤은 아닌 경우
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UPDOWNEMPTY ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
	}
	else
	{
		//01.04.04 ytchoi to solve warning
		//WORD pSoftkey_Move[3]	= {0x86b1, 0x88ed, 0x88ee};// 이동

        //가장 처음 페이지
		if(nDlgOffset == 0)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_DOWN ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//가장 마지막 페이지
		else if(ptEnd.y <= nDlgOffset + WAP_DOC_HEIGHT)
		{
            WAP_Display_Image_CML(WAP_UPDOWN_CML, WAP_CML_UP ,WAP_UPDOWN_CML_X, WAP_SOFTKEY_Y_POS);
		}
		//중간 페이지
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
	// 중간 키
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//예
	{
		//ok예 : {0x88bf, 0x85be, 0x85bf}
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
	else if(g_nDlgMode == WAP_DLG_MODE_ALERT)//확인
	{
		//ok확인 : {0x88bf, 0x86be, 0x86bf}
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
	// 오른쪽 키
	if(g_nDlgMode == WAP_DLG_MODE_CONFIRM)//아니요
	{
		//아니요)  : {0x85bc, 0x85bd, 6}
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
	// 스크롤 키
	if(nDlgOffset == 0 && ptEnd.y <= WAP_DOC_HEIGHT)
	{
	}
	else
	{
		BYTE *pFont;
#if !((defined WAP_B1) || (defined WAP_B2))
		BYTE *pWordFont;
#endif //WAP_B1
		WORD pSoftkey_Move[3]	= {0x86b1, 0x88ed, 0x88ee};// 이동
#ifdef WAPCORE_FOR_WARNING
		MSG_LOW(" Dummy for WARNING in mmi_dialog.c",0,0,0);
#endif //WAPCORE_FOR_WARNING
		//가장 처음 페이지
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
		//가장 마지막 페이지
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
		//중간 페이지
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