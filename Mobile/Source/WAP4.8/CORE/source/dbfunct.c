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
 * dbFunct.c
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
 *   000914, NKE: data channel code moved from wipdb.c
 *   001004, NKE: host authentication added
 *   001010, NKE: proxy authentication api added
 *   001019, NKE: host and proxy api changed
 *   001106, NKE: proxy authentication functionality added
 *   001116, NKE: updates and corrections
 *   001117, NKE: documentation
 *   001207, NKE: corrected addDataChannel
 *   001211, NKE: removed persistent proxy handling
 *   010122, NKE: replaced configuration variable names with constants
 *   010130, NKE: changed the declaration of addHost and deleteHost 
 *   010130, NKE: added getFirstConfiguredObject
 *   010208, RMZ: added functions for cookies
 *   010315, NKE: added COOKIES_ENABLED default value
 *   010320, NKE: bugfix and some optimizations of data channels
 *   010402, RMZ: modified host handling/matching for data channels
 *   010511, IPN: added ALLOW_WTLS_CLASS_1 and MASTER_GW default values.
 *   010514, NKE: added setPersistentAuthStatus
 *   010514, NKE: fixed bug in setPersistentAuthStatus
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
   DB_keyLen = 6,

   DB_flag_rec = 0,
   DB_flag_str = 3,
   DB_flag_dataExternal = 0x0080,
   DB_flag_lastElement  = 0x0100
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
   BYTE       key[DB_keyLen];

} Field, *FieldPtr;


typedef struct RecordStruct* RecordPtr;
typedef struct RecordStruct {
   ElementPtr next;
   DataUnion  data;
   UINT16     flags;
   BYTE       key[DB_keyLen];
   UINT16     ref;
   RecordPtr  nextRec; 

} Record;


/* Common types used */

/*
Typedef to store substrings of an url
*/

typedef struct UrlParts {
   BYTE* host;
   BYTE* hEnd; /* one byte past host end */
   BYTE* path;
   BYTE* pEnd; /* one byte past path end */
} UrlParts;



/*************************
 * External declarations *
 *************************/

/* Found in wipdb.c */
RecordPtr db_findRecord(DB_ref record);
FieldPtr  db_findField(const BYTE* key);
UINT8 db_deleteItem_ptr(RecordPtr rec, ElementPtr currentElem);
void db_moveToFront(RecordPtr rec, ElementPtr elem);
ElementPtr db_createNewElement (RecordPtr rec, const BYTE* key, UINT16 flags, DataUnion data,
                                ElementPtr prev, ElementPtr element, UINT8 *errorCode);

/* Found in CmmnRsrc.c */
extern const BYTE latin1_lc[256];


/***************************************
 * Common static function declarations *
 ***************************************/

static BOOL parseUrl(BYTE* url, UrlParts* urlParts);

static BOOL domainMatch(UrlParts* urlParts, BYTE* domain, BYTE* dEnd,
                        BOOL reqDotRule, BOOL emptyRule, INT32 maxPreLabels,
                        INT32* preLabels);
                        
static BOOL pathMatch(UrlParts* urlParts, BYTE* path, BOOL fullSegRule,
                      UINT32* pLen);

static BYTE* dupSubStr(const BYTE* str, const BYTE* strEnd);



/*************************************************************/
/******************  data channel handling  ******************/
/*************************************************************/

/*

For information on data channels, objects and what you can 
configure, see Users Manual. You can find all all abbreviations
for element names in capiclnt.c functions: CLNTc_setIntConfig,
CLNTc_setStrConfig, CLNTc_setDCHIntConfig, CLNTc_setDCHStrConfig.

There are three types of configure: global, template and local:

* The global configurations are saved under root/globl. They are
  valid for all objects and channels.

* The template is object 0, saved as root/glob/obj00. When
  creating a new object, the template is copied. The new object
  includes copies of all parameters and data channels. In fact,
  the object 0 is a complete channel and can be used for searching
  if one would like to.

  If changing the template, only when a new object is created, the
  changes take place.

* The local objects are saved under the root and named objxx,
  where xx is the hexadecimal representation of the object number.


STRUCTURE

The structure used is a combination of a database hierachy and
external lists containing data and pointers to database elements
This combination is not desirable. If the database is manipulated
through ordinary database operations, such as db_deleteItem, then
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
  |          |
  |          updtI --- dsplI --- hstSz
  |
  |
  dch00  ------------------------ dch01 ------------- updtI --- dsplI --- hstSz
   |                               |
   hosts --- onln --- tmout        hosts --- onln --- tmout
    |                               |
    <> --- <> --- <> ---            <> --- <> --- <> --- 
     |      |      |                 |      |      |
     string string string            string string string


At top level there are 'globl' and 'obj'. 'globl' contains the 
global constants. It hosts 'obj00', the object template. When
creating a new object, the template is copied and renamed. 

In this code, elements are created and inserted into the database
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

/* variables to collect id:s (Nbr:s) of expired channels */
static UINT8 expChNbrList[DB_maxNbrOfDataChannels];
static int   expChNbrListSize = 0;
static int   nextExpChNbrListInd = 0;

/*************
 * Functions *
 *************/

/*
 * Return pointer the struct associatied with 'channel' in the
 * channel list of 'object'. NULL is returned if 'object' or
 *'channel' does not exist.
 */
static ChannelListPtr getChannel(UINT8 object, UINT8 channel)
{
   ChannelListPtr channelList;
   int            i, j;

   /* Find channel list for object */
   for (i=0; ; i++) {
      if (i == objectListSize) 
         return NULL;
      if (objectList[i].objectNbr == object) {
         channelList = objectList[i].channelList;
         break;
      }
   }

   /* Find channel */
   for (j=0; j < objectList[i].channelListSize; j++) {
      if (channelList[j].channelNbr == channel)
         return &channelList[j];
   }
   return NULL;
}



/*
 * Return a reference to the record associated with object
 * 'object'.
 */
DB_ref getObjectRef(UINT8 object)
{
   int i;

   /* Find object */
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
   ChannelListPtr channelStruct;

   channelStruct = getChannel(object, channel);
   if (channelStruct == NULL)
      return DB_null;
   else
      return  channelStruct->channelRef;
}



/*
 * Create an record containing all mandatory fields. The record
 * is named "objxx", where xx is the hexidecimal representation
 * of 'object' in lower-case. This is done by copying the template
 * globl/obj00. If obj00 contains any predefined data channels,
 * the are copied too.
 *
 * Return a pointer to the new channel list.
 */
ObjectListPtr addObjectInternal(INT8 object) {
   BYTE           key[] = "obj??";
   ChannelListPtr channelList;
   ChannelListPtr sourceList;
   ObjectListPtr  objectRecord;
   FieldPtr       field;
   DB_ref         refO;
   DB_ref         ref;
   UINT8          error;
   UINT8          nbrOfChannels = 0;
   int            i;

   if (objectListSize == DB_maxNbrOfObjects || object == 0)
      return NULL;

   /* Create a new object by copying the template */
   ByteToHex(object, key+3);
   refO = db_copyRecord(obj00Ref, DB_root, key, DB_noPersistentSave, &error);
   if (error)
      return NULL;

   /* Create a channel list for the new object */
   channelList = (ChannelListPtr) OSConnectorAlloc(sizeof(ChannelList));
   #ifndef HAS_SETJMP
   if (channelList == NULL)
      return NULL;
   #endif

   /* Init object list struct */
    objectRecord = &objectList[objectListSize++];
   
   objectRecord->objectRef       = refO;
   objectRecord->objectNbr       = object;
   objectRecord->channelList     = channelList;
   objectRecord->defaultChannel  = objectList[0].defaultChannel;

   /* Search for channels inherited from the template */
   *(key + 0) = 'd';
   *(key + 1) = 'c';
   *(key + 2) = 'h';

   sourceList = objectList[0].channelList; /* List of numbers of template channels */
   for (i=0; i < objectList[0].channelListSize; i++) {
      ByteToHex(sourceList->channelNbr, key+3);
      ref = db_getRef(refO, key, &error);
      if (db_findRecord(ref) == NULL)
         goto Could_not_find_channel;
      field = db_findField((BYTE*) "hosts");
      if (field != NULL) {
         channelList->channelNbr = sourceList->channelNbr;
         channelList->channelRef = ref;
         channelList->hostPtr    = (RecordPtr) field;
         channelList++;
         nbrOfChannels++;
      } else
Could_not_find_channel:
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

   /* Find object */
   for (i=0; ; i++) {
      if (i == objectListSize) 
         return;
      if (objectList[i].objectNbr == object) 
         break;
   }

   ByteToHex(object, key+3);
   db_deleteItem(DB_root, key);

   OSConnectorFree(objectList[i].channelList);
   objectListSize--;
}



/*
 * Search for an object not connected to a push, WTA, template or
 * content user agent. The number of first one found with at least 
 * one configured data channel is returned. If no matching object
 * is found, '*errorCode' is set to DB_err_objectNotFound, else
 * DB_err_success.
 */
UINT8 getFirstConfiguredObject(UINT8 *errorCode)
{
   int i;

   /* Find object */
   for (i=0; i < objectListSize; i++) {
      UINT8 obj = objectList[i].objectNbr;
      if (obj != ALL_USER_AGENT && obj != CONTENT_USER_AGENT &&
         obj != REPOSITORY_USER_AGENT && obj != PUSH_USER_AGENT)
         if (objectList[i].channelListSize >= 1) {
            *errorCode = DB_err_success;
            return obj;
         }
   }

   *errorCode = DB_err_objectNotFound;
   return 255;
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

   /* Find channel list for object */
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
   refC = db_createRecord(refO, key, DB_rec, &error);
   if (error == DB_err_exists)
      return ((RecordPtr) db_findField(key))->ref;
   else if (error)
      return DB_null;
   refU = db_createRecord(refC, (BYTE*) "hosts", DB_set, &error);
   if (error) {
      db_deleteItem(refO, key);
      return DB_null;
   }

   if (objectList[i].defaultChannel == -1)
      objectList[i].defaultChannel = channel;

   nbr = objectList[i].channelListSize++;
   channelList[nbr].channelNbr = channel;
   channelList[nbr].channelRef = refC;
   channelList[nbr].hostPtr    = db_findRecord(refU);
   
   /* Fields added to the data channel */
#ifdef FEATURE_WAP
   db_setInt(refC, (BYTE*) WAP_TIMEOUT, 60);            /* Request timeout */
#else
   db_setInt(refC, (BYTE*) TIMEOUT, 60);            /* Request timeout */
#endif //#ifdef FEATURE_WAP

   db_setInt(refC, (BYTE*) ONLINE , 0);             /* Online */
   db_setInt(refC, (BYTE*) ALLOW_WTLS_CLASS_1 , 1); /* Allow WTLS class 1 */
   db_setInt(refC, (BYTE*) MASTER_GW , 1);          /* Master Gateway */
   return refC;
}



/*
 * Removes a data channel.
 * Internal use ony!
 * The inparams are indexes into the object and channel arrays.
 * The indexes must not be out of bounds when calling.
 * The last data channel is moved to the chInd position, thereby
 * replacing the channel to be deleted. The channelListSize is
 * decremented.
 * If the default channel is deleted, then a 
 * call to getDataChannel that does not result
 * in a match will cause an error code to be returned.
 */
static void deleteDataChannelInternal(int objInd, int chInd)
{
   BYTE           key[] = "dch??";
   UINT8          channel;
   ChannelListPtr channelList;
   int            lastChannel;

   channelList = objectList[objInd].channelList;
   lastChannel = objectList[objInd].channelListSize - 1;  
   channel = channelList[chInd].channelNbr;

   channelList[chInd].channelNbr = channelList[lastChannel].channelNbr;
   channelList[chInd].channelRef = channelList[lastChannel].channelRef;
   channelList[chInd].hostPtr    = channelList[lastChannel].hostPtr;
   objectList[objInd].channelListSize--;

   if (objectList[objInd].defaultChannel == channel)
      objectList[objInd].defaultChannel = -1;

   ByteToHex(channel, key+3);
   db_deleteItem(objectList[objInd].objectRef, key);
}


/*
 * Removes a data channel, if found. If the default channel is
 * deleted, then a call to getDataChannel that does not result
 * in a match will cause an error code to be returned.
 */
void deleteDataChannel(UINT8 object, UINT8 channel)
{
   ChannelListPtr channelList;
   int            lastChannel;
   int            i, j;

   /* Find object */
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
         deleteDataChannelInternal(i, j);
         break;
      }
   }
}



/*
 * Searches through all host addresses connected to the channels
 * in an object. The channel number belonging to the best
 * matching host is returned. If there is no match, then if set,
 * the default channel number is returned, or else an error occurs. 
 *
 * getDataChannel assumes the URL to be well formatted, including
 * a scheme, host and optionally a path.
 *
 * The host address connected to a channel contains a host name
 * which may be followed by a path name.
 *
 * getDataChannel matches url:s with host addresses as sepcified
 * in the WAP End to End Security Specification (section 10.3.1). 
 *
 * During getDataChannel it is checked if channels for the specified
 * object have expired. If any expired channels are found the outparam
 * expired is set, otherwise it is false. The id of any expired channels
 * for the object is remembered in an internal set (list). These channel
 * id:s may be retrieved by calling getNextExpired. Note, the internal set
 * of expired channel id:s is reset for each call of getDataChannel.
 *
 * '*errorCode' signals the return status.
 */
UINT8 getDataChannel(UINT8 object, const BYTE* url,
                     BOOL* expired, UINT8 *errorCode)
{
   ChannelListPtr channelList;
   ElementPtr     element;
   BYTE*          host;
   BYTE*          path;
   int            i, j;
   UINT32         exp;
   UrlParts       urlParts;
   int            chListSize;
   int            currIndex;
   INT32          currPreLabels;
   UINT32         currPathLen;
   INT32          preLabels;
   UINT32         pathLen;

   if (expired) *expired = FALSE;
   expChNbrListSize = 0;
   nextExpChNbrListInd = 0;

   /* Find channel list for object */
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
   
   
   /* remove expired data channels */
   j = 0;
   while (j < objectList[i].channelListSize) {
      exp = db_getInt(channelList[j].channelRef, (BYTE*) CHANNEL_EXPIRES,
                      FALSE, errorCode);
      if ((*errorCode == DB_err_success) && (exp > 0) &&
           (exp <= CLNTa_currentTime()))
      {
         if (expired && !*expired) *expired = TRUE;
         expChNbrList[expChNbrListSize] = channelList[j].channelNbr;
         expChNbrListSize++;
         deleteDataChannelInternal(i, j); /* channel j is replaced by last */
                                          /* and list size is decremented  */
      }
      else
         j++;
   }

   *errorCode = DB_err_success;         
   parseUrl((BYTE*) url, &urlParts);

   /* Search through all channels and match its host and path against */
   /* the url. Compare each match with previous best match stored in  */
   /* currIndex, currPreLabels and currPathLen. Compare matches       */
   /* according to WAP End to End Security specification              */
   currIndex = -1;
   currPreLabels = -1;
   currPathLen = 0;
   chListSize = objectList[i].channelListSize;
   for (j = 0; j < chListSize; j++) {

     /* Search through all elements in this channel */
      for (element = channelList[j].hostPtr->data.e;
           element != (ElementPtr) channelList[j].hostPtr;
           element = element->next)
      {
         host = element->data.s;
         for (path = host; (*path) && (*path != '/'); path++) ;

         if (!domainMatch(&urlParts, host, path, FALSE, TRUE, 1,
                          &preLabels))
            continue;

         /* Compare with previous best match. Minimize preLabels.        */
         /* If preLabels is smaller or equal, path has to be considered. */
         if ((currIndex >= 0) && (preLabels > currPreLabels))
            continue;
            
         if (!pathMatch(&urlParts, path, TRUE, &pathLen))
            continue;
         
         /* Change the current (best hit) if domain is more precise, */
         /* if preLabels is smaller. Otherwise if the domains are    */
         /* equally precise, change the current only if path is more */
         /* precise (maximize pathLen).                              */
         if ((currIndex < 0) ||
             (preLabels < currPreLabels) || (pathLen > currPathLen))
         {
            currIndex = j;
            currPreLabels = preLabels;
            currPathLen = pathLen;
         }

         /* Keep on searching for better hits in this channel even       */
         /* if currIndex was updated for the present j-value, since      */
         /* we otherwise would miss an even better hit in this channel.  */
         /* This could lead to reporting a channel (j-value) which does  */
         /* not contain the best hit.                                    */

      } /* end for loop over elements */
   } /* end for loop over channels */

   if (currIndex < 0) {                         /* Take default channel. */
      if (objectList[i].defaultChannel == -1) {
         *errorCode = DB_err_noDefaultChannel;
         return 255;
      }
      return (UINT8) objectList[i].defaultChannel;
   }

   return channelList[currIndex].channelNbr;
}



typedef union {
   ChannelListPtr channelStruct;
   ObjectListPtr  objectStruct;

} ObjectOrChannel;



/*
 * Return a pointer to the struct associated with 'channel'
 * if getChannel is TRUE, else return the struct associated
 * with 'object'.
 *
 * An object and a data channel are created if either is missing.
 */
static ObjectOrChannel getObjectOrChannel_add(UINT8 object, UINT8 channel, BOOL getChannel)
{
   ChannelListPtr  channelList;
   ObjectOrChannel u = {NULL};
   int             i, j;

   /* Find channel list for object */
   for (i=0; ; i++) {
      if (i == objectListSize)
         if (addDataChannel(object, channel) == DB_null) 
            return u;
         /* else caught by next if */ 
      if (objectList[i].objectNbr == object) {
         channelList = objectList[i].channelList;
         break;
      }
   }

   /* Find channel */
   for (j=0; ; j++) {
      if (j == objectList[i].channelListSize)
         if (addDataChannel(object, channel) == DB_null)
            return u;
         else
            break;
      if (channelList[j].channelNbr == channel)
         break;
   }

   if (getChannel)
      u.channelStruct = &channelList[j];
   else
      u.objectStruct  = &objectList[i];
   return u;
}



/*
 * Change the data channel to be used when no host matches when
 * calling getDataChannel.
 *
 * An object and a data channel are created if either is missing.
 */
void setDefaultDataChannel(UINT8 object, UINT8 channel)
{
   ObjectListPtr objectStruct;

   objectStruct = getObjectOrChannel_add(object, channel, FALSE).objectStruct;
   if (objectStruct != NULL)
      objectStruct->defaultChannel = channel;
}


/* getNextExpired
 *
 * getNextExpired iterates on the internal set of expired channels.
 * The internal set is reset when calling getDataChannel. getNextExpired
 * returns TRUE if outparam channelId is valid, i.e. containing the next
 * channel id in the iteration. getNextExpired returns FALSE when the
 * iteration ends and then the outparam channelId is not valid.
 * The channelId param must not be NULL.
 */
BOOL getNextExpired(UINT8* channelId)
{
   if (nextExpChNbrListInd >= expChNbrListSize)
      return FALSE;

   *channelId = expChNbrList[nextExpChNbrListInd];
   nextExpChNbrListInd++;
   return TRUE;
}


/*
 * Add a host to the specified channel. The string pointed out
 * by 'host' is copied and not taken over by the database.
 *
 * The string pointed out by 'host' must include a host name which
 * may be followed by a path name. The first '/' delimits the host from
 * the path. The host name may be the empty string.
 *
 * An object or a data channel is created if either is missing.
 *
 * 'length' is the length of host including any path but excluding an  
 * optional terminating EOL.
 */
void addHost(UINT8 object, UINT8 channel, const BYTE* host, UINT16 length)
{
   ChannelListPtr channelStruct;
   DataUnion      newHost;
   UINT8          error;

   if (host == NULL)
      return;
   if (length && (host[length-1] == '\0'))    /* Null terminated, even  */
      if (--length == 0)                      /* though it shouldn't be */ 
         return;

   channelStruct = getObjectOrChannel_add(object, channel, TRUE).channelStruct;
   if (channelStruct == NULL)
      return;

   newHost.s = dupSubStr(host, host + length);
   #ifndef HAS_SETJMP
   if (newHost.s == NULL)
      return;
   #endif

   /* Create new element */
   db_createNewElement(channelStruct->hostPtr, NULL, DB_flag_str | DB_flag_dataExternal,
                       newHost, NULL, NULL, &error);
   /* Insert new flags, when introduced in wipdb.c */
	 /* BOOKMARK:FLAGS */
}



/*
 * Remove a host, if found, from the named channel.
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
void deleteHost(UINT8 object, UINT8 channel, const BYTE* host, UINT16 length)
{
   ChannelListPtr channelStruct;
   ElementPtr     element;
   ElementPtr     prev;
   ElementPtr     hostPtr;
   BYTE*          s;
   const BYTE*    t;
   UINT16         i;
   BOOL           caseSens;

   if (host == NULL || length == 0)
      return;
   if (host[length-1] == '\0')
      if (--length == 0)
         return;

   channelStruct = getChannel(object, channel);
   if (channelStruct == NULL)
      return;
   hostPtr = (ElementPtr) channelStruct->hostPtr;

   /* Search through all channels, until a match is found */
   for (element = hostPtr->data.e, prev = NULL;
        element != (ElementPtr) hostPtr;
        prev = element, element = element->next) {
      caseSens = FALSE;
      s = element->data.s;
      for (t=host, i=length; ; s++, t++) {
         if (!caseSens && (*s == '/'))
            caseSens = TRUE;
         if (caseSens && (*s != *t))
            goto Next_loop;
         if (!caseSens && (latin1_lc[*s] != latin1_lc[*t]))
            goto Next_loop;
         if (--i == 0)
            if (!*(s+1))
               break;
            else
               goto Next_loop;
      }

      /* Delete element */
      if (prev == NULL)
         hostPtr->data.p = element->next;
      else {
         prev->next = element->next;
         if (prev->next == hostPtr)
            prev->flags |= DB_flag_lastElement;
      }
      OSConnectorFree(element->data.s); 
      OSConnectorFree(element);
      return;
Next_loop:;
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

   globalRef = db_createRecord(DB_root, (BYTE*) "globl", DB_rec, &error);
   obj00Ref  = db_createRecord(globalRef, (BYTE*) "obj00", DB_rec, &error);

   channelList = (ChannelListPtr) OSConnectorAlloc(sizeof(ChannelList));

   objectRecord = &objectList[0];
   objectRecord->objectRef       = obj00Ref;
   objectRecord->channelListSize = 0;
   objectRecord->objectNbr       = 0;
   objectRecord->defaultChannel  = -1;
   objectRecord->channelList     = channelList;

   /* Fields added to the global object */

   db_setInt(globalRef, (BYTE*) CACHE_AGE, 86400);  /* Cache age      */
   db_setInt(globalRef, (BYTE*) CACHE_MODE, 1);     /* Cache mode     */
   db_setInt(globalRef, (BYTE*) COOKIES_ENABLED, 1);/* Using cookies  */
   if (db_setStr(globalRef, (BYTE*) USER_AGENT, temp = B_STRDUP((BYTE*) "WAPPER")))
      OSConnectorFree(temp);                        /* User agent     */

   /* Fields added to the object template */

   db_setInt(obj00Ref, (BYTE*) HISTORY_SIZE, 10);   /* History size   */
   db_setInt(obj00Ref, (BYTE*) DISPLAY_IMAGES, 1);  /* Display images */
   db_setInt(obj00Ref, (BYTE*) UPDATE_IMAGES, 0);   /* Update images  */
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
  authP ---- authH
    |          |
    proxy      host
      |         |
      |         <> ------------------------------ <> ---------------------------- <> ---
      |          |                                 |
      |          host --- realm --- cred           host --- realm --- cred
      |            |        |         |             |        |         |
      |            string   string    mem           string   string    mem
      |
      |
      <> ---------------------------- <> --------------------------- <> ---
       |                               |
       addr --- port --- cred          addr --- port --- cred
        |                 |             |                 |
        string            mem           string            mem


Name    type  comment
---------------------
authP - rec   write-through
authH - rec   write-through
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

The 'authH' record is write through and thus the entire structure.
The 'authP' record is not saved persistently.

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
   for (tempRec = hostListRec->data.r; tempRec != hostListRec;
        tempRec = (RecordPtr) rec->next) {
      rec = tempRec;
      for (s = rec->data.f->data.s, t = *url; ; s++, t++) {
         if (latin1_lc[*s] != latin1_lc[*t])
            if (!*s && t == endPtr)
               break;
            else
               goto Next_loop;
         if (!*s)
            break;
      }
      /* Host found, check realm */
      if (realm != NULL) {
         for (s=rec->data.f->next->data.s, t=realm; ;s++, t++)
            if (latin1_lc[*s] == latin1_lc[*t] && !*s)
               goto Match_found;
      } else
         goto Match_found;
Next_loop:;
   }

   /* --- No match found ---*/
   *endPointer  = endPtr;
   *currentRec  = rec;
   return FALSE;
   

   /* --- Matching host and realm (if present) found --- */
Match_found:

   /* Move current record first in queue. Nessesary if several
    * realms are connected to a single host.*/
   if (hostListRec->data.r != rec) 
      db_moveToFront(hostListRec, (ElementPtr) rec);

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

   /* Update or create host information */
   if (findHost(&url, realm, &endPtr, &rec))
      db_setMem(rec->ref, (BYTE*) "cred", cred); /* Update */
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
         db_moveToFront(hostListRec, (ElementPtr) rec);
         hostAuthSize--;
         ref = rec->ref;
      } else {
         ref = db_createRecord(hostListRec->ref, NULL, DB_flag_rec, &error);   
         if (error) {
            OSConnectorFree(host);
            OSConnectorFree(cred);
            return FALSE;
         }
      }
      /* Order significant if creating new record; reverse list order */
      error  = db_setMem(ref, (BYTE*) "cred",  cred);
      error |= db_setStr(ref, (BYTE*) "realm", B_STRDUP(realm));
      error |= db_setStr(ref, (BYTE*) "host",  host);
      if (error)
         db_deleteItem_ptr(hostListRec, (ElementPtr) db_findRecord(ref));
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
   db_clearRecord(hostListRec->ref);
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
   for (tempRec=proxyListRec->data.r; tempRec != proxyListRec;
        tempRec=(RecordPtr) rec->next) {
      rec = tempRec;

      s=rec->data.f->data.s;
      sLen = (UINT8)*(UINT16*) s;
      for (s += 2, t=address, len=0; len < addressLength && len < sLen; s++, t++, len++) {
         if (*s != *t)
            goto Next_loop;
      }
      /* Address found, check realm */
      if (portNumber == rec->data.f->next->data.i)
         goto Match_found;   

Next_loop:;
   }

   /* --- No match found ---*/
   *currentRec  = rec;
   return FALSE;
   

   /* --- Matching address and port found --- */
Match_found:

   /* Move current record first in queue. When list is full, 
    * the least recently used item is deleted. */
   if (proxyListRec->data.r != rec)
      db_moveToFront(proxyListRec, (ElementPtr) rec);
  
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
      db_setMem(rec->ref, (BYTE*) "cred", cred);
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
         db_moveToFront(proxyListRec, (ElementPtr) rec);
         proxyAuthSize--;
         ref = rec->ref;
      } else {
         ref = db_createRecord(proxyListRec->ref, NULL, DB_flag_rec, &error);  
         if (error) {
            OSConnectorFree(cred);
            OSConnectorFree(addr);
            return FALSE;
         }
      }
      /* Order significant if creating new record; reverse list order */
      error  = db_setMem(ref, (BYTE*) "cred",  cred); 
      error |= db_setInt(ref, (BYTE*) "port", portNumber);
      error |= db_setMem(ref, (BYTE*) "addr", addr);
      if (error)
         db_deleteItem_ptr(proxyListRec, (ElementPtr) db_findRecord(ref));
      else
         proxyAuthSize++;
   }

   return TRUE;
}



/*
 * Search the database and return the credentials connected to
 * the host of 'address' and 'portNumber'.
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
 * Remove all proxy authentication data.
 */
void deleteProxyAuth(void)
{
   db_clearRecord(proxyListRec->ref);
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
   while (rec != parent) {
      tempElem = (ElementPtr) rec;
      rec = (RecordPtr) rec->next;
      if (tempElem->data.e == tempElem ||
          tempElem->data.e->next == tempElem ||
          tempElem->data.e->next->next == tempElem)
         db_deleteItem_ptr(parent, tempElem);
      else
         size++;
   }
   return size;
}


/*
 * Turn on or off persistent status for host or proxy 
 * authentication. 'hostNotProxy' is TRUE for host and FALSE
 * for proxy. 'status' is
 * 0 - no persistent save
 * 1 - backup
 * 2 - write-through
 *
 * Return error code.
 */
UINT8 setPersistentAuthStatus (BOOL hostNotProxy, int status)
{
	DB_ref ref;
	UINT16 flags;
	UINT8  error;

	ref = db_getRef (DB_root, hostNotProxy ? (BYTE*) "authH" : (BYTE*) "authP", &error);
	if (error)
		return error;

	switch (status) {
	case 0:
		flags = DB_noPersistentSave;
		break;
	case 1:
		flags = DB_backup;
		break;
	default:
		flags = DB_writeThrough;
		break;
	}

  ref = db_moveRecord (ref, DB_root, DB_root, flags, &error);
	if (error)
		return error;

  if (hostNotProxy)
    hostListRec  = db_findRecord (db_getRef (ref, (BYTE*) "host", &error));
  else
    proxyListRec = db_findRecord (db_getRef (ref, (BYTE*) "proxy", &error));
	return error;
}


/*
 * Initiate authentication lists. If first time, create host and
 * proxy write through list, else verify the correctness of the
 * list loaded from persistent memory.
 *
 * Must be called after db_dbInit.
 */
void authenticateInit(void)
{
	DB_ref authRef;
	DB_ref ref;
	UINT8  error;

	/* Create proxy database structure */
	authRef = db_createRecord(DB_root, (BYTE*) "authP", DB_rec, &error);
	
	ref = db_createRecord(authRef, (BYTE*) "proxy", DB_set, &error);
	proxyAuthSize = 0;
	proxyListRec = db_findRecord(ref);
	/* proxyAuthSize = checkAuthList(proxyListRec); Not needed if no persistent save */


	/* Create or load host database structure */
	authRef = db_getRef(DB_root, (BYTE*) "authH", &error);
	if (error) {
		/* First time, no copy found on persistent memory. */
		authRef = db_createRecord(DB_root, (BYTE*) "authH",
			DB_rec | DB_writeThrough, &error);
		
		hostAuthSize  = 0;
		
		ref = db_createRecord(authRef, (BYTE*) "host",  DB_set, &error);
		hostListRec = db_findRecord(ref);
		
	} else {
		hostListRec  = db_findRecord(db_getRef(authRef, (BYTE*) "host",  &error));
		
		hostAuthSize  = checkAuthList(hostListRec);
	}
}



/*
 * Terminate authentication.
 *
 * Must be called before db_dbTerminate.
 */
void authenticateTerminate(void)
{
}








/*************************************************************/
/**********************     Cookies    ***********************/
/*************************************************************/

/*

STRUCTURE

The rules for database structures are:
 - compound nodes are records (RecordStruct)
 - leaf nodes with names are fields (FieldStruct)
 - leaf nodes without names are elements (ElementStruct)
In addition, a record may be used as a set in which case its members
are namesless. This implies that in a set with leaf members, the 
members are elements. In a set with compund members, the members are
records but the names are not used in them.

All lists in the database are circular.

The database structure used to save cookies is arranged like this:

root
 |
 coo
  |
  <> ---------------------------------- <> ---------------------------------
   |                                     |
   name - val - ver - dom - path - exp   name - val - ver - dom - path - exp


Key/Name      elem-type  value-type    comment
---------------------------------------------------------------------
coo/cookies   rec        rec list,     cookie list, backup
                         set  
<>            rec        field list    a cookie, nameless record
name/name     field      str           mandatory
val/value     field      str           mandatory
ver/version   field      int           mandatory
dom/domain    field      str           mandatory
path/path     field      str           optional
exp/expires   field      int           optional

The cookie components/fields domain, path and name identifies a cookie.
Domain and name are case insensitive, while path is case sensitive.
The order of component/fields for a cookie is significant. The order is
as listed above.
(Ordering was introduced to speed up access, not using the get<field>
access functions which searches keywords, but instead directly position
field pointers based on the ordering. This is the same technique as
used in the authenticate part of the database.)

The order of elements within the cookie list is significant.
The cookie list is in recently used order with the least recently used, last.

A counter cookieListSize is used for saving the list lengths.
It can be at most cfg_wae_wspif_CookieItems elements long. 

If new optional components are added in the future, these should be
assigned a position among the other optionals last in the component list.
In this case one should evaluate to let all optionals still be last in the
list but among each other without any specific ordering. If this will be
chosen, optionals are probably best accessed using get<Field> functions and
the faster access used presently of the path component must be abandoned.


SEARCHING

Searching is done giving an url as input, finding any cookies with
a matching domain and path.

A match as defined in rfc2109 is:
 - the url domain part is domain matched against the cookie domain,
   domains are case insensitive
 - the cookie path must be a prefix of the url path part,
   paths are case sensitive

The found cookies must be returned in the following order:
 - for a specific domain, more specific paths precedes less specific paths,
 - the order between domains is unspecified.

All matched cookies will be stored internally. The found cookies are retrieved 
one by one calling getNextCookie().

During the search, the expires date is checked and
if a cookie is expired it will be removed from the database.

All matching cookies found during the search, are moved to the beginning of
the cookie list to keep the list in recently used order.

*/



/*************
 * Constants *
 *************/


/*********
 * Types *
 *********/

/*
Typedefs for a list of integers.
*/

typedef struct IntItem* IntItemPtr;
typedef struct IntItem {
   IntItemPtr next;
   UINT32     i;
} *IntListPtr;


/*
Typedef for cookie id
*/

typedef struct CookieId {
   BYTE* name; /* null terminated string */
   BYTE* domain;
   BYTE* dEnd; /* one byte past domain end */
   BYTE* path;
   BYTE* pEnd; /* one byte past path end */
} CookieId;


/*************
 * Variables *
 *************/

static RecordPtr  cookiesPtr;     /* pointer to cookies record/set */
static UINT8      cookieListSize; /* size of cookie list */

static IntListPtr pathLenList;    /* sorted path lengths used when */
                                  /* searching for cookies */
static RecordPtr  nextCookie;     /* next found cookie to return */
static RecordPtr  lastCookie;     /* last found cookie to return */

/*
The items in pathLenList are accumulated, i.e. items are not deleted as long
as the database is open.
*/

/********************************
 * Static function declarations *
 ********************************/

static void deleteExpired(void);

static void setCookieId(CookieId* cookieId, UrlParts* urlParts,
                        BYTE* name, BYTE* domain, BYTE* path);

static RecordPtr getCookie(CookieId* cookieId, RecordPtr* last);

static BOOL setCookie(DB_ref ref, BOOL created, CookieId* cookieId,
                      BYTE* value, UINT32* expires, UINT32 version);

static BOOL updateCookie(DB_ref ref, BYTE* value, UINT32* expires,
                         UINT32 version);

static void moveElement(RecordPtr parent, ElementPtr prevPtr, ElementPtr ePtr,
                        UINT32 pos);

static UINT8 checkCookieList(RecordPtr parent);


/************************
 * Function definitions *
 ************************/



/* Stores a cookie.
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
 * All input parameters are copied and stored.
 *
 * Returns TRUE if successfull.
 */
BOOL storeCookie(BYTE* url, BYTE* name, BYTE* value, BYTE* domain,
                 BYTE* path, UINT32* expires, UINT32 version)
{
   UrlParts  urlParts;
   CookieId  cookieId;
   BYTE*     s;
   BYTE*     t;
   RecordPtr rPtr;
   RecordPtr lastPtr;
   DB_ref    ref;
   UINT8     error;
   BOOL      created;
   BOOL      res;

   if (!url || !name || !*name || (!value && (!expires || *expires)))
      return FALSE;

   /* treat empty domain and path strings as NULL */
   if (domain && !*domain) domain = NULL;
   if (path && !*path) path = NULL;

   /* invalidate search result and delete expired cookies */
   nextCookie = NULL;
   lastCookie = NULL;
   deleteExpired();

   if (!parseUrl(url, &urlParts))
      return FALSE;

   if (domain) {
      if (!domainMatch(&urlParts, domain, NULL, TRUE, FALSE, 1, NULL))
         return FALSE;
   } else {
      /* check that url host part has embedded dot */
      s = urlParts.host + 1;
      t = urlParts.hEnd;
      while ((s != t) && (*s++ != '.'));
      if (s == t)
         return FALSE;
   }

   if (path && !pathMatch(&urlParts, path, FALSE, NULL))
      return FALSE;

   /* sets cookie id: name, domain & path, for the input cookie */
   /* if needed, default values are set from urlParts */
   setCookieId(&cookieId, &urlParts, name, domain, path);

   /* check if cookie already exists, i.e. compare cookie id:s */
   rPtr = getCookie(&cookieId, &lastPtr);

   /* check if we must delete any found cookie */
   if (expires && (*expires <= CLNTa_currentTime())) {
      if (rPtr) {
         error = db_deleteItem_ptr(cookiesPtr, (ElementPtr) rPtr);
         cookieListSize--;
      }
      else
         error = 0;
      return !error;
   }

   if (!rPtr) {                                        /* cookie not found */
      if (cookieListSize == cfg_wae_wspif_CookieItems) {
         /* List full, reuse the least recently used, i.e. last record */
         db_moveToFront(cookiesPtr, (ElementPtr) lastPtr);
         ref = lastPtr->ref;
         created = FALSE;
      } else {
         ref = db_createRecord(cookiesPtr->ref, NULL, DB_rec, &error);
                                              /* creates it first in list */
         if (error) return FALSE;
         created = TRUE;
         cookieListSize++;
      }
      res = setCookie(ref, created, &cookieId, value, expires, version);
   } else {                                               /* cookie found */
      db_moveToFront(cookiesPtr, (ElementPtr) rPtr);
      ref = rPtr->ref;
      res = updateCookie(ref, value, expires, version);
   }

   if (!res) {
      db_deleteItem_ptr(cookiesPtr, (ElementPtr) rPtr);
      cookieListSize--;
   }
   
   return res;
}



/*
 * Searches the database for all cookies valid for the passed in url.
 * To retrieve the result of the search, call getNextCookie repeatedly
 * unitl false is returned.
 *
 * This function is intended to be called before a http request.
 * The url must be well formed containing the domain and optionally
 * the path.
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
 * Calling storeCookie, checkCookie, or deleteAllCookies
 * invalidates the internally stored search result from the previous
 * checkCookie call.
 *
 * Side effect: during the search, the expires date is checked and
 *              if a cookie is expired it will be removed from the database.
 *
 * The cookie list will be kept in least recently used order.
 * All cookies found to match in this function are regarded as used
 * simultanously. Therefore, the matched cookies are put in the first
 * positions of the cookie list. The order of these matched cookies
 * are with more specific (longer) paths first.
 *
 * To sort the matched cookies, a helping list pathLenList is used which
 * stores path lengths in order.
 *
 * Returns TRUE if one or more cookies matched.
 */
BOOL checkCookies(BYTE* url)
{
   UrlParts   urlParts;
   UINT32     matched;
   RecordPtr  rPtr;
   RecordPtr  prevRPtr;
   RecordPtr  nextRPtr;
   FieldPtr   fPtr;
   BYTE*      domain;
   BYTE*      path;
   UINT32     pLen;
   UINT32     pos;
   IntItemPtr freeLenPtr;
   IntItemPtr prevFreeLenPtr;
   IntItemPtr lenPtr;
   IntItemPtr prevLenPtr;
   IntItemPtr matchedLenPtr;

   /* invalidate search result and delete expired cookies */
   nextCookie = NULL;
   lastCookie = NULL;
   deleteExpired();

   if (!parseUrl(url, &urlParts))
      return FALSE;

   matched = 0;
   freeLenPtr = pathLenList;
   prevFreeLenPtr = NULL;
   prevRPtr = NULL;
   rPtr = cookiesPtr->data.r;
   while (rPtr != cookiesPtr) {
      fPtr = (FieldPtr) (rPtr->data.f->next->next->next);
      domain = fPtr->data.s;
      fPtr = (FieldPtr) fPtr->next;
      if ((fPtr != (FieldPtr) rPtr) &&
          (B_COMPARESTRING(fPtr->key, COOKIE_PATH) == 0))
         path = fPtr->data.s;
      else
         path = NULL;

      if (!domainMatch(&urlParts, domain, NULL, FALSE, FALSE, -1, NULL) ||
          !pathMatch(&urlParts, path, FALSE, &pLen)) {
         prevRPtr = rPtr;
         rPtr = (RecordPtr) rPtr->next;
         continue;
      }

      /* cookie matched */

      /*  reuse or create an IntItem */
      if (freeLenPtr) {
         /* unlink the next free IntItem */
         matchedLenPtr = freeLenPtr;
         if (prevFreeLenPtr)
            prevFreeLenPtr->next = matchedLenPtr->next;
         else
            pathLenList = matchedLenPtr->next;
         freeLenPtr = matchedLenPtr->next;
      } else {
         matchedLenPtr = OSConnectorAlloc(sizeof(struct IntItem));
		   #ifndef HAS_SETJMP
		   if (matchedLenPtr == NULL) {
			   return FALSE;
         }
		   #endif
      }

      /* sort the IntItem into pathLenList */
      /* since it is relatively small, use linear search */
      /* longest path first */
      pos = 0;
      lenPtr = pathLenList;
      prevLenPtr = NULL;
      while ((pos < matched) && (pLen <= lenPtr->i)) {
         pos++;
         prevLenPtr = lenPtr;
         lenPtr = lenPtr->next;
      }
      matchedLenPtr->i = pLen;
      matchedLenPtr->next = lenPtr;
      if (prevLenPtr)
         prevLenPtr->next = matchedLenPtr;
      else
         pathLenList = matchedLenPtr;

      nextRPtr = (RecordPtr) rPtr->next;
      /* move the record ahead in the cookie list to the position in pos */
      moveElement(cookiesPtr, (ElementPtr) prevRPtr, (ElementPtr) rPtr, pos);

      if (pos == matched) {    /* matched cookie put last in search result */
         lastCookie = rPtr;
         freeLenPtr = matchedLenPtr->next;
         prevFreeLenPtr = matchedLenPtr;
      }
      matched++;

      if ((ElementPtr) nextRPtr == rPtr->next)
         prevRPtr = rPtr;
      rPtr = nextRPtr;

   } /* end of while loop over cookies */

   if (matched)
      nextCookie = cookiesPtr->data.r;

   return matched;
}


/*
 * Retrieves the next cookie from the set of matched cookies found
 * in a previous checkCookies() call.
 *
 * This function should be called repeatedly after a checkCookies call
 * until getNextCookie returns false.
 *
 * Cookies are returned in the following order:
 *  - more specific paths precedes less specific paths,
 *
 * getNextCookie returns the cookie components as out-paramameters.
 * The out-parameters are not copied out, so the database owns
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
                   UINT32* version)
{
   FieldPtr fPtr;

   if (!nextCookie) return FALSE;

   fPtr = nextCookie->data.f;
   if (name) *name = fPtr->data.s;

   fPtr = (FieldPtr) fPtr->next;
   if (value) *value = fPtr->data.s;

   fPtr = (FieldPtr) fPtr->next;
   if (version) *version = fPtr->data.i;

   fPtr = (FieldPtr) fPtr->next;
   if (domain) *domain = fPtr->data.s;

   if (path) {
      fPtr = (FieldPtr) fPtr->next;
      if ((fPtr != (FieldPtr) nextCookie) &&
           B_COMPARESTRING(fPtr->key, COOKIE_PATH) == 0)
         *path = fPtr->data.s;
      else   
         *path = NULL;
   }

   nextCookie = nextCookie != lastCookie ? (RecordPtr) nextCookie->next : NULL;

   return TRUE;
}


/*
 * RESET COOKIE ITERATION
 *
 *	Resets the iteration done via getNextCookie to the first cookie in
 * the set of matched cookies if any. That is, after this function
 * has been called, a call to getNextCookie will return the first
 * matched cookie if any. 
 * If the search result is invalidated, nothing is done in this function.
 */
void resetCookieIter(void)
{
   if (lastCookie)
      nextCookie = cookiesPtr->data.r;
}


/*
 * Removes all cookies.
 * At present, cookies are just removed from RAM,
 * not from persistent memory.
 */
void deleteAllCookies(void)
{
   nextCookie = NULL;
   lastCookie = NULL;

   db_clearRecord(cookiesPtr->ref);
   cookiesPtr->data.r = cookiesPtr;
   cookieListSize = 0;
}




/* ======== admin ======== */




/*
 * Initiate cookies lists. If first time, create the cookies list,
 * else verify the correctness of the list loaded from persistent memory.
 * Since cookies with missing expires component, isn't stored in
 * persistent memory, i.e. when closing down, the expires component
 * must be present when initiating. Any expired cookies will not
 * be created in RAM.
 *
 * Must be called after db_dbInit.
 */
void cookiesInit(void)
{
   DB_ref ref;
   UINT8  error;

   ref = db_getRef(DB_root, (BYTE*) COOKIE_LIST, &error);
   if (error) /* no copy found on persistent memory. */
      ref = db_createRecord(DB_root, (BYTE*) COOKIE_LIST, DB_set | DB_backup,
                            &error);

   cookiesPtr = db_findRecord(ref);
   cookieListSize = checkCookieList(cookiesPtr);

   pathLenList = NULL;
   nextCookie = NULL;
   lastCookie = NULL;

}



/*
 * Terminate cookies.
 *
 * Must be called before db_dbTerminate.
 *
 * When terminating, cookies with missing expires component will be deleted.
 * All remaining non-expired cookies will be stored on persistent media
 * on close-down.
 */
void cookiesTerminate(void)
{
   IntItemPtr pLenPtr;

   nextCookie = NULL;
   lastCookie = NULL;
   while (pathLenList) {
      pLenPtr = pathLenList->next;
      OSConnectorFree(pathLenList);
      pathLenList = pLenPtr;
   }
   pathLenList = NULL;

   checkCookieList(cookiesPtr);
   cookiesPtr = NULL;
   cookieListSize = 0;
}



/* ======== static cookie functions ======== */

/* Deletes expired cookies */
static void deleteExpired(void)
{
   RecordPtr rec;
   RecordPtr tempCookie;
   UINT8     error;
   UINT32    expires;

   rec = cookiesPtr->data.r;
   while (rec != cookiesPtr) {
      tempCookie = rec;
      rec = (RecordPtr) rec->next;
#ifdef FEATURE_WAP
      expires = db_getInt(tempCookie->ref, (const BYTE*)COOKIE_EXPIRES, FALSE, &error);
#else
      expires = db_getInt(tempCookie->ref, COOKIE_EXPIRES, FALSE, &error);
#endif //#ifdef FEATURE_WAP

      if (!error && (expires <= CLNTa_currentTime())) {  /* expired */
         db_deleteItem_ptr(cookiesPtr, (ElementPtr) tempCookie);
         cookieListSize--;
      }
   }
}


/* setCookieId
 * Sets the passed in cookieId which must not be NULL.
 * The cookieId is set from the other inparameters.
 * The inparameter name must not be NULL.
 * The inparameters domain and path may be NULL
 * in which case the corresponding parts of the cookieId
 * will get default values. The default values are taken
 * from the urlParts which must not be NULL.
 */
static void setCookieId(CookieId* cookieId, UrlParts* urlParts,
                        BYTE* name, BYTE* domain, BYTE* path)
{
   BYTE* p;
   BYTE* pEnd;

   cookieId->name = name;
   
   if (domain) {
      cookieId->domain = domain;
      cookieId->dEnd = domain + B_STRINGLENGTH(domain);
   } else {
      cookieId->domain = urlParts->host;
      cookieId->dEnd = urlParts->hEnd;
   }

   if (path) {
      cookieId->path = path;
      cookieId->pEnd = path + B_STRINGLENGTH(path);
   } else {
      /* find rightmost slash in url path */
      p = urlParts->path;
      pEnd = urlParts->pEnd;
      while (pEnd != p) {
         pEnd--;
         if (*pEnd == '/') break;
      }
      cookieId->path = p;
      cookieId->pEnd = pEnd;
   }

   if (cookieId->path == cookieId->pEnd) {
      cookieId->path = NULL;
      cookieId->pEnd = NULL;
   }
}


   
/* getCookie
 * 
 * Gets a cookie with the given id in cookieId from the cookie list. 
 * The cookie id is the triplet name, domain and path.
 *
 * In case a cookie with the given id could not be found, the last
 * cookie is returned in *last if the caller passed in a non-NULL last param.
 * In all other cases any *last out value is not valid.
 * Note, the caller may pass a NULL last param, if finding the last cookie
 * is not of interest.
 *
 * Returns a pointer to the cookie if found otherwise NULL is returned.
 */
static RecordPtr getCookie(CookieId* cookieId, RecordPtr* last)
{
   RecordPtr  rPtr;
   RecordPtr  lastPtr;
   FieldPtr   fPtr;
   BYTE*      s;
   BYTE*      t;
   BYTE*      u;

   if (last) *last = NULL;
   lastPtr = NULL;
   for (rPtr = cookiesPtr->data.r; rPtr != cookiesPtr; rPtr = (RecordPtr)
                                                              rPtr->next) {
      lastPtr = rPtr;
      /* check name, case unsensitive */
      fPtr = rPtr->data.f;
      for (s = fPtr->data.s, t = cookieId->name; *s; s++, t++)
         if (latin1_lc[*s] != latin1_lc[*t]) break;
      if (*s || *t) continue;

      /* check domain, case unsensitive */
      fPtr = (FieldPtr) fPtr->next->next->next;
      t = cookieId->domain;
      u = cookieId->dEnd;
      for (s = fPtr->data.s; *s && (t != u); s++, t++)
         if (latin1_lc[*s] != latin1_lc[*t]) break;
      if (*s || (t != u)) continue;

      /* check path, case sensitive */
      fPtr = (FieldPtr) fPtr->next;
      if ((fPtr != (FieldPtr) rPtr) &&
          (B_COMPARESTRING(fPtr->key, COOKIE_PATH) != 0))
         fPtr = (FieldPtr) rPtr;
      if ((fPtr == (FieldPtr) rPtr) && !(cookieId->path))
         return rPtr;
      if (fPtr != (FieldPtr) rPtr) {
         t = cookieId->path;
         u = cookieId->pEnd;
         for (s = fPtr->data.s; *s && (t != u); s++, t++)
            if (*s != *t) break;
         if (!*s && (t == u))
            return rPtr;
      }
   }

   if (last) *last = lastPtr;
   return NULL;
}


/* SetCookie.
 *
 * Sets all cookie components for the cookie with reference ref.
 *
 * For mandatory component values, NULL must not be passed in.
 *
 * All component input parameters are copied and stored.
 *
 * Order of components are significant.
 * The inparameter created must be true if the cookie record was
 * newly created without any components.
 * If creating new record components, reverse list order.
 *
 * If the cookie and its components already exists and is reused,
 * create must be false.
 *
 * Note that when a cookie is reused, the optional components may
 * be created fresh in this function. In this case the function
 * checks and adjusts so the optional components, path and expire, 
 * are last when this function returns.
 *
 * Set functions takes delete responsibility for values passed in,
 * except when they return error.
 *
 * Returns TRUE if successfull.
 */
static BOOL setCookie(DB_ref ref, BOOL created, CookieId* cookieId,
                      BYTE* value, UINT32* expires, UINT32 version)
{
   UINT8     err;
   BYTE*     str;
   RecordPtr rPtr;
   FieldPtr  fPtr;
   FieldPtr  tmpPathPtr;

   /* the following do/while construct is not repeated */
   /* it is only used to be able to break out to error handling */
   do {
      if (expires) {
         err = db_setInt(ref, (BYTE*) COOKIE_EXPIRES, *expires);
         if (err) break;
      }

      if (cookieId->path) {
         str = dupSubStr(cookieId->path, cookieId->pEnd);
         err = db_setStr(ref, (BYTE*) COOKIE_PATH, str);
         if (err) break;
      }

      str = dupSubStr(cookieId->domain, cookieId->dEnd);
      err = db_setStr(ref, (BYTE*) COOKIE_DOMAIN, str);
      if (err) break;

      str = NULL;
      err = db_setInt(ref, (BYTE*) COOKIE_VERSION, version);
      if (err) break;

      str = B_STRDUP(value);
      err = db_setStr(ref, (BYTE*) COOKIE_VALUE, str);
      if (err) break;

      str = B_STRDUP(cookieId->name);
      err = db_setStr(ref, (BYTE*) COOKIE_NAME, str);
      if (err) break;
   } while (FALSE);

   if (err) {
      if (str) OSConnectorFree(str);
      return FALSE;
   }

   if (!created) {
#ifdef FEATURE_WAP
      if (!(cookieId->path)) db_deleteItem(ref,(const BYTE*)COOKIE_PATH);
      if (!expires) db_deleteItem(ref, (const BYTE*)COOKIE_EXPIRES);
#else
      if (!(cookieId->path)) db_deleteItem(ref, COOKIE_PATH);
      if (!expires) db_deleteItem(ref, COOKIE_EXPIRES);
#endif //#ifdef FEATURE_WAP


      /* The optional components might have been created above even if the */
      /* cookie record is not newly created. If some of the optional       */
      /* components were created they are at the beginning of the          */
      /* component list. Move them to their proper position.               */
      if (cookieId->path || expires) {
         rPtr = db_findRecord(ref);
         if (!rPtr) return FALSE;
         tmpPathPtr = NULL;
         fPtr = rPtr->data.f;
         if (cookieId->path && B_COMPARESTRING(fPtr->key, COOKIE_PATH) == 0) {
            /* we cannot know if we should move path to pos 4 or 5 here */
            tmpPathPtr = fPtr;
            fPtr = (FieldPtr) fPtr->next;
         }
         if (expires && B_COMPARESTRING(fPtr->key, COOKIE_EXPIRES) == 0)
            /* move expires to end, tmpPathPtr is either NULL or the */
            /* previous element */
            moveElement(rPtr, (ElementPtr) tmpPathPtr, (ElementPtr) fPtr, 5);
         if (tmpPathPtr)
            /* expire cannot be in pos 1 here, so its safe to move path */
            /* to its proper position */
            moveElement(rPtr, NULL, (ElementPtr) tmpPathPtr, 4);
      }

   }
   return TRUE;
}


/* updateCookie.
 *
 * Updates the cookie components value, expires and version.
 *
 * Returns TRUE if successfull.
 */
static BOOL updateCookie(DB_ref ref, BYTE* value, UINT32* expires,
                         UINT32 version)
{
   UINT8     error;
   BYTE*     str;
   RecordPtr rPtr;
   FieldPtr  fPtr;

   str = B_STRDUP(value);
   error = db_setStr(ref, (BYTE*) COOKIE_VALUE, str);
   if (error) {
      if (str) OSConnectorFree(str);
      return FALSE;
   }

   error = db_setInt(ref, (BYTE*) COOKIE_VERSION, version);

   if (expires) {
      error |= db_setInt(ref, (BYTE*) COOKIE_EXPIRES, *expires);
      if (error) return FALSE;
      /* expires might have been created and then it is first in the list */
      /* if so, move it to end */
      rPtr = db_findRecord(ref);
      if (!rPtr) return FALSE;
      fPtr = rPtr->data.f;
      if (B_COMPARESTRING(fPtr->key, COOKIE_EXPIRES) == 0)
         moveElement(rPtr, NULL, (ElementPtr) fPtr, 6);
   }
   else
#ifdef FEATURE_WAP
      db_deleteItem(ref, (const BYTE*)COOKIE_EXPIRES);
#else
      db_deleteItem(ref, COOKIE_EXPIRES);
#endif //#ifdef FEATURE_WAP


   return !error;
}


/* moveElement
 *
 * Moves the element ePtr to position pos in the list held by parent.
 * The inparameters parent and ePtr must not be NULL.
 * Positions are counted from 0 and the position moved to is as counted
 * after this function is finnished.
 *
 * If pos is greater than the position of the last element, the ePtr element
 * is moved to the last position.
 * The element just before ePtr must be supplied in prevPtr, except when
 * ePtr is the first element in the list, in which case prevPtr must be NULL.
 */
static void moveElement(RecordPtr parent, ElementPtr prevPtr, ElementPtr ePtr,
                        UINT32 pos)
{
   ElementPtr  iter;
   ElementPtr  prevIter;

   /* check and move last flag */
   if (ePtr->next == (ElementPtr) parent)
   {
      ePtr->flags &= ~DB_flag_lastElement;
      if (prevPtr)
         prevPtr->flags |= DB_flag_lastElement;
   }

   /* unlink ePtr */
   if (prevPtr)
      prevPtr->next = ePtr->next;
   else
      parent->data.e = ePtr->next;

   iter = parent->data.e;
   prevIter = NULL;
   while (pos && (iter != (ElementPtr) parent)) {
      prevIter = iter;
      iter = iter->next;
      pos--;
   }

   /* link in ePtr in the new position */
   if (prevIter)
      prevIter->next = ePtr;
   else
      parent->data.e = ePtr;
   ePtr->next = iter;

   /* check and set last flag */
   if (iter == (ElementPtr) parent) {
      if (prevIter)
         prevIter->flags &= ~DB_flag_lastElement;
      ePtr->flags |= DB_flag_lastElement;
   }
}


/* checkCookieList
 *
 * Count number of elements in cookies list. Delete elements
 * that are not complete, i.e. some data is missing. Also
 * delete expired cookies and cookies without an expires
 * component.
 *
 * Return size of list.
 */
static UINT8 checkCookieList(RecordPtr parent)
{
   RecordPtr rec;
   RecordPtr tempCookie;
   RecordPtr tempElem;
   UINT8     i;
   UINT8     error;
   UINT32    expires;
   UINT8     size;

   size = 0;
   rec = parent->data.r;
   while (rec != parent) {
      tempCookie = rec;
      tempElem = tempCookie->data.r;
      rec = (RecordPtr) rec->next;
      /* count mandatory fields */
      for(i = 0; (tempElem != tempCookie) && (i < 4); i++)
         tempElem = (RecordPtr) tempElem->next;
#ifdef FEATURE_WAP
      expires = db_getInt(tempCookie->ref, (const BYTE*)COOKIE_EXPIRES, FALSE, &error);
#else
      expires = db_getInt(tempCookie->ref, COOKIE_EXPIRES, FALSE, &error);
#endif //#ifdef FEATURE_WAP

      if ((i < 4) ||                         /* 4 mand. fields not present */
          (error != DB_err_success) ||                  /* expires missing */
          (expires <= CLNTa_currentTime()))                     /* expired */
         db_deleteItem_ptr(parent, (ElementPtr) tempCookie);
      else
         size++;
   }
   return size;
}



/***************************************************************/
/******************  common static functions  ******************/
/***************************************************************/


/* parseUrl
 *
 * Parses an URL and finds the host and path parts of it.
 * This function sets the start and end pointers of the host and path
 * sub-strings provided in urlParts. The end pointers are one byte past the
 * host and path end respectively. If no path part could be found, then
 * urlParts->path and urlParts->pEnd are NULL upon return.
 *
 * The caller must not pass in any NULL values.
 *
 * Returns TRUE if the host part could be found, in which case
 * urlParts->host != urlParts->hEnd.
 */
static BOOL parseUrl(BYTE* url, UrlParts* urlParts)
{
   BYTE* host;
   BYTE* hEnd;
   BYTE* path;
   BYTE* pEnd;

   /* The url is known to be a well formatted http/https URL */
   /* find host and path part */
   host = url;
   while (*host++ != ':')
      ;
   host += 2;
   for (hEnd = host; *hEnd != '\0' && *hEnd != '/' && *hEnd != ':' &&
                     *hEnd != '?'  && *hEnd!='#'; hEnd++)
      ;

   for (path = hEnd; *path != '\0' && *path != '/' &&
                     *path != '?'  && *path != '#'; path++)
      ;
   for (pEnd = path;  *pEnd != '\0' && *pEnd != ';' &&
                      *pEnd != '?'  && *pEnd != '#'; pEnd++)
      ;

   urlParts->host = host;
   urlParts->hEnd = hEnd;
   if (path != pEnd) {
     urlParts->path = path;
     urlParts->pEnd = pEnd;
   } else {
     urlParts->path = NULL;
     urlParts->pEnd = NULL;
   }

   return (host != hEnd);
}


/* domainMatch
 *
 * Checks if a url matches a given domain.
 *
 * A match as defined in rfc2109 and in WAP Provisioning User Agent Behaviour
 * (sect. 7) is basically:
 *  - the url domain part is domain matched against the cookie domain,
 *    domains are case insensitive
 *
 * Additional match criterias may apply depending on specification and
 * context. These additional criterias are controlled by input parameters.
 *
 * The url is given in an UrlParts structure.
 * The urlParts parameter must not be NULL.
 * The domain matched against, is given in domain which must not be NULL.
 * The dEnd inparameter is either NULL or points one byte past the end 
 * of the domain string. If dEnd is NULL the domain string must be 
 * null terminated.
 *
 * The reqDotRule, emptyRule and maxPreLabels params are parameters that may
 * affect the matching:
 *   - if reqDotRule is set, the match criteria is extended with:
 *       a domain must not be a top domain
 *   - if emptyRule is set, the match criteria is extended with:
 *       an empty domain matches all urls:s
 *   - if a maxPreLabels value > 0 is passed in, the match criteria is
 *     extended with:
 *       an url host of the form HD must not contain more than maxPreLabels
 *       labels in H where D is the value of the domain and which starts 
 *       with '.'.
 *
 * If any extended criteria regarding maximum numbers of labels, shouldn't
 * apply, pass in a maxPreLabels value == -1.
 *
 * Independent of any extended matching criterias, the caller may retrive
 * the number of labels in any prefix part. This is done via preLabels
 * if a non NULL value is supplied. I.e. NULL may be passed for preLabels
 * if the caller is not interested in this kind of information.
 *
 * Returns TRUE if the url and domain matched.
 */



static BOOL domainMatch(UrlParts* urlParts, BYTE* domain, BYTE* dEnd,
                        BOOL reqDotRule, BOOL emptyRule, INT32 maxPreLabels,
                        INT32* preLabels)
{
   BOOL  emptyDomain;
   BOOL  dotDomain;
   BOOL  emptyMatch;
   BYTE* urlHost = urlParts->host;
   BYTE* hEnd = urlParts->hEnd;
   BYTE* s;
   BYTE* u;
   INT32 lCnt;
   BOOL  lLimit;
   BOOL  dot;

   if (preLabels) *preLabels = 0;

   emptyDomain = ((domain == dEnd) || !*domain);
   dotDomain = !emptyDomain && (*domain == '.');
   emptyMatch = emptyRule && emptyDomain;

   if (!dotDomain && !emptyMatch) {
      /* To match, host and domain string must match. */
      /* Either dEnd is known or domain is null terminated. */
      dot = 0;
      if (dEnd && ((hEnd - urlHost) != (dEnd - domain)))
         return FALSE;
      for (s = urlHost, u = domain; s != hEnd; s++, u++) {
         if (latin1_lc[*s] != latin1_lc[*u])    /* *t is never 0 */
            return FALSE;
         if (reqDotRule && !dot)
            dot = (*s =='.') && (s < hEnd-1);
      }
      if ((s != hEnd) || (!dEnd && *u))       /* no domain match */
         return FALSE;
      if (reqDotRule && !dot)                      /* top domain */
         return FALSE;

      return TRUE;
   }

   if (dotDomain)
   {
      if ((urlHost == hEnd) || (*urlHost == '.'))
         return FALSE;

      if (dEnd == NULL)
         dEnd = domain + B_STRINGLENGTH(domain);

      /* domain must not contain a single dot */
      if ((dEnd - domain) == 1) return FALSE;

      hEnd--;
      dEnd--;
      while ((hEnd != urlHost) && (dEnd != domain))
      {
         if (latin1_lc[*hEnd] != latin1_lc[*dEnd]) break; 
         hEnd--;
         dEnd--;
      }
      if ((dEnd != domain) || (*hEnd != '.')) /* domain is not suffix of urlHost */
         return FALSE;
   }

   if ((maxPreLabels > 0) || (preLabels != NULL) ||
       (emptyMatch && reqDotRule))
   {
      /* count labels in prefix part */
      lLimit = (maxPreLabels >= 0);
      if (hEnd != urlHost)
      {
         hEnd--;
         lCnt = 1;
      } else
         lCnt = 0;
      for (s = urlHost; s != hEnd; s++)
      {
         if (*s == '.')
         {
            lCnt++;
            if (lLimit && (lCnt > maxPreLabels))
               break;
         }
      }
      if (preLabels)
         *preLabels = lCnt;
      if (lLimit && (lCnt > maxPreLabels))
         return FALSE;
      if (emptyMatch && reqDotRule && (lCnt < 2))
         return FALSE;
   }

   return TRUE;
}


/* pathMatch
 *
 * Checks if an url matches a cookie having a given path component.
 *
 * A match as defined in rfc2109 and in WAP Provisioning User Agent Behaviour
 * (sect. 7) is:
 *  - the cookie path must be a prefix of the url path part,
 *    paths are case sensitive
 *
 * All but pLen are input parameters.
 * Input params may not be NULL.
 * The urlParts parameter contains the path from the url which is
 * compared to the parameter path.
 *
 * If the caller gives a non-NULL pLen parameter, the lenght of the matching
 * value of the path parameter is returned in *pLen. If the paths
 * does not match, *pLen is not valid.
 *
 * Note, that according to the rule that the cookie path must
 * be a prefix of the url path, the following url paths is valid
 * given a path parameter /a:
 * -  /a/b
 * -  /ab
 *
 * If the input parameter fullSegRule is true, full path
 * segments are required to get a match, i.e. in the example
 * above the url path /ab will not match, otherwise it will.
 *
 * Returns TRUE if the cookie having path, matched.
 */

static BOOL pathMatch(UrlParts* urlParts, BYTE* path, BOOL fullSegRule,
                      UINT32* pLen)
{
   BYTE* s;
   BYTE* t;
   BYTE* pEnd;

   /* if there is no path, there is a match */
   if (path && *path) {
      s = path;
      t = urlParts->path;
      pEnd = urlParts->pEnd;
      while (*s && (t != pEnd) && (*s == *t)) {
         s++;
         t++;
      }
      if (*s) return FALSE;                   /* no path match */
      if (pLen) *pLen = s - path;
      if (fullSegRule && (t != pEnd) && (s != path)) {
         /* Check that char after prefix is '/'. However, this */
         /* check is only valid if last char in prefix != '/'. */
         s--;
         if ((*s != '/') && (*t !=  '/'))
            return FALSE;
      }
   }
   else {                                           /* no path */
      if (pLen) *pLen = 0;
   }

   return TRUE;
}
         
/* dupSubStr
 *
 * Allocates and copies a sub-string of the string which starts 
 * at str and ends at the byte immediately before strEnd.
 * The caller is responsible of deleting the returned memory.
 *
 * Returns non-NULL if successfull.
 */
static BYTE* dupSubStr(const BYTE* str, const BYTE* strEnd)
{
   BYTE*  s;
   UINT32 len;

   if (!str || !strEnd) return NULL;

   len = strEnd - str + 1;
   s = (BYTE*) OSConnectorAlloc(len);
	#ifndef HAS_SETJMP
	if (s == NULL) {
		return NULL;
	}
	#endif
   B_COPYSTRINGN(s, str, len - 1);
   s[len - 1] = 0;
   return s;
}



