#ifndef MDARM_DEF
#define MDARM_DEF

#define	XSIG_SET(sig)	(xsig_flags |= (sig))
#define	XSIG_CLR(sig)	(xsig_flags &= ~(sig))
#define	XSIG_TEST(sig)	(xsig_flags & (sig))

#define	XSIG_SKT_SET(sig)	(xsig_flags_skt |= (sig))
#define	XSIG_SKT_CLR(sig)	(xsig_flags_skt &= ~(sig))
#define	XSIG_SKT_TEST(sig)	(xsig_flags_skt & (sig))

#define RAM
#define	MAX_OPEN	32
#define	SKIP_OPEN	3	/* must be factor of (MAX_OPEN+1) */
#define	STOP_KEY_DELAY	   1000
#define	REPEAT_KEY_DELAY   1000
#define	REPEAT_KEY_INTERVAL	200

#define	DS_NETWORK_XSIG		0x0001
#define	DS_DNS_XSIG			0x0002
#define	DS_CLOSE_XSIG		0x0004
#define	DS_READ_XSIG		0x0008
#define	DS_WRITE_XSIG		0x0010
#define DS_PPP_XSIG			0x0020
#define SND_END_XSIG		0x0040
#define	TIMER1_XSIG			0x0100
#define	TIMER2_XSIG			0x0200
#define	UPDATE_ANNU_XSIG	0x0400
#define	SND_TONE_XSIG		0x0800	/* if set --> NO tone snd */
#define	BACKLIGHT_XSIG		0x1000	/* if set --> NO backlight */
#define	REFRESH_LCD_XSIG	0x2000	/* if set --> xvm_state => xvm_suspend */

#define KEY_XSIG			0x0080

/* SOUND */
#define SOUND_UNSUPPORTED     0x0      
#define SOUND_BUZZER          0x1  
#define SOUND_MA1             0x2  
#define SOUND_MA2             0x4  
#define SOUND_MMF             0x8  
#define SOUND_CMX             0x10
#define SOUND_MIDI            0x20
#define SOUND_EVRC            0x40
#define SOUND_WAVE            0x80

// alarm type
#define ALERT_ALARM				1
#define ALERT_CONFIRMATION	 	2
#define ALERT_ERROR				3
#define ALERT_INFO				4
#define ALERT_WARNING			5

// phonebook field type
// PBOOK_MAX_FIELD와 PBOOK_ALL는 단말에서 쓰지 않는다.
#define PBOOK_MAX_FIELD		7
#define PBOOK_ALL           0
#define PBOOK_NAME      	1
#define PBOOK_GROUP     	2
#define PBOOK_HANDPHONE		3
#define PBOOK_HOME	    	4
#define PBOOK_OFFICE		5
#define PBOOK_EMAIL	    	6
#define PBOOK_MEMO		    7

// SMS Tid type  => 나중에 get하는 함수가 쓰이면 거기에 필요
#define SMSTYPE_UNKNOWN							0
#define	SMSTYPE_SHORT_MESSAGE					1
#define	SMSTYPE_DOWNLOAD_NOTIFICATION			2

//원래는 이 정도를 제공해야한다. 
#define STATIC_HEAP_SIZE        (375 * 1024)
#define SHARED_BUFFER_SIZE      0x8000          /* must be more than 32k */
#define ROMFILENAME     "__xvmrom__.jar"
#define	ISROMFILENAME(x) strstr(x, ROMFILENAME)
#define ROMFILEFD       0x7fff
#define ROMSAFE_FD(x)   (((unsigned int)(x))>=ROMFILEFD ? md_close(x),-1: (x))


#define FEATURE_SKVM_COLOR_256    //SKVM이 8bit color 를 쓸 때는 이것을 16bit일 때는 아래를 쓴다. 
//#define FEATURE_SKVM_COLOR_64K

// extern variable
extern short  xsig_flags;
extern short  xsig_flags_skt;
#endif  