#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <math.h>
#include "dac.h"
// ------------------------------------------------ //
//                    USB serial                    //
// ------------------------------------------------ //
#define BAUD_RATE 12000000

// ------------------------------------------------ //
//                       GPIO                       //
// ------------------------------------------------ //
// DAC
#define PIN_SCLK 13  // DAC serial clock
#define PIN_SDI 11   // DAC serial input
#define PIN_SDO 12   // DAC serial output
#define PIN_NSYNC 14 // DAC frame synchronization
#define PIN_NRESET 19
#define PIN_NALERT 17
#define PIN_NLDAC 15
#define PIN_NCLEAR 16
// Encoders
#define CHA_ENC1   3 // Encoder channel for encoder #1
#define CHA_ENC2   2 // Encoder channel for encoder #2
#define CHA_ENC3   1 // Encoder channel for encoder #3
#define PIN_ENC1_A 8 // Phase A pin for encoder #1
#define PIN_ENC1_B 7 // Phase B pin for encoder #1
#define PIN_ENC2_A 3 // Phase A pin for encoder #2
#define PIN_ENC2_B 2 // Phase B pin for encoder #2
#define PIN_ENC3_A 0 // Phase A pin for encoder #3
#define PIN_ENC3_B 1 // Phase B pin for encoder #3

// Debug
#define PIN_DEBUG 38
// ------------------------------------------------ //
//                       SPI                        //
// ------------------------------------------------ //

#define SPI_MODE SPI_MODE2            // Idle clock high, latching data on falling edge and MSB transfered first
#define SPI_ENDIANNESS MSBFIRST       // DAC expects MSB to be send first, ie. big endian
#define SPI_CLOCK_DIV SPI_CLOCK_DIV4  // SPI clock divider. Default speed is 16 MHz.
#define DEL_NSYNC_US 1                // Delay for changing LSYNC before/after SPI transfer

// ------------------------------------------------ //
//                     Encoders                     //
// ------------------------------------------------ //

#define ENC_FILTER_T_US 10 // Encoder filters sample period
#define ENC_FILTER_N    5  // Encoder filter order
#define PPR 4000.0         // Encoder pulses per rotation
#define CPR 16000.0             // Encoder counts per rotation
#define C2D 360.0/CPR    // Encoder counts to degrees
#define C2R 2.0*M_PI/CPR // Encoder counts to radians
#define D2R M_PI/180.0   // Degrees to radians

// ------------------------------------------------ //
//                     RANDOM                       //
// ------------------------------------------------ //

#define TS_TEENSY    0.00025
#define TS_MATLAB    0.004
#define SIM_DELAY    3.0
#define DEC_PLCS     6
#define START_CMD    'B'
#define STOP_CMD     'S'
#define TORQUE_LIM 0.1f // Limit torque command (Nm)
#define TORQUE_MAX 2.0f // Maximum torque command (Nm)
#define DEC_PLCS 6      // Decimal places for 

// ------------------------------------------------ //
//                      DAC                         //
// ------------------------------------------------ //
#define DAC_CONF_RA  RA_PM_10
#define DAC_CONF_PV  PV_MID_SCALE
#define DAC_CONF_IRO IRO_ENABLED
#define DAC_CONF_ETS ETS_ENABLED
#define DAC_CONF_B2C B2C_BINARY
#define DAC_CONF_OVR OVR_DISABLED
#define DAC_CONF_CV  CV_MID_SCALE
#define DAC_CONF_DDC DDC_ENABLED 



#endif