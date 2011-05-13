/**
   First EEPROM clearing before using the Endurance Eeprom Test.
   
   This program has be run only once to initialize memory, and never again
   (as long as the parameters to create an EnduranceEeprom stucture does
   not change).
*/

#include <wiring.h>

#include "eepromRingBufferTest.h"

#include "EnduranceEeprom.h"

int main(void)
{
  init();
  
  uint16_t size = ENDURANCE*(sizeof(EnduranceEeprom::Status)
                             +sizeof(EepromRingBuffer::Indexes))
    + BUFFER_SZ * DATA_SZ;

  for (uint16_t i=START_ADDR; i<START_ADDR+size; i++) {
    SafeEeprom::write_byte(i, 0xFF);
  }
  
  return 0;
}
