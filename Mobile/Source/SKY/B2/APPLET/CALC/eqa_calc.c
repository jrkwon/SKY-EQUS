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
** 2000-09-03 jrkwon     Created.
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
#include "eqs.h"

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

/* ************************************************************************* *
 
   Expression --> Term [+ Term] [- Term]
   Term       --> Factor [* Factor] [/ Factor]
   Factor     --> Variable, Number or (Expression)

   -----------------------------------------------
   (*) Any part of the abobe can be NULL.
       [ ] : optional
       --> : produces

 * ************************************************************************* */

#define CALC_INT        1
#define CALC_DOUBLE     2

#define CALC_TYPE       CALC_DOUBLE


#define MAX_VARIABLES               ('Z'-'Z'+1)
#define MAX_LINE_LENGTH             256
#define MAX_TOKEN_LENGTH            81

/* Return true if c is a delimeter */
#define isdelim(c)  (strchr(" ;,+-<>/*%^=()", (c)) || (c) == 9 \
                     || (c) == '\r' || (c) == 0)
/* Return true if c is space or tab. */
#define iswhite(c)  (c == ' ' || c == '\t')


/* Calculator error codes */
enum tagCALC_RESULT {
    CALC_OK                               = 1,
    CALC_ERR_NO_EXPRESSION_PRESENT        = -100,
    CALC_ERR_UNBALANCED_PARENTHESES,
    CALC_ERR_SYNTAX_ERROR,
    CALC_ERR_NOT_A_VARIABLE,
    CALC_ERR_EQUALS_SIGN_EXPECTED
};
typedef enum tagCALC_RESULT        CALC_RESULT;

enum tagTOKEN_TYPE {
    TT_NULL,                        /* initial value */
    TT_DELIMITER,                   /* delimiter */
    TT_VARIABLE,                    /* variable */
    TT_NUMBER,                      /* number */
    TT_EOE                          /* end of the expression */
};
typedef enum tagTOKEN_TYPE       TOKEN_TYPE;

/* Primitive operand type */
#if CALC_TYPE == CALC_DOUBLE
typedef DOUBLE              OP_TYPE;     /* operand type */
#elif CALC_TYPE == CALC_INT
typedef INT32               OP_TYPE;     /* operand type */
#endif
typedef BYTE                OPERATOR;


/* A buffer for the expression. it's input to the calculator */
LOCAL BYTE g_bLine[MAX_LINE_LENGTH];

/* A variable can be sotred in from 'A' to 'Z' */
LOCAL OP_TYPE g_Variables[MAX_VARIABLES] = { 0, };

/* The current point in the buffer for the expression */
LOCAL BYTE* g_pbCP;
/* The current token buffer */
LOCAL BYTE g_bToken[MAX_TOKEN_LENGTH];
/* The current position in the token buffer */
LOCAL BYTE* g_pbToken;
/* A type of the current token */
LOCAL TOKEN_TYPE g_TokenType;



/* entry point into the parser */
LOCAL CALC_RESULT EQS_API sGetExpression(OP_TYPE* pResult);
/* Add or subtract two terms */
LOCAL VOID EQS_API sLevel2(OP_TYPE* pResult);
/* Multiply or divide tow factores  */
LOCAL VOID EQS_API sLevel3(OP_TYPE* pResult);
/* Process integer exponent */
LOCAL VOID EQS_API sLevel4(OP_TYPE* pResult);
/* Is a unary + or - */
LOCAL VOID EQS_API sLevel5(OP_TYPE* pResult);
/* Process parenthesized expression */
LOCAL CALC_RESULT EQS_API sLevel6(OP_TYPE* pResult);
/* Find value of number or variable */
LOCAL CALC_RESULT EQS_API sPrimitive(OP_TYPE* pResult);
/* Perform the specified arithmetic */
LOCAL VOID EQS_API sArithmetic(OPERATOR Operator, OP_TYPE* pResult, 
                               OP_TYPE* pHold);
/* Reverse the sign */
LOCAL VOID EQS_API sUnary(OPERATOR Operator, OP_TYPE* pResult);
/* Find the value of a variable */
LOCAL CALC_RESULT EQS_API sFindVariable(BYTE* pbVar, OP_TYPE* pValue);
/* Get a token */
LOCAL CALC_RESULT EQS_API sGetToken(VOID);
/* Return a token to input stream */
LOCAL VOID EQS_API sPutBack(VOID);
/* Assign a variable a value */
LOCAL CALC_RESULT EQS_API sAssignVariable(VOID);
LOCAL VOID EQS_API sCalculator(VOID);


/* entry point into the parser */
LOCAL CALC_RESULT EQS_API sGetExpression(OP_TYPE* pResult)
{
    sGetToken();
    if(!*g_bToken)
    {
        return CALC_ERR_NO_EXPRESSION_PRESENT;
    }
    sLevel2(pResult);
    sPutBack();

    return CALC_OK;
}

/* Add or subtract two terms */
LOCAL VOID EQS_API sLevel2(OP_TYPE* pResult)
{
    OPERATOR Operator;
    OP_TYPE Hold; 

    sLevel3(pResult);
    while((Operator = *g_bToken) == '+' || Operator == '-')
    {
        sGetToken();
        sLevel3(&Hold);
        sArithmetic(Operator, pResult, &Hold);
    }
}

/* Multiply or divide tow factores  */
LOCAL VOID EQS_API sLevel3(OP_TYPE* pResult)
{
    OPERATOR Operator;
    OP_TYPE Hold; 

    sLevel4(pResult);
    while((Operator = *g_bToken) == '*' 
            || Operator == '/' || Operator == '%')
    {
        sGetToken();
        sLevel4(&Hold);
        sArithmetic(Operator, pResult, &Hold);
    }
}

/* Process integer exponent */
LOCAL VOID EQS_API sLevel4(OP_TYPE* pResult)
{
    OP_TYPE Hold;

    sLevel5(pResult);
    if(*g_bToken == '^')
    {
        sGetToken();
        sLevel4(&Hold);
        sArithmetic('^', pResult, &Hold);
    }
}

/* Is a unary + or - */
LOCAL VOID EQS_API sLevel5(OP_TYPE* pResult)
{
    OPERATOR Operator;

    Operator = '\0';
    if((g_TokenType == TT_DELIMITER) && *g_bToken == '+' || *g_bToken == '-')
    {
        Operator = *g_bToken;
        sGetToken();
    }
    sLevel6(pResult);
    if(Operator)
        sUnary(Operator, pResult);
}

/* Process parenthesized expression */
LOCAL CALC_RESULT EQS_API sLevel6(OP_TYPE* pResult)
{
    if((*g_bToken == '(') && (g_TokenType == TT_DELIMITER))
    {
        sGetToken();
        sLevel2(pResult);
        if(*g_bToken != ')')
            return CALC_ERR_UNBALANCED_PARENTHESES;
        sGetToken();
    }
    else
        sPrimitive(pResult);

    return CALC_OK;
}

/* Find value of number or variable */
LOCAL CALC_RESULT EQS_API sPrimitive(OP_TYPE* pResult)
{
    CALC_RESULT Result;

    switch(g_TokenType)
    {
    case TT_VARIABLE : 
        Result = sFindVariable(g_bToken, pResult);
        if(Result != CALC_OK)
            return Result;
        sGetToken();
        break;
    case TT_NUMBER :
#if CALC_TYPE == CALC_INT
        *pResult = atoi(g_bToken);
#elif CALC_TYPE == CALC_DOUBLE
        *pResult = atof(g_bToken);
#endif
        sGetToken();
        break;
    default :
        return CALC_ERR_SYNTAX_ERROR;
    }
    
    return CALC_OK;
}

/* Perform the specified arithmetic */
LOCAL VOID EQS_API sArithmetic(OPERATOR Operator, OP_TYPE* pResult, 
                               OP_TYPE* pHold)
{
    OP_TYPE Exp, Temp;
    INT16 i;

    switch(Operator)
    {
    case '+' :
        *pResult = *pResult + *pHold;
        break;
    case '-' :
        *pResult = *pResult - *pHold;
        break;
    case '*' :
        *pResult = *pResult * *pHold;
        break;
    case '/' :
        *pResult = (*pResult) / (*pHold);
        break;
    case '%' :
        Temp = (*pResult) / (*pHold);
        *pResult = *pResult - (Temp * (*pHold));
        //*pResult = *pResult % *pHold;
        break;
    case '^' :
        Exp = *pResult;
        if(*pHold == 0)
        {
            *pResult = 1;
            break;
        }
        for(i = (INT16)*pHold-1; i > 0; i--)
            *pResult = (*pResult) * Exp;
        break;
    }
}

/* Reverse the sign */
LOCAL VOID EQS_API sUnary(OPERATOR Operator, OP_TYPE* pResult)
{
    if(Operator == '-')
        *pResult = -(*pResult);
}

/* Find the value of a variable */
LOCAL CALC_RESULT EQS_API sFindVariable(BYTE* pbVar, OP_TYPE* pValue)
{
    if(!isalpha(*pbVar))
    {
        return CALC_ERR_NOT_A_VARIABLE;
    }
    *pValue = g_Variables[toupper(*g_bToken)-'A'];
    return CALC_OK;
}

/* Get a token */
LOCAL CALC_RESULT EQS_API sGetToken(VOID)
{
    BYTE* pbTemp;

    g_TokenType = TT_NULL;
    pbTemp = g_bToken;
    if(*g_pbCP == '\0') /* the end of the expression */
    {
        *g_bToken = 0;
        g_TokenType = TT_EOE;
        return CALC_OK;
    }

    while(iswhite(*g_pbCP))
        g_pbCP++;

    if(*g_pbCP == '\r')
    {
        g_pbCP++; g_pbCP++;
        *g_bToken = '\r'; g_bToken[1] = '\n'; g_bToken[2] = '\0';
    }

    if(strchr("+-*^/%=;(),<>", *g_pbCP))
    {
        *pbTemp = *g_pbCP;
        g_pbCP++;
        pbTemp++;
        *pbTemp = '\0';
        g_TokenType = TT_DELIMITER;
        return CALC_OK;
    }

    /* number */
    if(isdigit(*g_pbCP))
    {
        while(!isdelim(*g_pbCP))
            *pbTemp++ = *g_pbCP++;
        *pbTemp = '\0';
        g_TokenType = TT_NUMBER;
        return CALC_OK;
    }

    /* var */
    if(isalpha(*g_pbCP))
    {
        while(!isdelim(*g_pbCP))
            *pbTemp++ = *g_pbCP++;
        g_TokenType = TT_VARIABLE;
        return CALC_OK;
    }

    *pbTemp = '\0';

    return CALC_OK;
}

/* Return a token to input stream */
LOCAL VOID EQS_API sPutBack(VOID)
{
    BYTE* pbTemp;

    pbTemp = g_bToken;
    for(; *pbTemp; pbTemp++)
        g_pbCP--;
}

/* Assign a variable a value */
LOCAL CALC_RESULT EQS_API sAssignVariable(VOID)
{
    OP_TYPE Value;
    INT8 nIndex;

    /* get the variable name */
    sGetToken();
    if(!isalpha(*g_bToken))
    {
        return CALC_ERR_EQUALS_SIGN_EXPECTED;
    }

    nIndex = toupper(*g_bToken) - 'A';

    /* get the equals sign */
    sGetToken();
    if(*g_bToken != '=')
    {
        return CALC_ERR_NOT_A_VARIABLE;
    }

    /* get the value to assign to var */
    sGetExpression(&Value);

    /* assign the value */
    g_Variables[nIndex] = Value;

    return CALC_OK;
}

LOCAL VOID EQS_API sCalculator(VOID)
{
    OP_TYPE Answer;

    strcpy(g_bLine, "-1.2+(2.2*2.0)");

    g_pbCP = g_bLine;
    do {
        sGetToken();
        if(g_TokenType == TT_VARIABLE)
        {
            sPutBack();
            sAssignVariable();
        }
        else
        {
            sPutBack();
            sGetExpression(&Answer);
            sGetToken();
        }
    } while(g_TokenType != TT_EOE);
}


VOID EQS_API EQA_Calculator(EQS_EVENT *pInEvent)
{
#ifndef T_MSM5000
    sCalculator();
#else

    switch (pInEvent->EventID)
    {
    case EQE_APP_START:
        EQS_GetDC(TRUE);

//        EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
        break;

    case EQE_APP_END:
        EQS_ReleaseDC();
        break;

    case EQE_SOFTKEY_SELECT:
        sCalculator();
        break;

    default:
        EQS_DefEventProc(pInEvent);
        break;
    }
#endif
}
