/*
Copyright (C) 2016 Wouter Deconinck

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "AD7731.h"

AD7731::AD7731(unsigned int baud = 0)
: hasSerial(false)
{
  // Initialize SPI connection
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV16);

  // Initialize serial connection
  if (baud > 0) {
    hasSerial = true;
    Serial.begin(baud);
    Serial.println("AD7731 init");
  }
}

AD7731::~AD7731()
{
  // Close SPI connection
  SPI.end();

  // Close serial connection
  if (hasSerial) {
    Serial.println("AD7731 end");
    Serial.end();
  }
}
