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

Info: http://www.analog.com/en/products/analog-to-digital-converters/ad-converters/ad7731.html
Datasheet: http://www.analog.com/media/en/technical-documentation/data-sheets/AD7731.pdf

Author:       Wouter Deconinck
Contributors: Lisa Oswald

*/

#ifndef AD7731_h
#define AD7731_h

#include "Arduino.h"
#include "SPI.h"
#include <cstdint>

#define AD7731_COMM_WEN  0x80
#define AD7731_COMM_RW1  0x20
#define AD7731_COMM_RW0  0x10
#define AD7731_COMM_RS2  0x04
#define AD7731_COMM_RS1  0x02
#define AD7731_COMM_RS0  0x01

#define AD7731_COMM_READ_REGISTER_STATUS_W 0x10
#define AD7731_COMM_READ_REGISTER_STATUS_Y 0x20
#define AD7731_COMM_READ_REGISTER_STATUS_Z 0x30

#define AD7731_COMM_READ_REGISTER_DATA_W 0x11
#define AD7731_COMM_READ_REGISTER_DATA_Y 0x21
#define AD7731_COMM_READ_REGISTER_DATA_Z 0x30

#define AD7731_COMM_READ_REGISTER_MODE_W 0x12
#define AD7731_COMM_READ_REGISTER_MODE_Y 0x22
#define AD7731_COMM_READ_REGISTER_MODE_Z 0x30

#define AD7731_COMM_READ_REGISTER_FILTER_W 0x13

#define AD7731_COMM_READ_REGISTER_OFFSET_W 0x15

#define AD7731_COMM_READ_REGISTER_GAIN_W 0x16

#define AD7731_COMM_WRITE_REGISTER_COMM_Y 0x00
#define AD7731_COMM_WRITE_REGISTER_MODE_Y 0x02
#define AD7731_COMM_WRITE_REGISTER_FILTER_Y 0x03
#define AD7731_COMM_WRITE_REGISTER_OFFSET_Y 0x05
#define AD7731_COMM_WRITE_REGISTER_GAIN_Y 0x06

#define AD7731_FILTER_1KHZ_NONCHOP 0x1332


#define AD7731_MODE_DOUT_DISABLE 0x0000
#define AD7731_MODE_DOUT_ENABLE  0x0800
#define AD7731_MODE_DOUT_D1_MASK 0x0400
#define AD7731_MODE_DOUT_D0_MASK 0x0200

#define AD7731_MODE_WORDLENGTH_16 0x0000
#define AD7731_MODE_WORDLENGTH_24 0x0100


#define AD7731_MODE_CHANNEL_PSEUDO_DIFF_AIN16   0x0000
#define AD7731_MODE_CHANNEL_PSEUDO_DIFF_AIN26   0x0001
#define AD7731_MODE_CHANNEL_PSEUDO_DIFF_AIN36   0x0002
#define AD7731_MODE_CHANNEL_PSEUDO_DIFF_AIN46   0x0003
#define AD7731_MODE_CHANNEL_FULLY_DIFF_AIN12    0x0004
#define AD7731_MODE_CHANNEL_FULLY_DIFF_AIN34    0x0005
#define AD7731_MODE_CHANNEL_FULLY_DIFF_AIN56    0x0006
#define AD7731_MODE_CHANNEL_TEST                0x0007

#define AD7731_FILTER_CHP  0x0004
#define AD7731_FILTER_SKIP 0x0002
#define AD7731_FILTER_FAST 0x0001

#define AD7731_STATUS_MASK_READY 0x80
#define AD7731_STATUS_MASK_STDY  0x40
#define AD7731_STATUS_MASK_STBY  0x20
#define AD7731_STATUS_MASK_NOREF 0x10
#define AD7731_STATUS_MASK_READY 0x08
#define AD7731_STATUS_MASK_STDY  0x04
#define AD7731_STATUS_MASK_STBY  0x02
#define AD7731_STATUS_MASK_NOREF 0x01

#define AD7731_MODE_IDLE    b00000000
#define AD7731_MODE_STANDBY b01100000
#define AD7731_MODE_CONVERSION_CONTINUOUS b00100000
#define AD7731_MODE_CONVERSION_SINGLE     b01000000
#define AD7731_MODE_CALIBRATION_INTERNAL_ZERO_SCALE b10000000
#define AD7731_MODE_CALIBRATION_INTERNAL_FULL_SCALE b10100000
#define AD7731_MODE_CALIBRATION_SYSTEM_ZERO_SCALE   b11000000
#define AD7731_MODE_CALIBRATION_SYSTEM_FULL_SCALE   b11100000



class AD7731 {

  public:

    AD7731();
    ~AD7731();


    #define AD7731_MODE_RANGE_MASK 0x0070
    enum AD7731_MODE_RANGE {
      MODE_RANGE_10mV   = 0x0000,
      MODE_RANGE_20mV   = 0x0010,
      MODE_RANGE_40mV   = 0x0020,
      MODE_RANGE_80mV   = 0x0030,
      MODE_RANGE_160mV  = 0x0040,
      MODE_RANGE_320mV  = 0x0050,
      MODE_RANGE_640mV  = 0x0060,
      MODE_RANGE_1280mV = 0x0070};
    void setRange(const AD7731_MODE_RANGE r) { range = r; }
    const AD7731_MODE_RANGE getRange() const { return range; }

    #define AD7731_MODE_POLARITY_MASK 0x1000
    enum AD7731_MODE_POLARITY { BIPOLAR = 0x0000, UNIPOLAR = 0x1000};
    void setPolarity(const AD7731_MODE_POLARITY p) {
    }
    const AD7731_MODE_POLARITY getPolarity() const {
      SPI.transfer(AD7731_COMM_READ_REGISTER_MODE_Y);
      SPI.transfer();
      return polarity;
    }


    void setRefIn(float minus, float plus) {
      V_ref_in_minus = minus;
      V_ref_in_plus  = plus;
    }
    float getRefInMinus() { return V_ref_in_minus; };
    

    unsigned int getData();

    void reset() {
      // Writing 4 times 8 ones resets the chip
      SPI.transfer(0xFF);
      SPI.transfer(0xFF);
      SPI.transfer(0xFF);
      SPI.transfer(0xFF);
    }

  private:

    void writeRegisterMode() {
      SPI.transfer(AD7731_COMM_WRITE_REGISTER_MODE_Y);
      register_mode = register_mode | (p & AD7731_MODE_POLARITY_MASK);
      SPI.transfer(register_mode);

      if (hasSerial) {
        Serial.print("AD7731: mode ");
        Serial.println(register_mode, HEX);
      }
    }

    uint16_t readRegisterMode() {
      SPI.transfer(AD7731_COMM_READ_REGISTER_MODE_Y);
      uint16_t reg = readBytes(2);
      uint16_t reg2 = SPI.transfer(0);
      if (hasSerial) {
        Serial.print("AD7731: mode ");
        Serial.println(register_mode, HEX);
      }
    }

    uint32_t readBytes(unsigned int n) {
      uint32_t value = 0;
      for (unsigned int i = 0; i < n; i++) {
        value <<= 8;
        value |= SPI.transfer(0);
      }
    }

    float V_ref_in_minus, V_ref_in_plus;

    bool hasSerial;

    // Registers for writing
    uint16_t register_mode; // 16 bits
    uint16_t register_gain; // 16 bits
    uint32_t register_filter; // 24 bits
    uint32_t register_offset; // 24 bits


    AD7731_MODE_RANGE range;
    AD7731_MODE_POLARITY polarity;
};

#endif // AD7731
