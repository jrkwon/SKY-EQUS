//************************************************************************
//
// * File Name: HCodeCnvt.cpp
//
// Hangul Code Conversion: Another Solution
// 
// Date. 199. 7. 12.
// Contact Info.
//         Choi, Woonho. at Parole Science, Inc.
//         e-mail: whchoi@clepsi.co.kr
//         http://www.clepsi.co.kr
//         TEL : +82-2-887-8064
//         FAX : +82-2-888-6954
//
//


#include "HCodeCnvt.h"
#include "HCodeTable.h"
#include "HCodeDef.h"


#define	KSC2UNI_DEFAULT_CHAR	(0x0020)

//------------------------------------------------
//
//  Hangul Union Structure for KSSM CHO-JUNG-JONG
//
// * Hangul CHO-JUNG-JONG Bitfield Structure
//------------------------------------------------

typedef union _hSyl_
{
	unsigned short hCode;
	struct
	{
		unsigned int hJong:	5;
		unsigned int hJung:	5;
		unsigned int hCho :	5;
		unsigned int hFlag:	1;
	} hPhoneme;
} structHSyl;

/////////////////////////////////////////////////
// Hangul Combination Macro using Bit-Field
/////////////////////////////////////////////////

#define HAN_COMBI(hStruct, i, m, f) ( (hStruct).hPhoneme.hFlag = 1, (hStruct).hPhoneme.hCho = (i), (hStruct).hPhoneme.hJung = (m), (hStruct).hPhoneme.hJong = (f), (hStruct).hCode)

/////////////////////////////////////////////////


unsigned short
searchKSTable(unsigned short uiTGCode)
{
	unsigned short iStart = 0;
	unsigned short iEnd = NUMOF_KSCODE_HANGUL;
	unsigned short iMid;


	while (1)
	{
		iMid = ((iEnd - iStart) >> 1) + iStart;
		if (hctbl_KSCode[iMid] == uiTGCode)
			return iMid;

		if (iEnd <= iStart)		// Hangul Syllable not found in 'hctbl_KSCode' Table
			return HCTBL_NOT_FOUND;
		else if (hctbl_KSCode[iMid] > uiTGCode)
			iEnd = iMid - 1;
		else
			iStart = iMid + 1;
	}

	return HCTBL_NOT_FOUND;
}


//===========================================================
// Combination Code to KS C-5601 1987
//
// Return Value
// - KS C-5601 Code Value (if found in table)
// - HCTBL_NOT_FOUND (if not in KS C-5601 1987, 2350 Chars)
//==========================================================

unsigned short
ConvertTG2KSSyl(unsigned short uiTG)
{

	unsigned short uiIdx;

	uiIdx = searchKSTable(uiTG);

	if (uiIdx != HCTBL_NOT_FOUND)
	{		
		return ( (((uiIdx/94) + 0xB0) << 8) + (uiIdx % 94) + 0xA1);
	}

	// Syllable not found in the range of 2,350 Char(s). KS C-5601 1987
	
	return HCTBL_NOT_FOUND;
}

unsigned short
ConvertKS2TGSyl(unsigned short uiKS)
{
	short c1;
	short c2;
	short idx;

	c1 = uiKS >> 8;
	c2 = uiKS & 0x00ff;

		// Get Array Index from Table ROW-COLUMN Index
	idx = (c1 - 0x00B0) * 94 + (c2 - 0x00A1);

	if (idx < 0 || idx > NUMOF_KSCODE_HANGUL)
		return HCTBL_NOT_FOUND;

	return (hctbl_KSCode[idx]);
}

//
// CJK Compatible - Converting
bool
hccUni2KSC_CJKCompatible(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;

	if (uniSyl >= 0x3380 && uniSyl <= 0x3384)
		uiSyl = uniSyl - 0x3380 + 0xA7C9;
	else if (uniSyl == 0x3388 || uniSyl == 0x3389)
		uiSyl = uniSyl - 0x3388 + 0xA7BA;
	else if (uniSyl >= 0x338A && uniSyl <= 0x338C)
		uiSyl = uniSyl - 0x338A + 0xA7DC;
	else if (uniSyl >= 0x338D && uniSyl <= 0x338F)
		uiSyl = uniSyl - 0x338D + 0xA7B6;
	else if (uniSyl >= 0x3390 && uniSyl <= 0x3394)
		uiSyl = uniSyl - 0x3390 + 0xA7D4;
	else if (uniSyl >= 0x3395 && uniSyl <= 0x3398)
	{
		uiSyl = uniSyl - 0x3395;
		switch (uniSyl)
		{
		case 0x3395:
		case 0x3396:
		case 0x3397:
			uiSyl += 0xA7A1; break;
		case 0x3398:
			uiSyl = 0xA7A5; break;
		}
	}
	else if (uniSyl >= 0x3399 && uniSyl <= 0x33A2)
		uiSyl = uniSyl - 0x3399 + 0xA7AB;
	else if (uniSyl >= 0x33A3 && uniSyl <= 0x33A6)
		uiSyl = uniSyl - 0x33A3 + 0xA7A7;
	else if (uniSyl == 0x33A7 || uniSyl == 0x33A8)
		uiSyl = uniSyl - 0x33A7 + 0xA7BD;
	else if (uniSyl >= 0x33A9 && uniSyl <= 0x33AC)
		uiSyl = uniSyl - 0x33A9 + 0xA7E5;
	else if (uniSyl >= 0x33AD && uniSyl <= 0x33AF)
		uiSyl = uniSyl - 0x33AD + 0xA7E1;
	else if (uniSyl >= 0x33B0 && uniSyl <= 0x33B9)
		uiSyl = uniSyl - 0x33B0 + 0xA7BF;
	else if (uniSyl >= 0x33BA && uniSyl <= 0x33BF)
		uiSyl = uniSyl - 0x33BA + 0xA7CE;
	else if (uniSyl == 0x33C0 || uniSyl == 0x33C1)
		uiSyl = uniSyl - 0x33C0 + 0xA7DA;
	else if (uniSyl >= 0x33C2 && uniSyl <= 0x33FF)
	{
		switch (uniSyl)
		{
		case 0x33C2: uiSyl = 0xA2E3; break;
		case 0x33C3: uiSyl = 0xA7EC; break;
		case 0x33C4: uiSyl = 0xA7A6; break;
		case 0x33C5: uiSyl = 0xA7E0; break;
		case 0x33C6: uiSyl = 0xA7EF; break;
		case 0x33C7: uiSyl = 0xA2E1; break;
		case 0x33C8: uiSyl = 0xA7BC; break;
		case 0x33C9: uiSyl = 0xA7ED; break;
		case 0x33CA: uiSyl = 0xA7B5; break;
		case 0x33CF: uiSyl = 0xA7B9; break;
		case 0x33D0: uiSyl = 0xA7EA; break;
		case 0x33D3: uiSyl = 0xA7EB; break;
		case 0x33D6: uiSyl = 0xA7DF; break;
		case 0x33D8: uiSyl = 0xA2E4; break;
		case 0x33DB: uiSyl = 0xA7E4; break;
		case 0x33DC: uiSyl = 0xA7EE; break;
		case 0x33DD: uiSyl = 0xA7E9; break;
		default: return false;
		}
	}

	*kscSyl = uiSyl >> 8;
	*(kscSyl+1) = uiSyl & 0x00ff;
	*(kscSyl+2) = '\0';
	return true;
}

bool
hccUni2KSCJapanese(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;

	// Hiragana
	if (uniSyl >= 0x3041 && uniSyl <= 0x3093)
	{
		uiSyl = (uniSyl - 0x3041) + 0xAAA1;
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}
		// Katakana
	if (uniSyl >= 0x30A1 && uniSyl <= 0x30F6)
	{
		uiSyl = (uniSyl - 0x30A1) + 0xABA1;
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool
hccUni2KSC_BoxDrawing(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	short uiIdx;

	uiIdx = uniSyl - 0x2500;

	if (uiIdx < 0 || uiIdx > NUMOF_BOXDRAW - 1)
		return false;

	uiSyl = hctbl_UNI2KSC_BOXDRAW[uiIdx];

	if (uiSyl == 0xffff)
		return false;

	*kscSyl = uiSyl >> 8;
	*(kscSyl+1) = uiSyl & 0x00ff;
	*(kscSyl+2) = '\0';
	return true;
}

bool
hccUni2KSC_MathOperator(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;

	if (uniSyl < 0x2200 || uniSyl > 0x22FF)
		return false;
	
	switch (uniSyl)
	{
				// Unicode [Mathematical Operators]
	case 0x2200:		// Quantifier All
		uiSyl = 0xA2A3; bTranslated = true; break;
	case 0x2202:
		uiSyl = 0xA1D3; bTranslated = true; break;
	case 0x2203:
		uiSyl = 0xA2A4; bTranslated = true; break;
	case 0x2207:
		uiSyl = 0xA1D4; bTranslated = true; break;
	case 0x2208:
		uiSyl = 0xA1F4; bTranslated = true; break;
	case 0x220B:
		uiSyl = 0xA1F5; bTranslated = true; break;
	case 0x220F:
		uiSyl = 0xA2B3; bTranslated = true; break;
	case 0x2211:
		uiSyl = 0xA2B2; bTranslated = true; break;
	case 0x221A:
		uiSyl = 0xA1EE; bTranslated = true; break;
	case 0x221D:
		uiSyl = 0xA1F0; bTranslated = true; break;
	case 0x221E:
		uiSyl = 0xA1C4; bTranslated = true; break;
	case 0x2220:
		uiSyl = 0xA1D0; bTranslated = true; break;
	case 0x2225:
		uiSyl = 0xA1AB; bTranslated = true; break;
	case 0x2227:
		uiSyl = 0xA1FC; bTranslated = true; break;
	case 0x2228:
		uiSyl = 0xA1FD; bTranslated = true; break;
	case 0x2229:
		uiSyl = 0xA1FB; bTranslated = true; break;
	case 0x222A:
		uiSyl = 0xA1FA; bTranslated = true; break;
	case 0x222B:
		uiSyl = 0xA1F2; bTranslated = true; break;
	case 0x222C:
		uiSyl = 0xA1F3; bTranslated = true; break;
	case 0x222E:
		uiSyl = 0xA2B1; bTranslated = true; break;
	case 0x2234:
		uiSyl = 0xA1C5; bTranslated = true; break;
	case 0x2235:
		uiSyl = 0xA1F1; bTranslated = true; break;
	case 0x223C:
		uiSyl = 0xA1AD; bTranslated = true; break;
	case 0x223D:
		uiSyl = 0xA1EF; bTranslated = true; break;
	case 0x2252:
		uiSyl = 0xA1D6; bTranslated = true; break;
	case 0x2260:
		uiSyl = 0xA1C1; bTranslated = true; break;
	case 0x2261:
		uiSyl = 0xA1D5; bTranslated = true; break;
	case 0x2264:
		uiSyl = 0xA1C2; bTranslated = true; break;
	case 0x2265:
		uiSyl = 0xA1C3; bTranslated = true; break;
	case 0x226A:
		uiSyl = 0xA1EC; bTranslated = true; break;
	case 0x226B:
		uiSyl = 0xA1ED; bTranslated = true; break;
	case 0x2282:
		uiSyl = 0xA1F8; bTranslated = true; break;
	case 0x2283:
		uiSyl = 0xA1F9; bTranslated = true; break;
	case 0x2286:
		uiSyl = 0xA1F6; bTranslated = true; break;
	case 0x2287:
		uiSyl = 0xA1F7; bTranslated = true; break;
	case 0x2299:
		uiSyl = 0xA2C1; bTranslated = true; break;
	case 0x22A5:
		uiSyl = 0xA1D1; bTranslated = true; break;
	default:
		bTranslated = false; break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;

}

bool
hccUni2KSC_EnclosedAlphaNumerics(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated= false;

	//Range 0x2460 - 0x24ff

	if (uniSyl < 0x2460 || uniSyl > 0x24ff)
		return false;

	if (uniSyl >= 0x2460 && uniSyl <= 0x246E)
	{
		uiSyl = uniSyl - 0x2460 + 0xA8E7;
		bTranslated = true;
	}
	else if (uniSyl >= 0x2474 && uniSyl <= 0x2482)
	{
		uiSyl = uniSyl - 0x2474 + 0xA9E7;
		bTranslated = true;
	}
	else if (uniSyl >= 0x249C && uniSyl <= 0x24B5)
	{
		uiSyl = uniSyl - 0x249C + 0xA9CD;
		bTranslated = true;
	}
	else if (uniSyl >= 0x24D0 && uniSyl <= 0x24E9)
	{
		uiSyl = uniSyl - 0x24D0 + 0xA8CD;
		bTranslated = true;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool 
hccUni2KSC_GeometricShapes(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;

	if (uniSyl < 0x25a0 || uniSyl > 0x25ff)
		return false;

	switch (uniSyl)
	{
	case 0x25A0: 
		uiSyl = 0xA1E1; bTranslated = true; break;
	case 0x25A1:
		uiSyl = 0xA1E0; bTranslated = true; break;
	case 0x25A3:
		uiSyl = 0xA2C3; bTranslated = true; break;
	case 0x25A4:
		uiSyl = 0xA2C7; bTranslated = true; break;
	case 0x25A5:
		uiSyl = 0xA2C8; bTranslated = true; break;
	case 0x25A6:
		uiSyl = 0xA2CB; bTranslated = true; break;
	case 0x25A7:
		uiSyl = 0xA2CA; bTranslated = true; break;
	case 0x25A8:
		uiSyl = 0xA2C9; bTranslated = true; break;
	case 0x25A9:
		uiSyl = 0xA2CC; bTranslated = true; break;
	case 0x25B2:
		uiSyl = 0xA1E3; bTranslated = true; break;
	case 0x25B3:
		uiSyl = 0xA1E2; bTranslated = true; break;
	case 0x25B6:
		uiSyl = 0xA2BA; bTranslated = true; break;
	case 0x25B7:
		uiSyl = 0xA2B9; bTranslated = true; break;
	case 0x25BC:
		uiSyl = 0xA1E5; bTranslated = true; break;
	case 0x25BD:
		uiSyl = 0xA1E4; bTranslated = true; break;
	case 0x25C0:
		uiSyl = 0xA2B8; bTranslated = true; break;
	case 0x25C1:
		uiSyl = 0xA2B7; bTranslated = true; break;
	case 0x25C6:
		uiSyl = 0xA1DF; bTranslated = true; break;
	case 0x25C7:
		uiSyl = 0xA1DE; bTranslated = true; break;
	case 0x25C8:
		uiSyl = 0xA2C2; bTranslated = true; break;
	case 0x25CB:
		uiSyl = 0xA1DB; bTranslated = true; break;
	case 0x25CE:
		uiSyl = 0xA1DD; bTranslated = true; break;
	case 0x25CF:
		uiSyl = 0xA1DC; bTranslated = true; break;
	case 0x25D0:
		uiSyl = 0xA2C4; bTranslated = true; break;
	case 0x25D1:
		uiSyl = 0xA2C5; bTranslated = true; break;
	default: break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool 
hccUni2KSC_MiscDingbats(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;

	if (uniSyl < 0x2600 || uniSyl > 0x26ff)
		return false;

	switch (uniSyl)
	{
	case 0x2605:
		uiSyl = 0xA1DA; bTranslated = true; break;
	case 0x2606:
		uiSyl = 0xA1D9; bTranslated = true; break;
	case 0x260E:
		uiSyl = 0xA2CF; bTranslated = true; break;
	case 0x260F:
		uiSyl = 0xA2CE; bTranslated = true; break;
	case 0x261C:
		uiSyl = 0xA2D0; bTranslated = true; break;
	case 0x261E:
		uiSyl = 0xA2D1; bTranslated = true; break;
	case 0x2640:
		uiSyl = 0xA1CF; bTranslated = true; break;
	case 0x2642:
		uiSyl = 0xA1CE; bTranslated = true; break;
	case 0x2660:
		uiSyl = 0xA2BC; bTranslated = true; break;
	case 0x2661:
		uiSyl = 0xA2BD; bTranslated = true; break;
	case 0x2663:
		uiSyl = 0xA2C0; bTranslated = true; break;
	case 0x2664:
		uiSyl = 0xA2BB; bTranslated = true; break;
	case 0x2665:
		uiSyl = 0xA2BE; bTranslated = true; break;
	case 0x2667:
		uiSyl = 0xA2BF; bTranslated = true; break;
	case 0x2668:
		uiSyl = 0xA2CD; bTranslated = true; break;
	case 0x2669:
		uiSyl = 0xA2DB; bTranslated = true; break;
	case 0x266A:
		uiSyl = 0xA2DC; bTranslated = true; break;
	case 0x266C:
		uiSyl = 0xA2DD; bTranslated = true; break;
	case 0x266D:
		uiSyl = 0xA2DA; bTranslated = true; break;
	default: break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool 
hccUni2KSC_CJKSymPunc(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;

	if (uniSyl < 0x3000 || uniSyl > 0x303F)
		return false;

	switch (uniSyl)
	{
	case 0x3001:
		uiSyl = 0xA1A2; bTranslated = true; break;
	case 0x3002:
		uiSyl = 0xA1C6; bTranslated = true; break;
	case 0x3003:
		uiSyl = 0xA1C8; bTranslated = true; break;
	case 0x3008:
		uiSyl = 0xA1B4; bTranslated = true; break;
	case 0x3009:
		uiSyl = 0xA1B5; bTranslated = true; break;
	case 0x300A:
		uiSyl = 0xA1B6; bTranslated = true; break;
	case 0x300B:
		uiSyl = 0xA1B7; bTranslated = true; break;
	case 0x300C:
		uiSyl = 0xA1B8; bTranslated = true; break;
	case 0x300D:
		uiSyl = 0xA1B9; bTranslated = true; break;
	case 0x300E:
		uiSyl = 0xA1BA; bTranslated = true; break;
	case 0x300F:
		uiSyl = 0xA1BB; bTranslated = true; break;
	case 0x3010:
		uiSyl = 0xA1BC; bTranslated = true; break;
	case 0x3011:
		uiSyl = 0xA1BD; bTranslated = true; break;
	case 0x3013:
		uiSyl = 0xA1EB; bTranslated = true; break;
	case 0x3014:
		uiSyl = 0xA1B2; bTranslated = true; break;
	case 0x3015:
		uiSyl = 0xA1B3; bTranslated = true; break;
	default: break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool 
hccUni2KSC_EnclosedCJKLetters(wchar_t uniSyl, unsigned char *kscSyl)
{

	unsigned short uiSyl;
	bool bTranslated = false;

	// 0x3200 - 0x32ff

	if (uniSyl < 0x3200 || uniSyl > 0x32FF)
		return false;

	if (uniSyl >= 0x3200 && uniSyl <= 0x321B)
	{
		uiSyl = uniSyl - 0x3200 + 0xA9B1;
		bTranslated = true;
	}
	else if (uniSyl >= 0x3260 && uniSyl <= 0x327B)
	{
		uiSyl = uniSyl - 0x3260 + 0xA8B1;
		bTranslated = true;
	}
	else if (uniSyl == 0x321C)
	{
		uiSyl = 0xA2DF;
		bTranslated = true;
	}
	else if (uniSyl == 0x327F)
	{
		uiSyl = 0xA2DE;
		bTranslated = true;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool 
hccUni2KSC_HalfFullForms(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = true;

	if (uniSyl < 0xff00 || uniSyl > 0x00ef)
		return false;

	if (uniSyl >= 0xFF00 && uniSyl <= 0xFF5D)
	{
		uiSyl = uniSyl - 0xFF00 + 0xA3A0;
		bTranslated = true;
	}
	else
	{
		switch (uniSyl)
		{
		case 0xFF5E:
			uiSyl = 0xA1AD; bTranslated = true; break;
		case 0xFFE0:
			uiSyl = 0xA1CB; bTranslated = true; break;
		case 0xFFE1:
			uiSyl = 0xA1CC; bTranslated = true; break;
		case 0xFFE2:
			uiSyl = 0xA1FE; bTranslated = true; break;
		case 0xFFE3:
			uiSyl = 0xA3FE; bTranslated = true; break;
		case 0xFFE5:
			uiSyl = 0xA1CD; bTranslated = true; break;
		case 0xFFE6:
			uiSyl = 0xA1DC; bTranslated = true; break;
		default: break;
		}
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool 
hccUni2KSC_SpacingModifier(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;
	//0x02b0 - 0x02ff

	if (uniSyl < 0x02B0 || uniSyl > 0x02FF)
		return false;

	switch (uniSyl)
	{
	case 0x02C7:
		uiSyl = 0xA2A7; bTranslated = true; break;
	case 0x02D0:
		uiSyl = 0xA2B0; bTranslated = true; break;
	case 0x02D8:
		uiSyl = 0xA2A8; bTranslated = true; break;
	case 0x02D9:
		uiSyl = 0xA2AB; bTranslated = true; break;
	case 0x02DA:
		uiSyl = 0xA2AA; bTranslated = true; break;
	case 0x02DB:
		uiSyl = 0xA2AD; bTranslated = true; break;
	case 0x02DD:
		uiSyl = 0xA2A9; bTranslated = true; break;
	default: break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;

}

bool 
hccUni2KSC_GeneralPunc(wchar_t uniSyl, unsigned char *kscSyl)
{

	unsigned short uiSyl;
	bool bTranslated = false;
	// 0x2000 - 0x206f

	if (uniSyl < 0x2000 || uniSyl > 0x206f)
		return false;

	switch (uniSyl)
	{
	case 0x2015:
		uiSyl = 0xA1AA; bTranslated = true; break;
	case 0x2018:
		uiSyl = 0xA1AE; bTranslated = true; break;
	case 0x2019:
		uiSyl = 0xA1AF; bTranslated = true; break;
	case 0x201C:
		uiSyl = 0xA1A8; bTranslated = true; break;
	case 0x201D:
		uiSyl = 0xA3A2; bTranslated = true; break;
	case 0x2020:
		uiSyl = 0xA2D3; bTranslated = true; break;
	case 0x2021:
		uiSyl = 0xA2D4; bTranslated = true; break;
	case 0x2025:
		uiSyl = 0xA1A5; bTranslated = true; break;
	case 0x2026:
		uiSyl = 0xA1A6; bTranslated = true; break;
	case 0x2030:
		uiSyl = 0xA2B6; bTranslated = true; break;
	case 0x2032:
		uiSyl = 0xA1C7; bTranslated = true; break;
	case 0x2033:
		uiSyl = 0xA1C8; bTranslated = true; break;
	case 0x203B:
		uiSyl = 0xA1D8; bTranslated = true; break;
	default: break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool
hccUni2KSC_Latin(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;

		// If not Lantin-1, Latin Extended-A, Latin Extended-B
		// return false
	//if (uniSyl < 0x0020 || uniSyl > 0x023f)
	if (uniSyl < 0x0001 || uniSyl > 0x023f)
		return false;

	//Latin-1
	//if (uniSyl >= 0x0020 && uniSyl <= 0x007E)
	if (uniSyl >= 0x0001 && uniSyl <= 0x007E)
	{
		*kscSyl = uniSyl & 0x00FF;
		*(kscSyl+1) = '\0';
		return true;
	}

	// [Latin-1]
	switch (uniSyl)
	{
	case 0x00A1:
		uiSyl = 0xA2AE; bTranslated = true; break;
	case 0x00A2:
		uiSyl = 0xA1CB; bTranslated = true; break;
	case 0x00A3:
		uiSyl = 0xA1CC; bTranslated = true; break;
	case 0x00A4:
		uiSyl = 0xA2B4; bTranslated = true; break;
	case 0x00A5:
		uiSyl = 0xA1CD; bTranslated = true; break;
	case 0x00A6:
		uiSyl = 0xA3FC; bTranslated = true; break;
	case 0x00A7:
		uiSyl = 0xA1D7; bTranslated = true; break;
	case 0x00A8:
		uiSyl = 0xA1A7; bTranslated = true; break;
	case 0x00A9:
		uiSyl = 0xA8CF; bTranslated = true; break;
	case 0x00AB:
		uiSyl = 0xA1EC; bTranslated = true; break;
	case 0x00AC:
		uiSyl = 0xA1FE; bTranslated = true; break;
	case 0x00AD:
		uiSyl = 0xA1A9; bTranslated = true; break;
	case 0x00AE:
		uiSyl = 0xA8DE; bTranslated = true; break;
	case 0xA1B0:
		uiSyl = 0xA2AA; bTranslated = true; break;
	case 0x00B1:
		uiSyl = 0xA1BE; bTranslated = true; break;
	case 0x00B2:
		uiSyl = 0xA9F7; bTranslated = true; break;
	case 0x00B3:
		uiSyl = 0xA9F8; bTranslated = true; break;
	case 0x00B4:
		uiSyl = 0xA2A5; bTranslated = true; break;
	case 0x00B5:
		uiSyl = 0xA5EC; bTranslated = true; break;
	case 0x00B6:
		uiSyl = 0xA2D2; bTranslated = true; break;
	case 0x00B7:
		uiSyl = 0xA2AB; bTranslated = true; break;
	case 0x00B8:
		uiSyl = 0xA3AC; bTranslated = true; break;
	case 0x00B9:
		uiSyl = 0xA9F6; bTranslated = true; break;
	case 0x00BA:
		uiSyl = 0xA8AC; bTranslated = true; break;
	case 0x00BB:
		uiSyl = 0xA1ED; bTranslated = true; break;
	case 0x00BC:
		uiSyl = 0xA8F9; bTranslated = true; break;
	case 0x00BD:
		uiSyl = 0xA8F6; bTranslated = true; break;
	case 0x00BE:
		uiSyl = 0xA8FA; bTranslated = true; break;
	case 0x00BF:
		uiSyl = 0xA2AF; bTranslated = true; break;
	case 0x00C5:
		uiSyl = 0xA1CA; bTranslated = true; break;
	case 0x00C6:
		uiSyl = 0xA8A1; bTranslated = true; break;
	case 0x00D0:
		uiSyl = 0xA8A2; bTranslated = true; break;
	case 0x00D7:
		uiSyl = 0xA1BF; bTranslated = true; break;
	case 0x00D8:
		uiSyl = 0xA8AA; bTranslated = true; break;
	case 0x00DE:
		uiSyl = 0xA8AD; bTranslated = true; break;
	case 0x00DF:
		uiSyl = 0xA9AC; bTranslated = true; break;
	case 0x00E6:
		uiSyl = 0xA9A1; bTranslated = true; break;
	case 0x00F0:
		uiSyl = 0xA9A3; bTranslated = true; break;
	case 0x00F7:
		uiSyl = 0xA1C0; bTranslated = true; break;
	case 0x00F8:
		uiSyl = 0xA9AA; bTranslated = true; break;
	case 0x00FE:
		uiSyl = 0xA9AD; bTranslated = true; break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}






	// [Latin Extended-A]
	if (uniSyl >= 0x0100 && uniSyl <= 0x017F)
	{
		switch (uniSyl)
		{
		case 0x0111:
			uiSyl = 0xA9A2; bTranslated = true; break;
		case 0x0126:
			uiSyl = 0xA8A4; bTranslated = true; break;
		case 0x0127:
			uiSyl = 0xA9A4; bTranslated = true; break;
		case 0x0131:
			uiSyl = 0xA9A5; bTranslated = true; break;
		case 0x0132:
			uiSyl = 0xA8A6; bTranslated = true; break;
		case 0x0133:
			uiSyl = 0xA9A6; bTranslated = true; break;
		case 0x0138:
			uiSyl = 0xA9A7; bTranslated = true; break;
		case 0x013F:
			uiSyl = 0xA8A8; bTranslated = true; break;
		case 0x0140:
			uiSyl = 0xA9A8; bTranslated = true; break;
		case 0x0141:
			uiSyl = 0xA8A9; bTranslated = true; break;
		case 0x0142:
			uiSyl = 0xA9A9; bTranslated = true; break;
		case 0x0149:
			uiSyl = 0xA9B0; bTranslated = true; break;
		case 0x014A:
			uiSyl = 0xA8AF; bTranslated = true; break;
		case 0x014B:
			uiSyl = 0xA9AF; bTranslated = true; break;
		case 0x0166:
			uiSyl = 0xA8AE; bTranslated = true; break;
		case 0x0167:
			uiSyl = 0xA9AE; bTranslated = true; break;
		default: break;
		}

		if (bTranslated == true)
		{
			*kscSyl = uiSyl >> 8;
			*(kscSyl+1) = uiSyl & 0x00ff;
			*(kscSyl+2) = '\0';
			return true;
		}		
	}

	return false;
}

bool
hccUni2KSC_DefaultChars(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;

	switch (uniSyl)
	{
		// Unicode [Letterlike Symbols]
	case 0x2103:	// Clecius
		uiSyl = 0xA1C9; bTranslated = true; break;
	case 0x2109:	// Farenheit
		uiSyl = 0xA2B5; bTranslated = true; break;
	case 0x2113:	// Litter
		uiSyl = 0xA7A4; bTranslated = true; break;
	case 0x2116:	// No.
		uiSyl = 0xA2E0; bTranslated = true; break;
	case 0x2121:	// Tel
		uiSyl = 0xA2E5; bTranslated = true; break;
	case 0x2126:	// Omega
		uiSyl = 0xA7D9; bTranslated = true; break;
	case 0x212B:	// A0
		uiSyl = 0xA1CA; bTranslated = true; break;

		//Subscripts & Superscripts
	case 0x2074:
		uiSyl = 0xA9F9; bTranslated = true; break;
	case 0x207F:
		uiSyl = 0xA9FA; bTranslated = true; break;
	case 0x2081:
		uiSyl = 0xA9FB; bTranslated = true; break;
	case 0x2082:
		uiSyl = 0xA9FC; bTranslated = true; break;
	case 0x2083:
		uiSyl = 0xA9FD; bTranslated = true; break;
	case 0x2084:
		uiSyl = 0xA9FE; bTranslated = true; break;


		// Unicode [Currency Symbols]
	case 0x20A9:
		uiSyl = 0xA3DC; bTranslated = true; break;
		
		// Unicode [Arrows]
	case 0x2190:	// 
		uiSyl = 0xA1E7; bTranslated = true; break;
	case 0x2191:
		uiSyl = 0xA1E8; bTranslated = true; break;
	case 0x2192:
		uiSyl = 0xA1E6; bTranslated = true; break;		
	case 0x2193:
		uiSyl = 0xA1E9; bTranslated = true; break;
	case 0x2194:
		uiSyl = 0xA1EA; bTranslated = true; break;
	case 0x2195:
		uiSyl = 0xA2D5; bTranslated = true; break;
	case 0x2196:
		uiSyl = 0xA2D8; bTranslated = true; break;
	case 0x2197:
		uiSyl = 0xA2D6; bTranslated = true; break;
	case 0x2198:
		uiSyl = 0xA2D9; bTranslated = true; break;
	case 0x2199:
		uiSyl = 0xA2D7; bTranslated = true; break;
	case 0x21D2:
		uiSyl = 0xA2A1; bTranslated = true; break;
	case 0x21D4:
		uiSyl = 0xA2A2; bTranslated = true; break;		

		// [Micellaneous Technical]
	case 0x2312:
		uiSyl = 0xA1D2; bTranslated = true; break;
		// [Block Elements]
	case 0x2592:
		uiSyl = 0xA2C6; bTranslated = true; break;

	default: break;
	}

	if (bTranslated == true)
	{
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	return false;
}

bool
hccUni2KSCSymbols(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;

	//If [ASCII]
	//if (uniSyl >= 0x0020 && uniSyl <= 0x007E)
	if (uniSyl >= 0x0001 && uniSyl <= 0x007E)
	{
		*kscSyl = (unsigned char) uniSyl;
		*(kscSyl+1) = '\0';
		return true;
	}
		//Basic Greek Alphabets - Capital Letters
	else if (uniSyl >= 0x0391 && uniSyl <= 0x03A9)
	{
		uiSyl = (uniSyl - 0x0391) + 0xA5C1;
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}
		// Basic Greek Alphabets - Small Letters
	else if (uniSyl >= 0x03B1 && uniSyl <= 0x03C9)
	{
		uiSyl = (uniSyl - 0x03B1) + 0xA5E1;
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}
		// Japanese
	else if (uniSyl >= 0x3040 && uniSyl <= 0x30FF)
	{
		if (hccUni2KSCJapanese(uniSyl, kscSyl) == true)
			return true;
	}	
		// Cyrill Alphabets - Capital
	else if (uniSyl >= 0x0401 && uniSyl <= 0x047F)
	{
		if (uniSyl == 0x0401)
			uiSyl = 0xACA7;
		else if (uniSyl == 0x0451)
			uiSyl = 0xACD7;
		else if (uniSyl >= 0x0410 && uniSyl <= 0x0415)
			uiSyl = uniSyl - 0x0410 + 0xACA1;
		else if (uniSyl >= 0x0416 && uniSyl <= 0x042F)		
			uiSyl = uniSyl - 0x0410 + 0xACA8;
		else if (uniSyl >= 0x0430 && uniSyl <= 0x0435)
			uiSyl = uniSyl - 0x0430 + 0xACD1;
		else if (uniSyl >= 0x0436 && uniSyl <= 0x044F)
			uiSyl = uniSyl - 0x0436 + 0xACD8;
		else return false;

		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}
		// Number Forms
	else if (uniSyl >= 0x2150 && uniSyl <= 0x218F)
	{
		if (uniSyl >= 0x2160 && uniSyl <= 0x2169)
			uiSyl = uniSyl - 0x2160 + 0xA5B0;
		else if (uniSyl >= 0x2170 && uniSyl <= 0x2179)
			uiSyl = uniSyl - 0x2170 + 0xA5A1;
		else if (uniSyl >= 0x215B && uniSyl <= 0x215E)
			uiSyl = uniSyl - 0x215B + 0xA8FB;
		else if (uniSyl == 0x2153 || uniSyl == 0x2154)
			uiSyl = uniSyl - 0x2153 + 0xA8F7;
		else
			return false;
		
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}
		// CJK Compatibility: 3300 - 33ff
	else if (uniSyl >= 0x3300 && uniSyl <= 0x33FF)
	{
		if (hccUni2KSC_CJKCompatible(uniSyl, kscSyl) == true)
			return true;		
	}
		// Box Drawing
	else if (uniSyl >= 0x2500 && uniSyl <= 0x257F)
	{
		if (hccUni2KSC_BoxDrawing(uniSyl, kscSyl) == true)		
			return true;
		
	}

	return false;
}

void
Uni2KSCSyl(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uiSyl;
	bool bTranslated = false;

	unsigned short uniCHO;
	unsigned short uniJUNG;
	unsigned short uniJONG;
	structHSyl hSyl;



	// [Hangul] or [Hangul Compatibility Jamo]
	if ( (uniSyl >= 0xAC00 && uniSyl <= 0xD7A3) ||			// [Hangul]
		 (uniSyl >= 0x3131 && uniSyl <= 0x318E) )			// [Hangul Compatibility Jamo]
	{
		bTranslated = hccUni2KSCSyl(uniSyl, kscSyl);
		
		if (bTranslated == false)
		{
			uniCHO = (uniSyl - 0xAC00) / 588;
			uniJUNG = ((uniSyl - 0xAC00) % 588) / 28;
			uniJONG = (uniSyl - 0xAC00) % 28;

			uniCHO = uniCHO + 0x0002;
			uniJUNG = hctbl_UNI2KSSM_JUNG[uniJUNG];
			HAN_COMBI(hSyl, uniCHO, uniJUNG, KSSM_JONG_FILL);			


			uiSyl = ConvertTG2KSSyl(hSyl.hCode);

			*kscSyl = uiSyl >> 8;
			*(kscSyl+1) = uiSyl & 0x00ff;

			// JONG to CHO Convert
			uniJONG = hctbl_UNIJONG2KSCHO[uniJONG];
			*(kscSyl+2) = uniJONG >> 8;
			*(kscSyl+3) = uniJONG & 0x00ff;
			*(kscSyl+4) = '\0';
		}

		return;
	}	

		// [Latin-1, Latin Extended-A, Latin Extended-B
	//if (uniSyl >= 0x0020 && uniSyl <= 0x023F)	
	if (uniSyl >= 0x0001 && uniSyl <= 0x023F)	
		bTranslated = hccUni2KSC_Latin(uniSyl, kscSyl);	
	
		// [Spacing Modifier Letters]
	else if (uniSyl >= 0x02B0 && uniSyl <= 0x02EF)	
		bTranslated = hccUni2KSC_SpacingModifier(uniSyl, kscSyl);		
	
		// [Basic Greek]
	else if (uniSyl >= 0x0370 && uniSyl <= 0x03CF)	
		bTranslated = hccUni2KSCSymbols(uniSyl, kscSyl);

		// [Cyrillic]
	else if (uniSyl >= 0x0400 && uniSyl <= 0x04FF)	
		bTranslated = hccUni2KSCSymbols(uniSyl, kscSyl);			
	
		// [General Punctuation]
	else if (uniSyl >= 0x2000 && uniSyl <= 0x205F)	
		bTranslated = hccUni2KSC_GeneralPunc(uniSyl, kscSyl);
			
		// [Superscripts and Subscripts]
	else if (uniSyl >= 0x2070 && uniSyl <= 0x208F)
		bTranslated = hccUni2KSC_DefaultChars(uniSyl, kscSyl);

		// [Currency Symbols]
	else if (uniSyl >= 0x20A0 && uniSyl <= 0x20CF)
		bTranslated = hccUni2KSC_DefaultChars(uniSyl, kscSyl);

		// [Letter Like Symbols]
	else if (uniSyl >= 0x2100 && uniSyl <= 0x213F)
		bTranslated = hccUni2KSC_DefaultChars(uniSyl, kscSyl);

		// [Number Fomrs]
	else if (uniSyl >= 0x2150 && uniSyl <= 0x218F)
		bTranslated = hccUni2KSCSymbols(uniSyl, kscSyl);

		// [Arrows]
	else if (uniSyl >= 0x2190 && uniSyl <= 0x21FF)
		bTranslated = hccUni2KSC_DefaultChars(uniSyl, kscSyl);
	
		// [Mathematical Operators]
	else if (uniSyl >= 0x2200 && uniSyl <= 0x22FF)
		bTranslated = hccUni2KSC_MathOperator(uniSyl, kscSyl);

		// [Miscellaneous Technical]
	else if (uniSyl >= 0x2300 && uniSyl <= 0x23FF)
		bTranslated = hccUni2KSC_DefaultChars(uniSyl, kscSyl);

		// [Enclosed Alphnumerics]
	else if (uniSyl >= 0x2460 && uniSyl <= 0x24FF)
		bTranslated = hccUni2KSC_EnclosedAlphaNumerics(uniSyl, kscSyl);

		// [Box Drawing]
	else if (uniSyl >= 0x2500 && uniSyl <= 0x257F)
		bTranslated = hccUni2KSC_BoxDrawing(uniSyl, kscSyl);

		// [ Block Elements]
	else if (uniSyl >= 0x2580 && uniSyl <= 0x259F)
		bTranslated = hccUni2KSC_DefaultChars(uniSyl, kscSyl);

		// [Geometric Shapes]
	else if (uniSyl >= 0x25A0 && uniSyl <= 0x25FF)
		bTranslated = hccUni2KSC_GeometricShapes(uniSyl, kscSyl);

		// [Miscellaneous Dingbats]
	else if (uniSyl >= 0x2600 && uniSyl <= 0x26FF)
	{
		bTranslated = hccUni2KSC_MiscDingbats(uniSyl, kscSyl);		
	}

		// [CJK Symbols and Punctuation]
	else if (uniSyl >= 0x3000 && uniSyl <= 0x303F)
		bTranslated = hccUni2KSC_CJKSymPunc(uniSyl, kscSyl);

		// (Japanese) [Hiragana] [Katakana]
	else if (uniSyl >= 0x3040 && uniSyl <= 0x30FF)
		bTranslated = hccUni2KSCJapanese(uniSyl, kscSyl);

		// [Enclosed CJK Letters & Months]
	else if (uniSyl >= 0x3200 && uniSyl <= 0x32FF)
		bTranslated = hccUni2KSC_EnclosedCJKLetters(uniSyl, kscSyl);

		// [CJK Compatibility]
	else if (uniSyl >= 0x3300 && uniSyl <= 0x33FF)
		bTranslated = hccUni2KSC_CJKCompatible(uniSyl, kscSyl);
		
		// [Halfwidth and Fullwidth Forms]	
	else if (uniSyl >= 0xFF00 && uniSyl <= 0xFFEF)
		bTranslated = hccUni2KSC_HalfFullForms(uniSyl, kscSyl);

	if (bTranslated == false)
	{
		*kscSyl = ' ';
		*(kscSyl+1) = '\0';
	}	
}


int
Uni2KSCString(wchar_t *uniString, unsigned char *kscString)
{
	unsigned char kscBuf[128];
	unsigned char *pStr;
	
	unsigned long i;
	int kscLen;

	for (pStr = kscString, i = 0, kscLen = 0; uniString[i] != 0x0000; i++)
	{
		Uni2KSCSyl(uniString[i], kscBuf);
		strcpy((char *) pStr, (char *) kscBuf);
		pStr = (unsigned char *) strchr((char *) pStr, '\0');
		kscLen += strlen((char *) kscBuf);
	}

	return kscLen;
}



//**************************************************************
//
// KS C 5601 to Unicode Conversion
//
//**************************************************************

bool
KSC2UniSyl(unsigned char *kscSyl, wchar_t *uniSyl)
{
	unsigned short ksSyl;
	//structHSyl tgSyl;
	//unsigned short uniCHO;
	//unsigned short uniJUNG;
	//unsigned short uniJONG;		

	if (!kscSyl || !(*kscSyl) )
	{
		*uniSyl = 0x0000;
		return false;
	}
	
	if ( ! (*kscSyl & 0x80) )
	{
		*uniSyl = (wchar_t) *kscSyl;
		return true;
	}
	

	ksSyl = (*kscSyl << 8) | (*(kscSyl+1) & 0x00ff);

	if (hccKSC2UniSyl(ksSyl, uniSyl) == false)
	{
		*uniSyl = 0x0020;		
		return true;
	}

	return true;
}


int
KSC2UniString(unsigned char *kscString, wchar_t *uniString)
{
	unsigned char *pStr;
	unsigned long i;
	int uniLen;

	for (pStr = kscString, i = 0, uniLen = 0; *pStr; pStr++, i++, uniLen++)
	{
		if (KSC2UniSyl(pStr, &uniString[i]) == true)
		{
			if (uniString[i] == 0x0000)
				uniString[i] = KSC2UNI_DEFAULT_CHAR;
		}
		else
			uniString[i] = KSC2UNI_DEFAULT_CHAR;

		if (*pStr & 0x80)
			pStr++;		
	}

	uniString[i] = 0x0000;

	return uniLen;
}

bool
hccKSC2Uni_BlockA8(unsigned short kscSyl, wchar_t *uniSyl)
{
	switch (kscSyl)
	{
	case 0xA8A1:
		*uniSyl = 0x00C6; return true;
	case 0xA8A2:
		*uniSyl = 0x00D0; return true;
	case 0xA8A4:
		*uniSyl = 0x0126; return true;
	case 0xA8A6:
		*uniSyl = 0x0132; return true;
	case 0xA8A8:
		*uniSyl = 0x013F; return true;
	case 0xA8A9:
		*uniSyl = 0x0141; return true;
	case 0xA8AA:
		*uniSyl = 0x00D8; return true;
	case 0xA8AC:
		*uniSyl = 0x00BA; return true;
	case 0xA8AD:
		*uniSyl = 0x00DE; return true;
	case 0xA8AE:
		*uniSyl = 0x0166; return true;
	case 0xA8AF:
		*uniSyl = 0x014A; return true;
	case 0xA8F6:
		*uniSyl = 0x00BD; return true;
	case 0xA8F7:
		*uniSyl = 0x2153; return true;
	case 0xA8F8:
		*uniSyl = 0x2154; return true;
	case 0xA8F9:
		*uniSyl = 0x00BC; return true;
	case 0xA8FA:
		*uniSyl = 0x00BE; return true;
	}

	if (kscSyl >= 0xA8FB && kscSyl <= 0xA8FE)
	{
		*uniSyl = kscSyl - 0xA8FB + 0x215B;
		return true;
	}

	if (kscSyl >= 0xA8B1 && kscSyl <= 0xA8CC)
	{
		*uniSyl = kscSyl - 0xA8B1 + 0x3260;
		return true;
	}

	if (kscSyl >= 0xA8CD && kscSyl <= 0xA8E6)
	{
		*uniSyl = kscSyl - 0xA8CD + 0x24D0;
		return true;
	}

	if (kscSyl >= 0xA8E7 && kscSyl <= 0xA8F5)
	{
		*uniSyl = kscSyl - 0xA8E7 + 0x2460;
		return true;
	}

	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;
}

bool
hccKSC2Uni_BlockA9(unsigned short kscSyl, wchar_t *uniSyl)
{

	switch (kscSyl)
	{
	case 0xA9A1:
		*uniSyl = 0x00E6; return true;
	case 0xA9A2:
		*uniSyl = 0x0111; return true;
	case 0xA9A3:
		*uniSyl = 0x00F0; return true;
	case 0xA9A4:
		*uniSyl = 0x0127; return true;
	case 0xA9A5:
		*uniSyl = 0x0131; return true;
	case 0xA9A6:
		*uniSyl = 0x0133; return true;
	case 0xA9A7:
		*uniSyl = 0x0138; return true;
	case 0xA9A8:
		*uniSyl = 0x0140; return true;
	case 0xA9A9:
		*uniSyl = 0x0142; return true;
	case 0xA9AA:
		*uniSyl = 0x00F8; return true;
	case 0xA9AC:
		*uniSyl = 0x00DF; return true;
	case 0xA9AD:
		*uniSyl = 0x00FE; return true;
	case 0xA9AE:
		*uniSyl = 0x0167; return true;
	case 0xA9AF:
		*uniSyl = 0x014B; return true;
	case 0xA9B0:
		*uniSyl = 0x0149; return true;
	case 0xA9FA:
		*uniSyl = 0x207F; return true;
	case 0xA9F6:
		*uniSyl = 0x00B9; return true;
	case 0xA9F7:
		*uniSyl = 0x00B2; return true;
	case 0xA9F8:
		*uniSyl = 0x00B3; return true;
	}

	if (kscSyl >= 0xA9B1 && kscSyl <= 0xA9CC)
	{
		*uniSyl = kscSyl - 0xA9B1 + 0x3200;
		return true;
	}

	if (kscSyl >= 0xA9CD && kscSyl <= 0xA9E6)
	{
		*uniSyl = kscSyl - 0xA9CD + 0x249C;
		return true;
	}

	if (kscSyl >= 0xA9E7 && kscSyl <= 0xA9F5)
	{
		*uniSyl = kscSyl - 0xA9E7 + 0x2474;
		return true;
	}

	if (kscSyl >= 0xA9FB && kscSyl <= 0xA9FE)
	{
		*uniSyl = kscSyl - 0xA9FB + 0x2081;
		return true;
	}

	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;
}


bool
hccKSC2Uni_BlockAAAB(unsigned short kscSyl, wchar_t *uniSyl)
{
	if (kscSyl >= 0xAAA1 && kscSyl <= 0xAAF3)
	{
		*uniSyl = kscSyl - 0xAAA1 + 0x3041;
		return true;
	}
	else if (kscSyl >= 0xABA1 && kscSyl <= 0xABF6)
	{
		*uniSyl = kscSyl - 0xABA1 + 0x30A1;
		return true;
	}

	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;
}

bool
hccKSC2Uni_BlockAC(unsigned short kscSyl, wchar_t *uniSyl)
{
	if (kscSyl == 0xACA7)
	{
		*uniSyl = 0x0401;
		return true;
	}
	else if (kscSyl == 0xACD7)
	{
		*uniSyl = 0x0451;
		return true;
	}
	else if (kscSyl >= 0xACA1 && kscSyl <= 0xACA6)
	{
		*uniSyl = kscSyl - 0xACA1 + 0x0410;
		return true;
	}
	else if (kscSyl >= 0xACA8 && kscSyl <= 0xACC1)
	{
		*uniSyl = kscSyl - 0xACA8 + 0x0416;
		return true;
	}
	else if (kscSyl >= 0xACD1 && kscSyl <= 0xACD6)
	{
		*uniSyl = kscSyl - 0xACD1 + 0x0430;
		return true;
	}
	else if (kscSyl >= 0xACD8 && kscSyl <= 0xACF1)
	{
		*uniSyl = kscSyl - 0xACD8 + 0x0436;
		return true;
	}

	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;
}

bool
hccKSC2Uni_BlockA1A2(unsigned short kscSyl, wchar_t *uniSyl)
{
	int idx;	
	unsigned short uiSyl;


	if (kscSyl >= 0xa1a1 && kscSyl <= 0xa1ff)
	{
		idx = kscSyl - 0xa1a0;
		if (idx >= NUMOF_KSC_A1 || idx < 0)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}

		uiSyl = hctbl_KSC2UNI_BLOCK_A1[idx];
		if (uiSyl == 0xffff)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}
		else
		{
			*uniSyl = uiSyl;
			return true;
		}
	}
	else if (kscSyl >= 0xa2a0 && kscSyl <= 0xa2ef)
	{
		idx = kscSyl - 0xa2a0;
		if (idx >= NUMOF_KSC_A2 || idx < 0)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}
		uiSyl = hctbl_KSC2UNI_BLOCK_A2[idx];
		if (uiSyl == 0xffff)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}
		else
		{
			*uniSyl = uiSyl;
			return true;
		}
	}

	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;			
}

bool
hccKSC2Uni_BlockA6(unsigned short kscSyl, wchar_t *uniSyl)
{
	int idx;
	unsigned short uiSyl;

	if (kscSyl >= 0xA6A0 && kscSyl <= 0xA6FF)
	{
		idx = kscSyl - 0xA6A0;
		if (idx < 0 || idx >= NUMOF_KSC_A6)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}

		uiSyl = hctbl_KSC2UNI_BLOCK_A6[idx];

		if (uiSyl == 0xffff)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}

		*uniSyl = uiSyl;
		return true;
	}

	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;
}

bool
hccKSC2Uni_BlockA7(unsigned short kscSyl, wchar_t *uniSyl)
{
	int idx;
	unsigned short uiSyl;

	if (kscSyl >= 0xA7A0 && kscSyl <= 0xA7FF)
	{
		idx = kscSyl - 0xA7A0;
		if (idx < 0 || idx >= NUMOF_KSC_A7)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}

		uiSyl = hctbl_KSC2UNI_BLOCK_A7[idx];

		if (uiSyl == 0xffff)
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}

		*uniSyl = uiSyl;
		return true;
	}

	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;
}

bool
hccKSC2UniSyl(unsigned short kscSyl, wchar_t *uniSyl)
{
	structHSyl tgSyl;

	unsigned short uniCHO;
	unsigned short uniJUNG;
	unsigned short uniJONG;
	
	

		// KS C 5601 Block A1 - A2
	if (kscSyl >= 0xa1a0 && kscSyl <= 0xa2ff)
	{
		if (hccKSC2Uni_BlockA1A2(kscSyl, uniSyl) == false)		
		{
			return false;		
		}
		else 		
		{			
			return true;
		}
		
	}
		// KS C 5601: Block A3 [Full-width]
	else if (kscSyl >= 0xA3A0 && kscSyl <= 0xA3FE)
	{
		if (kscSyl == 0xA3FF)
			return false;
		*uniSyl = kscSyl - 0xA3A0 + 0x0020;
		return true;
	}
		// KS C 5601: Block A4 [JAMO-Syllable]
	else if (kscSyl >= 0xA4A0 && kscSyl <= 0xA4FF)
	{
		if (kscSyl == 0xA4A0 || kscSyl == 0xA4FF)
			return false;
		*uniSyl = kscSyl - 0xA4A1 + 0x3131;
		return true;
	}
			// KS C 5601: Block A5 [Roman/Greek]
	else if (kscSyl >= 0xA5A0 && kscSyl <= 0xA5FF)
	{
			// KS C 5601: Roman Alphabet - Number
		if (kscSyl >= 0xA5A1 && kscSyl <= 0xA5AA)
		{
			*uniSyl = kscSyl - 0xA5A1 + 0x2170;
			return true;
		}
			// KS C 5601: Roman Alphabet - Number
		else if (kscSyl >= 0xA5B0 && kscSyl <= 0xA5B9)
		{
			*uniSyl = kscSyl - 0xA5B0 + 0x2160;
			return true;
		}
			// KS C 5601: Basic Greek Alphabet
		else if (kscSyl >= 0xA5C1 && kscSyl <= 0xA5D1)
		{
			*uniSyl = kscSyl - 0xA5C1 + 0x0391;
			return true;
		}
		else if (kscSyl >= 0xA5D2 && kscSyl <= 0xA5D8)
		{
			*uniSyl = kscSyl - 0xA5D2 + 0x03A3;
			return true;
		}
		else if (kscSyl >= 0xA5E1 && kscSyl <= 0xA5F1)
		{
			*uniSyl = kscSyl - 0xA5E1 + 0x03B1;
			return true;
		}
		else if (kscSyl >= 0xA5F2 && kscSyl <= 0xA5F8)
		{
			*uniSyl = kscSyl - 0xA5F2 + 0x03C3;
			return true;
		}

		return false;
	}
		// Block A6
	else if (kscSyl >= 0xA6A0 && kscSyl <= 0xA6FF)
	{
		if (hccKSC2Uni_BlockA6(kscSyl, uniSyl) == true)
			return true;
		else
			return false;
	}
		// Block A7
	else if (kscSyl >= 0xA7A0 && kscSyl <= 0xA7FF)
	{
		if (hccKSC2Uni_BlockA7(kscSyl, uniSyl) == true)
			return true;
		else
			return false;
	}
		// Block A8
	else if (kscSyl >= 0xA8A0 && kscSyl <= 0xA8FF)
	{
		if (hccKSC2Uni_BlockA8(kscSyl, uniSyl) == true)
			return true;
		else
			return false;
	}
		// Block A9
	else if (kscSyl >= 0xA9A0 && kscSyl <= 0xA9FF)
	{
		if (hccKSC2Uni_BlockA9(kscSyl, uniSyl) == true)
			return true;
		else
			 return false;
	}
		// Block AA - AB
	else if (kscSyl >= 0xAAA0 && kscSyl <= 0xABFF)
	{
		if (hccKSC2Uni_BlockAAAB(kscSyl, uniSyl) == true)
			return true;
		else
			return false;
	}
		// Block AC
	else if (kscSyl >= 0xACA0 && kscSyl <= 0xACFF)
	{
		if (hccKSC2Uni_BlockAC(kscSyl, uniSyl) == true)
			return true;
		else
			return false;
	}


	// Check if KSC 5601: 2,350 Chars 

	if (kscSyl >= 0xB0A0 && kscSyl <= 0xC8FF)
	{

		// Check if Microsoft Unified Hangul

		if ( ((kscSyl & 0x00ff) <= 0x00A0) || ((kscSyl & 0x00ff) == 0x00ff))
		{
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}

		tgSyl.hCode = ConvertKS2TGSyl(kscSyl);
	
		if (tgSyl.hCode == HCTBL_NOT_FOUND)
		{			
			*uniSyl = KSC2UNI_DEFAULT_CHAR;
			return false;
		}		

		uniCHO = tgSyl.hPhoneme.hCho - 0x02;
		uniJUNG = hctbl_KSSM2UNI_JUNG[tgSyl.hPhoneme.hJung];
		uniJONG = hctbl_KSSM2UNI_JONG[tgSyl.hPhoneme.hJong];

		*uniSyl = 0xAC00 + (uniCHO * 588) + uniJUNG * 28 + uniJONG;	
		return true;
	}
	*uniSyl = KSC2UNI_DEFAULT_CHAR;
	return false;
		
}


bool
hccUni2KSCSyl(wchar_t uniSyl, unsigned char *kscSyl)
{
	unsigned short uniCHO;
	unsigned short uniJUNG;
	unsigned short uniJONG;
	unsigned short kssmCHO;
	unsigned short kssmJUNG;
	unsigned short kssmJONG;

	structHSyl hSyl;
	unsigned short uiSyl;

	// If [Hangul Compatibility Jamo]
	if (uniSyl >= 0x3131 && uniSyl <= 0x318E )
	{
		uiSyl = (uniSyl - 0x3131) + 0xA4A1;
		*kscSyl = uiSyl >> 8;
		*(kscSyl+1) = uiSyl & 0x00ff;
		*(kscSyl+2) = '\0';
		return true;
	}

	// Check if [Hangul]

	if (uniSyl < 0xAC00 || uniSyl > 0xD79F)
		return false;

	//uniCHO = ((uniSyl - 0xAC00) / (21 * 28) );
	uniCHO = ((uniSyl - 0xAC00) / 588 );
	//uniJUNG = ((uniSyl - 0xAC00) % (21 * 28)) / 28;
	uniJUNG = ((uniSyl - 0xAC00) % 588) / 28;
	uniJONG = (uniSyl - 0xAC00) % 28;

	kssmCHO = uniCHO + 0x0002;
	kssmJUNG = hctbl_UNI2KSSM_JUNG[uniJUNG];
	kssmJONG = hctbl_UNI2KSSM_JONG[uniJONG];

	
	HAN_COMBI(hSyl, kssmCHO, kssmJUNG, kssmJONG);

	uiSyl = ConvertTG2KSSyl(hSyl.hCode);
	if (uiSyl == HCTBL_NOT_FOUND)
		return false;	
	
	*kscSyl = uiSyl >> 8;
	*(kscSyl+1) = uiSyl & 0x00ff;
	*(kscSyl+2) = '\0';
	return true;
}


//***********************************************
//
// AU-System requests the following APIs
//
//***********************************************

int
KSCStrLenOfUni(wchar_t *uStr)
{
	int kscLen = 0;
	int i;

	unsigned short uiCHO;
	unsigned short uiJUNG;
	unsigned short uiJONG;
	unsigned short kssmCHO;
	unsigned short kssmJUNG;
	unsigned short kssmJONG;

	structHSyl hSyl;

	if (! uStr)
		return kscLen;

	for (i = 0; uStr[i] != 0x0000; i++)
	{
		//if (uStr[i] >= 0x0020 && uStr[i] <= 0x007e)
		if (uStr[i] >= 0x0001 && uStr[i] <= 0x007e)
			kscLen++;
		else if (uStr[i] >= 0xac00 && uStr[i] <= 0xd7a3)
		{
			uiCHO = (uStr[i] - 0xAC00) / 588;
			uiJUNG = ( (uStr[i] - 0xAC00) % 588 ) / 28;
			uiJONG = (uStr[i] - 0xAC00) % 28;

			kssmCHO = uiCHO + 0x0002;
			kssmJUNG = hctbl_UNI2KSSM_JUNG[uiJUNG];
			kssmJONG = hctbl_UNI2KSSM_JONG[uiJONG];
			HAN_COMBI(hSyl, kssmCHO, kssmJUNG, kssmJONG);

// Changed by AU-System
//			if (ConvertTG2KSSyl(hSyl.hCode == HCTBL_NOT_FOUND))
			if (ConvertTG2KSSyl((unsigned short)(hSyl.hCode == HCTBL_NOT_FOUND)))
				kscLen += 4;			
			else
				kscLen += 2;
		}
		else
			kscLen += 2;
	}

	return kscLen;
}

int
UniLenOfKSCStr(char *kStr)
{
	int uniLen = 0;
	int i;

	if (! kStr)
		return uniLen;

	for (i = 0, uniLen = 0; kStr[i] != '\0'; i++, uniLen++)
	{
		if (kStr[i] & 0x80)
			i++;
	}

	return uniLen;
}

INT32
KSC2Unicode_calcLen(BYTE *str, BOOL isNullTerminated, UINT32 readBytes, UINT32 *strByteLen)
{
	int iLen;

	if (isNullTerminated == TRUE)
	{
		*strByteLen = (UINT32) strlen((char *) str);
		iLen = UniLenOfKSCStr((char *) str);
	}
	else
	{
		/* Removed by AU-System
		// In case that the 'str' does not terminate with the NULL character
		pStr = (char *) malloc(sizeof(char) * readBytes);
		memcpy(pStr, str, readBytes);
		pStr[readBytes] = '\0';
		iLen = UniLenOfKSCStr(pStr);
		free(pStr);
		*strByteLen = readBytes;
		pStr = NULL;

		Added by AU-System */
		UINT32 i;
		*strByteLen = readBytes;

		if (str)
		{
			for (i = 0, iLen = 0; i < readBytes; i++, iLen++)
			{
				if (str[i] & 0x80)
					i++;
			}
		}
	}

	return iLen;
}
