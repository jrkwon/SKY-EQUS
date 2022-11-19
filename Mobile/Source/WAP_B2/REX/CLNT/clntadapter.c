#include "customer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>



#include "sky.h"
//    #include "ui.h"
//    #include "uiuint.h"

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
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
#include "waptask.h"
#include "clntapp.h"




////////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////////


#ifdef LOG_ENABLE
////////////////////////////////////////////////////////////////////////////////////
// Message buffers for CLNTa_log()
////////////////////////////////////////////////////////////////////////////////////
char debug[NUM_OF_BUFFER][DEBUG_BUF_SIZE];
char temp[TEMP_BUF_SIZE];
char buf[MSG_BUF_SIZE];
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

#ifdef DIRECT_RESPONSE
extern boolean isIntegerResponse;
#endif//#ifdef DIRECT_RESPONSE

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


static ui_cmd_type   *ui_cmd_ptr;/* to hold the UI cmd. */

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
extern int typeOfContent;
extern setImageType imageInfo;
extern BOOL g_fCurrentSate_WAPEXIT;

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
VOID    CLNTa_content (UINT8 urlID, const CHAR *data, UINT16 length, CHAR *contentType, INT16 errorNo)
{
#ifndef WAPCORE_FOR_WARNING
	extern BOOL Wtlshello;
#endif //WAPCORE_FOR_WARNING

#ifdef ABORT_IN_CONTENT		
	extern BOOL bCall_content;
	bCall_content = FALSE;
	isStringResponse = FALSE;

  #ifndef DIRECT_RESPONSE
		isIntegerResponse = FALSE;
  #endif//#ifndef DIRECT_RESPONSE

#endif//#ifdef ABORT_IN_CONTENT		

	//===============================================================
	// Download된 이미지를 단말기의 Animation Data로 저장한다
	//===============================================================
	
	contentLength = length;
	CLNTa_log(1, 0, "CLNTa_content is called! dataLength[%d]bytes\n",length);

#ifndef WAP_USE_SECURE
	if(data == NULL && !Wtlshello )
#else//WAP_USE_SECURE
	if(data == NULL)
#endif//#ifndef WAP_USE_SECURE
	{
		CLNTa_log(1,0,"data = NULL");
		typeOfContent = CONTENT_TYPE_INVALID;

#ifndef DIRECT_RESPONSE
		if (isStringResponse)
		{
			clnt_make_string_response(urlID, NULL);
		}
		else if (isIntegerResponse)
		{
			
			clnt_make_integer_response(urlID, 1004);
		}
#endif //#ifndef DIRECT_RESPONSE
			
		CLNTa_error(1, errorNo, ERRTYPE_INFORMATION);
		return;
	}
	
#ifndef WAP_USE_SECURE
	if(Wtlshello)
	{
		if(typeOfContent == SecTLS)
		{
			CLNTa_log(1,0," bStopkey setting in clntadapter.c");
		}
		else
		{
			typeOfContent = SecTLS;
			memset(contentBuffer, '\0', MAX_CONTENT_SIZE);
			length = 0;
		}
	}
#endif//#ifndef WAP_USE_SECURE

	if ((length <= MAX_CONTENT_SIZE) && (length > 0))
	{
		memset(contentBuffer, '\0', MAX_CONTENT_SIZE);
		memcpy(contentBuffer, data, length);
	} 
	else
	{
#ifdef FEATURE_MUSIC_BELL
		extern void musicbell_cmd(void);
		musicbell_cmd();
#endif //#ifdef FEATURE_MUSIC_BELL

		CLNTa_log(1, 0, "[CLNTa_content]Data size is too big:[%d]bytes\n",length);

		//Content를 받는 것을 실패
		g_bGettingContent = FALSE;//실패

		//Initialize typeOfContent
#ifndef WAP_USE_SECURE
		if(!Wtlshello)
#endif//#ifndef WAP_USE_SECURE
		{
			typeOfContent = CONTENT_TYPE_INVALID;

#ifndef DIRECT_RESPONSE
			if (isStringResponse)
			{
				clnt_make_string_response(urlID, NULL);
			}
			else if (isIntegerResponse)
			{
				clnt_make_integer_response(urlID, 1004);
			}			
#endif //#ifndef DIRECT_RESPONSE
			return;
		}
#ifndef WAP_USE_SECURE
		else
		{
			length = 0;				
		}
#endif//#ifndef WAP_USE_SECURE
	}
	
	g_bSavingContent = TRUE;//저장시작

	switch(typeOfContent)
	{
		case IMAGE_BMP:
		case IMAGE_WBMP:
		case IMAGE_ANIMATION:
		{
			CLNTa_log(1, 0, "\nIMAGE! IMAGE! IMAGE! IMAGE! OK OK OK!!\n");

			clnt_make_integer_response(urlID,	Process_DownloadedImage(contentBuffer, length,
												imageInfo.url, imageInfo.title,
												imageInfo.kindOfImage, imageInfo.imageType));

			//----------------------------------
			//Content를 모두 받았다
			g_bGettingContent = FALSE;//성공
			g_bSavingContent = FALSE;//저장완료
			//----------------------------------

			//--------------------------------------------------------
			// Animation Stop
			if(( ui_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
			{
				CLNTa_log(1,0,"No UI cmd buffers");
			}
			else		
			{
				ui_cmd_ptr->hdr.cmd        = UI_WAP_DISPLAY_F;
				ui_cmd_ptr->hdr.task_ptr   = NULL;
				ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
				ui_cmd( ui_cmd_ptr);
			}

			//--------------------------------------------------------
			// repaint screen
#ifdef PORTING_B0_WAP
			ui_wappic_set(0,FALSE);
#endif //PORTING_B0_WAP

			//--------------------------------------------------------
			// 이미지 저장중 End키로 wap종료시 화면 문제 해결
			if(g_fCurrentSate_WAPEXIT != TRUE)	//yjahn96 01.05.04
				WAP_Repaint();

		    // yjahn96 01.04.12 --------------------------------------
			// 저장후, 이미지가 있는 화면에서 Animation timer stop 
			CancelAnimationTimer(); //	ui_sistimer_set(0, FALSE);//stop sis animation
			break;
		}

#ifdef FOR_MS
		case SecTLS:
		{
			CLNTa_log(1,0,"Server hello received");
			if((ui_cmd_ptr =(ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
			{	
				CLNTa_log(1,0,"No UI cmd buffers");
			}
			else		
			{
#ifdef PORTING_B0_WAP
				CLNTa_log(1,0,"GO to CHANGE");
				ui_cmd_ptr->hdr.cmd        = UI_WTLS_CHNKEY_F;
				ui_cmd_ptr->hdr.task_ptr   = NULL;
				ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
				ui_cmd_ptr->wtlsgo.length  = length;
				CLNTa_log(1,0,"GET content length %d",length);
				ui_cmd( ui_cmd_ptr);
#endif //PORTING_B0_WAP
			}
			break;
		}
#endif


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

			}

#ifndef DIRECT_RESPONSE
			clnt_make_string_response(urlID, mResult);
#endif //#ifndef DIRECT_RESPONSE

			//----------------------------------
			//Content를 모두 받았다
			g_bGettingContent = FALSE;//성공
			g_bSavingContent = FALSE;//저장완료
			//----------------------------------

			if(( ui_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
			{
				CLNTa_log(1,0,"No UI cmd buffers");
			}
			else		
			{
				ui_cmd_ptr->hdr.cmd        = UI_WAP_DISPLAY_F;
				ui_cmd_ptr->hdr.task_ptr   = NULL;
				ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
				ui_cmd( ui_cmd_ptr);
			}
			
			//--------------------------------------------------------
			// repaint screen
#ifdef PORTING_B0_WAP
			ui_wappic_set(0,FALSE);
#endif //PORTING_B0_WAP

			//--------------------------------------------------------
			// 뮤직벨 연주준비중 / 저장중 End키로 wap종료시 화면 문제 해결
			if(g_fCurrentSate_WAPEXIT != TRUE)	//yjahn96 01.05.04
				WAP_Repaint();
			break;

#endif //FEATURE_MUSIC_BELL

		case MYBELL:break;
		default:
			break;
	}
#ifdef DEBUG_CLNT
    CLNTa_log(1, 0, "CLNTa_content\nUINT8 \t urlID \t %u\nconst CHAR * \t confirmId \t %s\nUINT16 \t length \t %u\nCHAR * \t contentType \t %s\nINT16 \t errorNo \t %i\n", (int)urlID, data, (int)length, contentType, (int)errorNo);
	CLNTa_log(1, 0, "[%s]\n",contentBuffer);
#endif //#ifdef DEBUG_CLNT

	//----------------------------------
	//Content를 모두 받았다
	g_bGettingContent = FALSE;//성공
	g_bSavingContent = FALSE;//저장완료
	//----------------------------------

	//Initialize typeOfContent
	typeOfContent = CONTENT_TYPE_INVALID;

}


///////////////////////////////////////////////////////////////////////////////////////////////////
// CLNTa_confirmDownload()
///////////////////////////////////////////////////////////////////////////////////////////////////

VOID    CLNTa_confirmDownload (INT16 confirmId)
{
#ifndef CBASIC_WITH_NO_CONNECTORS
    BOOL    bisOTAallowed = TRUE;
#endif

#ifdef DEBUG_CLNT
    CLNTa_log(1, 0, "CLNTa_confirmDownload\nINT16 \t confirmId \t %i\n", (int)confirmId);
#endif //#ifdef DEBUG_CLNT

#ifndef CBASIC_WITH_NO_CONNECTORS
    CLNTc_confirmDownload(confirmId, bisOTAallowed);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLNTa_error()
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SOCKET_AUWAP
byte clnta_err_flag = 0;
extern void wap_display_error_message(int kindOfError, int kindOfType);

print_msg(int a, int type)
{
	switch(type) {
	case 0:
		MSG_HIGH("MMIa_status %d", a,0,0);
		break;
	case 1:
		MSG_HIGH("MMIa_wait %d", a,0,0);
		break;
	case 2:
		MSG_HIGH("MMIa_newcard %d", a,0,0);
		break;
	case 3:
		MSG_HIGH("MMIa_showcard %d", a,0,0);
		break;
	case 4:
		MSG_HIGH("CLNTa_choosePort %d", a,0,0);
		break;
	case 5:
		MSG_HIGH("CLNTa_error %d", a,0,0);
	default:
		break;
	}

}

VOID    CLNTa_error (UINT8 viewId, INT16 errorNo, UINT8 errorType)
{   
	clnta_err_flag=1;

//	error_cnt = 0;
	{
		extern print_msg(int a,int b);
		print_msg( errorNo,5);
	}
	wait_ani_stop();
	wap_display_error_message(errorNo, errorType);
}

#else //SOCKET_AUWAP

VOID    CLNTa_error (UINT8 viewId, INT16 errorNo, UINT8 errorType)
{
#ifdef FEATURE_WAP_ERROR_RELOAD
    extern int g_reloaderrorTime;
#endif //FEATURE_WAP_ERROR_RELOAD
#ifdef WAPCORE_FOR_WARNING
	extern void	wap_set_error(unsigned char isError);
	MSG_LOW(" Dummy for WARNING in clntadapter.c",0,0,0);
#endif //WAPCORE_FOR_WARNING
	ui_errortype = errorType;
	errornum = errorNo;
	
	if (errornum)
    {
		switch (errornum)
        {
#ifdef BLOCK
			case ERR_WTP_ABORT_START:
            {
                cszErrorCode = "ERR_WTP_ABORT_START";
                break;
            }
#endif //#ifdef BLOCK
                case ERR_WTP_UNKNOWN:   
                {
                    cszErrorCode = "ERR_WTP_UNKNOWN";
                    break;
                }
                case ERR_WTP_PROTOERR:
                {
                    cszErrorCode = "ERR_WTP_PROTOERR";
                    break;
                }
                case ERR_WTP_INVALIDTID:
                {
                    cszErrorCode = "ERR_WTP_INVALIDTID";
                    break;
                }
                case ERR_WTP_NOTIMPLEMENTEDCL2:
				{
                    cszErrorCode = "ERR_WTP_NOTIMPLEMENTEDCL2";
                    break;
                }
                case ERR_WTP_NOTIMPLEMENTEDSAR:
                {
                    cszErrorCode = "ERR_WTP_NOTIMPLEMENTEDSAR";
                    break;
                }
                case ERR_WTP_NOTIMPLEMENTEDUACK:
                {
                    cszErrorCode = "ERR_WTP_NOTIMPLEMENTEDUACK";
                    break;
                }
                case ERR_WTP_WTPVERSIONONE:
                {
                    cszErrorCode = "ERR_WTP_WTPVERSIONONE";
                    break;
                }
                case ERR_WTP_CAPTEMPEXCEEDED:
                {
                    cszErrorCode = "ERR_WTP_CAPTEMPEXCEEDED";
                    break;
                }
                case ERR_WTP_NORESPONSE:
                {
                    cszErrorCode = "ERR_WTP_NORESPONSE";
                    break;
                }
                case ERR_WTP_MESSAGETOOLARGE:
                {
                    cszErrorCode = "ERR_WTP_MESSAGETOOLARGE";
                    break;
                }
#ifdef BLOCK
            case ERR_WTP_ABORT_END:
            {
                cszErrorCode = "ERR_WTP_ABORT_END";
                break;
            }
#endif //#ifdef BLOCK

            /*  WSP Status codes */
            /*  These are statuscodes from WSP requests, mainly server status messagages. See further RFC 2068 and WSP/B Appendix A */
            case ERR_HTTP_START:
            {
                cszErrorCode = "ERR_HTTP_START";
                break;
            }
                case HTTPBadRequest:    /* = 0x40 */
                {
                    cszErrorCode = "HTTPBadRequest";
                    break;
                }
                case HTTPUnauthorized:
                {
                    cszErrorCode = "HTTPUnauthorized";
                    break;
                }
                case HTTPPaymentRequired:
                {
                    cszErrorCode = "HTTPPaymentRequired";
                    break;
                }
                case HTTPForbidden:
				{
                    cszErrorCode = "HTTPForbidden";
                    break;
                }
                case HTTPFileNotFound:
                {
                    cszErrorCode = "HTTPFileNotFound";
#ifdef FEATURE_WAP_ERROR_RELOAD
                    if(g_reloaderrorTime <= 2)
                    {
                        g_reloaderrorTime++;
                    }
#endif //#ifdef FEATURE_WAP_ERROR_RELOAD
                    break;
                }
                case HTTPMethodNotAllowed:
                {
                    cszErrorCode = "HTTPMethodNotAllowed";
                    break;
                }
                case HTTPNotAcceptable:
				{
                    cszErrorCode = "HTTPNotAcceptable";
                    break;
                }
                case HTTPProxyAuthenticationRequired:
                {
                    cszErrorCode = "HTTPProxyAuthenticationRequired";
                    break;
                }
                case HTTPRequestTimeout:
                {
                    cszErrorCode = "HTTPRequestTimeout";
                    break;
                }
                case HTTPConflict:
                {
                    cszErrorCode = "HTTPConflict";
                    break;
                }
                case HTTPGone:
                {
                    cszErrorCode = "HTTPGone";
                    break;
                }
                case HTTPLengthRequired:
                {
                    cszErrorCode = "HTTPLengthRequired";
                    break;
                }
                case HTTPPreconditionFailed:
                {
                    cszErrorCode = "HTTPPreconditionFailed";
                    break;
                }
                case HTTPRequestedEntityTooLarge:
                {
                    cszErrorCode = "HTTPRequestedEntityTooLarge";
                    break;
                }
                case HTTPRequestURITooLarge:
                {
                    cszErrorCode = "HTTPRequestURITooLarge";
                    break;
                }
                case HTTPUnsupportedMediaType:
                {
                    cszErrorCode = "HTTPUnsupportedMediaType";
                    break;
                }

                case HTTPInternalServerError:   /* = 0x60 */
                {
                    cszErrorCode = "HTTPInternalServerError";
#ifdef FEATURE_WAP_ERROR_RELOAD
                    if(g_reloaderrorTime <= 2)
                    {
                        g_reloaderrorTime++;
                    }
#endif //#ifdef FEATURE_WAP_ERROR_RELOAD
                    break;
                }
                case HTTPNotImplemented:
                {
                    cszErrorCode = "HTTPNotImplemented";
                    break;
                }
                case HTTPBadGateway:
                {
                    cszErrorCode = "HTTPBadGateway";
                    break;
                }
                case HTTPServiceUnavailable:
                {
                    cszErrorCode = "HTTPServiceUnavailable";
                    break;
                }
                case HTTPGatewayTimeout:
                {
                    cszErrorCode = "HTTPGatewayTimeout";
                    break;
                }
                case HTTPVerNotSupported:
                {
                    cszErrorCode = "HTTPVerNotSupported";
                    break;
                }
            case ERR_HTTP_END:
            {
                cszErrorCode = "ERR_HTTP_END";
                break;
            }


			case ERR_WSP_ABORT_START:
            {
                cszErrorCode = "ERR_WSP_ABORT_START";
                break;
            }
                case ERR_WSP_PROTOERR:   
                {
                    cszErrorCode = "ERR_WSP_PROTOERR";
                    break;
                }
                case ERR_WSP_DISCONNECT:
                {
                    cszErrorCode = "ERR_WSP_DISCONNECT";
                    break;
                }
                case ERR_WSP_SUSPEND:
                {
                    cszErrorCode = "ERR_WSP_SUSPEND";
                    break;
                }
                case ERR_WSP_RESUME:
				{
                    cszErrorCode = "ERR_WSP_RESUME";
                    break;
                }
                case ERR_WSP_CONGESTION:
                {
                    cszErrorCode = "ERR_WSP_CONGESTION";
                    break;
                }
                case ERR_WSP_CONNECTERR:
                {
                    cszErrorCode = "ERR_WSP_CONNECTERR";
                    break;
                }
                case ERR_WSP_MRUEXCEEDED:
                {
                    cszErrorCode = "ERR_WSP_MRUEXCEEDED";
                    break;
                }
                case ERR_WSP_PEERREQ:
                {
                    cszErrorCode = "ERR_WSP_PEERREQ";
                    break;
                }
                case ERR_WSP_NETERR:
                {
                    cszErrorCode = "ERR_WSP_NETERR";
                    break;
                }
                case ERR_WSP_USERREQ:
                {
                    cszErrorCode = "ERR_WSP_USERREQ";
                    break;
                }
            case ERR_WSP_ABORT_END:
            {
                cszErrorCode = "ERR_WSP_ABORT_END";
                break;
            }
            case ERR_WAE_START:
            {
                cszErrorCode = "ERR_WAE_START";
                break;
            }
                case ERR_WAE_OUT_OF_MEMORY:
                {
                    cszErrorCode = "ERR_WAE_OUT_OF_MEMORY";
                    break;
                }

                case ERR_WAE_UA_START:
                {
                    cszErrorCode = "ERR_WAE_UA_START";
                    break;
                }
                    case ERR_WAE_UA_VIEWID_INVALID:             /* invalid view id was used */
                    {
                        cszErrorCode = "ERR_WAE_UA_VIEWID_INVALID";
                        break;
                    }
                    case ERR_WAE_UA_MAX_EXCEEDED:               /* too many active user agents */
                    {
                        cszErrorCode = "ERR_WAE_UA_MAX_EXCEEDED";
                        break;
                    }
                    case ERR_WAE_UA_PARSE:                      /* error in wml code */
                    {
                        cszErrorCode = "ERR_WAE_UA_PARSE";
                        break;
                    }
                    case ERR_WAE_UA_DISPLAY_ERROR:              /* mmi engine is not able to process content */
                    {
                        cszErrorCode = "ERR_WAE_UA_DISPLAY_ERROR";
                        break;
                    }
                    case ERR_WAE_UA_RESPONSE_BODY_INVALID:      /* no proper WML repsonse body, possibly empty */
                    {
                        cszErrorCode = "ERR_WAE_UA_RESPONSE_BODY_INVALID";
                        break;
                    }
                    case ERR_WAE_UA_URL_INVALID:                /* malformed url was encountered */
                    {
                        cszErrorCode = "ERR_WAE_UA_URL_INVALID";
                        break;
                    }
                    case ERR_WAE_UA_URL_TIMEOUT:                /* urlrequest timed out */
                    {
                        cszErrorCode = "ERR_WAE_UA_URL_TIMEOUT";
                        break;
                    }
                    case ERR_WAE_UA_WSP_RESPONSE_INVALID:       /* error in wsp header e.g.no valid contenttype, non recognizable HTTP-WSP/B response was encountered */
                    {
                        cszErrorCode = "ERR_WAE_UA_WSP_HEADER_INVALID";
                        break;
                    }
                    case ERR_WAE_UA_WMLDECK_ACCESS_DENIED:      /* wmldeck contained accessrestrictions not fullfilled by ua */
                    {
                        cszErrorCode = "ERR_WAE_UA_WMLDECK_ACCESS_DENIED";
                        break;
                    }
                    case ERR_WAE_UA_URL_NONSUPPORTED_SCHEME:    /* a non supported scheme was used in the wrong context*/
                    {
                        cszErrorCode = "ERR_WAE_UA_URL_NONSUPPORTED_SCHEME";
                        break;
                    }
                    case ERR_WAE_UA_REDIRECT_ERROR:             /* a request has redirected more than 5 times */
                    {
                        cszErrorCode = "ERR_WAE_UA_REDIRECT_ERROR";
                        break;
                    }
					case ERR_WAE_UA_SESSION_NOT_CONNECTED:             /* a request has redirected more than 5 times */
                    {
                        cszErrorCode = "ERR_WAE_UA_SESSION_NOT_CONNECTED";
                        break;
                    }
					case ERR_WAE_UA_MAX_NR_OF_SESSIONS_REACHED:             /* a request has redirected more than 5 times */
                    {
                        cszErrorCode = "ERR_WAE_UA_MAX_NR_OF_SESSIONS_REACHED";
                        break;
                    }
                case ERR_WAE_UA_END:
                {
                    cszErrorCode = "ERR_WAE_UA_END";
                    break;
                }
                case ERR_WAE_WML_START:
                {
                    cszErrorCode = "ERR_WAE_WML_START";
                    break;
                }
                    case ERR_WAE_WML_INSTREAM_FAULT:
                    {
                        cszErrorCode = "ERR_WAE_WML_INSTREAM_FAULT";
                        break;
                    }
                    case ERR_WAE_WML_CONTENT_CHARSET_ERROR:         /* error in the charset coding */
                    {
                        cszErrorCode = "ERR_WAE_WML_CONTENT_CHARSET_ERROR";
                        break;
                    }
                    case ERR_WAE_WML_CONTENT_CHARSET_NOT_SUPPORTED: /* charset coding not supported */
                    {
                        cszErrorCode = "ERR_WAE_WML_CONTENT_CHARSET_NOT_SUPPORTED";
                        break;
                    }
                    case ERR_WAE_WML_UNKNOWN_TOKEN:
                    {
                        cszErrorCode = "ERR_WAE_WML_UNKNOWN_TOKEN";
                        break;
                    }
                    case ERR_WAE_WML_WML_ERROR:
                    {
                        cszErrorCode = "ERR_WAE_WML_WML_ERROR";
                        break;
                    }
                    case ERR_WAE_WML_CONTENT_VERSION_WARNING:       /* version nbr warning - parsing is NOT cancelled */
                    {
                        cszErrorCode = "ERR_WAE_WML_CONTENT_VERSION_WARNING";
                        break;
                    }
                    case ERR_WAE_WML_CONTENT_PUBLIC_ID_WARNING:     /* public id warning - parsing is NOT cancelled */
                    {
                        cszErrorCode = "ERR_WAE_WML_CONTENT_PUBLIC_ID_WARNING";
                        break;
                    }
                    case ERR_WAE_WML_CONTENT_PUBLIC_ID_ERROR:       /* public id NOT wml- parsing is cancelled */
                    {
                        cszErrorCode = "ERR_WAE_WML_CONTENT_PUBLIC_ID_ERROR";
                        break;
                    }
                case ERR_WAE_WML_END:
                {
                    cszErrorCode = "ERR_WAE_WML_END";
                    break;
                }
                case ERR_WAE_WMLS_START:
                {
                    cszErrorCode = "ERR_WAE_WMLS_START";
                    break;
                }
                    case ERR_WAE_WMLS_NONE:             /* NO ERROR */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_NONE";
                        break;
                    }
                    case ERR_WAE_WMLS_VERIFICATION:     /* Verification failed */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_VERIFICATION";
                        break;
                    }
                    case ERR_WAE_WMLS_LIB:              /* Fatal library function error */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_LIB";
                        break;
                    }
                    case ERR_WAE_WMLS_FUNC_ARGS:        /* Invalid function arguments */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_FUNC_ARGS";
                        break;
                    }
                    case ERR_WAE_WMLS_EXTERNAL:         /* External function not found */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_EXTERNAL";
                        break;
                    }
                    case ERR_WAE_WMLS_LOAD:             /* Unable to load compilation unit */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_LOAD";
                        break;
                    }
                    case ERR_WAE_WMLS_ACCESS:           /* Access violation */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_ACCESS";
                        break;
                    }
                    case ERR_WAE_WMLS_STACK_UNDERFLOW:  /* Stack underflow */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_STACK_UNDERFLOW";
                        break;
                    }
                    case ERR_WAE_WMLS_ABORT:            /* Programmed abort */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_ABORT";
                        break;
                    }
                    case ERR_WAE_WMLS_STACK_OVRFLW:     /* Stack overflow */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_STACK_OVRFLW";
                        break;
                    }
                    case ERR_WAE_WMLS_USER_ABORT:       /* User initiated */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_USER_ABORT";
                        break;
                    }
                    case ERR_WAE_WMLS_SYSTEM_ABORT:     /* System initiated */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_SYSTEM_ABORT";
                        break;
                    }
                    case ERR_WAE_WMLS_NULL:             /* some component was inaccessible */
                    {
                        cszErrorCode = "ERR_WAE_WMLS_NULL";
                        break;
                    }
                case ERR_WAE_WMLS_END:
                {
                    cszErrorCode = "ERR_WAE_WMLS_END";
                    break;
                }
				case ERR_WAE_REP_START:
				{
					cszErrorCode = "ERR_WAE_REP_START";
					break;
				}
					case ERR_WAE_REP_SERVICE_INSTALL_FAILED:     
                    {
                        cszErrorCode = "ERR_WAE_REP_SERVICE_INSTALL_FAILED";
                        break;
                    }
                    case ERR_WAE_REP_MEM_ACCESS_FAILED:           
                    {
                        cszErrorCode = "ERR_WAE_REP_MEM_ACCESS_FAILED";
                        break;
                    }

				case ERR_WAE_REP_END:
				{
					cszErrorCode = "ERR_WAE_REP_END";
					break;
				}
            case ERR_WAE_END:
            {
                cszErrorCode = "ERR_WAE_END";
                break;
            }

            case ERR_WSPCL_START:
            {
                cszErrorCode = "ERR_WSPCL_START";
                break;
            }
                case ERR_WSPCL_ErrorInAddressFromWAE:               /* Address received from WAE incorrect */
                {
                    cszErrorCode = "ERR_WSPCL_ErrorInAddressFromWAE";
                    break;
                }
                case ERR_WSPCL_ErrorExtractReplyPDUFailed:          /* Received ReplyPDU incorrect or no memory available for extraction */
                {
                    cszErrorCode = "ERR_WSPCL_ErrorExtractReplyPDUFailed";
                    break;
                }
                case ERR_WSPCL_ErrorNoBuffersAvailable:    /* Input from WAE too large or empty */
                {
                    cszErrorCode = "ERR_WSPCL_ErrorNoBuffersAvailable";
                    break;
                }
                case ERR_WSPCL_ErrorMethodNotSupported:          
                {
                    cszErrorCode = "ERR_WSPCL_ErrorMethodNotSupported";
                    break;
                }
                case ERR_WSPCL_ErrorExtractPushPDUFailed:             
                {
                    cszErrorCode = "ERR_WSPCL_ErrorExtractPushPDUFailed";
                    break;
                }
				case ERR_WSPCL_ErrorErroneousPDUTypeReceived:             
                {
                    cszErrorCode = "ERR_WSPCL_ErrorErroneousPDUTypeReceived";
                    break;
                }
                case ERR_WSPCL_ErrorErroneousStackConfigReceived:          
                {
                    cszErrorCode = "ERR_WSPCL_ErrorErroneousStackConfigReceived";
                    break;
                }
            case ERR_WSPCL_END:
            {
                cszErrorCode = "ERR_WSPCL_END";
                break;
            }
            case ERR_WSPCM_START:
            {
                cszErrorCode = "ERR_WSPCM_START";
                break;
            }
                case ERR_WSPCM_ErrorNoMemoryAvailableForPDUPacking:        /* No MBM available for PDU construction */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorNoMemoryAvailableForPDUPacking";
                    break;
                }
                case ERR_WSPCM_ErrorNoMemoryAvailableForPDUUnPacking:   /* No memory available for PDU extraction  */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorNoMemoryAvailableForPDUUnPacking";
                    break;
                }
                case ERR_WSPCM_ErrorInDataFromWAE:                      /* Data from WAE erroneous */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorInDataFromWAE";
                    break;
                }
                case ERR_WSPCM_ErrorInReplyFromServer:                  /* Data from server erroneous */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorInReplyFromServer";
                    break;
                }
                case ERR_WSPCM_ErrorMaxSessionsAlreadyReached:          /* Number of active sessions already equal to MXSESSIONS */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorMaxSessionsAlreadyReached";
                    break;
                }
                case ERR_WSPCM_ErrorMOMAlreadyReached:                  /* Number of outstanding methods already equal to MOM  */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorMOMAlreadyReached";
                    break;
                }
				case ERR_WSPCM_WAEErrorNoPIdFoundMatchingSession:          /* The dispatcher finds no PId corresponding to the wanted session */
                {
                    cszErrorCode = "ERR_WSPCM_WAEErrorNoPIdFoundMatchingSession";
                    break;
                }
                case ERR_WSPCM_ErrorNoPIdFoundMatchingMethod:           /* No ClientSessionProcess found matching TID */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorNoPIdFoundMatchingMethod";
                    break;
                }
				case ERR_WSPCM_ErrorNoPIdFoundMatchingPush:           /* No ClientSessionProcess found matching TID */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorNoPIdFoundMatchingPush";
                    break;
                }
                case ERR_WSPCM_ErrorStoreOMInfoFailed:                  /* Session data could not be stored */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorStoreOMInfoFailed";
                    break;
                }
                case ERR_WSPCM_ErrorStoreHandleFailed:                  /* Handle could not be stored */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorStoreHandleFailed";
                    break;
                }
                case ERR_WSPCM_ErrorMethodNotSupported:                 /* Requested method not supported */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorMethodNotSupported";
                    break;
                }
				case ERR_WSPCM_ErrorSameAQUsed:                  /* Handle could not be stored */
                {
                    cszErrorCode = "ERR_WSPCM_ErrorSameAQUsed";
                    break;
                }
                case ERR_WSPCM_WAEErrorNoPIdFoundMatchingMethod:                 /* Requested method not supported */
                {
                    cszErrorCode = "ERR_WSPCM_WAEErrorNoPIdFoundMatchingMethod";
                    break;
                }
            case ERR_WSPCM_END:
            {
                cszErrorCode = "ERR_WSPCM_END";
                break;
            }
            case ERR_WTP_START:
            {
                cszErrorCode = "ERR_WTP_START";
                break;
            }
				case ERR_WTP_ErrorNORESPONSE:                
                {
                    cszErrorCode = "ERR_WTP_ErrorNORESPONSE";
                    break;
                }
                case ERR_WTP_ErrorNOFREEBUFF:         
                {
                    cszErrorCode = "ERR_WTP_ErrorNOFREEBUFF";
                    break;
                }
				case ERR_WTP_ErrorINVALID_BEARER:            
                {
                    cszErrorCode = "ERR_WTP_ErrorINVALID_BEARER";
                    break;
                }
                case ERR_WTP_ErrorINVALID_CLASS:             
                {
                    cszErrorCode = "ERR_WTP_ErrorINVALID_CLASS";
                    break;
                }
				case ERR_WTP_ErrorINVALID_ACKTYPE:         
                {
                    cszErrorCode = "ERR_WTP_ErrorINVALID_ACKTYPE";
                    break;
                }
            case ERR_WTP_END:
            {
                cszErrorCode = "ERR_WTP_END";
                break;
            }
            case ERR_WDP_START:
            {
                cszErrorCode = "ERR_WDP_START";
                break;
            }
                case ERR_WDP_ErrorInDatafromWSP:        /* The data reveived from WSP was faulty */
                {
                    cszErrorCode = "ERR_WDP_ErrorInDatafromWSP";
                    break;
                }
                case ERR_WDP_ErrorInDatafromWTP:        /* The data reveived from WTP was faulty */
                {
                    cszErrorCode = "ERR_WDP_ErrorInDatafromWTP";
                    break;
                }
                case ERR_WDP_ErrorBearerNotSupported:   /* The bearer is not supported */
                {
                    cszErrorCode = "ERR_WDP_ErrorBearerNotSupported";
                    break;
                }
				case ERR_WDP_WDErrorInd:    
                {
                    cszErrorCode = "ERR_WDP_WDErrorInd";
                    break;
                }
                case ERR_WDP_UDPErrorInd: 
                {
                    cszErrorCode = "ERR_WDP_UDPErrorInd";
                    break;
                }
                case ERR_WDP_UDPBigBuffer:      
                {
                    cszErrorCode = "ERR_WDP_UDPBigBuffer";
                    break;
                }
                case ERR_WDP_SMSErrorInd:  
                {
                    cszErrorCode = "ERR_WDP_SMSErrorInd";
                    break;
                }
				case ERR_WDP_SMSBigBuffer:    
                {
                    cszErrorCode = "ERR_WDP_SMSBigBuffer";
                    break;
                }
                case ERR_WCMP_PortUnreachable:   
                {
                    cszErrorCode = "ERR_WCMP_PortUnreachable";
                    break;
                }
				case ERR_WCMP_MessageTooBig:
                {
                    cszErrorCode = "ERR_WCMP_MessageTooBig";
                    break;
                }
            case ERR_WDP_END:
            {
                cszErrorCode = "ERR_WDP_END";
                break;
            }

			case ERR_UDCP_START:
            {
                cszErrorCode = "ERR_UDCP_START";
                break;
            }

            case ERR_UDCP_END:
            {
                cszErrorCode = "ERR_UDCP_END";
                break;
            }

            case ERR_MEM_START:
            {
                cszErrorCode = "ERR_MEM_START";
                break;
            }
                case ERR_MEMORY_WARNING:    /* The memory has reached the level specified in the configuration variable MEMORY_WARNING in the file confvars.h */
                {
                    cszErrorCode = "ERR_MEMORY_WARNING";
                    break;
                }
                case ERR_OUT_OF_MEMORY:     /* No more memory! */
                {
                    cszErrorCode = "ERR_OUT_OF_MEMORY";
					errorCode = ERR_OUT_OF_MEMORY;
                    break;
                }
            case ERR_MEM_END:
            {
                cszErrorCode = "ERR_MEM_END";
                break;
            }

            /*------------------------------- these errors are only used internally -----------------------------*/
            case ERR_INTERNAL_START:
            {
                cszErrorCode = "ERR_INTERNAL_START";
                break;
            }
                case ERR_WAE_UA_OTA_NOT_ALLOWED:    /* ONLY USED INTERNALLY! : a non allowed OTA-request was attempted */
                {
                    cszErrorCode = "ERR_WAE_UA_OTA_NOT_ALLOWED";
                    break;
                }
                case ERR_WAE_UA_REQUEST_ABORTED:    /* ONLY USED INTERNALLY! : the urlrequest was aborted by the UA */
                {
                    cszErrorCode = "ERR_WAE_UA_REQUEST_ABORTED";
                    break;
                }
				case ERR_WAE_UA_INVALID_SESSION_ID:  
                {
                    cszErrorCode = "ERR_WAE_UA_INVALID_SESSION_ID";
                    break;
                }
                case ERR_WAE_WMLS_EXIT:    
                {
                    cszErrorCode = "ERR_WAE_WMLS_EXIT";
                    break;
                }
            case ERR_INTERNAL_END:
            {
                cszErrorCode = "ERR_INTERNAL_END";
                break;
            }

            default:
            {
                cszErrorCode = "errcodes.h에 정의되지 않은 에러";
                break;
            }
        }
#ifdef DEBUG_CLNT
        CLNTa_log(0, 0, "Error No: %d, view id: %u, errorCode = %s\n, errorType = %d\n", errornum, viewId, cszErrorCode, errorType);
#endif //#ifdef DEBUG_CLNT

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
    else
    {
#ifdef DEBUG_CLNT
      CLNTa_log(0, 0, "Error No: %d, view id: %u\n", errornum, viewId);
#endif //#ifdef DEBUG_CLNT
	
		errorResult = wap_check_error_range(errornum);
		if (errorResult != ERR_RESULT_IS_IGNORE)
		{
			wap_set_error(TRUE);
			wap_handle_error(errornum, errorType);
		}
    }
}
#endif //SOCKET_AUWAP



///////////////////////////////////////////////////////////////////////////////////////////////////
//  CLNTa_log()
///////////////////////////////////////////////////////////////////////////////////////////////////

void    CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...)
{

#ifdef LOG_ENABLE

    const char  *cszLogCode;
    va_list		arg_ptr;

    static int	i=0;
//	static int	log_Count = 0;
	
	int			j=0;
	int			times, residue;
	
	int			m=0;
	int			n=0;
	
	int			right_offset;
	int			buf_size;
	
	msg_ptr=msg;

	memset(temp,'\0',TEMP_BUF_SIZE);
	memset(buf,'\0',MSG_BUF_SIZE);
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

        //sprintf(temp, "Log No: %d, view id: %u\n%s", ++log_Count, viewId, format);
		sprintf(temp, "%s",format);
    }
    else
    {
		//sprintf(temp, "Log No: %d, view id: %u\n%s", ++log_Count, viewId, format);
		sprintf(temp, "%s",format);
    }

    va_start(arg_ptr, format);
	vsprintf(buf, temp, arg_ptr);
    va_end(arg_ptr);

	/*
	** Expand escape characters to according spaces
	*/
	buf_size=strlen(buf);

	while(buf_size--)
	{   
		/* right_offset indicates the rest size of right part */
		right_offset = DM_LINE_SIZE - n%DM_LINE_SIZE;

		switch (buf[m]) {
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
			msg[n++] = buf[m];
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
		memset(debug[i],'\0',DEBUG_BUF_SIZE);

		strncpy(debug[i], &msg[j*DM_LINE_SIZE], DM_LINE_SIZE);
		
		debug_msg(debug[i],0,0,0);

		i = (i+1) % 100;
		j++;
	}

	if (residue) {

		memset(debug[i],'\0',DEBUG_BUF_SIZE);

		strncpy(debug[i], &msg[j*DM_LINE_SIZE], residue);

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
////////////////////////////////////////////////////////////////////////////////////////////////////////

extern  uint16  SAport;
extern  boolean bchoose;
VOID    CLNTa_choosePort (UINT8 viewId, UINT8 id)
{
#ifdef BROWSER_UPDATE	
	wap_cmd_type msg_ptr;

	CLNTa_log(1,0,"Choose Port  id = %d, port %d  viewid %d",id,SAport,viewId);

	bchoose = TRUE;

	if((SAport > 0 ) && (SAport < 5000) )
	{
		SAport+=100;
	}
	else
	{
		SAport = 1000;
	}

	msg_ptr.hdr.cmd          = CLNT_USE_PORT;
	msg_ptr.use_port.viewId  = viewId;	
	msg_ptr.use_port.id      = id;
	msg_ptr.use_port.port    = (UINT16)SAport;
	msg_ptr.use_port.success = TRUE;

	make_cmd_other2wap( &msg_ptr );	
#endif//BROWSER_UPDATE
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

void    CLNTa_terminated (void)
{
#ifdef WAP_USE_LEAK
    extern INT16 wip_initmalloc (void *mem, UINT32 memsize);
	extern char Image$$APP_SHARE_RAM$$Base[];
	extern unsigned char *pShareRAM;
	extern BOOL WAP_Terminate_WML_Test(void);
#endif //WAP_USE_LEAK
#ifdef WAP_USE_START
    extern boolean rcvcall;
#endif //WAP_USE_START

	wap_set_exit();

	suspend_wap_browser();//두개의 순서를 바꾸어 보았음
#ifdef WAP_USE_LEAK
	WAP_Terminate_WML_Test();
#else//#ifdef WAP_USE_LEAK
    WAP_Terminate_WML();
#endif //WAP_USE_LEAK
    CLNTa_log(1,0,"browser_init started");
#ifdef WAP_USE_LEAK
	memset(pShareRAM,0x00,WIP_MALLOC_MEM_SIZE);	
#endif //WAP_USE_LEAK
	browserTerComp = TRUE;

#ifdef REDIRECT//jjkyg/06/28막음 
#ifdef WAP_USE_START
	if(redirect.bCorrect || rcvcall )
#else
	if(redirect.bCorrect)
#endif//#ifdef WAP_USE_START
	{
		if(( ui_cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q)) == NULL)
		{
			CLNTa_log(1,0,"No UI cmd buffers");
		}
		else		
		{
			ui_cmd_ptr->hdr.cmd        = UI_WAP_COMPLETE_F;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr);
		}
	}
#endif//#ifdef REDIRECT

	CLNTa_log(1,0,"-------[CLNTa_terminated]-------\n");
}

void  suspend_wap_browser(void)
{
	extern resume_is_ok;
	
    clk_dereg(&clk_timer); /* Unregister periodic callback to timerTick */

    timerTerminate(); /* Delete all timers and free memory */

	resume_is_ok = TRUE;

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
	if( !first_flag_tid ) {
		first_flag_tid = 1;

		tid_list = 0;

		timerInsert(timeInterval); //Just addeded 2000.1.4

		clk_reg(&clk_timer,timerTick,100,100,TRUE); /* timerTick to be called every 100 ms */
	}
	else /* Insert into queue */
	{
		timerInsert(timeInterval);
	}
}


void    CLNTa_resetTimer(void)
{
	
    //CLNTa_log(0, 0, "CLNTa_resetTimer\nUINT8 \t timerId \t %u\n", (int)timerId);
    timerDelete();
}

#ifdef WAP_USE_LEAK
void SendCmdtoUI(void)
{
	extern int isReStartComleted;
	ui_cmd_type   *ui_cmd_ptr;   

	isReStartComleted = 1;
    
	if(( ui_cmd_ptr = (ui_cmd_type *) q_get( &ui_cmd_free_q)) == NULL)
	{
	        ERR_FATAL( "No UI cmd buffers",0,0,0);
	}
	else
	{
			ui_cmd_ptr->hdr.cmd        = UI_WAP_COMPLETE_F;;
			ui_cmd_ptr->hdr.task_ptr   = NULL;
			ui_cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
			ui_cmd( ui_cmd_ptr); 
	}
}
#endif//#ifdef WAP_USE_LEAK
