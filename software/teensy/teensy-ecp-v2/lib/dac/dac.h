#ifndef DAC_H
#define DAC_H
#include <Arduino.h>
#include <SPI.h>
#include "../../src/constants.h"
// Device types
#define AD5721R 0 // 12 bit DAC with internal reference
#define AD5761R 1 // 16 bit DAC with internal reference
#define AD5721  2 // 12 bit DAC without internal reference
#define AD5761  3 // 16 bit DAC without internal reference
// Error types
#define NO_ERR 0
#define ERR_INVALID_DEVICE 1
#define ERR_INVALID_PARAM 2
#define ERR_PARAM_MISMATCH 3
#define ERR_COMMAND_RECIEVED 4
// Fault types. It is assumed that
// critical die temperature and brownout or short can be reported at the same time
#define NO_FAULT                              0
#define FAULT_DIE_TEMP                        1
#define FAULT_BROWNOUT                        2
#define FAULT_SHORT                           3
#define FAULT_BROWNOUT_AND_SHORT              4
// #define FAULT_DIE_TEMP_AND_BROWNOUT           3
// #define FAULT_SHORT                           4
// #define FAULT_DIE_TEMP_AND_SHORT              5
// #define FAULT_BROWNOUT_AND_SHORT              6
// #define FAULT_DIE_TEMP_AND_BROWNOUT_AND_SHORT 7

// Control register bit fields
// Clear voltage scale
#define CV_ZERO_SCALE 0x0 // AD5761: 0x0000, AD5721: 0x0000
#define CV_MID_SCALE  0x1 // AD5761: 0x00FF, AD5721: 0x003F
#define CV_FULL_SCALE 0x2 // AD5761: 0xFFFF, AD5721: 0x0FFF
// 5% output overrange
#define OVR_DISABLED  0x0 
#define OVR_ENABLED   0x1
// DAC register encoding
#define B2C_BINARY    0x0 // binary encoding
#define B2C_2CMP      0x1 // two's complement encoding
// Power down on thermal overload of the die (T > 150 C)
#define ETS_DISABLED  0x0
#define ETS_ENABLED   0x1
// Power up voltage scale
#define PV_ZERO_SCALE 0x0 // AD5761: 0x0000, AD5721: 0x0000
#define PV_MID_SCALE  0x1 // AD5761: 0x00FF, AD5721: 0x003F
#define PV_FULL_SCALE 0x2 // AD5761: 0xFFFF, AD5721: 0x0FFF
// Output range
#define RA_PM_10      0x0 // -10V to 10V 
#define RA_0_10       0x1 // 0V to 10V
#define RA_PM_5       0x2 // -5V to 5V
#define RA_0_5        0x3 // 0V to 5V
#define RA_M25_75     0x4 // -2.5V to 7.5V
#define RA_PM_3       0x5 // -3V to 3V
#define RA_0_16       0x6 // 0V to 16V
#define RA_0_20       0x7 // 0V to 20V
// Daisy-chain feature
#define DDC_ENABLED   0x0
#define DDC_DISABLED  0x1
// Internal reference (for model 5721R and 5761R only)
#define IRO_DISABLED  0x0
#define IRO_ENABLED   0x1

// Commands for input shift register
#define CMD_NOP_1                 0x0 // No operation
#define CMD_W_INPUT_REG           0x1 // Write input register
#define CMD_UPDATE_DAC_REG        0x2 // Load output register with contents of input register
#define CMD_W_DAC_REG             0x3 // Write input and output register
#define CMD_W_CTRL_REG            0x4 // Write control register
#define CMD_NOP_2                 0x5 // No operation
#define CMD_NOP_3                 0x6 // No operation
#define CMD_SW_DATA_RST           0x7 // Software data reset (software version of #CLEAR strobe)
#define CMD_RESERVED              0x8 // Reserved
#define CMD_SET_RESET_DAISY_CHAIN 0x9 // Set/reset daisy chain feature
#define CMD_R_INPUT_REG           0xA // Readback input register
#define CMD_R_DAC_REG             0xB // Readback output register
#define CMD_R_CTRL_REG            0xC // Readback control register
#define CMD_NOP_4                 0xD // No operation
#define CMD_NOP_5                 0xE // No operation
#define CMD_SW_FULL_RST           0xF // Software full reset (Software version of #RESET strobe)
class Dac
{
  private:
    SPIClass *_spi_port; // SPI port
    uint8_t _pin_nsync;  // Frame synchronization pin
    uint8_t _pin_nldac;  // Load DAC pin
    uint8_t _pin_nreset; // Reset pin
    uint8_t _pin_nclear; // Clear pin
    uint8_t _pin_nalert; // Alert pin
    uint8_t _dev_type;    // Type of device (12 or 16 bit)
    uint8_t _ra;         // Output voltage range
    uint8_t _pv;         // Power-up voltage
    uint8_t _iro;        // Internal reference
    uint8_t _ets;        // Thermal shutdown alert
    uint8_t _b2c;        // Binary encoding
    uint8_t _ovr;        // 5% overrange
    uint8_t _cv;         // #CLEAR voltage
    uint8_t _ddc;        // Daisy-chain functionality
    uint8_t _bfr[3];     // SPI data buffer
    uint8_t _bo;          // Brownout condition   
    uint8_t _sc;          // Short-circuit condition
    uint8_t _error;        // Debug error code
    uint8_t _config_valid; // Valid config - False if the config stored in object
                           // is not the same as the one stored in device
    void transfer();
    uint16_t encode_ctrl();
    bool is_params_equal(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv);
    bool is_params_valid(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv);
  public:
    Dac();
    void begin(SPIClass *spi_port, uint8_t dev_type, uint8_t pin_nsync, uint8_t pin_nldac, uint8_t pin_nreset, uint8_t pin_nclear, uint8_t pin_nalert);
    bool init_device(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv, uint8_t ddc);
    void hw_clear_output();
    void hw_reset();
    void sw_clear_output();
    void sw_reset();
    void disable_daisy_chain();
    uint8_t get_buffer(uint8_t i);
    bool read_config();
    bool write_config(uint8_t ra, uint8_t pv, uint8_t iro, uint8_t ets, uint8_t b2c, uint8_t ovr, uint8_t cv);
    void print_config();
    void print_buffer();
    void write_output(uint16_t in);
    uint8_t get_error();
    uint8_t get_fault();
    uint16_t read_input_reg();
    uint16_t read_output_reg();
};
#endif