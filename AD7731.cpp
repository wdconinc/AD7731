#include "AD7731.h"

AD7731::AD7731(bool serial = false) {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  
  if (serial) {
    Serial.begin(9600);
    Serial.println("past setup");
  }
}

