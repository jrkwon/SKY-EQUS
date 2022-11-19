/*
** ============================================================================
**
** Project: 
**     Athens
**
** File:
**     
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
** 00-07-27 woodstok   Created.
**
** ============================================================================
*/

#ifndef _EQS_SFNT_H_
#define _EQS_SFNT_H_

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

/* ---------------------------------- 0x81 --------------------------------- */

#define	SF_FACE                 "\x81\xA1"
#define	SF_SNOWMAN              "\x81\xA2"
#define	SF_CAR                  "\x81\xA3"
#define	SF_TREE                 "\x81\xA4"
#define	SF_FLOWER               "\x81\xA5"
#define	SF_SMILE                "\x81\xA6"
#define	SF_MUSTACHE_MAN         "\x81\xA7"
#define	SF_SCISSORS             "\x81\xA8"
#define	SF_EGG                  "\x81\xA9"
#define	SF_SNAIL                "\x81\xAA"
#define	SF_DUCK                 "\x81\xAB"
#define	SF_FOX                  "\x81\xAC"
#define	SF_SMILE2               "\x81\xAD"
#define	SF_UP_FINGER            "\x81\xAE"
#define	SF_PC                   "\x81\xAF"
#define	SF_TV                   "\x81\xB0"
#define	SF_SWORD                "\x81\xB1"
#define	SF_MAGIC_BOX            "\x81\xB2"
#define	SF_RABBIT               "\x81\xB3"
#define	SF_EYE                  "\x81\xB4"
#define	SF_DISKETTE             "\x81\xB5"
#define	SF_NOTE                 "\x81\xB6"
#define	SF_KEY                  "\x81\xB7"
#define	SF_ENVELOPE             "\x81\xB8"
#define	SF_HEART                "\x81\xB9"
#define	SF_RIGHT_ARROW          "\x81\xBA"
#define	SF_8TH_NOTE             "\x81\xBB"
#define	SF_16TH_NOTE            "\x81\xBC"
#define	SF_FORK_KNIFE           "\x81\xBD"
#define	SF_LOCK                 "\x81\xBE"
#define	SF_UMBRELLA             "\x81\xBF"
#define	SF_PENCIL               "\x81\xC0"
#define	SF_UP_LEFT_PENCIL       "\x81\xC1"
#define	SF_DESK_CLOCK           "\x81\xC2"
#define	SF_BOAT                 "\x81\xC3"
#define	SF_CHIKEN               "\x81\xC4"
#define	SF_BULB                 "\x81\xC5"
#define	SF_APPLE                "\x81\xC6"
#define	SF_FISH                 "\x81\xC7"
#define	SF_LIPS                 "\x81\xC8"
#define	SF_THUNDER              "\x81\xC9"
#define	SF_PUPPY                "\x81\xCA"
#define	SF_CUTTLE_FISH          "\x81\xCB"
#define	SF_GLASSES              "\x81\xCC"
#define	SF_BALL                 "\x81\xCD"
#define	SF_JUICE                "\x81\xCE"
#define	SF_TEA                  "\x81\xCF"
#define	SF_MONSTER              "\x81\xD0"
#define	SF_RUNNER               "\x81\xD1"
#define	SF_STOP                 "\x81\xD2"
#define	SF_SKULL                "\x81\xD3"
#define	SF_DUNG                 "\x81\xD4"
#define	SF_FEMAIL               "\x81\xD5"
#define	SF_MAIL                 "\x81\xD6"
#define	SF_RIGHT_FINGER         "\x81\xD7"
#define	SF_LEFT_ARROW           "\x81\xD8"
#define	SF_DOWN_RIGHT_PENCIL    "\x81\xD9"

/* reserved ---- >
#define	SF_                 "\x81\xDA"
#define	SF_                 "\x81\xDB"
#define	SF_                 "\x81\xDC"
#define	SF_                 "\x81\xDD"
#define	SF_                 "\x81\xDE"
#define	SF_                 "\x81\xDF"
 <----- */
/* 12 궁도 --- > */
#define	SF_STAR_1             "\x81\xE0"
#define	SF_STAR_2             "\x81\xE1"
#define	SF_STAR_3             "\x81\xE2"
#define	SF_STAR_4             "\x81\xE3"
#define	SF_STAR_5             "\x81\xE4"
#define	SF_STAR_6             "\x81\xE5"
#define	SF_STAR_7             "\x81\xE6"
#define	SF_STAR_8             "\x81\xE7"
#define	SF_STAR_9             "\x81\xE8"
#define	SF_STAR_10            "\x81\xE9"
#define	SF_STAR_11            "\x81\xEA"
#define	SF_STAR_12            "\x81\xEB"


#define	SF_CYCLE		    "\x81\xEC" // 반복 아이콘
#define	SF_DATE             "\x81\xED" // 날짜 아이콘
#define	SF_TIME             "\x81\xEE" // 시간 아이콘
#define	SF_TITLE            "\x81\xEF" // 제목 아이콘
#define	SF_ALL_DAY          "\x81\xF0" // 종일 아이콘
#define	SF_ALARM            "\x81\xF1" // 알람 아이콘 


#define	SF_DATE_TITLE       "\x81\xF2" // 달력에 타이틀...


#define	SF_AUTORPLY1_2      "\x81\xF3"//자동응답
#define	SF_AUTORPLY2_2      "\x81\xF4"//자동응답
#define	SF_NEW_AUTORPLY1_2  "\x81\xF5"//새 자동응답
#define	SF_NEW_AUTORPLY2_2  "\x81\xF6"//새 자동응답
#define	SF_VOICE_REC1_2     "\x81\xF7"//음성녹음
#define	SF_VOICE_REC2_2     "\x81\xF8"//음성녹음

/* reserved ---- >
#define	SF_                 "\x81\xF9"
#define	SF_                 "\x81\xFA"
#define	SF_                 "\x81\xFB"
#define	SF_                 "\x81\xFC"
#define	SF_                 "\x81\xFD"
#define	SF_                 "\x81\xFE"
#define	SF_                 "\x81\xFF"
 <--------- */

/* ---------------------------------- 0x82 --------------------------------- */

#define	SF_REPEAT_DAY           "\x82\xA0"
#define	SF_REPEAT_MONTH         "\x82\xA1"
#define	SF_REPEAT_WEEK          "\x82\xA2"
#define	SF_START_TIME1_4        "\x82\xA3"
#define	SF_START_TIME2_4        "\x82\xA4"
#define	SF_START_TIME3_4        "\x82\xA5"
#define	SF_START_TIME4_4        "\x82\xA6"
#define	SF_END_TIME1_4          "\x82\xA7"
#define	SF_END_TIME2_4          "\x82\xA8"
#define	SF_END_TIME3_4          "\x82\xA9"
#define	SF_END_TIME4_4          "\x82\xAA"
#define	SF_ALARM_TIME1_4        "\x82\xAB"
#define	SF_ALARM_TIME2_4        "\x82\xAC"
#define	SF_ALARM_TIME3_4        "\x82\xAD"
#define	SF_ALARM_TIME4_4        "\x82\xAE"
#define	SF_SCHEDULE1_2          "\x82\xAF"
#define	SF_SCHEDULE2_2          "\x82\xB0"
#define	SF_DAILY1_2             "\x82\xB1"
#define	SF_DAILY2_2             "\x82\xB2"
#define	SF_WEEKLY1_2            "\x82\xB3"
#define	SF_WEEKLY2_2            "\x82\xB4"
#define	SF_MONTHLY1_2           "\x82\xB5"
#define	SF_MONTHLY2_2           "\x82\xB6"
#define	SF_GROUP_TEXT           "\x82\xB7"
#define	SF_NAME_TEXT            "\x82\xB8"
#define	SF_ADDRESS_TEXT         "\x82\xB9"
#define	SF_ANNIVERSARY_TEXT     "\x82\xBA"
#define	SF_INFORMATION          "\x82\xBB"
#define	SF_MEMO_TEXT            "\x82\xBC"
#define	SF_MONTH_DAY_TEXT       "\x82\xBD"
#define	SF_CONTENT_TEXT         "\x82\xBE"
 
#define	SF_ADDRESS              "\x82\xBF"//////////////추가

#define	SF_REPLY_NUMBER1_4      "\x82\xC0"
#define	SF_REPLY_NUMBER2_4      "\x82\xC1"
#define	SF_REPLY_NUMBER3_4      "\x82\xC2"
#define	SF_REPLY_NUMBER4_4      "\x82\xC3"
#define	SF_CONNECT_URL1_4       "\x82\xC4"
#define	SF_CONNECT_URL2_4       "\x82\xC5"
#define	SF_CONNECT_URL3_4       "\x82\xC6"
#define	SF_CONNECT_URL4_4       "\x82\xC7"
#define	SF_MESSAGE_CONTENT1_5   "\x82\xC8"
#define	SF_MESSAGE_CONTENT2_5   "\x82\xC9"
#define	SF_MESSAGE_CONTENT3_5   "\x82\xCA"
#define	SF_MESSAGE_CONTENT4_5   "\x82\xCB"
#define	SF_MESSAGE_CONTENT5_5   "\x82\xCC"
#define	SF_WRITE_DATE1_4        "\x82\xCD"
#define	SF_WRITE_DATE2_4        "\x82\xCE"
#define	SF_WRITE_DATE3_4        "\x82\xCF"
#define	SF_WRITE_DATE4_4        "\x82\xD0"
#define	SF_SENT_DATE1_4         "\x82\xD1"
#define	SF_SENT_DATE2_4         "\x82\xD2"
#define	SF_SENT_DATE3_4         "\x82\xD3"
#define	SF_SENT_DATE4_4         "\x82\xD4"
#define	SF_RECEIVE_DATE1_4       "\x82\xD5"
#define	SF_RECEIVE_DATE2_4      "\x82\xD6"
#define	SF_RECEIVE_DATE3_4      "\x82\xD7"
#define	SF_RECEIVE_DATE4_4      "\x82\xD8"
#define	SF_ORIGINATE_CALL1_2    "\x82\xD9"
//#define	SF_ORIGINATE_CALL2_2    "\x82\xDA"
#define	SF_ALERT_CALL1_2        "\x82\xDB"
//#define	SF_ALERT_CALL2_2        "\x82\xDC"
//#define	SF_ORIGINATE_CALL2_2    "\x82\xDD"
//#define	SF_ALERT_CALL1_2        "\x82\xDE"
//#define	SF_ALERT_CALL2_2        "\x82\xDF"
#define	SF_MISSED_CALL1_2       "\x82\xE0"
//#define	SF_MISSED_CALL2_2       "\x82\xE1"
#define	SF_MESSAGE_SENT1_2      "\x82\xE2"
//#define	SF_MESSAGE_SENT2_2      "\x82\xE3"
#define	SF_MESSAGE_RECEIVED1_2  "\x82\xE4"
//#define	SF_MESSAGE_RECEIVED2_2  "\x82\xE5"
#define	SF_READ_MESSAGE         "\x82\xE6"
#define	SF_UNREAD_MESSAGE       "\x82\xE7"
#define	SF_UNREAD_PROT_MESSAGE  "\x82\xE8"
#define	SF_UNREAD_PRI_MESSAGE   "\x82\xE9"
#define	SF_SECRET1_2            "\x82\xEA"
#define	SF_SECRET2_2            "\x82\xEB"
#define	SF_SOLAR                "\x82\xEC"
#define	SF_LUNAR                "\x82\xED"
#define	SF_NTOP1_4              "\x82\xEE"
#define	SF_NTOP2_4              "\x82\xEF"
#define	SF_NTOP3_4              "\x82\xF0"
#define	SF_NTOP4_4              "\x82\xF1"

////////////////////////////////////////////////////////첨가 /////////////////////
#define	SF_READ_PROT_MESSAGE    "\x82\xF2"
#define	SF_READ_PRI_MESSAGE     "\x82\xF3"
#define SF_READ_PRI_PROTECT     "\x82\xF4"
#define SF_UNREAD_PRI_PROTECT   "\x82\xF5"
#define	SF_NEW_DAY			    "\x82\xF6" //////////////////////////////////////
#define	SF_NEW_MONTH		    "\x82\xF7" // 새로 바뀐 일 월 주 반복 설정 아이콘 
#define	SF_NEW_WEEK			    "\x82\xF8" //////////////////////////////////////
#define	SF_WHISPER1_2		    "\x82\xF9" // 속삭임
#define	SF_WHISPER2_2			"\x82\xFA" // 속삭임 
/////////////////////////////////////////////////////////////////////////////////

#define	SF_LINE_BUSY_RECEIPT1_2 "\x82\xFB" // 통화중 착신 
//#define	SF_LINE_BUSY_RECEIPT2_2 "\x82\xFC" // 통화중 착신


#define	SF_AUTO_RECEIPT1_3	   "\x82\xFD" //자동착신
#define	SF_AUTO_RECEIPT2_3	   "\x82\xFE" //자동착신
#define	SF_AUTO_RECEIPT3_3     "\x82\xFF" //자동착신
/* reserved --->
 <---- */

/* ---------------------------------- 0x83 --------------------------------- */

#define	SF_A                  "\x83\xA0"
#define	SF_B                  "\x83\xA1"
#define	SF_C                  "\x83\xA2"
#define	SF_D                  "\x83\xA3"
#define	SF_E                  "\x83\xA4"
#define	SF_F                  "\x83\xA5"
#define	SF_G                  "\x83\xA6"
#define	SF_H                  "\x83\xA7"
#define	SF_I                  "\x83\xA8"
#define	SF_J                  "\x83\xA9"
#define	SF_K                  "\x83\xAA"
#define	SF_L                  "\x83\xAB"
#define	SF_M                  "\x83\xAC"
#define	SF_N                  "\x83\xAD"
#define	SF_O                  "\x83\xAE"
#define	SF_P                  "\x83\xAF"
#define	SF_Q                  "\x83\xB0"
#define	SF_R                  "\x83\xB1"
#define	SF_S                  "\x83\xB2"
#define	SF_T                  "\x83\xB3"
#define	SF_U                  "\x83\xB4"
#define	SF_V                  "\x83\xB5"
#define	SF_W                  "\x83\xB6"
#define	SF_X                  "\x83\xB7"
#define	SF_Y                  "\x83\xB8"
#define	SF_Z                  "\x83\xB9"
#define	SF_a                  "\x83\xBA"
#define	SF_b                  "\x83\xBB"
#define	SF_c                  "\x83\xBC"
#define	SF_d                  "\x83\xBD"
#define	SF_e                  "\x83\xBE"
#define	SF_f                  "\x83\xBF"
#define	SF_g                  "\x83\xC0"
#define	SF_h                  "\x83\xC1"
#define	SF_i                  "\x83\xC2"
#define	SF_j                  "\x83\xC3"
#define	SF_k                  "\x83\xC4"
#define	SF_l                  "\x83\xC5"
#define	SF_m                  "\x83\xC6"
#define	SF_n                  "\x83\xC7"
#define	SF_o                  "\x83\xC8"
#define	SF_p                  "\x83\xC9"
#define	SF_q                  "\x83\xCA"
#define	SF_r                  "\x83\xCB"
#define	SF_s                  "\x83\xCC"
#define	SF_t                  "\x83\xCD"
#define	SF_u                  "\x83\xCE"
#define	SF_v                  "\x83\xCF"
#define	SF_w                  "\x83\xD0"
#define	SF_x                  "\x83\xD1"
#define	SF_y                  "\x83\xD2"
#define	SF_z                  "\x83\xD3"
#define	SF_EXCLAMATION        "\x83\xD4"
#define	SF_DOUBLE_QUOTATION   "\x83\xD5"
#define	SF_POUND              "\x83\xD6"
#define	SF_DOLLAR             "\x83\xD7"
#define	SF_PERCENT            "\x83\xD8"
#define	SF_AND                "\x83\xD9"
#define	SF_QUOTATION          "\x83\xDA"
#define	SF_LEFT_PARENTHESIS   "\x83\xDB"
#define	SF_RIGHT_PARENTHESIS  "\x83\xDC"
#define	SF_STAR               "\x83\xDD"
#define	SF_PLUS               "\x83\xDE"
#define	SF_COMMA              "\x83\xDF"
#define	SF_MINUS              "\x83\xE0"
#define	SF_PERIOD             "\x83\xE1"
#define	SF_SLASH              "\x83\xE2"
#define	SF_COLON              "\x83\xE3"
#define	SF_SEMI_COLON         "\x83\xE4"
#define	SF_LESS_THAN          "\x83\xE5"
#define	SF_EQUAL              "\x83\xE6"
#define	SF_GREATER_THAN       "\x83\xE7"
#define	SF_QUESTION           "\x83\xE8"
#define	SF_AT                 "\x83\xE9"
#define	SF_LEFT_BRACKET       "\x83\xEA"
#define	SF_WON                "\x83\xEB"
#define	SF_RIGHT_BRACKET      "\x83\xEC"
#define	SF_PIVOT              "\x83\xED"
#define	SF_UNDERBAR           "\x83\xEE"
#define	SF_LEFT_QUOTATION     "\x83\xEF"
#define	SF_LEFT_BRACE         "\x83\xF0"
#define	SF_OR                 "\x83\xF1"
#define	SF_RIGHT_BRACE        "\x83\xF2"
#define	SF_TILT               "\x83\xF3"
#define	SF_TRIANGLE           "\x83\xF4"

#define	SF_MAKE_MSG1_5		  "\x83\xF5" //SMS 메시지 작성
#define	SF_MAKE_MSG2_5		  "\x83\xF6" //SMS 메시지 작성
#define	SF_MAKE_MSG3_5		  "\x83\xF7" //SMS 메시지 작성
#define	SF_MAKE_MSG4_5		  "\x83\xF8" //SMS 메시지 작성
#define	SF_MAKE_MSG5_5		  "\x83\xF9" //SMS 메시지 작성


#define	SF_LETTER_CLASS1_4    "\x83\xFA" //편지종류 
#define	SF_LETTER_CLASS2_4    "\x83\xFB" //편지종류
#define	SF_LETTER_CLASS3_4    "\x83\xFC" //편지종류
#define	SF_LETTER_CLASS4_4    "\x83\xFD" //편지종류

#define	SF_ETC_SYMBOL         "\x83\xFE"

#define	SF_DST			      "\x83\xFF" //DST
/* reserved ---> *
 <--- */

/* ---------------------------------- 0x84 --------------------------------- */

#define	SF_MOON                "\x84\xA0"
#define	SF_SUN                 "\x84\xA1"
#define	SF_CELLULAR            "\x84\xA2"
#define	SF_HOME                "\x84\xA3"
#define	SF_OFFICE              "\x84\xA4"
#define	SF_PAGER               "\x84\xA5"
#define	SF_FAX                 "\x84\xA6"
#define	SF_CAKE                "\x84\xA7"
#define	SF_COCKTAIL            "\x84\xA8"
#define	SF_LOVE                "\x84\xA9"
#define	SF_100TH_DAY_ANNIVERSARY  "\x84\xAA"
#define	SF_EMAIL               "\x84\xAB"
#define	SF_BOOK                "\x84\xAC"
#define	SF_GROUP               "\x84\xAD"
#define	SF_GROUP_GRAY          "\x84\xAE"
#define	SF_123                 "\x84\xAF"
#define	SF_MEMO                "\x84\xB0"
#define	SF_CAMERA              "\x84\xB1"
#define	SF_SCHEDULE            "\x84\xB2"
#define	SF_SCHEDULE_ALARM      "\x84\xB3"
#define	SF_VIEW_CONTENT1_2     "\x84\xB4"
#define	SF_VIEW_CONTENT2_2     "\x84\xB5"

#define SF_CNAP                "\x84\xBB"
#define	SF_PHONEBOOK           "\x84\xBC"
#define	SF_RING                "\x84\xBE"

/* reserved ---> 
#define	SF_   "\x84\xB6"
#define	SF_   "\x84\xB7"
#define	SF_   "\x84\xB8"
#define	SF_   "\x84\xB9"
#define	SF_   "\x84\xBA"
#define	SF_   "\x84\xBD"

#define	SF_   "\x84\xBF"
 <---- */
#define	SF_CELLULAR1           "\x84\xC0"
#define	SF_CELLULAR2           "\x84\xC1"
#define	SF_CELLULAR3           "\x84\xC2"
#define	SF_CELLULAR4           "\x84\xC3"
#define	SF_CELLULAR5           "\x84\xC4"
#define	SF_HOME1               "\x84\xC5"
#define	SF_HOME2               "\x84\xC6"
#define	SF_HOME3               "\x84\xC7"
#define	SF_HOME4               "\x84\xC8"
#define	SF_HOME5               "\x84\xC9"
#define	SF_OFFICE1             "\x84\xCA"
#define	SF_OFFICE2             "\x84\xCB"
#define	SF_OFFICE3             "\x84\xCC"
#define	SF_OFFICE4             "\x84\xCD"
#define	SF_OFFICE5             "\x84\xCE"
#define	SF_PAGER1              "\x84\xCF"
#define	SF_PAGER2              "\x84\xD0"
#define	SF_PAGER3              "\x84\xD1"
#define	SF_PAGER4              "\x84\xD2"
#define	SF_PAGER5              "\x84\xD3"
#define	SF_FAX1                "\x84\xD4"
#define	SF_FAX2                "\x84\xD5"
#define	SF_FAX3                "\x84\xD6"
#define	SF_FAX4                "\x84\xD7"
#define	SF_FAX5                "\x84\xD8"
/////////////////////////////////////////////4월 12일 추가  
#define	SF_SCHEDULE_2		   "\x84\xD9" /////////////////////알람 설정 안된 일정 수정본 
#define	SF_SCHEDULE_ALARM_2    "\x84\xDA" /////////////////////알람 설정 된 일정
#define	SF_SENDING			   "\x84\xDB" /////////////////////발신
#define	SF_RECEIVING		   "\x84\xDC" /////////////////////착신
#define	SF_ABSENT_RECEIPT	   "\x84\xDD" /////////////////////부재 중 착신
#define	SF_LINE_BUSY_RECEIPT   "\x84\xDE" /////////////////////통화 중 착신 
/* reserve --->
#define	SF_   "\x84\xDF"
 <--- */
#define	SF_CHECK_BOX           "\x84\xE0"
#define	SF_CHECK               "\x84\xE1"
#define	SF_BOX                 "\x84\xE2"
#define	SF_RADIO_BUTTON_ON     "\x84\xE3"
#define	SF_RADIO_BUTTON_POINT  "\x84\xE4"
#define	SF_RADIO_BUTTON_OFF    "\x84\xE5"
#define	SF_COMBO_BOX           "\x84\xE6"
#define SF_DPROGRESSBAR_ON     "\x84\xE7"
#define SF_DPROGRESSBAR_OFF    "\x84\xE8"
#define SF_SPROGRESSBAR_ON     "\x84\xE9"
#define SF_SPROGRESSBAR_OFF    "\x84\xEA"
#define SF_SPIN                "\x84\xEB"

//////////////////////////////////추가 
#define	SF_NPROGRESSBAR_1      "\x84\xEC"
#define	SF_NPROGRESSBAR_2      "\x84\xED"
#define	SF_NPROGRESSBAR_3      "\x84\xEE"


////////////////////////////////4월 12일 추가
#define	SF_MSGBOX_QUESTION		   "\x84\xEF" /////////////////////메시지박스 물음표

#define	SF_MSGBOX_OK			   "\x84\xF0" /////////////////////메시지박스 OK
#define	SF_MSGBOX_EXCLAMATION	   "\x84\xF1" /////////////////////메시지박스 느낌표 
#define	SF_MSGBOX_X				   "\x84\xF2" /////////////////////메시지박스 "X"표
#define	SF_MSGBOX_I				   "\x84\xF3" /////////////////////메시지박스 "i"표

#define	SF_CALL_STND			   "\x84\xF4"
#define	SF_CALL_CALLING			   "\x84\xF5"
#define	SF_CALL_D				   "\x84\xF6"


#define	SF_UPPER_TRIANGLE		   "\x84\xF7"
#define	SF_LOWER_TRIANGLE		   "\x84\xF8"


#define	SF_BRIGHT_SPIN			   "\x84\xF9"
#define	SF_BRIGHT_UPPER_TRIANGLE   "\x84\xFA"
#define	SF_BRIGHT_LOWER_TRIANGLE   "\x84\xFB"


//////////////////////////////////5월 2일 추가 
#define	SF_LEFT_TRIANGLE		  "\x84\xFC"
#define	SF_RIGHT_TRIANGLE		  "\x84\xFD"
//#define SF_BACKLIGHT              "\x84\xA1"


#define	SF_DARK_LEFT_TRIANGLE	  "\x84\xFE"
#define	SF_DARK_RIGHT_TRIANGLE	  "\x84\xFF"

/* reserved --->
 <--- */

/* ---------------------------------- 0x85 --------------------------------- */

#define	SF_BLACK_NUM0	         "\x85\xA0"
#define	SF_BLACK_NUM1	         "\x85\xA1"
#define	SF_BLACK_NUM2	         "\x85\xA2"
#define	SF_BLACK_NUM3	         "\x85\xA3"
#define	SF_BLACK_NUM4	         "\x85\xA4"
#define	SF_BLACK_NUM5	         "\x85\xA5"
#define	SF_BLACK_NUM6	         "\x85\xA6"
#define	SF_BLACK_NUM7	         "\x85\xA7"
#define	SF_BLACK_NUM8	         "\x85\xA8"
#define	SF_BLACK_NUM9	         "\x85\xA9"
#define	SF_BLACK_NUM10	         "\x85\xAA"
#define	SF_BLACK_NUM11	         "\x85\xAB"
#define	SF_BLACK_NUM12	         "\x85\xAC"
#define	SF_BLACK_NUM13	         "\x85\xAD"
#define	SF_BLACK_NUM14	         "\x85\xAE"
#define	SF_BLACK_NUM15	         "\x85\xAF"
#define	SF_BLACK_NUM16	         "\x85\xB0"
#define	SF_BLACK_NUM17	         "\x85\xB1"
#define	SF_BLACK_NUM18	         "\x85\xB2"
#define	SF_BLACK_NUM19	         "\x85\xB3"
#define	SF_BLACK_NUM20	         "\x85\xB4"
#define	SF_BLACK_NUM21	         "\x85\xB5"
#define	SF_BLACK_NUM22	         "\x85\xB6"
#define	SF_BLACK_NUM23	         "\x85\xB7"
#define	SF_BLACK_NUM24	         "\x85\xB8"
#define	SF_BLACK_NUM25	         "\x85\xB9"
#define	SF_BLACK_NUM26	         "\x85\xBA"
#define	SF_BLACK_NUM27	         "\x85\xBB"
#define	SF_BLACK_NUM28	         "\x85\xBC"
#define	SF_BLACK_NUM29	         "\x85\xBD"
#define	SF_BLACK_NUM30	         "\x85\xBE"
#define	SF_BLACK_NUM31	         "\x85\xBF"
#define	SF_BLACK_NUM32	         "\x85\xC0"
#define	SF_BLACK_NUM33	         "\x85\xC1"
#define	SF_BLACK_NUM34	         "\x85\xC2"
#define	SF_BLACK_NUM35	         "\x85\xC3"
#define	SF_BLACK_NUM36	         "\x85\xC4"
#define	SF_BLACK_NUM37	         "\x85\xC5"
#define	SF_BLACK_NUM38	         "\x85\xC6"
#define	SF_BLACK_NUM39	         "\x85\xC7"
#define	SF_BLACK_NUM40	         "\x85\xC8"
#define	SF_BLACK_NUM41	         "\x85\xC9"
#define	SF_BLACK_NUM42	         "\x85\xCA"
#define	SF_BLACK_NUM43	         "\x85\xCB"
#define	SF_BLACK_NUM44	         "\x85\xCC"
#define	SF_BLACK_NUM45	         "\x85\xCD"
#define	SF_BLACK_NUM46	         "\x85\xCE"
#define	SF_BLACK_NUM47	         "\x85\xCF"
#define	SF_BLACK_NUM48	         "\x85\xD0"
#define	SF_BLACK_NUM49	         "\x85\xD1"
#define	SF_BLACK_NUM50	         "\x85\xD2"

////////////////////////////////////////////////4월 4일 추가 
#define	SF_BLACK_NUM51   "\x85\xD3"
#define	SF_BLACK_NUM52   "\x85\xD4"
#define	SF_BLACK_NUM53   "\x85\xD5"
#define	SF_BLACK_NUM54   "\x85\xD6"
#define	SF_BLACK_NUM55   "\x85\xD7"
#define	SF_BLACK_NUM56   "\x85\xD8"
#define	SF_BLACK_NUM57   "\x85\xD9"
#define	SF_BLACK_NUM58   "\x85\xDA"
#define	SF_BLACK_NUM59   "\x85\xDB"
#define	SF_BLACK_NUM60   "\x85\xDC"
#define	SF_BLACK_NUM61   "\x85\xDD"
#define	SF_BLACK_NUM62   "\x85\xDE"
#define	SF_BLACK_NUM63   "\x85\xDF"
#define	SF_BLACK_NUM64   "\x85\xE0"
#define	SF_BLACK_NUM65   "\x85\xE1"
#define	SF_BLACK_NUM66   "\x85\xE2"
#define	SF_BLACK_NUM67   "\x85\xE3"
#define	SF_BLACK_NUM68   "\x85\xE4"
#define	SF_BLACK_NUM69   "\x85\xE5"
#define	SF_BLACK_NUM70   "\x85\xE6"
#define	SF_BLACK_NUM71   "\x85\xE7"
#define	SF_BLACK_NUM72   "\x85\xE8"
#define	SF_BLACK_NUM73   "\x85\xE9"
#define	SF_BLACK_NUM74   "\x85\xEA"
#define	SF_BLACK_NUM75   "\x85\xEB"
#define	SF_BLACK_NUM76   "\x85\xEC"
#define	SF_BLACK_NUM77   "\x85\xED"
#define	SF_BLACK_NUM78   "\x85\xEE"
#define	SF_BLACK_NUM79   "\x85\xEF"
#define	SF_BLACK_NUM80   "\x85\xF0"
#define	SF_BLACK_NUM81   "\x85\xF1"
#define	SF_BLACK_NUM82   "\x85\xF2"
#define	SF_BLACK_NUM83   "\x85\xF3"
#define	SF_BLACK_NUM84   "\x85\xF4"
#define	SF_BLACK_NUM85   "\x85\xF5"
#define	SF_BLACK_NUM86   "\x85\xF6"
#define	SF_BLACK_NUM87   "\x85\xF7"
#define	SF_BLACK_NUM88   "\x85\xF8"
#define	SF_BLACK_NUM89   "\x85\xF9"
#define	SF_BLACK_NUM90   "\x85\xFA"
#define	SF_BLACK_NUM91   "\x85\xFB"
#define	SF_BLACK_NUM92   "\x85\xFC"
#define	SF_BLACK_NUM93   "\x85\xFD"
#define	SF_BLACK_NUM94   "\x85\xFE"
#define	SF_BLACK_NUM95   "\x85\xFF"
/////////////////////////////////////////////////////////


/* ---------------------------------- 0x86 --------------------------------- */

#define	SF_BLACK_NUM96	         "\x86\xA0"
#define	SF_BLACK_NUM97	         "\x86\xA1"
#define	SF_BLACK_NUM98	         "\x86\xA2"
#define	SF_BLACK_NUM99	         "\x86\xA3"
#define	SF_BLACK_NUM100	         "\x86\xA4"


#define	SF_SMS_SEND_NUM1_6       "\x86\xA5"//SMS 상대방 전화번호
#define	SF_SMS_SEND_NUM2_6       "\x86\xA6"
#define	SF_SMS_SEND_NUM3_6	     "\x86\xA7"
#define	SF_SMS_SEND_NUM4_6       "\x86\xA8"
#define	SF_SMS_SEND_NUM5_6       "\x86\xA9"
#define	SF_SMS_SEND_NUM6_6       "\x86\xAA"
#define	SF_SMS_RECEIVE_NUM1_6    "\x86\xAB"//SMS 연락받을 번호 
#define	SF_SMS_RECEIVE_NUM2_6    "\x86\xAC"
#define	SF_SMS_RECEIVE_NUM3_6    "\x86\xAD"
#define	SF_SMS_RECEIVE_NUM4_6    "\x86\xAE"
#define	SF_SMS_RECEIVE_NUM5_6    "\x86\xAF"
#define	SF_SMS_RECEIVE_NUM6_6    "\x86\xB0"
#define	SF_ADD_NUM1_3         	 "\x86\xB1"//번호추가
#define	SF_ADD_NUM2_3            "\x86\xB2"
#define	SF_ADD_NUM3_3          	 "\x86\xB3"
#define	SF_DEL_NUM1_3            "\x86\xB4"//번호삭제
#define	SF_DEL_NUM2_3            "\x86\xB5"
#define	SF_DEL_NUM3_3            "\x86\xB6"
#define	SF_SAVE_DAY1_3           "\x86\xB7"//저장날짜
#define	SF_SAVE_DAY2_3           "\x86\xB8"
#define	SF_SAVE_DAY3_3           "\x86\xB9"


#define	SF_REC_START_1_6         "\x86\xBA"//길게 : 녹음시작 
#define	SF_REC_START_2_6         "\x86\xBB"
#define	SF_REC_START_3_6         "\x86\xBC"
#define	SF_REC_START_4_6         "\x86\xBD"
#define	SF_REC_START_5_6         "\x86\xBE"
#define	SF_REC_START_6_6         "\x86\xBF"
/* reserved --->
 <--- */
/*
#define	SF_SAVE_DATE1_4          "\x86\xC0"///////저장날짜
#define	SF_SAVE_DATE2_4 	     "\x86\xC1"
#define	SF_SAVE_DATE3_4          "\x86\xC2"
#define	SF_SAVE_DATE4_4 	     "\x86\xC3"

#define	SF_PIC_NAME1_4	         "\x86\xC4"////////사진이름
#define	SF_PIC_NAME2_4	         "\x86\xC5"
#define	SF_PIC_NAME3_4	         "\x86\xC6"
#define	SF_PIC_NAME4_4	         "\x86\xC7"

#define	SF_NUM_PLUS1_4	         "\x86\xC8"////////번호추가
#define	SF_NUM_PLUS2_4	         "\x86\xC9"
#define	SF_NUM_PLUS3_4	         "\x86\xCA"
#define	SF_NUM_PLUS4_4	         "\x86\xCB"
*/

//기존 스카이 폰과의 호환을 위하여...
#define	SF_TELEPHONE	 	     "\x86\xC1"
#define	SF_CELLPHONE	         "\x86\xC2"
#define	SF_PAGER4SMS	 	     "\x86\xC3"
#define	SF_SOMETHING	         "\x86\xC4"
#define	SF_HOUSE		         "\x86\xC5"
#define	SF_BUILDING		         "\x86\xC6"
#define	SF_FATHER		         "\x86\xC7"
#define	SF_MOTHER		         "\x86\xC8"
#define	SF_BOY			         "\x86\xC9"
#define	SF_GIRL			         "\x86\xCA"
//기존 스카이 폰과의 호환을 위하여...

#define	SF_NUM_DEL1_4	         "\x86\xCC"////////번호삭제
#define	SF_NUM_DEL2_4	         "\x86\xCD"
#define	SF_NUM_DEL3_4	         "\x86\xCE"
#define	SF_NUM_DEL4_4	         "\x86\xCF"

#define	SF_REPEAT_CYCLE1_4       "\x86\xD0"////////반복주기
#define	SF_REPEAT_CYCLE2_4	     "\x86\xD1"
#define	SF_REPEAT_CYCLE3_4	     "\x86\xD2"
#define	SF_REPEAT_CYCLE4_4       "\x86\xD3"

#define	SF_SET_TIME1_4			 "\x86\xD4"////////설정시간
#define	SF_SET_TIME2_4           "\x86\xD5"

// Mobile Wallet 용.. =====================
//사용자정보
#define	SF_MW_U_NAME		       "\x86\xCD"
#define	SF_MW_U_PHONE		       "\x86\xCE"
#define SF_MW_U_EMAIL		       "\x86\xCF"
// 결제정보
#define	SF_MW_C_NUM				      "\x86\xD6"
#define	SF_MW_C_PASSWD		      "\x86\xD7"
#define	SF_MW_C_VALID				 "\x86\xD8"
#define SF_MW_C_RNO					  "\x86\xD9"
#define SF_MW_P_NUM					  "\x86\xDB"
#define SF_MW_P_PASSWD			  "\x86\xDC"
// 배송정보
#define SF_MW_D_ZIPCODE			  "\x86\xD1"
#define SF_MW_D_ADDR			  "\x86\xD2"
#define SF_MW_D_RNAME			  "\x86\xD3"
#define SF_MW_D_RPHONE			  "\x86\xAB"
// 인증서정보
#define SF_MW_CERT_ORG				"\x86\xE1"
#define SF_MW_CERT_VALID			"\x86\xE2"
#define SF_MW_CERT_CERT				"\x86\xE4"
#define SF_MW_CERT_ETC				 "\x86\xE5"
//====================================

//#define	SF_ALARM_NAME3_4         "\x86\xDA"
//#define	SF_MAKE_TIME2_4          "\x86\xDD"
//#define	SF_MAKE_TIME3_4          "\x86\xDE"
//#define	SF_MAKE_TIME4_4          "\x86\xDF"
//#define	SF_SEND_NUM2_3           "\x86\xE3"

#define	SF_RECEIPT_NUM2_3        "\x86\xE6"
#define	SF_RECEIPT_NUM3_3        "\x86\xE7"

#define	SF_NORMAL1_2             "\x86\xE8"///////일반
#define	SF_NORMAL2_2             "\x86\xE9"

#define	SF_MEETING1_3            "\x86\xEA"///////회의중
#define	SF_MEETING2_3            "\x86\xEB"
#define	SF_MEETING3_3            "\x86\xEC"

#define	SF_DRIVING1_3            "\x86\xED"///////운전중
#define	SF_DRIVING2_3            "\x86\xEE"
#define	SF_DRIVING3_3            "\x86\xEF"

#define	SF_USER1_3               "\x86\xF0"///////사용자
#define	SF_USER2_3               "\x86\xF1"
#define	SF_USER3_3               "\x86\xF2"

#define	SF_ABSENCE_CALL1_5       "\x86\xF3"///////부재중 전화
#define	SF_ABSENCE_CALL2_5       "\x86\xF4"
#define	SF_ABSENCE_CALL3_5       "\x86\xF5"
#define	SF_ABSENCE_CALL4_5       "\x86\xF6"
#define	SF_ABSENCE_CALL5_5       "\x86\xF7"

#define	SF_CALL_VOL1_6           "\x86\xF8"///////통화음량조절
#define	SF_CALL_VOL2_6           "\x86\xF9"
#define	SF_CALL_VOL3_6           "\x86\xFA"
#define	SF_CALL_VOL4_6           "\x86\xFB"
#define	SF_CALL_VOL5_6           "\x86\xFC"
#define	SF_CALL_VOL6_6           "\x86\xFD"


#define	SF_START			     "\x86\xFE"
#define	SF_END		             "\x86\xFF"

/* reserved --->
 <--- */

/* ---------------------------------- 0x87 --------------------------------- */
#define	SF_SAVE_DATE1_4          "\x87\xA0"///////저장날짜
#define	SF_SAVE_DATE2_4 	     "\x87\xA1"
#define	SF_SAVE_DATE3_4          "\x87\xA2"
#define	SF_SAVE_DATE4_4 	     "\x87\xA3"

#define	SF_PIC_NAME1_4	         "\x87\xA4"////////사진이름
#define	SF_PIC_NAME2_4	         "\x87\xA5"
#define	SF_PIC_NAME3_4	         "\x87\xA6"
#define	SF_PIC_NAME4_4	         "\x87\xA7"

#define	SF_NUM_PLUS1_4	         "\x87\xA8"////////번호추가
#define	SF_NUM_PLUS2_4	         "\x87\xA9"
#define	SF_NUM_PLUS3_4	         "\x87\xAA"
#define	SF_NUM_PLUS4_4	         "\x87\xAB"


////////////////////////////////////////////////////새로운 메시지 박스용//////////
#define	SF_ANOTHER_POUND	         "\x87\xAD"//#								//
#define	SF_NEW_MSGBOX_OK1_2		     "\x87\xAE"//OK								//
#define	SF_NEW_MSGBOX_OK2_2			 "\x87\xAF"//OK								//
#define	SF_NEW_MSGBOX_EXCLAMATION1_2 "\x87\xB0"//!								//
#define	SF_NEW_MSGBOX_EXCLAMATION2_2 "\x87\xB1"//!								//
#define	SF_NEW_MSGBOX_I1_2	         "\x87\xB2"//i								//
#define	SF_NEW_MSGBOX_I2_2	         "\x87\xB3"//i								//
#define	SF_NEW_MSGBOX_X1_2	         "\x87\xB4"//X								//
#define	SF_NEW_MSGBOX_X2_2	         "\x87\xB5"//X								//
#define	SF_NEW_MSGBOX_QUESTION1_2	 "\x87\xB6"//?								//
#define	SF_NEW_MSGBOX_QUESTION2_2	 "\x87\xB7"//?								//
//////////////////////////////////////////////////////////////////////////////////
#define SF_IRPC_1                   "\x87\xBD" // PC와의 통신
#define SF_IRPC_2                   "\x87\xBE"
#define SF_IRPC_3                   "\x87\xBF"

#define SF_VOICE_RECORD1_4			"\x87\xC0"//음성녹음
#define SF_VOICE_RECORD2_4			"\x87\xC1"//음성녹음
#define SF_VOICE_RECORD3_4			"\x87\xC2"//음성녹음
#define SF_VOICE_RECORD4_4			"\x87\xC3"//음성녹음

#define SF_VOICE_MEMO1_4			"\x87\xE0"//음성메모
#define SF_VOICE_MEMO2_4			"\x87\xE1"//음성메모
#define SF_VOICE_MEMO3_4			"\x87\xE2"//음성메모
#define SF_VOICE_MEMO4_4			"\x87\xE3"//음성메모

#define SF_AUTO_REPLY1_4			"\x87\xE6"//자동응답
#define SF_AUTO_REPLY2_4			"\x87\xE7"//자동응답
#define SF_AUTO_REPLY3_4			"\x87\xE8"//자동응답
#define SF_AUTO_REPLY4_4			"\x87\xE9"//자동응답

#define SF_NEW_AUTO_REPLY1_4		"\x87\xEA"//새자동응답
#define SF_NEW_AUTO_REPLY2_4		"\x87\xEB"//새자동응답
#define SF_NEW_AUTO_REPLY3_4		"\x87\xEC"//새자동응답
#define SF_NEW_AUTO_REPLY4_4		"\x87\xED"//새자동응답 

#define SF_ALARM0_3                 "\x87\xEE"//알람울림 아이콘
#define SF_ALARM1_3					"\x87\xEF"//알람울림
#define SF_ALARM2_3					"\x87\xF0"//알람울림
#define SF_ALARM3_3					"\x87\xF1"//알람울림

#define SF_HOLLYDAY_ALARM0_4		"\x87\xF2"//휴일에도 울림 아이콘
#define SF_HOLLYDAY_ALARM1_4		"\x87\xF3"//휴일에도 울림
#define SF_HOLLYDAY_ALARM2_4		"\x87\xF4"//휴일에도 울림
#define SF_HOLLYDAY_ALARM3_4		"\x87\xF5"//휴일에도 울림
#define SF_HOLLYDAY_ALARM4_4		"\x87\xF6"//휴일에도 울림

#define SF_ALARM_TIME0_3			"\x87\xF7"//알람시간 아이콘
#define SF_ALARM_TIME1_3			"\x87\xF8"//알람시간
#define SF_ALARM_TIME2_3			"\x87\xF9"//알람시간
#define SF_ALARM_TIME3_3			"\x87\xFA"//알람시간

#define SF_CHARBELL_1               "\x87\xFC" //메모그림
#define SF_CHARACBELL_1             "\x87\xFD" //삭제불가 아이콘
#define SF_CHARACBELL_2             "\x87\xFE" //캐릭벨 그룹단위 설정
#define SF_CHARACBELL_3             "\x87\xFF" //캐릭벨 그룹단위 설정

/////////////////////////////////////////////////////////




#define SF_DOTDOTDOT        "\xA1\xA6"

/* -----------------------------------------
** old style --> should be deleted
**
*/

#define	SF_BLACK_STAR	    SF_STAR
#define	SF_BLACK_POUND	    SF_POUND

#define SF_IRDA_WAIT        "\x88\xb9"
#define SF_IRDA_CONNECT     "\x88\xb6\x88\xb7\x88\xb8"

#define SF_PBOOK_GROUP      SF_GROUP_TEXT//"\203\365"
#define SF_PBOOK_SECRET     SF_SECRET1_2""SF_SECRET2_2//"\204\262\204\263"
#define SF_PBOOK_ARROW      SF_RIGHT_FINGER//"\206\332"

#define SF_PBOOK_HOME       SF_HOME//"\206\305"
#define SF_PBOOK_WORK       SF_OFFICE//"\206\306"
#define SF_PBOOK_CELL       SF_CELLULAR//"\206\302"
#define SF_PBOOK_PAGER      SF_PAGER//"\206\303"
  
#define SF_MEMO_ICON        SF_MEMO//"\203\372"

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

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

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQS_SFNT_H_ */
