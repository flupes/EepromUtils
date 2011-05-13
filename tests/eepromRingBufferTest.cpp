/**
   Test program to experiment with the EepromRingBuffer

   Lorenzo Flueckiger -- May 2011
*/

#include "eepromRingBufferTest.h"

#include "WProgram.h"

EepromRingBuffer ring(START_ADDR, BUFFER_SZ, DATA_SZ, ENDURANCE);
int number;

void setup()
{
  // ring.clear();
  delay(12000);
  number=1;
  Serial.begin(9600);
  SafeEeprom::show(START_ADDR, ring.storageSize());  
  ring.rotate(1);
}

void loop()
{
  Serial.println(number, DEC);
  SafeEeprom::show(START_ADDR, ring.storageSize());
  if ( number % 5 == 0 ) {
    ring.rotate(2);
    number += 2;
  }
  else {
    ring.push((void *)&number);
    number++;
  }
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
