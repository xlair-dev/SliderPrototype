#include <Wire.h>
#include "Adafruit_MPR121.h"

Adafruit_MPR121 cap0 = Adafruit_MPR121();
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();

void setup() {
  Serial.begin(9600);

  // needed to keep leonardo/micro from starting too fast!
  while (!Serial) {
    delay(10);
  }
  Serial.println("Serial is ready!");

  if (!cap0.begin(0x5A)) {
    Serial.println("MPR121 (0x5A) not found");
    while (true) {}
  }
  Serial.println("MPR121 (0x5A) is ready!");

  if (!cap1.begin(0x5B)) {
    Serial.println("MPR121 (0x5B) not found");
    while (true) {}
  }
  Serial.println("MPR121 (0x5B) is ready!");

  if (!cap2.begin(0x5C)) {
    Serial.println("MPR121 (0x5C) not found");
    while (true) {}
  }
  Serial.println("MPR121 (0x5C) is ready!");
}

void loop() {
  auto touched = cap2.touched();

  for (uint8_t i = 0; i < 12; i++) {
    if (touched & (1 << i)) {
      Serial.print("#");
    } else {
      Serial.print(".");
    }
  }

  touched = cap1.touched();

  for (uint8_t i = 0; i < 12; i++) {
    if (touched & (1 << i)) {
      Serial.print("#");
    } else {
      Serial.print(".");
    }
  }

  touched = cap0.touched();

  for (uint8_t i = 0; i < 12; i++) {
    if (touched & (1 << i)) {
      Serial.print("#");
    } else {
      Serial.print(".");
    }
  }

  Serial.println("");
}