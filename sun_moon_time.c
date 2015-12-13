#include "sun_moon_time.h"

double SolarAzimuth(time_t * point_in_time){
  time_t noon = solar_noon(point_in_time) % ONE_DAY;      // time of noon mod one day
  time_t tday = *point_in_time % ONE_DAY;                 // current time mod one day
  long r = tday - noon;                                   // offset from noon, -43200 to + 43200 seconds
  double HourAngle = r / 86400.0;                         // hour angle as percentage of full day
  HourAngle = HourAngle * 2.0 * M_PI;                     // hour angle in radians

  double Declination = solar_declination(point_in_time);  // declination in radians

  extern long  __latitude;
  double Latitude =  __latitude / ( ONE_DEGREE * RAD_TO_DEGREE);          // latitude in radians

  double Azimuth = atan ( sin(HourAngle) / (cos(HourAngle) * sin(Latitude) - tan(Declination) * cos(Latitude)));

  return Azimuth; // return azimuth in radians
};

void SolarPosition(time_t * point_in_time, double * Azimuth, double * Altitude){
  
  time_t noon = solar_noon(point_in_time) % ONE_DAY;      // time of noon mod one day
  time_t tday = *point_in_time % ONE_DAY;                 // current time mod one day
  long r = tday - noon;                                   // offset from noon, -43200 to + 43200 seconds
  double HourAngle = r / 86400.0;                         // hour angle as percentage of full day
  HourAngle = HourAngle * 2.0 * M_PI;                     // hour angle in radians

  double Declination = solar_declination(point_in_time);  // declination in radians

  extern long  __latitude;
  double Latitude =  __latitude / ( ONE_DEGREE * RAD_TO_DEGREE);          // latitude in radians

  double CosHourAngle = cos(HourAngle);
  double SinLatitude = sin(Latitude);
  double CosLatitude = cos(Latitude);

  *Azimuth = atan ( sin(HourAngle) / ( CosHourAngle * SinLatitude - tan(Declination ) * CosLatitude ) );
  *Altitude = asin( CosHourAngle * cos(Declination) * CosLatitude + sin(Declination) * SinLatitude );

}

