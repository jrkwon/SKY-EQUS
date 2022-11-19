/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqa_calc.c
**
** Description: 
**     The original version of this calculator is the part of the "Small C" 
**     interpreter  "C Power User's Guide which is written by HERVERT SCHILDT.
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-20 redstar     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

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
**  Global variables
**---------------------------------------------------------------------------
*/
BYTE   szResult[20];
BYTE   szNumber[20];  
BYTE   szCommResult[20];
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
void comma_sprintf(CHAR *src, CHAR *dest);
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/


#define MAX_CALCULATOR_SIZE    16

enum tagCALC_OPERATOR_E
{       
    CO_PLUS = 0,
    CO_MINUS,
    CO_MULTIPLAY,
    CO_DIVISOR,
    CO_NONE
};
typedef enum tagCALC_OPERATOR_E   CALC_OPERATOR_E;

enum tagCALC_TEXT_E
{ 
    CT_BACKGROUND = 0,
    CT_FIRST_OPERAND,
//    CT_OPERATOR,
    CT_SECOND_OPERAND  
};
typedef enum tagCALC_TEXT_E   CALC_TEXT_E;
	

void comma_sprintf(CHAR *src, CHAR *dest)
{
    INT8 cnt, i, j, rslt, ch , len, minus, space, nTemp;
	CHAR *pos, temp1[20], temp2[20];
	ch = '.';
	cnt = 0;
    temp1[0] = 0;
    temp2[0] = 0;
    
    pos = strchr(src , ch);
    
    if(pos != NULL)
    {
        rslt = pos - src;

        for(j = rslt; j >= 0 ; j-- )
        {
            if(cnt % 4 == 3)
            {
                if(src[j] != '-')
                {
                    temp1[0] = ',';
                    for( i = 0 ; temp1[i] != 0; i++ )
                    {
                        temp2[i+1] = temp1[i];
                    }
                    temp2[i+1] = 0;
                }
                memcpy(temp1, temp2, 20);
                //STRCPY(temp1, temp2);
                cnt++;
            }

            temp1[0] = src[j];

            if(rslt == j)
            {
                temp1[1] = 0;
            }
            
            if(j == 0)
            {  
                temp1[cnt+1] = 0;
                break;
            }

            for( i = 0 ; temp1[i] != 0; i++ )
            {
                temp2[i+1] = temp1[i];
            }
            temp2[i+1] = 0; 
            memcpy(temp1, temp2, 20);
            //STRCPY(temp1, temp2);

            if(rslt != j)
                cnt++;
        }

        len = STRLEN(temp1);
        space = len;
        
        for( i = rslt ; i < (MAX_CALCULATOR_SIZE + rslt) - (space + 1) ; i++)
        {
            if(i == (MAX_CALCULATOR_SIZE + rslt) - (space + 1)|| src[i] == 0 )
            {
                temp1[len] = 0;
                break;
            }
            minus = (rslt - 1 ) % 3;
            if(src[0]=='-' && src[i] == '.' && minus == 0)
                i++;
            temp1[len++] = src[i];        
            temp1[len+1] = 0;
        }
        memcpy(dest, temp1, 20);
        //STRCPY(dest , temp1);
        

    }else
    {
        rslt = STRLEN(src);
        for(j = rslt; j >= 0 ; j-- )
        {
            if((cnt % 4 == 3))
            {
                if(src[j] != '-')
                {
                    temp1[0] = ',';
                    for( i = 0 ; temp1[i] != 0; i++ )
                    {
                        temp2[i+1] = temp1[i];
                    }
                    temp2[i+1] = 0; 
                }
                memcpy(temp1, temp2, 20);
                //STRCPY(temp1, temp2);
                cnt++;
            }

            temp1[0] = src[j];

            if(j == 0)
            {  
                temp1[cnt+1] = 0;
                break;
            }

            for( i = 0 ; temp1[i] != 0; i++ )
            {
                temp2[i+1] = temp1[i];
            }
            temp2[i+1] = 0; 
            memcpy(temp1, temp2, 20);
            //STRCPY(temp1, temp2);
            if(rslt != j)
                cnt++;
        }
        memcpy(dest, temp1, 20);
        //STRCPY(dest , temp1);
    }
/*    nTemp = STRLEN(dest);
    if(dest[nTemp] == '.')
        dest[nTemp] = 0;
*/
}

void tem_sprintf( double num_value, char *num_string, int radix)
{ 
	char     tmp_buf[33], tmp1_buf[33];
    double   d1_value;
	int      i = 0, j = 0, check, tmpval, k, leng, dot_flag =0, minus_flag = 0; 
	int      zero_flag =0, zero1_flag =0, zero2_flag =0, plus_flag =0; 

	strcpy(tmp_buf, "");
	strcpy(tmp1_buf, "");

	if ( num_value < 0 ) {
		minus_flag = 1;
		num_value = num_value * (-1);
    }

    d1_value = num_value;

	for (check = 12; check >= 0; check--) {
		tmpval = (int)(d1_value / pow(10, check));
		if ((tmpval > 0) || (zero_flag !=0)) {        // zero_flag : 沥荐何 0 check 
			tmp_buf[i++] = (char)(tmpval + '0');
			zero_flag =1;
			d1_value = d1_value - (tmpval * pow(10, check));
		}
	}

	if( d1_value >0 ) {
		tmp_buf[i++] = (char)('.');
		d1_value = d1_value *10000000;
	}

	for (check = 6; check > 0; check--) {
		tmpval = (int)(d1_value / pow(10, check));
		if ((tmpval > 0) || (zero1_flag !=0) || 
			((tmpval == 0) && (d1_value > 0))) {      // zero1_flag : 家荐何 0 check
			tmp_buf[i++] = (char)(tmpval + '0');
			zero1_flag =1;
			d1_value = d1_value - (tmpval * pow(10, check));
		}
	}

	if (d1_value >= 5.0) plus_flag =1;

	for (k = i-1; k >= 0; k--) {
		if(plus_flag !=0) {
			if((char)(tmp_buf[k]) == (char)('9')) {
				if(zero2_flag !=0) {
					tmp1_buf[j++] = (char)('0');
					plus_flag = 1;
				} else { 
					if(dot_flag == 1) { 
					     tmp1_buf[j++] = (char)('0');
					} else {              
						;
					}
				}
			} else 	{
				if((tmp_buf[k]) == (char)('.')) {
					if (zero2_flag !=0) {
						tmp1_buf[j++] = (char)('.');
					} else {
						;
					}
					dot_flag =1;
					plus_flag = 1;
				} else {
					tmp1_buf[j++] = (char)(tmp_buf[k] + 1 ) ;
					plus_flag = 0;
					zero2_flag =1;
				}
			}
		} else { 
			if (((char)(tmp_buf[k]) == (char)('0')) && (zero2_flag == 0)) {
				if((dot_flag == 0) && (zero1_flag == 0)) {
					tmp1_buf[j++] = tmp_buf[k];
				} else {
					;
				}
			} else {
				tmp1_buf[j++] = tmp_buf[k];
				zero2_flag =1;
			}
		}
	}

  tmp1_buf[j] = '\0';
	leng = strlen(tmp1_buf);

	if(leng == 0 && dot_flag == 1 && plus_flag ==1)
		tmp1_buf[j++] = (char)('1');

	if(( leng > 1 ) && (tmp1_buf[j-1] == (char) ('0'))) {
		tmp1_buf[j++] = (char)('1');
	}

	if (num_value == 0) 
		tmp1_buf[j++] = (char)('0');
	else if(tmp1_buf[j-1] == (char)('.'))
		tmp1_buf[j++] = (char)('0');

	if (minus_flag) {
		tmp1_buf[j++] = (char)('-');
	}

    tmp1_buf[j] = '\0';
	leng = strlen(tmp1_buf);

	strcpy(num_string, "");
	
	for(k=0; k< leng; k++) {
        num_string[ leng - k - 1 ] = tmp1_buf[k];
    }
    num_string[ leng ] = '\0';
}


VOID SKY_API SKA_MainCalculator(SKY_EVENT_T* pInEvent)
{
    HCONTROL hControl;
    static   double dValue;
    static   CALC_OPERATOR_E   Operator;
    static   INT8   nFloatCount = 0;
    static   BOOL   fFloat, s_fResult = FALSE;
    static   INT8     nLen, printLen;
    STATIC   BOOL   fNoInput = FALSE;
    BITMAP OpBitmap, CmlBitmap;
    COLOR_SCHEME_E CommandLine;
    STATIC      BOOL    s_fOpCompare;
    DRAW_MODE     DrawMode;

    double   dNumber;

    switch (pInEvent->EventID)
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_POINT_NONE_AC, 
            SKY_GET_TOKEN(TKN_CALCULATOR), 
            WA_SHOW_HEADER|WA_SHOW_CML);

        s_fOpCompare = FALSE;
        
        s_fResult = TRUE;
        dValue     = 0.0;
        Operator  = CO_NONE;     //no operator
        fFloat    = FALSE;
        nFloatCount = 0;
        szNumber[0] = '0';
        szNumber[1] =   0;
        nLen        =   1;
        printLen    =   0;
        szCommResult[0] = '0';
        szCommResult[1] =   0;
        fNoInput = FALSE;
        
        EQC_Image(8,5,(BITMAP*)&g_BmpCalCulatorOutputBox);
        EQC_Image(13,56,(BITMAP*)&g_BmpCalCulator);

        hControl = EQC_StaticText(14,7,TKN_CALBLANK);
        EQC_SET_STYLE(hControl,CS_STATIC_TEXT_RIGHT|CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING);
        EQC_SetStaticTextColor(hControl,WHITE);

//        hControl = EQC_StaticText(4,23,TKN_BLANK); 22 + 3
//        EQC_SET_STYLE(hControl,CS_STATIC_TEXT_RIGHT|CS_STATIC_TEXT_NO_CHANGE);

        hControl = EQC_StaticText(14,36,TKN_CALBLANK);
        EQC_SET_STYLE(hControl,CS_STATIC_TEXT_RIGHT|CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING);      
        EQC_SetStaticText(hControl,szNumber);
        EQC_SetStaticTextColor(hControl,WHITE);
		

//        EQC_StaticText(15+EQS_CharWidth((BYTE)' ')*4,80,TKN_CAL_COMMAND_1);
//        EQC_StaticText(15,80+EQS_CharHeight((BYTE)' '),TKN_CAL_COMMAND_2);

          
        break;

    case SE_APP_DRAW:

        //EQS_PutBitmap(0, 0, &g_BmpCalc120x106x256C);
/*
        CommandLine = SKY_GET_COLOR_SCHEME();
        CmlBitmap = g_BmpCalcCml120x13x256C;
        CmlBitmap.pbData = g_BmpCalcCml120x13x256C.pbData + EQC_GET_FRAME_SIZE(CmlBitmap.Header)*CommandLine;
        EQS_PutBitmap(0,92, &CmlBitmap);
*/      
        //DrawMode = EQS_GetDrawMode();
        //EQS_SetDrawMode(DM_OR);
        if(Operator != CO_NONE)
        {
            OpBitmap = g_BmpCalCulatorSym;
            OpBitmap.pbData = g_BmpCalCulatorSym.pbData + EQC_GET_FRAME_SIZE(OpBitmap.Header)*Operator;
            EQS_PutBitmap(110, 23, &OpBitmap);
            //EQS_PutBitmap(105, 102, &OpBitmap);
        }
        //EQS_SetDrawMode(DrawMode);
        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;
   
    case SE_KEY_UP: 
    case SE_KEY_DOWN:     
    case SE_KEY_LEFT:   
    case SE_KEY_RIGHT:
        
        if(szNumber[0] == '-' && nLen ==1)
            break;
    
        s_fResult = FALSE;
        
        if( Operator == CO_NONE )
        {
            if((nLen == 1 && szNumber[0] == '0') && (pInEvent->EventID == SE_KEY_RIGHT))       //澜荐贸府
            {
                s_fResult = TRUE;
                szNumber[0] = '-';
                szNumber[1] = 0;
                dValue = EQS_atof((CHAR *)szNumber);

                printLen = STRLEN(szNumber);

                hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
                EQC_SetStaticText(hControl, szNumber);
                EQS_SEND_EVENT(SE_APP_DRAW,EQS_DRAW_LEVEL_ALL, 0 );
                break;
            }
            STRCPY(szResult,szNumber);  
            dValue = EQS_atof((CHAR*)szNumber);            
        }
        else
        {
            fNoInput = TRUE;
            if(!s_fOpCompare)
            {
                dNumber = EQS_atof((CHAR*)szNumber);
                switch( Operator )
                {
                case CO_PLUS:
                    dValue += dNumber;
                    break;
                case CO_MINUS:
                    dValue -= dNumber;
                    break;
                case CO_MULTIPLAY:
                    dValue *= dNumber;
                    break;                
                case CO_DIVISOR:
                    if( dNumber != 0 )
                    {
                        dValue /= dNumber;
                    }
                    else
                    {
                        EQC_MessageBox(SKY_GET_TOKEN(TKN_OVER_FLOW), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    }
                    break;
                default:
                    break;
                }
                if(Operator == CO_DIVISOR && dNumber == 0)
                {
                    break;
                }

                if(dValue > 999999999999.0 || dValue < -99999999999.0)
                {
                    EQC_MessageBox(SKY_GET_TOKEN(TKN_OVER_FLOW), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    break;
                }
                
                tem_sprintf(dValue,szResult,10);
                comma_sprintf(szResult, szCommResult);
            }
        }

        if(!s_fOpCompare)
        {
            hControl    = EQS_GetNthIOC(CT_FIRST_OPERAND);
            EQC_SetStaticText(hControl,szCommResult);      
            szNumber[0] =   '0';
            szNumber[1] =   0;
            nLen        =   1;
            printLen    =   0;
            szCommResult[0] = '0';
            szCommResult[1] = 0;
            fNoInput = FALSE;
            fFloat      = FALSE;
            nFloatCount = 0;
            hControl    = EQS_GetNthIOC(CT_SECOND_OPERAND);
            EQC_SetStaticText(hControl,szNumber);
        }
        
//        hControl = EQS_GetNthIOC(CT_OPERATOR);

        if( pInEvent->EventID == SE_KEY_UP )
        {
            Operator = CO_MULTIPLAY;                 
//            EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_CAL_OP_MULTIPLAY));            
        }        
        else if( pInEvent->EventID == SE_KEY_DOWN )
        {
            Operator = CO_DIVISOR;
//            EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_CAL_OP_DIVISOR));
        }            
        else if( pInEvent->EventID == SE_KEY_LEFT )
        {
            Operator = CO_PLUS;
//            EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_CAL_OP_PLUS));
        }            
        else // if( pInEvent->EventID == SE_KEY_RIGHT )
        {
            Operator = CO_MINUS;
//            EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_CAL_OP_MINUS));
        }
        s_fOpCompare = TRUE;
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
        
        break;

    case SE_KEY_0:
    case SE_KEY_1:
    case SE_KEY_2:
    case SE_KEY_3:
    case SE_KEY_4:
    case SE_KEY_5:
    case SE_KEY_6:
    case SE_KEY_7:
    case SE_KEY_8:
    case SE_KEY_9:

        if( printLen < MAX_CALCULATOR_SIZE-2 )
        {
            if(fNoInput)
            {
                s_fResult = TRUE;
                fFloat    = FALSE;
                nFloatCount = 0;
                szNumber[0] = '0';
                szNumber[1] =   0;
                nLen        =   1;
                printLen    =   0;
                szCommResult[0] = '0';
                szCommResult[1] = 0;
                fNoInput = FALSE;
                s_fOpCompare = FALSE;

                EQS_SEND_EVENT( pInEvent->EventID, 0, 0 );
            }else
            {
                if(nFloatCount > 5)
                    break;

                if(fFloat)
                    nFloatCount++;

                s_fResult = TRUE;
                if( nLen == 1 && szNumber[0] == '0')
                    nLen--;
                
                szNumber[nLen++] = '0' + pInEvent->EventID - SE_KEY_0 ;
                szNumber[nLen]   = 0;
            
                comma_sprintf(szNumber, szCommResult);
                printLen = STRLEN(szCommResult);

                hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);

                EQC_SetStaticText(hControl, szCommResult);
                s_fOpCompare = FALSE;
                EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
            }
        }else
        {
            if(fNoInput)
            {
                s_fResult = TRUE;
                fFloat    = FALSE;
                nFloatCount = 0;
                szNumber[0] = '0';
                szNumber[1] =   0;
                nLen        =   1;
                printLen    =   0;
                szCommResult[0] = '0';
                szCommResult[1] = 0;
                fNoInput = FALSE;
                s_fOpCompare = FALSE;
        
                EQS_SEND_EVENT( pInEvent->EventID, 0, 0 );
            }
        }
        
        break;
    case SE_SOFTKEY_F1://SE_KEY_STAR:    // point
        s_fResult = TRUE;
        if(fNoInput)
        {
            s_fResult = TRUE;
            fFloat    = FALSE;
            nFloatCount = 0;
            szNumber[0] = '0';
            szNumber[1] =   0;
            nLen        =   1;
            printLen    =   0;
            szCommResult[0] = '0';
            szCommResult[1] = 0;
            fNoInput = FALSE;
            s_fOpCompare = FALSE;
    
            EQS_SEND_EVENT( pInEvent->EventID, 0, 0 );
        }else
        {
            if( fFloat == FALSE )
            {
                fFloat = TRUE;
                nFloatCount = 0;

                if((nLen == 1) && (szNumber[0] == '-'))
                {
                    szNumber[nLen++] = '0';
                    szNumber[nLen]   = 0;                   
                }
                szNumber[nLen++] = '.';
                szNumber[nLen]   = 0;
                s_fOpCompare = FALSE;
            
                comma_sprintf(szNumber, szCommResult);
                printLen = STRLEN(szCommResult);

                hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
                EQC_SetStaticText(hControl, szCommResult);
                EQS_SEND_EVENT(SE_APP_DRAW,EQS_DRAW_LEVEL_ALL, 0 );
            }
        }
        break;

    case SE_SOFTKEY_F2://SE_KEY_POUND:  //AC
        s_fResult = TRUE;
        fNoInput = FALSE;
        dValue     = 0.0;
        Operator  = CO_NONE;     //no operator
        fFloat    = FALSE;
        nFloatCount = 0;
        szNumber[0] = '0';
        szNumber[1] =   0;
        nLen        =   1;
        printLen    =   0;
        szCommResult[0] = '0';
        szCommResult[1] = 0;
        s_fOpCompare = FALSE;
        
        printLen = STRLEN(szNumber);

        hControl = EQS_GetNthIOC(CT_FIRST_OPERAND);
        EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALBLANK));
        hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
        EQC_SetStaticText(hControl,szNumber);
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
        break;
    
    case SE_KEY_CENTER:    // result
    //case SE_KEY_POUND:   
        s_fOpCompare = FALSE;
        if( Operator == CO_NONE )
        {
            STRCPY(szResult,szNumber); 
            s_fResult = TRUE;
            dValue = EQS_atof((CHAR*)szNumber);
        }
        else 
        {
            if( nLen > 0 )
            {
                dNumber = EQS_atof((CHAR*)szNumber);
                switch( Operator  )
                {            
                case CO_PLUS:
                    dValue += dNumber;
                    break;
                case CO_MINUS:
                    dValue -= dNumber;
                    break;
                case CO_MULTIPLAY:
                    dValue *= dNumber;
                    break;                
                case CO_DIVISOR:
                    if( dNumber != 0 )
                    {
                        dValue /= dNumber;
                    }
                    else
                    {
                        EQC_MessageBox(SKY_GET_TOKEN(TKN_OVER_FLOW), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                        //dValue = 0.0;        // devide by zero
                    }
                    break;
                default:
                    break;   
                }
            }
            if(Operator == CO_DIVISOR && dNumber == 0)
            {
                break;
            }

            Operator = CO_NONE;
            if(dValue > 999999999999.0 || dValue < -99999999999.0)
            {
                EQC_MessageBox(SKY_GET_TOKEN(TKN_OVER_FLOW), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                break;
            }
            tem_sprintf(dValue,szNumber,10);
            
            comma_sprintf(szNumber, szCommResult);
            printLen = STRLEN(szCommResult);
            
            hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
            EQC_SetStaticText(hControl,szCommResult);
            
            fNoInput = TRUE;
            s_fResult = TRUE;
            dValue     = 0.0;
            Operator  = CO_NONE;     //no operator
            fFloat    = FALSE;
            nFloatCount = 0;
            nLen = STRLEN(szNumber);
            //szNumber[0] = '0';
            //szNumber[1] =   0;
            //nLen        =   1;
            //printLen    =   0;
            //szCommResult[0] = 0;

        }                  

        hControl = EQS_GetNthIOC(CT_FIRST_OPERAND);
        EQC_SetStaticText(hControl,(BYTE*)"");
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
        break;

    case SE_KEY_CLEAR:
        if( nLen > 0 )
        {
            if(fNoInput)
            {
                s_fResult = TRUE;
                fNoInput = FALSE;
                dValue     = 0.0;
                Operator  = CO_NONE;     //no operator
                fFloat    = FALSE;
                nFloatCount = 0;
                szNumber[0] = '0';
                szNumber[1] =   0;
                nLen        =   1;
                printLen    =   0;
                szCommResult[0] = '0';
                szCommResult[1] = 0;
                s_fOpCompare = FALSE;
        
                printLen = STRLEN(szNumber);

                hControl = EQS_GetNthIOC(CT_FIRST_OPERAND);
                EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALBLANK));
                hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
                EQC_SetStaticText(hControl,szNumber);
                EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
                break;
            }

            if( szNumber[nLen-1] == '.' )
            {   
                fFloat = FALSE;
                nFloatCount = 0;
            }
            
            if(nLen == 1 && szNumber[nLen-1] == '0' )
            {
                if(Operator == CO_NONE && s_fResult)
                    EQS_End();
                else
                {
                    fNoInput = FALSE;
                    s_fResult = TRUE;
                    dValue     = 0.0;
                    Operator  = CO_NONE;     
                    fFloat    = FALSE;
                    nFloatCount = 0;
                    szNumber[0] = '0';
                    szNumber[1] =   0;
                    nLen        =   1;
                    printLen    =   0;
                    szCommResult[0] = 0;
                    s_fOpCompare = FALSE;

                    hControl = EQS_GetNthIOC(CT_FIRST_OPERAND);
                    EQC_SetStaticText(hControl,(BYTE*)"");
                    hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
                    EQC_SetStaticText(hControl,(BYTE*)"0");
                    EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
                    break;
                }
                //EQS_End();
            }else if(nLen == 1)
            {
                szNumber[nLen] = 0;
                szNumber[nLen-1] = '0';
            }else 
            {
                szNumber[nLen-1] = 0;
                nLen--;

                if(fFloat)
                    nFloatCount--;

            }
            
            comma_sprintf(szNumber, szCommResult);
            printLen = STRLEN(szCommResult);

            hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
            EQC_SetStaticText(hControl,szCommResult);
            EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
        }
        break;

    case SE_KEY_CLEAR_LONG:
        s_fResult = TRUE;
        fNoInput = FALSE;
        dValue     = 0.0;
        Operator  = CO_NONE;     
        fFloat    = FALSE;
        nFloatCount = 0;
        szNumber[0] = '0';
        szNumber[1] =   0;
        nLen        =   1;
        printLen    =   0;
        szCommResult[0] = 0;
        s_fOpCompare = FALSE;

        hControl = EQS_GetNthIOC(CT_FIRST_OPERAND);
        EQC_SetStaticText(hControl,(BYTE*)"");
        hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
        EQC_SetStaticText(hControl,(BYTE*)"0");
        EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, 0 );
        break;

    case SE_IO_MESGBOX_END:
        EQS_START_SIBLING( SKA_MainCalculator, FALSE);
/*        s_fResult = TRUE;
        fNoInput = FALSE;
        dValue     = 0.0;
        Operator  = CO_NONE;     //no operator
        fFloat    = FALSE;
        nFloatCount = 0;
        szNumber[0] = '0';
        szNumber[1] =   0;
        nLen        =   1;
        printLen    =   0;
        szCommResult[0] = '0';
        szCommResult[1] =   0;
        
        hControl = EQS_GetNthIOC(CT_FIRST_OPERAND);
        EQC_SetStaticText(hControl,szCommResult);
//        hControl = EQS_GetNthIOC(CT_OPERATOR);
        EQC_SetStaticText(hControl,szCommResult);
        hControl = EQS_GetNthIOC(CT_SECOND_OPERAND);
        EQC_SetStaticText(hControl,szNumber);
*/
        break;

    default:    
        SKY_DefEventProc( pInEvent );
        break;
    }

}
