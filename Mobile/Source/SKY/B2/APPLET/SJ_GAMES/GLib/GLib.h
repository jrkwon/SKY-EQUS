//==============================================================================
//
//	SINJISOFT GAME LIBRARY
//
//	FILE NAME
//		GLib.h
//
//	DESCRIPTION
//		Game library header
//
//	REFERENCES
//		SINJISOFT Game Porting Guide
//
//	Copyright (C) 1999 by SINJISOFT. All Rights Reserved.
//
//	HISTORY
//		1999.11. 0	Release Version 0.9
//		1999.12.21	Release Version 1.0
//		2000. 1. 7  Release Version 1.1
//		2000. 2.22  Release Version 1.2
//		2000.10. 8  Release Version 2.0
//		2001. 1.21	Start Version 2.1
//
//==============================================================================

#ifndef _GLIB_H_
#define _GLIB_H_

#include "GExt.h"
#include "Mil.h"

////////////////////////////////
// General constant

#define	G_LOGO_TIMEOUT			100
#define	G_SCORE_TIMEOUT			300

////////////////////////////////
// General data structure

typedef	__packed struct _s_GameDefType	{
	char*	Title;
	char*	Description;
	int		(*GameMain)(int nReason, int nData);
	int		GameMode;
	int		LcdWidth, LcdHeight;
	int		NvSize;
} s_GameDefType;

typedef __packed struct _g_Record {
	BYTE	month, day;
	BYTE	score[2];
	char	name[11];
} g_Record;

// Object control structure
typedef __packed struct _g_Object {
	INT1	state;							// state
	int		x, y;							// position
	INT1	dx, dy;							// movement setting
	INT1	cx, cy;							// Center position
} g_Object;

// Object control structure
typedef __packed struct _g_Object3 {
	INT1	state;							// state
	int		x, y;							// position
	INT1	dx, dy;							// movement setting
	BYTE	c1, c2, c3;						// condition
} g_Object3;

////////////////////////////////
// Global variables

extern	const	s_GameDefType	s_GameDef[];

extern	int		s_Frame;					// Global counter 0..5040
extern	int		s_Frame2;					// Global counter 0..1
extern	int		s_Frame3;					// Global counter 0..2

extern	BYTE	*swLcdBuffer;				// LCD buffer
extern	BYTE	*swBackLcdBuffer;			// LCD back-buffer

extern	int		s_GameID;

//======================================
//	GAME STATE DEFINITION
//		Definition of glib_GameMain
//		parameter
//======================================

// Game engine event
#define	GLIB_START_EVENT		0		// Game engine start
#define	GLIB_TIMEOUT_EVENT		1		// Timeout event
#define	GLIB_KEY_EVENT			2		// Keypress event
#define	GLIB_END_EVENT			3		// End event
//	Touch Screen Processing
#define	GLIB_SCREEN_EVENT		4		// Touch Screen event

// Glib state
#define	GLIB_MODE_MENU			0
#define	GLIB_MODE_GAME			1
#define	GLIB_MODE_SCOREEDIT		2
#define	GLIB_MODE_SCORE			3
#define	GLIB_MODE_HELP			4

// Game event
#define	GLIB_MENU				0		// Draw menu title
#define	GLIB_START				1		// Game start
#define	GLIB_PAUSE				2		// Pause event
#define	GLIB_CONTINUE			3		// Continue event
#define	GLIB_TIMEOUT			4		// Timeout event
#define	GLIB_KEYPRESS			5		// Keypress event
#define	GLIB_END				6		// END event
#define	GLIB_MENU_SCORE			7		// Show score in Menu Mode
#define	GLIB_SCORE				8		// Show score in Game Mode
#define	GLIB_HELP				9		// Show help
#define	GLIB_HELP_TIMEOUT		10		// Show help in time out
#define	GLIB_CHECK_STATE		11		// return status

#define	GLIB_NET_START			12
#define	GLIB_NET_RECEIVE		13

// Return code
#define	GRET_NORMAL				0
#define	GRET_PAUSE				1
#define	GRET_SCORE_VIEW			2
#define	GRET_SCORE_EDIT			3
#define	GRET_ERROR				4

////////////////////////////////////////////////////////////////
// GLibCommon.cpp
////////////////////////////////////////////////////////////////

#define	G_NV_MAGIC				0x99
// Number of score save data size
#define	G_NV_SCORE				76	// (2+2+11)*5 + 1

#define	RANDRATIO(r)			((gext_Random(0, 100) < (r)) ? 1 : 0)
#define	RANDOM(r)				(gext_Random(0, 100) < (r))
#define	CENTER(l)				((s_LcdWidth - (l))/2)
#ifndef ABS
    #define ABS(n)					(((n) >= 0) ? (n) : -(n))
#endif //ABS
#define	HIT_CHECK(p1, p2, r)	(((p1) >= (p2)-(r)) && ((p1) <= (p2)+(r)))
#define	HIT_CHECK2(x1, x2, dx, y1, y2, dy) \
								(HIT_CHECK(x1, x2, dx) && HIT_CHECK(y1, y2, dy))

extern	void	g_StartTimer(const int nTid, const int nMsec, const int nRepeat);
extern	void	g_StopTimer(const int nTid);
extern	void	g_SetColor(const int color);

extern	int		g_CheckScore(WORD score);
extern	void	g_LoadScore(void);
extern	void	g_SaveScore(void);
extern	void	g_DrawScoreBoard(int mode);
extern	int		g_EditScore(int state, int data);

#endif	// _GLIB_H_

