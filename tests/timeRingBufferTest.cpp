#include "timeRingBufferTest.h"

#include "WProgram.h"

TimePermRingBuffer samples(START_ADDR, BUFFER_SZ, sizeof(FloatData), PERIOD);

long times[] = { 0, 1, 2, 3, 4, 6, 8, 10, 11, 12, 14, 16, 19, 20, 22, 24, 29, 30, 32, 33, 34, 36, 38, 56, 58, 60 };
//long times[] = { 20, 22, 24, 29, 30, 32, 33, 34, 36, 38, 56, 58, 60 };

short counter = 0;

void printBuffer()
{
  FloatSample tmp;
  long t;

  Serial.print("ring buffer index=");
  Serial.print(samples.currentIndex(), DEC);
  Serial.print(" :: timestamp=");
  Serial.println(samples.lastTimeStamp(), DEC);
  for (int i=0; i<BUFFER_SZ; i++) {
    t = samples.read(i, tmp);
    Serial.print("  i=");
    Serial.print(i, DEC);
    Serial.print(" -> number=");
    Serial.print(tmp.get(), DEC);
    Serial.print(" (t=");
    Serial.print(t, DEC);
    Serial.println(")");
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.print("storage size = ");
  Serial.println(samples.storageSize(), DEC);
  printBuffer();
  delay(12000);
}

void loop()
{
  FloatSample fs;
  bool a;
  long time;
  
  time = times[counter % (sizeof(times)/sizeof(long))];
  fs.set(time/10.0);

  Serial.println("");

  a = samples.insert(fs, time);

  Serial.print("Time=");
  Serial.println(time, DEC);

  if ( a ) {
    printBuffer();
    delay(6000);
  }
  else {
    delay(3000);
  }

  counter++;
}

int main(void)
{
  init();
  
  setup();
  
  for (;;)
    loop();
  
  return 0;
}
