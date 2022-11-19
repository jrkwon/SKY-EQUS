// client header file
#ifndef CLNT_H
#define CLNT_H

/*===========================================================================

							CLIENT ADAPTER HEADER

DESCRIPTION

 Copyright (c) 1999 by SK teletech. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/99   swjung  Define macros for client adapter function 
===========================================================================*/
#include "tapicmmn.h"
#include "tapiclnt.h"
#include "customer.h"

/////////////////////////////////////////////////////////////////////////////
// Buffers for CLNTa_log()
/////////////////////////////////////////////////////////////////////////////
#define DM_LINE_SIZE     38
#define DEBUG_BUF_SIZE   50
#define NUM_OF_BUFFER    100
#define TEMP_BUF_SIZE    400
#define MSG_BUF_SIZE     400

/////////////////////////////////////////////////////////////////////////////
// Definitions for CLNTa_content()
/////////////////////////////////////////////////////////////////////////////
/* CLNTa_getContent() */
#define MAX_VND_NUM	10
#define MAX_VND_SIZE	20

/* Content type 추가는 여기에.... */
#define IMAGE_WBMP			0
#define IMAGE_ANIMATION		1
#define IMAGE_BMP			2
#define SecTLS				3
#define MYBELL				4

#ifdef FEATURE_MUSIC_BELL
	#define MUSICBELL_TYPE_PLAY          5
	#define MUSICBELL_TYPE_DOWN          6
#endif//FEATURE_MUSIC_BELL

#define CONTENT_TYPE_INVALID -1

//#define MAX_CONTENT_SIZE	2048//MAX MRU SIZE
//#define MAX_CONTENT_SIZE	4096//MAX MRU SIZE
#define MAX_CONTENT_SIZE    15*1000 //MAX MRU SIZE
#define MAX_SIZE_EVENT_TYPE 30
#define MAX_SIZE_URL		100
#define MAX_SIZE_TITLE		30
#define MAX_NUM_MYBELL		10

#ifdef FEATURE_MUSIC_BELL
	#define MAX_SIZE_MUSIC_RESULT   10
#endif //FEATURE_MUSIC_BELL

#ifdef FEATURE_MUSIC_BELL
	#define MAX_SIZE_PLAYTYPE   30
	#define MAX_SIZE_AUDIOTYPE  30
#endif //FEATURE_MUSIC_BELL

#define SUCCESS								0
#define SETIMAGE_SUCCESS					0
#define SETIMAGE_UNDEFINED_ERROR			1

#ifdef FEATURE_MUSIC_BELL
	#define MUSICBELL_SUCCESS		"0"
	//#define M_INVALID_URL			"1001"
	//#define M_INVALID_DATA_FORMAT	"1002"
	//#define M_INVALID_PARAMETER		"1003"
	//#define M_FILENUM_FULL			"1004"
	#define BUFFER_FULL				"1005"
	//#define UNDEFINED_ERROR			"1006"
	//#define DOWNLOAD_FAIL			""
#endif //#ifdef FEATURE_MUSIC_BELL

/////////////////////////////////////////////////////////////////////////////
// Definitions for formatting output string of CLNTa_log()
/////////////////////////////////////////////////////////////////////////////
#define HT_SIZE          2
#define NL_SIZE          DM_LINE_SIZE
#define CR_SIZE          DM_LINE_SIZE

#define NEW_LINE         '\n'
#define HORIZONTAL_TAB   '\t'
#define CARRIAGE_RETURN  '\r'
#define PERIOD              '.'
#define SPACE				' '
#define SEMI_COLON			';'


/////////////////////////////////////////////////////////////////////////////
// Definitions for CLNTa_currentTime()
//		Elapsed time from 1970 Jan 1st 00:00:00 
//		to 1980 Jan 6th 00:00:00 in second unit
/////////////////////////////////////////////////////////////////////////////
#define ELAPSED_SECONDS  315964800
//#define ELAPSED_SECONDS  315932400 //(315964800 - 3600*9)

extern UINT32 clk_read_secs( void );
extern void debug_msg( char * str, int a, int b, int c);


/*
** Management Information Base enum value --> refer to IANA character set information
*/
#define KS_C_5601_1987					36 
#define KS_C_5601_1987_NULL_LENGTH 		1 
#define UNKNOWN_CHAR_SET				0

/*
** Define pointer to functions for setTranscodes
*/
typedef BOOL	(*fPtr_Ksc2Unicode_canConvert) (INT16);
typedef INT32	(*fPtr_Ksc2Unicode_calcLen) (BYTE*, INT16, BOOL, UINT32, UINT32*);
typedef BOOL	(*fPtr_Ksc2Unicode_convert) (BYTE*, INT16, UINT32, WCHAR*, UINT32);
typedef UINT8	(*fPtr_Ksc2Unicode_getNullTermByteLen) (INT16);

/////////////////////////////////////////////////////////////////////////////
// Definitions for CLNTa_error()
//		Checks the range of error and return it 
/////////////////////////////////////////////////////////////////////////////
#define ERR_RESULT_IS_TERMINATE				100
#define ERR_RESULT_IS_REPAINT_OR_TERMINATE	200
#define ERR_RESULT_IS_IGNORE				300
#define ERR_RESULT_IS_TIMEOUT				400

typedef struct
{
	int		urlID;
	char	eventType[MAX_SIZE_EVENT_TYPE];
} eventTableType;

/////////////////////////////////////////////////////////////////////////////
// Definitions for CLNTc_start()
//		Define view id according to user agent
/////////////////////////////////////////////////////////////////////////////
#define	VIEW_ID_WML		1
#define	VIEW_ID_WTA		VIEW_ID_WML+1
#define	VIEW_ID_CONTENT	VIEW_ID_WTA+1

typedef struct
{
	char    url[MAX_SIZE_URL];
	char	title[MAX_SIZE_TITLE];
	int		kindOfImage;
	int		imageType;
} setImageType;

#ifdef BLOCK
	extern char vendorType[MAX_VND_NUM][MAX_VND_SIZE];
	extern void wap_call_get_content(int kindOfContent, int index);
#endif //BLOCK
extern void	wap_handle_error(int errorNumber, int errorType);
#ifdef PORTING_B0_WAP
extern void	wap_set_error(boolean isError);
#endif //PORTING_B0_WAP

extern void	wap_get_item(char *pSource, char *pTarget);
extern void	wap_make_point(char *pSource);
extern void	wap_make_line(char *pSource);
extern void	wap_make_rectangle(char *pSource);
extern void	wap_make_circle(char *pSource);
extern int	wap_check_error_range(INT16 errorNo);
extern int	wap_compare_name(const char *pSource, const char *pTarget);
extern int	wap_check_library(const char *pSource, const char *pTarget);

extern BOOL clnt_check_lib_range(UINT16 libNbr, UINT8 funcNbr);
extern void clnt_adjust_index(UINT16* libNbr, UINT8* funcNbr);
extern void clnt_make_string_response(unsigned short invokeId, char *pStringResult);
extern void clnt_make_integer_response(unsigned short invokeId, int integerResult);

#ifdef FEATURE_MUSIC_BELL
	typedef struct
	{
		char    URL[MAX_SIZE_URL];
		char	Title[MAX_SIZE_TITLE];
		char    PlayType[MAX_SIZE_PLAYTYPE];
		char    AudioType[MAX_SIZE_AUDIOTYPE];
		char    Attribute[MAX_SIZE_AUDIOTYPE];
	} DownMusicbellType;
#endif //FEATURE_MUSIC_BELL


/////////////////////////////////////////////////////////////////////////////
// Definitions for MMIc_openView()
//		Enable or disable CONTENT_UA viewId according to AUS WAP Browser version
/////////////////////////////////////////////////////////////////////////////
#define BROWSER_VERSION_2010	2010
#define BROWSER_VERSION_2030	2030
#define BROWSER_VERSION_2501	2501
#define BROWSER_VERSION					BROWSER_VERSION_2501

#if	BROWSER_VERSION_2010 < BROWSER_VERSION
	#define	DISABLE_CONTENT_UA_VIEWID
	#define	CLNT_SINGLE_ACTIVE_TIMER
	#define BROWSER_UPDATE
	#define ABORT_IN_CONTENT
#endif


//////////////////////////////////////////////////////////////////////
// 1. invoke id
// 2. argument list
// 3. pSeparateResponse
// 4. nbrOfArgs
//////////////////////////////////////////////////////////////////////
typedef struct 
{
	WMLSvar*	(*PFunc)(UINT16, const WMLSvar * const *, BOOL*, UINT8);
	UINT8		nbrOfArgs;
} ExtraLibInfo;

#define MAX_NUM_OF_ARG 8

typedef struct 
{
	int	argType[MAX_NUM_OF_ARG];
} ExtraLibArgInfo;

enum libraryType
{
    EXTRA_LIB_START			=	31999,
	EXTRA_LIB_MULTIMEDIA,	
	EXTRA_LIB_TERMINAL,	
	EXTRA_LIB_PHONEBOOK,
#ifdef FEATURE_MCOMMERCE
	EXTRA_LIB_MCOMMERCE,
#endif //#ifdef FEATURE_MCOMMERCE
	EXTRA_LIB_DUMMY,
	EXTRA_LIB_MCOMMERCE_COMMERCIAL,
    EXTRA_LIB_END
};



#ifdef FEATURE_MCOMMERCE
//For mobile wallet shopping list
typedef struct 
{
  unsigned short invokeId;
  char appendString[1000];
  unsigned int setStringEnc;
  unsigned int setStringSign;
  char extField[32];
  char recipientCrt[2000];
  int	keyAgreeAlg;
  char contentEncAlg[32];
  int keyIdType;
  char keyId[32];

} GetMobileWalletDataType;

typedef struct 
{
  unsigned short invokeId;
  int nCommand;
  char strSubject[100];
  char strPolicy[100];
} CSRDataType;

#define MAX_CERT_SIZE 4096
typedef struct 
{
  int  length;
  char data[MAX_CERT_SIZE];
} CertMIMEDataType;

enum tagENUM_SCRIPT_E {
    REQUEST_CERT = 0,
	ENCRYPT_TEXT,
	SIGN_TEXT,
	GET_MW_DATA
};

typedef enum tagENUM_SCRIPT_E ENUM_SCRIPT_E;

#define CERT_SUCCESS					1
#define CERT_FAILURE					0
#define UNKNOWN_TYPE					100
#define CERT_RESPONSE_TYPE				0
#define HASHED_CA_CERT_RESPONSE_TYPE	1
#define SIGNED_CA_CERT_RESPONSE_TYPE	2

#endif //#ifdef FEATURE_MCOMMERCE




#define TYPE_INTEGER	0
#define TYPE_FLOAT		1
#define TYPE_STRING		2
#define TYPE_BOOL		3
#define TYPE_INVALID	4


#define NEW_VERSION 100

#define RESULT_STRING_MAX			1500
#define URL_STRING_MAX				100
#define TEMP_STRING_MAX				100


#ifdef FEATURE_MUSIC_BELL
	#define AUDIOTYPE_STRING_MAX	100
	#define PLAYTYPE_STRING_MAX		100
#endif //FEATURE_MUSIC_BELL


#define EXTRA_LIB_FUNC_MAX			20

#define MUSICBELL_FUNC_OFFSET		100
#define MUSICBELL_FUNC_MIN			101
#define MUSICBELL_FUNC_MAX			106
#define MULTIMEDIA_LIB_FUNC_MAX		MUSICBELL_FUNC_MAX

#define FUNC_NUM_GVM_GAMERUM		1
#define FUNC_NUM_GVM_INDEX  		8

#define FUNC_NUM_SKVM_SETURL		30
#define FUNC_NUM_SKVM_SETURL_INDEX	10

#define FUNC_NUM_WAVELET	    	15
#define FUNC_NUM_WAVELET_INDEX    	12

#define DIRECT_RESPONSE
#define FEATURE_WAP_ERROR_RELOAD

#endif //#ifndef CLNT_H
