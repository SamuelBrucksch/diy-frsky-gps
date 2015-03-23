/* #### Protocol Type ####
 *
 * choose between FRSKY_X for X-Series receivers like X4R, X6R, X8R and FRSKY_D for D-series receivers like D4R, D8R
 *
 * If using X-series you need to download the Sport Library:
 * http://www.rcgroups.com/forums/showthread.php?t=2245978
 * Its attached to the first post.
 *
 */
#define FRSKY_X

/* #### GPS ####
 *
 * set baud rate of your gps
 *
 */
#define BAUD 38400


/* #### Telemetry pin ####
 *
 * chose a pin for telemetry output. You need to solder the cable to that pin and connect it to RX of the receiver
 *
 */
#ifdef FRSKY_D
  #define TELEMETRY_TX_PIN 9
#endif
#ifdef FRSKY_X
  #define TELEMETRY_TX_PIN FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_9
#endif
