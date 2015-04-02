#include "Arduino.h"

float      heading = 0;
float      gps_alt = 0;
float      groundspeed = 0.0f;
float      lat = 0;
float      lon = 0;
uint8_t    sats = 0;
uint16_t   fix = 0;

byte lat_dir;
byte lon_dir;

