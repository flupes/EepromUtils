#include "TimePermRingBuffer.h"
#include "DataSample.h"

#define START_ADDR 512
#define BUFFER_SZ 8
#define PERIOD 2

struct FloatData {
  float number;
};

class FloatSample : public DataSample
{
public:
  FloatSample() : DataSample(sizeof(FloatData)) {
    
  }

  void *data() {
    return (void *)&storage;
  }

  float get() {
    return storage.number;
  }

  void set(float f) {
    storage.number = f;
  }

protected:
  FloatData storage;

};

