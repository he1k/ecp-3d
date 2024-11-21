#include "bridge.h"

Bridge::Bridge()
{
  
}
// Initialize class
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
  this->_cmd = CMD_NONE;
  this->_data = 0.0;
}

// Transmit a float in ascii by their hex value
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

// Writes data to the host. 
void Bridge::write(uint8_t stat, float enc1, float enc2, float enc3)
{
  this->_usb_port->print(stat);
  this->_usb_port->print(" ");
  this->_usb_port->print(enc1, DEC_PLCS);
  this->_usb_port->print(" ");
  this->_usb_port->print(enc2, DEC_PLCS);
  this->_usb_port->print(" ");
  this->_usb_port->print(enc3, DEC_PLCS);
  this->_usb_port->print("\n");
}

// Returns true if data from host is available
bool Bridge::data_valid()
{
  return this->_state == STATE_DATA_VALID;
}
// Reads the current message from Simulink (if available)
// Returns true if new data is available, otherwise false
bool Bridge::read()
{
  if(this->_state == STATE_DATA_VALID){
    this->_state = STATE_IDLE;
    char s1[16];
    int i;
    for(i = 0; i < BFR_SIZE; i++)
    {
    if(((char) this->_bfr[i]) == ' ')
    {
      break;
    }
    s1[i] = this->_bfr[i];
    }
    this->_cmd = atoi(s1);
    this->_data = atof((char*)(this->_bfr+i+1)); 
    return true;
  }else
  {
    return false;
  }
}
// Debug function to access the byte buffer
uint8_t Bridge::get_buffer(uint8_t i)
{
  return this->_bfr[i];
}

// ISR function for reading serial data
void Bridge::rx_ready()
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