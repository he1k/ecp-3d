#include "ring_buffer.h"

RingBuffer::RingBuffer()
{
  this->_head = 0;
  this->_tail = 0;
  for(uint8_t i = 0; i < BFR_SIZE; i++)
  {
    this->_bfr[i] = 0;
  }
  this->n = 0;
}

bool RingBuffer::empty()
{
  return this->_head == this->_tail;
}
bool RingBuffer::full()
{
  if(this->_head == BFR_SIZE - 1 && this->_tail == 0)
  {
    return true;
  }
  if(this->_tail > this->_head)
  {
    if(this->_tail - this->_head == 1)
    {
      return true;
    }
  }
  return false;
}
uint8_t RingBuffer::available()
{
  if(this->full())
  {
    return BFR_SIZE-1;
  }else if(this->empty())
  {
    return 0;
  }else
  {
    if(this->_tail < this->_head)
    {
      return this->_head - this->_tail;
    }else
    {
      return this->_head + (BFR_SIZE - this->_tail);
    }
  }
}
bool RingBuffer::write(uint8_t b)
{
  if(!this->full())
  {
    this->_bfr[this->_head] = b;
    this->_head++;
    if(this->_head > BFR_SIZE - 1)
    {
      this->_head = 0;
    }
    this->n++;
    return true;
  }
  return false;
}
uint8_t RingBuffer::read()
{
  uint8_t res = this->_bfr[this->_tail];
  if(!this->empty())
  {
    this->_tail++;
    if(this->_tail > BFR_SIZE - 1)
    {
      this->_tail = 0;
    }
    this->n--;
  }
  return res;
}