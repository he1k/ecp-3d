#ifndef BRIDGE_H
#define BRIDGE_H
#include <Arduino.h>
#include "../../src/constants.h"
#define BFR_SIZE 32
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_DATA_VALID 2
#define LINE_FEED 0xA
#define CMD_NONE 1
#define CMD_START 2
#define BYTE_START 0x42
#define BYTE_STOP 0x53
class Bridge
{
  private:
    usb_serial_class *_usb_port;  // USB port
    uint8_t _bfr[BFR_SIZE];       // Byte buffer
    uint8_t _state;               // State variable
    uint8_t _idx;                 // Buffer index
    void tx_float(float f);       // Unused function
  public:
    uint8_t _cmd;                 // Command / integer part of message
    float _data;                  // Actual data from message
    Bridge();
    void begin(usb_serial_class *usb_port);
    void rx_ready();
    bool data_valid();
    bool read();
    uint8_t get_buffer(uint8_t i);
    void write(uint8_t stat, float enc1, float enc2, float enc3);
};




#endif