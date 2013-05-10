#include "SafeEeprom.h"

#include "Arduino.h"

int main(void)
{
  init();

  for (uint16_t i=0; i<SafeEeprom::memSize(); i++) {
    SafeEeprom::write_byte(i, 0xFF);
  }

  return 0;
}
