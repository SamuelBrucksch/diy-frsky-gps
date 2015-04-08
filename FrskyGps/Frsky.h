#ifdef FRSKY_D

byte addPayload(byte DataID);
byte sendPayload(byte len);

unsigned char outBuff[48];     // Array for payloads
unsigned char outBuffFixed[48];     // Array for payloads

byte msCounter;
boolean packetOpen;
byte payloadLen;
#endif
