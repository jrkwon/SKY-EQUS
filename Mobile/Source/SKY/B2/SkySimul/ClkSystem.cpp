#include "stdafx.h"
#include <time.h>
#include "SkySimul.h"
#include "SkySimulDlg.h"
#include "msm4win32.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  WORD year;            /* Year [1980..2100]                            */
  WORD month;           /* Month of year [1..12]                        */
  WORD day;             /* Day of month [1..31] or day of year [1..366] */
  WORD hour;            /* Hour of day [0..23]                          */
  WORD minute;          /* Minute of hour [0..59]                       */
  WORD second;          /* Second of minute [0..59]                     */
  WORD day_of_week;     /* Day of the week [0..6] Monday..Sunday        */
} clk_julian_type;

DWORD clk_read_secs(void) 
{
    
    CTime theTime;

    theTime = CTime::GetCurrentTime();

    return static_cast<unsigned long>(theTime.GetTime());
    
    return 1;
}

void clk_secs_to_julian
(
  DWORD secs,                   /* Number of seconds since base date    */
  clk_julian_type *julian_ptr   /* Pointer to Julian date record        */
)

{

    CTime theTime;

    theTime = secs;

  julian_ptr->year = theTime.GetYear();
  julian_ptr->month = theTime.GetMonth();
  julian_ptr->day = theTime.GetDay();
  julian_ptr->day_of_week = theTime.GetDayOfWeek() == 1 ? 6 : theTime.GetDayOfWeek()-2;
  julian_ptr->hour = theTime.GetHour();
  julian_ptr->minute = theTime.GetMinute();
  julian_ptr->second = theTime.GetSecond();


}

DWORD clk_julian_to_secs
(
  clk_julian_type *julian_ptr   /* Pointer to Julian date record        */
)
{

    CTime theTime(julian_ptr->year, julian_ptr->month, 
                  julian_ptr->day, julian_ptr->hour, julian_ptr->minute, julian_ptr->second);

    time_t secs = theTime.GetTime();

    return (DWORD)secs;

}

DWORD clk_uptime( void )
{
    CTime theTime;

    theTime = CTime::GetCurrentTime();

    return static_cast<unsigned long>(theTime.GetTime());
}


void clk_uptime_ms(
  qword uptime_ms   /* Returned uptime, in millisecond units. */
)
{
    DWORD index;

    index = GetTickCount();
    *((DWORD*)uptime_ms)   = GetTickCount();
    *((DWORD*)uptime_ms+1) = 0;

/*
    if ((0xFFFFFFFF - (*(DWORD *)uptime_ms)) < index)
     {
      (*((DWORD *)uptime_ms+1))++;
     }
    *(DWORD *)uptime_ms += index;
*/
}

void SKY_GetSystemTime(qword uptime_ms)
{
    *((DWORD*)uptime_ms)   = GetTickCount();
    *((DWORD*)uptime_ms+1) = 0;
}

#ifdef __cplusplus
}
#endif

