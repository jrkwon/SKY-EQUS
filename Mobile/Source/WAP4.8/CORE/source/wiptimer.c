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

#include "wiptrgt.h"
#include "wiptimer.h"
#include "cmmnrsrc.h"
#include "aapiclnt.h"


typedef struct timerType {
  unsigned int Signal;
  xPID OwnerProcessPID;
  long timerId;
  unsigned long expire_s; /* the part of the expire time that is in seconds */
  unsigned long expire_10;/* the part of the expire time that is in 10th of seconds */
  struct timerType *next;
} timerType;

static timerType *timerList;

void
xmk_InitTimer (void)
{
  timerList = NULL;
}

int
timerQueue_empty (void)
{
  return timerList == 0;
}

static void
checkTimerList (int do_reset)
{
  timerType     *t = timerList;
  unsigned long now = CLNTa_currentTime ();

  if (!t)
    return;

  /* Send signals for all timers with the same expiration time.
   * The condition we use here for considering a timer to have expired
   * leaves room for an error of +-0.9 seconds. However, in the absence
   * of a clock with higher resolution, we cannot do much better.
   * The present method assures that CLNTa_setTimer is always called
   * with a non-zero argument from this routine.
   */
  while (t && ((t->expire_s < now) ||
               (t->expire_s == now) && (t->expire_10 == 0))) {
    if (t->timerId == -1) {
      XMK_SF (t->Signal, XMK_TIMERPRIO, 0, NULL, t->OwnerProcessPID);
    }
    else {
      long var = t->timerId;
      XMK_SF (t->Signal, XMK_TIMERPRIO,
              sizeof (long), &var, t->OwnerProcessPID);
    }
    /* Remove the timer from the front of the list. */
    timerList = t->next;
    OSConnectorFree (t);
    t = timerList;
  }

  if (timerList) {
    if (do_reset)
      CLNTa_resetTimer ();
    CLNTa_setTimer ((timerList->expire_s - now) * 10 + timerList->expire_10);
  }
}

static void
wipSetTimer (long time, unsigned int sid, long timerId)
{
  timerType *prev = NULL;
  timerType *t = timerList;
  timerType *timer = (timerType *)OSConnectorAlloc (sizeof(timerType));

	timer->Signal = sid;
	timer->OwnerProcessPID = xRunPID;
	timer->timerId = timerId;
	timer->expire_s = CLNTa_currentTime () + (time / 10);
	timer->expire_10 = time - (time/10*10);

	/* Find the timer that has an expiration time that is
	   later than the expiration time of the new timer. */
	while (t)	{
		if (timer->expire_s < t->expire_s)
			break;
		else if ((timer->expire_s == t->expire_s)
             && (timer->expire_10 < t->expire_10))
			break;
		prev = t;
		t = t->next;
	}

	/* insert the timers with later expire time after the new timer */
	timer->next = t;

	if (prev) { /* put the timer in the middle or at the end of the list */
		prev->next = timer;
	}
	else if (t) { /* The new timer shall preceed the first timer in the list */
		CLNTa_resetTimer ();
		timerList = timer;
		CLNTa_setTimer (time);
	}
	else { /* The new timer is the only one in the list */
		timerList = timer;
		CLNTa_setTimer (time);
	}
}


static void
wipResetTimer (unsigned int sid, long timerId)
{
	timerType *prev = NULL;
	timerType *t = timerList;

	/* Find the timer in matter. */
	while (t && (t->Signal != sid || t->timerId != timerId
               || t->OwnerProcessPID != xRunPID))	{
		prev = t;
		t = t->next;
	}
  if (!t)
    return;

	if (prev) { /* remove a timer in the middle or at the end of the list */
		prev->next = t->next;
	}
	else { /* Remove the first timer in the list */
    timerList = t->next;
    CLNTa_resetTimer ();
    checkTimerList (0);
	}
  OSConnectorFree (t);
}


void
xmk_TimerSet (long time, unsigned int sid)
{
	wipResetTimer (sid, -1);
	wipSetTimer (time, sid, -1);
}


void
xmk_TimerReset (unsigned int sid)
{
	wipResetTimer (sid, -1);
}

void
xmk_ResetAllTimer (xPID pid)
{
	timerType *prev = NULL;
	timerType *t = timerList;
	BOOL firstTimerIsRemoved = FALSE;;

	/* Remove the timers in matter. */
	while (t)	{
		if (t->OwnerProcessPID == pid) {
			if (prev) { /* remove a timer in the middle or at the end of the list */
				prev->next = t->next;
				OSConnectorFree (t);
				t = prev->next;
			}
			else { /* Remove the first timer in the list */
				firstTimerIsRemoved = TRUE;
				timerList = t->next;
				OSConnectorFree (t);
				t = timerList;
			}
		}
		else {
			prev = t;
			t = t->next;
		}
	}

	if (firstTimerIsRemoved) {
    CLNTa_resetTimer ();
    checkTimerList (0);
  }
}


void
timerExpired (void)
{
  checkTimerList (1);
}


void
xmk_ChkTimer(void)
{
}


void
deleteAllTimers (void)
{
  timerType *t = timerList;

	CLNTa_resetTimer ();

  while (t != NULL) {
    timerType *timer = t;
    t = t->next;
    OSConnectorFree (timer);
  }

  timerList = NULL;
}


void
SetTimerWithParam (long Time, unsigned int sid , long timerId)
{
	wipResetTimer (sid, timerId);
	wipSetTimer (Time, sid, timerId);
}


void
ResetTimerWithParam (unsigned int sid, long timerId)
{
	wipResetTimer (sid, timerId);
}
