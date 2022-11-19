/************************************************************************************/
/*																					*/
/*							External Interface 1.1 Header file						*/
/*							for WAP Functions										*/
/*																					*/
/************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "aapimem.h"
#include "tapicmmn.h"
#include "tapimmi.h"
#include "mmi_data.h"
#include "mmi_func.h"

#include "ds.h"
#include "dsm.h"						// need to make ICMP message in UPING command
#include "icmp.h"						// need to make ICMP message in UPING command
#include "ip.h"							// need to make ICMP message in UPING command
#include "psmisc.h"						// need to make ICMP message in UPING command
#include "qw.h"							// need to make ICMP message in UPING command
#include "GenDsp.h"

#define EIF_WAP_UPING_ID 65535			// UPING command uses this value for id value in ICMP header




extern int g_nViewMode;					// 입력 완료 후 다시 그리기 위해 
extern int g_nLCDOffset;				// 입력 완료 후 포커스 위치를 화면에 보여주기 위해 
extern WAP_Node *g_pCurFocusNode;		// 입력 완료 후 포커스를 지정하기 위해 
extern WAP_Card *g_pCard;				// pointer of current card



/*********************** for IWFCON Function **********************************/
//typedef unsigned long int ip4a; 

extern ip4a Ip_addr;		// phone에 할당된 IP
extern ip4a wapip;			// Gateway IP
extern ip4a primaryIP;		// dns IP
extern ip4a secondaryIP;	// dns IP



/*********************** for DELCACHE Function **********************************/
extern const    memSizeType   cCacheSize;
extern const    memSizeType   crestoredSize;




/*********************** for debuging **********************************/
extern void CLNTa_log(unsigned char viewID, signed short logNo, const char *format, ...);


/*********************** for WAP_Parsing_MMI_data **********************************/
typedef struct WAP_Parsing_MMI_data_NODE
{
	int ContentType;
	void *pContent;
	struct WAP_Parsing_MMI_data_NODE *pNext;

} WAP_Parsing_WML_NODE;



typedef struct WAP_Parsing_MMI_data_MENU
{
	UINT8 keycode;
	char* eventType;
	char *title;
	int nKey_number;
//	struct WAP_Parsing_MMI_data_MENU *pNext;

} WAP_Parsing_MENU;

typedef struct WAP_Parsing_MMI_data_INPUT
{
	char *title;
//	struct WAP_Parsing_MMI_data_INPUT *pNext;

} WAP_Parsing_INPUT;

typedef struct WAP_Parsing_MMI_data_TEXT
{
	char *text;
//	struct WAP_Parsing_MMI_data_TEXT *pNext;

} WAP_Parsing_TEXT;

typedef struct WAP_Parsing_MMI_data_LINK_TEXT
{
	char *text;
    UINT8 textID;
//	struct WAP_parsing_MMI_data_LINK_TEXT *pNext;

} WAP_Parsing_LINK_TEXT;


typedef struct WAP_Parsing_MMI_data_IMAGE
{
	int imagetype;
//	struct WAP_Parsing_MMI_data_IMAGE *pNext;

} WAP_Parsing_IMAGE;


typedef struct WAP_Parsing_MMI_data_LINK_IMAGE
{
	char *linkTitle;
//	struct WAP_Parsing_MMI_data_LINK_IMAGE *pNext;

} WAP_Parsing_LINK_IMAGE;

typedef struct WAP_Parsing_MMI_data_OPTION
{
	char *OptionLabel;
	struct WAP_Parsing_MMI_data_OPTION *pNext;

} WAP_Parsing_OPTION;

typedef struct WAP_Parsing_MMI_data_SELECT
{
	char *title;
	BOOL multiSelect;
//	WAP_Node* pHeadFocusOption;
	WAP_Parsing_OPTION* pHeadFocusOption;
	
//	struct WAP_Parsing_MMI_data_SELECT *pNext;

} WAP_Parsing_SELECT;

/*
typedef struct WAP_UPING_TIME
{
	uint16 count;
	dword send_time;

	struct WAP_UPING_TIME *pNext;

} WAP_UPING_ICMP_TIME;


typedef struct WAP_UPONG_RTT
{
	dword RTT;
	struct WAP_UPONG_RTT *pNext;

} WAP_UPING_UPONG_RTT;
*/



/************************************************************************************/
/*																					*/
/*							WAP Function Prototypes for EIF 1.1						*/
/*																					*/
/************************************************************************************/
void WAP_Parsed_MMI_output(BOOL);

void WAP_Parsing_MMI(BOOL);

void iwfcon_command(void);

void cap_command(void);

void input_command(char*);

void delcache_command(void);

void uping_command(int, ip4a);

void uopen_command(char*);

void eif_error_command(int);

void newpage_command(BOOL);

void geturl_command(int);

void delallmail_command(void);

void select_command(char*);

void EIF_WAP_start(int, char*);