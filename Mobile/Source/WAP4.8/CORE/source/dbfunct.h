/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000, 2001.
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
 * dbFunct.h
 *
 * Data channel handling.
 * Store and find authentication information in database.
 * Store and find cookies.
 *
 * This file contains various functions with two things in common:
 * they both use wipdb database and they access internal
 * structures in the database.
 *
 *
 * Created by Niclas Kellgren 000904.
 *
 * Revision history:
 *   000914, NKE: data channel code moved from wipdb.h
 *   001004, NKE: host and proxy authentication added
 *   001019, NKE: host and proxy api changed
 *   001117, NKE: documentation
 *   010122, NKE: added constants for configuration variable names
 *   010123, NKE: adding missing symbolic constant CREDENTIALS
 *   010130, NKE: changed the declaration of addHost and deleteHost 
 *   010130, NKE: added getFirstConfiguredObject
 *   010208, RMZ: added functions for cookies
 *   010315, NKE: added COOKIES_ENABLED constant
 *   010410, IPN: added MASTER_GW, STARTPAGE, PROXY_PROVIDER_ID constants
 *   010514, NKE: added setPersistentAuthStatus
 */

#ifndef _DBFUNCT_H
#define _DBFUNCT_H

#include "tapimmi.h"
#include "aapiclnt.h"
#include "cmmnrsrc.h"
#include "wipdb.h"


/*
 * DATA CHANNEL HANDLING
 *
 * This file contains among other things a collection of functions
 * handling data channels. Always use specialized function if 
 * available. I.e. deleting a object record with deleteItem instead
 * of	deleteObject may cause a corrupt database and a system crash!
 *
 *
 * HOST AND PROXY AUTHENTICATION
 *
 * The authentication functions do not use external structures, but
 * is dependent on pointers to elements. The database records
 * created by the code must not be deleted or manipulated by 
 * ordinary database operations.
 *
 * The structures are saved to persistent memory. This makes
 * operations that change data slow. Searching though, is done
 * entirely in RAM menory and is fast.
 */



 
/*************************************************************/
/******************  data channel handling  ******************/
/*************************************************************/


/*************
 * Constants *
 *************/

#define HISTORY_SIZE         "hstSz"
#define CACHE_AGE            "cAge"
#define CACHE_MODE           "cMode"
#define DISPLAY_IMAGES       "dsplI"
#define UPDATE_IMAGES        "updtI"
#define PUSH_SECURITY_LEVEL  "pshSL"
#define WSP_LANGUAGE         "wsp l"
#define PROFILE              "profl"
#define PROFILE_DIFF         "pDiff"
#define USER_AGENT           "ua"
#define ACCESS_TYPE          "accTp"
#define ONLINE               "onln"
#define CLIENT_LOCAL_PORT    "cltLP"
#define USSD_GW_TYPE         "usdGT"
//redefinition conflict
#ifdef FEATURE_WAP
#define WAP_TIMEOUT          "tmout"
#else
#define TIMEOUT              "tmout"
#endif //#ifdef FEATURE_WAP

#define STACKMODE            "stckm"
#define CHANNEL_EXPIRES      "cExp"
#define MASTER_GW            "mast"
#define STARTPAGE            "stPa"
#define PROXY_PROVIDER_ID    "prPrI"
#define ALLOW_WTLS_CLASS_1   "wtls1"
#define UDP_IP_SRC           "udpIS"
#define UDP_IP_GW            "udpIG"
#define SMS_C                "smsC"
#define SMS_GW               "smsGW"
#define USSD_C               "usdC"
#define USSD_GW              "usdGW"
#define CREDENTIALS          "cred"
#define COOKIES_ENABLED      "cEnbl"

/* cookie component keys */
#define COOKIE_LIST          "coo"
#define COOKIE_NAME          "name"
#define COOKIE_VALUE         "val"
#define COOKIE_DOMAIN        "dom"
#define COOKIE_PATH          "path"
#define COOKIE_EXPIRES       "exp"
#define COOKIE_VERSION       "ver"


/* 
 * Default values for fields automatically added can be found in
 * addDataChannel() or dchInit().
 */

enum {
	DB_maxNbrOfObjects = 10 + 1, /* Upper limit for number of concurrent objects plus one template */
	DB_maxNbrOfDataChannels = 7  /* Upper limit for number of concurrent data channels */
};


/*************
 * Functions *
 *************/

/*
 * GET REFERENCE
 *
 * Return a reference to the record associated with 'object'
 * and 'channel'.
*/
DB_ref getObjectRef(UINT8 object);
DB_ref getChannelRef(UINT8 object, UINT8 channel);


/*
 * ADD/DELETE OBJECT
 *
 * addObject adds an object with number 'object' if it does not
 * exist. The function returns a reference to the new object
 * record.
 *
 * removeObject removes an object with number 'object' and all
 * its data.
 */
DB_ref addObject(INT8 object);
void   deleteObject(UINT8 object);


/*
 * SEARCH FOR CONFIGURED OBJECT
 *
 * Search for an object not connected to a push, WTA, template or
 * content user agent. The number of first one found with at least 
 * one configured data channel is returned. If no matching object
 * is found, '*errorCode' is set to DB_err_objectNotFound, else
 * DB_err_success.
 */
UINT8 getFirstConfiguredObject(UINT8 *errorCode);


/*
 * DATA CHANNEL
 *
 * addDataChannel creates a new channel record and an new object
 * record if nessesary.	It returns a reference to the new channel
 * record.
 *
 * deleteChannel removes the channel object if found. If the
 * default channel is deleted, then a call to getDataChannel
 * that does not result in a match will cause an error code to
 * be returned.
 *
 * getDataChannel searches through all host addresses connected
 * to the channels in an object. The channel number belonging
 * to the best matching host is returned. If there is no match,
 * then if set, the default channel number is returned, or else
 * an error occurs. Assumes the url to be well formatted, including
 * a scheme, host and optionally a path. The status of the
 * function at return is signaled through '*errorCode'.
 *
 * The host address connected to a channel contains a host name
 * which may be followed by a path name.
 *
 * getDataChannel matches url:s with host addresses as sepcified
 * in the WAP End to End Security Specification (section 10.3.1).
 * During getDataChannel it is checked if channels for the specified
 * object have expired. If any expired channels are found the outparam
 * expired is set, otherwise it is false. The id of any expired channels
 * for the object is remembered in an internal set (list). These channel
 * id:s may be retrieved by calling getNextExpired. Note, the internal set
 * of expired channel id:s is reset for each call of getDataChannel.
 *
 * setDefaultDataChannel changes the data channel to be used
 * when no host matches when calling getDataChannel. An object
 * and a data channel are created if either is missing.
 *
 * getNextExpired iterates on the internal set of expired channels.
 * The internal set is reset when calling getDataChannel. getNextExpired
 * returns TRUE if outparam channelId is valid, i.e. containing the next
 * channel id in the iteration. getNextExpired returns FALSE when the
 * iteration ends and then the outparam channelId is not valid.
 * The channelId param must not be NULL.
*/
DB_ref addDataChannel(UINT8 object, UINT8 channel);
void   deleteDataChannel(UINT8 object, UINT8 channel);
UINT8  getDataChannel(UINT8 object, const BYTE* url,
                      BOOL* expired, UINT8 *errorCode);
void   setDefaultDataChannel(UINT8 object, UINT8 channel);
BOOL   getNextExpired(UINT8* channelId);

/*
 * ADD/DELETE HOST
 *
 * Add/delete host address and optionally path. 
 *
 * When calling these functions, the string pointed out by 'host'
 * is not taken over by the database.
 *
 * The string pointed out by 'host' must include a host name which
 * may be followed by a path name. The first '/' delimits the host from
 * the path. The host name may be the empty string.
 *
 * 'length' is the length of host including any path but excluding an  
 * optional terminating EOL.
 *
 * Calling deleteHost, the 'host'
 * string must match the host and any path to be deleted.
 * When matching, the host part is compared case insensitively while any path
 * part is compared case sensitively.
 */
void addHost   (UINT8 object, UINT8 channel, const BYTE* host, UINT16 length);
void deleteHost(UINT8 object, UINT8 channel, const BYTE* host, UINT16 length);


/*
    --------------
    Administration
    --------------
*/

/*
 * INIT/TERMINATE
 *
 * To be called at start-up or close-down.
 */
void dataChannelInit(void);
void dataChannelTerminate(void);






/*************************************************************/
/**********************  authenticate  ***********************/
/*************************************************************/


/*************
 * Functions *
 *************/

/*
    -------------------
    Host authentication
    -------------------
*/

/*
 *	STORE HOST CREDENTIALS
 *
 * Store the host of 'url' together with 'realm' and 'credientials'.
 * If already cfg_wae_wspif_AuthenticationItems are present, the
 * least reasently used element is replaced by the new one.
 *
 * If host and 'realm' matches an item, then the credentials are
 * uppdated to 'credentials'. The host and 'realm' are text
 * strings and are matched agaist the database case insensitivily.
 *
 * Return TRUE if successfull.
 */
BOOL storeHostAuth(BYTE* url, BYTE* realm, BYTE*  credentials, UINT8 credLength);


/*
 * CHECK HOST CREDENTIALS
 *
 * Search the database and return the credentials connected to
 * the host of 'url' and 'realm'.
 *
 * This function is called in two cases: before a http request
 * and when a request results in a challange for authenification.
 * In the first case, checkHostAuth is called with 'realm' set
 * to NULL and the matching is done based on only the URL. Even
 * if the item exists in the database, a mismatch may occur.
 * This can happen if several items with identical host are in
 * the database. A mismatch will result in a challenge containing
 * the correct realm and the correct credentials can be found.
 *
 * '*credentials' is a pointer to the corresponding credentials.
 * This is a pointer to the database item and not to a copy and
 * and may not be freed. 
 *
 * The host of 'url' and 'realm' are text strings and are matched
 * agaist the database case insensitivily.
 *
 * Return TRUE if found a match.
 */
BOOL checkHostAuth(BYTE* url, BYTE* realm, BYTE* *credentials);


/*
 * DELETE HOST CREDENTIALS
 *
 *	Remove all host authentication data.
 */
void deleteHostAuth(void);


/*
    --------------------
    Proxy authentication
    --------------------
*/

/*
 *	STORE PROXY CREDENTIALS
 *
 *	Store 'address' and 'portNumber' together with 'credentials'.
 * If already cfg_wae_wspif_AuthenticationItems are present, the
 * least recently added element is replaced by the new one.
 * 
 * If 'adress' and 'portNumber' matches an item, then the
 * credentials are uppdated to 'credentials'. 'address' can
 * contain any data and is only an exact match counts when
 * comparing addresses.
 *
 * 'address' can contain any data and is only an exact match counts
 * when comparing addresses.
 *
 * Return TRUE if successfull.
 */
BOOL storeProxyAuth(BYTE* address, UINT8 addressLength, UINT16 portNbr,
                    BYTE* credentials, UINT8 credLength);


/*
 * CHECK PROXY CREDENTIALS
 *
 * Search the database and return the credentials connected to
 * the host of 'url' and 'realm'.
 *
 * '*credentials' is a pointer to the corresponding credentials.
 * This is a pointer to the database item and not to a copy and
 * and may not be freed. 
 *
 * 'address' can contain any data and is only an exact match counts
 * when comparing addresses.
 *
 *	Return TRUE if found a match.
 */
BOOL checkProxyAuth(BYTE* address, UINT8 addressLength, UINT16 portNbr, BYTE* *credentials);


/*
 * DELETE PROXY CREDENTIALS
 *
 * Remove all proxy authentication data.
 */
void deleteProxyAuth(void);


/*
    --------------
    Administration
    --------------
*/

/*
 * SET PERSISTENT STATUS
 *
 * Turn on or off persistent status for host or proxy 
 * authentication. 'hostNotProxy' is TRUE for host and FALSE
 * for proxy. 'status' is
 * 0 - no persistent save
 * 1 - backup
 * 2 - write-through
 *
 * Return error code.
 */
UINT8 setPersistentAuthStatus (BOOL hostNotProxy, int status);

/*
 * INIT/TERMINATE
 *
 * To be called at start-up or close-down. Call init after dbInit
 * and call terminate before dbTerminate.
 */
void authenticateInit(void);
void authenticateTerminate(void);





/*************************************************************/
/**********************     Cookies    ***********************/
/*************************************************************/


/*************
 * Functions *
 *************/

/*
    -------------------
    Access functions
    -------------------
*/

/*
 *	STORE COOKIE
 *
 * Stores a cookie composed of the given inparameters:
 *  - name,    c-string; mandatory
 *  - value,   c-string; mandatory with one exception, see below
 *  - domain,  c-string, either an explicit domain which starts with '.'
 *             or host, e.g. x.y.com; optional;
 *  - path,    c-string, path, may be empty, must not end with '/'
 *             optional
 *  - expires, unsgined integer in secs after 1970;
 *             expires is time when received + Max_Age; optional
 *  - version, unsigned integer; mandatory
 *
 * The url inparameter must be non-NULL and well formed containing the domain 
 * and optionally the path.
 *
 * If no domain is given (i.e. NULL is passed in), the storeCookie function
 * will set a default value for the cookie domain before storing the cookie. 
 * The default value used is the domain (host) part of the passed in url
 * inparameter.
 *
 * If no path is given, the storeCookie function will set the default value
 * which is the path part up to the rightmost but not including '/' of the
 * url.
 *
 * Next, this function checks that the passed in parameter url, the domain 
 * and path follows the rules as defined in rfc2109 chapter 4.3.2. I.e. it 
 * checks if a cookie received in a response should be rejected. The 
 * following rules apply:
 *  - the url domain part is domain matched against the cookie domain,
 *    domains are case insensitive
 *  - the cookie path must be a prefix of the url path part,
 *    paths are case sensitive
 *  - the cookie domain must not be a top domain, i.e. it must start with a 
 *    dot or contain embedded dots
 *  - a url host of the form HD must not contain any dots in H where D is
 *    the value of the cookie domain.
 *
 * If the check fails, the cookie is rejected.
 *
 * A missing value (NULL) for expires will be interpreted as keeping the
 * cookie until the next time the database is closed down, i.e the
 * next time cookiesTerminate() is called.
 *
 * For mandatory values, NULL must not be passed in, except when a zero
 * expire value is given, see below.
 *
 * A cookie is identified by the components domain, path and name.
 *
 * If already cfg_wae_wspif_CookieItems are present, the
 * least reasently used element is replaced by the new one.
 *
 * Trying to store a cookie that already exists updates the existing cookie.
 * The exception is when a zero expires value is given. This is 
 * interpreted as immediate expiration and any cookie matching the cookie
 * beeing passed in will be deleted. In either case, a cookie with
 * an expires value 0 will not be stored.
 *
 * When a zero expire value is given, the value parameter may be NULL.
 *
 * The cookie is stored in ram. However, when closing the database
 * cookies will be stored on persistent media, see cookiesTerminate().
 *
 * When a cookie is updated or created, all input parameters are copied
 * and stored.
 *
 * Returns TRUE if successfull.
 */
BOOL storeCookie(BYTE* url, BYTE* name, BYTE* value, BYTE* domain,
                 BYTE* path, UINT32* expires, UINT32 version);


/*
 * CHECK COOKIES
 *
 * Searches the database for all cookies valid for the passed in url.
 * To retrieve the result of the search, call getNextCookie repeatedly
 * unitl false is returned.
 *
 * This function is intended to be called before a http request.
 * The url inparameter must be non-NULL and well formed containing the domain
 * and optionally the path.
 *
 * A match as defined in rfc2109 is:
 *  - the url domain part is domain matched against the cookie domain,
 *    domains are case insensitive
 *  - the cookie path must be a prefix of the url path part,
 *    paths are case sensitive
 *
 * All matched cookies will be stored internally. Calling 
 * getNextCookie retrieves cookies from the stored search result.
 *
 * Calling storeCookie, checkCookie or deleteAllCookies
 * invalidates the internally stored search result from the previous
 * checkCookie call.
 *
 * Side effect: during the search, the expires date is checked and
 *              if a cookie is expired it will be removed from the database.
 *
 * Returns TRUE if one or more cookies matched.
 */
BOOL checkCookies(BYTE* url);


/*
 * GET NEXT COOKIE
 *
 * Retrieves the next cookie from the set of matched cookies found
 * in a previous checkCookies() call.
 *
 * This function should be called repeatedly after a checkCookies call
 * until getNextCookie returns false.
 *
 * Cookies are returned in the following order:
 *  - more specific paths precedes less specific paths,
 *
 * getNextCookie returns the cookie components as out-paramameters, except
 * expires. The out-parameters are not copied out, so the database owns
 * (has delete responsibility of) the memory pointed to.
 *
 * If the caller is not interested in an out-parameter, NULL can be 
 * passed in instead, e.g. when passing in NULL for the path parameter, 
 * the path component won't be returned.
 *
 * Of the returned out-params, only the path, i.e. *path, may be NULL.
 * All other pointer out-params has non-NULL values.
 *
 * Returns TRUE when a cookie was retrieved in which case there might
 * be cookies left in the search result set to retrieve.
 * When FALSE is returned, no cookie was retrieved, the out-parameters are
 * not valid and also there are no more cookies left to retrieve from
 * the search result.
 */
BOOL getNextCookie(BYTE** name, BYTE** value, BYTE** domain, BYTE** path,
                   UINT32* version);


/*
 * RESET COOKIE ITERATION
 *
 *	Resets the iteration done via getNextCookie to the first cookie in
 * the set of matched cookies if any. That is, after this function
 * has been called, a call to getNextCookie will return the first
 * matched cookie if any.
 * If the search result is invalidated, nothing is done in this function.
 */
void resetCookieIter(void);


/*
 * DELETE ALL COOKIES
 *
 *	Removes all cookies.
 * At present, cookies are just removed from RAM,
 * not from persistent memory.
 */
void deleteAllCookies(void);



/*
    --------------
    Administration
    --------------
*/

/*
 * INIT/TERMINATE
 *
 * To be called at start-up or close-down. Call init after dbInit
 * and call terminate before dbTerminate.
 *
 * When terminating, cookies with missing expires component will be deleted.
 * All remaining non-expired cookies will be stored on persistent media
 * on close-down.
 * 
 */
void cookiesInit(void);
void cookiesTerminate(void);



#endif

