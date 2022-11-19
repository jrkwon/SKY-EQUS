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
#ifndef AAPISSEC_H
#define AAPISSEC_H


#include "tapicmmn.h"


INT16   SECa_clientHello(CHAR **string);

INT16   SECa_serverHello (const CHAR *string, INT16 length);

INT16   SECa_keyExchange (CHAR **string);

INT16   SECa_finished (const CHAR *string, INT16 length);

INT16   SECa_encrypt (const CHAR *string, INT16 length, CHAR **outString);

INT16   SECa_decrypt (const CHAR *string, INT16 length, CHAR **outString);


#endif
