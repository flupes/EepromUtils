/**
   Test program to experiment with the EepromRingBuffer

   Lorenzo Flueckiger -- May 2011
*/

#include "eepromRingBufferTest.h"

#include "WProgram.h"

EepromRingBuffer ring(START_ADDR, BUFFER_SZ, DATA_SZ, ENDURANCE);
int number;

uint16_t index(int i)
{
  uint16_t dataSize = 2;
  uint16_t bufferLength = 12;
  if ( i < 0 ) {
    i = -dataSize*i;
    i = i % bufferLength;
    return bufferLength - i;
  }
  else {
    return (i*dataSize)%bufferLength;
  }
}

void setup()
{
  Serial.begin(9600);
  // for (int i=0; i<BUFFER_SZ; i++) {
  //   Serial.print((-2*i)%12, DEC);
  //   Serial.print(" -- i="); 
  //   Serial.print(i, DEC);
  //   Serial.print(" + ");
  //   Serial.print(index(i), DEC);
  //   Serial.print(" / - ");
  //   Serial.println(index(-i), DEC);
  // }
  // Let a chance to open a serial console before the program 
  // actually starts to write data.
  delay(9000);
  number=1;
  Serial.print("---- Ring Buffer Storage Size = ");
  Serial.print(ring.storageSize(), DEC);
  Serial.print(" | buffer size = ");
  Serial.println(ring.bufferSize(), DEC);
#ifndef NDEBUG
  Serial.println("==== Initial Eeprom State ====");
  SafeEeprom::show(START_ADDR, ring.storageSize());  
#endif
}

void loop()
{
  Serial.print("==== Iteration: ");
  Serial.println(number, DEC);
  if ( number % 5 == 0 ) {
    ring.rotate(2);
    number += 2;
  }
  else {
    ring.push((void *)&number);
    number++;
  }

#ifndef NDEBUG
  SafeEeprom::show(START_ADDR, ring.storageSize());
#endif

  Serial.print("---- ring buffer index=");
  Serial.println(ring.currentIndex(), DEC);

  int tmp;
  for (int i=0; i<BUFFER_SZ; i++) {
    ring.get(i, (void *)&tmp);
    Serial.print("  i=");
    Serial.print(i, DEC);
    Serial.print(" -> number=");
    Serial.println(tmp, DEC);
  }
  ring.get(-1, (void *)&tmp);
  Serial.print("  older element = ");
  Serial.println(tmp, DEC);
  delay (9000);
}

int main(void)
{
  init();
  
  setup();
  
  for (;;)
    loop();
  
  return 0;
}
