package com.sktt.lcdui;

public class CodeConvert {
	
	public final static char KSC2UNI_DEFAULT_CHAR	= 0x0020;
	public final static char HCTBL_NOT_FOUND = 0xFFFF;
	public final static int NUMOF_KSC_A1 = (96);
	public final static int NUMOF_KSC_A2 = (80);
	public final static int NUMOF_KSC_A6 = (80);
	public final static int NUMOF_KSC_A7 = (80);
	public final static int  NUMOF_KSCODE_HANGUL = 2350;

	public static final char
        SBase = 0xAC00,
		LBase = 0x1100,		// 초성
		VBase = 0x1161,		// 중성
		TBase = 0x11A7;		// 종성
	public static final int
        LCount = 19, VCount = 21, TCount = 28,
        NCount = VCount * TCount,   // 588
        SCount = LCount * NCount;   // 11172

	// For
	// "ㅇㄱㅣㅏㄷㄴㅓㅁㅂㅗㅈㅜ",
	// "ㅎㅋㅡㅑㅌㄹㅕㅅㅍㅛㅊㅠ",
	// "   ㄲㅢ   ㄸ      ㅆㅃ   ㅉ  ",
	
	public final static char [][] KoreanTable = {
		{LBase+11, LBase+0, VBase+20, VBase+0, LBase+3 , LBase+2, VBase+4, LBase+6, LBase+7, VBase+8, LBase+12, VBase+13},
		{LBase+18, LBase+15,VBase+18, VBase+2, LBase+16, LBase+5, VBase+6, LBase+9, LBase+17,VBase+12,LBase+14, VBase+17},
		{0x2020,   LBase+1, VBase+19,   0x2020,  LBase+4,  0x2020,  0x2020,  LBase+10, LBase+8, 0x2020, LBase+13,   0x2020}
	};
	
	public final static String[] EnglishUTable = {   
		"@.ADGJMPTW*#",
		"0QBEHKNRUX  ",
		" ZCFILOSVY  ",
		" 123456789  "
	};
	
	public final static String[] EnglishLTable = {
		"@.adgjmptw*#",
		"0qbehknrux  ",
		" zcfilosvy  ",
		" 123456789  "
	};
	
	// 나중에 숫자모드 추가할 때 넣어주기
	public final static String[] NumberTable = {
		"0123456789  "
	};
	
	// 한글 복모음 코드  
	public final static char[][] BokMoumTable = {   
		{VBase + 0, VBase + 20, VBase + 1},			/* ㅏ, ㅣ, ㅐ */
		{VBase + 2, VBase + 20, VBase + 3},			/* ㅑ, ㅣ, ㅒ */
		{VBase + 4, VBase + 20, VBase + 5},			/* ㅓ, ㅣ, ㅔ */
		{VBase + 6, VBase + 20, VBase + 7},			/* ㅕ, ㅣ, ㅖ */
		{VBase + 8, VBase + 0,  VBase + 9 },		/* ㅗ, ㅏ, ㅘ */
		{VBase + 8, VBase + 20, VBase + 11},		/* ㅗ, ㅣ, ㅚ */
		{VBase + 13, VBase + 4, VBase + 14},		/* ㅜ, ㅓ, ㅝ */  //ㅜ+ㅓ에서 ㅓ는 ㅏ를 변경해줘야 하므로 대신 ㅏ로 처리
		{VBase + 13, VBase + 20, VBase + 16},		/* ㅜ, ㅣ, ㅟ */
		{VBase + 9, VBase + 20,  VBase + 10},		/* ㅘ, ㅣ  ㅙ */
		{VBase + 14, VBase + 20, VBase + 15},		/* ㅝ, ㅣ, ㅞ */		
		{VBase + 18, VBase + 20, VBase + 19}		/* ㅡ, ㅣ, ㅢ */		
	};
	
	// 한글 복자음 코드 
	public final static char[][] BokJaumTable = {
		{LBase + 0, LBase + 9, TBase + 3},			/* ㄱ, ㅅ, ㄳ */
		{LBase + 2, LBase + 12, TBase + 5},			/* ㄴ, ㅈ, ㄵ */
		{LBase + 2, LBase + 18, TBase + 6},			/* ㄴ, ㅎ, ㄶ */		
		{LBase + 5, LBase + 0, TBase + 9},			/* ㄹ, ㄱ, ㄺ */
		{LBase + 5, LBase + 6, TBase + 10},			/* ㄹ, ㅁ, ㄻ */
		{LBase + 5, LBase + 7, TBase + 11},			/* ㄹ, ㅂ, ㄼ */
		{LBase + 5, LBase + 9, TBase + 12},			/* ㄹ, ㅅ, ㄽ */
		{LBase + 5, LBase + 16, TBase + 13},		/* ㄹ, ㅌ, ㄾ */
		{LBase + 5, LBase + 17, TBase + 14},		/* ㄹ, ㅍ, ㄿ */
		{LBase + 5, LBase + 18, TBase + 15},		/* ㄹ, ㅎ, ㅀ */
		{LBase + 7, LBase + 9, TBase + 18},			/* ㅂ, ㅅ, ㅄ */

	};
	
	public static final char Cho2Jong[] = {
		TBase + 1, TBase + 2, TBase + 4, TBase + 7, 0, TBase + 8,
		TBase + 16, TBase + 17, 0, TBase + 19, TBase + 20, TBase + 21, TBase + 22, 0, 
		TBase + 23, TBase + 24, TBase + 25, TBase + 26, TBase + 27
	};

	// constructor
	CodeConvert(){}
	
	// Check 자음   
	public boolean ISJAUM(char ch){
		if (ch >= VBase && ch <= 0x11A2) {
			return false;           // 모음 
		} else {
			return true;            // 자음 
		}
	}
	
	// 두가지 모음의 조합이 복모음인지를 검사함.
	public char GetBokMoum(char code1, char code2){
		for (int i = 0; i < 11; i++)
			if (BokMoumTable[i][0] == code1 && BokMoumTable[i][1] == code2)
				return BokMoumTable[i][2];
		return 0;
	}
	
	// 두가지 자음의 조합이 복자음인지를 검사함.  
	public char GetBokJaum(char code1, char code2){
		for (int i = 0; i < 11; i++)
			if (BokJaumTable[i][0] == code1 && BokJaumTable[i][1] == code2)
				return BokJaumTable[i][2];
		return 0;
	}
	
}