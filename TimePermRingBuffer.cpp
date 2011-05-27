#include "TimePermRingBuffer.h"

//#include <stdint.h>

#ifdef SERIAL_DEBUG
#include <HardwareSerial.h>
#endif

TimePermRingBuffer::TimePermRingBuffer(uint16_t startAddr, uint16_t bufferSize,
                                       size_t dataSize, int timePeriod,
                                       uint16_t endurFactor) :
  EepromRingBuffer(startAddr, bufferSize, dataSize, endurFactor),
  m_period(timePeriod),
  m_lastTimeStamp(startAddr+storageSize(), endurFactor, sizeof(long))
{
  // long time;
  // m_lastTimeStamp.readData((void *)&time);
  // if ( 0xFFFFFFFFl == time ) {
  //   time = INT32_MIN;
  //   m_lastTimeStamp.writeData((void *)&time);
  // }
}

int TimePermRingBuffer::period()
{
  return m_period;
}

void TimePermRingBuffer::setTimeStamp(long ts)
{
  m_lastTimeStamp.writeData((void *)&ts);
}

bool TimePermRingBuffer::insert(DataSample &data, long current_time)
{
  long last_time, delta, steps;
  m_lastTimeStamp.readData((void*)&last_time);
  delta = current_time - last_time;

#ifdef SERIAL_DEBUG
  Serial.print("==== TimePermRingBuffer::insert -> current=");
  Serial.print(current_time, DEC);
  Serial.print(" - last_time=");
  Serial.print(last_time, DEC);
  Serial.print(" : delta=");
  Serial.println(delta, DEC);
#endif

  if ( delta < 0 ) {
    // something happened: we are going back in the past
    // just let clear everything and start orer
#ifdef SERIAL_DEBUG
    Serial.println("------ back to the past -> clear");
#endif
    clear();
    m_lastTimeStamp.writeData((void*)&current_time);
    push(data.data());
    return true;
  }

  if ( delta > m_period * bufferSize() ) {
    // elapsed time greater than buffer time span
#ifdef SERIAL_DEBUG
    Serial.println("------ elapsed time greater than buffer time span -> clear");
#endif
    clear();
    m_lastTimeStamp.writeData((void*)&current_time);
    push(data.data());
  }
  else {
    if ( 0 == delta % m_period ) {
      steps = delta / m_period;
      if ( steps > 0 ) {
        // this is time to insert the new sample
        if ( steps > 1 ) {
          // elapsed time more than one single period
#ifdef SERIAL_DEBUG
          Serial.print("------ elapsed time more than a single period -> rotate steps = ");
          Serial.println(steps-1, DEC);
#endif
          rotate(steps-1);
        }
        m_lastTimeStamp.writeData((void*)&current_time);
        push(data.data());
      }      
    }
    else {
#ifdef SERIAL_DEBUG
      Serial.println("------ period not elapsed -> no insertion");
#endif
      return false;
    }
  }
  return true;
}

long TimePermRingBuffer::read(int index, DataSample &data)
{
  get(index, (void*)data.data());
  long time;
  m_lastTimeStamp.readData((void*)&time);
  time -= ( index % bufferSize() ) * m_period;
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
