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

/* AD7731

Arduino library for the Analog Device AD7731 6-channel 24-bit ADC chip

http://www.analog.com/en/products/analog-to-digital-converters/ad-converters/ad7731.html

Author:       Wouter Deconinck
Contributors: Lisa Oswald

*/

#ifndef AD7731_h
#define AD7731_h

#include "Arduino.h"
#include <SPI.h>

#define AD7731_MODE_IDLE    0x0
#define AD7731_MODE_STANDBY 0x3
#define AD7731_MODE_CONVERSION_CONTINUOUS 0x1
#define AD7731_MODE_CONVERSION_SINGLE     0x2
#define AD7731_MODE_CALIBRATION_INTERNAL_ZERO_SCALE 0x4
#define AD7731_MODE_CALIBRATION_INTERNAL_FULL_SCALE 0x5
#define AD7731_MODE_CALIBRATION_SYSTEM_ZERO_SCALE   0x6
#define AD7731_MODE_CALIBRATION_SYSTEM_FULL_SCALE   0x7

class AD7731 {

  public:
    AD7731() { };
    ~AD7731()  { };
    
  private:
    
}

#endif // AD7731
