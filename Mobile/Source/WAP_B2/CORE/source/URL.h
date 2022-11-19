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
 * URL.h
 *
 * Library of routines for handling URLs.
 *
 * Created by Anders Edenbrandt, Mon Mar 29 16:50:15 1999.
 *
 * Revision history:
 *   991123, AED: added new function, b_IsPrefix.
 *   991123, AED: parsing all URIs as generic
 *
 */

#ifndef _URL_h
#define _URL_h

#include "CmmnRsrc.h"

#define NO_COMP       0
#define SCHEME_COMP   0x1
#define USERINFO_COMP 0x2
#define HOST_COMP     0x4
#define PORT_COMP     0x8
#define AUTHORITY_COMP USERINFO_COMP | HOST_COMP | PORT_COMP
#define PATH_COMP     0x10
#define QUERY_COMP    0x40
#define FRAG_COMP     0x80
#define ALL_COMP      0xFF

/* The Scheme type */
typedef enum SchemeEnum {
	Scheme_empty = 0,
	Scheme_http = 1,
	Scheme_https = 2,
	Scheme_file = 3,
	Scheme_wtai = 4,
	Scheme_about = 5,
	Scheme_function = 6,
	Scheme_wapdevice = 7,
	Scheme_unsupported = 90,
	Scheme_unknown = 99
} Scheme;


#define NUM_URL_PARTS  8

#define SCHEME_PART    0
#define AUTHORITY_PART 1
#define PATH_PART      2
#define QUERY_PART     3
#define FRAGMENT_PART  4

#define USERINFO_PART  5
#define HOST_PART      6
#define PORT_PART      7

/* The URL type */
typedef struct URL_s {
  Scheme scheme_type;
  BYTE   *s[NUM_URL_PARTS];
  UINT16 len[NUM_URL_PARTS];
} URL;


/*
 * Return the scheme type named by the given string. Returns Scheme_unknown
 * if its not one of the predefined types.
 */
Scheme
Scheme_FromString (BYTE *sch);

/*
 * Return a string representation of the Scheme value.
 * NOTE: the caller must NOT modify or deallocate the returned string!
 */
const BYTE *
Scheme_ToString (Scheme scheme);

/*
 * Sets all fields in the URL struct to NULL and 0, respectively.
 */
void
URL_Clear (URL *url);

/*
 * Compute a hash value from a URL and store it in the location pointed
 * to by "hv". All equivalent URLs will hash to the same value,
 * but two non-equal URLs may also have the same hash value. However,
 * the probability of a collision is small.
 * Returns FALSE on error, TRUE otherwise.
 */
BOOL
b_HashURL (const BYTE *bs, UINT32 *hv);

/*
 * Take a string representation of a URL and parse it into its
 * components, and store these as fields in the given URL struct.
 * All components are stored in their original (possibly escaped) form.
 * Returns TRUE if the URL was valid, FALSE otherwise. In the latter
 * case, nothing is stored in the URL struct.
 */
BOOL
URL_FromWideString (const WCHAR *ws, URL *url);
BOOL
URL_FromByteString (const BYTE *bs, URL *url);

/*
 * Given a URL struct, construct and return a string representation
 * of the URL.
 * Returns NULL in case of error.
 * NOTE: It is the callers responsibility to deallocate the returned string.
 */
WCHAR *
URL_ToWideString (URL *url);
BYTE *
URL_ToByteString (URL *url);

/*
 * Given a base URL and a relative URL, resolve the relative reference
 * and store as an absolute URL in the string "*abs".
 * Returns TRUE on success, FALSE otherwise, in which case nothing
 * is stored in "abs".
 * NOTE: It is the callers responsibility to deallocate the returned string.
 */
BOOL
URL_Resolve (URL *base, URL *rel, BYTE **abs);

BOOL
w_Resolve (const WCHAR *base, const WCHAR *rel, WCHAR **abs);
BOOL
b_Resolve (const BYTE *base, const BYTE *rel, BYTE **abs);

/*
 * Return TRUE if the two URLs are equal, FALSE otherwise.
 * "whichComponents" is a bitmap indicating which parts of the URLs
 * should be included in the comparison.
 * Returns FALSE in case of error.
 */
BOOL
URL_Equal (URL *url1, URL *url2, BYTE whichComponents);

BOOL
w_EqualURL (const WCHAR *url1, const WCHAR *url2, BYTE whichComponents);
BOOL
b_EqualURL (const BYTE *url1, const BYTE *url2, BYTE whichComponents);

/*
 * Return TRUE if the given string URL has a valid format, FALSE otherwise.
 */
BOOL
w_IsValid (const WCHAR* pchUrl);
BOOL
b_IsValid (const BYTE* pbUrl);


/************************************************************
 * Retrieval of the different parts of a URL.
 ************************************************************/

/*
 * Return the Scheme of the URL.
 * Returns NULL in case of error, or if the URL does not have a scheme part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetScheme (URL *url);
Scheme
URL_GetSchemeType (URL *url);

/*
 * Extract the scheme of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a scheme component.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string (applies to w_GetScheme and b_GetScheme).
 */
BOOL
w_GetScheme (const WCHAR* pchUrl, WCHAR **scheme);
BOOL
b_GetScheme (const BYTE* pbUrl, BYTE **scheme);
BOOL
w_GetSchemeType (const WCHAR* pchUrl, Scheme *scheme);
BOOL
b_GetSchemeType (const BYTE* pbUrl, Scheme *scheme);

/*
 * Return the host part of a URL.
 * Returns NULL in case of error, or if the URL does not have a host part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetHost (URL *url);

/*
 * Extract the host part of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a host component.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetHost (const WCHAR* pchUrl, WCHAR **host);
BOOL
b_GetHost (const BYTE* pchUrl, BYTE **host);

/*
 * Return the port number of a URL.
 * Returns NULL in case of error, or if the URL does not have a port number.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetPort (URL *url);

/*
 * Extract the port number of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a port number.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetPort (const WCHAR* pchUrl, WCHAR **port);
BOOL
b_GetPort (const BYTE* pbUrl, BYTE **port);

/*
 * Return the path component of a URL.
 * Returns NULL in case of error, or if the URL does not
 * have a path component.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetPath (URL *url);

/*
 * Extract the path component of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a path component.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetPath (const WCHAR* pchUrl, WCHAR **path);
BOOL
b_GetPath (const BYTE* pbUrl, BYTE **path);

/*
 * Return the parameter component of a URL.
 * Returns NULL in case of error, or if the URL does not have any parameters.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetParameters (URL *url);

/*
 * Extract the parameter component of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have any parameters.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetParameters (const WCHAR* pchUrl, WCHAR **param);
BOOL
b_GetParameters (const BYTE* pbUrl, BYTE **param);

/*
 * Return the query part of a URL.
 * Returns NULL in case of error, or if the URL does not have a query part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetQuery (URL *url);

/*
 * Extract the query part of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a query part.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetQuery (const WCHAR* pchUrl, WCHAR **query);
BOOL
b_GetQuery (const BYTE* pbUrl, BYTE **query);

/*
 * Return the fragment part of a URL.
 * Returns NULL in case of error, or if the URL does not have a fragment part.
 * NOTE: it is the responsibility of the caller to deallocate the string.
 */
BYTE *
URL_GetFragment (URL *url);

/*
 * Extract the fragment part of a URL.
 * Returns FALSE in case of error, including that the URL is not valid.
 * Sets the out-parameter to NULL if the URL does not have a fragment part.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BOOL
w_GetFragment (const WCHAR* pchUrl, WCHAR **frag);
BOOL
b_GetFragment (const BYTE* pbUrl, BYTE **frag);


/************************************************************
 * Other utility routines
 ************************************************************/

/*
 * Return a copy of 'pchString' where each character belonging to the set of
 * so called "special characters" or being in the range 0x80-0xff,
 * has been replaced by a hexadecimal esacape sequence of the form "%xy".
 * Returns NULL in case of error or if any character in the input
 * has a character code > 0xff.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR *
w_WMLS_EscapeString (const WCHAR* pchString);

/*
 * Return a copy of 'pchString' where each character belonging to the set of
 * so called "special characters" or being in the range 0x80-0xff,
 * has been replaced by a hexadecimal esacape sequence of the form "%xy".
 * Returns NULL in case of error or if any character in the input
 * has a character code > 0xff.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR *
w_EscapeString (const WCHAR* pchString);
BYTE *
b_EscapeString (const BYTE* pbString);

/*
 * Return a copy of 'ws', where every 16-bit character has been
 * replaced by two escape sequences of the form "%xy", representing
 * the two bytes in the 16-bit character. The parts of each 16-bit
 * character are read in big-endian order.
 * Returns NULL in case of error.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
BYTE *
b_UnicodeEscape (WCHAR *ws);

/*
 * Return a copy of 'pchString' where each hexadecimal escape sequence
 * of the form "%xy" has been replaced with the character it represents.
 * Returns NULL if the original string contains non-ASCII-characters.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR *
w_WMLS_UnescapeString (const WCHAR* pchString);

/*
 * Return a copy of 'pchString' where each hexadecimal escape sequence
 * of the form "%xy" has been replaced with the character it represents.
 * Returns NULL in case of error.
 * NOTE: it is the callers responsibility to deallocate the returned string.
 */
WCHAR *
w_UnescapeString (const WCHAR* pchString);
BYTE *
b_UnescapeString (const BYTE* pbString);

/*
 * Copy the string "src" to the string "dst", while replacing
 * all escape sequences with the characters they represent.
 * Works correctly even if called with the same argument for src and dst.
 */
VOID
UnescapeString (BYTE *dst, const BYTE *src);

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
URL_CheckAccess (BYTE *abs_url, BYTE *domain, BYTE *path);

/*
 * Take an incomplete URL, like "www.abc.com", and turn it into
 * a correct absolute URL using heuristic methods. This is not a
 * well-defined process, rather it makes a guess as to what the user means.
 * In the example above, the result would be "http://www.abc.com/".
 * In case of failure, NULL is returned.
 * NOTE: it is the caller's responsibility to deallocate the returned string.
 */
BYTE *
b_CompleteURLHeuristically (BYTE *bs);

/*
 * Given two URLs, compute the minimum relative path, i.e., the shortest
 * relative path that together with 'fromUrl' can be resolved to yield
 * 'toUrl'. If 'includeFragment' is TRUE any fragment part in 'toUrl'
 * is included in the result, otherwise it is dropped.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned string.
 */
BYTE *
b_MinRelativePath (BYTE *fromUrl, BYTE *toUrl, BOOL includeFragment);

/*
 * Check that a URL is valid and has at least a scheme, host, and path
 * component.
 */
BOOL
b_UrlIsCompleteValid (const BYTE* pbUrl);

/*
 * Return a URL that is a copy of 'old_url', but with 'new_query'
 * appended to the query part. If 'old_url' already has a query part,
 * that old part will be separated from the new by a '&'-character.
 * Returns NULL in case of error.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BYTE *
b_AppendToQuery (BYTE *old_url, BYTE *new_query);

/*
 * Return the longest componentwise common prefix of two URL paths.
 * Returns NULL in case of error.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned string.
 */
BYTE *
b_LongestCommonPrefix (BYTE *path1, BYTE *path2);

/*
 * Check if the URL "path1" is a prefix of "path2".
 * The prefix match is done according to the rules in section 7.4
 * in "WAP Cache Operation Specification".
 */
BOOL
b_IsPrefix (BYTE *path1, BYTE *path2);

#endif
