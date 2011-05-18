/**
   Test program to compare the EEPROM access speed with SafeEeprom
   versus avr direct eeprom calls.
   
   The program simply does repetitive writes to the EEPROM using both
   methods. Since the write operation itself takes a huge amount of time,
   there is not detectable different between the direct avr calls and the
   SafeEeprom calls.

   On my Uno board (ATmega238p, 16MHz), I get:
   eeprom_write_dword :     13ms
   SafeEeprom::write_long : 13ms

   Of course, the reads are much faster, and here this trivial test rig
   shows its limitations (the compiler is probably too smart and optimize
   the code in a way that the operations may be skipped).

   On my Uno board again, I then get a huge difference:
   eeprom_read_dword:       0us (not meaningful!)
   SafeEeprom::read_long :  6us

   More tests would be required to measure the degradation of the read
   using the SafeEeprom class, however, if a 10us read is not an issue for
   application, you should be fine. And there is not penalty for writes
   since the overhead is negligible compared to the burning time itself.

  */
#include "SafeEeprom.h"

#include <avr/eeprom.h>
#include <wiring.h>
#include <HardwareSerial.h>

const unsigned int size = sizeof(long);
const unsigned int length = (SafeEeprom::memSize()+1)/size;

void printElapsed(unsigned int start, unsigned long stop,
                  unsigned long ops, const char *str)
{
  long elapsed = stop - start;
  Serial.print("Time for ");
  Serial.print(length, DEC);
  Serial.print(str);
  Serial.print(elapsed, DEC);
  Serial.print("ms -> ");
  Serial.print(1000*elapsed/ops, DEC);
  Serial.println("us/op.");
}

int main(void)
{
  unsigned long start, stop;

  init();
  Serial.begin(9600);

  long value = 0xFFFFFFFFl;

  start=millis();
  for (unsigned int i=0; i<length; i++) {
    eeprom_write_dword((uint32_t*)(i*4), value);
  }
  stop=millis();
  printElapsed(start, stop, length, " direct writes: ");

  start=millis();
  for (unsigned int i=0; i<length; i++) {
    SafeEeprom::write_long(i*4, value);
  }
  stop=millis();
  printElapsed(start, stop, length, " safe writes: ");

  long tmp;

  start=millis();
  for (int k=0; k<10; k++) {
    for (unsigned int i=0; i<length; i++) {
      value = eeprom_read_dword((uint32_t*)(i*4));
      tmp = value % 10;
    }
  }
  stop=millis();
  printElapsed(start, stop, 10*length, " direct reads: ");

  start=millis();
  for (int k=0; k<10; k++) {
    for (unsigned int i=0; i<length; i++) {
      value = SafeEeprom::read_long(i*4);
      tmp = value % 10;
    }
  }
  stop=millis();
  printElapsed(start, stop, 10*length, " safe reads: ");

  return 0;
}



