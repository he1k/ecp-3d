#ifndef BRIDGE_H
#define BRIDGE_H
#include <Arduino.h>
#include "../../src/constants.h"
#include "../../src/serial_commands.h"
class Bridge
{
  private:
    uint8_t bfr[BFR_SIZE];
    uint8_t head, tail;
    int pending_cmd;
    bool missed_byte;
    int cmd_in;
    float data_in;
  public:
    Bridge();
    void begin();
    bool emptyRX();
    uint8_t readBuffer();
    bool writeBuffer();
    bool Bridge::writeCommand(uint8_t cmd, uint8_t *data, uint8_t n_bytes);
    bool pending();
    bool readCommand();
};
#endif