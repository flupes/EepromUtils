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
#ifndef SafeEeprom_h
#define SafeEeprom_h

#include <stddef.h>
#include <stdint.h>

/**
   Class to access the permanent storage (EEPROM) of the AVR chip.
   
   This class aggregates static methods for a direct access to the
   EEPROM. It simply wraps the avr/eeprom.h functionality.
 */
class SafeEeprom
{
public:
  /** Write a byte to the EEPROM.
      @param addr       address to put the byte
      @param data       byte to write
  */
  static void write_byte(uint16_t addr, uint8_t data);
  
  /** Read a byte from the EEPROM.
      @param addr       address of the byte to read
      @return           byte read
  */
  static uint8_t read_byte(uint16_t addr);
  
  /** Write a word (unsigned 16 bits int) to the EEPROM.
      @param addr       address to put the word
      @param data       word to write
  */
  static void write_word(uint16_t addr, uint16_t data);
  
  /** Read a word (unsigned 16 bits int) from the EEPROM.
      @param addr       address of the 2 bytes to read
      @return           word read
  */
  static uint16_t read_word(uint16_t addr);

  /** Write a long (unsigned 32 bits int) to the EEPROM.
      @param addr       address to put the long
      @param data       long to write
  */
  static void write_long(uint16_t addr, uint32_t data);
  
  /** Read a long (unsigned 32 bits int) from the EEPROM.
      @param addr       address of the 4 bytes to read
      @return           long read
  */
  static uint32_t read_long(uint16_t addr);

  /** Write a block of data to the EEPROM.
      @param addr       address to put the data
      @param data       pointer to data to write
      @param len        size of the block of data to write
  */
  static void write_block(uint16_t addr, void* data, size_t len);
  
  /** Read a block of data from the EEPROM.
      @param addr       address of the data to read
      @param data       pointer to some RAM storage for the data to read
      @param len        size of the data to read (in bytes)
  */
  static void read_block(uint16_t addr, void* data, size_t len);

  /** Return the EEPROM total size (measured in bytes).
   */
  static int memSize();

  /** Return the size of one EEPROM page for this board.
   */
  static int pageSize();

  /** Print on the serial port the content of the EEPROM.
      
      The function prints the bytes per memory pages, so it may
      include some extra padding bytes at the start and the end.

      @param start  start address [default=0 -> first EEPROM byte]
      @param len    how many bytes to print [default=-1 -> print all]
  */
  static void show(uint16_t start=0, int len=-1);

};

#endif
