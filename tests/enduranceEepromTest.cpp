/**
   Test program to experiment with the EnduranceEeprom class.

   Lorenzo Flueckiger -- May 2011
*/

#include "enduranceEepromTest.h"
#include "AvrEeprom.h"

#include <Arduino.h>

Sample sample;

void setup(EnduranceEeprom &endurSample)
{
  endurSample.readData((void *)&sample);
  if ( 0xFFFF == sample.a  ) {
    sample.a = 0;
    sample.b = 0;
  }
  Serial.print("Sample recovered on EEPROM = [");
  Serial.print(sample.a, DEC);
  Serial.print(", ");
  Serial.print(sample.b, DEC);
  Serial.println("]");
}

void loop(EnduranceEeprom &endurSample)
{
  Sample check;

  sample.a++;
  sample.b+=10;
  endurSample.writeData((void *)&sample);
  delay(100);
  check.a = 0;
  check.b = 0;
  endurSample.readData((void *)&check);
  AvrEeprom::instance().show(EESTART, endurSample.storageSize());
  Serial.print("Current Sample = [");
  Serial.print(check.a, DEC);
  Serial.print(", ");
  Serial.print(check.b, DEC);
  Serial.println("]");
  delay(3000);  
}

int main(void)
{
  init();

  Serial.begin(9600);
  delay(3000);
  EnduranceEeprom endurSample(AvrEeprom::instance(), EESTART, ENDURANCE, sizeof(Sample));
  AvrEeprom::instance().show(EESTART, endurSample.storageSize());
  
  setup(endurSample);
  
  for (;;)
    loop(endurSample);

  return 0;
}
