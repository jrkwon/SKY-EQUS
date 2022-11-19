#include "customer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>



#include "sky.h"

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
#endif //#ifdef WAP_NEO

#include "logcodes.h"
#include "errcodes.h"
#include "confvars.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "si_libs.h"

#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif

#include "GenDsp.h"
#include "mmi_data.h"
#include "mmi_func.h"

#include "queue.h"

#include "customer.h"
#include "clnt.h"

#include "wapconnector.h"
#include "clntapp.h"
#include "capimmi.h"

#ifdef FEATURE_WAP
#include "wapcommon.h"
#endif // #ifdef FEATURE_WAP


////////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////////

#ifdef LOG_ENABLE
////////////////////////////////////////////////////////////////////////////////////
// Message buffers for CLNTa_log()
////////////////////////////////////////////////////////////////////////////////////
#define DM_LINE_SIZE     38
#define DEBUG_BUF_SIZE   50
#define NUM_OF_BUFFER    100
#define TEMP_BUF_SIZE    1000
#define MSG_BUF_SIZE     10000

char debug[NUM_OF_BUFFER][DEBUG_BUF_SIZE];
char temp[TEMP_BUF_SIZE];
char logbuf[MSG_BUF_SIZE];
char msg[MSG_BUF_SIZE];
#endif //#ifdef LOG_ENABLE

char *msg_ptr;
////////////////////////////////////////////////////////////////////////////////////

int	maxLength;
int	maxCount;

char	errorContent[50];
char	*cszErrorCode;
int		errornum;
int		errorCode;
int		errorResult;
extern	UINT8 ui_errortype;

#ifdef DEBUG_CLNT
static	int s_errorCount = 0;
#endif //#ifdef DEBUG_CLNT

#ifdef FEATURE_MUSIC_BELL
	char mResult[MAX_SIZE_MUSIC_RESULT]; //10 byte
#endif //FEATURE_MUSIC_BELL


#ifdef ABORT_IN_CONTENT
extern boolean isStringResponse;
#endif//ABORT_IN_CONTENT

///////////////////////////////////////////////////////////////////////////////////////////////////
// Contents downloaded from network is here 
///////////////////////////////////////////////////////////////////////////////////////////////////

int contentLength = 0;
char contentBuffer[MAX_CONTENT_SIZE]={0};

BOOL g_bGettingContent = FALSE;//init for Picture Mate/Music Bell
BOOL g_bSavingContent = FALSE;//init for Saving Picture Mate/Music Bell


///////////////////////////////////////////////////////////////////////////////////////////////////
// Contents getting method by decoding wml vendor type 
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef BLOCK
	/* clnt.h에 있는 content type과 동일한 순서로 할 것 */
	char vendorType[MAX_VND_NUM][MAX_VND_SIZE]=
	{	{"vnd.sky.wbmp"}, {"vnd.sky.animation"}, {"vnd.sky.security"}, {"vnd.sky.mybell"}, {NULL},
		{NULL}, {NULL}, {NULL}, {NULL}, {NULL}	};


	/* URL list to be downloaded according to the "vnd.*" in wml */
	char mybellTable[MAX_NUM_MYBELL][MAX_SIZE_URL]=					
	{	{"http://dev.n-top.com:9090/swjung/mybell/goodlife.txt"},
		{"http://dev.n-top.com:9090/swjung/mybell/semaul.gif"}	};
#endif //BLOCK
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// External global variables
///////////////////////////////////////////////////////////////////////////////////////////////////
extern void WAP_Repaint(void);
extern int Process_DownloadedImage(char *data, int length, char *url, char * title, int useImage, int imageType);
extern int Process_DownloadedCharacbell(char *Data, int Length, 
						char *URL, char *cbelltitle, 
						char *imagetitle, char *melodytitle,
                        char *callernum1, char *callernum2, char *callernum3);
extern int typeOfContent;
extern setImageType imageInfo;

#ifdef FEATURE_CHARACTER_BELL
extern setCharacBellType  g_setCharacBellInfo;
#endif // FEATURE_CHARACTER_BELL

BOOL g_fCurrentSate_WAPEXIT = FALSE;

#ifdef FOR_MS
	extern boolean bStopKey;
#endif

#ifdef FEATURE_MUSIC_BELL
	extern DownMusicbellType   MusicbellInfo;
	extern void Musicbell_play(char *Data, int Length, char *URL, char *Title,  char *AudioType, char *PlayType, char *Result);
	extern void Musicbell_download(char *Data, int Length, char *URL, char *Title, char *AudioType, char* Attribute, char *Result);
#endif //FEATURE_MUSIC_BELL

extern const ExtraLibInfo	arrayExtraScriptLib[3][20];
extern void CancelAnimationTimer(void);
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// AUS WAP adapter functions
///////////////////////////////////////////////////////////////////////////////////////////////////

VOID    CLNTa_nonSupportedScheme (UINT8 viewId, const CHAR *URL)
{
#ifdef DEBUG_CLNT
//    CLNTa_log(viewId, 0, "CLNTa_nonSupportedScheme\nconst CHAR * \t URL \t %s\n", URL);
#endif //#ifdef DEBUG_CLNT
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// CLNTa_content()
//		Error handling routine should be added!
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID    CLNTa_content (UINT8 urlID, const CHAR *data, UINT16 length, BOOL moreData, const CHAR *contentType, UINT32 totalSize, INT16 errorNo)
{	

	int result  = 0;
	//===============================================================
	// Download된 이미지를 단말기의 Animation Data로 저장한다
	//===============================================================
	
	contentLength = length;
	CLNTa_log(1, 0, "CLNTa_content is called! dataLength[%d]bytes\n",length);

	if(data == NULL)
	{
		CLNTa_log(1,0,"data = NULL");
		typeOfContent = CONTENT_TYPE_INVALID;

        if(g_Responsetype == TYPE_STRING)
        {
            clnt_make_string_response(urlID, NULL);
        }
        else if(g_Responsetype == TYPE_INTEGER) 
        {
            clnt_make_integer_response(urlID, 1004);
        }			
		CLNTa_error(1, errorNo, ERRTYPE_INFORMATION);
		return;
	}
	
	if ((length <= MAX_CONTENT_SIZE) && (length > 0))
	{
		memset(contentBuffer, '\0', MAX_CONTENT_SIZE);
		memcpy(contentBuffer, data, length);
	} 
	else
	{

		CLNTa_log(1, 0, "[CLNTa_content]Data size is too big:[%d]bytes\n",length);

		//Content를 받는 것을 실패
		g_bGettingContent = FALSE;//실패

		//Initialize typeOfContent
		{
			typeOfContent = CONTENT_TYPE_INVALID;

            if(g_Responsetype == TYPE_STRING)
            {
                clnt_make_string_response(urlID, NULL);
            }
            else if(g_Responsetype == TYPE_INTEGER) 
            {
                clnt_make_integer_response(urlID, 1004);
            }
			return;
		}
	}
	
	g_bSavingContent = TRUE;//저장시작

	// To avoid small buffer exhaustion, 
	// set the UI task priority as bellow the one of PS Task
	// Possible Side Effect is large buffer exhaustion
	// because UI cannot get a data from DS Task 
	// and then incoming download data may bring large buffer exhaustion.
	// jmpark 010921
    rex_set_pri(PS_PRI-1);


	switch(typeOfContent)
	{
		case IMAGE_BMP:
		case IMAGE_WBMP:
		case IMAGE_ANIMATION:
		{
			CLNTa_log(1, 0, "\nIMAGE! IMAGE! IMAGE! IMAGE! OK OK OK!!\n");

			result = Process_DownloadedImage(contentBuffer, length,
												imageInfo.url, imageInfo.title,
												imageInfo.kindOfImage, imageInfo.imageType);

			MSG_HIGH("Result of download [%d]",result,0,0);

			clnt_make_integer_response(urlID, result);

			//----------------------------------
			//Content를 모두 받았다
			g_bGettingContent = FALSE;//성공
			g_bSavingContent = FALSE;//저장완료
			//----------------------------------

			//저장후, backlight off - yjahn96 01.11.12
			SKY_SET_NO_OFF_BACKLIGHT(FALSE);    
			SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

			//--------------------------------------------------------
			// Animation Stop / repaint screen
			//--------------------------------------------------------
			// 이미지 저장중 End키로 wap종료시 화면 문제 해결
			if(g_fCurrentSate_WAPEXIT != TRUE)	//yjahn96 01.05.04
			{
				g_bDrawBar=FALSE; //yjahn96 01.11.05 for command line
				WAP_Repaint();
			}
		    // yjahn96 01.04.12 --------------------------------------
			// 저장후, 이미지가 있는 화면에서 Animation timer stop 
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation
			break;
		}

#ifdef FEATURE_MUSIC_BELL
		case MUSICBELL_TYPE_PLAY:
		case MUSICBELL_TYPE_DOWN:

			if (typeOfContent == MUSICBELL_TYPE_PLAY)
			{
				CLNTa_log(1,0,"CLNTa_content] Musicbell_play is called!!");
				Musicbell_play(contentBuffer, length,
							MusicbellInfo.URL, MusicbellInfo.Title,
							MusicbellInfo.AudioType, MusicbellInfo.PlayType,
							mResult);
			}
			else if (typeOfContent == MUSICBELL_TYPE_DOWN)
			{
				CLNTa_log(1,0,"CLNTa_content] Musicbell_download is called!![%x]", mResult);			
				Musicbell_download(contentBuffer, length,
							MusicbellInfo.URL, MusicbellInfo.Title,
							MusicbellInfo.AudioType,MusicbellInfo.Attribute, mResult);

				clnt_make_string_response(urlID, mResult);
			}
			//----------------------------------
			//Content를 모두 받았다
			g_bGettingContent = FALSE;//성공
			g_bSavingContent = FALSE;//저장완료
			//----------------------------------

			//--------------------------------------------------------
			// 뮤직벨 연주준비중 / 저장중 End키로 wap종료시 화면 문제 해결
			if(g_fCurrentSate_WAPEXIT != TRUE)	//yjahn96 01.05.04
			{
				g_bDrawBar=FALSE; //yjahn96 01.11.05 for command line
				WAP_Repaint();
			}
			break;

#endif //FEATURE_MUSIC_BELL

#ifdef FEATURE_CHARACTER_BELL
        case CHARACBELL_TYPE_DOWN:
        {
			CLNTa_log(1, 0, "\nIMAGE! IMAGE! IMAGE! IMAGE! OK OK OK!!\n");


			clnt_make_integer_response(urlID,	Process_DownloadedCharacbell(contentBuffer, length,
												g_setCharacBellInfo.URL, g_setCharacBellInfo.cbelltitle,
												g_setCharacBellInfo.imagetitle, g_setCharacBellInfo.melodytitle,
                                                g_setCharacBellInfo.callernum1,g_setCharacBellInfo.callernum2,
                                                g_setCharacBellInfo.callernum3));
            //TMP Dummy!!
            //clnt_make_integer_response(urlID,1);


			//----------------------------------
			//Content를 모두 받았다
			g_bGettingContent = FALSE;//성공
			g_bSavingContent = FALSE;//저장완료
			//----------------------------------

			//저장후, backlight off - yjahn96 01.11.12
			SKY_SET_NO_OFF_BACKLIGHT(FALSE);    
			SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

			//--------------------------------------------------------
			// Animation Stop / repaint screen
			//--------------------------------------------------------
			// 이미지 저장중 End키로 wap종료시 화면 문제 해결
			if(g_fCurrentSate_WAPEXIT != TRUE)	//yjahn96 01.05.04
			{
				g_bDrawBar=FALSE; //yjahn96 01.11.05 for command line
				WAP_Repaint();
			}

		    // yjahn96 01.04.12 --------------------------------------
			// 저장후, 이미지가 있는 화면에서 Animation timer stop 
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation
			break;
		}
#endif // FEATURE_CHARACTER_BELL

		case MYBELL:break;
		default:
			break;
	}
#ifdef DEBUG_CLNT
    CLNTa_log(1, 0, "CLNTa_content\nUINT8 \t urlID \t %u\nconst CHAR * \t confirmId \t %s\nUINT16 \t length \t %u\nCHAR * \t contentType \t %s\nINT16 \t errorNo \t %i\n", (int)urlID, data, (int)length, contentType, (int)errorNo);
	CLNTa_log(1, 0, "[%s]\n",contentBuffer);
#endif //#ifdef DEBUG_CLNT

	rex_set_pri(UI_PRI);

	//----------------------------------
	//Content를 모두 받았다
	g_bGettingContent = FALSE;//성공
	g_bSavingContent = FALSE;//저장완료
	//----------------------------------

	//Initialize typeOfContent
	typeOfContent = CONTENT_TYPE_INVALID;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLNTa_error()
///////////////////////////////////////////////////////////////////////////////////////////////////
VOID    CLNTa_error (UINT8 viewId, INT16 errorNo, UINT8 errorType)
{
    ui_errortype = errorType;
	errornum = errorNo;
	
	if (ERR_WTP_ABORT_START<= errornum &&
        errornum <= ERR_WTP_ABORT_END)
    {
		switch (errornum)
        {
#ifdef BLOCK
            case ERR_WTP_ABORT_START:
                cszErrorCode = "ERR_WTP_ABORT_START";
                break;
#endif //#ifdef BLOCK
                
                case ERR_WTP_UNKNOWN:
                    cszErrorCode = "ERR_WTP_UNKNOWN";
                    break;

                case ERR_WTP_PROTOERR:
                    cszErrorCode = "ERR_WTP_PROTOERR";
                    break;
                    
                case ERR_WTP_INVALIDTID:
                    cszErrorCode = "ERR_WTP_INVALIDTID";
                    break;
                
                case ERR_WTP_NOTIMPLEMENTEDCL2:
                    cszErrorCode = "ERR_WTP_NOTIMPLEMENTEDCL2";
                    break;
                
                case ERR_WTP_NOTIMPLEMENTEDSAR:
                    cszErrorCode = "ERR_WTP_NOTIMPLEMENTEDSAR";
                    break;
                
                case ERR_WTP_NOTIMPLEMENTEDUACK:
                    cszErrorCode = "ERR_WTP_NOTIMPLEMENTEDUACK";
                    break;
                
                case ERR_WTP_WTPVERSIONONE:
                    cszErrorCode = "ERR_WTP_WTPVERSIONONE";
                    break;
                
                case ERR_WTP_CAPTEMPEXCEEDED:
                    cszErrorCode = "ERR_WTP_CAPTEMPEXCEEDED";
                    break;
                
                case ERR_WTP_NORESPONSE:
                    cszErrorCode = "ERR_WTP_NORESPONSE";
                    break;
                
                case ERR_WTP_MESSAGETOOLARGE:
                    cszErrorCode = "ERR_WTP_MESSAGETOOLARGE";
                    break;
                
            case ERR_WTP_ABORT_END:
                cszErrorCode = "ERR_WTP_ABORT_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;          
         }
    }
    else if(ERR_HTTP_START<= errornum &&
        errornum <= ERR_HTTP_END)
    {
        switch (errornum)
        {
            case ERR_HTTP_START:
                cszErrorCode = "ERR_HTTP_START";
                break;
            
                case HTTPContinue:
                    cszErrorCode = "HTTPContinue";
                    break;
                
                case HTTPSwitchingProtocols:
                    cszErrorCode = "HTTPSwitchingProtocols";
                    break;
                
                case HTTPOKSuccess:
                    cszErrorCode = "HTTPOKSuccess";
                    break;
                
                case HTTPCreated:
                    cszErrorCode = "HTTPCreated";
                    break;
                
                case HTTPAccepted:
                    cszErrorCode = "HTTPAccepted";
                    break;
                
                case HTTPNonAuthoritativeInformation:
                    cszErrorCode = "HTTPNonAuthoritativeInformation";
                    break;
                
                case HTTPNoContent:
                    cszErrorCode = "HTTPNoContent";
                    break;
                
                case HTTPResetContent:
                    cszErrorCode = "HTTPResetContent";
                    break;
                
                case HTTPPartialContent:
                    cszErrorCode = "HTTPPartialContent";
                    break;
                
                case HTTPMultipleChoices:
                    cszErrorCode = "HTTPMultipleChoices";
                    break;
                
                case HTTPMovedPermanently:
                    cszErrorCode = "HTTPMovedPermanently";
                    break;
                
                case HTTPMovedtemporarily:
                    cszErrorCode = "HTTPMovedtemporarily";
                    break;
                
                case HTTPSeeOther:
                    cszErrorCode = "HTTPSeeOther";
                    break;
                
                case HTTPNotmodified:
                    cszErrorCode = "HTTPNotmodified";
                    break;
                
                case HTTPUseProxy:
                    cszErrorCode = "HTTPUseProxy";
                    break;
                
                case HTTPBadRequest:
                    cszErrorCode = "HTTPBadRequest";
                    break;
                
                case HTTPUnauthorized:
                    cszErrorCode = "HTTPUnauthorized";
                    break;
                
                case HTTPPaymentRequired:
                    cszErrorCode = "HTTPPaymentRequired";
                    break;
                
                case HTTPForbidden:
                    cszErrorCode = "HTTPForbidden";
                    break;
                
                case HTTPFileNotFound:
                    cszErrorCode = "HTTPFileNotFound";
                    break;
                
                case HTTPMethodNotAllowed:
                    cszErrorCode = "HTTPMethodNotAllowed";
                    break;
                
                case HTTPNotAcceptable:
                    cszErrorCode = "HTTPNotAcceptable";
                    break;
                
                case HTTPProxyAuthenticationRequired:
                    cszErrorCode = "HTTPProxyAuthenticationRequired";
                    break;
                
                case HTTPRequestTimeout:
                    cszErrorCode = "HTTPRequestTimeout";
                    break;
                
                case HTTPConflict:
                    cszErrorCode = "HTTPConflict";
                    break;
                
                case HTTPGone:
                    cszErrorCode = "HTTPGone";
                    break;
                
                case HTTPLengthRequired:
                    cszErrorCode = "HTTPLengthRequired";
                    break;
                
                case HTTPPreconditionFailed:
                    cszErrorCode = "HTTPPreconditionFailed";
                    break;
                
                case HTTPRequestedEntityTooLarge:
                    cszErrorCode = "HTTPRequestedEntityTooLarge";
                    break;
                
                case HTTPRequestURITooLarge:
                    cszErrorCode = "HTTPRequestURITooLarge";
                    break;
                
                case HTTPUnsupportedMediaType:
                    cszErrorCode = "HTTPUnsupportedMediaType";
                    break;
                
                case HTTPInternalServerError:
                    cszErrorCode = "HTTPInternalServerError";
                    break;
                
                case HTTPNotImplemented:
                    cszErrorCode = "HTTPNotImplemented";
                    break;
                
                case HTTPBadGateway:
                    cszErrorCode = "HTTPBadGateway";
                    break;
                
                case HTTPServiceUnavailable:
                    cszErrorCode = "HTTPServiceUnavailable";
                    break;
                
                case HTTPGatewayTimeout:
                    cszErrorCode = "HTTPGatewayTimeout";
                    break;
                
                case HTTPVerNotSupported:
                    cszErrorCode = "HTTPVerNotSupported";
                    break;
                
            case ERR_HTTP_END:
                cszErrorCode = "ERR_HTTP_END";
                break;
                
            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
         }
     }
     else if(ERR_WSP_ABORT_START<= errornum &&
        errornum <= ERR_WSP_ABORT_END)
     {
        switch (errornum)
        {
            case ERR_WSP_ABORT_START:
                cszErrorCode = "ERR_WSP_ABORT_START";
                break;

                case ERR_WSP_PROTOERR:
                    cszErrorCode = "ERR_WSP_PROTOERR";
                    break;

                case ERR_WSP_DISCONNECT:
                    cszErrorCode = "ERR_WSP_DISCONNECT";
                    break;

                case ERR_WSP_SUSPEND:
                    cszErrorCode = "ERR_WSP_SUSPEND";
                    break;

                case ERR_WSP_RESUME:
                    cszErrorCode = "ERR_WSP_RESUME";
                    break;

                case ERR_WSP_CONGESTION:
                    cszErrorCode = "ERR_WSP_CONGESTION";
                    break;

                case ERR_WSP_CONNECTERR:
                    cszErrorCode = "ERR_WSP_CONNECTERR";
                    break;

                case ERR_WSP_MRUEXCEEDED:
                    cszErrorCode = "ERR_WSP_MRUEXCEEDED";
                    break;

                case ERR_WSP_MOREXCEEDED:
                    cszErrorCode = "ERR_WSP_MOREXCEEDED";
                    break;

                case ERR_WSP_PEERREQ:
                    cszErrorCode = "ERR_WSP_PEERREQ";
                    break;

                case ERR_WSP_NETERR:
                    cszErrorCode = "ERR_WSP_NETERR";
                    break;

                case ERR_WSP_USERREQ:
                    cszErrorCode = "ERR_WSP_USERREQ";
                    break;

            case ERR_WSP_ABORT_END:
                cszErrorCode = "ERR_WSP_ABORT_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
        }
     }
     else if(ERR_WAE_START<= errornum &&
        errornum <= ERR_WAE_END)
     {
        switch (errornum)
        {
            case ERR_WAE_START:
                cszErrorCode = "ERR_WAE_START";
                break;

                case ERR_WAE_OUT_OF_MEMORY:
                    cszErrorCode = "ERR_WAE_OUT_OF_MEMORY";
                    break;

                case ERR_WAE_UA_START:
                    cszErrorCode = "ERR_WAE_UA_START";
                    break;

                case ERR_WAE_UA_VIEWID_INVALID:
                    cszErrorCode = "ERR_WAE_UA_VIEWID_INVALID";
                    break;

                case ERR_WAE_UA_MAX_EXCEEDED:
                    cszErrorCode = "ERR_WAE_UA_MAX_EXCEEDED";
                    break;

                case ERR_WAE_UA_PARSE:
                    cszErrorCode = "ERR_WAE_UA_PARSE";
                    break;

                case ERR_WAE_UA_DISPLAY_ERROR:
                    cszErrorCode = "ERR_WAE_UA_DISPLAY_ERROR";
                    break;

                case ERR_WAE_UA_RESPONSE_BODY_INVALID:
                    cszErrorCode = "ERR_WAE_UA_RESPONSE_BODY_INVALID";
                    break;

                case ERR_WAE_UA_URL_INVALID:
                    cszErrorCode = "ERR_WAE_UA_URL_INVALID";
                    break;

                case ERR_WAE_UA_URL_TIMEOUT:
                    cszErrorCode = "ERR_WAE_UA_URL_TIMEOUT";
                    break;

                case ERR_WAE_UA_WSP_RESPONSE_INVALID:
                    cszErrorCode = "ERR_WAE_UA_WSP_RESPONSE_INVALID";
                    break;

                case ERR_WAE_UA_WMLDECK_ACCESS_DENIED:
                    cszErrorCode = "ERR_WAE_UA_WMLDECK_ACCESS_DENIED";
                    break;

                case ERR_WAE_UA_URL_NONSUPPORTED_SCHEME:
                    cszErrorCode = "ERR_WAE_UA_URL_NONSUPPORTED_SCHEME";
                    break;

                case ERR_WAE_UA_REDIRECT_ERROR:
                    cszErrorCode = "ERR_WAE_UA_REDIRECT_ERROR";
                    break;

                case ERR_WAE_UA_SESSION_NOT_CONNECTED:
                    cszErrorCode = "ERR_WAE_UA_SESSION_NOT_CONNECTED";
                    break;

                case ERR_WAE_UA_MAX_NR_OF_SESSIONS_REACHED:
                    cszErrorCode = "ERR_WAE_UA_MAX_NR_OF_SESSIONS_REACHED";
                    break;

                case ERR_WAE_UA_INVALID_STACKMODE:
                    cszErrorCode = "ERR_WAE_UA_INVALID_STACKMODE";
                    break;

                case ERR_WAE_UA_WTA_ACCESS_DENIED:
                    cszErrorCode = "ERR_WAE_UA_WTA_ACCESS_DENIED";
                    break;

                case ERR_WAE_UA_TOO_LARGE_DATA_TRANSFER:
                    cszErrorCode = "ERR_WAE_UA_TOO_LARGE_DATA_TRANSFER";
                    break;

                case ERR_WAE_UA_LARGE_DATA_TRANSFER_DISABLED:
                    cszErrorCode = "ERR_WAE_UA_LARGE_DATA_TRANSFER_DISABLED";
                    break;

                case ERR_WAE_UA_ILLEGAL_ENCTYPE:
                    cszErrorCode = "ERR_WAE_UA_ILLEGAL_ENCTYPE";
                    break;
            
                case ERR_WAE_UA_END:
                    cszErrorCode = "ERR_WAE_UA_END";
                    break;

                case ERR_WAE_WML_START:
                    cszErrorCode = "ERR_WAE_WML_START";
                    break;

                case ERR_WAE_WML_INSTREAM_FAULT:
                    cszErrorCode = "ERR_WAE_WML_INSTREAM_FAULT";
                    break;

                case ERR_WAE_WML_CONTENT_CHARSET_ERROR:
                    cszErrorCode = "ERR_WAE_WML_CONTENT_CHARSET_ERROR";
                    break;

                case ERR_WAE_WML_CONTENT_CHARSET_NOT_SUPPORTED:
                    cszErrorCode = "ERR_WAE_WML_CONTENT_CHARSET_NOT_SUPPORTED";
                    break;

                case ERR_WAE_WML_UNKNOWN_TOKEN:
                    cszErrorCode = "ERR_WAE_WML_UNKNOWN_TOKEN";
                    break;

                case ERR_WAE_WML_WML_ERROR:
                    cszErrorCode = "ERR_WAE_WML_WML_ERROR";
                    break;

                case ERR_WAE_WBXML_CONTENT_VERSION_WARNING:
                    cszErrorCode = "ERR_WAE_WBXML_CONTENT_VERSION_WARNING";
                    break;

                case ERR_WAE_WML_CONTENT_PUBLIC_ID_WARNING:
                    cszErrorCode = "ERR_WAE_WML_CONTENT_PUBLIC_ID_WARNING";
                    break;

                case ERR_WAE_WBXML_CONTENT_PUBLIC_ID_ERROR:
                    cszErrorCode = "ERR_WAE_WBXML_CONTENT_PUBLIC_ID_ERROR";
                    break;

                case ERR_WAE_WML_END:
                    cszErrorCode = "ERR_WAE_WML_END";
                    break;

                case ERR_WAE_WMLS_START:
                    cszErrorCode = "ERR_WAE_WMLS_START";
                    break;

                case ERR_WAE_WMLS_NONE:
                    cszErrorCode = "ERR_WAE_WMLS_NONE";
                    break;

                case ERR_WAE_WMLS_VERIFICATION:
                    cszErrorCode = "ERR_WAE_WMLS_VERIFICATION";
                    break;

                case ERR_WAE_WMLS_LIB:
                    cszErrorCode = "ERR_WAE_WMLS_LIB";
                    break;

                case ERR_WAE_WMLS_FUNC_ARGS:
                    cszErrorCode = "ERR_WAE_WMLS_FUNC_ARGS";
                    break;

                case ERR_WAE_WMLS_EXTERNAL:
                    cszErrorCode = "ERR_WAE_WMLS_EXTERNAL";
                    break;

                case ERR_WAE_WMLS_LOAD:
                    cszErrorCode = "ERR_WAE_WMLS_LOAD";
                    break;

                case ERR_WAE_WMLS_ACCESS:
                    cszErrorCode = "ERR_WAE_WMLS_ACCESS";
                    break;

                case ERR_WAE_WMLS_STACK_UNDERFLOW:
                    cszErrorCode = "ERR_WAE_WMLS_STACK_UNDERFLOW";
                    break;

                case ERR_WAE_WMLS_ABORT:
                    cszErrorCode = "ERR_WAE_WMLS_ABORT";
                    break;

                case ERR_WAE_WMLS_STACK_OVRFLW:
                    cszErrorCode = "ERR_WAE_WMLS_STACK_OVRFLW";
                    break;

                case ERR_WAE_WMLS_USER_ABORT:
                    cszErrorCode = "ERR_WAE_WMLS_USER_ABORT";
                    break;

                case ERR_WAE_WMLS_SYSTEM_ABORT:
                    cszErrorCode = "ERR_WAE_WMLS_SYSTEM_ABORT";
                    break;

                case ERR_WAE_WMLS_NULL:
                    cszErrorCode = "ERR_WAE_WMLS_NULL";
                    break;

                case ERR_WAE_WMLS_END:
                    cszErrorCode = "ERR_WAE_WMLS_END";
                    break;

                case ERR_WAE_REP_START:
                    cszErrorCode = "ERR_WAE_REP_START";
                    break;

                case ERR_WAE_REP_SERVICE_INSTALL_FAILED:
                    cszErrorCode = "ERR_WAE_REP_SERVICE_INSTALL_FAILED";
                    break;

                case ERR_WAE_REP_MEM_ACCESS_FAILED:
                    cszErrorCode = "ERR_WAE_REP_MEM_ACCESS_FAILED";
                    break;

                case ERR_WAE_REP_SERVICE_UNLOAD_FAILED:
                    cszErrorCode = "ERR_WAE_REP_SERVICE_UNLOAD_FAILED";
                    break;

                case ERR_WAE_REP_END:
                    cszErrorCode = "ERR_WAE_REP_END";
                    break;

                case ERR_WAE_PUSH_START:
                    cszErrorCode = "ERR_WAE_PUSH_START";
                    break;

                case ERR_WAE_PUSH_ACTIVATE_FAILED:
                    cszErrorCode = "ERR_WAE_PUSH_ACTIVATE_FAILED";
                    break;

                case ERR_WAE_PUSH_DELETE_FAILED:
                    cszErrorCode = "ERR_WAE_PUSH_DELETE_FAILED";
                    break;

                case ERR_WAE_PUSH_STORE_FAULT:
                    cszErrorCode = "ERR_WAE_PUSH_STORE_FAULT";
                    break;

                case ERR_WAE_PUSH_END:
                    cszErrorCode = "ERR_WAE_PUSH_END";
                    break;

                case ERR_WAE_SKT_SECURITY_START:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_START";
                    break;

                case ERR_WAE_SKT_SECURITY_CLIENTHELLO:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_START";
                    break;

                case ERR_WAE_SKT_SECURITY_SERVERHELLO:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_SERVERHELLO";
                    break;

                case ERR_WAE_SKT_SECURITY_KEYEXCHANGE:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_KEYEXCHANGE";
                    break;

                case ERR_WAE_SKT_SECURITY_FINISHED:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_FINISHED";
                    break;

                case ERR_WAE_SKT_SECURITY_ENCRYPT:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_ENCRYPT";
                    break;

                case ERR_WAE_SKT_SECURITY_DECRYPT:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_DECRYPT";
                    break;

                case ERR_WAE_SKT_SECURITY_TIMEOUT:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_TIMEOUT";
                    break;

                case ERR_WAE_SKT_SECURITY_END:
                    cszErrorCode = "ERR_WAE_SKT_SECURITY_END";
                    break;
       
            case ERR_WAE_END:
                cszErrorCode = "ERR_WAE_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
          }
     }
     else if(ERR_WSPCL_START<= errornum &&
     errornum <= ERR_WSPCL_END)
     {
        switch (errornum)
        {
            case ERR_WSPCL_START:
                cszErrorCode = "ERR_WSPCL_START";
                break;

                case ERR_WSPCL_ErrorInAddressFromWAE:
                    cszErrorCode = "ERR_WSPCL_ErrorInAddressFromWAE";
                    break;

                case ERR_WSPCL_ErrorExtractReplyPDUFailed:
                    cszErrorCode = "ERR_WSPCL_ErrorExtractReplyPDUFailed";
                    break;

                case ERR_WSPCL_ErrorNoBuffersAvailable:
                    cszErrorCode = "ERR_WSPCL_ErrorNoBuffersAvailable";
                    break;

                case ERR_WSPCL_ErrorMethodNotSupported:
                    cszErrorCode = "ERR_WSPCL_ErrorMethodNotSupported";
                    break;

                case ERR_WSPCL_ErrorExtractPushPDUFailed:
                    cszErrorCode = "ERR_WSPCL_ErrorExtractPushPDUFailed";
                    break;

                case ERR_WSPCL_ErrorErroneousPDUTypeReceived:
                    cszErrorCode = "ERR_WSPCL_ErrorErroneousPDUTypeReceived";
                    break;

                case ERR_WSPCL_ErrorErroneousStackConfigReceived:
                    cszErrorCode = "ERR_WSPCL_ErrorErroneousStackConfigReceived";
                    break;

            case ERR_WSPCL_END:
                cszErrorCode = "ERR_WSPCL_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
        }
     }
     else if(ERR_WSPCM_START<= errornum &&
     errornum <= ERR_WSPCM_END)
     {
        switch (errornum)
        {
            case ERR_WSPCM_START:
                cszErrorCode = "ERR_WSPCM_START";
                break;

                case ERR_WSPCM_ErrorNoMemoryForPDUPacking:
                    cszErrorCode = "ERR_WSPCM_ErrorNoMemoryForPDUPacking";
                    break;

                case ERR_WSPCM_ErrorNoMemoryForPDUUnPacking:
                    cszErrorCode = "ERR_WSPCM_ErrorNoMemoryForPDUUnPacking";
                    break;

                case ERR_WSPCM_ErrorInDataFromWAE:
                    cszErrorCode = "ERR_WSPCM_ErrorInDataFromWAE";
                    break;

                case ERR_WSPCM_ErrorInReplyFromServer:
                    cszErrorCode = "ERR_WSPCM_ErrorInReplyFromServer";
                    break;

                case ERR_WSPCM_ErrorMaxSessionsAlreadyReached:
                    cszErrorCode = "ERR_WSPCM_ErrorMaxSessionsAlreadyReached";
                    break;

                case ERR_WSPCM_ErrorMOMAlreadyReached:
                    cszErrorCode = "ERR_WSPCM_ErrorMOMAlreadyReached";
                    break;

                case ERR_WSPCM_WAEErrNoPIdMatchingSession:
                    cszErrorCode = "ERR_WSPCM_WAEErrNoPIdMatchingSession";
                    break;

                case ERR_WSPCM_ErrorNoPIdMatchingMethod:
                    cszErrorCode = "ERR_WSPCM_ErrorNoPIdMatchingMethod";
                    break;

                case ERR_WSPCM_ErrorNoPIdMatchingPush:
                    cszErrorCode = "ERR_WSPCM_ErrorNoPIdMatchingPush";
                    break;

                case ERR_WSPCM_ErrorStoreOMInfoFailed:
                    cszErrorCode = "ERR_WSPCM_ErrorStoreOMInfoFailed";
                    break;

                case ERR_WSPCM_ErrorStoreHandleFailed:
                    cszErrorCode = "ERR_WSPCM_ErrorStoreHandleFailed";
                    break;

                case ERR_WSPCM_ErrorMethodNotSupported:
                    cszErrorCode = "ERR_WSPCM_ErrorMethodNotSupported";
                    break;

                case ERR_WSPCM_ErrorSameAQUsed:
                    cszErrorCode = "ERR_WSPCM_ErrorSameAQUsed";
                    break;

                case ERR_WSPCM_WAEErrNoPIdMatchingMethod:
                    cszErrorCode = "ERR_WSPCM_WAEErrNoPIdMatchingMethod";
                    break;

                case ERR_WSPCM_ErrorServerMRUisReached:
                    cszErrorCode = "ERR_WSPCM_ErrorServerMRUisReached";
                    break;

            case ERR_WSPCM_END:
                cszErrorCode = "ERR_WSPCM_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
        }
     }
     else if(ERR_WTP_START<= errornum &&
     errornum <= ERR_WTP_END)
     {
         switch (errornum)
         {
            case ERR_WTP_START:
                cszErrorCode = "ERR_WTP_START";
                break;

                case ERR_WTP_ErrorNORESPONSE:
                    cszErrorCode = "ERR_WTP_ErrorNORESPONSE";
                    break;

                case ERR_WTP_ErrorNOFREEBUFF:
                    cszErrorCode = "ERR_WTP_ErrorNOFREEBUFF";
                    break;

                case ERR_WTP_ErrorINVALID_BEARER:
                    cszErrorCode = "ERR_WTP_ErrorINVALID_BEARER";
                    break;

                case ERR_WTP_ErrorINVALID_CLASS:
                    cszErrorCode = "ERR_WTP_ErrorINVALID_CLASS";
                    break;

                case ERR_WTP_ErrorINVALID_ACKTYPE:
                    cszErrorCode = "ERR_WTP_ErrorINVALID_ACKTYPE";
                    break;

            case ERR_WTP_END:
                cszErrorCode = "ERR_WTP_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
         }
     }
     else if(ERR_WTLS_START<= errornum &&
     errornum <= ERR_WTLS_END)
     {
         switch (errornum)
         {
            case ERR_WTLS_START:
                cszErrorCode = "ERR_WTLS_START";
                break;

                case ERR_WTLS_GENERAL_START:
                    cszErrorCode = "ERR_WTLS_GENERAL_START";
                    break;

                case ERR_WTLS_GENERAL_END:
                    cszErrorCode = "ERR_WTLS_GENERAL_END";
                    break;

                case ERR_WTLS_HANDSHAKE_FAILURE_START:
                    cszErrorCode = "ERR_WTLS_HANDSHAKE_FAILURE_START";
                    break;

                case ERR_WTLS_HANDSHAKE_FAILURE_END:
                    cszErrorCode = "ERR_WTLS_HANDSHAKE_FAILURE_END";
                    break;

                case ERR_WTLS_CRYPTLIB_START:
                    cszErrorCode = "ERR_WTLS_CRYPTLIB_START";
                    break;

                case ERR_WTLS_CRYPTLIB_END:
                    cszErrorCode = "ERR_WTLS_CRYPTLIB_END";
                    break;

                case ERR_WTLS_RECEIVED_ALERT_START:
                    cszErrorCode = "ERR_WTLS_RECEIVED_ALERT_START";
                    break;

                case ERR_WTLS_RECEIVED_ALERT_END:
                    cszErrorCode = "ERR_WTLS_RECEIVED_ALERT_END";
                    break;

                case ERR_WTLS_INTERNAL_START:
                    cszErrorCode = "ERR_WTLS_INTERNAL_START";
                    break;

                case ERR_WTLS_INTERNAL_END:
                    cszErrorCode = "ERR_WTLS_INTERNAL_END";
                    break;

            case ERR_WTLS_END:
                cszErrorCode = "ERR_WTLS_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
         }
     }
     else if(ERR_WDP_START<= errornum &&
     errornum <= ERR_WDP_END)
     {
         switch (errornum)
         {
            case ERR_WDP_START:
                cszErrorCode = "ERR_WDP_START";
                break;

                case ERR_WDP_ErrorInDatafromWSP:
                    cszErrorCode = "ERR_WDP_ErrorInDatafromWSP";
                    break;

                case ERR_WDP_ErrorInDatafromWTP:
                    cszErrorCode = "ERR_WDP_ErrorInDatafromWTP";
                    break;

                case ERR_WDP_ErrorBearerNotSupported:
                    cszErrorCode = "ERR_WDP_ErrorBearerNotSupported";
                    break;

                case ERR_WDP_WDErrorInd:
                    cszErrorCode = "ERR_WDP_WDErrorInd";
                    break;

                case ERR_WDP_UDPErrorInd:
                    cszErrorCode = "ERR_WDP_UDPErrorInd";
                    break;

                case ERR_WDP_UDPBigBuffer:
                    cszErrorCode = "ERR_WDP_UDPBigBuffer";
                    break;

                case ERR_WDP_SMSErrorInd:
                    cszErrorCode = "ERR_WDP_SMSErrorInd";
                    break;

                case ERR_WDP_SMSBigBuffer:
                    cszErrorCode = "ERR_WDP_SMSBigBuffer";
                    break;

                case ERR_WCMP_PortUnreachable:
                    cszErrorCode = "ERR_WCMP_PortUnreachable";
                    break;

                case ERR_WCMP_MessageTooBig:
                    cszErrorCode = "ERR_WCMP_MessageTooBig";
                    break;

            case ERR_WDP_END:
                cszErrorCode = "ERR_WDP_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
         }
     }
     else if(ERR_UDCP_START<= errornum &&
     errornum <= ERR_UDCP_END)
     {
         switch (errornum)
         {
            case ERR_UDCP_START:
                cszErrorCode = "ERR_UDCP_START";
                break;

                case ERR_UDCP_UNKNOWN:
                    cszErrorCode = "ERR_UDCP_UNKNOWN";
                    break;

                case ERR_UDCP_PROTOERR:
                    cszErrorCode = "ERR_UDCP_PROTOERR";
                    break;

                case ERR_UDCP_UDCPVERSIONZERO:
                    cszErrorCode = "ERR_UDCP_UDCPVERSIONZERO";
                    break;

                case ERR_UDCP_EXTADDRNOTSUPP:
                    cszErrorCode = "ERR_UDCP_EXTADDRNOTSUPP";
                    break;

                case ERR_UDCP_NETERROR:
                    cszErrorCode = "ERR_UDCP_NETERROR";
                    break;

                case ERR_UDCP_QUEUEFULL:
                    cszErrorCode = "ERR_UDCP_QUEUEFULL";
                    break;

                case REL_UDCP_UNKNOWN:
                    cszErrorCode = "REL_UDCP_UNKNOWN";
                    break;

                case REL_UDCP_UTIMEOUT:
                    cszErrorCode = "REL_UDCP_UTIMEOUT";
                    break;

                case REL_UDCP_UIDLE:
                    cszErrorCode = "REL_UDCP_UIDLE";
                    break;

                case REL_UDCP_USER:
                    cszErrorCode = "REL_UDCP_USER";
                    break;

                case REL_UDCP_NETRELEASE:
                    cszErrorCode = "REL_UDCP_NETRELEASE";
                    break;

            case ERR_UDCP_END:
                cszErrorCode = "ERR_UDCP_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
         }
     }
     else if(ERR_MEM_START<= errornum &&
     errornum <= ERR_MEM_END)
     {
         switch (errornum)
         {
            case ERR_MEM_START:
                cszErrorCode = "ERR_MEM_START";
                break;

                case ERR_MEMORY_WARNING:
                    cszErrorCode = "ERR_MEMORY_WARNING";
                    break;

                case ERR_OUT_OF_MEMORY:
                    cszErrorCode = "ERR_MEMORY_WARNING";
                    break;

            case ERR_MEM_END:
                cszErrorCode = "ERR_MEM_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
         }
     }
     else if(ERR_INTERNAL_START<= errornum &&
     errornum <= ERR_INTERNAL_END)
     {
         switch (errornum)
         {
            case ERR_INTERNAL_START:
                cszErrorCode = "ERR_INTERNAL_START";
                break;

            case ERR_WAE_UA_REQUEST_ABORTED:
                cszErrorCode = "ERR_WAE_UA_REQUEST_ABORTED";
                break;

            case ERR_WAE_UA_INVALID_SESSION_ID:
                cszErrorCode = "ERR_WAE_UA_INVALID_SESSION_ID";
                break;

            case ERR_WAE_WMLS_EXIT:
                cszErrorCode = "ERR_WAE_WMLS_EXIT";
                break;            

            case ERR_INTERNAL_END:
                cszErrorCode = "ERR_INTERNAL_END";
                break;

            default:
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
         }
     }
     else
     {
         cszErrorCode = "errcodes.h에 정의되지 않은 에러";
     }

#ifdef FEATURE_PIM_INVALIDERROR
    if(errornum == ERR_WAE_UA_RESPONSE_BODY_INVALID || errornum == ERR_WAE_UA_WSP_RESPONSE_INVALID )
    {           
        extern BOOL g_PIM;
        extern WAP_Card *g_pCard;
        if(g_PIM)
        {
            g_PIM = FALSE;
            MMIc_reload(g_pCard->viewId);
        }
        else
        {
            errorResult = wap_check_error_range(errornum);

		    if (errorResult != ERR_RESULT_IS_IGNORE)
		    {
			    wap_set_error(TRUE);
			    wap_handle_error(errornum, errorType);
		    }
        }
    }
    else        
#endif //FEATURE_PIM_INVALIDERROR
#ifdef FEATURE_WAP_ERROR_RELOAD
    if(errornum == HTTPFileNotFound || errornum == HTTPInternalServerError )
    {
        if(g_reloaderrorTime <= 2 && g_reloaderrorTime > 0 )
        {                
            MMIc_reload(g_pCard->viewId);
        }
        else
        {
            errorResult = wap_check_error_range(errornum);

		    if (errorResult != ERR_RESULT_IS_IGNORE)
		    {
			    wap_set_error(TRUE);
			    wap_handle_error(errornum, errorType);
		    }
            g_reloaderrorTime = 0;
        }
    }
    else            
#endif //FEATURE_WAP_ERROR_RELOAD
    {
	    errorResult = wap_check_error_range(errornum);

	    if (errorResult != ERR_RESULT_IS_IGNORE)
	    {
		    wap_set_error(TRUE);
		    wap_handle_error(errornum, errorType);
	    }
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//  CLNTa_log()
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL test_flag20 = 0;
BOOL test_flag21 = 0;

void    CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...)
{
#ifdef LOG_ENABLE

    const char  *cszLogCode;
    va_list		arg_ptr;

    static int	i=0;
	
	int			j=0;
	int			times, residue;
	
	int			m=0;
	int			n=0;
	
	int			right_offset;
	int			buf_size;
	
	msg_ptr=msg;

	memset(temp,'\0',TEMP_BUF_SIZE);
    memset(logbuf,'\0',MSG_BUF_SIZE);
	memset(msg,'\0',MSG_BUF_SIZE);

    if (logNo > log_Undefined)
    {
        switch (logNo)
        {
            case log_wae_start:
            {
                cszLogCode = "log_wae_start";
                break;
            }
      
        }
		sprintf(temp, "%s",format);
    }
    else
    {
		sprintf(temp, "%s",format);
    }

    va_start(arg_ptr, format);
    vsprintf(logbuf, temp, arg_ptr);
    va_end(arg_ptr);

	/*
	** Expand escape characters to according spaces
	*/
	buf_size=strlen(logbuf);

	if(buf_size >= MSG_BUF_SIZE)
	{
		MSG_ERROR("logbufLen [%d]",buf_size,0,0);
		ERR_FATAL("logbuf >= MSG_BUF_SIZE",0,0,0);
	}

	while(buf_size--)
	{   
		/* right_offset indicates the rest size of right part */
		right_offset = DM_LINE_SIZE - n%DM_LINE_SIZE;
        switch (logbuf[m]) 
		{

		case HORIZONTAL_TAB:
			if ( right_offset < HT_SIZE ) {
				memset(&msg[n], ' ', right_offset);
				n += right_offset;
			}
			else {
				memset(&msg[n], ' ', HT_SIZE);
				n += HT_SIZE;
			}
			break;
		case NEW_LINE:
		case CARRIAGE_RETURN:
			if ( right_offset < NL_SIZE ) {
				memset(&msg[n], ' ', right_offset);
				n += right_offset;
			}
			else {
				memset(&msg[n], ' ', NL_SIZE);
				n += NL_SIZE;
			}
			break;
		default:
            msg[n++] = logbuf[m];
			break;
		} // end of switch
		m++;
	} // end of while loop

	/*
	** Disassemble message into pieces by dm_line_size
	*/
	times=strlen(msg_ptr)/DM_LINE_SIZE;
	residue=strlen(msg_ptr)%DM_LINE_SIZE;

	while(times--) 
	{
		memset((char*)&debug[i][0],'\0',DEBUG_BUF_SIZE);

		strncpy((char*)&debug[i][0], &msg[j*DM_LINE_SIZE], DM_LINE_SIZE);
		
		debug_msg(debug[i],0,0,0);

		i = (i+1) % 100;
		j++;
	}

	if (residue) {

		memset((char*)&debug[i][0],'\0',DEBUG_BUF_SIZE);

		strncpy((char*)&debug[i][0], &msg[j*DM_LINE_SIZE], residue);

		debug_msg(debug[i],0,0,0);

		i = (i+1) % 100;
	}
#endif //#ifdef LOG_ENABLE
}






///////////////////////////////////////////////////////////////////////////////////////////////////
//  CLNTa_getFile()
///////////////////////////////////////////////////////////////////////////////////////////////////

void CLNTa_getFile (unsigned char fileId, const char *URL)
{
	extern void SKY_graphicsStart(void);
	extern void SKY_graphicsEnd(void);

	char scheme_file[] = "file:///";
	char point[] ="Graphispoint";
	char line[]	="Graphisline";
	char circle[] ="Circle";
	char rectangle[] ="Rectangle";
	char start[] ="SKY_graphicsStart";
	char end[] ="SKY_graphicsEnd";
	char function[20]   = {0};
	char *pFunction = function;
	char *pURL;
	
#ifdef DEBUG_CLNT
	CLNTa_log(1,0,"CLNTa_getFile called.......\n");
#endif //#ifdef DEBUG_CLNT

	pURL = (char *)URL;

	maxLength = strlen(pURL);
	
	if (wap_compare_name(pURL, scheme_file) == SUCCESS)

	{
#ifdef DEBUG_CLNT
		CLNTa_log(1,0,"wap_compare_name success .......\n");
#endif //#ifdef DEBUG_CLNT
		pURL += strlen(scheme_file);
		
		wap_get_item(pURL, pFunction);

		pURL += strlen(pFunction) + 1;

		if (wap_check_library(function, point) == SUCCESS)
		{
			wap_make_point(pURL);
#ifdef DEBUG_CLNT
			CLNTa_log(1,0,"Graph] Point is called........\n");
#endif //#ifdef DEBUG_CLNT
		}
		else if (wap_check_library(function, line)== SUCCESS)		
		{
			wap_make_line(pURL);
#ifdef DEBUG_CLNT
			CLNTa_log(1,0,"Graph] Line is called.........\n");
#endif //#ifdef DEBUG_CLNT
		}
		else if (wap_check_library(function, circle)== SUCCESS)	
		{
			wap_make_circle(pURL);
#ifdef DEBUG_CLNT
			CLNTa_log(1,0,"Graph] Circle is called.......\n");
#endif //#ifdef DEBUG_CLNT
		}
		else if (wap_check_library(function, rectangle)== SUCCESS)	
		{
#ifdef DEBUG_CLNT
			CLNTa_log(1,0,"Graph] Rectangle is called....\n");
#endif //#ifdef DEBUG_CLNT
			wap_make_rectangle(pURL);
		}	
		else if (wap_check_library(function, start)== SUCCESS)	
		{
			SKY_graphicsStart();
		}	
		else if (wap_check_library(function, end)== SUCCESS)	
		{
			SKY_graphicsEnd();
		}	
		else
		{
#ifdef DEBUG_CLNT
			CLNTa_log(1,0,"Graph] Undefined Function\n");
			CLNTa_log(1,0,"Graph] Function is %s\n",function);
#endif //#ifdef DEBUG_CLNT
			;
		}
	}

	else
	{
#ifdef DEBUG_CLNT
		CLNTa_log(1,0,"Graph] Undefined scheme\n");
#endif //#ifdef DEBUG_CLNT
		;
	}

	maxCount=0;
	maxLength=0;

	/* Call connector function */
#ifdef DEBUG_CLNT
	CLNTa_log(1,0,"Call CLNTc_file .......\n");
#endif //#ifdef DEBUG_CLNT
	CLNTc_file (fileId,0,0,0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLNTa_hasWMLSLibFunc
////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL    CLNTa_hasWMLSLibFunc (UINT16 libNbr, UINT8 funcNbr, UINT8 *nbrOfParams)
{
	ExtraLibInfo*	pExtraLibInfo = NULL;

	//CLNTa_log(1,0,"CLNTa_hasWMLSLibFunc] Is called");

	if(!clnt_check_lib_range(libNbr, funcNbr)) 
	{
		return FALSE;
	}

	clnt_adjust_index(&libNbr, &funcNbr);

	pExtraLibInfo = (ExtraLibInfo*) &( arrayExtraScriptLib[libNbr][funcNbr] );

	if(pExtraLibInfo->PFunc)
	{
		//CLNTa_log(1,0,"CLNTa_hasWMLSLibFunc] There is function");
		*nbrOfParams = pExtraLibInfo->nbrOfArgs;
		return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLNTa_callWMLSLibFunc
////////////////////////////////////////////////////////////////////////////////////////////////////////
WMLSvar* CLNTa_callWMLSLibFunc (UINT16 invokeId, UINT16 libNbr, UINT8 funcNbr, const WMLSvar * const *params, BOOL *pSeparateResponse)
{
	ExtraLibInfo*	pExtraLibInfo = NULL;

	//CLNTa_log(1,0,"CLNTa_callWMLSLibFunc] [LIB:%d][FUNC:%d]",libNbr, funcNbr);

	clnt_adjust_index(&libNbr, &funcNbr);

	pExtraLibInfo = (ExtraLibInfo*) &( arrayExtraScriptLib[libNbr][funcNbr] );

	//CLNTa_log(1,0,"CLNTa_callWMLSLibFunc] [%d][%d][%d]",libNbr, funcNbr, pExtraLibInfo->nbrOfArgs);
	
	return (pExtraLibInfo->PFunc)(invokeId, params, pSeparateResponse, pExtraLibInfo->nbrOfArgs);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions to be implemented
////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLNTa_callFunction
////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID    CLNTa_callFunction (UINT8 functionId, const CHAR *device, const CHAR *function, const CHAR *attributes)
{
	;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//#ifdef PORTING_B0_WAP
#define WAP_USE_START
#define REDIRECT
void suspend_wap_browser(void);
extern void wap_set_exit(void);
extern boolean browserTerComp;
extern redirect_config redirect;
clk_cb_type clk_timer;

#ifdef FEATURE_WAP
void    CLNTa_terminated (void)
{
	extern unsigned char *pShareRAM;
	extern BOOL WAP_Terminate_WML_Test(void);
	
	suspend_wap_browser();//두개의 순서를 바꾸어 보았음
	WAP_Terminate_WML_Test();
    CLNTa_log(1,0,"browser_init started");
	memset(pShareRAM,0x00,WIP_MALLOC_MEM_SIZE);	
	CLNTa_log(1,0,"-------[CLNTa_terminated]-------\n");
    EQS_SEND_EVENT(SE_WAP_COMPLETE, EQS_NA, EQS_NA);
}
#endif //FEATURE_WAP

void  suspend_wap_browser(void)
{
#ifndef FEATURE_WAP
	extern resume_is_ok;
#endif
    clk_dereg(&clk_timer); /* Unregister periodic callback to timerTick */
    timerTerminate(); /* Delete all timers and free memory */
#ifndef FEATURE_WAP
	resume_is_ok = TRUE;
#endif //FEATURE_WAP
	//CLNTa_log(0, 0, "***** Browser Suspended ***** \n");
}

uint32  CLNTa_currentTime (void)
{
    //CLNTa_log(0, 0, "CLNTa_currentTime\nVOID\n");

	/*
	** ELAPSED_SECONDS - 1970 Jan 1st 00:00:00 ~ 1980 Jan 6th 00:00:00
	** clk_read_secs() - 1980 Jan 6th 00:00:00 ~ very now
	*/

	return (ELAPSED_SECONDS + SKY_GetLocalTime());
}


void    CLNTa_setTimer (uint32 timeInterval)
{
	extern void RestartBrowserTimer(unsigned long ms);

	MSG_MED("CLNTa_setTimer [%d]",timeInterval*100,0,0);
	RestartBrowserTimer(timeInterval * 100); //millisecond
}
void    CLNTa_resetTimer(void)
{
	extern void CancelBrowserTimer(void);

	MSG_MED("CLNTa_resetTimer",0,0,0);
	CancelBrowserTimer();
}


VOID    CLNTa_acknowledgePostContent (UINT8 urlID)
{
    CLNTa_log(1,0,"DUMMY CLNTa_acknowledgePostContent");
}

VOID    CLNTa_resultDecodeProvisioning (const DATATABSTRUCT *dataTab, UINT16 dataTabLen,const WCHAR *strTab, UINT16 strTabLen, UINT16 iResult)
{
        CLNTa_log(1,0,"DUMMY CLNTa_resultDecodeProvisioning");
}

VOID    CLNTa_resultDecodeE2Esecurity (UINT8 objectID, UINT8 id, UINT32 expires, const DATATABSTRUCT *dataTab, UINT16 dataTabLen, const WCHAR *strTab, UINT16 strTabLen, UINT16 iResult)
{
        CLNTa_log(1,0,"DUMMY CLNTa_resultDecodeE2Esecurity");
}

VOID	CLNTa_getProvSharedSecret(UINT16 iListID, UINT16 SEC)
{
        CLNTa_log(1,0,"DUMMY CLNTa_getProvSharedSecret");
}

VOID	CLNTa_changedChannel(UINT8 objectId, UINT8 channelId, UINT8 isWTLSchannel)
{
        CLNTa_log(1,0,"DUMMY CLNTa_changedChannel %d %d",objectId,channelId);
}

