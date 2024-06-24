#include <Wire.h>
#include "Adafruit_MPR121.h"

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t state[12]  = {};
uint16_t rstate[12] = {};
bool     tstate[12] = {};

void setup() {
  Serial.begin(9600);

  // needed to keep leonardo/micro from starting too fast!
  while (!Serial) {
    delay(10);
  }

  Serial.println("Serial ready!");
  
  if (!cap.begin(0x5A)) {
    Serial.println("cap (0x5A) error!");
    while (1);
  }

  Serial.println("cap (0x5A) ready!");
}

void loop() {
  for (uint8_t i = 0; i < 12; i++) {
    if (cap.filteredData(i) < cap.baselineData(i) - 5){
      state[i]++;
      rstate[i] = 0;
    } else {
      rstate[i]++;
      state[i] = 0;
    }
  }

  for (uint8_t i = 0; i < 12; i++) {
    if (!tstate[i]) {
      if (state[i] >= 3) {
        tstate[i] = true;
      }
    } else {
      if (rstate[i] >= 3) {
        tstate[i] = false;
      }
    }
  }

  for (uint8_t i = 0; i < 12; i++) {
    if (tstate[i]) {
      Serial.print("#");
    } else {
      Serial.print(".");
    }
  }
  Serial.println("");
}
