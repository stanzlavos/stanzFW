#include "utils_wifi.h"
#include "utils_ntp.h"

#ifdef NTP_IN_USE

#include <NTPtimeESP.h>

NTPtime NTPch("pool.ntp.org");
strDateTime dateTime;
uint8_t ntp_hours   = 0;
uint8_t ntp_minutes = 0;

bool get_ntp_time(uint8_t *hours, uint8_t *minutes)
{
  bool ret = false;

  if (is_wifi_connected())
  {
    dateTime = NTPch.getNTPtime(5.5, 0);

    if (dateTime.valid)
    {
      *hours    = dateTime.hour;
      *minutes  = dateTime.minute;

      ret = true;
    }
  }
  return ret;
}

void ntp_time_to_str(char *str, uint8_t hours, uint8_t minutes)
{
  if ((hours > 12) || (hours == 0))
  {
    hours %= 12;
  }

  sprintf(str, "%02d:%02d", hours, minutes);
}

bool get_ntp_time_str_if_mod(char *str)
{
  uint8_t hours = 0, minutes = 0;
  if (get_ntp_time(&hours, &minutes))
  {
    if ((hours != ntp_hours) || (minutes != ntp_minutes))
    {
      ntp_hours   = hours;
      ntp_minutes = minutes;
      ntp_time_to_str(str, hours, minutes);

      return true;
    }
  }

  return false;
}

void get_curr_ntp_time(uint8_t *hours, uint8_t *minutes)
{
  *hours    = ntp_hours;
  *minutes  = ntp_minutes;
}

/*
  uint8_t actualHour      = dateTime.hour;
  uint8_t actualMinute    = dateTime.minute;
  uint8_t actualsecond    = dateTime.second;
  int     actualyear      = dateTime.year;
  uint8_t actualMonth     = dateTime.month;
  uint8_t actualday       = dateTime.day;
  uint8_t actualdayofWeek = dateTime.dayofWeek;

  //NTPch.printDateTime(dateTime);
*/

#endif // NTP_IN_USE
