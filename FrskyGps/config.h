/* #### Protocol Type ####
 *
 * choose between FRSKY_X for X-Series receivers like X4R, X6R, X8R and FRSKY_D for D-series receivers like D4R, D8R
 *
 * If using X-series you need to download the Sport Library:
 * http://www.rcgroups.com/forums/showthread.php?t=2245978
 * It is attached to the first post.
 *
 */
#define FRSKY_D

/* #### Telemetry send rate for d-series ####
*
* recommended 3-5hz - 5hz may be too much, to be tested
* however more than 5hz doesnt really make much sense and i dont know if the taranis can handle it
*
*/
#define TELEMETRY_HZ 3

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

#define REFRESH_RATE_MILLIS 1000/TELEMETRY_HZ
