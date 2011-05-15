#ifndef DataSample_h
#define DataSample_h

class DataSample
{
public:
  DataSample(uint8_t size) {
    m_size = size;
  }

  uint8_t size() {
    return m_size;
  }

//  float value(uint8_t index);

  virtual void *data() = 0;
  
protected:
  uint8_t m_size;

};

#endif


