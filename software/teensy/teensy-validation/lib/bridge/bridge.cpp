#include "bridge.h"
Bridge::Bridge()
{
  
}

void Bridge::begin(usb_serial_class *usb_port)
{
  this->_usb_port = usb_port;
  this->_usb_port->begin(BAUD_RATE);
  for(uint8_t i = 0; i < BFR_SIZE; i++) // Zero out buffer
  {
    this->_bfr[i] = 0;
  }
  this->_state = STATE_IDLE;
  this->_idx = 0;
}

void Bridge::tx_float(float f)
{
  uint8_t b[4];
  memcpy(b, &f, 4);
  this->_usb_port->print(b[3], HEX);
  if(!(b[2] & 0xf0)){
    this->_usb_port->print(0);
  }
  this->_usb_port->print(b[2], HEX);
  if(!(b[1] & 0xf0)){
    this->_usb_port->print(0);
  }
  this->_usb_port->print(b[1], HEX);
  if(!(b[0] & 0xf0)){
    this->_usb_port->print(0);
  }
  this->_usb_port->print(b[0], HEX);
}

void Bridge::write_encoder(float enc1, float enc2, float enc3)
{
  //this->tx_float(enc1);
  this->_usb_port->print(enc1, DEC_PLCS);
  this->_usb_port->print(" ");
  //this->tx_float(enc2);
  this->_usb_port->print(enc2, DEC_PLCS);
  this->_usb_port->print(" ");
  //this->tx_float(enc3);
  this->_usb_port->print(enc3, DEC_PLCS);
  this->_usb_port->print("\n");
}

// Returns true if data from host is available
bool Bridge::data_valid()
{
  return this->_state == STATE_DATA_VALID;
}

float Bridge::read_data()
{
  if(this->_state == STATE_DATA_VALID){
    this->_state = STATE_IDLE;
    return atof((char*)this->_bfr);
  }else
  {
    return 0.0f;
  }
}

uint8_t Bridge::get_state()
{
  return this->_state;
}

uint8_t Bridge::get_buffer(uint8_t i)
{
  return this->_bfr[i];
}

uint8_t Bridge::read_command()
{
  if(this->_state != STATE_DATA_VALID || this->_idx > 1)
  {
    return CMD_NONE;
  }else if(this->_bfr[0] == BYTE_START)
  {
    this->_state = STATE_IDLE;
    return CMD_START;
  }else if(this->_bfr[0] == BYTE_STOP)
  {
    this->_state = STATE_IDLE;
    return CMD_STOP;
  }
  return CMD_NONE;
}

// ISR function for reading serial data
void Bridge::rx_ready_isr()
{
  if(this->_state == STATE_DATA_VALID) // Must read current valid data
  {
    return;
  }
  if(this->_state == STATE_IDLE)
  {
    this->_idx = 0;
    this->_state = STATE_READING;
  }
  while(this->_usb_port->available())
  {
    uint8_t b = this->_usb_port->read();
    if(b == LINE_FEED)
    {
      this->_state = STATE_DATA_VALID;
      for(uint8_t i = this->_idx; i < BFR_SIZE; i++)
      {
        this->_bfr[i] = 0;
      }
    }else
    {
      this->_bfr[this->_idx] = b;
      this->_idx++;
    }
  }
}