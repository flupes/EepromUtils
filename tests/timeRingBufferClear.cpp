#include "timeRingBufferTest.h"
#include "SafeEeprom.h"

#include <Arduino.h>

int main(void)
{

  init();

  for (uint16_t i=START_ADDR; i<START_ADDR+160; i++) {
    SafeEeprom::write_byte(i, 0xFF);
  }

  return 0;

};
