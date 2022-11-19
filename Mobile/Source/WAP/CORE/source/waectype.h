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
 * WAEctype.h
 *
 * Character type classification with fast lookup functions.
 *
 */
#ifndef _WAEctype_h
#define _WAEctype_h

extern const short int _wae_ctype[256];

#define _UC    0x1            /* Lower case, a-z */
#define _LC    0x2            /* Upper case, A-Z */
#define _D     0x4            /* Digit, 0-9 */
#define _X     0x8            /* Hex digit, a-f, A-F */
#define _M     0x10           /* Mark, -_.!~*'() */
#define _SRES  0x20           /* Sometimes reserved, :@&=+$, */
#define _ARES  0x40           /* Always reserved, ;/? */
#define _DL    0x80           /* Delimiters, <>#%" */
#define _UW    0x100          /* Unwise, {}|\^[]` */
#define _WS    0x200          /* Whitespace, HT, VT, CR, LF, SP */
#define _SP    0x400          /* Space, ' ' */
#define _B     0x800          /* Blank, SP TAB */
#define _C     0x1000         /* Ctlr character, 0x0-0x1f, 0xff */
#define _T     0x2000         /* Tspecials, ()<>@,;:\"/[]?={}SPHT */
#define _ESC   0x4000         /* Escape character, % */

#define wae_isascii(c)      ((c) < 0x80)
#define wae_isalpha(c)      (_wae_ctype[(c)] & (_UC | _LC))
#define wae_isalphanum(c)   (_wae_ctype[(c)] & (_UC | _LC | _D))
#define wae_islower(c)      (_wae_ctype[(c)] & _LC)
#define wae_isupper(c)      (_wae_ctype[(c)] & _UC)
#define wae_isdigit(c)      (_wae_ctype[(c)] & _D)
#define wae_ishex(c)        (_wae_ctype[(c)] & (_D | _X))
#define wae_ishexletter(c)  (_wae_ctype[(c)] & _X)
#define wae_isblank(c)      (_wae_ctype[(c)] & (_B))
#define wae_iswhitespace(c) (_wae_ctype[(c)] & _WS)
#define wae_isctrl(c)       (_wae_ctype[(c)] & _C)

#define wae_isescape(c)     (_wae_ctype[(c)] & _ESC)
#define wae_isreserved(c)   (_wae_ctype[(c)] & (_SRES | _ARES))
#define wae_isexcluded(c)   (_wae_ctype[(c)] & (_C | _SP | _DL))
#define wae_isdelimiter(c)  (_wae_ctype[(c)] & _DL)
#define wae_isunwise(c)     (_wae_ctype[(c)] & _UW)
#define wae_istspecial(c)   (_wae_ctype[(c)] & _T)
#define wae_isspecial(c)    (_wae_ctype[(c)] & (_C | _SP | _SRES | _ARES | _UW | _DL))
#define wae_ispchar(c)      (_wae_ctype[(c)] & (_UC | _LC | _D | _M | _SRES))
#define wae_isuric(c)       (_wae_ctype[(c)] & (_SRES | _ARES | _UC | _LC | _D | _M))

#endif

