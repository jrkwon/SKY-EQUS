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
 * dbFunct.h
 *
 * Data channel handling.
 * Store and find authentication information in database.
 *
 *	This file contains various functions with two things in common:
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
 */

#ifndef _DBFUNCT_H
#define _DBFUNCT_H

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
 * to the first matching host is returned. If there is no match,
 * then if set, the default channel number is returned, or else
 * an error occurs. Assumes the url to be well formatted, including
 * a scheme. Matching is case insensitive. The status of the
 * function at return is signaled through '*errorCode'.
 *
 * getDataChannel uses pattern matching. A host "*.x.com" added by
 * addHost matches all URLs with the host "x.com" or a host ending
 * in ".x.com". 
 *
 * setDefaultDataChannel changes the data channel to be used
 * when no host matches when calling getDataChannel. An object
 * and a data channel are created if either is missing.
*/
DB_ref addDataChannel(UINT8 object, UINT8 channel);
void   deleteDataChannel(UINT8 object, UINT8 channel);
UINT8  getDataChannel(UINT8 object, const BYTE* url, UINT8 *errorCode);
void   setDefaultDataChannel(UINT8 object, UINT8 channel);

/*
 * ADD/DELETE HOST
 *
 * Add/delete host adress. Calling deleteHost, the 'host'
 * string must match the host to delete when doing a case
 * insensitive character by character comparison.
 *
 * When calling these functions, the string pointed out by 'host'
 * is not taken over by the database.
 *
 * 'length' is the length of host, excluding an optional 
 * terminating EOL.
 */
void addHost   (UINT8 object, UINT8 channel, const BYTE* host, UINT8 length);
void deleteHost(UINT8 object, UINT8 channel, const BYTE* host, UINT8 length);


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
 * INIT/TERMINATE
 *
 * To be called at start-up or close-down. Call init after dbInit
 * and call terminate before dbTerminate.
 */
void authenticateInit(void);
void authenticateTerminate(void);



#endif
