/*
# clearcase: CmicroPatchRel2.3.3
*/

/*
+------------------------------------------------------------------------------+
|                                                                              |
|  Copyright by Telelogic AB 1993 - 1998                                       | 
|  Copyright by S&P Media GmbH Germany 1993 - 1998                             | 
|                                                                              |
|  This Program is owned by Telelogic and is protected by national             | 
|  copyright laws and international copyright treaties. Telelogic              | 
|  grants you the right to use this Program on one computer or in              | 
|  one local computer network at any one time.                                 | 
|  Under this License you may only modify the source code for the purpose      | 
|  of adapting it to your environment. You must reproduce and include          | 
|  any copyright and trademark notices on all copies of the source code.       | 
|  You may not use, copy, merge, modify or transfer the Program except as      | 
|  provided in this License.                                                   | 
|  Telelogic does not warrant that the Program will meet your                  | 
|  requirements or that the operation of the Program will be                   | 
|  uninterrupted and error free. You are solely responsible that the           | 
|  selection of the Program and the modification of the source code            | 
|  will achieve your intended results and that the results are actually        | 
|  obtained.                                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/ 
 

#ifndef X_SCTPRED_H
#define X_SCTPRED_H

/* This file contains implementation of the predefined data types
   except PId. The functions defined here can be found in the file
   sctpred.c.
*/
/*
FILE INDEX
00   Some utility macros
01   Typedef, Default value, Literals
02   Prototypes and macros for the operators of the data types
03   Read and Write functions

     NOTE: 
     The predefined generators are contained in an extra file
     "sctpredg.h" which is included at the end of this file.
*/
          
/* 
** General check on the codegenerator that is used 
*/ 
#if !defined(XSCT_CBASIC) && !defined(XSCT_CADVANCED) && !defined(XSCT_CMICRO)
 #error "ERROR_in_sctpred_h_Please_Define_A_Codegenerator"
#endif


/****+***************************************************************
00   Some utility macros
********************************************************************/

/* Macros used in the generator implementations */

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#ifdef XECSOP
#define xReportStringError(S1,S2)   xSDLOpError(S1,S2);
#else
#define xReportStringError(S1,S2)
#endif /* ... XECSOP */

#else /* then XSCT_CMICRO */

#ifdef XMK_USE_MAX_ERR_CHECK
#define xReportStringError(S1,S2)   xSDLOpError(S1,S2);
#else
#define xReportStringError(S1,S2)   ErrorHandler (ERR_N_PREDEFINED_OPERATOR_CALL);
#endif /* ... XMK_USE_MAX_ERR_CHECK */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */


/****+***************************************************************
01   Typedef, Default value, Literals
********************************************************************/

/*---+---------------------------------------------------------------
     Boolean
-------------------------------------------------------------------*/

#if !defined(X_COMPACT_BOOL)
typedef int     SDL_Boolean;
typedef int     SDL_boolean;
#else
typedef unsigned char     SDL_Boolean;
typedef unsigned char     SDL_boolean;
#endif

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_Boolean(V)       *(V) = (SDL_Boolean)0
#define yDef_SDL_Boolean(V)       xDef_SDL_Boolean(V)

#else /* then XSCT_CMICRO */

#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
#define xDef_SDL_Boolean(V)

#else

#define xDef_SDL_Boolean(V)       *(V) = (SDL_Boolean)0

#endif /* ... XMK_USE_KERNEL_INIT */

#define yDef_SDL_Boolean(V)       xDef_SDL_Boolean(V)

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yAssF_SDL_Boolean(V,E,A)  (V = E)
#define yEqF_SDL_Boolean(E1,E2)   (E1) == (E2)
#define yNEqF_SDL_Boolean(E1,E2)  (E1) != (E2)
#define ySrtN_SDL_Boolean         xSrtN_SDL_Boolean

#define SDL_False 0
#define SDL_false 0
#define SDL_True  1
#define SDL_true  1


/*---+---------------------------------------------------------------
     Bit
-------------------------------------------------------------------*/
typedef unsigned char SDL_Bit;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_Bit(V)       *(V) = (SDL_Bit)0

#else /* then XSCT_CMICRO */

#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
#define xDef_SDL_Bit(V)

#else

#define xDef_SDL_Bit(V)       *(V) = (SDL_Bit)0

#endif /* ... XMK_USE_KERNEL_INIT */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_Bit(V)       xDef_SDL_Bit(V)
#define yAssF_SDL_Bit(V,E,A)  (V = E)
#define yEqF_SDL_Bit(E1,E2)   ((E1) == (E2))
#define yNEqF_SDL_Bit(E1,E2)  ((E1) != (E2))
#define ySrtN_SDL_Bit         xSrtN_SDL_Bit

#define SDL_BIT_LIT(I)        I


/*---+---------------------------------------------------------------
     Bit_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

typedef struct {
  int             Length;
  xbool           IsAssigned;
  xbool           IsUsedInSignal;
  unsigned char * Bits;
} SDL_Bit_String;

#define xDef_SDL_Bit_String(V) \
  (*V).Length = 0; (*V).IsUsedInSignal = (xbool)0; (*V).IsAssigned = (xbool)1; (*V).Bits = 0;
#define yDef_SDL_Bit_String(V)       xDef_SDL_Bit_String(V)
#define yAssF_SDL_Bit_String(V,E,A)  xAss_SDL_Bit_String(&(V),E,A)
#define yEqF_SDL_Bit_String(E1,E2)   xEq_SDL_Bit_String(E1,E2)
#define yNEqF_SDL_Bit_String(E1,E2)  (! xEq_SDL_Bit_String(E1,E2))
#define ySrtN_SDL_Bit_String         xSrtN_SDL_Bit_String

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*---+---------------------------------------------------------------
     Octet
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

typedef unsigned char SDL_Octet;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_Octet(V)       *(V) = (SDL_Octet)0

#else /* then XSCT_CMICRO */

#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
#define xDef_SDL_Octet(V)

#else
 
#define xDef_SDL_Octet(V)       *(V) = (SDL_Octet)0
 
#endif /* ... XMK_USE_KERNEL_INIT */
 
#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_Octet(V)       xDef_SDL_Octet(V)
#define yAssF_SDL_Octet(V,E,A)  (V = E)
#define yEqF_SDL_Octet(E1,E2)   ((E1) == (E2))
#define yNEqF_SDL_Octet(E1,E2)  ((E1) != (E2))
#define ySrtN_SDL_Octet         xSrtN_SDL_Octet

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*---+---------------------------------------------------------------
     Octet_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

typedef SDL_Bit_String SDL_Octet_String;

#define xDef_SDL_Octet_String(V) \
  (*V).Length = 0; (*V).IsUsedInSignal = (xbool)0; (*V).IsAssigned = (xbool)1; (*V).Bits = 0;
#define yDef_SDL_Octet_String(V)       xDef_SDL_Octet_String(V)
#define yAssF_SDL_Octet_String(V,E,A)  xAss_SDL_Bit_String(&(V),E,A)
#define yEqF_SDL_Octet_String(E1,E2)   xEq_SDL_Bit_String(E1,E2)
#define yNEqF_SDL_Octet_String(E1,E2)  (! xEq_SDL_Bit_String(E1,E2))
#define ySrtN_SDL_Octet_String         xSrtN_SDL_Octet_String

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*---+---------------------------------------------------------------
     Character
-------------------------------------------------------------------*/

typedef char    SDL_Character;
typedef char    SDL_character;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_Character(V)       *(V) = '\000'

#else /* then XSCT_CMICRO */

#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
#define xDef_SDL_Character(V)       

#else

#define xDef_SDL_Character(V)       *(V) = '\000'

#endif /* ... XMK_USE_KERNEL_INIT */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_Character(V)       xDef_SDL_Character(V)
#define yAssF_SDL_Character(V,E,A)  (V = E)
#define yEqF_SDL_Character(E1,E2)   ((E1) == (E2))
#define yNEqF_SDL_Character(E1,E2)  ((E1) != (E2))
#define ySrtN_SDL_Character         xSrtN_SDL_Character

/* predefined literals for non-printable characters,
   other literals as in C */
#define SDL_NUL '\000'
#define SDL_SOH '\001'
#define SDL_STX '\002'
#define SDL_ETX '\003'
#define SDL_EOT '\004'
#define SDL_ENQ '\005'
#define SDL_ACK '\006'
#define SDL_BEL '\007'
#define SDL_BS  '\010'
#define SDL_HT  '\011'
#define SDL_LF  '\012'
#define SDL_VT  '\013'
#define SDL_FF  '\014'
#define SDL_CR  '\015'
#define SDL_SO  '\016'
#define SDL_SI  '\017'
#define SDL_DLE '\020'
#define SDL_DC1 '\021'
#define SDL_DC2 '\022'
#define SDL_DC3 '\023'
#define SDL_DC4 '\024'
#define SDL_NAK '\025'
#define SDL_SYN '\026'
#define SDL_ETB '\027'
#define SDL_CAN '\030'
#define SDL_EM  '\031'
#define SDL_SUB '\032'
#define SDL_ESC '\033'
#define SDL_IS4 '\034'
#define SDL_IS3 '\035'
#define SDL_IS2 '\036'
#define SDL_IS1 '\037'
#define SDL_DEL '\177'


/*---+---------------------------------------------------------------
     Charstring
-------------------------------------------------------------------*/

#ifndef XNOUSEOFCHARSTRING


#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

typedef char   *SDL_Charstring;
typedef char   *SDL_charstring;

#define xDef_SDL_Charstring(V) \
   xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)
#define yDef_SDL_Charstring(V)       xDef_SDL_Charstring(V)
#define yAssF_SDL_Charstring(V,E,A)  xAss_SDL_Charstring(&(V),E,A)
#define yEqF_SDL_Charstring(E1,E2)   xEq_SDL_Charstring(E1,E2)
#define yNEqF_SDL_Charstring(E1,E2)  (! xEq_SDL_Charstring(E1,E2))
#define ySrtN_SDL_Charstring         xSrtN_SDL_Charstring

#define SDL_CHARSTRING_LIT(P1, P2)   P1

#else /* then XSCT_CMICRO */

/*
** Charstrings are complicated to handle them most efficiently
** in the target. There are two basic mechanisms for Cmicro.
** For most SDL conformity, the XRESTUSEOFCHARSTRING
** flag should NOT BE SET. In this case, the solution with
** dynamic memory allocation is selected.
*/
#ifdef XRESTUSEOFCHARSTRING
  /*
  ** Non-Standard Charstring implementation !
  **
  ** Each SDL Charstring in the system is represented in C as
  ** a buffer with the following upper limit : XMK_MAX_SDL_CHARSTRING.
  ** That is good for ROM memory optimization, but not very good
  ** for RAM optimization. If you prefer RAM optimization, then you
  ** have to use alternative 2 (XMK_USE_SDL_CHARSTRING).
  ** The user must be aware of the case of buffer overflow.
  ** Especially such cases can occur when the environment sends
  ** Charstrings into the SDL system, when the Environment
  ** doesn't care about the upper limit.
  */
  typedef char  SDL_Charstring [XMK_MAX_SDL_CHARSTRING] ;
  typedef char  SDL_charstring [XMK_MAX_SDL_CHARSTRING] ;

  #ifdef XMK_USE_KERNEL_INIT
    /* Cmicro Kernel does initialization */
    #define xDef_SDL_Charstring(V)
  #else
    #define xDef_SDL_Charstring(V)     (*V)[0] = '\0';
  #endif

  #define yDef_SDL_Charstring(V)       xDef_SDL_Charstring(V)
  #define yAssF_SDL_Charstring(V,E,A)  (void)strcpy (V,E) ;
  #define yEqF_SDL_Charstring(E1,E2)   (!strcmp (E1,E2))
  #define yNEqF_SDL_Charstring(E1,E2)  (strcmp (E1,E2))
  #define ySrtN_SDL_Charstring         xSrtN_SDL_Charstring_NotImplemented
  #define xLength_SDL_Charstring(STR)  strlen (STR)
  #define SDL_CHARSTRING_LIT(P1, P2)   P2

  /*
  ** Not implemented Charstring operations ....
  ** These would require dynamic memory management
  */
  #define xMkString_SDL_Charstring(STR)  MkString_SDL_Charstring_NotImplemented
  #define xMod_SDL_Charstring(STR)       Mod_SDL_Charstring_NotImplemented
  #define xSubString_SDL_Charstring(STR) SubString_SDL_Charstring_NotImplemented
  #define xConcat_SDL_Charstring(STR)    Concat_SDL_Charstring_NotImplemented

  #ifndef XMK_STRINGS_IN_WRITE_MEM
    /*
    ** Caution :
    ** ===========
    ** Charstrings only can be handled in the same physical address range
    */
    #define yAddr_SDL_Charstring       Addr_SDL_Charstring_NotImplemented
  #endif /* ... XMK_STRINGS_IN_WRITE_MEM */


#else /* then ! XRESTUSEOFCHARSTRING */
  /* 
  ** Standard Charstring Implementation !
  **
  ** Implementation of charstrings as most conform to SDL.
  ** This is the master library implementation for SDL charstrings.
  ** Each Charstring is represented by a pointer. Memory
  ** is allocated dynamically.
  ** This methodology should be used, when SDL Charstrings
  ** are used frequently on SDL level and where the upper limits
  ** of Charstrings cannot be calculated by the user. This is
  ** the implementation with the most conformity to SDL.
  ** On the other hand, it requires full dynamic memory management
  ** on target. Also, a lot of function calls are generated
  ** which results in more executable code.
  ** Also take care of the defines XFREEFUNCS and XFREEVARS, which are
  ** used in generated code to free a previously allocated
  ** memory block with a SDL charstring, after it is not
  ** longer wanted.
  */
  typedef char   *SDL_Charstring;
  typedef char   *SDL_charstring;
 
  /*
  ** The following is the older definition of xDef_SDL_Charstring
  ** which has lead to the problem of free on uninitialized
  ** charstring variable. Do not use this !
  ** #define xDef_SDL_Charstring(V) \
  **         xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)
  */
  #define xDef_SDL_Charstring(V)       *(V)=(SDL_Charstring)0;
  #define yDef_SDL_Charstring(V)       xDef_SDL_Charstring(V)
 
  #if defined(XMK_USE_GC)
    #define yAssF_SDL_Charstring(V,E,ASG) xAss_SDL_Charstring(&(V),E,(ASG|xmk_NotInProcess))
  #else
    #define yAssF_SDL_Charstring(V,E,ASG) xAss_SDL_Charstring(&(V),E,ASG)
  #endif
   
  #define yEqF_SDL_Charstring(E1,E2)   xEq_SDL_Charstring(E1,E2)
  #define yNEqF_SDL_Charstring(E1,E2)  (! xEq_SDL_Charstring(E1,E2))
  #define ySrtN_SDL_Charstring         xSrtN_SDL_Charstring
 
  #define SDL_CHARSTRING_LIT(P1, P2)   P1
 
#endif /* ... XRESTUSEOFCHARSTRING */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#else

  /* 
  ** Dummy definition if charstrings are not used, but ASN.1 types 
  */
  typedef char   *SDL_Charstring;
  typedef char   *SDL_charstring;
#endif
   /* XNOUSEOFCHARSTRING */


/*---+---------------------------------------------------------------
     Duration
-------------------------------------------------------------------*/

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

typedef struct { 
    xint32  s, ns;
  }  SDL_Duration;
typedef SDL_Duration  SDL_duration;
 
#define xDef_SDL_Duration(V)       *(V) = SDL_Duration_Lit(0, 0)   
#define yDef_SDL_Duration(V)       xDef_SDL_Duration(V)
#define yAssF_SDL_Duration(V,E,A)  (V = E)
#define yEqF_SDL_Duration(E1,E2)   xEq_SDL_Duration(E1,E2)
#define yNEqF_SDL_Duration(E1,E2)  (! xEq_SDL_Duration(E1,E2))
#define ySrtN_SDL_Duration         xSrtN_SDL_Duration
 
#define SDL_DURATION_LIT(R,I,D)    SDL_Duration_Lit((xint32)(I),(xint32)(D))
extern SDL_Duration SDL_Duration_Lit  XPP((
  xint32 s,
  xint32 ns ));
 
#else /* then XSCT_CMICRO */


#ifndef XMK_USE_STANDARD_DURATION
  /*
  ** This is the central point, where the representation of
  ** time in the Cmicro Library is fixed up.
  **
  ** Modifications in the following typedef cause changes
  ** in all defines and functions which are explicitely or
  ** implicitely using SDL_Duration and the types depending
  ** on it.
  **
  ** Time is represented as "long" in C.
  ** The resolution is normally then 2^32.
  ** For one tick = 10 msec, the maximum time for the execution of
  ** the SDL System, before the value overruns, is then about 490 day`s.
  */
  typedef long SDL_Duration;

  /*
  ** Internal typedef
  */
  typedef SDL_Duration  SDL_duration;

  #ifdef XMK_USE_KERNEL_INIT
    /*
    ** Cmicro Kernel does initialization ...
    */
    #define xDef_SDL_Duration(V)
  #else
    #define xDef_SDL_Duration(V)       *(V) = 0 ;
  #endif

  #define yDef_SDL_Duration(V)       xDef_SDL_Duration(V)
  #define yAssF_SDL_Duration(V,E,A)  V = E
  #define yEqF_SDL_Duration(E1,E2)   xEq_SDL_Duration(E1,E2)
  #define yNEqF_SDL_Duration(E1,E2)  (! xEq_SDL_Duration(E1,E2))
  #define ySrtN_SDL_Duration         xSrtN_SDL_Duration

  #define SDL_DURATION_LIT(R,I,D)    I

#else /* then XMK_USE_STANDARD_DURATION */
  /* 
  ** This is SDT standard implementation
  ** which cannot be used without implementing
  ** new timer model in Cmicro, so its not useful.
  ** It is contained to ease some integrations.
  */
  typedef struct {
      xint32  s, ns;
    }  SDL_Duration;

  typedef SDL_Duration  SDL_duration;

  #define xDef_SDL_Duration(V)       *(V) = SDL_Duration_Lit(0, 0)
  #define yDef_SDL_Duration(V)       xDef_SDL_Duration(V)
  #define yAssF_SDL_Duration(V,E,A)  V = E
  #define yEqF_SDL_Duration(E1,E2)   xEq_SDL_Duration(E1,E2)
  #define yNEqF_SDL_Duration(E1,E2)  (! xEq_SDL_Duration(E1,E2))
  #define ySrtN_SDL_Duration         xSrtN_SDL_Duration

  #define SDL_DURATION_LIT(R,I,D)    SDL_Duration_Lit(I,D)

#endif /* ... XMK_USE_STANDARD_DURATION */
 
#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */


/*---+---------------------------------------------------------------
     IA5String
-------------------------------------------------------------------*/

typedef SDL_Charstring  SDL_IA5String;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_IA5String(V) \
   xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)

#else /* then XSCT_CMICRO */
  /*
  ** The following is the older definition of xDef_SDL_IA5String
  ** which has lead to the problem of free on uninitialized
  ** charstring variable. Do not use this !
  ** #define xDef_SDL_IA5String(V) \
  **   xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)
  */
#define xDef_SDL_IA5String(V)       *(V)=(SDL_IA5String)0;

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_IA5String(V)       xDef_SDL_IA5String(V)
#define yAssF_SDL_IA5String(V,E,A)  xAss_SDL_Charstring(&(V),E,A)
#define yEqF_SDL_IA5String(E1,E2)   xEq_SDL_Charstring(E1,E2)
#define yNEqF_SDL_IA5String(E1,E2)  (! xEq_SDL_Charstring(E1,E2))
#define ySrtN_SDL_IA5String         xSrtN_SDL_IA5String


/*---+---------------------------------------------------------------
     NumericString
-------------------------------------------------------------------*/

typedef SDL_Charstring  SDL_NumericString;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) 

#define xDef_SDL_NumericString(V) \
   xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)

#else /* then XSCT_CMICRO */
  /*
  ** The following is the older definition of xDef_SDL_NumericString
  ** which has lead to the problem of free on uninitialized
  ** charstring variable. Do not use this !
  ** #define xDef_SDL_NumericString(V) \
  **   xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)
  */
#define xDef_SDL_NumericString(V)       *(V)=(SDL_NumericString)0;
 
#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */ 

#define yDef_SDL_NumericString(V)       xDef_SDL_NumericString(V)
#define yAssF_SDL_NumericString(V,E,A)  xAss_SDL_Charstring(&(V),E,A)
#define yEqF_SDL_NumericString(E1,E2)   xEq_SDL_Charstring(E1,E2)
#define yNEqF_SDL_NumericString(E1,E2)  (! xEq_SDL_Charstring(E1,E2))
#define ySrtN_SDL_NumericString         xSrtN_SDL_NumericString


/*---+---------------------------------------------------------------
     PrintableString
-------------------------------------------------------------------*/

typedef SDL_Charstring  SDL_PrintableString;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) 

#define xDef_SDL_PrintableString(V) \
   xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)

#else /* then XSCT_CMICRO */
  /*
  ** The following is the older definition of xDef_SDL_PrintableString
  ** which has lead to the problem of free on uninitialized
  ** charstring variable. Do not use this !
  ** #define xDef_SDL_PrintableString(V) \
  **    xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)
  */
#define xDef_SDL_PrintableString(V)       *(V)=(SDL_Charstring)0;
 
#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */ 

#define yDef_SDL_PrintableString(V)       xDef_SDL_PrintableString(V)
#define yAssF_SDL_PrintableString(V,E,A)  xAss_SDL_Charstring(&(V),E,A)
#define yEqF_SDL_PrintableString(E1,E2)   xEq_SDL_Charstring(E1,E2)
#define yNEqF_SDL_PrintableString(E1,E2)  (! xEq_SDL_Charstring(E1,E2))
#define ySrtN_SDL_PrintableString         xSrtN_SDL_PrintableString


/*---+---------------------------------------------------------------
     VisibleString
-------------------------------------------------------------------*/

typedef SDL_Charstring  SDL_VisibleString;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)  

#define xDef_SDL_VisibleString(V) \
   xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)

#else /* then XSCT_CMICRO */
  /*
  ** The following is the older definition of xDef_SDL_VisibleString
  ** which has lead to the problem of free on uninitialized
  ** charstring variable. Do not use this !
  ** #define xDef_SDL_VisibleString(V) \
  **    xAss_SDL_Charstring(V, (SDL_Charstring)0, XASS)
  */
#define xDef_SDL_VisibleString(V)       *(V)=(SDL_Charstring)0;

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */  

#define yDef_SDL_VisibleString(V)       xDef_SDL_VisibleString(V)
#define yAssF_SDL_VisibleString(V,E,A)  xAss_SDL_Charstring(&(V),E,A)
#define yEqF_SDL_VisibleString(E1,E2)   xEq_SDL_Charstring(E1,E2)
#define yNEqF_SDL_VisibleString(E1,E2)  (! xEq_SDL_Charstring(E1,E2))
#define ySrtN_SDL_VisibleString         xSrtN_SDL_VisibleString


/*---+---------------------------------------------------------------
     Integer
-------------------------------------------------------------------*/

#if !defined(X_LONG_INT)
typedef int  SDL_Integer;
typedef int  SDL_integer;
#else
typedef long  SDL_Integer;
typedef long  SDL_integer;
#endif

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
 
#define xDef_SDL_Integer(V)       *(V) = (SDL_Integer)0
 
#ifdef XVALIDATOR_LIB
#define xMax_SDL_Integer 2147483647
#endif

#else /* then XSCT_CMICRO */ 
 
#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
#define xDef_SDL_Integer(V)
#else
#define xDef_SDL_Integer(V)       *(V) = (SDL_Integer)0
#endif
 
#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_Integer(V)       xDef_SDL_Integer(V)
#define yAssF_SDL_Integer(V,E,A)  (V = E)
#define yEqF_SDL_Integer(E1,E2)   ((E1) == (E2))
#define yNEqF_SDL_Integer(E1,E2)  ((E1) != (E2))
#define ySrtN_SDL_Integer         xSrtN_SDL_Integer

#define SDL_INTEGER_LIT(I)        I
/* Literals as in C */

#ifdef XVALIDATOR_LIB
#define xMax_SDL_Integer 2147483647
#endif


/*---+---------------------------------------------------------------
     Natural
-------------------------------------------------------------------*/

/*  Modified by GBU,990617 */
/*  natural is >= 0, i.e. unsigned int/long. */
/*
#if !defined(X_LONG_INT)
typedef int  SDL_Natural;
typedef int  SDL_natural;
#else
typedef long  SDL_Natural;
typedef long  SDL_natural;
#endif
*/
typedef UINT32  SDL_Natural;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)
 
#define xDef_SDL_Natural(V)       *(V) = (SDL_Natural)0

#else /* then XSCT_CMICRO */ 
 
#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
  #define xDef_SDL_Natural(V)
#else
  #define xDef_SDL_Natural(V)       *(V) = (SDL_Natural)0
#endif

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_Natural(V)       xDef_SDL_Natural(V)
#define yAssF_SDL_Natural(V,E,A)  (V = E)
#define yEqF_SDL_Natural(E1,E2)   ((E1) == (E2))
#define yNEqF_SDL_Natural(E1,E2)  ((E1) != (E2))
#define ySrtN_SDL_Natural         xSrtN_SDL_Natural


/*---+---------------------------------------------------------------
     Null
-------------------------------------------------------------------*/

typedef int     SDL_Null;

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_Null(V)       *(V) = (SDL_Null)0

#else /* then XSCT_CMICRO */

#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
#define xDef_SDL_Null(V)
#else
#define xDef_SDL_Null(V)       *(V) = (SDL_Null)0
#endif   

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_Null(V)       xDef_SDL_Null(V)
#define yAssF_SDL_Null(V,E,A)  (V = E)
#define yEqF_SDL_Null(E1,E2)   ((E1) == (E2))
#define yNEqF_SDL_Null(E1,E2)  ((E1) != (E2))
#define ySrtN_SDL_Null         xSrtN_SDL_Null

#define SDL_NullValue  0


/*---+---------------------------------------------------------------
     Object_Identifier
-------------------------------------------------------------------*/

/* see sctpredg.h */


/*---+---------------------------------------------------------------
     PId
-------------------------------------------------------------------*/

#define ySrtN_SDL_PId             xSrtN_SDL_PId
/* see sctsdl.c and scttypes.h for implementation of PId */


#ifndef XNOUSEOFREAL
/*---+---------------------------------------------------------------
     Real
-------------------------------------------------------------------*/

#if !defined(X_SHORT_REAL)
typedef double   SDL_Real;
typedef double   SDL_real;
#else
typedef float    SDL_Real;
typedef float    SDL_real;
#endif

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_Real(V)       *(V) = 0.0

#else /* then XSCT_CMICRO */

#ifdef XMK_USE_KERNEL_INIT
  /* Cmicro Kernel does initialization */
#define xDef_SDL_Real(V)
#else
#define xDef_SDL_Real(V)       *(V) = 0.0
#endif

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#define yDef_SDL_Real(V)       xDef_SDL_Real(V)
#define yAssF_SDL_Real(V,E,A)  (V = E)
#define yEqF_SDL_Real(E1,E2)   ((E1) == (E2))
#define yNEqF_SDL_Real(E1,E2)  ((E1) != (E2))
#define ySrtN_SDL_Real         xSrtN_SDL_Real

#define SDL_REAL_LIT(R,I,D)    R
/* Literals as in C */

#ifndef PLUS_INFINITY
#define PLUS_INFINITY 1.79769313486231570e+308
#endif
/* #include <values.h> */
/* #define PLUS_INFINITY MAXDOUBLE */
/* #define MINUS_INFINITY -MAXDOUBLE */

#ifndef MINUS_INFINITY
#define MINUS_INFINITY -PLUS_INFINITY
#endif

#endif /* ... XNOUSEOFREAL */


/*---+---------------------------------------------------------------
     Time
-------------------------------------------------------------------*/

typedef SDL_Duration  SDL_Time;
typedef SDL_Time  SDL_time;
 
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#define xDef_SDL_Time(V)       *(V) = SDL_Time_Lit(0, 0)
 
#define yDef_SDL_Time(V)       xDef_SDL_Time(V)
#define yAssF_SDL_Time(V,E,A)  (V = E)
#define yEqF_SDL_Time(E1,E2)   xEq_SDL_Time(E1,E2)
#define yNEqF_SDL_Time(E1,E2)  (! xEq_SDL_Time(E1,E2))
#define ySrtN_SDL_Time         xSrtN_SDL_Time

#define SDL_TIME_LIT(R,I,D)    SDL_Duration_Lit((xint32)(I),(xint32)(D))
#define SDL_Time_Lit(I,D)      SDL_Duration_Lit(I,D)

#ifdef XVALIDATOR_LIB
extern SDL_Time xMaxTime;
#endif
 
#else /* then XSCT_CMICRO */ 

#ifndef XMK_USE_STANDARD_TIME
 
  #ifdef XMK_USE_KERNEL_INIT
    /*
    ** Cmicro Kernel does initialization ...
    */
    #define xDef_SDL_Time(V)
  #else
    #define xDef_SDL_Time(V)       *(V) = 0
  #endif

  #define yDef_SDL_Time(V)       xDef_SDL_Time(V)
  #define yAssF_SDL_Time(V,E,A)  V = E
  #define yEqF_SDL_Time(E1,E2)   xEq_SDL_Time(E1,E2)
  #define yNEqF_SDL_Time(E1,E2)  (! xEq_SDL_Time(E1,E2))
  #define ySrtN_SDL_Time         xSrtN_SDL_Time

  #define SDL_TIME_LIT(R,I,D)    I
  #define SDL_Time_Lit(I,D)      I

  #define xGT_SDL_Time(P1, P2)  (P1 > P2)
  #define xGE_SDL_Time(P1, P2)  (P1 >= P2)
  #define xLT_SDL_Time(P1, P2)  (P1 < P2)
  #define xLE_SDL_Time(P1, P2)  (P1 <= P2)
  #define xEq_SDL_Time(P1, P2)  (P1 == P2)

#else
  /* 
  ** This is SDT standard implementation
  ** which cannot be used without implementing
  ** new timer model in Cmicro, so its not useful.
  ** It is contained to ease some integrations.
  */
  #define xDef_SDL_Time(V)       *(V) = SDL_Time_Lit(0, 0)
  #define yDef_SDL_Time(V)       xDef_SDL_Time(V)
  #define yAssF_SDL_Time(V,E,A)  V = E
  #define yEqF_SDL_Time(E1,E2)   xEq_SDL_Time(E1,E2)
  #define yNEqF_SDL_Time(E1,E2)  (! xEq_SDL_Time(E1,E2))
  #define ySrtN_SDL_Time         xSrtN_SDL_Time
 
  #define SDL_TIME_LIT(R,I,D)    SDL_Duration_Lit(I,D)
  #define SDL_Time_Lit(I,D)      SDL_Duration_Lit(I,D)
#endif

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */
 

/****+***************************************************************
02   Prototypes and macros for the operators of the data types
********************************************************************/

/*---+---------------------------------------------------------------
     yInit_Predefined
-------------------------------------------------------------------*/
#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#ifndef XOPTSORT
extern void yInit_Predefined XPP((void));
#endif

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */
 


/*---+---------------------------------------------------------------
     Boolean
-------------------------------------------------------------------*/

#define xNot_SDL_Boolean(B)       (!(B))
#define xAnd_SDL_Boolean(B1,B2)   ((B1)&&(B2))
#define xOr_SDL_Boolean(B1,B2)    ((B1)||(B2))
#define xXor_SDL_Boolean(B1,B2)   ((B1)!=(B2))
#define xImpl_SDL_Boolean(B1,B2)  ((B1)<=(B2))
#define ANY_SDL_Boolean           (SDL_Boolean)(GETINTRAND % 2)

/*---+---------------------------------------------------------------
     Bit
-------------------------------------------------------------------*/

#define xNot_SDL_Bit(B)       (!(B))
#define xAnd_SDL_Bit(B1,B2)   ((B1)&(B2))
#define xOr_SDL_Bit(B1,B2)    ((B1)|(B2))
#define xXor_SDL_Bit(B1,B2)   ((B1)^(B2))
#define xImpl_SDL_Bit(B1,B2)  ((B1)<=(B2))
#define ANY_SDL_Bit           (SDL_Bit)(GETINTRAND % 2)

/*---+---------------------------------------------------------------
     Bit_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

/*-- yFree_SDL_Bit_String -----------------------------------------*/
#define yFree_SDL_Bit_String(P)  xFree_SDL_Bit_String(P)
/* function in sctos.c */

/*-- xAss_SDL_Bit_String ------------------------------------------*/
extern void xAss_SDL_Bit_String  XPP((
  SDL_Bit_String *,
  SDL_Bit_String,
  int ));

/*-- xEq_SDL_Bit_String -------------------------------------------*/
extern SDL_Boolean xEq_SDL_Bit_String  XPP((
  SDL_Bit_String,
  SDL_Bit_String ));

/*-- yAddr_SDL_Bit_String -----------------------------------------*/
extern SDL_Bit * yAddr_SDL_Bit_String XPP((
  SDL_Bit_String *,
  SDL_Integer ));

/*-- xExtr_SDL_Bit_String -----------------------------------------*/
extern SDL_Bit xExtr_SDL_Bit_String XPP((
  SDL_Bit_String,
  SDL_Integer ));

/*-- xNot_SDL_Bit_String ------------------------------------------*/
extern SDL_Bit_String xNot_SDL_Bit_String  XPP((
  SDL_Bit_String ));

/*-- xAnd_SDL_Bit_String ------------------------------------------*/
extern SDL_Bit_String xAnd_SDL_Bit_String  XPP((
  SDL_Bit_String,
  SDL_Bit_String ));

/*-- xOr_SDL_Bit_String -------------------------------------------*/
extern SDL_Bit_String xOr_SDL_Bit_String  XPP((
  SDL_Bit_String,
  SDL_Bit_String ));

/*-- xXor_SDL_Bit_String ------------------------------------------*/
extern SDL_Bit_String xXor_SDL_Bit_String  XPP((
  SDL_Bit_String,
  SDL_Bit_String ));

/*-- xImpl_SDL_Bit_String -----------------------------------------*/
extern SDL_Bit_String xImpl_SDL_Bit_String  XPP((
  SDL_Bit_String,
  SDL_Bit_String ));

/*-- xMkString_SDL_Bit_String -------------------------------------*/
extern SDL_Bit_String xMkString_SDL_Bit_String  XPP((
  SDL_Bit ));

/*-- xLength_SDL_Bit_String ---------------------------------------*/
extern SDL_Integer xLength_SDL_Bit_String  XPP((
  SDL_Bit_String ));

/*-- xFirst_SDL_Bit_String ----------------------------------------*/
extern SDL_Bit xFirst_SDL_Bit_String  XPP((
  SDL_Bit_String ));

/*-- xLast_SDL_Bit_String -----------------------------------------*/
extern SDL_Bit xLast_SDL_Bit_String  XPP((
  SDL_Bit_String ));

/*-- xConcat_SDL_Bit_String ---------------------------------------*/
extern SDL_Bit_String xConcat_SDL_Bit_String  XPP((
  SDL_Bit_String,
  SDL_Bit_String ));

/*-- xSubString_SDL_Bit_String ------------------------------------*/
extern SDL_Bit_String xSubString_SDL_Bit_String  XPP((
  SDL_Bit_String,
  SDL_Integer,
  SDL_Integer ));

/*-- xBitStr_SDL_Bit_String ---------------------------------------*/
extern SDL_Bit_String xBitStr_SDL_Bit_String  XPP((
  SDL_Charstring ));

/*-- xHexStr_SDL_Bit_String ---------------------------------------*/
extern SDL_Bit_String xHexStr_SDL_Bit_String  XPP((
  SDL_Charstring ));

#ifndef ANY_SDL_Bit_String
#define ANY_SDL_Bit_String \
   xConcat_SDL_Bit_String \
     (xMkString_SDL_Bit_String( ((unsigned char)(GETINTRAND%2)) ), \
      xMkString_SDL_Bit_String( ((unsigned char)(GETINTRAND%2)) ) \
     )
#endif

#endif
       /* XNOUSEOFOCTETBITSTRING */

/*---+---------------------------------------------------------------
     Octet
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

/*-- yAddr_SDL_Octet ----------------------------------------------*/
extern SDL_Bit * yAddr_SDL_Octet XPP((
  SDL_Octet *,
  SDL_Integer ));

/*-- xExtr_SDL_Octet ----------------------------------------------*/
extern SDL_Bit xExtr_SDL_Octet XPP((
  SDL_Octet,
  SDL_Integer ));

#define xLT_SDL_Octet(B1,B2)  ((B1)<(B2))
#define xLE_SDL_Octet(B1,B2)  ((B1)<=(B2))
#define xGT_SDL_Octet(B1,B2)  ((B1)>(B2))
#define xGE_SDL_Octet(B1,B2)  ((B1)>=(B2))

#define xNot_SDL_Octet(B)       (SDL_Octet)(~(B))
#define xAnd_SDL_Octet(B1,B2)   ((B1)&(B2))
#define xOr_SDL_Octet(B1,B2)    ((B1)|(B2))
#define xXor_SDL_Octet(B1,B2)   ((B1)^(B2))
#define xImpl_SDL_Octet(B1,B2)  xNot_SDL_Octet(xAnd_SDL_Octet((B1),xNot_SDL_Octet(B2)))
#define xShiftL_SDL_Octet(B,I)  (SDL_Octet)(((B)<<(I)) & 0xFF)
#define xShiftR_SDL_Octet(B,I)  ((B)>>(I))
#define xPlus_SDL_Octet(B1,B2)  (SDL_Octet)(((B1)+(B2)) & 0xFF )
#define xMinus_SDL_Octet(B1,B2) ((B1)-(B2))
#define xMult_SDL_Octet(B1,B2)  (SDL_Octet)(((B1)*(B2)) & 0xFF )
#define xI2O_SDL_Octet(I)       (SDL_Octet)(I)
#define xO2I_SDL_Octet(B)       (SDL_Integer)(B)
#define ANY_SDL_Octet           (SDL_Octet)(GETINTRAND % 256)

#ifndef XEINTDIV
#define xDiv_SDL_Octet(B1,B2)   ((B1)/(B2))
#define xMod_SDL_Octet(B1,B2)   ((B1)%(B2))
#define xRem_SDL_Octet(B1,B2)   ((B1)%(B2))
#else
extern SDL_Octet xDiv_SDL_Octet XPP((SDL_Octet, SDL_Octet));
extern SDL_Octet xMod_SDL_Octet XPP((SDL_Octet, SDL_Octet));
extern SDL_Octet xRem_SDL_Octet XPP((SDL_Octet, SDL_Octet));
#endif

/*-- xBitStr_SDL_Octet ----------------------------------------------*/
extern SDL_Octet xBitStr_SDL_Octet  XPP((
  SDL_Charstring ));

/*-- xHexStr_SDL_Octet ----------------------------------------------*/
extern SDL_Octet xHexStr_SDL_Octet  XPP((
  SDL_Charstring ));

#endif
       /* XNOUSEOFOCTETBITSTRING */

/*---+---------------------------------------------------------------
     Octet_String
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

/*-- yFree_SDL_Octet_String ---------------------------------------*/
#define yFree_SDL_Octet_String(P)  xFree_SDL_Bit_String(P)
#define xFree_SDL_Octet_String(P)  xFree_SDL_Bit_String(P)
/* function in sctos.c */

/*-- xAss_SDL_Octet_String ----------------------------------------*/
#define xAss_SDL_Octet_String(V,E,A)  xAss_SDL_Bit_String(V,E,A)

/*-- xEq_SDL_Octet_String -----------------------------------------*/
#define xEq_SDL_Octet_String(V,E,A)  xEq_SDL_Bit_String(V,E,A)

/*-- yAddr_SDL_Octet_String ---------------------------------------*/
extern SDL_Octet * yAddr_SDL_Octet_String XPP((
  SDL_Octet_String *,
  SDL_Integer ));

/*-- xExtr_SDL_Octet_String ---------------------------------------*/
extern SDL_Octet xExtr_SDL_Octet_String XPP((
  SDL_Octet_String,
  SDL_Integer ));

/*-- xMkString_SDL_Octet_String -----------------------------------*/
#define xMkString_SDL_Octet_String(B)  xMkString_SDL_Bit_String(B)

/*-- xLength_SDL_Octet_String ---------------------------------------*/
#define xLength_SDL_Octet_String(B)  xLength_SDL_Bit_String(B)

/*-- xFirst_SDL_Octet_String ----------------------------------------*/
#define xFirst_SDL_Octet_String(B)  xFirst_SDL_Bit_String(B)

/*-- xLast_SDL_Octet_String -----------------------------------------*/
#define xLast_SDL_Octet_String(B)  xLast_SDL_Bit_String(B)

/*-- xConcat_SDL_Octet_String ---------------------------------------*/
#define xConcat_SDL_Octet_String(B1,B2)  xConcat_SDL_Bit_String(B1,B2)

/*-- xSubString_SDL_Octet_String ------------------------------------*/
#define xSubString_SDL_Octet_String(B1,B2,B3)  xSubString_SDL_Bit_String(B1,B2,B3)

/*-- xBitStr_SDL_Octet_String ---------------------------------------*/
extern SDL_Bit_String xBitStr_SDL_Octet_String  XPP((
  SDL_Charstring ));

/*-- xHexStr_SDL_Octet_String ---------------------------------------*/
extern SDL_Bit_String xHexStr_SDL_Octet_String  XPP((
  SDL_Charstring ));

/*-- xBit_String_SDL_Octet_String -----------------------------------*/
extern SDL_Bit_String xBit_String_SDL_Octet_String  XPP((
  SDL_Octet_String ));

/*-- xOctet_String_SDL_Octet_String ---------------------------------*/
extern SDL_Octet_String xOctet_String_SDL_Octet_String  XPP((
  SDL_Bit_String ));

#ifndef ANY_SDL_Octet_String
#define ANY_SDL_Octet_String \
   xConcat_SDL_Octet_String \
     (xMkString_SDL_Octet_String( ((unsigned char)(GETINTRAND%256)) ), \
      xMkString_SDL_Octet_String( ((unsigned char)(GETINTRAND%256)) ) \
     )
#endif

#endif
       /* XNOUSEOFOCTETBITSTRING */


/*---+---------------------------------------------------------------
     Character
-------------------------------------------------------------------*/

#define xLT_SDL_Character(C1,C2)  ((C1)<(C2))  
#define xLE_SDL_Character(C1,C2)  ((C1)<=(C2))  
#define xGT_SDL_Character(C1,C2)  ((C1)>(C2))  
#define xGE_SDL_Character(C1,C2)  ((C1)>=(C2))  
#define xChr_SDL_Character(I)  ((SDL_Character)xMod_SDL_Integer(I,128))
#define xNum_SDL_Character(C)  ((SDL_Integer)C)
#define ANY_SDL_Character         xChr_SDL_Character(GETINTRAND)


/*---+---------------------------------------------------------------
     Charstring
-------------------------------------------------------------------*/

#ifndef XNOUSEOFCHARSTRING

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

/*-- yFree_SDL_Charstring -----------------------------------------*/
#define yFree_SDL_Charstring(P)  xFree_SDL_Charstring(P)
/* function in sctos.c */

/*-- xAss_SDL_Charstring ------------------------------------------*/
extern void xAss_SDL_Charstring  XPP((
  SDL_Charstring *,
  SDL_Charstring,
  int ));

/*-- xEq_SDL_Charstring -------------------------------------------*/
extern SDL_Boolean xEq_SDL_Charstring  XPP((
  SDL_Charstring,
  SDL_Charstring ));

/*-- xMod_SDL_Charstring ------------------------------------------*/
/* No longer used by generated code */
extern SDL_Charstring xMod_SDL_Charstring XPP((
  SDL_Charstring,
  SDL_Integer,
  SDL_Character ));

/*-- yAddr_SDL_Charstring ------------------------------------------*/
extern SDL_Character * yAddr_SDL_Charstring XPP((
  SDL_Charstring *,
  SDL_Integer ));

/*-- xExtr_SDL_Charstring -----------------------------------------*/
extern SDL_Character xExtr_SDL_Charstring XPP((
  SDL_Charstring,
  SDL_Integer ));

/*-- xMkString_SDL_Charstring -------------------------------------*/
extern SDL_Charstring xMkString_SDL_Charstring XPP((
  SDL_Character ));

/*-- xLength_SDL_Charstring ---------------------------------------*/
extern SDL_Integer xLength_SDL_Charstring XPP((
  SDL_Charstring ));

/*-- xFirst_SDL_Charstring ----------------------------------------*/
extern SDL_Character xFirst_SDL_Charstring XPP((
  SDL_Charstring ));

/*-- xLast_SDL_Charstring -----------------------------------------*/
extern SDL_Character xLast_SDL_Charstring XPP((
  SDL_Charstring ));

/*-- xConcat_SDL_Charstring ---------------------------------------*/
extern SDL_Charstring xConcat_SDL_Charstring XPP((
  SDL_Charstring,
  SDL_Charstring ));

/*-- xSubString_SDL_Charstring ------------------------------------*/
extern SDL_Charstring xSubString_SDL_Charstring XPP((
  SDL_Charstring,
  SDL_Integer,
  SDL_Integer ));

#define ANY_SDL_Charstring \
   xConcat_SDL_Charstring \
     (xMkString_SDL_Charstring( ((SDL_Character)(1+GETINTRAND%127)) ), \
      xMkString_SDL_Charstring( ((SDL_Character)(1+GETINTRAND%127)) ) \
     )


#else /* then XSCT_CMICRO */

#ifdef XRESTUSEOFCHARSTRING

  /*
  ** Restricted use of charstrings - Cmicro only
  */
  #define yFree_SDL_Charstring(P)  
  #define xFree_SDL_Charstring(P)  

  #ifdef  XMK_STRINGS_IN_WRITE_MEM
    /*-- yAddr_SDL_Charstring ------------------------------------------*/
    extern SDL_Character * yAddr_SDL_Charstring XPP((
      SDL_Charstring *C,
      SDL_Integer     Index ));
  #endif /* ... XMK_STRINGS_IN_WRITE_MEM */

  /*-- xFirst_SDL_Charstring ----------------------------------------*/
  extern SDL_Character xFirst_SDL_Charstring XPP((
    SDL_Charstring  C ));

  /*-- xLast_SDL_Charstring -----------------------------------------*/
  extern SDL_Character xLast_SDL_Charstring XPP((
    SDL_Charstring  C ));

#else
  /* then ! XRESTUSEOFCHARSTRING */

  /*-- yFree_SDL_Charstring -----------------------------------------*/
  #define yFree_SDL_Charstring(P)  xFree_SDL_Charstring(P)
  /* function in sctos.c */

  /*-- xAss_SDL_Charstring ------------------------------------------*/
  extern void xAss_SDL_Charstring  XPP((
    SDL_Charstring *CVar,
    SDL_Charstring  CExpr,
    int             AssType ));

  /*-- xEq_SDL_Charstring -------------------------------------------*/
  extern SDL_Boolean xEq_SDL_Charstring  XPP((
    SDL_Charstring C1,
    SDL_Charstring C2 ));

  /*-- xMod_SDL_Charstring ------------------------------------------*/
  /* No longer used by generated code */
  extern SDL_Charstring xMod_SDL_Charstring XPP((
    SDL_Charstring  C,
    SDL_Integer     Index,
    SDL_Character   Value ));

  /*-- yAddr_SDL_Charstring ------------------------------------------*/
  extern SDL_Character * yAddr_SDL_Charstring XPP((
    SDL_Charstring *C,
    SDL_Integer     Index ));

  /*-- xExtr_SDL_Charstring -----------------------------------------*/
  extern SDL_Character xExtr_SDL_Charstring XPP((
    SDL_Charstring  C,
    SDL_Integer     Index ));

  /*-- xMkString_SDL_Charstring -------------------------------------*/
  extern SDL_Charstring xMkString_SDL_Charstring XPP((
    SDL_Character C ));

  /*-- xLength_SDL_Charstring ---------------------------------------*/
  extern SDL_Integer xLength_SDL_Charstring XPP((
    SDL_Charstring  C ));

  /*-- xFirst_SDL_Charstring ----------------------------------------*/
  extern SDL_Character xFirst_SDL_Charstring XPP((
    SDL_Charstring  C ));

  /*-- xLast_SDL_Charstring -----------------------------------------*/
  extern SDL_Character xLast_SDL_Charstring XPP((
    SDL_Charstring  C ));

  /*-- xConcat_SDL_Charstring ---------------------------------------*/
  extern SDL_Charstring xConcat_SDL_Charstring XPP((
    SDL_Charstring  C1,
    SDL_Charstring  C2 ));

  /*-- xSubString_SDL_Charstring ------------------------------------*/
  extern SDL_Charstring xSubString_SDL_Charstring XPP((
    SDL_Charstring  C,
    SDL_Integer     Start,
    SDL_Integer     SubLength ));
#endif /* ... XRESTUSEOFCHARSTRING */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

#endif
   /* XNOUSEOFCHARSTRING */

/*---+---------------------------------------------------------------
     IA5String
-------------------------------------------------------------------*/

#define ANY_SDL_IA5String        ANY_SDL_Charstring


/*---+---------------------------------------------------------------
     NumericString
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

/*-- xTest_SDL_NumericString --------------------------------------*/
#ifdef XTESTF
extern xbool xTest_SDL_NumericString  XPP((
  void * ));
#endif

/*-- xTstA_SDL_NumericString --------------------------------------*/
#ifdef XERANGE
extern SDL_NumericString xTstA_SDL_NumericString XPP(( SDL_NumericString ));
#define yTstA_SDL_NumericString(N)  xTstA_SDL_NumericString(N)
#else
#define xTstA_SDL_NumericString(N)  N
#define yTstA_SDL_NumericString(N)  N
#endif

/*-- xTstI_SDL_NumericString --------------------------------------*/
#ifdef XEINDEX
extern SDL_NumericString xTstI_SDL_NumericString XPP(( SDL_NumericString ));
#define yTstI_SDL_NumericString(N)  xTstI_SDL_NumericString(N)
#else
#define xTstI_SDL_NumericString(N)  N
#define yTstI_SDL_NumericString(N)  N
#endif

#define ANY_SDL_NumericString       ANY_SDL_Charstring

#endif /* ... XNOUSEOFOCTETBITSTRING */

/*---+---------------------------------------------------------------
     PrintableString
-------------------------------------------------------------------*/

#ifndef XNOUSEOFOCTETBITSTRING

/*-- xTest_SDL_PrintableString ------------------------------------*/
#ifdef XTESTF
extern xbool xTest_SDL_PrintableString  XPP((
  void * ));
#endif

/*-- xTstA_SDL_PrintableString ------------------------------------*/
#ifdef XERANGE
extern SDL_PrintableString xTstA_SDL_PrintableString XPP(( SDL_PrintableString ));
#define yTstA_SDL_PrintableString(N)  xTstA_SDL_PrintableString(N)
#else
#define xTstA_SDL_PrintableString(N)  N
#define yTstA_SDL_PrintableString(N)  N
#endif

/*-- xTstI_SDL_PrintableString -----------------------------------*/
#ifdef XEINDEX
extern SDL_PrintableString xTstI_SDL_PrintableString XPP(( SDL_PrintableString ));
#define yTstI_SDL_PrintableString(N)  xTstI_SDL_PrintableString(N)
#else
#define xTstI_SDL_PrintableString(N)  N
#define yTstI_SDL_PrintableString(N)  N
#endif

#define ANY_SDL_PrintableString       ANY_SDL_Charstring

#endif /* ... XNOUSEOFOCTETBITSTRING */

/*---+---------------------------------------------------------------
     VisibleString
-------------------------------------------------------------------*/

/*-- xTest_SDL_VisibleString --------------------------------------*/
#ifdef XTESTF
#define xTest_SDL_VisibleString(N)  xTest_SDL_PrintableString(N)
#endif

/*-- xTstA_SDL_VisibleString --------------------------------------*/
#define xTstA_SDL_VisibleString(N)  xTstA_SDL_PrintableString(N)
#define yTstA_SDL_VisibleString(N)  yTstA_SDL_PrintableString(N)

/*-- xTstI_SDL_VisibleString --------------------------------------*/
#define xTstI_SDL_VisibleString(N)  xTstI_SDL_PrintableString(N)
#define yTstI_SDL_VisibleString(N)  yTstI_SDL_PrintableString(N)

#define ANY_SDL_VisibleString       ANY_SDL_Charstring


/*---+---------------------------------------------------------------
     Duration
-------------------------------------------------------------------*/

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

/*-- xPlus_SDL_Duration -------------------------------------------*/
extern SDL_Duration xPlus_SDL_Duration  XPP((
  SDL_Duration,
  SDL_Duration ));

/*-- xMinus_SDL_Duration ------------------------------------------*/
#define xMinus_SDL_Duration(P1, P2) \
  xPlus_SDL_Duration(P1, xMonMinus_SDL_Duration(P2))

/*-- xMonMinus_SDL_Duration ---------------------------------------*/
extern SDL_Duration xMonMinus_SDL_Duration  XPP((
  SDL_Duration ));

/*-- xMult_SDL_Duration -------------------------------------------*/
#ifndef XNOUSEOFREAL
extern SDL_Duration xMult_SDL_Duration  XPP((
  SDL_Duration,
  SDL_Real ));
#endif

/*-- xMultRD_SDL_Duration -----------------------------------------*/
#define xMultRD_SDL_Duration(R,D) xMult_SDL_Duration(D, R)

/*-- xDiv_SDL_Duration --------------------------------------------*/
#ifndef XNOUSEOFREAL
extern SDL_Duration xDiv_SDL_Duration  XPP((
  SDL_Duration,
  SDL_Real ));
#endif

#define xGT_SDL_Duration(P1, P2)  xLT_SDL_Time(P2, P1)
#define xGE_SDL_Duration(P1, P2)  xLE_SDL_Time(P2, P1)
#define xLT_SDL_Duration(P1, P2)  xLT_SDL_Time(P1, P2)
#define xLE_SDL_Duration(P1, P2)  xLE_SDL_Time(P1, P2)

/*-- xEq_SDL_Duration ---------------------------------------------*/
extern SDL_Boolean xEq_SDL_Duration  XPP((
  SDL_Duration,
  SDL_Duration ));

#define ANY_SDL_Duration \
   SDL_Duration_Lit(GETINTRAND, GETINTRAND % 1000000000)

#else /* then XSCT_CMICRO */

#ifndef XMK_USE_STANDARD_DURATION

  #define xPlus_SDL_Duration(a,b)   ((long) a + (long) b)
  #define xMinus_SDL_Duration(a,b)  ((long) a - (long) b)
  #define xMult_SDL_Duration(a,b)   (a*b)

  #define xGT_SDL_Duration(P1, P2)  (P1 > P2)
  #define xGE_SDL_Duration(P1, P2)  (P1 >= P2)
  #define xLT_SDL_Duration(P1, P2)  (P1 < P2)
  #define xLE_SDL_Duration(P1, P2)  (P1 <= P2)
  #define xEq_SDL_Duration(P1, P2)  (P1 == P2)

  #define xMonMinus_SDL_Duration(a) (-(long)a)              
  #define xMultRD_SDL_Duration(a,b) xMult_SDL_Duration(b,a)
  #define xDiv_SDL_Duration(a,b)    (a/b)                 

#else /* then XMK_USE_STANDARD_DURATION */

  /*-- xPlus_SDL_Duration -------------------------------------------*/
  extern SDL_Duration xPlus_SDL_Duration  XPP((
    SDL_Duration D1,
    SDL_Duration D2 ));

  /*-- xMinus_SDL_Duration ------------------------------------------*/
  #define xMinus_SDL_Duration(P1, P2) \
    xPlus_SDL_Duration(P1, xMonMinus_SDL_Duration(P2))

  /*-- xMonMinus_SDL_Duration ---------------------------------------*/
  extern SDL_Duration xMonMinus_SDL_Duration  XPP((
    SDL_Duration D ));

  /*-- xMult_SDL_Duration -------------------------------------------*/
  #ifndef XNOUSEOFREAL
  extern SDL_Duration xMult_SDL_Duration  XPP((
    SDL_Duration D,
    SDL_Real     R ));
  #endif

  /*-- xMultRD_SDL_Duration -----------------------------------------*/
  #define xMultRD_SDL_Duration(R,D) xMult_SDL_Duration(D, R)

  /*-- xDiv_SDL_Duration --------------------------------------------*/
  #ifndef XNOUSEOFREAL
  extern SDL_Duration xDiv_SDL_Duration  XPP((
    SDL_Duration D,
    SDL_Real     R ));
  #endif

  #define xGT_SDL_Duration(P1, P2)  xLT_SDL_Time(P2, P1)
  #define xGE_SDL_Duration(P1, P2)  xLE_SDL_Time(P2, P1)
  #define xLT_SDL_Duration(P1, P2)  xLT_SDL_Time(P1, P2)
  #define xLE_SDL_Duration(P1, P2)  xLE_SDL_Time(P1, P2)

  /*-- xEq_SDL_Duration ---------------------------------------------*/
  extern SDL_Boolean xEq_SDL_Duration  XPP((
    SDL_Duration D1,
    SDL_Duration D2 ));

#endif /* ... XMK_USE_STANDARD_DURATION */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */

/*---+---------------------------------------------------------------
     Integer
-------------------------------------------------------------------*/

#define xMonMinus_SDL_Integer(I)   (-(I))
#define xPlus_SDL_Integer(I1,I2)   ((I1)+(I2))
#define xMinus_SDL_Integer(I1,I2)  ((I1)-(I2))
#define xMult_SDL_Integer(I1,I2)   ((I1)*(I2))

/*-- xDiv_SDL_Integer ---------------------------------------------*/
#ifdef XEINTDIV
  extern SDL_Integer xDiv_SDL_Integer XPP(( SDL_Integer, SDL_Integer ));
#else
  #define xDiv_SDL_Integer(i,k) ((i)/(k))
#endif

/*-- xMod_SDL_Integer ---------------------------------------------*/
extern SDL_Integer xMod_SDL_Integer XPP((
  SDL_Integer,
  SDL_Integer ));

/*-- xRem_SDL_Integer ---------------------------------------------*/
#ifdef XEINTDIV
extern SDL_Integer xRem_SDL_Integer XPP((
  SDL_Integer,
  SDL_Integer ));
#else
#define xRem_SDL_Integer(i,k) ((i)%(k))
#endif

#define xLT_SDL_Integer(I1,I2)  ((I1)<(I2))
#define xLE_SDL_Integer(I1,I2)  ((I1)<=(I2))
#define xGT_SDL_Integer(I1,I2)  ((I1)>(I2))
#define xGE_SDL_Integer(I1,I2)  ((I1)>=(I2))

/*-- xFix_SDL_Integer ---------------------------------------------*/
#ifndef XNOUSEOFREAL
#ifdef XEFIXOF
extern SDL_Integer xFix_SDL_Integer XPP((
  SDL_Real ));
#else
#define xFix_SDL_Integer(Re)   ((SDL_Integer)Re)
#endif
#endif

/* -- xFloat_SDL_Integer ------------------------------------------*/
#ifndef XNOUSEOFREAL
#define xFloat_SDL_Integer(I)   ((SDL_Real)I)
#endif

#define ANY_SDL_Integer      (SDL_Integer)(GETINTRAND - GETINTRAND_MAX/2)

/*---+---------------------------------------------------------------
     Null
-------------------------------------------------------------------*/

#define ANY_SDL_Null         (SDL_Null)0


/*---+---------------------------------------------------------------
     Natural
-------------------------------------------------------------------*/

/*-- xTest_SDL_Natural --------------------------------------------*/
#ifdef XTESTF
extern xbool xTest_SDL_Natural  XPP((
  void * ));
#endif

/*-- xTstA_SDL_Natural --------------------------------------------*/
#ifdef XERANGE
extern SDL_Natural xTstA_SDL_Natural XPP(( SDL_Natural ));
#define yTstA_SDL_Natural(N)  xTstA_SDL_Natural(N)
#else
#define xTstA_SDL_Natural(N)  N
#define yTstA_SDL_Natural(N)  N
#endif

/*-- xTstI_SDL_Natural --------------------------------------------*/
#ifdef XEINDEX
extern SDL_Natural xTstI_SDL_Natural XPP(( SDL_Natural ));
#define yTstI_SDL_Natural(N)  xTstI_SDL_Natural(N)
#else                                                                                          
#define xTstI_SDL_Natural(N)  N
#define yTstI_SDL_Natural(N)  N
#endif

#define ANY_SDL_Natural         (SDL_Natural)(GETINTRAND)

/*---+---------------------------------------------------------------
     PId
-------------------------------------------------------------------*/

#define ANY_SDL_PId             SDL_NULL


/*---+---------------------------------------------------------------
     Real
-------------------------------------------------------------------*/

#define xMonMinus_SDL_Real(R)   (-(R))
#define xPlus_SDL_Real(R1,R2)   ((R1)+(R2))
#define xMinus_SDL_Real(R1,R2)  ((R1)-(R2))
#define xMult_SDL_Real(R1,R2)   ((R1)*(R2))

/*-- xDiv_SDL_Real ------------------------------------------------*/
#ifndef XNOUSEOFREAL
#ifdef XEREALDIV
extern SDL_Real xDiv_SDL_Real XPP((
  SDL_Real,
  SDL_Real ));
#else
#define xDiv_SDL_Real(i,k) ((i)/(k))
#endif
#endif

#define xLT_SDL_Real(R1,R2)  ((R1)<(R2))
#define xLE_SDL_Real(R1,R2)  ((R1)<=(R2))
#define xGT_SDL_Real(R1,R2)  ((R1)>(R2))
#define xGE_SDL_Real(R1,R2)  ((R1)>=(R2))

#define ANY_SDL_Real         (SDL_Real)(GETINTRAND - GETINTRAND_MAX/2)


/*---+---------------------------------------------------------------
     Time
-------------------------------------------------------------------*/


#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

/*-- xPlus_SDL_Time -----------------------------------------------*/
#define xPlus_SDL_Time(P1, P2)  xPlus_SDL_Duration(P1, P2)

/*-- xPlusDT_SDL_Time ---------------------------------------------*/
#define xPlusDT_SDL_Time(P1, P2)  xPlus_SDL_Duration(P2, P1)

/*-- xMinusD_SDL_Time ---------------------------------------------*/
#define xMinusD_SDL_Time(P1, P2) \
  xPlus_SDL_Duration(P1, xMonMinus_SDL_Duration(P2))

/*-- xMinusT_SDL_Time ---------------------------------------------*/
#define xMinusT_SDL_Time(P1, P2) \
  xPlus_SDL_Duration(P1, xMonMinus_SDL_Duration(P2))

/*-- xEq_SDL_Time -------------------------------------------------*/
#define xEq_SDL_Time(P1, P2)  xEq_SDL_Duration(P1, P2)

/*-- xLT_SDL_Time -------------------------------------------------*/
extern SDL_Boolean xLT_SDL_Time  XPP((
  SDL_Time,
  SDL_Time ));

/*-- xLE_SDL_Time -------------------------------------------------*/
extern SDL_Boolean xLE_SDL_Time  XPP((
  SDL_Time,
  SDL_Time ));

/*-- xGT_SDL_Time -------------------------------------------------*/
#define xGT_SDL_Time(P1, P2)  xLT_SDL_Time(P2, P1)

/*-- xGE_SDL_Time -------------------------------------------------*/
#define xGE_SDL_Time(P1, P2)  xLE_SDL_Time(P2, P1)

#define ANY_SDL_Time \
   SDL_Time_Lit(GETINTRAND, GETINTRAND % 1000000000)

#else /* then XSCT_CMICRO */

#ifndef XMK_USE_STANDARD_TIME

  #define xPlus_SDL_Time(P1, P2)     (P1 + P2)
  #define xPlusDT_SDL_Time(P1, P2)   (P1 + P2)
  #define xMinusD_SDL_Time(P1, P2)   (P1 - P2)
  #define xMinusT_SDL_Time(P1, P2)   (P1 - P2)
  #define xEq_SDL_Time(P1, P2)       (P1 == P2)
  #define xLT_SDL_Time(P1, P2)       (P1 < P2)
  #define xGT_SDL_Time(P1, P2)       (P1 > P2)
  #define xGE_SDL_Time(P1, P2)       (P1 >= P2)

#else /* then XMK_USE_STANDARD_TIME */

  /*-- xPlus_SDL_Time -----------------------------------------------*/
  #define xPlus_SDL_Time(P1, P2)  xPlus_SDL_Duration(P1, P2)

  /*-- xPlusDT_SDL_Time ---------------------------------------------*/
  #define xPlusDT_SDL_Time(P1, P2)  xPlus_SDL_Duration(P2, P1)

  /*-- xMinusD_SDL_Time ---------------------------------------------*/
  #define xMinusD_SDL_Time(P1, P2) \
    xPlus_SDL_Duration(P1, xMonMinus_SDL_Duration(P2))

  /*-- xMinusT_SDL_Time ---------------------------------------------*/
  #define xMinusT_SDL_Time(P1, P2) \
    xPlus_SDL_Duration(P1, xMonMinus_SDL_Duration(P2))

  /*-- xEq_SDL_Time -------------------------------------------------*/
  #define xEq_SDL_Time(P1, P2)  xEq_SDL_Duration(P1, P2)

  /*-- xLT_SDL_Time -------------------------------------------------*/
  extern SDL_Boolean xLT_SDL_Time  XPP((
    SDL_Time T1,
    SDL_Time T2 ));

  /*-- xLE_SDL_Time -------------------------------------------------*/
  extern SDL_Boolean xLE_SDL_Time  XPP((
    SDL_Time T1,
    SDL_Time T2 ));

  /*-- xGT_SDL_Time -------------------------------------------------*/
  #define xGT_SDL_Time(P1, P2)  xLT_SDL_Time(P2, P1)

  /*-- xGE_SDL_Time -------------------------------------------------*/
  #define xGE_SDL_Time(P1, P2)  xLE_SDL_Time(P2, P1)

  #define ANY_SDL_Time ERROR_ANY_SDL_Time_IsNotImplemented

#endif /* ... XMK_USE_STANDARD_TIME */

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */


/*---+---------------------------------------------------------------
     own/oref
-------------------------------------------------------------------*/
extern void * xPassOwn XPP((void **P));


/****+***************************************************************
03   Read and Write functions
********************************************************************/

#if defined(XSCT_CADVANCED) || defined(XSCT_CBASIC)

#ifdef XREADANDWRITEF

/*-- xRead_SDL_Boolean --------------------------------------------*/
#define xRead_SDL_Boolean(Addr)  xReadSort(Addr, xSrtN_SDL_Boolean)

/*-- xWri_SDL_Boolean ---------------------------------------------*/
#define xWri_SDL_Boolean(Value)  xWriteSort(Value, xSrtN_SDL_Boolean)

/*-- xRead_SDL_Bit ------------------------------------------------*/
extern int xRead_SDL_Bit XPP((void *));

/*-- xWri_SDL_Bit -------------------------------------------------*/
extern char * xWri_SDL_Bit XPP((void *));

/*-- xRead_SDL_Bit_String -----------------------------------------*/
extern int xRead_SDL_Bit_String XPP((void *));

/*-- xWri_SDL_Bit_String ------------------------------------------*/
extern char * xWri_SDL_Bit_String XPP((void *));

/*-- xRead_SDL_Octet ----------------------------------------------*/
extern int xRead_SDL_Octet XPP((void *));

/*-- xWri_SDL_Octet -----------------------------------------------*/
extern char * xWri_SDL_Octet XPP((void *));

/*-- xRead_SDL_Octet_String ---------------------------------------*/
extern int xRead_SDL_Octet_String XPP((void *));

/*-- xWri_SDL_Octet_String ----------------------------------------*/
extern char * xWri_SDL_Octet_String XPP((void *));

/*-- xRead_SDL_Character ------------------------------------------*/
extern int xRead_SDL_Character XPP((void *));

/*-- xWri_SDL_Character -------------------------------------------*/
extern char * xWri_SDL_Character XPP((void *));

/*-- xRead_SDL_Charstring -----------------------------------------*/
extern int xRead_SDL_Charstring XPP((void *));

/*-- xWri_SDL_Charstring ------------------------------------------*/
extern char * xWri_SDL_Charstring XPP((void *));

/*-- xRead_SDL_Duration -------------------------------------------*/
extern int xRead_SDL_Duration XPP((void *));

/*-- xWri_SDL_Duration --------------------------------------------*/
extern char * xWri_SDL_Duration XPP((void *));

/*-- xRead_SDL_Integer --------------------------------------------*/
extern int xRead_SDL_Integer XPP((void *));

/*-- xWri_SDL_Integer ---------------------------------------------*/
extern char * xWri_SDL_Integer XPP((void *));

/*-- xRead_SDL_Null -----------------------------------------------*/
extern int xRead_SDL_Null XPP((void *));

/*-- xWri_SDL_Null ------------------------------------------------*/
extern char * xWri_SDL_Null XPP((void *));

/*-- xRead_SDL_Real -----------------------------------------------*/
extern int xRead_SDL_Real XPP((void *));

/*-- xWri_SDL_Real ------------------------------------------------*/
extern char * xWri_SDL_Real XPP((void *));

/*-- xRead_SDL_Time -----------------------------------------------*/
extern int xRead_SDL_Time XPP((void *));

/*-- xWri_SDL_Time ------------------------------------------------*/
extern char * xWri_SDL_Time XPP((void *));



/* Help functions, also used by sctutil.c and sctmon.c */
/*-- xReadSDL_Time ------------------------------------------------*/
extern int xReadSDL_Time XPP((SDL_Time *));

/*-- xWriteSDL_Time -----------------------------------------------*/
extern char * xWriteSDL_Time XPP((SDL_Time, int));

#endif /* ... defined(XSCT_CADVANCED) || defined(XSCT_CBASIC) */ 

#endif
   /* XREADANDWRITEF */


#ifndef XNO_LONG_MACROS
/*
** Generators are in an extra file because
** some C compilers cannot preprocess correctly
*/
#include "sctpredg.h"
#endif

#endif
   /* X_SCTPRED_H */
