/*
  LM 75 temperature sensor reader.
*/
#include <Wire.h>
#include <math.h>

const uint8_t LM75_ADDR = 0x48;


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Wire.begin();

  Serial.println("LM75 reader start...");
}

float readLM75Celsius() {
  Wire.beginTransmission(LM75_ADDR);
  Wire.write(0x00); // LM75 temperature register
  Wire.endTransmission(false);

  Wire.requestFrom(LM75_ADDR, (uint8_t)2);

  if (Wire.available() < 2) {
    return NAN;
  }

  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();

  int16_t raw = ((int16_t)msb << 8) | lsb;

  // LM75: 9-bit temperature, 0.5°C resolution
  raw >>= 7;

  // sign extension for negative temperature
  if (raw & 0x0100) {
    raw |= 0xFE00;
  }

  return raw * 0.5;
}

void loop() {
  float tempC = readLM75Celsius();

  if (isnan(tempC)) {
    Serial.println("LM75 read failed");
  } else {
    Serial.print("TEMP_C=");
    Serial.println(tempC, 1);
  }

  delay(1000);
}
