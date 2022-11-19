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
 * MgtEnt.c
 *
 * Created by Anders Edenbrandt, Tue Jun 01 10:14:49 1999.
 *
 * Revision history:
 *
 */
#include "wtlsdef.h"
#ifdef LOG_EXTERNAL
#include "aapiclnt.h"
#include "logcodes.h"
#endif
#ifdef LOG_WTLS
#include "logging.h"
#endif

/*
 * Initialize the Management Entity process.
 */
void
wtls_me_init (void)
{
#ifdef LOG_WTLS
  wtls_log_init (NULL);
#endif

  /* Init crypto library has been moved to CLNTc_start (). */

  /* Initialization of the session cache is done from within
     the crypto library. */
}

/*
 * Terminate the Management Entity process.
 */
void
wtls_me_terminate (void)
{
  /* Termination of the  crypto library has been
     moved to TerminationDone (). */

  /* Termination of the session cache is now done from within
     the crypto library. */

#ifdef LOG_WTLS
  wtls_log_close ();
#endif
}
