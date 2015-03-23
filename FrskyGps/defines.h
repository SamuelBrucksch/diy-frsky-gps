#include "Arduino.h"

int        heading = 0;
int        gps_alt = 0;
int        groundspeed = 0;
float      lat = 0;
float      lon = 0;
uint8_t    sats = 0;
int   fix = 0;

byte lat_dir;
byte lon_dir;
