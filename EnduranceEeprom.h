/**
   EnduranceEeprom.h is part of EepromUtils.

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
#ifndef EnduranceEeprom_h
#define EnduranceEeprom_h

#include <stddef.h>
#include <avr/io.h>

/**
   EnduranceEeprom increases the number of data writes you can do in the
   EEPROM by using a circular buffer to spread the data writes across the
   memory.


   Obviously, storing data to the EEPROM using EnduranceEeprom consume
   much more data space. This is the trade-off to allow more data writes
   than the number of cycles the EEPROM can support. Typically the AVR
   chip will allow 100K writes. This can seems a lot, but imagine you need
   to store a value every minute to the EEPROM, after 70 days of run-time
   the EEPROM would be dead.


   To keep track of the current address of the data in the circular buffer,
   a second circular buffer for the status (of the same size than the data
   buffer) is maintained on EEPROM. For more details on this technique:
   http://www.atmel.com/dyn/resources/prod_documents/doc2526.pdf

   The AVR implementation of the EEPROM writes always write a full PAGE
   (usually 4 bytes), even if you perform a single byte write. Because of
   this, there is no reason to use data space smaller than a PAGE size.

   The implementation of the status buffer uses 4 bytes: 2 for the index
   and 2 for a CRC16.

   @note Currently the CRC16 is not really used...
 */
class EnduranceEeprom
{
public:
  
  /** Initialize a Endurance EEPROM data structure.
      
      @param startAddr      Where in the EEPROM the data structure should start
      @param endurFactor    Endurance Factor: size of the circular buffer
      @param dataSize       Size of the element to store in the Endurance EEPROM.

      @note If endurFactor is 1, we have a degenerative scenario and the
      circular buffer algorithm would not work. However, EnduranceEeprom
      can be initialized with an endurFactor of 1. In this case, the data
      is simply stored in a single area of memory and no status buffer is
      used. Of course, this scenario would be more efficiently handled
      with SafeEeprom, however it allows for other classes using
      EnduranceEeprom to have a unified interface to the EEPROM with or
      without endurance, and will not consume more space than the dataSize
      itself if no endurance is required.
   */
  EnduranceEeprom(uint16_t startAddr, uint16_t endurFactor, size_t dataSize);

  /** Return the total space required for this EnduranceEeprom data structure.
   */
  uint16_t storageSize();

  /** Write the data to the EEPROM.
   */
  void writeData(void *data);
  
  /** Read the data from the EEPROM.
   */
  bool readData(void *data);
  
  /** Internal structure for the status buffer.
      It is made public for others to evaluate the size of the structure.
  */
  struct Status {
    uint16_t index;
    uint16_t crc16;
  };

protected:
  /** Current status of the circular buffers */
  Status m_status;
  
  /** Address of the beginning of the status circular buffer. */
  uint16_t m_statusAddr;

  /** Address of the beginning of the data circular buffer. */
  uint16_t m_dataAddr;

  /** Endurance factor. */
  uint16_t m_endurFactor;

  /** Size of the data sample to store. */
  size_t m_dataSize;

  /** Compute the CRC16 of the a data sample. */
  uint16_t memCrc16(uint16_t addr, size_t len);

  /** Find the current status buffer at boot time. */
  bool findCurrent();

};

#endif
