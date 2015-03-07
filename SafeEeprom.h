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
   Interface to access a generic EEPROM.
*/
class SafeEeprom
{
public:
  /** Write a byte to the EEPROM.
      @param addr       address to put the byte
      @param data       byte to write
  */
  virtual void write_byte(uint16_t addr, uint8_t data);
  
  /** Read a byte from the EEPROM.
      @param addr       address of the byte to read
      @return           byte read
  */
  virtual uint8_t read_byte(uint16_t addr);
  
  /** Write a word (unsigned 16 bits int) to the EEPROM.
      @param addr       address to put the word
      @param data       word to write
  */
  virtual void write_word(uint16_t addr, uint16_t data);
  
  /** Read a word (unsigned 16 bits int) from the EEPROM.
      @param addr       address of the 2 bytes to read
      @return           word read
  */
  virtual uint16_t read_word(uint16_t addr);

  /** Write a long (unsigned 32 bits int) to the EEPROM.
      @param addr       address to put the long
      @param data       long to write
  */
  virtual void write_long(uint16_t addr, uint32_t data);
  
  /** Read a long (unsigned 32 bits int) from the EEPROM.
      @param addr       address of the 4 bytes to read
      @return           long read
  */
  virtual uint32_t read_long(uint16_t addr);

  /** Write a block of data to the EEPROM.
      @param addr       address to put the data
      @param data       pointer to data to write
      @param len        size of the block of data to write
  */
  virtual void write_block(uint16_t addr, void* data, size_t len);
  
  /** Read a block of data from the EEPROM.
      @param addr       address of the data to read
      @param data       pointer to some RAM storage for the data to read
      @param len        size of the data to read (in bytes)
  */
  virtual void read_block(uint16_t addr, void* data, size_t len);

  /** Return the EEPROM total size (measured in bytes).
   */
  virtual uint16_t memSize();

  /** Return the size of one EEPROM page for this board.
   */
  virtual uint16_t pageSize();

  /** Print on the serial port the content of the EEPROM.
      
      The function prints the bytes per memory pages, so it may
      include some extra padding bytes at the start and the end.

      @param start  start address [default=0 -> first EEPROM byte]
      @param len    how many bytes to print [default=-1 -> print all]
  */
  virtual void show(uint16_t start=0, int len=-1);

};

#endif
