#ifndef BRIDGE_H
#define BRIDGE_H
#include <Arduino.h>
#include "../../src/constants.h"
#define BFR_SIZE 128
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_DATA_VALID 2
#define LINE_FEED 0xA
#define CMD_NONE 0
#define CMD_START 1
#define CMD_STOP 2
#define BYTE_START 0x42
#define BYTE_STOP 0x53
class Bridge
{
  private:
    usb_serial_class *_usb_port;  // USB port
    uint8_t _bfr[BFR_SIZE];       // Byte buffer
    uint8_t _state;
    uint8_t _idx;
    void tx_float(float f);
    uint8_t on;
  public:
    Bridge();
    void begin(usb_serial_class *usb_port);
    void write_encoder(float enc1, float enc2, float enc3);
    void rx_ready_isr();
    bool data_valid();
    float read_data();
    uint8_t get_state();
    uint8_t get_buffer(uint8_t i);
    uint8_t read_command();
};




#endif