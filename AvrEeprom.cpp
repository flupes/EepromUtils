/**
   AvrEeprom.cpp is part of EepromUtils.

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
#include <avr/eeprom.h>

#define SERIAL_DEBUG 1

#ifdef SERIAL_DEBUG
#include <HardwareSerial.h>
#endif

#define LAST E2END+1

#include "AvrEeprom.h"

void AvrEeprom::write_byte(uint16_t addr, uint8_t data)
{
  if ( addr < LAST )
    eeprom_write_byte((uint8_t *)addr, data);
}

uint8_t AvrEeprom::read_byte(uint16_t addr)
{
  return eeprom_read_byte((uint8_t *)addr);
}

void AvrEeprom::write_word(uint16_t addr, uint16_t data)
{
  if ( addr < LAST-1 )
    eeprom_write_word((uint16_t *)addr, data);
}

uint16_t AvrEeprom::read_word(uint16_t addr)
{
  return eeprom_read_word((uint16_t *)addr);
}

void AvrEeprom::write_long(uint16_t addr, uint32_t data)
{
  if ( addr < LAST-3 ) 
    eeprom_write_dword((uint32_t *)addr, data);
}

uint32_t AvrEeprom::read_long(uint16_t addr)
{
  return eeprom_read_dword((uint32_t *)addr);
}

void AvrEeprom::write_block(uint16_t addr, void* data, size_t len)
{
  if ( addr < LAST+1-len )
    eeprom_write_block(data, (void *)addr, len);
}

void AvrEeprom::read_block(uint16_t addr, void* data, size_t len)
{
  eeprom_read_block(data, (void *)addr, len);
}

uint16_t AvrEeprom::memSize()
{
  return E2END;
}

uint16_t AvrEeprom::pageSize()
{
  return E2PAGESIZE;
}

void AvrEeprom::show(uint16_t start, int len)
{
  uint16_t ptr;     // start of the first page we will print
  uint16_t end;     // just after the last page we will print

  // can only address bytes below the max eeprom limit
  if ( start > E2END ) return;
  // if no length is given, then print up to the end of the eeprom
  ptr = start - start % E2PAGESIZE;

  if ( len < 0 ) {
    end = E2END + 1;
  }
  else {
    // @bug It seems that we alway show one extra page...
    end = start+len - (start+len) % E2PAGESIZE + E2PAGESIZE;
    if ( end > E2END+1 ) end = E2END;
  }

#ifdef SERIAL_DEBUG
  // iterate through all pages
  int page = ptr / E2PAGESIZE;
  uint8_t data = 0;
  while ( ptr < end ) {
    Serial.print("bytes [");
    Serial.print(ptr, DEC);
    Serial.print("-");
    Serial.print(ptr+4, DEC);
    Serial.print("] (page=");
    Serial.print(page, DEC);
    Serial.print(") : ");
    for (int i=0; i<E2PAGESIZE; i++) {
      data = read_byte(ptr++);
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    page++;
    Serial.println();
  }
#else
#warning "AvrEeprom is a noop with SERIAL_DEBUG on."
#endif  
}
