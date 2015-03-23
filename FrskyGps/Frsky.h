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
#endif
