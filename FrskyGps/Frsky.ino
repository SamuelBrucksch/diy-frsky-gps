#ifdef FRSKY_D

void update_frsky() {

  f_curMillis = millis();
  if (f_curMillis - f_preMillis > f_delMillis) {
    // save the last time you sent the messaga
    f_preMillis = f_curMillis;

    // 200ms payload, construct Frame 1 on every loop
    packetOpen = true;

    payloadLen += addPayload(0x14);   // Course, degree
    payloadLen += addPayload(0x1c);   // Course, after "."

    payloadLen += addPayload(0x13);   // Longitude dddmmm
    payloadLen += addPayload(0x1b);   // Longitude .mmmm (after ".")
    payloadLen += addPayload(0x23);   // E/W

    payloadLen += addPayload(0x12);   // Latitude dddmmm
    payloadLen += addPayload(0x1a);   // Latitude .mmmm (after ".")
    payloadLen += addPayload(0x22);   // N/S

    payloadLen += addPayload(0x11);   // GPS Speed Knots
    payloadLen += addPayload(0x19);   // GPS Speed after "."

    payloadLen += addPayload(0x01);   // GPS Altitude
    payloadLen += addPayload(0x09);   // GPS Altitude "."

    payloadLen += addPayload(0x05);   // Temperature 2 -> sats

    packetOpen = false;
    payloadLen = sendPayload(payloadLen);

    // Update loop counter
    msCounter ++;
  }
}

byte addPayload(byte DataID) {
  byte addedLen;
  switch (DataID) {
  case 0x01:  // GPS Altitude
    outBuff[payloadLen + 0] = 0x01;
    outBuff[payloadLen + 1] = lowByte(gps_alt);
    outBuff[payloadLen + 2] = highByte(gps_alt);
    addedLen = 3;
    break;
  case 0x01+8:  // GPS Altitude
    {
      //float tmp = (gps_alt - int(gps_alt)) * 10000.0f;
      outBuff[payloadLen + 0] = 0x01 + 8;
      outBuff[payloadLen + 1] = 0;
      outBuff[payloadLen + 2] = 0;
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
    outBuff[payloadLen + 1] = sats;
    outBuff[payloadLen + 2] = fix;
    addedLen = 3;
    break;
  case 0x11:  // GPS Speed, before "."
    outBuff[payloadLen + 0] = 0x11;
    outBuff[payloadLen + 1] = lowByte(groundspeed);//FixInt(groundspeed, 1);
    outBuff[payloadLen + 2] = highByte(groundspeed);//FixInt(groundspeed, 2);
    addedLen = 3;
    break;
  case 0x11+8:  // GPS Speed, after "."
    outBuff[payloadLen + 0] = 0x11 + 8;
    outBuff[payloadLen + 1] = 0x00;
    outBuff[payloadLen + 2] = 0x00;
    addedLen = 3;
    break;
    //Little Endian exception
  case 0x12:  // Longitude, before "."
    outBuff[payloadLen + 0] = 0x12;
    outBuff[payloadLen + 1] = lowByte(long(lon));
    outBuff[payloadLen + 2] = highByte(long(lon));
    //outBuff[payloadLen + 1] = FixInt(long(lon), 1);
    //outBuff[payloadLen + 2] = FixInt(long(lon), 2);
    addedLen = 3;
    break;
  case 0x12+8:  // Longitude, after "."
    outBuff[payloadLen + 0] = 0x12 + 8;
    outBuff[payloadLen + 1] = lowByte(long((lon - long(lon)) * 100000.0));  // Only allow .00000 5 digits
    outBuff[payloadLen + 2] = highByte(long((lon - long(lon)) * 100000.0));  // Only allow .00000 5 digits after .
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
    outBuff[payloadLen + 1] = lowByte(long((lat - long(lat)) * 100000.0));   // Only allow .00000 5 digits
    outBuff[payloadLen + 2] = highByte(long((lat - long(lat)) * 100000.0));  // Only allow .00000 5 digits after .
    addedLen = 3;
    break;
  case 0x1B+8:  // N/S
    outBuff[payloadLen + 0] = 0x1B + 8;
    outBuff[payloadLen + 1] = lat_dir;
    outBuff[payloadLen + 2] = 0;
    addedLen = 3;
    break;
  case 0x14:  // course, before ".". OK
    outBuff[payloadLen + 0] = 0x14;
    outBuff[payloadLen + 1] = lowByte(heading);//FixInt(heading, 1);
    outBuff[payloadLen + 2] = highByte(heading);//FixInt(heading, 2);
    addedLen = 3;
    break;
  case 0x14+8:  // course, after "."  .. check calculation
    outBuff[payloadLen + 0] = 0x14 + 8;
    outBuff[payloadLen + 1] = 0x00;
    outBuff[payloadLen + 2] = 0x00;
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

// Sending packets. Create frame with correct data
// Frame format:
//
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

