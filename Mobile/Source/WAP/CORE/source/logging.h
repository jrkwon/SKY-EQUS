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
 * Logging.h
 *
 * Created by Anders Edenbrandt, Thu Oct 21 08:55:58 1999.
 *
 *
 * Revision history:
 *   981021, AED:  Created
 *   000119, AED:  New function, LogClose; new argument to LogInit.
 *
 */
#ifndef _Logging_h
#define _Logging_h

#ifdef LOG_WTLS
#include "wtlsdef.h"
#include "logcodes.h"

#define LOG_OUTBOUND   1
#define LOG_INCOMING   2

void
wtls_log_init (char *filename);

void
wtls_log_close (void);

void
wtls_log_msg (int indentationLevel, char *fmt, ...);

void
wtls_log_record (wtls_record_t *rec, BYTE direction);

void
wtls_log_record_brief (wtls_record_t *rec);

void
wtls_log_bytevector (BYTE *buf, UINT16 buflen);

#endif
#endif
