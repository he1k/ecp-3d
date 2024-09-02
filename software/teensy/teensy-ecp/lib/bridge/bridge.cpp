#include "bridge.h"

Bridge::Bridge()
{

}

void Bridge::begin()
{
  for(int i = 0; i < BFR_SIZE; i++)
  {
    this->bfr[i] = 0;
  }
  this->pending_cmd = 0;
  this->cmd_in = 0;
  this->data_in = 0;
  this->head = 0;
  this->tail = 0;
  this->missed_byte = false;
}
bool Bridge::emptyRX()
{
  return this->tail == this->head;
}
// Write an incoming byte from UART to the buffer
bool Bridge::writeBuffer()
{
  if((this->head == BFR_SIZE-1 && this->tail == 0) || (this->tail-this->head == 1)) // Buffer is full
  {
    this->missed_byte = true;
    digitalWrite(LED_BUILTIN, HIGH);
    return false;
  }
  this->head = this->head == BFR_SIZE-1 ? 0 : this->head + 1; // Increment write pointer
  uint8_t b = Serial.read();
  this->bfr[this->head] = b;
  if(b == LINE_FEED)
  {
    this->pending_cmd++;
  }
  return true;
}
// Read a byte from the buffer
uint8_t Bridge::readBuffer()
{
  if(this->emptyRX()) // Buffer is empty
  {
    return 0;
  }
  this->tail = this->tail == BFR_SIZE-1 ? 0 : this->tail + 1; // Increment write pointer
  uint8_t b = this->bfr[this->tail];
  if(b == LINE_FEED)
  {
    this->pending_cmd--;
  }
  return b;
}
// Writes one byte command anmd
bool Bridge::writeCommand(uint8_t cmd, uint8_t *data, uint8_t n_bytes)
{
  Serial.write(cmd);
  for(int i = 0; i < n_bytes; i++){
    Serial.write(data[i]);
  }
  Serial.write(LINE_FEED);
  return true;
}
bool Bridge::pending()
{
  return this->pending_cmd > 0;
}
bool Bridge::readCommand()
{
  if(this->pending_cmd == 0)
  {
    return false;
  }
  this->cmd_in = this->readBuffer();
  uint8_t b = this->readBuffer();
  uint8_t ba[4];
  uint8_t i = 0;
  while(b != LINE_FEED)
  {
    ba[i] = b;
    i++;
    b = this->readBuffer();
  }
  float *f = (float*)ba;
  this->data_in = 
}