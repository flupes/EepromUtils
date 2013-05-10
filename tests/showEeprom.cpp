/**
   Show a part of the EEPROM content and demonstrate how to read write to
   the EEPROM.
 */

#include <Arduino.h>

#include "SafeEeprom.h"

#define OFFSET 8
#define N_BYTES 16
#define N_WORDS 8
#define N_LONGS 4
#define N_BLOCKS 2

struct Block {
  int data[4];
};

void setup()
{
  unsigned int addr = OFFSET;

  unsigned int a;
  for (byte i = 0; i<N_BYTES; i++)
  {
    SafeEeprom::write_byte(addr, i);
    addr++;
  } 

  for (byte i=0; i<N_WORDS; i++)
  {
    SafeEeprom::write_word(addr, i+256);
    addr += sizeof(word);
  }

  for (byte i=0; i<N_LONGS; i++)
  {
    SafeEeprom::write_long(addr, (long)i+65536l);
    addr += sizeof(long);
  }

  Block blocks[N_BLOCKS];
  int num=-1000;
  for (int k=0; k<N_BLOCKS; k++) {
    for (int l=0; l<4; l++) {
      blocks[k].data[l] = num;
      num -= 250;
    }
  }
  for (byte i=0; i<N_BLOCKS; i++)
  {    
    SafeEeprom::write_block(addr, (void *)&blocks[i], sizeof(Block));
    addr += sizeof(Block);
  }

  Serial.begin(9600);
}

void loop()
{
  Serial.print("This board as ");
  Serial.print(SafeEeprom::memSize(), DEC);
  Serial.print(" bytes of EEPROM (Page Size = ");
  Serial.print(SafeEeprom::pageSize(), DEC);
  Serial.println(")");

#ifndef NDEBUG
  Serial.println("First 64 bytes of EEPROM:");
  SafeEeprom::show(0, 64);
#endif

  unsigned int addr = OFFSET;
  byte num8;
  word num16;
  long num32;
  Block block;

  Serial.print("Bytes: ");
  for (byte i=0; i<N_BYTES; i++) {
    num8 = SafeEeprom::read_byte(addr);
    Serial.print(num8, DEC);
    Serial.print("  ");
    addr++;
  }
  Serial.println();
  Serial.print("Words: ");
  for (byte i=0; i<N_WORDS; i++) {
    num16 = SafeEeprom::read_word(addr);
    Serial.print(num16, DEC);
    Serial.print("  ");
    addr += sizeof(word);
  }
  Serial.println();
  Serial.print("Longs: ");
  for (byte i=0; i<N_LONGS; i++) {
    num32 = SafeEeprom::read_long(addr);
    Serial.print(num32, DEC);
    Serial.print("  ");
    addr += sizeof(long);
  }
  Serial.println();
  Serial.println("Blocks: ");
  for (byte i=0; i<N_BLOCKS; i++) {
    SafeEeprom::read_block(addr, (void *)&block, 4);
    Serial.print("[");
    for (int j=0; j<4; j++) {
      Serial.print(block.data[j], DEC);
      if ( j<3 ) {
        Serial.print(",");
      } else {
        Serial.print("]");
      }
    }
    Serial.print("  ");
    addr += sizeof(Block);
  }
  Serial.println();

  delay(6000);  
}

int main(void)
{
  init();
  
  setup();
  
  for (;;)
    loop();
  
  return 0;
}
