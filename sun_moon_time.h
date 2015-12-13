#ifndef __SUN_MOON_TIME_H_
#define __SUN_MOON_TIME_H_

#define RAD_TO_DEGREE 57.29577951308233

#include <math.h>
#include <time.h>

#ifdef __cplusplus
extern "C" 
{
#endif

  typedef enum AUS_LOCATIONS_KEY {
    CANBERRA_ACT,
    NEWCASTLE_NSW,
    PORT_MACQUARIE_NSW,
    SYDNEY_NSW,
    ALICE_SPRINGS_NT,
    CAIRNS_NT,
    DARWIN_NT,
    BRISBAVE_QLD,
    GOLD_CAOST_QLD,
    MACKAY_QLD,
    TOWNSVILLE_QLD,
    ADELAIDE_SA,
    GERALDTON_WA,
    KALGOORLIE_WA,
    PERTH_WA,
    PORT_HEDLAND_WA,
    HOBART_TAS,
    LAUNCESTON_TAS,
    MELBOURNE_VIC,
  } AUS_LOC_t; 

  void setLocation(AUS_LOC_t loc);
  
  double SolarAzimuth(time_t * point_in_time);
  
  void SolarPosition(time_t * point_in_time, double * Azimuth, double * Altitude);

#ifdef __cplusplus
}
#endif

static double latitude_by_aus_loc[] = {
  -35.2834625,
  -32.9271484,
  -31.4333333,
  -33.86785,
  -23.7,
  -16.9166667,
  -12.4611337,
  -27.4679357,
  -28,
  -21.15,
  -19.25,
  -34.9333333,
  -28.7666667,
  -30.75,
  -31.9333333,
  -20.3166667,
  -42.9166667,
  -41.45,
  -37.8139966,
};

static double Latitude;

#endif
