/**
 * created by Samuel Brucksch
 *
 * parts of this code are not done by me
 **/

#include "config.h"
#include "inttypes.h"
#include <SoftwareSerial.h>
#include "defines.h";
#ifdef FRSKY_D
  #include "Frsky.h"
  SoftwareSerial frsky(3, TELEMETRY_TX_PIN, true);
#endif
#ifdef FRSKY_X
  #include "FrSkySportSensor.h"
  #include "FrSkySportSensorGps.h"
  #include "FrSkySportSensorRpm.h"
  #include "FrSkySportTelemetry.h"
  FrSkySportSensorGps gpsSensor;
  FrSkySportSensorRpm rpmSensor;
  FrSkySportTelemetry telemetry;
#endif

#include <TinyGPS++.h>
TinyGPSPlus gps;
TinyGPSCustom fixType(gps, "GPGGA", 6);

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Define BAUD (included by config.h) and F_CPU before including setbaud.h.
// This will force automatic generation of correct baud rate values
#include <util/setbaud.h>

ISR(USART_RX_vect)
{
  uint8_t c = UDR0;
  gps.encode(c);
}

void initUart() {
  UCSR0B |= (1 << RXEN0)  | (1 << TXEN0) | (1 << RXCIE0); // UART RX, TX und RX Interrupt einschalten
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);                // Asynchron 8N1

  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif
}

//maybe needs to be adapted for other CPUs
void uart_putc(unsigned char c) {
  while (!(UCSR0A & (1 << UDRE0))){
  }
  UDR0 = c;
}

void uart_puts (char *s){
  while (*s) {
    uart_putc(*s);
    s++;
  }
}

void setup() {
  initUart();
#ifdef FRSKY_D
  frsky.begin(9600);
#endif
#ifdef FRSKY_X
  telemetry.begin(TELEMETRY_TX_PIN, &gpsSensor, &rpmSensor);
#endif
  pinMode(13, OUTPUT);
}

void loop() {
  //TODO if fixtype == 0 -> do not send data
  if (fixType.isValid()){
    fix = (uint16_t)fixType.value();
  }

  switch(fix){
  case 0:
    digitalWrite(13, LOW);
    break;
  case 1:
    //TODO blinking
    if (millis() % 1000 < 500){
      digitalWrite(13, HIGH);
    }
    else{
      digitalWrite(13, LOW);
    }
    break;
  case 2:
    digitalWrite(13, HIGH);
    break;
  default:
    digitalWrite(13, LOW);
    break;
  }

  if (gps.location.isValid()) {
#ifdef FRSKY_D
    int deg = gps.location.lat();
    lat = ((deg * 100) +  ((gps.location.lat() - deg) * 60.0));
    lat_dir = gps.location.lat() > 0 ? 'N' : 'S';

    deg = gps.location.lng();
    lon = ((deg * 100) +  ((gps.location.lng() - deg) * 60.0));
    lon_dir = gps.location.lng() > 0 ? 'E' : 'W';
#endif
#ifdef FRSKY_X
    lat = gps.location.lat();
    lon = gps.location.lng();
#endif
  }

  if (gps.altitude.isValid()) {
    gps_alt = (int)gps.altitude.meters();
  }

  if (gps.speed.isValid()) {
#ifdef FRSKY_D
    groundspeed = gps.speed.knots();
#endif
#ifdef FRSKY_X
    groundspeed = gps.speed.kmph();
#endif
  }

  if (gps.course.isValid()) {
    heading = (int)gps.course.deg();
  }

  if (gps.satellites.isValid()) {
    sats = gps.satellites.value();
  }

#ifdef FRSKY_D
  update_frsky();
#endif
#ifdef FRSKY_X
  //ignore time
  gpsSensor.setData(lat, lon, gps_alt, groundspeed, heading, 0, 0, 0, 0, 0, 0);
  rpmSensor.setData(0,0,(sats*10)+fix);
  telemetry.send();
#endif
}
