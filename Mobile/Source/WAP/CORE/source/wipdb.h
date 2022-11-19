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
 * wipdb.h
 *
 * Simple database system.
 *
 *
 * Created by Niclas Kellgren 000606.
 *
 * Revision history:
 *   000703, NKE: first version
 *   000728, NKE: data channel handling added
 *   000804, NKE: data channel api updated 
 *   000809, NKE: minor changes to data channel api
 *   000906, NKE: addHost, deletHost interfaces changed
 *   000914, NKE: data channel handling moved to dbFunct.h
 *   001018, NKE: added persistent memory handling
 */

#ifndef _WIPDB_H
#define _WIPDB_H

#include "cmmnrsrc.h"
#include "confvars.h"
#include "aapimem.h"
#include "storage.h"

 	 
/*************
 * Constants *
 *************/

enum ErrorCodes {
	DB_err_success             = 0,  /* Successful operation.                                   */
	DB_err_nullValue           = 1,  /* Null pointer as parameter.                              */
	DB_err_record              = 2,  /* Couldn't find record.                                   */
	DB_err_field               = 3,  /* Couldn't find field.                                    */
	DB_err_outOfRefs           = 4,  /* Out of unique references.                               */
	DB_err_wrongType           = 5,  /* Tried to do get/set on an element of a different type.  */
	DB_err_setError            = 6,  /* Unable to set data in element.                          */
	DB_err_exists              = 7,  /* Element with given key does already exist.              */
	DB_err_tooLongKey          = 8,  /* The key is too long.                                    */
	DB_err_objectNotFound      = 9,  /* Object not found.                                       */
	DB_err_noDefaultChannel    = 10, /* No channel set as default.                              */
	DB_err_persistentTypeError = 11, /* Pointer elements cannot be save on persistent memory.   */
	DB_err_outOfMemory         = 12, /* Out of memory. Can normaly not occur.                   */
	DB_err_noPersistentSave    = 13  /* Warning: created element, failed to save it persitently.*/
};

enum {
	DB_null = 0,                     /* Not an element.                                         */
	DB_root = 65535u-31,             /* Reference to root element. Root hash to zero.           */

	DB_rec           = 0x00,			/* Normal record.                                          */
	DB_set           = 0x01,         /* Elements in record are nameless.                        */
	DB_backup        = 0x10,         /* Backup element and its data/sub elements at closedown.  */
	DB_writeThrough  = 0x20          /* Backup element as soon it is changed.                   */
};


/*********
 * Types *
 *********/

typedef UINT16 DB_ref;


/*************
 * Functions *
 *************/

/*
    ----------------
    Element handling
    ----------------

 * Four types of data is handled: 32 bits unsigned integers,
 * strings, memory blocks and pointers. A memory block is a block
 * where its first 16 bits are used for telling the size of the
 * block in octets, the first two inclusive. To avoid endian 
 * problems, access the size by *(UINT16 *) p, where p is a
 * pointer to the block.
 *
 * Pointers are used to point out a structure outside the data-
 * base. Data belonging to the other three types are saved inside
 * the database, are owned by the data base and deallocated when
 * needed.
 *
 * All strings and memory blocks must be allocated by WIPAlloc()
 * and so are copies recieved from the database.
 *
 * All records have a reference of the DB_ref type. Null references
 * are DB_null and the root element has the reference DB_root.
 * 
 * A record can be of two types: record or set. A set is a record
 * with two restriction:
 * 1. The elements in a set do not have a key and can therefore
 *    not be fetched with any of the get operations.
 * 2. A record or set cannot be a field in a set.
 *
 * Error codes are specified by enum ErrorCodes.
 */


/*
 * GET
 * 
 * Return requested data. Str and mem functions return a copy
 * interna data. Can only be done on a field of the same type
 * as requested. And exeption is getPtr, which can be applied
 * on a string, memory block och pointer element. Returns 
 * pointer to internal or external object.
 *
 * If 'del' is true, the element is deleted together with its
 * data. Deleting the element when calling getPtr only removes
 * the field, not its data even if it is a string or memory 
 * block element.
*/
INT32  getInt (DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode);
BYTE*  getStr (DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode);
BYTE*  getMem (DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode);
void*  getPtr (DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode);

DB_ref getRef (DB_ref record, const BYTE* key, UINT8* errorCode);
UINT8  getType(DB_ref record, const BYTE* key, UINT8* errorCode);


/*
 * SET
 *
 * Set field to specified value. If the field does not exist,
 * it is created. If it does exist, it has be of the same
 * type as the the operation. Its data is updated and in the
 * str and mem case, its old data is freed.
 *
 * An error code is returned.
 */
UINT8 setInt(DB_ref record, const BYTE* key, INT32 value);
UINT8 setStr(DB_ref record, const BYTE* key, BYTE* value);
UINT8 setMem(DB_ref record, const BYTE* key, BYTE* value);
UINT8 setPtr(DB_ref record, const BYTE* key, void* value);


/*
 * CREATE
 *
 * Create a new empty record. 'flags' can be of two types, DB_rec
 * means record and DB_set creates a set.
 * 
 * If a field with the same name exists, an error will be generated,
 * except when the parent record is a set.
 *
 * 'errorCode' returns the status of the function. DB_err_success
 * when the operation i successfully completed.
 *
 * If 'key' is NULL, the key of the new record will be an empty
 * string.
 */
DB_ref createRecord(DB_ref record, const BYTE* key, UINT16 flags, UINT8* errorCode);


/*
 * COPY
 *
 * Duplicates a record tree 'from' and inserts it at the record
 * 'to' as a field. It is legal to insert the copy into source
 * tree.
 *
 * An error code is returned.
 *
 * 'key' is the name of the copy. If 'key' is NULL, the original
 * name will be used.
 *
 * If an error occurs, all work is undone and a correct
 * structrure is maintained.
 *
 * A reference to the new record is returned.
 */
DB_ref copyRecord(DB_ref from, DB_ref to, BYTE* key, UINT8 *errorCode);


/*
 * DELETE
 *
 * deleteItem removes the specified item. If it is a string or 
 * memory block, associated data is freed. For records and sets,
 * the entire subtree is deleted.
 *
 * clearRecord removes all fields connected to 'record'. The 
 * entire subtree is deleted. Works on both records and sets.
 *
 * An error code is returned.
 */
UINT8 deleteItem(DB_ref record, const BYTE* key);
UINT8 clearRecord(DB_ref record);



/*
    --------------
    Administration
    --------------
*/

/*
 * INIT/TERMINATE
 *
 * To be called at start-up or close-down.
 *
 * Remove of all non-database structures connected to the
 * database before calling dbTerminate. 
 *
 * On some systems (Ericsson), when out of memory, the system
 * does not restart until the control has been returned to the
 * event loop. In this case, it is not legal to call any database
 * functions between dbInit and returning to the event loop.
 */
void dbInit(void);
void dbTerminate(void);


#endif 
