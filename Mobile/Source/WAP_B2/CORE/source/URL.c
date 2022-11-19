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
 * URL.c
 *
 * Library of routines for handling URLs.
 *
 * Created by Anders Edenbrandt, Mon Mar 29 08:21:14 1999.
 *
 * Revision history:
 *   990818, AED: Complete rewrite of practically everything.
 *   991123, AED: added new function, b_IsPrefix.
 *   991123, AED: parsing all URIs as generic
 *
 */

#include "CmmnRsrc.h"
#include "WMLDef.h"
#include "WAEctype.h"
#include "URL.h"


/****************************
 * Private utility routines:
 ****************************/

/*
 * Return a copy of a given string.
 * NOTE: it is the caller's responsibility to deallocated the returned
 * string.
 */
static BYTE *
newstring (const BYTE *s, UINT16 len)
{
  BYTE *pnew = NULL;

  if ((s != NULL) && ((pnew = NEWARRAY (BYTE, len + 1)) != NULL)) {
    B_COPYSTRINGN (pnew, s, len);
    pnew[len] = '\0';
  }

  return pnew;
}

/*
 * Return the value of the unsigned decimal integer stored
 * in the first "len" bytes of the string "bs".
 */
static UINT16
GetNum (const BYTE *bs, UINT16 len)
{
  UINT16 n = 0;
  INT16  i;

  for (i = 0; i < len; i++)
    n = n * 10 + (bs[i] - '0');

  return n;
}

/*
 * String comparison routine that returns TRUE if the strings are equal.
 * If 'noCase' is TRUE, the case of letters is insignificant.
 * Hex escape sequences of the form '%XY' are handled correctly,
 * that is, such a sequence is compared as if the character had first
 * been unescaped, unless it is a character in the "reserved" set.
 * The routine accepts NULL pointers as input, in which case the
 * result is TRUE if both string pointers are NULL.
*/
static BOOL
CompareStrings (BYTE *bs1, UINT16 len1, BYTE *bs2, UINT16 len2, BOOL noCase)
{
  BYTE b1, b2, tmp;
  BOOL reservedEscape1, reservedEscape2;

  if (bs1 == NULL)
    return bs2 == NULL;
  else if (bs2 == NULL)
    return FALSE;

  for (;(len1 > 0) && (len2 > 0); len1--, len2--) {
    reservedEscape1 = reservedEscape2 = FALSE;

    b1 = *bs1++;
    if ((b1 == '%') && (len1 >= 2) && HexToByte (bs1, &tmp)) {
      bs1 += 2;
      len1 -= 2;
      b1 = tmp;
      if (wae_isreserved (tmp) || (tmp == '#')) {
        reservedEscape1 = TRUE;
      }
    }

    b2 = *bs2++;
    if ((b2 == '%') && (len2 >= 2) && HexToByte (bs2, &tmp)) {
      bs2 += 2;
      len2 -= 2;
      b2 = tmp;
      if (wae_isreserved (tmp) || (tmp == '#')) {
        reservedEscape2 = TRUE;
      }
    }

    if (noCase) {
      b1 = DowncaseByte (b1);
      b2 = DowncaseByte (b2);
    }

    if ((b1 != b2) || (reservedEscape1 ^ reservedEscape2))
      return FALSE;
  }

  return (len1 == len2);
}

/************************************************************
 * Public routines:
 ************************************************************/

static struct {
  const Scheme sch;
  const BYTE   *str;
  const UINT8  slen;
} schemes[] = {
  {Scheme_empty, (const BYTE *)"", 0},
  {Scheme_http, (const BYTE *)"http", 4},
  {Scheme_https, (const BYTE *)"https", 5},
  {Scheme_file, (const BYTE *)"file", 4},
  {Scheme_wtai, (const BYTE *)"wtai", 4},
  {Scheme_about, (const BYTE *)"about", 5},
  {Scheme_function, (const BYTE *)"function", 8},
  {Scheme_wapdevice, (const BYTE *)"wapdevice", 9},
  {Scheme_unknown, (const BYTE *)"unknown", 7}
};
#define LAST_SCHEME 8

/*
 * Return the scheme type named by the given string. Returns Scheme_unknown
 * if it is not one of the predefined types.
 */
static Scheme
GetSchemeType (BYTE *bs, UINT16 len)
{
  INT16 i;

  if (bs == NULL)
    return Scheme_empty;

  for (i = 0; i < LAST_SCHEME; i++) {
    if (CompareStrings (bs, len,
                        (BYTE*)schemes[i].str, schemes[i].slen, TRUE)) {
      return schemes[i].sch;
    }
  }

  return Scheme_unknown;
}

/*
 * Return the scheme type named by the given string. Returns Scheme_unknown
 * if its not one of the predefined types.
 */
Scheme
Scheme_FromString (BYTE *sch)
{
  return GetSchemeType (sch, B_STRINGLENGTH ((const char *)sch));
}

/*
 * Return a string representation of the Scheme value.
 * NOTE: the caller must NOT modify or deallocate the returned string!
 */
const BYTE *
Scheme_ToString (Scheme scheme)
{
  return schemes[(scheme < LAST_SCHEME) ? scheme : LAST_SCHEME].str;
}

/*
 * Sets all fields in the URL struct to NULL and 0, respectively.
 */
void
URL_Clear (URL *url)
{
  UINT16 i;

  if (url == NULL)
    return;

  url->scheme_type = Scheme_empty;
  for (i = 0; i < NUM_URL_PARTS; i++) {
    url->s[i] = NULL;
    url->len[i] = 0;
  }
}

/************************************************************
 * The parsing of a URL.
 * First comes a number of private functions and macros,
 * and then the main routine, URL_Parse.
 ************************************************************/

/*
 * Check that the string of length 'len' is a non-empty decimal number.
 */
static BOOL
IsNumber (const BYTE *bs, INT16 len)
{
  INT16 i;

  if (len <= 0)
    return FALSE;

  for (i = 0; i < len; i++, bs++)
    if (!wae_isdigit (*bs))
      return FALSE;

  return TRUE;
}


static BOOL
CheckTopLabel (const BYTE *bs, UINT16 len)
{
  INT16 i;

  if (len == 0)
    return FALSE;

  if (!wae_isalpha (bs[0]))
    return FALSE;
  for (i = 1; i < len - 1; i++) {
    if (!wae_isalphanum (bs[i]) && (bs[i] != '-'))
      return FALSE;
  }
  return wae_isalphanum (bs[len - 1]);
}

static BOOL
CheckDomainLabel (const BYTE *bs, UINT16 len)
{
  INT16 i;

  if (len == 0)
    return FALSE;

  if (!wae_isalphanum (bs[0]))
    return FALSE;
  for (i = 1; i < len - 1; i++) {
    if (!wae_isalphanum (bs[i]) && (bs[i] != '-'))
      return FALSE;
  }
  return wae_isalphanum (bs[len - 1]);
}

static BOOL
CheckHostName (const BYTE *bs, UINT16 len)
{
  INT16 i, k;

  for (k = 0; k < len;) {
    for (i = k; (i < len) && (bs[i] != '.'); i++);
    if (i >= len)
      return CheckTopLabel (&bs[k], (UINT16)(i - k));
    if (!CheckDomainLabel (&bs[k], (UINT16)(i - k)))
      return FALSE;
    k = i + 1;
  }

  return TRUE;
}

static BOOL
CheckIPv4address (const BYTE *bs, UINT16 len)
{
  INT16 i, k;
  UINT16 n = 0;

  for (k = 0; k < len;) {
    for (i = k; (i < len) && (bs[i] != '.'); i++);
    if (i + 1 == len)
      return FALSE;
    if (!IsNumber (&bs[k], (INT16)(i - k)))
      return FALSE;
    n++;
    k = i + 1;
  }

  return (n == 4);
}

/*
 * Check that the string of length 'len' is a legal host name.
 * A host part is eiher a hostname or an IPv4 address.
 * In both cases, it is a string separated into parts by '.'
 */
BOOL
ValidateHost (const BYTE *bs, UINT16 len)
{
  INT16 i;

  if ((len == 0) || (bs[0] == '.'))
    return FALSE;

  /* Find right-most '.' */
  for (i = len - 1; (i > 0) && (bs[i] != '.'); i--);

  if (i == 0) {
    /* Only one component, must be a top-level domain. */
    return CheckTopLabel (bs, len);
  }
  else if (i == len - 1) {
    /* A final '.' can be ignored in a hostname. */
    return CheckHostName (bs, (UINT16)(len - 1));
  }
  else if (wae_isdigit (bs[i + 1])) {
    /* If the final component starts with a digit, it must be an IP-address */
    return CheckIPv4address (bs, len);
  }
  else {
    return CheckHostName (bs, len);
  }
}

/*
 * Parsing routines.
 * All parsing routines have the same function prototype.
 * The parameter "pbs" is a pointer to a pointer indicating
 * a position in a string where the parsing should start.
 * Upon success, this parameter is updated to point beyond
 * the segment of the string consumed by our parsing.
 * The second parameter, "url", is a pointer to a URL struct
 * where information about discovered components is stored,
 * if a component of the specified type is found.
 * In case of error, a parsing routine returns FALSE and does not update
 * the "pbs" parameter.
 */

static BOOL
ParseScheme (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p = start;
  UINT16 len;

  url->s[SCHEME_PART] = NULL;
  url->len[SCHEME_PART] = 0;

  for (p = start; *p; p++) { 
    BYTE b = *p;
    if ((b == ':') || (b == '/') || (b == '?') || (b == '#')) {
      break;
    }
  }

  len = (UINT16)(p - start);
  if ((len > 0) && (*p == ':')) {
    BYTE *q;

    if (!wae_isalpha (*start))
      return FALSE;

    for (q = start + 1; q < p; q++) {
      BYTE b = *q;
      if (!wae_isalphanum (b) && (b != '+') && (b != '-') && (b != '.')) {
        return FALSE;
      }
    }
    url->s[SCHEME_PART] = start;
    url->len[SCHEME_PART] = len;
    url->scheme_type = GetSchemeType (start, len);
    *pbs = p + 1;
  }
  else {
    url->scheme_type = Scheme_empty;
  }

  return TRUE;
}

static BOOL
ParseUserinfo (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p, *q;

  url->s[USERINFO_PART] = NULL;
  url->len[USERINFO_PART] = 0;

  for (p = start; *p; p++) {
    BYTE b = *p;
    if (b == '@') {
      break;
    }
    if ((b == '/') || (b == '?') || (b == '#')) {
      return TRUE;
    }
  }
  if (!*p) {
    return TRUE;
  }
  for (q = start; q < p; q++) {
    BYTE b = *q;
    if (!wae_ispchar (b) && (b != ';')) {
      return FALSE;
    }
  }
  url->s[USERINFO_PART] = start;
  url->len[USERINFO_PART] = (UINT16)(p - start);

  *pbs = p + 1;
  return TRUE;
}

static BOOL
ParseHost (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p;
  UINT16 len;

  url->s[HOST_PART] = NULL;
  url->len[HOST_PART] = 0;

  for (p = start; *p; p++) {
    BYTE b = *p;
    if ((b == ':') || (b == '/') || (b == '?') || (b == '#')) {
      break;
    }
  }  
  len = (UINT16)(p - start);
  if (len == 0) {
    return TRUE;
  }
  if (!ValidateHost (start, len)) {
    return FALSE;
  }
  url->s[HOST_PART] = start;
  url->len[HOST_PART] = len;

  *pbs = p;
  return TRUE;
}

static BOOL
ParsePort (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p;

  url->s[PORT_PART] = NULL;
  url->len[PORT_PART] = 0;

  if (*start != ':') {
    return TRUE;
  }
  start++;
  for (p = start; *p; p++) {
    BYTE b = *p;
    if ((b == '/') || (b == '?') || (b == '#')) {
      break;
    }
    if (!wae_isdigit (b)) {
      return FALSE;
    }
  }
  url->s[PORT_PART] = start;
  url->len[PORT_PART] = (UINT16)(p - start);

  *pbs = p;
  return TRUE;
}

static BOOL
ParseHostPort (BYTE **pbs, URL *url)
{
  BYTE *p = *pbs;

  if (!ParseHost (&p, url))
    return FALSE;
  if (url->len[HOST_PART] > 0) {
    if (!ParsePort (&p, url))
      return FALSE;
  }

  *pbs = p;
  return TRUE;
}

static BOOL
ParseAuthority (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p = start;

  if (!ParseUserinfo (&p, url) ||
      !ParseHostPort (&p, url))
    return FALSE;

  url->s[AUTHORITY_PART] = start;
  url->len[AUTHORITY_PART] = (UINT16)(p - start);
  
  *pbs = p;
  return TRUE;
}

static BOOL
ParseQuery (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p;

  url->s[QUERY_PART] = NULL;
  url->len[QUERY_PART] = 0;

  if (*start != '?') {
    return TRUE;
  }
  start++;
  for (p = start; *p; p++) {
    BYTE b = *p;
    if (b == '#') {
      break;
    }
    if (!wae_isuric (b)) {
      if ((b == '%') && HexToByte (p + 1, &b)) {
        p += 2;
      }
      else
        return FALSE;
    }
  }
  url->s[QUERY_PART] = start;
  url->len[QUERY_PART] = (UINT16)(p - start);

  *pbs = p;
  return TRUE;
}

/*
 * NOTE: our parsing of fragments differ from RFC2396 in that we
 * allow '#' and '"' in fragment parts. This is in response to
 * a request from ECS.
 */
static BOOL
ParseFragment (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p;

  url->s[FRAGMENT_PART] = NULL;
  url->len[FRAGMENT_PART] = 0;

  if (*start != '#') {
    return TRUE;
  }
  start++;
  for (p = start; *p; p++) {
    BYTE b = *p;
    if (!wae_isuric (b) && (b != '#') && (b != '"')) {
      if ((b == '%') && HexToByte (p + 1, &b)) {
        p += 2;
      }
      else
        return FALSE;
    }
  }
  url->s[FRAGMENT_PART] = start;
  url->len[FRAGMENT_PART] = (UINT16)(p - start);

  *pbs = p;
  return TRUE;
}

static BOOL
ParseAbsPath (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p;

  url->s[PATH_PART] = start;
  url->len[PATH_PART] = 0;
  if (*start != '/') {
    return TRUE;
  }
  for (p = start; *p; p++) {
    BYTE b = *p;
    if ((b == '?') || (b == '#')) {
      break;
    }
    if (!wae_ispchar (b) && (b != '/') && (b != ';')) {
      if ((b == '%') && HexToByte (p + 1, &b)) {
        p += 2;
      }
      else
        return FALSE;
    }
  }
  url->s[PATH_PART] = start;
  url->len[PATH_PART] = (UINT16)(p - start);

  *pbs = p;
  return TRUE;
}

static BOOL
ParseRelPath (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p;
  UINT16 len;

  url->s[PATH_PART] = start;
  url->len[PATH_PART] = 0;

  for (p = start; *p; p++) {
    BYTE b = *p;
    if ((b == '/') || (b == '?') || (b == '#')) {
      break;
    }
    if (b == ':')
      return FALSE;
    if (!wae_ispchar (b) && (b != '/') && (b != ';')) {
      if ((b == '%') && HexToByte (p + 1, &b)) {
        p += 2;
      }
      else
        return FALSE;
    }
  }

  len = (UINT16)(p - start);
  if (len == 0) {
    return TRUE;
  }

  if (!ParseAbsPath (&p, url)) {
    return FALSE;
  }
  url->s[PATH_PART] -= len;
  url->len[PATH_PART] += len;

  *pbs = p;
  return TRUE;
}

static BOOL
ParseNetPath (BYTE **pbs, URL *url)
{
  BYTE *pos = *pbs;

  if ((pos[0] != '/') || (pos[1] != '/'))
    return TRUE;

  pos += 2;
  if (!ParseAuthority (&pos, url) ||
      !ParseAbsPath (&pos, url))
    return FALSE;

  *pbs = pos;
  return TRUE;
}

static BOOL
ParseRelativeURI (BYTE **pbs, URL *url)
{
  BYTE *pos = *pbs;

  if ((pos[0] == '/') && (pos[1] == '/')) {
    if (!ParseNetPath (&pos, url))
      return FALSE;
  }
  else if (pos[0] == '/') {
    if (!ParseAbsPath (&pos, url))
      return FALSE;
  }
  else if (!ParseRelPath (&pos, url))
    return FALSE;

  if (!ParseQuery (&pos, url))
    return FALSE;

  *pbs = pos;
  return TRUE;
}

static BOOL
ParseHierPart (BYTE **pbs, URL *url)
{
  BYTE *pos = *pbs;

  if ((pos[0] == '/') && (pos[1] == '/')) {
    if (!ParseNetPath (&pos, url))
      return FALSE;
  }
  else {
    if (!ParseAbsPath (&pos, url))
      return FALSE;
  }

  if (!ParseQuery (&pos, url))
    return FALSE;

  *pbs = pos;
  return TRUE;
}

static BOOL
ParseOpaquePart (BYTE **pbs, URL *url)
{
  BYTE *start = *pbs;
  BYTE *p;
  BYTE b;

  url->s[PATH_PART] = start;
  url->len[PATH_PART] = 0;

  p = start;
  b = *p;
  if (!*p || (b == '/'))
    return TRUE;

  if (!wae_isuric (b)) {
    if ((b == '%') && HexToByte (p + 1, &b)) {
      p += 2;
    }
    else
      return FALSE;
  }
  p++;

  for (;*p; p++) {
    b = *p;
    if (b == '#')
      break;
    if (!wae_isuric (b)) {
      if ((b == '%') && HexToByte (p + 1, &b)) {
        p += 2;
      }
      else
        return FALSE;
    }
  }

  url->s[PATH_PART] = start;
  url->len[PATH_PART] = (UINT16)(p - start);

  *pbs = p;
  return TRUE;
}

/*
 * Take a string representation of a URL and parse it into its
 * components, and store these as fields in the given URL struct.
 * All components are stored in their original (possibly escaped) form.
 * Returns TRUE if the URL was valid, FALSE otherwise.
 *
 * Unless otherwise noted, we parse according to the definition
 * in RFC2396.
 */
BOOL
URL_Parse (BYTE *bs, URL *url)
{
  BYTE *start = bs;
  UINT16 len;

  if ((bs == NULL) || (url == NULL))
    return FALSE;

  URL_Clear (url);
  len = B_STRINGLENGTH ((const char *)bs);

  if (!ParseScheme (&start, url))
    return FALSE;

  if (url->scheme_type == Scheme_empty) {
    if (!ParseRelativeURI (&start, url))
      return FALSE;
  }
  else if (start[0] == '/') {
    if (!ParseHierPart (&start, url))
      return FALSE;
  }
  else {
    if (!ParseOpaquePart (&start, url))
      return FALSE;
  }

  if (!ParseFragment (&start, url))
    return FALSE;

  /* Make sure that there is nothing left in the string */
  return len == (start - bs);
}

/*
 * The inverse of the parsing routine: create a URL string
 * from its different parts.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned string.
 */
BYTE *
AssembleURLString (URL *url)
{
  extern BOOL Portdetect;
  UINT16 len =
    (url->s[SCHEME_PART] != NULL ? url->len[SCHEME_PART] + 1 : 0) +
    (url->s[AUTHORITY_PART] != NULL ? 2 : 0) +
    (url->s[USERINFO_PART] != NULL ? url->len[USERINFO_PART] + 1 : 0) +
    (url->s[HOST_PART] != NULL ? url->len[HOST_PART] : 0) +
    (url->s[PORT_PART] != NULL ? url->len[PORT_PART] + 1 : 0) +
    (url->s[PATH_PART] != NULL ? url->len[PATH_PART] : 0) +
    (url->s[QUERY_PART] != NULL ? url->len[QUERY_PART] + 1 : 0) +
    (url->s[FRAGMENT_PART] != NULL ? url->len[FRAGMENT_PART] + 1 : 0);
  BYTE *buf, *p;

  //jjkyg
  Portdetect = FALSE;
  
  if ((buf = NEWARRAY (BYTE, len + 1 )) == NULL) {
    return NULL;
  }

  p = buf;
  if (url->s[SCHEME_PART] != NULL) {
    B_COPYSTRINGN (p, url->s[SCHEME_PART], url->len[SCHEME_PART]);
    p += url->len[SCHEME_PART];
    *p++ = ':';
  }

  if (url->s[AUTHORITY_PART] != NULL) {
    *p++ = '/';
    *p++ = '/';
  }    

  if (url->s[USERINFO_PART] != NULL) {
    B_COPYSTRINGN (p, url->s[USERINFO_PART], url->len[USERINFO_PART]);
    p += url->len[USERINFO_PART];
    *p++ = '@';
  }

  if (url->s[HOST_PART] != NULL) {
    B_COPYSTRINGN (p, url->s[HOST_PART], url->len[HOST_PART]);
    p += url->len[HOST_PART];
  }

  if (url->s[PORT_PART] != NULL) 
  {
	extern VOID CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);  
	CHAR* pdetect;
	int portlength = 0;
	
	portlength = (url->len[PORT_PART] > 4)?4:url->len[PORT_PART];
	CLNTa_log(1,0,"portlength %d",portlength);
	pdetect = (CHAR*)url->s[PORT_PART];

	if(strncmp(pdetect,"9091",portlength)==0)
	{
		Portdetect = TRUE;
	}
	else
	{
		Portdetect = FALSE;
	}

    *p++ = ':';
    B_COPYSTRINGN (p, url->s[PORT_PART], url->len[PORT_PART]);
	p += url->len[PORT_PART];
  }
  else//jjkyg
  {
	Portdetect = FALSE;
  }

  if (url->s[PATH_PART] != NULL) {
    B_COPYSTRINGN (p, url->s[PATH_PART], url->len[PATH_PART]);
    p += url->len[PATH_PART];
  }

  if (url->s[QUERY_PART] != NULL) {
    *p++ = '?';
    B_COPYSTRINGN (p, url->s[QUERY_PART], url->len[QUERY_PART]);
    p += url->len[QUERY_PART];
  }

  if (url->s[FRAGMENT_PART] != NULL) {
    *p++ = '#';
    B_COPYSTRINGN (p, url->s[FRAGMENT_PART], url->len[FRAGMENT_PART]);
    p += url->len[FRAGMENT_PART];
  }
  *p = '\0';
#ifdef WAP_USE_SECURE
  if(strstr((char *)buf,(char *)"#")!=NULL) 
  {
	  Portdetect = FALSE;
  }
#endif//#ifdef WAP_USE_SECURE

  return buf;
}

/*
 * Copy part of a URL, while replacing escape sequences with
 * the characters they represent, unless they are "reserved".
 */
static void
FetchPart (BYTE **pbs, URL *url, UINT8 whichPart)
{
  BYTE *p, *q;
  UINT16 len;
  INT16 i, j, k;
  BYTE b, tmp;

  p = *pbs;
  q = url->s[whichPart];
  len = url->len[whichPart];
  if ((q != NULL) && (len > 0)) {
    for (i = 0, j = 0; j < len; i++, j++) {
      b = q[j];
      if ((b == '%') && (j + 2 < len) && HexToByte (&q[j + 1], &tmp)) {
        if (wae_isreserved (tmp) || (tmp == '#')) {
          for (k = 0; k < 3; k++)
            p[i + k] = DowncaseByte (q[j + k]);
          i += 2;
        }
        else {
          p[i] = tmp;
        }
        j += 2;
      }
      else {
        p[i] = q[j];
      }
    }
    p += i;
  }

  *pbs = p;
}

/*
 * Create a "canonical" form of a given URL.
 * The following conversions are applied:
 *   1) The scheme and authority parts are changed into lower case.
 *   2) Default port numbers are removed, other port numbers have
 *      leading zeros dropped.
 *   3) Escape sequences (%xy) are replaced by the character they
 *      represent, unless it is a reserved character. In the latter
 *      case, the hex digits in the escape sequence are changed to
 *      lower case.
 * For any two URLs X and Y that compare equal (i.e., the routine
 * URL_Equal returns TRUE), this routine will produce identical
 * canonical forms.
 * Returns FALSE in case of error.
 */
BOOL
b_MakeCanonical (const BYTE *bs, BYTE **can)
{
  UINT16 len, port;
  BYTE   *p, *q, *buf;
  INT16  i;
  URL    url;

  if ((bs == NULL) || (can == NULL))
    return FALSE;
  *can = NULL;
  if (!URL_Parse ((BYTE*)bs, &url))
    return FALSE;

  len = B_STRINGLENGTH ((const char *)bs);
  if ((buf = NEWARRAY (BYTE, len + 1)) == NULL)
    return FALSE;

  p = buf;
  if (url.s[SCHEME_PART] != NULL) {
    q = url.s[SCHEME_PART];
    for (i = 0; i < url.len[SCHEME_PART]; i++)
      p[i] = DowncaseByte (q[i]);
    p += url.len[SCHEME_PART];
    *p++ = ':';
  }
  if (url.s[AUTHORITY_PART] != NULL) {
    *p++ = '/';
    *p++ = '/';

    if (url.s[USERINFO_PART] != NULL) {
      q = url.s[USERINFO_PART];
      for (i = 0; i < url.len[USERINFO_PART]; i++)
        p[i] = DowncaseByte (q[i]);
      p += url.len[USERINFO_PART];
      *p++ = '@';
    }

    if (url.s[HOST_PART] != NULL) {
      q = url.s[HOST_PART];
      len = url.len[HOST_PART];
      if ((len > 0) && (q[len - 1] == '.'))
        len--;
      for (i = 0; i < len; i++)
        p[i] = DowncaseByte (q[i]);
      p += len;
    }

    /* Get port number: skip default port numbers and empty port numbers,
     * and drop leading zeros in the port number. */
    q = url.s[PORT_PART];
    len = url.len[PORT_PART];
    if ((q != NULL) && (len > 0)) {
      port = GetNum (q, len);
      if (!(((url.scheme_type == Scheme_http) && (port == 80)) ||
            ((url.scheme_type == Scheme_https) && (port == 443)))) {
        *p++ = ':';
        if (port == 0) {
          *p++ = '0';
        }
        else {
          while (*q == '0') {
            q++;
            len--;
          }
          for (i = 0; i < len; i++)
            p[i] = q[i];
          p += len;
        }
      }
    }
  }

  FetchPart (&p, &url, PATH_PART);

  if (url.s[QUERY_PART] != NULL) {
    *p++ = '?';
    FetchPart (&p, &url, QUERY_PART);
  }
  if (url.s[FRAGMENT_PART] != NULL) {
    *p++ = '#';
    FetchPart (&p, &url, FRAGMENT_PART);
  }
  *p = '\0';

  *can = buf;
  return TRUE;
}

/*
 * Compute a hash value from a URL and store it in the location pointed
 * to by "hv". All equivalent URLs will hash to the same value,
 * but two non-equal URLs may also have the same hash value. However,
 * the probability of a collision is small.
 * The hash function is simply an XOR of the URL string divided
 * into four-byte blocks.
 * Returns FALSE on error, TRUE otherwise.
 */
BOOL
b_HashURL (const BYTE *bs, UINT32 *hv)
{
  BYTE   *cb;
  UINT16 len;
  UINT32 n = 0;

  if (!b_MakeCanonical (bs, &cb))
    return FALSE;

  len = B_STRINGLENGTH ((const char *)cb);
  if (len > 0) {
    INT16 i = 0;
    /* The following code uses "Duff's Device", i.e., a partially
     * unwound loop and with a switch statement to jump into the middle
     * of the loop! */
    switch (len & 0x3) {
    case 0:
      while (i < len) {
        n ^= cb[i++];
      case 3:
        n ^= cb[i++] << 8;
      case 2:
        n ^= cb[i++] << 16;
      case 1:
        n ^= cb[i++] << 24;
      }
    }
  }

  DEALLOC (&cb);
  *hv = n;
  return TRUE;
}

/*
 * Take a string representation of a URL and parse it into its
 * components, and store these as fields in the given URL struct.
 * All components are stored in their original (possibly escaped) form.
 * Returns TRUE if the URL was valid, FALSE otherwise. In the latter
 * case, nothing is stored in the URL struct.
 */
BOOL
URL_FromWideString (const WCHAR *ws, URL *url)
{
  BOOL bReturn = FALSE;
  BOOL bOverflowDetected = FALSE;
  BYTE* pbStr = NULL;

  if (ws != NULL) {
    pbStr = wip_wchar2byte (ws, &bOverflowDetected);
    if (pbStr != NULL && !bOverflowDetected) {
      bReturn = URL_FromByteString (pbStr, url);
    }
    else if (!bReturn)
      DEALLOC (&pbStr);
  }
  return bReturn;
}

BOOL
URL_FromByteString (const BYTE *bs, URL *url)
{
  return URL_Parse ((BYTE*)bs, url);
}


/*
 * Given a URL struct, construct and return a string representation
 * of the URL.
 * Returns NULL in case of error.
 * NOTE: It is the callers responsibility to deallocate the returned string.
 */
WCHAR *
URL_ToWideString (URL *url)
{
  WCHAR *ws = NULL;
  BYTE  *tmp = URL_ToByteString (url);

  if (tmp == NULL) {
    return NULL;
  }
  ws = wip_byte2wchar (tmp);
  DEALLOC (&tmp);

  return ws;
}


BYTE *
URL_ToByteString (URL *url)
{
  if (url == NULL) {
    return NULL;
  }

  return AssembleURLString (url);
}


#define IS_DOTDOTSEGMENT(x) ((x + 2 < n) && (buf[(x)] == '.') &&\
    (buf[(x + 1)] == '.') && (buf[(x + 2)] == '/'))

#define NEXTSEGMENT(x) if (buf[x] == '/') { do x++; while (buf[x] == '/');}\
  else { while ((x < n) && (buf[x] != '/')) x++; if (x < n) x++;}

#define PREVSEGMENT(x) if (x > 1) {\
  x -= 2;\
  while ((x > 0) && (buf[x] != '/')) x--;\
  x++;}\
  else {\
    x = 0;\
  }

/*
 * Given two path names, an absolute path (starting with '/') in 'base',
 * and a relative path in 'rel', combine them into a new absolute path
 * removing all unnecessary './' and '../' segments.
 * Returns a newly allocated string with the new path.
 */
static BYTE *
RemoveDots (BYTE *base, UINT16 base_len, BYTE *rel, UINT16 rel_len)
{
  UINT16 len;
  BYTE   *buf;
  INT16  i, j, k, m, n;

  /* Find the right-most '/' character in the base path. */
  for (i = base_len - 1; i >= 0; i--) {
    if (base[i] == '/') {
      break;
    }
  }
  len = i + 1;
  if ((buf = NEWARRAY (BYTE, len + rel_len + 1)) == NULL)
    return NULL;

  /* Append the relative path to the base path. */
  B_COPYSTRINGN (buf, base, len);
  B_COPYSTRINGN (buf + len, rel, rel_len);
  n = len + rel_len;
  buf[n] = '\0';

  /* Remove all occurrences of './' */
  for (i = j = 0; j < n; ) {
    if (((j == 0) || (buf[j - 1] == '/')) && (buf[j] == '.') &&
        (j < n - 1) && (buf[j + 1] == '/')) {
      j += 2;
      continue;
    }
    else if (i != j) {
      buf[i] = buf[j];
    }
    i++, j++;
  }
  n = i;

  /* If the string ends with '.', remove it. */
  if (((n == 1) || (n >= 2) && (buf[n - 2] == '/')) && (buf[n - 1] == '.')) {
    n--;
  }
  buf[n] = '\0';

  /* Remove all occurrences of '<segment>/../'. */
  i = 0;
  NEXTSEGMENT (i);
  j = i;
  NEXTSEGMENT (j);
  m = n;
  while (j < n) {
    if ((buf[i] != '/') && !IS_DOTDOTSEGMENT (i) && IS_DOTDOTSEGMENT (j)) {
      k = i;
      NEXTSEGMENT (k);
      m -= (k - i) + 3;
      PREVSEGMENT (i);
      NEXTSEGMENT (j);
    }
    else {
      k = j;
      NEXTSEGMENT (j);
      NEXTSEGMENT (i);
      /* Copy one segment from k to i */
      if (i != k) {
        B_COPYSTRINGN (&buf[i], &buf[k], j - k);
      }
    }
  }
  n = m;

  /* If the string ends with '<segment>/..', then remove that part. */
  if ((n > 3) && (buf[n - 3] == '/') && (buf[n - 2] == '.') &&
      (buf[n - 1] == '.')) {
    i = n - 2;
    PREVSEGMENT (i);
    if (!IS_DOTDOTSEGMENT (i)) {
      n = i;
    }
  }
  buf[n] = '\0';

  return buf;
}

/*
 * Given a base URL and a relative URL, resolve the relative reference
 * and store as an absolute URL in the string "*abs".
 * Returns TRUE on success, FALSE otherwise, in which case nothing
 * is stored in "abs".
 * NOTE: It is the callers responsibility to deallocate the returned string.
 */
BOOL
URL_Resolve (URL *base, URL *rel, BYTE **abs)
{
  URL new_url;

  if ((base == NULL) || (rel == NULL) || (abs == NULL))
    return FALSE;

  if (base->s[SCHEME_PART] == NULL)
    /* Not a correct absolute URL. */
    return FALSE;

  if (base->len[PATH_PART] == 0) {
    base->s[PATH_PART] =(BYTE *) "/";
    base->len[PATH_PART] = 1;
  }

  /* If rel has a scheme, then it is an absolute reference.
   * Just copy it. */
  if (rel->s[SCHEME_PART] != NULL) {
    if ((*abs = AssembleURLString (rel)) == NULL)
      return FALSE;
  }

  /* If rel has an authority part, just use the scheme from the base part. */
  else if (rel->s[AUTHORITY_PART] != NULL) {
    new_url = *rel;
    new_url.s[SCHEME_PART] = base->s[SCHEME_PART];
    new_url.len[SCHEME_PART] = base->len[SCHEME_PART];
    new_url.scheme_type = base->scheme_type;

    *abs = AssembleURLString (&new_url);
  }
  else if ((rel->len[PATH_PART] == 0) && (rel->s[QUERY_PART] == NULL)) {
    /* 'rel' is empty or just a fragment */
    new_url = *base;
    new_url.s[FRAGMENT_PART] = rel->s[FRAGMENT_PART];
    new_url.len[FRAGMENT_PART] = rel->len[FRAGMENT_PART];

    *abs = AssembleURLString (&new_url);
  }
  else {
    /* 'rel' does not have a scheme nor an authority part,
     * but is more than just a fragment */
    BYTE *tmp;

    if (rel->len[PATH_PART] == 0)
      tmp = RemoveDots (base->s[PATH_PART], base->len[PATH_PART],
                        (BYTE *)"", 0);
    else if (*(rel->s[PATH_PART]) != '/')
      tmp = RemoveDots (base->s[PATH_PART], base->len[PATH_PART],
                        rel->s[PATH_PART], rel->len[PATH_PART]);
    else
      tmp = newstring (rel->s[PATH_PART], rel->len[PATH_PART]);

    if (tmp == NULL) /* Memory allocation failed */
      return FALSE;

    new_url = *base;
    new_url.s[PATH_PART] = tmp;
    new_url.len[PATH_PART] = B_STRINGLENGTH ((const char *)tmp);
    new_url.s[QUERY_PART] = rel->s[QUERY_PART];
    new_url.len[QUERY_PART] = rel->len[QUERY_PART];
    new_url.s[FRAGMENT_PART] = rel->s[FRAGMENT_PART];
    new_url.len[FRAGMENT_PART] = rel->len[FRAGMENT_PART];

    *abs = AssembleURLString (&new_url);
    DEALLOC (&tmp);
  }

  return TRUE;
}

BOOL
w_Resolve (const WCHAR *base, const WCHAR *rel, WCHAR **abs)
{
  BYTE *b_base, *b_rel, *b_abs;
  BOOL err = FALSE;

  if ((base == NULL) || (rel == NULL) || (abs == NULL))
    return FALSE;

  if (((b_base = wip_wchar2byte (base, &err)) == NULL) || err) {
    return FALSE;
  }
  if (((b_rel = wip_wchar2byte (rel, &err)) == NULL) || err) {
    DEALLOC (&b_base);
    return FALSE;
  }
  if (!b_Resolve (b_base, b_rel, &b_abs)) {
    DEALLOC (&b_base);
    DEALLOC (&b_rel);
    return FALSE;
  }
  if ((*abs = wip_byte2wchar (b_abs)) == NULL) {
    DEALLOC (&b_base);
    DEALLOC (&b_rel);
    DEALLOC (&b_abs);
    return FALSE;
  } 

  DEALLOC (&b_base);
  DEALLOC (&b_rel);
  DEALLOC (&b_abs);

  return TRUE;
}

BOOL
b_Resolve (const BYTE *base, const BYTE *rel, BYTE **abs)
{
  URL base_url, rel_url;
  BOOL ok = TRUE;

  if ((base == NULL) || (rel == NULL) || (abs == NULL))
    return FALSE;

  URL_Clear (&rel_url);

  if (!URL_FromByteString (base, &base_url) ||
      !URL_FromByteString (rel, &rel_url) ||
      !URL_Resolve (&base_url, &rel_url, abs)) {
    *abs = NULL;
    ok = FALSE;
  }

  return ok;
}


/*
 * Return TRUE if the two URLs are equal, FALSE otherwise.
 * "whichComponents" is a bitmap indicating which parts of the URLs
 * should be included in the comparison.
 * Returns FALSE in case of error.
 */
BOOL
URL_Equal (URL *url1, URL *url2, BYTE whichComponents)
{
  BYTE   *p1, *p2, *path1, *path2;
  UINT16 p1_len, p2_len, path1_len, path2_len;
  UINT16 port1 = 0, port2 = 0;

  if (url1 == NULL)
    return (url2 == NULL);
  if (url2 == NULL)
    return FALSE;

  if (whichComponents & PORT_COMP) {
    if ((url1->s[PORT_PART] == NULL) &&
        (url1->scheme_type == Scheme_http)) {
      p1 = (BYTE *)"80";
      p1_len = 2;
    }
    else if ((url1->s[PORT_PART] == NULL) &&
             (url1->scheme_type == Scheme_https)) {
      p1 = (BYTE *)"443";
      p1_len = 3;
    }
    else {
      p1 = url1->s[PORT_PART];
      p1_len = url1->len[PORT_PART];
    }
    port1 = GetNum (p1, p1_len);

    if ((url2->s[PORT_PART] == NULL) &&
        (url2->scheme_type == Scheme_http)) {
      p2 = (BYTE *)"80";
      p2_len = 2;
    }
    else if ((url2->s[PORT_PART] == NULL) &&
             (url2->scheme_type == Scheme_https)) {
      p2 = (BYTE *)"443";
      p2_len = 3;
    }
    else {
      p2 = url2->s[PORT_PART];
      p2_len = url2->len[PORT_PART];
    }
    port2 = GetNum (p2, p2_len);
  }

  if (url1->len[PATH_PART] == 0) {
    path1 = (BYTE *)"/";
    path1_len = 1;
  }
  else {
    path1 = url1->s[PATH_PART];
    path1_len = url1->len[PATH_PART];
  }

  if (url2->len[PATH_PART] == 0) {
    path2 = (BYTE *)"/";
    path2_len = 1;
  }
  else {
    path2 = url2->s[PATH_PART];
    path2_len = url2->len[PATH_PART];
  }

  return
    (!(whichComponents & SCHEME_COMP) ||
     CompareStrings (url1->s[SCHEME_PART], url1->len[SCHEME_PART],
                     url2->s[SCHEME_PART], url2->len[SCHEME_PART], TRUE)) &&
    (!(whichComponents & USERINFO_COMP) ||
     CompareStrings (url1->s[USERINFO_PART], url1->len[USERINFO_PART],
                     url2->s[USERINFO_PART], url2->len[USERINFO_PART],
                     TRUE)) &&
    (!(whichComponents & HOST_COMP) ||
     CompareStrings (url1->s[HOST_PART], url1->len[HOST_PART],
                     url2->s[HOST_PART], url2->len[HOST_PART], TRUE)) &&
    (!(whichComponents & PORT_COMP) || (port1 == port2)) &&
    (!(whichComponents & PATH_COMP) ||
     CompareStrings (path1, path1_len, path2, path2_len, FALSE)) &&
    (!(whichComponents & QUERY_COMP) ||
     CompareStrings (url1->s[QUERY_PART], url1->len[QUERY_PART],
                     url2->s[QUERY_PART], url2->len[QUERY_PART], FALSE)) &&
    (!(whichComponents & FRAG_COMP) ||
     CompareStrings (url1->s[FRAGMENT_PART], url1->len[FRAGMENT_PART],
                     url2->s[FRAGMENT_PART], url2->len[FRAGMENT_PART], FALSE));
}

BOOL
w_EqualURL (const WCHAR *url1, const WCHAR *url2, BYTE whichComponents)
{
  BYTE *b_url1, *b_url2;
  BOOL err = FALSE;
  BOOL res;

  if (url1 == NULL)
    return (url2 == NULL);
  if (url2 == NULL)
    return FALSE;

  if (((b_url1 = wip_wchar2byte (url1, &err)) == NULL) || err) {
    return FALSE;
  }
  if (((b_url2 = wip_wchar2byte (url2, &err)) == NULL) || err) {
    DEALLOC (&b_url1);
    return FALSE;
  }
  res = b_EqualURL (b_url1, b_url2, whichComponents);

  DEALLOC (&b_url1);
  DEALLOC (&b_url2);
    
  return res;
}

BOOL
b_EqualURL (const BYTE *bs1, const BYTE *bs2, BYTE whichComponents)
{
  URL url1, url2;

  if (bs1 == NULL)
    return (bs2 == NULL);
  if (bs2 == NULL)
    return FALSE;

  return URL_FromByteString (bs1, &url1) &&
    URL_FromByteString (bs2, &url2) &&
    URL_Equal (&url1, &url2, whichComponents);
}


/*
 * Return TRUE if the given string URL has a valid format, FALSE otherwise.
 */
BOOL
w_IsValid (const WCHAR* pchUrl)
{
  BOOL  bReturn = FALSE;
  BOOL  bOverflowDetected = FALSE;
  BYTE* pbStr = NULL;

  if (pchUrl != NULL) {
    pbStr = wip_wchar2byte (pchUrl, &bOverflowDetected);
    if (pbStr != NULL && !bOverflowDetected) {
      bReturn = b_IsValid (pbStr);
    }
    DEALLOC (&pbStr);
  }
  return bReturn;
}

BOOL
b_IsValid (const BYTE* pbUrl)
{
  URL url;

  return URL_Parse ((BYTE*)pbUrl, &url);
}


/*
 * Retrieval of the different parts of a URL.
 */

/*
 * Return the Scheme of the URL.
 * Returns NULL in case of error, or if the URL does not have a scheme part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE*
URL_GetScheme (URL *url)
{
  if (url == NULL)
    return NULL;

  return newstring (url->s[SCHEME_PART], url->len[SCHEME_PART]);
}

Scheme
URL_GetSchemeType (URL *url)
{
  if (url == NULL)
    return Scheme_empty;

  return url->scheme_type;
}

static BOOL
w_GetPart (const WCHAR* pchUrl, UINT16 whichpart, WCHAR **part)
{
  URL  url;
  BYTE *bs = NULL;
  BOOL err = FALSE;

  if ((pchUrl == NULL) || (part == NULL))
    return FALSE;
  *part = NULL;

  bs = wip_wchar2byte (pchUrl, &err);
  if ((bs == NULL) || err ) {
    return FALSE;
  }
  if (!URL_Parse (bs, &url)) {
    DEALLOC (&bs);
    return FALSE;
  }

  if (url.s[whichpart] != NULL) {
    UINT16 len = url.len[whichpart];
    WCHAR *buf = NEWARRAY (WCHAR, len + 1);
    WCHAR *p = buf;
    BYTE  *q = url.s[whichpart];
    INT16 i;

    if (buf == NULL) {
      DEALLOC (&bs);
      return FALSE;
    }
    for (i = 0; i < len; i++) {
      p[i] = (WCHAR)q[i];
    }
    p[len] = (WCHAR)0;
    *part = buf;
  }

  DEALLOC (&bs);
  return TRUE;
}

static BOOL
b_GetPart (const BYTE* pbUrl, UINT16 whichpart, BYTE **part)
{
  URL url;

  if ((pbUrl == NULL) || (part == NULL))
    return FALSE;

  if (!URL_Parse ((BYTE*)pbUrl, &url)) {
    return FALSE;
  }
  *part = NULL;
  if ((url.s[whichpart] != NULL) &&
      ((*part = newstring (url.s[whichpart], url.len[whichpart])) == NULL)) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Extract the scheme of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a scheme component.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string (applies to w_GetScheme and b_GetScheme).
 */
BOOL
w_GetScheme (const WCHAR* pchUrl, WCHAR **scheme)
{
  return w_GetPart (pchUrl, SCHEME_PART, scheme);
}

BOOL
w_GetSchemeType (const WCHAR* pchUrl, Scheme *scheme)
{
  URL  url;
  BYTE *bs = NULL;
  BOOL err = FALSE;
  BOOL ok = TRUE;

  if ((pchUrl == NULL) || (scheme == NULL))
    return FALSE;

  bs = wip_wchar2byte (pchUrl, &err);
  if ((bs != NULL) && !err && URL_Parse (bs, &url)) {
    *scheme = url.scheme_type;
  }
  else
    ok = FALSE;
  DEALLOC (&bs);

  return ok;
}

BOOL
b_GetScheme (const BYTE* pbUrl, BYTE **scheme)
{
  return b_GetPart (pbUrl, SCHEME_PART, scheme);
}

BOOL
b_GetSchemeType (const BYTE* pbUrl, Scheme *scheme)
{
  URL url;

  if ((pbUrl == NULL) || (scheme == NULL))
    return FALSE;

  if (!URL_Parse ((BYTE*)pbUrl, &url))
    return FALSE;
  *scheme = url.scheme_type;

  return TRUE;
}


/*
 * Return the host part of a URL.
 * Returns NULL in case of error, or if the URL does not have a host part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE*
URL_GetHost (URL *url)
{
  if (url == NULL)
    return NULL;

  return newstring (url->s[HOST_PART], url->len[HOST_PART]);
}

/*
 * Extract the host part of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a host component.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetHost (const WCHAR* pchUrl, WCHAR **host)
{
  return w_GetPart (pchUrl, HOST_PART, host);
}

BOOL
b_GetHost (const BYTE* pchUrl, BYTE **host)
{
  return b_GetPart (pchUrl, HOST_PART, host);
}


/*
 * Return the port number of a URL.
 * Returns NULL in case of error, or if the URL does not have a port number.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE*
URL_GetPort (URL *url)
{
  if (url == NULL)
    return NULL;

  return newstring (url->s[PORT_PART], url->len[PORT_PART]);
}

/*
 * Extract the port number of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a port number.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetPort (const WCHAR* pchUrl, WCHAR **port)
{
  return w_GetPart (pchUrl, PORT_PART, port);
}

BOOL
b_GetPort (const BYTE* pbUrl, BYTE **port)
{
  return b_GetPart (pbUrl, PORT_PART, port);
}


/*
 * Return the path component of a URL.
 * Returns NULL in case of error, or if the URL does not have a path component.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE*
URL_GetPath (URL *url)
{
  BYTE  *bs, *p, *q;
  INT16 i, len;
  INT16 inparam;

  if ((url == NULL) || (url->len[PATH_PART] == 0)) {
    return NULL;
  }

  q = url->s[PATH_PART];
  len = 0;
  inparam = 0;
  for (i = 0; i < url->len[PATH_PART]; i++) {
    if (inparam && (q[i] == '/')) {
      inparam = 0;
    }
    else if (!inparam && (q[i] == ';')) {
      inparam = 1;
    }
    if (!inparam) {
      len++;
    }
  }

  bs = NEWARRAY (BYTE, len + 1);
  if (bs == NULL)
    return NULL;

  p = bs;
  q = url->s[PATH_PART];
  inparam = 0;
  for (i = 0; i < url->len[PATH_PART]; i++) {
    if (inparam && (q[i] == '/')) {
      inparam = 0;
    }
    else if (!inparam && (q[i] == ';')) {
      inparam = 1;
    }
    if (!inparam) {
      *p++ = q[i];
    }
  }
  *p = '\0';

  return bs;
}

/*
 * Extract the path component of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a path component.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetPath (const WCHAR* pchUrl, WCHAR **path)
{
  URL  url;
  BYTE *bs = NULL;
  BOOL err = FALSE;
  BYTE *p;

  if ((pchUrl == NULL) || (path == NULL))
    return FALSE;
  *path = NULL;

  bs = wip_wchar2byte (pchUrl, &err);
  if ((bs == NULL) || err) {
    return FALSE;
  }
  if (!URL_Parse (bs, &url)) {
    DEALLOC (&bs);
    return FALSE;
  }
  p = URL_GetPath (&url);
  *path = wip_byte2wchar (p);

  DEALLOC (&bs);
  DEALLOC (&p);

  return TRUE;
}

BOOL
b_GetPath (const BYTE* pbUrl, BYTE **path)
{
  URL url;

  if ((pbUrl == NULL) || (path == NULL))
    return FALSE;
  *path = NULL;

  if (!URL_Parse ((BYTE*)pbUrl, &url)) {
    return FALSE;
  }
  *path = URL_GetPath (&url);

  return TRUE;
}


/*
 * Return the parameter component of a URL.
 * Returns NULL in case of error, or if the URL does not have any parameters.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetParameters (URL *url)
{
  INT16  i;
  UINT16 len, plen;
  BYTE   *p, *bs;

  if ((url == NULL) || (url->len[PATH_PART] == 0)) {
    return NULL;
  }

  len = url->len[PATH_PART];
  p = url->s[PATH_PART];

  /* Find the right-most '/', if any. */
  for (i = len - 1; (i >= 0) && (p[i] != '/'); i--);
  i++;

  /* Find the first ';' after the last '/', if any. */
  for (; (i < len) && (p[i] != ';'); i++);
  if (i == len) {
    /* The last path segment has no parameter part */
    return NULL;
  }
  p += i + 1;
  plen = len - i - 1;
  if ((bs = NEWARRAY (BYTE, plen + 1)) == NULL) {
    return NULL;
  }
  B_COPYSTRINGN (bs, p, plen);
  bs[plen] = '\0';

  return bs;
}

/*
 * Extract the parameter component of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have any parameters.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetParameters (const WCHAR* pchUrl, WCHAR **param)
{
  URL  url;
  BYTE *bs = NULL;
  BOOL err = FALSE;
  BYTE *p;

  if ((pchUrl == NULL) || (param == NULL))
    return FALSE;
  *param = NULL;

  bs = wip_wchar2byte (pchUrl, &err);
  if ((bs == NULL) || err) {
    return FALSE;
  }
  if (!URL_Parse (bs, &url)) {
    DEALLOC (&bs);
    return FALSE;
  }
  p = URL_GetParameters (&url);
  if (p != NULL) {
    *param = wip_byte2wchar (p);
  }

  DEALLOC (&bs);
  DEALLOC (&p);

  return TRUE;
}

BOOL
b_GetParameters (const BYTE* pbUrl, BYTE **param)
{
  URL url;

  if ((pbUrl == NULL) || (param == NULL))
    return FALSE;
  *param = NULL;

  if (!URL_Parse ((BYTE*)pbUrl, &url)) {
    return FALSE;
  }
  *param = URL_GetParameters (&url);

  return TRUE;
}


/*
 * Return the query part of a URL.
 * Returns NULL in case of error, or if the URL does not have a query part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetQuery (URL *url)
{
  if (url == NULL)
    return NULL;

  return newstring (url->s[QUERY_PART], url->len[QUERY_PART]);
}

/*
 * Extract the query part of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a query part.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetQuery (const WCHAR* pchUrl, WCHAR **query)
{
  return w_GetPart (pchUrl, QUERY_PART, query);
}

BOOL
b_GetQuery (const BYTE* pbUrl, BYTE **query)
{
  return b_GetPart (pbUrl, QUERY_PART, query);
}


/*
 * Return the fragment part of a URL.
 * Returns NULL in case of error, or if the URL does not have a fragment part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetFragment (URL *url)
{
  if (url == NULL)
    return NULL;

  return newstring (url->s[FRAGMENT_PART], url->len[FRAGMENT_PART]);
}

/*
 * Extract the fragment part of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a fragment part.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetFragment (const WCHAR* pchUrl, WCHAR **frag)
{
  return w_GetPart (pchUrl, FRAGMENT_PART, frag);
}

BOOL
b_GetFragment (const BYTE* pbUrl, BYTE **frag)
{
  return b_GetPart (pbUrl, FRAGMENT_PART, frag);
}


/*
 * Other utility routines
 */


BYTE* b_EscapeURLString (const BYTE* pbString)
{
  const BYTE *p;
  BYTE       *q, *s;
  UINT16     l = 0;
  UINT16     r = 0;

  if (pbString == NULL)
    return NULL;

  for (p = pbString; *p; p++) {
    if((*p > 0x7f)||(*p==0x20))
      r++;
    else
      l++;
  }

  if ((s = NEWARRAY (BYTE, l + 3 * r + 1)) == NULL)
    return NULL;

  for (p = pbString, q = s; *p; p++) {
    if((*p > 0x7f)||(*p ==0x20)){
      *q++ = '%';
      ByteToHex (*p, q);
      q += 2;
    }
    else
      *q++ = *p;
  }
  *q = '\0';

  return s;
}


/*
 * Return a copy of 'pchString' where each character belonging to the set of
 * so called "special characters" or being in the range 0x80-0xff,
 * has been replaced by a hexadecimal esacape sequence of the form "%xy".
 * Returns NULL in case of error or if any character in the input
 * has a character code > 0xff.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR *
w_WMLS_EscapeString (const WCHAR* pchString)
{
  return w_EscapeString (pchString);
}

/*
 * Return a copy of 'pchString' where each character belonging to the set of
 * so called "special characters" or being in the range 0x80-0xff,
 * has been replaced by a hexadecimal esacape sequence of the form "%xy".
 * Returns NULL in case of error or if any character in the input
 * has a character code > 0xff.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR*
w_EscapeString (const WCHAR* pchString)
{
  const WCHAR *p;
  WCHAR       *q, *s;
  BYTE        tmp[2];
  UINT16      l = 0;
  UINT16      r = 0;

  if (pchString == NULL)
    return NULL;

  for (p = pchString; *p; p++) {
    BYTE b = (BYTE)(*p & 0xff);
    if (*p > 0xff) {
      return NULL;
    }
    if (wae_isspecial (b) || (b > 0x7f))
      r++;
    else
      l++;
  }

  if ((s = NEWARRAY (WCHAR, l + 3 * r + 1)) == NULL)
    return NULL;

  for (p = pchString, q = s; *p; p++) {
    BYTE b = (BYTE)(*p & 0xff);
    if (wae_isspecial (b) || (b > 0x7f)) {
      *q++ = '%';
      ByteToHex (b, tmp);
      *q++ = (WCHAR)tmp[0];
      *q++ = (WCHAR)tmp[1];
    }
    else
      *q++ = *p;
  }
  *q = '\0';

  return s;
}

/*
 * Return a copy of 'pbString' where each character belonging to the set of
 * so called "special characters" or being in the range 0x80-0xff,
 * has been replaced by a hexadecimal esacape sequence of the form "%xy".
 * Returns NULL in case of error.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
BYTE*
b_EscapeString (const BYTE* pbString)
{
  const BYTE *p;
  BYTE       *q, *s;
  UINT16     l = 0;
  UINT16     r = 0;

  if (pbString == NULL)
    return NULL;

  for (p = pbString; *p; p++) {
    if (wae_isspecial (*p) || (*p > 0x7f))
      r++;
    else
      l++;
  }

  if ((s = NEWARRAY (BYTE, l + 3 * r + 1)) == NULL)
    return NULL;

  for (p = pbString, q = s; *p; p++) {
    if (wae_isspecial (*p) || (*p > 0x7f)) {
      *q++ = '%';
      ByteToHex (*p, q);
      q += 2;
    }
    else
      *q++ = *p;
  }
  *q = '\0';

  return s;
}

/*
 * Return a copy of 'ws', where every 16-bit character has been
 * replaced by two escape sequences of the form "%xy", representing
 * the two bytes in the 16-bit character. The parts of each 16-bit
 * character are read in big-endian order.
 * Returns NULL in case of error.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
BYTE *
b_UnicodeEscape (WCHAR *ws)
{
  WCHAR  *wp;
  BYTE   *p, *bs;
  UINT16 len;

  if (ws == NULL)
    return NULL;

  len = STRINGLENGTH (ws);
  bs = NEWARRAY (BYTE, 6 * len + 1);
  if (bs == NULL)
    return NULL;

  p = bs;
  for (wp = ws; *wp; wp++) {
    BYTE b1 = (BYTE)((*wp >> 8) & 0xff);
    BYTE b2 = (BYTE)(*wp & 0xff);
    *p++ = '%';
    ByteToHex (b1, p);
    p += 2;
    *p++ = '%';
    ByteToHex (b2, p);
    p += 2;
  }
  *p = '\0';

  return bs;
}

/*
 * Return a copy of 'pchString' where each hexadecimal escape sequence
 * of the form "%xy" has been replaced with the character it represents.
 * Returns NULL if the original string contains non-ASCII-characters.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR *
w_WMLS_UnescapeString (const WCHAR* pchString)
{
  const WCHAR *p;
  WCHAR       *q, *s;
  BYTE        b;
  BYTE        tmp[2];

  if (pchString == NULL)
    return NULL;

  if ((s = NEWARRAY (WCHAR, STRINGLENGTH (pchString) + 1)) == NULL)
    return NULL;

  for (p = pchString, q = s; *p != 0;) {
    if (*p > 0x7f) {
      DEALLOC (&s);
      return NULL;
    }
    if (*p == '%') {
      if ((*(p + 1) > 0x7f) || (*(p + 2) > 0x7f)) {
        DEALLOC (&s);
        return NULL;
      }
      tmp[0] = (BYTE)*(p + 1);
      tmp[1] = (BYTE)*(p + 2);
      if (HexToByte (tmp, &b)) {
        *q++ = (WCHAR)b;
        p += 3;
        continue;
      }
    }
    *q++ = *p++;
  }
  *q = '\0';

  return s;
}


/*
 * Return a copy of 'pchString' where each hexadecimal escape sequence
 * of the form "%xy" has been replaced with the character it represents.
 * Returns NULL in case of error.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR*
w_UnescapeString (const WCHAR* pchString)
{
  const WCHAR *p;
  WCHAR       *q, *s;
  BYTE        b;
  BYTE        tmp[2];

  if (pchString == NULL)
    return NULL;

  if ((s = NEWARRAY (WCHAR, STRINGLENGTH (pchString) + 1)) == NULL)
    return NULL;

  for (p = pchString, q = s; *p != 0;) {
    if (*p == (WCHAR)'%') {
      if ((*(p + 1) < 0x80) && (*(p + 2) < 0x80)) {
        tmp[0] = (BYTE)*(p + 1);
        tmp[1] = (BYTE)*(p + 2);
        if (HexToByte (tmp, &b)) {
          *q++ = (WCHAR)b;
          p += 3;
          continue;
        }
      }
    }
    *q++ = *p++;
  }
  *q = '\0';

  return s;
}

/*
 * Return a copy of 'pbString' where each hexadecimal escape sequence
 * of the form "%xy" has been replaced with the character it represents.
 * Returns NULL in case of error.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
BYTE*
b_UnescapeString (const BYTE* pbString)
{
  BYTE *s = NULL;

  if ((pbString != NULL) &&
      ((s = NEWARRAY (BYTE, B_STRINGLENGTH ((const char *)pbString) + 1)) != NULL)) {
    UnescapeString (s, pbString);
  }

  return s;
}


/*
 * Copy the string "src" to the string "dst", while replacing
 * all escape sequences with the characters they represent.
 * Works correctly even if called with the same argument for src and dst.
 */
VOID
UnescapeString (BYTE *dst, const BYTE *src)
{
  BYTE b;

  if ((src == NULL) || (dst == NULL))
    return;

  while (*src) {
    if ((*src == '%') && HexToByte (src + 1, &b)) {
      *dst++ = b;
      src += 3;
    }
    else
      *dst++ = *src++;
  }
  *dst = '\0';
}


/**********************************************************************
 * Routines for access control.
 **********************************************************************/

/*
 * Check that the string 's1' is a componentwise suffix of string 's2'.
 * That is, if s1 = "def.com" and s2 = "abc.def.com", then return TRUE,
 * but if s1 = "c.def.com" return FALSE.
 */
static BOOL
DomainIsSuffix (BYTE *s1, UINT16 len1, BYTE *s2, UINT16 len2)
{
  INT16  i1, i2, k1, k2;
  UINT16 l1, l2;

  if (len1 == 0)
    return TRUE;

  if (len2 == 0)
    return FALSE;

  for (k1 = len1, k2 = len2; (k1 > 0) && (k2 > 0);) {
    /* Search backwards for next '.' */
    for (i1 = k1 - 1; (i1 >= 0) && (s1[i1] != '.'); i1--);
    for (i2 = k2 - 1; (i2 >= 0) && (s2[i2] != '.'); i2--);
    l1 = k1 - i1 - 1;
    l2 = k2 - i2 - 1;
    if (!CompareStrings (&s1[i1 + 1], l1, &s2[i2 + 1], l2, TRUE))
      return FALSE;
    k1 = i1;
    k2 = i2;
  }

  return (k1 <= 0);
}

/*
 * Check that the string 'a' is a componentwise prefix of string 'b'.
 * That is, if a = "/abc/def" and b = "/abc/def/jkl", then return TRUE,
 * but if a = "/abc/def/j" return FALSE.
 */
static BOOL
PathIsPrefix (BYTE *s1, UINT16 len1, BYTE *s2, UINT16 len2)
{
  INT16  i1, i2, k1, k2;
  UINT16 l1, l2;

  /* Handle special cases first: */
  if (len1 == 0)
    return TRUE;

  if (len2 == 0)
    return FALSE;

  if ((s1[0] != '/') || (s2[0] != '/'))
    return FALSE;

  /* General case: */
  for (k1 = 0, k2 = 0; (k1 < len1) && (k2 < len2);) {
    /* Search forward for next '/' */
    for (i1 = k1 + 1; (i1 < len1) && (s1[i1] != '/'); i1++);
    for (i2 = k2 + 1; (i2 < len2) && (s2[i2] != '/'); i2++);
    l1 = i1 - k1 - 1;
    l2 = i2 - k2 - 1;
    if ((l1 > 0) && !CompareStrings (&s1[k1 + 1], l1, &s2[k2 + 1], l2, FALSE))
      return FALSE;
    k1 = i1;
    k2 = i2;
  }

  return (k1 >= len1);
}

/*
 * Check if the given absolute URL has access rights,
 * given 'domain' and 'path' as access control attributes.
 *
 * Assumes that all three strings are zero-terminated BYTE strings,
 * and that 'abs_url' is a valid absolute URL.
 * Returns FALSE if either 'abs_url' or 'domain' is NULL.
 * If 'path' is NULL, the routine works as if it were the path "/".
 *
 * The check is performed by verifying that the domain attribute
 * is a component-wise suffix of the domain part of the absolute URL,
 * and that the path attribute is a component-wise prefix of
 * the path part of the absolute URL. Comparisons of the paths
 * are case-sensitive, but comparisons of the domain components are not.
 * Handles escape sequences ("%xy") correctly.
 */
BOOL
URL_CheckAccess (BYTE *abs_url, BYTE *domain, BYTE *path)
{
  URL url;

  if ((abs_url == NULL) || (domain == NULL))
    return FALSE;

  if (path == NULL)
    path = (BYTE *)"/";

  if (!URL_FromByteString (abs_url, &url))
    return FALSE;

  return
    (url.s[HOST_PART] != NULL) &&
    (url.len[PATH_PART] > 0) &&
    DomainIsSuffix (domain, B_STRINGLENGTH ((const char *)domain),
                    url.s[HOST_PART], url.len[HOST_PART]) &&
    PathIsPrefix (path, B_STRINGLENGTH ((const char *)path), url.s[PATH_PART],
                  url.len[PATH_PART]);
}


/*
 * Take an incomplete URL, like "www.abc.com", and turn it into
 * a correct absolute URL using heuristic methods. This is not a
 * well-defined process, rather it makes a guess as to what the user means.
 * In the example above, the result would be "http://www.abc.com/".
 * In case of failure, NULL is returned.
 * NOTE: it is the caller's responsibility to deallocate the returned string.
 */
BYTE *
b_CompleteURLHeuristically (BYTE *bs)
{
  URL    url, new_url;
  BYTE   *p;

  if (bs == NULL)
    return NULL;

  if (!URL_FromByteString (bs, &url))
    return NULL;

  new_url = url;

  /* The original URL has a scheme different from http(s); we simply
   * return a copy of the input string. */
  if ((url.s[SCHEME_PART] != NULL) &&
      (url.scheme_type != Scheme_http) &&
      (url.scheme_type != Scheme_https)) {
    return B_STRDUP (bs);
  }

  /* The original URL does not have a scheme; we attach 'http:' */
  if (url.s[SCHEME_PART] == NULL) {
    new_url.s[SCHEME_PART] = (BYTE *)"http";
    new_url.len[SCHEME_PART] = 4;
    new_url.scheme_type = Scheme_http;
  }

  /* The original URL does not have an authority part (starting with '//');
   * we promote the initial segment of the path, up to (but not including)
   * the first '/'-character, or the whole path in case it has no such
   * delimiter. If the path is an absolute path (starting with '/'),
   * we cannot guess what the host should have been. */
  if (url.s[AUTHORITY_PART] == NULL) {
    if (url.len[PATH_PART] == 0)
      return NULL;
    p = (BYTE *)strchr ((const char *)url.s[PATH_PART], '/');
    if (p == NULL) {
      new_url.s[AUTHORITY_PART] = new_url.s[HOST_PART] = url.s[PATH_PART];
      new_url.len[AUTHORITY_PART] = new_url.len[HOST_PART]
        = url.len[PATH_PART];
      new_url.len[PATH_PART] = 0;
    }
    else if (p != url.s[PATH_PART]) {
      new_url.s[AUTHORITY_PART] = new_url.s[HOST_PART] = url.s[PATH_PART];
      new_url.len[AUTHORITY_PART] = new_url.len[HOST_PART]
        = (UINT16)(p - url.s[PATH_PART]);
      new_url.s[PATH_PART] = p;
      new_url.len[PATH_PART] = (UINT16)(url.len[PATH_PART] -
                                        new_url.len[HOST_PART]);
    }
    else {
      /* The URL is simply an absolute path, we cannot deduce the host! */
      return NULL;
    }
  }
  
  /* The URL (original or modified as above), which is now a http(s) URL,
   * has no path part; we attach '/', the root path. */
  if (new_url.len[PATH_PART] == 0) {
    new_url.s[PATH_PART] = (BYTE *)"/";
    new_url.len[PATH_PART] = 1;
  }

  /* Allocate a new buffer and copy all the parts to it. */
  return AssembleURLString (&new_url);
}

/*
 * Given two URLs, compute the minimum relative path, i.e., the shortest
 * relative path that together with 'bs1' can be resolved to yield
 * 'bs2'. If 'includeFragment' is TRUE any fragment part in 'bs2'
 * is included in the result, otherwise it is dropped.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned string.
 */
BYTE *
b_MinRelativePath (BYTE *bs1, BYTE *bs2, BOOL includeFragment)
{
  URL    url1, url2;
  BYTE *s1, *s2, *buf, *p, *res;
  UINT16 len, len1, len2, l1, l2, n;
  INT16 i, i1, i2, k1, k2;

  if (!URL_FromByteString (bs1, &url1) ||
      !URL_FromByteString (bs2, &url2))
    return NULL;
  
  if (!URL_Equal (&url1, &url2, SCHEME_COMP | HOST_COMP | PORT_COMP)) {
    return URL_ToByteString (&url2);
  }
  s1 = url1.s[PATH_PART];
  s2 = url2.s[PATH_PART];
  if ((s1 == NULL) || (s2 == NULL))
    return NULL;

  len1 = url1.len[PATH_PART];
  len2 = url2.len[PATH_PART];
  if ((len1 == 0) || (len2 == 0))
    return NULL;
  if ((s1[0] != '/') || (s2[0] != '/'))
    return NULL;

  /* Drop everything after the last '/' in path 1. */
  for (i1 = len1 - 1; i1 > 0; i1--) {
    if (s1[i1] == '/')
      break;
  }
  len1 = i1 + 1;

  /* Now, path 1 ends with a '/'. Search past the initial, equal,
   * path segments in the two paths. A path segment starts right
   * after a '/' and ends on the next '/'. */
  for (k1 = 1, k2 = 1; (k1 < len1) && (k2 < len2);) {
    /* Here, k1 is the index following the last seen '/',
     * and i1 is moved  up to the next '/', or the end of the string. */
    for (i1 = k1; (i1 < len1) && (s1[i1] != '/'); i1++);
    for (i2 = k2; (i2 < len2) && (s2[i2] != '/'); i2++);
    l1 = i1 - k1;
    l2 = i2 - k2;
    if (!((l1 == l2) &&
          CompareStrings (&s1[k1], l1, &s2[k2], l2, FALSE))) {
      break;
    }
    k1 = i1 + 1;
    k2 = i2 + 1;
  }

  /* Count how many more '/' there are in path 1. */
  n = 0;
  for (i1 = k1; i1 < len1; i1++) {
    if (s1[i1] == '/')
      n++;
  }

  /* The path we create will consist of what ever part was left
   * in path 2 (i.e., that did not match a corresponding part in
   * path 1), plus n "../" segments. Note, that we might end up
   * with a string of length 0 here.
   * Special case: if the paths differ already in the first
   * segment, then just use the absolute path from url2. */
  if (k1 == 1) {
    len = len2;
    if ((buf = NEWARRAY (BYTE, len + 1)) == NULL) {
      return NULL;
    }
    p = buf;
    B_COPYSTRINGN (p, s2, len2);
    p += len2;
  }
  else {
    len = n * 3 + len2 - k2;
    if ((buf = NEWARRAY (BYTE, len + 1)) == NULL) {
      return NULL;
    }
    p = buf;
    for (i = 0; i < n; i++) {
      B_COPYSTRINGN (p, "../", 3);
      p += 3;
    }
    if (len2 > k2) {
      B_COPYSTRINGN (p, &s2[k2], len2 - k2);
      p += len2 - k2;
    }
  }
  *p = '\0';

  url2.s[PATH_PART] = buf;
  url2.len[PATH_PART] = len;
  url2.s[AUTHORITY_PART] = url2.s[SCHEME_PART]
    = url2.s[HOST_PART] = url2.s[PORT_PART] = NULL;
  url2.len[AUTHORITY_PART] = url2.len[SCHEME_PART]
    = url2.len[HOST_PART] = url2.len[PORT_PART] = 0;
  url2.scheme_type = Scheme_empty;

  if (!includeFragment) {
    url2.s[FRAGMENT_PART] = NULL;
    url2.len[FRAGMENT_PART] = 0;
  }

  res = AssembleURLString (&url2);
  DEALLOC (&buf);

  return res;
}

/*
 * Check that a URL is valid and has at least a scheme, host, and path
 * component.
 */
BOOL
b_UrlIsCompleteValid (const BYTE* pbUrl)
{
  URL url;

  return
    URL_Parse ((BYTE *)pbUrl, &url) &&
    (url.s[SCHEME_PART] != NULL) &&
    (url.s[HOST_PART] != NULL) &&
    (url.len[PATH_PART] > 0);
}


/*
 * Return a URL that is a copy of 'old_url', but with 'new_query'
 * appended to the query part. If 'old_url' already has a query part,
 * that old part will be separated from the new by a '&'-character.
 * Returns NULL in case of error.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BYTE *
b_AppendToQuery (BYTE *old_url, BYTE *new_query)
{
  URL    url;
  UINT16 nqlen;
  BYTE   *new_url, *tmp = NULL;

  if (old_url == NULL)
    return NULL;

  if ((new_query == NULL) || ((nqlen = B_STRINGLENGTH ((const char *)new_query)) == 0))
    return B_STRDUP (old_url);

  if (!URL_FromByteString (old_url, &url))
    return NULL;

  if (url.s[QUERY_PART] == NULL) {
    url.s[QUERY_PART] = new_query;
    url.len[QUERY_PART] = nqlen;
  }
  else {
    if ((tmp = NEWARRAY (BYTE, url.len[QUERY_PART] + nqlen + 2)) == NULL) {
      return NULL;
    }
    B_COPYSTRING ((char *)tmp, (const char *)url.s[QUERY_PART]);
    tmp[url.len[QUERY_PART]] = '&';
    B_COPYSTRING ((char *)tmp + url.len[QUERY_PART] + 1, (const char *)new_query);
    url.s[QUERY_PART] = tmp;
    url.len[QUERY_PART] += nqlen + 1;
  }

  new_url = AssembleURLString (&url);
  if (tmp != NULL) {
    DEALLOC (&tmp);
  }

  return new_url;
}

/*
 * Return the longest componentwise common prefix of two URL paths.
 * Returns NULL in case of error.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BYTE *
b_LongestCommonPrefix (BYTE *s1, BYTE *s2)
{
  UINT16 len1, len2, l1, l2;
  INT16 i1, i2, k1, k2;
  BYTE   *p;

  if ((s1 == NULL) || (s2 == NULL))
    return NULL;

  len1 = B_STRINGLENGTH ((const char *)s1);
  len2 = B_STRINGLENGTH ((const char *)s2);
  if ((len1 == 0) || (len2 == 0))
    return NULL;
  if ((s1[0] != '/') || (s2[0] != '/'))
    return NULL;

  for (k1 = 0, k2 = 0; (k1 < len1) && (k2 < len2);) {
    /* Search forward for next '/' */
    for (i1 = k1 + 1; (i1 < len1) && (s1[i1] != '/'); i1++);
    for (i2 = k2 + 1; (i2 < len2) && (s2[i2] != '/'); i2++);
    l1 = i1 - k1 - 1;
    l2 = i2 - k2 - 1;
    if (!CompareStrings (&s1[k1 + 1], l1, &s2[k2 + 1], l2, FALSE))
      break;
    k1 = i1;
    k2 = i2;
  }
  if (k1 == 0)
    k1 = 1;

  if ((p = NEWARRAY (BYTE, k1 + 1)) == NULL) {
    return NULL;
  }
  B_COPYSTRINGN (p, s1, k1);
  p[k1] = '\0';

  return p;
}

/*
 * Check if the URL "path1" is a prefix of "path2".
 * The prefix match is done according to the rules in section 7.4
 * in "WAP Cache Operation Specification".
 */
BOOL
b_IsPrefix (BYTE *path1, BYTE *path2)
{
  URL url1, url2;

  if (!URL_FromByteString (path1, &url1) ||
      !URL_FromByteString (path2, &url2)) {
    return FALSE;
  }
  
  if (!URL_Equal (&url1, &url2, SCHEME_COMP)) {
    return FALSE;
  }
  if ((url1.s[AUTHORITY_PART] == NULL) &&
      (url2.s[AUTHORITY_PART] == NULL) &&
      (url1.len[PATH_PART] > 0) &&
      (url2.len[PATH_PART] > 0) &&
      ((url1.s[PATH_PART])[0] != '/') &&
      ((url2.s[PATH_PART])[0] != '/')) {
    /* Both URLs are of the "opaque" type. */
    return URL_Equal (&url1, &url2, PATH_COMP);
  }
      
  /* For non-opaque URIs, the authority parts must be equal. */
  if (!URL_Equal (&url1, &url2, USERINFO_COMP | HOST_COMP | PORT_COMP)) {
    return FALSE;
  }

  /* An empty path is the prefix on any other path. */
  if (url1.len[PATH_PART] == 0) {
    return TRUE;
  }

  /* Check that path1 is a segment-wise prefix of path2. */
  return PathIsPrefix (url1.s[PATH_PART], url1.len[PATH_PART],
                       url2.s[PATH_PART], url2.len[PATH_PART]);
}

BYTE* EscapeALLString (const BYTE* pbString, UINT16 iLen)
{
  const BYTE *p;
  BYTE       *q, *s;
  UINT16     l = 0;
  UINT16     r = 0;
  UINT16    i=0;

  if (pbString == NULL || iLen==0)
    return NULL;

  for (p = pbString; i<iLen; i++, p++) {
    if (!wae_isalphanum(*p))
      r++;
    else
      l++;
  }

  if ((s = NEWARRAY (BYTE, l + 3 * r + 1)) == NULL)
    return NULL;
   i=0;
  for (p = pbString, q = s; i<iLen; p++,i++) {
    if (wae_isspecial (*p) || (*p > 0x7f)) {
      *q++ = '%';
      ByteToHex (*p, q);
      q += 2;
    }
    else
      *q++ = *p;
  }
  *q = '\0';

  return s;
}
