/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           A S C I I     F O N T

GENERAL DESCRIPTION 

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

INITIALIZATION AND SEQUENCING REQUIREMENTS

AUTHOR
  SK Teletech Software Team 

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who      what, where, why
--------   ---      ---------------------------------------------------------
99.10.27   kmh      Create for A7 Project

===========================================================================*/

#include "comdef.h"
#include "target.h" 
#include "lcddef_a10.h"   // because of "FONT_DATA" Define

//------------------------------
#if FONT_DATA == BYTE_DATA  
//------------------------------

/*******************************************************************
  1 byte code fonts
********************************************************************/

// ASCII code font : 2 color
// 
byte ROM  SmallASCIIFontBuffer[] =
{
  #include "image\ASFontN.def"
};

// ASCII code font : 2 color
// 6 * 13
byte ROM MedASCIIFontBuffer[] =
{
  #include "image\AMFontN.def"
}; 
 
// ASCII code font : 2 color
// 7 * 16
byte ROM ASCIIFontBuffer[] = 
{     
  #include "image\AFontN.def" 
};

// ASCII code font : 2 color
// 7 * 21
byte ROM  BigASCIIFontBuffer[] =
{
    #include "image\ABFontN.def"
};


/*******************************************************************
  Korean & 2 byte code fonts
********************************************************************/
// Korean font : 2 color
// 12 * 13
byte ROM  MedHanFontBuffer[] =
{
    #include "image\HMFontN.def"
};

// Korean font : 2 color
// 14 * 26
byte ROM  HanFontBuffer[] =
{
    #include "image\HFontN.def"
};

// KSC5601 symbol font : 2 color
// 12 * 13
byte ROM MedSpecialFontBuffer[] = 
{
	#include "image\HSMFontN.def"
};

// KSC5601 symbol font : 2 color
// 14 * 16
byte ROM SpecialFontBuffer[] =
{ 
  #include "image\HSFontN.def" 
  
  //#include "font\image\a7\HSFontN.def" 
}; 

// SK Special font : 4gray
// 12 * 13
word ROM MedSymbolFontBuffer[] = 
{
#if (MODEL_ID!=MODEL_A8)
	#include "image\WSMFontH.dhf"
#else
  // moda,05.20. add floder type phone figure as B5.
  #include "image\A8\WSMFontH_A8.dhf"
#endif
};

// SK Special font : 4gray
// 14 * 16
word ROM SymbolFontBuffer[] =
{
#if (MODEL_ID!=MODEL_A8)
  #include "image\WSFontH.dhf" 
#else
  // moda,05.19. add origination and terminating menu as B5.
  #include "image\A8\WSFontH.dhf"
#endif
};

//-------------------------------------
#else // #if FONT_DATA == BYTE_DATA
//-------------------------------------

// include ASCII font data
word ROM ASCIIWordFontBuffer[] =
{

  #include "WAFontN.def"

};

// include big ASCII font data
word ROM  BigASCIIWordFontBuffer[] =
{

    #include "WBAFontN.def"

};

// include small ASCII font data
// small ASCII중 0x01 ~ 0x1f 까지는 calendar용 font로 사용
word ROM  SmallASCIIWordFontBuffer[] =
{

    #include "WSAFontN.def"

};

// include SPECIAL(KSC5601) font data 
word ROM SpecialWordFontBuffer[] =
{

    #include "WHSFontN.def"

};

// SKY Symbol font data
word ROM SymbolWordFontBuffer[] =
{

    #include "WSFontNC5.def"

};

word ROM  HanWordFontBuffer[] =
{
    #include "WHFontN.def"
};

//-------------------------------------
#endif  // #if FONT_DATA == BYTE_DATA
//-------------------------------------


/*---------------------------------------------
  Insert for ID LCD (SUB LCD) of A8 Project
---------------------------------------------*/
#if MODEL_ID == MODEL_A8	// kmh 00.04.25
#ifdef A8_SUB_LCD
const byte sub_lcd_medium_ascii_font[] =  // 6 x 13 ASCII Table - Numeric
{
  #include "..\IDLCD\Font\AFontE.dxf"
  
};

const byte sub_lcd_medium_han_font[] =   // 12 x 13 Korean Character
{

  #include "..\IDLCD\Font\HFontE.dxf"

};

const byte sub_lcd_medium_special_font[] =  // 12 x 13 Korean Symbol Character
{
  #include "..\IDLCD\Font\HSFontE.dxf"
};

const byte sub_lcd_medium_sky_font[] =      // 12 x 13 SK Symbol Character
{
  #include "..\IDLCD\Font\SFontEB5.dxf"
//#include "..\IDLCD\Font\SFontEB5.dpf"

};
#endif	// A8_SUB_LCD
#endif	// #if MODEL_ID == MODEL_A8














