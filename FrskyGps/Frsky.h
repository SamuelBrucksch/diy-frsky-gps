#ifdef FRSKY_D

#include "inttypes.h"
#include "Arduino.h"

byte addPayload(byte DataID);
byte sendPayload(byte len);
void updateTime();
long FixInt(long val, byte mp);
void initFrskySerial();

static long f_curMillis;
static long f_preMillis;
static int f_delMillis = 200;

unsigned char outBuff[48];     // Array for payloads
unsigned char outBuffFixed[48];     // Array for payloads

byte msCounter;
boolean packetOpen;
byte payloadLen;

static int        heading = 0;                // ground course heading from GPS
static int        gps_alt = 0;                // altitude
static int        groundspeed = 0;            // ground speed

static float      lat = 0;                    // latitude
static float      lon = 0;                    // longitude
static uint8_t    sats = 0;                   // number of satelites
static uint16_t   fix = 0;                    // fix type 

byte lat_dir;
byte lon_dir;

#endif
