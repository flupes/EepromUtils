#include "timeRingBufferTest.h"

#include "WProgram.h"

TimePermRingBuffer samples(START_ADDR, BUFFER_SZ, sizeof(FloatData), PERIOD);

long time = 0;
int step = 1;

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

  time += step;
  if ( 0 == time % 5  )  step++;
}

int main(void)
{
  init();
  
  setup();
  
  for (;;)
    loop();
  
  return 0;
}
