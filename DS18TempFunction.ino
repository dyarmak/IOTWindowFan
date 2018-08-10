#include <OneWire.h>

OneWire  ds(8);  // on pin 8 (a 4.7K resistor is necessary, my breakout has that included)
   byte addr[8];
   byte i;
   byte type_s;
   byte data[12];


void DS18B20SetupFunction(){
    
//check that there is a device connected
    if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
// Uncomment below to print addr of the found device
/*
    Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
*/
//Check the CRC for god knows what...
    if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
  
// We are using a DS18B20, so we know type_s = 0
 type_s = 0; 
}

void tempRead(){
  ds.reset();
  ds.select(addr);
  ds.write(0x44);        // start conversion, without parasite power
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  // Loop to read the data from the device
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  
  //This is the Cyclic Redundancy Check
  OneWire::crc8(data, 8);

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];

    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time

  outCelsius = (float)raw / 16.0;
  Serial.print(" Outside Temperature = ");
  Serial.print(outCelsius);
  Serial.println(" Celsius, ");
  } // End of tempRead()

  
