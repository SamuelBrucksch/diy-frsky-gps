#ifdef FRSKY_D
void update_frsky() {
  packetOpen = true;

    payloadLen += addPayload(0x13);   // Longitude dddmmm
    payloadLen += addPayload(0x1b);   // Longitude .mmmm (after ".")
    payloadLen += addPayload(0x23);   // E/W

    payloadLen += addPayload(0x12);   // Latitude dddmmm
    payloadLen += addPayload(0x1a);   // Latitude .mmmm (after ".")
    payloadLen += addPayload(0x22);   // N/S

    payloadLen += addPayload(0x01);   // GPS Altitude
    payloadLen += addPayload(0x09);   // GPS Altitude (after ".")

  // 1000ms (1s) payload, contruct Frame every second
  if(msCounter % TELEMETRY_HZ == 0) {
    payloadLen += addPayload(0x05);   // Temperature 2 -> sats    
    payloadLen += addPayload(0x14);   // Course, degree
    payloadLen += addPayload(0x1c);   // Course, after "."
    payloadLen += addPayload(0x11);   // GPS Speed Knots
    payloadLen += addPayload(0x19);   // GPS Speed after "."
    msCounter = 0;
  }

  packetOpen = false;
  payloadLen = sendPayload(payloadLen);

  // Update loop counter
  msCounter ++;
}

byte addPayload(byte DataID) {
  byte addedLen;
  int tmp;
  switch (DataID) {
  case 0x01:  // GPS Altitude
    tmp = int(gps_alt);
    outBuff[payloadLen + 0] = 0x01;
    outBuff[payloadLen + 1] = lowByte(tmp);
    outBuff[payloadLen + 2] = highByte(tmp);
    addedLen = 3;
    break;
  case 0x01+8:  // GPS Altitude after "."
    {
      int tmp = int(gps_alt*100.0f) % 100;
      outBuff[payloadLen + 0] = 0x01 + 8;
      outBuff[payloadLen + 1] = lowByte(tmp);
      outBuff[payloadLen + 2] = highByte(tmp);
      addedLen = 3;
    }
    break;
    // Temperature 2
    // We are using Temperature 2 to show Visible GPS satellites and also FIX type
    // Visible satellites is multiplied with 10 and fix type is added on final number
    // For example if we have 7 satellites and we have solid 3D fix outcome will be
    // (7 * 10) + 3 = 73   (7 satelliteds, 3 = 3D Fix)
  case 0x05:
    outBuff[payloadLen + 0] = 0x05;
    outBuff[payloadLen + 1] = lowByte(sats);
    outBuff[payloadLen + 2] = highByte(sats);
    addedLen = 3;
    break;
  case 0x11:  // GPS Speed, before "."
    tmp = int(groundspeed);
    outBuff[payloadLen + 0] = 0x11;
    outBuff[payloadLen + 1] = lowByte(tmp);
    outBuff[payloadLen + 2] = highByte(tmp);
    addedLen = 3;
    break;
  case 0x11+8:  // GPS Speed, after "."
    tmp = int(groundspeed*100.0f) % 100;
    outBuff[payloadLen + 0] = 0x11 + 8;
    outBuff[payloadLen + 1] = lowByte(tmp);
    outBuff[payloadLen + 2] = highByte(tmp);
    addedLen = 3;
    break;
    //Little Endian exception
  case 0x12:  // Longitude, before "."
    outBuff[payloadLen + 0] = 0x12;
    outBuff[payloadLen + 1] = lowByte(long(lon));
    outBuff[payloadLen + 2] = highByte(long(lon));
    addedLen = 3;
    break;
  case 0x12+8:  // Longitude, after "."
    outBuff[payloadLen + 0] = 0x12 + 8;
    outBuff[payloadLen + 1] = lowByte(long((lon - long(lon)) * 10000.0));
    outBuff[payloadLen + 2] = highByte(long((lon - long(lon)) * 10000.0));
    addedLen = 3;
    break;
  case 0x1A+8:  // E/W
    outBuff[payloadLen + 0] = 0x1A + 8;
    outBuff[payloadLen + 1] = lon_dir;
    outBuff[payloadLen + 2] = 0;
    addedLen = 3;
    break;
    //Little Endian exception
  case 0x13:  // Latitude, before "."
    outBuff[payloadLen + 0] = 0x13;
    outBuff[payloadLen + 1] = lowByte(long(lat));
    outBuff[payloadLen + 2] = highByte(long(lat));
    addedLen = 3;
    break;
  case 0x13+8:  // Latitude, after "."
    outBuff[payloadLen + 0] = 0x13 + 8;
    outBuff[payloadLen + 1] = lowByte(long((lat - long(lat)) * 10000.0));
    outBuff[payloadLen + 2] = highByte(long((lat - long(lat)) * 10000.0));
    addedLen = 3;
    break;
  case 0x1B+8:  // N/S
    outBuff[payloadLen + 0] = 0x1B + 8;
    outBuff[payloadLen + 1] = lat_dir;
    outBuff[payloadLen + 2] = 0;
    addedLen = 3;
    break;
  case 0x14:  // course, before ".". OK
    tmp = int(heading);
    outBuff[payloadLen + 0] = 0x14;
    outBuff[payloadLen + 1] = lowByte(tmp);
    outBuff[payloadLen + 2] = highByte(tmp);
    addedLen = 3;
    break;
  case 0x14+8:  // course, after "."
    tmp = int(heading*100.0f)%100;
    outBuff[payloadLen + 0] = 0x14 + 8;
    outBuff[payloadLen + 1] = lowByte(tmp);
    outBuff[payloadLen + 2] = lowByte(tmp);
    addedLen = 3;
    break;
  default:
    addedLen = 0;
  }
  return addedLen;
}

byte addEnd() {
  return 1;
}

byte sendPayload(byte len) {

  frsky.write(0x5E);

  for (byte pos = 0; pos <= len - 1; pos = pos + 3) {
    frsky.write(byte(outBuff[pos + 0]));

    switch  (outBuff[pos + 1]) {
    case 0x5E:
      frsky.write(byte(0x5D));
      frsky.write(byte(0x3E));
      break;
    case 0x5D:
      frsky.write(byte(0x5D));
      frsky.write(byte(0x3D));
      break;

    default:
      frsky.write(byte(outBuff[pos + 1]));
    }

    switch  (outBuff[pos + 2]) {
    case 0x5E:
      frsky.write(byte(0x5D));
      frsky.write(byte(0x3E));
      break;
    case 0x5D:
      frsky.write(byte(0x5D));
      frsky.write(byte(0x3D));
      break;
    default:
      frsky.write(byte(outBuff[pos + 2]));
    }
    frsky.write(0x5E);
  }
  return 0;
}
#endif


