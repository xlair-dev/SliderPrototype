#include <Wire.h>
#include "Adafruit_MPR121.h"

constexpr uint8_t CapsCount = 3;
constexpr uint8_t I2cAddrs[] = { 0x5A, 0x5B, 0x5C };
constexpr uint8_t SensorNum[] = { 10, 12, 10 };
Adafruit_MPR121 caps[CapsCount];

void setup() {
  Serial.begin(115200);

  // needed to keep leonardo/micro from starting too fast!
  while (!Serial) {
    delay(10);
  }
  // Serial.println("Serial is ready!");

  for (uint8_t i = 0; i < CapsCount; i++) {
    caps[i] = Adafruit_MPR121();
    if (!caps[i].begin(I2cAddrs[i])) {
      // Serial.print("Caps ");
      // Serial.print(i);
      // Serial.println(" not found.");
      while (true) {}
    }
    caps[i].setThresholds(8, 4);
  }

  // Serial.println("ready!");
}

void loop() {
  for (uint8_t i = 0; i < CapsCount; i++) {
    const auto touched = caps[i].touched();
    for (uint8_t t = 0; t < SensorNum[i]; t++) {
      if (touched & (1 << t)) {
        Serial.print("1");
      } else {
        Serial.print("0");
      }
    }
  }
  Serial.println("");
  Serial.flush();
}