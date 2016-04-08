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

    AD7731(bool serial = false, int baud = 9600);
    ~AD7731();

    // Set the input range
    #define AD7731_MODE_RANGE_MASK 0x0070
    enum AD7731_MODE_RANGE {
      MODE_RANGE_10mV   = 0x0000,
      MODE_RANGE_20mV   = 0x0010,
      MODE_RANGE_40mV   = 0x0020,
      MODE_RANGE_80mV   = 0x0030,
      MODE_RANGE_160mV  = 0x0040,
      MODE_RANGE_320mV  = 0x0050,
      MODE_RANGE_640mV  = 0x0060,
      MODE_RANGE_1280mV = 0x0070
    };
    void setRange(const AD7731_MODE_RANGE r) {
      register_mode = register_mode | (r & AD7731_MODE_RANGE_MASK);
      writeRegisterMode(register_mode);
    }

    // Set the polarity (bipolar v. unipolar)
    #define AD7731_MODE_POLARITY_MASK 0x1000
    enum AD7731_MODE_POLARITY {
      BIPOLAR  = 0x0000,
      UNIPOLAR = 0x1000
    };
    void setPolarity(const AD7731_MODE_POLARITY p) {
      register_mode = register_mode | (p & AD7731_MODE_POLARITY_MASK);
      writeRegisterMode(register_mode);
    }
    const AD7731_MODE_POLARITY getPolarity() const {
      return readRegisterMode() & AD7731_MODE_POLARITY_MASK;
    }

    #define AD7731_MODE_CHANNEL_MASK 0x0007
    enum AD7731_MODE_CHANNEL {
      PSEUDO_DIFF_AIN16 = 0x0000,
      PSEUDO_DIFF_AIN26 = 0x0001,
      PSEUDO_DIFF_AIN36 = 0x0002,
      PSEUDO_DIFF_AIN46 = 0x0003,
      FULLY_DIFF_AIN12  = 0x0004,
      FULLY_DIFF_AIN34  = 0x0005,
      FULLY_DIFF_AIN56  = 0x0006,
      TEST              = 0x0007
    };
    void setChannelMode(const AD7731_MODE_CHANNEL c) const {
      setRegisterBits(register_mode,c,AD7731_MODE_CHANNEL_MASK);
      writeRegisterMode();
    }

    #define AD7731_MODE_DOUT_MASK 0x0f00
    enum AD7731_MODE_DOUT {
      DISABLE = 0x0000,
      ENABLE  = 0x0800,
      D1      = 0x0400,
      D0      = 0x0200
    };
    void setDigitalOut(const AD7731_MODE_DOUT d) const {
      setRegisterBits(register_mode,d,AD7731_MODE_DOUT_MASK);
      writeRegisterMode();
    }

    #define AD7731_MODE_WORDLENGTH_MASK 0x0100
    enum AD7731_MODE_WORDLENGTH {
      WL_16BITS = 0x0000,
      WL_24BITS = 0x0100
    };
    void setWordLength(const AD7731_MODE_WORDLENGTH wl) const {
      setRegisterBits(register_mode,wl,AD7731_MODE_WORDLENGTH_MASK);
      writeRegisterMode();
    }

    void setRefIn(float minus, float plus) {
      V_ref_in_minus = minus;
      V_ref_in_plus  = plus;
    }
    float getRefInMinus() { return V_ref_in_minus; };

    // Reset the chip
    void reset() {
      // Writing 4 times 8 ones resets the chip
      SPI.transfer(0xFF);
      SPI.transfer(0xFF);
      SPI.transfer(0xFF);
      SPI.transfer(0xFF);
    }

    // Write mode register
    void printRegisterMode(Stream stream) {
      unsigned int reg = readRegisterMode();
      stream.print("AD7731: mode ");
      stream.println(reg, HEX);
    }

    // Write gain register
    void printRegisterGain(Stream stream) {
      unsigned int reg = readRegisterGain();
      stream.print("AD7731: gain ");
      stream.println(reg, HEX);
    }

    // Write filter register
    void printRegisterFilter(Stream stream) {
      unsigned int reg = readRegisterFilter();
      stream.print("AD7731: filter ");
      stream.println(reg, HEX);
    }

    // Write offset register
    void printRegisterOffset(Stream stream) {
      unsigned int reg = readRegisterOffset();
      stream.print("AD7731: offset ");
      stream.println(reg, HEX);
    }

  private:

    void writeRegisterOffset() {
      SPI.transfer(AD7731_COMM_WRITE_REGISTER_OFFSET_Y);
      SPI.transfer(register_offset & 0xff0000 >> 16);
      SPI.transfer(register_offset & 0x00ff00 >> 8);
      SPI.transfer(register_offset & 0x0000ff >> 0);
    }

    void writeRegisterFilter() {
      SPI.transfer(AD7731_COMM_WRITE_REGISTER_FILTER_Y);
      SPI.transfer(register_filter & 0xff0000 >> 16);
      SPI.transfer(register_filter & 0x00ff00 >> 8);
      SPI.transfer(register_filter & 0x0000ff >> 0);
    }

    void writeRegisterGain() {
      SPI.transfer(AD7731_COMM_WRITE_REGISTER_GAIN_Y);
      SPI.transfer(register_gain & 0xff00 >> 8);
      SPI.transfer(register_gain & 0x00ff >> 0);
    }

    void writeRegisterMode() {
      SPI.transfer(AD7731_COMM_WRITE_REGISTER_MODE_Y);
      SPI.transfer(register_mode & 0xff00 >> 8);
      SPI.transfer(register_mode & 0x00ff >> 0);
    }

    unsigned int readRegisterOffset() {
      SPI.transfer(AD7731_COMM_READ_REGISTER_OFFSET_Y);
      return readBytes(3) & 0xffffff;
    }

    unsigned int readRegisterFilter() {
      SPI.transfer(AD7731_COMM_READ_REGISTER_FILTER_Y);
      return readBytes(3) & 0xffffff;
    }

    unsigned int readRegisterGain() {
      SPI.transfer(AD7731_COMM_READ_REGISTER_GAIN_Y);
      return readBytes(2) & 0xffff;
    }

    unsigned int readRegisterMode() {
      SPI.transfer(AD7731_COMM_READ_REGISTER_MODE_Y);
      return readBytes(2) & 0xffff;
    }

    // Set register bits with bits and mask
    void setRegisterBits(unsigned int & reg, unsigned int bits, unsigned int mask) {
      return (reg & ~mask) | (bits & mask);
    }

    // Read a specified number of bytes from SPI (n = 1,2,3,4)
    unsigned int readBytes(unsigned int n) {
      unsigned int value = 0;
      for (unsigned int i = 0; i < n; i++) {
        value <<= 8;
        value |= SPI.transfer(0);
      }
      return value;
    }

    float V_ref_in_minus, V_ref_in_plus;

    // Registers for writing (local copy)
    unsigned int register_mode; // 16 bits
    unsigned int register_gain; // 16 bits
    unsigned int register_filter; // 24 bits
    unsigned int register_offset; // 24 bits
};

#endif // AD7731
