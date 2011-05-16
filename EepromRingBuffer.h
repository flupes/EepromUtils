/**
   EepromRingBuffer.h is part of EepromUtils.

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
#ifndef EepromRingBuffer_h
#define EepromRingBuffer_h

#include "EnduranceEeprom.h"

/** 
    Ring Buffer stored on the EEPROM.
    
    Implement a ring buffer where data is stored in the EEPROM of the AVR
    chip for persistent storage.

    Despite EnduranceEeprom also uses a circular buffer to minimize the
    wear of the EEPROM, the purpose is really different: the
    EepromRingBuffer is designed to store a series of data samples.
 */
class EepromRingBuffer
{
public:
  /** Create a ring buffer on the EEPROM.
      @param startAddr      at which EEPROM address the data structure will start
      @param bufferSize     desired size of ring buffer
      @param dataSize       size of the each element to store
      @param indexEndurance endurance factor to use for the ring buffer index

      @note Every new data push in the ring buffer will trigger a EEPROM
      write. By the nature itself of the ring buffer, the write will be
      spread out in memory according to the size of the ring
      buffer. However, the class also need to maintain some indexes to
      recover the last elements inserted in the ring buffer after a power
      loss. The index is updated at each write. Thus if indexEndurance is
      bigger than one, an EnduranceEeprom data structure will be used to
      maintain the index.
  */
  EepromRingBuffer(uint16_t startAddr, uint16_t bufferSize, size_t dataSize,
                   uint16_t indexEndurance=1);

  /** Push a new data sample in the buffer.
      @param data        pointer to the the data to be copied to the EEPROM buffer
  */
  void push(void *data);

  /** Rotate the ring buffer by *steps* elements.

      This methods increment the ring buffer last element by steps, and
      also mark all the skipped elements with 0xFF. After the rotate, the
      ring buffer index point to the element with only 0xFF that rotate
      created.

      @param steps      number of single element rotation to perform
   */
  void rotate(uint16_t steps);

  /** Returns the element in the ring buffer referenced by the given  index.
     
      Index is a signed integer and can be positive of negative. 0 index
      returns the last elements inserted in the ring buffer. A positive
      index indicate a past element (from the last). A negative (less
      meaningful) will be accessing elements from the older one: -1 is
      the older element.

      @warning The positive/negative meaning of index can be
      counter intuitive since we query elements in the past with positive
      indexes.

      @param index      index of the desired element
      @param data       pointer to a location where to store the data read

      @note The index is always wrapped around (modulo buffer size). So
      even indexes larger than the size of the ring buffer are valid.
   */
  void get(int index, void *data);

  /** Clears completely the ring buffer.

      This methods writes 0xFF to all the EEPROM bytes used by the ring
      buffer. The EEPROM area used to store the endurance indexes are not
      cleared, but the last element is assigned to the first memory
      address of the ring buffer (not a significant things from the user
      point of view).
   */
  void clear();

  /** Returns the total storage size on the EEPROM used by the ring buffer structure.

      The return size includes the Endurance Indexes size plus the
      Ring Buffer size itself.
      
      @return total storage size required for the ring buffer
   */
  uint16_t storageSize();

  /** Returns the size of the buffer in element unit.

      @note This method require one division. The trade-off here is that we
      save on int of storage by keeping the total buffer length in bytes
      rather than the buffer size: the buffer length is used more often
      and thus we compute buffer size if required.
  */
  uint16_t bufferSize();

  uint16_t currentIndex();

  /** Structure to maintain the ring buffer indexes */
  struct Indexes {
    uint16_t last;  /** Index of the last element inserted in the ring buffer */
    uint16_t start; /** Extra index, not used currently. Just pad the 4 bytes. */
  };

protected:
  EnduranceEeprom m_eepromIndex;
  uint16_t m_bufferLength;          /** Store the total length of the buffer:
                                        ring buffer size * data size */
  uint16_t m_dataSize;              /** Keep the the data size of one element */

  Indexes m_ramIndex;

  uint16_t m_bufferStart;           /** Start of the the Ring Buffer */

};

#endif
