#include "TimePermRingBuffer.h"

#include <HardwareSerial.h>

#define ENDURANCE_FACTOR 8

TimePermRingBuffer::TimePermRingBuffer(uint16_t startAddr, uint16_t bufferSize,
                                       size_t dataSize, int timePeriod) :
  EepromRingBuffer(startAddr, bufferSize, dataSize, ENDURANCE_FACTOR),
  m_period(timePeriod),
  m_lastTimeStamp(startAddr+storageSize(), ENDURANCE_FACTOR, sizeof(long))
{
  long time;
  m_lastTimeStamp.readData((void *)&time);
  if ( 0xFFFFFFFFl == time ) {
    time = -100000l;
    m_lastTimeStamp.writeData((void *)&time);
  }
}

void TimePermRingBuffer::insert(DataSample &data, long current_time)
{
  long last_time, delta, steps;
  m_lastTimeStamp.readData((void*)&last_time);
  delta = current_time - last_time;

  Serial.print("==== TimePermRingBuffer::insert -> current=");
  Serial.print(current_time, DEC);
  Serial.print(" - last_time=");
  Serial.print(last_time, DEC);
  Serial.print(" : delta=");
  Serial.println(delta, DEC);

  if ( delta > m_period * m_dataSize ) {
    // elapsed time greater than buffer time span
    Serial.println("------ elapsed time greater than buffer time span");
    clear();
    m_lastTimeStamp.writeData((void*)&current_time);
    push(data.data());
  }
  else {
    steps = delta / m_period;
    if ( steps > 0 ) {
      // this is time to insert the new sample
      if ( steps > 1 ) {
        // elapsed time more than one single period
        Serial.println("------ elapsed time more than a single period");
        rotate(steps-1);
      }
      m_lastTimeStamp.writeData((void*)&current_time);
      push(data.data());
    }
  }
}

long TimePermRingBuffer::read(int index, DataSample &data)
{
  get(index, (void*)data.data());
  long time;
  m_lastTimeStamp.readData((void*)&time);
  time -= ( -index % bufferSize() ) * m_period;
  return time;
}

uint16_t TimePermRingBuffer::storageSize()
{
  return m_eepromIndex.storageSize() + m_bufferLength + m_lastTimeStamp.storageSize();
}

long TimePermRingBuffer::lastTimeStamp()
{
  long time;
  m_lastTimeStamp.readData((void *)&time);
  return time;
}
