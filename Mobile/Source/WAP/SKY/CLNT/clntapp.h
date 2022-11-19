#ifndef CLNTFUNCTION_H
#define CLNTFUNCTION_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           Wireless Application Protocol   Client Function Header

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/21/00   swjung  Define prototypes of extra wmlscript function applications
===========================================================================*/
/* <EJECT> */
//#include "custA7.h"
/////////////////////////////////////////////////////////////////////////
// Prototypes for CLNTa_resetTimer, CLNTa_setTimer
/////////////////////////////////////////////////////////////////////////
#ifdef WAP_B1
#include "comdef.h"
#endif //WAP_B1

#include "si_si.h"

#if defined(WAPPER) || defined(FEATURE_WAP) 
/*===========================================================================

  WMLScript Extra Library Execution Result Definition

===========================================================================*/
#define SUCCESS                       0
#define INVALID_URL                   1001
#define NOT_SUPPORTED_FORMAT          1002
#define INVALID_PARAMETER             1003
#define INVALID_DATA_FORMAT           1004
#define OVERRANGE_PARAMETER           1005
#define EMPTY_ENTRY                   1006
#define SAME_ENTRY                    1007
#define UNDEFINED_ERROR               1

#define INITIAL_URL					  0
#define GATEWAY_ADDR				  1
#define NTOP_EMAIL					  2


#define	PB_ITEM_ENTRY_ID	0x0001
#define	PB_ITEM_NAME		0x0002
#define	PB_ITEM_HOME		0x0004
#define	PB_ITEM_MOBILE		0x0008
#define	PB_ITEM_SCHOOL		0x0010
#define	PB_ITEM_FAX			0x0020
#define	PB_ITEM_EMAIL		0x0040
#define	PB_ITEM_GROUP_ID	0x0080
#define	PB_ITEM_GROUP_NAME	0x0100

#define	PB_ITEM_EXT_MEMO	0x0001
#define	PB_ITEM_EXT_ANNI	0x0002

#endif //#ifdef WAPPER

extern uint32 tid_list;
extern int first_flag_tid;

void timerInsert(uint32 ticks);
void timerDelete(void);
void timerTick(int4  interval);
void timerTerminate(void);

/////////////////////////////////////////////////////////////////////////
// Prototypes for Extra WMLScript Library Functions
/////////////////////////////////////////////////////////////////////////
void clnt_convert_string_response(char *pStringResult, WMLSvar *pResult);
void clnt_make_string_response(unsigned short invokeId, char *pStringResult);
void clnt_make_integer_response(unsigned short invokeId, int integerResult);
void clnt_init_return_value(WMLSvar * pVariable);
void clnt_alloc_string(const WMLSvar * const *params, char **argString, UINT8 nbrOfArgs);
void clnt_free_string(char **argString, UINT8 nbrOfArgs);
WMLSvar*  clnt_decide_library_response(BOOL isSeparateResponse, BOOL *pSeparateResponse, WMLSvar *pResult, char *pResultString);
BOOL clnt_check_lib_range(UINT16 libNbr, UINT8 funcNbr);
void clnt_adjust_index(UINT16* libNbr, UINT8* funcNbr);



/////////////////////////////////////////////////////////////////////////
// Multimedia Library Functions
/////////////////////////////////////////////////////////////////////////
void setImage(unsigned short invokeId, char* url, char* title, int kindOfImage, int typeOfImage);
#ifdef FEATURE_MUSIC_BELL
	void Musicbell_play_tmp(unsigned short invokeId, char* URL,char * Title,  char* AudioType, char* PlayType);
	void Musicbell_stop(char* Result);
	void Musicbell_download_tmp(unsigned short invokeId, char* URL,char * Title, char* AudioType, char* Attribute, char* Result);
	void Musicbell_getlistTitle(char* Result);
	void Musicbell_memplay(char* Title, char* Result);
	void Musicbell_deletelist(char* Title, char* Result);
#endif //FEATURE_MUSIC_BELL

#ifdef FEATURE_GVM
#ifdef BLOCK
	void GoToGVM
	(
		char* uid,       // 선택한 게임에서 사용하는 사용자 이름
		int   cpid,      // CP의 id, 범위는           0x0000 ~ 0xfffe
		int   gid,       // 실행할 게임의 id          0x0000 ~ 0xffff
		char* dlsIP,     // 다운로드서버의 IP address "game.sinjisoft.com"
		int   dlsPort,   // 다운로드서버의 port       0 ~ 65535
		char* svrIP,     // 서버 IP address           
		int   svrPort,   // 서버 port
		char* retURL,    // 게임종료후 go back을 위한 URL
		char* Result
	);
#endif //BLOCK
#endif //netgame
/////////////////////////////////////////////////////////////////////////
// Terminal Library Functions
/////////////////////////////////////////////////////////////////////////
#ifdef WAP_SETURL
	int setURL(char *URL, int kindOfURL);
#endif //WAP_SETURL
void sendMail(char * result);

//Decide direct response or separate response?????????????????
//void setDormantTime(unsigned short invokeId, int dormantTime);
int setDormantTime(unsigned short invokeId, int dormantTime);

#ifdef WAP95C_TEST
int muxOption(unsigned short invokeId, int Option);
#endif //WAP95C_TEST


void getNID(char *result);
void getZone(char *result);
void getBase(char *result);
void getPilot(char *result);

/////////////////////////////////////////////////////////////////////////
// Phonebook Library Functions
/////////////////////////////////////////////////////////////////////////

#ifdef NEW_VERSION
	void isModifyAll(int Start, int End, char *result);
#else //#ifdef NEW_VERSION
	void isModifyAll(char *result);
#endif //#ifdef NEW_VERSION

void isExist(char *name, char *result);
void isGroup(char *groupname, char *result);
void getMaxEntry(char *result);
void getMaxGroup(char *result);
void getMaxShortKey(char *result);
void getPhonebook(char *result);

#ifdef NEW_VERSION
	void getDeletedEntry(int Start, int End, char *result);
#else //#ifdef NEW_VERSION
	void getDeletedEntry(char *result);
#endif //#ifdef NEW_VERSION

void getGroup(char *result);
int setGroup(int GroupID, char *groupname);
void getEntry(int EntryID, char *result);
int deleteEntry(int EntryID );
int setEntry(int EntryID, char *data);
void getVerID(char *result);
int getEntryList(int subList, int startEntryID, int number, int extFlag, char *result, unsigned int sizeOfString);
/////////////////////////////////////////////////////////////////////////


//MCommerce test library function
/////////////////////////////////////////////////////////////////////////
#ifdef FEATURE_MCOMMERCE
#ifndef FEATURE_WAP
	extern char * pkSignData(const char * input, int nAlgo);
	extern char * createCSR(const char * dn);
	extern char * pkEncryptData(const char * input, const char * cert);
#endif //#ifndef FEATURE_WAP
#endif //#define FEATURE_MCOMMERCE
/////////////////////////////////////////////////////////////////////////


#ifdef FEATURE_GVM

	enumErrorCode Call_Netgame_Gamerun( pstructSI si, unsigned short libIndex, unsigned char funcIndex );		
#ifdef BLOCK
WMLSvar* Call_Netgame_Gamerun(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8 nbrOfArgs);
#endif //BLOCK
#endif //netgame

#ifdef FEATURE_SK_VM
WMLSvar* Call_SKVM_SetURL(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8 nbrOfArgs);	
#endif //FEATURE_SK_VM

#ifdef FEATURE_WAP_WAVELET
WMLSvar* Call_Wavelet(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8 nbrOfArgs);			
#endif//FEATURE_WAP_WAVELET

WMLSvar* Call_Multimedia_setImage(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8 nbrOfArgs);
WMLSvar* Call_Musicbell_Play(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Musicbell_Stop(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Musicbell_DownLoad(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Musicbell_GetListTitle(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Musicbell_MemPlay(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Musicbell_DeleteList(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);

WMLSvar* Call_Terminal_setURL(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_sendMail(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_setDormantTime(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_muxOption(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);

WMLSvar* Call_Terminal_isModifyAll(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_isExist(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_isGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getMaxEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getMaxGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getMaxShortKey(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getPhonebook(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getDeletedEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_setGroup(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_deleteEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_setEntry(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getVerID(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Terminal_getEntryList(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);

#ifdef FEATURE_MCOMMERCE
WMLSvar* Call_Mcommerce_testScriptA(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Mcommerce_testScriptB(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
WMLSvar* Call_Mcommerce_testScriptC(UINT16 invokeId, const WMLSvar * const *params, BOOL *pSeparateResponse, UINT8	nbrOfArgs);
#endif //#ifdef FEATURE_MCOMMERCE

#define FEATURE_PIM_INVALIDERROR

#endif //#ifndef CLNTFUNCTION_H