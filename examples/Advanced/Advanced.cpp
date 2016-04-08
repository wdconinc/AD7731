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

/*
 * Advanced.cpp
 *
 *  Created on: Apr 7, 2016
 *      Author: Wouter Deconinck
 */

#include "AD7731.h"
#include "Serial.h"

// Create global AD7731 ADC variable
AD7731 ADC();

void setup()
{
  // Start serial
  Serial.begin(9600);

  // Reset ADC
  ADC.reset(); // not strictly required

  // Set range to -1280 mV to +1280 mV
  ADC.setPolarity(AD7731::BIPOLAR);
  ADC.setRange(AD7731::MODE_RANGE_1280mV);

  // Print registers to serial stream
  ADC.printRegisterMode(Serial);
  ADC.printRegisterGain(Serial);
  ADC.printRegisterFilter(Serial);
  ADC.printRegisterOffset(Serial);
}

void loop()
{
  Serial.print(ADC.getData(),HEX);

  delay(1000);
}
