/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
/*
 * WAEctype.c
 *
 */

#include "WAEctype.h"

const short int _wae_ctype[256] = {
  _C, _C, _C, _C, _C, _C, _C, _C,
  _C,
  _C | _WS | _B | _T, /* HT */
  _C | _WS, /* LF */
  _C | _WS, /* VT */
  _C,
  _C | _WS, /* CR */
  _C, _C,
  _C, _C, _C, _C, _C, _C, _C, _C,
  _C, _C, _C, _C, _C, _C, _C, _C,
  _SP | _WS | _B | _T, /* space */
  _M, /* '!' */
  _DL | _T, /* '"' */
  _DL, /* '#' */
  _SRES, /* '$' */
  _ESC | _DL, /* '%' */
  _SRES, /* '&' */
  _M, /* '\'' */
  _M | _T, /* '(' */
  _M | _T, /* ')' */
  _M, /* '*' */
  _SRES, /* '+' */
  _SRES | _T, /* ',' */
  _M, /* '-' */
  _M, /* '.' */
  _ARES | _T, /* '/' */
  _D, /* '0' */
  _D, /* '1' */
  _D, /* '2' */
  _D, /* '3' */
  _D, /* '4' */
  _D, /* '5' */
  _D, /* '6' */
  _D, /* '7' */
  _D, /* '8' */
  _D, /* '9' */
  _SRES | _T, /* ':' */
  _ARES | _T, /* ';' */
  _DL | _T, /* '<' */
  _SRES | _T, /* '=' */
  _DL | _T, /* '>' */
  _ARES | _T, /* '?' */
  _SRES | _T, /* '@' */
  _UC | _X, /* 'A' */
  _UC | _X, /* 'B' */
  _UC | _X, /* 'C' */
  _UC | _X, /* 'D' */
  _UC | _X, /* 'E' */
  _UC | _X, /* 'F' */
  _UC, /* 'G' */
  _UC, /* 'H' */
  _UC, /* 'I' */
  _UC, /* 'J' */
  _UC, /* 'K' */
  _UC, /* 'L' */
  _UC, /* 'M' */
  _UC, /* 'N' */
  _UC, /* 'O' */
  _UC, /* 'P' */
  _UC, /* 'Q' */
  _UC, /* 'R' */
  _UC, /* 'S' */
  _UC, /* 'T' */
  _UC, /* 'U' */
  _UC, /* 'V' */
  _UC, /* 'W' */
  _UC, /* 'X' */
  _UC, /* 'Y' */
  _UC, /* 'Z' */
  _UW | _T, /* '[' */
  _UW | _T, /* '\\' */
  _UW | _T, /* ']' */
  _UW, /* '^' */
  _M, /* '_' */
  _UW, /* '`' */
  _LC | _X, /* 'a' */
  _LC | _X, /* 'b' */
  _LC | _X, /* 'c' */
  _LC | _X, /* 'd' */
  _LC | _X, /* 'e' */
  _LC | _X, /* 'f' */
  _LC, /* 'g' */
  _LC, /* 'h' */
  _LC, /* 'i' */
  _LC, /* 'j' */
  _LC, /* 'k' */
  _LC, /* 'l' */
  _LC, /* 'm' */
  _LC, /* 'n' */
  _LC, /* 'o' */
  _LC, /* 'p' */
  _LC, /* 'q' */
  _LC, /* 'r' */
  _LC, /* 's' */
  _LC, /* 't' */
  _LC, /* 'u' */
  _LC, /* 'v' */
  _LC, /* 'w' */
  _LC, /* 'x' */
  _LC, /* 'y' */
  _LC, /* 'z' */
  _UW | _T, /* '{' */
  _UW, /* '|' */
  _UW | _T, /* '}' */
  _M, /* '~' */
  _C,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

