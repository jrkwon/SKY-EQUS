/*
** ============================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_irda.h
**
** Description:
**     Common Phone to Phone IrDA Functions
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-23 woodstok     Created.
**
** ============================================================================
*/

#ifndef _SKA_IRDA_H_
#define _SKA_IRDA_H_

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
#define IRDA_FILE_BUFFER

/* rosa 2001-09-19 --> @why, what */
//#define IREX_MODEL_ID			"SK011601"  // B1
#define IREX_MODEL_ID			"SK011801"  // B2
/* <-- */


#define IREX_PING_WAIT_TIME		5000
#define IREX_SEND_AGAIN_TIME	400
#define IREX_SEND_AGAIN_TIME_FILE	1000
#define IREX_BOOK_WAIT_RESP_TIME 100
#define	IREX_MSG_WAIT_TIME		60000
#define IREX_BOOK_RESP_RETRY_TIME 2000  // 2Sec
/* rosa 2001-10-05 --> @why, what */
#define IREX_BOOK_MSG_WAIT_TIME 10000
/* <-- */
#define IREX_FILE_MSG_WAIT_TIME 20000

#define	NUM_PING_RETRIES			60 /* 60 seconds */
#define	NUM_MSG_RES_WAIT_RETRIES	25 

/*
	Packet structure
	1. Packet start 0xf0
	2. Packet body		0x1x
	  2.1 Length			2 bytes -> phisically 4 bytes
	    - The length of message data symbols
	  2.2 Message type		2 bytes -> phisically 4 bytes
	  2.3 Message data		0~MAX_MSG_LENGTH bytes -> 0~MAX_MSG_LENGTH*2
	  2.4 Row CRC16			2 bytes -> phisically 4 bytes
	  2.5 CRC30				4 bytes -> phisically 8 bytes
*/
#define	IREX_P_START_BYTE	0x74 /* 't' */
#define	IREX_P_END_BYTE		0x0c /* Carrige return */
#define	IREX_P_SYMBOL_BYTE	0x10

// Definition for old models(~B0)
#define  A7_UI_WIDE                 17
#define  C5_NV_MAX_DIAL_DIGITS		32        /* Maximum 32 digit number */    
#define  C5_NV_MAX_LTRS			    16      /* With up to 16-letter names */
#define	 C5_NV_MAX_MEMO			    32		

#define	 IREX_MAX_BLOCK_SIZE		512

#define IRDA_PERSONAL_NUMBER_LIMIT  5

#define MODEL_ID_LEN		8

#define A7_MM_MAX_FPATH_NAME    40
#define A7_MM_MAX_FILE_NAME		14

#define	IREX_USE_PING_S_ATT		0x0000
#define	IREX_SKIP_PING_S_ATT	0x0001

/* rosa 01-08-17 --> @why, what */
#define IRDA_PIC_X      21//13
#define IRDA_PIC_Y      12//2

#define IRDA_FRAME_X    4//3
#define IRDA_FRAME_Y    79//53

#define IRDA_STATE_X    8//70
#define IRDA_STATE_Y    81//55

#define IRDA_MESSAGE1_X 8//8
#define IRDA_MESSAGE1_Y 103//71
#define IRDA_MESSAGE2_X 8//8
#define IRDA_MESSAGE2_Y IRDA_MESSAGE1_Y + EQS_CharHeight((BYTE)' ')//86
/* <-- */

#define IRDA_FILE_BUFFER_SIZE   20 * 1024
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/***************************************************************************
	Model Number
****************************************************************************/

enum tag_IREX_MODEL_E {
    IREX_MODEL_C5,
    IREX_MODEL_B5,
    IREX_MODEL_B5_WAP,
    IREX_MODEL_C5_WAP,
    IREX_MODEL_A7,
    IREX_MODEL_A8,
    IREX_MODEL_B0,
    IREX_MODEL_B1,
    /* rosa 2001-09-19 --> @why, what */
    IREX_MODEL_B2,
    /* <-- */
    IREX_MAX_MODELS
};

typedef enum tag_IREX_MODEL_E IREX_MODEL_E;

/***************************************************************************
	Message Type of Server
****************************************************************************/
#define	IREX_SERVER_MASK	0x7fff
enum tag_IREX_SERVER_MSG_E {
    IREX_PING_S             = 0x0010,
    IREX_FINISHED_S         = 0x001f,
    IREX_MELODY_S           = 0x0020,
    IREX_ANI_S              = 0x0030,
    IREX_PHONEBOOK_S        = 0x0040,
    IREX_FILE_TRANS_S       = 0x0050,
    IREX_PHONEBOOK_SDIAL_S  = 0x0060
};

typedef enum tag_IREX_SERVER_MSG_E IREX_SERVER_MSG_E;

/***************************************************************************
	Message Type of Client
****************************************************************************/
#define	IREX_CLIENT_MASK	0x8000
enum tag_IREX_CLIENT_MSG_E {
    IREX_CRC_ERROR_C        = 0x8011,
    IREX_PING_C             = 0x8010,
    IREX_FINISHED_C         = 0x801f,
    IREX_MELODY_C           = 0x8020,
    IREX_ANI_C              = 0x8030,
    IREX_PHONEBOOK_C        = 0x8040,
    IREX_FILE_TRANS_C       = 0x8050,
    IREX_PHONEBOOK_SDIAL_C  = 0x8060
};

typedef enum tag_IREX_CLIENT_MSG_E IREX_CLIENT_MSG_E;

/***************************************************************************
	Result code of Response
****************************************************************************/

enum tag_IREX_RESULT_E {
    IREX_NONE_RES				= 0x0000,
    IREX_OK_RES					= 0x0001,
    IREX_NG_RES					= 0x0002,
    IREX_FINISHED_RES			= 0x000f,
    IREX_WAIT_FOR_USER_RES		= 0x0010,
    IREX_FILE_TRANS_FINISHED	= 0x0011,
    IREX_CONTINUE_MELODY_RES	= 0x0018,
    IREX_GROUP_CREATE_FAILURE	= 0x0031,
    IREX_ITEM_CREATE_FAILURE	= 0x0032,
    IREX_ITEM_SAME_NAME			= 0x0033,
    IREX_CONTINUE_PHONEBOOK_RES	= 0x0038,
    IREX_INVAILD_MELODY_RES		= 0x0040,
    IREX_INVAILD_ANI_RES		= 0x0041,
    IREX_ENTER_ANI_FILE_NAME	= 0x0042,
    IREX_NO_MEM_AVAIL			= 0x0043,
    IREX_ERR_FILE_VER           = 0x0046,
    IREX_ERR_FULE_FILE          = 0x0047,
    IREX_ERR_CLIENT             = 0x0048,
    IREX_ITEM_NOT_EXIST     	= 0x0050
};

typedef enum tag_IREX_RESULT_E IREX_RESULT_E;

/***************************************************************************
	Client State Machine Response Type
****************************************************************************/

enum tag_IREX_EVENT_E {
	SEND_MSG_E,
	GET_MSG_E,
	TIMEOUT_E,
	WAIT_RESPONSE_E
};

typedef enum tag_IREX_EVENT_E IREX_EVENT_E;

/***************************************************************************
	IREX State type
****************************************************************************/

enum tag_IREX_STATE_E {
    IREX_FAILURE   = -1,
    IREX_ON_GOING,
    IREX_SUCCESS,
    IREX_PING_FINISHED
};

typedef enum tag_IREX_STATE_E IREX_STATE_E;

/***************************************************************************
	Result code of GetMessage
****************************************************************************/

enum tag_IREX_RESULT_GETMSG_E {
    IREX_CRE_ERROR   = -1,
    IREX_NOT_COMPLETE,
    IREX_MSG_COMPLETE
};

typedef enum tag_IREX_RESULT_GETMSG_E IREX_RESULT_GETMSG_E;

/***************************************************************************
	Packet Receive State
****************************************************************************/

enum tag_IREX_PACKET_STATE_TYPE_E {
	START_S,
	LENGTH_S,
	MSG_TYPE_S,
	MSG_S,
	CHECKSUM_S,
	CRC_S
};

typedef enum tag_IREX_PACKET_STATE_TYPE_E IREX_PACKET_STATE_TYPE_E;

/***************************************************************************
	IrDA Receive Type
****************************************************************************/

enum tag_IREX_RECEIVE_TYPE_E {
	IRT_PHONEBOOK,
	IRT_PHONEBOOK_SDIAL,
	IRT_IMAGE,
	IRT_MELODY,
	IRT_PHOTO,
    IRT_READY
};

typedef enum tag_IREX_RECEIVE_TYPE_E IREX_RECEIVE_TYPE_E;

/***************************************************************************
	File Transfer Type
****************************************************************************/

enum tag_IREX_FILETRANS_E {
  	IREX_FT_RESERVED,
	IREX_FT_FILE_INF,
	IREX_FT_BLOCK,
	IREX_FT_LAST_BLOCK
};

typedef enum tag_IREX_FILETRANS_E IREX_FILETRANS_E;

enum tag_IREX_FILETYPE_E {
	IREX_FT_FILE_NAME,
	IREX_FT_MELODY,
	IREX_FT_ANI,
    IREX_FT_PHOTO
};

typedef enum tag_IREX_FILETYPE_E IREX_FILETYPE_E;

/***************************************************************************
	Result code of File Transfer
****************************************************************************/
enum tag_IREX_FTRSPCODE_TYPE_E {
	IREX_FT_NAK,
	IREX_FT_ACK,
	IREX_FT_FIN,
	IREX_FT_ERR,			// General Error
	IREX_FT_WAIT,			// Wait for response of client user answer
	IREX_FT_ERR_NO_MEM,		// Client need more memory to download the file 	
	IREX_FT_ERR_FILE_NUM,	// Exceed the allowed number of files
	IREX_FT_ERR_VER,		// Not compatible version
	IREX_FT_ESC = 0xff  // user has aborted transfer
};

typedef enum tag_IREX_FTRSPCODE_TYPE_E IREX_FTRSPCODE_TYPE_E;

/* rosa 01-08-06 --> @why, what */
enum tag_IRDA_BITMAP_E {
	IB_READY,
	IB_RECEIVE,
	IB_SEND
};
/* <-- */
#define MAX_MSG_LENGTH			(sizeof(IREX_MSG_TYPE_T))
#define MAX_PACKET_LENGTH		(MAX_MSG_LENGTH*2 + 4 + 4 + 8 + 3 + 4)
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/***************************************************************************
	IrDA Exchange message type : Server Messages
****************************************************************************/
typedef PACKED struct {
    BYTE Model[9];
	BYTE padding1[3]; // jwpark 01.08.30
} IREX_PING_S_TYPE_T;

typedef PACKED struct {
    WORD ResultCode;
	BYTE padding[2];  // jwpark 01.08.30
} IREX_FINISHED_S_TYPE_T;

/***************************************************************************
	MELODY : IM1000, IM1100, IM1100N, IM1200, IM1200N
****************************************************************************/
typedef PACKED struct {
  WORD		tone;           /* Tone/DTMF to generate        */
  WORD      duration_ms;    /* Duration in milliseconds     */
} old_snd_type;

#define		OLD_MYMELODY_DATA_MAX	127
/* Type to store MyMelody data                                               */
typedef	PACKED struct {
	BYTE		address;
	BYTE		tempo;
	old_snd_type	mymelody_snd_buf[OLD_MYMELODY_DATA_MAX];
} old_nv_mymelody_type;

typedef PACKED struct {
	WORD				Index;
	old_nv_mymelody_type	Data;
} IREX_OLD_MELODY_S_TYPE_T;

/***************************************************************************
	ANIMATION : IM1000, IM1100, IM1100N, IM1200, IM1200N
****************************************************************************/
#define	 C5_MYANI_DATA_MAX		612
#define	 B5_MYANI_DATA_MAX		768
#define	 B5_WAP_MYANI_DATA_MAX	1000
#define	 C5_WAP_MYANI_DATA_MAX	1000

/* Type to store MyAni data                                               */
typedef	PACKED struct {
	WORD		address;
	BYTE		type;
	BYTE		data[C5_MYANI_DATA_MAX];
} c5_nv_myani_type;

/* Type to store MyAni data                                               */
typedef	PACKED struct {
	WORD		address;
	BYTE		type;
	BYTE		data[B5_MYANI_DATA_MAX];
} b5_nv_myani_type;

typedef	PACKED struct {
	WORD		address;
	BYTE		type;
	BYTE		data[B5_WAP_MYANI_DATA_MAX];
} b5_wap_nv_myani_type;

typedef	PACKED struct {
	WORD		address;
	BYTE		type;
	BYTE		data[C5_WAP_MYANI_DATA_MAX];
} c5_wap_nv_myani_type;

typedef PACKED struct {
	WORD					Index;
	c5_nv_myani_type		Data;
	BYTE					padding1[3];	// jwpark 01.08.30
} IREX_C5_ANI_S_TYPE_T;

typedef	PACKED struct {
	WORD					Index;
	b5_nv_myani_type		Data;
	BYTE					padding1[3];	// jwpark 01.08.30
} IREX_B5_ANI_S_TYPE_T;

typedef	PACKED struct {
	WORD					Index;
	b5_wap_nv_myani_type	Data;
	BYTE					padding1[3];	// jwpark 01.08.30
} IREX_B5_WAP_ANI_S_TYPE_T;

typedef	PACKED struct {
	WORD					Index;
	c5_wap_nv_myani_type	Data;
	BYTE					padding1[3];	// jwpark 01.08.30
} IREX_C5_WAP_ANI_S_TYPE_T;

/***************************************************************************
	PHONEBOOK : IM1000, IM1100, IM1100N, IM1200, IM1200N
****************************************************************************/

typedef PACKED struct {
    INT       address; /* Storage address */
    BYTE      status;                       /* flag bits */
    BYTE      reserved1;                   /* Number of digits */
    BYTE      email[C5_NV_MAX_DIAL_DIGITS];   /* Array of digits */
    BYTE      exist;
    BYTE      group;                        /* group 0: 그룹[기타] */
    BYTE      multi_num_digits[4];          /* 0: 휴대폰 1:집 2:사무실 3:호출기 */
    BYTE      multi_digits[4][C5_NV_MAX_DIAL_DIGITS];   

    BYTE      anniv_contidx; /* 기념일 내용 item index 0:없음 1:생일 2: 결혼 3:첫만남 4:편집 */
    BYTE      anniv_month;   /* 기념일 달 */
    BYTE      anniv_day;     /* 기념일 일 */
    BYTE		anniv_lunar;   /* 기념일 0: 양력 1:음력 */
    BYTE      anniv_content[C5_NV_MAX_LTRS]; /* anniv_contidx가 4인 경우: 편집된 내용 */

    BYTE      memo[C5_NV_MAX_MEMO];
    BYTE      letters[C5_NV_MAX_LTRS];         /* Name associated with number */
    BYTE      sync_flag;           /* Cmodify flag for WAP sync */
    DWORD     tstamp;              /* Time stamp for WAP sync : elasped time since 1980.1.6*/
    BYTE      padding[11];      /* make size of nv_dial_type 256 byte */
} c5_nv_dial_type;

typedef	PACKED struct {
	BYTE			GroupName[A7_UI_WIDE+1];
	c5_nv_dial_type	Data;
	BYTE			padding1[2];	// jwpark 01.08.30
} IREX_C5_PHONEBOOK_S_TYPE_T;

/***************************************************************************
	PHONEBOOK : IM2000 ~ B0
****************************************************************************/

typedef NV_DIAL_TYPE    a7_nv_dial_type;

typedef	PACKED struct {
	BYTE			GroupName[A7_UI_WIDE+1];
	a7_nv_dial_type	Data;
	BYTE			padding1[1];	// jwpark 01.08.30
} IREX_A7_PHONEBOOK_S_TYPE_T;

typedef  PACKED struct
{
    BYTE                address;                      /* Storage address */
    INT                 dial_address;                 /* address of ponbook item */
    BYTE                whereto;                      /* 0: 휴대폰 1:집 2:사무실 3:호출기 */
	BYTE			    GroupName[A7_UI_WIDE+1];
	a7_nv_dial_type 	DialData;
} IREX_A7_PHONEBOOK_SDIAL_S_TYPE_T;

/***************************************************************************
	PHONEBOOK : B1~
****************************************************************************/
typedef	PACKED struct {
    BYTE		bType;
    BYTE		szNumber[PB_MAX_DIGIT_LENGTH+1];
	BYTE		padding1[2];	// jwpark 01.08.30
} IREX_PHONENUMBER_TYPE_T;

typedef	PACKED struct {
    BYTE       bType;
    BYTE       szContent[MAX_ANNIV_NAME+1];
    UINT8      nDay;
    UINT8      nMonth;   
    BOOL       fLunar;
	BYTE       padding1[1];		// jwpark 01.08.30	
    WORD       wYear;
} IREX_ANNIV_TYPE_T;

typedef	PACKED struct {
    BYTE    szGroupName[MAX_GROUP_NAME+1];
    BYTE    szMultiGroup[MAX_GROUP_NAME+1];
    BYTE    szName[MAX_BOOK_NAME+1];
    BYTE    aszEmail[PERSONAL_EMAIL_LIMIT][PB_MAX_STRING_LENGTH+1];
	BYTE	padding1[3];		// jwpark 01.08.30

    IREX_PHONENUMBER_TYPE_T Number[IRDA_PERSONAL_NUMBER_LIMIT];
    BYTE    szMemo[PB_MAX_MEMO_LENGTH+1];
	BYTE	padding2[3];		// jwpark 01.08.30
    IREX_ANNIV_TYPE_T       AnnivData[PERSONAL_ANNIV_LIMIT];
    BYTE    szPadding;
	BYTE	padding3[3];		// jwpark 01.08.30
} IREX_PHONEBOOK_S_TYPE_T;

typedef	PACKED struct {
    UINT8       nAddress;
	BYTE		padding1[3];	// jwpark 01.08.30
    IREX_PHONENUMBER_TYPE_T Number;
	IREX_PHONEBOOK_S_TYPE_T	DialData;
} IREX_PHONEBOOK_SDIAL_S_TYPE_T;

/***************************************************************************
	IREX_FILE_TRANS_S  : IM2000
****************************************************************************/

typedef	PACKED struct {
	WORD					Type;
	BYTE					FileName[A7_MM_MAX_FPATH_NAME];
	BYTE					padding1[2];	// jwpark 01.08.30
	DWORD					FileSize;
	WORD					BlockSize;
	WORD					nBlocks;
} IREX_FT_FILEINF_TYPE_T;


typedef	PACKED struct {
	WORD				Type;
	WORD				Seq;
	WORD				nRest;
	BYTE				padding1[2];	// jwpark 01.08.30
	PACKED union {
		IREX_FT_FILEINF_TYPE_T	Inf;
		BYTE					Block[IREX_MAX_BLOCK_SIZE];
	} Body;
} IREX_FILE_TRANS_S_TYPE_T;

/***************************************************************************
	IrDA Exchange message type : Client Messages
****************************************************************************/
typedef	PACKED struct {
	BYTE Model[9];
	BYTE padding1[3];	// jwpark 01.08.30	
} IREX_PING_C_TYPE_T;

typedef	PACKED struct {
	WORD ResultCode;
	BYTE padding1[2];	// jwpark 01.08.30	
} IREX_PHONEBOOK_C_TYPE_T;

typedef	PACKED struct {
	WORD ResultCode;
	BYTE padding1[2];	// jwpark 01.08.30	
} IREX_PHONEBOOK_SDIAL_C_TYPE_T;

typedef	PACKED struct {
	WORD				RspCode;
	WORD				SeqRsp;	
} IREX_FILE_TRANS_C_TYPE_T;

typedef	PACKED struct {
	WORD ResultCode;
	BYTE padding1[2];	// jwpark 01.08.30	
} IREX_MELODY_C_TYPE_T;

typedef	PACKED struct {
	WORD ResultCode;
	BYTE padding1[2];	// jwpark 01.08.30	
} IREX_ANI_C_TYPE_T;

typedef	PACKED struct {
	UINT16   MsgType;
	BYTE padding1[2];	// jwpark 01.08.30	
	PACKED union {
        // Server -> Client
		IREX_PING_S_TYPE_T			    Ping_s;
		IREX_FINISHED_S_TYPE_T		    Finished_s;

		IREX_OLD_MELODY_S_TYPE_T	    old_Melody_s;

		IREX_C5_ANI_S_TYPE_T		    c5_Ani_s;
		IREX_B5_ANI_S_TYPE_T		    b5_Ani_s;
		IREX_B5_WAP_ANI_S_TYPE_T	    b5_wap_Ani_s;
		IREX_C5_WAP_ANI_S_TYPE_T	    c5_wap_Ani_s;

		IREX_C5_PHONEBOOK_S_TYPE_T	    c5_PhoneBook_s;
		IREX_A7_PHONEBOOK_S_TYPE_T	    a7_PhoneBook_s;
        IREX_A7_PHONEBOOK_SDIAL_S_TYPE_T	a7_PhoneBook_SDial_s;

        IREX_PHONEBOOK_S_TYPE_T         PhoneBook_s;
        IREX_PHONEBOOK_SDIAL_S_TYPE_T	PhoneBook_SDial_s;

		IREX_FILE_TRANS_S_TYPE_T	    FileTrans_s;

        // Client -> Server
		IREX_PING_C_TYPE_T			    Ping_c;
		IREX_MELODY_C_TYPE_T		    Melody_c;
		IREX_ANI_C_TYPE_T			    Ani_c;
		IREX_PHONEBOOK_C_TYPE_T		    PhoneBook_c;
        IREX_PHONEBOOK_SDIAL_C_TYPE_T   PhoneBook_SDial_c;
		IREX_FILE_TRANS_C_TYPE_T	    FileTrans_c;
	} Msg;
} IREX_MSG_TYPE_T;
/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

extern BOOL    g_fIrDAPortForPC;

extern IREX_MSG_TYPE_T	gIrex_RxMsg, gIrex_TxMsg;
//extern IREX_MODEL_E		gIrex_ModelIndex;

extern const BOOL afSendMelody[IREX_MAX_MODELS];
extern const BOOL afReceiveMelody[IREX_MAX_MODELS];
extern const BOOL afSendAni[IREX_MAX_MODELS];
extern const BOOL afReceiveAni[IREX_MAX_MODELS];
extern const BOOL afSendPhoto[IREX_MAX_MODELS];
extern const BOOL afReceivePhoto[IREX_MAX_MODELS];

extern IREX_RECEIVE_TYPE_E	gIrDACommType;
extern BOOL	gfPhoneBookOverwriteAll;
extern BOOL	gfPhoneBookSkipAll;

extern HTIMER  gHTimer;

extern IREX_FILETYPE_E gTransferType;
extern HFILE           gSendFile;
extern HFILE           gRecvFile;
extern BYTE            *g_pSendFileName;
extern BYTE            *g_pSendPhotoName;
extern BYTE            g_ReceiveFileName[SKY_FILE_EXT_NAME_MAX_LENGTH];

extern BYTE	           g_GroupName[MAX_GROUP_NAME+1];
extern BYTE	           g_Letters[MAX_BOOK_NAME+1];

extern BYTE   irex_SendingModelID[MODEL_ID_LEN+1];
extern BYTE   irex_RxedModelID[MODEL_ID_LEN+1];

#ifdef IRDA_FILE_BUFFER
extern BYTE            g_RecvFileBuffer[IRDA_FILE_BUFFER_SIZE];   // 10KB
extern UINT16          g_nBufferOffset;
extern UINT16          g_nFileOffset;
#endif

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
    
extern IREX_RESULT_E SKY_API SKY_IrDAClientStateMachine(IREX_EVENT_E Event);
extern IREX_STATE_E SKY_API SKY_IrDAServerStateMachine(IREX_EVENT_E Event,
                                                 WORD Attrib,
                                                 WORD *pResultCode);
extern IREX_RESULT_E SKY_API SKY_ProcessFileTransferMsg(void);
extern IREX_RESULT_E SKY_API SKY_ProcessFileSend(IREX_STATE_E Ret, BOOL fStart);

extern IREX_RESULT_E SKY_API SKY_ProcessPhoneBookMsg(BOOL fOverWrite);
extern IREX_RESULT_E SKY_API SKY_ProcessSDialMsg(BOOL fOverWrite);

extern void SKY_API SKY_IrDACloseFile(void);
extern void SKY_API SKY_IrDASendESCtoServer(void);

extern void SKY_API SKY_CloseIrDAForPC(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKA_IRDA_H_ */
