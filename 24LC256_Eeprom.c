/*
 * EEPROM_PAGE_WRITE
 *
 * Example program showing a method for writing large amounts of
 * data to an eeprom (24LC256/512 etc), whilst avoiding page write
 * boundaries
 *
 * copyright www.hobbytronics.co.uk 2012
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version. see <http://www.gnu.org/licenses/>
 *
 * PROBLEM: There are two methods for writing data to an eeprom
 * 1. Write a single byte. Apart from the overhead of sending the
 *    eeprom memory address, the eeprom needs 3ms for each write to
 *    complete. Thus if you send 100 bytes it needs 300ms
 * 2. Write an array of bytes (called Page write). Up to 64 bytes 
 *    can be sent before the eeprom needs to save the data. 
 *    The save only needs 5ms thus saving considerable time. 
 * There is however a problem with method 2. The eeprom's memory is
 * split into 64byte chunks (or pages) and the page write can only write
 * within one page. Attempting to write past a page boundary will lead
 * to the the memory pointer wrapping back to the start of the page
 * and data at the beginning will be overwritten.
 *
 * The solution provided here calculates the remaining space in the
 * initial page that is being written to and breaks up the write into
 * separate writes to make the data fit the page boudaries.
 *
 * E.g. We want to write 78 bytes starting at memory location 55 (ending
 * at memory location 132). This overlaps into 3 pages.
 * 0                               64                               128 
 * |------------page----------------|--------------page--------------| 
 *                            |------------------data-------------------|
 *                            55                                       132 
 * The program below will split the write up as follows
 * - write 9 bytes  ( 55 to  63) - up to page boundary
 * - write 16 bytes ( 64 to  79)
 * - write 16 bytes ( 80 to  95)
 * - write 16 bytes ( 96 to 111)
 * - write 16 bytes (112 to 127) - up to page boundary
 * - write 5 bytes  (128 to 132)
 *
 * Why 16 bytes and not 32 or 64? Arduino uses a 32 byte buffer for sending
 * and receiving data so this is the maximum you can send. But this buffer
 * needs to include the memory location bytes which reduces the available 
 * space to 30 bytes. So 16 bytes is used as an easy divisor of 64.
 *
 * The overhead for these 6 writes              = 5ms x 6    = 30ms
 * The overhead for writing 78 individual bytes = 3.5ms x 78 = 273ms
*/

#include <Wire.h>
#define eeprom1 0x50    //Address of 24LC256 eeprom chip
#define WRITE_CNT 5

unsigned char rdata[32];

void setup(void)
{
  Serial.begin(9600);
  Wire.begin();  
 
  unsigned int i;
  // define large string of data to be written
  char str_data[]={"Hello-1234567890-and-abcdefghijklmnopqrstuvwxyz-Goodbye\n"};

  // Work out length of data
  char str_len=0;  
  do{ str_len++; } while(str_data[str_len]);  
  
  // Write out data several times consecutively starting at address 0
  for(i=0;i<WRITE_CNT;i++) writeEEPROM(eeprom1,i*str_len,str_data);

  // read back the data 28 bytes at a time
  // reading data doesn't suffer from the page boundary rules
  Serial.println("DATA READ");
  for(i=0;i<10;i++) {
    readEEPROM(eeprom1, (i*28), rdata, 28);
    Serial.write(rdata,28);
  }  

}
 
void loop(){
}
 
void writeEEPROM(int deviceaddress, unsigned int eeaddress, char* data) 
{
  // Uses Page Write for 24LC256
  // Allows for 64 byte page boundary
  // Splits string into max 16 byte writes
  unsigned char i=0, counter=0;
  unsigned int  address;
  unsigned int  page_space;
  unsigned int  page=0;
  unsigned int  num_writes;
  unsigned int  data_len=0;
  unsigned char first_write_size;
  unsigned char last_write_size;  
  unsigned char write_size;  
  
  // Calculate length of data
  do{ data_len++; } while(data[data_len]);   
   
  // Calculate space available in first page
  page_space = int(((eeaddress/64) + 1)*64)-eeaddress;

  // Calculate first write size
  if (page_space>16){
     first_write_size=page_space-((page_space/16)*16);
     if (first_write_size==0) first_write_size=16;
  }   
  else 
     first_write_size=page_space; 
    
  // calculate size of last write  
  if (data_len>first_write_size) 
     last_write_size = (data_len-first_write_size)%16;   
  
  // Calculate how many writes we need
  if (data_len>first_write_size)
     num_writes = ((data_len-first_write_size)/16)+2;
  else
     num_writes = 1;  
     
  i=0;   
  address=eeaddress;
  for(page=0;page<num_writes;page++) 
  {
     if(page==0) write_size=first_write_size;
     else if(page==(num_writes-1)) write_size=last_write_size;
     else write_size=16;
  
     Wire.beginTransmission(deviceaddress);
     Wire.write((int)((address) >> 8));   // MSB
     Wire.write((int)((address) & 0xFF)); // LSB
     counter=0;
     do{ 
        Wire.write((byte) data[i]);
        i++;
        counter++;
     } while((data[i]) && (counter<write_size));  
     Wire.endTransmission();
     address+=write_size;   // Increment address for next write
     
     delay(6);  // needs 5ms for page write
  }
}
 
void readEEPROM(int deviceaddress, unsigned int eeaddress,  
                 unsigned char* data, unsigned int num_chars) 
{
  unsigned char i=0;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,num_chars);
 
  while(Wire.available()) data[i++] = Wire.read();

}

