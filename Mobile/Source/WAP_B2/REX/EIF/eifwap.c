//#include "customer.h"
#if !(defined WAP_B1 || defined WAP_B2)
#include "custB0.h"
#endif //not WAP_B1
//#ifdef FEATURE_SKT_EIF//#ifdef EIF_WAP
/************************************************************************************/
/*																					*/
/*							External Interface 1.1									*/
/*							for WAP Functions										*/
/*																					*/
/************************************************************************************/

#include <stdlib.h>
#include "eifwap.h"

#include "ska_eif.h"

#if !(defined WAP_B1 || defined WAP_B2) //yjahn96 01.02.24
#include "ui.h"
#include "..\MMI_A10\uiCommon.h" // for user defined types
#include "uistate.h"
#endif  //#ifndef WAP_B1
#include "psmisc.h"							// for put16, get16 function in uping_command
#include "waptask.h"


char resultBuf[4096];						// Global variable for EIF



void NKey_uopen_command(int whatKey);
void LinkText_open_command(int whatNumber);

//char url_from_keys[192];					// for geturl_command

BOOL g_EIF_WAP_UOPEN = FALSE;				// for uopen_command

BOOL g_EIF_WAP_NEWPAGE = FALSE;				// for newpage_command

BOOL g_EIF_WAP_GETURL = FALSE;				// for geturl_command

WAP_Parsing_WML_NODE *Wml_Card = NULL;		// for uopen & newpage command

//WAP_UPING_ICMP_TIME *TimeStore = NULL;		// for uping command 메시지 전송할때의 Time을 저장한다.

char EIF_url[192];

extern void at_send_extended_text(char*);
extern BOOL SendEIFCommand(SKY_EVENT_E Event);

extern void clk_uptime_ms(qword);
void resultTokening_output(char*, int);

int NKey_keyID[10];							// zeus(yjchoi) 01.01.29

BOOL g_NKey_UOPEN = FALSE;
BOOL g_Link_Text_UOPEN = FALSE;				

extern BOOL Wtlshello;

/************************************************************************************/
/*																					*/
/*						EIF에서 WAP 단말기능을 위한 각종 함수들						*/
/*																					*/
/************************************************************************************/

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void WAP_Parsed_MMI_output(BOOL u_or_n)
{
	int resultLen = 0;
	int nKeyCount = 0;

	WAP_Parsing_WML_NODE *OutPutPtr = Wml_Card;
	
	memset(resultBuf, 0x00, sizeof(resultBuf));

	for (nKeyCount = 0 ; nKeyCount < 10 ; nKeyCount++)			// zeus(yjchoi) 01.01.29    for initialize
		NKey_keyID[nKeyCount] = 0;

	nKeyCount = 0;												// zeus(yjchoi) 01.01.29    for nKey counting


	if (u_or_n)			// when uopen_command is called u_or_n's value is TRUE
	{
		char * result = "UOPEN:1";
		strncpy(resultBuf, result, sizeof(resultBuf));		
	}


	while(OutPutPtr != NULL)
	{

		switch (OutPutPtr->ContentType)
		{
			case _ELEMENT_TYPE_KEY:
			{
				WAP_Parsing_MENU *ParsedKey = (WAP_Parsing_MENU*)(OutPutPtr->pContent);

				if(strlen(resultBuf) == 0)
				{
                    //----------------------------------------
                    // B1에는 이전, 다음 키가 mapping되어 있지 않으므로.. -- yjahn96 01.05.28
					/*if (ParsedKey->nKey_number == WAP_KEY_SK_PREV)						//---이전
						sprintf(resultBuf, "KEY(MENU):R,%s", ParsedKey->title);
					else if (ParsedKey->nKey_number == WAP_KEY_SK_NEXT)					//---다음
						sprintf(resultBuf, "KEY(MENU):P,%s", ParsedKey->title);
					else*/ if(0 < ParsedKey->nKey_number && ParsedKey->nKey_number < 10)	// 0 ~ 9
						sprintf(resultBuf, "KEY(MENU):%d,%s", ParsedKey->nKey_number, ParsedKey->title);
					else
					{
						g_NKey_UOPEN = TRUE;
						NKey_keyID[nKeyCount] = ParsedKey->keycode;						// zeus(yjchoi) 01.01.29   n Key ID store
						nKeyCount++;
						sprintf(resultBuf, "KEY(MENU):N%d,%s", nKeyCount, ParsedKey->title);
					}
				}
				else
				{
                    //----------------------------------------
                    // B1에는 이전, 다음 키가 mapping되어 있지 않으므로.. -- yjahn96 01.05.28
					/*if (ParsedKey->nKey_number == WAP_KEY_SK_PREV)						//---이전
						sprintf(resultBuf, "%s\nKEY(MENU):R,%s", resultBuf, ParsedKey->title);
					else if (ParsedKey->nKey_number == WAP_KEY_SK_NEXT)					//---다음
						sprintf(resultBuf, "%s\nKEY(MENU):P,%s", resultBuf, ParsedKey->title);
					else*/ if(0 < ParsedKey->nKey_number && ParsedKey->nKey_number < 10)  // 0 ~ 9
						sprintf(resultBuf, "%s\nKEY(MENU):%d,%s", resultBuf, ParsedKey->nKey_number, ParsedKey->title);
					else
					{
						g_NKey_UOPEN = TRUE;
						NKey_keyID[nKeyCount] = ParsedKey->keycode;						// zeus(yjchoi) 01.01.29	n Key ID store
						nKeyCount++;
						sprintf(resultBuf, "%s\nKEY(MENU):N%d,%s", resultBuf, nKeyCount, ParsedKey->title);
					}
				}
				
				wip_free((WAP_Parsing_MENU*)ParsedKey);
				break;
			}

			case _ELEMENT_TYPE_INPUT:
			{
				WAP_Parsing_INPUT *ParsedInput = (WAP_Parsing_INPUT*)(OutPutPtr->pContent);

				if(strlen(resultBuf) == 0)
					sprintf(resultBuf, "INPUT:[%s]", ParsedInput->title);
				else
					sprintf(resultBuf, "%s\nINPUT:[%s]", resultBuf, ParsedInput->title);

				wip_free((WAP_Parsing_INPUT*)ParsedInput);
				break;
			}

			case _ELEMENT_TYPE_TEXT:
			{
				WAP_Parsing_TEXT *ParsedText = (WAP_Parsing_TEXT*)(OutPutPtr->pContent);

				if(strlen(resultBuf) == 0)
					sprintf(resultBuf, "TEXT:%s", ParsedText->text);
				else
					sprintf(resultBuf, "%s\nTEXT:%s", resultBuf, ParsedText->text);

				wip_free((WAP_Parsing_TEXT*)ParsedText);
				break;
			}

			case _ELEMENT_TYPE_LINK_TEXT:
			{
				WAP_Parsing_LINK_TEXT *ParsedLinkText = (WAP_Parsing_LINK_TEXT*)(OutPutPtr->pContent);

				g_Link_Text_UOPEN = TRUE;

				if(strlen(resultBuf) == 0)
					sprintf(resultBuf, "LINKTEXT_%d:[%s]", ParsedLinkText->textID, ParsedLinkText->text);
				else
					sprintf(resultBuf, "%s\nLINKTEXT_%d:[%s]", resultBuf, ParsedLinkText->textID, ParsedLinkText->text);

				wip_free((WAP_Parsing_LINK_TEXT*)ParsedLinkText);
				break;
			}

			case _ELEMENT_TYPE_IMAGE:
			{
				WAP_Parsing_IMAGE *ParsedImage = (WAP_Parsing_IMAGE*)(OutPutPtr->pContent);

				if(strlen(resultBuf) == 0)
				{
					if (ParsedImage->imagetype == WAP_IMAGE_TYPE_BMP)
						strcpy(resultBuf, "IMAGE:BMP");
					else if (ParsedImage->imagetype == WAP_IMAGE_TYPE_WBMP)
						strcpy(resultBuf, "IMAGE:WBMP");
					else if (ParsedImage->imagetype == WAP_IMAGE_TYPE_SIS)
						strcpy(resultBuf, "IMAGE:SIS");
					else
						strcpy(resultBuf, "IMAGE:UNKNOWN");
				}
				else
				{
					if (ParsedImage->imagetype == WAP_IMAGE_TYPE_BMP)
						sprintf(resultBuf, "%s\nIMAGE:BMP", resultBuf);
					else if (ParsedImage->imagetype == WAP_IMAGE_TYPE_WBMP)
						sprintf(resultBuf, "%s\nIMAGE:WBMP", resultBuf);
					else if (ParsedImage->imagetype == WAP_IMAGE_TYPE_SIS)
						sprintf(resultBuf, "%s\nIMAGE:SIS", resultBuf);
					else
						sprintf(resultBuf, "%s\nIMAGE:UNKNOWN", resultBuf);
				}

				wip_free((WAP_Parsing_IMAGE*)ParsedImage);
				break;
			}

			case _ELEMENT_TYPE_LINK_IMAGE:
			{
				WAP_Parsing_LINK_IMAGE *ParsedLinkImage = (WAP_Parsing_LINK_IMAGE*)(OutPutPtr->pContent);

				if (strlen(resultBuf) == 0)
					sprintf(resultBuf, "LINKIMAGE:[%s]", ParsedLinkImage->linkTitle);
				else
					sprintf(resultBuf, "%s\nLINKIMAGE:[%s]", resultBuf, ParsedLinkImage->linkTitle);

				wip_free((WAP_Parsing_LINK_IMAGE*)ParsedLinkImage);
				break;
			}

			case _ELEMENT_TYPE_SELECT:
			{
				WAP_Parsing_SELECT *ParsedSelect = (WAP_Parsing_SELECT*)(OutPutPtr->pContent);
				WAP_Parsing_OPTION *OptionList = (WAP_Parsing_OPTION*)(ParsedSelect->pHeadFocusOption);
				WAP_Parsing_OPTION *OptionForFree = OptionList;

				char OptionLabels[128];
				memset(OptionLabels, '\0', sizeof(OptionLabels));

				while(OptionList != NULL)					// select 내의 option label을 정리한다.
				{
					if(strlen(OptionLabels) == 0)
						strcpy(OptionLabels, OptionList->OptionLabel);
					else
						sprintf(OptionLabels, "%s,%s", OptionLabels, OptionList->OptionLabel);

					OptionList = OptionList->pNext;
				}

//				CLNTa_log(1,0, "[%s]", OptionLabels);

				if(strlen(resultBuf) == 0)
				{
					if(ParsedSelect->multiSelect)
						sprintf(resultBuf, "SELECT:{%s}", OptionLabels);
					else
						sprintf(resultBuf, "SELECT:(%s)", OptionLabels);
				}
				else
				{
					if(ParsedSelect->multiSelect)
						sprintf(resultBuf, "%s\nSELECT:{%s}", resultBuf, OptionLabels);
					else
						sprintf(resultBuf, "%s\nSELECT:(%s)", resultBuf, OptionLabels);
				}

				// 각 option 컨텐트를 free 하는 루틴...
				OptionList = OptionForFree;
				while(OptionList != NULL)
				{
					if (OptionForFree->pNext != NULL)
					{
						OptionList = OptionForFree->pNext;
						OptionForFree->pNext = NULL;
						wip_free((WAP_Parsing_OPTION*)OptionForFree);
					}
					else
					{	
						OptionList = NULL;
						wip_free((WAP_Parsing_OPTION*)OptionForFree);
					}
					OptionForFree = OptionList;
				}

				break;
			}

		}

		OutPutPtr = OutPutPtr->pNext;
	}

	// Wml_Card에서 이어지는 각 컨텐트 node를 free 하는 루틴...
	OutPutPtr = Wml_Card;

	while(Wml_Card != NULL)
	{
		if (OutPutPtr->pNext != NULL)
		{
			Wml_Card = OutPutPtr->pNext;
			OutPutPtr->pNext = NULL;
			wip_free((WAP_Parsing_WML_NODE*)OutPutPtr);
		}
		else
		{
			Wml_Card = NULL;
			wip_free((WAP_Parsing_WML_NODE*)OutPutPtr);
		}
		OutPutPtr = Wml_Card;
	}


	CLNTa_log(1,0, "[[[ Long result output start ]]]");

	resultLen = strlen(resultBuf);

	resultTokening_output(resultBuf, resultLen);		// result display......

	CLNTa_log(1,0, "[[[ RESULT OUTPUT END ]]]");

	at_send_extended_text("END");


}


void resultTokening_output(char* pSrc, int nLen)
{
	char resultTokening[256];
	char * pDest;
	int count = 0;
	memset(resultTokening, 0x00, sizeof(resultTokening));
	pDest = resultTokening;

	while (nLen > 0) 
	{
		if (count >= 254)		// tokenized result output
		{
			*pDest = '\0';
			at_send_extended_text(resultTokening);

			CLNTa_log(1,0, "%s", resultTokening);

			count = 0;
			memset(resultTokening, 0x00, sizeof(resultTokening));
			pDest = resultTokening;
		}
		if (*pSrc & 0x80) {		// hangul
			if (nLen < 2)
				break;
			*pDest++ = *pSrc++;
			*pDest++ = *pSrc++;
			nLen -= 2;
			count += 2;
		}
		else {					// ascii
			*pDest++ = *pSrc++;
			nLen--;
			count++;
		} 
	}
	*pDest = '\0';
	at_send_extended_text(resultTokening);

}

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void WAP_Parsing_MMI(BOOL uopen_or_newpage)
{	
	WAP_Node *pParsingContent = g_pCard->pHeadContent;


	while(pParsingContent != NULL)		// Parsing MMI data
	{
		if (pParsingContent->type == _ELEMENT_TYPE_KEY)				// keys == menu
		{
			WAP_Content_Key *ParsedContent = (WAP_Content_Key*)(pParsingContent->pContent);
#ifdef WAP_USE_LEAK
			WAP_Parsing_MENU *ParsedKey = (WAP_Parsing_MENU*) WIPAlloc(sizeof(WAP_Parsing_MENU));
			if(!ParsedKey) return;
#else //#ifdef WAP_USE_LEAK
			WAP_Parsing_MENU *ParsedKey = (WAP_Parsing_MENU*) wip_malloc(sizeof(WAP_Parsing_MENU));
#endif //#ifdef WAP_USE_LEAK
			
			ParsedKey->keycode = ParsedContent->keyId; 
			ParsedKey->eventType = ParsedContent->eventType;
			ParsedKey->title = ParsedContent->label;
			ParsedKey->nKey_number = ParsedContent->nSKT_Number;
//			ParsedKey->pNext = NULL;

			if (Wml_Card == NULL)
			{
#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));		// 가장 기본이 되는 Wml_Card 자료구조 선언 
				if(!Wml_Card) return;
#else //#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));		// 가장 기본이 되는 Wml_Card 자료구조 선언 
#endif //#ifdef WAP_USE_LEAK
				Wml_Card->ContentType = _ELEMENT_TYPE_KEY;
				Wml_Card->pContent = (WAP_Parsing_MENU*) ParsedKey;
				Wml_Card->pNext = NULL;
			}
			else 
			{
				WAP_Parsing_WML_NODE *temp_card = Wml_Card;

#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));				
				if(!Card) return;
#else //#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));				
#endif //#ifdef WAP_USE_LEAK
				Card->ContentType = _ELEMENT_TYPE_KEY;
				Card->pContent = (WAP_Parsing_MENU*) ParsedKey;
				Card->pNext = NULL;

				while(temp_card->pNext != NULL)
				{
					temp_card = temp_card->pNext;
				}
				temp_card->pNext = Card;
			}

		}
		else if (pParsingContent->type == _ELEMENT_TYPE_INPUT)		// inputs
		{
			WAP_Content_Input *ParsedContent = (WAP_Content_Input*)(pParsingContent->pContent);
#ifdef WAP_USE_LEAK
			WAP_Parsing_INPUT *ParsedInput = (WAP_Parsing_INPUT*) WIPAlloc(sizeof(WAP_Parsing_INPUT));
			if(!ParsedInput) return;
#else //#ifdef WAP_USE_LEAK
			WAP_Parsing_INPUT *ParsedInput = (WAP_Parsing_INPUT*) wip_malloc(sizeof(WAP_Parsing_INPUT));
#endif //#ifdef WAP_USE_LEAK

			if (ParsedContent->title != NULL)
				ParsedInput->title = ParsedContent->title;
			else 
				ParsedInput->title = "입력";
				
//			ParsedInput->pNext = NULL;

			if (Wml_Card == NULL)
			{
#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));		// 가장 기본이 되는 Wml_Card 자료구조 선언 
				if(!Wml_Card) return;
#else //#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));		// 가장 기본이 되는 Wml_Card 자료구조 선언 
#endif //#ifdef WAP_USE_LEAK
				Wml_Card->ContentType = _ELEMENT_TYPE_INPUT;
				Wml_Card->pContent = (WAP_Parsing_INPUT*) ParsedInput;
				Wml_Card->pNext = NULL;
			}
			else
			{
				WAP_Parsing_WML_NODE *temp_card = Wml_Card;

#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));				
				if(!Card) return;
#else //#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));				
#endif //#ifdef WAP_USE_LEAK
				Card->ContentType = _ELEMENT_TYPE_INPUT;
				Card->pContent = (WAP_Parsing_INPUT*) ParsedInput;
				Card->pNext = NULL;

				while(temp_card->pNext != NULL)
				{
					temp_card = temp_card->pNext;
				}
				temp_card->pNext = Card;
			}

		}
		else if (pParsingContent->type == _ELEMENT_TYPE_TEXT)		// texts
		{
			WAP_Content_Text *ParsedContent = (WAP_Content_Text*)(pParsingContent->pContent);
#ifdef WAP_USE_LEAK
			WAP_Parsing_TEXT *ParsedText = (WAP_Parsing_TEXT*) WIPAlloc(sizeof(WAP_Parsing_TEXT));
			if(!ParsedText) return;
#else //#ifdef WAP_USE_LEAK
			WAP_Parsing_TEXT *ParsedText = (WAP_Parsing_TEXT*) wip_malloc(sizeof(WAP_Parsing_TEXT));
#endif //#ifdef WAP_USE_LEAK

			ParsedText->text = ParsedContent->text;
//			ParsedText->pNext = NULL;

			if (Wml_Card == NULL)
			{
#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));		// 가장 기본이 되는 Wml_Card 자료구조 선언 
				if(!Wml_Card) return;
#else //#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));		// 가장 기본이 되는 Wml_Card 자료구조 선언 
#endif //#ifdef WAP_USE_LEAK
				Wml_Card->ContentType = _ELEMENT_TYPE_TEXT;
				Wml_Card->pContent = (WAP_Parsing_TEXT*) ParsedText;
				Wml_Card->pNext = NULL;
			}
			else 
			{
				WAP_Parsing_WML_NODE *temp_card = Wml_Card;

#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));				
				if(!Card) return;
#else //#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));				
#endif //#ifdef WAP_USE_LEAK
				Card->ContentType = _ELEMENT_TYPE_TEXT;
				Card->pContent = (WAP_Parsing_TEXT*) ParsedText;
				Card->pNext = NULL;

				while(temp_card->pNext != NULL)
				{
					temp_card = temp_card->pNext;
				}
				temp_card->pNext = Card;
			}

		}
		else if (pParsingContent->type == _ELEMENT_TYPE_LINK_TEXT)	// link texts
		{
			WAP_Content_Text *ParsedContent = (WAP_Content_Text*)(pParsingContent->pContent);
#ifdef WAP_USE_LEAK
			WAP_Parsing_LINK_TEXT *ParsedLinkText = (WAP_Parsing_LINK_TEXT*) WIPAlloc(sizeof(WAP_Parsing_LINK_TEXT));
			if(!ParsedLinkText) return;
#else //#ifdef WAP_USE_LEAK
			WAP_Parsing_LINK_TEXT *ParsedLinkText = (WAP_Parsing_LINK_TEXT*) wip_malloc(sizeof(WAP_Parsing_LINK_TEXT));
#endif //#ifdef WAP_USE_LEAK

			ParsedLinkText->text = ParsedContent->text;
			ParsedLinkText->textID = ParsedContent->textId;
//			ParsedLinkText->pNext = NULL;

			if(Wml_Card == NULL)
			{
#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));
				if(!Wml_Card) return;
#else //#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));
#endif //#ifdef WAP_USE_LEAK
				Wml_Card->ContentType = _ELEMENT_TYPE_LINK_TEXT;
				Wml_Card->pContent = (WAP_Parsing_LINK_TEXT*) ParsedLinkText;
				Wml_Card->pNext = NULL;
			}
			else
			{
				WAP_Parsing_WML_NODE *temp_card = Wml_Card;

#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));				
				if(!Card) return;
#else //#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));				
#endif //#ifdef WAP_USE_LEAK
				Card->ContentType = _ELEMENT_TYPE_LINK_TEXT;
				Card->pContent = (WAP_Parsing_LINK_TEXT*) ParsedLinkText;
				Card->pNext = NULL;

				while(temp_card->pNext != NULL)
				{
					temp_card = temp_card->pNext;
				}
				temp_card->pNext = Card;
			}

		}
		else if (pParsingContent->type == _ELEMENT_TYPE_IMAGE)	// image
		{
			WAP_Content_Image *ParsedContent = (WAP_Content_Image*)(pParsingContent->pContent);
#ifdef WAP_USE_LEAK
			WAP_Parsing_IMAGE *ParsedImage = (WAP_Parsing_IMAGE*) WIPAlloc(sizeof(WAP_Parsing_IMAGE));
			if(!ParsedImage) return;
#else //#ifdef WAP_USE_LEAK
			WAP_Parsing_IMAGE *ParsedImage = (WAP_Parsing_IMAGE*) wip_malloc(sizeof(WAP_Parsing_IMAGE));
#endif //#ifdef WAP_USE_LEAK

			ParsedImage->imagetype = ParsedContent->imageType;
//			ParsedImage->pNext = NULL;

			if(Wml_Card == NULL)
			{
#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));
				if(!Wml_Card) return;
#else //#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));
#endif //#ifdef WAP_USE_LEAK
				Wml_Card->ContentType = _ELEMENT_TYPE_IMAGE;
				Wml_Card->pContent = (WAP_Parsing_IMAGE*) ParsedImage;
				Wml_Card->pNext = NULL;
			}
			else
			{
				WAP_Parsing_WML_NODE *temp_card = Wml_Card;

#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));				
				if(!Card) return;
#else //#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));				
#endif //#ifdef WAP_USE_LEAK
				Card->ContentType = _ELEMENT_TYPE_IMAGE;
				Card->pContent = (WAP_Parsing_IMAGE*) ParsedImage;
				Card->pNext = NULL;

				while(temp_card->pNext != NULL)
				{
					temp_card = temp_card->pNext;
				}
				temp_card->pNext = Card;
			}

		}

		else if (pParsingContent->type == _ELEMENT_TYPE_LINK_IMAGE)	// link image
		{
			WAP_Content_Image *ParsedContent = (WAP_Content_Image*)(pParsingContent->pContent);
#ifdef WAP_USE_LEAK
			WAP_Parsing_LINK_IMAGE *ParsedLinkImage = (WAP_Parsing_LINK_IMAGE*) WIPAlloc(sizeof(WAP_Parsing_LINK_IMAGE));
			if(!ParsedLinkImage) return;
#else //#ifdef WAP_USE_LEAK
			WAP_Parsing_LINK_IMAGE *ParsedLinkImage = (WAP_Parsing_LINK_IMAGE*) wip_malloc(sizeof(WAP_Parsing_LINK_IMAGE));
#endif //#ifdef WAP_USE_LEAK

			ParsedLinkImage->linkTitle = ParsedContent->linkTitle;
//			ParsedLinkImage->pNext = NULL;

			if(Wml_Card == NULL)
			{
#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));
				if(!Wml_Card) return;
#else //#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));
#endif //#ifdef WAP_USE_LEAK
				Wml_Card->ContentType = _ELEMENT_TYPE_LINK_IMAGE;
				Wml_Card->pContent = (WAP_Parsing_LINK_IMAGE*) ParsedLinkImage;
				Wml_Card->pNext = NULL;
			}
			else
			{
				WAP_Parsing_WML_NODE *temp_card = Wml_Card;

#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));				
				if(!Card) return;
#else //#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));				
#endif //#ifdef WAP_USE_LEAK
				Card->ContentType = _ELEMENT_TYPE_LINK_IMAGE;
				Card->pContent = (WAP_Parsing_LINK_IMAGE*) ParsedLinkImage;
				Card->pNext = NULL;

				while(temp_card->pNext != NULL)
				{
					temp_card = temp_card->pNext;
				}
				temp_card->pNext = Card;
			}

		}
		else if (pParsingContent->type == _ELEMENT_TYPE_SELECT)					// Wml_Card 에 먼저 Select를 이어지게 하고 그 Select 안에 다시 Option이 링크드되는 구조.
		{
			WAP_Content_Select *ParsedContent = (WAP_Content_Select*)(pParsingContent->pContent);	// Select Content를 파악 
			WAP_Node *ContentInSelect = (WAP_Node*)(ParsedContent->pHeadFocusOption);				// Select Content 내의 Option을 가지는 Node 파악 

#ifdef WAP_USE_LEAK
			WAP_Parsing_SELECT *ParsedSelect = (WAP_Parsing_SELECT*) WIPAlloc(sizeof(WAP_Parsing_SELECT));
			if(!ParsedSelect) return;
#else //#ifdef WAP_USE_LEAK
			WAP_Parsing_SELECT *ParsedSelect = (WAP_Parsing_SELECT*) wip_malloc(sizeof(WAP_Parsing_SELECT));
#endif //#ifdef WAP_USE_LEAK
			ParsedSelect->title = ParsedContent->title;
			ParsedSelect->multiSelect = ParsedContent->multiSelect;
//			ParsedSelect->pNext = NULL;
			ParsedSelect->pHeadFocusOption = NULL;

			if (Wml_Card == NULL)
			{
#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));
				if(!Wml_Card) return;
#else //#ifdef WAP_USE_LEAK
				Wml_Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));
#endif //#ifdef WAP_USE_LEAK
				Wml_Card->ContentType = _ELEMENT_TYPE_SELECT;
				Wml_Card->pContent = (WAP_Parsing_SELECT*) ParsedSelect;
				Wml_Card->pNext = NULL;
			}
			else
			{
				WAP_Parsing_WML_NODE *temp_card = Wml_Card;

#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) WIPAlloc(sizeof(WAP_Parsing_WML_NODE));				
				if(!Card) return;
#else //#ifdef WAP_USE_LEAK
				WAP_Parsing_WML_NODE *Card = (WAP_Parsing_WML_NODE*) wip_malloc(sizeof(WAP_Parsing_WML_NODE));				
#endif //#ifdef WAP_USE_LEAK

				Card->ContentType = _ELEMENT_TYPE_SELECT;
				Card->pContent = (WAP_Parsing_SELECT*) ParsedSelect;
				Card->pNext = NULL;

				while(temp_card->pNext != NULL)
				{
					temp_card = temp_card->pNext;
				}
				temp_card->pNext = Card;
			}

			
			while(ContentInSelect != NULL)
			{
				WAP_Content_Option *OptionContent = (WAP_Content_Option*)(ContentInSelect->pContent);	// 실제 option content를 받는 포인터 선언 

				if (ParsedSelect->pHeadFocusOption == NULL) 
				{
#ifdef WAP_USE_LEAK
					WAP_Parsing_OPTION *ParsedOption = (WAP_Parsing_OPTION*) WIPAlloc(sizeof(WAP_Parsing_OPTION));
					if(!ParsedOption) return;
#else //#ifdef WAP_USE_LEAK
					WAP_Parsing_OPTION *ParsedOption = (WAP_Parsing_OPTION*) wip_malloc(sizeof(WAP_Parsing_OPTION));
#endif //#ifdef WAP_USE_LEAK
					ParsedOption->OptionLabel = OptionContent->label;
					ParsedOption->pNext = NULL;

					ParsedSelect->pHeadFocusOption = (WAP_Parsing_OPTION*)ParsedOption;

//					CLNTa_log(1,0, "[%s]", ParsedOption->OptionLabel);
				}
				else 
				{
					WAP_Parsing_OPTION *TempOption = (WAP_Parsing_OPTION*)ParsedSelect->pHeadFocusOption;

#ifdef WAP_USE_LEAK
					WAP_Parsing_OPTION *options = (WAP_Parsing_OPTION*) WIPAlloc(sizeof(WAP_Parsing_OPTION));
					if(!options) return;
#else //#ifdef WAP_USE_LEAK
					WAP_Parsing_OPTION *options = (WAP_Parsing_OPTION*) wip_malloc(sizeof(WAP_Parsing_OPTION));
#endif //#ifdef WAP_USE_LEAK
					options->OptionLabel = OptionContent->label;
					options->pNext = NULL;
					
					while (TempOption->pNext != NULL)
					{
						TempOption = TempOption->pNext;
					}
					TempOption->pNext = options;

//					CLNTa_log(1,0, "[%s]", options->OptionLabel);
				}

				ContentInSelect = ContentInSelect->pNextFocusNode;		// option 만 포커스를 받으므로 pNextFocusNode 포인터를 이용한다. 
			} // end of while(ContentInSelect != NULL)

		} // end of else if (pParsingContent->type == _ELEMENT_TYPE_SELECT)

		pParsingContent = pParsingContent->pNextNode;			// next node
	}

	WAP_Parsed_MMI_output(uopen_or_newpage);
}



/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void iwfcon_command(void)
{

	// char resultBuf[128];

	char phoneIP[4];
	char gatewayIP[4];
	char primary_dnsIP[4];

	int loop;

	// initialinzing buffers
	memset(resultBuf, 0x00, sizeof(resultBuf));
	memset(phoneIP, 0x00, 4);
	memset(gatewayIP, 0x00, 4);
	memset(primary_dnsIP, 0x00, 4);

	if(wapip != 0)		// WAP connection OK!!
	{
		for (loop=0 ; loop<4 ; loop++)
		{
			phoneIP[4 - loop -1] = ((char*)&Ip_addr)[loop];
			gatewayIP[4 - loop -1] = ((char*)&wapip)[loop];
			primary_dnsIP[4 - loop -1] = ((char*)&primaryIP)[loop];
		}

		sprintf(resultBuf, "*SKT*IWFCON:Connected(IP[%d.%d.%d.%d], Gateway[%d.%d.%d.%d], DNS[%d.%d.%d.%d])",
			phoneIP[0],phoneIP[1],phoneIP[2],phoneIP[3],
			gatewayIP[0],gatewayIP[1],gatewayIP[2],gatewayIP[3],
			primary_dnsIP[0],primary_dnsIP[1],primary_dnsIP[2],primary_dnsIP[3]);

		CLNTa_log(1,0,"%s",resultBuf); 
	}
	else				// WAP connection Fail!!
	{
		char * ConnectFail = "*SKT*IWFCON:Connection Failed";
		strncpy(resultBuf, ConnectFail, 128);
		
	}

    at_send_extended_text(resultBuf);
	
}

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES		

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
BYTE get_Stringlength( BYTE *chk_str, BYTE buff_length)
{
	BYTE i;
	BYTE ret_length = 0;

	for(i=0; i < buff_length ; i++ ) {
		if( (*chk_str != ' ') && *chk_str )
			ret_length = i + 1;
		chk_str++;
	}

	return( ret_length );
}

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES			

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
char* getStringToken(char *source, char *target)
{
	char * temp = target;

//	while(*source != ',')
	while(*source != '`') // SISI, 수정된 Spec.에 의거하여 구분자를 Back Quote로 함. 00/12/08
	{
		if(*source == NULL) return source;

		if(*source & 0x80)			// 한글인 경우 
		{
			*temp = *source;	temp++;		source++;

			if(*source == '`')
				return source;

			*temp = *source;	temp++;		source++;
		}
		else						
		{
			*temp = *source;	temp++;		source++;
		}

	}
	source++;
	return source;

}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES		

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
BYTE token[WAP_MAX_INPUT+1];
char eif_is_delimeter[200] = {0,};

void input_command(char *InputString)
{

//	extern int g_nViewMode;				// 입력 완료 후 다시 그리기 위해 
//	extern WAP_Card *g_pCard;			// pointer of current card
	WAP_Node *pCurNode;
	
	if ((wapip != 0))//&&(SKY_IsInCall()))//if ((wapip != 0)&&(ui.call))
	{
		if(g_pCard == NULL) return;		// 현재 카드가 만들어지지 않았으면 기냥 return

		// InputString = InputString + 6;	// "INPUT=" 을 잘라버리고...
		
		pCurNode = g_pCard->pHeadFocus;		// 포커스 이동을 위해 포인터를 세팅 
		while(pCurNode != NULL)
		{
			if(pCurNode->type == _ELEMENT_TYPE_INPUT)
			{
				int length;

				WAP_Content_Input *pInput = (WAP_Content_Input*)pCurNode->pContent;
			
				memset(token, '\0', WAP_MAX_INPUT+1);

				InputString = getStringToken(InputString, (char*)token);					// INPUT 명령에 실제 필요한 token을 얻는다.


				length = get_Stringlength(token, WAP_MAX_INPUT);

				memset(pInput->text, '\0', WAP_MAX_INPUT+1);
				memcpy(pInput->text, (char*)token, length);

/*				if(strlen(pInput->format) == 0)				
				{
					memcpy(pInput->text, (char*)token, length);
				}
				else
				{
					int nIndexDel = 0;//delimeter
					int nIndexSrc = 0;//source
					int nIndexDest = 0;//destination

					int nKorean = 0;
					for(nIndexSrc=0; nIndexSrc<length; nIndexSrc++)
					{
						if(token[nIndexSrc] & 0x80)//한글
						{
							nKorean++;
							nIndexSrc++;
						}
					}

					nIndexSrc = 0;
					for(nIndexDel=0; nIndexDel<length-nKorean; nIndexDel++)
					{
						if(eif_is_delimeter[nIndexDel] == 0x00)//copy
						{
							if(token[nIndexSrc] & 0x80)//한글(2회 copy)
							{
								pInput->text[nIndexDest] = token[nIndexSrc];
								nIndexDest++;
								nIndexSrc++;
							}
							pInput->text[nIndexDest] = token[nIndexSrc];
							nIndexDest++;
							nIndexSrc++;
						}
						else//skip
						{
							nIndexSrc++;
						}
					}//end of for
				}
*/				pInput = NULL;
				g_pCurFocusNode = (WAP_Node*)pCurNode;									// 마지막 Input에 포커스를 두기 위해 

			}	// end of if(pCurNode->type == _ELEMENT_TYPE_INPUT)

			pCurNode = (WAP_Node*)pCurNode->pNextFocusNode;
			
		}

		//--------------------------------------------------
		g_nViewMode = WAP_VIEW_MODE_WML;
		WAP_Formatting();

		g_nLCDOffset = g_pCurFocusNode->y2 - WAP_DOC_HEIGHT;
		if(g_nLCDOffset < 0) g_nLCDOffset = 0;

        SendEIFCommand(SE_WAP_EIF_INPUT);//		WAP_Display();
		at_send_extended_text("*SKT*INPUT:1");
	}
	else
	{
		at_send_extended_text("*SKT*INPUT:0");
	}
}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void select_value_setting(int (*npOptions)[10])
{
	int op_count = 0;
	int count = 0;
	int count2 = 0;


	WAP_Node *pCurMainNode;
	WAP_Content_Select *pSelect;
   
	if(g_pCard == NULL) return;				// 현재 카드가 만들어지지 않았으면 그냥 return

	pCurMainNode = g_pCard->pHeadFocus;			// 포커스 이동을 위해 포인터를 세팅 
	while(pCurMainNode != NULL)// && npOptions[count][count2] != -1)
	{
		CLNTa_log(1,0,"npOptions[%d][%d] -> [%d]",count,count2,npOptions[count][count2]);

		if(pCurMainNode->type == _ELEMENT_TYPE_SELECT)
		{
			WAP_Node *pCurOptionNode;
					
			pSelect = (WAP_Content_Select*)pCurMainNode->pContent;
			pCurOptionNode = pSelect->pHeadFocusOption;


			if(!pSelect->multiSelect)						// single select
			{
				op_count = 0;
				while(pCurOptionNode != NULL)
				{
					WAP_Content_Option *pOption;
					pOption = (WAP_Content_Option *)pCurOptionNode->pContent;

					op_count++;

					if(npOptions[count][count2] == op_count)
					{
						CLNTa_log(1,0,"%s[[[SELECT]]] npOptions[%d][%d]=[%d],op_count[%d]", pOption->label, count, count2, npOptions[count][count2],op_count);

						pOption->isSelected = TRUE;
						memset(pSelect->selectedOptions, '\0', 255);
						strcpy(pSelect->selectedOptions, pOption->label);

						SKY_optionSelected(pOption->viewId, pOption->optionId);


					}
					else
					{
						CLNTa_log(1,0,"%s<<<deselect>>> npOptions[%d][%d]=[%d], op_count[%d]", pOption->label, count, count2, npOptions[count][count2],op_count);
						pOption->isSelected = FALSE;

					}
					pCurOptionNode = pCurOptionNode->pNextFocusNode;
				}
				count++;
			}
			else											// multi select
			{

//				count2 = 0;
				memset(pSelect->selectedOptions, '\0', 255);

				while(npOptions[count][count2] != -1)
				{
					op_count = 1;

					while(pCurOptionNode != NULL)
					{
						WAP_Content_Option *pOption;
						pOption = (WAP_Content_Option*)(pCurOptionNode->pContent);	// 실제 option content를 받는 포인터 선언 

						if(npOptions[count][count2] == op_count)
						{
							if(!pOption->isSelected)
							{
								pOption->isSelected = TRUE;
								if (strlen(pSelect->selectedOptions) == 0)
									strcpy(pSelect->selectedOptions, pOption->label);
								else
									sprintf(pSelect->selectedOptions, "%s,%s", pSelect->selectedOptions, pOption->label);
								SKY_optionSelected(pOption->viewId, pOption->optionId);
							}
						}
/*
						else
						{
							if(pOption->isSelected)
							{
								pOption->isSelected = FALSE;
								SKY_optionSelected(pOption->viewId, pOption->optionId);
							}
						}
*/

						pCurOptionNode = pCurOptionNode->pNextFocusNode;			// next Option searching
						op_count++;
					}
					pCurOptionNode = pSelect->pHeadFocusOption;
					count2++;
				}

				count++;
				count2 = 0;

			}	// end of if(!pSelect->multiSelect)

		    g_pCurFocusNode = (WAP_Node*)pCurMainNode;						// 마지막 Select에 포커스를 두기 위해 

		}// end of if(pCurMainNode->type == _ELEMENT_TYPE_SELECT)

		pCurMainNode = (WAP_Node*)pCurMainNode->pNextFocusNode;				// next Select searching
		
	}	// end of while(pCurNode != NULL)

	
	//--------------------------------------------------
	g_nViewMode = WAP_VIEW_MODE_WML;
	WAP_Formatting();

    SendEIFCommand(SE_WAP_EIF_AFTER_SELECT);//		WAP_Display();
    at_send_extended_text("*SKT*SELECT:1");

}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void select_command(char* selectedString)
{
	int i, j;
	int npOption[10][10];
	char eachValue[2];						// selectedString에서 얻어온 값.
	int OptionValue = 0;

	for(i=0; i<10; i++)						//init
	{
		for (j=0 ; j<10 ; j++)
			npOption[i][j] = -1;
	}

	i = 0; 
	j = 0;

	while(*selectedString != NULL)
	{
		/////////////////////////////////////////////////////////////
		//
		// selectedString으로부터 선택된 option이 몇번째 option 인지 가져온다. 
		memset((char*)eachValue, '\0', 2);
		eachValue[0] = *selectedString;

		OptionValue = atoi((char*)eachValue);

		selectedString++;

//		if ((*selectedString != ',') && (*selectedString != '/') && (*selectedString != NULL))		// 두자리 수 
		if ((0x30 <= *selectedString) && (*selectedString <= 0x39))
		{
			memset((char*)eachValue, '\0', 2);
			eachValue[0] = *selectedString;
			OptionValue *= 10;
			OptionValue = OptionValue + atoi((char*)eachValue);
			selectedString++;
		}

		npOption[i][j] = OptionValue;

//		CLNTa_log(1,0, "OptionValue[%d], *selectedString[%s]\n npOption[%d][%d][%d]", OptionValue, *selectedString, i,j,npOption[i][j]);

		if(*selectedString == '/')		// Select value is MULTI SELECT VALUE !!!
		{
			j++;

			while(*selectedString != '`')
			{
				selectedString++;
				if (*selectedString == NULL) break;

				memset((char*)eachValue, '\0', 2);
				eachValue[0] = *selectedString;
				OptionValue = atoi((char*)eachValue);
				selectedString++;

//				if ((*selectedString != ',') && (*selectedString != '/') && (*selectedString != NULL))		// 두자리 수 
				if ((0x30 <= *selectedString) && (*selectedString <= 0x39))
				{
					memset((char*)eachValue, '\0', 2);
					eachValue[0] = *selectedString;
					OptionValue *= 10;
					OptionValue = OptionValue + atoi((char*)eachValue);
					selectedString++;
				}				
				npOption[i][j] = OptionValue;

//				CLNTa_log(1,0, "OptionValue[%d], npOption[%d][%d][%d]", OptionValue, i, j, npOption[i][j]);

				j++;

			} // end of while(*selectedString != ',')

			j = 0;

		}	// end of if(*selectedString == '/')		// Select value is MULTI SELECT VALUE !!!

		if (*selectedString != NULL) selectedString++;

		i++;		// 다음번 인덱스에 넣기 위해 

	}

	select_value_setting(npOption);

}


		
/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
#ifdef EIF_WAP
void delcache_command(void)
{
	// extern variable and functions for Cache clear!!
	extern void MEMc_initCache (memSizeType cacheSize, memSizeType restoredSize);
	extern void initCache(void);
	
	// char resultBuf[128];		
	char *ClearGood = "*SKT*DELCACHE:1";		// cache clear OK!!

	memset(resultBuf, 0x00, sizeof(resultBuf));

	if ((wapip != 0)&&(ui.call))
	{
		MEMc_initCache(cCacheSize, crestoredSize);	// browser cache clear
		initCache();

		strncpy(resultBuf, ClearGood, 128);
		CLNTa_log(1,0, "%s", resultBuf);
		at_send_extended_text("*SKT*DELCACHE:1");
	}
	else
	{
		at_send_extended_text("*SKT*DELCACHE:0");
	}
}
#endif  //#ifdef EIF_WAP
/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
ip4a Dest_url_IP;
char Dest_IP[4];
BOOL EIF_dns_query = FALSE;					// EIF_WAP 내에서 uping을 위해 dns query 할 경우 사용한다. 

dword ping_seq;
dword ping_timeStamp;


void uping_command(int count, ip4a Destination_IP)				// 일단은 IP address 로 destination 이 들어오도록 한다. 
{	
	extern int ip_send(ip4a,ip4a,uint8,uint8,uint8,struct dsm_item_s*,uint16,uint16,uint8);
//	extern uint32 CLNTa_currentTime(void);		

	struct icmp uping_icmp_msg;								// uping을 하기 위한 pseudo icmp msg
	dsm_item_type * icmp_msg_to_send;						// 실제로 uping으로 전송되는 data 
	dsm_item_type ** icmp_msg_ptr;

	register uint8 *cp;
	uint16		checksum;
	dword		sendTime;
	qword		startTime;


	icmp_msg_to_send = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);
	icmp_msg_ptr = &icmp_msg_to_send;

	// pseudo icmp msg making 
	uping_icmp_msg.type = ICMP_ECHO;						// ping(echo) request type
	uping_icmp_msg.code = 0;								// ping(echo) request code 
	uping_icmp_msg.args.echo.id = EIF_WAP_UPING_ID;			// UPING 을 위해 임의의 id 값을 정의해서 사용
	uping_icmp_msg.args.echo.seq = count;

	dsm_pushdown(icmp_msg_ptr, NULL, ICMPLEN + sizeof(dword), DSM_DS_SMALL_ITEM_POOL);	// send time을 실어 보낼 때 

	cp = (*icmp_msg_ptr)->data_ptr;

	*cp++ = uping_icmp_msg.type;
	*cp++ = uping_icmp_msg.code;
	cp	  = put16(cp, 0);
	cp    = put16(cp, uping_icmp_msg.args.echo.id);
	cp    = put16(cp, uping_icmp_msg.args.echo.seq);

//	sendTime = CLNTa_currentTime();							// sending 때의 32byte 를 맞추기 위해 ping packet에 넣는 time

	clk_uptime_ms(startTime);								// time을 얻어오고 
	sendTime = qw_lo(startTime);							// 실제 ping packet이 전송된 millisecond time

	cp		 = put32(cp, sendTime);							// Time storing

	checksum = cksum(NULL, *icmp_msg_ptr, dsm_length_packet(*icmp_msg_ptr));
	cp       = &(*icmp_msg_ptr)->data_ptr[2];
	cp       = put16(cp, checksum);

	ping_seq = count;
	ping_timeStamp = sendTime;

//	MSG_HIGH("SND: seq[%d] Stamp[%ld]", ping_seq, ping_timeStamp,0);

	ip_send(Ip_addr, Destination_IP, ICMP_PTCL, NULL, 0, icmp_msg_to_send, ICMPLEN + sizeof(dword), 0, 0);		// send time을 실어 보낼 때 

}

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
//WAP_UPING_UPONG_RTT *g_MainRTT = NULL;

dword ping_RTT = 0;
dword ping_MIN = 0;
dword ping_MAX = 0;

int uping_reply_command(struct icmp *icmp)
{
//	extern uint32 CLNTa_currentTime(void);
	qword  arriveTime;
	dword  rcvTime;
	dword  RTT;


	if(icmp->args.echo.id == EIF_WAP_UPING_ID)	// UPING 에서 사용하는 id 인지 파악 
	{
		clk_uptime_ms(arriveTime);					// 받은 시간 
		rcvTime = qw_lo(arriveTime);

//		MSG_HIGH("RCV(stamp): seq[%d] Stamp[%ld]", icmp->args.echo.seq, rcvTime,0);
		if(icmp->args.echo.seq == ping_seq)
		{
			RTT = rcvTime - ping_timeStamp;

			if (ping_MAX < RTT)						// MAX 계산
			ping_MAX = RTT;

			if (ping_MIN == 0)
				ping_MIN = RTT;
			if (RTT < ping_MIN)						// MIN 계산 
				ping_MIN = RTT;

			ping_RTT = ping_RTT + RTT;				// 평균 계산을 위해 계속 더한다. 

//			MSG_HIGH("RCV(RTT): seq[%d] rtt[%ld]", icmp->args.echo.seq, RTT,0);

			return 1;
		}

	}
	else
	{
		CLNTa_log(1,0, "Invalid ICMP message");
	}
	return 0;

}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void uopen_command(char *WantToRead_URL)
{
	extern void SKY_loadURL (UINT8 viewId, const CHAR* url, BOOL reload);
	char *temp_url = WantToRead_URL;
	
	g_EIF_WAP_UOPEN = TRUE;					// TE2 로부터 UOPEN 명령어가 들어왔다. 

	if (strstr(temp_url, "http") != NULL)	// URL has header.
	{
		CLNTa_log(1,0, "%s", WantToRead_URL);		
		SKY_loadURL(1, WantToRead_URL, TRUE);

	}
	else									// URL has not header
	{
		char url_plus_header[100];
		strcpy(url_plus_header, "http://");
		strcat(url_plus_header, WantToRead_URL);
		CLNTa_log(1,0, "%s", url_plus_header);		
		SKY_loadURL(1, url_plus_header, TRUE);
	}
	
	// 원하는 URL을 OPEN하고 수신한 WML 내용을 SKT format 으로 파싱 -> mmi_adapter.c 파일 내 SKY_showCard() 함수.
}

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void newpage_command(BOOL newpage)
{
	memset(resultBuf, 0x00, sizeof(resultBuf));

	if (newpage)			// NEWPAGE mode 설정 
	{
		g_EIF_WAP_NEWPAGE = TRUE;		
	}
	else
	{		
		g_EIF_WAP_NEWPAGE = FALSE;	
	}	
}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void geturl_command(int number)
{
	WAP_Node *pParsingContent = g_pCard->pHeadContent;

	memset(resultBuf, 0x00, sizeof(resultBuf));

	if ((wapip != 0))//&&(SKY_IsInCall())) //if ((wapip != 0)&&(ui.call))
	{
		if (number == 0)			// The URL of current card
		{
			char * result = "*SKT*GETURL:0,";
			strncpy(resultBuf, result, sizeof(resultBuf));
			strcat(resultBuf, g_pCard->URL);

			CLNTa_log(1,0, "%s", resultBuf);
			
			at_send_extended_text(resultBuf);
		}
		else		// 현재 카드의 key에 대응되는 URL 가져오기.... current card URL을 제외하고 key에 따른 URL은 모두 SKY_linkInfo 함수 내에서 출력 
		{
			extern void MMIc_linkInfo (UINT8 viewId, UINT8 widgetType , UINT8 widgetId);
//			WAP_Node *pParsingContent = g_pCard->pHeadContent;
			int How_Many_Key = 0;
			
			while (pParsingContent != NULL)
			{
				if (pParsingContent->type == _ELEMENT_TYPE_KEY)			// keys == menu
				{
					How_Many_Key = How_Many_Key + 1;					// key 개수 파악 
				}
				CLNTa_log(1,0, "TYPE:%d ", pParsingContent->type);
				pParsingContent = pParsingContent->pNextNode;			// next node
			}
    		CLNTa_log(1,0, "How_Many_Key=%d", How_Many_Key);
			
			if (number > How_Many_Key)									// current card has not key number
			{
				sprintf(resultBuf, "*SKT*GETURL:%d,", number);
				sprintf(resultBuf, "%s wrong menu number", resultBuf);

				CLNTa_log(1,0, "%s", resultBuf);
			}
			else
			{
				g_EIF_WAP_GETURL = TRUE;
				sprintf(resultBuf, "*SKT*GETURL:%d,", number);
				MMIc_linkInfo(1, WIDGET_DOLINK, number);

				CLNTa_log(1,0, "%s", resultBuf);		
			}
		}

	}
	else
	{
		at_send_extended_text("ERROR : You must connect first.");
	}
}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
#ifdef EIF_WAP
int After_WAP_start_what_call = 0;

void EIF_WAP_start(int what_func, char* connect_url)			// wap start and IWFCON, UOPEN, UPING func call 
{
	extern void ds_send_ui_cmd(ui_name_type);					// send cmd to ui task from ds task

	if (what_func == 1)											// IWFCON
	{
		After_WAP_start_what_call = 1;
		CLNTa_log(1, 0, "EIF_WAP_start to IWFCON");

		ds_send_ui_cmd(UI_DS_EIF_WAP_CONNECT_F);
	}
	else if (what_func == 2)									// UOPEN
	{
		After_WAP_start_what_call = 2;					
		strncpy(EIF_url, connect_url, sizeof(EIF_url));
		CLNTa_log(1, 0, "[%s] in EIF_WAP_start to UOPEN", EIF_url);

		if ((wapip != 0)&&(ui.call))							// WAP connection OK!!
		{
			After_WAP_start_what_call = 0;
			uopen_command(EIF_url);
		}
		else
			ds_send_ui_cmd(UI_DS_EIF_WAP_CONNECT_F);			// wap start and UOPEN func is called in wapstate.c 

	}
	else if (what_func == 3)									// UPING
	{
		After_WAP_start_what_call = 3;			
	
		strncpy(EIF_url, connect_url, sizeof(EIF_url));		

		ds_send_ui_cmd(UI_DS_EIF_WAP_CONNECT_F);				// wap start and UPING func is called in wapstate.c
	}

}
#endif // #ifdef EIF_WAP
/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
int NKey_count = 0;								// N Key control 과 Link Text uopen 에서 같이 사용한다. 

void NKey_uopen_command(int whatKey)
{
	if (g_NKey_UOPEN)
	{
		NKey_count = whatKey; 
		SendEIFCommand(SE_WAP_EIF_NKEY_OPEN);//ds_send_ui_cmd(UI_EIF_WAP_NKEY_OPEN_F);					// N keys Selected..... Load URL N keyID..... 
	}
	else
	{
		at_send_extended_text("N Key Open Fail.. ");
	}

}

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
int NKey_Uopen_Loading(int KeyCode)
{
	extern BOOL WAP_Handle_NumKey_ForDo_EIFWAP(int);

	if(WAP_Handle_NumKey_ForDo_EIFWAP(KeyCode))
	{
//		if(Wtlshello)
//		{
//			return(WAP_WTLS_SESSION_START);
//		}
		return(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
	MSG_ERROR("dummy!! called nkey_upon_loading",0,0,0);
	//01.04.04 ytchoi to solve warning
	return -1;
}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void LinkText_open_command(int whatNumber)
{
	if (g_Link_Text_UOPEN)
	{
		NKey_count = whatNumber; 
		SendEIFCommand(SE_WAP_EIF_LINKTEXT_OPEN);//ds_send_ui_cmd(UI_EIF_WAP_LINKTEXT_OPEN_F);					// Link Text Selected..... Load URL text ID..... 
	}
	else
	{
		at_send_extended_text("Link Text Open Fail.. ");
	}

}

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
int LinkText_Uopen_Loading(int textID)
{
	extern BOOL WAP_Handle_LinkText_ForDo_EIFWAP(int);

	if(WAP_Handle_LinkText_ForDo_EIFWAP(textID))
	{
//		if(Wtlshello)
//		{
//			return(WAP_WTLS_SESSION_START);
//		}
		return(WAP_HANDLE_KEY_RESULT_MAYBE_LOADING);
	}
	//01.04.04 ytchoi to solve warning
	return -1;
}


/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void eif_error_command(int Kind_of_error)
{
	switch(Kind_of_error)
	{
		case EIF_ERR_UOPEN_UNKONWN_CONTENT:
			at_send_extended_text("*SKT*UOPEN:0 [Unknown Content]");
			break;
		case EIF_ERR_UOPEN_FILE_NOT_FOUND:
			at_send_extended_text("*SKT*UOPEN:0 [File not found]");
			break;
		case EIF_ERR_UOPEN_NOANSWER_SERVER:
			at_send_extended_text("*SKT*UOPEN:0 [No answer from server]");
			break;
		case EIF_ERR_UOPEN_INVALID_URL:
			at_send_extended_text("*SKT*UOPEN:0 [Invalid URL]");
			break;
		case EIF_ERR_UOPEN_CONNECT_FAIL:
			at_send_extended_text("*SKT*UOPEN:0 [Network connecting failure]");
			break;
		case EIF_ERR_IWFCON_NOANSWER_SERVER:
			at_send_extended_text("*SKT*IWFCON:Connection Failed [No answer from server]");
			break;
		case EIF_ERR_IWFCON_CONNECT_FAIL:
			at_send_extended_text("*SKT*IWFCON:Connection Failed [Network connecting failure]");
			break;
		case EIF_ERR_IWFCON_NOT_IDLE:
			at_send_extended_text("*SKT*IWFCON:Connection Failed [Phone is not idling]");
			break;
		case EIF_ERR_UPING_CONNECT_FAIL:
			at_send_extended_text("*SKT*UPING:Connection Failed");
			break;
	}
	
}

//#endif //#ifdef FEATURE_SKT_EIF// #ifdef EIF_WAP



