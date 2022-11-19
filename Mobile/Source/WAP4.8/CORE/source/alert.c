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
 * Alert.c
 *
 * Created by Anders Edenbrandt, Thu Jun 03 13:27:53 1999.
 *
 * Revision history:
 *
 */
#include "wtlsdef.h"

/*
 * Given a message of indicated length, compute and return
 * the alert checksum.
 */
UINT32
wtls_alert_compute_checksum (BYTE *msg, UINT16 msglen)
{
  INT16 j;
  BYTE  c0 = 0, c1 = 0, c2 = 0, c3 = 0;

  j = msglen;
  /* This merge of a switch and a while statement is known
   * as "Duff's device", and is used for efficiency reasons. */
  switch (msglen % 4) {
  case 0:
    while (j > 0) {
      c3 ^= msg[--j];
  case 3:
      c2 ^= msg[--j];
  case 2:
      c1 ^= msg[--j];
  case 1:
      c0 ^= msg[--j];
    }
  }

  return (((UINT32)c0) << 24) | (((UINT32)c1) << 16) |
    (((UINT32)c2) <<  8) | ((UINT32)c3);
}
