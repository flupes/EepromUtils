/**
   EnduranceEeprom.cpp is part of EepromUtils.

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
#include "EnduranceEeprom.h"

#include <avr/eeprom.h>
#include <util/crc16.h>

#include "SafeEeprom.h"

#ifndef NDEBUG
#include <HardwareSerial.h>
#endif

#include <stdlib.h>     // for exit

EnduranceEeprom::EnduranceEeprom(uint16_t startAddr, uint16_t endurFactor, size_t dataSize) :
  m_statusAddr(startAddr),
  m_endurFactor(endurFactor),
  m_dataSize(dataSize)
{
  if ( m_endurFactor > 1 ) {
    // Check if there is enough memory from the start address
    if ( (startAddr+storageSize()) > (1+E2END) ) {
      exit(-1);
    }
#ifndef NDEBUG
    if ( ( dataSize % E2PAGESIZE ) != 0 ) {
      Serial.println("EnduranceEeprom Warning: dataSize is not a multiple of the page size -> non optimal endurance!");
    }
#endif
    m_dataAddr = m_statusAddr+m_endurFactor*sizeof(Status);
    bool found = findCurrent();
    if ( ! found ) {
      // This area of memory has never been used for this circular buffer
      m_status.index = 1;
      for ( uint16_t i=0; i<m_dataSize; i++) {
        SafeEeprom::write_byte(m_dataAddr+i, 0xFF);
      }
      m_status.crc16 = memCrc16(m_dataAddr, m_dataSize);
      SafeEeprom::write_block(m_statusAddr, (void *)&m_status, sizeof(Status));
    }
  }
  else {
    m_dataAddr = m_statusAddr;
  }
}

void EnduranceEeprom::writeData(void *data)
{
  if ( m_endurFactor > 1 ) {  
    // m_status.index already point to the next element
    // we wrap it here and keep it value for the 2 writes.
    uint16_t index = m_status.index % m_endurFactor;
    
    // destination of the next data write
    uint16_t addr = m_dataAddr+index*m_dataSize;
    
    // writing first the data
    SafeEeprom::write_block(addr, data, m_dataSize);
    
    // crc computation
    m_status.crc16 = memCrc16(addr, m_dataSize);
    
    // we are incrementing our position (status data, not memory pointer)
    m_status.index++;
    
    // writing last the new status: index + crc together
    SafeEeprom::write_block(m_statusAddr+index*sizeof(Status), (void *)&m_status, sizeof(Status));
  }
  else {
    SafeEeprom::write_block(m_dataAddr, data, m_dataSize);
  }
}

bool EnduranceEeprom::readData(void *data)
{
  if ( m_endurFactor > 1 ) {  
    uint16_t addr = m_dataAddr+((m_status.index-1)%m_endurFactor)*m_dataSize;
    SafeEeprom::read_block(addr, data, m_dataSize);
    uint16_t crc = memCrc16(addr, m_dataSize);
    if ( crc == m_status.crc16 ) return true; else return false;
  }
  else {
    SafeEeprom::read_block(m_dataAddr, data, m_dataSize);
    return true;
  }
}

uint16_t EnduranceEeprom::storageSize()
{
  if ( m_endurFactor > 1 ) {  
    return m_endurFactor*(sizeof(Status)+m_dataSize);
  }
  else {
    return m_dataSize;
  }
}

uint16_t EnduranceEeprom::memCrc16(uint16_t addr, size_t len)
{
  uint16_t crc = 0xFFFF;
  for (uint16_t i=0; i<len; i++) {
    crc = _crc16_update(crc, SafeEeprom::read_byte(addr++));
  }
  return crc;
}

bool EnduranceEeprom::findCurrent()
{
  bool found = false;
  uint16_t addr = m_statusAddr;
  uint16_t next;
  Status ns;
  // Iterate trhough the status buffer to find which was the last element
  while ( addr < m_endurFactor*sizeof(Status) || ! found ) {
    next = addr + sizeof(Status);
    if ( next == m_endurFactor*sizeof(Status) ) next = m_statusAddr;
    SafeEeprom::read_block(addr, (void *)&m_status, sizeof(Status));
    SafeEeprom::read_block(next, (void *)&ns, sizeof(Status));
    if ( (ns.index-m_status.index) > 1 ) found = true; else addr += sizeof(Status);
  }
  // Check CRC to make sure this value was correctly written 
  uint16_t crc = memCrc16(m_dataAddr+(m_status.index-1)*m_dataSize, m_dataSize);
  if ( crc != m_status.crc16 ) {
#ifndef NDEBUG
    Serial.println("EnduranceEeprom Warning: memory corruption detected!");
#endif
    // Not sure yet what to do in case of corrupted data...
  }
  return found;
}
