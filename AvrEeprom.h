/**
   SafeEeprom.h is part of EepromUtils.

   Copyright (c) 2011 Lorenzo Flueckiger

   EepromUtils is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   EepromUtils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with EepromUtils. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef AvrEeprom_h
#define AvrEeprom_h

#include "SafeEeprom.h"

/**
   Class to access the permanent storage (EEPROM) of the AVR chip.
   
   This class aggregates static methods for a direct access to the
   EEPROM. It simply wraps the avr/eeprom.h functionality.

   Documentation of each method is provided by the interface SafeEeprom.
 */
class AvrEeprom : public SafeEeprom
{
public:
  static AvrEeprom &instance() {
    static AvrEeprom ee;
    return ee;
  }
  
  void write_byte(uint16_t addr, uint8_t data);
  
  uint8_t read_byte(uint16_t addr);
  
  void write_word(uint16_t addr, uint16_t data);
  
  uint16_t read_word(uint16_t addr);

  void write_long(uint16_t addr, uint32_t data);
  
  uint32_t read_long(uint16_t addr);

  void write_block(uint16_t addr, void* data, size_t len);

  void read_block(uint16_t addr, void* data, size_t len);

  uint16_t memSize();

  uint16_t pageSize();

  void show(uint16_t start=0, int len=-1);

private:
  AvrEeprom() {
    // cannot call externally
  }
  
  // prohibited...
  AvrEeprom(AvrEeprom const&);
  void operator=(AvrEeprom const&);

};

#endif
