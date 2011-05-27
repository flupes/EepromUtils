#ifndef TimePermRingBuffer_h
#define TimePermRingBuffer_h

#include "EepromRingBuffer.h"
#include "DataSample.h"

/**
   TimePermRingBuffer is a ring buffer, augmented with concept of
   timestamped elements, implemented with EEPROM.

   This class is a EepromRingBuffer with the additional logic to handle
   timestamped sampled in the buffer. TimePermRingBuffer keeps the
   timestamp of the last inserted element on the EEPROM too. This allow
   to resume correct operations after a power loss.

   The time unit used for TimePermRingBuffer is coded on a long, but is
   completely arbitrary: it can represent seconds, hours or anything else.

   The buffer is created with a given "period", meaning expected interval
   (in time unit chosen) between two samples. Thus, the insert method can
   repetively be called, but it will only insert the passed element if the
   the rigth time as elapsed since the last insertion. In addition, when
   the elapsed time is larger than the deined period, the buffer is
   rotated (null values are inserted for the missing elements)
   accordingly.
 */
class TimePermRingBuffer : public EepromRingBuffer
{
public:
  TimePermRingBuffer(uint16_t startAddr, uint16_t bufferSize,
                     size_t dataSize, int timePeriod,
                     uint16_t endurFactor=8);

  bool insert(DataSample &data, long time);

  long read(int index, DataSample &data);

  /** Return the time stamp of the last element push in the buffer. 
      @return timestamp in arbitrary units
   */
  long lastTimeStamp();

  /** Artifically set the last time stamp of the buffer.
      @warning This method breaks the whole logic of the timed
      ring buffer. It should only be used for specific 
      initialization cases.
   */
  void setTimeStamp(long ts);

  /** Return the period of this timed ring buffer.
      @return delay between two samples
  */
  int period();

  uint16_t storageSize();

protected:
  int m_period;
  EnduranceEeprom m_lastTimeStamp;

};

#endif
