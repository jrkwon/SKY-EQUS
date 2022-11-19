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
 * wipdb.c
 *
 * Simple database system.
 *
 *
 * Created by Niclas Kellgren 000606.
 *
 * Revision history:
 *   000703, NKE: first version
 *   000728, NKE: data channel handling added
 *   000804, NKE: data channel handling updated 
 *   000807, NKE: new data channel functions added
 *   000809, NKE: bugfixes
 *   000817, NKE: the object template is changed into a general object
 *   000822, NKE: constants for default fields changed
 *   000906, NKE: pattern matching added for getDataChannel
 *   000914, NKE: data channel handling moved to dbFunct.c
 *   001013, NKE: added persistent memory handling
 *   001101, NKE: write through rewritten
 *   001114, NKE: bugfixes and minor updates
 *   001212, NKE: corrected saveWriteThroughElement
 */

#include "wipdb.h"


/*************
 * Constants *
 *************/

/* 16-bit flag field

	pos len use
	------------------
	0-2  3  type
	 3   1  key present
	 4   1  backup
	 5   1  write-through
	6-7  2  data use, compactation of 'type'
*/

enum DB_ElementType {
	Flag_rec = 0,
	Flag_set = 1, 
	Flag_int = 2,
	Flag_str = 3,
	Flag_mem = 4,
	Flag_ptr = 5,

	Flag_key = 8,

	Flag_backup       = 16,

	Flag_writeThrough = 32,

	Flag_dataChild    = 0,
	Flag_dataInternal = 64,
	Flag_dataExternal = 128
};

enum MaskType {
	Mask_type         = 0x07,
	Mask_key          = 0x08,
	Mask_backup       = 0x10,
	Mask_writeThrough = 0x20,
	Mask_persistent   = 0x30,
	Mask_dataUse      = 0xC0
};


enum {
	DB_max  = DB_root-1,                   /* Maximum number of created records.                */

	RefHashSize = 32,                      /* Must be 2**n, unsigned int n, n > 0.              */
	RefHashMask = 32 - 1,                  /* Must be RefHashSize - 1.                          */

	KeyLen = 6                             /* Element key length. Must be at least 6 long.      */
};


/*********
 * Types *
 *********/

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

typedef struct ElementBlockStruct* ElementBlockPtr;
typedef struct ElementBlockStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;
	UINT32     blockId;
	UINT32     blockNext;

} ElementBlock;



typedef struct FieldStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;
	BYTE       key[KeyLen];

} Field, *FieldPtr;

typedef struct FieldBlockStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;
	BYTE       key[KeyLen];
	UINT32     blockId;
	UINT32     blockNext;

} FieldBlock, *FieldBlockPtr;



typedef struct RecordStruct* RecordPtr;
typedef struct RecordStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;
	BYTE       key[KeyLen];
	UINT16     ref;
	RecordPtr  nextRec; 

} Record;

typedef struct RecordBlockStruct* RecordBlockPtr;
typedef struct RecordBlockStruct {
	ElementPtr next;
	DataUnion  data;
	UINT16     flags;
	BYTE       key[KeyLen];
	UINT16     ref;
	RecordPtr  nextRec; 
	UINT32     blockId;
	UINT32     blockNext;

} RecordBlock;


/*************
 * Variables *
 *************/

static RecordPtr* refHashTable;            /* Hash table of liked list. RecordPtr.nextRec.     */
static DB_ref     recordNbr = 0;           /* Record counter. Null reference is number 0.      */

static RecordPtr  root;                    /* Pointer to database root (dummy) element.        */

static RecordPtr  currentRecord = NULL;    /* Used by findField/deleteElement.                 */
static DB_ref     currentRef    = DB_null; /* Matches currentRecord. Used for caching.         */

static ElementPtr currentElement;          /* Carries information between several functions\   */
static ElementPtr prevElement;             /* findField, findRecord, deleteRecord...           */

static StorageObject store;                /* Main object used by storage.c                    */

static BOOL       serviceMode;             /* TRUE at startup or shutdown. Protects\           */
                                           /* presistent memory during rebuild/close.          */ 

/*************
 * Functions *
 *************/

UINT32 getBlockId(ElementPtr element);
void setBlockNext(ElementPtr element, UINT32 nextId);
void deleteWriteThroughElement(RecordPtr rec, ElementPtr currentElem, ElementPtr prevElem);
BOOL saveWriteThroughElement(RecordPtr parent, ElementPtr elem);


/* ======== help functions ======== */


/*
 * Search hash structure for record. Sets currentRecord and 
 * currentRef. Must be called prior to findField.
 */
RecordPtr findRecord(DB_ref record)
{
	RecordPtr rec;
	
	if (record == currentRef) /* Speed up the most common case by caching */
		return currentRecord;
	rec = refHashTable[record & RefHashMask];
	while (TRUE) {
		if (rec == NULL) {
			currentRef = DB_null;
			break;
		} else if (rec->ref == record) {
			currentRef = record;
			break;
		} else
			rec = rec->nextRec;
	}
	currentRecord = rec;
	return currentRecord;
}



/*
 * Find field named 'key' if present, else return NULL. Sets 
 * currentElement and prevElement. currentRecord defines the record
 * to search and findRecord must be called first to set it.
 *
 * Can only be used on records, NOT on sets.
 */
FieldPtr findField(const BYTE* key) 
{
	const BYTE* t;
	BYTE*       s;

	if (key == NULL)
		return NULL;
	if ((currentRecord->flags & Mask_type) != Flag_rec)
		return NULL;
	prevElement = NULL;
	currentElement = currentRecord->data.e;
	while (currentElement != NULL) {
		for (s=((FieldPtr)currentElement)->key, t=key; *s == *t; s++, t++)
			if (*s == '\0')
				return (FieldPtr) currentElement;
		prevElement = currentElement;
		currentElement = currentElement->next;
	}
	return NULL;
}



/*
 * Recursively delete entire subtree, excluding 'rec' root
 * element. Does not update rec->data.e. Preserves ref hash
 * structure.
 */
void deleteRecord(RecordPtr rec)
{
	ElementPtr current;
	ElementPtr prev;
	RecordPtr* ptr;
	UINT32     id;

	current = rec->data.e;
	while (current != NULL) {
		switch (current->flags & Mask_dataUse) {
		case Flag_dataChild:
			ptr = refHashTable + (((RecordPtr)current)->ref & RefHashMask);
			while ((*ptr)->ref != ((RecordPtr)current)->ref)
				ptr = &(**ptr).nextRec;
			*ptr = (*ptr)->nextRec;
			
			if (current->data.e != NULL)
				deleteRecord((RecordPtr) current);
			break;

		case Flag_dataExternal:
			OSConnectorFree(current->data.s);
		}

		if (current->flags & Mask_writeThrough && ! serviceMode) {
			if ((current->flags & Mask_dataUse) == Flag_dataChild)
				id = ((RecordBlockPtr) current)->blockId;
			else
				if (current->flags & Mask_key)
					id = ((FieldBlockPtr) current)->blockId;
				else
					id = ((ElementBlockPtr) current)->blockId;

			Storage_DeleteBlock(&store, id); 
		}
		prev = current;
		current = current->next;
		OSConnectorFree(prev);
	}
		
}



/*
 * Delete currentElement. If it is a record or set, use 
 * deleteRecord to remove the subtree. Preserves ref hash 
 * structure.
 *
 * Depends on currentElement, currentRecord, prevElement.
 */
void deleteElement(void)
{
	RecordPtr *ptr;

	if (currentElement->flags & Mask_writeThrough)
		deleteWriteThroughElement(currentRecord, currentElement, prevElement);
	if (prevElement == NULL)
		currentRecord->data.e = currentElement->next;
	else
		prevElement->next = currentElement->next;

	switch (currentElement->flags & Mask_dataUse) {
	case Flag_dataChild:
		ptr = refHashTable + (((RecordPtr)currentElement)->ref & RefHashMask);
		while ((*ptr)->ref != ((RecordPtr)currentElement)->ref)
			ptr = &(**ptr).nextRec; 
		*ptr = (*ptr)->nextRec;
		
		if (currentElement->data.e != NULL)
			deleteRecord((RecordPtr) currentElement);
		break;

	case Flag_dataExternal:
		OSConnectorFree(currentElement->data.s);
	}
	OSConnectorFree(currentElement);
}


/* ======== get ======== */


/*
 * Return integer from element.
 */
INT32 getInt(DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode)
{
	RecordPtr rec;
	FieldPtr field;
	register INT32 temp;
	
	rec = findRecord(record);
	if (rec == NULL) {
		*errorCode = DB_err_record;
		return 0;
	}
	field = findField(key);
	if (field == NULL) {
		*errorCode = DB_err_field;
		return 0;
	}
	if ((field->flags & Mask_type) != Flag_int) {
		*errorCode = DB_err_wrongType;
		return 0;
	} else {
		*errorCode = DB_err_success;
		temp = field->data.i;
		if (del)
			deleteElement();
		return temp;
	}
}



/*
 * Return copy of string from element.
 */
BYTE* getStr(DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode)
{
	RecordPtr rec;
	FieldPtr  field;
	BYTE*     temp;
	BYTE*     s;
	BYTE*     t;
	UINT32    len;

	rec = findRecord(record);
	if (rec == NULL) {
		*errorCode = DB_err_record;
		return NULL;
	}
	field = findField(key);
	if (field == NULL) {
		*errorCode = DB_err_field;
		return NULL;
	}
	if ((field->flags & Mask_type) != Flag_str) {
		*errorCode = DB_err_wrongType;
		return NULL;
	} else {
		*errorCode = DB_err_success;
		if (del) {
			temp = field->data.s;
			deleteElement();
		} else {
			t = field->data.s;
			for (len=1, s=t; *s; s++)
				len++;
			temp = (BYTE*) OSConnectorAlloc(len); 
			#ifndef HAS_SETJMP
			if (temp == NULL) {
				*errorCode = DB_err_outOfMemory;
				return NULL;
			}
			#endif
			for (s=temp; *s = *t; s++, t++)
				;
		}
		return temp;
	}
}



/*
 * Return copy of memory block from element.
 */
BYTE* getMem(DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode)
{
	RecordPtr rec;
	FieldPtr  field;
	BYTE*     temp;

	rec = findRecord(record);
	if (rec == NULL) {
		*errorCode = DB_err_record;
		return NULL;
	}
	field = findField(key);
	if (field == NULL) {
		*errorCode = DB_err_field;
		return NULL;
	}
	if ((field->flags & Mask_type) != Flag_mem) {
		*errorCode = DB_err_wrongType;
		return NULL;
	} else {
		*errorCode = DB_err_success;
		if (del) {
			temp = field->data.s;
			deleteElement();
		} else {
			temp = (BYTE*) OSConnectorAlloc(*(UINT16*) field->data.s); 
			#ifndef HAS_SETJMP
			if (temp	== NULL) {
				*errorCode = DB_err_outOfMemory;
				return NULL;
			}
			#endif
			memcpy(temp, field->data.s, *(UINT16*) field->data.s);
		}
		return temp;
	}
}



/*
 * Return pointer from element. It is legal to getPtr on a 
 * string or memory block. If 'del' is true, the element is
 * deleted, but not its data even if it is a string or memory
 * block.
 */
void* getPtr(DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode)
{
	RecordPtr rec;
	FieldPtr field;
	register void* temp;

	rec = findRecord(record);
	if (rec == NULL) {
		*errorCode = DB_err_record;
		return NULL;
	}
	field = findField(key);
	if (field == NULL) {
		*errorCode = DB_err_field;
		return NULL;
	}
	if ((field->flags & Mask_type) != Flag_ptr &&
		 (field->flags & Mask_type) != Flag_str &&
		 (field->flags & Mask_type) != Flag_mem) { 
		*errorCode = DB_err_wrongType;
		return NULL;
	} else {
		*errorCode = DB_err_success;
		temp = field->data.p;
		if (del)
			deleteElement();
		return temp;
	}
}



/*
 * Return reference to record element.
 */
DB_ref getRef(DB_ref record, const BYTE* key, UINT8* errorCode)
{
	RecordPtr rec;
	FieldPtr field;

	rec = findRecord(record);
	if (rec == NULL) {
		*errorCode = DB_err_record;
		return DB_null;
	}
	field = findField(key);
	if (field == NULL) {
		*errorCode = DB_err_field;
		return DB_null;
	}
	if ((field->flags & Mask_dataUse) != Flag_dataChild) {
		*errorCode = DB_err_wrongType;
		return DB_null;
	} else {
		*errorCode = DB_err_success;
		currentRecord = (RecordPtr)field;      /* Guess the next search uses the\*/
		currentRef = ((RecordPtr)field)->ref;  /* returned ref. Cache result. */
		return currentRef;
	}
}



/*
 * Return type of element.
 */
UINT8 getType(DB_ref record, const BYTE* key, UINT8* errorCode)
{
	RecordPtr rec;
	FieldPtr field;

	rec = findRecord(record);
	if (rec == NULL) {
		*errorCode = DB_err_record;
		return 0;
	}
	field = findField(key);
	if (field == NULL) {
		*errorCode = DB_err_field;
		return 0;
	}
	*errorCode = DB_err_success;
	return field->flags & Mask_type;
}





/* ======== set ======== */

/*
 * Create and insert an empty Element or Field.
 * If an element named 'key' already exists, 'flags' must
 * include the type of the element. Old strings or memory
 * blocks are freed.
 *
 * If NULL is returned, an error has occurred and its number
 * is returned in 'errorCode'.
 */
ElementPtr setItem(DB_ref record, const BYTE* key, UINT16 flags, DataUnion data,
                   UINT8 *errorCode)
{
	ElementPtr element;
	RecordPtr  rec;
	BYTE*      s;
	int        i;

	*errorCode = DB_err_success;

	rec = findRecord(record);
	if (rec == NULL) { 
		*errorCode = DB_err_record;
		return NULL;
	}
	
	if (record != DB_root)
		flags = (flags & ~ Mask_writeThrough) | (rec->flags & Mask_writeThrough);
	if (flags & Mask_writeThrough && (flags & Mask_type) == Flag_ptr) {
			*errorCode = DB_err_persistentTypeError;
			return NULL;
	}
	
	if ((rec->flags & Mask_type) == Flag_set) { 
		/* Flag_set */
		if (flags & Mask_writeThrough)
			element = (ElementPtr) OSConnectorAlloc(sizeof(ElementBlock));
		else
			element = (ElementPtr) OSConnectorAlloc(sizeof(Element));
		#ifndef HAS_SETJMP
		if (element == NULL) {
			*errorCode = DB_err_outOfMemory;
			return NULL;
		}
		#endif
		element->flags = ! Flag_key;
	} else {  
		/* Flag_rec */
		if (key == NULL) {
			*errorCode = DB_err_nullValue;
			return NULL;
		}
		element = (ElementPtr) findField(key);
		if (element != NULL) { /* update element */
			if ((element->flags & Mask_type) != (flags & Mask_type)) {
				*errorCode = DB_err_wrongType;
				return NULL;
			} else {
				if ((flags & Mask_dataUse) == Flag_dataExternal)
					OSConnectorFree(element->data.s);
				element->data = data;
				if (flags & Mask_writeThrough)
					if (! saveWriteThroughElement(rec, element))
						/* No persistent save, set warning. Return value still valid. */
						*errorCode = DB_err_noPersistentSave;
				return element;
			}
		} else { /* New element */
			if (flags & Mask_writeThrough)
				element = (ElementPtr) OSConnectorAlloc(sizeof(FieldBlock));
			else
				element = (ElementPtr) OSConnectorAlloc(sizeof(Field));
			#ifndef HAS_SETJMP
			if (element == NULL) {
				*errorCode = DB_err_outOfMemory;
				return NULL;
			}
			#endif
			for (s=((FieldPtr)element)->key, i=1; *s = *key; s++, key++, i++)
				if (i == KeyLen) { /* In case of too long input keys. */
					OSConnectorFree(element);
					*errorCode = DB_err_tooLongKey;
					return NULL;
				}
			element->flags = Flag_key;
		}
	}
	element->flags |= flags;  
	element->data   = data;
	element->next   = rec->data.e;
	rec->data.e     = element;

	if (flags & Mask_writeThrough && ! serviceMode) {
		if (element->flags & Mask_key)
			((FieldBlockPtr) element)->blockId = 0;
		else
			((ElementBlockPtr) element)->blockId = 0;
		if (! saveWriteThroughElement(rec, element))
			/* No persistent save, set warning. Return value still valid. */
			*errorCode = DB_err_noPersistentSave;
	}

	return element;
}



/*
 * Update or insert an int field.
 */
UINT8 setInt(DB_ref record, const BYTE* key, INT32 value)
{
	DataUnion  data;
	UINT8      error;

	data.i = value;
	setItem(record, key, Flag_int | Flag_dataInternal, data, &error);
	return error;
}



/*
 * Update or insert a string field.
 */
UINT8 setStr(DB_ref record, const BYTE* key, BYTE* value)
{
	DataUnion  data;
	UINT8      error;

	if (value == NULL)
		return DB_err_nullValue;
	data.s = value;
	setItem(record, key, Flag_str | Flag_dataExternal, data, &error);
	return error;
}



/*
 * Update or insert a memory block field.
 */
UINT8 setMem(DB_ref record, const BYTE* key, BYTE* value)
{
	DataUnion  data;
	UINT8      error;

	if (value == NULL)
		return DB_err_nullValue;
	data.s = value;
	setItem(record, key, Flag_mem | Flag_dataExternal, data, &error);
	return error;
}



/*
 * Update or insert a pointer field.
 */
UINT8 setPtr(DB_ref record, const BYTE* key, void* value)
{
	DataUnion  data;
	UINT8      error;

	data.p = value;
	setItem(record, key, Flag_ptr | Flag_dataInternal, data, &error);
	return error;
}



/* ======== structural operations ======== */


/*
 * Create a new record. If a field with the same name exists, an
 * error will be generated, except when the parent record is a
 * set. If 'flags' bit 0 is set, it signals this is a set, not a
 * record. This can be done by using paramater DB_set instead of
 * DB_rec for record.
 *
 * 'errorCode' returns the status of the function. DB_err_success
 * when the operation i successfully completed.
 *
 * If 'key' is NULL, the key of the new record will be an empty
 * string.
 */
DB_ref createRecord(DB_ref record, const BYTE* key, UINT16 flags, UINT8 *errorCode)
{
	RecordPtr newRec;
	RecordPtr rec;
	BYTE*     s;
	int       len;

	/* When updating createRecord, update copyRecord as well */

	if (recordNbr == DB_max){ 
		*errorCode = DB_err_outOfRefs;
		return DB_null;
	}

	rec = findRecord(record);
	if (rec == NULL) { 
		*errorCode = DB_err_record;
		return DB_null;
	} 
	if ((rec->flags & Mask_type) == Flag_rec && findField(key)) {
		*errorCode = DB_err_exists;
		return DB_null;
	}

	if (record != DB_root)
		flags = (flags & ~ Mask_writeThrough) | (rec->flags & Mask_writeThrough);

	if (flags & Mask_writeThrough) {
		newRec = (RecordPtr) OSConnectorAlloc(sizeof(RecordBlock)); 
		newRec->flags = Flag_writeThrough;
	} else {
		newRec = (RecordPtr) OSConnectorAlloc(sizeof(Record)); 
		newRec->flags =  ! Flag_writeThrough;
	}
	#ifndef HAS_SETJMP
	if (newRec == NULL) {
		*errorCode = DB_err_outOfMemory;
		return DB_null;
	}
	#endif
	
	if (key == NULL || (rec->flags & Mask_type) == Flag_set)
		*newRec->key = '\0';
	else {
		for (s=newRec->key, len=1; *s = *key; s++, key++, len++) 
			if (len == KeyLen) {
				OSConnectorFree(newRec);
				*errorCode = DB_err_tooLongKey;
				return DB_null;
			}
	}
	newRec->data.e = NULL; 	
	if (flags & Mask_type)  /* Add more flags when introduced */
		newRec->flags |= Flag_set | Flag_key | Flag_dataChild;
	else
		newRec->flags |= Flag_rec | Flag_key | Flag_dataChild;
	newRec->ref     = ++recordNbr;
	newRec->nextRec = refHashTable[recordNbr & RefHashMask];
	refHashTable[recordNbr & RefHashMask] = newRec;
	newRec->next    = rec->data.e;
	rec->data.r     = newRec;

	currentRecord = newRec; /* Assume this record will be used in \*/
	currentRef = recordNbr; /* the next operation. Cache record. */

	*errorCode = DB_err_success;

	if (flags & Mask_writeThrough && ! serviceMode) {
		((RecordBlockPtr) newRec)->blockId = 0;
		if (! saveWriteThroughElement(rec, (ElementPtr) newRec))
			/* No persistent save, set warning. Return value still valid. */
			*errorCode = DB_err_noPersistentSave;			
	}

	return recordNbr;
}



/*
 * Recusive help function. Used by copyRecord. 
 * 
 * Make an copy of a record structure. The element order is
 * preserved. Fields of type int, str and mem are copied. For
 * pointer fields, only the pointer is copied, not the content
 * pointed out. Even if the original is write through, the will
 * not be.
 *
 * If an error occures, the clone operation aborts, but none of
 * the fields copied to the 'to' record are removed. A correct 
 * structure is always preserved.
 *
 * 'formElement' is a pointer to the first element in a record.
 * 'flags' signals what kind of element the parent record is, e.g.
 * if it is a set och rec.
 */
UINT8 clone(ElementPtr fromElement, DB_ref to, UINT16 flags)
{
	DataUnion  data;
	UINT32     len;
	BYTE*      s;
	BYTE*      t;
	UINT8      error;

	if (fromElement->next != NULL) {
		error = clone(fromElement->next, to, flags);
		if (error)
			return error;
	}
	if ((fromElement->flags & Mask_dataUse) == Flag_dataChild) {
		to = createRecord(to, ((RecordPtr) fromElement)->key,
			                    (UINT16) (fromElement->flags & ~Mask_writeThrough), &error);
		if (error)
			return error;
		if (fromElement->data.e != NULL) {
			error = clone(fromElement->data.e, to, (UINT16) (fromElement->flags & Mask_type));
			if (error)
				return error;
		}
	} else {
		switch (fromElement->flags & Mask_type) {
		case Flag_int:
			data.i = fromElement->data.i;
			break;
		case Flag_ptr:
			data.p = fromElement->data.p;
			break;
		case Flag_str:
			t = fromElement->data.s;
			for (s=t, len=1; *s; s++)
				len++;
			data.s = (BYTE*) OSConnectorAlloc(len); 
			#ifndef HAS_SETJMP
			if (data.s == NULL)
				return DB_err_outOfMemory;
			#endif
			for (s=data.s; *s = *t; s++, t++)
				;
			break;
		case Flag_mem:
			t = fromElement->data.s;
			len = *((UINT16*) t);
			data.s = (BYTE*) OSConnectorAlloc(len); 
			#ifndef HAS_SETJMP
			if (data.s == NULL)
				return DB_err_outOfMemory;
			#endif
			for (s=data.s; len--; s++, t++)
				*s = *t;
			break;
		}
		setItem(to, flags & Mask_type ? NULL : ((RecordPtr) fromElement)->key,
								(UINT16) (fromElement->flags & ~Mask_writeThrough), data, &error);
		if (error) { /* Can at present only happen when out of memory. */
			if ((fromElement->flags & Mask_dataUse) == Flag_dataExternal)
				OSConnectorFree(data.p);
			return error;
		}
	}
	return DB_err_success;
}



/*
 * Clone the 'from' record and place the copy it as a new field
 * in the 'to' record. It is legal to insert the copy into the 
 * source tree.
 *
 * 'key' is the name of the copy. If 'key' is NULL, the original
 * name will be used. If an error occurs, all work is undone and
 * a correct structrure is maintained. A copy of a write through
 * structure do not become write through.
 *	
 * A reference to the new record is returned.
 */
DB_ref copyRecord(DB_ref from, DB_ref to, BYTE* key, UINT8 *errorCode)
{
	RecordPtr fromRec;
	RecordPtr newRec;
	RecordPtr toRec;
	UINT8     error;
	BYTE*     s;
	int       len;

	/* When updating copyRecord, update createRecord as well */

	fromRec = findRecord(from);
	if (fromRec == NULL) {
		*errorCode = DB_err_record;
		return DB_null;
	}

	if (key == NULL) 
		key = fromRec->key;
	
	/* Create a new record (code almost identical to createRecord)
		and add the structure to copy by invoking clone() before
		inserting the new record into the database. If using
		createRecord instead, there is a risk of an infinite 
		recursive call loop. */

	if (recordNbr == DB_max){ 
		*errorCode = DB_err_outOfRefs;
		return DB_null;
	}

	toRec = findRecord(to);
	if (toRec == NULL) { 
		*errorCode = DB_err_record;
		return DB_null;
	} 
	if ((toRec->flags & Mask_type) == Flag_rec && findField(key)) {
		*errorCode = DB_err_exists;
		return DB_null;
	}
	
	newRec = (RecordPtr) OSConnectorAlloc(sizeof(Record));
	#ifndef HAS_SETJMP
	if (newRec == NULL) {
		*errorCode = DB_err_outOfMemory;
		return DB_null;
	}
	#endif
	for (s=newRec->key, len=1; *s = *key; s++, key++, len++)
		if (len == KeyLen) {
			OSConnectorFree(newRec);
			*errorCode = DB_err_tooLongKey;
			return DB_null;
		}
	newRec->data.e = NULL;
	if (fromRec->flags & Mask_type) /* Add more flags when introduced */
		newRec->flags = Flag_set | Flag_key | Flag_dataChild;
	else
		newRec->flags = Flag_rec | Flag_key | Flag_dataChild;
	newRec->ref      = ++recordNbr;
	newRec->nextRec  = refHashTable[recordNbr & RefHashMask];
	refHashTable[recordNbr & RefHashMask] = newRec;

	if (fromRec->data.e != NULL) {
		error = clone(fromRec->data.e, newRec->ref,
						  (UINT16) (fromRec->flags & Mask_type));
		if (error) {
			currentElement = (ElementPtr) newRec;
			prevElement    = (ElementPtr) newRec;
			deleteElement();
			*errorCode = error;
			return DB_null;
		}
	};

	newRec->next  = toRec->data.e;
	toRec->data.r = newRec;

	*errorCode = DB_err_success;
	return newRec->ref;
}



/*
 * Remove the specified item. If it is a string or memory block,
 * associated data is freed. For records and sets, the entire
 * subtree is deleted.
 */
UINT8 deleteItem(DB_ref record, const BYTE* key)
{
	if (! findRecord(record)) 
		return DB_err_record;

	if (! findField(key)) 
		return DB_err_field;

	deleteElement();
	return DB_err_success;
}



/*
 * Same functionality as deleteItem. 'elem' is a pointer to the
 * element to be deleted and 'rec' its parent.
 */
UINT8 deleteItem_ptr(RecordPtr rec, ElementPtr elem)
{
	if (rec == NULL || elem == NULL) 
		return DB_err_nullValue;

	prevElement = NULL;
	currentElement = rec->data.e;
	while (currentElement != NULL) {
		if (currentElement == elem) {
			currentRecord = rec;
			currentRef = DB_null;
			deleteElement();
			return DB_err_success;
		}
		prevElement = currentElement;
		currentElement = currentElement->next;
	}
	return DB_err_field;
}



/*
 * Remove all fields connected to 'record'. The entire subtree
 * is deleted. Works on both records and sets.
 */
UINT8 clearRecord(DB_ref record)
{
	UINT32 id;

	if (! findRecord(record)) 
		return DB_err_record;

	if (currentRecord->flags & Mask_writeThrough) {
		id = 0;
		Storage_Put(&store, ((RecordBlockPtr) currentRecord)->blockId, 4, 4, &id);
	}
	deleteRecord(currentRecord);
	currentRecord->data.e = NULL;
	return DB_err_success;
}



/*
 * Move 'elem' to the beginning of the list it is in.
 *
 * If the system crashes during this function, the
 * 'elem' item may be lost, even when using persitent
 * memory. 
 */
void moveToFront(RecordPtr parent, ElementPtr element)
{
	ElementPtr currentElem;
	ElementPtr prevElem;
	UINT32 prevId;

	/* Find previous element in list */
	currentElem = parent->data.e;
	prevElem = NULL;
	while (currentElem != element) {
		if (currentElem == NULL)
			return;
		prevElem = currentElem;
		currentElem = currentElem->next;
	}
	if (prevElem == NULL)
		return;
	
	/* Move element to the beginning of the list */
	if (element->flags & Mask_writeThrough) {
		prevId = getBlockId(prevElem);
		if (element->next == NULL)
			Storage_Put(&store, getBlockId((ElementPtr) parent), 4, 4, &prevId);
		else
			setBlockNext(element->next, prevId);
		setBlockNext(element, 0);
		setBlockNext(parent->data.e, getBlockId(element));
	} 
	prevElem->next = element->next;
	element->next = parent->data.e;
	parent->data.e = element;
}


/* ======== persistent memory handling ======== */


/* 
   Structure of write-through blocks at presistet memory:

   type   structure[size bytes]
   ---------------------------------
	rec 
	set    nextBlockId[4] childBlockId[4] flags[2] key[KeyLen] 

	int    nextBlockId[4] data[4]         flags[2] key[KeyLen] 

	str
	mem    nextBlockId[4] dataLength[4]   flags[2] key[KeyLen] data[dataLength]


  If the Flag_key is not set, the key field is missing.

  The processor endian will affect the byte order within each
  field, but not the functionality.

  There is no type corresponding to pointer fields! The pointers
  point to structures outside of the database and the system has
  been restarted. They make no sense and are unsafe. Convert the
  pointer and save as an integer if using write-through elements. 

  Elements are save in reversed order. The first element in each
  list is placed last when saved. This is faster and makes it
  easier to reconstruct the structure when rebuilding.
*/

extern char* mem;

BYTE* buf;
UINT32* idArr;
enum {
	BufLen = 64                   /* must fit everything but the data and be multiple of 8 */
};



/*
 *	Search through idArr and sets the MSB of the element equal
 * to 'id'.
 */
void markId(UINT32 id)
{
	UINT32* p;

	for (p=idArr; *p != id; p++)
		;
	*p |= 0x80000000;
}



/*
 *	Return blockId field of 'element'.
 */
UINT32 getBlockId(ElementPtr element)
{
	if ((element->flags & Mask_dataUse) == Flag_dataChild)
		return ((RecordBlockPtr) element)->blockId;
	else
		if (element->flags & Mask_key)
			return ((FieldBlockPtr) element)->blockId;
		else
			return ((ElementBlockPtr) element)->blockId;
}



/*
 *	Return blockNext field of 'element'.
 */
UINT32 getBlockNext(ElementPtr element)
{
	if ((element->flags & Mask_dataUse) == Flag_dataChild)
		return ((RecordBlockPtr) element)->blockNext;
	else
		if (element->flags & Mask_key)
			return ((FieldBlockPtr) element)->blockNext;
		else
			return ((ElementBlockPtr) element)->blockNext;
}



/*
 * Set blockNext field of 'element' to 'nextId' and updata block
 * at persistent memory.
 */
void setBlockNext(ElementPtr element, UINT32 nextId)
{
	if ((element->flags & Mask_dataUse) == Flag_dataChild)
		((RecordBlockPtr) element)->blockNext = nextId;
	else
		if (element->flags & Mask_key)
			((FieldBlockPtr) element)->blockNext = nextId;
		else
			((ElementBlockPtr) element)->blockNext = nextId;

	Storage_Put(&store, getBlockId(element), 0, 4, &nextId);
}



/*
 *	Rebuilds database by reading write-through data. Recursive,
 * adds fields to an already existing record.
 *
 * 'parent' is a reference to the record corresponding to the
 * parent of 'id'.
 *
 * If the function returns FALSE, the reconstruction has failed
 * and the database has only been partially reconstructed.
 */
BOOL rebuildDB_wt(UINT32 id, DB_ref parent)
{
	DataUnion  data;
	ElementPtr element;
	DB_ref     newRef;
	BYTE*      key;
	UINT32     blockLen;
	UINT32     offset;
	UINT32     readLen;
	UINT32     nextId;
	UINT16     flags;
	UINT8      error;

	while (id != 0) {
		blockLen = Storage_GetBlockSize(&store, id); /* blockLen rounded up */
		readLen = MIN(BufLen, blockLen);

		Storage_Get(&store, id, 0, readLen, buf);

		flags  = *(UINT16*) (buf + 8);
		nextId = *(UINT32*) (buf + 0);
		
		if (flags & Mask_key) {
			key = buf + 10;
			offset = 10 + KeyLen;
		} else
			offset = 10;

		switch (flags & Mask_dataUse) {
		case Flag_dataChild:
			newRef = createRecord(parent, key, flags, &error);
			((RecordBlockPtr) currentRecord)->blockId    = id;
			((RecordBlockPtr) currentRecord)->blockNext  = nextId;
			 if ( ! rebuildDB_wt(*(UINT32*)(buf + 4), newRef))
				 return FALSE;
			goto label_rebuildDB_wt;
		case Flag_dataExternal:
			blockLen = *(UINT32*) (buf + 4); /* Exact blockLen */
			readLen = MIN(BufLen, blockLen);
			data.s = (BYTE*) OSConnectorAlloc(blockLen - offset);	
			#ifndef HAS_SETJMP
			if (data.s == NULL)
				return FALSE;
			#endif
			memcpy(data.s, buf + offset, (size_t)(readLen - offset));
			if (readLen < blockLen)
				Storage_Get(&store, id, readLen, blockLen - readLen, data.s + readLen - offset);
			element = setItem(parent, key, flags, data, &error);
			if (error) {
				OSConnectorFree(data.s);
				return FALSE;
			}
			break;
		case Flag_dataInternal:
			data.i = *(UINT32*) (buf + 4);	
			element = setItem(parent, key, flags, data, &error);
			break;
		}
		if (flags & Mask_key) {
			((FieldBlockPtr) element)->blockId   = id;
			((FieldBlockPtr) element)->blockNext = nextId;
		} else {
			((ElementBlockPtr) element)->blockId   = id;
			((ElementBlockPtr) element)->blockNext = nextId;
		}
label_rebuildDB_wt:
		markId(id);
		id = nextId;
	}
	return TRUE;
}



/*
 *
 * Return FALSE if there is nothing to load.
 */
BOOL loadDB(void)
{
	BOOL   clearAll = FALSE;
	UINT32 id;
	UINT16 len;
	int    i;

	if (!Storage_GetAllBlockIds(&store, &idArr, &len) || len == 0)
		return FALSE;
	
	buf = (BYTE*) OSConnectorAlloc(BufLen); 
	#ifndef HAS_SETJMP
	if (buf == NULL)
		return FALSE;
	#endif

	serviceMode = TRUE;
	Storage_Get(&store, 1024, 4, 4, &id);
	markId(1024);

	if ( ! rebuildDB_wt(id, DB_root)) {
		/* Rebuild has failed. Delete everything. */
		clearRecord(root->ref);
		clearAll = TRUE;
	}
	
	serviceMode = FALSE;
	OSConnectorFree(buf);

	for (i=0; i < len; i++) {
		if ((idArr[i] <= 0x80000000 || clearAll) && idArr[i] > 1024) {
			/* Found a loose block */
			Storage_DeleteBlock(&store, idArr[i] & 0x7fffffff);
		}
	}

	OSConnectorFree(idArr);
	return TRUE;
}



/*
 *	Delete a write-through block. If 'prevElem' is NULL, 'currentElem'
 * is first in list and the record block conncted to 'rec' is
 * updated instead.
 */
void deleteWriteThroughElement(RecordPtr parent, ElementPtr currentElem, ElementPtr prevElem)
{
	UINT32 nextId;

	if (prevElem == NULL)
		nextId = 0;
	else
		nextId = getBlockId(prevElem);

	if (currentElem->next == NULL) /* End element is head in the reversed block list */
		Storage_Put(&store, ((RecordBlockPtr) parent)->blockId, 4, 4, &nextId);
	else
		setBlockNext(currentElement->next, nextId);

	Storage_DeleteBlock(&store, getBlockId(currentElement));
}			 



/*
 *
 * If element->blockId == 0, then this is the first time this 
 * element is saved.
 *
 * Return TRUE if success.
 */
BOOL saveWriteThroughElement(RecordPtr parent, ElementPtr element)
{
	char buffer[10+KeyLen];

	ElementPtr currentElem;
	ElementPtr prevElem;

	UINT32     id;
	UINT32     nextId;
	UINT32     newId;
	UINT32     childId;
	UINT32     len;
	UINT32     dataLen;
	UINT32     blockLen;

	UINT16     type    = element->flags & Mask_type;
	UINT16     dataUse = element->flags & Mask_dataUse;
	UINT16     keyLen  = (element->flags & Mask_key) ? KeyLen : 0;

	/* block len */
	len = 10 + keyLen;
	if (type == Flag_str) {
 		dataLen = strlen((char*) element->data.s) + 1;
		len += dataLen;
	} else if (type == Flag_mem) {
		dataLen = *(UINT16*) element->data.p;
		len += dataLen;
	}
	

	/* Allocate block */
	id = getBlockId(element);

	if (id == 0) { /* First time */
		newId = 0;
		if (! Storage_AllocateBlock(&store, len, &newId))
			/* Out of persistent memory */
			return FALSE;
		nextId = 0;
	} else { 
		blockLen = Storage_GetBlockSize(&store, id);
		if (len <= blockLen && len > blockLen - 8) {	/* Rounded up, 8 byte alignment */
			/* Reuse block */
			newId = id;
		} else {
			/* New block */
			newId = 0;
			if (! Storage_AllocateBlock(&store, len, &newId))
				/* Out of persistent memory */
				return FALSE;
		}
		nextId = getBlockNext(element);
	}


	*(UINT32*) (buffer+0) = nextId;
	*(UINT16*) (buffer+8) = element->flags;

	
	/* Set data (buffer+4) */
	if (dataUse == Flag_dataChild) { /* rec, set */
		if (((RecordPtr) element)->data.e == NULL)
			childId = 0;
		else {
			for (currentElem = ((RecordPtr) element)->data.e; currentElem != NULL;
			                                                        currentElem = currentElem->next)
				prevElem = currentElem;
			childId = getBlockId(prevElem);
		}
		*(UINT32*) (buffer+4) = childId;
	} else if (dataUse == Flag_dataInternal) /* int */
		*(UINT32*) (buffer+4) = element->data.i;
	else /* str, mem */
		*(UINT32*) (buffer+4) = len;
	
	if (keyLen != 0)
		memcpy(buffer+10, ((FieldPtr) element)->key, KeyLen);
	Storage_Put(&store, newId, 0, 10 + keyLen, buffer);
	
	if (dataUse == Flag_dataExternal)
		Storage_Put(&store, newId, 10 + keyLen, dataLen, element->data.p);


	/* Insert into list */
	if (newId != id) {
		if (parent == root) {
			for (prevElement = element->next; prevElement != NULL; 
														  prevElement = prevElement->next)
				if (prevElement->flags & Mask_writeThrough)
					break;
		} else
			prevElement = element->next;

		if (prevElement == NULL) /* End element is head in the reversed block list */
			Storage_Put(&store, ((RecordBlockPtr) parent)->blockId, 4, 4, &newId);
		else
			setBlockNext(prevElement, newId);

		if (id != 0)
			Storage_DeleteBlock(&store, id);
	}


	/* Update blockId and blockNext */
	if (dataUse == Flag_dataChild) {
		((RecordBlockPtr) element)->blockId = newId;
		((RecordBlockPtr) element)->blockNext = nextId;
	} else
		if (keyLen != 0) {
			((FieldBlockPtr) element)->blockId = newId;
			((FieldBlockPtr) element)->blockNext = nextId;
		} else {
			((ElementBlockPtr) element)->blockId = newId;
			((ElementBlockPtr) element)->blockNext = nextId;
		}
	return TRUE;
}



/*
 *
 */
void saveDB(void)
{
}



/* ======== admin ======== */

/*
 * Initiate the database and its structures.
 *
 * On some systems (Ericsson), when out of memory, the system
 * does not restart until the control has been returned to the
 * event loop. In this case, it is not legal to call any database
 * functions between dbInit and returning to the event loop.
 */
void dbInit(void)
{
	char   buffer[10+KeyLen];
	UINT32 id;
	int    i;

	serviceMode = FALSE;

	refHashTable = (RecordPtr*) OSConnectorAlloc(sizeof(RecordPtr) * RefHashSize); 
	root = (RecordPtr) OSConnectorAlloc(sizeof(RecordBlock)); 
	#ifndef HAS_SETJMP
	if (!refHashTable || !root)
		return; /* A new event loop iteraration will cause a restart. */
	#endif
	
	root->next        = NULL;
	root->data.e      = NULL;
	root->flags       = Flag_rec | Flag_key | Flag_writeThrough | Flag_dataChild;
	root->ref         = DB_root;
	root->nextRec     = NULL;

	*(root->key + 0 ) = 'r';
	*(root->key + 1 ) = 'o';
	*(root->key + 2 ) = 'o';
	*(root->key + 3 ) = 't';
	*(root->key + 4 ) = '\0';

	((RecordBlockPtr)root)->blockId    = 1024;
	((RecordBlockPtr)root)->blockNext  = 0;

	for (i=0; i<RefHashSize; i++) 
		refHashTable[i] = NULL;
	refHashTable[DB_root & RefHashMask] = root;

	currentRef = DB_null;

	if (!Storage_Init(&store, DATABASE_STORAGESIZE, (ReadFunction*)  MEMa_readDatabase,
		               (WriteFunction*) MEMa_writeDatabase))
		return;

	if (! loadDB()) {
		/* First time */

		*(UINT16*) (buffer + 8) = root->flags;
		*(UINT32*) (buffer + 0) = 0; /* Next id */
		*(UINT32*) (buffer + 4) = 0; /* Child id */

		*(BYTE*) (buffer + 10) = 'r';
		*(BYTE*) (buffer + 11) = 'o';
		*(BYTE*) (buffer + 12) = 'o';
		*(BYTE*) (buffer + 13) = 't';
		*(BYTE*) (buffer + 14) = '\0';

		id = 1024;
		Storage_AllocateBlock(&store, 10+KeyLen, &id);
		Storage_Put(&store, id, 0, 10+KeyLen, buffer);
	}
}




/*
 * Deletes the database content and frees all static structures.
 *
 * Remove of all non-database structures connected to the
 * database first. 
 */
void dbTerminate(void)
{
	serviceMode = TRUE;
	saveDB();

	deleteRecord(root);
	OSConnectorFree(root);

	OSConnectorFree(refHashTable);

	Storage_Finalize(&store);
}


