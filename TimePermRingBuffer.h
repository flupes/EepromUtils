#ifndef TimePermRingBuffer_h
#define TimePermRingBuffer_h

#include "EepromRingBuffer.h"
#include "DataSample.h"

class TimePermRingBuffer : public EepromRingBuffer
{
public:
  TimePermRingBuffer(uint16_t startAddr, uint16_t bufferSize,
                     size_t dataSize, int timePeriod);

  void insert(DataSample &data, long time);

  long read(int index, DataSample &data);

  long lastTimeStamp();

  uint16_t storageSize();

protected:
  int m_period;
  EnduranceEeprom m_lastTimeStamp;

};

#endif
