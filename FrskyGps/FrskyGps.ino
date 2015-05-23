/**
 * created by Samuel Brucksch
 *
 * parts of this code are not done by me
 **/

#include "config.h"
#include "inttypes.h"
#include "SoftwareSerial.h"
#include "defines.h";
#include "GpsAdapter.h"

#if defined(FRSKY_D)
#include "Frsky.h"
SoftwareSerial frsky(3, TELEMETRY_TX_PIN, true);
#elif defined(FRSKY_X)
#include "FrSkySportSensor.h"
#include "FrSkySportSensorGps.h"
#include "FrSkySportSensorRpm.h"
#include "FrSkySportSingleWireSerial.h"
#include "FrSkySportTelemetry.h"
FrSkySportSensorGps gpsSensor;
FrSkySportSensorRpm rpmSensor;
FrSkySportTelemetry frsky;
#endif

long timer;
long timer2;

void setup() {
  initializeGps();
#if defined(FRSKY_D)
  frsky.begin(9600);
#elif defined(FRSKY_X)
  frsky.begin(TELEMETRY_TX_PIN, &gpsSensor, &rpmSensor);
#endif
  pinMode(13, OUTPUT);
  timer = millis();
  timer2 = timer;
}

void loop() {
  if (millis() - timer > 10) {
    updateGps();
    timer = millis();
  }

  fix = getState();
  switch (fix) {
    case GPS_DETECTING:
      //blink with 4hz
      if (millis() % 500 < 250) {
        digitalWrite(13, HIGH);
      }
      else {
        digitalWrite(13, LOW);
      }
      break;
    case GPS_NOFIX:
      //no blinking
      digitalWrite(13, LOW);
      break;
    case GPS_FIX2D:
    case GPS_FIX3D:
    case GPS_FIX3DD:
      //solid
      digitalWrite(13, HIGH);
      break;
  }

#if defined(FRSKY_D)
  //frsky_d needs to manually check for frequency
  if (millis() - timer2 > REFRESH_RATE_MILLIS) {
    timer2 = millis();
#endif

    if (fix > GPS_NOFIX) {
#if defined(FRSKY_D)
      float deg = abs(getLat() / 10000000.0f);
      lat = ((int)deg * 100) +  (deg - (int)deg) * 60.0f;
      lat_dir = getLat() > 0 ? 'N' : 'S';

      deg = abs(getLon() / 10000000.0f);
      lon = ((int)deg * 100) +  (deg - (int)deg) * 60.0f;
      lon_dir = getLon() > 0 ? 'E' : 'W';
#elif defined(FRSKY_X)
      lat = getLat() / 10000000.0f;
      lon = getLon() / 10000000.0f;
#endif
      gps_alt = getAlt() / 1000.0f;

#if defined(FRSKY_D)
      groundspeed = getSpeed() * 0.01944f;
#elif defined(FRSKY_X)
      groundspeed = getSpeed() / 100.0f;
#endif
      heading = getCourse() / 10.0f;
    }
    sats = (getSats() * 10) + fix;

#if defined(FRSKY_D)
    update_frsky();
  }
#elif defined(FRSKY_X)
    //ignore time
    rpmSensor.setData(0, 0, sats);
    gpsSensor.setData(lat, lon, gps_alt, groundspeed, heading, 0, 0, 0, 0, 0, 0);
    frsky.send();
#endif
}
