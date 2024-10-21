#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#define BFR_SIZE 16
#include <stdio.h>
#include <stdint.h>
#include <math.h>
// Class for ring buffer
// No more than 255 entries are possible
// Size of each element is restricted to 8 bits
class RingBuffer
{
  private:
    uint8_t _bfr[BFR_SIZE];
    uint8_t _head;
    uint8_t _tail;
  public:
    uint8_t n;
    RingBuffer();
    bool empty();
    bool full();
    uint8_t available();
    bool write(uint8_t b);
    uint8_t read();
};




#endif