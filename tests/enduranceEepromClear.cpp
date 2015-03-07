/**
   First EEPROM clearing before using the Endurance Eeprom Test.
   
   This program has be run only once to initialize memory, and never again
   (as long as the parameters to create an EnduranceEeprom stucture does
   not change).
*/

#include <Arduino.h>

#include "enduranceEepromTest.h"
#include "AvrEeprom.h"

int main(void)
{
  init();
  
  uint16_t size = ENDURANCE*(sizeof(EnduranceEeprom::Status)+sizeof(Sample));
  for (uint16_t i=EESTART; i<EESTART+size; i++) {
    AvrEeprom::instance().write_byte(i, 0xFF);
  }
  
  return 0;
}
