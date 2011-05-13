/**
   Test program to experiment with the EnduranceEeprom class.

   Lorenzo Flueckiger -- May 2011
*/

#include "enduranceEepromTest.h"
#include "SafeEeprom.h"

#include <WProgram.h>

EnduranceEeprom endurSample(EESTART, ENDURANCE, sizeof(Sample));

Sample sample;

void setup()
{
  sample.a = 0;
  sample.b = 0;
  Serial.begin(9600);
}

void loop()
{
  Sample check;

  sample.a--;
  sample.b++;
  endurSample.writeData((void *)&sample);
  delay(100);
  check.a = 0;
  check.b = 0;
  endurSample.readData((void *)&check);
  SafeEeprom::show(EESTART, endurSample.storageSize());
  Serial.print("Current Sample = [");
  Serial.print(check.a, DEC);
  Serial.print(", ");
  Serial.print(check.b, DEC);
  Serial.println("]");
  // Second read just for fun
  endurSample.readData((void *)&check);

  delay(12000);  
}

int main(void)
{
  init();

  setup();
  
  for (;;)
    loop();

  return 0;
}
