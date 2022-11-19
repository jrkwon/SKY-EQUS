#ifndef FONT_H
#define FONT_H

//************************************************************************
//
//							 F O N T	 H E A D E R		F I L E
//
//	File Name   : Font.h
//  Description : Support Decalares of FONT data Buffer
//	Author		  : SK Teletech Software Team
//
//************************************************************************
//
//		 EDIT HISTORY FOR MODULE
//
//  This section contains comments describing changes made to this file.
//  Notice that changes are listed in reverse chronological order.
//
//
//	when       who      what, where, why
//--------     ---      ---------------------------------------------------
//99.10.27     kmh      create for A7 Project
//
//
//*************************************************************************

#include "comdef.h"
#include <lcddef_a10.h>

//------------------------------
#if FONT_DATA == BYTE_DATA
//------------------------------

extern byte ROM MedASCIIFontBuffer[];


extern byte ROM ASCIIFontBuffer[]; 


// include big ASCII font data
extern byte ROM	BigASCIIFontBuffer[];


// include small ASCII font data
// small ASCII중 0x01 ~ 0x1f 까지는 calendar용 font로 사용
extern byte ROM	SmallASCIIFontBuffer[];


// include SPECIAL(KSC5601) font data
extern byte ROM SpecialFontBuffer[];


// SKY Symbol font data
extern word ROM SymbolFontBuffer[];

// girin. add. 2000.1.26
extern byte ROM MedSpecialFontBuffer[];

extern word ROM MedSymbolFontBuffer[];
// girin. end.


/*******************************************************************
	Korean & 2 byte Fonts
********************************************************************/
extern byte ROM	MedHanFontBuffer[];
extern byte ROM	HanFontBuffer[];


//-------------------------------------
#else		// #if FONT_DATA == BYTE_DATA
//-------------------------------------

//-----------------------------
extern word ROM	HanWordFontBuffer[];

// include ASCII font data
extern word ROM ASCIIWordFontBuffer[];
	
// include big ASCII font data
extern word ROM	BigASCIIWordFontBuffer[];

// include small ASCII font data
// small ASCII중 0x01 ~ 0x1f 까지는 calendar용 font로 사용
extern word ROM	SmallASCIIWordFontBuffer[];

// include SPECIAL(KSC5601) font data
extern word ROM SpecialWordFontBuffer[];

// SKY Symbol font data
extern word ROM SymbolWordFontBuffer[];

//-------------------------------------
#endif		// #if FONT_DATA == BYTE_DATA
//-------------------------------------

#endif /* FONT_H */
