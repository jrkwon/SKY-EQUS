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
 * dbFunct.c
 *
 * Data channel handling.
 * Store and find authentication information in database.
 *
 *	This file contains various functions with two things in common:
 * they both use wipdb database and they access structures
 * internal to the database.
 *
 *
 * Created by Niclas Kellgren 000904.
 *
 * Revision history:
 *   000914, NKE: data channel code moved from wipdb.c
 *   001004, NKE: host authentication added
 *   001010, NKE: proxy authentication api added
 *   001019, NKE: host and proxy api changed
 *   001106, NKE: proxy authentication functionality added
 *   001116, NKE: updates and corrections
 *   001117, NKE: documentation
 *   001207, NKE: corrected addDataChannel
 *   001211, NKE: removed persitent proxy handling
 */

#include "dbfunct.h"


/*************************************************************/
/************************  external  *************************/
/*************************************************************/

/*************
 * Constants *
 *************/

/* These constants are copies from wipdb.c */

enum {
	KeyLen = 6,

	Flag_rec = 0,
	Flag_str = 3,
	Flag_dataExternal = 128
};


/*********
 * Types *
 *********/

/* These types are copies from wipdb.c */

typedef union {
	UINT32                i;
	void*                 p;
	BYTE*                 s;
	struct RecordStruct*  r;
	struct FieldStruct*   f;
	struct ElementStruct* e;

} DataUnion;


typedef struct ElementStruct* ElementPtr;
typedef struct ElementStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;

} Element;


typedef struct FieldStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;
	BYTE       key[KeyLen];

} Field, *FieldPtr;


typedef struct RecordStruct* RecordPtr;
typedef struct RecordStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;
	BYTE       key[KeyLen];
	UINT16     ref;
	RecordPtr  nextRec; 

} Record;


/*************************
 * External declarations *
 *************************/

/* Found in wipdb.c */
RecordPtr findRecord(DB_ref record);
FieldPtr  findField(const BYTE* key);
UINT8 deleteItem_ptr(RecordPtr rec, ElementPtr currentElem);
void moveToFront(RecordPtr rec, ElementPtr elem);

/* Found in CmmnRsrc.c */
extern const BYTE latin1_lc[256];





/*************************************************************/
/******************  data channel handling  ******************/
/*************************************************************/

/*

For information on data channels, objects and what you can 
configure, see Users Manual. You can find all all abbreviations
for element names in capiclnt.c functions: CLNTc_setIntConfig,
CLNTc_setStrConfig, CLNTc_setDCHIntConfig, CLNTc_setDCHStrConfig.

There are three types of configure: global, template and local.
* The global configurations are saved under root/globl. They are
  valid for all objects and channels.
* The template is object 0, saved as root/glob/obj00. When
  creating a new object, the template is copied. The new object
  includes copies of all parameters and data channels. In fact,
  the object 0 is a complete channel and can be used for searching
  if one would like to.
* The local objects are saved under the root and named objxx,
  where xx is the hexadecimal representation of the object number.


STRUCTURE

The structure used is a combination of a database hierachy and
external lists containing data and pointers to database elements
This combination is not desirable. If the database is manipulated
through ordinary database operations, such as deleteItem, then
the external structure will contains dangling pointers and the
system may crash. An alternative could be to put all information
outside or inside the database. None of these alternatives are
appealing. A pure database solution is far to slow, since the
data channel handling are very frequent operations. If not using
the database at all, then all of its advantages are lost and
the program will be significantly larger. The database handles
deallocation and persistent memory among other things.

The database tree looks like this if two channels have been added
to the default system.


root
  |
  obj01 --- globl ---
    |         |
    |         obj00 ---  ua --- cMode --- cAge
    |           |
    |           updtI --- dsplI --- hstSz
    |
    |
    dch00  ------------------------ dch01 ------------- updtI --- dsplI --- hstSz
      |                               |
      hosts --- onln --- tmout        hosts --- onln --- tmout
        |                               |
        <> --- <> --- <> ---            <> --- <> --- <> --- 
        |      |      |                 |      |      |
        string string string            string string string


At top level there are 'globl' and 'obj'.

...//


In this code elements are created and inserted into the database
without calling the approptiate database operations (setItem).
This is done to speed up handling, but at the same time care must
be taken to introduce new flags etc. into this code as well. 

*/


/*************
 * Constants *
 *************/


/*********
 * Types *
 *********/

typedef struct ChannelListStruct {
	RecordPtr hostPtr;
	DB_ref    channelRef;
	UINT8     channelNbr;
} ChannelList[DB_maxNbrOfDataChannels], *ChannelListPtr;

typedef struct ObjectListStruct {
	ChannelListPtr channelList;
	INT16          defaultChannel;
	DB_ref         objectRef;
	UINT8          channelListSize;
	UINT8          objectNbr;
} ObjectList[DB_maxNbrOfObjects], *ObjectListPtr;


/*************
 * Variables *
 *************/

static ObjectListPtr objectList;          /* List of all objects created by createObjects.\*/
static int objectListSize = 0;            /* Both used for data channel handling. */
static DB_ref obj00Ref  = DB_null;        /* Reference to the object template = /globl/obj00 */
       DB_ref globalRef = DB_null;        /* Reference to the global object   = /glob */


/*************
 * Functions *
 *************/

/*
 * Return a reference to the record associated with object
 * 'object'.
 */
DB_ref getObjectRef(UINT8 object)
{
	int i;

	for (i=0; i < objectListSize; i++) {
		if (objectList[i].objectNbr == object)
			return objectList[i].objectRef;
	}
	return DB_null;
}



/*
 * Return a reference to the record associatied with channel
 * 'channel' in object 'object'.
 */
DB_ref getChannelRef(UINT8 object, UINT8 channel)
{
	ChannelListPtr channelList;
	int            i, j;

	for (i=0; ; i++) {
		if (i == objectListSize) 
			return DB_null;
		if (objectList[i].objectNbr == object) {
			channelList = objectList[i].channelList;
			break;
		}
	}

	for (j=0; j < objectList[i].channelListSize; j++) {
		if (channelList[j].channelNbr == channel)
			return channelList[j].channelRef;
	}
	return DB_null;
}



/*
 * Create an record containing all mandatory fields. The record
 * is named "objxx", where xx is the hexidecimal representation
 * of 'object' in lower-case.
 *
 * Return a pointer to the new channel list.
 */
ObjectListPtr addObjectInternal(INT8 object) {
	BYTE           key[] = "obj??";
	ChannelListPtr channelList;
	ChannelListPtr sourceList;
	ObjectListPtr  objectRecord;
	FieldPtr       field;
	DB_ref         refO, ref;
	UINT8          error;
	UINT8          nbrOfChannels = 0;
	int            i;

	if (objectListSize == DB_maxNbrOfObjects || object == 0)
		return NULL;

	ByteToHex(object, key+3);
	refO = copyRecord(obj00Ref, DB_root, key, &error);
	if (error)
		return NULL;

	channelList = (ChannelListPtr) OSConnectorAlloc(sizeof(ChannelList)); 
	#ifndef HAS_SETJMP
	if (channelList == NULL)
		return NULL;
	#endif

	objectRecord = &objectList[objectListSize++];
	objectRecord->objectRef       = refO;
	objectRecord->objectNbr       = object;
	objectRecord->channelList     = channelList;
	objectRecord->defaultChannel  = objectList[0].defaultChannel;

	*(key + 0) = 'd';
	*(key + 1) = 'c';
	*(key + 2) = 'h';

	sourceList = objectList[0].channelList;
	for (i=0; i < objectList[0].channelListSize; i++) {
		ByteToHex(sourceList->channelNbr, key+3);
		ref = getRef(refO, key, &error);
		if (findRecord(ref) == NULL)
			goto label_addObjectInternal;
		field = findField((BYTE*) "hosts");
		if (field != NULL) {
			channelList->channelNbr = sourceList->channelNbr;
			channelList->channelRef = ref;
			channelList->hostPtr    = (RecordPtr) field;
			channelList++;
			nbrOfChannels++;
		} else
label_addObjectInternal:
			if (sourceList->channelNbr == objectList[0].defaultChannel)
				objectRecord->defaultChannel  = -1;
		sourceList++;
	}

	objectRecord->channelListSize = nbrOfChannels;

	return objectRecord;
}



/*
 * Interface to addObjectInternal for external calls.
 *
 * Create an record containing all mandatory fields. The record
 * is named "objxx", where xx is the hexidecimal representation
 * of 'object' in lower-case.
 *
 * Return a reference to the new object record.
 */
DB_ref addObject(INT8 object) {
	ObjectListPtr temp;

	temp = addObjectInternal(object);
	if (temp == NULL)
		return DB_null;
	else
		return temp->objectRef;
}



/*
 * Removes an object and its record from the database.
 */
void deleteObject(UINT8 object)
{
	BYTE key[] = "obj??";
	int  i;

	if (object == 0)
		return;

	for (i=0; ; i++) {
		if (i == objectListSize) 
			return;
		if (objectList[i].objectNbr == object) 
			break;
	}

	ByteToHex(object, key+3);
	deleteItem(DB_root, key);

	OSConnectorFree(objectList[i].channelList);
	objectListSize--;
}



/*
 * Add and update all fields needed in the record "objxx", where
 * xx is the hexidecimal representation of 'object'. If "objxx"
 * does not exist, it is created by addObjectInternal.
 *
 * A new record field "dchyy" is added to "objxx", containing an
 * empty set "hosts". yy is the hexidecimal representation of
 * 'channel'.
 *
 * "tmout" and "onln" fields are added to the channel record.
 * See capiclnt.c for more information.
 *
 * Return reference to the new channel record or to an old
 * channel record with the same name if it already exists.
 */
DB_ref addDataChannel(UINT8 object, UINT8 channel)
{
	BYTE           key[] = "dch??";
	ChannelListPtr channelList;
	DB_ref         refO, refC, refU;
	UINT8          error;
	UINT8          nbr;
	int            i;

	for (i=0; i < objectListSize; i++) {
		if (objectList[i].objectNbr == object) {
			channelList = objectList[i].channelList;
			break;
		}
	}
	if (i == objectListSize) {
		channelList = addObjectInternal(object)->channelList;
		if (channelList == NULL)
			return DB_null;
	} else
		if (objectList[i].channelListSize == DB_maxNbrOfDataChannels)
			return DB_null;

	refO = objectList[i].objectRef;

	ByteToHex(channel, key+3);
	refC = createRecord(refO, key, DB_rec, &error);
	if (error == DB_err_exists)
		return ((RecordPtr) findField(key))->ref;
	else if (error)
		return DB_null;
	refU = createRecord(refC, (BYTE*) "hosts", DB_set, &error);
	if (error) {
		deleteItem(refO, key);
		return DB_null;
	}

	if (objectList[i].defaultChannel == -1)
		objectList[i].defaultChannel = channel;

	nbr = objectList[i].channelListSize++;
	channelList[nbr].channelNbr = channel;
	channelList[nbr].channelRef = refC;
	channelList[nbr].hostPtr    = findRecord(refU);
	
	/* Fields added to the data channel */

	setInt(refC, (BYTE*) "tmout", 60); /* Request timeout */
	setInt(refC, (BYTE*) "onln" , 0);  /* Online */
	return refC;
}



/*
 * Removes a data channel, if found. If the default channel is
 * deleted, then a call to getDataChannel that does not result
 * in a match will cause an error code to be returned.
 */
void deleteDataChannel(UINT8 object, UINT8 channel)
{
	BYTE           key[] = "dch??";
	ChannelListPtr channelList;
	int            lastChannel;
	int            i, j;

	for (i=0; ; i++) {
		if (i == objectListSize) 
			return;
		if (objectList[i].objectNbr == object) 
			break;
	}

	channelList = objectList[i].channelList;
	lastChannel = objectList[i].channelListSize - 1;  
	for (j=lastChannel; ; j--) {
		if (j < 0)
			return;
		if (channelList[j].channelNbr == channel) {
			channelList[j].channelNbr = channelList[lastChannel].channelNbr;
			channelList[j].channelRef = channelList[lastChannel].channelRef;
			channelList[j].hostPtr    = channelList[lastChannel].hostPtr;
			objectList[i].channelListSize--;
			break;
		}
	}

	if (objectList[i].defaultChannel == channel)
		objectList[i].defaultChannel = -1;

	ByteToHex(channel, key+3);
	deleteItem(objectList[i].objectRef, key);
}



/*
 * Searches through all host addresses connected to the channels
 * in an object. The channel number belonging to the first
 * matching host is returned. If there is no match, then if set,
 * the default channel number is returned, or else an error occurs. 
 *
 * getDataChannel assumes the URL to be well formatted, including
 * a scheme and a network path. Matching is case insensitive.
 *
 * Pattern matching is used. A host "*.x.com" matches all URLs with
 * the host "x.com" or a host ending in ".x.com".
 *
 * '*errorCode' signals the return status.
 */
UINT8 getDataChannel(UINT8 object, const BYTE* url, UINT8 *errorCode)
{
	ChannelListPtr channelList;
	ElementPtr     element;
	BYTE*          s;
	const BYTE*    t;
	const BYTE*    endPtr;
	int            i, j;

	for (i=0; ; i++) {
		if (i == objectListSize) {
			*errorCode = DB_err_objectNotFound;
			return 255;
		}
		if (objectList[i].objectNbr == object) {
			channelList = objectList[i].channelList;
			break;
		}
	}
	
	*errorCode = DB_err_success;
	while (*url++ != ':')
		;
	url += 2;
	
	for (endPtr=url; *endPtr!='\0' && *endPtr!='/' && *endPtr!=':' &&
	                 *endPtr!='?'  && *endPtr!='#'; endPtr++)
		;

	for (j=0; ;j++) {
		if (j == objectList[i].channelListSize) 
			if (objectList[i].defaultChannel == -1) {
				*errorCode = DB_err_noDefaultChannel;
				return 255;
			} else 
				return (UINT8) objectList[i].defaultChannel;
		for (element=channelList[j].hostPtr->data.e; element; element=element->next) {
			/* Using the same array - latin1_lc - as CmmnRsrc.c: ByteStringNCmpNoCase. */
			s=element->data.s;
			if (*s == '*') {
				for (t=endPtr-1, s++; ; s++, t--) {
					if (latin1_lc[*s] != latin1_lc[*t])
						if (!*s)
							break;
						else
							goto label_getDataChannel;
					if (t == url)
						if (*(s+1)=='.' && !*(s+2))
							break;
						else
							goto label_getDataChannel;
				}
			} else {
				for (t=url; ; s++, t++) {
					if (latin1_lc[*s] != latin1_lc[*t])
						if (!*s && t == endPtr)
							break;
						else
							goto label_getDataChannel;
					if (!*s)
						break;
				}
			}
			return channelList[j].channelNbr;
label_getDataChannel:;
		}
	}
}



/*
 * Change the data channel to be used when no host matches when
 * calling getDataChannel.
 *
 * An object and a data channel are created if either is missing.
 */
void setDefaultDataChannel(UINT8 object, UINT8 channel)
{
	ChannelListPtr channelList;
	int            i, j;

	for (i=0; ; i++) {
		if (i == objectListSize)
			if (addDataChannel(object, channel) == DB_null) 
				return;
			/* else caught by next if */ 
		if (objectList[i].objectNbr == object) {
			channelList = objectList[i].channelList;
			break;
		}
	}

	for (j=0; ; j++) {
		if (j == objectList[i].channelListSize)
			if (addDataChannel(object, channel) == DB_null)
				return;
			else
				break;
		if (channelList[j].channelNbr == channel)
			break;
	}

	objectList[i].defaultChannel = channel;
}



/*
 * Add a host to the specified channel. The string pointed out
 * by 'host' is copied and not taken over by the database.
 *
 * An object or a data channel is created if either is missing.
 *
 * 'length' is the length of host, excluding an optional 
 * terminating EOL.
 */
void addHost(UINT8 object, UINT8 channel, const BYTE* host, UINT8 length)
{
	ChannelListPtr channelList;
	ElementPtr     element;
	BYTE*          newHost;
	BYTE*          s;
	int            i, j;

	if (host == NULL && length == 0)
		return;
	if (host[length-1] == '\0')
		if (--length == 0)
			return;

	newHost = (BYTE*) OSConnectorAlloc(length + 1); 
	#ifndef HAS_SETJMP
	if (newHost == NULL)
		return;
	#endif
	
	if (*host == '*') {
		*newHost = '*';
		for (s=newHost+length-1, i=length; --i;)
			*s-- = *++host;
	} else
		for (s=newHost, i=length; i--;)
			*s++ = *host++;
	newHost[length] = '\0';

	for (i=0; ; i++) {
		if (i == objectListSize)
			if (addDataChannel(object, channel) == DB_null) {
				OSConnectorFree(newHost);
				return;
			} /* else caught by next if */
		if (objectList[i].objectNbr == object) {
			channelList = objectList[i].channelList;
			break;
		}
	}
	
	for (j=0; ; j++) {
		if (j == objectList[i].channelListSize)
			if (addDataChannel(object, channel) == DB_null) {
				OSConnectorFree(newHost);
				return;
			} else
				break;
		if (channelList[j].channelNbr == channel)
			break;
	}
	
	element = (ElementPtr) OSConnectorAlloc(sizeof(Element)); 
	#ifndef HAS_SETJMP
	if (element == NULL) {
		OSConnectorFree(newHost);
		return;
	}
	#endif

	element->data.s = newHost;
	/* Insert new flags, when introduced in wipdb.c */
	element->flags = Flag_str | Flag_dataExternal;  
	element->next = channelList[j].hostPtr->data.e;
	channelList[j].hostPtr->data.p = (void*) element;
}



/*
 * Remove a host, if found, from the named channel. The 'host'
 * string must match the host to delete when doing a case
 * insensitive character by character comparison.
 *
 * 'length' is the length of host, excluding an optional 
 * terminating EOL.
 */
void deleteHost(UINT8 object, UINT8 channel, const BYTE* host, UINT8 length)
{
	ChannelListPtr channelList;
	ElementPtr     element;
	ElementPtr     prev;
	BYTE*          s;
	const BYTE*    t;
	int            i, j;

	if (host == NULL || length == 0)
		return;
	if (host[length-1] == '\0')
		if (--length == 0)
			return;

	for (i=0; ; i++) {
		if (i == objectListSize) 
			return;
		if (objectList[i].objectNbr == object) {
			channelList = objectList[i].channelList;
			break;
		}
	}

	for (j=0; ; j++) {
		if (j == objectList[i].channelListSize)
			return;
		if (channelList[j].channelNbr == channel)
			break;
	}

	for (element=channelList[j].hostPtr->data.e, prev=NULL; element; prev=element, element=element->next) {
		/* Using the same array - latin1_lc - as CmmnRsrc.c: ByteStringNCmpNoCase */
		s=element->data.s;
		if (*s == '*') {
			if (*host != '*')
				goto label_deleteHost;
			for (t=host+length-1, s++;	; s++, t--) {
				if (latin1_lc[*s] != latin1_lc[*t])
					if (!*s && *t=='*')
						break;
					else
						goto label_deleteHost;
				if (t == host)
					goto label_deleteHost;
			}
		} else {
			for (t=host, i=length; ; s++, t++) {
				if (latin1_lc[*s] != latin1_lc[*t])
					goto label_deleteHost;
				if (--i == 0)
					if (!*(s+1))
						break;
					else
						goto label_deleteHost;
			}
		}
		if (prev == NULL)
			channelList[j].hostPtr->data.p = element->next;
		else
			prev->next = element->next;
		OSConnectorFree(element->data.s); 
		OSConnectorFree(element);
		return;
label_deleteHost:;
	}
}



/* ======== admin ======== */

/*
 * Initiate structures connected to data channel handling.
 */
void dataChannelInit(void)
{
	ChannelListPtr channelList;
	ObjectListPtr  objectRecord;
	BYTE*          temp;
	UINT8          error;

	objectList = (ObjectListPtr) OSConnectorAlloc(sizeof(ObjectList)); 
	#ifndef HAS_SETJMP
	if (objectList == NULL)
		return;
	#endif
	objectListSize = 1;

	globalRef = createRecord(DB_root, (BYTE*) "globl", DB_rec, &error);
	obj00Ref  = createRecord(globalRef, (BYTE*) "obj00", DB_rec, &error);

	channelList = (ChannelListPtr) OSConnectorAlloc(sizeof(ChannelList)); 

	objectRecord = &objectList[0];
	objectRecord->objectRef       = obj00Ref;
	objectRecord->channelListSize = 0;
	objectRecord->objectNbr       = 0;
	objectRecord->defaultChannel  = -1;
	objectRecord->channelList     = channelList;

	/* Fields added to the global object */

	setInt(globalRef, (BYTE*) "cAge", 86400);  /* Cache age */
	setInt(globalRef, (BYTE*) "cMode", 1);     /* Cache mode */
	if (setStr(globalRef, (BYTE*) "ua", temp = B_STRDUP((BYTE*) "WAPPER")))
		OSConnectorFree(temp);                  /* User agent */

	/* Fields added to the object template */

	setInt(obj00Ref, (BYTE*) "hstSz", 10);     /* History size */
	setInt(obj00Ref, (BYTE*) "dsplI", 1);      /* Display images */
	setInt(obj00Ref, (BYTE*) "updtI", 0);      /* Update images */
}



/* 
 * Remove all structures associated with objects and data
 * channels. To be called when closing down the database.
 */
void dataChannelTerminate(void) {
	int i;

	for (i=0; i < objectListSize; i++) 
		OSConnectorFree(objectList[i].channelList);
	OSConnectorFree(objectList);
}







/*************************************************************/
/**********************  authenticate  ***********************/
/*************************************************************/

/*

STRUCTURE

The database structure used to save authentication information
is arranged like this:

root
  |
  auth
    |
    proxy ---- host
      |         |
      |         <> ------------------------------ <> ---------------------------- <> ---
      |           |                                 |
      |           host --- realm --- cred          host --- realm --- cred
      |           |        |         |             |        |         |
      |           string   string    mem           string   string    mem
      |
      |
      <> ---------------------------- <> --------------------------- <> ---
        |                               |
        addr --- port --- cred          addr --- port --- cred
        |                 |             |                 |
        string            mem           string            mem


Name    type  comment
---------------------
auth  - rec   write through
proxy - set   proxy authentication list
host  - set   host authentication list
<>    - rec   nameless record
host  - str   host taken from URL
realm - str   realm
cred  - mem   credentials
addr  - str   address of proxy
port  - int   port number

The order of elements within a list is significant for the
addr-port-cred and host-realm-cred lists.

The 'auth' record is write through and thus the entire structure.

Two counters (hostAuthSize and proxyAuthSize) are used for saving
the list lengths. They can be at most maxHostListSize resp.
maxProxyAuthSize elements long. 


SEARCHING

Searching for a matching host is not done in the same way as is
done for proxies. The host find function (checkHostAuth) is 
called in two cases: before a http request and when a request
results in a challange for authenification. In the first case,
checkHostAuth is called with no realm and the matching is done
based on only the host. Even if the item exists in the database,
a mismatch may occur. This can happen if several items with
identical host are in the database. A mismatch will result in a
challenge for the server, containing the correct realm and the
correct credentials can be found.

When found an match, this item is moved first in list for three
reasons. 
1. When the list is full the least reasently used item
   (i.e. last element in the list) is deleted.
2. When searching, it is likely that the item searched for is
   the same as last time, especially for proxies.
3. If searching for a host without a realm, the first matching
   item is used. If this is not the correct on, a realm is
   returned and the correct one is placed ahead of the wrong one.
	The next lookup for the same host will probably be for the
	same realm.

The third case can be solved a little more elegant by saving the
path along with the host. Out of the paths, a more inteligen
guess can be made. Still, it is not foolproof and demands 
more code, a larger amount of RAM and more persistent memory
will be locked up. Add to this, it will be slower. If sending
alternating requests to the same host, but using different
realms is a frequent operation, then it will probably worthwhile
to implement such a solution.

The host and realm are both text strings and are matched agaist
the database case insensitivily.

The address can contain any data not only character data and is
only an exact match counts when comparing addresses.

*/



/*************
 * Constants *
 *************/

enum {
	/* Must be at least 2 */
	maxHostListSize  = cfg_wae_wspif_AuthenticationItems,                  
	maxProxyListSize = cfg_wae_wspif_AuthenticationItems
};


/*********
 * Types *
 *********/


/*************
 * Variables *
 *************/

static RecordPtr hostListRec;             /* Pointer to host list record */ 
static RecordPtr proxyListRec;            /* Pointer to proxy list record */ 

static UINT8 hostAuthSize;                /* Number of site authentification records in database. */
static UINT8 proxyAuthSize;               /* Number of proxy authentification records in database. */


/*************
 * Functions *
 *************/


/* ======== host ======== */

/*
 * Find host in database. Functionality common to storeHostAuth
 * and checkHostAuth is located in this function.
 *
 * Matches the host of 'url' and 'realm', if not NULL.
 *
 * Return TRUE if match found.
 */
BOOL findHost(BYTE* *url, BYTE* realm, BYTE* *endPointer, RecordPtr *currentRec)
{
	RecordPtr rec;
	RecordPtr tempRec;
	BYTE*     endPtr;
	BYTE*     s;
	BYTE*     t;

	/* The url is known to be a well formatted http/https URL */
	
	s = *url;
	while (*s++ != ':')
		;
	*url = s + 2;
	for (endPtr=*url; *endPtr!='\0' && *endPtr!='/' && *endPtr!=':' &&
	                 *endPtr!='?'  && *endPtr!='#'; endPtr++)
		;

	/* Search for matching host and realm */
	rec = NULL;
	for (tempRec=hostListRec->data.r; tempRec != NULL; tempRec=(RecordPtr) rec->next) {
		rec = tempRec;
		for (s=rec->data.f->data.s, t=*url; ; s++, t++) {
			if (latin1_lc[*s] != latin1_lc[*t])
				if (!*s && t == endPtr)
					break;
				else
					goto label_findHost_loop;
			if (!*s)
				break;
		}
		/* Host found, check realm */
		if (realm != NULL) {
			for (s=rec->data.f->next->data.s, t=realm; ;s++, t++)
				if (latin1_lc[*s] == latin1_lc[*t] && !*s)
					goto label_findHost_match;
		} else
			goto label_findHost_match;
label_findHost_loop:;
	}

	/* --- No match found ---*/
	*endPointer  = endPtr;
	*currentRec  = rec;
	return FALSE;
	

	/* --- Matching host and realm (if present) found --- */
label_findHost_match:

	/* Move current record first in queue. Nessesary if several
	 * realms are connected to a single host.*/
	if (hostListRec->data.r != rec) 
		moveToFront(hostListRec, (ElementPtr) rec);

	*currentRec = rec;
	return TRUE;
}



/*
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
BOOL storeHostAuth(BYTE* url, BYTE* realm, BYTE* credentials, UINT8 credLength)
{
	DB_ref    ref;
	RecordPtr rec;
	BYTE*     endPtr;
	BYTE*     host;
	BYTE*     cred;
	BYTE*     s;
	BYTE*     t;
	UINT8     error;

	if (!url || !realm || !credentials)
		return FALSE;

	cred = (BYTE*) OSConnectorAlloc(credLength+2);  
	#ifndef HAS_SETJMP
	if (cred == NULL)
		return FALSE;
	#endif
	*(UINT16*) cred = credLength + 2;
	memcpy(cred+2, credentials, credLength);

	if (findHost(&url, realm, &endPtr, &rec))
		setMem(rec->ref, (BYTE*) "cred", cred);
	else {
		/* Host not found - create new list item */
		host = (BYTE*) OSConnectorAlloc(endPtr-url+1); 
		#ifndef HAS_SETJMP
		if (host == NULL)
			return FALSE;
		#endif
		for (s=host, t=url; t != endPtr; s++, t++)
			*s = *t;
		*s = '\0';
		
		if (hostAuthSize == maxHostListSize) {
			/* List full; delete the least recently used record.
			 * This is done by reusing the record to delete and
			 * placing it first in the queue. */
			moveToFront(hostListRec, (ElementPtr) rec);
			hostAuthSize--;
			ref = rec->ref;
		} else {
			ref = createRecord(hostListRec->ref, NULL, Flag_rec, &error);	 
			if (error) {
				OSConnectorFree(host);
				OSConnectorFree(cred);
				return FALSE;
			}
		}
		/* Order significant if creating new record; reverse list order */
		error  = setMem(ref, (BYTE*) "cred",  cred); 
		error |= setStr(ref, (BYTE*) "realm", B_STRDUP(realm));
		error |= setStr(ref, (BYTE*) "host",  host);
		if (error)
			deleteItem_ptr(hostListRec, (ElementPtr) findRecord(ref));
		else
			hostAuthSize++;
	}

	return TRUE;
}



/*
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
BOOL checkHostAuth(BYTE* url, BYTE* realm, BYTE* *credentials)
{
	RecordPtr rec;
	void*     bogousPtr;

	if (!url)
		return FALSE;

	if (findHost(&url, realm, (BYTE**)&bogousPtr, &rec)) {
		*credentials = rec->data.f->next->next->data.s;
		return TRUE;
	} else {
		*credentials = NULL;
		return FALSE;
	}
}



/*
 * Remove all host authentication data.
 */
void deleteHostAuth(void)
{
	clearRecord(hostListRec->ref);
	hostListRec->data.e = NULL;
	hostAuthSize = 0;
}



/* ======== proxy ======== */

/*
 * Find proxy in database. Functionality common to storeProxyAuth
 * and checkProxyAuth is located in this function.
 *
 * Matches 'address' and port number.
 *
 * Return TRUE if match found.
 */
BOOL findProxy(BYTE* address, UINT8 addressLength, UINT16 portNumber, RecordPtr *currentRec)
{
	RecordPtr rec;
	RecordPtr tempRec;
	BYTE*     s;
	BYTE*     t;
	UINT8     sLen;
	UINT8     len;

	/* Search for matching address and port */
	rec = NULL;
	for (tempRec=proxyListRec->data.r; tempRec != NULL; tempRec=(RecordPtr) rec->next) {
		rec = tempRec;

		s=rec->data.f->data.s;
		sLen = (UINT8)*(UINT16*) s;
		for (s += 2, t=address, len=0; len < addressLength && len < sLen; s++, t++, len++) {
			if (*s != *t)
				goto label_findProxy_loop;
		}
		/* Address found, check realm */
		if (portNumber == rec->data.f->next->data.i)
			goto label_findProxy_match;	

label_findProxy_loop:;
	}

	/* --- No match found ---*/
	*currentRec  = rec;
	return FALSE;
	

	/* --- Matching address and port found --- */
label_findProxy_match:;

	/* Move current record first in queue. When list is full, 
	 * the least recently used item is deleted. */
	if (proxyListRec->data.r != rec)
		moveToFront(proxyListRec, (ElementPtr) rec);
  
	*currentRec  = rec;
	return TRUE;
}


/*
 * Store 'address' and 'portNumber' together with 'credentials'.
 * If already cfg_wae_wspif_AuthenticationItems are present, the
 * least recently used element is replaced by the new one.
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
BOOL storeProxyAuth(BYTE* address, UINT8 addressLength, UINT16 portNumber,
                    BYTE* credentials, UINT8 credLength)
{
	DB_ref    ref;
	RecordPtr rec;
	BYTE*     addr;
	BYTE*     cred;
	UINT8     error;

	if (!address || !credentials)
		return FALSE;

	cred = (BYTE*) OSConnectorAlloc(credLength+2);  
	#ifndef HAS_SETJMP
	if (cred == NULL)
		return FALSE;
	#endif
	*(UINT16*) cred = credLength + 2;
	memcpy(cred+2, credentials, credLength);

	if (findProxy(address, addressLength, portNumber, &rec))
		setMem(rec->ref, (BYTE*) "cred", cred);
	else {
		/* Address not found - create new list item */
		addr = (BYTE*) OSConnectorAlloc(addressLength + 2); 
		#ifndef HAS_SETJMP
		if (addr == NULL)
			return FALSE;
		#endif
		*(UINT16*) addr = addressLength + 2;
		memcpy(addr + 2, address, addressLength);
		
		if (proxyAuthSize == maxProxyListSize) {
			/* List full; delete the least recently used record.
			 * This is done by reusing the record to delete and
			 * placing it first in the queue. */
			moveToFront(proxyListRec, (ElementPtr) rec);
			proxyAuthSize--;
			ref = rec->ref;
		} else {
			ref = createRecord(proxyListRec->ref, NULL, Flag_rec, &error);	 
			if (error) {
				OSConnectorFree(cred);
				OSConnectorFree(addr);
				return FALSE;
			}
		}
		/* Order significant if creating new record; reverse list order */
		error  = setMem(ref, (BYTE*) "cred",  cred); 
		error |= setInt(ref, (BYTE*) "port", portNumber);
		error |= setMem(ref, (BYTE*) "addr", addr);
		if (error)
			deleteItem_ptr(proxyListRec, (ElementPtr) findRecord(ref));
		else
			proxyAuthSize++;
	}

	return TRUE;
}



/*
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
 * Return TRUE if found a match.
 */
BOOL checkProxyAuth(BYTE* address, UINT8 addressLength, UINT16 portNumber, BYTE* *credentials)
{
	RecordPtr rec;

	if (!address || addressLength == 0)
		return FALSE;

	if (findProxy(address, addressLength, portNumber, &rec)) {
		*credentials = rec->data.f->next->next->data.s;
		return TRUE;
	} else {
		*credentials = NULL;
		return FALSE;
	}
}



/*
 *	Remove all proxy authentication data.
 */
void deleteProxyAuth(void)
{
	clearRecord(proxyListRec->ref);
	proxyAuthSize = 0;
}



/* ======== admin ======== */


/*
 * Count number of elements in host or proxy list. Delete items
 * that are not compete, i.e. some data is missing.
 *
 * Return size of list.
 */
UINT8 checkAuthList(RecordPtr parent)
{
	RecordPtr  rec;
	ElementPtr tempElem;
	UINT8      size;

	size = 0;
	rec = parent->data.r;
	while (rec != NULL) {
		tempElem = (ElementPtr) rec;
		rec = (RecordPtr) rec->next;
		if (tempElem->data.e == NULL || tempElem->data.e->next == NULL ||
		                                tempElem->data.e->next->next == NULL)
			deleteItem_ptr(parent, tempElem);
		else
			size++;
	}
	return size;
}



/*
 * Initiate authentication lists. If first time, create host and
 * proxy write through list, else verify the correctness of the
 * list loaded from persistent memory.
 *
 * Must be called after dbInit.
 */
void authenticateInit(void)
{
	DB_ref authRef;
	DB_ref ref;
	UINT8  error;

	/* This version does not use persistently saved proxy data. */
	authRef = createRecord(DB_root, (BYTE*) "authP", DB_rec, &error);
	ref = createRecord(authRef, (BYTE*) "proxy", DB_set, &error);
	proxyAuthSize = 0;
	proxyListRec = findRecord(ref);


	authRef = getRef(DB_root, (BYTE*) "auth", &error);
	if (error) {
		/* First time, no copy found on persistent memory. */
		authRef = createRecord(DB_root, (BYTE*) "auth", DB_rec | DB_writeThrough, &error);

		hostAuthSize  = 0;
		/*
		proxyAuthSize = 0;
		*/

		ref = createRecord(authRef, (BYTE*) "host",  DB_set, &error);
		hostListRec = findRecord(ref);
		
		/*
		ref = createRecord(authRef, (BYTE*) "proxy", DB_set, &error);
		proxyListRec = findRecord(ref);
		*/
	} else {
		hostListRec  = findRecord(getRef(authRef, (BYTE*) "host",  &error));
		/*
		proxyListRec = findRecord(getRef(authRef, (BYTE*) "proxy", &error));
		*/

		hostAuthSize  = checkAuthList(hostListRec);
		/*
		proxyAuthSize = checkAuthList(proxyListRec);
		*/
	}
}



/*
 * Terminate authentication.
 *
 * Must be called before dbTerminate.
 */
void authenticateTerminate(void)
{
}


