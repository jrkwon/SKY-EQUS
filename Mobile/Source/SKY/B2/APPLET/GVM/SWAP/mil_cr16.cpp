//==============================================================================
//
//	Color LCD library
//
//==============================================================================

#ifdef SWAP_LCD_COLOR

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)

#define EW6LVBlK 0x20

#define tyJDygB4 0x05 

#define k2ax889Q 0x10 

#define OWsyYiyk 0x8C 

#define qlGFWhKG 0xB6 

#define b80baDCo 0xFF 

#ifdef LGIC16C
typedef enum { C16_TRPT, 
C16_BLCK, C16_BLUE, C16_RED , C16_MGTA, C16_GREN,
C16_CYAN, C16_YELW, C16_WHTE, C16_GRAY, C16_LBLU,
C16_LRED, C16_LMGT, C16_LGRN, C16_LCYN, C16_LYLW
} Pal16; 
#define JRIvsd2n C16_GRAY
static const BYTE BCu736FU[5][2] = { C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, C16_GRAY, C16_GRAY, EW6LVBlK, EW6LVBlK };
static const BYTE k3ux4fDW[5][2] = { C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, C16_GRAY, C16_GRAY, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE YT8oq9M8[5][2] = { C16_WHTE, C16_WHTE, C16_GRAY, C16_WHTE, C16_WHTE, C16_GRAY, C16_GRAY, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE YX6WZJ9k[5][2] = { C16_WHTE, C16_WHTE, C16_GRAY, C16_WHTE, C16_GRAY, C16_GRAY, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE uXtW6Xl8[5][2] = { C16_WHTE, C16_GRAY, C16_GRAY, C16_WHTE, C16_GRAY, C16_BLCK, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE I8XDgmy2[5][2] = { C16_WHTE, C16_GRAY, C16_GRAY, C16_GRAY, C16_GRAY, C16_BLCK, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE M0dimU4G[5][2] = { C16_GRAY, C16_GRAY, C16_GRAY, C16_GRAY, C16_BLCK, C16_BLCK, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };

#else
typedef enum { C16_BLCK, C16_RED , C16_GREN,
C16_YELW, C16_BLUE, C16_MGTA, C16_CYAN, C16_GRAY,
JRIvsd2n, C16_LRED, C16_LGRN, C16_LYLW, C16_LBLU,
C16_LMGT, C16_LCYN, C16_WHTE } Pal16; static const BYTE BCu736FU[5][2] = { C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, JRIvsd2n, JRIvsd2n, C16_GRAY, EW6LVBlK, EW6LVBlK };
static const BYTE k3ux4fDW[5][2] = { C16_WHTE, C16_WHTE, C16_WHTE, JRIvsd2n, JRIvsd2n, JRIvsd2n, C16_GRAY, C16_GRAY, EW6LVBlK, EW6LVBlK };
static const BYTE YT8oq9M8[5][2] = { C16_WHTE, C16_WHTE, C16_WHTE, JRIvsd2n, JRIvsd2n, C16_GRAY, C16_GRAY, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE YX6WZJ9k[5][2] = { C16_WHTE, C16_WHTE, JRIvsd2n, JRIvsd2n, C16_GRAY, C16_GRAY, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE uXtW6Xl8[5][2] = { C16_WHTE, JRIvsd2n, JRIvsd2n, C16_GRAY, C16_GRAY, C16_BLCK, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE I8XDgmy2[5][2] = { JRIvsd2n, JRIvsd2n, JRIvsd2n, C16_GRAY, C16_GRAY, C16_BLCK, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };
static const BYTE M0dimU4G[5][2] = { JRIvsd2n, C16_GRAY, C16_GRAY, C16_GRAY, C16_BLCK, C16_BLCK, C16_BLCK, C16_BLCK, EW6LVBlK, EW6LVBlK };

#endif
static const BYTE* HjvlX3r5[7] = { (BYTE*)BCu736FU,
(BYTE*)k3ux4fDW, (BYTE*)YT8oq9M8, (BYTE*)YX6WZJ9k,
(BYTE*)uXtW6Xl8, (BYTE*)I8XDgmy2, (BYTE*)M0dimU4G
}; static BYTE PlsvbI8t[5][2]; 
static const BYTE tWA94Aka[0x7C][2] = {
C16_WHTE, C16_LBLU, 
C16_WHTE, C16_LBLU,  C16_WHTE, C16_LBLU, 
C16_WHTE, C16_LGRN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LGRN, C16_WHTE, C16_LCYN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LGRN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LGRN, C16_WHTE, C16_LGRN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LGRN, 
C16_WHTE, C16_LGRN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LGRN, C16_WHTE, C16_LGRN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LGRN,  C16_WHTE, C16_LGRN, 
C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LRED, C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LYLW,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, C16_LYLW, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, C16_LYLW,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n, C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n, C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, C16_LRED,  C16_WHTE, C16_LRED, C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_LYLW, 
C16_WHTE, C16_WHTE,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_WHTE, 
C16_WHTE, C16_WHTE, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_WHTE, 
C16_WHTE, C16_WHTE,  C16_WHTE, C16_LRED, 
C16_WHTE, C16_LRED, 
C16_WHTE, C16_LMGT, C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LYLW, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_WHTE,  C16_WHTE, C16_WHTE, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_WHTE, 
C16_WHTE, C16_WHTE,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_WHTE };
static const BYTE wwIRCwE5[0x7C][2] = { C16_LBLU, C16_LBLU, 
C16_LBLU, C16_LBLU,  C16_WHTE, C16_LBLU, 
C16_WHTE, C16_LGRN,  C16_LBLU, C16_LCYN, 
C16_LBLU, C16_LCYN,  C16_WHTE, C16_LCYN, 
C16_LGRN, C16_LGRN, C16_LCYN, C16_LCYN,  C16_LCYN, C16_LCYN, 
C16_LCYN, C16_LCYN,  C16_LGRN, C16_LGRN, 
C16_LCYN, C16_LCYN,  C16_LCYN, C16_LCYN, 
C16_LCYN, C16_LCYN, 
C16_LGRN, C16_LGRN, C16_LCYN, C16_LGRN,  C16_LCYN, C16_LCYN, 
C16_LCYN, C16_LCYN,  C16_LGRN, C16_LGRN, 
C16_LCYN, C16_LGRN,  C16_LCYN, C16_LCYN, 
C16_LCYN, C16_LCYN,  C16_WHTE, C16_LGRN, C16_WHTE, C16_LGRN, 
C16_WHTE, C16_LCYN,  C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LGRN,  C16_WHTE, C16_LGRN, 
C16_WHTE, C16_LCYN, 
C16_WHTE, C16_LCYN,  C16_LRED, C16_LRED, C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LRED, C16_LYLW,  C16_LMGT, JRIvsd2n, 
C16_LMGT, JRIvsd2n,  C16_LMGT, JRIvsd2n, 
C16_LYLW, C16_LYLW, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_LYLW, C16_LYLW,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_LYLW, JRIvsd2n, 
C16_WHTE, JRIvsd2n, C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_LYLW, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n, C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_WHTE, JRIvsd2n,  C16_WHTE, JRIvsd2n, 
C16_LRED, C16_LRED,  C16_LMGT, C16_LRED, C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LRED, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT, 
C16_LYLW, C16_LMGT,  C16_WHTE, C16_LMGT, C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_LYLW, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_LYLW, C16_LYLW, 
C16_WHTE, C16_LYLW, 
C16_WHTE, C16_WHTE,  C16_LYLW, C16_LYLW, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_WHTE, 
C16_WHTE, C16_WHTE, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_WHTE, 
C16_WHTE, C16_WHTE,  C16_WHTE, C16_LRED, 
C16_WHTE, C16_LRED, 
C16_WHTE, C16_LMGT, C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LMGT,  C16_WHTE, C16_LMGT, 
C16_WHTE, C16_LYLW, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_WHTE, C16_WHTE, C16_WHTE, 
C16_WHTE, C16_LYLW,  C16_WHTE, C16_LYLW, 
C16_WHTE, C16_WHTE,  C16_WHTE, C16_WHTE, 
C16_LYLW, C16_LYLW,  C16_LYLW, C16_LYLW, 
C16_LYLW, C16_WHTE, 
C16_WHTE, C16_WHTE,  C16_LYLW, C16_LYLW, 
C16_LYLW, C16_LYLW,  C16_LYLW, C16_WHTE };
static const BYTE reI5L8za[0x7C][2] = { C16_LBLU, C16_BLUE, 
C16_LBLU, C16_LBLU,  C16_LBLU, C16_LBLU, 
C16_GRAY, C16_GREN,  C16_LBLU, C16_CYAN, 
C16_LBLU, C16_CYAN,  C16_LBLU, C16_CYAN, 
C16_LGRN, C16_GREN, C16_LCYN, C16_CYAN,  C16_LCYN, C16_CYAN, 
C16_LCYN, C16_CYAN,  C16_LGRN, C16_GREN, 
C16_LCYN, C16_CYAN,  C16_LCYN, C16_CYAN, 
C16_LCYN, C16_CYAN, 
C16_LGRN, C16_LGRN, C16_LCYN, C16_LGRN,  C16_LCYN, C16_LCYN, 
C16_LCYN, C16_LCYN,  C16_LGRN, C16_LGRN, 
C16_LCYN, C16_LGRN,  C16_LCYN, C16_LCYN, 
C16_LCYN, C16_LCYN,  C16_LGRN, C16_LGRN, C16_LGRN, C16_LGRN, 
C16_LGRN, C16_LCYN,  C16_LCYN, C16_LCYN, 
C16_LGRN, C16_LGRN,  C16_LGRN, C16_LGRN, 
C16_LGRN, C16_LCYN, 
C16_LCYN, C16_LCYN,  C16_LRED, C16_RED , C16_LMGT, C16_MGTA, 
C16_LMGT, C16_MGTA,  C16_LMGT, C16_MGTA, 
C16_LRED, C16_YELW,  C16_LMGT, JRIvsd2n, 
C16_LMGT, JRIvsd2n,  C16_LMGT, JRIvsd2n, 
C16_LYLW, C16_YELW, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
C16_LYLW, C16_YELW,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  C16_LYLW, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
C16_LYLW, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
C16_LRED, C16_LRED,  C16_LMGT, C16_LRED, C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LRED, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT, 
C16_LYLW, C16_LMGT,  JRIvsd2n, C16_LMGT, JRIvsd2n, C16_LMGT, 
JRIvsd2n, C16_LMGT,  C16_LYLW, C16_LMGT, 
JRIvsd2n, C16_LMGT,  JRIvsd2n, C16_LMGT, 
JRIvsd2n, C16_LMGT,  C16_LYLW, C16_LYLW, 
JRIvsd2n, C16_LYLW, 
JRIvsd2n, C16_WHTE,  C16_LYLW, C16_LYLW, 
JRIvsd2n, C16_LYLW,  JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_LYLW,  JRIvsd2n, C16_LYLW, 
JRIvsd2n, C16_WHTE, JRIvsd2n, C16_WHTE,  JRIvsd2n, C16_LYLW, 
JRIvsd2n, C16_LYLW,  JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_WHTE,  C16_LRED, C16_LRED, 
C16_LRED, C16_LRED, 
C16_LRED, C16_LMGT, C16_LMGT, C16_LMGT,  C16_LRED, C16_LMGT, 
C16_LRED, C16_LMGT,  C16_LRED, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LYLW, 
C16_LMGT, C16_LYLW,  C16_LMGT, C16_WHTE, C16_LMGT, C16_WHTE, 
C16_LMGT, C16_LYLW,  C16_LMGT, C16_LYLW, 
C16_LMGT, C16_WHTE,  C16_LMGT, C16_WHTE, 
C16_LYLW, C16_LYLW,  C16_LYLW, C16_LYLW, 
C16_LYLW, C16_WHTE, 
C16_WHTE, C16_WHTE,  C16_LYLW, C16_LYLW, 
C16_LYLW, C16_LYLW,  C16_LYLW, C16_WHTE };
static const BYTE nsZP3ml5[0x7C][2] = { C16_BLUE, C16_BLUE, 
C16_BLUE, C16_LBLU,  C16_LBLU, C16_LBLU, 
C16_BLCK, C16_GREN,  C16_BLUE, C16_CYAN, 
C16_BLUE, C16_CYAN,  C16_LBLU, C16_CYAN, 
C16_GREN, C16_GREN, C16_CYAN, C16_CYAN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN,  C16_GREN, C16_GREN, 
C16_CYAN, C16_CYAN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN, 
C16_GREN, C16_LGRN, C16_CYAN, C16_LGRN,  C16_CYAN, C16_LCYN, 
C16_CYAN, C16_LCYN,  C16_GREN, C16_LGRN, 
C16_CYAN, C16_LGRN,  C16_CYAN, C16_LCYN, 
C16_CYAN, C16_LCYN,  C16_LGRN, C16_LGRN, C16_LGRN, C16_LGRN, 
C16_LGRN, C16_LCYN,  C16_LCYN, C16_LCYN, 
C16_LGRN, C16_LGRN,  C16_LGRN, C16_LGRN, 
C16_LGRN, C16_LCYN, 
C16_LCYN, C16_LCYN,  C16_RED , C16_RED , C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, 
C16_RED , C16_YELW,  C16_MGTA, JRIvsd2n, 
C16_MGTA, JRIvsd2n,  C16_MGTA, JRIvsd2n, 
C16_YELW, C16_YELW, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
C16_YELW, C16_YELW,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  C16_YELW, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
C16_YELW, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n, JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
JRIvsd2n, JRIvsd2n,  JRIvsd2n, JRIvsd2n, 
C16_RED , C16_LRED,  C16_MGTA, C16_LRED, C16_MGTA, C16_LMGT, 
C16_MGTA, C16_LMGT,  C16_RED , C16_LMGT, 
C16_MGTA, C16_LMGT,  C16_MGTA, C16_LMGT, 
C16_MGTA, C16_LMGT, 
C16_YELW, C16_LMGT,  JRIvsd2n, C16_LMGT, JRIvsd2n, C16_LMGT, 
JRIvsd2n, C16_LMGT,  C16_YELW, C16_LMGT, 
JRIvsd2n, C16_LMGT,  JRIvsd2n, C16_LMGT, 
JRIvsd2n, C16_LMGT,  C16_YELW, C16_LYLW, 
JRIvsd2n, C16_LYLW, 
JRIvsd2n, C16_WHTE,  C16_YELW, C16_LYLW, 
JRIvsd2n, C16_LYLW,  JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_LYLW,  JRIvsd2n, C16_LYLW, 
JRIvsd2n, C16_WHTE, JRIvsd2n, C16_WHTE,  JRIvsd2n, C16_LYLW, 
JRIvsd2n, C16_LYLW,  JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_WHTE,  C16_LRED, C16_LRED, 
C16_LRED, C16_LRED, 
C16_LRED, C16_LMGT, C16_LMGT, C16_LMGT,  C16_LRED, C16_LMGT, 
C16_LRED, C16_LMGT,  C16_LRED, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LMGT,  C16_LMGT, C16_LMGT, 
C16_LMGT, C16_LYLW, 
C16_LMGT, C16_LYLW,  C16_LMGT, C16_WHTE, C16_LMGT, C16_WHTE, 
C16_LMGT, C16_LYLW,  C16_LMGT, C16_LYLW, 
C16_LMGT, C16_WHTE,  C16_LMGT, C16_WHTE, 
C16_LYLW, C16_LYLW,  C16_LYLW, C16_LYLW, 
C16_LYLW, C16_WHTE, 
C16_WHTE, C16_WHTE,  C16_LYLW, C16_LYLW, 
C16_LYLW, C16_LYLW,  C16_LYLW, C16_WHTE };
static const BYTE bdnHdpxf[0x7C][2] = { C16_BLUE, C16_BLUE, 
C16_BLUE, C16_BLUE,  C16_LBLU, C16_BLUE, 
C16_BLCK, C16_GREN,  C16_BLUE, C16_CYAN, 
C16_BLUE, C16_CYAN,  C16_LBLU, C16_CYAN, 
C16_GREN, C16_GREN, C16_CYAN, C16_CYAN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN,  C16_GREN, C16_GREN, 
C16_CYAN, C16_CYAN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN, 
C16_GREN, C16_GREN, C16_CYAN, C16_GREN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN,  C16_GREN, C16_GREN, 
C16_CYAN, C16_GREN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN,  C16_LGRN, C16_GREN, C16_LGRN, C16_GREN, 
C16_LGRN, C16_CYAN,  C16_LCYN, C16_CYAN, 
C16_LGRN, C16_GREN,  C16_LGRN, C16_GREN, 
C16_LGRN, C16_CYAN, 
C16_LCYN, C16_CYAN,  C16_RED , C16_RED , C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, 
C16_RED , C16_YELW,  C16_MGTA, C16_GRAY, 
C16_MGTA, C16_GRAY,  C16_MGTA, C16_GRAY, 
C16_YELW, C16_YELW, 
JRIvsd2n, C16_GRAY,  JRIvsd2n, C16_GRAY, 
C16_YELW, C16_YELW,  JRIvsd2n, C16_GRAY, 
JRIvsd2n, C16_GRAY, 
JRIvsd2n, C16_GRAY,  C16_YELW, C16_GRAY, 
JRIvsd2n, C16_GRAY, JRIvsd2n, C16_GRAY,  JRIvsd2n, C16_GRAY, 
C16_YELW, C16_GRAY,  JRIvsd2n, C16_GRAY, 
JRIvsd2n, C16_GRAY,  JRIvsd2n, C16_GRAY, 
JRIvsd2n, C16_GRAY, 
JRIvsd2n, C16_GRAY, JRIvsd2n, C16_GRAY,  JRIvsd2n, C16_GRAY, 
JRIvsd2n, C16_GRAY,  JRIvsd2n, C16_GRAY, 
JRIvsd2n, C16_GRAY,  JRIvsd2n, C16_GRAY, 
C16_RED , C16_RED ,  C16_MGTA, C16_RED , C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_RED , C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA, 
C16_YELW, C16_MGTA,  JRIvsd2n, C16_MGTA, JRIvsd2n, C16_MGTA, 
JRIvsd2n, C16_MGTA,  C16_YELW, C16_MGTA, 
JRIvsd2n, C16_MGTA,  JRIvsd2n, C16_MGTA, 
JRIvsd2n, C16_MGTA,  C16_YELW, C16_YELW, 
JRIvsd2n, C16_YELW, 
JRIvsd2n, C16_WHTE,  C16_YELW, C16_YELW, 
JRIvsd2n, C16_YELW,  JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_YELW,  JRIvsd2n, C16_YELW, 
JRIvsd2n, C16_WHTE, JRIvsd2n, C16_WHTE,  JRIvsd2n, C16_YELW, 
JRIvsd2n, C16_YELW,  JRIvsd2n, C16_WHTE, 
JRIvsd2n, C16_WHTE,  C16_LRED, C16_RED , 
C16_LRED, C16_RED , 
C16_LRED, C16_MGTA, C16_LMGT, C16_MGTA,  C16_LRED, C16_MGTA, 
C16_LRED, C16_MGTA,  C16_LRED, C16_MGTA, 
C16_LMGT, C16_MGTA,  C16_LMGT, C16_MGTA, 
C16_LMGT, C16_MGTA,  C16_LMGT, C16_MGTA, C16_LMGT, C16_MGTA, 
C16_LMGT, C16_MGTA,  C16_LMGT, C16_MGTA, 
C16_LMGT, C16_MGTA,  C16_LMGT, C16_MGTA, 
C16_LMGT, C16_YELW, 
C16_LMGT, C16_YELW,  C16_LMGT, C16_WHTE, C16_LMGT, C16_WHTE, 
C16_LMGT, C16_YELW,  C16_LMGT, C16_YELW, 
C16_LMGT, C16_WHTE,  C16_LMGT, C16_WHTE, 
C16_LYLW, C16_YELW,  C16_LYLW, C16_YELW, 
C16_LYLW, C16_WHTE, 
C16_WHTE, C16_WHTE,  C16_LYLW, C16_YELW, 
C16_LYLW, C16_YELW,  C16_LYLW, C16_WHTE };
static const BYTE KjJA0KYA[0x7C][2] = { C16_BLUE, C16_BLCK, 
C16_BLUE, C16_BLUE,  C16_BLUE, C16_BLUE, 
C16_BLCK, C16_BLCK,  C16_BLUE, C16_BLCK, 
C16_BLUE, C16_BLCK,  C16_BLUE, C16_BLCK, 
C16_GREN, C16_BLCK, C16_CYAN, C16_BLCK,  C16_CYAN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_GREN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_CYAN, C16_BLCK, 
C16_CYAN, C16_BLCK, 
C16_GREN, C16_GREN, C16_CYAN, C16_GREN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN,  C16_GREN, C16_GREN, 
C16_CYAN, C16_GREN,  C16_CYAN, C16_CYAN, 
C16_CYAN, C16_CYAN,  C16_GREN, C16_GREN, C16_GREN, C16_GREN, 
C16_GREN, C16_CYAN,  C16_CYAN, C16_CYAN, 
C16_GREN, C16_GREN,  C16_GREN, C16_GREN, 
C16_GREN, C16_CYAN, 
C16_CYAN, C16_CYAN,  C16_RED , C16_BLCK, C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_RED , C16_BLCK,  C16_MGTA, C16_GRAY, 
C16_MGTA, C16_GRAY,  C16_MGTA, C16_GRAY, 
C16_YELW, C16_BLCK, 
C16_GRAY, C16_GRAY,  C16_GRAY, C16_GRAY, 
C16_YELW, C16_BLCK,  C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY,  C16_YELW, C16_GRAY, 
C16_GRAY, C16_GRAY, C16_GRAY, C16_GRAY,  C16_GRAY, C16_GRAY, 
C16_YELW, C16_GRAY,  C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY,  C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY, C16_GRAY, C16_GRAY,  C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY,  C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY,  C16_GRAY, C16_GRAY, 
C16_RED , C16_RED ,  C16_MGTA, C16_RED , C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_RED , C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA, 
C16_YELW, C16_MGTA,  C16_GRAY, C16_MGTA, C16_GRAY, C16_MGTA, 
C16_GRAY, C16_MGTA,  C16_YELW, C16_MGTA, 
C16_GRAY, C16_MGTA,  C16_GRAY, C16_MGTA, 
C16_GRAY, C16_MGTA,  C16_YELW, C16_YELW, 
C16_GRAY, C16_YELW, 
C16_GRAY, C16_GRAY,  C16_YELW, C16_YELW, 
C16_GRAY, C16_YELW,  C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY, 
C16_GRAY, C16_YELW,  C16_GRAY, C16_YELW, 
C16_GRAY, C16_GRAY, C16_GRAY, C16_GRAY,  C16_GRAY, C16_YELW, 
C16_GRAY, C16_YELW,  C16_GRAY, C16_GRAY, 
C16_GRAY, C16_GRAY,  C16_RED , C16_RED , 
C16_RED , C16_RED , 
C16_RED , C16_MGTA, C16_MGTA, C16_MGTA,  C16_RED , C16_MGTA, 
C16_RED , C16_MGTA,  C16_RED , C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, 
C16_MGTA, C16_MGTA,  C16_MGTA, C16_MGTA, 
C16_MGTA, C16_YELW, 
C16_MGTA, C16_YELW,  C16_MGTA, C16_GRAY, C16_MGTA, C16_GRAY, 
C16_MGTA, C16_YELW,  C16_MGTA, C16_YELW, 
C16_MGTA, C16_GRAY,  C16_MGTA, C16_GRAY, 
C16_YELW, C16_YELW,  C16_YELW, C16_YELW, 
C16_YELW, C16_GRAY, 
C16_WHTE, C16_GRAY,  C16_YELW, C16_YELW, 
C16_YELW, C16_YELW,  C16_YELW, C16_GRAY };
static const BYTE ZOq5qzwQ[0x7C][2] = { C16_BLUE, C16_BLCK, 
C16_BLUE, C16_BLCK,  C16_BLUE, C16_BLCK, 
C16_BLCK, C16_BLCK,  C16_BLUE, C16_BLCK, 
C16_BLUE, C16_BLCK,  C16_BLUE, C16_BLCK, 
C16_GREN, C16_BLCK, C16_CYAN, C16_BLCK,  C16_CYAN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_GREN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_CYAN, C16_BLCK, 
C16_CYAN, C16_BLCK, 
C16_GREN, C16_BLCK, C16_CYAN, C16_BLCK,  C16_CYAN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_GREN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_CYAN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_GREN, C16_BLCK, C16_GREN, C16_BLCK, 
C16_GREN, C16_BLCK,  C16_CYAN, C16_BLCK, 
C16_GREN, C16_BLCK,  C16_GREN, C16_BLCK, 
C16_GREN, C16_BLCK, 
C16_CYAN, C16_BLCK,  C16_RED , C16_BLCK, C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_RED , C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_YELW, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_YELW, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_YELW, C16_BLCK, 
C16_GRAY, C16_BLCK, C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_YELW, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK, C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_RED , C16_BLCK,  C16_MGTA, C16_BLCK, C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_RED , C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK, 
C16_YELW, C16_BLCK,  C16_GRAY, C16_BLCK, C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_YELW, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_YELW, C16_BLCK, 
C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_YELW, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK, C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_GRAY, C16_BLCK, 
C16_GRAY, C16_BLCK,  C16_RED , C16_BLCK, 
C16_RED , C16_BLCK, 
C16_RED , C16_BLCK, C16_MGTA, C16_BLCK,  C16_RED , C16_BLCK, 
C16_RED , C16_BLCK,  C16_RED , C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_MGTA, C16_BLCK,  C16_MGTA, C16_BLCK, 
C16_YELW, C16_BLCK,  C16_YELW, C16_BLCK, 
C16_YELW, C16_BLCK, 
C16_WHTE, C16_BLCK,  C16_YELW, C16_BLCK, 
C16_YELW, C16_BLCK,  C16_YELW, C16_BLCK };
static const BYTE* yN7fykzl[7] = { (BYTE*)tWA94Aka,
(BYTE*)wwIRCwE5, (BYTE*)reI5L8za, (BYTE*)nsZP3ml5,
(BYTE*)bdnHdpxf, (BYTE*)KjJA0KYA, (BYTE*)ZOq5qzwQ
}; static BYTE ysPpvW0m[0x7C][2]; 
static BYTE jYPKuxnw[2]; 
static const BYTE wxQOsShb[11] = { 0, 1, 2, 1, 2, 3, 0, 1, 2, 3, 0 };
static const BYTE H1sBAY04[11] = { 1, 2, 3, 0, 1, 2, 1, 2, 1, 2, 1 };
static const BYTE oJWdHhFi[11] = { 0, 1, 2, 1, 2, 3, 1, 2, 1, 2, 2 };
static const BYTE i3tjYRQj[11] = { 1, 2, 3, 0, 1, 2, 2, 3, 0, 1, 3 };
static const BYTE xc9U5L3x[11] = { 0, 1, 2, 1, 2, 3, 1, 2, 1, 2, 2 };
static const BYTE v9AXzz0Z[11] = { 1, 2, 3, 0, 1, 2, 1, 2, 1, 2, 1 };
static const BYTE* JqAuamKe[6] =  { wxQOsShb,
H1sBAY04, oJWdHhFi, i3tjYRQj, xc9U5L3x, v9AXzz0Z
}; static BYTE* fcQTGqqD; 
static const BYTE caJazpv1[42] = {
3, 109, 43, 3, 137, 55, 3, 43, 23, 3, 46, 24, 3, 18, 16, 3, 112, 48, 3, 109, 
47, 109, 3, 137, 55, 138, 3, 43, 23, 43, 3, 46, 24, 46, 3, 18, 16, 18, 3, 112, 
48, 112 }; static const BYTE YNzmk8Mt[42] = {
109, 0, 109, 137, 0, 137, 43, 0, 43, 46, 0, 46, 18, 0, 18, 112, 0, 112, 47, 118,
78, 117, 55, 138, 94, 105, 23, 75, 31, 44, 24, 74, 33, 34, 16, 57, 17, 50, 48, 118,
80, 111 }; static const BYTE aoCoeMc6[42] = {
3, 109, 43, 3, 137, 55, 3, 43, 23, 3, 46, 24, 3, 18, 16, 3, 112, 48, 78, 127,
78, 125, 94, 139, 94, 74, 23, 107, 31, 45, 33, 106, 33, 26, 17, 96, 17, 81, 80, 126,
80, 110 }; static const BYTE dpv7Xqnw[42] = {
109, 0, 109, 137, 0, 137, 43, 0, 43, 46, 0, 46, 18, 18, 18, 112, 0, 112, 109, 0,
109, 137, 138, 0, 138, 43, 43, 0, 43, 46, 46, 0, 46, 18, 18, 0, 18, 112, 112, 0,
112, 109 }; static const BYTE n4emhOcH[42] = {
3, 109, 43, 3, 137, 55, 3, 43, 23, 3, 46, 24, 3, 18, 16, 3, 112, 48, 78, 127,
78, 125, 94, 139, 94, 74, 23, 107, 31, 45, 33, 106, 33, 26, 17, 96, 17, 81, 80, 126,
80, 110 }; static const BYTE JTgvU2RB[42] = {
109, 0, 109, 137, 0, 137, 43, 0, 43, 46, 0, 46, 18, 18, 18, 112, 0, 112, 47, 118,
78, 117, 55, 138, 94, 105, 23, 75, 31, 44, 24, 74, 33, 34, 16, 57, 17, 50, 48, 118,
80, 111 }; static const BYTE* Btd8PqaC[6] = 
{ caJazpv1, YNzmk8Mt, aoCoeMc6, dpv7Xqnw,
n4emhOcH, JTgvU2RB }; static BYTE* Jy2VajCp; 
static BYTE kkNN5Ev6; 
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void RVhGK7nm(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void wdYZBfxC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void (*SZysP4qw[])(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp) = {
&iW0pdjQP, &pohX7Er3, &jGtO1FB9, &iW0pdjQP,
&pohX7Er3, &jGtO1FB9, &RVhGK7nm }; static void (*inTEQRbH[])(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp) = {
&IOd5DYzC, &VJ9k9RQN, &M1P1NHy9, &IOd5DYzC,
&VJ9k9RQN, &M1P1NHy9, &wdYZBfxC };
void mil_ClearWhite()
{ CKOk9Z4Q(JZagjmLi, (C16_WHTE << 4) | C16_WHTE, AvwqZ1W4);
} void mil_ClearBlack() { CKOk9Z4Q(JZagjmLi, (C16_BLCK << 4) | C16_BLCK, AvwqZ1W4);
}
void mil_InitLcd(int lcdWidth, int lcdHeight, BYTE* foreBuf, BYTE* backBuf)
{ JZagjmLi = foreBuf; nmRH4auG = backBuf;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;
NP3y8tCu = (lcdWidth + 1)/2; AvwqZ1W4 = lcdWidth*lcdHeight/2;
fcQTGqqD = (BYTE*)JqAuamKe[0]; Jy2VajCp = (BYTE*)Btd8PqaC[0];
mil_SetGamma(3); mil_SetColor(S_BLACK); mil_ResetClip();
}
void mil_Clear(int D397hZog)
{ BYTE m3ESBpLZ; if (D397hZog == S_TRANSPARENT)
D397hZog = S_WHITE; if (D397hZog < tyJDygB4) 
{ m3ESBpLZ = PlsvbI8t[D397hZog][0]; } else if (D397hZog < k2ax889Q) 
{ m3ESBpLZ = PlsvbI8t[fcQTGqqD[D397hZog - tyJDygB4]][0];
} else if (D397hZog < OWsyYiyk) 
{ m3ESBpLZ = ysPpvW0m[D397hZog - k2ax889Q][0];
} else 
{ if(Jy2VajCp[D397hZog - OWsyYiyk] < tyJDygB4) 
m3ESBpLZ = PlsvbI8t[Jy2VajCp[D397hZog - OWsyYiyk]][0];
else  m3ESBpLZ = ysPpvW0m[Jy2VajCp[D397hZog - OWsyYiyk] - k2ax889Q][0];
} CKOk9Z4Q(JZagjmLi, (m3ESBpLZ << 4) | m3ESBpLZ, AvwqZ1W4);
}
void mil_PutPixel(int LbPt5vw8, int y, int D397hZog)
{ BYTE *kor8Q3C3; BYTE dither; BYTE m3ESBpLZ;
if (D397hZog == S_TRANSPARENT) return; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU || y < s_ClipY1 || y > s_ClipY2)
return; dither = (LbPt5vw8%2 == y%2) ? 0 : 1;
if (D397hZog < tyJDygB4) 
{ m3ESBpLZ = PlsvbI8t[D397hZog][dither]; }
else if (D397hZog < k2ax889Q) 
{ m3ESBpLZ = PlsvbI8t[fcQTGqqD[D397hZog - tyJDygB4]][dither];
} else if (D397hZog < OWsyYiyk) 
{ m3ESBpLZ = ysPpvW0m[D397hZog - k2ax889Q][dither];
} else 
{ if(Jy2VajCp[D397hZog - OWsyYiyk] < tyJDygB4) 
m3ESBpLZ = PlsvbI8t[Jy2VajCp[D397hZog - OWsyYiyk]][dither];
else  m3ESBpLZ = ysPpvW0m[Jy2VajCp[D397hZog - OWsyYiyk] - k2ax889Q][dither];
} kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 1)];

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
if (LbPt5vw8%2 == 0) *kor8Q3C3 = (*kor8Q3C3 & 0x0F) | (m3ESBpLZ << 4); 
else *kor8Q3C3 = (*kor8Q3C3 & 0xF0) | m3ESBpLZ; 

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
if (LbPt5vw8%2 == 0) *kor8Q3C3 = (*kor8Q3C3 & 0xF0) | m3ESBpLZ; 
else *kor8Q3C3 = (*kor8Q3C3 & 0x0F) | (m3ESBpLZ << 4); 

#endif
}
static void bjupxEy9(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE dither; BYTE m3ESBpLZ;
if (kkNN5Ev6 != S_TRANSPARENT) { kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 1)];
dither = (LbPt5vw8%2 == y%2) ? 0 : 1; m3ESBpLZ = jYPKuxnw[dither];

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
if (LbPt5vw8%2 == 0) *kor8Q3C3 = (*kor8Q3C3 & 0x0F) | (m3ESBpLZ << 4); 
else *kor8Q3C3 = (*kor8Q3C3 & 0xF0) | m3ESBpLZ; 

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
if (LbPt5vw8%2 == 0) *kor8Q3C3 = (*kor8Q3C3 & 0xF0) | m3ESBpLZ; 
else *kor8Q3C3 = (*kor8Q3C3 & 0x0F) | (m3ESBpLZ << 4); 

#endif
} }
void mil_SetColor(int D397hZog)
{ b5Lr0cwi = D397hZog; kkNN5Ev6 = D397hZog;
if (D397hZog != S_TRANSPARENT) { if (D397hZog < tyJDygB4) 
{ sW4PB08i(jYPKuxnw, PlsvbI8t[D397hZog], 2);
} else if (D397hZog < k2ax889Q) 
{ sW4PB08i(jYPKuxnw, PlsvbI8t[fcQTGqqD[D397hZog - tyJDygB4]], 2);
} else if (D397hZog < OWsyYiyk) 
{ sW4PB08i(jYPKuxnw, ysPpvW0m[D397hZog - k2ax889Q], 2);
} else 
{ if(Jy2VajCp[D397hZog - OWsyYiyk]<tyJDygB4)
sW4PB08i(jYPKuxnw, PlsvbI8t[Jy2VajCp[D397hZog - OWsyYiyk]], 2);
else  sW4PB08i(jYPKuxnw, ysPpvW0m[Jy2VajCp[D397hZog - OWsyYiyk] - k2ax889Q], 2);
} } cAQOtAsc = bjupxEy9; }
void mil_SetBlinkColor(int frame6)
{ fcQTGqqD = (BYTE*)JqAuamKe[frame6]; Jy2VajCp = (BYTE*)Btd8PqaC[frame6];
if (kkNN5Ev6 != S_TRANSPARENT) { if (kkNN5Ev6 < tyJDygB4) 
{ sW4PB08i(jYPKuxnw, PlsvbI8t[kkNN5Ev6], 2);
} else if (kkNN5Ev6 < k2ax889Q) 
{ sW4PB08i(jYPKuxnw, PlsvbI8t[fcQTGqqD[kkNN5Ev6 - tyJDygB4]], 2);
} else if (kkNN5Ev6 < OWsyYiyk) 
{ sW4PB08i(jYPKuxnw, ysPpvW0m[kkNN5Ev6 - k2ax889Q], 2);
} else 
{ if(Jy2VajCp[kkNN5Ev6 - OWsyYiyk]<tyJDygB4) 
sW4PB08i(jYPKuxnw, PlsvbI8t[Jy2VajCp[kkNN5Ev6 - OWsyYiyk]], 2);
else  sW4PB08i(jYPKuxnw, ysPpvW0m[Jy2VajCp[kkNN5Ev6 - OWsyYiyk] - k2ax889Q], 2);
} } }
void mil_SetGamma(int Gamma)
{ U6liRwBT = Gamma; sW4PB08i(PlsvbI8t, HjvlX3r5[Gamma], sizeof(PlsvbI8t));
sW4PB08i(ysPpvW0m, yN7fykzl[Gamma], sizeof(ysPpvW0m));
mil_SetColor(kkNN5Ev6); }
int mil_GetPixel(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; if( LbPt5vw8 >= 0 && LbPt5vw8 < h8iL9Zij && y >= 0 && y < UwJNuSkK ){
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 1)];

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
if (LbPt5vw8%2 == 0) return (( *kor8Q3C3 & 0xF0 ) >> 4 );
else return ( *kor8Q3C3 & 0x0F ); 
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
if (LbPt5vw8%2 == 0) return ( *kor8Q3C3 & 0x0F );
else return (( *kor8Q3C3 & 0xF0 ) >> 4 );

#endif
} else { return -1;
} }
void mil_InvertRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ BYTE *kor8Q3C3; int OOzmyCcI, N3jRR8ZR;
int mSdyqwa7; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; } if( CYVLXJkW < f3CK6csT ) CYVLXJkW = f3CK6csT;
else if( CYVLXJkW > Kh97ARZU ) CYVLXJkW = Kh97ARZU;
if( LINj8eGO < f3CK6csT ) LINj8eGO = f3CK6csT;
else if( LINj8eGO > Kh97ARZU ) LINj8eGO = Kh97ARZU;
if( qb0L1TPI < s_ClipY1 ) qb0L1TPI = s_ClipY1;
else if( qb0L1TPI > s_ClipY2 ) qb0L1TPI = s_ClipY2;
if( TYFI6PC8 < s_ClipY1 ) TYFI6PC8 = s_ClipY1;
else if( TYFI6PC8 > s_ClipY2 ) TYFI6PC8 = s_ClipY2;
for( N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++ ){
kor8Q3C3 = &JZagjmLi[N3jRR8ZR*NP3y8tCu + (CYVLXJkW >> 1)];
if(( CYVLXJkW % 2 ) != 0 ){ 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*kor8Q3C3 = ( *kor8Q3C3 ^ 0x0F ); 
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*kor8Q3C3 = ( *kor8Q3C3 ^ 0xF0 ); 
#endif
kor8Q3C3++; } for( OOzmyCcI = (( CYVLXJkW + 1 ) / 2 ); OOzmyCcI <= (( LINj8eGO - 1 ) / 2 ); OOzmyCcI++ ){
*kor8Q3C3 = ~( *kor8Q3C3 ); kor8Q3C3++; }
if(( LINj8eGO % 2 ) == 0 ){ 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*kor8Q3C3 = ( *kor8Q3C3 ^ 0xF0 ); 
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*kor8Q3C3 = ( *kor8Q3C3 ^ 0x0F ); 
#endif
} } }
void mil_ScrollLCD(int lcd, int dx, int dy, int wrap)
{ BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE]; int tdx;
if(( wrap & ( ~0x01 )) || ( lcd & ( ~0x01 )))
return; if( lcd == 0 ){ tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if( dx > h8iL9Zij || dx < -h8iL9Zij ){
if( wrap == 1 ){ dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } } if( dx != 0 ){
tdx = dx; for( N3jRR8ZR = 0; N3jRR8ZR < UwJNuSkK; N3jRR8ZR++ ){
dx = tdx; if( dx % 2 ){ 
kor8Q3C3 = &tempLcd[N3jRR8ZR * NP3y8tCu];
s_ScrollBuffer[0] = kor8Q3C3[NP3y8tCu-1];

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
for( OOzmyCcI = ( NP3y8tCu - 1 ); OOzmyCcI > 0; OOzmyCcI-- ){
kor8Q3C3[OOzmyCcI] = (( kor8Q3C3[OOzmyCcI-1] << 4 ) & 0xF0 ) | (( kor8Q3C3[OOzmyCcI] >> 4 ) & 0x0F );
} kor8Q3C3[0] = (( kor8Q3C3[0] >> 4 ) & 0x0F ) | (( s_ScrollBuffer[0] << 4 ) & 0xF0 );
if( wrap != 1 ){ if( dx > 0 ){ kor8Q3C3[0] = ( kor8Q3C3[0] & 0x0F ) | ( C16_WHTE << 4 );
}  }

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
for( OOzmyCcI = ( NP3y8tCu - 1 ); OOzmyCcI > 0; OOzmyCcI-- ){
kor8Q3C3[OOzmyCcI] = (( kor8Q3C3[OOzmyCcI] << 4 ) & 0xF0 ) | (( kor8Q3C3[OOzmyCcI-1] >> 4 ) & 0x0F );
} kor8Q3C3[0] = (( kor8Q3C3[0] << 4 ) & 0xF0 ) | (( s_ScrollBuffer[0] >> 4 ) & 0x0F );
if( wrap != 1 ){ if( dx > 0 ){ kor8Q3C3[0] = ( kor8Q3C3[0] & 0xF0 ) | ( C16_WHTE & 0x0F );
}  }

#endif
dx -= 1; } if (dx == 0) continue; dx /= 2;
if( dx > 0 ){ 
if( wrap == 1 ){ 
kor8Q3C3 = &tempLcd[(N3jRR8ZR+1)*NP3y8tCu - dx];
sW4PB08i( s_ScrollBuffer, kor8Q3C3, dx );
} kor8Q3C3 = &tempLcd[N3jRR8ZR * NP3y8tCu];
GICEruWy( kor8Q3C3 + dx, kor8Q3C3, NP3y8tCu - dx );
if( wrap == 1 ){ sW4PB08i( kor8Q3C3, s_ScrollBuffer, dx );
} else { CKOk9Z4Q( kor8Q3C3, (( C16_WHTE << 4 ) | C16_WHTE ), dx );
} } else if( dx < 0 ){ 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; if( wrap == 1 ){
sW4PB08i( s_ScrollBuffer, kor8Q3C3, -dx );
} GICEruWy( kor8Q3C3, kor8Q3C3 - dx, NP3y8tCu + dx );
kor8Q3C3 = &tempLcd[( N3jRR8ZR + 1 ) * NP3y8tCu + dx];
if( wrap == 1 ){ sW4PB08i( kor8Q3C3, s_ScrollBuffer, -dx );
} else { CKOk9Z4Q( kor8Q3C3, (( C16_WHTE << 4 ) | C16_WHTE ), -dx );
} } } } if( dy > UwJNuSkK || dy < -UwJNuSkK ){
if( wrap == 1 ){ dy %= UwJNuSkK; } else {
dy = UwJNuSkK; } }
if( dy != 0 ){
for( OOzmyCcI = 0; OOzmyCcI < NP3y8tCu; OOzmyCcI++ ){
if( dy > 0 ){ 
kor8Q3C3 = &tempLcd[(( UwJNuSkK - dy ) * NP3y8tCu ) + OOzmyCcI];
if( wrap == 1 ){ for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++, kor8Q3C3 += NP3y8tCu ){
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; } }
kor8Q3C3 = &tempLcd[OOzmyCcI]; for( N3jRR8ZR = ( UwJNuSkK - dy - 1 ); N3jRR8ZR >= 0; N3jRR8ZR-- ){
kor8Q3C3[(N3jRR8ZR + dy) * NP3y8tCu] = kor8Q3C3[N3jRR8ZR * NP3y8tCu];
} if( wrap == 1 ){ for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = ( C16_WHTE << 4 ) | C16_WHTE;
} } } else { 
kor8Q3C3 = &tempLcd[OOzmyCcI]; if( wrap == 1 ){
for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
s_ScrollBuffer[N3jRR8ZR] = kor8Q3C3[N3jRR8ZR * NP3y8tCu];
} } for( N3jRR8ZR = 0; N3jRR8ZR < ( UwJNuSkK + dy ); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = kor8Q3C3[( N3jRR8ZR - dy ) * NP3y8tCu];
} kor8Q3C3 = &tempLcd[( UwJNuSkK + dy ) * NP3y8tCu + OOzmyCcI];
if( wrap == 1 ){ for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = ( C16_WHTE << 4 ) | C16_WHTE;
} } } } } }
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2][2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
int H0Ng1GoT; if (VDI_TYPE(TB4GUKNg) <= S_IMG_GRAY4)
{ 
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); LG0dStpd[0][0] = (*ObyzSETp & 0xf0) >> 4;
LG0dStpd[1][0] = (*ObyzSETp & 0x0f); } else
{ 
r2Z9u4o4 = VDI_BITMAP_COLOR1(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 2; H0Ng1GoT++)
LG0dStpd[H0Ng1GoT][0] = *(ObyzSETp + H0Ng1GoT);
} for (H0Ng1GoT = 0; H0Ng1GoT < 2; H0Ng1GoT++)
{ if (LG0dStpd[H0Ng1GoT][0] < tyJDygB4) sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[LG0dStpd[H0Ng1GoT][0]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < k2ax889Q)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[LG0dStpd[H0Ng1GoT][0] - tyJDygB4]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < OWsyYiyk)
sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[LG0dStpd[H0Ng1GoT][0] - k2ax889Q], 2);
else{ if(Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]<tyJDygB4)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]], 2);
else  sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]- k2ax889Q], 2);
} } vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + (cx >> 1);
kT6WFmL3 = cx & 0x01;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj)) & 0x01;
uidVeSnj++; jzr2C3mn = LG0dStpd[KrspwWdU][dMask1];
if (v32pnQd7 == 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
v32pnQd7++; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
FcyUMPlI++; v32pnQd7 = 0; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4][2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
int H0Ng1GoT; if (VDI_TYPE(TB4GUKNg) <= S_IMG_GRAY4)
{ 
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); LG0dStpd[0][0] = (*ObyzSETp & 0xf0) >> 4;
LG0dStpd[1][0] = (*ObyzSETp++ & 0x0f) ; LG0dStpd[2][0] = (*ObyzSETp & 0xf0) >> 4;
LG0dStpd[3][0] = (*ObyzSETp & 0x0f) ; } else
{ 
r2Z9u4o4 = VDI_BITMAP_COLOR2(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 4; H0Ng1GoT++)
LG0dStpd[H0Ng1GoT][0] = *(ObyzSETp + H0Ng1GoT);
} for (H0Ng1GoT = 0; H0Ng1GoT < 4; H0Ng1GoT++)
{ if (LG0dStpd[H0Ng1GoT][0] < tyJDygB4) sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[LG0dStpd[H0Ng1GoT][0]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < k2ax889Q)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[LG0dStpd[H0Ng1GoT][0] - tyJDygB4]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < OWsyYiyk)
sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[LG0dStpd[H0Ng1GoT][0] - k2ax889Q], 2);
else{ if(Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]<tyJDygB4)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]], 2);
else  sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]- k2ax889Q], 2);
} } vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + (cx >> 1);
kT6WFmL3 = cx & 0x01;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 1) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU][dMask1];
if (v32pnQd7 == 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
v32pnQd7++; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
FcyUMPlI++; v32pnQd7 = 0; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[16][2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
int H0Ng1GoT; if (VDI_TYPE(TB4GUKNg) <= S_IMG_GRAY4)
{ 
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 4; H0Ng1GoT++)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[H0Ng1GoT], 2);
LG0dStpd[S_TRANSPARENT][0] = EW6LVBlK; LG0dStpd[S_TRANSPARENT][1] = EW6LVBlK;
for (H0Ng1GoT = 5; H0Ng1GoT < 16; H0Ng1GoT++)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[H0Ng1GoT - tyJDygB4]], 2);
} else { 
r2Z9u4o4 = VDI_BITMAP_COLOR4(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 16; H0Ng1GoT++)
{ LG0dStpd[H0Ng1GoT][0] = *(ObyzSETp + H0Ng1GoT);
if (LG0dStpd[H0Ng1GoT][0] < tyJDygB4) sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[LG0dStpd[H0Ng1GoT][0]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < k2ax889Q)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[LG0dStpd[H0Ng1GoT][0] - tyJDygB4]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < OWsyYiyk)
sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[LG0dStpd[H0Ng1GoT][0] - k2ax889Q], 2);
else{ if(Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]<tyJDygB4)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]], 2);
else  sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]- k2ax889Q], 2);
} } } vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + (cx >> 1);
kT6WFmL3 = cx & 0x01;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 2) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; jzr2C3mn = LG0dStpd[KrspwWdU][dMask1];
if (v32pnQd7 == 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
v32pnQd7++; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
FcyUMPlI++; v32pnQd7 = 0; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void RVhGK7nm(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_COLOR8(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = (vzL7ZKBJ - qm7J64q3);
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + (cx >> 1);
kT6WFmL3 = cx & 0x01;
CJ6JYaHh = r2Z9u4o4 + (bDefoJ6l * vzL7ZKBJ + IsDSC069);
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = *CJ6JYaHh++; if (KrspwWdU < tyJDygB4)
jzr2C3mn = PlsvbI8t[KrspwWdU][dMask1]; else if (KrspwWdU < k2ax889Q)
jzr2C3mn = PlsvbI8t[fcQTGqqD[KrspwWdU - tyJDygB4]][dMask1];
else if (KrspwWdU < OWsyYiyk) jzr2C3mn = ysPpvW0m[KrspwWdU - k2ax889Q][dMask1];
else{ if(Jy2VajCp[KrspwWdU - OWsyYiyk]<tyJDygB4)
jzr2C3mn = PlsvbI8t[Jy2VajCp[KrspwWdU - OWsyYiyk]][dMask1];
else jzr2C3mn = ysPpvW0m[Jy2VajCp[KrspwWdU - OWsyYiyk]- k2ax889Q][dMask1];
} if (v32pnQd7 == 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
v32pnQd7++; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
FcyUMPlI++; v32pnQd7 = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
dMask0 = 1 - dMask0; } }
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2][2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
int H0Ng1GoT; if (VDI_TYPE(TB4GUKNg) <= S_IMG_GRAY4)
{ 
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); LG0dStpd[0][0] = (*ObyzSETp & 0xf0) >> 4;
LG0dStpd[1][0] = (*ObyzSETp & 0x0f); } else
{ 
r2Z9u4o4 = VDI_BITMAP_COLOR1(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 2; H0Ng1GoT++)
LG0dStpd[H0Ng1GoT][0] = *(ObyzSETp + H0Ng1GoT);
} for (H0Ng1GoT = 0; H0Ng1GoT < 2; H0Ng1GoT++)
{ if (LG0dStpd[H0Ng1GoT][0] < tyJDygB4) sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[LG0dStpd[H0Ng1GoT][0]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < k2ax889Q)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[LG0dStpd[H0Ng1GoT][0] - tyJDygB4]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < OWsyYiyk)
sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[LG0dStpd[H0Ng1GoT][0] - k2ax889Q], 2);
else{ if(Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]<tyJDygB4)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]], 2);
else  sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]- k2ax889Q], 2);
} } vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + ((cx + qm7J64q3 - 1) >> 1);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x01;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj)) & 0x01;
uidVeSnj++; jzr2C3mn = LG0dStpd[KrspwWdU][dMask1];
if (v32pnQd7 != 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
v32pnQd7--; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
FcyUMPlI--; v32pnQd7 = 1; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4][2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
int H0Ng1GoT; if (VDI_TYPE(TB4GUKNg) <= S_IMG_GRAY4)
{ 
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); LG0dStpd[0][0] = (*ObyzSETp & 0xf0) >> 4;
LG0dStpd[1][0] = (*ObyzSETp++ & 0x0f) ; LG0dStpd[2][0] = (*ObyzSETp & 0xf0) >> 4;
LG0dStpd[3][0] = (*ObyzSETp & 0x0f) ; } else
{ 
r2Z9u4o4 = VDI_BITMAP_COLOR2(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 4; H0Ng1GoT++)
LG0dStpd[H0Ng1GoT][0] = *(ObyzSETp + H0Ng1GoT);
} for (H0Ng1GoT = 0; H0Ng1GoT < 4; H0Ng1GoT++)
{ if (LG0dStpd[H0Ng1GoT][0] < tyJDygB4) sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[LG0dStpd[H0Ng1GoT][0]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < k2ax889Q)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[LG0dStpd[H0Ng1GoT][0] - tyJDygB4]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < OWsyYiyk)
sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[LG0dStpd[H0Ng1GoT][0] - k2ax889Q], 2);
else{ if(Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]<tyJDygB4)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]], 2);
else  sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]- k2ax889Q], 2);
} } vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + ((cx + qm7J64q3 - 1) >> 1);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x01;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 1) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU][dMask1];
if (v32pnQd7 != 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
v32pnQd7--; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
FcyUMPlI--; v32pnQd7 = 1; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[16][2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
int H0Ng1GoT; if (VDI_TYPE(TB4GUKNg) <= S_IMG_GRAY4)
{ 
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 4; H0Ng1GoT++)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[H0Ng1GoT], 2);
LG0dStpd[S_TRANSPARENT][0] = EW6LVBlK; LG0dStpd[S_TRANSPARENT][1] = EW6LVBlK;
for (H0Ng1GoT = 5; H0Ng1GoT < 16; H0Ng1GoT++)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[H0Ng1GoT - tyJDygB4]], 2);
} else { 
r2Z9u4o4 = VDI_BITMAP_COLOR4(TB4GUKNg); for (H0Ng1GoT = 0; H0Ng1GoT < 16; H0Ng1GoT++)
{ LG0dStpd[H0Ng1GoT][0] = *(ObyzSETp + H0Ng1GoT);
if (LG0dStpd[H0Ng1GoT][0] < tyJDygB4) sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[LG0dStpd[H0Ng1GoT][0]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < k2ax889Q)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[fcQTGqqD[LG0dStpd[H0Ng1GoT][0] - tyJDygB4]], 2);
else if (LG0dStpd[H0Ng1GoT][0] < OWsyYiyk)
sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[LG0dStpd[H0Ng1GoT][0] - k2ax889Q], 2);
else{ if(Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]<tyJDygB4)
sW4PB08i(LG0dStpd[H0Ng1GoT], PlsvbI8t[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]], 2);
else  sW4PB08i(LG0dStpd[H0Ng1GoT], ysPpvW0m[Jy2VajCp[LG0dStpd[H0Ng1GoT][0] - OWsyYiyk]- k2ax889Q], 2);
} } } vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + ((cx + qm7J64q3 - 1) >> 1);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x01;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; jzr2C3mn = LG0dStpd[KrspwWdU][dMask1];
if (v32pnQd7 != 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
v32pnQd7--; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
FcyUMPlI--; v32pnQd7 = 1; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void wdYZBfxC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_COLOR8(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = (vzL7ZKBJ - qm7J64q3);
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + ((cx + qm7J64q3 - 1) >> 1);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x01;
CJ6JYaHh = r2Z9u4o4 + (bDefoJ6l * vzL7ZKBJ + IsDSC069);
dMask0 = 0; for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = *CJ6JYaHh++; if (KrspwWdU < tyJDygB4)
jzr2C3mn = PlsvbI8t[KrspwWdU][dMask1]; else if (KrspwWdU < k2ax889Q)
jzr2C3mn = PlsvbI8t[fcQTGqqD[KrspwWdU - tyJDygB4]][dMask1];
else if (KrspwWdU < OWsyYiyk) jzr2C3mn = ysPpvW0m[KrspwWdU - k2ax889Q][dMask1];
else{ if(Jy2VajCp[KrspwWdU - OWsyYiyk]< tyJDygB4)
jzr2C3mn = PlsvbI8t[Jy2VajCp[KrspwWdU - OWsyYiyk]][dMask1];
else  jzr2C3mn = ysPpvW0m[Jy2VajCp[KrspwWdU - OWsyYiyk]- k2ax889Q][dMask1];
} if (v32pnQd7 != 0)
{ if (jzr2C3mn != EW6LVBlK) 
#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#endif
v32pnQd7--; } else { if (jzr2C3mn != EW6LVBlK)

#if (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF)
*FcyUMPlI = (*FcyUMPlI & 0x0F) | (jzr2C3mn << 4);

#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & 0xF0) | jzr2C3mn;

#endif
FcyUMPlI--; v32pnQd7 = 1; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
dMask0 = 1 - dMask0; } }
void mil_CopyImage(BYTE* TB4GUKNg, int cx, int cy)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg) - S_IMG_GRAY1;
if (type < 0 || 6 < type) return; qm7J64q3 = VDI_WIDTH(TB4GUKNg);
h = VDI_HEIGHT(TB4GUKNg); cx -= VDI_CX(TB4GUKNg);
cy -= VDI_CY(TB4GUKNg); SZysP4qw[type](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} void mil_CopyImageDir(BYTE* TB4GUKNg, int cx, int cy, int W11wt2C6)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg) - S_IMG_GRAY1;
if (type < 0 || 6 < type) return; qm7J64q3 = VDI_WIDTH(TB4GUKNg);
h = VDI_HEIGHT(TB4GUKNg); if (!W11wt2C6) {
cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} else { cx -= VDI_WIDTH(TB4GUKNg) - VDI_CX(TB4GUKNg);
cy -= VDI_CY(TB4GUKNg); inTEQRbH[type](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} }
void mil_CopyImagePal(BYTE* TB4GUKNg, int cx, int cy, BYTE *OEjq6ZgK)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg) - S_IMG_GRAY1;
if (type < 0 || 6 < type) return; qm7J64q3 = VDI_WIDTH(TB4GUKNg);
h = VDI_HEIGHT(TB4GUKNg); cx -= VDI_CX(TB4GUKNg);
cy -= VDI_CY(TB4GUKNg); SZysP4qw[type](TB4GUKNg, cx, cy, qm7J64q3, h, (BYTE*)OEjq6ZgK);
}
void mil_CopyImageDirPal(BYTE* TB4GUKNg, int cx, int cy, int W11wt2C6, BYTE *OEjq6ZgK)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg) - S_IMG_GRAY1;
if (type < 0 || 6 < type) return; qm7J64q3 = VDI_WIDTH(TB4GUKNg);
h = VDI_HEIGHT(TB4GUKNg); if (!W11wt2C6) {
cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type](TB4GUKNg, cx, cy, qm7J64q3, h, (BYTE*)OEjq6ZgK);
} else { cx -= VDI_WIDTH(TB4GUKNg) - VDI_CX(TB4GUKNg);
cy -= VDI_CY(TB4GUKNg); inTEQRbH[type](TB4GUKNg, cx, cy, qm7J64q3, h, (BYTE*)OEjq6ZgK);
} } 
#endif 

#endif 
