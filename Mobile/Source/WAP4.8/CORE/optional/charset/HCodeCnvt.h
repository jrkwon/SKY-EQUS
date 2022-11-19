#ifndef HCODECNVT_H
#define HCODECNVT_H
/*******************************************************************************
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
//
*******************************************************************************/


/***************************************************************************
   Removed by AU-System
****************************************************************************


#define wchar_t WCHAR


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Check the following "type definition"

//typedef int INT32;
//typedef short INT16;
//typedef unsigned int UINT32;
//typedef unsigned char BYTE;
//typedef unsigned char BOOL;
//#define TRUE    1
//#define FALSE   0
//typedef unsigned char bool;
//#define	true	1
//#define	false	0

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/


/***************************************************************************
   New by AU-System
***************************************************************************/

#include "tapicmmn.h"
#include "ansilibs.h"
#define wchar_t WCHAR


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Check the following "type definition"

#ifndef __cplusplus
typedef unsigned char bool;
#define	true	1
#define	false	0
#endif

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


/***************************************************************************
   End of new by AU-System
***************************************************************************/



void Uni2KSCSyl(wchar_t uniSyl, unsigned char *kscSyl);

bool KSC2UniSyl(unsigned char *kscSyl, wchar_t *uniSyl);

int Uni2KSCString(wchar_t *uniString, unsigned char *kscString);
int KSC2UniString(unsigned char *kscString, wchar_t *uniString);

int KSCStrLenOfUni(wchar_t *uStr);
int UniLenOfKSCStr(char *kStr);
INT32 KSC2Unicode_calcLen(BYTE *str, BOOL isNullTerminated, UINT32 readBytes, UINT32 *strByteLen);

bool hccKSC2UniSyl(unsigned short kscSyl, wchar_t *uniSyl);
bool hccUni2KSCSyl(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSCSymbols(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_CJKCompatible(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_BoxDrawing(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSCJapanese(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_DefaultChars(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_EnclosedAlphaNumerics(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_GeometricShapes(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_MiscDingbats(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_CJKSymPunc(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_EnclosedCJKLetters(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_HalfFullForms(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_SpacingModifier(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_MathOperator(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_GeneralPunc(wchar_t uniSyl, unsigned char *kscSyl);
bool hccUni2KSC_Latin(wchar_t uniSyl, unsigned char *kscSyl);


bool hccKSC2Uni_BlockAC(unsigned short kscSyl, wchar_t *uniSyl);
bool hccKSC2Uni_BlockA9(unsigned short kscSyl, wchar_t *uniSyl);
bool hccKSC2Uni_BlockAAAB(unsigned short kscSyl, wchar_t *uniSyl);
bool hccKSC2Uni_BlockA8(unsigned short kscSyl, wchar_t *uniSyl);
bool hccKSC2Uni_BlockA1A2(unsigned short kscSyl, wchar_t *uniSyl);
bool hccKSC2Uni_BlockA6(unsigned short kscSyl, wchar_t *uniSyl);
bool hccKSC2Uni_BlockA7(unsigned short kscSyl, wchar_t *uniSyl);


unsigned short searchKSTable(unsigned short uiTGCode);
unsigned short ConvertTG2KSSyl(unsigned short uiTG);
unsigned short ConvertKS2TGSyl(unsigned short uiKS);

#endif
