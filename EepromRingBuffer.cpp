/**
   EepromRingBuffer.cpp is part of EepromUtils.

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
#include "EepromRingBuffer.h"

#include "SafeEeprom.h"

#ifndef NDEBUG
#include "HardwareSerial.h"
#endif

#include <stdlib.h>     // for exit

EepromRingBuffer::EepromRingBuffer(uint16_t startAddr,
                                   uint16_t bufferSize,
                                   size_t dataSize,
                                   uint16_t indexEndurance)
  : m_eepromIndex(startAddr, indexEndurance, sizeof(Indexes)),
    m_bufferLength(bufferSize*dataSize),
    m_dataSize(dataSize)
{
  m_bufferStart = startAddr + m_eepromIndex.storageSize();

  // Check if there is enough EEPROM
  if ( startAddr + storageSize() > (E2END+1) ) {
    exit(-1);
  }

  m_eepromIndex.readData((void *)&m_ramIndex);

  if ( 0xFFFF == m_ramIndex.last ) {
    // This buffer never existed before. Let's initialize it
    clear();
  }

}

void EepromRingBuffer::push(void *data)
{

#ifndef NDEBUG
  Serial.print("push: Current byte index = ");
  Serial.print(m_ramIndex.last, DEC);
#endif
  m_ramIndex.last = (m_ramIndex.last+m_dataSize) % m_bufferLength;
#ifndef NDEBUG
  Serial.print(" -> New byte index = ");
  Serial.println(m_ramIndex.last, DEC);
#endif
  SafeEeprom::write_block(m_bufferStart+m_ramIndex.last, data, m_dataSize);
  m_eepromIndex.writeData((void *)&m_ramIndex);
}

void EepromRingBuffer::get(int index, void *data)
{
#ifndef NDEBUG
  Serial.print("-- get index=");
  Serial.print(index, DEC);
#endif
  // The if statment for positive and negative values of the index
  // seems necessary because the module function did not generate
  // the desired values for negative numbers!
  if ( index < 0 ) {
    index = (-m_dataSize*index) % m_bufferLength;
    index = m_ramIndex.last + index;
    if ( (uint16_t)index > m_bufferLength-1 ) index -= m_bufferLength;
  }
  else {
    index = (m_dataSize*index) % m_bufferLength;
    index = m_ramIndex.last - index;
    if ( index < 0 ) index += m_bufferLength;
  }
#ifndef NDEBUG
  Serial.print(" -> byte index=");
  Serial.print(index, DEC);
  Serial.print(" :: ");
#endif
  SafeEeprom::read_block(m_bufferStart+index, data, m_dataSize);
}

void EepromRingBuffer::rotate(uint16_t steps)
{
#ifndef NDEBUG
  Serial.print("rotate (steps=");
  Serial.print(steps, DEC);
  Serial.print(") : Current byte index = ");
  Serial.print(m_ramIndex.last, DEC);
#endif
  if ( steps < bufferSize() ) {
    m_ramIndex.last += m_dataSize;
    for (uint16_t i=0; i<steps*m_dataSize; i++) {
      m_ramIndex.last = m_ramIndex.last % m_bufferLength;
      SafeEeprom::write_byte(m_bufferStart+m_ramIndex.last, 0xFF);
      m_ramIndex.last++;
    }
    m_ramIndex.last -= m_dataSize;
    m_ramIndex.last = m_ramIndex.last % m_bufferLength;
#ifndef NDEBUG
    Serial.print(" -> New byte index = ");
    Serial.println(m_ramIndex.last, DEC);
#endif
    m_eepromIndex.writeData((void *)&m_ramIndex);
  }
  else {
    clear();
  }
}

void EepromRingBuffer::clear()
{
  for (uint16_t i=0; i<m_bufferLength; i++) {
    SafeEeprom::write_byte(m_bufferStart+i, 0xFF);
  }
  m_ramIndex.last = 0;
  m_eepromIndex.writeData((void *)&m_ramIndex);
}

uint16_t EepromRingBuffer::storageSize()
{
  return m_eepromIndex.storageSize() + m_bufferLength;
}

uint16_t EepromRingBuffer::bufferSize()
{
  return m_bufferLength / m_dataSize;
}

uint16_t EepromRingBuffer::currentIndex()
{
  return m_ramIndex.last / m_dataSize;
}
