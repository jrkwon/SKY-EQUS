//==============================================================================
//
//	SINJISOFT GAME LIBRARY
//
//	FILE NAME
//		GLibScore.cpp
//
//	DESCRIPTION
//		Score manager routines
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
//
//==============================================================================

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "GLib.h"

#if defined (SKTT7) 

#define drSwT6Gs 89

#define x8GCfooj ((G_SCOREBOARD_SIZE)*12 - 2)

#else

#define drSwT6Gs 105

#define x8GCfooj ((G_SCOREBOARD_SIZE)*17 - 2)

#endif

#define z48637g9 " " 

#if defined (LGIC13) || defined (LGIC14)

#include "letter_LGIC13.cpp"

#endif

#if defined (SAMSUNG1) || defined (SAMSUNG2)

#define KGRTuzlW 1 
static const char *F6VdJTwN[] = {
".", "@", "-", "_", "/", ":", "~", "1", 
"A", "B", "C", "2", 
"D", "E", "F", "3", 
"G", "H", "I", "4", 
"J", "K", "L", "5", 
"M", "N", "O", "6", 
"P", "Q", "R", "S", "7", 
"T", "U", "V", "8", 
"W", "X", "Y", "Z", "9", 
"0", 
"*", 
"#", 
}; static const int ND5kuLMs[KGRTuzlW][12] = {
8, 4, 4, 4, 4, 4, 5, 4, 5, 1, 1, 1 }; static const int Fd0iW106[KGRTuzlW][12] = {
0, 8, 12, 16, 20, 24, 28, 33, 37, 42, 43, 44
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu GLIB_KEY_LEFT

#define Gte7bGwX GLIB_KEY_RIGHT

#define jynRrmeb GLIB_KEY_CLR

#elif defined(SKTT3) || defined (SKTT3E) || defined (SKTT3M) || defined (SKTT3MA2) || defined (SKTT4) || defined (SKTT5) || defined (SKTT6)

#define KGRTuzlW 1
static const char *F6VdJTwN[] = {
".", "Q", "Z", "1", 
"A", "B", "C", "2", 
"D", "E", "F", "3", 
"G", "H", "I", "4", 
"J", "K", "L", "5", 
"M", "N", "O", "6", 
"P", "R", "S", "7", 
"T", "U", "V", "8", 
"W", "X", "Y", "9", 
"0", "@", 
"*", 
"#", 
}; static const int ND5kuLMs[KGRTuzlW][12] = {
4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 1, 1 }; static const int Fd0iW106[KGRTuzlW][12] = {
0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 38, 39
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu GLIB_KEY_LEFT

#define Gte7bGwX GLIB_KEY_RIGHT

#define jynRrmeb GLIB_KEY_CLR

#elif defined (SKTT7)

#define KGRTuzlW 1
static const char *F6VdJTwN[] = {
"&", "@", "1", 
"A", "B", "C", "2", 
"D", "E", "F", "3", 
"G", "H", "I", "4", 
"J", "K", "L", "5", 
"M", "N", "O", "6", 
"P", "Q", "R", "S", "7", 
"T", "U", "V", "8", 
"W", "X", "Y", "Z", "9", 
"0", 
"*", 
"#", 
}; static const int ND5kuLMs[KGRTuzlW][12] = {
3, 4, 4, 4, 4, 4, 5, 4, 5, 1, 1, 1 }; static const int Fd0iW106[KGRTuzlW][12] = {
0, 3, 7, 11, 15, 19, 23, 28, 32, 37, 38, 39
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu 0xFF

#define Gte7bGwX GLIB_KEY_RIGHT

#define jynRrmeb GLIB_KEY_CLR

#elif defined(WORLDMTEL)

#define KGRTuzlW 1
static const char *F6VdJTwN[] = {
"1", 
"A", "B", "C", "2", 
"D", "E", "F", "3", 
"G", "H", "I", "4", 
"J", "K", "L", "5", 
"M", "N", "O", "6", 
"P", "Q", "R", "S", "7", 
"T", "U", "V", "8", 
"W", "X", "Y", "Z", "9", 
"0", 
"*", 
"#", 
}; static const int ND5kuLMs[KGRTuzlW][12] = {
1, 4, 4, 4, 4, 4, 5, 4, 5, 1, 1, 1 }; static const int Fd0iW106[KGRTuzlW][12] = {
0, 1, 5, 9, 13, 17, 21, 26, 30, 35, 36, 37
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu GLIB_KEY_LEFT

#define Gte7bGwX GLIB_KEY_RIGHT

#define jynRrmeb GLIB_KEY_CLR

#elif defined(SMARTMOBILE) || defined (WIDE2)

#define KGRTuzlW 1
static const char *F6VdJTwN[] = {
".", "Q", "Z", "1", 
"A", "B", "C", "2", 
"D", "E", "F", "3", 
"G", "H", "I", "4", 
"J", "K", "L", "5", 
"M", "N", "O", "6", 
"P", "R", "S", "7", 
"T", "U", "V", "8", 
"W", "X", "Y", "9", 
"0", "@", 
"*", 
"#", 
}; static const int ND5kuLMs[KGRTuzlW][12] = {
4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 1, 1 }; static const int Fd0iW106[KGRTuzlW][12] = {
0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 38, 39
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu GLIB_KEY_LEFT

#define Gte7bGwX GLIB_KEY_RIGHT

#define jynRrmeb GLIB_KEY_CLR

#elif defined(LGIC11)

#define KGRTuzlW 1
static const char *F6VdJTwN[] = {
"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", 
"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", 
"U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", 
"5", "6", "7", "8", "9", "0", "*", "#", ".", "@", 
"-", "_", "/", ":", "~" }; static const int ND5kuLMs[KGRTuzlW][1] = {
45 }; static const int Fd0iW106[KGRTuzlW][1] = {
0 };

#define d9AshwlE 0xFFFF

#define A6pcPPFu 0xFFFF

#define Gte7bGwX GLIB_KEY_RIGHT

#define jynRrmeb GLIB_KEY_LEFT

#elif (SWAP_LANG == SWAP_ASCII || SWAP_LANG == SWAP_KSC5601)

#define KGRTuzlW 2
static const char *lu0rwjYj[] = { "Uppercase",
"Lowercase" };

#if defined (LGIC9) 
static const char *F6VdJTwN[] = { "1", ".", ",", "?", "!", "-", "'", "@", ":", 
"2", "A", "B", "C", 
"3", "D", "E", "F", 
"4", "G", "H", "I", 
"5", "J", "K", "L", 
"6", "M", "N", "O", 
"7", "P", "Q", "R", "S", 
"8", "T", "U", "V", 
"9", "W", "X", "Y", "Z", 
"0", 
"1", ".", ",", "?", "!", "-", "'", "@", ":", 
"2", "a", "b", "c", 
"3", "d", "e", "f", 
"4", "g", "h", "i", 
"5", "j", "k", "l", 
"6", "m", "n", "o", 
"7", "p", "q", "r", "s", 
"8", "t", "u", "v", 
"9", "w", "x", "y", "z", 
"0" 
}; static const int ND5kuLMs[KGRTuzlW][10] = {
9, 4, 4, 4, 4, 4, 5, 4, 5, 1, 9, 4, 4, 4, 4, 4, 5, 4, 5, 1
}; static const int Fd0iW106[KGRTuzlW][10] = {
0, 9, 13, 17, 21, 25, 29, 34, 38, 43, 44, 53, 57, 61, 65, 69, 73, 78, 82, 87
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu 0xFFFF 

#define Gte7bGwX GLIB_KEY_SHARP

#define jynRrmeb GLIB_KEY_CLR

#elif defined (LGIC10) 
static const char *F6VdJTwN[] = { "@", "1", "$", ".", ",", "?", "!", "+", "-", "*", "/", "=", "'", "\"", ";", ":", 
"A", "B", "C", "2", 
"D", "E", "F", "3", 
"G", "H", "I", "4", 
"J", "K", "L", "5", 
"M", "N", "O", "6", 
"P", "Q", "R", "S", "7", 
"T", "U", "V", "8", 
"W", "X", "Y", "Z", "9", 
"0", "&", "^", "|", "~", "#", "%", "(", ")", "[", "]", "{", "}", "<", ">","_", 
"@","1", "$", ".", ",", "?", "!", "+", "-", "*", "/", "=", "'", "\"", ";", ":", 
"a", "b", "c", "2", 
"d", "e", "f", "3", 
"g", "h", "i", "4", 
"j", "k", "l", "5", 
"m", "n", "o", "6", 
"p", "q", "r", "s", "7", 
"t", "u", "v", "8", 
"w", "x", "y", "z", "9", 
"0", "&", "^", "|", "~", "#", "%", "(", ")", "[", "]", "{", "}", "<", ">","_" 
}; static const int ND5kuLMs[KGRTuzlW][10] = {
16, 4, 4, 4, 4, 4, 5, 4, 5, 16, 16, 4, 4, 4, 4, 4, 5, 4, 5, 16
}; static const int Fd0iW106[KGRTuzlW][10] = {
0, 16, 20, 24, 28, 32, 36, 41, 45, 50, 66, 82, 86, 90, 94, 98, 102, 107, 111, 116
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu 0xFFFF 

#define Gte7bGwX GLIB_KEY_SHARP

#define jynRrmeb GLIB_KEY_CLR

#define uzBLxvrM GLIB_KEY_CLR_L

#elif defined (GIGATEL1) || defined (GIGATEL2) || defined (GIGATEL1_SINJILOGO) || defined (GIGATEL2_SINJILOGO)
static const char *F6VdJTwN[] = { "1", ".", ",", "?", "!", "-", "'", "@", ":", 
"2", "A", "B", "C", 
"3", "D", "E", "F", 
"4", "G", "H", "I", 
"5", "J", "K", "L", 
"6", "M", "N", "O", 
"7", "P", "Q", "R", "S", 
"8", "T", "U", "V", 
"9", "W", "X", "Y", "Z", 
"0", 
"1", ".", ",", "?", "!", "-", "'", "@", ":", 
"2", "a", "b", "c", 
"3", "d", "e", "f", 
"4", "g", "h", "i", 
"5", "j", "k", "l", 
"6", "m", "n", "o", 
"7", "p", "q", "r", "s", 
"8", "t", "u", "v", 
"9", "w", "x", "y", "z", 
"0" 
}; static const int ND5kuLMs[KGRTuzlW][10] = {
9, 4, 4, 4, 4, 4, 5, 4, 5, 1, 9, 4, 4, 4, 4, 4, 5, 4, 5, 1
}; static const int Fd0iW106[KGRTuzlW][10] = {
0, 9, 13, 17, 21, 25, 29, 34, 38, 43, 44, 53, 57, 61, 65, 69, 73, 78, 82, 87
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu 0xFFFF 

#define Gte7bGwX GLIB_KEY_SHARP

#define jynRrmeb GLIB_KEY_CLR

#elif defined (LGIC12)
static const char *F6VdJTwN[] = {
"1", ".", "Q", "Z", 
"2", "A", "B", "C", 
"3", "D", "E", "F", 
"4", "G", "H", "I", 
"5", "J", "K", "L", 
"6", "M", "N", "O", 
"7", "P", "R", "S", 
"8", "T", "U", "V", 
"9", "W", "X", "Y", 
"0", "@", ",", "!", "?", "*", "/", "#", "$", "%", 
"1", ".", "q", "z", 
"2", "a", "b", "c", 
"3", "d", "e", "f", 
"4", "g", "h", "i", 
"5", "j", "k", "l", 
"6", "m", "n", "o", 
"7", "p", "r", "s", 
"8", "t", "u", "v", 
"9", "w", "x", "y", 
"0", "@", ",", "!", "?", "*", "/", "#", "$", "%" 
}; static const int ND5kuLMs[KGRTuzlW][10] = {
4, 4, 4, 4, 4, 4, 4, 4, 4, 10, 4, 4, 4, 4, 4, 4, 4, 4, 4, 10
}; static const int Fd0iW106[KGRTuzlW][10] = {
0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu 0xFFFF 

#define Gte7bGwX GLIB_KEY_RIGHT

#define jynRrmeb GLIB_KEY_SOFT2

#else
static const char *F6VdJTwN[] = {
"1", ".", "Q", "Z", 
"2", "A", "B", "C", 
"3", "D", "E", "F", 
"4", "G", "H", "I", 
"5", "J", "K", "L", 
"6", "M", "N", "O", 
"7", "P", "R", "S", 
"8", "T", "U", "V", 
"9", "W", "X", "Y", 
"0", "@", ",", "!", "?", "*", "/", "#", "$", "%", 
"1", ".", "q", "z", 
"2", "a", "b", "c", 
"3", "d", "e", "f", 
"4", "g", "h", "i", 
"5", "j", "k", "l", 
"6", "m", "n", "o", 
"7", "p", "r", "s", 
"8", "t", "u", "v", 
"9", "w", "x", "y", 
"0", "@", ",", "!", "?", "*", "/", "#", "$", "%" 
}; static const int ND5kuLMs[KGRTuzlW][10] = {
4, 4, 4, 4, 4, 4, 4, 4, 4, 10, 4, 4, 4, 4, 4, 4, 4, 4, 4, 10
}; static const int Fd0iW106[KGRTuzlW][10] = {
0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 46, 50, 54, 58, 62, 66, 70, 74, 78, 82
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu 0xFFFF 

#define Gte7bGwX GLIB_KEY_SHARP

#define jynRrmeb GLIB_KEY_STAR

#endif 

#endif 

#if (SWAP_LANG == SWAP_SHIFTJIS)

#define KGRTuzlW 4
static const char *lu0rwjYj[] = { "Uppercase",
"Lowercase", "Hiragana", "Katagana", };
static const char *F6VdJTwN[] = {
"1", ".", "@", ",", "!", "?", "*", "/", "#", "$", "%", 
"2", "A", "B", "C", 
"3", "D", "E", "F", 
"4", "G", "H", "I", 
"5", "J", "K", "L", 
"6", "M", "N", "O", 
"7", "P", "Q", "R", "S", 
"8", "T", "U", "V", 
"9", "W", "X", "Y", "Z", 
"0", 
"1", ".", "@", ",", "!", "?", "*", "/", "#", "$", "%", 
"2", "a", "b", "c", 
"3", "d", "e", "f", 
"4", "g", "h", "i", 
"5", "j", "k", "l", 
"6", "m", "n", "o", 
"7", "p", "q", "r", "s", 
"8", "t", "u", "v", 
"9", "w", "x", "y", "z", 
"0", 
"\x82\x9F", "\x82\xA0", "\x82\xA1", "\x82\xA2", "\x82\xA3",
"\x82\xA4", "\x82\xA5", "\x82\xA6", "\x82\xA7", "\x82\xA8", 
"\x82\xA9", "\x82\xAA", "\x82\xAB", "\x82\xAC", "\x82\xAD",
"\x82\xAE", "\x82\xAF", "\x82\xB0", "\x82\xB1", "\x82\xB2", 
"\x82\xB3", "\x82\xB4", "\x82\xB5", "\x82\xB6", "\x82\xB7",
"\x82\xB8", "\x82\xB9", "\x82\xBA", "\x82\xBB", "\x82\xBC", 
"\x82\xBD", "\x82\xBE", "\x82\xBF", "\x82\xC0", "\x82\xC1",
"\x82\xC2", "\x82\xC3", "\x82\xC4", "\x82\xC5", "\x82\xC6", "\x82\xC7", 
"\x82\xC8", "\x82\xC9", "\x82\xCA", "\x82\xCB", "\x82\xCC", 
"\x82\xCD", "\x82\xCE", "\x82\xCF", "\x82\xD0", "\x82\xD1",
"\x82\xD2", "\x82\xD3", "\x82\xD4", "\x82\xD5", "\x82\xD6",
"\x82\xD7", "\x82\xD8", "\x82\xD9", "\x82\xDA", "\x82\xDB", 
"\x82\xDC", "\x82\xDD", "\x82\xDE", "\x82\xDF", "\x82\xE0", 
"\x82\xE1", "\x82\xE2", "\x82\xE3", "\x82\xE4", "\x82\xE5", "\x82\xE6", 
"\x82\xE7", "\x82\xE8", "\x82\xE9", "\x82\xEA", "\x82\xEB", 
"\x82\xEC", "\x82\xED", "\x82\xEE", "\x82\xEF", "\x82\xF0", "\x82\xF1", 
"\x83\x40", "\x83\x41", "\x83\x42", "\x83\x43", "\x83\x44",
"\x83\x45", "\x83\x94", "\x83\x46", "\x83\x47", "\x83\x48", "\x83\x49", 
"\x83\x90", "\x83\x4A", "\x83\x4B", "\x83\x4C", "\x83\x4D", "\x83\x4E",
"\x83\x4F", "\x83\x91", "\x83\x50", "\x83\x51", "\x83\x52", "\x83\x53", 
"\x83\x54", "\x83\x55", "\x83\x56", "\x83\x57", "\x83\x58",
"\x83\x59", "\x83\x5A", "\x83\x5B", "\x83\x5C", "\x83\x5D", 
"\x83\x5E", "\x83\x5F", "\x83\x60", "\x83\x61", "\x83\x62",
"\x83\x63", "\x83\x64", "\x83\x65", "\x83\x66", "\x83\x67", "\x83\x68", 
"\x83\x69", "\x83\x6A", "\x83\x6B", "\x83\x6C", "\x83\x6D", 
"\x83\x6E", "\x83\x6F", "\x83\x70", "\x83\x71", "\x83\x72",
"\x83\x73", "\x83\x74", "\x83\x75", "\x83\x76", "\x83\x77",
"\x83\x78", "\x83\x79", "\x83\x7A", "\x83\x7B", "\x83\x7C", 
"\x83\x7D", "\x83\x7E", "\x83\x80", "\x83\x81", "\x83\x82", 
"\x83\x83", "\x83\x84", "\x83\x85", "\x83\x86", "\x83\x87", "\x83\x88", 
"\x83\x89", "\x83\x8A", "\x83\x8B", "\x83\x8C", "\x83\x8D", 
"\x83\x8E", "\x83\x8F", "\x83\x90", "\x83\x91", "\x83\x92", "\x83\x93" 
}; static const int ND5kuLMs[KGRTuzlW][10] = {
11, 4, 4, 4, 4, 4, 5, 4, 5, 1, 11, 4, 4, 4, 4, 4, 5, 4, 5, 1,
10, 10, 10, 11, 5, 15, 5, 6, 5, 6, 11, 12, 10, 11, 5, 15, 5, 6, 5, 6
}; static const int Fd0iW106[KGRTuzlW][10] = {
0, 11, 15, 19, 23, 27, 31, 36, 40, 45, 46, 57, 61, 65, 69, 73, 77, 82, 86, 91,
92,102,112,122,133,138,153,158,164,169, 175,186,198,208,219,224,239,244,250,255
};

#define d9AshwlE GLIB_KEY_DOWN

#define A6pcPPFu 0xFFFF 

#define Gte7bGwX GLIB_KEY_SHARP

#define jynRrmeb GLIB_KEY_STAR

#endif 

#define Iiir6kHM(mode, key, index) (char*)(F6VdJTwN[Fd0iW106[mode][key] + index])
static BYTE zl43W2TJ[G_NV_SCORE]; 
static g_Record* Tf8yQ77b; 
static int vVP7jCTx; 
static int Hx3dIuDA; 
static int abSzvvHf; 
static int ZiXpvN3k; 
static int zroUNpSW; 
static int zwMe7uTc; 
static char *R0C2PjBV; 
static int Fju2VHHp; 
static int ufCNKLtK; 
static int wPfkxgQc; 
static int LA9zIClI;
void g_LoadScore()
{ int H0Ng1GoT; gext_GetNV(s_GameID, zl43W2TJ);
Hx3dIuDA = (SWAP_LCD_HEIGHT - x8GCfooj)*4/5;
abSzvvHf = (SWAP_LCD_WIDTH - drSwT6Gs)/2;
Tf8yQ77b = (g_Record*)&zl43W2TJ[1];
if (zl43W2TJ[0] != G_NV_MAGIC) {
zl43W2TJ[0] = G_NV_MAGIC; for (H0Ng1GoT = 0; H0Ng1GoT < G_SCOREBOARD_SIZE; H0Ng1GoT++) {
Tf8yQ77b[H0Ng1GoT].month = 0; Tf8yQ77b[H0Ng1GoT].day = 0;
Tf8yQ77b[H0Ng1GoT].score[0] = 0; Tf8yQ77b[H0Ng1GoT].score[1] = 0;
strcpy(Tf8yQ77b[H0Ng1GoT].name, z48637g9);
} } } void g_SaveScore() { gext_PutNV(s_GameID, zl43W2TJ);
}
int g_CheckScore(WORD score)
{ int H0Ng1GoT, hRldyQfl; WORD logScore; int year, month, day, hour, IExrkaR4, sec;
g_LoadScore();
for (H0Ng1GoT = 0; H0Ng1GoT < G_SCOREBOARD_SIZE; H0Ng1GoT++) {
logScore = Tf8yQ77b[H0Ng1GoT].score[0] << 8 | Tf8yQ77b[H0Ng1GoT].score[1];
if (logScore < score) { 
for (hRldyQfl = G_SCOREBOARD_SIZE - 1; hRldyQfl > H0Ng1GoT; hRldyQfl--)
memcpy((char*)&Tf8yQ77b[hRldyQfl], (char*)&Tf8yQ77b[hRldyQfl - 1], sizeof(g_Record));
gext_GetDate(&year, &month, &day, &hour, &IExrkaR4, &sec);
vVP7jCTx = H0Ng1GoT; Tf8yQ77b[H0Ng1GoT].month = month;
Tf8yQ77b[H0Ng1GoT].day = day; Tf8yQ77b[H0Ng1GoT].score[0] = score >> 8;
Tf8yQ77b[H0Ng1GoT].score[1] = score & 0xFF;
memset((void*)Tf8yQ77b[H0Ng1GoT].name, 0, 11);

#ifdef SKTT7
zroUNpSW = abSzvvHf + 28; zwMe7uTc = Hx3dIuDA + vVP7jCTx*12 + 6;

#else 
zroUNpSW = abSzvvHf + 46; zwMe7uTc = Hx3dIuDA + vVP7jCTx*17 + 2;

#endif
R0C2PjBV = Tf8yQ77b[H0Ng1GoT].name; return GRET_SCORE_EDIT;
} } return GRET_SCORE_VIEW; }
static int RmhvEPkn(char *str, int idx)
{ int AJqLAOin = 0; if (idx == 0) return 0;
if (idx == 1) return 1; while (str[AJqLAOin] != '\0') {
if (str[AJqLAOin] & 0x80) AJqLAOin += 2; else
AJqLAOin += 1; if (AJqLAOin == idx - 1) return 1;
else if (AJqLAOin == idx) return 2; } return 0;
}
static void cdSPy35v(char *str, int idx, char *kstr)
{ int len = strlen(str); if (idx >= len) 
strcat(str, kstr); else { 
if (strlen(kstr) == 1) { 
if (str[idx] & 0x80) { 
str[idx] = kstr[0]; str[idx + 1] = ' '; }
else { 
str[idx] = kstr[0]; } } else { 
if (str[idx] & 0x80) { 
str[idx] = kstr[0]; str[idx + 1] = kstr[1];
} else if (str[idx + 1] & 0x80) { 
str[idx] = kstr[0]; str[idx + 1] = kstr[1];
str[idx + 2] = ' '; } else if (str[idx + 1] == '\0') { 
str[idx] = kstr[0]; str[idx + 1] = kstr[1];
str[idx + 2] = '\0'; } else { 
str[idx] = kstr[0]; str[idx + 1] = kstr[1];
} } } }

#if defined (SKTT7) 
void g_DrawScoreBoard(int mode) { char strTemp[80];
char curChar[3]; int cursor; int score; int LbPt5vw8, y, H0Ng1GoT;
for (H0Ng1GoT = 0; H0Ng1GoT < G_SCOREBOARD_SIZE; H0Ng1GoT++) {
y = Hx3dIuDA + H0Ng1GoT*12+4; sprintf(strTemp, "%d", H0Ng1GoT + 1);
g_SetColor(S_BLACK); mil_FillRect(abSzvvHf, y + 1, abSzvvHf + 6, y + 9);
mil_DrawString(abSzvvHf + 1, y + 2, strTemp, S_FONT_MEDIUM, S_WHITE, S_LEFT);
score = (Tf8yQ77b[H0Ng1GoT].score[0] << 8) | Tf8yQ77b[H0Ng1GoT].score[1];
if (score != 0) { if (Tf8yQ77b[H0Ng1GoT].month != 0) {
sprintf(strTemp, "%2d/%2d", Tf8yQ77b[H0Ng1GoT].month, Tf8yQ77b[H0Ng1GoT].day);
mil_DrawStringSolid(abSzvvHf + 8, y, strTemp, S_FONT_SMALL, S_BLACK, S_WHITE, S_LEFT);
} else mil_DrawString(abSzvvHf + 8, y, "--/--", S_FONT_SMALL, S_BLACK, S_LEFT);
sprintf(strTemp, "%05d", score); mil_DrawStringSolid(abSzvvHf + 8, y + 6, strTemp, S_FONT_SMALL, S_BLACK, S_WHITE, S_LEFT);
mil_DrawStringSolid(abSzvvHf + 28, y + 2, Tf8yQ77b[H0Ng1GoT].name, S_FONT_MEDIUM, S_BLACK, S_WHITE, S_LEFT);
} else { mil_DrawString(abSzvvHf + 8, y, "--/--", S_FONT_SMALL, S_BLACK, S_LEFT);
mil_DrawString(abSzvvHf + 8, y + 6, "-----", S_FONT_SMALL, S_BLACK, S_LEFT);
mil_DrawString(abSzvvHf + 28, y + 2, z48637g9, S_FONT_MEDIUM, S_BLACK, S_LEFT);
} } if (mode == 1) {
cursor = ZiXpvN3k; LbPt5vw8 = zroUNpSW + cursor*6;
if (R0C2PjBV[cursor] & 0x80) { curChar[0] = R0C2PjBV[cursor];
curChar[1] = R0C2PjBV[cursor + 1]; curChar[2] = '\0';
} else if (R0C2PjBV[cursor] != '\0') { curChar[0] = R0C2PjBV[cursor];
curChar[1] = '\0'; } else { curChar[0] = ' ';
curChar[1] = '\0'; } if (s_Frame2) mil_DrawStringSolid(LbPt5vw8, zwMe7uTc, curChar, S_FONT_MEDIUM, S_WHITE, S_BLACK, S_LEFT);
else mil_DrawStringSolid(LbPt5vw8, zwMe7uTc, curChar, S_FONT_MEDIUM, S_BLACK, S_WHITE, S_LEFT);

#if (KGRTuzlW > 1)
if (vVP7jCTx < 3) {
mil_DrawStringSolid(SWAP_LCD_WIDTH/2, Hx3dIuDA + 61, (char*)lu0rwjYj[Fju2VHHp],
S_FONT_MEDIUM, S_WHITE, S_BLACK, S_CENTER);
} else { mil_DrawStringSolid(SWAP_LCD_WIDTH/2, Hx3dIuDA + 10, (char*)lu0rwjYj[Fju2VHHp],
S_FONT_MEDIUM, S_WHITE, S_BLACK, S_CENTER);
} 
#endif
}

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
gext_DrawLcd((WORD*)swLcdBuffer); 
#else
gext_DrawLcd((char*)swLcdBuffer); 
#endif
} 
#else 
void g_DrawScoreBoard(int mode) { char strTemp[80];
char curChar[3]; int cursor; int score; int LbPt5vw8, y, H0Ng1GoT;
for (H0Ng1GoT = 0; H0Ng1GoT < G_SCOREBOARD_SIZE; H0Ng1GoT++) {
y = Hx3dIuDA + H0Ng1GoT*17; sprintf(strTemp, "%d", H0Ng1GoT + 1);
g_SetColor(S_BLACK); mil_FillRect(abSzvvHf, y + 1, abSzvvHf + 8, y + 13);
mil_DrawString(abSzvvHf + 2, y + 2, strTemp, S_FONT_LARGE, S_WHITE, S_LEFT);
score = (Tf8yQ77b[H0Ng1GoT].score[0] << 8) | Tf8yQ77b[H0Ng1GoT].score[1];
if (score != 0) { if (Tf8yQ77b[H0Ng1GoT].month != 0) {
sprintf(strTemp, "%2d/%2d", Tf8yQ77b[H0Ng1GoT].month, Tf8yQ77b[H0Ng1GoT].day);
mil_DrawStringSolid(abSzvvHf + 13, y, strTemp, S_FONT_MEDIUM, S_BLACK, S_WHITE, S_LEFT);
} else mil_DrawString(abSzvvHf + 13, y, "--/--", S_FONT_MEDIUM, S_BLACK, S_LEFT);
sprintf(strTemp, "%05d", score); mil_DrawStringSolid(abSzvvHf + 13, y + 8, strTemp, S_FONT_MEDIUM, S_BLACK, S_WHITE, S_LEFT);
mil_DrawStringSolid(abSzvvHf + 46, y + 2, Tf8yQ77b[H0Ng1GoT].name, S_FONT_LARGE, S_BLACK, S_WHITE, S_LEFT);
} else { mil_DrawString(abSzvvHf + 13, y, "--/--", S_FONT_MEDIUM, S_BLACK, S_LEFT);
mil_DrawString(abSzvvHf + 13, y + 8, "-----", S_FONT_MEDIUM, S_BLACK, S_LEFT);
mil_DrawString(abSzvvHf + 46, y + 2, z48637g9, S_FONT_LARGE, S_BLACK, S_LEFT);
} } if (mode == 1) {
cursor = ZiXpvN3k; LbPt5vw8 = zroUNpSW + cursor*6;
if (R0C2PjBV[cursor] & 0x80) { curChar[0] = R0C2PjBV[cursor];
curChar[1] = R0C2PjBV[cursor + 1]; curChar[2] = '\0';
} else if (R0C2PjBV[cursor] != '\0') { curChar[0] = R0C2PjBV[cursor];
curChar[1] = '\0'; } else { curChar[0] = ' ';
curChar[1] = '\0'; } if (s_Frame2) mil_DrawStringSolid(LbPt5vw8, zwMe7uTc, curChar, S_FONT_LARGE, S_WHITE, S_BLACK, S_LEFT);
else mil_DrawStringSolid(LbPt5vw8, zwMe7uTc, curChar, S_FONT_LARGE, S_BLACK, S_WHITE, S_LEFT);

#if (KGRTuzlW > 1)

#if defined (LGIC13) || defined (LGIC14)
if (vVP7jCTx < 3) {
if(Fju2VHHp==0) mil_CopyImage((s_ImgBlock*)Capital_Letter, SWAP_LCD_WIDTH/2, Hx3dIuDA + 61);
else mil_CopyImage((s_ImgBlock*)Small_Letter, SWAP_LCD_WIDTH/2, Hx3dIuDA + 61);
} else { if(Fju2VHHp==0) mil_CopyImage((s_ImgBlock*)Capital_Letter, SWAP_LCD_WIDTH/2, Hx3dIuDA + 10);
else mil_CopyImage((s_ImgBlock*)Small_Letter, SWAP_LCD_WIDTH/2, Hx3dIuDA + 10);
} 
#elif defined LGIC12
mil_DrawStringSolid(SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT-12, (char*)lu0rwjYj[Fju2VHHp], S_FONT_LARGE, S_WHITE, S_BLACK, S_CENTER);

#else
if (vVP7jCTx < 3) {
mil_DrawStringSolid(SWAP_LCD_WIDTH/2, Hx3dIuDA + 61, (char*)lu0rwjYj[Fju2VHHp],
S_FONT_LARGE, S_WHITE, S_BLACK, S_CENTER);
} else { mil_DrawStringSolid(SWAP_LCD_WIDTH/2, Hx3dIuDA + 10, (char*)lu0rwjYj[Fju2VHHp],
S_FONT_LARGE, S_WHITE, S_BLACK, S_CENTER);
} 
#endif

#endif
}

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
gext_DrawLcd((WORD*)swLcdBuffer); 
#else
gext_DrawLcd((char*)swLcdBuffer); 
#endif
} 
#endif

#ifndef LGIC11 
int g_EditScore(int state, int data)
{ int input; 
#ifdef LGIC10
int H0Ng1GoT; 
#endif
switch (state) { case GLIB_START: 
ZiXpvN3k = 0;
Fju2VHHp = 0; ufCNKLtK = -1; wPfkxgQc = 0;
LA9zIClI = 0; g_DrawScoreBoard(1); break;
case GLIB_TIMEOUT: 
if (ufCNKLtK != -1) { 
if (++LA9zIClI == 3) { 
if (R0C2PjBV[ZiXpvN3k] & 0x80) ZiXpvN3k += 2;
else ZiXpvN3k += 1;
if (ZiXpvN3k == 10)
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
ufCNKLtK = -1;
wPfkxgQc = 0; } } g_DrawScoreBoard(1); break;
case GLIB_KEYPRESS: 
LA9zIClI = 0;
switch (data) { case GLIB_KEY_1: case GLIB_KEY_2: case GLIB_KEY_3: case GLIB_KEY_4: case GLIB_KEY_5:
case GLIB_KEY_6: case GLIB_KEY_7: case GLIB_KEY_8: case GLIB_KEY_9: case GLIB_KEY_0:

#if defined(SKTT3) || defined (SKTT3E) || defined (SKTT3M) || defined (SKTT3MA2) || \
defined (SKTT4) || defined (SKTT5) || defined (SKTT6) || defined (SKTT7) 
case GLIB_KEY_STAR: case GLIB_KEY_SHARP: 
#endif
input = data - GLIB_KEY_1; if (ufCNKLtK == -1) { 
if (ZiXpvN3k + strlen(Iiir6kHM(Fju2VHHp, input, 0)) <= 10) {
ufCNKLtK = input;
wPfkxgQc = 0; cdSPy35v(R0C2PjBV, ZiXpvN3k, Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ND5kuLMs[Fju2VHHp][ufCNKLtK] == 1) {
ZiXpvN3k += strlen(Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ZiXpvN3k == 10)
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
ufCNKLtK = -1;
wPfkxgQc = 0; } } }  else { 
if (ufCNKLtK == input) { 
wPfkxgQc = (wPfkxgQc + 1)%ND5kuLMs[Fju2VHHp][ufCNKLtK];
cdSPy35v(R0C2PjBV, ZiXpvN3k, Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
} else { 
ZiXpvN3k += strlen(Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ZiXpvN3k == 10)
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
if (ZiXpvN3k + strlen(Iiir6kHM(Fju2VHHp, input, 0)) <= 10) {
ufCNKLtK = input;
wPfkxgQc = 0; cdSPy35v(R0C2PjBV, ZiXpvN3k, Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ND5kuLMs[Fju2VHHp][ufCNKLtK] == 1) {
ZiXpvN3k += strlen(Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ZiXpvN3k == 10)
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
ufCNKLtK = -1;
wPfkxgQc = 0; } } else { 
ufCNKLtK = -1;
wPfkxgQc = 0; } } } break; case A6pcPPFu:

#ifdef SANYO
if (ZiXpvN3k == 10)
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);

#endif 
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
ufCNKLtK = -1;
wPfkxgQc = 0; break; case Gte7bGwX: if (ZiXpvN3k + 1 < 10) {
if (R0C2PjBV[ZiXpvN3k] != '\0') {
if (R0C2PjBV[ZiXpvN3k] & 0x80) ZiXpvN3k += 2;
else ZiXpvN3k += 1; } else { cdSPy35v(R0C2PjBV, ZiXpvN3k, " ");
ZiXpvN3k += 1;
} ufCNKLtK = -1;
wPfkxgQc = 0; } break; case jynRrmeb: if (R0C2PjBV[ZiXpvN3k] & 0x80) {
memmove(&R0C2PjBV[ZiXpvN3k], &R0C2PjBV[ZiXpvN3k + 2], strlen(&R0C2PjBV[ZiXpvN3k + 2]) + 1);
} else if (R0C2PjBV[ZiXpvN3k] != '\0') {
memmove(&R0C2PjBV[ZiXpvN3k], &R0C2PjBV[ZiXpvN3k + 1], strlen(&R0C2PjBV[ZiXpvN3k + 1]) + 1);
} else {
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
R0C2PjBV[ZiXpvN3k] = '\0'; } ufCNKLtK = -1;
wPfkxgQc = 0; break; case d9AshwlE: 
if (ufCNKLtK != -1) 
ZiXpvN3k += strlen(Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
Fju2VHHp = (Fju2VHHp + 1)%KGRTuzlW;
ufCNKLtK = -1; wPfkxgQc = 0; break; 
#ifdef LGIC10
case uzBLxvrM: for(H0Ng1GoT=ZiXpvN3k;H0Ng1GoT>=0;H0Ng1GoT--)
{ if (R0C2PjBV[ZiXpvN3k] & 0x80) {
memmove(&R0C2PjBV[ZiXpvN3k], &R0C2PjBV[ZiXpvN3k + 2], strlen(&R0C2PjBV[ZiXpvN3k + 2]) + 1);
} else if (R0C2PjBV[ZiXpvN3k] != '\0') {
memmove(&R0C2PjBV[ZiXpvN3k], &R0C2PjBV[ZiXpvN3k + 1], strlen(&R0C2PjBV[ZiXpvN3k + 1]) + 1);
} else {
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
R0C2PjBV[ZiXpvN3k] = '\0'; } } ZiXpvN3k = 0;
Fju2VHHp = 0; ufCNKLtK = -1; wPfkxgQc = 0;
LA9zIClI = 0;
break;

#endif
}
g_DrawScoreBoard(1); break; case GLIB_END: 
return 1; } return 0; } 
#else 
int g_EditScore(int state, int data)
{ int input; switch (state) { case GLIB_START: 
ZiXpvN3k = 0;
Fju2VHHp = 0; ufCNKLtK = -1; wPfkxgQc = 0;
LA9zIClI = 0; g_DrawScoreBoard(1); break;
case GLIB_TIMEOUT: 
if (ufCNKLtK != -1) { 
if (++LA9zIClI == 8) { 
if (R0C2PjBV[ZiXpvN3k] & 0x80)
ZiXpvN3k += 2; else ZiXpvN3k += 1; ufCNKLtK = -1;
wPfkxgQc = 0; } } g_DrawScoreBoard(1); break;
case GLIB_KEYPRESS: 
LA9zIClI = 0;
switch (data) { case GLIB_KEY_UP: case GLIB_KEY_DOWN:
input = 0; if (ufCNKLtK == -1) { 
if (ZiXpvN3k + strlen(Iiir6kHM(Fju2VHHp, input, 0)) <= 10) {
ufCNKLtK = input;
wPfkxgQc = 0; cdSPy35v(R0C2PjBV, ZiXpvN3k, Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ND5kuLMs[Fju2VHHp][ufCNKLtK] == 1) {
ZiXpvN3k += strlen(Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
ufCNKLtK = -1;
wPfkxgQc = 0; } } }  else { 
if (ufCNKLtK == input) { 
if(data==GLIB_KEY_UP) wPfkxgQc = (wPfkxgQc + 1)%ND5kuLMs[Fju2VHHp][ufCNKLtK];
else wPfkxgQc = (wPfkxgQc +44 )%ND5kuLMs[Fju2VHHp][ufCNKLtK];
cdSPy35v(R0C2PjBV, ZiXpvN3k, Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
} else { 
ZiXpvN3k += strlen(Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ZiXpvN3k + strlen(Iiir6kHM(Fju2VHHp, input, 0)) <= 10) {
ufCNKLtK = input;
wPfkxgQc = 0; cdSPy35v(R0C2PjBV, ZiXpvN3k, Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
if (ND5kuLMs[Fju2VHHp][ufCNKLtK] == 1) {
ZiXpvN3k += strlen(Iiir6kHM(Fju2VHHp, ufCNKLtK, wPfkxgQc));
ufCNKLtK = -1;
wPfkxgQc = 0; } } else { 
ufCNKLtK = -1;
wPfkxgQc = 0; } } } break; case A6pcPPFu:
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
ufCNKLtK = -1;
wPfkxgQc = 0; break; case Gte7bGwX: if (ZiXpvN3k + 1 <= 10) {
if (R0C2PjBV[ZiXpvN3k] != '\0') {
if (R0C2PjBV[ZiXpvN3k] & 0x80) ZiXpvN3k += 2;
else ZiXpvN3k += 1; } else { cdSPy35v(R0C2PjBV, ZiXpvN3k, " ");
ZiXpvN3k += 1;
} ufCNKLtK = -1;
wPfkxgQc = 0; } break; case jynRrmeb: if (R0C2PjBV[ZiXpvN3k] & 0x80) {
memmove(&R0C2PjBV[ZiXpvN3k], &R0C2PjBV[ZiXpvN3k + 2], strlen(&R0C2PjBV[ZiXpvN3k + 2]) + 1);
} else if (R0C2PjBV[ZiXpvN3k] != '\0') {
memmove(&R0C2PjBV[ZiXpvN3k], &R0C2PjBV[ZiXpvN3k + 1], strlen(&R0C2PjBV[ZiXpvN3k + 1]) + 1);
} else {
ZiXpvN3k -= RmhvEPkn(R0C2PjBV, ZiXpvN3k);
R0C2PjBV[ZiXpvN3k] = '\0'; } ufCNKLtK = -1;
wPfkxgQc = 0; break;
}
g_DrawScoreBoard(1); break; case GLIB_END: 
return 1; } return 0; } 
#endif