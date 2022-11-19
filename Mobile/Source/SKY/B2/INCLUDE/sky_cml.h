/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     sky_cml.h
**
** Description:
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-08-01 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_CML_H_
#define _SKY_CML_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/* new command line 2001-02-26 */
enum tagLEFT_COMMAND_LINE_E {
    LCL_NONE=0,
    LCL_INIT,               /* 초기화 */
    LCL_VIEW,               /* 보기 */
    LCL_DELETE,             /* 삭제 */
    LCL_MENU,               /* 메뉴 */
    LCL_SAVE,               /* 저장 */
    LCL_FIND,               /* 찾기 */
	LCL_ALARM,				/* 알람 */
	LCL_KORAM,				/* 오전 */
	LCL_KORPM,				/* 오후 */
	LCL_AM,					/*  AM */
	LCL_PM,					/*  pm */
	LCL_BEFOREVIEW,			/* 미리보기 */
	LCL_RECEIPTBELL,		/* 착신벨 */
	LCL_EDIT,               /* 편집 */
	LCL_WRITE,              /* 작성 */
	LCL_FWD,                /* 전달 */
	LCL_CANCEL,				/* 취소 */
    LCL_POINT,              /* . */
#ifdef FEATURE_WAVELET
	LCL_PLAY,				//yoursun 01/06/19
	LCL_REPLY,
#endif //FEATURE_WAVELET
    LCL_MAX   
};
typedef enum tagLEFT_COMMAND_LINE_E LEFT_COMMAND_LINE_E;

enum tagCENTER_COMMAND_LINE_E {
    CCL_NONE=0,
    CCL_SELECT,             /* 선택 */
    CCL_START,              /* 시작 */
    CCL_SAVE,               /* 저장 */
    CCL_CONTINUE,           /* 계속 */
    CCL_CONFIRM,            /* 확인 */
    CCL_REPLY,              /* 회신 */
    CCL_SORISAM,            /* 소리샘 */
    CCL_MOVE,               /* 이동 */
    CCL_COMPLETE,           /* 완료 */
    CCL_VOLUME,             /* 볼륨 */
    CCL_SEND,               /* 전송 */
    CCL_MODIFY,             /* 수정 */
    CCL_TOSMS,              /* To SMS */
	CCL_PBOOK,				/* 전화부 */ 
    CCL_PASTE,              /* 붙이기 */
	CCL_MOVEUP,				/* 위로 이동*/
	CCL_MOVEDOWN,			/* 아래로 이동*/
	CCL_PLAY,				/* 재생 */
	CCL_LIST,				/* 목록 */
	CCL_OKUPDOWN,			/* ^ OK v */
	CCL_OKUP,				/* ^ OK */
	CCL_OKDOWN,				/* OK v */
	CCL_SETUP,				/* 설정 */
	CCL_DISSOLVE,			/* 해제 */
	CCL_INPUT,				/* 입력 */
	CCL_MINUTELY,			/* 자세히 -> 사용량  */
	CCL_REC,				/* 녹 음 */
	CCL_BEFOREVIEW,			/* 미리보기 */
	CCL_RECEIVESEND,		/* 착발신 */
	CCL_WRITE,				/* 작성 */
	CCL_NEWALARM,			/* 알람 */
	CCL_VIEW,				/* 보기 */
	CCL_CALL,               /* 통화 */
	CCL_RECEIPT,			/* 착신 */
	CCL_SIGN,
#ifdef FEATURE_WAVELET
	CCL_STOP,				//yoursun 01/06/19
#endif //FEATURE_WAVELET
    CCL_MAX
};
typedef enum tagCENTER_COMMAND_LINE_E CENTER_COMMAND_LINE_E;

enum tagRIGHT_COMMAND_LINE_E {
    RCL_NONE = 0,
    RCL_SAVE,               /* 저장 */
    RCL_STOP,               /* 정지 */
    RCL_RESET,              /* 리셋 */
    RCL_DST,                /* DST */
    RCL_DELETE,             /* 삭제 */
    RCL_FIND,               /* 찾기 */
    RCL_PROTECT,            /* 보호 */
    RCL_RELEASE,            /* 해제 */
    RCL_FWD,                /* 전달 */
    RCL_NEXT,               /* 다음 */
    RCL_SEND,               /* 전송 */
    RCL_MODIFY,             /* 수정 */
    RCL_ASSIGN,             /* 지정 */
    RCL_SORT,               /* 정렬 */
    RCL_EDIT,               /* 편집 */
    RCL_ADD,                /* 추가 */
    RCL_VIEW,               /* 보기 */
    RCL_PLAYMUSIC,          /* 연주 */
    RCL_REPLY,              /* 회신 */
    RCL_LUNAR,              /* 음력 */
    RCL_JULIAN,             /* 양력 */
    RCL_NEWSCHED,           /* 새일정 */
    RCL_ALARM,              /* 알람 */
    RCL_SET,                /* 설정 */
    RCL_NEWHOLIDAY,         /* 새휴일 */
    RCL_NEWALARM,           /* 새알람 */
    RCL_NEWMEMO,            /* 새메모 */
    RCL_PLAY,               /* 재생 */
    RCL_PBOOK,              /* 전화부 */
    RCL_WRITE,              /* 작성 */
    RCL_TOSMS,              /* To SMS */
    RCL_BOTTOMUP,           /* 거꾸로 */
    RCL_CANCEL,              /* 취소 <-- 잠금 기능에서만 사용 */
	RCL_NEWMAIL,			/* 새메일 */
    RCL_PASTE,              /* 붙이기 */
	RCL_SELECT,				/* 선택 */
	RCL_INIT,				/*초기화*/
	RCL_NEWNAME,			/*새이름*/
	RCL_NEWGROUP,			/*새그룹*/
	RCL_INDETAIL,			/*자세히*/
	RCL_DOT,				/* 점 */
	RCL_NEWARTICLE,			/* 새문장 */
    RCL_AC,                /* A/C  */
#ifdef FEATURE_WAVELET
	RCL_END,				//yoursun 01/09/3
#endif //FEATURE_WAVELET
    RCL_MAX
};
typedef enum tagRIGHT_COMMAND_LINE_E RIGHT_COMMAND_LINE_E;

/* command line */
enum tagCOMMAND_LINE_E {
    CL_NONE = 0,
	CL_NONE_SELECT_ADD,
    CL_SCHED_UPDOWN_PBOOK,  /* IDLE command line. */
    CL_NONE_NONE_SAVE,
    CL_NONE_SELECT_NONE,
    CL_NONE_SELECT_BOTTOMUP,
    CL_MENU_MOVEDOWN_BOTTOMUP,
    CL_NONE_START_NONE,
	CL_NONE_VOLUME_NONE,
	CL_NONE_TOSMS_NONE,
    CL_NONE_MOVE_NONE,
	CL_NONE_MOVE_EDIT,
	CL_DELETE_MOVE_MODIFY,
	CL_NONE_RECEIVESEND_NONE,
	CL_NONE_MOVEUP_NONE,
    CL_NONE_MOVEDOWN_NONE,
	CL_MENU_MOVEDOWN_NONE,
	CL_NONE_MOVE_DELETE,
    CL_NONE_MOVE_SEND,
    CL_MENU_MOVEDOWN_SEND,
    CL_NONE_SAVE_STOP,
    CL_NONE_SAVE_FIND, 
    CL_NONE_MOVE_DST,
    CL_NONE_CONFIRM_DELETE,
    CL_NONE_CONFIRM_FIND,
	CL_NONE_CONFIRM_TOSMS,
	CL_NONE_CONFIRM_ASSIGN,
    CL_NONE_SEND_NONE,
    CL_NONE_NONE_FIND,
    CL_NONE_NONE_SET,
    CL_NONE_NONE_MODIFY,
    CL_NONE_NONE_NEWHOLIDAY,
    CL_NONE_CONFIRM_NONE,
	CL_CANCEL_CONFIRM_NONE,
    CL_NONE_NONE_DELETE,
    CL_NONE_NONE_EDIT,
    CL_NONE_NONE_RELEASE,
    CL_NONE_NONE_CANCEL,
    CL_NONE_COMPLETE_NONE,
    CL_NONE_NONE_BOTTOMUP, 
	CL_NONE_NONE_ASSIGN,
	CL_NONE_SELECT_ASSIGN,
	CL_NONE_NONE_NEXT,
	CL_NONE_NONE_TOSMS,
    CL_NONE_NONE_NEWMAIL,
    CL_NONE_NONE_PASTE,
    CL_NONE_PASTE_NONE,
	CL_NONE_CONFIRM_PASTE,
    CL_NONE_MOVE_SELECT,
	CL_NONE_PLAY_DELETE,
    CL_NONE_OKUPDOWN_SEND,            
    CL_NONE_OKUPDOWN_NONE,            
	CL_MENU_LIST_PLAY,
    CL_MENU_NONE_NEXT,
    CL_MENU_NONE_SEND,
    CL_MENU_NONE_MODIFY,
    CL_MENU_NONE_DELETE,
    CL_MENU_NONE_ASSIGN,
    CL_MENU_NONE_SORT,
    CL_MENU_NONE_EDIT,
	CL_MENU_VIEW_EDIT,
    CL_MENU_NONE_FIND,
    CL_MENU_SELECT_FIND,
    CL_MENU_SELECT_ADD,
	CL_MENU_SELECT_NONE,
	CL_MENU_PBOOK_DELETE,
	CL_MENU_TOSMS_NONE,
    CL_MENU_NONE_ADD,
    CL_MENU_NONE_VIEW,
    CL_MENU_NONE_SAVE,
    CL_MENU_NONE_REPLY,
    CL_MENU_CONFIRM_LUNAR,
    CL_MENU_CONFIRM_JULIAN,
    CL_MENU_CONFIRM_NEWSCHED,
	CL_NONE_NONE_NEWALARM,
    CL_MENU_CONFIRM_NEWALARM,
	CL_MENU_CONFIRM_DELETE,
    CL_MENU_NONE_NONE,
    CL_MENU_NONE_PLAY,
    CL_MENU_VOLUME_PBOOK,
    CL_MENU_SELECT_NEWMAIL,
    CL_MENU_SELECT_WRITE,
	CL_NONE_CONTINUE_RESET,
    CL_VIEW_CONTINUE_RESET,
    CL_VIEW_SAVE_BOTTOMUP, /* REV */
	CL_MENU_SORISAM_DELETE,
	CL_MENU_SAVE_DELETE,
	CL_ALARM_CONFIRM_NEWSCHED,
	CL_DELETE_CONFIRM_NEWSCHED,
	CL_NONE_NONE_NEWSCHED,
	CL_NONE_NONE_NEWMEMO,
	CL_MENU_NONE_PLAYMUSIC,
	CL_MENU_NONE_STOP,
    CL_MENU_SELECT_SAVE,
	CL_MENU_CONFIRM_REPLY,
	CL_MENU_SELECT_SEND,
	CL_MENU_INPUT_SEND,
	CL_NONE_INPUT_SEND,
    CL_MENU_OKUPDOWN_SEND,
    CL_MENU_OKUP_SEND,
    CL_MENU_MOVE_SEND,
	CL_DELETE_NONE_REPLY,
    CL_DELETE_NONE_PROTECT,
    CL_DELETE_NONE_RELEASE,
    CL_FWD_REPLY_DELETE,
    CL_DELETE_NONE_NONE,
    CL_DELETE_NONE_SEND,
    CL_DELETE_SORISAM_SAVE,
    CL_DELETE_NONE_SAVE,
    CL_DELETE_NONE_PLAYMUSIC,
    CL_DELETE_NONE_STOP,
    CL_DELETE_NONE_VIEW,
    CL_DELETE_NONE_FWD,
	CL_DELETE_NONE_WRITE,
	CL_DELETE_NONE_MODIFY,
	CL_DELETE_SELECT_MODIFY,
    CL_DELETE_CONFIRM_NEWMEMO,
	CL_DELETE_CONFIRM_NEWHOLIDAY,
    CL_DELETE_CONFIRM_NONE,
    CL_DELETE_MODIFY_SEND,
    CL_DELETE_SEND_NONE,
    CL_DELETE_SORISAM_NONE,
	CL_DELETE_NONE_ASSIGN,
    CL_DELETE_COMPLETE_NONE,
	CL_DELETE_REC_PLAY,
    CL_SAVE_NONE_ALARM,
    CL_INIT_NONE_SAVE,
    CL_INIT_SELECT_SAVE,
    CL_FIND_NONE_SEND,
    CL_FIND_MOVE_SEND,
    CL_FIND_MOVEUP_SEND,
    CL_FIND_OKUPDOWN_SEND,           
    CL_FIND_OKUP_SEND,                
    CL_NONE_SELECT_VIEW,
    CL_NONE_SELECT_SAVE,
	CL_BEFOREVIEW_SELECT_SAVE,
	CL_RECEIPTBELL_MINUTELY_DELETE,
	CL_MENU_BEFOREVIEW_NONE,	
	CL_NONE_NONE_INIT,
	CL_WRITE_SEND_DELETE,		
	CL_WRITE_NONE_DELETE,
	CL_EDIT_SEND_DELETE,
	CL_EDIT_NONE_DELETE,
	CL_MENU_CONFIRM_NONE,
	CL_MENU_PBOOK_NONE,	
	CL_DELETE_PLAY_NONE,
	CL_DELETE_MOVE_NONE,
	CL_DELETE_REPLY_FWD,
	CL_DELETE_SEND_WRITE,
	CL_DELETE_SEND_EDIT,
	CL_DELETE_NONE_EDIT,
	CL_NONE_SELECT_DST,	
	CL_NONE_WRITE_NONE,	
	CL_NONE_NEWALARM_NONE,
	CL_NONE_NONE_NEWNAME,
	CL_MENU_SELECT_NEWNAME,
	CL_NONE_NONE_NEWGROUP,
	CL_MENU_SELECT_NEWGROUP,
    CL_NONE_SET_SAVE,
    CL_NONE_DISSOLVE_SAVE,
	CL_MENU_SORISAM_NONE,
	CL_MENU_SAVE_NONE,
	CL_MENU_PBOOK_CANCEL,
	CL_MENU_NONE_CANCEL,
	CL_NONE_CALL_CANCEL,
	CL_NONE_INPUT_SAVE,
	CL_DELETE_REPLY_NONE,
	CL_NONE_TOSMS_CANCEL,
	CL_NONE_CONFIRM_CANCEL,
	CL_DELETE_SELECT_SAVE,
	CL_NONE_SELECT_INDETAIL,
	CL_NONE_SELECT_CANCEL,	
    CL_NONE_CONFIRM_SAVE,	
	CL_CANCEL_CONFIRM_DOT,	
	CL_MENU_REPLY_FWD,		
	CL_MENU_SELECT_NEWARTICLE,
	CL_NONE_NONE_NEWARTICLE,
	CL_DELETE_WRITE_NONE,	
	CL_NONE_RECEIPT_NONE,	
	CL_MENU_SAVE_CANCEL,	
	CL_NONE_SETUP_NONE,		
	CL_NONE_DISSOLVE_NONE,	
	CL_DELETE_MODIFY_NONE,	
	CL_NONE_VOLUME_NEXT,
    CL_NONE_NONE_STOP,            
    CL_DELETE_NONE_CANCEL,  // by undersea
    CL_POINT_NONE_AC,
#ifdef FEATURE_WAVELET
	CL_PLAY_STOP_SAVE,				//yoursun 01/06/19
	CL_PLAY_NONE_SAVE,				//yoursun 01/09/03
	CL_PLAY_STOP_END,				//yoursun 01/09/03
	CL_PLAY_NONE_END,				//yoursun 01/09/03
    CL_NONE_NONE_END,				//yoursun 01/09/03
	CL_REPLY_STOP_SAVE,				//yoursun 01/10/16
	CL_REPLY_NONE_SAVE,				//yoursun 01/10/16	
	CL_REPLY_STOP_END,				//yoursun 01/10/16
	CL_REPLY_NONE_END,				//yoursun 01/10/16
#endif //FEATURE_WAVELET
#ifdef FEATURE_CHARACTER_BELL
    CL_FIND_SELECT_SAVE,            // bgjung 01/09/15
    CL_MENU_BEFOREVIEW_SET,         // bgjung 01/09/24 (or ADD)
    CL_MENU_NONE_SET,               // bgjung 01/11/01 
#endif // FEATURE_CHARACTER_BELL
	CL_CANCEL_CONFIRM_NEXT,   // by yhlee 01/09/25
	CL_CANCEL_SIGN_NONE,		 // by yhlee 01/09/25
    CL_MAX
};
typedef enum tagCOMMAND_LINE_E COMMAND_LINE_E;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern ROM CML_INFO_T SKY_CML;   

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_CML_H_ */
