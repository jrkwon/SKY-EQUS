#ifndef WIPTIMER_H
#define WIPTIMER_H
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
#include "ml_typw.h"
#include "tapicmmn.h"


void xmk_TimerSet(long, unsigned int);
void xmk_TimerReset(unsigned int);
void xmk_InitTimer(void);
void xmk_ChkTimer(void);
void xmk_ResetAllTimer(xPID);

void deleteAllTimers(void);

void timerExpired(void);

void SetTimerWithParam(long, unsigned int, long);
void ResetTimerWithParam(unsigned int, long);


#endif
